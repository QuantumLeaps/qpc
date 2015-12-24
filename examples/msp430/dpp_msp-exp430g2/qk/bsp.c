/*****************************************************************************
* Product: DPP on MSP-EXP430G2 board, preemptive QK kernel
* Last updated for version 5.6.0
* Last updated on  2015-12-18
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* http://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

#include <msp430g2553.h>  /* MSP430 variant used */
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE

/* Local-scope objects -----------------------------------------------------*/
/* 8MHz clock setting, see BSP_init() */
#define BSP_MCK     8000000U
#define BSP_SMCLK   8000000U

/* LEDs on the MSP-EXP430G2 board */
#define LED1        (1U << 0)
#define LED2        (1U << 6)

/* Buttons on the MSP-EXP430G2 board */
#define BTN1        (1U << 3)

/* random seed */
static uint32_t l_rnd;

#ifdef Q_SPY

    #define TXD     (1U << 2)
    #define RXD     (1U << 1)

    QSTimeCtr QS_tickTime_;
    static uint8_t const l_timerA_ISR = 0U;

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER
    };

#endif

/* ISRs used in this project ===============================================*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
    __interrupt void TIMER0_A0_ISR(void); /* prototype */
    #pragma vector=TIMER0_A0_VECTOR
    __interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
    void __attribute__ ((interrupt(TIMER0_A0_VECTOR)))
    TIMER0_A0_ISR(void)
#else
    #error MSP430 compiler not supported!
#endif
{
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint8_t depressed;
        uint8_t previous;
    } buttons = { (uint8_t)~0U, (uint8_t)~0U };
    uint8_t current;
    uint8_t tmp;

    QK_ISR_ENTRY();    /* inform QK about entering the ISR */

    TACTL &= ~TAIFG;   /* clear the interrupt pending flag */

#ifdef Q_SPY
    QS_tickTime_ +=
       (((BSP_SMCLK / 8) + BSP_TICKS_PER_SEC/2) / BSP_TICKS_PER_SEC) + 1;
#endif

    QF_TICK_X(0U, (void *)0);  /* process all time events at rate 0 */

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = ~P1IN; /* read P1 port with the state of BTN1 */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & BTN1) != 0U) {     /* debounced BTN1 state changed? */
        if ((buttons.depressed & BTN1) != 0U) { /* is BTN1 depressed? */
            static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U};
            QF_PUBLISH(&pauseEvt, &l_timerA_ISR);
        }
        else {            /* the button is released */
            static QEvt const serveEvt = { SERVE_SIG, 0U, 0U};
            QF_PUBLISH(&serveEvt, &l_timerA_ISR);
        }
    }

    QK_ISR_EXIT();     /* inform QK about exiting the ISR */
}

/* BSP functions ===========================================================*/
void BSP_init(void) {
    WDTCTL = WDTPW | WDTHOLD; /* stop watchdog timer */

    /* configure the Basic Clock Module */
    DCOCTL = 0;             /* Select lowest DCOx and MODx settings */
    BCSCTL1 = CALBC1_8MHZ;  /* Set DCO */
    DCOCTL = CALDCO_8MHZ;

    /* configure pins for LEDs */
    P1DIR |= (LED1 | LED2); /* set LED1 and LED2 pins to output */

    /* configure pin for Button */
    P1DIR &= ~BTN1;   /* set BTN1 pin as input */
    P1OUT |= BTN1;    /* drive output to hi */
    P1REN |= BTN1;    /* enable internal pull up register */

    if (QS_INIT((void *)0) == 0) { /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_timerA_ISR);
    QS_USR_DICTIONARY(PHILO_STAT);
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    if (stat[0] == 'h') { /* is Philo hungry? */
        P1OUT |=  LED1;  /* turn LED1 on */
    }
    else {
        P1OUT &= ~LED1;  /* turn LED1 off */
    }

    QS_BEGIN(PHILO_STAT, AO_Philo[n]) /* application-specific record begin */
        QS_U8(1, n);                  /* Philosopher number */
        QS_STR(stat);                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    /* not enouhg LEDs to implement this feature */
    if (paused != 0U) {
        //P1OUT |=  LED1;
    }
    else {
        //P1OUT &= ~LED1;
    }
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    l_rnd = l_rnd * ((uint32_t)3U*7U*11U*13U*23U);

    return l_rnd >> 8;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}


/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    TACTL  = (ID_3 | TASSEL_2 | MC_1);  /* SMCLK, /8 divider, upmode */
    TACCR0 = (((BSP_SMCLK / 8U) + BSP_TICKS_PER_SEC/2U) / BSP_TICKS_PER_SEC);
    CCTL0 = CCIE;  /* CCR0 interrupt enabled */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QK_onIdle(void) {
    /* toggle LED2 on and then off, see NOTE1 */
    QF_INT_DISABLE();
    P1OUT |=  LED2;  /* turn LED2 on */
    P1OUT &= ~LED2;  /* turn LED2 off */
    QF_INT_ENABLE();

#ifdef Q_SPY
     if (((IFG2 & UCA0TXIFG)) != 0U) { /* UART not transmitting? */
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {
            UCA0TXBUF = (uint8_t)b; /* stick the byte to the TX BUF */
        }
    }
#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular MSP430 MCU.
    */
    __low_power_mode_1(); /* Enter LPM1; also ENABLES interrupts */
#endif
}
/*..........................................................................*/
void Q_onAssert(char const *module, int loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;
    QS_ASSERTION(module, loc, (uint32_t)10000U); /* report assertion to QS */

    /* cause the reset of the CPU... */
    QF_INT_DISABLE();
    WDTCTL = WDTPW | WDTHOLD;
    __asm("    push &0xFFFE");
    /* return from function does the reset */
}

/* QS callbacks ============================================================*/
#ifdef Q_SPY

uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[80]; /* buffer for QS; RAM is tight! */
    uint16_t tmp;

    QS_initBuf(qsBuf, sizeof(qsBuf));

    /* configure the UART pins... */
    P1DIR  |=  (RXD | TXD);   /* config RX and TX pins as outputs */
    P1OUT  &= ~(RXD | TXD);   /* drive RX and TX pins hi */
    P1SEL  |=  (RXD | TXD);   /* select the UART function... */
    P1SEL2 |=  (RXD | TXD);   /* ... for RXD and TXD */

    /* configure the hardware UART... */
    UCA0CTL1 |= UCSSEL_2;      /* select SMCLK for the UART */

    tmp = BSP_SMCLK / 9600U;   /* baud-rate value for 9600 bauds */
    UCA0BR0 = (uint8_t)tmp;    /* load the baud-rate register low */
    UCA0BR1 = (uint8_t)(tmp >> 8); /* load the baud-rate register hi */

    UCA0MCTL = UCBRS0;         /* modulation UCBRSx = 1 */
    UCA0CTL1 &= ~UCSWRST;      /* initialize USCI state machine */

    /* setup the QS filters... */
    QS_FILTER_ON(QS_QEP_STATE_ENTRY);
    QS_FILTER_ON(QS_QEP_STATE_EXIT);
    QS_FILTER_ON(QS_QEP_STATE_INIT);
    QS_FILTER_ON(QS_QEP_INIT_TRAN);
    QS_FILTER_ON(QS_QEP_INTERN_TRAN);
    QS_FILTER_ON(QS_QEP_TRAN);
    QS_FILTER_ON(QS_QEP_IGNORED);
    QS_FILTER_ON(QS_QEP_DISPATCH);
    QS_FILTER_ON(QS_QEP_UNHANDLED);

    QS_FILTER_ON(PHILO_STAT);

    return (uint8_t)1; /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {  /* invoked with interrupts DISABLED */
    if ((TACTL & TAIFG) == 0U) {  /* interrupt not pending? */
        return QS_tickTime_ + TAR;
    }
    else { /* the rollover occured, but the timerA_ISR did not run yet */
        return QS_tickTime_
           + (((BSP_SMCLK/8U) + BSP_TICKS_PER_SEC/2U)/BSP_TICKS_PER_SEC) + 1U
           + TAR;
    }
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    QF_INT_DISABLE();
    while ((b = QS_getByte()) != QS_EOD) { /* next QS byte available? */
        QF_INT_ENABLE();
        while ((IFG2 & UCA0TXIFG) == 0U) { /* TX not ready? */
        }
        UCA0TXBUF = (uint8_t)b; /* stick the byte to the TX BUF */
        QF_INT_DISABLE();
    }
    QF_INT_ENABLE();
}

#endif /* Q_SPY */

/*****************************************************************************
* NOTE1:
* One of the LEDs is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/

