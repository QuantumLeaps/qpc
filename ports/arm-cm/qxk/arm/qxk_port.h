/**
* @file
* @brief QXK/C port to ARM Cortex-M, ARM-KEIL toolset
* @cond
******************************************************************************
* Last Updated for Version: 6.0.3
* Date of the Last Update:  2017-12-09
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
#ifndef qxk_port_h
#define qxk_port_h

/* determination if the code executes in the ISR context */
#define QXK_ISR_CONTEXT_() (QXK_get_IPSR() != (uint32_t)0)

/* inline function for getting the IPSR register */
static __inline uint32_t QXK_get_IPSR(void) {
    register uint32_t volatile __regIPSR __asm("ipsr");
    return __regIPSR;
}

/* trigger the PendSV exception to pefrom the context switch */
#define QXK_CONTEXT_SWITCH_() \
    (*Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (uint32_t)(1U << 28))

/* QXK ISR entry and exit */
#define QXK_ISR_ENTRY() ((void)0)

#define QXK_ISR_EXIT()  do { \
    QF_INT_DISABLE(); \
    if (QXK_sched_() != (uint_fast8_t)0) { \
        *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (uint32_t)(1U << 28); \
    } \
    QF_INT_ENABLE(); \
} while (0)

/* initialization of the QXK kernel */
#define QXK_INIT() QXK_init()
void QXK_init(void);

#include "qxk.h" /* QXK platform-independent public interface */

#endif /* qxk_port_h */
