/*****************************************************************************
* Product: DPP example, EFM32-SLSTK3401A board, preemptive QXK kernel
* Last updated for version 6.7.0
* Last updated on  2019-12-19
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

#include "em_device.h"  /* the device specific header (SiLabs) */
#include "em_cmu.h"     /* Clock Management Unit (SiLabs) */
#include "em_gpio.h"    /* GPIO (SiLabs) */
#include "em_usart.h"   /* USART (SiLabs) */
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
* DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
*/
enum KernelUnawareISRs { /* see NOTE00 */
    USART0_RX_PRIO,
    /* ... */
    MAX_KERNEL_UNAWARE_CMSIS_PRI  /* keep always last */
};
/* "kernel-unaware" interrupts can't overlap "kernel-aware" interrupts */
Q_ASSERT_COMPILE(MAX_KERNEL_UNAWARE_CMSIS_PRI <= QF_AWARE_ISR_CMSIS_PRI);

enum KernelAwareISRs {
    GPIO_EVEN_PRIO = QF_AWARE_ISR_CMSIS_PRI, /* see NOTE00 */
    SYSTICK_PRIO,
    /* ... */
    MAX_KERNEL_AWARE_CMSIS_PRI /* keep always last */
};
/* "kernel-aware" interrupts should not overlap the PendSV priority */
Q_ASSERT_COMPILE(MAX_KERNEL_AWARE_CMSIS_PRI <= (0xFF >>(8-__NVIC_PRIO_BITS)));

/* ISRs defined in this BSP ------------------------------------------------*/
void SysTick_Handler(void);
void GPIO_EVEN_IRQHandler(void);
void USART0_RX_IRQHandler(void);

/* Local-scope objects -----------------------------------------------------*/
#define LED_PORT    gpioPortF
#define LED0_PIN    4
#define LED1_PIN    5

#define PB_PORT     gpioPortF
#define PB0_PIN     6
#define PB1_PIN     7

static uint32_t l_rnd;      /* random seed */

#ifdef Q_SPY

    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    /* QS source IDs */
    static uint8_t const l_SysTick_Handler = 0U;
    static uint8_t const l_GPIO_EVEN_IRQHandler = 0U;
    static USART_TypeDef * const l_USART0 = ((USART_TypeDef *)(0x40010000UL));

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

    /*QF_TICK_X(0U, &l_SysTick_Handler);*/ /* process time events for rate 0 */
    QACTIVE_POST(the_Ticker0, 0, &l_SysTick_Handler); /* post to Ticker0 */

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = ~GPIO->P[PB_PORT].DIN; /* read PB0 and BP1 */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & (1U << PB0_PIN)) != 0U) {  /* debounced PB0 state changed? */
        if ((buttons.depressed & (1U << PB0_PIN)) != 0U) { /* PB0 depressed?*/
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
void GPIO_EVEN_IRQHandler(void) { /* for testing, NOTE03 */
    QXK_ISR_ENTRY(); /* inform QXK about entering an ISR */

/* for testing...
    QACTIVE_POST(AO_Table, Q_NEW(QEvt, TEST_SIG),
                 &l_GPIO_EVEN_IRQHandler);
*/
    QF_PUBLISH(Q_NEW(QEvt, TEST_SIG), /* for testing... */
               &l_GPIO_EVEN_IRQHandler);

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
void USART0_RX_IRQHandler(void) {
    /* while RX FIFO NOT empty */
    while ((l_USART0->STATUS & USART_STATUS_RXDATAV) != 0U) {
        uint32_t b = l_USART0->RXDATA;
        QS_RX_PUT(b);
    }
}
#else
void USART0_RX_IRQHandler(void) {}
#endif

/*..........................................................................*/
void BSP_init(void) {
    /* NOTE: SystemInit() already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    /* NOTE: The VFP (hardware Floating Point) unit is configured by QXK */

    /* enable clock for to the peripherals used by this application... */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO,  true);
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO,  true);

    /* configure the LEDs */
    GPIO_PinModeSet(LED_PORT, LED0_PIN, gpioModePushPull, 0);
    GPIO_PinModeSet(LED_PORT, LED1_PIN, gpioModePushPull, 0);
    GPIO_PinOutClear(LED_PORT, LED0_PIN);
    GPIO_PinOutClear(LED_PORT, LED1_PIN);

    /* configure the Buttons */
    GPIO_PinModeSet(PB_PORT, PB0_PIN, gpioModeInputPull, 1);
    GPIO_PinModeSet(PB_PORT, PB1_PIN, gpioModeInputPull, 1);

    /*... */
    BSP_randomSeed(1234U);

    if (QS_INIT((void *)0) == 0) { /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_OBJ_DICTIONARY(&l_GPIO_EVEN_IRQHandler);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(PAUSED_STAT);
    QS_USR_DICTIONARY(COMMAND_STAT);
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    if (stat[0] == 'e') {
        GPIO->P[LED_PORT].DOUT |=  (1U << LED0_PIN);
    }
    else {
        GPIO->P[LED_PORT].DOUT &=  ~(1U << LED0_PIN);
    }

    QS_BEGIN(PHILO_STAT, AO_Philo[n]) /* application-specific record begin */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    if (paused != 0U) {
        /* for testing the extended threads... */
        static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U};
        QXThread_delayCancel(XT_Test2); /* make sure Test2 is not delayed */
        QACTIVE_POST_X(&XT_Test2->super, &pauseEvt, 1U, (void *)0);

        GPIO->P[LED_PORT].DOUT |=  (1U << LED0_PIN);
    }
    else {
        GPIO->P[LED_PORT].DOUT &= ~(1U << LED0_PIN);
    }

    QS_BEGIN(PAUSED_STAT, (void *)0) /* application-specific record begin */
        QS_U8(1, paused);  /* Paused status */
    QS_END()
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    uint32_t rnd;
    QSchedStatus lockStat;

    /* Some flating point code is to exercise the VFP... */
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    lockStat = QXK_schedLock(N_PHILO); /* lock scheduler up to N_PHILO prio */
    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    rnd = l_rnd * (3U*7U*11U*13U*23U);
    l_rnd = rnd; /* set for the next time */
    QXK_schedUnlock(lockStat); /* unlock the scheduler */

    return (rnd >> 8);
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void BSP_ledOn(void) {
    GPIO->P[LED_PORT].DOUT |=  (1U << LED1_PIN);
}
/*..........................................................................*/
void BSP_ledOff(void) {
    GPIO->P[LED_PORT].DOUT &= ~(1U << LED1_PIN);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}

/*..........................................................................*/
void QF_onStartup(void) {
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* assing all priority bits for preemption-prio. and none to sub-prio. */
    NVIC_SetPriorityGrouping(0U);

    /* set priorities of ALL ISRs used in the system, see NOTE00
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    * Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
    * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
    */
    NVIC_SetPriority(USART0_RX_IRQn, USART0_RX_PRIO);
    NVIC_SetPriority(SysTick_IRQn,   SYSTICK_PRIO);
    NVIC_SetPriority(GPIO_EVEN_IRQn, GPIO_EVEN_PRIO);
    /* ... */

    /* enable IRQs... */
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
#ifdef Q_SPY
    NVIC_EnableIRQ(USART0_RX_IRQn); /* UART0 interrupt used for QS-RX */
#endif
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QXK_onIdle(void) {
    float volatile x;

    /* toggle the User LED on and then off, see NOTE01 */
/*
    QF_INT_DISABLE();
    GPIO->P[LED_PORT].DOUT |=  (1U << LED1_PIN);
    GPIO->P[LED_PORT].DOUT &= ~(1U << LED1_PIN);
    QF_INT_ENABLE();
*/
    /* Some flating point code is to exercise the VFP... */
    x = 1.73205F;
    x = x * 1.73205F;

#ifdef Q_SPY
    QS_rxParse();  /* parse all the received bytes */

    if ((l_USART0->STATUS & USART_STATUS_TXBL) != 0) {  /* is TXE empty? */
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {  /* not End-Of-Data? */
            l_USART0->TXDATA = (b & 0xFFU);  /* put into the DR register */
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
    /* light up both LEDs */
    GPIO->P[LED_PORT].DOUT |= ((1U << LED0_PIN) | (1U << LED1_PIN));
    /* for debugging, hang on in an endless loop until PB1 is pressed... */
    while ((GPIO->P[PB_PORT].DIN & (1U << PB1_PIN)) != 0) {
    }
#endif

    NVIC_SystemReset();
}

/* QS callbacks ============================================================*/
#ifdef Q_SPY
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsTxBuf[2*1024]; /* buffer for QS transmit channel */
    static uint8_t qsRxBuf[100];    /* buffer for QS receive channel */
    static USART_InitAsync_TypeDef init = {
        usartEnable,      /* Enable RX/TX when init completed */
        0,                /* Use current clock for configuring baudrate */
        115200,           /* 115200 bits/s */
        usartOVS16,       /* 16x oversampling */
        usartDatabits8,   /* 8 databits */
        usartNoParity,    /* No parity */
        usartStopbits1,   /* 1 stopbit */
        0,                /* Do not disable majority vote */
        0,                /* Not USART PRS input mode */
        usartPrsRxCh0,    /* PRS channel 0 */
        0,                /* Auto CS functionality enable/disable switch */
        0,                /* Auto CS Hold cycles */
        0                 /* Auto CS Setup cycles */
    };

    (void)arg; /* unused parameter */
    QS_initBuf  (qsTxBuf, sizeof(qsTxBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    /* Enable peripheral clocks */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* To avoid false start, configure output as high */
    GPIO_PinModeSet(gpioPortA, 0, gpioModePushPull, 1); /* TX pin */
    GPIO_PinModeSet(gpioPortA, 1, gpioModeInput, 0);    /* RX pin */

    /* Enable DK RS232/UART switch */
    GPIO_PinModeSet(gpioPortA, 5, gpioModePushPull, 1);
    CMU_ClockEnable(cmuClock_USART0, true);

    /* configure the UART for the desired baud rate, 8-N-1 operation */
    init.enable = usartDisable;
    USART_InitAsync(l_USART0, &init);

    /* enable pins at correct UART/USART location. */
    l_USART0->ROUTEPEN = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
    l_USART0->ROUTELOC0 = (l_USART0->ROUTELOC0 &
                           ~(_USART_ROUTELOC0_TXLOC_MASK
                           | _USART_ROUTELOC0_RXLOC_MASK));

    /* Clear previous RX interrupts */
    USART_IntClear(l_USART0, USART_IF_RXDATAV);
    NVIC_ClearPendingIRQ(USART0_RX_IRQn);

    /* Enable RX interrupts */
    USART_IntEnable(l_USART0, USART_IF_RXDATAV);
    /* NOTE: do not enable the UART0 interrupt in the NVIC yet.
    * Wait till QF_onStartup()
    */

    /* Finally enable the UART */
    USART_Enable(l_USART0, usartEnable);

    QS_tickPeriod_ = SystemCoreClock / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_; /* to start the timestamp at zero */

    /* setup the QS filters... */
    QS_FILTER_ON(QS_SM_RECORDS); /* state machine records */
    QS_FILTER_ON(QS_UA_RECORDS); /* all usedr records */
    /*
    QS_FILTER_ON(QS_MUTEX_LOCK);
    QS_FILTER_ON(QS_MUTEX_UNLOCK);
    */
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
        while ((l_USART0->STATUS & USART_STATUS_TXBL) == 0U) {
        }
        l_USART0->TXDATA  = (b & 0xFFU);  /* put into the DR register */
        QF_INT_DISABLE();
    }
    QF_INT_ENABLE();
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
    QS_BEGIN(COMMAND_STAT, (void *)1) /* application-specific record begin */
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
* NOTE00:
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
* NOTE01:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*
* NOTE03:
* GPIO_EVEN_IRQHandler() is for testing various preemption scenarios in QXK.
* The general testing strategy is to trigger this IRQ manually from the
* debugger. To do so in IAR, you need to:
* 1. open the Register view
* 2. open NVIC registers
* 3. scroll down to NVIC_ISPR0 register
* 4. write 0x200 to NVIC_ISPR0.SETPEND register
*/
