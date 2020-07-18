/**
* @file
* @brief QV/C port example for a generic C compiler.
* @ingroup qv
* @cond
******************************************************************************
* Last updated for version 6.8.2
* Last updated on  2020-07-17
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef QV_PORT_H
#define QV_PORT_H

/*! Macro to put the CPU to sleep safely in the cooperative
* QV kernel (inside QV_onIdle()).
*/
/**
* @description
* This macro is provided in some QP ports for the QV kernel and
* in general it depends on the interrupt disabling policy.
*
* @note The provided code is just an example (for ARM Cortex-M).
*/
#define QV_CPU_SLEEP() do { \
    __disable_interrupt(); \
    QF_INT_ENABLE(); \
    __WFI(); \
    __enable_interrupt(); \
} while (false)

#include "qv.h" /* QV platform-independent public interface */

#endif /* QV_PORT_H */

