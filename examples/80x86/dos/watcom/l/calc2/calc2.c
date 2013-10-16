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
#include "qp_port.h"
#include "bsp.h"                                   /* board support package */
#include "calc2.h"

#include <stdlib.h>

Q_DEFINE_THIS_FILE

/* state variables ---------------------------------------------------------*/
QStateHandler Calc2_state_operand2 = (QStateHandler)&Calc2_operand2;

/* Ctor definition ---------------------------------------------------------*/
void Calc2_ctor(Calc2 *me) {                                        /* ctor */
    Calc1_ctor(&me->super);                   /* instantiate the superclass */
                                      /* subsitute all overridden states... */
    Calc1_state_operand2 = Calc2_state_operand2;
}
/*..........................................................................*/
QState Calc2_operand2(Calc2 *me, QEvt const *e) {
    switch (e->sig) {
        case PERCENT_SIG: {
            double operand2 = BSP_get_value();
            switch (me->super.operator) {
                case KEY_PLUS:
                case KEY_MINUS: {
                    operand2 = me->super.operand1 * operand2 / 100.0;
                    break;
                }
                case KEY_MULT:
                case KEY_DIVIDE: {
                    operand2 /= 100.0;
                    break;
                }
                default: {
                    Q_ERROR();
                    break;
                }
            }
            if (BSP_eval(me->super.operand1, me->super.operator, operand2)) {
                return Q_TRAN(Calc1_state_result);
            }
            else {
                return Q_TRAN(Calc1_state_error);
            }
        }
    }
    return Q_SUPER(&Calc1_operand2);       /* let Calc1 handle other events */
}
