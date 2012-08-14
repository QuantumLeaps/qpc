/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_dat.h
 *
 *  @brief Functional layer API header file for DAT CSL
 *
 *  Path: \(CSLPATH)/inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 16-Sept-2008 Added DAT of CSL.
 * ============================================================================
 */

/** @defgroup CSL_DAT_API DAT
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 * The DAT module is abstarction of DMA module its having some default
 *    configuration of DMA Register and used to transfer the Data from
 *    one location to another similar to DMA.
 *  @subsection References
 *    -# TMS320VC5505 DSP Direct Memory Access (DMA) Controller
 *    -# SPRUFO9 May 2008.
 *
 */


#ifndef _CSL_DAT_H_
#define _CSL_DAT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <soc.h>
#include <tistdtypes.h>
#include <cslr_sysctrl.h>
#include <csl_types.h>
#include <csl_error.h>
#include <csl_general.h>

/**
@defgroup CSL_DAT_SYMBOL  DAT Symbols Defined
@ingroup CSL_DAT_API
*/

/**
@defgroup CSL_DAT_DATASTRUCT  DAT Data Structures
@ingroup CSL_DAT_API
*/

/**
@defgroup CSL_DAT_ENUM DAT Enumerated Data Types
@ingroup CSL_DAT_API
*/

/**
@defgroup CSL_DAT_FUNCTION  DAT Functions
@ingroup CSL_DAT_API
*/

/**
@defgroup CSL_DAT_INLINE_FUNCTION  DAT Inline Function
@ingroup CSL_DAT_API
*/

/*****************************************************************************\
          DAT global macro declarations
\*****************************************************************************/

/** @addtogroup CSL_DAT_SYMBOL
 @{ */

/** Time out value for comming out of the infinite loop   */
#define CSL_DAT_TIME_OUT                (0xFFF)
/** hash define value for channel busy status  */
#define CSL_DAT_CHANNEL_BUSY            (0x1)
/** hash define value for channel free status  */
#define CSL_DAT_CHANNEL_FREE            (0)
/** hash define value for burst length  */
#define CSL_DAT_TXBURST_WORD_LENGTH        (0x0)
/** hash define value for channel enable bit   */
#define CSL_DAT_CHANNEL_ENABLE            (Uint16)1
/** hash define value for channel disable bit  */
#define CSL_DAT_CHANNEL_DISABLE           (Uint16)0
/** Total no of channels for all DMA Engines    */
#define CSL_DAT_CHAN_MAX               (16)
/** Total no of channels for all DMA Engines    */
#define CSL_DAT_CHAN_PRIORITY_HIGH     (0xF)
/** Total no of channels for all DMA Engines    */
#define CSL_DAT_CHAN_PRIORITY_LOW      (0x0)

#ifndef _CSL_DMA_H_
/** Minimum Number of Bytes transfered by DMA    */
#define CSL_DMA_MIN_TX_SIZE             (4)
/** mask value for 16 bit variable    */
#define CSL_DMA_UINT16_MASK             (0xFFFF)
/** no of bits in a Uint16 datatype  variable    */
#define CSL_DMA_UINT16_NUMBER_BITS      (0x0010)
/** shift value to change cpu addr for DMA    */
#define CSL_DMA_ADDR_SHIFT              (0x01)
/** Value for fixed addr mode  */
#define CSL_DMA_ADDR_MODE_FIXED         (0x02)
/** Value for incr. addr mode  */
#define CSL_DMA_ADDR_MODE_INCREMENT     (0x00)
/** DARAM starting address */
#define CSL_DMA_DARAM_START_ADDR        (0x00060)
/** DARAM ending address       */
#define CSL_DMA_DARAM_END_ADDR          (0x07FFF)
/** SARAM starting address     */
#define CSL_DMA_SARAM_START_ADDR        (0x08000)
/** SARAM ending address       */
#define CSL_DMA_SARAM_END_ADDR          (0x27FFF)
/** DARAM starting address seen by DMA*/
#define CSL_DMA_DARAM_ADDR_OFFSET       (0x010000)
/** SARAM starting address seen by DMA*/
#define CSL_DMA_SARAM_ADDR_OFFSET       (0x080000)
#endif
/**
@} */

/**************************************************************************\
* DAT global typedef declarations                                         *
\**************************************************************************/

/** @addtogroup CSL_DAT_ENUM
 @{ */

/**
 * \brief DAT data transfer type
 *
 */
typedef enum
{
    CSL_DAT_1D2D,   /**< 1 Dimension to 2 Dimension */
    CSL_DAT_2D1D,   /**< 2 Dimension to 1 Dimension */
    CSL_DAT_2D2D    /**< 2 Dimension to 2 Dimension */
} CSL_DAT_TxType;

/**
 * \brief DAT engine number
 *
 * Enumeration for the DAT engine numbers
 */
typedef enum
{
    CSL_DAT_ENGINE0 = 0,                    /**< DAT engine instance 0       */
    CSL_DAT_ENGINE1,                        /**< DAT engine instance 1       */
    CSL_DAT_ENGINE2,                        /**< DAT engine instance 2       */
    CSL_DAT_ENGINE3,                        /**< DAT engine instance 3       */
    CSL_DAT_ENGINE_INV                      /**< DAT engine invalid instance */
} CSL_DATEngineId;

/**
 * \brief DAT channel number
 *
 * Enumeration for the total channels DAT is having
 */
typedef enum
{
    CSL_DAT_CHAN0 = 0,                      /**< DAT channel number 0        */
    CSL_DAT_CHAN1,                          /**< DAT channel number 1        */
    CSL_DAT_CHAN2,                          /**< DAT channel number 2        */
    CSL_DAT_CHAN3,                          /**< DAT channel number 3        */
    CSL_DAT_CHAN4,                          /**< DAT channel number 4        */
    CSL_DAT_CHAN5,                          /**< DAT channel number 5        */
    CSL_DAT_CHAN6,                          /**< DAT channel number 6        */
    CSL_DAT_CHAN7,                          /**< DAT channel number 7        */
    CSL_DAT_CHAN8,                          /**< DAT channel number 8        */
    CSL_DAT_CHAN9,                          /**< DAT channel number 9        */
    CSL_DAT_CHAN10,                         /**< DAT channel number 10       */
    CSL_DAT_CHAN11,                         /**< DAT channel number 11       */
    CSL_DAT_CHAN12,                         /**< DAT channel number 12       */
    CSL_DAT_CHAN13,                         /**< DAT channel number 13       */
    CSL_DAT_CHAN14,                         /**< DAT channel number 14       */
    CSL_DAT_CHAN15,                         /**< DAT channel number 15       */
    CSL_DAT_CHAN_INV                        /**< DAT channel number Invalid  */
} CSL_DATChanNum;
/**
@} */

/**
\addtogroup CSL_DAT_DATASTRUCT
@{ */

/**
 * \brief DAT channel object
 *
 * Contains the Information of a DAT Object.
 */
typedef struct
{
    CSL_DmaRegsOvly        dmaRegs;    /**< register overlay struct pointer */
    CSL_DATChanNum         chanNum;    /**< DAT channel number for DMA Engines */
    CSL_DATEngineId        datInstNum; /**< DAT instance number (Engine Num)*/
    Uint16                 chanPrio;   /**< channel priority */
    Bool                  isChanFree;  /**< Indicates Availability of chan  */
} CSL_DAT_ChannelObj;

/**
 * \brief DAT Handle
 *
 */
typedef CSL_DAT_ChannelObj     *CSL_DAT_Handle;

/**
@} */


/******************************************************************************
 * DAT function declarations
 *****************************************************************************/

/** @addtogroup CSL_DAT_FUNCTION
 @{ */
/** ===========================================================================
 *   @n@b DAT_open
 *
 *   @b Description
 *   @n This function returns the handle to the DAT
 *      instance. This handle is passed to all other CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            chanNum        channel number to use.
            chanPriority   channel priority.
            pDatChanObj    pointer to dat channel object.
            status         Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_DAT_Handle
 *   @n                     Valid DAT handle will be returned if
 *                          status value is equal to CSL_SOK.
 *
 *   <b> Pre Condition </b>
 *   @n  Before Calling this API User must configure the system control register
 *       to enabling the clock for DMA Controller.
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is returned in the status variable. If status
 *              returned is
 *   @li            CSL_SOK            - Valid DAT handle is returned
 *   @li            CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   @b Modifies
 *   @n    1. The status variable
 *
 *   @b Example
 *   @verbatim

            CSL_status           status;
            CSL_DATChanNum       chanNum;
            Uint16               chanPriority;
            CSL_DAT_ChannelObj   pDatChanObj
            CSL_DAT_Handle       hDAT;
            ...
            chanNum = CSL_DAT_CHAN0;
            chanNum = CSL_DAT_CHAN_PRIORITY_HIGH;

            hDAT = DAT_open(chanNum, chanPriority, &pDatChanObj, &status);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_DAT_Handle DAT_open (
    CSL_DATChanNum      chanNum,
    Uint16              chanPriority,
    CSL_DAT_ChannelObj  *pDatChanObj,
    CSL_Status          *status
);

/** ===========================================================================
 *   @n@b DAT_close
 *
 *   @b Description
 *   @n This function closes the specified handle to DAT.
 *
 *   @b Arguments
 *   @verbatim
            hDAT            Handle to the DAT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  DAT_open() API must be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  All the registers of this DMA channel will be reset.
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_DATChanNum       chanNum;
            Uint16               chanPriority;
            CSL_DAT_ChannelObj   pDatChanObj
            CSL_DAT_Handle       hDAT;
            ...
            chanNum = CSL_DAT_CHAN0;
            chanNum = CSL_DAT_CHAN_PRIORITY_HIGH;

            hDAT = DAT_open(chanNum, chanPriority, &pDatChanObj, &status);

            status = DAT_Close(hDAT);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DAT_close (
    CSL_DAT_Handle         hDAT
);

/** ===========================================================================
 *   @n@b DAT_copy
 *
 *   @b Description
 *   @n It configures the DMA Controller registers of particular handle by some
 *      default values and trigger the transfer.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA.
            srcAddr         source location .
            destAddr        destination location .
            dataLength      length of data to transfer.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Parameters are invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  DAT_open() API must be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  DAT_wait() API must be called after this.
 *
 *   @b Example
 *   @verbatim

      #define CSL_DAT_BUFFER_LENGTH 100

            CSL_DAT_ChannelObj   pDatChanObj
            CSL_DAT_Handle       hDAT;
            CSL_DAT_TxType       type;
            CSL_DATChanNum       chanNum;
            Uint16               chanPriority;
            Uint32               srcAddr;
            Uint32               destAddr;
            Uint16               dataLength;
            CSL_status           status;

     Uint16 srcBuffer[CSL_DAT_BUFFER_LENGTH]
     Uint16 destBuffer[CSL_DAT_BUFFER_LENGTH]
                ...
            chanNum = CSL_DAT_CHAN0;
            chanNum = CSL_DAT_CHAN_PRIORITY_HIGH;

            hDAT = DAT_open(chanNum, chanPriority, &pDatChanObj, &status);
            ...
            srcAddr      = srcBuffer;
            destAddr     = destBuffer;
            dataLength   = CSL_DAT_BUFFER_LENGTH;
            status = DAT_copy(hDAT, srcAddr, destAddr, dataLength);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status DAT_copy (
    CSL_DAT_Handle            hDAT,
    Uint32                    srcAddr,
    Uint32                    destAddr,
    Uint16                    dataLength
);

/** ===========================================================================
 *   @n@b DAT_fill
 *
 *   @b Description
 *   @n It configures the DMA Controller registers of particular handle by some
 *      default values and fill the destination location with passed data value.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA.
            destAddr        destination location.
            pdataValue      pointer to the data.
            dataLength      length of data to transfer.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Parameters are invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  DAT_open() API must be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  DAT_wait() API must be called after this.
 *
 *   @b Example
 *   @verbatim

      #define CSL_DAT_BUFFER_LENGTH 100

            CSL_DAT_ChannelObj   pDatChanObj
            CSL_DAT_Handle       hDAT;
            CSL_DAT_TxType       type;
            CSL_DATChanNum       chanNum;
            Uint16               chanPriority;

            Uint32           srcAddr;
            Uint32           destAddr;
            Uint16           dataLength;
            CSL_status       status;
            Uint32           pdataValue;

 Uint16 destBuffer[CSL_DAT_BUFFER_LENGTH]
            ...
            chanNum = CSL_DAT_CHAN0;
            chanNum = CSL_DAT_CHAN_PRIORITY_HIGH;

            hDAT = DAT_open(chanNum, chanPriority, &pDatChanObj, &status);
            ...
            destAddr     = destBuffer;
            dataLength   = CSL_DAT_BUFFER_LENGTH;
            pdataValue   = 0xABCD;

            status = DAT_fill(hDAT, destAddr, dataLength, &pdataValue);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status DAT_fill (
    CSL_DAT_Handle            hDAT,
    Uint32                    destAddr,
    Uint16                    dataLength,
    Uint32                    *pdataValue
);

/** ===========================================================================
 *   @n@b DAT_wait
 *
 *   @b Description
 *   @n This function wait till the completion of Data Transfer.
 *
 *   @b Arguments
 *   @verbatim
            hDAT            Handle to the DAT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  DAT_open() API must be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Example
 *   @verbatim
      #define CSL_DAT_BUFFER_LENGTH 100

            CSL_DAT_ChannelObj   pDatChanObj
            CSL_DAT_Handle       hDAT;
            CSL_DAT_TxType       type;
            CSL_DATChanNum       chanNum;
            Uint16               chanPriority;

            Uint32           srcAddr;
            Uint32           destAddr;
            Uint16           dataLength;
            CSL_status       status;
            Uint32           pdataValue;

     Uint16 destBuffer[CSL_DAT_BUFFER_LENGTH]
            ...
            chanNum = CSL_DAT_CHAN0;
            chanNum = CSL_DAT_CHAN_PRIORITY_HIGH;

            hDAT = DAT_open(chanNum, chanPriority, &pDatChanObj, &status);
            ...
            destAddr     = destBuffer;
            dataLength   = CSL_DAT_BUFFER_LENGTH;
            pdataValue   = 0xABCD;

            status   = DAT_fill(hDAT, destAddr, dataLength, &pdataValue);

            status = DAT_wait(hDAT);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DAT_wait (
    CSL_DAT_Handle         hDAT
);

/**
@} */

#ifdef __cplusplus
}
#endif

#endif /* _CSL_DMA_H_ */

