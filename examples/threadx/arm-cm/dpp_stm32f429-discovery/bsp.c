//============================================================================
// DPP example, STM32F429 Discovery board, ThreadX kernel
// Last updated for version 7.3.3
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

#include "stm32f4xx.h"  // CMSIS-compliant header file for the MCU used
#include "stm32f4xx_exti.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
// add other drivers if necessary...

Q_DEFINE_THIS_FILE

// Local-scope defines -----------------------------------------------------
#define LED_GPIO_PORT     GPIOD
#define LED_GPIO_CLK      RCC_AHB1Periph_GPIOD

#define LED4_PIN          GPIO_Pin_12
#define LED3_PIN          GPIO_Pin_13
#define LED5_PIN          GPIO_Pin_14
#define LED6_PIN          GPIO_Pin_15

#define BTN_GPIO_PORT     GPIOA
#define BTN_GPIO_CLK      RCC_AHB1Periph_GPIOA
#define BTN_B1            GPIO_Pin_0

static uint32_t l_rndSeed;
static TX_TIMER l_tick_timer; // ThreadX timer to call QTIMEEVT_TICK_X()

#ifdef Q_SPY

    // QSpy source IDs
    static QSpyId const l_clock_tick = { QS_AP_ID };

    // ThreadX "idle" thread for QS output, see NOTE1
    static TX_THREAD idle_thread;
    static ULONG idle_thread_stack[100];

    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    enum AppRecords { // application-specific trace records
        PHILO_STAT = QS_USER,
        PAUSED_STAT,
    };

#endif

static TX_TIMER l_tick_timer; // ThreadX timer to call QTIMEEVT_TICK_X()
static VOID timer_expiration(ULONG id);
static VOID timer_expiration(ULONG id) {
    QTIMEEVT_TICK_X(id, &l_clock_tick); // QF clock tick processing

#ifdef Q_SPY
    uint32_t volatile tmp = SysTick->CTRL; // clear SysTick_CTRL_COUNTFLAG
    QS_tickTime_ += QS_tickPeriod_; // account for the clock rollover
#endif
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
    // turn LED on
    LED_GPIO_PORT->BSRRL = LED4_PIN;
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
        // turn the LED6 on an off to visualize the QS activity
        LED_GPIO_PORT->BSRRL = LED6_PIN; // turn LED on
        __NOP(); // wait a little to actually see the LED glow
        __NOP();
        __NOP();
        __NOP();
        LED_GPIO_PORT->BSRRH = LED6_PIN; // turn LED off

        QS_rxParse();  // parse all the received bytes
        QF_CRIT_EXIT();

        if ((USART2->SR & 0x80U) != 0U) { // is TXE empty?
            QF_CRIT_ENTRY();
            uint16_t b = QS_getByte();
            QF_CRIT_EXIT();

            if (b != QS_EOD) {  // not End-Of-Data?
                USART2->DR = b; // put into the DR register
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
//
//TBD...

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

    // Initialize port for the LEDs
    RCC_AHB1PeriphClockCmd(LED_GPIO_CLK , ENABLE);

    // GPIO Configuration for the LEDs...
    GPIO_InitTypeDef GPIO_struct;
    GPIO_struct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_struct.GPIO_OType = GPIO_OType_PP;
    GPIO_struct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_struct.GPIO_Pin = LED3_PIN;
    GPIO_Init(LED_GPIO_PORT, &GPIO_struct);
    LED_GPIO_PORT->BSRRH = LED3_PIN; // turn LED off

    GPIO_struct.GPIO_Pin = LED4_PIN;
    GPIO_Init(LED_GPIO_PORT, &GPIO_struct);
    LED_GPIO_PORT->BSRRH = LED4_PIN; // turn LED off

    GPIO_struct.GPIO_Pin = LED5_PIN;
    GPIO_Init(LED_GPIO_PORT, &GPIO_struct);
    LED_GPIO_PORT->BSRRH = LED5_PIN; // turn LED off

    GPIO_struct.GPIO_Pin = LED6_PIN;
    GPIO_Init(LED_GPIO_PORT, &GPIO_struct);
    LED_GPIO_PORT->BSRRH = LED6_PIN; // turn LED off

    // Initialize thr port for Button
    RCC_AHB1PeriphClockCmd(BTN_GPIO_CLK , ENABLE);

    // GPIO Configuration for the Button...
    GPIO_struct.GPIO_Pin   = BTN_B1;
    GPIO_struct.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_struct.GPIO_OType = GPIO_OType_PP;
    GPIO_struct.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BTN_GPIO_PORT, &GPIO_struct);

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

    if (stat[0] == 'h') {
        LED_GPIO_PORT->BSRRL = LED3_PIN; // turn LED on
    }
    else {
        LED_GPIO_PORT->BSRRH = LED3_PIN; // turn LED off
    }
    if (stat[0] == 'e') {
        LED_GPIO_PORT->BSRRL = LED5_PIN; // turn LED on
    }
    else {
        LED_GPIO_PORT->BSRRH = LED5_PIN; // turn LED on
    }

    // app-specific trace record...
    QS_BEGIN_ID(PHILO_STAT, AO_Table->prio)
        QS_U8(1, n);  // Philosopher number
        QS_STR(stat); // Philosopher status
    QS_END()
}
//............................................................................
void BSP_displayPaused(uint8_t const paused) {
    if (paused) {
        LED_GPIO_PORT->BSRRL = LED4_PIN; // turn LED on
    }
    else {
        LED_GPIO_PORT->BSRRH = LED4_PIN; // turn LED off
    }

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
    LED_GPIO_PORT->BSRRL = LED3_PIN;
}
//............................................................................
void BSP_ledOff(void) {
    LED_GPIO_PORT->BSRRH = LED3_PIN;
}
//............................................................................
void BSP_terminate(int16_t result) {
    (void)result;
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
    //TBD: enable the UART ISR for receiving bytes...

    // start a ThreadX "idle" thread. See NOTE1...
    tx_err = tx_thread_create(&idle_thread, // thread control block
        (CHAR *)("idle"), // thread name
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

    //TBD initialize the QS-RX channel...

    // enable peripheral clock for USART2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // GPIOA clock enable
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // GPIOA Configuration:  USART2 TX on PA2
    GPIO_InitTypeDef GPIO_struct;
    GPIO_struct.GPIO_Pin = GPIO_Pin_2;
    GPIO_struct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_struct.GPIO_OType = GPIO_OType_PP;
    GPIO_struct.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOA, &GPIO_struct);

    // Connect USART2 pins to AF2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // TX = PA2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // RX = PA3

    USART_InitTypeDef USART_struct;
    USART_struct.USART_BaudRate = 115200U;
    USART_struct.USART_WordLength = USART_WordLength_8b;
    USART_struct.USART_StopBits = USART_StopBits_1;
    USART_struct.USART_Parity = USART_Parity_No;
    USART_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_struct.USART_Mode = USART_Mode_Tx;
    USART_Init(USART2, &USART_struct);

    USART_Cmd(USART2, ENABLE); // enable USART2

    // configure UART interrupts (for the RX channel)
    //TBD...

    QS_tickPeriod_ = SystemCoreClock / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_; // to start the timestamp at zero

    return 1U; // return success
}
//............................................................................
void QS_onCleanup(void) {
}
//............................................................................
QSTimeCtr QS_onGetTime(void) { // NOTE: invoked with interrupts DISABLED
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) { // not set?
        return QS_tickTime_ - (QSTimeCtr)SysTick->VAL;
    }
    else { // the rollover occured, but the SysTick_ISR did not run yet
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
        if (b != QS_EOD) { // NOT end-of-data
            // busy-wait as long as TXE not set
            while ((USART2->SR & USART_FLAG_TXE) == 0U) {
            }
            // place the byte in the UART DR register
            USART2->DR = b;
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

