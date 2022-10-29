/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-08-29
* @version Last updated for: @ref qpc_7_1_1
*
* @file
* @brief "Experimental" QF/C port to Espressif ESP-IDF (version 4.x)
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#include "qassert.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS package-scope internal interface */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_port")
//static const char *TAG = "qf_port";

#if ( configSUPPORT_STATIC_ALLOCATION == 0 )
    #error "This QP/C port to FreeRTOS requires configSUPPORT_STATIC_ALLOCATION "
#endif

#if ( configMAX_PRIORITIES < QF_MAX_ACTIVE )
    #error "FreeRTOS configMAX_PRIORITIES must not be less than QF_MAX_ACTIVE"
#endif

/* Global objects ----------------------------------------------------------*/
PRIVILEGED_DATA portMUX_TYPE QF_esp32mux = portMUX_INITIALIZER_UNLOCKED;

/* Local objects -----------------------------------------------------------*/
static void task_function(void *pvParameters); /* FreeRTOS task signature */

/* The following macro provides the number of free slots in the FreeRTOS
* queue.
*
* NOTE1:
* The official FreeRTOS API uxQueueSpacesAvailable() is not used
* here, because that API uses task-level critical section internally.
* Instead, the free slots calculation happens here in already
* established critical section. Unfortunately, the bizarre "information
* obfuscating" policy of FreeRTOS (incorrectly called "information
* hiding") forces the use of the StaticQueue_t with "dummy" members.
* This could potentially break in the future releases of FreeRTOS.
*
* Currently, the correspondence between xQUEUE and StaticQueue_t
* is as follows (see queue.c and FreeRTOS.h, respectively):
*
* xQUEUE.uxMessagesWaiting == StaticQueue_t.uxDummy4[0];
* xQUEUE.uxLength          == StaticQueue_t.uxDummy4[1];
*/
#define FREERTOS_QUEUE_GET_FREE(me_) \
    ((me_)->osObject.uxDummy4[1] - (me_)->osObject.uxDummy4[0])

/*==========================================================================*/
void QF_init(void) {
    /* empty for esp-idf */
}
/*..........................................................................*/
int_t QF_run(void) {
    QF_onStartup(); /* the startup callback (configure/enable interrupts) */

    return 0; /* dummy return to make the compiler happy */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup(); /* cleanup callback */
}

/*..........................................................................*/
static void task_function(void *pvParameters) { /* FreeRTOS task signature */
    QActive *act = (QActive *)pvParameters;

    /* event-loop */
    for (;;) { /* for-ever */
        QEvt const *e = QActive_get_(act);
        QHSM_DISPATCH(&act->super, e, act->prio);
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }
}
/*..........................................................................*/
void QActive_start_(QActive * const me, QPrioSpec const prioSpec,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    Q_REQUIRE_ID(200,
        (qSto != (QEvt const **)0) /* queue storage must be provided */
        && (qLen > 0U)             /* queue size must be provided */
        && (stkSto != (void *)0)   /* stack storage must be provided */
        && (stkSize > 0U));        /* stack size must be provided */

    /* create FreeRTOS message queue */
    me->eQueue = xQueueCreateStatic(
            (UBaseType_t)qLen,     /* length of the queue */
            (UBaseType_t)sizeof(QEvt *), /* element size */
            (uint8_t *)qSto,       /* storage buffer */
            &me->osObject);        /* static queue buffer */
    Q_ASSERT_ID(210, me->eQueue != (QueueHandle_t)0);

    me->prio  = (uint8_t)(prioSpec & 0xFFU); /* QF-priority of the AO */
    me->pthre = (uint8_t)(prioSpec >> 8U);   /* preemption-threshold */
    QActive_register_(me); /* register this AO */

    QHSM_INIT(&me->super, par, me->prio); /* the top-most initial tran. */
    QS_FLUSH(); /* flush the QS trace buffer to the host */

    /* task name provided by the user in QActive_setAttr() or default name */
    char const *taskName = (me->thread.pxDummy1 != (void *)0)
                             ? (char const *)me->thread.pxDummy1
                             : (char const *)"AO";

    /* statically create the FreeRTOS task for the AO */
     Q_ALLEGE_ID(220,
         (TaskHandle_t)0 != xTaskCreateStaticPinnedToCore(
              &task_function,           /* the task function */
              taskName ,                /* the name of the task */
              stkSize/sizeof(portSTACK_TYPE), /* stack length */
              (void *)me,               /* the 'pvParameters' parameter */
              FREERTOS_TASK_PRIO(me->prio), /* FreeRTOS priority */
              (StackType_t *)stkSto,    /* stack storage */
              &me->thread,              /* task buffer */
              QPC_CPU_NUM));            /* CPU number */
}
/*..........................................................................*/
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    /* this function must be called before QACTIVE_START(),
    * which implies that me->thread.pxDummy1 must not be used yet;
    */
    Q_REQUIRE_ID(300, me->thread.pxDummy1 == (void *)0);
    switch (attr1) {
        case TASK_NAME_ATTR:
            /* temporarily store the name */
            me->thread.pxDummy1 = (void *)attr2; /* cast 'const' away */
            break;
        /* ... */
    }
}

/*==========================================================================*/
bool IRAM_ATTR QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
{
    QF_CRIT_STAT_
    QF_CRIT_E_();

    /* find out the number of free slots available in the queue */
    uint_fast16_t const nFree = (uint_fast16_t)FREERTOS_QUEUE_GET_FREE(me);

    bool status;
    if (margin == QF_NO_MARGIN) {
        if (nFree > 0U) {
            status = true; /* can post */
        }
        else {
            status = false; /* cannot post */
            Q_ERROR_ID(510); /* must be able to post the event */
        }
    }
    else if (nFree > margin) {
        status = true; /* can post */
    }
    else {
        status = false; /* cannot post */
    }

    if (status) { /* can post the event? */

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST, me->prio)
            QS_TIME_PRE_();      /* timestamp */
            QS_OBJ_PRE_(sender); /* the sender object */
            QS_SIG_PRE_(e->sig); /* the signal of the event */
            QS_OBJ_PRE_(me);     /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_((QEQueueCtr)nFree); /* # free entries available */
            QS_EQC_PRE_(0U);     /* min # free entries (unknown) */
        QS_END_NOCRIT_PRE_()

        if (e->poolId_ != 0U) { /* is it a pool event? */
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        QF_CRIT_X_();

        /* posting to the FreeRTOS message queue must succeed, see NOTE3 */
        Q_ALLEGE_ID(520,
            xQueueSend(me->eQueue, (void const *)&e, portMAX_DELAY)
            == pdPASS);
    }
    else {

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();      /* timestamp */
            QS_OBJ_PRE_(sender); /* the sender object */
            QS_SIG_PRE_(e->sig); /* the signal of the event */
            QS_OBJ_PRE_(me);     /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_((QEQueueCtr)nFree); /* # free entries available */
            QS_EQC_PRE_(margin); /* margin requested */
        QS_END_NOCRIT_PRE_()

        QF_CRIT_X_();
   }

    return status;
}
/*..........................................................................*/
void IRAM_ATTR QActive_postLIFO_(QActive * const me, QEvt const * const e) {
    QF_CRIT_STAT_
    QF_CRIT_E_();

    QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST_LIFO, me->prio)
        QS_TIME_PRE_();          /* timestamp */
        QS_SIG_PRE_(e->sig);     /* the signal of this event */
        QS_OBJ_PRE_(me);         /* this active object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_EQC_PRE_((QEQueueCtr)FREERTOS_QUEUE_GET_FREE(me)); /* # free */
        QS_EQC_PRE_(0U);         /* min # free entries (unknown) */
    QS_END_NOCRIT_PRE_()

    if (e->poolId_ != 0U) { /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
    }

    QF_CRIT_X_();

    /* LIFO posting to the FreeRTOS queue must succeed */
    Q_ALLEGE_ID(610,
        xQueueSendToBack(me->eQueue, (void const *)&e, portMAX_DELAY)
            == pdPASS);
}
/*..........................................................................*/
QEvt const *QActive_get_(QActive * const me) {
    QEvt const *e;
    xQueueReceive(me->eQueue, (void *)&e, portMAX_DELAY);

    QS_CRIT_STAT_
    QS_BEGIN_PRE_(QS_QF_ACTIVE_GET, me->prio)
        QS_TIME_PRE_();          /* timestamp */
        QS_SIG_PRE_(e->sig);     /* the signal of this event */
        QS_OBJ_PRE_(me);         /* this active object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_EQC_PRE_((QEQueueCtr)FREERTOS_QUEUE_GET_FREE(me)); /* # free */
    QS_END_PRE_()

    return e;
}

/*==========================================================================*/
/* The "FromISR" QP APIs for the FreeRTOS port... */
bool IRAM_ATTR QActive_postFromISR_(QActive * const me, QEvt const * const e,
                          uint_fast16_t const margin,
                          BaseType_t * const pxHigherPriorityTaskWoken,
                          void const * const sender)
{
    portENTER_CRITICAL_ISR(&QF_esp32mux);

    /* find out the number of free slots in the queue */
    uint_fast16_t const nFree = (uint_fast16_t)FREERTOS_QUEUE_GET_FREE(me);

    bool status;
    if (margin == QF_NO_MARGIN) {
        if (nFree > 0U) {
            status = true; /* can post */
        }
        else {
            status = false; /* cannot post */
            Q_ERROR_ID(810); /* must be able to post the event */
        }
    }
    else if (nFree > margin) {
        status = true; /* can post */
    }
    else {
        status = false; /* cannot post */
    }

    if (status) { /* can post the event? */

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST, me->prio)
            QS_TIME_PRE_();      /* timestamp */
            QS_OBJ_PRE_(sender); /* the sender object */
            QS_SIG_PRE_(e->sig); /* the signal of the event */
            QS_OBJ_PRE_(me);     /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);  /* # free entries available */
            QS_EQC_PRE_(0U);     /* min # free entries (unknown) */
        QS_END_NOCRIT_PRE_()

        if (e->poolId_ != 0U) { /* is it a pool event? */
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        portEXIT_CRITICAL_ISR(&QF_esp32mux);

        /* posting to the FreeRTOS message queue must succeed */
        Q_ALLEGE_ID(820,
            xQueueSendFromISR(me->eQueue, (void const *)&e,
                              pxHigherPriorityTaskWoken)
            == pdTRUE);
    }
    else {

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();      /* timestamp */
            QS_OBJ_PRE_(sender); /* the sender object */
            QS_SIG_PRE_(e->sig); /* the signal of the event */
            QS_OBJ_PRE_(me);     /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);  /* # free entries available */
            QS_EQC_PRE_(margin); /* margin requested */
        QS_END_NOCRIT_PRE_()

        portEXIT_CRITICAL_ISR(&QF_esp32mux);

        QF_gcFromISR(e); /* recycle the event to avoid a leak */
    }

    return status;
}
/*..........................................................................*/
void IRAM_ATTR QActive_publishFromISR_(QEvt const * const e,
                             BaseType_t * const pxHigherPriorityTaskWoken,
                             void const * const sender)
{
    /** @pre the published signal must be within the configured range */
    Q_REQUIRE_ID(500, e->sig < (QSignal)QActive_maxPubSignal_);

    portENTER_CRITICAL_ISR(&QF_esp32mux);

    QS_BEGIN_NOCRIT_PRE_(QS_QF_PUBLISH, 0U)
        QS_TIME_PRE_();          /* the timestamp */
        QS_OBJ_PRE_(sender);     /* the sender object */
        QS_SIG_PRE_(e->sig);     /* the signal of the event */
        QS_2U8_PRE_(e->poolId_, e->refCtr_);/* pool-Id & ref-Count */
    QS_END_NOCRIT_PRE_()

    /* is it a dynamic event? */
    if (e->poolId_ != 0U) {
        /* NOTE: The reference counter of a dynamic event is incremented to
        * prevent premature recycling of the event while the multicasting
        * is still in progress. At the end of the function, the garbage
        * collector step (QF_gcFromISR()) decrements the reference counter and
        * recycles the event if the counter drops to zero. This covers the
        * case when the event was published without any subscribers.
        */
        QF_EVT_REF_CTR_INC_(e);
    }

    /* make a local, modifiable copy of the subscriber list */
    QPSet subscrList = QActive_subscrList_[e->sig];
    portEXIT_CRITICAL_ISR(&QF_esp32mux);

    if (QPSet_notEmpty(&subscrList)) { /* any subscribers? */
        /* the highest-prio subscriber */
        uint_fast8_t p = QPSet_findMax(&subscrList);

        /* no need to lock the scheduler in the ISR context */
        do { /* loop over all subscribers */
            /* the prio of the AO must be registered with the framework */
            Q_ASSERT_ID(510, QActive_registry_[p] != (QActive *)0);

            /* QACTIVE_POST_FROM_ISR() asserts if the queue overflows */
            QACTIVE_POST_FROM_ISR(QActive_registry_[p], e,
                                  pxHigherPriorityTaskWoken, sender);

            QPSet_remove(&subscrList, p); /* remove the handled subscriber */
            if (QPSet_notEmpty(&subscrList)) { /* still more subscribers? */
                /* highest-prio subscriber */
                p = QPSet_findMax(&subscrList);
            }
            else {
                p = 0U; /* no more subscribers */
            }
        } while (p != 0U);
        /* no need to unlock the scheduler in the ISR context */
    }

    /* The following garbage collection step decrements the reference counter
    * and recycles the event if the counter drops to zero. This covers both
    * cases when the event was published with or without any subscribers.
    */
    QF_gcFromISR(e);
}
/*..........................................................................*/
void IRAM_ATTR QTimeEvt_tickFromISR_(uint_fast8_t const tickRate,
                      BaseType_t * const pxHigherPriorityTaskWoken,
                      void const * const sender)
{
    QTimeEvt *prev = &QTimeEvt_timeEvtHead_[tickRate];

    portENTER_CRITICAL_ISR(&QF_esp32mux);

    QS_BEGIN_NOCRIT_PRE_(QS_QF_TICK, 0U)
        ++prev->ctr;
        QS_TEC_PRE_(prev->ctr); /* tick ctr */
        QS_U8_PRE_(tickRate);   /* tick rate */
    QS_END_NOCRIT_PRE_()

    /* scan the linked-list of time events at this rate... */
    for (;;) {
        QTimeEvt *t = prev->next;  /* advance down the time evt. list */

        /* end of the list? */
        if (t == (QTimeEvt *)0) {

            /* any new time events armed since the last QTimeEvt_tick_()? */
            if (QTimeEvt_timeEvtHead_[tickRate].act != (void *)0) {

                /* sanity check */
                Q_ASSERT_ID(610, prev != (QTimeEvt *)0);
                prev->next = (QTimeEvt *)QTimeEvt_timeEvtHead_[tickRate].act;
                QTimeEvt_timeEvtHead_[tickRate].act = (void *)0;
                t = prev->next;  /* switch to the new list */
            }
            else {
                break; /* all currently armed time evts. processed */
            }
        }

        /* time event scheduled for removal? */
        if (t->ctr == 0U) {
            prev->next = t->next;
            /* mark time event 't' as NOT linked */
            t->super.refCtr_ &= (uint8_t)(~QTE_IS_LINKED);
            /* do NOT advance the prev pointer */
            /* exit crit. section to reduce latency */
            portEXIT_CRITICAL_ISR(&QF_esp32mux);
        }
        else {
            --t->ctr;

            /* is time event about to expire? */
            if (t->ctr == (QTimeEvtCtr)0) {
                QActive *act = (QActive *)t->act; /* temp. for volatile */

                /* periodic time evt? */
                if (t->interval != 0U) {
                    t->ctr = t->interval; /* rearm the time event */
                    prev = t; /* advance to this time event */
                }
                /* one-shot time event: automatically disarm */
                else {
                    prev->next = t->next;
                    /* mark time event 't' as NOT linked */
                    t->super.refCtr_ &= (uint8_t)(~QTE_IS_LINKED);
                    /* do NOT advance the prev pointer */

                    QS_BEGIN_NOCRIT_PRE_(QS_QF_TIMEEVT_AUTO_DISARM, act->prio)
                        QS_OBJ_PRE_(t);        /* this time event object */
                        QS_OBJ_PRE_(act);      /* the target AO */
                        QS_U8_PRE_(tickRate);  /* tick rate */
                    QS_END_NOCRIT_PRE_()
                }

                QS_BEGIN_NOCRIT_PRE_(QS_QF_TIMEEVT_POST, act->prio)
                    QS_TIME_PRE_();            /* timestamp */
                    QS_OBJ_PRE_(t);            /* the time event object */
                    QS_SIG_PRE_(t->super.sig); /* signal of this time event */
                    QS_OBJ_PRE_(act);          /* the target AO */
                    QS_U8_PRE_(tickRate);      /* tick rate */
                QS_END_NOCRIT_PRE_()

                /* exit critical section before posting */
                portEXIT_CRITICAL_ISR(&QF_esp32mux);

                /* QACTIVE_POST_FROM_ISR() asserts if the queue overflows */
                QACTIVE_POST_FROM_ISR(act, &t->super,
                                      pxHigherPriorityTaskWoken,
                                      sender);
            }
            else {
                prev = t;         /* advance to this time event */
                /* exit crit. section to reduce latency */
                portEXIT_CRITICAL_ISR(&QF_esp32mux);
            }
        }
        /* re-enter crit. section to continue */
        portENTER_CRITICAL_ISR(&QF_esp32mux);
    }
    portEXIT_CRITICAL_ISR(&QF_esp32mux);
}
/*..........................................................................*/
QEvt IRAM_ATTR *QF_newXFromISR_(uint_fast16_t const evtSize,
                      uint_fast16_t const margin, enum_t const sig)
{
    /* find the pool index that fits the requested event size ... */
    uint_fast8_t idx;
    for (idx = 0U; idx < QF_maxPool_; ++idx) {
        if (evtSize <= QF_EPOOL_EVENT_SIZE_(QF_ePool_[idx])) {
            break;
        }
    }
    /* cannot run out of registered pools */
    Q_ASSERT_ID(710, idx < QF_maxPool_);

    /* get e -- platform-dependent */
#ifdef Q_SPY
    QEvt *e = QMPool_getFromISR(&QF_ePool_[idx],
                  ((margin != QF_NO_MARGIN) ? margin : 0U),
                  (uint_fast8_t)QS_EP_ID + idx + 1U);
#else
    QEvt *e = QMPool_getFromISR(&QF_ePool_[idx],
                      ((margin != QF_NO_MARGIN) ? margin : 0U), 0U);
#endif

    /* was e allocated correctly? */
    if (e != (QEvt *)0) {
        e->sig = (QSignal)sig;   /* set signal for this event */
        e->poolId_ = (uint8_t)(idx + 1U); /* store the pool ID */
        e->refCtr_ = 0U; /* set the reference counter to 0 */

#ifdef Q_SPY
        portENTER_CRITICAL_ISR(&QF_esp32mux);
        QS_BEGIN_PRE_(QS_QF_NEW, (uint_fast8_t)QS_EP_ID + e->poolId_)
            QS_TIME_PRE_();         /* timestamp */
            QS_EVS_PRE_(evtSize);   /* the size of the event */
            QS_SIG_PRE_(sig);       /* the signal of the event */
        QS_END_NOCRIT_PRE_()
        portEXIT_CRITICAL_ISR(&QF_esp32mux);
#endif /* Q_SPY */
    }
    /* event cannot be allocated */
    else {
        /* must tolerate bad alloc. */
        Q_ASSERT_ID(720, margin != QF_NO_MARGIN);

#ifdef Q_SPY
        portENTER_CRITICAL_ISR(&QF_esp32mux);
        QS_BEGIN_PRE_(QS_QF_NEW_ATTEMPT, (uint_fast8_t)QS_EP_ID + idx + 1U)
            QS_TIME_PRE_();         /* timestamp */
            QS_EVS_PRE_(evtSize);   /* the size of the event */
            QS_SIG_PRE_(sig);       /* the signal of the event */
        QS_END_NOCRIT_PRE_()
        portEXIT_CRITICAL_ISR(&QF_esp32mux);
#endif /* Q_SPY */
    }
    return e; /* can't be NULL if we can't tolerate bad allocation */
}
/*..........................................................................*/
void IRAM_ATTR QF_gcFromISR(QEvt const * const e) {

    /* is it a dynamic event? */
    if (e->poolId_ != 0U) {
        portENTER_CRITICAL_ISR(&QF_esp32mux);

        /* isn't this the last ref? */
        if (e->refCtr_ > 1U) {
            QF_EVT_REF_CTR_DEC_(e); /* decrements the ref counter */

            QS_BEGIN_NOCRIT_PRE_(QS_QF_GC_ATTEMPT, (uint_fast8_t)e->poolId_)
                QS_TIME_PRE_();      /* timestamp */
                QS_SIG_PRE_(e->sig); /* the signal of the event */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_PRE_()

            portEXIT_CRITICAL_ISR(&QF_esp32mux);
        }
        /* this is the last reference to this event, recycle it */
        else {
            uint_fast8_t idx = (uint_fast8_t)e->poolId_ - 1U;

            QS_BEGIN_NOCRIT_PRE_(QS_QF_GC, (uint_fast8_t)e->poolId_)
                QS_TIME_PRE_();         /* timestamp */
                QS_SIG_PRE_(e->sig);    /* the signal of the event */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_PRE_()

            portEXIT_CRITICAL_ISR(&QF_esp32mux);

            /* pool ID must be in range */
            Q_ASSERT_ID(810, idx < QF_maxPool_);

#ifdef Q_SPY
            /* cast 'const' away in (QEvt *)e is OK,
             * because it's a pool event */
            QMPool_putFromISR(&QF_ePool_[idx], (QEvt *)e,
                              (uint_fast8_t)QS_EP_ID + e->poolId_);
#else
            QMPool_putFromISR(&QF_ePool_[idx], (QEvt *)e, 0U);
#endif
        }
    }
}
/*..........................................................................*/
void IRAM_ATTR QMPool_putFromISR(QMPool * const me, void *b,
                                 uint_fast8_t const qs_id)
{
    /** @pre # free blocks cannot exceed the total # blocks and
    * the block pointer must be from this pool.
    */
    Q_REQUIRE_ID(900, (me->nFree < me->nTot)
                      && QF_PTR_RANGE_(b, me->start, me->end));

    (void)qs_id; /* unused parameter (outside Q_SPY build configuration) */

    portENTER_CRITICAL_ISR(&QF_esp32mux);

    ((QFreeBlock *)b)->next = (QFreeBlock *)me->free_head;/* link into list */
    me->free_head = b;      /* set as new head of the free list */
    ++me->nFree;            /* one more free block in this pool */

    QS_BEGIN_NOCRIT_PRE_(QS_QF_MPOOL_PUT, qs_id)
        QS_TIME_PRE_();         /* timestamp */
        QS_OBJ_PRE_(me->start); /* the memory managed by this pool */
        QS_MPC_PRE_(me->nFree); /* # free blocks in the pool */
    QS_END_NOCRIT_PRE_()

    portEXIT_CRITICAL_ISR(&QF_esp32mux);
}
/*..........................................................................*/
void *QMPool_getFromISR(QMPool * const me, uint_fast16_t const margin,
                        uint_fast8_t const qs_id)
{
    (void)qs_id; /* unused parameter (outside Q_SPY build configuration) */

    portENTER_CRITICAL_ISR(&QF_esp32mux);

    /* have more free blocks than the requested margin? */
    QFreeBlock *fb;
    if (me->nFree > (QMPoolCtr)margin) {
        void *fb_next;
        fb = (QFreeBlock *)me->free_head; /* get a free block */

        /* the pool has some free blocks, so a free block must be available */
        Q_ASSERT_ID(910, fb != (QFreeBlock *)0);

        fb_next = fb->next; /* put volatile to a temporary to avoid UB */

        /* is the pool becoming empty? */
        --me->nFree; /* one less free block */
        if (me->nFree == (QMPoolCtr)0) {
            /* pool is becoming empty, so the next free block must be NULL */
            Q_ASSERT_ID(920, fb_next == (QFreeBlock *)0);

            me->nMin = (QMPoolCtr)0; /* remember that the pool got empty */
        }
        else {
            /* pool is not empty, so the next free block must be in range
            *
            * NOTE: the next free block pointer can fall out of range
            * when the client code writes past the memory block, thus
            * corrupting the next block.
            */
            Q_ASSERT_ID(930, QF_PTR_RANGE_(fb_next, me->start, me->end));

            /* is the number of free blocks the new minimum so far? */
            if (me->nMin > me->nFree) {
                me->nMin = me->nFree; /* remember the new minimum */
            }
        }

        me->free_head = fb_next; /* set the head to the next free block */

        QS_BEGIN_NOCRIT_PRE_(QS_QF_MPOOL_GET, qs_id)
            QS_TIME_PRE_();         /* timestamp */
            QS_OBJ_PRE_(me->start); /* the memory managed by this pool */
            QS_MPC_PRE_(me->nFree); /* # free blocks in the pool */
            QS_MPC_PRE_(me->nMin);  /* min # free blocks ever in the pool */
        QS_END_NOCRIT_PRE_()

    }
    /* don't have enough free blocks at this point */
    else {
        fb = (QFreeBlock *)0;

        QS_BEGIN_NOCRIT_PRE_(QS_QF_MPOOL_GET_ATTEMPT, qs_id)
            QS_TIME_PRE_();         /* timestamp */
            QS_OBJ_PRE_(me->start); /* the memory managed by this pool */
            QS_MPC_PRE_(me->nFree); /* # free blocks in the pool */
            QS_MPC_PRE_(margin);    /* the requested margin */
        QS_END_NOCRIT_PRE_()
    }
    portEXIT_CRITICAL_ISR(&QF_esp32mux);

    return fb; /* return the pointer to memory block or NULL to the caller */
}

