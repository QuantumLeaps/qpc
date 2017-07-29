/**
* @file
* @ingroup qxk
* @brief QXMutex_init(), QXMutex_lock and QXMutex_unlock() definitions.
* @cond
******************************************************************************
* Last updated for version 5.9.6
* Last updated on  2017-07-27
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
* https://state-machine.com
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

enum {
    MUTEX_UNUSED = 0xFF
};

/****************************************************************************/
/**
* @description
* Initializes QXK priority-ceiling mutex to the specified ceiling priority.
*
* @param[in,out] me      pointer (see @ref oop)
* @param[in]     prio    ceiling priority of the mutex
*
* @note
* A mutex must be initialized before it can be locked or unlocked.
*
* @sa QXMutex_lock(), QXMutex_unlock()
*
* @usage
* The following example shows how to initialize, lock and unlock QXK mutex:
* @include qxk_mux.c
*/
void QXMutex_init(QXMutex * const me, uint_fast8_t prio) {
    me->lockPrio = prio;
    me->prevPrio = (uint_fast8_t)MUTEX_UNUSED;
}

/****************************************************************************/
/**
* @description
* This function locks the QXK mutex.
*
* @param[in,out] me      pointer (see @ref oop)
*
* @note
* A mutex must be initialized before it can be locked or unlocked.
*
* @note
* A QXK mutex can be used from both basic threads (AOs) and extended threads.
*
* @note
* QXMutex_lock() must be always followed by the corresponding
* QXMutex_unlock().
*
* @attention
* QXK will fire an assertion if a thread holding the mutex attempts
* to block.
*
* @sa QXMutex_init(), QXMutex_unlock()
*
* @usage
* The following example shows how to initialize, lock and unlock QXK mutex:
* @include qxk_mux.c
*/
void QXMutex_lock(QXMutex * const me) {
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    /** @pre The mutex cannot be locked from the ISR context
    * and the mutex must be unused
    */
    Q_REQUIRE_ID(700, (!QXK_ISR_CONTEXT_())
                      && (me->prevPrio == (uint_fast8_t)MUTEX_UNUSED));

    me->prevPrio   = QXK_attr_.lockPrio;   /* save previous lock prio */
    me->prevHolder = QXK_attr_.lockHolder; /* save previous lock holder */

    if (QXK_attr_.lockPrio < me->lockPrio) { /* raising the lock prio? */
        QXK_attr_.lockPrio = me->lockPrio;
    }
    QXK_attr_.lockHolder =
        (QXK_attr_.curr != (struct QActive *)0)
        ? QXK_attr_.curr->prio
        : (uint_fast8_t)0;

    QS_BEGIN_NOCRIT_(QS_SCHED_LOCK, (void *)0, (void *)0)
        QS_TIME_(); /* timestamp */
        QS_2U8_((uint8_t)me->prevPrio,   /* the previous lock prio */
                (uint8_t)QXK_attr_.lockPrio); /* the new lock prio */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
}

/****************************************************************************/
/**
* @description
* This function unlocks the QXK mutex.
*
* @param[in] me      pointer (see @ref oop)
*
* @note
* A mutex must be initialized before it can be locked or unlocked.
*
* @note
* A QXK mutex can be used from both basic threads (AOs) and extended threads.
*
* @note
* QXMutex_unlock() must always follow the corresponding QXMutex_lock().
*
* @attention
* QXK will fire an assertion if a thread holding the mutex attempts
* to block.
*
* @sa QXMutex_init(), QXMutex_lock()
*
* @usage
* The following example shows how to initialize, lock and unlock QXK mutex:
* @include qxk_mux.c
*/
void QXMutex_unlock(QXMutex * const me) {
    uint_fast8_t p;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    /** @pre The mutex cannot be unlocked from the ISR context
    * and the mutex must be used (NOT unused)
    */
    Q_REQUIRE_ID(800, (!QXK_ISR_CONTEXT_())
                 && (me->prevPrio != (uint_fast8_t)MUTEX_UNUSED));

    p = me->prevPrio;
    me->prevPrio = (uint_fast8_t)MUTEX_UNUSED;
    QXK_attr_.lockHolder = me->prevHolder; /* restore previous lock holder */

    if (QXK_attr_.lockPrio > p) {

        QS_BEGIN_NOCRIT_(QS_SCHED_UNLOCK, (void *)0, (void *)0)
            QS_TIME_(); /* timestamp */
            QS_2U8_((uint8_t)QXK_attr_.lockPrio, /* previouis lock priority */
                    (uint8_t)p);                 /* new lock priority */
        QS_END_NOCRIT_()

        QXK_attr_.lockPrio = p; /* restore the previous lock prio */
        /* find the highest-prio thread ready to run */
        if (QXK_sched_() != (uint_fast8_t)0) { /* priority found? */
            QXK_activate_(); /* activate any unlocked basic threads */
        }
    }
    else {
        QS_BEGIN_NOCRIT_(QS_SCHED_UNLOCK, (void *)0, (void *)0)
            QS_TIME_(); /* timestamp */
            p = QXK_attr_.lockPrio;
            QS_2U8_((uint8_t)p,  /* previouis lock priority */
                    (uint8_t)p); /* new lock priority */
        QS_END_NOCRIT_()
    }
    QF_CRIT_EXIT_();
}
