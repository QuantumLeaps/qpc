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
* \brief QS_getBlock() implementation
*/

/*..........................................................................*/
/* get up to *pn bytes of contiguous memory */
uint8_t const *QS_getBlock(uint16_t *pNbytes) {
    uint8_t *block;
    if (QS_used_ == (QSCtr)0) {
        *pNbytes = (uint16_t)0;
        block = (uint8_t *)0;               /* no bytes to return right now */
    }
    else {
        QSCtr t;
        QSCtr n = (QSCtr)(QS_end_ - QS_tail_);
        if (n > QS_used_) {
            n = QS_used_;
        }
        if (n > (QSCtr)(*pNbytes)) {
            n = (QSCtr)(*pNbytes);
        }
        *pNbytes = (uint16_t)n;
        QS_used_ = (QSCtr)(QS_used_ - n);
        t = QS_tail_;
        QS_tail_ = (QSCtr)(QS_tail_ + n);
        if (QS_tail_ == QS_end_) {
            QS_tail_ = (QSCtr)0;
        }
        block = &QS_PTR_AT_(t);
    }
    return block;
}
