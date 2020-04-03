/*****************************************************************************
* Product: Reminder state pattern example
* Last updated for version 6.8.0
* Last updated on  2020-04-01
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"
#include "bsp.h"

#include "safe_std.h" /* portable "safe" <stdio.h>/<string.h> facilities */

Q_DEFINE_THIS_FILE

/*..........................................................................*/
enum SensorSignals {
    TIMEOUT_SIG = Q_USER_SIG, /* the periodic timeout signal */
    DATA_READY_SIG,           /* the invented reminder signal */
    TERMINATE_SIG             /* terminate the application */
};
/*..........................................................................*/
typedef struct SensorTag {    /* the Sensor active object */
    QActive super;            /* inherit QActive */

    QTimeEvt timeEvt;         /* private time event generator */
    uint16_t pollCtr;
    uint16_t procCtr;
} Sensor;


void Sensor_ctor(Sensor * const me);

/* hierarchical state machine ... */
static QState Sensor_initial   (Sensor * const me, QEvt const * const e);
static QState Sensor_polling   (Sensor * const me, QEvt const * const e);
static QState Sensor_processing(Sensor * const me, QEvt const * const e);
static QState Sensor_idle      (Sensor * const me, QEvt const * const e);
static QState Sensor_busy      (Sensor * const me, QEvt const * const e);
static QState Sensor_final     (Sensor * const me, QEvt const * const e);

/*..........................................................................*/
void Sensor_ctor(Sensor * const me) {
    QActive_ctor(&me->super, (QStateHandler)&Sensor_initial);
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
}

/* HSM definition ----------------------------------------------------------*/
QState Sensor_initial(Sensor * const me, QEvt const * const e) {
    (void)e; /* unused parameter */

    me->pollCtr = 0U;
    me->procCtr = 0U;

    return Q_TRAN(&Sensor_polling);
}
/*..........................................................................*/
QState Sensor_final(Sensor * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QF_stop(); /* terminate the application */
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
QState Sensor_polling(Sensor * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            /* periodic timeout in 1/2 second and every 1/2 second */
            QTimeEvt_armX(&me->timeEvt, BSP_TICKS_PER_SEC/2, BSP_TICKS_PER_SEC/2);
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->timeEvt);
            status = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status = Q_TRAN(&Sensor_processing);
            break;
        }
        case TIMEOUT_SIG: {
            /* NOTE: this constant event is statically pre-allocated.
            * It can be posted/published as any other event.
            */
            static const QEvt reminderEvt = { DATA_READY_SIG, 0U, 0U };

            ++me->pollCtr;
            PRINTF_S("polling %3d\n", me->pollCtr);
            if ((me->pollCtr & 0x3U) == 0U) {  /* modulo 4 */
                QACTIVE_POST(&me->super, &reminderEvt, me);
            }
            status = Q_HANDLED();
            break;
        }
        case TERMINATE_SIG: {
            status = Q_TRAN(&Sensor_final);
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
QState Sensor_processing(Sensor * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_INIT_SIG: {
            status = Q_TRAN(&Sensor_idle);
            break;
        }
        default: {
            status = Q_SUPER(&Sensor_polling);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Sensor_idle(Sensor * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            PRINTF_S("%s\n", "idle-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case DATA_READY_SIG: {
            status = Q_TRAN(&Sensor_busy);
            break;
        }
        default: {
            status = Q_SUPER(&Sensor_processing);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState Sensor_busy(Sensor * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            PRINTF_S("%s\n", "busy-ENTRY;");
            status = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG: {
            ++me->procCtr;
            PRINTF_S("processing %3d\n", me->procCtr);
            if ((me->procCtr & 0x1U) == 0U) { /* modulo 2 */
                status = Q_TRAN(&Sensor_idle);
            }
            else {
                status = Q_HANDLED();
            }
            break;
        }
        default: {
            status = Q_SUPER(&Sensor_processing);
            break;
        }
    }
    return status;
}

/* test harness ============================================================*/

/* Local-scope objects -----------------------------------------------------*/
static Sensor l_sensor; /* the Sensor active object */
static QEvt const *l_sensorQSto[10]; /* Event queue storage for Sensor */

/*..........................................................................*/
int main(int argc, char *argv[]) {
    PRINTF_S("Reminder state pattern\nQP version: %s\n"
           "Press ESC to quit...\n",
           QP_versionStr);

    BSP_init(argc, argv); /* initialize the BSP */

    QF_init(); /* initialize the framework and the underlying RT kernel */

    /* publish-subscribe not used, no call to QF_psInit() */
    /* dynamic event allocation not used, no call to QF_poolInit() */

    /* instantiate and start the active objects... */
    Sensor_ctor(&l_sensor);
    QACTIVE_START(&l_sensor.super,
                  1U,
                  l_sensorQSto, Q_DIM(l_sensorQSto),
                  (void *)0, 0U, (QEvt *)0);

    return QF_run(); /* run the QF application */
}
/*..........................................................................*/
void BSP_onKeyboardInput(uint8_t key) {
    switch (key) {
        case '\033': { /* ESC pressed? */
            /* NOTE: this constant event is statically pre-allocated.
            * It can be posted/published as any other event.
            */
            static QEvt const terminateEvt = { TERMINATE_SIG, 0U, 0U };
            QACTIVE_POST((QActive *)&l_sensor, &terminateEvt, (void *)0);
            break;
        }
    }
}
