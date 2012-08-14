/*****************************************************************************
* Product: BSP for MCF52223-SK board, with the Vanilla kernel
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Apr 24, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "dpp.h"
#include "bsp.h"

#include <io52223.h>                                        /* MCF52223 MCU */
#include "intr_handlers.h"
//#include "icm_drv_stat.h"

Q_DEFINE_THIS_FILE

/* Local-scope objects -----------------------------------------------------*/
static uint32_t l_delay = 0UL; /* limit for the loop counter in busyDelay() */

#define SYS_CLK           48000000L
#define PER_CLK           (SYS_CLK/2)

#define AN_TR_PIN         MCF_GPIO_PORTAN_PORTAN0
#define AN_TR_CHANNEL     0

                                                       /* interrupt IDs ... */
#define INTR_ID_EPF1      ( 1+64)
#define INTR_ID_EPF2      ( 2+64)
#define INTR_ID_EPF3      ( 3+64)
#define INTR_ID_EPF4      ( 4+64)
#define INTR_ID_EPF5      ( 5+64)
#define INTR_ID_EPF6      ( 6+64)
#define INTR_ID_EPF7      ( 7+64)
#define INTR_ID_SWT       ( 8+64)
#define INTR_ID_DMA0      ( 9+64)
#define INTR_ID_DMA1      (10+64)
#define INTR_ID_DMA2      (11+64)
#define INTR_ID_DMA3      (12+64)
#define INTR_ID_UART0     (13+64)
#define INTR_ID_UART1     (14+64)
#define INTR_ID_UART2     (15+64)
#define INTR_ID_I2S       (17+64)
#define INTR_ID_QSPI      (18+64)
#define INTR_ID_DTIM0     (19+64)
#define INTR_ID_DTIM1     (20+64)
#define INTR_ID_DTIM2     (21+64)
#define INTR_ID_DTIM3     (22+64)
#define INTR_ID_GPT_OVF   (41+64)
#define INTR_ID_GPT_PAI   (42+64)
#define INTR_ID_GPT_PAOVF (43+64)
#define INTR_ID_GPT_C0    (44+64)
#define INTR_ID_GPT_C1    (45+64)
#define INTR_ID_GPT_C2    (46+64)
#define INTR_ID_GPT_C3    (47+64)
#define INTR_ID_GPT_LVD   (48+64)
#define INTR_ID_ADC_A     (49+64)
#define INTR_ID_ADC_B     (50+64)
#define INTR_ID_ADC       (51+64)
#define INTR_ID_PWM       (52+64)
#define INTR_ID_USB       (53+64)
#define INTR_ID_PIT0      (55+64)
#define INTR_ID_PIT1      (56+64)
#define INTR_ID_CFM_BE    (59+64)
#define INTR_ID_CFM_CC    (60+64)
#define INTR_ID_CFM_PV    (61+64)
#define INTR_ID_CFM_AE    (62+64)
#define INTR_ID_RTC       (63+64)

#define PIT_PERIOD        (PER_CLK / (BSP_TICKS_PER_SEC * 16))
#define PIT0_LEVEL        2

#ifdef Q_SPY
    QSTimeCtr QS_tickTime;

    #define UART_BAUD  115200

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };
#endif

/*..........................................................................*/
__interrupt void PIT0_IntrHandler(void) {
    MCF_PIT0_PCSR |= MCF_PIT_PCSR_PIF;        /* clear the interrupt source */
#ifdef Q_SPY
    QS_tickTime += PIT_PERIOD;                          /* add the rollover */
#endif
    QF_tick();                             /* process all armed time events */
}

/*..........................................................................*/
void BSP_init(int argc, char *argv[]) {
    (void)argc;                       /* unused: avoid the complier warning */
    (void)argv;                       /* unused: avoid the compiler warning */

                                 /* initialize the Interrupt Control Module */
    MCF_SCM_PPMRC = MCF_SCM_PPMRC_ENABLE_INTC;          /* enable ICM clock */
    MCF_INTC0_IMRH = MCF_INTC0_IMRL = 0xFFFFFFFF;    /* mask all interrupts */
    MCF_INTC0_INTFRCH = MCF_INTC0_INTFRCL = 0;  /* clear  forced interrupts */

                                            /* initialize the LEDs lines... */
    MCF_GPIO_PANPAR &= ~0xFE;
    MCF_GPIO_SETAN   =  0xFE;
    MCF_GPIO_DDRAN  |=  0xFE;
    MCF_GPIO_PDDPAR &= ~0xFF;
    MCF_GPIO_SETDD   =  0xFF;
    MCF_GPIO_DDRDD  |=  0xFF;
    MCF_GPIO_PTAPAR &= ~0x02;
    MCF_GPIO_SETTA   =  0x02;
    MCF_GPIO_DDRTA  |=  0x02;
                                            /* configure the low-power mode */
    MCF_PMM_LPCR  = MCF_PMM_LPCR_LPMD_DOZE;
    MCF_PMM_LPICR = MCF_PMM_LPICR_XLPM_IPL(0);    /* any interrupt wakes up */

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
}
/*..........................................................................*/
void BSP_displyPhilStat(uint8_t n, char const *stat) {
    if (stat[0] == 'e') {
        MCF_GPIO_CLRAN = ~(2UL << n);
    }
    else {
        MCF_GPIO_SETAN = (2UL << n);
    }

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_busyDelay(void) {
    uint32_t volatile i = l_delay;
    while (i-- > 0UL) {                                   /* busy-wait loop */
    }
}

/*..........................................................................*/
void QF_onStartup(void) {
    MCF_SCM_PPMRC = MCF_SCM_PPMRC_ENABLE_PIT0;         /* enable PIT0 clock */
                                 /* configure PIT0 Ctrl and status register */
    MCF_PIT0_PCSR = MCF_PIT_PCSR_PRE(4)                   /* prescaler 1/16 */
                  | MCF_PIT_PCSR_RLD                       /* enable reload */
                  | MCF_PIT_PCSR_PIF             /* clear pending interrupt */
                  | MCF_PIT_PCSR_PIE                    /* enable interrupt */
                  | MCF_PIT_PCSR_OVW                    /* enable overwrite */
                  | MCF_PIT_PCSR_DBG;                  /* enable debug mode */

    MCF_PIT0_PMR  = PIT_PERIOD - 1;     /* set the PMR for the desired rate */
                                                /* configure PIT0 interrupt */
    *(&MCF_INTC0_ICR01 + INTR_ID_PIT0 - 64 - 1) = (PIT0_LEVEL << 3);
                                                   /* enable PIT0 interrupt */
    MCF_INTC0_IMRH &= ~(1UL << (INTR_ID_PIT0 & 0x1F));

    MCF_PIT0_PCSR |= MCF_PIT_PCSR_EN;               /* enable PIC0 counting */
    __enable_interrupts();                      /* Global interrupts enable */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QF_onIdle(QF_INT_KEY_TYPE key) {          /* interrupts LOCKED, NOTE01 */

    /* toggle the LED-16 on and then off, see NOTE02 */
    MCF_GPIO_CLRTA = ~0x02;
    MCF_GPIO_SETTA = 0x02;

#ifdef Q_SPY
    QF_INT_UNLOCK(key);

    if ((MCF_DMA3_BCR & 0x00FFFFFF) == 0) {       /* DMA byte counter zero? */
        uint16_t len = 0xFFFF;         /* request as many bytes as possible */
        uint8_t const *block;
        QF_INT_LOCK(key);
        block = QS_getBlock(&len);     /* try to get next block to transmit */
        QF_INT_UNLOCK(key);

        if (block != (uint8_t *)0) {
            MCF_DMA3_DCR = 0x00;         /* clear the DCR to avoid CE error */
            MCF_DMA3_DSR = 0x01;                           /* reset the DSR */
            MCF_DMA3_SAR = (uint32_t)block;               /* set the source */
            MCF_DMA3_DAR = (uint32_t)&MCF_UART_UTB(2); /* destination UART2 */
            MCF_DMA3_BCR = MCF_DMA_BCR_BCR(len);            /* byte counter */

            /* external Request, cycle Steal, dest & source = 8 bit
            * dest address NOT incremented source incremented by 1
            */
            MCF_DMA3_DCR = MCF_DMA_DCR_EEXT
                           | MCF_DMA_DCR_CS
                           | MCF_DMA_DCR_SINC
                           | MCF_DMA_DCR_SSIZE(01)
                           | MCF_DMA_DCR_DSIZE(01);
        }
    }
    else {
        QF_INT_UNLOCK(key);                            /* unlock interrupts */
    }

#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the ColdFire Reference Manual for your particular device.
    */
    __stop(0x2000);                          /* privileged STOP instruction */
#else
    QF_INT_UNLOCK(key);                            /* unlock the interrupts */
#endif
}

/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    __disable_interrupts();   /* make sure that all interrupts are disabled */
    for (;;) {       /* NOTE: replace the loop with reset for final version */
    }
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[2*1024];                 /* buffer for Quantum Spy */
    extern uint32_t __RAMBEGIN;

    QS_initBuf(qsBuf, sizeof(qsBuf));

                                         /* set port UC to initialize UTXD2 */
    MCF_GPIO_PUCPAR = MCF_GPIO_PUCPAR_UTXD2_UTXD2;      /* config UTXD2 pin */
    MCF_UART2_UCR   = MCF_UART_UCR_RESET_TX;           /* reset Transmitter */
    MCF_UART2_UCR   = MCF_UART_UCR_RESET_MR;     /* reset the Mode Register */
              /* config no parity, 8-bits, no echo, no loopback, 1 stop bit */
    MCF_UART2_UMR1  = MCF_UART_UMR_PM_NONE | MCF_UART_UMR_BC_8
                      | MCF_UART_UMR_CM_NORMAL | MCF_UART_UMR_SB_STOP_BITS_1;
                    /* set the Tx baud rate generator from the SYSTEM CLOCK */
    MCF_UART2_UCSR  = MCF_UART_UCSR_TCS_SYS_CLK;
    MCF_UART2_UBG1  = (uint8_t)((SYS_CLK / (UART_BAUD * 32)) >> 8);
    MCF_UART2_UBG2  = (uint8_t)(SYS_CLK / (UART_BAUD * 32));

    MCF_UART2_UCR   = MCF_UART_UCR_TX_ENABLED;        /* enable transmitter */

                                /* set BDE for DMA access in MCF_SCM_RAMBAR */
    MCF_SCM_RAMBAR  = (uint32_t)&__RAMBEGIN | (MCF_SCM_RAMBAR_BDE | 0x1);
    MCF_SCM_MPR    |= 0x04;   /* Master Privilege Register bit-2 DMA master */
    MCF_SCM_DMAREQC|= MCF_SCM_DMAREQC_DMAC3(0x0E);     /* DMA3 for UART2 TX */

    MCF_DMA3_DCR = 0x00;                 /* clear the DCR to avoid CE error */
    MCF_DMA3_DSR = 0x01;                                   /* reset the DSR */

    QS_tickTime  = PIT_PERIOD - 1;        /* to start the timestamp at zero */

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
    QS_FILTER_OFF(QS_QF_INT_LOCK);
    QS_FILTER_OFF(QS_QF_INT_UNLOCK);
    QS_FILTER_OFF(QS_QF_ISR_ENTRY);
    QS_FILTER_OFF(QS_QF_ISR_EXIT);

    return (uint8_t)1;                                    /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {            /* invoked with interrupts locked */
    if ((MCF_PIT0_PCSR & MCF_PIT_PCSR_PIF) == 0) {          /* no rollover? */
        return QS_tickTime - (QSTimeCtr)MCF_PIT0_PCNTR;
    }
    else {  /* the rollover occured, but PIT0_IntrHandler() did not run yet */
        return QS_tickTime + PIT_PERIOD
               - (QSTimeCtr)(QSTimeCtr)MCF_PIT0_PCNTR;
    }
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t len = 0xFFFF;                   /* request as much as possible */
    uint8_t const *block;
    QF_INT_KEY_TYPE intKeyLock;
    QF_INT_LOCK(intKeyLock);
    while ((block = QS_getBlock(&len)) != (uint8_t *)0) {
        QF_INT_UNLOCK(intKeyLock);

                           /*  wait in-line while DMA byte counter not zero */
        while ((MCF_DMA3_BCR & 0x00FFFFFF) != 0) {
        }

        MCF_DMA3_DCR = 0x00;             /* clear the DCR to avoid CE error */
        MCF_DMA3_DSR = 0x01;                               /* reset the DSR */
        MCF_DMA3_SAR = (uint32_t)block;                   /* set the source */
        MCF_DMA3_DAR = (uint32_t)&MCF_UART_UTB(2);     /* destination UART2 */
        MCF_DMA3_BCR = MCF_DMA_BCR_BCR(len);                /* byte counter */

        /* external Request, cycle Steal, dest & source = 8 bit
        * dest address NOT incremented source incremented by 1
        */
        MCF_DMA3_DCR = MCF_DMA_DCR_EEXT
                       | MCF_DMA_DCR_CS
                       | MCF_DMA_DCR_SINC
                       | MCF_DMA_DCR_SSIZE(01)
                       | MCF_DMA_DCR_DSIZE(01);


        len = 0xFFFF;                                 /* re-load the length */
        QF_INT_LOCK(intKeyLock);
    }
    QF_INT_UNLOCK(intKeyLock);
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
