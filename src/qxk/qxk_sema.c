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
* @brief QXK preemptive kernel semaphore functions
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qxk_pkg.h"      /* QXK package-scope internal interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* QS port */
    #include "qs_pkg.h"   /* QS facilities for pre-defined trace records */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

/* protection against including this source file in a wrong project */
#ifndef QXK_H
    #error "Source file included in a project NOT based on the QXK kernel"
#endif /* QXK_H */

Q_DEFINE_THIS_MODULE("qxk_sema")

/*..........................................................................*/
void QXSemaphore_init(QXSemaphore * const me, uint_fast16_t count,
                      uint_fast16_t max_count)
{
    /*! @pre max_count must be greater than zero */
    Q_REQUIRE_ID(100, max_count > 0U);

    me->count     = (uint16_t)count;
    me->max_count = (uint16_t)max_count;
    QPSet_setEmpty(&me->waitSet);
}

/*..........................................................................*/
bool QXSemaphore_wait(QXSemaphore * const me, uint_fast16_t const nTicks) {
    QF_CRIT_STAT_
    QF_CRIT_E_();

    /* volatile into temp. */
    QXThread * const curr = QXK_PTR_CAST_(QXThread*, QXK_attr_.curr);

    /*! @pre this function must:
    * - NOT be called from an ISR;
    * - the semaphore must be initialized
    * - be called from an extended thread;
    * - the thread must NOT be already blocked on any object.
    */
    Q_REQUIRE_ID(200, (!QXK_ISR_CONTEXT_()) /* can't wait inside an ISR */
        && (me->max_count > 0U) /* sema must be initialized */
        && (curr != (QXThread *)0) /* curr must be extended */
        && (curr->super.super.temp.obj == (QMState *)0)); /* NOT blocked */
    /*! @pre also: the thread must NOT be holding a scheduler lock. */
    Q_REQUIRE_ID(201, QXK_attr_.lockHolder != curr->super.prio);

    bool signaled = true; /* assume that the semaphore will be signaled */
    if (me->count > 0U) {
        --me->count; /* semaphore taken: decrement the count */
    }
    else {
        uint_fast8_t const p = (uint_fast8_t)curr->super.dynPrio;

        /* remember the blocking object (this semaphore) */
        curr->super.super.temp.obj = QXK_PTR_CAST_(QMState*, me);
        QXThread_teArm_(curr, (QSignal)QXK_SEMA_SIG, nTicks);

        /* remove this curr prio from the ready set (will block)
        * and insert to the waiting set on this semaphore */
        QPSet_insert(&me->waitSet,        p); /* add to waiting-set */
        QPSet_remove(&QXK_attr_.readySet, p); /* remove from ready-set */

        /* schedule the next thread if multitasking started */
        (void)QXK_sched_();
        QF_CRIT_X_();
        QF_CRIT_EXIT_NOP(); /* BLOCK here !!! */

        QF_CRIT_E_();   /* AFTER unblocking... */
        /* the blocking object must be this semaphore */
        Q_ASSERT_ID(240, curr->super.super.temp.obj
                         == QXK_PTR_CAST_(QMState*, me));

        /* did the blocking time-out? (signal of zero means that it did) */
        if (curr->timeEvt.super.sig == 0U) {
            if (QPSet_hasElement(&me->waitSet, p)) { /* still waiting? */
                QPSet_remove(&me->waitSet, p); /* remove unblocked thread */
                signaled = false; /* the semaphore was NOT signaled */
                /* semaphore NOT taken: do NOT decrement the count */
            }
            else { /* semaphore was both signaled and timed out */
                --me->count; /* semaphore signaled: decrement the count */
            }
        }
        else { /* blocking did NOT time out */
            /* the thread must NOT be waiting on this semaphore */
            Q_ASSERT_ID(250,!QPSet_hasElement(&me->waitSet, p));

            --me->count; /* semaphore signaled: decrement the count */
        }
        curr->super.super.temp.obj = (QMState *)0; /* clear blocking obj. */
    }
    QF_CRIT_X_();

    return signaled;
}

/*..........................................................................*/
bool QXSemaphore_tryWait(QXSemaphore * const me) {
    /*! @pre the semaphore must be initialized */
    Q_REQUIRE_ID(300, me->max_count > 0U);

    QF_CRIT_STAT_
    QF_CRIT_E_();

    bool isAvailable;
    /* is the semaphore available? */
    if (me->count > 0U) {
        --me->count;
        isAvailable = true;
    }
    else { /* the semaphore is NOT available (would block) */
        isAvailable = false;
    }
    QF_CRIT_X_();

    return isAvailable;
}

/*..........................................................................*/
bool QXSemaphore_signal(QXSemaphore * const me) {
    /*! @pre the semaphore must be initialized */
    Q_REQUIRE_ID(400, me->max_count > 0U);

    QF_CRIT_STAT_
    QF_CRIT_E_();

    bool signaled = true; /* assume that the semaphore will be signaled */
    if (me->count < me->max_count) {

        ++me->count; /* increment the semaphore count */

        if (QPSet_notEmpty(&me->waitSet)) {
            /* find the highest-priority thread waiting on this semaphore */
            uint_fast8_t const p = QPSet_findMax(&me->waitSet);
            QXThread * const thr = QXK_PTR_CAST_(QXThread*, QF_active_[p]);

            /* assert that the tread:
            * - must be registered in QF;
            * - must be extended; and
            * - must be blocked on this semaphore;
            */
            Q_ASSERT_ID(410, (thr != (QXThread *)0)
                && (thr->super.osObject != (struct QActive *)0)
                && (thr->super.super.temp.obj
                    == QXK_PTR_CAST_(QMState*, me)));

            /* disarm the internal time event */
            (void)QXThread_teDisarm_(thr);

            /* make the thread ready to run and remove from the wait-list */
            QPSet_insert(&QXK_attr_.readySet, p);
            QPSet_remove(&me->waitSet, p);

            if (!QXK_ISR_CONTEXT_()) { /* not inside ISR? */
                (void)QXK_sched_(); /* schedule the next thread */
            }
        }
    }
    else {
        signaled = false; /* semaphore NOT signaled */
    }
    QF_CRIT_X_();

    return signaled;
}
