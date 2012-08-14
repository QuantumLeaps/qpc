/*****************************************************************************
* Product: QDK/C_ARM-GNU_AT91SAM7S-EK
* Last Updated for Version: 4.3.00
* Date of the Last Update:  Nov 08, 2011
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
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

typedef void (*IntVector)(void);           /* IntVector pointer-to-function */

/*..........................................................................*/
__attribute__ ((section (".text.fastcode")))
void BSP_irq(void) {
    IntVector vect = (IntVector)AT91C_BASE_AIC->AIC_IVR;    /* read the IVR */
    AT91C_BASE_AIC->AIC_IVR = (AT91_REG)vect; /* write AIC_IVR if protected */
    asm("MSR cpsr_c,#(0x1F)");                  /* allow nesting interrupts */
    (*vect)();              /* call the IRQ ISR via the pointer to function */
    asm("MSR cpsr_c,#(0x1F | 0x80 | 0x40)");  /* lock IRQ/FIQ before return */
    AT91C_BASE_AIC->AIC_EOICR = 0;    /* write AIC_EOICR to clear interrupt */
}
/*..........................................................................*/
__attribute__ ((section (".text.fastcode")))
void BSP_fiq(void) {
    /* TBD: implement the FIQ handler directly right here, see NOTE01  */
    /* NOTE: Do NOT enable interrupts throughout the whole FIQ processing. */
    /* NOTE: Do NOT write EOI to the AIC */
}
/*..........................................................................*/
__attribute__ ((section (".text.fastcode")))
static void tickIRQ(void) {
    uint32_t tmp = AT91C_BASE_PITC->PITC_PIVR;    /* clear interrupt source */
    QF_TICK(&QS_tickIRQ);
}
/*..........................................................................*/
static void spurIRQ(void) {
}
/*..........................................................................*/
void QF_onStartup(void) {
                            /* hook the exception handlers from the QF port */
    *(uint32_t volatile *)0x24 = (uint32_t)&QF_undef;
    *(uint32_t volatile *)0x28 = (uint32_t)&QF_swi;
    *(uint32_t volatile *)0x2C = (uint32_t)&QF_pAbort;
    *(uint32_t volatile *)0x30 = (uint32_t)&QF_dAbort;
    *(uint32_t volatile *)0x34 = (uint32_t)&QF_reserved;

    *(uint32_t volatile *)0x38 = (uint32_t)&QK_irq;
    *(uint32_t volatile *)0x3C = (uint32_t)&QK_fiq;

    AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (uint32_t)&tickIRQ;
    AT91C_BASE_AIC->AIC_SPU = (uint32_t)&spurIRQ;           /* spurious IRQ */

    AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] =
        (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | AT91C_AIC_PRIOR_LOWEST);
    AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_SYS);
    AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_SYS);

    asm("MSR cpsr_c,#(0x1F)");              /* SYSTEM mode, IRQ/FIQ enabled */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}

/*****************************************************************************
* NOTE01:
* The QK FIQ assembly "wrapper" QF_fiq() calls the FIQ handler BSP_fiq()
* with interrupts locked at the ARM core level. In contrast to the IRQ line,
* the FIQ line is NOT prioritized by the AIC. Therefore, you must NOT enable
* interrupts while processing FIQ. All FIQs should be the highest-priority
* in the system. All FIQs run at the same (highest) priority level.
*****************************************************************************/
