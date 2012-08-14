/*****************************************************************************
* Product: Capstone Scuba Diving computer example
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
#include "qp_port.h"
#include "capstone.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

typedef struct NoteTag {
    uint16_t freq;                                 /* frequency of the note */
    uint16_t dur;                  /* duration of the note (in clock ticks) */
} Note;

typedef struct AlarmTag {
    Note const *note;
    uint16_t    length;
} Alarm;

/* Active object class -----------------------------------------------------*/
typedef struct AlarmMgrTag {
    QActive  super;                                  /* derive from QActive */
    QTimeEvt timer;
    QPSet8   active_alarms;                         /* set of active alarms */

    uint8_t  curr_alarm;              /* currently played alarm form number */
    uint8_t  curr_note;      /* currently played note within the alarm form */
} AlarmMgr;

static QState AlarmMgr_initial(AlarmMgr *me, QEvt const *e);
static QState AlarmMgr_on     (AlarmMgr *me, QEvt const *e);
static QState AlarmMgr_silent (AlarmMgr *me, QEvt const *e);
static QState AlarmMgr_playing(AlarmMgr *me, QEvt const *e);

/* Local objects -----------------------------------------------------------*/
static AlarmMgr l_alarmmgr;       /* the single instance of the AlarmMgr AO */

static Note const l_alarm1[] = {                             /* depth alarm */
    { 0x20, BSP_TICKS_PER_SEC/2 },
    { 0x80, BSP_TICKS_PER_SEC/2 }
};

static Note const l_alarm2[] = {                       /* ascent rate alarm */
    { 0x80, BSP_TICKS_PER_SEC/2 },
    { 0xD0, BSP_TICKS_PER_SEC/2 }
};

static Note const l_alarm3[] = {                        /* out of air alarm */
    { 0x20, BSP_TICKS_PER_SEC/4 },
    { 0x00, BSP_TICKS_PER_SEC/4 },
    { 0x20, BSP_TICKS_PER_SEC/4 },
    { 0x00, BSP_TICKS_PER_SEC/4 },
    { 0xD0, BSP_TICKS_PER_SEC/4 },
    { 0x00, BSP_TICKS_PER_SEC/4 },
    { 0xD0, BSP_TICKS_PER_SEC/4 },
    { 0x00, BSP_TICKS_PER_SEC/4 }
};

static Alarm const l_alarms[] = {        /* consistent with enum AlarmTypes */
    { (Note *)0, 0               },                           /* ALL_ALARMS */
    { l_alarm1,  Q_DIM(l_alarm1) },                          /* DEPTH_ALARM */
    { l_alarm2,  Q_DIM(l_alarm2) },                    /* ASCENT_RATE_ALARM */
    { l_alarm3,  Q_DIM(l_alarm3) }                      /* OUT_OF_AIR_ALARM */
};


/* helper functions --------------------------------------------------------*/
#define QPSet8_clear(me_) ((me_)->bits = (uint8_t)0)

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_AlarmMgr = (QActive *)&l_alarmmgr;/* "opaque" AO pointer */

/* local constants ---------------------------------------------------------*/

/*..........................................................................*/
void AlarmMgr_ctor(void) {
    AlarmMgr *me = &l_alarmmgr;

    QActive_ctor(&me->super, (QStateHandler)&AlarmMgr_initial);
    QTimeEvt_ctor(&me->timer, TIMEOUT_SIG);

    QPSet8_clear(&me->active_alarms);
}
/*..........................................................................*/
QState AlarmMgr_initial(AlarmMgr *me, QEvt const *e) {
    (void)e;        /* suppress the compiler warning about unused parameter */

    /* active object and time events */
    QS_OBJ_DICTIONARY(&l_alarmmgr);
    QS_OBJ_DICTIONARY(&l_alarmmgr.timer);

    /* states */
    QS_FUN_DICTIONARY(&AlarmMgr_on);
    QS_FUN_DICTIONARY(&AlarmMgr_silent);
    QS_FUN_DICTIONARY(&AlarmMgr_playing);

    /* signals just for AlarmMgr */
    QS_SIG_DICTIONARY(ALARM_REQUEST_SIG, me);
    QS_SIG_DICTIONARY(ALARM_SILENCE_SIG, me);
    QS_SIG_DICTIONARY(TIMEOUT_SIG, me);

    return Q_TRAN(&AlarmMgr_silent);
}
/*..........................................................................*/
QState AlarmMgr_on(AlarmMgr *me, QEvt const *e) {
    switch (e->sig) {
        case ALARM_REQUEST_SIG: {
            uint8_t alarm_type = ((AlarmEvt *)e)->alarm_type;

            Q_ASSERT((ALL_ALARMS < alarm_type)
                     && (alarm_type < Q_DIM(l_alarms)));

            if (QPSet8_hasElement(&me->active_alarms, alarm_type)) {
                return Q_HANDLED();
            }
            else {
                QPSet8_insert(&me->active_alarms, alarm_type);
                QPSet8_findMax(&me->active_alarms, alarm_type);
                me->curr_alarm = alarm_type;
                me->curr_note  = 0;            /* start with the first note */
                return Q_TRAN(&AlarmMgr_playing);
            }
        }
        case ALARM_SILENCE_SIG: {
            uint8_t alarm_type = ((AlarmEvt *)e)->alarm_type;

            Q_ASSERT(alarm_type < Q_DIM(l_alarms));

            if (alarm_type == ALL_ALARMS) {
                QPSet8_clear(&me->active_alarms);
                return Q_TRAN(&AlarmMgr_silent);
            }
            else {
                QPSet8_remove(&me->active_alarms, alarm_type);
                if (QPSet8_isEmpty(&me->active_alarms)) {
                    return Q_TRAN(&AlarmMgr_silent);
                }
                else {
                    QPSet8_findMax(&me->active_alarms, alarm_type);
                    if (me->curr_alarm != alarm_type) {
                        me->curr_alarm = alarm_type;
                        me->curr_note = 0;
                        return Q_TRAN(&AlarmMgr_playing);
                    }
                    else {
                        return Q_HANDLED();
                    }
                }
            }
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState AlarmMgr_silent(AlarmMgr *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            TIM_CounterCmd(TIM3, TIM_STOP);                      /* silence */
            me->curr_alarm = 0;
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&AlarmMgr_on);
}
/*..........................................................................*/
QState AlarmMgr_playing(AlarmMgr *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_postIn(&me->timer, (QActive *)me,
                            l_alarms[me->curr_alarm].note[me->curr_note].dur);
            TIM_PrescalerConfig(TIM3,
                           l_alarms[me->curr_alarm].note[me->curr_note].freq);
            TIM_CounterCmd(TIM3, TIM_START);                 /* start noise */
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->timer);
            return Q_HANDLED();
        }
        case TIMEOUT_SIG: {
            ++me->curr_note;
            if (me->curr_note == l_alarms[me->curr_alarm].length) {
                me->curr_note = 0;                      /* start over again */
            }
            return Q_TRAN(&AlarmMgr_playing);         /* transition to self */
        }
    }
    return Q_SUPER(&AlarmMgr_on);
}
