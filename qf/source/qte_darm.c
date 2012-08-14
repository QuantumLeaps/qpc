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

/**
* \file
* \ingroup qf
* \brief QTimeEvt_disarm() implementation.
*/

/*..........................................................................*/
/* NOTE: disarm a timer (no harm in disarming an already disarmed timer)    */
uint8_t QTimeEvt_disarm(QTimeEvt * const me) {
    uint8_t wasArmed;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();
    if (me->ctr != (QTimeEvtCtr)0) {   /* is the time event actually armed? */
        wasArmed = (uint8_t)1;

        QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_DISARM, QS_teObj_, me)
            QS_TIME_();                                        /* timestamp */
            QS_OBJ_(me);                          /* this time event object */
            QS_OBJ_(me->act);                          /* the active object */
            QS_TEC_(me->ctr);                        /* the number of ticks */
            QS_TEC_(me->interval);                          /* the interval */
        QS_END_NOCRIT_()

        me->ctr = (QTimeEvtCtr)0;         /* schedule removal from the list */
    }
    else {                                  /* the time event was not armed */
        wasArmed = (uint8_t)0;

        QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_DISARM_ATTEMPT, QS_teObj_, me)
            QS_TIME_();                                        /* timestamp */
            QS_OBJ_(me);                          /* this time event object */
            QS_OBJ_(me->act);                          /* the active object */
        QS_END_NOCRIT_()
    }
    QF_CRIT_EXIT_();
    return wasArmed;
}
