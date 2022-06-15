/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-06-14
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief Cooperative QV kernel, definition of QP::QV_attr_.readySet and
* implementation of kernel-specific functions.
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

/* Package-scope objects ===================================================*/
QV QV_attr_; /*  private attributes of the QV kernel */

/*..........................................................................*/
void QF_init(void) {
    QF_maxPool_      = 0U;
    QF_subscrList_   = (QSubscrList *)0;
    QF_maxPubSignal_ = 0;

    QF_bzero(&QF_timeEvtHead_[0], sizeof(QF_timeEvtHead_));
    QF_bzero(&QF_active_[0],      sizeof(QF_active_));
    QF_bzero(&QV_attr_.readySet,  sizeof(QV_attr_.readySet));

#ifdef QV_INIT
    QV_INIT(); /* port-specific initialization of the QV kernel */
#endif
}

/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup(); /* application-specific cleanup callback */
    /* nothing else to do for the cooperative QV kernel */
}

/*..........................................................................*/
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
        /* find the maximum priority AO ready to run */
        if (QPSet_notEmpty(&QV_attr_.readySet)) {
            uint_fast8_t const p = QPSet_findMax(&QV_attr_.readySet);
            QActive * const a = QF_active_[p];

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
            *    time must be non-empty and The QV kernel asserts it.
            * 2. dispatch the event to the AO's state machine.
            * 3. determine if event is garbage and collect it if so
            */
            QEvt const * const e = QActive_get_(a);
            QHSM_DISPATCH(&a->super, e, a->prio);
            QF_gc(e);

            QF_INT_DISABLE();

            if (a->eQueue.frontEvt == (QEvt *)0) { /* empty queue? */
                QPSet_remove(&QV_attr_.readySet, p);
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

/*..........................................................................*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    (void)stkSize; /* unused parameter */

    /*! @pre The priority must be in range and the stack storage must not
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
