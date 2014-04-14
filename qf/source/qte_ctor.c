/**
* \file
* \ingroup qf
* \brief QTimeEvt_ctorX() implementation.
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
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qte_ctor")

/****************************************************************************/
/**
* \description
* When creating a time event, you must commit it to a specific active object
* \a act, tick rate \a tickRate and event signal \a sig. You cannot change
* these attributes later.
*
* \arguments
* \arg[in,out] \c me   pointer (see \ref derivation)
* \arg[in]     \c act  pointer to the active object associated with this
*              time event. The time event will post itself to this AO.
* \arg[in]     \c sig  signal to associate with this time event.
* \arg[in]     \c tickRate system tick rate to associate with this time event.
*
* \note You should call the constructor exactly once for every Time Event
* object __before__ arming the Time Event. The ideal place for initializing
* the time event(s) associated with a given AO is the AO's constructor.
*/
void QTimeEvt_ctorX(QTimeEvt * const me, QActive * const act,
                    enum_t const sig, uint_fast8_t tickRate)
{
    /** \pre The signal must be valid and the tick rate in range */
    Q_REQUIRE_ID(100, (sig >= (enum_t)Q_USER_SIG)
        && (tickRate < (uint_fast8_t)QF_MAX_TICK_RATE));

    me->next      = (QTimeEvt *)0;
    me->ctr       = (QTimeEvtCtr)0;
    me->interval  = (QTimeEvtCtr)0;
    me->super.sig = (QSignal)sig;

    /* For backwards compatibility with QTimeEvt_ctor(), the active object
    * pointer can be uninitialized (NULL) and is NOT validated in the
    * precondition. The active object pointer is validated in preconditions
    * to QTimeEvt_arm_() and QTimeEvt_rearm().
    */
    me->act       = act;

    /* Setting the POOL_ID event attribute to zero is correct only for
    * events not allocated from event pools, which must be the case
    * for Time Events.
    */
    me->super.poolId_ = (uint8_t)0;


    /* The reference counter attribute is not used in static events,
    * so for the Time Events it is reused to hold the tickRate in the
    * bits [0..6] and the linkedFlag in the MSB (bit [7]). The linkedFlag
    * is 0 for time events unlinked from any list and 1 otherwise.
    */
    me->super.refCtr_ = (uint8_t)tickRate;
}
