/*****************************************************************************
* Product:  QF/C, port to TMS320C55x, TI C5500 compiler
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Feb 08, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
#include "qf_pkg.h"
#include "qassert.h"

/*Q_DEFINE_THIS_MODULE("qf_port")*/

/* Global objects ----------------------------------------------------------*/
QSignal      QF_maxSignal_;
QSubscrList *QF_subscrList_;

/*..........................................................................*/
void bzero(uint8_t *ptr, uint16_t len) {
    while (len-- != (uint16_t)0) {
        *ptr++ = (uint8_t)0;
    }
}
/*..........................................................................*/
void QF_zero(void) {                                          /* see NOTE01 */
    extern uint8_t QF_intLockNest_;

    QF_intLockNest_     = (uint8_t)0;
    QF_maxPool_         = (uint8_t)0;
    QF_timeEvtListHead_ = (QTimeEvt *)0;
    bzero((uint8_t *)QF_active_, sizeof(QActive*)*(QF_MAX_ACTIVE + 1));
}
/*..........................................................................*/
void QF_psInit(QSubscrList *subscrSto, QSignal maxSignal) {   /* see NOTE01 */
    QF_subscrList_ = subscrSto;
    QF_maxSignal_  = maxSignal;
    bzero((uint8_t *)subscrSto, maxSignal*sizeof(QSubscrList));
}

/*****************************************************************************
* NOTE01:
* The standard TI startup code (c_int00) does NOT zero the uninitialized
* variables, as required by the C-standard. Since QF relies on the clearing
* of the static uninitialized variables, the critical QF objects are cleared
* explicitly in this port.
*/
