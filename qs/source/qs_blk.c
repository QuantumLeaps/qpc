/**
* \file
* \brief QS_getBlock() implementation
* \ingroup qs
* \cond
******************************************************************************
* Product: QS/C
* Last updated for version 5.3.0
* Last updated on  2014-03-01
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
* This function delivers a contiguous block of data from the QS data buffer.
* The function returns the pointer to the beginning of the block, and writes
* the number of bytes in the block to the location pointed to by \a pNbytes.
* The argument \a pNbytes is also used as input to provide the maximum size of
* the data block that the caller can accept.
*
* \returns if data is available, the function returns pointer to the
* contiguous block of data and sets the value pointed to by \c pNbytes
* to the # available bytes. If data is available at the time the function is
* called, the function returns NULL pointer and sets the value pointed to by
* \c pNbytes to zero.
*
* \note Only the NULL return from QS_getBlock() indicates that the QS buffer
* is empty at the time of the call. The non-NULL return often means that
* the block is at the end of the buffer and you need to call QS_getBlock()
* again to obtain the rest of the data that "wrapped around" to the beginning
* of the QS data buffer.
*
* \note QS_getBlock() is NOT protected with a critical section.
*/
uint8_t const *QS_getBlock(uint16_t *pNbytes) {
    QSCtr used = QS_priv_.used; /* put in a temporary (register) */
    uint8_t *buf;

    /* any bytes used in the ring buffer? */
    if (used != (QSCtr)0) {
        QSCtr tail = QS_priv_.tail;  /* put in a temporary (register) */
        QSCtr end  = QS_priv_.end;   /* put in a temporary (register) */
        QSCtr n = (QSCtr)(end - tail);
        if (n > used) {
            n = used;
        }
        if (n > (QSCtr)(*pNbytes)) {
            n = (QSCtr)(*pNbytes);
        }
        *pNbytes = (uint16_t)n;      /* n-bytes available */
        buf = QS_priv_.buf;
        buf = QS_PTR_AT_(tail);      /* the bytes are at the tail */

        QS_priv_.used = (QSCtr)(used - n);
        tail += n;
        if (tail == end) {
            tail = (QSCtr)0;
        }
        QS_priv_.tail = tail;
    }

    else { /* no bytes available */
        *pNbytes = (uint16_t)0;  /* no bytes available right now */
        buf      = (uint8_t *)0; /* no bytes available right now */
    }
    return buf;
}
