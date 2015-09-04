/*****************************************************************************
* Product: DPP example, Arduino-UNO board, preemptive QK kernel, IAR-AVR
* Last Updated for Version: 5.5.0
* Date of the Last Update:  2015-08-28
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

#include <iom328p.h>   /* the AVR MCU variant used */
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE

/* Local-scope objects -----------------------------------------------------*/
/* Arduino-UNO runs off the 16MHz oscillator */
#define F_CPU   16000000UL

/* the on-board LED labeled "L" on Arduino-UNO (PORTB) */
#define LED_L   (1U << 5)

/* external button BTN_EXT (PORTD), http://arduino.cc/en/tutorial/button */
#define BTN_EXT (1U << 2)

static unsigned  l_rnd;  /* random seed */

#ifdef Q_SPY

    QSTimeCtr BSP_tickTime;
    static uint8_t l_ISR_TIMER2_COMPA;

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER
    };

#endif

/* ISRs used in this project ===============================================*/
#pragma vector = TIMER2_COMPA_vect
__interrupt void timer2_ISR(void) {
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint8_t depressed;
        uint8_t previous;
    } buttons = { 0xFFU, 0xFFU };
    uint8_t current;
    uint8_t tmp;

    QK_ISR_ENTRY();  /* inform QK about entering an ISR */

    QF_TICK_X(0U, &l_ISR_TIMER2_COMPA); /* process time events at rate 0 */

#ifdef Q_SPY
    BSP_tickTime += (F_CPU / BSP_TICKS_PER_SEC / 1024U);
#endif

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = PIND; /* read PORTD with the state of BTN_EXT */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & BTN_EXT) != 0U) {  /* debounced BTN_EXT state changed? */
        if ((buttons.depressed & BTN_EXT) != 0U) { /* is BTN_EXT depressed? */
            static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U};
            QF_PUBLISH(&pauseEvt, &l_ISR_TIMER2_COMPA);
        }
        else {            /* the button is released */
            static QEvt const serveEvt = { SERVE_SIG, 0U, 0U};
            QF_PUBLISH(&serveEvt, &l_ISR_TIMER2_COMPA);
        }
    }

    QK_ISR_EXIT();  /* inform QK about exiting an ISR */
}

/* BSP functions ===========================================================*/
void BSP_init(void) {
    /* setup the port for the LED (PORTB.5) */
    DDRB   =  0xFFU;    /* set all pins on PORTB as output */
    PORTB &= ~LED_L;    /* drive LED_L pin low */

    /* setup the port for the Button (PORTD.2) */
    DDRD  &= ~BTN_EXT;  /* set PORTD pin BTN_EXT as input */

    if (QS_INIT((void *)0) == 0) { /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_ISR_TIMER2_COMPA);
    QS_USR_DICTIONARY(PHILO_STAT);
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    if (stat[0] == 'h') {
        PORTB |= LED_L;
    }
    else {
        PORTB &= ~LED_L;
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
        //PORTB |= LED_L;
    }
    else {
        //PORTB &= ~LED_L;
    }
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
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
void BSP_terminate(int16_t result) {
    (void)result;
}


/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    /* set Timer2 in CTC mode, 1/1024 prescaler, start the timer ticking... */
    TCCR2A = (1U << WGM21) | (0U << WGM20);
    TCCR2B = (1U << CS22 ) | (1U << CS21) | (1U << CS20); /* 1/2^10 */
    ASSR  &= ~(1U << AS2);
    TIMSK2 = (1U << OCIE2A); /* Enable TIMER2 compare Interrupt */
    TCNT2  = 0U;
    OCR2A  = (F_CPU / BSP_TICKS_PER_SEC / 1024U) - 1U;
}
/*..........................................................................*/
void QF_onCleanup(void) {
}

/*..........................................................................*/
void QK_onIdle(void) {
    /* toggle the User LED, see NOTE1 , not enough LEDs to implement! */
    //QF_INT_DISABLE();
    //PORTB |= LED_L;
    //PORTB &= ~LED_L;
    //QF_INT_ENABLE();

#ifdef Q_SPY
    if ((UCSR0A & (1U << UDRE0)) != 0U) {
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {
            UDR0 = (uint8_t)b; /* stick the byte to the TX UDR0 */
        }
    }
#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular AVR MCU.
    */
    SMCR = (0 << SM0) | (1 << SE); /* idle mode, adjust to your project */
    QK_CPU_SLEEP(); /* put CPU to sleep */
#endif
}

/*..........................................................................*/
void Q_onAssert(char const Q_ROM *module, int loc) {
    QF_INT_DISABLE(); /* disable all interrupts */
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;
    QS_ASSERTION(module, loc, (uint32_t)2000U); /* report assertion to QS */
    QF_RESET();  /* reset the CPU */
}


/* QS callbacks ============================================================*/
#ifdef Q_SPY

uint8_t QS_onStartup(void const *arg) {
    /* buffer for QS software tracing. Easy here! Total RAM is only 2K */
    static uint8_t qsBuf[512];
    uint16_t tmp;

    /* initialize QS software tracing */
    QS_initBuf(qsBuf, sizeof(qsBuf));

    /* make sure that USART0 is powered up */
    PRR &= ~(1U << PRUSART0);

    /* set baud rate... */
    UCSR0A = (1U << U2X0);  /* use double-speed mode */
    tmp =  (F_CPU / 4U / 115200U - 1U) / 2U;
    UBRR0H = (uint8_t)(tmp >> 8);
    UBRR0L = (uint8_t)tmp;

    /* configure the UART: enable transmitter, no interrupts */
    UCSR0B = (0U << RXEN0) | (1U << TXEN0);

    /* configure the UART: Async. mode, 8N1, no clock polarity */
    UCSR0C = (0U << UMSEL00) | (0U << UPM00) | (0U << USBS0)
             | (3U << UCSZ00) | (0U << UCPOL0);

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
QSTimeCtr QS_onGetTime(void) {  /* NOTE: invoked with interrupts DISABLED */
    if ((TIFR2 & (1U << OCF2A)) == 0U) { /* output compare flag NOT set? */
        return BSP_tickTime + (QSTimeCtr)TCNT2;
    }
    else { /* the output compare occured, but the ISR did not run yet */
        return BSP_tickTime + (F_CPU / BSP_TICKS_PER_SEC / 1024U)
               + (QSTimeCtr)TCNT2;
    }
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    QF_INT_DISABLE();
    while ((b = QS_getByte()) != QS_EOD) { /* next QS trace byte available? */
        QF_INT_ENABLE();
        while ((UCSR0A & (1U << UDRE0)) == 0U) {  /* TX UDR not empty? */
        }
        UDR0 = (uint8_t)b; /* stick the byte to the TX UDR */
    }
    QF_INT_ENABLE();
}

#endif /* Q_SPY */

/*****************************************************************************
* NOTE1:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/

