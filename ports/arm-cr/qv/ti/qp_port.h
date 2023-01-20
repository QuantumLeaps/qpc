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
//! @date Last updated on: 2023-09-07
//! @version Last updated for: @ref qpc_7_3_0
//!
//! @file
//! @brief QP/C port to ARM Cortex-R, cooperative QV kernel, TI-ARM

#ifndef QP_PORT_H_
#define QP_PORT_H_

#include <stdint.h>  // Exact-width types. WG14/N843 C99 Standard
#include <stdbool.h> // Boolean type.      WG14/N843 C99 Standard

#ifdef QP_CONFIG
#include "qp_config.h" // external QP configuration
#endif

// no-return function specifier (C11 Standard)
#define Q_NORETURN   _Noreturn void

// QF configuration for QV -- data members of the QActive class...

// QV event-queue used for AOs
#define QACTIVE_EQUEUE_TYPE     QEQueue

// QF interrupt disable/enable, see NOTE2
#ifdef __16bis__
    #define QF_INT_DISABLE()    __asm(" CPSID i")
    #define QF_INT_ENABLE()     __asm(" CPSIE i")
    #define QF_INT_ENABLE_ALL() __asm(" CPSIE if")
#else
    #define QF_INT_DISABLE()    _disable_IRQ()
    #define QF_INT_ENABLE()     _enable_IRQ()
    #define QF_INT_ENABLE_ALL() _enable_interrupts()
#endif

// Cortex-R provide the CLZ instruction for fast LOG2
#define QF_LOG2(n_) ((uint8_t)(32U - __clz(n_)))

// QF critical section entry/exit, see NOTE3
#define QF_CRIT_STAT            uint32_t cpsr_;
#define QF_CRIT_ENTRY() do {          \
    cpsr_ = _get_CPSR();              \
    QF_INT_DISABLE();                 \
} while (false)
#define QF_CRIT_EXIT()  do {          \
    if ((cpsr_ & (1U << 7U)) == 0U) { \
        QF_INT_ENABLE();              \
    }                                 \
} while (false)
#define QF_CRIT_EXIT_NOP()     __asm(" ISB")

// macro to put the CPU to sleep inside QV_onIdle()
#define QV_CPU_SLEEP() do { \
    __wfi(); \
    QF_INT_ENABLE(); \
} while (false)

// include files -------------------------------------------------------------
#include "qequeue.h"   // QV kernel uses the native QP event queue
#include "qmpool.h"    // QV kernel uses the native QP memory pool
#include "qp.h"        // QP framework
#include "qv.h"        // QV kernel

//============================================================================
// NOTE2:
// The FIQ-type interrupts are NEVER disabled in this port, so the FIQ is
// a "kernel-unaware" interrupt. If the FIQ is ever used in the application,
// it must be an "ARM FIQ"-type function. For this to work, the FIQ
// stack needs to be initialized.
//
// NOTE3:
// This port implements the "save and restore" interrupt status policy,
// which again never disables the FIQ-type interrupts. This policy allows
// for nesting critical sections, which is necessary inside IRQ-type
// interrupts that run with interrupts (IRQ) disabled.
//

#endif // QP_PORT_H_

