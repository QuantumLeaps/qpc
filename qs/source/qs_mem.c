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
* \brief QS_mem() implementation
*/

/*..........................................................................*/
void QS_mem(uint8_t const *blk, uint8_t size) {
    QS_INSERT_BYTE((uint8_t)QS_MEM_T)
    QS_chksum_ = (uint8_t)(QS_chksum_ + (uint8_t)QS_MEM_T);
    QS_INSERT_ESC_BYTE(size)
    while (size != (uint8_t)0) {
        QS_INSERT_ESC_BYTE(*blk)
        QS_PTR_INC_(blk);
        --size;
    }
}
