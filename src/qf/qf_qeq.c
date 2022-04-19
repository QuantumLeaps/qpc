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
* @date Last updated on: 2021-12-23
* @version Last updated for: @ref qpc_7_0_0
*
* @file
* @brief ::QEQueue implementation (QP native thread-safe queue)
* @ingroup qf
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

Q_DEFINE_THIS_MODULE("qf_qeq")

/*==========================================================================*/
/*!
* @public @memberof QEQueue
* @description
* Initialize the event queue by giving it the storage for the ring buffer.
*
* @param[in,out] me   pointer (see @ref oop)
* @param[in]     qSto an array of pointers to ::QEvt to sereve as the
*                     ring buffer for the event queue
* @param[in]     qLen the length of the @p qSto buffer (in ::QEvt pointers)
*
* @note The actual capacity of the queue is qLen + 1, because of the extra
* location forntEvt.
*
* @note
* This function is also used to initialize the event queues of active
* objects in the built-int QV and QK kernels, as well as other
* QP ports to OSes/RTOSes that do provide a suitable message queue.
*/
void QEQueue_init(QEQueue * const me, QEvt const * * const qSto,
                  uint_fast16_t const qLen)
{
    me->frontEvt = (QEvt *)0; /* no events in the queue */
    me->ring     = qSto;      /* the beginning of the ring buffer */
    me->end      = (QEQueueCtr)qLen;
    if (qLen != 0U) {
        me->head = 0U;
        me->tail = 0U;
    }
    me->nFree    = (QEQueueCtr)(qLen + 1U); /* +1 for frontEvt */
    me->nMin     = me->nFree;
}

/*==========================================================================*/
/*!
* @public @memberof QEQueue
* @description
* Post an event to the "raw" thread-safe event queue using the
* First-In-First-Out (FIFO) order.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     e      pointer to the event to be posted to the queue
* @param[in]     margin number of required free slots in the queue after
*                       posting the event. The special value #QF_NO_MARGIN
*                       means that this function will assert if posting
* @note
* The #QF_NO_MARGIN value of the @p margin parameter is special and
* denotes situation when the post() operation is assumed to succeed (event
* delivery guarantee). An assertion fires, when the event cannot be
* delivered in this case.
*
* @returns 'true' (success) when the posting succeeded with the provided
* margin and 'false' (failure) when the posting fails.
*
* @note This function can be called from any task context or ISR context.
*
* @sa QEQueue_postLIFO(), QEQueue_get()
*/
bool QEQueue_post(QEQueue * const me, QEvt const * const e,
                  uint_fast16_t const margin, uint_fast8_t const qs_id)
{
    /* @pre event must be valid */
    Q_REQUIRE_ID(200, e != (QEvt *)0);

    (void)qs_id; /* unused parameter (outside Q_SPY build configuration) */

    QF_CRIT_STAT_
    QF_CRIT_E_();
    QEQueueCtr nFree  = me->nFree; /* get volatile into the temporary */

    /* required margin available? */
    bool status;
    if (((margin == QF_NO_MARGIN) && (nFree > 0U))
        || (nFree > (QEQueueCtr)margin))
    {
        /* is it a dynamic event? */
        if (e->poolId_ != 0U) {
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        --nFree; /* one free entry just used up */
        me->nFree = nFree; /* update the volatile */
        if (me->nMin > nFree) {
            me->nMin = nFree; /* update minimum so far */
        }

        QS_BEGIN_NOCRIT_PRE_(QS_QF_EQUEUE_POST, qs_id)
            QS_TIME_PRE_();          /* timestamp */
            QS_SIG_PRE_(e->sig);     /* the signal of this event */
            QS_OBJ_PRE_(me);         /* this queue object */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);      /* number of free entries */
            QS_EQC_PRE_(me->nMin);   /* min number of free entries */
        QS_END_NOCRIT_PRE_()

        /* was the queue empty? */
        if (me->frontEvt == (QEvt *)0) {
            me->frontEvt = e; /* deliver event directly */
        }
        /* queue was not empty, insert event into the ring-buffer */
        else {
            /* insert event into the ring buffer (FIFO)... */
            QF_PTR_AT_(me->ring, me->head) = e; /* insert e into buffer */
            /* need to wrap the head? */
            if (me->head == 0U) {
                me->head = me->end; /* wrap around */
            }
            --me->head;
        }
        status = true; /* event posted successfully */
    }
    else {
        /*! @note assert if event cannot be posted and dropping events is
        * not acceptable
        */
        Q_ASSERT_CRIT_(210, margin != QF_NO_MARGIN);

        QS_BEGIN_NOCRIT_PRE_(QS_QF_EQUEUE_POST_ATTEMPT, qs_id)
            QS_TIME_PRE_();          /* timestamp */
            QS_SIG_PRE_(e->sig);     /* the signal of this event */
            QS_OBJ_PRE_(me);         /* this queue object */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_PRE_(nFree);      /* number of free entries */
            QS_EQC_PRE_(margin);     /* margin requested */
        QS_END_NOCRIT_PRE_()

        status = false;
    }
    QF_CRIT_X_();

    return status;
}

/*==========================================================================*/
/*!
* @public @memberof QEQueue
* @description
* Post an event to the "raw" thread-safe event queue using the
* Last-In-First-Out (LIFO) order.
*
* @param[in,out] me  pointer (see @ref oop)
* @param[in]     e   pointer to the event to be posted to the queue
*
* @attention
* The LIFO policy should be used only with great __caution__, because
* it alters the order of events in the queue.
*
* @note
* This function can be called from any task context or ISR context.
*
* @note
* this function is used for the "raw" thread-safe queues and __not__
* for the queues of active objects.
*
* @sa
* QEQueue_post(), QEQueue_get(), QActive_defer()
*/
void QEQueue_postLIFO(QEQueue * const me, QEvt const * const e,
                      uint_fast8_t const qs_id)
{
    (void)qs_id; /* unused parameter, if Q_SPY not defined */

    QF_CRIT_STAT_
    QF_CRIT_E_();
    QEQueueCtr nFree = me->nFree; /* get volatile into the temporary */

    /*! @pre the queue must be able to accept the event (cannot overflow) */
    Q_REQUIRE_CRIT_(300, nFree != 0U);

    /* is it a dynamic event? */
    if (e->poolId_ != 0U) {
        QF_EVT_REF_CTR_INC_(e);  /* increment the reference counter */
    }

    --nFree;  /* one free entry just used up */
    me->nFree = nFree; /* update the volatile */
    if (me->nMin > nFree) {
        me->nMin = nFree; /* update minimum so far */
    }

    QS_BEGIN_NOCRIT_PRE_(QS_QF_EQUEUE_POST_LIFO, qs_id)
        QS_TIME_PRE_();         /* timestamp */
        QS_SIG_PRE_(e->sig);    /* the signal of this event */
        QS_OBJ_PRE_(me);        /* this queue object */
        QS_2U8_PRE_(e->poolId_, e->refCtr_);/* pool Id & ref Count of event */
        QS_EQC_PRE_(nFree);     /* number of free entries */
        QS_EQC_PRE_(me->nMin);  /* min number of free entries */
    QS_END_NOCRIT_PRE_()

    QEvt const * const frontEvt = me->frontEvt; /* read volatile into temp */
    me->frontEvt = e; /* deliver event directly to the front of the queue */

    /* was the queue not empty? */
    if (frontEvt != (QEvt *)0) {
        ++me->tail;
        if (me->tail == me->end) {     /* need to wrap the tail? */
            me->tail = 0U;  /* wrap around */
        }
        QF_PTR_AT_(me->ring, me->tail) = frontEvt; /* save old front evt */
    }
    QF_CRIT_X_();
}

/*==========================================================================*/
/*!
* @public @memberof QEQueue
* @description
* Retrieves an event from the front of the "raw" thread-safe queue and
* returns a pointer to this event to the caller.
*
* @param[in,out] me     pointer (see @ref oop)
*
* @returns
* pointer to event at the front of the queue, if the queue is
* not empty and NULL if the queue is empty.
*
* @note
* this function is used for the "raw" thread-safe queues and __not__
* for the queues of active objects.
*
* @sa
* QEQueue_post(), QEQueue_postLIFO(), QActive_recall()
*/
QEvt const *QEQueue_get(QEQueue * const me, uint_fast8_t const qs_id) {
    (void)qs_id; /* unused parameter (outside Q_SPY build configuration) */

    QF_CRIT_STAT_
    QF_CRIT_E_();
    QEvt const * const e = me->frontEvt; /* remove event from the front */

    /* was the queue not empty? */
    if (e != (QEvt *)0) {
        /* use a temporary variable to increment volatile me->nFree */
        QEQueueCtr const nFree = me->nFree + 1U;
        me->nFree = nFree; /* update the number of free */

        /* any events in the ring buffer? */
        if (nFree <= me->end) {
            me->frontEvt = QF_PTR_AT_(me->ring, me->tail); /* get from tail */
            if (me->tail == 0U) { /* need to wrap the tail? */
                me->tail = me->end; /* wrap around */
            }
            --me->tail;

            QS_BEGIN_NOCRIT_PRE_(QS_QF_EQUEUE_GET, qs_id)
                QS_TIME_PRE_();      /* timestamp */
                QS_SIG_PRE_(e->sig); /* the signal of this event */
                QS_OBJ_PRE_(me);     /* this queue object */
                QS_2U8_PRE_(e->poolId_, e->refCtr_);/* pool Id & ref Count */
                QS_EQC_PRE_(nFree);  /* number of free entries */
            QS_END_NOCRIT_PRE_()
        }
        else {
            me->frontEvt = (QEvt *)0; /* queue becomes empty */

            /* all entries in the queue must be free (+1 for fronEvt) */
            Q_ASSERT_CRIT_(410, nFree == (me->end + 1U));

            QS_BEGIN_NOCRIT_PRE_(QS_QF_EQUEUE_GET_LAST, qs_id)
                QS_TIME_PRE_();      /* timestamp */
                QS_SIG_PRE_(e->sig); /* the signal of this event */
                QS_OBJ_PRE_(me);     /* this queue object */
                QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_END_NOCRIT_PRE_()
        }
    }
    QF_CRIT_X_();
    return e;
}
