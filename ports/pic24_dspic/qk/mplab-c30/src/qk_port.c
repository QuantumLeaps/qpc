/*****************************************************************************
* Product:  QK/C port, PIC/dsPIC, QK, MPLAB-C30 compiler
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Feb 03, 2012
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
void QK_init(void) {
}
/*..........................................................................*/
/*lint -e970 -e971            ignore MISRA rules 13 and 14 in this function */
char const Q_ROM *QK_getPortVersion(void) {
    static char const Q_ROM version[] = "4.4.00";
    return version;
}
