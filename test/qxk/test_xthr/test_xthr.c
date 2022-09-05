/*============================================================================
* Product: System test fixture for QXK on the EFM32 target
* Last updated for version 7.1.0
* Last updated on  2022-08-20
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
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
============================================================================*/
#include "qpc.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

/*==========================================================================*/
static QXThread thr1;

/*..........................................................................*/
static void Thread1_run(QXThread * const me) {

    for (;;) {
        BSP_ledOn();
        QXThread_delay(1U); /* BLOCK */
        BSP_ledOff();
        QXThread_delay(1U); /* BLOCK */
    }
}
/*..........................................................................*/
static void Test1_ctor(void) {
    QXThread_ctor(&thr1, &Thread1_run, 0U);
}

/*==========================================================================*/
int main() {

    QF_init();  /* initialize the framework and the underlying QXK kernel */
    BSP_init(); /* initialize the Board Support Package */

    /* dictionaries */
    QS_OBJ_DICTIONARY(&thr1);
    QS_OBJ_DICTIONARY(&thr1.timeEvt);
    QS_SIG_DICTIONARY(TEST_SIG,    (void *)0);
    QS_SIG_DICTIONARY(TRIG_SIG,    (void *)0);
    QS_SIG_DICTIONARY(TIMEOUT_SIG, (void *)0);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_ALL_RECORDS); /* all records */

    /* initialize publish-subscribe... */
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize event pools... */
    static QF_MPOOL_EL(QEvt) smlPoolSto[10]; /* small pool */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* start the extended thread1 */
    static void const *test1QueueSto[5];
    static uint64_t test1StackSto[64];
    Test1_ctor(); /* instantiate the Test1 extended thread */
    QXTHREAD_START(&thr1,                /* thread to start */
                  Q_PRIO(1U, 1U),        /* QF-priority/preemption-thre. */
                  test1QueueSto,         /* message queue storage */
                  Q_DIM(test1QueueSto),  /* message length [events] */
                  test1StackSto,         /* stack storage */
                  sizeof(test1StackSto), /* stack size [bytes] */
                  (void *)0);            /* initialization param */

    return QF_run(); /* run the QF application */
}

/*==========================================================================*/
void QS_onTestSetup(void) {
}
/*..........................................................................*/
void QS_onTestTeardown(void) {
}
/*..........................................................................*/
/*! callback function to execute user commands */
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    switch (cmdId) {
       case 0U: {
           break;
       }
       case 1U: {
           break;
       }
       default:
           break;
    }
}

/****************************************************************************/
/*! Host callback function to "massage" the event, if necessary */
void QS_onTestEvt(QEvt *e) {
    (void)e;
}
/*..........................................................................*/
/*! callback function to output the posted QP events (not used here) */
void QS_onTestPost(void const *sender, QActive *recipient,
                   QEvt const *e, bool status)
{
}

