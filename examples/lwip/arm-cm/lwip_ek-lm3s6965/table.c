/*****************************************************************************
* Product: DPP example with lwIP and direct screen output
* Last updated for version 6.9.1
* Last updated on  2020-09-11
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"     /* QP/C header file */
#include "dpp.h"     /* application events and active objects */
#include "bsp.h"     /* Board Support Package header file */

#include "rit128x96x4.h"      /* RITEK 128x96x4 OLED used in Rev C-D boards */

#include <stdio.h>   /* for snprintf() */

Q_DEFINE_THIS_FILE

/* Active object class -----------------------------------------------------*/
typedef struct TableTag {
    QActive super;

    uint8_t fork[N_PHILO];
    uint8_t isHungry[N_PHILO];
    uint8_t displayOn;
    uint8_t udpCtr;
    QTimeEvt te_DISPLAY_TIMEOUT;
} Table;

static QState Table_initial(Table *me, QEvt const *e);
static QState Table_serving(Table *me, QEvt const *e);

static void Table_displayInit(Table *me);
static void Table_displayOn(Table *me);
static void Table_displayOff(Table *me);
static void Table_displayPhilStat(Table *me, uint8_t n, char const *stat);
static void Table_displyIPAddr(Table *me, char const *ip_addr);
static void Table_displyUdpText(Table *me, char const *text);
static void Table_displyCgiText(Table *me, char const *text);

#define RIGHT(n_) ((uint8_t)(((n_) + (N_PHILO - 1)) % N_PHILO))
#define LEFT(n_)  ((uint8_t)(((n_) + 1) % N_PHILO))
enum ForkState { FREE, USED };

#ifdef Q_SPY
enum AppRecords { /* application-specific QS trace records */
    PHILO_STAT = QS_USER,
    CGI_TEXT,
    UDP_TEXT,
};
#endif

/* Local objects -----------------------------------------------------------*/
static Table l_table; /* the single instance of the Table active object */

enum TablePrivateSignals {
   DISPLAY_TIMEOUT_SIG = MAX_SIG
};

#define DISPLAY_TIMEOUT (BSP_TICKS_PER_SEC * 30)

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_Table = (QActive *)&l_table; /* "opaque" AO pointer */

/*..........................................................................*/
void Table_ctor(void) {
    uint8_t n;
    Table *me = &l_table;

    QActive_ctor(&me->super, (QStateHandler)&Table_initial);
    QTimeEvt_ctorX(&me->te_DISPLAY_TIMEOUT, &me->super,
                   DISPLAY_TIMEOUT_SIG, 0U);

    for (n = 0; n < N_PHILO; ++n) {
        me->fork[n] = FREE;
        me->isHungry[n] = 0;
    }
    me->udpCtr = 0;
}
/*..........................................................................*/
QState Table_initial(Table *me, QEvt const *e) {
    (void)e; /* unused parameter */

    /* Initialize the OLED display */
    Table_displayInit(me);

    QActive_subscribe((QActive *)me, DONE_SIG);
    QActive_subscribe((QActive *)me, BTN_DOWN_SIG);
    QActive_subscribe((QActive *)me, DISPLAY_IPADDR_SIG);
    QActive_subscribe((QActive *)me, DISPLAY_CGI_SIG);
    QActive_subscribe((QActive *)me, DISPLAY_UDP_SIG);

    QS_OBJ_DICTIONARY(&l_table);
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&Table_initial);
    QS_FUN_DICTIONARY(&Table_serving);

    QS_SIG_DICTIONARY(DONE_SIG,            0); /* global signals */
    QS_SIG_DICTIONARY(EAT_SIG,             0);
    QS_SIG_DICTIONARY(DISPLAY_IPADDR_SIG,  0);
    QS_SIG_DICTIONARY(DISPLAY_CGI_SIG,     0);
    QS_SIG_DICTIONARY(DISPLAY_UDP_SIG,     0);

    QS_SIG_DICTIONARY(HUNGRY_SIG,          me); /* signal just for Table */
    QS_SIG_DICTIONARY(DISPLAY_TIMEOUT_SIG, me); /* signal just for Table */

    return Q_TRAN(&Table_serving);
}
/*..........................................................................*/
QState Table_serving(Table *me, QEvt const *e) {
    uint8_t n, m;
    TableEvt *pe;

    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_armX(&me->te_DISPLAY_TIMEOUT,
                          DISPLAY_TIMEOUT, DISPLAY_TIMEOUT);
            Table_displayOn(me);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->te_DISPLAY_TIMEOUT);
            Table_displayOff(me);
            return Q_HANDLED();
        }
        case HUNGRY_SIG: {
            n = ((TableEvt const *)e)->philoNum;
            /* phil ID must be in range and he must be not hungry */
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            Table_displayPhilStat(me, n, "hungry  ");
            m = LEFT(n);
            if ((me->fork[m] == FREE) && (me->fork[n] == FREE)) {
                me->fork[m] = me->fork[n] = USED;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = n;
                QF_PUBLISH((QEvt *)pe, me);
                Table_displayPhilStat(me, n, "eating  ");
            }
            else {
                me->isHungry[n] = 1;
            }
            return Q_HANDLED();
        }
        case DONE_SIG: {
            n = ((TableEvt const *)e)->philoNum;
            /* phil ID must be in range and he must be not hungry */
            Q_ASSERT((n < N_PHILO) && (!me->isHungry[n]));

            Table_displayPhilStat(me, n, "thinking");
            m = LEFT(n);
            /* both forks of Phil[n] must be used */
            Q_ASSERT((me->fork[n] == USED) && (me->fork[m] == USED));

            me->fork[m] = me->fork[n] = FREE;
            m = RIGHT(n); /* check the right neighbor */
            if (me->isHungry[m] && (me->fork[m] == FREE)) {
                me->fork[n] = me->fork[m] = USED;
                me->isHungry[m] = 0;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = m;
                QF_PUBLISH((QEvt *)pe, me);
                Table_displayPhilStat(me, m, "eating  ");
            }
            m = LEFT(n); /* check the left neighbor */
            n = LEFT(m); /* left fork of the left neighbor */
            if (me->isHungry[m] && (me->fork[n] == FREE)) {
                me->fork[m] = me->fork[n] = USED;
                me->isHungry[m] = 0;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = m;
                QF_PUBLISH((QEvt *)pe, me);
                Table_displayPhilStat(me, m, "eating  ");
            }
            return Q_HANDLED();
        }
        case BTN_DOWN_SIG: {
            Table_displayOn(me);
            return Q_HANDLED();
        }
        case DISPLAY_TIMEOUT_SIG: {
            Table_displayOff(me);
            return Q_HANDLED();
        }
        case DISPLAY_IPADDR_SIG: {
            Table_displyIPAddr(me, ((TextEvt *)e)->text);
            return Q_HANDLED();
        }
        case DISPLAY_CGI_SIG: {
            Table_displyCgiText(me, ((TextEvt *)e)->text);
            return Q_HANDLED();
        }
        case DISPLAY_UDP_SIG: {
            TextEvt *te;

            Table_displyUdpText(me, ((TextEvt *)e)->text);
            ++me->udpCtr;

            te = Q_NEW(TextEvt, SEND_UDP_SIG);
            snprintf(te->text, Q_DIM(te->text), "%s-%d",
                      Q_EVT_CAST(TextEvt)->text, (int)me->udpCtr);
            QACTIVE_POST(AO_LwIPMgr, (QEvt *)te, me); /* post directly */

            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

/* helper functions for the display ........................................*/
/* include the correct OLED display implementation... */
void Table_displayInit(Table *me) {
    RIT128x96x4Init(1000000);
    RIT128x96x4StringDraw("QP-lwIP Demo",       4*6, 0*8,  9);
    RIT128x96x4StringDraw("IP :",               0*6, 2*8,  5);
    RIT128x96x4StringDraw("DPP: 0 ,1 ,2 ,3 ,4", 0*6, 4*8,  5);
    RIT128x96x4StringDraw("CGI:",               0*6, 6*8,  5);
    RIT128x96x4StringDraw("UDP:",               0*6, 8*8,  5);
    RIT128x96x4StringDraw("state-machine.com",  2*6,10*8,  9);
    me->displayOn = 1;
}
/*..........................................................................*/
static void Table_displayOn(Table *me) {
    QTimeEvt_rearm(&me->te_DISPLAY_TIMEOUT, DISPLAY_TIMEOUT);
    if (!me->displayOn) {
        me->displayOn = 1;
        RIT128x96x4DisplayOn();
    }
}
/*..........................................................................*/
static void Table_displayOff(Table *me) {
    if (me->displayOn) {
        me->displayOn = 0;
        RIT128x96x4DisplayOff();
    }
}
/*..........................................................................*/
static void Table_displayPhilStat(Table *me, uint8_t n, char const *stat) {
    if (me->displayOn) {
        char str[2];
        str[0] = stat[0];
        str[1] = '\0';
        RIT128x96x4StringDraw(str, (6*6 + 3*6*n), 4*8, 15);
    }
    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
static void Table_displyIPAddr(Table *me, char const *ip_addr) {
    Table_displayOn(me); /* make sure the screen is on */
    RIT128x96x4StringDraw("               ", 5*6, 2*8, 15); /* wipe */
    RIT128x96x4StringDraw(ip_addr,           5*6, 2*8, 15);
}
/*..........................................................................*/
void Table_displyCgiText(Table *me, char const *text) {
    Table_displayOn(me); /* make sure the screen is on */
    RIT128x96x4StringDraw("               ", 5*6, 6*8, 15); /* wipe */
    RIT128x96x4StringDraw(text,              5*6, 6*8, 15);

    QS_BEGIN_ID(CGI_TEXT, 0U) /* app-specific record */
        QS_STR(text); /* User text */
    QS_END()
}
/*..........................................................................*/
void Table_displyUdpText(Table *me, char const *text) {
    Table_displayOn(me); /* make sure the screen is on */
    RIT128x96x4StringDraw("               ", 5*6, 6*8, 15); /* wipe */
    RIT128x96x4StringDraw(text,              5*6, 6*8, 15);

    QS_BEGIN_ID(UDP_TEXT, 0U) /* app-specific record */
        QS_STR(text); /* User text */
    QS_END()
}

