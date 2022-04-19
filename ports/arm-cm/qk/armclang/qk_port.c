/*============================================================================
* QK/C port to ARM Cortex-M, ARM-CLANG
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-04-09
* @version Last updated for: @ref qpc_7_0_0
*
* @file
* @brief QK/C port to ARM Cortex-M, ARM-CLANG toolset
*/
/* This QK port is part of the interanl QP implementation */
#define QP_IMPL 1U
#include "qf_port.h"

/* prototypes --------------------------------------------------------------*/
void PendSV_Handler(void);
#ifndef QK_ARM_CM_USE_NMI
    void SVC_Handler(void);
#else
    void NMI_Handler(void);
#endif

#define SCnSCB_ICTR  ((uint32_t volatile *)0xE000E004)
#define SCB_SYSPRI   ((uint32_t volatile *)0xE000ED14)
#define NVIC_IP      ((uint32_t volatile *)0xE000E400)
#define NVIC_ICSR    0xE000ED04

/* helper macros to "stringify" values */
#define VAL(x) #x
#define STRINGIFY(x) VAL(x)

/*
* Initialize the exception priorities and IRQ priorities to safe values.
*
* Description:
* On Cortex-M3/M4/M7, this QK port disables interrupts by means of the
* BASEPRI register. However, this method cannot disable interrupt
* priority zero, which is the default for all interrupts out of reset.
* The following code changes the SysTick priority and all IRQ priorities
* to the safe value QF_BASEPRI, wich the QF critical section can disable.
* This avoids breaching of the QF critical sections in case the
* application programmer forgets to explicitly set priorities of all
* "kernel aware" interrupts.
*
* The interrupt priorities established in QK_init() can be later
* changed by the application-level code.
*/
void QK_init(void) {

#if (__ARM_ARCH != 6) /* if ARMv7-M and higher... */

    /* set exception priorities to QF_BASEPRI...
    * SCB_SYSPRI1: Usage-fault, Bus-fault, Memory-fault
    */
    SCB_SYSPRI[1] |= (QF_BASEPRI << 16U) | (QF_BASEPRI << 8U) | QF_BASEPRI;

    /* SCB_SYSPRI2: SVCall */
    SCB_SYSPRI[2] |= (QF_BASEPRI << 24U);

    /* SCB_SYSPRI3:  SysTick, PendSV, Debug */
    SCB_SYSPRI[3] |= (QF_BASEPRI << 24U) | (QF_BASEPRI << 16U) | QF_BASEPRI;

    /* set all implemented IRQ priories to QF_BASEPRI... */
    uint32_t n = 8U + ((*SCnSCB_ICTR & 0x7U) << 3U); /*(# NVIC_PRIO regs)/4 */
    do {
        --n;
        NVIC_IP[n] = (QF_BASEPRI << 24U) | (QF_BASEPRI << 16U)
                     | (QF_BASEPRI << 8U) | QF_BASEPRI;
    } while (n != 0U);

#endif /* ARMv7-M and higher */

    /* SCB_SYSPRI3: PendSV set to priority 0xFF (lowest) */
    SCB_SYSPRI[3] |= (0xFFU << 16U);

#ifndef QK_ARM_CM_USE_NMI
    /* SCB_SYSPRI2: SVCall */
    SCB_SYSPRI[2] &= ~(0xFFU << 24U); /* prioiry 0 (highest) */
#endif
}

/*==========================================================================*/
/* The PendSV_Handler exception handler is used for handling context switch
* and asynchronous preemption in QK. The use of the PendSV exception is
* the recommended and most efficient method for performing context switches
* with ARM Cortex-M.
*
* The PendSV exception should have the lowest priority in the whole system
* (0xFF, see QK_init). All other exceptions and interrupts should have higher
* priority. For example, for NVIC with 2 priority bits all interrupts and
* exceptions must have numerical value of priority lower than 0xC0. In this
* case the interrupt priority levels available to your applications are (in
* the order from the lowest urgency to the highest urgency): 0x80, 0x40, 0x00.
*
* Also, *all* "kernel aware" ISRs in the QK application must call the
* QK_ISR_EXIT() macro, which triggers PendSV when it detects a need for
* a context switch or asynchronous preemption.
*
* Due to tail-chaining and its lowest priority, the PendSV exception will be
* entered immediately after the exit from the *last* nested interrupt (or
* exception). In QK, this is exactly the time when the QK activator needs to
* handle the asynchronous preemption.
*/
__attribute__ ((naked))
void PendSV_Handler(void) {
__asm volatile (

    /* Prepare constants in registers before entering critical section */
    "  LDR     r3,=" STRINGIFY(NVIC_ICSR) "\n" /* Interrupt Control and State */
    "  MOVS    r1,#1            \n"
    "  LSLS    r1,r1,#27        \n" /* r0 := (1 << 27) (UNPENDSVSET bit) */

    /*<<<<<<<<<<<<<<<<<<<<<<< CRITICAL SECTION BEGIN <<<<<<<<<<<<<<<<<<<<<<<<*/
#if (__ARM_ARCH == 6)               /* if ARMv6-M... */
    "  CPSID   i                \n" /* disable interrupts (set PRIMASK) */
#else                               /* ARMv7-M and higher */
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  PUSH    {r0,lr}          \n" /* ... push lr plus stack-aligner */
#endif                              /* VFP available */
    "  MOVS    r0,#" STRINGIFY(QF_BASEPRI) "\n"
    "  CPSID   i                \n" /* disable interrutps with BASEPRI */
    "  MSR     BASEPRI,r0       \n" /* apply the Cortex-M7 erraturm */
    "  CPSIE   i                \n" /* 837070, see SDEN-1068427. */
#endif                              /* ARMv7-M and higher */

    /* The PendSV exception handler can be preempted by an interrupt,
    * which might pend PendSV exception again. The following write to
    * ICSR[27] un-pends any such spurious instance of PendSV.
    */
    "  STR     r1,[r3]          \n" /* ICSR[27] := 1 (unpend PendSV) */

    /* The QK activator must be called in a Thread mode, while this code
    * executes in the Handler mode of the PendSV exception. The switch
    * to the Thread mode is accomplished by returning from PendSV using
    * a fabricated exception stack frame, where the return address is
    * QK_activate_().
    *
    * NOTE: the QK activator is called with interrupts DISABLED and also
    * returns with interrupts DISABLED.
    */
    "  LSRS    r3,r1,#3         \n" /* r3 := (r1 >> 3), set the T bit (new xpsr) */
    "  LDR     r2,=QK_activate_ \n" /* address of QK_activate_ */
    "  SUBS    r2,r2,#1         \n" /* align Thumb-address at halfword (new pc) */
    "  LDR     r1,=QK_thread_ret \n" /* return address after the call   (new lr) */

    "  SUB     sp,sp,#8*4       \n" /* reserve space for exception stack frame */
    "  ADD     r0,sp,#5*4       \n" /* r0 := 5 registers below the SP */
    "  STM     r0!,{r1-r3}      \n" /* save xpsr,pc,lr */

    "  MOVS    r0,#6            \n"
    "  MVNS    r0,r0            \n" /* r0 := ~6 == 0xFFFFFFF9 */
#if (__ARM_ARCH != 6)               /* if ARMv7-M and higher... */
    "  DSB                      \n" /* ARM Erratum 838869 */
#endif                              /* ARMv7-M and higher */
    "  BX      r0               \n" /* exception-return to the QK activator */
    );
}

/*==========================================================================*/
/* QK_thread_ret is a helper function executed when the QXK activator returns.
*
* NOTE: QK_thread_ret does not execute in the PendSV context!
* NOTE: QK_thread_ret is entered with interrupts DISABLED.
*/
__attribute__ ((naked))
void QK_thread_ret(void) {
__asm volatile (

    /* After the QK activator returns, we need to resume the preempted
    * thread. However, this must be accomplished by a return-from-exception,
    * while we are still in the thread context. The switch to the exception
    * contex is accomplished by triggering the SVC or NMI exception.
    */

#if (__ARM_ARCH == 6)               /* if ARMv6-M... */
    "  CPSIE   i                \n" /* enable interrupts (clear PRIMASK) */
#else                               /* ARMv7-M and higher */
    "  MOVS    r0,#0            \n"
    "  MSR     BASEPRI,r0       \n" /* enable interrupts (clear BASEPRI) */
#if (__ARM_FP != 0)                 /* if VFP available... */
    /* make sure that the VFP stack frame will NOT be used */
    "  MRS     r0,CONTROL       \n" /* r0 := CONTROL */
    "  BICS    r0,r0,#4         \n" /* r0 := r0 & ~4 (FPCA bit) */
    "  MSR     CONTROL,r0       \n" /* CONTROL := r0 (clear CONTROL[2] FPCA bit) */
    "  ISB                      \n" /* ISB after MSR CONTROL (ARM AN321,Sect.4.16) */
#endif                              /* VFP available */
#endif                              /* ARMv7-M and higher */

#ifndef QK_ARM_CM_USE_NMI            /* if use SVC... */
    "  SVC     #0               \n" /* trigger SVC */
#else                               /* use NMI */
    "  LDR     r0,=" STRINGIFY(NVIC_ICSR) "\n" /* Interrupt Control and State */
    "  MOVS    r1,#1            \n"
    "  LSLS    r1,r1,#31        \n" /* r1 := (1 << 31) (NMI bit) */
    "  STR     r1,[r0]          \n" /* ICSR[31] := 1 (pend NMI) */
    "  B       .                \n" /* wait for preemption by NMI */
#endif                              /* use NMI */
    );
}

/*==========================================================================*/
/* This exception handler is used for returning back to the interrupted task.
* The exception handler simply removes its own interrupt stack frame from
* the stack (MSP) and returns to the preempted task using the interrupt
* stack frame that must be at the top of the stack.
*/
__attribute__ ((naked))
#ifndef QK_ARM_CM_USE_NMI            /* if use SVC... */
void SVC_Handler(void)
#else                               /* use NMI */
void NMI_Handler(void)
#endif                              /* use NMI */
{
__asm volatile (
    "  ADD     sp,sp,#(8*4)     \n" /* remove one 8-register exception frame */

#if (__ARM_ARCH != 6)               /* if ARMv7-M and higher... */
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  POP     {r0,lr}          \n" /* pop stack aligner and EXC_RETURN to LR */
    "  DSB                      \n" /* ARM Erratum 838869 */
#endif                              /* VFP available */
#endif                              /* ARMv7-M and higher */
    "  BX      lr               \n" /* return to the preempted task */
    );
}

/*==========================================================================*/
#if (__ARM_ARCH == 6) /* if ARMv6-M... */

/* hand-optimized quick LOG2 in assembly (No CLZ instruction in ARMv6-M) */
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
    "  ADDS    r0,r1, r0        \n"
    "  BX      lr               \n"
    "  .align                   \n"
    "QF_qlog2_LUT:              \n"
    "  .byte 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4"
    );
}

#endif /* ARMv6-M */

