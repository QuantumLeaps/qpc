/*****************************************************************************
* Product: QK port to ARM7-9, GNU-ARM assembler
* Last Updated for Version: 6.3.6
* Date of the Last Update:  2018-10-29
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/

    .equ    NO_IRQ,      0x80   /* mask to disable interrupts (IRQ) */
    .equ    NO_FIQ,      0x40   /* mask to disable interrupts (FIQ) */
    .equ    FIQ_MODE,    0x11
    .equ    IRQ_MODE,    0x12
    .equ    SYS_MODE,    0x1F

    /* NOTE: keep in synch with the QK_Attr struct in "qk.h" !!! */
    .equ    QK_INT_NEST, 4

    .text
    .arm

    /* use the special section (.text.fastcode), to enable fine-tuning
    * of the placement of this section inside the linker script
    */
    .section .text.fastcode

/*****************************************************************************
* unsigned int QF_int_disable_SYS(void);
*/
    .global QF_int_disable_SYS
    .func   QF_int_disable_SYS
QF_int_disable_SYS:
    MRS     r0,cpsr             /* get the original CPSR in r0 to return  */
    MSR     cpsr_c,#(SYS_MODE | NO_IRQ) /* disable IRQ only, FIQ enabled! */
    BX      lr                  /* return the original CPSR in r0         */

    .size   QF_int_disable_SYS, . - QF_int_disable_SYS
    .endfunc


/*****************************************************************************
* void QF_int_enable_SYS(unsigned int key);
*/
    .global QF_int_enable_SYS
    .func   QF_int_enable_SYS
QF_int_enable_SYS:
    MSR     cpsr_c,r0           /* restore the original CPSR from r0 */
    BX      lr                  /* return to ARM or THUMB */

    .size   QF_int_enable_SYS, . - QF_int_enable_SYS
    .endfunc


/*****************************************************************************
* void QK_init();
*/
    .global QK_init
    .func   QK_init
QK_init:
    BX      lr                  /* return to ARM or THUMB */

    .size   QK_init, . - QK_init
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

    MSR     cpsr_c,#(SYS_MODE | NO_IRQ) /* SYSTEM mode, IRQ disabled */
/* IRQ entry }}} */

    LDR     r0,=QK_attr_        /* load address in already saved r0 */
    LDRB    r12,[r0,#QK_INT_NEST] /* load QK_attr_.intNest into saved r12 */
    ADD     r12,r12,#1          /* increment the nesting level */
    STRB    r12,[r0,#QK_INT_NEST] /* store the value in QK_attr_.intNest */

    /*     MSR     cpsr_c,#(SYS_MODE | NO_IRQ) ; enable FIQ
    * NOTE: BSP_irq might re-enable IRQ interrupts (the FIQ is enabled
    * already), if IRQs are prioritized by the interrupt controller.
    */
    LDR     r12,=BSP_irq
    MOV     lr,pc               /* copy the return address to link register */
    BX      r12                 /* call the C IRQ-handler (ARM/THUMB) */

    MSR     cpsr_c,#(SYS_MODE | NO_IRQ) /* make sure IRQs are disabled */
    LDR     r0,=QK_attr_        /* load address */
    LDRB    r12,[r0,#QK_INT_NEST] /* load QK_attr_.intNest into saved r12 */
    SUBS    r12,r12,#1          /* decrement the nesting level */
    STRB    r12,[r0,#QK_INT_NEST] /* store the value in QK_attr_.intNest */
    BNE     QK_irq_exit         /* branch if interrupt nesting not zero */

    LDR     r12,=QK_sched_
    MOV     lr,pc               /* copy the return address to link register */
    BX      r12                 /* call QK_sched_ (ARM/THUMB) */
    CMP     r0,#0               /* check the returned priority */
    BEQ     QK_irq_exit         /* branch if priority zero */

    LDR     r12,=QK_activate_
    MOV     lr,pc               /* copy the return address to link register */
    BX      r12                 /* call QK_activate_ (ARM/THUMB) */

QK_irq_exit:
/* IRQ exit {{{ */              /* IRQ/FIQ disabled--return from scheduler */
    MOV     r0,sp               /* make sp_SYS visible to IRQ mode */
    ADD     sp,sp,#(8*4)        /* fake unstacking 8 registers from sp_SYS */

    MSR     cpsr_c,#(IRQ_MODE | NO_IRQ) /* IRQ mode, IRQ disabled */
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
* void QF_fiq_dummy(void);
*/
    .global QF_fiq_dummy
    .func   QF_fiq_dummy
    .align  3
QF_fiq_dummy:
    LDR     r0,=Csting_fiq
    B       QF_except
    .size   QF_fiq_dummy, . - QF_fiq_dummy
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
    MSR     cpsr_c,#(SYS_MODE | NO_IRQ | NO_FIQ) /* SYSTEM,IRQ/FIQ disabled */
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
Csting_fiq:         .string  "FIQ dummy"

    .size   QF_except, . - QF_except
    .endfunc

    .end


