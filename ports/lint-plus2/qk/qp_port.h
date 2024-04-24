//============================================================================
// QP/C Real-Time Embedded Framework (RTEF)
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
//! @date Last updated on: 2024-05-01
//! @version Last updated for: @ref qpc_7_3_4
//!
//! @file
//! @brief QP/C "port" to PC-Lint-Plus, QK kernel, generic C99

#ifndef QP_PORT_H_
#define QP_PORT_H_

#include <stdint.h>  // Exact-width types. WG14/N843 C99 Standard
#include <stdbool.h> // Boolean type.      WG14/N843 C99 Standard

#ifdef QP_CONFIG
#include "qp_config.h" // external QP configuration
#endif

// no-return function specifier (C11 Standard)
#define Q_NORETURN   [[noreturn]] void

// QF configuration for QK -- data members of the QActive class...

// QK event-queue used for AOs
#define QACTIVE_EQUEUE_TYPE  QEQueue

// QF "thread" type used to store the MPU settings in the AO
#define QACTIVE_THREAD_TYPE  void const *

// interrupt disabling mechanism
#define QF_INT_DISABLE()     intDisable()
#define QF_INT_ENABLE()      intEnable()

// QF critical section mechanism
#define QF_CRIT_STAT         uint32_t crit_stat_;
#define QF_CRIT_ENTRY()      (crit_stat_ = critEntry())
#define QF_CRIT_EXIT()       critExit(crit_stat_)

// Check if the code executes in the ISR context
#define QK_ISR_CONTEXT_()   (QK_get_IPSR() != 0U)

// Define the ISR entry sequence
#define QK_ISR_ENTRY()      ((void)0)

// Define the ISR exit sequence
#define QK_ISR_EXIT()  do {                                   \
    QF_INT_DISABLE();                                         \
    if (QK_sched_() != 0U) {                                  \
        *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (1U << 28U);\
    }                                                         \
    QF_INT_ENABLE();                                          \
} while (false)

void intDisable(void);
void intEnable(void);
uint32_t critEntry(void);
void critExit(uint32_t stat);
uint32_t QK_get_IPSR(void);

// include files -------------------------------------------------------------
#include "qequeue.h"  // QK kernel uses the native QP event queue
#include "qmpool.h"   // QK kernel uses the native QP memory pool
#include "qp.h"       // QP framework
#include "qk.h"       // QK kernel

#endif // QP_PORT_H_

