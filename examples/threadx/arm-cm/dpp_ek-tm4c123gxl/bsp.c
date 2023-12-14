//============================================================================
// DPP example, EK-TM4C123GXL board, ThreadX RTOS
// Last updated for version 7.3.2
// Last updated on  2023-12-13
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. <state-machine.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"                 // QP/C real-time embedded framework
#include "dpp.h"                 // DPP Application interface
#include "bsp.h"                 // Board Support Package

#include "TM4C123GH6PM.h"        // the device specific header (TI)
#include "rom.h"                 // the built-in ROM functions (TI)
#include "sysctl.h"              // system control driver (TI)
#include "gpio.h"                // GPIO driver (TI)
// add other drivers if necessary...

Q_DEFINE_THIS_FILE  // define the name of this file for assertions

// Local-scope defines -----------------------------------------------------
#define LED_RED     (1U << 1U)
#define LED_GREEN   (1U << 3U)
#define LED_BLUE    (1U << 2U)

#define BTN_SW1     (1U << 4U)
#define BTN_SW2     (1U << 0U)

static uint32_t l_rndSeed;
static TX_TIMER l_tick_timer; // ThreadX timer to call QTIMEEVT_TICK_X()

#ifdef Q_SPY

    // QSpy source IDs
    static QSpyId const l_clock_tick = { QS_AP_ID };

    // ThreadX "idle" thread for QS output, see NOTE1
    static TX_THREAD idle_thread;
    static ULONG idle_thread_stack[100];

    #define UART_BAUD_RATE      115200U
    #define UART_FR_TXFE        (1U << 7U)
    #define UART_FR_RXFE        (1U << 4U)
    #define UART_TXFIFO_DEPTH   16U

    enum AppRecords { // application-specific trace records
        PHILO_STAT = QS_USER,
        PAUSED_STAT,
    };

#endif

static TX_TIMER l_tick_timer; // ThreadX timer to call QTIMEEVT_TICK_X()
static VOID timer_expiration(ULONG id);
static VOID timer_expiration(ULONG id) {
    QTIMEEVT_TICK_X(id, &l_clock_tick); // QF clock tick processing
}

//============================================================================
// Error handler

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    // NOTE: this implementation of the error handler is intended only
    // for debugging and MUST be changed for deployment of the application
    // (assuming that you ship your production code with assertions enabled).
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);
    QS_ASSERTION(module, id, 10000U); // report assertion to QS

#ifndef NDEBUG
    // light up all LEDs
    GPIOF_AHB->DATA_Bits[LED_GREEN | LED_RED | LED_BLUE] = 0xFFU;
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

// ISRs and ThreadX IRS callbacks used in the application ====================

//............................................................................
#ifdef Q_SPY

//............................................................................
static void idle_thread_fun(ULONG thread_input); // prototype
static void idle_thread_fun(ULONG thread_input) { // see NOTE1
    for (;;) {
        QF_CRIT_STAT

        QF_CRIT_ENTRY();
        QS_rxParse();  // parse all the received bytes
        QF_CRIT_EXIT();

        if ((UART0->FR & UART_FR_TXFE) != 0U) { // TX done?
            uint16_t fifo = UART_TXFIFO_DEPTH; // max bytes we can accept

            QF_CRIT_ENTRY();
            uint8_t const *block = QS_getBlock(&fifo);
            QF_CRIT_EXIT();

            while (fifo-- != 0U) {    // any bytes in the block?
                UART0->DR = *block++; // put into the FIFO
            }
        }

        // no blocking in this "idle" thread; see NOTE1
    }
}

//............................................................................
// ISR for receiving bytes from the QSPY Back-End
// NOTE: This ISR is "kernel-unaware" meaning that it does not interact with
// the  or QP and is not disabled. Such ISRs don't cannot call any
// ThreadX or QP APIs.
void UART0_IRQHandler(void); // prototype
void UART0_IRQHandler(void) {
    uint32_t status = UART0->RIS; // get the raw interrupt status
    UART0->ICR = status;          // clear the asserted interrupts

    while ((UART0->FR & UART_FR_RXFE) == 0U) { // while RX FIFO NOT empty
        uint32_t b = UART0->DR;
        QS_RX_PUT(b);
    }
}

#endif // Q_SPY

// BSP functions ===========================================================
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

    // NOTE: VFP (hardware Floating Point) unit is configured by ThreadX

    // enable clock for to the peripherals used by this application...
    SYSCTL->RCGCGPIO  |= (1U << 5U); // enable Run mode for GPIOF
    SYSCTL->GPIOHBCTL |= (1U << 5U); // enable AHB for GPIOF
    __ISB();
    __DSB();

    // configure LEDs (digital output)
    GPIOF_AHB->DIR |= (LED_RED | LED_BLUE | LED_GREEN);
    GPIOF_AHB->DEN |= (LED_RED | LED_BLUE | LED_GREEN);
    GPIOF_AHB->DATA_Bits[LED_RED | LED_BLUE | LED_GREEN] = 0U;

    // configure switches...

    // unlock access to the SW2 pin because it is PROTECTED
    GPIOF_AHB->LOCK = 0x4C4F434BU; // unlock GPIOCR register for SW2
    // commit the write (cast const away)
    *(uint32_t volatile *)&GPIOF_AHB->CR = 0x01U;

    GPIOF_AHB->DIR &= ~(BTN_SW1 | BTN_SW2); // input
    GPIOF_AHB->DEN |= (BTN_SW1 | BTN_SW2); // digital enable
    GPIOF_AHB->PUR |= (BTN_SW1 | BTN_SW2); // pull-up resistor enable

    *(uint32_t volatile *)&GPIOF_AHB->CR = 0x00U;
    GPIOF_AHB->LOCK = 0x0; // lock GPIOCR register for SW2

    // seed the random number generator
    BSP_randomSeed(1234U);

    // initialize the QS software tracing...
    if (QS_INIT((void *)0) == 0U) {
        Q_ERROR();
    }

    // dictionaries...
    QS_OBJ_DICTIONARY(&l_clock_tick);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(PAUSED_STAT);

    QS_ONLY(produce_sig_dict());

    // setup the QS filters...
    QS_GLB_FILTER(QS_ALL_RECORDS); // all records
    QS_GLB_FILTER(-QS_QF_TICK);    // exclude the clock tick
}
//............................................................................
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    Q_UNUSED_PAR(n);

    GPIOF_AHB->DATA_Bits[LED_GREEN] = ((stat[0] == 'e') ? LED_GREEN : 0U);

    // app-specific trace record...
    QS_BEGIN_ID(PHILO_STAT, AO_Table->prio)
        QS_U8(1, n);  // Philosopher number
        QS_STR(stat); // Philosopher status
    QS_END()
}
//............................................................................
void BSP_displayPaused(uint8_t const paused) {
    GPIOF_AHB->DATA_Bits[LED_BLUE] = ((paused != 0U) ? LED_BLUE : 0U);

    // application-specific trace record
    QS_BEGIN_ID(PAUSED_STAT, AO_Table->prio)
        QS_U8(1, paused);  // Paused status
    QS_END()
}
//............................................................................
void BSP_randomSeed(uint32_t const seed) {
    l_rndSeed = seed;
}
//............................................................................
uint32_t BSP_random(void) { // a very cheap pseudo-random-number generator
    // Some flating point code is to exercise the VFP...
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    // NOTE: l_rndSeed is SHARED among the Philo AOs, but does not need
    // to be protected because all Philos have the same ThreadX preemption
    // threshold, so they cannot preempt each other.
    uint32_t rnd = l_rndSeed * (3U*7U*11U*13U*23U);
    l_rndSeed = rnd; // set for the next time

    return (rnd >> 8);
}
//............................................................................
void BSP_ledOn(void) {
    GPIOF_AHB->DATA_Bits[LED_RED] = 0xFFU;
}
//............................................................................
void BSP_ledOff(void) {
    GPIOF_AHB->DATA_Bits[LED_RED] = 0x00U;
}
//............................................................................
void BSP_terminate(int16_t result) {
    Q_UNUSED_PAR(result);
}

// QF callbacks ============================================================

//............................................................................
void QF_onStartup(void) {

    // NOTE:
    // This application uses the ThreadX timer to periodically call
    // the QTimeEvt_tick_(0) function. Here, only the clock tick rate of 0
    // is used, but other timers can be used to call QTimeEvt_tick_() for
    // other clock tick rates, if needed.
    //
    // The choice of a ThreadX timer is not the only option. Applications
    // might choose to call QTIMEEVT_TICK_X() directly from timer interrupts
    // or from active object(s).
    //
    UINT tx_err = tx_timer_create(&l_tick_timer, // ThreadX timer object
        (CHAR *)"TICK0", // name of the timer
        &timer_expiration, // expiration function
        0U,       // expiration function input (tick rate)
        1U,       // initial ticks
        1U,       // reschedule ticks
        TX_AUTO_ACTIVATE);
    Q_ASSERT(tx_err == TX_SUCCESS);

#ifdef Q_SPY
    NVIC_EnableIRQ(UART0_IRQn);  // UART0 interrupt used for QS-RX

    // start a ThreadX "idle" thread. See NOTE1...
    tx_err = tx_thread_create(&idle_thread, // thread control block
        (CHAR *)("idle"),       // thread name
        &idle_thread_fun,       // thread function
        0LU,                    // thread input (unsued)
        idle_thread_stack,       // stack start
        sizeof(idle_thread_stack), // stack size in bytes
        TX_MAX_PRIORITIES - 1U, // ThreadX priority (LOWEST possible), NOTE1
        TX_MAX_PRIORITIES - 1U, // preemption threshold disabled
        TX_NO_TIME_SLICE,
        TX_AUTO_START);
    Q_ASSERT(tx_err == TX_SUCCESS);
#endif // Q_SPY
}
//............................................................................
void QF_onCleanup(void) {
}

// QS callbacks --------------------------------------------------------------
#ifdef Q_SPY

//............................................................................
uint8_t QS_onStartup(void const *arg) {
    Q_UNUSED_PAR(arg);

    static uint8_t qsTxBuf[2*1024]; // buffer for QS-TX channel
    QS_initBuf(qsTxBuf, sizeof(qsTxBuf));

    static uint8_t qsRxBuf[100];    // buffer for QS-RX channel
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    // enable clock for UART0 and GPIOA (used by UART0 pins)
    SYSCTL->RCGCUART |= (1U << 0U); // enable Run mode for UART0
    SYSCTL->RCGCGPIO |= (1U << 0U); // enable Run mode for GPIOA

    // configure UART0 pins for UART operation
    uint32_t tmp = (1U << 0U) | (1U << 1U);
    GPIOA->DIR   &= ~tmp;
    GPIOA->SLR   &= ~tmp;
    GPIOA->ODR   &= ~tmp;
    GPIOA->PUR   &= ~tmp;
    GPIOA->PDR   &= ~tmp;
    GPIOA->AMSEL &= ~tmp;  // disable analog function on the pins
    GPIOA->AFSEL |= tmp;   // enable ALT function on the pins
    GPIOA->DEN   |= tmp;   // enable digital I/O on the pins
    GPIOA->PCTL  &= ~0x00U;
    GPIOA->PCTL  |= 0x11U;

    // configure the UART for the desired baud rate, 8-N-1 operation
    tmp = (((SystemCoreClock * 8U) / UART_BAUD_RATE) + 1U) / 2U;
    UART0->IBRD   = tmp / 64U;
    UART0->FBRD   = tmp % 64U;
    UART0->LCRH   = (0x3U << 5U); // configure 8-N-1 operation
    UART0->LCRH  |= (0x1U << 4U); // enable FIFOs
    UART0->CTL    = (1U << 0U)    // UART enable
                    | (1U << 8U)  // UART TX enable
                    | (1U << 9U); // UART RX enable

    // configure UART interrupts (for the RX channel)
    UART0->IM   |= (1U << 4U) | (1U << 6U); // enable RX and RX-TO interrupt
    UART0->IFLS |= (0x2U << 2U);    // interrupt on RX FIFO half-full
    // NOTE: do not enable the UART0 interrupt yet. Wait till QF_onStartup()

    // configure TIMER5 to produce QS time stamp
    SYSCTL->RCGCTIMER |= (1U << 5U); // enable run mode for Timer5
    TIMER5->CTL  = 0U;               // disable Timer1 output
    TIMER5->CFG  = 0x0U;             // 32-bit configuration
    TIMER5->TAMR = (1U << 4U) | 0x02U; // up-counting periodic mode
    TIMER5->TAILR= 0xFFFFFFFFU;      // timer interval
    TIMER5->ICR  = 0x1U;             // TimerA timeout flag bit clears
    TIMER5->CTL |= (1U << 0U);       // enable TimerA module

    return 1U; // return success
}
//............................................................................
void QS_onCleanup(void) {
}
//............................................................................
QSTimeCtr QS_onGetTime(void) { // NOTE: invoked with interrupts DISABLED
    return TIMER5->TAV;
}
//............................................................................
// NOTE:
// No critical section in QS_onFlush() to avoid nesting of critical sections
// in case QS_onFlush() is called from Q_onError().
void QS_onFlush(void) {
    for (;;) {
        uint16_t b = QS_getByte();
        if (b != QS_EOD) { // NOT end-of-data
            // busy-wait as long as TXE not set
            while ((UART0->FR & UART_FR_TXFE) == 0U) {
            }
            // place the byte in the UART DR register
            UART0->DR = b;
        }
        else {
            break; // break out of the loop
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

#endif // Q_SPY
//----------------------------------------------------------------------------

//============================================================================
// NOTE1:
// ThreadX apparently does not have a concept of an "idle" thread, but
// it can be emulated by a regular, but NON-BLOCKING ThreadX thread of
// the lowest priority.
//
// In the Q_SPY configuration, this "idle" thread is used to perform
// the QS data output to the host. This is not the only choice available, and
// other applications might choose to perform the QS output some other way.
//

