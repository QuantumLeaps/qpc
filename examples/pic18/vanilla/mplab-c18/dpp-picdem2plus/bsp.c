/*****************************************************************************
* BSP for PICDEM 2 PLUS with PIC18F452
* Last Updated for Version: 4.4.01
* Date of the Last Update:  Mar 23, 2012
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

/* Local objects -----------------------------------------------------------*/
uint8_t const l_led[] = {
    (1U << 0),                            /* LED RB0 on PICDEM 2 PLUS board */
    (1U << 1),                            /* LED RB1 on PICDEM 2 PLUS board */
    (1U << 2),                            /* LED RB2 on PICDEM 2 PLUS board */
    (1U << 3),                            /* LED RB3 on PICDEM 2 PLUS board */
    0U                                                  /* non-existent LED */
};

#define LED_ON(n_)        (LATB |= l_led[n_])
#define LED_OFF(n_)       (LATB &= ~l_led[n_])

#ifdef Q_SPY
    QSTimeCtr QS_tickTime;
    uint8_t const Q_ROM QS_ISR_hi = 0U;
#endif

/*..........................................................................*/
void BSP_init(void) {
    TRISB = 0U;                 /* data direction for Port B (LEDs): output */
    LATB  = 0U;                                      /* extinguish all LEDs */

    RCONbits.IPEN = 1;                            /* enable priority levels */

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
}
/*..........................................................................*/
void QF_onStartup(void) {
    TMR1H = 0;
    TMR1L = 1;                              /* make TMR1 exprie immediately */
    T1CON = 0x8F;      /* ext Timer1 clock, 16-bit, no synch, prescaler 1:1 */
    IPR1bits.TMR1IP = 1;             /* set TMR1 interrupt as high priority */
    PIE1bits.TMR1IE = 1;                           /* enable TMR1 interrupt */

#ifdef Q_SPY
    /* initialize Timer0 to deliver the timestamp */
    TMR0H = 0x0;                                              /* clear TMR0 */
    TMR0L = 0x0;                                              /* clear TMR0 */
    T0CON = 0x88;                  /* set up TMR0: 16-bit, bypass prescaler */
    INTCONbits.TMR0IE = 1;                         /* enable TMR0 interrupt */
#endif
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QF_onIdle(void) {      /* entered with interrupts disabled, see NOTE01 */

#ifdef Q_SPY
    if (PIR1bits.TXIF != 0U) {                              /* TXREG empty? */
        uint16_t b = QS_getByte();
        if (b != QS_EOD) {                 /* next QS trace byte available? */
            TXREG = (uint8_t)b; /* stick the byte to TXREG for transmission */
        }
    }
#elif defined NDEBUG
    Sleep();                        /* transition to SLEEP mode, see NOTE02 */
#endif

    QF_INT_ENABLE();                                   /* enable interrupts */
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
        QS_STR_ROM(stat);                             /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_DISABLE();                  /* make sure interrupts are disabled */
    for (;;) {
    }
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY

#define QS_BUF_SIZE        200U
#define BAUD_RATE          19200U

QSTimeCtr BSP_tickTime;

uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE];            /* buffer for Quantum Spy */
    uint16_t n;

    QS_initBuf(qsBuf, sizeof(qsBuf));     /* initialize the QS trace buffer */

    /* initialize the USART for transmitting the QS trace data */
    SPBRG = BSP_FOSC_HZ/16/BAUD_RATE - 1;   /* set up for desired baud rate */
    RCSTAbits.SPEN = 1;                               /* enable serial port */
    TXSTAbits.SYNC = 0;                                /* asynchronous mode */
    TXSTAbits.BRGH = 1;                                     /* high TX rate */
    TXSTAbits.TXEN = 1;                                        /* enable TX */

                                                     /* setup QS filters... */
    QS_FILTER_ON(QS_ALL_RECORDS);
    QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
    QS_FILTER_OFF(QS_QF_CRIT_EXIT);
    QS_FILTER_OFF(QS_QF_ISR_ENTRY);
    QS_FILTER_OFF(QS_QF_ISR_EXIT);
    QS_FILTER_OFF(QS_QF_TICK);
    QS_FILTER_OFF(QS_QK_SCHEDULE);

    return (uint8_t)1;            /* indicate successfull QS initialization */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    while ((b = QS_getByte()) != QS_EOD) { /* next QS trace byte available? */
        while (PIR1bits.TXIF == 0U) {                   /* TXREG not empty? */
        }
        TXREG = (uint8_t)b;     /* stick the byte to TXREG for transmission */
    }
}
/*..........................................................................*/
/* NOTE: invoked within a critical section (inetrrupts disabled) */
QSTimeCtr QS_onGetTime(void) {
    uint8_t tlow = TMR0L;    /* read TMR0L and latch TMR0H at the same time */
    if (INTCONbits.TMR0IF == 0U) {          /* TMR0 overflow accounted for? */
        return BSP_tickTime
               + (QSTimeCtr)(((uint16_t)TMR0H << 8) | (uint16_t)tlow);
    }
    else {              /* the TMR0 overflowed, but the ISR did not run yet */
        return BSP_tickTime + (QSTimeCtr)0x10000
               + (QSTimeCtr)(((uint16_t)TMR0H << 8) | (uint16_t)tlow);
    }
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE01:
* The callback function QF_onIdle() is called with interrupts disabled,
* because the idle condition can be invalidated by any enabled interrupt that
* would post events. The QF_onIdle() function *must* enable interrupts
* internally
*
* NOTE02:
* The low-power mode must be entered with interrupts disabled to avoid
* an interrupt to preempt the transition to the low-power mode. According to
* the "PIC18FXX2 Data Sheet", the Sleep mode can be exited even if the global
* inetrrupt enable flag is cleared (INTCON<7> == 0). This allows for
* an atomic transition to the SLEEP mode.
*
* Selected interrupts, such as Timer1 interrupt with external clock (not
* synchronized to the CPU clock) can wake up the CPU from the SLEEP mode.
* after waking up, the CPU does not service the interrupt immediately,
* because interrupts are still disabled. The interrupt gets serviced only
* after the CPU unlocks interrupts.
*/

