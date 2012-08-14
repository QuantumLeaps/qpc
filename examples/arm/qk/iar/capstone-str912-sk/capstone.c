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

#include "scuba.h"                       /* generic scuba diving algorithms */
#include "drv_hd44780.h"                              /* LCD display driver */

//Q_DEFINE_THIS_FILE

/* Active object class -----------------------------------------------------*/
typedef struct CapstoneTag {
    QActive  super;                                  /* derive from QActive */

    QTimeEvt heartbeat;
    QTimeEvt dt_tts;/* timer to alternate dive time/time to surface display */
    QTimeEvt add_gas;

    int32_t  depth_in_mm;
    int8_t   depth_units[2];
    uint8_t  heartbeat_led_sel;
    uint8_t  dt_tts_sel;
    int32_t  ascent_rate_in_mm_per_sec;
    uint32_t start_dive_time_in_ticks;
    uint32_t dive_time_in_ticks;
    uint32_t tts_in_ticks;                               /* time to surface */
    uint32_t gas_in_cylinder_in_cl;   /* amount of gas in centliters at STP */
    uint32_t consumed_gas_in_cl;             /* consumed gas in centiliters */
} Capstone;

static QState Capstone_initial   (Capstone *me, QEvt const *e);
static QState Capstone_always    (Capstone *me, QEvt const *e);
static QState Capstone_surfaced  (Capstone *me, QEvt const *e);
static QState Capstone_adding_gas(Capstone *me, QEvt const *e);
static QState Capstone_diving    (Capstone *me, QEvt const *e);

static void Capstone_display_assent  (Capstone *me);
static void Capstone_display_depth   (Capstone *me);
static void Capstone_display_pressure(Capstone *me);

/* Local objects -----------------------------------------------------------*/
static Capstone l_capstone;       /* the single instance of the Capstone AO */

/* helper functions --------------------------------------------------------*/
static int8_t const *bin2dec3(uint32_t val);
static int8_t const *ticks2min_sec(uint32_t val);

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_Capstone = (QActive *)&l_capstone;/* "opaque" AO pointer */

/* local constants ---------------------------------------------------------*/
                                        /** mid point of a 10 bit converter */
#define MID_A2D_SCALE           511

       /** conversion from raw A2D reading to ascent/descent rate in mm/min */
#define ASCENT_RATE_MM_PER_MIN(raw_) \
    (int32_t)((((int32_t)(raw_) - MID_A2D_SCALE) * FULL_SCALE_ASCENT_RATE) \
              / MID_A2D_SCALE)

                  /** the column on the LCD used for displaying ascent rate */
#define LCD_AR_X                1
                     /** the row on the LCD used for displaying ascent rate */
#define LCD_AR_Y                1
                           /** the row on the LCD used for displaying depth */
#define LCD_DEPTH_Y             1
                  /** the column on the LCD used for displaying depth label */
#define LCD_DEPTH_X             9
                  /** the column on the LCD used for displaying depth units */
#define LCD_DEPTH_UNITS_X       16
              /** the row on the LCD used for displaying cylinder presssure */
#define LCD_CP_Y                2
           /** the column on the LCD used for displaying cylinder presssure */
#define LCD_CP_X                1
                  /* the row on the LCD used for displaying time to surface */
#define LCD_TTS_Y               2
              /** the column on the LCD used for displaying time to surface */
#define LCD_TTS_X               9
                      /** number of bars on the cylinder-pressure bar-chart */
#define LCD_PRESSURE_LIMIT      40
                            /** number of bars on the ascent rate bar-chart */
#define LCD_ASCENT_RATE_LIMIT   40


       /** depths greater than this will result in an alarm being generated */
#define MAXIMUM_DEPTH_IN_MM     (40*1000)

                             /** the assumed cylinder volume in centiLitres */
#define CYLINDER_VOLUME_IN_CL   (10*100)

                                     /** maximum ascent rate in mm / minute */
#define FULL_SCALE_ASCENT_RATE  (40*1000)
                                       /** maximum cylinder pressure in bar */
#define FULL_SCALE_CYLINDER_PRESSURE   200

                                /** gas safety margin of 20% in centiLitres */
#define GAS_SAFETY_MARGIN  \
    ((CYLINDER_VOLUME_IN_CL * FULL_SCALE_CYLINDER_PRESSURE) / 5)

                              /** added gas increment volume in centiLitres */
#define GAS_INCREMENT_IN_CL      500

/* ctor ....................................................................*/
void Capstone_ctor(void) {
    Capstone *me = &l_capstone;

    QActive_ctor(&me->super, (QStateHandler)&Capstone_initial);
    QTimeEvt_ctor(&me->heartbeat, HEARTBEAT_SIG);
    QTimeEvt_ctor(&me->dt_tts,    DT_TTS_SIG);
    QTimeEvt_ctor(&me->add_gas,   ADD_GAS_SIG);
}
/* HSM =====================================================================*/
QState Capstone_initial(Capstone *me, QEvt const *e) {
    (void)e;        /* suppress the compiler warning about unused parameter */

    HD44780_PowerUpInit();                            /* LCD initialization */
    HD44780_DispHorBarInit();
    HD44780_StrShow(LCD_DEPTH_X, LCD_DEPTH_Y, "Dpt");

    me->depth_units[0]        = 'm';                              /* meters */
    me->depth_units[1]        = '\0';                     /* zero terminate */
    me->gas_in_cylinder_in_cl = 0;
    me->heartbeat_led_sel     = 0;
    me->dt_tts_sel            = 0;

    QActive_subscribe((QActive *)me, B1_DOWN_SIG);
    QActive_subscribe((QActive *)me, B1_UP_SIG);
    QActive_subscribe((QActive *)me, B2_DOWN_SIG);
    QActive_subscribe((QActive *)me, B2_UP_SIG);


    /* active object and time events */
    QS_OBJ_DICTIONARY(&l_capstone);
    QS_OBJ_DICTIONARY(&l_capstone.heartbeat);
    QS_OBJ_DICTIONARY(&l_capstone.dt_tts);
    QS_OBJ_DICTIONARY(&l_capstone.add_gas);

    /* states */
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&Capstone_initial);
    QS_FUN_DICTIONARY(&Capstone_always);
    QS_FUN_DICTIONARY(&Capstone_surfaced);
    QS_FUN_DICTIONARY(&Capstone_diving);

    /* global published signals */
    QS_SIG_DICTIONARY(B1_DOWN_SIG, 0);
    QS_SIG_DICTIONARY(B1_UP_SIG,   0);
    QS_SIG_DICTIONARY(B2_DOWN_SIG, 0);
    QS_SIG_DICTIONARY(B2_UP_SIG,   0);

    /* signals just for Capstone */
    QS_SIG_DICTIONARY(ASCENT_RATE_ADC_SIG, me);
    QS_SIG_DICTIONARY(HEARTBEAT_SIG, me);
    QS_SIG_DICTIONARY(DT_TTS_SIG, me);

    return Q_TRAN(&Capstone_always);
}
/*..........................................................................*/
QState Capstone_always(Capstone *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_postEvery(&me->heartbeat, (QActive *)me,
                               BSP_TICKS_PER_SEC/2);    /* every 1/2 second */
            QTimeEvt_postEvery(&me->dt_tts, (QActive *)me,
                               BSP_TICKS_PER_SEC*2);     /* every 2 seconds */
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->heartbeat);
            QTimeEvt_disarm(&me->dt_tts);
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            return Q_TRAN(&Capstone_surfaced);
        }
        case B2_DOWN_SIG: {                    /* depth unit change request */
            if (me->depth_units[0] == 'm') {
                me->depth_units[0] = 'f';
            }
            else {
                me->depth_units[0] = 'm';
            }
            Capstone_display_depth(me);
            return Q_HANDLED();
        }
        case HEARTBEAT_SIG: {            /* heartbeat arrives every 1/2 sec */
            ADC_StandbyModeCmd(DISABLE);           /* exit ADC standby mode */
            ADC_ConversionCmd(ADC_Conversion_Start);/* start the conversion */
            BSP_LED_on(15);               /* trun on the ADC conversion LED */

            if (me->heartbeat_led_sel) {
                BSP_LED_on(9);
            }
            else {
                BSP_LED_off(9);
            }
            me->heartbeat_led_sel = !me->heartbeat_led_sel;

            return Q_HANDLED();
        }
        case DT_TTS_SIG: {               /* alternate between dive-time/tts */
            if (me->dt_tts_sel) {
                HD44780_StrShow(LCD_TTS_X, LCD_TTS_Y, "TTS");
                HD44780_StrShow(LCD_TTS_X + 3, LCD_TTS_Y,
                                ticks2min_sec(me->tts_in_ticks));
            }
            else {
                HD44780_StrShow(LCD_TTS_X, LCD_TTS_Y, "Div");
                HD44780_StrShow(LCD_TTS_X + 3, LCD_TTS_Y,
                                ticks2min_sec(me->dive_time_in_ticks));
            }
            me->dt_tts_sel = !me->dt_tts_sel;
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState Capstone_surfaced(Capstone *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_LED_on(10);                      /* turn on the surface LED */

            me->depth_in_mm = 0;            /* depth is zero at the surface */
            Capstone_display_depth(me);

            me->dive_time_in_ticks = 0; /* dive time is zero at the surface */
            me->tts_in_ticks = 0; /* time to surface is zero at the surface */

            me->ascent_rate_in_mm_per_sec = 0;       /* zero at the surface */

            /* make sure that ascent-rate bar graph is clear at the surface */
            Capstone_display_assent(me);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            BSP_LED_off(10);                    /* turn off the surface LED */
            return Q_HANDLED();
        }
        case ASCENT_RATE_ADC_SIG: {
            BSP_LED_off(15);             /* turn off the ADC conversion LED */

            me->ascent_rate_in_mm_per_sec =
                ASCENT_RATE_MM_PER_MIN(((ADCEvt *)e)->raw);

            if (me->ascent_rate_in_mm_per_sec >= 0) {   /* still ascending? */
                me->ascent_rate_in_mm_per_sec = 0;/* at the surface already */
                return Q_HANDLED();
            }
            else {                                                /* diving */
                return Q_TRAN(&Capstone_diving);
            }
        }
        case B1_DOWN_SIG: {                     /* cylinder filling request */
            return Q_TRAN(&Capstone_adding_gas);
        }
        case B1_UP_SIG: {
            Capstone_display_pressure(me);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&Capstone_always);
}
/*..........................................................................*/
QState Capstone_adding_gas(Capstone *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_postEvery(&me->add_gas, (QActive *)me,
                               BSP_TICKS_PER_SEC/5);    /* every 1/5 second */
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->add_gas);
            Capstone_display_pressure(me);
            return Q_HANDLED();
        }
        case ADD_GAS_SIG: {
                         /* can the cylinder hold additional gas increment? */
            if (me->gas_in_cylinder_in_cl + GAS_INCREMENT_IN_CL
                <= (CYLINDER_VOLUME_IN_CL * FULL_SCALE_CYLINDER_PRESSURE))
            {
                me->gas_in_cylinder_in_cl += GAS_INCREMENT_IN_CL;/* add gas */
                Capstone_display_pressure(me);
            }
            else {                                  /* the cylinder is full */
                HD44780_StrShow(LCD_CP_X + 2, LCD_CP_Y, "FULL");
            }
            return Q_HANDLED();
        }
        case B1_UP_SIG: {
            return Q_TRAN(&Capstone_surfaced);
        }
    }
    return Q_SUPER(&Capstone_surfaced);
}
/*..........................................................................*/
QState Capstone_diving(Capstone *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            BSP_LED_on(11);                       /* turn on the diving LED */
            BSP_LCD_light_on();             /* turn on the LCD backlight on */
            me->start_dive_time_in_ticks = BSP_get_ticks();
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            AlarmEvt *ae;
            BSP_LED_off(11);                     /* turn off the diving LED */
            BSP_LCD_light_off();           /* turn on the LCD backlight off */

                                                      /* silence ALL alarms */
            ae = Q_NEW(AlarmEvt, ALARM_SILENCE_SIG);
            ae->alarm_type = ALL_ALARMS;
            QACTIVE_POST(AO_AlarmMgr, (QEvt *)ae, me);

            return Q_HANDLED();
        }
        case ASCENT_RATE_ADC_SIG: {
            BSP_LED_off(15);             /* turn off the ADC conversion LED */

            me->ascent_rate_in_mm_per_sec =
                ASCENT_RATE_MM_PER_MIN(((ADCEvt *)e)->raw);

                            /* integrate the depth based on the ascent rate */
            me->depth_in_mm -=
                depth_change_in_mm(me->ascent_rate_in_mm_per_sec);

            if (me->depth_in_mm > 0) {           /* not on the surface yet? */
                AlarmEvt *ae;
                uint32_t consumed_gas_in_cl = gas_rate_in_cl(me->depth_in_mm);

                if (me->gas_in_cylinder_in_cl > consumed_gas_in_cl) {
                    me->gas_in_cylinder_in_cl -= consumed_gas_in_cl;
                }
                else {
                    me->gas_in_cylinder_in_cl = 0;
                }

                me->dive_time_in_ticks = BSP_get_ticks()
                                         - me->start_dive_time_in_ticks;

                me->tts_in_ticks = me->depth_in_mm * (60 * BSP_TICKS_PER_SEC)
                                   / ASCENT_RATE_LIMIT;

                Capstone_display_depth(me);
                Capstone_display_assent(me);
                Capstone_display_pressure(me);

                                           /* check the OUT_OF_AIR_ALARM... */
                if (me->gas_in_cylinder_in_cl <
                    gas_to_surface_in_cl(me->depth_in_mm) + GAS_SAFETY_MARGIN)
                {
                    ae = Q_NEW(AlarmEvt, ALARM_REQUEST_SIG);
                    ae->alarm_type = OUT_OF_AIR_ALARM;
                    QACTIVE_POST(AO_AlarmMgr, (QEvt *)ae, me);
                }
                else {
                    ae = Q_NEW(AlarmEvt, ALARM_SILENCE_SIG);
                    ae->alarm_type = OUT_OF_AIR_ALARM;
                    QACTIVE_POST(AO_AlarmMgr, (QEvt *)ae, me);
                }

                                          /* check the ASCENT_RATE_ALARM... */
                if (me->ascent_rate_in_mm_per_sec > ASCENT_RATE_LIMIT) {
                    ae = Q_NEW(AlarmEvt, ALARM_REQUEST_SIG);
                    ae->alarm_type = ASCENT_RATE_ALARM;
                    QACTIVE_POST(AO_AlarmMgr, (QEvt *)ae, me);
                }
                else {
                    ae = Q_NEW(AlarmEvt, ALARM_SILENCE_SIG);
                    ae->alarm_type = ASCENT_RATE_ALARM;
                    QACTIVE_POST(AO_AlarmMgr, (QEvt *)ae, me);
                }

                                                /* check the DEPTH_ALARM... */
                if (me->depth_in_mm > MAXIMUM_DEPTH_IN_MM) {
                    ae = Q_NEW(AlarmEvt, ALARM_REQUEST_SIG);
                    ae->alarm_type = DEPTH_ALARM;
                    QACTIVE_POST(AO_AlarmMgr, (QEvt *)ae, me);
                }
                else {
                    ae = Q_NEW(AlarmEvt, ALARM_SILENCE_SIG);
                    ae->alarm_type = DEPTH_ALARM;
                    QACTIVE_POST(AO_AlarmMgr, (QEvt *)ae, me);
                }

                return Q_HANDLED();
            }
            else {                             /* depth is zero or negative */
                return Q_TRAN(&Capstone_surfaced);      /* we are surfacing */
            }
        }
    }
    return Q_SUPER(&Capstone_always);
}

/* action functions ========================================================*/
void Capstone_display_assent(Capstone *me) {
    if (me->ascent_rate_in_mm_per_sec > 0) {                  /* ascending? */
        HD44780_DispHorBar(LCD_AR_X, LCD_AR_Y,
            (me->ascent_rate_in_mm_per_sec * LCD_ASCENT_RATE_LIMIT)
                / FULL_SCALE_ASCENT_RATE,
            LCD_ASCENT_RATE_LIMIT);
    }
    else {                          /* descending--show empty ascending bar */
        HD44780_DispHorBar(LCD_AR_X, LCD_AR_Y, 0, LCD_ASCENT_RATE_LIMIT);
    }
}
/*..........................................................................*/
void Capstone_display_depth(Capstone *me) {
    int32_t displayed_depth;

    if (me->depth_units[0] == 'm') {                        /* show meters? */
        displayed_depth = (me->depth_in_mm + 1000/2) / 1000;
    }
    else {                                                     /* show feet */
        displayed_depth = (me->depth_in_mm * 328 + 100000/2) / 100000;
    }

    if (displayed_depth > 999) {             /* clamp the depth to 3 digits */
        displayed_depth = 999;
    }

    HD44780_StrShow(LCD_DEPTH_X + 4,  LCD_DEPTH_Y, bin2dec3(displayed_depth));
    HD44780_StrShow(LCD_DEPTH_UNITS_X, LCD_DEPTH_Y, me->depth_units);
}
/*..........................................................................*/
void Capstone_display_pressure(Capstone *me) {
    uint32_t cylinder_pressure_in_bar =
         1 + (me->gas_in_cylinder_in_cl / CYLINDER_VOLUME_IN_CL);
    HD44780_DispHorBar(LCD_CP_X, LCD_CP_Y,
        (cylinder_pressure_in_bar * LCD_PRESSURE_LIMIT
             / FULL_SCALE_CYLINDER_PRESSURE),
        LCD_PRESSURE_LIMIT);
}
/*..........................................................................*/
int8_t const *bin2dec3(uint32_t val) {
    static char str[] = "DDD";
    str[2] = '0' + (val % 10);
    val /= 10;
    str[1] = '0' + (val % 10);
    val /= 10;
    str[0] = '0' + (val % 10);
    if (str[0] == '0') {
         str[0] = ' ';
         if (str[1] == '0') {
             str[1] = ' ';
         }
    }
    return (int8_t const *)str;
}
/*..........................................................................*/
int8_t const *ticks2min_sec(uint32_t val) {
    static char str[] = "MM:SS";
    uint32_t tmp;

    val /= BSP_TICKS_PER_SEC;                     /* convert val to seconds */
    tmp = val / 60;                                    /* tmp holds minutes */
    str[1] = '0' + (tmp % 10);
    str[0] = '0' + (tmp / 10);
    val -= tmp * 60;                                    /* subtract minutes */
    str[4] = '0' + (val % 10);
    str[3] = '0' + (val / 10);
    if (str[0] == '0') {
         str[0] = ' ';
    }
    return (int8_t const *)str;
}
