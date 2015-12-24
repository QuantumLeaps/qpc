/**
* @file
* @brief QK preemptive kernel core functions
* @ingroup qk
* @cond
******************************************************************************
* Last updated for version 5.6.0
* Last updated on  2015-12-23
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
* http://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"       /* QF package-scope internal interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qk")

/* Public-scope objects *****************************************************/
#if (QF_MAX_ACTIVE <= 8)
    QPSet8  QK_readySet_; /* QK ready-set (8-bits) */
#else
    QPSet64 QK_readySet_; /* QK ready-set (64-bits) */
#endif

uint_fast8_t volatile QK_currPrio_;
uint_fast8_t volatile QK_intNest_;

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
    extern uint_fast8_t QF_maxPool_;
    extern QTimeEvt QF_timeEvtHead_[QF_MAX_TICK_RATE];

    QK_currPrio_ = (uint_fast8_t)(QF_MAX_ACTIVE + 1); /* scheduler locked */

#ifndef QK_ISR_CONTEXT_
    QK_intNest_  = (uint_fast8_t)0; /* no nesting level */
#endif /* QK_ISR_CONTEXT_ */

    /* clear the internal QF variables, so that the framework can start
    * correctly even if the startup code fails to clear the uninitialized
    * data (as is required by the C Standard).
    */
    QF_maxPool_  = (uint_fast8_t)0;
    QF_bzero(&QK_readySet_,       (uint_fast16_t)sizeof(QK_readySet_));
    QF_bzero(&QF_timeEvtHead_[0], (uint_fast16_t)sizeof(QF_timeEvtHead_));
    QF_bzero(&QF_active_[0],      (uint_fast16_t)sizeof(QF_active_));

    QK_init(); /* QK-port initialization, might be defined in assembly */
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
    /* nothing else to do for the preemptive QK kernel */
}

/****************************************************************************/
/*! process all events posted during initialization */
static void initial_events(void); /* prototype */
static void initial_events(void) {
    uint_fast8_t p;

    QK_currPrio_ = (uint_fast8_t)0; /* priority of the QK idle loop */
    p = QK_schedPrio_();

    /* any active objects need to be scheduled before starting event loop? */
    if (p != (uint_fast8_t)0) {
        QK_sched_(p); /* process all events produced so far */
    }
}

/****************************************************************************/
/**
* @description
* QF_run() is typically called from your startup code after you initialize
* the QF and start at least one active object with QActive_start().
*
* @returns QF_run() typically does not return in embedded applications.
* However, when QP runs on top of an operating system,  QF_run() might
* return and in this case the return represents the error code (0 for
* success). Typically the value returned from QF_run() is subsequently
* passed on as return from main().
*
* @note This function is strongly platform-dependent and is not implemented
* in the QF, but either in the QF port or in the Board Support Package (BSP)
* for the given application. All QF ports must implement QF_run().
*/
int_t QF_run(void) {
    QF_INT_DISABLE();
    initial_events(); /* process all events posted during initialization */
    QF_onStartup(); /* application-specific startup callback */
    QF_INT_ENABLE();

    /* the QK idle loop... */
    for (;;) {
        QK_onIdle(); /* application-specific QK on-idle callback */
    }

#ifdef __GNUC__ /* GNU compiler? */
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
* @param[in]     qLen    length of the event queue (in events)
* @param[in]     stkSto  pointer to the stack storage (used only when
*                        per-AO stack is needed)
* @param[in]     stkSize stack size (in bytes)
* @param[in]     ie      pointer to the initial event (might be NULL).
*
* @note This function should be called via the macro QACTIVE_START().
*
* @usage
* The following example shows starting an AO when a per-task stack is needed:
* @include qf_start.c
*/
void QActive_start_(QMActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    Q_REQUIRE_ID(500, ((uint_fast8_t)0 < prio)
                      && (prio <= (uint_fast8_t)QF_MAX_ACTIVE));

    QEQueue_init(&me->eQueue, qSto, qLen); /* initialize the built-in queue */
    me->prio = prio;   /* set the current priority of the AO */
    me->thread = prio; /* set the start priority of the AO */
    QF_add_(me); /* make QF aware of this active object */

#ifdef QK_TLS
    me->osObject = (uint_fast8_t)stkSize; /* used for thread flags */
    me->thread = stkSto; /* pointer to the thread-local storage */
#else
    Q_ASSERT_ID(510, (stkSto == (void *)0) && (stkSize == (uint_fast16_t)0));
#endif /* ifdef QK_TLS */

    QMSM_INIT(&me->super, ie); /* take the top-most initial tran. */
    QS_FLUSH();                /* flush the trace buffer to the host */
}

/****************************************************************************/
/**
* @description
* The preferred way of calling this function is from within the active
* object that needs to stop. In other words, an active object should stop
* itself rather than being stopped by someone else. This policy works
* best, because only the active object itself "knows" when it has reached
* the appropriate state for the shutdown.
*
* @note By the time the AO calls QActive_stop(), it should have unsubscribed
* from all events and no more events should be directly-posted to it.
*/
void QActive_stop(QMActive *me) {
    QF_remove_(me); /* remove this active object from the QF */
}

/****************************************************************************/
/**
* @description
* This function finds out the priority of the highest-priority active object
* that (1) has events to process and (2) has priority that is above the
* current priority.
*
* @returns the 1-based priority of the the active object, or zero if
* no eligible active object is ready to run.
*
* @attention
* QK_schedPrio_() must be always called with interrupts **disabled** and
* returns with interrupts **disabled**.
*/
uint_fast8_t QK_schedPrio_(void) {
    uint_fast8_t p; /* for priority */

    /* find the highest-priority AO with non-empty event queue */
    QK_prioFindMax(&QK_readySet_, p);

    /* is the priority below the current preemption threshold? */
    if (p <= QK_currPrio_) {
        p = (uint_fast8_t)0; /* active object not eligible */
    }
    return p;
}

/****************************************************************************/
/**
* @param[in] p  priority of the next AO to schedule
*
* @attention
* QK_sched_() must be always called with interrupts **disabled**  and
* returns with interrupts **disabled**.
*
* @note
* The scheduler might enable interrupts internally, but always
* returns with interrupts __disabled__.
*/
void QK_sched_(uint_fast8_t p) {
    uint_fast8_t pin = QK_currPrio_; /* save the initial priority */
    QMActive *a;

    /* QS tracing or thread-local storage? */
#if (defined Q_SPY) || (defined QK_TLS)
    uint_fast8_t pprev = pin;
#endif /* Q_SPY || QK_TLS */

    /* loop until have ready-to-run AOs of higher priority than the initial */
    do {
        QEvt const *e;
        a = QF_active_[p]; /* obtain the pointer to the AO */

        QK_currPrio_ = p;  /* this becomes the current task priority */

        QS_BEGIN_NOCRIT_(QS_QVK_SCHEDULE, QS_priv_.aoObjFilter, a)
            QS_TIME_();            /* timestamp */
            QS_2U8_((uint8_t)p,    /* priority of the scheduled AO */
                    (uint8_t)pprev); /* previous priority */
        QS_END_NOCRIT_()

        /* QS tracing or thread-local storage? */
#if (defined Q_SPY) || (defined QK_TLS)
        if (p != pprev) {  /* changing priorities? */
            pprev = p;     /* update previous priority */
#ifdef QK_TLS /* thread-local storage used? */
            QK_TLS(a);     /* switch new thread-local storage */
#endif /* QK_TLS */
        }
#endif /* Q_SPY || QK_TLS */

        QF_INT_ENABLE(); /* unconditionally enable interrupts */

        /* perform the run-to-completion (RTC) step...
        * 1. retrieve the event from the AO's event queue, which by this
        *    time must be non-empty and QActive_get_() asserts it.
        * 2. dispatch the event to the AO's state machine.
        * 3. determine if event is garbage and collect it if so
        */
        e = QActive_get_(a);
        QMSM_DISPATCH(&a->super, e);
        QF_gc(e);

        QF_INT_DISABLE(); /* unconditionally disable interrupts */

        /* find new highest-prio AO ready to run... */
        QK_prioFindMax(&QK_readySet_, p);

        /* is the new priority below the current preemption threshold? */
        if (p <= pin) {
            p = (uint_fast8_t)0;
        }

    } while (p != (uint_fast8_t)0);

    QK_currPrio_ = pin; /* restore the initial priority */

    /* QS tracing or thread-local storage? */
#if (defined Q_SPY) || (defined QK_TLS)
    if (pin != (uint_fast8_t)0) { /* resuming an active object? */
        a = QF_active_[pin]; /* the pointer to the preempted AO */

#ifdef QK_TLS /* thread-local storage used? */
        QK_TLS(a); /* restore the original TLS */
#endif /* QK_TLS */

        QS_BEGIN_NOCRIT_(QS_QK_RESUME, QS_priv_.aoObjFilter, a)
            QS_TIME_();              /* timestamp */
            QS_2U8_((uint8_t)pin,    /* priority of the resumed AO */
                    (uint8_t)pprev); /* previous priority */
        QS_END_NOCRIT_()
    }
    else {  /* resuming priority==0 --> idle */
        QS_BEGIN_NOCRIT_(QS_QVK_IDLE, (void *)0, (void *)0)
            QS_TIME_();              /* timestamp */
            QS_U8_((uint8_t)pprev);  /* previous priority */
        QS_END_NOCRIT_()
    }
#endif /* Q_SPY || QK_TLS */
}
