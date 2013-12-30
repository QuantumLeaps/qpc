/*****************************************************************************
* Product: QEP/C
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Nov 30, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
* \brief QFsm_ctor() and QFsm_init_() implementations.
*/

/*..........................................................................*/
void QFsm_ctor(QFsm * const me, QStateHandler initial) {
    static QMsmVtbl const vtbl = {                    /* QFsm virtual table */
        &QFsm_init_,
        &QFsm_dispatch_
    };
    /* do not call the QMsm_ctor() here, see NOTE01 */
    me->vptr = &vtbl;            /* hook the vptr to the QFsm virtual table */
    me->state.fun = Q_STATE_CAST(0);
    me->temp.fun  = initial;
}
/*..........................................................................*/
void QFsm_init_(QFsm * const me, QEvt const * const e) {
    QS_CRIT_STAT_

    Q_REQUIRE((me->vptr != (QMsmVtbl const *)0)    /* ctor must be executed */
              && (me->temp.fun != Q_STATE_CAST(0)) /* ctor must be executed */
              && (me->state.fun == Q_STATE_CAST(0)));/*init tran. NOT taken */

    QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.smObjFilter, me)
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(Q_STATE_CAST(0));   /* source state (not defined for a FSM) */
        QS_FUN_(me->temp.fun);              /* the target of the transition */
    QS_END_()

                                 /* execute the top-most initial transition */
    Q_ALLEGE((*me->temp.fun)(me, e) == (QState)Q_RET_TRAN);/* must be taken */

    (void)QEP_TRIG_(me->temp.fun, Q_ENTRY_SIG);         /* enter the target */
    me->state.fun = me->temp.fun;            /* record the new active state */

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS_priv_.smObjFilter, me)
        QS_TIME_();                                           /* time stamp */
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(me->state.fun);                     /* the new active state */
    QS_END_()
}

/*****************************************************************************
* NOTE01:
* QFsm inherits QMsm, so by the "inheritance of structures" convention
* it should call the constructor of the superclass, i.e., QMsm_ctor().
* However, this would pull in the QMsmVtbl, which in turn will pull in
* the code for QMsm_init_() and QMsm_dispatch_() implemetations. To avoid
* this code size penalty, in case QMsm is not used in a given project,
* the QFsm_ctor() performs direct intitialization of the Vtbl, which avoids
* pulling in the code for QMsm.
*/
