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
#include "qs_pkg.h"

/**
* \file
* \ingroup qs
* \brief QS_getBlock() implementation
*/

/*..........................................................................*/
/* get up to *pNbytes bytes of contiguous memory */
uint8_t const *QS_getBlock(uint16_t *pNbytes) {
    QSCtr used = QS_priv_.used;            /* put in a temporary (register) */
    uint8_t *buf;
    if (used == (QSCtr)0) {
        *pNbytes = (uint16_t)0;             /* no bytes available right now */
        buf      = (uint8_t *)0;            /* no bytes available right now */
    }
    else {
        QSCtr tail = QS_priv_.tail;        /* put in a temporary (register) */
        QSCtr end  = QS_priv_.end;         /* put in a temporary (register) */
        QSCtr n = (QSCtr)(end - tail);
        if (n > used) {
            n = used;
        }
        if (n > (QSCtr)(*pNbytes)) {
            n = (QSCtr)(*pNbytes);
        }
        *pNbytes = (uint16_t)n;                        /* n-bytes available */
        buf = QS_priv_.buf;
        buf = QS_PTR_AT_(tail);                /* the bytes are at the tail */

        QS_priv_.used = (QSCtr)(used - n);
        tail += n;
        if (tail == end) {
            tail = (QSCtr)0;
        }
        QS_priv_.tail = tail;
    }
    return buf;
}
