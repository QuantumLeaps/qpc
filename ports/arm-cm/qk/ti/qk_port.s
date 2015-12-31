;*****************************************************************************
; Product: QK port to ARM Cortex-M (M0,M0+,M1,M3,M4,M7), TI-ARM assembler
; Last Updated for Version: 5.6.1
; Date of the Last Update:  2015-12-30
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
    .global QK_ISR_ENTRY      ; inform QK about interrupt entry
    .global QK_ISR_EXIT       ; inform QK about interrupt exit
    .global assert_failed     ; low-level assert handler

    .ref    QK_schedPrio_     ; external reference
    .ref    QK_sched_         ; external reference
    .ref    QK_intNest_       ; external reference
    .ref    Q_onAssert        ; external reference

    ; NOTE: keep in synch with QF_BASEPRI value defined in "qf_port.h" !!!
QF_BASEPRI: .equ  (0xFF >> 2)

    .data
;*****************************************************************************
; Priority of the next task to execute or zero to indicate return
; to the preempted task
;*****************************************************************************
QK_nextPrio_: .word   0


    .text
    .thumb
;*****************************************************************************
; The QK_init function sets the priorities of PendSV to 0xFF (lowest).
; The priority is set within a critical section.
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
; Also, *all* ISRs in the QK application must trigger the PendSV exception
; by calling the QK_ISR_EXIT() function.
;
; Due to tail-chaining and its lowest priority, the PendSV exception will be
; entered immediately after the exit from the *last* nested interrupt (or
; exception). In QK, this is exactly the time when the QK scheduler needs to
; check for the asynchronous preemption.
;*****************************************************************************
PendSV_Handler: .asmfunc

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#QF_BASEPRI
    MSR     BASEPRI,r0        ; selectively disable interrupts
  .else                       ; M0/M0+/M1
    CPSID   i                 ; disable interrupts (set PRIMASK)
  .endif                      ; M0/M0+/M1
    ISB                       ; reset the instruction pipeline

    LDR     r0,QK_nextPrio_addr
    LDR     r0,[r0]
    CMP     r0,#0
    BNE.N   PendSV_sched      ; if QK_nextPrio_ != 0, branch to scheduler

    ; QK_nextPrio_ == 0: return to the preempted task...
    ADD     sp,sp,#(8*4)      ; remove one 8-register exception frame

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    ; NOTE: r0 == 0 at this point
    MSR     BASEPRI,r0        ; enable interrupts (clear BASEPRI)
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    POP     {r0,pc}           ; pop stack "aligner" and EXC_RETURN to PC
  .else
    MOVS    r0,#6
    MVNS    r0,r0             ; r0 := ~6 == 0xFFFFFFF9
    BX      r0                ; exception-return to the task
  .endif                      ; VFP available
  .else                       ; M0/M0+/M1
    CPSIE   i                 ; enable interrupts (clear PRIMASK)
    MOVS    r0,#6
    MVNS    r0,r0             ; r0 := ~6 == 0xFFFFFFF9
    BX      r0                ; exception-return to the task
  .endif                      ; M0/M0+/M1

PendSV_sched:
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    PUSH    {r0,lr}           ; push lr (EXC_RETURN) plus stack "aligner"
  .endif                      ; VFP available

    MOVS    r3,#1
    LSLS    r3,r3,#24         ; r3:=(1 << 24), set the T bit  (new xpsr)
    LDR     r2,QK_sched_addr  ; address of the QK scheduler   (new pc)
    LDR     r1,PendSV_sched_ret_addr ; ret address after the call (new lr)

    SUB     sp,sp,#8*4        ; reserve space for exception stack frame
    STR     r0,[sp]           ; save the prio argument        (new r0)
    ADD     r0,sp,#5*4        ; r0 := 5 registers below the top of stack
    STM     r0!,{r1-r3}       ; save xpsr,pc,lr

    MOVS    r0,#6
    MVNS    r0,r0             ; r0 := ~6 == 0xFFFFFFF9
    BX      r0                ; exception-return to the QK scheduler
  .endasmfunc

PendSV_sched_ret: .asmfunc    ; to ensure that the label is THUMB
    LDR     r0,QK_nextPrio_addr
    MOVS    r1,#0
    STR     r1,[r0]           ; QK_nextPrio_ = 0;

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    MRS     r0,CONTROL        ; r0 := CONTROL
    BICS    r0,r0,#4          ; r0 := r0 & ~4 (FPCA bit)
    MSR     CONTROL,r0        ; CONTROL := r0 (clear CONTROL[2] FPCA bit)
  .endif                      ; VFP available
    MOVS    r0,#0
    MSR     BASEPRI,r0        ; enable interrupts (clear BASEPRI)
  .else                       ; M0/M0+/M1
    CPSIE   i                 ; enable interrupts (clear BASEPRI)
  .endif                      ; M0/M0+/M1

    ; trigger PendSV to return to preempted task...
    LDR     r0,ICSR_addr      ; Interrupt Control and State Register
    MOVS    r1,#1
    LSLS    r1,r1,#28         ; r0 := (1 << 28) (PENDSVSET bit)
    STR     r1,[r0]           ; ICSR[28] := 1 (pend PendSV)
PendSV_sched_wait:
    B       PendSV_sched_wait ; wait for preemption by PendSV
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
; Inform QK about interrupt entry.
; C prototype: void QK_ISR_ENTRY(void);
;*****************************************************************************
QK_ISR_ENTRY: .asmfunc
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#QF_BASEPRI
    MSR     BASEPRI,r0        ; selectively disable interrupts
    .else   ; M0/M0+/M1
    CPSID   i                 ; disable interrupts (set PRIMASK)
  .endif                      ; M0/M0+/M1

    ; ++QK_intNest_
    LDR     r1,QK_intNest_addr ; address of the QK_intNest_
    LDR     r0,[r1]
    ADDS    r0,r0,#1
    STR     r0,[r1]

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#0
    MSR     BASEPRI,r0        ; set BASEPRI (disable interrupts)
  .else                       ; M0/M0+/M1
    CPSIE   i                 ; enable interrupts (set PRIMASK)
  .endif                      ; M0/M0+/M1

    BX      lr                ; return to the caller
    .endasmfunc


;*****************************************************************************
; Inform QK about interrupt exit.
; C prototype: void QK_ISR_EXIT(void);
;*****************************************************************************
QK_ISR_EXIT: .asmfunc
    PUSH    {r0,lr}           ; push lr (return address) plus stack "aligner"
                              ; NOTE: because of the call to QK_schedPrio_()

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#QF_BASEPRI
    MSR     BASEPRI,r0        ; selectively disable interrupts
  .else                       ; M0/M0+/M1
    CPSID   i                 ; disable interrupts (set PRIMASK)
  .endif                      ; M0/M0+/M1

    ; --QK_intNest_
    LDR     r1,QK_intNest_addr ; address of the QK_intNest_
    LDR     r0,[r1]
    SUBS    r0,r0,#1
    STR     r0,[r1]

    BL      QK_schedPrio_     ; check if we have preemption
    CMP     r0,#0             ; is prio == 0 ?
    BEQ.N   no_preemption     ; if prio == 0, branch to no_preemption

    LDR     r1,QK_nextPrio_addr ; address of the QK_nextPrio_
    STR     r0,[r1]           ; QK_nextPrio_ = prio

    ; trigger PendSV to return to preempted task...
    LDR     r0,ICSR_addr      ; Interrupt Control and State Register
    MOVS    r1,#1
    LSLS    r1,r1,#28         ; r0 := (1 << 28) (PENDSVSET bit)
    STR     r1,[r0]           ; ICSR[28] := 1 (pend PendSV)

no_preemption:
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#0
    MSR     BASEPRI,r0        ; set BASEPRI (disable interrupts)
  .else                       ; M0/M0+/M1
    CPSIE   i                 ; enable interrupts (set PRIMASK)
  .endif                      ; M0/M0+/M1

    POP     {r0,pc}           ; pop the stack "aligner" and saved LR to PC
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
    ISB                       ; flush the instruction pipeline
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
QK_nextPrio_addr: .word QK_nextPrio_
QK_sched_addr:    .word QK_sched_
QK_intNest_addr:  .word QK_intNest_
PendSV_sched_ret_addr .word PendSV_sched_ret
