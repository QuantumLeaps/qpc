/**
* @file
* @brief QS floating point output implementation
* @ingroup qs
* @cond
******************************************************************************
* Last updated for version 6.9.2
* Last updated on  2021-01-13
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2021 Quantum Leaps, LLC. All rights reserved.
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
#include "qs_pkg.h"       /* QS package-scope internal interface */

/****************************************************************************/
/**
* @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_f32_fmt_(uint8_t format, float32_t f) {
    union F32Rep {
        float32_t f;
        uint32_t  u;
    } fu32;  /* the internal binary representation */
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */
    uint_fast8_t i;

    fu32.f = f; /* assign the binary representation */

    QS_priv_.used += 5U; /* 5 bytes about to be added */
    QS_INSERT_ESC_BYTE_(format) /* insert the format byte */

    /* insert 4 bytes... */
    for (i = 4U; i != 0U; --i) {
        QS_INSERT_ESC_BYTE_((uint8_t)fu32.u)
        fu32.u >>= 8;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}

/****************************************************************************/
/**
* @description
* This function is only to be used through macros, never in the
* client code directly.
*/
void QS_f64_fmt_(uint8_t format, float64_t d) {
    union F64Rep {
        float64_t d;
        uint32_t  u[2];
    } fu64; /* the internal binary representation */
    uint8_t chksum = QS_priv_.chksum;
    uint8_t *buf   = QS_priv_.buf;
    QSCtr   head   = QS_priv_.head;
    QSCtr   end    = QS_priv_.end;
    uint32_t i;

    /* static constant untion to detect endianness of the machine */
    static union U32Rep {
        uint32_t u32;
        uint8_t  u8;
    } const endian = { 1U };

    fu64.d = d; /* assign the binary representation */

    /* is this a big-endian machine? */
    if (endian.u8 == 0U) {
        /* swap fu64.u[0] <-> fu64.u[1]... */
        i = fu64.u[0];
        fu64.u[0] = fu64.u[1];
        fu64.u[1] = i;
    }

    QS_priv_.used += 9U; /* 9 bytes about to be added */
    QS_INSERT_ESC_BYTE_(format) /* insert the format byte */

    /* output 4 bytes from fu64.u[0]... */
    for (i = 4U; i != 0U; --i) {
        QS_INSERT_ESC_BYTE_((uint8_t)fu64.u[0])
        fu64.u[0] >>= 8;
    }

    /* output 4 bytes from fu64.u[1]... */
    for (i = 4U; i != 0U; --i) {
        QS_INSERT_ESC_BYTE_((uint8_t)fu64.u[1])
        fu64.u[1] >>= 8;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}

