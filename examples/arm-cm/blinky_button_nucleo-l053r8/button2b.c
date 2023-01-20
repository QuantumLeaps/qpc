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

Q_DEFINE_THIS_MODULE("button2b") // for DBC assertions in this module

//............................................................................
typedef struct {    // Button2b task
    QActive super;  // inherit QActive
    // add internal variables for this AO...
} Button2b;

static void Button2b_ctor(Button2b * const me);
static QState Button2b_initial(Button2b * const me, QEvt const * const e);
static QState Button2b_active(Button2b * const me, QEvt const * const e);

//............................................................................
static Button2b Button2b_inst; // the Button2b instance
QActive * const AO_Button2b = &Button2b_inst.super; // opaque AO pointer

void Button2b_instantiate(void) {
    Button2b_ctor(&Button2b_inst);
}

//............................................................................
static void Button2b_ctor(Button2b * const me) {
    QActive_ctor(
       &me->super,
       Q_STATE_CAST(&Button2b_initial));
}
//............................................................................
static QState Button2b_initial(Button2b * const me, QEvt const * const ie) {
    (void)me;
    (void)ie;
    return Q_TRAN(&Button2b_active);
}
//............................................................................
static QState Button2b_active(Button2b * const me, QEvt const * const e) {
    (void)me;
    QState status_;
    switch (e->sig) {
        case FORWARD_PRESSED_SIG: {
            BSP_d3on();
            // Button2b --> Blinky3
            QACTIVE_POST(AO_Blinky3, BSP_getWorkEvtBlinky3(1U), me);
            BSP_d3off();

            for (uint16_t i = Q_EVT_CAST(ButtonWorkEvt)->toggles;
                 i > 0U; --i)
            {
                BSP_d3on();
                BSP_d3off();
            }
            status_ = Q_HANDLED();
            break;
        }
        case FORWARD_RELEASED_SIG: {
            BSP_d3on();
            // Button2b --> Blinky3
            QACTIVE_POST(AO_Blinky3, BSP_getWorkEvtBlinky3(0U), me);
            BSP_d3off();

            for (uint16_t i = Q_EVT_CAST(ButtonWorkEvt)->toggles;
                 i > 0U; --i)
            {
                BSP_d3on();
                BSP_d3off();
            }
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
