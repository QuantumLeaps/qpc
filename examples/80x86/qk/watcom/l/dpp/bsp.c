/*****************************************************************************
* Product: DPP example, BSP for 80x86, QK/DOS, Open Watcom
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
#include "video.h"

#include <math.h>                                        /* to test the FPU */
#include <dos.h>                       /* for _dos_setvect()/_dos_getvect() */
#include <conio.h>                                      /* for inp()/outp() */
#include <stdlib.h>                                          /* for _exit() */

Q_DEFINE_THIS_FILE

/* Global-scope objects ----------------------------------------------------*/
Lib1_context * volatile impure_ptr1;
Lib2_context * volatile impure_ptr2;

/* Local-scope objects -----------------------------------------------------*/
static void interrupt (*l_dosTmrISR)();
static void interrupt (*l_dosKbdISR)();
static uint32_t l_delay = 0UL;     /* loop counter limit in BSP_busyDelay() */

#ifdef Q_SPY
    static uint16_t  l_uart_base;       /* QS data uplink UART base address */
    static QSTimeCtr l_tickTime;                 /* keeps timestamp at tick */
    static uint32_t  l_lastTime;                          /* last timestamp */
    static uint8_t   l_tmr;
    static uint8_t   l_kbd;

    #define UART_16550_TXFIFO_DEPTH 16

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };
#endif

#define TMR_VECTOR      0x08
#define KBD_VECTOR      0x09

#define TMR_ISR_PRIO    (0xFF)
#define KBD_ISR_PRIO    (0xFF - 1)

#define M_PI            3.14159265358979323846264338327950288

static void dispPreemptions(uint8_t pisr);       /* for testing, see NOTE01 */
static void busyDelay(void);                     /* for testing, see NOTE02 */

/*..........................................................................*/
void interrupt ISR_tmr(void) {
#ifdef Q_SPY
    l_tickTime += 0x10000;                           /* add 16-bit rollover */
#endif
    dispPreemptions(TMR_ISR_PRIO);              /* for testing only, NOTE01 */
    QK_ISR_ENTRY();                     /* inform QK about entering the ISR */

    QF_TICK(&l_tmr);          /* call QF_tick() outside of critical section */

    busyDelay();                                     /* for testing, NOTE02 */
    QK_ISR_EXIT();                       /* inform QK about exiting the ISR */
}
/*..........................................................................*/
void interrupt ISR_kbd(void) {
    uint8_t key;
    uint8_t kcr;

    dispPreemptions(KBD_ISR_PRIO);              /* for testing only, NOTE01 */
    QK_ISR_ENTRY();                     /* inform QK about entering the ISR */

    key = inp(0x60);          /* key scan code from the 8042 kbd controller */
    kcr = inp(0x61);                       /* get keyboard control register */
    outp(0x61, (uint8_t)(kcr | 0x80));       /* toggle acknowledge bit high */
    outp(0x61, kcr);                          /* toggle acknowledge bit low */
    if (key == (uint8_t)129) {                          /* ESC key pressed? */
        static QEvt term = {TERMINATE_SIG, 0};            /* static event */
        QF_PUBLISH(&term, &l_kbd);         /* publish to all interested AOs */
    }
    else {
        static QEvt test = {TEST_SIG, 0};                 /* static event */
        QACTIVE_POST(AO_Table, &test, &l_kbd);/* post a test event to Table */
    }
    Video_printNumAt(60, 12 + 0, VIDEO_FGND_YELLOW, key);/* display the key */

    busyDelay();                                     /* for testing, NOTE02 */
    QK_ISR_EXIT();                       /* inform QK about exiting the ISR */
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
void QK_onIdle(void) {
#ifdef Q_SPY
    if ((inp(l_uart_base + 5) & (1 << 5)) != 0) {         /* Tx FIFO empty? */
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
    uint8_t n;

    if (argc > 1) {
        l_delay = atol(argv[1]);    /* set the delay counter for busy delay */
    }
    if (argc > 2) {
        com = argv[2];
        (void)com;           /* avoid compiler warning if Q_SPY not defined */
    }
    if (!QS_INIT(com)) {                                   /* initialize QS */
        Q_ERROR();
    }

    QS_OBJ_DICTIONARY(&l_tmr);
    QS_OBJ_DICTIONARY(&l_kbd);

    Video_clearScreen(VIDEO_BGND_BLACK);
    Video_clearRect( 0,  0, 80,  7, VIDEO_BGND_LIGHT_GRAY);
    Video_clearRect( 0, 11, 80, 12, VIDEO_BGND_LIGHT_GRAY);
    Video_clearRect( 0, 12, 41, 23, VIDEO_BGND_BLUE);
    Video_clearRect(41, 12, 80, 23, VIDEO_BGND_RED);
    Video_clearRect( 0, 23, 80, 24, VIDEO_BGND_LIGHT_GRAY);

    n = VIDEO_FGND_BLUE;
    Video_printStrAt(10, 0, n, "  __");
    Video_printStrAt(10, 1, n, " /  |      _   _ -|-     _ _");
    Video_printStrAt(10, 2, n, " \\__| | |  _\\ | \\ | | | | \\ \\");
    Video_printStrAt(10, 3, n, "    | \\_/ |_| | | | \\_| | | |");
    Video_printStrAt(10, 4, n, "    |");
    n = VIDEO_FGND_RED;
    Video_printStrAt(43, 0, n, "    _       __ ");
    Video_printStrAt(43, 1, n, "|  /_\\     |  \\  TM");
    Video_printStrAt(43, 2, n, "|  \\_   _  |__/ _");
    Video_printStrAt(43, 3, n, "|       _\\ |   |_");
    Video_printStrAt(43, 4, n, "|___   |_| |    _|");
    Video_printStrAt(10, 5, VIDEO_FGND_BLUE,
                     "_____________________________________________________");
    Video_printStrAt(10, 6, VIDEO_FGND_RED,
                     "i n n o v a t i n g   e m b e d d e d   s y s t e m s");
    Video_printStrAt(18,  7, VIDEO_FGND_WHITE,
                     "Dining Philosophers Problem (DPP)");
    Video_printStrAt(18,  8, VIDEO_FGND_WHITE, "QEP/C");
    Video_printStrAt(28,  8, VIDEO_FGND_YELLOW, QEP_getVersion());
    Video_printStrAt(18,  9, VIDEO_FGND_WHITE, "QF/C");
    Video_printStrAt(28,  9, VIDEO_FGND_YELLOW, QF_getVersion());
    Video_printStrAt(18, 10, VIDEO_FGND_WHITE, "QK/C");
    Video_printStrAt(28, 10, VIDEO_FGND_YELLOW, QK_getVersion());
    Video_printStrAt(41, 10, VIDEO_FGND_WHITE, "Delay Counter");
    Video_printNumAt(54, 10, VIDEO_FGND_YELLOW, l_delay);

    Video_printStrAt( 1, 11, VIDEO_FGND_BLUE,
                     "Active Object   State     Preemptions");

    Video_printStrAt(42, 11, VIDEO_FGND_RED,
                     "ISR      Calls    Data    Preemptions");
    for (n = 0; n < N_PHILO; ++n) {
        Video_printStrAt( 1, 12 + n, VIDEO_FGND_WHITE, "Philosopher");
        Video_printNumAt(12, 12 + n, VIDEO_FGND_WHITE, n);
    }
    Video_printStrAt( 1, 12 + N_PHILO, VIDEO_FGND_WHITE,  "Table");
    Video_printStrAt(17, 12 + N_PHILO, VIDEO_FGND_YELLOW, "serving");

    Video_printStrAt(42, 12 + 0, VIDEO_FGND_WHITE,  "kbdISR");
    Video_printStrAt(42, 12 + 1, VIDEO_FGND_WHITE,  "tmrISR");

    Video_printStrAt(10, 23, VIDEO_FGND_BLUE,
         "* Copyright (c) Quantum Leaps, LLC * www.quantum-leaps.com *");
    Video_printStrAt(28, 24, VIDEO_FGND_LIGHT_RED,
         "<< Press Esc to quit >>");
}
/*..........................................................................*/
void BSP_displyPhilStat(uint8_t n, char const *stat) {
    Video_printStrAt(17, 12 + n, VIDEO_FGND_YELLOW, stat);

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int line) {
    Video_clearRect ( 0, 24, 80, 25, VIDEO_BGND_RED);
    Video_printStrAt( 0, 24, VIDEO_FGND_WHITE, "ASSERTION FAILED in file:");
    Video_printStrAt(26, 24, VIDEO_FGND_YELLOW, file);
    Video_printStrAt(57, 24, VIDEO_FGND_WHITE, "line:");
    Video_printNumAt(62, 24, VIDEO_FGND_YELLOW, line);
    QF_stop();
}
/*..........................................................................*/
void busyDelay(void) {                           /* for testing, see NOTE02 */
    uint32_t volatile i = l_delay << 4;
    while (i-- > 0UL) {                                   /* busy-wait loop */
    }
}
/*..........................................................................*/
void dispPreemptions(uint8_t pisr) {             /* for testing, see NOTE01 */
    if (pisr == TMR_ISR_PRIO) {
        static uint32_t tmrIsrCtr;               /* timer interrupt counter */
        Video_printNumAt(51, 12 + 1, VIDEO_FGND_YELLOW, ++tmrIsrCtr);
    }
    else if (pisr == KBD_ISR_PRIO) {
        static uint32_t kbdIsrCtr;                 /* kbd interrupt counter */
        Video_printNumAt(51, 12 + 0, VIDEO_FGND_YELLOW, ++kbdIsrCtr);
    }
    else {
        Q_ERROR();                         /* unexpected interrupt priority */
    }

    if (QK_intNest_ == (uint8_t)0) {          /* is this a task preemption? */
        if (QK_currPrio_ > (uint8_t)0) {
            static uint32_t preCtr[QF_MAX_ACTIVE + 1];
            Video_printNumAt(30, 12 + QK_currPrio_ - 1, VIDEO_FGND_YELLOW,
                             ++preCtr[QK_currPrio_]);
        }
    }
    else if (QK_intNest_ == (uint8_t)1) {      /* this is an ISR preemption */
        if (pisr == TMR_ISR_PRIO) {          /* TMR_ISR preempting KBD_ISR? */
            static uint32_t kbdPreCtr;        /* kbd ISR preemption counter */
            Video_printNumAt(71, 12 + 0, VIDEO_FGND_YELLOW, ++kbdPreCtr);
        }
        else {
            static uint32_t tmrPreCtr;        /* tmr ISR preemption counter */
            Video_printNumAt(71, 12 + 1, VIDEO_FGND_YELLOW, ++tmrPreCtr);
        }
    }
    else {
        Q_ERROR();         /* impossible ISR nesting level with just 2 ISRs */
    }
}

/*--------------------------------------------------------------------------*/
void lib1_reent_init(uint8_t prio) {
    impure_ptr1->x = (double)prio * (M_PI / 6.0);
}
/*..........................................................................*/
void lib1_test(void) {
    uint32_t volatile i = l_delay;
    while (i-- > 0UL) {
        double volatile r = sin(impure_ptr1->x) * sin(impure_ptr1->x)
                            + cos(impure_ptr1->x) * cos(impure_ptr1->x);
        Q_ASSERT(fabs(r - 1.0) < 1e-99);
    }
}
/*--------------------------------------------------------------------------*/
void lib2_reent_init(uint8_t prio) {
    impure_ptr2->y = (double)prio * (M_PI / 6.0) + M_PI;
}
/*..........................................................................*/
void lib2_test(void) {
    uint32_t volatile i = l_delay;
    while (i-- > 0UL) {
        double volatile r = sin(impure_ptr2->y) * sin(impure_ptr2->y)
                            + cos(impure_ptr2->y) * cos(impure_ptr2->y);
        Q_ASSERT(fabs(r - 1.0) < 1e-99);
    }
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
    static uint8_t qsBuf[2*1024];                 /* buffer for Quantum Spy */
    QS_initBuf(qsBuf, sizeof(qsBuf));
    return UART_config((char const *)arg, 115200UL);
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {            /* invoked with interrupts locked */
    uint32_t now;
    uint16_t count16;                         /* 16-bit count from the 8254 */

    if (l_tickTime != 0) {                        /* time tick has started? */
        outp(0x43, 0);                  /* latch the 8254's counter-0 count */
        count16 = (uint16_t)inp(0x40);    /* read the low byte of counter-0 */
        count16 += ((uint16_t)inp(0x40) << 8);        /* add on the hi byte */

        now = l_tickTime + (0x10000 - count16);

        if (l_lastTime > now) {             /* are we going "back" in time? */
            now += 0x10000;           /* assume that there was one rollover */
        }
        l_lastTime = now;
    }
    else {
        now = 0;
    }

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

/*****************************************************************************
* NOTE01:
* The function call to displayPreemptions() is added only to monitor the
* "asynchronous" preemptions within the QK.
*
* NOTE02:
* The call to busyDelay() is added only to extend the execution time of the
* code to increase the chance of an asynchronous preemption.
*/
