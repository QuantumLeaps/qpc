/**
* @file
* @brief QK/C port to ARM Cortex-M, IAR-ARM toolset
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
#ifndef QK_PORT_H
#define QK_PORT_H

/* determination if the code executes in the ISR context */
#define QK_ISR_CONTEXT_() (__get_IPSR() != 0U)

/* QK interrupt entry and exit */
#define QK_ISR_ENTRY() ((void)0)

#define QK_ISR_EXIT()  do {                                   \
    QF_INT_DISABLE();                                         \
    if (QK_sched_() != 0U) {                                  \
        *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (1U << 28U);\
        QK_ARM_ERRATUM_838869();                              \
    }                                                         \
    QF_INT_ENABLE();                                          \
} while (false)

#if (__ARM_ARCH == 6) /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    #define QK_ARM_ERRATUM_838869() ((void)0)
#else /* Cortex-M3/M4/M7 (v7-M) */
    /* The following macro implements the recommended workaround for the
    * ARM Erratum 838869. Specifically, for Cortex-M3/M4/M7 the DSB
    * (memory barrier) instruction needs to be added before exiting an ISR.
    */
    #define QK_ARM_ERRATUM_838869() __DSB()
#endif

/* initialization of the QK kernel */
#define QK_INIT() QK_init()
void QK_init(void);
void QK_thread_ret(void);

#include "qk.h" /* QK platform-independent public interface */

#endif /* QK_PORT_H */

