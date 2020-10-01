/*****************************************************************************
* Product: DPP example, NXP mbed-LPC1768 board, QK kernel
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

#include "LPC17xx.h"  /* CMSIS-compliant header file for the MCU used */
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE

/* ISRs defined in this BSP ------------------------------------------------*/
void SysTick_Handler(void);
void EINT0_IRQHandler(void);

/* Local-scope objects -----------------------------------------------------*/
/* LEDs available on the board */
#define LED_1    (1U << 18)  /* P1.18 */
#define LED_2    (1U << 20)  /* P1.20 */
#define LED_3    (1U << 21)  /* P1.21 */
#define LED_4    (1U << 23)  /* P1.23 */

/* Push-Button wired externally to DIP8 (P0.6) */
#define BTN_EXT  (1U << 6)    /* P0.6 */

static uint32_t l_rnd;  /* random seed */

#ifdef Q_SPY
    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    /* event-source identifiers used for tracing */
    static uint8_t l_SysTick_Handler;
    static uint8_t l_EINT0_IRQHandler;

    #define UART_BAUD_RATE      115200U
    #define UART_FR_TXFE        0x80U
    #define UART_TXFIFO_DEPTH   16U

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER
    };

#endif

/* ISRs used in the application ==========================================*/
void SysTick_Handler(void) {
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { ~0U, ~0U };
    uint32_t current;
    uint32_t tmp;

    QK_ISR_ENTRY();   /* inform QK about entering an ISR */

#ifdef Q_SPY
    {
        tmp = SysTick->CTRL; /* clear CTRL_COUNTFLAG */
        QS_tickTime_ += QS_tickPeriod_; /* account for the clock rollover */
    }
#endif

    QF_TICK_X(0U, &l_SysTick_Handler); /* process time events for rate 0 */

    /* get state of the buttons */
    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = ~LPC_GPIO0->FIOPIN; /* read P0 with the state of the Buttons */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & BTN_EXT) != 0U) {  /* debounced BTN_EXT state changed? */
        if ((buttons.depressed & BTN_EXT) != 0U) { /* is BTN_EXT depressed? */
            static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U};
            QF_PUBLISH(&pauseEvt, &l_SysTick_Handler);
        }
        else {            /* the button is released */
            static QEvt const serveEvt = { SERVE_SIG, 0U, 0U};
            QF_PUBLISH(&serveEvt, &l_SysTick_Handler);
        }
    }

    QK_ISR_EXIT();  /* inform QK about exiting an ISR */
}
/*..........................................................................*/
void EINT0_IRQHandler(void) {
    QK_ISR_ENTRY(); /* inform QK about entering an ISR */

    QACTIVE_POST(AO_Table, Q_NEW(QEvt, MAX_PUB_SIG), /* for testing... */
                 &l_EINT0_IRQHandler);

    QK_ISR_EXIT();  /* inform QK about exiting an ISR */
}


/* BSP functions ===========================================================*/
void BSP_init(void) {
    /* NOTE: SystemInit() has been already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    /* turn the GPIO clock on */
    LPC_SC->PCONP |= (1U << 15);

    /* setup the GPIO pin functions for the LEDs... */
    LPC_PINCON->PINSEL3 &= ~(3U <<  4); /* LED_1: function P1.18 to GPIO */
    LPC_PINCON->PINSEL3 &= ~(3U <<  8); /* LED_2: function P1.20 to GPIO */
    LPC_PINCON->PINSEL3 &= ~(3U << 10); /* LED_3: function P1.21 to GPIO */
    LPC_PINCON->PINSEL3 &= ~(3U << 14); /* LED_4: function P1.23 to GPIO */

    /* Set GPIO-P1 LED pins to output */
    LPC_GPIO1->FIODIR |= (LED_1 | LED_2 | LED_3 | LED_4);


    /* setup the GPIO pin function for the Button... */
    LPC_PINCON->PINSEL0 &= ~(3U << 12); /* function P0.6 to GPIO, pull-up */

    /* Set GPIO-P0 Button pin as input */
    LPC_GPIO0->FIODIR &= ~BTN_EXT;

    BSP_randomSeed(1234U);

    if (QS_INIT((void *)0) == 0U) { /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
    QS_OBJ_DICTIONARY(&l_EINT0_IRQHandler);
    QS_USR_DICTIONARY(PHILO_STAT);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_SM_RECORDS); /* state machine records */
    QS_GLB_FILTER(QS_AO_RECORDS); /* active object records */
    QS_GLB_FILTER(QS_UA_RECORDS); /* all user records */
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    if (stat[0] == 'h') {
        LPC_GPIO1->FIOSET = LED_1;  /* turn LED on  */
    }
    else {
        LPC_GPIO1->FIOCLR = LED_1;  /* turn LED off */
    }
    if (stat[0] == 'e') {
        LPC_GPIO1->FIOSET = LED_2;  /* turn LED on  */
    }
    else {
        LPC_GPIO1->FIOCLR = LED_2;  /* turn LED off */
    }

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);                  /* Philosopher number */
        QS_STR(stat);                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    if (paused != 0U) {
        LPC_GPIO1->FIOSET = LED_3;  /* turn LED on  */
    }
    else {
        LPC_GPIO1->FIOCLR = LED_3;  /* turn LED off */
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


/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC);

    /* set priorities of ALL ISRs used in the system, see NOTE1
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    * Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
    * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
    */
    NVIC_SetPriority(EINT0_IRQn,     QF_AWARE_ISR_CMSIS_PRI);
    NVIC_SetPriority(SysTick_IRQn,   QF_AWARE_ISR_CMSIS_PRI + 1U);
    /* ... */

    /* enable IRQs in the NVIC... */
    NVIC_EnableIRQ(EINT0_IRQn);
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QK_onIdle(void) {
    /* toggle the User LED on and then off, see NOTE2 */
    QF_INT_DISABLE();
    LPC_GPIO1->FIOSET = LED_4;  /* turn LED on  */
    __NOP();   /* a couple of NOPs to actually see the LED glow */
    __NOP();
    __NOP();
    __NOP();
    LPC_GPIO1->FIOCLR = LED_4;  /* turn LED off */
    QF_INT_ENABLE();

#ifdef Q_SPY

    if ((LPC_UART0->LSR & 0x20U) != 0U) {  /* TX Holding Register empty? */
        uint16_t fifo = UART_TXFIFO_DEPTH; /* max bytes we can accept */
        uint8_t const *block;

        QF_INT_DISABLE();
        block = QS_getBlock(&fifo); /* try to get next block to transmit */
        QF_INT_ENABLE();

        while (fifo-- != 0) { /* any bytes in the block? */
            LPC_UART0->THR = *block++; /* put into the FIFO */
        }
    }

#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
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
    NVIC_SystemReset();
}

/* QS callbacks ============================================================*/
#ifdef Q_SPY

static void UART0_setBaudrate(uint32_t baud);  /* helper function */

/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[2*1024]; /* buffer for Quantum Spy */
    QS_initBuf(qsBuf, sizeof(qsBuf));

    // setup the P0_2 UART0 TX pin
    LPC_PINCON->PINSEL0  &= ~(3U << 4); /* clear P0_2 function */
    LPC_PINCON->PINSEL0  |=  (1U << 4); /* P0_2 to UART function (TX) */
    LPC_PINCON->PINMODE0 &= ~(3U << 4); /* P0_2 pull-up register */

    // setup the P0_3 UART0 RX pin
    LPC_PINCON->PINSEL0  &= ~(3U << 6); /* clear P0_3 function */
    LPC_PINCON->PINSEL0  |=  (1U << 6); /* P0_3 to UART function (RX) */
    LPC_PINCON->PINMODE0 &= ~(3U << 6); /* P0_3 pull-up register */

    /* enable power to UART0 */
    LPC_SC->PCONP |= (1U << 3);

    /* enable FIFOs and default RX trigger level */
    LPC_UART0->FCR =
        (1U << 0)    /* FIFO Enable - 0 = Disables, 1 = Enabled */
        | (0U << 1)  /* Rx Fifo Reset */
        | (0U << 2)  /* Tx Fifo Reset */
        | (0U << 6); /* Rx irq trig: 0=1char, 1=4chars, 2=8chars, 3=14chars */

    /* disable IRQs */
    LPC_UART0->IER =
        (0U << 0)    /* Rx Data available IRQ disable */
        | (0U << 1)  /* Tx Fifo empty IRQ disable     */
        | (0U << 2); /* Rx Line Status IRQ disable    */


    // set default baud rate
    UART0_setBaudrate(115200U);

    // format 8-data-bits, 1-stop-bit, parity-none
    LPC_UART0->LCR =
        (3U << 0)    /* 8-data-bits    */
        | (0U << 2)  /* 1 stop-bit     */
        | (0U << 3)  /* parity disable */
        | (0U << 4); /* parity none    */


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
    while ((b = QS_getByte()) != QS_EOD) {    /* while not End-Of-Data... */
        QF_INT_ENABLE();
        while ((LPC_UART0->LSR & 0x20U) == 0U) { /* while THR empty... */
        }
        LPC_UART0->THR = (b & 0xFFU);  /* put into the DR register */
        QF_INT_DISABLE();
    }
    QF_INT_ENABLE();
}


/*..........................................................................*/
/*
* Set the LPC UART0 barud-rate generator according to
* Section 14.4.12 in LPC176x Manual (document UM10360)
*/
static void UART0_setBaudrate(uint32_t baud) {

    /* First we check to see if the basic divide with no DivAddVal/MulVal
    * ratio gives us an integer result. If it does, we set DivAddVal = 0,
    * MulVal = 1. Otherwise, we search the valid ratio value range to find
    * the closest match. This could be more elegant, using search methods
    * and/or lookup tables, but the brute force method is not that much
    * slower, and is more maintainable.
    */
    uint32_t PCLK     = SystemCoreClock; /* divider /1 set below */
    uint16_t DL       = PCLK / (16U * baud);
    uint8_t DivAddVal = 0U;
    uint8_t MulVal    = 1U;

    /* set PCLK divider to 1 */
    LPC_SC->PCLKSEL0 &= ~(0x3U << 6); /* clear divider bits */
    LPC_SC->PCLKSEL0 |=  (0x1U << 6); /* set divider to 1 */

    if ((PCLK % (16U * baud)) != 0U) { /* non zero remainder? */
        uint32_t err_best = baud;
        bool found = false;
        uint32_t b;
        uint8_t mv;
        for (mv = 1U; mv < 16U && !found; mv++) {
            uint16_t dlv;
            uint8_t dav;
            for (dav = 0U; dav < mv; ++dav) {
                /*
                * baud = PCLK / (16 * dlv * (1 + (DivAdd / Mul))
                * solving for dlv, we get
                * dlv = mul * PCLK / (16 * baud * (divadd + mul))
                * mul has 4 bits, PCLK has 27 so we have 1 bit headroom,
                * which can be used for rounding for many values of mul
                * and PCLK we have 2 or more bits of headroom which can
                * be used to improve precision
                * note: X / 32 doesn't round correctly.
                * Instead, we use ((X / 16) + 1) / 2 for correct rounding
                */
                if ((mv*PCLK*2U) & 0x80000000U) { /* 1 bit headroom */
                    dlv = ((((2U*mv*PCLK) / (baud*(dav + mv)))/16U) + 1U)/2U;
                }
                else { /* 2 bits headroom, use more precision */
                    dlv = ((((4U*mv*PCLK) / (baud*(dav+mv)))/32U) + 1U)/2U;
                }

                /* datasheet says if DLL==DLM==0, then 1 is used instead */
                if (dlv == 0U) {
                    dlv = 1U;
                }
                /* datasheet says if dav > 0 then DL must be >= 2 */
                if ((dav > 0U) && (dlv < 2U)) {
                    dlv = 2U;
                }
                /* integer rearrangement of baud equation (with rounding) */
                b = ((PCLK*mv / (dlv*(dav + mv)*8U)) + 1U)/2U;
                b = (b >= baud) ? (b - baud) : (baud - b);

                /* check to see how we did */
                if (b < err_best) {
                    err_best  = b;
                    DL        = dlv;
                    MulVal    = mv;
                    DivAddVal = dav;

                    if (b == baud) {
                        found = true;
                        break;   /* break out of the inner for-loop */
                    }
                }
            }
        }
    }

    // set LCR[DLAB] to enable writing to divider registers
    LPC_UART0->LCR |= (1U << 7);

    // set divider values
    LPC_UART0->DLM = (DL >> 8) & 0xFFU;
    LPC_UART0->DLL = (DL >> 0) & 0xFFU;
    LPC_UART0->FDR = ((uint32_t)DivAddVal << 0)
                   | ((uint32_t)MulVal    << 4);

    // clear LCR[DLAB]
    LPC_UART0->LCR &= ~(1U << 7);
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
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
