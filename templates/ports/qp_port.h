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
#ifndef QP_PORT_H_
#define QP_PORT_H_

//============================================================================
// !!! SUPER-TEMPLATE -- NOT COMPILABLE AS IS !!!
//
// This file collects in one place ALL port-specific customizations used by
// the QP/C ports: the built-in kernels QV, QK, and QXK (on ARM Cortex-M, ARM
// Cortex-R, and MSP430), the ports to third-party operating systems (POSIX
// and Win32, including their single-threaded QV variants), and the QUTest
// unit-testing ports. Each customizable element is defined exactly ONCE, as a
// representative example taken from one of the actual ports (the source port
// is indicated next to the definition). This file is the reference and the
// documentation anchor for the port interface. The definitions in the actual
// ports differ in detail. The ports to other third-party RTOSes (e.g., embOS,
// FreeRTOS, ThreadX, uC/OS-II, Zephyr) customize the same elements as the
// third-party OS section below.
//
// The file is organized in visually separated sections, labeled with the
// kernels (and CPU families) to which they apply. Sections shared by
// several kernels are labeled as such.
//
// To create an actual qp_port.h from this super-template:
// 1. Copy this file to the directory of the new port.
// 2. DELETE all sections (and elements) that do not apply to the selected
//    kernel and CPU.
// 3. Adapt the remaining definitions to the CPU, compiler, and kernel/OS.
// 4. Delete the #error directive below.
//============================================================================
#error "qp_port.h super-template is not compilable as is (see comment above)"

#include <stdint.h>     // Exact-width types. WG14/N843 C99 Standard
#include <stdbool.h>    // Boolean type.      WG14/N843 C99 Standard
#include <stddef.h>     // size_t type        ISO/IEC 9899:1990 Standard
#include "qp_config.h"  // QP configuration from the application

//############################################################################
// SECTION: ALL PORTS (QV, QK, QXK, third-party OS, QUTest)
//
// Elements needed by every port. The implementation of the critical
// section and of the interrupt disabling is CPU- and kernel-specific.
// The definitions below are taken from ports/arm-cm/qk/gnu.
//############################################################################

#ifdef __cplusplus
    // no-return function specifier (C++11 Standard)
    #define Q_NORETURN  [[ noreturn ]] void

    // static assertion (C++11 Standard)
    #define Q_ASSERT_STATIC(expr_)  static_assert((expr_), "QP static assert")
#else
    // no-return function specifier (C11 Standard)
    #define Q_NORETURN   _Noreturn void

    // static assertion (C11 Standard)
    #define Q_ASSERT_STATIC(expr_)  _Static_assert((expr_), "QP static assert")
#endif

// QActive event queue type (all ports except qep-only)
#define QACTIVE_EQUEUE_TYPE     QEQueue

// interrupt disabling policy (built-in kernels and embedded QUTest ports),
// see NOTE2 and NOTE3
#define QF_INT_DISABLE()        (QF_int_disable_())
#define QF_INT_ENABLE()         (QF_int_enable_())

// QF critical section (all ports except qep-only), see NOTE1, NOTE2, NOTE3
#define QF_CRIT_STAT
#define QF_CRIT_ENTRY()         (QF_crit_entry_())
#define QF_CRIT_EXIT()          (QF_crit_exit_())
#define QF_CRIT_EST()           (QF_crit_entry_())

// prototypes of the interrupt-disabling and critical-section functions
void QF_int_disable_(void);
void QF_int_enable_(void);
void QF_crit_entry_(void);
void QF_crit_exit_(void);

//############################################################################
// SECTION: CPU FAMILY -- ARM Cortex-M and ARM Cortex-R
// (built-in kernels QV, QK, QXK; QF_LOG2 also in the ARM Cortex-M QUTest port)
//
// Taken from ports/arm-cm/qk/gnu, except QF_INT_ENABLE_ALL()
// (ARM Cortex-R only, taken from ports/arm-cr/qk/gnu).
//############################################################################

// no-operation after exiting a critical section (instruction barrier)
#define QF_CRIT_EXIT_NOP()      __asm volatile ("isb" ::: "memory")

// Efficient log2()
#if (__ARM_ARCH == 6) // ARMv6-M?
    // hand-optimized LOG2 in assembly for Cortex-M0/M0+/M1(v6-M, v6S-M)
    #define QF_LOG2(n_) QF_qlog2((uint32_t)(n_))

    // hand-optimized quick LOG2 in assembly
    uint_fast8_t QF_qlog2(uint32_t x);
#else // ARMv7-M or higher
    // ARMv7-M or higher provide the CLZ instruction for fast LOG2
    #define QF_LOG2(n_) ((uint_fast8_t)(32 - __builtin_clz((unsigned)(n_))))
#endif // ARMv7-M or higher

// ARM Cortex-R only: enable both IRQ and FIQ (THUMB mode)
#define QF_INT_ENABLE_ALL() __asm volatile ("cpsie if")

//############################################################################
// SECTION: CPU FAMILY -- ARM Cortex-M only (built-in kernels QV, QK, QXK)
//
// Taken from ports/arm-cm/qk/gnu.
//############################################################################

// Critical section policy
#ifdef QF_USE_BASEPRI
    // CMSIS threshold for "QF-aware" interrupts, see NOTE4
    #define QF_AWARE_ISR_CMSIS_PRI (QF_USE_BASEPRI >> (8 - __NVIC_PRIO_BITS))
#else
    // CMSIS threshold for "QF-aware" interrupts, see NOTE2 and NOTE4
    #define QF_AWARE_ISR_CMSIS_PRI 0
#endif // QF_USE_BASEPRI

//############################################################################
// SECTION: ACTIVE OBJECT "THREAD" AND "OS-OBJECT" TYPES -- shared
//
// QACTIVE_THREAD_TYPE: ARM Cortex-M built-in kernels (QV, QK, QXK), where it
// stores the MPU settings of the AO, and the POSIX/Win32 ports.
// QACTIVE_OS_OBJ_TYPE: QXK kernel, where it stores the private stack pointer
// of eXtended threads, and the POSIX/Win32 ports, where it is the OS object
// used to block the AO thread (e.g., pthread_cond_t in POSIX).
//
// Taken from ports/arm-cm/qxk/gnu.
//############################################################################

// QActive "thread" type used to store the MPU settings in the AO
#define QACTIVE_THREAD_TYPE     void const *

// QActive OS-Object type used for the private stack pointer for
// eXtended threads (the private stack pointer is NULL for basic-threads)
#define QACTIVE_OS_OBJ_TYPE     void*

//############################################################################
// SECTION: QV KERNEL (non-preemptive, built-in)
//
// QV_CPU_SLEEP(): ARM Cortex-M and ARM Cortex-R QV ports;
// all other elements: ARM Cortex-M QV ports.
// Taken from ports/arm-cm/qv/gnu.
//############################################################################

// QV kernel sleep mode transition
#ifdef QF_USE_BASEPRI
    // macro to put the CPU to sleep inside QV_onIdle()
    #define QV_CPU_SLEEP() do {                  \
        __asm volatile ("cpsid i" ::: "memory"); \
        QF_INT_ENABLE();                         \
        __asm volatile ("wfi" ::: "memory");     \
        __asm volatile ("cpsie i" ::: "memory"); \
    } while (false)
#else // no BASEPRI (QV uses PRIMASK)
    // macro to put the CPU to sleep inside QV_onIdle()
    #define QV_CPU_SLEEP() do {             \
        __asm volatile ("wfi"::: "memory"); \
        QF_INT_ENABLE();                    \
    } while (false)
#endif // no BASEPRI (QV uses PRIMASK)

#if (__ARM_ARCH == 6) // ARMv6-M?
    #define QV_ARM_ERRATUM_838869() ((void)0)
#else // ARMv7-M or higher
    // The following macro implements the recommended workaround for the
    // ARM Erratum 838869. Specifically, for Cortex-M3/M4/M7 the DSB
    // (memory barrier) instruction needs to be added before exiting an ISR.
    // This macro should be inserted at the end of ISRs.
    #define QV_ARM_ERRATUM_838869() __asm volatile ("dsb" ::: "memory")
#endif

// initialization of the QV kernel
#define QV_INIT()  QV_init()
void QV_init(void);

#ifdef __ARM_FP         //--------- if VFP available...
// When the FPU is configured, clear the FPCA bit in the CONTROL register
// to prevent wasting the stack space for the FPU context.
#define QV_START()     __asm volatile ("msr CONTROL,%0" :: "r" (0) : "memory")
#endif

// include files
#include "qequeue.h"   // QV kernel uses the native QP event queue
#include "qmpool.h"    // QV kernel uses the native QP memory pool
#include "qp.h"        // QP framework
#include "qv.h"        // QV kernel

//############################################################################
// SECTION: QK KERNEL (preemptive, non-blocking, built-in)
//
// QK_ISR_CONTEXT_(): ARM Cortex-M, ARM Cortex-R, and MSP430 QK ports;
// QK_ISR_ENTRY()/QK_ISR_EXIT(): ARM Cortex-M and MSP430 QK ports;
// QK_IRQ_BEGIN()/QK_IRQ_END(): ARM Cortex-R QK ports (taken from
// ports/arm-cr/qk/gnu); all other elements: ARM Cortex-M QK ports
// (taken from ports/arm-cm/qk/gnu).
//############################################################################

// determination if the code executes in the ISR context
#define QK_ISR_CONTEXT_()     (QK_get_IPSR() != 0U)

//! @cond INTERNAL
__attribute__((always_inline))
static inline uint32_t QK_get_IPSR(void) {
    uint32_t regIPSR;
    __asm volatile ("mrs %0,ipsr" : "=r" (regIPSR));
    return regIPSR;
}
//! @endcond

// QK ISR entry and exit
#define QK_ISR_ENTRY() ((void)0)

    #define QK_ISR_EXIT()  do {                                   \
        QF_INT_DISABLE();                                         \
        if (QK_sched_() != 0U) {                                  \
            *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (1U << 28U);\
        }                                                         \
        QF_INT_ENABLE();                                          \
        QK_ARM_ERRATUM_838869();                                  \
    } while (false)

#if (__ARM_ARCH == 6) // ARMv6-M?
    #define QK_ARM_ERRATUM_838869() ((void)0)
#else // ARMv7-M or higher
    // The following macro implements the recommended workaround for the
    // ARM Erratum 838869. Specifically, for Cortex-M3/M4/M7 the DSB
    // (memory barrier) instruction needs to be added before exiting an ISR.
    // This macro should be inserted at the end of "kernel-unaware" ISRs.
    #define QK_ARM_ERRATUM_838869() __asm volatile ("dsb" ::: "memory")
#endif // ARMv6-M

// initialization of the QK kernel
#define QK_INIT()     QK_init()

#ifdef __ARM_FP         //--------- if VFP available...
// When the FPU is configured, clear the FPCA bit in the CONTROL register
// to prevent wasting the stack space for the FPU context.
#define QK_START()     __asm volatile ("msr CONTROL,%0" :: "r" (0) : )
#endif

// ARM Cortex-R only: QK-specific Interrupt Request handler BEGIN
#ifdef __ARM_FP
#define QK_IRQ_BEGIN(name_)                    \
    void name_(void)                           \
    __attribute__ ((interrupt ("irq")));       \
    __attribute__ ((naked)) void name_(void) { \
    __asm volatile (" SUB LR, LR, #4\n"        \
    " SRSDB #31!\n"                            \
    " CPS #31\n"                               \
    " PUSH {R0-R3, R12}");                     \
    __asm(" FMRX R12, FPSCR\n"                 \
    " STMFD SP!, {R12}\n"                      \
    " FMRX R12, FPEXC\n"                       \
    " STMFD SP!, {R12}\n"                      \
    " FSTMDBD SP!, {D0-D7}");                  \
    __asm(" AND R3, SP, #4\n"                  \
    " SUB SP, SP, R3\n"                        \
    " PUSH {R3, LR}\n");                       \
    ++QK_priv_.intNest; {
#else
#define QK_IRQ_BEGIN(name_)                    \
    void name_(void)                           \
    __attribute__ ((target ("arm")));          \
    __attribute__ ((naked)) void name_(void) { \
    __asm volatile (" SUB LR, LR, #4\n"        \
    " SRSDB #31!\n"                            \
    " CPS #31\n"                               \
    " PUSH {R0-R3, R12}");                     \
    __asm(" AND R3, SP, #4\n"                  \
    " SUB SP, SP, R3\n"                        \
    " PUSH {R3, LR}\n");                       \
    ++QK_priv_.intNest; {
#endif

// ARM Cortex-R only: QK-specific Interrupt Request handler END
#ifdef __ARM_FP
#define QK_IRQ_END()                  \
    } --QK_priv_.intNest;             \
    if (QK_priv_.intNest == 0U) {     \
        if (QK_sched_() != 0U) {      \
            QK_activate_();           \
        }                             \
    }                                 \
    __asm volatile (" POP {R3, LR}\n" \
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
    __asm volatile (" POP {R3, LR}\n" \
    " ADD SP, SP, R3");               \
    __asm volatile (" POP {R0-R3, R12}\n" \
    " RFEIA SP!");                    \
}
#endif

// include files
#include "qequeue.h"   // QK kernel uses the native QP event queue
#include "qmpool.h"    // QK kernel uses the native QP memory pool
#include "qp.h"        // QP framework
#include "qk.h"        // QK kernel

// prototypes
void QK_init(void);
void QK_thread_ret(void);

//############################################################################
// SECTION: QXK KERNEL (preemptive, dual-mode, built-in)
//
// ARM Cortex-M QXK ports. Taken from ports/arm-cm/qxk/gnu.
// (QACTIVE_OS_OBJ_TYPE for QXK is defined in the shared section above.)
//############################################################################

// determination if the code executes in the ISR context
#define QXK_ISR_CONTEXT_()     (QXK_get_IPSR() != 0U)

//! @cond INTERNAL
__attribute__((always_inline))
static inline uint32_t QXK_get_IPSR(void) {
    uint32_t regIPSR;
    __asm volatile ("mrs %0,ipsr" : "=r" (regIPSR));
    return regIPSR;
}
//! @endcond

// trigger the PendSV exception to perform the context switch
#define QXK_CONTEXT_SWITCH_()  \
    *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (1U << 28U)

// QXK ISR entry and exit
#define QXK_ISR_ENTRY() ((void)0)

    #define QXK_ISR_EXIT()  do {                                  \
        QF_INT_DISABLE();                                         \
        if (QXK_sched_() != 0U) {                                 \
            *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (1U << 28U);\
        }                                                         \
        QF_INT_ENABLE();                                          \
        QXK_ARM_ERRATUM_838869();                                 \
    } while (false)

#if (__ARM_ARCH == 6) // ARMv6-M?
    #define QXK_ARM_ERRATUM_838869() ((void)0)
#else // ARMv7-M or higher
    // The following macro implements the recommended workaround for the
    // ARM Erratum 838869. Specifically, for Cortex-M3/M4/M7 the DSB
    // (memory barrier) instruction needs to be added before exiting an ISR.
    // This macro should be inserted at the end of "kernel-unaware" ISRs.
    #define QXK_ARM_ERRATUM_838869() __asm volatile ("dsb" ::: "memory")
#endif // ARMv6-M

// initialization of the QXK kernel
#define QXK_INIT()      QXK_init()

#ifdef __ARM_FP         //--------- if VFP available...
// When the FPU is configured, clear the FPCA bit in the CONTROL register
// to prevent wasting the stack space for the FPU context.
#define QXK_START()     __asm volatile ("msr CONTROL,%0" :: "r" (0) : )
#endif

// include files
#include "qequeue.h"   // QXK kernel uses the native QP event queue
#include "qmpool.h"    // QXK kernel uses the native QP memory pool
#include "qp.h"        // QP framework
#include "qxk.h"       // QXK kernel

// prototypes
void QXK_init(void);
void QXK_thread_ret(void);

//############################################################################
// SECTION: THIRD-PARTY OS PORTS AND QUTest PORTS -- shared
//
// POSIX and Win32 ports (including the single-threaded posix-qv and win32-qv
// variants) and all QUTest ports. With the built-in kernels (QV, QK, QXK),
// these operations are provided by the kernel headers (qv.h, qk.h, qxk.h)
// and must NOT be defined in the port. The definitions below are taken from
// ports/posix (the QUTest ports define the scheduler locking and the event-
// queue signaling differently, see ports/arm-cm/qutest).
//############################################################################

#include <pthread.h>   // POSIX-thread API (POSIX port)

// include files
#include "qequeue.h"   // the port uses the native QP event queue
#include "qmpool.h"    // the port uses the native QP memory pool
#include "qp.h"        // QP platform-independent public interface

// interface used only inside QF implementation, but not in applications
#ifdef QP_IMPL

// QF scheduler locking (not used in the POSIX port)
#define QF_SCHED_STAT_
#define QF_SCHED_LOCK_(ceil_) ((void)0)
#define QF_SCHED_UNLOCK_()    ((void)0)

// QF event queue customization (blocking and signaling of the AO thread)
#define QACTIVE_EQUEUE_WAIT_(me_) do { \
    while ((me_)->eQueue.frontEvt.e == (QEvt *)0) { \
        Q_ASSERT_INCRIT(400, QF_critSectNest_ == 1); \
        --QF_critSectNest_; \
        pthread_cond_wait(&(me_)->osObject, &QF_critSectMutex_); \
        Q_ASSERT_INCRIT(401, QF_critSectNest_ == 0); \
        ++QF_critSectNest_; \
    } \
} while (false)

#define QACTIVE_EQUEUE_SIGNAL_(me_) \
    pthread_cond_signal(&(me_)->osObject)

// QMPool operations
#define QF_EPOOL_TYPE_  QMPool
#define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
            (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
#define QF_EPOOL_EVENT_SIZE_(p_)  (QMPool_getBlockSize(&(p_)))
#define QF_EPOOL_GET_(p_, e_, m_, qsId_) \
            ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qsId_)))
#define QF_EPOOL_PUT_(p_, e_, qsId_) (QMPool_put(&(p_), (e_), (qsId_)))
#define QF_EPOOL_USE_(ePool_)   (QMPool_getUse(ePool_))
#define QF_EPOOL_FREE_(ePool_)  ((uint16_t)(ePool_)->nFree)
#define QF_EPOOL_MIN_(ePool_)   ((uint16_t)(ePool_)->nMin)

// mutex for QF critical section (POSIX port)
extern pthread_mutex_t QF_critSectMutex_;
extern int_t QF_critSectNest_;

#endif // QP_IMPL

//============================================================================
// NOTES (ARM Cortex-M built-in kernels)
//
// NOTE1:
// The critical section policy does not use the "saving and restoring"
// interrupt status policy (macro QF_CRIT_STAT is defined to nothing).
// However, this the QF critical sections might still be able to nest,
// depending on the implementation of the QF_crit_entry_()/QF_crit_exit_()
// functions. They are defined as "weak" in the qv_port.c module,
// so the application can provide a different implementation.
// Please see the definitions of the interrupt and critical-section
// functions in the qv_port.c module for details.
//
// NOTE2:
// On Cortex-M0/M0+/M1 (architecture ARMv6-M, ARMv6S-M), the interrupt
// disabling policy uses the PRIMASK register to disable interrupts globally.
// The QF_AWARE_ISR_CMSIS_PRI level is zero, meaning that all interrupts
// are "kernel-aware".
//
// NOTE3:
// If macro QF_USE_BASEPRI is defined, the interrupt disable/enable policy
// uses BASEPRI register to disable interrupts only with priority lower than
// the threshold specified by the QF_USE_BASEPRI macro. The interrupts with
// priorities above QF_USE_BASEPRI (i.e., with numerical priority values
// lower than QF_USE_BASEPRI) are NOT disabled in this method.
// These free-running interrupts have very low ("zero") latency, but they
// are NOT allowed to call any QF services, because QF is unaware of them
// ("kernel-unaware" interrupts). Consequently, only interrupts with
// numerical values of priorities equal to or higher than QF_USE_BASEPRI
// ("kernel-aware" interrupts ), can call QF services.
//
// NOTE4:
// The QF_AWARE_ISR_CMSIS_PRI macro is useful as an offset for enumerating
// the "QF-aware" interrupt priorities in the applications, whereas the
// numerical values of the "QF-aware" interrupts must be greater or equal to
// QF_AWARE_ISR_CMSIS_PRI. The values based on QF_AWARE_ISR_CMSIS_PRI can be
// passed directly to the CMSIS function NVIC_SetPriority(), which shifts
// them by (8 - __NVIC_PRIO_BITS) into the correct bit position, while
// __NVIC_PRIO_BITS is the CMSIS macro defining the number of implemented
// priority bits in the NVIC. Please note that the macro QF_AWARE_ISR_CMSIS_PRI
// is intended only for applications and is not used inside the QF port, which
// remains generic and not dependent on the number of implemented priority bits
// implemented in the NVIC.

#endif // QP_PORT_H_
