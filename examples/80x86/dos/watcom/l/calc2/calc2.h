/*****************************************************************************
* Product: Calculator Example with inheritance of whole state model
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
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
#ifndef calc2_h
#define calc2_h

#include "calc1.h"

enum Calc2Signals {
    PERCENT_SIG = MAX_CALC1_SIG,
    MAX_CALC2_SIG
};

typedef struct Calc2Tag {                            /* Calc2 state machine */
    Calc1 super;                                       /* derive from Calc1 */
} Calc2;

void Calc2_ctor(Calc2 *me);                                         /* ctor */

QState Calc2_operand2(Calc2 *me, QEvt const *e);
extern QStateHandler Calc2_state_operand2;

#endif                                                           /* calc2_h */
