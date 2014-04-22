/**
* \file
* \brief cooperative "vanilla" kernel, definition of ::QF_readySet_ and
* implementation of kernel-specific functins.
* \ingroup qf
* \cond
******************************************************************************
* Product: QEP/C
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
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qvanilla")

/**
* \note The functions implemented in this module can have a different
* implementation in other QF ports. The implementations included here
* are appropriate for the "vanilla" cooperative kernel only.
*/

/* Package-scope objects ****************************************************/
#if (QF_MAX_ACTIVE <= 8)
    QPSet8  QF_readySet_; /* QF-ready set of active objects */
#else
    QPSet64 QF_readySet_; /* QF-ready set of active objects */
#endif

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
    /* clear the internal QF variables, so that the framework can start
    * correctly even if the startup code fails to clear the uninitialized
    * data (as is required by the C Standard).
    */
    QF_maxPool_ = (uint_fast8_t)0;
    QF_bzero(&QF_readySet_,       (uint_fast16_t)sizeof(QF_readySet_));
    QF_bzero(&QF_timeEvtHead_[0], (uint_fast16_t)sizeof(QF_timeEvtHead_));
    QF_bzero(&QF_active_[0],      (uint_fast16_t)sizeof(QF_active_));
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

    QF_onStartup(); /* application-specific startup callback */

    /* the combined event-loop and background-loop of the "Vanilla" kernel */
    for (;;) {
        QEvt const *e;
        QActive *a;
        uint_fast8_t p;

        QF_INT_DISABLE();

        /* find the maximum priority AO ready to run */
#if (QF_MAX_ACTIVE <= 8)
        if (QPSet8_notEmpty(&QF_readySet_)) {
            QPSet8_findMax(&QF_readySet_, p);
#else
        if (QPSet64_notEmpty(&QF_readySet_)) {
            QPSet64_findMax(&QF_readySet_, p);
#endif
            a = QF_active_[p];
            QF_INT_ENABLE();

            /* perform the run-to-completion (RTS) step...
            * 1. retrieve the event from the AO's event queue, which by this
            *    time must be non-empty and The "Vanialla" kernel asserts it.
            * 2. dispatch the event to the AO's state machine.
            * 3. determine if event is garbage and collect it if so
            */
            e = QActive_get_(a);
            QMSM_DISPATCH(&a->super, e);
            QF_gc(e);
        }
        else {
            /* QF_onIdle() must be called with interrupts DISABLED because
            * the determination of the idle condition (no events in the
            * queues) can change at any time by an interrupt posting events
            * to a queue. QF_onIdle() MUST enable interrupts internally,
            * perhaps at the same time as putting the CPU into a power-saving
            * mode.
            */
            QF_onIdle();
        }
    }
#ifdef __GNUC__  /* GNU compiler? */
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
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    /** \pre the priority must be in range and the stack storage must not
    * be provided, because "Vanilla" kernel does not need per-AO stacks.
    */
    Q_REQUIRE_ID(400, ((uint_fast8_t)0 < prio)
                && (prio <= (uint_fast8_t)QF_MAX_ACTIVE)
              && (stkSto == (void *)0));

    (void)stkSize; /* avoid the "unused parameter" compiler warning */

    QEQueue_init(&me->eQueue, qSto, qLen);
    me->prio = prio; /* set QF priority of this AO before adding it to QF */
    QF_add_(me);     /* make QF aware of this active object */
    QMSM_INIT(&me->super, ie); /* execute top-most initial transition */

    QS_FLUSH(); /* flush the QS trace buffer to the host */
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
void QActive_stop(QActive * const me) {
    QF_remove_(me);  /* remove the AO from the framework */
}
