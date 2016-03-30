/**
* @file
* @ingroup qk
* @brief QMutex_init(), QMutex_lock and QMutex_unlock() definitions.
* @cond
******************************************************************************
* Last updated for version 5.6.2
* Last updated on  2016-03-29
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
* Initializes QK priority-ceiling mutex to the specified ceiling priority.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in] prio        ceiling priority of the mutex
*
* @note
* A mutex must be initialized before it can be locked or unlocked.
*
* @sa QMutex_lock(), QMutex_unlock()
*
* @usage
* The following example shows how ti initialize, lock and unlock QK mutex:
* @include qk_mux.c
*/
void QMutex_init(QMutex * const me, uint_fast8_t prio) {
    me->lockPrio = prio;
}

/****************************************************************************/
/**
* @description
* This function locks the QK mutex.
*
* @param[in,out] me      pointer (see @ref oop)
*
* @note
* A mutex must be initialized before it can be locked or unlocked.
*
* @note
* QMutex_lock() must be always followed by the corresponding
* QMutex_unlock().
*
* @sa QMutex_init(), QMutex_unlock()
*
* @usage
* The following example shows how ti initialize, lock and unlock QXK mutex:
* @include qk_mux.c
*/
void QMutex_lock(QMutex * const me) {
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    /** @pre scheduler cannot be locked from the ISR context */
    Q_REQUIRE_ID(700, !QK_ISR_CONTEXT_());

    me->prevPrio = QK_lockPrio_;   /* save the previous prio */
    if (QK_lockPrio_ < me->lockPrio) { /* raising the lock prio? */
        QK_lockPrio_ = me->lockPrio;
    }

    QS_BEGIN_NOCRIT_(QS_SCHED_LOCK, (void *)0, (void *)0)
        QS_TIME_(); /* timestamp */
        QS_2U8_((uint8_t)me->prevPrio,  /* the previouis lock prio */
                (uint8_t)QK_lockPrio_); /* the new lock prio */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* This function unlocks the QK mutex.
*
* @param[in] me      pointer (see @ref oop)
*
* @note
* A mutex must be initialized before it can be locked or unlocked.
*
* @note
* QMutex_unlock() must always follow the corresponding QMutex_lock().
*
* @sa QMutex_init(), QMutex_lock()
*
* @usage
* The following example shows how ti initialize, lock and unlock QXK mutex:
* @include qk_mux.c
*/
void QMutex_unlock(QMutex const * const me) {
    uint_fast8_t p;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    /** @pre scheduler cannot be unlocked from the ISR context */
    Q_REQUIRE_ID(800, !QK_ISR_CONTEXT_());

    QS_BEGIN_NOCRIT_(QS_SCHED_UNLOCK, (void *)0, (void *)0)
        QS_TIME_(); /* timestamp */
        QS_2U8_((uint8_t)me->prevPrio,  /* the previouis lock priority */
                (uint8_t)QK_lockPrio_); /* the current lock priority */
    QS_END_NOCRIT_()

    p = me->prevPrio;
    if (QK_lockPrio_ > p) {
        QK_lockPrio_ = p; /* restore the previous lock prio */
        p = QK_schedPrio_(); /* find the highest-prio AO ready to run */
        if (p != (uint_fast8_t)0) { /* priority found? */
            QK_sched_(p); /* schedule any unlocked AOs */
        }
    }
    QF_CRIT_EXIT_();
}
