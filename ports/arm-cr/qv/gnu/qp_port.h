//============================================================================
// QP/C Real-Time Embedded Framework (RTEF)
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
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
// The GPL (see <www.gnu.org/licenses/gpl-3.0>) does NOT permit the
// incorporation of the QP/C software into proprietary programs. Please
// contact Quantum Leaps for commercial licensing options, which expressly
// supersede the GPL and are designed explicitly for licensees interested
// in using QP/C in closed-source proprietary applications.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
//! @date Last updated on: 2024-11-22
//! @version Last updated for: @ref qpc_8_0_0
//!
//! @file
//! @brief QP/C port to ARM Cortex-R, cooperative QV kernel, GNU-ARM

#ifndef QP_PORT_H_
#define QP_PORT_H_

#include <stdint.h>     // Exact-width types. WG14/N843 C99 Standard
#include <stdbool.h>    // Boolean type.      WG14/N843 C99 Standard
#include "qp_config.h"  // QP configuration from the application

// no-return function specifier (C11 Standard)
#define Q_NORETURN   _Noreturn void

// QF configuration for QV -- data members of the QActive class...

// QV event-queue used for AOs
#define QACTIVE_EQUEUE_TYPE     QEQueue

// QF interrupt disable/enable, see NOTE2
#ifdef __thumb__  // THUMB mode?

    #define QF_INT_DISABLE()    __asm volatile ("cpsid i")
    #define QF_INT_ENABLE()     __asm volatile ("cpsie i")
    #define QF_INT_ENABLE_ALL() __asm volatile ("cpsie if")

#elif (defined __arm__) // ARM mode?

    #define QF_INT_DISABLE()   \
        __asm volatile ("MSR cpsr_c,#(0x1F | 0x80)" ::: "cc")
    #define QF_INT_ENABLE() \
        __asm volatile ("MSR cpsr_c,#(0x1F)" ::: "cc")
    #define QF_INT_ENABLE_ALL() \
        __asm volatile ("MSR cpsr_c,#(0x1F)" ::: "cc")
#else
    #error Incorrect CPU mode. Must be either __arm__ or __thumb__.
#endif

// Cortex-R provide the CLZ instruction for fast LOG2
#define QF_LOG2(n_) ((uint8_t)(32U - __builtin_clz(n_)))

// QF critical section entry/exit, see NOTE3
#define QF_CRIT_STAT            uint32_t cpsr_;
#define QF_CRIT_ENTRY() do { \
    __asm volatile ("MRS %0,cpsr" : "=r" (cpsr_) :: "cc"); \
    QF_INT_DISABLE(); \
} while (false)

#define QF_CRIT_EXIT() \
    __asm volatile ("MSR cpsr_c,%0" :: "r" (cpsr_) : "cc")

#define QF_CRIT_EXIT_NOP()     __asm volatile ("ISB")

// macro to put the CPU to sleep inside QV_onIdle()
#define QV_CPU_SLEEP() do { \
    __asm volatile ("WFI"); \
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

