/*****************************************************************************
* Product: Console-based BSP, MinGW
* Last updated for version 5.6.0
* Last updated on  2015-12-18
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* http://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#include "qpc.h"
#include "clock.h"
#include "bsp.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Q_DEFINE_THIS_FILE

/*..........................................................................*/
void BSP_init(int argc, char *argv[]) {
    (void)argc; /* unused parameter */
    (void)argv; /* unused parameter */
}
/*..........................................................................*/
void BSP_showMsg(char_t const *str) {
    printf(str);
    printf("\n");
    fflush(stdout);
}
/*..........................................................................*/
void BSP_showTime24H(char_t const *str, uint32_t time, uint32_t base) {
    printf(str);
    printf("%02d:%02d\n", (int)(time / base), (int)(time % base));
    fflush(stdout);
}
/*..........................................................................*/
void BSP_showTime12H(char_t const *str, uint32_t time, uint32_t base) {
    uint32_t h = time / base;

    printf(str);
    printf("%02d:%02d %s\n", (h % 12) ? (h % 12) : 12,
           time % base, (h / 12) ? "PM" : "AM");
    fflush(stdout);
}
/*..........................................................................*/
void QF_onStartup(void) {
    QF_setTickRate(BSP_TICKS_PER_SEC); /* set the desired tick rate */
}
/*..........................................................................*/
void QF_onCleanup(void) {
    printf("\nBye! Bye!\n");
}
/*..........................................................................*/
void QF_onClockTick(void) {
    QF_TICK(&l_clock_tick); /* perform the QF clock tick processing */
    if (_kbhit()) { /* any key pressed? */
        BSP_onKeyboardInput(_getch());
    }
}
/*..........................................................................*/
void BSP_onKeyboardInput(uint8_t key) {
    switch (key) {
        case 'o': { /* 'o': Alarm on event? */
            QACTIVE_POST(APP_alarmClock,
                         Q_NEW(QEvt, ALARM_ON_SIG), (void *)0);
            break;
        }
        case 'f': { /* 'f': Alarm off event? */
            QACTIVE_POST(APP_alarmClock,
                         Q_NEW(QEvt, ALARM_OFF_SIG), (void *)0);
            break;
        }
        case '1':   /* '1' */
        case '2':   /* '2' */
        case '3':   /* '3' */
        case '4':   /* '4' */
        case '5':   /* '5' */
        case '6':   /* '6' */
        case '7':   /* '7' */
        case '8':   /* '8' */
        case '9': { /* '9' */
            SetEvt *e = Q_NEW(SetEvt, ALARM_SET_SIG);
            e->digit = (uint8_t)(key - '0');
            QACTIVE_POST(APP_alarmClock, (QEvt *)e, (void *)0);
            break;
        }
        case '0': { /* '0' */
            SetEvt *e = Q_NEW(SetEvt, ALARM_SET_SIG);
            e->digit = 0;
            QACTIVE_POST(APP_alarmClock, (QEvt *)e, (void *)0);
            break;
        }
        case 'a': { /* 'a': Clock 12H event? */
            QACTIVE_POST(APP_alarmClock,
                         Q_NEW(QEvt, CLOCK_12H_SIG), (void *)0);
            break;
        }
        case 'b': { /* 'b': Clock 24H event? */
            QACTIVE_POST(APP_alarmClock,
                         Q_NEW(QEvt, CLOCK_24H_SIG), (void *)0);
            break;
        }
        case '\33': { /* ESC pressed? */
            QACTIVE_POST(APP_alarmClock,
                         Q_NEW(QEvt, TERMINATE_SIG), (void *)0);
            break;
        }
    }
}

/*..........................................................................*/
void Q_onAssert(char const * const file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    QF_stop();
}

