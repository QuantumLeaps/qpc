//============================================================================
// Product: DPP example, NUCLEO-H743ZI board, QK kernel
// Last updated for version 7.3.0
// Last updated on  2023-08-22
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

// STM32CubeH7 include files
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo_144.h"
// add other drivers if necessary...

Q_DEFINE_THIS_FILE

// ISRs defined in this BSP ------------------------------------------------
void SysTick_Handler(void);
void USART3_IRQHandler(void);

// Local-scope objects -----------------------------------------------------
static uint32_t l_rndSeed;

#ifdef Q_SPY

    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    // QSpy source IDs
    static QSpyId const l_SysTick_Handler = { 0U };

    static UART_HandleTypeDef l_uartHandle;

    enum AppRecords { // application-specific trace records
        PHILO_STAT = QS_USER,
        PAUSED_STAT,
        CONTEXT_SW,
    };

#endif

//============================================================================
// Error handler and ISRs...

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    // NOTE: this implementation of the assertion handler is intended only
    // for debugging and MUST be changed for deployment of the application
    // (assuming that you ship your production code with assertions enabled).
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);
    QS_ASSERTION(module, id, 10000U);

#ifndef NDEBUG
    // light all LEDs
    BSP_LED_On(LED1);
    BSP_LED_On(LED2);
    BSP_LED_On(LED3);
    // for debugging, hang on in an endless loop...
    for (;;) {
    }
#endif

    NVIC_SystemReset();
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}


// ISRs used in the application ============================================

void SysTick_Handler(void); // prototype
void SysTick_Handler(void) {
    QK_ISR_ENTRY();   // inform QK about entering an ISR

    uint32_t tmp;
#ifdef Q_SPY
    {
        tmp = SysTick->CTRL; // clear SysTick_CTRL_COUNTFLAG
        QS_tickTime_ += QS_tickPeriod_; // account for the clock rollover
    }
#endif

    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler); // time events for rate 0

    // Perform the debouncing of buttons. The algorithm for debouncing
    // adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    // and Michael Barr, page 71.
    //
    // state of the button debouncing
    static struct {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };

    uint32_t current = BSP_PB_GetState(BUTTON_USER); // read the User button
    tmp = buttons.depressed; // save the depressed buttons
    buttons.depressed |= (buttons.previous & current); // set depressed
    buttons.depressed &= (buttons.previous | current); // clear released
    buttons.previous   = current; // update the history
    tmp ^= buttons.depressed;     // changed debounced depressed
    current = buttons.depressed;

    if (tmp != 0U) {  // debounced Key button state changed?
        if (current != 0U) { // PB0 depressed?
            static QEvt const pauseEvt = QEVT_INITIALIZER(PAUSE_SIG);
            QACTIVE_PUBLISH(&pauseEvt, &l_SysTick_Handler);
        }
        else { // the button is released
            static QEvt const serveEvt = QEVT_INITIALIZER(SERVE_SIG);
            QACTIVE_PUBLISH(&serveEvt, &l_SysTick_Handler);
        }
    }

    QK_ISR_EXIT();  // inform QK about exiting an ISR
}

//............................................................................
#ifdef Q_SPY
// ISR for receiving bytes from the QSPY Back-End
// NOTE: This ISR is "QF-unaware" meaning that it does not interact with
// the QF/QK and is not disabled. Such ISRs don't need to call QK_ISR_ENTRY/
// QK_ISR_EXIT and they cannot post or publish events.

void USART3_IRQHandler(void); // prototype
void USART3_IRQHandler(void) {
    // is RX register NOT empty?
    if ((l_uartHandle.Instance->ISR & USART_ISR_RXNE_RXFNE) != 0) {
        uint32_t b = l_uartHandle.Instance->RDR;
        QS_RX_PUT(b);
        l_uartHandle.Instance->ISR &= ~USART_ISR_RXNE_RXFNE; // clear int.
    }

    QK_ARM_ERRATUM_838869();
}
#endif // Q_SPY

//............................................................................
#ifdef QF_ON_CONTEXT_SW
// NOTE: the context-switch callback is called with interrupts DISABLED
void QF_onContextSw(QActive *prev, QActive *next) {
    QS_BEGIN_INCRIT(CONTEXT_SW, 0U) // in critical section!
        QS_OBJ(prev);
        QS_OBJ(next);
    QS_END_INCRIT()
}
#endif // QF_ON_CONTEXT_SW

//............................................................................
// MPU setup for STM32H743ZI MCU
static void STM32H743ZI_MPU_setup(void) {
    // The following MPU configuration contains just a generic ROM
    // region (with read-only access) and NULL-pointer protection region.
    // Otherwise, the MPU will fall back on the background region (PRIVDEFENA).
    //
    static struct {
        uint32_t rbar;
        uint32_t rasr;
    } const mpu_setup[] = {

        { // region #0: Flash: base=0x0000'0000, size=512M=2^(28+1)
          0x00000000U                       // base address
              | MPU_RBAR_VALID_Msk          // valid region
              | (MPU_RBAR_REGION_Msk & 0U), // region #0
          (28U << MPU_RASR_SIZE_Pos)        // 2^(18+1) region
              | (0x6U << MPU_RASR_AP_Pos)   // PA:ro/UA:ro
              | (1U << MPU_RASR_C_Pos)      // C=1
              | MPU_RASR_ENABLE_Msk         // region enable
        },

        { // region #7: NULL-pointer: base=0x000'0000, size=128M=2^(26+1)
          // NOTE: this region extends to  0x080'0000, which is where
          // the ROM is re-mapped by STM32
          //
          0x00000000U                       // base address
              | MPU_RBAR_VALID_Msk          // valid region
              | (MPU_RBAR_REGION_Msk & 7U), // region #7
          (26U << MPU_RASR_SIZE_Pos)        // 2^(26+1)=128M region
              | (0x0U << MPU_RASR_AP_Pos)   // PA:na/UA:na
              | (1U << MPU_RASR_XN_Pos)     // XN=1
              | MPU_RASR_ENABLE_Msk         // region enable
        },
    };

    // enable the MemManage_Handler for MPU exception
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

    __DSB();
    MPU->CTRL = 0U; // disable the MPU
    for (uint_fast8_t n = 0U; n < Q_DIM(mpu_setup); ++n) {
        MPU->RBAR = mpu_setup[n].rbar;
        MPU->RASR = mpu_setup[n].rasr;
    }
    MPU->CTRL = MPU_CTRL_ENABLE_Msk         // enable the MPU
                | MPU_CTRL_PRIVDEFENA_Msk;  // enable background region
    __ISB();
    __DSB();
}

//============================================================================
// BSP functions...

void BSP_init(void) {
    // setup the MPU...
    STM32H743ZI_MPU_setup();

    // NOTE: SystemInit() has been already called from the startup code
    // but SystemCoreClock needs to be updated
    SystemCoreClockUpdate();

    SCB_EnableICache(); // Enable I-Cache
    SCB_EnableDCache(); // Enable D-Cache

    // Configure Flash prefetch and Instr. cache through ART accelerator
#if (ART_ACCLERATOR_ENABLE != 0)
    __HAL_FLASH_ART_ENABLE();
#endif // ART_ACCLERATOR_ENABLE

    // Configure the LEDs
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    // Configure the User Button in GPIO Mode
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

    //...
    BSP_randomSeed(1234U);

    // initialize the QS software tracing...
    if (!QS_INIT((void *)0)) {
        Q_ERROR();
    }

    // dictionaries...
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
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

            // NOTE: set the preemption-threshold of all Philos to
            // the same level, so that they cannot preempt each other.
            Q_PRIO(n + 3U, N_PHILO + 2U), // QF-prio/pre-thre.

            philoQueueSto[n],        // event queue storage
            Q_DIM(philoQueueSto[n]), // queue length [events]
            (void *)0, 0U,           // no stack storage
            (void *)0);              // no initialization param
    }

    static QEvt const *tableQueueSto[N_PHILO];
    Table_ctor();
    QACTIVE_START(AO_Table,
        N_PHILO + 7U,            // QP prio. of the AO
        tableQueueSto,           // event queue storage
        Q_DIM(tableQueueSto),    // queue length [events]
        (void *)0, 0U,           // no stack storage
        (void *)0);              // no initialization param
}
//............................................................................
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    if (stat[0] == 'e') {
        BSP_LED_On(LED1);
    }
    else {
        BSP_LED_Off(LED1);
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
        BSP_LED_On(LED2);
    }
    else {
        BSP_LED_Off(LED2);
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
    // Some flating point code is to exercise the VFP...
    double volatile x = 3.1415926;
    x = x + 2.7182818;

    QSchedStatus lockStat = QK_schedLock(N_PHILO); // N_PHILO prio. ceiling
    // "Super-Duper" Linear Congruential Generator (LCG)
    // LCG(2^32, 3*7*11*13*23, 0, seed)
    //
    uint32_t rnd = l_rndSeed * (3U*7U*11U*13U*23U);
    l_rndSeed = rnd; // set for the next time
    QK_schedUnlock(lockStat);

    return (rnd >> 8U);
}
//............................................................................
void BSP_terminate(int16_t result) {
    (void)result;
}
//............................................................................
void BSP_ledOn(void) {
    BSP_LED_On(LED1);
}
//............................................................................
void BSP_ledOff(void) {
    BSP_LED_Off(LED1);
}

//============================================================================
// QF callbacks...
void QF_onStartup(void) {
    // assign all priority bits for preemption-prio. and none to sub-prio.
    // NOTE: this might have been changed by STM32Cube.
    //
    NVIC_SetPriorityGrouping(0U);

    // set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    // set priorities of ALL ISRs used in the system, see NOTE1
    NVIC_SetPriority(USART3_IRQn,  0U); // kernel unaware interrupt
    NVIC_SetPriority(SysTick_IRQn, QF_AWARE_ISR_CMSIS_PRI);
    // ...

    // enable IRQs...
#ifdef Q_SPY
    NVIC_EnableIRQ(USART3_IRQn); // UART interrupt used for QS-RX
#endif
}
//............................................................................
void QF_onCleanup(void) {
}
//............................................................................
void QK_onIdle(void) {
    // toggle the User LED on and then off, see NOTE2
    QF_INT_DISABLE();
    BSP_LED_On (LED3);
    BSP_LED_Off(LED3);
    QF_INT_ENABLE();

    // Some floating point code is to exercise the VFP...
    double volatile x = 1.73205;
    x = x * 1.73205;

#ifdef Q_SPY
    QF_INT_DISABLE();
    QS_rxParse();  // parse all the received bytes
    QF_INT_ENABLE();
    QF_CRIT_EXIT_NOP();

    QF_INT_DISABLE();
    if ((l_uartHandle.Instance->ISR & UART_FLAG_TXE) != 0U) { // TXE empty?
        QF_INT_DISABLE();
        uint16_t b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {  // not End-Of-Data?
            l_uartHandle.Instance->TDR = b;  // put into TDR
        }
    }
    QF_INT_ENABLE();
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

//============================================================================
// QS callbacks...
#ifdef Q_SPY

//............................................................................
uint8_t QS_onStartup(void const *arg) {
    Q_UNUSED_PAR(arg);

    static uint8_t qsTxBuf[2*1024]; // buffer for QS-TX channel
    QS_initBuf(qsTxBuf, sizeof(qsTxBuf));

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
    else { // the rollover occured, but the SysTick_ISR did not run yet
        return QS_tickTime_ + QS_tickPeriod_ - (QSTimeCtr)SysTick->VAL;
    }
}
//............................................................................
void QS_onFlush(void) {
    for (;;) {
        QF_INT_DISABLE();
        uint16_t b = QS_getByte();
        if (b != QS_EOD) {
            while ((l_uartHandle.Instance->ISR & UART_FLAG_TXE) == 0U) {
                QF_INT_ENABLE();
                QF_CRIT_EXIT_NOP();

                QF_INT_DISABLE();
            }
            l_uartHandle.Instance->TDR = b;
            QF_INT_ENABLE();
        }
        else {
            QF_INT_ENABLE();
            break;
        }
    }
}
//............................................................................
//! callback function to reset the target (to be implemented in the BSP)
void QS_onReset(void) {
    NVIC_SystemReset();
}
//............................................................................
//! callback function to execute a user command
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
// NOTE0:
// The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
// ISR priority that is disabled by the QF framework. The value is suitable
// for the NVIC_SetPriority() CMSIS function.
//
// Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
// with the numerical values of priorities equal or higher than
// QF_AWARE_ISR_CMSIS_PRI) are allowed to call the QK_ISR_ENTRY/QK_ISR_ENTRY
// macros or any other QF services. These ISRs are "QF-aware".
//
// Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
// level (i.e., with the numerical values of priorities less than
// QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
// Such "QF-unaware" ISRs cannot call any QF services. In particular they
// can NOT call the macros QK_ISR_ENTRY/QK_ISR_ENTRY. The only mechanism
// by which a "QF-unaware" ISR can communicate with the QF framework is by
// triggering a "QF-aware" ISR, which can post/publish events.
//
// NOTE2:
// The User LED is used to visualize the idle loop activity. The brightness
// of the LED is proportional to the frequency of invcations of the idle loop.
// Please note that the LED is toggled with interrupts locked, so no interrupt
// execution time contributes to the brightness of the User LED.
//
