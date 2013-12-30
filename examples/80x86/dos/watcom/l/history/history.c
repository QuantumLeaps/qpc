/*****************************************************************************
* Product: History state pattern example
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 24, 2013
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
#include "qp_port.h"                                         /* the QP port */
#include "qassert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Q_DEFINE_THIS_FILE

/*..........................................................................*/
enum ToasterOvenSignals {
    OPEN_SIG = Q_USER_SIG,
    CLOSE_SIG,
    TOAST_SIG,
    BAKE_SIG,
    OFF_SIG,
    TERMINATE_SIG                              /* terminate the application */
};
/*..........................................................................*/
typedef struct ToasterOvenTag ToasterOven;
struct ToasterOvenTag {
    QHsm super;                                         /* derive from QHsm */
    QStateHandler doorClosed_history;    /* history of the doorClosed state */
};

void ToasterOven_ctor(ToasterOven *me);                     /* default ctor */

QState ToasterOven_initial   (ToasterOven *me, QEvt const *e);
QState ToasterOven_doorOpen  (ToasterOven *me, QEvt const *e);
QState ToasterOven_off       (ToasterOven *me, QEvt const *e);
QState ToasterOven_heating   (ToasterOven *me, QEvt const *e);
QState ToasterOven_toasting  (ToasterOven *me, QEvt const *e);
QState ToasterOven_baking    (ToasterOven *me, QEvt const *e);
QState ToasterOven_doorClosed(ToasterOven *me, QEvt const *e);
QState ToasterOven_final     (ToasterOven *me, QEvt const *e);

/*..........................................................................*/
void ToasterOven_ctor(ToasterOven *me) {                    /* default ctor */
    QHsm_ctor(&me->super, (QStateHandler)&ToasterOven_initial);
}

/* HSM definitio -----------------------------------------------------------*/
QState ToasterOven_initial(ToasterOven *me, QEvt const *e) {
    (void)e;               /* avoid compiler warning about unused parameter */
    me->doorClosed_history = Q_STATE_CAST(&ToasterOven_off);
    return Q_TRAN(&ToasterOven_doorClosed);
}
/*..........................................................................*/
QState ToasterOven_final(ToasterOven *me, QEvt const *e) {
    (void)me;              /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("-> final\nBye!Bye!\n");
            _exit(0);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState ToasterOven_doorClosed(ToasterOven *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("door-Closed;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            me->doorClosed_history = QHsm_state(&me->super);
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            return Q_TRAN(&ToasterOven_off);
        }
        case OPEN_SIG: {
            return Q_TRAN(&ToasterOven_doorOpen);
        }
        case TOAST_SIG: {
            return Q_TRAN(&ToasterOven_toasting);
        }
        case BAKE_SIG: {
            return Q_TRAN(&ToasterOven_baking);
        }
        case OFF_SIG: {
            return Q_TRAN(&ToasterOven_off);
        }
        case TERMINATE_SIG: {
            return Q_TRAN(&ToasterOven_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState ToasterOven_off(ToasterOven *me, QEvt const *e) {
    (void)me;              /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("toaster-Off;");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&ToasterOven_doorClosed);
}
/*..........................................................................*/
QState ToasterOven_heating(ToasterOven *me, QEvt const *e) {
    (void)me;              /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("heater-On;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            printf("heater-Off;");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&ToasterOven_doorClosed);
}
/*..........................................................................*/
QState ToasterOven_toasting(ToasterOven *me, QEvt const *e) {
    (void)me;              /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("toasting;");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&ToasterOven_heating);
}
/*..........................................................................*/
QState ToasterOven_baking(ToasterOven *me, QEvt const *e) {
    (void)me;              /* avoid compiler warning about unused parameter */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("baking;");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&ToasterOven_heating);
}
/*..........................................................................*/
QState ToasterOven_doorOpen(ToasterOven *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("door-Open,lamp-On;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            printf("lamp-Off;");
            return Q_HANDLED();
        }
        case CLOSE_SIG: {
            return Q_TRAN(me->doorClosed_history);/*<-tranisition to HISTORY*/
        }
    }
    return Q_SUPER(&QHsm_top);
}
/* test harness ============================================================*/

#include <conio.h>

static ToasterOven l_test;

void main() {
    printf("History state pattern\nQEP version: %s\n"
           "Press 'o' to OPEN  the door\n"
           "Press 'c' to CLOSE the door\n"
           "Press 't' to start TOASTING\n"
           "Press 'b' to start BAKING\n"
           "Press 'f' to turn the oven OFF\n"
           "Press ESC to quit...\n",
           QEP_getVersion());

      /* instantiate the ToasterOven HSM and trigger the initial transition */
    ToasterOven_ctor(&l_test);
    QHsm_init((QHsm *)&l_test, (QEvt *)0);

    for (;;) {
        QEvt e;
        uint8_t c;

        printf("\n");
        c = (uint8_t)_getch();       /* read one character from the console */
        printf("%c: ", c);

        switch (c) {
            case 'o':  e.sig = OPEN_SIG;        break;
            case 'c':  e.sig = CLOSE_SIG;       break;
            case 't':  e.sig = TOAST_SIG;       break;
            case 'b':  e.sig = BAKE_SIG;        break;
            case 'f':  e.sig = OFF_SIG;         break;
            case 0x1B: e.sig = TERMINATE_SIG;   break;
        }
                               /* dispatch the event into the state machine */
        QHsm_dispatch((QHsm *)&l_test,  &e);
    }
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    _exit(-1);
}
