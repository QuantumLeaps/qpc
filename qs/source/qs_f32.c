/**
* \file
* \brief QS_f32() implementation
* \ingroup qs
* \cond
******************************************************************************
* Product: QS/C
* Last updated for version 5.3.0
* Last updated on  2014-03-27
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* \endcond
*/
#include "qs_port.h" /* QS port */
#include "qs_pkg.h"

/****************************************************************************/
/**
* \note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_f32(uint8_t format, float32_t f) {
    union F32Rep {
        float32_t f;
        uint32_t  u;
    } fu32;  /* the internal binary representation */
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */
    int_fast8_t i;

    fu32.f = f; /* assign the binary representation */

    QS_priv_.used += (QSCtr)5; /* 5 bytes about to be added */
    QS_INSERT_ESC_BYTE(format) /* insert the format byte */

    /* insert 4 bytes... */
    for (i = (int_fast8_t)4; i != (int_fast8_t)0; --i) {
        format = (uint8_t)fu32.u;
        QS_INSERT_ESC_BYTE(format)
        fu32.u >>= 8;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}
