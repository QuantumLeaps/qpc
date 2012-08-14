/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
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
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"
#include "freertos_task.h"

/* Local-scope objects -----------------------------------------------------*/
static QSubscrList   l_subscrSto[MAX_PUB_SIG];

static union SmallEvents {
    void   *e0;                                       /* minimum event size */
    uint8_t e1[sizeof(TableEvt)];
    /* ... other event types to go into this pool */
} l_smlPoolSto[2*N_PHILO];              /* storage for the small event pool */

/*..........................................................................*/
__arm int main(void) {
    uint8_t n;

    Philo_ctor();             /* instantiate all Philosopher active objects */
    Table_ctor();                    /* instantiate the Table active object */

    BSP_init();           /* initialize the Board Support Package */

    QF_init();     /* initialize the framework and the underlying RT kernel */

                                                  /* object dictionaries... */
    QS_OBJ_DICTIONARY(l_smlPoolSto);

    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));   /* init publish-subscribe */

                                               /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    for (n = 0; n < N_PHILO; ++n) {          /* start the active objects... */
        QActive_start(AO_Philo[n], (uint8_t)(n + 1),
            (QEvt const **)0, N_PHILO,
            (void *)0, (configMINIMAL_STACK_SIZE * sizeof(portSTACK_TYPE)),
            (QEvt *)0);
    }
    QActive_start(AO_Table, (uint8_t)(N_PHILO + 1),
        (QEvt const **)0, N_PHILO,
        (void *)0, (configMINIMAL_STACK_SIZE * sizeof(portSTACK_TYPE)),
        (QEvt *)0);

                           /* create a raw FreeRTOS task, not managed by QP */
    xTaskCreate(&vTaskFunction,
                (signed portCHAR *)"TASK",
                configMINIMAL_STACK_SIZE,
                (void *)0,
                tskIDLE_PRIORITY + N_PHILO + 2,
                (xTaskHandle *)0);

    return QF_run();                              /* run the QF application */
}
