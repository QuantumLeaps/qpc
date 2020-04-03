/*****************************************************************************
* Product: BSP for SEGGER emWin (version 6.10), Win32 simulation
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
#include "DIALOG.h"

#include "safe_std.h" /* portable "safe" <stdio.h>/<string.h> facilities */
#include <time.h>
#include <windows.h>

Q_DEFINE_THIS_FILE

/*..........................................................................*/
static void simHardKey(int keyIndex, int keyState) {
    static const QEvt keyEvt[] = {
        { KEY_UP_REL_SIG,       0 }, /* hardkey UP released */
        { KEY_UP_PRESS_SIG,     0 }, /* hardkey UP pressed */
        { KEY_RIGHT_REL_SIG,    0 }, /* hardkey RIGHT released */
        { KEY_RIGHT_PRESS_SIG,  0 }, /* hardkey RIGHT pressed */
        { KEY_CENTER_REL_SIG,   0 }, /* hardkey CENTER released */
        { KEY_CENTER_PRESS_SIG, 0 }, /* hardkey CENTER pressed */
        { KEY_LEFT_REL_SIG,     0 }, /* hardkey LEFT released */
        { KEY_LEFT_PRESS_SIG,   0 }, /* hardkey LEFT pressed */
        { KEY_DOWN_REL_SIG,     0 }, /* hardkey DOWN released */
        { KEY_DOWN_PRESS_SIG,   0 }, /* hardkey DOWN pressed */
        { KEY_POWER_REL_SIG,    0 }, /* hardkey POWER released */
        { KEY_POWER_PRESS_SIG,  0 }  /* hardkey POWER pressed */
    };
    /* do not overrun the array */
    Q_REQUIRE((keyIndex * 2) + keyState < Q_DIM(keyEvt));

         /* post the hardkey event to the Table active object (GUI manager) */
    QACTIVE_POST(AO_Table, &keyEvt[(keyIndex * 2) + keyState], &l_simHardKey);

    if ((keyIndex == 5) && (keyState == 0)) { /* hardkey POWER released? */
        QF_stop(); /* terminate the simulation */
    }
}
/*..........................................................................*/
void GUI_MOUSE_StoreState(const GUI_PID_STATE *pState) {
    MouseEvt* pe = Q_NEW(MouseEvt, MOUSE_CHANGE_SIG);
    pe->x = pState->x;
    pe->y = pState->y;
    pe->Pressed = pState->Pressed;
    pe->Layer   = pState->Layer;
    QACTIVE_POST(AO_Table, (QEvt *)pe, &l_MOUSE_StoreState);
}
/*..........................................................................*/
void BSP_init(void) {
    int n = SIM_HARDKEY_GetNum();/*initialize hardkeys for emWin simulation */
    for (n = n - 1; n >= 0; --n) {
        SIM_HARDKEY_SetCallback(n, &simHardKey);
    }
}
/*..........................................................................*/
void QF_onStartup(void) {
    QF_setTickRate(BSP_TICKS_PER_SEC, 50); /* set the desired tick rate */

    QS_RX_INPUT(); /* handle the QS-RX input */
    QS_OUTPUT();   /* handle the QS output */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QF_onClockTick(void) {
    QF_TICK(&l_clock_tick); /* perform the QF clock tick processing */
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char_t const * const file, int_t const loc) {
    char str[256];

    QF_CRIT_ENTRY(dummy); /* make sure nothing else is running */
    SNPRINTF_S(str, sizeof(str), "%s:%d", file, loc);
    MessageBox(NULL, str, "Assertion Failure", MB_TASKMODAL | MB_OK);
    QF_stop(); /* terminate the QF, causes termination of the MainTask() */
}

