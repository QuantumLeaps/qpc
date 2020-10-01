/**
* @file
* @brief QF/C port to Win32 API (single-threaded, like the QV kernel)
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
    InitializeCriticalSection(&l_win32CritSect);
    QV_win32Event_ = CreateEvent(NULL, FALSE, FALSE, NULL);
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

    if (l_tickMsec != 0U) { /* system clock tick configured? */
        /* create the ticker thread... */
        HANDLE ticker = CreateThread(NULL, 1024, &ticker_thread,
                                    (void *)0, 0, NULL);
        Q_ASSERT_ID(310, ticker != (HANDLE)0); /* thread must be created */
    }

    /* the combined event-loop and background-loop of the QV kernel */
    QF_CRIT_E_();

    /* produce the QS_QF_RUN trace record */
    QS_BEGIN_NOCRIT_PRE_(QS_QF_RUN, 0U)
    QS_END_NOCRIT_PRE_()

    while (l_isRunning) {
        /* find the maximum priority AO ready to run */
        if (QPSet_notEmpty(&QV_readySet_)) {
            uint_fast8_t p;
            QActive *a;
            QEvt const *e;

            QPSet_findMax(&QV_readySet_, p);
            a = QF_active_[p];
            QF_CRIT_X_();

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
            QHSM_DISPATCH(&a->super, e, a->prio);
            QF_gc(e);

            QF_CRIT_E_();

            if (a->eQueue.frontEvt == (QEvt *)0) { /* empty queue? */
                QPSet_remove(&QV_readySet_, p);
            }
        }
        else {
            /* the QV kernel in embedded systems calls here the QV_onIdle()
            * callback. However, the Win32-QV port does not do busy-waiting
            * for events. Instead, the Win32-QV port efficiently waits until
            * QP events become available.
            */
            QF_CRIT_X_();

            (void)WaitForSingleObject(QV_win32Event_, (DWORD)INFINITE);

            QF_CRIT_E_();
        }
    }
    QF_CRIT_X_();
    QF_onCleanup();  /* cleanup callback */
    QS_EXIT();       /* cleanup the QSPY connection */

    return 0; /* return success */
}
/****************************************************************************/
void QF_setTickRate(uint32_t ticksPerSec, int_t tickPrio) {
    if (ticksPerSec != 0U) {
        l_tickMsec = 1000UL / ticksPerSec;
    }
    else {
        l_tickMsec = 0U; /* means NO system clock tick */
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
        return (int)_getwch();
    }
    return 0;
}
/*..........................................................................*/
int QF_consoleWaitForKey(void) {
    return (int)_getwch();
}

/* QActive functions =======================================================*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    (void)stkSize;   /* unused parameter in the Win32-QV port */

    Q_REQUIRE_ID(600, (0U < prio) /* priority must be in range */
        && (prio <= QF_MAX_ACTIVE)
        && (stkSto == (void *)0));    /* statck storage must NOT...
                                       * ... be provided */

    QEQueue_init(&me->eQueue, qSto, qLen);
    me->prio = prio; /* set QF priority of this AO before adding it to QF */
    QF_add_(me);     /* make QF aware of this AO */

    /* the top-most initial tran. (virtual) */
    QHSM_INIT(&me->super, par, me->prio);
    QS_FLUSH(); /* flush the trace buffer to the host */
}
/*..........................................................................*/
#ifdef QF_ACTIVE_STOP
void QActive_stop(QActive * const me) {
    QF_CRIT_STAT_

    QActive_unsubscribeAll(me); /* unsubscribe from all events */

    /* make sure the AO is no longer in "ready set" */
    QF_CRIT_E_();
    QPSet_remove(&QV_readySet_, me->prio);
    QF_CRIT_X_();

    QF_remove_(me); /* remove this AO from QF */
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
static DWORD WINAPI ticker_thread(LPVOID arg) { /* for CreateThread() */
    int threadPrio = THREAD_PRIORITY_NORMAL;

    /* set the ticker thread priority according to selection made in
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
        QF_onClockTick();  /* clock tick callback (must call QF_TICK_X()) */
    }

    (void)arg; /* unused parameter */

    return 0U; /* return success */
}

