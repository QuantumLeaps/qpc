/*****************************************************************************
* Product: DPP example, RSK-R8C25 board, with QK
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Aug 20, 2008
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

                                        /* number of underflows of Timer RD */
    static uint16_t l_nTimerRDunderflows = 1;
#endif


/*..........................................................................*/
#pragma interrupt timer_re_isr (vect = 10)
void timer_re_isr(void){

#ifdef Q_SPY
    if (ovf_trdsr0 != 0) {                            /* overflow flag set? */
        ++l_nTimerRDunderflows; /* account for 16-bit underflow (0->0xFFFF) */
        ovf_trdsr0 = 0;                          /* clear the overflow flag */
    }
#endif

    QK_ISR_ENTRY();                     /* inform QK about entering the ISR */
    QF_tick();                             /* process all armed time events */
    QK_ISR_EXIT();                       /* inform QK about exiting the ISR */
}
/*..........................................................................*/
void BSP_init(void) {
                                             /* initialize the CPU clock... */
    prcr = 1;                                                /* protect off */
    cm13 = 1;                                                   /* Xin Xout */
    cm15 = 1;                /* XCIN-XCOUT drive capacity select bit : HIGH */
    cm05 = 0;                                                     /* Xin on */
    cm16 = 0;                              /* main clock = No division mode */
    cm17 = 0;                              /* main clock = No division mode */
    cm06 = 0;                                       /* CM16 and CM17 enable */
    asm("nop");                        /* Wait for stable of oscillation... */
    asm("nop");
    asm("nop");
    asm("nop");
    ocd2 = 0;                                          /* main clock change */
    prcr = 0;                                                 /* protect on */

                                    /* configure 32kHz clock..., see NOTE01 */
    prc0  = 1;                                               /* protect off */
    pd4_3 = 0;    /* unlock CM0 and CM1 and set GPIO to inputs (XCin/XCout) */
    pd4_4 = 0;                                                       /* ... */
    cm04  = 1;                                   /* Start the 32KHz crystal */
    prc0  = 0;                                                /* protect on */

                                      /* start Timer RE running from fc4... */
    trecr1 = 0x00;                                         /* stop Timer RE */
    trecr2 = 0x00;       /* output compare mode, interrupt disabled for now */
    trecsr = 0x63;    /* fc4 clock source, no 4-bit counter, output compare */
                                                /* set output compare value */
    tremin = (uint8_t)((fc_CLK_SPEED/4/2 + BSP_TICKS_PER_SEC/2)
                       / BSP_TICKS_PER_SEC - 1);
                                                    /* enable the User LEDs */
    LED0_DDR = 1;
    LED1_DDR = 1;
    LED2_DDR = 1;
    LED3_DDR = 1;
    LED0 = LED_OFF;
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
}
/*..........................................................................*/
void QF_onStartup(void) {
    comie_trecr2  = 1;          /* enable Timer RE output compare interrupt */
    tstart_trecr1 = 1;                                    /* start Timer RE */
    treic         = 3;                      /* TRE interrupt priority level */

#ifdef Q_SPY
    tstart0_trdstr = 1;                    /* start the QS time-stamp timer */
#endif
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QK_init(void) {
}
/*..........................................................................*/
void QK_onIdle(void) {
#ifdef Q_SPY
    if (ti_u1c1 != 0) {
        uint16_t b;
        QF_INT_LOCK(dummy);                              /* lock interrupts */
        b = QS_getByte();
        QF_INT_UNLOCK(dummy);                          /* unlock interrupts */
        if (b != QS_EOD) {
            u1tb = b;                    /* stick the byte to the TX buffer */
        }
    }
    else {
        QF_INT_UNLOCK(dummy);                          /* unlock interrupts */
    }
#elif (defined NDEBUG)          /* low-power mode interferes with debugging */
    /* stop all peripheral clocks that you can in your applicaiton ... */
    _asm("WAIT");             /* entering any pending interrupt, see NOTE02 */
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

    u1sr = 0x0F;                                   /* UART1 select register */
    u1pinsel = 1;        /* UART1 Pin select register: 1 : CLK1, TXD1, RXD1 */
    pd6_6 = 1;                                /* set UART1 TX pin as output */
    pd6_7 = 0;                                /* set UART1 RX pin as input  */

                                         /* set UART1 bit rate generator... */
    /* bit rate can be calculated by:
    *  bit rate = ((BRG_count_source / 16)/baud_rate) - 1
    * the value of BCLK does not affect BRG count source
    */
    u1brg = (uint8_t)(((f1_CLK_SPEED/16 + BAUD_RATE/2) / BAUD_RATE) - 1);

    /* 00000101 8 bit, internal clock, 1 stop, no parity, no inversion
    *  b2:b0    SMD2:0 Selects Uart mode, 8 bit data transfer
    *  b3       CKDIR  Internal clock selected
    *  b4       STPS   One stop bit
    *  b5       PRY    Odd parity (parity is disabled using b6)
    *  b6       PRYE   Disable parity
    *  b7              Reserved, set to 0
    */
    u1mr = 0x05;

    /* 00010000 MSB first, cmos output, CTS/RTS disabled, f1 clock source
    *   b1:b0   CLK1:0 Count source divided by 1
    *   b2             Reserved, set to 0
    *   b3      TXEPT  Transmit register empty flag
    *   b4             Reserved, set to 0
    *   b5      NCH    Data output select bit
    *   b6      CKPOL  Clock polarity selected,TX falling edge,RX rising edge
    *   b7      UFORM  MSB first
    */
    u1c0 = 0x00;

    u1tb = u1rb;                /* clear UART1 receive buffer by reading it */
    u1tb = 0;                                /* clear UART1 transmit buffer */

    /* 00000101 UART1 transmit/receive control register 1
    *  b0       TE     Transmit Enable Bit
    *  b1       TI     Transmit buffer empty flag,
    *  b2       RE     Receive enable bit,
    *  b3       RI     Receive complete flag,
    *  b4       U1IRS  Transmit buffer empty select,
    *  b5       U1RRM  Disable continous receive mode,
    *  b6:b7           Reserved, set to 0
    */
    u1c1 = 0x05;

    /* start the 16-bit Timer D, channel 1 to provide the time stamp for QS */
    trdcr0 = 0x03;   /* Timer RD, channel 0 as free running counter from f8 */

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
        while (ti_u1c1 == 0) {
        }
        u1tb = b;                        /* stick the byte to the TX buffer */
    }
}
/*..........................................................................*/
/* NOTE: invoked within a critical section (inetrrupts disabled) */
QSTimeCtr QS_onGetTime(void) {
    if (ovf_trdsr0 != 0) {                            /* overflow flag set? */
        ++l_nTimerRDunderflows; /* account for 16-bit underflow (0->0xFFFF) */
        ovf_trdsr0 = 0;                          /* clear the overflow flag */
    }
    return ((QSTimeCtr)l_nTimerRDunderflows << 16) + (QSTimeCtr)trd0;
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE01:
* The low-power 32KHz clock is used for the system clock tick, because this
* clock is *not* stopped in the low-power WAIT mode.
*/