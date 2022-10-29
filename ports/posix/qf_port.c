/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-08-29
* @version Last updated for: @ref qpc_7_1_0
*
* @file
* @brief QF/C port to POSIX API (multi-threaded)
* @ingroup ports
*/

/* expose features from the 2008 POSIX standard (IEEE Standard 1003.1-2008) */
#define _POSIX_C_SOURCE 200809L

#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#include "qassert.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS package-scope internal interface */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

#include <limits.h>       /* for PTHREAD_STACK_MIN */
#include <sys/mman.h>     /* for mlockall() */
#include <sys/select.h>
#include <sys/ioctl.h>
#include <string.h>       /* for memcpy() and memset() */
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

Q_DEFINE_THIS_MODULE("qf_port")

/* Global objects ==========================================================*/
pthread_mutex_t QF_pThreadMutex_; /* mutex for QF critical section */

/* Local objects ===========================================================*/
static pthread_mutex_t l_startupMutex;
static bool l_isRunning;      /* flag indicating when QF is running */
static struct termios l_tsav; /* structure with saved terminal attributes */
static struct timespec l_tick;
static int_t l_tickPrio;
enum { NANOSLEEP_NSEC_PER_SEC = 1000000000 }; /* see NOTE05 */

static void sigIntHandler(int dummy);

/* QF functions ============================================================*/
void QF_init(void) {
    struct sigaction sig_act;

    /* lock memory so we're never swapped out to disk */
    /*mlockall(MCL_CURRENT | MCL_FUTURE);  uncomment when supported */

    /* init the global mutex with the default non-recursive initializer */
    pthread_mutex_init(&QF_pThreadMutex_, NULL);

    /* init the startup mutex with the default non-recursive initializer */
    pthread_mutex_init(&l_startupMutex, NULL);

    /* lock the startup mutex to block any active objects started before
    * calling QF_run()
    */
    pthread_mutex_lock(&l_startupMutex);

    l_tick.tv_sec = 0;
    l_tick.tv_nsec = NANOSLEEP_NSEC_PER_SEC/100L; /* default clock tick */
    l_tickPrio = sched_get_priority_min(SCHED_FIFO); /* default tick prio */

    /* install the SIGINT (Ctrl-C) signal handler */
    memset(&sig_act, 0, sizeof(sig_act));
    sig_act.sa_handler = &sigIntHandler;
    sigaction(SIGINT, &sig_act, NULL);
}

/****************************************************************************/
void QF_enterCriticalSection_(void) {
    pthread_mutex_lock(&QF_pThreadMutex_);
}
/****************************************************************************/
void QF_leaveCriticalSection_(void) {
    pthread_mutex_unlock(&QF_pThreadMutex_);
}

/****************************************************************************/
int_t QF_run(void) {
    struct sched_param sparam;

    QF_onStartup();  /* invoke startup callback */

    /* produce the QS_QF_RUN trace record */
    QS_BEGIN_NOCRIT_PRE_(QS_QF_RUN, 0U)
    QS_END_NOCRIT_PRE_()

    /* try to set the priority of the ticker thread, see NOTE01 */
    sparam.sched_priority = l_tickPrio;
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sparam) == 0) {
        /* success, this application has sufficient privileges */
    }
    else {
        /* setting priority failed, probably due to insufficient privieges */
    }

    /* unlock the startup mutex to unblock any active objects started before
    * calling QF_run()
    */
    pthread_mutex_unlock(&l_startupMutex);

    l_isRunning = true;
    while (l_isRunning) { /* the clock tick loop... */
        QF_onClockTick(); /* callback (must call QTIMEEVT_TICK_X()) */

        nanosleep(&l_tick, NULL); /* sleep for the number of ticks, NOTE05 */
    }
    QF_onCleanup(); /* invoke cleanup callback */
    pthread_mutex_destroy(&l_startupMutex);
    pthread_mutex_destroy(&QF_pThreadMutex_);

    return 0; /* return success */
}
/*..........................................................................*/
void QF_setTickRate(uint32_t ticksPerSec, int_t tickPrio) {
    Q_REQUIRE_ID(300, ticksPerSec != 0U);
    l_tick.tv_nsec = NANOSLEEP_NSEC_PER_SEC / ticksPerSec;
    l_tickPrio = tickPrio;
}
/*..........................................................................*/
void QF_stop(void) {
    l_isRunning = false; /* stop the loop in QF_run() */
}

/*..........................................................................*/
void QF_consoleSetup(void) {
    struct termios tio;   /* modified terminal attributes */

    tcgetattr(0, &l_tsav); /* save the current terminal attributes */
    tcgetattr(0, &tio);    /* obtain the current terminal attributes */
    tio.c_lflag &= ~(ICANON | ECHO); /* disable the canonical mode & echo */
    tcsetattr(0, TCSANOW, &tio);     /* set the new attributes */
}
/*..........................................................................*/
void QF_consoleCleanup(void) {
    tcsetattr(0, TCSANOW, &l_tsav); /* restore the saved attributes */
}
/*..........................................................................*/
int QF_consoleGetKey(void) {
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    if (byteswaiting > 0) {
        char ch;
        read(0, &ch, 1);
        return (int)ch;
    }
    return 0; /* no input at this time */
}
/*..........................................................................*/
int QF_consoleWaitForKey(void) {
    return getchar();
}

/****************************************************************************/
static void *thread_routine(void *arg) { /* the expected POSIX signature */
    QActive *act = (QActive *)arg;

    /* block this thread until the startup mutex is unlocked from QF_run() */
    pthread_mutex_lock(&l_startupMutex);
    pthread_mutex_unlock(&l_startupMutex);

#ifdef QF_ACTIVE_STOP
    act->thread = true;
    while (act->thread)
#else
    for (;;) /* for-ever */
#endif
    {
        QEvt const *e = QActive_get_(act); /* wait for the event */
        QHSM_DISPATCH(&act->super, e, act->prio); /* dispatch to the HSM */
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }
#ifdef QF_ACTIVE_STOP
    QActive_unregister_(act); /* un-register this active object */
#endif
    return (void *)0; /* return success */
}

/****************************************************************************/
void QActive_start_(QActive * const me, QPrioSpec const prioSpec,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    pthread_t thread;
    pthread_attr_t attr;
    struct sched_param param;
    int err;

    /* p-threads allocate stack internally */
    Q_REQUIRE_ID(600, stkSto == (void *)0);

    QEQueue_init(&me->eQueue, qSto, qLen);
    pthread_cond_init(&me->osObject, NULL);

    me->prio  = (uint8_t)(prioSpec & 0xFFU); /* QF-priority of the AO */
    me->pthre = (uint8_t)(prioSpec >> 8U);   /* preemption-threshold */
    QActive_register_(me); /* register this AO */

    /* the top-most initial tran. (virtual) */
    QHSM_INIT(&me->super, par, me->prio);
    QS_FLUSH(); /* flush the trace buffer to the host */

    pthread_attr_init(&attr);

    /* SCHED_FIFO corresponds to real-time preemptive priority-based scheduler
    * NOTE: This scheduling policy requires the superuser privileges
    */
    pthread_attr_setschedpolicy (&attr, SCHED_FIFO);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);

    /* priority of the p-thread, see NOTE04 */
    param.sched_priority = me->prio
                           + (sched_get_priority_max(SCHED_FIFO)
                              - QF_MAX_ACTIVE - 3U);
    pthread_attr_setschedparam(&attr, &param);

    pthread_attr_setstacksize(&attr, (stkSize < PTHREAD_STACK_MIN
                                      ? PTHREAD_STACK_MIN
                                      : stkSize));

    err = pthread_create(&thread, &attr, &thread_routine, me);
    if (err != 0) {
        /* Creating p-thread with the SCHED_FIFO policy failed. Most likely
        * this application has no superuser privileges, so we just fall
        * back to the default SCHED_OTHER policy and priority 0.
        */
        pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
        param.sched_priority = 0;
        pthread_attr_setschedparam(&attr, &param);
        err = pthread_create(&thread, &attr, &thread_routine, me);
    }
    Q_ASSERT_ID(610, err == 0); /* AO thread must be created */

    //pthread_attr_getschedparam(&attr, &param);
    //printf("param.sched_priority==%d\n", param.sched_priority);

    pthread_attr_destroy(&attr);
}
/*..........................................................................*/
#ifdef QF_ACTIVE_STOP
void QActive_stop(QActive * const me) {
    QActive_unsubscribeAll(me); /* unsubscribe this AO from all events */
    me->thread = false; /* stop the thread loop (see thread_routine()) */
}
#endif
/*..........................................................................*/
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    (void)me;    /* unused parameter */
    (void)attr1; /* unused parameter */
    (void)attr2; /* unused parameter */
    Q_ERROR_ID(900); /* this function should not be called in this QP port */
}

/****************************************************************************/
static void sigIntHandler(int dummy) {
    (void)dummy; /* unused parameter */
    QF_onCleanup();
    exit(-1);
}

/*****************************************************************************
* NOTE01:
* In Linux, the scheduler policy closest to real-time is the SCHED_FIFO
* policy, available only with superuser privileges. QF_run() attempts to set
* this policy as well as to maximize its priority, so that the ticking
* occurrs in the most timely manner (as close to an interrupt as possible).
* However, setting the SCHED_FIFO policy might fail, most probably due to
* insufficient privileges.
*
* NOTE02:
* On some Linux systems nanosleep() might actually not deliver the finest
* time granularity. For example, on some Linux implementations, nanosleep()
* could not block for shorter intervals than 20ms, while the underlying
* clock tick period was only 10ms. Sometimes, the select() system call can
* provide a finer granularity.
*
* NOTE03:
* Any blocking system call, such as nanosleep() or select() system call can
* be interrupted by a signal, such as ^C from the keyboard. In this case this
* QF port breaks out of the event-loop and returns to main() that exits and
* terminates all spawned p-threads.
*
* NOTE04:
* According to the man pages (for pthread_attr_setschedpolicy) the only value
* supported in the Linux p-threads implementation is PTHREAD_SCOPE_SYSTEM,
* meaning that the threads contend for CPU time with all processes running on
* the machine. In particular, thread priorities are interpreted relative to
* the priorities of all other processes on the machine.
*
* This is good, because it seems that if we set the priorities high enough,
* no other process (or thread running within) can gain control over the CPU.
*
* However, QF limits the number of priority levels to QF_MAX_ACTIVE.
* Assuming that a QF application will be real-time, this port reserves the
* three highest p-thread priorities for the ISR-like threads (e.g., I/O),
* and the rest highest-priorities for the active objects.
*
* NOTE05:
* In some (older) Linux kernels, the POSIX nanosleep() system call might
* deliver only 2*actual-system-tick granularity. To compensate for this,
* you would need to reduce the constant NANOSLEEP_NSEC_PER_SEC by factor 2.
*/

