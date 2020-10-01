/**
* @file
* @brief Publish-Subscribe services
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

Q_DEFINE_THIS_MODULE("qf_ps")


/* Package-scope objects ****************************************************/
QSubscrList *QF_subscrList_;
enum_t QF_maxPubSignal_;

/****************************************************************************/
/**
* @description
* This function initializes the publish-subscribe facilities of QF and must
* be called exactly once before any subscriptions/publications occur in
* the application.
*
* @param[in] subscrSto pointer to the array of subscriber lists
* @param[in] maxSignal the dimension of the subscriber array and at
*                      the same time the maximum signal that can be published
*                      or subscribed.
*
* The array of subscriber-lists is indexed by signals and provides a mapping
* between the signals and subscriber-lists. The subscriber-lists are bitmasks
* of type ::QSubscrList, each bit in the bit mask corresponding to the unique
* priority of an active object. The size of the ::QSubscrList bit mask
* depends on the value of the #QF_MAX_ACTIVE macro.
*
* @note
* The publish-subscribe facilities are optional, meaning that you might
* choose not to use publish-subscribe. In that case calling QF_psInit()
* and using up memory for the subscriber-lists is unnecessary.
*
* @sa ::QSubscrList
*
* @usage
* The following example shows the typical initialization sequence of QF:
* @include qf_main.c
*/
void QF_psInit(QSubscrList * const subscrSto, enum_t const maxSignal) {
    QF_subscrList_   = subscrSto;
    QF_maxPubSignal_ = maxSignal;

    /* zero the subscriber list, so that the framework can start correctly
    * even if the startup code fails to clear the uninitialized data
    * (as is required by the C Standard).
    */
    QF_bzero(subscrSto, (uint_fast16_t)maxSignal * sizeof(QSubscrList));
}

/****************************************************************************/
/**
* @description
* This function posts (using the FIFO policy) the event @a e to **all**
* active objects that have subscribed to the signal @a e->sig, which is
* called _multicasting_. The multicasting performed in this function is
* very efficient based on reference-counting inside the published event
* ("zero-copy" event multicasting). This function is designed to be
* callable from any part of the system, including ISRs, device drivers,
* and active objects.
*
* @note
* To avoid any unexpected re-ordering of events posted into AO queues,
* the event multicasting is performed with scheduler __locked__. However,
* the scheduler is locked only up to the priority level of the highest-
* priority subscriber, so any AOs of even higher priority, which did not
* subscribe to this event are _not_ affected.
*
* @attention this function should be called only via the macro QF_PUBLISH()
*/
#ifndef Q_SPY
void QF_publish_(QEvt const * const e)
#else
void QF_publish_(QEvt const * const e, void const * const sender)
#endif
{
    QPSet subscrList; /* local, modifiable copy of the subscriber list */
    QF_CRIT_STAT_

    /** @pre the published signal must be within the configured range */
    Q_REQUIRE_ID(200, e->sig < (QSignal)QF_maxPubSignal_);

    QF_CRIT_E_();

    QS_BEGIN_NOCRIT_PRE_(QS_QF_PUBLISH, 0U)
        QS_TIME_PRE_();          /* the timestamp */
        QS_OBJ_PRE_(sender);     /* the sender object */
        QS_SIG_PRE_(e->sig);     /* the signal of the event */
        QS_2U8_PRE_(e->poolId_, e->refCtr_);/* pool Id & ref Count */
    QS_END_NOCRIT_PRE_()

    /* is it a dynamic event? */
    if (e->poolId_ != 0U) {
        /* NOTE: The reference counter of a dynamic event is incremented to
        * prevent premature recycling of the event while the multicasting
        * is still in progress. At the end of the function, the garbage
        * collector step (QF_gc()) decrements the reference counter and
        * recycles the event if the counter drops to zero. This covers the
        * case when the event was published without any subscribers.
        */
        QF_EVT_REF_CTR_INC_(e);
    }

    /* make a local, modifiable copy of the subscriber list */
    subscrList = QF_PTR_AT_(QF_subscrList_, e->sig);
    QF_CRIT_X_();

    if (QPSet_notEmpty(&subscrList)) { /* any subscribers? */
        uint_fast8_t p;
        QF_SCHED_STAT_

        QPSet_findMax(&subscrList, p); /* the highest-prio subscriber */

        QF_SCHED_LOCK_(p); /* lock the scheduler up to prio 'p' */
        do { /* loop over all subscribers */
            /* the prio of the AO must be registered with the framework */
            Q_ASSERT_ID(210, QF_active_[p] != (QActive *)0);

            /* QACTIVE_POST() asserts internally if the queue overflows */
            QACTIVE_POST(QF_active_[p], e, sender);

            QPSet_remove(&subscrList, p); /* remove the handled subscriber */
            if (QPSet_notEmpty(&subscrList)) { /* still more subscribers? */
                QPSet_findMax(&subscrList, p); /* highest-prio subscriber */
            }
            else {
                p = 0U; /* no more subscribers */
            }
        } while (p != 0U);
        QF_SCHED_UNLOCK_(); /* unlock the scheduler */
    }

    /* The following garbage collection step decrements the reference counter
    * and recycles the event if the counter drops to zero. This covers both
    * cases when the event was published with or without any subscribers.
    */
    QF_gc(e);
}

/****************************************************************************/
/**
* @description
* This function is part of the Publish-Subscribe event delivery mechanism
* available in QF. Subscribing to an event means that the framework will
* start posting all published events with a given signal @p sig to the
* event queue of the active object @p me.
*
* @param[in,out] me  pointer (see @ref oop)
* @param[in]     sig event signal to subscribe
*
* @usage
* The following example shows how the Table active object subscribes
* to three signals in the initial transition:
* @include qf_subscribe.c
*
* @sa
* QF_publish_(), QActive_unsubscribe(), and QActive_unsubscribeAll()
*/
void QActive_subscribe(QActive const * const me, enum_t const sig) {
    uint_fast8_t p = (uint_fast8_t)me->prio;
    QF_CRIT_STAT_

    Q_REQUIRE_ID(300, ((enum_t)Q_USER_SIG <= sig)
              && (sig < QF_maxPubSignal_)
              && (0U < p) && (p <= QF_MAX_ACTIVE)
              && (QF_active_[p] == me));

    QF_CRIT_E_();

    QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_SUBSCRIBE, me->prio)
        QS_TIME_PRE_();    /* timestamp */
        QS_SIG_PRE_(sig);  /* the signal of this event */
        QS_OBJ_PRE_(me);   /* this active object */
    QS_END_NOCRIT_PRE_()

    /* set the priority bit */
    QPSet_insert(&QF_PTR_AT_(QF_subscrList_, sig), p);

    QF_CRIT_X_();
}

/****************************************************************************/
/**
* @description
* This function is part of the Publish-Subscribe event delivery mechanism
* available in QF. Un-subscribing from an event means that the framework
* will stop posting published events with a given signal @p sig to the
* event queue of the active object @p me.
*
* @param[in] me  pointer (see @ref oop)
* @param[in] sig event signal to unsubscribe
*
* @note
* Due to the latency of event queues, an active object should NOT
* assume that a given signal @p sig will never be dispatched to the
* state machine of the active object after un-subscribing from that signal.
* The event might be already in the queue, or just about to be posted
* and the un-subscribe operation will not flush such events.
*
* @note
* Un-subscribing from a signal that has never been subscribed in the
* first place is considered an error and QF will raise an assertion.
*
* @sa
* QF_publish_(), QActive_subscribe(), and QActive_unsubscribeAll()
*/
void QActive_unsubscribe(QActive const * const me, enum_t const sig) {
    uint_fast8_t p = (uint_fast8_t)me->prio;
    QF_CRIT_STAT_

    /** @pre the singal and the prioriy must be in ragne, the AO must also
    * be registered with the framework
    */
    Q_REQUIRE_ID(400, ((enum_t)Q_USER_SIG <= sig)
              && (sig < QF_maxPubSignal_)
              && (0U < p) && (p <= QF_MAX_ACTIVE)
              && (QF_active_[p] == me));

    QF_CRIT_E_();

    QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_UNSUBSCRIBE, me->prio)
        QS_TIME_PRE_();    /* timestamp */
        QS_SIG_PRE_(sig);  /* the signal of this event */
        QS_OBJ_PRE_(me);   /* this active object */
    QS_END_NOCRIT_PRE_()

    /* clear priority bit */
    QPSet_remove(&QF_PTR_AT_(QF_subscrList_, sig), p);

    QF_CRIT_X_();
}

/****************************************************************************/
/**
* @description
* This function is part of the Publish-Subscribe event delivery mechanism
* available in QF. Un-subscribing from all events means that the framework
* will stop posting any published events to the event queue of the active
* object @p me.
*
* @param[in] me  pointer (see @ref oop)
*
* @note
* Due to the latency of event queues, an active object should NOT
* assume that no events will ever be dispatched to the state machine of
* the active object after un-subscribing from all events.
* The events might be already in the queue, or just about to be posted
* and the un-subscribe operation will not flush such events. Also, the
* alternative event-delivery mechanisms, such as direct event posting or
* time events, can be still delivered to the event queue of the active
* object.
*
* @sa
* QF_publish_(), QActive_subscribe(), and QActive_unsubscribe()
*/
void QActive_unsubscribeAll(QActive const * const me) {
    uint_fast8_t p = (uint_fast8_t)me->prio;
    enum_t sig;

    Q_REQUIRE_ID(500, (0U < p) && (p <= QF_MAX_ACTIVE)
                        && (QF_active_[p] == me));

    for (sig = (enum_t)Q_USER_SIG; sig < QF_maxPubSignal_; ++sig) {
        QF_CRIT_STAT_
        QF_CRIT_E_();
        if (QPSet_hasElement(&QF_PTR_AT_(QF_subscrList_, sig), p)) {
            QPSet_remove(&QF_PTR_AT_(QF_subscrList_, sig), p);

            QS_BEGIN_NOCRIT_PRE_(QS_QF_ACTIVE_UNSUBSCRIBE, me->prio)
                QS_TIME_PRE_();   /* timestamp */
                QS_SIG_PRE_(sig); /* the signal of this event */
                QS_OBJ_PRE_(me);  /* this active object */
            QS_END_NOCRIT_PRE_()
        }
        QF_CRIT_X_();

        /* prevent merging critical sections */
        QF_CRIT_EXIT_NOP();
    }
}

