/*****************************************************************************
* Product: Simple Blinky example
* Last Updated for Version: 5.0.0
* Date of the Last Update:  Sep 07, 2013
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
#include "qp_port.h"
#include "bsp.h"

#include <stdio.h>

Q_DEFINE_THIS_FILE

/*..........................................................................*/
enum Signals {
    TIMEOUT_SIG = Q_USER_SIG,                /* the periodic timeout signal */
};
/*..........................................................................*/
typedef struct BlinkyTag {                      /* the Blinky active object */
    QActive super;                                   /* derive from QActive */

    QTimeEvt timeEvt;                       /* private time event generator */
} Blinky;


void Blinky_ctor(Blinky * const me);
                                          /* hierarchical state machine ... */
static QState Blinky_initial(Blinky * const me, QEvt const * const e);
static QState Blinky_off    (Blinky * const me, QEvt const * const e);
static QState Blinky_on     (Blinky * const me, QEvt const * const e);

/*..........................................................................*/
void Blinky_ctor(Blinky * const me) {
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
}

/* HSM definition ----------------------------------------------------------*/
QState Blinky_initial(Blinky * const me, QEvt const * const e) {
    (void)e;               /* avoid compiler warning about unused parameter */

    /* arm the time event to expire in half a second and every half second */
    QTimeEvt_armX(&me->timeEvt, BSP_TICKS_PER_SEC/2U, BSP_TICKS_PER_SEC/2U);
    return Q_TRAN(&Blinky_off);
}
/*..........................................................................*/
QState Blinky_off(Blinky * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_ledOff();
            status = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG: {
            status = Q_TRAN(&Blinky_on);
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
QState Blinky_on(Blinky * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_ledOn();
            status = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG: {
            status = Q_TRAN(&Blinky_off);
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

/* Local-scope objects -----------------------------------------------------*/
static Blinky l_blinky;                         /* the Blinky active object */
static QEvt const *l_blinkyQSto[10];      /* Event queue storage for Blinky */

/*..........................................................................*/
int main(int argc, char *argv[]) {
    (void)argc;        /* avoid the compiler warning about unused parameter */
    (void)argv;        /* avoid the compiler warning about unused parameter */

    BSP_init();                    /* initialize the Board Support Package */
    QF_init();    /* initialize the framework and the underlying RT kernel */

    /* publish-subscribe not used, no call to QF_psInit() */
    /* dynamic event allocation not used, no call to QF_poolInit() */

                             /* instantiate and start the active objects... */
    Blinky_ctor(&l_blinky);
    QACTIVE_START(&l_blinky.super,
                  1U,
                  l_blinkyQSto, Q_DIM(l_blinkyQSto),
                  (void *)0, 1024, (QEvt *)0);

    return QF_run();                              /* run the QF application */
}
