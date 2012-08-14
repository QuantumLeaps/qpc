 /*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_uart.h
 *
 *  @brief UART functional layer API header file
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 8-Sep-2008 Created
 * ============================================================================
 */

/** @defgroup CSL_UART_API UART
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 *  The UART performs serial to parallel converiosn on data received from the peripheral
 *  device and parallel to serial conversion on data recieved from the CPU.The UART includes
 *  control capability and processor interrupt system that can be tailored to minimize software
 *  management of communications link
 *  @subsection References
 *
 */

#ifndef _CSL_UART_H_
#define _CSL_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cslr.h"
#include "csl_error.h"
#include "csl_types.h"
#include "cslr_uart.h"
#include "soc.h"
#include "csl_intc.h"
#include "csl_general.h"

/**
@defgroup CSL_UART_SYMBOL  UART Symbols Defined
@ingroup CSL_UART_API
*/
/**
@defgroup CSL_UART_DATASTRUCT  UART Data Structures
@ingroup CSL_UART_API
*/
/**
@defgroup CSL_UART_FUNCTION  UART Functions
@ingroup CSL_UART_API
*/
/**
@defgroup CSL_UART_ENUM UART Enumerated Data Types
@ingroup CSL_UART_API
*/

/**
@addtogroup CSL_UART_SYMBOL
@{
*/

/** global variable */

/*****************************************************************************\
          UART global macro declarations                                      *
\*****************************************************************************/
/** RBR & THR have same offset */
#define RBR THR
/** IIR & FCR have same offset */
#define IIR FCR
/** Each tranmitted/received bit lasts for 16 clk cycles,so baud rate multiplier
 *  is 16
 */
#define CSL_UART_BAUD_MULTIPLIER        (16u)
/** Macro to calculate the clock divider for the UART*/
#define CSL_UART_CLK_DIVIDER( baud )  ( Uint32 ) ( (Uint32)CSL_UART_BAUD_MULTIPLIER * (Uint32) ( baud) )
 /** Transmitter holding register empty interrupt identification value*/
#define    UART_EVT_TBEI_IID   0x01
 /** Received data available interrupt identification value*/
#define    UART_EVT_RBI_IID   0x02
 /** Receiver line status interrupt identification value*/
#define    UART_EVT_LSI_IID    0x03
/** Character timeout interrupt identification value*/
#define    UART_EVT_CTOI_IID   0x06

/** UART serial character word length 5*/
#define   CSL_UART_WORD5            0x05
/** UART serial character word length 6*/
#define   CSL_UART_WORD6            0x06
/** UART serial character word length 7*/
#define   CSL_UART_WORD7            0x07
/** UART serial character word length 8*/
#define   CSL_UART_WORD8            0x08

/**
ST EPS PEN
x   x   0 Parity disabled: No PARITY bit is transmitted or checked
0   0   1 Odd parity selected: Odd number of logic 1s
0   1   1 Even parity selected: Even number of logic 1s
1   0   1 Stick parity selected with PARITY bit transmitted and checked as set
1   1   1 Stick parity selected with PARITY bit transmitted and checked as cleared
*/

/**disable parity checking*/
#define   CSL_UART_DISABLE_PARITY   (0x00u)
/** SP - 0, EPS-0, PEN-1 odd parity selected */
#define   CSL_UART_ODD_PARITY       (0x01u)
/** SP - 0, EPS-1, PEN-1  Even parity selected */
#define   CSL_UART_EVEN_PARITY      (0x02u)
/** SP - 1, EPS-0, PEN-1  Stick parity selected
 * with parity bit transmitted and checked as set*/
#define   CSL_UART_MARK_PARITY      (0x03u)
/** SP - 1, EPS-1, PEN-1  Stick parity selected
 * with parity bit transmitted and checked as clear*/
#define   CSL_UART_SPACE_PARITY     (0x04u)

/**  UART FIFO disabled*/
#define   CSL_UART_FIFO_DISABLE     CSL_FMKT(UART_FCR_FIFOEN,DISABLE)

/**DMA MODE 1 disabled and trigger level 01*/
#define   CSL_UART_FIFO_DMA1_DISABLE_TRIG01   CSL_FMKT(UART_FCR_FIFOEN,ENABLE)|\
                    CSL_FMKT(UART_FCR_RXCLR,CLR)|CSL_FMKT(UART_FCR_TXCLR,CLR)|\
                    CSL_FMKT(UART_FCR_DMAMODE1,DISABLE)|CSL_FMKT(UART_FCR_RXFIFTL,CHAR1)

/**DMA MODE 1 disabled and trigger level 04*/
#define   CSL_UART_FIFO_DMA1_DISABLE_TRIG04   CSL_FMKT(UART_FCR_FIFOEN,ENABLE)|\
                    CSL_FMKT(UART_FCR_RXCLR,CLR)|CSL_FMKT(UART_FCR_TXCLR,CLR)|\
                    CSL_FMKT(UART_FCR_DMAMODE1,DISABLE)|CSL_FMKT(UART_FCR_RXFIFTL,CHAR4)

/**DMA MODE 1 disabled and trigger level 08*/
#define   CSL_UART_FIFO_DMA1_DISABLE_TRIG08   CSL_FMKT(UART_FCR_FIFOEN,ENABLE)|\
                    CSL_FMKT(UART_FCR_RXCLR,CLR)|CSL_FMKT(UART_FCR_TXCLR,CLR)|\
                    CSL_FMKT(UART_FCR_DMAMODE1,DISABLE)|CSL_FMKT(UART_FCR_RXFIFTL,CHAR8)

/**DMA MODE 1 disabled and trigger level 14*/
#define   CSL_UART_FIFO_DMA1_DISABLE_TRIG14   CSL_FMKT(UART_FCR_FIFOEN,ENABLE)|\
                    CSL_FMKT(UART_FCR_RXCLR,CLR)|CSL_FMKT(UART_FCR_TXCLR,CLR)|\
                    CSL_FMKT(UART_FCR_DMAMODE1,DISABLE)|CSL_FMKT(UART_FCR_RXFIFTL,CHAR14)

/**DMA MODE 1 enabled and trigger level 01*/
#define   CSL_UART_FIFO_DMA1_ENABLE_TRIG01   CSL_FMKT(UART_FCR_FIFOEN,ENABLE)|\
                    CSL_FMKT(UART_FCR_RXCLR,CLR)|CSL_FMKT(UART_FCR_TXCLR,CLR)|\
                    CSL_FMKT(UART_FCR_DMAMODE1,ENABLE)|CSL_FMKT(UART_FCR_RXFIFTL,CHAR1)

/**DMA MODE 1 enabled and trigger level 04*/
#define   CSL_UART_FIFO_DMA1_ENABLE_TRIG04   CSL_FMKT(UART_FCR_FIFOEN,ENABLE)|\
                    CSL_FMKT(UART_FCR_RXCLR,CLR)|CSL_FMKT(UART_FCR_TXCLR,CLR)|\
                    CSL_FMKT(UART_FCR_DMAMODE1,ENABLE)|CSL_FMKT(UART_FCR_RXFIFTL,CHAR4)

/**DMA MODE 1 enabled and trigger level 08*/
#define   CSL_UART_FIFO_DMA1_ENABLE_TRIG08   CSL_FMKT(UART_FCR_FIFOEN,ENABLE)|\
                    CSL_FMKT(UART_FCR_RXCLR,CLR)|CSL_FMKT(UART_FCR_TXCLR,CLR)|\
                    CSL_FMKT(UART_FCR_DMAMODE1,ENABLE)|CSL_FMKT(UART_FCR_RXFIFTL,CHAR8)

/**DMA MODE 1 enabled and trigger level 14*/
#define   CSL_UART_FIFO_DMA1_ENABLE_TRIG14   CSL_FMKT(UART_FCR_FIFOEN,ENABLE)|\
                    CSL_FMKT(UART_FCR_RXCLR,CLR)|CSL_FMKT(UART_FCR_TXCLR,CLR)|\
                    CSL_FMKT(UART_FCR_DMAMODE1,ENABLE)|CSL_FMKT(UART_FCR_RXFIFTL,CHAR14)
/**This Macro is used to set the value of 'loopBackEnable' memeber of CSL_UartSetup
 * strcuture.
 */
#define   CSL_UART_NO_LOOPBACK    (0x00u)
/**This Macro is used to set the value of 'loopBackEnable' memeber of CSL_UartSetup
 * strcuture.
 */
#define   CSL_UART_LOOPBACK       (0x01u)
/**This Macro is used to set the value of 'afeEnable' memeber of CSL_UartSetup
 * strcuture.
 */
#define   CSL_UART_NO_AFE    (0x00u)
/**This Macro is used to set the value of 'afeEnable' memeber of CSL_UartSetup
 * strcuture.
 */
#define   CSL_UART_AFE       (0x01u)
/**This Macro is used to set the value of ' rtsEnable' memeber of CSL_UartSetup
 * strcuture.
 */
#define   CSL_UART_NO_RTS    (0x00u)
/**This Macro is used to set the value of ' rtsEnable' memeber of CSL_UartSetup
 * strcuture.
 */
#define   CSL_UART_RTS       (0x01u)

/**
@}*/

/**
@addtogroup CSL_UART_ENUM
@{*/
/**
 * \brief UART instance numbers
 *
 */
typedef enum
{
    /** UART hardware instance 0                                             */
    CSL_UART_INST_0 = (0u),
    /** Invalid UART hardware instance                                       */
    CSL_UART_INST_INVALID = (1u)
} CSL_UartInsId;
/**
 *  \brief UART Device mode of transmission
 *
 *  This is used at device open and to configure the UART device
 */
typedef enum
{
    UART_POLLED=0,                /**< UART transfer is through UART polled mode   */
    UART_INTERRUPT,                /**< UART transfer is through UART intr mode     */
    UART_OPMODE_OTHER            /**< Undefined transfer mode                     */
}CSL_UartOpmode;


/**
 * \brief   This enum defines Xfer mode of UART
 */
typedef enum {
    /** UART in FIFO mode                                                     */
    CSL_UART_FIFO_MODE = 0,
    /** UART in NON FIFO mode                                                */
    CSL_UART_NON_FIFO_MODE = 1,
    /** Reserved values                                                      */
    CSL_UART_MODE_UNDEFINED = 2

} CSL_UartXferMode;

/**
 *\brief
 *   enum variable for the selection of UART interrupt
 *   Receiver Line Status Int,Transmitter Holding Register Empty Int,
 *   Receiver data available Int.
 */
typedef enum CSL_UartEventType
{
    /** receiver reg data available interrupt      */
    CSL_UART_RECVOR_REG_DATA_INTERRUPT,
    /** Transmittor reg empty interrupt           */
    CSL_UART_XMITOR_REG_EMPTY_INTERRUPT,
    /** Line status interrupt interrupt          */
    CSL_UART_RECVOR_LINE_STATUS_INTERRUPT
}CSL_UartEventType;

/**
@}*/

/**
\addtogroup CSL_UART_DATASTRUCT
@{
*/

/**************************************************************************\
* UART global data structure declarations                                  *
\**************************************************************************/
/**
 *  \brief UART Setup structure
 *
 *  This structure contains the UART setup parameters.
 *  Using this structure individual parameters can be passed
 */
typedef struct
{
   /** Input clock in Hz*/
    Uint32              clkInput;
   /**Baud rate  */
    Uint32              baud;
   /**Word length selection
      Valid values - 5,6,7, or 8bits*/
    Uint16              wordLength;
   /**Number of stop bits to be generated
      Valid values - 0 or 1 */
    Uint16              stopBits;
   /** Parity generation enable/disable*/
    Uint16              parity;
   /** Fifo control register */
    Uint16              fifoControl;
    /**Looppback mode enable/disable*/
    Uint16              loopBackEnable;
    /** AFE enable/disable*/
    Uint16              afeEnable;
    /** RTS enable/disable*/
    Uint16              rtsEnable;


} CSL_UartSetup;
/**
 *  \brief UART Config structure
 *  The members of this structure directly maps to a particular register of  UART
 */
typedef struct
{
   /**DLL holds least significant bits of the divisor*/
   Uint16              DLL;
   /**DLH holds most significant bits of the divisor*/
   Uint16              DLH;
   /**LCR controls word length,parity selection,stop bit generation*/
   Uint16              LCR;
   /**FCR controls fifo enable/disable,trigger level selection,dma selection*/
   Uint16              FCR;
   /** MCR controls autoflow selection,loopback feature,RTS selection   */
   Uint16              MCR;
} CSL_UartConfig;
/**
 *  \brief UART callback pointer structure
 *
 *  This structure contains pointers to UART interrupt callback functions
 */
typedef struct
{
    /**Receiver line status interrupt*/
    void (*lsiAddr)(void);
    /** received data available and character timeout indication interrupt*/
       void (*rbiAddr)(void);
    /** transmiter holding register empty interrupt*/
    void (*tbeiAddr)(void);
    /** Character timeout interrupt*/
    void (*ctoi)(void);

} CSL_UartIsrAddr;
/**
 *  \brief UART Object structure
 *
 *  This structure maintains information of UART object
 */
typedef struct CSL_UartObj
{
    /** Instance id of UART     */
    Uint16 insId;
    /** UART register overlay structure pointer     */
    CSL_UartRegsOvly uartRegs;
    /** System control register overlay structure pointer                    */
    CSL_SysRegsOvly sysAddr;
    /**Operation mode of UART interrupt/polled*/
    CSL_UartOpmode opmode;
    /**Trigger level of fifo*/
    Uint16 trigLevel;
    /** ISR  dispatch table for storing call back function pointers*/
    Uint32 UART_isrDispatchTable[7];
}CSL_UartObj;
/** Handle to the UART object structure                                       */
typedef CSL_UartObj  *CSL_UartHandle;

/**
@}
*/


/*****************************************************************************\
 * UART function declarations                                                                              *
\*****************************************************************************/

/** @addtogroup CSL_UART_FUNCTION
@{
*/

/** ============================================================================
 *   @n@b UART_init
 *
 *   @b Description
 *   @n    This is the initialization function for the uart CSL. The function
 *      must be called before calling any other API from this CSL. This
 *      will initialize the uart object.
 *   @b Arguments
 *   @verbatim
        uartInstId  Instance number of the uart
        uartObj     Pointer to the uart object
        opmode      Operation mode of uart
     @endverbatim
 *
 *   <b> Return Value </b>    CSL_Status
 *   @li                CSL_SOK - Init call is successful
 *   @li                CSL_ESYS_INVPARAMS- Invalid parameter
 *   @li                CSL_UART_INVALID_INST_ID- Invalid instance id
 *   <b> Pre Condition </b>
 *   @n    None
 *   <b> Post Condition </b>
 *   @n    PLL object structure is populated
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
        CSL_Status              status;
        CSL_UartObj                uartObj;
        Uint32                  uartInstId;
        CSL_UartOpmode opmode = UART_POLLED;
        uartInstId = CSL_UART_INST_0;
        status = UART_init(&uartObj,uartInstId);
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_init(CSL_UartObj *  uartObj,Uint32 uartInstId,
                     CSL_UartOpmode opmode);
/** ============================================================================
 *   @n@b UART_setup
 *
 *   @b Description
 *   @n    The main function of this API is to setup the
 *        baud rate,configure fifo,enable transmitter and receiver UART
 *      setup word size,stop bits and parity.
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
        uartSetup     Pointer to the UART_Setup structure.
     @endverbatim
 *   <b> Return Value </b>    CSL_Status
 *   @li                CSL_SOK - UART_setup call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid Handle
 *   @li                CSL_ESYS_INVPARAMS- Invalid uartSetup
 *   <b> Pre Condition </b>
 *   @n    UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n    Setup the UART for transmission and reception.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
        CSL_Status       status;
         CSL_UartObj         uartObj;
        Uint32           uartInstId;
        CSL_UartSetup          uartSetup;
        uartInstId = 0;
        status = UART_init(&uartObj,uartInstId);
        status =  UART_setup(CSL_UartHandle hUart,&uartSetup);
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_setup(CSL_UartHandle hUart, CSL_UartSetup *uartSetup);

/** ============================================================================
 *   @n@b UART_config
 *
 *   @b Description
 *   @n    This is a low level API than UART_setup API. It's used to configure
 *      registers directly.
 *
 *   @b Arguments
 *   @verbatim
         hUart  Handle to the UART.
        pConfig     Pointer to the CSL_UartConfig structure.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK - UART_config call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   @li                CSL_ESYS_INVPARAMS- Invalid pointer to UART Config strucutre.
 *   <b> Pre Condition </b>
 *   @n  UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n   Confgiure the registers.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
        CSL_Status              status;
         CSL_UartObj                uartObj;
        Uint32                      uartInstId;
        CSL_UartConfig          Config;

        Config.DLL  =
        Config.DLH =
        Config.LCR  =
        Config.FCR =
        Config.MCR =
        uartInstId = 0;

        status = UART_init(&uartObj,uartInstId);
        status =  UART_config(CSL_UartHandle hUart,&Config);
     @endverbatim
 *  ============================================================================
 */

 CSL_Status UART_config(CSL_UartHandle hUart,CSL_UartConfig *uartConfig) ;
/** ============================================================================
 *   @n@b UART_setupBaudRate
 *
 *   @b Description
 *   @n    This API is used to set the baud rate.
 *
 *   @b Arguments
 *   @verbatim
        hUart       Handle to the UART
        clkInput    Input clock frequency in Hz
        baudRate    Baud rate.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK - UART_setupBaudRate call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   <b> Pre Condition </b>
 *   @n  UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n   DLL and DLH registers are set.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
        CSL_Status              status;
         CSL_UartObj                uartObj;
        Uint32                  uartInstId;
        UART_baud                 uartBaud;
        uartInstId = 0;

        status = UART_init(&uartObj,uartInstId);
        uartBaud.clkInput = 60000000;
        uartBaud.baudRate = 2400;
        status =  UART_setupBaudRate(CSL_UartHandle hUart,&Config);
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_setupBaudRate(CSL_UartHandle hUart,Uint32 clkInput,Uint32 baudRate);
/** ============================================================================
 *   @n@b UART_reset
 *
 *   @b Description
 *   @n    This API is used to reset the UART receiver and transmitter.
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK - UART_setupBaudRate call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   <b> Pre Condition </b>
 *   @n  UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n   uart receiver and transmitter are in reset state.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
          CSL_Status              status;
          CSL_UartObj                uartObj;
         CSL_UartHandle             hUart;
         Uint32                  uartInstId;
         Bool                    brecvstatus;
         Bool                    bxmitstatus;
         uartInstId = 0;
         status = UART_init(&uartObj,uartInstId);
         hUart = CSL_UartHandle(&uartObj);
         status = UART_resetOff(hUart);
         status = UART_reset(hUart);
         brecvstatus = UART_getRecvorResetStatus(hUart);
         bxmitstatus = UART_getXmitorResetState(hUart);

     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_reset(CSL_UartHandle hUart);
/** ============================================================================
 *   @n@b UART_resetOff
 *
 *   @b Description
 *   @n    This API is used to set the UART receiver and transmitter.
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK - UART_setupBaudRate call is successful
 *   @li                         CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   <b> Pre Condition </b>
 *   @n  UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n   uart receiver and transmitter are on state.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
         CSL_Status              status;
          CSL_UartObj                uartObj;
         CSL_UartHandle             hUart;
         Uint32                  uartInstId;
         Bool                    brecvstatus;
         Bool                    bxmitstatus;
         uartInstId = 0;
         status = UART_init(&uartObj,uartInstId);
         hUart = CSL_UartHandle(&uartObj);
         status = UART_resetOff(hUart);
         brecvstatus = UART_getRecvorResetStatus(hUart);
         bxmitstatus = UART_getXmitorResetState(hUart);
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_resetOff(CSL_UartHandle hUart);
/** ============================================================================
 *   @n@b UART_eventEnable
 *
 *   @b Description
 *   @n    This API is used to enable events like Receiver data availabe,
 *        Transmitter holding register empty
 *
 *   @b Arguments
 *   @verbatim
         hUart  Handle to the UART.
         isrMask Interested particular events can be enabled by setting the
        correspoding bits in the variable
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_eventEnable call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   <b> Pre Condition </b>
 *   @n  UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n The corresponding events are enabled.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
         CSL_Status              status;
         CSL_UartObj                uartObj;
           CSL_UartSetup                 uartSetup;
         Uint16 isrMask = 0x01;
         Uint32 uartInstId = 0x0;
        uartSetup.afeEnable = CSL_UART_NO_AFE;
        uartSetup.baud = 2400;
        uartSetup.clkInput = 60000000;
        uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
        uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
        uartSetup.parity =  CSL_UART_DISABLE_PARITY;
        uartSetup.rtsEnable = CSL_UART_NO_RTS;
        uartSetup.stopBits = 0;
        uartSetup.wordLength = CSL_UART_WORD8;
        status = UART_init(&uartObj,uartInstId);
        status = UART_Setup( &uartObj,&uartSetup);
        status = UART_eventEnable(&uartObj,CSL_RTC_RECVOR_REG_DATA_INTERRUPT)
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_eventEnable(CSL_UartHandle hUart, CSL_UartEventType uartEvent);
/** ============================================================================
 *   @n@b UART_eventDisable
 *
 *   @b Description
 *   @n This API is used to enable events like Receiver data availabe,
 *        Transmitter holding register empty
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
        isrMask Particular events can be disabled by setting correspoding
           bits in the variable
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_eventDisable call is successful
 *   @li                CSL_ESYS_BADHANDLE - Invalid UART Handle
 *   <b> Pre Condition </b>
 *   @n  UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n  The correspoding events are  disabled.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
          CSL_Status              status;
          CSL_UartObj                uartObj;
         Uint32                  uartInstId;
         CSL_UartSetup              uartSetup;
         Uint16 isrMask = 0x01;
         uartInstId = 0;

         uartSetup.afeEnable = CSL_UART_NO_AFE;
         uartSetup.baud = 2400;
         uartSetup.clkInput = 60000000;
         uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
         uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
         uartSetup.parity =  CSL_UART_DISABLE_PARITY;
         uartSetup.rtsEnable = CSL_UART_NO_RTS;
         uartSetup.stopBits = 0;
         uartSetup.wordLength = CSL_UART_WORD8;

         status = UART_init(&uartObj,uartInstId);
         status = UART_eventDisable(&uartObj,CSL_RTC_RECVOR_REG_DATA_INTERRUPT);
         status = CSL_UartSetup( hUart,&uartSetup);
         status = UART_eventEnable(hUart,CSL_RTC_XMITOR_REG_EMPTYINTERRUPT);

     @endverbatim
 *  ============================================================================
 */

CSL_Status  UART_eventDisable(CSL_UartHandle hUart,CSL_UartEventType uartEvent);
/** ============================================================================
 *   @n@b UART_read
 *
 *   @b Description
 *   @n UART_read attempts to read up to count characters
 *        from RBR(Recieve Buffer Register) into the buffer starting at pBuf
 *
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
           pBuf   Output buffer
          count  No of characters to read.
        timeout Timeout value
                0 - wait indefinitely for DR bit to get set.
                x - wait x value for DR bit to get set.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li          CSL_SOK -  UART_read call is successful.
 *   @li          CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   @li          CSL_ESYS_INVPARAMS- Invalid address of the  buffer.
 *   @li          CSL_EUART_TIMEOUT - Read had a timeout due to DR bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and CSL_UartSetup should be called successfully.
 *   <b> Post Condition </b>
 *   @n  pBuf contains count number of characters.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @n bBuf is modified
 *   @b Example
 *   @verbatim
          CSL_Status              status;
          CSL_UartObj                uartObj;
         Uint32                  uartInstId;
         CSL_UartSetup             uartSetup;
         CSL_UartHandle hUart;
         Char  buf[12];
         Uint16 N = 12;
         uartInstId = 0;

         status = UART_init(&uartObj,uartInstId);
         hUart = (CSL_UartHandle)&uartObj;
         status = CSL_UartSetup( hUart,&uartSetup);
         status = UART_read(hUart,buf,N,1000) ;

     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_read(CSL_UartHandle hUart,
                     Char* pBuf, Uint16 count, Uint32 timeout);

/** ============================================================================
 *   @n@b UART_write
 *
 *   @b Description
 *   @n UART_write writes up to count characters to the THR(Transmitter Holding Register)
 *        from the buffer starting at pBuf
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
        pBuf   Input buffer
        count  No of characters to write.
        timeout Timeout value
                0 - wait indefinitely for THRE bit to get set.
                x - wait x value for THRE bit to get set.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li               CSL_SOK -  UART_write call is successful
 *   @li               CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   @li               CSL_ESYS_INVPARAMS- Invalid address of the pBuf
 *   @li               CSL_EUART_TIMEOUT - Write had a timeout due to THRE bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and CSL_UartSetup should be called successfully.
 *   <b> Post Condition </b>
 *   @n  count number of characters have written to THR.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @b Example
 *   @verbatim
              CSL_Status              status;
              CSL_UartObj                uartObj;
            Uint32                      uartInstId;
            CSL_UartSetup          uartSetup;
            CSL_UartHandle hUart;
            Char  buf[12];
            Uint16 N = 12;
            uartInstId = 0;

            status = UART_init(&uartObj,uartInstId);
            hUart = (CSL_UartHandle)&uartObj;
            status =  CSL_UartSetup( hUart,&uartSetup);
            status = UART_write(hUart,buf,N,1000) ;
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_write( CSL_UartHandle hUart,
                       Char *pBuf, Uint16 count, Uint32 timeout) ;
/** ============================================================================
 *   @n@b UART_fgetc
 *
 *   @b Description
 *   @n UART_fgetc reads a single character from the RBR into elem.
 *
 *   @b Arguments
 *   @verbatim
         hUart  Handle to the UART.
        elem   A size one buffer.
        timeout Timeout value
                0 - wait indefinitely for DR bit to get set.
                x - wait x value for DR bit to get set.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_fgetc call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   @li                CSL_ESYS_INVPARAMS- Invalid address of input buffer
 *   @li                CSL_EUART_TIMEOUT -  had a timeout due to DR bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and CSL_UartSetup should be called successfully.
 *   <b> Post Condition </b>
 *   @n elem contains a single character.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @n elem is modified
 *   @b Example
 *   @verbatim
              CSL_Status              status;
              CSL_UartObj                uartObj;
             Uint32                      uartInstId;
            CSL_UartSetup          uartSetup;
            CSL_UartHandle hUart;
            Char  buf[1];
            uartInstId = 0;

            status = UART_init(&uartObj,uartInstId);
            hUart = (CSL_UartHandle)&uartObj;
            status =  CSL_UartSetup( hUart,&uartSetup);
            status = UART_fgetc(hUart,buf,200)
     @endverbatim
 *  ============================================================================
 */
CSL_Status UART_fgetc(CSL_UartHandle hUart, Char *elem, Uint32 timeout);
/** ============================================================================
 *   @n@b UART_fputc
 *
 *   @b Description
 *   @n UART_fputc writes a single character to THR(Transmitter Holding Register).
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
        elem   Character to transmit
        timeout Timeout value
                0 - wait indefinitely for THRE bit to get set.
                x - wait x value for THRE bit to get set.
     @endverbatim
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_fputc call is successful
 *   @li                CSL_ESYS_BADHANDLE - Handle is invalid
 *   @li                CSL_EUART_TIMEOUT - had a timeout due to THRE bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and CSL_UartSetup should be called successfully.
 *   <b> Post Condition </b>
 *   @n A character got transmitted.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @b Example
 *   @verbatim
              CSL_Status              status;
              CSL_UartObj                uartObj;
            Uint32                  uartInstId;
            CSL_UartSetup                 uartSetup;
            CSL_UartHandle hUart;
            Char  buf[1];
            uartInstId = 0;

            status = UART_init(&uartObj,uartInstId);
            hUart = (CSL_UartHandle)&uartObj;
            status = CSL_UartSetup( hUart,&uartSetup);
            status = UART_fputc(hUart,buf[0],200)

     @endverbatim
 *  ============================================================================
 */
CSL_Status UART_fputc(CSL_UartHandle hUart, const Char elem, Uint32 timeout);

/** ============================================================================
 *   @n@b UART_fgets
 *
 *   @b Description
 *   @n UART_fgets reads in at most one less than bufSize characters from RBR and stores them
 *      into the buffer pointed to by pBuf.Reading stops after a newline. If a newline is read, it is
 *      stored into the buffer. A "\0" is stored after the last character in the buffer.
 *   @b Arguments
 *   @verbatim
            hUart  Handle to the UART.
                pBuf   Output buffer
            bufSize  Size of output buffer
            timeout Timeout value
                    0 - wait indefinitely for DR bit to get set.
                    x - wait x value for DR bit to get set.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_fgets call is successful
 *   @li                CSL_ESYS_BADHANDLE -  Invalid handle.
 *   @li                CSL_ESYS_INVPARAMS- Invalid output buffer
 *   @li                CSL_EUART_INVALID_LENGTH- Invalid string length.
 *   @li                CSL_EUART_TIMEOUT - had a timeout due to DR bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and CSL_UartSetup should be called successfully.
 *   <b> Post Condition </b>
 *   @n  Input buffer will contain a string.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @n pBuf is modified
 *   @b Example
 *   @verbatim
             CSL_Status              status;
             CSL_UartObj                uartObj;
            Uint32                      uartInstId;
            CSL_UartSetup          uartSetup;
            CSL_UartHandle hUart;
            Char  buf[12];
            Uint16 N = 12;
            uartInstId = 0;

            status = UART_init(&uartObj,uartInstId);
            hUart = (CSL_UartHandle)&uartObj;
            status =  CSL_UartSetup( hUart,&uartSetup);
            status = UART_fgets(hUart,buf,N,100) ;

     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_fgets(CSL_UartHandle hUart,
                      Char * pBuf, Uint16 bufSize, Uint32 timeout);
/** ============================================================================
 *   @n@b UART_fputs
 *
 *   @b Description
 *   @n UART_fputs writes the string pointed by pBuf to THR, without its trailing "\0".
 *
 *   @b Arguments
 *   @verbatim

        hUart  Handle to the UART.
        pBuf   Input buffer
        timeout Timeout value
                0 - wait indefinitely for THRE bit to get set.
                x - wait x value for THRE bit to get set.

      @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_fputs call is successful
 *   @li                CSL_ESYS_BADHANDLE -  UART_fputs call is successful
 *   @li                CSL_ESYS_INVPARAMS - Invalid input buffer
 *   @li                CSL_EUART_TIMEOUT   - had a timeout due to THRE bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and CSL_UartSetup should be called successfully.
 *   <b> Post Condition </b>
 *   @n  A string will get transmitted.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @b Example
 *   @verbatim
              CSL_Status              status;
              CSL_UartObj                uartObj;
            Uint32                  uartInstId;
            CSL_UartSetup                 uartSetup;
            CSL_UartHandle hUart;
            Char  buf[12] ="Test";
            uartInstId = 0;
            status =  UART_init(&uartObj,uartInstId);
            hUart  = (CSL_UartHandle)&uartObj;
            status =  UART_Setup( hUart,&uartSetup);
            status =  UART_fputs(hUart,buf);
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_fputs(CSL_UartHandle hUart, const Char* pBuf, Uint32 timeout);
/** ============================================================================
 *   @n@b UART_getEventId
 *
 *   @b Description
 *   @n UART_getEventId returns the event id.
 *
 *   @b Arguments
 *   @verbatim
        None
      @endverbatim
 *
 *   <b> Return Value </b> Int16
 *   @li          Returns Eventid - call is successful.
 *   @li          Returns CSL_ESYS_BADHANDLE - Handle is NULL.
 *   <b> Pre Condition </b>
 *   @n  UART_init  should be called successfully.
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *   @b Example
 *   @verbatim
              CSL_Status              status;
              CSL_UartObj                uartObj;
            CSL_UartHandle             hUart;
            Uint16                  uartEvtId;
            Uint32 uartInstId = 0;
            status =  UART_init(&uartObj,uartInstId);
            hUart = (CSL_UartHandle)(&uartObj);
            uartEvtId =  UART_getEventId(hUart);
     @endverbatim
 *  ============================================================================
 */

 Int16 UART_getEventId(CSL_UartHandle hUart);

 /** ============================================================================
  *   @n@b UART_getEventId
  *
  *   @b Description
  *   @n UART_getEventId returns the event id in the Handle.
  *
  *   @b Arguments
  *   @verbatim
         None
       @endverbatim
  *
  *   <b> Return Value </b> Uint16
  *   @li               IRQ_EVT_UART -UART_getEventId call is successful.
  *   <b> Pre Condition </b>
  *   @n  UART_init  should be called successfully.
  *   <b> Post Condition </b>
  *   @n  None
  *
  *   @b Modifies
  *   @n None
  *   @b Example
  *   @verbatim
             CSL_Status              status;
             CSL_UartObj                uartObj;
             Uint32                      uartInstId;
             Uint16                      uartEvtId;
             uartInstId = 0;
             status =  UART_init(&uartObj,uartInstId);
             uartEvtId =  UART_getEventId();
      @endverbatim
  *  ============================================================================
  */

CSL_Status UART_getConfig(CSL_UartHandle hUart,CSL_UartConfig *config);

/** ============================================================================
 *   @n@b UART_setCallback
 *
 *   @b Description
 *   @n UART_setCallback will set call back functions in isrDispatchTable.
 *
 *   @b Arguments
 *   @verbatim
         hUart - Handle to the UART.
         isrAddr - pointer to the     CSL_UartIsrAddr.
      @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                              CSL_SOK - UART_setCallback call is successful.
 *   <b> Pre Condition </b>
 *   @n  UART_init  should be called successfully.
 *   <b> Post Condition </b>
 *   @n  Setup call back functions in the UART_isrDispatchTable
 *
 *   @b Modifies
 *   @n hUart
 *   @b Example
 *   @verbatim
          CSL_Status              status;
          CSL_UartObj                uartObj;
            Uint32                      uartInstId;
            CSL_UartHandle          hUart;
            CSL_UartIsrAddr         isrAddr;
            uartInstId = 0;

            status =  UART_init(&uartObj,uartInstId);
            hUart = (CSL_UartHandle)&uartObj;
            status =   UART_setCallback(hUart,&isrAddr);

     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_setCallback(CSL_UartHandle hUart,CSL_UartIsrAddr *isrAddr);

/**
@}*/

#ifdef __cplusplus
}
#endif

#endif
