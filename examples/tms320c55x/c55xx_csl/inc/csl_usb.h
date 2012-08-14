/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_usb.h
 *
 *  @brief USB MUSB functional layer API header file
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 05-Sep-2008 Created
 * 25-Dec-2008 Added Isochronous Transfer Support
 * ============================================================================
 */

/** @defgroup CSL_USB_API USB
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 *  Universal serial bus is an interface using which USB devices can be
 *  interfaced to the host device. MUSB module is designed for the
 *  host-peripheral communication through different end points.
 *  It supports the operations like configuring the USB registers,
 *  Reading data from USB FIFO and writing data to USB FIFO etc.
 *  MUSB FCSL layer supports only peripheral mode functionality.
 *  Data can be transferred between the Host and the USB device either
 *  in DMA mode or in interrupt mode.
 *
 *  @subsection References
 *   musbmhdrc_pg.pdf
 *   usb20otgss_cppi4_1p0p2.doc    Ver 1.0.2
 *
 */

#ifndef _CSL_USB_H_
#define _CSL_USB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_types.h>
#include <cslr_usb.h>
#include <soc.h>
#include <csl_general.h>


/**
@defgroup CSL_USB_SYMBOL  USB MUSB Symbols Defined
@ingroup CSL_USB_API
*/
/**
@defgroup CSL_USB_DATASTRUCT  USB MUSB Data Structures
@ingroup CSL_USB_API
*/
/**
@defgroup CSL_USB_FUNCTION  USB MUSB Functions
@ingroup CSL_USB_API
*/
/**
@defgroup CSL_USB_ENUM USB MUSB Enumerated Data Types
@ingroup CSL_USB_API
*/


/*****************************************************************************\
          USB MUSB global macro declarations                                  *
\*****************************************************************************/
/**
@addtogroup CSL_USB_SYMBOL
@{
*/

/** \brief  USB end point numbers                                            */
/** USB End point 0                                                          */
#define CSL_USB_EP0                                 (0)
/** USB End point 1                                                          */
#define CSL_USB_EP1                                 (1)
/** USB End point 2                                                          */
#define CSL_USB_EP2                                 (2)
/** USB End point 3                                                          */
#define CSL_USB_EP3                                 (3)
/** USB End point 4                                                          */
#define CSL_USB_EP4                                 (4)

/** \brief Flags for transfer functions                                      */
/** USB IN transfer                                                          */
#define CSL_USB_IN_TRANSFER                         (0x80)
/** USB OUT transfer                                                         */
#define CSL_USB_OUT_TRANSFER                        (0x00)
/** USB EP0 maximum FIFO length                                              */
#define CSL_USB_MAX_EP0_FIFO_LEN                    (0x0040)
/** USB EP0 setup packet length                                              */
#define CSL_USB_SETUP_PACKET_LEN                    (4)

/** USB EP0 data packet size                                                 */
#define CSL_USB_EP0_PACKET_SIZE                     (0x40)
/** USB EP data packet size in full speed mode                               */
#define CSL_USB_EP_PACKET_SIZE_FS                   (0x40)
/** USB EP data packet size in high speed mode                               */
#define CSL_USB_EP_PACKET_SIZE_HS                   (0x200)
/** USB EP1 data packet size in full speed mode                              */
#define CSL_USB_EP1_PACKET_SIZE_FS                  (0x40)
/** USB EP1 data packet size in high speed mode                              */
#define CSL_USB_EP1_PACKET_SIZE_HS                  (0x200)
/** USB EP2 data packet size in full speed mode                              */
#define CSL_USB_EP2_PACKET_SIZE_FS                  (0x40)
/** USB EP2 data packet size in high speed mode                              */
#define CSL_USB_EP2_PACKET_SIZE_HS                  (0x200)
/** USB EP3 data packet size in full speed mode                              */
#define CSL_USB_EP3_PACKET_SIZE_FS                  (0x40)
/** USB EP3 data packet size in high speed mode                              */
#define CSL_USB_EP3_PACKET_SIZE_HS                  (0x200)
/** USB EP4 data packet size in full speed mode                              */
#define CSL_USB_EP4_PACKET_SIZE_FS                  (0x40)
/** USB EP4 data packet size in high speed mode                              */
#define CSL_USB_EP4_PACKET_SIZE_HS                  (0x200)

/** USB Total FIFO Size **/
/** 512*8 = 4096 byte **/
#define CSL_USB_FIFO_SIZE                            (512)
/** USB IN End point FIFO start address                                      */
#define CSL_USB_IN_EP_FIFO_START                    (8)
/** USB OUT End point FIFO start address                                     */
#define CSL_USB_OUT_EP_FIFO_START                   (8)

/** \brief USB TX FIFO size register shift value                             */
#define CSL_USB_TXRXFIFOSZ_TX_SHIFT                 (0)
/** \brief USB RX FIFO size register shift value                             */
#define CSL_USB_TXRXFIFOSZ_RX_SHIFT                 (8)

/** \brief USB TX FIFO size - 8 bits                                         */
#define CSL_USB_TX_FIFO_SIZE_8                      (0)
/** \brief USB TX FIFO size - 16 bits                                        */
#define CSL_USB_TX_FIFO_SIZE_16                     (1)
/** \brief USB TX FIFO size - 32 bits                                        */
#define CSL_USB_TX_FIFO_SIZE_32                     (2)
/** \brief USB TX FIFO size - 64 bits                                        */
#define CSL_USB_TX_FIFO_SIZE_64                     (3)
/** \brief USB TX FIFO size - 128 bits                                       */
#define CSL_USB_TX_FIFO_SIZE_128                    (4)
/** \brief USB TX FIFO size - 256 bits                                       */
#define CSL_USB_TX_FIFO_SIZE_256                    (5)
/** \brief USB TX FIFO size - 512 bits                                       */
#define CSL_USB_TX_FIFO_SIZE_512                    (6)
/** \brief USB TX FIFO size - 1024 bits                                      */
#define CSL_USB_TX_FIFO_SIZE_1024                   (7)
/** \brief USB TX FIFO size - 2048 bits                                      */
#define CSL_USB_TX_FIFO_SIZE_2048                   (8)
/** \brief USB TX FIFO size - 4096 bits                                      */
#define CSL_USB_TX_FIFO_SIZE_4096                   (9)

/** \brief USB RX FIFO size - 8 bits                                         */
#define CSL_USB_RX_FIFO_SIZE_8               (0 << CSL_USB_TXRXFIFOSZ_RX_SHIFT)
/** \brief USB RX FIFO size - 16 bits                                        */
#define CSL_USB_RX_FIFO_SIZE_16              (1 << CSL_USB_TXRXFIFOSZ_RX_SHIFT)
/** \brief USB RX FIFO size - 32 bits                                        */
#define CSL_USB_RX_FIFO_SIZE_32              (2 << CSL_USB_TXRXFIFOSZ_RX_SHIFT)
/** \brief USB RX FIFO size - 64 bits                                        */
#define CSL_USB_RX_FIFO_SIZE_64              (3 << CSL_USB_TXRXFIFOSZ_RX_SHIFT)
/** \brief USB RX FIFO size - 128 bits                                       */
#define CSL_USB_RX_FIFO_SIZE_128             (4 << CSL_USB_TXRXFIFOSZ_RX_SHIFT)
/** \brief USB RX FIFO size - 256 bits                                       */
#define CSL_USB_RX_FIFO_SIZE_256             (5 << CSL_USB_TXRXFIFOSZ_RX_SHIFT)
/** \brief USB RX FIFO size - 512 bits                                       */
#define CSL_USB_RX_FIFO_SIZE_512             (6 << CSL_USB_TXRXFIFOSZ_RX_SHIFT)
/** \brief USB RX FIFO size - 1024 bits                                      */
#define CSL_USB_RX_FIFO_SIZE_1024            (7 << CSL_USB_TXRXFIFOSZ_RX_SHIFT)
/** \brief USB RX FIFO size - 2048 bits                                      */
#define CSL_USB_RX_FIFO_SIZE_2048            (8 << CSL_USB_TXRXFIFOSZ_RX_SHIFT)
/** \brief USB RX FIFO size - 4096 bits                                      */
#define CSL_USB_RX_FIFO_SIZE_4096            (9 << CSL_USB_TXRXFIFOSZ_RX_SHIFT)

/** \brief USB End point count                                               */
#define CSL_USB_ENDPOINT_COUNT                      (8)
/** \brief USB maximum output buffer length                                  */
#define CSL_USB_MAX_OUTEP0BUF_LEN                   (128)
/** \brief USB maximum number of User Interface messages                     */
#define CSL_USB_MAX_NUMBER_OF_UI_MSG                (2)

/** \brief USB IO Flags */
/** do not expect or insert a 0 byte                                         */
#define CSL_USB_IOFLAG_NONE                         (0x0000)
/** packet after a full size packet                                          */
#define CSL_USB_IOFLAG_NOSHORT                      (0x0001)
/** swap hi/lo bytes before data transmit and after data receive             */
#define CSL_USB_IOFLAG_SWAP                         (0x0002)
/** Transmit or receive data buffer passed is a linked list                  */
#define CSL_USB_IOFLAG_LNK                          (0x0004)
/** concatenate multiple linked list to fill up the data buffer with to
    max packet size before sending or receiving a data packet                */
#define CSL_USB_IOFLAG_CAT                          (0x0008)
/** Ignore argument byteCnt, transfer                                        */
#define CSL_USB_IOFLAG_EOLL                         (0x0010)
/** Nothing to Transfer                                                      */
#define CSL_USB_IOFLAG_NOT                          (0x0020)

/** \brief USB Events */
/** No interrupt received                                                    */
#define CSL_USB_EVENT_NONE                          (0x0000)
/** USB Bus Reset event                                                      */
#define CSL_USB_EVENT_RESET                         (0x0001)
/** USB Start of Frame event                                                 */
#define CSL_USB_EVENT_SOF                           (0x0002)
/** USB Bus Suspend event                                                    */
#define CSL_USB_EVENT_SUSPEND                       (0x0004)
/** USB Bus Resume event                                                     */
#define CSL_USB_EVENT_RESUME                        (0x0008)
/** USB Setup Packet Received event                                          */
#define CSL_USB_EVENT_SETUP                         (0x0010)
/** USB End of posted transaction event                                      */
#define CSL_USB_EVENT_EOT                           (0x0020)
/** USB Setup Packet Overwrite  event                                        */
#define CSL_USB_EVENT_STPOW                         (0x0040)
/** USB Pre Start of Frame event                                             */
#define CSL_USB_EVENT_PSOF                          (0x0080)
/** USB Host interrupt event                                                 */
#define CSL_USB_EVENT_HINT                          (0x0100)
/** USB Host Error event                                                     */
#define CSL_USB_EVENT_HERR                          (0x0200)
/** USB event to transmit data to isochronous endpoint                       */
#define CSL_USB_EVENT_ISO_TX                        (0x0400)
/** USB event to receive data from isochronous endpoint                      */
#define CSL_USB_EVENT_ISO_RX                        (0x0800)
/** USB event to read data from media                                        */
#define CSL_USB_EVENT_READ_MEDIA                    (0x1000)
/** USB event to write data to media                                         */
#define CSL_USB_EVENT_WRITE_MEDIA                   (0x2000)
/** USB event transmit HID report                                            */
#define CSL_USB_EVENT_HID_REPORT_TX                 (0x4000)

/** \brief Standard USB requests                                             */
/** USB get status request                                                   */
#define CSL_USB_GET_STATUS                          (0x00)
/** USB clear feature request                                                */
#define CSL_USB_CLEAR_FEATURE                       (0x01)
/** USB set feature request                                                  */
#define CSL_USB_SET_FEATURE                         (0x03)
/** USB set address request                                                  */
#define CSL_USB_SET_ADDRESS                         (0x05)
/** USB get descriptor request                                               */
#define CSL_USB_GET_DESCRIPTOR                      (0x06)
/** USB set descriptor request                                               */
#define CSL_USB_SET_DESCRIPTOR                      (0x07)
/** USB get configuration request                                            */
#define CSL_USB_GET_CONFIGURATION                   (0x08)
/** USB set configuration request                                            */
#define CSL_USB_SET_CONFIGURATION                   (0x09)
/** USB get interface request                                                */
#define CSL_USB_GET_INTERFACE                       (0x0A)
/** USB set interface request                                                */
#define CSL_USB_SET_INTERFACE                       (0x0B)
/** USB synchronization frame request                                        */
#define CSL_USB_SYNCH_FRAME                         (0x0C)
/** Get the maximum number of logical units                                  */
#define CSL_USB_GET_MAX_LUN                         (0xFE)
/** Bulk Only transport MSC reset request                                    */
#define CSL_USB_BOT_MSC_RESET                       (0xFF)

/** USB status request types                                                 */
#define CSL_USB_REQUEST_TYPE_DEVICE_STATUS            (0x80)
#define CSL_USB_REQUEST_TYPE_INTERFACE_STATUS        (0x81)
#define CSL_USB_REQUEST_TYPE_ENDPOINT_STATUS        (0x82)

/** \brief Standard USB requests for audio class                             */
/** USB request to set current                                               */
#define CSL_USB_SET_CUR                             (0x01)
/** USB request to get current                                               */
#define CSL_USB_GET_CUR                             (0x81)
/** USB request to set minimum                                               */
#define CSL_USB_SET_MIN                             (0x02)
/** USB request to get minimum                                               */
#define CSL_USB_GET_MIN                             (0x82)
/** USB request to set maximum                                               */
#define CSL_USB_SET_MAX                             (0x03)
/** USB request to get maximum                                               */
#define CSL_USB_GET_MAX                             (0x83)
/** USB request to set resolution                                            */
#define CSL_USB_SET_RES                             (0x04)
/** USB request to get resolution                                            */
#define CSL_USB_GET_RES                             (0x84)
/** USB request to set memory                                                */
#define CSL_USB_SET_MEM                             (0x05)
/** USB request to get memory                                                */
#define CSL_USB_GET_MEM                             (0x85)
/** USB request to get status                                                */
#define CSL_USB_GET_STAT                            (0xFF)

/** \brief Standard USB requests for HID class                             */
/** USB request to get Input or Feature report from HID using control
    transfer                                                                 */
#define CSL_USB_GET_REPORT                          (0x01)
/** USB request to get current Idle rate from HID                            */
#define CSL_USB_GET_IDLE                            (0x02)
/** USB request to get whether boot or report protocol currently active
    in HID                                                                   */
#define CSL_USB_GET_PROTOCOL                        (0x03)
/** USB request to send Output or Feature report to HID using control
    transfer                                                                 */
#define CSL_USB_SET_REPORT                          (0x09)
/** USB request to limit reporting frequency of interrupt IN endpoint
    when data hasn't changed since last report                               */
#define CSL_USB_SET_IDLE                            (0x0A)
/** USB request specifying whether HID should use boot or report
    protocol                                                                 */
#define CSL_USB_SET_PROTOCOL                        (0x0B)

/** \brief  USB Descriptor Type Values                                       */
/** \brief  USB Device Descriptor Type Value                                 */
#define CSL_USB_DEVICE_DESCRIPTOR_TYPE               (0x01)
/** \brief  USB Configuration Descriptor Type Value                          */
#define CSL_USB_CONFIGURATION_DESCRIPTOR_TYPE        (0x02)
/** \brief  USB String Descriptor Type Value                                 */
#define CSL_USB_STRING_DESCRIPTOR_TYPE               (0x03)
/** \brief  USB Interface Descriptor Type Value                              */
#define CSL_USB_INTERFACE_DESCRIPTOR_TYPE            (0x04)
/** \brief  USB End point Descriptor Type Value                              */
#define CSL_USB_ENDPOINT_DESCRIPTOR_TYPE             (0x05)
/** \brief  USB deice qualifier Descriptor Type Value                        */
#define CSL_USB_DEVICE_QUAL_DESCRIPTOR_TYPE          (0x06)
/** \brief  USB other speed configuration Descriptor Type Value              */
#define CSL_USB_OTHERSPEED_CFG_DESCRIPTOR_TYPE       (0x07)
/** \brief  USB HID Descriptor Type Value                                    */
#define CSL_USB_HID_DESCRIPTOR_TYPE                  (0x21)
/** \brief  USB HID Report Descriptor Type Value                             */
#define CSL_USB_HID_REPORT_DESCRIPTOR_TYPE           (0x22)
/** \brief  USB End point Stall Feature                                      */
#define CSL_USB_FEATURE_ENDPOINT_STALL               (0x0000)
/** \brief  USB Remote Wakeup Feature                                        */
#define CSL_USB_FEATURE_REMOTE_WAKEUP                (0x0001)

/** \brief USB Interrupt source register bit positions                       */
/** USB Suspend interrupt                                                    */
#define CSL_USB_GBL_INT_SUSPEND                     (1<<0)
/** USB Resume interrupt                                                     */
#define CSL_USB_GBL_INT_RESUME                      (1<<1)
/** USB Reset interrupt                                                      */
#define CSL_USB_GBL_INT_RESET                       (1<<2)
/** USB Start Of Frame  interrupt                                            */
#define CSL_USB_GBL_INT_SOF                         (1<<3)
/** USB Device connect interrupt                                             */
#define CSL_USB_GBL_INT_DEVCONN                     (1<<4)
/** USB Device disconnect interrupt                                          */
#define CSL_USB_GBL_INT_DEVDISCONN                  (1<<5)
/** USB SRP interrupt                                                        */
#define CSL_USB_GBL_INT_SRP                         (1<<6)
/** USB VBus error interrupt                                                 */
#define CSL_USB_GBL_INT_VBUSERR                     (1<<7)
/** USB Drive VBus interrupt                                                 */
#define CSL_USB_GBL_INT_DRVVBUS                     (1<<8)


/** \brief Bit positions for USB Rx interrupts                               */
/** USB End Point4 Rx interrupt                                              */
#define CSL_USB_RX_INT_EP4                          (1<<12)
/** USB End Point3 Rx interrupt                                              */
#define CSL_USB_RX_INT_EP3                          (1<<11)
/** USB End Point2 Rx interrupt                                              */
#define CSL_USB_RX_INT_EP2                          (1<<10)
/** USB End Point1 Rx interrupt                                              */
#define CSL_USB_RX_INT_EP1                          (1<<9)

/** \brief Bit positions for USB Tx (EP 4 - 1) and Tx & Rx (EP0) interrupts  */
/** USB End Point4 Tx interrupt                                              */
#define CSL_USB_TX_INT_EP4                          (1<<4)
/** USB End Point3 Tx interrupt                                              */
#define CSL_USB_TX_INT_EP3                          (1<<3)
/** USB End Point2 Tx interrupt                                              */
#define CSL_USB_TX_INT_EP2                          (1<<2)
/** USB End Point1 Tx interrupt                                              */
#define CSL_USB_TX_INT_EP1                          (1<<1)
/** USB End Point0 Tx/Rx interrupt                                           */
#define CSL_USB_TX_RX_INT_EP0                       (1<<0)

/** \brief USB delay value                                                   */
#define CSL_USB_DELAY                               (0xFF)
/** \brief USB delay value used for data transfer operations                 */
#define CSL_USB_DATA_DELAY                          (0xFFF)

/** \brief USB delay for resume protocol (In msecs)                          */
#define CSL_USB_RESUME_DELAY                        (10)

/** \brief USB peripheral reset count value                                  */
#define CSL_USB_PER_RESTCOUNT                       (0x20)

/** \brief USB Interrupt register default values                             */
/** USB Interrupt enable register default value                              */
#define CSL_USB_INTRUSB_INTRUSBE_DEFVAL             (0x7fff)
/** USB Interrupt mask set low register default value                        */
#define CSL_USB_INTMSKSETR1_DEFVAL                  (0xFFFF)
/** USB Interrupt mask set low register default value                        */
#define CSL_USB_INTMSKSETR2_DEFVAL                  (0xFFF7)
/** USB system register default value                                        */
#define CSL_USB_SYS_CTRL_REG_DEF_VAL                (0x6040)
/** USB clock stop default value                                             */
#define CSL_USB_CLOCKSTOP_REG_DEF_VAL               (0x0)

/** \brief USB Dma registers default values                                  */
/** USB Dma scheduler control register default value                          */
#define CSL_USB_DMA_SCHED_CTRL1_DEFVAL              (0x0001)
/** USB Dma scheduler table register default value                           */
#define CSL_USB_ENTRYLSW_DEFVAL                     (0x0081)
/** USB Dma receiver config low register default value                       */
#define CSL_USB_RXGCR1_DEFVAL                       (0x401A)
/** USB Dma receiver config high register default value                      */
#define CSL_USB_RXGCR2_DEFVAL                       (0x8100)
/** USB Dma Linking ram size default value                                   */
#define CSL_USB_LRAM0SIZE_DEFVAL                    (0x200)
/** USB Dma Queue manager register default value                             */
#define CSL_USB_QMEMRCTRL1_DEFVAL                   (0x200)
/** USB Dma Queue manager register default value                             */
#define CSL_USB_QMEMRCTRL2_DEFVAL                   (0x0000)
/** USB 16 bit register Mask value                                           */
#define CSL_USB_16BIT_REG_MASK                      (0xFFFF)
/** USB 16 bit register shift value                                          */
#define CSL_USB_16BIT_REG_SHIFT                     (16)

/** \brief PLL register default values(100MHz)                               */

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
/** PLL control1 register default value                                      */
#define CSL_USB_PLL_PLLCNTL1_DEFVAL                 (0x8BE8)

#else
#define CSL_USB_PLL_PLLCNTL1_DEFVAL                 (0x82FA)
#endif

/** PLL IN control register default value                                    */
#define CSL_USB_PLL_PLLINCNTL_DEFVAL                (0x8000)
/** PLL control1 register default value                                      */
#define CSL_USB_PLL_PLLCNTL2_DEFVAL                 (0x0806)
/** PLL OUT control register default value                                   */
#define CSL_USB_PLL_PLLOUTCNTL_DEFVAL               (0x0000)
/** PLL TESTLOCKMON bit timeout value                                        */
#define CSL_USB_PLL_TIMEOUT                         (0xFFFF)

/** USB Interrupt Masked Low Register LSB position                           */
#define CSL_USB_INTMASKEDRL_LSB                     (0)
/** USB Interrupt Masked Low Register MSB position                           */
#define CSL_USB_INTMASKEDRL_MSB                     (12)
/** USB Interrupt Masked High Register LSB position                          */
#define CSL_USB_INTMASKEDRH_LSB                     (0)
/** USB Interrupt Masked High Register MSB position                          */
#define CSL_USB_INTMASKEDRH_MSB                     (8)
/** USB Interrupt Clear Low Register LSB position                            */
#define CSL_USB_INTCLRRL_LSB                        (0)
/** USB Interrupt Clear Low Register MSB position                            */
#define CSL_USB_INTCLRRL_MSB                        (12)
/** USB Interrupt Clear High Register LSB position                           */
#define CSL_USB_INTCLRRH_LSB                        (0)
/** USB Interrupt Clear High Register MSB position                           */
#define CSL_USB_INTCLRRH_MSB                        (8)

/** USB Invalid maximum current value - returned in case of error            */
#define CSL_USB_INVALID_MAXPOWER                    (0xFFFF)

/** Enables USB Word Access                                                  */
#define CSL_USB_WORD_ACCESS                         (0)
/** Enables USB Low Byte Access                                              */
#define CSL_USB_HIGHBYTE_ACCESS                     (1)
/** Enables USB High Byte Access                                             */
#define CSL_USB_LOWBYTE_ACCESS                      (2)

/** Macro to find the minimum of two values                                  */
#define CSL_USB_MIN(a,b)                            (((a)<(b))?(a):(b))

/**
  *  \brief Macro to enable disable byte access
  *  byte = 0 - Enables word access
  *  byte = 1 - Enables MSB byte access
  *  byte = 2 - Enables LSB byte access
  */
#define CSL_USB_CHANGE_ACCESSTYPE(byte)                                       \
               CSL_FINS(CSL_SYSCTRL_REGS->USBSCR, SYS_USBSCR_USBBYTEMODE, byte)

/** Swap the MSB with the LSB of a 16 bit integer                            */
#define CSL_USB_BYTESWAP(word)                                                \
                          ((((word) >> 8u) & 0xFFu) | (((word) & 0xFFu) << 8u))

/** USB IO Flags                                                             */
typedef Uint16    CSL_UsbIoFlags;
/** USB Event Mask                                                           */
typedef Uint16    CSL_UsbEventMask;
/** USB Byte count remaining in a transfer                                   */
typedef Uint16    CSL_UsbByteCount;
/** USB Current Frame number data type                                       */
typedef Uint16    CSL_UsbFrameNum;

/** USB event handler                                                        */
typedef void      (*CSL_USB_EVENT_ISR)();
/** USB application call back pointer                                        */
typedef void      (*CSL_USB_APP_CALLBACK)(Uint16 flag);
/** USB application call back pointer for data transfers                     */
typedef CSL_Status (*CSL_USB_APP_TRANSFER_CALLBACK)(void *, void *);

/**
@} */

/**************************************************************************\
* USB MUSB global typedef declarations                                     *
\**************************************************************************/

/** @addtogroup CSL_USB_ENUM
 @{ */

/**
 * \brief   This enum defines Ep Input output numbers
 *
 *  These values are use by the CSL GUI or the USB API to initialize the
 *  USB Endpoint Objects
 */
typedef enum {
  /**  \brief   OUT ENDPOINTS                                                */
  /** Out Endpoint 0  - Control Out Endpoint                                 */
  CSL_USB_OUT_EP0  = 0x00,
  /** Out Endpoint 1                                                         */
  CSL_USB_OUT_EP1  = 0x01,
  /** Out Endpoint 2                                                         */
  CSL_USB_OUT_EP2  = 0x02,
  /** Out Endpoint 3                                                         */
  CSL_USB_OUT_EP3  = 0x03,
  /** Out Endpoint 4                                                         */
  CSL_USB_OUT_EP4  = 0x04,
  /** Out Endpoint 5                                                         */
  CSL_USB_OUT_EP5  = 0x05,
  /** Out Endpoint 6                                                         */
  CSL_USB_OUT_EP6  = 0x06,
  /** Out Endpoint 7                                                         */
  CSL_USB_OUT_EP7  = 0x07,
  /**  \brief   IN ENDPOINTS                                                 */
  /** In Endpoint 0  - Control In Endpoint                                   */
  CSL_USB_IN_EP0   = 0x08,
  /** In Endpoint 1                                                          */
  CSL_USB_IN_EP1   = 0x09,
  /** In Endpoint 2                                                          */
  CSL_USB_IN_EP2   = 0x0A,
  /** In Endpoint 3                                                          */
  CSL_USB_IN_EP3   = 0x0B,
  /** In Endpoint 4                                                          */
  CSL_USB_IN_EP4   = 0x0C,
  /** In Endpoint 5                                                          */
  CSL_USB_IN_EP5   = 0x0D,
  /** In Endpoint 6                                                          */
  CSL_USB_IN_EP6   = 0x0E,
  /** In Endpoint 7                                                          */
  CSL_USB_IN_EP7   = 0x0F
} CSL_UsbEpNum;

/**
 * \brief   This enum defines the various EP states
 */
typedef enum {
    /** USB End point0 Idle state                                            */
    CSL_USB_EP0_IDLE = 0,
    /** USB End point0 input state                                           */
    CSL_USB_EP0_STATUS_IN,
    /** USB End point0 transmit state                                        */
    CSL_USB_EP0_TX,
    /** USB End point0 receive state                                         */
    CSL_USB_EP0_RX
} CSL_UsbEp0State;

/**
 * \brief   This enum defines USB Device Events
 */
typedef enum {
    /**  USB device detach event                                             */
    CSL_USB_DEVICE_DETACH = 0,
    /**  USB device attach event                                             */
    CSL_USB_DEVICE_ATTACH
} CSL_UsbDeviceEvent;

/**
 * \brief   This enum defines USB Bus Speeds
 */
typedef enum {
    /** USB bus speed unknown                                                */
    CSL_USB_BS_UNKNOWN_SPEED = 0,
    /** USB full speed device                                                */
    CSL_USB_BS_FULL_SPEED    = (1 << 1),
    /** USB high speed device                                                */
    CSL_USB_BS_HIGH_SPEED    = (1 << 2)
} CSL_UsbBusSpeed;

/**
 * \brief   This enum defines the messages for MSC Task
 */
typedef enum {
    /** Sends control message to MSC task                                    */
    CSL_USB_MSG_MSC_CTL      = 0,
    /** Sends bulk in message to MSC task                                    */
    CSL_USB_MSG_MSC_BULK_IN,
    /** Sends bulk out message to MSC task                                   */
    CSL_USB_MSG_MSC_BULK_OUT,
    /** Sends Iso OUT message                                                */
    CSL_USB_MSG_ISO_OUT,
    /** Sends Iso IN message                                                 */
    CSL_USB_MSG_ISO_IN,
    /** Sends HID interrupt IN report                                        */
    CSL_USB_MSG_HID_INT_IN,
    /** Sends task exit message to MSC task                                  */
    CSL_USB_MSG_MSC_TASK_EXIT
} CSL_UsbMscMsg;

/**
 * \brief   This enum defines the messages for USBUI Task
 */
typedef enum {
    /** Sends host feature message to USBUI task                             */
    CSL_USB_MSG_USBUI_HOSTFEATURE = 0,
    /** Sends error message to USBUI task                                    */
    CSL_USB_MSG_USBUI_USBERROR,
    /** Sends task exit message to USBUI task                                */
    CSL_USB_MSG_USBUI_TASK_EXIT
} CSL_UsbUiMsg;

/**
 * \brief   This enum defines the messages for USB Main Task
 */
typedef enum {
    /** Data transfer message                                                */
    CSL_USB_MSG_DATA = 0,
    /** Interrupt message                                                    */
    CSL_USB_MSG_USB_INT,
    /** Device reset message                                                 */
    CSL_USB_MSG_RESET_DEVICE,
    /** Device connect message                                               */
    CSL_USB_MSG_CONNECT_DEVICE,
    /** Device disconnect message                                            */
    CSL_USB_MSG_DISCONNECT_DEVICE,
    /** Stall end point message                                              */
    CSL_USB_MSG_STALL_ENDPOINT,
    /** Clear end point stall message                                        */
    CSL_USB_MSG_CLEAR_ENDPOINT_STALL,
    /** Abort data transfer message                                          */
    CSL_USB_MSG_ABORT_TRANSFER,
    /** Send hand shake message                                              */
    CSL_USB_MSG_SEND_HAND_SHAKE,
    /** Main task exit message                                               */
    CSL_USB_MSG_MAIN_TASK_EXIT
} CSL_UsbMainMsg;

/**
 * \brief   This enum defines the USB Transfer Types
 */
typedef enum {
    /** Endpoint functions as control endpoint                               */
    CSL_USB_CTRL  = 0x00,
    /** Endpoint functions as bulk endpoint                                  */
    CSL_USB_BULK  = 0x01,
    /** Endpoint functions as interrupt endpoint                             */
    CSL_USB_INTR  = 0x02,
    /** Endpoint functions as isochronous endpoint                           */
    CSL_USB_ISO   = 0x03,
    /** Endpoint functions as Host Port-special feature not part of USB spec */
    CSL_USB_HPORT = 0x04
} CSL_UsbXferType;

/**
 * \brief   This enum defines the USB device number
 */
typedef enum {
    /* 1st USB module  - Currently supported mode                            */
    CSL_USB0 = 0x00,
    /* 2nd USB module  - if the DSP support 2 USB module                     */
    CSL_USB1 = 0x01,
    /* 3rd USB module  - if the DSP support 3 USB module                     */
    CSL_USB2 = 0x02
} CSL_UsbDevNum;

/**
 * \brief   This enum defines the USB EP0 data dir type
 */
typedef enum {
    /* Endpoint0 data dir: Host -> Target                                    */
    CSL_USB_EP0_DATA_OUT = 0x00,
    /* Endpoint0 data dir: Target -> host                                    */
    CSL_USB_EP0_DATA_IN  = 0x01
} CSL_UsbEp0DataDir;

/**
 * \brief   This enum defines status return values of USB
 */
typedef enum {
    /* Status of the operation is FALSE                                      */
    CSL_USB_FALSE = 0,
    /* Status of the operation is TRUE                                       */
    CSL_USB_TRUE
} CSL_UsbBoolean;

/**
 * \brief   This enum is used to select the operating mode of the USB
 *
 * This opMode decides how the data is transferred(Using CPU or DMA) to and
 * from the USB FIFO. Even when operating in DMA mode, USB module uses CPU
 * to access the FIFO data which is of length less than 512 bytes.
 *
 */
typedef enum {
    /* FIFO data is directly accessed by the CPU                             */
    CSL_USB_OPMODE_POLLED = 0,
    /* RX and TX FIFO data is accessed by the CPPI DMA Engine                          */
    CSL_USB_OPMODE_DMA,
    /* TX FIFO data is accessed by the CPPI DMA Engine                          */
    CSL_USB_OPMODE_DMA_TXONLY
} CSL_UsbOpMode;


/**
@} */

/**************************************************************************\
* USB MUSB global data structure declarations                              *
\**************************************************************************/

/**
\addtogroup CSL_USB_DATASTRUCT
@{ */

/**
 *  \brief USB end point transfer data structure
 *
 *  This structure is used by endpoints to hold data that needs to be
 *  transferred over USB bus
 */
typedef struct CSL_UsbTransfer {
    /** \brief  Flags associated                                             */
    DWORD             dwFlags;
    /** \brief  Endpoint                                                     */
    DWORD             dwEndpoint;
    /** \brief  Pointer to the data buffer                                   */
    PVOID             pvBuffer;
    /** \brief  No. of bytes in the buffer                                   */
    DWORD             cbBuffer;
    /** \brief  No. of bytes transferred                                     */
    DWORD             cbTransferred;
    /** \brief  No. of bytes transmitted or received during the previous data
                transaction                                                  */
    DWORD             prevTxferLen;
    /** \brief  TRUE, if Tx Completed                                        */
    Bool              fComplete;
    /** \brief  Flags to indicate data transfer type                         */
    CSL_UsbIoFlags    ioFlags;
} CSL_UsbTransfer;

/** USB End point transfer data structure pointer                            */
typedef CSL_UsbTransfer    *pUsbTransfer;

/**
 *  \brief USB end point status structure
 *
 *  This structure is used to hold the status of an Endpoint
 */
typedef struct CSL_UsbEpStatus {
    /** \brief  Endpoint                                                     */
    DWORD                dwEndpoint;
    /** \brief  USB xfer type supported by the EP                            */
    CSL_UsbXferType      xferType;
    /** \brief  Packet Size assigned                                         */
    DWORD                dwPacketSizeAssigned;
    /** \brief  TRUE, if initialized                                         */
    Bool                 fInitialized;
    /** \brief  FIFO address of the EP                                       */
    DWORD*                  pFifoAddr;
    /** \brief  Pointer to the Tx Buf                                        */
    CSL_UsbTransfer      *pTransfer;
    /** \brief  OR'ed combination of USB Events                              */
    WORD                 wUSBEvents;
    /** \brief  Variable to store the most recent data size read from FIFO   */
    WORD                 dataReadFromFifo;
    /** \brief  Call back for the associated event handler                   */
    CSL_USB_EVENT_ISR    hEventHandler;
    /** \brief  TRUE, if stalled                                             */
    Bool                 fStalled;
    /** \brief  TRUE, if selected                                            */
    Bool                 fSelected;
} CSL_UsbEpStatus;

/** USB End point status structure pointer                                   */
typedef CSL_UsbEpStatus  *pUsbEpStatus;

/**
 *  \brief Data structure to hold USB setup packet.
 */
typedef struct CSL_UsbSetupStruct {
    /** \brief  new = 1, structure holds new setup packet                    */
    int       newPacket;
    /** \brief  Byte 0 of setup packet                                       */
    Uint16    bmRequestType;
    /** \brief  Byte 1 of setup packet                                       */
    Uint16    bRequest;
    /** \brief  Byte 2 and 3 of setup packet                                 */
    Uint16    wValue;
    /** \brief  Byte 4 and 5 setup packet                                    */
    Uint16    wIndex;
    /** \brief  Byte 6 and 7 of setup packet                                 */
    Uint16    wLength;
} CSL_UsbSetupStruct;

/**
 *  \brief USB data structure to send and receive data
 *
 *  This structure is used by the USB to send and received data in
 *  linked list form. USB Descriptors should be created as a
 *  linked list of CSL_UsbDataStruct type
 */
typedef struct CSL_UsbDataStruct {
    /** \brief  Total number of bytes in the buf                             */
    Uint16                      bytes;
    /** \brief  pointer to the start of buffer                               */
    Uint16                      *pBuffer;
    /** \brief  pointer to the next structure                                */
    struct CSL_UsbDataStruct    *pNextBuffer;
} CSL_UsbDataStruct;

/**
 *  \brief Data Structure for USB Endpoint Object
 *
 *  Every active USB endpoint is associated with an endpoint object
 *  which keeps track of the endpoint related initialization and
 *  runtime information
 */
typedef struct CSL_UsbEpObj {
    /** \brief  USB endpoint number                                          */
    CSL_UsbEpNum         epNum;
    /** \brief  USB xfer type supported by the EP                            */
    CSL_UsbXferType      xferType;
    /** \brief  Max pkt size supported by the EP                             */
    Uint16               maxPktSiz;
    /** \brief  ORed value of USB_EVENTS, the USB                            */
    Uint16               eventMask;
    /**
    * \brief  Pointer to USB event ISR
    * Event dispatcher will call the event
    * ISR if event matches with EventMask                                    */
    CSL_USB_EVENT_ISR    fxn;
    /** \brief  ORed combination of USB_DATA_INOUT_FLAGS                     */
    Uint16               dataFlags;
    /** \brief  Reserved for future use                                      */
    Uint16               status;
    /** \brief  Endpoint desc reg block start addr                           */
    Uint16               epDescRegSAddr;
    /** \brief  DMA reg block start addr                                     */
    Uint16               dmaRegSAddr;
    /** \brief  Total number of bytes to xfer                                */
    Uint16               totByteCnt;
    /** \brief  # of bytes in the active node of the linked list             */
    Uint16               byteInThisSeg;
    /** \brief  Pointer to store the number of bytes moved in (out)
    *  stored in the first of the buffer                                     */
    Uint16               *xferBytCnt;
    /** \brief  Active data buffer pointer                                   */
    Uint16               *pBuffer;
    /** \brief  Pointer to the next node of the linked list                  */
    CSL_UsbDataStruct    *pNextBuffer;
    /** \brief  Flag to indicate the event caused USB interrupt              */
    Uint16               eventFlag;
} CSL_UsbEpObj;

/** USB End point object structure pointer                                   */
typedef CSL_UsbEpObj    *pUsbEpHandle;

/**
 *  \brief USB context information structure
 *
 *  This structure maintains the context of MUSB controller
 */
typedef struct CSL_UsbContext {
    /** \brief  Operating mode of the USB module to access the FIFO data     */
    CSL_UsbOpMode           opMode;
    /** \brief  Maximum current drawn by the USB device                      */
    WORD                    maxCurrent;
    /** \brief  TRUE, if Initialized                                         */
    Bool                    fInitialized;
    /** \brief  TRUE, if there is a setup packet                             */
    Bool                    fSetupPktCmd;
    /** \brief  TURE, when MUSB receives EP0 data                            */
    Bool                    fOutPhaseCmd;
    /** \brief  TRUE, when DATA is ready in BULK IN EP FIFO                  */
    Bool                    fWaitingOnFlagA;
    /** \brief  TRUE, when DATA is ready in BULK OUT EP FIFO                 */
    Bool                    fWaitingOnFlagB;
    /** \brief  TRUE, if waiting for data in EP0 buffer                      */
    Bool                    fWaitingOnEP0BUFAvail;
    /** \brief  TRUE, if waiting for data in EP1 buffer                      */
    Bool                    fWaitingOnEP1InBUFAvail;
    /** \brief  Pointer to the Device Descriptor structure                   */
    WORD                    *rgbDescriptors;
    /** \brief  No. of bytes in Device Descriptor structure                  */
    WORD                    cbDescriptors;
    /** \brief  Flag A, can be assigned to a NON-Control EP                  */
    DWORD                   dwFlagAEndpointAssigned;
    /** \brief  FLAG B end point assigned                                    */
    DWORD                   dwFlagBEndpointAssigned;
    /** \brief  FIFO Base address                                            */
    DWORD                   *pFIFOBaseAddress;
    /** \brief  Interrupt Register Addr                                      */
    DWORD                   *pInterruptAddr;
    /** \brief  TRUE, for handshake pkt                                      */
    Bool                    fSendHandshake;
    /** \brief  TRUE, if MUSB is ready                                       */
    Bool                    fMUSBIsReady;
    /** \brief  No. of bytes in EP0 Buf                                      */
    WORD                    cbOutEP0Buf;
    /** \brief  Data Received in EP0 FIFO                                    */
    WORD                    cbOutEP0DataReceived;
    /** \brief  Buffer to hold data for EP0 Out transaction                  */
    DWORD                   dwOutEP0Buf[CSL_USB_MAX_OUTEP0BUF_LEN / 4];
    //WORD                    dwOutEP0Buf[CSL_USB_MAX_OUTEP0BUF_LEN / 2];
    /** \brief  TRUE, if data available in EP0 Buffer                        */
    Bool                    fEP0BUFAvailable;
    /** \brief  TRUE, if data available in EP1 Buffer                        */
    Bool                    fEP1InBUFAvailable;
    /** \brief  Current pending high Bitmap                                      */
    WORD                    dwQueuePend1;
    /** \brief  TRUE, if data available in EP3 Buffer                        */
    Bool                    fEP3InBUFAvailable;
    /** \brief  Current Intr Low Bitmap                                      */
    WORD                    dwIntSourceL;
    /** \brief  Current Intr High Bitmap                                     */
    WORD                    dwIntSourceH;
    /** \brief  USB remote wake up status                                    */
    CSL_UsbBoolean          remoteWkpStat;
    /** \brief  Pointer to the array of EP Status Structures                 */
    CSL_UsbEpStatus         *pEpStatus;
    /** \brief  BUS SPEED negotiated                                         */
    CSL_UsbBusSpeed         busSpeed;
    /** \brief  EP0 State                                                    */
    CSL_UsbEp0State         ep0State;
    /** \brief  Cable State                                                  */
    CSL_UsbDeviceEvent      cableState;
    /** \brief  Structure to hold the setup packet                           */
    CSL_UsbSetupStruct      usbSetup;
    /** \brief  Application callback for suspend protocol                    */
    CSL_USB_APP_CALLBACK    suspendCallBackFxn;
    /** \brief  Application callback for wakeup protocol                     */
    CSL_USB_APP_CALLBACK    wakeupCallBackFxn;
    /** \brief  USB start transfer application call back function            */
    CSL_USB_APP_TRANSFER_CALLBACK    startTransferCallback;
    /** \brief  USB complete transfer application call back function         */
    CSL_USB_APP_TRANSFER_CALLBACK    completeTransferCallback;
    WORD                    dwFIFOStartAddr;
    /** \brief  device descriptor pointer                                     */
    Uint16                  *deviceDescPtr;
    /** \brief  device qualification descriptor pointer                                     */
    Uint16                  *deviceQualDescPtr;
    /** \brief  configuration descriptor pointer                             */
    Uint16                  *cfgDescPtr;
    /** \brief  configuration descriptor for FullSpeed  pointer                             */
    Uint16                  *cfgDescFSPtr;
    /** \brief  User specific string descriptor pointer                         */
    Uint16                  *strDescPtr[4];
    /** \brief USB device address                                             */
    Uint16                    devAddr;
    /** \brief  Control In Endpoint Object                                   */
    CSL_UsbEpObj             ctrlInEpObj;
    /** \brief  Object of Type Device Number                                 */
    CSL_UsbDevNum            devNum;
    /** \brief  Control Out Endpoint Object                                  */
    CSL_UsbEpObj             ctrlOutEpObj;
    /** \brief  Flag Used internally by APIs                                 */
      Uint16                   suspendFlag;
      /** \brief  NULL terminated array of Endpoint Objects                    */
    pUsbEpHandle             hEpObjArray[CSL_USB_ENDPOINT_COUNT];
    /** \brief  Current Device Configuration Status                          */
    Uint16                   curConfigStat;
    /** \brief  USB speed configuration
                usbSpeedCfg - 1; High speed
                usbSpeedCfg - 0; Full speed                                  */
    Bool                     usbSpeedCfg;
} CSL_UsbContext;

/** USB Context structure pointer                                            */
typedef CSL_UsbContext  *pUsbContext;

/**
 *  \brief USB message object structure
 *
 *  This structure is used to post messages to the USB Tasks running
 */
typedef struct CSL_UsbMsgObj {
    /** \brief  Message posted                                               */
    WORD    wMsg;
    /** \brief  End point number                                             */
    WORD    wEndpoint;
} CSL_UsbMsgObj;

/**
 *  \brief USB Config structure
 *
 *  This structure is filled by the application and passed to the
 *  USB init function
 */
typedef struct CSL_UsbConfig {
    /** \brief  USB device number, Only USB0 is active currently.            */
    CSL_UsbDevNum           devNum;
    /** \brief  Operating mode of the USB module to access the FIFO data     */
    CSL_UsbOpMode           opMode;
    /** \brief  Maximum current drawn by the USB device                      */
    Uint16                  maxCurrent;
    /** \brief  Application callback for suspend protocol                    */
    CSL_USB_APP_CALLBACK    appSuspendCallBack;
    /** \brief  Application callback for wakeup protocol                     */
    CSL_USB_APP_CALLBACK    appWakeupCallBack;
    /** \brief  USB start transfer application call back function            */
    CSL_USB_APP_TRANSFER_CALLBACK    startTransferCallback;
    /** \brief  USB complete transfer application call back function         */
    CSL_USB_APP_TRANSFER_CALLBACK    completeTransferCallback;
} CSL_UsbConfig;


/**
 *  \brief USB DMA host packet descriptor structure
 */
typedef struct CSL_UsbHostPktDescr {
    /** \brief  DMA packet information word 0                                */
    volatile Uint32    PacketInfoWord0;
    /** \brief  DMA packet information word 1                                */
    volatile Uint32    PacketInfoWord1;
    /** \brief  DMA packet information word 2                                */
    volatile Uint32    PacketInfoWord2;
    /** \brief  DMA buffer0 information word 0                               */
    volatile Uint32    Buffer0InfoWord0;
    /** \brief  DMA buffer0 information word 1                               */
    volatile Uint32    Buffer0InfoWord1;
    /** \brief  Linking word to the next packet                              */
    volatile Uint32    LinkingWord;
    /** \brief  DMA Original buffer0 information word 0                      */
    volatile Uint32    OriginalBuffer0InfoWord0;
    /** \brief  DMA Original buffer0 information word 1                      */
    volatile Uint32    OriginalBuffer0InfoWord1;
} CSL_UsbHostPktDescr;

/**
 *  \brief USB DMA host buffer descriptor structure
 */
typedef struct CSL_UsbHostBufDescr {
    /** \brief  Reserved for the future use                                  */
    volatile Uint32    Reserved0;
    /** \brief  Reserved for the future use                                  */
    volatile Uint32    Reserved1;
    /** \brief  DMA packet information word 2                                */
    volatile Uint32    PacketInfoWord2;
    /** \brief  DMA buffer0 information word 0                               */
    volatile Uint32    Buffer0InfoWord0;
    /** \brief  DMA buffer0 information word 1                               */
    volatile Uint32    Buffer0InfoWord1;
    /** \brief  Linking word to the next packet                              */
    volatile Uint32    LinkingWord;
    /** \brief  DMA Original buffer0 information word 0                      */
    volatile Uint32    OriginalBuffer0InfoWord0;
    /** \brief  DMA Original buffer0 information word 1                      */
    volatile Uint32    OriginalBuffer0InfoWord1;
} CSL_UsbHostBufDescr;

/**
@} */

/**************************************************************************\
* USB MUSB function declarations                                            *
\**************************************************************************/

/** @addtogroup CSL_USB_FUNCTION
 @{ */

/** ============================================================================
 *   @n@b USB_init
 *
 *   @b Description
 *   @n This function initializes USB module
 *
 *   @b Arguments
 *   @verbatim
            usbConfig        USB Config structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes the USB module
 *
 *   @b Modifies
 *   @n USB context structure
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_init(CSL_UsbConfig    *usbConfig);

/** ============================================================================
 *   @n@b USB_resetDev
 *
 *   @b Description
 *   @n This function resets the USB device
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Resets the USB device
 *
 *   @b Modifies
 *   @n USB registers
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_resetDev(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_resetDev(CSL_UsbDevNum    devNum);

/** ============================================================================
 *   @n@b USB_initEndptObj
 *
 *   @b Description
 *   @n This function initializes an endpoint object.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            hEp           Handle to an endpoint object to be initialized.
            epNum         End point number
            xferType      Type of data transfer to be supported by the endpoint
            maxPktSize    Max data packet size supported by the endpoint.
            evMask        ORed combination of USB Interrupt Events to
                             be broadcasted to the associated event handler
            fxn           Associated event handler routine.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init and USB_epNumToHandle functions should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Initializes an end point object
 *
 *   @b Modifies
 *   @n End point object structure
 *
 *   @b Example
 *   @verbatim
            void      (*evtHandler)();

            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            pUsbEpHandle       hEp;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            .....
            hEp = USB_epNumToHandle(CSL_USB0, CSL_USB_IN_TRANSFER);
            .....
            .....
            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_initEndptObj(CSL_UsbDevNum        devNum,
                            pUsbEpHandle         hEp,
                            CSL_UsbEpNum         epNum,
                            CSL_UsbXferType      xferType,
                            Uint16               maxPktSize,
                            Uint16               evMask,
                            CSL_USB_EVENT_ISR    fxn);

/** ============================================================================
 *   @n@b USB_getEvents
 *
 *   @b Description
 *   @n This API reads and clears all the pending USB_EVENTS associated with a
 *      particular USB endpoint.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbEventMask
 *   @n                     ORed combination of all the pending USB_EVENTS
 *                            associated with a particular endpoint.
 *
 *   <b> Pre Condition </b>
 *   @n  End point object should be initialized
 *
 *   <b> Post Condition </b>
 *   @n  1. Clears and returns the end point events
 *   @n  2. Function call status is returned in status variable
 *   @li    CSL_SOK            - Returned for success
 *   @li    CSL_ESYS_BADHANDLE - Invalid end point handle
 *
 *   @b Modifies
 *   @n status variable
 *
 *   @b Example
 *   @verbatim
             void  (*evtHandler)();

             pUsbEpHandle     hEp;
            CSL_Status       status;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);
            USB_getEvents(hEp,
                          &status);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbEventMask USB_getEvents(pUsbEpHandle    hEp,
                               CSL_Status      *status);

/** ============================================================================
 *   @n@b USB_peekEvents
 *
 *   @b Description
 *   @n This function read all the pending USB_EVENTS associated with
 *   a particular  USB endpoint
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>    CSL_UsbEventMask
 *   @n                       ORed combination of all the pending USB_EVENTS
 *                              associated with a particular endpoint.
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  End point object should be initialized
 *
 *   <b> Post Condition </b>
 *   @n  1. Returns the end point events
 *   @n  2. Function call status is returned in status variable
 *   @li    CSL_SOK            - Returned for success
 *   @li    CSL_ESYS_BADHANDLE - Invalid end point handle
 *
 *   @b Modifies
 *   @n status variable
 *
 *   @b Example
 *   @verbatim
             void  (*evtHandler)();

             CSL_UsbEpObj    epObj;
            CSL_Status      status;


            status = USB_initEndptObj(CSL_USB0,
                                      &epObj,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);
            USB_peekEvents(&epObj,
                           &status);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbEventMask USB_peekEvents(pUsbEpHandle    hEp,
                                CSL_Status      *status);

/** ============================================================================
 *   @n@b USB_getSetupPacket
 *
 *   @b Description
 *   @n This function read the setup packet from the setup data buffer.
 *      This function can be used to read the setup packet from the FIFO
 *      or we can read the setup packet from the USB context structure.
 *      We can choose one of two options using readFromFifo parameter.
 *      If readFromFifo is TRUE setup packet is read from the FIFO.
 *      If readFromFifo is FALSE setup packet is read from the context structure.
 *      Setup packet should be read from FIFO when this function is called from
 *      interrupt handler(In this case read packet is stored in context structure)
 *      and setup packet should be read from USB context structure when this
 *      function is called from MSC layer.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number
            usbSetup      Pointer to setup structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB Init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  USB setup structure is populated
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_UsbSetupStruct    usbSetup;
            CSL_Status            status;
            CSL_UsbConfig         usbConfig

            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_getSetupPacket(CSL_USB0, &usbSetup , TRUE);

     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_getSetupPacket(CSL_UsbDevNum         devNum,
                              CSL_UsbSetupStruct    *usbSetup,
                              Bool                  readFromFifo);

/** ============================================================================
 *   @n@b USB_postTransaction
 *
 *   @b Description
 *   @n This function transmits and receives USB data through an endpoint.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            byteCnt     Total number of bytes in the buffer pointed by *data
            Data        Pointer to a data buffer
            flags       ORed combination of USB Data Transfer Flags
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init end point should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Starts the data transfer
 *
 *   @b Modifies
 *   @n USB FIFO registers/data buffer
 *
 *   @b Example
 *   @verbatim
             void      (*evtHandler)();
             Uint16    dadaBuf[256];

             pUsbEpHandle    hEp;
            CSL_Status      status;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);

            status = USB_postTransaction(hEp, 256, dataBuf, CSL_USB_IOFLAG_NONE);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_postTransaction(pUsbEpHandle      hEp,
                               Uint16            byteCnt,
                               void              *data,
                               CSL_UsbIoFlags    flags);

/** ============================================================================
 *   @n@b USB_isTransactionDone
 *
 *   @b Description
 *   @n This function returns the status of the previously posted
 *      data transfer request.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbBoolean
 *   @li                    CSL_USB_TRUE  - If the previously posted transfer
 *                                          is completed.
 *   @li                    CSL_USB_FALSE - If the previously posted transfer
 *                                          is not completed.
 *
 *   <b> Pre Condition </b>
 *   @n  Post transaction API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  1. Returns the previous transaction status
 *   @n  2. Function call status is returned in status variable
 *   @li    CSL_SOK            - Returned for success
 *   @li    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   @b Modifies
 *   @n status variable
 *
 *   @b Example
 *   @verbatim
             void      (*evtHandler)();
             Uint16    dadaBuf[256];

             pUsbEpHandle      hEp;
            CSL_Status        status;
            CSL_UsbBoolean    tscStatus;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);

            status = USB_postTransaction(hEp, 256, dataBuf, CSL_USB_IOFLAG_NONE);
            ...
            ...
            tscStatus = USB_isTransactionDone(hEp ,&status);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbBoolean USB_isTransactionDone(pUsbEpHandle    hEp,
                                     CSL_Status      *status);

/** ============================================================================
 *   @n@b USB_bytesRemaining
 *
 *   @b Description
 *   @n This API returns number of bytes waiting to be transferred from the
 *      previously posted data transfer request.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbByteCount
 *   @n                     Number of bytes waiting to be transferred
 *
 *   <b> Pre Condition </b>
 *   @n  Post transaction API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  1. Returns the number of bytes waiting
 *   @n  2. Function call status is returned in status variable
 *   @li    CSL_SOK            - Returned for success
 *   @li    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   @b Modifies
 *   @n status variable
 *
 *   @b Example
 *   @verbatim
             void      (*evtHandler)();
             Uint16    dadaBuf[256];

             pUsbEpHandle   hEp;
            CSL_Status     status;
            Uint16         byteCount;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);

            status = USB_postTransaction(hEp, 256, dataBuf, CSL_USB_IOFLAG_NONE);
            ...
            ...
            byteCount = USB_bytesRemaining(hEp ,&status);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbByteCount USB_bytesRemaining(pUsbEpHandle    hEp,
                                    CSL_Status      *status);

/** ============================================================================
 *   @n@b USB_stallEndpt
 *
 *   @b Description
 *   @n This function stalls an end point
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  End point init function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Stalls an end point associated with the passed handle
 *
 *   @b Modifies
 *   @n USB control registers
 *
 *   @b Example
 *   @verbatim
             void      (*evtHandler)();

             pUsbEpHandle   hEp;
            CSL_Status     status;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);            ...
            ...
            status = USB_stallEndpt(hEp);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_stallEndpt(pUsbEpHandle  hEp);

/** ============================================================================
 *   @n@b USB_clearEndptStall
 *
 *   @b Description
 *   @n This function clears an end point stall
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_initEndptObj and USB_stallEndpt should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Clears an end point stall
 *
 *   @b Modifies
 *   @n USB control registers
 *
 *   @b Example
 *   @verbatim
              void      (*evtHandler)();

             pUsbEpHandle   hEp;
            CSL_Status     status;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);

            ....
            ....
            status = USB_stallEndpt(hEp);
            ....
            ....
            status = USB_clearEndptStall(hEp);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_clearEndptStall(pUsbEpHandle  hEp);

/** ============================================================================
 *   @n@b USB_getEndptStall
 *
 *   @b Description
 *   @n This function checks if an endpoint is stalled.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbBoolean
 *   @li                    CSL_USB_TRUE  - If the end point is stalled.
 *   @li                    CSL_USB_FALSE - If the end point is not stalled
 *
 *   <b> Pre Condition </b>
 *   @n  Init End point API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  1. Returns the end point stall status
 *   @n  2. Function call status is returned in status variable
 *   @li    CSL_SOK            - Returned for success
 *   @li    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   @b Modifies
 *   @n status variable
 *
 *   @b Example
 *   @verbatim
             void      (*evtHandler)();

             pUsbEpHandle      hEp;
            CSL_Status        status;
            CSL_UsbBoolean    epStall;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);
            ...
            ...
            epStall = USB_getEndptStall(hEp, &status);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbBoolean USB_getEndptStall(pUsbEpHandle    hEp,
                                 CSL_Status      *status);

/** ============================================================================
 *   @n@b USB_epNumToHandle
 *
 *   @b Description
 *   @n This function returns handle to an end point
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            endPt         16-bit endpoint number as specified
                           in the USB specification.
                           0x00 -> Endpt 0 Out.
                           0x01 -> Endpt 1 Out.
                            0x80 -> Endpt 0 In.
                            0x81 -> Endpt 1 In.
     @endverbatim
 *
 *   <b> Return Value </b>  pUsbEpHandle
 *   @n                     Handle to the endpoint object if a valid
 *                          endpoint object exists, else NULL object.
 *
 *   <b> Pre Condition </b>
 *   @n  USB_init function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  returns End point handle
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig    usbConfig;
            CSL_Status       status;
            pUsbEpHandle     hEp;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            .....
            hEp = USB_epNumToHandle(CSL_USB0, CSL_USB_IN_TRANSFER);
     @endverbatim
 *  ============================================================================
 */
pUsbEpHandle USB_epNumToHandle(CSL_UsbDevNum    devNum,
                               Uint16           endpt);

/** ============================================================================
 *   @n@b USB_abortTransaction
 *
 *   @b Description
 *   @n This function terminates the data transfer in progress and
 *      free up the endpoint to post a new data transfer request.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_postTransaction function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Terminates data transfer in progress
 *
 *   @b Modifies
 *   @n USB registers
 *
 *   @b Example
 *   @verbatim
             Uint16    dadaBuf[256];

             pUsbEpHandle    hEp;
            CSL_Status      status;

            status = USB_postTransaction(hEp, 256, dataBuf, CSL_USB_IOFLAG_NONE);
            .....
            .....
            status = USB_abortTransaction(hEp);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_abortTransaction(pUsbEpHandle    hEp);

/** ============================================================================
 *   @n@b USB_abortAllTransaction
 *
 *   @b Description
 *   @n This function terminates all the data transfer in progress and
 *      free up the endpoints to post new data transfer requests.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_postTransaction function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Aborts all transactions in progress
 *
 *   @b Modifies
 *   @n USB registers
 *
 *   @b Example
 *   @verbatim
             Uint16    dadaBuf[256];

             pUsbEpHandle    hEp;
            CSL_Status      status;

            status = USB_postTransaction(hEp, 256, dataBuf, CSL_USB_IOFLAG_NONE);
            .....
            .....
            status = USB_abortAllTransaction(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_abortAllTransaction(CSL_UsbDevNum    devNum);

/** ============================================================================
 *   @n@b USB_connectDev
 *
 *   @b Description
 *   @n This function connects the USB module to upstream port
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init function should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Connects the USB module
 *
 *   @b Modifies
 *   @n USB FADDR_POWER register
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_connectDev(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_connectDev(CSL_UsbDevNum    devNum);

/** ============================================================================
 *   @n@b USB_disconnectDev
 *
 *   @b Description
 *   @n This function disconnects the USB module from the upstream port
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_connectDev should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Disconnects the USB module.
 *
 *   @b Modifies
 *   @n USB FADDR_POWER register
 *
 *   @b Example
 *   @verbatim
             CSL_Status    status;

            status = USB_connectDev(CSL_USB0);
            ....
            ....
            status = USB_disconnectDev(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_disconnectDev(CSL_UsbDevNum    devNum);

/** ============================================================================
 *   @n@b USB_setRemoteWakeup
 *
 *   @b Description
 *   @n This function sets or clears Remote Wakeup Feature.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            rmtWkpStat    Remote wake up status flag
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init function should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Sets the remote wake up status
 *
 *   @b Modifies
 *   @n Remote wakeup status variable in the USB global context structure
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_setRemoteWakeup(CSL_USB0, CSL_USB_TRUE);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_setRemoteWakeup(CSL_UsbDevNum     devNum,
                               CSL_UsbBoolean    rmtWkpStat);

/** ============================================================================
 *   @n@b USB_getRemoteWakeupStat
 *
 *   @b Description
 *   @n This function returns the remote wake up feature status.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbBoolean
 *   @li                    CSL_USB_TRUE  - Remote wakeup feature is enabled.
 *   @li                    CSL_USB_FALSE - Remote wakeup feature is disabled.
 *
 *   <b> Pre Condition </b>
 *   @n  USB_setRemoteWakeup function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns remote wakeup status
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_Status        status;
            CSL_UsbBoolean    rmtWkpStat

            status = USB_setRemoteWakeup(CSL_USB0, CSL_USB_TRUE);

            if(status == CSL_SOK)
            {
                rmtWkpStat = USB_getRemoteWakeupStat(CSL_USB0);
            }
     @endverbatim
 *  ============================================================================
 */
CSL_UsbBoolean USB_getRemoteWakeupStat(CSL_UsbDevNum    devNum);

/** ============================================================================
 *   @n@b USB_issueRemoteWakeup
 *
 *   @b Description
 *   @n This function issues a remote wakeup signal to the host.
 *      NOTE: For remote wakeup protocol application program needs to call
 *      this function two times. In the first call resumeBitVal should be
 *      TRUE and in the second call resumeBitVal should be FALSE.
 *      Application should give a delay of 2 - 15 mSecs between the two calls.
 *      This is to avoid delays internal to the CSL module
 *
 *   @b Arguments
 *   @verbatim
            devNum           USB device number, Only USB0 is active currently.
            resumeBitVal     Value of the resume bit - TRUE or FALSE
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_setRemoteWakeup function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Issues a remote wake up signal.
 *
 *   @b Modifies
 *   @n USB FADDR POWER register
 *
 *   @b Example
 *   @verbatim
            CSL_Status    status;

            status = USB_setRemoteWakeup(CSL_USB0, CSL_USB_TRUE);

            if(status == CSL_SOK)
            {
                status = USB_issueRemoteWakeup(CSL_USB0);
            }
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_issueRemoteWakeup(CSL_UsbDevNum    devNum,
                                 Bool             resumeBitVal);

/** ============================================================================
 *   @n@b USB_setDevAddr
 *
 *   @b Description
 *   @n This function sets the device Address to USB controller
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            addr          Address to set
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Configures device address registers
 *
 *   @b Modifies
 *   @n Device address registers
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            Uint16             addr;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_setDevAddr(CSL_USB0, addr);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_setDevAddr(CSL_UsbDevNum    devNum,
                          Uint16            addr);

/** ============================================================================
 *   @n@b USB_getFrameNo
 *
 *   @b Description
 *   @n This function reads current USB frame number
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbFrameNum
 *   @n                     Current USB frame number
 *
 *   <b> Pre Condition </b>
 *   @n  USB init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns the frame number
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            Uint16             addr;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_getFrameNo(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbFrameNum USB_getFrameNo(CSL_UsbDevNum    devNum);

/** ============================================================================
 *   @n@b USB_suspendDevice
 *
 *   @b Description
 *   @n This function suspends the USB device. This function informs the
 *      application about the device suspend through a call back function.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Puts the USB device into suspend mode.
 *
 *   @b Modifies
 *   @n USB FADDR_POWER register
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            Uint16             addr;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_suspendDevice(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_suspendDevice(CSL_UsbDevNum    devNum);

/** ============================================================================
 *   @n@b USB_selfwakeup
 *
 *   @b Description
 *   @n This function issues a self wakeup signal on the bus.
 *      This function can be mapped to an external event like pressing a button
 *      on the EVM. After responding to that event through a call back, USB
 *      device send a wakeup signal on the bus.
 *      NOTE: Application needs to send a delay as a parameter to this function.
 *      According to USB wakeup specification an approximate delay of
 *      10mSecs(Min 2 to Max 15 mSecs) is required to send a wakeup signal on
 *      the bus. This delay is introduced between setting the resume bit to '1'
 *      and restting it to '0'. Application needs to decide a delay such that
 *      it is maximum enough to satisfy USB wakeup protocol and minimum to
 *      satisfy the USB driver performance.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            wakeupDelay   Delay between Resume bit set and reset
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init be called successfully and an external event should occur.
 *
 *   <b> Post Condition </b>
 *   @n  Send a wakeup signal on the bus.
 *
 *   @b Modifies
 *   @n USB FADDR_POWER register
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            Uint16             addr;
            Uint32             wakeupDelay;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            //This function call should be mapped to an external event
            wakeupDelay = 1000000;
            status = USB_selfwakeup(CSL_USB0, wakeupDelay);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_selfwakeup(CSL_UsbDevNum    devNum,
                          Uint32           wakeupDelay);

/** ============================================================================
 *   @n@b USB_initPLL
 *
 *   @b Description
 *   @n Initializes PLL for USB operation. This function initializes the PLL to
 *      100 MHz. For configuring PLL to any other frequency use PLL module.
 *
 *   @b Arguments
 *   @verbatim
                   None
     @endverbatim
 *
 *   <b> Return Value </b> None
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes PLL
 *
 *   @b Modifies
 *   @n PLL registers
 *
 *   @b Example
 *   @verbatim
            USB_initPLL();
     @endverbatim
 *  ============================================================================
 */
void USB_initPLL(void);

/** ============================================================================
 *   @n@b USB_setParams
 *
 *   @b Description
 *   @n This function initializes the global end point handle array
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            hEpObj        Pointer to a array of endpoint handle.
            pSofTmrCnt    8-bit counter value for the pre SOF timer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes global end point handle array
 *
 *   @b Modifies
 *   @n Global end point handle array
 *
 *   @b Example
 *   @verbatim
            CSL_UsbEpObj    hEpObjArray[CSL_USB_ENDPOINT_COUNT];
            CSL_Status      status;

            status = USB_setParams(CSL_USB0, &hEpObjArray, 0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_setParams(CSL_UsbDevNum    devNum,
                         pUsbEpHandle     hEpObj[],
                         Uint16           pSofTmrCnt);

/** ============================================================================
 *   @n@b USB_getMaxPower
 *
 *   @b Description
 *   @n This function returns the maximum current drawn by the device.
 *      This is to inform the host about the device power requirement.
 *      This value is passed to the host through the configuration
 *      descriptor.
 *
 *   @b Arguments
 *   @verbatim
             None
     @endverbatim
 *
 *   <b> Return Value </b> Maximum current drawn by the device
 *
 *   <b> Pre Condition </b>
 *   @n  USB_init function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns the maximum current drawn by the device
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            Uint16             maxPower;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            .....
            maxPower = USB_getMaxPower(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
WORD USB_getMaxPower(CSL_UsbDevNum    devNum);

/** ============================================================================
 *   @n@b USB_setFullSpeedMode
 *
 *   @b Description
 *   @n This function is used to set the USB controller into full speed mode
 *      ep0DataSize is the size of the EP0 FIFO. This value should be within
 *      the range of values recommended by the USB specification.
 *
 *   @b Arguments
 *   @verbatim
             None
     @endverbatim
 *
 *   <b> Return Value </b> None
 *
 *   <b> Pre Condition </b>
 *   @n  USB Init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  USB will be set for full speed mode
 *
 *   @b Modifies
 *   @n USB registers
 *
 *   @b Example
 *   @verbatim
            CSL_UsbSetupStruct    usbSetup;
            CSL_Status            status;
            CSL_UsbConfig         usbConfig

            status = USB_init(&usbConfig);
            ....
            ....
            USB_setFullSpeedMode(0x20);

     @endverbatim
 *  ============================================================================
 */
void USB_setFullSpeedMode(Uint16    ep0DataSize);

/** ============================================================================
 *   @n@b USB_getDataCountReadFromFifo
 *
 *   @b Description
 *   @n This function returns the most recent data count read from the FIFO.
 *      This function is supposed to use for the OUT end point. This function
 *      always return 0 for IN end point.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
     @endverbatim
 *
 *   <b> Return Value </b>  Data count read from FIFO
 *
 *   <b> Pre Condition </b>
 *   @n  End point object should be initialized
 *
 *   <b> Post Condition </b>
 *   @n  Terminates data transfer in progress
 *
 *   @b Modifies
 *   @n USB registers
 *
 *   @b Example
 *   @verbatim
             void  (*evtHandler)();

             pUsbEpHandle     hEp;
            CSL_Status       status;
            Uint16           fifoDataCount;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);

            fifoDataCount = USB_getEvents(hEp);
     @endverbatim
 *  ============================================================================
 */
Uint16 USB_getDataCountReadFromFifo (pUsbEpHandle    hEp);

// determine the fifo size according to the transfer type and the maximum packet size
Uint16 USB_findFifoSize(Uint16 maxPktSize, CSL_UsbXferType xferType);

// processing all the EP0 events for enumeration
// call the CDC event handler for further processing
CSL_Status USB_coreEventProcessEp0(pUsbContext pContext);

/**
@} */

#ifdef __cplusplus
}
#endif

#endif    //_CSL_USB_H_

