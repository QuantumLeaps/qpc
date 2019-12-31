;-----------------------------------------------------------------------------
; Product:  QK port to ARM7/9, IAR-ARM Assembler
; Last Updated for Version: 6.3.6
; Date of the Last Update:  2018-10-29
;
;                    Q u a n t u m  L e a P s
;                    ------------------------
;                    Modern Embedded Software
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
; <www.state-machine.com/licensing>
; <info@state-machine.com>
;-----------------------------------------------------------------------------

NO_IRQ      DEFINE     0x80     ; mask to disable interrupts (FIQ and IRQ)
NO_FIQ      DEFINE     0x40     ; mask to disable interrupts (FIQ and IRQ)
FIQ_MODE    DEFINE     0x11
IRQ_MODE    DEFINE     0x12
SYS_MODE    DEFINE     0x1F

    ; NOTE: keep in synch with the QK_Attr struct in "qk.h" !!!
QK_INT_NEST DEFINE     4

;-----------------------------------------------------------------------------
; Interrupt disabling/enabling and QK initialization
;-----------------------------------------------------------------------------

    SECTION .textrw:DATA:NOROOT(2)
    PUBLIC  QF_int_disable_SYS, QF_int_enable_SYS, QK_init
    CODE32

QF_int_disable_SYS:
    MRS     r0,cpsr             ; get the original CPSR in r0 to return
    MSR     cpsr_c,#(SYS_MODE | NO_IRQ) ; disable IRQ only, FIQ enabled!
    BX      lr                  ; return the original CPSR in r0


QF_int_enable_SYS:
    MSR     cpsr_c,r0           ; restore the original CPSR from r0
    BX      lr                  ; return to ARM or THUMB


QK_init:
    BX      lr                  ; return to ARM or THUMB


;-----------------------------------------------------------------------------
; IRQ assembly wrapper
;-----------------------------------------------------------------------------

    SECTION .textrw:DATA:NOROOT(2)
    PUBLIC  QK_irq
    EXTERN  BSP_irq
    EXTERN  QK_attr_, QK_sched_, QK_activate_
    CODE32

QK_irq:
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

    LDR     r0,=QK_attr_        ; load address in already saved r0
    LDRB    r12,[r0,#QK_INT_NEST] ; load QK_attr_.intNest into saved r12
    ADD     r12,r12,#1          ; increment the nesting level
    STRB    r12,[r0,#QK_INT_NEST] ; store the value in QK_attr_.intNest

    ;     MSR     cpsr_c,#(SYS_MODE | NO_IRQ) ; enable FIQ
    ; NOTE: BSP_irq might re-enable IRQ interrupts (the FIQ is enabled
    ; already), if IRQs are prioritized by the interrupt controller.

    LDR     r12,=BSP_irq
    MOV     lr,pc               ; copy the return address to link register
    BX      r12                 ; call the C IRQ-handler (ARM/THUMB)


    MSR     cpsr_c,#(SYS_MODE | NO_IRQ) ; make sure IRQs are disabled
    LDR     r0,=QK_attr_        ; load address
    LDRB    r12,[r0,#QK_INT_NEST] ; load QK_attr_.intNest into saved r12
    SUBS    r12,r12,#1          ; decrement the nesting level
    STRB    r12,[r0,#QK_INT_NEST] ; store the value in QK_attr_.intNest
    BNE     QK_irq_exit         ; branch if interrupt nesting not zero

    LDR     r12,=QK_sched_
    MOV     lr,pc               ; copy the return address to link register
    BX      r12                 ; call QK_sched_ (ARM/THUMB)
    CMP     r0,#0               ; check the returned priority
    BEQ     QK_irq_exit         ; branch if priority zero

    LDR     r12,=QK_activate_
    MOV     lr,pc               ; copy the return address to link register
    BX      r12                 ; call QK_activate_ (ARM/THUMB)

QK_irq_exit:
; IRQ exit {{{                  ; IRQ/FIQ disabled--return from scheduler
    MOV     r0,sp               ; make sp_SYS visible to IRQ mode
    ADD     sp,sp,#(8*4)        ; fake unstacking 8 registers from sp_SYS

    MSR     cpsr_c,#(IRQ_MODE | NO_IRQ) ; IRQ mode, IRQ disabled
    MOV     sp,r0               ; copy sp_SYS to sp_IRQ
    LDR     r0,[sp,#(7*4)]      ; load the saved SPSR from the stack
    MSR     spsr_cxsf,r0        ; copy it into spsr_IRQ

    LDMFD   sp,{r0-r3,r12,lr}^  ; unstack all saved USER/SYSTEM registers
    NOP                         ; can't access banked reg immediately
    LDR     lr,[sp,#(6*4)]      ; load return address from the SYS stack
    MOVS    pc,lr               ; return restoring CPSR from SPSR
; IRQ exit }}}


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
    MSR     cpsr_c,#(SYS_MODE | NO_IRQ | NO_FIQ) ; SYSTEM, IRQ/FIQ disabled
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
