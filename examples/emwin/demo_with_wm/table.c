/*****************************************************************************
* Product: DPP examplefor SEGGER emWin (version 6.10), Win32 simulation
* Last updated for version 6.8.0
* Last updated on  2020-01-22
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
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

#include "GUI.h"
#include "GUI_SIM.h"
#include "WM.h"      /* emWin Windows Manager */
#include "DIALOG.h"

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
    static uint8_t const l_onDialogGUI = 0U;
#endif

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_Table = (QActive *)&l_table; /* "opaque" AO pointer */

/* GUI definition ==========================================================*/
static WM_HWIN l_hDlg;
static WM_CALLBACK *l_cb_WM_HBKWIN;

static const GUI_WIDGET_CREATE_INFO l_dialog[] = {
    { &FRAMEWIN_CreateIndirect, "Dining Philosopher Problem",
        0,  30,  30, 260, 180, FRAMEWIN_CF_MOVEABLE },
    { &TEXT_CreateIndirect, "Philosopher 0",
        GUI_ID_TEXT9,  50,  10, 0, 0, TEXT_CF_LEFT },
    { &TEXT_CreateIndirect, "Philosopher 1",
        GUI_ID_TEXT9,  50,  30, 0, 0, TEXT_CF_LEFT },
    { &TEXT_CreateIndirect, "Philosopher 2",
        GUI_ID_TEXT9,  50,  50, 0, 0, TEXT_CF_LEFT },
    { &TEXT_CreateIndirect, "Philosopher 3",
        GUI_ID_TEXT9,  50,  70, 0, 0, TEXT_CF_LEFT },
    { &TEXT_CreateIndirect, "Philosopher 4",
        GUI_ID_TEXT9,  50,  90, 0, 0, TEXT_CF_LEFT },
    { &TEXT_CreateIndirect, "Table",
        GUI_ID_TEXT9,  50, 110, 0, 0, TEXT_CF_LEFT },

    { &TEXT_CreateIndirect, "thinking",
        GUI_ID_TEXT0, 130,  10, 0, 0, TEXT_CF_LEFT },
    { &TEXT_CreateIndirect, "thinking",
        GUI_ID_TEXT1, 130,  30, 0, 0, TEXT_CF_LEFT },
    { &TEXT_CreateIndirect, "thinking",
        GUI_ID_TEXT2, 130,  50, 0, 0, TEXT_CF_LEFT },
    { &TEXT_CreateIndirect, "thinking",
        GUI_ID_TEXT3, 130,  70, 0, 0, TEXT_CF_LEFT },
    { &TEXT_CreateIndirect, "thinking",
        GUI_ID_TEXT4, 130,  90, 0, 0, TEXT_CF_LEFT },
    { &TEXT_CreateIndirect, "serving",
        GUI_ID_TEXT5, 130, 110, 0, 0, TEXT_CF_LEFT },

    { &BUTTON_CreateIndirect,"PAUSE",
        GUI_ID_BUTTON0,    160, 130, 80, 30 }
};

/*..........................................................................*/
static void onMainWndGUI(WM_MESSAGE* pMsg) {
    switch (pMsg->MsgId) {
        case WM_PAINT: {
            GUI_SetBkColor(GUI_GRAY);
            GUI_Clear();
            GUI_SetColor(GUI_BLACK);
            GUI_SetFont(&GUI_Font24_ASCII);
            GUI_DispStringHCenterAt("Dining Philosophers - Demo", 160, 5);
            break;
        }
        default: {
            WM_DefaultProc(pMsg);
            break;
        }
    }
}
/*..........................................................................*/
static void onDialogGUI(WM_MESSAGE * pMsg) {
    switch (pMsg->MsgId) {
        case WM_INIT_DIALOG: {
            break;
        }
        case WM_NOTIFY_PARENT: {
            switch (pMsg->Data.v) {
                case WM_NOTIFICATION_RELEASED: { /* react only if released */
                    switch (WM_GetId(pMsg->hWinSrc)) {
                        case GUI_ID_BUTTON0: {
                            /* static PAUSE event for the Table AO */
                            static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U };
                            QACTIVE_POST(AO_Table, &pauseEvt, &l_onDialogGUI);
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
        default: {
            WM_DefaultProc(pMsg);
            break;
        }
    }
}
/*..........................................................................*/
static void displyPhilStat(uint8_t n, char const *stat) {
    TEXT_SetText(WM_GetDialogItem(l_hDlg, GUI_ID_TEXT0 + n), stat);
    WM_Exec(); /* update the screen and invoke WM callbacks */
}
/*..........................................................................*/
static void displyTableStat(char const *stat) {
    TEXT_SetText(WM_GetDialogItem(l_hDlg, GUI_ID_TEXT5), stat);
    WM_Exec();  /* update the screen and invoke WM callbacks */
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
    (void)e; /* unused parameter */

    GUI_Init(); /* initialize the embedded GUI */

    QActive_subscribe((QActive *)me, DONE_SIG);
    return Q_TRAN(&Table_ready);
}
/*..........................................................................*/
QState Table_ready(Table *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            l_cb_WM_HBKWIN = WM_SetCallback(WM_HBKWIN, &onMainWndGUI);

            /* create the diaglog box and return right away... */
            l_hDlg = GUI_CreateDialogBox(l_dialog, GUI_COUNTOF(l_dialog),
                                         &onDialogGUI, 0, 0, 0);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            GUI_EndDialog(l_hDlg, 0);
            WM_SetCallback(WM_HBKWIN, l_cb_WM_HBKWIN);
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            return Q_TRAN(&Table_serving);
        }

        case MOUSE_CHANGE_SIG: { /* mouse change (move/touch or click) event */
            GUI_PID_STATE mouse;
            mouse.x = Q_EVT_CAST(MouseEvt)->x;
            mouse.y = Q_EVT_CAST(MouseEvt)->y;
            mouse.Pressed = Q_EVT_CAST(MouseEvt)->Pressed;
            mouse.Layer   = Q_EVT_CAST(MouseEvt)->Layer;
            GUI_PID_StoreState(&mouse);/* update the state of the Mouse PID */
            return Q_HANDLED();
        }

        /* ... hardkey events ... */
        case KEY_LEFT_REL_SIG: { /* hardkey LEFT released */
            WM_MoveWindow(l_hDlg, -5, 0);
            WM_Exec(); /* update the screen and invoke WM callbacks */
            return Q_HANDLED();
        }
        case KEY_RIGHT_REL_SIG: { /* hardkey RIGHT released */
            WM_MoveWindow(l_hDlg, 5, 0);
            WM_Exec(); /* update the screen and invoke WM callbacks */
            return Q_HANDLED();
        }
        case KEY_DOWN_REL_SIG: { /* hardkey DOWN released */
            WM_MoveWindow(l_hDlg, 0, 5);
            WM_Exec();         /* update the screen and invoke WM callbacks */
            return Q_HANDLED();
        }
        case KEY_UP_REL_SIG: { /* hardkey UP released */
            WM_MoveWindow(l_hDlg, 0, -5);
            WM_Exec(); /* update the screen and invoke WM callbacks */
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
            for (n = 0; n < N_PHILO; ++n) {/* give permissions to eat... */
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
        case PAUSE_SIG: /* "Toggle" button pressed */
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
        case PAUSE_SIG: /* "Toggle" button pressed */
        case KEY_CENTER_REL_SIG: { /* hardkey CENTER released */
            return Q_TRAN(&Table_serving);
        }
    }
    return Q_SUPER(&Table_ready);
}

