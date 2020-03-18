/**
* @file
* @brief QK/C port port to MSP430
* @cond
******************************************************************************
* Last Updated for Version: 6.3.8
* Date of the Last Update:  2019-01-23
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef QK_PORT_H
#define QK_PORT_H

/* QK interrupt entry and exit... */
#define QK_ISR_ENTRY()    (++QK_attr_.intNest)

#define QK_ISR_EXIT()     do {    \
    --QK_attr_.intNest;           \
    if (QK_attr_.intNest == 0U) { \
        if (QK_sched_() != 0U) {  \
            QK_activate_();       \
        }                         \
    }                             \
    else {                        \
        Q_ERROR();                \
    }                             \
} while (false)

#include "qk.h"  /* QK platform-independent public interface */

#endif /* QK_PORT_H */

