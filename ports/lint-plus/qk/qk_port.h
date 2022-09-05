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
* @date Last updated on: 2022-09-03
* @version Last updated for: @ref qpc_7_1_1
*
* @file
* @brief QK/C port example for a generic C compiler.
*/
#ifndef QK_PORT_H
#define QK_PORT_H

/****************************************************************************/

/*! enable the context-switch callback */
#define QK_ON_CONTEXT_SW 1

/*! determination if the code executes in the ISR context (ARM Cortex-M) */
#define QK_ISR_CONTEXT_() (get_IPSR() != 0U)

/* QK interrupt entry and exit... */
/*! Define the ISR entry sequence, if the compiler supports writing
* interrupts in C.
*/
/**
* @note This is just an example of #QK_ISR_ENTRY. You need to define
* the macro appropriately for the CPU/compiler you're using. Also, some
* QK ports will not define this macro, but instead will provide ISR
* skeleton code in assembly.
*/
#define QK_ISR_ENTRY() do {                     \
    QF_INT_DISABLE();                           \
    ++QF_intNest_;                              \
    QF_QS_ISR_ENTRY(QF_intNest_, QK_currPrio_); \
    QF_INT_ENABLE();                            \
} while (false)

/*! Define the ISR exit sequence, if the compiler supports writing
* interrupts in C.
*/
/**
* @note
* This is just an example of #QK_ISR_EXIT. You need to define the macro
* appropriately for the CPU/compiler you're using. Also, some QK ports will
* not define this macro, but instead will provide ISR skeleton in assembly.
*/
#define QK_ISR_EXIT() do {       \
    QF_INT_DISABLE();            \
    --QF_intNest_;               \
    if (QF_intNest_ == 0U) {     \
        if (QK_sched_() != 0U) { \
            QK_activate_(1U);    \
        }                        \
    }                            \
    QF_INT_ENABLE();             \
} while (false)

#include "qk.h" /* QK platform-independent public interface */

uint32_t get_IPSR(void);

#endif /* QK_PORT_H */

