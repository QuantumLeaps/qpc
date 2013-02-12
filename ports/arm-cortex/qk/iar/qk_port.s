;*****************************************************************************
; Product: QK port to ARM Cortex-M (M0,M0+,M1,M3,M4,M4F), IAR ARM assembler
; Last Updated for Version: 4.5.04
; Date of the Last Update:  Feb 04, 2013
;
;                    Q u a n t u m     L e a P s
;                    ---------------------------
;                    innovating embedded systems
;
; Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
;
; This program is open source software: you can redistribute it and/or
; modify it under the terms of the GNU General Public License as published
; by the Free Software Foundation, either version 2 of the License, or
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
; Quantum Leaps Web sites: http://www.quantum-leaps.com
;                          http://www.state-machine.com
; e-mail:                  info@quantum-leaps.com
;*****************************************************************************

    RSEG CODE:CODE:NOROOT(2)

    PUBLIC  QK_init
    PUBLIC  PendSV_Handler    ; CMSIS-compliant PendSV exception name
    PUBLIC  SVC_Handler       ; CMSIS-compliant SVC exception name

    EXTERN  QK_schedPrio_     ; external reference
    EXTERN  QK_sched_         ; external reference


;*****************************************************************************
; The QK_init function sets the priorities of PendSV and SVCall exceptions
; to 0xFF and 0x00, respectively. The function internally disables
; interrupts, but restores the original interrupt lock before exit.
;*****************************************************************************
QK_init
    MRS     r0,PRIMASK        ; store the state of the PRIMASK in r0
    CPSID   i                 ; disable interrupts (set PRIMASK)

    LDR     r1,=0xE000ED18    ; System Handler Priority Register
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
    PUSH    {lr}              ; push the exception lr (EXC_RETURN)
    CPSID   i                 ; disable interrupts at processor level
    BL      QK_schedPrio_     ; check if we have preemption
    CMP     r0,#0             ; is prio == 0 ?
    BNE.N   scheduler         ; if prio != 0, branch to scheduler

    CPSIE   i                 ; enable interrupts at processor level
    POP     {r0}              ; pop the EXC_RETURN into r0 (low register)
    BX      r0                ; exception-return to the task

scheduler:
    SUB     sp,sp,#4          ; align the stack to 8-byte boundary
    MOVS    r3,#1
    LSLS    r3,r3,#24         ; r3:=(1 << 24), set the T bit  (new xpsr)
    LDR     r2,=QK_sched_     ; address of the QK scheduler   (new pc)
    LDR     r1,=svc_ret       ; return address after the call (new lr)
    PUSH    {r1-r3}           ; push xpsr,pc,lr
    SUB     sp,sp,#(4*4)      ; don't care for r12,r3,r2,r1
    PUSH    {r0}              ; push the prio argument        (new r0)
    MOVS    r0,#0x6
    MVNS    r0,r0             ; r0 := ~0x6 == 0xFFFFFFF9
    BX      r0                ; exception-return to the scheduler

svc_ret:
    CPSIE   i                 ; enable interrupts to allow SVCall exception

#ifdef __ARMVFP__             ; If Vector FPU used-clear CONTROL[2] (FPCA bit)
    MRS     r0,CONTROL        ; r0 := CONTROL
    MOVS    r1,#4             ; r1 := 0x04 (FPCA bit)
    BICS    r0,r1             ; r0 := r0 & ~r1
    MSR     CONTROL,r0        ; CONTROL := r0
#endif

    SVC     #0                ; SV exception returns to the preempted task


;*****************************************************************************
; The SVC_Handler exception handler is used for returning back to the
; interrupted task. The SVCall exception simply removes its own interrupt
; stack frame from the stack and returns to the preempted task using the
; interrupt stack frame that must be at the top of the stack.
;*****************************************************************************
SVC_Handler:
    ADD     sp,sp,#(9*4)      ; remove one 8-register exception frame
                              ; plus the "aligner" from the stack
    POP     {r0}              ; pop the original EXC_RETURN into r0
    BX      r0                ; return to the preempted task


    ALIGNROM 2,0xFF           ; make sure the END is properly aligned
    END

