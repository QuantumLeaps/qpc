/**
* \file
* \ingroup qf
* \brief QActive_get_() and QF_getQueueMin() definitions.
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

Q_DEFINE_THIS_MODULE("qa_get_")

/**
* \note this source file is only included in the QF library when the native
* QF active object queue is used (instead of a message queue of an RTOS).
*/

/****************************************************************************/
/**
* \description
* The behavior of this function depends on the kernel/OS used in the QF port.
* For built-in kernels (Vanilla or QK) the function can be called only when
* the queue is not empty, so it doesn't block. For a blocking kernel/OS
* the function can block and wait for delivery of an event.
*
* \arguments
* \arg[in,out] \c me  pointer (see \ref derivation)
*
* \returns a pointer to the received event. The returned pointer is always
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
        Q_ASSERT_ID(110, nFree == (me->eQueue.end + (QEQueueCtr)1));

        QACTIVE_EQUEUE_ONEMPTY_(me);

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
* \description
* Queries the minimum of free ever present in the given event queue of
* an active object with priority \a prio, since the active object
* was started.
*
* \note QF_getQueueMin() is available only when the native QF event
* queue implementation is used. Requesting the queue minimum of an unused
* priority level raises an assertion in the QF. (A priority level becomes
* used in QF after the call to the QF_add_() function.)
*
* \arguments
* \arg[in] \c prio  Priority of the active object, whose queue is queried
*
* \returns the minimum of free ever present in the given event
* queue of an active object with priority \a prio, since the active object
* was started.
*/
uint_fast16_t QF_getQueueMin(uint_fast8_t const prio) {
    uint_fast16_t min;
    QF_CRIT_STAT_

    Q_REQUIRE_ID(200, (prio <= (uint_fast8_t)QF_MAX_ACTIVE)
                      && (QF_active_[prio] != (QActive *)0));

    QF_CRIT_ENTRY_();
    min = (uint_fast16_t)QF_active_[prio]->eQueue.nMin;
    QF_CRIT_EXIT_();

    return min;
}
