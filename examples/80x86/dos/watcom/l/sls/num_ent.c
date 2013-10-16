/*****************************************************************************
* Product: Number data entry
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
#include "num_ent.h"
#include "ui.h"
#include "video.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static QState NumEntry_initial (NumEntry *me, QEvt const *e);
static QState NumEntry_top     (NumEntry *me, QEvt const *e);
static QState NumEntry_negative(NumEntry *me, QEvt const *e);
static QState NumEntry_zero    (NumEntry *me, QEvt const *e);
static QState NumEntry_integer (NumEntry *me, QEvt const *e);
static QState NumEntry_fraction(NumEntry *me, QEvt const *e);

/*..........................................................................*/
void NumEntry_ctor(NumEntry *me) {                                  /* ctor */
    QHsm_ctor(&me->super, (QStateHandler)&NumEntry_initial);
}
/*..........................................................................*/
void NumEntry_xtor(NumEntry *me) {                                  /* xtor */
    (void)me;          /* avoid the compiler warning about unused parameter */
}
/*..........................................................................*/
void NumEntry_config(NumEntry *me, uint8_t x, uint8_t y, uint8_t color) {
    me->x     = x;
    me->y     = y;
    me->color = color;
}
/*..........................................................................*/
void NumEntry_insert(NumEntry *me, uint8_t keyId) {
    if (me->len == 0) {
        me->str[NUM_STR_WIDTH - 1] = (char)keyId;
    }
    else if (me->len < (NUM_STR_WIDTH - 1)) {
        memmove(&me->str[0], &me->str[1], NUM_STR_WIDTH - 1);
        me->str[NUM_STR_WIDTH - 1] = (char)keyId;
    }
    ++me->len;
    Video_printStrAt(me->x, me->y, me->color, me->str);
}
/*..........................................................................*/
double NumEntry_get(NumEntry *me) {
    return strtod(me->str, (char **)0);
}

/* HSM definition ----------------------------------------------------------*/
QState NumEntry_initial(NumEntry *me, QEvt const *e) {
    (void)e;           /* avoid the compiler warning about unused parameter */

                      /* send functions dictionaries for NumEntry states... */
    QS_FUN_DICTIONARY(&NumEntry_top);
    QS_FUN_DICTIONARY(&NumEntry_negative);
    QS_FUN_DICTIONARY(&NumEntry_zero);
    QS_FUN_DICTIONARY(&NumEntry_integer);
    QS_FUN_DICTIONARY(&NumEntry_fraction);

            /* send signal dictionaries for signals specific to NumEntry... */
    QS_SIG_DICTIONARY(C_SIG,         me);
    QS_SIG_DICTIONARY(DIGIT_0_SIG,   me);
    QS_SIG_DICTIONARY(DIGIT_1_9_SIG, me);
    QS_SIG_DICTIONARY(POINT_SIG,     me);
    QS_SIG_DICTIONARY(NEG_SIG,       me);


    return Q_TRAN(&NumEntry_zero);
}
/*..........................................................................*/
QState NumEntry_top(NumEntry *me, QEvt const *e) {
    switch (e->sig) {
        case C_SIG: {
            memset(me->str, ' ', NUM_STR_WIDTH - 1);
            me->str[NUM_STR_WIDTH - 1] = '0';
            me->str[NUM_STR_WIDTH] = '\0';
            me->len = 0;
            Video_printStrAt(me->x, me->y, me->color, me->str);
            return Q_TRAN(&NumEntry_zero);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState NumEntry_zero(NumEntry *me, QEvt const *e) {
    switch (e->sig) {
        case DIGIT_0_SIG: {
            ;                                          /* explicitly ignore */
            return Q_HANDLED();
        }
        case NEG_SIG: {
            me->str[NUM_STR_WIDTH - 2] = '-';
            Video_printStrAt(me->x, me->y, me->color, me->str);
            return Q_TRAN(&NumEntry_negative);
        }
        case DIGIT_1_9_SIG: {
            NumEntry_insert(me, ((KeyboardEvt const *)e)->key_code);
            return Q_TRAN(&NumEntry_integer);
        }
        case POINT_SIG: {
            NumEntry_insert(me, ((KeyboardEvt const *)e)->key_code);
            return Q_TRAN(&NumEntry_fraction);
        }
    }
    return Q_SUPER(&NumEntry_top);
}
/*..........................................................................*/
QState NumEntry_negative(NumEntry *me, QEvt const *e) {
    switch (e->sig) {
        case DIGIT_0_SIG: {
            ;                                          /* explicitly ignore */
            return Q_HANDLED();
        }
        case DIGIT_1_9_SIG: {
            NumEntry_insert(me, ((KeyboardEvt const *)e)->key_code);
            return Q_TRAN(&NumEntry_integer);
        }
        case POINT_SIG: {
            NumEntry_insert(me, ((KeyboardEvt const *)e)->key_code);
            return Q_TRAN(&NumEntry_fraction);
        }
    }
    return Q_SUPER(&NumEntry_top);
}
/*..........................................................................*/
QState NumEntry_integer(NumEntry *me, QEvt const *e) {
    switch (e->sig) {
        case DIGIT_0_SIG:                     /* intentionally fall through */
        case DIGIT_1_9_SIG: {
            NumEntry_insert(me, ((KeyboardEvt const *)e)->key_code);
            return Q_HANDLED();
        }
        case POINT_SIG: {
            NumEntry_insert(me, ((KeyboardEvt const *)e)->key_code);
            return Q_TRAN(&NumEntry_fraction);
        }
    }
    return Q_SUPER(&NumEntry_top);
}
/*..........................................................................*/
QState NumEntry_fraction(NumEntry *me, QEvt const *e) {
    switch (e->sig) {
        case DIGIT_0_SIG:                     /* intentionally fall through */
        case DIGIT_1_9_SIG: {
            NumEntry_insert(me, ((KeyboardEvt const *)e)->key_code);
            return Q_HANDLED();
        }
        case POINT_SIG: {
            ;                                          /* explicitly ignore */
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&NumEntry_top);
}

