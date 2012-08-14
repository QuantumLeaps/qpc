/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jul 25, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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

QEvt *QEvt_ctor(QEvt * const me, enum_t const sig) {
    me->sig = (QSignal)sig;
    return me;
}

/*..........................................................................*/
QEvt *QF_new_(QEvtSize const evtSize)

#else

QEvt *QF_new_(QEvtSize const evtSize, enum_t const sig)

#endif
{
    QEvt *e;
    QS_CRIT_STAT_

              /* find the pool index that fits the requested event size ... */
    uint8_t idx = (uint8_t)0;
    while (evtSize > QF_EPOOL_EVENT_SIZE_(QF_pool_[idx])) {
        ++idx;
        Q_ASSERT(idx < QF_maxPool_);  /* cannot run out of registered pools */
    }

    QS_BEGIN_(QS_QF_NEW, (void *)0, (void *)0)
        QS_TIME_();                                            /* timestamp */
        QS_EVS_(evtSize);                          /* the size of the event */
#ifndef Q_EVT_CTOR
        QS_SIG_((QSignal)sig);                   /* the signal of the event */
#else
        QS_SIG_(0);
#endif
    QS_END_()

    QF_EPOOL_GET_(QF_pool_[idx], e);         /* get e -- platform-dependent */
    Q_ASSERT(e != (QEvt *)0);            /* pool must not run out of events */

#ifndef Q_EVT_CTOR
    e->sig = (QSignal)sig;                     /* set signal for this event */
#endif

    QF_EVT_POOL_ID_(e) = (uint8_t)(idx + (uint8_t)1);  /* store the pool ID */
    QF_EVT_REF_CTR_(e) = (uint8_t)0;      /* set the reference counter to 0 */

    return e;
}
