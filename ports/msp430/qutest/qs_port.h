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

// object pointer size in bytes
#define QS_OBJ_PTR_SIZE  2U

// function pointer size in bytes
#define QS_FUN_PTR_SIZE  2U

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
