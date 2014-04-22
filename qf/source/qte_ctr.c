/**
* \file
* \ingroup qf
* \brief QTimeEvt_ctr() implementation.
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-02-17
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
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

/****************************************************************************/
/**
* \description
* Useful for checking how many clock ticks (at the tick rate associated
* with the time event) remain until the time event expires.
*
* \arguments
* \arg[in,out] \c me   pointer (see \ref derivation)
*
* \returns For an armed time event, the function returns the current value
* of the down-counter of the given time event. If the time event is not
* armed, the function returns 0.
*
* /note The function is thread-safe.
*/
QTimeEvtCtr QTimeEvt_ctr(QTimeEvt const * const me) {
    QTimeEvtCtr ret;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();
    ret = me->ctr;

    QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_CTR, QS_priv_.teObjFilter, me)
        QS_TIME_();              /* timestamp */
        QS_OBJ_(me);             /* this time event object */
        QS_OBJ_(me->act);        /* the target AO */
        QS_TEC_(ret);            /* the current counter */
        QS_TEC_(me->interval);   /* the interval */
        QS_U8_((uint8_t)(me->super.refCtr_ & (uint8_t)0x7F)); /* tick rate */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
    return ret;
}
