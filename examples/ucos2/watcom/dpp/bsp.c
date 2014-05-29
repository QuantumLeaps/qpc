/*****************************************************************************
* Product: DPP example, uC/OS-II, Open Watcom compiler
* Last updated for version 5.3.1
* Last updated on  2014-05-29
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"
#include "video.h"

#include <dos.h>      /* for _dos_setvect()/_dos_getvect() */
#include <conio.h>    /* for inp()/outp() */
#include <stdlib.h>   /* for _exit() */

Q_DEFINE_THIS_FILE

/* Local objects -----------------------------------------------------------*/
static void interrupt (*l_dosSpareISR)(void);
static void interrupt (*l_dosTickISR)(void);

static uint32_t l_delay = 0UL; /* limit for the loop counter in busyDelay() */

#ifdef Q_SPY
    static uint16_t l_uart_base; /* QS data uplink UART base address */
    static QSTimeCtr l_tickTime; /* keeps timetsamp at tick */
    static uint8_t   l_tickHook;
    static uint8_t   l_kbdTask;

    #define UART_16550_TXFIFO_DEPTH 16

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER
    };
#endif

#define TMR_VECTOR         0x08
#define DOS_CHAIN_VECTOR   0x81

/*..........................................................................*/
void ucosTask(void *pdata) {
    (void)pdata; /* avoid the compiler warning about unused parameter */

    QF_onStartup(); /* start interrupts including the clock tick, NOTE01 */

    for (;;) {

        OSTimeDly(OS_TICKS_PER_SEC/10); /* sleep for 1/10 s */

        if (kbhit()) { /* poll for a new keypress */
            uint8_t key = (uint8_t)getch();
            if (key == 0x1B) { /* is this the ESC key? */
                QF_PUBLISH(Q_NEW(QEvt, TERMINATE_SIG), &l_kbdTask);
            }
            else {  /* other key pressed */
                Video_printNumAt(30, 13 + N_PHILO, VIDEO_FGND_YELLOW, key);
            }
        }
    }
}
/*..........................................................................*/
void BSP_init(int argc, char *argv[]) {
    char const *com = "COM1";
    uint8_t n;

    if (argc > 1) {
        l_delay = atol(argv[1]); /* set the delay counter for busy delay */
    }
    if (argc > 2) {
        com = argv[2];
        (void)com; /* avoid compiler warning if Q_SPY not defined */
    }
    if (!QS_INIT(com)) { /* initialize QS */
        Q_ERROR();
    }

    QS_OBJ_DICTIONARY(&l_tickHook);
    QS_OBJ_DICTIONARY(&l_kbdTask);

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
                     "Dining Philosophers Example");
    Video_printStrAt(18,  8, VIDEO_FGND_WHITE, "QEP/C");
    Video_printStrAt(28,  8, VIDEO_FGND_YELLOW, QEP_getVersion());
    Video_printStrAt(18,  9, VIDEO_FGND_WHITE, "QF/C");
    Video_printStrAt(28,  9, VIDEO_FGND_YELLOW, QF_getVersion());
    Video_printStrAt(18, 10, VIDEO_FGND_WHITE, "uC/OS-II");

    /* uC/OS-II version is returned as an integer value multiplied by 100 */
    Video_printNumAt(29, 10, VIDEO_FGND_YELLOW, OSVersion()%100);
    Video_printStrAt(28, 10, VIDEO_FGND_YELLOW, "2.");

    Video_printStrAt( 1, 11, VIDEO_FGND_BLUE,
                     "Active Object   State        Data");

    for (n = 0; n < N_PHILO; ++n) {
        Video_printStrAt( 1, 12 + n, VIDEO_FGND_WHITE, "Philosopher");
        Video_printNumAt(12, 12 + n, VIDEO_FGND_WHITE, n + 1);
    }
    Video_printStrAt( 1, 12 + N_PHILO, VIDEO_FGND_WHITE,  "Table");
    Video_printStrAt(17, 12 + N_PHILO, VIDEO_FGND_YELLOW, "serving");
    Video_printStrAt( 1, 12 + N_PHILO + 1, VIDEO_FGND_WHITE,  "ucosTask");
    Video_printStrAt(17, 12 + N_PHILO + 1, VIDEO_FGND_YELLOW, "active");

    Video_printStrAt(4, 23, VIDEO_FGND_BLUE,
         "* Copyright (c) Quantum Leaps, LLC * www.state-machine.com *");
    Video_printStrAt(28, 24, VIDEO_FGND_LIGHT_RED,
         "<< Press Esc to quit >>");
}
/*..........................................................................*/
void BSP_displyPhilStat(uint8_t n, char const *stat) {
    Video_printStrAt(17, 12 + n, VIDEO_FGND_YELLOW, stat);

    QS_BEGIN(PHILO_STAT, AO_Philo[n]) /* application-specific record begin */
        QS_U8(1, n);   /* Philosopher number */
        QS_STR(stat);  /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_busyDelay(void) {
    uint32_t volatile i = l_delay;
    while (i-- > 0UL) { /* busy-wait loop */
    }
}
/*..........................................................................*/
void QF_onStartup(void) {

    /* divisor for the 8254 timer/counter */
    uint16_t count = (uint16_t)(((1193180U * 2) / OS_TICKS_PER_SEC + 1) >> 1);
    QF_CRIT_STAT_TYPE crit_stat;

    QF_CRIT_ENTRY(crit_stat);


    /* install uC/OS-II context switch vector */
    l_dosSpareISR = _dos_getvect(uCOS);
    _dos_setvect(uCOS, (void interrupt (*)(void))&OSCtxSw);

    /* install the original DOS timer vector for uC/OS-II to chain */
    l_dosTickISR  = _dos_getvect(TMR_VECTOR);
    _dos_setvect(DOS_CHAIN_VECTOR, l_dosTickISR);

    /* install the uC/OS-II tick vector */
    _dos_setvect(TMR_VECTOR, (void interrupt (*)(void))&OSTickISR);

    outp(0x43U, 0x36U); /* use mode-3 for timer 0 in the 8254 */
    outp(0x40U, count & 0xFFU);  /* load low  byte of timer 0 */
    outp(0x40U, (count >> 8) & 0xFFU); /* load high byte of timer 0 */
    QF_CRIT_EXIT(crit_stat);
}
/*..........................................................................*/
void QF_onCleanup(void) {
    QF_CRIT_STAT_TYPE crit_stat;

    QF_CRIT_ENTRY(crit_stat);
    outp(0x43U, 0x36U);  /* use mode-3 for timer 0 in the 8254 */
    outp(0x40U, 0U);     /* load low  byte of timer 0 */
    outp(0x40U, 0U);     /* load high byte of timer 0 */
    _dos_setvect(TMR_VECTOR, l_dosTickISR); /* restore the DOS vector */
    _dos_setvect(uCOS, l_dosSpareISR); /* restore the original DOS vector */
    QF_CRIT_EXIT(crit_stat);

    QS_EXIT(); /* cleanp the QS software tracing */
    _exit(0);  /* exit to DOS */
}
/*..........................................................................*/
void OSTimeTickHook(void) {
    QF_TICK(&l_tickHook);
}
/*..........................................................................*/
void OSTaskIdleHook(void) {
#ifdef Q_SPY
    if ((inp(l_uart_base + 5) & (1 << 5)) != 0) { /* Tx FIFO empty? */
        uint16_t fifo = UART_16550_TXFIFO_DEPTH;  /* 16550 Tx FIFO depth */
        uint8_t const *block;
        QF_CRIT_STAT_TYPE crit_stat;
        QF_CRIT_ENTRY(crit_stat);
        block = QS_getBlock(&fifo); /* try to get next block to transmit */
        QF_CRIT_EXIT(crit_stat);
        while (fifo-- != 0) {  /* any bytes in the block? */
            outp(l_uart_base + 0, *block++);
        }
    }
#endif
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int line) {
    Video_clearRect(0, 24, 80, 25, VIDEO_BGND_RED);
    Video_printStrAt(0, 24, VIDEO_FGND_WHITE, "ASSERTION FAILED in file:");
    Video_printStrAt(26, 24, VIDEO_FGND_YELLOW, file);
    Video_printStrAt(57, 24, VIDEO_FGND_WHITE, "line:");
    Video_printNumAt(62, 24, VIDEO_FGND_YELLOW, line);

    QF_stop();
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY /* define QS callbacks */

/*..........................................................................*/
static uint8_t UART_config(char const *comName, uint32_t baud) {
    switch (comName[3]) { /* Set the base address of the COMx port */
        case '1': l_uart_base = (uint16_t)0x03F8; break; /* COM1 */
        case '2': l_uart_base = (uint16_t)0x02F8; break; /* COM2 */
        case '3': l_uart_base = (uint16_t)0x03E8; break; /* COM3 */
        case '4': l_uart_base = (uint16_t)0x02E8; break; /* COM4 */
        default: return (uint8_t)0; /* COM port out of range failure */
    }
    baud = (uint16_t)(115200UL / baud); /* divisor for baud rate */
    outp(l_uart_base + 3, (1 << 7));    /* Set divisor access bit (DLAB) */
    outp(l_uart_base + 0, (uint8_t)baud); /* Load divisor */
    outp(l_uart_base + 1, (uint8_t)(baud >> 8));
    outp(l_uart_base + 3, (1 << 1) | (1 << 0)); /* LCR:8-bits,no p,1stop */
    outp(l_uart_base + 4, (1 << 3) | (1 << 1) | (1 << 0)); /* DTR,RTS,Out2 */
    outp(l_uart_base + 1, 0); /* Put UART into the polling FIFO mode */
    outp(l_uart_base + 2, (1 << 2) | (1 << 0)); /* FCR: enable, TX clear */

    return (uint8_t)1; /* success */
}
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[1*1024]; /* buffer for Quantum Spy */
    QS_initBuf(qsBuf, sizeof(qsBuf));
    return UART_config((char const *)arg, 115200UL);
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) { /* invoked with interrupts locked */
    static uint32_t l_lastTime;
    uint32_t now;
    uint16_t count16; /* 16-bit count from the 8254 */

    outp(0x43, 0); /* latch the 8254's counter-0 count */
    count16 = (uint16_t)inp(0x40); /* read the low byte of counter-0 */
    count16 += ((uint16_t)inp(0x40) << 8); /* add on the hi byte */

    now = l_tickTime + (0x10000 - count16);

    if (l_lastTime > now) { /* are we going "back" in time? */
        now += 0x10000U; /* assume that there was one rollover */
    }
    l_lastTime = now;

    return (QSTimeCtr)now;
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t fifo = UART_16550_TXFIFO_DEPTH; /* 16550 Tx FIFO depth */
    uint8_t const *block;
    while ((block = QS_getBlock(&fifo)) != (uint8_t *)0) {
        /* busy-wait until TX FIFO empty */
        while ((inp(l_uart_base + 5) & (1 << 5)) == 0) {
        }

        while (fifo-- != 0) { /* any bytes in the block? */
            outp(l_uart_base + 0, *block++);
        }
        fifo = UART_16550_TXFIFO_DEPTH; /* re-load 16550 Tx FIFO depth */
    }
}
#endif /* Q_SPY */
/*--------------------------------------------------------------------------*/
