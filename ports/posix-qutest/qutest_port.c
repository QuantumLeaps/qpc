/**
* @file
* @brief QS/C QUTEST port for POSIX
* @ingroup qf
* @cond
******************************************************************************
* Last Updated for Version: 5.9.0
* Date of the Last Update:  2017-05-17
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
* https://state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#define QP_IMPL       /* this is QP implementation */
#include "qf_port.h"  /* QF port */
#include "qassert.h"  /* QP embedded systems-friendly assertions */
#include "qs_port.h"  /* include QS port */

Q_DEFINE_THIS_MODULE("qs_port")

#ifndef Q_SPY
    #error "Q_SPY must be defined for QTEST application"
#endif /* Q_SPY */

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
#include <termios.h>
#include <unistd.h>

#define QS_TX_SIZE    (4*1024)
#define QS_RX_SIZE    1024
#define QS_IMEOUT_MS  100
#define INVALID_SOCKET -1

/* local variables .........................................................*/
static int l_sock = INVALID_SOCKET;
static struct termios l_termios_saved; /* saved terminal attributes */

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

    struct termios t;

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

    l_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* TCP socket */
    if (l_sock == INVALID_SOCKET){
        printf("<TARGET> ERROR   cannot create client socket, errno=%d\n",
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
    //sa_local.sin_addr.s_addr = inet_addr(
    //    inet_ntoa(*(struct in_addr *)*host->h_addr_list));
    //if (bind(l_sock, &sa_local, sizeof(sa_local)) == -1) {
    //    printf("<TARGET> Cannot bind to the local port Err=0x%08X\n",
    //           WSAGetLastError());
    //    /* no error */
    //}

    /* remote hostName:port (QSPY server socket) */
    host = gethostbyname(hostName);
    if (host == NULL) {
        printf("<TARGET> ERROR   cannot be resolve host name, errno=%d\n",
               errno);
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
        printf("<TARGET> ERROR   cannot be connected to QSPY, errno=%d\n",
               errno);
        goto error;
    }

    printf("<TARGET> Connected to QSPY via TCP/IP\n"); //???

    /* modify the terminal attributes... */
    /* get the original terminal settings */
    if (tcgetattr(0, &l_termios_saved) == -1) {
        printf("    <CONS> ERROR    getting terminal attributes\n");
        goto error;
    }

    t = l_termios_saved;
    t.c_lflag &= ~(ICANON | ECHO); /* disable canonical mode and echo */
    if (tcsetattr(0, TCSANOW, &t) == -1) {
        printf("    <CONS> ERROR    setting terminal attributes");
        goto error;
    }

    //printf("<TARGET> Connected to QSPY via TCP/IP\n");
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
    /* restore the saved terminal settings */
    tcsetattr(0, TCSANOW, &l_termios_saved);
    //printf("<TARGET> Disconnected from QSPY via TCP/IP\n");
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

        FD_SET(0,      &readSet);   /* console/terminal */
        FD_SET(l_sock, &readSet);   /* the socket */

        /* selective, timed blocking on the TCP/IP socket... */
        timeout.tv_usec = (long)(QS_IMEOUT_MS * 1000);
        nrec = select(l_sock + 1, &readSet,
                      (fd_set *)0, (fd_set *)0, &timeout);
        if (nrec < 0) {
            printf("    <CONS> ERROR    select() errno=%d\n", errno);
            QS_onCleanup();
            exit(-2);
        }
        else if (nrec > 0) {
            if (FD_ISSET(l_sock, &readSet)) { /* socket ready to read? */
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
            if (FD_ISSET(0, &readSet)) { /* console/terminal redy to read? */
                char ch;
                read(0, &ch, 1);
                switch (ch) {
                    case 'x':      /* 'x' pressed? */
                    case 'X':      /* 'X' pressed? */
                    case '\033': { /* ESC pressed? */
                        QS_onCleanup();
                        exit(0);
                        break;
                    }
                }
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
    uint16_t nBytes = (uint16_t)QS_TX_SIZE;
    uint8_t const *block;
    while ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        send(l_sock, (char const *)block, nBytes, 0);
        nBytes = (uint16_t)QS_TX_SIZE;
    }
}
