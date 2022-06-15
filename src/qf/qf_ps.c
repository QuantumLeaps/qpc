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
* @brief Publish-Subscribe services
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

/*==========================================================================*/
/* Package-scope objects */
/*! @static @private @memberof QF */
QSubscrList *QF_subscrList_;

/*! @static @private @memberof QF */
enum_t QF_maxPubSignal_;

/*..........................................................................*/
void QF_psInit(QSubscrList * const subscrSto, enum_t const maxSignal) {
    QF_subscrList_   = subscrSto;
    QF_maxPubSignal_ = maxSignal;

    /* zero the subscriber list, so that the framework can start correctly
    * even if the startup code fails to clear the uninitialized data
    * (as is required by the C Standard).
    */
    QF_bzero(subscrSto, (uint_fast16_t)maxSignal * sizeof(QSubscrList));
}

/*..........................................................................*/
#ifndef Q_SPY
void QF_publish_(QEvt const * const e)
#else
void QF_publish_(QEvt const * const e,
                 void const * const sender, uint_fast8_t const qs_id)
#endif
{
    /*! @pre the published signal must be within the configured range */
    Q_REQUIRE_ID(200, e->sig < (QSignal)QF_maxPubSignal_);

    QF_CRIT_STAT_
    QF_CRIT_E_();

    QS_BEGIN_NOCRIT_PRE_(QS_QF_PUBLISH, qs_id)
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
    QPSet subscrList = QF_PTR_AT_(QF_subscrList_, e->sig);
    QF_CRIT_X_();

    if (QPSet_notEmpty(&subscrList)) { /* any subscribers? */
        /* the highest-prio subscriber */;
        uint_fast8_t p = QPSet_findMax(&subscrList);
        QF_SCHED_STAT_

        QF_SCHED_LOCK_(p); /* lock the scheduler up to prio 'p' */
        do { /* loop over all subscribers */
            /* the prio of the AO must be registered with the framework */
            Q_ASSERT_ID(210, QF_active_[p] != (QActive *)0);

            /* QACTIVE_POST() asserts internally if the queue overflows */
            QACTIVE_POST(QF_active_[p], e, sender);

            QPSet_remove(&subscrList, p); /* remove the handled subscriber */
            if (QPSet_notEmpty(&subscrList)) { /* still more subscribers? */
                /* highest-prio subscriber */
                p = QPSet_findMax(&subscrList);
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

/*..........................................................................*/
void QActive_subscribe(QActive const * const me, enum_t const sig) {
    uint_fast8_t const p = (uint_fast8_t)me->prio;

    Q_REQUIRE_ID(300, ((enum_t)Q_USER_SIG <= sig)
              && (sig < QF_maxPubSignal_)
              && (0U < p) && (p <= QF_MAX_ACTIVE)
              && (QF_active_[p] == me));

    QF_CRIT_STAT_
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

/*..........................................................................*/
void QActive_unsubscribe(QActive const * const me, enum_t const sig) {
    uint_fast8_t const p = (uint_fast8_t)me->prio;

    /*! @pre the singal and the prioriy must be in ragne, the AO must also
    * be registered with the framework
    */
    Q_REQUIRE_ID(400, ((enum_t)Q_USER_SIG <= sig)
              && (sig < QF_maxPubSignal_)
              && (0U < p) && (p <= QF_MAX_ACTIVE)
              && (QF_active_[p] == me));

    QF_CRIT_STAT_
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

/*..........................................................................*/
void QActive_unsubscribeAll(QActive const * const me) {
    uint_fast8_t const p = (uint_fast8_t)me->prio;

    Q_REQUIRE_ID(500, (0U < p) && (p <= QF_MAX_ACTIVE)
                        && (QF_active_[p] == me));

    for (enum_t sig = (enum_t)Q_USER_SIG; sig < QF_maxPubSignal_; ++sig) {
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
