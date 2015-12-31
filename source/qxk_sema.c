/**
* @file
* @brief QXK preemptive kernel semaphore functions
* @ingroup qxk
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
    QF_bzero(&me->waitSet, (uint_fast16_t)sizeof(me->waitSet));
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
        /* this must be a "naked" thread (no state) */
        && (thr->super.super.state.act == (QActionHandler)0));

    if (me->count > (uint_fast16_t)0) {
        --me->count;
    }
    else {
        thr->super.super.temp.obj = (QMState const *)me;
        QXThread_teArm_(thr, (QSignal)QXK_SEMA_SIG, nTicks, tickRate);
        QXK_prioInsert(&me->waitSet,        thr->super.prio);
        QXK_prioRemove(&QXK_attr_.readySet, thr->super.prio);
        QXK_sched_();
    }
    QF_CRIT_EXIT_();

    /* signal of non-zero means that the time event has not expired */
    return (bool)(thr->timeEvt.super.sig != (QSignal)0);
}

/****************************************************************************/
void QXSemaphore_signal(QXSemaphore * const me) {
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    if (QXK_prioNotEmpty(&me->waitSet)) {
        uint_fast8_t p;

        QXK_prioFindMax(&me->waitSet,       p);
        QXK_prioInsert(&QXK_attr_.readySet, p);
        QXK_prioRemove(&me->waitSet,        p);

        (void)QXThread_teDisarm_((QXThread *)QF_active_[p]);

        /* the waitSet is not empty, so the semaphore count must be zero */
        Q_ASSERT_ID(910, me->count == (uint_fast16_t)0);

        if ((!QXK_ISR_CONTEXT_())              /* not inside ISR? */
            && (QXK_attr_.curr != (void *)0))  /* multitasking started? */
        {
            QXK_sched_();
        }
    }
    else {
        ++me->count;
    }
    QF_CRIT_EXIT_();
}

