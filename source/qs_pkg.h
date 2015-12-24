/**
* @file
* @ingroup qs
* @brief Internal (package scope) QS/C interface.
* @cond
******************************************************************************
* Last updated for version 5.6.0
* Last updated on  2015-12-18
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* http://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qs_pkg_h
#define qs_pkg_h

/****************************************************************************/
/*! Internal QS macro to insert an un-escaped byte into the QS buffer */
#define QS_INSERT_BYTE(b_) \
    QS_PTR_AT_(buf, head) = (b_); \
    ++head; \
    if (head == end) { \
        head = (QSCtr)0; \
    }

/*! Internal QS macro to insert an escaped byte into the QS buffer */
#define QS_INSERT_ESC_BYTE(b_) \
    chksum = (uint8_t)(chksum + (b_)); \
    if (((b_) != QS_FRAME) && ((b_) != QS_ESC)) { \
        QS_INSERT_BYTE(b_) \
    } \
    else { \
        QS_INSERT_BYTE(QS_ESC) \
        QS_INSERT_BYTE((uint8_t)((b_) ^ QS_ESC_XOR)) \
        ++QS_priv_.used; \
    }

/*! Internal QS macro to increment the given pointer parameter @p ptr_ */
/**
* @note Incrementing a pointer violates the MISRA-C 2004 Rule 17.4(req),
* pointer arithmetic other than array indexing. Encapsulating this violation
* in a macro allows to selectively suppress this specific deviation.
*/
#define QS_PTR_INC_(ptr_) (++(ptr_))

/*! Frame character of the QS output protocol */
#define QS_FRAME    ((uint8_t)0x7E)

/*! Escape character of the QS output protocol */
#define QS_ESC      ((uint8_t)0x7D)

/*! The expected checksum value over an uncorrupted QS record */
#define QS_GOOD_CHKSUM ((uint8_t)0xFF)

/*! Escape modifier of the QS output protocol */
/**
* @description
* The escaped byte is XOR-ed with the escape modifier before it is inserted
* into the QS buffer.
*/
#define QS_ESC_XOR  ((uint8_t)0x20)

/*! send the Target info (object sizes, build time-stamp, QP version) */
void QS_target_info_(uint8_t isReset);

#endif  /* qs_pkg_h */

