/*****************************************************************************
* BSP for Blinky example, Microstick II board, QK kernel
* Last updated for version 6.5.1
* Last updated on  2019-06-10
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
#include "blinky.h"

#include "p24FJ64GB002.h"  /* header for PIC24F device in use */

Q_DEFINE_THIS_FILE

#ifdef Q_SPY
    #error Simple Blinky Application does not provide Spy build configuration
#endif

/* configure Fast RC Oscillator (FRC) */
#pragma config FNOSC=FRC

/* Local-scope objects -----------------------------------------------------*/


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

/* ISRs --------------------------------------------------------------------*/
QK_ISR(no_auto_psv) _T2Interrupt() {
    _T2IF = 0; /* clear Timer 2 interrupt flag */

    QF_TICK_X(0U, (void *)0); /* handle armed time events at tick rate 0 */

    QK_ISR_EXIT(); /* inform QK about exiting the ISR */
}
/*..........................................................................*/
QK_ISR(auto_psv) _INT0Interrupt() {
    static QEvt const tout_evt = { TIMEOUT_SIG, 0U, 0U };

    _INT0IF = 0;

    QACTIVE_POST(AO_Blinky, &tout_evt, (void *)0);

    QK_ISR_EXIT(); /* inform QK about exiting the ISR */
}

/*--------------------------------------------------------------------------*/
void BSP_init(void) {
    RCONbits.SWDTEN = 0; /* disable Watchdog */

    TRISA = 0x00; /* set LED pins as outputs */
    PORTA = 0x00; /* set LED drive state low */
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}
/*..........................................................................*/
void BSP_ledOff(void) {
    LED_OFF();
}
/*..........................................................................*/
void BSP_ledOn(void) {
    LED_ON();
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

#ifdef NDEBUG
    //Idle(); /* transition to Idle mode */
#endif
}

