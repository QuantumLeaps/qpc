/**
* @file
* @brief QV/C port to ARM Cortex-M, TI-ARM (CCS) toolset
* @cond
******************************************************************************
* Last Updated for Version: 5.8.1
* Date of the Last Update:  2016-12-14
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* http://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qv_port_h
#define qv_port_h

#if (defined __TI_TMS470_V7M3__) || (defined __TI_TMS470_V7M4__)

    /* macro to put the CPU to sleep inside QV_onIdle() */
    #define QV_CPU_SLEEP() do { \
        __asm(" CPSID I"); \
        QF_INT_ENABLE(); \
        __asm(" WFI"); \
        __asm(" CPSIE I"); \
    } while (0)

    /* initialization of the QV kernel for Cortex-M3/M4/M4F */
    #define QV_INIT() QV_init()
    void QV_init(void);

#else /* not M3 or M4 */

    /* macro to put the CPU to sleep inside QV_onIdle() */
    #define QV_CPU_SLEEP() do { \
        __asm(" WFI"); \
        QF_INT_ENABLE(); \
    } while (0)

#endif

#include "qv.h" /* QV platform-independent public interface */

#endif /* qv_port_h */
