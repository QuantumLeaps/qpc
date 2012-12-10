/*****************************************************************************
* Product: BSP for YRDKRX63N board, QK kenrel, Renesas RX Standard Toolchain
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Oct 17, 2012
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
#include "bsp.h"
#include "dpp.h"

#include <machine.h>                                 /* intrinsic functions */
#include "iodefine.h"
#include "yrdkrx63n.h"

Q_DEFINE_THIS_FILE

                 /* interrupt priorities (higher number is higher priority) */
enum InterruptPriorities {
    /* lowest urgency */
    SW1_INT_PRIORITY  = 3,
    TICK_INT_PRIORITY = 4,
    SW2_INT_PRIORITY  = 5,
    SW3_INT_PRIORITY  = 6,
    /* highest urgency */
};

/* clock configuration -----------------------------------------------------*/
#define PCLK_FREQ         48000000U
#define TICK_COUNT_VAL    (PCLK_FREQ / 128U / BSP_TICKS_PER_SEC)

/* local variables ---------------------------------------------------------*/
static uint32_t l_rnd;                                       /* random seed */

/* Q-Spy -------------------------------------------------------------------*/
#ifdef Q_SPY

    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    uint8_t const QS_CMT0_isr  = 0U;       /* identifies event source */
    uint8_t const QS_IRQ8_isr  = 0U;       /* identifies event source */
    uint8_t const QS_IRQ9_isr  = 0U;       /* identifies event source */
    uint8_t const QS_IRQ12_isr = 0U;       /* identifies event source */

    #define UART_BAUD_RATE      115200U

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };

#endif

/*..........................................................................*/
#pragma interrupt (CMT0_isr (vect = VECT(CMT0, CMI0)))
static void CMT0_isr(void) {
    QK_ISR_ENTRY();          /* inform the QK kernel about entering the ISR */

#ifdef Q_SPY
    QS_tickTime_ += QS_tickPeriod_;       /* account for the clock rollover */
#endif
    QF_TICK(& QS_CMT0_isr);                /* process all armed time events */

    QK_ISR_EXIT();            /* inform the QK kernel about exiting the ISR */
}
/*..........................................................................*/
#pragma interrupt (IRQ8_isr (vect = VECT(ICU, IRQ8)))
void IRQ8_isr(void) {
    QK_ISR_ENTRY();          /* inform the QK kernel about entering the ISR */

    QACTIVE_POST(AO_Table, Q_NEW(QEvt, PAUSE_SIG),
                 &QS_IRQ8_isr);
    QK_ISR_EXIT();            /* inform the QK kernel about exiting the ISR */
}
/*..........................................................................*/
#pragma interrupt (IRQ9_isr (vect = VECT(ICU, IRQ9)))
void IRQ9_isr(void) {
    QK_ISR_ENTRY();          /* inform the QK kernel about entering the ISR */

    QACTIVE_POST(AO_Philo[0], Q_NEW(QEvt, MAX_PUB_SIG),   /* for testing... */
                 &QS_IRQ9_isr);
    QK_ISR_EXIT();            /* inform the QK kernel about exiting the ISR */
}
/*..........................................................................*/
#pragma interrupt (IRQ12_isr (vect = VECT(ICU, IRQ12)))
void IRQ12_isr(void) {
    QK_ISR_ENTRY();          /* inform the QK kernel about entering the ISR */

    QACTIVE_POST(AO_Philo[0], Q_NEW(QEvt, MAX_PUB_SIG), /* for testing... */
                 &QS_IRQ12_isr);
    QK_ISR_EXIT();            /* inform the QK kernel about exiting the ISR */
}

/*..........................................................................*/
void BSP_init(void) {
    SYSTEM.PRCR.WORD   = 0xA50B;                             /* protect off */
    MPC.PWPR.BIT.B0WI  = 0U;                  /* unlock protection register */
    MPC.PWPR.BIT.PFSWE = 1U;                        /* unlock MPC registers */
    SYSTEM.PRCR.WORD   = 0xA500;                              /* protect on */

    /* configure LED's of the YRDKRX63N board... */
    PORTD.PODR.BYTE = 0xFFU;                 /* all PORTD outputs LED's off */
    PORTD.PDR.BYTE  = 0xFFU;                           /* all PORTD outputs */

    /* Port E -  LED's, WiFi & PMOD control */
    PORTE.PODR.BYTE = 0xFFU;    /* all LED's off, all chip selects inactive */
    PORTE.PDR.BYTE  = 0x7FU;          /* all PORTE outputs except PMOD_MISO */

    /* configure Switches's of the YRDKRX63N board...
    * Switch  Port    IRQ
    * ------  ----    ----
    * SW1     P4.0    IRQ8
    * SW2     P4.1    IRQ9
    * SW3     P4.4    IRQ12
    */
    PORT4.PDR.BIT.B0 = 0U;                          /* Make the pins inputs */
    PORT4.PDR.BIT.B1 = 0U;
    PORT4.PDR.BIT.B2 = 0U;

    PORT4.PMR.BYTE  = 0x00U;    /* All GPIO for now */
    MPC.P40PFS.BYTE = 0x40U;    /* P40 is used as IRQ pin */
    MPC.P41PFS.BYTE = 0x40U;    /* P41 is used as IRQ pin */
    MPC.P44PFS.BYTE = 0x40U;    /* P44 is used as IRQ pin */

    BSP_randomSeed(1234U);

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_RESET();
    QS_OBJ_DICTIONARY(&QS_CMT0_isr);
    QS_OBJ_DICTIONARY(&QS_IRQ8_isr);
    QS_OBJ_DICTIONARY(&QS_IRQ9_isr);
    QS_OBJ_DICTIONARY(&QS_IRQ12_isr);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
                         /* turn LED on when eating and off when not eating */
    uint8_t on_off = (stat[0] == 'e' ? LED_ON : LED_OFF);
    switch (n) {
        case 0: LED4 = on_off; break;
        case 1: LED5 = on_off; break;
        case 2: LED6 = on_off; break;
        case 3: LED7 = on_off; break;
        case 4: LED8 = on_off; break;
        default: Q_ERROR();    break;
    }

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    LED11 = (paused != 0U) ? LED_ON : LED_OFF;
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
void QF_onStartup(void) {

    SYSTEM.PRCR.WORD = 0xA50B;                               /* Protect off */
    MSTP(CMT0) = 0;                                        /* Power up CMT0 */
    SYSTEM.PRCR.WORD = 0xA500;                               /* Protect on  */

    /* set the interrupt priorities, (manual 11.2.3) */
    IPR(CMT0,)     = TICK_INT_PRIORITY;
    IPR(ICU,IRQ8)  = SW1_INT_PRIORITY;
    IPR(ICU,IRQ9)  = SW2_INT_PRIORITY;
    IPR(ICU,IRQ12) = SW3_INT_PRIORITY;

    /* setup the system clock tick interrupt as CMT0... */
    CMT.CMSTR0.BIT.STR0 = 0;
    CMT0.CMCR.WORD  = 0x0042;     /* CMIE:1(CMin enabled), CKS:2,(PCLK/128) */
    CMT0.CMCOR      = TICK_COUNT_VAL;
    IPR(CMT0, CMI0) = TICK_INT_PRIORITY;   /* Set interrupt priority in ICU */
    IEN(CMT0, CMI0) = 1U;                /* Enable the interrupt in the ICU */
    CMT.CMSTR0.BIT.STR0 = 1U;                    /* Start the clock running */


    /* setup the Switch interrupts... */
    ICU.IRQCR[ 8].BIT.IRQMD = 0x03U;  /* IRQ type: raising and falling edge */
    ICU.IRQCR[ 9].BIT.IRQMD = 0x01U;
    ICU.IRQCR[12].BIT.IRQMD = 0x01U;

    IR(ICU, IRQ8 ) = 0U;                    /* clear any pending interrupts */
    IR(ICU, IRQ9 ) = 0U;
    IR(ICU, IRQ12) = 0U;

    IEN(ICU, IRQ8 ) = 1U;                          /* enable the interrupts */
    IEN(ICU, IRQ9 ) = 1U;
    IEN(ICU, IRQ12) = 1U;
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
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QK_onIdle(void) {

    QF_INT_DISABLE();
    LED12 = LED_ON;      /* toggle the User LED on and then off, see NOTE01 */
    LED12 = LED_OFF;
    QF_INT_ENABLE();

#ifdef Q_SPY

    /* RX62N has no TX FIFO, just a Transmit Data Register (TDR) */
    if (SCI2.SSR.BIT.TEND) {                        /* Is SMC's TDR Empty? */
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {
            SCI2.TDR = (uint8_t)b;                /* send byte to UART TDR */
        }
    }
#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular MCU.
    *
    * Sleep mode for RX CPU:
    * Use compiler intrinsic function to put CPU to sleep.
    * Note that we execute this instruction with interrupts disabled,
    * i.e., PSW[I] = 0.
    * HOWEVER, executing the WAIT instruction sets PSW[I] = 1,
    * therefore we do not need to unlock (enable) interrupts upon wakeup,
    * since they will already be enabled.
    */
    wait();
#endif
}
/*..........................................................................*/
void abort(void) {
    Q_ERROR();
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[6*256];                  /* buffer for Quantum Spy */
    (void) arg;            /* get rid of compiler warning (unused argument) */
    QS_initBuf(qsBuf, sizeof(qsBuf));

    SYSTEM.PRCR.WORD = 0xA50B;                               /* protect off */
    SYSTEM.MSTPCRA.BIT.ACSE = 0;       /* all-Module Clock Stop Mode Enable */
    MSTP(SCI2) = 0;           /* expands to: SYSTEM.MSTPCRB.BIT.MSTPB29 = 0 */
    SYSTEM.PRCR.WORD = 0xA500;                               /* protect on  */

    /* Configure & enable the serial interface used by QS   */
    /* See Figure 28.7 "SCI Initialization Flowchart (Async Mode)" */
    SCI2.SCR.BYTE = 0;                             /* disable SCI (TX & RX) */
    SCI2.BRR = (PCLK_FREQ / (32UL * UART_BAUD_RATE) - 1);  /* set baud rate */

    /* Set up the UART I/O port and pins. */
    MPC.P50PFS.BYTE  = 0x4A; /* P50 is TxD2 */
    MPC.P52PFS.BYTE  = 0x4A; /* P52 is RxD2 */

    PORT5.PDR.BIT.B0 = 1;    /* TxD2 is output. */
    PORT5.PDR.BIT.B2 = 0;    /* RxD2 is input. */

    PORT5.PMR.BIT.B0 = 1;    /* TxD2 is peripheral. */
    PORT5.PMR.BIT.B2 = 1;    /* RxD2 is peripheral. */

    SCI2.SCR.BIT.CKE = 0;                   /* On-chip baud rate generator */
    SCI2.SMR.BYTE = 0;             /* PCLK input; asynchronous; mode: 8,N,1 */

    /* RX interrupt stuff */
    IR(SCI2,RXI2)  = 0;            /* clear pending interrupt flag (polled) */
    IPR(SCI2,RXI2) = 11;                      /* irq level 11 (pretty high) */
    IEN(SCI2,RXI2) = 0;      /* disable RXI2 interrupt (just poll RXI flag) */

    /* TX interrupt stuff */
    IR(SCI2,TXI2)  = 0;            /* clear pending interrupt flag (polled) */
    IPR(SCI2,TXI2) = 10;                      /* irq level 11 (pretty high) */
    IEN(SCI2,TXI2) = 0;      /* disable TXI2 interrupt (just poll RXI flag) */

    SCI2.SCR.BYTE |= 0x30 ;              /* enable RX & TX at the same time */

    /* Initialize QS timing variables */
    QS_tickPeriod_ = TICK_COUNT_VAL;
    QS_tickTime_   = 0;                    /* count up timer starts at zero */

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
//    QS_FILTER_OFF(QS_QF_ACTIVE_UNSUBSCRIBE);
    QS_FILTER_OFF(QS_QF_ACTIVE_POST_FIFO);
//    QS_FILTER_OFF(QS_QF_ACTIVE_POST_LIFO);
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

    QS_FILTER_AO_OBJ(AO_Table);

    return (uint8_t)1;                                    /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {            /* invoked with interrupts locked */
    if (IR(CMT0, CMI0)) {                           /* is tick ISR pending? */
        return QS_tickTime_ + CMT0.CMCNT + QS_tickPeriod_;
    }
    else {                                           /* no rollover occured */
        return QS_tickTime_ + CMT0.CMCNT;
    }
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    b = QS_getByte();
    while (b != QS_EOD) { /* next QS trace byte available? */
        while (SCI2.SSR.BIT.TEND == 0) {                   /* TX not ready? */
        }
        SCI2.TDR = (uint8_t)b;                     /* send byte to UART TDR */

        b = QS_getByte();                           /* try to get next byte */
    }
}
#endif                                                             /* Q_SPY */

/*****************************************************************************
* NOTE01:
* The LED12 is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/


