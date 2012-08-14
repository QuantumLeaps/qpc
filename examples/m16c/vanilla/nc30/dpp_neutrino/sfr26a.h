/****************************************************************************
*                                                                           *
*   File name   : sfr26a.h
*    Description : Definition of M16C/26A's SFR                              *
*                                                                           *
*   Copyright   : 2005 Renesas Technology Europe Ltd.                        *
*     Copyright   : 2005 Renesas Technology Corporation.                        *
*     All Rights Reserved                                                     *
*                                                                           *
*****************************************************************************/

/********************************************************
*   declare SFR addresses                               *
********************************************************/

#pragma ADDRESS     pm0_addr    0004H       /* Processor mode register 0 */

#pragma ADDRESS     pm1_addr    0005H       /* Processor mode register 1 */

#pragma ADDRESS     cm0_addr    0006H       /* System clock control register 0 */

#pragma ADDRESS     cm1_addr    0007H       /* System clock control register 1 */

#pragma ADDRESS     aier_addr   0009H       /* Address match interrupt enable register */

#pragma ADDRESS     prcr_addr   000aH       /* Protect register */

#pragma ADDRESS     cm2_addr    000cH       /* Oscillation stop detection register */

#pragma ADDRESS     wdts_addr   000eH       /* Watchdog timer start register */

#pragma ADDRESS     wdc_addr    000fH       /* Watchdog timer control register */

#pragma ADDRESS     rmad0_addr  0010H       /* Address match interrupt register 0 */

#pragma ADDRESS     rmad1_addr  0014H       /* Address match interrupt register 1 */

#pragma ADDRESS     vcr1_addr   0019H       /* Voltage detection register 1 */

#pragma ADDRESS     vcr2_addr   001aH       /* Voltage detection register 2 */

#pragma ADDRESS     plc0_addr   001cH       /* PLL control register 0 */

#pragma ADDRESS     pm2_addr    001eH       /* Processor mode register 2 */

#pragma ADDRESS     d4int_addr  001fH       /* Voltage down detection interrupt register */

#pragma ADDRESS     sar0_addr   0020H       /* DMA0 source pointer */

#pragma ADDRESS     dar0_addr   0024H       /* DMA0 destination pointer */

#pragma ADDRESS     tcr0_addr   0028H       /* DMA0 transfer counter */

#pragma ADDRESS     dm0con_addr 002cH       /* DMA0 control register */

#pragma ADDRESS     sar1_addr   0030H       /* DMA1 source pointer */

#pragma ADDRESS     dar1_addr   0034H       /* DMA1 destination pointer */

#pragma ADDRESS     tcr1_addr   0038H       /* DMA1 transfer counter */

#pragma ADDRESS     dm1con_addr 003cH       /* DMA1 control register */

#pragma ADDRESS     int3ic_addr   0044H     /* INT3~ interrupt control register */

#pragma ADDRESS     int5ic_addr 0048H       /* INT5~ interrupt control register */

#pragma ADDRESS     int4ic_addr 0049H       /* INT4~ interrupt control register */

#pragma ADDRESS     bcnic_addr  004aH       /* UART2 Bus collision detection interrupt control register */

#pragma ADDRESS     dm0ic_addr  004bH       /* DMA0 interrupt control register */

#pragma ADDRESS     dm1ic_addr  004cH       /* DMA1 interrupt control register */

#pragma ADDRESS     kupic_addr  004dH       /* Key input interrupt control register */

#pragma ADDRESS     adic_addr   004eH       /* A-D conversion interrupt control register */

#pragma ADDRESS     s2tic_addr  004fH       /* UART2 transmit interrupt control register */

#pragma ADDRESS     s2ric_addr  0050H       /* UART2 receive interrupt control register */

#pragma ADDRESS     s0tic_addr  0051H       /* UART0 transmit interrupt control register */

#pragma ADDRESS     s0ric_addr  0052H       /* UART0 receive interrupt control register */

#pragma ADDRESS     s1tic_addr  0053H       /* UART1 transmit interrupt control register */

#pragma ADDRESS     s1ric_addr  0054H       /* UART1 receive interrupt control register */

#pragma ADDRESS     ta0ic_addr  0055H       /* Timer A0 interrupt control register */

#pragma ADDRESS     ta1ic_addr  0056H       /* Timer A1 interrupt control register */

#pragma ADDRESS     ta2ic_addr  0057H       /* Timer A2 interrupt control register */

#pragma ADDRESS     ta3ic_addr  0058H       /* Timer A3 interrupt control register */

#pragma ADDRESS     ta4ic_addr  0059H       /* Timer A4 interrupt control register */

#pragma ADDRESS     tb0ic_addr  005aH       /* Timer B0 interrupt control register */

#pragma ADDRESS     tb1ic_addr  005bH       /* Timer B1 interrupt control register */

#pragma ADDRESS     tb2ic_addr  005cH       /* Timer B2 interrupt control register */

#pragma ADDRESS     int0ic_addr 005dH       /* INT0~ interrupt control register */

#pragma ADDRESS     int1ic_addr 005eH       /* INT1~ interrupt control register */

#pragma ADDRESS     int2ic_addr 005fH       /* INT2~ interrupt control register */

#pragma ADDRESS     fmr4_addr   01b3H       /* Flash memory control register 4 */

#pragma ADDRESS     fmr1_addr   01b5H       /* Flash memory control register 1 */

#pragma ADDRESS     fmr0_addr   01b7H       /* Flash memory control register 0 */

#pragma ADDRESS     tprc_addr   025aH       /* Three-phase protect control register */

#pragma ADDRESS     rocr_addr   025cH       /* Ring oscillator control register */

#pragma ADDRESS     pacr_addr   025dH       /* Pin assignment control register */

#pragma ADDRESS     pclkr_addr  025eH       /* Peripheral clock select register */

#pragma ADDRESS     nddr_addr    033eH      /* NMI digital debounce register */

#pragma ADDRESS     p17ddr_addr  033fH      /* P17 digital debounce register */

#pragma ADDRESS     ta11_addr   0342H       /* Timer A1-1 register */

#pragma ADDRESS     ta21_addr   0344H       /* Timer A2-1 register */

#pragma ADDRESS     ta41_addr   0346H       /* Timer A4-1 register */

#pragma ADDRESS     invc0_addr  0348H       /* Three-phase PWM control register 0 */

#pragma ADDRESS     invc1_addr  0349H       /* Three-phase PWM control register 1 */

#pragma ADDRESS     idb0_addr   034aH       /* Three-phase output buffer register 0 */

#pragma ADDRESS     idb1_addr   034bH       /* Three-phase output buffer register 1 */

#pragma ADDRESS     dtt_addr    034cH       /* Dead time timer */

#pragma ADDRESS     ictb2_addr  034dH       /* Timer B2 interrupt occurrences frequency set counter */

#pragma ADDRESS     pdrf_addr   034eH       /* Position-data-retain function register */

#pragma ADDRESS     pfcr_addr   0358H       /* Port function control register */

#pragma ADDRESS     ifsr2a_addr 035eH       /* Interrupt cause select register 2 */

#pragma ADDRESS     ifsr_addr   035fH       /* Interrupt cause select register */

#pragma ADDRESS     u2smr4_addr 0374H       /* UART2 special mode register 4 */

#pragma ADDRESS     u2smr3_addr 0375H       /* UART2 special mode register 3 */

#pragma ADDRESS     u2smr2_addr 0376H       /* UART2 special mode register 2 */

#pragma ADDRESS     u2smr_addr  0377H       /* UART2 special mode register */

#pragma ADDRESS     u2mr_addr   0378H       /* UART2 transmit/receive mode register */

#pragma ADDRESS     u2brg_addr  0379H       /* UART2 bit rate generator */

#pragma ADDRESS     u2tb_addr   037aH       /* UART2 transmit buffer register */

#pragma ADDRESS     u2c0_addr   037cH       /* UART2 transmit/receive control register 0 */

#pragma ADDRESS     u2c1_addr   037dH       /* UART2 transmit/receive control register 1 */

#pragma ADDRESS     u2rb_addr   037eH       /* UART2 receive buffer register */

#pragma ADDRESS     tabsr_addr  0380H       /* Count start flag */

#pragma ADDRESS     cpsrf_addr  0381H       /* Clock prescaler reset flag */

#pragma ADDRESS     onsf_addr   0382H       /* One-shot start flag */

#pragma ADDRESS     trgsr_addr  0383H       /* Trigger select register */

#pragma ADDRESS     udf_addr    0384H       /* Up/down flag */

#pragma ADDRESS     ta0_addr    0386H       /* Timer A0 register */

#pragma ADDRESS     ta1_addr    0388H       /* Timer A1 register */

#pragma ADDRESS     ta2_addr    038aH       /* Timer A2 register */

#pragma ADDRESS     ta3_addr    038cH       /* Timer A3 register */

#pragma ADDRESS     ta4_addr    038eH       /* Timer A4 register */

#pragma ADDRESS     tb0_addr    0390H       /* Timer B0 register */

#pragma ADDRESS     tb1_addr    0392H       /* Timer B1 register */

#pragma ADDRESS     tb2_addr    0394H       /* Timer B2 register */

#pragma ADDRESS     ta0mr_addr  0396H       /* Timer A0 mode register */

#pragma ADDRESS     ta1mr_addr  0397H       /* Timer A1 mode register */

#pragma ADDRESS     ta2mr_addr  0398H       /* Timer A2 mode register */

#pragma ADDRESS     ta3mr_addr  0399H       /* Timer A3 mode register */

#pragma ADDRESS     ta4mr_addr  039aH       /* Timer A4 mode register */

#pragma ADDRESS     tb0mr_addr  039bH       /* Timer B0 mode register */

#pragma ADDRESS     tb1mr_addr  039cH       /* Timer B1 mode register */

#pragma ADDRESS     tb2mr_addr  039dH       /* Timer B2 mode register */

#pragma ADDRESS     tb2sc_addr  039eH        /* Timer B2 special mode register */

#pragma ADDRESS     u0mr_addr   03a0H       /* UART0 transmit/receive mode register */

#pragma ADDRESS     u0brg_addr  03a1H       /* UART0 bit rate generator */

#pragma ADDRESS     u0tb_addr   03a2H       /* UART0 transmit buffer register */

#pragma ADDRESS     u0c0_addr   03a4H       /* UART0 transmit/receive control register 0 */

#pragma ADDRESS     u0c1_addr   03a5H       /* UART0 transmit/receive control register 1 */

#pragma ADDRESS     u0rb_addr   03a6H       /* UART0 receive buffer register */

#pragma ADDRESS     u1mr_addr   03a8H       /* UART1 transmit/receive mode register */

#pragma ADDRESS     u1brg_addr  03a9H       /* UART1 bit rate generator */

#pragma ADDRESS     u1tb_addr   03aaH       /* UART1 transmit buffer register */

#pragma ADDRESS     u1c0_addr   03acH       /* UART1 transmit/receive control register 0 */

#pragma ADDRESS     u1c1_addr   03adH       /* UART1 transmit/receive control register 1 */

#pragma ADDRESS     u1rb_addr   03aeH       /* UART1 receive buffer register */

#pragma ADDRESS     ucon_addr   03b0H       /* UART transmit/receive control register 2 */

#pragma ADDRESS     crcsar_addr 03b4H       /* sfr address register */

#pragma ADDRESS     crcmr_addr  03b6H       /* crc mode register */

#pragma ADDRESS     dm0sl_addr  03b8H       /* DMA0 request cause select register */

#pragma ADDRESS     dm1sl_addr  03baH       /* DMA1 request cause select register */

#pragma ADDRESS     crcd_addr   03bcH       /* CRC data register */

#pragma ADDRESS     crcin_addr  03beH       /* CRC input register */

#pragma ADDRESS     ad0_addr    03c0H       /* A-D register 0 */

#pragma ADDRESS     ad1_addr    03c2H       /* A-D register 1 */

#pragma ADDRESS     ad2_addr    03c4H       /* A-D register 2 */

#pragma ADDRESS     ad3_addr    03c6H       /* A-D register 3 */

#pragma ADDRESS     ad4_addr    03c8H       /* A-D register 4 */

#pragma ADDRESS     ad5_addr    03caH       /* A-D register 5 */

#pragma ADDRESS     ad6_addr    03ccH       /* A-D register 6 */

#pragma ADDRESS     ad7_addr    03ceH       /* A-D register 7 */

#pragma ADDRESS     adtrgcon_addr 03d2H     /* A-D trigger control register */

#pragma ADDRESS     adstat0_addr 03d3H      /* A-D convert status register 0 */

#pragma ADDRESS     adcon2_addr 03d4H       /* A-D control register 2 */

#pragma ADDRESS     adcon0_addr 03d6H       /* A-D control register 0 */

#pragma ADDRESS     adcon1_addr 03d7H       /* A-D control register 1 */

#pragma ADDRESS     p1_addr     03e1H       /* Port P1 register */

#pragma ADDRESS     pd1_addr    03e3H       /* Port P1 direction register */

#pragma ADDRESS     p6_addr     03ecH       /* Port P6 register */

#pragma ADDRESS     p7_addr     03edH       /* Port P7 register */

#pragma ADDRESS     pd6_addr    03eeH       /* Port P6 direction register */

#pragma ADDRESS     pd7_addr    03efH       /* Port P7 direction register */

#pragma ADDRESS     p8_addr     03f0H       /* Port P8 register */

#pragma ADDRESS     p9_addr     03f1H       /* Port P9 register */

#pragma ADDRESS     pd8_addr    03f2H       /* Port P8 direction register */

#pragma ADDRESS     pd9_addr    03f3H       /* Port P9 direction register */

#pragma ADDRESS     p10_addr    03f4H       /* Port P10 register */

#pragma ADDRESS     pd10_addr   03f6H       /* Port P10 direction register */

#pragma ADDRESS     pur0_addr   03fcH       /* Pull-up control register 0 */

#pragma ADDRESS     pur1_addr   03fdH       /* Pull-up control register 1 */

#pragma ADDRESS     pur2_addr   03feH       /* Pull-up control register 2 */

#pragma ADDRESS     pcr_addr    03ffH       /* Port control register */

/********************************************************
*   declare SFR bit                                     *
********************************************************/
struct bit_def {
        char    b0:1;
        char    b1:1;
        char    b2:1;
        char    b3:1;
        char    b4:1;
        char    b5:1;
        char    b6:1;
        char    b7:1;
};
union byte_def{
    struct bit_def bit;
    char    byte;
};

/*------------------------------------------------------
    Processor mode register 0
------------------------------------------------------*/
union byte_def pm0_addr;
#define     pm0     pm0_addr.byte

#define     pm03        pm0_addr.bit.b3     /* Software reset bit */

/*------------------------------------------------------
    Processor mode register 1
------------------------------------------------------*/
union byte_def pm1_addr;
#define     pm1     pm1_addr.byte

#define     pm10        pm1_addr.bit.b0     /* Flash data block access bit */
#define     pm12        pm1_addr.bit.b2     /* Watchdog timer function select bit */
#define     pm17        pm1_addr.bit.b7     /* Wait bit */

/*------------------------------------------------------
    System clock control register 0
------------------------------------------------------*/
union byte_def cm0_addr;
#define     cm0     cm0_addr.byte

#define     cm00        cm0_addr.bit.b0     /* Clock output function select bit */
#define     cm01        cm0_addr.bit.b1     /* Clock output function select bit */
#define     cm02        cm0_addr.bit.b2     /* WAIT Mode peripheral function clock stop bit */
#define     cm03        cm0_addr.bit.b3     /* Xcin-Xcout drive capacity select bit */
#define     cm04        cm0_addr.bit.b4     /* Port Xc select bit */
#define     cm05        cm0_addr.bit.b5     /* Main clock stop bit */
#define     cm06        cm0_addr.bit.b6     /* Main clock division select bit 0 */
#define     cm07        cm0_addr.bit.b7     /* System clock select bit */

/*------------------------------------------------------
    System clock control register 1
------------------------------------------------------*/
union byte_def cm1_addr;
#define     cm1     cm1_addr.byte

#define     cm10        cm1_addr.bit.b0     /* All clock stop control bit */
#define     cm11        cm1_addr.bit.b1     /* System clock select bit 1 */
#define     cm15        cm1_addr.bit.b5     /* Xin-Xout drive capacity select bit */
#define     cm16        cm1_addr.bit.b6     /* Main clock division select bit 1 */
#define     cm17        cm1_addr.bit.b7     /* Main clock division select bit 1 */

/*------------------------------------------------------
    Address match interrupt enable register
------------------------------------------------------*/
union byte_def aier_addr;
#define     aier        aier_addr.byte

#define     aier0       aier_addr.bit.b0    /* Address match interrupt 0 enable bit */
#define     aier1       aier_addr.bit.b1    /* Address match interrupt 1 enable bit */

/*------------------------------------------------------
    Protect register
------------------------------------------------------*/
union byte_def prcr_addr;
#define     prcr        prcr_addr.byte

#define     prc0        prcr_addr.bit.b0    /* Enable write to CM0,CM1,CM2,ROCR,PLC0 and PCLKR registers */
#define     prc1        prcr_addr.bit.b1    /* Enable write to PM0,PM1,PM2,TB2SC,INVC0 and INVC1 registers */
#define     prc2        prcr_addr.bit.b2    /* Enable write to PD9 and PACR registers */
#define     prc3        prcr_addr.bit.b3    /* Enable write to VCR2 and D4INT registers */

/*------------------------------------------------------
    Oscillation stop detection register
------------------------------------------------------*/
union byte_def cm2_addr;
#define     cm2         cm2_addr.byte

#define     cm20        cm2_addr.bit.b0     /* Oscillation stop,reoscillation detection bit */
#define     cm21        cm2_addr.bit.b1     /* System clock select bit 2 */
#define     cm22        cm2_addr.bit.b2     /* Oscillation stop,reoscillation detection flag */
#define     cm23        cm2_addr.bit.b3     /* Xin monitor flag */
#define     cm27        cm2_addr.bit.b7     /* Operation select bit */

/*------------------------------------------------------
    Watchdog timer start register
------------------------------------------------------*/
union byte_def wdts_addr;
#define     wdts        wdts_addr.byte

/*------------------------------------------------------
    Watchdog timer control register
------------------------------------------------------*/
union byte_def wdc_addr;
#define     wdc         wdc_addr.byte

#define     wdc5        wdc_addr.bit.b5     /* Cold start/warm start discrimination flag */
#define     wdc7        wdc_addr.bit.b7     /* Prescaler select bit */

/*------------------------------------------------------
    Voltage detection register 1
------------------------------------------------------*/
union byte_def vcr1_addr;
#define     vcr1        vcr1_addr.byte

#define     vcr13       vcr1_addr.bit.b3    /* Voltage down monitor flag */

/*------------------------------------------------------
    Voltage detection register 2
------------------------------------------------------*/
union byte_def vcr2_addr;
#define     vcr2        vcr2_addr.byte

#define     vcr25       vcr2_addr.bit.b5    /* RAM retention limit detection monitor bit */
#define     vcr26       vcr2_addr.bit.b6    /* Reset level monitor bit */
#define     vcr27       vcr2_addr.bit.b7    /* Voltage down monitor bit */

/*------------------------------------------------------
    PLL control register 0
------------------------------------------------------*/
union byte_def plc0_addr;
#define     plc0        plc0_addr.byte

#define     plc00       plc0_addr.bit.b0    /* PLL multiplying factor select bit */
#define     plc01       plc0_addr.bit.b1    /* PLL multiplying factor select bit */
#define     plc02       plc0_addr.bit.b2    /* PLL multiplying factor select bit */
#define     plc07       plc0_addr.bit.b7    /* operation enable bit */

/*------------------------------------------------------
    Processor mode register 2
------------------------------------------------------*/
union byte_def pm2_addr;
#define     pm2     pm2_addr.byte

#define     pm20        pm2_addr.bit.b0     /* Specifying wait when accessing SFR */
#define     pm21        pm2_addr.bit.b1     /* System clock protective bit */
#define     pm22        pm2_addr.bit.b2     /* WDT count source protective bit */
#define     pm24        pm2_addr.bit.b4     /* P85/NMI configuration bit */

/*------------------------------------------------------
    CRC input register
------------------------------------------------------*/
union byte_def crcin_addr;
#define     crcin   crcin_addr.byte

/*------------------------------------------------------
    Voltage down detection interrupt register
------------------------------------------------------*/
union byte_def d4int_addr;
#define     d4int     d4int_addr.byte

#define     d40         d4int_addr.bit.b0   /* Voltage down detection interrupt enable bit */
#define     d41         d4int_addr.bit.b1   /* STOP mode deactivation control bit */
#define     d42         d4int_addr.bit.b2   /* Voltage change detection flag */
#define     d43         d4int_addr.bit.b3   /* WDT overflow detect flag */
#define     df0         d4int_addr.bit.b4   /* Sampling clock select bit */
#define     df1         d4int_addr.bit.b5   /* Sampling clock select bit */

/*------------------------------------------------------
Three-phase protect control register
------------------------------------------------------*/
union byte_def tprc_addr;
#define     tprc        tprc_addr.byte

#define     tprc0       tprc_addr.bit.b0

/*------------------------------------------------------
    Ring oscillator control register
------------------------------------------------------*/
union byte_def rocr_addr;
#define     rocr        rocr_addr.byte

#define     rocr0       rocr_addr.bit.b0    /* Frequency select bit */
#define     rocr1       rocr_addr.bit.b1    /*                      */
#define     rocr2       rocr_addr.bit.b2    /* Divider select bit */
#define     rocr3       rocr_addr.bit.b3    /*                    */

/*------------------------------------------------------
    Pin assignment control register
------------------------------------------------------*/
union byte_def pacr_addr;
#define     pacr        pacr_addr.byte

#define     pacr0       pacr_addr.bit.b0    /* Pin enabling bit */
#define     pacr1       pacr_addr.bit.b1    /* Pin enabling bit */
#define     pacr2       pacr_addr.bit.b2    /* Pin enabling bit */
#define     u1map       pacr_addr.bit.b7    /* UART1 pin remapping bit */

/*------------------------------------------------------
    Peripheral clock select register
------------------------------------------------------*/
union byte_def pclkr_addr;
#define    pclkr        pclkr_addr.byte

#define    pclk0        pclkr_addr.bit.b0   /* Timers A,B clock select bit */
#define    pclk1        pclkr_addr.bit.b1   /* SI/O clock select bit */
#define    pclk5        pclkr_addr.bit.b5   /* Clock output function expansion select bit */

/*------------------------------------------------------
    NMI digital debounce register
------------------------------------------------------*/
union byte_def nddr_addr;
#define     nddr        nddr_addr.byte

/*------------------------------------------------------
    P17 digital debounce register
------------------------------------------------------*/
union byte_def p17ddr_addr;
#define     p17ddr      p17ddr_addr.byte

/*------------------------------------------------------
    Three-phase PWM control register 0
------------------------------------------------------*/
union byte_def invc0_addr;
#define     invc0       invc0_addr.byte

#define     inv00       invc0_addr.bit.b0   /* Effective interrupt output polarity select bit */
#define     inv01       invc0_addr.bit.b1   /* Effective interrupt output specification bit */
#define     inv02       invc0_addr.bit.b2   /* Mode select bit */
#define     inv03       invc0_addr.bit.b3   /* Output control bit */
#define     inv04       invc0_addr.bit.b4   /* Positive and negative phases concurrent output disable bit */
#define     inv05       invc0_addr.bit.b5   /* Positive and negative phases concurrent output detect flag */
#define     inv06       invc0_addr.bit.b6   /* Modulation mode select bit */
#define     inv07       invc0_addr.bit.b7   /* Software trigger select bit */

/*------------------------------------------------------
    Three-phase PWM control register 1
------------------------------------------------------*/
union byte_def invc1_addr;
#define     invc1       invc1_addr.byte

#define     inv10       invc1_addr.bit.b0   /* Timer A1,A2,A4 start trigger signal select bit */
#define     inv11       invc1_addr.bit.b1   /* Timer A1-1,A2-1,A4-1 control bit */
#define     inv12       invc1_addr.bit.b2   /* Dead time timer count source select bit */
#define     inv13       invc1_addr.bit.b3   /* Carrier wave detect flag */
#define     inv14       invc1_addr.bit.b4   /* Output polarity control bit */
#define     inv15       invc1_addr.bit.b5   /* Dead time invalid bit */
#define     inv16       invc1_addr.bit.b6   /* Dead time timer trigger select bit */

/*------------------------------------------------------
    Three-phase output buffer register 0
------------------------------------------------------*/
union byte_def idb0_addr;
#define     idb0        idb0_addr.byte

#define     du0         idb0_addr.bit.b0    /* U  phase output buffer 0 */
#define     dub0        idb0_addr.bit.b1    /* U~ phase output buffer 0 */
#define     dv0         idb0_addr.bit.b2    /* V  phase output buffer 0 */
#define     dvb0        idb0_addr.bit.b3    /* V~ phase output buffer 0 */
#define     dw0         idb0_addr.bit.b4    /* W  phase output buffer 0 */
#define     dwb0        idb0_addr.bit.b5    /* W~ phase output buffer 0 */

/*------------------------------------------------------
    Three-phase output buffer register 1
------------------------------------------------------*/
union byte_def idb1_addr;
#define     idb1        idb1_addr.byte

#define     du1         idb1_addr.bit.b0    /* U  phase output buffer 1 */
#define     dub1        idb1_addr.bit.b1    /* U~ phase output buffer 1 */
#define     dv1         idb1_addr.bit.b2    /* V  phase output buffer 1 */
#define     dvb1        idb1_addr.bit.b3    /* V~ phase output buffer 1 */
#define     dw1         idb1_addr.bit.b4    /* W  phase output buffer 1 */
#define     dwb1        idb1_addr.bit.b5    /* W~ phase output buffer 1 */

/*------------------------------------------------------
     Dead time timer ; Use "MOV" instruction when writing to this register.
------------------------------------------------------*/
union byte_def dtt_addr;
#define     dtt         dtt_addr.byte

/*------------------------------------------------------------------
     Timer B2 interrupt occurrences frequency set counter
     ; Use "MOV" instruction when writing to this register.
-------------------------------------------------------------------*/
union byte_def ictb2_addr;
#define     ictb2       ictb2_addr.byte

/*------------------------------------------------------
    Position-data-retain function register
------------------------------------------------------*/
union byte_def pdrf_addr;
#define     pdrf        pdrf_addr.byte

#define     pdrw        pdrf_addr.bit.b0    /* W-phase position data retain bit */
#define     pdrv        pdrf_addr.bit.b1    /* V-phase position data retain bit */
#define     pdru        pdrf_addr.bit.b2    /* U-phase position data retain bit */
#define     pdrt        pdrf_addr.bit.b3    /* Retain-trigger polarity select bit */

/*------------------------------------------------------
    Port function control register
------------------------------------------------------*/
union byte_def pfcr_addr;
#define     pfcr        pfcr_addr.byte

#define     pfc0        pfcr_addr.bit.b0    /* Port P8_0 output function select bit */
#define     pfc1        pfcr_addr.bit.b1    /* Port P8_1 output function select bit */
#define     pfc2        pfcr_addr.bit.b2    /* Port P7_2 output function select bit */
#define     pfc3        pfcr_addr.bit.b3    /* Port P7_3 output function select bit */
#define     pfc4        pfcr_addr.bit.b4    /* Port P7_4 output function select bit */
#define     pfc5        pfcr_addr.bit.b5    /* Port P7_5 output function select bit */

/*------------------------------------------------------
     ifsr2a
------------------------------------------------------*/
union byte_def ifsr2a_addr;
#define     ifsr2a      ifsr2a_addr.byte

#define     ifsr20      ifsr2a_addr.bit.b0  /* Reserved bit */

/*------------------------------------------------------
     ifsr
------------------------------------------------------*/
union byte_def ifsr_addr;
#define     ifsr        ifsr_addr.byte

#define     ifsr0       ifsr_addr.bit.b0     /* INT0~ interrupt polarity switching bit */
#define     ifsr1       ifsr_addr.bit.b1     /* INT1~ interrupt polarity switching bit */
#define     ifsr2       ifsr_addr.bit.b2     /* INT2~ interrupt polarity switching bit */
#define     ifsr3       ifsr_addr.bit.b3     /* INT3~ interrupt polarity switching bit */
#define     ifsr4       ifsr_addr.bit.b4     /* INT4~ interrupt polarity switching bit */
#define     ifsr5       ifsr_addr.bit.b5     /* INT5~ interrupt polarity switching bit */
#define     ifsr6       ifsr_addr.bit.b6     /* Interrupt request cause select bit */
#define     ifsr7       ifsr_addr.bit.b7     /* Interrupt request cause select bit */

/*------------------------------------------------------
    UART2 special mode register 4
------------------------------------------------------*/
union byte_def u2smr4_addr;
#define     u2smr4      u2smr4_addr.byte

#define     stareq_u2smr4  u2smr4_addr.bit.b0  /* Start condition generate bit */
#define     rstareq_u2smr4 u2smr4_addr.bit.b1  /* Restart condition generate bit */
#define     stpreq_u2smr4  u2smr4_addr.bit.b2  /* Stop condition generate bit */
#define     stspsel_u2smr4 u2smr4_addr.bit.b3  /* SCL,SDA output select bit */
#define     ackd_u2smr4    u2smr4_addr.bit.b4  /* ACK data bit */
#define     ackc_u2smr4    u2smr4_addr.bit.b5  /* ACK data output enable bit */
#define     sclhi_u2smr4   u2smr4_addr.bit.b6  /* SCL output stop enable bit */
#define     swc9_u2smr4    u2smr4_addr.bit.b7  /* SCL wait bit 3 */

/*------------------------------------------------------
    UART2 special mode register 3
------------------------------------------------------*/
union byte_def u2smr3_addr;
#define     u2smr3         u2smr3_addr.byte

#define     ckph_u2smr3    u2smr3_addr.bit.b1   /* Clock phase set bit */
#define     nodc_u2smr3    u2smr3_addr.bit.b3   /* Clock output select bit */
#define     dl0_u2smr3     u2smr3_addr.bit.b5   /* SDA digital delay setup bit */
#define     dl1_u2smr3     u2smr3_addr.bit.b6   /* SDA digital delay setup bit */
#define     dl2_u2smr3     u2smr3_addr.bit.b7   /* SDA digital delay setup bit */

/*------------------------------------------------------
    UART2 special mode register 2
------------------------------------------------------*/
union byte_def u2smr2_addr;
#define     u2smr2         u2smr2_addr.byte

#define     iicm2_u2smr2   u2smr2_addr.bit.b0  /* IIC mode selection bit 2 */
#define     csc_u2smr2     u2smr2_addr.bit.b1  /* Clock-synchronous bit */
#define     swc_u2smr2     u2smr2_addr.bit.b2  /* SCL wait output bit */
#define     als_u2smr2     u2smr2_addr.bit.b3  /* SDA output stop bit */
#define     stac_u2smr2    u2smr2_addr.bit.b4  /* UART2 initialization bit */
#define     swc2_u2smr2    u2smr2_addr.bit.b5  /* SCL wait output bit 2 */
#define     sdhi_u2smr2    u2smr2_addr.bit.b6  /* SDA output disable bit */

/*------------------------------------------------------
    UART2 special mode register
------------------------------------------------------*/
union byte_def u2smr_addr;
#define     u2smr   u2smr_addr.byte

#define     iicm_u2smr     u2smr_addr.bit.b0   /* IIC mode selection bit */
#define     abc_u2smr      u2smr_addr.bit.b1   /* Arbitration lost detecting flag control bit */
#define     bbs_u2smr      u2smr_addr.bit.b2   /* Bus busy flag */
#define     abscs_u2smr    u2smr_addr.bit.b4   /* Bus collision detect sampling clock select bit */
#define     acse_u2smr     u2smr_addr.bit.b5   /* Auto clear function select bit of transmit enable bit */
#define     sss_u2smr      u2smr_addr.bit.b6   /* Transmit start condition select bit */

/*------------------------------------------------------
    UART2 baud rate generation register ; Use "MOV" instruction when writing to these registers.
------------------------------------------------------*/
union byte_def u2brg_addr;
#define     u2brg       u2brg_addr.byte

/*------------------------------------------------------
    UART2 transmit/receive control register 1
------------------------------------------------------*/
union byte_def u2c1_addr;
#define     u2c1        u2c1_addr.byte

#define     te_u2c1     u2c1_addr.bit.b0    /* Transmit enable bit */
#define     ti_u2c1     u2c1_addr.bit.b1    /* Transmit buffer empty flag */
#define     re_u2c1     u2c1_addr.bit.b2    /* Receive enable bit */
#define     ri_u2c1     u2c1_addr.bit.b3    /* Receive complete flag */
#define     u2irs       u2c1_addr.bit.b4    /* UART2 transmit interrupt cause select bit */
#define     u2rrm       u2c1_addr.bit.b5    /* UART2 continuous receive mode enable bit */
#define     u2lch       u2c1_addr.bit.b6    /* Data logic select bit */
#define     u2ere       u2c1_addr.bit.b7    /* Error signal output enable bit */

/*------------------------------------------------------
    Count start flag
------------------------------------------------------*/
union byte_def tabsr_addr;
#define     tabsr       tabsr_addr.byte

#define     ta0s        tabsr_addr.bit.b0   /* Timer A0 count start flag */
#define     ta1s        tabsr_addr.bit.b1   /* Timer A1 count start flag */
#define     ta2s        tabsr_addr.bit.b2   /* Timer A2 count start flag */
#define     ta3s        tabsr_addr.bit.b3   /* Timer A3 count start flag */
#define     ta4s        tabsr_addr.bit.b4   /* Timer A4 count start flag */
#define     tb0s        tabsr_addr.bit.b5   /* Timer B0 count start flag */
#define     tb1s        tabsr_addr.bit.b6   /* Timer B1 count start flag */
#define     tb2s        tabsr_addr.bit.b7   /* Timer B2 count start flag */

/*------------------------------------------------------
    Clock prescaler reset flag
------------------------------------------------------*/
union byte_def cpsrf_addr;
#define     cpsrf       cpsrf_addr.byte

#define     cpsr        cpsrf_addr.bit.b7   /* Clock prescaler reset flag */

/*------------------------------------------------------
    One-shot start flag
------------------------------------------------------*/
union byte_def onsf_addr;
#define     onsf        onsf_addr.byte

#define     ta0os       onsf_addr.bit.b0    /* Timer A0 one-shot start flag */
#define     ta1os       onsf_addr.bit.b1    /* Timer A1 one-shot start flag */
#define     ta2os       onsf_addr.bit.b2    /* Timer A2 one-shot start flag */
#define     ta3os       onsf_addr.bit.b3    /* Timer A3 one-shot start flag */
#define     ta4os       onsf_addr.bit.b4    /* Timer A4 one-shot start flag */
#define     tazie       onsf_addr.bit.b5    /* Z-phase input enable bit */
#define     ta0tgl      onsf_addr.bit.b6    /* Timer A0 event/trigger select bit */
#define     ta0tgh      onsf_addr.bit.b7    /* Timer A0 event/trigger select bit */

/*------------------------------------------------------
    Trigger select register
------------------------------------------------------*/
union byte_def trgsr_addr;
#define     trgsr       trgsr_addr.byte

#define     ta1tgl      trgsr_addr.bit.b0   /* Timer A1 event/trigger select bit */
#define     ta1tgh      trgsr_addr.bit.b1   /* Timer A1 event/trigger select bit */
#define     ta2tgl      trgsr_addr.bit.b2   /* Timer A2 event/trigger select bit */
#define     ta2tgh      trgsr_addr.bit.b3   /* Timer A2 event/trigger select bit */
#define     ta3tgl      trgsr_addr.bit.b4   /* Timer A3 event/trigger select bit */
#define     ta3tgh      trgsr_addr.bit.b5   /* Timer A3 event/trigger select bit */
#define     ta4tgl      trgsr_addr.bit.b6   /* Timer A4 event/trigger select bit */
#define     ta4tgh      trgsr_addr.bit.b7   /* Timer A4 event/trigger select bit */

/*--------------------------------------------------------
    Up/down flag ; Use "MOV" instruction to write to this register.
--------------------------------------------------------*/
union byte_def   udf_addr;               /* UP/down flag */
#define     udf     udf_addr.byte

#define        ta0ud        udf_addr.bit.b0        /* Timer A0 up/down flag */
#define        ta1ud        udf_addr.bit.b1        /* Timer A1 up/down flag */
#define        ta2ud        udf_addr.bit.b2        /* Timer A2 up/down flag */
#define        ta3ud        udf_addr.bit.b3        /* Timer A3 up/down flag */
#define        ta4ud        udf_addr.bit.b4        /* Timer A4 up/down flag */
#define        ta2p        udf_addr.bit.b5        /* Timer A2 two-phase pulse signal processing select bit */
#define        ta3p        udf_addr.bit.b6        /* Timer A3 two-phase pulse signal processing select bit */
#define        ta4p        udf_addr.bit.b7        /* Timer A4 two-phase pulse signal processing select bit */

/*------------------------------------------------------
    Timer B2 special mode register
------------------------------------------------------*/
union byte_def tb2sc_addr;
#define     tb2sc      tb2sc_addr.byte

#define     pwcon          tb2sc_addr.bit.b0   /* Timer B2 reload timing switching bit */
#define     ivpcr1         tb2sc_addr.bit.b1   /* Three phase output port ~SD control bit 1 */
#define     tb0en          tb2sc_addr.bit.b2   /* Timer B0 operation mode select bit */
#define     tb1en          tb2sc_addr.bit.b3   /* Timer B1 operation mode select bit */
#define     tb2sel         tb2sc_addr.bit.b4   /* Trigger select bit */

/*------------------------------------------------------
    UART0 baud rate generation register ; Use "MOV" instruction when writing to these registers.
------------------------------------------------------*/
union byte_def u0brg_addr;
#define     u0brg       u0brg_addr.byte

/*------------------------------------------------------
    UART1 baud rate generation register ; Use "MOV" instruction when writing to these registers.
------------------------------------------------------*/
union byte_def u1brg_addr;
#define     u1brg       u1brg_addr.byte

/*------------------------------------------------------
    UART transmit/receive control register 2
------------------------------------------------------*/
union byte_def ucon_addr;
#define     ucon        ucon_addr.byte

#define     u0irs       ucon_addr.bit.b0    /* UART0 transmit interrupt cause select bit */
#define     u1irs       ucon_addr.bit.b1    /* UART1 transmit interrupt cause select bit */
#define     u0rrm       ucon_addr.bit.b2    /* UART0 continuous receive mode enable bit */
#define     u1rrm       ucon_addr.bit.b3    /* UART1 continuous receive mode enable bit */
#define     clkmd0      ucon_addr.bit.b4    /* CLK/CLKS select bit 0 */
#define     clkmd1      ucon_addr.bit.b5    /* CLK/CLKS select bit 1 */
#define     rcsp        ucon_addr.bit.b6    /* Separate RTS/CTS bit */

/*------------------------------------------------------
    crc mode register
------------------------------------------------------*/
union byte_def crcmr_addr;
#define     crcmr      crcmr_addr.byte

#define     crcps        crcmr_addr.bit.b0
#define     crcms        crcmr_addr.bit.b7

/*------------------------------------------------------
    AD trigger control register
------------------------------------------------------*/
union byte_def adtrgcon_addr;
#define     adtrgcon    adtrgcon_addr.byte

#define     sse        adtrgcon_addr.bit.b0 /* A-D operation mode select bit 2 */
#define     dte        adtrgcon_addr.bit.b1 /* A-D operation mode select bit 3 */
#define     hptrg0     adtrgcon_addr.bit.b2 /* AN0 trigger select bit */
#define     hptrg1     adtrgcon_addr.bit.b3 /* AN1 trigger select bit */

/*------------------------------------------------------
    AD convert status register 0
------------------------------------------------------*/
union byte_def adstat0_addr;
#define     adstat0     adstat0_addr.byte

#define     aderr0      adstat0_addr.bit.b0 /* AN1 trigger status flag */
#define     aderr1      adstat0_addr.bit.b1 /* Conversion termination flag */
#define     adtcsf      adstat0_addr.bit.b3 /* Delayed triggered sweep status flag */
#define     adstt0      adstat0_addr.bit.b4 /* AN0 conversion status flag */
#define     adstt1      adstat0_addr.bit.b5 /* AN1 conversion status flag */
#define     adstrt0     adstat0_addr.bit.b6 /* AN0 conversion completion status flag */
#define     adstrt1     adstat0_addr.bit.b7 /* AN1 conversion completion status flag */

/*------------------------------------------------------
    A-D control register 2
------------------------------------------------------*/
union byte_def adcon2_addr;
#define     adcon2      adcon2_addr.byte

#define     smp         adcon2_addr.bit.b0  /* A-D conversion method select bit */
#define     adgsel0     adcon2_addr.bit.b1  /* AD input group select bit */
#define     adgsel1     adcon2_addr.bit.b2  /* AD input group select bit */
#define     cks2        adcon2_addr.bit.b4  /* Frequency select bit 2 */
#define     trg1        adcon2_addr.bit.b5  /* Trigger select bit */

/*------------------------------------------------------
    A-D control register 0
------------------------------------------------------*/
union byte_def adcon0_addr;
#define     adcon0      adcon0_addr.byte

#define     ch0         adcon0_addr.bit.b0  /* Analog input pin select bit */
#define     ch1         adcon0_addr.bit.b1  /* Analog input pin select bit */
#define     ch2         adcon0_addr.bit.b2  /* Analog input pin select bit */
#define     md0         adcon0_addr.bit.b3  /* A-D operation mode select bit 0 */
#define     md1         adcon0_addr.bit.b4  /* A-D operation mode select bit 0 */
#define     trg         adcon0_addr.bit.b5  /* Trigger select bit */
#define     adst        adcon0_addr.bit.b6  /* A-D conversion start flag */
#define     cks0        adcon0_addr.bit.b7  /* Frequency select bit 0 */

/*------------------------------------------------------
    A-D control register 1
------------------------------------------------------*/
union byte_def adcon1_addr;
#define     adcon1      adcon1_addr.byte

#define     scan0       adcon1_addr.bit.b0  /* A-D sweep pin select bit */
#define     scan1       adcon1_addr.bit.b1  /* A-D sweep pin select bit */
#define     md2         adcon1_addr.bit.b2  /* A-D operation mode select bit 1 */
#define     bits        adcon1_addr.bit.b3  /* 8/10-bit mode select bit */
#define     cks1        adcon1_addr.bit.b4  /* Frequency select bit 1 */
#define     vcut        adcon1_addr.bit.b5  /* Vref connect bit */

/*------------------------------------------------------
    Port P1 register
------------------------------------------------------*/
union byte_def p1_addr;
#define     p1      p1_addr.byte

#define     p1_5        p1_addr.bit.b5      /* Port P15 register */
#define     p1_6        p1_addr.bit.b6      /* Port P16 register */
#define     p1_7        p1_addr.bit.b7      /* Port P17 register */

/*------------------------------------------------------
    Port P1 direction register
------------------------------------------------------*/
union byte_def pd1_addr;
#define     pd1     pd1_addr.byte

#define     pd1_5       pd1_addr.bit.b5     /* Port P15 direction register */
#define     pd1_6       pd1_addr.bit.b6     /* Port P16 direction register */
#define     pd1_7       pd1_addr.bit.b7     /* Port P17 direction register */

/*------------------------------------------------------
    Port P6 register
------------------------------------------------------*/
union byte_def p6_addr;
#define     p6      p6_addr.byte

#define     p6_0        p6_addr.bit.b0      /* Port P60 register */
#define     p6_1        p6_addr.bit.b1      /* Port P61 register */
#define     p6_2        p6_addr.bit.b2      /* Port P62 register */
#define     p6_3        p6_addr.bit.b3      /* Port P63 register */
#define     p6_4        p6_addr.bit.b4      /* Port P64 register */
#define     p6_5        p6_addr.bit.b5      /* Port P65 register */
#define     p6_6        p6_addr.bit.b6      /* Port P66 register */
#define     p6_7        p6_addr.bit.b7      /* Port P67 register */

/*------------------------------------------------------
    Port P7 register
------------------------------------------------------*/
union byte_def p7_addr;
#define     p7      p7_addr.byte

#define     p7_0        p7_addr.bit.b0      /* Port P70 register */
#define     p7_1        p7_addr.bit.b1      /* Port P71 register */
#define     p7_2        p7_addr.bit.b2      /* Port P72 register */
#define     p7_3        p7_addr.bit.b3      /* Port P73 register */
#define     p7_4        p7_addr.bit.b4      /* Port P74 register */
#define     p7_5        p7_addr.bit.b5      /* Port P75 register */
#define     p7_6        p7_addr.bit.b6      /* Port P76 register */
#define     p7_7        p7_addr.bit.b7      /* Port P77 register */

/*------------------------------------------------------
    Port P6 direction register
------------------------------------------------------*/
union byte_def pd6_addr;
#define     pd6     pd6_addr.byte

#define     pd6_0       pd6_addr.bit.b0     /* Port P60 direction register */
#define     pd6_1       pd6_addr.bit.b1     /* Port P61 direction register */
#define     pd6_2       pd6_addr.bit.b2     /* Port P62 direction register */
#define     pd6_3       pd6_addr.bit.b3     /* Port P63 direction register */
#define     pd6_4       pd6_addr.bit.b4     /* Port P64 direction register */
#define     pd6_5       pd6_addr.bit.b5     /* Port P65 direction register */
#define     pd6_6       pd6_addr.bit.b6     /* Port P66 direction register */
#define     pd6_7       pd6_addr.bit.b7     /* Port P67 direction register */

/*------------------------------------------------------
    Port P7 direction register
------------------------------------------------------*/
union byte_def pd7_addr;
#define     pd7     pd7_addr.byte

#define     pd7_0       pd7_addr.bit.b0     /* Port P70 direction register */
#define     pd7_1       pd7_addr.bit.b1     /* Port P71 direction register */
#define     pd7_2       pd7_addr.bit.b2     /* Port P72 direction register */
#define     pd7_3       pd7_addr.bit.b3     /* Port P73 direction register */
#define     pd7_4       pd7_addr.bit.b4     /* Port P74 direction register */
#define     pd7_5       pd7_addr.bit.b5     /* Port P75 direction register */
#define     pd7_6       pd7_addr.bit.b6     /* Port P76 direction register */
#define     pd7_7       pd7_addr.bit.b7     /* Port P77 direction register */

/*------------------------------------------------------
    Port P8 register
------------------------------------------------------*/
union byte_def p8_addr;
#define     p8      p8_addr.byte

#define     p8_0        p8_addr.bit.b0      /* Port P80 register */
#define     p8_1        p8_addr.bit.b1      /* Port P81 register */
#define     p8_2        p8_addr.bit.b2      /* Port P82 register */
#define     p8_3        p8_addr.bit.b3      /* Port P83 register */
#define     p8_4        p8_addr.bit.b4      /* Port P84 register */
#define     p8_5        p8_addr.bit.b5      /* Port P85 register */
#define     p8_6        p8_addr.bit.b6      /* Port P86 register */
#define     p8_7        p8_addr.bit.b7      /* Port P87 register */

/*------------------------------------------------------
    Port P9 register
------------------------------------------------------*/
union byte_def p9_addr;
#define     p9      p9_addr.byte

#define     p9_0        p9_addr.bit.b0      /* Port P90 register */
#define     p9_1        p9_addr.bit.b1      /* Port P91 register */
#define     p9_2        p9_addr.bit.b2      /* Port P92 register */
#define     p9_3        p9_addr.bit.b3      /* Port P93 register */

/*------------------------------------------------------
    Port P8 direction register
------------------------------------------------------*/
union byte_def pd8_addr;
#define     pd8     pd8_addr.byte

#define     pd8_0       pd8_addr.bit.b0     /* Port P80 direction register */
#define     pd8_1       pd8_addr.bit.b1     /* Port P81 direction register */
#define     pd8_2       pd8_addr.bit.b2     /* Port P82 direction register */
#define     pd8_3       pd8_addr.bit.b3     /* Port P83 direction register */
#define     pd8_4       pd8_addr.bit.b4     /* Port P84 direction register */
#define     pd8_5       pd8_addr.bit.b5     /* Port P85 direction register */
#define     pd8_6       pd8_addr.bit.b6     /* Port P86 direction register */
#define     pd8_7       pd8_addr.bit.b7     /* Port P87 direction register */

/*------------------------------------------------------
    Port P9 direction register
------------------------------------------------------*/
union byte_def pd9_addr;
#define     pd9     pd9_addr.byte

#define     pd9_0       pd9_addr.bit.b0     /* Port P90 direction register */
#define     pd9_1       pd9_addr.bit.b1     /* Port P91 direction register */
#define     pd9_2       pd9_addr.bit.b2     /* Port P92 direction register */
#define     pd9_3       pd9_addr.bit.b3     /* Port P93 direction register */

/*------------------------------------------------------
    Port P10 register
------------------------------------------------------*/
union byte_def p10_addr;
#define     p10     p10_addr.byte

#define     p10_0       p10_addr.bit.b0     /* Port P100 register */
#define     p10_1       p10_addr.bit.b1     /* Port P101 register */
#define     p10_2       p10_addr.bit.b2     /* Port P102 register */
#define     p10_3       p10_addr.bit.b3     /* Port P103 register */
#define     p10_4       p10_addr.bit.b4     /* Port P104 register */
#define     p10_5       p10_addr.bit.b5     /* Port P105 register */
#define     p10_6       p10_addr.bit.b6     /* Port P106 register */
#define     p10_7       p10_addr.bit.b7     /* Port P107 register */

/*------------------------------------------------------
    Port P10 direction register
------------------------------------------------------*/
union byte_def pd10_addr;
#define     pd10        pd10_addr.byte

#define     pd10_0      pd10_addr.bit.b0    /* Port P100 direction register */
#define     pd10_1      pd10_addr.bit.b1    /* Port P101 direction register */
#define     pd10_2      pd10_addr.bit.b2    /* Port P102 direction register */
#define     pd10_3      pd10_addr.bit.b3    /* Port P103 direction register */
#define     pd10_4      pd10_addr.bit.b4    /* Port P104 direction register */
#define     pd10_5      pd10_addr.bit.b5    /* Port P105 direction register */
#define     pd10_6      pd10_addr.bit.b6    /* Port P106 direction register */
#define     pd10_7      pd10_addr.bit.b7    /* Port P107 direction register */

/*------------------------------------------------------
    Pull-up control register 0
------------------------------------------------------*/
union byte_def pur0_addr;
#define     pur0        pur0_addr.byte

#define     pu03        pur0_addr.bit.b3    /* P14 to P17 pull-up */

/*------------------------------------------------------
    Pull-up control register 1
------------------------------------------------------*/
union byte_def pur1_addr;
#define     pur1        pur1_addr.byte

#define     pu14        pur1_addr.bit.b4    /* P60 to P63 pull-up */
#define     pu15        pur1_addr.bit.b5    /* P64 to P67 pull-up */
#define     pu16        pur1_addr.bit.b6    /* P70 to P73 pull-up (Except P70,P71 ; P70,P71 -> N-channel open drain ports)*/
#define     pu17        pur1_addr.bit.b7    /* P74 to P77 pull-up */

/*------------------------------------------------------
    Pull-up control register 2
------------------------------------------------------*/
union byte_def pur2_addr;
#define     pur2        pur2_addr.byte

#define     pu20        pur2_addr.bit.b0    /* P80 to P83 pull-up */
#define     pu21        pur2_addr.bit.b1    /* P84 to P87 pull-up (Except P85) */
#define     pu22        pur2_addr.bit.b2    /* P90 to P93 pull-up */
#define     pu24        pur2_addr.bit.b4    /* P100 to P103 pull-up */
#define     pu25        pur2_addr.bit.b5    /* P104 to P107 pull-up */

/*------------------------------------------------------
    Port control register
------------------------------------------------------*/
union byte_def pcr_addr;
#define     pcr     pcr_addr.byte

#define     pcr0        pcr_addr.bit.b0     /* Port P1 control register */

/********************************************************
*   declare  SFR char                                   *
********************************************************/
/********************************************************
*   declare  SFR short                                  *
********************************************************/
/*--------------------------------------------------------
    Timer registers : Read and write data in 16-bit units.
--------------------------------------------------------*/
unsigned short   ta11_addr;             /* Timer A1-1 register */
#define     ta11    ta11_addr

unsigned short   ta21_addr;             /* Timer A2-1 register */
#define     ta21    ta21_addr

unsigned short   ta41_addr;             /* Timer A4-1 register */
#define     ta41    ta41_addr

unsigned short   ta0_addr;              /* Timer A0 register */
#define     ta0     ta0_addr

unsigned short   ta1_addr;              /* Timer A1 register */
#define     ta1     ta1_addr

unsigned short   ta2_addr;              /* Timer A2 register */
#define     ta2     ta2_addr

unsigned short   ta3_addr;              /* Timer A3 register */
#define     ta3     ta3_addr

unsigned short   ta4_addr;              /* Timer A4 register */
#define     ta4     ta4_addr

unsigned short   tb0_addr;              /* Timer B0 register */
#define     tb0     tb0_addr

unsigned short   tb1_addr;              /* Timer B1 register */
#define     tb1     tb1_addr

unsigned short   tb2_addr;              /* Timer B2 register */
#define     tb2     tb2_addr

/********************************************************
*   declare SFR union                                   *
********************************************************/

/*------------------------------------------------------
    DMAi control register union
------------------------------------------------------*/
union{
    struct{
        char    dmbit:1;            /* Transfer unit bit select bit */
        char    dmasl:1;            /* Repeat transfer mode select bit */
        char    dmas:1;             /* DMA request bit */
        char    dmae:1;             /* DMA enable bit */
        char    dsd:1;              /* Source address direction select bit */
        char    dad:1;              /* Destination address direction select bit */
        char    b6:1;
        char    b7:1;
    }bit;
    char    byte;
}dm0con_addr,dm1con_addr;
/*------------------------------------------------------
     dm0con
------------------------------------------------------*/
#define     dm0con      dm0con_addr.byte

#define     dmbit_dm0con    dm0con_addr.bit.dmbit   /* Transfer unit bit select bit */
#define     dmasl_dm0con    dm0con_addr.bit.dmasl   /* Repeat transfer mode select bit */
#define     dmas_dm0con     dm0con_addr.bit.dmas    /* DMA request bit */
#define     dmae_dm0con     dm0con_addr.bit.dmae    /* DMA enable bit */
#define     dsd_dm0con      dm0con_addr.bit.dsd     /* Source address direction select bit */
#define     dad_dm0con      dm0con_addr.bit.dad     /* Destination address direction select bit */

/*------------------------------------------------------
     dm1con
------------------------------------------------------*/
#define     dm1con      dm1con_addr.byte

#define     dmbit_dm1con    dm1con_addr.bit.dmbit   /* Transfer unit bit select bit */
#define     dmasl_dm1con    dm1con_addr.bit.dmasl   /* Repeat transfer mode select bit */
#define     dmas_dm1con     dm1con_addr.bit.dmas    /* DMA request bit */
#define     dmae_dm1con     dm1con_addr.bit.dmae    /* DMA enable bit */
#define     dsd_dm1con      dm1con_addr.bit.dsd     /* Source address direction select bit */
#define     dad_dm1con      dm1con_addr.bit.dad     /* Destination address direction select bit */

/*------------------------------------------------------
    interrupt control registers
------------------------------------------------------*/
union{
    struct{
        char    ilvl0:1;            /* Interrupt priority level select bit */
        char    ilvl1:1;            /* Interrupt priority level select bit */
        char    ilvl2:1;            /* Interrupt priority level select bit */
        char    ir:1;               /* Interrupt request bit */
        char    pol:1;              /* Polarity select bit */
        char    lvs:1;
        char    b6:1;
        char    b7:1;
    }bit;
    char    byte;
}int3ic_addr,int5ic_addr,int4ic_addr,
 bcnic_addr,dm0ic_addr,dm1ic_addr,kupic_addr,adic_addr,
 s2tic_addr,s2ric_addr,s0tic_addr,s0ric_addr,s1tic_addr,s1ric_addr,
 ta0ic_addr,ta1ic_addr,ta2ic_addr,ta3ic_addr,ta4ic_addr,
 tb0ic_addr,tb1ic_addr,tb2ic_addr,
 int0ic_addr,int1ic_addr,int2ic_addr;

/*------------------------------------------------------
     int3ic
------------------------------------------------------*/
#define     int3ic      int3ic_addr.byte

#define     ilvl0_int3ic    int3ic_addr.bit.ilvl0  /* Interrupt priority level select bit */
#define     ilvl1_int3ic    int3ic_addr.bit.ilvl1  /* Interrupt priority level select bit */
#define     ilvl2_int3ic    int3ic_addr.bit.ilvl2  /* Interrupt priority level select bit */
#define     ir_int3ic       int3ic_addr.bit.ir     /* Interrupt request bit */
#define     pol_int3ic      int3ic_addr.bit.pol    /* Polarity select bit */

/*------------------------------------------------------
     INT5~ interrupt control register
------------------------------------------------------*/
#define     int5ic      int5ic_addr.byte

#define     ilvl0_int5ic    int5ic_addr.bit.ilvl0  /* Interrupt priority level select bit */
#define     ilvl1_int5ic    int5ic_addr.bit.ilvl1  /* Interrupt priority level select bit */
#define     ilvl2_int5ic    int5ic_addr.bit.ilvl2  /* Interrupt priority level select bit */
#define     ir_int5ic       int5ic_addr.bit.ir     /* Interrupt request bit */
#define     pol_int5ic      int5ic_addr.bit.pol    /* Polarity select bit */

/*------------------------------------------------------
     INT4~ interrupt control register
------------------------------------------------------*/
#define     int4ic      int4ic_addr.byte

#define     ilvl0_int4ic    int4ic_addr.bit.ilvl0  /* Interrupt priority level select bit */
#define     ilvl1_int4ic    int4ic_addr.bit.ilvl1  /* Interrupt priority level select bit */
#define     ilvl2_int4ic    int4ic_addr.bit.ilvl2  /* Interrupt priority level select bit */
#define     ir_int4ic       int4ic_addr.bit.ir     /* Interrupt request bit */
#define     pol_int4ic      int4ic_addr.bit.pol    /* Polarity select bit */

/*------------------------------------------------------
     UART2 Bus collision detection interrupt control register
------------------------------------------------------*/
#define     bcnic       bcnic_addr.byte

#define     ilvl0_bcnic bcnic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_bcnic bcnic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_bcnic bcnic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_bcnic    bcnic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     DMA0 interrupt control register
------------------------------------------------------*/
#define     dm0ic       dm0ic_addr.byte

#define     ilvl0_dm0ic dm0ic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_dm0ic dm0ic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_dm0ic dm0ic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_dm0ic    dm0ic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     DMA1 interrupt control register
------------------------------------------------------*/
#define     dm1ic       dm1ic_addr.byte

#define     ilvl0_dm1ic dm1ic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_dm1ic dm1ic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_dm1ic dm1ic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_dm1ic    dm1ic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     Key input interrupt control register
------------------------------------------------------*/
#define     kupic       kupic_addr.byte

#define     ilvl0_kupic kupic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_kupic kupic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_kupic kupic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_kupic    kupic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     A-D conversion interrupt control register
------------------------------------------------------*/
#define     adic        adic_addr.byte

#define     ilvl0_adic  adic_addr.bit.ilvl0     /* Interrupt priority level select bit */
#define     ilvl1_adic  adic_addr.bit.ilvl1     /* Interrupt priority level select bit */
#define     ilvl2_adic  adic_addr.bit.ilvl2     /* Interrupt priority level select bit */
#define     ir_adic     adic_addr.bit.ir        /* Interrupt request bit */

/*------------------------------------------------------
     UART2 transmit interrupt control register
------------------------------------------------------*/
#define     s2tic       s2tic_addr.byte

#define     ilvl0_s2tic s2tic_addr.bit.ilvl0     /* Interrupt priority level select bit */
#define     ilvl1_s2tic s2tic_addr.bit.ilvl1     /* Interrupt priority level select bit */
#define     ilvl2_s2tic s2tic_addr.bit.ilvl2     /* Interrupt priority level select bit */
#define     ir_s2tic    s2tic_addr.bit.ir        /* Interrupt request bit */

/*------------------------------------------------------
     UART2 receive interrupt control register
------------------------------------------------------*/
#define     s2ric       s2ric_addr.byte

#define     ilvl0_s2ric s2ric_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_s2ric s2ric_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_s2ric s2ric_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_s2ric    s2ric_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     UART0 transmit interrupt control register
------------------------------------------------------*/
#define     s0tic       s0tic_addr.byte

#define     ilvl0_s0tic s0tic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_s0tic s0tic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_s0tic s0tic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_s0tic    s0tic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     UART0 receive interrupt control register
------------------------------------------------------*/
#define     s0ric       s0ric_addr.byte

#define     ilvl0_s0ric s0ric_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_s0ric s0ric_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_s0ric s0ric_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_s0ric    s0ric_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     UART1 transmit interrupt control register
------------------------------------------------------*/
#define     s1tic       s1tic_addr.byte

#define     ilvl0_s1tic s1tic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_s1tic s1tic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_s1tic s1tic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_s1tic    s1tic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     UART1 receive interrupt control register
------------------------------------------------------*/
#define     s1ric       s1ric_addr.byte

#define     ilvl0_s1ric s1ric_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_s1ric s1ric_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_s1ric s1ric_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_s1ric    s1ric_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     Timer A0 interrupt control register
------------------------------------------------------*/
#define     ta0ic       ta0ic_addr.byte

#define     ilvl0_ta0ic ta0ic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_ta0ic ta0ic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_ta0ic ta0ic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_ta0ic    ta0ic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     Timer A1 interrupt control register
------------------------------------------------------*/
#define     ta1ic       ta1ic_addr.byte

#define     ilvl0_ta1ic ta1ic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_ta1ic ta1ic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_ta1ic ta1ic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_ta1ic    ta1ic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     Timer A2 interrupt control register
------------------------------------------------------*/
#define     ta2ic       ta2ic_addr.byte

#define     ilvl0_ta2ic ta2ic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_ta2ic ta2ic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_ta2ic ta2ic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_ta2ic    ta2ic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     Timer A3 interrupt control register
------------------------------------------------------*/
#define     ta3ic       ta3ic_addr.byte

#define     ilvl0_ta3ic ta3ic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_ta3ic ta3ic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_ta3ic ta3ic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_ta3ic    ta3ic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     Timer A4 interrupt control register
------------------------------------------------------*/
#define     ta4ic       ta4ic_addr.byte

#define     ilvl0_ta4ic ta4ic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_ta4ic ta4ic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_ta4ic ta4ic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_ta4ic    ta4ic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     Timer B0 interrupt control register
------------------------------------------------------*/
#define     tb0ic       tb0ic_addr.byte

#define     ilvl0_tb0ic tb0ic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_tb0ic tb0ic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_tb0ic tb0ic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_tb0ic    tb0ic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     Timer B1 interrupt control register
------------------------------------------------------*/
#define     tb1ic       tb1ic_addr.byte

#define     ilvl0_tb1ic tb1ic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_tb1ic tb1ic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_tb1ic tb1ic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_tb1ic    tb1ic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     Timer B2 interrupt control register
------------------------------------------------------*/
#define     tb2ic       tb2ic_addr.byte

#define     ilvl0_tb2ic tb2ic_addr.bit.ilvl0    /* Interrupt priority level select bit */
#define     ilvl1_tb2ic tb2ic_addr.bit.ilvl1    /* Interrupt priority level select bit */
#define     ilvl2_tb2ic tb2ic_addr.bit.ilvl2    /* Interrupt priority level select bit */
#define     ir_tb2ic    tb2ic_addr.bit.ir       /* Interrupt request bit */

/*------------------------------------------------------
     INT0~ interrupt control register
------------------------------------------------------*/
#define     int0ic      int0ic_addr.byte

#define     ilvl0_int0ic    int0ic_addr.bit.ilvl0  /* Interrupt priority level select bit */
#define     ilvl1_int0ic    int0ic_addr.bit.ilvl1  /* Interrupt priority level select bit */
#define     ilvl2_int0ic    int0ic_addr.bit.ilvl2  /* Interrupt priority level select bit */
#define     ir_int0ic       int0ic_addr.bit.ir     /* Interrupt request bit */
#define     pol_int0ic      int0ic_addr.bit.pol    /* Polarity select bit */

/*------------------------------------------------------
     INT1~ interrupt control register
------------------------------------------------------*/
#define     int1ic      int1ic_addr.byte

#define     ilvl0_int1ic    int1ic_addr.bit.ilvl0  /* Interrupt priority level select bit */
#define     ilvl1_int1ic    int1ic_addr.bit.ilvl1  /* Interrupt priority level select bit */
#define     ilvl2_int1ic    int1ic_addr.bit.ilvl2  /* Interrupt priority level select bit */
#define     ir_int1ic       int1ic_addr.bit.ir     /* Interrupt request bit */
#define     pol_int1ic      int1ic_addr.bit.pol    /* Polarity select bit */

/*------------------------------------------------------
     INT2~ interrupt control register
------------------------------------------------------*/
#define     int2ic      int2ic_addr.byte

#define     ilvl0_int2ic    int2ic_addr.bit.ilvl0  /* Interrupt priority level select bit */
#define     ilvl1_int2ic    int2ic_addr.bit.ilvl1  /* Interrupt priority level select bit */
#define     ilvl2_int2ic    int2ic_addr.bit.ilvl2  /* Interrupt priority level select bit */
#define     ir_int2ic       int2ic_addr.bit.ir     /* Interrupt request bit */
#define     pol_int2ic      int2ic_addr.bit.pol    /* Polarity select bit */

/*------------------------------------------------------
    Flash memory control register 4
------------------------------------------------------*/
union byte_def fmr4_addr;
#define     fmr4        fmr4_addr.byte

#define     fmr40       fmr4_addr.bit.b0  /* Suspend enable */
#define     fmr41       fmr4_addr.bit.b1  /* Suspend request */
#define     fmr46       fmr4_addr.bit.b6  /* Suspend status */

/*------------------------------------------------------
    Flash memory control register 1
------------------------------------------------------*/
union byte_def fmr1_addr;
#define     fmr1    fmr1_addr.byte

#define     fmr11   fmr1_addr.bit.b1    /* EW1 mode select bit */
#define     fmr16   fmr1_addr.bit.b6    /* Write protect bit 1 */
#define     fmr17   fmr1_addr.bit.b7    /* Block A,B access wait bit */

/*------------------------------------------------------
    Flash memory control register 0
------------------------------------------------------*/
union byte_def fmr0_addr;
#define     fmr0    fmr0_addr.byte

#define     fmr00   fmr0_addr.bit.b0     /* RY/BY status flag */
#define     fmr01   fmr0_addr.bit.b1     /* EW entry bit */
#define     fmr02   fmr0_addr.bit.b2     /* Block 0,1 write enable bit */
#define     fmstp   fmr0_addr.bit.b3     /* Flash memory stop bit */
#define     fmr06   fmr0_addr.bit.b6     /* Program status flag */
#define     fmr07   fmr0_addr.bit.b7     /* Erase status flag */

/*------------------------------------------------------
    Timer mode registers
------------------------------------------------------*/
union{
    struct{
        char    tmod0:1;            /* Operation mode select bit */
        char    tmod1:1;            /* Operation mode select bit */
        char    mr0:1;
        char    mr1:1;
        char    mr2:1;
        char    mr3:1;
        char    tck0:1;             /* Count source select bit */
        char    tck1:1;             /* Count source select bit */
    }bit;
    char    byte;
}ta0mr_addr,ta1mr_addr,ta2mr_addr,ta3mr_addr,ta4mr_addr,
 tb0mr_addr,tb1mr_addr,tb2mr_addr;

/*------------------------------------------------------
     Timer A0 mode registers
------------------------------------------------------*/
#define     ta0mr       ta0mr_addr.byte

#define     tmod0_ta0mr ta0mr_addr.bit.tmod0        /* Operation mode select bit */
#define     tmod1_ta0mr ta0mr_addr.bit.tmod1        /* Operation mode select bit */
#define     mr0_ta0mr   ta0mr_addr.bit.mr0
#define     mr1_ta0mr   ta0mr_addr.bit.mr1
#define     mr2_ta0mr   ta0mr_addr.bit.mr2
#define     mr3_ta0mr   ta0mr_addr.bit.mr3
#define     tck0_ta0mr  ta0mr_addr.bit.tck0         /* Count source select bit */
#define     tck1_ta0mr  ta0mr_addr.bit.tck1         /* Count source select bit */

/*------------------------------------------------------
     Timer A1 mode registers
------------------------------------------------------*/
#define     ta1mr       ta1mr_addr.byte

#define     tmod0_ta1mr ta1mr_addr.bit.tmod0        /* Operation mode select bit */
#define     tmod1_ta1mr ta1mr_addr.bit.tmod1        /* Operation mode select bit */
#define     mr0_ta1mr   ta1mr_addr.bit.mr0
#define     mr1_ta1mr   ta1mr_addr.bit.mr1
#define     mr2_ta1mr   ta1mr_addr.bit.mr2
#define     mr3_ta1mr   ta1mr_addr.bit.mr3
#define     tck0_ta1mr  ta1mr_addr.bit.tck0         /* Count source select bit */
#define     tck1_ta1mr  ta1mr_addr.bit.tck1         /* Count source select bit */

/*------------------------------------------------------
     Timer A2 mode registers
------------------------------------------------------*/
#define     ta2mr       ta2mr_addr.byte

#define     tmod0_ta2mr ta2mr_addr.bit.tmod0        /* Operation mode select bit */
#define     tmod1_ta2mr ta2mr_addr.bit.tmod1        /* Operation mode select bit */
#define     mr0_ta2mr   ta2mr_addr.bit.mr0
#define     mr1_ta2mr   ta2mr_addr.bit.mr1
#define     mr2_ta2mr   ta2mr_addr.bit.mr2
#define     mr3_ta2mr   ta2mr_addr.bit.mr3
#define     tck0_ta2mr  ta2mr_addr.bit.tck0         /* Count source select bit */
#define     tck1_ta2mr  ta2mr_addr.bit.tck1         /* Count source select bit */

/*------------------------------------------------------
     Timer A3 mode registers
------------------------------------------------------*/
#define     ta3mr       ta3mr_addr.byte

#define     tmod0_ta3mr ta3mr_addr.bit.tmod0        /* Operation mode select bit */
#define     tmod1_ta3mr ta3mr_addr.bit.tmod1        /* Operation mode select bit */
#define     mr0_ta3mr   ta3mr_addr.bit.mr0
#define     mr1_ta3mr   ta3mr_addr.bit.mr1
#define     mr2_ta3mr   ta3mr_addr.bit.mr2
#define     mr3_ta3mr   ta3mr_addr.bit.mr3
#define     tck0_ta3mr  ta3mr_addr.bit.tck0         /* Count source select bit */
#define     tck1_ta3mr  ta3mr_addr.bit.tck1         /* Count source select bit */

/*------------------------------------------------------
     Timer A4 mode registers
------------------------------------------------------*/
#define     ta4mr       ta4mr_addr.byte

#define     tmod0_ta4mr ta4mr_addr.bit.tmod0        /* Operation mode select bit */
#define     tmod1_ta4mr ta4mr_addr.bit.tmod1        /* Operation mode select bit */
#define     mr0_ta4mr   ta4mr_addr.bit.mr0
#define     mr1_ta4mr   ta4mr_addr.bit.mr1
#define     mr2_ta4mr   ta4mr_addr.bit.mr2
#define     mr3_ta4mr   ta4mr_addr.bit.mr3
#define     tck0_ta4mr  ta4mr_addr.bit.tck0         /* Count source select bit */
#define     tck1_ta4mr  ta4mr_addr.bit.tck1         /* Count source select bit */

/*------------------------------------------------------
     Timer B0 mode registers
 ------------------------------------------------------*/
#define     tb0mr       tb0mr_addr.byte

#define     tmod0_tb0mr tb0mr_addr.bit.tmod0        /* Operation mode select bit */
#define     tmod1_tb0mr tb0mr_addr.bit.tmod1        /* Operation mode select bit */
#define     mr0_tb0mr   tb0mr_addr.bit.mr0
#define     mr1_tb0mr   tb0mr_addr.bit.mr1
#define     mr2_tb0mr   tb0mr_addr.bit.mr2
#define     mr3_tb0mr   tb0mr_addr.bit.mr3
#define     tck0_tb0mr  tb0mr_addr.bit.tck0         /* Count source select bit */
#define     tck1_tb0mr  tb0mr_addr.bit.tck1         /* Count source select bit */

/*------------------------------------------------------
     Timer B1 mode registers
------------------------------------------------------*/
#define     tb1mr       tb1mr_addr.byte

#define     tmod0_tb1mr tb1mr_addr.bit.tmod0        /* Operation mode select bit */
#define     tmod1_tb1mr tb1mr_addr.bit.tmod1        /* Operation mode select bit */
#define     mr0_tb1mr   tb1mr_addr.bit.mr0
#define     mr1_tb1mr   tb1mr_addr.bit.mr1
#define     mr2_tb1mr   tb1mr_addr.bit.mr2
#define     mr3_tb1mr   tb1mr_addr.bit.mr3
#define     tck0_tb1mr  tb1mr_addr.bit.tck0         /* Count source select bit */
#define     tck1_tb1mr  tb1mr_addr.bit.tck1         /* Count source select bit */

/*------------------------------------------------------
     Timer B2 mode registers
------------------------------------------------------*/
#define     tb2mr       tb2mr_addr.byte

#define     tmod0_tb2mr tb2mr_addr.bit.tmod0        /* Operation mode select bit */
#define     tmod1_tb2mr tb2mr_addr.bit.tmod1        /* Operation mode select bit */
#define     mr0_tb2mr   tb2mr_addr.bit.mr0
#define     mr1_tb2mr   tb2mr_addr.bit.mr1
#define     mr2_tb2mr   tb2mr_addr.bit.mr2
#define     mr3_tb2mr   tb2mr_addr.bit.mr3
#define     tck0_tb2mr  tb2mr_addr.bit.tck0         /* Count source select bit */
#define     tck1_tb2mr  tb2mr_addr.bit.tck1         /* Count source select bit */

/*------------------------------------------------------
    UARTi transmit/receive mode register
------------------------------------------------------*/
union{
    struct{
        char    smd0:1;             /* Serial I/O mode select bit */
        char    smd1:1;             /* Serial I/O mode select bit */
        char    smd2:1;             /* Serial I/O mode select bit */
        char    ckdir:1;            /* Internal/external clock select bit */
        char    stps:1;             /* Stop bit length select bit */
        char    pry:1;              /* Odd/even parity select bit */
        char    prye:1;             /* Parity enable bit */
        char    iopol:1;            /* TxD, RxD I/O polarity reverse bit */
    }bit;
    char    byte;
}u2mr_addr,u0mr_addr,u1mr_addr;

/*------------------------------------------------------
     UART2 transmit/receive mode register
------------------------------------------------------*/
#define     u2mr        u2mr_addr.byte

#define     smd0_u2mr   u2mr_addr.bit.smd0          /* Serial I/O mode select bit */
#define     smd1_u2mr   u2mr_addr.bit.smd1          /* Serial I/O mode select bit */
#define     smd2_u2mr   u2mr_addr.bit.smd2          /* Serial I/O mode select bit */
#define     ckdir_u2mr  u2mr_addr.bit.ckdir         /* Internal/external clock select bit */
#define     stps_u2mr   u2mr_addr.bit.stps          /* Stop bit length select bit */
#define     pry_u2mr    u2mr_addr.bit.pry           /* Odd/even parity select bit */
#define     prye_u2mr   u2mr_addr.bit.prye          /* Parity enable bit */
#define     iopol_u2mr  u2mr_addr.bit.iopol         /* TxD RxD I/O polarity reverse bit */

/*------------------------------------------------------
     UART0 transmit/receive mode register
------------------------------------------------------*/
#define     u0mr        u0mr_addr.byte

#define     smd0_u0mr   u0mr_addr.bit.smd0           /* Serial I/O mode select bit */
#define     smd1_u0mr   u0mr_addr.bit.smd1           /* Serial I/O mode select bit */
#define     smd2_u0mr   u0mr_addr.bit.smd2           /* Serial I/O mode select bit */
#define     ckdir_u0mr  u0mr_addr.bit.ckdir          /* Internal/external clock select bit */
#define     stps_u0mr   u0mr_addr.bit.stps           /* Stop bit length select bit */
#define     pry_u0mr    u0mr_addr.bit.pry            /* Odd/even parity select bit */
#define     prye_u0mr   u0mr_addr.bit.prye           /* Parity enable bit */

/*------------------------------------------------------
     UART1 transmit/receive mode register
------------------------------------------------------*/
#define     u1mr        u1mr_addr.byte

#define     smd0_u1mr   u1mr_addr.bit.smd0          /* Serial I/O mode select bit */
#define     smd1_u1mr   u1mr_addr.bit.smd1          /* Serial I/O mode select bit */
#define     smd2_u1mr   u1mr_addr.bit.smd2          /* Serial I/O mode select bit */
#define     ckdir_u1mr  u1mr_addr.bit.ckdir         /* Internal/external clock select bit */
#define     stps_u1mr   u1mr_addr.bit.stps          /* Stop bit length select bit */
#define     pry_u1mr    u1mr_addr.bit.pry           /* Odd/even parity select bit */
#define     prye_u1mr   u1mr_addr.bit.prye          /* Parity enable bit */

/*------------------------------------------------------
    UARTi transmit/receive control register 0
------------------------------------------------------*/
union{
    struct{
        char    clk0:1;             /* BRG count source select bit */
        char    clk1:1;             /* BRG count source select bit */
        char    crs:1;              /* CTS~/RTS~ function select bit */
        char    txept:1;            /* Transmit register empty flag */
        char    crd:1;              /* CTS~/RTS~ disable bit */
        char    nch:1;              /* Data output select bit */
        char    ckpol:1;            /* CLK polarity select bit */
        char    uform:1;            /* Transfer format select bit */
    }bit;
    char    byte;
}u0c0_addr,u1c0_addr,u2c0_addr;

/*------------------------------------------------------
     UART2 transmit/receive control register 0
------------------------------------------------------*/
#define     u2c0        u2c0_addr.byte

#define     clk0_u2c0   u2c0_addr.bit.clk0          /* BRG count source select bit */
#define     clk1_u2c0   u2c0_addr.bit.clk1          /* BRG count source select bit */
#define     crs_u2c0    u2c0_addr.bit.crs           /* CTS~/RTS~ function select bit */
#define     txept_u2c0  u2c0_addr.bit.txept         /* Transmit register empty flag */
#define     crd_u2c0    u2c0_addr.bit.crd           /* CTS~/RTS~ disable bit */
#define     nch_u2c0    u2c0_addr.bit.nch           /* Data output select bit */
#define     ckpol_u2c0  u2c0_addr.bit.ckpol         /* CLK polarity select bit */
#define     uform_u2c0  u2c0_addr.bit.uform         /* Transfer format select bit */

/*------------------------------------------------------
     UART0 transmit/receive control register 0
------------------------------------------------------*/
#define     u0c0        u0c0_addr.byte

#define     clk0_u0c0   u0c0_addr.bit.clk0          /* BRG count source select bit */
#define     clk1_u0c0   u0c0_addr.bit.clk1          /* BRG count source select bit */
#define     crs_u0c0    u0c0_addr.bit.crs           /* CTS~/RTS~ function select bit */
#define     txept_u0c0  u0c0_addr.bit.txept         /* Transmit register empty flag */
#define     crd_u0c0    u0c0_addr.bit.crd           /* CTS~/RTS~ disable bit */
#define     nch_u0c0    u0c0_addr.bit.nch           /* Data output select bit */
#define     ckpol_u0c0  u0c0_addr.bit.ckpol         /* CLK polarity select bit */
#define     uform_u0c0  u0c0_addr.bit.uform         /* Transfer format select bit */

/*------------------------------------------------------
     UART1 transmit/receive control register 0
------------------------------------------------------*/
#define     u1c0        u1c0_addr.byte

#define     clk0_u1c0   u1c0_addr.bit.clk0          /* BRG count source select bit */
#define     clk1_u1c0   u1c0_addr.bit.clk1          /* BRG count source select bit */
#define     crs_u1c0    u1c0_addr.bit.crs           /* CTS~/RTS~ function select bit */
#define     txept_u1c0  u1c0_addr.bit.txept         /* Transmit register empty flag */
#define     crd_u1c0    u1c0_addr.bit.crd           /* CTS~/RTS~ disable bit */
#define     nch_u1c0    u1c0_addr.bit.nch           /* Data output select bit */
#define     ckpol_u1c0  u1c0_addr.bit.ckpol         /* CLK polarity select bit */
#define     uform_u1c0  u1c0_addr.bit.uform         /* Transfer format select bit */

/*------------------------------------------------------
    UARTi transmit/receive control register 1
------------------------------------------------------*/
union{
    struct{
        char    te:1;               /* Transmit enable bit */
        char    ti:1;               /* Transmit buffer empty flag */
        char    re:1;               /* Receive enable bit */
        char    ri:1;               /* Receive complete flag */
        char    b4:1;
        char    b5:1;
        char    b6:1;
        char    b7:1;
    }bit;
    char    byte;
}u0c1_addr,u1c1_addr;

/*------------------------------------------------------
     u0c1
------------------------------------------------------*/
#define     u0c1        u0c1_addr.byte

#define     te_u0c1     u0c1_addr.bit.te            /* Transmit enable bit */
#define     ti_u0c1     u0c1_addr.bit.ti            /* Transmit buffer empty flag */
#define     re_u0c1     u0c1_addr.bit.re            /* Receive enable bit */
#define     ri_u0c1     u0c1_addr.bit.ri            /* Receive complete flag */

/*------------------------------------------------------
     u1c1
------------------------------------------------------*/
#define     u1c1        u1c1_addr.byte

#define     te_u1c1     u1c1_addr.bit.te            /* Transmit enable bit */
#define     ti_u1c1     u1c1_addr.bit.ti            /* Transmit buffer empty flag */
#define     re_u1c1     u1c1_addr.bit.re            /* Receive enable bit */
#define     ri_u1c1     u1c1_addr.bit.ri            /* Receive complete flag */

/*------------------------------------------------------
    UARTi receive buffer registers
------------------------------------------------------*/
union{
    struct{
        char    b0:1;
        char    b1:1;
        char    b2:1;
        char    b3:1;
        char    b4:1;
        char    b5:1;
        char    b6:1;
        char    b7:1;
        char    b8:1;
        char    b9:1;
        char    b10:1;
        char    abt:1;              /* Arbitration lost detecting flag */
        char    oer:1;              /* Overrun error flag */
        char    fer:1;              /* Framing error flag*/
        char    per:1;              /* Parity error flag */
        char    sum:1;              /* Error sum flag */
    }bit;
    struct{
        char    low;                /* Low  8 bit */
        char    high;               /* High 8 bit */
    }byte;
    unsigned short  word;
}u0rb_addr,u1rb_addr,u2rb_addr;

/*------------------------------------------------------
     UART2 receive buffer register
------------------------------------------------------*/
#define     u2rb        u2rb_addr.word
#define     u2rbl       u2rb_addr.byte.low
#define     u2rbh       u2rb_addr.byte.high

#define     abt_u2rb    u2rb_addr.bit.abt           /* Arbitration lost detecting flag */
#define     oer_u2rb    u2rb_addr.bit.oer           /* Overrun error flag */
#define     fer_u2rb    u2rb_addr.bit.fer           /* Framing error flag*/
#define     per_u2rb    u2rb_addr.bit.per           /* Parity error flag */
#define     sum_u2rb    u2rb_addr.bit.sum           /* Error sum flag */

/*------------------------------------------------------
     UART0 receive buffer register
------------------------------------------------------*/
#define     u0rb        u0rb_addr.word
#define     u0rbl       u0rb_addr.byte.low
#define     u0rbh       u0rb_addr.byte.high

#define     oer_u0rb    u0rb_addr.bit.oer           /* Overrun error flag */
#define     fer_u0rb    u0rb_addr.bit.fer           /* Framing error flag*/
#define     per_u0rb    u0rb_addr.bit.per           /* Parity error flag */
#define     sum_u0rb    u0rb_addr.bit.sum           /* Error sum flag */

/*------------------------------------------------------
     UART1 receive buffer register
------------------------------------------------------*/
#define     u1rb        u1rb_addr.word
#define     u1rbl       u1rb_addr.byte.low
#define     u1rbh       u1rb_addr.byte.high

#define     oer_u1rb    u1rb_addr.bit.oer           /* Overrun error flag */
#define     fer_u1rb    u1rb_addr.bit.fer           /* Framing error flag*/
#define     per_u1rb    u1rb_addr.bit.per           /* Parity error flag */
#define     sum_u1rb    u1rb_addr.bit.sum           /* Error sum flag */

/*------------------------------------------------------
    DMAi request cause select registers
------------------------------------------------------*/
union{
    struct{
        char    dsel0:1;            /* DMA request cause select bit */
        char    dsel1:1;            /* DMA request cause select bit */
        char    dsel2:1;            /* DMA request cause select bit */
        char    dsel3:1;            /* DMA request cause select bit */
        char    b4:1;
        char    b5:1;
        char    dms:1;              /* DMA request cause expansion bit */
        char    dsr:1;              /* Software DMA request bit */
    }bit;
    char    byte;
}dm0sl_addr,dm1sl_addr;

/*------------------------------------------------------
     dm0sl
------------------------------------------------------*/
#define     dm0sl       dm0sl_addr.byte

#define     dsel0_dm0sl dm0sl_addr.bit.dsel0        /* DMA request cause select bit */
#define     dsel1_dm0sl dm0sl_addr.bit.dsel1        /* DMA request cause select bit */
#define     dsel2_dm0sl dm0sl_addr.bit.dsel2        /* DMA request cause select bit */
#define     dsel3_dm0sl dm0sl_addr.bit.dsel3        /* DMA request cause select bit */
#define     dms_dm0sl   dm0sl_addr.bit.dms          /* DMA request cause expansion bit */
#define     dsr_dm0sl   dm0sl_addr.bit.dsr          /* Software DMA request bit */

/*------------------------------------------------------
     dm1sl
------------------------------------------------------*/
#define     dm1sl       dm1sl_addr.byte

#define     dsel0_dm1sl dm1sl_addr.bit.dsel0        /* DMA request cause select bit */
#define     dsel1_dm1sl dm1sl_addr.bit.dsel1        /* DMA request cause select bit */
#define     dsel2_dm1sl dm1sl_addr.bit.dsel2        /* DMA request cause select bit */
#define     dsel3_dm1sl dm1sl_addr.bit.dsel3        /* DMA request cause select bit */
#define     dms_dm1sl   dm1sl_addr.bit.dms          /* DMA request cause expansion bit */
#define     dsr_dm1sl   dm1sl_addr.bit.dsr          /* Software DMA request bit */

/********************************************************
*   declare SFR union                                   *
********************************************************/

union{
    struct{
        char    b0:1;
        char    b1:1;
        char    b2:1;
        char    b3:1;
        char    b4:1;
        char    b5:1;
        char    b6:1;
        char    b7:1;
        char    b8:1;
        char    b9:1;
        char    b10:1;
        char    b11:1;
        char    b12:1;
        char    b13:1;
        char    b14:1;
        char    b15:1;
        char    b16:1;
        char    b17:1;
        char    b18:1;
        char    b19:1;
    }bit;
    struct{
        char    low;                /* low  8 bit */
        char    mid;                /* mid  8 bit */
        char    high;               /* high 8 bit */
        char    nc;                 /* non use */
    }byte;
    unsigned long   dword;
}rmad0_addr,rmad1_addr,sar0_addr,dar0_addr,sar1_addr,dar1_addr;
#define     rmad0       rmad0_addr.dword        /* Address match interrupt register 0 32 bit */
#define     rmad0l      rmad0_addr.byte.low         /* Address match interrupt register 0 low  8 bit */
#define     rmad0m      rmad0_addr.byte.mid         /* Address match interrupt register 0 mid  8 bit */
#define     rmad0h      rmad0_addr.byte.high        /* Address match interrupt register 0 high 8 bit */
#define     rmad1       rmad1_addr.dword        /* Address match interrupt register 1 32 bit */
#define     rmad1l      rmad1_addr.byte.low         /* Address match interrupt register 1 low  8 bit */
#define     rmad1m      rmad1_addr.byte.mid         /* Address match interrupt register 1 mid  8 bit */
#define     rmad1h      rmad1_addr.byte.high        /* Address match interrupt register 1 high 8 bit */
#define     sar0        sar0_addr.dword         /* DMA0 source pointer 32 bit */
#define     sar0l       sar0_addr.byte.low          /* DMA0 source pointer low  8 bit */
#define     sar0m       sar0_addr.byte.mid          /* DMA0 source pointer mid  8 bit */
#define     sar0h       sar0_addr.byte.high         /* DMA0 source pointer high 8 bit */
#define     dar0        dar0_addr.dword         /* DMA0 destination pointer 32 bit */
#define     dar0l       dar0_addr.byte.low          /* DMA0 destination pointer low  8 bit */
#define     dar0m       dar0_addr.byte.mid          /* DMA0 destination pointer mid  8 bit */
#define     dar0h       dar0_addr.byte.high         /* DMA0 destination pointer high 8 bit */
#define     sar1        sar1_addr.dword         /* DMA1 source pointer 32 bit */
#define     sar1l       sar1_addr.byte.low          /* DMA1 source pointer low  8 bit */
#define     sar1m       sar1_addr.byte.mid          /* DMA1 source pointer mid  8 bit */
#define     sar1h       sar1_addr.byte.high         /* DMA1 source pointer high 8 bit */
#define     dar1        dar1_addr.dword         /* DMA1 destination pointer 32 bit */
#define     dar1l       dar1_addr.byte.low          /* DMA1 destination pointer low  8 bit */
#define     dar1m       dar1_addr.byte.mid          /* DMA1 destination pointer mid  8 bit */
#define     dar1h       dar1_addr.byte.high         /* DMA1 destination pointer high 8 bit */

union{
    struct{
        char    b0:1;
        char    b1:1;
        char    b2:1;
        char    b3:1;
        char    b4:1;
        char    b5:1;
        char    b6:1;
        char    b7:1;
        char    b8:1;
        char    b9:1;
        char    b10:1;
        char    b11:1;
        char    b12:1;
        char    b13:1;
        char    b14:1;
        char    b15:1;
    }bit;
    struct{
        char    low;                /* low  8 bit */
        char    high;               /* high 8 bit */
    }byte;
    unsigned short  word;
}tcr0_addr,tcr1_addr,
 u2tb_addr,u0tb_addr,u1tb_addr,crcd_addr,
 ad0_addr,ad1_addr,ad2_addr,ad3_addr,ad4_addr,ad5_addr,ad6_addr,ad7_addr,crcsar_addr;

#define     tcr0    tcr0_addr.word             /* DMA0 transfer counter 16 bit */
#define     tcr0l   tcr0_addr.byte.low              /* DMA0 transfer counter low  8 bit */
#define     tcr0h   tcr0_addr.byte.high             /* DMA0 transfer counter high 8 bit */
#define     tcr1    tcr1_addr.word             /* DMA1 transfer counter 16 bit */
#define     tcr1l   tcr1_addr.byte.low              /* DMA1 transfer counter low  8 bit */
#define     tcr1h   tcr1_addr.byte.high             /* DMA1 transfer counter high 8 bit */
#define     u2tb    u2tb_addr.word              /* UART2 Transmit buffer register 16 bit ; Use "MOV" instruction when writing to this register. */
#define     u2tbl   u2tb_addr.byte.low              /* UART2 Transmit buffer register low  8 bit */
#define     u2tbh   u2tb_addr.byte.high             /* UART2 Transmit buffer register high 8 bit */
#define     u0tb    u0tb_addr.word              /* UART0 Transmit buffer register 16 bit ; Use "MOV" instruction when writing to this register. */
#define     u0tbl   u0tb_addr.byte.low              /* UART0 Transmit buffer register low  8 bit */
#define     u0tbh   u0tb_addr.byte.high             /* UART0 Transmit buffer register high 8 bit */
#define     u1tb    u1tb_addr.word              /* UART1 Transmit buffer register 16 bit ; Use "MOV" instruction when writing to this register. */
#define     u1tbl   u1tb_addr.byte.low              /* UART1 Transmit buffer register low  8 bit */
#define     u1tbh   u1tb_addr.byte.high             /* UART1 Transmit buffer register high 8 bit */
#define     crcd     crcd_addr.word             /* CRC data register */
#define     crcdl    crcd_addr.byte.low             /* CRC data register low */
#define     crcdh    crcd_addr.byte.high            /* CRC data register high */
#define     ad0     ad0_addr.word               /* A-D register 0 16 bit */
#define     ad0l    ad0_addr.byte.low               /* A-D register 0 low  8 bit */
#define     ad0h    ad0_addr.byte.high              /* A-D register 0 high 8 bit */
#define     ad1     ad1_addr.word               /* A-D register 1 16 bit */
#define     ad1l    ad1_addr.byte.low               /* A-D register 1 low  8 bit */
#define     ad1h    ad1_addr.byte.high              /* A-D register 1 high 8 bit */
#define     ad2     ad2_addr.word               /* A-D register 2 16 bit */
#define     ad2l    ad2_addr.byte.low               /* A-D register 2 low  8 bit */
#define     ad2h    ad2_addr.byte.high              /* A-D register 2 high 8 bit */
#define     ad3     ad3_addr.word               /* A-D register 3 16 bit */
#define     ad3l    ad3_addr.byte.low               /* A-D register 3 low  8 bit */
#define     ad3h    ad3_addr.byte.high              /* A-D register 3 high 8 bit */
#define     ad4     ad4_addr.word               /* A-D register 4 16 bit */
#define     ad4l    ad4_addr.byte.low               /* A-D register 4 low  8 bit */
#define     ad4h    ad4_addr.byte.high              /* A-D register 4 high 8 bit */
#define     ad5     ad5_addr.word               /* A-D register 5 16 bit */
#define     ad5l    ad5_addr.byte.low               /* A-D register 5 low  8 bit */
#define     ad5h    ad5_addr.byte.high              /* A-D register 5 high 8 bit */
#define     ad6     ad6_addr.word               /* A-D register 6 16 bit */
#define     ad6l    ad6_addr.byte.low               /* A-D register 6 low  8 bit */
#define     ad6h    ad6_addr.byte.high              /* A-D register 6 high 8 bit */
#define     ad7     ad7_addr.word               /* A-D register 7 16 bit */
#define     ad7l    ad7_addr.byte.low               /* A-D register 7 low  8 bit */
#define     ad7h    ad7_addr.byte.high              /* A-D register 7 high 8 bit */
#define     crcsar     crcsar_addr.word             /* sfr address register */
#define     crcsarl    crcsar_addr.byte.low             /* sfr address register low */
#define     crcsarh    crcsar_addr.byte.high            /* sfr address register high */
#define     crcsar0    crcsar_addr.bit.b0               /* sfr address register b0 */
#define     crcsar1    crcsar_addr.bit.b1               /* sfr address register b1 */
#define     crcsar2    crcsar_addr.bit.b2               /* sfr address register b2 */
#define     crcsar3    crcsar_addr.bit.b3               /* sfr address register b3 */
#define     crcsar4    crcsar_addr.bit.b4               /* sfr address register b4 */
#define     crcsar5    crcsar_addr.bit.b5               /* sfr address register b5 */
#define     crcsar6    crcsar_addr.bit.b6               /* sfr address register b6 */
#define     crcsar7    crcsar_addr.bit.b7               /* sfr address register b7 */
#define     crcsar8    crcsar_addr.bit.b8               /* sfr address register b8 */
#define     crcsar9    crcsar_addr.bit.b9               /* sfr address register b9 */
#define     crcsr      crcsar_addr.bit.b14              /* sfr address register b14 */
#define     crcsw      crcsar_addr.bit.b15              /* sfr address register b15 */

/********************************************************
*   declare SFR bit                                     *
********************************************************/
struct slot_bit_def {
        char    slot0:1;
        char    slot1:1;
        char    slot2:1;
        char    slot3:1;
        char    slot4:1;
        char    slot5:1;
        char    slot6:1;
        char    slot7:1;
        char    slot8:1;
        char    slot9:1;
        char    slot10:1;
        char    slot11:1;
        char    slot12:1;
        char    slot13:1;
        char    slot14:1;
        char    slot15:1;
};
union slot_def{
    struct slot_bit_def b;
    short   word;
};

/********************************************************
*   declare SFR bit                                     *
********************************************************/
struct bit_word_def {
        char    b0:1;
        char    b1:1;
        char    b2:1;
        char    b3:1;
        char    b4:1;
        char    b5:1;
        char    b6:1;
        char    b7:1;
        char    b8:1;
        char    b9:1;
        char    b10:1;
        char    b11:1;
        char    b12:1;
        char    b13:1;
        char    b14:1;
        char    b15:1;
};
union word_def{
    struct bit_word_def b;
    struct{
        char    low;                /* Lower byte */
        char    high;               /* Higher byte */
    }byte;
    short   word;
};

