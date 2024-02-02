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
//! @brief QP/C port to embOS RTOS (v5), generic C11 compiler

#ifndef QP_PORT_H_
#define QP_PORT_H_

#include <stdint.h>  // Exact-width types. WG14/N843 C99 Standard
#include <stdbool.h> // Boolean type.      WG14/N843 C99 Standard

#ifdef QP_CONFIG
#include "qp_config.h" // external QP configuration
#endif

// no-return function specifier (C11 Standard)
#define Q_NORETURN   _Noreturn void

// embOS message mailbox and thread types
#define QACTIVE_EQUEUE_TYPE     OS_MAILBOX
#define QACTIVE_THREAD_TYPE     OS_TASK
#define QACTIVE_OS_OBJ_TYPE     uint32_t

// QF interrupt disable/enable
#define QF_INT_DISABLE()        OS_INT_IncDI()
#define QF_INT_ENABLE()         OS_INT_DecRI()

// QF critical section for embOS, see NOTE2
#define QF_CRIT_STAT
#define QF_CRIT_ENTRY()         QF_INT_DISABLE()
#define QF_CRIT_EXIT()          QF_INT_ENABLE()

// include files -------------------------------------------------------------
#include "RTOS.h"      // embOS API

#include "qequeue.h"   // QP event queue (for deferring events)
#include "qmpool.h"    // QP memory pool (for event pools)
#include "qp.h"        // QP platform-independent public interface


enum EmbOS_TaskAttrs {
    TASK_NAME_ATTR,
    TASK_USES_FPU
};


//============================================================================
// interface used only inside QF, but not in applications

#ifdef QP_IMPL

    // embOS-specific scheduler locking, see NOTE3
    #define QF_SCHED_STAT_
    #define QF_SCHED_LOCK_(dummy) do {            \
        if (OS_INT_InInterrupt() == (OS_BOOL)0) { \
            OS_TASK_EnterRegion();                \
        }                                         \
    } while (false)

    #define QF_SCHED_UNLOCK_() do { \
        if (OS_INT_InInterrupt() == (OS_BOOL)0) { \
            OS_TASK_LeaveRegion();                \
        }                                         \
    } while (false)

    // native QF event pool customization
    #define QF_EPOOL_TYPE_        QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_, qsId_) \
        ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qsId_)))
    #define QF_EPOOL_PUT_(p_, e_, qsId_) \
        (QMPool_put(&(p_), (e_), (qsId_)))

#endif // QP_IMPL

//============================================================================
// NOTE2:
// The embOS critical section does not use the status variable, because the
// implementation uses a global up-down counter, which allows the embOS
// critical section to nest. Nesting of critical sections is needed in this
// QP-embOS port.
//
// NOTE3:
// embOS provides only global scheduler locking for all thread priorities
// by means of OS_EnterRegion() and OS_LeaveRegion(). Therefore, locking the
// scheduler only up to the specified lock priority is not supported.
//

#endif // QP_PORT_H_

