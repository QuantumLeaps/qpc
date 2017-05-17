/**
* @file
* @brief QK/C port to ARM Cortex-M, IAR-ARM toolset
* @cond
******************************************************************************
* Last Updated for Version: 5.9.0
* Date of the Last Update:  2017-03-17
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
* https://state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qk_port_h
#define qk_port_h

/* determination if the code executes in the ISR context */
#define QK_ISR_CONTEXT_() (__get_IPSR() != (uint32_t)0)

/* QK interrupt entry and exit */
#define QK_ISR_ENTRY() ((void)0)

#define QK_ISR_EXIT()  do { \
    QF_INT_DISABLE(); \
    if (QK_sched_() != (uint_fast8_t)0) { \
        *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (uint32_t)(1U << 28); \
    } \
    QF_INT_ENABLE(); \
} while (0)

/* initialization of the QK kernel */
#define QK_INIT() QK_init()
void QK_init(void);

#include "qk.h" /* QK platform-independent public interface */

#endif /* qk_port_h */
