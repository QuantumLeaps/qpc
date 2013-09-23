/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 5.1.0
* Date of the Last Update:  Sep 18, 2013
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

Q_DEFINE_THIS_MODULE("qte_rarm")

/**
* \file
* \ingroup qf
* \brief QTimeEvt_rearm() implementation.
*/

/*..........................................................................*/
uint8_t QTimeEvt_rearm(QTimeEvt * const me, QTimeEvtCtr const nTicks) {
    uint8_t tickRate = (uint8_t)(me->super.refCtr_ & (uint8_t)0x7F);
    uint8_t isArmed;
    QF_CRIT_STAT_

    Q_REQUIRE((me->act != (void *)0)                    /* AO must be valid */
              && (tickRate < (uint8_t)QF_MAX_TICK_RATE) /* must be in range */
              && (nTicks != (QTimeEvtCtr)0)      /* cannot arm with 0 ticks */
              && (me->super.sig >= (QSignal)Q_USER_SIG));   /* valid signal */

    QF_CRIT_ENTRY_();
    if (me->ctr == (QTimeEvtCtr)0) {        /* is the time evt not running? */
        isArmed = (uint8_t)0;
                                  /* is the time event unlinked? see NOTE01 */
        if ((me->super.refCtr_ & (uint8_t)0x80) == (uint8_t)0) {
            me->super.refCtr_ |= (uint8_t)0x80;           /* mark as linked */
            me->next = QF_timeEvtHead_[tickRate].act;         /* see NOTE02 */
            QF_timeEvtHead_[tickRate].act = me;
        }
    }
    else {                                       /* the time event is armed */
        isArmed = (uint8_t)1;
    }
    me->ctr = nTicks;       /* re-load the tick counter (shift the phasing) */

    QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_REARM, QS_priv_.teObjFilter, me)
        QS_TIME_();                                            /* timestamp */
        QS_OBJ_(me);                              /* this time event object */
        QS_OBJ_(me->act);                                  /* the target AO */
        QS_TEC_(me->ctr);                            /* the number of ticks */
        QS_TEC_(me->interval);                              /* the interval */
        QS_2U8_(tickRate, isArmed);  /* the tick rate, was the timer armed? */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
    return isArmed;
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
