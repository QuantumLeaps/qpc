/**
* @file
* @ingroup qkx
* @brief QXMutex_init(), QXMutex_lock(), and QXMutex_unlock() definitions.
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
* @param[in]     prioCeiling ceiling priotity of the mutex
*
* @note The ceiling priority must be unused by any thread. The ceiling
* priority must be higher than priority of any thread that uses the
* protected resource.
*
* @usage
* @include qxk_mux.c
*/
void QXMutex_init(QXMutex * const me, uint_fast8_t prioCeiling) {
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    /** @pre the celiling priority of the mutex must not be zero and cannot
    * exceed the maximum #QF_MAX_ACTIVE. Also, the ceiling priority of the
    * mutex must not be already in use. QF requires priority to be __unique__.
    */
    Q_REQUIRE_ID(100, ((uint_fast8_t)0 < prioCeiling)
                       && (prioCeiling <= (uint_fast8_t)QF_MAX_ACTIVE)
              && (QF_active_[prioCeiling] == (QMActive *)0));

    me->prioCeiling = (uint8_t)prioCeiling;
    me->lockNest = (uint8_t)0;
    QF_bzero(&me->waitSet, (uint_fast16_t)sizeof(me->waitSet));

    /* reserve the ceiling priority level for this mutex */
    QF_active_[prioCeiling] = (QMActive *)me;

    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* Lock the QXK priority ceiling mutex.
*
* @param[in,out] me      pointer (see @ref oop)
*
* @note This function should be always paired with QXK_mutexUnlock(). The
* code between QXK_mutexLock() and QXK_mutexUnlock() should be kept to the
* minimum.
*
* @usage
* @include qxk_mux.c
*/
void QXMutex_lock(QXMutex * const me) {
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    if (QXK_attr_.curr != (void *)0) { /* Is QXK running? */
        QMActive *act = (QMActive *)QXK_attr_.curr;

        /** @pre QXMutex_lock() must not be called from ISR level,
        * the thread priority must not exceed the mutex priority ceiling
        * and the mutex must be initialized.
        */
        Q_REQUIRE_ID(200, (!QXK_ISR_CONTEXT_()) /* don't call from an ISR! */
            && (act->thread.startPrio <= (uint_fast8_t)me->prioCeiling)
            && (QF_active_[me->prioCeiling] != (QMActive *)0));

        /* is the mutex available? */
        if (me->lockNest == (uint8_t)0) {
            me->lockNest = (uint8_t)1;

            /* the priority slot must be set to this mutex */
            Q_ASSERT_ID(210, QF_active_[me->prioCeiling] == (QMActive *)me);

            /* switch the priority of this thread to the ceiling priority */
            QF_active_[me->prioCeiling] = act;
            act->prio = (uint_fast8_t)me->prioCeiling; /*set to the ceiling */

            QXK_prioRemove(&QXK_attr_.readySet, act->thread.startPrio);
            QXK_prioInsert(&QXK_attr_.readySet, act->prio);

            QS_BEGIN_NOCRIT_(QS_QK_MUTEX_LOCK, (void *)0, (void *)0)
                QS_TIME_();                    /* timestamp */
                QS_2U8_((uint8_t)act->thread.startPrio, /* the start prio */
                        (uint8_t)act->prio);   /* the current ceiling */
            QS_END_NOCRIT_()
        }
        /* is the mutex locked by this thread already (nested locking)? */
        else if (QF_active_[me->prioCeiling] == act) {
            ++me->lockNest;
        }
        /* the mutex is locked by a different thread -- block */
        else {
            /* store the blocking object (this mutex) */
            act->super.temp.obj = (QMState const *)me;

            QXK_prioInsert(&me->waitSet,        act->prio);
            QXK_prioRemove(&QXK_attr_.readySet, act->prio);

            if (QXK_attr_.curr != (void *)0) { /* multitasking started? */
                QXK_sched_();
            }
        }
    }
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* Unlock the QXK priority ceiling mutex.
*
* @param[in,out] me      pointer (see @ref oop)
*
* @note This function should be always paired with QXK_mutexLock(). The
* code between QXK_mutexLock() and QXK_mutexUnlock() should be kept to the
* minimum.
*
* @usage
* @include qxk_mux.c
*/
void QXMutex_unlock(QXMutex * const me) {
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    if (QXK_attr_.curr != (void *)0) { /* Is QXK running? */
        QMActive *act = (QMActive *)QXK_attr_.curr;

        /** @pre QXMutex_unlock() must not be called from ISR level
        * and the mutex must be owned by this thread.
        */
        Q_REQUIRE_ID(300, (!QXK_ISR_CONTEXT_()) /* don't call from an ISR! */
                          && (me->lockNest > (uint8_t)0)
                          && (QF_active_[me->prioCeiling] == act));

        /* Unlocking the first nesting level? */
        if (me->lockNest == (uint8_t)1) {
            me->lockNest = (uint8_t)0;

            /* free up the ceiling priority and put the mutex at this slot */
            QXK_prioRemove(&QXK_attr_.readySet, me->prioCeiling);
            QF_active_[me->prioCeiling] = (QMActive *)me;

            /* restore the thread priority to the original */
            QXK_prioInsert(&QXK_attr_.readySet, act->thread.startPrio);
            act->prio = act->thread.startPrio;

            QS_BEGIN_NOCRIT_(QS_QK_MUTEX_UNLOCK, (void *)0, (void *)0)
                QS_TIME_();                        /* timestamp */
                QS_2U8_((uint8_t)act->prio,        /* the original priority */
                        (uint8_t)me->prioCeiling); /* the current ceiling */
            QS_END_NOCRIT_()

            /* are any threads waiting for the mutex? */
            if (QXK_prioNotEmpty(&me->waitSet)) {
                uint_fast8_t p;

                /* find the highest-priority waiting thread */
                QXK_prioFindMax(&me->waitSet, p);

                Q_ASSERT_ID(310, p <= (uint_fast8_t)me->prioCeiling);

                act = QF_active_[p];

                /* this thread is no longer waiting for the mutex */
                QXK_prioRemove(&me->waitSet, p);

                /* switch the priority of this thread to the ceiling prio */
                QF_active_[me->prioCeiling] = act;
                act->prio = (uint_fast8_t)me->prioCeiling; /* raise prio */

                QXK_prioRemove(&QXK_attr_.readySet, p);
                QXK_prioInsert(&QXK_attr_.readySet, act->prio);

                QS_BEGIN_NOCRIT_(QS_QK_MUTEX_LOCK, (void *)0, (void *)0)
                    QS_TIME_();                  /* timestamp */
                    QS_2U8_((uint8_t)p,          /* the original priority */
                            (uint8_t)me->prioCeiling); /* current ceiling */
                QS_END_NOCRIT_()
            }

            if (QXK_attr_.curr != (void *)0) { /* multitasking started? */
                QXK_sched_();
            }
        }
        /* this thread is releasing a nested lock */
        else {
            --me->lockNest;
        }
    }
    QF_CRIT_EXIT_();
}
