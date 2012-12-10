/*****************************************************************************
* Product: History state pattern example
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jul 21, 2012
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
#include "qep_port.h"
#include "qassert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Q_DEFINE_THIS_FILE

/*..........................................................................*/
enum ToastOvenSignals {
    OPEN_SIG = Q_USER_SIG,
    CLOSE_SIG,
    TOAST_SIG,
    BAKE_SIG,
    OFF_SIG,
    TERMINATE_SIG                              /* terminate the application */
};
/*..........................................................................*/
typedef struct ToastOvenTag ToastOven;
struct ToastOvenTag {
    QHsm super;                                         /* derive from QHsm */
    QStateHandler doorClosed_history;    /* history of the doorClosed state */
};

static void ToastOven_ctor(ToastOven * const me);           /* default ctor */

static QState ToastOven_initial  (ToastOven * const me, QEvt const * const e);
static QState ToastOven_doorOpen (ToastOven * const me, QEvt const * const e);
static QState ToastOven_off      (ToastOven * const me, QEvt const * const e);
static QState ToastOven_heating  (ToastOven * const me, QEvt const * const e);
static QState ToastOven_toasting (ToastOven * const me, QEvt const * const e);
static QState ToastOven_baking   (ToastOven * const me, QEvt const * const e);
static QState ToastOven_doorClosed(ToastOven * const me, QEvt const *const e);
static QState ToastOven_final    (ToastOven * const me, QEvt const * const e);

/*..........................................................................*/
static void ToastOven_ctor(ToastOven * const me) {          /* default ctor */
    QHsm_ctor(&me->super, Q_STATE_CAST(&ToastOven_initial));
}

/* HSM definitio -----------------------------------------------------------*/
static QState ToastOven_initial(ToastOven * const me, QEvt const * const e) {
    (void)e;               /* avoid compiler warning about unused parameter */
    me->doorClosed_history = Q_STATE_CAST(&ToastOven_off);
    return Q_TRAN(&ToastOven_doorClosed);
}
/*..........................................................................*/
static QState ToastOven_final(ToastOven * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("-> final\nBye!Bye!\n");
            _exit(0);
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
/*..........................................................................*/
static QState ToastOven_doorClosed(ToastOven * const me,
                                  QEvt const * const e)
{
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("door-Closed;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            me->doorClosed_history = QHsm_state(&me->super);
            status = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status = Q_TRAN(&ToastOven_off);
            break;
        }
        case OPEN_SIG: {
            status = Q_TRAN(&ToastOven_doorOpen);
            break;
        }
        case TOAST_SIG: {
            status = Q_TRAN(&ToastOven_toasting);
            break;
        }
        case BAKE_SIG: {
            status = Q_TRAN(&ToastOven_baking);
            break;
        }
        case OFF_SIG: {
            status = Q_TRAN(&ToastOven_off);
            break;
        }
        case TERMINATE_SIG: {
            status = Q_TRAN(&ToastOven_final);
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
static QState ToastOven_off(ToastOven * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("toaster-Off;");
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&ToastOven_doorClosed);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
static QState ToastOven_heating(ToastOven * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("heater-On;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            printf("heater-Off;");
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&ToastOven_doorClosed);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
static QState ToastOven_toasting(ToastOven * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("toasting;");
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&ToastOven_heating);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
static QState ToastOven_baking(ToastOven * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("baking;");
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&ToastOven_heating);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
static QState ToastOven_doorOpen(ToastOven * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("door-Open,lamp-On;");
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            printf("lamp-Off;");
            status = Q_HANDLED();
            break;
        }
        case CLOSE_SIG: {
            status = Q_TRAN(me->doorClosed_history);/*<-tranisition to HISTORY*/
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
/* test harness ============================================================*/

#include <conio.h>

static ToastOven l_test;

int main() {
    printf("History state pattern\nQEP version: %s\n"
           "Press 'o' to OPEN  the door\n"
           "Press 'c' to CLOSE the door\n"
           "Press 't' to start TOASTING\n"
           "Press 'b' to start BAKING\n"
           "Press 'f' to turn the oven OFF\n"
           "Press ESC to quit...\n",
           QEP_getVersion());

      /* instantiate the ToastOven HSM and trigger the initial transition */
    ToastOven_ctor(&l_test);
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
    return 0;
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    _exit(-1);
}
