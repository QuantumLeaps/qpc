/*****************************************************************************
* Product: DPP example, STM32746G-Discovery board, FreeRTOS kernel
* Last updated for version 6.9.1
* Last updated on  2020-10-06
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
#include "dpp.h"
#include "bsp.h"

/* STM32Cube include files */
#include "stm32f7xx_hal.h"
#include "stm32746g_discovery.h"
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE /* define the name of this file for assertions */

/* "RTOS-aware" interrupt priorities for FreeRTOS on ARM Cortex-M, NOTE1 */
#define RTOS_AWARE_ISR_CMSIS_PRI \
    (configMAX_SYSCALL_INTERRUPT_PRIORITY >> (8-__NVIC_PRIO_BITS))

/* Local-scope objects -----------------------------------------------------*/
static uint32_t l_rnd; /* random seed */

#ifdef Q_SPY

    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    /* QS identifiers for non-QP sources of events */
    static uint8_t const l_TickHook = 0U;
    static UART_HandleTypeDef l_uartHandle;
    static uint8_t const l_EXTI0_IRQHandler = 0U;

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER,
        PAUSED_STAT,
        COMMAND_STAT
    };

#endif

/* ISRs used in this project ===============================================*/
/* NOTE: only the "FromISR" API variants are allowed in the ISRs! */
void EXTI0_IRQHandler(void); // prototype
void EXTI0_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* for testing... */
    QACTIVE_POST_FROM_ISR(AO_Table,
        Q_NEW_FROM_ISR(QEvt, MAX_PUB_SIG),
        &xHigherPriorityTaskWoken,
        &l_EXTI0_IRQHandler);

    /* the usual end of FreeRTOS ISR... */
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
/*..........................................................................*/
#ifdef Q_SPY
/*
* ISR for receiving bytes from the QSPY Back-End
* NOTE: This ISR is "kernel-unaware" meaning that it does not interact with
* the FreeRTOS or QP and is not disabled. Such ISRs don't need to call
* portEND_SWITCHING_ISR(() at the end, but they also cannot call any
* FreeRTOS or QP APIs.
*/
void USART1_IRQHandler(void); /* prototype */
void USART1_IRQHandler(void) {
    /* is RX register NOT empty? */
    if ((l_uartHandle.Instance->ISR & USART_ISR_RXNE) != 0) {
        uint32_t b = l_uartHandle.Instance->RDR;
        QS_RX_PUT(b);
        l_uartHandle.Instance->ISR &= ~USART_ISR_RXNE; /* clear interrupt */
    }
}
#endif

/* Application hooks used in this project ==================================*/
/* NOTE: only the "FromISR" API variants are allowed in vApplicationTickHook */
void vApplicationTickHook(void) {
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { ~0U, ~0U };
    uint32_t current;
    uint32_t tmp;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* process time events for rate 0 */
    QF_TICK_X_FROM_ISR(0U, &xHigherPriorityTaskWoken, &l_TickHook);

#ifdef Q_SPY
    {
        tmp = SysTick->CTRL; /* clear SysTick_CTRL_COUNTFLAG */
        QS_tickTime_ += QS_tickPeriod_; /* account for the clock rollover */
    }
#endif

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = BSP_PB_GetState(BUTTON_KEY); /* read the Key button */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if (tmp != 0U) {  /* debounced Key button state changed? */
        if (buttons.depressed != 0U) { /* PB0 depressed?*/
            /* demonstrate the ISR APIs: QF_PUBLISH_FROM_ISR and Q_NEW_FROM_ISR */
            QF_PUBLISH_FROM_ISR(Q_NEW_FROM_ISR(QEvt, PAUSE_SIG),
                                &xHigherPriorityTaskWoken,
                                &l_TickHook);
        }
        else { /* the button is released */
            /* demonstrate the ISR APIs: POST_FROM_ISR and Q_NEW_FROM_ISR */
            QACTIVE_POST_FROM_ISR(AO_Table,
                                  Q_NEW_FROM_ISR(QEvt, SERVE_SIG),
                                  &xHigherPriorityTaskWoken,
                                  &l_TickHook);
        }
    }

    /* notify FreeRTOS to perform context switch from ISR, if needed */
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
/*..........................................................................*/
void vApplicationIdleHook(void) {
    float volatile x;

    /* toggle the User LED on and then off, see NOTE01 */
    QF_INT_DISABLE();
    //BSP_LED_On(LED3); not enough LEDs
    //BSP_LED_On(LED3); not enough LEDs
    QF_INT_ENABLE();

    /* Some flating point code is to exercise the VFP... */
    x = 1.73205F;
    x = x * 1.73205F;

#ifdef Q_SPY
    QS_rxParse();  /* parse all the received bytes */

    if ((l_uartHandle.Instance->ISR & UART_FLAG_TXE) != 0U) { /* TXE empty? */
        uint16_t b;

        b = QS_getByte();

        if (b != QS_EOD) {  /* not End-Of-Data? */
            l_uartHandle.Instance->TDR = (b & 0xFFU);  /* put into TDR */
        }
    }
#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    /* !!!CAUTION!!!
    * The WFI instruction stops the CPU clock, which unfortunately disables
    * the JTAG port, so the ST-Link debugger can no longer connect to the
    * board. For that reason, the call to __WFI() has to be used with CAUTION.
    *
    * NOTE: If you find your board "frozen" like this, strap BOOT0 to VDD and
    * reset the board, then connect with ST-Link Utilities and erase the part.
    * The trick with BOOT(0) is it gets the part to run the System Loader
    * instead of your broken code. When done disconnect BOOT0, and start over.
    */
    //__WFI(); /* Wait-For-Interrupt */
#endif
}
/*..........................................................................*/
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void)xTask;
    (void)pcTaskName;
    Q_ERROR();
}
/*..........................................................................*/
/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must
* provide an implementation of vApplicationGetIdleTaskMemory() to provide
* the memory that is used by the Idle task.
*/
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
    /* If the buffers to be provided to the Idle task are declared inside
    * this function then they must be declared static - otherwise they will
    * be allocated on the stack and so not exists after this function exits.
    */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the
    * Idle task's state will be stored.
    */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    * Note that, as the array is necessarily of type StackType_t,
    * configMINIMAL_STACK_SIZE is specified in words, not bytes.
    */
    *pulIdleTaskStackSize = Q_DIM(uxIdleTaskStack);
}

/* BSP functions ===========================================================*/
void BSP_init(void) {
    /* NOTE: SystemInit() has been already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    /* NOTE: The VFP (hardware Floating Point) unit is configured by FreeRTOS */

    SCB_EnableICache(); /* Enable I-Cache */
    SCB_EnableDCache(); /* Enable D-Cache */

    /* Configure Flash prefetch and Instr. cache through ART accelerator */
#if (ART_ACCLERATOR_ENABLE != 0)
    __HAL_FLASH_ART_ENABLE();
#endif /* ART_ACCLERATOR_ENABLE */

    /* Configure LED1 */
    BSP_LED_Init(LED1);

    /* Configure the User Button in GPIO Mode */
    BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

    //...
    BSP_randomSeed(1234U);

    /* initialize the QS software tracing... */
    if (QS_INIT((void *)0) == 0) {
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_TickHook);
    QS_OBJ_DICTIONARY(&l_EXTI0_IRQHandler);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(PAUSED_STAT);
    QS_USR_DICTIONARY(COMMAND_STAT);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_SM_RECORDS); /* state machine records */
    QS_GLB_FILTER(QS_AO_RECORDS); /* active object records */
    QS_GLB_FILTER(QS_UA_RECORDS); /* all user records */
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    if (stat[0] == 'e') {
        BSP_LED_On(LED1);
    }
    else {
        BSP_LED_Off(LED1);
    }

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()          /* application-specific record end */
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    if (paused != 0U) {
        //BSP_LED_On(LED2); not enough LEDs
    }
    else {
        //BSP_LED_Off(LED2); not enough LEDs
    }
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    uint32_t rnd;

    /* Some flating point code is to exercise the VFP... */
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    vTaskSuspendAll(); /* lock FreeRTOS scheduler */
    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    rnd = l_rnd * (3U*7U*11U*13U*23U);
    l_rnd = rnd; /* set for the next time */
    xTaskResumeAll(); /* unlock the FreeRTOS scheduler */

    return (rnd >> 8);
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
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    //SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC); // done in FreeRTOS

    /* assing all priority bits for preemption-prio. and none to sub-prio. */
    NVIC_SetPriorityGrouping(0U);

    /* set priorities of ALL ISRs used in the system, see NOTE00
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    * Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
    * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
    */
    NVIC_SetPriority(USART1_IRQn,  0U); /* kernel unaware interrupt */
    NVIC_SetPriority(EXTI0_IRQn,   RTOS_AWARE_ISR_CMSIS_PRI);
    NVIC_SetPriority(SysTick_IRQn, RTOS_AWARE_ISR_CMSIS_PRI + 1U);
    /* ... */

    /* enable IRQs... */
    NVIC_EnableIRQ(EXTI0_IRQn);
#ifdef Q_SPY
    NVIC_EnableIRQ(USART1_IRQn); /* UART interrupt used for QS-RX */
#endif
}
/*..........................................................................*/
void QF_onCleanup(void) {
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char_t const * const module, int_t const loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;
    QS_ASSERTION(module, loc, 10000U); /* report assertion to QS */

#ifndef NDEBUG
    /* light up LED */
    BSP_LED_On(LED1);
    /* for debugging, hang on in an endless loop... */
    for (;;) {
    }
#endif

    NVIC_SystemReset();
}

/* QS callbacks ============================================================*/
#ifdef Q_SPY
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsTxBuf[2*1024]; /* buffer for QS-TX channel */
    static uint8_t qsRxBuf[100];    /* buffer for QS-RX channel */

    QS_initBuf  (qsTxBuf, sizeof(qsTxBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    l_uartHandle.Instance        = USART1;
    l_uartHandle.Init.BaudRate   = 115200;
    l_uartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    l_uartHandle.Init.StopBits   = UART_STOPBITS_1;
    l_uartHandle.Init.Parity     = UART_PARITY_NONE;
    l_uartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    l_uartHandle.Init.Mode       = UART_MODE_TX_RX;
    l_uartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&l_uartHandle) != HAL_OK) {
        return 0U; /* return failure */
    }

    /* Set UART to receive 1 byte at a time via interrupt */
    HAL_UART_Receive_IT(&l_uartHandle, (uint8_t *)qsRxBuf, 1);
    /* NOTE: wait till QF::onStartup() to enable UART interrupt in NVIC */

    QS_tickPeriod_ = SystemCoreClock / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_; /* to start the timestamp at zero */

    return 1U; /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {  /* NOTE: invoked with interrupts DISABLED */
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) { /* not set? */
        return QS_tickTime_ - (QSTimeCtr)SysTick->VAL;
    }
    else { /* the rollover occured, but the SysTick_ISR did not run yet */
        return QS_tickTime_ + QS_tickPeriod_ - (QSTimeCtr)SysTick->VAL;
    }
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;

    while ((b = QS_getByte()) != QS_EOD) { /* while not End-Of-Data... */
        /* while TXE not empty */
        while ((l_uartHandle.Instance->ISR & UART_FLAG_TXE) == 0U) {
        }
        l_uartHandle.Instance->TDR = (b & 0xFFU);  /* put into TDR */
    }
}
/*..........................................................................*/
/*! callback function to reset the target (to be implemented in the BSP) */
void QS_onReset(void) {
    NVIC_SystemReset();
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

    QS_BEGIN_ID(COMMAND_STAT, 0U)/* app-specific record */
        QS_U8(2, cmdId);
        QS_U32(8, param1);
        QS_U32(8, param2);
        QS_U32(8, param3);
    QS_END()

    if (cmdId == 10U) {
        Q_ERROR(); /* for testing of assertion failure */
    }
    else if (cmdId == 11U) {
        assert_failed("QS_onCommand", 123);
    }
}

#endif /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE1:
* The configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY constant from the
* FreeRTOS configuration file specifies the highest ISR priority that
* is disabled by the QF framework. The value is suitable for the
* NVIC_SetPriority() CMSIS function.
*
* Only ISRs prioritized at or below the
* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY level (i.e.,
* with the numerical values of priorities equal or higher than
* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY) are allowed to call any
* QP/FreeRTOS services. These ISRs are "kernel-aware".
*
* Conversely, any ISRs prioritized above the
* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY priority level (i.e., with
* the numerical values of priorities less than
* configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY) are never disabled and are
* not aware of the kernel. Such "kernel-unaware" ISRs cannot call any
* QP/FreeRTOS services. The only mechanism by which a "kernel-unaware" ISR
* can communicate with the QF framework is by triggering a "kernel-aware"
* ISR, which can post/publish events.
*
* For more information, see article "Running the RTOS on a ARM Cortex-M Core"
* http://www.freertos.org/RTOS-Cortex-M3-M4.html
*
* NOTE2:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/

