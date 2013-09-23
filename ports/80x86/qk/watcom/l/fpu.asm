;*****************************************************************************
; Purpose: FPU_save() and FPU_restore() implementation for x86/x87
; Last Updated for Version: 4.0.00
; Date of the Last Update:  Feb 18, 2008
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
;*****************************************************************************

            PUBLIC _FPU_save
            PUBLIC _FPU_restore

.MODEL      LARGE
.CODE
.386

;*****************************************************************************
; void FPU_save(FPU_context *ctx);

_FPU_save    PROC FAR
             PUSH   BP                      ; Save work registers
             MOV    BP,SP
             PUSH   ES
             PUSH   BX
;
             LES    BX, DWORD PTR [BP+6]    ; Point to FPU context memory
;
             FSAVE  ES:[BX]                 ; Save FPU context
;
             POP    BX                      ; Restore work registers
             POP    ES
             POP    BP
;
             RET                            ; Return to caller
_FPU_save    ENDP

;*****************************************************************************
; void FPU_restore(FPU_context *ctx);

_FPU_restore PROC FAR
             PUSH   BP                      ; Save work registers
             MOV    BP,SP
             PUSH   ES
             PUSH   BX
;
             LES    BX, DWORD PTR [BP+6]    ; Point to FPU context memory
;
             FRSTOR ES:[BX]                 ; Restore FPU context
;
             POP    BX                      ; Restore work registers
             POP    ES
             POP    BP
;
             RET                            ; Return to caller
_FPU_restore ENDP

             END
