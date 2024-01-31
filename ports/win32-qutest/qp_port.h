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
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
//! @date Last updated on: 2023-09-07
//! @version Last updated for: @ref qpc_7_3_0
//!
//! @file
//! @brief QP/C port to Win32-QUtest with GNU or Visual Studio C/C++

#ifndef QP_PORT_H_
#define QP_PORT_H_

#include <stdint.h>  // Exact-width types. WG14/N843 C99 Standard
#include <stdbool.h> // Boolean type.      WG14/N843 C99 Standard

#ifdef QP_CONFIG
#include "qp_config.h" // external QP configuration
#endif

#ifdef __GNUC__

    // no-return function specifier (GCC-ARM compiler)
    #define Q_NORETURN   __attribute__ ((noreturn)) void

#elif (defined _MSC_VER)
    #ifdef __cplusplus
        // no-return function specifier (Microsoft Visual Studio C++ compiler)
        #define Q_NORETURN   [[ noreturn ]] void
    #else
        // no-return function specifier C11
        #define Q_NORETURN   _Noreturn void
    #endif

    // This is the case where QP/C is compiled by the Microsoft Visual C++
    // compiler in the C++ mode, which can happen when qep_port.h is included
    // in a C++ module, or the compilation is forced to C++ by the option /TP.
    //
    // The following pragma suppresses the level-4 C++ warnings C4510, C4512,
    // and C4610, which warn that default constructors and assignment operators
    // could not be generated for structures QMState and QMTranActTable.
    //
    // The QP/C source code cannot be changed to avoid these C++ warnings
    // because the structures QMState and QMTranActTable must remain PODs
    // (Plain Old Datatypes) to be initializable statically with constant
    // initializers.
    //
    #pragma warning (disable: 4510 4512 4610)

#endif

// QUTest event queue and thread types
#define QACTIVE_EQUEUE_TYPE  QEQueue
// QACTIVE_OS_OBJ_TYPE not used in this port
// QACTIVE_THREAD_TYPE not used in this port

// QF interrupt disable/enable
#define QF_INT_DISABLE()     (++QS_tstPriv_.intLock)
#define QF_INT_ENABLE()      (--QS_tstPriv_.intLock)

// QF critical section
#define QF_CRIT_STAT
#define QF_CRIT_ENTRY()      QF_INT_DISABLE()
#define QF_CRIT_EXIT()       QF_INT_ENABLE()

// QF_LOG2 not defined -- use the internal LOG2() implementation

// include files -------------------------------------------------------------
#include "qequeue.h"   // QUTest port uses QEQueue event-queue
#include "qmpool.h"    // QUTest port uses QMPool memory-pool
#include "qp.h"        // QP platform-independent public interface

#ifdef _MSC_VER
    #pragma warning (default: 4510 4512 4610)
#endif

//============================================================================
// interface used only inside QF implementation, but not in applications

#ifdef QP_IMPL

    // QUTest scheduler locking (not used)
    #define QF_SCHED_STAT_
    #define QF_SCHED_LOCK_(dummy) ((void)0)
    #define QF_SCHED_UNLOCK_()    ((void)0)

    // native event queue operations
    #define QACTIVE_EQUEUE_WAIT_(me_) \
        Q_ASSERT_INCRIT(305, (me_)->eQueue.frontEvt != (QEvt *)0)
#ifndef Q_UNSAFE
    #define QACTIVE_EQUEUE_SIGNAL_(me_) \
        QPSet_insert(&QS_tstPriv_.readySet, (uint_fast8_t)(me_)->prio); \
        QPSet_update_(&QS_tstPriv_.readySet, &QS_tstPriv_.readySet_dis)
#else
    #define QACTIVE_EQUEUE_SIGNAL_(me_) \
        QPSet_insert(&QF_readySet_, (uint_fast8_t)(me_)->prio)
#endif

    // native QF event pool operations
    #define QF_EPOOL_TYPE_  QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_, qsId_) \
        ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qsId_)))
    #define QF_EPOOL_PUT_(p_, e_, qsId_) \
        (QMPool_put(&(p_), (e_), (qsId_)))

#endif // QP_IMPL

#endif // QP_PORT_H_

