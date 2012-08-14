/*****************************************************************************
* BSP for Explorer 16 board with PIC24FJ128GA010, Vanilla kernel
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Feb 03, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
#include "bsp.h"
#include "dpp.h"

Q_DEFINE_THIS_FILE

                  /* prmiary oscillator frequency for the Explorer 16 board */
#define BSP_PRIMARY_OSC_HZ      8000000UL
                                                /* CPU frequency (FOSC / 2) */
#define BSP_FCY_HZ              (BSP_PRIMARY_OSC_HZ / 2)
                 /* system clock tick period in CPU clocks / TMR2 prescaler */
#define BSP_TMR2_PERIOD         (BSP_FCY_HZ / BSP_TICKS_PER_SEC)


static uint8_t const l_led[] = {
    (1 << 0),                            /* LED D3 on the Explorer 16 board */
    (1 << 1),                            /* LED D4 on the Explorer 16 board */
    (1 << 2),                            /* LED D5 on the Explorer 16 board */
    (1 << 3),                            /* LED D6 on the Explorer 16 board */
    (1 << 4),                            /* LED D7 on the Explorer 16 board */
    (1 << 5),                            /* LED D8 on the Explorer 16 board */
    (1 << 6),                            /* LED D9 on the Explorer 16 board */
    (1 << 7),                           /* LED D10 on the Explorer 16 board */
    0                                                   /* non-existent LED */
};

#define LED_ON(n_)              (PORTA |= l_led[n_])
#define LED_OFF(n_)             (PORTA &= ~l_led[n_])
#define IDLE_LED                7

#ifdef Q_SPY
    static uint32_t l_tickTime;                        /* timestamp at tick */
    static uint8_t const l_T2Interrupt = 0;

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };

#endif

/* ISRs --------------------------------------------------------------------*/
#define TIMER2_ISR_PRIO         4

void  __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void) {
    _T2IF = 0;                              /* clear Timer 2 interrupt flag */

#ifdef Q_SPY
    l_tickTime += BSP_TMR2_PERIOD;             /* account for TMR2 overflow */
#endif

    QF_TICK(&l_T2Interrupt);          /* handle all armed time events in QF */
}

/*--------------------------------------------------------------------------*/
void BSP_init(void) {
    RCONbits.SWDTEN = 0;                                /* disable Watchdog */

    TRISA = 0x00;                                /* set LED pins as outputs */
    PORTA = 0x00;                               /* set LEDs drive state low */

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_T2Interrupt);
}
/*..........................................................................*/
void QF_onStartup(void) {                 /* entered with interrupts locked */
    T2CON  = 0;       /* Use Internal Osc (Fcy), 16 bit mode, prescaler = 1 */
    TMR2   = 0;      /* Start counting from 0 and clear the prescaler count */
    PR2    = BSP_TMR2_PERIOD - 1;                   /* set the timer period */
    _T2IP  = TIMER2_ISR_PRIO;             /* set Timer 2 interrupt priority */
    _T2IF  = 0;                          /* clear the interrupt for Timer 2 */
    _T2IE  = 1;                             /* enable interrupt for Timer 2 */
    T2CONbits.TON = 1;                                     /* start Timer 2 */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QF_onIdle(void) {          /* entered with interrupts disabled, NOTE01 */

    LED_ON (IDLE_LED);                    /* blink the IDLE LED, see NOTE02 */
    LED_OFF(IDLE_LED);

#ifdef Q_SPY
    QF_INT_ENABLE();                       /* enable interrupts, see NOTE01 */

    while (U2STAbits.UTXBF == 0U) {                  /* TX Buffer not full? */
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b == QS_EOD) {                          /* End-Of-Data reached? */
            break;                                 /* break out of the loop */
        }
        U2TXREG = (uint8_t)b;   /* stick the byte to TXREG for transmission */
    }
#elif defined NDEBUG
    __asm__ volatile("disi #0x0001");
    Idle();                          /* transition to Idle mode, see NOTE03 */
#else
    QF_INT_ENABLE();                       /* enable interrupts, see NOTE01 */
#endif
}
/*..........................................................................*/
void BSP_displyPhilStat(uint8_t n, char const Q_ROM *stat) {
    if (stat[0] == (char const)'e') {        /* is this Philosopher eating? */
        LED_ON(n);
    }
    else {                                /* this Philosopher is not eating */
        LED_OFF(n);
    }
    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_DISABLE();             /* make sure that interrupts are disabled */
    for (;;) {
    }
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY

#define QS_BUF_SIZE        1024U
#define BAUD_RATE          38400U

uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE];            /* buffer for Quantum Spy */

    QS_initBuf(qsBuf, sizeof(qsBuf));     /* initialize the QS trace buffer */

                 /* initialize the UART2 for transmitting the QS trace data */
    TRISFbits.TRISF5 = 0;                     /* set UART2 TX pin as output */
    U2MODE = 0x0008;                               /* enable high baud rate */
    U2STA  = 0x0000;                         /* use default settings of 8N1 */
    U2BRG  = ((BSP_FCY_HZ / 4 / BAUD_RATE) - 1);     /* baud rate generator */
    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN   = 1;

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
    QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
    QS_FILTER_OFF(QS_QF_CRIT_EXIT);
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
        while (U2STAbits.UTXBF != 0U) {                  /* TX Buffer full? */
        }
        U2TXREG = (uint8_t)b;   /* stick the byte to TXREG for transmission */
    }
}
/*..........................................................................*/
/* NOTE: invoked within a critical section (inetrrupts disabled) */
QSTimeCtr QS_onGetTime(void) {
    if (_T2IF == 0) {
        return l_tickTime + (uint32_t)TMR2;
    }
    else {
        return l_tickTime + BSP_TMR2_PERIOD + (uint32_t)TMR2;
    }
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE01:
* The callback function QF_onIdle() is called with interrupts locked, because
* the idle condition can be invalidated by any enabled interrupt that would
* post events. The QF_onIdle() function *must* unlock interrupts internally
*
* NOTE02:
* The LED 7 (D10 on the Explorer 16 board) is used to visualize the idle loop
* activity. The brightness of the LED is proportional to the frequency of
* invcations of the idle loop. Please note that the LED is toggled with
* interrupts locked, so no interrupt execution time contributes to the
* brightness of the User LED.
*
* NOTE03:
* To be on the safe side, the DISICNT counter is set to just 1 cycle just
* before entering the Idle mode (or Sleep mode, if you choose). This way,
* interrupts (with priorities 1-6) get enabled at the same time as the
* transition to the low-power mode.
*/

