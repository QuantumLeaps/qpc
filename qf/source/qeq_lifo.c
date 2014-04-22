/**
* \file
* \brief QEQueue_postLIFO() definition.
* \ingroup qf
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-04-09
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

Q_DEFINE_THIS_MODULE("qeq_lifo")

/****************************************************************************/
/**
* \description
* Post an event to the "raw" thread-safe event queue using the
* Last-In-First-Out (LIFO) order.
*
* \arguments
* \arg[in,out] \c me  pointer (see \ref derivation)
* \arg[in]     \c e   pointer to the event to be posted to the queue
*
* \attention The LIFO policy should be used only with great __caution__,
* because it alters the order of events in the queue.
*
* \note This function can be called from any task context or ISR context.
*
* \note this function is used for the "raw" thread-safe queues and __not__
* for the queues of active objects.
*
* \sa QEQueue_post(), QEQueue_get(), QActive_defer()
*/
void QEQueue_postLIFO(QEQueue * const me, QEvt const * const e) {
    QEvt const *frontEvt; /* temporary to avoid UB for volatile access */
    QEQueueCtr nFree;     /* temporary to avoid UB for volatile access */
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    nFree = me->nFree;    /* get volatile into the temporary */

    /** \pre the queue must be able to accept the event (cannot overflow) */
    Q_REQUIRE_ID(100, nFree != (QEQueueCtr)0);

    QS_BEGIN_NOCRIT_(QS_QF_EQUEUE_POST_LIFO, QS_priv_.eqObjFilter, me)
        QS_TIME_();              /* timestamp */
        QS_SIG_(e->sig);         /* the signal of this event */
        QS_OBJ_(me);             /* this queue object */
        QS_2U8_(e->poolId_, e->refCtr_);/* pool Id & ref Count of the event */
        QS_EQC_(nFree);          /* number of free entries */
        QS_EQC_(me->nMin);       /* min number of free entries */
    QS_END_NOCRIT_()

    /* is it a pool event? */
    if (e->poolId_ != (uint8_t)0) {
        QF_EVT_REF_CTR_INC_(e);  /* increment the reference counter */
    }

    --nFree;  /* one free entry just used up */
    me->nFree = nFree; /* update the volatile */
    if (me->nMin > nFree) {
        me->nMin = nFree; /* update minimum so far */
    }

    frontEvt = me->frontEvt; /* read volatile into the temporary */
    me->frontEvt = e; /* deliver event directly to the front of the queue */

    /* was the queue not empty? */
    if (frontEvt != (QEvt const *)0) {
        ++me->tail;
        if (me->tail == me->end) {     /* need to wrap the tail? */
            me->tail = (QEQueueCtr)0;  /* wrap around */
        }
        QF_PTR_AT_(me->ring, me->tail) = frontEvt; /* save old front evt */
    }
    QF_CRIT_EXIT_();
}
