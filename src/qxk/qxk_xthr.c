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
* @date Last updated on: 2022-06-14
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QXK preemptive kernel extended (blocking) thread functions
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qxk_pkg.h"      /* QXK package-scope internal interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS facilities for pre-defined trace records */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

/* protection against including this source file in a wrong project */
#ifndef QXK_H
    #error "Source file included in a project NOT based on the QXK kernel"
#endif /* QXK_H */

Q_DEFINE_THIS_MODULE("qxk_xthr")

/*! internal macro to encapsulate casting of pointers for MISRA deviations */
/*!
* @description
* This macro is specifically and exclusively used for downcasting pointers
* to QActive to pointers to QXThread in situations when it is known
* that such downcasting is correct.However, such pointer casting is not
* compliant with MISRA-2012-Rule 11.3(req) as well as other messages (e.g.,
* PC-Lint-Plus warning 826). Defining this specific macro for this purpose
* allows to selectively disable the warnings for this particular case.
*/
#define QXTHREAD_CAST_(ptr_) ((QXThread *)(ptr_))

/*==========================================================================*/
#ifndef Q_SPY
    static void QXThread_init_(QHsm * const me, void const * const par);
    static void QXThread_dispatch_(QHsm * const me, QEvt const * const e);
    static bool QXThread_post_(QActive * const me, QEvt const * const e,
                    uint_fast16_t const margin);
#else
    static void QXThread_init_(QHsm * const me, void const * const par,
                    uint_fast8_t const qs_id);
    static void QXThread_dispatch_(QHsm * const me, QEvt const * const e,
                    uint_fast8_t const qs_id);
    static bool QXThread_post_(QActive * const me, QEvt const * const e,
                    uint_fast16_t const margin, void const * const sender);
#endif
static void QXThread_postLIFO_(QActive * const me, QEvt const * const e);
static void QXThread_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par);

/*..........................................................................*/
void QXThread_ctor(QXThread * const me,
                   QXThreadHandler handler, uint_fast8_t tickRate)
{
    static QXThreadVtable const vtable = { /* QXThread virtual table */
        { &QXThread_init_,       /* not used in QXThread */
          &QXThread_dispatch_    /* not used in QXThread */
#ifdef Q_SPY
         ,&QHsm_getStateHandler_ /* not used in QXThread */
#endif
        },
        &QXThread_start_,
        &QXThread_post_,
        &QXThread_postLIFO_
    };
    union QHsmAttr tmp;
    tmp.thr = handler;

    QActive_ctor(&me->super, tmp.fun); /* superclass' ctor */
    me->super.super.vptr = &vtable.super; /* hook to QXThread vtable */
    me->super.super.state.act = Q_ACTION_CAST(0); /*mark as extended thread */

    /* construct the time event member added in the QXThread class */
    QTimeEvt_ctorX(&me->timeEvt, &me->super,
                   (enum_t)QXK_DELAY_SIG, tickRate);
}

/*==========================================================================*/
/* QXThread virtual function implementations... */
#ifndef Q_SPY
static void QXThread_init_(QHsm * const me, void const * const par)
#else
static void QXThread_init_(QHsm * const me, void const * const par,
                           uint_fast8_t const qs_id)
#endif
{
    (void)me; /* unused parameter */
    (void)par; /* unused parameter */
#ifdef Q_SPY
    (void)qs_id; /* unused parameter */
#endif
    Q_ERROR_ID(110);
}

/*==========================================================================*/
#ifndef Q_SPY
static void QXThread_dispatch_(QHsm * const me, QEvt const * const e)
#else
static void QXThread_dispatch_(QHsm * const me, QEvt const * const e,
                               uint_fast8_t const qs_id)
#endif
{
    (void)me; /* unused parameter */
    (void)e;  /* unused parameter */
#ifdef Q_SPY
    (void)qs_id; /* unused parameter */
#endif
    Q_ERROR_ID(120);
}

/*..........................................................................*/
/*! start QXThread private implementation
* @private @memberof QXThread
*
* @description
* Starts execution of an extended thread and registers it with the framework.
* The extended thread becomes ready-to-run immediately and is scheduled
* if the QXK is already running.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     prio    priority at which to start the extended thread
* @param[in]     qSto    pointer to the storage for the ring buffer of the
*                        event queue. This cold be NULL, if this extended
*                        thread does not use the built-in event queue.
* @param[in]     qLen    length of the event queue [in events],
*                        or zero if queue not used
* @param[in]     stkSto  pointer to the stack storage (must be provided)
* @param[in]     stkSize stack size [in bytes] (must not be zero)
* @param[in]     par     pointer to an extra parameter (might be NULL).
*
* @note
* Should be called via the macro QXTHREAD_START().
*
* @usage
* The following example shows starting an extended thread:
* @include qxk_start.c
*/
static void QXThread_start_(QActive * const me, uint_fast8_t prio,
                        QEvt const * * const qSto, uint_fast16_t const qLen,
                        void * const stkSto, uint_fast16_t const stkSize,
                        void const * const par)
{
    QF_CRIT_STAT_

    /*! @pre this function must:
    * - NOT be called from an ISR;
    * - the thread priority cannot exceed #QF_MAX_ACTIVE;
    * - the stack storage must be provided;
    * - the thread must be instantiated (see QXThread_ctor()).
    */
    Q_REQUIRE_ID(200, (!QXK_ISR_CONTEXT_()) /* don't call from an ISR! */
        && (prio <= QF_MAX_ACTIVE)
        && (stkSto != (void *)0) /* stack must be provided */
        && (stkSize != 0U)
        && (me->super.state.act == (QActionHandler)0));

    (void)par; /* unused parameter */

    /* is storage for the queue buffer provided? */
    if (qSto != (QEvt const **)0) {
        QEQueue_init(&me->eQueue, qSto, qLen);
    }

    /* extended thread constructor puts the thread handler in place of
    * the top-most initial transition 'me->super.temp.act'
    */
    QXK_stackInit_(me, me->super.temp.thr, stkSto, stkSize);

    me->prio    = (uint8_t)prio;
    me->dynPrio = (uint8_t)prio;

    /* the new thread is not blocked on any object */
    me->super.temp.obj = (QMState *)0;

    QF_add_(me); /* make QF aware of this extended thread */

    QF_CRIT_E_();
    /* extended-thread becomes ready immediately */
    QPSet_insert(&QXK_attr_.readySet, (uint_fast8_t)me->dynPrio);

    /* see if this thread needs to be scheduled in case QXK is running */
    (void)QXK_sched_();
    QF_CRIT_X_();
}

/*..........................................................................*/
#ifdef Q_SPY
/*! post to the QXThread event queue private implementation
* @private @memberof QXThread
*
* @description
* Direct event posting is the simplest asynchronous communication method
* available in QF. The following example illustrates how the Philo active
* object posts directly the HUNGRY event to the Table active object.@n
* @n
* The parameter @p margin specifies the minimum number of free slots in
* the queue that must be available for posting to succeed. The function
* returns 1 (success) if the posting succeeded (with the provided margin)
* and 0 (failure) when the posting fails.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     e      pointer to the event to be posted
* @param[in]     margin number of required free slots in the queue after
*                       posting the event. The special value #QF_NO_MARGIN
*                       means that this function will assert if posting fails.
* @param[in]     sender pointer to a sender object (used only for QS tracing).
*
* @returns
* 'true' (success) if the posting succeeded (with the provided margin) and
* 'false' (failure) when the posting fails.
*
* @note
* Should be called only via the macro QXTHREAD_POST_X().
*
* @note
* The #QF_NO_MARGIN value of the @p margin parameter is special and
* denotes situation when the post() operation is assumed to succeed
* (event delivery guarantee). An assertion fires, when the event cannot
* be delivered in this case.
*
* @note
* For compatibility with the V-table from the superclass ::QActive, the
* me-pointer is typed as pointing to QActive. However, the @p me pointer
* here actually points to the QXThread subclass. Therefore the downcast
* (QXThread *)me is always correct.
*/
static bool QXThread_post_(QActive * const me, QEvt const * const e,
                           uint_fast16_t const margin,
                           void const * const sender)
#else
static bool QXThread_post_(QActive * const me, QEvt const * const e,
                           uint_fast16_t const margin)
#endif
{
    QF_CRIT_STAT_
    QS_TEST_PROBE_DEF(&QXThread_post_)

    /* is it the private time event? */
    bool status;
    if (e == &QXTHREAD_CAST_(me)->timeEvt.super) {
        QF_CRIT_E_();
        /* the private time event is disarmed and not in any queue,
        * so it is safe to change its signal. The signal of 0 means
        * that the time event has expired.
        */
        QXTHREAD_CAST_(me)->timeEvt.super.sig = 0U;

        QXThread_unblock_(QXTHREAD_CAST_(me));
        QF_CRIT_X_();

        status = true;
    }
    /* is the event queue provided? */
    else if (me->eQueue.end != 0U) {
        QEQueueCtr nFree; /* temporary to avoid UB for volatile access */

        /*! @pre event pointer must be valid */
        Q_REQUIRE_ID(300, e != (QEvt *)0);

        QF_CRIT_E_();
        nFree = me->eQueue.nFree; /* get volatile into the temporary */

        /* test-probe#1 for faking queue overflow */
        QS_TEST_PROBE_ID(1,
            nFree = 0U;
        )

        if (margin == QF_NO_MARGIN) {
            if (nFree > 0U) {
                status = true; /* can post */
            }
            else {
                status = false; /* cannot post */
                Q_ERROR_CRIT_(310); /* must be able to post the event */
            }
        }
        else if (nFree > (QEQueueCtr)margin) {
            status = true; /* can post */
        }
        else {
            status = false; /* cannot post, but don't assert */
        }

        /* is it a dynamic event? */
        if (e->poolId_ != 0U) {
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        if (status) { /* can post the event? */

            --nFree; /* one free entry just used up */
            me->eQueue.nFree = nFree;       /* update the volatile */
            if (me->eQueue.nMin > nFree) {
                me->eQueue.nMin = nFree;    /* update minimum so far */
            }

            QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST, me->prio)
                QS_TIME_PRE_();        /* timestamp */
                QS_OBJ_PRE_(sender);   /* the sender object */
                QS_SIG_PRE_(e->sig);   /* the signal of the event */
                QS_OBJ_PRE_(me);       /* this active object (recipient) */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
                QS_EQC_PRE_(nFree);    /* number of free entries */
                QS_EQC_PRE_(me->eQueue.nMin); /* min number of free entries */
            QS_END_NOCRIT_PRE_()

            /* queue empty? */
            if (me->eQueue.frontEvt == (QEvt *)0) {
                me->eQueue.frontEvt = e; /* deliver event directly */

                /* is this thread blocked on the queue? */
                if (me->super.temp.obj
                    == QXK_PTR_CAST_(QMState*, &me->eQueue))
                {
                    (void)QXThread_teDisarm_(QXTHREAD_CAST_(me));
                    QPSet_insert(&QXK_attr_.readySet,
                                 (uint_fast8_t)me->dynPrio);
                    if (!QXK_ISR_CONTEXT_()) {
                        (void)QXK_sched_();
                    }
                }
            }
            /* queue is not empty, insert event into the ring-buffer */
            else {
                /* insert event into the ring buffer (FIFO) */
                QF_PTR_AT_(me->eQueue.ring, me->eQueue.head) = e;

                /* need to wrap the head counter? */
                if (me->eQueue.head == 0U) {
                    me->eQueue.head = me->eQueue.end;   /* wrap around */
                }
                --me->eQueue.head; /* advance the head (counter clockwise) */
            }

            QF_CRIT_X_();
        }
        else { /* cannot post the event */

            QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
                QS_TIME_PRE_();        /* timestamp */
                QS_OBJ_PRE_(sender);   /* the sender object */
                QS_SIG_PRE_(e->sig);   /* the signal of the event */
                QS_OBJ_PRE_(me);       /* this active object (recipient) */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
                QS_EQC_PRE_(nFree);    /* number of free entries */
                QS_EQC_PRE_(margin);  /* margin requested */
            QS_END_NOCRIT_PRE_()

            QF_CRIT_X_();

            QF_gc(e); /* recycle the event to avoid a leak */
        }
    }
    else { /* the queue is not available */
         QF_gc(e); /* make sure the event is not leaked */
         status = false;
         Q_ERROR_ID(320); /* this extended thread cannot accept events */
    }

    return status;
}

/*..........................................................................*/
/*! post to the QXThread event queue (LIFO) private implementation
* @private @memberof QXThread
*
* @description
* Last-In-First-Out (LIFO) policy is not supported for extened threads.
*
* @param[in] me pointer (see @ref oop)
* @param[in] e  pointer to the event to post to the queue
*
* @sa
* QActive_postLIFO_()
*/
static void QXThread_postLIFO_(QActive * const me, QEvt const * const e) {
    (void)me;
    (void)e;
    Q_ERROR_ID(410);
}

/*..........................................................................*/
QEvt const *QXThread_queueGet(uint_fast16_t const nTicks) {
    QF_CRIT_STAT_
    QF_CRIT_E_();

    QXThread * const thr = QXTHREAD_CAST_(QXK_attr_.curr);

    /*! @pre this function must:
    * - NOT be called from an ISR;
    * - be called from an extended thread;
    * - the thread must NOT be already blocked on any object.
    */
    Q_REQUIRE_ID(500, (!QXK_ISR_CONTEXT_()) /* can't block inside an ISR */
        && (thr != (QXThread *)0) /* current thread must be extended */
        && (thr->super.super.temp.obj == (QMState *)0)); /* !blocked */
    /*! @pre also: the thread must NOT be holding a scheduler lock. */
    Q_REQUIRE_ID(501, QXK_attr_.lockHolder != thr->super.prio);

    /* is the queue empty? */
    if (thr->super.eQueue.frontEvt == (QEvt *)0) {

        /* remember the blocking object (the thread's queue) */
        thr->super.super.temp.obj
            = QXK_PTR_CAST_(QMState const*, &thr->super.eQueue);

        QXThread_teArm_(thr, (QSignal)QXK_QUEUE_SIG, nTicks);
        QPSet_remove(&QXK_attr_.readySet, (uint_fast8_t)thr->super.dynPrio);
        (void)QXK_sched_();
        QF_CRIT_X_();
        QF_CRIT_EXIT_NOP(); /* BLOCK here */

        QF_CRIT_E_();
        /* the blocking object must be this queue */
        Q_ASSERT_ID(510, thr->super.super.temp.obj
                        == QXK_PTR_CAST_(QMState const*, &thr->super.eQueue));
        thr->super.super.temp.obj = (QMState *)0; /* clear */
    }

    /* is the queue not empty? */
    QEvt const *e;
    if (thr->super.eQueue.frontEvt != (QEvt *)0) {
        e = thr->super.eQueue.frontEvt; /* remove from the front */
        QEQueueCtr const nFree= thr->super.eQueue.nFree + 1U;
        thr->super.eQueue.nFree = nFree; /* update the number of free */

        /* any events in the ring buffer? */
        if (nFree <= thr->super.eQueue.end) {

            /* remove event from the tail */
            thr->super.eQueue.frontEvt =
                QF_PTR_AT_(thr->super.eQueue.ring, thr->super.eQueue.tail);
            if (thr->super.eQueue.tail == 0U) { /* need to wrap? */
                thr->super.eQueue.tail = thr->super.eQueue.end;  /* wrap */
            }
            --thr->super.eQueue.tail;

            QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_GET, thr->super.prio)
                QS_TIME_PRE_();      /* timestamp */
                QS_SIG_PRE_(e->sig); /* the signal of this event */
                QS_OBJ_PRE_(&thr->super); /* this active object */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
                QS_EQC_PRE_(nFree);  /* number of free entries */
            QS_END_NOCRIT_PRE_()
        }
        else {
            thr->super.eQueue.frontEvt = (QEvt *)0; /* empty queue */

            /* all entries in the queue must be free (+1 for fronEvt) */
            Q_ASSERT_ID(520, nFree == (thr->super.eQueue.end + 1U));

            QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_GET_LAST, thr->super.prio)
                QS_TIME_PRE_();      /* timestamp */
                QS_SIG_PRE_(e->sig); /* the signal of this event */
                QS_OBJ_PRE_(&thr->super); /* this active object */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_PRE_()
        }
    }
    else { /* the queue is still empty -- the timeout must have fired */
         e = (QEvt *)0;
    }
    QF_CRIT_X_();

    return e;
}

/*..........................................................................*/
/*! block QXThread private implementation
* @private @memberof QXThread
*
* @description
* Internal implementation of blocking the given extended thread.
*
* @note
* Must be called from within a critical section
*/
void QXThread_block_(QXThread const * const me) {
    /*! @pre the thread holding the lock cannot block! */
    Q_REQUIRE_ID(600, (QXK_attr_.lockHolder != me->super.prio));

    QPSet_remove(&QXK_attr_.readySet, (uint_fast8_t)me->super.dynPrio);
    (void)QXK_sched_();
}

/*..........................................................................*/
/*! unblock QXThread private implementation
* @private @memberof QXThread
*
* @description
* Internal implementation of un-blocking the given extended thread.
*
* @note
* must be called from within a critical section
*/
void QXThread_unblock_(QXThread const * const me) {
    QPSet_insert(&QXK_attr_.readySet, (uint_fast8_t)me->super.dynPrio);
    if ((!QXK_ISR_CONTEXT_()) /* not inside ISR? */
        && (QF_active_[0] != (QActive *)0))  /* kernel started? */
    {
        (void)QXK_sched_();
    }
}

/*..........................................................................*/
/*! arm internal time event private implementation
* @private @memberof QXThread
*
* @description
* Internal implementation of arming the private time event for a given
* timeout at a given system tick rate.
*
* @note
* Must be called from within a critical section
*/
void QXThread_teArm_(QXThread * const me, QSignal sig,
                     uint_fast16_t const nTicks)
{
    /*! @pre the time event must be unused */
    Q_REQUIRE_ID(700, me->timeEvt.ctr == 0U);

    me->timeEvt.super.sig = sig;

    if (nTicks != QXTHREAD_NO_TIMEOUT) {
        me->timeEvt.ctr = (QTimeEvtCtr)nTicks;
        me->timeEvt.interval = 0U;

        /* is the time event unlinked?
        * NOTE: For the duration of a single clock tick of the specified tick
        * rate a time event can be disarmed and yet still linked in the list,
        * because un-linking is performed exclusively in QF_tickX().
        */
        if ((me->timeEvt.super.refCtr_ & QTE_IS_LINKED) == 0U) {
            uint_fast8_t const tickRate
                 = ((uint_fast8_t)me->timeEvt.super.refCtr_ & QTE_TICK_RATE);
            Q_ASSERT_ID(710, tickRate < QF_MAX_TICK_RATE);

            me->timeEvt.super.refCtr_ |= QTE_IS_LINKED;

            /* The time event is initially inserted into the separate
            * "freshly armed" list based on QF_timeEvtHead_[tickRate].act.
            * Only later, inside the QF_tickX() function, the "freshly armed"
            * list is appended to the main list of armed time events based on
            * QF_timeEvtHead_[tickRate].next. Again, this is to keep any
            * changes to the main list exclusively inside QF_tickX().
            */
            me->timeEvt.next
                = QXK_PTR_CAST_(QTimeEvt*, QF_timeEvtHead_[tickRate].act);
            QF_timeEvtHead_[tickRate].act = &me->timeEvt;
        }
    }
}

/*..........................................................................*/
/*! disarm internal time event private implementation
* @private @memberof QXThread
*
* @description
* Internal implementation of disarming the private time event.
*
* @note
* Must be called from within a critical section
*/
bool QXThread_teDisarm_(QXThread * const me) {
    bool wasArmed;
    /* is the time evt running? */
    if (me->timeEvt.ctr != 0U) {
        wasArmed = true;
        me->timeEvt.ctr = 0U; /* schedule removal from list */
    }
    /* the time event was already automatically disarmed */
    else {
        wasArmed = false;
    }
    return wasArmed;
}

/*..........................................................................*/
bool QXThread_delay(uint_fast16_t const nTicks) {
    QF_CRIT_STAT_
    QF_CRIT_E_();

    QXThread * const thr = QXTHREAD_CAST_(QXK_attr_.curr);

    /*! @pre this function must:
    * - NOT be called from an ISR;
    * - number of ticks cannot be zero
    * - be called from an extended thread;
    * - the thread must NOT be already blocked on any object.
    */
    Q_REQUIRE_ID(800, (!QXK_ISR_CONTEXT_()) /* can't block inside an ISR */
        && (nTicks != 0U) /* number of ticks cannot be zero */
        && (thr != (QXThread *)0) /* current thread must be extended */
        && (thr->super.super.temp.obj == (QMState *)0)); /* !blocked */
    /*! @pre also: the thread must NOT be holding a scheduler lock. */
    Q_REQUIRE_ID(801, QXK_attr_.lockHolder != thr->super.prio);

    /* remember the blocking object */
    thr->super.super.temp.obj = QXK_PTR_CAST_(QMState const*, &thr->timeEvt);
    QXThread_teArm_(thr, (QSignal)QXK_DELAY_SIG, nTicks);
    QXThread_block_(thr);
    QF_CRIT_X_();
    QF_CRIT_EXIT_NOP(); /* BLOCK here */

    QF_CRIT_E_();
    /* the blocking object must be the time event */
    Q_ENSURE_ID(890, thr->super.super.temp.obj
                          == QXK_PTR_CAST_(QMState const*, &thr->timeEvt));
    thr->super.super.temp.obj =  (QMState *)0; /* clear */
    QF_CRIT_X_();

    /* signal of zero means that the time event was posted without
    * being canceled.
    */
    return thr->timeEvt.super.sig == 0U;
}

/*..........................................................................*/
bool QXThread_delayCancel(QXThread * const me) {
    QF_CRIT_STAT_
    QF_CRIT_E_();

    bool wasArmed;
    if (me->super.super.temp.obj == QXK_PTR_CAST_(QMState*, &me->timeEvt)) {
        wasArmed = QXThread_teDisarm_(me);
        QXThread_unblock_(me);
    }
    else {
        wasArmed = false;
    }
    QF_CRIT_X_();

    return wasArmed;
}

/*..........................................................................*/
/*! return from QXThread function private implementation
* @private @memberof QXThread
*
* @description
* Called when the extended-thread handler function returns.
*
* @note
* Most thread handler functions are structured as endless loops that never
* return. But it is also possible to structure threads as one-shot functions
* that perform their job and return. In that case this function peforms
* cleanup after the thread.
*/
void QXK_threadRet_(void) {
    QF_CRIT_STAT_
    QF_CRIT_E_();

    QXThread const * const thr = QXTHREAD_CAST_(QXK_attr_.curr);

    /*! @pre this function must:
    * - NOT be called from an ISR;
    * - be called from an extended thread;
    */
    Q_REQUIRE_ID(900, (!QXK_ISR_CONTEXT_()) /* can't be in the ISR context */
        && (thr != (QXThread *)0)); /* current thread must be extended */
    /*! @pre also: the thread must NOT be holding a scheduler lock. */
    Q_REQUIRE_ID(901, QXK_attr_.lockHolder != thr->super.prio);

    uint_fast8_t const p = (uint_fast8_t)thr->super.prio;

    /* remove this thread from the QF */
    QF_active_[p] = (QActive *)0;
    QPSet_remove(&QXK_attr_.readySet, p);
    (void)QXK_sched_();
    QF_CRIT_X_();
}
