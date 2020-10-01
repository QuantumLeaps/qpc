/**
* @file
* @brief QActive_defer() and QActive_recall() implementation.
* @ingroup qf
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-03
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

Q_DEFINE_THIS_MODULE("qf_defer")

/****************************************************************************/
/**
* @description
* This function is part of the event deferral support. An active object
* uses this function to defer an event @p e to the QF-supported native
* event queue @p eq. QF correctly accounts for another outstanding
* reference to the event and will not recycle the event at the end of
* the RTC step. Later, the active object might recall one event at a
* time from the event queue.
*
* @param[in,out] me  pointer (see @ref oop)
* @param[in]     eq  pointer to a "raw" thread-safe queue to recall
*                    an event from.
* @param[in]     e   pointer to the event to be deferred
*
* @returns
* 'true' (success) when the event could be deferred and 'false' (failure)
* if event deferral failed due to overflowing the queue.
*
* An active object can use multiple event queues to defer events of
* different kinds.
*
* @sa
* QActive_recall(), QEQueue, QActive_flushDeferred()
*/
bool QActive_defer(QActive const * const me, QEQueue * const eq,
                   QEvt const * const e)
{
    bool status = QEQueue_post(eq, e, 0U, me->prio);
    QS_CRIT_STAT_

    (void)me; /* unused parameter */

    QS_BEGIN_PRE_(QS_QF_ACTIVE_DEFER, me->prio)
        QS_TIME_PRE_();      /* time stamp */
        QS_OBJ_PRE_(me);     /* this active object */
        QS_OBJ_PRE_(eq);     /* the deferred queue */
        QS_SIG_PRE_(e->sig); /* the signal of the event */
        QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
    QS_END_PRE_()

    return status;
}

/****************************************************************************/
/**
* @description
* This function is part of the event deferral support. An active object
* uses this function to recall a deferred event from a given QF
* event queue. Recalling an event means that it is removed from the
* deferred event queue @p eq and posted (LIFO) to the event queue of
* the active object.
*
* @param[in,out] me  pointer (see @ref oop)
* @param[in]     eq  pointer to a "raw" thread-safe queue to recall
*                    an event from.
* @returns
* 'true' if an event has been recalled and 'false' if not.
*
* @note
* An active object can use multiple event queues to defer events of
* different kinds.
*
* @sa
* QActive_recall(), ::QEQueue, QACTIVE_POST_LIFO()
*/
bool QActive_recall(QActive * const me, QEQueue * const eq) {
    QEvt const *e = QEQueue_get(eq, me->prio);
    bool recalled;

    /* event available? */
    if (e != (QEvt *)0) {
        QF_CRIT_STAT_

        QACTIVE_POST_LIFO(me, e); /* post it to the front of the AO's queue */

        QF_CRIT_E_();

        /* is it a dynamic event? */
        if (e->poolId_ != 0U) {

            /* after posting to the AO's queue the event must be referenced
            * at least twice: once in the deferred event queue (eq->get()
            * did NOT decrement the reference counter) and once in the
            * AO's event queue.
            */
            Q_ASSERT_CRIT_(210, e->refCtr_ >= 2U);

            /* we need to decrement the reference counter once, to account
            * for removing the event from the deferred event queue.
            */
            QF_EVT_REF_CTR_DEC_(e); /* decrement the reference counter */
        }

        QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_RECALL, me->prio)
            QS_TIME_PRE_();      /* time stamp */
            QS_OBJ_PRE_(me);     /* this active object */
            QS_OBJ_PRE_(eq);     /* the deferred queue */
            QS_SIG_PRE_(e->sig); /* the signal of the event */
            QS_2U8_PRE_(e->poolId_, e->refCtr_); /* pool Id & ref Count */
        QS_END_NOCRIT_PRE_()

        QF_CRIT_X_();
        recalled = true;
    }
    else {
        QS_CRIT_STAT_

        QS_BEGIN_PRE_(QS_QF_ACTIVE_RECALL_ATTEMPT, me->prio)
            QS_TIME_PRE_();      /* time stamp */
            QS_OBJ_PRE_(me);     /* this active object */
            QS_OBJ_PRE_(eq);     /* the deferred queue */
        QS_END_PRE_()

        recalled = false;
    }
    return recalled;
}

/****************************************************************************/
/**
* @description
* This function is part of the event deferral support. An active object
* can use this function to flush a given QF event queue. The function makes
* sure that the events are not leaked.
*
* @param[in,out] me  pointer (see @ref oop)
* @param[in]     eq  pointer to a "raw" thread-safe queue to flush.
*
* @returns
* the number of events actually flushed from the queue.
*
* @sa
* QActive_defer(), QActive_recall(), QEQueue
*/
uint_fast16_t QActive_flushDeferred(QActive const * const me,
                                    QEQueue * const eq)
{
    QEvt const *e = QEQueue_get(eq, me->prio);
    uint_fast16_t n = 0U;

    (void)me; /* unused parameter */

    for (; e != (QEvt *)0; e = QEQueue_get(eq, me->prio)) {
        QF_gc(e); /* garbage collect */
        ++n; /* count the flushed event */
    }
    return n;
}

