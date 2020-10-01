/**
* @file
* @brief QF/C port to Win32 API
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-18
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS package-scope internal interface */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

#include <limits.h>       /* limits of dynamic range for integers */
#include <conio.h>        /* console input/output */

Q_DEFINE_THIS_MODULE("qf_port")

/* Local objects ===========================================================*/
static CRITICAL_SECTION l_win32CritSect;
static CRITICAL_SECTION l_startupCritSect;
static DWORD l_tickMsec = 10U; /* clock tick in msec (argument for Sleep()) */
static int_t l_tickPrio = 50;  /* default priority of the "ticker" thread */
static bool  l_isRunning;      /* flag indicating when QF is running */

static DWORD WINAPI ao_thread(LPVOID arg);

/* QF functions ============================================================*/
void QF_init(void) {
    InitializeCriticalSection(&l_win32CritSect);

    /* initialize and enter the startup critical section object to block
    * any active objects started before calling QF_run()
    */
    InitializeCriticalSection(&l_startupCritSect);
    EnterCriticalSection(&l_startupCritSect);
}
/****************************************************************************/
void QF_enterCriticalSection_(void) {
    EnterCriticalSection(&l_win32CritSect);
}
/****************************************************************************/
void QF_leaveCriticalSection_(void) {
    LeaveCriticalSection(&l_win32CritSect);
}
/****************************************************************************/
void QF_stop(void) {
    l_isRunning = false;  /* terminate the main (ticker) thread */
}
/****************************************************************************/
int_t QF_run(void) {
    int threadPrio = THREAD_PRIORITY_NORMAL;


    QF_onStartup(); /* application-specific startup callback */

    /* produce the QS_QF_RUN trace record */
    QS_BEGIN_NOCRIT_PRE_(QS_QF_RUN, 0U)
    QS_END_NOCRIT_PRE_()

    /* leave the startup critical section to unblock any active objects
    * started before calling QF_run()
    */
    LeaveCriticalSection(&l_startupCritSect);

    l_isRunning = true; /* QF is running */

    /* set the ticker (this thread) priority according to selection made in
    * QF_setTickRate()
    */
    if (l_tickPrio < 33) {
        threadPrio = THREAD_PRIORITY_BELOW_NORMAL;
    }
    else if (l_tickPrio > 66) {
        threadPrio = THREAD_PRIORITY_ABOVE_NORMAL;
    }
    SetThreadPriority(GetCurrentThread(), threadPrio);

    while (l_isRunning) {
        Sleep(l_tickMsec); /* wait for the tick interval */
        QF_onClockTick();  /* clock tick callback (must call QF_TICKX()) */
    }

    QF_onCleanup();  /* cleanup callback */
    QS_EXIT();       /* cleanup the QSPY connection */
    /*DeleteCriticalSection(&l_startupCritSect);*/
    /*DeleteCriticalSection(&l_win32CritSect);*/
    return 0; /* return success */
}
/****************************************************************************/
void QF_setTickRate(uint32_t ticksPerSec, int_t tickPrio) {
    Q_REQUIRE_ID(600, ticksPerSec != 0U);
    l_tickMsec = 1000UL / ticksPerSec;
    l_tickPrio = tickPrio;
}
/****************************************************************************/
void QF_setWin32Prio(QActive *act, int_t win32Prio) {
    HANDLE win32thread = (HANDLE)act->thread;

    /* thread must be already created, see QActive_start_() */
    Q_REQUIRE_ID(700, win32thread != (HANDLE)0);
    SetThreadPriority(win32thread, win32Prio);
}

/* QActive functions =======================================================*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    Q_REQUIRE_ID(800, (0U < prio) /* priority must be in range */
                 && (prio <= QF_MAX_ACTIVE)
                 && (stkSto == (void *)0));   /* statck storage must NOT...
                                               * ... be provided */
    me->prio = prio; /* set QF priority of this AO before adding it to QF */
    QF_add_(me);     /* make QF aware of this active object */

    QEQueue_init(&me->eQueue, qSto, qLen);

    /* create the Win32 "event" to throttle the AO's event queue */
    me->osObject = CreateEvent(NULL, FALSE, FALSE, NULL);

    /* the top-most initial tran. (virtual) */
    QHSM_INIT(&me->super, par, me->prio);
    QS_FLUSH(); /* flush the trace buffer to the host */

    /* create a Win32 thread for the AO;
    * The thread is created with THREAD_PRIORITY_NORMAL
    */
    me->thread = CreateThread(
        NULL,
        (stkSize < 1024U ? 1024U : stkSize),
        &ao_thread,
        me,
        0,
        NULL);
    Q_ENSURE_ID(830, me->thread != (HANDLE)0); /* must succeed */
}
/*..........................................................................*/
#ifdef QF_ACTIVE_STOP
void QActive_stop(QActive * const me) {
    QActive_unsubscribeAll(me); /* unsubscribe this AO from all events */
    me->thread = (void *)0; /* stop the thread loop (see ao_thread()) */
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
void QF_consoleSetup(void) {
}
/*..........................................................................*/
void QF_consoleCleanup(void) {
}
/*..........................................................................*/
int QF_consoleGetKey(void) {
    if (_kbhit()) { /* any key pressed? */
        return (int)_getwch();
    }
    return 0;
}
/*..........................................................................*/
int QF_consoleWaitForKey(void) {
    return (int)_getwch();
}

/****************************************************************************/
static DWORD WINAPI ao_thread(LPVOID arg) { /* for CreateThread() */
    QActive *act = (QActive *)arg;

    /* block this thread until the startup critical section is exited
    * from QF_run() */
    EnterCriticalSection(&l_startupCritSect);
    LeaveCriticalSection(&l_startupCritSect);

#ifdef QF_ACTIVE_STOP
    while (act->thread)
#else
    for (;;) /* for-ever */
#endif
    {
        QEvt const *e = QActive_get_(act); /* wait for event */
        QHSM_DISPATCH(&act->super, e, act->prio); /* dispatch to the SM */
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }
#ifdef QF_ACTIVE_STOP
    QF_remove_(act); /* remove this object from QF */
#endif
    return (DWORD)0; /* return success */
}

