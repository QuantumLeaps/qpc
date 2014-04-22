/**
* \file
* \brief QEQueue_post() definition.
* \ingroup qf
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-02-15
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

Q_DEFINE_THIS_MODULE("qeq_fifo")

/**
* \note this function is used for the "raw" thread-safe queues and NOT
* for the queues of active objects.
*/

/****************************************************************************/
/**
* \description
* Post an event to the "raw" thread-safe event queue using the
* First-In-First-Out (FIFO) order.
*
* \arguments
* \arg[in,out] \c me     pointer (see \ref derivation)
* \arg[in]     \c e      pointer to the event to be posted to the queue
* \arg[in]     \c margin number of unused slots in the queue that must
*                 be still available after posting the event
* \note
* The zero value of the \c margin argument is special and denotes situation
* when event posting is assumed to succeed (event delivery guarantee).
* An assertion fires, when the event cannot be delivered in this case.
*
* \returns 'true' (success) when the posting succeeded with the provided
* margin and 'false' (failure) when the posting fails.
*
* \note This function can be called from any task context or ISR context.
*
* \sa QEQueue_postLIFO(), QEQueue_get()
*/
bool QEQueue_post(QEQueue * const me, QEvt const * const e,
                  uint_fast16_t const margin)
{
    QEQueueCtr nFree; /* temporary to avoid UB for volatile access */
    bool status;
    QF_CRIT_STAT_

    /* \pre event must be valid */
    Q_REQUIRE_ID(100, e != (QEvt const *)0);

    QF_CRIT_ENTRY_();
    nFree = me->nFree; /* get volatile into the temporary */

    /* required margin available? */
    if (nFree > (QEQueueCtr)margin) {

        QS_BEGIN_NOCRIT_(QS_QF_EQUEUE_POST_FIFO, QS_priv_.eqObjFilter, me)
            QS_TIME_();                      /* timestamp */
            QS_SIG_(e->sig);                 /* the signal of this event */
            QS_OBJ_(me);                     /* this queue object */
            QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_(nFree);                  /* number of free entries */
            QS_EQC_(me->nMin);               /* min number of free entries */
        QS_END_NOCRIT_()

        /* is it a pool event? */
        if (e->poolId_ != (uint8_t)0) {
            QF_EVT_REF_CTR_INC_(e); /* increment the reference counter */
        }

        --nFree; /* one free entry just used up */
        me->nFree = nFree; /* update the volatile */
        if (me->nMin > nFree) {
            me->nMin = nFree; /* update minimum so far */
        }

        /* was the queue empty? */
        if (me->frontEvt == (QEvt const *)0) {
            me->frontEvt = e; /* deliver event directly */
        }
        /* queue was not empty, insert event into the ring-buffer */
        else {
            /* insert event into the ring buffer (FIFO)... */
            QF_PTR_AT_(me->ring, me->head) = e; /* insert e into buffer */
            /* need to wrap the head? */
            if (me->head == (QEQueueCtr)0) {
                me->head = me->end; /* wrap around */
            }
            --me->head;
        }
        status = true; /* event posted successfully */
    }
    else {
        /** \note If the \c margin is zero, assert that the queue can accept
        * the event. This is to support the "guaranteed event delivery"
        * policy for most events posted within the framework.
        */
        Q_ASSERT_ID(110, margin != (uint_fast16_t)0);

        QS_BEGIN_NOCRIT_(QS_QF_EQUEUE_POST_ATTEMPT, QS_priv_.eqObjFilter, me)
            QS_TIME_();                      /* timestamp */
            QS_SIG_(e->sig);                 /* the signal of this event */
            QS_OBJ_(me);                     /* this queue object */
            QS_2U8_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
            QS_EQC_(nFree);                  /* number of free entries */
            QS_EQC_(margin);                 /* margin requested */
        QS_END_NOCRIT_()

        status = false;
    }
    QF_CRIT_EXIT_();

    return status;
}
