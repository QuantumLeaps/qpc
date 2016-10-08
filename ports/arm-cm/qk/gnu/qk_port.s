/*****************************************************************************
* Product: QK port to ARM Cortex-M (M0,M0+,M3,M4,M7), GNU-ARM assembler
* Last Updated for Version: 5.7.3
* Date of the Last Update:  2016-10-07
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* http://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/

    .syntax unified
    .thumb

    /* NOTE: keep in synch with QF_BASEPRI value defined in "qf_port.h" !!! */
    .equ QF_BASEPRI,(0xFF >> 2)


/*****************************************************************************
* The QK_init() function sets the priority of PendSV to 0xFF (lowest).
* This operation is performed in a critical section.
*****************************************************************************/
    .section .text.QK_init
    .global QK_init
    .type   QK_init, %function

QK_init:
    MRS     r0,PRIMASK        /* store the state of the PRIMASK in r0 */
    CPSID   i                 /* disable interrupts (set PRIMASK) */

    LDR     r1,=0xE000ED18    /* System Handler Priority Register */
    LDR     r2,[r1,#8]        /* load the System 12-15 Priority Register */
    MOVS    r3,#0xFF
    LSLS    r3,r3,#16
    ORRS    r2,r3             /* set PRI_14 (PendSV) to 0xFF */
    STR     r2,[r1,#8]        /* write the System 12-15 Priority Register */

    MSR     PRIMASK,r0        /* restore the original PRIMASK */
    BX      lr                /* return to the caller */
  .size   QK_init, . - QK_init


/*****************************************************************************
* The PendSV_Handler exception handler is used for handling the asynchronous
* preemption in QK. The use of the PendSV exception is the recommended
* and most efficient method for performing context switches with ARM Cortex-M.
*
* The PendSV exception should have the lowest priority in the whole system
* (0xFF, see QK_init). All other exceptions and interrupts should have higher
* priority. For example, for NVIC with 2 priority bits all interrupts and
* exceptions must have numerical value of priority lower than 0xC0. In this
* case the interrupt priority levels available to your applications are (in
* the order from the lowest urgency to the highest urgency): 0x80, 0x40, 0x00.
*
* Also, *all* "kernel aware" ISRs in the QK application must trigger
* the PendSV exception by calling the QK_ISR_EXIT() macro.
*
* Due to tail-chaining and its lowest priority, the PendSV exception will be
* entered immediately after the exit from the *last* nested interrupt (or
* exception). In QK, this is exactly the time when the QK activator needs to
* handle the asynchronous preemption.
*****************************************************************************/
    .section .text.PendSV_Handler
    .global PendSV_Handler    /* CMSIS-compliant exception name */
    .type   PendSV_Handler, %function

PendSV_Handler:
    /* Prepare some constants in registers before entering critical section */
    LDR     r3,=0xE000ED04    /* Interrupt Control and State Register */
    MOVS    r1,#1
    LSLS    r1,r1,#27         /* r0 := (1 << 27) (UNPENDSVSET bit) */


    /* <<<<<<<<<<<<<<<<<<<<<<< CRITICAL SECTION BEGIN <<<<<<<<<<<<<<<<<<<<< */
  .if  __ARM_ARCH == 6        /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    CPSID   i                 /* disable interrupts (set PRIMASK) */
  .else                       /* M3/M4/M7 */
  .ifdef  __FPU_PRESENT       /* if VFP available... */
    PUSH    {r0,lr}           /* ... push EXC_RETURN plus stack-aligner */
  .endif                      /* VFP */
    MOVS    r0,#QF_BASEPRI
    MSR     BASEPRI,r0        /* selectively disable interrupts */
  .endif                      /* M3/M4/M7 */

    /* The PendSV exception handler can be preempted by an interrupt,
    * which might pend PendSV exception again. The following write to
    * ICSR[27] un-pends any such spurious instance of PendSV.
    */
    STR     r1,[r3]           /* ICSR[27] := 1 (unpend PendSV) */

    /* The QK activator must be called in a Thread mode, while this code
    * executes in the Handler mode of the PendSV exception. The switch
    * to the Thread mode is accomplished by returning from PendSV using
    * a fabricated exception stack frame, where the return address is
    * QK_activate_().
    *
    * NOTE: the QK activator is called with interrupts DISABLED and also
    * returns with interrupts DISABLED.
    */
    LSRS    r3,r1,#3          /* r3 := (r1 >> 3), set the T bit (new xpsr) */
    LDR     r2,=QK_activate_  /* address of QK_activate_ */
    SUBS    r2,r2,#1          /* align Thumb-address at halfword (new pc) */
    LDR     r1,=Thread_ret    /* return address after the call   (new lr) */

    SUB     sp,sp,#8*4        /* reserve space for exception stack frame */
    STR     r0,[sp]           /* save the prio argument (new r0) */
    ADD     r0,sp,#5*4        /* r0 := 5 registers below the top of stack */
    STM     r0!,{r1-r3}       /* save xpsr,pc,lr */

    MOVS    r0,#6
    MVNS    r0,r0             /* r0 := ~6 == 0xFFFFFFF9 */
    BX      r0                /* exception-return to the QK activator */
  .size   PendSV_Handler, . - PendSV_Handler


/*****************************************************************************
* Thread_ret is a helper function executed when the QXK activator returns.
*
* NOTE: Thread_ret does not execute in the PendSV context!
* NOTE: Thread_ret executes entirely with interrupts DISABLED.
****************************************************************************/
    .section .text.Thread_ret
    .type   Thread_ret, %function

Thread_ret:
    /* After the QK activator returns, we need to resume the preempted
    * task. However, this must be accomplished by a return-from-exception,
    * while we are still in the task context. The switch to the exception
    * contex is accomplished by triggering the NMI exception.
    * NOTE: The NMI exception is triggered with nterrupts DISABLED,
    * because QK activator disables interrutps before return.
    */

    /* before triggering the PendSV exception, make sure that the
    * VFP stack frame will NOT be used...
    */
  .ifdef  __FPU_PRESENT       /* if VFP available... */
    MRS     r0,CONTROL        /* r0 := CONTROL */
    BICS    r0,r0,#4          /* r0 := r0 & ~4 (FPCA bit) */
    MSR     CONTROL,r0        /* CONTROL := r0 (clear CONTROL[2] FPCA bit) */
  .endif                      /* VFP available */


    /* trigger NMI to return to preempted task... */
    LDR     r0,=0xE000ED04    /* Interrupt Control and State Register */
    MOVS    r1,#1
    LSLS    r1,r1,#31         /* r0 := (1 << 31) (NMI bit) */
    STR     r1,[r0]           /* ICSR[31] := 1 (pend NMI) */
    B       .                 /* wait for preemption by NMI */
  .size   Thread_ret, . - Thread_ret


/*****************************************************************************
* The NMI_Handler exception handler is used for returning back to the
* interrupted task. The NMI exception simply removes its own interrupt
* stack frame from the stack and returns to the preempted task using the
* interrupt stack frame that must be at the top of the stack.
*
* NOTE: The NMI exception is entered with interrupts DISABLED, so it needs
* to re-enable interrupts before it returns to the preempted task.
*****************************************************************************/
    .section .text.NMI_Handler
    .global NMI_Handler
    .type   NMI_Handler, %function

NMI_Handler:
    ADD     sp,sp,#(8*4)      /* remove one 8-register exception frame */

  .if  __ARM_ARCH == 6        /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    CPSIE   i                 /* enable interrupts (clear PRIMASK) */
    BX      lr                /* return to the preempted task */
  .else                       /* M3/M4/M7 */
    MOVS    r0,#0
    MSR     BASEPRI,r0        /* enable interrupts (clear BASEPRI) */
  .ifdef  __FPU_PRESENT       /* if VFP available... */
    POP     {r0,pc}           /* pop stack "aligner" and EXC_RETURN to PC */
  .else
    BX      lr                /* return to the preempted task */
  .endif                      /* VFP available */
  .endif                      /* M3/M4/M7 */
  .size   NMI_Handler, . - NMI_Handler

  .end
