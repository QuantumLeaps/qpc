//$file${src::qxk::qxk.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: qpc.qm
// File:  ${src::qxk::qxk.c}
//
// This code has been generated by QM 6.1.1 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This code is covered by the following QP license:
// License #    : LicenseRef-QL-dual
// Issued to    : Any user of the QP/C real-time embedded framework
// Framework(s) : qpc
// Support ends : 2024-12-31
// License scope:
//
// Copyright (C) 2005 Quantum Leaps, LLC <state-machine.com>.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
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
//
//$endhead${src::qxk::qxk.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#define QP_IMPL           // this is QP implementation
#include "qp_port.h"      // QP port
#include "qp_pkg.h"       // QP package-scope interface
#include "qsafe.h"        // QP Functional Safety (FuSa) Subsystem
#ifdef Q_SPY              // QS software tracing enabled?
    #include "qs_port.h"  // QS port
    #include "qs_pkg.h"   // QS facilities for pre-defined trace records
#else
    #include "qs_dummy.h" // disable the QS software tracing
#endif // Q_SPY

// protection against including this source file in a wrong project
#ifndef QXK_H_
    #error "Source file included in a project NOT based on the QXK kernel"
#endif // QXK_H_

Q_DEFINE_THIS_MODULE("qxk")

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${QXK::QXK-base} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${QXK::QXK-base::priv_} ....................................................
QXK_Attr QXK_priv_;

//${QXK::QXK-base::schedLock} ................................................
//! @static @public @memberof QXK
QSchedStatus QXK_schedLock(uint_fast8_t const ceiling) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(100, !QXK_ISR_CONTEXT_());

    QSchedStatus stat; // saved lock status to be returned

    // is the lock ceiling being raised?
    if (ceiling > QXK_priv_.lockCeil) {
        QS_BEGIN_PRE_(QS_SCHED_LOCK, 0U)
            QS_TIME_PRE_(); // timestamp
            // the previous lock ceiling & new lock ceiling
            QS_2U8_PRE_((uint8_t)QXK_priv_.lockCeil, (uint8_t)ceiling);
        QS_END_PRE_()

        // previous status of the lock
        stat  = (QSchedStatus)QXK_priv_.lockHolder;
        stat |= (QSchedStatus)QXK_priv_.lockCeil << 8U;

        // new status of the lock
        QXK_priv_.lockHolder = (QXK_priv_.curr != (QActive *)0)
                               ? (uint_fast8_t)QXK_priv_.curr->prio
                               : 0U;
        QXK_priv_.lockCeil   = ceiling;
    }
    else {
       stat = 0xFFU; // scheduler not locked
    }
    QF_MEM_APP();
    QF_CRIT_EXIT();

    return stat; // return the status to be saved in a stack variable
}

//${QXK::QXK-base::schedUnlock} ..............................................
//! @static @public @memberof QXK
void QXK_schedUnlock(QSchedStatus const stat) {
    // has the scheduler been actually locked by the last QXK_schedLock()?
    if (stat != 0xFFU) {
        uint8_t const prevCeil = (uint8_t)(stat >> 8U);
        QF_CRIT_STAT
        QF_CRIT_ENTRY();
        QF_MEM_SYS();

        Q_REQUIRE_INCRIT(200, !QXK_ISR_CONTEXT_());
        Q_REQUIRE_INCRIT(201, QXK_priv_.lockCeil > prevCeil);

        QS_BEGIN_PRE_(QS_SCHED_UNLOCK, 0U)
            QS_TIME_PRE_(); // timestamp
            // ceiling before unlocking & prio after unlocking
            QS_2U8_PRE_((uint8_t)QXK_priv_.lockCeil, (uint8_t)prevCeil);
        QS_END_PRE_()

        // restore the previous lock ceiling and lock holder
        QXK_priv_.lockCeil   = prevCeil;
        QXK_priv_.lockHolder = (stat & 0xFFU);

        // find if any threads should be run after unlocking the scheduler
        if (QXK_sched_() != 0U) { // activation needed?
            QXK_activate_(); // synchronously activate basic-thread(s)
        }

        QF_MEM_APP();
        QF_CRIT_EXIT();
    }
}

//${QXK::QXK-base::current} ..................................................
//! @static @public @memberof QXK
QActive * QXK_current(void) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(600, QXK_priv_.lockCeil <= QF_MAX_ACTIVE);

    struct QActive *curr = QXK_priv_.curr;
    if (curr == (QActive *)0) { // basic thread?
        curr = QActive_registry_[QXK_priv_.actPrio];
    }

    Q_ASSERT_INCRIT(690, curr != (QActive *)0);

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return curr;
}

//${QXK::QXK-base::sched_} ...................................................
//! @static @private @memberof QXK
uint_fast8_t QXK_sched_(void) {
    Q_INVARIANT_INCRIT(402, QPSet_verify_(&QXK_priv_.readySet,
                                          &QXK_priv_.readySet_dis));

    QActive const * const curr = QXK_priv_.curr;
    uint_fast8_t p;
    if (QPSet_isEmpty(&QXK_priv_.readySet)) {
        p = 0U; // no activation needed
    }
    else {
        // find the highest-prio thread ready to run
        p = QPSet_findMax(&QXK_priv_.readySet);
        if (p <= QXK_priv_.lockCeil) {
            p = (uint_fast8_t)QActive_registry_[QXK_priv_.lockHolder]->prio;
            if (p != 0U) {
                Q_ASSERT_INCRIT(410,
                    QPSet_hasElement(&QXK_priv_.readySet, p));
            }
        }
    }
    QActive * const next = QActive_registry_[p];

    // is the current thread a basic-thread?
    if (curr == (QActive *)0) {

        // is the next prio. above the active prio.?
        if (p > QXK_priv_.actPrio) {
            QXK_priv_.next = next; // set the next AO to activate

            if (next->osObject != (void *)0) { // is next extended?
                QXK_CONTEXT_SWITCH_();
                p = 0U; // no activation needed
            }
        }
        else { // below the active prio.
            QXK_priv_.next = (QActive *)0;
            p = 0U; // no activation needed
        }
    }
    else { // currently executing an extended-thread
        // is the current thread different from the next?
        if (curr != next) {
            QXK_priv_.next = next;
            QXK_CONTEXT_SWITCH_();
        }
        else { // current is the same as next
            QXK_priv_.next = (QActive *)0; // no need to context-switch
        }
        p = 0U; // no activation needed
    }
    return p;
}

//${QXK::QXK-base::activate_} ................................................
//! @static @private @memberof QXK
void QXK_activate_(void) {
    uint_fast8_t const prio_in = QXK_priv_.actPrio;
    QActive *next = QXK_priv_.next; // the next AO (basic-thread) to run

    Q_REQUIRE_INCRIT(500, (next != (QActive *)0)
                           && (prio_in <= QF_MAX_ACTIVE));

    // QXK Context switch callback defined or QS tracing enabled?
    #if (defined QF_ON_CONTEXT_SW) || (defined Q_SPY)
    QXK_contextSw_(next);
    #endif // QF_ON_CONTEXT_SW || Q_SPY

    QXK_priv_.next = (QActive *)0; // clear the next AO
    QXK_priv_.curr = (QActive *)0; // current is basic-thread

    // prio. of the next thread
    uint_fast8_t p = (uint_fast8_t)next->prio;

    // loop until no more ready-to-run AOs of higher prio than the initial
    do  {
        QXK_priv_.actPrio = p; // next active prio

        QF_MEM_APP();
        QF_INT_ENABLE(); // unconditionally enable interrupts

        QEvt const * const e = QActive_get_(next);
        // NOTE QActive_get_() performs QS_MEM_APP() before return

        // dispatch event (virtual call)
        (*next->super.vptr->dispatch)(&next->super, e, p);
    #if (QF_MAX_EPOOL > 0U)
        QF_gc(e);
    #endif

        QF_INT_DISABLE(); // unconditionally disable interrupts
        QF_MEM_SYS();

        // check internal integrity (duplicate inverse storage)
        Q_INVARIANT_INCRIT(502, QPSet_verify_(&QXK_priv_.readySet,
                                              &QXK_priv_.readySet_dis));

        if (next->eQueue.frontEvt == (QEvt *)0) { // empty queue?
            QPSet_remove(&QXK_priv_.readySet, p);
    #ifndef Q_UNSAFE
            QPSet_update_(&QXK_priv_.readySet, &QXK_priv_.readySet_dis);
    #endif
        }

        if (QPSet_isEmpty(&QXK_priv_.readySet)) {
            QXK_priv_.next = (QActive *)0;
            next = QActive_registry_[0];
            p = 0U; // no activation needed
        }
        else {
            // find next highest-prio below the lock ceiling
            p = (uint8_t)QPSet_findMax(&QXK_priv_.readySet);
            if (p <= QXK_priv_.lockCeil) {
                p = QXK_priv_.lockHolder; // thread holding lock
                if (p != 0U) {
                    Q_ASSERT_INCRIT(510,
                        QPSet_hasElement(&QXK_priv_.readySet, p));
                }
            }

            // set the next thread and ensure that it is registered
            next = QActive_registry_[p];
            Q_ASSERT_INCRIT(520, next != (QActive *)0);

            // is next a basic thread?
            if (next->osObject == (void *)0) {
                // is the next prio. above the initial prio.?
                if (p > QActive_registry_[prio_in]->prio) {
    #if (defined QF_ON_CONTEXT_SW) || (defined Q_SPY)
                    if (p != QXK_priv_.actPrio) { // changing threads?
                        QXK_contextSw_(next);
                    }
    #endif // QF_ON_CONTEXT_SW || Q_SPY
                    QXK_priv_.next = next;
                }
                else {
                    QXK_priv_.next = (QActive *)0;
                    p = 0U; // no activation needed
                }
            }
            else {  // next is the extended-thread
                QXK_priv_.next = next;
                QXK_CONTEXT_SWITCH_();
                p = 0U; // no activation needed
            }
        }
    } while (p != 0U); // while activation needed

    // restore the active prio.
    QXK_priv_.actPrio = prio_in;

    #if (defined QF_ON_CONTEXT_SW) || (defined Q_SPY)
    if (next->osObject == (void *)0) {
        QXK_contextSw_((prio_in == 0U)
                       ? (QActive *)0
                       : QActive_registry_[prio_in]);
    }
    #endif // QF_ON_CONTEXT_SW || Q_SPY
}

//${QXK::QXK-base::contextSw_} ...............................................
//! @static @public @memberof QXK
void QXK_contextSw_(QActive * const next) {
    #ifdef Q_SPY
    uint8_t const prev_prio = (QXK_priv_.prev != (QActive *)0)
                        ? QXK_priv_.prev->prio
                        : 0U;
    if (next != (QActive *)0) { // next is NOT idle?
        QS_BEGIN_PRE_(QS_SCHED_NEXT, next->prio)
            QS_TIME_PRE_(); // timestamp
            QS_2U8_PRE_(next->prio, prev_prio);
        QS_END_PRE_()
    }
    else { // going to idle
        QS_BEGIN_PRE_(QS_SCHED_IDLE, prev_prio)
            QS_TIME_PRE_(); // timestamp
            QS_U8_PRE_(prev_prio);
        QS_END_PRE_()
    }
    #endif // Q_SPY

    #ifdef QF_ON_CONTEXT_SW
    QF_onContextSw(QXK_priv_.prev, next);
    #endif // QF_ON_CONTEXT_SW

    QXK_priv_.prev = next; // update the previous thread
}

//${QXK::QXK-base::threadExit_} ..............................................
//! @static @private @memberof QXK
void QXK_threadExit_(void) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();

    QXThread const * const thr = QXTHREAD_CAST_(QXK_priv_.curr);

    Q_REQUIRE_INCRIT(900, (!QXK_ISR_CONTEXT_())
        && (thr != (QXThread *)0)); // current thread must be extended
    Q_REQUIRE_INCRIT(901,
        QXK_priv_.lockHolder != (uint_fast8_t)thr->super.prio);

    uint_fast8_t const p = (uint_fast8_t)thr->super.prio;

    QF_MEM_SYS();
    QActive_registry_[p] = (QActive *)0;
    QPSet_remove(&QXK_priv_.readySet, p);
    #ifndef Q_UNSAFE
    QPSet_update_(&QXK_priv_.readySet, &QXK_priv_.readySet_dis);
    #endif

    (void)QXK_sched_(); // schedule other threads

    QF_MEM_APP();
    QF_CRIT_EXIT();
}
//$enddef${QXK::QXK-base} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${QXK::QF-cust} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${QXK::QF-cust::init} ......................................................
//! @static @public @memberof QF
void QF_init(void) {
    QF_bzero_(&QF_priv_,                 sizeof(QF_priv_));
    QF_bzero_(&QXK_priv_,                sizeof(QXK_priv_));
    QF_bzero_(&QActive_registry_[0],     sizeof(QActive_registry_));

    // setup the QXK scheduler as initially locked and not running
    QXK_priv_.lockCeil = (QF_MAX_ACTIVE + 1U); // scheduler locked

    #ifndef Q_UNSAFE
    QPSet_update_(&QXK_priv_.readySet, &QXK_priv_.readySet_dis);
    #endif

    for (uint_fast8_t tickRate = 0U;
         tickRate < Q_DIM(QTimeEvt_timeEvtHead_);
         ++tickRate)
    {
        QTimeEvt_ctorX(&QTimeEvt_timeEvtHead_[tickRate],
                       (QActive *)0, (enum_t)Q_USER_SIG, tickRate);
    }

    // QXK idle AO object (const in ROM)
    static QActive const idle_ao = { (struct QAsmVtable const *)0 };

    // register the idle AO object (cast 'const' away)
    QActive_registry_[0] = QACTIVE_CAST_(&idle_ao);
    QXK_priv_.prev = QActive_registry_[0];

    #ifdef QXK_INIT
    QXK_INIT(); // port-specific initialization of the QXK kernel
    #endif
}

//${QXK::QF-cust::stop} ......................................................
//! @static @public @memberof QF
void QF_stop(void) {
    QF_onCleanup(); // application-specific cleanup callback
    // nothing else to do for the dual-mode QXK kernel
}

//${QXK::QF-cust::run} .......................................................
//! @static @public @memberof QF
int_t QF_run(void) {
    #ifdef Q_SPY
    QS_SIG_DICTIONARY(QXK_DELAY_SIG,   (void *)0);
    QS_SIG_DICTIONARY(QXK_TIMEOUT_SIG, (void *)0);

    // produce the QS_QF_RUN trace record
    QF_INT_DISABLE();
    QF_MEM_SYS();
    QS_beginRec_((uint_fast8_t)QS_QF_RUN);
    QS_endRec_();
    QF_MEM_APP();
    QF_INT_ENABLE();
    #endif // Q_SPY

    QF_onStartup(); // application-specific startup callback

    QF_INT_DISABLE();
    QF_MEM_SYS();

    #ifdef QXK_START
    QXK_START(); // port-specific startup of the QXK kernel
    #endif

    QXK_priv_.lockCeil = 0U; // unlock the QXK scheduler

    // activate AOs to process events posted so far
    if (QXK_sched_() != 0U) {
        QXK_activate_();
    }

    QF_MEM_APP();
    QF_INT_ENABLE();

    for (;;) { // QXK idle loop...
        QXK_onIdle(); // application-specific QXK idle callback
    }

    #ifdef __GNUC__  // GNU compiler?
    return 0;
    #endif
}
//$enddef${QXK::QF-cust} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${QXK::QActive} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${QXK::QActive} ............................................................

//${QXK::QActive::start_} ....................................................
//! @public @memberof QActive
void QActive_start_(QActive * const me,
    QPrioSpec const prioSpec,
    QEvt const * * const qSto,
    uint_fast16_t const qLen,
    void * const stkSto,
    uint_fast16_t const stkSize,
    void const * const par)
{
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(300, (!QXK_ISR_CONTEXT_())
                      && ((prioSpec & 0xFF00U) == 0U));
    QF_CRIT_EXIT();

    me->prio  = (uint8_t)(prioSpec & 0xFFU); // QF-prio. of the AO
    me->pthre = 0U; // not used
    QActive_register_(me); // make QF aware of this active object

    if (stkSto == (void *)0) { // starting basic thread (AO)?

        QEQueue_init(&me->eQueue, qSto, qLen); // init the built-in queue
        me->osObject = (void *)0; // no private stack for AO

        // top-most initial tran. (virtual call)
        (*me->super.vptr->init)(&me->super, par, me->prio);
        QS_FLUSH(); // flush the trace buffer to the host

        // see if this AO needs to be scheduled if QXK is already running
        QF_CRIT_ENTRY();
        QF_MEM_SYS();
        if (QXK_priv_.lockCeil <= QF_MAX_ACTIVE) { // scheduler running?
            if (QXK_sched_() != 0U) { // activation needed?
                QXK_activate_(); // synchronously activate basic-thread(s)
            }
        }
        QF_MEM_APP();
        QF_CRIT_EXIT();
    }
    else { // starting QXThread

        // is storage for the queue buffer provided?
        if (qSto != (QEvt const **)0) {
            QEQueue_init(&me->eQueue, qSto, qLen);
        }

        // extended thread constructor puts the thread handler in place of
        // the top-most initial tran. 'me->super.temp.act'
        QXThread_stackInit_(me, me->super.temp.thr, stkSto, stkSize);

        // the new thread is not blocked on any object
        me->super.temp.obj = (QMState *)0;

        QF_CRIT_ENTRY();
        QF_MEM_SYS();

        // extended-thread becomes ready immediately
        QPSet_insert(&QXK_priv_.readySet, (uint_fast8_t)me->prio);
        #ifndef Q_UNSAFE
        QPSet_update_(&QXK_priv_.readySet, &QXK_priv_.readySet_dis);
        #endif

        // see if this thread needs to be scheduled in case QXK is running
        if (QXK_priv_.lockCeil <= QF_MAX_ACTIVE) {
            (void)QXK_sched_(); // schedule other threads
        }
        QF_MEM_APP();
        QF_CRIT_EXIT();
    }
}
//$enddef${QXK::QActive} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
