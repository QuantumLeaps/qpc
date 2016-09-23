/**
* @file
* @brief QXK preemptive kernel semaphore functions
* @ingroup qxk
* @cond
******************************************************************************
* Last updated for version 5.7.1
* Last updated on  2016-09-18
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
void QXSemaphore_init(QXSemaphore * const me, uint_fast16_t count) {
    me->count = count;
    QPSet_setEmpty(&me->waitSet);
}

/****************************************************************************/
bool QXSemaphore_wait(QXSemaphore * const me,
                      uint_fast16_t const nTicks, uint_fast8_t const tickRate)
{
    QXThread *thr;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    thr = (QXThread *)QXK_attr_.curr;

    Q_REQUIRE_ID(100, (!QXK_ISR_CONTEXT_()) /* can't block inside an ISR */
        && (thr != (QXThread *)0)); /* current thread must be extended */

    if (me->count > (uint_fast16_t)0) {
        --me->count;
    }
    else {
#ifndef NDEBUG
        /* remember the blocking object */
        thr->super.super.temp.obj = (QMState const *)me;
#endif /* NDEBUG */
        QXThread_teArm_(thr, (QSignal)QXK_SEMA_SIG, nTicks, tickRate);
        QPSet_insert(&me->waitSet,        thr->super.prio);
        QPSet_remove(&QXK_attr_.readySet, thr->super.prio);
        (void)QXK_sched_();
    }
    QF_CRIT_EXIT_();

    /* signal of non-zero means that the time event has not expired */
    return (bool)(thr->timeEvt.super.sig != (QSignal)0);
}

/****************************************************************************/
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

        Q_ASSERT_ID(910, (thr->super.thread != (void *)0) /* extended thread */
            && (me->count == (uint_fast16_t)0)); /* sema counter must be 0 */

        (void)QXThread_teDisarm_(thr);

        if ((!QXK_ISR_CONTEXT_()) /* not inside ISR? */
            && (QF_active_[0] != (QMActive *)0))  /* kernel started? */
        {
            (void)QXK_sched_();
        }
    }
    else {
        ++me->count;
    }
    QF_CRIT_EXIT_();
}

