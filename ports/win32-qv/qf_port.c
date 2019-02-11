/**
* @file
* @brief QF/C port to Win32 API (single-threaded, like the QV kernel)
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.4.0
* Last updated on  2019-02-07
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* https://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

#include <limits.h>       /* limits of dynamic range for integers */
#include <conio.h>        /* console input/output */

Q_DEFINE_THIS_MODULE("qf_port")

/* Global objects ==========================================================*/
QPSet  QV_readySet_;   /* QV-ready set of active objects */
HANDLE QV_win32Event_; /* Win32 event to signal events */

/* Local objects ===========================================================*/
static CRITICAL_SECTION l_win32CritSect;
static DWORD l_tickMsec = 10U; /* clock tick in msec (argument for Sleep()) */
static int_t l_tickPrio = 50;  /* default priority of the "ticker" thread */
static bool  l_isRunning;      /* flag indicating when QF is running */

static DWORD WINAPI ticker_thread(LPVOID arg);

/* QF functions ============================================================*/
void QF_init(void) {
    extern uint_fast8_t QF_maxPool_;
    extern QTimeEvt QF_timeEvtHead_[QF_MAX_TICK_RATE];

    InitializeCriticalSection(&l_win32CritSect);
    QV_win32Event_ = CreateEvent(NULL, FALSE, FALSE, NULL);

    /* clear the internal QF variables, so that the framework can (re)start
    * correctly even if the startup code is not called to clear the
    * uninitialized data (as is required by the C Standard).
    */
    QF_maxPool_ = (uint_fast8_t)0;
    QF_bzero(&QF_timeEvtHead_[0], (uint_fast16_t)sizeof(QF_timeEvtHead_));
    QF_bzero(&QF_active_[0],      (uint_fast16_t)sizeof(QF_active_));
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
    l_isRunning = false;  /* terminate the QV event loop and ticker thread */
    SetEvent(QV_win32Event_); /* unblock the event-loop so it can terminate */
}
/****************************************************************************/
int_t QF_run(void) {
    QF_CRIT_STAT_

    QF_onStartup(); /* application-specific startup callback */

    l_isRunning = true; /* QF is running */

    if (l_tickMsec != (uint32_t)0) { /* system clock tick configured? */
        /* create the ticker thread... */
        HANDLE ticker = CreateThread(NULL, 1024, &ticker_thread,
                                    (void *)0, 0, NULL);
        Q_ASSERT_ID(310, ticker != (HANDLE)0); /* thread must be created */
    }

    /* the combined event-loop and background-loop of the QV kernel */
    QF_CRIT_ENTRY_();
    while (l_isRunning) {
        /* find the maximum priority AO ready to run */
        if (QPSet_notEmpty(&QV_readySet_)) {
            uint_fast8_t p;
            QActive *a;
            QEvt const *e;

            QPSet_findMax(&QV_readySet_, p);
            a = QF_active_[p];
            QF_CRIT_EXIT_();

            /* the active object 'a' must still be registered in QF
            * (e.g., it must not be stopped)
            */
            Q_ASSERT_ID(320, a != (QActive *)0);

            /* perform the run-to-completion (RTS) step...
            * 1. retrieve the event from the AO's event queue, which by this
            *    time must be non-empty and The "Vanialla" kernel asserts it.
            * 2. dispatch the event to the AO's state machine.
            * 3. determine if event is garbage and collect it if so
            */
            e = QActive_get_(a);
            QHSM_DISPATCH(&a->super, e);
            QF_gc(e);

            QF_CRIT_ENTRY_();

            if (a->eQueue.frontEvt == (QEvt const *)0) { /* empty queue? */
                QPSet_remove(&QV_readySet_, p);
            }
        }
        else {
            /* the QV kernel in embedded systems calls here the QV_onIdle()
            * callback. However, the Win32-QV port does not do busy-waiting
            * for events. Instead, the Win32-QV port efficiently waits until
            * QP events become available.
            */
            QF_CRIT_EXIT_();

            (void)WaitForSingleObject(QV_win32Event_, (DWORD)INFINITE);

            QF_CRIT_ENTRY_();
        }
    }
    QF_CRIT_EXIT_();
    QF_onCleanup();  /* cleanup callback */
    QS_EXIT();       /* cleanup the QSPY connection */

    //CloseHandle(QV_win32Event_);
    //DeleteCriticalSection(&l_win32CritSect);
    //free all "fudged" event pools...
    return (int_t)0; /* return success */
}
/****************************************************************************/
void QF_setTickRate(uint32_t ticksPerSec, int_t tickPrio) {
    if (ticksPerSec != (uint32_t)0) {
        l_tickMsec = 1000UL / ticksPerSec;
    }
    else {
        l_tickMsec = (uint32_t)0; /* means NO system clock tick */
    }
    l_tickPrio = tickPrio;
}

/*..........................................................................*/
void QF_consoleSetup(void) {
}
/*..........................................................................*/
void QF_consoleCleanup(void) {
}
/*..........................................................................*/
int QF_consoleGetKey(void) {
    if (_kbhit()) { /* any key pressed? */
        return _getch();
    }
    return 0;
}
/*..........................................................................*/
int QF_consoleWaitForKey(void) {
    return _getch();
}

/* QActive functions =======================================================*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    Q_REQUIRE_ID(600, ((uint_fast8_t)0 < prio) /* priority must be in range */
        && (prio <= (uint_fast8_t)QF_MAX_ACTIVE)
        && (stkSto == (void *)0));    /* statck storage must NOT...
                                       * ... be provided */

    QEQueue_init(&me->eQueue, qSto, qLen);
    me->prio = prio; /* set QF priority of this AO before adding it to QF */
    QF_add_(me);     /* make QF aware of this AO */

    QHSM_INIT(&me->super, ie); /* take the top-most initial tran. */
    QS_FLUSH();      /* flush the QS trace buffer to the host */

    (void)stkSize;   /* unused parameter */
}

/****************************************************************************/
static DWORD WINAPI ticker_thread(LPVOID arg) { /* for CreateThread() */
    int threadPrio = THREAD_PRIORITY_NORMAL;

    // set the ticker thread priority according to selection made in
    // QF_setTickRate()
    //
    if (l_tickPrio < 33) {
        threadPrio = THREAD_PRIORITY_BELOW_NORMAL;
    }
    else if (l_tickPrio > 66) {
        threadPrio = THREAD_PRIORITY_ABOVE_NORMAL;
    }

    SetThreadPriority(GetCurrentThread(), threadPrio);

    while (l_isRunning) {
        Sleep(l_tickMsec); /* wait for the tick interval */
        QF_onClockTick();  /* clock tick callback (must call QF_TICK_X()) */
    }

    (void)arg; /* unused parameter */

    return (DWORD)0; /* return success */
}

