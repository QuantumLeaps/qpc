/**
* @file
* @brief QXK preemptive dual-mode kernel core functions
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 6.2.0
* Last updated on  2018-03-16
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2018 Quantum Leaps, LLC. All rights reserved.
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
* https://www.state-machine.com
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
static QActive l_idleThread;

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
    QXK_attr_.lockPrio = (uint8_t)(QF_MAX_ACTIVE + 1);

    /* setup the QXK idle loop... */
    QF_active_[0] = &l_idleThread; /* register idle thread with QF */
    QXK_attr_.idleThread = &l_idleThread; /* save the idle thread ptr */
    QXK_attr_.actPrio = l_idleThread.prio; /* set the base priority */

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
    QXK_attr_.lockPrio = (uint8_t)0; /* unlock the scheduler */

    /* any active objects need to be scheduled before starting event loop? */
    if (QXK_sched_() != (uint_fast8_t)0) {
        QXK_activate_(); /* activate AOs to process all events posted so far*/
    }
}

/****************************************************************************/
/**
* @description
* QF_run() is typically called from main() after you initialize
* the QF and start at least one active object with QACTIVE_START().
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

    /** @pre AO cannot be started:
    * - from an ISR;
    * - the priority must be in range;
    * - the stack storage must NOT be provided (because the QK kernel does
    * not need per-AO stacks).
    */
    Q_REQUIRE_ID(200, (!QXK_ISR_CONTEXT_())
        && ((uint_fast8_t)0 < prio)
        && (prio <= (uint_fast8_t)QF_MAX_ACTIVE)
        && (stkSto == (void *)0)
        && (stkSize == (uint_fast16_t)0));

    QEQueue_init(&me->eQueue, qSto, qLen); /* initialize the built-in queue */
    me->osObject  = (void *)0; /* no private stack for AO */
    me->prio      = (uint8_t)prio;  /* set the current priority of the AO */
    me->startPrio = (uint8_t)prio;  /* set the start priority of the AO */
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
    Q_REQUIRE_ID(300, (me == QF_active_[QXK_attr_.actPrio]));

    QF_remove_(me); /* remove this active object from the QF */

    QF_CRIT_ENTRY_();
    QPSet_remove(&QXK_attr_.readySet, (uint_fast8_t)me->prio);
    if (QXK_sched_() != (uint_fast8_t)0) {
        QXK_activate_();
    }
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* This function locks the QXK scheduler to the specified ceiling.
*
* @param[in]   ceiling    priority ceiling to which the QXK scheduler
*                         needs to be locked
*
* @returns
* The previous QXK Scheduler lock status, which is to be used to unlock
* the scheduler by restoring its previous lock status in QXK_schedUnlock().
*
* @note
* A QXK scheduler can be locked from both basic threads (AOs) and
* extended threads and the scheduler locks can nest.
*
* @note
* QXK_schedLock() must be always followed by the corresponding
* QXK_schedUnlock().
*
* @attention
* QXK will fire an assertion if a thread holding the lock attempts
* to block.
*
* @sa QXK_schedUnlock()
*
* @usage
* The following example shows how to lock and unlock the QXK scheduler:
* @include qxk_lock.c
*/
QSchedStatus QXK_schedLock(uint_fast8_t ceiling) {
    QSchedStatus stat;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    /** @pre The QXK scheduler lock:
    * - cannot be called from an ISR;
    */
    Q_REQUIRE_ID(400, !QXK_ISR_CONTEXT_());

    /* first store the previous lock prio */
    if ((uint_fast8_t)QXK_attr_.lockPrio < ceiling) { /* raising lock prio? */
        stat = ((QSchedStatus)QXK_attr_.lockPrio << 8);
        QXK_attr_.lockPrio = (uint8_t)ceiling;

        QS_BEGIN_NOCRIT_(QS_SCHED_LOCK, (void *)0, (void *)0)
            QS_TIME_(); /* timestamp */
            QS_2U8_((uint8_t)stat,   /* the previous lock prio */
                    (uint8_t)QXK_attr_.lockPrio); /* the new lock prio */
        QS_END_NOCRIT_()

        /* add the previous lock holder priority */
        stat |= (QSchedStatus)QXK_attr_.lockHolder;
        QXK_attr_.lockHolder =
            (QXK_attr_.curr != (struct QActive *)0)
            ? QXK_attr_.curr->prio
            : (uint8_t)0;
    }
    else {
       stat = (QSchedStatus)0xFF;
    }
    QF_CRIT_EXIT_();

    return stat; /* return the status to be saved in a stack variable */
}

/****************************************************************************/
/**
* @description
* This function unlocks the QXK scheduler to the previous status.
*
* @param[in]   stat       previous QXK Scheduler lock status returned from
*                         QXK_schedLock()
*
* @note
* A QXK scheduler can be locked from both basic threads (AOs) and
* extended threads and the scheduler locks can nest.
*
* @note
* QXK_schedUnlock() must always follow the corresponding QXK_schedLock().
*
* @sa QXK_schedLock()
*
* @usage
* The following example shows how to lock and unlock the QXK scheduler:
* @include qxk_lock.c
*/
void QXK_schedUnlock(QSchedStatus stat) {
    /* has the scheduler been actually locked by the last QXK_schedLock()? */
    if (stat != (QSchedStatus)0xFF) {
        uint_fast8_t lockPrio = (uint_fast8_t)QXK_attr_.lockPrio;
        uint_fast8_t prevPrio = (uint_fast8_t)(stat >> 8);
        QF_CRIT_STAT_
        QF_CRIT_ENTRY_();

        /** @pre The scheduler cannot be unlocked:
        * - from the ISR context; and
        * - the current lock priority must be greater than the previous
        */
        Q_REQUIRE_ID(500, (!QXK_ISR_CONTEXT_())
                          && (lockPrio > prevPrio));

        QS_BEGIN_NOCRIT_(QS_SCHED_UNLOCK, (void *)0, (void *)0)
            QS_TIME_(); /* timestamp */
            QS_2U8_((uint8_t)lockPrio,  /* lock prio before unlocking */
                    (uint8_t)prevPrio); /* lock priority after unlocking */
        QS_END_NOCRIT_()

        /* restore the previous lock priority and lock holder */
        QXK_attr_.lockPrio   = (uint8_t)prevPrio;
        QXK_attr_.lockHolder = (uint8_t)(stat & (QSchedStatus)0xFF);

        /* find the highest-prio thread ready to run */
        if (QXK_sched_() != (uint_fast8_t)0) { /* priority found? */
            QXK_activate_(); /* activate any unlocked basic threads */
        }

        QF_CRIT_EXIT_();
    }
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

    if (p <= (uint_fast8_t)QXK_attr_.lockPrio) { /* below the lock prio? */
        p = (uint_fast8_t)QXK_attr_.lockHolder; /* thread holding the lock */
        Q_ASSERT_ID(610, (p == (uint_fast8_t)0)
                         || QPSet_hasElement(&QXK_attr_.readySet, p));
    }
    next = QF_active_[p];

    /* the thread found must be registered in QF */
    Q_ASSERT_ID(620, next != (QActive *)0);

    /* is the current thread a basic-thread? */
    if (QXK_attr_.curr == (struct QActive *)0) {

        /* is next a basic-thread? */
        if (next->osObject == (void *)0) {
            if (p > (uint_fast8_t)QXK_attr_.actPrio) {
                QXK_attr_.next = next; /* set the next AO to activate */
            }
            else {
                QXK_attr_.next = (struct QActive *)0;
                p = (uint_fast8_t)0; /* no activation needed */
            }
        }
        else {  /* this is an extened-thread */

            QS_BEGIN_NOCRIT_(QS_SCHED_NEXT, QS_priv_.locFilter[AO_OBJ],
                             next)
                QS_TIME_();         /* timestamp */
                QS_2U8_((uint8_t)p, /* priority of the next AO */
                                    /* priority of the curent AO */
                        QXK_attr_.actPrio);
            QS_END_NOCRIT_()

            QXK_attr_.next = next;
            p = (uint_fast8_t)0; /* no activation needed */
            QXK_CONTEXT_SWITCH_();
        }
    }
    else { /* currently executing an extended-thread */

        /* is the next thread different from the current? */
        if (next != QXK_attr_.curr) {

            QS_BEGIN_NOCRIT_(QS_SCHED_NEXT, QS_priv_.locFilter[AO_OBJ],
                             next)
                QS_TIME_();         /* timestamp */
                QS_2U8_((uint8_t)p, /* next prio */
                        QXK_attr_.curr->prio); /* curr prio */
            QS_END_NOCRIT_()

            QXK_attr_.next = next;
            p = (uint_fast8_t)0; /* no activation needed */
            QXK_CONTEXT_SWITCH_();
        }
        else { /* next is the same as the current */
            QXK_attr_.next = (QActive *)0; /* no need to context-switch */
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
    uint_fast8_t pin = (uint_fast8_t)QXK_attr_.actPrio; /* initial act prio */
    QActive *a = QXK_attr_.next; /* next AO (basic-thread) to run */
    uint_fast8_t p;

    /* QXK Context switch callback defined or QS tracing enabled? */
#if (defined QXK_ON_CONTEXT_SW) || (defined Q_SPY)
    uint_fast8_t pprev = pin;
#endif /* QXK_ON_CONTEXT_SW || Q_SPY */

    /* QXK_attr_.next must be valid */
    Q_REQUIRE_ID(700, a != (QActive *)0);

    p = (uint_fast8_t)a->prio; /* the priority of the next AO */

    /* loop until no more ready-to-run AOs of higher prio than the initial */
    do  {
        QEvt const *e;

        a = QF_active_[p]; /* obtain the pointer to the AO */
        QXK_attr_.actPrio = (uint8_t)p; /* this becomes the base prio */
        QXK_attr_.next = (struct QActive *)0; /* clear the next AO */

        QS_BEGIN_NOCRIT_(QS_SCHED_NEXT, QS_priv_.locFilter[AO_OBJ], a)
            QS_TIME_();            /* timestamp */
            QS_2U8_((uint8_t)p, /* prio of the scheduled AO */
                    (uint8_t)pprev); /* previous priority */
        QS_END_NOCRIT_()

#if (defined QXK_ON_CONTEXT_SW) || (defined Q_SPY)
        if (p != pprev) {  /* changing threads? */

#ifdef QXK_ON_CONTEXT_SW
            /* context-switch callback */
            QXK_onContextSw(((pprev != (uint_fast8_t)0)
                             ? QF_active_[pprev]
                             : (QActive *)0),
                             a);
#endif /* QXK_ON_CONTEXT_SW */

            pprev = p;     /* update previous priority */
        }
#endif /* QXK_ON_CONTEXT_SW || Q_SPY */

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

        /* find new highest-prio AO ready to run...
        * NOTE: this part must match the QXK_sched_(),
        * current is a basic-thread path.
        */
        QPSet_findMax(&QXK_attr_.readySet, p);

        if (p <= (uint_fast8_t)QXK_attr_.lockPrio) { /* below lock prio? */
            p = (uint_fast8_t)QXK_attr_.lockHolder; /* thread holding lock */
            Q_ASSERT_ID(710, (p == (uint_fast8_t)0)
                             || QPSet_hasElement(&QXK_attr_.readySet, p));
        }
        a = QF_active_[p];

        /* the AO must be registered in QF */
        Q_ASSERT_ID(720, a != (QActive *)0);

        /* is the next a basic thread? */
        if (a->osObject == (void *)0) {
            if (p > pin) {
                QXK_attr_.next = a;
            }
            else {
                QXK_attr_.next = (struct QActive *)0;
                p = (uint_fast8_t)0; /* no activation needed */
            }
        }
        else {  /* next is the extened thread */

            QS_BEGIN_NOCRIT_(QS_SCHED_NEXT, QS_priv_.locFilter[AO_OBJ], a)
                QS_TIME_();         /* timestamp */
                QS_2U8_((uint8_t)p, /* next prio */
                        QXK_attr_.actPrio); /* curr prio */
            QS_END_NOCRIT_()

            QXK_attr_.next = a;
            p = (uint_fast8_t)0; /* no activation needed */
            QXK_CONTEXT_SWITCH_();
        }
    } while (p != (uint_fast8_t)0); /* while activation needed */

    QXK_attr_.actPrio = (uint8_t)pin; /* restore the base prio */

#if (defined QK_ON_CONTEXT_SW) || (defined Q_SPY)
    if (pin != (uint_fast8_t)0) { /* resuming an active object? */
        a = QF_active_[pin]; /* the pointer to the preempted AO */

        QS_BEGIN_NOCRIT_(QS_SCHED_RESUME, QS_priv_.locFilter[AO_OBJ], a)
            QS_TIME_();              /* timestamp */
            QS_2U8_((uint8_t)pin,    /* resumed prio */
                    (uint8_t)pprev); /* previous prio */
        QS_END_NOCRIT_()
    }
    else {  /* resuming priority==0 --> idle */
        a = (QActive *)0;

        QS_BEGIN_NOCRIT_(QS_SCHED_IDLE, (void *)0, (void *)0)
            QS_TIME_();              /* timestamp */
            QS_U8_((uint8_t)pprev);  /* previous priority */
        QS_END_NOCRIT_()
    }

#ifdef QXK_ON_CONTEXT_SW
    /* context-switch callback */
    QXK_onContextSw(QF_active_[pprev], a);
#endif /* QXK_ON_CONTEXT_SW */

#endif /* QK_ON_CONTEXT_SW || Q_SPY */
}

/****************************************************************************/
struct QActive *QXK_current(void) {
    struct QActive *curr;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();

    /** @pre the QXK kernel must be running */
    Q_REQUIRE_ID(800, QXK_attr_.lockPrio <= (uint8_t)QF_MAX_ACTIVE);

    curr = QXK_attr_.curr;
    if (curr == (struct QActive *)0) { /* basic thread? */
        curr = QF_active_[QXK_attr_.actPrio];
    }
    QF_CRIT_EXIT_();

    /** @post the current thread must be valid */
    Q_ENSURE_ID(890, curr != (struct QActive *)0);

    return curr;
}

