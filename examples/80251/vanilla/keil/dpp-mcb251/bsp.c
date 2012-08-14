/*****************************************************************************
* Product: Board Support Package for MCB251, QDPP example, Vanilla port
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Aug 11, 2008
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

Q_DEFINE_THIS_FILE

sfr16 TMR2RL = 0xCA;          /* Timer2 reload value (16-bit register pair) */
sfr16 TMR2   = 0xCC;               /* Timer2 counter (16-bit register pair) */

/* global objects ----------------------------------------------------------*/
volatile uint8_t bdata QF_pcon;  /* shadow of the PCON register, see NOTE03 */

/*..........................................................................*/
void BSP_init(void) {
    P1 = 0x80;                                       /* extinguish all LEDs */

    /* setup the Timer2 to generate the system clock-tick interrupt */
    TR2 = 0;                                                 /* stop Timer2 */
    T2MOD  = 0;                    /* set Timer2 to auto-reload, up-counter */
    /* setup the reload registers for the desired tick rate... */
    TMR2RL = (uint16_t)(0x10000 - BSP_PERIPHERAL_HZ/BSP_TICKS_PER_SEC + 0.5);
    TMR2 = TMR2RL;

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
}
/*..........................................................................*/
void BSP_displyPhilStat(uint8_t n, char const *stat) {
    switch (n) {
        case 0: LED0 = (stat[0] == 'e'); break;
        case 1: LED1 = (stat[0] == 'e'); break;
        case 2: LED2 = (stat[0] == 'e'); break;
        case 3: LED3 = (stat[0] == 'e'); break;
        case 4: LED4 = (stat[0] == 'e'); break;
        default: Q_ERROR(); break;          /* unexpected Philospher number */
    }
}
/*..........................................................................*/
void BSP_busyDelay(void) {
}
/*..........................................................................*/
void QF_onIdle(void) {        /* entered with interrupts LOCKED, see NOTE01 */

    LED6 = 1;                         /* toggle LED6 on and off, see NOTE01 */
    LED6 = 0;

#ifdef Q_SPY
    if (TI != 0) {                                    /* ready to transmit? */
        uint16_t b = QS_getByte();          /* get the next QS byte to send */
        QF_INT_UNLOCK(dummy);

        if (b != QS_EOD) {                  /* End-Of-Data not reached yet? */
            TI = 0;      /* clear the transmit-interrupt flag for next time */
            SBUF = (uint8_t)b;  /* insert the byte to the transmit register */
        }
    }
    else {                                            /* still transmitting */
        QF_INT_UNLOCK(dummy);               /* either way unlock interrupts */
    }
#elif (defined NDEBUG)
    /* stop as many peripheral clocks as possible for you application... */
    QF_pcon = PCON | 0x01;    /* set the IDL bit in the PCON shadow, NOTE03 */

    QF_INT_UNLOCK(dummy);                          /* unlock the interrupts */
    PCON = QF_pcon;                          /* go to low-power, see NOTE03 */
#else
    QF_INT_UNLOCK(dummy);                          /* unlock the interrupts */
#endif
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */

    /* stop interrupts to preserve the system's state as much as possible.. */
    /* NOTE: global interrupt lock blocks the ROM-monitor, so don't do this */
    /* QF_INT_LOCK(dummy); */

    TR2 = 0;                             /* stop Timer2 (system clock tick) */
    /* ... stop other interrupts in your system individually */

    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    for (;;) {       /* NOTE: replace the loop with reset for final version */
    }
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY

#if (BSP_CPU_HZ == 12000000)
                    /* best match for the standard PC serial port baud rate */
    #define QS_BAUD_RATE        4800
#elif (BSP_CPU_HZ == 11059000)
                    /* best match for the standard PC serial port baud rate */
    #define QS_BAUD_RATE        57600
#else
    #error "Please choose the best QSpy baud rate for your custom clock"
#endif

#define QS_BUF_SIZE        (350)

QSTimeCtr QS_tickTime;                           /* keeps timetsamp at tick */

/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE];            /* buffer for Quantum Spy */

    (void)arg;                                /* avoid the compiler warning */
    QS_initBuf(qsBuf, sizeof(qsBuf));

    /* setup Timer1-based baud rate generator */
    TR1 = 0;                                                 /* stop Timer1 */
    ET1 = 0;                                    /* disable Timer1 interrupt */
    PCON |= 0x80;              /* 0x80 = SMOD: set serial baud rate doubler */
    TMOD &= ~0xF0;                                /* clear Timer1 mode bits */
    TMOD |= 0x20;                                 /* put Timer1 into MODE 2 */
           /* set the Timer1 reload value to generate the desired baud rate */
    TH1 = (uint8_t)(0x100 - 2.0*BSP_PERIPHERAL_HZ/32.0/QS_BAUD_RATE + 0.5);
    TL1 = TH1;

    /* setup serial port registers */
    P3 |= (1 << 1); /* configure P3.1 as controlled by alternative function */
    SCON = 0x40;                                      /* serial port MODE 1 */
    REN = 0;                                     /* disable serial receiver */
    TR1 = 1;                                                /* start Timer1 */
    TI  = 1;                                         /* enable transmitting */

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
void QS_onFlush(void) {
    uint16_t b;
    while ((b = QS_getByte()) != QS_EOD) { /* next QS trace byte available? */
        while (TI == 0) {   /* hang in a loop as long as transmit not ready */
        }
        TI = 0;          /* clear the transmit-interrupt flag for next time */
        SBUF = (uint8_t)b;
    }
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {            /* invoked with interrupts locked */
    uint8_t volatile t2h1 = TH2;                                  /* step 1 */
    uint8_t volatile t2l  = TL2;                                  /* step 2 */
    uint8_t volatile t2h2 = TH2;                                  /* step 3 */
    QSTimeCtr tickTime = QS_tickTime;

    if (TF2 != 0) {    /* Timer2 auto-reloaded but tick ISR didn't run yet? */
        tickTime += (QSTimeCtr)(BSP_PERIPHERAL_HZ/BSP_TICKS_PER_SEC + 0.5);
    }

    if (t2h1 == t2h2) {   /* the high-register unchanged from step 1 and 3? */
        return tickTime
               + (QSTimeCtr)((uint16_t)t2l + ((uint16_t)t2h1 << 8));
    }
    else {                /* the high-register changed between step 1 and 3 */
        return tickTime + ((uint16_t)t2h2 << 8);
    }
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE01:
* The LED6 of the MCB251 board is used to visualize the idle loop activity.
* The brightness of the LED is proportional to the frequency of invocations
* of the idle loop. Please note that the LED is toggled with interrupts
* locked, so no interrupt execution time contributes to the brightness of
* the LED.
*
* NOTE02:
* The callback function QF_onIdle() is called with interrupts locked, because
* the idle condition can be invalidated by any enabled interrupt that would
* post events. The QF_onIdle() function *must* unlock interrupts internally
*
* NOTE03:
* The low-power mode must be entered with interrupts locked to avoid
* an interrupt to preempt the transition to the low-power mode. However,
* the 80x51 architecture does not provide an *atomic* transition to the Idle
* mode with simultaneous unlocking interrupts. Instead, here the PCON
* register is shadowed in the QF_pcon bdata variable. The idle mode
* is set first in the shadow register QF_pcon. At the end of every interrupt,
* the ISR copies the PCON value into the QF_pcon shadow (the 80x51 clears the
* IDL and PD bits in the PCON register upon interrupt entry). Later in
* QF_onIdle(), the PCON register is restored from the shadow QF_pcon, which
* the 80x51 performs *atomically* as a single machine instruction (such as
* MOV 87H,20H). If the interrupt gets serviced between unlocking interrupts
* and restoring PCON from the shadow, the IDL/PD bits will be cleared in the
* shadow, so the machine will *not* go to the Idle?PD mode. Only if the
* IDL/PD bits survive in the shadow, the Idle/PD mode is entered.
*/
