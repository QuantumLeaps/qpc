/*****************************************************************************
* Product: QDK/C_ARM-GNU_AT91SAM7S-EK, QK kernel
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Nov 09, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

#include <AT91SAM7S64.H>                        /* AT91SAMT7S64 definitions */

typedef void (*IntVector)(void);           /* IntVector pointer-to-function */

#ifdef Q_SPY
    static uint8_t const QS_tickIRQ = 0;
#endif

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
    *(uint32_t volatile *)0x3C = (uint32_t)&QF_fiq_dummy; /* unimplemented! */

    AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (uint32_t)&tickIRQ;
    AT91C_BASE_AIC->AIC_SPU = (uint32_t)&spurIRQ;           /* spurious IRQ */

    AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] =
        (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | AT91C_AIC_PRIOR_LOWEST);
    AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_SYS);
    AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_SYS);

    QS_OBJ_DICTIONARY(&QS_tickIRQ);

    asm("MSR cpsr_c,#(0x1F)");              /* SYSTEM mode, IRQ/FIQ enabled */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
