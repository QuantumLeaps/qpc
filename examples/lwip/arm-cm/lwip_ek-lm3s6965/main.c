/*****************************************************************************
* Product: DPP with lwIP application
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
#include "dpp.h"  /* application events and active objects */
#include "bsp.h"

Q_DEFINE_THIS_FILE

/* Local-scope objects -----------------------------------------------------*/
static QEvt const *l_tableQueueSto[N_PHILO];
static QEvt const *l_philoQueueSto[N_PHILO][N_PHILO];
static QEvt const *l_lwIPMgrQueueSto[10];
static QSubscrList   l_subscrSto[MAX_PUB_SIG];

static QF_MPOOL_EL(TableEvt) l_smlPoolSto[20]; /* small event pool */
static QF_MPOOL_EL(TextEvt)  l_medPoolSto[4];  /* med.  event pool */

/*..........................................................................*/
int main(void) {
    uint8_t n;

    Philo_ctor();   /* instantiate all Philosopher active objects */
    Table_ctor();   /* instantiate the Table active object */
    LwIPMgr_ctor(); /* instantiate all LwIP-Manager active object */

    BSP_init();     /* initialize the Board Support Package */

    QF_init(); /* initialize the framework and the underlying RT kernel */

    /* object dictionaries... */
    QS_OBJ_DICTIONARY(l_smlPoolSto);
    QS_OBJ_DICTIONARY(l_medPoolSto);
    QS_OBJ_DICTIONARY(l_lwIPMgrQueueSto);
    QS_OBJ_DICTIONARY(l_philoQueueSto[0]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[1]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[2]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[3]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[4]);
    QS_OBJ_DICTIONARY(l_tableQueueSto);

    /* init publish-subscribe */
    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));

    /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));
    QF_poolInit(l_medPoolSto, sizeof(l_medPoolSto), sizeof(l_medPoolSto[0]));

    QACTIVE_START(AO_LwIPMgr, 1,
                  l_lwIPMgrQueueSto, Q_DIM(l_lwIPMgrQueueSto),
                  (void *)0, 0, (QEvt *)0);
    for (n = 0; n < N_PHILO; ++n) { /* start the active objects... */
        QACTIVE_START(AO_Philo[n], (uint8_t)(n + 2),
                      l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]),
                      (void *)0, 0, (QEvt *)0);
    }
    QACTIVE_START(AO_Table, (uint8_t)(N_PHILO + 2),
                  l_tableQueueSto, Q_DIM(l_tableQueueSto),
                  (void *)0, 0, (QEvt *)0);

    return QF_run(); /* run the QF application */
}
