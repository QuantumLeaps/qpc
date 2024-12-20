//$file${src::qf::qf_act.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: qpc.qm
// File:  ${src::qf::qf_act.c}
//
// This code has been generated by QM 7.0.0 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// The QP/C software is dual-licensed under the terms of the open-source GNU
// General Public License (GPL) or under the terms of one of the closed-
// source Quantum Leaps commercial licenses.
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// NOTE:
// The GPL does NOT permit the incorporation of this code into proprietary
// programs. Please contact Quantum Leaps for commercial licensing options,
// which expressly supersede the GPL and are designed explicitly for
// closed-source distribution.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${src::qf::qf_act.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
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

//Q_DEFINE_THIS_MODULE("qf_act")

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${QF::QActive::registry_[QF_MAX_ACTIVE + 1U]} vvvvvvvvvvvvvvvvvvvvvvv
//! @static @private @memberof QActive
QActive * QActive_registry_[QF_MAX_ACTIVE + 1U];
//$enddef${QF::QActive::registry_[QF_MAX_ACTIVE + 1U]} ^^^^^^^^^^^^^^^^^^^^^^^

//$define${QF::QF-pkg} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${QF::QF-pkg::priv_} .......................................................
QF_Attr QF_priv_;

//${QF::QF-pkg::bzero_} ......................................................
//! @static @private @memberof QF
void QF_bzero_(
    void * const start,
    uint_fast16_t const len)
{
    uint8_t *ptr = (uint8_t *)start;
    for (uint_fast16_t n = len; n > 0U; --n) {
        *ptr = 0U;
        ++ptr;
    }
}
//$enddef${QF::QF-pkg} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${QF::types::QF_LOG2} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${QF::types::QF_LOG2} ......................................................
#ifndef QF_LOG2
uint_fast8_t QF_LOG2(QPSetBits const bitmask) {
    static uint8_t const log2LUT[16] = {
        0U, 1U, 2U, 2U, 3U, 3U, 3U, 3U,
        4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U
    };
    uint_fast8_t n = 0U;
    QPSetBits tmp;
    QPSetBits x = bitmask;

    #if (QF_MAX_ACTIVE > 16U)
    tmp = (x >> 16U);
    if (tmp != 0U) {
        n += 16U;
        x = tmp;
    }
    #endif
    #if (QF_MAX_ACTIVE > 8U)
    tmp = (x >> 8U);
    if (tmp != 0U) {
        n += 8U;
        x = tmp;
    }
    #endif
    tmp = (x >> 4U);
    if (tmp != 0U) {
        n += 4U;
        x = tmp;
    }
    return n + log2LUT[x];
}
#endif // ndef QF_LOG2
//$enddef${QF::types::QF_LOG2} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
