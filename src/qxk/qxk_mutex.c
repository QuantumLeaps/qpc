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
* @brief QXMutex_init(), QXMutex_lock(), QXMutex_tryLock() and
* QXMutex_unlock() definitions.
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qxk_pkg.h"      /* QXK package-scope interface */
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

Q_DEFINE_THIS_MODULE("qxk_mutex")

/*..........................................................................*/
void QXMutex_init(QXMutex * const me, uint_fast8_t ceiling) {
    QF_CRIT_STAT_
    QF_CRIT_E_();

    /*! @pre the ceiling priority of the mutex must:
    * - cannot exceed the maximum #QF_MAX_ACTIVE;
    * - the ceiling priority of the mutex must not be already in use;
    * (QF requires priority to be **unique**).
    */
    Q_REQUIRE_ID(100,
        (ceiling <= QF_MAX_ACTIVE)
        && ((ceiling == 0U)
            || (QF_active_[ceiling] == (QActive *)0)));

    me->ceiling    = (uint8_t)ceiling;
    me->lockNest   = 0U;
    me->holderPrio = 0U;
    QF_bzero((void *)&me->waitSet, sizeof(me->waitSet));

    if (ceiling != 0U) {
        /* reserve the ceiling priority level for this mutex */
        QF_active_[ceiling] = QXK_PTR_CAST_(QActive*, me);
    }
    QF_CRIT_X_();
}

/*..........................................................................*/
bool QXMutex_lock(QXMutex * const me,
                  uint_fast16_t const nTicks)
{
    QF_CRIT_STAT_
    QF_CRIT_E_();

    QXThread * const curr = QXK_PTR_CAST_(QXThread*, QXK_attr_.curr);

    /*! @pre this function must:
    * - NOT be called from an ISR;
    * - be called from an extended thread;
    * - the ceiling priority must not be used; or if used
    *   - the thread priority must be below the ceiling of the mutex;
    * - the ceiling must be in range
    * - the thread must NOT be already blocked on any object.
    */
    Q_REQUIRE_ID(200, (!QXK_ISR_CONTEXT_()) /* don't call from an ISR! */
        && (curr != (QXThread *)0) /* current thread must be extended */
        && ((me->ceiling == 0U)
            || (curr->super.prio < me->ceiling)) /* below ceiling */
        && (me->ceiling <= QF_MAX_ACTIVE) /* in range */
        && (curr->super.super.temp.obj == (QMState *)0)); /* not blocked */
    /*! @pre also: the thread must NOT be holding a scheduler lock. */
    Q_REQUIRE_ID(201, QXK_attr_.lockHolder != curr->super.prio);

    /* is the mutex available? */
    bool locked = true; /* assume that the mutex will be locked */
    if (me->lockNest == 0U) {
        me->lockNest = 1U;

        if (me->ceiling != 0U) {
            /* the priority slot must be occupied by this mutex */
            Q_ASSERT_ID(210, QF_active_[me->ceiling]
                             == QXK_PTR_CAST_(QActive*, me));

            /* boost the dynamic priority of this thread to the ceiling */
            QPSet_remove(&QXK_attr_.readySet,
                         (uint_fast8_t)curr->super.dynPrio);
            curr->super.dynPrio = me->ceiling;
            QPSet_insert(&QXK_attr_.readySet,
                         (uint_fast8_t)curr->super.dynPrio);
            QF_active_[me->ceiling] = &curr->super;
        }

        /* make the curr thread the new mutex holder */
        me->holderPrio = (uint8_t)curr->super.prio;

        QS_BEGIN_NOCRIT_PRE_(QS_MUTEX_LOCK, curr->super.prio)
            QS_TIME_PRE_();  /* timestamp */
            QS_2U8_PRE_(curr->super.prio, /* the start prio */
                        me->ceiling); /* current ceiling */
        QS_END_NOCRIT_PRE_()
    }
    /* is the mutex locked by this thread already (nested locking)? */
    else if (me->holderPrio == curr->super.prio) {

        /* the nesting level must not exceed the dynamic range of uint8_t */
        Q_ASSERT_ID(220, me->lockNest < 0xFFU);

        ++me->lockNest; /* lock one level */
    }
    else { /* the mutex is alredy locked by a different thread */
        /* the ceiling holder priority must be valid */
        Q_ASSERT_ID(230, 0U < me->holderPrio);
        Q_ASSERT_ID(231, me->holderPrio <= QF_MAX_ACTIVE);

        if (me->ceiling != 0U) {
            /* the prio slot must be occupied by the thr. holding the mutex */
            Q_ASSERT_ID(240, QF_active_[me->ceiling]
                             == QF_active_[me->holderPrio]);
        }

        /* remove the curr dynamic prio from the ready set (block)
        * and insert it to the waiting set on this mutex
        */
        uint_fast8_t const p = (uint_fast8_t)curr->super.dynPrio;
        QPSet_remove(&QXK_attr_.readySet, p);
        QPSet_insert(&me->waitSet,        p);

        /* store the blocking object (this mutex) */
        curr->super.super.temp.obj = QXK_PTR_CAST_(QMState*, me);
        QXThread_teArm_(curr, (QSignal)QXK_SEMA_SIG, nTicks);

        /* schedule the next thread if multitasking started */
        (void)QXK_sched_();
        QF_CRIT_X_();
        QF_CRIT_EXIT_NOP(); /* BLOCK here !!! */

        QF_CRIT_E_();   /* AFTER unblocking... */
        /* the blocking object must be this mutex */
        Q_ASSERT_ID(240, curr->super.super.temp.obj
                         == QXK_PTR_CAST_(QMState*, me));

        /* did the blocking time-out? (signal of zero means that it did) */
        if (curr->timeEvt.super.sig == 0U) {
            if (QPSet_hasElement(&me->waitSet, p)) { /* still waiting? */
                QPSet_remove(&me->waitSet, p); /* remove unblocked thread */
                locked = false; /* the mutex was NOT locked */
            }
        }
        else { /* blocking did NOT time out */
            /* the thread must NOT be waiting on this mutex */
            Q_ASSERT_ID(250, !QPSet_hasElement(&me->waitSet, p));
        }
        curr->super.super.temp.obj = (QMState *)0; /* clear blocking obj. */
    }
    QF_CRIT_X_();

    return locked;
}

/*..........................................................................*/
bool QXMutex_tryLock(QXMutex * const me) {
    QF_CRIT_STAT_
    QF_CRIT_E_();

    QActive *curr = QXK_attr_.curr;
    if (curr == (QActive *)0) { /* called from a basic thread? */
        curr = QF_active_[QXK_attr_.actPrio];
    }

    /*! @pre this function must:
    * - NOT be called from an ISR;
    * - the calling thread must be valid;
    * - the ceiling must be not used; or
    *   - the thread priority must be below the ceiling of the mutex;
    * - the ceiling must be in range
    */
    Q_REQUIRE_ID(300, (!QXK_ISR_CONTEXT_()) /* don't call from an ISR! */
        && (curr != (QActive *)0) /* current thread must be valid */
        && ((me->ceiling == 0U) /* ceiling not used */
            || (curr->prio < me->ceiling)) /* below ceiling */
        && (me->ceiling <= QF_MAX_ACTIVE));
    /*! @pre also: the thread must NOT be holding a scheduler lock. */
    Q_REQUIRE_ID(301, QXK_attr_.lockHolder != curr->prio);

    /* is the mutex available? */
    if (me->lockNest == 0U) {
        me->lockNest = 1U;

        if (me->ceiling != 0U) {
            /* the priority slot must be set to this mutex */
            Q_ASSERT_ID(310, QF_active_[me->ceiling]
                             == QXK_PTR_CAST_(QActive*, me));

            /* boost the dynamic priority of this thread to the ceiling */
            QPSet_remove(&QXK_attr_.readySet, (uint_fast8_t)curr->dynPrio);
            curr->dynPrio = me->ceiling;
            QPSet_insert(&QXK_attr_.readySet, (uint_fast8_t)curr->dynPrio);
            QF_active_[me->ceiling] = curr;
        }

        /* make curr thread the new mutex holder */
        me->holderPrio = curr->prio;

        QS_BEGIN_NOCRIT_PRE_(QS_MUTEX_LOCK, curr->prio)
            QS_TIME_PRE_();  /* timestamp */
            QS_2U8_PRE_(curr->prio, me->ceiling);
        QS_END_NOCRIT_PRE_()
    }
    /* is the mutex held by this thread already (nested locking)? */
    else if (me->holderPrio == curr->prio) {
        /* the nesting level must not exceed  the dynamic range of uint8_t */
        Q_ASSERT_ID(320, me->lockNest < 0xFFU);

        ++me->lockNest; /* lock one level */
    }
    else { /* the mutex is alredy locked by a different thread */
        if (me->ceiling != 0U) {
            /* the prio slot must be claimed by the mutex holder */
            Q_ASSERT_ID(330, QF_active_[me->ceiling]
                             == QF_active_[me->holderPrio]);
        }
        curr = (QActive *)0; /* means that mutex is NOT available */
    }
    QF_CRIT_X_();

    return curr != (QActive *)0;
}

/*..........................................................................*/
void QXMutex_unlock(QXMutex * const me) {
    QF_CRIT_STAT_
    QF_CRIT_E_();

    QActive *curr = QXK_attr_.curr;
    if (curr == (QActive *)0) { /* called from a basic thread? */
        curr = QF_active_[QXK_attr_.actPrio];
    }

    /*! @pre this function must:
    * - NOT be called from an ISR;
    * - the calling thread must be valid;
    * - the ceiling must not be used or
    *    - the current thread must have priority equal to the mutex ceiling.
    * - the ceiling must be in range
    */
    Q_REQUIRE_ID(400, (!QXK_ISR_CONTEXT_()) /* don't call from an ISR! */
        && (curr != (QActive *)0) /* current thread must be valid */
        && ((me->ceiling == 0U) /* ceiling not used OR... */
            || (curr->dynPrio == me->ceiling)) /* curr at ceiling prio */
        && (me->ceiling <= QF_MAX_ACTIVE));
    /*! @pre also: the mutex must be already locked at least once. */
    Q_REQUIRE_ID(401, me->lockNest > 0U);
    /*! @pre also: the mutex must be held by this thread. */
    Q_REQUIRE_ID(402, me->holderPrio == curr->prio);

    /* is this the last nesting level? */
    if (me->lockNest == 1U) {

        if (me->ceiling != 0U) {
            /* restore the holding thread's dynamic prio to the original */
            QPSet_remove(&QXK_attr_.readySet, (uint_fast8_t)curr->dynPrio);
            curr->dynPrio = curr->prio;
            QPSet_insert(&QXK_attr_.readySet, (uint_fast8_t)curr->dynPrio);

            /* put the mutex at the priority ceiling slot */
            QF_active_[me->ceiling] = QXK_PTR_CAST_(QActive*, me);
        }

        /* the mutex no longer held by a thread */
        me->holderPrio = 0U;

        QS_BEGIN_NOCRIT_PRE_(QS_MUTEX_UNLOCK, curr->prio)
            QS_TIME_PRE_();              /* timestamp */
            /* start prio & the mutex ceiling */
            QS_2U8_PRE_(curr->prio, me->ceiling);
        QS_END_NOCRIT_PRE_()

        /* are any other threads waiting for this mutex? */
        if (QPSet_notEmpty(&me->waitSet)) {
            /* find the highest-priority thread waiting on this mutex */
            uint_fast8_t const p = QPSet_findMax(&me->waitSet);
            QXThread * const thr = QXK_PTR_CAST_(QXThread*, QF_active_[p]);

            /* the waiting thread must:
            * - the ceiling must not be used; or if used
            *   - the thread must have priority below the ceiling
            * - be registered in QF
            * - have the dynamic priority the same as initial priority
            * - be blocked on this mutex
            */
            Q_ASSERT_ID(410,
                ((me->ceiling == 0U) || (p < (uint_fast8_t)me->ceiling))
                && (thr != (QXThread *)0)
                && (thr->super.dynPrio == thr->super.prio)
                && (thr->super.super.temp.obj
                    == QXK_PTR_CAST_(QMState*, me)));

            /* disarm the internal time event */
            (void)QXThread_teDisarm_(thr);

            if (me->ceiling != 0U) {
                /* boost the dynamic priority of this thread to the ceiling */
                thr->super.dynPrio = me->ceiling;
                QF_active_[me->ceiling] = &thr->super;
            }

            /* make the thread the new mutex holder */
            me->holderPrio = (uint8_t)thr->super.prio;

            /* make the thread ready to run (at the ceiling prio)
            * and remove from the waiting list */
            QPSet_insert(&QXK_attr_.readySet,
                         (uint_fast8_t)thr->super.dynPrio);
            QPSet_remove(&me->waitSet, p);

            QS_BEGIN_NOCRIT_PRE_(QS_MUTEX_LOCK, thr->super.prio)
                QS_TIME_PRE_();  /* timestamp */
                /* start priority & ceiling priority */
                QS_2U8_PRE_(thr->super.prio, me->ceiling);
            QS_END_NOCRIT_PRE_()
        }
        else { /* no threads are waiting for this mutex */
            me->lockNest = 0U;

            if (me->ceiling != 0U) {
                /* put the mutex at the priority ceiling slot */
                QF_active_[me->ceiling] = QXK_PTR_CAST_(QActive*, me);
            }
        }

        /* schedule the next thread if multitasking started */
        if (QXK_sched_() != 0U) {
            QXK_activate_(); /* activate a basic thread */
        }
    }
    else { /* releasing the mutex */
        --me->lockNest; /* unlock one level */
    }
    QF_CRIT_X_();
}
