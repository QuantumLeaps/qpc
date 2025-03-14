//============================================================================
// QP/C Real-Time Event Framework (RTEF)
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open-source GNU
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
//============================================================================
#ifndef QP_PORT_H_
#define QP_PORT_H_

#include <stdint.h>     // Exact-width types. WG14/N843 C99 Standard
#include <stdbool.h>    // Boolean type.      WG14/N843 C99 Standard
#include <intrinsics.h> // IAR intrinsic functions
#include "qp_config.h"  // QP configuration from the application

// no-return function specifier (C11 Standard)
#define Q_NORETURN   _Noreturn void

// QF configuration for QK -- data members of the QActive class...

// QK event-queue used for AOs
#define QACTIVE_EQUEUE_TYPE     QEQueue

// QF interrupt disable/enable, see NOTE2
#define QF_INT_DISABLE()        __disable_irq()
#define QF_INT_ENABLE()         __enable_irq()
#define QF_INT_ENABLE_ALL()     __enable_interrupt()

// Cortex-R provide the CLZ instruction for fast LOG2
#define QF_LOG2(n_) ((uint8_t)(32U - __CLZ(n_)))

// QF critical section entry/exit, see NOTE3
#define QF_CRIT_STAT            uint32_t cpsr_;
#define QF_CRIT_ENTRY() do { \
    cpsr_ = __get_CPSR(); \
    QF_INT_DISABLE();     \
} while (false)
#define QF_CRIT_EXIT()          __set_CPSR(cpsr_)
#define QF_CRIT_EXIT_NOP()      __ISB()

// Check if the code executes in the ISR context
#define QK_ISR_CONTEXT_() (QK_priv_.intNest != 0U)

// QK-specific Interrupt Request handler BEGIN
#ifdef __ARMVFP__
#define QK_IRQ_BEGIN(name_)              \
    __stackless __arm void name_(void) { \
    __asm(" SUB LR, LR, #4\n"            \
    " SRSDB #31!\n"                      \
    " CPS #31\n"                         \
    " PUSH {R0-R3, R12}");               \
    __asm(" FMRX R12, FPSCR\n"           \
    " STMFD SP!, {R12}\n"                \
    " FMRX R12, FPEXC\n"                 \
    " STMFD SP!, {R12}\n"                \
    " FSTMDBD SP!, {D0-D7}");            \
    __asm(" AND R3, SP, #4\n"            \
    " SUB SP, SP, R3\n"                  \
    " PUSH {R3, LR}\n");                 \
    ++QK_priv_.intNest; {
#else
#define QK_IRQ_BEGIN(name_)              \
    __stackless __arm void name_(void) { \
    __asm(" SUB LR, LR, #4\n"            \
    " SRSDB #31!\n"                      \
    " CPS #31\n"                         \
    " PUSH {R0-R3, R12}");               \
    __asm(" AND R3, SP, #4\n"            \
    " SUB SP, SP, R3\n"                  \
    " PUSH {R3, LR}\n");                 \
    ++QK_priv_.intNest; {
#endif

// QK-specific Interrupt Request handler END
#ifdef __ARMVFP__
#define QK_IRQ_END()                  \
    } --QK_priv_.intNest;             \
    if (QK_priv_.intNest == 0U) {     \
        if (QK_sched_() != 0U) {      \
            QK_activate_();           \
        }                             \
    }                                 \
    __asm(" POP {R3, LR}\n"           \
    " ADD SP, SP, R3");               \
    __asm(" FLDMIAD SP!, {D0-D7}\n"   \
    " LDMFD SP!, {R12}\n"             \
    " FMXR FPEXC, R12 \n"             \
    " LDMFD SP!, {R12} \n"            \
    " FMXR FPSCR, R12");              \
    __asm(" POP {R0-R3, R12}\n"       \
    " RFEIA SP!");                    \
}
#else
#define QK_IRQ_END()                  \
    } --QK_priv_.intNest;             \
    if (QK_priv_.intNest == 0U) {     \
        if (QK_sched_() != 0U) {      \
            QK_activate_();           \
        }                             \
    }                                 \
    __asm(" POP {R3, LR}\n"           \
    " ADD SP, SP, R3");               \
    __asm(" POP {R0-R3, R12}\n"       \
    " RFEIA SP!");                    \
}
#endif

// include files -------------------------------------------------------------
#include "qequeue.h"   // QK kernel uses the native QP event queue
#include "qmpool.h"    // QK kernel uses the native QP memory pool
#include "qp.h"        // QP framework
#include "qk.h"        // QK kernel

//============================================================================
// NOTE2:
// The FIQ-type interrupts are NEVER disabled in this port, so the FIQ is
// a "kernel-unaware" interrupt. If the FIQ is ever used in the application,
// it must be an "__arm __fiq"-type function. For this to work, the FIQ
// stack needs to be initialized.
//
// NOTE3:
// This port implements the "save and restore" interrupt status policy,
// which again never disables the FIQ-type interrupts. This policy allows
// for nesting critical sections, which is necessary inside IRQ-type
// interrupts that run with interrupts (IRQ) disabled.
//

#endif // QP_PORT_H_

