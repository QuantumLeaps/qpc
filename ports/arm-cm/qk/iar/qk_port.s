;*****************************************************************************
; Product: QK port to ARM Cortex-M (M0,M0+,M1,M3,M4,M7), IAR ARM assembler
; Last Updated for Version: 5.6.4
; Date of the Last Update:  2016-04-21
;
;                    Q u a n t u m     L e a P s
;                    ---------------------------
;                    innovating embedded systems
;
; Copyright (C) Quantum Leaps, LLC. All rights reserved.
;
; This program is open source software: you can redistribute it and/or
; modify it under the terms of the GNU General Public License as published
; by the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; Alternatively, this program may be distributed and modified under the
; terms of Quantum Leaps commercial licenses, which expressly supersede
; the GNU General Public License and are specifically designed for
; licensees interested in retaining the proprietary status of their code.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program. If not, see <http://www.gnu.org/licenses/>.
;
; Contact information:
; http://www.state-machine.com
; mailto:info@state-machine.com
;*****************************************************************************

    PUBLIC  QK_init
    PUBLIC  PendSV_Handler    ; CMSIS-compliant PendSV exception name
    PUBLIC  NMI_Handler       ; CMSIS-compliant NMI exception name

    EXTERN  QK_schedPrio_     ; external reference
    EXTERN  QK_sched_         ; external reference

    ; NOTE: keep in synch with QF_BASEPRI value defined in "qf_port.h" !!!
QF_BASEPRI EQU (0xFF >> 2)


    RSEG CODE:CODE:NOROOT(2)
;*****************************************************************************
; The QK_init function sets the priorities of PendSV to 0xFF (lowest).
; The priority is set within a critical section.
;*****************************************************************************
QK_init:
    MRS     r0,PRIMASK        ; store the state of the PRIMASK in r0
    CPSID   i                 ; disable interrupts (set PRIMASK)

    LDR     r1,=0xE000ED18    ; System Handler Priority Register
    LDR     r2,[r1,#8]        ; load the System 12-15 Priority Register
    MOVS    r3,#0xFF
    LSLS    r3,r3,#16
    ORRS    r2,r3             ; set PRI_14 (PendSV) to 0xFF
    STR     r2,[r1,#8]        ; write the System 12-15 Priority Register

    MSR     PRIMASK,r0        ; restore the original PRIMASK
    BX      lr                ; return to the caller


;*****************************************************************************
; The PendSV_Handler exception handler is used for handling asynchronous
; preemption in QK. The use of the PendSV exception is the recommended
; and most efficient method for performing context switches with ARM Cortex-M.
;
; The PendSV exception should have the lowest priority in the whole system
; (0xFF, see QK_init). All other exceptions and interrupts should have higher
; priority. For example, for NVIC with 2 priority bits all interrupts and
; exceptions must have numerical value of priority lower than 0xC0. In this
; case the interrupt priority levels available to your applications are (in
; the order from the lowest urgency to the highest urgency): 0x80, 0x40, 0x00.
;
; Also, *all* ISRs in the QK application must trigger the PendSV exception
; by calling the QK_ISR_EXIT() macro.
;
; Due to tail-chaining and its lowest priority, the PendSV exception will be
; entered immediately after the exit from the *last* nested interrupt (or
; exception). In QK, this is exactly the time when the QK scheduler needs to
; check for the asynchronous preemption.
;*****************************************************************************
PendSV_Handler:
#if (__CORE__ == __ARM6M__)   ; Cortex-M0/M0+/M1 ?
    CPSID   i                 ; disable interrupts (set PRIMASK)
#else ; M3/M4/M7
    MOVS    r0,#QF_BASEPRI
    MSR     BASEPRI,r0        ; selectively disable interrupts
#endif ; M3/M4/M7
    ISB                       ; reset the instruction pipeline

#ifdef __ARMVFP__             ; if VFP available...
    PUSH    {r0,lr}           ; push lr (EXC_RETURN) plus stack "aligner"
#endif                        ; VFP available

    BL      QK_schedPrio_     ; call QK_schedPrio_()
    CMP     r0,#0             ; is the returned next prio 0?
    BNE.N   PendSV_sched      ; if next prio != 0, branch to scheduler

PendSV_ret:
#if (__CORE__ == __ARM6M__)   ; Cortex-M0/M0+/M1 ?
    CPSIE   i                 ; enable interrupts (clear PRIMASK)
    MOVS    r0,#6
    MVNS    r0,r0             ; r0 := ~6 == 0xFFFFFFF9
    BX      r0                ; exception-return to the task
#else                         ; M3/M4/M7
    ; NOTE: r0 == 0 at this point
    MSR     BASEPRI,r0        ; enable interrupts (clear BASEPRI)
#ifdef __ARMVFP__             ; if VFP available...
    POP     {r0,pc}           ; pop stack "aligner" and EXC_RETURN to PC
#else                         ; no VFP
    MOVS    r0,#6
    MVNS    r0,r0             ; r0 := ~6 == 0xFFFFFFF9
    BX      r0                ; exception-return to the task
#endif                        ; no VFP
#endif                        ; M3/M4/M7

PendSV_sched:                 ; call the QK scheduler...
    ; NOTE: The QK scheduler must be called in a task context, while
    ; we are still in the PendSV exception context. The switch to the
    ; task context is accomplished by returning from PendSV using a
    ; fabricated exception stack frame, where the return address is
    ; the QK scheduler.
    ; NOTE: the QK scheduler is called with interrupts DISABLED.
    MOVS    r3,#1
    LSLS    r3,r3,#24         ; r3:=(1 << 24), set the T bit  (new xpsr)
    LDR     r2,=QK_sched_     ; address of the QK scheduler   (new pc)
    LDR     r1,=PendSV_sched_ret ; return address after the call (new lr)

    SUB     sp,sp,#8*4        ; reserve space for exception stack frame
    STR     r0,[sp]           ; save the prio argument (new r0)
    ADD     r0,sp,#5*4        ; r0 := 5 registers below the top of stack
    STM     r0!,{r1-r3}       ; save xpsr,pc,lr

    MOVS    r0,#6
    MVNS    r0,r0             ; r0 := ~6 == 0xFFFFFFF9
    BX      r0                ; exception-return to the QK scheduler

PendSV_sched_ret:
    ; NOTE: After the QK scheduler returns, we need to resume the preempted
    ; task. However, this must be accomplished by a return-from-exception,
    ; while we are still in the task context. The switch to the exception
    ; contex is accomplished by triggering the NMI exception.
    ; NOTE: The NMI exception is triggered with nterrupts DISABLED,
    ; because QK scheduler disables interrutps before return.

    ; before triggering the NMI exception, make sure that the
    ; VFP stack frame will NOT be used...
#ifdef __ARMVFP__             ; if VFP available...
    MRS     r0,CONTROL        ; r0 := CONTROL
    BICS    r0,r0,#4          ; r0 := r0 & ~4 (FPCA bit)
    MSR     CONTROL,r0        ; CONTROL := r0 (clear CONTROL[2] FPCA bit)
#endif                        ; VFP available

    ; trigger NMI to return to preempted task...
    LDR     r0,=0xE000ED04    ; Interrupt Control and State Register
    MOVS    r1,#1
    LSLS    r1,r1,#31         ; r0 := (1 << 31) (NMI bit)
    STR     r1,[r0]           ; ICSR[31] := 1 (pend NMI)
    B       .                 ; wait for preemption by NMI


;*****************************************************************************
; The NMI_Handler exception handler is used for returning back to the
; interrupted task. The NMI exception simply removes its own interrupt
; stack frame from the stack and returns to the preempted task using the
; interrupt stack frame that must be at the top of the stack.
;
; NOTE: The NMI exception is entered with interrupts DISABLED, so it needs
; to re-enable interrupts before it returns to the preempted task.
;*****************************************************************************
NMI_Handler:
    ADD     sp,sp,#(8*4)      ; remove one 8-register exception frame

#if (__CORE__ == __ARM6M__)   ; Cortex-M0/M0+/M1 ?
    CPSIE   i                 ; enable interrupts (clear PRIMASK)
    BX      lr                ; return to the preempted task
#else                         ; M3/M4/M7
    MOVS    r0,#0
    MSR     BASEPRI,r0        ; enable interrupts (clear BASEPRI)
#ifdef __ARMVFP__             ; if VFP available...
    POP     {r0,pc}           ; pop stack "aligner" and EXC_RETURN to PC
#else                         ; no VFP
    BX      lr                ; return to the preempted task
#endif                        ; no VFP
#endif                        ; M3/M4/M7

    ALIGNROM 2,0xFF           ; make sure the END is properly aligned

    END
