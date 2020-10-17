/*****************************************************************************
* BSP for DPP example, Microstick II board, QK kernel
* Last updated for version 6.9.1
* Last updated on  2020-09-22
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"
#include "bsp.h"
#include "dpp.h"

#include "p24FJ64GB002.h"  /* header for PIC24F device in use */

Q_DEFINE_THIS_FILE

/* configure Fast RC Oscillator (FRC) */
#pragma config FNOSC=FRC

/* Local-scope objects -----------------------------------------------------*/
static uint32_t l_rnd;  /* random seed */

/* frequency of the FRC oscillator ~ 8 MHz */
#define FOSC_HZ                 8000000.0
/* instruction cycle clock frequency */
#define FCY_HZ                  (FOSC_HZ / 2.0)
/* system clock tick period in CPU clocks / TMR2 prescaler */
#define BSP_TMR2_PERIOD         ((uint16_t)(FCY_HZ / BSP_TICKS_PER_SEC))

/* controlling the LED of Microstick II */
#define LED_ON()                (LATA |= (1U << 0))
#define LED_OFF()               (LATA &= ~(1U << 0))
#define LED_TOGGLE()            (LATA ^= (1U << 0))

#ifdef Q_SPY
    static uint32_t l_tickTime; /* timestamp at tick */
    static uint8_t const l_T2Interrupt   = 0;
    static uint8_t const l_INT0Interrupt = 0;

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER
    };

#endif

/* ISRs --------------------------------------------------------------------*/
QK_ISR(no_auto_psv) _T2Interrupt() {
    _T2IF = 0; /* clear Timer 2 interrupt flag */

#ifdef Q_SPY
    l_tickTime += BSP_TMR2_PERIOD; /* account for TMR2 overflow */
#endif

    QF_TICK_X(0U, &l_T2Interrupt); /* handle time events at tick rate 0 */

    QK_ISR_EXIT(); /* inform QK about exiting the ISR */
}
/*..........................................................................*/
QK_ISR(auto_psv) _INT0Interrupt() {
    static QEvt const eat_evt = { EAT_SIG, 0U, 0U };

    _INT0IF = 0;

    QACTIVE_POST(AO_Table, &eat_evt, &l_INT0Interrupt);

    QK_ISR_EXIT(); /* inform QK about exiting the ISR */
}

/*--------------------------------------------------------------------------*/
void BSP_init(void) {
    RCONbits.SWDTEN = 0; /* disable Watchdog */

    TRISA = 0x00; /* set LED pins as outputs */
    PORTA = 0x00; /* set LED drive state low */

    BSP_randomSeed(1234U);

    if (QS_INIT((void *)0) == 0) { /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_T2Interrupt);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_SM_RECORDS); /* state machine records */
    QS_GLB_FILTER(QS_AO_RECORDS); /* active object records */
    QS_GLB_FILTER(QS_UA_RECORDS); /* all user records */
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    (void)n;
    (void)stat;
    LED_TOGGLE();

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
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
Q_NORETURN Q_onAssert(char_t const * const file, int_t const loc) {
    (void)file;       /* unused parameter */
    (void)loc;        /* unused parameter */
    QF_INT_DISABLE(); /* make sure that interrupts are disabled */
    for (;;) {
    }
}

/*..........................................................................*/
void QF_onStartup(void) { /* entered with interrupts locked */
    T2CON = 0x0000U; /* Use Internal Osc (Fcy), 16 bit mode, prescaler = 1 */
    TMR2  = 0x0000U; /* Start counting from 0 and clear the prescaler count */
    PR2   = (uint16_t)(BSP_TMR2_PERIOD - 1U); /* Timer2 period */
    _T2IF = 0;               /* clear the interrupt for Timer 2 */
    _T2IE = 1;               /* enable interrupt for Timer 2 */
    T2CONbits.TON = 1;       /* start Timer 2 */

    INTCON2bits.INT0EP = 0;  /* INT0 interrupt on positive edge */
    _INT0IF = 0;             /* clear the interrupt for INT0 */
    _INT0IE = 1;             /* enable INT0 interrupt */

    /* explicitly assign priorities to all interrutps... */
    _T2IP   = 4; /* Timer 2 interrupt priority (kernel aware) */
    _INT0IP = 6; /* INT0 interrupt priority (kernel aware) */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QK_onIdle(void) {

    /* NOTE: not enough LEDs on the Microstick II board to implement
    *  the idle loop activity indicator ...
    */
    //LED_ON (); /* blink the IDLE LED, see NOTE01 */
    //LED_OFF();

#ifdef Q_SPY
    while (U2STAbits.UTXBF == 0U) { /* TX Buffer not full? */
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b == QS_EOD) { /* End-Of-Data reached? */
            break;         /* break out of the loop */
        }
        U2TXREG = (uint8_t)b;   /* stick the byte to TXREG for transmission */
    }
#elif defined NDEBUG
    //Idle(); /* transition to Idle mode */
#endif
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY

#define QS_BUF_SIZE        1024
#define QS_BAUD_RATE       38400.0

uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE]; /* buffer for Quantum Spy */

    QS_initBuf(qsBuf, sizeof(qsBuf));  /* initialize the QS trace buffer */

    /* initialize the UART2 for transmitting the QS trace data */
    RPOR5bits.RP11R = 5;   /* Assign U2TX To Pin RP11 */
    U2STA  = 0x0000U;      /* use default settings of 8N1 */
    U2MODE = 0x0008U;      /* enable high baud rate */
    U2BRG  = (uint16_t)((FCY_HZ / (4.0 * QS_BAUD_RATE)) - 1.0 + 0.5);
    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN   = 1;

    return 1U; /* indicate successful QS initialization */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    while ((b = QS_getByte()) != QS_EOD) { /* next QS trace byte available? */
        while (U2STAbits.UTXBF != 0U) { /* TX Buffer full? */
        }
        U2TXREG = (uint8_t)b; /* stick the byte to TXREG for transmission */
    }
}
/*..........................................................................*/
/* NOTE: invoked within a critical section (interrupts disabled) */
QSTimeCtr QS_onGetTime(void) {
    if (_T2IF == 0) {
        return l_tickTime + (uint32_t)TMR2;
    }
    else {
        return l_tickTime + BSP_TMR2_PERIOD + (uint32_t)TMR2;
    }
}
/*..........................................................................*/
/*! callback function to reset the target (to be implemented in the BSP) */
void QS_onReset(void) {
    __asm__ volatile ("RESET");
}
/*..........................................................................*/
/*! callback function to execute a user command (to be implemented in BSP) */
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif /* Q_SPY */
/*--------------------------------------------------------------------------*/

