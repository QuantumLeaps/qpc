/**
* \file
* \brief QMsm_ctor() and QMsm_init_() definitions
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
* Web:   http://www.state-machine.com
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

Q_DEFINE_THIS_MODULE("qmsm_ini")

/****************************************************************************/
static QMState const l_msm_top_s = {
    (QMState const *)0,
    Q_STATE_CAST(0),
    Q_ACTION_CAST(0),
    Q_ACTION_CAST(0),
    Q_ACTION_CAST(0)
};

/****************************************************************************/
/**
* \description
* Performs the first step of QMsm initialization by assigning the initial
* pseudostate to the currently active state of the state machine.
*
* \arguments
* \arg[in,out] \c me       pointer (see \ref derivation)
* \arg[in]     \c initial  the top-most initial transition for the MSM.
*
* \note  Must be called only ONCE before QMSM_INIT().
*
* \usage
* The following example illustrates how to invoke QMsm_ctor() in the
* "constructor" of a derived state machine:
* \include qep_qmsm_ctor.c
*/
void QMsm_ctor(QMsm * const me, QStateHandler initial) {
    static QMsmVtbl const vtbl = { /* QMsm virtual table */
        &QMsm_init_,
        &QMsm_dispatch_
    };
    me->vptr = &vtbl;
    me->state.obj = &l_msm_top_s; /* the current state (top) */
    me->temp.fun  = initial;      /* the initial transition handler */
}

/****************************************************************************/
/**
* \description
* Executes the top-most initial transition in a MSM.
*
* \arguments
* \arg[in,out] \c me pointer (see \ref derivation)
* \arg[in]     \c e  pointer to the initialization event (might be NULL)
*
* \note Must be called only ONCE after the QMsm_ctor().
*/
void QMsm_init_(QMsm * const me, QEvt const * const e) {
    QState r;
    QS_CRIT_STAT_

    /** \pre the virtual pointer must be initialized, the top-most initial
    * transition must be initialized, and the initial transition must not
    * be taken yet.
    */
    Q_REQUIRE_ID(200, (me->vptr != (QMsmVtbl const *)0)
                      && (me->temp.fun != Q_STATE_CAST(0))
                      && (me->state.obj == &l_msm_top_s));

    r = (*me->temp.fun)(me, e); /* the action of the top-most initial tran. */

    /* the top-most initial transition must be taken */
    Q_ASSERT_ID(210, r == (QState)Q_RET_TRAN_INIT);

    QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.smObjFilter, me)
        QS_OBJ_(me); /* this state machine object */
        QS_FUN_(me->state.obj->stateHandler);        /* source state handler*/
        QS_FUN_(me->temp.tatbl->target->stateHandler);/*target state handler*/
    QS_END_()

    /* set state to the last tran. target */
    me->state.obj = me->temp.tatbl->target;

    /* drill down into the state hierarchy with initial transitions... */
    do {
        r = QMsm_execTatbl_(me, me->temp.tatbl); /* execute the tran. table */
    } while (r >= (QState)Q_RET_TRAN_INIT);

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS_priv_.smObjFilter, me)
        QS_TIME_();                           /* time stamp */
        QS_OBJ_(me);                          /* this state machine object */
        QS_FUN_(me->state.obj->stateHandler); /* the new current state */
    QS_END_()
}
