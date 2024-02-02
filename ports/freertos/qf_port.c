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
//! @brief QF/C port to FreeRTOS 10.x, generic C11 compiler

#define QP_IMPL           // this is QP implementation
#include "qp_port.h"      // QP port
#include "qp_pkg.h"       // QP package-level interface
#include "qsafe.h"        // QP Functional Safety (FuSa) Subsystem
#ifdef Q_SPY              // QS software tracing enabled?
    #include "qs_port.h"  // QS port
    #include "qs_pkg.h"   // QS package-scope internal interface
#else
    #include "qs_dummy.h" // disable the QS software tracing
#endif // Q_SPY

Q_DEFINE_THIS_MODULE("qf_port")

#if ( configSUPPORT_STATIC_ALLOCATION == 0 )
#error "This QP/C port to FreeRTOS requires configSUPPORT_STATIC_ALLOCATION "
#endif

#if ( configMAX_PRIORITIES < QF_MAX_ACTIVE )
#error "FreeRTOS configMAX_PRIORITIES must not be less than QF_MAX_ACTIVE"
#endif

// Local objects -------------------------------------------------------------
static void task_function(void *pvParameters); // FreeRTOS task signature

// The following macro provides the number of free slots in the FreeRTOS
// queue.
//
// NOTE1:
// The official FreeRTOS API uxQueueSpacesAvailable() is not used
// here, because that API uses task-level critical section internally.
// Instead, the free slots calculation happens here in already
// established critical section. Unfortunately, the bizarre "information
// obfuscating" policy of FreeRTOS (incorrectly called "information
// hiding") forces the use of the StaticQueue_t with "dummy" members.
// This could potentially break in the future releases of FreeRTOS.
//
// Currently, the correspondence between xQUEUE and StaticQueue_t
// is as follows (see queue.c and FreeRTOS.h, respectively):
//
// xQUEUE.uxMessagesWaiting == StaticQueue_t.uxDummy4[0];
// xQUEUE.uxLength          == StaticQueue_t.uxDummy4[1];
//
#define FREERTOS_QUEUE_GET_FREE(me_) \
    ((me_)->osObject.uxDummy4[1] - (me_)->osObject.uxDummy4[0])

//============================================================================
void QF_init(void) {
    // empty for FreeRTOS
}
//............................................................................
int_t QF_run(void) {
    QF_onStartup(); // the startup callback (configure/enable interrupts)

    // produce the QS_QF_RUN trace record
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QS_BEGIN_PRE_(QS_QF_RUN, 0U)
    QS_END_PRE_()
    QF_CRIT_EXIT();

    vTaskStartScheduler(); // start the FreeRTOS scheduler

    QF_CRIT_ENTRY();
    Q_ERROR_INCRIT(110); // the FreeRTOS scheduler should never return
    QF_CRIT_EXIT();

    return 0; // dummy return to make the compiler happy
}
//............................................................................
void QF_stop(void) {
    QF_onCleanup(); // cleanup callback
}

//............................................................................
static void task_function(void *pvParameters) { // FreeRTOS task signature
    QActive *act = (QActive *)pvParameters;

#ifdef QACTIVE_CAN_STOP
    while (act->eQueue != (QueueHandle_t)0)
#else
    for (;;) // for-ever
#endif
    {
        QEvt const *e = QActive_get_(act); // wait for event
        QASM_DISPATCH(&act->super, e, act->prio); // dispatch to the SM
        QF_gc(e); // check if the event is garbage, and collect it if so
    }
#ifdef QACTIVE_CAN_STOP
    QActive_unregister_(act); // remove this object from the framewrok
    vTaskDelete((TaskHandle_t)0); // delete this FreeRTOS task
#endif
}

//............................................................................
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
    // precondition:
    // - queue storage must be provided
    // - queue size must be provided
    // - stack storage must be provided
    // - stack size must be provided
    Q_REQUIRE_INCRIT(200,
        (qSto != (QEvt const **)0) && (qLen > 0U)
        && (stkSto != (void *)0) && (stkSize > 0U));
    QF_CRIT_EXIT();

    // create FreeRTOS message queue
    me->eQueue = xQueueCreateStatic(
        (UBaseType_t)qLen,     // length of the queue
        (UBaseType_t)sizeof(QEvt *), // element size
        (uint8_t *)qSto,       // storage buffer
        &me->osObject);        // static queue buffer
    QF_CRIT_ENTRY();
    Q_ASSERT_INCRIT(210, me->eQueue != (QueueHandle_t)0);
    QF_CRIT_EXIT();

    me->prio  = (uint8_t)(prioSpec & 0xFFU); // QF-priority of the AO
    me->pthre = 0U; // preemption-threshold (not used for AO registration)
    QActive_register_(me); // register this AO

    // top-most initial tran. (virtual call)
    (*me->super.vptr->init)(&me->super, par, me->prio);
    QS_FLUSH(); // flush the QS trace buffer to the host

    // task name provided by the user in QActive_setAttr() or default name
    char const *taskName = (me->thread.pxDummy1 != (void *)0)
                             ? (char const *)me->thread.pxDummy1
                             : (char const *)"AO";

    // The FreeRTOS priority of the AO thread can be specified in two ways:
    //
    // 1. Implictily based on the AO's priority (by the formula specified
    //    in the macro FREERTOS_TASK_PRIO(), see qp_port.h). This option
    //    is chosen, when the higher-byte of the prioSpec parameter is set
    //    to zero.
    //
    // 2. Explicitly as the higher-byte of the prioSpec parameter.
    //    This option is chosen when the prioSpec parameter is not-zero.
    //    For example, Q_PRIO(10U, 5U) will explicitly specify AO priority
    //    as 10 and FreeRTOS priority as 5.
    //
    //    NOTE: The explicit FreeRTOS priority is NOT sanity-checked,
    //    so it is the responsibility of the application to ensure that
    //    it is consistent with the AO's priority. An example of
    //    inconsistent setting would be assigning FreeRTOS priorities that
    //    would result in a different relative priritization of AO's threads
    //    than indicated by the AO priorities assigned.
    //
    UBaseType_t freertos_prio = (prioSpec >> 8U);
    if (freertos_prio == 0U) {
        freertos_prio = FREERTOS_TASK_PRIO(me->prio);
    }

    // statically create the FreeRTOS task for the AO
    TaskHandle_t task = xTaskCreateStatic(
        &task_function, // the task function
        taskName ,      // the name of the task
        stkSize/sizeof(portSTACK_TYPE), // stack length
        (void *)me,     // the 'pvParameters' parameter
        freertos_prio,  // FreeRTOS priority
        (StackType_t *)stkSto,        // stack storage
        &me->thread);   // task buffer

    QF_CRIT_ENTRY();
    Q_ASSERT_INCRIT(220, task != (TaskHandle_t)0);
    QF_CRIT_EXIT();
}
//............................................................................
#ifdef QACTIVE_CAN_STOP
void QActive_stop(QActive * const me) {
    QActive_unsubscribeAll(me); // unsubscribe from all events
    me->eQueue = (QueueHandle_t)0; // stop the thread (see task_function())
}
#endif
//............................................................................
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    // this function must be called before QACTIVE_START(),
    // which implies that me->thread.pxDummy1 must not be used yet;
    Q_REQUIRE_INCRIT(300, me->thread.pxDummy1 == (void *)0);
    switch (attr1) {
        case TASK_NAME_ATTR:
            // temporarily store the name, cast 'const' away
            me->thread.pxDummy1 = (void *)attr2;
            break;
        // ...
    }
    QF_CRIT_EXIT();
}

//============================================================================
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
{
#ifndef Q_SPY
    Q_UNUSED_PAR(sender);
#endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();

    // find the number of free slots available in the queue
    uint_fast16_t const nFree = (uint_fast16_t)FREERTOS_QUEUE_GET_FREE(me);

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
    else if (nFree > margin) {
        status = true; // can post
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
            QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // poolNum & refCtr
            QS_EQC_PRE_((QEQueueCtr)nFree); // # free entries
            QS_EQC_PRE_(0U);     // min # free entries (unknown)
        QS_END_PRE_()

        if (QEvt_getPoolNum_(e) != 0U) { // is it a pool event?
            QEvt_refCtr_inc_(e); // increment the reference counter
        }
        QF_CRIT_EXIT();

        BaseType_t err = xQueueSendToBack(
                             me->eQueue, (void const *)&e, (TickType_t)0);

        // posting to the FreeRTOS message queue must succeed, see NOTE3
        QF_CRIT_ENTRY();
        Q_ASSERT_INCRIT(520, err == pdPASS);
    }
    else {

        QS_BEGIN_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();      // timestamp
            QS_OBJ_PRE_(sender); // the sender object
            QS_SIG_PRE_(e->sig); // the signal of the event
            QS_OBJ_PRE_(me);     // this active object (recipient)
            QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // poolNum & refCtr
            QS_EQC_PRE_((QEQueueCtr)nFree); // # free entries
            QS_EQC_PRE_(margin); // margin requested
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
        QS_TIME_PRE_();          // timestamp
        QS_SIG_PRE_(e->sig);     // the signal of this event
        QS_OBJ_PRE_(me);         // this active object
        QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // poolNum & refCtr
        QS_EQC_PRE_((QEQueueCtr)FREERTOS_QUEUE_GET_FREE(me)); // # free
        QS_EQC_PRE_(0U);         // min # free entries (unknown)
    QS_END_PRE_()

    if (QEvt_getPoolNum_(e) != 0U) { // is it a pool event?
        QEvt_refCtr_inc_(e); // increment the reference counter
    }
    QF_CRIT_EXIT();

    BaseType_t err = xQueueSendToFront(
                         me->eQueue, (void const *)&e, (TickType_t)0);

    // LIFO posting to the FreeRTOS queue must succeed, see NOTE3
    QF_CRIT_ENTRY();
    Q_ASSERT_INCRIT(610, err == pdPASS);
    QF_CRIT_EXIT();
}
//............................................................................
QEvt const *QActive_get_(QActive * const me) {
    QEvt const *e;
    xQueueReceive(me->eQueue, (void *)&e, portMAX_DELAY);

    QS_CRIT_STAT
    QS_CRIT_ENTRY();
    QS_BEGIN_PRE_(QS_QF_ACTIVE_GET, me->prio)
        QS_TIME_PRE_();          // timestamp
        QS_SIG_PRE_(e->sig);     // the signal of this event
        QS_OBJ_PRE_(me);         // this active object
        QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // poolNum & refCtr
        QS_EQC_PRE_((QEQueueCtr)FREERTOS_QUEUE_GET_FREE(me)); // # free
    QS_END_PRE_()
    QS_CRIT_EXIT();

    return e;
}

//============================================================================
// The "FromISR" QP APIs for the FreeRTOS port...
bool QActive_postFromISR_(QActive * const me, QEvt const * const e,
                          uint_fast16_t const margin,
                          BaseType_t * const pxHigherPriorityTaskWoken,
                          void const * const sender)
{
    UBaseType_t uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();

    // find the number of free slots available in the queue
    uint_fast16_t const nFree = (uint_fast16_t)FREERTOS_QUEUE_GET_FREE(me);

    bool status;
    if (margin == QF_NO_MARGIN) {
        if (nFree > 0U) {
            status = true; // can post
        }
        else {
            status = false; // cannot post
            Q_ERROR_INCRIT(810); // must be able to post the event
        }
    }
    else if (nFree > margin) {
        status = true; // can post
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
            QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // poolNum & refCtr
            QS_EQC_PRE_(nFree);  // # free entries available
            QS_EQC_PRE_(0U);     // min # free entries (unknown)
        QS_END_PRE_()

        if (QEvt_getPoolNum_(e) != 0U) { // is it a pool event?
            QEvt_refCtr_inc_(e); // increment the reference counter
        }
        portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);

        BaseType_t err = xQueueSendToBackFromISR(me->eQueue,
                              (void const *)&e,
                              pxHigherPriorityTaskWoken);

        // posting to the FreeRTOS message queue must succeed
        uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
        Q_ASSERT_INCRIT(820, err == pdPASS);
        portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
    }
    else {

        QS_BEGIN_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();      // timestamp
            QS_OBJ_PRE_(sender); // the sender object
            QS_SIG_PRE_(e->sig); // the signal of the event
            QS_OBJ_PRE_(me);     // this active object (recipient)
            QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // poolNum & refCtr
            QS_EQC_PRE_(nFree);  // # free entries available
            QS_EQC_PRE_(margin); // margin requested
        QS_END_PRE_()
        portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);

        QF_gcFromISR(e); // recycle the event to avoid a leak
    }

    return status;
}
//............................................................................
void QActive_publishFromISR_(QEvt const * const e,
                          BaseType_t * const pxHigherPriorityTaskWoken,
                          void const * const sender)
{
    QSignal const sig = e->sig;

    UBaseType_t uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();

    //! @pre the published signal must be within the configured range
    Q_REQUIRE_INCRIT(500, sig < (QSignal)QActive_maxPubSignal_);
    Q_REQUIRE_INCRIT(502,
        QPSet_verify_(&QActive_subscrList_[sig].set,
                      &QActive_subscrList_[sig].set_dis));

    QS_BEGIN_PRE_(QS_QF_PUBLISH, 0U)
        QS_TIME_PRE_();          // the timestamp
        QS_OBJ_PRE_(sender);     // the sender object
        QS_SIG_PRE_(sig);        // the signal of the event
        QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_);// pool-Id & ref-Count
    QS_END_PRE_()

    // is it a dynamic event?
    if (QEvt_getPoolNum_(e) != 0U) {
        // NOTE: The reference counter of a dynamic event is incremented to
        // prevent premature recycling of the event while the multicasting
        // is still in progress. At the end of the function, the garbage
        // collector step (QF_gcFromISR()) decrements the reference counter
        // and recycles the event if the counter drops to zero. This covers
        // the case when the event was published without any subscribers.
        QEvt_refCtr_inc_(e);
    }

    // make a local, modifiable copy of the subscriber list
    QPSet subscrSet = QActive_subscrList_[sig].set;
    portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);

    if (QPSet_notEmpty(&subscrSet)) { // any subscribers?
        // the highest-prio subscriber
        uint_fast8_t p = QPSet_findMax(&subscrSet);

        // no need to lock the scheduler in the ISR context
        do { // loop over all subscribers
            // the prio of the AO must be registered with the framework
            uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
            Q_ASSERT_INCRIT(510, QActive_registry_[p] != (QActive *)0);
            portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);

            // QACTIVE_POST_FROM_ISR() asserts if the queue overflows
            QACTIVE_POST_FROM_ISR(QActive_registry_[p], e,
                                  pxHigherPriorityTaskWoken, sender);

            QPSet_remove(&subscrSet, p); // remove the handled subscriber
            if (QPSet_notEmpty(&subscrSet)) { // still more subscribers?
                p = QPSet_findMax(&subscrSet); // the highest-prio subscriber
            }
            else {
                p = 0U; // no more subscribers
            }
        } while (p != 0U);
        // no need to unlock the scheduler in the ISR context
    }

    // The following garbage collection step decrements the reference counter
    // and recycles the event if the counter drops to zero. This covers both
    // cases when the event was published with or without any subscribers.
    QF_gcFromISR(e);
}
//............................................................................
void QTimeEvt_tickFromISR_(uint_fast8_t const tickRate,
                          BaseType_t * const pxHigherPriorityTaskWoken,
                          void const * const sender)
{
    UBaseType_t uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();

    QTimeEvt *prev = &QTimeEvt_timeEvtHead_[tickRate];

    QS_BEGIN_PRE_(QS_QF_TICK, 0U)
        ++prev->ctr;
        QS_TEC_PRE_(prev->ctr); // tick ctr
        QS_U8_PRE_(tickRate);   // tick rate
    QS_END_PRE_()

    // scan the linked-list of time events at this rate...
    for (;;) {
        QTimeEvt *t = prev->next; // advance down the time evt. list

        // end of the list?
        if (t == (QTimeEvt *)0) {

            // any new time events armed since the last QTimeEvt_tick_()?
            if (QTimeEvt_timeEvtHead_[tickRate].act != (void *)0) {

                // sanity check
                Q_ASSERT_INCRIT(610, prev != (QTimeEvt *)0);
                prev->next = (QTimeEvt *)QTimeEvt_timeEvtHead_[tickRate].act;
                QTimeEvt_timeEvtHead_[tickRate].act = (void *)0;
                t = prev->next;  // switch to the new list
            }
            else {
                break; // all currently armed time evts. processed
            }
        }

        // time event scheduled for removal?
        if (t->ctr == 0U) {
            prev->next = t->next;
            // mark time event 't' as NOT linked
            t->super.refCtr_ &= (uint8_t)(~QTE_IS_LINKED);
            // do NOT advance the prev pointer
            // exit crit. section to reduce latency
            portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
        }
        else {
            --t->ctr;

            // is time evt about to expire?
            if (t->ctr == 0U) {
                QActive *act = (QActive *)t->act; // temp. for volatile

                // periodic time evt?
                if (t->interval != 0U) {
                    t->ctr = t->interval; // rearm the time event
                    prev = t; // advance to this time event
                }
                // one-shot time event: automatically disarm
                else {
                    prev->next = t->next;
                    // mark time event 't' as NOT linked
                    t->super.refCtr_ &= (uint8_t)(~QTE_IS_LINKED);
                    // do NOT advance the prev pointer

                    QS_BEGIN_PRE_(QS_QF_TIMEEVT_AUTO_DISARM, act->prio)
                        QS_OBJ_PRE_(t);        // this time event object
                        QS_OBJ_PRE_(act);      // the target AO
                        QS_U8_PRE_(tickRate);  // tick rate
                    QS_END_PRE_()
                }

                QS_BEGIN_PRE_(QS_QF_TIMEEVT_POST, act->prio)
                    QS_TIME_PRE_();            // timestamp
                    QS_OBJ_PRE_(t);            // the time event object
                    QS_SIG_PRE_(t->super.sig); // signal of time event
                    QS_OBJ_PRE_(act);          // the target AO
                    QS_U8_PRE_(tickRate);      // tick rate
                QS_END_PRE_()

                // exit critical section before posting
                portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);

                // QACTIVE_POST_FROM_ISR() asserts if the queue overflows
                QACTIVE_POST_FROM_ISR(act, &t->super,
                                      pxHigherPriorityTaskWoken,
                                      sender);
            }
            else {
                prev = t; // advance to this time event
                // exit crit. section to reduce latency
                portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
            }
        }
        // re-enter crit. section to continue
        uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
}
//............................................................................
QEvt *QF_newXFromISR_(uint_fast16_t const evtSize,
                      uint_fast16_t const margin, enum_t const sig)
{
    // find the poolNum that fits the requested event size ...
    uint_fast8_t idx;
    for (idx = 0U; idx < QF_priv_.maxPool_; ++idx) {
        if (evtSize <= QF_EPOOL_EVENT_SIZE_(QF_priv_.ePool_[idx])) {
            break;
        }
    }
    // cannot run out of registered pools
    UBaseType_t uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
    Q_REQUIRE_INCRIT(700, idx < QF_priv_.maxPool_);
    portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);

    // get e -- platform-dependent
#ifdef Q_SPY
    QEvt *e = QMPool_getFromISR(&QF_priv_.ePool_[idx],
                  ((margin != QF_NO_MARGIN) ? margin : 0U),
                  (uint_fast8_t)QS_EP_ID + idx + 1U);
#else
    QEvt *e = QMPool_getFromISR(&QF_priv_.ePool_[idx],
                      ((margin != QF_NO_MARGIN) ? margin : 0U), 0U);
#endif

    // was e allocated correctly?
    if (e != (QEvt *)0) {
        e->sig     = (QSignal)sig; // set the signal
        e->refCtr_ = 0U; // initialize the reference counter to 0
        e->evtTag_ = (uint8_t)(QEVT_MARKER | (idx + 1U)); // pool ID

#ifdef Q_SPY
        uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
        QS_BEGIN_PRE_(QS_QF_NEW,
                      (uint_fast8_t)QS_EP_ID + idx + 1U)
            QS_TIME_PRE_();         // timestamp
            QS_EVS_PRE_(evtSize);   // the size of the event
            QS_SIG_PRE_(sig);       // the signal of the event
        QS_END_PRE_()
        portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
#endif // Q_SPY
    }
    else { // event cannot be allocated
        // must tolerate bad alloc.
        uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
        Q_ASSERT_INCRIT(720, margin != QF_NO_MARGIN);
        portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);

#ifdef Q_SPY
        uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
        QS_BEGIN_PRE_(QS_QF_NEW_ATTEMPT,
                             (uint_fast8_t)QS_EP_ID + idx + 1U)
            QS_TIME_PRE_();         // timestamp
            QS_EVS_PRE_(evtSize);   // the size of the event
            QS_SIG_PRE_(sig);       // the signal of the event
        QS_END_PRE_()
        portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
#endif // Q_SPY
    }
    return e; // can't be NULL if we can't tolerate bad allocation
}
//............................................................................
void QF_gcFromISR(QEvt const * const e) {
    // is it a dynamic event?
    if (QEvt_getPoolNum_(e) != 0U) {
        UBaseType_t uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();

        // isn't this the last ref?
        if (e->refCtr_ > 1U) {
            QEvt_refCtr_dec_(e); // decrement the ref counter

            QS_BEGIN_PRE_(QS_QF_GC_ATTEMPT,
                          (uint_fast8_t)QEvt_getPoolNum_(e))
                QS_TIME_PRE_();      // timestamp
                QS_SIG_PRE_(e->sig); // the signal of the event
                QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_);//pool-Id&ref-Count
            QS_END_PRE_()

            portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
        }
        // this is the last reference to this event, recycle it
        else {
            uint_fast8_t idx = (uint_fast8_t)QEvt_getPoolNum_(e) - 1U;

            QS_BEGIN_PRE_(QS_QF_GC, (uint_fast8_t)QEvt_getPoolNum_(e))
                QS_TIME_PRE_();         // timestamp
                QS_SIG_PRE_(e->sig);    // the signal of the event
                QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_);//pool-Id&ref-Count
            QS_END_PRE_()

            // pool ID must be in range
            Q_ASSERT_INCRIT(810, idx < QF_priv_.maxPool_);

            portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);

#ifdef Q_SPY
            // cast 'const' away in (QEvt *)e is OK because it's a pool event
            QMPool_putFromISR(&QF_priv_.ePool_[idx], (QEvt *)e,
                              (uint_fast8_t)QS_EP_ID + QEvt_getPoolNum_(e));
#else
            QMPool_putFromISR(&QF_priv_.ePool_[idx], (QEvt *)e, 0U);
#endif
        }
    }
}
//............................................................................
void QMPool_putFromISR(QMPool * const me, void *block,
                        uint_fast8_t const qsId)
{
#ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
#endif

    QFreeBlock * const fb = (QFreeBlock *)block;

    UBaseType_t uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();

    // precondition:
    // - # free blocks cannot exceed the total # blocks and
    // - the block pointer must be from this pool.
    Q_REQUIRE_INCRIT(900, (me->nFree < me->nTot)
                           && (me->start <= fb) && (fb <= me->end));

    fb->next = me->free_head; // link into list
#ifndef Q_UNSAFE
    fb->next_dis = (uintptr_t)(~Q_UINTPTR_CAST_(fb->next));
#endif

    me->free_head = fb; // set as new head of the free list
    ++me->nFree;        // one more free block in this pool

    QS_BEGIN_PRE_(QS_QF_MPOOL_PUT, qsId)
        QS_TIME_PRE_();         // timestamp
        QS_OBJ_PRE_(me);        // this memory pool
        QS_MPC_PRE_(me->nFree); // the number of free blocks in the pool
    QS_END_PRE_()

    portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
}
//............................................................................
void *QMPool_getFromISR(QMPool * const me, uint_fast16_t const margin,
                        uint_fast8_t const qsId)
{
#ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
#endif

    UBaseType_t uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();

    // have more free blocks than the requested margin?
    QFreeBlock *fb;
    if (me->nFree > (QMPoolCtr)margin) {
        fb = me->free_head; // get a free block

        // the pool has some free blocks, so a free block must be available
        Q_ASSERT_INCRIT(900, fb != (QFreeBlock *)0);

        QFreeBlock * const fb_next = fb->next; // fast temporary to avoid UB

        // the free block must have integrity (duplicate inverse storage)
        Q_ASSERT_INCRIT(902, Q_UINTPTR_CAST_(fb_next)
                              == (uintptr_t)~fb->next_dis);

        // is the pool becoming empty?
        --me->nFree; // one less free block
        if (me->nFree == 0U) {
            // pool is becoming empty, so the next free block must be NULL
            Q_ASSERT_INCRIT(920, fb_next == (QFreeBlock *)0);

            me->nMin = 0U; // remember that the pool got empty
        }
        else {
            // invariant
            // The pool is not empty, so the next free-block pointer,
            // so the next free block must be in range.
            //
            // NOTE: The next free block pointer can fall out of range
            // when the client code writes past the memory block, thus
            // corrupting the next block.
            Q_ASSERT_INCRIT(930,
                (me->start <= fb_next) && (fb_next <= me->end));

            // is the number of free blocks the new minimum so far?
            if (me->nMin > me->nFree) {
                me->nMin = me->nFree; // remember the new minimum
            }
        }

        me->free_head = fb_next; // set the head to the next free block

        QS_BEGIN_PRE_(QS_QF_MPOOL_GET, qsId)
            QS_TIME_PRE_();         // timestamp
            QS_OBJ_PRE_(me);        // this memory pool
            QS_MPC_PRE_(me->nFree); // # free blocks in the pool
            QS_MPC_PRE_(me->nMin);  // min # free blocks ever in the pool
        QS_END_PRE_()
    }
    else { // don't have enough free blocks at this point
        fb = (QFreeBlock *)0;

        QS_BEGIN_PRE_(QS_QF_MPOOL_GET_ATTEMPT, qsId)
            QS_TIME_PRE_();         // timestamp
            QS_OBJ_PRE_(me);        // this memory pool
            QS_MPC_PRE_(me->nFree); // # free blocks in the pool
            QS_MPC_PRE_(margin);    // the requested margin
        QS_END_PRE_()
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);

    return fb; // return the block or NULL pointer to the caller
}

//============================================================================
// NOTE3:
// The event posting to FreeRTOS message queue occurs OUTSIDE critical section,
// which means that the remaining margin of available slots in the queue
// cannot be guaranteed. The problem is that interrupts and other tasks can
// preempt the event posting after checking the margin, but before actually
// posting the event to the queue.
//

