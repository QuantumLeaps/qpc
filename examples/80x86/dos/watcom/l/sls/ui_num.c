/*****************************************************************************
* Product: User Interface with State-Local Storage Example
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 24, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
#include "qp_port.h"                        /* the port of the QP framework */
#include "num_ent.h"
#include "ui.h"
#include "video.h"

#include <stdio.h>
#include <math.h>

/* Local objects -----------------------------------------------------------*/
static QEvt const l_clear_evt = { C_SIG, 0, 0 };

#define NUM_ENTRY_X      13
#define NUM_ENTRY_Y      13
#define NUM_ENTRY_COLOR  (VIDEO_FGND_YELLOW)

/*..........................................................................*/
QState UI_num_handler(UI_num *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
                                 /* send object dictionaries for UI objects */
            QS_OBJ_DICTIONARY(&me->num_entry);

                                     /* instantiate the state-local objects */
            NumEntry_ctor(&me->num_entry);
                            /* take the initial transition in the component */
            QHsm_init((QHsm *)&me->num_entry, (QEvt *)0);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            me->super.history = QHsm_state((QHsm const *)me);/*save history */
                                      /* destroy the state-local objects... */
            NumEntry_xtor(&me->num_entry);
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            return Q_TRAN(&UI_num_sd_handler);
        }
        case HELP_SIG: {
            return Q_TRAN(&UI_help_handler);                 /* Help screen */
        }
    }
    return Q_SUPER(&UI_top_handler);
}
/*..........................................................................*/
QState UI_num_sd_handler(UI_num_sd *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            uint8_t c;
            static char const * const help_sd[] = {
                "Standard Deviation:         ",
                "Find the mean value and the ",
                " root-mean-square (RMS)     ",
                "deviation of n data samples ",
                "xi, where i = 1..n.         ",
                "Mean value <x> is calculated",
                "as follows:                 ",
                "<x> = Sum(xi)/n;            ",
                "Two RMS estimatators are    ",
                "provided:                   ",
                "sig(n) =                    ",
                "   sqrt(Sum(xi-<x>)**2 / n);",
                "sig(n-1) =                  ",
                "sqrt(Sum(xi-<x>)**2 / (n-1))"
            };
                                     /* instantiate the state-local objects */
            me->super.super.help_text = help_sd;
            me->super.super.help_len  = Q_DIM(help_sd);
            me->n      = 0.0;
            me->sum    = 0.0;
            me->sum_sq = 0.0;

            Video_printStrAt(2, 10, VIDEO_FGND_BLACK,
                "Screen 1: Standard Deviation     ");
            Video_clearRect( 0, 11, 35, 23, VIDEO_BGND_BLUE);
            Video_clearRect(35, 11, 80, 23, VIDEO_BGND_BLACK);

            c = VIDEO_FGND_LIGHT_GRAY;
            Video_printStrAt(36, 12, c,
                "Press '-'        to enter a negative number");
            Video_printStrAt(36, 13, c,
                "Press '0' .. '9' to enter a digit");
            Video_printStrAt(36, 14, c,
                "Press '.'        to enter the decimal point");
            Video_printStrAt(36, 15, c,
                "Press <Enter>    to enter the data sample");
            Video_printStrAt(36, 16, c,
                "Press 'e' or 'E' to Cancel last entry");
            Video_printStrAt(36, 17, c,
                "Press 'c' or 'C' to Cancel the data set");

            c = VIDEO_FGND_WHITE;
            Video_printStrAt(36, 20, c,
                "Press UP-arrow   for previous screen");
            Video_printStrAt(36, 21, c,
                "Press DOWN-arrow for next screen");
            Video_printStrAt(36, 22, c,
                "Press F1         for help");

            Video_clearRect(NUM_ENTRY_X, NUM_ENTRY_Y,
                NUM_ENTRY_X + NUM_STR_WIDTH, NUM_ENTRY_Y + 1,
                VIDEO_BGND_BLACK);
            Video_drawRect(NUM_ENTRY_X - 1, NUM_ENTRY_Y - 1,
                NUM_ENTRY_X + NUM_STR_WIDTH + 1, NUM_ENTRY_Y + 2,
                VIDEO_FGND_WHITE, 2);

            NumEntry_config(&me->super.num_entry, NUM_ENTRY_X, NUM_ENTRY_Y,
                            NUM_ENTRY_COLOR);
            QHsm_dispatch((QHsm *)&me->super.num_entry, &l_clear_evt);

            c = VIDEO_FGND_WHITE;                                 /* labels */
            Video_printStrAt(NUM_ENTRY_X - 1, NUM_ENTRY_Y + 4, c,
                 "n        =");
            Video_printStrAt(NUM_ENTRY_X - 1, NUM_ENTRY_Y + 5, c,
                 "<x>      =");
            Video_printStrAt(NUM_ENTRY_X - 1, NUM_ENTRY_Y + 6, c,
                "sig(n)   =");
            Video_printStrAt(NUM_ENTRY_X - 1, NUM_ENTRY_Y + 7, c,
                "sig(n-1) =");

            c = VIDEO_FGND_YELLOW;                                /* values */
            Video_printStrAt(NUM_ENTRY_X + 10, NUM_ENTRY_Y + 4, c,
                "0           ");
            Video_printStrAt(NUM_ENTRY_X + 10, NUM_ENTRY_Y + 5, c,
                "N/A         ");
            Video_printStrAt(NUM_ENTRY_X + 10, NUM_ENTRY_Y + 6, c,
                "N/A         ");
            Video_printStrAt(NUM_ENTRY_X + 10, NUM_ENTRY_Y + 7, c,
                "N/A         ");

            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
                                      /* destroy the state-local objects... */
            /* noting to destroy */
            return Q_HANDLED();
        }
        case C_SIG: {
            return Q_TRAN(&UI_num_sd_handler);        /* transition-to-self */
        }
        case CE_SIG: {
            QHsm_dispatch((QHsm *)&me->super.num_entry, &l_clear_evt);
            return Q_HANDLED();
        }
        case UP_SIG: {
            return Q_TRAN(&UI_num_lr_handler);   /* Liner Regression screen */
        }
        case DOWN_SIG: {
            return Q_TRAN(&UI_num_lr_handler);   /* Liner Regression screen */
        }
        case NEG_SIG:
        case DIGIT_0_SIG:
        case DIGIT_1_9_SIG:
        case POINT_SIG: {
            QHsm_dispatch((QHsm *)&me->super.num_entry, e);
            return Q_HANDLED();
        }
        case ENTER_SIG: {
            double tmp = NumEntry_get(&me->super.num_entry);
            char buf[14];

            me->n      += 1.0;
            me->sum    += tmp;
            me->sum_sq += tmp*tmp;

            sprintf(buf, "%-12.6g", me->n);
            Video_printStrAt(NUM_ENTRY_X + 10, NUM_ENTRY_Y + 4,
                              VIDEO_FGND_YELLOW, buf);

            tmp = me->sum / me->n;                                   /* <x> */
            sprintf(buf, "%-12.6g", tmp);
            Video_printStrAt(NUM_ENTRY_X + 10, NUM_ENTRY_Y + 5,
                              VIDEO_FGND_YELLOW, buf);

            tmp = me->sum_sq / me->n - tmp*tmp;
            if (tmp >= 0.0) {                                   /* sigma(n) */
                tmp = sqrt(tmp);
                sprintf(buf, "%-12.6g", tmp);
                Video_printStrAt(NUM_ENTRY_X + 10, NUM_ENTRY_Y + 6,
                                  VIDEO_FGND_YELLOW, buf);
                if (me->n > 1.0) {                            /* sigma(n-1) */
                     tmp *= sqrt(me->n/(me->n - 1.0));
                     sprintf(buf, "%-12.6g", tmp);
                     Video_printStrAt(NUM_ENTRY_X + 10, NUM_ENTRY_Y + 7,
                                       VIDEO_FGND_YELLOW, buf);
                }
            }
            QHsm_dispatch((QHsm *)&me->super.num_entry, &l_clear_evt);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&UI_num_handler);
}
