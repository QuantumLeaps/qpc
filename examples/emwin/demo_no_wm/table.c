/*****************************************************************************
* Product: DPP example with emWin/uC/GUI, NO Window Manager
* Last Updated for Version: 6.2.0
* Date of the Last Update:  2018-03-16
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
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

#include "GUI.h"
#include "DIALOG.h"
//#include "SIM.h"

Q_DEFINE_THIS_FILE

/* Active object class -----------------------------------------------------*/
typedef struct TableTag {
    QActive super;
    uint8_t fork[N_PHILO];
    uint8_t isHungry[N_PHILO];
} Table;

static QState Table_initial(Table *me, QEvt const *e);
static QState Table_ready  (Table *me, QEvt const *e);
static QState Table_serving(Table *me, QEvt const *e);
static QState Table_paused (Table *me, QEvt const *e);

#define RIGHT(n_) ((uint8_t)(((n_) + (N_PHILO - 1)) % N_PHILO))
#define LEFT(n_)  ((uint8_t)(((n_) + 1) % N_PHILO))
enum ForkState { FREE, USED };

/* Local objects -----------------------------------------------------------*/
static Table l_table; /* the single instance of the Table active object */

#ifdef Q_SPY
enum QSUserRecords {
    PHILO_STAT = QS_USER,
    TABLE_STAT
};
#endif

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_Table = (QActive *)&l_table; /* "opaque" AO pointer */

/* GUI definition ==========================================================*/
enum YCoord {
    PHILO_0_Y =   0,
    PHILO_1_Y =  20,
    PHILO_2_Y =  40,
    PHILO_3_Y =  60,
    PHILO_4_Y =  80,
    TABLE_Y   = 100,
    STATE_X   = 100
};

static int l_xOrg = 80;
static int l_yOrg = 60;
static char const l_thinking[] = "thinking";
static char const *l_philoStat[N_PHILO] = {
    l_thinking, l_thinking, l_thinking, l_thinking, l_thinking
};
static int const l_philoY[N_PHILO] = {
    0, 20, 40, 60, 80
};
static char const *l_tableState = " ";
static int const l_tableY = 100;

/*..........................................................................*/
static void displyPhilStat(uint8_t n, char const *stat) {
    GUI_SetBkColor(GUI_GRAY);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font13_ASCII);

    l_philoStat[n] = stat;
    GUI_DispStringAt(stat,  l_xOrg + STATE_X, l_yOrg + l_philoY[n]);

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
static void displyTableStat(char const *stat) {
    GUI_SetBkColor(GUI_GRAY);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font13_ASCII);

    l_tableState = stat;
    GUI_DispStringAt(stat,  l_xOrg + STATE_X, l_yOrg + l_tableY);

    QS_BEGIN_ID(TABLE_STAT, AO_Table->prio) /* app-specific record */
        QS_STR(stat); /* Philosopher status */
    QS_END()
}

/*..........................................................................*/
static void renderDppScreen(void) {
    GUI_SetBkColor(GUI_GRAY);
    GUI_Clear();
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_DispStringHCenterAt("Dining Philosophers - Demo", 160, 5);
    GUI_SetFont(&GUI_Font13_ASCII);

    GUI_DispStringAt("Philosopher 0",  l_xOrg, l_yOrg + l_philoY[0]);
    GUI_DispStringAt("Philosopher 1",  l_xOrg, l_yOrg + l_philoY[1]);
    GUI_DispStringAt("Philosopher 2",  l_xOrg, l_yOrg + l_philoY[2]);
    GUI_DispStringAt("Philosopher 3",  l_xOrg, l_yOrg + l_philoY[3]);
    GUI_DispStringAt("Philosopher 4",  l_xOrg, l_yOrg + l_philoY[4]);
    GUI_DispStringAt("Table",          l_xOrg, l_yOrg + l_tableY);

    displyPhilStat(0, l_philoStat[0]);
    displyPhilStat(1, l_philoStat[1]);
    displyPhilStat(2, l_philoStat[2]);
    displyPhilStat(3, l_philoStat[3]);
    displyPhilStat(4, l_philoStat[4]);
}
/*..........................................................................*/
static void moveDppScreen(int dx, int dy) {
    l_xOrg += dx;
    l_yOrg += dy;

    renderDppScreen();
}
/*..........................................................................*/
void Table_ctor(void) {
    uint8_t n;
    Table *me = &l_table;

    QActive_ctor(&me->super, (QStateHandler)&Table_initial);

    for (n = 0; n < N_PHILO; ++n) {
        me->fork[n] = FREE;
        me->isHungry[n] = 0;
    }
}

/* Table HSM ===============================================================*/
QState Table_initial(Table *me, QEvt const *e) {
    (void)e;        /* suppress the compiler warning about unused parameter */

    QS_OBJ_DICTIONARY(&l_table);
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&Table_initial);
    QS_FUN_DICTIONARY(&Table_serving);

    QS_SIG_DICTIONARY(DONE_SIG,      0); /* global signals */
    QS_SIG_DICTIONARY(EAT_SIG,       0);
    QS_SIG_DICTIONARY(PAUSE_SIG, 0);

    QS_SIG_DICTIONARY(HUNGRY_SIG,    me); /* signal just for Table */

    GUI_Init(); /* initialize the embedded GUI */

    QActive_subscribe((QActive *)me, DONE_SIG);

    return Q_TRAN(&Table_ready);
}
/*..........................................................................*/
QState Table_ready(Table *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            renderDppScreen();
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            return Q_TRAN(&Table_serving);
        }

        /* ... hardkey events ... */
        case KEY_LEFT_REL_SIG: { /* hardkey LEFT released */
            moveDppScreen(-5, 0);
            return Q_HANDLED();
        }
        case KEY_RIGHT_REL_SIG: { /* hardkey RIGHT released */
            moveDppScreen(5, 0);
            return Q_HANDLED();
        }
        case KEY_DOWN_REL_SIG: {  /* hardkey DOWN released */
            moveDppScreen(0, 5);
            return Q_HANDLED();
        }
        case KEY_UP_REL_SIG: {    /* hardkey UP released */
            moveDppScreen(0, -5);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState Table_serving(Table *me, QEvt const *e) {
    uint8_t n, m;
    TableEvt *pe;

    switch (e->sig) {
        case Q_ENTRY_SIG: {
            displyTableStat("serving");
            for (n = 0; n < N_PHILO; ++n) { /* give permissions to eat... */
                if (me->isHungry[n]
                    && (me->fork[LEFT(n)] == FREE)
                        && (me->fork[n] == FREE))
                {
                    me->fork[LEFT(n)] = me->fork[n] = USED;
                    pe = Q_NEW(TableEvt, EAT_SIG);
                    pe->philoNum = n;
                    QF_PUBLISH((QEvt *)pe, me);
                    me->isHungry[n] = 0;
                    displyPhilStat(n, "eating  ");
                }
            }
            return Q_HANDLED();
        }
        case HUNGRY_SIG: {
            n = ((TableEvt *)e)->philoNum;
            Q_ASSERT(n < N_PHILO && !me->isHungry[n]);
            displyPhilStat(n, "hungry  ");
            m = LEFT(n);
            if (me->fork[m] == FREE && me->fork[n] == FREE) {
                me->fork[m] = me->fork[n] = USED;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = n;
                QF_PUBLISH((QEvt *)pe, me);
                displyPhilStat(n, "eating  ");
            }
            else {
                me->isHungry[n] = 1;
            }
            return Q_HANDLED();
        }
        case DONE_SIG: {
            n = ((TableEvt *)e)->philoNum;
            Q_ASSERT(n < N_PHILO);
            displyPhilStat(n, "thinking");
            me->fork[LEFT(n)] = me->fork[n] = FREE;
            m = RIGHT(n); /* check the right neighbor */
            if (me->isHungry[m] && me->fork[m] == FREE) {
                me->fork[n] = me->fork[m] = USED;
                me->isHungry[m] = 0;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = m;
                QF_PUBLISH((QEvt *)pe, me);
                displyPhilStat(m, "eating  ");
            }
            m = LEFT(n); /* check the left neighbor */
            n = LEFT(m);
            if (me->isHungry[m] && me->fork[n] == FREE) {
                me->fork[m] = me->fork[n] = USED;
                me->isHungry[m] = 0;
                pe = Q_NEW(TableEvt, EAT_SIG);
                pe->philoNum = m;
                QF_PUBLISH((QEvt *)pe, me);
                displyPhilStat(m, "eating  ");
            }
            return Q_HANDLED();
        }
        case PAUSE_SIG:              /* "Toggle" button pressed */
        case KEY_CENTER_PRESS_SIG: { /* hardkey CENTER pressed */
            return Q_TRAN(&Table_paused);
        }
    }
    return Q_SUPER(&Table_ready);
}
/*..........................................................................*/
QState Table_paused(Table *me, QEvt const *e) {
    uint8_t n;

    switch (e->sig) {
        case Q_ENTRY_SIG: {
            displyTableStat("paused");
            return Q_HANDLED();
        }
        case HUNGRY_SIG: {
            n = ((TableEvt *)e)->philoNum;
            Q_ASSERT(n < N_PHILO && !me->isHungry[n]);
            displyPhilStat(n, "hungry  ");
            me->isHungry[n] = 1;
            return Q_HANDLED();
        }
        case DONE_SIG: {
            n = ((TableEvt *)e)->philoNum;
            Q_ASSERT(n < N_PHILO);
            displyPhilStat(n, "thinking");
            me->fork[LEFT(n)] = me->fork[n] = FREE;
            return Q_HANDLED();
        }
        case PAUSE_SIG:            /* "Toggle" button pressed */
        case KEY_CENTER_REL_SIG: { /* hardkey CENTER released */
            return Q_TRAN(&Table_serving);
        }
    }
    return Q_SUPER(&Table_ready);
}
