/*****************************************************************************
* Product: DPP example, 80x86, Vanilla/DOS, Open Watcom
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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

#include <dos.h>                       /* for _dos_setvect()/_dos_getvect() */
#include <conio.h>                                      /* for inp()/outp() */
#include <stdlib.h>                                          /* for _exit() */
#include <stdio.h>

Q_DEFINE_THIS_FILE

/* Local-scope objects -----------------------------------------------------*/
static void interrupt (*l_dosTmrISR)();
static void interrupt (*l_dosKbdISR)();
static uint32_t l_delay = 0UL; /* limit for the loop counter in busyDelay() */

#ifdef Q_SPY
    static uint16_t  l_uart_base;       /* QS data uplink UART base address */
    static QSTimeCtr l_tickTime;                 /* keeps timetsamp at tick */
    static uint8_t   l_tmr;
    static uint8_t   l_kbd;

    #define UART_16550_TXFIFO_DEPTH 16

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };
#endif

#define TMR_VECTOR      0x08
#define KBD_VECTOR      0x09

/*..........................................................................*/
void interrupt ISR_tmr(void) {
    QF_ISR_ENTRY();                                /* QF-specific ISR entry */

    QF_TICK(&l_tmr);          /* call QF_tick() outside of critical section */
#ifdef Q_SPY
    l_tickTime += 0x10000;                           /* add 16-bit rollover */
#endif

    QF_ISR_EXIT();                                  /* QF-specific ISR exit */
}
/*..........................................................................*/
void interrupt ISR_kbd(void) {
    uint8_t key;
    uint8_t kcr;

    QF_ISR_ENTRY();                                /* QF-specific ISR entry */

    key = inp(0x60);          /* key scan code from the 8042 kbd controller */
    kcr = inp(0x61);                       /* get keyboard control register */
    outp(0x61, (uint8_t)(kcr | 0x80));       /* toggle acknowledge bit high */
    outp(0x61, kcr);                          /* toggle acknowledge bit low */
    if (key == (uint8_t)129) {                          /* ESC key pressed? */
        static QEvt term = {TERMINATE_SIG, 0, 0 };        /* static event */
        QF_PUBLISH(&term, &l_kbd);         /* publish to all interested AOs */
    }

    QF_ISR_EXIT();                                  /* QF-specific ISR exit */
}
/*..........................................................................*/
void QF_onStartup(void) {
                                      /* save the origingal DOS vectors ... */
    l_dosTmrISR = _dos_getvect(TMR_VECTOR);
    l_dosKbdISR = _dos_getvect(KBD_VECTOR);

    QF_INT_DISABLE();
    _dos_setvect(TMR_VECTOR, &ISR_tmr);
    _dos_setvect(KBD_VECTOR, &ISR_kbd);
    QF_INT_ENABLE();
}
/*..........................................................................*/
void QF_onCleanup(void) {           /* restore the original DOS vectors ... */
    QF_INT_DISABLE();
    _dos_setvect(TMR_VECTOR, l_dosTmrISR);
    _dos_setvect(KBD_VECTOR, l_dosKbdISR);
    QF_INT_ENABLE();

    QS_EXIT();                                                   /* exit QS */
    _exit(0);                                                /* exit to DOS */
}
/*..........................................................................*/
void QF_onIdle(void) {                     /* called with interrupts LOCKED */

    QF_INT_ENABLE();                            /* always enable interrupts */

#ifdef Q_SPY
    if ((inp(l_uart_base + 5) & (1 << 5)) != 0) {             /* THR empty? */
        uint16_t fifo = UART_16550_TXFIFO_DEPTH;     /* 16550 Tx FIFO depth */
        uint8_t const *block;
        QF_INT_DISABLE();
        block = QS_getBlock(&fifo);    /* try to get next block to transmit */
        QF_INT_ENABLE();
        while (fifo-- != 0) {                    /* any bytes in the block? */
            outp(l_uart_base + 0, *block++);
        }
    }
#endif
}
/*..........................................................................*/
void BSP_init(int argc, char *argv[]) {
    char const *com = "COM1";

    if (argc > 1) {
        l_delay = atol(argv[1]);    /* set the delay counter for busy delay */
    }
    if (argc > 2) {
        com = argv[2];
        com = com;         /* avoid compiler warning about unused parameter */
    }
    if (!QS_INIT(com)) {                                   /* initialize QS */
        Q_ERROR();
    }

    QS_OBJ_DICTIONARY(&l_tmr);
    QS_OBJ_DICTIONARY(&l_kbd);

    printf("Dining Philosopher Problem example"
           "\nQEP %s\nQF  %s\n"
           "Press ESC to quit...\n",
           QEP_getVersion(),
           QF_getVersion());
}
/*..........................................................................*/
void BSP_displyPhilStat(uint8_t n, char const *stat) {
    printf("Philosopher %2d is %s\n", (int)n, stat);

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_busyDelay(void) {
    uint32_t volatile i = l_delay;
    while (i-- > 0UL) {                                   /* busy-wait loop */
    }
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    QF_stop();
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY                                         /* define QS callbacks */

/*..........................................................................*/
static uint8_t UART_config(char const *comName, uint32_t baud) {
    switch (comName[3]) {          /* Set the base address of the COMx port */
        case '1': l_uart_base = (uint16_t)0x03F8; break;            /* COM1 */
        case '2': l_uart_base = (uint16_t)0x02F8; break;            /* COM2 */
        case '3': l_uart_base = (uint16_t)0x03E8; break;            /* COM3 */
        case '4': l_uart_base = (uint16_t)0x02E8; break;            /* COM4 */
        default: return (uint8_t)0;        /* COM port out of range failure */
    }
    baud = (uint16_t)(115200UL / baud);            /* divisor for baud rate */
    outp(l_uart_base + 3, (1 << 7));       /* Set divisor access bit (DLAB) */
    outp(l_uart_base + 0, (uint8_t)baud);                   /* Load divisor */
    outp(l_uart_base + 1, (uint8_t)(baud >> 8));
    outp(l_uart_base + 3, (1 << 1) | (1 << 0));    /* LCR:8-bits,no p,1stop */
    outp(l_uart_base + 4, (1 << 3) | (1 << 1) | (1 << 0));  /* DTR,RTS,Out2 */
    outp(l_uart_base + 1, 0);        /* Put UART into the polling FIFO mode */
    outp(l_uart_base + 2, (1 << 2) | (1 << 0));    /* FCR: enable, TX clear */

    return (uint8_t)1;                                           /* success */
}
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[1*1024];                 /* buffer for Quantum Spy */
    QS_initBuf(qsBuf, sizeof(qsBuf));

                                                 /* setup the QS filters... */
    QS_FILTER_ON(QS_ALL_RECORDS);

//    QS_FILTER_OFF(QS_QEP_STATE_ENTRY);
//    QS_FILTER_OFF(QS_QEP_STATE_EXIT);
//    QS_FILTER_OFF(QS_QEP_STATE_INIT);
//    QS_FILTER_OFF(QS_QEP_TRAN_HIST);
//    QS_FILTER_OFF(QS_QEP_INTERN_TRAN);
//    QS_FILTER_OFF(QS_QEP_TRAN);
//    QS_FILTER_OFF(QS_QEP_IGNORED);

    QS_FILTER_OFF(QS_QF_ACTIVE_ADD);
    QS_FILTER_OFF(QS_QF_ACTIVE_REMOVE);
    QS_FILTER_OFF(QS_QF_ACTIVE_SUBSCRIBE);
    QS_FILTER_OFF(QS_QF_ACTIVE_UNSUBSCRIBE);
    QS_FILTER_OFF(QS_QF_ACTIVE_POST_FIFO);
    QS_FILTER_OFF(QS_QF_ACTIVE_POST_LIFO);
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
//    QS_FILTER_OFF(QS_QF_TICK);
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

    return UART_config((char const *)arg, 115200UL);
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {            /* invoked with interrupts locked */
    static uint32_t l_lastTime;
    uint32_t now;
    uint16_t count16;                         /* 16-bit count from the 8254 */

    outp(0x43, 0);                      /* latch the 8254's counter-0 count */
    count16 = (uint16_t)inp(0x40);        /* read the low byte of counter-0 */
    count16 += ((uint16_t)inp(0x40) << 8);            /* add on the hi byte */

    now = l_tickTime + (0x10000 - count16);

    if (l_lastTime > now) {                 /* are we going "back" in time? */
        now += 0x10000;               /* assume that there was one rollover */
    }
    l_lastTime = now;

    return (QSTimeCtr)now;
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t fifo = UART_16550_TXFIFO_DEPTH;         /* 16550 Tx FIFO depth */
    uint8_t const *block;
    while ((block = QS_getBlock(&fifo)) != (uint8_t *)0) {
                                           /* busy-wait until TX FIFO empty */
        while ((inp(l_uart_base + 5) & (1 << 5)) == 0) {
        }

        while (fifo-- != 0) {                    /* any bytes in the block? */
            outp(l_uart_base + 0, *block++);
        }
        fifo = UART_16550_TXFIFO_DEPTH;      /* re-load 16550 Tx FIFO depth */
    }
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/
