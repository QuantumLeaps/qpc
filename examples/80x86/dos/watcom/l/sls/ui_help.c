/*****************************************************************************
* Product: User Interface with State-Local Storage Example
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

#define HELP_X      3
#define HELP_Y      14
#define HELP_DX     28
#define HELP_DY     5

/*..........................................................................*/
static void printHelp(char const * const *txt) {
    uint8_t y;
    for (y = 0; y < HELP_DY; ++y) {
        Video_printStrAt(HELP_X, HELP_Y + y, VIDEO_FGND_YELLOW, txt[y]);
    }
}

/*..........................................................................*/
QState UI_help_handler(UI_help *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
                                     /* instantiate the state-local objects */
            Video_printStrAt(2, 10, VIDEO_FGND_BLACK,
                "Screen 0: Help                   ");
            Video_clearRect( 0, 11, 35, 23, VIDEO_BGND_BLUE);
            Video_clearRect(35, 11, 80, 23, VIDEO_BGND_BLACK);

            Video_printStrAt(36, 12, VIDEO_FGND_LIGHT_GRAY,
                "Press DOWN-Arrow to scroll down");
            Video_printStrAt(36, 13, VIDEO_FGND_LIGHT_GRAY,
                "Press UP-Arrow   to scroll up");

            Video_printStrAt(36, 20, VIDEO_FGND_WHITE,
                "Press F1         to return to last screen");

            Video_clearRect(HELP_X - 1, HELP_Y,
                HELP_X + HELP_DX + 1, HELP_Y + HELP_DY, VIDEO_BGND_BLACK);
            Video_drawRect (HELP_X - 2, HELP_Y - 1,
                HELP_X + HELP_DX + 2, HELP_Y + HELP_DY + 1,
                VIDEO_FGND_WHITE,2);

            me->help_line = 0;
            printHelp(me->super.help_text + me->help_line);

            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
                                      /* destroy the state-local objects... */
            return Q_HANDLED();
        }
        case DOWN_SIG: {
            if (me->help_line + HELP_DY < me->super.help_len) {
                ++me->help_line;
            }
            printHelp(me->super.help_text + me->help_line);
            return Q_HANDLED();
        }
        case UP_SIG: {
            if (me->help_line > 0) {
                --me->help_line;
            }
            printHelp(me->super.help_text + me->help_line);
            return Q_HANDLED();
        }
        case HELP_SIG: {
            return Q_TRAN(me->super.history); /* go back to the last screen */
        }
    }
    return Q_SUPER(&UI_top_handler);
}
