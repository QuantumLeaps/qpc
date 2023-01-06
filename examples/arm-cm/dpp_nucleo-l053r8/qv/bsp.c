/*============================================================================
* Product: DPP example, NUCLEO-L053R8 board, cooperative QV kernel
* Last updated for version 7.2.0
* Last updated on  2022-12-13
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
============================================================================*/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

#include "stm32l0xx.h"  /* CMSIS-compliant header file for the MCU used */
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE

/* Local-scope defines -----------------------------------------------------*/
/* LED pins available on the board (just one user LED LD2--Green on PA.5) */
#define LED_LD2  (1U << 5)

/* Button pins available on the board (just one user Button B1 on PC.13) */
#define BTN_B1   (1U << 13)

static uint32_t l_rnd;  /* random seed */

#ifdef Q_SPY
    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    /* QSpy source IDs */
    static QSpyId const l_SysTick_Handler = { 0U };

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER,
        CONTEXT_SW,
    };

#endif

/* ISRs used in the application ==========================================*/
void SysTick_Handler(void);
void EXTI0_1_IRQHandler(void);
void USART2_IRQHandler(void);

/*..........................................................................*/
void SysTick_Handler(void) {   /* system clock tick ISR */
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { 0U, 0U };
    uint32_t current;
    uint32_t tmp;

#ifdef Q_SPY
    {
        tmp = SysTick->CTRL; /* clear CTRL_COUNTFLAG */
        QS_tickTime_ += QS_tickPeriod_; /* account for the clock rollover */
    }
#endif

    QTIMEEVT_TICK_X(0U, &l_SysTick_Handler); /* process time events for rate 0 */
    //QACTIVE_POST(the_Ticker0, 0, &l_SysTick_Handler); /* post to Ticker0 */

    /* get state of the user button */
    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = ~GPIOC->IDR; /* read Port C with the state of Button B1 */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & BTN_B1) != 0U) {  /* debounced B1 state changed? */
        if ((buttons.depressed & BTN_B1) != 0U) { /* is B1 depressed? */
            static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U};
            QACTIVE_PUBLISH(&pauseEvt, &l_SysTick_Handler);
        }
        else {            /* the button is released */
            static QEvt const serveEvt = { SERVE_SIG, 0U, 0U};
            QACTIVE_PUBLISH(&serveEvt, &l_SysTick_Handler);
        }
    }
    QV_ARM_ERRATUM_838869();
}
/*..........................................................................*/
/* interrupt handler for testing preemptions in QV */
void EXTI0_1_IRQHandler(void) {
    static QEvt const testEvt = { TEST_SIG, 0U, 0U };
    QACTIVE_POST(AO_Table, &testEvt, (void *)0);
    QV_ARM_ERRATUM_838869();
}
/*..........................................................................*/
#ifdef Q_SPY
void USART2_IRQHandler(void) { /* used in QS-RX (kernel UNAWARE interrutp) */
    /* is RX register NOT empty? */
    if ((USART2->ISR & (1U << 5)) != 0) {
        uint32_t b = USART2->RDR;
        QS_RX_PUT(b);
    }
    QV_ARM_ERRATUM_838869();
}
#endif

/* BSP functions ===========================================================*/
/*..........................................................................*/
/* MPU setup for STM32L053R8 MCU */
static void STM32L053R8_MPU_setup(void) {
    /* The following MPU configuration contains the general STM32 memory model
    * as described in the ST AppNote AN4838 "Managing memory protection unit
    * in STM32 MCUs", Figure 2. Cortex-M0+/M3/M4/M7 processor memory map.
    *
    * Please note that the actual STM32 MCUs provide much less Flash and SRAM
    * than the maximums configured here. This means that actual MCUs have
    * unmapped memory regions (e.g., beyond the actual SRAM). Attempts to
    * access these regions causes the HardFault exception, which is the
    * desired behavior.
    */
    static struct {
        uint32_t rbar;
        uint32_t rasr;
    } const mpu_setup[] = {

        { /* region #0: Flash: base=0x0000'0000, size=512M=2^(28+1) */
          0x00000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 0U), /* region #0 */
          (28U << MPU_RASR_SIZE_Pos)        /* 2^(18+1) region */
              | (0x6U << MPU_RASR_AP_Pos)   /* PA:ro/UA:ro */
              | (1U << MPU_RASR_C_Pos)      /* C=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        { /* region #1: SRAM: base=0x2000'0000, size=512M=2^(28+1) */
          0x20000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 1U), /* region #1 */
          (28U << MPU_RASR_SIZE_Pos)        /* 2^(28+1) region */
              | (0x3U << MPU_RASR_AP_Pos)   /* PA:rw/UA:rw */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | (1U << MPU_RASR_S_Pos)      /* S=1 */
              | (1U << MPU_RASR_C_Pos)      /* C=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        /* region #3: (not configured) */
        { MPU_RBAR_VALID_Msk | (MPU_RBAR_REGION_Msk & 2U), 0U },

        { /* region #3: Peripherals: base=0x4000'0000, size=512M=2^(28+1) */
          0x40000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 3U), /* region #3 */
          (28U << MPU_RASR_SIZE_Pos)        /* 2^(28+1) region */
              | (0x3U << MPU_RASR_AP_Pos)   /* PA:rw/UA:rw */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | (1U << MPU_RASR_S_Pos)      /* S=1 */
              | (1U << MPU_RASR_B_Pos)      /* B=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        { /* region #4: Priv. Periph: base=0xE000'0000, size=512M=2^(28+1) */
          0xE0000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 4U), /* region #4 */
          (28U << MPU_RASR_SIZE_Pos)        /* 2^(28+1) region */
              | (0x3U << MPU_RASR_AP_Pos)   /* PA:rw/UA:rw */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | (1U << MPU_RASR_S_Pos)      /* S=1 */
              | (1U << MPU_RASR_B_Pos)      /* B=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        { /* region #5: Ext RAM: base=0x6000'0000, size=1G=2^(29+1) */
          0x60000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 5U), /* region #5 */
          (29U << MPU_RASR_SIZE_Pos)        /* 2^(28+1) region */
              | (0x3U << MPU_RASR_AP_Pos)   /* PA:rw/UA:rw */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | (1U << MPU_RASR_S_Pos)      /* S=1 */
              | (1U << MPU_RASR_B_Pos)      /* B=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        { /* region #6: Ext Dev: base=0xA000'0000, size=1G=2^(29+1) */
          0xA0000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 6U), /* region #6 */
          (29U << MPU_RASR_SIZE_Pos)        /* 2^(28+1) region */
              | (0x3U << MPU_RASR_AP_Pos)   /* PA:rw/UA:rw */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | (1U << MPU_RASR_S_Pos)      /* S=1 */
              | (1U << MPU_RASR_B_Pos)      /* B=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

        { /* region #7: NULL-pointer: base=0x000'0000, size=128M=2^(26+1) */
          /* NOTE: this region extends to  0x080'0000, which is where
          * the ROM is re-mapped by STM32
          */
          0x00000000U                       /* base address */
              | MPU_RBAR_VALID_Msk          /* valid region */
              | (MPU_RBAR_REGION_Msk & 7U), /* region #7 */
          (26U << MPU_RASR_SIZE_Pos)        /* 2^(26+1)=128M region */
              | (0x0U << MPU_RASR_AP_Pos)   /* PA:na/UA:na */
              | (1U << MPU_RASR_XN_Pos)     /* XN=1 */
              | MPU_RASR_ENABLE_Msk         /* region enable */
        },

    };

    __DSB();
    MPU->CTRL = 0U; /* disable the MPU */
    for (uint_fast8_t n = 0U; n < Q_DIM(mpu_setup); ++n) {
        MPU->RBAR = mpu_setup[n].rbar;
        MPU->RASR = mpu_setup[n].rasr;
    }
    MPU->CTRL = MPU_CTRL_PRIVDEFENA_Msk     /* enable background region */
                | MPU_CTRL_ENABLE_Msk;      /* enable the MPU */
    __ISB();
    __DSB();
}
/*..........................................................................*/
void BSP_init(void) {
    /* setup the MPU... */
    STM32L053R8_MPU_setup();

    /* NOTE: SystemInit() has been already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    /* enable GPIOA clock port for the LED LD2 */
    RCC->IOPENR |= (1U << 0);

    /* configure LED (PA.5) pin as push-pull output, no pull-up, pull-down */
    GPIOA->MODER   &= ~((3U << 2*5));
    GPIOA->MODER   |=  ((1U << 2*5));
    GPIOA->OTYPER  &= ~((1U <<   5));
    GPIOA->OSPEEDR &= ~((3U << 2*5));
    GPIOA->OSPEEDR |=  ((1U << 2*5));
    GPIOA->PUPDR   &= ~((3U << 2*5));

    /* enable GPIOC clock port for the Button B1 */
    RCC->IOPENR |=  (1U << 2);

    /* configure Button (PC.13) pins as input, no pull-up, pull-down */
    GPIOC->MODER   &= ~(3U << 2*13);
    GPIOC->OSPEEDR &= ~(3U << 2*13);
    GPIOC->OSPEEDR |=  (1U << 2*13);
    GPIOC->PUPDR   &= ~(3U << 2*13);

    BSP_randomSeed(1234U); /* seed the random number generator */

    /* initialize the QS software tracing... */
    if (QS_INIT((void *)0) == 0U) {
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(CONTEXT_SW);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_ALL_RECORDS); /* all records */
    QS_GLB_FILTER(-QS_QF_TICK);    /* exclude the clock tick */
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    if (stat[0] == 'h') {
        GPIOA->BSRR |= LED_LD2;  /* turn LED on  */
    }
    else {
        GPIOA->BSRR |= (LED_LD2 << 16);  /* turn LED off */
    }

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);                  /* Philosopher number */
        QS_STR(stat);                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    /* not enough LEDs to implement this feature */
    if (paused != 0U) {
        //GPIOA->BSRR |= (LED_LD2);  /* turn LED[n] on  */
    }
    else {
        //GPIOA->BSRR |= (LED_LD2 << 16);  /* turn LED[n] off */
    }
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
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
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}
/*..........................................................................*/
void BSP_wait4SW1(void) {
    while ((GPIOC->IDR  & BTN_B1) != 0U) {
        GPIOA->BSRR |= (LED_LD2);        /* turn LED2 on  */
        GPIOA->BSRR |= (LED_LD2 << 16);  /* turn LED2 off */
    }
}
/*..........................................................................*/
void BSP_ledOn(void) {
    //GPIOA->BSRR |= (LED_LD2);        /* turn LED2 on  */
}
/*..........................................................................*/
void BSP_ledOff(void) {
    //GPIOA->BSRR |= (LED_LD2 << 16);  /* turn LED2 off */
}

/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* set priorities of ALL ISRs used in the system */
    NVIC_SetPriority(USART2_IRQn,    0);
    NVIC_SetPriority(SysTick_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 0U);
    NVIC_SetPriority(EXTI0_1_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 1U);
    /* ... */

    /* enable IRQs... */
    NVIC_EnableIRQ(EXTI0_1_IRQn);
#ifdef Q_SPY
    NVIC_EnableIRQ(USART2_IRQn); /* UART2 interrupt used for QS-RX */
#endif
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
#ifdef QF_ON_CONTEXT_SW
/* NOTE: the context-switch callback is called with interrupts DISABLED */
void QF_onContextSw(QActive *prev, QActive *next) {
    QS_BEGIN_NOCRIT(CONTEXT_SW, 0U) /* no critical section! */
        QS_OBJ(prev);
        QS_OBJ(next);
    QS_END_NOCRIT()
}
#endif /* QF_ON_CONTEXT_SW */
/*..........................................................................*/
void QV_onIdle(void) {  /* called with interrupts disabled, see NOTE01 */

    /* toggle an LED on and then off (not enough LEDs, see NOTE02) */
    //GPIOA->BSRR |= (LED_LD2);        /* turn LED[n] on  */
    //GPIOA->BSRR |= (LED_LD2 << 16);  /* turn LED[n] off */

#ifdef Q_SPY
    QF_INT_ENABLE();
    QS_rxParse();  /* parse all the received bytes */

    if ((USART2->ISR & (1U << 7)) != 0) {  /* is TXE empty? */
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {  /* not End-Of-Data? */
            USART2->TDR = (b & 0xFFU);  /* put into the DR register */
        }
    }
#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    /* !!!CAUTION!!!
    * QV_CPU_SLEEP() contains the WFI instruction, which stops the CPU
    * clock, which unfortunately disables the JTAG port, so the ST-Link
    * debugger can no longer connect to the board. For that reason, the call
    * to QV_CPU_SLEEP() has to be used with CAUTION.
    */
    /* NOTE: If you find your board "frozen" like this, strap BOOT0 to VDD and
    * reset the board, then connect with ST-Link Utilities and erase the part.
    * The trick with BOOT(0) is it gets the part to run the System Loader
    * instead of your broken code. When done disconnect BOOT0, and start over.
    */
    //QV_CPU_SLEEP();  /* atomically go to sleep and enable interrupts */
    QF_INT_ENABLE(); /* for now, just enable interrupts */
#else
    QF_INT_ENABLE(); /* just enable interrupts */
#endif
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char const * const module, int_t const loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;
    QS_ASSERTION(module, loc, 10000U); /* report assertion to QS */

#ifndef NDEBUG
    BSP_wait4SW1();
#endif
    NVIC_SystemReset();
}

/* QS callbacks ============================================================*/
#ifdef Q_SPY
/*..........................................................................*/
#define __DIV(__PCLK, __BAUD)       (((__PCLK / 4) *25)/(__BAUD))
#define __DIVMANT(__PCLK, __BAUD)   (__DIV(__PCLK, __BAUD)/100)
#define __DIVFRAQ(__PCLK, __BAUD)   \
    (((__DIV(__PCLK, __BAUD) - (__DIVMANT(__PCLK, __BAUD) * 100)) \
        * 16 + 50) / 100)
#define __USART_BRR(__PCLK, __BAUD) \
    ((__DIVMANT(__PCLK, __BAUD) << 4)|(__DIVFRAQ(__PCLK, __BAUD) & 0x0F))

/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[2*1024]; /* buffer for Quantum Spy */
    static uint8_t qsRxBuf[256];  /* buffer for QS-RX channel */

    (void)arg; /* avoid the "unused parameter" compiler warning */

    QS_initBuf(qsBuf, sizeof(qsBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    /* enable peripheral clock for USART2 */
    RCC->IOPENR  |= ( 1U <<  0);   /* Enable GPIOA clock   */
    RCC->APB1ENR |= ( 1U << 17);   /* Enable USART#2 clock */

    /* Configure PA3 to USART2_RX, PA2 to USART2_TX */
    GPIOA->AFR[0] &= ~((15U << 4* 3) | (15U << 4* 2) );
    GPIOA->AFR[0] |=  (( 4U << 4* 3) | ( 4U << 4* 2) );
    GPIOA->MODER  &= ~(( 3U << 2* 3) | ( 3U << 2* 2) );
    GPIOA->MODER  |=  (( 2U << 2* 3) | ( 2U << 2* 2) );

    USART2->BRR  = __USART_BRR(SystemCoreClock, 115200U);  /* baud rate */
    USART2->CR3  = 0x0000 |       /* no flow control */
                   (1U << 12);    /* disable overrun detection (OVRDIS) */
    USART2->CR2  = 0x0000;        /* 1 stop bit      */
    USART2->CR1  = ((1U <<  2) |  /* enable RX       */
                    (1U <<  3) |  /* enable TX       */
                    (1U <<  5) |  /* enable RX interrupt */
                    (0U << 12) |  /* 8 data bits     */
                    (0U << 28) |  /* 8 data bits     */
                    (1U <<  0) ); /* enable USART    */

    QS_tickPeriod_ = SystemCoreClock / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_; /* to start the timestamp at zero */

    return 1U; /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) { /* NOTE: invoked with interrupts DISABLED */
    if ((SysTick->CTRL & 0x00010000) == 0) {  /* COUNT no set? */
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
    while ((b = QS_getByte()) != QS_EOD) {    /* while not End-Of-Data... */
        QF_INT_ENABLE();
        while ((USART2->ISR & (1U << 7)) == 0U) { /* while TXE not empty */
        }
        USART2->TDR = (b & 0xFFU);  /* put into the DR register */
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
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE01:
* The QV_onIdle() callback is called with interrupts disabled, because the
* determination of the idle condition might change by any interrupt posting
* an event. QV_onIdle() must internally enable interrupts, ideally
* atomically with putting the CPU to the power-saving mode.
*
* NOTE02:
* The User LED is used to visualize the idle loop activity. The brightness
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
