/**
* @file
* @ingroup qk
* @brief QMutex_init(), QMutex_lock and QMutex_unlock() definitions.
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
#include "qk_pkg.h"       /* QK package-scope interface */
#include "qassert.h"      /* QP embedded systems-friendly assertions */
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

/* protection against including this source file in a wrong project */
#ifndef qk_h
    #error "Source file included in a project NOT based on the QK kernel"
#endif /* qk_h */

Q_DEFINE_THIS_MODULE("qk_mutex")

/****************************************************************************/
/**
* @description
* Initialize the QK priority ceiling mutex.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     prioCeiling ceiling priotity of the mutex
*
* @note The ceiling priority must be unused by any AO. The ceiling
* priority must be higher than priority of any AO that uses the
* protected resource.
*
* @usage
* @include qk_mux.c
*/
void QMutex_init(QMutex * const me, uint_fast8_t prioCeiling) {
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

    /* reserve the ceiling priority level for this mutex */
    QF_active_[prioCeiling] = (QMActive *)me;

    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* Lock the QK priority ceiling mutex.
*
* @param[in,out] me      pointer (see @ref oop)
*
* @note This function should be always paired with QXK_mutexUnlock(). The
* code between QMutex_lock() and QMutex_unlock() should be kept to the
* minimum.
*
* @usage
* @include qk_mux.c
*/
void QMutex_lock(QMutex * const me) {
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    /* is the scheduler unloacked? */
    if (QK_currPrio_ <= (uint_fast8_t)QF_MAX_ACTIVE) {
        QMActive *act = QF_active_[QK_currPrio_];

        /** @pre QMutex_lock() must not be called from ISR level,
        * the thread priority must not exceed the mutex priority ceiling
        * and the mutex must be initialized.
        */
        Q_REQUIRE_ID(200, (!QK_ISR_CONTEXT_()) /* don't call from an ISR! */
                          && (act->thread <= (uint_fast8_t)me->prioCeiling)
                          && (QF_active_[me->prioCeiling] != (QMActive *)0));

        /* is the mutex available? */
        if (me->lockNest == (uint8_t)0) {
            me->lockNest = (uint8_t)1;

            /* the priority slot must be set to this mutex */
            Q_ASSERT_ID(210, QF_active_[me->prioCeiling] == (QMActive *)me);

            /* switch the priority of this thread to the ceiling priority */
            QF_active_[me->prioCeiling] = act;
            act->prio = (uint_fast8_t)me->prioCeiling; /* set to ceiling */

            if (QK_prioIsSet(&QK_readySet_, act->thread)) {
                QK_prioRemove(&QK_readySet_, act->thread);
                QK_prioInsert(&QK_readySet_, act->prio);
            }
            QK_currPrio_ = act->prio;

            QS_BEGIN_NOCRIT_(QS_QK_MUTEX_LOCK, (void *)0, (void *)0)
                QS_TIME_();                   /* timestamp */
                QS_2U8_((uint8_t)act->thread, /* the start priority */
                        (uint8_t)act->prio);  /* the current ceiling */
            QS_END_NOCRIT_()
        }
        /* is the mutex locked by this thread already (nested locking)? */
        else if (QF_active_[me->prioCeiling] == act) {
            ++me->lockNest;
        }
        /* the mutex can't be locked by a different AO -- error */
        else {
            Q_ERROR_ID(220);
        }
    }
    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* Unlock the QK priority ceiling mutex.
*
* @param[in,out] me      pointer (see @ref oop)
*
* @note This function should be always paired with QK_mutexLock(). The
* code between QMutex_lock() and QMutex_unlock() should be kept to the
* minimum.
*
* @usage
* @include qk_mux.c
*/
void QMutex_unlock(QMutex * const me) {
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    /* is the scheduler unloacked? */
    if (QK_currPrio_ <= (uint_fast8_t)QF_MAX_ACTIVE) {
        QMActive *act = QF_active_[QK_currPrio_];

        /** @pre QMutex_unlock() must not be called from ISR level
        * and the mutex must be owned by this thread.
        */
        Q_REQUIRE_ID(300, (!QK_ISR_CONTEXT_()) /* don't call from an ISR! */
                          && (me->lockNest > (uint8_t)0)
                          && (QF_active_[me->prioCeiling] == act));

        /* Unlocking the first nesting level? */
        if (me->lockNest == (uint8_t)1) {
            uint_fast8_t p;

            me->lockNest = (uint8_t)0;

            /* reclaim the ceiling priority for this mutex */
            QF_active_[me->prioCeiling] = (QMActive *)me;

            /* restore the start priority for this AO... */
            act->prio = act->thread;
            QK_currPrio_ = act->thread;
            if (QK_prioIsSet(&QK_readySet_, me->prioCeiling)) {
                QK_prioRemove(&QK_readySet_, me->prioCeiling);
                QK_prioInsert(&QK_readySet_, act->thread);
            }

            QS_BEGIN_NOCRIT_(QS_QK_MUTEX_UNLOCK, (void *)0, (void *)0)
                QS_TIME_();                        /* timestamp */
                QS_2U8_((uint8_t)act->thread,      /* the start priority */
                        (uint8_t)me->prioCeiling); /* the current ceiling */
            QS_END_NOCRIT_()

            /* find if some other AO has higher priority than the current */
            p = QK_schedPrio_();
            if (p != (uint_fast8_t)0) {
                QK_sched_(p);
            }
        }
        /* this AO is releasing a nested mutex lock */
        else {
            --me->lockNest;
        }
    }
    QF_CRIT_EXIT_();
}
