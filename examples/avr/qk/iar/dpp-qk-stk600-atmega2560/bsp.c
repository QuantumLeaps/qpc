/*****************************************************************************
* Product: Board Support Package for STK600-ATMEGA2560, QK kernel
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jul 04, 2012
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
#include "dpp.h"
#include "bsp.h"

#define F_CPU                8000000UL
#include <iom2560.h>                                    /* AVR variant used */

Q_DEFINE_THIS_FILE

#define LED_ON(num_)       (PORTD &= ~(1 << (num_)))
#define LED_OFF(num_)      (PORTD |= (1 << (num_)))
#define LED_ON_ALL()       (PORTD = 0x00)
#define LED_OFF_ALL()      (PORTD = 0xFF)

/* Local objects -----------------------------------------------------------*/
static unsigned  l_rnd;                                      /* random seed */

#ifdef Q_SPY
    #define QS_BUF_SIZE        (256)
    #define BAUD_RATE          38400

    static QTimeEvtCtr l_tickTime;
    static uint8_t const l_timer2_ISR = 0;

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };
#endif


/* ISRs --------------------------------------------------------------------*/
#pragma vector = TIMER2_COMPA_vect
__interrupt void timer2_ISR(void) {
    /* No need to clear the interrupt source since the Timer0 compare
    * interrupt is automatically cleared in hardware when the ISR runs.
    */
    QK_ISR_ENTRY();           /* inform the QK kernel about entering an ISR */

#ifdef Q_SPY
    l_tickTime += (F_CPU / BSP_TICKS_PER_SEC / 1024);
#endif

    QF_TICK(&l_timer2_ISR);

    QK_ISR_EXIT();             /* inform the QK kernel about exiting an ISR */
}

/*..........................................................................*/
void BSP_init(void) {
    DDRD  = 0xFF;                    /* All PORTD pins are outputs for LEDs */
    LED_OFF_ALL();                                     /* turn off all LEDs */

    BSP_randomSeed(1234U);

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }

    QS_OBJ_DICTIONARY(&l_timer2_ISR);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char_t const *stat) {
    if (stat[0] == (uint8_t)'e') {           /* is this Philosopher eating? */
        LED_ON(n);
    }
    else {                                /* this Philosopher is not eating */
        LED_OFF(n);
    }

//    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
//        QS_U8(1, n);                                  /* Philosopher number */
//        QS_STR(stat);                                 /* Philosopher status */
//    QS_END()
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
    l_rnd = l_rnd * (3*7*11*13*23);
    return l_rnd >> 8;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    QF_INT_DISABLE();
    LED_ON_ALL();                                            /* all LEDs on */
    for (;;) {                            /* hang here in the for-ever loop */
    }
}

/*..........................................................................*/
void QF_onStartup(void) {
       /* set Timer2 in CTC mode, 1/1024 prescaler, start the timer ticking */
    TCCR2A = ((1 << WGM21) | (0 << WGM20));
    TCCR2B = ((1 << CS22) | (1 << CS21) | (1 << CS20));           /* 1/2^10 */
    ASSR  &= ~(1 << AS2);
    TIMSK2 = (1 << OCIE2A);            /* enable TIMER2 compare A interrupt */
    TCNT2 = 0;
    OCR2A = ((F_CPU / BSP_TICKS_PER_SEC / 1024) - 1);          /* keep last */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QK_onIdle() {
                     /* toggle the LED number 7 on and then off, see NOTE01 */
    QF_INT_DISABLE();
    LED_ON(7);
    LED_OFF(7);
    QF_INT_ENABLE();

#ifdef Q_SPY                     /* use the idle cycles for QS transmission */

    if ((UCSR0A & (1 << UDRE0)) != 0) {
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {
            UDR0 = (uint8_t)b;              /* stick the byte to the TX UDR */
        }
    }

#elif defined NDEBUG

    SMCR = (0 << SM0) | (1 << SE);/*idle sleep mode, adjust to your project */
    __sleep();                                               /* go to sleep */
    SMCR = 0;                                           /* clear the SE bit */

#endif
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE];            /* buffer for Quantum Spy */
    uint16_t n;

    QS_initBuf(qsBuf, sizeof(qsBuf));

    /* The following PORT setting is only valid to USART0.
    * For other USARTs, you might need to use different PORT and/or pins.
    */
    DDRE &= ~(1 << 0);                             /* PIN0 (RXD0) as input  */
    DDRE |= (1 << 1);                              /* PIN1 (TXD0) as output */

    n = F_CPU / 16 / BAUD_RATE - 1;                        /* set baud rate */
    UBRR0H = (uint8_t)(n >> 8);
    UBRR0L = (uint8_t)n;

    /* enable transmitter in polled mode, no interrupts */
    UCSR0B = (0 << RXEN0) | (1 << TXEN0) | (0 << RXCIE0) | (0 << UDRIE0);

    /* Async. mode, 8N1 */
    UCSR0C = (0 << UMSEL00) | (0 << UPM00) | (0 << USBS0)
              | (3 << UCSZ00) | (0 << UCPOL0);

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
        while ((UCSR0A & (1 << UDRE0)) == 0) {         /* TX UDR not empty? */
        }
        UDR0 = (uint8_t)b;                  /* stick the byte to the TX UDR */
    }
}
/*..........................................................................*/
/* NOTE: invoked within a critical section (inetrrupts disabled) */
QSTimeCtr QS_onGetTime(void) {
    if ((TIFR0 & (1 << OCF0A)) == 0) {      /* output compare flag NOT set? */
        return l_tickTime + (QSTimeCtr)TCNT0;
    }
    else {       /* the output compare occured, but the ISR did not run yet */
        return l_tickTime + (F_CPU / BSP_TICKS_PER_SEC / 1024)
               + (QSTimeCtr)TCNT0;
    }
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE01:
* The LED[7] is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/

