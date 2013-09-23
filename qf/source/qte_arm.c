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

Q_DEFINE_THIS_MODULE("qte_arm")

/**
* \file
* \ingroup qf
* \brief QF_timeEvtHead_ definition;
* QTimeEvt_armX() implementation.
*/

/*..........................................................................*/
void QTimeEvt_armX(QTimeEvt * const me,
                   QTimeEvtCtr const nTicks, QTimeEvtCtr const interval)
{
    uint8_t tickRate = (uint8_t)(me->super.refCtr_ & (uint8_t)0x7F);
    QTimeEvtCtr ctr = me->ctr;
    QF_CRIT_STAT_

    Q_REQUIRE((me->act != (void *)0)                    /* AO must be valid */
              && (ctr == (QTimeEvtCtr)0)                /* must be disarmed */
              && (nTicks != (QTimeEvtCtr)0)      /* cannot arm with 0 ticks */
              && (tickRate < (uint8_t)QF_MAX_TICK_RATE) /* must be in range */
              && (me->super.sig >= (QSignal)Q_USER_SIG));   /* valid signal */

    QF_CRIT_ENTRY_();
    me->ctr = nTicks;
    me->interval = interval;
                                 /* is the time event unlinked?, see NOTE01 */
    if ((me->super.refCtr_ & (uint8_t)0x80) == (uint8_t)0) {
        me->super.refCtr_ |= (uint8_t)0x80;               /* mark as linked */
        me->next = QF_timeEvtHead_[tickRate].act;             /* see NOTE02 */
        QF_timeEvtHead_[tickRate].act = me;
    }

    QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_ARM, QS_priv_.teObjFilter, me)
        QS_TIME_();                                            /* timestamp */
        QS_OBJ_(me);                              /* this time event object */
        QS_OBJ_(me->act);                              /* the active object */
        QS_TEC_(nTicks);                             /* the number of ticks */
        QS_TEC_(interval);                                  /* the interval */
        QS_U8_(tickRate);                                      /* tick rate */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
}

/*****************************************************************************
* NOTE01:
* For a duration of a single clock tick of the specified tick rate a time
* event can be disarmed and yet still linked into the list, because unlinking
* is performed exclusively in the QF_tickX() function.
*
* NOTE02:
* The time event is initially inserted into the separate "freshly armed"
* link list based on QF_timeEvtHead_[tickRate].act. Only later, inside
* the QF_tickX() function, the "freshly armed" list is appended to the
* main list of armed time events based on QF_timeEvtHead_[tickRate].next.
* Again, this is to keep any changes to the main list exclusively inside
* the QF_tickX() function.
*/
