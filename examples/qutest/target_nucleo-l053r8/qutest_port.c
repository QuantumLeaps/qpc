/*****************************************************************************
* Product: QUTEST port for NUCLEO-L053R8 board
* Last updated for version 6.9.3
* Last updated on  2021-12-01
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2021 Quantum Leaps, LLC. All rights reserved.
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
#ifndef Q_SPY
    #error "Q_SPY must be defined to compile qutest_port.c"
#endif /* Q_SPY */

#define QP_IMPL       /* this is QP implementation */
#include "qf_port.h"  /* QF port */
#include "qassert.h"  /* QP embedded systems-friendly assertions */
#include "qs_port.h"  /* QS port */
#include "qs_pkg.h"   /* QS package-scope interface */

#include "stm32l0xx.h"  /* CMSIS-compliant header file for the MCU used */
/* add other drivers if necessary... */

//Q_DEFINE_THIS_MODULE("qutest_port")

/* ISRs defined in this BSP ------------------------------------------------*/
void USART2_IRQHandler(void);

/* Local-scope defines -----------------------------------------------------*/
/* LED pins available on the board (just one user LED LD2--Green on PA.5) */
#define LED_LD2  (1U << 5)

/* Button pins available on the board (just one user Button B1 on PC.13) */
#define BTN_B1   (1U << 13)

#define __DIV(__PCLK, __BAUD)       (((__PCLK / 4) *25)/(__BAUD))
#define __DIVMANT(__PCLK, __BAUD)   (__DIV(__PCLK, __BAUD)/100)
#define __DIVFRAQ(__PCLK, __BAUD)   \
    (((__DIV(__PCLK, __BAUD) - (__DIVMANT(__PCLK, __BAUD) * 100)) \
        * 16 + 50) / 100)
#define __USART_BRR(__PCLK, __BAUD) \
    ((__DIVMANT(__PCLK, __BAUD) << 4)|(__DIVFRAQ(__PCLK, __BAUD) & 0x0F))

/*..........................................................................*/
/*
* ISR for receiving bytes from the QSPY Back-End
* NOTE: This ISR is "QF-unaware" meaning that it does not interact with
* the QF/QK and is not disabled. Such ISRs don't need to call QK_ISR_ENTRY/
* QK_ISR_EXIT and they cannot post or publish events.
*/
void USART2_IRQHandler(void) { /* used in QS-RX (kernel UNAWARE interrutp) */
    /* is RX register NOT empty? */
    if ((USART2->ISR & (1U << 5)) != 0) {
        uint32_t b = USART2->RDR;
        QS_RX_PUT(b);
    }
}

/* QS callbacks ============================================================*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[2*1024]; /* buffer for Quantum Spy */
    static uint8_t qsRxBuf[128];  /* buffer for QS-RX channel */

    (void)arg; /* avoid the "unused parameter" compiler warning */

    QS_initBuf(qsBuf, sizeof(qsBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    /* NOTE: SystemInit() already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

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

    /* explicitly set NVIC priorities of all Cortex-M interrupts used */
    NVIC_SetPriorityGrouping(0U);
    NVIC_SetPriority(USART2_IRQn,    0U); /* kernel UNAWARE interrupt */

    /* enable the UART RX interrupt... */
    NVIC_EnableIRQ(USART2_IRQn); /* UART2 interrupt used for QS-RX */

    return 1U; /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;

    while ((b = QS_getByte()) != QS_EOD) {    /* while not End-Of-Data... */
        while ((USART2->ISR & (1U << 7)) == 0U) { /* while TXE not empty */
        }
        USART2->TDR  = (b & 0xFFU);  /* put into the DR register */
    }
}
/*..........................................................................*/
/*! callback function to reset the target (to be implemented in the BSP) */
void QS_onReset(void) {
    NVIC_SystemReset();
}
/*..........................................................................*/
void QS_onTestLoop() {
    QS_rxPriv_.inTestLoop = true;
    while (QS_rxPriv_.inTestLoop) {

        /* toggle an LED on and then off (not enough LEDs, see NOTE02) */
        GPIOA->BSRR |= (LED_LD2);        /* turn LED[n] on  */
        GPIOA->BSRR |= (LED_LD2 << 16);  /* turn LED[n] off */

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
    }
    /* set inTestLoop to true in case calls to QS_onTestLoop() nest,
    * which can happen through the calls to QS_TEST_PAUSE().
    */
    QS_rxPriv_.inTestLoop = true;
}
