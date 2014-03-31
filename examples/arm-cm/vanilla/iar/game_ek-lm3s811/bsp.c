/*****************************************************************************
* Product: "Fly 'n' Shoot" game example, cooperative Vanilla kernel
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 25, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "game.h"
#include "bsp.h"

#include "lm3s_cmsis.h"
#include "display96x16x1.h"

Q_DEFINE_THIS_FILE

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
* DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
*/
enum KernelUnawareISRs {                                      /* see NOTE00 */
    /* ... */
    MAX_KERNEL_UNAWARE_CMSIS_PRI                        /* keep always last */
};
/* "kernel-unaware" interrupts can't overlap "kernel-aware" interrupts */
Q_ASSERT_COMPILE(MAX_KERNEL_UNAWARE_CMSIS_PRI <= QF_AWARE_ISR_CMSIS_PRI);

enum KernelAwareISRs {
    GPIOPORTA_PRIO = QF_AWARE_ISR_CMSIS_PRI,                  /* see NOTE00 */
    ADCSEQ3_PRIO,
    SYSTICK_PRIO,
    /* ... */
    MAX_KERNEL_AWARE_CMSIS_PRI                          /* keep always last */
};
/* "kernel-aware" interrupts should not overlap the PendSV priority */
Q_ASSERT_COMPILE(MAX_KERNEL_AWARE_CMSIS_PRI <= (0xFF >>(8-__NVIC_PRIO_BITS)));

/* ISRs defined in this BSP ------------------------------------------------*/
void SysTick_Handler(void);
void GPIOPortA_IRQHandler(void);

/* Local-scope objects -----------------------------------------------------*/
#define PUSH_BUTTON             (1U << 4)
#define USER_LED                (1U << 5)

#define ADC_TRIGGER_TIMER       0x00000005U
#define ADC_CTL_IE              0x00000040U
#define ADC_CTL_END             0x00000020U
#define ADC_CTL_CH0             0x00000000U
#define ADC_SSFSTAT0_EMPTY      0x00000100U
#define UART_FR_TXFE            0x00000080U

#ifdef Q_SPY

    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;
    static uint8_t l_SysTick_Handler;
    static uint8_t l_ADCSeq3_IRQHandler;
    static uint8_t l_GPIOPortA_IRQHandler;

    #define UART_BAUD_RATE      115200U
    #define UART_TXFIFO_DEPTH   16U
    #define UART_FR_TXFE        0x00000080U

#endif

/* prototypes of ISRs defined in the BSP....................................*/
void SysTick_Handler(void);
void ADCSeq3_IRQHandler(void);
void assert_failed(char const *file, int line);

/*..........................................................................*/
void SysTick_Handler(void) {
    static QEvt const tickEvt = { TIME_TICK_SIG, 0U, 0U };

#ifdef Q_SPY
    {
        uint32_t dummy = SysTick->CTRL;     /* clear SysTick_CTRL_COUNTFLAG */
        QS_tickTime_ += QS_tickPeriod_;   /* account for the clock rollover */
    }
#endif

    QF_TICK_X(0U, &l_SysTick_Handler); /* process all time events at rate 0 */
    QF_PUBLISH(&tickEvt, &l_SysTick_Handler); /* publish to all subscribers */
}
/*..........................................................................*/
void ADCSeq3_IRQHandler(void) {
    static uint32_t adcLPS = 0U;           /* Low-Pass-Filtered ADC reading */
    static uint32_t wheel  = 0U;                 /* the last wheel position */

    static uint32_t btn_debounced  = 0U;
    static uint8_t  debounce_state = 0U;

    uint32_t tmp;

    ADC->ISC = (1U << 3);                    /* clear the ADCSeq3 interrupt */
                              /* the ADC Sequence 3 FIFO must have a sample */
    Q_ASSERT((ADC->SSFSTAT3 & ADC_SSFSTAT0_EMPTY) == 0);
    tmp = ADC->SSFIFO3;                       /* read the data from the ADC */

    /* 1st order low-pass filter: time constant ~= 2^n samples
     * TF = (1/2^n)/(z-((2^n - 1)/2^n)),
     * eg, n=3, y(k+1) = y(k) - y(k)/8 + x(k)/8 => y += (x - y)/8
     */
    adcLPS += (((int)tmp - (int)adcLPS + 4) >> 3);

    /* compute the next position of the wheel */
    tmp = (((1 << 10) - adcLPS)*(BSP_SCREEN_HEIGHT - 2)) >> 10;

    if (tmp != wheel) {                   /* did the wheel position change? */
        ObjectPosEvt *ope = Q_NEW(ObjectPosEvt, PLAYER_SHIP_MOVE_SIG);
        ope->x = (uint8_t)GAME_SHIP_X;               /* x-position is fixed */
        ope->y = (uint8_t)tmp;
        QACTIVE_POST(AO_Ship, (QEvt *)ope, &l_ADCSeq3_IRQHandler);
        wheel = tmp;                 /* save the last position of the wheel */
    }

    tmp = GPIOC->DATA_Bits[PUSH_BUTTON];               /* read the push btn */
    switch (debounce_state) {
        case 0:
            if (tmp != btn_debounced) {
                debounce_state = 1U;        /* transition to the next state */
            }
            break;
        case 1:
            if (tmp != btn_debounced) {
                debounce_state = 2U;        /* transition to the next state */
            }
            else {
                debounce_state = 0U;          /* transition back to state 0 */
            }
            break;
        case 2:
            if (tmp != btn_debounced) {
                debounce_state = 3U;        /* transition to the next state */
            }
            else {
                debounce_state = 0U;          /* transition back to state 0 */
            }
            break;
        case 3:
            if (tmp != btn_debounced) {
                btn_debounced = tmp;     /* save the debounced button value */

                if (tmp == 0U) {                /* is the button depressed? */
                    static QEvt const fireEvt = { PLAYER_TRIGGER_SIG, 0 };
                    QF_PUBLISH(&fireEvt, &l_ADCSeq3_IRQHandler);
                }
            }
            debounce_state = 0U;              /* transition back to state 0 */
            break;
    }
}
/*..........................................................................*/
void GPIOPortA_IRQHandler(void) {
    QACTIVE_POST(AO_Tunnel, Q_NEW(QEvt, MAX_PUB_SIG),     /* for testing... */
                 &l_GPIOPortA_IRQHandler);
}

/*..........................................................................*/
void BSP_init(void) {
    /* set the system clock as specified in lm3s_config.h (20MHz from PLL)  */
    SystemInit();

    /* enable clock to the peripherals used by the application */
    SYSCTL->RCGC0 |= (1 << 16);               /* enable clock to ADC        */
    SYSCTL->RCGC1 |= (1 << 16) | (1 << 17);   /* enable clock to TIMER0 & 1 */
    SYSCTL->RCGC2 |= (1 <<  0) | (1 <<  2);   /* enable clock to GPIOA & C  */
    __NOP();                                  /* wait after enabling clocks */
    __NOP();
    __NOP();

    /* Configure the ADC Sequence 3 to sample the potentiometer when the
    * timer expires. Set the sequence priority to 0 (highest).
    */
    ADC->EMUX   = (ADC->EMUX   & ~(0xF << (3*4)))
                  | (ADC_TRIGGER_TIMER << (3*4));
    ADC->SSPRI  = (ADC->SSPRI  & ~(0xF << (3*4)))
                  | (0 << (3*4));
    /* set ADC Sequence 3 step to 0 */
    ADC->SSMUX3 = (ADC->SSMUX3 & ~(0xF << (0*4)))
                  | ((ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END) << (0*4));
    ADC->SSCTL3 = (ADC->SSCTL3 & ~(0xF << (0*4)))
                  | (((ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END) >> 4) <<(0*4));
    ADC->ACTSS |= (1 << 3);

    /* configure TIMER1 to trigger the ADC to sample the potentiometer. */
    TIMER1->CTL  &= ~((1 << 0) | (1 << 16));
    TIMER1->CFG   = 0;
    TIMER1->TAMR  = 0x02;
    TIMER1->TAILR = SystemFrequency / 120;
    TIMER1->CTL  |= 0x02;
    TIMER1->CTL  |= 0x20;

    /* configure the LED and push button */
    GPIOC->DIR |= USER_LED;                        /* set direction: output */
    GPIOC->DEN |= USER_LED;                               /* digital enable */
    GPIOC->DATA_Bits[USER_LED] = 0;                /* turn the User LED off */

    GPIOC->DIR &= ~PUSH_BUTTON;                    /*  set direction: input */
    GPIOC->DEN |= PUSH_BUTTON;                            /* digital enable */

    Display96x16x1Init(1);                   /* initialize the OLED display */

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_OBJ_DICTIONARY(&l_ADCSeq3_IRQHandler);
}
/*..........................................................................*/
void BSP_drawBitmap(uint8_t const *bitmap) {
    Display96x16x1ImageDraw(bitmap, 0, 0,
                            BSP_SCREEN_WIDTH, (BSP_SCREEN_HEIGHT >> 3));
}
/*..........................................................................*/
void BSP_drawBitmapXY(uint8_t const *bitmap, uint8_t x, uint8_t y) {
    Display96x16x1ImageDraw(bitmap, x, y,
                            BSP_SCREEN_WIDTH, (BSP_SCREEN_HEIGHT >> 3));
}
/*..........................................................................*/
void BSP_drawNString(uint8_t x, uint8_t y, char const *str) {
    Display96x16x1StringDraw(str, x, y);
}
/*..........................................................................*/
void BSP_updateScore(uint16_t score) {
    /* no room on the OLED display of the EV-LM3S811 board for the score */
}
/*..........................................................................*/
void BSP_displayOn(void) {
    Display96x16x1DisplayOn();
}
/*..........................................................................*/
void BSP_displayOff(void) {
    Display96x16x1DisplayOff();
}

/*..........................................................................*/
void QF_onStartup(void) {
              /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(SystemFrequency / BSP_TICKS_PER_SEC);

    /* assing all priority bits for preemption-prio. and none to sub-prio. */
    NVIC_SetPriorityGrouping(0U);

    /* set priorities of ALL ISRs used in the system, see NOTE00
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    * Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
    * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
    */
    NVIC_SetPriority(ADCSeq3_IRQn,   ADCSEQ3_PRIO);
    NVIC_SetPriority(SysTick_IRQn,   SYSTICK_PRIO);
    /* ... */

                                                          /* enable IRQs... */
    NVIC_EnableIRQ(ADCSeq3_IRQn);
    NVIC_EnableIRQ(GPIOPortA_IRQn);

    ADC->ISC = (1 << 3);
    ADC->IM |= (1 << 3);

    TIMER1->CTL |= ((1 << 0) | (1 << 16));                 /* enable TIMER1 */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QF_onIdle(void) {       /* called with interrupts disabled, see NOTE01 */

    /* toggle the User LED on and then off, see NOTE02 */
    GPIOC->DATA_Bits[USER_LED] = USER_LED;         /* turn the User LED on  */
    GPIOC->DATA_Bits[USER_LED] = 0;                /* turn the User LED off */

#ifdef Q_SPY
    QF_INT_ENABLE();
    if ((UART0->FR & UART_FR_TXFE) != 0) {                      /* TX done? */
        uint16_t fifo = UART_TXFIFO_DEPTH;       /* max bytes we can accept */
        uint8_t const *block;

        QF_INT_DISABLE();
        block = QS_getBlock(&fifo);    /* try to get next block to transmit */
        QF_INT_ENABLE();

        while (fifo-- != 0) {                    /* any bytes in the block? */
            UART0->DR = *block++;                      /* put into the FIFO */
        }
    }
#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    QF_CPU_SLEEP();         /* atomically go to sleep and enable interrupts */
#else
    QF_INT_ENABLE();                              /* just enable interrupts */
#endif
}

/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const file, int_t line) {
    assert_failed(file, line);
}
/*..........................................................................*/
/* error routine that is called if the CMSIS library encounters an error    */
void assert_failed(char const *file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_DISABLE();         /* make sure that all interrupts are disabled */
    NVIC_SystemReset();                             /* perform system reset */
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[6*256];                  /* buffer for Quantum Spy */
    uint32_t tmp;
    QS_initBuf(qsBuf, sizeof(qsBuf));

                                /* enable the peripherals used by the UART0 */
    SYSCTL->RCGC1 |= (1 << 0);                     /* enable clock to UART0 */
    SYSCTL->RCGC2 |= (1 << 0);                     /* enable clock to GPIOA */
    __NOP();                                  /* wait after enabling clocks */
    __NOP();
    __NOP();

                                 /* configure UART0 pins for UART operation */
    tmp = (1 << 0) | (1 << 1);
    GPIOA->DIR   &= ~tmp;
    GPIOA->AFSEL |= tmp;
    GPIOA->DR2R  |= tmp;        /* set 2mA drive, DR4R and DR8R are cleared */
    GPIOA->SLR   &= ~tmp;
    GPIOA->ODR   &= ~tmp;
    GPIOA->PUR   &= ~tmp;
    GPIOA->PDR   &= ~tmp;
    GPIOA->DEN   |= tmp;

           /* configure the UART for the desired baud rate, 8-N-1 operation */
    tmp = (((SystemFrequency * 8) / UART_BAUD_RATE) + 1) / 2;
    UART0->IBRD   = tmp / 64;
    UART0->FBRD   = tmp % 64;
    UART0->LCRH   = 0x60;                      /* configure 8-N-1 operation */
    UART0->LCRH  |= 0x10;
    UART0->CTL   |= (1 << 0) | (1 << 8) | (1 << 9);

    QS_tickPeriod_ = SystemFrequency / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_;        /* to start the timestamp at zero */

                                                 /* setup the QS filters... */
    QS_FILTER_ON(QS_ALL_RECORDS);

//    QS_FILTER_OFF(QS_QEP_STATE_ENTRY);
//    QS_FILTER_OFF(QS_QEP_STATE_EXIT);
//    QS_FILTER_OFF(QS_QEP_STATE_INIT);
//    QS_FILTER_OFF(QS_QEP_TRAN_HIST);
//    QS_FILTER_OFF(QS_QEP_INTERN_TRAN);
//    QS_FILTER_OFF(QS_QEP_TRAN);
//    QS_FILTER_OFF(QS_QEP_IGNORED);
//    QS_FILTER_OFF(QS_QEP_DISPATCH);
//    QS_FILTER_OFF(QS_QEP_UNHANDLED);

    QS_FILTER_OFF(QS_QF_ACTIVE_ADD);
    QS_FILTER_OFF(QS_QF_ACTIVE_REMOVE);
    QS_FILTER_OFF(QS_QF_ACTIVE_SUBSCRIBE);
    QS_FILTER_OFF(QS_QF_ACTIVE_UNSUBSCRIBE);
    QS_FILTER_OFF(QS_QF_ACTIVE_POST_FIFO);
    QS_FILTER_OFF(QS_QF_ACTIVE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET_LAST);
    QS_FILTER_OFF(QS_QF_EQUEUE_INIT);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_FIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET_LAST);
    QS_FILTER_OFF(QS_QF_MPOOL_INIT);
    QS_FILTER_OFF(QS_QF_MPOOL_GET);
    QS_FILTER_OFF(QS_QF_MPOOL_PUT);
    QS_FILTER_OFF(QS_QF_PUBLISH);
    QS_FILTER_OFF(QS_QF_NEW);
    QS_FILTER_OFF(QS_QF_GC_ATTEMPT);
    QS_FILTER_OFF(QS_QF_GC);
//    QS_FILTER_OFF(QS_QF_TICK);
    QS_FILTER_OFF(QS_QF_TIMEEVT_ARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_AUTO_DISARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM_ATTEMPT);
    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_REARM);
    QS_FILTER_OFF(QS_QF_TIMEEVT_POST);
    QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
    QS_FILTER_OFF(QS_QF_CRIT_EXIT);
    QS_FILTER_OFF(QS_QF_ISR_ENTRY);
    QS_FILTER_OFF(QS_QF_ISR_EXIT);

    return (uint8_t)1;                                    /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {            /* invoked with interrupts locked */
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) {    /* not set? */
        return QS_tickTime_ - (QSTimeCtr)SysTick->VAL;
    }
    else {     /* the rollover occured, but the SysTick_ISR did not run yet */
        return QS_tickTime_ + QS_tickPeriod_ - (QSTimeCtr)SysTick->VAL;
    }
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t fifo = UART_TXFIFO_DEPTH;                     /* Tx FIFO depth */
    uint8_t const *block;
    QF_INT_DISABLE();
    while ((block = QS_getBlock(&fifo)) != (uint8_t *)0) {
        QF_INT_ENABLE();
                                           /* busy-wait until TX FIFO empty */
        while ((UART0->FR & UART_FR_TXFE) == 0) {
        }

        while (fifo-- != 0) {                    /* any bytes in the block? */
            UART0->DR = *block++;                   /* put into the TX FIFO */
        }
        fifo = UART_TXFIFO_DEPTH;              /* re-load the Tx FIFO depth */
        QF_INT_DISABLE();
    }
    QF_INT_ENABLE();
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE00:
* The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
* ISR priority that is disabled by the QF framework. The value is suitable
* for the NVIC_SetPriority() CMSIS function.
*
* Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
* with the numerical values of priorities equal or higher than
* QF_AWARE_ISR_CMSIS_PRI) are allowed to call any QF services. These ISRs
* are "QF-aware".
*
* Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
* level (i.e., with the numerical values of priorities less than
* QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
* Such "QF-unaware" ISRs cannot call any QF services. The only mechanism
* by which a "QF-unaware" ISR can communicate with the QF framework is by
* triggering a "QF-aware" ISR, which can post/publish events.
*
* NOTE01:
* The QF_onIdle() callback is called with interrupts disabled, because the
* determination of the idle condition might change by any interrupt posting
* an event. QF::onIdle() must internally enable interrupts, ideally
* atomically with putting the CPU to the power-saving mode.
*
* NOTE02:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
