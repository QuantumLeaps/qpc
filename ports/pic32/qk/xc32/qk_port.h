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
* @date Last updated on: 2022-07-30
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QK/C port, preemptive QK kernel, MPLAB-X XC32 compiler
* @ingroup ports
*/
#ifndef QK_PORT_H
#define QK_PORT_H

/* QK interrupt entry and exit */
#define QK_ISR_ENTRY() do { \
    QF_INT_DISABLE();       \
    ++QF_intNest_;          \
    QF_INT_ENABLE();        \
} while (false)

#define QK_ISR_EXIT() do {          \
    QF_INT_DISABLE();               \
    --QF_intNest_;                  \
    if (QK_sched_() != 0U) {        \
        IFS0SET = _IFS0_CS0IF_MASK; \
    }                               \
    QF_INT_ENABLE();                \
} while (false)

/* initialization of the QK kernel */
#define QK_INIT() QK_init()
void QK_init(void);

#include "qk.h"  /* QK platform-independent public interface */

/*==========================================================================*/
/* NOTE01:
* Any interrupt service routine that interacts with QP must begin with the
* QK_ISR_ENTRY() macro and must end with the QK_ISR_EXIT() macro. The source
* file containing the interrupt service routine must #include <sys/attribs.h>.
* Core software interrupt 0 and Interrupt Priority Level 1 are reserved for
* use by QK.
*/

#endif /* QK_PORT_H */
