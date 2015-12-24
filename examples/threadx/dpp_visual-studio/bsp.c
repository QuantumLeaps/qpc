/*****************************************************************************
* Product: DPP example, ThreadX demo on Windows
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
#include "dpp.h"
#include "bsp.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Q_DEFINE_THIS_FILE

/* local variables ---------------------------------------------------------*/
static TX_TIMER l_tick_timer; /* ThreadX timer to call QF_tickX_() */
static unsigned l_rnd;        /* random seed */

#ifdef Q_SPY

    enum QSUserRecords {
        PHILO_STAT = QS_USER
    };
    /* ThreadX thread and thread function for QS output, see NOTE1 */
    static char const *l_hostAndPort = "localhost:6601";
    static TX_THREAD l_qs_output_thread;
    static void qs_thread_function(ULONG thread_input);
    static ULONG qs_thread_stkSto[64];

#endif /* Q_SPY */

/*..........................................................................*/
void BSP_init(int argc, char *argv[]) {
    printf("Dining Philosopher Problem example"
           "\nQP %s\n"
           "Press Ctrl-C to quit...\n",
           QP_versionStr);

    BSP_randomSeed(1234U); /* seed the random number generator */

    if (argc > 1) { /* port specified? */
        l_hostAndPort = argv[1];
    }
    if (!QS_INIT(l_hostAndPort)) {
        printf("\nUnable to open QSpy socket\n");
        exit(-1);
    }
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    printf("Philosopher %2d is %s\n", (int)n, stat);

    QS_BEGIN(PHILO_STAT, AO_Philo[n]) /* application-specific record begin */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    printf("PAUSED\n");
}
/*..........................................................................*/
uint32_t BSP_random(void) {  /* a very cheap pseudo-random-number generator */
    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    l_rnd = l_rnd * (3U*7U*11U*13U*23U);
    return l_rnd >> 8;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
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
    * might choose to call QF_tickX_() directly from timer interrupts
    * or from active object(s).
    */
    Q_ALLEGE(tx_timer_create(&l_tick_timer, /* ThreadX timer object */
                 "QF",     /* name of the timer */
                 (VOID (*)(ULONG))&QF_tickX_, /* expiration function */
                 0U,       /* expiration function input (tick rate) */
                 1U,       /* initial ticks */
                 1U,       /* reschedule ticks */
                 TX_AUTO_ACTIVATE) /* automatically activate timer */
             == TX_SUCCESS);

#ifdef Q_SPY
    /* start a ThreadX timer to perform QS output. See NOTE1... */
    Q_ALLEGE(tx_thread_create(&l_qs_output_thread, /* thread control block */
        "QS",                     /* thread name */
        &qs_thread_function,      /* thread function */
        (ULONG)0,                 /* thread input (unsued) */
        qs_thread_stkSto,         /* stack start */
        sizeof(qs_thread_stkSto), /* stack size in bytes */
        TX_MAX_PRIORITIES - 1,    /* ThreadX priority (lowest possible)*/
        TX_MAX_PRIORITIES - 1,    /* preemption threshold disabled */
        TX_NO_TIME_SLICE,
        TX_AUTO_START)
        == TX_SUCCESS);
#endif /* Q_SPY */
}
/*..........................................................................*/
void QF_onCleanup(void) {
    tx_timer_deactivate(&l_tick_timer);
    QS_EXIT();
    exit(0);
}
/*..........................................................................*/
void Q_onAssert(char const * const file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    QF_stop();
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY /* define QS callbacks */

/*
* In this demo, the QS software tracing output is sent out of the application
* through a TCP/IP socket. This requires the QSPY host application to
* be started first to open a server socket (qspy -t ...) to wait for the
* incoming TCP/IP connection from the DPP demo.
*
* In an embedded target, the QS software tracing output can be sent out
* using any method available, such as a UART. This would require changing
* the implementation of the functions in this section, but the rest of the
* application code does not "know" (and should not care) how the QS ouptut
* is actually performed. In other words, the rest of the application does NOT
* need to change in any way to produce QS output.
*/

static SOCKET l_sock = INVALID_SOCKET;

/*..........................................................................*/
static void qs_thread_function(ULONG thread_input) { /* see NOTE1 */

    (void)thread_input;

    for (;;) {
        uint16_t nBytes = 1024U;
        uint8_t const *block;
        QF_CRIT_STAT_TYPE int_posture;

        QF_CRIT_ENTRY(int_posture);
        block = QS_getBlock(&nBytes);
        QF_CRIT_EXIT(int_posture);

        if (block != (uint8_t *)0) {
            send(l_sock, (char const *)block, nBytes, 0);
        }

        tx_thread_sleep(1U); /* sleep for a tick */
    }
}

/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[1024]; /* 1K buffer for Quantum Spy */
    static WSADATA wsaData;
    char host[64];
    char const *src;
    char *dst;
    USHORT port = 6601; /* default QSPY server port */
    ULONG ioctl_opt = 1;
    struct sockaddr_in sockAddr;
    struct hostent *server;

    QS_initBuf(qsBuf, sizeof(qsBuf));

    /* initialize Windows sockets */
    if (WSAStartup(MAKEWORD(2,0), &wsaData) == SOCKET_ERROR) {
        printf("Windows Sockets cannot be initialized.");
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

    l_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* TCP socket */
    if (l_sock == INVALID_SOCKET){
        printf("Socket cannot be created; error 0x%08X\n",
               WSAGetLastError());
        return (uint8_t)0; /* failure */
    }

    server = gethostbyname(host);
    if (server == NULL) {
        printf("QSpy host name cannot be resolved; error 0x%08X\n",
               WSAGetLastError());
        return (uint8_t)0;
    }

    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    memcpy(&sockAddr.sin_addr, server->h_addr, server->h_length);
    sockAddr.sin_port = htons(port);
    if (connect(l_sock, (struct sockaddr *)&sockAddr, sizeof(sockAddr))
        == SOCKET_ERROR)
    {
        printf("Cannot connect to the QSPY server; error 0x%08X\n",
               WSAGetLastError());
        QS_EXIT();
        return (uint8_t)0; /* failure */
    }

    /* Set the socket to non-blocking mode. */
    if (ioctlsocket(l_sock, FIONBIO, &ioctl_opt) == SOCKET_ERROR) {
        printf("Socket configuration failed.\n"
               "Windows socket error 0x%08X.",
               WSAGetLastError());
        QS_EXIT();
        return (uint8_t)0; /* failure */
    }

    // Set QS global filters...
    QS_FILTER_ON(QS_QEP_STATE_ENTRY);
    QS_FILTER_ON(QS_QEP_STATE_EXIT);
    QS_FILTER_ON(QS_QEP_STATE_INIT);
    QS_FILTER_ON(QS_QEP_TRAN_HIST);
    QS_FILTER_ON(QS_QEP_INTERN_TRAN);
    QS_FILTER_ON(QS_QEP_TRAN);
    QS_FILTER_ON(QS_QEP_IGNORED);
    QS_FILTER_ON(QS_QEP_DISPATCH);
    QS_FILTER_ON(QS_QEP_UNHANDLED);

    QS_FILTER_ON(PHILO_STAT);

    return (uint8_t)1; /* success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
    tx_thread_delete(&l_qs_output_thread);

    if (l_sock != INVALID_SOCKET) {
        closesocket(l_sock);
    }
    WSACleanup();
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t nBytes = 1000;
    uint8_t const *block;
    while ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        send(l_sock, (char const *)block, nBytes, 0);
        nBytes = 1000;
    }
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {
    return (QSTimeCtr)clock();
}

/*
* NOTE1:
* This application uses the ThreadX thread of the lowest priority to perform
* the QS data output to the host. This is not the only choice available, and
* other applications might choose to peform the QS output some other way.
*/

#endif /* Q_SPY */
