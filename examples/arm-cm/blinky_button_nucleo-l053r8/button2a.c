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

Q_DEFINE_THIS_MODULE("button2a") // for DBC assertions in this module

//............................................................................
typedef struct {    // Button2a task
    QActive super;  // inherit QActive
    // add internal variables for this AO...
} Button2a;

static void Button2a_ctor(Button2a * const me);
static QState Button2a_initial(Button2a * const me, QEvt const * const ie);
static QState Button2a_active(Button2a * const me, QEvt const * const e);

//............................................................................
static Button2a Button2a_inst; // the Button2a instance
QActive * const AO_Button2a = &Button2a_inst.super; // opaque AO pointer

void Button2a_instantiate(void) {
    Button2a_ctor(&Button2a_inst);
}

//............................................................................
static void Button2a_ctor(Button2a * const me) {
    QActive_ctor(
       &me->super,
       Q_STATE_CAST(&Button2a_initial));
}
//............................................................................
static QState Button2a_initial(Button2a * const me, QEvt const * const ie) {
    (void)me;
    (void)ie;
    return Q_TRAN(&Button2a_active);
}
//............................................................................
static QState Button2a_active(Button2a * const me, QEvt const * const e) {
    (void)me;
    QState status_;
    switch (e->sig) {
        case BUTTON_PRESSED_SIG: {
            BSP_d4on();
            // Button2a --> Blinky1
            QACTIVE_POST(AO_Blinky1, BSP_getWorkEvtBlinky1(1U), me);
            BSP_d4off();

            for (uint16_t i = Q_EVT_CAST(ButtonWorkEvt)->toggles;
                 i > 0U; --i)
            {
                BSP_d4on();
                BSP_d4off();
            }
            status_ = Q_HANDLED();
            break;
        }
        case FORWARD_PRESSED_SIG: {
            BSP_d4on();
            // immutable event can be forwarded to another Task
            QACTIVE_POST(AO_Button2b, e, me); // Button2a --> Button2b
            status_ = Q_HANDLED();
            BSP_d4off();
            break;
        }
        case BUTTON_RELEASED_SIG: {
            BSP_d4on();
            // Button2a --> Blinky1
            QACTIVE_POST(AO_Blinky1, BSP_getWorkEvtBlinky1(0U), me);
            BSP_d4off();

            for (uint16_t i = Q_EVT_CAST(ButtonWorkEvt)->toggles;
                 i > 0U; --i)
            {
                BSP_d4on();
                BSP_d4off();
            }
            status_ = Q_HANDLED();
            break;
        }
        case FORWARD_RELEASED_SIG: {
            BSP_d4on();
            // immutable event can be forwarded to another Task
            QACTIVE_POST(AO_Button2b, e, me); // Button2a --> Button2b
            status_ = Q_HANDLED();
            BSP_d4off();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
