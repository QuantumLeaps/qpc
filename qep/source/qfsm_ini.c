/*****************************************************************************
* Product: QEP/C
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
#include "qep_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qfsm_ini")

/**
* \file
* \ingroup qep
* \brief QFsm_init() implementation.
*/

/*..........................................................................*/
void QFsm_init(QFsm * const me, QEvt const * const e) {
    QS_CRIT_STAT_

    Q_REQUIRE((me->temp != Q_STATE_CAST(0))        /* ctor must be executed */
              && (me->state == Q_STATE_CAST(0))); /*initial tran. NOT taken */

    QS_BEGIN_(QS_QEP_STATE_INIT, QS_smObj_, me)
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(Q_STATE_CAST(0));   /* source state (not defined for a FSM) */
        QS_FUN_(me->temp);                  /* the target of the transition */
    QS_END_()

                                 /* execute the top-most initial transition */
    Q_ALLEGE((*me->temp)(me, e) == Q_RET_TRAN); /* transition must be taken */

    (void)QEP_TRIG_(me->temp, Q_ENTRY_SIG);             /* enter the target */
    me->state = me->temp;                    /* record the new active state */

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS_smObj_, me)
        QS_TIME_();                                           /* time stamp */
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(me->state);                         /* the new active state */
    QS_END_()
}
