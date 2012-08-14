/*****************************************************************************
* Interrupt Service Routines, QDPP example, Vanilla port
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Aug 11, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
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

Q_DEFINE_THIS_FILE

/*..........................................................................*/
void QF_onStartup(void) {
    TR2 = 1;                                                /* start Timer2 */
    ET2 = 1;                                     /* enable Timer2 interrupt */
    EA  = 1;                                /* enable global interrupt flag */
}
/*..........................................................................*/
void timer2_ISR(void) interrupt 5 {         /* interrupt vector at FF:002BH */

    TF2 = 0;    /* clear Timer2 overflow flag (must be cleared by software) */

#ifdef Q_SPY
    QS_tickTime += (QSTimeCtr)(BSP_PERIPHERAL_HZ/BSP_TICKS_PER_SEC + 0.5);
#endif

    QF_tick();                    /* QF processing of the system clock-tick */

#ifdef NDEBUG
    QF_pcon = PCON;   /* prevent low-power mode upon ISR return, see NOTE01 */
#endif
}
/*..........................................................................*/
/* place other ISRs here... */

