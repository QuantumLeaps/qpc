//============================================================================
// QP/C Real-Time Event Framework (RTEF)
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open-source GNU
// General Public License (GPL) or under the terms of one of the closed-
// source Quantum Leaps commercial licenses.
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// NOTE:
// The GPL does NOT permit the incorporation of this code into proprietary
// programs. Please contact Quantum Leaps for commercial licensing options,
// which expressly supersede the GPL and are designed explicitly for
// closed-source distribution.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================

// This QP/C port is NOT a real-time kernel.
// See other POSIX ports for a better effort at a real-time kernel.

// This port can be used to run a QP/C application within the context of a
// POSIX system where events are processed and mediated via a pollfd event-loop.
//
// This port provides a QF_run() function (like other QP/C kernels) based on
// looping around and polling on a file-descriptor (read end of a pipe) and
// timing out for the next tick (QF_setTickRate).
//
// Thus, this port is completely single-threaded (as opposed to POSIX-QV, which
// uses a background pthread for clock ticks). The critical sections enter/exit
// logic only guards against nesting of critical sections, but does not
// protect against concurrent access.
//
// The main benefit of this port is that event processing of the QP/C
// application can be integrated within other frameworks that rely on polling
// file-descriptors, as is common in POSIX software. To that end, this port
// exposes internal functions that encapsulate the constituent parts of a
// main event loop:
//  - QF_preRun_: Initializes the main loop (global) variables.
//  - QF_getReadyFD_: Gets the file-descriptor to poll, indicating QP/C has events to process.
//  - QF_updateNextTick_: Advances the time at which the next tick will occur.
//  - QF_getNextTimeoutMS_: Gets the number of milliseconds until the next tick (or -1 for infinite timeout).
//  - QF_onReadySignal_: Processes the next queued event in the QP/C kernel.
//  - QF_postRun_: Finalizes the main loop, i.e., free resources.
// See QF_run for a basic event-loop that uses the above parts.
// The intent is for an external event-loop system, based on polling, to be
// able to integrate those parts in a manner appropriate for that framework.
//
// As a single-threaded port, it is only safe to post or publish events to
// QP/C while neither of the above functions (plus QF_onClockTick) are
// running.

#ifndef QP_PORT_H_
#define QP_PORT_H_

#include <stdint.h>  // Exact-width types. WG14/N843 C99 Standard
#include <stdbool.h> // Boolean type.      WG14/N843 C99 Standard
#include "qp_config.h"  // QP configuration from the application

// no-return function specifier (C11 Standard)
#define Q_NORETURN   _Noreturn void

// QActive event queue and thread types for POSIX-FD
#define QACTIVE_EQUEUE_TYPE  QEQueue
//QACTIVE_OS_OBJ_TYPE  not used in this port
//QACTIVE_THREAD_TYPE  not used in this port

// QF critical section for POSIX-FD
#define QF_CRIT_STAT
#define QF_CRIT_ENTRY()      QF_enterCriticalSection_()
#define QF_CRIT_EXIT()       QF_leaveCriticalSection_()
#define QF_CRIT_EST()        QF_enterCriticalSection_()

// QF_LOG2 not defined -- use the internal LOG2() implementation

// internal functions for critical section management
void QF_enterCriticalSection_(void);
void QF_leaveCriticalSection_(void);

// Set clock tick rate
// (NOTE ticksPerSec==0 disables the poll timeout)
// (NOTE ticksPerSec is limited to 1000 due to poll timeout resolution in milliseconds)
// (NOTE tickPrio is ignored in this port, no background ticker thread, kept for compat.)
void QF_setTickRate(uint32_t ticksPerSec, int tickPrio);

// clock tick callback (NOTE not called when ticksPerSec==0)
// must call QTIMEEVT_TICK_X()
void QF_onClockTick(void);

// Implementation functions that break-down the "QF_run" loop into steps
// Useful to integrate into external pollfd-based event-loop.
// See QF_run in qf_port.c for the appropriate usage pattern.
void QF_preRun_(void);
int QF_getReadyFD_(void);
void QF_updateNextTick_(void);
int QF_getNextTimeoutMS_(void);
void QF_onReadySignal_(void);
void QF_postRun_(void);

// include files -------------------------------------------------------------
#include "qequeue.h"   // POSIX-FD needs the native event-queue
#include "qmpool.h"    // POSIX-FD needs the native memory-pool
#include "qp.h"        // QP platform-independent public interface

//============================================================================
// interface used only inside QF implementation, but not in applications

#ifdef QP_IMPL

    // QF scheduler locking for POSIX-FD (not needed in single-thread port)
    #define QF_SCHED_STAT_
    #define QF_SCHED_LOCK_(dummy) ((void)0)
    #define QF_SCHED_UNLOCK_()    ((void)0)

    // QF event queue customization for POSIX-FD...
    #define QACTIVE_EQUEUE_WAIT_(me_) ((void)0)
    #define QACTIVE_EQUEUE_SIGNAL_(me_) \
        QPSet_insert(&QF_readySet_, (me_)->prio); \
        QF_signalReadyOnPipe_()

    // native QF event pool operations
    #define QF_EPOOL_TYPE_  QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_, qsId_) \
        ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qsId_)))
    #define QF_EPOOL_PUT_(p_, e_, qsId_) \
        (QMPool_put(&(p_), (e_), (qsId_)))

    extern QPSet QF_readySet_;
    extern int QF_readyPipeWrite_; // Pipe to signal events
    extern int QF_readyPipeRead_; // Pipe to signal events

    void QF_signalReadyOnPipe_(void);

#endif // QP_IMPL

#endif // QP_PORT_H_
