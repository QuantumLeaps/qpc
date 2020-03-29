/**
* @file
* @brief QXK/C port example for a generic C compiler.
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 6.7.0
* Last updated on  2019-12-18
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

/****************************************************************************/

/*! determination if the code executes in the ISR context
* (used internally in QXK only)
*/
#define QXK_ISR_CONTEXT_() (getSR() != 0U)

/*! activate the context-switch callback */
#define QXK_ON_CONTEXT_SW 1

/*! trigger context switch (used internally in QXK only) */
#define QXK_CONTEXT_SWITCH_() (trigSWI())

/* QXK interrupt entry and exit... */
/*! Define the ISR entry sequence, if the compiler supports writing
* interrupts in C.
*/
/**
* @note This is just an example of #QXK_ISR_ENTRY. You need to define
* the macro appropriately for the CPU/compiler you're using. Also, some
* QXK ports will not define this macro, but instead will provide ISR
* skeleton code in assembly.
*/
#define QXK_ISR_ENTRY() do {                      \
    QF_INT_DISABLE();                             \
    ++QXK_intNest_;                               \
    QF_QS_ISR_ENTRY(QXK_intNest_, QXK_currPrio_); \
    QF_INT_ENABLE();                              \
} while (false)


/*! Define the ISR exit sequence, if the compiler supports writing
* interrupts in C. */
/**
* @note
* This is just an example of #QXK_ISR_EXIT. You need to define the macro
* appropriately for the CPU/compiler you're using. Also, some QXK ports will
* not define this macro, but instead will provide ISR skeleton in assembly.
*/
#define QXK_ISR_EXIT() do {   \
    QF_INT_DISABLE();         \
    --QXK_intNest_;           \
    if (QXK_intNest_ == 0U) { \
        QXK_sched_();         \
    }                         \
    QF_INT_ENABLE();          \
} while (false)

#include "qxk.h" /* QXK platform-independent public interface */

uint32_t getSR(void);
void trigSWI(void);

#endif /* QXK_PORT_H */

