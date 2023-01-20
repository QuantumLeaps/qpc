//============================================================================
// QP/C Real-Time Embedded Framework (RTEF)
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC <state-machine.com>.
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
//! @brief QP/C port to MSP430, QV kernel, TI, GNU, IAR compilers

#ifndef QP_PORT_H_
#define QP_PORT_H_

#include <stdint.h>  // Exact-width types. WG14/N843 C99 Standard
#include <stdbool.h> // Boolean type.      WG14/N843 C99 Standard

#include "qp_config.h" // external QP configuration required in this port

#if defined(__TI_COMPILER_VERSION__) || defined(__GNUC__)
    // no-return function specifier (C11 Standard)
    #define Q_NORETURN   _Noreturn void
#elif defined(__IAR_SYSTEMS_ICC__)
    #define Q_NORETURN   __noreturn void
#endif

// QF configuration for QV -- data members of the QActive class...

// QActive event queue type
#define QACTIVE_EQUEUE_TYPE     QEQueue
// QACTIVE_OS_OBJ_TYPE not used in this port
// QACTIVE_THREAD_TYPE not used in this port

// QF interrupt disable/enable...
#define QF_INT_DISABLE()        __disable_interrupt()
#define QF_INT_ENABLE()         __enable_interrupt()

// QF critical section entry/exit...
#define QF_CRIT_STAT            unsigned short int_state_;
#define QF_CRIT_ENTRY() do {               \
    int_state_ =  __get_interrupt_state(); \
    __disable_interrupt();                 \
} while (false)

#define QF_CRIT_EXIT()          __set_interrupt_state(int_state_)

// include files -------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
    #include <intrinsics.h>
#elif defined(__GNUC__)
    #include <msp430.h>
    #include "in430.h"
#endif

#include "qequeue.h"   // QV kernel uses the native QP event queue
#include "qmpool.h"    // QV kernel uses the native QP memory pool
#include "qp.h"        // QP framework
#include "qv.h"        // QV kernel

#endif // QP_PORT_H_

