/**
* \file
* \brief QS_getByte() implementation
* \ingroup qs
* \cond
******************************************************************************
* Product: QS/C
* Last updated for version 5.3.0
* Last updated on  2014-04-09
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
* \description
* This function delivers one byte at a time from the QS data buffer.
*
* \returns the byte in the least-significant 8-bits of the 16-bit return
* value if the byte is available. If no more data is available at the time,
* the function returns ::QS_EOD (End-Of-Data).
*
* \note QS_getByte() is __not__ protected with a critical section.
*/
uint16_t QS_getByte(void) {
    uint16_t ret;
    if (QS_priv_.used == (QSCtr)0) {
        ret = QS_EOD; /* set End-Of-Data */
    }
    else {
        uint8_t *buf = QS_priv_.buf;  /* put in a temporary (register) */
        QSCtr tail   = QS_priv_.tail; /* put in a temporary (register) */
        ret = (uint16_t)(*QS_PTR_AT_(tail)); /* set the byte to return */
        ++tail; /* advance the tail */
        if (tail == QS_priv_.end) { /* tail wrap around? */
            tail = (QSCtr)0;
        }
        QS_priv_.tail = tail; /* update the tail */
        --QS_priv_.used;      /* one less byte used */
    }
    return ret; /* return the byte or EOD */
}
