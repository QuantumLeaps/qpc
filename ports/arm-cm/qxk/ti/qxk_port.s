;*****************************************************************************
; Product: QXK port to ARM Cortex-M (M0,M0+,M3,M4,M7), TI-ARM assembler
; Last Updated for Version: 5.9.4
; Date of the Last Update:  2017-07-06
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
; https://state-machine.com
; mailto:info@state-machine.com
;*****************************************************************************

    .global QXK_init          ; initialze the QXK kernel
    .global QXK_stackInit_    ; initialize the stack of an extended thread
    .global PendSV_Handler    ; CMSIS-compliant PendSV exception name
    .global NMI_Handler       ; CMSIS-compliant NMI exception name

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    .global QF_set_BASEPRI    ; set BASEPRI register
  .endif                      ; M3/M4/M7

    .global QF_crit_entry     ; enter critical section
    .global QXK_get_IPSR      ; get the IPSR
    .global assert_failed     ; low-level assert handler

    .ref    QXK_attr_         ; QXK attribute structure
    .ref    QXK_threadRet_    ; return from an extended thread function
    .ref    QXK_activate_     ; external reference
    .ref    Q_onAssert        ; external reference

    ; NOTE: keep in synch with QF_BASEPRI value defined in "qf_port.h" !!!
QF_BASEPRI        .equ (0xFF >> 2)

    ; NOTE: keep in synch with the QXK_Attr struct in "qxk.h" !!!
QXK_CURR          .equ 0
QXK_NEXT          .equ 4
QXK_TOP_PRIO      .equ 8

    ; NOTE: keep in synch with the QMActive struct in "qf.h/qxk.h" !!!
QMACTIVE_OSOBJ   .equ 44
QMACTIVE_PRIO     .equ 48

    .text
    .thumb

;*****************************************************************************
; The QXK_init() function sets the priority of PendSV to 0xFF (lowest urgency).
; For Cortex-M3/4/7, it also sets priorities of all other exceptions and IRQs
; to the safe value. All this is performed in a nestable critical section.
;*****************************************************************************
QXK_init:   .asmfunc
    MRS     r0,PRIMASK        ; store the state of the PRIMASK
    MOV     r12,r0            ; r12 := PRIMASK
    CPSID   i                 ; PRIMASK := 1

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__ ; M3/4/7?
    ; NOTE:
    ; On Cortex-M3/M4/M7.., this QK port disables interrupts by means of
    ; the BASEPRI register. However, this method cannot disable interrupt
    ; priority zero, which is the default for all interrupts out of reset.
    ; The following code changes the SysTick priority and all IRQ priorities
    ; to the safe value QF_BASEPRI, wich the QF critical section can disable.
    ; This avoids breaching of the QF critical sections in case the
    ; application programmer forgets to explicitly set priorities of all
    ; "kernel aware" interrupts.

    ; set all prioriy bytes to QF_BASEPRI in r1
    MOVS    r1,#QF_BASEPRI
    LSLS    r1,r1,#8
    ORRS    r1,r1,#QF_BASEPRI
    LSLS    r1,r1,#8
    ORRS    r1,r1,#QF_BASEPRI
    LSLS    r1,r1,#8
    ORRS    r1,r1,#QF_BASEPRI

    LDR     r3,SHPR_addr      ; System Handler Priority Register
    LDR     r2,[r3]           ; r2 := SYSPRI1
    ORRS    r2,r1             ; r2 |= "all values to QF_BASEPRI"
    STR     r2,[r3]           ; SYSPRI1 |= r2, Usage-fault/Bus-fault/Mem-fault

    LDR     r2,[r3,#4]        ; r2 := SYSPRI2
    ORRS    r2,r1             ; r2 |= "all values to QF_BASEPRI"
    STR     r2,[r3,#4]        ; SYSPRI2 := r2, SVCall

    LDR     r2,[r3,#8]        ; r2 := SYSPRI3
    ORRS    r1,r1,#(0xFF << 16) ; r1 |= 0xFF for PendSV
    ORRS    r2,r1
    STR     r2,[r3,#8]        ; SYSPRI3 |= r2, SysTick/PendSV/Debug

    ; set again all prioriy bytes to QF_BASEPRI in r1
    MOVS    r1,#QF_BASEPRI
    LSLS    r1,r1,#8
    ORRS    r1,r1,#QF_BASEPRI
    LSLS    r1,r1,#8
    ORRS    r1,r1,#QF_BASEPRI
    LSLS    r1,r1,#8
    ORRS    r1,r1,#QF_BASEPRI

    LDR     r3,ICTR_addr      ; Interrupt Controller Type Register
    LDR     r3,[r3]           ; r3 := INTLINESUM
    LSLS    r3,r3,#3
    ADDS    r3,r3,#8          ; r3 == number of NVIC_PRIO registers

    ; loop over all implemented NVIC_PRIO registers for IRQs...
QXK_init_irq:
    SUBS    r3,r3,#1
    LDR     r2,PRI0_addr      ; NVIC_PRI0 register
    STR     r1,[r2,r3,LSL #2] ; NVIC_PRI0[r3]  := r1
    CMP     r3,#0
    BNE     QXK_init_irq

  .else                       ; Cortex-M0/M0+/M1 ?

    LDR     r3,=0xE000ED18    ; System Handler Priority Register
    LDR     r2,[r3,#8]        ; r2 := SYSPRI3
    MOVS    r1,#0xFF
    LSLS    r1,r1,#16
    ORRS    r2,r1
    STR     r2,[r3,#8]        ; SYSPRI3 := r2, PendSV <- 0xFF

  .endif                      ; M0/M0+/M1

    MOV     r0,r12            ; r0 := original PRIMASK
    MSR     PRIMASK,r0        ; PRIMASK := r0
    BX      lr                ; return to the caller
  .endasmfunc

;*****************************************************************************
; The PendSV_Handler exception handler is used for handling context switch
; and asynchronous preemption in QXK. The use of the PendSV exception is
; the recommended and most efficient method for performing context switches
; with ARM Cortex-M.
;
; The PendSV exception should have the lowest priority in the whole system
; (0xFF, see QXK_init). All other exceptions and interrupts should have higher
; priority. For example, for NVIC with 2 priority bits all interrupts and
; exceptions must have numerical value of priority lower than 0xC0. In this
; case the interrupt priority levels available to your applications are (in
; the order from the lowest urgency to the highest urgency): 0x80, 0x40, 0x00.
;
; Also, *all* "kernel aware" ISRs in the QXK application must call the
; QXK_ISR_EXIT() macro, which triggers PendSV when it detects a need for
; a context switch or asynchronous preemption.
;
; Due to tail-chaining and its lowest priority, the PendSV exception will be
; entered immediately after the exit from the *last* nested interrupt (or
; exception). In QXK, this is exactly the time when the QXK activator needs to
; handle the asynchronous preemption.
;*****************************************************************************
PendSV_Handler: .asmfunc

    ; Prepare some constants (an address and a bitmask) before entering
    ; a critical section...
    LDR     r3,QXK_attr_addr
    LDR     r2,ICSR_addr      ; Interrupt Control and State Register
    MOVS    r1,#1
    LSLS    r1,r1,#27         ; r0 := (1 << 27) (UNPENDSVSET bit)

    ; <<<<<<<<<<<<<<<<<<<<<<< CRITICAL SECTION BEGIN <<<<<<<<<<<<<<<<<<<<<<<<<
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#QF_BASEPRI
    CPSID   i                 ; selectively disable interrutps with BASEPRI
    MSR     BASEPRI,r0        ; apply the workaround the Cortex-M7 erraturm
    CPSIE   i                 ; 837070, see ARM-EPM-064408.
  .else                       ; M0/M0+
    CPSID   i                 ; disable interrupts (set PRIMASK)
  .endif                      ; M0/M0+

    ; The PendSV exception handler can be preempted by an interrupt,
    ; which might pend PendSV exception again. The following write to
    ; ICSR[27] un-pends any such spurious instance of PendSV.
    STR     r1,[r2]           ; ICSR[27] := 1 (unpend PendSV)

    ; Check QXK_attr_.next, which contains the pointer to the next thread
    ; to run, which is set in QXK_ISR_EXIT(). This pointer must not be NULL.
    LDR     r0,[r3,#QXK_NEXT] ; r1 := QXK_attr_.next
    CMP     r0,#0             ; is (QXK_attr_.next == 0)?
    BEQ     PendSV_error      ; branch if (QXK_attr_.next == 0)

    ; Load pointers into registers...
    MOV     r12,r0            ; save QXK_attr_.next in r12
    LDR     r2,[r0,#QMACTIVE_OSOBJ] ; r2 := QXK_attr_.next->osObject
    LDR     r1,[r3,#QXK_CURR] ; r1 := QXK_attr_.curr

    CMP     r1,#0             ; (QXK_attr_.curr != 0)?
    BNE     PendSV_save_ex    ; branch if (current thread is extended)

    CMP     r2,#0             ; (QXK_attr_.next->osObject != 0)?
    BNE     PendSV_save_ao    ; branch if (next tread is extended)

PendSV_activate:
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    PUSH    {r0,lr}           ; ...push lr (EXC_RETURN) plus stack-aligner
  .endif                      ; VFP available
    ; The QXK activator must be called in a thread context, while this code
    ; executes in the handler contex of the PendSV exception. The switch
    ; to the Thread mode is accomplished by returning from PendSV using
    ; a fabricated exception stack frame, where the return address is
    ; QXK_activate_().
    ;
    ; NOTE: the QXK activator is called with interrupts DISABLED and also
    ; returns with interrupts DISABLED.
    MOVS    r3,#1
    LSLS    r3,r3,#24         ; r3:=(1 << 24), set the T bit  (new xpsr)
    LDR     r2,QXK_activate_addr ; address of QXK_activate_
    SUBS    r2,r2,#1          ; align Thumb-address at halfword (new pc)
    LDR     r1,Thread_ret_addr ; return address after the call  (new lr)

    SUB     sp,sp,#8*4        ; reserve space for exception stack frame
    ADD     r0,sp,#5*4        ; r0 := 5 registers below the top of stack
    STM     r0!,{r1-r3}       ; save xpsr,pc,lr

    MOVS    r0,#6
    MVNS    r0,r0             ; r0 := ~6 == 0xFFFFFFF9
    BX      r0                ; exception-return to the QXK activator

    ;-------------------------------------------------------------------------
PendSV_error:
    LDR     r3,assert_failed_addr
    BX      r3                ; long-branch to the assertion-handler

    ;=========================================================================
    ; Saving AO-thread before crossing to eXtended-thread
    ; expected register contents:
    ; r0  -> QXK_attr_.next
    ; r1  -> QXK_attr_.curr
    ; r2  -> QXK_attr_.next->osObject (SP)
    ; r3  -> &QXK_attr_
    ; r12 -> QXK_attr_.next
PendSV_save_ao:
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    PUSH    {r4-r11}          ; save r4-r11 on top of the exception frame
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    TST     lr,#(1 << 4)      ; is it return with the VFP exception frame?
    IT      EQ                ; if lr[4] is zero...
    VSTMDBEQ sp!,{s16-s31}    ; ... save VFP registers s16..s31

    PUSH    {r0,lr}           ; save the "aligner" and the EXC_RETURN value
  .endif                      ; VFP available
  .else                       ; M0/M0+
    PUSH    {r4-r7}           ; save the low registers
    MOV     r4,r8             ; move the high registers to low registers...
    MOV     r5,r9
    MOV     r6,r10
    MOV     r7,r11
    PUSH    {r4-r7}           ; save the high registers
  .endif                      ; M0/M0+

    CMP     r2,#0
    BNE     PendSV_restore_ex ; branch if (QXK_attr_.next->osObject != 0)
    ; otherwise continue to restoring next AO-thread...

    ;-------------------------------------------------------------------------
    ; Restoring AO-thread after crossing from eXtended-thread
    ; expected register contents:
    ; r1  -> QXK_attr_.curr
    ; r2  -> QXK_attr_.next->osObject (SP)
    ; r3  -> &QXK_attr_
    ; r12 -> QXK_attr_.next
PendSV_restore_ao:
    MOVS    r0,#0
    STR     r0,[r3,#QXK_CURR] ; QXK_attr_.curr := 0 (QXK_attr_.next)

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    POP     {r0,lr}           ; restore alighner and EXC_RETURN into lr
    TST     lr,#(1 << 4)      ; is it return to the VFP exception frame?
    IT      EQ                ; if EXC_RETURN[4] is zero...
    VLDMIAEQ sp!,{s16-s31}    ; ... restore VFP registers s16..s31
  .else
    BIC     lr,lr,#(1 << 2)   ; make sure MSP is used
  .endif                      ; VFP available
    POP     {r4-r11}          ; restore r4-r11 from the next thread's stack
  .else                       ; M0/M0+
    MOV     r0,sp             ; r0 := top of stack
    MOV     r1,r0
    ADDS    r1,r1,#(4*4)      ; point r0 to the 4 high registers r7-r11
    LDMIA   r1!,{r4-r7}       ; pop the 4 high registers into low registers
    MOV     r8,r4             ; move low registers into high registers
    MOV     r9,r5
    MOV     r10,r6
    MOV     r11,r7
    LDMIA   r0!,{r4-r7}       ; pop the low registers
    ADD     sp,sp,#(8*4)      ; remove 8 registers from the stack

    MOVS    r1,#6
    MVNS    r1,r1             ; r2 := ~6 == 0xFFFFFFF9
    MOV     lr,r1             ; make sure MSP is used
  .endif                      ; M0/M0+

    MOV     r0,r12            ; r0 := QXK_attr_.next
    LDR     r0,[r0,#QMACTIVE_PRIO] ; r0 := QXK_attr_.next->prio
    LDR     r1,[r3,#QXK_TOP_PRIO]  ; r1 := QXK_attr_.topPrio
    CMP     r1,r0
    BCC     PendSV_activate   ; if (next->prio > topPrio) activate the next AO

    ; otherwise re-enable interrupts and return to the preempted AO-thread
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#0
    MSR     BASEPRI,r0        ; enable interrupts (clear BASEPRI)
  .else                       ; M0/M0+
    CPSIE   i                 ; enable interrupts (clear PRIMASK)
  .endif                      ; M0/M0+
    ; >>>>>>>>>>>>>>>>>>>>>>>> CRITICAL SECTION END >>>>>>>>>>>>>>>>>>>>>>>>>>
    BX      lr                ; return to the preempted AO-thread

    ;-------------------------------------------------------------------------
    ; Saving extended-thread before crossing to AO-thread
    ; expected register contents:
    ; r0  -> QXK_attr_.next
    ; r1  -> QXK_attr_.curr
    ; r2  -> QXK_attr_.next->osObject (SP)
    ; r3  -> &QXK_attr_
    ; r12 -> QXK_attr_.next
PendSV_save_ex:
    MRS     r0,PSP            ; r0 := Process Stack Pointer

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    STMDB   r0!,{r4-r11}      ; save r4-r11 on top of the exception frame
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    TST     lr,#(1 << 4)      ; is it return with the VFP exception frame?
    IT      EQ                ; if lr[4] is zero...
    VSTMDBEQ r0!,{s16-s31}    ; ... save VFP registers s16..s31
    STMDB   r0!,{r1,lr}       ; save the "aligner" and the EXC_RETURN value
  .endif                      ; VFP available
  .else                       ; M0/M0+
    SUBS    r0,r0,#(8*4)      ; make room for 8 registers r4-r11
    MOVS    r1,r0             ; r1 := temporary PSP (do not clobber r0!)
    STMIA   r1!,{r4-r7}       ; save the low registers
    MOV     r4,r8             ; move the high registers to low registers...
    MOV     r5,r9
    MOV     r6,r10
    MOV     r7,r11
    STMIA   r1!,{r4-r7}       ; save the high registers
    ; NOTE: at this point r0 holds the top of stack

    LDR     r1,[r3,#QXK_CURR] ; r1 := QXK_attr_.curr (restore value)
  .endif                      ; M0/M0+

    ; store the SP of the current extended-thread
    STR     r0,[r1,#QMACTIVE_OSOBJ] ; QXK_attr_.curr->osObject := r0
    MOV     r0,r12            ; QXK_attr_.next (restore value)

    CMP     r2,#0
    BEQ     PendSV_restore_ao ; branch if (QXK_attr_.next->osObject == 0)
    ; otherwise continue to restoring next extended-thread...

    ;-------------------------------------------------------------------------
    ; Restoring extended-thread after crossing from AO-thread
    ; expected register contents:
    ; r0  -> QXK_attr_.next
    ; r1  -> QXK_attr_.curr
    ; r2  -> QXK_attr_.next->osObject (SP)
    ; r3  -> &QXK_attr_
    ; r12 -> QXK_attr_.next
PendSV_restore_ex:
    STR     r0,[r3,#QXK_CURR] ; QXK_attr_.curr := r0 (QXK_attr_.next)

    ; exit the critical section
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r1,#0
    MSR     BASEPRI,r1        ; enable interrupts (clear BASEPRI)
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    LDMIA   r2!,{r1,lr}       ; restore aligner and EXC_RETURN into lr
    TST     lr,#(1 << 4)      ; is it return to the VFP exception frame?
    IT      EQ                ; if lr[4] is zero...
    VLDMIAEQ r2!,{s16-s31}    ; ... restore VFP registers s16..s31
  .else
    ORR     lr,lr,#(1 << 2)   ; make sure PSP is used
  .endif                      ; VFP available
    LDMIA   r2!,{r4-r11}      ; restore r4-r11 from the next thread's stack
  .else                       ; M0/M0+
    CPSIE   i                 ; enable interrupts (clear PRIMASK)

    MOVS    r0,r2             ; r2 := top of stack
    ADDS    r0,r0,#(4*4)      ; point r0 to the 4 high registers r7-r11
    LDMIA   r0!,{r4-r7}       ; pop the 4 high registers into low registers
    MOV     r8,r4             ; move low registers into high registers
    MOV     r9,r5
    MOV     r10,r6
    MOV     r11,r7
    LDMIA   r2!,{r4-r7}       ; pop the low registers
    MOVS    r2,r0             ; r2 := holds the new top of stack

    MOVS    r1,#2
    MVNS    r1,r1             ; r1 := ~2 == 0xFFFFFFFD
    MOV     lr,r1             ; make sure PSP is used
  .endif                      ; M0/M0+

    ; set the PSP to the next thread's SP
    MSR     PSP,r2            ; Process Stack Pointer := r2

    BX      lr                ; return to the next extended-thread
  .endasmfunc


;*****************************************************************************
; Thread_ret is a helper function executed when the QXK activator returns.
;
; NOTE: Thread_ret does not execute in the PendSV context!
; NOTE: Thread_ret executes entirely with interrupts DISABLED.
;*****************************************************************************
Thread_ret: .asmfunc          ; to ensure that the label is THUMB
    ; After the QXK activator returns, we need to resume the preempted
    ; thread. However, this must be accomplished by a return-from-exception,
    ; while we are still in the thread context. The switch to the exception
    ; contex is accomplished by triggering the NMI exception.
    ; NOTE: The NMI exception is triggered with nterrupts DISABLED,
    ; because QXK activator disables interrutps before return.

    ; before triggering the NMI exception, make sure that the
    ; VFP stack frame will NOT be used...
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    MRS     r0,CONTROL        ; r0 := CONTROL
    BICS    r0,r0,#4          ; r0 := r0 & ~4 (FPCA bit)
    MSR     CONTROL,r0        ; CONTROL := r0 (clear CONTROL[2] FPCA bit)
    ISB                       ; ISB after MSR CONTROL (ARM AN 321, Sect.4.16)
  .endif                      ; VFP available

    ; trigger NMI to return to preempted thread...
    ; NOTE: The NMI exception is triggered with nterrupts DISABLED
    LDR     r0,ICSR_addr      ; Interrupt Control and State Register
    MOVS    r1,#1
    LSLS    r1,r1,#31         ; r0 := (1 << 31) (NMI bit)
    STR     r1,[r0]           ; ICSR[31] := 1 (pend NMI)
Thread_wait_NMI:
    B       Thread_wait_NMI   ; wait for preemption by NMI
  .endasmfunc


;*****************************************************************************
; The NMI_Handler exception handler is used for returning back to the
; interrupted task. The NMI exception simply removes its own interrupt
; stack frame from the stack and returns to the preempted task using the
; interrupt stack frame that must be at the top of the stack.
;
; NOTE: The NMI exception is entered with interrupts DISABLED, so it needs
; to re-enable interrupts before it returns to the preempted task.
;*****************************************************************************
NMI_Handler: .asmfunc
    ADD     sp,sp,#(8*4)      ; remove one 8-register exception frame

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#0
    MSR     BASEPRI,r0        ; enable interrupts (clear BASEPRI)
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    POP     {r0,pc}           ; pop stack "aligner" and EXC_RETURN to PC
  .else
    BX      lr                ; return to the preempted task
  .endif
  .else                       ; Cortex-M0/M0+/M1 ?
    CPSIE   i                 ; enable interrupts (clear PRIMASK)
    BX      lr                ; return to the preempted task
  .endif                      ; M0/M0+/M1
  .endasmfunc


;*****************************************************************************
; Initialize the private stack of a QXK thread.
;
; NOTE: the function aligns the stack to the 8-byte boundary for
; compatibility with the AAPCS. Additionally, the function pre-fills
; the stack with the known bit pattern (0xDEADBEEF).
;
; The C signature:
; void QXK_stackInit_(void *act, QActionHandler thread,
;                     void *stkSto, uint_fast16_t stkSize);
;
; NOTE: QXK_stackInit_() must be called before the QF is made
; aware of this QXK thread. In that case there can be no external
; communication with this thread, so no critical section is needed.
;*****************************************************************************
QXK_stackInit_: .asmfunc
    ; assignment of parameters (AAPCS)
    ; r0 - QMActive pointer (act)
    ; r1 - thread routine
    ; r2 - begining of stack
    ; r3 - size of stack [bytes]

    MOV     r12,r0            ; temporarily save r0 in r12 (act)
    STR     r1,[r0,#QMACTIVE_OSOBJ] ; temporarily save the thread routine
    ADDS    r3,r2,r3          ; r3 := end of stack (top of stack)

    ; round up the beginning of stack to the 8-byte boundary
    ; r2 := (((r2 -1) >> 3) + 1) << 3;
    SUBS    r0,r2,#1
    LSRS    r0,r0,#3
    ADDS    r0,r0,#1
    LSLS    r2,r0,#3

    ; round down the end of stack to the 8-byte boundary
    ; r3 := (r3 >> 3) << 3;
    LSRS    r0,r3,#3
    LSLS    r3,r0,#3

    ; make room for the thread's stack frame...
    SUBS    r3,r3,#16*4       ; r3 := top of the 16-register stack frame
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    SUBS    r3,r3,#2*4        ; r3 := top of the 18-register stack frame
  .endif                      ; VFP available

    ; pre-fill the unused part of the stack with 0xDEADBEEF...................
    LDR     r0,STACK_FILL_const
    MOV     r1,r0
QXK_stackInit_fill:
    STMIA   r2!,{r0,r1}
    CMP     r2,r3
    BLT.N   QXK_stackInit_fill

    ; prepare the standard exception (without VFP) stack frame................
    MOV     r0,r12            ; restore r0 from r12 (act)
    LDR     r1,[r0,#QMACTIVE_OSOBJ] ; restore the thread routine

    STR     r3,[r0,#QMACTIVE_OSOBJ] ; act->osObject := top of stack

  .if __TI_VFP_SUPPORT__      ; if VFP available...
    MOVS    r2,#0
    STMIA   r3!,{r2}          ; stack "aligner"

    ; synthesize EXC_RETURN for return to Thread mode with no FPU-state
    MOVS    r2,#2
    MVNS    r2,r2             ; r2 := ~2 == 0xFFFFFFFD
    STMIA   r3!,{r2}          ; save EXC_RETURN
  .endif                      ; VFP available

    MOVS    r2,#0x04
    STMIA   r3!,{r2}          ; r4

    MOVS    r2,#0x05
    STMIA   r3!,{r2}          ; r5

    MOVS    r2,#0x06
    STMIA   r3!,{r2}          ; r6

    MOVS    r2,#0x07
    STMIA   r3!,{r2}          ; r7

    MOVS    r2,#0x08
    STMIA   r3!,{r2}          ; r8

    MOVS    r2,#0x09
    STMIA   r3!,{r2}          ; r9

    MOVS    r2,#0x0A
    STMIA   r3!,{r2}          ; r10

    MOVS    r2,#0x0B
    STMIA   r3!,{r2}          ; r11

    STMIA   r3!,{r0}          ; r0 (argument to thread routine, me pointer)

    MOVS    r2,#0x01
    STMIA   r3!,{r2}          ; r1

    MOVS    r2,#0x02
    STMIA   r3!,{r2}          ; r2

    MOVS    r2,#0x03
    STMIA   r3!,{r2}          ; r3

    MOVS    r2,#0x0C
    STMIA   r3!,{r2}          ; r12

    LDR     r2,QXK_threadRet_addr
    STMIA   r3!,{r2}          ; LR (return address)

    STMIA   r3!,{r1}          ; PC (entry point, thread routine)

    MOVS    r2,#1
    LSLS    r2,r2,#24         ; r2 := 0x01000000
    STMIA   r3!,{r2}          ; xPSR

    BX      lr                ; return to the caller
  .endasmfunc


;*****************************************************************************
; The QF_set_BASEPRI function sets the BASEPRI register to the value
; passed in r0.
; NOTE: The BASEPRI register is implemented only in ARMv7 architecture
; and is **not** available in ARMv6 (M0/M0+/M1)
;
; C prototype: void QF_set_BASEPRI(unsigned basepri);
;*****************************************************************************
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
QF_set_BASEPRI: .asmfunc
    MSR     BASEPRI,r0        ; BASEPRI := r0 (basepri parameter)
    BX      lr                ; return to the caller
  .endasmfunc
  .endif                      ; M3/M4/M7

;*****************************************************************************
; The QF_crit_entry function enters the QF critical section
; passed in r0.
; NOTE: The BASEPRI register is implemented only in ARMv7 architecture
; and is **not** available in ARMv6 (M0/M0+/M1)
;
; C prototype: unsigned QF_crit_entry(void);
;*****************************************************************************
QF_crit_entry: .asmfunc
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MRS     r0,BASEPRI        ; r0 := BASEPRI (to return)
    MOVS    r1,#QF_BASEPRI
    CPSID   i                 ; selectively disable interrutps with BASEPRI
    MSR     BASEPRI,r1        ; apply the workaround the Cortex-M7 erraturm
    CPSIE   i                 ; 837070, see ARM-EPM-064408.
    BX      lr                ; return to the caller (previous BASEPRI in r0)
  .else                       ; M0/M0+
    MRS     r0,PRIMASK        ; r0 := PRIMASK (to return)
    CPSID   i                 ; globally disable interrutps with PRIMASK
    BX      lr                ; return to the caller (previous PRIMASK in r0)
  .endif                      ; M0/M0+
  .endasmfunc

;*****************************************************************************
; The QXK_get_IPSR function gets the IPSR register and returns it in r0.
; C prototype: uint32_t QXK_get_IPSR(void);
;*****************************************************************************
QXK_get_IPSR: .asmfunc
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
    DSB                       ; DSB/ISB pair if instructions needed...
    ISB                       ; ...after MSR MSP (ARM AN 321, Sect.4.10)
    BL      Q_onAssert
  .endasmfunc

;*****************************************************************************
; Constants for PC-relative LDR
;*****************************************************************************
SHPR_addr:        .word 0xE000ED18
ICSR_addr:        .word 0xE000ED04
VTOR_addr:        .word 0xE000ED08
ICTR_addr:        .word 0xE000E004 ; Interrupt Controller Type Register
PRI0_addr:        .word 0xE000E400 ; NVIC_PRI0 register
STACK_FILL_const: .word 0xDEADBEEF

;*****************************************************************************
; Addresses for PC-relative LDR
;*****************************************************************************
QXK_attr_addr:      .word QXK_attr_
QXK_activate_addr:  .word QXK_activate_
QXK_threadRet_addr: .word QXK_threadRet_
Thread_ret_addr:    .word Thread_ret
assert_failed_addr: .word assert_failed

