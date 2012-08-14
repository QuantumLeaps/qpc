/*****************************************************************************
* Product:  QS/C
* Last Updated for Version: 4.4.02
* Date of the Last Update:  Apr 13, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
#ifndef qs_pkg_h
#define qs_pkg_h

/**
* \file
* \ingroup qs
* \brief Internal (package scope) QS/C interface.
*/

#include "qs_port.h"                                             /* QS port */

/** \brief QS ring buffer counter and offset type */
typedef uint16_t QSCtr;

/*..........................................................................*/
/** \brief Internal QS macro to insert an un-escaped byte into
* the QS buffer
*/
#define QS_INSERT_BYTE(b_) \
    QS_PTR_AT_(QS_head_) = (b_); \
    ++QS_head_; \
    if (QS_head_ == QS_end_) { \
        QS_head_ = (QSCtr)0; \
    } \
    ++QS_used_;

/** \brief Internal QS macro to insert an escaped byte into the QS buffer */
#define QS_INSERT_ESC_BYTE(b_) \
    QS_chksum_ = (uint8_t)(QS_chksum_ + (b_)); \
    if (((b_) == QS_FRAME) || ((b_) == QS_ESC)) { \
        QS_INSERT_BYTE(QS_ESC) \
        QS_INSERT_BYTE((uint8_t)((b_) ^ QS_ESC_XOR)) \
    } \
    else { \
        QS_INSERT_BYTE(b_) \
    }

/** \brief Internal QS macro to insert a escaped checksum byte into
* the QS buffer
*/
#define QS_INSERT_CHKSUM_BYTE() \
    QS_chksum_ = (uint8_t)~QS_chksum_; \
    if ((QS_chksum_ == QS_FRAME) || (QS_chksum_ == QS_ESC)) { \
        QS_INSERT_BYTE(QS_ESC) \
        QS_INSERT_BYTE((uint8_t)(QS_chksum_ ^ QS_ESC_XOR)) \
    } \
    else { \
        QS_INSERT_BYTE(QS_chksum_) \
    }

/** \brief Internal QS macro to access the QS ring buffer
*
* \note The QS buffer is allocated by the user and is accessed through the
* pointer QS_ring_, which violates the MISRA-C 2004 Rule 17.4(req), pointer
* arithmetic other than array indexing. Encapsulating this violation in a
* macro allows to selectively suppress this specific deviation.
*/
#define QS_PTR_AT_(i_) (QS_ring_[i_])

/** \brief Internal QS macro to increment the given pointer argument \a ptr_
*
* \note Incrementing a pointer violates the MISRA-C 2004 Rule 17.4(req),
* pointer arithmetic other than array indexing. Encapsulating this violation
* in a macro allows to selectively suppress this specific deviation.
*/
#define QS_PTR_INC_(ptr_) (++(ptr_))

/** \brief Frame character of the QS output protocol */
#define QS_FRAME    ((uint8_t)0x7E)

/** \brief Escape character of the QS output protocol */
#define QS_ESC      ((uint8_t)0x7D)

/** \brief Escape modifier of the QS output protocol
*
* The escaped byte is XOR-ed with the escape modifier before it is inserted
* into the QS buffer.
*/
#define QS_ESC_XOR  ((uint8_t)0x20)

#ifndef Q_ROM_BYTE
    /** \brief Macro to access a byte allocated in ROM
    *
    * Some compilers for Harvard-architecture MCUs, such as gcc for AVR, do
    * not generate correct code for accessing data allocated in the program
    * space (ROM). The workaround for such compilers is to explictly add
    * assembly code to access each data element allocated in the program
    * space. The macro Q_ROM_BYTE() retrieves a byte from the given ROM
    * address.
    *
    * The Q_ROM_BYTE() macro should be defined for the compilers that
    * cannot handle correctly data allocated in ROM (such as the gcc).
    * If the macro is left undefined, the default definition simply returns
    * the argument and lets the compiler generate the correct code.
    */
    #define Q_ROM_BYTE(rom_var_)   (rom_var_)
#endif

/*..........................................................................*/
extern uint8_t *QS_ring_;      /**< pointer to the start of the ring buffer */
extern QSCtr QS_end_;             /**< offset of the end of the ring buffer */
extern QSCtr QS_head_;      /**< offset to where next byte will be inserted */
extern QSCtr QS_tail_;    /**< offset of where next event will be extracted */
extern QSCtr QS_used_;    /**< number of bytes currently in the ring buffer */
extern uint8_t QS_seq_;                     /**< the record sequence number */
extern uint8_t QS_chksum_;          /**< the checksum of the current record */
extern uint8_t QS_full_;           /**< the ring buffer is temporarily full */

#endif                                                          /* qs_pkg_h */

