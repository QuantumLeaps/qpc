/**
* @file
* @ingroup qxk
* @brief QXMutex_init(), QXMutex_lock(), QXMutex_tryLock() and
* QXMutex_unlock() definitions.
* @cond
******************************************************************************
* Last updated for version 6.2.0
* Last updated on  2018-03-16
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2018 Quantum Leaps, LLC. All rights reserved.
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
* https://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qxk_pkg.h"      /* QXK package-scope interface */
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

Q_DEFINE_THIS_MODULE("qxk_mutex")

/****************************************************************************/
/**
* @description
* Initialize the QXK priority ceiling mutex.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     ceiling the ceiling-priotity of this mutex or zero
*
* @note
* `ceiling == 0` means that the priority-ceiling protocol shall __not__
* be used by this mutex. Such mutex will __not__ change (boost) the
* priority of the holding thread.
*
* @note
* `ceiling > 0` means that the priority-ceiling protocol shall be used
* by this mutex. Such mutex __will__ boost the priority of the holding
* thread to the `ceiling` level for as long as the thread holds this mutex.
*
* @attention
* When the priority-ceiling protocol is used (`ceiling > 0`), the
* `ceiling` priority must be unused by any other thread or mutex.
* Also, the `ceiling` priority must be higher than priority of any thread
* that uses this mutex.
*
* @usage
* @include qxk_mux.c
*/
void QXMutex_init(QXMutex * const me, uint_fast8_t ceiling) {
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    /** @pre the celiling priority of the mutex must:
    * - cannot exceed the maximum #QF_MAX_ACTIVE;
    * - the ceiling priority of the mutex must not be already in use;
    * (QF requires priority to be **unique**).
    */
    Q_REQUIRE_ID(100,
        (ceiling <= (uint_fast8_t)QF_MAX_ACTIVE)
        && ((ceiling == (uint_fast8_t)0)
            || (QF_active_[ceiling] == (QActive *)0)));

    me->ceiling    = (uint8_t)ceiling;
    me->lockNest   = (uint8_t)0;
    me->holderPrio = (uint8_t)0;
    QF_bzero(&me->waitSet, (uint_fast16_t)sizeof(me->waitSet));

    if (ceiling != (uint_fast8_t)0) {
        /* reserve the ceiling priority level for this mutex */
        QF_active_[ceiling] = (QActive *)me;
    }
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* Lock the QXK priority ceiling mutex ::QXMutex.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]  nTicks    number of clock ticks (at the associated rate)
*                       to wait for the semaphore. The value of
*                       QXTHREAD_NO_TIMEOUT indicates that no timeout will
*                       occur and the semaphore will wait indefinitely.
* @returns
* 'true' if the mutex has been acquired and 'false' if a timeout occured.
*
* @note
* The mutex locks are allowed to nest, meaning that the same extended thread
* can lock the same mutex multiple times (< 255). However, each call to
* QXMutex_lock() must be ballanced by the matching call to QXMutex_unlock().
*
* @usage
* @include qxk_mux.c
*/
bool QXMutex_lock(QXMutex * const me,
                  uint_fast16_t const nTicks)
{
    QXThread *curr;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    curr = (QXThread *)QXK_attr_.curr;

    /** @pre this function must:
    * - NOT be called from an ISR;
    * - be called from an extended thread;
    * - the ceiling priority must not be used; or if used
    *   - the thread priority must be below the ceiling of the mutex;
    * - the thread must NOT be holding a scheduler lock;
    * - the thread must NOT be already blocked on any object.
    */
    Q_REQUIRE_ID(200, (!QXK_ISR_CONTEXT_()) /* don't call from an ISR! */
        && (curr != (QXThread *)0) /* current thread must be extended */
        && ((me->ceiling == (uint8_t)0) /* below ceiling */
            || (curr->super.startPrio < me->ceiling))
        && (QXK_attr_.lockHolder != curr->super.prio) /* not holding a lock */
        && (curr->super.super.temp.obj == (QMState *)0)); /* not blocked */

    /* is the mutex available? */
    if (me->lockNest == (uint8_t)0) {
        me->lockNest = (uint8_t)1;

        if (me->ceiling != (uint8_t)0) {
            /* the priority slot must be set to this mutex */
            Q_ASSERT_ID(210, QF_active_[me->ceiling] == (QActive *)me);

            /* switch the priority of this thread to the mutex ceiling */
            curr->super.prio = me->ceiling;
            QF_active_[me->ceiling] = &curr->super;

            QPSet_remove(&QXK_attr_.readySet,
                         (uint_fast8_t)curr->super.startPrio);
            QPSet_insert(&QXK_attr_.readySet,
                         (uint_fast8_t)curr->super.prio);
        }
        me->holderPrio = (uint8_t)curr->super.startPrio;

        QS_BEGIN_NOCRIT_(QS_MUTEX_LOCK, (void *)0, (void *)0)
            QS_TIME_();  /* timestamp */
            QS_2U8_(curr->super.startPrio, /* the start prio */
                    me->ceiling); /* current ceiling */
        QS_END_NOCRIT_()
    }
    /* is the mutex locked by this thread already (nested locking)? */
    else if (me->holderPrio == curr->super.startPrio) {

        /* the nesting level must not exceed 0xFF */
        Q_ASSERT_ID(220, me->lockNest < (uint8_t)0xFF);

        ++me->lockNest;
    }
    else { /* the mutex is alredy locked by a different thread */

        /* the ceiling holder priority must be valid */
        Q_ASSERT_ID(230, me->holderPrio != (uint8_t)0);

        if (me->ceiling != (uint8_t)0) {
            /* the prio slot must be claimed by the thr. holding the mutex */
            Q_ASSERT_ID(240, QF_active_[me->ceiling] != (QActive *)0);
        }
        /* remove this curr prio from the ready set (block)
        * and insert to the waiting set on this mutex */
        QPSet_remove(&QXK_attr_.readySet, (uint_fast8_t)curr->super.prio);
        QPSet_insert(&me->waitSet,        (uint_fast8_t)curr->super.prio);

        /* store the blocking object (this mutex) */
        curr->super.super.temp.obj = (QMState *)me;
        QXThread_teArm_(curr, (QSignal)QXK_SEMA_SIG, nTicks);

        /* schedule the next thread if multitasking started */
        (void)QXK_sched_();
        QF_CRIT_EXIT_();
        QF_CRIT_EXIT_NOP(); /* BLOCK here */

        QF_CRIT_ENTRY_();
        /* the blocking object of the current thread must be this mutex */
        Q_ASSERT_ID(240, curr->super.super.temp.obj == (QMState *)me);

        curr->super.super.temp.obj = (QMState *)0; /* clear blocking obj. */
    }
    QF_CRIT_EXIT_();

    /* signal of non-zero means that the time event has not expired */
    return (bool)(curr->timeEvt.super.sig != (QSignal)0);
}

/****************************************************************************/
/**
* @description
* Try to lock the QXK priority ceiling mutex ::QXMutex.
*
* @param[in,out] me      pointer (see @ref oop)
*
* @returns
* 'true' if the mutex was successfully locked and 'false' if the mutex was
* unavailable and was NOT locked.
*
* @note
* This function **can** be called from both basic threads (active objects)
* and extended threads.
*
* @note
* The mutex locks are allowed to nest, meaning that the same extended thread
* can lock the same mutex multiple times (<= 225). However, each successful
* call to QXMutex_tryLock() must be ballanced by the matching call to
* QXMutex_unlock().
*/
bool QXMutex_tryLock(QXMutex * const me) {
    QActive *curr;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    curr = QXK_attr_.curr;
    if (curr == (QActive *)0) { /* called from a basic thread? */
        curr = QF_active_[QXK_attr_.actPrio];
    }

    /** @pre this function must:
    * - NOT be called from an ISR;
    * - the QXK kernel must be running;
    * - the calling thread must be valid;
    * - the ceiling must be not used; or
    *   - the thread priority must be below the ceiling of the mutex;
    * - the thread must NOT be holding a scheduler lock;
    */
    Q_REQUIRE_ID(300, (!QXK_ISR_CONTEXT_()) /* don't call from an ISR! */
        && (QXK_attr_.lockPrio <= (uint8_t)QF_MAX_ACTIVE)
        && (curr != (QActive *)0) /* current thread must be valid */
        && ((me->ceiling == (uint8_t)0) /* below ceiling */
            || (curr->startPrio < me->ceiling))
        && (curr->prio != QXK_attr_.lockHolder)); /* not holding a lock */

    /* is the mutex available? */
    if (me->lockNest == (uint8_t)0) {
        me->lockNest = (uint8_t)1;

        if (me->ceiling != (uint8_t)0) {
            /* the priority slot must be set to this mutex */
            Q_ASSERT_ID(310, QF_active_[me->ceiling] == (QActive *)me);

            /* switch the priority of this thread to the mutex ceiling */
            curr->prio = me->ceiling;
            QF_active_[me->ceiling] = curr;

            QPSet_remove(&QXK_attr_.readySet, (uint_fast8_t)curr->startPrio);
            QPSet_insert(&QXK_attr_.readySet, (uint_fast8_t)curr->prio);
        }

        /* make curr thread the new mutex holder */
        me->holderPrio = curr->startPrio;

        QS_BEGIN_NOCRIT_(QS_MUTEX_LOCK, (void *)0, curr)
            QS_TIME_();  /* timestamp */
            QS_2U8_(curr->startPrio, /* the start prio */
                    me->ceiling); /* the current ceiling */
        QS_END_NOCRIT_()
    }
    /* is the mutex held by this thread already (nested locking)? */
    else if (me->holderPrio == curr->startPrio) {
        /* the nesting level must not exceed 0xFF */
        Q_ASSERT_ID(320, me->lockNest < (uint8_t)0xFF);

        ++me->lockNest;
    }
    else { /* the mutex is alredy locked by a different thread */
        if (me->ceiling != (uint8_t)0) {
            /* the prio slot must be claimed by the mutex holder */
            Q_ASSERT_ID(330, (me->holderPrio != (uint8_t)0)
                && (QF_active_[me->ceiling] == QF_active_[me->holderPrio]));
        }
        curr = (QActive *)0; /* means that mutex is NOT available */
    }
    QF_CRIT_EXIT_();

    return (curr != (QActive *)0);
}

/****************************************************************************/
/**
* @description
* Unlock the QXK priority ceiling mutex.
*
* @param[in,out] me      pointer (see @ref oop)
*
* @note
* This function **can** be called from both basic threads (active objects)
* and extended threads.
*
* @note
* The mutex locks are allowed to nest, meaning that the same extended thread
* can lock the same mutex multiple times (<= 225). However, each call to
* QXMutex_lock() or a _successfull_ call to QXMutex_tryLock() must be
* ballanced by the matching call to QXMutex_unlock().
*
* @usage
* @include qxk_mux.c
*/
void QXMutex_unlock(QXMutex * const me) {
    QActive *curr;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    curr = QXK_attr_.curr;
    if (curr == (QActive *)0) { /* called from a basic thread? */
        curr = QF_active_[QXK_attr_.actPrio];
    }

    /** @pre this function must:
    * - NOT be called from an ISR;
    * - the calling thread must be valid;
    * - the mutex must be held by this thread;
    * - the ceiling must not be used or
    *    - the current thread must have priority equal to the mutex ceiling;
    * - the mutex must be already locked at least once.
    */
    Q_REQUIRE_ID(400, (!QXK_ISR_CONTEXT_()) /* don't call from an ISR! */
        && (curr != (QActive *)0) /* current thread must be valid */
        && (curr->startPrio == me->holderPrio)
        && ((me->ceiling == (uint8_t)0) /* curr at ceiling prio */
            || (curr->prio == me->ceiling))
        && (me->lockNest > (uint8_t)0)); /* locked at least once */

    /* is this the last nesting level? */
    if (me->lockNest == (uint8_t)1) {

        if (me->ceiling != (uint8_t)0) {
            /* restore the holding thread's priority to the original */
            curr->prio = curr->startPrio;

            /* remove the boosted priority and insert the original priority */
            QPSet_remove(&QXK_attr_.readySet, (uint_fast8_t)me->ceiling);
            QPSet_insert(&QXK_attr_.readySet, (uint_fast8_t)curr->startPrio);
        }

        /* the mutex no longer held by a thread */
        me->holderPrio = (uint8_t)0;

        QS_BEGIN_NOCRIT_(QS_MUTEX_UNLOCK, (void *)0, curr)
            QS_TIME_();  /* timestamp */
            QS_2U8_((uint8_t)curr->startPrio,  /* the start priority */
                    me->ceiling);   /* the mutex ceiling */
        QS_END_NOCRIT_()

        /* are any other threads waiting for this mutex? */
        if (QPSet_notEmpty(&me->waitSet)) {
            uint_fast8_t p;
            QXThread *thr;

            /* find the highest-priority waiting thread */
            QPSet_findMax(&me->waitSet, p);
            thr = (QXThread *)QF_active_[p];

            /* the waiting thread must:
            * (1) the ceiling must not be used; or if used
            *     the thread must have priority below the ceiling
            * (2) be extended
            * (3) not be redy to run
            * (4) have still the start priority
            * (5) be blocked on this mutex
            */
            Q_ASSERT_ID(410,
                ((me->ceiling == (uint8_t)0) /* below ceiling */
                   || (p < (uint_fast8_t)me->ceiling))
                && (thr != (QXThread *)0) /* extended thread */
                && (!QPSet_hasElement(&QXK_attr_.readySet, p))
                && (thr->super.prio == thr->super.startPrio)
                && (thr->super.super.temp.obj == (QMState *)me));

            /* disarm the internal time event */
            (void)QXThread_teDisarm_(thr);

            /* this thread is no longer waiting for the mutex */
            QPSet_remove(&me->waitSet, p);

            if (me->ceiling != (uint8_t)0) {
                /* switch the priority of this thread to the mutex ceiling */
                thr->super.prio = me->ceiling;
                QF_active_[me->ceiling] = &thr->super;
            }

            /* make thr the new mutex holder */
            me->holderPrio = (uint8_t)thr->super.startPrio;

            /* make the thread ready to run (at the ceiling prio) */
            QPSet_insert(&QXK_attr_.readySet, (uint_fast8_t)thr->super.prio);

            QS_BEGIN_NOCRIT_(QS_MUTEX_LOCK, (void *)0, thr)
                QS_TIME_();  /* timestamp */
                QS_2U8_(thr->super.startPrio, /* start priority */
                        me->ceiling);  /* ceiling priority */
            QS_END_NOCRIT_()
        }
        else { /* no threads are waiting for this mutex */
            me->lockNest = (uint8_t)0;

            if (me->ceiling != (uint8_t)0) {
                /* put the mutex at the priority ceiling slot */
                QF_active_[me->ceiling] = (QActive *)me;
            }
        }

        /* schedule the next thread if multitasking started */
        if (QXK_sched_() != (uint_fast8_t)0) {
            QXK_activate_(); /* activate a basic thread */
        }
    }
    else { /* releasing the mutex */
        --me->lockNest; /* release one level */
    }
    QF_CRIT_EXIT_();
}

