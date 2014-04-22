/**
* \file
* \brief QFsm_dispatch_() definition
* \ingroup qep
* \cond
******************************************************************************
* Product: QEP/C
* Last updated for version 5.3.0
* Last updated on  2014-04-09
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* \endcond
*/
#include "qep_port.h"     /* QEP port */
#include "qep_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qfsm_dis")

/****************************************************************************/
/**
* \description
* Dispatches an event for processing to a non-hierarchical ("flat") state
* machine (FSM). The processing of an event represents one run-to-completion
* (RTC) step.
*
* \arguments
* \arg[in,out] \c me pointer (see \ref derivation)
* \arg[in]     \c e  pointer to the event to be dispatched to the FSM
*/
void QFsm_dispatch_(QFsm * const me, QEvt const * const e) {
    QState r;
    QS_CRIT_STAT_

    /** \pre the FSM must be in a stable state configuration */
    Q_REQUIRE_ID(100, me->state.fun == me->temp.fun);

    QS_BEGIN_(QS_QEP_DISPATCH, QS_priv_.smObjFilter, me)
        QS_TIME_();                 /* time stamp */
        QS_SIG_(e->sig);            /* the signal of the event */
        QS_OBJ_(me);                /* this state machine object */
        QS_FUN_(me->state.fun);     /* the current state */
    QS_END_()

    r = (*me->state.fun)(me, e);    /* call the event handler */

    /* transition taken? */
    if (r == (QState)Q_RET_TRAN) {

        QS_BEGIN_(QS_QEP_TRAN, QS_priv_.smObjFilter, me)
            QS_TIME_();             /* time stamp */
            QS_SIG_(e->sig);        /* the signal of the event */
            QS_OBJ_(me);            /* this state machine object */
            QS_FUN_(me->state.fun); /* the source of the transition */
            QS_FUN_(me->temp.fun);  /* the target of the transition */
        QS_END_()

        QEP_EXIT_(me->state.fun);   /* exit the source */
        QEP_ENTER_(me->temp.fun);   /* enter the target */
        me->state.fun = me->temp.fun; /* record the new active state */
    }
    else { /* transition not taken */
#ifdef Q_SPY

        if (r == (QState)Q_RET_UNHANDLED) {
            QS_BEGIN_(QS_QEP_UNHANDLED, QS_priv_.smObjFilter, me)
                QS_SIG_(e->sig);    /* the signal of the event */
                QS_OBJ_(me);        /* this state machine object */
                QS_FUN_(me->state.fun); /* the current state */
            QS_END_()
        }

        if (r == (QState)Q_RET_HANDLED) {

            QS_BEGIN_(QS_QEP_INTERN_TRAN, QS_priv_.smObjFilter, me)
                QS_TIME_();         /* time stamp */
                QS_SIG_(e->sig);    /* the signal of the event */
                QS_OBJ_(me);        /* this state machine object */
                QS_FUN_(me->state.fun); /* the current state */
            QS_END_()

        }
        else {

            QS_BEGIN_(QS_QEP_IGNORED, QS_priv_.smObjFilter, me)
                QS_TIME_();         /* time stamp */
                QS_SIG_(e->sig);    /* the signal of the event */
                QS_OBJ_(me);        /* this state machine object */
                QS_FUN_(me->state.fun); /* the current state */
            QS_END_()

        }
#endif /* Q_SPY */
    }
}
