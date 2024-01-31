//============================================================================
// QP/C Real-Time Embedded Framework (RTEF)
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
//! @date Last updated on: 2024-01-03
//! @version Last updated for: @ref qpc_7_3_2
//!
//! @file
//! @brief QF/C port to Zephyr RTOS (v 3.1.99)

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

Q_DEFINE_THIS_MODULE("qf_port")

//............................................................................
struct k_spinlock QF_spinlock;

//............................................................................
void QF_init(void) {
    QF_spinlock = (struct k_spinlock){};
}
//............................................................................
int_t QF_run(void) {
    QF_onStartup();

#ifdef Q_SPY

#if (CONFIG_NUM_PREEMPT_PRIORITIES > 0)
    // lower the priority of the main thread to the level of idle thread
    k_thread_priority_set(k_current_get(),
                          CONFIG_NUM_PREEMPT_PRIORITIES - 1);
#endif

    // produce the QS_QF_RUN trace record
    QS_CRIT_STAT
    QS_CRIT_ENTRY();
    QS_BEGIN_PRE_(QS_QF_RUN, 0U)
    QS_END_PRE_()
    QS_CRIT_EXIT();

    // perform QS work...
    while (true) {
        QS_rxParse();   // parse any QS-RX bytes
        QS_doOutput();  // perform the QS-TX output
    }
#else
    return 0; // return from the main Zephyr thread
#endif
}
//............................................................................
void QF_stop(void) {
    QF_onCleanup();  // cleanup callback
}

//............................................................................
static void thread_entry(void *p1, void *p2, void *p3) {
    Q_UNUSED_PAR(p2);
    Q_UNUSED_PAR(p3);
    QActive *act = (QActive *)p1;

    // event-loop
    for (;;) { // for-ever
        QEvt const *e = QActive_get_(act);
        // dispatch event (virtual call)
        (*act->super.vptr->dispatch)(&act->super, e, act->prio);
        QF_gc(e); // check if the event is garbage, and collect it if so
    }
}

//............................................................................
//
// In the Zephyr port the generic function QActive_setAttr() is used to
// set the options for the Zephyr thread (attr1) and thread name (attr2).
// QActive_setAttr() needs to be called *before* QACTIVE_START() for the
// given active object.
//
// In this Zephyr port the attributes will be used as follows
// (see also QActive_start_()):
// - attr1 - will be used for thread options in k_thread_create()
// - attr2 - will be used for thread name in k_thread_name_set()
//
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    me->thread.base.order_key = attr1; // will be used for thread options
    me->thread.init_data = (void *)attr2; // will be used for thread name
}
//............................................................................
void QActive_start_(QActive * const me, QPrioSpec const prioSpec,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    me->prio  = (uint8_t)(prioSpec & 0xFFU); // QF-priority of the AO
    me->pthre = 0U;   // preemption-threshold (not used for AO registration)
    QActive_register_(me); // register this AO

    // initialize the Zephyr message queue
    k_msgq_init(&me->eQueue, (char *)qSto, sizeof(QEvt *), (uint32_t)qLen);

    // top-most initial tran. (virtual call)
    (*me->super.vptr->init)(&me->super, par, me->prio);
    QS_FLUSH(); // flush the trace buffer to the host

    // The Zephyr priority of the AO thread can be specified in two ways:
    //
    // 1. Implictily based on the AO's priority (Zephyr uses the reverse
    //    priority numbering scheme than QP). This option is chosen, when
    //    the higher-byte of the prioSpec parameter is set to zero.
    //
    // 2. Explicitly as the higher-byte of the prioSpec parameter.
    //    This option is chosen when the prioSpec parameter is not-zero.
    //    For example, Q_PRIO(10U, -1U) will explicitly specify AO priority
    //    as 10 and Zephyr priority as -1.
    //
    //    NOTE: The explicit Zephyr priority is NOT sanity-checked,
    //    so it is the responsibility of the application to ensure that
    //    it is consistent with the AO's priority. An example of
    //    inconsistent setting would be assigning Zephyr priorities that
    //    would result in a different relative priritization of AO's threads
    //    than indicated by the AO priorities assigned.
    //
    int zephyr_prio = (int)((int16_t)prioSpec >> 8);
    if (zephyr_prio == 0) {
        zephyr_prio = (int)QF_MAX_ACTIVE - (int)me->prio;
    }

    // extract data temporarily saved in me->thread by QActive_setAttr()
    uint32_t opt = me->thread.base.order_key;
#ifdef CONFIG_THREAD_NAME
    char const *name = (char const *)me->thread.init_data;
#endif

    // clear the Zephyr thread structure before creating the thread
    me->thread = (struct k_thread){};

    // create a Zephyr thread for the AO...
    k_thread_create(&me->thread,
                    (k_thread_stack_t *)stkSto,
                    (size_t)stkSize,
                    &thread_entry,
                    (void *)me, // p1
                    (void *)0,  // p2
                    (void *)0,  // p3
                    zephyr_prio,// Zephyr priority
                    opt,        // thread options
                    K_NO_WAIT); // start immediately

#ifdef CONFIG_THREAD_NAME
    // set the Zephyr thread name, if initialized, or the default name "AO"
    k_thread_name_set(&me->thread, (name != (char *)0) ? name : "AO");
#endif
}
//............................................................................
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
{
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    // NOTE: k_msgq_num_free_get() can be safely called from crit-section
    uint_fast16_t nFree = (uint_fast16_t)k_msgq_num_free_get(&me->eQueue);

    bool status;
    if (margin == QF_NO_MARGIN) {
        if (nFree > 0U) {
            status = true; // can post
        }
        else {
            status = false; // cannot post
            Q_ERROR_INCRIT(510); // must be able to post the event
        }
    }
    else if (nFree > (QEQueueCtr)margin) {
        status = true; // can post
    }
    else {
        status = false; // cannot post
    }

    if (status) { // can post the event?

        QS_BEGIN_PRE_(QS_QF_ACTIVE_POST, me->prio)
            QS_TIME_PRE_();       // timestamp
            QS_OBJ_PRE_(sender);  // the sender object
            QS_SIG_PRE_(e->sig);  // the signal of the event
            QS_OBJ_PRE_(me);      // this active object (recipient)
            QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_);// pool-Id & ref-Count
            QS_EQC_PRE_(nFree);   // # free entries available
            QS_EQC_PRE_(0U);      // min # free entries (unknown)
        QS_END_PRE_()

        if (QEvt_getPoolNum_(e) != 0U) { // is it a pool event?
            QEvt_refCtr_inc_(e); // increment the reference counter
        }
        QF_CRIT_EXIT();

        int err = k_msgq_put(&me->eQueue, (void const *)&e, K_NO_WAIT);

        // posting to the Zephyr message queue must succeed, see NOTE1
        QF_CRIT_ENTRY();
        Q_ASSERT_INCRIT(520, err == 0);
    }
    else {

        QS_BEGIN_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();       // timestamp
            QS_OBJ_PRE_(sender);  // the sender object
            QS_SIG_PRE_(e->sig);  // the signal of the event
            QS_OBJ_PRE_(me);      // this active object (recipient)
            QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_);// pool-Id & ref-Count
            QS_EQC_PRE_(nFree);   // # free entries available
            QS_EQC_PRE_(0U);      // min # free entries (unknown)
        QS_END_PRE_()
    }
    QF_CRIT_EXIT();

    return status;
}
//............................................................................
void QActive_postLIFO_(QActive * const me, QEvt const * const e) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();

    QS_BEGIN_PRE_(QS_QF_ACTIVE_POST_LIFO, me->prio)
        QS_TIME_PRE_();       // timestamp
        QS_SIG_PRE_(e->sig);  // the signal of this event
        QS_OBJ_PRE_(me);      // this active object
        QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // pool-Id & ref-Count
        QS_EQC_PRE_(k_msgq_num_free_get(&me->eQueue)); // # free entries
        QS_EQC_PRE_(0U);      // min # free entries (unknown)
    QS_END_PRE_()

    if (QEvt_getPoolNum_(e) != 0U) { // is it a pool event?
        QEvt_refCtr_inc_(e); // increment the reference counter
    }
    QF_CRIT_EXIT();

    // NOTE: Zephyr message queue does not currently support LIFO posting
    // so normal FIFO posting is used instead.
    int err = k_msgq_put(&me->eQueue, (void *)&e, K_NO_WAIT);

    QF_CRIT_ENTRY();
    Q_ASSERT_INCRIT(710, err == 0);
    QF_CRIT_EXIT();
}
//............................................................................
QEvt const *QActive_get_(QActive * const me) {

    // wait for an event (forever)
    QEvt const *e;
    int err = k_msgq_get(&me->eQueue, (void *)&e, K_FOREVER);

    // queue-get must succeed
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_ASSERT_INCRIT(810, err == 0);

    QS_BEGIN_PRE_(QS_QF_ACTIVE_GET, me->prio)
        QS_TIME_PRE_();       // timestamp
        QS_SIG_PRE_(e->sig);  // the signal of this event
        QS_OBJ_PRE_(me);      // this active object
        QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // pool-Id & ref-Count
        QS_EQC_PRE_(k_msgq_num_free_get(&me->eQueue));// # free entries
    QS_END_PRE_()

    QF_CRIT_EXIT();

    return e;
}

