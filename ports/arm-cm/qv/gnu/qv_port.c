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
//! @date Last updated on: 2023-12-03
//! @version Last updated for: @ref qpc_7_3_1
//!
//! @file
//! @brief QV/C port to ARM Cortex-M, GNU-ARM

#define QP_IMPL 1U
#include "qp_port.h"
#include "qsafe.h"        // QP Functional Safety (FuSa) Subsystem

#define SCB_SYSPRI   ((uint32_t volatile *)0xE000ED18U)
#define NVIC_IP      ((uint32_t volatile *)0xE000E400U)
#define SCB_CPACR   *((uint32_t volatile *)0xE000ED88U)
#define FPU_FPCCR   *((uint32_t volatile *)0xE000EF34U)

// helper macros to "stringify" values
#define VAL(x) #x
#define STRINGIFY(x) VAL(x)

//============================================================================
// interrupts and critical section...
//
// NOTE:
// The following interrupt disabling/enabling as well as critical section
// entry/exit functions are defined as "weak" so that they can be
// re-implemented differently at the application level.
//
// NOTE:
// For best performance, these functions are implemented in assembly,
// but they can be implemented in C as well.

//int32_t volatile QF_int_lock_nest_; // not used
extern char const QF_port_module_[];
char const QF_port_module_[] = "qv_port";

//............................................................................
// Unconditionally disable interrupts.
//
// description:
// On ARMv6-M, interrupts are disabled with the PRIMASK register.
// On ARMv7-M and higher, interrupts are disabled *selectively* with the
// BASEPRI register.
// Additionally, the function also asserts that the interrupts are
// NOT disabled upon the entry to the function.
__attribute__ ((naked, weak)) void QF_int_disable_(void) {
__asm volatile (
#if (__ARM_ARCH == 6)   //--------- ARMv6-M architecture?
    "  MRS     r0,PRIMASK       \n" // r0 <- previous PRIMASK
    "  CPSID   i                \n" // set PRIMASK
#else                   //--------- ARMv7-M or higher
    "  MRS     r0,BASEPRI       \n" // r0 <- previous BASEPRI
    "  MOVS    r1,#" STRINGIFY(QF_BASEPRI) "\n"
    "  MSR     BASEPRI,r1       \n" // NOTE: Cortes-M7 erratum 837070 is OK
#endif                  //--------- ARMv7-M or higher
    "  CMP     r0,#0            \n" // assert(PRIMASK/BASEPRI == 0)
    "  BNE     QF_int_disable_error\n"
    "  BX      lr               \n"
    "QF_int_disable_error:      \n"
    "  LDR     r0,=QF_port_module_ \n"
    "  MOVS    r1,#100          \n"
    "  LDR     r2,=Q_onError    \n"
    "  BX      r2               \n"
    );
}
//............................................................................
// Unconditionally enable interrupts.
//
// description:
// On ARMv6-M, interrupts are enabled with the PRIMASK register.
// On ARMv7-M and higher, interrupts are enabled with the BASEPRI register.
// Additionally, the function also asserts that the interrupts ARE
// disabled upon the entry to the function.
__attribute__ ((naked, weak)) void QF_int_enable_(void) {
__asm volatile (
#if (__ARM_ARCH == 6)   //--------- ARMv6-M architecture?
    "  MRS     r0,PRIMASK       \n" // r0 <- previous PRIMASK
#else                   //--------- ARMv7-M or higher
    "  MRS     r0,BASEPRI       \n" // r0 <- previous BASEPRI
#endif                  //--------- ARMv7-M or higher
    "  CMP     r0,#0            \n" // assert(PRIMASK/BASEPRI != 0)
    "  BEQ     QF_int_enable_error\n"
#if (__ARM_ARCH == 6)   //--------- ARMv6-M architecture?
    "  CPSIE   i                \n" // clear PRIMASK
#else                   //--------- ARMv7-M or higher
    "  MOVS    r1,#0            \n"
    "  MSR     BASEPRI,r1       \n" // NOTE: Cortes-M7 erratum 837070 is OK
#endif                  //--------- ARMv7-M or higher
    "  BX      lr               \n"
    "QF_int_enable_error:       \n"
    "  LDR     r0,=QF_port_module_ \n"
    "  MOVS    r1,#101          \n"
    "  LDR     r2,=Q_onError    \n"
    "  BX      r2               \n"
    );
}
//............................................................................
// Enter QF critical section.
//
// description:
// On ARMv6-M, critical section is entered by disabling interrupts
// with the PRIMASK register.
// On ARMv7-M and higher, critical section is entered by disabling
// interrupts *selectively* with the BASEPRI register.
// Additionally, the function also asserts that the interrupts are
// NOT disabled upon the entry to the function.
//
// NOTE:
// The assertion means that this critical section CANNOT nest.
__attribute__ ((naked, weak)) void QF_crit_entry_(void) {
__asm volatile (
#if (__ARM_ARCH == 6)   //--------- ARMv6-M architecture?
    "  MRS     r0,PRIMASK       \n" // r0 <- previous PRIMASK
    "  CPSID   i                \n" // set PRIMASK
#else                   //--------- ARMv7-M or higher
    "  MRS     r0,BASEPRI       \n" // r0 <- previous BASEPRI
    "  MOVS    r1,#" STRINGIFY(QF_BASEPRI) "\n"
    "  MSR     BASEPRI,r1       \n" // NOTE: Cortes-M7 erratum 837070 is OK
#endif                  //--------- ARMv7-M or higher
    "  CMP     r0,#0            \n" // assert(PRIMASK/BASEPRI == 0)
    "  BNE     QF_crit_entry_error\n"
    "  BX      lr               \n"
    "QF_crit_entry_error:       \n"
    "  LDR     r0,=QF_port_module_ \n"
    "  MOVS    r1,#110          \n"
    "  LDR     r2,=Q_onError    \n"
    "  BX      r2               \n"
    );
}
//............................................................................
// Exit QF critical section.
//
// description:
// On ARMv6-M, critical section is exited by enabling interrupts
// with the PRIMASK register.
// On ARMv7-M and higher, critical section is exited by enabling
// interrupts with the BASEPRI register.
// Additionally, the function also asserts that the interrupts ARE
// disabled upon the entry to the function.
//
// NOTE:
// The assertion means that this critical section CANNOT nest.
__attribute__ ((naked, weak)) void QF_crit_exit_(void) {
__asm volatile (
#if (__ARM_ARCH == 6)   //--------- ARMv6-M architecture?
    "  MRS     r0,PRIMASK       \n" // r0 <- previous PRIMASK
#else                   //--------- ARMv7-M or higher
    "  MRS     r0,BASEPRI       \n" // r0 <- previous BASEPRI
#endif                  //--------- ARMv7-M or higher
    "  CMP     r0,#0            \n" // assert(PRIMASK/BASEPRI != 0)
    "  BEQ     QF_crit_exit_error\n"
#if (__ARM_ARCH == 6)   //--------- ARMv6-M architecture?
    "  CPSIE   i                \n" // clear PRIMASK
#else                   //--------- ARMv7-M or higher
    "  MOVS    r1,#0            \n"
    "  MSR     BASEPRI,r1       \n" // NOTE: Cortes-M7 erratum 837070 is OK
#endif                  //--------- ARMv7-M or higher
    "  BX      lr               \n"
    "QF_crit_exit_error:        \n"
    "  LDR     r0,=QF_port_module_ \n"
    "  MOVS    r1,#111          \n"
    "  LDR     r2,=Q_onError    \n"
    "  BX      r2               \n"
    );
}

//============================================================================
// Initialize the exception priorities and IRQ priorities to safe values.
//
// description:
// On ARMv7-M or higher, this QF port disables interrupts by means of the
// BASEPRI register. However, this method cannot disable interrupt
// priority zero, which is the default for all interrupts out of reset.
// The following code changes the SysTick priority and all IRQ priorities
// to the safe value QF_BASEPRI, which the QF critical section can disable.
// This avoids breaching of the QF critical sections in case the
// application programmer forgets to explicitly set priorities of all
// "kernel aware" interrupts.
//
// The interrupt priorities established in QV_init() can be later
// changed by the application-level code.
void QV_init(void) {

#if (__ARM_ARCH != 6)   //--------- if ARMv7-M and higher...

    // SCB_SYSPRI[2]:  SysTick
    SCB_SYSPRI[2] = (SCB_SYSPRI[2] | (QF_BASEPRI << 24U));

    // set all 240 possible IRQ priories to QF_BASEPRI...
    for (uint_fast8_t n = 0U; n < (240U/sizeof(uint32_t)); ++n) {
        NVIC_IP[n] = (QF_BASEPRI << 24U) | (QF_BASEPRI << 16U)
                     | (QF_BASEPRI << 8U) | QF_BASEPRI;
    }

#endif                  //--------- ARMv7-M or higher

#ifdef __ARM_FP         //--------- if VFP available...
    // make sure that the FPU is enabled by setting CP10 & CP11 Full Access
    SCB_CPACR = (SCB_CPACR | ((3UL << 20U) | (3UL << 22U)));

    // FPU automatic state preservation (ASPEN) lazy stacking (LSPEN)
    FPU_FPCCR = (FPU_FPCCR | (1U << 30U) | (1U << 31U));
#endif                  //--------- VFP available
}

//============================================================================
#if (__ARM_ARCH == 6) // if ARMv6-M...

// hand-optimized quick LOG2 in assembly (no CLZ instruction in ARMv6-M)
// NOTE:
// The inline GNU assembler does not accept mnemonics MOVS, LSRS and ADDS,
// but for ARMv6-M the mnemonics MOV, LSR and ADD always set the condition
// flags in the PSR.
__attribute__ ((naked, optimize("-fno-stack-protector")))
uint_fast8_t QF_qlog2(uint32_t x) {
    Q_UNUSED_PAR(x);
__asm volatile (
    "  MOV     r1,#0            \n"
#if (QF_MAX_ACTIVE > 16U)
    "  LSR     r2,r0,#16        \n"
    "  BEQ     QF_qlog2_1       \n"
    "  MOV     r1,#16           \n"
    "  MOV     r0,r2            \n"
    "QF_qlog2_1:                \n"
#endif
#if (QF_MAX_ACTIVE > 8U)
    "  LSR     r2,r0,#8         \n"
    "  BEQ     QF_qlog2_2       \n"
    "  ADD     r1, r1,#8        \n"
    "  MOV     r0, r2           \n"
    "QF_qlog2_2:                \n"
#endif
    "  LSR     r2,r0,#4         \n"
    "  BEQ     QF_qlog2_3       \n"
    "  ADD     r1,r1,#4         \n"
    "  MOV     r0,r2            \n"
    "QF_qlog2_3:                \n"
    "  LDR     r2,=QF_qlog2_LUT \n"
    "  LDRB    r0,[r2,r0]       \n"
    "  ADD     r0,r1,r0         \n"
    "  BX      lr               \n"
    "  .align                   \n"
    "QF_qlog2_LUT:              \n"
    "  .byte 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4"
    );
}

#endif // ARMv6-M

