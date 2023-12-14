//============================================================================
// Product: DPP on MSP-EXP430F5529LP, QV kernel
// Last updated for version 7.3.2
// Last updated on  2023-12-13
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. <state-machine.com>
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <www.gnu.org/licenses/>.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"                 // QP/C real-time embedded framework
#include "dpp.h"                 // DPP Application interface
#include "bsp.h"                 // Board Support Package

#include <msp430f5529.h>  // MSP430 variant used
// add other drivers if necessary...

Q_DEFINE_THIS_FILE

static uint32_t l_rndSeed;

#ifdef Q_SPY
    // UART1 pins TX:P4.4, RX:P4.5
    #define TXD     (1U << 4U)
    #define RXD     (1U << 5U)

    QSTimeCtr QS_tickTime_;
    static uint8_t const l_timerA_ISR = 0U;

    enum AppRecords { // application-specific trace records
        PHILO_STAT = QS_USER,
        CONTEXT_SW,
        PAUSED_STAT,
    };

#endif

// Local-scope objects -----------------------------------------------------
// 1MHz clock setting, see BSP_init()
#define BSP_MCK     1000000U
#define BSP_SMCLK   1000000U

#define LED1        (1U << 0U)
#define LED2        (1U << 7U)

#define BTN_S1      (1U << 1U)


//============================================================================
// Error handler and ISRs...

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    // NOTE: this implementation of the error handler is intended only
    // for debugging and MUST be changed for deployment of the application
    // (assuming that you ship your production code with assertions enabled).
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);
    QS_ASSERTION(module, id, 10000U); // report assertion to QS

#ifndef NDEBUG
    P4OUT |=  LED2;  // turn LED2 on
    // for debugging, hang on in an endless loop...
    for (;;) {
    }
#else
    // write invalid password to WDT: cause a password-validation RESET
    WDTCTL = 0xDEAD;
    for (;;) {} // explicit no-return
#endif
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}

// ISRs used in this project ===============================================
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
    __interrupt void TIMER0_A0_ISR(void); // prototype
    #pragma vector=TIMER0_A0_VECTOR
    __interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
    __attribute__ ((interrupt(TIMER0_A0_VECTOR)))
    void TIMER0_A0_ISR(void)
#else
    #error MSP430 compiler not supported!
#endif
{
#ifdef NDEBUG
    __low_power_mode_off_on_exit(); // see NOTE1
#endif

#ifdef Q_SPY
    QS_tickTime_ +=
       (((BSP_SMCLK / 8) + BSP_TICKS_PER_SEC/2) / BSP_TICKS_PER_SEC) + 1;
#endif

    QTIMEEVT_TICK_X(0U, (void *)0);  // process all time events at rate 0
}


// BSP functions ===========================================================
void BSP_init(void) {
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    // leave the MCK and SMCLK at default DCO setting

    // configure pins for LEDs
    P1DIR |= LED1;  // set LED1 pin to output
    P4DIR |= LED2;  // set LED2 pin to output

    // initialize the QS software tracing...
    if (!QS_INIT((void *)0)) {
        Q_ERROR();
    }

    // dictionaries...
    QS_OBJ_DICTIONARY(&l_timerA_ISR);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(PAUSED_STAT);
    QS_USR_DICTIONARY(CONTEXT_SW);

    QS_ONLY(produce_sig_dict());

    // setup the QS filters...
    QS_GLB_FILTER(QS_ALL_RECORDS);   // all records
    QS_GLB_FILTER(-QS_QF_TICK);      // exclude the clock tick
}
//............................................................................
void BSP_start(void) {
    // initialize event pools
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO];
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // initialize publish-subscribe
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // instantiate and start AOs/threads...

    static QEvt const *philoQueueSto[N_PHILO][10];
    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        Philo_ctor(n);
        QACTIVE_START(AO_Philo[n],
            n + 1U,                  // QF-prio/pthre. see NOTE1
            philoQueueSto[n],        // event queue storage
            Q_DIM(philoQueueSto[n]), // queue length [events]
            (void *)0, 0U,           // no stack storage
            (void *)0);              // no initialization param
    }

    static QEvt const *tableQueueSto[N_PHILO];
    Table_ctor();
    QACTIVE_START(AO_Table,
        N_PHILO + 1U,                // QP prio. of the AO
        tableQueueSto,               // event queue storage
        Q_DIM(tableQueueSto),        // queue length [events]
        (void *)0, 0U,               // no stack storage
        (void *)0);                  // no initialization param
}
//............................................................................
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    Q_UNUSED_PAR(n);

    if (stat[0] == 'h') { // is Philo hungry?
        P1OUT |=  LED1;  // turn LED1 on
    }
    else {
        P1OUT &= ~LED1;  // turn LED1 off
    }

    // app-specific trace record...
    QS_BEGIN_ID(PHILO_STAT, AO_Table->prio)
        QS_U8(1, n);  // Philosopher number
        QS_STR(stat); // Philosopher status
    QS_END()
}
//............................................................................
void BSP_displayPaused(uint8_t const paused) {
    // not enouhg LEDs to implement this feature
    if (paused != 0U) {
        //P1OUT |=  LED1;
    }
    else {
        //P1OUT &= ~LED1;
    }
}
//............................................................................
void BSP_randomSeed(uint32_t const seed) {
    l_rndSeed = seed;
}
//............................................................................
uint32_t BSP_random(void) { // a very cheap pseudo-random-number generator
    // "Super-Duper" Linear Congruential Generator (LCG)
    // LCG(2^32, 3*7*11*13*23, 0, seed)
    //
    uint32_t rnd = l_rndSeed * (3U*7U*11U*13U*23U);
    l_rndSeed = rnd; // set for the next time

    return (rnd >> 8U);
}
//............................................................................
void BSP_terminate(int16_t result) {
    Q_UNUSED_PAR(result);
}

//============================================================================
// QF callbacks...
void QF_onStartup(void) {
    TA0CCTL0 = CCIE;                          // CCR0 interrupt enabled
    TA0CCR0 = BSP_MCK / BSP_TICKS_PER_SEC;
    TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, upmode, clear TAR
}
//............................................................................
void QF_onCleanup(void) {
}
//............................................................................
void QV_onIdle(void) { // NOTE: called with interrutps DISABLED, see NOTE1
    // toggle LED2 on and then off, see NOTE2
    P4OUT |=  LED2;  // turn LED2 on
    P4OUT &= ~LED2;  // turn LED2 off

#ifdef Q_SPY
    QS_rxParse();  // parse all the received bytes
    QF_INT_ENABLE();
    QF_CRIT_EXIT_NOP();

    if ((UCA1STAT & UCBUSY) == 0U) { // TX done?

        QF_INT_DISABLE();
        uint16_t b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {
            UCA1TXBUF = (uint8_t)b; // stick the byte to the TX BUF
        }
    }
#elif defined NDEBUG
    // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular MSP430 MCU.
    //
    __low_power_mode_1(); // enter LPM1; also ENABLES interrupts
#else
    QF_INT_ENABLE(); // just enable interrupts
#endif
}

//============================================================================
// QS callbacks...
#ifdef Q_SPY

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
    __interrupt void USCI_A1_ISR(void); // prototype
    #pragma vector=USCI_A1_VECTOR
    __interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
    __attribute__ ((interrupt(USCI_A1_VECTOR)))
    void USCI_A1_ISR(void)
#else
    #error MSP430 compiler not supported!
#endif
{
    if (UCA1IV == 2) {
        uint16_t b = UCA1RXBUF;
        QS_RX_PUT(b);
    }
}
//............................................................................
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[256];  // buffer for QS; RAM is tight!
    static uint8_t qsRxBuf[80]; // buffer for QS receive channel
    //uint16_t tmp;

    QS_initBuf(qsBuf, sizeof(qsBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    // USCI setup code...
    P4SEL |= (RXD | TXD);  // select the UART function for the pins
    UCA1CTL1 |= UCSWRST;   // reset USCI state machine
    UCA1CTL1 |= UCSSEL_2;  // choose the SMCLK clock
#if 1
    UCA1BR0 = 6; // 1MHz 9600 (see User's Guide)
    UCA1BR1 = 0; // 1MHz 9600
    // modulation UCBRSx=0, UCBRFx=0, oversampling
    UCA1MCTL = UCBRS_0 | UCBRF_13 | UCOS16;
#else
    UCA1BR0 = 9;           // 1MHz 115200 (see User's Guide)
    UCA1BR1 = 0;           // 1MHz 115200
    UCA1MCTL |= UCBRS_1 | UCBRF_0; // modulation UCBRSx=1, UCBRFx=0
#endif
    UCA1CTL1 &= ~UCSWRST;  // initialize USCI state machine
    UCA1IE |= UCRXIE;      // Enable USCI_A1 RX interrupt

    return 1U; // return success
}
//............................................................................
void QS_onCleanup(void) {
}
//............................................................................
QSTimeCtr QS_onGetTime(void) {  // invoked with interrupts DISABLED
    if ((TA0CTL & TAIFG) == 0U) {  // interrupt not pending?
        return QS_tickTime_ + TA0R;
    }
    else { // the rollover occured, but the timerA_ISR did not run yet
        return QS_tickTime_
           + (((BSP_SMCLK/8U) + BSP_TICKS_PER_SEC/2U)/BSP_TICKS_PER_SEC) + 1U
           + TA0R;
    }
}
//............................................................................
// NOTE:
// No critical section in QS_onFlush() to avoid nesting of critical sections
// in case QS_onFlush() is called from Q_onError().
void QS_onFlush(void) {
    for (;;) {
        uint16_t b = QS_getByte();
        if (b != QS_EOD) {
            while ((UCA1STAT & UCBUSY) != 0U) { // TX busy?
            }
            UCA1TXBUF = (uint8_t)b; // stick the byte to the TX BUF
        }
        else {
            break;
        }
    }
}
//............................................................................
void QS_onReset(void) {
    // write invalid password to WDT: cause a password-validation RESET
    WDTCTL = 0xDEAD;
}
//............................................................................
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    Q_UNUSED_PAR(cmdId);
    Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);
}

#endif // Q_SPY

//============================================================================
// NOTE1:
// With the cooperative QV kernel for MSP430, it is necessary to explicitly
// turn the low-power mode OFF in the interrupt, because the return
// from the interrupt will restore the CPU status register, which will
// re-enter the low-power mode. This, in turn, will prevent the QV event-loop
// from running.
//
// NOTE2:
// One of the LEDs is used to visualize the idle loop activity. The brightness
// of the LED is proportional to the frequency of invocations of the idle loop.
// Please note that the LED is toggled with interrupts disabled, so no
// interrupt execution time contributes to the brightness of the User LED.
//

