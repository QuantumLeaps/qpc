/*****************************************************************************
* Product: QF/C
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
#include "qf_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qf_new")

/**
* \file
* \ingroup qf
* \brief QF_new_() implementation.
*/

#ifdef Q_EVT_CTOR            /* Provide the constructor for the QEvt class? */

/*..........................................................................*/
QEvt *QEvt_ctor(QEvt * const me, enum_t const sig) {
    Q_REQUIRE(me != (QEvt *)0);             /* the me pointer must be valid */
    me->sig = (QSignal)sig;
    return me;
}

#endif

/*..........................................................................*/
QEvt *QF_newX_(uint_t const evtSize,
               uint_t const margin, enum_t const sig)
{
    QEvt *e;
    uint_t idx;
    QS_CRIT_STAT_

              /* find the pool index that fits the requested event size ... */
    for (idx = (uint_t)0; idx < QF_maxPool_; ++idx) {
        if (evtSize <= QF_EPOOL_EVENT_SIZE_(QF_pool_[idx])) {
            break;
        }
    }
    Q_ASSERT(idx < QF_maxPool_);      /* cannot run out of registered pools */

    QS_BEGIN_(QS_QF_NEW, (void *)0, (void *)0)
        QS_TIME_();                                            /* timestamp */
        QS_EVS_((QEvtSize)evtSize);                /* the size of the event */
        QS_SIG_((QSignal)sig);                   /* the signal of the event */
    QS_END_()

    QF_EPOOL_GET_(QF_pool_[idx], e, margin); /* get e -- platform-dependent */

    if (e != (QEvt *)0) {                     /* was e allocated correctly? */
        e->sig = (QSignal)sig;                 /* set signal for this event */
        e->poolId_ = (uint8_t)(idx + (uint_t)1);       /* store the pool ID */
        e->refCtr_ = (uint8_t)0;          /* set the reference counter to 0 */
    }
    else {                                     /* event cannot be allocated */
        Q_ASSERT(margin != (uint_t)0);     /* must tollerate bad allocation */
    }
    return e;         /* can't be NULL if we can't tollerate bad allocation */
}
