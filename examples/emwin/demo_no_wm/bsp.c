/*****************************************************************************
* Product: BSP for emWin/uC/GUI, Win32 simulation
* Last Updated for Version: 5.1.1
* Date of the Last Update:  Nov 08, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
*                          https://state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
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
        { KEY_UP_REL_SIG,       0 },                 /* hardkey UP released */
        { KEY_UP_PRESS_SIG,     0 },                  /* hardkey UP pressed */
        { KEY_RIGHT_REL_SIG,    0 },              /* hardkey RIGHT released */
        { KEY_RIGHT_PRESS_SIG,  0 },               /* hardkey RIGHT pressed */
        { KEY_CENTER_REL_SIG,   0 },             /* hardkey CENTER released */
        { KEY_CENTER_PRESS_SIG, 0 },              /* hardkey CENTER pressed */
        { KEY_LEFT_REL_SIG,     0 },               /* hardkey LEFT released */
        { KEY_LEFT_PRESS_SIG,   0 },                /* hardkey LEFT pressed */
        { KEY_DOWN_REL_SIG,     0 },               /* hardkey DOWN released */
        { KEY_DOWN_PRESS_SIG,   0 },                /* hardkey DOWN pressed */
        { KEY_POWER_REL_SIG,    0 },              /* hardkey POWER released */
        { KEY_POWER_PRESS_SIG,  0 }                /* hardkey POWER pressed */
    };
                                                /* do not overrun the array */
    Q_REQUIRE((keyIndex * 2) + keyState < Q_DIM(keyEvt));

         /* post the hardkey event to the Table active object (GUI manager) */
    QACTIVE_POST(AO_Table, &keyEvt[(keyIndex * 2) + keyState], &l_simHardKey);

    if ((keyIndex == 5) && (keyState == 0)) {    /* hardkey POWER released? */
        QF_stop();                              /* terminate the simulation */
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
#ifdef Q_SPY
static DWORD WINAPI idleThread(LPVOID par) {/* signature for CreateThread() */
    (void)par;
    l_running = (uint8_t)1;
    while (l_running) {
        uint16_t nBytes = 1024;
        uint8_t const *block;
        QF_CRIT_ENTRY(dummy);
        block = QS_getBlock(&nBytes);
        QF_CRIT_EXIT(dummy);
        if (block != (uint8_t *)0) {
            send(l_sock, (char const *)block, nBytes, 0);
        }
        Sleep(10);                                      /* wait for a while */
    }
    return 0;                                             /* return success */
}
#endif
/*..........................................................................*/
void BSP_init(void) {
    int n = SIM_HARDKEY_GetNum();/*initialize hardkeys for emWin simulation */
    for (n = n - 1; n >= 0; --n) {
        SIM_HARDKEY_SetCallback(n, &simHardKey);
    }

#ifdef Q_SPY
    {
        HANDLE hIdle;
        char const *hostAndPort = SIM_GetCmdLine();
        if (hostAndPort != NULL) {                       /* port specified? */
            hostAndPort = "localhost:6601";
        }
        if (!QS_INIT(hostAndPort)) {
            MessageBox(NULL, "Failed to open the TCP/IP socket for QS output",
                       "QS Socket Failure", MB_TASKMODAL | MB_OK);
            return;
        }
        hIdle = CreateThread(NULL, 1024, &idleThread, (void *)0, 0, NULL);
        Q_ASSERT(hIdle != (HANDLE)0);             /* thread must be created */
        SetThreadPriority(hIdle, THREAD_PRIORITY_IDLE);
    }
#endif
}
/*..........................................................................*/
void QF_onStartup(void) {
    QF_setTickRate(BSP_TICKS_PER_SEC);         /* set the desired tick rate */
}
/*..........................................................................*/
void QF_onCleanup(void) {
#if Q_SPY
    l_running = (uint8_t)0;
#endif
}
/*..........................................................................*/
void QF_onClockTick(void) {
    QF_TICK(&l_clock_tick);         /* perform the QF clock tick processing */
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
#ifdef Q_SPY                                         /* define QS callbacks */

uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[1024];                /* 1K buffer for Quantum Spy */
    static WSADATA wsaData;
    char host[64];
    char const *src;
    char *dst;
    USHORT port = 6601;                                     /* default port */
    ULONG ioctl_opt = 1;
    struct sockaddr_in servAddr;
    struct hostent *server;

    QS_initBuf(qsBuf, sizeof(qsBuf));

    /* initialize Windows sockets */
    if (WSAStartup(MAKEWORD(2,0), &wsaData) == SOCKET_ERROR) {
        return (uint8_t)0;
    }

    src = (char const *)arg;
    dst = host;
    while ((*src != '\0') && (*src != ':') && (dst < &host[sizeof(host)])) {
        *dst++ = *src++;
    }
    *dst = '\0';
    if (*src == ':') {
        port = (USHORT)strtoul(src + 1, NULL, 10);
    }


    l_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);       /* TCP socket */
    if (l_sock == INVALID_SOCKET){
        return (uint8_t)0;
    }

    server = gethostbyname(host);
    if (server == NULL) {
        return (uint8_t)0;
    }
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    memcpy(&servAddr.sin_addr, server->h_addr, server->h_length);
    servAddr.sin_port = htons(port);
    if (connect(l_sock, (struct sockaddr *)&servAddr, sizeof(servAddr))
        == SOCKET_ERROR)
    {
        QS_EXIT();
        return (uint8_t)0;
    }

    /* Set the socket to non-blocking mode. */
    if (ioctlsocket(l_sock, FIONBIO, &ioctl_opt) == SOCKET_ERROR) {
        QS_EXIT();
        return (uint8_t)0;
    }

    /* only after successful opeing of the socket turn on QS global filters */
    QS_FILTER_ON(QS_ALL_RECORDS);
    QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
    QS_FILTER_OFF(QS_QF_CRIT_EXIT);
    QS_FILTER_OFF(QS_QF_ISR_ENTRY);
    QS_FILTER_OFF(QS_QF_ISR_EXIT);
    QS_FILTER_OFF(QS_QF_TICK);
    QS_FILTER_OFF(QS_QK_SCHEDULE);

    return (uint8_t)1;                                           /* success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
    if (l_sock != INVALID_SOCKET) {
        closesocket(l_sock);
    }
    WSACleanup();
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t nBytes = 1000;
    uint8_t const *block;
    QF_CRIT_ENTRY(dummy);
    while ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        QF_CRIT_EXIT(dummy);
        send(l_sock, (char const *)block, nBytes, 0);
        nBytes = 1000;
        QF_CRIT_ENTRY(dummy);
    }
    QF_CRIT_EXIT(dummy);
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {
    return (QSTimeCtr)clock();
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/
