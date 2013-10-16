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
* \brief QS_f64() implementation
*/

/*..........................................................................*/
void QS_f64(uint8_t format, float64_t d) {
    union F64Rep {
        float64_t d;
        struct UInt2 {
            uint32_t u1;
            uint32_t u2;
        } i;
    } fu64;                           /* the internal binary representation */
    uint8_t chksum = QS_priv_.chksum;
    uint8_t *buf   = QS_priv_.buf;
    QSCtr   head   = QS_priv_.head;
    QSCtr   end    = QS_priv_.end;
    int_t   i;

    fu64.d = d;                         /* assign the binary representation */

    QS_priv_.used += (QSCtr)9;                 /* 9 bytes about to be added */
    QS_INSERT_ESC_BYTE(format)                    /* insert the format byte */

    for (i = (int_t)4; i != (int_t)0; --i) { /* insert 4 bytes of fu64.i.u1 */
        format = (uint8_t)fu64.i.u1;
        QS_INSERT_ESC_BYTE(format)
        fu64.i.u1 >>= 8;
    }
    for (i = (int_t)4; i != (int_t)0; --i) { /* insert 4 bytes of fu64.i.u2 */
        format = (uint8_t)fu64.i.u2;
        QS_INSERT_ESC_BYTE(format)
        fu64.i.u2 >>= 8;
    }

    QS_priv_.head   = head;                                /* save the head */
    QS_priv_.chksum = chksum;                          /* save the checksum */
}
