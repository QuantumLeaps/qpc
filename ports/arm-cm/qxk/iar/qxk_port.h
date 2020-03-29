/**
* @file
* @brief QXK/C port to ARM Cortex-M, IAR-ARM toolset
* @cond
******************************************************************************
* Last updated for version 6.7.0
* Last updated on  2019-11-11
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
#ifndef QXK_PORT_H
#define QXK_PORT_H

/* determination if the code executes in the ISR context */
#define QXK_ISR_CONTEXT_() (__get_IPSR() != 0U)

#if (__CORE__ == __ARM6M__)  /* Cortex-M0/M0+/M1? */

    #define QXK_CONTEXT_SWITCH_() \
        (*Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (1U << 28))

#else /* Cortex-M3/M4/M7 */

    #define QXK_CONTEXT_SWITCH_() do {                        \
        *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (1U << 28); \
        __DSB();                                              \
        __ISB();                                              \
    } while (false)

#endif

/* QXK interrupt entry and exit */
#define QXK_ISR_ENTRY() ((void)0)

#define QXK_ISR_EXIT()  do {                                  \
    QF_INT_DISABLE();                                         \
    if (QXK_sched_() != 0U) {                                 \
        *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (1U << 28); \
    }                                                         \
    QF_INT_ENABLE();                                          \
} while (false)

/* initialization of the QXK kernel */
#define QXK_INIT() QXK_init()
void QXK_init(void);

/* prototype needed for IAR "Multi-file Compilation" */
void Thread_ret(void);

#include "qxk.h" /* QXK platform-independent public interface */

#endif /* QXK_PORT_H */
