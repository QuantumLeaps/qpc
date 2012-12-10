/*****************************************************************************
* Product:  QF/C port to POSIX/P-threads, GNU
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jul 25, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qf_pkg.h"
#include "qassert.h"

#include <sys/mman.h>                                     /* for mlockall() */
#include <sys/select.h>

Q_DEFINE_THIS_MODULE("qf_port")

/* Global objects ----------------------------------------------------------*/
pthread_mutex_t QF_pThreadMutex_ = PTHREAD_MUTEX_INITIALIZER;

/* Local objects -----------------------------------------------------------*/
static long int l_tickUsec = 10000UL;   /* clock tick in usec (for tv_usec) */
static uint8_t l_running;

/*..........................................................................*/
const char *QF_getPortVersion(void) {
    static const char Q_ROM version[] =  "4.5.02";
    return version;
}
/*..........................................................................*/
void QF_init(void) {
                          /* lock memory so we're never swapped out to disk */
    /*mlockall(MCL_CURRENT | MCL_FUTURE);          uncomment when supported */
}
/*..........................................................................*/
int16_t QF_run(void) {
    struct sched_param sparam;
    struct timeval timeout = { 0 };                 /* timeout for select() */

    QF_onStartup();                              /* invoke startup callback */

           /* try to maximize the priority of the ticker thread, see NOTE01 */
    sparam.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sparam) == 0) {
                     /* success, this application has sufficient privileges */
    }
    else {
         /* setting priority failed, probably due to insufficient privieges */
    }

    l_running = (uint8_t)1;
    while (l_running) {
        QF_onClockTick();      /* clock tick callback (must call QF_TICK()) */

        timeout.tv_usec = l_tickUsec;      /* set the desired tick interval */
        select(0, 0, 0, 0, &timeout); /* sleep for the desired tick, NOTE05 */
    }
    QF_onCleanup();                              /* invoke cleanup callback */
    pthread_mutex_destroy(&QF_pThreadMutex_);

    return (int16_t)0;                                    /* return success */
}
/*..........................................................................*/
void QF_setTickRate(uint32_t ticksPerSec) {
    l_tickUsec = 1000000UL / ticksPerSec;
}
/*..........................................................................*/
void QF_stop(void) {
    l_running = (uint8_t)0;                    /* stop the loop in QF_run() */
}
/*..........................................................................*/
static void *thread_routine(void *arg) {    /* the expected POSIX signature */
    QActive *act = (QActive *)arg;
    do {                /* loop until m_thread is cleared in QActive_stop() */
        QEvt const *e = QActive_get_(act);            /* wait for the event */
        QF_ACTIVE_DISPATCH_(&act->super, e);             /* dispatch to  SM */
        QF_gc(e);    /* check if the event is garbage, and collect it if so */
    } while (act->thread != (uint8_t)0);
    QF_remove_(act);           /* remove this object from any subscriptions */
    pthread_cond_destroy(&act->osObject); /* cleanup the condition variable */
    return (void *)0;                                     /* return success */
}
/*..........................................................................*/
void QActive_start(QActive *me, uint8_t prio,
                  QEvt const *qSto[], uint32_t qLen,
                  void *stkSto, uint32_t stkSize,
                  QEvt const *ie)
{
    pthread_t thread;
    pthread_attr_t attr;
    struct sched_param param;

    Q_REQUIRE(stkSto == (void *)0);  /* p-threads allocate stack internally */

    QEQueue_init(&me->eQueue, qSto, (QEQueueCtr)qLen);
    pthread_cond_init(&me->osObject, 0);

    me->prio = prio;
    QF_add_(me);                     /* make QF aware of this active object */
    QF_ACTIVE_INIT_(&me->super, ie);      /* execute the initial transition */

    QS_FLUSH();                       /* flush the trace buffer to the host */

    pthread_attr_init(&attr);

    /* SCHED_FIFO corresponds to real-time preemptive priority-based scheduler
    * NOTE: This scheduling policy requires the superuser privileges
    */
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
                                                              /* see NOTE04 */
    param.sched_priority = prio
                           + (sched_get_priority_max(SCHED_FIFO)
                              - QF_MAX_ACTIVE - 3);

    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (pthread_create(&thread, &attr, &thread_routine, me) != 0) {
               /* Creating the p-thread with the SCHED_FIFO policy failed.
               * Most probably this application has no superuser privileges,
               * so we just fall back to the default SCHED_OTHER policy
               * and priority 0.
               */
        pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
        param.sched_priority = 0;
        pthread_attr_setschedparam(&attr, &param);
        Q_ALLEGE(pthread_create(&thread, &attr, &thread_routine, me)== 0);
    }
    pthread_attr_destroy(&attr);
    me->thread = (uint8_t)1;
}
/*..........................................................................*/
void QActive_stop(QActive *me) {
    me->thread = (uint8_t)0;                /* stop the QActive thread loop */
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
* three highest Linux priorities for the ISR-like threads (e.g., the ticker,
* I/O), and the rest highest-priorities for the active objects.
*
* NOTE05:
* The select() system call seems to deliver the finest time granularity of
* 1 clock tick. The timeout value passed to select() is rounded up to the
* nearest tick (10 ms on desktop Linux). The timeout cannot be too short,
* because the system might choose to busy-wait for very short timeouts.
* An alternative, POSIX nanosleep() system call seems to deliver only 20ms
* granularity.
*
* Here the select() call is used not just as a fairly portable way to sleep
* with subsecond precision. The select() call is also used to detect any
* characters typed on the console.
*
* Also according to man pages, on Linux, the function select() modifies
* timeout to reflect the amount of time not slept; most other implementations
* do not do this. This causes problems both when Linux code which reads
* timeout is ported to other operating systems, and when code is ported to
* Linux that reuses a struct timeval for multiple selects in a loop without
* reinitializing it. Here the microsecond part of the structure is re-
* initialized before each select() call.
*/

