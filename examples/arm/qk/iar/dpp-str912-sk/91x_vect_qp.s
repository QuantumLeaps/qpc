;-----------------------------------------------------------------------------
; Product: startup code for the STR91x, IAR C/C++ compiler, with QK
; Date of the Last Update:  Jun 28, 2008
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

    SECTION .intvec:CODE:ROOT(2)

    PUBLIC  __vector
    IMPORT  __iar_program_start

    IMPORT  QF_undef
    IMPORT  QF_swi
    IMPORT  QF_pAbort
    IMPORT  QF_dAbort
    IMPORT  QF_reserved
    IMPORT  QK_irq
    IMPORT  QK_fiq


    CODE32
__vector:
    LDR     PC,=__iar_program_start
    LDR     PC,=QF_undef
    LDR     PC,=QF_swi
    LDR     PC,=QF_pAbort
    LDR     PC,=QF_dAbort
    LDR     PC,=QF_reserved
    LDR     PC,=QK_irq
    LDR     PC,=QK_fiq

    LTORG

    END
