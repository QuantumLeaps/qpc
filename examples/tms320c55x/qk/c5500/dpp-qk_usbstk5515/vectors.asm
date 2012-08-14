;****************************************************************************
; Interrupt Vector Table
;
; 32 interrupt vector addresses.  Each is 64 bits long.  The first 32 bits
; contains the 24-bit program address of the Interrupt Service Routine (ISR).
; The second 32 bits are executed before transferring control to the ISR.
;****************************************************************************

    .sect   "vectors"
    .align  256
    .global _VECSTART

;****************************************************************************
;* Point Reset Vector to C Environment Entry Point
;****************************************************************************
    .def    _Reset
    .ref    _c_int00
    .ref    _illegal_isr

_VECSTART:

    .if __TMS320C55X_PLUS_BYTE__
_Reset:    .ivec _c_int00, STK_LINEAR | RET_FAST | DATA_PTR_BYTE
    .else
_Reset:    .ivec _c_int00, USE_RETA
    .endif

;****************************************************************************
;* Other interrupt vector definitions go here
;****************************************************************************
nmi:      .ivec _illegal_isr  ; Non-maskable hardware interrupt
int02:    .ivec _illegal_isr
int03:    .ivec _illegal_isr
int04:    .ivec _illegal_isr
int05:    .ivec _illegal_isr
int06:    .ivec _illegal_isr
int07:    .ivec _illegal_isr
int08:    .ivec _illegal_isr
int09:    .ivec _illegal_isr
int10:    .ivec _illegal_isr
int11:    .ivec _illegal_isr
int12:    .ivec _illegal_isr
int13:    .ivec _illegal_isr
int14:    .ivec _illegal_isr
int15:    .ivec _illegal_isr
int16:    .ivec _illegal_isr
int17:    .ivec _illegal_isr
int18:    .ivec _illegal_isr
int19:    .ivec _illegal_isr
int20:    .ivec _illegal_isr
int21:    .ivec _illegal_isr
int22:    .ivec _illegal_isr
int23:    .ivec _illegal_isr
int24:    .ivec _illegal_isr  ; Bus error interrupt
int25:    .ivec _illegal_isr  ; Data log interrupt
int26:    .ivec _illegal_isr  ; Real-time OS interrupt
int27:    .ivec _illegal_isr  ; General-purpose software-only interrupt
int28:    .ivec _illegal_isr  ; General-purpose software-only interrupt
int29:    .ivec _illegal_isr  ; General-purpose software-only interrupt
int30:    .ivec _illegal_isr  ; General-purpose software-only interrupt
int31:    .ivec _illegal_isr  ; General-purpose software-only interrupt

    .end
