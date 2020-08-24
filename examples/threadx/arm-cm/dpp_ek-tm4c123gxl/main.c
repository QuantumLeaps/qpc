/*****************************************************************************
* Last updated for version 6.9.0
* Last updated on  2020-08-11
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

/* Local-scope objects -----------------------------------------------------*/
static QEvt const *l_tableQueueSto[N_PHILO];
static QEvt const *l_philoQueueSto[N_PHILO][N_PHILO];
static QSubscrList l_subscrSto[MAX_PUB_SIG];

static union SmallEvents {
    void   *e0; /* minimum event size */
    uint8_t e1[sizeof(TableEvt)];
    /* ... other event types to go into this pool */
} l_smlPoolSto[2*N_PHILO + 10]; /* storage for the small event pool */

static ULONG l_philoStk[N_PHILO][256]; /* stacks for the Philosophers */
static ULONG l_tableStk[256];          /* stack for the Table */

/*..........................................................................*/
int main() {
    Philo_ctor(); /* instantiate all Philosopher active objects */
    Table_ctor(); /* instantiate the Table active object */
    BSP_init();   /* initialize the Board Support Package */
    tx_kernel_enter();
    return 0; /* tx_kernel_enter() does not return */
}
/*..........................................................................*/
void tx_application_define(void *first_unused_memory) {
    uint8_t n;

    (void)first_unused_memory; /* avoid compiler warning about unused arg. */

    QF_init(); /* initialize the framework and the underlying RT kernel */

    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto)); /* init publish-subscribe */

    /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    QS_OBJ_DICTIONARY(l_smlPoolSto);
    QS_OBJ_DICTIONARY(l_tableQueueSto);
    QS_OBJ_DICTIONARY(l_philoQueueSto[0]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[1]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[2]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[3]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[4]);

    for (n = 0; n < N_PHILO; ++n) { /* start the active objects... */
        QACTIVE_START(AO_Philo[n], (uint8_t)(n + 1),
                      l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]),
                      l_philoStk[n], sizeof(l_philoStk[n]), (QEvt *)0);
    }
    QACTIVE_START(AO_Table, (uint8_t)(N_PHILO + 1),
                  l_tableQueueSto, Q_DIM(l_tableQueueSto),
                  l_tableStk, sizeof(l_tableStk), (QEvt *)0);

    (void)QF_run();
}
