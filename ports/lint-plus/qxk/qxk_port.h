/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-07-29
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QXK/C port example for a generic C99 compiler.
* @description
* This is an example of a QP/C port with the documentation for the
* configuration macros and includes.
*/
#ifndef QXK_PORT_H
#define QXK_PORT_H

/*==========================================================================*/

/*! determination if the code executes in the ISR context (ARM Cortex-M) */
#define QXK_ISR_CONTEXT_() (getSR() != 0U)

/*! activate the context-switch callback */
#define QXK_ON_CONTEXT_SW 1

/*! trigger context switch (used internally in QXK only) */
#define QXK_CONTEXT_SWITCH_() (trigSWI())

/* QXK interrupt entry and exit... */
/*! Define the ISR entry sequence, if the compiler supports writing
* interrupts in C.
*/
#define QXK_ISR_ENTRY() do {                     \
    QF_INT_DISABLE();                            \
    ++QF_intNest_;                               \
    QF_QS_ISR_ENTRY(QF_intNest_, QXK_currPrio_); \
    QF_INT_ENABLE();                             \
} while (false)


/*! Define the ISR exit sequence, if the compiler supports writing
* interrupts in C. */
/**
* @note
* This is just an example of #QXK_ISR_EXIT. You need to define the macro
* appropriately for the CPU/compiler you're using. Also, some QXK ports will
* not define this macro, but instead will provide ISR skeleton in assembly.
*/
#define QXK_ISR_EXIT() do {  \
    QF_INT_DISABLE();        \
    --QF_intNest_;           \
    if (QF_intNest_ == 0U) { \
        QXK_sched_();        \
    }                        \
    QF_INT_ENABLE();         \
} while (false)

uint32_t getSR(void);
void trigSWI(void);

#include "qxk.h" /* QXK platform-independent public interface */

#endif /* QXK_PORT_H */

