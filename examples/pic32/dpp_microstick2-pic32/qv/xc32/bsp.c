/*****************************************************************************
* BSP for DPP example, Microstick II board, cooperative QV kernel, XC32
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
#include "bsp.h"
#include "dpp.h"

Q_DEFINE_THIS_FILE

#pragma config  FNOSC    = FRCPLL    /* 8 MHz */
#pragma config  FPLLIDIV = DIV_2     /* 4 MHz */
#pragma config  FPLLMUL  = MUL_20    /* 80 MHz */
#pragma config  FPLLODIV = DIV_2     /* 40 MHz == FRC */
#pragma config  FWDTEN   = OFF       /* watchdog off */
#pragma config  FPBDIV   = DIV_1     /* same peripheral clock */

/* #pragma config statements should precede project file includes */
#include <xc.h>   /* header for PIC32 device in use */
#include <sys/attribs.h>

/* system clock using FRC and PLL: 40 MHz */
#define SYS_FREQ         40000000U
/* peripheral clock frequency */
#define PER_HZ           (SYS_FREQ / 1U)

/* controlling the LED of Microstick II */
#define LED_ON()         (LATASET = (1U << 0))
#define LED_OFF()        (LATACLR = (1U << 0))
#define LED_TOGGLE()     (LATAINV = (1U << 0))

/* Local-scope objects -----------------------------------------------------*/
static uint32_t l_rnd; /* random seed */

#ifdef Q_SPY
    static uint8_t const l_tickISR = 0U;
    static uint8_t const l_testISR = 0U;

    enum AppRecords { /* application-specific trace records */
        PHILO_STAT = QS_USER
    };

#endif

/* ISRs --------------------------------------------------------------------*/
void __ISR(_TIMER_2_VECTOR, IPL4SOFT) tickISR(void) {
    IFS0CLR = _IFS0_T2IF_MASK; /* clear the interrupt source */

    QF_TICK_X(0U, &l_tickISR); /* handle armed time events at tick rate 0 */
}
/*..........................................................................*/
/* for testing interrupt nesting and active object preemption */
void __ISR(_EXTERNAL_0_VECTOR, IPL6SOFT) testISR(void) {
    static QEvt const eat_evt = { EAT_SIG, 0U, 0U };

    IFS0CLR = _IFS0_INT0IF_MASK; /* clear the interrupt source */

    QACTIVE_POST(AO_Table, &eat_evt, &l_testISR);
}

/*--------------------------------------------------------------------------*/
void BSP_init(void) {
    TRISA = 0x00; /* set LED pins as outputs */
    PORTA = 0x00; /* set LED drive state low */

    BSP_randomSeed(1234U);

    Q_ALLEGE(QS_INIT((void *)0) != 0); /*initialize the QS software tracing */
    QS_OBJ_DICTIONARY(&l_tickISR);
    QS_OBJ_DICTIONARY(&l_testISR);

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
void BSP_displayPhilStat(uint8_t const n, char_t const *stat) {
    (void)n;
    (void)stat;
    LED_TOGGLE();

    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t const paused) {
    (void)paused;
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a cheap pseudo-random-number generator */
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
/*..........................................................................*/
/* NOTE: this implementation of the assertion handler is intended only for
* debugging and MUST be changed for deployment of the application (assuming
* that you ship your production code with assertions enabled).
*/

Q_NORETURN Q_onAssert(char_t const * const file, int_t const loc) {
    (void)file;       /* unused parameter */
    (void)loc;        /* unused parameter */
    QF_INT_DISABLE(); /* make sure that interrupts are disabled */
    for (;;) {
    }
}

/*..........................................................................*/
void QF_onStartup(void) { /* entered with interrupts locked */
    INTCONSET = _INTCON_MVEC_MASK;   /* configure multi-vectored interrupts */

    T2CON = 0x0060; /* stop timer, set up for 1:64 prescaler */
    TMR2 = 0;       /* count from zero up to the period */
    PR2 = SYS_FREQ / (BSP_TICKS_PER_SEC * 64); /* set the Timer2 period */
    IFS0CLR = _IFS0_T2IF_MASK;   /* clear Timer2 Interrupt Flag */
    IEC0SET = _IEC0_T2IE_MASK;   /* enable Timer2 interrupt */
    T2CONSET = _T2CON_ON_MASK;   /* Start Timer2 */

    INTCONbits.INT0EP = 1;       /* INT0 interrupt on positive edge */
    IEC0SET = _IEC0_INT0IE_MASK; /* enable INT0 interrupt */
    IFS0CLR = _IFS0_INT0IF_MASK; /* clear the interrupt for INT0 */

    /* explicitly assign priorities to all interrupts... */
    /* NOTE: must match the IPLxSOFT settings in the __ISR() macros */
    IPC2bits.T2IP   = 4; /* Timer2 interrupt priority, must match tickISR */
    IPC0bits.INT0IP = 6; /* set INT0 priority; must match IPL in testISR */
}
/*..........................................................................*/
void QF_onCleanup(void) {
}
/*..........................................................................*/
void QV_onIdle(void) { /* entered with interrupts DISABLED, see NOTE01 */

    /* NOTE: not enough LEDs on the Microstick II board to implement
    *  the idle loop activity indicator ...
    */
    //LED_ON (); /* blink the IDLE LED, see NOTE01 */
    //LED_OFF();

#ifdef Q_SPY
    QF_INT_ENABLE(); /* enable interrupts, see NOTE01 */

    while (U2STAbits.UTXBF == 0) { /* TX Buffer not full? */
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b == QS_EOD) { /* End-Of-Data reached? */
            break; /* break out of the loop */
        }
        U2TXREG = (uint8_t)b; /* stick the byte to TXREG for transmission */
    }
#elif defined NDEBUG
    INTCONbits.TPC = 7;  /* enable the Proximity Timer for all IPLs, NOTE01 */
    IPTMR = 4;           /* set the proximity timer to 4 CPU clocks */
    QF_INT_ENABLE();     /* enable CPU interrupts */
    _wait();             /* execute the WAIT instruction to stop the CPU */
    INTCONbits.TPC = 0;  /* disable the Proximity Timer for all IPLs */
#else
    QF_INT_ENABLE(); /* enable interrupts, see NOTE01 */
#endif
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY

#define QS_BUF_SIZE   4096
#define QS_BAUD_RATE  115200

uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[QS_BUF_SIZE]; /* buffer for Quantum Spy */

    QS_initBuf(qsBuf, sizeof(qsBuf)); /* initialize the QS trace buffer */

    /* initialize the UART2 for transmitting the QS trace data */
    U2RXRbits.U2RXR = 3;    /* Set U2RX to RPB11, pin 22, J6-5 */
    RPB10Rbits.RPB10R = 2;  /* Set U2TX to RPB10, pin 21, J6-4 */

    U2STA  = 0x0000U;       /* use default settings of 8N1 */
    U2MODE = 0x0008U;       /* enable high baud rate */
    U2BRG  = (uint16_t)((PER_HZ / (4.0 * QS_BAUD_RATE)) - 1.0 + 0.5);
    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN   = 1;

    return 1U; /* indicate successfull QS initialization */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    while ((b = QS_getByte()) != QS_EOD) { /* next QS trace byte available? */
        while (U2STAbits.UTXBF) { /* TX Buffer full? */
        }
        U2TXREG = (uint8_t)b; /* stick the byte to TXREG for transmission */
    }
}
/*..........................................................................*/
/* NOTE: works properly with interrupts enabled or disabled */
QSTimeCtr QS_onGetTime(void) {
    return  __builtin_mfc0(_CP0_COUNT, _CP0_COUNT_SELECT);
}
/*..........................................................................*/
/*! callback function to reset the target (to be implemented in the BSP) */
void QS_onReset(void) {
    uint32_t dummy;

    /* perform a system unlock sequence ,starting critical sequence*/
    SYSKEY = 0x00000000; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write key1 to SYSKEY
    SYSKEY = 0x556699AA; //write key2 to SYSKEY
    /* set SWRST bit to arm reset */
    RSWRSTSET = 1;
    /* read RSWRST register to trigger reset */
    dummy = RSWRST;
    /* prevent any unwanted code execution until reset occurs*/}
/*..........................................................................*/
/*! callback function to execute a user command (to be implemented in BSP) */
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif /* Q_SPY */

/*****************************************************************************
* NOTE01:
* The callback function QV_onIdle() is called with interrupts disabled,
* because the idle condition can be invalidated by any enabled interrupt
* that would post events. The QV_onIdle() function *must* re-enable interrupts
 internally
*
* NOTE02:
* The Temporal Proximity Timer is used to prevent a race condition of
* servicing an interrupt after re-enabling interrupts and before executing
* the WAIT instruction. The Proximity Timer is enabled for all interrupt
* priority levels (see QF_onStartup()). The Proximity Timer is set to 4
* CPU clocks right before re-enabling interrupts (with the DI instruction)
* The 4 clock ticks should be enough to execute the (DI,WAIT) instruction
* pair _atomically_.
*/

