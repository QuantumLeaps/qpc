/*****************************************************************************
* Product: "Dining Philosophers Problem" example, QP-FreeRTOS port
* Last Updated for Version: 5.3.1
* Date of the Last Update:  2014-09-24
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. www.state-machine.com.
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
* Web:   http://www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

/*..........................................................................*/
int main() {
    static QEvt const *tableQueueSto[N_PHILO];
    static QEvt const *philoQueueSto[N_PHILO][N_PHILO];
    static QSubscrList subscrSto[MAX_PUB_SIG];
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO]; /* small pool */
    uint8_t n;

    Philo_ctor(); /* instantiate all Philosopher active objects */
    Table_ctor(); /* instantiate the Table active object */

    QF_init();    /* initialize the framework and the underlying RT kernel */
    BSP_init();   /* initialize the Board Support Package */

    /* object dictionaries... */
    QS_OBJ_DICTIONARY(smlPoolSto);
    QS_OBJ_DICTIONARY(tableQueueSto);
    QS_OBJ_DICTIONARY(philoQueueSto[0]);
    QS_OBJ_DICTIONARY(philoQueueSto[1]);
    QS_OBJ_DICTIONARY(philoQueueSto[2]);
    QS_OBJ_DICTIONARY(philoQueueSto[3]);
    QS_OBJ_DICTIONARY(philoQueueSto[4]);

    QF_psInit(subscrSto, Q_DIM(subscrSto)); /* init publish-subscribe */

    /* initialize event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* start the active objects... */
    for (n = 0U; n < N_PHILO; ++n) {
        QACTIVE_START(AO_Philo[n], (uint8_t)(n + 1),
                      philoQueueSto[n], Q_DIM(philoQueueSto[n]),
                      (void *)0, 512U, (QEvt *)0);
    }
    QACTIVE_START(AO_Table, (uint8_t)(N_PHILO + 1),
                  tableQueueSto, Q_DIM(tableQueueSto),
                  (void *)0, 512U, (QEvt *)0);

    return QF_run(); /* run the QF application */
}

