/**
* @file
* @brief QXK preemptive kernel semaphore functions
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 5.9.5
* Last updated on  2017-07-20
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
* Initializes a semaphore with the specified count. If the semaphore is used
* for resource sharing, the initial value of the semaphore count should be
* set to the number of identical resources guarded by the semaphore. If the
* semaphore is used as a signaling mechanism, the initial count should set
* to 0.
*
* @param[in,out] me     pointer (see @ref oop)
* @param[in]     count  initial value of the semaphore counter
*
* @note
* QXSemaphore_init() must be called **before** the semaphore can be used
* (signaled or waited on).
*/
void QXSemaphore_init(QXSemaphore * const me, uint_fast16_t count) {
    me->count = count;
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
* @param[in]  nTicks    number of clock ticks (at the associated rate)
*                       to wait for the semaphore. The value of
*                       QXTHREAD_NO_TIMEOUT indicates that no timeout will
*                       occur and the semaphore will wait indefinitely.
* @param[in]  tickRate  system clock tick rate serviced in this call.
*
* @returns
* true if the semaphore has been signaled, and false if the timeout occured.
*
* @note
* Multiple extended threads can wait for a given semahpre.
*/
bool QXSemaphore_wait(QXSemaphore * const me,
                      uint_fast16_t const nTicks, uint_fast8_t const tickRate)
{
    QXThread *thr;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    thr = (QXThread *)QXK_attr_.curr;

    Q_REQUIRE_ID(100, (!QXK_ISR_CONTEXT_()) /* can't block inside an ISR */
        && (thr != (QXThread *)0) /* current thread must be extended */
        && (thr->super.super.temp.obj == (QMState const *)0)); /* !blocked */

    if (me->count > (uint_fast16_t)0) {
        --me->count;
        thr->timeEvt.super.sig = (QSignal)QXK_SEMA_SIG; /* non-zero signal */
    }
    else {
        /* remember the blocking object */
        thr->super.super.temp.obj = (QMState const *)me;
        QXThread_teArm_(thr, (QSignal)QXK_SEMA_SIG, nTicks, tickRate);
        QPSet_insert(&me->waitSet,        thr->super.prio);
        QPSet_remove(&QXK_attr_.readySet, thr->super.prio);
        (void)QXK_sched_();
        QF_CRIT_EXIT_();
        QF_CRIT_EXIT_NOP(); /* BLOCK here */

        QF_CRIT_ENTRY_();
        /* the blocking object must be this semaphore */
        Q_ASSERT_ID(110, thr->super.super.temp.obj == (QMState const *)me);
        thr->super.super.temp.obj = (QMState const *)0; /* clear */
    }
    QF_CRIT_EXIT_();

    /* signal of non-zero means that the time event has not expired */
    return (bool)(thr->timeEvt.super.sig != (QSignal)0);
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
* @note
* A semaphore can be signaled from many places, including from ISRs, basic
* threads (AOs), and extended threads.
*/
void QXSemaphore_signal(QXSemaphore * const me) {
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    if (QPSet_notEmpty(&me->waitSet)) {
        uint_fast8_t p;
        QXThread *thr;

        QPSet_findMax(&me->waitSet,       p);
        QPSet_insert(&QXK_attr_.readySet, p);
        QPSet_remove(&me->waitSet,        p);

        thr = (QXThread *)QF_active_[p]; /* thread waiting on the semaphore */

        Q_ASSERT_ID(210,
            (thr->super.osObject != (struct QActive *)0) /* must be extended */
            && (me->count == (uint_fast16_t)0)); /* sema counter must be 0 */

        /* disarm the internal time event */
        (void)QXThread_teDisarm_(thr);

        if (!QXK_ISR_CONTEXT_()) { /* not inside ISR? */
            (void)QXK_sched_();
        }
    }
    else {
        ++me->count;
    }
    QF_CRIT_EXIT_();
}

