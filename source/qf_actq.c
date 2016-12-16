/**
* @file
* @brief QActive native queue operations (based on QEQueue)
*
* @note this source file is only included in the QF library when the native
* QF active object queue is used (instead of a message queue of an RTOS).
*
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 5.8.1
* Last updated on  2016-12-14
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_actq")


/****************************************************************************/
/**
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
* @param[in]     margin number of required free slots in the queue
*                       after posting the event.
*
* @note this function should be called only via the macro QACTIVE_POST()
* or QACTIVE_POST_X().
*
* @note The zero value of the @p margin parameter is special and denotes
* situation when the post() operation is assumed to succeed (event delivery
* guarantee). An assertion fires, when the event cannot be delivered in
* this case.
*
* @note Direct event posting should not be confused with direct event
* dispatching. In contrast to asynchronous event posting through event
* queues, direct event dispatching is synchronous. Direct event
* dispatching occurs when you call QMSM_DISPATCH().
*
* @note
* This function is used internally by a QF port to extract events from
* the event queue of an active object. This function depends on the event
* queue implementation and is sometimes implemented in the QF port
* (file qf_port.c). Depending on the underlying OS or kernel, the
* function might block the calling thread when no events are available.
*
* @usage
* @include qf_post.c
*
* @sa QActive_post_(), QActive_postLIFO()
*/
#ifndef Q_SPY
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin)
#else
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
#endif
{
    QEQueueCtr nFree; /* temporary to avoid UB for volatile access */
    bool status;
    QF_CRIT_STAT_

    /** @pre event pointer must be valid */
    Q_REQUIRE_ID(100, e != (QEvt const *)0);

    QF_CRIT_ENTRY_();
    nFree = me->eQueue.nFree; /* get volatile into the temporary */

    /* margin available? */
    if (nFree > (QEQueueCtr)margin) {

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_FIFO, QS_priv_.aoObjFilter, me)
            QS_TIME_();               /* timestamp */
            QS_OBJ_(sender);          /* the sender object */
            QS_SIG_(e->sig);          /* the signal of the event */
            QS_OBJ_(me);              /* this active object (recipient) */
            QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_(nFree);           /* number of free entries */
            QS_EQC_(me->eQueue.nMin); /* min number of free entries */
        QS_END_NOCRIT_()

        /* is it a pool event? */
        if (e->poolId_ != (uint8_t)0) {
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        --nFree; /* one free entry just used up */
        me->eQueue.nFree = nFree;       /* update the volatile */
        if (me->eQueue.nMin > nFree) {
            me->eQueue.nMin = nFree;    /* update minimum so far */
        }

        /* empty queue? */
        if (me->eQueue.frontEvt == (QEvt const *)0) {
            me->eQueue.frontEvt = e;    /* deliver event directly */
            QACTIVE_EQUEUE_SIGNAL_(me); /* signal the event queue */
        }
        /* queue is not empty, insert event into the ring-buffer */
        else {
            /* insert event into the ring buffer (FIFO) */
            QF_PTR_AT_(me->eQueue.ring, me->eQueue.head) = e;
            if (me->eQueue.head == (QEQueueCtr)0) { /* need to wrap head? */
                me->eQueue.head = me->eQueue.end;   /* wrap around */
            }
            --me->eQueue.head; /* advance the head (counter clockwise) */
        }
        QF_CRIT_EXIT_();

        status = true; /* event posted successfully */
    }
    else {
        /** @note assert if event cannot be posted and dropping events is
        * not acceptable
        */
        Q_ASSERT_ID(110, margin != (uint_fast16_t)0);

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_ATTEMPT, QS_priv_.aoObjFilter, me)
            QS_TIME_();           /* timestamp */
            QS_OBJ_(sender);      /* the sender object */
            QS_SIG_(e->sig);      /* the signal of the event */
            QS_OBJ_(me);          /* this active object (recipient) */
            QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_(nFree);       /* number of free entries */
            QS_EQC_(margin);      /* margin requested */
        QS_END_NOCRIT_()

        QF_CRIT_EXIT_();

        QF_gc(e); /* recycle the event to avoid a leak */
        status = false; /* event not posted */
    }

    return status;
}

/****************************************************************************/
/**
* @description
* posts an event to the event queue of the active object @p me using the
* Last-In-First-Out (LIFO) policy.
*
* @note The LIFO policy should be used only for self-posting and with
* caution because it alters order of events in the queue.
*
* @param[in] me pointer (see @ref oop)
* @param[in  e  pointer to the event to post to the queue
*
* @sa QActive_post_()
*/
void QActive_postLIFO_(QActive * const me, QEvt const * const e) {
    QEvt const *frontEvt;  /* temporary to avoid UB for volatile access */
    QEQueueCtr nFree;      /* temporary to avoid UB for volatile access */
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    nFree = me->eQueue.nFree; /* get volatile into the temporary */

    /* the queue must be able to accept the event (cannot overflow) */
    Q_ASSERT_ID(210, nFree != (QEQueueCtr)0);

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_LIFO, QS_priv_.aoObjFilter, me)
        QS_TIME_();                  /* timestamp */
        QS_SIG_(e->sig);             /* the signal of this event */
        QS_OBJ_(me);                 /* this active object */
        QS_2U8_(e->poolId_, e->refCtr_);/* pool Id & ref Count of the event */
        QS_EQC_(nFree);              /* number of free entries */
        QS_EQC_(me->eQueue.nMin);    /* min number of free entries */
    QS_END_NOCRIT_()

    /* is it a pool event? */
    if (e->poolId_ != (uint8_t)0) {
        QF_EVT_REF_CTR_INC_(e);      /* increment the reference counter */
    }

    --nFree; /* one free entry just used up */
    me->eQueue.nFree = nFree; /* update the volatile */
    if (me->eQueue.nMin > nFree) {
        me->eQueue.nMin = nFree; /* update minimum so far */
    }

    frontEvt = me->eQueue.frontEvt; /* read volatile into the temporary */
    me->eQueue.frontEvt = e; /* deliver the event directly to the front */

    /* was the queue empty? */
    if (frontEvt == (QEvt const *)0) {
        QACTIVE_EQUEUE_SIGNAL_(me); /* signal the event queue */
    }
    /* queue was not empty, leave the event in the ring-buffer */
    else {
        ++me->eQueue.tail;
        /* need to wrap the tail? */
        if (me->eQueue.tail == me->eQueue.end) {
            me->eQueue.tail = (QEQueueCtr)0; /* wrap around */
        }

        QF_PTR_AT_(me->eQueue.ring, me->eQueue.tail) = frontEvt;
    }
    QF_CRIT_EXIT_();
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
* @returns a pointer to the received event. The returned pointer is always
* valid (can't be NULL).
*/
QEvt const *QActive_get_(QActive * const me) {
    QEQueueCtr nFree;
    QEvt const *e;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QACTIVE_EQUEUE_WAIT_(me);  /* wait for event to arrive directly */

    e = me->eQueue.frontEvt; /* always remove event from the front location */
    nFree= me->eQueue.nFree + (QEQueueCtr)1; /* get volatile into tmp */
    me->eQueue.nFree = nFree; /* update the number of free */

    /* any events in the ring buffer? */
    if (nFree <= me->eQueue.end) {

        /* remove event from the tail */
        me->eQueue.frontEvt = QF_PTR_AT_(me->eQueue.ring, me->eQueue.tail);
        if (me->eQueue.tail == (QEQueueCtr)0) { /* need to wrap the tail? */
            me->eQueue.tail = me->eQueue.end;   /* wrap around */
        }
        --me->eQueue.tail;

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_GET, QS_priv_.aoObjFilter, me)
            QS_TIME_();                   /* timestamp */
            QS_SIG_(e->sig);              /* the signal of this event */
            QS_OBJ_(me);                  /* this active object */
            QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_(nFree);               /* number of free entries */
        QS_END_NOCRIT_()
    }
    else {
        me->eQueue.frontEvt = (QEvt const *)0; /* queue becomes empty */

        /* all entries in the queue must be free (+1 for fronEvt) */
        Q_ASSERT_ID(310, nFree == (me->eQueue.end + (QEQueueCtr)1));

        QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_GET_LAST, QS_priv_.aoObjFilter, me)
            QS_TIME_();                   /* timestamp */
            QS_SIG_(e->sig);              /* the signal of this event */
            QS_OBJ_(me);                  /* this active object */
            QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_END_NOCRIT_()
    }
    QF_CRIT_EXIT_();
    return e;
}

/****************************************************************************/
/**
* @description
* Queries the minimum of free ever present in the given event queue of
* an active object with priority @p prio, since the active object
* was started.
*
* @note QF_getQueueMin() is available only when the native QF event
* queue implementation is used. Requesting the queue minimum of an unused
* priority level raises an assertion in the QF. (A priority level becomes
* used in QF after the call to the QF_add_() function.)
*
* @param[in] prio  Priority of the active object, whose queue is queried
*
* @returns the minimum of free ever present in the given event
* queue of an active object with priority @p prio, since the active object
* was started.
*/
uint_fast16_t QF_getQueueMin(uint_fast8_t const prio) {
    uint_fast16_t min;
    QF_CRIT_STAT_

    Q_REQUIRE_ID(400, (prio <= (uint_fast8_t)QF_MAX_ACTIVE)
                      && (QF_active_[prio] != (QActive *)0));

    QF_CRIT_ENTRY_();
    min = (uint_fast16_t)QF_active_[prio]->eQueue.nMin;
    QF_CRIT_EXIT_();

    return min;
}


/****************************************************************************/
/****************************************************************************/
static void QTicker_init_(QHsm * const me, QEvt const * const e);
static void QTicker_dispatch_(QHsm * const me, QEvt const * const e);
#ifdef Q_SPY
    /*! virtual function to asynchronously post (FIFO) an event to an AO */
    static bool QTicker_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender);
#else
    static bool QTicker_post_(QActive * const me, QEvt const * const e,
                      uint_fast16_t const margin);
#endif
static void QTicker_postLIFO_(QActive * const me, QEvt const * const e);

/*..........................................................................*/
/*! "constructor" of QTicker */
void QTicker_ctor(QTicker * const me, uint8_t tickRate) {
    static QActiveVtbl const vtbl = {  /* QActiveVtbl virtual table */
        { &QTicker_init_,
          &QTicker_dispatch_ },
        &QActive_start_,
        &QTicker_post_,
        &QTicker_postLIFO_
    };
    QActive_ctor(me, Q_STATE_CAST(0)); /* superclass' ctor */
    me->super.vptr = &vtbl.super; /* hook the vptr */

    /*  reuse eQueue.head for tick-rate */
    me->eQueue.head = (QEQueueCtr)tickRate;
}
/*..........................................................................*/
static void QTicker_init_(QHsm * const me, QEvt const * const e) {
    (void)me;
    (void)e;
    ((QActive *)me)->eQueue.tail = (QEQueueCtr)0;
}
/*..........................................................................*/
static void QTicker_dispatch_(QHsm * const me, QEvt const * const e) {
    QEQueueCtr n;
    QF_CRIT_STAT_

    (void)e; /* unused parameter */

    QF_CRIT_ENTRY_();
    n = ((QActive *)me)->eQueue.tail; /* # ticks since last call */
    ((QActive *)me)->eQueue.tail = (QEQueueCtr)0; /* clear the # ticks */
    QF_CRIT_EXIT_();

    for (; n > (QEQueueCtr)0; --n) {
        QF_TICK_X(((QActive const *)me)->eQueue.head, me);
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

    QF_CRIT_ENTRY_();
    if (me->eQueue.frontEvt == (QEvt const *)0) {

        static QEvt const tickEvt = { (QSignal)0, (uint8_t)0, (uint8_t)0 };
        me->eQueue.frontEvt = &tickEvt; /* deliver event directly */
        --me->eQueue.nFree; /* one less free event */

        QACTIVE_EQUEUE_SIGNAL_(me); /* signal the event queue */
    }

    ++me->eQueue.tail; /* account for one more tick event */

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_FIFO, QS_priv_.aoObjFilter, me)
        QS_TIME_();           /* timestamp */
        QS_OBJ_(sender);      /* the sender object */
        QS_SIG_((QSignal)0);  /* the signal of the event */
        QS_OBJ_(me);          /* this active object */
        QS_2U8_((uint8_t)0, (uint8_t)0); /* pool Id & refCtr of the evt */
        QS_EQC_((uint8_t)0);  /* number of free entries */
        QS_EQC_((uint8_t)0);  /* min number of free entries */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();

    return true; /* the event is always posted correctly */
}
/*..........................................................................*/
static void QTicker_postLIFO_(QActive * const me, QEvt const * const e) {
    (void)me;
    (void)e;
    Q_ERROR_ID(900);
}
