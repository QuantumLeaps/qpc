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
* \brief QS_str() adn QS_str_ROM() implementation
*/

/*..........................................................................*/
void QS_str(char_t const *s) {
    QS_INSERT_BYTE((uint8_t)QS_STR_T)
    QS_chksum_ = (uint8_t)(QS_chksum_ + (uint8_t)QS_STR_T);
    while ((*s) != (char_t)'\0') {
                                    /* ASCII characters don't need escaping */
        QS_INSERT_BYTE((uint8_t)(*s))
        QS_chksum_ = (uint8_t)(QS_chksum_ + (uint8_t)(*s));
        QS_PTR_INC_(s);
    }
    QS_INSERT_BYTE((uint8_t)0)
}
/*..........................................................................*/
void QS_str_ROM(char_t const Q_ROM * Q_ROM_VAR s) {
    uint8_t b;
    QS_INSERT_BYTE((uint8_t)QS_STR_T)
    QS_chksum_ = (uint8_t)(QS_chksum_ + (uint8_t)QS_STR_T);
    while ((b = (uint8_t)Q_ROM_BYTE(*s)) != (uint8_t)0) {
                                    /* ASCII characters don't need escaping */
        QS_INSERT_BYTE(b)
        QS_chksum_ = (uint8_t)(QS_chksum_ + b);
        QS_PTR_INC_(s);
    }
    QS_INSERT_BYTE((uint8_t)0)
}
