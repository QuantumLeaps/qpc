/**
* \file
* \ingroup qf
* \brief QF/C port to Win32.
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-03-01
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* \endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#include "qassert.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_port")

/* Local objects ************************************************************/
static CRITICAL_SECTION l_win32CritSect;
static DWORD l_tickMsec = 10; /* clock tick in msec (argument for Sleep()) */
static int_t l_running;

static DWORD WINAPI thread_function(LPVOID arg);

/****************************************************************************/
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
    l_running = (int_t)0;
}
/****************************************************************************/
int_t QF_run(void) {
    QF_onStartup();  /* startup callback */

    /* if necessary, raise the priority of this (main) thread
    * to tick more timely
    */
    /*SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);*/
    l_running = (int_t)1;
    while (l_running) {
        Sleep(l_tickMsec); /* wait for the tick interval */
        QF_onClockTick();  /* clock tick callback (must call QF_TICKX()) */
    }
    QF_onCleanup();  /* cleanup callback */
    QS_EXIT();       /* cleanup the QSPY connection */
    //DeleteCriticalSection(&l_win32CritSect);
    return (int_t)0; /* return success */
}
/****************************************************************************/
void QF_setTickRate(uint32_t ticksPerSec) {
    l_tickMsec = 1000UL / ticksPerSec;
}

/*==========================================================================*/
/**
* \description
* Port-specific active Object start operation.
*
* \arguments
* \arg[in,out] \c me      pointer (see \ref derivation)
* \arg[in]     \c prio    priority at which to start the active object
* \arg[in]     \c qSto    pointer to the storage for the ring buffer of the
*                         event queue (used only with the built-in ::QEQueue)
* \arg[in]     \c qLen    length of the event queue (in events)
* \arg[in]     \c stkSto  pointer to the stack storage (used only when
*                         per-AO stack is needed)
* \arg[in]     \c stkSize stack size (in bytes)
* \arg[in]     \c ie      pointer to the optional initialization event
*                         (might be NULL).
*
* \note This QActive_start_() implementation is just an example and different
* QF ports might define this function differently.
*/
/****************************************************************************/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    DWORD threadId;
    int p;

    Q_REQUIRE((qSto != (QEvt const **)0) /* queue storage must be provided */
       && (stkSto == (void *)0)); /* Windows allocates stack internally */

    me->prio = prio;
    QF_add_(me); /* make QF aware of this active object */

    QEQueue_init(&me->eQueue, qSto, qLen);
    me->osObject = CreateEvent(NULL, FALSE, FALSE, NULL);
    QMSM_INIT(&me->super, ie); /* execute initial transition */

    /* stack size not provided? */
    if (stkSize == 0U) {
        stkSize = 1024U; /* NOTE: will be rounded up to the nearest page */
    }
    me->thread = CreateThread(NULL, stkSize,
                              &thread_function, me, 0, &threadId);
    Q_ASSERT(me->thread != (HANDLE)0); /* thread must be created */

    switch (me->prio) { /* remap QF priority to Win32 priority */
        case 1:
            p = THREAD_PRIORITY_IDLE;
            break;
        case 2:
            p = THREAD_PRIORITY_LOWEST;
            break;
        case 3:
            p = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case (QF_MAX_ACTIVE - 1):
            p = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case QF_MAX_ACTIVE:
            p = THREAD_PRIORITY_HIGHEST;
            break;
        default:
            p = THREAD_PRIORITY_NORMAL;
            break;
    }
    SetThreadPriority(me->thread, p);
}
/****************************************************************************/
void QActive_stop(QActive * const me) {
    me->thread = (HANDLE)0; /* stop the event loop in thread_function() */
}
/****************************************************************************/
static DWORD WINAPI thread_function(LPVOID arg) { /* for CreateThread() */
    /* QActive_stop() stops the loop */
    do {
        QEvt const *e = QActive_get_((QActive *)arg); /* wait for event */
        QMSM_DISPATCH((QMsm *)arg, e);  /* dispatch to the AO's SM */
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    } while (((QActive *)arg)->thread != (HANDLE)0);

    QF_remove_((QActive *)arg);/* remove this object from any subscriptions */
    CloseHandle(((QActive *)arg)->osObject); /* cleanup the OS event */
    return 0; /* return success */
}
