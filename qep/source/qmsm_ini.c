/*****************************************************************************
* Product: QEP/C
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 20, 2013
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

Q_DEFINE_THIS_MODULE("qmsm_ini")

/**
* \file
* \ingroup qep
* \brief QMsm_ctor() and QMsm_init_() implementations.
*/

/*..........................................................................*/
QActionHandler const QMsm_emptyAction_[1] = {
    Q_ACTION_CAST(0)
};

/*..........................................................................*/
void QMsm_ctor(QMsm * const me, QStateHandler initial) {
    static QMsmVtbl const vtbl = {                    /* QMsm virtual table */
        &QMsm_init_,
        &QMsm_dispatch_
    };
    me->vptr = &vtbl;
    me->state.obj = (void *)0;
    me->temp.fun  = initial;
}
/*..........................................................................*/
void QMsm_init_(QMsm * const me, QEvt const * const e) {
    QState r;
#ifdef Q_SPY
    QMState const *t = (QMState const *)0;       /* the current state (top) */
    QS_CRIT_STAT_
#endif

    Q_REQUIRE((me->vptr != (QMsmVtbl const *)0)    /* ctor must be executed */
              && (me->temp.fun != Q_STATE_CAST(0)) /* ctor must be executed */
              && (me->state.obj == (void *)0));   /*initial tran. NOT taken */

    r = (*me->temp.fun)(me, e);  /* execute the top-most initial transition */
    Q_ASSERT(r == (QState)Q_RET_INITIAL);    /* initial tran. must be taken */

    while (r == (QState)Q_RET_INITIAL) {
        QActionHandler const *a;

        QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.smObjFilter, me)
            QS_OBJ_(me);                       /* this state machine object */
            QS_FUN_((t == (QMState const *)0)
                    ? Q_STATE_CAST(0)
                    : t->stateHandler);          /* source of initial tran. */
            QS_FUN_(me->state.obj->stateHandler);   /* target state handler */
        QS_END_()

#ifdef Q_SPY
        t = me->state.obj;            /* store the target of the transition */
#endif
        r = (QState)0U;                      /* invalidate the return value */
        for (a = me->temp.act; *a != Q_ACTION_CAST(0); QEP_ACT_PTR_INC_(a)) {
            r = (*(*a))(me);     /* call the action through the 'a' pointer */
#ifdef Q_SPY
            if (r == (QState)Q_RET_ENTRY) {
                QS_BEGIN_(QS_QEP_STATE_ENTRY, QS_priv_.smObjFilter, me)
                    QS_OBJ_(me);
                    QS_FUN_(me->temp.obj->stateHandler);   /* entered state */
                QS_END_()
            }
#endif
        }
    }

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS_priv_.smObjFilter, me)
        QS_TIME_();                                           /* time stamp */
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(me->state.obj->stateHandler);      /* the new current state */
    QS_END_()
}
