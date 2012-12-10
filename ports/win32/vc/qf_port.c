/*****************************************************************************
* Product: QF/C port to Win32
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jun 30, 2012
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

Q_DEFINE_THIS_MODULE("qf_port")

/* Local objects -----------------------------------------------------------*/
static CRITICAL_SECTION l_win32CritSect;
static DWORD l_tickMsec = 10;  /* clock tick in msec (argument for Sleep()) */
static uint8_t l_running;

static DWORD WINAPI thread_function(LPVOID arg);

/*..........................................................................*/
void QF_init(void) {
    InitializeCriticalSection(&l_win32CritSect);
}
/*..........................................................................*/
void QF_enterCriticalSection_(void) {
    EnterCriticalSection(&l_win32CritSect);
}
/*..........................................................................*/
void QF_leaveCriticalSection_(void) {
    LeaveCriticalSection(&l_win32CritSect);
}
/*..........................................................................*/
void QF_stop(void) {
    l_running = (uint8_t)0;
}
/*..........................................................................*/
int16_t QF_run(void) {
    QF_onStartup();                                     /* startup callback */

            /* raise the priority of this (main) thread to tick more timely */
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
    l_running = (uint8_t)1;
    while (l_running) {
        Sleep(l_tickMsec);                    /* wait for the tick interval */
        QF_onClockTick();      /* clock tick callback (must call QF_TICK()) */
    }
    QF_onCleanup();                                     /* cleanup callback */
    QS_EXIT();                               /* cleanup the QSPY connection */
    //DeleteCriticalSection(&l_win32CritSect);
    return (int16_t)0;                                    /* return success */
}
/*..........................................................................*/
void QF_setTickRate(uint32_t ticksPerSec) {
    l_tickMsec = 1000UL / ticksPerSec;
}
/*..........................................................................*/
void QActive_start(QActive * const me, uint8_t prio,
                  QEvt const *qSto[], uint32_t qLen,
                  void *stkSto, uint32_t stkSize,
                  QEvt const *ie)
{
    DWORD threadId;
    int p;

    Q_REQUIRE((qSto != (QEvt const **)0)  /* queue storage must be provided */
       && (stkSto == (void *)0));     /* Windows allocates stack internally */

    me->prio = prio;
    QF_add_(me);                     /* make QF aware of this active object */

    QEQueue_init(&me->eQueue, qSto, (QEQueueCtr)qLen);
    me->osObject = CreateEvent(NULL, FALSE, FALSE, NULL);
    QF_ACTIVE_INIT_(&me->super, ie);          /* execute initial transition */

    /* NOTE: if stkSize==0, Windows allocates default stack size */
    me->thread = CreateThread(NULL, stkSize,
                                &thread_function, me, 0, &threadId);
    Q_ASSERT(me->thread != (HANDLE)0);            /* thread must be created */

    switch (me->prio) {              /* remap QF priority to Win32 priority */
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
/*..........................................................................*/
void QActive_stop(QActive * const me) {
    me->thread = (HANDLE)0;     /* stop the event loop in thread_function() */
}
/*..........................................................................*/
static DWORD WINAPI thread_function(LPVOID arg) {     /* for CreateThread() */
    do {                                   /* QActive_stop() stops the loop */
        QEvt const *e = QActive_get_((QActive *)arg);     /* wait for event */
        QF_ACTIVE_DISPATCH_((QHsm *)arg, e);     /* dispatch to the AO's SM */
        QF_gc(e);    /* check if the event is garbage, and collect it if so */
    } while (((QActive *)arg)->thread != (HANDLE)0);
    QF_remove_((QActive *)arg);/* remove this object from any subscriptions */
    CloseHandle(((QActive *)arg)->osObject);        /* cleanup the OS event */
    return 0;                                             /* return success */
}



