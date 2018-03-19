/**
* @file
* @brief QS/C QUTEST port for POSIX
* @ingroup qf
* @cond
******************************************************************************
* Last Updated for Version: 6.2.0
* Date of the Last Update:  2018-03-16
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
* https://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef Q_SPY
    #error "Q_SPY must be defined for QTEST application"
#endif /* Q_SPY */

#define QP_IMPL       /* this is QP implementation */
#include "qf_port.h"  /* QF port */
#include "qassert.h"  /* QP embedded systems-friendly assertions */
#include "qs_port.h"  /* include QS port */

Q_DEFINE_THIS_MODULE("qutest_port")

#include <stdio.h>    /* for printf() and _snprintf_s() */
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define QS_TX_SIZE     (4*1024)
#define QS_RX_SIZE     1024
#define QS_IMEOUT_MS   100
#define INVALID_SOCKET -1

/* local variables .........................................................*/
static void sigIntHandler(int dummy);
static int l_sock = INVALID_SOCKET;

/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_TX_SIZE];   /* buffer for QS-TX channel */
    static uint8_t qsRxBuf[QS_RX_SIZE]; /* buffer for QS-RX channel */
    char hostName[64];
    char const *src;
    char *dst;

    uint16_t port_local  = 51234; /* default local port */
    uint16_t port_remote = 6601;  /* default QSPY server port */
    int sockopt_bool;
    struct sockaddr_in sa_local;
    struct sockaddr_in sa_remote;
    struct hostent *host;

    struct sigaction sig_act;

    QS_initBuf(qsBuf, sizeof(qsBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    src = (arg != (void const *)0)
          ? (char const *)arg
          : "localhost";
    dst = hostName;
    while ((*src != '\0')
           && (*src != ':')
           && (dst < &hostName[sizeof(hostName)]))
    {
        *dst++ = *src++;
    }
    *dst = '\0';
    if (*src == ':') {
        port_remote = (uint16_t)strtoul(src + 1, NULL, 10);
    }

    //printf("<TARGET> Connecting to QSPY on Host=%s:%d...\n",
    //       hostName, port_remote);

    l_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* TCP socket */
    if (l_sock == INVALID_SOCKET){
        printf("<TARGET> ERROR   cannot create client socket,errno=%d\n",
               errno);
        goto error;
    }

    /* configure the socket */
    sockopt_bool = 1;
    setsockopt(l_sock, SOL_SOCKET, SO_REUSEADDR,
               &sockopt_bool, sizeof(sockopt_bool));

    sockopt_bool = 0;
    setsockopt(l_sock, SOL_SOCKET, SO_LINGER,
               &sockopt_bool, sizeof(sockopt_bool));

    /* local address:port */
    memset(&sa_local, 0, sizeof(sa_local));
    sa_local.sin_family = AF_INET;
    sa_local.sin_port = htons(port_local);
    host = gethostbyname(""); /* local host */

    /* remote hostName:port (QSPY server socket) */
    host = gethostbyname(hostName);
    if (host == NULL) {
        printf("<TARGET> ERROR   cannot resolve host Name=%s,errno=%d\n",
               hostName, errno);
        goto error;
    }

    memset(&sa_remote, 0, sizeof(sa_remote));
    sa_remote.sin_family = AF_INET;
    memcpy(&sa_remote.sin_addr, host->h_addr, host->h_length);
    sa_remote.sin_port = htons(port_remote);

    /* try to connect to the QSPY server */
    if (connect(l_sock, (struct sockaddr *)&sa_remote, sizeof(sa_remote))
        == -1)
    {
        printf("<TARGET> ERROR   cannot connect to QSPY at "
               "Host=%s:%d,errno=%d\n",
               hostName, port_remote, errno);
        goto error;
    }

    //printf("<TARGET> Connected to QSPY at Host=%s:%d\n",
    //       hostName, port_remote);
    QS_onFlush();

    /* install the SIGINT (Ctrl-C) signal handler */
    sig_act.sa_handler = &sigIntHandler;
    sigaction(SIGINT, &sig_act, NULL);

    return (uint8_t)1;  /* success */

error:
    return (uint8_t)0; /* failure */
}
/*..........................................................................*/
void QS_onCleanup(void) {
    if (l_sock != INVALID_SOCKET) {
        close(l_sock);
        l_sock = INVALID_SOCKET;
    }
    //printf("<TARGET> Disconnected from QSPY\n");
}
/*..........................................................................*/
void QS_onReset(void) {
    QS_onCleanup();
    exit(0);
}
/*..........................................................................*/
void QS_onTestLoop() {
    fd_set readSet;
    FD_ZERO(&readSet);

    QS_rxPriv_.inTestLoop = true;
    while (QS_rxPriv_.inTestLoop) {
        static struct timeval timeout = {
            (long)0, (long)(QS_IMEOUT_MS * 1000)
        };
        int nrec;
        uint16_t nBytes;
        uint8_t const *block;

        FD_SET(l_sock, &readSet);

        /* selective, timed blocking on the TCP/IP socket... */
        timeout.tv_usec = (long)(QS_IMEOUT_MS * 1000);
        nrec = select(l_sock + 1, &readSet,
                      (fd_set *)0, (fd_set *)0, &timeout);
        if (nrec < 0) {
            printf("<TARGET> ERROR socket select,errno=%d\n", errno);
            QS_onCleanup();
            exit(-2);
        }
        else if (FD_ISSET(l_sock, &readSet)) { /* socket ready to read? */
            uint8_t buf[QS_RX_SIZE];
            int status = recv(l_sock, (char *)buf, (int)sizeof(buf), 0);
            while (status > 0) { /* any data received? */
                uint8_t *pb;
                int i = (int)QS_rxGetNfree();
                if (i > status) {
                    i = status;
                }
                status -= i;
                /* reorder the received bytes into QS-RX buffer */
                for (pb = &buf[0]; i > 0; --i, ++pb) {
                    QS_RX_PUT(*pb);
                }
                QS_rxParse(); /* parse all n-bytes of data */
            }
        }

        nBytes = QS_TX_SIZE;
        //QF_CRIT_ENTRY(dummy);
        block = QS_getBlock(&nBytes);
        //QF_CRIT_EXIT(dummy);

        if (block != (uint8_t *)0) {
            send(l_sock, (char const *)block, nBytes, 0);
        }
    }
    /* set inTestLoop to true in case calls to QS_onTestLoop() nest,
    * which can happen through the calls to QS_TEST_PAUSE().
    */
    QS_rxPriv_.inTestLoop = true;
}
/*..........................................................................*/
void QS_onFlush(void) {
    if (l_sock != INVALID_SOCKET) {  /* socket initialized? */
        uint16_t nBytes = QS_TX_SIZE;
        uint8_t const *data;
        while ((data = QS_getBlock(&nBytes)) != (uint8_t *)0) {
            send(l_sock, (char const *)data, nBytes, 0);
            nBytes = QS_TX_SIZE;
        }
    }
}

/*..........................................................................*/
static void sigIntHandler(int dummy) {
    (void)dummy; /* unused parameter */
    QS_onCleanup();
    printf("\n<TARGET> disconnecting from QSPY\n");
    exit(-1);
}

