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
//! @date Last updated on: 2023-12-04
//! @version Last updated for: @ref qpc_7_3_1
//!
//! @file
//! @brief QXK/C port to ARM Cortex-M, ARM-CLANG

#define QP_IMPL 1U
#include "qp_port.h"
#include "qp_pkg.h"
#include "qsafe.h"        // QP Functional Safety (FuSa) System

#include <stddef.h>       // for offsetof()

//============================================================================
// NOTE: keep in synch with struct QXK_Attr in "qxk.h" !!!
#define QXK_CURR       0
#define QXK_NEXT       4
#define QXK_ACT_PRIO   12

// make sure that the offsets match the QXK declaration in "qxk.h"
_Static_assert(QXK_CURR == offsetof(QXK_Attr, curr),
              "QXK_Attr.curr at unexpected offset");
_Static_assert(QXK_NEXT == offsetof(QXK_Attr, next),
              "QXK_Attr.next at unexpected offset");
_Static_assert(QXK_ACT_PRIO == offsetof(QXK_Attr, actPrio),
              "QXK_Attr.actPrio at unexpected offset");

// offsets within struct QActive; NOTE: keep in synch with "qp.h" !!!
#define QACTIVE_PRIO   12
#define QACTIVE_OSOBJ  20

// make sure that the offsets match the QActvie declaration in "qp.h"
_Static_assert(QACTIVE_OSOBJ == offsetof(QActive, osObject),
               "QActive.osObject at unexpected offset");
_Static_assert(QACTIVE_PRIO  == offsetof(QActive, prio),
               "QActive.prio at unexpected offset");

//============================================================================
// Initialize the private stack of an extended QXK thread.
//
// NOTE
// The function aligns the stack to the 8-byte boundary for compatibility
// with the AAPCS. Additionally, the function pre-fills the stack with the
// known bit pattern (0xDEADBEEF).
//
// NOTE: QXK_stackInit_() must be called before the QXK kernel is made aware
// of this thread. In that case the kernel cannot use the thread yet, so no
// critical section is needed.
void QXThread_stackInit_(QActive *me, QXThreadHandler const handler,
                    void * const stkSto, uint_fast16_t const stkSize)
{
    // round down the stack top to the 8-byte boundary
    // NOTE: ARM Cortex-M stack grows down from hi -> low memory
    uint32_t *sp =
        (uint32_t *)((((uint32_t)stkSto + stkSize) >> 3U) << 3U);
    uint32_t *sp_limit;

    // synthesize the ARM Cortex-M exception stack frame...
    *(--sp) = (1U << 24);    // xPSR  (just the THUMB bit)
    *(--sp) = (uint32_t)handler;          // PC (the thread handler)
    *(--sp) = (uint32_t)&QXK_threadExit_; // LR (exit from thread)
    *(--sp) = 0x0000000CU;   // R12
    *(--sp) = 0x00000003U;   // R3
    *(--sp) = 0x00000002U;   // R2
    *(--sp) = 0x00000001U;   // R1
    *(--sp) = (uint32_t)me;  // R0 parameter to handler (thread object)
    *(--sp) = 0x0000000BU;   // R11
    *(--sp) = 0x0000000AU;   // R10
    *(--sp) = 0x00000009U;   // R9
    *(--sp) = 0x00000008U;   // R8
    *(--sp) = 0x00000007U;   // R7
    *(--sp) = 0x00000006U;   // R6
    *(--sp) = 0x00000005U;   // R5
    *(--sp) = 0x00000004U;   // R4

#ifdef __ARM_FP         //--------- if VFP available...
    *(--sp) = 0xFFFFFFFDU;   // exception return with VFP state
    *(--sp) = 0xAAAAAAAAU;   // stack "aligner"
#endif                       // VFP available

    // save the top of the stack in the thread's attribute
    me->osObject = sp;

    // pre-fill the unused part of the stack with 0xDEADBEEF
    sp_limit = (uint32_t *)(((((uint32_t)stkSto - 1U) >> 3U) + 1U) << 3U);
    for (; sp >= sp_limit; --sp) {
        *sp = 0xDEADBEEFU;
    }
}

// prototypes ----------------------------------------------------------------
void PendSV_Handler(void);
#ifdef QXK_USE_IRQ_HANDLER          // if use IRQ...
void QXK_USE_IRQ_HANDLER(void);
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
char const QF_port_module_[] = "qxk_port";

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
//
// NOTE:
// QF_int_enable_() is NOT allowed to push anything on the stack
// (see NOTE-1 at the end of QXK_thread_ret().
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
// The interrupt priorities established in QXK_init() can be later
// changed by the application-level code.
void QXK_init(void) {

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

#ifdef QXK_USE_IRQ_NUM   //--------- QXK IRQ specified?
    // The QXK port is configured to use a given ARM Cortex-M IRQ #
    // to return to thread mode (default is to use the NMI exception)
    NVIC_IP[QXK_USE_IRQ_NUM] = 0U; // priority 0 (highest)
    NVIC_EN[QXK_USE_IRQ_NUM >> 5U] = (1U << (QXK_USE_IRQ_NUM & 0x1FU));
#endif                  //--------- QXK IRQ specified

#ifdef __ARM_FP         //--------- if VFP available...
    // make sure that the FPU is enabled by setting CP10 & CP11 Full Access
    SCB_CPACR = (SCB_CPACR | ((3UL << 20U) | (3UL << 22U)));

    // FPU automatic state preservation (ASPEN) lazy stacking (LSPEN)
    FPU_FPCCR = (FPU_FPCCR | (1U << 30U) | (1U << 31U));
#endif                  //--------- VFP available
}

//============================================================================
// The PendSV exception is used for performing asynchronous preemption in QXK.
// The use of the PendSV exception is the recommended and most efficient
// method for performing context switches in ARM Cortex-M.
//
// The PendSV exception should have the lowest interrupt priority in the system
// (0xFF, see QXK_init()). All other exceptions and interrupts should have
// higher interrupt priority.
//
// Also, *all* "kernel aware" ISRs in the QXK application must call the
// QXK_ISR_EXIT() macro, which triggers PendSV when it detects a need for
// asynchronous preemption.
//
// Due to tail-chaining and its lowest priority, the PendSV exception will be
// entered immediately after the exit from the *last* nested interrupt (or
// exception). In QXK, this is exactly the time when the QXK activator needs to
// handle the asynchronous preemption.
__attribute__ ((naked))
void PendSV_Handler(void) {
__asm volatile (

    //<<<<<<<<<<<<<<<<<<<<<<< CRITICAL SECTION BEGIN <<<<<<<<<<<<<<<<<<<<<<<<
    "  PUSH    {r0,lr}          \n" // save stack-aligner + EXC_RETURN
    "  LDR     r0,=QF_int_disable_ \n"
    "  BLX     r0               \n" // call QF_int_disable_()

#ifdef QF_MEM_ISOLATE
    "  LDR     r0,=QF_onMemSys  \n"
    "  BLX     r0               \n" // call QF_onMemSys()
#endif
    "  POP     {r0,r1}          \n" // restore the aligner + EXC_RETURN in r1
    "  MOV     lr,r1            \n" // restore EXC_RETURN into lr

    // The PendSV exception handler can be preempted by an interrupt,
    // which might pend PendSV exception again. The following write to
    // ICSR[27] un-pends any such spurious instance of PendSV.
    "  MOVS    r1,#1            \n"
    "  LSLS    r1,r1,#27        \n" // r1 := (1 << 27) (UNPENDSVSET bit)
    "  LDR     r2,=" STRINGIFY(SCB_ICSR) "\n" // Interrupt Control and State
    "  STR     r1,[r2]          \n" // ICSR[27] := 1 (unpend PendSV)

    // Check QXK_priv_.next, which contains the pointer to the next thread
    // to run, which is set in QXK_ISR_EXIT(). This pointer must not be NULL.
    "  LDR     r3,=QXK_priv_    \n"
    "  LDR     r0,[r3,#" STRINGIFY(QXK_NEXT) "]\n" // r1 := QXK_priv_.next
    "  CMP     r0,#0            \n" // is (QXK_priv_.next == 0)?
    "  BEQ     PendSV_return    \n" // branch if (QXK_priv_.next == 0)

    // Load pointers into registers...
    "  MOV     r12,r0           \n" // save QXK_priv_.next in r12
    "  LDR     r2,[r0,#" STRINGIFY(QACTIVE_OSOBJ) "]\n" // r2 := QXK_priv_.next->osObject
    "  LDR     r1,[r3,#" STRINGIFY(QXK_CURR) "]\n" // r1 := QXK_priv_.curr

    "  CMP     r1,#0            \n" // (QXK_priv_.curr != 0)?
    "  BNE     PendSV_save_ex   \n" // branch if (current thread is extended)

    "  CMP     r2,#0            \n" // (QXK_priv_.next->osObject != 0)?
    "  BNE     PendSV_save_ao   \n" // branch if (next thread is extended)

    "PendSV_activate:           \n"
#ifdef __ARM_FP         //--------- if VFP available...
    "  TST     lr,#(1 << 4)     \n" // is it return with the VFP exception frame?
    "  IT      EQ               \n" // if lr[4] is zero...
    "  VSTMDBEQ sp!,{s16-s31}   \n" // ... save VFP registers s16..s31
    "  PUSH    {r0,lr}          \n" // save the "aligner" and the EXC_RETURN value
#endif                              // VFP available
    // The QXK activator must be called in a Thread mode, while this code
    // executes in the Handler mode of the PendSV exception. The switch
    // to the Thread mode is accomplished by returning from PendSV using
    // a fabricated exception stack frame, where the return address is
    // QXK_activate_().
    //
    // NOTE: the QXK activator is called with interrupts DISABLED and also
    // returns with interrupts DISABLED.
    "  MOVS    r3,#1            \n"
    "  LSLS    r3,r3,#24        \n" // r3 := (1 << 24), set T bit (new xpsr)
    "  LDR     r2,=QXK_activate_\n" // address of QXK_activate_
    "  SUBS    r2,r2,#1         \n" // align Thumb-address at halfword (new pc)
    "  LDR     r1,=QXK_thread_ret\n" // return address after the call  (new lr)

    "  SUB     sp,sp,#(8*4)     \n" // reserve space for exception stack frame
    "  ADD     r0,sp,#(5*4)     \n" // r0 := 5 registers below the SP
    "  STM     r0!,{r1-r3}      \n" // save xpsr,pc,lr

    "  MOVS    r0,#6            \n"
    "  MVNS    r0,r0            \n" // r0 := ~6 == 0xFFFFFFF9
#if (__ARM_ARCH != 6)               // ARMv7-M or higher
    "  DSB                      \n" // ARM Erratum 838869
#endif                              // ARMv7-M or higher
    "  BX      r0               \n" // exception-return to the QXK activator

    //========================================================================
    // Saving AO-thread before crossing to eXtended-thread
    // expected register contents:
    // r0  -> QXK_priv_.next / basic-thread
    // r1  -> QXK_priv_.curr / basic-thread
    // r2  -> QXK_priv_.next->osObject (SP)
    // r3  -> &QXK_priv_
    // r12 -> QXK_priv_.next / basic-thread
    "PendSV_save_ao:            \n"
#if (__ARM_ARCH == 6)               // if ARMv6-M...
    "  SUB     sp,sp,#(8*4)     \n" // make room for 8 registers r4-r11
    "  MOV     r0,sp            \n" // r0 := temporary stack pointer
    "  STMIA   r0!,{r4-r7}      \n" // save the low registers
    "  MOV     r4,r8            \n" // move the high registers to low registers...
    "  MOV     r5,r9            \n"
    "  MOV     r6,r10           \n"
    "  MOV     r7,r11           \n"
    "  STMIA   r0!,{r4-r7}      \n" // save the high registers
    "  MOV     r0,r12           \n" // restore QXK_priv_.next in r0
#else                               // ARMv7-M or higher
    "  PUSH    {r4-r11}         \n" // save r4-r11 on top of the exception frame
#ifdef __ARM_FP         //--------- if VFP available...
    "  TST     lr,#(1 << 4)     \n" // is it return with the VFP exception frame?
    "  IT      EQ               \n" // if lr[4] is zero...
    "  VSTMDBEQ sp!,{s16-s31}   \n" // ... save VFP registers s16..s31
    "  PUSH    {r0,lr}          \n" // save the "aligner" and the EXC_RETURN value
#endif                              // VFP available
#endif                              // ARMv7-M or higher

    "  CMP     r2,#0            \n"
    "  BNE     PendSV_restore_ex\n" // branch if (QXK_priv_.next->osObject != 0)
    // otherwise continue to restoring next AO-thread...

    //------------------------------------------------------------------------
    // Restoring AO-thread after crossing from eXtended-thread
    // expected register contents:
    // r1  -> QXK_priv_.curr / basic-thread
    // r2  -> QXK_priv_.next->osObject (SP)
    // r3  -> &QXK_priv_
    // r12 -> QXK_priv_.next / basic-thread
    "PendSV_restore_ao:         \n"
    // don NOT clear QXK_priv_.curr or QXK_priv_.next,
    // as they might be needed for AO activation

#if (__ARM_ARCH == 6)               // if ARMv6-M...
    "  MOV     r0,sp            \n" // r0 := top of stack
    "  MOV     r2,r0            \n"
    "  ADDS    r2,r2,#(4*4)     \n" // point r2 to the 4 high registers r7-r11
    "  LDMIA   r2!,{r4-r7}      \n" // pop the 4 high registers into low registers
    "  MOV     r8,r4            \n" // move low registers into high registers
    "  MOV     r9,r5            \n"
    "  MOV     r10,r6           \n"
    "  MOV     r11,r7           \n"
    "  LDMIA   r0!,{r4-r7}      \n" // pop the low registers
    "  ADD     sp,sp,#(8*4)     \n" // remove 8 registers from the stack

    "  MOVS    r2,#6            \n"
    "  MVNS    r2,r2            \n" // r2 := ~6 == 0xFFFFFFF9
    "  MOV     lr,r2            \n" // make sure MSP is used
#else                               // ARMv7-M or higher
#ifdef __ARM_FP         //--------- if VFP available...
    "  POP     {r0,lr}          \n" // restore alighner and EXC_RETURN into lr
    "  DSB                      \n" // ARM Erratum 838869
    "  TST     lr,#(1 << 4)     \n" // is it return to the VFP exception frame?
    "  IT      EQ               \n" // if EXC_RETURN[4] is zero...
    "  VLDMIAEQ sp!,{s16-s31}   \n" // ... restore VFP registers s16..s31
#else
    "  BIC     lr,lr,#(1 << 2)  \n" // make sure MSP is used
#endif                              // VFP available
    "  POP     {r4-r11}         \n" // restore r4-r11 from the next thread's stack
#endif                              // ARMv7-M or higher

    "  MOV     r0,r12           \n" // r0 := QXK_priv_.next
    "  MOVS    r2,#" STRINGIFY(QACTIVE_PRIO) "\n" // r2 := offset of .prio
    "  LDRB    r0,[r0,r2]       \n" // r0 := QXK_priv_.next->prio
    "  LDR     r2,[r3,#" STRINGIFY(QXK_ACT_PRIO) "]\n" // r2 := QXK_priv_.actPrio
    "  CMP     r2,r0            \n"
    "  BCC     PendSV_activate  \n" // if (next->prio > actPrio) activate the next AO

    // otherwise no activation needed...
    "  MOVS    r0,#0            \n"
    "  STR     r0,[r3,#" STRINGIFY(QXK_CURR) "]\n" // QXK_priv_.curr := 0
    "  STR     r0,[r3,#" STRINGIFY(QXK_NEXT) "]\n" // QXK_priv_.next := 0

    "  PUSH    {r0,lr}          \n" // save the aligner + EXC_RETURN
#if defined(Q_SPY) || defined(QF_ON_CONTEXT_SW)
    // r0 is still 0 (parameter next) for QXK_contextSw_()
    "  LDR     r3,=QXK_contextSw_ \n"
    "  BLX     r3               \n" // call QXK_contextSw_()
#ifdef QF_MEM_ISOLATE
    "  LDR     r3,=QF_onMemApp  \n"
    "  BLX     r3               \n" // call QF_onMemApp()
#endif
#endif // defined(Q_SPY) || defined(QF_ON_CONTEXT_SW)
    "  B       PendSV_return1   \n" // skip over saving aligner + EXC_RETURN

    // re-enable interrupts and return from PendSV
    "PendSV_return:             \n"
    "  PUSH    {r0,lr}          \n" // save the aligner + EXC_RETURN

    "PendSV_return1:            \n"
    "  LDR     r0,=QF_int_enable_ \n"
    "  BLX     r0               \n" // call QF_int_enable_()
    //>>>>>>>>>>>>>>>>>>>>>>>> CRITICAL SECTION END >>>>>>>>>>>>>>>>>>>>>>>>>

    "  POP     {r0,pc}          \n" // return to the preempted AO-thread

    //------------------------------------------------------------------------
    // Saving extended-thread
    // expected register contents:
    // r0  -> QXK_priv_.next / basic-thread
    // r1  -> QXK_priv_.curr / basic-thread
    // r2  -> QXK_priv_.next->osObject (SP)
    // r3  -> &QXK_priv_
    // r12 -> QXK_priv_.next / basic-thread
    "PendSV_save_ex:            \n"
    "  MRS     r0,PSP           \n" // r0 := Process Stack Pointer
#if (__ARM_ARCH == 6)               // if ARMv6-M...
    "  SUBS    r0,r0,#(8*4)     \n" // make room for 8 registers r4-r11
    "  MOVS    r1,r0            \n" // r1 := temporary PSP (do not clobber r0!)
    "  STMIA   r1!,{r4-r7}      \n" // save the low registers
    "  MOV     r4,r8            \n" // move the high registers to low registers...
    "  MOV     r5,r9            \n"
    "  MOV     r6,r10           \n"
    "  MOV     r7,r11           \n"
    "  STMIA   r1!,{r4-r7}      \n" // save the high registers
    // NOTE: at this point r0 holds the top of stack

    "  LDR     r1,[r3,#" STRINGIFY(QXK_CURR) "]\n" // r1 := QXK_priv_.curr (restore value)
#else                               // ARMv7-M or higher
    "  ISB                      \n" // reset pipeline after fetching PSP
    "  STMDB   r0!,{r4-r11}     \n" // save r4-r11 on top of the exception frame
#ifdef __ARM_FP         //--------- if VFP available...
    "  TST     lr,#(1 << 4)     \n" // is it return with the VFP exception frame?
    "  IT      EQ               \n" // if lr[4] is zero...
    "  VSTMDBEQ r0!,{s16-s31}   \n" // ... save VFP registers s16..s31
    "  STMDB   r0!,{r1,lr}      \n" // save the "aligner" and the EXC_RETURN value
#endif                              // VFP available
#endif                              // ARMv7-M or higher

    // store the SP of the current extended-thread
    "  STR     r0,[r1,#" STRINGIFY(QACTIVE_OSOBJ) "]\n" // QXK_priv_.curr->osObject := r0
    "  MOV     r0,r12           \n" // QXK_priv_.next (restore value)

    "  CMP     r2,#0            \n"
    "  BEQ     PendSV_restore_ao\n" // branch if (QXK_priv_.next->osObject == 0)
    // otherwise continue to restoring next extended-thread...

    //------------------------------------------------------------------------
    // Restoring extended-thread
    // expected register contents:
    // r0  -> QXK_priv_.next / basic-thread
    // r1  -> QXK_priv_.curr / basic-thread
    // r2  -> QXK_priv_.next->osObject (SP)
    // r3  -> &QXK_priv_
    // r12 -> QXK_priv_.next / basic-thread
    "PendSV_restore_ex:         \n"
    "  STR     r0,[r3,#" STRINGIFY(QXK_CURR) "]\n" // QXK_priv_.curr := r0 (QXK_priv_.next)
    "  MOVS    r0,#0            \n"
    "  STR     r0,[r3,#" STRINGIFY(QXK_NEXT) "]\n" // QXK_priv_.next := 0

    "  PUSH    {r0-r2,lr}       \n" // save next, osObject, EXC_RETURN
#if defined(Q_SPY) || defined(QF_ON_CONTEXT_SW)
    "  MOV     r0,r12           \n" // parameter next
    "  LDR     r3,=QXK_contextSw_ \n"
    "  BLX     r3               \n" // call QXK_contextSw_()
#ifdef QF_MEM_ISOLATE
    "  LDR     r3,=QF_onMemApp  \n"
    "  BLX     r3               \n" // call QF_onMemApp()
#endif
#endif // defined(Q_SPY) || defined(QF_ON_CONTEXT_SW)


    // exit the critical section
    "  LDR     r3,=QF_int_enable_ \n"
    "  BLX     r3               \n" // call QF_int_enable_()
    "  POP     {r0-r3}          \n" // restore next, osObject, EXC_RETURN
    "  MOV     lr,r3            \n" // restore the EXC_RETURN into lr

#if (__ARM_ARCH == 6)               // if ARMv6-M...
    "  MOVS    r0,r2            \n" // r2 := top of stack
    "  ADDS    r0,r0,#(4*4)     \n" // point r0 to the 4 high registers r7-r11
    "  LDMIA   r0!,{r4-r7}      \n" // pop the 4 high registers into low registers
    "  MOV     r8,r4            \n" // move low registers into high registers
    "  MOV     r9,r5            \n"
    "  MOV     r10,r6           \n"
    "  MOV     r11,r7           \n"
    "  LDMIA   r2!,{r4-r7}      \n" // pop the low registers
    "  MOVS    r2,r0            \n" // r2 := holds the new top of stack

    "  MOVS    r1,#2            \n"
    "  MVNS    r1,r1            \n" // r1 := ~2 == 0xFFFFFFFD
    "  MOV     lr,r1            \n" // make sure PSP is used
#else                               // ARMv7-M or higher
#ifdef __ARM_FP         //--------- if VFP available...
    "  LDMIA   r2!,{r1,lr}      \n" // restore aligner and EXC_RETURN into lr
    "  TST     lr,#(1 << 4)     \n" // is it return to the VFP exception frame?
    "  IT      EQ               \n" // if lr[4] is zero...
    "  VLDMIAEQ r2!,{s16-s31}   \n" // ... restore VFP registers s16..s31
#else
    "  ORR     lr,lr,#(1 << 2)  \n" // make sure PSP is used
#endif                              // VFP available
    "  LDMIA   r2!,{r4-r11}     \n" // restore r4-r11 from the next thread's stack
#endif                              // ARMv7-M or higher

    // set the PSP to the next thread's SP
    "  MSR     PSP,r2           \n" // Process Stack Pointer := r2

#if (__ARM_ARCH != 6)               // if ARMv7-M or higher...
    "  DSB                      \n" // ARM Erratum 838869
#endif                              // ARMv7-M or higher
    "  BX      lr               \n" // return to the next extended-thread
    );
}

//============================================================================
// QXK_thread_ret is a helper function executed when the QXK activator returns.
//
// NOTE: QXK_thread_ret() does not execute in the PendSV context!
// NOTE: QXK_thread_ret() is entered with interrupts DISABLED.
__attribute__ ((naked, used))
void QXK_thread_ret(void) {
__asm volatile (

    // After the QXK activator returns, we need to resume the preempted
    // thread. However, this must be accomplished by a return-from-exception,
    // while we are still in the thread context. The switch to the exception
    // context is accomplished by triggering the NMI exception or the selected
    // IRQ (if macro #QXK_USE_IRQ_NUM is defined).

    // before triggering the NMI/IRQ, make sure that the VFP stack frame
    // will NOT be used...
#ifdef __ARM_FP         //--------- if VFP available...
    // make sure that the VFP stack frame will NOT be used
    "  MRS     r0,CONTROL       \n" // r0 := CONTROL
    "  BICS    r0,r0,#4         \n" // r0 := r0 & ~4 (FPCA bit)
    "  MSR     CONTROL,r0       \n" // CONTROL := r0 (clear CONTROL[2] FPCA bit)
    "  ISB                      \n" // ISB after MSR CONTROL (ARM AN321,Sect.4.16)
#endif                  //--------- VFP available

    // NOTE: the following function calls corrupt lr, but it is NOT
    // used to return from QXK_thread_ret(). Instead QXK_thread_ret()
    // "returns" by entering an exception (either NMI or IRQ).
#ifdef QF_MEM_ISOLATE
    "  LDR     r0,=QF_onMemApp  \n"
    "  BLX     r0               \n" // call QF_onMemApp()
#endif

#ifndef QXK_USE_IRQ_NUM //--------- IRQ NOT defined, use NMI by default
    "  LDR     r0,=" STRINGIFY(SCB_ICSR) "\n" // Interrupt Control and State
    "  MOVS    r1,#1            \n"
    "  LSLS    r1,r1,#31        \n" // r1 := (1 << 31) (NMI bit)
    "  STR     r1,[r0]          \n" // ICSR[31] := 1 (pend NMI)
    // NOTE! interrupts are still disabled when NMI is entered

#else                   //--------- use the selected IRQ
    "  LDR     r0,=" STRINGIFY(NVIC_PEND + ((QXK_USE_IRQ_NUM >> 5) << 2)) "\n"
    "  MOVS    r1,#1            \n"
    "  LSLS    r1,r1,#" STRINGIFY(QXK_USE_IRQ_NUM & 0x1F) "\n" // r1 := IRQ bit
    "  STR     r1,[r0]          \n" // pend the IRQ

    // now enable interrupts so that pended IRQ can be entered
    //
    // NOTE-1:
    // The IRQ preempts immediately after interrupts are enabled,
    // without cleanly returning from QF_int_enable_(). Therefore
    // QF_int_enable_() is NOT allowed to push anything on the stack
    // because the stack is NOT restored when the IRQ preempts.
    "  LDR     r0,=QF_int_enable_ \n"
    "  BLX     r0               \n" // call QF_int_enable_()
#endif                  //--------- use IRQ

    "  B       .                \n" // wait for preemption by NMI/IRQ
    );
}

//============================================================================
// This exception handler is used for returning back to the preempted thread.
// The exception handler simply removes its own interrupt stack frame from
// the stack (MSP) and returns to the preempted task using the interrupt
// stack frame that must be at the top of the stack.
__attribute__ ((naked))
#ifndef QXK_USE_IRQ_NUM //--------- IRQ NOT defined, use NMI by default

// NOTE: The NMI_Handler() is entered with interrupts still DISABLED!
void NMI_Handler(void) {
__asm volatile (
    // call QF_int_enable_(), NOTE: corrupts lr (EXC_RETURN)
    // but see code after "BLX r0"
    "  LDR     r0,=QF_int_enable_ \n"
    "  BLX     r0               \n" // call QF_int_enable_()
#ifdef __ARM_FP         //--------- if VFP available...
    // When VFP is available, lr (EXC_RETURN) is restored from the stack
    // before returning from exception, so the value in lr doesn't matter
#else                   //--------- VFP NOT available
    // lr (EXC_RETURN) can be synthesized because it is known (0xFFFFFFF9):
    // - return to Thread mode;
    // - exception return uses non-floating-point state from MSP;
    // - execution uses MSP after return.
    "  MOVS    r0,#6            \n"
    "  MVNS    r0,r0            \n" // r0 := ~6 == 0xFFFFFFF9
    "  MOV     lr,r0            \n" // lr := 0xFFFFFFF9 (EXC_RETURN)
#endif                  //--------- VFP NOT available
);

#else                   //--------- IRQ defined, use the selected IRQ

// NOTE: The IRQ Handler is entered with interrupts already ENABLED
void QXK_USE_IRQ_HANDLER(void) {
#endif                  //--------- IRQ defined

  // ...continue here from either NMI or IRQ
__asm volatile (
    "  ADD     sp,sp,#(8*4)     \n" // remove one 8-register exception frame

#ifdef __ARM_FP         //--------- if VFP available...
    "  POP     {r0,lr}          \n" // restore alighner and EXC_RETURN into lr
    "  DSB                      \n" // ARM Erratum 838869
    "  TST     lr,#(1 << 4)     \n" // is it return to the VFP exception frame?
    "  IT      EQ               \n" // if EXC_RETURN[4] is zero...
    "  VLDMIAEQ sp!,{s16-s31}   \n" // ... restore VFP registers s16..s31
#endif                  //--------- VFP available
    "  BX      lr               \n" // return to the preempted task
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

