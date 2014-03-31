/*****************************************************************************
* Product: BSP for State-Local Storage Example, 80x86, Vanilla, Open Watcom
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
#include "qp_port.h"                        /* the port of the QP framework */
#include "num_ent.h"
#include "ui.h"
#include "bsp.h"
#include "video.h"

#include <dos.h>                       /* for _dos_setvect()/_dos_getvect() */
#include <conio.h>                                      /* for inp()/outp() */
#include <stdlib.h>                                          /* for _exit() */

Q_DEFINE_THIS_FILE

/* Local-scope objects -----------------------------------------------------*/
static void interrupt (*l_dosTmrISR)();
static void interrupt (*l_dosKbdISR)();

#ifdef Q_SPY
    static uint16_t l_uart_base;        /* QS data uplink UART base address */
    QSTimeCtr       l_tickTime;                  /* keeps timetsamp at tick */
    static uint8_t  l_tmr;
    static uint8_t  l_kbd;

    #define UART_TXFIFO_DEPTH 16
#endif

#define TMR_VECTOR      0x08
#define KBD_VECTOR      0x09

/*..........................................................................*/
static void interrupt ISR_tmr() {
    QF_ISR_ENTRY();                                /* QF-specific ISR entry */

    QF_TICK(&l_tmr);          /* call QF_tick() outside of critical section */

#ifdef Q_SPY
    l_tickTime += 0x10000;
#endif

    QF_ISR_EXIT();                                  /* QF-specific ISR exit */
}
/*..........................................................................*/
static void interrupt ISR_kbd() {
    uint8_t key;
    uint8_t kcr;
    KeyboardEvt *ke;

    QF_ISR_ENTRY();                                /* QF-specific ISR entry */

    key = inp(0x60);              /* key scan code from 8042 kbd controller */
    kcr = inp(0x61);                       /* get keyboard control register */
    outp(0x61, (uint8_t)(kcr | 0x80));       /* toggle acknowledge bit high */
    outp(0x61, kcr);                          /* toggle acknowledge bit low */

    switch (key) {
        default: {
//            Video_printNumAt(1, 24, VIDEO_FGND_YELLOW, key);
            break;
        }
        case 2:                                                      /* '1' */
        case 3:                                                      /* '2' */
        case 4:                                                      /* '3' */
        case 5:                                                      /* '4' */
        case 6:                                                      /* '5' */
        case 7:                                                      /* '6' */
        case 8:                                                      /* '7' */
        case 9:                                                      /* '8' */
        case 10: {                                                   /* '9' */
            ke = Q_NEW(KeyboardEvt, DIGIT_1_9_SIG);
            ke->key_code = '0' + (key - 1);
            QACTIVE_POST(AO_UI, (QEvt *)ke, &l_kbd);
            break;
        }
        case 11: {                                                   /* '0' */
            KeyboardEvt *ke = Q_NEW(KeyboardEvt, DIGIT_0_SIG);
            ke->key_code = '0';
            QACTIVE_POST(AO_UI, (QEvt *)ke, &l_kbd);
            break;
        }
        case 52: {                                                   /* '.' */
            ke = Q_NEW(KeyboardEvt, POINT_SIG);
            ke->key_code = '.';
            QACTIVE_POST(AO_UI, (QEvt *)ke, &l_kbd);
            break;
        }
        case 12: {                                                   /* '-' */
            ke = Q_NEW(KeyboardEvt, NEG_SIG);
            ke->key_code = '-';
            QACTIVE_POST(AO_UI, (QEvt *)ke, &l_kbd);
            break;
        }
        case 46: {                                                   /* 'c' */
            static QEvt const ce = { C_SIG, 0, 0 };
            QACTIVE_POST(AO_UI, (QEvt *)&ce, &l_kbd);
            break;
        }
        case 18: {                                                   /* 'e' */
            static QEvt const cee = { CE_SIG, 0, 0 };
            QACTIVE_POST(AO_UI, (QEvt *)&cee, &l_kbd);
            break;
        }
        case 28: {                                                 /* ENTER */
            static QEvt const ee = { ENTER_SIG, 0, 0 };
            QACTIVE_POST(AO_UI, (QEvt *)&ee, &l_kbd);
            break;
        }
        case 200: {                                             /* UP-arrow */
            static QEvt const ue = { UP_SIG, 0, 0 };
            QACTIVE_POST(AO_UI, (QEvt *)&ue, &l_kbd);
            break;
        }
        case 208: {                                           /* DOWN-arrow */
            static QEvt const de = { DOWN_SIG, 0, 0 };
            QACTIVE_POST(AO_UI, (QEvt *)&de, &l_kbd);
            break;
        }
        case 59: {                                                    /* F1 */
            static QEvt const he = { HELP_SIG, 0, 0 };
            QACTIVE_POST(AO_UI, (QEvt *)&he, &l_kbd);
            break;
        }
        case 129: {                                                  /* ESC */
            static QEvt const qe = { QUIT_SIG, 0, 0 };
            QF_PUBLISH(&qe, &l_kbd);
            break;
        }
    }

    QF_ISR_EXIT();                                  /* QF-specific ISR exit */
}

/*..........................................................................*/
void BSP_init(int argc, char *argv[]) {
    char const *com = "COM1";

    com = com;                  /* avoid compiler warning if QS is not used */

    if (argc > 1) {
        com = argv[1];
    }
    if (!QS_INIT(com)) {                                   /* initialize QS */
        Q_ERROR();
    }
}
/*..........................................................................*/
void QF_onStartup(void) {
    uint16_t count;
                                      /* save the origingal DOS vectors ... */
    l_dosTmrISR   = _dos_getvect(TMR_VECTOR);
    l_dosKbdISR   = _dos_getvect(KBD_VECTOR);

    QF_INT_DISABLE();
    count = (uint16_t)(((1193180 * 2) / BSP_TICKS_PER_SEC + 1) >> 1);
    outp(0x43, 0x36);                 /* use mode-3 for timer 0 in the 8254 */
    outp(0x40, count & 0xFF);                  /* load low  byte of timer 0 */
    outp(0x40, (count >> 8) & 0xFF);           /* load high byte of timer 0 */

    _dos_setvect(TMR_VECTOR, &ISR_tmr);
    _dos_setvect(KBD_VECTOR, &ISR_kbd);
    QF_INT_ENABLE();
}
/*..........................................................................*/
void QF_onCleanup(void) {
                               /* restore the DOS system clock tick rate... */
    QF_INT_DISABLE();
    outp(0x43, 0x36);                 /* use mode-3 for timer 0 in the 8254 */
    outp(0x40, 0);                             /* load low  byte of timer 0 */
    outp(0x40, 0);                             /* load high byte of timer 0 */
                                   /* restore the original DOS vectors ...  */
    _dos_setvect(TMR_VECTOR, l_dosTmrISR);
    _dos_setvect(KBD_VECTOR, l_dosKbdISR);
    QF_INT_ENABLE();

    QS_EXIT();                                                   /* exit QS */
    _exit(0);                                                /* exit to DOS */
}
/*..........................................................................*/
void QF_onIdle(void) {
    QF_INT_ENABLE();                            /* always enable interrupts */
#ifdef Q_SPY
    if ((inp(l_uart_base + 5) & (1 << 5)) != 0) {         /* Tx FIFO empty? */
        uint16_t fifo = UART_TXFIFO_DEPTH;    /* depth of the 15550 Tx FIFO */
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
/* this function is used by the QP embedded systems-friendly assertions     */
void Q_onAssert(char const * const file, int line) {
    Video_clearRect ( 0, 24, 80, 25, VIDEO_BGND_RED);
    Video_printStrAt( 0, 24, VIDEO_FGND_WHITE, "ASSERTION FAILED in file:");
    Video_printStrAt(26, 24, VIDEO_FGND_YELLOW, file);
    Video_printStrAt(57, 24, VIDEO_FGND_WHITE, "line:");
    Video_printNumAt(62, 24, VIDEO_FGND_YELLOW, line);
    _exit(-1);
}

//----------------------------------------------------------------------------
#ifdef Q_SPY

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
    outp(l_uart_base + 4, (1 << 3) | (1 << 1) | (1 << 0));   /* DTR,RTS,Out */
    outp(l_uart_base + 1, 0);        /* Put UART into the polling FIFO mode */
    outp(l_uart_base + 2, (1 << 2) | (1 << 0));    /* FCR: enable, TX clear */

    return (uint8_t)1;                                           /* success */
}
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[1*1024];                 /* buffer for Quantum Spy */
    QS_initBuf(qsBuf, sizeof(qsBuf));

    QS_FILTER_ON(QS_SIG_DICTIONARY);
    QS_FILTER_ON(QS_OBJ_DICTIONARY);
    QS_FILTER_ON(QS_FUN_DICTIONARY);
    QS_FILTER_ON(QS_ASSERT);

    QS_FILTER_ON(QS_QEP_STATE_ENTRY);
    QS_FILTER_ON(QS_QEP_STATE_EXIT);
    QS_FILTER_ON(QS_QEP_STATE_INIT);
    QS_FILTER_ON(QS_QEP_TRAN_HIST);
    QS_FILTER_ON(QS_QEP_INTERN_TRAN);
    QS_FILTER_ON(QS_QEP_TRAN);
    QS_FILTER_ON(QS_QEP_IGNORED);

    QS_FILTER_ON(QS_QF_ACTIVE_ADD);
    QS_FILTER_ON(QS_QF_ACTIVE_REMOVE);
    QS_FILTER_ON(QS_QF_ACTIVE_SUBSCRIBE);
    QS_FILTER_ON(QS_QF_ACTIVE_UNSUBSCRIBE);
    QS_FILTER_ON(QS_QF_ACTIVE_POST_FIFO);
    QS_FILTER_ON(QS_QF_ACTIVE_POST_LIFO);
    QS_FILTER_ON(QS_QF_ACTIVE_GET);
    QS_FILTER_ON(QS_QF_ACTIVE_GET_LAST);
    QS_FILTER_ON(QS_QF_EQUEUE_INIT);
    QS_FILTER_ON(QS_QF_EQUEUE_POST_FIFO);
    QS_FILTER_ON(QS_QF_EQUEUE_POST_LIFO);
    QS_FILTER_ON(QS_QF_EQUEUE_GET);
    QS_FILTER_ON(QS_QF_EQUEUE_GET_LAST);
    QS_FILTER_ON(QS_QF_MPOOL_INIT);
    QS_FILTER_ON(QS_QF_MPOOL_GET);
    QS_FILTER_ON(QS_QF_MPOOL_PUT);
    QS_FILTER_ON(QS_QF_PUBLISH);
    QS_FILTER_ON(QS_QF_NEW);
    QS_FILTER_ON(QS_QF_GC_ATTEMPT);
    QS_FILTER_ON(QS_QF_GC);
//    QS_FILTER_ON(QS_QF_TICK);
    QS_FILTER_ON(QS_QF_TIMEEVT_ARM);
    QS_FILTER_ON(QS_QF_TIMEEVT_AUTO_DISARM);
    QS_FILTER_ON(QS_QF_TIMEEVT_DISARM_ATTEMPT);
    QS_FILTER_ON(QS_QF_TIMEEVT_DISARM);
    QS_FILTER_ON(QS_QF_TIMEEVT_REARM);
    QS_FILTER_ON(QS_QF_TIMEEVT_POST);
//    QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
//    QS_FILTER_OFF(QS_QF_CRIT_EXIT);
//    QS_FILTER_ON(QS_QF_ISR_ENTRY);
//    QS_FILTER_ON(QS_QF_ISR_EXIT);

    return UART_config((char const *)arg, 115200UL);
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {           /* invoked with interrupts locked */
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
    uint16_t b;
    while ((b = QS_getByte()) != QS_EOD) { /* next QS trace byte available? */
        while ((inp(l_uart_base + 5) & (1 << 5)) == 0) {      /* not empty? */
        }
        outp(l_uart_base + 0, (uint8_t)b);       /* put the byte to TX FIFO */
    }
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/
