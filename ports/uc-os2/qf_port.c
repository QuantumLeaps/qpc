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
//! @date Last updated on: 2023-11-15
//! @version Last updated for: @ref qpc_7_3_1
//!
//! @file
//! @brief QP/C port to uC-OS2, generic C11 compiler
//!
#define QP_IMPL           // this is QP implementation
#include "qp_port.h"      // QP port
#include "qp_pkg.h"       // QP package-scope interface
#include "qsafe.h"        // QP Functional Safety (FuSa) System
#ifdef Q_SPY              // QS software tracing enabled?
    #include "qs_port.h"  // QS port
    #include "qs_pkg.h"   // QS package-scope internal interface
#else
    #include "qs_dummy.h" // disable the QS software tracing
#endif // Q_SPY

Q_DEFINE_THIS_MODULE("qf_port")

// Local objects -----------------------------------------------------------
static void task_function(void *pdata); // uC-OS2 task signature

//............................................................................
void QF_init(void) {
    OSInit();  // initialize uC-OS2
}
//............................................................................
int_t QF_run(void) {
    QF_onStartup();  // QF callback to configure and start interrupts

    // produce the QS_QF_RUN trace record
    QS_CRIT_STAT
    QS_CRIT_ENTRY();
    QS_BEGIN_PRE_(QS_QF_RUN, 0U)
    QS_END_PRE_()
    QS_CRIT_EXIT();

    OSStart(); // start uC-OS2 multitasking,  should never return

    return 0; // this unreachable return keeps the compiler happy
}
//............................................................................
void QF_stop(void) {
    QF_onCleanup();  // cleanup callback
}

//............................................................................
static void task_function(void *pdata) { // uC-OS2 task signature
    QActive *act = (QActive *)pdata;

    // event-loop
    for (;;) { // for-ever
        QEvt const *e = QActive_get_((QActive *)pdata);
        // dispatch event (virtual call)
        (*act->super.vptr->dispatch)(&act->super, e, act->prio);
        QF_gc(e); // check if the event is garbage, and collect it if so
    }
    //act->unregister_(); // remove this object from QF
}

//............................................................................
void QActive_start_(QActive * const me, QPrioSpec const prioSpec,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    // task name to be passed to OSTaskCreateExt()
    void * const task_name = (void *)me->eQueue;

    // create uC-OS2 queue and make sure it was created correctly
    me->eQueue = OSQCreate((void **)qSto, qLen);  // create uC-OS2 queue

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    // the uC-OS2 queue must be created correctly
    Q_ASSERT_INCRIT(210, me->eQueue != (OS_EVENT *)0);
    QF_CRIT_EXIT();

    me->prio  = (uint8_t)(prioSpec & 0xFFU); // QF-priority of the AO
    me->pthre = 0U; // preemption-threshold (not used)
    QActive_register_(me); // make QF aware of this AO

    // top-most initial tran. (virtual call)
    (*me->super.vptr->init)(&me->super, par, me->prio);
    QS_FLUSH(); // flush the trace buffer to the host

    // map from QP to uC-OS2 priority
    // The uC-OS2 priority of the AO thread can be specified in two ways:
    //
    // 1. Implictily based on the AO's priority (uC-OS2 uses the reverse
    //    priority numbering scheme than QP). This option is chosen when
    //    the higher-byte of the prioSpec parameter is set to zero.
    //
    // 2. Explicitly as the higher-byte of the prioSpec parameter.
    //    This option is chosen when the prioSpec parameter is not-zero.
    //    For example, Q_PRIO(10U, 5U) will explicitly specify AO priority
    //    as 10 and FreeRTOS priority as 5.
    //
    //    NOTE: The explicit uC-OS2 priority is NOT sanity-checked,
    //    so it is the responsibility of the application to ensure that
    //    it is consistent with the AO's priority. An example of
    //    inconsistent setting would be assigning uC-OS2 priorities that
    //    would result in a different relative priritization of AO's threads
    //    than indicated by the AO priorities assigned.
    //
    INT8U ucos2_prio = (prioSpec >> 8U);
    if (ucos2_prio == 0U) {
        ucos2_prio = (INT8U)(OS_LOWEST_PRIO - me->prio);
    }

    // create AO's task...
    //
    // NOTE: The call to uC-OS2 API OSTaskCreateExt() assumes that the
    // pointer to the top-of-stack (ptos) is at the end of the provided
    // stack memory. This is correct only for CPUs with downward-growing
    // stack, but must be changed for CPUs with upward-growing stack
    INT8U const err = OSTaskCreateExt(&task_function, // the task function
        (void *)me,          // the 'pdata' parameter
#if OS_STK_GROWTH
        &((OS_STK *)stkSto)[(stkSize/sizeof(OS_STK)) - 1], // ptos
#else
        (OS_STK *)stkSto,    // ptos
#endif
        ucos2_prio,          // uC-OS2 task priority
        (INT16U)me->prio,    // the unique AO priority as task ID
#if OS_STK_GROWTH
        (OS_STK *)stkSto,    // pbos
#else
        &((OS_STK *)stkSto)[(stkSize/sizeof(OS_STK)) - 1], // pbos
#endif
        (INT32U)(stkSize/sizeof(OS_STK)), // stack size in OS_STK units
        task_name,           // pext
        (INT16U)me->thread); // task options, see NOTE1

    QF_CRIT_ENTRY();
    // uC-OS2 task must be created correctly
    Q_ASSERT_INCRIT(220, err == OS_ERR_NONE);
    QF_CRIT_EXIT();
}
//............................................................................
// NOTE: This function must be called BEFORE starting an active object
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    switch (attr1) {
        case TASK_NAME_ATTR:
           // this function must be called before QACTIVE_START(),
           // which implies that me->eQueue must not be used yet;
           Q_ASSERT_INCRIT(300, me->eQueue == (OS_EVENT *)0);
           // temporarily store the name, cast 'const' away
            me->eQueue = (OS_EVENT *)attr2;
            break;
        // ...
        default:
            me->thread = attr1;
            break;
    }
    QF_CRIT_EXIT();
}

//............................................................................
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
{
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    uint_fast16_t const nFree =
        (uint_fast16_t)(((OS_Q_DATA *)me->eQueue)->OSQSize
         - ((OS_Q_DATA *)me->eQueue)->OSNMsgs);
    bool status;
    if (margin == QF_NO_MARGIN) {
        if (nFree > 0U) {
            status = true;   // can post
        }
        else {
            status = false;  // cannot post
            Q_ERROR_INCRIT(710); // must be able to post the event
        }
    }
    else if (nFree > (QEQueueCtr)margin) {
        status = true;  // can post
    }
    else {
        status = false; // cannot post
    }

    if (status) { // can post the event?

        QS_BEGIN_PRE_(QS_QF_ACTIVE_POST, me->prio)
            QS_TIME_PRE_();      // timestamp
            QS_OBJ_PRE_(sender); // the sender object
            QS_SIG_PRE_(e->sig); // the signal of the event
            QS_OBJ_PRE_(me);     // this active object (recipient)
            QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_);// pool-Id & ref-Count
            QS_EQC_PRE_(nFree);  // # free entries
            QS_EQC_PRE_(0U);     // min # free entries (unknown)
        QS_END_PRE_()

        if (QEvt_getPoolNum_(e) != 0U) {  // is it a pool event?
            QEvt_refCtr_inc_(e); // increment the reference counter
        }

        QF_CRIT_EXIT();

        INT8U err = OSQPost(me->eQueue, (void *)e);

        QF_CRIT_ENTRY();
        // posting to uC-OS2 message queue must succeed, see NOTE3
        Q_ASSERT_INCRIT(720, err == OS_ERR_NONE);
        QF_CRIT_EXIT();
    }
    else {

        QS_BEGIN_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();      // timestamp
            QS_OBJ_PRE_(sender); // the sender object
            QS_SIG_PRE_(e->sig); // the signal of the event
            QS_OBJ_PRE_(me);     // this active object (recipient)
            QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_);// pool-Id & ref-Count
            QS_EQC_PRE_(nFree);  // # free entries available
            QS_EQC_PRE_(margin); // margin requested
        QS_END_PRE_()

        QF_CRIT_EXIT();
    }

    return status;
}
//............................................................................
void QActive_postLIFO_(QActive * const me, QEvt const * const e) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();

    QS_BEGIN_PRE_(QS_QF_ACTIVE_POST_LIFO, me->prio)
        QS_TIME_PRE_();      // timestamp
        QS_SIG_PRE_(e->sig); // the signal of this event
        QS_OBJ_PRE_(me);     // this active object
        QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // pool-Id & ref-Count
        QS_EQC_PRE_(((OS_Q *)me->eQueue)->OSQSize
                     - ((OS_Q *)me->eQueue)->OSQEntries); // # free entries
        QS_EQC_PRE_(0U);     // min # free entries (unknown)
    QS_END_PRE_()

    if (QEvt_getPoolNum_(e) != 0U) {  // is it a pool event?
        QEvt_refCtr_inc_(e); // increment the reference counter
    }
    QF_CRIT_EXIT();

    INT8U err = OSQPostFront((OS_EVENT *)me->eQueue, (void *)e);

    QF_CRIT_ENTRY();
    // posting to uC-OS2 message queue must succeed, see NOTE3
    Q_ASSERT_INCRIT(810, err == OS_ERR_NONE);
    QF_CRIT_EXIT();
}
//............................................................................
QEvt const *QActive_get_(QActive * const me) {
    INT8U err;
    QEvt const *e = (QEvt const *)OSQPend((OS_EVENT *)me->eQueue, 0U, &err);

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_ASSERT_INCRIT(910, err == OS_ERR_NONE);

    QS_BEGIN_PRE_(QS_QF_ACTIVE_GET, me->prio)
        QS_TIME_PRE_();      // timestamp
        QS_SIG_PRE_(e->sig); // the signal of this event
        QS_OBJ_PRE_(me);     // this active object
        QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // pool-Id & ref-Count
        QS_EQC_PRE_(((OS_Q *)me->eQueue)->OSQSize
                    - ((OS_Q *)me->eQueue)->OSQEntries); // # free entries
    QS_END_PRE_()
    QF_CRIT_EXIT();

    return e;
}

//============================================================================
// NOTE0:
// The QF_onStartup() should enter the critical section before configuring
// and starting interrupts and it should NOT exit the critical section.
// Thus the interrupts cannot fire until uC-OS2 starts multitasking
// in OSStart(). This is to prevent a (narrow) time window in which interrupts
// could make some tasks ready to run, but the OS would not be ready yet
// to perform context switch.
//
// NOTE1:
// The member QActive.thread is set to the uC-OS2 task options in the
// function QF_setUCosTaskAttr(), which must be called **before**
// QACTIVE_START().
//
// NOTE3:
// The event posting to uC-OS2 message queue occurs OUTSIDE critical section,
// which means that the remaining margin of available slots in the queue
// cannot be guaranteed. The problem is that interrupts and other tasks can
// preempt the event posting after checking the margin, but before actually
// posting the event to the queue.

