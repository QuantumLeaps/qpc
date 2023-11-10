//============================================================================
// Product: "Dining Philosophers Problem" example, embOS kernel
// Last updated for: @ref qpc_7_3_1
// Last updated on  2023-11-15
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
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
// along with this program. If not, see <www.gnu.org/licenses>.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"                 // QP/C real-time embedded framework
#include "dpp.h"                 // DPP Application interface
#include "bsp.h"                 // Board Support Package

// STM32CubeH7 include files
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo_144.h"
// add other drivers if necessary...

Q_DEFINE_THIS_FILE

// ISRs defined in this BSP ------------------------------------------------
void SysTick_Handler(void);
void USART3_IRQHandler(void);

// Local-scope objects -----------------------------------------------------
static uint32_t l_rnd; // random seed

#ifdef Q_SPY

    // QS source IDs
    static QSpyId const l_embos_ticker = { 0U };

    static UART_HandleTypeDef l_uartHandle;
    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    enum AppRecords { // application-specific trace records
        PHILO_STAT = QS_USER,
        PAUSED_STAT,
    };

#endif

//============================================================================
// Error handler

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    //
    // NOTE: add here your application-specific error handling
    //
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);
    QS_ASSERTION(module, id, 10000U); // report assertion to QS

#ifndef NDEBUG
    BSP_LED_On(LED1); // turn LED on
    BSP_LED_On(LED2); // turn LED on
    BSP_LED_On(LED3); // turn LED on
    // for debugging, hang on in an endless loop...
    for (;;) {
    }
#endif

    NVIC_SystemReset();
    for (;;) {} // explicitly no-retur
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}

// ISRs used in the application ==========================================
#ifdef Q_SPY

// ISR for receiving bytes from the QSPY Back-End
// NOTE: This ISR is "QF-unaware" meaning that it does not interact with
// the QF/QK and is not disabled. Such ISRs don't need to call QK_ISR_ENTRY/
// QK_ISR_EXIT and they cannot post or publish events.
//
void USART3_IRQHandler(void) {
    // is RX register NOT empty?
    if ((l_uartHandle.Instance->ISR & USART_ISR_RXNE_RXFNE) != 0U) {
        uint32_t b = l_uartHandle.Instance->RDR;
        QS_RX_PUT(b);
        l_uartHandle.Instance->ISR &= ~USART_ISR_RXNE_RXFNE; // clear int.
    }
}
#endif

// embOS application hooks =================================================
static void tick_handler(void) { // signature of embOS tick hook routine
    // process time events at rate 0
    QTIMEEVT_TICK_X(0U, &l_embos_ticker);

    // Perform the debouncing of buttons. The algorithm for debouncing
    // adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    // and Michael Barr, page 71.
    static struct {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };

    uint32_t current = BSP_PB_GetState(BUTTON_KEY); // read the Key button
    uint32_t tmp = buttons.depressed; // save debounced depressed buttons
    buttons.depressed |= (buttons.previous & current); // set depressed
    buttons.depressed &= (buttons.previous | current); // clear released
    buttons.previous   = current; // update the history
    tmp ^= buttons.depressed;     // changed debounced depressed
    current = buttons.depressed;

    if (tmp != 0U) {  // debounced Key button state changed?
        if (current != 0U) { // is PB0 depressed?
            static QEvt const pauseEvt = QEVT_INITIALIZER(PAUSE_SIG);
            QACTIVE_PUBLISH(&pauseEvt, &l_embos_ticker);
        }
        else { // the button is released
            static QEvt const serveEvt = QEVT_INITIALIZER(SERVE_SIG);
            QACTIVE_PUBLISH(&serveEvt, &l_embos_ticker);
        }
    }

#ifdef Q_SPY
    tmp = SysTick->CTRL; // clear SysTick_CTRL_COUNTFLAG
    QS_tickTime_ += QS_tickPeriod_; // account for the clock rollover
#endif
}

//............................................................................
//
// OS_Idle() function overridden from RTOSInit_STM32F4x_CMSIS.c
//
// Function description
//   This is basically the "core" of the embOS idle loop.
//   This core loop can be changed, but:
//   The idle loop does not have a stack of its own, therefore no
//   functionality should be implemented that relies on the stack
//   to be preserved. However, a simple program loop can be programmed
//   (like toggling an output or incrementing a counter)
//
void OS_Idle(void) {
    while (1) {

        // toggle the User LED on and then off, see NOTE2
        QF_INT_DISABLE();
        BSP_LED_On(LED3);
        QF_INT_ENABLE();

#ifdef Q_SPY
        QF_INT_DISABLE();
        QS_rxParse();  // parse all the received bytes
        QF_INT_ENABLE();

        if ((l_uartHandle.Instance->ISR & UART_FLAG_TXE) != 0U) { // TXE empty?
            QF_INT_DISABLE();
            uint16_t b = QS_getByte();
            QF_INT_ENABLE();

            if (b != QS_EOD) { // not End-Of-Data?
                l_uartHandle.Instance->TDR = (b & 0xFFU); // put into TDR
            }
        }
#elif defined NDEBUG
        // Put the CPU and peripherals to the low-power mode.
        // you might need to customize the clock management for your application,
        // see the datasheet for your particular Cortex-M MCU.
        //
        // !!!CAUTION!!!
        // The WFI instruction stops the CPU clock, which unfortunately disables
        // the JTAG port, so the ST-Link debugger can no longer connect to the
        // board. For that reason, the call to __WFI() has to be used with CAUTION.
        //
        // NOTE: If you find your board "frozen" like this, strap BOOT0 to VDD and
        // reset the board, then connect with ST-Link Utilities and erase the part.
        // The trick with BOOT(0) is it gets the part to run the System Loader
        // instead of your broken code. When done disconnect BOOT0, and start over.
        //
        //__WFI(); // Wait-For-Interrupt
#endif
    }
}

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

    // NOTE: VFP (hardware Floating Point) unit is configured by embOS

    // enable clock for to the peripherals used by this application...
    SCB_EnableICache(); // Enable I-Cache
    SCB_EnableDCache(); // Enable D-Cache

    // configure Flash prefetch and Instr. cache through ART accelerator
#if (ART_ACCLERATOR_ENABLE != 0)
    __HAL_FLASH_ART_ENABLE();
#endif // ART_ACCLERATOR_ENABLE

    // Configure the LEDs
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    // configure the User Button in GPIO Mode
    BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

    // seed the random number generator
    BSP_randomSeed(1234U);

    // initialize the QS software tracing...
    if (QS_INIT((void *)0) == 0U) {
        Q_ERROR();
    }

    // dictionaries...
    QS_OBJ_DICTIONARY(&l_embos_ticker);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(PAUSED_STAT);

    QS_ONLY(produce_sig_dict());

    // setup the QS filters...
    QS_GLB_FILTER(QS_ALL_RECORDS); // all records
    QS_GLB_FILTER(-QS_QF_TICK);    // exclude the clock tick
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
    static OS_STACKPTR int philoStack[N_PHILO][128];
    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        Philo_ctor(n);
        QACTIVE_START(AO_Philo[n],
            Q_PRIO(n + 3U, 3U),      // QP-prio., embOS prio.
            philoQueueSto[n],        // event queue storage
            Q_DIM(philoQueueSto[n]), // queue length [events]
            philoStack[n],           // private stack for embOS
            sizeof(philoStack[n]),   // stack size [bytes]
            (void *)0);              // no initialization param
    }

    static QEvt const *tableQueueSto[N_PHILO];
    static OS_STACKPTR int tableStack[128];
    Table_ctor();
    QACTIVE_START(AO_Table,
        Q_PRIO(N_PHILO + 7U, 7U),// QP-prio., embOS prio.
        tableQueueSto,           // event queue storage
        Q_DIM(tableQueueSto),    // queue length [events]
        tableStack,              // private stack for embOS
        sizeof(tableStack),      // stack size [bytes]
        (void *)0);              // no initialization param
}
//............................................................................
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    (void)n;

    if (stat[0] == 'h') {
        BSP_LED_On(LED1); // turn LED on
    }
    else {
        BSP_LED_Off(LED1); // turn LED off
    }
    if (stat[0] == 'e') {
        BSP_LED_On(LED2); // turn LED on
    }
    else {
        BSP_LED_Off(LED2); // turn LED on
    }

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) // app-specific record
        QS_U8(1, n);  // Philosopher number
        QS_STR(stat); // Philosopher status
    QS_END()
}
//............................................................................
void BSP_displayPaused(uint8_t paused) {
    if (paused) {
        BSP_LED_On(LED3); // turn LED on
    }
    else {
        BSP_LED_Off(LED3); // turn LED on
    }

    QS_BEGIN_ID(PAUSED_STAT, 0U) // app-specific record
        QS_U8(1, paused);  // Paused status
    QS_END()
}
//............................................................................
uint32_t BSP_random(void) { // a very cheap pseudo-random-number generator
    // exercise the FPU with some floating point computations
    // NOTE: this code can be only called from a task that created with
    // the option OS_TASK_OPT_SAVE_FP.
    //
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    // "Super-Duper" Linear Congruential Generator (LCG)
    // LCG(2^32, 3*7*11*13*23, 0, seed)
    uint32_t rnd = l_rnd * (3U*7U*11U*13U*23U);
    l_rnd = rnd; // set for the next time

    return (rnd >> 8);
}
//............................................................................
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
//............................................................................
void BSP_terminate(int16_t result) {
    (void)result;
}

// QF callbacks ============================================================
void QF_onStartup(void) {
    static OS_TICK_HOOK tick_hook;
    OS_TICK_AddHook(&tick_hook, &tick_handler);

#ifdef Q_SPY
    NVIC_SetPriority(USART3_IRQn,  0U); // kernel unaware interrupt
    NVIC_EnableIRQ(USART3_IRQn); // UART interrupt used for QS-RX
#endif
}
//............................................................................
void QF_onCleanup(void) {
}

// QS callbacks ============================================================
#ifdef Q_SPY

//............................................................................
uint8_t QS_onStartup(void const *arg) {
    Q_UNUSED_PAR(arg);

    static uint8_t qsTxBuf[2*1024]; // buffer for QS-TX channel
    QS_initBuf  (qsTxBuf, sizeof(qsTxBuf));

    static uint8_t qsRxBuf[100];    // buffer for QS-RX channel
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    l_uartHandle.Instance        = USART3;
    l_uartHandle.Init.BaudRate   = 115200;
    l_uartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    l_uartHandle.Init.StopBits   = UART_STOPBITS_1;
    l_uartHandle.Init.Parity     = UART_PARITY_NONE;
    l_uartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    l_uartHandle.Init.Mode       = UART_MODE_TX_RX;
    l_uartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&l_uartHandle) != HAL_OK) {
        return 0U; // return failure
    }

    // Set UART to receive 1 byte at a time via interrupt
    HAL_UART_Receive_IT(&l_uartHandle, (uint8_t *)qsRxBuf, 1);
    // NOTE: wait till QF_onStartup() to enable UART interrupt in NVIC

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
void QS_onFlush(void) {
    while (true) {
        // try to get next byte to transmit
        QF_CRIT_STAT
        QF_CRIT_ENTRY();
        uint16_t b = QS_getByte();
        QF_CRIT_EXIT();

        if (b != QS_EOD) { // NOT end-of-data
            // busy-wait as long as TXE not set
            while ((l_uartHandle.Instance->ISR & UART_FLAG_TXE) == 0U) {
            }
            // place the byte in the UART TDR register
            l_uartHandle.Instance->TDR = b;
        }
        else {
            break; // break out of the loop
        }
    }
}
//............................................................................
//! callback function to reset the target (to be implemented in the BSP)
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
// One of the LEDs is used to visualize the idle loop activity. The brightness
// of the LED is proportional to the frequency of invcations of the idle loop.
// Please note that the LED is toggled with interrupts disabled, so no
// interrupt execution time contributes to the brightness of the LED.
//
// NOTE2:
// If you find your board "frozen" like this, strap BOOT0 to VDD and reset
// the board, then connect with ST-Link Utilities and erase the part. The
// trick with BOOT(0) is it gets the part to run the System Loader instead of
// your broken code. When done disconnect BOOT0, and start over.
//

