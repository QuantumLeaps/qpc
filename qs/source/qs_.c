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
#include "qs_pkg.h"

/**
* \file
* \ingroup qs
* \brief QS functions for internal use inside QP components
*/

/*..........................................................................*/
void const *QS_smObj_;                    /* local state machine QEP filter */
void const *QS_aoObj_;                     /* local active object QF filter */
void const *QS_mpObj_;                        /* local event pool QF filter */
void const *QS_eqObj_;                         /* local raw queue QF filter */
void const *QS_teObj_;                        /* local time event QF filter */
void const *QS_apObj_;                   /* local object Application filter */

QSTimeCtr QS_tickCtr_;            /* tick counter for the QS_QF_TICK record */

/*..........................................................................*/
void QS_u8_(uint8_t d) {
    QS_INSERT_ESC_BYTE(d)
}
/*..........................................................................*/
void QS_u16_(uint16_t d) {
    QS_INSERT_ESC_BYTE((uint8_t)d)
    d >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)d)
}
/*..........................................................................*/
void QS_u32_(uint32_t d) {
    QS_INSERT_ESC_BYTE((uint8_t)d)
    d >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)d)
    d >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)d)
    d >>= 8;
    QS_INSERT_ESC_BYTE((uint8_t)d)
}
/*..........................................................................*/
void QS_str_(char_t const *s) {
    uint8_t b = (uint8_t)(*s);
    while (b != (uint8_t)(0)) {
                                   /* ASCII characters don't need escaping  */
        QS_chksum_ = (uint8_t)(QS_chksum_ + (uint8_t)*s);
        QS_INSERT_BYTE((uint8_t)*s)
        QS_PTR_INC_(s);
        b = (uint8_t)(*s);
    }
    QS_INSERT_BYTE((uint8_t)0)
}
/*..........................................................................*/
void QS_str_ROM_(char_t const Q_ROM * Q_ROM_VAR s) {
    uint8_t b = (uint8_t)Q_ROM_BYTE(*s);
    while (b != (uint8_t)0) {
                                   /* ASCII characters don't need escaping  */
        QS_chksum_ = (uint8_t)(QS_chksum_ + b);
        QS_INSERT_BYTE(b)
        QS_PTR_INC_(s);
        b = (uint8_t)Q_ROM_BYTE(*s);
    }
    QS_INSERT_BYTE((uint8_t)0)
}
