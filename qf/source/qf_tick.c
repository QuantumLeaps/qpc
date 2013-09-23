/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 5.0.0
* Date of the Last Update:  Sep 12, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qf_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qf_tick")

/**
* \file
* \ingroup qf
* \brief QF_tickX() and QF_noTimeEvtsActiveX() implementation.
*/

/* Package-scope objects ---------------------------------------------------*/
QTimeEvt QF_timeEvtHead_[QF_MAX_TICK_RATE];    /* heads of time event lists */

/*..........................................................................*/
#ifndef Q_SPY
void QF_tickX(uint8_t const tickRate)                         /* see NOTE01 */
#else
void QF_tickX(uint8_t const tickRate, void const * const sender)
#endif
{
    QTimeEvt *prev = &QF_timeEvtHead_[tickRate];
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_TICK, (void *)0, (void *)0)
        QS_TEC_((QTimeEvtCtr)(++prev->ctr));                    /* tick ctr */
        QS_U8_(tickRate);                                      /* tick rate */
    QS_END_NOCRIT_()

    for (;;) {
        QTimeEvt *t = prev->next;        /* advance down the time evt. list */
        if (t == (QTimeEvt *)0) {                       /* end of the list? */
            if (QF_timeEvtHead_[tickRate].act != (void *)0) { /* new armed? */
                Q_ASSERT(prev != (QTimeEvt *)0);            /* sanity check */
                prev->next = QF_timeEvtHead_[tickRate].act;
                QF_timeEvtHead_[tickRate].act = (void *)0;
                t = prev->next;                   /* switch to the new list */
            }
            else {
                break;          /* all currently armed time evts. processed */
            }
        }

        if (t->ctr == (QTimeEvtCtr)0) { /* time evt. scheduled for removal? */
            prev->next = t->next;
            t->super.refCtr_ &= (uint8_t)0x7F;          /* mark as unlinked */
            /* do NOT advance the prev pointer */
            QF_CRIT_EXIT_();        /* exit crit. section to reduce latency */
            QF_CRIT_EXIT_NOP();                               /* see NOTE02 */
        }
        else {
            --t->ctr;
            if (t->ctr == (QTimeEvtCtr)0) { /* is time evt about to expire? */
                QActive *act = (QActive *)t->act; /* temporary for volatile */

                if (t->interval != (QTimeEvtCtr)0) {  /* periodic time evt? */
                    t->ctr = t->interval;           /* rearm the time event */
                    prev = t;                 /* advance to this time event */
                }
                else {         /* one-shot time event: automatically disarm */
                    prev->next = t->next;
                    t->super.refCtr_ &= (uint8_t)0x7F;  /* mark as unlinked */
                    /* do NOT advance the prev pointer */

                    QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_AUTO_DISARM,
                                     QS_priv_.teObjFilter, t)
                        QS_OBJ_(t);               /* this time event object */
                        QS_OBJ_(act);                      /* the target AO */
                        QS_U8_(tickRate);                      /* tick rate */
                    QS_END_NOCRIT_()
                }

                QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_POST, QS_priv_.teObjFilter, t)
                    QS_TIME_();                                /* timestamp */
                    QS_OBJ_(t);                    /* the time event object */
                    QS_SIG_(t->super.sig);     /* signal of this time event */
                    QS_OBJ_(act);                          /* the target AO */
                    QS_U8_(tickRate);                          /* tick rate */
                QS_END_NOCRIT_()

                QF_CRIT_EXIT_();       /* exit crit. section before posting */
                /* QACTIVE_POST() asserts internally if the queue overflows */
                QACTIVE_POST(act, &t->super, sender);
            }
            else {
                prev = t;                     /* advance to this time event */
                QF_CRIT_EXIT_();    /* exit crit. section to reduce latency */
                QF_CRIT_EXIT_NOP();                           /* see NOTE02 */
            }
        }
        QF_CRIT_ENTRY_();             /* re-enter crit. section to continue */
    }
    QF_CRIT_EXIT_();
}
/*..........................................................................*/
uint8_t QF_noTimeEvtsActiveX(uint8_t const tickRate) {        /* see NOTE03 */
    uint8_t active;

    Q_REQUIRE(tickRate < (uint8_t)QF_MAX_TICK_RATE);    /* must be in range */

    if (QF_timeEvtHead_[tickRate].next != (QTimeEvt *)0) {
        active = (uint8_t)1;
    }
    else if ((QF_timeEvtHead_[tickRate].act != (void *)0)) {
        active = (uint8_t)1;
    }
    else {
        active = (uint8_t)0;
    }
    return active;
}

/*****************************************************************************
* NOTE01:
* QF_tickX() must always run to completion and never preempt itself.
* In particular, if QF_tickX() runs in an ISR, the ISR is not allowed to
* preempt itself. Also, QF_tickX() should not be called from two different
* ISRs, which potentially could preempt each other.
*
* NOTE02:
* In some QF ports the critical section exit takes effect only on the next
* machine instruction. If this case, the next instruction is another entry
* to a critical section, the critical section won't be really exited, but
* rather the two adjecent critical sections would be merged.
* The QF_CRIT_EXIT_NOP() macro contains minimal code required to prevent
* such merging of critical sections in QF ports, in which it can occur.
*
* NOTE03:
* QF::noTimeEvtsActiveX() must be invoked from within a critical section.
*/
