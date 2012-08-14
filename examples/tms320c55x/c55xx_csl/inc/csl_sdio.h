/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @defgroup CSL_SDIO_API SDIO
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * SDIO CSL provides a set of APIs to utilize the SDIO functionality provided
 * C5515/C5505 DSP Hw controller. Hw devices that uses SDIO CSL should be
 * compliant to SDIO specification. SDIO specification is the extension for
 * the SD card specification.
 *
 * @subsection References
 *
 *  TMS320VC5505 MMCSD DRAFT USERS GUIDE.pdf
 */

/** @file csl_sdio.h
 *
 *  @brief Functional layer API header file for SDIO CSL
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 21-Dec-2009 Created
 * 25-Jan-2009 Formatted the code
 * ============================================================================
 */

#ifndef _CSL_SDIO_H_
#define _CSL_SDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <soc.h>
#include <csl_types.h>
#include <csl_error.h>
#include <tistdtypes.h>

/**
\defgroup CSL_SDIO_API SDIO
*/
/**
@defgroup CSL_SDIO_SYMBOL SDIO Symbols Defined
@ingroup CSL_SDIO_API
*/

/**
@defgroup CSL_SDIO_DATASTRUCT SDIO Data Structures
@ingroup CSL_SDIO_API
*/

/**
@defgroup CSL_SDIO_ENUM  SDIO Enumerated Data Types
@ingroup CSL_SDIO_API
*/

/**
@defgroup CSL_SDIO_FUNCTION  SDIO Functions
@ingroup CSL_SDIO_API
*/

/**
@defgroup CSL_SDIOC_INLINE_FUNCTION  SDIO Inline Functions
@ingroup CSL_SDIO_API
*/

/*****************************************************************************\
          SDIO global macro declarations
\*****************************************************************************/

/** \addtogroup CSL_SDIO_SYMBOL
* @{ */

/** Stuff bits used to fill the command argument                             */
#define CSL_SDIO_STUFF_BITS           (0x0000)
/** Reset SDIO bit field                                                     */
#define CSL_SDIO_RESET               (0x1u)
/** Reset count for SDIO                                                     */
#define CSL_SDIO_RESET_COUNT         (0x20u)
/** Time delay                                                               */
#define CSL_SDIO_DELAY_TIME          (100u)
/** Time out duration for response                                           */
#define CSL_SDIO_RESPONSE_TIMEOUT    (0xFFFFu)
/** Time out duration for command retries during card initialization         */
#define CSL_SDIO_INIT_TIMEOUT        (0xFFFFu)
/** Time out duration for data read/write                                    */
#define CSL_SDIO_DATA_RW_TIMEOUT     (0xFFFFu)

/** SDIO command retry count during card detection and initialization        */
#define CSL_SDIO_CMD_RETRY_COUNT     (500u)

/** Minimum clock division for which both card can respond                   */
#define CSL_SDIO_CLK_DIV_INIT        (70u)
/** This bit is used to check whether the card is in busy state              */
#define CSL_SDIO_BUSY_STATE          (0x01u)
/** This bit is used to check whether the FIFO is empty                      */
#define CSL_SDIO_FIFO_EMPTY          (0x20u)
/** This bit is used to check whether the card has responded to a command    */
#define CSL_SDIO_RESP_DONE             (0x04u)
/** Bits that are used to check whether data can be read from the card       */
#define CSL_SDIO_READ_READY          (0x0400u)
/** Bits that are used to check whether data can be written to the card      */
#define CSL_SDIO_WRITE_READY         (0x0200u)

/** RCA value to deselect the SDIO card                                      */
#define CSL_SDIO_CARD_DESELECT_RCA   (0)

/** Macro to compare the NULL values                                         */
#define CSL_SDIO_NULL                (0)

/** Recommended value for command 0                                          */
#define CSL_SDIO_CMD_0              (0x0000u)
/** Recommended value for command 2                                          */
#define CSL_SDIO_CMD_2              (0x0402u)
/** Recommended value for command 3                                          */
#define CSL_SDIO_CMD_3              (0x0203u)
/** Recommended value for command 5                                          */
#define CSL_SDIO_CMD_5              (0x0605u)
/** Recommended value for command 7                                          */
#define CSL_SDIO_CMD_7              (0x0307u)
/** Recommended value for command 8                                          */
#define CSL_SDIO_CMD_8              (0x0208u)
/** Recommended value for command 41                                         */
#define CSL_SDIO_CMD_41             (0x0629u)
/** Recommended value for command 55                                         */
#define CSL_SDIO_CMD_55             (0x0237u)
/** Recommended value for command 53 for read operation                      */
#define CSL_SDIO_CMD_53_READ        (0x2235u)
/** Recommended value for command 53 for write operation                     */
#define CSL_SDIO_CMD_53_WRITE       (0x2A35u)
/** Recommended value for command 52                                         */
#define CSL_SDIO_CMD_52             (0x0234u)

/** Argument value for SDIO command 8                                        */
#define CSL_SDIO_CMD_8_ARG          (0x1AAu)
/** Argument value for SDIO command 41 for standard capacity cards           */
#define CSL_SDIO_CMD_41_ARG         (0x00ff8000)
/** Argument value for SDIO command 41 for high capacity cards               */
#define CSL_SDIO_CMD_41_HCS_ARG     (0x40ff8000)

/** SDIO response for ACMD41                                                 */
#define CSL_SDIO_CMD41_RESP         (0x80FF8000u)
/** SDIO response for SDHC card                                              */
#define CSL_SDIO_SDHC_RESP          (0x40000000u)

/** Error code to indicate IO portion of the SDIO card is not ready          */
#define CSL_ESDIO_IO_NOTREADY       (0x101u)
/** Error code to indicate Memory portion of the SDIO card is not ready      */
#define CSL_ESDIO_MEM_NOTREADY      (0x102u)

/** SDIO read FIFO level in bytes                                            */
#define CSL_SDIO_READ_FIFO_LEVEL    (32u)
/** SDIO write FIFO level in bytes                                           */
#define CSL_SDIO_WRITE_FIFO_LEVEL   (32u)

/** Macro to shift 16 bit word                                               */
#define CSL_SDIO_SHIFT_WORD         (16u)
/** Macro to mask 16 bit word                                                */
#define CSL_SDIO_MASK_WORD          (0xFFFFu)

/** Macro to shift a byte                                                    */
#define CSL_SDIO_SHIFT_BYTE         (8u)
/** Macro to mask a byte                                                     */
#define CSL_SDIO_MASK_BYTE          (0xFFu)

/** Mask value for mmc interrupt enable register                             */
#define CSL_SDIO_MMCIM_REG_MASK     (0x1FFFu)

/** Maximum number of bytes that can be read using CMD53 in byte mode        */
#define CSL_SDIO_MAX_BYTE_COUNT     (512u)
/** Maximum number of IO functions that are supported by SDIO specification  */
#define CSL_SDIO_MAX_FUNCTION_COUNT (7u)
/** Maximum size of an data transfer block supported by SDIO specification   */
#define CSL_SDIO_MAX_BLOCK_SIZE     (2048u)
/** Maximum number of an data blocks supported by SDIO specification         */
#define CSL_SDIO_MAX_BLOCK_COUNT    (511u)

/** SDIO maximum clock rate value                                            */
#define CSL_SDIO_MAX_CLKRATE        (0xFFu)

/** Macro to perform a left shift operation on a value                       */
#define CSL_SDIO_LSHIFT(val, shift)    ((Uint32)val << shift)
/** Macro to perform a right shift operation on a value                      */
#define CSL_SDIO_RSHIFT(val, shift)    (val >> shift)

/** Macro to set bit position for register address field in CMD52 and CMD53  */
#define CSL_SDIO_REGADDR(regAddr)    CSL_SDIO_LSHIFT(regAddr, 9)
/** Macro to set bit position for read after write flag in CMD52             */
#define CSL_SDIO_RAWFLAG(rawFlag)    CSL_SDIO_LSHIFT(rawFlag, 27)
/** Macro to set bit position for function number field in CMD52 and CMD53   */
#define CSL_SDIO_FUNNUM(funNum)      CSL_SDIO_LSHIFT(funNum, 28)
/** Macro to set bit position for read/write flag in CMD52 and CMD53         */
#define CSL_SDIO_RWFLAG(rwFlag)      CSL_SDIO_LSHIFT(rwFlag, 31)
/** Macro to set bit position for opcode field in CMD53                      */
#define CSL_SDIO_OPCODE(opCode)      CSL_SDIO_LSHIFT(opCode, 26)
/** Macro to set bit position for block mode field in CMD53                  */
#define CSL_SDIO_BLKMODE(blkMode)    CSL_SDIO_LSHIFT(blkMode, 27)

/** Macro to make 32 bit argument for CMD52 for write operation              */
#define CSL_SDIO_MAKE_CMD52_WRITE_ARG(wrData, regAddr, rawFlag, funNum, rwFlag)\
                               (wrData                    |      \
                                CSL_SDIO_REGADDR(regAddr) |      \
                                CSL_SDIO_RAWFLAG(rawFlag) |      \
                                CSL_SDIO_FUNNUM(funNum)   |      \
                                CSL_SDIO_RWFLAG(rwFlag))

/** Macro to make 32 bit argument for CMD52 for read operation               */
#define CSL_SDIO_MAKE_CMD52_READ_ARG(regAddr, rawFlag, funNum, rwFlag) \
                                (CSL_SDIO_REGADDR(regAddr) |     \
                                 CSL_SDIO_RAWFLAG(rawFlag) |     \
                                 CSL_SDIO_FUNNUM(funNum)   |     \
                                 CSL_SDIO_RWFLAG(rwFlag))

/** Macro to make 32 bit argument for CMD53                                  */
#define CSL_SDIO_MAKE_CMD53_ARG(count, regAddr, opCode, blkMode, funNum, rwFlag)\
                               (count                     |      \
                                CSL_SDIO_REGADDR(regAddr) |      \
                                CSL_SDIO_OPCODE(opCode)   |      \
                                CSL_SDIO_BLKMODE(blkMode) |      \
                                CSL_SDIO_FUNNUM(funNum)   |      \
                                CSL_SDIO_RWFLAG(rwFlag))

/** Macro to read ocr value from CMD5 response                               */
#define CSL_SDIO_GET_OCR(resp)       (resp & 0xFFFFFF)
/** Macro to read mem field value from CMD5 response                         */
#define CSL_SDIO_GET_MEM(resp)       (CSL_SDIO_RSHIFT(resp, 27) & 0x01)
/** Macro to read function count field value from CMD5 response              */
#define CSL_SDIO_GET_FUNCNT(resp)    (CSL_SDIO_RSHIFT(resp, 28) & 0x07)
/** Macro to read c field value from CMD5 response                           */
#define CSL_SDIO_GET_C(resp)         (CSL_SDIO_RSHIFT(resp, 31) & 0x01)

/**
@} */

/*****************************************************************************\
          SDIO global typedef declarations
\*****************************************************************************/

/**\addtogroup CSL_SDIO_ENUM
@{*/

/**
 *  \brief  Hardware Instance Id to select SDIO controller registers
 */
typedef enum {
    CSL_SDIO_INST0,         /**< SDIO instance number 0                      */
    CSL_SDIO_INST1,         /**< SDIO instance number 1                      */
    CSL_SDIO_INST_INV
} CSL_SdioInstId;

/**
 *  \brief  Response type from SDIO card
 *
 *  Depending on the command value sent to the card, the card responds with
 *  different response values.The type of response is defined in the structure.
 */
typedef enum {
    CSL_SDIO_RESPTYPE_NO,     /**< Command response is None                  */
    CSL_SDIO_RESPTYPE_R1,     /**< Command response is R1                    */
    CSL_SDIO_RESPTYPE_R2,     /**< Command response is R2                    */
    CSL_SDIO_RESPTYPE_R3,     /**< Command response is R3                    */
    CSL_SDIO_RESPTYPE_R4,     /**< Command response is R4                    */
    CSL_SDIO_RESPTYPE_R5,     /**< Command response is R5                    */
    CSL_SDIO_RESPTYPE_R6      /**< Command response is R6                    */
} CSL_SdioRespType;

/**
 *  \brief  Response busy signal from the card.
 */
typedef enum {
    CSL_SDIO_RESPBUSY_NO,  /**< No busy signal during the response           */
    CSL_SDIO_RESPBUSY_YES  /**< Busy signal during the response              */
} CSL_SdioRespBusy;

/**
 *  \brief  Type of card that is inserted
 *
 */
typedef enum
{
    CSL_SDIO_CARD_NONE = 0,
    /**< No card detected                                                    */
    CSL_SDIO_IO_CARD,
    /**< SD IO only card is detected - No memory                             */
    CSL_SDIO_COMBO_CARD,
    /**< SDIO combo card is detected - both IO and memory                    */
    CSL_SDIO_INVALID_CARD
    /**< SDIO card inserted is as invalid/unusable card                      */
} CSL_SdioCardType;

/**
 *  \brief  SDIO event responses
 *
 */
typedef enum {
    CSL_SDIO_EVENT_EOFCMD              = (1U << 2U),
    /**< for commands with response, an end of
     * Command + Response; for commands without response,
     * an end of Command                                                     */
    CSL_SDIO_EVENT_READ                = (1U << 10U),
    /**< data available with controller for reading                          */
    CSL_SDIO_EVENT_WRITE               = (1U << 9U),
    /**< data required by controller for writing                             */
    CSL_SDIO_EVENT_ERROR_CMDCRC        = (1U << 7U),
    /**< Error detected in the CRC during command
         response phase                                                      */
    CSL_SDIO_EVENT_ERROR_DATACRC       = ((1U << 6U)|(1 << 5U)),
    /**< Error detected in the CRC during data transfer                      */
    CSL_SDIO_EVENT_ERROR_CMDTIMEOUT    = ((1U) << (4U)),
    /**< Timeout detected during command - response phase                    */
    CSL_SDIO_EVENT_ERROR_DATATIMEOUT   = ((1U) << (3U)),
    /**< Timeout detected during data transfer                               */
    CSL_SDIO_EVENT_CARD_EXITBUSY       = ((1U) << (1U)),
    /**< Card has exited busy state                                          */
    CSL_SDIO_EVENT_BLOCK_XFERRED       = 1U
    /**< block transfer done                                                 */
} CSL_SdioCardRespEvent;

/**
 *\brief
 *   Enum variable for the selection of SDIO events.
 */
typedef enum
{
    CSL_SDIO_EDATDNE_INTERRUPT,
    /**< Data done (DATDNE) interrupt enable.                                */
    CSL_SDIO_EBSYDNE_INTERRUPT,
    /**< Busy done (BSYDNE) interrupt enable.                                */
    CSL_SDIO_ERSPDNE_INTERRUPT,
    /**< Command/response done (RSPDNE) interrupt enable.                    */
    CSL_SDIO_ETOUTRD_INTERRUPT,
    /**< Read-data time-out event (TOUTRD) interrupt enable.                 */
    CSL_SDIO_ETOUTRS_INTERRUPT,
    /**< Response time-out event (TOUTRS) interrupt enable.                  */
    CSL_SDIO_ECRCWR_INTERRUPT,
    /**< Write-data CRC error (CRCWR) interrupt enable.                      */
    CSL_SDIO_ECRCRD_INTERRUPT,
    /**< Read-data CRC error (CRCRD) interrupt enable.                       */
    CSL_SDIO_ECRCRS_INTERRUPT,
    /**< Response CRC error (CRCRS) interrupt enable.                        */
    CSL_SDIO_RSV_BIT,
    /**< this bit is reserved.                                               */
    CSL_SDIO_EDXRDY_INTERRUPT,
    /**< Data transmit register (MMCDXR) ready interrupt enable.             */
    CSL_SDIO_EDRRDY_INTERRUPT,
    /**< Data receive register ready (DRRDY) interrupt enable.               */
    CSL_SDIO_EDATED_INTERRUPT,
    /**< DAT3 edge detect (DATED) interrupt enable.                          */
    CSL_SDIO_ETRNDNE_INTERRUPT,
    /**< Transfer done (TRNDNE) interrupt enable.                            */
    CSL_SDIO_MMCIM_ALL_INTERRUPT,
    /**< All interrupts in MMCIM register                                    */
    CSL_SDIO_READWAIT_INTERRUPT,
    /**< SDIO read wait interrupt                                            */
    CSL_SDIO_CARD_INTERRUPT,
    /**< SDIO card interrupt                                                 */
    CSL_SDIO_SDIOIEN_ALL_INTERRUPT
    /**< All interrupts in SDIOIEN register                                  */
} CSL_SdioEventType;

/**
 * \brief
 *   Enum variable for the selection of SDIO interrupt status.
 */
typedef enum
{
    CSL_SDIO_READWAIT_INTR_ENABLE,
    /**< Enable SDIO read wait interrupt                                     */
    CSL_SDIO_READWAIT_INTR_DISABLE,
    /**< Disable SDIO read wait interrupt                                    */
    CSL_SDIO_CARD_INTR_ENABLE,
    /**< Enable SDIO card interrupt                                          */
    CSL_SDIO_CARD_INTR_DISABLE,
    /**< Disable SDIO card interrupt                                         */
    CSL_SDIO_ALL_INTR_ENABLE,
    /**< Enable both read wit and SDIO card interrupts                       */
    CSL_SDIO_ALL_INTR_DISABLE
    /**< Disable both read wit and SDIO card interrupts                      */
} CSL_SdioIntrStatus;

/**
 *  \brief  SDIO endianness
 *
 *  This enum holds the values to set endianness of the SDIO controller
 */
typedef enum {
    CSL_SDIO_ENDIAN_LITTLE = 0U, /**< Little endian mode configuration       */
    CSL_SDIO_ENDIAN_BIG    = 1U  /**< Big endian mode configuration          */
} CSL_SdioEndianMode;

/**
 *  \brief  Error type as received by the SDIO controller on response for a command
 */
#define CSL_SDIO_EVENT_ERROR   (CSL_SDIO_EVENT_ERROR_CMDCRC     |    \
                                CSL_SDIO_EVENT_ERROR_DATACRC    |    \
                                CSL_SDIO_EVENT_ERROR_CMDTIMEOUT |    \
                                CSL_SDIO_EVENT_ERROR_DATATIMEOUT)

/**
 *  \brief  Error type for command response timeout and CRC error
 */
#define CSL_SDIO_CMD_TOUT_CRC_ERROR   (CSL_SDIO_EVENT_ERROR_CMDCRC |     \
                                        CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)

/**
 *  \brief  Error type for data timeout and CRC error
 */
#define CSL_SDIO_DATA_TOUT_CRC_ERROR   (CSL_SDIO_EVENT_ERROR_DATACRC |    \
                                         CSL_SDIO_EVENT_ERROR_DATATIMEOUT)
/**
@} */

/**
\addtogroup CSL_SDIO_DATASTRUCT
@{*/

/**
 *  \brief  Response array received from the controller
 *
 *    This object holds the information the response that is received for a command
 *    sent to the card.
 */
typedef struct {
    Uint16    respBuf[8];
    /**< The response will interpreted as:
     * respBuf[0] - R2[15:0]
     * respBuf[1] - R2[31:16]
     * respBuf[2] - R2[47:32]
     * respBuf[3] - R2[63:48]
     * respBuf[4] - R2[79:64]
     * respBuf[5] - R2[95:80]
     * respBuf[6] - R2[111:96] or R1/R1B/R3/R4/R5/R6[23:8]
     * respBuf[7] - R2[127:112] or R1/R1B/R3/R4/R5/R6[39:24]
     * */
} CSL_SdioResponse;

/**
 * \brief SDIo config structure for setting SDIO Control register.
 *
 */
typedef struct {
  Uint16    mmcctl;     /**< MMCSD control register value                    */
  Uint16    mmcclk;     /**< MMCSD clock control register value              */
  Uint16    mmcim;      /**< Required interrupt field for MMCSD              */
  Uint16    mmctor;     /**< mmc response time out register                  */
  Uint16    mmctod;     /**< mmc data read time out register                 */
  Uint16    mmcblen;    /**< Number of bytes in a block of mmc or sd card    */
  Uint16    mmcnblk;    /**< Total no of blocks for read or write            */
  Uint16    mmcfifoctl; /**< FIFO control register                           */
  Uint16    sdioctl;    /**< SDIO control register                           */
  Uint16    sdioien;    /**< SDIO interrupt enable register                  */
} CSL_SdioConfig;

/**
 * \brief structure for card specific information.
 *
 */
typedef struct {
  Uint16              rca;         /**< User assigned relative card address   */
  Uint32              ocr;         /**< OCR value read from the card          */
  Uint8               funCount;    /**< Number of IO functions on the card    */
  Bool                cardReady;   /**< Flag to indicate card is ready or not */
  Bool                cardActive;  /**< Flag to indicate card is active or not*/
  Bool                sdHcDetected;/**< Flag to indicate high capacity card   */
  CSL_SdioCardType    sdioCardType;/**< SDIO card type                        */
} CSL_SdioCardObj;

/**
 * \brief structure for card specific information.
 *
 */
typedef struct {
   CSL_MmcsdRegsOvly    sdioRegs;       /**< Pointer to Hw regs structure    */
   CSL_SdioCardObj      *pSdioCardObj;  /**< Card Object structure pointer   */
   CSL_SdioEndianMode   readEndianMode; /**< Endian mode for read operation  */
   CSL_SdioEndianMode   writeEndianMode;/**< Endian mode for write operation */
   Bool                 cmd8Resp;       /**< Response status for CMD8        */
} CSL_SdioControllerObj;

/**
 * \brief SDIO Handle
 *
 */
typedef CSL_SdioControllerObj     *CSL_SdioHandle;

/**
@} */

/*****************************************************************************\
*  CSL SDIO function declarations                                             *
\*****************************************************************************/
/** @addtogroup CSL_SDIO_FUNCTION
 @{ */

/** ===========================================================================
 *   @n@b SDIO_init
 *
 *   @b Description
 *   @n This is the initialization function for the SDIO CSL. The function
 *      must be called before calling any other API from this CSL. This
 *      function is for enabling the clock to the SDIO Controller.
 *      This function should be called only once during system initialization
 *      time. This function gates the clock for the two MMCSD modules.
 *      Application programs may disable the clocks for MMCSD module which is
 *      not in use.
 *
 *    NOTE: Calling this function is not required if MMC_init is called since
 *    both functions use the same MMCSD controller
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK - Always returns
 *
 *   <b> Pre Condition </b>
 *   @n  This should be called before calling any other SDIO Function.
 *
 *   <b> Post Condition </b>
 *   @n  Enables clock for MMCSD controller
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_status       status;

            status = SDIO_init();
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_init (void);

/** ===========================================================================
 *   @n@b SDIO_open
 *
 *   @b Description
 *   @n This function opens the CSL SDIO module and returns handle to the
 *      SDIO object structure.
 *
 *      NOTE: THIS API SETS THE ENDIAN MODE OF SDIO READ AND WRITE OPERATIONS
 *            TO LITTLE ENDIAN. USE SDIO_setEndianMode() TO CHANGE ENDIAN MODE.
 *
 *   @b Arguments
 *   @verbatim
            pSdioContObj    SDIO Controller object structure
            instId          Instance number for SDIO
            status          Status of the function
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_SdioHandle
 *   @n                     Valid SDIO handle will be returned if
 *                          status value is equal to CSL_SOK.
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_init() API should be called before SDIO_open().
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is returned in the status variable. If status
 *              returned is
 *   @li            CSL_SOK            - Valid SDIO handle is returned
 *   @li            CSL_ESYS_INVPARAMS - Resource is already in use
 *   @n   2.    CSL_SdioObj object structure is populated
 *
 *   @b Modifies
 *   @n    1. Status variable
 *   @n    2. CSL_SdioObj object structure
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio = SDIO_open(&sdioContObj, instId, &status);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_SdioHandle SDIO_open (CSL_SdioControllerObj    *pSdioContObj,
                          CSL_SdioInstId           instId,
                          CSL_Status               *status);

/** ===========================================================================
 *   @n@b SDIO_close
 *
 *   @b Description
 *   @n This function closes the specified handle to SDIO.
 *
 *   @b Arguments
 *   @verbatim
            hSdio            Handle to the SDIO
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully before this function.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n SDIO handle
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_close(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_close(CSL_SdioHandle    hSdio);

/** ===========================================================================
 *   @n@b SDIO_config
 *
 *   @b Description
 *   @n This function configures the SDIO controller hardware registers.
 *
 *   @b Arguments
 *   @verbatim
            hSdio              Handle to the SDIO.
            pSdioConfig        Pointer to the SDIO config Parameter.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called before this function.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n SDIO Registers.
 *
 *   @b Example
 *   @verbatim

            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            CSL_SdioConfig           sdioConfig;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            // Initialize SDIO configuration structure
            sdioConfig.mmcctl  = .... ;
            ...
            ...
            sdioConfig.sdioien = .... ;


            // Configure SDIO
            status = SDIO_config(hSdio, &sdioConfig);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_config(CSL_SdioHandle     hSdio,
                       CSL_SdioConfig     *pSdioConfig);

/** ===========================================================================
 *   @n@b SDIO_detectCard
 *
 *   @b Description
 *   @n This function is used to detect type of the SDIO card inserted. This
 *      function sends SDIO specification defined command sequence to detect
 *      type of the card. Type of the card shall be populated in pSdioCardObj.
 *      This function also populates other fields of card object structure.
 *      Address pointed by 'pSdioCardObj' should be global structure variable
 *      which persists till end of the program since it is stored by SDIO CSL
 *      for reference in other APIs. This API returns success only incase of
 *      SDIO card inserted into the MMC/SD slot of the EVM. For the SDIO cards
 *      which returns OCR value as 0, card type will be set to
 *      'CSL_SDIO_INVALID_CARD'. Application/Driver programs should validate
 *      card type and ocr value updated in the card object structure to
 *      proceed further.
 *
 *   @b Arguments
 *   @verbatim
            hSdio              Handle to the SDIO.
            pSdioCardObj       Pointer to SDIO card object structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESYS_FAIL       - Card detection failed
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called before this function.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n pSdioCardObj
 *
 *   @b Example
 *   @verbatim

            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            CSL_SdioCardObj          sdioCardObj;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_detectCard(CSL_SdioHandle      hSdio,
                           CSL_SdioCardObj     *pSdioCardObj);

/** ===========================================================================
 *   @n@b SDIO_initCard
 *
 *   @b Description
 *   @n This function is used to initialize the SDIO card inserted. This
 *      function sends SDIO specification defined command sequence to initialize
 *      the SDIO card. Proper OCR value should be sent to this function based
 *      on the OCR value received from the card during card detection.
 *      SDIO card shall be ready to use after successful execution of this
 *      function. This function also supports initializing memory portion of
 *      the COMBO SDIO card. Parameter 'initMem' is useful to select(initMem = 1)
 *      or skip(initMem = 0) the initialization of COMBO SDIO cards.
 *      This function returns with error incase of IO portion or memory(COMBO)
 *      portion of the card is not ready.
 *
 *   @b Arguments
 *   @verbatim
            hSdio        Handle to the SDIO.
            OpCondReg    Operation Conditions Regsiter(OCR) value
            initMem      Flag to select/skip memory initialization
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK                - successful
 *   @li                    CSL_ESYS_BADHANDLE     - Invalid handle
 *   @li                    CSL_ESDIO_TIMEOUT      - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR       - Response/Data CRC Error
 *   @li                    CSL_ESDIO_IO_NOTREADY  - Card IO is not ready
 *   @li                    CSL_ESDIO_MEM_NOTREADY - Card memory is not ready
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_detectCard() API should be called before this function.
 *
 *   <b> Post Condition </b>
 *   @n  Initializes SDIO card
 *
 *   @b Modifies
 *   @n SDIO card registers
 *
 *   @b Example
 *   @verbatim

            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            CSL_SdioCardObj          sdioCardObj;
            Uint32                   ocr;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_initCard(CSL_SdioHandle    hSdio,
                         Uint32            OpCondReg,
                         Bool              initMem);

/** ===========================================================================
 *   @n@b SDIO_resetCard
 *
 *   @b Description
 *   @n This function is used to reset the SDIO card. This function uses CMD52
 *      to reset SDIO card. After execution of this function SDIO card will
 *      not receive any commands except CMD5.
 *
 *   @b Arguments
 *   @verbatim
            hSdio              Handle to the SDIO.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called before this function.
 *
 *   <b> Post Condition </b>
 *   @n  Resets SDIO card
 *
 *   @b Modifies
 *   @n SDIO card registers
 *
 *   @b Example
 *   @verbatim

            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_resetCard(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_resetCard(CSL_SdioHandle    hSdio);

/** ===========================================================================
 *   @n@b SDIO_getRca
 *
 *   @b Description
 *   @n This function is used to read new relative card address from the card.
 *      This function sends CMD3 and reads relative card address. RCA read from
 *      the card will be populated to the pRCardAddr and card object structure
 *      in SDIO handle.
 *
 *   @b Arguments
 *   @verbatim
            hSdio            Handle to the SDIO.
            pRCardAddr       Relative card address.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n pRCardAddr variable will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint16                   rca;
            Uint32                   ocr;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            status = SDIO_getRca(hSdio, &rca);

     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_getRca(CSL_SdioHandle     hSdio,
                       Uint16             *pRCardAddr);

/** ===========================================================================
 *   @n@b SDIO_deselectCard
 *
 *   @b Description
 *   @n This function is used to deselect SDIO which has been selected earlier.
 *      CMD7 shall be sent to the SDIO card with the relative card address set
 *      to 0x0000.
 *
 *   @b Arguments
 *   @verbatim
            hSdio      Handle to the SDIO.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called.
 *   @n  3:Card should be active
 *
 *   <b> Post Condition </b>
 *   @n  Deselects the SDIO card
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            CSL_SdioCardObj          sdioCardObj;
            Uint32                   ocr;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            status = SDIO_deselectCard(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_deselectCard(CSL_SdioHandle    hSdio);

/** ===========================================================================
 *   @n@b SDIO_selectCard
 *
 *   @b Description
 *   @n This function is used to select SDIO card. CMD7 shall be sent to the
 *      SDIO card along with the relative card address to select the card.
 *      RCA value should be the one published by the card. Sending CMD7
 *      with invalid RCA will put the card into standby/disabled state.
 *
 *   @b Arguments
 *   @verbatim
            hSdio      Handle to the SDIO.
            rca        Relative card address.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called.
 *
 *   <b> Post Condition </b>
 *   @n  Selects the SDIO card
 *
 *   @b Modifies
 *   @n rca
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint16                   rca;
            Uint32                   ocr;
            CSL_SdioCardObj          sdioCardObj;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            rca = sdioCardObj.rca;

            status = SDIO_initCard(hSdio, ocr, TRUE);

            status = SDIO_selectCard(hSdio, rca);

     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_selectCard(CSL_SdioHandle    hSdio,
                           Uint16            rca);

/** ===========================================================================
 *   @n@b SDIO_sendCommand
 *
 *   @b Description
 *   @n This function is used to send commands to the SDIO card. This function
 *      sends given command to the card and waits for the response.
 *      Lower 16 bits of the parameter 'cmd' should contain the command that
 *      needs to be written to MMCCMD1 register. Upper 16 bits of the 'cmd'
 *      should contain the data that needs to be written to MMCCMD2 register.
 *
 *      NOTE:SETTING THE MMCCMD2 VALUE TO '1' IS ALLOWED ONLY FOR DATA WRITE
 *      COMMANDS THAT USES DMA FOR DATA TRANSFER. FOR OTHER COMMANDS UPPER
 *      16 BITS OF 'CMD' SHOULD ALWAYS BE ZERO.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            cmd            MMCCMD1 and MMCCMD2 register value.
            arg            Command argument.
            respTimeout    Timeout value for the response.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Sends command to the card
 *
 *   @b Modifies
 *   @n Card Command registers
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   cmd;
            Uint32                   arg;
            Uint16                   respTimeout;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            //Send Command 5
            cmd         = 0x205;
            arg         = 0;
            respTimeout = 0xFFFF;
            status = SDIO_sendCommand(hSdio, cmd, arg, respTimeout);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_sendCommand(CSL_SdioHandle    hSdio,
                            Uint32            cmd,
                            Uint32            arg,
                            Uint16            respTimeout);

/** ===========================================================================
 *   @n@b SDIO_getResponse
 *
 *   @b Description
 *   @n This function is used to read the response registers. This function
 *      reads response registers based on the response type and populates the
 *      response structure. Response registers are not cleared by this function.
 *
 *\n    Response type     Response bits       Resp buf Fields populated
 *\n    ==============    ============       =========================
 *\n    R1,R3,R4,R5,R6      [23:8]             pResponse->respBuf[6]
 *\n                        [39:24]            pResponse->respBuf[7]
 *\n
 *\n        R2              [15:0]             pResponse->respBuf[0]
 *\n                        [31:16]            pResponse->respBuf[1]
 *\n                        [47:32]            pResponse->respBuf[2]
 *\n                        [63:48]            pResponse->respBuf[3]
 *\n                        [79:64]            pResponse->respBuf[4]
 *\n                        [95:80]            pResponse->respBuf[5]
 *\n                        [111:96]           pResponse->respBuf[6]
 *\n                        [127:112]          pResponse->respBuf[7]
 *
 *
 * NOTE: IF RESPONSE TYPE SENT IS NOT RECOGNIZED BY THIS API ALL THE RESPONSE
 * REGISTERS ARE POPULATED IN THE SAME FORMAT MENTIONED ABOVE FOR R2 RESPONSE.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            respType       Type of the response
            pResponse      Pointer to response structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_sendCommand() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n Response structure
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   cmd;
            Uint32                   arg;
            Uint16                   respTimeout;
            CSL_SdioResponse         response;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            //Send Command 5
            cmd         = 0x205;
            arg         = 0;
            respTimeout = 0xFFFF;
            status = SDIO_sendCommand(hSdio, cmd, arg, respTimeout);

            status = SDIO_getResponse(hSdio, CSL_SDIO_RESPTYPE_R4, &response);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_getResponse(CSL_SdioHandle      hSdio,
                            CSL_SdioRespType    respType,
                            CSL_SdioResponse    *pResponse);

/** ===========================================================================
 *   @n@b SDIO_clearResponse
 *
 *   @b Description
 *   @n This function is used to clear all the response registers.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n Response structure
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            CSL_SdioResponse         response;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_clearResponse(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_clearResponse(CSL_SdioHandle      hSdio);

/** ===========================================================================
 *   @n@b SDIO_getStatus
 *
 *   @b Description
 *   @n This function reads MMCST0 and MMCST1 registers and returns the value.
 *      When mask value is set to 0xFFFFFFFF, value of MMCST0 will be stored
 *      in lower 16 bits and value of MMCST1 will be store in upper 16 bits.
 *      Value of a particular bit in the passed mask should be set to '1' to
 *      read the corresponding bit in the MMC status register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            maskValue      Mask value to specify the field.
            pStatus        Status of API Success or Failed.
     @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *                          Status  - MMCST0 and MMCST1 register value.
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   mmcstatus;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            mmcstatus = SDIO_getStatus(hSdio, 0xFFFF, &status);
     @endverbatim
 *  ===========================================================================
 */
Uint32 SDIO_getStatus(CSL_SdioHandle    hSdio,
                      Uint32            maskValue,
                      CSL_Status        *pStatus);

/** ===========================================================================
 *   @n@b SDIO_getSdioStatus
 *
 *   @b Description
 *   @n This function reads SDIOST0 and SDIOIST registers and returns the value.
 *      When mask value is set to 0xFFFFFFFF, value of SDIOST0 will be stored
 *      in lower 16 bits and value of SDIOIST will be store in upper 16 bits.
 *      Value of a particular bit in the passed mask should be set to '1' to
 *      read the corresponding bit in the SDIO status register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            maskValue      Mask value to specify the field.
            pStatus        status of API Success of Failed.
     @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *                          Status  - SDIOST0 and SDIOIST register value.
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   sdiostatus;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            sdiostatus = SDIO_getSdioStatus(hSdio, 0xFFFF, &status);
     @endverbatim
 *  ===========================================================================
 */
Uint32 SDIO_getSdioStatus(CSL_SdioHandle    hSdio,
                          Uint32            maskValue,
                          CSL_Status        *pStatus);

/** ===========================================================================
 *   @n@b SDIO_eventEnable
 *
 *   @b Description
 *   @n This function will enable the events in the MMC interrupt mask register
 *      and SDIO interrupt enable register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio         Handle to the SDIO.
            sdioEvent     Event type.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Enables the given event
 *
 *   @b Modifies
 *   @n MMC/SDIO Interrupt Mask Registers
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_eventEnable(hSdio, CSL_SDIO_EDRRDY_INTERRUPT);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_eventEnable(CSL_SdioHandle       hSdio,
                            CSL_SdioEventType    sdioEvent);

/** ===========================================================================
 *   @n@b SDIO_eventDisable
 *
 *   @b Description
 *   @n This function will disable the events in the MMC interrupt mask
 *      register and SDIO interrupt enable register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio         Handle to the SDIO.
            sdioEvent     Event type.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Disables the given event
 *
 *   @b Modifies
 *   @n MMC/SDIO Interrupt Mask Register
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_eventDisable(hSdio, CSL_SDIO_EDRRDY_INTERRUPT);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_eventDisable(CSL_SdioHandle       hSdio,
                             CSL_SdioEventType    sdioEvent);

/** ===========================================================================
 *   @n@b SDIO_readWaitEnable
 *
 *   @b Description
 *   @n This function is used to enable SDIO read-wait bit in SDIO control
 *      register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio             Handle to the SDIO.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Enables the sdio read-wait
 *
 *   @b Modifies
 *   @n SDIO control Register
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_readWaitEnable(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_readWaitEnable(CSL_SdioHandle    hSdio);

/** ===========================================================================
 *   @n@b SDIO_readWaitDisable
 *
 *   @b Description
 *   @n This function is used to disable SDIO read-wait bit in SDIO control
 *      register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio             Handle to the SDIO.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Disables the sdio read-wait
 *
 *   @b Modifies
 *   @n SDIO control Register
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_readWaitDisable(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_readWaitDisable(CSL_SdioHandle    hSdio);

/** ===========================================================================
 *   @n@b SDIO_readSingleByte
 *
 *   @b Description
 *   @n This function reads a single byte of data from the SDIO card.
 *      'readAddr' is address on the SDIO card from where data byte will be
 *      read. Command 52 (IO_RW_DIRECT Command) is used to read the data.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            readAddr       Location to read from.
            funNum         IO function number to read data
            pReadData      Pointer to populate data read
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Reads a byte of data from the SDIO card
 *
 *   @b Modifies
 *   @n Read data pointer will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint16                   rca;
            Uint32                   ocr;
            Uint8                    readData;
            Uint32                   readAddr;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            readAddr  = 0x0;
            funNum    = 0;

            status = SDIO_readSingleByte(hSdio, readAddr, funNum, &readData);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_readSingleByte(CSL_SdioHandle    hSdio,
                               Uint32            readAddr,
                               Uint8             funNum,
                               Uint8             *pReadData);

/** ===========================================================================
 *   @n@b SDIO_writeSingleByte
 *
 *   @b Description
 *   @n This function writes a single byte of data to the SDIO card.
 *      'writeAddr' is address on the SDIO card to where data byte will be
 *      written. Command 52 (IO_RW_DIRECT Command) is used to write the data.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            writeAddr      Location to write data byte.
            funNum         IO function number to read data
            writeData      Data to be written
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Writes a byte of data to the SDIO card
 *
 *   @b Modifies
 *   @n SDIO card registers
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint16                   rca;
            Uint32                   ocr;
            Uint8                    writeData;
            Uint32                   writeAddr;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            writeAddr  = 0x02;
            writeData  = 0x02;
            funNum     = 0;

            status = SDIO_writeSingleByte(hSdio, writeAddr, funNum, writeData);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_writeSingleByte(CSL_SdioHandle    hSdio,
                                Uint32            writeAddr,
                                Uint8             funNum,
                                Uint8             writeData);

/** ===========================================================================
 *   @n@b SDIO_readBytes
 *
 *   @b Description
 *   @n This function reads requested number of bytes from the SDIO card.
 *      'readAddr' is address on the SDIO card from where the reading
 *      starts. This function reads multiple bytes of data from the given
 *      address and populates into read buffer. Maximum number of bytes
 *      that can be read using this function are 512.
 *
 *      Reading of data can be done from a fixed address(opCode = 0) or
 *      incrementing address(opCode = 1). Fixed mode data read is useful
 *      when I/O data is transferred using a FIFO inside SDIO card. Opcode
 *      should be 1 for the SDIO cards which doesn't have support for FIFO.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'readEndianMode' in the SDIO handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during SDIO open. Endian mode can
 *      be configured using SDIO_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            readAddr       Location to read from.
            funNum         IO function number to read data
            opCode         Bit to decide source address mode
                             - Fixed/Incrementing
            noOfBytes      no of bytes to read.
            pReadBuf       pointer to a buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Reads data from the SDIO card
 *
 *   @b Modifies
 *   @n readBuffer will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint16                   rca;
            Uint32                   ocr;
            Uint16                   readBuf[512];
            Uint32                   readAddr;
            Uint16                   noOfBytes;
            Bool                     opCode;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            readAddr  = 0x0;
            noOfBytes = 16;
            opCode    = 1;
            funNum    = 0;

            status = SDIO_readBytes(hSdio, readAddr, funNum, opCode,
                                    noOfBytes, readBuf);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_readBytes(CSL_SdioHandle    hSdio,
                          Uint32            readAddr,
                          Uint8             funNum,
                          Bool              opCode,
                          Uint16            noOfBytes,
                          Uint16            *pReadBuf);

/** ===========================================================================
 *   @n@b SDIO_writeBytes
 *
 *   @b Description
 *   @n This function writes requested number of bytes to the SDIO card.
 *      'writeAddr' is address on the SDIO card from where the writing
 *      starts. Maximum length of the data that can be written using
 *      this function is 512 bytes.
 *
 *      Writing of data can be done to a fixed address(opCode = 0) or
 *      incrementing address(opCode = 1). Fixed mode data write is useful
 *      when I/O data is transferred using a FIFO inside SDIO card. Opcode
 *      should be 1 for the SDIO cards which doesn't have support for FIFO.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'writeEndianMode' in the SDIO handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during SDIO open. Endian mode can
 *      be configured using SDIO_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            writeAddr      Location to write to.
            funNum         IO function number to write data
            opCode         Bit to decide destination address mode
                             - Fixed/Incrementing
            noOfBytes      Number of bytes to write.
            pWriteBuf      Pointer to write buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Writes data to the SDIO card
 *
 *   @b Modifies
 *   @n Data registers
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   ocr;
            Uint16                   writeBuf[512];
            Uint32                   writeAddr;
            Uint16                   noOfBytes;
            Bool                     opCode;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            writeAddr  = 0x07;
            noOfBytes  = 1;
            opCode     = 1;
            funNum     = 0;

            status = SDIO_writeBytes(hSdio, writeAddr, funNum, opCode,
                                     noOfBytes, writeBuf);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_writeBytes(CSL_SdioHandle    hSdio,
                           Uint32            writeAddr,
                           Uint8             funNum,
                           Bool              opCode,
                           Uint16            noOfBytes,
                           Uint16            *pWriteBuf);

/** ===========================================================================
 *   @n@b SDIO_readBlocks
 *
 *   @b Description
 *   @n This function reads requested number of blocks of data from SDIO card.
 *      'readAddr' is address on the SDIO card from where the reading
 *      starts. Block size of IO function to read data should be configured
 *      before calling this function.
 *
 *      Reading of data can be done from a fixed address(opCode = 0) or
 *      incrementing address(opCode = 1). Fixed mode data read is useful
 *      when I/O data is transferred using a FIFO inside SDIO card. Opcode
 *      should be 1 for the SDIO cards which doesn't have support for FIFO.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'readEndianMode' in the SDIO handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during SDIO open. Endian mode can
 *      be configured using SDIO_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            readAddr       Location to read from.
            funNum         IO function number to read data
            opCode         Bit to decide source address mode
                           - Fixed/Incrementing
            noOfBlocks     Number of blocks to read.
            blockSize      Size of each block
            pReadBuf       pointer to a buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Reads data from the SDIO card
 *
 *   @b Modifies
 *   @n readBuffer will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   ocr;
            Uint16                   readBuf[512];
            Uint32                   readAddr;
            Uint16                   noOfBlocks;
            Bool                     opCode;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            readAddr   = 0x0;
            noOfBlocks = 2;
            opCode     = 1;
            funNum     = 0;

            status = SDIO_readBlocks(hSdio, readAddr, funNum, opCode,
                                     noOfBlocks, 256, readBuf);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_readBlocks(CSL_SdioHandle    hSdio,
                           Uint32            readAddr,
                           Uint8             funNum,
                           Bool              opCode,
                           Uint16            noOfBlocks,
                           Uint16            blockSize,
                           Uint16            *pReadBuf);

/** ===========================================================================
 *   @n@b SDIO_writeBlocks
 *
 *   @b Description
 *   @n This function writes requested number of blocks of data to SDIO card.
 *      'writeAddr' is address on the SDIO card from where the writing
 *       starts. Block size of IO function to write data should be configured
 *      before calling this function.
 *
 *      Writing of data can be done to a fixed address(opCode = 0) or
 *      incrementing address(opCode = 1). Fixed mode data write is useful
 *      when I/O data is transferred using a FIFO inside SDIO card. Opcode
 *      should be 1 for the SDIO cards which doesn't have support for FIFO.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'writeEndianMode' in the SDIO handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during SDIO open. Endian mode can
 *      be configured using SDIO_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            writeAddr      Location to write to.
            funNum         IO function number to write data
            opCode         Bit to decide destination address mode
                             - Fixed/Incrementing
            noOfBlocks     Number of blocks to write.
            blockSize      Size of each block
            pWriteBuf      Pointer to write buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Writes data to the SDIO card
 *
 *   @b Modifies
 *   @n Data registers
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   ocr;
            Uint16                   writeBuf[512];
            Uint32                   writeAddr;
            Uint16                   noOfBlocks;
            Bool                     opCode;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            writeAddr  = 0x110;
            noOfBlocks = 2;
            opCode     = 1;
            funNum     = 0;

            status = SDIO_writeBlocks(hSdio, writeAddr, funNum, opCode,
                                      noOfBlocks, 256, writeBuf);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_writeBlocks(CSL_SdioHandle    hSdio,
                            Uint32            writeAddr,
                            Uint8             funNum,
                            Bool              opCode,
                            Uint16            noOfBlocks,
                            Uint16            blockSize,
                            Uint16            *pWriteBuf);

/** ===========================================================================
 *   @n@b SDIO_setEndianMode
 *
 *   @b Description
 *   @n This function is used to set endian mode of the SDIO controller.
 *      Endian mode set using this API decides the way in which data is
 *      transferred from FIFO to storage device.
 *
 *      Default endianness of SDIO controller:
 *      Chip C5504 and C5505: LITTLE ENDIAN.
 *      Chip C5514 and C5515: BIG ENDIAN.
 *
 *   @b Arguments
 *   @verbatim
            hSdio              Handle to the SDIO.
            writeEndianMode    Endian mode for SDIO write operation
            readEndianMode     Endian mode for SDIO read operation
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n   Sets SDIO endian mode for write and read operations
 *
 *   @b Modifies
 *   @n SDIO control register
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_setEndianMode(hSdio, CSL_SDIO_ENDIAN_LITTLE,
                                        CSL_SDIO_ENDIAN_LITTLE);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_setEndianMode (CSL_SdioHandle        hSdio,
                               CSL_SdioEndianMode    writeEndianMode,
                               CSL_SdioEndianMode    readEndianMode);

/** ===========================================================================
 *   @n@b SDIO_setClock
 *
 *   @b Description
 *   @n This function is used to set SDIO controller clock divider value. Clock
 *      controller register bits are configured by this function. Clock rate or
 *      clock divider value can range from 0 to 0xFF.
 *
 *   @b Arguments
 *   @verbatim
            hSdio        Handle to the SDIO.
            clockRate    Value of clock rate
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets SDIO clock
 *
 *   @b Modifies
 *   @n SDIO clock control register
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_setClock(hSdio, 5);
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_setClock (CSL_SdioHandle    hSdio,
                          Uint16            clockRate);

/**
@} */

#ifdef __cplusplus
}
#endif
#endif /* _CSL_SDIO_H_ */

