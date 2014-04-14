/**
* \file
* \brief QS_mem() implementation
* \ingroup qs
* \cond
******************************************************************************
* Product: QS/C
* Last updated for version 5.3.0
* Last updated on  2014-03-07
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
/** \note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_mem(uint8_t const *blk, uint8_t size) {
    uint8_t b      = (uint8_t)(QS_MEM_T);
    uint8_t chksum = (uint8_t)(QS_priv_.chksum + b);
    uint8_t *buf   = QS_priv_.buf;  /* put in a temporary (register) */
    QSCtr   head   = QS_priv_.head; /* put in a temporary (register) */
    QSCtr   end    = QS_priv_.end;  /* put in a temporary (register) */

    QS_priv_.used += ((QSCtr)size + (QSCtr)2); /* size+2 bytes to be added */

    QS_INSERT_BYTE(b)
    QS_INSERT_ESC_BYTE(size)

    /* output the 'size' number of bytes */
    while (size != (uint8_t)0) {
        b = *blk;
        QS_INSERT_ESC_BYTE(b)
        QS_PTR_INC_(blk);
        --size;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}
