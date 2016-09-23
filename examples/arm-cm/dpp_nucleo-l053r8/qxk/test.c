/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 5.7.1
* Date of the Last Update:  2016-09-20
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* http://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

/* local "naked" thread object .............................................*/
static QXThread l_test1;
static QXThread l_test2;
static QXMutex l_mutex;
static QXSemaphore l_sema;

/* global pointer to the test thread .......................................*/
QXThread * const XT_Test1 = &l_test1;
QXThread * const XT_Test2 = &l_test2;

/*..........................................................................*/
static void Thread1_run(QXThread * const me) {

    QXMutex_init(&l_mutex, 3U);

    (void)me;
    for (;;) {
        float volatile x;

        /* wait on a semaphore (BLOCK) */
        QXSemaphore_wait(&l_sema, QXTHREAD_NO_TIMEOUT, 0U);
        BSP_ledOn();

        /* some flating point code to exercise the VFP... */
        QXMutex_lock(&l_mutex);
        x = 1.4142135F;
        x = x * 1.4142135F;
        QXMutex_unlock(&l_mutex);

        QXThread_delay(BSP_TICKS_PER_SEC/8U, 0U);  /* BLOCK */

        /* publish to thread2 */
        QF_PUBLISH(Q_NEW(QEvt, TEST_SIG), &l_test1);
    }
}

/*..........................................................................*/
void Test1_ctor(void) {
    QXThread_ctor(&l_test1, Q_XTHREAD_CAST(&Thread1_run), 0U);
}

/*..........................................................................*/
static void Thread2_run(QXThread * const me) {

    /* subscribe to the test signal */
    QActive_subscribe(&me->super, TEST_SIG);

    /* initialize the semaphore before using it
    * NOTE: the semaphore is initialized in the highest-priority thread
    * that uses it. Alternatively, the semaphore can be initialized
    * before any thread runs.
    */
    QXSemaphore_init(&l_sema, 0U); /* start with zero count */

    for (;;) {
        QEvt const *e;
        float volatile x;

        /* signal thread1... */
        QXSemaphore_signal(&l_sema);

        /* some flating point code to exercise the VFP... */
        x = 1.4142135F;
        x = x * 1.4142135F;

        /* wait on the internal event queue (BLOCK) */
        e = QXThread_queueGet(QXTHREAD_NO_TIMEOUT, 0U);
        BSP_ledOff();
        QF_gc(e); /* recycle the event manually! */

        QXThread_delay(BSP_TICKS_PER_SEC, 0U);  /* BLOCK */
    }
}

/*..........................................................................*/
void Test2_ctor(void) {
    QXThread_ctor(&l_test2, Q_XTHREAD_CAST(&Thread2_run), 0U);
}
