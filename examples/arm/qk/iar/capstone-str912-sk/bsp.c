/*****************************************************************************
* Product: BSP for Capstone scuba diving computer example, STR12-SK, QK
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
#include "capstone.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

/* Local objects -----------------------------------------------------------*/
static u8 const l_gpioPin[] = {
    0,            /* unused */

    GPIO_Pin_0,   /* LED  1 */
    GPIO_Pin_1,   /* LED  2 */
    GPIO_Pin_2,   /* LED  3 */
    GPIO_Pin_3,   /* LED  4 */
    GPIO_Pin_4,   /* LED  5 */
    GPIO_Pin_5,   /* LED  6 */
    GPIO_Pin_6,   /* LED  7 */
    GPIO_Pin_7,   /* LED  8 */

    GPIO_Pin_0,   /* LED  9 */
    GPIO_Pin_1,   /* LED 10 */
    GPIO_Pin_2,   /* LED 11 */
    GPIO_Pin_3,   /* LED 12 */
    GPIO_Pin_4,   /* LED 13 */
    GPIO_Pin_5,   /* LED 14 */
    GPIO_Pin_6,   /* LED 15 */
    GPIO_Pin_7    /* LED 16 */
};
static uint32_t l_nticks;                    /* number of system time ticks */

#define LED_ON(num_)       GPIO_WriteBit(GPIO6, l_gpioPin[num_], Bit_RESET)
#define LED_OFF(num_)      GPIO_WriteBit(GPIO6, l_gpioPin[num_], Bit_SET)
#define LED_ON_ALL()       GPIO_WriteBit(GPIO6, GPIO_Pin_All,    Bit_RESET)
#define LED_OFF_ALL()      GPIO_WriteBit(GPIO6, GPIO_Pin_All,    Bit_SET)

#define LCD_LIGHT_ON()     GPIO_WriteBit(GPIO9, GPIO_Pin_5, Bit_SET)
#define LCD_LIGHT_OFF()    GPIO_WriteBit(GPIO9, GPIO_Pin_5, Bit_RESET)

typedef void (*IntVector)(void);           /* IntVector pointer-to-function */

#define BSP_CPU_CLOCK_HZ   96000000
#define BSP_CPU_PERIPH_HZ  (BSP_CPU_CLOCK_HZ/2)
#define BSP_TIM0_PRESCALER 48

#define BSP_ADC_PRIO       15
#define BSP_TICK_PRIO      14

#define BSP_UART_TX_FIFO   16
#define BSP_QS_BUF_SIZE    (2*1024)
#define BSP_QS_BAUD_RATE   115200

#define BSP_TIM0_PERIOD    \
    (BSP_CPU_PERIPH_HZ / BSP_TIM0_PRESCALER / BSP_TICKS_PER_SEC)

#ifdef Q_SPY
    static uint32_t l_currTime32;
    static uint16_t l_prevTime16;
    static uint16_t const l_TIM0_IRQHandler = 0;
    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };
#endif

/*..........................................................................*/
__arm void BSP_irq(void) {
    IntVector vect = (IntVector)VIC0->VAR;        /* read the VAR from VIC0 */
    IntVector vect1 = (IntVector)VIC1->VAR;       /* read the VAR from VIC1 */

    QF_INT_ENABLE();                            /* allow nesting interrupts */
    (*vect)();              /* call the IRQ ISR via the pointer to function */
    QF_INT_DISABLE();            /* disable interrups for the exit sequence */

    VIC0->VAR = 0;                         /* send End-Of-Interrupt to VIC0 */
    VIC1->VAR = 0;                         /* send End-Of-Interrupt to VIC1 */
}
/*..........................................................................*/
__arm void BSP_fiq(void) {
    /* TBD: implement the FIQ handler directly right here, see NOTE02  */
    /* NOTE: Do NOT enable interrupts throughout the whole FIQ processing. */
    /* NOTE: Do NOT write EOI to the AIC */
}
/* ISRs --------------------------------------------------------------------*/
__arm void TIM0_IRQHandler(void) {
    static uint8_t old_buttons       = 0;
    static uint8_t debounced_buttons = 0;
    uint8_t        new_buttons;
    uint8_t        changed_buttons;
    static QEvt const b1_down = { B1_DOWN_SIG, 0 };
    static QEvt const b1_up   = { B1_UP_SIG,   0 };
    static QEvt const b2_down = { B2_DOWN_SIG, 0 };
    static QEvt const b2_up   = { B2_UP_SIG,   0 };


    TIM0->OC1R += BSP_TIM0_PERIOD - 1;   /* set the output compare register */
    TIM0->SR &= ~TIM_IT_OC1;             /* clear interrupt source (Timer0) */

#ifdef Q_SPY
    {
        uint16_t currTime16 = (uint16_t)TIM0->CNTR;
        l_currTime32 += (currTime16 - l_prevTime16) & 0xFFFF;
        l_prevTime16 = currTime16;
    }
#endif

    QF_TICK(&l_TIM0_IRQHandler);    /* process all arm time events (timers) */

    ++l_nticks;                            /* account for another time tick */

                                                 /* debounce the buttons... */
    new_buttons = (uint8_t)(~(GPIO_ReadBit(GPIO7, GPIO_Pin_5)
                              | (GPIO_ReadBit(GPIO7, GPIO_Pin_6) << 1)));
    changed_buttons    = debounced_buttons;
    debounced_buttons |= (old_buttons & new_buttons);        /* new presses */
    debounced_buttons &= (old_buttons | new_buttons);       /* new releases */
    changed_buttons   ^= debounced_buttons;      /* change in the debounced */
    old_buttons        = new_buttons;                 /* save for next time */

    if ((changed_buttons & (1 << 0)) != 0) {                 /* B1 changed? */
        if ((new_buttons & (1 << 0)) != 0) {
            QF_PUBLISH(&b1_down, &l_TIM0_IRQHandler);
        }
        else {
            QF_PUBLISH(&b1_up, &l_TIM0_IRQHandler);
        }
    }
    if ((changed_buttons & (1 << 1)) != 0) {                 /* B2 changed? */
        if ((new_buttons & (1 << 1)) != 0) {
            QF_PUBLISH(&b2_down, &l_TIM0_IRQHandler);
        }
        else {
            QF_PUBLISH(&b2_up, &l_TIM0_IRQHandler);
        }
    }

}
/*..........................................................................*/
__arm void ADC_IRQHandler(void) {
    ADCEvt *e  = Q_NEW(ADCEvt, ASCENT_RATE_ADC_SIG);
    e->raw = ADC_GetConversionValue(ADC_Channel_5);
    QActive_postLIFO(AO_Capstone, (QEvt *)e);

    ADC_ClearFlag(ADC_FLAG_ECV);        /* reset the End of Conversion flag */
    ADC_StandbyModeCmd(ENABLE);                         /* put ADC to sleep */
}
/*..........................................................................*/
static __arm void ISR_spur(void) {
}
/*..........................................................................*/
__arm void QF_onStartup(void) {
    TIM_CounterCmd(TIM0, TIM_START);
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QK_init(void) {
}
/*..........................................................................*/
__arm void QK_onIdle(void) {

    QF_INT_DISABLE();
    LED_ON(16);
    LED_OFF(16);
    QF_INT_ENABLE();

#ifdef Q_SPY                     /* use the idle cycles for QS transmission */

    if ((UART0->FR & 0x80) != 0) {                        /* TX FIFO empty? */
        uint16_t nBytes = BSP_UART_TX_FIFO;      /* capacity of the TX FIFO */
        uint8_t const *block;

        QF_INT_DISABLE();
        block = QS_getBlock(&nBytes);     /* get the data block to transfer */
        QF_INT_ENABLE();

        while (nBytes-- != 0) {
            UART0->DR = *block++;          /* stick the byte to the TX FIFO */
        }
    }

#elif defined NDEBUG /* only if not debugging (idle mode hinders debugging) */
    /* shut down unused peripherals to save power ...*/
//    SCU_EnterIdleMode();         /* Power-Management: disable the CPU clock */
// NOTE: idle or sleep mode hangs the J-TAG, it's difficult to
// get control of the MCU again!!!
    /* NOTE: an interrupt starts the CPU clock again */

#endif
}

/*..........................................................................*/
void BSP_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;          /* for initialising the GPIO */
    TIM_InitTypeDef  TIM_InitStruct;           /* for initialising the TIM  */
    ADC_InitTypeDef  ADC_InitStruct;           /* for initialising the ADC  */
    uint8_t n;

    WDG_Cmd(DISABLE);

    SCU_PCLKDivisorConfig(SCU_PCLK_Div2); /* peripheral bus clokdivisor = 2 */

                            /* GPIO 6 clock source enable, used by the LEDs */
    SCU_APBPeriphClockConfig(__GPIO6, ENABLE);

                         /* GPIO 6 clock source enable, used by the Buttons */
    SCU_APBPeriphClockConfig(__GPIO7, ENABLE);
                                                        /* enable VIC clock */
    SCU_AHBPeriphClockConfig(__VIC, ENABLE);
    SCU_AHBPeriphReset(__VIC, DISABLE);

    VIC_DeInit();
    VIC0->DVAR = (uint32_t)&ISR_spur;  /* install spurious handler for VIC0 */
    VIC1->DVAR = (uint32_t)&ISR_spur;  /* install spurious handler for VIC1 */
    for (n = 0; n < 32; ++n) {
        VIC_ITCmd(n, DISABLE);
    }
                                    /* configure GPIO6 to drive the LEDs... */
    GPIO_DeInit(GPIO6);
    GPIO_InitStruct.GPIO_Pin          = GPIO_Pin_All;
    GPIO_InitStruct.GPIO_Direction    = GPIO_PinOutput;
    GPIO_InitStruct.GPIO_Type         = GPIO_Type_PushPull;
    GPIO_InitStruct.GPIO_IPConnected  = GPIO_IPConnected_Disable;
    GPIO_InitStruct.GPIO_Alternate    = GPIO_OutputAlt1;
    GPIO_Init(GPIO6, &GPIO_InitStruct);

                           /* configure GPIO9 to drive the LCD backlight... */
    GPIO_DeInit(GPIO9);
    GPIO_InitStruct.GPIO_Pin          = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Direction    = GPIO_PinOutput;
    GPIO_InitStruct.GPIO_Type         = GPIO_Type_PushPull;
    GPIO_InitStruct.GPIO_IPConnected  = GPIO_IPConnected_Disable;
    GPIO_InitStruct.GPIO_Alternate    = GPIO_OutputAlt1;
    GPIO_Init(GPIO9, &GPIO_InitStruct);

                                 /* configure GPIO7 to drive the Buttons... */
    GPIO_DeInit(GPIO7);
    GPIO_InitStruct.GPIO_Pin          = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Direction    = GPIO_PinInput;
    GPIO_InitStruct.GPIO_Type         = GPIO_Type_PushPull;
    GPIO_InitStruct.GPIO_IPConnected  = GPIO_IPConnected_Disable;
    GPIO_InitStruct.GPIO_Alternate    = GPIO_InputAlt1;
    GPIO_Init(GPIO7, &GPIO_InitStruct);

                          /* configure the TIM0 and interrupt controller... */
    SCU_APBPeriphClockConfig(__TIM01, ENABLE);
    SCU_APBPeriphReset(__TIM01, DISABLE);

    TIM_DeInit(TIM0);

    TIM_StructInit(&TIM_InitStruct);
    TIM_InitStruct.TIM_Mode           = TIM_OCM_CHANNEL_1;
    TIM_InitStruct.TIM_OC1_Modes      = TIM_TIMING;
    TIM_InitStruct.TIM_Clock_Source   = TIM_CLK_APB;
    TIM_InitStruct.TIM_Clock_Edge     = TIM_CLK_EDGE_RISING;
    TIM_InitStruct.TIM_Prescaler      = BSP_TIM0_PRESCALER - 1;
    TIM_InitStruct.TIM_Pulse_Level_1  = TIM_HIGH;
    TIM_InitStruct.TIM_Pulse_Length_1 = BSP_TIM0_PERIOD - 1;
    TIM_Init(TIM0, &TIM_InitStruct);

    VIC_Config(TIM0_ITLine, VIC_IRQ, BSP_TICK_PRIO);
    VIC_ITCmd(TIM0_ITLine, ENABLE);

    TIM_ITConfig(TIM0, TIM_IT_OC1, ENABLE);
    TIM_CounterCmd(TIM0, TIM_CLEAR);

                           /* configure the ADC and interrupt controller... */
    SCU_APBPeriphClockConfig(__ADC, ENABLE);           /* enable ADC clocks */
    SCU_APBPeriphReset(__ADC, DISABLE);           /* release ADC from reset */

                                       /* make the GPIO pin an analog input */
    GPIO_ANAPinConfig(GPIO_ANAChannel5, ENABLE);
    ADC_StructInit(&ADC_InitStruct);
    ADC_InitStruct.ADC_Channel_5_Mode = ADC_NoThreshold_Conversion;
    ADC_InitStruct.ADC_Select_Channel = ADC_Channel_5;
    ADC_InitStruct.ADC_Scan_Mode = DISABLE;

    ADC_Cmd(ENABLE);                                     /* turn on the ADC */
    ADC_PrescalerConfig(0x02);                   /* setup the A2D prescaler */
    ADC_Init(&ADC_InitStruct);       /* do the basic setup of the converter */
    ADC_ITConfig(ADC_IT_ECV, ENABLE);     /* interrupt on end of conversion */

    VIC_Config(ADC_ITLine, (VIC_ITLineMode)VIC_IRQ, BSP_ADC_PRIO);
    VIC_ITCmd(ADC_ITLine,  ENABLE);


                              /* configure PWM DAC (TIM3) 10 bit GPIO4.6... */
    SCU_APBPeriphClockConfig(__GPIO4, ENABLE);        /* enable GPIO clocks */
    SCU_APBPeriphReset(__GPIO4, DISABLE);        /* release GPIO from reset */

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Direction    = GPIO_PinOutput;
    GPIO_InitStruct.GPIO_Type         = GPIO_Type_PushPull;
    GPIO_InitStruct.GPIO_IPConnected  = GPIO_IPConnected_Disable;
    GPIO_InitStruct.GPIO_Alternate    = GPIO_OutputAlt2;
    GPIO_InitStruct.GPIO_Pin          = GPIO_Pin_6;
    GPIO_Init(GPIO4, &GPIO_InitStruct);

    SCU_APBPeriphClockConfig(__TIM23, ENABLE);       /* Enable TIM23 clocks */
    SCU_APBPeriphReset(__TIM23,DISABLE);             /* Release TIM23 reset */

    TIM_StructInit(&TIM_InitStruct);
    TIM_InitStruct.TIM_Mode           = TIM_PWM;
    TIM_InitStruct.TIM_OC1_Modes      = TIM_WAVE;
    TIM_InitStruct.TIM_Clock_Source   = TIM_CLK_APB;
    TIM_InitStruct.TIM_Prescaler      = 0x30;
    TIM_InitStruct.TIM_Pulse_Level_1  = TIM_LOW;
    TIM_InitStruct.TIM_Period_Level   = TIM_HIGH;
    TIM_InitStruct.TIM_Pulse_Length_1 = 0x200;
    TIM_InitStruct.TIM_Full_Period    = 0x3FF;
    TIM_Init(TIM3, &TIM_InitStruct);

    TIM_CounterCmd(TIM3, TIM_CLEAR);                  /* Clear TIM3 counter */
    TIM_ClearFlag(TIM3,TIM_IT_OC2);                  /* Clear TIM3 flag OC2 */
    TIM_ITConfig(TIM3,                       /* Disable all TIM3 interrupts */
        TIM_IT_OC1 | TIM_IT_IC1 | TIM_IT_IC2 | TIM_IT_OC2 | TIM_IT_TO,
        DISABLE);

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }

    QS_OBJ_DICTIONARY(&l_TIM0_IRQHandler);
}
/*..........................................................................*/
uint32_t BSP_get_ticks(void) {
    return l_nticks;
}
/*..........................................................................*/
void BSP_LED_on(uint8_t n) {
    LED_ON(n);
}
/*..........................................................................*/
void BSP_LED_off(uint8_t n) {
    LED_OFF(n);
}
/*..........................................................................*/
void BSP_LCD_light_on(void) {
    LCD_LIGHT_ON();
}
/*..........................................................................*/
void BSP_LCD_light_off(void) {
    LCD_LIGHT_OFF();
}

/*..........................................................................*/
__arm void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    QF_INT_DISABLE();         /* make sure that all interrupts are disabled */
    LED_ON_ALL();                                      /* light up all LEDs */
    for (;;) {                            /* hang here in the for-ever loop */
    }
}

/*--------------------------------------------------------------------------*/
void Dly100us(void *arg) {           /* required for the HD44780 LCD driver */
    uint32_t volatile dly = (uint32_t)arg*(BSP_CPU_CLOCK_HZ/100000);
    while (dly-- != 0) {
    }
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
uint32_t l_timeOverflow;

uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[BSP_QS_BUF_SIZE];        /* buffer for Quantum Spy */
    GPIO_InitTypeDef GPIO_InitStruct;
    UART_InitTypeDef UART_InitStruct;

    QS_initBuf(qsBuf, sizeof(qsBuf));

                                 /* configure the UART0 for QSPY output ... */
    SCU_APBPeriphClockConfig(__UART0, ENABLE);    /* enable clock for UART0 */
    SCU_APBPeriphClockConfig(__GPIO3, ENABLE);    /* enable clock for GPIO3 */

    SCU_APBPeriphReset(__UART0, DISABLE);        /* remove UART0 from reset */
    SCU_APBPeriphReset(__GPIO3, DISABLE);        /* remove GPIO3 from reset */

                                      /* configure UART0_TX pin GPIO3.4 ... */
    GPIO_DeInit(GPIO3);
    GPIO_InitStruct.GPIO_Pin         = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Direction   = GPIO_PinOutput;
    GPIO_InitStruct.GPIO_Type        = GPIO_Type_PushPull;
    GPIO_InitStruct.GPIO_IPConnected = GPIO_IPConnected_Disable;
    GPIO_InitStruct.GPIO_Alternate   = GPIO_OutputAlt3;
    GPIO_Init(GPIO3, &GPIO_InitStruct);

                                                      /* configure UART0... */
    UART_DeInit(UART0);            /* force UART0 registers to reset values */
    UART_InitStruct.UART_WordLength  = UART_WordLength_8D;
    UART_InitStruct.UART_StopBits    = UART_StopBits_1;
    UART_InitStruct.UART_Parity      = UART_Parity_No;
    UART_InitStruct.UART_BaudRate    = BSP_QS_BAUD_RATE;
    UART_InitStruct.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStruct.UART_Mode        = UART_Mode_Tx;
    UART_InitStruct.UART_FIFO        = UART_FIFO_Enable;
    UART_InitStruct.UART_TxFIFOLevel = UART_FIFOLevel_1_8;
    UART_InitStruct.UART_RxFIFOLevel = UART_FIFOLevel_1_8;
    UART_Init(UART0, &UART_InitStruct);                 /* initialize UART0 */
    UART_Cmd(UART0, ENABLE);                                /* enable UART0 */

                                                 /* setup the QS filters... */
    QS_FILTER_ON(QS_ALL_RECORDS);

//    QS_FILTER_OFF(QS_QEP_STATE_EMPTY);
//    QS_FILTER_OFF(QS_QEP_STATE_ENTRY);
//    QS_FILTER_OFF(QS_QEP_STATE_EXIT);
//    QS_FILTER_OFF(QS_QEP_STATE_INIT);
//    QS_FILTER_OFF(QS_QEP_INIT_TRAN);
//    QS_FILTER_OFF(QS_QEP_INTERN_TRAN);
//    QS_FILTER_OFF(QS_QEP_TRAN);
//    QS_FILTER_OFF(QS_QEP_dummyD);

//    QS_FILTER_OFF(QS_QF_ACTIVE_ADD);
//    QS_FILTER_OFF(QS_QF_ACTIVE_REMOVE);
//    QS_FILTER_OFF(QS_QF_ACTIVE_SUBSCRIBE);
//    QS_FILTER_OFF(QS_QF_ACTIVE_UNSUBSCRIBE);
//    QS_FILTER_OFF(QS_QF_ACTIVE_POST_FIFO);
//    QS_FILTER_OFF(QS_QF_ACTIVE_POST_LIFO);
//    QS_FILTER_OFF(QS_QF_ACTIVE_GET);
//    QS_FILTER_OFF(QS_QF_ACTIVE_GET_LAST);
//    QS_FILTER_OFF(QS_QF_EQUEUE_INIT);
//    QS_FILTER_OFF(QS_QF_EQUEUE_POST_FIFO);
//    QS_FILTER_OFF(QS_QF_EQUEUE_POST_LIFO);
//    QS_FILTER_OFF(QS_QF_EQUEUE_GET);
//    QS_FILTER_OFF(QS_QF_EQUEUE_GET_LAST);
//    QS_FILTER_OFF(QS_QF_MPOOL_INIT);
//    QS_FILTER_OFF(QS_QF_MPOOL_GET);
//    QS_FILTER_OFF(QS_QF_MPOOL_PUT);
//    QS_FILTER_OFF(QS_QF_PUBLISH);
//    QS_FILTER_OFF(QS_QF_NEW);
//    QS_FILTER_OFF(QS_QF_GC_ATTEMPT);
//    QS_FILTER_OFF(QS_QF_GC);
    QS_FILTER_OFF(QS_QF_TICK);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_ARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_AUTO_DISARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM_ATTEMPT);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_REARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_POST);
//    QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
//    QS_FILTER_OFF(QS_QF_CRIT_EXIT);
//    QS_FILTER_OFF(QS_QF_ISR_ENTRY);
//    QS_FILTER_OFF(QS_QF_ISR_EXIT);

    return (uint8_t)1;            /* indicate successfull QS initialization */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t nBytes = BSP_UART_TX_FIFO; /* the capacity of the UART TX FIFO */
    uint8_t const *block;
    while ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        while ((UART0->FR & 0x80) == 0) {             /* TX FIFO not empty? */
        }                                                /* keep waiting... */
        while (nBytes-- != 0) {
            UART0->DR = *block++;          /* stick the byte to the TX FIFO */
        }
        nBytes = BSP_UART_TX_FIFO;              /* for the next time around */
    }
}
/*..........................................................................*/
/* NOTE: getTime is invoked within a critical section (inetrrupts disabled) */
uint32_t QS_onGetTime(void) {
    uint16_t currTime16 = (uint16_t)TIM0->CNTR;
    l_currTime32 += (currTime16 - l_prevTime16) & 0xFFFF;
    l_prevTime16 = currTime16;
    return l_currTime32;
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/
