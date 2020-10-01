/**
* @file
* @brief QK/C port to ARM Cortex-M, GNU-ARM toolset
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-23
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
/* This QK port is part of the interanl QP implementation */
#define QP_IMPL 1U
#include "qf_port.h"

/* prototypes --------------------------------------------------------------*/
void PendSV_Handler(void);
void NMI_Handler(void);

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

#if (__ARM_ARCH != 6) /* NOT Cortex-M0/M0+/M1 (v6-M, v6S-M)? */

    uint32_t n;

    /* set exception priorities to QF_BASEPRI...
    * SCB_SYSPRI1: Usage-fault, Bus-fault, Memory-fault
    */
    SCB_SYSPRI[1] |= (QF_BASEPRI << 16) | (QF_BASEPRI << 8) | QF_BASEPRI;

    /* SCB_SYSPRI2: SVCall */
    SCB_SYSPRI[2] |= (QF_BASEPRI << 24);

    /* SCB_SYSPRI3:  SysTick, PendSV, Debug */
    SCB_SYSPRI[3] |= (QF_BASEPRI << 24) | (QF_BASEPRI << 16) | QF_BASEPRI;

    /* set all implemented IRQ priories to QF_BASEPRI... */
    n = 8U + ((*SCnSCB_ICTR & 0x7U) << 3); /* (# NVIC_PRIO registers)/4 */
    do {
        --n;
        NVIC_IP[n] = (QF_BASEPRI << 24) | (QF_BASEPRI << 16)
                     | (QF_BASEPRI << 8) | QF_BASEPRI;
    } while (n != 0);

#endif /* NOT Cortex-M0/M0+/M1(v6-M, v6S-M) */

    /* SCB_SYSPRI3: PendSV set to the lowest priority 0xFF */
    SCB_SYSPRI[3] |= (0xFFU << 16);
}

/*****************************************************************************
* The PendSV_Handler exception handler is used for handling context switch
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
*
* NOTE:
* The inline GNU assembler does not accept mnemonics MOVS, LSRS and ADDS,
* but for Cortex-M0/M0+/M1 the mnemonics MOV, LSR and ADD always set the
* condition flags in the PSR.
*****************************************************************************/
__attribute__ ((naked, optimize("-fno-stack-protector")))
void PendSV_Handler(void) {
__asm volatile (

    /* Prepare constants in registers before entering critical section */
    "  LDR     r3,=" STRINGIFY(NVIC_ICSR) "\n" /* Interrupt Control and State */
    "  MOV     r1,#1            \n"
    "  LSL     r1,r1,#27        \n" /* r0 := (1 << 27) (UNPENDSVSET bit) */

    /*<<<<<<<<<<<<<<<<<<<<<<< CRITICAL SECTION BEGIN <<<<<<<<<<<<<<<<<<<<<<<<*/
#if (__ARM_ARCH == 6)               /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  CPSID   i                \n" /* disable interrupts (set PRIMASK) */
#else                               /* M3/M4/M7 */
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  PUSH    {r0,lr}          \n" /* ... push lr plus stack-aligner */
#endif                              /* VFP available */
    "  MOV     r0,#" STRINGIFY(QF_BASEPRI) "\n"
    "  CPSID   i                \n" /* disable interrutps with BASEPRI */
    "  MSR     BASEPRI,r0       \n" /* apply the Cortex-M7 erraturm */
    "  CPSIE   i                \n" /* 837070, see SDEN-1068427. */
#endif                              /* M3/M4/M7 */

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
    "  LSR     r3,r1,#3         \n" /* r3 := (r1 >> 3), set the T bit (new xpsr) */
    "  LDR     r2,=QK_activate_ \n" /* address of QK_activate_ */
    "  SUB     r2,r2,#1         \n" /* align Thumb-address at halfword (new pc) */
    "  LDR     r1,=QK_thread_ret \n" /* return address after the call   (new lr) */

    "  SUB     sp,sp,#8*4       \n" /* reserve space for exception stack frame */
    "  ADD     r0,sp,#5*4       \n" /* r0 := 5 registers below the SP */
    "  STM     r0!,{r1-r3}      \n" /* save xpsr,pc,lr */

    "  MOV     r0,#6            \n"
    "  MVN     r0,r0            \n" /* r0 := ~6 == 0xFFFFFFF9 */
#if (__ARM_ARCH != 6)               /* NOT Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  DSB                      \n" /* ARM Erratum 838869 */
#endif                              /* NOT (v6-M, v6S-M) */
    "  BX      r0               \n" /* exception-return to the QK activator */
    );
}

/*****************************************************************************
* QK_thread_ret is a helper function executed when the QK activator returns.
*
* NOTE: QK_thread_ret does not execute in the PendSV context!
* NOTE: QK_thread_ret executes entirely with interrupts DISABLED.
*****************************************************************************/
__attribute__ ((naked, optimize("-fno-stack-protector")))
void QK_thread_ret(void) {
__asm volatile (

    /* After the QK activator returns, we need to resume the preempted
    * thread. However, this must be accomplished by a return-from-exception,
    * while we are still in the thread context. The switch to the exception
    * contex is accomplished by triggering the NMI exception.
    * NOTE: The NMI exception is triggered with nterrupts DISABLED,
    * because QK activator disables interrutps before return.
    */

    /* before triggering the NMI exception, make sure that the
    * VFP stack frame will NOT be used...
    */
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  MRS     r0,CONTROL       \n" /* r0 := CONTROL */
    "  BICS    r0,r0,#4         \n" /* r0 := r0 & ~4 (FPCA bit) */
    "  MSR     CONTROL,r0       \n" /* CONTROL := r0 (clear CONTROL[2] FPCA bit) */
    "  ISB                      \n" /* ISB after MSR CONTROL (ARM AN321,Sect.4.16) */
#endif                              /* VFP available */

    /* trigger NMI to return to preempted task...
    * NOTE: The NMI exception is triggered with nterrupts DISABLED
    */
    "  LDR     r0,=0xE000ED04   \n" /* Interrupt Control and State Register */
    "  MOV     r1,#1            \n"
    "  LSL     r1,r1,#31        \n" /* r1 := (1 << 31) (NMI bit) */
    "  STR     r1,[r0]          \n" /* ICSR[31] := 1 (pend NMI) */
    "  B       .                \n" /* wait for preemption by NMI */
    );
}

/*****************************************************************************
* The NMI_Handler exception handler is used for returning back to the
* interrupted task. The NMI exception simply removes its own interrupt
* stack frame from the stack and returns to the preempted task using the
* interrupt stack frame that must be at the top of the stack.
*
* NOTE: The NMI exception is entered with interrupts DISABLED, so it needs
* to re-enable interrupts before it returns to the preempted task.
*****************************************************************************/
__attribute__ ((naked, optimize("-fno-stack-protector")))
void NMI_Handler(void) {
__asm volatile (

    "  ADD     sp,sp,#(8*4)     \n" /* remove one 8-register exception frame */

#if (__ARM_ARCH == 6)               /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  CPSIE   i                \n" /* enable interrupts (clear PRIMASK) */
    "  BX      lr               \n" /* return to the preempted task */
#else                               /* M3/M4/M7 */
    "  MOV     r0,#0            \n"
    "  MSR     BASEPRI,r0       \n" /* enable interrupts (clear BASEPRI) */
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  POP     {r0,lr}          \n" /* pop stack aligner and EXC_RETURN to LR */
    "  DSB                      \n" /* ARM Erratum 838869 */
#endif                              /* if VFP available */
    "  BX      lr               \n" /* return to the preempted task */
#endif                              /* M3/M4/M7 */
    );
}

/****************************************************************************/
#if (__ARM_ARCH == 6) /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */

/* hand-optimized quick LOG2 in assembly (M0/M0+ have no CLZ instruction)
*
* NOTE:
* The inline GNU assembler does not accept mnemonics MOVS, LSRS and ADDS,
* but for Cortex-M0/M0+/M1 the mnemonics MOV, LSR and ADD always set the
* condition flags in the PSR.
*/
__attribute__ ((naked, optimize("-fno-stack-protector")))
uint_fast8_t QF_qlog2(uint32_t x) {
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

#endif /* Cortex-M0/M0+/M1(v6-M, v6S-M)? */

