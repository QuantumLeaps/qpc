/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-08-24
* @version Last updated for: Zephyr 3.1.99 and @ref qpc_7_1_0
*
* @file
* @brief main() for Zephyr, DPP example
*/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

/*..........................................................................*/
static QEvt const *tableQueueSto[10];
K_THREAD_STACK_DEFINE(tableStack, 1024); /* stack storage */

static QEvt const *philoQueueSto[N_PHILO][10];
K_THREAD_STACK_DEFINE(philoStack[N_PHILO], 512); /* stack storage */

static QSubscrList subscrSto[MAX_PUB_SIG];
static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO]; /* small pool */

int main(void) {
    QF_init();  /* initialize the framework and the underlying RT kernel */
    BSP_init(); /* initialize the Board Support Package */

    /* initialize publish-subscribe... */
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* start the active objects... */
    Philo_ctor(); /* instantiate all Philosopher active objects */
    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        QActive_setAttr(AO_Philo[n],
            0,                       /* thread options */
            "Philo");                /* thread name */
        QACTIVE_START(AO_Philo[n],   /* AO to start */
            (uint_fast8_t)(n + 1),   /* QP priority of the AO */
            philoQueueSto[n],        /* event queue storage */
            Q_DIM(philoQueueSto[n]), /* queue length [events] */
            (void *)philoStack[n],   /* stack storage */
            K_THREAD_STACK_SIZEOF(philoStack[n]), /* stack size [bytes] */
            (void *)0);              /* initialization param */
    }
    Table_ctor(); /* instantiate the Table active object */
    QActive_setAttr(AO_Table,
        0,                           /* thread options */
        "Table");                    /* thread name */
    QACTIVE_START(AO_Table,          /* AO to start */
        (uint_fast8_t)(N_PHILO + 1), /* QP priority of the AO */
        tableQueueSto,               /* event queue storage */
        Q_DIM(tableQueueSto),        /* queue length [events] */
        (void *)tableStack,          /* stack storage */
        K_THREAD_STACK_SIZEOF(tableStack), /* stack size [bytes] */
        (void *)0);                  /* initialization param */

    return QF_run(); /* run the QF application */
}
