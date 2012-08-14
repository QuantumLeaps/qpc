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
* \brief QTimeEvt_ctr() implementation.
*/

/*..........................................................................*/
QTimeEvtCtr QTimeEvt_ctr(QTimeEvt const * const me) {
    QTimeEvtCtr ret;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    ret = me->ctr;

    QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_CTR, QS_teObj_, me)
        QS_TIME_();                                            /* timestamp */
        QS_OBJ_(me);                              /* this time event object */
        QS_OBJ_(me->act);                              /* the active object */
        QS_TEC_(ret);                                /* the current counter */
        QS_TEC_(me->interval);                              /* the interval */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
    return ret;
}
