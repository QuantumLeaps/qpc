/**
* @file
* @brief QF/C port to FreeRTOS 10.x
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-11
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
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

#if ( configSUPPORT_STATIC_ALLOCATION == 0 )
    #error "This QP/C port to FreeRTOS requires configSUPPORT_STATIC_ALLOCATION "
#endif

#if ( configMAX_PRIORITIES < QF_MAX_ACTIVE )
    #error "FreeRTOS configMAX_PRIORITIES must not be less than QF_MAX_ACTIVE"
#endif

/* Local objects -----------------------------------------------------------*/
static void task_function(void *pvParameters); /* FreeRTOS task signature */

/*==========================================================================*/
void QF_init(void) {
    /* empty for FreeRTOS */
}
/*..........................................................................*/
int_t QF_run(void) {
    QS_CRIT_STAT_

    QF_onStartup();  /* the startup callback (configure/enable interrupts) */

    /* produce the QS_QF_RUN trace record */
    QS_BEGIN_PRE_(QS_QF_RUN, 0U)
    QS_END_PRE_()

    vTaskStartScheduler(); /* start the FreeRTOS scheduler */
    Q_ERROR_ID(110); /* the FreeRTOS scheduler should never return */
    return 0; /* dummy return to make the compiler happy */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup(); /* cleanup callback */
}
/*..........................................................................*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    TaskHandle_t thr;
    /* task name provided by the user in QF_setTaskName() or default name */
    char_t const *taskName = (me->thread.pxDummy1 != (void *)0)
                             ? (char_t const *)me->thread.pxDummy1
                             : (char_t const *)"AO";

    Q_REQUIRE_ID(200, (0U < prio)
        && (prio <= QF_MAX_ACTIVE) /* in range */
        && (qSto != (QEvt const **)0) /* queue storage must be provided */
        && (qLen > 0U)             /* queue size must be provided */
        && (stkSto != (void *)0)   /* stack storage must be provided */
        && (stkSize > 0U));        /* stack size must be provided */

    /* create the event queue for the AO */
    QEQueue_init(&me->eQueue, qSto, qLen);

    me->prio = prio;  /* save the QF priority */
    QF_add_(me);      /* make QF aware of this active object */
    QHSM_INIT(&me->super, par, me->prio); /* the top-most initial tran. */
    QS_FLUSH(); /* flush the QS trace buffer to the host */

    /* statically create the FreeRTOS task for the AO */
    thr = xTaskCreateStatic(
              &task_function,           /* the task function */
              taskName ,                /* the name of the task */
              stkSize/sizeof(portSTACK_TYPE), /* stack size */
              (void *)me,               /* the 'pvParameters' parameter */
              (UBaseType_t)(prio + tskIDLE_PRIORITY),  /* FreeRTOS priority */
              (StackType_t *)stkSto,    /* stack storage */
              &me->thread);             /* task buffer */
    Q_ENSURE_ID(210, thr != (TaskHandle_t)0); /* must be created */
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

/*==========================================================================*/
/* The "FromISR" QP APIs for the FreeRTOS port... */
#ifdef Q_SPY
bool QActive_postFromISR_(QActive * const me, QEvt const * const e,
                          uint_fast16_t const margin,
                          BaseType_t * const pxHigherPriorityTaskWoken,
                          void const * const sender)
#else
bool QActive_postFromISR_(QActive * const me, QEvt const * const e,
                          uint_fast16_t const margin,
                          BaseType_t * const pxHigherPriorityTaskWoken)
#endif
{
    QEQueueCtr nFree; /* temporary to avoid UB for volatile access */
    bool status;
    UBaseType_t uxSavedInterruptState;

    /** @pre event pointer must be valid */
    Q_REQUIRE_ID(400, e != (QEvt *)0);

    uxSavedInterruptState = taskENTER_CRITICAL_FROM_ISR();
    nFree = me->eQueue.nFree; /* get volatile into the temporary */

    if (margin == QF_NO_MARGIN) {
        if (nFree > 0U) {
            status = true; /* can post */
        }
        else {
            status = false; /* cannot post */
            Q_ERROR_ID(410); /* must be able to post the event */
        }
    }
    else if (nFree > (QEQueueCtr)margin) {
        status = true; /* can post */
    }
    else {
        status = false; /* cannot post */
    }

    if (status) { /* can post the event? */

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST, me->prio)
            QS_TIME_PRE_();       /* timestamp */
            QS_OBJ_PRE_(sender);  /* the sender object */
            QS_SIG_PRE_(e->sig);  /* the signal of the event */
            QS_OBJ_PRE_(me);      /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);   /* number of free entries */
            QS_EQC_PRE_(me->eQueue.nMin); /* min number of free entries */
        QS_END_NOCRIT_PRE_()

        /* is it a pool event? */
        if (e->poolId_ != 0U) {
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        --nFree; /* one free entry just used up */
        me->eQueue.nFree = nFree;    /* update the volatile */
        if (me->eQueue.nMin > nFree) {
            me->eQueue.nMin = nFree; /* update minimum so far */
        }

        /* empty queue? */
        if (me->eQueue.frontEvt == (QEvt *)0) {
            me->eQueue.frontEvt = e;    /* deliver event directly */
            taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);

            /* signal the event queue */
            vTaskNotifyGiveFromISR((TaskHandle_t)&me->thread,
                                   pxHigherPriorityTaskWoken);
        }
        /* queue is not empty, insert event into the ring-buffer */
        else {
            /* insert event into the ring buffer (FIFO) */
            QF_PTR_AT_(me->eQueue.ring, me->eQueue.head) = e;
            if (me->eQueue.head == 0U) { /* need to wrap head? */
                me->eQueue.head = me->eQueue.end;   /* wrap around */
            }
            --me->eQueue.head; /* advance the head (counter clockwise) */
            taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);
        }
    }
    else {

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();      /* timestamp */
            QS_OBJ_PRE_(sender); /* the sender object */
            QS_SIG_PRE_(e->sig); /* the signal of the event */
            QS_OBJ_PRE_(me);     /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);  /* number of free entries */
            QS_EQC_PRE_(margin); /* margin requested */
        QS_END_NOCRIT_PRE_()

        taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);

        QF_gcFromISR(e); /* recycle the event to avoid a leak */
    }

    return status;
}
/*..........................................................................*/
#ifdef Q_SPY
void QF_publishFromISR_(QEvt const * const e,
                        BaseType_t * const pxHigherPriorityTaskWoken,
                        void const * const sender)
#else
void QF_publishFromISR_(QEvt const * const e,
                        BaseType_t * const pxHigherPriorityTaskWoken)
#endif
{
    QPSet subscrList; /* local, modifiable copy of the subscriber list */
    UBaseType_t uxSavedInterruptState;

    /** @pre the published signal must be within the configured range */
    Q_REQUIRE_ID(500, e->sig < (QSignal)QF_maxPubSignal_);

    uxSavedInterruptState = taskENTER_CRITICAL_FROM_ISR();

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
    subscrList = QF_PTR_AT_(QF_subscrList_, e->sig);
    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);

    if (QPSet_notEmpty(&subscrList)) { /* any subscribers? */
        uint_fast8_t p;

        QPSet_findMax(&subscrList, p); /* the highest-prio subscriber */

        /* no need to lock the scheduler in the ISR context */
        do { /* loop over all subscribers */
            /* the prio of the AO must be registered with the framework */
            Q_ASSERT_ID(510, QF_active_[p] != (QActive *)0);

            /* QACTIVE_POST_FROM_ISR() asserts if the queue overflows */
            QACTIVE_POST_FROM_ISR(QF_active_[p], e,
                                  pxHigherPriorityTaskWoken, sender);

            QPSet_remove(&subscrList, p); /* remove the handled subscriber */
            if (QPSet_notEmpty(&subscrList)) { /* still more subscribers? */
                QPSet_findMax(&subscrList, p); /* highest-prio subscriber */
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
#ifdef Q_SPY
void QF_tickXFromISR_(uint_fast8_t const tickRate,
                      BaseType_t * const pxHigherPriorityTaskWoken,
                      void const * const sender)
#else
void QF_tickXFromISR_(uint_fast8_t const tickRate,
                      BaseType_t * const pxHigherPriorityTaskWoken)
#endif
{
    QTimeEvt *prev = &QF_timeEvtHead_[tickRate];
    UBaseType_t uxSavedInterruptState = taskENTER_CRITICAL_FROM_ISR();

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

            /* any new time events armed since the last run of QF_tickX_()? */
            if (QF_timeEvtHead_[tickRate].act != (void *)0) {

                /* sanity check */
                Q_ASSERT_ID(610, prev != (QTimeEvt *)0);
                prev->next = (QTimeEvt *)QF_timeEvtHead_[tickRate].act;
                QF_timeEvtHead_[tickRate].act = (void *)0;
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
            t->super.refCtr_ &= (uint8_t)(~TE_IS_LINKED);
            /* do NOT advance the prev pointer */
            /* exit crit. section to reduce latency */
            taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);
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
                    t->super.refCtr_ &= (uint8_t)(~TE_IS_LINKED);
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
                taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);

                /* QACTIVE_POST_FROM_ISR() asserts if the queue overflows */
                QACTIVE_POST_FROM_ISR(act, &t->super,
                                      pxHigherPriorityTaskWoken,
                                      sender);
            }
            else {
                prev = t;         /* advance to this time event */
                /* exit crit. section to reduce latency */
                taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);
            }
        }
        /* re-enter crit. section to continue */
        uxSavedInterruptState = taskENTER_CRITICAL_FROM_ISR();
    }
    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);
}
/*..........................................................................*/
QEvt *QF_newXFromISR_(uint_fast16_t const evtSize,
                      uint_fast16_t const margin, enum_t const sig)
{
    QEvt *e;
    uint_fast8_t idx;
#ifdef Q_SPY
    UBaseType_t uxSavedInterruptState;
#endif /* Q_SPY */

    /* find the pool index that fits the requested event size ... */
    for (idx = 0U; idx < QF_maxPool_; ++idx) {
        if (evtSize <= QF_EPOOL_EVENT_SIZE_(QF_pool_[idx])) {
            break;
        }
    }
    /* cannot run out of registered pools */
    Q_ASSERT_ID(710, idx < QF_maxPool_);


    /* get e -- platform-dependent */
#ifdef Q_SPY
    e = QMPool_getFromISR(&QF_pool_[idx],
                  ((margin != QF_NO_MARGIN) ? margin : 0U),
                  (uint_fast8_t)QS_EP_ID + idx + 1U);
#else
    e = QMPool_getFromISR(&QF_pool_[idx],
                      ((margin != QF_NO_MARGIN) ? margin : 0U), 0U);
#endif

    /* was e allocated correctly? */
    if (e != (QEvt *)0) {
        e->sig = (QSignal)sig;   /* set signal for this event */
        e->poolId_ = (uint8_t)(idx + 1U); /* store the pool ID */
        e->refCtr_ = 0U; /* set the reference counter to 0 */

#ifdef Q_SPY
        uxSavedInterruptState = taskENTER_CRITICAL_FROM_ISR();
        QS_BEGIN_PRE_(QS_QF_NEW, (uint_fast8_t)QS_EP_ID + e->poolId_)
            QS_TIME_PRE_();         /* timestamp */
            QS_EVS_PRE_(evtSize);   /* the size of the event */
            QS_SIG_PRE_(sig);       /* the signal of the event */
        QS_END_NOCRIT_PRE_()
        taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);
#endif /* Q_SPY */
    }
    /* event cannot be allocated */
    else {
        /* must tolerate bad alloc. */
        Q_ASSERT_ID(720, margin != QF_NO_MARGIN);

#ifdef Q_SPY
        uxSavedInterruptState = taskENTER_CRITICAL_FROM_ISR();
        QS_BEGIN_PRE_(QS_QF_NEW_ATTEMPT, (uint_fast8_t)QS_EP_ID + idx + 1U)
            QS_TIME_PRE_();         /* timestamp */
            QS_EVS_PRE_(evtSize);   /* the size of the event */
            QS_SIG_PRE_(sig);       /* the signal of the event */
        QS_END_NOCRIT_PRE_()
        taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);
#endif /* Q_SPY */
    }
    return e; /* can't be NULL if we can't tolerate bad allocation */
}
/*..........................................................................*/
void QF_gcFromISR(QEvt const * const e) {

    /* is it a dynamic event? */
    if (e->poolId_ != 0U) {
        UBaseType_t uxSavedInterruptState;
        uxSavedInterruptState = taskENTER_CRITICAL_FROM_ISR();

        /* isn't this the last ref? */
        if (e->refCtr_ > 1U) {
            QF_EVT_REF_CTR_DEC_(e); /* decrements the ref counter */

            QS_BEGIN_NOCRIT_PRE_(QS_QF_GC_ATTEMPT, (uint_fast8_t)e->poolId_)
                QS_TIME_PRE_();      /* timestamp */
                QS_SIG_PRE_(e->sig); /* the signal of the event */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_PRE_()

            taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);
        }
        /* this is the last reference to this event, recycle it */
        else {
            uint_fast8_t idx = (uint_fast8_t)e->poolId_ - 1U;

            QS_BEGIN_NOCRIT_PRE_(QS_QF_GC, (uint_fast8_t)e->poolId_)
                QS_TIME_PRE_();         /* timestamp */
                QS_SIG_PRE_(e->sig);    /* the signal of the event */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_PRE_()

            taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);

            /* pool ID must be in range */
            Q_ASSERT_ID(810, idx < QF_maxPool_);

#ifdef Q_SPY
            /* cast 'const' away in (QEvt *)e is OK,
             * because it's a pool event */
            QMPool_putFromISR(&QF_pool_[idx], (QEvt *)e,
                              (uint_fast8_t)QS_EP_ID + e->poolId_);
#else
            QMPool_putFromISR(&QF_pool_[idx], (QEvt *)e, 0U);
#endif
        }
    }
}
/*..........................................................................*/
void QMPool_putFromISR(QMPool * const me, void *b, uint_fast8_t const qs_id) {
    UBaseType_t uxSavedInterruptState;

    /** @pre # free blocks cannot exceed the total # blocks and
    * the block pointer must be from this pool.
    */
    Q_REQUIRE_ID(900, (me->nFree < me->nTot)
                      && QF_PTR_RANGE_(b, me->start, me->end));

    (void)qs_id; /* unused parameter (outside Q_SPY build configuration) */

    uxSavedInterruptState = taskENTER_CRITICAL_FROM_ISR();

    ((QFreeBlock *)b)->next = (QFreeBlock *)me->free_head;/* link into list */
    me->free_head = b;      /* set as new head of the free list */
    ++me->nFree;            /* one more free block in this pool */

    QS_BEGIN_NOCRIT_PRE_(QS_QF_MPOOL_PUT, qs_id)
        QS_TIME_PRE_();         /* timestamp */
        QS_OBJ_PRE_(me->start); /* the memory managed by this pool */
        QS_MPC_PRE_(me->nFree); /* # free blocks in the pool */
    QS_END_NOCRIT_PRE_()

    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);
}
/*..........................................................................*/
void *QMPool_getFromISR(QMPool * const me, uint_fast16_t const margin,
                 uint_fast8_t const qs_id)
{
    QFreeBlock *fb;
    UBaseType_t uxSavedInterruptState;

    (void)qs_id; /* unused parameter (outside Q_SPY build configuration) */

    uxSavedInterruptState = taskENTER_CRITICAL_FROM_ISR();

    /* have more free blocks than the requested margin? */
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
    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptState);

    return fb; /* return the pointer to memory block or NULL to the caller */
}

