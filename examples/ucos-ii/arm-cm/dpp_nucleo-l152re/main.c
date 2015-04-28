/*****************************************************************************
* Product: DPP example, uC/OS-II
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-03-26
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
* Web  : http://www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

/*..........................................................................*/
int main() {
    static QEvt const *tableQueueSto[N_PHILO];
    static QEvt const *philoQueueSto[N_PHILO][N_PHILO];

    static OS_STK philoStk[N_PHILO][128]; /* stacks for the Philosophers */
    static OS_STK tableStk[256];          /* stack for the Table */

    static QSubscrList subscrSto[MAX_PUB_SIG];
    static TableEvt smlPoolSto[2*N_PHILO]; /* storage for small event pool */
    uint8_t n;

    /* in C you need to explicitly instantiate all AOs...  */
    Philo_ctor();
    Table_ctor();

    QF_init();  /* initialize the framework and the underlying RT kernel */
    BSP_init(); /* initialize the Board Support Package */

    /* object dictionaries... */
    QS_OBJ_DICTIONARY(smlPoolSto);
    QS_OBJ_DICTIONARY(tableQueueSto);
    QS_OBJ_DICTIONARY(philoQueueSto[0]);
    QS_OBJ_DICTIONARY(philoQueueSto[1]);
    QS_OBJ_DICTIONARY(philoQueueSto[2]);
    QS_OBJ_DICTIONARY(philoQueueSto[3]);
    QS_OBJ_DICTIONARY(philoQueueSto[4]);

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
        QF_setUCosTaskAttr(AO_Philo[n], OS_TASK_OPT_STK_CHK);
        QACTIVE_START(AO_Philo[n],
                      (uint_fast8_t)(n + 1), /* QP priority */
                      philoQueueSto[n],      /* storage for the AO's queue */
                      Q_DIM(philoQueueSto[n]), /* queue's length [entries] */
                      philoStk[n],           /* stack storage */
                      sizeof(philoStk[n]),   /* sack size [bytes] */
                      (QEvt const *)0);      /* initialization event */
    }

    /* NOTE: provide uC/OS-II task attributes for the AO's task */
    QF_setUCosTaskAttr(AO_Table, OS_TASK_OPT_STK_CHK);
    QACTIVE_START(AO_Table,
                  (uint_fast8_t)(N_PHILO + 1), /* QP priority */
                  tableQueueSto,  /* storage for the AO's queue */
                  Q_DIM(tableQueueSto), /* queue's length [entries] */
                  tableStk,             /* stack storage */
                  sizeof(tableStk),     /* sack size [bytes] */
                  (QEvt const *)0);     /* initialization event */

    return QF_run(); /* run the QF application */
}

