/*============================================================================
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of open-source GPL 3.0
* (or any later version), or alternatively, under the terms of one of the
* closed-source Quantum Leaps commercial licenses.
*
* The terms of the open source GPL 3.0 license can be found at:
* <www.gnu.org/licenses/gpl-3.0.txt>
*
* The terms of the closed-source Quantum Leaps commercial licenses
* can be found at:
* <www.state-machine.com/licensing>
*
* NOTE: Please do NOT plagiarize this software to sidestep the license
* obligations. This is both unfair and illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/**
* @date Last updated on: 2021-12-23
* @version Last updated for: @ref qpc_7_0_0
*
* @file
* @brief QS floating point output implementation
* @ingroup qs
*/
#define QP_IMPL           /* this is QP implementation */
#include "qs_port.h"      /* QS port */
#include "qs_pkg.h"       /* QS package-scope internal interface */

/*==========================================================================*/

/**
* @static @private @memberof QS
* @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_f32_fmt_(uint8_t format, float32_t f) {
    union F32Rep {
        float32_t f;
        uint32_t  u;
    } fu32;  /* the internal binary representation */
    uint8_t chksum      = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t * const buf = QS_priv_.buf;
    QSCtr head          = QS_priv_.head;
    QSCtr const end     = QS_priv_.end;
    uint_fast8_t i;

    fu32.f = f; /* assign the binary representation */

    QS_priv_.used += 5U; /* 5 bytes about to be added */
    QS_INSERT_ESC_BYTE_(format) /* insert the format byte */

    /* insert 4 bytes... */
    for (i = 4U; i != 0U; --i) {
        QS_INSERT_ESC_BYTE_((uint8_t)fu32.u)
        fu32.u >>= 8U;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}

/*==========================================================================*/

/**
* @static @private @memberof QS
* @description
* This function is only to be used through macros, never in the
* client code directly.
*/
void QS_f64_fmt_(uint8_t format, float64_t d) {
    union F64Rep {
        float64_t d;
        uint32_t  u[2];
    } fu64; /* the internal binary representation */
    uint8_t chksum      = QS_priv_.chksum;
    uint8_t * const buf = QS_priv_.buf;
    QSCtr head          = QS_priv_.head;
    QSCtr const end     = QS_priv_.end;
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
        fu64.u[0] >>= 8U;
    }

    /* output 4 bytes from fu64.u[1]... */
    for (i = 4U; i != 0U; --i) {
        QS_INSERT_ESC_BYTE_((uint8_t)fu64.u[1])
        fu64.u[1] >>= 8U;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}
