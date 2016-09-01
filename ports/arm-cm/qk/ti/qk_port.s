;*****************************************************************************
; Product: QK port to ARM Cortex-M (M0,M0+,M3,M4,M7), TI-ARM assembler
; Last Updated for Version: 5.7.0
; Date of the Last Update:  2016-08-08
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

    .global QK_init
    .global PendSV_Handler    ; CMSIS-compliant PendSV exception name

    .global QF_set_BASEPRI    ; set BASEPRI register
    .global QK_get_IPSR       ; get the IPSR
    .global assert_failed     ; low-level assert handler

    .ref    QK_sched_         ; external reference
    .ref    QK_attr_          ; external reference
    .ref    Q_onAssert        ; external reference

    ; NOTE: keep in synch with QF_BASEPRI value defined in "qf_port.h" !!!
QF_BASEPRI: .equ  (0xFF >> 2)

    ; NOTE: keep in synch with the QK_Attr struct in "qk.h" !!!
QK_CURR:    .equ  0
QK_NEXT:    .equ  4
QK_AUX:     .equ  8

    .text
    .thumb

;*****************************************************************************
; The QK_init() function sets the priority of PendSV to 0xFF (lowest).
; Also, it initializes QK_attr_.aux to zero. Both these operations
; are performed in a critical section.
;*****************************************************************************
QK_init:    .asmfunc
    MRS     r0,PRIMASK        ; store the state of the PRIMASK in r0
    CPSID   i                 ; disable interrupts (set PRIMASK)

    LDR     r1,SHPR_addr      ; System Handler Priority Register
    LDR     r2,[r1,#8]        ; load the System 12-15 Priority Register
    MOVS    r3,#0xFF
    LSLS    r3,r3,#16
    ORRS    r2,r3             ; set PRI_14 (PendSV) to 0xFF
    STR     r2,[r1,#8]        ; write the System 12-15 Priority Register

    ; The QK_attr_.aux attribute is used to determine the purpose of the
    ; PendSV exception. When (QK_attr_.aux == 0), PendSV is used for
    ; scheduling the next thread. Otherwise (QK_attr_.aux != 0), it is
    ; used for returning to the preempted thread. Here QK_attr_.aux is
    ; initialized to zero.
    LDR     r3,QK_attr_addr
    MOVS    r2,#0
    STR     r2,[r3,#QK_AUX]   ; QK_attr_.aux := 0

    MSR     PRIMASK,r0        ; restore the original PRIMASK
    BX      lr                ; return to the caller
  .endasmfunc

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
; Also, *all* "kernel aware" ISRs in the QK application must trigger
; the PendSV exception by calling the QK_ISR_EXIT() macro.
;
; Due to tail-chaining and its lowest priority, the PendSV exception will be
; entered immediately after the exit from the *last* nested interrupt (or
; exception). In QK, this is exactly the time when the QK scheduler needs to
; check for the asynchronous preemption.
;*****************************************************************************
PendSV_Handler: .asmfunc

    ; Check QK_attr_.aux to determine the purpose of this PendSV exception.
    ; When (QK_attr_.aux == 0), PendSV is used for scheduling the next thread.
    ; Otherwise (QK_attr_.aux != 0), it is used for returning to the
    ; preempted thread.
    ;
    ; NOTE: no critical section is necessary, because the only other place
    ; QK_attr_.aux is accessed is inside a critical section and no other
    ; instance of PendSV can preempt itself.
    ;
    LDR     r3,QK_attr_addr
    LDR     r0,[r3,#QK_AUX]   ; r0 := QK_attr_.aux
    CMP     r0,#0             ; if (QK_attr_.aux == 0) ...
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    BEQ     PendSV_save_lr    ; save lr before checking QK_attr_.next
  .else
    BEQ     PendSV_check_next ; go straight to checking QK_attr_.next
  .endif                      ; VFP available

    ; Here you know that (QK_attr_.aux != 0), meaning that this PendSV
    ; instance has been triggered for returning to the preempted thread.
    ; The no-FPU exception stack frame of this PendSV instance is removed
    ; from the stack
    ADD     sp,sp,#(8*4)      ; remove one 8-register exception frame

    ; clear QK_attr_.aux for the next time
    MOVS    r1,#0
    STR     r1,[r3,#QK_AUX]   ; QK_attr_.aux = 0

  .if __TI_VFP_SUPPORT__      ; if VFP available...
    B       PendSV_check_next

PendSV_save_lr:
    ; When FPU is available, the lr of this exception needs to be saved,
    ; because it contains the information about the type of the exception
    ; stack frame (FPU/no-FPU registers).
    PUSH    {r0,lr}           ; push lr (EXC_RETURN) plus stack "aligner"
  .endif                      ; VFP available

PendSV_check_next:
    ; Prepare some constants (an address and a bitmask) before entering
    ; a critical section...
    LDR     r2,ICSR_addr      ; Interrupt Control and State Register
    MOVS    r1,#1
    LSLS    r1,r1,#27         ; r0 := (1 << 27) (UNPENDSVSET bit)

    ; <<<<<<<<<<<<<<<<<<<<<<< CRITICAL SECTION BEGIN <<<<<<<<<<<<<<<<<<<<<<<<<
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#QF_BASEPRI
    MSR     BASEPRI,r0        ; selectively disable interrupts
  .else                       ; Cortex-M0/M0+/M1 ?
    CPSID   i                 ; disable interrupts (set PRIMASK)
  .endif                      ; M0/M0+/M1

    ; The PendSV exception handler can be preempted by an interrupt,
    ; which might pend PendSV exception again. The following write to
    ; ICSR[27] un-pends any such spurious instance of PendSV.
    STR     r1,[r2]           ; ICSR[27] := 1 (unpend PendSV)

    ; Check QK_attr_.next, which contains the priority of the next thread
    ; to run, which is set in QK_ISR_EXIT(). If this priority is non-zero
    ; (QK_attr_.next != 0), this next thread needs to be scheduled.
    ; Otherwise, if (QK_attr_.next == 0), this PendSV exception needs to
    ; simply return without activating the QK scheduler.
    ;
    LDR     r0,[r3,#QK_NEXT]  ; r0 := QK_attr_.next
    CMP     r0,#0             ; if (QK_attr_.next == 0) ...
    BEQ     PendSV_ret

    ; set QK_attr_.next to 0 for the next time
    MOVS    r1,#0
    STR     r1,[r3,#QK_NEXT]  ; QK_attr_.next := 0

PendSV_call_sched:
    ; The QK scheduler must be called in a thread context, while this code
    ; executes in the handler contex of the PendSV exception. The switch
    ; to the thread context is accomplished by returning from PendSV using
    ; a fabricated exception stack frame, where the return address is the
    ; QK scheduler QK_sched_().
    ;
    ; NOTE: the QK scheduler is called with interrupts DISABLED and also
    ; it returns with interrupts DISABLED.
    MOVS    r3,#1
    LSLS    r3,r3,#24         ; r3:=(1 << 24), set the T bit  (new xpsr)
    LDR     r2,QK_sched_addr  ; address of the QK scheduler   (new pc)
    LDR     r1,Thread_sched_ret_addr ; return address after the call (new lr)

    SUB     sp,sp,#8*4        ; reserve space for exception stack frame
    STR     r0,[sp]           ; save the prio argument (new r0)
    ADD     r0,sp,#5*4        ; r0 := 5 registers below the top of stack
    STM     r0!,{r1-r3}       ; save xpsr,pc,lr

    MOVS    r0,#6
    MVNS    r0,r0             ; r0 := ~6 == 0xFFFFFFF9
    BX      r0                ; exception-return to the QK scheduler

    ; This part of the code executes when PendSV returns to the preempted task
PendSV_ret:
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#0
    MSR     BASEPRI,r0        ; enable interrupts (clear BASEPRI)
  .else                       ; Cortex-M0/M0+/M1 ?
    CPSIE   i                 ; enable interrupts (clear PRIMASK)
  .endif                      ; M0/M0+/M1
    ; >>>>>>>>>>>>>>>>>>>>>>>> CRITICAL SECTION END >>>>>>>>>>>>>>>>>>>>>>>>>>

  .if __TI_VFP_SUPPORT__      ; if VFP available...
    POP     {r0,pc}           ; pop stack "aligner" and EXC_RETURN to PC
  .else                       ; no VFP
    BX      lr                ; return to the preempted task
  .endif                      ; VFP
  .endasmfunc

    ; NOTE: the following code does not execute in the PendSV context!
    ;=========================================================================
    ; NOTE: QK scheduler returns with interrupts DISABLED.
Thread_sched_ret: .asmfunc    ; to ensure that the label is THUMB
    ; After the QK scheduler returns, we need to resume the preempted
    ; task. However, this must be accomplished by a return-from-exception,
    ; while we are still in the task context. The switch to the exception
    ; contex is accomplished by triggering the PendSV exception.

    ; set QK_attr_.aux to non-zero to tell the next PendSV instance
    ; to remove its own stack frame
    LDR     r3,QK_attr_addr
    MOVS    r1,#0xFF
    STR     r1,[r3,#QK_AUX]   ; QK_attr_.aux = 0xFF (not zero)

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#0
    MSR     BASEPRI,r0        ; enable interrupts (clear BASEPRI)

    ; before triggering the PendSV exception, make sure that the
    ; VFP stack frame will NOT be used...
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    MRS     r0,CONTROL        ; r0 := CONTROL
    BICS    r0,r0,#4          ; r0 := r0 & ~4 (FPCA bit)
    MSR     CONTROL,r0        ; CONTROL := r0 (clear CONTROL[2] FPCA bit)
  .endif                      ; VFP

  .else                       ; Cortex-M0/M0+/M1 ?
    CPSIE   i                 ; enable interrupts (clear PRIMASK)
  .endif                      ; M0/M0+/M1

    ; trigger PendSV to return to preempted thread...
    LDR     r0,ICSR_addr      ; Interrupt Control and State Register
    MOVS    r1,#1
    LSLS    r1,r1,#28         ; r0 := (1 << 28) (PendSV bit)
    STR     r1,[r0]           ; ICSR[28] := 1 (pend PendSV)
Thread_wait_PendSV:
    B       Thread_wait_PendSV ; wait for preemption by PendSV
  .endasmfunc


;*****************************************************************************
; The QF_set_BASEPRI function sets the BASEPRI register to the value
; passed in r0.
; C prototype: void QF_set_BASEPRI(unsigned basePri);
;*****************************************************************************
QF_set_BASEPRI: .asmfunc
    MSR     BASEPRI,r0        ; set BASEPRI
    BX      lr                ; return to the caller
  .endasmfunc


;*****************************************************************************
; The QK_get_IPSR function gets the IPSR register and returns it in r0.
; C prototype: uint32_t QK_get_IPSR(void);
;*****************************************************************************
QK_get_IPSR: .asmfunc
    MRS     r0,ipsr           ; r0 := IPSR
    BX      lr                ; return to the caller
  .endasmfunc

;*****************************************************************************
; The assert_failed() function restores the SP (in case stack is corrupted)
; and calls Q_onAssert(module, loc)
; C prototype: void assert_failed(char const *module, int loc);
;*****************************************************************************
assert_failed: .asmfunc
    LDR     r0,VTOR_addr      ; r0 := address of Vector Table Offset register
    LDR     r0,[r0,#0]        ; r0 := contents of VTOR
    LDR     r0,[r0]           ; r0 := VT[0] (first entry is the top of stack)
    MSR     MSP,r0            ; main SP := initial top of stack
    BL      Q_onAssert
  .endasmfunc

;*****************************************************************************
; Constants for PC-relative LDR
;*****************************************************************************
SHPR_addr:        .word 0xE000ED18
ICSR_addr:        .word 0xE000ED04
VTOR_addr:        .word 0xE000ED08

;*****************************************************************************
; Addresses for PC-relative LDR
;*****************************************************************************
QK_attr_addr:     .word QK_attr_
QK_sched_addr:    .word QK_sched_
Thread_sched_ret_addr .word Thread_sched_ret
