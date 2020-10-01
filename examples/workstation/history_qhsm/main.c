/*****************************************************************************
* Product: History Example, Win32
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"
#include "history.h"

#include "safe_std.h" /* portable "safe" <stdio.h>/<string.h> facilities */
#include <stdlib.h>   /* for exit() */

Q_DEFINE_THIS_FILE

/*..........................................................................*/
int main() {
    QF_init();
    QF_onStartup();

    PRINTF_S("History state pattern\nQP version: %s\n"
           "Press 'o' to OPEN  the door\n"
           "Press 'c' to CLOSE the door\n"
           "Press 't' to start TOASTING\n"
           "Press 'b' to start BAKING\n"
           "Press 'f' to turn the oven OFF\n"
           "Press ESC to quit...\n",
           QP_versionStr);

    /* instantiate the ToastOven HSM and trigger the initial transition */
    ToastOven_ctor();
    QHSM_INIT(the_oven, (void *)0, 0U);

    for (;;) {
        QEvt e;
        uint8_t c;

        PRINTF_S("\n", "");
        c = (uint8_t)QF_consoleWaitForKey();
        PRINTF_S("%c: ", (c >= ' ') ? c : 'X');

        switch (c) {
            case 'o':  e.sig = OPEN_SIG;        break;
            case 'c':  e.sig = CLOSE_SIG;       break;
            case 't':  e.sig = TOAST_SIG;       break;
            case 'b':  e.sig = BAKE_SIG;        break;
            case 'f':  e.sig = OFF_SIG;         break;
            case 0x1B: e.sig = TERMINATE_SIG;   break;
        }

        /* dispatch the event into the state machine */
        QHSM_DISPATCH(the_oven,  &e, 0U);
    }

    QF_onCleanup();
    return 0;
}

/*..........................................................................*/
void QF_onStartup(void) {
    QF_consoleSetup();
}
/*..........................................................................*/
void QF_onCleanup(void) {
    QF_consoleCleanup();
}
/*..........................................................................*/
void QF_onClockTick(void) {
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char_t const * const file, int_t const line) {
    FPRINTF_S(stderr, "Assertion failed in %s, line %d", file, line);
    QF_onCleanup();
    exit(-1);
}
