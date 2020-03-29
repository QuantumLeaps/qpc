/**
* @file
* @brief QV/C port to ARM Cortex-R, IAR-ARM toolset
* @cond
******************************************************************************
* Last Updated for Version: 5.7.0
* Date of the Last Update:  2016-08-26
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2016 Quantum Leaps, LLC. All rights reserved.
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
#ifndef QV_PORT_H
#define QV_PORT_H

/* macro to put the CPU to sleep inside QV_onIdle() */
#define QV_CPU_SLEEP() do { \
    __WFI(); \
    QF_INT_ENABLE(); \
} while (false)

#include "qv.h" /* QV platform-independent public interface */

#endif /* QV_PORT_H */
