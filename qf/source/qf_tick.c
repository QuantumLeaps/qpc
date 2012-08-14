/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 17, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
* \brief QF_tick() implementation.
*/

/*..........................................................................*/
#ifndef Q_SPY
void QF_tick(void)                                            /* see NOTE01 */
#else
void QF_tick(void const * const sender)
#endif
{
    QTimeEvt *t;
    QTimeEvt *prev = (QTimeEvt *)0;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_TICK, (void *)0, (void *)0)
        QS_TEC_((QTimeEvtCtr)(++QS_tickCtr_));          /* the tick counter */
    QS_END_NOCRIT_()

    for (t = QF_timeEvtListHead_; t != (QTimeEvt *)0; t = t->next) {
        if (t->ctr == (QTimeEvtCtr)0) { /* time evt. scheduled for removal? */
            if (t == QF_timeEvtListHead_) {
                QF_timeEvtListHead_ = t->next;
            }
            else {
                Q_ASSERT(prev != (QTimeEvt *)0);
                prev->next = t->next;
            }
            QF_EVT_REF_CTR_DEC_(&t->super);           /* mark as not linked */
        }
        else {
            --t->ctr;
            if (t->ctr == (QTimeEvtCtr)0) { /* is time evt about to expire? */
                if (t->interval != (QTimeEvtCtr)0) {  /* periodic time evt? */
                    t->ctr = t->interval;           /* rearm the time event */
                }
                else {
                    QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_AUTO_DISARM, QS_teObj_, t)
                        QS_OBJ_(t);               /* this time event object */
                        QS_OBJ_(t->act);               /* the active object */
                    QS_END_NOCRIT_()
                }

                QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_POST, QS_teObj_, t)
                    QS_TIME_();                                /* timestamp */
                    QS_OBJ_(t);                    /* the time event object */
                    QS_SIG_(t->super.sig);     /* signal of this time event */
                    QS_OBJ_(t->act);                   /* the active object */
                QS_END_NOCRIT_()

                QF_CRIT_EXIT_();       /* exit crit. section before posting */
                /* QACTIVE_POST() asserts internally if the queue overflows */
                QACTIVE_POST(t->act, &t->super, sender);
                QF_CRIT_ENTRY_();     /* re-enter crit. section to continue */

                if (t->ctr == (QTimeEvtCtr)0) {  /* still marked to expire? */
                    if (t == QF_timeEvtListHead_) {
                        QF_timeEvtListHead_ = t->next;
                    }
                    else {
                        Q_ASSERT(prev != (QTimeEvt *)0);
                        prev->next = t->next;
                    }
                    QF_EVT_REF_CTR_DEC_(&t->super);      /* mark as removed */
                }
                else {
                    prev = t;
                }
            }
            else {
                prev = t;
            }
        }
    }
    QF_CRIT_EXIT_();
}

/*****************************************************************************
* NOTE01:
* QF_tick() must always run to completion and never preempt itself.
* In particular, if QF_tick() runs in an ISR, the ISR is not allowed to
* preempt itself. Also, QF_tick() should not be called from two different
* ISRs, which potentially could preempt each other.
*
* NOTE02:
* On many CPUs, the interrupt unlocking takes only effect on the next
* machine instruction, which happens to be here another interrupt lock.
* The assignment of a volatile variable requires a few instructions, which
* the compiler cannot optimize away. This ensures that the interrupts get
* actually unlocked, so that the interrupt latency stays low.
*/
