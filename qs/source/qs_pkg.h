/*****************************************************************************
* Product:  QS/C
* Last Updated for Version: 5.0.0
* Date of the Last Update:  Sep 16, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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

/*..........................................................................*/
/** \brief Internal QS macro to insert an un-escaped byte into
* the QS buffer
*/
#define QS_INSERT_BYTE(b_) \
    *QS_PTR_AT_(head) = (b_); \
    ++head; \
    if (head == end) { \
        head = (QSCtr)0; \
    }

/** \brief Internal QS macro to insert an escaped byte into the QS buffer */
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


/** \brief Internal QS macro to access the QS ring buffer
*
* \note The QS buffer is allocated by the user and is accessed through the
* pointer QS_ring_, which violates the MISRA-C 2004 Rule 17.4(req), pointer
* arithmetic other than array indexing. Encapsulating this violation in a
* macro allows to selectively suppress this specific deviation.
*/
#define QS_PTR_AT_(i_) (buf + (i_))

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
    * space (ROM). The workaround for such compilers is to explicitly add
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

#endif                                                          /* qs_pkg_h */

