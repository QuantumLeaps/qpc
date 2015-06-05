/*****************************************************************************
* Product: Orthogonal Component state pattern example
* Last updated for version 5.4.2
* Last updated on  2015-06-03
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#include "qpc.h"
#include "bsp.h"
#include "alarm.h"
#include "clock.h"

#include <stdio.h>

Q_DEFINE_THIS_FILE

/*..........................................................................*/
typedef struct AlarmClockTag { /* the AlarmClock active object */
    QActive super; /* inherit QActive */

    uint32_t current_time; /* the current time in seconds */
    Alarm alarm;      /* Alarm orthogonal component */
    QTimeEvt timeEvt; /* time event generator (generates time ticks) */
} AlarmClock;

void AlarmClock_ctor(AlarmClock * const me);  /* default ctor */

/* hierarchical state machine ... */
static QState AlarmClock_initial(AlarmClock * const me, QEvt const * const e);
static QState AlarmClock_timekeeping(AlarmClock*const me, QEvt const*const e);
static QState AlarmClock_mode12hr(AlarmClock * const me, QEvt const *const e);
static QState AlarmClock_mode24hr(AlarmClock * const me, QEvt const *const e);
static QState AlarmClock_final   (AlarmClock * const me, QEvt const *const e);

/*..........................................................................*/
void AlarmClock_ctor(AlarmClock * const me) { /* default ctor */
    QActive_ctor(&me->super, Q_STATE_CAST(&AlarmClock_initial));
    Alarm_ctor(&me->alarm);                    /* orthogonal component ctor */
    /* private time event ctor */
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TICK_SIG, 0U);
}
/* HSM definition ----------------------------------------------------------*/
QState AlarmClock_initial(AlarmClock * const me, QEvt const * const e) {
    (void)e; /* unused parameter */
    me->current_time = 0U;

    /* take the top-most initial transition in the component */
    QMSM_INIT(&me->alarm.super, (QEvt *)0);

    return Q_TRAN(&AlarmClock_timekeeping);
}
/*..........................................................................*/
QState AlarmClock_final(AlarmClock * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("-> final\n");
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
QState AlarmClock_timekeeping(AlarmClock * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            /* periodic timeout every second and every second */
            QTimeEvt_armX(&me->timeEvt, BSP_TICKS_PER_SEC, BSP_TICKS_PER_SEC);
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->timeEvt);
            status = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status = Q_TRAN(&AlarmClock_mode24hr);
            break;
        }
        case CLOCK_12H_SIG: {
            status = Q_TRAN(&AlarmClock_mode12hr);
            break;
        }
        case CLOCK_24H_SIG: {
            status = Q_TRAN(&AlarmClock_mode24hr);
            break;
        }
        case ALARM_SIG: {
            printf("Wake up!!!\n");
            status = Q_HANDLED();
            break;
        }
        case ALARM_SET_SIG:
        case ALARM_ON_SIG:
        case ALARM_OFF_SIG: {
            /* synchronously dispatch to the orthogonal component */
            QMSM_DISPATCH(&me->alarm.super, e);
            status = Q_HANDLED();
            break;
        }
        case TERMINATE_SIG: {
            status = Q_TRAN(&AlarmClock_final);
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
QState AlarmClock_mode24hr(AlarmClock * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("*** 24-hour mode\n");
            status = Q_HANDLED();
            break;
        }
        case TICK_SIG: {
            TimeEvt pe; /* temporary synchronous event for the component */

            /* roll over in 24-hr mode? */
            if (++me->current_time == 24U*60U) {
                me->current_time = 0U;
            }
            printf("%02d:%02d\n",
                   me->current_time / 60U, me->current_time % 60U);
            pe.super.sig    = TIME_SIG;
            pe.current_time = me->current_time;

            /* synchronously dispatch to the orthogonal component */
            QMSM_DISPATCH(&me->alarm.super, (QEvt *)&pe);
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&AlarmClock_timekeeping);
            break;
        }
    }
    return status;
}
/*..........................................................................*/
QState AlarmClock_mode12hr(AlarmClock * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            printf("*** 12-hour mode\n");
            status = Q_HANDLED();
            break;
        }
        case TICK_SIG: {
            TimeEvt pe; /* temporary synchronous event for the component */
            uint32_t h; /* temporary variable to hold hour */

            /* roll over in 12-hr mode? */
            if (++me->current_time == 12U * 60U) {
                me->current_time = 0U;
            }
            h = me->current_time / 60U;
            printf("%02d:%02d %s\n", (h % 12U) ? (h % 12U) : 12U,
                   me->current_time % 60U, (h / 12) ? "PM" : "AM");
            pe.super.sig = TIME_SIG;
            pe.current_time = me->current_time;

            /* synchronously dispatch to the orthogonal component */
            QMSM_DISPATCH(&me->alarm.super, (QEvt *)&pe);
            status = Q_HANDLED();
            break;
        }
        default: {
            status = Q_SUPER(&AlarmClock_timekeeping);
            break;
        }
    }
    return status;
}

/* test harness ============================================================*/

/* Local-scope objects -----------------------------------------------------*/
static AlarmClock l_alarmClock;  /* the AlarmClock active object  */
static QEvt const *l_alarmClockQSto[10]; /* queue storage for AlarmClock */
static QF_MPOOL_EL(TimeEvt) l_smlPoolSto[10]; /* storage for small pool */

/* Global-scope objects (opaque pointer to the AlarmClock container) -------*/
QActive *APP_alarmClock= &l_alarmClock.super;/* AlarmClock container AO */

/*..........................................................................*/
int main(int argc, char *argv[]) {
    printf("Orthogonal Component pattern\nQP version: %s\n"
           "Press 'o' to turn the Alarm ON\n"
           "Press 'f' to turn the Alarm OFF\n"
           "Press '0'..'9' to set the Alarm time\n"
           "Press 'a' to set the Clock in 12-hour mode\n"
           "Press 'b' to set the Clock in 24-hour mode\n"
           "Press ESC to quit...\n",
           QP_versionStr);

    BSP_init(argc, argv); /* initialize the BSP */

    QF_init(); /* initialize the framework and the underlying RT kernel */

    /* publish-subscribe not used, no call to QF_psInit() */

    /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    /* instantiate and start the active objects... */
    AlarmClock_ctor(&l_alarmClock);
    QACTIVE_START(&l_alarmClock.super,
                  1U,
                  l_alarmClockQSto, Q_DIM(l_alarmClockQSto),
                  (void *)0, 0U, (QEvt *)0);

    return QF_run();                              /* run the QF application */
}
/*..........................................................................*/
void BSP_onKeyboardInput(uint8_t key) {
    switch (key) {
        case 'o': {                                 /* 'o': Alarm on event? */
            QACTIVE_POST((QActive *)&l_alarmClock,
                          Q_NEW(QEvt, ALARM_ON_SIG), (void *)0);
            break;
        }
        case 'f': {                                /* 'f': Alarm off event? */
            QACTIVE_POST((QActive *)&l_alarmClock,
                             Q_NEW(QEvt, ALARM_OFF_SIG), (void *)0);
            break;
        }
        case '1':                                                    /* '1' */
        case '2':                                                    /* '2' */
        case '3':                                                    /* '3' */
        case '4':                                                    /* '4' */
        case '5':                                                    /* '5' */
        case '6':                                                    /* '6' */
        case '7':                                                    /* '7' */
        case '8':                                                    /* '8' */
        case '9': {                                                  /* '9' */
            SetEvt *e = Q_NEW(SetEvt, ALARM_SET_SIG);
            e->digit = (uint8_t)(key - '0');
            QACTIVE_POST((QActive *)&l_alarmClock, (QEvt *)e, (void *)0);
            break;
        }
        case '0': {                                                  /* '0' */
            SetEvt *e = Q_NEW(SetEvt, ALARM_SET_SIG);
            e->digit = 0;
            QACTIVE_POST((QActive *)&l_alarmClock, (QEvt *)e, (void *)0);
            break;
        }
        case 'a': {                                /* 'a': Clock 12H event? */
            QACTIVE_POST((QActive *)&l_alarmClock,
                         Q_NEW(QEvt, CLOCK_12H_SIG), (void *)0);
            break;
        }
        case 'b': {                                /* 'b': Clock 24H event? */
            QACTIVE_POST((QActive *)&l_alarmClock,
                         Q_NEW(QEvt, CLOCK_24H_SIG), (void *)0);
            break;
        }
        case '\33': {                                       /* ESC pressed? */
            QACTIVE_POST((QActive *)&l_alarmClock,
                         Q_NEW(QEvt, TERMINATE_SIG), (void *)0);
            break;
        }
    }
}
