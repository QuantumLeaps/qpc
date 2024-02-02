//============================================================================
// QP/C Real-Time Embedded Framework (RTEF)
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC <state-machine.com>.
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
//! @brief QK/C port to ARM Cortex-M, ARM-CLANG

#define QP_IMPL 1U
#include "qp_port.h"
#include "qsafe.h"        // QP Functional Safety (FuSa) Subsystem

// prototypes ----------------------------------------------------------------
void PendSV_Handler(void);
#ifdef QK_USE_IRQ_HANDLER           // if use IRQ...
void QK_USE_IRQ_HANDLER(void);
#else                               // use default (NMI)
void NMI_Handler(void);
#endif

#define SCB_SYSPRI   ((uint32_t volatile *)0xE000ED18U)
#define NVIC_EN      ((uint32_t volatile *)0xE000E100U)
#define NVIC_IP      ((uint32_t volatile *)0xE000E400U)
#define SCB_CPACR   *((uint32_t volatile *)0xE000ED88U)
#define FPU_FPCCR   *((uint32_t volatile *)0xE000EF34U)
#define NVIC_PEND    0xE000E200
#define SCB_ICSR     0xE000ED04

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
char const QF_port_module_[] = "qk_port";

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
// The interrupt priorities established in QK_init() can be later
// changed by the application-level code.
void QK_init(void) {

#if (__ARM_ARCH != 6)   //--------- if ARMv7-M and higher...

    // SCB_SYSPRI[2]:  SysTick
    SCB_SYSPRI[2] = (SCB_SYSPRI[2] | (QF_BASEPRI << 24U));

    // set all 240 possible IRQ priories to QF_BASEPRI...
    for (uint_fast8_t n = 0U; n < (240U/sizeof(uint32_t)); ++n) {
        NVIC_IP[n] = (QF_BASEPRI << 24U) | (QF_BASEPRI << 16U)
                     | (QF_BASEPRI << 8U) | QF_BASEPRI;
    }

#endif                  //--------- ARMv7-M or higher

    // SCB_SYSPRI[2]: PendSV set to priority 0xFF (lowest)
    SCB_SYSPRI[2] = (SCB_SYSPRI[2] | (0xFFU << 16U));

#ifdef QK_USE_IRQ_NUM   //--------- QK IRQ specified?
    // The QK port is configured to use a given ARM Cortex-M IRQ #
    // to return to thread mode (default is to use the NMI exception)
    NVIC_IP[QK_USE_IRQ_NUM] = 0U; // priority 0 (highest)
    NVIC_EN[QK_USE_IRQ_NUM >> 5U] = (1U << (QK_USE_IRQ_NUM & 0x1FU));
#endif                  //--------- QK IRQ specified

#ifdef __ARM_FP         //--------- if VFP available...
    // make sure that the FPU is enabled by setting CP10 & CP11 Full Access
    SCB_CPACR = (SCB_CPACR | ((3UL << 20U) | (3UL << 22U)));

    // FPU automatic state preservation (ASPEN) lazy stacking (LSPEN)
    FPU_FPCCR = (FPU_FPCCR | (1U << 30U) | (1U << 31U));
#endif                  //--------- VFP available
}

//............................................................................
// The PendSV_Handler exception is used for handling the asynchronous
// preemption in QK. The use of the PendSV exception is the recommended and
// the most efficient method for performing context switches with ARM Cortex-M.
//
// The PendSV exception should have the lowest priority in the whole system
// (0xFF, see QK_init). All other exceptions and interrupts should have higher
// priority. For example, for NVIC with 2 priority bits all interrupts and
// exceptions must have numerical value of priority lower than 0xC0. In this
// case the interrupt priority levels available to your applications are (in
// the order from the lowest urgency to the highest urgency): 0x80, 0x40, 0x00.
//
// Also, *all* "kernel aware" ISRs in the QK application must call the
// QK_ISR_EXIT() macro, which triggers PendSV when it detects a need for
// a context switch or asynchronous preemption.
//
// Due to tail-chaining and its lowest priority, the PendSV exception will be
// entered immediately after the exit from the *last* nested interrupt (or
// exception). In QK, this is exactly the time when the QK activator needs to
// handle the asynchronous preemption.
__attribute__ ((naked))
void PendSV_Handler(void) {
__asm volatile (
    "  PUSH    {r0,lr}          \n" // save stack-aligner + EXC_RETURN

    //<<<<<<<<<<<<<<<<<<<<<<< CRITICAL SECTION BEGIN <<<<<<<<<<<<<<<<<<<<<<<<
    "  LDR     r0,=QF_int_disable_ \n"
    "  BLX     r0               \n" // call QF_int_disable_()

#ifdef QF_MEM_ISOLATE
    "  LDR     r0,=QF_onMemSys  \n"
    "  BLX     r0               \n" // call QF_onMemSys()
#endif

    // The PendSV exception handler can be preempted by an interrupt,
    // which might pend PendSV exception again. The following write to
    // ICSR[27] un-pends any such spurious instance of PendSV.
    "  LDR     r2,=" STRINGIFY(SCB_ICSR) "\n" // Interrupt Control and State
    "  MOVS    r1,#1            \n"
    "  LSLS    r1,r1,#27        \n" // r1 := (1 << 27) (UNPENDSVSET bit)
    "  STR     r1,[r2]          \n" // ICSR[27] := 1 (unpend PendSV)

    // The QK activator must be called in a Thread mode, while this code
    // executes in the Handler mode of the PendSV exception. The switch
    // to the Thread mode is accomplished by returning from PendSV using
    // a fabricated exception stack frame, where the return address is
    // QK_activate_().
    //
    // NOTE: the QK activator is called with interrupts DISABLED and also
    // returns with interrupts DISABLED.
    "  LSRS    r3,r1,#3         \n" // r3 := (r1 >> 3), set T bit (new xpsr)
    "  LDR     r2,=QK_activate_ \n" // address of QK_activate_
    "  SUBS    r2,r2,#1         \n" // align Thumb-address at halfword (new pc)
    "  LDR     r1,=QK_thread_ret \n" // return address after the call  (new lr)

    "  SUB     sp,sp,#8*4       \n" // reserve space for exception stack frame
    "  ADD     r0,sp,#5*4       \n" // r0 := 5 registers below the SP
    "  STM     r0!,{r1-r3}      \n" // save xpsr,pc,lr

    "  MOVS    r0,#6            \n"
    "  MVNS    r0,r0            \n" // r0 := ~6 == 0xFFFFFFF9
#if (__ARM_ARCH != 6)   //--------- if ARMv7-M and higher...
    "  DSB                      \n" // ARM Erratum 838869
#endif                  //--------- ARMv7-M and higher
    "  BX      r0               \n" // exception-return to the QK activator
    );
}

//============================================================================
// QK_thread_ret is a helper function executed when the QXK activator returns.
//
// NOTE: QK_thread_ret does not execute in the PendSV context!
// NOTE: QK_thread_ret is entered with interrupts DISABLED.
__attribute__ ((naked, used))
void QK_thread_ret(void) {
__asm volatile (

    // After the QK activator returns, we need to resume the preempted
    // thread. However, this must be accomplished by a return-from-exception,
    // while we are still in the thread context. The switch to the exception
    // context is accomplished by triggering the NMI exception or the selected
    // IRQ (if macro #QK_USE_IRQ_NUM is defined).

    // before triggering the NMI/IRQ, make sure that the VFP stack frame
    // will NOT be used...
#ifdef __ARM_FP         //--------- if VFP available...
    // make sure that the VFP stack frame will NOT be used
    "  MRS     r0,CONTROL       \n" // r0 := CONTROL
    "  BICS    r0,r0,#4         \n" // r0 := r0 & ~4 (FPCA bit)
    "  MSR     CONTROL,r0       \n" // CONTROL := r0 (clear CONTROL[2] FPCA bit)
    "  ISB                      \n" // ISB after MSR CONTROL (ARM AN321,Sect.4.16)
#endif                  //--------- VFP available

#ifdef QF_MEM_ISOLATE
    "  LDR     r0,=QF_onMemApp  \n"
    "  BLX     r0               \n" // call QF_onMemApp()
#endif

#ifndef QK_USE_IRQ_NUM  //--------- IRQ NOT defined, use NMI by default
    "  LDR     r0,=" STRINGIFY(SCB_ICSR) "\n" // Interrupt Control and State
    "  MOVS    r1,#1            \n"
    "  LSLS    r1,r1,#31        \n" // r1 := (1 << 31) (NMI bit)
    "  STR     r1,[r0]          \n" // ICSR[31] := 1 (pend NMI)

#else                   //--------- use the selected IRQ
    "  LDR     r0,=" STRINGIFY(NVIC_PEND + ((QK_USE_IRQ_NUM >> 5) << 2)) "\n"
    "  MOVS    r1,#1            \n"
    "  LSLS    r1,r1,#" STRINGIFY(QK_USE_IRQ_NUM & 0x1F) "\n" // r1 := IRQ bit
    "  STR     r1,[r0]          \n" // pend the IRQ

    // now enable interrupts so that pended IRQ can be entered
    "  LDR     r0,=QF_int_enable_ \n"
    "  BLX     r0               \n" // call QF_int_enable_()
#endif                  //--------- use IRQ

    // NOTE! interrupts are still disabled when NMI is used
    "  B       .                \n" // wait for preemption by NMI/IRQ
    );
}

//============================================================================
// This exception handler is used for returning back to the preempted thread.
// The exception handler simply removes its own interrupt stack frame from
// the stack (MSP) and returns to the preempted task using the interrupt
// stack frame that must be at the top of the stack.
__attribute__ ((naked))
#ifndef QK_USE_IRQ_NUM  //--------- IRQ NOT defined, use NMI by default

// NOTE: The NMI_Handler() is entered with interrupts still DISABLED!
void NMI_Handler(void) {
__asm volatile (
    // enable interrupts
    "  LDR     r0,=QF_int_enable_ \n"
    "  BLX     r0               \n" // call QF_int_enable_()
);

#else                   //--------- use the selected IRQ

// NOTE: The IRQ Handler is entered with interrupts already ENABLED
void QK_USE_IRQ_HANDLER(void) {
#endif                  //--------- use IRQ

  // ...continue here from either NMI_Handler() or IRQ_Handler()
__asm volatile (
    "  ADD     sp,sp,#(8*4)     \n" // remove one 8-register exception frame
    "  POP     {r0,pc}          \n" // pop stack aligner and EXC_RETURN to pc
    );
}

//============================================================================
#if (__ARM_ARCH == 6) // if ARMv6-M...

// hand-optimized quick LOG2 in assembly (no CLZ instruction in ARMv6-M)
__attribute__ ((naked))
uint_fast8_t QF_qlog2(uint32_t x) {
__asm volatile (
    "  MOVS    r1,#0            \n"
#if (QF_MAX_ACTIVE > 16U)
    "  LSRS    r2,r0,#16        \n"
    "  BEQ     QF_qlog2_1       \n"
    "  MOVS    r1,#16           \n"
    "  MOVS    r0,r2            \n"
    "QF_qlog2_1:                \n"
#endif
#if (QF_MAX_ACTIVE > 8U)
    "  LSRS    r2,r0,#8         \n"
    "  BEQ     QF_qlog2_2       \n"
    "  ADDS    r1, r1,#8        \n"
    "  MOVS    r0, r2           \n"
    "QF_qlog2_2:                \n"
#endif
    "  LSRS    r2,r0,#4         \n"
    "  BEQ     QF_qlog2_3       \n"
    "  ADDS    r1,r1,#4         \n"
    "  MOV     r0,r2            \n"
    "QF_qlog2_3:                \n"
    "  LDR     r2,=QF_qlog2_LUT \n"
    "  LDRB    r0,[r2,r0]       \n"
    "  ADDS    r0,r1,r0         \n"
    "  BX      lr               \n"
    "  .align                   \n"
    "QF_qlog2_LUT:              \n"
    "  .byte 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4"
    );
}

#endif // ARMv6-M

