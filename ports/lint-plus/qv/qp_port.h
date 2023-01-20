//============================================================================
// QP/C Real-Time Embedded Framework (RTEF)
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com>
// <info@state-machine.com>
//============================================================================
//! @date Last updated on: 2023-09-07
//! @version Last updated for: @ref qpc_7_3_0
//!
//! @file
//! @brief QP/C "port" to PC-Lint-Plus, QV kernel, generic C99

#ifndef QP_PORT_H_
#define QP_PORT_H_

#include <stdint.h>  // Exact-width types. WG14/N843 C99 Standard
#include <stdbool.h> // Boolean type.      WG14/N843 C99 Standard

#ifdef QP_CONFIG
#include "qp_config.h" // external QP configuration
#endif

// no-return function specifier (C11 Standard)
#define Q_NORETURN   _Noreturn  void

// QF configuration for QV -- data members of the QActive class...

// QV event-queue used for AOs
#define QACTIVE_EQUEUE_TYPE  QEQueue

// QF "thread" type used to store the MPU settings in the AO
#define QACTIVE_THREAD_TYPE  void const *

// interrupt disabling mechanism
#define QF_INT_DISABLE()     intDisable()
#define QF_INT_ENABLE()      intEnable()

// QF critical section mechanism
#define QF_CRIT_STAT         uint32_t crit_stat_;
#define QF_CRIT_ENTRY()      (crit_stat_ = critEntry())
#define QF_CRIT_EXIT()       critExit(crit_stat_)

#define QV_CPU_SLEEP() \
do {                        \
    __disable_interrupt();  \
    QF_INT_ENABLE();        \
    __WFI();                \
    __enable_interrupt();   \
} while (false)

void intDisable(void);
void intEnable(void);
uint32_t critEntry(void);
void critExit(uint32_t stat);

// include files -------------------------------------------------------------
#include "qequeue.h"  // QV kernel uses the native QP event queue
#include "qmpool.h"   // QV kernel uses the native QP memory pool
#include "qp.h"       // QP framework
#include "qv.h"       // QV kernel

#endif // QP_PORT_H_

