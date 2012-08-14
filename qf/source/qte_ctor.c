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

Q_DEFINE_THIS_MODULE("qte_ctor")

/**
* \file
* \ingroup qf
* \brief QTimeEvt_ctor() implementation.
*/

/*..........................................................................*/
void QTimeEvt_ctor(QTimeEvt * const me, enum_t const sig) {
    Q_REQUIRE(sig >= (enum_t)Q_USER_SIG);                   /* valid signal */
    me->next = (QTimeEvt *)0;
    me->act  = (QActive *)0;
    me->ctr  = (QTimeEvtCtr)0;
    me->interval  = (QTimeEvtCtr)0;
    me->super.sig = (QSignal)sig;

                                   /* time event must be static, see NOTE01 */
    QF_EVT_POOL_ID_(&me->super) = (uint8_t)0;    /* not from any event pool */
    QF_EVT_REF_CTR_(&me->super) = (uint8_t)0;                 /* not linked */
}

/*****************************************************************************
* NOTE01:
* Setting the POOL_ID event attribute to zero is correct only for events not
* allocated from event pools. In the future releases of QF, time events
* actually could be allocated dynamically. However, for simplicity in this
* release of QF, time events are limited to be statically allocated.
*/
