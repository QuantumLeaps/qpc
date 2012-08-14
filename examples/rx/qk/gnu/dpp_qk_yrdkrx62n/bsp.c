/*****************************************************************************
* Product: BSP for YRDKRX62N board, QK, GNU-RX compiler
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Mar 08, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
#include "bsp.h"
#include "dpp.h"

#include "iodefine.h"

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

                           /* Number of timer ticks for each OS kernel tick */
#define TICK_COUNT_VAL ((TICK_TIMER_FREQ) / (BSP_TICKS_PER_SEC))

/* On board LEDs -----------------------------------------------------------*/
#define  LED04                  (PORTD.DR.BIT.B5)
#define  LED05                  (PORTE.DR.BIT.B3)
#define  LED06                  (PORTD.DR.BIT.B2)
#define  LED07                  (PORTE.DR.BIT.B0)
#define  LED08                  (PORTD.DR.BIT.B4)
#define  LED09                  (PORTE.DR.BIT.B2)
#define  LED10                  (PORTD.DR.BIT.B1)
#define  LED11                  (PORTD.DR.BIT.B7)
#define  LED12                  (PORTD.DR.BIT.B3)
#define  LED13                  (PORTE.DR.BIT.B1)
#define  LED14                  (PORTD.DR.BIT.B0)
#define  LED15                  (PORTD.DR.BIT.B6)

/* Note: These choices are not arbitrary.
*       Board LEDs are turned ON by driving the port LOW.
*/
#define  LED_ON                 0U
#define  LED_OFF                1U

#define  BSP_LED_IDLE_ON()      (LED12 = LED_ON)
#define  BSP_LED_IDLE_OFF()     (LED12 = LED_OFF)

/* On board Buttonss -------------------------------------------------------*/
enum GPIOIntTriggerType {        /* GPIO interrupt triggers (manual 11.2.8) */
    IT_LOW_LEVEL           = 0,
    IT_FALLING_EDGE        = 1,
    IT_RISING_EDGE         = 2,
    IT_RISING_FALLING_EDGE = 3,
};

#define  BSP_BUTTON_PRESSED(button_) \
    ((PORT4.PORT.BYTE & (1 << Button)) == 0);

/* clock configuration -----------------------------------------------------*/
#define SYSCLK_FREQ             (12UL * 1000UL * 1000UL)          /* 12 MHz */
#define PCLK_FREQ               (4UL * SYSCLK_FREQ)               /* 48 MHz */
#define CMT_PCLK_DIV_512        3U           /* See manual 21.2.3, CMCR.CKS */
#define BSP_CLKDIV              512U                   /* See manual 21.2.3 */
#define TICK_TIMER_FREQ         (PCLK_FREQ / BSP_CLKDIV)

#ifdef Q_SPY

    QSTimeCtr QS_tickTime_;
    QSTimeCtr QS_tickPeriod_;

    uint8_t const QS_Excep_CMTU0_CMT0 = 0;       /* identifies event source */
    uint8_t const QS_Excep_IRQ8       = 0;       /* identifies event source */
    uint8_t const QS_Excep_IRQ9       = 0;       /* identifies event source */
    uint8_t const QS_Excep_IRQ10      = 0;       /* identifies event source */

    #define UART_BAUD_RATE      (115200)

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };

#endif

/*..........................................................................*/
void BSP_init(void) {

    /* Set up system clocks, see manual 8.2.1
    * 12MHz clock
    * I Clk   = 96 MHz
    * B Clk   = 24 MHz
    * P Clock = 48 MHz
    */
    SYSTEM.SCKCR.LONG = ((0UL << 24) | (2UL << 16) | (1UL << 8));

    /* init LEDs (GPIOs and LED states to OFF) */
    PORTD.DDR.BYTE  = 0xFF;
    PORTE.DDR.BYTE |= 0x0F;
    PORTD.DR.BYTE   = 0xFF;             /* initialize all LEDs to OFF state */
    PORTE.DR.BYTE  |= 0x0F;             /* initialize all LEDs to OFF state */

    /* Init buttons as GPIO inputs
    * Config GPIO Port 4 as input for reading buttons
    * Not needed after POR because this is the default value...
    */
    PORT4.DDR.BYTE = 0;

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }

    QS_OBJ_DICTIONARY(&QS_Excep_CMTU0_CMT0);
    QS_OBJ_DICTIONARY(&QS_Excep_IRQ8);
    QS_OBJ_DICTIONARY(&QS_Excep_IRQ9);
    QS_OBJ_DICTIONARY(&QS_Excep_IRQ10);
}
/*..........................................................................*/
void BSP_busyDelay(void) {
    /* limit for the loop counter in busyDelay() */
    static uint32_t l_delay = 0UL;
    uint32_t volatile i = l_delay;
    while (i-- > 0UL) {                                   /* busy-wait loop */
    }
}
/*..........................................................................*/
void BSP_displyPhilStat(uint8_t n, char const *stat) {
                         /* turn LED on when eating and off when not eating */
    uint8_t on_off = (stat[0] == 'e' ? LED_ON : LED_OFF);
    switch (n) {
        case 0: LED04 = on_off; break;
        case 1: LED05 = on_off; break;
        case 2: LED06 = on_off; break;
        case 3: LED07 = on_off; break;
        case 4: LED08 = on_off; break;
        default: Q_ERROR();    break;
    }

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void QF_onStartup(void) {
    /* set the interrupt priorities, (manual 11.2.3) */
    IPR(CMT0,)     = TICK_INT_PRIORITY;
    IPR(ICU,IRQ8)  = SW1_INT_PRIORITY;
    IPR(ICU,IRQ9)  = SW2_INT_PRIORITY;
    IPR(ICU,IRQ10) = SW3_INT_PRIORITY;

    /* enable GPIO interrupts ...*/
    /* configure interrupts for SW1 (IRQ8) */
    PORT4.ICR.BIT.B0 = 1 ;                  /* enable input buffer for P4.0 */
    ICU.IRQCR[8].BIT.IRQMD = IT_FALLING_EDGE;      /* falling edge (11.2.8) */
    IEN(ICU, IRQ8) = 1;                 /* enable interrupt source (11.2.2) */

    /* configure interrupts for SW2 (IRQ9) */
    PORT4.ICR.BIT.B1 = 1 ;                  /* enable input buffer for P4.1 */
    ICU.IRQCR[9].BIT.IRQMD = IT_FALLING_EDGE;      /* falling edge (11.2.8) */
    IEN(ICU, IRQ9) = 1;                 /* enable interrupt source (11.2.2) */

    /* configure interrupts for SW3 (IRQ10) */
    PORT4.ICR.BIT.B2 = 1 ;                  /* enable input buffer for P4.2 */
    ICU.IRQCR[10].BIT.IRQMD = IT_FALLING_EDGE;     /* falling edge (11.2.8) */
    IEN(ICU, IRQ10) = 1;                /* enable interrupt source (11.2.2) */

    /* configure & start tick timer. Enable CMT0 module. (see manual 9.2.2) */
    MSTP(CMT0) = 0;
    CMT.CMSTR0.BIT.STR0 = 0;      /* Stop CMT0 (channel 0)  (manual 21.2.1) */
    CMT0.CMCR.BIT.CKS   = CMT_PCLK_DIV_512;  /* peripheral divider (21.2.3) */
    CMT0.CMCOR          = TICK_COUNT_VAL; /* compare-match value.  (21.2.4) */
    CMT0.CMCNT          = 0;         /* Clear free-running counter (21.2.4) */
    IR(CMT0, CMI0)      = 0;        /* ensure no pending timer ISR (11.2.1) */

    IEN(CMT0, CMI0)     = 1;            /* enable interrupt source (11.2.2) */
    CMT0.CMCR.BIT.CMIE  = 1;            /* enable timer interrupt  (21.2.3) */
    CMT.CMSTR0.BIT.STR0 = 1;  /* start tick timer (CMT0 channel 0) (21.2.1) */
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
    BSP_LED_IDLE_ON();   /* toggle the User LED on and then off, see NOTE02 */
    BSP_LED_IDLE_OFF();
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
    __asm volatile ("WAIT");
#endif
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
/*..........................................................................*/
uint8_t qsBuf[4*256];                  /* buffer for Quantum Spy */
uint8_t QS_onStartup(void const *arg) {
    (void) arg;            /* get rid of compiler warning (unused argument) */
    QS_initBuf(qsBuf, sizeof(qsBuf));

    /* Configure & enable the serial interface used by QS   */
    /* See Figure 28.7 "SCI Initialization Flowchart (Async Mode)" */
    MSTP(SCI2) = 0;                                          /* enable SCI2 */
    SCI2.SCR.BYTE = 0;// Disable SCI (TX & RX)
    SCI2.BRR = (PCLK_FREQ / (32UL * UART_BAUD_RATE) - 1);  /* set baud rate */

    /* SCI2 pin select; RXD2 is pin 42 (P5.2), TXD2 is pin 44 (P5.0) */
    IOPORT.PFFSCI.BIT.SCI2S = 1;

    /* RXD2 pin = input (set Port 5.2 Input Buffer Control Register) */
    PORT5.ICR.BIT.B2 = 1;

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
    else {     /* no rollover occured */
        return QS_tickTime_ + CMT0.CMCNT;
    }
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    b = QS_getByte();
    while (b != QS_EOD) { /* next QS trace byte available? */
        while (SCI2.SSR.BIT.TEND == 0) {                  /* TX not ready? */
        }
        SCI2.TDR = (uint8_t)b;             /* send byte to UART TDR */
        // Try to get next byte
        b = QS_getByte();
    }
}
#endif                                                             /* Q_SPY */

/*****************************************************************************
* NOTE02:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts disabled, so no
* interrupt execution time contributes to the brightness of the User LED.
*/


