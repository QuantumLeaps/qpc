/*****************************************************************************
* Product:  Calculator Example with percentage calculations
* Last Updated for Version: 4.5.04
* Date of the Last Update:  Jan 07, 2013
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
#include "qp_port.h"                        /* the port of the QP framework */
#include "bsp.h"                                   /* board support package */
#include "calc.h"

typedef struct CalcTag {
    QHsm super;                                        /* derives from QHsm */
    double operand1;                              /* the value of operand 1 */
    uint8_t operator;                               /* operator key entered */
} Calc;

static QState Calc_initial  (Calc * const me, QEvt const * const e);
static QState Calc_on       (Calc * const me, QEvt const * const e);
static QState Calc_error    (Calc * const me, QEvt const * const e);
static QState Calc_ready    (Calc * const me, QEvt const * const e);
static QState Calc_result   (Calc * const me, QEvt const * const e);
static QState Calc_begin    (Calc * const me, QEvt const * const e);
static QState Calc_negated1 (Calc * const me, QEvt const * const e);
static QState Calc_operand1 (Calc * const me, QEvt const * const e);
static QState Calc_zero1    (Calc * const me, QEvt const * const e);
static QState Calc_int1     (Calc * const me, QEvt const * const e);
static QState Calc_frac1    (Calc * const me, QEvt const * const e);
static QState Calc_opEntered(Calc * const me, QEvt const * const e);
static QState Calc_negated2 (Calc * const me, QEvt const * const e);
static QState Calc_operand2 (Calc * const me, QEvt const * const e);
static QState Calc_zero2    (Calc * const me, QEvt const * const e);
static QState Calc_int2     (Calc * const me, QEvt const * const e);
static QState Calc_frac2    (Calc * const me, QEvt const * const e);
static QState Calc_final    (Calc * const me, QEvt const * const e);


/* Local objects -----------------------------------------------------------*/
static Calc l_calc;                              /* the calculator instance */

/* Global objects ----------------------------------------------------------*/
QHsm * const the_calc = (QHsm *)&l_calc;

/*..........................................................................*/
void Calc_ctor(void) {
    Calc *me = &l_calc;
    QHsm_ctor(&me->super, Q_STATE_CAST(&Calc_initial));
}
/* HSM definition ----------------------------------------------------------*/
QState Calc_initial(Calc * const me, QEvt const * const e) {
    (void)e;                  /* avoid the "unreferenced parameter" warning */
    BSP_clear();
    return Q_TRAN(&Calc_on);
}
/*..........................................................................*/
QState Calc_on(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("on-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("on-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            BSP_message("on-INIT;");
            status = Q_TRAN(&Calc_ready);
            break;
        }
        case C_SIG: {
            BSP_clear();
            status = Q_TRAN(&Calc_on);                  /* transition-to-self */
            break;
        }
        case OFF_SIG: {
            status = Q_TRAN(&Calc_final);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_error(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("error-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("error-EXIT;");
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&Calc_on);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_ready(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("ready-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("ready-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            BSP_message("ready-INIT;");
            status = Q_TRAN(&Calc_begin);
            break;
        }
        case DIGIT_0_SIG: {
            BSP_clear();
            status = Q_TRAN(&Calc_zero1);
            break;
        }
        case DIGIT_1_9_SIG: {
            BSP_clear();
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_int1);
            break;
        }
        case POINT_SIG: {
            BSP_clear();
            BSP_insert((int)'0');
            BSP_insert((int)'.');
            status = Q_TRAN(&Calc_frac1);
            break;
        }
        case OPER_SIG: {
            me->operand1 = BSP_get_value();
            me->operator = ((CalcEvt const *)e)->key_code;
            status = Q_TRAN(&Calc_opEntered);
            break;
        }
        default: {
            status = Q_SUPER(&Calc_on);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_result(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("result-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("result-EXIT;");
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&Calc_ready);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_begin(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("begin-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("begin-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case OPER_SIG: {
            if (((CalcEvt const *)e)->key_code == KEY_MINUS) {
                status = Q_TRAN(&Calc_negated1);
            }
            else {
                status = Q_UNHANDLED();
            }
            break;
        }
        default: {
            status = Q_SUPER(&Calc_ready);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_negated1(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("negated1-ENTRY;");
            BSP_negate();
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("negated1-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case OPER_SIG: {
            if (((CalcEvt const *)e)->key_code == KEY_MINUS) {
                ;                                      /* explicitly ignore */
                status = Q_HANDLED();                      /* event handled */
            }
            else {
                status = Q_HANDLED();
            }
            break;
        }
        case CE_SIG: {
            BSP_clear();
            status = Q_TRAN(&Calc_begin);
            break;
        }
        case DIGIT_0_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_zero1);
            break;
        }
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_int1);
            break;
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_frac1);
            break;
        }
        default: {
            status = Q_SUPER(&Calc_on);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_negated2(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("negated2-ENTRY;");
            BSP_negate();
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("negated2-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case OPER_SIG: {
            if (((CalcEvt const *)e)->key_code == KEY_MINUS) {
                ;                                      /* explicitly ignore */
                status = Q_HANDLED();                      /* event handled */
            }
            else {
                status = Q_HANDLED();
            }
            break;
        }
        case CE_SIG: {
            BSP_clear();
            status = Q_TRAN(&Calc_opEntered);
            break;
        }
        case DIGIT_0_SIG: {
            status = Q_TRAN(&Calc_zero2);
            break;
        }
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_int2);
            break;
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_frac2);
            break;
        }
        default: {
            status = Q_SUPER(&Calc_on);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_operand1(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("operand1-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("operand1-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case CE_SIG: {
            BSP_clear();
            status = Q_TRAN(&Calc_begin);
            break;
        }
        case OPER_SIG: {
            me->operand1 = BSP_get_value();
            me->operator = ((CalcEvt const *)e)->key_code;
            status = Q_TRAN(&Calc_opEntered);
            break;
        }
        case EQUALS_SIG: {
            status = Q_TRAN(&Calc_result);
            break;
        }
        default: {
            status = Q_SUPER(&Calc_on);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_zero1(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("zero1-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("zero1-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case DIGIT_0_SIG: {
            ;                                          /* explicitly ignore */
            status = Q_HANDLED();
            break;
        }
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_int1);
            break;
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_frac1);
            break;
        }
        default: {
            status = Q_SUPER(&Calc_operand1);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_int1(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("int1-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("int1-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case DIGIT_0_SIG:                     /* intentionally fall through */
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_HANDLED();
            break;
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_frac1);
            break;
        }
        default: {
            status = Q_SUPER(&Calc_operand1);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_frac1(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("frac1-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("frac1-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case DIGIT_0_SIG:                     /* intentionally fall through */
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_HANDLED();
            break;
        }
        case POINT_SIG: {
            ;                                          /* explicitly ignore */
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&Calc_operand1);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_opEntered(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("opEntered-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("opEntered-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case OPER_SIG: {
            if (((CalcEvt const *)e)->key_code == KEY_MINUS) {
                BSP_clear();
                status = Q_TRAN(&Calc_negated2);
            }
            break;
        }
        case DIGIT_0_SIG: {
            BSP_clear();
            status = Q_TRAN(&Calc_zero2);
            break;
        }
        case DIGIT_1_9_SIG: {
            BSP_clear();
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_int2);
            break;
        }
        case POINT_SIG: {
            BSP_clear();
            BSP_insert((int)'0');
            BSP_insert((int)'.');
            status = Q_TRAN(&Calc_frac2);
            break;
        }
        default: {
            status = Q_SUPER(&Calc_on);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_operand2(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("operand2-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("operand2-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case CE_SIG: {
            BSP_clear();
            status = Q_TRAN(&Calc_opEntered);
            break;
        }
        case OPER_SIG: {
            if (BSP_eval(me->operand1, me->operator, BSP_get_value())) {
                me->operand1 = BSP_get_value();
                me->operator = ((CalcEvt const *)e)->key_code;
                status = Q_TRAN(&Calc_opEntered);
            }
            else {
                status = Q_TRAN(&Calc_error);
            }
            break;
        }
        case EQUALS_SIG: {
            if (BSP_eval(me->operand1, me->operator, BSP_get_value())) {
                status = Q_TRAN(&Calc_result);
            }
            else {
                status = Q_TRAN(&Calc_error);
            }
            break;
        }
        case PERCENT_SIG: {
            double operand2 = 0.0;
            switch (me->operator) {
                case KEY_PLUS:
                case KEY_MINUS: {
                    operand2 = (me->operand1 * BSP_get_value())/100.0;
                    break;
                }
                case KEY_MULT:
                case KEY_DIVIDE: {
                    operand2 = BSP_get_value()/100.0;
                    break;
                }
            }

            if (BSP_eval(me->operand1, me->operator, operand2)) {
                status = Q_TRAN(&Calc_result);
            }
            else {
                status = Q_TRAN(&Calc_error);
            }
            break;
        }
        default: {
            status = Q_SUPER(&Calc_on);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_zero2(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("zero2-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("zero2-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case DIGIT_0_SIG: {
            ;                                          /* explicitly ignore */
            status = Q_HANDLED();
            break;
        }
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_int2);
            break;
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_frac2);
        }
        default: {
            status = Q_SUPER(&Calc_operand2);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_int2(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("int2-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("int2-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case DIGIT_0_SIG:                     /* intentionally fall through */
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_HANDLED();
            break;
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_TRAN(&Calc_frac2);
            break;
        }
        default: {
            status = Q_SUPER(&Calc_operand2);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_frac2(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("frac2-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            BSP_message("frac2-EXIT;");
            status = Q_HANDLED();
            break;
        }
        case DIGIT_0_SIG:                     /* intentionally fall through */
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            status = Q_HANDLED();
            break;
        }
        case POINT_SIG: {
            ;                                          /* explicitly ignore */
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&Calc_operand2);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Calc_final(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("final-ENTRY;");
            BSP_exit();
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}

