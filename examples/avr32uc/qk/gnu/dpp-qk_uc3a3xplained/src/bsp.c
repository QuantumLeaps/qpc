/*****************************************************************************
* Product: Board Support Package for UC3-A3-Xplained board, QK kernel
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Sep 28, 2012
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
#include "dpp.h"
#include "bsp.h"

#include "sysclk.h"
#include "sys/sysregs.h"               /* for SYSREG_COUNT / SYSREG_COMPARE */
#include "board.h"
#include "tc.h"
#include "gpio.h"

Q_DEFINE_THIS_FILE

/* Local objects -----------------------------------------------------------*/
static uint32_t  l_rnd;                                      /* random seed */

#ifdef Q_SPY
    #include "usart.h"

    #define QS_USART               (&AVR32_USART1)
    #define QS_USART_RX_PIN        AVR32_USART1_RXD_0_0_PIN
    #define QS_USART_RX_FUNCTION   AVR32_USART1_RXD_0_0_FUNCTION
    #define QS_USART_TX_PIN        AVR32_USART1_TXD_0_0_PIN
    #define QS_USART_TX_FUNCTION   AVR32_USART1_TXD_0_0_FUNCTION
    #define QS_BUF_SIZE            512U
    #define QS_BAUD_RATE           115200U

    static uint32_t l_tickCount = 0U;
    static uint8_t const l_tickISR = 0U;
    static uint8_t const l_testISR = 0U;

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };
#endif

#define BSP_TICK_PERIOD (sysclk_get_pba_hz() / BSP_TICKS_PER_SEC)

/* ISRs --------------------------------------------------------------------*/
static void tickISR(void) {                                   /* see NOTE00 */
    /* write to COMPARE to clear the interrupt request */
    __builtin_mtsr(SYSREG_COMPARE, BSP_TICK_PERIOD);

#ifdef Q_SPY
    l_tickCount += BSP_TICK_PERIOD;
#endif

    QF_TICK(&l_tickISR);
}
/*..........................................................................*/
static void testISR(void) {                      /* for testing preemptions */
    tc_read_sr(&AVR32_TC0, 0);           /* clear int. source by reading SR */
    AVR32_TC0.channel[0].ccr = AVR32_TC_CCR0_CLKDIS_MASK;    /* disable clk */

    QACTIVE_POST(AO_Table, Q_NEW(QEvt, MAX_PUB_SIG),      /* for testing... */
                 &l_testISR);
}

/*..........................................................................*/
void BSP_init(void) {
    sysclk_init();      /* disable all clocks, except those in conf_clock.h */

    /* enable clock for AVR32_TC0, which is used for testing */
    sysclk_enable_peripheral_clock(&AVR32_TC0);

    BSP_randomSeed(1234U);

    Q_ALLEGE(QS_INIT((void *)0) != 0);/* initialize the QS software tracing */
    QS_RESET();
    QS_OBJ_DICTIONARY(&l_tickISR);
    QS_OBJ_DICTIONARY(&l_testISR);
    QS_USR_DICTIONARY(QS_USER);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char_t const *stat) {
    static uint32_t const leds[] = { LED2, LED3, LED1 };
    if (n < Q_DIM(leds)) { /* show the Philo status only for available LEDs */
        if (stat[0] == (uint8_t)'e') {       /* is this Philosopher eating? */
            LED_On(leds[n]);
        }
        else {                            /* this Philosopher is not eating */
            LED_Off(leds[n]);
        }
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
    l_rnd = l_rnd * (3*7*11*13*23);
    return l_rnd >> 8;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    QF_INT_DISABLE();
    LED_On(LED0 | LED1 | LED2 | LED3);                       /* all LEDs on */
    for (;;) {                            /* hang here in the for-ever loop */
    }
}

/*..........................................................................*/
void QF_onStartup(void) {
    INTC_init_interrupts();                 /* initialize interrupt vectors */
                                 /* set the tickISR IRQ number and priority */
    INTC_register_interrupt(&tickISR,
                            AVR32_CORE_COMPARE_IRQ, AVR32_INTC_INT0);

    /* configure TC0 interrupt used for testing... */
    INTC_register_interrupt(&testISR, AVR32_TC0_IRQ0, AVR32_INTC_INT1);

    /* enable compare register and pick clock 5 as input */
    AVR32_TC0.channel[0].cmr =
        (TC_TRIGGER_COMPARE_RC << AVR32_TC_CMR0_CPCTRG_OFFSET) |
        (AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK5 << AVR32_TC_CMR0_TCCLKS_OFFSET);

    /* set the compare counter to 1 */
    AVR32_TC0.channel[0].rc = 1;

    /* enable the compare interrupt */
    AVR32_TC0.channel[0].ier = AVR32_TC_IER0_CPCS_MASK;

    /* enable clock and start counting (will be done from the debugger) */
    //AVR32_TC0.channel[0].ccr = AVR32_TC_CCR0_CLKEN_MASK
    //                           | AVR32_TC_CCR0_SWTRG_MASK;


    /* setup the COUNT/COMPARE registers to generate system clock tick */
    __builtin_mtsr(SYSREG_COUNT,   0U);
    __builtin_mtsr(SYSREG_COMPARE, BSP_TICK_PERIOD);
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QK_onIdle() {

    QF_INT_DISABLE();
    LED_On (LED0);           /* toggle the LED0 on and then off, see NOTE01 */
    LED_Off(LED0);
    QF_INT_ENABLE();

#ifdef Q_SPY                     /* use the idle cycles for QS transmission */

    if ((QS_USART->csr & AVR32_USART_CSR_TXRDY_MASK) != 0U) {     /* ready? */
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {
            QS_USART->thr = (b << AVR32_USART_THR_TXCHR_OFFSET)
                            & AVR32_USART_THR_TXCHR_MASK;
        }
    }

#elif defined NDEBUG
    /* go to sleep and re-enable interrupts at the same time, see NOTE02 */
    __asm__ __volatile__ ("sleep 128");

#endif
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE];            /* buffer for Quantum Spy */
    static gpio_map_t const usart_gpio_map = {
       { QS_USART_RX_PIN, QS_USART_RX_FUNCTION },
       { QS_USART_TX_PIN, QS_USART_TX_FUNCTION }
    };
    usart_options_t const usart_options =    {
       .baudrate    = QS_BAUD_RATE,
       .charlength  = 8,
       .paritytype  = USART_NO_PARITY,
       .stopbits    = USART_1_STOPBIT,
       .channelmode = USART_NORMAL_CHMODE
    };

    QS_initBuf(qsBuf, sizeof(qsBuf));

    sysclk_enable_peripheral_clock(QS_USART);

    /* Setup GPIO for the QS USART */
    gpio_enable_module(usart_gpio_map, Q_DIM(usart_gpio_map));

    /* Initialize the QS USART in RS232 mode */
    usart_init_rs232(QS_USART, &usart_options, sysclk_get_pba_hz());

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

    return (uint8_t)1;             /* indicate successful QS initialization */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    while ((b = QS_getByte()) != QS_EOD) { /* next QS trace byte available? */
        while ((QS_USART->csr & AVR32_USART_CSR_TXRDY_MASK) == 0U) { /*busy?*/
        }
        QS_USART->thr = (b << AVR32_USART_THR_TXCHR_OFFSET)
                        & AVR32_USART_THR_TXCHR_MASK;
    }
}
/*..........................................................................*/
/* NOTE: invoked within a critical section (interrupts disabled) */
QSTimeCtr QS_onGetTime(void) {
    return (QSTimeCtr)(__builtin_mfsr(SYSREG_COUNT) + l_tickCount);
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE00:
* The user-defined interrupt service routines (ISRs) are plain C functions.
* Specifically, they are *NOT* __attribute__((__interrupt__)) type functions.
*
* NOTE01:
* The LED0 is used to visualize the idle loop activity. The brightness of
* the LED is proportional to the frequency of invocations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*
* NOTE02:
* The instruction sleep 128 puts the CPU to sleep and clears the SR[GM] flag
* at the same time.
*/

