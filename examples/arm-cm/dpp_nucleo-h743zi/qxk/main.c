/*****************************************************************************
* Product: DPP example extened for QXK
* Last updated for version 6.8.2
* Last updated on  2020-07-15
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
*****************************************************************************/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

static QTicker l_ticker0;
QActive *the_Ticker0 = &l_ticker0.super;

/*..........................................................................*/
int main() {
    static QEvt const *tableQueueSto[N_PHILO];
    static QEvt const *philoQueueSto[N_PHILO][N_PHILO];
    static QSubscrList subscrSto[MAX_PUB_SIG];
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO]; /* small pool */

    /* stacks and queues for the extended test threads */
    static void const *test1QueueSto[5];
    static uint64_t test1StackSto[64];
    static void const *test2QueueSto[5];
    static uint64_t test2StackSto[64];

    uint8_t n;

    Philo_ctor(); /* instantiate all Philosopher active objects */
    Table_ctor(); /* instantiate the Table active object */
    QTicker_ctor(&l_ticker0, 0U); /* ticker AO for tick rate 0 */
    Test1_ctor(); /* instantiate the Test1 extended thread */
    Test2_ctor(); /* instantiate the Test2 extended thread */

    QF_init();    /* initialize the framework and the underlying RT kernel */

    /* initialize publish-subscribe... */
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* initialize the Board Support Package
    * NOTE: BSP_init() is called *after* initializing publish-subscribe and
    * event pools, to make the system ready to accept SysTick interrupts.
    * Unfortunately, the STM32Cube code that must be called from BSP,
    * configures and starts SysTick.
    */
    BSP_init();

    /* object dictionaries... */
    QS_OBJ_DICTIONARY(AO_Table);
    QS_OBJ_DICTIONARY(AO_Philo[0]);
    QS_OBJ_DICTIONARY(AO_Philo[1]);
    QS_OBJ_DICTIONARY(AO_Philo[2]);
    QS_OBJ_DICTIONARY(AO_Philo[3]);
    QS_OBJ_DICTIONARY(AO_Philo[4]);
    QS_OBJ_DICTIONARY(&l_ticker0);

    /* start the extended thread */
    QXTHREAD_START(XT_Test1,                 /* Thread to start */
                  (uint_fast8_t)1,           /* QP priority of the thread */
                  test1QueueSto,             /* message queue storage */
                  Q_DIM(test1QueueSto),      /* message length [events] */
                  test1StackSto,             /* stack storage */
                  sizeof(test1StackSto),     /* stack size [bytes] */
                  (QEvt *)0);                /* initialization event */

    /* NOTE: leave priority 2 free for a mutex */

    /* start the Philo active objects... */
    for (n = 0U; n < N_PHILO; ++n) {
        QACTIVE_START(AO_Philo[n],           /* AO to start */
                      (uint_fast8_t)(n + 3), /* QP priority of the AO */
                      philoQueueSto[n],      /* event queue storage */
                      Q_DIM(philoQueueSto[n]), /* queue length [events] */
                      (void *)0,             /* stack storage (not used) */
                      0U,                    /* size of the stack [bytes] */
                      (QEvt *)0);            /* initialization event */
    }

    /* example of prioritizing the Ticker0 active object */
    QACTIVE_START(the_Ticker0, (uint_fast8_t)(N_PHILO + 3),
                  0, 0, 0, 0, 0);

    /* NOTE: leave priority (N_PHILO + 4) free for mutex */

    QXTHREAD_START(XT_Test2,                 /* Thread to start */
                  (uint_fast8_t)(N_PHILO + 5), /* QP priority of the thread */
                  test2QueueSto,             /* message queue storage */
                  Q_DIM(test2QueueSto),      /* message length [events] */
                  test2StackSto,             /* stack storage */
                  sizeof(test2StackSto),     /* stack size [bytes] */
                  (QEvt *)0);                /* initialization event */

    /* NOTE: leave priority (N_PHILO + 6) free for mutex */

    QACTIVE_START(AO_Table,                  /* AO to start */
                  (uint_fast8_t)(N_PHILO + 7), /* QP priority of the AO */
                  tableQueueSto,             /* event queue storage */
                  Q_DIM(tableQueueSto),      /* queue length [events] */
                  (void *)0,                 /* stack storage (not used) */
                  0U,                        /* size of the stack [bytes] */
                  (QEvt *)0);                /* initialization event */

    return QF_run(); /* run the QF application */
}

