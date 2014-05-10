;*****************************************************************************
; Product: QK port to ARM Cortex-M (M0,M0+,M1,M3,M4,M4F), TI ARM assembler
; Last updated for version 5.3.0
; Last updated on  2014-04-23
;
;                    Q u a n t u m     L e a P s
;                    ---------------------------
;                    innovating embedded systems
;
; Copyright (C) Quantum Leaps, www.state-machine.com.
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
; Web:   www.state-machine.com
; Email: info@state-machine.com
;*****************************************************************************

    .text
    .thumb

    .global QK_init
    .global PendSV_Handler    ; CMSIS-compliant PendSV exception name
    .global SVC_Handler       ; CMSIS-compliant SVC exception name

    .ref    QK_schedPrio_     ; external reference
    .ref    QK_sched_         ; external reference


;*****************************************************************************
; The QK_init function sets the priorities of PendSV and SVCall exceptions
; to 0xFF and 0x00, respectively. The function internally disables
; interrupts, but restores the original interrupt lock before exit.
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
    LDR     r2,[r1,#4]        ; load the System 8-11 Priority Register
    LSLS    r3,r3,#8
    BICS    r2,r3             ; set PRI_11 (SVCall) to 0x00
    STR     r2,[r1,#4]        ; write the System 8-11 Priority Register

    MSR     PRIMASK,r0        ; restore the original PRIMASK
    BX      lr                ; return to the caller

SHPR_addr:  .word 0E000ED18h
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
; Also, *all* ISRs in the QK application must trigger the PendSV exception
; by calling the QK_ISR_EXIT() macro.
;
; Due to tail-chaining and its lowest priority, the PendSV exception will be
; entered immediately after the exit from the *last* nested interrupt (or
; exception). In QK, this is exactly the time when the QK scheduler needs to
; check for the asynchronous preemption.
;*****************************************************************************
PendSV_Handler: .asmfunc
    PUSH    {lr}              ; push the exception lr (EXC_RETURN)
    .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__
    MOVS    r0,#(0xFF >> 2)   ; Keep in synch with QF_BASEPRI in qf_port.h!
    MSR     BASEPRI,r0        ; disable interrupts at processor level
    .else
    CPSIE   i                 ; enable interrupts at processor level
    .endif

    BL      QK_schedPrio_     ; check if we have preemption
    CMP     r0,#0             ; is prio == 0 ?
    BNE.N   scheduler         ; if prio != 0, branch to scheduler

    .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__
    MOVS    r0,#(0xFF >> 2)   ; Keep in synch with QF_BASEPRI in qf_port.h!
    MSR     BASEPRI,r0        ; disable interrupts at processor level
    .else
    CPSIE   i                 ; enable interrupts at processor level
    .endif

    POP     {r0}              ; pop the EXC_RETURN into r0 (low register)
    BX      r0                ; exception-return to the task

scheduler:
    SUB     sp,sp,#4          ; align the stack to 8-byte boundary
    MOVS    r3,#1
    LSLS    r3,r3,#24         ; r3:=(1 << 24), set the T bit  (new xpsr)
    LDR     r2,QK_sched_addr  ; address of the QK scheduler   (new pc)
    LDR     r1,svc_ret_addr   ; return address after the call (new lr)
    PUSH    {r1-r3}           ; push xpsr,pc,lr
    SUB     sp,sp,#(4*4)      ; don't care for r12,r3,r2,r1
    PUSH    {r0}              ; push the prio argument        (new r0)
    MOVS    r0,#0x6
    MVNS    r0,r0             ; r0 := ~0x6 == 0xFFFFFFF9
    BX      r0                ; exception-return to the scheduler

svc_ret:
    .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__
    MOVS    r0,#(0xFF >> 2)   ; Keep in synch with QF_BASEPRI in qf_port.h!
    MSR     BASEPRI,r0        ; disable interrupts at processor level
    .else
    CPSIE   i                 ; enable interrupts at processor level
    .endif

    .if __TI_VFP_SUPPORT__    ; If Vector FPU used-clear CONTROL[2] (FPCA bit)
    MRS     r0,CONTROL        ; r0 := CONTROL
    MOVS    r1,#4             ; r1 := 0x04 (FPCA bit)
    BICS    r0,r1             ; r0 := r0 & ~r1
    MSR     CONTROL,r0        ; CONTROL := r0
    .endif

    SVC     #0                ; SV exception returns to the preempted task

QK_sched_addr .word QK_sched_
svc_ret_addr  .word svc_ret
    .endasmfunc


;*****************************************************************************
; The SVC_Handler exception handler is used for returning back to the
; interrupted task. The SVCall exception simply removes its own interrupt
; stack frame from the stack and returns to the preempted task using the
; interrupt stack frame that must be at the top of the stack.
;*****************************************************************************
SVC_Handler: .asmfunc
    ADD     sp,sp,#(9*4)      ; remove one 8-register exception frame
                              ; plus the "aligner" from the stack
    POP     {r0}              ; pop the original EXC_RETURN into r0
    BX      r0                ; return to the preempted task
    .endasmfunc

