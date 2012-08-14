;-----------------------------------------------------------------------------
; Product:  QF/Vanilla port to ARM, IAR ARM Assembler 5.11
; Last Updated for Version: 4.0.00
; Date of the Last Update:  May 31, 2008
;
;                    Q u a n t u m     L e a P s
;                    ---------------------------
;                    innovating embedded systems
;
; Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
;
; This software may be distributed and modified under the terms of the GNU
; General Public License version 2 (GPL) as published by the Free Software
; Foundation and appearing in the file GPL.TXT included in the packaging of
; this file. Please note that GPL Section 2[b] requires that all works based
; on this software must also be made publicly available under the terms of
; the GPL ("Copyleft").
;
; Alternatively, this software may be distributed and modified under the
; terms of Quantum Leaps commercial licenses, which expressly supersede
; the GPL and are specifically designed for licensees interested in
; retaining the proprietary status of their code.
;
; Contact information:
; Quantum Leaps Web site:  http://www.quantum-leaps.com
; e-mail:                  info@quantum-leaps.com
;-----------------------------------------------------------------------------

NO_INT      DEFINE     0xC0     ; mask to disable interrupts (FIQ and IRQ)
NO_IRQ      DEFINE     0x80     ; mask to disable interrupts (FIQ and IRQ)
NO_FIQ      DEFINE     0x40     ; mask to disable interrupts (FIQ and IRQ)
FIQ_MODE    DEFINE     0x11
IRQ_MODE    DEFINE     0x12
SYS_MODE    DEFINE     0x1F

;-----------------------------------------------------------------------------
; Interrupt locking/unlocking
;-----------------------------------------------------------------------------

    SECTION .textrw:DATA:NOROOT(2)
    PUBLIC  QF_int_lock_SYS, QF_int_unlock_SYS
    CODE32

QF_int_lock_SYS:
    MRS     r0,cpsr             ; get the original CPSR in r0 to return
    MSR     cpsr_c,#(SYS_MODE | NO_INT) ; disable both IRQ/FIQ
    BX      lr                  ; return the original CPSR in r0


QF_int_unlock_SYS:
    MSR     cpsr_c,r0           ; restore the original CPSR from r0
    BX      lr                  ; return to ARM or THUMB


;-----------------------------------------------------------------------------
; IRQ assembly wrapper
;-----------------------------------------------------------------------------

    SECTION .textrw:DATA:NOROOT(2)
    PUBLIC  QF_irq
    EXTERN  BSP_irq
    CODE32

QF_irq:
; IRQ entry {{{
    MOV     r13,r0              ; save r0 in r13_IRQ
    SUB     r0,lr,#4            ; put return address in r0_SYS
    MOV     lr,r1               ; save r1 in r14_IRQ (lr)
    MRS     r1,spsr             ; put the SPSR in r1_SYS

    MSR     cpsr_c,#(SYS_MODE | NO_IRQ) ; SYSTEM, no IRQ, but FIQ enabled!
    STMFD   sp!,{r0,r1}         ; save SPSR and PC on SYS stack
    STMFD   sp!,{r2-r3,r12,lr}  ; save APCS-clobbered regs on SYS stack
    MOV     r0,sp               ; make the sp_SYS visible to IRQ mode
    SUB     sp,sp,#(2*4)        ; make room for stacking (r0_SYS, r1_SYS)

    MSR     cpsr_c,#(IRQ_MODE | NO_IRQ) ; IRQ mode, IRQ disabled
    STMFD   r0!,{r13,r14}       ; finish saving the context (r0_SYS,r1_SYS)

    MSR     cpsr_c,#(SYS_MODE | NO_IRQ) ; SYSTEM mode, IRQ disabled
; IRQ entry }}}

    ; NOTE: BSP_irq might re-enable IRQ interrupts (the FIQ is enabled
    ; already), if IRQs are prioritized by the interrupt controller.
    ;
    LDR     r12,=BSP_irq
    MOV     lr,pc               ; copy the return address to link register
    BX      r12                 ; call the C IRQ-handler (ARM/THUMB)


; IRQ exit {{{
    MSR     cpsr_c,#(SYS_MODE | NO_INT) ; make sure IRQ/FIQ are disabled
    MOV     r0,sp               ; make sp_SYS visible to IRQ mode
    ADD     sp,sp,#(8*4)        ; fake unstacking 8 registers from sp_SYS

    MSR     cpsr_c,#(IRQ_MODE | NO_INT) ; IRQ mode, both IRQ/FIQ disabled
    MOV     sp,r0               ; copy sp_SYS to sp_IRQ
    LDR     r0,[sp,#(7*4)]      ; load the saved SPSR from the stack
    MSR     spsr_cxsf,r0        ; copy it into spsr_IRQ

    LDMFD   sp,{r0-r3,r12,lr}^  ; unstack all saved USER/SYSTEM registers
    NOP                         ; can't access banked reg immediately
    LDR     lr,[sp,#(6*4)]      ; load return address from the SYS stack
    MOVS    pc,lr               ; return restoring CPSR from SPSR
; IRQ exit }}}

;-----------------------------------------------------------------------------
; FIQ assembly wrapper
;-----------------------------------------------------------------------------

    SECTION .textrw:DATA:NOROOT(2)
    PUBLIC  QF_fiq
    EXTERN  BSP_fiq
    CODE32

QF_fiq:
; FIQ entry {{{
    MOV     r13,r0              ; save r0 in r13_FIQ
    SUB     r0,lr,#4            ; put return address in r0_SYS
    MOV     lr,r1               ; save r1 in r14_FIQ (lr)
    MRS     r1,spsr             ; put the SPSR in r1_SYS

    MSR     cpsr_c,#(SYS_MODE | NO_INT) ; SYSTEM mode, IRQ/FIQ disabled
    STMFD   sp!,{r0,r1}         ; save SPSR and PC on SYS stack
    STMFD   sp!,{r2-r3,r12,lr}  ; save APCS-clobbered regs on SYS stack
    MOV     r0,sp               ; make the sp_SYS visible to FIQ mode
    SUB     sp,sp,#(2*4)        ; make room for stacking (r0_SYS, r1_SYS)

    MSR     cpsr_c,#(FIQ_MODE | NO_INT) ; FIQ mode, IRQ/FIQ disabled
    STMFD   r0!,{r13,r14}       ; finish saving the context (r0_SYS, r1_SYS)

    MSR     cpsr_c,#(SYS_MODE | NO_INT) ; SYSTEM mode, IRQ/FIQ disabled
; FIQ entry }}}


    ; NOTE:
    ; Because FIQ is typically NOT prioritized by the interrupt controller
    ; BSP_fiq must not enable IRQ/FIQ to avoid priority inversions!
    ;
    LDR     r12,=BSP_fiq
    MOV     lr,pc               ; store the return address
    BX      r12                 ; call the C FIQ-handler (ARM/THUMB)


; FIQ exit {{{                  ; both IRQ/FIQ disabled (see NOTE above)
    MSR     cpsr_c,#(SYS_MODE | NO_INT) ; make sure IRQ/FIQ are disabled
    MOV     r0,sp               ; make sp_SYS visible to FIQ mode
    ADD     sp,sp,#(8*4)        ; fake unstacking 8 registers from sp_SYS

    MSR     cpsr_c,#(FIQ_MODE | NO_INT) ; FIQ mode, IRQ/FIQ disabled
    MOV     sp,r0               ; copy sp_SYS to sp_FIQ
    LDR     r0,[sp,#(7*4)]      ; load the saved SPSR from the stack
    MSR     spsr_cxsf,r0        ; copy it into spsr_FIQ

    LDMFD   sp,{r0-r3,r12,lr}^  ; unstack all saved USER/SYSTEM registers
    NOP                         ; can't access banked reg immediately
    LDR     lr,[sp,#(6*4)]      ; load return address from the SYS stack
    MOVS    pc,lr               ; return restoring CPSR from SPSR
; FIQ exit }}}

;-----------------------------------------------------------------------------
; Exception assembler wrappers
;-----------------------------------------------------------------------------

    PUBLIC  QF_reset
    PUBLIC  QF_undef
    PUBLIC  QF_swi
    PUBLIC  QF_pAbort
    PUBLIC  QF_dAbort
    PUBLIC  QF_reserved

    EXTERN  Q_onAssert

    SECTION .text:CODE:NOROOT(2)
    CODE32

QF_reset:
    LDR     r0,=Csting_reset
    B       QF_except
QF_undef:
    LDR     r0,=Csting_undef
    B       QF_except
QF_swi:
    LDR     r0,=Csting_swi
    B       QF_except
QF_pAbort:
    LDR     r0,=Csting_pAbort
    B       QF_except
QF_dAbort:
    LDR     r0,=Csting_dAbort
    B       QF_except
QF_reserved:
    LDR     r0,=Csting_reserved
    B       QF_except
QF_except:
    ; r0 is set to the string with the exception name
    SUB     r1,lr,#4            ; set line number to the exception address
    MSR     cpsr_c,#(SYS_MODE | NO_INT) ; SYSTEM mode, IRQ/FIQ disabled
    LDR     r12,=Q_onAssert
    MOV     lr,pc               ; store the return address
    BX      r12                 ; call the assertion-handler (ARM/THUMB)
    ; the assertion handler should not return, but in case it does
    ; hang up the machine in this endless loop
    B       .


    LTORG  ; strings enclosed in "" are zero-terminated
Csting_reset:       DC8     "Reset"
Csting_undef:       DC8     "Undefined"
Csting_swi:         DC8     "Software Int"
Csting_pAbort:      DC8     "Prefetch Abort"
Csting_dAbort:      DC8     "Data Abort"
Csting_reserved:    DC8     "Reserved"


    END
