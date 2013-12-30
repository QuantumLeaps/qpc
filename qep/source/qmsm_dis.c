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

Q_DEFINE_THIS_MODULE("qmsm_dis")

/**
* \file
* \ingroup qep
* \brief QMsm_dispatch_() implementation.
*/

static void QMsm_tran_(QHsm * const me);          /* static helper function */

/*..........................................................................*/
void QMsm_dispatch_(QMsm * const me, QEvt const * const e) {
    QMState const *s = me->state.obj;            /* store the current state */
    QMState const *t;
    QState r = (QState)Q_RET_HANDLED;
    QS_CRIT_STAT_

    Q_REQUIRE(s != (QMState const *)0);              /* must be initialized */

    QS_BEGIN_(QS_QEP_DISPATCH, QS_priv_.smObjFilter, me)
        QS_TIME_();                                           /* time stamp */
        QS_SIG_(e->sig);                         /* the signal of the event */
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(s->stateHandler);              /* the current state handler */
    QS_END_()

    for (t = s; t != (QMState const *)0; t = t->parent) {
        r = (*t->stateHandler)(me, e);
        if (r != (QState)Q_RET_SUPER) {
            if (r == (QState)Q_RET_UNHANDLED) {/* unhandled due to a guard? */
                QS_BEGIN_(QS_QEP_UNHANDLED, QS_priv_.smObjFilter, me)
                    QS_SIG_(e->sig);             /* the signal of the event */
                    QS_OBJ_(me);               /* this state machine object */
                    QS_FUN_(t->stateHandler);          /* the current state */
                QS_END_()
            }
            else {
                break;              /* event handled--break out of the loop */
            }
        }
    }

    if (r == (QState)Q_RET_TRAN) {                     /* transition taken? */
        Q_ASSERT(t != (QMState const *)0);    /* tran. source can't be null */

         /* exit states starting from the current state to the source state */
        for (; s != t; s = s->parent) {
            if (s->exitAction != Q_ACTION_CAST(0)) {/* exit action defined? */
                (void)(*s->exitAction)(me);      /* execute the exit action */

                QS_BEGIN_(QS_QEP_STATE_EXIT, QS_priv_.smObjFilter, me)
                    QS_OBJ_(me);               /* this state machine object */
                    QS_FUN_(s->stateHandler);   /* the exited state handler */
                QS_END_()
            }
        }

        QMsm_tran_(me);                       /* take the actual transition */

        QS_BEGIN_(QS_QEP_TRAN, QS_priv_.smObjFilter, me)
            QS_TIME_();                                       /* time stamp */
            QS_SIG_(e->sig);                     /* the signal of the event */
            QS_OBJ_(me);                       /* this state machine object */
            QS_FUN_(s->stateHandler);       /* the source of the transition */
            QS_FUN_(me->state.obj->stateHandler);   /* the new active state */
        QS_END_()
    }

#ifdef Q_SPY
    else {                                          /* transition NOT taken */
        if (r == (QState)Q_RET_HANDLED) {         /* was the event handled? */
            Q_ASSERT(t != (QMState const *)0);     /* internal tran. source */

            QS_BEGIN_(QS_QEP_INTERN_TRAN, QS_priv_.smObjFilter, me)
                QS_TIME_();                                   /* time stamp */
                QS_SIG_(e->sig);                 /* the signal of the event */
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(t->stateHandler);               /* the source state */
            QS_END_()

        }
        else if (t == (QMState const *)0) {  /* bubbled to the 'top' state? */

            QS_BEGIN_(QS_QEP_IGNORED, QS_priv_.smObjFilter, me)
                QS_TIME_();                                   /* time stamp */
                QS_SIG_(e->sig);                 /* the signal of the event */
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(s->stateHandler);              /* the current state */
            QS_END_()

        }
        else {
            /* empty */
        }
    }
#endif
}

/*..........................................................................*/
static void QMsm_tran_(QHsm * const me) {
    QActionHandler const *a = me->temp.act;
    QState r = (QState)Q_RET_TRAN;
#ifdef Q_SPY
    QMState const *t = me->state.obj;           /* target of the transition */
#endif
    QS_CRIT_STAT_

    for (; *a != Q_ACTION_CAST(0); QEP_ACT_PTR_INC_(a)) {
        r = (*(*a))(me);         /* call the action through the 'a' pointer */
#ifdef Q_SPY
        if (r == (QState)Q_RET_EXIT) {
            QS_BEGIN_(QS_QEP_STATE_EXIT, QS_priv_.smObjFilter, me)
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(me->temp.obj->stateHandler);        /* exited state */
            QS_END_()
        }
        else if (r == (QState)Q_RET_ENTRY) {
            QS_BEGIN_(QS_QEP_STATE_ENTRY, QS_priv_.smObjFilter, me)
                QS_OBJ_(me);
                QS_FUN_(me->temp.obj->stateHandler);       /* entered state */
            QS_END_()
        }
        else {
            /* empty */
        }
#endif
    }

    while (r == (QState)Q_RET_INITIAL) {

        QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.smObjFilter, me)
            QS_OBJ_(me);                       /* this state machine object */
            QS_FUN_(t->stateHandler);    /* target (source of initial tran) */
            QS_FUN_(me->state.obj->stateHandler);     /* the target handler */
        QS_END_()

#ifdef Q_SPY
        t = me->state.obj;            /* store the target of the transition */
#endif
        r = (QState)0U;                      /* invalidate the return value */
        for (a = me->temp.act;
             *a != Q_ACTION_CAST(0);
             QEP_ACT_PTR_INC_(a))
        {
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
}
