/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file soc.h
 *
 *  @brief This file contains the Chip Description for VC5505
 *
 *  Path: \(CSLPATH)\inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 11-Aug-2008 Created
 * 29 -Sept -2008 Added cslr_gpio.h header file ,
 *                GPIO Overlay structure and Base Address
 * 15 -Oct -2008 Added cslr_cpu.h header file ,
 *                CPU Overlay structure and Base Address
 * ============================================================================
 */

#ifndef _SOC_H
#define _SOC_H

#include <cslr.h>
#include <tistdtypes.h>
#include <cslr_sysctrl.h>
#include <cslr_cpu.h>

/*****************************************************************************\
* Include files for all the modules in the device
\*****************************************************************************/

#include "cslr_i2c.h"
#include "cslr_i2s.h"
#include "cslr_emif.h"
#include "cslr_uart.h"
#include "cslr_spi.h"
#include "cslr_mmcsd.h"
#include "cslr_lcdc.h"
#include "cslr_rtc.h"
#include "cslr_dma.h"
#include "cslr_sar.h"
#include "cslr_usb.h"
#include "cslr_gpio.h"
#include "cslr_tim.h"
#include "cslr_wdt.h"
#include "csl_general.h"



/*****************************************************************************\
* Peripheral Instance counts
\*****************************************************************************/
#define CSL_DMA_PER_CNT        4
#define CSL_EMIF_PER_CNT    1
#define CSL_I2C_PER_CNT        1
#define CSL_I2S_PER_CNT        4
#define CSL_LCDC_PER_CNT    1
#define CSL_MMCSD_PER_CNT    2
#define CSL_PLL_PER_CNT      1
#define CSL_RTC_PER_CNT        1
#define CSL_SAR_PER_CNT        1
#define CSL_SPI_PER_CNT        1
#define CSL_UART_PER_CNT    1
#define CSL_USB_PER_CNT        1
#define CSL_TIM_PER_CNT        3
#define CSL_WDT_PER_CNT        1

/*****************************************************************************\
* Peripheral Overlay Structures
\*****************************************************************************/
typedef volatile ioport CSL_UsbRegs                * CSL_UsbRegsOvly;
typedef volatile ioport CSL_I2cRegs               * CSL_I2cRegsOvly;
typedef volatile ioport CSL_I2sRegs                 * CSL_I2sRegsOvly;
typedef volatile ioport CSL_EmifRegs              * CSL_EmifRegsOvly;
typedef volatile ioport CSL_UartRegs                * CSL_UartRegsOvly;
typedef volatile ioport CSL_SpiRegs              * CSL_SpiRegsOvly;
typedef volatile ioport CSL_MmcsdRegs            * CSL_MmcsdRegsOvly;
typedef volatile ioport CSL_LcdcRegs             * CSL_LcdcRegsOvly;
typedef volatile ioport CSL_RtcRegs              * CSL_RtcRegsOvly;
typedef volatile ioport CSL_AnactrlRegs          * CSL_SarRegsOvly;
typedef volatile ioport CSL_GpioRegs                * CSL_GpioRegsOvly;
typedef volatile ioport CSL_SysRegs                * CSL_SysRegsOvly;
typedef volatile ioport CSL_DmaRegs              * CSL_DmaRegsOvly;
typedef volatile CSL_CpuRegs                     * CSL_CpuRegsOvly;
typedef volatile ioport CSL_TimRegs                 * CSL_TimRegsOvly;
typedef volatile ioport CSL_WdtRegs                 * CSL_WdtRegsOvly;

/*****************************************************************************\
* Peripheral Base Address
\*****************************************************************************/
#define CSL_USB_REGS                    ((CSL_UsbRegsOvly)  0x8000)
#define CSL_SAR_REGS                    ((CSL_SarRegsOvly)  0x7000)
#define CSL_EMIF_REGS                   ((CSL_EmifRegsOvly) 0x1000)
#define CSL_I2C_0_REGS                  ((CSL_I2cRegsOvly)  0x1A00)
#define CSL_I2S0_REGS                      ((CSL_I2sRegsOvly)     0x2800)
#define CSL_I2S1_REGS                      ((CSL_I2sRegsOvly)     0x2900)
#define CSL_I2S2_REGS                      ((CSL_I2sRegsOvly)     0x2A00)
#define CSL_I2S3_REGS                      ((CSL_I2sRegsOvly)     0x2B00)
#define CSL_UART_REGS                   ((CSL_UartRegsOvly) 0x1B00)
#define CSL_SPI_REGS                       ((CSL_SpiRegsOvly)  0x3000)
#define CSL_MMCSD0_REGS                   ((CSL_MmcsdRegsOvly)0x3A00)
#define CSL_MMCSD1_REGS                   ((CSL_MmcsdRegsOvly)0x3B00)
#define CSL_LCDC_REGS                      ((CSL_LcdcRegsOvly) 0x2E00)
#define CSL_RTC_REGS                      ((CSL_RtcRegsOvly)  0x1900)
#define CSL_DMA0_REGS                      ((CSL_DmaRegsOvly)    0x0C00)
#define CSL_DMA1_REGS                      ((CSL_DmaRegsOvly)    0x0D00)
#define CSL_DMA2_REGS                      ((CSL_DmaRegsOvly)    0x0E00)
#define CSL_DMA3_REGS                      ((CSL_DmaRegsOvly)    0x0F00)
#define CSL_GPIO_REGS                   ((CSL_GpioRegsOvly)    0x1c00)
#define CSL_SYSCTRL_REGS                ((CSL_SysRegsOvly)    0x1c00)
#define CSL_CPU_REGS                     ((CSL_CpuRegsOvly)  0x0000)
#define CSL_WDT_REGS                    ((CSL_WdtRegsOvly)  0x1880)
#define CSL_TIM_0_REGS                  ((CSL_TimRegsOvly)  0x1810)
#define CSL_TIM_1_REGS                  ((CSL_TimRegsOvly)  0x1850)
#define CSL_TIM_2_REGS                  ((CSL_TimRegsOvly)  0x1890)


/** \brief EMIF CS2 Base address                                             */
#define CSL_EMIF_CS2_DATA_BASE_ADDR                 (0x400000u)
/** \brief EMIF CS3 Base address                                             */
#define CSL_EMIF_CS3_DATA_BASE_ADDR                 (0x600000u)
/** \brief EMIF CS4 Base address                                             */
#define CSL_EMIF_CS4_DATA_BASE_ADDR                 (0x700000u)
/** \brief EMIF CS5 Base address                                             */
#define CSL_EMIF_CS5_DATA_BASE_ADDR                 (0x780000u)
/** \brief EMIF CSx address offset - Default A11 used to drive ALE           */
#define CSL_EMIF_CSx_ADDR_OFFSET                    (0x1000u)
/** \brief EMIF CS2 command offset - Default A12 used to drive CLE           */
#define CSL_EMIF_CSx_CMD_OFFSET                     (0x2000u)
/** \brief NAND CE0 address for DMA access - EMIF CS2                        */
#define CSL_NAND_CE0_ADDR                           (0x2000000u)
/** \brief NAND CE1 address for DMA access - EMIF CS3                        */
#define CSL_NAND_CE1_ADDR                           (0x3000000u)
/** \brief ASYNC CE0 address for DMA access - EMIF CS4                       */
#define CSL_ASYNC_CE0_ADDR                          (0x4000000u)
/** \brief ASYNC CE1 address for DMA access - EMIF CS5                       */
#define CSL_ASYNC_CE1_ADDR                          (0x5000000u)
/** \brief Timer intreupt aggregation flag register                             */
#define CSL_IAFR_REGS                               (0x1C14)
#endif   // _SOC_H
