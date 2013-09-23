/**************************************************************************//**
 * @file     LPC13xx.h
 * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File for
 *           NXP LPC13xx Device Series
 * @version  V1.01
 * @date     19. October 2009
 *
 * @note
 * Copyright (C) 2009 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#ifndef __LPC13xx_H__
#define __LPC13xx_H__

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
  MemoryManagement_IRQn         = -12,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
  BusFault_IRQn                 = -11,      /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
  UsageFault_IRQn               = -10,      /*!< 6 Cortex-M3 Usage Fault Interrupt                */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M3 SV Call Interrupt                   */
  DebugMonitor_IRQn             = -4,       /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M3 Pend SV Interrupt                   */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC13xx Specific Interrupt Numbers *******************************************************/
  WAKEUP0_IRQn                  = 0,        /*!< All I/O pins can be used as wakeup source.       */
  WAKEUP1_IRQn                  = 1,        /*!< There are 40 pins in total for LPC17xx           */
  WAKEUP2_IRQn                  = 2,
  WAKEUP3_IRQn                  = 3,
  WAKEUP4_IRQn                  = 4,
  WAKEUP5_IRQn                  = 5,
  WAKEUP6_IRQn                  = 6,
  WAKEUP7_IRQn                  = 7,
  WAKEUP8_IRQn                  = 8,
  WAKEUP9_IRQn                  = 9,
  WAKEUP10_IRQn                 = 10,
  WAKEUP11_IRQn                 = 11,
  WAKEUP12_IRQn                 = 12,
  WAKEUP13_IRQn                 = 13,
  WAKEUP14_IRQn                 = 14,
  WAKEUP15_IRQn                 = 15,
  WAKEUP16_IRQn                 = 16,
  WAKEUP17_IRQn                 = 17,
  WAKEUP18_IRQn                 = 18,
  WAKEUP19_IRQn                 = 19,
  WAKEUP20_IRQn                 = 20,
  WAKEUP21_IRQn                 = 21,
  WAKEUP22_IRQn                 = 22,
  WAKEUP23_IRQn                 = 23,
  WAKEUP24_IRQn                 = 24,
  WAKEUP25_IRQn                 = 25,
  WAKEUP26_IRQn                 = 26,
  WAKEUP27_IRQn                 = 27,
  WAKEUP28_IRQn                 = 28,
  WAKEUP29_IRQn                 = 29,
  WAKEUP30_IRQn                 = 30,
  WAKEUP31_IRQn                 = 31,
  WAKEUP32_IRQn                 = 32,
  WAKEUP33_IRQn                 = 33,
  WAKEUP34_IRQn                 = 34,
  WAKEUP35_IRQn                 = 35,
  WAKEUP36_IRQn                 = 36,
  WAKEUP37_IRQn                 = 37,
  WAKEUP38_IRQn                 = 38,
  WAKEUP39_IRQn                 = 39,
  I2C_IRQn                      = 40,       /*!< I2C Interrupt                                    */
  TIMER_16_0_IRQn               = 41,       /*!< 16-bit Timer0 Interrupt                          */
  TIMER_16_1_IRQn               = 42,       /*!< 16-bit Timer1 Interrupt                          */
  TIMER_32_0_IRQn               = 43,       /*!< 32-bit Timer0 Interrupt                          */
  TIMER_32_1_IRQn               = 44,       /*!< 32-bit Timer1 Interrupt                          */
  SSP_IRQn                      = 45,       /*!< SSP Interrupt                                    */
  UART_IRQn                     = 46,       /*!< UART Interrupt                                   */
  USB_IRQn                      = 47,       /*!< USB Regular Interrupt                            */
  USB_FIQn                      = 48,       /*!< USB Fast Interrupt                               */
  ADC_IRQn                      = 49,       /*!< A/D Converter Interrupt                          */
  WDT_IRQn                      = 50,       /*!< Watchdog timer Interrupt                         */
  BOD_IRQn                      = 51,       /*!< Brown Out Detect(BOD) Interrupt                  */
  EINT3_IRQn                    = 53,       /*!< External Interrupt 3 Interrupt                   */
  EINT2_IRQn                    = 54,       /*!< External Interrupt 2 Interrupt                   */
  EINT1_IRQn                    = 55,       /*!< External Interrupt 1 Interrupt                   */
  EINT0_IRQn                    = 56,       /*!< External Interrupt 0 Interrupt                   */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */


#include "core_cm3.h"                       /* Cortex-M3 processor and core peripherals           */
#include "system_LPC13xx.h"                 /* System Header                                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/*------------- System Control (SYSCON) --------------------------------------*/
typedef struct
{
  __IO uint32_t SYSMEMREMAP;     /* Sys mem. Remap, Offset 0x0   */
  __IO uint32_t PRESETCTRL;
  __IO uint32_t SYSPLLCTRL;      /* Sys PLL control              */
  __IO uint32_t SYSPLLSTAT;
  __IO uint32_t USBPLLCTRL;      /* USB PLL control, offset 0x10 */
  __IO uint32_t USBPLLSTAT;
       uint32_t RESERVED0[2];

  __IO uint32_t SYSOSCCTRL;      /* Offset 0x20 */
  __IO uint32_t WDTOSCCTRL;
  __IO uint32_t IRCCTRL;
       uint32_t RESERVED1[1];
  __IO uint32_t SYSRESSTAT;      /* Offset 0x30 */
       uint32_t RESERVED2[3];
  __IO uint32_t SYSPLLCLKSEL;    /* Offset 0x40 */
  __IO uint32_t SYSPLLCLKUEN;
  __IO uint32_t USBPLLCLKSEL;
  __IO uint32_t USBPLLCLKUEN;
       uint32_t RESERVED3[8];

  __IO uint32_t MAINCLKSEL;      /* Offset 0x70 */
  __IO uint32_t MAINCLKUEN;
  __IO uint32_t SYSAHBCLKDIV;
       uint32_t RESERVED4[1];

  __IO uint32_t SYSAHBCLKCTRL;   /* Offset 0x80 */
       uint32_t RESERVED5[4];
  __IO uint32_t SSPCLKDIV;
  __IO uint32_t UARTCLKDIV;
       uint32_t RESERVED6[4];
  __IO uint32_t TRACECLKDIV;

  __IO uint32_t SYSTICKCLKDIV;   /* Offset 0xB0 */
       uint32_t RESERVED7[3];

  __IO uint32_t USBCLKSEL;       /* Offset 0xC0 */
  __IO uint32_t USBCLKUEN;
  __IO uint32_t USBCLKDIV;
       uint32_t RESERVED8[1];
  __IO uint32_t WDTCLKSEL;       /* Offset 0xD0 */
  __IO uint32_t WDTCLKUEN;
  __IO uint32_t WDTCLKDIV;
       uint32_t RESERVED9[1];
  __IO uint32_t CLKOUTCLKSEL;    /* Offset 0xE0 */
  __IO uint32_t CLKOUTUEN;
  __IO uint32_t CLKOUTDIV;
       uint32_t RESERVED10[5];

  __IO uint32_t PIOPORCAP0;      /* Offset 0x100 */
  __IO uint32_t PIOPORCAP1;
       uint32_t RESERVED11[18];

  __IO uint32_t BODCTRL;         /* Offset 0x150 */
       uint32_t RESERVED12[1];
  __IO uint32_t SYSTCKCAL;
       uint32_t RESERVED13[41];

  __IO uint32_t STARTAPRP0;      /* Offset 0x200 */
  __IO uint32_t STARTERP0;
  __IO uint32_t STARTRSRP0CLR;
  __IO uint32_t STARTSRP0;
  __IO uint32_t STARTAPRP1;
  __IO uint32_t STARTERP1;
  __IO uint32_t STARTRSRP1CLR;
  __IO uint32_t STARTSRP1;
       uint32_t RESERVED14[4];

  __IO uint32_t PDSLEEPCFG;      /* Offset 0x230 */
  __IO uint32_t PDAWAKECFG;
  __IO uint32_t PDRUNCFG;
       uint32_t RESERVED15[110];
  __I  uint32_t DEVICE_ID;
} LPC_SYSCON_TypeDef;


/*------------- Pin Connect Block (IOCON) --------------------------------*/
typedef struct
{
  __IO uint32_t PIO2_6;
       uint32_t RESERVED0[1];
  __IO uint32_t PIO2_0;
  __IO uint32_t RESET_PIO0_0;
  __IO uint32_t PIO0_1;
  __IO uint32_t PIO1_8;
       uint32_t RESERVED1[1];
  __IO uint32_t PIO0_2;

  __IO uint32_t PIO2_7;
  __IO uint32_t PIO2_8;
  __IO uint32_t PIO2_1;
  __IO uint32_t PIO0_3;
  __IO uint32_t PIO0_4;
  __IO uint32_t PIO0_5;
  __IO uint32_t PIO1_9;
  __IO uint32_t PIO3_4;

  __IO uint32_t PIO2_4;
  __IO uint32_t PIO2_5;
  __IO uint32_t PIO3_5;
  __IO uint32_t PIO0_6;
  __IO uint32_t PIO0_7;
  __IO uint32_t PIO2_9;
  __IO uint32_t PIO2_10;
  __IO uint32_t PIO2_2;

  __IO uint32_t PIO0_8;
  __IO uint32_t PIO0_9;
  __IO uint32_t JTAG_TCK_PIO0_10;
  __IO uint32_t PIO1_10;
  __IO uint32_t PIO2_11;
  __IO uint32_t JTAG_TDI_PIO0_11;
  __IO uint32_t JTAG_TMS_PIO1_0;
  __IO uint32_t JTAG_TDO_PIO1_1;

  __IO uint32_t JTAG_nTRST_PIO1_2;
  __IO uint32_t PIO3_0;
  __IO uint32_t PIO3_1;
  __IO uint32_t PIO2_3;
  __IO uint32_t ARM_SWDIO_PIO1_3;
  __IO uint32_t PIO1_4;
  __IO uint32_t PIO1_11;
  __IO uint32_t PIO3_2;

  __IO uint32_t PIO1_5;
  __IO uint32_t PIO1_6;
  __IO uint32_t PIO1_7;
  __IO uint32_t PIO3_3;
  __IO uint32_t SCKLOC;   /* For HB1 only, new feature */
} LPC_IOCON_TypeDef;


/*------------- Power Management Unit (PMU) --------------------------*/
typedef struct
{
  __IO uint32_t PCON;
  __IO uint32_t GPREG0;
  __IO uint32_t GPREG1;
  __IO uint32_t GPREG2;
  __IO uint32_t GPREG3;
  __IO uint32_t GPREG4;
} LPC_PMU_TypeDef;


/*------------- General Purpose Input/Output (GPIO) --------------------------*/
typedef struct
{
  union {
    __IO uint32_t MASKED_ACCESS[4096];
    struct {
         uint32_t RESERVED0[4095];
    __IO uint32_t DATA;
    };
  };
       uint32_t RESERVED1[4096];
  __IO uint32_t DIR;
  __IO uint32_t IS;
  __IO uint32_t IBE;
  __IO uint32_t IEV;
  __IO uint32_t IE;
  __IO uint32_t RIS;
  __IO uint32_t MIS;
  __IO uint32_t IC;
} LPC_GPIO_TypeDef;


/*------------- Timer (TMR) --------------------------------------------------*/
typedef struct
{
  __IO uint32_t IR;
  __IO uint32_t TCR;
  __IO uint32_t TC;
  __IO uint32_t PR;
  __IO uint32_t PC;
  __IO uint32_t MCR;
  __IO uint32_t MR0;
  __IO uint32_t MR1;
  __IO uint32_t MR2;
  __IO uint32_t MR3;
  __IO uint32_t CCR;
  __I  uint32_t CR0;
       uint32_t RESERVED1[3];
  __IO uint32_t EMR;
       uint32_t RESERVED2[12];
  __IO uint32_t CTCR;
  __IO uint32_t PWMC;
} LPC_TMR_TypeDef;

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
typedef struct
{
  union {
  __I  uint32_t  RBR;
  __O  uint32_t  THR;
  __IO uint32_t  DLL;
  };
  union {
  __IO uint32_t  DLM;
  __IO uint32_t  IER;
  };
  union {
  __I  uint32_t  IIR;
  __O  uint32_t  FCR;
  };
  __IO uint32_t  LCR;
  __IO uint32_t  MCR;
  __I  uint32_t  LSR;
  __I  uint32_t  MSR;
  __IO uint32_t  SCR;
  __IO uint32_t  ACR;
  __IO uint32_t  ICR;
  __IO uint32_t  FDR;
       uint32_t  RESERVED0;
  __IO uint32_t  TER;
       uint32_t  RESERVED1[6];
  __IO uint32_t  RS485CTRL;
  __IO uint32_t  ADRMATCH;
  __IO uint32_t  RS485DLY;
  __I  uint32_t  FIFOLVL;
} LPC_UART_TypeDef;

/*------------- Synchronous Serial Communication (SSP) -----------------------*/
typedef struct
{
  __IO uint32_t CR0;
  __IO uint32_t CR1;
  __IO uint32_t DR;
  __I  uint32_t SR;
  __IO uint32_t CPSR;
  __IO uint32_t IMSC;
  __IO uint32_t RIS;
  __IO uint32_t MIS;
  __IO uint32_t ICR;
} LPC_SSP_TypeDef;

/*------------- Inter-Integrated Circuit (I2C) -------------------------------*/
typedef struct
{
  __IO uint32_t CONSET;
  __I  uint32_t STAT;
  __IO uint32_t DAT;
  __IO uint32_t ADR0;
  __IO uint32_t SCLH;
  __IO uint32_t SCLL;
  __O  uint32_t CONCLR;
  __IO uint32_t MMCTRL;
  __IO uint32_t ADR1;
  __IO uint32_t ADR2;
  __IO uint32_t ADR3;
  __I  uint32_t DATA_BUFFER;
  __IO uint32_t MASK0;
  __IO uint32_t MASK1;
  __IO uint32_t MASK2;
  __IO uint32_t MASK3;
} LPC_I2C_TypeDef;

/*------------- Watchdog Timer (WDT) -----------------------------------------*/
typedef struct
{
  __IO uint32_t MOD;
  __IO uint32_t TC;
  __O  uint32_t FEED;
  __I  uint32_t TV;
} LPC_WDT_TypeDef;

/*------------- Analog-to-Digital Converter (ADC) ----------------------------*/
typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t GDR;
       uint32_t RESERVED0;
  __IO uint32_t INTEN;
  __I  uint32_t DR0;
  __I  uint32_t DR1;
  __I  uint32_t DR2;
  __I  uint32_t DR3;
  __I  uint32_t DR4;
  __I  uint32_t DR5;
  __I  uint32_t DR6;
  __I  uint32_t DR7;
  __I  uint32_t STAT;
} LPC_ADC_TypeDef;


/*------------- Universal Serial Bus (USB) -----------------------------------*/
typedef struct
{
  __I  uint32_t DevIntSt;            /* USB Device Interrupt Registers     */
  __IO uint32_t DevIntEn;
  __O  uint32_t DevIntClr;
  __O  uint32_t DevIntSet;

  __O  uint32_t CmdCode;             /* USB Device SIE Command Registers   */
  __I  uint32_t CmdData;

  __I  uint32_t RxData;              /* USB Device Transfer Registers      */
  __O  uint32_t TxData;
  __I  uint32_t RxPLen;
  __O  uint32_t TxPLen;
  __IO uint32_t Ctrl;
  __O  uint32_t DevFIQSel;
} LPC_USB_TypeDef;

#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Base addresses                                                             */
#define LPC_FLASH_BASE        (0x00000000UL)
#define LPC_RAM_BASE          (0x10000000UL)
#define LPC_APB0_BASE         (0x40000000UL)
#define LPC_AHB_BASE          (0x50000000UL)

/* APB0 peripherals                                                           */
#define LPC_I2C_BASE          (LPC_APB0_BASE + 0x00000)
#define LPC_WDT_BASE          (LPC_APB0_BASE + 0x04000)
#define LPC_UART_BASE         (LPC_APB0_BASE + 0x08000)
#define LPC_CT16B0_BASE       (LPC_APB0_BASE + 0x0C000)
#define LPC_CT16B1_BASE       (LPC_APB0_BASE + 0x10000)
#define LPC_CT32B0_BASE       (LPC_APB0_BASE + 0x14000)
#define LPC_CT32B1_BASE       (LPC_APB0_BASE + 0x18000)
#define LPC_ADC_BASE          (LPC_APB0_BASE + 0x1C000)
#define LPC_USB_BASE          (LPC_APB0_BASE + 0x20000)
#define LPC_PMU_BASE          (LPC_APB0_BASE + 0x38000)
#define LPC_SSP_BASE          (LPC_APB0_BASE + 0x40000)
#define LPC_IOCON_BASE        (LPC_APB0_BASE + 0x44000)
#define LPC_SYSCON_BASE       (LPC_APB0_BASE + 0x48000)

/* AHB peripherals                                                            */
#define LPC_GPIO_BASE         (LPC_AHB_BASE  + 0x00000)
#define LPC_GPIO0_BASE        (LPC_AHB_BASE  + 0x00000)
#define LPC_GPIO1_BASE        (LPC_AHB_BASE  + 0x10000)
#define LPC_GPIO2_BASE        (LPC_AHB_BASE  + 0x20000)
#define LPC_GPIO3_BASE        (LPC_AHB_BASE  + 0x30000)

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define LPC_I2C               ((LPC_I2C_TypeDef    *) LPC_I2C_BASE   )
#define LPC_WDT               ((LPC_WDT_TypeDef    *) LPC_WDT_BASE   )
#define LPC_UART              ((LPC_UART_TypeDef   *) LPC_UART_BASE  )
#define LPC_TMR16B0           ((LPC_TMR_TypeDef    *) LPC_CT16B0_BASE)
#define LPC_TMR16B1           ((LPC_TMR_TypeDef    *) LPC_CT16B1_BASE)
#define LPC_TMR32B0           ((LPC_TMR_TypeDef    *) LPC_CT32B0_BASE)
#define LPC_TMR32B1           ((LPC_TMR_TypeDef    *) LPC_CT32B1_BASE)
#define LPC_ADC               ((LPC_ADC_TypeDef    *) LPC_ADC_BASE   )
#define LPC_PMU               ((LPC_PMU_TypeDef    *) LPC_PMU_BASE   )
#define LPC_SSP               ((LPC_SSP_TypeDef    *) LPC_SSP_BASE   )
#define LPC_IOCON             ((LPC_IOCON_TypeDef  *) LPC_IOCON_BASE )
#define LPC_SYSCON            ((LPC_SYSCON_TypeDef *) LPC_SYSCON_BASE)
#define LPC_USB               ((LPC_USB_TypeDef    *) LPC_USB_BASE   )
#define LPC_GPIO0             ((LPC_GPIO_TypeDef   *) LPC_GPIO0_BASE )
#define LPC_GPIO1             ((LPC_GPIO_TypeDef   *) LPC_GPIO1_BASE )
#define LPC_GPIO2             ((LPC_GPIO_TypeDef   *) LPC_GPIO2_BASE )
#define LPC_GPIO3             ((LPC_GPIO_TypeDef   *) LPC_GPIO3_BASE )

#endif  // __LPC13xx_H__
