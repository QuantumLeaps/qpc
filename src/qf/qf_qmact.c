//============================================================================
// QP/C Real-Time Event Framework (RTEF)
//
// Copyright (C) 2005-2026 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open-source GNU
// General Public License, version 3 or later (GPL-3.0-or-later), or,
// alternatively, under the terms of one of the closed-source Quantum Leaps
// commercial licenses.
//
// Redistributions in source code must retain this top-level comment block.
//
// NOTE:
// The GPL does NOT permit distributing this code as part of proprietary
// (closed-source) programs. For such use, please contact Quantum Leaps for
// a commercial license, which is an alternative to the GPL designed
// explicitly for closed-source distribution.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#define QP_IMPL           // this is QP implementation
#include "qp_port.h"      // QP port
#include "qp_pkg.h"       // QP package-scope interface
#include "qsafe.h"        // QP Functional Safety (FuSa) Subsystem
#ifdef Q_SPY              // QS software tracing enabled?
    #include "qs_port.h"  // QS port
    #include "qs_pkg.h"   // QS facilities for pre-defined trace records
#else
    #include "qs_dummy.h" // disable the QS software tracing
#endif // Q_SPY

//Q_DEFINE_THIS_MODULE("qf_qmact")

//............................................................................
//! @protected @memberof QMActive
void QMActive_ctor(QMActive * const me,
    QStateHandler const initial)
{
    // call the superclass' ctor (initializes the QActive attributes)
    QActive_ctor(&me->super, initial);

    // NOTE: QMActive indirectly inherits the abstract QAsm base class,
    // but it will delegate the state machine behavior to the QMsm class,
    // so the following initialization is identical as in QMsm ctor:
    QMsm_ctor((QMsm *)(me), initial);

    // NOTE: this vtable is identical as QMsm, but is provided
    // for the QMActive subclass to provide a UNIQUE vptr to distinguish
    // subclasses of QActive (e.g., in the debugger).
    static struct QAsmVtable const vtable = { // QMActive virtual table
        &QMsm_init_,
        &QMsm_dispatch_,
        &QMsm_isIn_,
        &QMsm_getStateHandler_
    };
    me->super.super.vptr = &vtable; // hook vptr to QMActive vtable
}
