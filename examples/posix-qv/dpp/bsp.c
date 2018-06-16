/*****************************************************************************
* Product: DPP example, POSIX
* Last updated for version 6.3.2
* Last updated on  2018-06-16
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>      /* for memcpy() and memset() */
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

Q_DEFINE_THIS_FILE

/* Local objects -----------------------------------------------------------*/
static struct termios l_tsav; /* structure with saved terminal attributes */
static uint32_t l_rnd;        /* random seed */

#ifdef Q_SPY
    enum {
        PHILO_STAT = QS_USER
    };
    static uint8_t const l_clock_tick = 0U;
#endif

/* BSP functions ===========================================================*/
void BSP_init(int argc, char **argv) {

#ifndef Q_SPY
    (void)argc; /* unused parameter */
    (void)argv; /* unused parameter */
#endif

    printf("Dining Philosophers Problem example"
           "\nQP %s\n"
           "Press p to pause\n"
           "Press s to serve\n"
           "Press ESC to quit...\n",
           QP_versionStr);

    BSP_randomSeed(1234U);

    Q_ALLEGE(QS_INIT(argc > 1 ? argv[1] : (void *)0));
    QS_OBJ_DICTIONARY(&l_clock_tick); /* must be called *after* QF_init() */
    QS_USR_DICTIONARY(PHILO_STAT);

    /* setup the QS filters... */
    QS_FILTER_ON(QS_SM_RECORDS); // state machine records
    QS_FILTER_ON(QS_UA_RECORDS); // all usedr records
    //QS_FILTER_ON(QS_MUTEX_LOCK);
    //QS_FILTER_ON(QS_MUTEX_UNLOCK);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
    QF_stop();
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    printf("Philosopher %2d is %s\n", (int)n, stat);

    QS_BEGIN(PHILO_STAT, AO_Philo[n]) /* application-specific record begin */
        QS_U8(1, n);   /* Philosopher number */
        QS_STR(stat);  /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    printf("Paused is %s\n", paused ? "ON" : "OFF");
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    l_rnd = l_rnd * (3*7*11*13*23);
    return l_rnd >> 8;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}


/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    struct termios tio;   /* modified terminal attributes */

    tcgetattr(0, &l_tsav); /* save the current terminal attributes */
    tcgetattr(0, &tio);    /* obtain the current terminal attributes */
    tio.c_lflag &= ~(ICANON | ECHO); /* disable the canonical mode & echo */
    tcsetattr(0, TCSANOW, &tio);     /* set the new attributes */

    QF_setTickRate(BSP_TICKS_PER_SEC, 30); /* the desired tick rate/prio */
}
/*..........................................................................*/
void QF_onCleanup(void) {
    printf("\nBye! Bye!\n");
    tcsetattr(0, TCSANOW, &l_tsav);/* restore the saved terminal attributes */
    QS_EXIT(); /* perfomr the QS cleanup */
}
/*..........................................................................*/
void QF_onClockTick(void) {
    struct timeval timeout = { 0, 0 };  /* timeout for select() */
    fd_set con; /* FD set representing the console */

    QF_TICK_X(0U, &l_clock_tick); /* perform the QF clock tick processing */

    FD_ZERO(&con);
    FD_SET(0, &con);
    /* check if a console input is available, returns immediately */
    if (0 != select(1, &con, 0, 0, &timeout)) { /* any descriptor set? */
        char ch;
        read(0, &ch, 1);
        if (ch == '\33') { /* ESC pressed? */
            BSP_terminate(0);
        }
        else if (ch == 'p') {
            QF_PUBLISH(Q_NEW(QEvt, PAUSE_SIG), &l_clock_tick);
        }
        else if (ch == 's') {
            QF_PUBLISH(Q_NEW(QEvt, SERVE_SIG), &l_clock_tick);
        }
    }
}
/*..........................................................................*/
void Q_onAssert(char const *module, int loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    printf("Assertion failed in %s:%d", module, loc);
    QS_ASSERTION(module, loc, (uint32_t)10000U); /* report assertion to QS */
    exit(-1);
}

//============================================================================
#ifdef Q_SPY

/*
* NOTE:
* The QS target-resident component is implemented in two different ways:
* 1. Output to the TCP/IP socket, which requires a separate QSPY host
*    application running; or
* 2. Direct linking with the QSPY host application to perform direct output
*    to the console from the running application. (This option requires
*    the QSPY source code, which is part of the QTools collection).
*
* The two options are selected by the following QS_IMPL_OPTION macro.
* Please set the value of this macro to either 1 or 2:
*/
#define QS_IMPL_OPTION 1

/*--------------------------------------------------------------------------*/
#if (QS_IMPL_OPTION == 1)

/*
* 1. Output to the TCP/IP socket, which requires a separate QSPY host
*    application running. This option does not link to the QSPY code.
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>

#define QS_TX_SIZE    (4*1024)
#define QS_RX_SIZE    1024
#define QS_IMEOUT_MS  100
#define INVALID_SOCKET -1

/* local variables .........................................................*/
static void *idleThread(void *par); // the expected P-Thread signature
static int l_sock = INVALID_SOCKET;
static uint8_t l_running;

/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_TX_SIZE];   // buffer for QS-TX channel
    static uint8_t qsRxBuf[QS_RX_SIZE]; // buffer for QS-RX channel
    char hostName[64];
    char const *src;
    char *dst;

    uint16_t port_local  = 51234; /* default local port */
    uint16_t port_remote = 6601;  /* default QSPY server port */
    int sockopt_bool;
    struct sockaddr_in sa_local;
    struct sockaddr_in sa_remote;
    struct hostent *host;

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

    printf("<TARGET> Connecting to QSPY on Host=%s:%d...\n",
           hostName, port_remote);

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
        printf("<TARGET> ERROR   cannot resolve host Name=%s:%d,errno=%d\n",
               hostName, port_remote, errno);
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
        printf("<TARGET> ERROR   cannot connect to QSPY on Host="
               "%s:%d,errno=%d\n", hostName, port_remote, errno);
        goto error;
    }

    printf("<TARGET> Connected  to QSPY on Host=%s:%d\n",
           hostName, port_remote);

    pthread_attr_t attr;
    struct sched_param param;
    pthread_t idle;

    // SCHED_FIFO corresponds to real-time preemptive priority-based
    // scheduler.
    // NOTE: This scheduling policy requires the superuser priviledges

    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = sched_get_priority_min(SCHED_FIFO);

    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (pthread_create(&idle, &attr, &idleThread, 0) != 0) {
        // Creating the p-thread with the SCHED_FIFO policy failed.
        // Most probably this application has no superuser privileges,
        // so we just fall back to the default SCHED_OTHER policy
        // and priority 0.
        pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
        param.sched_priority = 0;
        pthread_attr_setschedparam(&attr, &param);
        if (pthread_create(&idle, &attr, &idleThread, 0) == 0) {
            return false;
        }
    }
    pthread_attr_destroy(&attr);

    return (uint8_t)1;  // success

error:
    return (uint8_t)0; // failure
}
/*..........................................................................*/
void QS_onCleanup(void) {
    l_running = (uint8_t)0;
    if (l_sock != INVALID_SOCKET) {
        close(l_sock);
        l_sock = INVALID_SOCKET;
    }
    //printf("<TARGET> Disconnected from QSPY via TCP/IP\n");
}
/*..........................................................................*/
void QS_onFlush(void) {
    if (l_sock != INVALID_SOCKET) {  // socket initialized?
        uint16_t nBytes = QS_TX_SIZE;
        uint8_t const *data;
        while ((data = QS_getBlock(&nBytes)) != (uint8_t *)0) {
            send(l_sock, (char const *)data, nBytes, 0);
            nBytes = QS_TX_SIZE;
        }
    }
}
/*..........................................................................*/
static void *idleThread(void *par) { // the expected P-Thread signature
    fd_set readSet;
    FD_ZERO(&readSet);

    (void)par; /* unused parameter */
    l_running = (uint8_t)1;
    while (l_running) {
        static struct timeval timeout = {
            (long)0, (long)(QS_IMEOUT_MS * 1000)
        };
        int nrec;
        uint16_t nBytes;
        uint8_t const *block;

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
        }

        nBytes = QS_TX_SIZE;
        //QF_CRIT_ENTRY(dummy);
        block = QS_getBlock(&nBytes);
        //QF_CRIT_EXIT(dummy);

        if (block != (uint8_t *)0) {
            send(l_sock, (char const *)block, nBytes, 0);
        }
    }

    return 0; // return success
}

/*--------------------------------------------------------------------------*/
#elif (QS_IMPL_OPTION == 2)

/*
* 2. Direct linking with the QSPY host application to perform direct output
*    to the console from the running application. (This option requires
*    the QSPY source code, which is part of the QTools collection).
*/
#include "qspy.h"

/*..........................................................................*/
static void *idleThread(void *par); // the expected P-Thread signature
static uint8_t l_running;

/*..........................................................................*/
bool QS_onStartup(void const */*arg*/) {
    static uint8_t qsBuf[4*1024]; // 4K buffer for Quantum Spy
    initBuf(qsBuf, sizeof(qsBuf));

    QSPY_config(QP_VERSION,         // version
                QS_OBJ_PTR_SIZE,    // objPtrSize
                QS_FUN_PTR_SIZE,    // funPtrSize
                QS_TIME_SIZE,       // tstampSize
                Q_SIGNAL_SIZE,      // sigSize,
                QF_EVENT_SIZ_SIZE,  // evtSize
                QF_EQUEUE_CTR_SIZE, // queueCtrSize
                QF_MPOOL_CTR_SIZE,  // poolCtrSize
                QF_MPOOL_SIZ_SIZE,  // poolBlkSize
                QF_TIMEEVT_CTR_SIZE,// tevtCtrSize
                (void *)0,          // matFile,
                (void *)0,
                (QSPY_CustParseFun)0); // customized parser function

    pthread_attr_t attr;
    struct sched_param param;
    pthread_t idle;

    // SCHED_FIFO corresponds to real-time preemptive priority-based
    // scheduler.
    // NOTE: This scheduling policy requires the superuser priviledges

    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    param.sched_priority = sched_get_priority_min(SCHED_FIFO);

    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (pthread_create(&idle, &attr, &idleThread, 0) != 0) {
        // Creating the p-thread with the SCHED_FIFO policy failed.
        // Most probably this application has no superuser privileges,
        // so we just fall back to the default SCHED_OTHER policy
        // and priority 0.
        pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
        param.sched_priority = 0;
        pthread_attr_setschedparam(&attr, &param);
        if (pthread_create(&idle, &attr, &idleThread, 0) == 0) {
            return false;
        }
    }
    pthread_attr_destroy(&attr);

    return true;
}
/*..........................................................................*/
void QS_onCleanup(void) {
    l_running = (uint8_t)0;
    QSPY_stop();
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t nBytes = 1024U;
    uint8_t const *block;
    while ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        QSPY_parse(block, nBytes);
        nBytes = 1024U;
    }
}
/*..........................................................................*/
void QSPY_onPrintLn(void) {
    fputs(QSPY_line, stdout);
    fputc('\n', stdout);
}
/*..........................................................................*/
static void *idleThread(void *par) { // the expected P-Thread signature
    (void)par;

    l_running = (uint8_t)1;
    while (l_running) {
        uint16_t nBytes = 256U;
        uint8_t const *block;
        struct timeval timeout = { 0, 10000 }; // timeout for select()

        QF_CRIT_ENTRY(dummy);
        block = QS_getBlock(&nBytes);
        QF_CRIT_EXIT(dummy);

        if (block != (uint8_t *)0) {
            QSPY_parse(block, nBytes);
        }
        select(0, 0, 0, 0, &timeout);   // sleep for a while
    }
    return 0; // return success
}

#else
#error Incorrect value of the QS_IMPL_OPTION macro
#endif // QS_IMPL_OPTION

//............................................................................
QSTimeCtr QS_onGetTime(void) {
    return (QSTimeCtr)clock(); // see NOTE01
}
//............................................................................
void QS_onReset(void) {
    QS_onCleanup();
    exit(0);
}
//............................................................................
//! callback function to execute a user command (to be implemented in BSP)
void QS_onCommand(uint8_t cmdId, uint32_t param1,
                   uint32_t param2, uint32_t param3)
{
    (void)cmdId;  // unused parameter
    (void)param1; // unused parameter
    (void)param2; // unused parameter
    (void)param3; // unused parameter
    //TBD
}

//****************************************************************************
// NOTE01:
// clock() is the most portable facility, but might not provide the desired
// granularity. Other, less-portable alternatives are clock_gettime(),
// rdtsc(), or gettimeofday().
//

#endif // Q_SPY
/*--------------------------------------------------------------------------*/
