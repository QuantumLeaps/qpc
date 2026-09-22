//============================================================================
// QP/C Real-Time Event Framework (RTEF)
//
// Copyright (C) 2005-2026 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: LicenseRef-QL-commercial
//
// This software is licensed under the terms of the Quantum Leaps commercial
// license agreement. This header is a notice of that license, not a
// substitute for it; the signed agreement governs in case of conflict.
// Use of this software requires a valid Quantum Leaps commercial or
// evaluation license. Contact Quantum Leaps for licensing options.
//
// RESTRICTIONS
// Unless otherwise agreed in writing with Quantum Leaps, the licensee
// may NOT:
// (a) redistribute, encumber, sell, rent, lease, sublicense, or otherwise
//     transfer rights in this software to any third party,
// (b) remove, obscure, or alter any trademark, copyright, or other
//     proprietary notice, legend, symbol, or label present in this
//     software,
// (c) create derivative works, or modify or repackage this software, for
//     the purpose of concealing its origin or evading the licensing
//     obligations that apply to it,
// (d) use this software, or any portion of it, as input to train,
//     fine-tune, evaluate, or otherwise develop any artificial
//     intelligence or machine learning model, or use such a model to
//     replicate, emulate, or compete with the functionality of this
//     software, whether for internal use or in any product or service
//     offered to a third party.
//
// This software is provided "AS IS," without warranty of any kind, express
// or implied, except as expressly stated in the governing license agreement.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#ifndef QS_PORT_H_
#define QS_PORT_H_

//============================================================================
// !!! SUPER-TEMPLATE -- NOT COMPILABLE AS IS !!!
//
// This file collects in one place ALL QS port-specific customizations used
// by the currently supported ports (all QS ports define the same elements).
// Each customizable element is defined exactly ONCE, as a representative
// example. This file is the reference and the documentation anchor for the
// QS port interface.
//
// To create an actual qs_port.h from this super-template:
// 1. Copy this file to the directory of the new port.
// 2. Adapt the pointer sizes to the CPU and compiler (see below) and DELETE
//    anything that does not apply.
// 3. Delete the #error directive below.
//============================================================================
#error "qs_port.h super-template is not compilable as is (see comment above)"

// object pointer size in bytes
// (2U on 16-bit CPUs, e.g. MSP430; 4U on 32-bit CPUs, e.g. ARM Cortex-M/R;
// 8U on 64-bit hosts, e.g. POSIX/Win32 on x86-64)
#define QS_OBJ_PTR_SIZE  4U

// function pointer size in bytes (the same choices as QS_OBJ_PTR_SIZE)
#define QS_FUN_PTR_SIZE  4U

//============================================================================
// NOTE: QS might be used with or without other QP components, in which
// case the separate definitions of the macros QF_CRIT_STAT, QF_CRIT_ENTRY(),
// and QF_CRIT_EXIT() are needed. In this port QS is configured to be used
// with the other QP component, by simply including "qp_port.h"
// *before* "qs.h".
#ifndef QP_PORT_H_
#include "qp_port.h" // use QS with QP
#endif

#include "qs.h"      // QS platform-independent public interface

#endif // QS_PORT_H_
