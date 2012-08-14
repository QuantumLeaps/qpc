/*****************************************************************************
* Product: BSP for DPP example, QK kernel, TMS320C2802x PICCOLO controlSTICK
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Feb 13, 2012
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
#include "dpp.h"
#include "bsp.h"

#include "DSP2802x_Device.h"                   /* defines the C28027 device */
#include <string.h>                                         /* for memcpy() */

Q_DEFINE_THIS_FILE

/* running from Flash */
#define FLASH 1

/*----------------------------------------------------------------------------
*  Target device (in DSP2802x_Device.h) determines CPU frequency
*      (for examples) - either 60 MHz (for 28026 and 28027) or 40 MHz
*      (for 28025, 28024, 28023, and 28022).
*      User does not have to change anything here.
*---------------------------------------------------------------------------*/
#if (DSP28_28026 || DSP28_28027) /* DSP28_28026 || DSP28_28027 devices only */
    #define CPU_FRQ_HZ    60000000U
#else
    #define CPU_FRQ_HZ    40000000U
#endif

#define CPU_TIMER_PERIOD \
    ((CPU_FRQ_HZ + BSP_TICKS_PER_SEC/2) / BSP_TICKS_PER_SEC)


#ifdef Q_SPY

    #define SCI_TXFIFO_DEPTH 8U
    #define SCI_BAUD_RATE    38400U
    uint8_t const l_cpu_timer0_isr = 0U;

    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };
#endif

                               /* LED LD2 of the PICCOLO controlSTICK board */
#define LD2_ON()   (GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1U)
#define LD2_OFF()  (GpioDataRegs.GPBSET.bit.GPIO34   = 1U)

static void PieInit(void);
static void PLLset(Uint16 val);
static void InitFlash(void);

/*..........................................................................*/
/* CPU Timer0 ISR is used for system clock tick (Group 1 interrupt 7) */
#pragma CODE_SECTION(cpu_timer0_isr, "ramfuncs"); /* place in RAM for speed */
static interrupt void cpu_timer0_isr(void) {
    QK_ISR_ENTRY();                      /* inform QK about entering an ISR */

    QF_TICK(&l_cpu_timer0_isr);           /* handle the QF-nano time events */

    QK_ISR_EXIT();                        /* inform QK about exiting an ISR */
}
/*..........................................................................*/
static interrupt void illegal_isr(void) {
    Q_ERROR();                                           /* assert an error */
}

/*..........................................................................*/
void BSP_init(void) {

    DINT;                                    // Global Disable all Interrupts
    IER = 0x0000;                            // Disable CPU interrupts
    IFR = 0x0000;                            // Clear all CPU interrupt flags

    EALLOW;
    SysCtrlRegs.WDCR = 0x0068;               // Disable the watchdog initially
    EDIS;

    // The Device_cal function, which copies the ADC & oscillator calibration
    // values from TI reserved OTP into the appropriate trim registers, occurs
    // automatically in the Boot ROM. If the boot ROM code is bypassed during
    // the debug process, the following function MUST be called for the ADC
    // and oscillators to function according to specification.
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;    // Enable ADC peripheral clock
    (*(void (*)(void))0x3D7C80)();           // Auto-calibrate from TI OTP
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0;    // Disable ADC clock
    EDIS;


    // Switch to Internal Oscillator 1 and turn off all other clock
    // sources to minimize power consumption
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF   = 0;
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 0; // Clk Src = INTOSC1
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF    = 1; // Turn off XCLKIN
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF   = 1; // Turn off XTALOSC
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF   = 1; // Turn off INTOSC2
    EDIS;


    // SYSTEM CLOCK speed based on internal oscillator = 10 MHz
    // 12 =  60 MHz
    // 11 =  55 MHz
    // 10 =  50 MHz
    // 9  =  45 MHz
    // 8  =  40 MHz
    // 7  =  35 MHz
    // 6  =  30 MHz
    // 5  =  25 MHz
    // 4  =  20 MHz
    // 3  =  15 MHz
    // 2  =  10 MHz
    PLLset(12);                              // choose from options above

    // Only used if running from FLASH
#ifdef FLASH
{
    extern uint8_t ramfuncs_loadstart, ramfuncs_loadend, ramfuncs_runstart;
    extern uint8_t ramconsts_loadstart, ramconsts_loadend, ramconsts_runstart;

    // Copy time critical code and Flash setup code to RAM
    memcpy(&ramfuncs_runstart, &ramfuncs_loadstart,
           (&ramfuncs_loadend - &ramfuncs_loadstart));

    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    InitFlash();    // Call the flash wrapper init function

    // Copy RAM constants from Flash to RAM
    memcpy(&ramconsts_runstart, &ramconsts_loadstart,
           (&ramconsts_loadend - &ramconsts_loadstart));
}
#endif //(FLASH)

    QF_zero();                                                /* see NOTE01 */

    // Initialise interrupt controller and Vector Table to defaults for now.
    // Application ISR mapping done later.
    PieInit();

    EALLOW;                                          /* PIE vectors setting */
    PieVectTable.TINT0 = (PINT)&cpu_timer0_isr;     // hook the CPU timer0 ISR
    /* . . . hook other ISRs */
    EDIS;

    EALLOW;               /* LOW SPEED CLOCKS prescale register settings... */
    SysCtrlRegs.LOSPCP.all              = 0x0002; // Sysclk / 4
    SysCtrlRegs.XCLK.bit.XCLKOUTDIV     = 2;

    // PERIPHERAL CLOCK ENABLES
    // If you are not using a peripheral you may want to switch
    // the clock off to save power, i.e. set to =0
    //
    // Note: not all peripherals are available on all 280x derivates.
    // Refer to the datasheet for your particular device.
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK    = 0; // ADC

    SysCtrlRegs.PCLKCR3.bit.COMP1ENCLK  = 0; // COMP1
    SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK  = 0; // COMP2

    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK   = 0; // I2C

    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK   = 0; // SPI-A

    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK   = 0; // SCI-A

    SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK  = 0; // eCAP1

    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK  = 0; // ePWM1
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK  = 0; // ePWM2
    SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK  = 0; // ePWM3
    SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK  = 0; // ePWM4

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC   = 0; // Enable TBCLK
    EDIS;


    EALLOW;                                               /* GPIO config... */
    //  GPIO-34 - PIN FUNCTION = LED for F28027 USB dongle
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;    // 0=GPIO,1=COMP2OUT,2=EMU1,3=Resv
    GpioCtrlRegs.GPBDIR .bit.GPIO34 = 1;    // 1=OUTput, 0=INput
    LD2_OFF();
    EDIS;

    // initialize the CPU Timer used for system clock tick
    CpuTimer0Regs.PRD.all      = CPU_TIMER_PERIOD;
    CpuTimer0Regs.TPR.all      = 0;   // Initialize pre-scaler to div by 1
    CpuTimer0Regs.TPRH.all     = 0;
    CpuTimer0Regs.TCR.bit.TSS  = 1;   // 1 = Stop timer initially
    CpuTimer0Regs.TCR.bit.TRB  = 1;   // 1 = reload timer
    CpuTimer0Regs.TCR.bit.SOFT = 0;
    CpuTimer0Regs.TCR.bit.FREE = 0;   // 0 = Timer Free Run Disabled
    CpuTimer0Regs.TCR.bit.TIE  = 1;   // 1 = Enable Timer Interrupt


    if (QS_INIT((void *)0) == 0) {    /* initialize the QS software tracing */
        Q_ERROR();
    }
    QS_OBJ_DICTIONARY(&l_cpu_timer0_isr);
}
/*..........................................................................*/
void QF_onStartup(void) {
    CpuTimer0Regs.TCR.bit.TSS = 0;     /* start the system clock tick timer */

    // Enable CPU INT1, which is connected to CPU-Timer 0:
    IER |= M_INT1;

    // Enable PIE: Group 1 interrupt 7 which is connected to CPU-Timer 0:
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    // Enable higher priority real-time debug events:
    ERTM;   // Enable Global realtime interrupt DBGM
}
/*..........................................................................*/
void QF_onCleanup(void) {                               /* nothing to clear */
}
/*..........................................................................*/
#pragma CODE_SECTION(QK_onIdle, "ramfuncs");      /* place in RAM for speed */
void QK_onIdle(void) {

#ifdef Q_SPY

    if (SciaRegs.SCICTL2.bit.TXRDY != 0) {                     /* TX ready? */
        uint16_t b;

        QF_INT_DISABLE();
        b = QS_getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {                              /* not End-Of-Data? */
            SciaRegs.SCITXBUF = b;                  /* put into the TX FIFO */
        }
    }

#elif defined NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular TMS320C2000 device.
    */
    asm(" IDLE");                                        /* go to IDLE mode */
#endif
}
/*..........................................................................*/
void BSP_displyPhilStat(uint8_t n, char const *stat) {
    if ((n == 0) && (stat[0] == 'e')) {
        LD2_ON();
    }
    else {
        LD2_OFF();
    }

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_busyDelay(void) {
    /* implement some busy-waiting dealy to stress-test the DPP application */
}
/*--------------------------------------------------------------------------*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    /* Next two lines for debug only to halt the processor here.
    * YOU need to change this policy for the production release!
    */
    asm(" ESTOP0");
    for(;;) {
    }
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY

#pragma DATA_SECTION(qsBuf, "spybuffer")
static uint8_t qsBuf[2*256];                      /* buffer for Quantum Spy */

/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {

    QS_initBuf(qsBuf, sizeof(qsBuf));

    EALLOW;                         /* enable the GPIO pins used by the SCI */
    /* Enable internal pull-up for the selected input pins and disable
    * internal pull-up for the output pins to reduce power consumption.
    * CAUTION: uncomment only one out of 3 available GPIO options.
    */
    //GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;// enable pull-up for GPIO28(SCIRXDA)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;  // enable pull-up for GPIO19(SCIRXDA)
    //GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0; // enable pull-up for GPIO7 (SCIRXDA)

    //GpioCtrlRegs.GPAPUD.bit.GPIO29 = 1;//disable pull-up for GPIO29(SCITXDA)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 1;  //disable pull-up for GPIO18(SCITXDA)
    //GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;//disable pull-up for GPIO12(SCITXDA)

    /* set qualification for selected pins to asynch only. Inputs are
    * synchronized to SYSCLKOUT by default. This will select asynch (no
    * qualification) for the selected pins.
    */
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // asynch input GPIO28(SCIRXDA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;    // asynch input GPIO19(SCIRXDA)
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 3;   // asynch input GPIO7 (SCIRXDA)

    /* Configure SCI-A pins using GPIO regs. This specifies which of the
    * possible GPIO pins will be SCI functional pins.
    */
    //GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;     // Configure GPIO19 for SCIRXDA
    //GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 2;    // Configure GPIO7  for SCIRXDA

    //GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;     // Configure GPIO18 for SCITXDA
    //GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2;   // Configure GPIO12 for SCITXDA

    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK   = 1; // enable SCI-A clock
    EDIS;

    /* SCICCR: 1 stop bit, even parity (ignored), no loopback, no parity,
    *  async mode, idle-line protocol, 8-bits per char
    */
    SciaRegs.SCICCR.all  = 0x0007;
    SciaRegs.SCICTL1.all = 0x0003;        /* enable TX, RX, internal SCICLK */

    /* 38400 baud @LSPCLK = 15MHz (60 MHz SYSCLK / 4) */
    SciaRegs.SCIHBAUD = 0;
    SciaRegs.SCILBAUD = (CPU_FRQ_HZ/4 + (SCI_BAUD_RATE * 8)/2)
                        / (SCI_BAUD_RATE * 8) - 1;

    SciaRegs.SCICTL1.all = 0x0023;                   // release SCI from Reset

    // initialize the CPU Timer 1 used for QS timestamp
    CpuTimer1Regs.PRD.all      = 0xFFFFFFFF;         // reload value
    CpuTimer1Regs.TPR.all      = CPU_FRQ_HZ/1000000; // pre-scaler
    CpuTimer1Regs.TPRH.all     = 0x0000;
    CpuTimer1Regs.TCR.bit.TRB  = 1;                  // 1 = reload timer
    CpuTimer1Regs.TCR.bit.TSS  = 0;                  // 0 = Start timer

                                                 /* setup the QS filters... */
    QS_FILTER_ON(QS_SIG_DIC);
    QS_FILTER_ON(QS_OBJ_DIC);
    QS_FILTER_ON(QS_FUN_DIC);

    QS_FILTER_ON(QS_QEP_STATE_EMPTY);
    QS_FILTER_ON(QS_QEP_STATE_ENTRY);
    QS_FILTER_ON(QS_QEP_STATE_EXIT);
    QS_FILTER_ON(QS_QEP_STATE_INIT);
    QS_FILTER_ON(QS_QEP_INIT_TRAN);
    QS_FILTER_ON(QS_QEP_INTERN_TRAN);
    QS_FILTER_ON(QS_QEP_TRAN);
    QS_FILTER_ON(QS_QEP_IGNORED);

//    QS_FILTER_ON(QS_QF_ACTIVE_ADD);
//    QS_FILTER_ON(QS_QF_ACTIVE_REMOVE);
//    QS_FILTER_ON(QS_QF_ACTIVE_SUBSCRIBE);
//    QS_FILTER_ON(QS_QF_ACTIVE_UNSUBSCRIBE);
//    QS_FILTER_ON(QS_QF_ACTIVE_POST_FIFO);
//    QS_FILTER_ON(QS_QF_ACTIVE_POST_LIFO);
//    QS_FILTER_ON(QS_QF_ACTIVE_GET);
//    QS_FILTER_ON(QS_QF_ACTIVE_GET_LAST);
//    QS_FILTER_ON(QS_QF_EQUEUE_INIT);
//    QS_FILTER_ON(QS_QF_EQUEUE_POST_FIFO);
//    QS_FILTER_ON(QS_QF_EQUEUE_POST_LIFO);
//    QS_FILTER_ON(QS_QF_EQUEUE_GET);
//    QS_FILTER_ON(QS_QF_EQUEUE_GET_LAST);
//    QS_FILTER_ON(QS_QF_MPOOL_INIT);
//    QS_FILTER_ON(QS_QF_MPOOL_GET);
//    QS_FILTER_ON(QS_QF_MPOOL_PUT);
//    QS_FILTER_ON(QS_QF_PUBLISH);
//    QS_FILTER_ON(QS_QF_NEW);
//    QS_FILTER_ON(QS_QF_GC_ATTEMPT);
//    QS_FILTER_ON(QS_QF_GC);
//    QS_FILTER_ON(QS_QF_TICK);
//    QS_FILTER_ON(QS_QF_TIMEEVT_ARM);
//    QS_FILTER_ON(QS_QF_TIMEEVT_AUTO_DISARM);
//    QS_FILTER_ON(QS_QF_TIMEEVT_DISARM_ATTEMPT);
//    QS_FILTER_ON(QS_QF_TIMEEVT_DISARM);
//    QS_FILTER_ON(QS_QF_TIMEEVT_REARM);
//    QS_FILTER_ON(QS_QF_TIMEEVT_POST);
//    QS_FILTER_ON(QS_QF_CRIT_ENTRY);
//    QS_FILTER_ON(QS_QF_CRIT_EXIT);
//    QS_FILTER_ON(QS_QF_ISR_ENTRY);
//    QS_FILTER_ON(QS_QF_ISR_EXIT);

    return (uint8_t)1;                                    /* return success */
}
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {            /* invoked with interrupts locked */
    return (QSTimeCtr)(0xFFFFFFFFUL - CpuTimer1Regs.TIM.all);
}
/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t b;
    while ((b = QS_getByte()) != QS_EOD) {      /* while not End-Of-Data... */
        while (SciaRegs.SCICTL2.bit.TXRDY == 0) {     /* while TX not ready */
        }
        SciaRegs.SCITXBUF = b;                    /* put into the TX BUFFER */
    }
}
#endif                                                             /* Q_SPY */

/*==========================================================================*/
// NOTE:
// IN MOST APPLICATIONS THE FUNCTIONS AFTER THIS POINT CAN BE LEFT UNCHANGED
// THE USER NEED NOT REALLY UNDERSTAND THE BELOW CODE TO SUCCESSFULLY RUN THIS
// APPLICATION.

/*..........................................................................*/
// This function initializes the PLLCR register.
//void InitPll(Uint16 val, Uint16 clkindiv)
void PLLset(Uint16 val) {
   volatile Uint16 iVol;

   // Make sure the PLL is not running in limp mode
   if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0) {
      EALLOW;
      // OSCCLKSRC1 failure detected. PLL running in limp mode.
      // Re-enable missing clock logic.
      SysCtrlRegs.PLLSTS.bit.MCLKCLR = 1;
      EDIS;
      // Replace this line with a call to an appropriate
      // SystemShutdown(); function.
      asm("   ESTOP0");     // Uncomment for debugging purposes
   }

   // DIVSEL MUST be 0 before PLLCR can be changed from
   // 0x0000. It is set to 0 by an external reset XRSn
   // This puts us in 1/4
   if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0) {
       EALLOW;
       SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
       EDIS;
   }

   // Change the PLLCR
   if (SysCtrlRegs.PLLCR.bit.DIV != val) {

        EALLOW;
        // Before setting PLLCR turn off missing clock detect logic
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
        SysCtrlRegs.PLLCR.bit.DIV = val;
        EDIS;

        // Optional: Wait for PLL to lock.
        // During this time the CPU will switch to OSCCLK/2 until
        // the PLL is stable.  Once the PLL is stable the CPU will
        // switch to the new PLL value.
        //
        // This time-to-lock is monitored by a PLL lock counter.
        //
        // Code is not required to sit and wait for the PLL to lock.
        // However, if the code does anything that is timing critical,
        // and requires the correct clock be locked, then it is best to
        // wait until this switching has completed.

        // Wait for the PLL lock bit to be set.
        // The watchdog should be disabled before this loop, or fed within
        // the loop via ServiceDog().
        //
        while (SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1) {
        }

        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
        EDIS;
    }

    // divide down SysClk by 2 to increase stability
    EALLOW;
    SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
    EDIS;
}
/*..........................................................................*/
// This function initializes the PIE control registers to a known state.
//
void PieInit(void) {
    int16  i;
    Uint32 *dest = (Uint32 *)&PieVectTable;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;         // disable the PIE Vector Table

                                              // Clear all PIEIER registers...
    PieCtrlRegs.PIEIER1.all  = 0;
    PieCtrlRegs.PIEIER2.all  = 0;
    PieCtrlRegs.PIEIER3.all  = 0;
    PieCtrlRegs.PIEIER4.all  = 0;
    PieCtrlRegs.PIEIER5.all  = 0;
    PieCtrlRegs.PIEIER6.all  = 0;
    PieCtrlRegs.PIEIER7.all  = 0;
    PieCtrlRegs.PIEIER8.all  = 0;
    PieCtrlRegs.PIEIER9.all  = 0;
    PieCtrlRegs.PIEIER10.all = 0;
    PieCtrlRegs.PIEIER11.all = 0;
    PieCtrlRegs.PIEIER12.all = 0;

                                              // Clear all PIEIFR registers...
    PieCtrlRegs.PIEIFR1.all  = 0;
    PieCtrlRegs.PIEIFR2.all  = 0;
    PieCtrlRegs.PIEIFR3.all  = 0;
    PieCtrlRegs.PIEIFR4.all  = 0;
    PieCtrlRegs.PIEIFR5.all  = 0;
    PieCtrlRegs.PIEIFR6.all  = 0;
    PieCtrlRegs.PIEIFR7.all  = 0;
    PieCtrlRegs.PIEIFR8.all  = 0;
    PieCtrlRegs.PIEIFR9.all  = 0;
    PieCtrlRegs.PIEIFR10.all = 0;
    PieCtrlRegs.PIEIFR11.all = 0;
    PieCtrlRegs.PIEIFR12.all = 0;

    EALLOW;
    for (i = 0; i < 128; ++i) {
        *dest++ = (Uint32)&illegal_isr;
    }
    EDIS;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
}
/*..........................................................................*/
// This function initializes the Flash Control registers
// CAUTION: This function MUST be executed out of RAM!!!
// Executing it out of OTP/Flash will yield unpredictable results
#pragma CODE_SECTION(InitFlash, "ramfuncs");
void InitFlash(void) {
    EALLOW;
    // Enable Flash Pipeline mode to improve performance of code
    // executed from Flash.
    FlashRegs.FOPT.bit.ENPIPE = 1;

    // CAUTION:
    // Minimum waitstates required for the flash operating at a given CPU
    // rate must be characterized by TI. Refer to the datasheet for the
    // latest information.

    // Set the Paged Waitstate for the Flash
    FlashRegs.FBANKWAIT.bit.PAGEWAIT = 3;

    // Set the Random Waitstate for the Flash
    FlashRegs.FBANKWAIT.bit.RANDWAIT = 3;

    // Set the Waitstate for the OTP
    FlashRegs.FOTPWAIT.bit.OTPWAIT = 5;

    // CAUTION:
    // ONLY THE DEFAULT VALUE FOR THESE 2 REGISTERS SHOULD BE USED
    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
    EDIS;

    // Force a pipeline flush to ensure that the write to
    // the last register configured occurs before returning.
    asm(" RPT #7 || NOP");
}

/*****************************************************************************
* NOTE01:
* The standard TI startup code (c_int00) does NOT zero the uninitialized
* variables, as required by the C-standard. Since QP relies on the clearing
* of the static uninitialized variables, the critical QP objects are cleared
* explicitly in this BSP.
*/








