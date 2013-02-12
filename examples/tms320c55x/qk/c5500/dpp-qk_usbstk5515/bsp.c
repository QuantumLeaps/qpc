/*****************************************************************************
* Product: BSP for DPP example, QK, TMS320C5515 eZdsp USB stick
* Last Updated for Version: 4.5.03
* Date of the Last Update:  Jan 17, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
#include "dpp.h"
#include "bsp.h"

#include "csl_intc.h"
#include "csl_pll.h"
#include "csl_gpio.h"
#include "csl_rtc.h"
#include "cslr_tim.h"
#include "cslr_sysctrl.h"

Q_DEFINE_THIS_FILE

/* CPU clock speed (100MHz) */
#define CPU_CLOCK_HZ       100000000U

static uint16_t const l_userLED[] = {
    CSL_GPIO_PIN17,
    CSL_GPIO_PIN16,
    CSL_GPIO_PIN15,
    CSL_GPIO_PIN14
};
#define ULED(n_)           ((CSL_GpioPinNum)l_userLED[n_])
#define ULED_ON            0U
#define ULED_OFF           1U

static uint32_t    l_rnd;                                     /* random seed */
static CSL_GpioObj l_gpioObj;

/* User LEDs */
static void ULED_init(void);
static inline void ULED_set(uint16_t n, uint16_t state) {
   GPIO_write(&l_gpioObj, (CSL_GpioPinNum)l_userLED[n], state);
}

/* System LED */
#define SLED_ON()  asm(" bit(ST1, ST1_XF) = #1")
#define SLED_OFF() asm(" bit(ST1, ST1_XF) = #0")

void VECSTART(void);

#ifdef Q_SPY
    #include "csl_uart.h"

    #define UART_BAUD_RATE   115200U
    #define UART_FIFO_DEPTH  16U

    static CSL_UartObj l_uartObj;
    uint8_t const l_TINT_isr = 0U;

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };
#endif


/*..........................................................................*/
interrupt void TINT_isr(void) {
    CSL_SYSCTRL_REGS->TIAFR |= 0x0001U;                 /* clear Timer0 bit */

    QK_ISR_ENTRY();                 /* inform the QK kernel about ISR entry */

    QF_TICK(&l_TINT_isr);                      /* handle the QF time events */

    QK_ISR_EXIT();                   /* inform the QK kernel about ISR exit */
}
/*..........................................................................*/
interrupt void RTC_isr(void) {
    static QEvt const testEvt = { MAX_SIG, 0U, 0U };

    /*CSL_RTC_REGS->RTCINTFL = 0x01U;*/       /* clear the interrupt source */

    QK_ISR_ENTRY();                 /* inform the QK kernel about ISR entry */

    QACTIVE_POST(AO_Table, &testEvt, 0);      /* post a test event to Table */

    QK_ISR_EXIT();                   /* inform the QK kernel about ISR exit */
}
/*..........................................................................*/
interrupt void illegal_isr(void) {
    Q_ERROR();                                           /* assert an error */
}

/*..........................................................................*/
void BSP_init(void) {
    PLL_Config pllCfg_100MHz = {
        0x8BE8U, 0x8000U, 0x0806U, 0x0000U
    };
    PLL_Obj pllObj;
    uint16_t i;

    PLL_init(&pllObj, CSL_PLL_INST_0);
    PLL_reset(&pllObj);
    PLL_config(&pllObj, &pllCfg_100MHz);

    QF_zero();                        /* clear the QF variables, see NOTE01 */

    CSL_SYSCTRL_REGS->PCGCR1 = 0U;      /* enable clocks to all peripherals */
    CSL_SYSCTRL_REGS->PCGCR2 = 0U;
    CSL_SYSCTRL_REGS->EBSR   = 0x1800U;             /* configure I/O muxing */
    CSL_SYSCTRL_REGS->PSRCR  = 0x0020U;            /* reset all peripherals */
    CSL_SYSCTRL_REGS->PRCR   = 0x00BFU;

    ULED_init();                              /* configure the User LEDs... */
    BSP_randomSeed(1234U);

    IRQ_globalDisable();
    IRQ_disableAll();                         /* disable all the interrupts */
    IRQ_clearAll();                         /* clear any pending interrupts */
    IRQ_setVecs((uint32_t)&VECSTART);               /* set the vector table */
    for (i = 1U; i < 32U; ++i) {               /* pre-fill the Vector table */
        IRQ_plug(i, &illegal_isr);                      /* with illegal ISR */
    }

    /* plug in all ISRs into the vector table...*/
    IRQ_plug(TINT_EVENT, &TINT_isr);
    IRQ_plug(RTC_EVENT,  &RTC_isr);
    /* ... */

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_TINT_isr);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char_t const *stat) {
    if (n < Q_DIM(l_userLED)) {
        ULED_set(n, (stat[0] == 'e') ? ULED_ON : ULED_OFF);
    }

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    (void)paused;
}
/*..........................................................................*/
uint32_t BSP_random(void) {  /* a very cheap pseudo-random-number generator */
    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    l_rnd = l_rnd * (3U*7U*11U*13U*23U);
    return l_rnd >> 8;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*--------------------------------------------------------------------------*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    /* Next two lines are for debug only to halt the processor here.
    * YOU need to change this policy for the production release!
    */
    QF_INT_DISABLE();
    for(;;) {
    }
}

/*..........................................................................*/
void QF_onStartup(void) {
                      /* configuration of Timer0 as the system clock tick...*/
    CSL_TIM_0_REGS->TIMPRD1 = (CPU_CLOCK_HZ / 4096U) / BSP_TICKS_PER_SEC;
    CSL_TIM_0_REGS->TIMPRD2 = 0U;
    CSL_TIM_0_REGS->TIMCNT1 = 0U;
    CSL_TIM_0_REGS->TIMCNT2 = 0U;
    CSL_SYSCTRL_REGS->TIAFR = 0x0007U;      /* clear timer aggregation reg. */
    CSL_TIM_0_REGS->TCR     = 0x802FU;     /* autoReload | prescaler = 4096 */
    IRQ_enable(TINT_EVENT);                    /* enable the TINT interrupt */

    /* setup the RTC interrupt for testing */
    CSL_RTC_REGS->RTCINTEN  = 1U;                  /* enable RTC interrupts */
    CSL_RTC_REGS->RTCINTREG = 1U;  /* enable millisecond periodic interrupt */
    IRQ_enable(RTC_EVENT);
}
/*..........................................................................*/
void QF_onCleanup(void) {
    GPIO_close(&l_gpioObj);
}
/*..........................................................................*/
void QK_onIdle(void) {

    QF_INT_DISABLE();
    SLED_ON();                          /* switch the System LED on and off */
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    SLED_OFF();
    QF_INT_ENABLE();

#ifdef Q_SPY

    if (CSL_FEXT(l_uartObj.uartRegs->LSR, UART_LSR_THRE)) {
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {                              /* not End-Of-Data? */
            CSL_FSET(l_uartObj.uartRegs->THR, 7U, 0U, b);
        }
    }

#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular TMS320C5500 device.
    */
    asm(" IDLE");
#endif
}

/*..........................................................................*/
static void ULED_init(void) {
    CSL_Status status;
    CSL_GpioPinConfig config;

    /* configure the User LEDs... */
    CSL_FINST(CSL_SYSCTRL_REGS->EBSR, SYS_EBSR_SP0MODE, MODE2);
    GPIO_open(&l_gpioObj, &status);
    GPIO_reset(&l_gpioObj);

    config.direction = CSL_GPIO_DIR_OUTPUT;
    config.trigger   = CSL_GPIO_TRIG_CLEAR_EDGE;

    config.pinNum = (CSL_GpioPinNum)l_userLED[0];
    GPIO_configBit(&l_gpioObj, &config);
    GPIO_write(&l_gpioObj, (CSL_GpioPinNum)l_userLED[0], ULED_OFF);

    config.pinNum = (CSL_GpioPinNum)l_userLED[1];
    GPIO_configBit(&l_gpioObj, &config);
    GPIO_write(&l_gpioObj, (CSL_GpioPinNum)l_userLED[1], ULED_OFF);

    config.pinNum = (CSL_GpioPinNum)l_userLED[2];
    GPIO_configBit(&l_gpioObj, &config);
    GPIO_write(&l_gpioObj, (CSL_GpioPinNum)l_userLED[2], ULED_OFF);

    config.pinNum = (CSL_GpioPinNum)l_userLED[3];
    GPIO_configBit(&l_gpioObj, &config);
    GPIO_write(&l_gpioObj, (CSL_GpioPinNum)l_userLED[3], ULED_OFF);
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY

/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[2*256];                  /* buffer for Quantum Spy */
    CSL_UartSetup uartSetup;

    QS_initBuf(qsBuf, sizeof(qsBuf));

    uartSetup.clkInput = CPU_CLOCK_HZ;            /* input clock freq in Hz */
    uartSetup.baud = UART_BAUD_RATE;                           /* baud rate */
    uartSetup.wordLength = CSL_UART_WORD8;              /* word length of 8 */
    uartSetup.stopBits = 0;                       /* to generate 1 stop bit */
    uartSetup.parity = CSL_UART_DISABLE_PARITY;           /* disable parity */
    uartSetup.fifoControl = CSL_UART_FIFO_DMA1_DISABLE_TRIG01;/*enable FIFO */
    uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;         /* no loopback */
    uartSetup.afeEnable = CSL_UART_NO_AFE;          /* no auto flow control */
    uartSetup.rtsEnable = CSL_UART_NO_RTS;                        /* no RTS */

    UART_init(&l_uartObj, CSL_UART_INST_0, UART_POLLED);/* init. UART oject */
    CSL_SYSCTRL_REGS->EBSR = 0x1800U;   /* re-configure I/O muxing for LEDs */
    UART_setup(&l_uartObj, &uartSetup);         /* configure UART registers */

    // initialize the CPU Timer 1 used for QS timestamp
    CSL_TIM_1_REGS->TCR = 0U;                                /* stop Timer1 */
    CSL_TIM_1_REGS->TIMPRD1 = ~0U;
    CSL_TIM_1_REGS->TIMPRD2 = ~0U;
    CSL_TIM_1_REGS->TIMCNT1 = ~0U;
    CSL_TIM_1_REGS->TIMCNT2 = ~0U;
    CSL_TIM_1_REGS->TCR     = 0x801BU;      /* autoReload | prescaler = 128 */

                                                 /* setup the QS filters... */
    QS_FILTER_ON(QS_SIG_DIC);
    QS_FILTER_ON(QS_OBJ_DIC);
    QS_FILTER_ON(QS_FUN_DIC);

    QS_FILTER_ON(QS_QEP_STATE_ENTRY);
    QS_FILTER_ON(QS_QEP_STATE_EXIT);
    QS_FILTER_ON(QS_QEP_STATE_INIT);
    QS_FILTER_ON(QS_QEP_INIT_TRAN);
    QS_FILTER_ON(QS_QEP_INTERN_TRAN);
    QS_FILTER_ON(QS_QEP_TRAN);
    QS_FILTER_ON(QS_QEP_IGNORED);
    QS_FILTER_ON(QS_QEP_DISPATCH);
    QS_FILTER_ON(QS_QEP_UNHANDLED);

//    QS_FILTER_ON(QS_QF_ACTIVE_ADD);
//    QS_FILTER_ON(QS_QF_ACTIVE_REMOVE);
//    QS_FILTER_ON(QS_QF_ACTIVE_SUBSCRIBE);
//    QS_FILTER_ON(QS_QF_ACTIVE_UNSUBSCRIBE);
//    QS_FILTER_ON(QS_QF_ACTIVE_POST_FIFO);
//    QS_FILTER_ON(QS_QF_ACTIVE_POST_LIFO);
//    QS_FILTER_ON(QS_QF_ACTIVE_GET);
//    QS_FILTER_ON(QS_QF_ACTIVE_GET_LAST);
//    QS_FILTER_ON(QS_QF_EQUEUE_INIT);
//    QS_FILTER_ON(QS_QF_EQUEUE_POST_FIFO);
//    QS_FILTER_ON(QS_QF_EQUEUE_POST_LIFO);
//    QS_FILTER_ON(QS_QF_EQUEUE_GET);
//    QS_FILTER_ON(QS_QF_EQUEUE_GET_LAST);
//    QS_FILTER_ON(QS_QF_MPOOL_INIT);
//    QS_FILTER_ON(QS_QF_MPOOL_GET);
//    QS_FILTER_ON(QS_QF_MPOOL_PUT);
//    QS_FILTER_ON(QS_QF_PUBLISH);
//    QS_FILTER_ON(QS_QF_NEW);
//    QS_FILTER_ON(QS_QF_GC_ATTEMPT);
//    QS_FILTER_ON(QS_QF_GC);
//    QS_FILTER_ON(QS_QF_TICK);
//    QS_FILTER_ON(QS_QF_TIMEEVT_ARM);
//    QS_FILTER_ON(QS_QF_TIMEEVT_AUTO_DISARM);
//    QS_FILTER_ON(QS_QF_TIMEEVT_DISARM_ATTEMPT);
//    QS_FILTER_ON(QS_QF_TIMEEVT_DISARM);
//    QS_FILTER_ON(QS_QF_TIMEEVT_REARM);
//    QS_FILTER_ON(QS_QF_TIMEEVT_POST);
//    QS_FILTER_ON(QS_QF_CRIT_ENTRY);
//    QS_FILTER_ON(QS_QF_CRIT_EXIT);
//    QS_FILTER_ON(QS_QF_ISR_ENTRY);
//    QS_FILTER_ON(QS_QF_ISR_EXIT);

    QS_FILTER_ON(PHILO_STAT);

    return (uint8_t)1;                                    /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {          /* invoked with interrupts disabled */
    uint32_t tmr32;

    tmr32  = (uint32_t)CSL_TIM_1_REGS->TIMCNT2 << 16;
    tmr32 |= (uint32_t)CSL_TIM_1_REGS->TIMCNT1;

    return (QSTimeCtr)(0xFFFFFFFFUL - tmr32);
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    while ((b = QS_getByte()) != QS_EOD) {      /* while not End-Of-Data... */
        while (!CSL_FEXT(l_uartObj.uartRegs->LSR, UART_LSR_THRE)) {
        }
        CSL_FSET(l_uartObj.uartRegs->THR, 7U, 0U, b);    /* output the byte */
    }
}
#endif                                                             /* Q_SPY */

/*****************************************************************************
* NOTE01:
* The standard TI startup code (c_int00) does NOT zero the uninitialized
* variables, as required by the C-standard. Since QP relies on the clearing
* of the static uninitialized variables, the critical QP objects are cleared
* explicitly in this BSP.
*/








