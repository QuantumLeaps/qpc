/*****************************************************************************
* Product:  QK port to ARM, QK, with GNU toolset
* Last Updated for Version: 4.3.00
* Date of the Last Update:  Nov 06, 2011
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2011 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/

    .equ    NO_INT,      0xC0   /* mask to disable interrupts (FIQ and IRQ) */
    .equ    NO_IRQ,      0x80   /* mask to disable interrupts (FIQ and IRQ) */
    .equ    NO_FIQ,      0x40   /* mask to disable interrupts (FIQ and IRQ) */
    .equ    FIQ_MODE,    0x11
    .equ    IRQ_MODE,    0x12
    .equ    SYS_MODE,    0x1F

    .equ    FIQ_PRIO,    0xFF
    .equ    IRQ_PRIO,    (0xFF - 1)

    .text
    .arm

    /* use the special section (.text.fastcode), to enable fine-tuning
    * of the placement of this section inside the linker script
    */
    .section .text.fastcode


/*****************************************************************************
* unsigned int QF_int_lock_SYS(void);
*/
    .global QF_int_lock_SYS
    .func   QF_int_lock_SYS
QF_int_lock_SYS:
    MRS     r0,cpsr             /* get the original CPSR in r0 to return */
    MSR     cpsr_c,#(SYS_MODE | NO_INT) /* disable both IRQ/FIQ */
    BX      lr                  /* return the original CPSR in r0 */

    .size   QF_int_lock_SYS, . - QF_int_lock_SYS
    .endfunc


/*****************************************************************************
* void QF_int_unlock_SYS(unsigned int key);
*/
    .global QF_int_unlock_SYS
    .func   QF_int_unlock_SYS
QF_int_unlock_SYS:
    MSR     cpsr_c,r0           /* restore the original CPSR from r0 */
    BX      lr                  /* return to ARM or THUMB */

    .size   QF_int_unlock_SYS, . - QF_int_unlock_SYS
    .endfunc


/*****************************************************************************
* void QK_irq(void);
*/
    .global QK_irq
    .func   QK_irq
    .align  3
QK_irq:
/* IRQ entry {{{ */
    MOV     r13,r0              /* save r0 in r13_IRQ */
    SUB     r0,lr,#4            /* put return address in r0_SYS */
    MOV     lr,r1               /* save r1 in r14_IRQ (lr) */
    MRS     r1,spsr             /* put the SPSR in r1_SYS */

    MSR     cpsr_c,#(SYS_MODE | NO_IRQ) /* SYSTEM, no IRQ, but FIQ enabled! */
    STMFD   sp!,{r0,r1}         /* save SPSR and PC on SYS stack */
    STMFD   sp!,{r2-r3,r12,lr}  /* save APCS-clobbered regs on SYS stack */
    MOV     r0,sp               /* make the sp_SYS visible to IRQ mode */
    SUB     sp,sp,#(2*4)        /* make room for stacking (r0_SYS, r1_SYS) */

    MSR     cpsr_c,#(IRQ_MODE | NO_IRQ) /* IRQ mode, IRQ disabled */
    STMFD   r0!,{r13,r14}       /* finish saving the context (r0_SYS,r1_SYS)*/

    MSR     cpsr_c,#(SYS_MODE | NO_INT) /* SYSTEM mode, IRQ/FIQ disabled */
/* IRQ entry }}} */

    LDR     r0,=QK_intNest_     /* load address in already saved r0 */
    LDRB    r12,[r0]            /* load original QK_intNest_ into saved r12 */
    ADD     r12,r12,#1          /* increment the nesting level */
    STRB    r12,[r0]            /* store the value in QK_intNest_ */

    MSR     cpsr_c,#(SYS_MODE | NO_IRQ) /* enable FIQ */

    /* NOTE: BSP_irq might re-enable IRQ interrupts (the FIQ is enabled
    * already), if IRQs are prioritized by the interrupt controller.
    */
    LDR     r12,=BSP_irq
    MOV     lr,pc               /* copy the return address to link register */
    BX      r12                 /* call the C IRQ-handler (ARM/THUMB) */

    MSR     cpsr_c,#(SYS_MODE | NO_INT) /* make sure IRQ/FIQ are disabled */
    LDR     r0,=QK_intNest_     /* load address */
    LDRB    r12,[r0]            /* load original QK_intNest_ into saved r12 */
    SUBS    r12,r12,#1          /* decrement the nesting level */
    STRB    r12,[r0]            /* store the value in QK_intNest_ */
    BNE     QK_irq_exit         /* branch if interrupt nesting not zero */

    LDR     r12,=QK_schedPrio_
    MOV     lr,pc               /* copy the return address to link register */
    BX      r12                 /* call QK_schedPrio_ (ARM/THUMB) */
    CMP     r0,#0               /* check the returned priority */
    BEQ     QK_irq_exit         /* branch if priority zero */

    LDR     r12,=QK_sched_
    MOV     lr,pc               /* copy the return address to link register */
    BX      r12                 /* call QK_sched_ (ARM/THUMB) */

QK_irq_exit:
/* IRQ exit {{{ */              /* IRQ/FIQ disabled--return from scheduler */
    MOV     r0,sp               /* make sp_SYS visible to IRQ mode */
    ADD     sp,sp,#(8*4)        /* fake unstacking 8 registers from sp_SYS */

    MSR     cpsr_c,#(IRQ_MODE | NO_INT) /* IRQ mode, both IRQ/FIQ disabled */
    MOV     sp,r0               /* copy sp_SYS to sp_IRQ */
    LDR     r0,[sp,#(7*4)]      /* load the saved SPSR from the stack */
    MSR     spsr_cxsf,r0        /* copy it into spsr_IRQ */

    LDMFD   sp,{r0-r3,r12,lr}^  /* unstack all saved USER/SYSTEM registers */
    NOP                         /* can't access banked reg immediately */
    LDR     lr,[sp,#(6*4)]      /* load return address from the SYS stack */
    MOVS    pc,lr               /* return restoring CPSR from SPSR */
/* IRQ exit }}} */

    .size   QK_irq, . - QK_irq
    .endfunc


/*****************************************************************************
* void QK_fiq(void);
*/
    .global QK_fiq
    .func   QK_fiq
    .align  3
QK_fiq:
/* FIQ entry {{{ */
    MOV     r13,r0              /* save r0 in r13_FIQ */
    SUB     r0,lr,#4            /* put return address in r0_SYS */
    MOV     lr,r1               /* save r1 in r14_FIQ (lr) */
    MRS     r1,spsr             /* put the SPSR in r1_SYS */

    MSR     cpsr_c,#(SYS_MODE | NO_INT) /* SYSTEM mode, IRQ/FIQ disabled */
    STMFD   sp!,{r0,r1}         /* save SPSR and PC on SYS stack */
    STMFD   sp!,{r2-r3,r12,lr}  /* save APCS-clobbered regs on SYS stack */
    MOV     r0,sp               /* make the sp_SYS visible to FIQ mode */
    SUB     sp,sp,#(2*4)        /* make room for stacking (r0_SYS, SPSR) */

    MSR     cpsr_c,#(FIQ_MODE | NO_INT) /* FIQ mode, IRQ/FIQ disabled */
    STMFD   r0!,{r13,r14}       /* finish saving the context (r0_SYS,r1_SYS)*/

    MSR     cpsr_c,#(SYS_MODE | NO_INT) /* SYSTEM mode, IRQ/FIQ disabled */
/* FIQ entry }}} */

    LDR     r0,=QK_intNest_     /* load address in already saved r0 */
    LDRB    r12,[r0]            /* load original QK_intNest_ into saved r12 */
    ADD     r12,r12,#1          /* increment interrupt nesting */
    STRB    r12,[r0]            /* store the value in QK_intNest_ */


    /* NOTE:
    * Because FIQ is typically NOT prioritized by the interrupt controller
    * BSP_fiq must not enable IRQ/FIQ to avoid priority inversions!
    */
    LDR     r12,=BSP_fiq
    MOV     lr,pc               /* copy the return address to link register */
    BX      r12                 /* call the C FIQ-handler (ARM/THUMB) */


    MSR     cpsr_c,#(SYS_MODE | NO_INT) /* make sure IRQ/FIQ are disabled */
    LDR     r0,=QK_intNest_     /* load address */
    LDRB    r12,[r0]            /* load original QK_intNest_ into saved r12 */
    SUBS    r12,r12,#1          /* decrement the nesting level */
    STRB    r12,[r0]            /* store the value in QK_intNest_ */
    BNE     QK_fiq_exit         /* branch if interrupt nesting not zero */

    LDR     r0,[sp,#(7*4)]      /* load the saved SPSR from the stack */
    AND     r0,r0,#0x1F         /* isolate the SPSR mode bits in r0 */
    CMP     r0,#IRQ_MODE        /* see if we interrupted IRQ mode */
    BEQ     QK_fiq_exit         /* branch if interrutped IRQ */

    /* We have interrupted a task. Call QK scheduler to handle preemptions */
    LDR     r12,=QK_schedPrio_
    MOV     lr,pc               /* copy the return address to link register */
    BX      r12                 /* call QK_schedPrio_ (ARM/THUMB) */
    CMP     r0,#0               /* check the returned priority */
    BEQ     QK_fiq_exit         /* branch if priority zero */

    LDR     r12,=QK_sched_
    MOV     lr,pc               /* copy the return address to link register */
    BX      r12                 /* call QK_sched_ (ARM/THUMB) */

QK_fiq_exit:
/* FIQ exit {{{ */              /* both IRQ/FIQ disabled (see NOTE above) */
    MOV     r0,sp               /* make sp_SYS visible to FIQ mode */
    ADD     sp,sp,#(8*4)        /* fake unstacking 8 registers from sp_SYS */

    MSR     cpsr_c,#(FIQ_MODE | NO_INT) /* FIQ mode, IRQ/FIQ disabled */
    MOV     sp,r0               /* copy sp_SYS to sp_FIQ */
    LDR     r0,[sp,#(7*4)]      /* load the saved SPSR from the stack */
    MSR     spsr_cxsf,r0        /* copy it into spsr_FIQ */

    LDMFD   sp,{r0-r3,r12,lr}^  /* unstack all saved USER/SYSTEM registers */
    NOP                         /* can't access banked reg immediately */
    LDR     lr,[sp,#(6*4)]      /* load return address from the SYS stack */
    MOVS    pc,lr               /* return restoring CPSR from SPSR */
/* FIQ exit }}} */

    .size   QK_fiq, . - QK_fiq
    .endfunc


/*****************************************************************************
* void QF_reset(void);
*/
    .global QF_reset
    .func   QF_reset
    .align  3
QF_reset:
    LDR     r0,=Csting_reset
    B       QF_except
    .size   QF_reset, . - QF_reset
    .endfunc

/*****************************************************************************
* void QF_undef(void);
*/
    .global QF_undef
    .func   QF_undef
    .align  3
QF_undef:
    LDR     r0,=Csting_undef
    B       QF_except
    .size   QF_undef, . - QF_undef
    .endfunc

/*****************************************************************************
* void QF_swi(void);
*/
    .global QF_swi
    .func   QF_swi
    .align  3
QF_swi:
    LDR     r0,=Csting_swi
    B       QF_except
    .size   QF_swi, . - QF_swi
    .endfunc

/*****************************************************************************
* void QF_pAbort(void);
*/
    .global QF_pAbort
    .func   QF_pAbort
    .align  3
QF_pAbort:
    LDR     r0,=Csting_pAbort
    B       QF_except
    .size   QF_pAbort, . - QF_pAbort
    .endfunc

/*****************************************************************************
* void QF_dAbort(void);
*/
    .global QF_dAbort
    .func   QF_dAbort
    .align  3
QF_dAbort:
    LDR     r0,=Csting_dAbort
    B       QF_except
    .size   QF_dAbort, . - QF_dAbort
    .endfunc

/*****************************************************************************
* void QF_reserved(void);
*/
    .global QF_reserved
    .func   QF_reserved
    .align  3
QF_reserved:
    LDR     r0,=Csting_reserved
    B       QF_except
    .size   QF_reserved, . - QF_reserved
    .endfunc

/*****************************************************************************
* void QF_except(void);
*/
    .global QF_except
    .func   QF_except
    .align  3
QF_except:
    /* r0 is set to the string with the exception name */
    SUB     r1,lr,#4            /* set line number to the exception address */
    MSR     cpsr_c,#(SYS_MODE | NO_INT) /* SYSTEM mode, IRQ/FIQ disabled */
    LDR     r12,=Q_onAssert
    MOV     lr,pc               /* store the return address */
    BX      r12                 /* call the assertion-handler (ARM/THUMB) */
    /* the assertion handler should not return, but in case it does
    * hang up the machine in this endless loop
    */
    B       .

Csting_reset:       .string  "Reset"
Csting_undef:       .string  "Undefined"
Csting_swi:         .string  "Software Int"
Csting_pAbort:      .string  "Prefetch Abort"
Csting_dAbort:      .string  "Data Abort"
Csting_reserved:    .string  "Reserved"

    .size   QF_except, . - QF_except
    .endfunc

    .end


