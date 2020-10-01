/*****************************************************************************
* Product: DPP on AT91SAM7S-EK, QK kernel, IAR-ARM
* Last updated for version 6.9.1
* Last updated on  2020-09-22
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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

#include "AT91SAM7S64.h"  /* Atmel AT91SAM7S64 MCU */

Q_DEFINE_THIS_FILE

#pragma diag_suppress=Ta021  /* call __iar_disable_interrupt from __ramfunc */
#pragma diag_suppress=Ta022  /* possible ROM access <ptr> from __ramfunc */
#pragma diag_suppress=Ta023  /* call to non __ramfunc from __ramfunc */

/* Local objects -----------------------------------------------------------*/
typedef void (*IntVector)(void);  /* IntVector pointer-to-function */
uint32_t get_MCK_FREQ(void);      /* CPU clock set up in __low_level_init() */

uint32_t const l_led[] = {
    (1 << 0),  /* LED D1 on AT91SAM7S-EK */
    (1 << 1),  /* LED D2 on AT91SAM7S-EK */
    (1 << 2),  /* LED D3 on AT91SAM7S-EK */
    (1 << 3)   /* LED D4 on AT91SAM7S-EK */
};

#define LED_ON(num_)  (AT91C_BASE_PIOA->PIO_CODR = l_led[num_])
#define LED_OFF(num_) (AT91C_BASE_PIOA->PIO_SODR = l_led[num_])

uint32_t const l_btn[] = {
    (1U << 19), /* BTN P1 on AT91SAM7S-EK */
    (1U << 20), /* BTN P2 on AT91SAM7S-EK */
    (1U << 14), /* BTN P3 on AT91SAM7S-EK */
    (1U << 15)  /* BTN P4 on AT91SAM7S-EK */
};

static unsigned l_rnd; /* random seed */

#ifdef Q_SPY

    static uint8_t const l_ISR_tick = 0;
    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER
    };

#endif

/* ISRs and ISR "wrappers" ================================================*/
__ramfunc
void BSP_irq(void) {
    IntVector vect = (IntVector)AT91C_BASE_AIC->AIC_IVR; /* read the IVR */
    AT91C_BASE_AIC->AIC_IVR = (AT91_REG)vect; /* write AIC_IVR if protected */

    QF_INT_ENABLE();    /* allow nesting interrupts */
    (*vect)();          /* call the IRQ ISR via the pointer to function */
    QF_INT_DISABLE();   /* disable interrups for the exit sequence */

    AT91C_BASE_AIC->AIC_EOICR = 0; /* write AIC_EOICR to clear interrupt */
}
/* ISRs --------------------------------------------------------------------*/
__ramfunc
static void ISR_tick(void) {
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { ~0U, ~0U };
    uint32_t current;
    uint32_t volatile tmp;

    /* clear the interrupt source */
    tmp = AT91C_BASE_PITC->PITC_PIVR;

    QF_TICK_X(0U, &l_ISR_tick); /* process all time events at tick rate 0 */

    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = ~AT91C_BASE_PIOA->PIO_PDSR;/* read PIOA with state of Buttons */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & l_btn[0]) != 0U) {  /* debounced BTN_P1 state changed? */
        if ((buttons.depressed & l_btn[0]) != 0U) { /* is BTN_P1 depressed? */
            static QEvt const pauseEvt = { PAUSE_SIG, 0U, 0U};
            QF_PUBLISH(&pauseEvt, &l_ISR_tick);
        }
        else {            /* the button is released */
            static QEvt const serveEvt = { SERVE_SIG, 0U, 0U};
            QF_PUBLISH(&serveEvt, &l_ISR_tick);
        }

    }
}
/*..........................................................................*/
__ramfunc
static void ISR_spur(void) {
}


/* BSP functions ===========================================================*/
void BSP_init(void) {
    uint32_t i;

    /* When using the JTAG debugger the AIC might not be initialised
    * to the correct default state. This line ensures that AIC does not
    * mask all interrupts at the start.
    */
    AT91C_BASE_AIC->AIC_EOICR = 0U;

    /* enable peripheral clock for PIOA */
    AT91C_BASE_PMC->PMC_PCER = (1U << AT91C_ID_PIOA);

    /* initialize the LEDs... */
    for (i = 0; i < Q_DIM(l_led); ++i) {
        AT91C_BASE_PIOA->PIO_PER = l_led[i]; /* enable pin */
        AT91C_BASE_PIOA->PIO_OER = l_led[i]; /* configure as output pin */
        LED_OFF(i);                          /* extinguish the LED */
    }

    /* initialize the Buttons... */
    for (i = 0; i < Q_DIM(l_btn); ++i) {
        AT91C_BASE_PIOA->PIO_ODR = l_btn[i]; /* disable output (input pin) */
        AT91C_BASE_PIOA->PIO_PER = l_btn[i]; /* enable pin */
    }

    /* configure Advanced Interrupt Controller (AIC) of AT91... */
    AT91C_BASE_AIC->AIC_IDCR = ~0;           /* disable all interrupts */
    AT91C_BASE_AIC->AIC_ICCR = ~0;           /* clear all interrupts */
    for (i = 0; i < 8; ++i) {
        AT91C_BASE_AIC->AIC_EOICR = 0;       /* write AIC_EOICR 8 times */
    }

    /* set the desired ticking rate for the PIT... */
    i = (get_MCK_FREQ() / 16U / BSP_TICKS_PER_SEC) - 1U;
    AT91C_BASE_PITC->PITC_PIMR = (AT91C_PITC_PITEN | AT91C_PITC_PITIEN | i);

    BSP_randomSeed(1234U); /* seed the random number generator */

    if (QS_INIT((void *)0) == 0) { /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_ISR_tick);

    /* setup the QS filters... */
    QS_GLB_FILTER(QS_SM_RECORDS); /* state machine records */
    QS_GLB_FILTER(QS_AO_RECORDS); /* active object records */
    QS_GLB_FILTER(QS_UA_RECORDS); /* all user records */
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    if (stat[0] == 'h') {
        LED_ON(0);  /* turn LED on  */
    }
    else {
        LED_OFF(0); /* turn LED off */
    }
    if (stat[0] == 'e') {
        LED_ON(1);  /* turn LED on  */
    }
    else {
        LED_OFF(1); /* turn LED off */
    }

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);                  /* Philosopher number */
        QS_STR(stat);                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    if (paused != 0U) {
        LED_ON(2);  /* turn LED on  */
    }
    else {
        LED_OFF(2); /* turn LED off */
    }
}
/*..........................................................................*/
uint32_t BSP_random(void) {  /* a very cheap pseudo-random-number generator */
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

/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    /* hook the exception handlers from the QF port */
    *(uint32_t volatile *)0x24 = (uint32_t)&QF_undef;
    *(uint32_t volatile *)0x28 = (uint32_t)&QF_swi;
    *(uint32_t volatile *)0x2C = (uint32_t)&QF_pAbort;
    *(uint32_t volatile *)0x30 = (uint32_t)&QF_dAbort;
    *(uint32_t volatile *)0x34 = (uint32_t)&QF_reserved;
    *(uint32_t volatile *)0x38 = (uint32_t)&QK_irq;
    *(uint32_t volatile *)0x3C = (uint32_t)0U; /* unimplemented! */

    AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (uint32_t)&ISR_tick;
    AT91C_BASE_AIC->AIC_SPU = (uint32_t)&ISR_spur; /* spurious IRQ */

    AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] =
        (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | AT91C_AIC_PRIOR_LOWEST);
    AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_SYS);
    AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_SYS);
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
__ramfunc
void QK_onIdle(void) {

    /* toggle first LED on and off, see NOTE01 */
    QF_INT_DISABLE();
    LED_ON(3);  /* turn LED on  */
    LED_OFF(3); /* turn LED off */
    QF_INT_ENABLE();

#ifdef Q_SPY
    /* use the idle cycles for QS transmission... */

    if ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXBUFE) != 0) { /* not busy? */
        uint16_t nBytes = 0xFFFFU; /* get all available bytes */
        uint8_t const *block;

        QF_INT_DISABLE();
        if ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) { /* new block? */
            AT91C_BASE_DBGU->DBGU_TPR = (uint32_t)block;
            AT91C_BASE_DBGU->DBGU_TCR = (uint32_t)nBytes;
            nBytes = 0xFFFFU; /* get all available bytes */
            if ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {/*another? */
                AT91C_BASE_DBGU->DBGU_TNPR = (uint32_t)block;
                AT91C_BASE_DBGU->DBGU_TNCR = (uint32_t)nBytes;
            }
        }
        QF_INT_ENABLE();
    }
#elif defined NDEBUG /* only if not debugging (idle mode hinders debugging) */
    AT91C_BASE_PMC->PMC_SCDR = 1;/* Power-Management: disable the CPU clock */
    /* NOTE: an interrupt starts the CPU clock again */
#endif
}
/*..........................................................................*/
Q_NORETURN Q_onAssert(char_t const * const module, int_t const loc) {
    QF_INT_DISABLE(); /* disable all interrupts */
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;
    QS_ASSERTION(module, loc, 10000U); /* report assertion to QS */

    /* trip the Watchdog to reset the system */
    AT91C_BASE_WDTC->WDTC_WDCR = (0xA5U << 24) | AT91C_WDTC_WDRSTT;

    /* hang in here until the reset occurrs */
    for (;;) {
    }
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY
uint32_t l_timeOverflow;

#define QS_BUF_SIZE        (2*1024)
#define BAUD_RATE          115200U

uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE]; /* buffer for Quantum Spy */
    AT91PS_DBGU pDBGU = AT91C_BASE_DBGU;
    AT91PS_TC   pTC0  = AT91C_BASE_TC0;/* TC0 used for timestamp generation */
    uint32_t volatile tmp;

    QS_initBuf(qsBuf, sizeof(qsBuf));

    /* configure the Debug UART for QSPY output ... */
    AT91C_BASE_PIOA->PIO_PDR = AT91C_PA10_DTXD; /* configure pin as DTXD */

    pDBGU->DBGU_CR   = AT91C_US_TXEN;   /* enable only transmitter */
    pDBGU->DBGU_IDR  = ~0;              /* disable all DBGU interrupts */
    pDBGU->DBGU_MR   = AT91C_US_PAR_NONE;  /* no parity bit */
    pDBGU->DBGU_BRGR = ((get_MCK_FREQ()/BAUD_RATE + 8) >> 4); /* baud rate */
    pDBGU->DBGU_PTCR = AT91C_PDC_TXTEN; /* enable PDC transfer from DBGU */

    /* configure Timer/Counter 0 for time measurements ... */
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_TC0); /* enable clock to TC0 */

    pTC0->TC_CCR = AT91C_TC_CLKDIS; /* TC_CCR: disable Clock Counter */
    pTC0->TC_IDR = ~0;              /* TC_IDR: disable all timer interrupts */
    tmp = pTC0->TC_SR;              /* TC_SR: read */
    (void)tmp; /* avoid the compiler warning about the unused variable */

    /* CPCTRG, MCK/32 clock... */
    pTC0->TC_CMR = (AT91C_TC_CPCTRG | AT91C_TC_CLKS_TIMER_DIV3_CLOCK);
    pTC0->TC_CCR = AT91C_TC_CLKEN;  /* TC_CCR: enable Clock Counter */
    pTC0->TC_CCR = AT91C_TC_SWTRG;  /* TC_CCR: start counting */

    return 1U; /* indicate successfull QS initialization */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t nBytes = 0xFFFFU; /* get all available bytes */
    uint8_t const *block;
    while ((AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXBUFE) == 0U) { /* busy? */
    }
    if ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        AT91C_BASE_DBGU->DBGU_TPR = (uint32_t)block;
        AT91C_BASE_DBGU->DBGU_TCR = (uint32_t)nBytes;
        nBytes = 0xFFFFU; /* get all available bytes */
        if ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {
            AT91C_BASE_DBGU->DBGU_TNPR = (uint32_t)block;
            AT91C_BASE_DBGU->DBGU_TNCR = (uint32_t)nBytes;
        }
    }
}
/*..........................................................................*/
/* NOTE: getTime is invoked within a critical section (inetrrupts disabled) */
__ramfunc
uint32_t QS_onGetTime(void) {
    AT91PS_TC pTC0  = AT91C_BASE_TC0;  /* TC0 used for timestamp generation */
    uint32_t now = pTC0->TC_CV; /* get the counter value */
                                /* did the timer overflow 0xFFFF? */
    if ((pTC0->TC_SR & AT91C_TC_COVFS) != 0U) {
        l_timeOverflow += 0x10000U; /* account for the overflow */
    }
    return l_timeOverflow + now;
}
#endif  /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE01:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
