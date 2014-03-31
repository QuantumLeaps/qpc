/*****************************************************************************
* Product: DPP example, POSIX
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 18, 2013
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
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>                            /* for memcpy() and memset() */
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

Q_DEFINE_THIS_FILE

/* Local objects -----------------------------------------------------------*/
static struct termios l_tsav;   /* structure with saved terminal attributes */
static uint32_t l_rnd;                                       /* random seed */

#ifdef Q_SPY
    enum {
        PHILO_STAT = QS_USER
    };
    static uint8_t const l_clock_tick = 0U;
#endif

/*..........................................................................*/
void QF_onStartup(void) {                               /* startup callback */
    struct termios tio;                     /* modified terminal attributes */

    tcgetattr(0, &l_tsav);          /* save the current terminal attributes */
    tcgetattr(0, &tio);           /* obtain the current terminal attributes */
    tio.c_lflag &= ~(ICANON | ECHO);   /* disable the canonical mode & echo */
    tcsetattr(0, TCSANOW, &tio);                  /* set the new attributes */

    QF_setTickRate(BSP_TICKS_PER_SEC);         /* set the desired tick rate */
}
/*..........................................................................*/
void QF_onCleanup(void) {                               /* cleanup callback */
    printf("\nBye! Bye!\n");
    tcsetattr(0, TCSANOW, &l_tsav);/* restore the saved terminal attributes */
    QS_EXIT();                                    /* perfomr the QS cleanup */
}
/*..........................................................................*/
void QF_onClockTick(void) {
    struct timeval timeout = { 0 };                 /* timeout for select() */
    fd_set con;                          /* FD set representing the console */

    QF_TICK(&l_clock_tick);         /* perform the QF clock tick processing */

    FD_ZERO(&con);
    FD_SET(0, &con);
    /* check if a console input is available, returns immediately */
    if (0 != select(1, &con, 0, 0, &timeout)) {      /* any descriptor set? */
        char ch;
        read(0, &ch, 1);
        if (ch == '\33') {                                  /* ESC pressed? */
            QF_PUBLISH(Q_NEW(QEvt, TERMINATE_SIG), &l_clock_tick);
        }
        else if (ch == 'p') {
            QF_PUBLISH(Q_NEW(QEvt, PAUSE_SIG), &l_clock_tick);
        }
    }
}
/*..........................................................................*/
void BSP_init() {
    printf("Dining Philosopher Problem example"
           "\nQEP %s\nQF  %s\n"
           "Press p to pause/un-pause\n"
           "Press ESC to quit...\n",
           QEP_getVersion(),
           QF_getVersion());

    BSP_randomSeed(1234U);
    Q_ALLEGE(QS_INIT((void *)0));
    QS_OBJ_DICTIONARY(&l_clock_tick);   /* must be called *after* QF_init() */
    QS_USR_DICTIONARY(PHILO_STAT);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
    QF_stop();
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    printf("Philosopher %2d is %s\n", (int)n, stat);

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    printf("Paused is %s\n", paused ? "ON" : "OFF");
}
/*..........................................................................*/
uint32_t BSP_random(void) {  /* a very cheap pseudo-random-number generator */
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
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int line) {
    QS_ASSERTION(file, line);
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    QF_stop();
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY                                         /* define QS callbacks */

#include "qspy.h"

static uint8_t l_running;

/*..........................................................................*/
static void *idleThread(void *par) {     /* the expected P-Thread signature */
    (void)par;

    l_running = (uint8_t)1;
    while (l_running) {
        uint16_t nBytes = 256;
        uint8_t const *block;
        struct timeval timeout = { 0 };             /* timeout for select() */

        QF_CRIT_ENTRY(dummy);
        block = QS_getBlock(&nBytes);
        QF_CRIT_EXIT(dummy);

        if (block != (uint8_t *)0) {
            QSPY_parse(block, nBytes);
        }
        timeout.tv_usec = 10000;
        select(0, 0, 0, 0, &timeout);                  /* sleep for a while */
    }
    return 0;                                             /* return success */
}
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[4*1024];                 // 4K buffer for Quantum Spy
    QS_initBuf(qsBuf, sizeof(qsBuf));

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

    QS_FILTER_ON(QS_ALL_RECORDS);

//    QS_FILTER_OFF(QS_QEP_STATE_ENTRY);
//    QS_FILTER_OFF(QS_QEP_STATE_EXIT);
//    QS_FILTER_OFF(QS_QEP_STATE_INIT);
//    QS_FILTER_OFF(QS_QEP_TRAN_HIST);
//    QS_FILTER_OFF(QS_QEP_INTERN_TRAN);
//    QS_FILTER_OFF(QS_QEP_TRAN);
//    QS_FILTER_OFF(QS_QEP_IGNORED);
//    QS_FILTER_OFF(QS_QEP_DISPATCH);
//    QS_FILTER_OFF(QS_QEP_UNHANDLED);

    QS_FILTER_OFF(QS_QF_ACTIVE_ADD);
    QS_FILTER_OFF(QS_QF_ACTIVE_REMOVE);
    QS_FILTER_OFF(QS_QF_ACTIVE_SUBSCRIBE);
//    QS_FILTER_OFF(QS_QF_ACTIVE_UNSUBSCRIBE);
    QS_FILTER_OFF(QS_QF_ACTIVE_POST_FIFO);
//    QS_FILTER_OFF(QS_QF_ACTIVE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET_LAST);
    QS_FILTER_OFF(QS_QF_EQUEUE_INIT);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_FIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET_LAST);
    QS_FILTER_OFF(QS_QF_MPOOL_INIT);
    QS_FILTER_OFF(QS_QF_MPOOL_GET);
    QS_FILTER_OFF(QS_QF_MPOOL_PUT);
    QS_FILTER_OFF(QS_QF_PUBLISH);
    QS_FILTER_OFF(QS_QF_NEW);
    QS_FILTER_OFF(QS_QF_GC_ATTEMPT);
    QS_FILTER_OFF(QS_QF_GC);
    QS_FILTER_OFF(QS_QF_TICK);
    QS_FILTER_OFF(QS_QF_TIMEEVT_ARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_AUTO_DISARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM_ATTEMPT);
    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_REARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_POST);
    QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
    QS_FILTER_OFF(QS_QF_CRIT_EXIT);
    QS_FILTER_OFF(QS_QF_ISR_ENTRY);
    QS_FILTER_OFF(QS_QF_ISR_EXIT);

    {
        pthread_attr_t attr;
        struct sched_param param;
        pthread_t idle;

        /* SCHED_FIFO corresponds to real-time preemptive priority-based
        * scheduler.
        * NOTE: This scheduling policy requires the superuser priviledges
        */
        pthread_attr_init(&attr);
        pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
        param.sched_priority = sched_get_priority_min(SCHED_FIFO);

        pthread_attr_setschedparam(&attr, &param);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        if (pthread_create(&idle, &attr, &idleThread, 0) != 0) {
               /* Creating the p-thread with the SCHED_FIFO policy failed.
               * Most probably this application has no superuser privileges,
               * so we just fall back to the default SCHED_OTHER policy
               * and priority 0.
               */
            pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
            param.sched_priority = 0;
            pthread_attr_setschedparam(&attr, &param);
            Q_ALLEGE(pthread_create(&idle, &attr, &idleThread, 0) == 0);
        }
        pthread_attr_destroy(&attr);
    }

    return (uint8_t)1;
}
/*..........................................................................*/
void QS_onCleanup(void) {
    l_running = (uint8_t)0;
    QSPY_stop();
}
/*..........................................................................*/
void QS_onFlush(void) {
    for (;;) {
        uint16_t nBytes = 1024;
        uint8_t const *block;

        QF_CRIT_ENTRY(dummy);
        block = QS_getBlock(&nBytes);
        QF_CRIT_EXIT(dummy);

        if (block != (uint8_t const *)0) {
            QSPY_parse(block, nBytes);
            nBytes = 1024;
        }
        else {
            break;
        }
    }
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {
    return (QSTimeCtr)clock();                                   // see NOTE01
}
/*..........................................................................*/
void QSPY_onPrintLn(void) {
    fputs(QSPY_line, stdout);
    fputc('\n', stdout);
}

/*****************************************************************************
* NOTE01:
* clock() is the most portable facility, but might not provide the desired
* granularity. Other, less-portable alternatives are clock_gettime(), rdtsc(),
* or gettimeofday().
*/

#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/
