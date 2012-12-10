/*****************************************************************************
* Product: Board Support Package for LPC-P123X with QK
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Oct 08, 2012
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

#include <iolpc2138.h>      /* IAR-compiler header file for the LPC2138 I/O */

#include "lpc_data_type.h"
#include "drv_hd44780.h"
#include "drv_hd44780_cnfg.h"

Q_DEFINE_THIS_FILE

#pragma diag_suppress=Ta021  /* call __iar_disable_interrupt from __ramfunc */
#pragma diag_suppress=Ta022     /* possible ROM access <ptr> from __ramfunc */
#pragma diag_suppress=Ta023         /* call to non __ramfunc from __ramfunc */

                                                                /* OSC [Hz] */
#define FOSC              14745600
                                                           /* Core clk [Hz] */
#define CCLK              (FOSC*4)
                                /* Peripheral clk -- the same as core clock */
#define PCLK              CCLK

/* Local objects -----------------------------------------------------------*/
static unsigned  l_rnd;                                      /* random seed */

typedef void (*IntVector)(void);           /* IntVector pointer-to-function */

int __low_level_init(void);                                    /* prototype */

#ifdef Q_SPY
    static uint32_t l_tickTime;
    static uint8_t const l_ISR_tick = 0;
    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };

#endif

/*..........................................................................*/
__arm __ramfunc void BSP_irq(void) {
    IntVector vect = (IntVector)VICVectAddr;/* read the vector from the VIC */

    QF_INT_ENABLE();                            /* allow nesting interrupts */
    (*vect)();              /* call the IRQ ISR via the pointer to function */
    QF_INT_DISABLE();            /* disable interrups for the exit sequence */

    VICVectAddr = 0;        /* send the End-Of-Interrupt command to the VIC */
}
/*..........................................................................*/
__arm __ramfunc void BSP_fiq(void) {
    /* TBD: implement the FIQ handler directly right here  */
    /* NOTE: Do NOT enable interrupts throughout the whole FIQ processing. */
    /* NOTE: Do NOT write EOI to the VIC */
}
/* ISRs --------------------------------------------------------------------*/
static __arm __ramfunc void ISR_tick(void) {
    T1IR = 0x1;                               /* clear the interrupt source */
#ifdef Q_SPY
    l_tickTime += (CCLK / (32 * BSP_TICKS_PER_SEC));
#endif
    QF_TICK(&l_ISR_tick);
}
/*..........................................................................*/
static __arm __ramfunc void ISR_ext(void) {
    /* TBD: clear the interrupt source ... */
    /* TBD: do the ISR work ... */
}
/*..........................................................................*/
static __arm __ramfunc void ISR_spur(void) {
}
/*..........................................................................*/
__arm void QF_onStartup(void) {
                            /* hook the exception handlers from the QF port */
    *(uint32_t volatile *)0x24 = (uint32_t)&QF_undef;
    *(uint32_t volatile *)0x28 = (uint32_t)&QF_swi;
    *(uint32_t volatile *)0x2C = (uint32_t)&QF_pAbort;
    *(uint32_t volatile *)0x30 = (uint32_t)&QF_dAbort;
    *(uint32_t volatile *)0x34 = (uint32_t)&QF_reserved;

    *(uint32_t volatile *)0x38 = (uint32_t)&QK_irq;
    *(uint32_t volatile *)0x3C = (uint32_t)&QK_fiq;

    VICIntSelect = 0x0;        /* assign all interrupts to the IRQ category */

    /* Setting up Timer0 to handle the external interrupt.
    * Timer0 has priorty 0 (highest)
    */
    VICVectCntl0 = 0x25;
    VICVectAddr0 = (uint32_t)&ISR_tick;
    VICIntEnable = (1 << 5);                    /* enable TIMER 1 interrupt */

    /* Setting up VIC to handle the external interrupt.
    * External Interrupt1 has priorty 1
    */
    VICVectCntl1 = 0x2F;
    VICVectAddr1 = (uint32_t)&ISR_ext;

    /*  set up the spurious interrupt handler */
    VICDefVectAddr = (uint32_t)&ISR_spur;

    T1TCR = 0x1;                                         /* start the timer */
}

/*............................................................................
* __low_level_init() is invoked by the standard IAR startup sequence after
* cstartup, but before initializing the segments in RAM. The function
* gives the application a chance to perform early initializations of
* the hardware. This function cannot use any static variables, because these
* have not yet been initialized in RAM.
*
* The value returned by __low_level_init() determines whether or not data
* segments should be initialized by __seqment_init. If __low_level_init()
* returns 0, the data segments will NOT be initialized. For more information
* see the "IAR ARM C/C++ Compiler Reference Guide".
*/
int __low_level_init(void) {
    /* Initialize PLL (Configured for Fosc = 14.7456MHz crystal) to
    * boost processor clock to CCLK = 4*Fosc = 58.9824MHz
    * The PLL values are M = 4, P = 2 (see "LPC213x User Manual" Sec 3.7.10)
    */
    PLLCFG_bit.MSEL = 4 - 1;
    PLLCFG_bit.PSEL = 2;
    PLLFEED = 0xAA;
    PLLFEED = 0x55;

    PLLCON_bit.PLLE = 1;                                  /* enable the PLL */
    PLLFEED = 0xAA;
    PLLFEED = 0x55;

    while (PLLSTAT_bit.PLOCK == 0) {            /* Wait for the PLL to lock */
    }
    PLLCON_bit.PLLC = 1;             /* Connect the PLL as the clock source */
    PLLFEED = 0xAA;
    PLLFEED = 0x55;

    /* enable the Memory Accelerator Module (MAM).
    * See also the LPC2148 Errata at
    * http://www.nxp.com/acrobat_download/erratasheets/ES_LPC2148_1.pdf
    */
    MAMCR  = 0x0;                                            /* disable MAM */
    MAMTIM = 3;        /* 3 MAM fetch cycles (recommended for CCLK > 40MHz) */
    MAMCR  = 0x2;                                             /* enable MAM */

    return 1;                /* proceed with the initialization of segments */
}
/*..........................................................................*/
void BSP_init(void) {
    VICIntEnClear = ~0x0;               /* initially disable all interrupts */

    /* Initialize GIO ... */
    PINSEL0 = 0;
    PINSEL1 = 0;
    IO0SET = 0x87FE01F1;
    IO0CLR = ~0x87FE01F1;
    IO0DIR = 0x0001FF0F;
    IO1SET = 0xFFFFFFFF;
    IO1CLR = ~0xFFFFFFFF;
    IO1DIR = 0x00000000;

    /* External interrupts setup as level-sensitive.
    * Some of the steps mentioned here are taking into considertaion
    * the EXTINT.2 Errata
    */
    EXTINT   = 0x2;
    VPBDIV   = 0x0;
    EXTMODE  = 0x0;
    VPBDIV   = 0x0;
    EXTPOLAR = 0x0;
    VPBDIV   = 0x0;

    /* Initialize Timer 1 ... */
    T1TCR = 0x0;
    T1TC  = 0x0;
    T1PR  = PCLK / (BSP_TICKS_PER_SEC * (1 << 14)) - 1;        /* prescaler */
    T1PC  = 0x0;
    T1MR0 = (1 << 14);                                   /* the match value */
    T1MCR = 0x3;                            /* reset and interrupt on match */

    VPBDIV = 1;             /* set peripheral (PCLK) to System (CCLK) clock */

    /* initialize the LCD and the backlight */
    HD44780_PowerUpInit();                       /* HD44780 LCD driver init */
    LightInit();                                          /* Backlight Init */
    LightCntr(LIGHT_FAST, LIGHT_ON);

    HD44780_StrShow((HD44780_XY_DEF)1, (HD44780_XY_DEF)1, "Quantum Leaps");
    HD44780_StrShow((HD44780_XY_DEF)1, (HD44780_XY_DEF)2, "0 ,1 ,2 ,3 ,4  ");

    BSP_randomSeed(1234);

    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_RESET();
    QS_OBJ_DICTIONARY(&l_ISR_tick);
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
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
__arm __ramfunc void QK_onIdle(void) {
#ifdef Q_SPY                     /* use the idle cycles for QS transmission */

    if (U0LSR_bit.THRE != 0) {                            /* TX FIFO empty? */
        uint16_t nBytes = 16;                    /* capacity of the TX FIFO */
        uint8_t const *block;

        QF_INT_DISABLE();
        block = QS_getBlock(&nBytes);     /* get the data block to transfer */
        QF_INT_ENABLE();

        while (nBytes-- != 0) {
            U0THR = *block++;              /* stick the byte to the TX FIFO */
        }
    }
#elif defined NDEBUG /* only if not debugging (idle mode hinders debugging) */
//    PCON_bit.IDL = 1;                      /* go to idle mode to save power */
// NOTE: idle or sleep mode hangs the J-TAG, it's difficult to
// get control of the MCU again!!!
    /* an interrupt starts the CPU clock again */
#endif
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    HD44780_CharShow((HD44780_XY_DEF)(3*n + 2), (HD44780_XY_DEF)2, stat[0]);

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}

/*..........................................................................*/
__arm void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    QF_INT_DISABLE();         /* make sure that all interrupts are disabled */
    for (;;) {                            /* hang here in the for-ever loop */
    }
}
/*..........................................................................*/
/* NOTE: delay for the HD44780 LCD */
void Dly100us(void *arg) {
    /* NOTE: the function takes 4 instruction on entry and 4 instructions
    * in every loop pass. Assuming 2 clocks per instruction, this gives
    * the following estimate for the loop counter ctr
    */
    uint32_t ctr = ((uint32_t)arg * CCLK)/10000/4/2 - 1;
    while (ctr-- != 0) {
    }
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
#define QS_BUF_SIZE        (2*1024)
#define BAUD_RATE          115200

QSTimeCtr l_tickTime;                           /* keeps timetsamp at tick */

/*..........................................................................*/
static uint8_t UART_config(uint32_t baud) {

    PINSEL0_bit.P0_0 = 0x1;                           /* select TX0 for pin */
    PINSEL0_bit.P0_1 = 0x1;                           /* selext RX0 for pin */

    baud = (uint16_t)(PCLK / (16*baud));           /* divisor for baud rate */
    U0LCR_bit.DLAB = 1;                                         /* set DLAB */
    U0DLL = (uint8_t)baud;                             /* divisor latch LSB */
    U0DLM = (uint8_t)(baud >> 8);                      /* divisor latch MSB */
    U0LCR = (1 << 1) | (1 << 0);       /* LCR: 8-bits, no parity, 1stop bit */
    U0IER = 0;                      /* put UART0 into the polling FIFO mode */
    U0FCR = (1 << 2) | (1 << 0);                   /* FCR: enable, TX clear */

    return (uint8_t)1;                                           /* success */
}
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE];                     /* buffer for QS */
    QS_initBuf(qsBuf, sizeof(qsBuf));

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

    return UART_config(115200UL);
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t nBytes = 16;              /* the capacity of the UART0 TX FIFO */
    uint8_t const *block;
    while ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        while (U0LSR_bit.THRE == 0) {                 /* TX FIFO not empty? */
        }                                                /* keep waiting... */
        while (nBytes-- != 0) {
            U0THR = *block++;              /* stick the byte to the TX FIFO */
        }
        nBytes = 16;                            /* for the next time around */
    }
}
/*..........................................................................*/
/* NOTE: getTime is invoked within a critical section (inetrrupts disabled) */
QSTimeCtr QS_onGetTime(void) {
    static QSTimeCtr l_lastTime;
    QSTimeCtr now = l_tickTime + T1TC;

    if (l_lastTime > now) {                 /* are we going "back" in time? */
        now += (CCLK / (32 * BSP_TICKS_PER_SEC));    /* assume one rollover */
    }
    l_lastTime = now;

    return now;
}
#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/

