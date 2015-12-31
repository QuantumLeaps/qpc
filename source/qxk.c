/**
* @file
* @brief QXK preemptive kernel core functions
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 5.6.1
* Last updated on  2015-12-30
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
* http://www.state-machine.com
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

static void thread_ao(void *par);
static void thread_idle(void *par);


/****************************************************************************/
static void thread_ao(void *par) { /* signature of QXThreadHandler */
    /* event-loop of an AO's thread... */
    for (;;) {
        QEvt const *e = QActive_get_((QMActive *)par);
        QMSM_DISPATCH((QMsm *)par, e); /* dispatch to the AO's SM */
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }
}

/****************************************************************************/
static void thread_idle(void *par) { /* signature of QXThreadHandler */
    (void)par;

    QF_INT_DISABLE();
    QF_onStartup(); /* application-specific startup callback */
    QF_INT_ENABLE();

    for (;;) {
        QXK_onIdle();
    }
}

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

    /* clear the internal QF variables, so that the framework can start
    * correctly even if the startup code fails to clear the uninitialized
    * data (as is required by the C Standard).
    */
    QF_maxPool_ = (uint_fast8_t)0;
    QF_bzero(&QF_timeEvtHead_[0], (uint_fast16_t)sizeof(QF_timeEvtHead_));
    QF_bzero(&QF_active_[0],      (uint_fast16_t)sizeof(QF_active_));
    QF_bzero(&QXK_attr_,          (uint_fast16_t)sizeof(QXK_attr_));
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
/**
* @description
* QF_run() is typically called from your startup code after you initialize
* the QF and start at least one active object or "naked" thrad (with
* QACTIVE_START() or QXTHREAD_START(), respectively).
*
* @returns QF_run() typically does not return in embedded applications.
* However, when QP runs on top of an operating system,  QF_run() might
* return and in this case the return represents the error code (0 for
* success). Typically the value returned from QF_run() is subsequently
* passed on as return from main().
*/
int_t QF_run(void) {
    uint_fast8_t p; /* next priority to run */

    /** @pre QXK_init() must be called __before__ QF_run() to initialize
    * the QXK idle thread.
    */
    Q_REQUIRE_ID(100, QF_active_[0] == &l_idleThread.super);

    /* switch to the highest-priority task */
    QF_INT_DISABLE();
    QXK_attr_.curr = &l_idleThread; /* mark QXK as running */
    QXK_prioFindMax(&QXK_attr_.readySet, p);
    QXK_attr_.next = QF_active_[p];
    QXK_start_(); /* start QXK multitasking (NOTE: enables interrupts) */

    /* the QXK start should not return, but just in case... */
    Q_ERROR_ID(110);

    return (int_t)0;
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
    QF_CRIT_STAT_

    Q_REQUIRE_ID(200, (!QXK_ISR_CONTEXT_()) /* don't start AO's in an ISR! */
                      && (prio <= (uint_fast8_t)QF_MAX_ACTIVE)
                      && (qSto != (QEvt const **)0)
                      && (qLen != (uint_fast16_t)0)
                      && (stkSto != (void *)0)
                      && (stkSize != (uint_fast16_t)0));

    /* initialize the built-in queue */
    QEQueue_init(&me->eQueue, qSto, qLen);

    /* initialize the stack of the private thread */
    QXK_stackInit_(me, &thread_ao, stkSto, stkSize);

    me->prio = prio;
    me->thread.startPrio = prio;
    QF_add_(me); /* make QF aware of this active object */

    QMSM_INIT(&me->super, ie); /* take the top-most initial tran. */
    QS_FLUSH();                /* flush the trace buffer to the host */

    QF_CRIT_ENTRY_();
    QXK_prioInsert(&QXK_attr_.readySet, me->prio);

    /* Is QXK running? */
    if (QXK_attr_.curr != (void *)0) {
        QXK_sched_();
    }
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* This function must be called from within the AO/thread that needs to stop.
* In other words, an AO should stop itself rather than being stopped by
* someone else. This policy works best, because only the AO itself "knows"
* when it has reached the appropriate state for the shutdown.
*
* @note By the time the AO calls QActive_stop(), it should have unsubscribed
* from all events and no more events should be directly-posted to it.
*/
void QActive_stop(QMActive *me) {
    QF_CRIT_STAT_

    /** @pre QActive_stop() must be called from the AO that wants to stop. */
    Q_REQUIRE_ID(300, (!QXK_ISR_CONTEXT_()) /* don't stop AO's from an ISR! */
                      && (me == QXK_attr_.curr));

    QF_remove_(me); /* remove this active object from the QF */

    QF_CRIT_ENTRY_();
    QXK_prioRemove(&QXK_attr_.readySet, me->prio);
    QXK_sched_();
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* Initializes QXK and must be called by the application exactly once
* before QF_run().
*
* @param[in]     idleStkSto  pointer to the stack storage for the idle thread
* @param[in]     idleStkSize idle thread stack size (in bytes)
*
*
* @note QXK_init() must be called once before QF_run().
*/
void QXK_init(void *idleStkSto, uint_fast16_t idleStkSize) {
    /* create the idle thread... */
    QMActive_ctor(&l_idleThread.super, Q_STATE_CAST(&thread_idle));

    QXK_stackInit_(&l_idleThread,
                   (QXThreadHandler)l_idleThread.super.super.temp.act,
                   idleStkSto, idleStkSize);
    l_idleThread.super.prio = (uint_fast8_t)0; /* idle thread prio is zero */

    QF_INT_DISABLE();
    QF_active_[0] = &l_idleThread.super;
    QF_INT_ENABLE();
}

/****************************************************************************/
/**
* @description
* Peforms QXK scheduling and context switch to the highest-priority thread
* that is ready to run.
*
* @attention
* QXK_sched_() must be always called with interrupts **disabled** and
* returns with interrupts **disabled**.
*/
void QXK_sched_(void) {
    uint_fast8_t p; /* next priority to run */

    /* find the highest-priority thread that is ready to run */
    QXK_prioFindMax(&QXK_attr_.readySet, p);

    /* is the new priority different from the currently executing thread? */
    if (p != ((QMActive volatile *)QXK_attr_.curr)->prio) {

        QXK_attr_.next = QF_active_[p];

        QS_BEGIN_NOCRIT_(QS_QVK_SCHEDULE, QS_priv_.aoObjFilter,
                         QXK_attr_.next)
            QS_TIME_();            /* timestamp */
            QS_2U8_((uint8_t)p,    /* priority of the next AO */
                                   /* priority of the curent AO */
                    (uint8_t)((QMActive volatile *)QXK_attr_.curr)->prio);
        QS_END_NOCRIT_()

        QXK_CONTEXT_SWITCH_();
    }
}

/****************************************************************************/
/**
* @description
* Called when the thread handler function returns.
*
* @note
* Most thread handler functions are structured as endless loops that never
* return. But it is also possible to structure threads as on-shot functions
* that perform their job and return. In that case this function peforms
* cleanup after the thread.
*/
void QXK_threadRet_(void) {
    uint_fast8_t p;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    p = ((QMActive volatile *)QXK_attr_.curr)->prio;
    /* remove this AO/Thread from the QF */
    QF_active_[p] = (QMActive *)0;
    QXK_prioRemove(&QXK_attr_.readySet, p);
    QXK_sched_();
    QF_CRIT_EXIT_();
}
