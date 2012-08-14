/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_spi.h
 *
 *  @brief SPI functional layer API header file
 *
 *  Path: \(CSLPATH)/inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 22-Aug-2008 Created
 * ============================================================================
 */

/** @defgroup CSL_SPI_API SPI
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 * SPI stands for Serial periferal interface
 *
 */

#ifndef _CSL_SPI_H_
#define _CSL_SPI_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "soc.h"
#include "csl_error.h"
#include "csl_types.h"
#include "csl_general.h"

/**
@defgroup CSL_SPI_SYMBOL  SPI Symbols Defined
@ingroup CSL_SPI_API
*/

/**
@defgroup CSL_SPI_DATASTRUCT  SPI Data Structures
@ingroup CSL_SPI_API
*/

/**
@defgroup CSL_SPI_FUNCTION  SPI Functions
@ingroup CSL_SPI_API
*/

/**
@defgroup CSL_SPI_ENUM SPI Enumerated Data Types
@ingroup CSL_SPI_API
*/

/**
@defgroup CSL_SPI_GLOBALS SPI Global declarations
@ingroup CSL_SPI_API
*/

/*****************************************************************************\
*         I2S global macro declarations
\*****************************************************************************/
/**
@addtogroup CSL_SPI_SYMBOL
@{
*/
/** SPI Frame intrupt disable                                                */
#define    CSL_SPI_SPICR1_FIRQ_DISABLE            (Uint16)0
/** SPI Frame intrupt enable                                                */
#define    CSL_SPI_SPICR1_FIRQ_ENABLE            (Uint16)1
/** SPI read command                                                        */
#define    SPI_READ_CMD                        (Uint16)1
/** SPI write command                                                        */
#define    SPI_WRITE_CMD                        (Uint16)2
/** SPI bit disable                                                            */
#define    SPI_DISABLE                            (0)
/** SPI bit enable                                                            */
#define    SPI_ENABLE                            (1)
/** SPI module active                                                        */
#define    SPI_ACTIVE                            (0)
/**
@}*/

/**************************************************************************\
* I2S global enum declaration                                             *
\**************************************************************************/
/**
@addtogroup CSL_SPI_ENUM
@{*/


/**
 *  \brief SPI chip select.
 *
 *  SPI support four chip select.
 */
typedef enum
{
    SPI_CS_NUM_0,            /**< SPI device chip select 1                 */
    SPI_CS_NUM_1,            /**< SPI device chip select 2                 */
    SPI_CS_NUM_2,            /**< SPI device chip select 3                 */
    SPI_CS_NUM_3,            /**< SPI device chip select 4                 */
    SPI_CS_NUM_INVALID        /**< SPI device chip select Invalid          */
}SPI_HwMode;

/**
 *  \brief SPI word length.
 *
 *  SPI support maximum 32 bit word length.
 */

typedef enum
{
    SPI_WORD_LENGTH_1 = 0,        /**< SPI Word length 1                     */
    SPI_WORD_LENGTH_2,            /**< SPI Word length 2                     */
    SPI_WORD_LENGTH_3,            /**< SPI Word length 3                     */
    SPI_WORD_LENGTH_4,            /**< SPI Word length 4                     */
    SPI_WORD_LENGTH_5,            /**< SPI Word length 5                     */
    SPI_WORD_LENGTH_6,            /**< SPI Word length 6                     */
    SPI_WORD_LENGTH_7,            /**< SPI Word length 7                     */
    SPI_WORD_LENGTH_8,            /**< SPI Word length 8                     */
    SPI_WORD_LENGTH_9,            /**< SPI Word length 9                     */
    SPI_WORD_LENGTH_10,            /**< SPI Word length 10                     */
    SPI_WORD_LENGTH_11,            /**< SPI Word length 11                     */
    SPI_WORD_LENGTH_12,            /**< SPI Word length 12                     */
    SPI_WORD_LENGTH_13,            /**< SPI Word length 13                     */
    SPI_WORD_LENGTH_14,            /**< SPI Word length 14                     */
    SPI_WORD_LENGTH_15,            /**< SPI Word length 15                     */
    SPI_WORD_LENGTH_16,            /**< SPI Word length 16                     */
    SPI_WORD_LENGTH_17,            /**< SPI Word length 17                     */
    SPI_WORD_LENGTH_18,            /**< SPI Word length 18                     */
    SPI_WORD_LENGTH_19,            /**< SPI Word length 19                     */
    SPI_WORD_LENGTH_20,            /**< SPI Word length 20                     */
    SPI_WORD_LENGTH_21,            /**< SPI Word length 21                     */
    SPI_WORD_LENGTH_22,            /**< SPI Word length 22                     */
    SPI_WORD_LENGTH_23,            /**< SPI Word length 23                     */
    SPI_WORD_LENGTH_24,            /**< SPI Word length 24                     */
    SPI_WORD_LENGTH_25,            /**< SPI Word length 25                     */
    SPI_WORD_LENGTH_26,            /**< SPI Word length 26                     */
    SPI_WORD_LENGTH_27,            /**< SPI Word length 27                     */
    SPI_WORD_LENGTH_28,            /**< SPI Word length 28                     */
    SPI_WORD_LENGTH_29,            /**< SPI Word length 29                     */
    SPI_WORD_LENGTH_30,            /**< SPI Word length 30                     */
    SPI_WORD_LENGTH_31,            /**< SPI Word length 31                     */
    SPI_WORD_LENGTH_32,            /**< SPI Word length 32                     */
    SPI_MAX_WORD_LEN            /**< SPI Word length invalid             */
}SPI_WordLen;

/**
 *  \brief SPI word intrupt.
 *
 *  SPI word intrupt enable or disable.
 */
typedef enum
{
    SPI_WORD_IRQ_DISABLE,        /**< SPI Word intrupt disable             */
    SPI_WORD_IRQ_ENABLE            /**< SPI Word intrupt enable             */
}SPI_WordCntEnb;

/**
 *  \brief SPI frame intrupt.
 *
 *  SPI frame intrupt enable or disable.
 */
typedef enum
{
    SPI_FRAME_IRQ_DISABLE,        /**< SPI frame intrupt disable             */
    SPI_FRAME_IRQ_ENABLE        /**< SPI frame intrupt enable             */
}SPI_FrameCntEnb;

/**
 *  \brief SPI Command.
 *
 *  SPI data read and write command.
 */
typedef enum SPI_Command
{
    SPI_RSVD_CMD = 0,            /**< SPI reserve command                 */
    SPI_READ,                    /**< SPI read command                     */
    SPI_WRITE,                    /**< SPI write command                     */
    SPI_RSVD1_CMD                /**< SPI reserve1 command                 */
} SPI_Command;

/**
 *  \brief SPI data delay.
 *
 *  SPI supports four data delay.
 */
typedef enum
{
    SPI_DATA_DLY_0 = 0,            /**< SPI data delay 0                    */
    SPI_DATA_DLY_1,                /**< SPI data delay 1                    */
    SPI_DATA_DLY_2,                /**< SPI data delay 2                    */
    SPI_DATA_DLY_3,                /**< SPI data delay 3                    */
    SPI_DATA_DLY_UNKNOWN        /**< SPI data delay unknowen            */
} SPI_DataDly;

/**
 *  \brief SPI clock polarity.
 *
 *  SPI clock polarity high or low at idle mode.
 */
typedef enum
{
    SPI_CLKP_LOW_AT_IDLE = 0,    /**< SPI clock polarity low                */
    SPI_CLKP_HIGH_AT_IDLE        /**< SPI clock polarity high            */
}SPI_ClkPolarity;

/**
 *  \brief SPI chip select polarity.
 *
 *  SPI chip select polarity high or low at active mode.
 */
typedef enum
{
    SPI_CSP_ACTIVE_LOW = 0,        /**< SPI chip select polarity low        */
    SPI_CSP_ACTIVE_HIGH            /**< SPI chip select polarity high        */
} SPI_ChipSelPol;

/**
 *  \brief SPI clock phase.
 *
 *  SPI clock phase falling or rising.
 */
typedef enum
{
    SPI_CLK_PH_FALL_EDGE = 0,    /**< SPI clock phase falling            */
    SPI_CLK_PH_RISE_EDGE        /**< SPI clock phase resing                */
} SPI_ClkPh;

/**
 *  \brief SPI Operation mode.
 *
 *  SPI operation mode .
 */
typedef enum
{
    SPI_POLLING_MODE,    /**< SPI Polling mode                        */
    SPI_INTERRUPT_MODE        /**< SPI Intrrupt mode                */
}SPI_OperMode;
/**
@}*/


/**************************************************************************\
*               SPI global data structures                                *
\**************************************************************************/
/**
\addtogroup CSL_SPI_DATASTRUCT
@{*/


/**
 *   \brief Object structure.
 *
 *   object structure for handler
 */
typedef struct
{
    Uint16            configured;        /**< Check for device configuration        */
    SPI_HwMode        mode;            /**< Chip select mode                    */
    SPI_OperMode    opMode;            /**< operation mode                        */
}CSL_SpiObj;

/**
 *   \brief Configuration structure.
 *
 *   Hardware register configuration structure
 */

typedef struct
{
    Uint16                spiClkDiv;    /**< clock division (minimum value is 3)*/
    SPI_WordLen            wLen;        /**< Word length set for data transmission
                                    or receive (supporting 8 bit or 16 bit)    */
    Uint16                frLen;        /**<Frame lenghth to transmit or receive*/
    SPI_WordCntEnb        wcEnable;    /**< Word complete enable                */
    SPI_FrameCntEnb        fcEnable;    /**< Frame complete enable                */
    SPI_HwMode            csNum;        /**< Chip select ie.0...3                */
    SPI_DataDly            dataDelay;    /**< Data delay                            */
    SPI_ClkPolarity        clkPol;        /**< Clock plarity                        */
    SPI_ChipSelPol        csPol;        /**< chip select polarity                */
    SPI_ClkPh            clkPh;        /**< Clock phase                        */
}SPI_Config;

/**
 *   \brief Object Handler.
 *
 *   Pointer to the object structure
 */
typedef CSL_SpiObj *CSL_SpiHandle;
/**
@}*/




/*****************************************************************************\
 * SPI function declarations                                                  *
\*****************************************************************************/

/** @addtogroup CSL_SPI_FUNCTION
@{*/

/** ===========================================================================
 *   @n@b SPI_init
 *
 *   @b Description
 *   @n This function intialize and activate the SPI module
 *
 *   @b Arguments
 *   @verbatim
            None
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_SOK
 *   @n                     Return value CSL_SOK for success
 *
 *   <b> Pre Condition </b>
 *   @n  This functionality has to call before any function call.
 *
 *   <b> Post Condition </b>
 *   @n   None
 *
 *   @b Modifies
 *   @n    None
 *
 *   @b Example
 *   @verbatim

            void

            result = SPI_init();
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_init(void);

/** ===========================================================================
 *   @n@b SPI_deInit
 *
 *   @b Description
 *   @n This function unintialize and activate the SPI module
 *
 *   @b Arguments
 *   @verbatim
            void    No parameter
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_SOK
 *   @n                     Return value CSL_SOK for success
 *
 *   <b> Pre Condition </b>
 *   @n  This function call call after SPI_init function.
 *
 *   <b> Post Condition </b>
 *   @n   SPI_close function can be call after this function call.
 *
 *   @b Modifies
 *   @n    1. Set the register to activate SPI module
 *   @n    2.
 *
 *   @b Example
 *   @verbatim

            void

            result = SPI_deInit();
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_deInit(void);

/** ===========================================================================
 *   @n@b SPI_open
 *
 *   @b Description
 *   @n This function returns the handle to the SPI
 *      instance. This handle is passed to all other CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            Mode        chip select mode.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_SpiHandle
 *   @n                     Valid SPI handle will be returned if
 *                          handler value is not equal to NULL.
 *
 *   <b> Pre Condition </b>
 *   @n   SPI_init function has to call before call this function.
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is returned in the status variable. If status
 *              returned is
 *   @li            NULL               - InValid SPI handle is returned
 *   @n   2.
 *
 *   @b Modifies
 *   @n    1. CSL_SpiObj     SPI object structure
 *   @n    2.
 *
 *   @b Example
 *   @verbatim

            SPI_HwMode         csNum;
            SPI_OperMode     opMode(Polling/Interrupt)
            ...

            hSpi = SPI_open(SPI_HwMode    csNum, SPI_OperMode opMode);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_SpiHandle SPI_open(SPI_HwMode    csNum, SPI_OperMode opMode);

/** ===========================================================================
 *   @n@b SPI_close
 *
 *   @b Description
 *   @n This function closes the specified handle to SPI.
 *
 *   @b Arguments
 *   @verbatim
            spiHandle          Handle to the spi
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  SPI_open() function has to call before call this function.
 *
 *   <b> Post Condition </b>
 *   @n  There should not be any function call after this function call.
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_SpiHandle   hSpi;
            CSL_status   status;

            status = SPI_close(hSpi);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_close(CSL_SpiHandle hSpi);

/** ===========================================================================
 *   @n@b SPI_config
 *
 *   @b Description
 *   @n It configures the SPI Controller registers of particular handle as per
 *      the values passed in the SPI config structure.
 *
 *   @b Arguments
 *   @verbatim
            spiHandle       Handle to the spi.
            spiHwConfig     Pointer to spi Config structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Congig Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  SPI_open function should call before call this function.
 *
 *   <b> Post Condition </b>
 *   @n  SPI_close function can call after this function call.
 *
 *   @b Modifies
 *   @n 1. SPI registers will be updated as per config parameter
 *   @n 2. CSL_SpiObj Object will be updated to store some info
 *              as passed config parameter.
 *
 *   @b Example
 *   @verbatim
            CSL_SpiHandle   hSpi;
            SPI_Config      spiHwConfig;
            Int16           status;

            status = SPI_config(hSpi, &spiHwConfig);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_config   (CSL_SpiHandle hSpi, SPI_Config     *spiHwConfig);

/** ===========================================================================
 *   @n@b SPI_read
 *
 *   @b Description
 *   @n This function Read data form specified device.
 *
 *   @b Arguments
 *   @verbatim
            hSpi              Pointer to handler
            readBuff          Pointer to the read buffer.
            buffLen           Maximum read buffer size.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Read successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  This function can call by SPI_dataTransaction function.
 *
 *   <b> Post Condition </b>
 *   @n   SPI_deInit can be call after this function call.
 *
 *   @b Modifies
 *   @n
 *
 *   @b Example
 *   @verbatim
            CSL_SpiHandle     hSpi
               Uint16          readBuff[size];
            Uint16          buffLen;
            CSL_status   status;

            status = SPI_read(hSpi,&readBuff, buffLen);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_read    (CSL_SpiHandle hSpi, Uint16    *readBuffer, Uint16    bufLen);

/** ===========================================================================
 *   @n@b SPI_write
 *
 *   @b Description
 *   @n This function Write data to specified device.
 *
 *   @b Arguments
 *   @verbatim
             hSpi              Pointer to handler
            writeBuff         Pointer to the write buffer.
            buffLen           Maximum read buffer size.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Write successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *       This function can call by SPI_dataTransaction function.
 *
 *   <b> Post Condition </b>
 *   @n   SPI_deInit can be call after this function call.
 *
 *   @b Modifies
 *   @n
 *
 *   @b Example
 *   @verbatim
            CSL_SpiHandle     hSpi
             Uint16          writeBuff[size];
            Uint16          buffLen;
            CSL_status       status;

            status = SPI_write(hSpi,&writeBuff, buffLen);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_write    (CSL_SpiHandle hSpi, Uint16    *writeBuffer, Uint16    bufLen);

/** ===========================================================================
 *   @n@b SPI_dataTransaction
 *
 *   @b Description
 *   @n This function read or Write data to specified device.
 *
 *   @b Arguments
 *   @verbatim
            spiHandle         Handle to the spi
            writeBuff         Pointer to the write buffer.
            buffLen           Maximum read buffer size.
            readOrWrite          Read or write command.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - transation success
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *
 *   <b> Pre Condition </b>
 *       This function can after SPI_open function.
 *
 *   <b> Post Condition </b>
 *   @n   SPI_deInit can be call after this function call.
 *
 *   @b Modifies
 *   @n
 *
 *   @b Example
 *   @verbatim
            CSL_SpiHandle   hSpi;
            Uint16          writeBuff[size];
            Uint16          buffLen;
            SPI_Command     readOrWrite
            CSL_status   status;
            ...
            ...
            status = SPI_dataTransaction(hSpi, &rwBuffer,
                                           rwBufLen, readOrWrite);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_dataTransaction(CSL_SpiHandle hSpi,
                                Uint16          *rwBuffer,
                                Uint16          rwBufLen,
                                SPI_Command     readOrWrite);

/**
@} */

/*****************************************************************************\
 * SPI Global variable                                                   *
\*****************************************************************************/
/** @addtogroup CSL_SPI_GLOBALS
 @{ */
static CSL_SpiObj    SPI_Instance;
/**< Object to store the information of SPI Handler */
/**
@} */


#ifdef __cplusplus
}
#endif
#endif /* _CSL_SPI_H_ */

