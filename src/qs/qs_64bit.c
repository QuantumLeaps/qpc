/**
* @file
* @brief QS long-long (64-bit) output
* @ingroup qs
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-10-17
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qs_port.h"      /* QS port */

#include "qs_pkg.h"

/****************************************************************************/
/** @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u64_raw_(uint64_t d) {
    uint8_t chksum = QS_priv_.chksum;
    uint8_t *buf   = QS_priv_.buf;
    QSCtr   head   = QS_priv_.head;
    QSCtr   end    = QS_priv_.end;
    int_fast8_t i;

    QS_priv_.used += 8U; /* 8 bytes are about to be added */
    for (i = 8U; i != 0U; --i) {
        uint8_t b = (uint8_t)d;
        QS_INSERT_ESC_BYTE_(b)
        d >>= 8;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}

/****************************************************************************/
/** @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u64_fmt_(uint8_t format, uint64_t d) {
    uint8_t chksum = QS_priv_.chksum;
    uint8_t *buf   = QS_priv_.buf;
    QSCtr   head   = QS_priv_.head;
    QSCtr   end    = QS_priv_.end;
    int_fast8_t i;

    QS_priv_.used += 9U; /* 9 bytes are about to be added */
    QS_INSERT_ESC_BYTE_(format) /* insert the format byte */

    /* output 8 bytes of data... */
    for (i = 8U; i != 0U; --i) {
        format = (uint8_t)d;
        QS_INSERT_ESC_BYTE_(format)
        d >>= 8;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}

