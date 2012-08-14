/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_dma.h
 *
 *  @brief Functional layer API header file for DMA CSL
 *
 *  Path: \(CSLPATH)/inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 25-Aug-2008 Added DMA for CSL.
 * 27-Aug-2008 Updated for Doxygen.
 * 28-Aug-2008 Added Return Types for Error checking for API's parameter.
 * 01-Oct-2008 Added additional parameter CSL_DMA_ChannelObj pointer to the
 *             DMA_open() API.
 * 08-May-2009 Added DMA_swapWords API for C5505 PG1.4
 * 26-May-2009 Modified as per the review comments
 * 17-Dec-2009 Added Support for Ping-Pong mode for chip C5515
 * 11-Jan-2010 Modified as per the review comments
 * ============================================================================
 */

/** @defgroup CSL_DMA_API DMA
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 * The C5505 Processor has four DMA controller and each controller has four
 * channel.Each controller can be work independentely. Bascially the DMA work
 * is to transfer  data from one location to another. DMA take byte address
 *  format so view for DMA of memory location will differ with respect to CPU.
 *  @subsection References
 *    -# TMS320VC5505 DSP Direct Memory Access (DMA) Controller
 *    -# SPRUFO9 May 2008.
 *
 */


#ifndef _CSL_DMA_H_
#define _CSL_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr_dma.h>
#include <cslr_sysctrl.h>
#include <tistdtypes.h>
#include <csl_types.h>
#include <csl_error.h>
#include <soc.h>
#include <csl_general.h>

/**
@defgroup CSL_DMA_SYMBOL  DMA Symbols Defined
@ingroup CSL_DMA_API
*/

/**
@defgroup CSL_DMA_DATASTRUCT  DMA Data Structures
@ingroup CSL_DMA_API
*/

/**
@defgroup CSL_DMA_ENUM DMA Enumerated Data Types
@ingroup CSL_DMA_API
*/

/**
@defgroup CSL_DMA_FUNCTION  DMA Functions
@ingroup CSL_DMA_API
*/

/**
@defgroup CSL_DMA_INLINE_FUNCTION  DMA Inline Function
@ingroup CSL_DMA_API
*/

/*****************************************************************************\
          DMA global macro declarations
\*****************************************************************************/

/** @addtogroup CSL_DMA_SYMBOL
 @{ */
/** Minimum Number of Bytes transfered by DMA    */
#define CSL_DMA_MIN_TX_SIZE                 (4)
/** Total no of channels for all DMA Engines    */
#define CSL_DMA_CHAN_MAX                    (16)
/** Number of channels on each DMA Engine    */
#define CSL_DMA_CHAN_COUNT                  (4)
/** hash define value for channel busy status  */
#define CSL_DMA_CHANNEL_BUSY                (1)
/** hash define value for channel free status  */
#define CSL_DMA_CHANNEL_FREE                (0)
/** hash define value for channel enable bit   */
#define CSL_DMA_CHANNEL_ENABLE                (Uint16)1
/** hash define value for channel disable bit  */
#define CSL_DMA_CHANNEL_DISABLE                (Uint16)0
/** hash define value for dma start and sync bit mask value  */
#define CSL_DMA_DMATCR2_DMASTART_SYNC_MASK  (0x8004)
/** mask value for 16 bit variable    */
#define CSL_DMA_UINT16_MASK                 (0xFFFF)
/** no of bits in a Uint16 datatype  variable    */
#define CSL_DMA_UINT16_NUMBER_BITS          (0x0010)
/** DMA reset clock cycle count                                   */
#define CSL_DMA_RESET_CLOCK_CYCLE           (0x20)
/** Value for fixed addr mode  */
#define CSL_DMA_ADDR_MODE_FIXED             (0x02)
/** Value for incr. addr mode  */
#define CSL_DMA_ADDR_MODE_INCREMENT         (0x00)
/** shift value to change cpu addr for DMA    */
#define CSL_DMA_ADDR_SHIFT                  (0x01)
/** DARAM starting address */
#define CSL_DMA_DARAM_START_ADDR            (0x00060)
/** DARAM ending address       */
#define CSL_DMA_DARAM_END_ADDR              (0x07FFF)
/** SARAM starting address     */
#define CSL_DMA_SARAM_START_ADDR            (0x08000)
/** SARAM ending address       */
#define CSL_DMA_SARAM_END_ADDR              (0x27FFF)
/** DARAM starting address seen by DMA*/
#define CSL_DMA_DARAM_ADDR_OFFSET           (0x010000)
/** SARAM starting address seen by DMA*/
#define CSL_DMA_SARAM_ADDR_OFFSET            (0x080000)
/** Macro to mask 16 bits in DMA buffer */
#define CSL_DMA_16BIT_MASK                  (0xFFFFu)
/** Maro to shift 16 bits in DMA buffer */
#define CSL_DMA_16BIT_SHIFT                 (16u)
/**
@} */

/**************************************************************************\
* DMA global typedef declarations                                         *
\**************************************************************************/

/** @addtogroup CSL_DMA_ENUM
 @{ */

/**
 * \brief DMA engine number.
 *
 * Enumeration for the DMA engine numbers.
 */
typedef enum
{
    CSL_DMA_ENGINE0 = 0,                    /**< DMA engine instance 0       */
    CSL_DMA_ENGINE1,                        /**< DMA engine instance 1       */
    CSL_DMA_ENGINE2,                        /**< DMA engine instance 2       */
    CSL_DMA_ENGINE3,                        /**< DMA engine instance 3       */
    CSL_DMA_ENGINEINV                       /**< Invalid DMA engine Instance */
} CSL_DMAEngineId;

/**
 * \brief DMA channel number.
 *
 * Enumeration for the total channels DMA is having.
 */
typedef enum
{
    CSL_DMA_CHAN0 = 0,                      /**< DMA channel number 0        */
    CSL_DMA_CHAN1,                          /**< DMA channel number 1        */
    CSL_DMA_CHAN2,                          /**< DMA channel number 2        */
    CSL_DMA_CHAN3,                          /**< DMA channel number 3        */
    CSL_DMA_CHAN4,                          /**< DMA channel number 4        */
    CSL_DMA_CHAN5,                          /**< DMA channel number 5        */
    CSL_DMA_CHAN6,                          /**< DMA channel number 6        */
    CSL_DMA_CHAN7,                          /**< DMA channel number 7        */
    CSL_DMA_CHAN8,                          /**< DMA channel number 8        */
    CSL_DMA_CHAN9,                          /**< DMA channel number 9        */
    CSL_DMA_CHAN10,                         /**< DMA channel number 10       */
    CSL_DMA_CHAN11,                         /**< DMA channel number 11       */
    CSL_DMA_CHAN12,                         /**< DMA channel number 12       */
    CSL_DMA_CHAN13,                         /**< DMA channel number 13       */
    CSL_DMA_CHAN14,                         /**< DMA channel number 14       */
    CSL_DMA_CHAN15,                         /**< DMA channel number 15       */
    CSL_DMA_CHAN_INV                        /**< DMA Invalid channel         */
} CSL_DMAChanNum;

/**
 * \brief DMA Auto reload bit.
 *
 * If this bit is enabled, DMA auto reloads source and destination addresses
 * and restarts the data transfer.
 */
typedef enum
{
    CSL_DMA_AUTORELOAD_DISABLE,   /**< Auto reload mode is disabled          */
    CSL_DMA_AUTORELOAD_ENABLE     /**< Auto reload mode is enabled           */
} CSL_DMAAutoReloadMode;

/**
 * \brief DMA burst mode bit
 *
 * This represents the number of words to be transferred by a channel
 * before giving the DMA FIFO to next channel.
 */
typedef enum
{
    CSL_DMA_TXBURST_1WORD,        /**< One 32-bit transfer:1-word burst      */
    CSL_DMA_TXBURST_2WORD,        /**< Two 32-bit transfer:2-word burst      */
    CSL_DMA_TXBURST_4WORD,        /**< Four 32-bit transfer:4-word burst     */
    CSL_DMA_TXBURST_8WORD,        /**< Eight 32-bit transfer:8-word burst    */
    CSL_DMA_TXBURST_16WORD        /**< Sixteen 32-bit transfer:16-word burst */
} CSL_DMATxBurstLen;

/**
 * \brief DMA transfer status parameter.
 *
 * Gives the status of the data transfer. This is passed to the application
 * after completing the data transfer through callback.
 */
typedef enum
{
    CSL_DMA_TRANSFER_COMPLETE,    /**< Transfer is completed successfully    */
    CSL_DMA_TRANSFER_ERROR        /**< Error in data transfer                */
} CSL_DMATransferStatus;

/**
 * \brief DMA event Id.
 *
 * This represents the event by which DMA transfer is being triggered.
 */
typedef enum
{
    CSL_DMA_EVT_NONE = 0,        /**< No DMA event(SW trig./mem to mem)     */
    CSL_DMA_EVT_I2S0_TX = 1,     /**< Tx of I2S0 data event triggers DMA    */
    CSL_DMA_EVT_I2S0_RX = 2,     /**< Rx of I2S0 data event triggers DMA    */
    CSL_DMA_EVT_I2S1_TX = 1,     /**< Tx of I2S1 data event triggers DMA    */
    CSL_DMA_EVT_I2S1_RX = 2,     /**< Rx of I2S1 data event triggers DMA    */
    CSL_DMA_EVT_I2S2_TX = 1,     /**< Tx of I2S2 data event triggers DMA    */
    CSL_DMA_EVT_I2S2_RX = 2,     /**< Rx of I2S2 data event triggers DMA    */
    CSL_DMA_EVT_I2S3_TX = 4,     /**< Tx of I2S3 data event triggers DMA    */
    CSL_DMA_EVT_I2S3_RX = 5,     /**< Rx of I2S3 data event triggers DMA    */
    CSL_DMA_EVT_MMC_SD0_TX = 5,  /**< MMC/SD0 TX event triggers DMA         */
    CSL_DMA_EVT_MMC_SD0_RX = 6,  /**< MMC/SD0 RX event triggers DMA         */
    CSL_DMA_EVT_MMC_SD1_TX = 7,  /**< MMC/SD1 TX event triggers DMA         */
    CSL_DMA_EVT_MMC_SD1_RX = 8,  /**< MMC/SD1 RX event triggers DMA         */
    CSL_DMA_EVT_TIMER0 = 12,     /**< timer0 event triggers DMA             */
    CSL_DMA_EVT_TIMER1 = 13,     /**< timer1 event triggers DMA             */
    CSL_DMA_EVT_TIMER2 = 14,     /**< timer2 event triggers DMA             */
    CSL_DMA_EVT_UART_TX = 5,     /**< Tx of UART data event triggers DMA    */
    CSL_DMA_EVT_UART_RX = 6,     /**< Rx of UART data event triggers DMA    */
    CSL_DMA_EVT_I2C_TX = 1,      /**< Tx of I2C data event triggers DMA     */
    CSL_DMA_EVT_I2C_RX = 2,      /**< Rx  of I2C data event triggers DMA    */
    CSL_DMA_EVT_SAR_AD = 3,      /**< SAR A/D event triggers DMA            */
    CSL_DMA_EVT_INVALID          /**< Invalid event Id for error checking   */
} CSL_DMAEvtType;

/**
 * \brief DMA channel direction.
 *
 * This is provided to the application to give information about DMA
 * operation. DMA read or DMA write.
 */
typedef enum
{
    CSL_DMA_READ,                 /**< Reading data from a device            */
    CSL_DMA_WRITE                 /**< Writing data to a device              */
} CSL_DMAChanDir;

/**
 * \brief DMA data transfer type.
 *
 * This is provided to the application to give information about DMA
 * transfer type is between IO and memory, or memory and memory.
 */
typedef enum
{
    CSL_DMA_TRANSFER_IO_MEMORY,   /**< Data transfer is b/w IO & memory      */
    CSL_DMA_TRANSFER_MEMORY       /**< Data transfer is b/w memory to memory */
} CSL_DMATransferType;

/**
 * \brief Type of the DMA triggering.
 *
 * This provides the information about DMA triggering.
 * DMA transfer can be triggered by software or hardware event.
 */
typedef enum
{
    CSL_DMA_SOFTWARE_TRIGGER,     /**< Triggering by software instruction    */
    CSL_DMA_EVENT_TRIGGER         /**< Triggering by hardware event          */
} CSL_DMATriggerType;

/**
 * \brief State of the DMA Interrupt.
 *
 * This provides the information about DMA Interrupt.
 * After completion of data transfer will be served by interrupt.
 */
typedef enum
{
    CSL_DMA_INTERRUPT_DISABLE,     /**< Interrupt Disable    */
    CSL_DMA_INTERRUPT_ENABLE       /**< Interrupt Enable     */
} CSL_DMAInterruptState;

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

/**
 * \brief Enum to enable or disable Ping-Pong Mode
 *
 * This Enum provides the option for the application programs to choose
 * enabling/disabling of Ping-Pong Mode
 */
typedef enum
{
    CSL_DMA_PING_PONG_DISABLE,     /**< Disables the Ping-Pong Mode    */
    CSL_DMA_PING_PONG_ENABLE       /**< Enables the Ping-Pong Mode     */
} CSL_DMAPingPongMode;

#endif

/**
@} */

/**
\addtogroup CSL_DMA_DATASTRUCT
@{ */

/**
 * \brief DMA channel object.
 *
 * Contains the Information of a DMA Object.
 */
typedef struct
{
    CSL_DmaRegsOvly        dmaRegs;    /**< register overlay struct pointer  */
    CSL_DMAChanNum         chanNum;   /**< DMA channel number for DMA Engins */
    CSL_DMAEngineId        dmaInstNum; /**< DMA instance number (Engine Num) */
    Bool                   isChanFree; /**< Indicates Availability of chan   */
    CSL_DMAChanDir         chanDir;    /**< Channel direction - Rd/Wr        */
    CSL_DMATriggerType     trigger;    /**< Type of DMA triggering HW/SW     */
    CSL_DMATransferType    trfType;    /**< Type of the data transfer        */
    CSL_DMAInterruptState  dmaInt;     /**< state of DMA interrupt           */

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
    Bool                   pingPongEnabled;
#endif

    /**< This flag notifies whether the ping-pong mode is enaled or not
         If TRUE - Ping-Pong is Enabled
         If FALSE - Ping-Pong is Disabled */
} CSL_DMA_ChannelObj;

/**
 * \brief DMA Handle.
 *
 */
typedef CSL_DMA_ChannelObj     *CSL_DMA_Handle;

/**
 * \brief DMA configuration structure.
 *
 * Contains configuration parameters for DMA hardware registers.
 * This has to be filled by the drivers using DMA.
 */
typedef struct
{
#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
    CSL_DMAPingPongMode       pingPongMode;/**< Enable Ping-Pong Mode        */
#endif

    CSL_DMAAutoReloadMode     autoMode;/**< Auto reloadmode bit          */
    CSL_DMATxBurstLen         burstLen;/**< Length of the transmit burst */
    CSL_DMATriggerType        trigger; /**< Type of DMA triggering HW/SW */
    CSL_DMAEvtType            dmaEvt;  /**< Type of the DMA event        */
    CSL_DMAInterruptState     dmaInt;  /**< state of DMA interrupt       */
    CSL_DMAChanDir            chanDir; /**< Channel direction - Rd/Wr    */
    CSL_DMATransferType       trfType; /**< Type of the data transfer    */
    Uint16                    dataLen; /**< Length of data to transfer
                                                in number of bytes           */
    Uint32                    srcAddr; /**< Address of source data buffer */
    Uint32                    destAddr;/**< Address of destination data buffer */
} CSL_DMA_Config;

/**
@} */


/******************************************************************************
 * DMA function declarations
 *****************************************************************************/

/** @addtogroup CSL_DMA_FUNCTION
 @{ */
/** ===========================================================================
 *   @n@b DMA_init
 *
 *   @b Description
 *   @n This is the initialization function for the DMA CSL. The function
 *      must be called before calling any other API from this CSL. This
 *      function is for enabling the clock to the DMA Controller.
 *      Currently, the function just return status CSL_SOK.
 *      This function should be called only once during system initialization
 *      time. This function gates the clock for all the four DMA modules.
 *      Application programs may disable the clocks for DMA modules which are
 *      are not in use.
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                        CSL_SOK - Always returns
 *
 *   <b> Pre Condition </b>
 *   @n  This should be call first before calling any other DMA Function.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_status       status;

            status = DMA_init();
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_init (void);

/** ===========================================================================
 *   @n@b DMA_open
 *
 *   @b Description
 *   @n This function returns the handle to the DMA
 *      instance. This handle is passed to all other CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            chanNum     channel number to use.
            pDmaChanObj pointer to dma channel object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_DMA_Handle
 *   @n                     Valid DMA handle will be returned if
 *                          status value is equal to CSL_SOK.
 *
 *   <b> Pre Condition </b>
 *   @n  DMA_init() API should be called before DMA_open().
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is updated in the status variable. If status
 *              returned is
 *   @li            CSL_SOK            - Valid DMA handle is returned
 *   @li            CSL_ESYS_INVPARAMS - Invalid parameter
 *   @n   2.    CSL_DMA_ChannelObj object structure is populated
 *
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. CSL_DMA_ChannelObj object structure
 *
 *   @b Example
 *   @verbatim

            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj
            CSL_DMA_Handle       hDMA;
            ...

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_DMA_Handle DMA_open (
    CSL_DMAChanNum       chanNum,
    CSL_DMA_ChannelObj   *pDmaChanObj,
    CSL_Status           *status
);

/** ===========================================================================
 *   @n@b DMA_close
 *
 *   @b Description
 *   @n This function closes the specified handle to DMA.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  DMA_open() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function no other API should be call
 *       for the same dma Handle.
 *
 *   @b Modifies
 *   @n    1. The DMA register will assigned to reset values for this handle.
 *   @n    2. CSL_DMA_ChannelObj object structure
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj
            CSL_DMA_Handle       hDMA;
            ...

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

            status = DMA_Close(hDMA);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_close (
    CSL_DMA_Handle         hDMA
);

/** ===========================================================================
 *   @n@b DMA_config
 *
 *   @b Description
 *   @n It configures the DMA Controller registers of particular handle as per
 *      the values passed in the DMA config structure.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA.
            Config          Pointer to DMA Config structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Congig Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  DMA_open() must be called successfully before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n 1. DMA registers will be updated as per config parameter
 *   @n 2. CSL_DMA_ChannelObj Object will be updated to store some info
 *              as passed config parameter.
 *
 *   @b Example
 *   @verbatim

        #define CSL_DMA_BUFFER_SIZE 1024

            CSL_DMA_Handle       hDMA;
            CSL_DMA_Config       pConfig;
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj

            Uint16 dmaSRCBuff[CSL_DMA_BUFFER_SIZE];
            Uint16 dmaDESTBuff[CSL_DMA_BUFFER_SIZE];
            ...

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
            pConfig.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
#endif

            pConfig.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            pConfig.burstLen     = CSL_DMA_TXBURST_8WORD;
            pConfig.trigger      = CSL_DMA_SOFTWARE_TRIGGER;
            pConfig.dmaEvt       = CSL_DMA_EVT_NONE;
            pConfig.dmaInt       = CSL_DMA_INTERRUPT_DISABLE;
            pConfig.chanDir      = CSL_DMA_READ;
            pConfig.trfType      = CSL_DMA_TRANSFER_MEMORY;
            pConfig.dataLen      = CSL_DMA_BUFFER_SIZE * 2;
            pConfig.srcAddr      = (Uint32)dmaSRCBuff;
            pConfig.destAddr     = (Uint32)dmaDESTBuff;

            status = DMA_Config(hDMA, &pConfig);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_config (
    CSL_DMA_Handle            hDMA,
    CSL_DMA_Config            *pConfig
);

/** ===========================================================================
 *   @n@b DMA_getConfig
 *
 *   @b Description
 *   @n Gets the config information for the different operations of DMA.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA
            Config          Pointer to DMA Config structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Both Parametrs are valid.
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - invalid Congig Pointer.
 *
 *   <b> Pre Condition </b>
 *   @n  DMA_open() must be called successfully before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n   1.    The Config pointer will be updated to store the DMA Register
 *              information as per DMA Handle passed.
 *
 *   @b Example
 *   @verbatim
            CSL_DMA_Handle       hDMA;
            CSL_DMA_Config       pConfig;
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

            status = DMA_getConfig(hDMA, &pConfig);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_getConfig (
    CSL_DMA_Handle            hDMA,
    CSL_DMA_Config            *pConfig
);

/** ===========================================================================
 *   @n@b DMA_start
 *
 *   @b Description
 *   @n This function trigger to DMA for data transfer.
 *
 *   @b Arguments
 *   @verbatim

            hDMA            Handle to the DMA

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Start successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n DMA_config API must be called before calling this API for same handle.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *    @n Destination buffer will be modified after this call.
 *
 *   @b Example
 *   @verbatim

        #define CSL_DMA_BUFFER_SIZE 1024

            CSL_DMA_Handle       hDMA;
            CSL_DMA_Config       pConfig;
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj

        Uint16 dmaSRCBuff[CSL_DMA_BUFFER_SIZE];
        Uint16 dmaDESTBuff[CSL_DMA_BUFFER_SIZE];

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
            pConfig.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
#endif

            pConfig.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            pConfig.burstLen     = CSL_DMA_TXBURST_8WORD;
            pConfig.trigger      = CSL_DMA_SOFTWARE_TRIGGER;
            pConfig.dmaEvt       = CSL_DMA_EVT_NONE;
            pConfig.dmaInt       = CSL_DMA_INTERRUPT_DISABLE;
            pConfig.chanDir      = CSL_DMA_READ;
            pConfig.trfType      = CSL_DMA_TRANSFER_MEMORY;
            pConfig.dataLen      = CSL_DMA_BUFFER_SIZE * 2;
            pConfig.srcAddr      = (Uint32)dmaSRCBuff;
            pConfig.destAddr     = (Uint32)dmaDESTBuff;

            status = DMA_Config(hDMA, &pConfig);

            status = DMA_start (hDMA);

     @endverbatim
 * ===========================================================================
 */
CSL_Status DMA_start (
    /** Pointer to the object that holds reference to the
     *  DMA handle object.
     */
    CSL_DMA_Handle            hDMA
);

/** ===========================================================================
 *   @n@b DMA_stop
 *
 *   @b Description
 *   @n This function set the DMA controller for not transfering the data.
 *
 *   @b Arguments
 *   @verbatim

            hDMA            Handle to the DMA

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Stop successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n 1. DMA_open() must be called successfully before this function call
 *      2. Call DMA_config() for the handle return by DMA_open()
 *      3. call DMA_start() and in the mean while call DMA_stop() if transfer
 *          is not required.
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

        #define CSL_DMA_BUFFER_SIZE 1024

            CSL_DMA_Handle       hDMA;
            CSL_DMA_Config       pConfig;
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj

        Uint16 dmaSRCBuff[CSL_DMA_BUFFER_SIZE];
        Uint16 dmaDESTBuff[CSL_DMA_BUFFER_SIZE];

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
            pConfig.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
#endif

            pConfig.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            pConfig.burstLen     = CSL_DMA_TXBURST_8WORD;
            pConfig.trigger      = CSL_DMA_SOFTWARE_TRIGGER;
            pConfig.dmaEvt       = CSL_DMA_EVT_NONE;
            pConfig.dmaInt       = CSL_DMA_INTERRUPT_DISABLE;
            pConfig.chanDir      = CSL_DMA_READ;
            pConfig.trfType      = CSL_DMA_TRANSFER_MEMORY;
            pConfig.dataLen      = CSL_DMA_BUFFER_SIZE * 2;
            pConfig.srcAddr      = (Uint32)dmaSRCBuff;
            pConfig.destAddr     = (Uint32)dmaDESTBuff;

            status = DMA_Config(hDMA, &pConfig);

            status = DMA_start (hDMA);

            status = DMA_stop (hDMA);

     @endverbatim
 * ===========================================================================
 */
CSL_Status DMA_stop (
    /** Pointer to the object that holds reference to the
     *  DMA handle object.
     */
    CSL_DMA_Handle            hDMA
);

/** ===========================================================================
 *   @n@b DMA_reset
 *
 *   @b Description
 *   @n Function reset all the register as per passed handle.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Reset successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n 1. DMA_open() must be called successfully before this function call
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  DMA Register will be populated with the reset values.
 *
 *   @b Example
 *   @verbatim
            CSL_DMA_Handle       hDMA;
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

            status = DMA_reset(hDMA);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_reset (
    CSL_DMA_Handle         hDMA
);

/** ===========================================================================
 *   @n@b DMA_swapWords
 *
 *   @b Description
 *   @n This function swaps words in a given buffer
 *
 *   @b Arguments
 *   @verbatim
            dataBuffer    Pointer to the buffer which words needs to be swaped
            dataLength    Length of the valid data buffer in the buffer
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Reset successful
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid parameters
 *
 *   <b> Pre Condition </b>
 *   @n None
 *
 *   <b> Post Condition </b>
 *    @n Swaps the words in the given buffer
 *
 *   @b Modifies
 *   @n  Order of data in the given buffer
 *
 *   @b Example
 *   @verbatim
            #define DMA_BUF_SIZE (256)

            Uint16        dmaBuf [DMA_BUF_SIZE];
            CSL_Status    status;

            status = DMA_swapWords (dmaBuf, DMA_BUF_SIZE);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_swapWords (Uint16    *dataBuffer,
                          Uint32    dataLength);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

/** ===========================================================================
 *   @n@b DMA_getLastTransferType
 *
 *   @b Description
 *   @n This function returns type of the last trasfer during Ping-Pong
 *      buffer mode. This function will return a 1 or 0.
 *      In ping-pong buffer mode with reload enabled, DMA data transfer keeps
 *      switching between Ping and Pong data buffers. This function is used to
 *      know which buffer is transferred by DMA in the last transfer.
 *      0 - Ping Set Transfer completed
 *      1 - Pong Set Transfer completed
 *
 *   @b Arguments
 *   @verbatim
            hDMA      Handle to the DMA
            status    Status variable to update function Success/Failure status
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    0 - Ping Set Transfer completed
 *   @li                    1 - Pong Set Transfer completed
 *
 *   <b> Pre Condition </b>
 *   @n DMA_start() must be called successfully before this function call
 *
 *   <b> Post Condition </b>
 *   @n Returns last transfer status
 *
 *   @b Modifies
 *   @n none
 *
 *   @b Example
 *   @verbatim
            CSL_DMA_Handle    hDMA;
            CSL_Status        status;
            Bool              txStatus;

            status = DMA_start(hDMA);

            status = DMA_getLastTransferType(hDMA, &status);
     @endverbatim
 *  ===========================================================================
 */
Bool DMA_getLastTransferType (CSL_DMA_Handle    hDMA,
                              CSL_Status        *status);
#endif
/**
@} */

/******************************************************************************
 * CSL DMA Inline function
 *****************************************************************************/
/** @addtogroup CSL_DMA_INLINE_FUNCTION
 @{ */
/** ===========================================================================
 *   @n@b DMA_getStatus
 *
 *   @b Description
 *   @n This function return the status whether data transfer has done or not
 *      as per passed CSL_DMA_Handle.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA
     @endverbatim
 *
 *   <b> Return Value </b>  Int
 *   @li                    0                  - Data Transfer done
 *   @li                    1                  - Data Transfer is happening.
 *   @li                    CSL_ESYS_BADHANDLE - Invalid DMA handle
 *
 *   <b> Pre Condition </b>
 *   @n 1. DMA_open() must be called successfully before this function call
 *      2. Call DMA_config() for the handle return by DMA_open()
 *      3. Start trasfer by calling DMA_start().
 *
 *   <b> Post Condition </b>
 *    @n This checking is applicable for Transfer in Polling mode.
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim
        #define CSL_DMA_BUFFER_SIZE 1024

            CSL_DMA_Handle       hDMA;
            CSL_DMA_Config       pConfig;
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj
            Int                  dmaStatus;

        Uint16 dmaSRCBuff[CSL_DMA_BUFFER_SIZE];
        Uint16 dmaDESTBuff[CSL_DMA_BUFFER_SIZE];

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
            pConfig.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
#endif

            pConfig.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            pConfig.burstLen     = CSL_DMA_TXBURST_8WORD;
            pConfig.trigger      = CSL_DMA_SOFTWARE_TRIGGER;
            pConfig.dmaEvt       = CSL_DMA_EVT_NONE;
            pConfig.dmaInt       = CSL_DMA_INTERRUPT_DISABLE;
            pConfig.chanDir      = CSL_DMA_READ;
            pConfig.trfType      = CSL_DMA_TRANSFER_MEMORY;
            pConfig.dataLen      = CSL_DMA_BUFFER_SIZE * 2;
            pConfig.srcAddr      = (Uint32)dmaSRCBuff;
            pConfig.destAddr     = (Uint32)dmaDESTBuff;

            status = DMA_Config(hDMA, &pConfig);

            status = DMA_start (hDMA);

            dmaStatus = DMA_getStatus (hDMA);

     @endverbatim
 * ===========================================================================
 */
static inline
Int DMA_getStatus (
    /** Pointer to the object that holds reference to the
     *  DMA handle object.
     */
    CSL_DMA_Handle            hDMA
)
{
    Int       status;
    Uint16    chanNum;

    if(hDMA != NULL)
    {
        chanNum = hDMA->chanNum;

        while(chanNum >= CSL_DMA_PER_CNT)
        {
            chanNum = chanNum - CSL_DMA_PER_CNT;
        }

        switch((CSL_DMAChanNum)chanNum)
        {
            case CSL_DMA_CHAN0:
                status = CSL_FEXT(hDMA->dmaRegs->DMACH0TCR2, DMA_DMACH0TCR2_EN);
                break;
            case CSL_DMA_CHAN1:
                status = CSL_FEXT(hDMA->dmaRegs->DMACH1TCR2, DMA_DMACH1TCR2_EN);
                break;
            case CSL_DMA_CHAN2:
                status = CSL_FEXT(hDMA->dmaRegs->DMACH2TCR2, DMA_DMACH2TCR2_EN);
                break;
            case CSL_DMA_CHAN3:
                status = CSL_FEXT(hDMA->dmaRegs->DMACH3TCR2, DMA_DMACH3TCR2_EN);
                break;
        }
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return status;
}
/**
@} */

#ifdef __cplusplus
}
#endif

#endif /* _CSL_DMA_H_ */

