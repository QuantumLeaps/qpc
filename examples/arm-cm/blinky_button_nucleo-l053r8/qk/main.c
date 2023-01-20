//============================================================================
// QP/C Example
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"           // QP/C API
#include "bsp.h"           // Board Support Package interface
#include "blinky_button.h" // application shared interface

Q_DEFINE_THIS_MODULE("main")

//............................................................................
int main() {
    QF_init();  // initialize the QF framework
    BSP_init(); // initialize the Board Support Package

    // instantiate and start QP/C active objects...
    Blinky1_instantiate();
    static QEvt const *blinky1QSto[10]; // Event queue storage
    QACTIVE_START(
        AO_Blinky1,          // AO pointer to start
        Q_PRIO(1U, 1U),      // QF-prio/pre-thre.
        blinky1QSto,         // storage for the AO's queue
        Q_DIM(blinky1QSto),  // queue length
        (void *)0,           // stack storage (not used)
        0U,                  // size of the stack [bytes]
        BSP_getWorkEvtBlinky1(0U)); // initialization event

    Button2a_instantiate();
    static QEvt const *button2aQSto[8]; // Event queue storage
    QACTIVE_START(
        AO_Button2a,         // AO pointer to start
        Q_PRIO(2U, 3U),      // QF-prio/pre-thre.
        button2aQSto,        // storage for the AO's queue
        Q_DIM(button2aQSto), // queue length
        (void *)0,           // stack storage (not used)
        0U,                  // size of the stack [bytes]
        (QEvt const *)0);    // initialization event -- not used

    Button2b_instantiate();
    static QEvt const *button2bQSto[8]; // Event queue storage
    QACTIVE_START(
        AO_Button2b,         // AO pointer to start
        Q_PRIO(3U, 3U),      // QF-prio/pre-thre.
        button2bQSto,        // storage for the AO's queue
        Q_DIM(button2bQSto), // queue length
        (void *)0,           // stack storage (not used)
        0U,                  // size of the stack [bytes]
        (QEvt const *)0);    // initialization event -- not used

    Blinky3_instantiate();
    static QEvt const *blinky3QSto[8]; // Event queue storage
    QACTIVE_START(
        AO_Blinky3,          // AO pointer to start
        Q_PRIO(4U, 4U),      // QF-prio/pre-thre.
        blinky3QSto,         // storage for the AO's queue
        Q_DIM(blinky3QSto),  // queue length
        (void *)0,           // stack storage (not used)
        0U,                  // size of the stack [bytes]
        BSP_getWorkEvtBlinky3(0U)); // initialization event

    return QF_run(); // run the QF application
    // NOTE: in embedded systems QF_run() should not return
}
