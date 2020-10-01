/**
* @file
* @brief QK preemptive kernel implementation
* @ingroup qk
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-18
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
#include "qf_pkg.h"       /* QF package-scope internal interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS facilities for pre-defined trace records */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

/* protection against including this source file in a wrong project */
#ifndef QK_H
    #error "Source file included in a project NOT based on the QK kernel"
#endif /* QK_H */

Q_DEFINE_THIS_MODULE("qk")

/* Global-scope objects *****************************************************/
QK_PrivAttr QK_attr_; /* private attributes of the QK kernel */

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
    QF_maxPool_      = 0U;
    QF_subscrList_   = (QSubscrList *)0;
    QF_maxPubSignal_ = 0;

    QF_bzero(&QF_timeEvtHead_[0], sizeof(QF_timeEvtHead_));
    QF_bzero(&QF_active_[0],      sizeof(QF_active_));
    QF_bzero(&QK_attr_,           sizeof(QK_attr_));

    QK_attr_.actPrio  = 0U; /* priority of the QK idle loop */
    QK_attr_.lockPrio = QF_MAX_ACTIVE; /* scheduler locked */

#ifdef QK_INIT
    QK_INIT(); /* port-specific initialization of the QK kernel */
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
*
* @attention
* After calling QF_stop() the application must terminate and cannot
* continue. In particular, QF_stop() is **not** intended to be followed
* by a call to QF_init() to "resurrect" the application.
*
* @sa QF_onCleanup()
*/
void QF_stop(void) {
    QF_onCleanup(); /* application-specific cleanup callback */
    /* nothing else to do for the preemptive QK kernel */
}

/****************************************************************************/
/*! process all events posted during initialization */
static void initial_events(void); /* prototype */
static void initial_events(void) {
    QK_attr_.lockPrio = 0U; /* scheduler unlocked */

    /* any active objects need to be scheduled before starting event loop? */
    if (QK_sched_() != 0U) {
        QK_activate_(); /* activate AOs to process all events posted so far */
    }
}

/****************************************************************************/
/**
* @description
* QF_run() is typically called from your startup code after you initialize
* the QF and start at least one active object with QACTIVE_START().
*
* @returns
* In QK, the QF_run() does not return.
*/
int_t QF_run(void) {
    QF_INT_DISABLE();
    initial_events(); /* process all events posted during initialization */
    QF_onStartup(); /* application-specific startup callback */

    /* produce the QS_QF_RUN trace record */
    QS_BEGIN_NOCRIT_PRE_(QS_QF_RUN, 0U)
    QS_END_NOCRIT_PRE_()

    QF_INT_ENABLE();

    /* the QK idle loop... */
    for (;;) {
        QK_onIdle(); /* application-specific QK on-idle callback */
    }
#ifdef __GNUC__
    return 0;
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
* @param[in]     stkSto  pointer to the stack storage (must be NULL in QK)
* @param[in]     stkSize stack size [bytes]
* @param[in]     par     pointer to an extra parameter (might be NULL).
*
* @note This function should be called via the macro QACTIVE_START().
*
* @usage
* The following example shows starting an AO when a per-task stack is needed:
* @include qf_start.c
*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    QF_CRIT_STAT_

    /** @pre AO cannot be started from an ISR, the priority must be in range
    * and the stack storage must not be provided, because the QK kernel does
    * not need per-AO stacks.
    */
    Q_REQUIRE_ID(300, (!QK_ISR_CONTEXT_())
                      && (0U < prio) && (prio <= QF_MAX_ACTIVE)
                      && (stkSto == (void *)0));

    (void)stkSize; /* unused parameter */

    QEQueue_init(&me->eQueue, qSto, qLen); /* initialize the built-in queue */

    me->prio = (uint8_t)prio; /* set the QF priority of the AO */
    QF_add_(me); /* make QF aware of this active object */

    QHSM_INIT(&me->super, par, me->prio); /* top-most initial tran. */
    QS_FLUSH(); /* flush the trace buffer to the host */

    /* See if this AO needs to be scheduled in case QK is already running */
    QF_CRIT_E_();
    if (QK_sched_() != 0U) { /* activation needed? */
        QK_activate_();
    }
    QF_CRIT_X_();
}

/****************************************************************************/
/**
* @description
* This function locks the QK scheduler to the specified ceiling.
*
* @param[in]   ceiling    priority ceiling to which the QK scheduler
*                         needs to be locked
*
* @returns
* The previous QK Scheduler lock status, which is to be used to unlock
* the scheduler by restoring its previous lock status in QK_schedUnlock().
*
* @note
* QK_schedLock() must be always followed by the corresponding
* QK_schedUnlock().
*
* @sa QK_schedUnlock()
*
* @usage
* The following example shows how to lock and unlock the QK scheduler:
* @include qk_lock.c
*/
QSchedStatus QK_schedLock(uint_fast8_t ceiling) {
    QSchedStatus stat;
    QF_CRIT_STAT_
    QF_CRIT_E_();

    /** @pre The QK scheduler lock:
    * - cannot be called from an ISR;
    */
    Q_REQUIRE_ID(600, !QK_ISR_CONTEXT_());

    /* first store the previous lock prio */
    if (QK_attr_.lockPrio < ceiling) { /* raising lock prio? */
        stat = ((QSchedStatus)QK_attr_.lockPrio << 8);
        QK_attr_.lockPrio = (uint8_t)ceiling;

        QS_BEGIN_NOCRIT_PRE_(QS_SCHED_LOCK, 0U)
            QS_TIME_PRE_();   /* timestamp */
            QS_2U8_PRE_(stat, /* the previous lock prio */
                        QK_attr_.lockPrio); /* the new lock prio */
        QS_END_NOCRIT_PRE_()

        /* add the previous lock holder priority */
        stat |= (QSchedStatus)QK_attr_.lockHolder;

        QK_attr_.lockHolder = QK_attr_.actPrio;
    }
    else {
       stat = 0xFFU;
    }
    QF_CRIT_X_();

    return stat; /* return the status to be saved in a stack variable */
}

/****************************************************************************/
/**
* @description
* This function unlocks the QK scheduler to the previous status.
*
* @param[in]   stat       previous QK Scheduler lock status returned from
*                         QK_schedLock()
* @note
* QK_schedUnlock() must always follow the corresponding QK_schedLock().
*
* @sa QK_schedLock()
*
* @usage
* The following example shows how to lock and unlock the QK scheduler:
* @include qk_lock.c
*/
void QK_schedUnlock(QSchedStatus stat) {
    /* has the scheduler been actually locked by the last QK_schedLock()? */
    if (stat != 0xFFU) {
        uint_fast8_t lockPrio = (uint_fast8_t)QK_attr_.lockPrio;
        uint_fast8_t prevPrio = (uint_fast8_t)(stat >> 8);
        QF_CRIT_STAT_
        QF_CRIT_E_();

        /** @pre The scheduler cannot be unlocked:
        * - from the ISR context; and
        * - the current lock priority must be greater than the previous
        */
        Q_REQUIRE_ID(700, (!QK_ISR_CONTEXT_())
                          && (lockPrio > prevPrio));

        QS_BEGIN_NOCRIT_PRE_(QS_SCHED_UNLOCK, 0U)
            QS_TIME_PRE_(); /* timestamp */
            QS_2U8_PRE_(lockPrio,  /* lock prio before unlocking */
                        prevPrio); /* lock prio after unlocking */
        QS_END_NOCRIT_PRE_()

        /* restore the previous lock priority and lock holder */
        QK_attr_.lockPrio   = (uint8_t)prevPrio;
        QK_attr_.lockHolder = (uint8_t)(stat & 0xFFU);

        /* find the highest-prio thread ready to run */
        if (QK_sched_() != 0U) { /* priority found? */
            QK_activate_(); /* activate any unlocked basic threads */
        }

        QF_CRIT_X_();
    }
}

/****************************************************************************/
/**
* @description
* The QK scheduler finds out the priority of the highest-priority AO
* that (1) has events to process and (2) has priority that is above the
* current priority.
*
* @returns
* the 1-based priority of the the active object, or zero if no eligible
* active object is ready to run.
*
* @attention
* QK_sched_() must be always called with interrupts **disabled** and
* returns with interrupts **disabled**.
*/
uint_fast8_t QK_sched_(void) {
    uint_fast8_t p; /* for priority */

    /* find the highest-prio AO with non-empty event queue */
    QPSet_findMax(&QK_attr_.readySet, p);

    /* is the highest-prio below the active priority? */
    if (p <= (uint_fast8_t)QK_attr_.actPrio) {
        p = 0U; /* no activation needed */
    }
    else if (p <= (uint_fast8_t)QK_attr_.lockPrio) { /* below the lock prio?*/
        p = 0U; /* no activation needed */
    }
    else {
        Q_ASSERT_ID(410, p <= QF_MAX_ACTIVE);
        QK_attr_.nextPrio = (uint8_t)p; /* next AO to run */
    }
    return p;
}

/****************************************************************************/
/**
* @description
* QK_activate_() activates ready-to run AOs that are above the initial
* active priority (QK_attr_.actPrio).
*
* @note
* The activator might enable interrupts internally, but always returns with
* interrupts **disabled**.
*/
void QK_activate_(void) {
    uint_fast8_t const pin = (uint_fast8_t)QK_attr_.actPrio;  /* save */
    uint_fast8_t p = (uint_fast8_t)QK_attr_.nextPrio; /* next prio to run */
    QActive *a;
#if (defined QK_ON_CONTEXT_SW) || (defined Q_SPY)
    uint_fast8_t pprev;
#endif /* QK_ON_CONTEXT_SW || Q_SPY */

    /* QK_attr_.actPrio and QK_attr_.nextPrio must be in ragne */
    Q_REQUIRE_ID(500, (pin < Q_DIM(QF_active_))
                      && (0U < p)
                      && (p < Q_DIM(QF_active_)));

#if (defined QK_ON_CONTEXT_SW) || (defined Q_SPY)
    pprev = pin;
#endif /* QK_ON_CONTEXT_SW || Q_SPY */

    QK_attr_.nextPrio = 0U; /* clear for the next time */

    /* loop until no more ready-to-run AOs of higher prio than the initial */
    do  {
        QEvt const *e;
        a = QF_active_[p]; /* obtain the pointer to the AO */
        QK_attr_.actPrio = (uint8_t)p; /* this becomes the active prio */

        QS_BEGIN_NOCRIT_PRE_(QS_SCHED_NEXT, a->prio)
            QS_TIME_PRE_();     /* timestamp */
            QS_2U8_PRE_(p,      /* priority of the scheduled AO */
                        pprev); /* previous priority */
        QS_END_NOCRIT_PRE_()

#if (defined QK_ON_CONTEXT_SW) || (defined Q_SPY)
        if (p != pprev) {  /* changing threads? */

#ifdef QK_ON_CONTEXT_SW
            /* context-switch callback*/
            QK_onContextSw(((pprev != 0U)
                            ? QF_active_[pprev]
                            : (QActive *)0), a);
#endif /* QK_ON_CONTEXT_SW */

            pprev = p;     /* update previous priority */
        }
#endif /* QK_ON_CONTEXT_SW || Q_SPY */

        QF_INT_ENABLE(); /* unconditionally enable interrupts */

        /* perform the run-to-completion (RTC) step...
        * 1. retrieve the event from the AO's event queue, which by this
        *    time must be non-empty and QActive_get_() asserts it.
        * 2. dispatch the event to the AO's state machine.
        * 3. determine if event is garbage and collect it if so
        */
        e = QActive_get_(a);
        QHSM_DISPATCH(&a->super, e, a->prio);
        QF_gc(e);

        /* determine the next highest-priority AO ready to run... */
        QF_INT_DISABLE(); /* unconditionally disable interrupts */

        if (a->eQueue.frontEvt == (QEvt *)0) { /* empty queue? */
            QPSet_remove(&QK_attr_.readySet, p);
        }

        /* find new highest-prio AO ready to run... */
        QPSet_findMax(&QK_attr_.readySet, p);

        /* is the new priority below the initial preemption threshold? */
        if (p <= pin) {
            p = 0U;
        }
        else if (p <= (uint_fast8_t)QK_attr_.lockPrio) {/* below lock prio? */
            p = 0U; /* active object not eligible */
        }
        else {
            Q_ASSERT_ID(510, p <= QF_MAX_ACTIVE);
        }
    } while (p != 0U);

    QK_attr_.actPrio = (uint8_t)pin; /* restore the active priority */

#if (defined QK_ON_CONTEXT_SW) || (defined Q_SPY)
    if (pin != 0U) { /* resuming an active object? */
        a = QF_active_[pin]; /* the pointer to the preempted AO */

        QS_BEGIN_NOCRIT_PRE_(QS_SCHED_RESUME, a->prio)
            QS_TIME_PRE_();     /* timestamp */
            QS_2U8_PRE_(pin,    /* priority of the resumed AO */
                        pprev); /* previous priority */
        QS_END_NOCRIT_PRE_()
    }
    else {  /* resuming priority==0 --> idle */
        a = (QActive *)0; /* QK idle loop */

        QS_BEGIN_NOCRIT_PRE_(QS_SCHED_IDLE, 0U)
            QS_TIME_PRE_();     /* timestamp */
            QS_U8_PRE_(pprev);  /* previous priority */
        QS_END_NOCRIT_PRE_()
    }

#ifdef QK_ON_CONTEXT_SW
    QK_onContextSw(QF_active_[pprev], a); /* context-switch callback */
#endif /* QK_ON_CONTEXT_SW */

#endif /* QK_ON_CONTEXT_SW || Q_SPY */
}

