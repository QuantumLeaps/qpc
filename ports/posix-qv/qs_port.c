/**
* @file
* @brief QS/C port to POSIX
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.8.0
* Last updated on  2020-01-21
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
******************************************************************************
* @endcond
*/
/* expose features from the 2008 POSIX standard (IEEE Standard 1003.1-2008) */
#define _POSIX_C_SOURCE 200809L

#ifndef Q_SPY
    #error "Q_SPY must be defined to compile qs_port.c"
#endif /* Q_SPY */

#define QP_IMPL       /* this is QP implementation */
#include "qf_port.h"  /* QF port */
#include "qassert.h"  /* QP embedded systems-friendly assertions */
#include "qs_port.h"  /* include QS port */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

//Q_DEFINE_THIS_MODULE("qs_port")

#define QS_TX_SIZE     (8*1024)
#define QS_RX_SIZE     (2*1024)
#define QS_TX_CHUNK    QS_TX_SIZE
#define QS_TIMEOUT_MS  10

#define INVALID_SOCKET -1
#define SOCKET_ERROR   -1

/* local variables .........................................................*/
static int l_sock = INVALID_SOCKET;
static struct timespec const c_timeout = { 0, QS_TIMEOUT_MS*1000000L };

/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_TX_SIZE];   /* buffer for QS-TX channel */
    static uint8_t qsRxBuf[QS_RX_SIZE]; /* buffer for QS-RX channel */
    char hostName[128];
    char const *serviceName = "6601";  /* default QSPY server port */
    char const *src;
    char *dst;
    int status;

    struct addrinfo *result = NULL;
    struct addrinfo *rp = NULL;
    struct addrinfo hints;
    int sockopt_bool;

    /* initialize the QS transmit and receive buffers */
    QS_initBuf(qsBuf, sizeof(qsBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    /* extract hostName from 'arg' (hostName:port_remote)... */
    src = (arg != (void *)0)
          ? (char const *)arg
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
        fprintf(stderr,
            "<TARGET> ERROR   cannot resolve host Name=%s:%s,Err=%d\n",
                    hostName, serviceName, status);
        goto error;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        l_sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (l_sock != INVALID_SOCKET) {
            if (connect(l_sock, rp->ai_addr, rp->ai_addrlen)
                == SOCKET_ERROR)
            {
                close(l_sock);
                l_sock = INVALID_SOCKET;
            }
            break;
        }
    }

    freeaddrinfo(result);

    /* socket could not be opened & connected? */
    if (l_sock == INVALID_SOCKET) {
        fprintf(stderr, "<TARGET> ERROR   cannot connect to QSPY at "
            "host=%s:%s\n",
            hostName, serviceName);
        goto error;
    }

    /* set the socket to non-blocking mode */
    status = fcntl(l_sock, F_GETFL, 0);
    if (status == -1) {
        fprintf(stderr,
            "<TARGET> ERROR   Socket configuration failed errno=%d\n",
            errno);
        QS_EXIT();
        goto error;
    }
    if (fcntl(l_sock, F_SETFL, status | O_NONBLOCK) != 0) {
        fprintf(stderr, "<TARGET> ERROR   Failed to set non-blocking socket "
            "errno=%d\n", errno);
        QS_EXIT();
        goto error;
    }

    /* configure the socket to reuse the address and not to linger */
    sockopt_bool = 1;
    setsockopt(l_sock, SOL_SOCKET, SO_REUSEADDR,
               &sockopt_bool, sizeof(sockopt_bool));
    sockopt_bool = 0; /* negative option */
    setsockopt(l_sock, SOL_SOCKET, SO_LINGER,
               &sockopt_bool, sizeof(sockopt_bool));
    QS_onFlush();

    return 1U; /* success */

error:
    return 0U; /* failure */
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
void QS_onFlush(void) {
    uint16_t nBytes;
    uint8_t const *data;
    QS_CRIT_STAT_

    if (l_sock == INVALID_SOCKET) { /* socket NOT initialized? */
        fprintf(stderr, "<TARGET> ERROR   invalid TCP socket\n");
        return;
    }

    nBytes = QS_TX_CHUNK;
    QS_CRIT_ENTRY_();
    while ((data = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        QS_CRIT_EXIT_();
        for (;;) { /* for-ever until break or return */
            int nSent = send(l_sock, (char const *)data, (int)nBytes, 0);
            if (nSent == SOCKET_ERROR) { /* sending failed? */
                if ((errno == EWOULDBLOCK) || (errno == EAGAIN)) {
                    /* sleep for the timeout and then loop back
                    * to send() the SAME data again
                    */
                    nanosleep(&c_timeout, NULL);
                }
                else { /* some other socket error... */
                    fprintf(stderr, "<TARGET> ERROR   sending data over TCP,"
                           "errno=%d\n", errno);
                    return;
                }
            }
            else if (nSent < (int)nBytes) { /* sent fewer than requested? */
                nanosleep(&c_timeout, NULL); /* sleep for the timeout */
                /* adjust the data and loop back to send() the rest */
                data   += nSent;
                nBytes -= (uint16_t)nSent;
            }
            else {
                break;
            }
        }
        /* set nBytes for the next call to QS_getBlock() */
        nBytes = QS_TX_CHUNK;
        QS_CRIT_ENTRY_();
    }
    QS_CRIT_EXIT_();
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {
    struct timespec tspec;
    QSTimeCtr time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tspec);

    /* convert to units of 0.1 microsecond */
    time = (QSTimeCtr)(tspec.tv_sec * 10000000 + tspec.tv_nsec / 100);
    return time;
}

/*..........................................................................*/
void QS_output(void) {
    uint16_t nBytes;
    uint8_t const *data;
    QS_CRIT_STAT_

    if (l_sock == INVALID_SOCKET) { /* socket NOT initialized? */
        fprintf(stderr, "<TARGET> ERROR   invalid TCP socket\n");
        return;
    }

    nBytes = QS_TX_CHUNK;
    QS_CRIT_ENTRY_();
    if ((data = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        QS_CRIT_EXIT_();
        for (;;) { /* for-ever until break or return */
            int nSent = send(l_sock, (char const *)data, (int)nBytes, 0);
            if (nSent == SOCKET_ERROR) { /* sending failed? */
                if ((errno == EWOULDBLOCK) || (errno == EAGAIN)) {
                    /* sleep for the timeout and then loop back
                    * to send() the SAME data again
                    */
                    nanosleep(&c_timeout, NULL);
                }
                else { /* some other socket error... */
                    fprintf(stderr, "<TARGET> ERROR   sending data over TCP,"
                           "errno=%d\n", errno);
                    return;
                }
            }
            else if (nSent < (int)nBytes) { /* sent fewer than requested? */
                nanosleep(&c_timeout, NULL); /* sleep for the timeout */
                /* adjust the data and loop back to send() the rest */
                data   += nSent;
                nBytes -= (uint16_t)nSent;
            }
            else {
                break;
            }
        }
    }
    else {
        QS_CRIT_EXIT_();
    }
}
/*..........................................................................*/
void QS_rx_input(void) {
    uint8_t buf[QS_RX_SIZE];
    int status = recv(l_sock, (char *)buf, (int)sizeof(buf), 0);
    if (status != SOCKET_ERROR) { /* any data received? */
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

