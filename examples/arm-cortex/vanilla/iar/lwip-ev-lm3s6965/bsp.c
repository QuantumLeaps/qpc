/*****************************************************************************
* Product: DPP with lwIP application
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
#include "qp_port.h"                                 /* QP port header file */
#include "dpp.h"                   /* application events and active objects */
#include "bsp.h"                       /* Board Support Package header file */

#include "lm3s_cmsis.h"

Q_DEFINE_THIS_FILE

#define USER_LED           (1 << 0)
#define USER_BTN           (1 << 1)

#define ETH0_LED           (1 << 3)
#define ETH1_LED           (1 << 2)

static uint32_t l_nTicks;

enum ISR_Priorities {   /* ISR priorities starting from the highest urgency */
    SYSTICK_PRIO,
    ETHERNET_PRIO,
    /* ... */
};

#ifdef Q_SPY

    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;
    static uint8_t l_SysTick_Handler;

    #define UART_BAUD_RATE      115200
    #define UART_TXFIFO_DEPTH   16
    #define UART_FR_TXFE        (1 << 7)

#endif

/*..........................................................................*/
void SysTick_Handler(void) {
    static uint32_t btn_debounced  = 0;
    static uint8_t  debounce_state = 0;
    uint32_t volatile tmp;

    ++l_nTicks;                          /* count the number of clock ticks */

#ifdef Q_SPY
    tmp = SysTick->CTRL;         /* clear SysTick_CTRL_COUNTFLAG in SysTick */
    QS_tickTime_ += QS_tickPeriod_;       /* account for the clock rollover */
#endif

    QF_TICK(&l_SysTick_Handler);           /* process all armed time events */

    tmp = GPIOF->DATA_Bits[USER_BTN];               /* read the User Button */
    switch (debounce_state) {
        case 0:
            if (tmp != btn_debounced) {
                debounce_state = 1;         /* transition to the next state */
            }
            break;
        case 1:
            if (tmp != btn_debounced) {
                debounce_state = 2;         /* transition to the next state */
            }
            else {
                debounce_state = 0;           /* transition back to state 0 */
            }
            break;
        case 2:
            if (tmp != btn_debounced) {
                debounce_state = 3;         /* transition to the next state */
            }
            else {
                debounce_state = 0;           /* transition back to state 0 */
            }
            break;
        case 3:
            if (tmp != btn_debounced) {
                btn_debounced = tmp;     /* save the debounced button value */
                if (tmp == 0) {                 /* is the button depressed? */
                    static QEvt const bd = { BTN_DOWN_SIG, 0 };
                    QF_PUBLISH(&bd, &l_SysTick_Handler);
                }
                else {
                    static QEvt const bu = { BTN_UP_SIG, 0 };
                    QF_PUBLISH(&bu, &l_SysTick_Handler);
                }
            }
            debounce_state = 0;               /* transition back to state 0 */
            break;
    }
}

/*..........................................................................*/
void BSP_init(void) {
    /* set the system clock as specified in lm3s_config.h (20MHz from PLL)  */
    SystemInit();

    SYSCTL->RCGC2 |= (1 << 5);  /* enable clock to GPIOF (User and Eth LEDs)*/
    __NOP();
    __NOP();
                              /* configure the pin driving the Ethernet LED */
    GPIOF->DIR   &= ~(ETH0_LED | ETH1_LED);      /* set direction: hardware */
    GPIOF->AFSEL |=  (ETH0_LED | ETH1_LED);
    GPIOF->DR2R  |=  (ETH0_LED | ETH1_LED);
    GPIOF->ODR   &= ~(ETH0_LED | ETH1_LED);
    GPIOF->PUR   |=  (ETH0_LED | ETH1_LED);
    GPIOF->PDR   &= ~(ETH0_LED | ETH1_LED);
    GPIOF->DEN   |=  (ETH0_LED | ETH1_LED);
    GPIOF->AMSEL &= ~(ETH0_LED | ETH1_LED);

                                  /* configure the pin driving the User LED */
    GPIOF->DIR   |=  USER_LED;                     /* set direction: output */
    GPIOF->DR2R  |=  USER_LED;
    GPIOF->DEN   |=  USER_LED;
    GPIOF->AMSEL &= ~USER_LED;
    GPIOF->DATA_Bits[USER_LED] = 0;                     /* turn the LED off */

                              /* configure the pin connected to the Buttons */
    GPIOF->DIR   &= ~USER_BTN;                      /* set direction: input */
    GPIOF->DR2R  |=  USER_BTN;
    GPIOF->ODR   &= ~USER_BTN;
    GPIOF->PUR   |=  USER_BTN;
    GPIOF->PDR   &= ~USER_BTN;
    GPIOF->DEN   |=  USER_BTN;
    GPIOF->AMSEL &= ~USER_BTN;

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }

    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
}
/*..........................................................................*/
void QF_onStartup(void) {
              /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(SystemFrequency / BSP_TICKS_PER_SEC);

                       /* set priorities of all interrupts in the system... */
    NVIC_SetPriority(SysTick_IRQn,   SYSTICK_PRIO);
    NVIC_SetPriority(Ethernet_IRQn,  ETHERNET_PRIO);

    NVIC_EnableIRQ(Ethernet_IRQn);         /* enable the Ethernet Interrupt */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QF_onIdle(void) {        /* entered with interrupts LOCKED, see NOTE01 */

    /* toggle the User LED on and then off, see NOTE02 */
    GPIOF->DATA_Bits[USER_LED] = USER_LED;         /* turn the User LED on  */
    GPIOF->DATA_Bits[USER_LED] = 0;                /* turn the User LED off */

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
    * see the datasheet for your particular MCU.
    */
    __WFI();                                          /* Wait-For-Interrupt */
    QF_INT_ENABLE();
#else
    QF_INT_ENABLE();
#endif
}

/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_DISABLE();         /* make sure that all interrupts are disabled */
    for (;;) {       /* NOTE: replace the loop with reset for final version */
    }
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
    GPIOA->AFSEL |=  tmp;
    GPIOA->DR2R  |=  tmp;       /* set 2mA drive, DR4R and DR8R are cleared */
    GPIOA->SLR   &= ~tmp;
    GPIOA->ODR   &= ~tmp;
    GPIOA->PUR   &= ~tmp;
    GPIOA->PDR   &= ~tmp;
    GPIOA->DEN   |=  tmp;
    GPIOA->AMSEL &= ~tmp;

           /* configure the UART for the desired baud rate, 8-N-1 operation */
    tmp = (((SystemFrequency * 8) / UART_BAUD_RATE) + 1) / 2;
    UART0->IBRD   = tmp / 64;
    UART0->FBRD   = tmp % 64;
    UART0->LCRH   = 0x60;                      /* configure 8-bit operation */
    UART0->LCRH  |= 0x10;                                   /* enable FIFOs */
    UART0->CTL   |= (1 << 0) | (1 << 8) | (1 << 9);

    QS_tickPeriod_ = SystemFrequency / BSP_TICKS_PER_SEC;
    QS_tickTime_ = QS_tickPeriod_;        /* to start the timestamp at zero */

                                                 /* setup the QS filters... */
    QS_FILTER_ON(QS_ALL_RECORDS);

//    QS_FILTER_OFF(QS_QEP_STATE_EMPTY);
//    QS_FILTER_OFF(QS_QEP_STATE_ENTRY);
//    QS_FILTER_OFF(QS_QEP_STATE_EXIT);
//    QS_FILTER_OFF(QS_QEP_STATE_INIT);
//    QS_FILTER_OFF(QS_QEP_INIT_TRAN);
//    QS_FILTER_OFF(QS_QEP_INTERN_TRAN);
//    QS_FILTER_OFF(QS_QEP_TRAN);
//    QS_FILTER_OFF(QS_QEP_IGNORED);

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
* NOTE01:
* The QF_onIdle() callback is called with interrupts locked, because the
* determination of the idle condition might change by any interrupt posting
* an event. QF::onIdle() must internally unlock interrupts, ideally
* atomically with putting the CPU to the power-saving mode.
*
* NOTE02:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
