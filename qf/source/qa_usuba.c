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

Q_DEFINE_THIS_MODULE("qa_usuba")

/**
* \file
* \ingroup qf
* \brief QActive_unsubscribeAll() definition.
*/

/*..........................................................................*/
void QActive_unsubscribeAll(QActive const * const me) {
    uint8_t p = me->prio;
    uint8_t i;
    enum_t sig;

    Q_REQUIRE(((uint8_t)0 < p) && (p <= (uint8_t)QF_MAX_ACTIVE)
              && (QF_active_[p] == me));

    i = QF_div8Lkup[p];
    for (sig = (enum_t)Q_USER_SIG; sig < QF_maxSignal_; ++sig) {
        QF_CRIT_STAT_
        QF_CRIT_ENTRY_();
        if ((QF_PTR_AT_(QF_subscrList_, sig).bits[i]
             & Q_ROM_BYTE(QF_pwr2Lkup[p])) != (uint8_t)0)
        {

            QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_UNSUBSCRIBE, QS_aoObj_, me)
                QS_TIME_();                                    /* timestamp */
                QS_SIG_((QSignal)sig);          /* the signal of this event */
                QS_OBJ_(me);                          /* this active object */
            QS_END_NOCRIT_()
                                                  /* clear the priority bit */
            QF_PTR_AT_(QF_subscrList_, sig).bits[i] &=
                Q_ROM_BYTE(QF_invPwr2Lkup[p]);
        }
        QF_CRIT_EXIT_();
    }
}
