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

Q_DEFINE_THIS_MODULE("blinky1") // for DBC assertions in this module

//............................................................................
typedef struct {      // Blinky task
    QActive super;    // inherit QActive
    QTimeEvt te;      // time event for generating TIMEOUT events
    uint16_t toggles; // number of toggles to perform for TIMEOUT event
} Blinky1;

static void Blinky1_ctor(Blinky1 * const me);
static QState Blinky1_initial(Blinky1 * const me, QEvt const * const ie);
static QState Blinky1_active(Blinky1 * const me, QEvt const * const e);

//............................................................................
static Blinky1 Blinky1_inst; // the Blinky instance
QActive * const AO_Blinky1 = &Blinky1_inst.super; // opaque AO pointer

void Blinky1_instantiate(void) {
    Blinky1_ctor(&Blinky1_inst);
}

//............................................................................
static void Blinky1_ctor(Blinky1 * const me) {
    QActive_ctor(
       &me->super,
       Q_STATE_CAST(&Blinky1_initial));
    QTimeEvt_ctorX(&me->te, &me->super, TIMEOUT_SIG, 0U);
}
//............................................................................
static QState Blinky1_initial(Blinky1 * const me, QEvt const * const e) {
    // the initial event must be provided and must be WORKLOAD_SIG
    Q_REQUIRE_ID(300,
        (e != (QEvt const *)0) && (e->sig == BLINKY_WORK_SIG));

    QTimeEvt_armX(&me->te,
        Q_EVT_CAST(BlinkyWorkEvt)->ticks,
        Q_EVT_CAST(BlinkyWorkEvt)->ticks);
    me->toggles = Q_EVT_CAST(BlinkyWorkEvt)->toggles;
    return Q_TRAN(&Blinky1_active);
}
//............................................................................
static QState Blinky1_active(Blinky1 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case TIMEOUT_SIG: {
            for (uint16_t i = me->toggles; i > 0U; --i) {
                BSP_d5on();
                BSP_d5off();
            }
           status_ = Q_HANDLED();
           break;
        }
        case BLINKY_WORK_SIG: {
            BSP_d5on();
            QTimeEvt_disarm(&me->te);
            QTimeEvt_armX(&me->te,
                Q_EVT_CAST(BlinkyWorkEvt)->ticks,
                Q_EVT_CAST(BlinkyWorkEvt)->ticks);
            me->toggles = Q_EVT_CAST(BlinkyWorkEvt)->toggles;
            status_ = Q_HANDLED();
            BSP_d5off();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
