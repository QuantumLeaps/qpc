;*****************************************************************************
; Product: QV port to ARM Cortex-M (M0,M0+,M1,M3,M4,M7), TI-ARM assembler
; Last Updated for Version: 5.9.0
; Date of the Last Update:  2017-03-17
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

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    .global QF_set_BASEPRI    ; set BASEPRI register
  .endif                      ; M3/M4/M7

    .global QF_crit_entry     ; enter critical section
    .global assert_failed     ; low-level assert handler

    .ref    Q_onAssert        ; external reference

    ; NOTE: keep in synch with QF_BASEPRI value defined in "qf_port.h" !!!
QF_BASEPRI: .equ  (0xFF >> 2)

    .text
    .thumb

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

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__ ; M3/4/7?
;*****************************************************************************
; For Cortex-M3/4/7, the QV_init() function sets priorities of all exceptions
; and IRQs to the safe value. All this is performed in a nestable
; critical section.
;*****************************************************************************
    .global QV_init
QV_init:    .asmfunc
    MRS     r0,PRIMASK        ; store the state of the PRIMASK
    MOV     r12,r0            ; r12 := PRIMASK
    CPSID   i                 ; PRIMASK := 1

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
QV_init_irq:
    SUBS    r3,r3,#1
    LDR     r2,PRI0_addr      ; NVIC_PRI0 register
    STR     r1,[r2,r3,LSL #2] ; NVIC_PRI0[r3]  := r1
    CMP     r3,#0
    BNE     QV_init_irq

    MOV     r0,r12            ; r0 := original PRIMASK
    MSR     PRIMASK,r0        ; PRIMASK := r0
    BX      lr                ; return to the caller
  .endasmfunc
  .endif                      ; Cortex-M3/4/7


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
; Addresses for PC-relative LDR
;*****************************************************************************
VTOR_addr:        .word 0xE000ED08
SHPR_addr:        .word 0xE000ED18
ICTR_addr:        .word 0xE000E004 ; Interrupt Controller Type Register
PRI0_addr:        .word 0xE000E400 ; NVIC_PRI0 register
