/*****************************************************************************
* Product: "Dining Philosophers Problem" example, embOS
* Last updated for version 6.4.0
* Last updated on  2019-02-08
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
    /* stacks */
    static OS_STACKPTR int tableStack[128];
    static OS_STACKPTR int philoStack[N_PHILO][128];

    static QEvt const *tableQueueSto[N_PHILO];
    static QEvt const *philoQueueSto[N_PHILO][N_PHILO];
    static QSubscrList subscrSto[MAX_PUB_SIG];
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO]; /* small pool */
    uint8_t n;

    Philo_ctor(); /* instantiate all Philosopher active objects */
    Table_ctor(); /* instantiate the Table active object */

    QF_init();    /* initialize the framework and the underlying RT kernel */
    BSP_init();   /* initialize the Board Support Package */

    /* init publish-subscribe */
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* start the active objects... */
    for (n = 0U; n < N_PHILO; ++n) {
        QACTIVE_START(AO_Philo[n], (uint8_t)(n + 1),
                      philoQueueSto[n], Q_DIM(philoQueueSto[n]),
                      philoStack[n], sizeof(philoStack[n]), (QEvt *)0);
    }

    /* set the embOS task attributes BEFORE calling QACTIVE_START() */
    QActive_setAttr(AO_Table, QF_TASK_USES_FPU, 0);
    QACTIVE_START(AO_Table, (uint8_t)(N_PHILO + 1),
                  tableQueueSto, Q_DIM(tableQueueSto),
                  tableStack, sizeof(tableStack), (QEvt *)0);

    return QF_run(); /* run the QF application */
}

