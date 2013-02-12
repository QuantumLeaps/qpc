/*****************************************************************************
* Product: QK/C port to Altera Nios II, GNU compiler
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
#ifndef __ALT_HOOKS_H__
#define __ALT_HOOKS_H__

#define ALT_OS_TIME_TICK()  QF_tick_()
#define ALT_OS_INIT()       ((void)0)
#define ALT_OS_STOP()       ((void)0)

#define ALT_OS_INT_ENTER()  (++QK_intNest_)

#define ALT_OS_INT_EXIT()   \
    if ((--QK_intNest_) == (alt_u8)0) { \
        alt_u8 p = QK_schedPrio_(); \
        if (p != (alt_u8)0) { \
            QK_schedExt_(p); \
        } \
    } else ((void)0)

extern alt_u8 QK_intNest_;
void QF_tick_(void);
void QK_schedExt_(alt_u8 p);

#endif                                                   /* __ALT_HOOKS_H__ */
