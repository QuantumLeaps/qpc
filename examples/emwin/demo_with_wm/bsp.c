/*****************************************************************************
* Product: BSP for emWin/uC/GUI, Win32 simulation
* Last updated for version 6.3.6
* Last updated on  2018-10-03
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2002-2018 Quantum Leaps, LLC. All rights reserved.
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
* https://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

#include "GUI.h"
#include "DIALOG.h"
#include "SIM.h"

#include <windows.h>
#include <stdio.h>
#include <time.h>

Q_DEFINE_THIS_FILE

/* local variables ---------------------------------------------------------*/
#ifdef Q_SPY
    static uint8_t l_running;
    static uint8_t const l_clock_tick = 0U;
    static uint8_t const l_simHardKey = 0U;
    static uint8_t const l_MOUSE_StoreState = 0U;
    static SOCKET  l_sock = INVALID_SOCKET;
#endif

/*..........................................................................*/
static void simHardKey(int keyIndex, int keyState) {
    static const QEvent keyEvt[] = {
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
    MouseEvt *pe = Q_NEW(MouseEvt, MOUSE_CHANGE_SIG);
    pe->xPos = pState->x;
    pe->yPos = pState->y;
    pe->buttonStates = pState->Pressed;
    QACTIVE_POST(AO_Table, (QEvent *)pe, &l_MOUSE_StoreState);
}
/*..........................................................................*/
void BSP_init(void) {
    int n = SIM_HARDKEY_GetNum();/*initialize hardkeys for emWin simulation */
    for (n = n - 1; n >= 0; --n) {
        SIM_HARDKEY_SetCallback(n, &simHardKey);
    }

#ifdef Q_SPY
    {
        char const *hostAndPort = SIM_GetCmdLine();
        if (hostAndPort == NULL) { /* port unspecified? */
            hostAndPort = "localhost:6601";
        }
        if (!QS_INIT(hostAndPort)) {
            MessageBox(NULL, "Failed to open the TCP/IP socket for QS output",
                       "QS Socket Failure", MB_TASKMODAL | MB_OK);
            return;
        }
    }
#endif
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
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    char str[256];

    QF_CRIT_ENTRY(dummy);              /* make sure nothing else is running */
    sprintf(str, "%s:%d", file, line);
    MessageBox(NULL, str, "Assertion Failure", MB_TASKMODAL | MB_OK);
    QF_stop();    /* terminate the QF, causes termination of the MainTask() */
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY /* define QS callbacks */

/*..........................................................................*/
/*! callback function to execute user commands */
void QS_onCommand(uint8_t cmdId,
    uint32_t param1, uint32_t param2, uint32_t param3)
{
    switch (cmdId) {
    case 0U: {
        break;
    }
    default:
        break;
    }

    /* unused parameters */
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif /* Q_SPY */
/*--------------------------------------------------------------------------*/
