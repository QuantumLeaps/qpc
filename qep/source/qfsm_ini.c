/**
* \file
* \brief QFsm_ctor() and QFsm_init_() definitions
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

Q_DEFINE_THIS_MODULE("qfsm_ini")

/****************************************************************************/
/**
* \description
* Performs the first step of FSM initialization by assigning the initial
* pseudostate to the currently active state of the state machine.
*
* \arguments
* \arg[in,out] \c me      pointer (see \ref derivation)
* \arg[in]     \c initial pointer to the top-most initial state-handler
*                         function in the derived state machine
*
* \note Must be called only by the constructors of the derived state
* machines.
*
* \note Must be called only ONCE before QMSM_INIT().
*
* \note
* QFsm inherits QMsm, so by the \ref derivation convention it should call the
* constructor of the superclass, i.e., QMsm_ctor(). However, this would pull
* in the QMsmVtbl, which in turn will pull in the code for QMsm_init_() and
* QMsm_dispatch_() implemetations. To avoid this code size penalty, in case
* ::QMsm is not used in a given project, the QFsm_ctor() performs direct
* intitialization of the Vtbl, which avoids pulling in the code for QMsm.
*
* \usage
* The following example illustrates how to invoke QFsm_ctor() in the
* "constructor" of a derived state machine:
* \include qep_qfsm_ctor.c
*/
void QFsm_ctor(QFsm * const me, QStateHandler initial) {
    static QMsmVtbl const vtbl = { /* QFsm virtual table */
        &QFsm_init_,
        &QFsm_dispatch_
    };
    /* do not call the QMsm_ctor() here, see the note */
    me->vptr = &vtbl; /* hook the vptr to the QFsm virtual table */
    me->state.fun = Q_STATE_CAST(0);
    me->temp.fun  = initial;
}

/****************************************************************************/
/**
* \description
* Executes the top-most initial transition in a FSM.
*
* \arguments
* \arg[in,out] \c me pointer (see \ref derivation)
* \arg[in]     \c e  pointer to the initialization event (might be NULL)
*
* \note Must be called only __once__ after the QFsm_ctor() and before
* QFsm_dispatch_().
*/
void QFsm_init_(QFsm * const me, QEvt const * const e) {
    QS_CRIT_STAT_

    /** \pre the virtual pointer must be initialized, the top-most initial
    * transition must be initialized, and the initial transition must not
    * be taken yet.
    */
    Q_REQUIRE_ID(200, (me->vptr != (QMsmVtbl const *)0)
                      && (me->temp.fun != Q_STATE_CAST(0))
                      && (me->state.fun == Q_STATE_CAST(0)));

    /* execute the top-most initial transition, which must be taken */
    Q_ALLEGE_ID(210, (*me->temp.fun)(me, e) == (QState)Q_RET_TRAN);

    (void)QEP_TRIG_(me->temp.fun, Q_ENTRY_SIG); /* enter the target */
    me->state.fun = me->temp.fun; /* record the new active state */

    QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.smObjFilter, me)
        QS_OBJ_(me);              /* this state machine object */
        QS_FUN_(Q_STATE_CAST(0)); /* source state (not defined for a FSM) */
        QS_FUN_(me->state.fun);   /* the target of the transition */
    QS_END_()
}
