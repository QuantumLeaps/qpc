/*****************************************************************************
* Product: DPP example, EK-TM4C123GXL board, preemptive QXK kernel
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
#include "dpp.h"
#include "bsp.h"

#include "TM4C123GH6PM.h"        /* the device specific header (TI) */
#include "rom.h"                 /* the built-in ROM functions (TI) */
#include "sysctl.h"              /* system control driver (TI) */
#include "gpio.h"                /* GPIO driver (TI) */
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE  /* define the name of this file for assertions */

/* prototypes of ISRs defined in this BSP ----------------------------------*/
void SysTick_Handler(void);
void GPIOPortA_IRQHandler(void);
void UART0_IRQHandler(void);

/* Local-scope objects -----------------------------------------------------*/
#define LED_RED     (1U << 1)
#define LED_GREEN   (1U << 3)
#define LED_BLUE    (1U << 2)

#define BTN_SW1     (1U << 4)
#define BTN_SW2     (1U << 0)

static uint32_t l_rnd; /* random seed */

#ifdef Q_SPY

    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    /* QS identifiers for non-QP sources of events */
    static uint8_t const l_SysTick_Handler = 0U;
    static uint8_t const l_GPIOPortA_IRQHandler = 0U;

    #define UART_BAUD_RATE      115200U
    #define UART_FR_TXFE        (1U << 7)
    #define UART_FR_RXFE        (1U << 4)
    #define UART_TXFIFO_DEPTH   16U

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER,
        PAUSED_STAT,
        COMMAND_STAT
    };

#endif

/*..........................................................................*/
void SysTick_Handler(void) {
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { ~0U, ~0U };
    uint32_t current;
    uint32_t tmp;

    QXK_ISR_ENTRY();  /* inform QXK about entering an ISR */

#ifdef Q_SPY
    {
        tmp = SysTick->CTRL; /* clear SysTick_CTRL_COUNTFLAG */
        QS_tickTime_ += QS_tickPeriod_; /* account for the clock rollover */
    }
#endif

    //QF_TICK_X(0U, &l_SysTick_Handler); /* process time events for rate 0 */
    QACTIVE_POST(the_Ticker0, 0, &l_SysTick_Handler); // post to Ticker0 */

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = ~GPIOF->DATA_Bits[BTN_SW1 | BTN_SW2]; /* read SW1 and SW2 */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & BTN_SW1) != 0U) {  /* debounced SW1 state changed? */
        if ((buttons.depressed & BTN_SW1) != 0U) { /* is SW1 depressed? */
            static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U};
            QF_PUBLISH(&pauseEvt, &l_SysTick_Handler);
        }
        else {            /* the button is released */
            static QEvt const serveEvt = { SERVE_SIG, 0U, 0U};
            QF_PUBLISH(&serveEvt, &l_SysTick_Handler);
        }
    }

    QXK_ISR_EXIT(); /* inform QXK about exiting an ISR */
}
/*..........................................................................*/
void GPIOPortA_IRQHandler(void) {
    QXK_ISR_ENTRY(); /* inform QXK about entering an ISR */

    //QACTIVE_POST(AO_Table, Q_NEW(QEvt, MAX_PUB_SIG), /* for testing... */
    //             &l_GPIOPortA_IRQHandler);
    QF_PUBLISH(Q_NEW(QEvt, TEST_SIG), /* for testing... */
               &l_GPIOPortA_IRQHandler);

    QXK_ISR_EXIT();  /* inform QXK about exiting an ISR */
}
/*..........................................................................*/
#ifdef Q_SPY
/*
* ISR for receiving bytes from the QSPY Back-End
* NOTE: This ISR is "QF-unaware" meaning that it does not interact with
* the QF/QXK and is not disabled. Such ISRs don't need to call QXK_ISR_ENTRY/
* QXK_ISR_EXIT and they cannot post or publish events.
*/
void UART0_IRQHandler(void) {
    uint32_t status = UART0->RIS; /* get the raw interrupt status */
    UART0->ICR = status;          /* clear the asserted interrupts */

    while ((UART0->FR & UART_FR_RXFE) == 0) { /* while RX FIFO NOT empty */
        uint32_t b = UART0->DR;
        QS_RX_PUT(b);
    }
}
#else
void UART0_IRQHandler(void) {}
#endif

/*..........................................................................*/
void BSP_init(void) {
    /* NOTE: SystemInit() has been already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    /* NOTE: The VFP (hardware Floating Point) unit is configured by QXK */

    /* enable clock for to the peripherals used by this application... */
    SYSCTL->RCGCGPIO |= (1U << 5); /* enable Run mode for GPIOF */

    /* configure the LEDs and push buttons */
    GPIOF->DIR |= (LED_RED | LED_GREEN | LED_BLUE); /* set as output */
    GPIOF->DEN |= (LED_RED | LED_GREEN | LED_BLUE); /* digital enable */
    GPIOF->DATA_Bits[LED_RED]   = 0U; /* turn the LED off */
    GPIOF->DATA_Bits[LED_GREEN] = 0U; /* turn the LED off */
    GPIOF->DATA_Bits[LED_BLUE]  = 0U; /* turn the LED off */

    /* configure the User Switches */
    GPIOF->DIR &= ~(BTN_SW1 | BTN_SW2); /*  set direction: input */
    ROM_GPIOPadConfigSet(GPIOF_BASE, (BTN_SW1 | BTN_SW2),
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    BSP_randomSeed(1234U);

    /* initialize the QS software tracing... */
    if (QS_INIT((void *)0) == 0U) {
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_OBJ_DICTIONARY(&l_GPIOPortA_IRQHandler);
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
    (void)n; /* unused parameter */
    GPIOF->DATA_Bits[LED_GREEN] =
         ((stat[0] == 'e')   /* Is Philo[n] eating? */
         ? 0xFFU             /* turn the LED1 on  */
         : 0U);              /* turn the LED1 off */

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    if (paused != 0U) {
        static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U};

        GPIOF->DATA_Bits[LED_GREEN] = 0xFFU;

        /* for testing the extended threads... */
        QXThread_delayCancel(XT_Test2); /* make sure Test2 is not delayed */
        QACTIVE_POST_X(&XT_Test2->super, &pauseEvt, 1U, (void *)0);
    }
    else {
        GPIOF->DATA_Bits[LED_GREEN] = 0x0U;
    }

    QS_BEGIN_ID(PAUSED_STAT, 0U) /* app-specific record */
        QS_U8(1, paused);  /* Paused status */
    QS_END()
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    uint32_t rnd = 0x100;
    QSchedStatus lockStat;
    QSchedStatus lockStat1;

    /* Some flating point code is to exercise the VFP... */
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    lockStat = QXK_schedLock(N_PHILO); /* lock scheduler around shared seed */

    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    rnd = l_rnd * (3U*7U*11U*13U*23U);
    lockStat1 = QXK_schedLock(N_PHILO + 1); /* nested lock */
    l_rnd = rnd; /* set for the next time */
    QXK_schedUnlock(lockStat1); /* unlock the scheduler */
    QXK_schedUnlock(lockStat); /* unlock the scheduler */

    return (rnd >> 8);
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void BSP_ledOn(void) {
    GPIOF->DATA_Bits[LED_RED] = 0xFFU;
}
/*..........................................................................*/
void BSP_ledOff(void) {
    GPIOF->DATA_Bits[LED_RED] = 0x00U;
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}

/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* assing all priority bits for preemption-prio. and none to sub-prio. */
    NVIC_SetPriorityGrouping(0U);

    /* set priorities of ALL ISRs used in the system, see NOTE1
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    * Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
    * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
    */
    NVIC_SetPriority(UART0_IRQn,     0U); /* kernel unaware interrupt */
    NVIC_SetPriority(GPIOA_IRQn,     QF_AWARE_ISR_CMSIS_PRI);
    NVIC_SetPriority(SysTick_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 1U);
    /* ... */

    /* enable IRQs... */
    NVIC_EnableIRQ(GPIOA_IRQn);

#ifdef Q_SPY
    NVIC_EnableIRQ(UART0_IRQn);  /* UART0 interrupt used for QS-RX */
#endif
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QXK_onIdle(void) {
    float volatile x;

    /* toggle the User LED on and then off, see NOTE2 */
    QF_INT_DISABLE();
    GPIOF->DATA_Bits[LED_BLUE] = 0xFFU;  /* turn the Blue LED on  */
    GPIOF->DATA_Bits[LED_BLUE] = 0U;     /* turn the Blue LED off */
    QF_INT_ENABLE();

    /* Some flating point code is to exercise the VFP... */
    x = 1.73205F;
    x = x * 1.73205F;

#ifdef Q_SPY
    QS_rxParse();  /* parse all the received bytes */

    if ((UART0->FR & UART_FR_TXFE) != 0U) {  /* TX done? */
        uint16_t fifo = UART_TXFIFO_DEPTH;   /* max bytes we can accept */
        uint8_t const *block;

        QF_INT_DISABLE();
        block = QS_getBlock(&fifo);  /* try to get next block to transmit */
        QF_INT_ENABLE();

        while (fifo-- != 0) {  /* any bytes in the block? */
            UART0->DR = *block++;  /* put into the FIFO */
        }
    }
#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M3 MCU.
    */
    __WFI(); /* Wait-For-Interrupt */
#endif
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
    /* light up all LEDs */
    GPIOF->DATA_Bits[LED_GREEN | LED_RED | LED_BLUE] = 0xFFU;
    /* for debugging, hang on in an endless loop... */
    for (;;) {
    }
#else
    NVIC_SystemReset();
#endif
}

/* QS callbacks ============================================================*/
#ifdef Q_SPY
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    (void)arg; /* unused parameter */

    static uint8_t qsTxBuf[2*1024]; /* buffer for QS transmit channel */
    static uint8_t qsRxBuf[100];    /* buffer for QS receive channel */
    uint32_t tmp;

    QS_initBuf  (qsTxBuf, sizeof(qsTxBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    /* enable clock for UART0 and GPIOA (used by UART0 pins) */
    SYSCTL->RCGCUART |= (1U << 0); /* enable Run mode for UART0 */
    SYSCTL->RCGCGPIO |= (1U << 0); /* enable Run mode for GPIOA */

    /* configure UART0 pins for UART operation */
    tmp = (1U << 0) | (1U << 1);
    GPIOA->DIR   &= ~tmp;
    GPIOA->SLR   &= ~tmp;
    GPIOA->ODR   &= ~tmp;
    GPIOA->PUR   &= ~tmp;
    GPIOA->PDR   &= ~tmp;
    GPIOA->AMSEL &= ~tmp;  /* disable analog function on the pins */
    GPIOA->AFSEL |= tmp;   /* enable ALT function on the pins */
    GPIOA->DEN   |= tmp;   /* enable digital I/O on the pins */
    GPIOA->PCTL  &= ~0x00U;
    GPIOA->PCTL  |= 0x11U;

    /* configure the UART for the desired baud rate, 8-N-1 operation */
    tmp = (((SystemCoreClock * 8U) / UART_BAUD_RATE) + 1U) / 2U;
    UART0->IBRD   = tmp / 64U;
    UART0->FBRD   = tmp % 64U;
    UART0->LCRH   = (0x3U << 5); /* configure 8-N-1 operation */
    UART0->LCRH  |= (0x1U << 4); /* enable FIFOs */
    UART0->CTL    = (1U << 0)    /* UART enable */
                    | (1U << 8)  /* UART TX enable */
                    | (1U << 9); /* UART RX enable */

    /* configure UART interrupts (for the RX channel) */
    UART0->IM   |= (1U << 4) | (1U << 6); /* enable RX and RX-TO interrupt */
    UART0->IFLS |= (0x2U << 2);    /* interrupt on RX FIFO half-full */
    /* NOTE: do not enable the UART0 interrupt yet. Wait till QF_onStartup() */

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
    uint16_t fifo = UART_TXFIFO_DEPTH; /* Tx FIFO depth */
    uint8_t const *block;
    while ((block = QS_getBlock(&fifo)) != (uint8_t *)0) {
        /* busy-wait as long as TX FIFO has data to transmit */
        while ((UART0->FR & UART_FR_TXFE) == 0) {
        }

        while (fifo-- != 0U) {    /* any bytes in the block? */
            UART0->DR = *block++; /* put into the TX FIFO */
        }
        fifo = UART_TXFIFO_DEPTH; /* re-load the Tx FIFO depth */
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
    void assert_failed(char const *module, int loc);
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;
    QS_BEGIN_ID(COMMAND_STAT, 0U) /* app-specific record */
        QS_U8(2, cmdId);
        QS_U32(8, param1);
        QS_U32(8, param2);
        QS_U32(8, param3);
    QS_END()

    if (cmdId == 10U) {
        Q_ERROR();
    }
    else if (cmdId == 11U) {
        assert_failed("QS_onCommand", 123);
    }
}

#endif /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE1:
* The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
* ISR priority that is disabled by the QF framework. The value is suitable
* for the NVIC_SetPriority() CMSIS function.
*
* Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
* with the numerical values of priorities equal or higher than
* QF_AWARE_ISR_CMSIS_PRI) are allowed to call the QXK_ISR_ENTRY/QXK_ISR_EXIT
* macros or any other QF/QXK  services. These ISRs are "QF-aware".
*
* Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
* level (i.e., with the numerical values of priorities less than
* QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
* Such "QF-unaware" ISRs cannot call any QF/QXK services. In particular they
* can NOT call the macros QXK_ISR_ENTRY/QXK_ISR_ENTRY. The only mechanism
* by which a "QF-unaware" ISR can communicate with the QF framework is by
* triggering a "QF-aware" ISR, which can post/publish events.
*
* NOTE2:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
