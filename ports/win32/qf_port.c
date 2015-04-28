/**
* @file
* @brief QF/C port to Win32 API
* @ingroup qf
* @cond
******************************************************************************
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-04-25
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
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

Q_DEFINE_THIS_MODULE("qf_port")


/* Local objects ===========================================================*/
static CRITICAL_SECTION l_win32CritSect;
static DWORD l_tickMsec = 10U; /* clock tick in msec (argument for Sleep()) */
static bool  l_isRunning;      /* flag indicating when QF is running */

static DWORD WINAPI ao_thread(LPVOID arg);

/* QF functions ============================================================*/
void QF_init(void) {
    InitializeCriticalSection(&l_win32CritSect);
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

    QF_onStartup(); /* application-specific startup callback */

    l_isRunning = true; /* QF is running */

    /* set the ticker thread priority below normal to prevent
    * flooding other threads with time events when the machine
    * is very busy.
    */
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);

    while (l_isRunning) {
        Sleep(l_tickMsec); /* wait for the tick interval */
        QF_onClockTick();  /* clock tick callback (must call QF_TICKX()) */
    }
    QF_onCleanup();  /* cleanup callback */
    QS_EXIT();       /* cleanup the QSPY connection */
    //DeleteCriticalSection(&l_win32CritSect);
    //free all "fudged" event pools...
    return (int_t)0; /* return success */
}
/****************************************************************************/
void QF_setWin32Prio(QActive *act, int_t win32Prio) {
    if (act->thread == (HANDLE)0) {  /* thread not created yet? */
        act->osObject = (void *)win32Prio; /* store the priority for later */
    }
    else {
        SetThreadPriority(act->thread, win32Prio);
    }
}
/****************************************************************************/
void QF_setTickRate(uint32_t ticksPerSec) {
    l_tickMsec = 1000UL / ticksPerSec;
}

/* QActive functions =======================================================*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    DWORD threadId;
    int   win32Prio;
    void *fudgedQSto;
    uint_fast16_t fudgedQLen;

    Q_REQUIRE_ID(700, ((uint_fast8_t)0 < prio) /* priority must be in range */
                 && (prio <= (uint_fast8_t)QF_MAX_ACTIVE)
                 && (qSto != (QEvt const **)0) /* queue storage must be... */
                 && (qLen > (uint_fast16_t)0)  /* ...provided */
                 && (stkSto == (void *)0));    /* statck storage must NOT...
                                               * ... be provided */

    me->prio = prio; /* set QF priority of this AO before adding it to QF */
    QF_add_(me);     /* make QF aware of this active object */

    /* ignore the original storage for the event queue 'qSto' and
    * instead allocate an oversized "fudged" storage for the queue.
    * See also NOTE2 in qf_port.h.
    */
    Q_ASSERT_ID(710, (uint32_t)qLen * QF_WIN32_FUDGE_FACTOR < USHRT_MAX);
    fudgedQLen = qLen * QF_WIN32_FUDGE_FACTOR; /* fudge the queue length */
    fudgedQSto = calloc(fudgedQLen, sizeof(QEvt *)); /* new queue storage */
    Q_ASSERT_ID(720, fudgedQSto != (void *)0); /* allocation must succeed */
    QEQueue_init(&me->eQueue, (QEvt const **)fudgedQSto, fudgedQLen);

    /* save osObject as integer, in case it contains the Win32 priority */
    win32Prio = (int)me->osObject;

    /* create the Win32 "event" to throttle the AO's event queue */
    me->osObject = CreateEvent(NULL, FALSE, FALSE, NULL);

    QMSM_INIT(&me->super, ie); /* take the top-most initial tran. */
    QS_FLUSH(); /* flush the QS trace buffer to the host */

    /* stack size not provided? */
    if (stkSize == 0U) {
        stkSize = 1024U; /* NOTE: will be rounded up to the nearest page */
    }

    /* create a Win32 thread for the AO;
    * The thread is created with THREAD_PRIORITY_NORMAL
    */
    me->thread = CreateThread(NULL, stkSize,
                              &ao_thread, me, 0, &threadId);
    Q_ASSERT(me->thread != (HANDLE)0); /* thread must be created */

    /* was the thread priority provided? */
    if (win32Prio != 0) {
        SetThreadPriority(me->thread, win32Prio);
    }
}
/****************************************************************************/
void QActive_stop(QActive * const me) {
    QActive_unsubscribeAll(me);
    QF_remove_(me);

    me->thread = (HANDLE)0; /* stop the AO event loop in thread_function() */
}
/****************************************************************************/
static DWORD WINAPI ao_thread(LPVOID arg) { /* for CreateThread() */
    QActive *act = (QActive *)arg;

    /* Active Object's event loop. QActive_stop() stops the loop */
    do {
        QEvt const *e = QActive_get_(act); /* wait for event */
        QMSM_DISPATCH(&act->super, e);     /* dispatch to the AO's SM */
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    } while (act->thread != (HANDLE)0);

    QF_remove_(act);  /* remove this object from any subscriptions */
    CloseHandle(act->osObject); /* cleanup the OS event */
    free((void *)act->eQueue.ring); /* free the fudged queue storage */
    return (DWORD)0; /* return success */
}
