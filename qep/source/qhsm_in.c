/*****************************************************************************
* Product: QEP/C
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 17, 2012
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
#include "qep_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qhsm_in")

/**
* \file
* \ingroup qep
* \brief QHsm_isIn() implementation.
*/

/*..........................................................................*/
uint8_t QHsm_isIn(QHsm * const me, QStateHandler const state) {
    uint8_t inState = (uint8_t)0; /* assume that this HSM is not in 'state' */
    QState r;

    Q_REQUIRE(me->temp == me->state); /* state configuration must be stable */

    do {
        if (me->temp == state) {                    /* do the states match? */
            inState = (uint8_t)1;               /* match found, return TRUE */
            r = Q_RET_IGNORED;                     /* break out of the loop */
        }
        else {
            r = QEP_TRIG_(me->temp, QEP_EMPTY_SIG_);
        }
    } while (r != Q_RET_IGNORED);             /* QHsm_top state not reached */
    me->temp = me->state;         /* restore the stable state configuration */

    return inState;                                    /* return the status */
}
