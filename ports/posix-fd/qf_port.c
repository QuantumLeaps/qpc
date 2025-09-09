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

// expose features from the 2008 POSIX standard (IEEE Standard 1003.1-2008)
#define _POSIX_C_SOURCE 200809L

#define QP_IMPL           // this is QP implementation
#include "qp_port.h"      // QP port
#include "qp_pkg.h"       // QP package-scope interface
#include "qsafe.h"        // QP Functional Safety (FuSa) Subsystem
#ifdef Q_SPY              // QS software tracing enabled?
    #include "qs_port.h"  // QS port
    #include "qs_pkg.h"   // QS package-scope internal interface
#else
    #include "qs_dummy.h" // disable the QS software tracing
#endif // Q_SPY

#include <string.h>       // for memcpy() and memset()
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/poll.h>

Q_DEFINE_THIS_MODULE("qf_port")

// Local objects =============================================================

static bool l_isRunning;       // flag indicating when QF is running
static struct timespec l_tick; // structure for the clock tick
static struct timespec l_nextTick; // next clock tick to wake-up for

#define NSEC_PER_SEC           1000000000L
#define NSEC_PER_MSEC          1000000L
#define DEFAULT_TICKS_PER_SEC  100

//............................................................................
static void sigIntHandler(int dummy); // prototype
static void sigIntHandler(int dummy) {
    Q_UNUSED_PAR(dummy);
    QF_onCleanup();
    exit(-1);
}

// Global objects ============================================================
QPSet QF_readySet_;
int QF_readyPipeWrite_; // Pipe to signal events
int QF_readyPipeRead_; // Pipe to signal events

//============================================================================
// QF functions

// NOTE: Only check that nesting of critical sections never occurs
// (see QF_enterCriticalSection_()/QF_leaveCriticalSection_()
static int l_critSectNest;   // critical section nesting up-down counter

//............................................................................
void QF_signalReadyOnPipe_(void) {
    const uint8_t write_val = 0xA5; // Magic, ignored.
    if (write(QF_readyPipeWrite_, &write_val, 1) < 0) {
        Q_ERROR();
    }
}
void QF_clearReadyPipe_(void) {
    ssize_t read_sz = 1;
    while (read_sz > 0) {
        uint8_t read_val = 0;
        read_sz = read(QF_readyPipeRead_, &read_val, 1);
    }
}

//............................................................................
void QF_enterCriticalSection_(void) {
    Q_ASSERT_INCRIT(100, l_critSectNest == 0); // NO nesting of crit.sect!
    ++l_critSectNest;
}
//............................................................................
void QF_leaveCriticalSection_(void) {
    Q_ASSERT_INCRIT(200, l_critSectNest == 1); // crit.sect. must ballace!
    --l_critSectNest;
}

//............................................................................
void QF_init(void) {
    // init the pipe that signals events
    int pipe_fds[2] = {-1, -1};
    if (pipe(pipe_fds) != 0) {
        Q_ERROR();
    }
    QF_readyPipeRead_ = pipe_fds[0];
    QF_readyPipeWrite_ = pipe_fds[1];

    int flags = fcntl(QF_readyPipeRead_, F_GETFL, 0);
    if (flags == -1) {
        Q_ERROR();
    }
    if (fcntl(QF_readyPipeRead_, F_SETFL, flags | O_NONBLOCK | FD_CLOEXEC) == -1) {
        Q_ERROR();
    }

    flags = fcntl(QF_readyPipeWrite_, F_GETFL, 0);
    if (flags == -1) {
        Q_ERROR();
    }
    if (fcntl(QF_readyPipeWrite_, F_SETFL, flags | FD_CLOEXEC) == -1) {
        Q_ERROR();
    }

    l_critSectNest = 0;
    QPSet_setEmpty(&QF_readySet_);

    QTimeEvt_init(); // initialize QTimeEvts

    l_tick.tv_sec = 0;
    l_tick.tv_nsec = NSEC_PER_SEC / DEFAULT_TICKS_PER_SEC; // default rate

    // install the SIGINT (Ctrl-C) signal handler
    struct sigaction sig_act;
    memset(&sig_act, 0, sizeof(sig_act));
    sig_act.sa_handler = &sigIntHandler;
    sigaction(SIGINT, &sig_act, NULL);
}

//............................................................................
void QF_preRun_(void) {
    l_isRunning = true; // QF is running

    QF_onStartup(); // application-specific startup callback

    // produce the QS_QF_RUN trace record
    QS_BEGIN_PRE_(QS_QF_RUN, 0U)
    QS_END_PRE_()

    if ((l_tick.tv_sec != 0) || (l_tick.tv_nsec != 0)) {
        // get the absolute monotonic time for no-drift sleeping
        clock_gettime(CLOCK_MONOTONIC, &l_nextTick);

        // round down nanoseconds to the nearest configured period
        l_nextTick.tv_nsec -= l_nextTick.tv_nsec % l_tick.tv_nsec;
    }
}

//............................................................................
int QF_getReadyFD_(void) {
    return QF_readyPipeRead_;
}

//............................................................................
void QF_updateNextTick_(void) {
    if ((l_tick.tv_sec != 0) || (l_tick.tv_nsec != 0)) {
        // advance to the next tick (absolute time)
        l_nextTick.tv_nsec += l_tick.tv_nsec;
        if (l_nextTick.tv_nsec >= NSEC_PER_SEC) {
            // Can only overflow by one second, since tick rate is a fraction of a second.
            l_nextTick.tv_nsec -= NSEC_PER_SEC;
            l_nextTick.tv_sec  += 1;
        }
    }
}

//............................................................................
int QF_getNextTimeoutMS_(void) {
    int poll_timeout_ms = -1; // Infinite.
    if ((l_tick.tv_sec != 0) || (l_tick.tv_nsec != 0)) {
        struct timespec now_tick;
        clock_gettime(CLOCK_MONOTONIC, &now_tick);
        if (l_nextTick.tv_sec < now_tick.tv_sec) {
            poll_timeout_ms = 0;
        } else if (now_tick.tv_sec == l_nextTick.tv_sec) {
            if (l_nextTick.tv_nsec <= now_tick.tv_nsec) {
                poll_timeout_ms = 0;
            } else {
                poll_timeout_ms = (l_nextTick.tv_nsec - now_tick.tv_nsec) / NSEC_PER_MSEC;
            }
        } else {
            poll_timeout_ms = 1000 + (l_nextTick.tv_nsec - now_tick.tv_nsec) / NSEC_PER_MSEC;
        }
    }
    return poll_timeout_ms;
}

//............................................................................
void QF_onReadySignal_(void) {
    // Flush out the ready pipe, since we are responding now.
    QF_clearReadyPipe_();

    if (QPSet_isEmpty(&QF_readySet_)) {
        return;
    }

    QF_CRIT_ENTRY();

    // find the maximum priority AO ready to run
    uint_fast8_t p = QPSet_findMax(&QF_readySet_);
    QActive *a = QActive_registry_[p];

    // the active object 'a' must still be registered in QF
    // (e.g., it must not be stopped)
    Q_ASSERT_INCRIT(320, a != (QActive *)0);
    QF_CRIT_EXIT();

    QEvt const *e = QActive_get_(a);
    QASM_DISPATCH(&a->super, e, a->prio); // dispatch to the HSM
    QF_gc(e);

    QF_CRIT_ENTRY();
    if (a->eQueue.frontEvt == (QEvt *)0) { // empty queue?
        QPSet_remove(&QF_readySet_, p);
    }
    QF_CRIT_EXIT();

    // Re-trigger ready pipe, in case we have more events.
    // We do not use a nested loop since we could have a tick to process too.
    QF_signalReadyOnPipe_();
}

//............................................................................
void QF_postRun_(void) {
    QF_onCleanup(); // cleanup callback
    QS_EXIT();      // cleanup the QSPY connection

    close(QF_readyPipeRead_);  // cleanup the pipe
    close(QF_readyPipeWrite_); // cleanup the pipe
}

//............................................................................
int QF_run(void) {
    QF_preRun_();

    while (l_isRunning) {
        int poll_timeout_ms = QF_getNextTimeoutMS_();

        struct pollfd pfds[1] = {
            {.fd = QF_getReadyFD_(), .events = POLLIN, .revents = 0}};
        int poll_result = poll(pfds, 1, poll_timeout_ms);
        if (poll_result < 0) {
            Q_ERROR();
        }
        if (poll_result == 0) {
            QF_onClockTick(); // must call QTIMEEVT_TICK_X()
            QF_updateNextTick_();
        }

        if ((pfds[0].revents & POLLIN) == 0) {
            continue;
        }

        QF_onReadySignal_();
    }

    QF_postRun_();

    return 0; // return success
}
//............................................................................
void QF_stop(void) {
    l_isRunning = false; // terminate the main event-loop

    // unblock the event-loop so it can terminate
    QPSet_insert(&QF_readySet_, 1U);
    QF_signalReadyOnPipe_();
}
//............................................................................
void QF_setTickRate(uint32_t ticksPerSec, int tickPrio) {
    Q_UNUSED_PAR(tickPrio);

    if (ticksPerSec > NSEC_PER_SEC / NSEC_PER_MSEC) {
        Q_ERROR();
    }
    if (ticksPerSec != 0U) {
        l_tick.tv_nsec = NSEC_PER_SEC / ticksPerSec;
    }
    else {
        l_tick.tv_nsec = 0U; // means NO system clock tick
    }
}

// QActive functions =========================================================

void QActive_start(QActive * const me,
    QPrioSpec const prioSpec,
    QEvtPtr * const qSto, uint_fast16_t const qLen,
    void * const stkSto, uint_fast16_t const stkSize,
    void const * const par)
{
    Q_UNUSED_PAR(stkSto);
    Q_UNUSED_PAR(stkSize);

    // no per-AO stack needed for this port
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(800, stkSto == (void *)0);
    QF_CRIT_EXIT();

    me->prio  = (uint8_t)(prioSpec & 0xFFU); // QF-priority of the AO
    me->pthre = 0U; // preemption-threshold (not used in this port)
    QActive_register_(me); // register this AO

    QEQueue_init(&me->eQueue, qSto, qLen);

    // top-most initial tran. (virtual call)
    (*me->super.vptr->init)(&me->super, par, me->prio);
    QS_FLUSH(); // flush the QS trace buffer to the host
}

//............................................................................
#ifdef QACTIVE_CAN_STOP
void QActive_stop(QActive * const me) {
    if (QActive_subscrList_ != (QSubscrList *)0) {
        QActive_unsubscribeAll(me); // unsubscribe from all events
    }

    // make sure the AO is no longer in "ready set"
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QPSet_remove(&QF_readySet_, me->prio);
    QF_CRIT_EXIT();

    QActive_unregister_(me);
}
#endif
//............................................................................
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    Q_UNUSED_PAR(me);
    Q_UNUSED_PAR(attr1);
    Q_UNUSED_PAR(attr2);
    Q_ERROR_INCRIT(900); // should not be called in this QP port
}

//============================================================================
