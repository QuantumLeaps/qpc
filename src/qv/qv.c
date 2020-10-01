/**
* @file
* @brief Cooperative QV kernel, definition of QP::QV_readySet_ and
* implementation of kernel-specific functions.
* @ingroup qv
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
#ifndef QV_H
    #error "Source file included in a project NOT based on the QV kernel"
#endif /* QV_H */

Q_DEFINE_THIS_MODULE("qv")

/* Package-scope objects ****************************************************/
QPSet QV_readySet_; /* QV ready-set of active objects */

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
    QF_bzero(&QV_readySet_,       sizeof(QV_readySet_));

#ifdef QV_INIT
    QV_INIT(); /* port-specific initialization of the QV kernel */
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
* @attention
* After calling QF_stop() the application must terminate and cannot
* continue. In particular, QF_stop() is **not** intended to be followed
* by a call to QF_init() to "resurrect" the application.
*
* @sa QF_onCleanup()
*/
void QF_stop(void) {
    QF_onCleanup(); /* application-specific cleanup callback */
    /* nothing else to do for the cooperative QV kernel */
}

/****************************************************************************/
/**
* @description
* QF_run() is typically called from main() after you initialize
* the QF and start at least one active object with QACTIVE_START().
*
* @returns In QV, the QF_run() function does not return.
*/
int_t QF_run(void) {
#ifdef Q_SPY
    uint_fast8_t pprev = 0U; /* previously used priority */
#endif

    QF_onStartup(); /* application-specific startup callback */

    /* the combined event-loop and background-loop of the QV kernel... */
    QF_INT_DISABLE();

    /* produce the QS_QF_RUN trace record */
    QS_BEGIN_NOCRIT_PRE_(QS_QF_RUN, 0U)
    QS_END_NOCRIT_PRE_()

    for (;;) {
        QEvt const *e;
        QActive *a;
        uint_fast8_t p;

        /* find the maximum priority AO ready to run */
        if (QPSet_notEmpty(&QV_readySet_)) {
            QPSet_findMax(&QV_readySet_, p);
            a = QF_active_[p];

#ifdef Q_SPY
            QS_BEGIN_NOCRIT_PRE_(QS_SCHED_NEXT, a->prio)
                QS_TIME_PRE_();     /* timestamp */
                QS_2U8_PRE_(p,      /* priority of the scheduled AO */
                            pprev); /* previous priority */
            QS_END_NOCRIT_PRE_()

            pprev = p; /* update previous priority */
#endif /* Q_SPY */

            QF_INT_ENABLE();

            /* perform the run-to-completion (RTC) step...
            * 1. retrieve the event from the AO's event queue, which by this
            *    time must be non-empty and The "Vanialla" kernel asserts it.
            * 2. dispatch the event to the AO's state machine.
            * 3. determine if event is garbage and collect it if so
            */
            e = QActive_get_(a);
            QHSM_DISPATCH(&a->super, e, a->prio);
            QF_gc(e);

            QF_INT_DISABLE();

            if (a->eQueue.frontEvt == (QEvt *)0) { /* empty queue? */
                QPSet_remove(&QV_readySet_, p);
            }
        }
        else { /* no AO ready to run --> idle */
#ifdef Q_SPY
            if (pprev != 0U) {
                QS_BEGIN_NOCRIT_PRE_(QS_SCHED_IDLE, 0U)
                    QS_TIME_PRE_();    /* timestamp */
                    QS_U8_PRE_(pprev); /* previous priority */
                QS_END_NOCRIT_PRE_()

                pprev = 0U; /* update previous priority */
            }
#endif /* Q_SPY */

            /* QV_onIdle() must be called with interrupts DISABLED because
            * the determination of the idle condition (no events in the
            * queues) can change at any time by an interrupt posting events
            * to a queue. QV_onIdle() MUST enable interrupts internally,
            * perhaps at the same time as putting the CPU into a power-saving
            * mode.
            */
            QV_onIdle();

            QF_INT_DISABLE();
        }
    }
#ifdef __GNUC__  /* GNU compiler? */
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
* @param[in]     stkSto  pointer to the stack storage (must be NULL in QV)
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
    (void)stkSize; /* unused parameter */

    /** @pre The priority must be in range and the stack storage must not
    * be provided, because the QV kernel does not need per-AO stacks.
    */
    Q_REQUIRE_ID(500, (0U < prio) && (prio <= QF_MAX_ACTIVE)
                      && (stkSto == (void *)0));

    QEQueue_init(&me->eQueue, qSto, qLen); /* initialize the built-in queue */
    me->prio = (uint8_t)prio; /* set the current priority of the AO */
    QF_add_(me); /* make QF aware of this active object */

    QHSM_INIT(&me->super, par, me->prio); /* top-most initial tran. */
    QS_FLUSH(); /* flush the trace buffer to the host */
}

