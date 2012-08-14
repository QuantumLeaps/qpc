/*****************************************************************************
* Product: QEP/C
* Last Updated for Version: 4.5.01
* Date of the Last Update:  Jun 13, 2012
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

Q_DEFINE_THIS_MODULE("qfsm_dis")

/**
* \file
* \ingroup qep
* \brief QFsm_dispatch() implementation.
*/

/*..........................................................................*/
void QFsm_dispatch(QFsm * const me, QEvt const * const e) {
    QState r;
    QS_CRIT_STAT_

    Q_REQUIRE(me->state == me->temp); /* state configuration must be stable */

    QS_BEGIN_(QS_QEP_DISPATCH, QS_smObj_, me)
        QS_TIME_();                                           /* time stamp */
        QS_SIG_(e->sig);                         /* the signal of the event */
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(me->state);                            /* the current state */
    QS_END_()

    r = (*me->state)(me, e);                      /* call the event handler */
    if (r == Q_RET_TRAN) {                             /* transition taken? */

        QS_BEGIN_(QS_QEP_TRAN, QS_smObj_, me)
            QS_TIME_();                                       /* time stamp */
            QS_SIG_(e->sig);                     /* the signal of the event */
            QS_OBJ_(me);                       /* this state machine object */
            QS_FUN_(me->state);             /* the source of the transition */
            QS_FUN_(me->temp);              /* the target of the transition */
        QS_END_()

        QEP_EXIT_(me->state);                            /* exit the source */
        QEP_ENTER_(me->temp);                           /* enter the target */
        me->state = me->temp;                /* record the new active state */
    }
    else {                                          /* transition not taken */
#ifdef Q_SPY

        if (r == Q_RET_UNHANDLED) {
            QS_BEGIN_(QS_QEP_UNHANDLED, QS_smObj_, me)
                QS_SIG_(e->sig);                 /* the signal of the event */
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(me->state);                    /* the current state */
            QS_END_()
        }

        if (r == Q_RET_HANDLED) {

            QS_BEGIN_(QS_QEP_INTERN_TRAN, QS_smObj_, me)
                QS_TIME_();                                   /* time stamp */
                QS_SIG_(e->sig);                 /* the signal of the event */
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(me->state);                    /* the current state */
            QS_END_()

        }
        else {

            QS_BEGIN_(QS_QEP_IGNORED, QS_smObj_, me)
                QS_TIME_();                                   /* time stamp */
                QS_SIG_(e->sig);                 /* the signal of the event */
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(me->state);                    /* the current state */
            QS_END_()

        }
#endif
    }
}
