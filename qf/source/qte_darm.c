/**
* \file
* \ingroup qf
* \brief QTimeEvt_disarm() implementation.
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-04-10
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
* Disarm the time event so it can be safely reused.
*
* \arguments
* \arg[in,out] \c me     pointer (see \ref derivation)
*
* \returns 'true' if the time event was truly disarmed, that is, it
* was running. The return of 'false' means that the time event was
* not truly disarmed because it was not running. The 'false' return is only
* possible for one-shot time events that have been automatically disarmed
* upon expiration. In this case the 'false' return means that the time event
* has already been posted or published and should be expected in the
* active object's state machine.
*
* \note there is no harm in disarming an already disarmed time event
*/
bool QTimeEvt_disarm(QTimeEvt * const me) {
    bool wasArmed;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();

    /* is the time evt running? */
    if (me->ctr != (QTimeEvtCtr)0) {
        wasArmed = true;

        QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_DISARM, QS_priv_.teObjFilter, me)
            QS_TIME_();            /* timestamp */
            QS_OBJ_(me);           /* this time event object */
            QS_OBJ_(me->act);      /* the target AO */
            QS_TEC_(me->ctr);      /* the number of ticks */
            QS_TEC_(me->interval); /* the interval */
            QS_U8_((uint8_t)(me->super.refCtr_ & (uint8_t)0x7F));/*tick rate*/
        QS_END_NOCRIT_()

        me->ctr = (QTimeEvtCtr)0;  /* schedule removal from the list */
    }
    /* the time event was already not running */
    else {
        wasArmed = false;

        QS_BEGIN_NOCRIT_(QS_QF_TIMEEVT_DISARM_ATTEMPT,
                         QS_priv_.teObjFilter, me)
            QS_TIME_();            /* timestamp */
            QS_OBJ_(me);           /* this time event object */
            QS_OBJ_(me->act);      /* the target AO */
            QS_U8_((uint8_t)(me->super.refCtr_ & (uint8_t)0x7F));/*tick rate*/
        QS_END_NOCRIT_()

    }
    QF_CRIT_EXIT_();
    return wasArmed;
}
