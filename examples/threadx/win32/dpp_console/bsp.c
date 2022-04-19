/*****************************************************************************
* Product: DPP example, ThreadX kernel, Win32 emulation, console app.
* Last updated for: @ref qpc_7_0_0
* Last updated on  2021-12-03
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
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
/* Win32 configuration macros needed for proper WinSock includes */
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501

#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

#include <stdio.h>    /* for printf_s() */
#include <stdlib.h>   /* for exit() */
#include <conio.h>    /* for keyboard input */

Q_DEFINE_THIS_FILE

/* Local-scope objects -----------------------------------------------------*/
static uint32_t l_rnd; /* random seed */
static TX_TIMER l_tick_timer; /* ThreadX timer to call QF_TICK_X() */

#ifdef Q_SPY

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER,
        COMMAND_STAT
    };

    /* QSpy source IDs */
    static QSpyId const l_clock_tick = { QS_AP_ID };
    static void QS_rx_input(void);
    static void QS_output(void);
#endif

/*..........................................................................*/
void BSP_init(void) {
    printf_s("Dining Philosophers Problem example\n"
        "QP port to ThreadX\n"
        "ThreadX port to Win32\n"
        "QP %s\n"
        "Press 'p' to pause\n"
        "Press 's' to serve\n"
        "Press 'x' to quit...\n",
        QP_VERSION_STR);

    /* seed the random number generator */
    BSP_randomSeed(1234U);

    if (QS_INIT((void *)0) == 0U) { /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(COMMAND_STAT);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_ALL_RECORDS);
    QS_GLB_FILTER(-QS_QF_TICK);
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    printf("Philo[%d] is %s\n", (int)n, stat);

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    printf("%s\n", paused ? "PAUSED" : "SERVING");
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    uint32_t rnd;

    /* Some flating point code is to exercise the VFP... */
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    rnd = l_rnd * (3U*7U*11U*13U*23U);
    l_rnd = rnd; /* set for the next time */

    return (rnd >> 8);
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}

/* QF callbacks ============================================================*/
static VOID timer_expiration(ULONG id) {
    (void)id; /* unused parameter */

    QF_TICK_X(0U, &l_clock_tick); /* perform the QF clock tick processing */

    /* handle keyborad input... */
    if (_kbhit()) {
        int ch = _getwch();
        switch (ch) {
            case 'p': {
                QF_PUBLISH(Q_NEW(QEvt, PAUSE_SIG), &l_clock_tick);
                break;
            }
            case 's': {
                QF_PUBLISH(Q_NEW(QEvt, SERVE_SIG), &l_clock_tick);
                break;
            }
            case 'x': {
                QF_stop();
                break;
            }
            default: {
                break;
            }
        }
    }

    /* also in case of Win32, perform here QS RX/TX... */
    QS_RX_INPUT(); /* handle the QS-RX input */
    QS_OUTPUT();   /* handle the QS-TX output */
}
/*..........................................................................*/
void QF_onStartup(void) {
    /*
    * NOTE:
    * This application uses the ThreadX timer to periodically call
    * the QF_tickX_(0) function. Here, only the clock tick rate of 0
    * is used, but other timers can be used to call QF_tickX_() for
    * other clock tick rates, if needed.
    *
    * The choice of a ThreadX timer is not the only option. Applications
    * might choose to call QF_TICK_X() directly from timer interrupts
    * or from active object(s).
    */
    Q_ALLEGE(tx_timer_create(&l_tick_timer, /* ThreadX timer object */
        (CHAR *)"QP-tick", /* name of the timer */
        &timer_expiration, /* expiration function */
        0U,       /* expiration function input (tick rate) */
        1U,       /* initial ticks */
        1U,       /* reschedule ticks */
        TX_AUTO_ACTIVATE) /* automatically activate timer */
             == TX_SUCCESS);
}
/*..........................................................................*/
void QF_onCleanup(void) {
#ifdef Q_SPY
    QS_onCleanup();
#endif
    exit(0);
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char const * const module, int_t const loc) {
    printf("Assertion failed in %s:%d\n", module, loc);
    QS_ASSERTION(module, loc, 10000U); /* report assertion to QS */
}

/* QS callbacks ============================================================*/
#ifdef Q_SPY

#include "qs_pkg.h"   /* QS package-scope interface for QS_rxPriv_ */
#include <ws2tcpip.h> /* for TCP/IP transport used by QS */

#define QS_TX_SIZE     (8*1024)
#define QS_RX_SIZE     (2*1024)
#define QS_TX_CHUNK    QS_TX_SIZE
#define QS_TIMEOUT_MS  10

/* local variables .........................................................*/
static SOCKET l_sock = INVALID_SOCKET;

/*..........................................................................*/
uint8_t QS_onStartup(void const* arg) {
    static uint8_t qsBuf[QS_TX_SIZE];   /* buffer for QS-TX channel */
    static uint8_t qsRxBuf[QS_RX_SIZE]; /* buffer for QS-RX channel */
    char hostName[128];
    char const* serviceName = "6601";   /* default QSPY server port */
    char const* src;
    char* dst;
    int status;

    struct addrinfo* result = NULL;
    struct addrinfo* rp = NULL;
    struct addrinfo  hints;
    BOOL   sockopt_bool;
    ULONG  ioctl_opt;
    WSADATA wsaData;

    /* initialize the QS transmit and receive buffers */
    QS_initBuf(qsBuf, sizeof(qsBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    /* initialize Windows sockets version 2.2 */
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
        fprintf_s(stderr, "<TARGET> ERROR %s\n",
            "Windows Sockets cannot be initialized");
        goto error;
    }

    /* extract hostName from 'arg' (hostName:port_remote)... */
    src = (arg != (void*)0)
        ? (char const*)arg
        : "localhost"; /* default QSPY host */
    dst = hostName;
    while ((*src != '\0')
        && (*src != ':')
        && (dst < &hostName[sizeof(hostName) - 1]))
    {
        *dst++ = *src++;
    }
    *dst = '\0'; /* zero-terminate hostName */

    /* extract serviceName from 'arg' (hostName:serviceName)... */
    if (*src == ':') {
        serviceName = src + 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    status = getaddrinfo(hostName, serviceName, &hints, &result);
    if (status != 0) {
        fprintf_s(stderr,
            "<TARGET> ERROR   cannot resolve host Name=%s:%s,Err=%d\n",
            hostName, serviceName, status);
        goto error;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        l_sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (l_sock != INVALID_SOCKET) {
            if (connect(l_sock, rp->ai_addr, (int)rp->ai_addrlen)
                == SOCKET_ERROR)
            {
                closesocket(l_sock);
                l_sock = INVALID_SOCKET;
            }
            break;
        }
    }

    freeaddrinfo(result);

    /* socket could not be opened & connected? */
    if (l_sock == INVALID_SOCKET) {
        fprintf_s(stderr, "<TARGET> ERROR   cannot connect to QSPY at "
            "host=%s:%s\n",
            hostName, serviceName);
        goto error;
    }

    /* set the socket to non-blocking mode */
    ioctl_opt = 1;
    if (ioctlsocket(l_sock, FIONBIO, &ioctl_opt) != NO_ERROR) {
        fprintf_s(stderr, "<TARGET> ERROR   %s WASErr=%d\n,",
            "Failed to set non-blocking socket", WSAGetLastError());
        goto error;
    }

    /* configure the socket to reuse the address and not to linger */
    sockopt_bool = TRUE;
    setsockopt(l_sock, SOL_SOCKET, SO_REUSEADDR,
        (const char*)&sockopt_bool, sizeof(sockopt_bool));
    sockopt_bool = TRUE;
    setsockopt(l_sock, SOL_SOCKET, SO_DONTLINGER,
        (const char*)&sockopt_bool, sizeof(sockopt_bool));
    QS_onFlush();

    return 1U; /* success */

error:
    return 0U; /* failure */
}
/*..........................................................................*/
void QS_onCleanup(void) {
    if (l_sock != INVALID_SOCKET) {
        closesocket(l_sock);
        l_sock = INVALID_SOCKET;
    }
    WSACleanup();
    printf_s("<TARGET> Disconnected from QSPY\n");
}
/*..........................................................................*/
void QS_onReset(void) {
    QS_onCleanup();
    exit(0);
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t nBytes;
    uint8_t const* data;
    QS_CRIT_STAT_

    if (l_sock == INVALID_SOCKET) { /* socket NOT initialized? */
        fprintf_s(stderr, "<TARGET> ERROR   %s\n",
            "invalid TCP socket");
        return;
    }

    nBytes = QS_TX_CHUNK;
    QS_CRIT_E_();
    while ((data = QS_getBlock(&nBytes)) != (uint8_t*)0) {
        QS_CRIT_X_();
        for (;;) { /* for-ever until break or return */
            int nSent = send(l_sock, (char const*)data, (int)nBytes, 0);
            if (nSent == SOCKET_ERROR) { /* sending failed? */
                int err = WSAGetLastError();
                if (err == WSAEWOULDBLOCK) {
                    /* sleep for the timeout and then loop back
                    * to send() the SAME data again
                    */
                    Sleep(QS_TIMEOUT_MS);
                }
                else { /* some other socket error... */
                    fprintf_s(stderr, "<TARGET> ERROR   %s WASErr=%d\n",
                        "sending data over TCP", err);
                    return;
                }
            }
            else if (nSent < (int)nBytes) { /* sent fewer than requested? */
                Sleep(QS_TIMEOUT_MS); /* sleep for the timeout */
                /* adjust the data and loop back to send() the rest */
                data += nSent;
                nBytes -= (uint16_t)nSent;
            }
            else {
                break; /* break out of the for-ever loop */
            }
        }
        /* set nBytes for the next call to QS_getBlock() */
        nBytes = QS_TX_CHUNK;
        QS_CRIT_E_();
    }
    QS_CRIT_X_();
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return (QSTimeCtr)time.QuadPart;
}
/*..........................................................................*/
/*! callback function to execute a user command (to be implemented in BSP) */
void QS_onCommand(uint8_t cmdId,
    uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;
    QS_BEGIN_ID(COMMAND_STAT, 0U) /* app-specific record */
        QS_U8(2, cmdId);
    QS_U32(8, param1);
    QS_U32(8, param2);
    QS_U32(8, param3);
    QS_END()
}

/*..........................................................................*/
static void QS_output(void) {
    uint16_t nBytes;
    uint8_t const* data;
    QS_CRIT_STAT_

        if (l_sock == INVALID_SOCKET) { /* socket NOT initialized? */
            fprintf_s(stderr, "<TARGET> ERROR   %s\n",
                "invalid TCP socket");
            return;
        }

    nBytes = QS_TX_CHUNK;
    QS_CRIT_E_();
    if ((data = QS_getBlock(&nBytes)) != (uint8_t*)0) {
        QS_CRIT_X_();
        for (;;) { /* for-ever until break or return */
            int nSent = send(l_sock, (char const*)data, (int)nBytes, 0);
            if (nSent == SOCKET_ERROR) { /* sending failed? */
                int err = WSAGetLastError();
                if (err == WSAEWOULDBLOCK) {
                    /* sleep for the timeout and then loop back
                    * to send() the SAME data again
                    */
                    Sleep(QS_TIMEOUT_MS);
                }
                else { /* some other socket error... */
                    fprintf_s(stderr, "<TARGET> ERROR   sending data over TCP,"
                        "WASErr=%d\n", err);
                    return;
                }
            }
            else if (nSent < (int)nBytes) { /* sent fewer than requested? */
                Sleep(QS_TIMEOUT_MS); /* sleep for the timeout */
                /* adjust the data and loop back to send() the rest */
                data += nSent;
                nBytes -= (uint16_t)nSent;
            }
            else {
                break;
            }
        }
    }
    else {
        QS_CRIT_X_();
    }
}
/*..........................................................................*/
static void QS_rx_input(void) {
    int status = recv(l_sock, (char*)QS_rxPriv_.buf, (int)QS_rxPriv_.end, 0);
    if (status > 0) { /* any data received? */
        QS_rxPriv_.tail = 0U;
        QS_rxPriv_.head = status; /* # bytes received */
        QS_rxParse(); /* parse all received bytes */
    }
}

#endif /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE1:
* This application uses the ThreadX thread of the lowest priority to perform
* the QS data output to the host. This is not the only choice available, and
* other applications might choose to peform the QS output some other way.
*
* The lowest-priority thread does not block, so in effect, it becomes the
* idle loop. This presents no problems to ThreadX - its idle task in the
* scheduler does not need to run.
*/

