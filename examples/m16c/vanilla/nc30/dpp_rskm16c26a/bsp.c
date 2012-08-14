/*****************************************************************************
* Product: DPP example, RSKM16C26A board
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Aug 30, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

/* Local objects -----------------------------------------------------------*/
#ifdef Q_SPY
    #define QS_BUF_SIZE        (256)
    #define BAUD_RATE          115200

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };
                                        /* number of underflows of Timer A1 */
    static uint16_t l_nTimerA1Underflows = 1;
#endif

                 /* priority of the time-tick ISR, (1-lowest ... 7-highest) */
#define TICK_ISR_PRIO       1

/*..........................................................................*/
#pragma INTERRUPT ta0_isr (vect = 21)              /* system clock tick ISR */
void ta0_isr(void) {

#ifdef Q_SPY
    if (ir_ta1ic != 0) {                          /* interrupt request set? */
        ++l_nTimerA1Underflows; /* account for 16-bit underflow (0->0xFFFF) */
        ir_ta1ic = 0;                            /* clear the overflow flag */
    }
#endif

    QF_INT_UNLOCK(dummy);                                     /* see NOTE01 */
    QF_tick();                             /* process all armed time events */
    QF_INT_LOCK(dummy);                                       /* see NOTE01 */
}
/*..........................................................................*/
void BSP_init(void) {
    uint16_t volatile delay;
                                                 /* initialize the clock... */
    prc1 = 1;                  /* enable access to processor mode registers */
    pm20 = 0;       /* 2 wait states for SFR access for >16MHz operation... */
    prc1 = 0;                 /* disable access to processor mode registers */

                      /* configure and switch main clock to PLL at 20MHz... */
    prc0 = 1;                           /* enable access to clock registers */
    cm1  = 0x20;                        /* set to hi-drive Xin, divide by 1 */
    cm0  = 0x08;                     /* set to main clock using divide by 1 */
    cm21 = 0;                                              /* switch to Xin */
    plc0 = 0x11;                                     /* configure PLL to x2 */
    plc0 = 0x91;                                              /* enable PLL */
    for (delay = 20000; delay != 0; --delay) {       /* wait for stable PLL */
    }
    cm11 = 1;                                              /* switch to PLL */
    prc0 = 0;                          /* disable access to clock registers */


                                            /* general device configuration */
    prcr = 0x04;                    /* enables next instruction to set pacr */
    pacr = 0x04;                                      /* set to 48 pin chip */

                                                  /* enable the User Button */
    SW1_DDR = 0;

                                               /* LED port configuration... */
    LED0_DDR = 1;
    LED1_DDR = 1;
    LED2_DDR = 1;
    LED3_DDR = 1;
    LED0     = LED_OFF;
    LED1     = LED_OFF;
    LED2     = LED_OFF;
    LED3     = LED_OFF;

                                /* configure 32kHz sub clock..., see NOTE02 */
    pd8_7 = 0;                           /* set GPIO to inputs (XCin/XCout) */
    pd8_6 = 0;
    prc0  = 1;                                        /* unlock CM0 and CM1 */
    cm04  = 1;                                   /* start the 32KHz crystal */
    prc0  = 0;                    /* lock the System Clock Control Register */

                                                       /* setup Timer A0... */
    ta0mr = 0xC0;                      /* Timer mode, fc32, no pulse output */
    ta0   = (int)((fc_CLK_SPEED/32 + BSP_TICKS_PER_SEC/2)
                  / BSP_TICKS_PER_SEC) - 1;                       /* period */
    ta0ic = TICK_ISR_PRIO;   /* set the clock tick interrupt priority level */

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
}
/*..........................................................................*/
void QF_onStartup(void) {
    ta0s = 1;                                             /* Start timer A0 */

#ifdef Q_SPY
    ta1s  = 1;                             /* start the QS time-stamp timer */
#endif
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QF_onIdle(void) {
#ifdef Q_SPY
    if (ti_u0c1 != 0) {
        uint16_t b = QS_getByte();
        QF_INT_UNLOCK(dummy);                          /* unlock interrupts */
        if (b != QS_EOD) {
            u0tb = b;                    /* stick the byte to the TX buffer */
        }
    }
    else {
        QF_INT_UNLOCK(dummy);                          /* unlock interrupts */
    }
#elif (defined NDEBUG)          /* low-power mode interferes with debugging */
    /* stop all peripheral clocks that you can in your applicaiton ... */
    _asm("FSET I");   /* the following WAIT instruction will execute before */
    _asm("WAIT");             /* entering any pending interrupt, see NOTE03 */
#else
    QF_INT_UNLOCK(dummy);                         /* just unlock interrupts */
#endif
}
/*..........................................................................*/
void BSP_displyPhilStat(uint8_t n, char const *stat) {
    uint8_t led_status = (stat[0] == 'e' ? LED_ON : LED_OFF);
    switch (n) {
        case 0: LED0 = led_status; break;
        case 1: LED1 = led_status; break;
        case 2: LED2 = led_status; break;
        case 3: LED3 = led_status; break;
    }
    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}
/*--------------------------------------------------------------------------*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;
    (void)line;
    QF_INT_LOCK(dummy);                              /* lock the interrupts */
    for (;;) {                                /* hang in this for-ever loop */
    }
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE];            /* buffer for Quantum Spy */
    uint16_t n;

    QS_initBuf(qsBuf, sizeof(qsBuf));

                                                     /* UART0 configuration */
    pd6_3 = 1;                                /* set UART0 TX pin as output */
    pd6_2 = 0;                                /* set UART0 RX pin as input  */

                                         /* set UART1 bit rate generator... */
    /* bit rate can be calculated by:
    *  bit rate = ((BRG_count_source / 16)/baud_rate) - 1
    * the value of BCLK does not affect BRG count source
    */
    u0brg = (uint8_t)(((f1_CLK_SPEED/16 + BAUD_RATE/2) / BAUD_RATE) - 1);

    /* 00000101 8 bit, internal clock, 1 stop, no parity, no inversion
    *  b2:b0    SMD2:0 Selects Uart mode, 8 bit data transfer
    *  b3       CKDIR  Internal clock selected
    *  b4       STPS   One stop bit
    *  b5       PRY    Odd parity (parity is disabled using b6)
    *  b6       PRYE   Disable parity
    *  b7              Reserved, set to 0
    */
    u0mr = 0x05;

    /* 00010000 MSB first, cmos output, CTS/RTS disabled, f1 clock source
    *   b1:b0   CLK1:0 Count source divided by 1
    *   b2             Reserved, set to 0
    *   b3      TXEPT  Transmit register empty flag
    *   b4             Reserved, set to 0
    *   b5      NCH    Data output select bit
    *   b6      CKPOL  Clock polarity selected,TX falling edge,RX rising edge
    *   b7      UFORM  MSB first
    */
    u0c0 = 0x10;

    u0tb = u0rb;                /* clear UART0 receive buffer by reading it */
    u0tb = 0;                                /* clear UART0 transmit buffer */

    /* 00000101 UART0 transmit/receive control register 1
    *  b0       TE     Transmit Enable Bit
    *  b1       TI     Transmit buffer empty flag,
    *  b2       RE     Receive enable bit,
    *  b3       RI     Receive complete flag,
    *  b4       U1IRS  Transmit buffer empty select,
    *  b5       U1RRM  Disable continous receive mode,
    *  b6:b7           Reserved, set to 0
    */
    u0c1 = 0x05;

    /* start the 16-bit Timer A, channel 1 to provide the time stamp for QS */
    ta1mr = 0x40;               /* Timer A1 in counter mode running from f8 */
    ta1   = 0xFFFF;                                 /* free-running counter */
    ta1ic = 0;               /* set interrutp priority (interrupt disabled) */

                                                 /* setup the QS filters... */
    QS_FILTER_ON(QS_ALL_RECORDS);

//    QS_FILTER_OFF(QS_QEP_STATE_EMPTY);
//    QS_FILTER_OFF(QS_QEP_STATE_ENTRY);
//    QS_FILTER_OFF(QS_QEP_STATE_EXIT);
//    QS_FILTER_OFF(QS_QEP_STATE_INIT);
//    QS_FILTER_OFF(QS_QEP_INIT_TRAN);
//    QS_FILTER_OFF(QS_QEP_INTERN_TRAN);
//    QS_FILTER_OFF(QS_QEP_TRAN);
//    QS_FILTER_OFF(QS_QEP_dummyD);

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
    QS_FILTER_OFF(QS_QF_INT_LOCK);
    QS_FILTER_OFF(QS_QF_INT_UNLOCK);
    QS_FILTER_OFF(QS_QF_ISR_ENTRY);
    QS_FILTER_OFF(QS_QF_ISR_EXIT);

    return (uint8_t)1;            /* indicate successfull QS initialization */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    while ((b = QS_getByte()) != QS_EOD) { /* next QS trace byte available? */
        while (ti_u0c1 == 0) {
        }
        u0tb = b;                        /* stick the byte to the TX buffer */
    }
}
/*..........................................................................*/
/* NOTE: invoked within a critical section (inetrrupts disabled) */
QSTimeCtr QS_onGetTime(void) {
    if (ir_ta1ic != 0) {                          /* interrupt request set? */
        ++l_nTimerA1Underflows; /* account for 16-bit underflow (0->0xFFFF) */
        ir_ta1ic = 0;                            /* clear the overflow flag */
    }
                   /* subtract the Timer A1 counter, because it dounts DOWN */
    return ((uint32_t)l_nTimerA1Underflows << 16) - (uint32_t)ta1;
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE01:
* The R8C microcontroller supports interrupt prioritization. Therefore it
* is safe to unlock interrupts inside ISRs. By assigning priorities to
* interrupts you have full control over interrupt nesting. In particular,
* you can avoid interrupt nesting by assigning the same priority level to
* all interrupts.
*
* The simple policy of unconditional unlocking of interrupts upon exit from
* a critical section precludes nesting of critical sections. This policy
* means that you *must* unlock interrupts inside every ISR before invoking
* any QF service, such as QActvie_postFIFO(), QF_publish(), or QF_tick().
* In fact, this QF port to M16C provides a pair of macros QF_ISR_ENTRY() and
* QF_ISR_EXIT() that you need to ivoke upon entry to every ISR and exit
* from the ISR, respectively. These macros, among others, unlock and lock
* the interrupts, respectively.
*
* NOTE02:
* The low-power 32KHz clock is used for the system clock tick, because this
* clock is *not* stopped in the low-power WAIT mode.
*
* NOTE03:
* The non-preemptive QF port (foreground/background) requires an atomic
* transiton to the low-power WAIT mode of the M16C MCU. As described in
* "M16C Software Manual", Section 5.2.1 "Interrupt Enable Flag", the
* instruction immediately following "FSET I" instruction, will be executed
* before any pending interrupt. This guarantees atomic transition to the
* WAIT mode. CAUTION: The instruction pair (FSET I, WAIT) should never be
* separated by any other instruction.
*/