/*****************************************************************************
* Product: QUTEST port for the EFM32-SLSTK3401A board
* Last updated for version 6.9.2a
* Last updated on  2021-01-28
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

#include "em_device.h"  /* the device specific header (SiLabs) */
#include "em_cmu.h"     /* Clock Management Unit (SiLabs) */
#include "em_gpio.h"    /* GPIO (SiLabs) */
#include "em_usart.h"   /* USART (SiLabs) */
/* add other drivers if necessary... */

//Q_DEFINE_THIS_MODULE("qutest_port")

/* ISRs defined in this BSP ------------------------------------------------*/
void USART0_RX_IRQHandler(void);

/* Local-scope objects -----------------------------------------------------*/
static USART_TypeDef * const l_USART0 = ((USART_TypeDef *)(0x40010000UL));

#define LED_PORT    gpioPortF
#define LED0_PIN    4
#define LED1_PIN    5

#define PB_PORT     gpioPortF
#define PB0_PIN     6
#define PB1_PIN     7

/*..........................................................................*/
/*
* ISR for receiving bytes from the QSPY Back-End
* NOTE: This ISR is "QF-unaware" meaning that it does not interact with
* the QF/QK and is not disabled. Such ISRs don't need to call QK_ISR_ENTRY/
* QK_ISR_EXIT and they cannot post or publish events.
*/
void USART0_RX_IRQHandler(void) {
    /* while RX FIFO NOT empty */
    while ((l_USART0->STATUS & USART_STATUS_RXDATAV) != 0) {
        uint32_t b = l_USART0->RXDATA;
        QS_RX_PUT(b);
    }
}

/* QS callbacks ============================================================*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsTxBuf[2*1024]; /* buffer for QS transmit channel */
    static uint8_t qsRxBuf[100];    /* buffer for QS receive channel */
    static USART_InitAsync_TypeDef init = {
        usartEnable,      /* Enable RX/TX when init completed */
        0,                /* Use current clock for configuring baudrate */
        115200,           /* 115200 bits/s */
        usartOVS16,       /* 16x oversampling */
        usartDatabits8,   /* 8 databits */
        usartNoParity,    /* No parity */
        usartStopbits1,   /* 1 stopbit */
        0,                /* Do not disable majority vote */
        0,                /* Not USART PRS input mode */
        usartPrsRxCh0,    /* PRS channel 0 */
        0,                /* Auto CS functionality enable/disable switch */
        0,                /* Auto CS Hold cycles */
        0                 /* Auto CS Setup cycles */
    };

    (void)arg; /* unused parameter */

    QS_initBuf  (qsTxBuf, sizeof(qsTxBuf));
    QS_rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    /* NOTE: SystemInit() already called from the startup code
    *  but SystemCoreClock needs to be updated
    */
    SystemCoreClockUpdate();

    /* configure the FPU usage by choosing one of the options... */
#if 0
    /* OPTION 1:
    * Use the automatic FPU state preservation and the FPU lazy stacking.
    *
    * NOTE:
    * Use the following setting when FPU is used in more than one task or
    * in any ISRs. This setting is the safest and recommended, but requires
    * extra stack space and CPU cycles.
    */
    FPU->FPCCR |= (1U << FPU_FPCCR_ASPEN_Pos) | (1U << FPU_FPCCR_LSPEN_Pos);
#else
    /* OPTION 2:
    * Do NOT to use the automatic FPU state preservation and
    * do NOT to use the FPU lazy stacking.
    *
    * NOTE:
    * Use the following setting when FPU is used in ONE task only and not
    * in any ISR. This setting is very efficient, but if more than one task
    * (or ISR) start using the FPU, this can lead to corruption of the
    * FPU registers. This option should be used with CAUTION.
    */
    FPU->FPCCR &= ~((1U << FPU_FPCCR_ASPEN_Pos)
                    | (1U << FPU_FPCCR_LSPEN_Pos));
#endif /* FPU */

    /* Enable peripheral clocks */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    /* To avoid false start, configure output as high */
    GPIO_PinModeSet(gpioPortA, 0, gpioModePushPull, 1); // TX pin
    GPIO_PinModeSet(gpioPortA, 1, gpioModeInput, 0);    // RX pin

    /* Enable DK RS232/UART switch */
    GPIO_PinModeSet(gpioPortA, 5, gpioModePushPull, 1);
    CMU_ClockEnable(cmuClock_USART0, true);

    /* configure the UART for the desired baud rate, 8-N-1 operation */
    init.enable = usartDisable;
    USART_InitAsync(l_USART0, &init);

    /* enable pins at correct UART/USART location. */
    l_USART0->ROUTEPEN = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
    l_USART0->ROUTELOC0 = (l_USART0->ROUTELOC0 &
                           ~(_USART_ROUTELOC0_TXLOC_MASK
                           | _USART_ROUTELOC0_RXLOC_MASK));

    /* enable the UART */
    USART_Enable(l_USART0, usartEnable);

    /* Clear previous RX interrupts */
    USART_IntClear(l_USART0, USART_IF_RXDATAV);
    NVIC_ClearPendingIRQ(USART0_RX_IRQn);

    /* Enable RX interrupts */
    USART_IntEnable(l_USART0, USART_IF_RXDATAV);

    /* explicitly set NVIC priorities of all Cortex-M interrupts used */
    NVIC_SetPriorityGrouping(0U);
    NVIC_SetPriority(USART0_RX_IRQn, 0U); /* kernel unaware interrupt */

    /* enable the UART RX interrupt... */
    NVIC_EnableIRQ(USART0_RX_IRQn); /* UART0 interrupt used for QS-RX */

    return 1U; /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    while ((b = QS_getByte()) != QS_EOD) { /* while not End-Of-Data... */
        GPIO->P[LED_PORT].DOUT |=  (1U << LED0_PIN);
        /* while TXE not empty */
        while ((l_USART0->STATUS & USART_STATUS_TXBL) == 0U) {
        }
        l_USART0->TXDATA  = (b & 0xFFU);  /* put into the DR register */
        GPIO->P[LED_PORT].DOUT &= ~(1U << LED0_PIN);
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

        /* turn the LED1 on and off (glow) */
        GPIO->P[LED_PORT].DOUT |=  (1U << LED1_PIN);
        GPIO->P[LED_PORT].DOUT &= ~(1U << LED1_PIN);

        QS_rxParse();  /* parse all the received bytes */

       if ((l_USART0->STATUS & USART_STATUS_TXBL) != 0) { /* is TXE empty? */
            uint16_t b = QS_getByte();
            if (b != QS_EOD) {  /* not End-Of-Data? */
                l_USART0->TXDATA = (b & 0xFFU); /* put into the DR register */
            }
        }
    }
    /* set inTestLoop to true in case calls to QS_onTestLoop() nest,
    * which can happen through the calls to QS_TEST_PAUSE().
    */
    QS_rxPriv_.inTestLoop = true;
}
