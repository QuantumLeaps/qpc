/**
* @file
* @brief QXK preemptive dual-mode kernel core functions
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 5.9.5
* Last updated on  2017-07-19
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* https://state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qxk_pkg.h"      /* QXK package-scope internal interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

/* protection against including this source file in a wrong project */
#ifndef qxk_h
    #error "Source file included in a project NOT based on the QXK kernel"
#endif /* qxk_h */

Q_DEFINE_THIS_MODULE("qxk")

/* Public-scope objects *****************************************************/
QXK_Attr QXK_attr_; /* global attributes of the QXK kernel */

/* Local-scope objects ******************************************************/
static QXThread l_idleThread;

/****************************************************************************/
/**
* @description
* Initializes QF and must be called exactly once before any other QF
* function. Typically, QF_init() is called from main() even before
* initializing the Board Support Package (BSP).
*
* @note QF_init() clears the internal QF variables, so that the framework
* can start correctly even if the startup code fails to clear the
* uninitialized data (as is required by the C Standard).
*/
void QF_init(void) {
    QF_maxPool_      = (uint_fast8_t)0;
    QF_subscrList_   = (QSubscrList *)0;
    QF_maxPubSignal_ = (enum_t)0;

    QF_maxPool_ = (uint_fast8_t)0;
    QF_bzero(&QF_timeEvtHead_[0], (uint_fast16_t)sizeof(QF_timeEvtHead_));
    QF_bzero(&QF_active_[0],      (uint_fast16_t)sizeof(QF_active_));
    QF_bzero(&QXK_attr_,          (uint_fast16_t)sizeof(QXK_attr_));
    QF_bzero(&l_idleThread,       (uint_fast16_t)sizeof(l_idleThread));

    /* setup the QXK scheduler as initially locked and not running */
    QXK_attr_.lockPrio = (uint_fast8_t)(QF_MAX_ACTIVE + 1);

    /* setup the QXK idle loop... */
    QF_active_[0] = &l_idleThread.super; /* register idle thread with QF */
    QXK_attr_.actPrio = (uint_fast8_t)0; /* set the idle thread priority */

#ifdef QXK_INIT
    QXK_INIT(); /* port-specific initialization of the QXK kernel */
#endif
}

/****************************************************************************/
/**
* @description
* This function stops the QF application. After calling this function,
* QF attempts to gracefully stop the application. This graceful shutdown
* might take some time to complete. The typical use of this function is
* for terminating the QF application to return back to the operating
* system or for handling fatal errors that require shutting down
* (and possibly re-setting) the system.
*
* @sa QF_onCleanup()
*/
void QF_stop(void) {
    QF_onCleanup(); /* application-specific cleanup callback */
    /* nothing else to do for the preemptive QXK kernel */
}

/****************************************************************************/
/*! process all events posted during initialization */
static void initial_events(void); /* prototype */
static void initial_events(void) {
    QXK_attr_.lockPrio = (uint_fast8_t)0; /* unlock the scheduler */

    /* any active objects need to be scheduled before starting event loop? */
    if (QXK_sched_() != (uint_fast8_t)0) {
        QXK_activate_(); /* activate AOs to process all events posted so far*/
    }
}

/****************************************************************************/
/**
* @description
* QP::QF::run() is typically called from your startup code after you
* initialize the QF and start at least one basic- or extended-thread
* (with QP::QActive::start() or QP::QXThread::start(), respectively).
*
* @returns In QXK, the QF_run() function does not return.
*/
int_t QF_run(void) {
    QF_INT_DISABLE();
    initial_events(); /* process all events posted during initialization */
    QF_onStartup(); /* application-specific startup callback */
    QF_INT_ENABLE();

    /* the QXK idle loop... */
    for (;;) {
        QXK_onIdle(); /* application-specific QXK idle callback */
    }

#ifdef __GNUC__
    return (int_t)0;
#endif
}

/****************************************************************************/
/**
* @description
* Starts execution of the AO and registers the AO with the framework.
* Also takes the top-most initial transition in the AO's state machine.
* This initial transition is taken in the callee's thread of execution.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     prio    priority at which to start the active object
* @param[in]     qSto    pointer to the storage for the ring buffer of the
*                        event queue (used only with the built-in ::QEQueue)
* @param[in]     qLen    length of the event queue [events]
* @param[in]     stkSto  pointer to the stack storage (used only when
*                        per-AO stack is needed)
* @param[in]     stkSize stack size [bytes]
* @param[in]     ie      pointer to the initial event (might be NULL).
*
* @note This function should be called via the macro QACTIVE_START().
*
* @usage
* The following example shows starting an AO when a per-task stack is needed:
* @include qf_start.c
*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    QF_CRIT_STAT_

    /** @pre AO cannot be started from an ISR, the priority must be in range
    * and the stack storage must not be provided, because the QK kernel does
    * not need per-AO stacks.
    */
    Q_REQUIRE_ID(500, (!QXK_ISR_CONTEXT_())
                      && ((uint_fast8_t)0 < prio)
                      && (prio <= (uint_fast8_t)QF_MAX_ACTIVE)
                      && (stkSto == (void *)0)
                      && (stkSize == (uint_fast16_t)0));

    QEQueue_init(&me->eQueue, qSto, qLen); /* initialize the built-in queue */
    me->osObject = (void *)0; /* no private stack for AO */
    me->prio = prio;   /* set the current priority of the AO */
    QF_add_(me); /* make QF aware of this active object */

    QHSM_INIT(&me->super, ie); /* take the top-most initial tran. */
    QS_FLUSH();                /* flush the trace buffer to the host */

    /* see if this AO needs to be scheduled in case QXK is already running */
    QF_CRIT_ENTRY_();
    if (QXK_sched_() != (uint_fast8_t)0) { /* activation needed? */
        QXK_activate_();
    }
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* This function must be called from within the AO that needs to stop.
* In other words, an AO should stop itself rather than being stopped by
* someone else. This policy works best, because only the AO itself "knows"
* when it has reached the appropriate state for the shutdown.
*
* @note By the time the AO calls QActive_stop(), it should have unsubscribed
* from all events and no more events should be directly-posted to it.
*/
void QActive_stop(QActive * const me) {
    QF_CRIT_STAT_

    /** @pre QActive_stop() must be called from the AO that wants to stop. */
    Q_REQUIRE_ID(600, (me == QF_active_[QXK_attr_.actPrio]));

    QF_remove_(me); /* remove this active object from the QF */

    QF_CRIT_ENTRY_();
    QPSet_remove(&QXK_attr_.readySet, me->prio);
    if (QXK_sched_() != (uint_fast8_t)0) {
        QXK_activate_();
    }
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* The QXK scheduler finds the priority of the highest-priority thread
* that is ready to run.
*
* @returns the 1-based priority of the the active object to run next,
* or zero if no eligible active object is found.
*
* @attention
* QXK_sched_() must be always called with interrupts **disabled** and
* returns with interrupts **disabled**.
*/
uint_fast8_t QXK_sched_(void) {
    uint_fast8_t p; /* next thread priority */
    QActive *next;

    /* find the highest-prio thread ready to run */
    QPSet_findMax(&QXK_attr_.readySet, p);

    if (p <= QXK_attr_.lockPrio) { /* is it below the lock prio? */
        p = QXK_attr_.lockHolder; /* prio of the thread holding the lock */
    }
    next = QF_active_[p];

    /* the thread found must be registered in QF */
    Q_ASSERT_ID(610, next != (QActive *)0);

    /* is the current thread a basic-thread? */
    if (QXK_attr_.curr == (struct QActive *)0) {

        /* is the next a basic-thread? */
        if (next->osObject == (void *)0) {
            if (p <= QXK_attr_.actPrio) {
                QXK_attr_.next = (struct QActive *)0;
                p = (uint_fast8_t)0; /* no activation needed */
            }
            else {
                QXK_attr_.next = next;
            }
        }
        else {  /* this is an extened-thread */

            QS_BEGIN_NOCRIT_(QS_SCHED_NEXT, QS_priv_.locFilter[AO_OBJ],
                             QXK_attr_.next)
                QS_TIME_();         /* timestamp */
                QS_2U8_((uint8_t)p, /* priority of the next AO */
                                    /* priority of the curent AO */
                        (uint8_t)QXK_attr_.actPrio);
            QS_END_NOCRIT_()

            QXK_attr_.next = next;
            p = (uint_fast8_t)0; /* no activation needed */
            QXK_CONTEXT_SWITCH_();
        }
    }
    else { /* currently executing an extended-thread */

        /* is the new prio different from the current prio? */
        if (p != QXK_attr_.curr->prio) {

            QS_BEGIN_NOCRIT_(QS_SCHED_NEXT, QS_priv_.locFilter[AO_OBJ],
                             QXK_attr_.next)
                QS_TIME_();         /* timestamp */
                QS_2U8_((uint8_t)p, /* priority of the next AO */
                                    /* priority of the curent AO */
                        (uint8_t)QXK_attr_.curr->prio);
            QS_END_NOCRIT_()

            QXK_attr_.next = next;
            p = (uint_fast8_t)0; /* no activation needed */
            QXK_CONTEXT_SWITCH_();
        }
        else {
            QXK_attr_.next = (struct QActive *)0;
            p = (uint_fast8_t)0; /* no activation needed */
        }
    }
    return p;
}

/****************************************************************************/
/**
* @attention
* QXK_activate_() must be always called with interrupts **disabled**  and
* returns with interrupts **disabled**.
*
* @note
* The activate function might enable interrupts internally, but it always
* returns with interrupts **disabled**.
*/
void QXK_activate_(void) {
    uint_fast8_t pin = QXK_attr_.actPrio; /* save the initial active prio */
    uint_fast8_t p   = QXK_attr_.next->prio;
    QActive *a;

    /* QS tracing or thread-local storage? */
#ifdef Q_SPY
    uint_fast8_t pprev = pin;
#endif /* Q_SPY */

    /* loop until no more ready-to-run AOs of higher prio than the initial */
    do  {
        QEvt const *e;

        a = QF_active_[p]; /* obtain the pointer to the AO */

        QXK_attr_.actPrio = p; /* this becomes the active prio */
        QXK_attr_.next = (struct QActive *)0; /* clear the next AO */

        QS_BEGIN_NOCRIT_(QS_SCHED_NEXT, QS_priv_.locFilter[AO_OBJ], a)
            QS_TIME_();            /* timestamp */
            QS_2U8_((uint8_t)p, /* prio of the scheduled AO */
                    (uint8_t)pprev); /* previous priority */
        QS_END_NOCRIT_()

#ifdef Q_SPY
        if (p != pprev) {  /* changing priorities? */
            pprev = p;     /* update previous priority */
        }
#endif /* Q_SPY */

        QF_INT_ENABLE(); /* unconditionally enable interrupts */

        /* perform the run-to-completion (RTC) step...
        * 1. retrieve the event from the AO's event queue, which by this
        *    time must be non-empty and QActive_get_() asserts it.
        * 2. dispatch the event to the AO's state machine.
        * 3. determine if event is garbage and collect it if so
        */
        e = QActive_get_(a);
        QHSM_DISPATCH(&a->super, e);
        QF_gc(e);

        QF_INT_DISABLE(); /* unconditionally disable interrupts */

        if (a->eQueue.frontEvt == (QEvt const *)0) { /* empty queue? */
            QPSet_remove(&QXK_attr_.readySet, p);
        }

        /* find new highest-prio AO ready to run... */
        QPSet_findMax(&QXK_attr_.readySet, p);

        if (p <= QXK_attr_.lockPrio) { /* is it below the lock prio? */
            p = QXK_attr_.lockHolder; /* prio of the thread holding lock */
        }
        a = QF_active_[p];
        Q_ASSERT_ID(710, a != (QActive *)0); /* must be registered */

        /* is the next an AO-thread? */
        if (a->osObject == (void *)0) {
            if (p <= pin) {
                QXK_attr_.next = (struct QActive *)0;
                p = (uint_fast8_t)0; /* no activation needed */
            }
            else {
                QXK_attr_.next = a;
            }
        }
        else {  /* next is the extened thread */

            QS_BEGIN_NOCRIT_(QS_SCHED_NEXT, QS_priv_.locFilter[AO_OBJ],
                             QXK_attr_.next)
                QS_TIME_();         /* timestamp */
                QS_2U8_((uint8_t)p, /* priority of the next AO */
                                    /* priority of the curent AO */
                        (uint8_t)QXK_attr_.actPrio);
            QS_END_NOCRIT_()

            QXK_attr_.next = a;
            p = (uint_fast8_t)0; /* no activation needed */
            QXK_CONTEXT_SWITCH_();
        }
    } while (p != (uint_fast8_t)0); /* while activation needed */

    QXK_attr_.actPrio = pin; /* restore the active priority */

#ifdef Q_SPY
    if (pin != (uint_fast8_t)0) { /* resuming an active object? */
        a = QF_active_[pin]; /* the pointer to the preempted AO */

        QS_BEGIN_NOCRIT_(QS_SCHED_RESUME, QS_priv_.locFilter[AO_OBJ], a)
            QS_TIME_();              /* timestamp */
            QS_2U8_((uint8_t)pin,    /* priority of the resumed AO */
                    (uint8_t)pprev); /* previous priority */
        QS_END_NOCRIT_()
    }
    else {  /* resuming priority==0 --> idle */
        QS_BEGIN_NOCRIT_(QS_SCHED_IDLE, (void *)0, (void *)0)
            QS_TIME_();              /* timestamp */
            QS_U8_((uint8_t)pprev);  /* previous priority */
        QS_END_NOCRIT_()
    }
#endif /* Q_SPY */
}

/****************************************************************************/
struct QActive *QXK_current(void) {
    struct QActive *curr;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    curr = QXK_attr_.curr;
    QF_CRIT_EXIT_();

    /** @post the current thread must be valid */
    Q_ENSURE_ID(900, curr != (struct QActive *)0);

    return curr;
}

