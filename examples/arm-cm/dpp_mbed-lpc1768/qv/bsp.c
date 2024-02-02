//============================================================================
// Product: DPP example, NXP mbed-LPC1768 board, QV kernel
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

#include "LPC17xx.h"  // CMSIS-compliant header file for the MCU used
// add other drivers if necessary...

Q_DEFINE_THIS_FILE  // define the name of this file for assertions

// Local-scope objects -----------------------------------------------------
#define LED_1    (1U << 18U)  // P1.18
#define LED_2    (1U << 20U)  // P1.20
#define LED_3    (1U << 21U)  // P1.21
#define LED_4    (1U << 23U)  // P1.23

// Push-Button wired externally to DIP8 (P0.6)
#define BTN_EXT  (1U << 6U)    // P0.6

static uint32_t l_rndSeed;

#ifdef Q_SPY
    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    // QSpy source IDs
    static QSpyId const l_SysTick_Handler = { 0U };
    static QSpyId const l_EINT0_IRQHandler = { 0U };

    #define UART_BAUD_RATE      115200U
    #define UART_FR_TXFE        0x80U
    #define UART_TXFIFO_DEPTH   16U

    enum AppRecords { // application-specific trace records
        PHILO_STAT = QS_USER,
        PAUSED_STAT,
        CONTEXT_SW,
    };

#endif

//============================================================================
// Error handler and ISRs...

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    // NOTE: this implementation of the error handler is intended only
    // for debugging and MUST be changed for deployment of the application
    // (assuming that you ship your production code with assertions enabled).
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);
    QS_ASSERTION(module, id, 10000U);

#ifndef NDEBUG
    // light up all LEDs
    LPC_GPIO1->FIOSET = LED_1;  // turn LED on
    LPC_GPIO1->FIOSET = LED_2;  // turn LED on
    LPC_GPIO1->FIOSET = LED_3;  // turn LED on
    LPC_GPIO1->FIOSET = LED_4;  // turn LED on
    // for debugging, hang on in an endless loop...
    for (;;) {
    }
#else
    NVIC_SystemReset();
    for (;;) { // explicitly "no-return"
    }
#endif
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}

// ISRs used in the application ==============================================

void SysTick_Handler(void); // prototype
void SysTick_Handler(void) {

    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler); // time events at rate 0

    // Perform the debouncing of buttons. The algorithm for debouncing
    // adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    // and Michael Barr, page 71.
    //
    // state of the button debouncing
    static struct {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };

    uint32_t current = ~LPC_GPIO0->FIOPIN; // read P0 with state of Buttons
    uint32_t tmp = buttons.depressed; // save the depressed buttons
    buttons.depressed |= (buttons.previous & current); // set depressed
    buttons.depressed &= (buttons.previous | current); // clear released
    buttons.previous   = current; // update the history
    tmp ^= buttons.depressed;     // changed debounced depressed
    current = buttons.depressed;

    if ((tmp & BTN_EXT) != 0U) {  // debounced BTN_EXT state changed?
        if ((current & BTN_EXT) != 0U) { // is BTN_EXT depressed?
            static QEvt const pauseEvt = QEVT_INITIALIZER(PAUSE_SIG);
            QACTIVE_PUBLISH(&pauseEvt, &l_SysTick_Handler);
        }
        else { // the button is released
            static QEvt const serveEvt = QEVT_INITIALIZER(SERVE_SIG);
            QACTIVE_PUBLISH(&serveEvt, &l_SysTick_Handler);
        }
    }

#ifdef Q_SPY
    tmp = SysTick->CTRL; // clear CTRL_COUNTFLAG
    QS_tickTime_ += QS_tickPeriod_; // account for the clock rollover
#endif // Q_SPY

    QV_ARM_ERRATUM_838869();
}
//............................................................................
// interrupt handler for testing preemptions in QK
void EINT0_IRQHandler(void); // prototype
void EINT0_IRQHandler(void) {
    // for testing...
    QACTIVE_POST(AO_Table, Q_NEW(QEvt, MAX_PUB_SIG), // for testing...
                 &l_EINT0_IRQHandler);

    QV_ARM_ERRATUM_838869();
}

//............................................................................
#ifdef QF_ON_CONTEXT_SW
// NOTE: the context-switch callback is called with interrupts DISABLED
void QF_onContextSw(QActive *prev, QActive *next) {
    Q_UNUSED_PAR(prev);
    Q_UNUSED_PAR(next);

    QS_BEGIN_INCRIT(CONTEXT_SW, 0U) // in critical section!
        QS_OBJ(prev);
        QS_OBJ(next);
    QS_END_INCRIT()
}
#endif // QF_ON_CONTEXT_SW

//============================================================================
// BSP functions...

void BSP_init(void) {
    // Configure the MPU to prevent NULL-pointer dereferencing ...
    MPU->RBAR = 0x0U                          // base address (NULL)
                | MPU_RBAR_VALID_Msk          // valid region
                | (MPU_RBAR_REGION_Msk & 7U); // region #7
    MPU->RASR = (7U << MPU_RASR_SIZE_Pos)     // 2^(7+1) region
                | (0x0U << MPU_RASR_AP_Pos)   // no-access region
                | MPU_RASR_ENABLE_Msk;        // region enable
    MPU->CTRL = MPU_CTRL_PRIVDEFENA_Msk       // enable background region
                | MPU_CTRL_ENABLE_Msk;        // enable the MPU
    __ISB();
    __DSB();

    // enable the MemManage_Handler for MPU exception
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

    // NOTE: SystemInit() has been already called from the startup code
    // but SystemCoreClock needs to be updated
    SystemCoreClockUpdate();

    // turn the GPIO clock on
    LPC_SC->PCONP |= (1U << 15);

    // setup the GPIO pin functions for the LEDs...
    LPC_PINCON->PINSEL3 &= ~(3U <<  4); // LED_1: function P1.18 to GPIO
    LPC_PINCON->PINSEL3 &= ~(3U <<  8); // LED_2: function P1.20 to GPIO
    LPC_PINCON->PINSEL3 &= ~(3U << 10); // LED_3: function P1.21 to GPIO
    LPC_PINCON->PINSEL3 &= ~(3U << 14); // LED_4: function P1.23 to GPIO

    // Set GPIO-P1 LED pins to output
    LPC_GPIO1->FIODIR |= (LED_1 | LED_2 | LED_3 | LED_4);


    // setup the GPIO pin function for the Button...
    LPC_PINCON->PINSEL0 &= ~(3U << 12); // function P0.6 to GPIO, pull-up

    // Set GPIO-P0 Button pin as input
    LPC_GPIO0->FIODIR &= ~BTN_EXT;

    BSP_randomSeed(1234U); // seed the random number generator

    // initialize the QS software tracing...
    if (!QS_INIT((void *)0)) {
        Q_ERROR();
    }

    // dictionaries...
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_OBJ_DICTIONARY(&l_EINT0_IRQHandler);
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
            n + 3U,                  // QF-prio.
            philoQueueSto[n],        // event queue storage
            Q_DIM(philoQueueSto[n]), // queue length [events]
            (void *)0, 0U,           // no stack storage
            (void *)0);              // no initialization param
    }

    static QEvt const *tableQueueSto[N_PHILO];
    Table_ctor();
    QACTIVE_START(AO_Table,
        N_PHILO + 7U,                // QP prio. of the AO
        tableQueueSto,               // event queue storage
        Q_DIM(tableQueueSto),        // queue length [events]
        (void *)0, 0U,               // no stack storage
        (void *)0);                  // no initialization param
}
//............................................................................
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    Q_UNUSED_PAR(n);

    if (stat[0] == 'h') {
        LPC_GPIO1->FIOSET = LED_1;  // turn LED on
    }
    else {
        LPC_GPIO1->FIOCLR = LED_1;  // turn LED off
    }
    if (stat[0] == 'e') {
        LPC_GPIO1->FIOSET = LED_2;  // turn LED on
    }
    else {
        LPC_GPIO1->FIOCLR = LED_2;  // turn LED off
    }

    // app-specific trace record...
    QS_BEGIN_ID(PHILO_STAT, AO_Table->prio)
        QS_U8(1, n);  // Philosopher number
        QS_STR(stat); // Philosopher status
    QS_END()
}
//............................................................................
void BSP_displayPaused(uint8_t const paused) {
    if (paused != 0U) {
        LPC_GPIO1->FIOSET = LED_3;  // turn LED on
    }
    else {
        LPC_GPIO1->FIOCLR = LED_3;  // turn LED off
    }

    // application-specific trace record
    QS_BEGIN_ID(PAUSED_STAT, AO_Table->prio)
        QS_U8(1, paused);  // Paused status
    QS_END()
}
//............................................................................
void BSP_randomSeed(uint32_t seed) {
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
//............................................................................
void BSP_ledOn(void) {
    LPC_GPIO1->FIOSET = LED_3;  // turn LED on
}
//............................................................................
void BSP_ledOff(void) {
    LPC_GPIO1->FIOCLR = LED_3;  // turn LED off
}

//============================================================================
// QF callbacks...
void QF_onStartup(void) {
    // set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    // assign all priority bits for preemption-prio. and none to sub-prio.
    NVIC_SetPriorityGrouping(0U);

    // set priorities of ALL ISRs used in the system, see NOTE1
    NVIC_SetPriority(EINT0_IRQn,     QF_AWARE_ISR_CMSIS_PRI + 0U);
    NVIC_SetPriority(SysTick_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 1U);
    // ...

    // enable IRQs in the NVIC...
    NVIC_EnableIRQ(EINT0_IRQn);
}
//............................................................................
void QF_onCleanup(void) {
}
//............................................................................
void QV_onIdle(void) { // CATION: called with interrupts DISABLED, see NOTE0
    // toggle the User LED on and then off, see NOTE2
    LPC_GPIO1->FIOSET = LED_4;  // turn LED on
    __NOP();   // a couple of NOPs to actually see the LED glow
    __NOP();
    __NOP();
    __NOP();
    LPC_GPIO1->FIOCLR = LED_4;  // turn LED off

#ifdef Q_SPY
    // interrupts still disabled
    QS_rxParse();  // parse all the received bytes
    QF_INT_ENABLE();

    if ((LPC_UART0->LSR & 0x20U) != 0U) {  // TX Holding Register empty?
        uint16_t fifo = UART_TXFIFO_DEPTH; // max bytes we can accept

        QF_INT_DISABLE();
        uint8_t const *block = QS_getBlock(&fifo);
        QF_INT_ENABLE();

        while (fifo-- != 0U) { // any bytes in the block?
            LPC_UART0->THR = *block++; // put into the FIFO
        }
    }
#elif defined NDEBUG
    // Put the CPU and peripherals to the low-power mode.
    // you might need to customize the clock management for your application,
    // see the datasheet for your particular Cortex-M MCU.
    //
    QV_CPU_SLEEP(); // atomically go to sleep and enable interrupts
#else
    QF_INT_ENABLE(); // just enable interrupts
#endif
}

//============================================================================
// QS callbacks...
#ifdef Q_SPY

static void UART0_setBaudrate(uint32_t baud);  // helper function

//............................................................................
uint8_t QS_onStartup(void const *arg) {
    Q_UNUSED_PAR(arg);

    static uint8_t qsTxBuf[2*1024]; // buffer for QS-TX channel
    QS_initBuf(qsTxBuf, sizeof(qsTxBuf));

    // TBD: add support for QS-RX channel

    // setup the P0_2 UART0 TX pin
    LPC_PINCON->PINSEL0  &= ~(3U << 4); // clear P0_2 function
    LPC_PINCON->PINSEL0  |=  (1U << 4); // P0_2 to UART function (TX)
    LPC_PINCON->PINMODE0 &= ~(3U << 4); // P0_2 pull-up register

    // setup the P0_3 UART0 RX pin
    LPC_PINCON->PINSEL0  &= ~(3U << 6); // clear P0_3 function
    LPC_PINCON->PINSEL0  |=  (1U << 6); // P0_3 to UART function (RX)
    LPC_PINCON->PINMODE0 &= ~(3U << 6); // P0_3 pull-up register

    // enable power to UART0
    LPC_SC->PCONP |= (1U << 3);

    // enable FIFOs and default RX trigger level
    LPC_UART0->FCR =
        (1U << 0)    // FIFO Enable - 0 = Disables, 1 = Enabled
        | (0U << 1)  // Rx Fifo Reset
        | (0U << 2)  // Tx Fifo Reset
        | (0U << 6); // Rx irq trig: 0=1char, 1=4chars, 2=8chars, 3=14chars

    // disable IRQs
    LPC_UART0->IER =
        (0U << 0)    // Rx Data available IRQ disable
        | (0U << 1)  // Tx Fifo empty IRQ disable
        | (0U << 2); // Rx Line Status IRQ disable


    // set default baud rate
    UART0_setBaudrate(115200U);

    // format 8-data-bits, 1-stop-bit, parity-none
    LPC_UART0->LCR =
        (3U << 0)    // 8-data-bits
        | (0U << 2)  // 1 stop-bit
        | (0U << 3)  // parity disable
        | (0U << 4); // parity none


    QS_tickPeriod_ = SystemCoreClock / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_; // to start the timestamp at zero

    return 1U; // return success
}
//............................................................................
void QS_onCleanup(void) {
}
//............................................................................
QSTimeCtr QS_onGetTime(void) {  // NOTE: invoked with interrupts DISABLED
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) { // not set?
        return QS_tickTime_ - (QSTimeCtr)SysTick->VAL;
    }
    else { // the rollover occurred, but the SysTick_ISR did not run yet
        return QS_tickTime_ + QS_tickPeriod_ - (QSTimeCtr)SysTick->VAL;
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
            while ((LPC_UART0->LSR & 0x20U) == 0U) { // while THR empty...
            }
            LPC_UART0->THR = b; // put into the THR register
        }
        else {
            break;
        }
    }
}
//............................................................................
void QS_onReset(void) {
    NVIC_SystemReset();
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

//............................................................................
//
// Set the LPC UART0 barud-rate generator according to
// Section 14.4.12 in LPC176x Manual (document UM10360)
//
static void UART0_setBaudrate(uint32_t baud) {

    // First we check to see if the basic divide with no DivAddVal/MulVal
    // ratio gives us an integer result. If it does, we set DivAddVal = 0,
    // MulVal = 1. Otherwise, we search the valid ratio value range to find
    // the closest match. This could be more elegant, using search methods
    // and/or lookup tables, but the brute force method is not that much
    // slower, and is more maintainable.
    //
    uint32_t PCLK     = SystemCoreClock; // divider /1 set below
    uint16_t DL       = PCLK / (16U * baud);
    uint8_t DivAddVal = 0U;
    uint8_t MulVal    = 1U;

    // set PCLK divider to 1
    LPC_SC->PCLKSEL0 &= ~(0x3U << 6); // clear divider bits
    LPC_SC->PCLKSEL0 |=  (0x1U << 6); // set divider to 1

    if ((PCLK % (16U * baud)) != 0U) { // non zero remainder?
        uint32_t err_best = baud;
        bool found = false;
        uint32_t b;
        uint8_t mv;
        for (mv = 1U; mv < 16U && !found; mv++) {
            uint16_t dlv;
            uint8_t dav;
            for (dav = 0U; dav < mv; ++dav) {
                //
                // baud = PCLK / (16 * dlv * (1 + (DivAdd / Mul))
                // solving for dlv, we get
                // dlv = mul * PCLK / (16 * baud * (divadd + mul))
                // mul has 4 bits, PCLK has 27 so we have 1 bit headroom,
                // which can be used for rounding for many values of mul
                // and PCLK we have 2 or more bits of headroom which can
                // be used to improve precision
                // note: X / 32 doesn't round correctly.
                // Instead, we use ((X / 16) + 1) / 2 for correct rounding
                //
                if ((mv*PCLK*2U) & 0x80000000U) { // 1 bit headroom
                    dlv = ((((2U*mv*PCLK) / (baud*(dav + mv)))/16U) + 1U)/2U;
                }
                else { // 2 bits headroom, use more precision
                    dlv = ((((4U*mv*PCLK) / (baud*(dav+mv)))/32U) + 1U)/2U;
                }

                // datasheet says if DLL==DLM==0, then 1 is used instead
                if (dlv == 0U) {
                    dlv = 1U;
                }
                // datasheet says if dav > 0 then DL must be >= 2
                if ((dav > 0U) && (dlv < 2U)) {
                    dlv = 2U;
                }
                // integer rearrangement of baud equation (with rounding)
                b = ((PCLK*mv / (dlv*(dav + mv)*8U)) + 1U)/2U;
                b = (b >= baud) ? (b - baud) : (baud - b);

                // check to see how we did
                if (b < err_best) {
                    err_best  = b;
                    DL        = dlv;
                    MulVal    = mv;
                    DivAddVal = dav;

                    if (b == baud) {
                        found = true;
                        break;   // break out of the inner for-loop
                    }
                }
            }
        }
    }

    // set LCR[DLAB] to enable writing to divider registers
    LPC_UART0->LCR |= (1U << 7);

    // set divider values
    LPC_UART0->DLM = (DL >> 8) & 0xFFU;
    LPC_UART0->DLL = (DL >> 0) & 0xFFU;
    LPC_UART0->FDR = ((uint32_t)DivAddVal << 0)
                   | ((uint32_t)MulVal    << 4);

    // clear LCR[DLAB]
    LPC_UART0->LCR &= ~(1U << 7);
}

#endif // Q_SPY
//----------------------------------------------------------------------------

//============================================================================
// NOTE0:
// The QV_onIdle() callback is called with interrupts disabled, because the
// determination of the idle condition might change by any interrupt posting
// an event. QV_onIdle() must internally enable interrupts, ideally
// atomically with putting the CPU to the power-saving mode.
//
// NOTE1:
// The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
// ISR priority that is disabled by the QF framework. The value is suitable
// for the NVIC_SetPriority() CMSIS function.
//
// Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
// with the numerical values of priorities equal or higher than
// QF_AWARE_ISR_CMSIS_PRI) are allowed to call any other QF/QV services.
// These ISRs are "QF-aware".
//
// Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
// level (i.e., with the numerical values of priorities less than
// QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
// Such "QF-unaware" ISRs cannot call ANY QF/QV services. In particular they
// can NOT call the macros QV_ISR_ENTRY/QV_ISR_ENTRY. The only mechanism
// by which a "QF-unaware" ISR can communicate with the QF framework is by
// triggering a "QF-aware" ISR, which can post/publish events.
//
// NOTE2:
// The User LED is used to visualize the idle loop activity. The brightness
// of the LED is proportional to the frequency of the idle loop.
// Please note that the LED is toggled with interrupts locked, so no interrupt
// execution time contributes to the brightness of the User LED.
//
