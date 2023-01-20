//============================================================================
// QP example for ThreadX
// Last updated for: @ref qpc_7_3_0
// Last updated on  2023-08-22
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <www.gnu.org/licenses/>.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"                 // QP/C real-time embedded framework
#include "dpp.h"                 // DPP Application interface
#include "bsp.h"                 // Board Support Package

//Q_DEFINE_THIS_FILE

//............................................................................
int main() {
    tx_kernel_enter();
    return 0; // tx_kernel_enter() does not return
}
//............................................................................
void tx_application_define(void *first_unused_memory) {
    Q_UNUSED_PAR(first_unused_memory);

    QF_init();  // initialize the framework
    BSP_init(); // initialize the Board Support Package

    static QSubscrList l_subscrSto[MAX_PUB_SIG];
    QActive_psInit(l_subscrSto, Q_DIM(l_subscrSto)); // init publish-subscribe

    // initialize event pools...
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO];
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // start the active objects/threads...

    static QEvt const *philoQueueSto[N_PHILO][10];
    static ULONG philoStk[N_PHILO][200]; // stacks for the Philosophers
    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        Philo_ctor(n); // instantiate the Philo AO
        QActive_setAttr(AO_Philo[n], THREAD_NAME_ATTR, "Philo");
        QACTIVE_START(AO_Philo[n],

            // NOTE: set the preemption-threshold of all Philos to
            // the same level, so that they cannot preempt each other.
            Q_PRIO(n + 1U, N_PHILO), // QF-prio/pre-thre.

            philoQueueSto[n], Q_DIM(philoQueueSto[n]),
            philoStk[n], sizeof(philoStk[n]),
            (void *)0);
    }

    static QEvt const *tableQueueSto[N_PHILO];
    static ULONG tableStk[200]; // stack for the Table
    Table_ctor(); // instantiate the Table AO
    QActive_setAttr(AO_Table, THREAD_NAME_ATTR, "Table");
    QACTIVE_START(AO_Table,
        N_PHILO + 1U,
        tableQueueSto, Q_DIM(tableQueueSto),
        tableStk, sizeof(tableStk),
        (void *)0);

    (void)QF_run();
}
