/**
* \file
* \brief ::QF_timeEvtHead_[], QF_tickX_() and QF_noTimeEvtsActiveX()
* definitions
* \ingroup qf
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-04-09
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* \endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qf_tick")

/* Package-scope objects ****************************************************/
QTimeEvt QF_timeEvtHead_[QF_MAX_TICK_RATE]; /* heads of time event lists */

/****************************************************************************/
/**
* \description
* This function must be called periodically from a time-tick ISR or from
* a task so that QF can manage the timeout events assigned to the given
* system clock tick rate.
*
* \arguments
* \arg[in]  \c tickRate  system clock tick rate serviced in this call.
*
* \note this function should be called only via the macro QF_TICK_X()
*
* \note the calls to QF_tickX_() with different tick rate argument can
* preempt each other. For example, higher clock tick rates might be serviced
* from interrupts while others from tasks (active objects).
*
* \sa ::QTimeEvt.
*/
#ifndef Q_SPY
void QF_tickX_(uint_fast8_t const tickRate)
#else
void QF_tickX_(uint_fast8_t const tickRate, void const * const sender)
#endif
{
    QTimeEvt *prev = &QF_timeEvtHead_[tickRate];
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_TICK, (void *)0, (void *)0)
        QS_TEC_((QTimeEvtCtr)(++prev->ctr)); /* tick ctr */
        QS_U8_((uint8_t)tickRate);           /* tick rate */
    QS_END_NOCRIT_()

    /* scan the linked-list of time events at this rate... */
    for (;;) {
        QTimeEvt *t = prev->next;  /* advance down the time evt. list */

        /* end of the list? */
        if (t == (QTimeEvt *)0) {

            /* ny newly armed time events? */
            if (QF_timeEvtHead_[tickRate].act != (void *)0) {

                /* sanity check */
                Q_ASSERT_ID(110, prev != (QTimeEvt *)0);
                prev->next = (QTimeEvt *)QF_timeEvtHead_[tickRate].act;
                QF_timeEvtHead_[tickRate].act = (void *)0;
                t = prev->next;  /* switch to the new list */
            }
            else {
                break; /* all currently armed time evts. processed */
            }
        }

        /* time evt. scheduled for removal? */
        if (t->ctr == (QTimeEvtCtr)0) {
            prev->next = t->next;
            t->super.refCtr_ &= (uint8_t)0x7F; /* mark as unlinked */
            /* do NOT advance the prev pointer */
            QF_CRIT_EXIT_(); /* exit crit. section to reduce latency */

            /* prevent merging critical sections, see NOTE1 below  */
            QF_CRIT_EXIT_NOP();
        }
        else {
            --t->ctr;

            /* is time event about to expire? */
            if (t->ctr == (QTimeEvtCtr)0) {
                QActive *act = (QActive *)t->act; /* temporary for volatile */

                /* periodic time evt? */
                if (t->interval != (QTimeEvtCtr)0) {
                    t->ctr = t->interval; /* rearm the time event */
                    prev = t; /* advance to this time event */
                }
                /* one-shot time event: automatically disarm */
                else {
                    prev->next = t->next;
                    t->super.refCtr_ &= (uint8_t)0x7F; /* mark as unlinked */
                    /* do NOT advance the prev pointer */

                    QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_AUTO_DISARM,
                                     QS_priv_.teObjFilter, t)
                        QS_OBJ_(t);            /* this time event object */
                        QS_OBJ_(act);          /* the target AO */
                        QS_U8_((uint8_t)tickRate); /* tick rate */
                    QS_END_NOCRIT_()
                }

                QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_POST, QS_priv_.teObjFilter, t)
                    QS_TIME_();                /* timestamp */
                    QS_OBJ_(t);                /* the time event object */
                    QS_SIG_(t->super.sig);     /* signal of this time event */
                    QS_OBJ_(act);              /* the target AO */
                    QS_U8_((uint8_t)tickRate); /* tick rate */
                QS_END_NOCRIT_()

                QF_CRIT_EXIT_(); /* exit critical section before posting */

                /* QACTIVE_POST() asserts internally if the queue overflows */
                QACTIVE_POST(act, &t->super, sender);
            }
            else {
                prev = t;         /* advance to this time event */
                QF_CRIT_EXIT_();  /* exit crit. section to reduce latency */

                /* prevent merging critical sections, see NOTE1 below  */
                QF_CRIT_EXIT_NOP();
            }
        }
        QF_CRIT_ENTRY_(); /* re-enter crit. section to continue */
    }
    QF_CRIT_EXIT_();
}

/*****************************************************************************
* NOTE1:
* In some QF ports the critical section exit takes effect only on the next
* machine instruction. If this case, the next instruction is another entry
* to a critical section, the critical section won't be really exited, but
* rather the two adjacent critical sections would be merged.
*
* The QF_CRIT_EXIT_NOP() macro contains minimal code required
* to prevent such merging of critical sections in QF ports,
* in which it can occur.
*/


/****************************************************************************/
/**
* \description
* Find out if any time events are armed at the given clock tick rate.
*
* \arguments
* \arg[in]  \c tickRate  system clock tick rate to find out about.
*
* \returns 'true' if no time events are armed at the given tick rate and
* 'false' otherwise.
*
* \note This function should be called in critical section.
*/
bool QF_noTimeEvtsActiveX(uint_fast8_t const tickRate) {
    bool inactive;

    /** \pre the tick rate must be in range */
    Q_REQUIRE_ID(200, tickRate < (uint_fast8_t)QF_MAX_TICK_RATE);

    if (QF_timeEvtHead_[tickRate].next != (QTimeEvt *)0) {
        inactive = false;
    }
    else if ((QF_timeEvtHead_[tickRate].act != (void *)0)) {
        inactive = false;
    }
    else {
        inactive = true;
    }
    return inactive;
}

