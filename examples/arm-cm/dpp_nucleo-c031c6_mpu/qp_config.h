//============================================================================
// QP configuration file example
// Last updated for version: 7.3.0
// Last updated on: 2023-09-09
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
// <www.state-machine.com>
// <info@state-machine.com>
//============================================================================
#ifndef QP_CONFIG_H_
#define QP_CONFIG_H_

// NOTE:
// The QP configuration takes effect only when the macro QP_CONFIG
// is defined on the command-line to the compiler for all QP source files.

// use memory isolation (MPU)
#define QF_MEM_ISOLATE

// use event constructors for dynamic events
#define QEVT_DYN_CTOR

// for QK kernel:
// use the Reserved31_IRQHandler() with IRQ number 31
// for the QK return-from-preemption handler
#define QK_USE_IRQ_NUM      31
#define QK_USE_IRQ_HANDLER  Reserved31_IRQHandler

// for QXK kernel:
// use the Reserved31_IRQHandler() with IRQ number 31
// for the QXK return-from-preemption handler
#define QXK_USE_IRQ_NUM      31
#define QXK_USE_IRQ_HANDLER  Reserved31_IRQHandler

#endif // QP_CONFIG_H_
