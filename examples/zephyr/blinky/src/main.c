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
* @brief main() for Zephyr, Blinky example
*/
#include "qpc.h"
#include "blinky.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

/*..........................................................................*/
static QEvt const *l_blinkyQSto[10]; /* event queue storage for Blinky */
K_THREAD_STACK_DEFINE(l_blinkyStack, 1024); /* stack storage for Blinky */

int main() {

    QF_init();  /* initialize the framework and the underlying RT kernel */
    BSP_init(); /* initialize the Board Support Package */

    /* publish-subscribe not used, no call to QF_psInit() */
    /* dynamic event allocation not used, no call to QF_poolInit() */

    /* instantiate and start the active objects... */
    Blinky_ctor();
    QActive_setAttr(AO_Blinky,
                    0,            /* thread options */
                    "Blinky");    /* thread name */
    QACTIVE_START(AO_Blinky,      /* AO pointer to start */
                  1U,             /* unique QP priority of the AO */
                  l_blinkyQSto,   /* storage for the AO's queue */
                  Q_DIM(l_blinkyQSto), /* length of the queue [entries] */
                  (void *)l_blinkyStack, /* stack storage */
                  K_THREAD_STACK_SIZEOF(l_blinkyStack), /* stack size [bytes] */
                  (void *)0);     /* initial event (or 0) */

    return QF_run(); /* run the QF application */
}
