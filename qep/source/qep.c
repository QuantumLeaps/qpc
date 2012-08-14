/*****************************************************************************
* Product: QEP/C
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
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
#include "qep_pkg.h"

/**
* \file
* \ingroup qep
* \brief QEP_reservedEvt_ definition and QEP_getVersion() implementation.
*/

/* Package-scope objects ---------------------------------------------------*/
QEvt const QEP_reservedEvt_[] = {
    { (QSignal)QEP_EMPTY_SIG_, (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_ENTRY_SIG,    (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_EXIT_SIG,     (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_INIT_SIG,     (uint8_t)0, (uint8_t)0 }
};

/*..........................................................................*/
char_t const Q_ROM * Q_ROM_VAR QEP_getVersion(void) {
    static char_t const Q_ROM Q_ROM_VAR version[] = {
        (char_t)((uint8_t)((QP_VERSION >> 12) & 0xFU) + (uint8_t)'0'),
        (char_t)'.',
        (char_t)((uint8_t)((QP_VERSION >>  8) & 0xFU) + (uint8_t)'0'),
        (char_t)'.',
        (char_t)((uint8_t)((QP_VERSION >>  4) & 0xFU) + (uint8_t)'0'),
        (char_t)((uint8_t)(QP_VERSION         & 0xFU) + (uint8_t)'0'),
        (char_t)'\0'
    };
    return version;
}
