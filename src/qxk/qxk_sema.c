/**
* @file
* @brief QXK preemptive kernel semaphore functions
* @ingroup qxk
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
#include "qxk_pkg.h"      /* QXK package-scope internal interface */
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

Q_DEFINE_THIS_MODULE("qxk_sema")

/****************************************************************************/
/**
* @description
* Initializes a semaphore with the specified count and maximum count.
* If the semaphore is used for resource sharing, both the initial count
* and maximum count should be set to the number of identical resources
* guarded by the semaphore. If the semaphore is used as a signaling
* mechanism, the initial count should set to 0 and maximum count to 1
* (binary semaphore).
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     count  initial value of the semaphore counter
* @param[in]     max_count  maximum value of the semaphore counter.
*                The purpose of the max_count is to limit the counter
*                so that the semaphore cannot unblock more times than
*                the maximum.
*
* @note
* QXSemaphore_init() must be called **before** the semaphore can be used
* (signaled or waited on).
*/
void QXSemaphore_init(QXSemaphore * const me, uint_fast16_t count,
                      uint_fast16_t max_count)
{
    /** @pre max_count must be greater than zero */
    Q_REQUIRE_ID(100, max_count > (uint_fast16_t)0);
    me->count     = (uint16_t)count;
    me->max_count = (uint16_t)max_count;
    QPSet_setEmpty(&me->waitSet);
}

/****************************************************************************/
/**
* @description
* When an extended thread calls QXSemaphore_wait() and the value of the
* semaphore counter is greater than 0, QXSemaphore_wait() decrements the
* semaphore counter and returns (true) to its caller. However, if the value
* of the semaphore counter is 0, the function places the calling thread in
* the waiting list for the semaphore. The thread waits until the semaphore
* is signaled by calling QXSemaphore_signal(), or the specified timeout
* expires. If the semaphore is signaled before the timeout expires, QXK
* resumes the highest-priority extended thread waiting for the semaphore.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     nTicks number of clock ticks (at the associated rate)
*                       to wait for the semaphore. The value of
*                       QXTHREAD_NO_TIMEOUT indicates that no timeout will
*                       occur and the semaphore will wait indefinitely.
* @param[in]  tickRate  system clock tick rate serviced in this call.
*
* @returns
* 'true' if the semaphore has been signaled and 'false' if a timeout occured.
*
* @note
* Multiple extended threads can wait for a given semahpre.
*/
bool QXSemaphore_wait(QXSemaphore * const me, uint_fast16_t const nTicks) {
    QXThread *curr;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    curr = (QXThread *)QXK_attr_.curr;

    /** @pre this function must:
    * - NOT be called from an ISR;
    * - the semaphore must be initialized
    * - be called from an extended thread;
    * - the thread must NOT be holding a scheduler lock;
    * - the thread must NOT be already blocked on any object.
    */
    Q_REQUIRE_ID(200, (!QXK_ISR_CONTEXT_()) /* can't block inside an ISR */
        && (me->max_count > (uint16_t)0) /* sema must be initialized */
        && (curr != (QXThread *)0) /* curr must be extended */
        && (QXK_attr_.lockHolder != curr->super.prio) /* not holding a lock */
        && (curr->super.super.temp.obj == (QMState *)0)); /* not blocked */

    if (me->count > (uint16_t)0) {
        --me->count;
        curr->timeEvt.super.sig = (QSignal)QXK_SEMA_SIG; /* non-zero signal */
    }
    else {
        /* remember the blocking object */
        curr->super.super.temp.obj = (QMState const *)me;
        QXThread_teArm_(curr, (QSignal)QXK_SEMA_SIG, nTicks);
        QPSet_insert(&me->waitSet,        (uint_fast8_t)curr->super.prio);
        QPSet_remove(&QXK_attr_.readySet, (uint_fast8_t)curr->super.prio);
        (void)QXK_sched_(); /* schedule the next thread */
        QF_CRIT_EXIT_();
        QF_CRIT_EXIT_NOP(); /* BLOCK here */

        QF_CRIT_ENTRY_();
        /* the blocking object must be this semaphore */
        Q_ASSERT_ID(210, curr->super.super.temp.obj == (QMState *)me);
        curr->super.super.temp.obj = (QMState *)0; /* clear */
    }
    QF_CRIT_EXIT_();

    /* signal of non-zero means that the time event has not expired */
    return (bool)(curr->timeEvt.super.sig != (QSignal)0);
}

/****************************************************************************/
/**
* @description
* This function checks if the semaphore counter is greater than 0,
* in which case the counter is decremented.
*
* @param[in,out] me     pointer (see @ref oop)
*
* @returns
* 'true' if the semaphore has count available and 'false' NOT available.
*
* @note
* This function can be called from any context, including ISRs and basic
* threds (active objects).
*/
bool QXSemaphore_tryWait(QXSemaphore * const me) {
    bool isAvailable;
    QF_CRIT_STAT_

    /** @pre the semaphore must be initialized */
    Q_REQUIRE_ID(300, (me->max_count > (uint16_t)0));

    QF_CRIT_ENTRY_();
    /* is the semaphore available? */
    if (me->count > (uint16_t)0) {
        --me->count;
        isAvailable = true;
    }
    else { /* the semaphore is NOT available (would block) */
        isAvailable = false;
    }
    QF_CRIT_EXIT_();

    return isAvailable;
}

/****************************************************************************/
/**
* @description
* If the semaphore counter value is 0 or more, it is incremented, and
* this function returns to its caller. If the extended threads are waiting
* for the semaphore to be signaled, QXSemaphore_signal() removes the highest-
* priority thread waiting for the semaphore from the waiting list and makes
* this thread ready-to-run. The QXK scheduler is then called to determine if
* the awakened thread is now the highest-priority thread that is ready-to-run.
*
* @param[in,out] me     pointer (see @ref oop)
*
* @returns
* 'true' when the semaphore signaled and 'false' when the semaphore count
* exceeded the maximum.
*
* @note
* A semaphore can be signaled from many places, including from ISRs, basic
* threads (AOs), and extended threads.
*/
bool QXSemaphore_signal(QXSemaphore * const me) {
    bool signaled = true; /* assume that the semaphore will be signaled */
    QF_CRIT_STAT_

    /** @pre the semaphore must be initialized */
    Q_REQUIRE_ID(400, (me->max_count > (uint16_t)0));

    QF_CRIT_ENTRY_();
    if (QPSet_notEmpty(&me->waitSet)) {
        uint_fast8_t p;
        QXThread *thr;

        /* find the highest-priority thread waiting on this semaphore */
        QPSet_findMax(&me->waitSet,       p);
        QPSet_insert(&QXK_attr_.readySet, p);
        QPSet_remove(&me->waitSet,        p);
        thr = (QXThread *)QF_active_[p];

        /* the thread must be registered in QF;
        * the thread must be extended;
        * must be blocked on this semaphore; and
        * the semaphore count must be zero (semaphore not signaled)
        */
        Q_ASSERT_ID(410, (thr != (QXThread *)0) /* must be registered */
            && (thr->super.osObject != (struct QActive *)0) /* extended thr.*/
            && (thr->super.super.temp.obj == (QMState const *)me)
            && (me->count == (uint16_t)0)); /* sema counter must be 0 */

        /* disarm the internal time event */
        (void)QXThread_teDisarm_(thr);

        if (!QXK_ISR_CONTEXT_()) { /* not inside ISR? */
            /* schedule the next thread if multitasking started */
            (void)QXK_sched_();
        }
    }
    else {
        if (me->count < me->max_count) {
            ++me->count;
        }
        else {
            signaled = false; /* semaphore NOT signaled */
        }
    }
    QF_CRIT_EXIT_();

    return signaled;
}

