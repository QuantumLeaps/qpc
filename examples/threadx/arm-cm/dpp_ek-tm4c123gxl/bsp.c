/*****************************************************************************
* Product: DPP example, EK-TM4C123GXL board, ThreadX kernel
* Last updated for version 7.1.3
* Last updated on  2022-10-18
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

#include "TM4C123GH6PM.h"        /* the device specific header (TI) */
#include "rom.h"                 /* the built-in ROM functions (TI) */
#include "sysctl.h"              /* system control driver (TI) */
#include "gpio.h"                /* GPIO driver (TI) */
/* add other drivers if necessary... */

Q_DEFINE_THIS_FILE  /* define the name of this file for assertions */

/* prototypes of ISRs defined in this BSP ----------------------------------*/
void UART0_IRQHandler(void);

/* Local-scope objects -----------------------------------------------------*/
#define LED_RED     (1U << 1)
#define LED_GREEN   (1U << 3)
#define LED_BLUE    (1U << 2)

#define BTN_SW1     (1U << 4)
#define BTN_SW2     (1U << 0)

static uint32_t l_rnd;  /* random seed */
static TX_TIMER l_tick_timer; /* ThreadX timer to call QTIMEEVT_TICK_X() */

#ifdef Q_SPY

    /* QSpy source IDs */
    static QSpyId const l_clock_tick = { QS_AP_ID };

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

/* ISRs used in the application ==========================================*/

#ifdef Q_SPY
/*
* ISR for receiving bytes from the QSPY Back-End
* NOTE: This ISR is "QF-unaware" meaning that it does not interact with
* the QF/QK and is not disabled. Such ISRs don't need to call QK_ISR_ENTRY/
* QK_ISR_EXIT and they cannot post or publish events.
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
    /* NOTE: SystemInit() already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    /* Explictily Disable the automatic FPU state preservation as well as
    * the FPU lazy stacking
    */
    FPU->FPCCR &= ~((1U << FPU_FPCCR_ASPEN_Pos) | (1U << FPU_FPCCR_LSPEN_Pos));

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

    /* seed the random number generator */
    BSP_randomSeed(1234U);

    /* initialize the QS software tracing... */
    if (QS_INIT((void *)0) == 0U) {
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_clock_tick);
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(PAUSED_STAT);
    QS_USR_DICTIONARY(COMMAND_STAT);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_ALL_RECORDS); /* all records */
    QS_GLB_FILTER(-QS_QF_TICK);    /* exclude the clock tick */
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    GPIOF->DATA_Bits[LED_GREEN] = ((stat[0] == 'e') ? LED_GREEN : 0U);

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    GPIOF->DATA_Bits[LED_BLUE] = ((paused != 0U) ? LED_BLUE : 0U);

    QS_BEGIN_ID(PAUSED_STAT, 0U) /* app-specific record */
        QS_U8(1, paused);  /* Paused status */
    QS_END()
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    /* Some flating point code is to exercise the VFP... */
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

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


#ifdef Q_SPY
    /* ThreadX "idle" thread for QS output, see NOTE1 */
    static TX_THREAD idle_thread;
    static void idle_thread_fun(ULONG thread_input);
    static ULONG idle_thread_stack[64];
#endif

/* QF callbacks ============================================================*/
static VOID timer_expiration(ULONG id) {
    QTIMEEVT_TICK_X(id, &l_clock_tick); /* perform the QF clock tick processing */
}
/*..........................................................................*/
void QF_onStartup(void) {
    /*
    * NOTE:
    * This application uses the ThreadX timer to periodically call
    * the QTimeEvt_tick_(0) function. Here, only the clock tick rate of 0
    * is used, but other timers can be used to call QTimeEvt_tick_() for
    * other clock tick rates, if needed.
    *
    * The choice of a ThreadX timer is not the only option. Applications
    * might choose to call QTIMEEVT_TICK_X() directly from timer interrupts
    * or from active object(s).
    */
    Q_ALLEGE(tx_timer_create(&l_tick_timer, /* ThreadX timer object */
        (CHAR *)"QP-tick", /* name of the timer */
        &timer_expiration, /* expiration function */
        0U,       /* expiration function input (tick rate) */
        1U,       /* initial ticks */
        1U,       /* reschedule ticks */
        TX_AUTO_ACTIVATE) /* automatically activate timer */
             == TX_SUCCESS);

#ifdef Q_SPY
    NVIC_EnableIRQ(UART0_IRQn);  /* UART0 interrupt used for QS-RX */

    /* start a ThreadX "idle" thread. See NOTE1... */
    Q_ALLEGE(tx_thread_create(&idle_thread, /* thread control block */
        (CHAR *)("idle"), /* thread name */
        &idle_thread_fun,       /* thread function */
        0LU,                    /* thread input (unsued) */
        idle_thread_stack,       /* stack start */
        sizeof(idle_thread_stack), /* stack size in bytes */
        TX_MAX_PRIORITIES - 1U, /* ThreadX priority (LOWEST possible), NOTE1 */
        TX_MAX_PRIORITIES - 1U, /* preemption threshold disabled */
        TX_NO_TIME_SLICE,
        TX_AUTO_START)
             == TX_SUCCESS);
#endif /* Q_SPY */
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

#ifndef NDEBUG
    /* light up all LEDs */
    GPIOF->DATA_Bits[LED_GREEN | LED_RED | LED_BLUE] = 0xFFU;
    /* for debugging, hang on in an endless loop... */
    for (;;) {
    }
#endif

    NVIC_SystemReset();
}

/* QS callbacks ============================================================*/
#ifdef Q_SPY

/*..........................................................................*/
static void idle_thread_fun(ULONG thread_input) { /* see NOTE1 */
    for (;;) {
        QS_rxParse();  /* parse all the received bytes */

        if ((UART0->FR & UART_FR_TXFE) != 0U) {  /* TX done? */
            uint16_t fifo = UART_TXFIFO_DEPTH;   /* max bytes we can accept */
            uint8_t const *block;
            QF_CRIT_STAT_TYPE intStat;

            QF_CRIT_ENTRY(intStat);
            block = QS_getBlock(&fifo); /* try to get next block to transmit */
            QF_CRIT_EXIT(intStat);

            while (fifo-- != 0U) {       /* any bytes in the block? */
                UART0->DR = *block++;    /* put into the FIFO */
            }
        }

        /* no blocking in this "idle" thread; see NOTE1 */
    }
}

/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsTxBuf[2*1024]; /* buffer for QS transmit channel */
    static uint8_t qsRxBuf[100];    /* buffer for QS receive channel */

    QS_initBuf  (qsTxBuf, sizeof(qsTxBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    /* enable clock for UART0 and GPIOA (used by UART0 pins) */
    SYSCTL->RCGCUART |= (1U << 0); /* enable Run mode for UART0 */
    SYSCTL->RCGCGPIO |= (1U << 0); /* enable Run mode for GPIOA */

    /* configure UART0 pins for UART operation */
    uint32_t tmp = (1U << 0) | (1U << 1);
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

    /* configure TIMER5 to produce QS time stamp */
    SYSCTL->RCGCTIMER |= (1U << 5);  /* enable run mode for Timer5 */
    TIMER5->CTL  = 0U;               /* disable Timer1 output */
    TIMER5->CFG  = 0x0U;             /* 32-bit configuration */
    TIMER5->TAMR = (1U << 4) | 0x02; /* up-counting periodic mode */
    TIMER5->TAILR= 0xFFFFFFFFU;      /* timer interval */
    TIMER5->ICR  = 0x1U;             /* TimerA timeout flag bit clears*/
    TIMER5->CTL |= (1U << 0);        /* enable TimerA module */

    return 1U; /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {  /* NOTE: invoked with interrupts DISABLED */
    return TIMER5->TAV;
}
/*..........................................................................*/
void QS_onFlush(void) {
    while (true) {
        /* try to get next byte to transmit */
        QF_CRIT_STAT_TYPE intStat;

        QF_CRIT_ENTRY(intStat);
        uint16_t b = QS_getByte();
        QF_CRIT_EXIT(intStat);

        if (b != QS_EOD) { /* NOT end-of-data */
            /* busy-wait as long as TX FIFO has data to transmit */
            while ((UART0->FR & UART_FR_TXFE) == 0) {
            }
            /* place the byte in the UART DR register */
            UART0->DR = b;
        }
        else {
            break; /* break out of the loop */
        }
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
    QS_BEGIN_ID(COMMAND_STAT, 0U) /* app-specific record */
        QS_U8(2, cmdId);
        QS_U32(8, param1);
        QS_U32(8, param2);
        QS_U32(8, param3);
    QS_END()
}

#endif /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*============================================================================
* NOTE1:
* ThreadX apparently does not have a concpet of an "idle" thread, but
* it can be emulated by a regular, but NON-BLOCKING ThreadX thread of
* the lowest priority.
*
* In the Q_SPY configuration, this "idle" thread is uded to perform
* the QS data output to the host. This is not the only choice available, and
* other applications might choose to peform the QS output some other way.
*/

