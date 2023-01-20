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
#ifndef BLINKY_BUTTON_H_
#define BLINKY_BUTTON_H_

enum Signals {
    TIMEOUT_SIG = Q_USER_SIG,
    BUTTON_PRESSED_SIG,
    BUTTON_RELEASED_SIG,
    BLINKY_WORK_SIG,
    FORWARD_PRESSED_SIG,
    FORWARD_RELEASED_SIG,
    // ...
    MAX_SIG  // the last signal
};

typedef struct {
    QEvt super;       // inherit QEvt
    uint16_t toggles; // number of toggles of the signal
    uint8_t ticks;    // number of clock ticks between
} BlinkyWorkEvt;

typedef struct {
    QEvt super;       // inherit QEvt
    uint16_t toggles; // number of toggles of the signal
} ButtonWorkEvt;

void Blinky1_instantiate(void);
extern QActive * const AO_Blinky1;  // opaque active object pointer

void Blinky3_instantiate(void);
extern QActive * const AO_Blinky3;  // opaque active object pointer

void Button2a_instantiate(void);
extern QActive * const AO_Button2a; // opaque active object pointer

void Button2b_instantiate(void);
extern QActive * const AO_Button2b; // opaque active object pointer

#endif // BLINKY_BUTTON_H_
