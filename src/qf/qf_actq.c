/**
* @file
* @brief ::QActive native queue operations (based on ::QEQueue)
*
* @note
* this source file is only included in the application build when the native
* QF active object queue is used (instead of a message queue of an RTOS).
*
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 6.9.2
* Last updated on  2020-12-16
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS facilities for pre-defined trace records */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_actq")


/****************************************************************************/
#ifdef Q_SPY
/**
* @description
* Direct event posting is the simplest asynchronous communication method
* available in QF.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     e      pointer to the event to be posted
* @param[in]     margin number of required free slots in the queue after
*                       posting the event. The special value #QF_NO_MARGIN
*                       means that this function will assert if posting fails.
* @param[in]     sender pointer to a sender object (used only for QS tracing)
*
* @returns
* 'true' (success) if the posting succeeded (with the provided margin) and
* 'false' (failure) when the posting fails.
*
* @attention
* This function should be called only via the macro QACTIVE_POST()
* or QACTIVE_POST_X().
*
* @note
* The #QF_NO_MARGIN value of the @p margin parameter is special and denotes
* situation when the post() operation is assumed to succeed (event delivery
* guarantee). An assertion fires, when the event cannot be delivered in this
* case.
*
* @usage
* @include qf_post.c
*
* @sa QActive_post_(), QActive_postLIFO()
*/
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
#else
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin)
#endif
{
    QEQueueCtr nFree; /* temporary to avoid UB for volatile access */
    bool status;
    QF_CRIT_STAT_
    QS_TEST_PROBE_DEF(&QActive_post_)

    /** @pre event pointer must be valid */
    Q_REQUIRE_ID(100, e != (QEvt *)0);

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
            Q_ERROR_CRIT_(110); /* must be able to post the event */
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
        me->eQueue.nFree = nFree; /* update the volatile */
        if (me->eQueue.nMin > nFree) {
            me->eQueue.nMin = nFree; /* increase minimum so far */
        }

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST, me->prio)
            QS_TIME_PRE_();               /* timestamp */
            QS_OBJ_PRE_(sender);          /* the sender object */
            QS_SIG_PRE_(e->sig);          /* the signal of the event */
            QS_OBJ_PRE_(me);              /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);           /* number of free entries */
            QS_EQC_PRE_(me->eQueue.nMin); /* min number of free entries */
        QS_END_NOCRIT_PRE_()

#ifdef Q_UTEST
        /* callback to examine the posted event under the same conditions
        * as producing the #QS_QF_ACTIVE_POST trace record, which are:
        * the local filter for this AO ('me->prio') is set
        */
        if ((QS_priv_.locFilter[me->prio >> 3U]
             & (1U << (me->prio & 7U))) != 0U)
        {
            /* callback to examine the posted event */
            QS_onTestPost(sender, me, e, status);
        }
#endif

        /* empty queue? */
        if (me->eQueue.frontEvt == (QEvt *)0) {
            me->eQueue.frontEvt = e;    /* deliver event directly */
            QACTIVE_EQUEUE_SIGNAL_(me); /* signal the event queue */
        }
        /* queue is not empty, insert event into the ring-buffer */
        else {
            /* insert event into the ring buffer (FIFO) */
            QF_PTR_AT_(me->eQueue.ring, me->eQueue.head) = e;

            if (me->eQueue.head == 0U) { /* need to wrap head? */
                me->eQueue.head = me->eQueue.end;   /* wrap around */
            }
            --me->eQueue.head; /* advance the head (counter clockwise) */
        }

        QF_CRIT_X_();
    }
    else { /* cannot post the event */

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();       /* timestamp */
            QS_OBJ_PRE_(sender);  /* the sender object */
            QS_SIG_PRE_(e->sig);  /* the signal of the event */
            QS_OBJ_PRE_(me);      /* this active object (recipient) */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);   /* number of free entries */
            QS_EQC_PRE_(margin);  /* margin requested */
        QS_END_NOCRIT_PRE_()

#ifdef Q_UTEST
        /* callback to examine the posted event under the same conditions
        * as producing the #QS_QF_ACTIVE_POST trace record, which are:
        * the local filter for this AO ('me->prio') is set
        */
        if ((QS_priv_.locFilter[me->prio >> 3U]
             & (1U << (me->prio & 7U))) != 0U)
        {
            QS_onTestPost(sender, me, e, status);
        }
#endif

        QF_CRIT_X_();

        QF_gc(e); /* recycle the event to avoid a leak */
    }

    return status;
}

/****************************************************************************/
/**
* @description
* posts an event to the event queue of the active object @p me using the
* Last-In-First-Out (LIFO) policy.
*
* @note
* The LIFO policy should be used only for self-posting and with caution,
* because it alters order of events in the queue.
*
* @param[in] me pointer (see @ref oop)
* @param[in] e  pointer to the event to post to the queue
*
* @attention
* This function should be called only via the macro QACTIVE_POST_LIFO().
*
* @sa QActive_post_(), QACTIVE_POST(), QACTIVE_POST_X()
*/
void QActive_postLIFO_(QActive * const me, QEvt const * const e) {
    QEvt const *frontEvt;  /* temporary to avoid UB for volatile access */
    QEQueueCtr nFree;      /* temporary to avoid UB for volatile access */
    QF_CRIT_STAT_
    QS_TEST_PROBE_DEF(&QActive_postLIFO_)

    QF_CRIT_E_();
    nFree = me->eQueue.nFree; /* get volatile into the temporary */

    /* test-probe#1 for faking queue overflow */
    QS_TEST_PROBE_ID(1,
        nFree = 0U;
    )

    /* the queue must be able to accept the event (cannot overflow) */
    Q_ASSERT_CRIT_(210, nFree != 0U);

    /* is it a dynamic event? */
    if (e->poolId_ != 0U) {
        QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
    }

    --nFree; /* one free entry just used up */
    me->eQueue.nFree = nFree; /* update the volatile */
    if (me->eQueue.nMin > nFree) {
        me->eQueue.nMin = nFree; /* update minimum so far */
    }

    QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST_LIFO, me->prio)
        QS_TIME_PRE_();      /* timestamp */
        QS_SIG_PRE_(e->sig); /* the signal of this event */
        QS_OBJ_PRE_(me);     /* this active object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_);/* pool Id & ref Count */
        QS_EQC_PRE_(nFree);  /* # free entries */
        QS_EQC_PRE_(me->eQueue.nMin); /* min number of free entries */
    QS_END_NOCRIT_PRE_()

#ifdef Q_UTEST
        /* callback to examine the posted event under the same conditions
        * as producing the #QS_QF_ACTIVE_POST trace record, which are:
        * the local filter for this AO ('me->prio') is set
        */
        if ((QS_priv_.locFilter[me->prio >> 3U]
             & (1U << (me->prio & 7U))) != 0U)
        {
            QS_onTestPost((QActive *)0, me, e, true);
        }
#endif

    frontEvt = me->eQueue.frontEvt; /* read volatile into the temporary */
    me->eQueue.frontEvt = e; /* deliver the event directly to the front */

    /* was the queue empty? */
    if (frontEvt == (QEvt *)0) {
        QACTIVE_EQUEUE_SIGNAL_(me); /* signal the event queue */
    }
    /* queue was not empty, leave the event in the ring-buffer */
    else {
        ++me->eQueue.tail;
        /* need to wrap the tail? */
        if (me->eQueue.tail == me->eQueue.end) {
            me->eQueue.tail = 0U; /* wrap around */
        }

        QF_PTR_AT_(me->eQueue.ring, me->eQueue.tail) = frontEvt;
    }
    QF_CRIT_X_();
}

/****************************************************************************/
/**
* @description
* The behavior of this function depends on the kernel/OS used in the QF port.
* For built-in kernels (QV or QK) the function can be called only when
* the queue is not empty, so it doesn't block. For a blocking kernel/OS
* the function can block and wait for delivery of an event.
*
* @param[in,out] me  pointer (see @ref oop)
*
* @returns
* a pointer to the received event. The returned pointer is guaranteed to be
* valid (can't be NULL).
*
* @note
* This function is used internally by a QF port to extract events from
* the event queue of an active object. This function depends on the event
* queue implementation and is sometimes customized in the QF port
* (file qf_port.h). Depending on the definition of the macro
* QACTIVE_EQUEUE_WAIT_(), the function might block the calling thread when
* no events are available.
*/
QEvt const *QActive_get_(QActive * const me) {
    QEQueueCtr nFree;
    QEvt const *e;
    QF_CRIT_STAT_

    QF_CRIT_E_();
    QACTIVE_EQUEUE_WAIT_(me);  /* wait for event to arrive directly */

    e = me->eQueue.frontEvt; /* always remove event from the front location */
    nFree = me->eQueue.nFree + 1U; /* get volatile into tmp */
    me->eQueue.nFree = nFree; /* update the number of free */

    /* any events in the ring buffer? */
    if (nFree <= me->eQueue.end) {

        /* remove event from the tail */
        me->eQueue.frontEvt = QF_PTR_AT_(me->eQueue.ring, me->eQueue.tail);
        if (me->eQueue.tail == 0U) { /* need to wrap the tail? */
            me->eQueue.tail = me->eQueue.end;   /* wrap around */
        }
        --me->eQueue.tail;

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_GET, me->prio)
            QS_TIME_PRE_();      /* timestamp */
            QS_SIG_PRE_(e->sig); /* the signal of this event */
            QS_OBJ_PRE_(me);     /* this active object */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);  /* # free entries */
        QS_END_NOCRIT_PRE_()
    }
    else {
        me->eQueue.frontEvt = (QEvt *)0; /* queue becomes empty */

        /* all entries in the queue must be free (+1 for fronEvt) */
        Q_ASSERT_CRIT_(310, nFree == (me->eQueue.end + 1U));

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_GET_LAST, me->prio)
            QS_TIME_PRE_();      /* timestamp */
            QS_SIG_PRE_(e->sig); /* the signal of this event */
            QS_OBJ_PRE_(me);     /* this active object */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_END_NOCRIT_PRE_()
    }
    QF_CRIT_X_();
    return e;
}

/****************************************************************************/
/**
* @description
* Queries the minimum of free ever present in the given event queue of
* an active object with priority @p prio, since the active object
* was started.
*
* @note
* This function is available only when the native QF event queue
* implementation is used. Requesting the queue minimum of an unused
* priority level raises an assertion in the QF. (A priority level becomes
* used in QF after the call to the QF_add_() function.)
*
* @param[in] prio  Priority of the active object, whose queue is queried
*
* @returns
* the minimum of free ever present in the given event queue of an active
* object with priority @p prio, since the active object was started.
*/
uint_fast16_t QF_getQueueMin(uint_fast8_t const prio) {
    uint_fast16_t min;
    QF_CRIT_STAT_

    Q_REQUIRE_ID(400, (prio <= QF_MAX_ACTIVE)
                      && (QF_active_[prio] != (QActive *)0));

    QF_CRIT_E_();
    min = (uint_fast16_t)QF_active_[prio]->eQueue.nMin;
    QF_CRIT_X_();

    return min;
}

/****************************************************************************/

#ifdef Q_SPY
    static void QTicker_init_(QHsm * const me, void const *par,
                              uint_fast8_t const qs_id);
    static void QTicker_dispatch_(QHsm * const me, QEvt const * const e,
                              uint_fast8_t const qs_id);
    /*! virtual function to asynchronously post (FIFO) an event to an AO */
    static bool QTicker_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender);
#else
    static void QTicker_init_(QHsm * const me, void const *par);
    static void QTicker_dispatch_(QHsm * const me, QEvt const * const e);
    static bool QTicker_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin);
#endif

static void QTicker_postLIFO_(QActive * const me, QEvt const * const e);

/*! Perform downcast to QTicker pointer. */
/**
* @description
* This macro encapsulates the downcast to (QTicker *), which is used in
* QTicker_init_() and QTicker_dispatch_(). Such casts can trigger PC-Lint-Plus
* "note 9087: cast from pointer to object type to pointer to different
* object type [MISRA 2012 Rule 11.3, required]". This macro helps to
* encapsulate this deviation.
*/
#define QTICKER_CAST_(me_)  ((QActive *)(me_))

/*..........................................................................*/
/*! "constructor" of QTicker */
void QTicker_ctor(QTicker * const me, uint_fast8_t tickRate) {
    static QActiveVtable const vtable = {  /* QActive virtual table */
        { &QTicker_init_,
          &QTicker_dispatch_
#ifdef Q_SPY
          ,&QHsm_getStateHandler_
#endif
        },
        &QActive_start_,
        &QTicker_post_,
        &QTicker_postLIFO_
    };
    QActive_ctor(&me->super, Q_STATE_CAST(0)); /* superclass' ctor */
    me->super.super.vptr = &vtable.super; /* hook the vptr */

    /* reuse eQueue.head for tick-rate */
    me->super.eQueue.head = (QEQueueCtr)tickRate;
}
/*..........................................................................*/
#ifdef Q_SPY
static void QTicker_init_(QHsm * const me, void const *par,
                              uint_fast8_t const qs_id)
#else
static void QTicker_init_(QHsm * const me, void const *par)
#endif
{
    (void)me;
    (void)par;
#ifdef Q_SPY
    (void)qs_id; /* unused parameter */
#endif
    QTICKER_CAST_(me)->eQueue.tail = 0U;
}
/*..........................................................................*/
#ifdef Q_SPY
static void QTicker_dispatch_(QHsm * const me, QEvt const * const e,
                              uint_fast8_t const qs_id)
#else
static void QTicker_dispatch_(QHsm * const me, QEvt const * const e)
#endif
{
    QEQueueCtr nTicks; /* # ticks since the last call */
    QF_CRIT_STAT_

    (void)e; /* unused parameter */
#ifdef Q_SPY
    (void)qs_id; /* unused parameter */
#endif

    QF_CRIT_E_();
    nTicks = QTICKER_CAST_(me)->eQueue.tail; /* save the # of ticks */
    QTICKER_CAST_(me)->eQueue.tail = 0U; /* clear the # ticks */
    QF_CRIT_X_();

    for (; nTicks > 0U; --nTicks) {
        QF_TICK_X((uint_fast8_t)QTICKER_CAST_(me)->eQueue.head, me);
    }
}
/*..........................................................................*/
#ifndef Q_SPY
static bool QTicker_post_(QActive * const me, QEvt const * const e,
                          uint_fast16_t const margin)
#else
static bool QTicker_post_(QActive * const me, QEvt const * const e,
                          uint_fast16_t const margin,
                          void const * const sender)
#endif
{
    QF_CRIT_STAT_

    (void)e; /* unused parameter */
    (void)margin; /* unused parameter */

    QF_CRIT_E_();
    if (me->eQueue.frontEvt == (QEvt *)0) {

        static QEvt const tickEvt = { 0U, 0U, 0U };
        me->eQueue.frontEvt = &tickEvt; /* deliver event directly */
        --me->eQueue.nFree; /* one less free event */

        QACTIVE_EQUEUE_SIGNAL_(me); /* signal the event queue */
    }

    ++me->eQueue.tail; /* account for one more tick event */

    QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_POST, me->prio)
        QS_TIME_PRE_();      /* timestamp */
        QS_OBJ_PRE_(sender); /* the sender object */
        QS_SIG_PRE_(0U);     /* the signal of the event */
        QS_OBJ_PRE_(me);     /* this active object */
        QS_2U8_PRE_(0U, 0U); /* pool Id & refCtr of the evt */
        QS_EQC_PRE_(0U);     /* number of free entries */
        QS_EQC_PRE_(0U);     /* min number of free entries */
    QS_END_NOCRIT_PRE_()

    QF_CRIT_X_();

    return true; /* the event is always posted correctly */
}
/*..........................................................................*/
static void QTicker_postLIFO_(QActive * const me, QEvt const * const e) {
    (void)me; /* unused parameter */
    (void)e;  /* unused parameter */
    Q_ERROR_ID(900);
}

