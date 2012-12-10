/*****************************************************************************
* BSP for DPP example, Microstick II board, QK kernel
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Oct 15, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
#include "bsp.h"
#include "dpp.h"

#include "p33FJ128MC802.h"              /* header for dsPICFJ device in use */

Q_DEFINE_THIS_FILE

_FOSCSEL(FNOSC_FRC);              /* set flash configuration for the device */
_FOSC(FCKSM_CSDCMD & OSCIOFNC_ON & POSCMD_NONE);
_FWDT(FWDTEN_OFF);

/* Local-scope objects -----------------------------------------------------*/
static uint32_t l_rnd;                                       /* random seed */

                             /* frequency of the FRC oscillator for dsPICFJ */
#define FOSC_HZ                 7370000.0
                                       /* instruction cycle clock frequency */
#define FCY_HZ                  (FOSC_HZ / 2.0)
                 /* system clock tick period in CPU clocks / TMR2 prescaler */
#define BSP_TMR2_PERIOD         ((uint16_t)(FCY_HZ / BSP_TICKS_PER_SEC))

                                    /* controlling the LED of Microstick II */
#define LED_ON()                (LATA |= (1U << 0))
#define LED_OFF()               (LATA &= ~(1U << 0))
#define LED_TOGGLE()            (LATA ^= (1U << 0))

#ifdef Q_SPY
    static uint32_t l_tickTime;                        /* timestamp at tick */
    static uint8_t const l_T2Interrupt   = 0;
    static uint8_t const l_INT0Interrupt = 0;

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };
                 /* system clock tick period in CPU clocks / TMR2 prescaler */
    #define BSP_TMR2_PERIOD         ((uint16_t)(FCY_HZ / BSP_TICKS_PER_SEC))

#endif

/* ISRs --------------------------------------------------------------------*/
#define TIMER2_ISR_PRIO         4
#define INT0_ISR_PRIO           6

QK_ISR(no_auto_psv) _T2Interrupt() {
    _T2IF = 0;                              /* clear Timer 2 interrupt flag */

#ifdef Q_SPY
    l_tickTime += BSP_TMR2_PERIOD;             /* account for TMR2 overflow */
#endif

    QF_TICK(&l_T2Interrupt);          /* handle all armed time events in QF */

    QK_ISR_EXIT();                       /* inform QK about exiting the ISR */
}
/*..........................................................................*/
QK_ISR(auto_psv) _INT0Interrupt() {
    static QEvt const eat_evt = { EAT_SIG, 0U, 0U };

    _INT0IF = 0;

    QACTIVE_POST(AO_Table, &eat_evt, &l_INT0Interrupt);

    QK_ISR_EXIT();                       /* inform QK about exiting the ISR */
}

/*--------------------------------------------------------------------------*/
void BSP_init(void) {
    RCONbits.SWDTEN = 0;                                /* disable Watchdog */

    TRISA = 0x00;                                /* set LED pins as outputs */
    PORTA = 0x00;                               /* set LEDs drive state low */

    BSP_randomSeed(1234U);

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_RESET();
    QS_OBJ_DICTIONARY(&l_T2Interrupt);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const Q_ROM *stat) {
    (void)n;
    (void)stat;
    LED_TOGGLE();

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    (void)paused;
}
/*..........................................................................*/
uint32_t BSP_random(void) {  /* a very cheap pseudo-random-number generator */
    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    l_rnd = l_rnd * (3U*7U*11U*13U*23U);
    return l_rnd >> 8;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_DISABLE();             /* make sure that interrupts are disabled */
    for (;;) {
    }
}

/*..........................................................................*/
void QF_onStartup(void) {                 /* entered with interrupts locked */
    T2CON = 0x0000U;  /* Use Internal Osc (Fcy), 16 bit mode, prescaler = 1 */
    TMR2  = 0x0000U; /* Start counting from 0 and clear the prescaler count */
    PR2   = (uint16_t)(BSP_TMR2_PERIOD - 1U);              /* Timer2 period */
    _T2IP = TIMER2_ISR_PRIO;              /* set Timer 2 interrupt priority */
    _T2IF = 0;                           /* clear the interrupt for Timer 2 */
    _T2IE = 1;                              /* enable interrupt for Timer 2 */
    T2CONbits.TON = 1;                                     /* start Timer 2 */

    INTCON2bits.INT0EP = 0;              /* INT0 interrupt on positive edge */
    _INT0IP = INT0_ISR_PRIO;                 /* set INT0 interrupt priority */
    _INT0IF = 0;                            /* clear the interrupt for INT0 */
    _INT0IE = 1;                                   /* enable INT0 interrupt */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QK_onIdle(void) {

    /* NOTE: not enough LEDs on the Microstick II board to implement
    *  the idle loop activity indicator ...
    */
    //LED_ON ();                    /* blink the IDLE LED, see NOTE01 */
    //LED_OFF();

#ifdef Q_SPY
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
    Idle();                                      /* transition to Idle mode */
#endif
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY

#define QS_BUF_SIZE        1024
#define QS_BAUD_RATE       38400.0

uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE];            /* buffer for Quantum Spy */

    QS_initBuf(qsBuf, sizeof(qsBuf));     /* initialize the QS trace buffer */

                 /* initialize the UART2 for transmitting the QS trace data */
    RPOR5bits.RP11R = 5;                         /* Assign U2TX To Pin RP11 */
    U2STA  = 0x0000U;                        /* use default settings of 8N1 */
    U2MODE = 0x0008U;                              /* enable high baud rate */
    U2BRG  = (uint16_t)((FCY_HZ / (4.0 * QS_BAUD_RATE)) - 1.0 + 0.5);
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

