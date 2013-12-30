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

Q_DEFINE_THIS_MODULE("qhsm_ini")

/**
* \file
* \ingroup qep
* \brief QHsm_ctor() and QHsm_init_() implementation.
*/

/*..........................................................................*/
void QHsm_ctor(QHsm * const me, QStateHandler initial) {
    static QMsmVtbl const vtbl = {                    /* QHsm virtual table */
        &QHsm_init_,
        &QHsm_dispatch_
    };
    /* do not call the QMsm_ctor() here, see NOTE01 */
    me->vptr  = &vtbl;
    me->state.fun = Q_STATE_CAST(&QHsm_top);
    me->temp.fun  = initial;
}
/*..........................................................................*/
void QHsm_init_(QHsm * const me, QEvt const * const e) {
    QStateHandler t = me->state.fun;
    QS_CRIT_STAT_

    Q_REQUIRE((me->vptr != (QMsmVtbl const *)0)    /* ctor must be executed */
              && (me->temp.fun != Q_STATE_CAST(0)) /* ctor must be executed */
              && (t == Q_STATE_CAST(&QHsm_top))); /*initial tran. NOT taken */

                           /* the top-most initial transition must be taken */
    Q_ALLEGE((*me->temp.fun)(me, e) == (QState)Q_RET_TRAN);

    do {                                        /* drill into the target... */
        QStateHandler path[QEP_MAX_NEST_DEPTH_];
        int_t ip = (int_t)0;                 /* transition entry path index */

        QS_BEGIN_(QS_QEP_STATE_INIT, QS_priv_.smObjFilter, me)
            QS_OBJ_(me);                       /* this state machine object */
            QS_FUN_(t);                                 /* the source state */
            QS_FUN_(me->temp.fun);  /* the target of the initial transition */
        QS_END_()

        path[0] = me->temp.fun;
        (void)QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);
        while (me->temp.fun != t) {
            ++ip;
            path[ip] = me->temp.fun;
            (void)QEP_TRIG_(me->temp.fun, QEP_EMPTY_SIG_);
        }
        me->temp.fun = path[0];
                                            /* entry path must not overflow */
        Q_ASSERT(ip < (int_t)QEP_MAX_NEST_DEPTH_);

        do {        /* retrace the entry path in reverse (desired) order... */
            QEP_ENTER_(path[ip]);                         /* enter path[ip] */
            --ip;
        } while (ip >= (int_t)0);

        t = path[0];                /* current state becomes the new source */
    } while (QEP_TRIG_(t, Q_INIT_SIG) == (QState)Q_RET_TRAN);

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS_priv_.smObjFilter, me)
        QS_TIME_();                                           /* time stamp */
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(t);                                 /* the new active state */
    QS_END_()

    me->state.fun = t;                   /* change the current active state */
    me->temp.fun  = t;                  /* mark the configuration as stable */
}

/*****************************************************************************
* NOTE01:
* QHsm inherits QMsm, so by the "inheritance of structures" convention
* it should call the constructor of the superclass, i.e., QMsm_ctor().
* However, this would pull in the QMsmVtbl, which in turn will pull in
* the code for QMsm_init_() and QMsm_dispatch_() implemetations. To avoid
* this code size penalty, in case QMsm is not used in a given project,
* the QHsm_ctor() performs direct intitialization of the Vtbl, which avoids
* pulling in the code for QMsm.
*/


