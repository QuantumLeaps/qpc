/*****************************************************************************
* Product:  QK/C, port to TMS320C55x, TI C5500 compiler
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
* terms of Quantum Leaps commercial licenses, which expressly allow the
* licensees to retain the proprietary status of their code. The licensees
* who use this software under one of Quantum Leaps commercial licenses do
* not use this software under the GPL and therefore are not subject to any
* of its terms.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qk_pkg.h"

/*..........................................................................*/
char_t const Q_ROM *QK_getPortVersion(void) {
    static char_t const Q_ROM version[] = "4.4.00";
    return version;
}
/*..........................................................................*/
void QK_init(void) { /* explicitly clear uninitialized QF variables, NOTE01 */
    QK_intNest_  = (uint8_t)0;
    QK_currPrio_ = (uint8_t)(QF_MAX_ACTIVE + 1);
    bzero((uint8_t *)&QK_readySet_, sizeof(QK_readySet_));

#ifndef QK_NO_MUTEX
    QK_ceilingPrio_ = (uint8_t)0;
#endif
}

/*****************************************************************************
* NOTE01:
* The standard TI startup code (c_int00) does NOT zero the uninitialized
* variables, as required by the C-standard. Since QF relies on the clearing
* of the static uninitialized variables, the critical QF objects are cleared
* explicitly in this port.
*/
