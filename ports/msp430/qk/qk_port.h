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
* @brief QK/C port port to MSP430
*/
#ifndef QK_PORT_H
#define QK_PORT_H

/* QK interrupt entry and exit... */
#define QK_ISR_ENTRY()    (++QF_intNest_)

#define QK_ISR_EXIT()     do {    \
    --QF_intNest_;                \
    if (QF_intNest_ == 0U) {      \
        if (QK_sched_() != 0U) {  \
            QK_activate_(1U);     \
        }                         \
    }                             \
    else {                        \
        Q_ERROR();                \
    }                             \
} while (false)

#include "qk.h"  /* QK platform-independent public interface */

#endif /* QK_PORT_H */
