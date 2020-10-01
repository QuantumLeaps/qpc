/**
* @file
* @brief QV/C port to ARM Cortex-M, ARM-KEIL toolset
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-23
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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

#if (__TARGET_ARCH_THUMB == 3) /* Cortex-M0/M0+/M1(v6-M, v6S-M)? */

    /* macro to put the CPU to sleep inside QV_onIdle() */
    #define QV_CPU_SLEEP() do { \
        __wfi(); \
        QF_INT_ENABLE(); \
    } while (false)

    #define QV_ARM_ERRATUM_838869() ((void)0)

#else /* Cortex-M3/M4/M7(v7-M) */

    /* macro to put the CPU to sleep inside QV_onIdle() */
    #define QV_CPU_SLEEP() do { \
        QF_PRIMASK_DISABLE();   \
        QF_INT_ENABLE();        \
        __wfi();                \
        QF_PRIMASK_ENABLE();    \
    } while (false)

    /* initialization of the QV kernel for Cortex-M3/M4/M4F */
    #define QV_INIT() QV_init()
    void QV_init(void);

    /* The following macro implements the recommended workaround for the
    * ARM Erratum 838869. Specifically, for Cortex-M3/M4/M7 the DSB
    * (memory barrier) instruction needs to be added before exiting an ISR.
    * This macro should be inserted at the end of ISRs.
    */
    #define QV_ARM_ERRATUM_838869() __asm("dsb")

#endif

#include "qv.h" /* QV platform-independent public interface */

#endif /* QV_PORT_H */

