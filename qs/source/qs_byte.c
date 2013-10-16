/*****************************************************************************
* Product:  QS/C
* Last Updated for Version: 5.1.0
* Date of the Last Update:  Sep 18, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qs_pkg.h"

/**
* \file
* \ingroup qs
* \brief QS_getByte() implementation
*/

/*..........................................................................*/
uint16_t QS_getByte(void) {
    uint16_t ret;
    if (QS_priv_.used == (QSCtr)0) {
        ret = QS_EOD;                                    /* set End-Of-Data */
    }
    else {
        uint8_t *buf = QS_priv_.buf;       /* put in a temporary (register) */
        QSCtr tail   = QS_priv_.tail;      /* put in a temporary (register) */
        ret = (uint16_t)(*QS_PTR_AT_(tail));      /* set the byte to return */
        ++tail;                                         /* advance the tail */
        if (tail == QS_priv_.end) {                    /* tail wrap around? */
            tail = (QSCtr)0;
        }
        QS_priv_.tail = tail;                            /* update the tail */
        --QS_priv_.used;                              /* one less byte used */
    }
    return ret;                                   /* return the byte or EOD */
}
