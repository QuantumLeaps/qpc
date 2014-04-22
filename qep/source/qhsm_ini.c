/**
* \file
* \brief QHsm_ctor() and QHsm_init_() definitions
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

Q_DEFINE_THIS_MODULE("qhsm_ini")

/****************************************************************************/
/**
* \description
* Performs the first step of HSM initialization by assigning the initial
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
* QHsm inherits QMsm, so by the \ref derivation convention it should call the
* constructor of the superclass, i.e., QMsm_ctor(). However, this would pull
* in the QMsmVtbl, which in turn will pull in the code for QMsm_init_() and
* QMsm_dispatch_() implemetations. To avoid this code size penalty, in case
* ::QMsm is not used in a given project, the QHsm_ctor() performs direct
* intitialization of the Vtbl, which avoids pulling in the code for QMsm.
*
* \usage
* The following example illustrates how to invoke QHsm_ctor() in the
* "constructor" of a derived state machine:
* \include qep_qhsm_ctor.c
*/
void QHsm_ctor(QHsm * const me, QStateHandler initial) {
    static QMsmVtbl const vtbl = { /* QHsm virtual table */
        &QHsm_init_,
        &QHsm_dispatch_
    };
    /* do not call the QMsm_ctor() here */
    me->vptr  = &vtbl;
    me->state.fun = Q_STATE_CAST(&QHsm_top);
    me->temp.fun  = initial;
}

/****************************************************************************/
/**
* \description
* Executes the top-most initial transition in a HSM.
*
* \arguments
* \arg[in,out] \c me pointer (see \ref derivation)
* \arg[in]     \c e  pointer to the initialization event (might be NULL)
*
* \note Must be called only ONCE after the QHsm_ctor().
*/
void QHsm_init_(QHsm * const me, QEvt const * const e) {
    QStateHandler t = me->state.fun;
    QState r;
    QS_CRIT_STAT_

    /** \pre the virtual pointer must be initialized, the top-most initial
    * transition must be initialized, and the initial transition must not
    * be taken yet.
    */
    Q_REQUIRE_ID(200, (me->vptr != (QMsmVtbl const *)0)
                      && (me->temp.fun != Q_STATE_CAST(0))
                      && (t == Q_STATE_CAST(&QHsm_top)));

    r = (*me->temp.fun)(me, e); /* execute the top-most initial transition */

    /* the top-most initial transition must be taken */
    Q_ASSERT_ID(210, r == (QState)Q_RET_TRAN);

    QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.smObjFilter, me)
        QS_OBJ_(me);           /* this state machine object */
        QS_FUN_(t);            /* the source state */
        QS_FUN_(me->temp.fun); /* the target of the initial transition */
    QS_END_()

    /* drill down into the state hierarchy with initial transitions... */
    do {
        QStateHandler path[QHSM_MAX_NEST_DEPTH_];
        int_fast8_t ip = (int_fast8_t)0; /* transition entry path index */

        path[0] = me->temp.fun;
        (void)QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);
        while (me->temp.fun != t) {
            ++ip;
            Q_ASSERT_ID(220, ip < (int_fast8_t)Q_DIM(path));
            path[ip] = me->temp.fun;
            (void)QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);
        }
        me->temp.fun = path[0];

        /* retrace the entry path in reverse (desired) order... */
        do {
            QEP_ENTER_(path[ip]); /* enter path[ip] */
            --ip;
        } while (ip >= (int_fast8_t)0);

        t = path[0]; /* current state becomes the new source */

        r = QEP_TRIG_(t, Q_INIT_SIG); /* execute initial transition */

#ifdef Q_SPY
        if (r == (QState)Q_RET_TRAN) {
            QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.smObjFilter, me)
                QS_OBJ_(me);           /* this state machine object */
                QS_FUN_(t);            /* the source state */
                QS_FUN_(me->temp.fun); /* the target of the initial tran. */
            QS_END_()
        }
#endif /* Q_SPY */

    } while (r == (QState)Q_RET_TRAN);

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS_priv_.smObjFilter, me)
        QS_TIME_();    /* time stamp */
        QS_OBJ_(me);   /* this state machine object */
        QS_FUN_(t);    /* the new active state */
    QS_END_()

    me->state.fun = t; /* change the current active state */
    me->temp.fun  = t; /* mark the configuration as stable */
}

