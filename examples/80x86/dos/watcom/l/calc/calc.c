/*****************************************************************************
* Product:  Calculator Example
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
#include "qp_port.h"                        /* the port of the QP framework */
#include "bsp.h"                                   /* board support package */
#include "calc.h"

typedef struct CalcTag {
    QHsm super;                                        /* derives from QHsm */
    double operand1;                              /* the value of operand 1 */
    uint8_t operator;                               /* operator key entered */
} Calc;

static QState Calc_initial  (Calc *me, QEvt const *e);/*init. pseudostate */
static QState Calc_on       (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_error    (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_ready    (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_result   (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_begin    (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_negated1 (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_operand1 (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_zero1    (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_int1     (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_frac1    (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_opEntered(Calc *me, QEvt const *e);   /* state handler */
static QState Calc_negated2 (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_operand2 (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_zero2    (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_int2     (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_frac2    (Calc *me, QEvt const *e);   /* state handler */
static QState Calc_final    (Calc *me, QEvt const *e);   /* state handler */


/* Local objects -----------------------------------------------------------*/
static Calc l_calc;                              /* the calculator instance */

/* Global objects ----------------------------------------------------------*/
QHsm * const the_calc = (QHsm *)&l_calc;

/*..........................................................................*/
void Calc_ctor(void) {
    Calc *me = &l_calc;
    QHsm_ctor(&me->super, (QStateHandler)&Calc_initial);
}
/* HSM definition ----------------------------------------------------------*/
QState Calc_initial(Calc *me, QEvt const *e) {
    (void)e;                  /* avoid the "unreferenced parameter" warning */
    BSP_clear();
    return Q_TRAN(&Calc_on);
}
/*..........................................................................*/
QState Calc_on(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("on-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("on-EXIT;");
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            BSP_message("on-INIT;");
            return Q_TRAN(&Calc_ready);
        }
        case C_SIG: {
            BSP_clear();
            return Q_TRAN(&Calc_on);                  /* transition-to-self */
        }
        case OFF_SIG: {
            return Q_TRAN(&Calc_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState Calc_error(Calc *me, QEvt const *e) {
    (void)me;          /* avoid the compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("error-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("error-EXIT;");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&Calc_on);
}
/*..........................................................................*/
QState Calc_ready(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("ready-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("ready-EXIT;");
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            BSP_message("ready-INIT;");
            return Q_TRAN(&Calc_begin);
        }
        case DIGIT_0_SIG: {
            BSP_clear();
            return Q_TRAN(&Calc_zero1);
        }
        case DIGIT_1_9_SIG: {
            BSP_clear();
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_int1);
        }
        case POINT_SIG: {
            BSP_clear();
            BSP_insert((int)'0');
            BSP_insert((int)'.');
            return Q_TRAN(&Calc_frac1);
        }
        case OPER_SIG: {
            me->operand1 = BSP_get_value();
            me->operator = ((CalcEvt const *)e)->key_code;
            return Q_TRAN(&Calc_opEntered);
        }
    }
    return Q_SUPER(&Calc_on);
}
/*..........................................................................*/
QState Calc_result(Calc *me, QEvt const *e) {
    (void)me;               /* avoid compiler warning about unused argument */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("result-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("result-EXIT;");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&Calc_ready);
}
/*..........................................................................*/
QState Calc_begin(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("begin-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("begin-EXIT;");
            return Q_HANDLED();
        }
        case OPER_SIG: {
            if (((CalcEvt const *)e)->key_code == KEY_MINUS) {
                return Q_TRAN(&Calc_negated1);
            }
            break;
        }
    }
    return Q_SUPER(&Calc_ready);
}
/*..........................................................................*/
QState Calc_negated1(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("negated1-ENTRY;");
            BSP_negate();
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("negated1-EXIT;");
            return Q_HANDLED();
        }
        case OPER_SIG: {
            if (((CalcEvt const *)e)->key_code == KEY_MINUS) {
                ;                                      /* explicitly ignore */
                return Q_HANDLED();                        /* event handled */
            }
            break;
        }
        case CE_SIG: {
            BSP_clear();
            return Q_TRAN(&Calc_begin);
        }
        case DIGIT_0_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_zero1);
        }
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_int1);
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_frac1);
        }
    }
    return Q_SUPER(&Calc_on);
}
/*..........................................................................*/
QState Calc_negated2(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("negated2-ENTRY;");
            BSP_negate();
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("negated2-EXIT;");
            return Q_HANDLED();
        }
        case OPER_SIG: {
            if (((CalcEvt const *)e)->key_code == KEY_MINUS) {
                ;                                      /* explicitly ignore */
                return Q_HANDLED();                        /* event handled */
            }
            break;
        }
        case CE_SIG: {
            BSP_clear();
            return Q_TRAN(&Calc_opEntered);
        }
        case DIGIT_0_SIG: {
            return Q_TRAN(&Calc_zero2);
        }
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_int2);
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_frac2);
        }
    }
    return Q_SUPER(&Calc_on);
}
/*..........................................................................*/
QState Calc_operand1(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("operand1-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("operand1-EXIT;");
            return Q_HANDLED();
        }
        case CE_SIG: {
            BSP_clear();
            return Q_TRAN(&Calc_begin);
        }
        case OPER_SIG: {
            me->operand1 = BSP_get_value();
            me->operator = ((CalcEvt const *)e)->key_code;
            return Q_TRAN(&Calc_opEntered);
        }
        case EQUALS_SIG: {
            return Q_TRAN(&Calc_result);
        }
    }
    return Q_SUPER(&Calc_on);
}
/*..........................................................................*/
QState Calc_zero1(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("zero1-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("zero1-EXIT;");
            return Q_HANDLED();
        }
        case DIGIT_0_SIG: {
            ;                                          /* explicitly ignore */
            return Q_HANDLED();
        }
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_int1);
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_frac1);
        }
    }
    return Q_SUPER(&Calc_operand1);
}
/*..........................................................................*/
QState Calc_int1(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("int1-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("int1-EXIT;");
            return Q_HANDLED();
        }
        case DIGIT_0_SIG:                     /* intentionally fall through */
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_HANDLED();
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_frac1);
        }
    }
    return Q_SUPER(&Calc_operand1);
}
/*..........................................................................*/
QState Calc_frac1(Calc *me, QEvt const *e) {
    (void)me;               /* avoid compiler warning about unused argument */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("frac1-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("frac1-EXIT;");
            return Q_HANDLED();
        }
        case DIGIT_0_SIG:                     /* intentionally fall through */
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_HANDLED();
        }
        case POINT_SIG: {
            ;                                          /* explicitly ignore */
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&Calc_operand1);
}
/*..........................................................................*/
QState Calc_opEntered(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("opEntered-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("opEntered-EXIT;");
            return Q_HANDLED();
        }
        case OPER_SIG: {
            if (((CalcEvt const *)e)->key_code == KEY_MINUS) {
                BSP_clear();
                return Q_TRAN(&Calc_negated2);
            }
            break;
        }
        case DIGIT_0_SIG: {
            BSP_clear();
            return Q_TRAN(&Calc_zero2);
        }
        case DIGIT_1_9_SIG: {
            BSP_clear();
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_int2);
        }
        case POINT_SIG: {
            BSP_clear();
            BSP_insert((int)'0');
            BSP_insert((int)'.');
            return Q_TRAN(&Calc_frac2);
        }
    }
    return Q_SUPER(&Calc_on);
}
/*..........................................................................*/
QState Calc_operand2(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("operand2-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("operand2-EXIT;");
            return Q_HANDLED();
        }
        case CE_SIG: {
            BSP_clear();
            return Q_TRAN(&Calc_opEntered);
        }
        case OPER_SIG: {
            if (BSP_eval(me->operand1, me->operator, BSP_get_value())) {
                me->operand1 = BSP_get_value();
                me->operator = ((CalcEvt const *)e)->key_code;
                return Q_TRAN(&Calc_opEntered);
            }
            else {
                return Q_TRAN(&Calc_error);
            }
        }
        case EQUALS_SIG: {
            if (BSP_eval(me->operand1, me->operator, BSP_get_value())) {
                return Q_TRAN(&Calc_result);
            }
            else {
                return Q_TRAN(&Calc_error);
            }
        }
    }
    return Q_SUPER(&Calc_on);
}
/*..........................................................................*/
QState Calc_zero2(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("zero2-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("zero2-EXIT;");
            return Q_HANDLED();
        }
        case DIGIT_0_SIG: {
            ;                                          /* explicitly ignore */
            return Q_HANDLED();
        }
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_int2);
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_frac2);
        }
    }
    return Q_SUPER(&Calc_operand2);
}
/*..........................................................................*/
QState Calc_int2(Calc *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("int2-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("int2-EXIT;");
            return Q_HANDLED();
        }
        case DIGIT_0_SIG:                     /* intentionally fall through */
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_HANDLED();
        }
        case POINT_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_TRAN(&Calc_frac2);
        }
    }
    return Q_SUPER(&Calc_operand2);
}
/*..........................................................................*/
QState Calc_frac2(Calc *me, QEvt const *e) {
    (void)me;               /* avoid compiler warning about unused argument */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("frac2-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_message("frac2-EXIT;");
            return Q_HANDLED();
        }
        case DIGIT_0_SIG:                     /* intentionally fall through */
        case DIGIT_1_9_SIG: {
            BSP_insert(((CalcEvt const *)e)->key_code);
            return Q_HANDLED();
        }
        case POINT_SIG: {
            ;                                          /* explicitly ignore */
            return Q_HANDLED();
        }
   }
    return Q_SUPER(&Calc_operand2);
}
/*..........................................................................*/
QState Calc_final(Calc *me, QEvt const *e) {
    (void)me;               /* avoid compiler warning about unused argument */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_message("final-ENTRY;");
            BSP_exit();
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

