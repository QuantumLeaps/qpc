/*****************************************************************************
* Product: Ultimate Hook state pattern example
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

typedef struct UltimateHookTag {
    QHsm super;                                         /* derive from QHsm */
} UltimateHook;

void UltimateHook_ctor(UltimateHook *me);

static QState UltimateHook_initial (UltimateHook *me, QEvt const *e);
static QState UltimateHook_generic (UltimateHook *me, QEvt const *e);
static QState UltimateHook_specific(UltimateHook *me, QEvt const *e);
static QState UltimateHook_final   (UltimateHook *me, QEvt const *e);

enum UltimateHookSignals {
    A_SIG = Q_USER_SIG,
    B_SIG,
    C_SIG,
    D_SIG
};

/*..........................................................................*/
void UltimateHook_ctor(UltimateHook *me) {
    QHsm_ctor(&me->super, (QStateHandler)&UltimateHook_initial);
}
/*..........................................................................*/
QState UltimateHook_initial(UltimateHook *me, QEvt const *e) {
    (void)e;                  /* avoid the "unreferenced parameter" warning */
    printf("top-INIT;");
    return Q_TRAN(&UltimateHook_generic);
}
/*..........................................................................*/
QState UltimateHook_final(UltimateHook *me, QEvt const *e) {
    (void)me;                 /* avoid the "unreferenced parameter" warning */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("final-ENTRY;");
            printf("\nBye!Bye!\n");
            exit(0);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState UltimateHook_generic(UltimateHook *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("generic-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            printf("generic-EXIT;");
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            printf("generic-INIT;");
            return Q_TRAN(&UltimateHook_specific);
        }
        case A_SIG: {
            printf("generic-A;");
            return Q_HANDLED();
        }
        case B_SIG: {
            printf("generic-B;");
            return Q_HANDLED();
        }
        case C_SIG: {
            printf("generic:C(reset);");
            return Q_TRAN(&UltimateHook_generic);
        }
        case D_SIG: {
            return Q_TRAN(&UltimateHook_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState UltimateHook_specific(UltimateHook *me, QEvt const *e) {
    (void)me;                 /* avoid the "unreferenced parameter" warning */
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("specific-ENTRY;");
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            printf("specific-EXIT;");
            return Q_HANDLED();
        }
        case A_SIG: {
            printf("specific-A;");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&UltimateHook_generic);
}
/* test harness ============================================================*/

#include <conio.h>

static UltimateHook l_test;

void main() {
    printf("Ultimate Hook pattern\nQEP version: %s\n"
           "Press 'a'..'c' to inject signals A..C\n"
           "Press 'd' or ESC to inject signal D and quit\n",
           QEP_getVersion());

    UltimateHook_ctor(&l_test);      /* instantiate the UltimateHook object */

                /* trigger the initial transition before dispatching events */
    QHsm_init((QHsm *)&l_test, (QEvt *)0);

    for (;;) {
        QEvt e;
        uint8_t c;

        printf("\n");
        c = (uint8_t)getch();        /* read one character from the console */
        printf("%c: ", c);

        switch (c) {
            case 'a':  e.sig = A_SIG;  break;
            case 'b':  e.sig = B_SIG;  break;
            case 'c':  e.sig = C_SIG;  break;
            case 'd':
            case 0x1B: e.sig = D_SIG;  break;                  /* terminate */
        }
                               /* dispatch the event into the state machine */
        QHsm_dispatch((QHsm *)&l_test, &e);
    }
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    _exit(-1);
}
