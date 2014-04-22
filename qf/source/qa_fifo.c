/**
* \file
* \ingroup qf
* \brief QActive_post_() definition
* \cond
******************************************************************************
* Product: QEP/C
* Last updated for version 5.3.0
* Last updated on  2014-02-12
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
* \endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qa_fifo")

/** \note this source file is only included in the QF library when the native
* QF active object queue is used (instead of a message queue of an RTOS).
*/

/****************************************************************************/
/**
* \description
* Direct event posting is the simplest asynchronous communication method
* available in QF. The following example illustrates how the Philo active
* object posts directly the HUNGRY event to the Table active object.\n
* \n
* The argument \a margin specifies the minimum number of free slots in
* the queue that must be available for posting to succeed. The function
* returns 1 (success) if the posting succeeded (with the provided margin)
* and 0 (failure) when the posting fails.
*
* \arguments
* \arg[in,out] \c me     pointer (see \ref derivation)
* \arg[in]     \c e      pointer to the event to be posted
* \arg[in]     \c margin number of required free slots in the queue
*                        after posting the event.
*
* \note this function should be called only via the macro QACTIVE_POST()
* or QACTIVE_POST_X().
*
* \note The zero value of the 'margin' argument is special and denotes
* situation when the post() operation is assumed to succeed (event delivery
* guarantee). An assertion fires, when the event cannot be delivered in
* this case.
*
* \note Direct event posting should not be confused with direct event
* dispatching. In contrast to asynchronous event posting through event
* queues, direct event dispatching is synchronous. Direct event
* dispatching occurs when you call QMSM_DISPATCH().
*
* \note
* This function is used internally by a QF port to extract events from
* the event queue of an active object. This function depends on the event
* queue implementation and is sometimes implemented in the QF port
* (file qf_port.c). Depending on the underlying OS or kernel, the
* function might block the calling thread when no events are available.
*
* \usage
* \include qf_post.c
*
* \sa QActive_post_(), QActive_postLIFO()
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

    /** \pre event pointer must be valid */
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
        status = true; /* event posted successfully */
    }
    else {
        /** \note assert if event cannot be posted and dropping events is
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

        QF_gc(e); /* recycle the event to avoid a leak */
        status = false; /* event not posted */
    }
    QF_CRIT_EXIT_();

    return status;
}

