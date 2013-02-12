/*****************************************************************************
* Product:  QK/C, port to TMS320C55x, TI C5500 compiler
* Last Updated for Version: 4.5.03
* Date of the Last Update:  Jan 16, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
#include "qk_pkg.h"

/*..........................................................................*/
char_t const Q_ROM *QK_getPortVersion(void) {
    static char_t const Q_ROM version[] = "4.4.00";
    return version;
}
/*..........................................................................*/
void QK_init(void) { /* explicitly clear uninitialized QF variables, NOTE01 */
    QK_intNest_  = (uint8_t)0;
    QK_currPrio_ = (uint8_t)(QF_MAX_ACTIVE + 1);
    bzero((uint8_t *)&QK_readySet_, sizeof(QK_readySet_));

#ifndef QK_NO_MUTEX
    QK_ceilingPrio_ = (uint8_t)0;
#endif
}

/*****************************************************************************
* NOTE01:
* The standard TI startup code (c_int00) does NOT zero the uninitialized
* variables, as required by the C-standard. Since QF relies on the clearing
* of the static uninitialized variables, the critical QF objects are cleared
* explicitly in this port.
*/
