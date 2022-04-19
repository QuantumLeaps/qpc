/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2021-12-23
* @version Last updated for: @ref qpc_7_0_0
*
* @file
* @brief QS long-long (64-bit) output
* @ingroup qs
*/
#define QP_IMPL           /* this is QP implementation */
#include "qs_port.h"      /* QS port */

#include "qs_pkg.h"

/*==========================================================================*/
/*!
* @static @private @memberof QS
* @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u64_raw_(uint64_t d) {
    uint8_t chksum      = QS_priv_.chksum;
    uint8_t * const buf = QS_priv_.buf;
    QSCtr head          = QS_priv_.head;
    QSCtr const end     = QS_priv_.end;

    QS_priv_.used += 8U; /* 8 bytes are about to be added */
    uint_fast8_t i;
    for (i = 8U; i != 0U; --i) {
        uint8_t const b = (uint8_t)d;
        QS_INSERT_ESC_BYTE_(b)
        d >>= 8U;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}

/*==========================================================================*/
/*!
* @static @private @memberof QS
* @note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u64_fmt_(uint8_t format, uint64_t d) {
    uint8_t chksum      = QS_priv_.chksum;
    uint8_t * const buf = QS_priv_.buf;
    QSCtr head          = QS_priv_.head;
    QSCtr const end     = QS_priv_.end;

    QS_priv_.used += 9U; /* 9 bytes are about to be added */
    QS_INSERT_ESC_BYTE_(format) /* insert the format byte */

    /* output 8 bytes of data... */
    uint_fast8_t i;
    for (i = 8U; i != 0U; --i) {
        format = (uint8_t)d;
        QS_INSERT_ESC_BYTE_(format)
        d >>= 8U;
    }

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
}
