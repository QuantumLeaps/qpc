/*****************************************************************************
* Product: Simple Blinky example
* Last Updated for Version: 5.6.5
* Date of the Last Update:  2016-06-03
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"
#include "blinky.h"
#include "bsp.h"

//Q_DEFINE_THIS_FILE

/*..........................................................................*/
typedef struct {     /* the Blinky active object */
    QActive super;   /* inherit QActive */

    QTimeEvt timeEvt; /* private time event generator */
} Blinky;

static Blinky l_blinky; /* the Blinky active object */

QActive * const AO_Blinky = &l_blinky.super;

/* hierarchical state machine ... */
static QState Blinky_initial(Blinky * const me, QEvt const * const e);
static QState Blinky_off    (Blinky * const me, QEvt const * const e);
static QState Blinky_on     (Blinky * const me, QEvt const * const e);

/*..........................................................................*/
void Blinky_ctor(void) {
    Blinky * const me = &l_blinky;
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
}

/* HSM definition ----------------------------------------------------------*/
QState Blinky_initial(Blinky * const me, QEvt const * const e) {
    (void)e; /* avoid compiler warning about unused parameter */

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

