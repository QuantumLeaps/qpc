/*****************************************************************************
* Product: DPP example, uC/OS-II
* Last updated for version 6.8.1
* Last updated on  2020-06-08
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

/*..........................................................................*/
int main() {
    static QEvt const *tableQueueSto[N_PHILO];
    static QEvt const *philoQueueSto[N_PHILO][N_PHILO];

    static OS_STK philoStk[N_PHILO][100]; /* stacks for the Philosophers */
    static OS_STK tableStk[128];          /* stack for the Table */

    static QSubscrList subscrSto[MAX_PUB_SIG];
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO]; /* event pool */
    uint8_t n;

    /* in C you need to explicitly instantiate all AOs...  */
    Philo_ctor();
    Table_ctor();

    QF_init();  /* initialize the framework and the underlying RT kernel */
    BSP_init(); /* initialize the Board Support Package */

    /* init publish-subscribe */
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* start the active objects... */
    /* CAUTION:
    * Make sure to configure **SUFFICIENT** number of uC/OS-II tasks and
    * message queues in "os_cfg.h" for all your active objects!!!
    */
    for (n = 0U; n < N_PHILO; ++n) {
        /* NOTE: provide uC/OS-II task attributes for the AO's task */
        QActive_setAttr(AO_Philo[n], OS_TASK_OPT_STK_CHK, 0);
        QACTIVE_START(AO_Philo[n],
                      n + 1U,                /* QP priority */
                      philoQueueSto[n],      /* storage for the AO's queue */
                      Q_DIM(philoQueueSto[n]), /* queue's length [entries] */
                      philoStk[n],           /* stack storage */
                      sizeof(philoStk[n]),   /* sack size [bytes] */
                      (QEvt const *)0);      /* initialization event */
    }

    /* NOTE: provide uC/OS-II task attributes for the AO's task */
    QActive_setAttr(AO_Table, OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK, 0);
    QACTIVE_START(AO_Table,
                  N_PHILO + 1U,         /* QP priority */
                  tableQueueSto,        /* storage for the AO's queue */
                  Q_DIM(tableQueueSto), /* queue's length [entries] */
                  tableStk,             /* stack storage */
                  sizeof(tableStk),     /* sack size [bytes] */
                  (QEvt const *)0);     /* initialization event */

    return QF_run(); /* run the QF application */
}

