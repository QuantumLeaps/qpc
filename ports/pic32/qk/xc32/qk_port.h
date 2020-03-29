/**
* @file
* @brief QK/C port, preemptive QK kernel, MPLAB-X XC32 compiler
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.5.1
* Last updated on  2019-06-12
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

/* QK interrupt entry and exit */
#define QK_ISR_ENTRY() do { \
    QF_INT_DISABLE();       \
    ++QK_attr_.intNest;     \
    QF_INT_ENABLE();        \
} while (false)

#define QK_ISR_EXIT() do {          \
    QF_INT_DISABLE();               \
    --QK_attr_.intNest;             \
    if (QK_sched_() != 0U) {        \
        IFS0SET = _IFS0_CS0IF_MASK; \
    }                               \
    QF_INT_ENABLE();                \
} while (false)

/* initialization of the QK kernel */
#define QK_INIT() QK_init()
void QK_init(void);

#include "qk.h"  /* QK platform-independent public interface */

/*****************************************************************************
* NOTE01:
* Any interrupt service routine that interacts with QP must begin with the
* QK_ISR_ENTRY() macro and must end with the QK_ISR_EXIT() macro. The source
* file containing the interrupt service routine must #include <sys/attribs.h>.
* Core software interrupt 0 and Interrupt Priority Level 1 are reserved for
* use by QK.
*/

#endif /* QK_PORT_H */

