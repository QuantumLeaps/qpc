/*****************************************************************************
* Product: DPP with lwIP application, cooperative QV kernel
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
#include "qpc.h"  /* QP/C header file */
#include "dpp.h"  /* application events and active objects */
#include "bsp.h"  /* Board Support Package header file */

#include "LM3S6965.h" /* the device specific header (TI) */

Q_DEFINE_THIS_FILE

/* ISRs defined in this BSP ------------------------------------------------*/
void SysTick_Handler(void);

/* Local-scope objects -----------------------------------------------------*/
#define USER_LED           (1U << 0)
#define USER_BTN           (1U << 1)

#define ETH0_LED           (1U << 3)
#define ETH1_LED           (1U << 2)

static uint32_t l_nTicks;

#ifdef Q_SPY

    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;
    static uint8_t l_SysTick_Handler;

    #define UART_BAUD_RATE      115200U
    #define UART_TXFIFO_DEPTH   16U
    #define UART_FR_TXFE        (1U << 7)

#endif

/*..........................................................................*/
void SysTick_Handler(void) {
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { ~0U, ~0U };
    uint32_t current;
    uint32_t volatile tmp;

    ++l_nTicks;        /* count the number of clock ticks */

#ifdef Q_SPY
    tmp = SysTick->CTRL; /* clear SysTick_CTRL_COUNTFLAG */
    QS_tickTime_ += QS_tickPeriod_; /* account for the clock rollover */
#endif

    QF_TICK_X(0U, &l_SysTick_Handler); /* process time events for rate 0 */

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = ~GPIOF->DATA_Bits[USER_BTN]; /* read USER_BTN */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & USER_BTN) != 0U) { /* debounced USER_BTN state changed? */
        if ((buttons.depressed & USER_BTN) != 0U) { /* is BTN depressed? */
            static QEvt const bd = { BTN_DOWN_SIG, 0U, 0U };
            QF_PUBLISH(&bd, &l_SysTick_Handler);
        }
        else { /* the button is released */
            static QEvt const bu = { BTN_UP_SIG, 0U, 0U };
            QF_PUBLISH(&bu, &l_SysTick_Handler);
        }
    }
}

/*..........................................................................*/
void BSP_init(void) {
    /* NOTE: SystemInit() already called from startup_TM4C123GH6PM.s
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    SYSCTL->RCGC2 |= (1 << 5); /* enable clock to GPIOF (User and Eth LEDs)*/
    __NOP();
    __NOP();

    /* configure the pin driving the Ethernet LED */
    GPIOF->DIR   &= ~(ETH0_LED | ETH1_LED); /* set direction: hardware */
    GPIOF->AFSEL |=  (ETH0_LED | ETH1_LED);
    GPIOF->DR2R  |=  (ETH0_LED | ETH1_LED);
    GPIOF->ODR   &= ~(ETH0_LED | ETH1_LED);
    GPIOF->PUR   |=  (ETH0_LED | ETH1_LED);
    GPIOF->PDR   &= ~(ETH0_LED | ETH1_LED);
    GPIOF->DEN   |=  (ETH0_LED | ETH1_LED);
    GPIOF->AMSEL &= ~(ETH0_LED | ETH1_LED);

    /* configure the pin driving the User LED */
    GPIOF->DIR   |=  USER_LED;  /* set direction: output */
    GPIOF->DR2R  |=  USER_LED;
    GPIOF->DEN   |=  USER_LED;
    GPIOF->AMSEL &= ~USER_LED;
    GPIOF->DATA_Bits[USER_LED] = 0; /* turn the LED off */

    /* configure the pin connected to the Buttons */
    GPIOF->DIR   &= ~USER_BTN;  /* set direction: input */
    GPIOF->DR2R  |=  USER_BTN;
    GPIOF->ODR   &= ~USER_BTN;
    GPIOF->PUR   |=  USER_BTN;
    GPIOF->PDR   &= ~USER_BTN;
    GPIOF->DEN   |=  USER_BTN;
    GPIOF->AMSEL &= ~USER_BTN;

    /* NOTE:
    * The OLED display is encapsulated inside the Table AO, so the
    * initialization of the OLED display happens in the top-most initial
    * transition of the Table AO (see Table_displayInit()).
    */

    if (QS_INIT((void *)0) == 0) {  /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_SM_RECORDS); /* state machine records */
    QS_GLB_FILTER(QS_AO_RECORDS); /* active object records */
    QS_GLB_FILTER(QS_UA_RECORDS); /* all user records */
}
/*..........................................................................*/
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
    NVIC_SetPriority(Ethernet_IRQn,  QF_AWARE_ISR_CMSIS_PRI);
    NVIC_SetPriority(SysTick_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 1U);
    /* ... */

    NVIC_EnableIRQ(Ethernet_IRQn); /* enable the Ethernet Interrupt */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QV_onIdle(void) { /* NOTE: called with interrutps DISABLED, see NOTE2 */

    /* toggle the User LED on and then off, see NOTE3 */
    QF_INT_DISABLE();
    GPIOF->DATA_Bits[USER_LED] = USER_LED; /* turn the User LED on  */
    GPIOF->DATA_Bits[USER_LED] = 0;        /* turn the User LED off */
    QF_INT_ENABLE();

#ifdef Q_SPY
    if ((UART0->FR & UART_FR_TXFE) != 0) { /* TX done? */
        uint16_t fifo = UART_TXFIFO_DEPTH; /* max bytes we can accept */
        uint8_t const *block;

        QF_INT_DISABLE();
        block = QS_getBlock(&fifo); /* try to get next block to transmit */
        QF_INT_ENABLE();

        while (fifo-- != 0) {  /* any bytes in the block? */
            UART0->DR = *block++; /* put into the FIFO */
        }
    }
#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular MCU.
    */
    QV_CPU_SLEEP();  /* atomically go to sleep and enable interrupts */
#else
    QF_INT_ENABLE(); /* just enable interrupts */
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
    NVIC_SystemReset();
}

/*..........................................................................*/
/* sys_now() is used in the lwIP stack
*/
uint32_t sys_now(void) {
    return l_nTicks * (1000 / BSP_TICKS_PER_SEC);
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[6*256]; /* buffer for Quantum Spy */
    uint32_t tmp;
    QS_initBuf(qsBuf, sizeof(qsBuf));

    /* enable the peripherals used by the UART0 */
    SYSCTL->RCGC1 |= (1 << 0);  /* enable clock to UART0 */
    SYSCTL->RCGC2 |= (1 << 0);  /* enable clock to GPIOA */
    __NOP();  /* wait after enabling clocks */
    __NOP();
    __NOP();

    /* configure UART0 pins for UART operation */
    tmp = (1 << 0) | (1 << 1);
    GPIOA->DIR   &= ~tmp;
    GPIOA->AFSEL |=  tmp;
    GPIOA->DR2R  |=  tmp;  /* set 2mA drive, DR4R and DR8R are cleared */
    GPIOA->SLR   &= ~tmp;
    GPIOA->ODR   &= ~tmp;
    GPIOA->PUR   &= ~tmp;
    GPIOA->PDR   &= ~tmp;
    GPIOA->DEN   |=  tmp;
    GPIOA->AMSEL &= ~tmp;

    /* configure the UART for the desired baud rate, 8-N-1 operation */
    tmp = (((SystemCoreClock * 8) / UART_BAUD_RATE) + 1) / 2;
    UART0->IBRD   = tmp / 64;
    UART0->FBRD   = tmp % 64;
    UART0->LCRH   = 0x60;  /* configure 8-bit operation */
    UART0->LCRH  |= 0x10;  /* enable FIFOs */
    UART0->CTL   |= (1 << 0) | (1 << 8) | (1 << 9);

    QS_tickPeriod_ = SystemCoreClock / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_;  /* to start the timestamp at zero */

    return 1U; /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) { /* invoked with interrupts disabled */
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) { /* not set? */
        return QS_tickTime_ - (QSTimeCtr)SysTick->VAL;
    }
    else { /* the rollover occured, but the SysTick_ISR did not run yet */
        return QS_tickTime_ + QS_tickPeriod_ - (QSTimeCtr)SysTick->VAL;
    }
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t fifo = UART_TXFIFO_DEPTH;  /* Tx FIFO depth */
    uint8_t const *block;
    QF_INT_DISABLE();
    while ((block = QS_getBlock(&fifo)) != (uint8_t *)0) {
        QF_INT_ENABLE();

        /* busy-wait until TX FIFO empty */
        while ((UART0->FR & UART_FR_TXFE) == 0) {
        }

        while (fifo-- != 0) {     /* any bytes in the block? */
            UART0->DR = *block++; /* put into the TX FIFO */
        }
        fifo = UART_TXFIFO_DEPTH; /* re-load the Tx FIFO depth */
        QF_INT_DISABLE();
    }
    QF_INT_ENABLE();
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
* QF_AWARE_ISR_CMSIS_PRI) are allowed to call the QK_ISR_ENTRY/QK_ISR_ENTRY
* macros or any other QF/QK  services. These ISRs are "QF-aware".
*
* Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
* level (i.e., with the numerical values of priorities less than
* QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
* Such "QF-unaware" ISRs cannot call any QF/QK services. In particular they
* can NOT call the macros QK_ISR_ENTRY/QK_ISR_ENTRY. The only mechanism
* by which a "QF-unaware" ISR can communicate with the QF framework is by
* triggering a "QF-aware" ISR, which can post/publish events.
*
* NOTE2:
* The QV_onIdle() callback is called with interrupts disabled, because the
* determination of the idle condition might change by any interrupt posting
* an event. QV_onIdle() must internally enable interrupts, ideally
* atomically with putting the CPU to the power-saving mode.
*
* NOTE3:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
