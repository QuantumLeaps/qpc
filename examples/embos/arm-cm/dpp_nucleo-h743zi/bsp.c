/*****************************************************************************
* Product: "Dining Philosophers Problem" example, embOS kernel
* Last updated for: @ref qpc_7_0_0
* Last updated on  2021-12-21
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
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

/* STM32CubeH7 include files */
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo_144.h"
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE

/* ISRs defined in this BSP ------------------------------------------------*/
void SysTick_Handler(void);
void USART3_IRQHandler(void);

/* Local-scope objects -----------------------------------------------------*/
static uint32_t l_rnd; /* random seed */

#ifdef Q_SPY
    /* event-source identifiers used for tracing */
    static QSpyId const l_embos_ticker = { 0U };

    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    /* QSpy source IDs */
    static QSpyId const l_SysTick_Handler = { 0U };
    static QSpyId const l_EXTI0_IRQHandler = { 0U };

    static UART_HandleTypeDef l_uartHandle;

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER,
        COMMAND_STAT
    };
#endif

/* embOS application hooks =================================================*/
static void tick_handler(void) {  /* signature of embOS tick hook routine */
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { ~0U, ~0U };
    uint32_t current;
    uint32_t tmp;
    static uint_fast8_t ctr = 1U;

#ifdef Q_SPY
    {
        tmp = SysTick->CTRL; /* clear SysTick_CTRL_COUNTFLAG */
        QS_tickTime_ += QS_tickPeriod_; /* account for the clock rollover */
    }
#endif

    /* scale down the 1000Hz embOS tick to the desired BSP_TICKS_PER_SEC */
    if (--ctr == 0U) {
        ctr = 1000U/BSP_TICKS_PER_SEC;
        QTIMEEVT_TICK_X(0U, &l_embos_ticker);

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
                static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U};
                QACTIVE_PUBLISH(&pauseEvt, &l_SysTick_Handler);
            }
            else { /* the button is released */
                static QEvt const serveEvt = { SERVE_SIG, 0U, 0U};
                QACTIVE_PUBLISH(&serveEvt, &l_SysTick_Handler);
            }
        }
    }
}

/*..........................................................................*/
#ifdef Q_SPY
/*
* ISR for receiving bytes from the QSPY Back-End
* NOTE: This ISR is "QF-unaware" meaning that it does not interact with
* the QF/QK and is not disabled. Such ISRs don't need to call QK_ISR_ENTRY/
* QK_ISR_EXIT and they cannot post or publish events.
*/
void USART3_IRQHandler(void) {
    /* is RX register NOT empty? */
    if ((l_uartHandle.Instance->ISR & USART_ISR_RXNE_RXFNE) != 0) {
        uint32_t b = l_uartHandle.Instance->RDR;
        QS_RX_PUT(b);
        l_uartHandle.Instance->ISR &= ~USART_ISR_RXNE_RXFNE; /* clear int. */
    }
}
#endif

/*..........................................................................*/
/*
*  OS_Idle() function overridden from RTOSInit_STM32F4x_CMSIS.c
*
*  Function description
*    This is basically the "core" of the embOS idle loop.
*    This core loop can be changed, but:
*    The idle loop does not have a stack of its own, therefore no
*    functionality should be implemented that relies on the stack
*    to be preserved. However, a simple program loop can be programmed
*    (like toggling an output or incrementing a counter)
*/
void OS_Idle(void) {
    while (1) {

        /* toggle the User LED on and then off, see NOTE2 */
        QF_INT_DISABLE();
        BSP_LED_On (LED3);
        BSP_LED_Off(LED3);
        QF_INT_ENABLE();

#ifdef Q_SPY
        QS_rxParse();  /* parse all the received bytes */

        if ((l_uartHandle.Instance->ISR & UART_FLAG_TXE) != 0U) { /* TXE empty? */
            uint16_t b;

            QF_INT_DISABLE();
            b = QS_getByte();
            QF_INT_ENABLE();

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
#elif defined NDEBUG
        /* Put the CPU and peripherals to the low-power mode.
        * NOTE: You might need to customize the clock management for your
        * application, see the datasheet for your particular Cortex-M3 MCU.
        */
        /* !!!CAUTION!!!
        * The WFI instruction stops the CPU clock, which unfortunately
        * disables the STM32 JTAG port, so the ST-Link debugger can no longer
        * connect to the board. For that reason, the call to __WFI() has to
        * be used with CAUTION. See also NOTE02
        */
#if ((OS_VIEW_IFSELECT != OS_VIEW_IF_JLINK) && (OS_DEBUG == 0))
        //__WFI(); /* Wait-For-Interrupt */
#endif
#endif
    }
}

/* BSP functions ===========================================================*/
void BSP_init(void) {

    /* NOTE: SystemInit() already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    SCB_EnableICache(); /* Enable I-Cache */
    SCB_EnableDCache(); /* Enable D-Cache */

    /* Configure Flash prefetch and Instr. cache through ART accelerator */
#if (ART_ACCLERATOR_ENABLE != 0)
    __HAL_FLASH_ART_ENABLE();
#endif /* ART_ACCLERATOR_ENABLE */

    /* Explictily Disable the automatic FPU state preservation as well as
    * the FPU lazy stacking
    */
    FPU->FPCCR &= ~((1U << FPU_FPCCR_ASPEN_Pos) | (1U << FPU_FPCCR_LSPEN_Pos));

    /* Configure the LEDs */
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* Configure the User Button in GPIO Mode */
    BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

    //...
    BSP_randomSeed(1234U);

    /* initialize the QS software tracing... */
    if (QS_INIT((void *)0) == 0) {
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_OBJ_DICTIONARY(&l_EXTI0_IRQHandler);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(COMMAND_STAT);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_SM_RECORDS);
    QS_GLB_FILTER(QS_UA_RECORDS);
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    (void)n;

    if (stat[0] == 'h') {
        BSP_LED_On(LED1); /* turn LED on  */
    }
    else {
        BSP_LED_Off(LED1); /* turn LED off  */
    }
    if (stat[0] == 'e') {
        BSP_LED_On(LED2); /* turn LED on  */
    }
    else {
        BSP_LED_Off(LED2); /* turn LED on  */
    }

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()          /* application-specific record end */
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    if (paused) {
        BSP_LED_On(LED3); /* turn LED on  */
    }
    else {
        BSP_LED_Off(LED3); /* turn LED on  */
    }
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    uint32_t rnd;

    /* exercise the FPU with some floating point computations */
    /* NOTE: this code can be only called from a task that created with
    * the option OS_TASK_OPT_SAVE_FP.
    */
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    rnd = l_rnd * (3U*7U*11U*13U*23U);
    l_rnd = rnd; /* set for the next time */

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
    static OS_TICK_HOOK tick_hook;
    OS_TICK_AddHook(&tick_hook, &tick_handler);

#ifdef Q_SPY
    NVIC_SetPriority(USART3_IRQn,  0U); /* kernel unaware interrupt */
    NVIC_EnableIRQ(USART3_IRQn); /* UART interrupt used for QS-RX */
#endif
}
/*..........................................................................*/
void QF_onCleanup(void) {
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char const * const module, int_t const loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;
    QS_ASSERTION(module, loc, 10000U); /* report assertion to QS */
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

    l_uartHandle.Instance        = USART3;
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

    QF_INT_DISABLE();
    while ((b = QS_getByte()) != QS_EOD) { /* while not End-Of-Data... */
        QF_INT_ENABLE();
        /* while TXE not empty */
        while ((l_uartHandle.Instance->ISR & UART_FLAG_TXE) == 0U) {
        }
        l_uartHandle.Instance->TDR = (b & 0xFFU);  /* put into TDR */
        QF_INT_DISABLE();
    }
    QF_INT_ENABLE();
}
/*..........................................................................*/
/*! callback function to reset the target (to be implemented in the BSP) */
_Noreturn void QS_onReset(void) {
    NVIC_SystemReset();
}
/*..........................................................................*/
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

/*****************************************************************************
* NOTE01:
* One of the LEDs is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts disabled, so no
* interrupt execution time contributes to the brightness of the LED.
*
* NOTE02:
* If you find your board "frozen" like this, strap BOOT0 to VDD and reset
* the board, then connect with ST-Link Utilities and erase the part. The
* trick with BOOT(0) is it gets the part to run the System Loader instead of
* your broken code. When done disconnect BOOT0, and start over.
*/

