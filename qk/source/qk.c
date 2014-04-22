/**
* \file
* \brief preemptive QK kernel, definition of ::QK_currPrio_, ::QK_intNest_,
* as well as implementation of kernel-specific functins.
* definition
* \ingroup qk
* \cond
******************************************************************************
* Product: QK/C
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
#include "qf_port.h"
#include "qk_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qk")

/* Public-scope objects *****************************************************/
#if (QF_MAX_ACTIVE <= 8)
    QPSet8  QK_readySet_; /* QK ready-set */
#else
    QPSet64 QK_readySet_; /* QK ready-set */
#endif

uint_fast8_t volatile QK_currPrio_;
uint_fast8_t volatile QK_intNest_;

/****************************************************************************/
/**
* \description
* Initializes QF and must be called exactly once before any other QF
* function. Typcially, QF_init() is called from main() even before
* initializing the Board Support Package (BSP).
*
* \note QF_init() clears the internal QF variables, so that the framework
* can start correctly even if the startup code fails to clear the
* uninitialized data (as is required by the C Standard).
*/
void QF_init(void) {
    extern uint_fast8_t QF_maxPool_;
    extern QTimeEvt QF_timeEvtHead_[QF_MAX_TICK_RATE];

    QK_intNest_  = (uint_fast8_t)0; /* no nesting level */
    QK_currPrio_ = (uint_fast8_t)(QF_MAX_ACTIVE + 1); /* scheduler locked */
#ifndef QK_NO_MUTEX
    QK_ceilingPrio_ = (uint_fast8_t)0;
#endif
    QF_maxPool_  = (uint_fast8_t)0;
    QF_bzero(&QK_readySet_,       (uint_fast16_t)sizeof(QK_readySet_));
    QF_bzero(&QF_timeEvtHead_[0], (uint_fast16_t)sizeof(QF_timeEvtHead_));
    QF_bzero(&QF_active_[0],      (uint_fast16_t)sizeof(QF_active_));

    QK_init(); /* QK-port initialization, might be defined in assembly */
}

/****************************************************************************/
/**
* \description
* This function stops the QF application. After calling this function,
* QF attempts to gracefully stop the application. This graceful shutdown
* might take some time to complete. The typical use of this function is
* for terminating the QF application to return back to the operating
* system or for handling fatal errors that require shutting down
* (and possibly re-setting) the system.
*
* \sa QF_onCleanup()
*/
void QF_stop(void) {
    QF_onCleanup(); /* application-specific cleanup callback */
    /* nothing else to do for the "vanilla" kernel */
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
* \description
* QF_run() is typically called from your startup code after you initialize
* the QF and start at least one active object with QActive_start().
*
* \returns QF_run() typically does not return in embedded applications.
* However, when QP runs on top of an operating system,  QF_run() might
* return and in this case the return represents the error code (0 for
* success). Typically the value returned from QF_run() is subsequently
* passed on as return from main().
*
* \note This function is strongly platform-dependent and is not implemented
* in the QF, but either in the QF port or in the Board Support Package (BSP)
* for the given application. All QF ports must implement QF_run().
*/
int_t QF_run(void) {
    QF_INT_DISABLE();
    initial_events(); /* process all events posted during initialization */
    QF_onStartup(); /* application-specific startup callback */
    QF_INT_ENABLE();

    /* the QK idle loop */
    for (;;) {
        QK_onIdle(); /* application-specific QK on-idle callback */
    }

#ifdef __GNUC__ /* GNU compiler? */
    return (int_t)0;
#endif
}

/****************************************************************************/
/**
* \description
* Starts execution of the AO and registers the AO with the framework.
*
* \arguments
* \arg[in,out] \c me      pointer (see \ref derivation)
* \arg[in]     \c prio    priority at which to start the active object
* \arg[in]     \c qSto    pointer to the storage for the ring buffer of the
*                         event queue (used only with the built-in ::QEQueue)
* \arg[in]     \c qLen    length of the event queue (in events)
* \arg[in]     \c stkSto  pointer to the stack storage (used only when
*                         per-AO stack is needed)
* \arg[in]     \c stkSize stack size (in bytes)
* \arg[in]     \c ie      pointer to the optional initialization event
*                         (might be NULL).
*
* \note This function should be called via the macro QACTIVE_START().
*
* \usage
* The following example shows starting an AO when a per-task stack is needed:
* \include qf_start.c
*/
void QActive_start_(QActive *me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    Q_REQUIRE_ID(500, ((uint_fast8_t)0 < prio)
                      && (prio <= (uint_fast8_t)QF_MAX_ACTIVE));

    QEQueue_init(&me->eQueue, qSto, qLen); /* initialize the built-in queue */
    me->prio = prio;
    QF_add_(me); /* make QF aware of this active object */

#if defined(QK_TLS) || defined(QK_EXT_SAVE)
    me->osObject = (uint_fast8_t)stkSize; /* used for thread flags */
    me->thread = stkSto; /* pointer to the thread-local storage */
#else
    Q_ASSERT((stkSto == (void *)0) && (stkSize == (uint_fast16_t)0));
#endif

    QMSM_INIT(&me->super, ie); /* execute initial transition */

    QS_FLUSH(); /* flush the trace buffer to the host */
}

/****************************************************************************/
/**
* \description
* The preferred way of calling this function is from within the active
* object that needs to stop. In other words, an active object should stop
* itself rather than being stopped by someone else. This policy works
* best, because only the active object itself "knows" when it has reached
* the appropriate state for the shutdown.
*
* \note By the time the AO calls QActive_stop(), it should have unsubscribed
* from all events and no more events should be directly-posted to it.
*/
void QActive_stop(QActive *me) {
    QF_remove_(me); /* remove this active object from the QF */
}
