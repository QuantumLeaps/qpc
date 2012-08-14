/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_i2s.h
 *
 *  @brief I2S functional layer API header file
 *
 *  Path: \(CSLPATH)/inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 22-Aug-2008 Created
 * ============================================================================
 */

/** @defgroup CSL_I2S_API I2S
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 * I2S stands for Phased lock loop and is basically a closed loop frequency control
 * system which functioning is based on the phase sensitive detection of phase difference
 * between the input signal output sigmals of the controlled oscillator.
 * @subsection References
 *
 */

#ifndef _CSL_I2S_H_
#define _CSL_I2S_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "soc.h"
#include "csl_error.h"
#include "csl_types.h"
#include "csl_general.h"

/**
@defgroup CSL_I2S_SYMBOL  I2S Symbols Defined
@ingroup CSL_I2S_API
*/

/**
@defgroup CSL_I2S_DATASTRUCT  I2S Data Structures
@ingroup CSL_I2S_API
*/

/**
@defgroup CSL_I2S_FUNCTION  I2S Functions
@ingroup CSL_I2S_API
*/

/**
@defgroup CSL_I2S_ENUM I2S Enumerated Data Types
@ingroup CSL_I2S_API
*/

/**
@defgroup CSL_I2S_GLOBALS I2S Global declarations
@ingroup CSL_I2S_API
*/

/*****************************************************************************\
*         I2S global macro declarations
\*****************************************************************************/
/**
@addtogroup CSL_I2S_SYMBOL
@{
*/

/** Serializer transmission or reception bit Disable                        */
#define    CSL_I2S_SCRL_ENABLE_DISABLE            (Uint16)0
/** serializer transmission or reception bit Enable                            */
#define    CSL_I2S_SCRL_ENABLE_ENABLE            (Uint16)1
/** Stereo Mode                                                                */
#define    CSL_I2S_SCRL_MONO_DISABLE            (Uint16)0
/** Mono Mode                                                                */
#define    CSL_I2S_SCRL_MONO_ENABLE            (Uint16)1
/** Loopback disable                                                        */
#define    CSL_I2S_SCRL_LOOPBACK_DISABLE        (Uint16)0
/** Loopback enable                                                            */
#define    CSL_I2S_SCRL_LOOPBACK_ENABLE        (Uint16)1
/** Frame-sync is pulsed high                                                */
#define    CSL_I2S_SCRL_FSPOL_DISABLE            (Uint16)0
/** Frame-sync is pulsed low                                                */
#define    CSL_I2S_SCRL_FSPOL_ENABLE            (Uint16)1
/** serializer bit-clock polarity enable                                    */
#define    CSL_I2S_SCRL_CLOCKPOL_DISABLE        (Uint16)0
/** serializer bit-clock polarity disable                                    */
#define    CSL_I2S_SCRL_CLOCKPOL_ENABLE        (Uint16)1
/** Serializer receive/transmit data delay disable                            */
#define    CSL_I2S_SCRL_DATADELAY_DISABLE        (Uint16)0
/** Serializer receive/transmit data delay enable                            */
#define    CSL_I2S_SCRL_DATADELAY_ENABLE        (Uint16)1
/** Disabling data packing                                                    */
#define    CSL_I2S_SCRL_PACK_DISABLE            (Uint16)0
/** Enabling data packing                                                    */
#define    CSL_I2S_SCRL_PACK_ENABLE            (Uint16)1
/** Disable sign extension of words                                            */
#define    CSL_I2S_SCRL_SIGNEXT_DISABLE        (Uint16)0
/** Enable sign extension of words                                            */
#define    CSL_I2S_SCRL_SIGNEXT_ENABLE            (Uint16)1
/** disable sign extension of words                                            */
#define    CSL_I2S_SCRL_WORDLENGTH_DISABLE        (Uint16)0
/** Enable sign extension of words                                            */
#define    CSL_I2S_SCRL_WORDLENGTH_ENABLE        (Uint16)1
/** Serializer in master or slave mode                                        */
#define    CSL_I2S_SCRL_MODE_DISABLE            (Uint16)0
/** Serializer in master or slave mode                                        */
#define    CSL_I2S_SCRL_MODE_ENABLE            (Uint16)1
/** I2S/left-justified format                                                */
#define    CSL_I2S_SCRL_FORMAT_DISABLE            (Uint16)0
/** DSP format                                                                */
#define    CSL_I2S_SCRL_FORMAT_ENABLE            (Uint16)1

/** Left channel data transmit request interrupt disable                    */
#define    CSL_I2S_ICMRL_XMIT0_DISABLE            (Uint16)0
/** Left channel data transmit request interrupt Enable                        */
#define    CSL_I2S_ICMRL_XMIT0_ENABLE            (Uint16)1
/** Right channel data transmit request interrupt disable                    */
#define    CSL_I2S_ICMRL_XMIT1_DISABLE            (Uint16)0
/** Right channel data transmit request interrupt Enable                    */
#define    CSL_I2S_ICMRL_XMIT1_ENABLE            (Uint16)1
/** Left channel data receive request interrupt disable                        */
#define    CSL_I2S_ICMRL_RCV0_DISABLE            (Uint16)0
/** Left channel data receive request interrupt Enable                        */
#define    CSL_I2S_ICMRL_RCV0_ENABLE            (Uint16)1
/** Right channel data receive request interrupt disable                    */
#define    CSL_I2S_ICMRL_RCV1_DISABLE            (Uint16)0
/** Right channel data receive request interrupt disable                    */
#define    CSL_I2S_ICMRL_RCV1_ENABLE            (Uint16)1
/** Frame-sync error reporting disable                                        */
#define    CSL_I2S_ICMRL_FERR_DISABLE            (Uint16)0
/** Frame-sync error reporting Enable                                        */
#define    CSL_I2S_ICMRL_FERR_ENABLE            (Uint16)1
/** Overrun or under-run error reporting disable                            */
#define    CSL_I2S_ICMRL_O_U_DISABLE            (Uint16)0
/** Overrun or under-run error reporting Enable                                */
#define    CSL_I2S_ICMRL_O_U_ENABLE            (Uint16)1

/** Macros to insert a latency of 2 frames                                  */
#define CSL_I2S_LATENCY_2                   (2)
/** Macros to insert a latency of 3 frames                                  */
#define CSL_I2S_LATENCY_3                   (3)
/** Macros to insert a latency of 5 frames                                  */
#define CSL_I2S_LATENCY_5                   (5)
/** Macros to insert a latency of 6 frames                                  */
#define CSL_I2S_LATENCY_6                   (6)

/**
@}*/

/**************************************************************************\
* I2S global enum declaration                                             *
\**************************************************************************/
/**
@addtogroup CSL_I2S_ENUM
@{*/


/**
 *  \brief I2S Instance Number.
 *
 *  This is the Instance Ids for each I2S module and unique to each module
 */
typedef enum
{
     I2S_INSTANCE0,            /**< I2S device instance number 0              */
     I2S_INSTANCE1,            /**< I2S device instance number 1              */
     I2S_INSTANCE2,            /**< I2S device instance number 2              */
     I2S_INSTANCE3,            /**< I2S device instance number 3              */
     I2S_INVALID            /**< Invalid I2S device instance number        */
} I2S_Instance;

/**
 *  \brief I2S Audio channel type.
 *
 *  This is used at h/w open for the Audio channel type
 */
typedef enum
{
    I2S_CHAN_MONO,            /**< Audio data to be transfered is mono   */
    I2S_CHAN_STEREO,        /**< Audio data to be transfered is stereo */
    I2S_CHAN_UNDEF            /**< Undefined channel                     */
} I2S_ChanType;

/**
 *  \brief I2S Device mode of transmission
 *
 *  This is used at device open and to configure the I2S device
 */
typedef enum
{
    I2S_POLLED,                /**< I2S transfer is through I2S polled mode   */
    I2S_INTERRUPT,            /**< I2S transfer is through I2S intr mode     */
    DMA_POLLED,                /**< I2S transfer is through DMA polled mode   */
    DMA_INTERRUPT,            /**< I2S transfer is through DMA intr mode     */
    I2S_OPMODE_OTHER        /**< Undefined transfer mode                   */
}I2S_OpMode;


/**
 *  \brief I2S Loop back mode bit
 *
 *  This is used to enable or disable loop back mode
 */
typedef enum
{
    I2S_LOOPBACK_DISABLE,    /**< Loopback mode is disabled                 */
    I2S_LOOPBACK_ENABLE        /**< Loopback mode is enabled                  */
}I2S_LoopbackType;


/**
 *  \brief Serial transmission mode
 *
 *  This is used to enable or disable Serial transmission or receive
 */
typedef enum
{
    I2S_SERIAL_DISABLE,     /**< Serializer transmission/reception diable    */
    I2S_SERIAL_ENABLE        /**< Serializer transmission/reception enable    */
}I2S_SerialEna;

/**
 *  \brief I2S transmission data type(Mono/Stereo)
 *
 *  This is used to configure transmission data type of I2S data.
 */
typedef enum
{
    I2S_STEREO_ENABLE ,      /**< Data to be transmitted is Stereo            */
    I2S_MONO_ENABLE          /**< Data to be transmitted is Mono            */
} I2S_MonoType;

/**
 *  \brief I2S FSYNC Bit Polarity.
 *
 *  This is used to configure  FSYNC polarity
 */
typedef enum
{
    I2S_FSPOL_LOW,          /**< Left chan. is transmitted when fs is low  */
    I2S_FSPOL_HIGH          /**< Left chan. is transmitted when fs is high */
}I2S_FsyncPol;

/**
 *  \brief I2S Clock Polarity
 *
 *  This is used to configure I2S Clock polarity
 */
typedef enum
{
    I2S_RISING_EDGE,         /**< Tx on Rising Edge                         */
    I2S_FALLING_EDGE             /**< Tx Polarity on Falling Edge               */
}I2S_ClkPol;

/**
 *   \brief I2S Data transmission delay
 *
 *    This is used to configure I2S data delay
 */
typedef enum
{
    I2S_DATADELAY_ONEBIT,      /**< One bit delay in data transmission       */
    I2S_DATADELAY_TWOBIT       /**< Two bits delay in data transmission      */
} I2S_DataDelay;

/**
 *  \brief I2S Data packing bit
 *
 *  This is used to enable or disable data packing
 */
typedef enum
{
    I2S_DATAPACK_DISABLE,     /**< No data packing                           */
    I2S_DATAPACK_ENABLE       /**< Data is packed depending on word length   */
} I2S_DatapackType;

/**
 *  \brief I2S Sign extension bit
 *
 *  This is used to set sign extension of data
 */
typedef enum
{
    I2S_SIGNEXT_DISABLE,       /**< No sign extension for data                */
    I2S_SIGNEXT_ENABLE       /**< Data is sign extended                     */
} I2S_SignextType;

/**
 *  \brief I2S data word length
 *
 *  This is used to configure word length.
 */
typedef enum
{
    I2S_WORDLEN_8,            /**< Number of bits in each word of data is 8  */
    I2S_WORDLEN_10,           /**< Number of bits in each word of data is 10 */
    I2S_WORDLEN_12,           /**< Number of bits in each word of data is 12 */
    I2S_WORDLEN_14,           /**< Number of bits in each word of data is 14 */
    I2S_WORDLEN_16,           /**< Number of bits in each word of data is 16 */
    I2S_WORDLEN_18,           /**< Number of bits in each word of data is 18 */
    I2S_WORDLEN_20,           /**< Number of bits in each word of data is 20 */
    I2S_WORDLEN_24,           /**< Number of bits in each word of data is 24 */
    I2S_WORDLEN_32,           /**< Number of bits in each word of data is 32 */
    I2S_WORDLEN_INVALID       /**< Invalid word length                       */
} I2S_WordLen;

/**
 *  \brief I2S Device mode of operation
 *
 *  This is used at device init and to configure the I2S device
 */
typedef enum
{
    I2S_SLAVE,                /**< I2S device clk and FSYNC as slave device  */
    I2S_MASTER               /**< I2S device is clk master and FSYNC master */
} I2S_Mode;

/**
 *   \brief I2S Data format
 *
 *   This is used to configure I2S data format
 */
typedef enum
{
    I2S_DATAFORMAT_LJUST,     /**< Data format is left justified             */
    I2S_DATAFORMAT_DSP        /**< Data format is DSP                        */
} I2S_DataFormat;

/**
 *   \brief Serializer frame sync divider.
 *
 *   This value is used as the divide-down number of
 *   the internally generated frame sync to the serializer.
 */
typedef enum
{
    I2S_FSDIV8,               /**< FS Divide ratio is 8                      */
    I2S_FSDIV16,              /**< FS Divide ratio is 16                     */
    I2S_FSDIV32,              /**< FS Divide ratio is 32                     */
    I2S_FSDIV64,              /**< FS Divide ratio is 64                     */
    I2S_FSDIV128,             /**< FS Divide ratio is 128                    */
    I2S_FSDIV256,           /**< FS Divide ratio is 256                    */
    I2S_FSDIV_RESERVE          /**< FS Divide ratio is Reserved               */
} I2S_Fsdiv;

/**
 *   \brief Serializer clock divider.
 *
 *   This value is used as the divide-down number of
 *   the internally generated clock to the serializer.
 */
typedef enum
{
    I2S_CLKDIV2,              /**< CLK Divide ratio is 2                     */
    I2S_CLKDIV4,              /**< CLK Divide ratio is 4                     */
    I2S_CLKDIV8,              /**< CLK Divide ratio is 8                     */
    I2S_CLKDIV16,             /**< CLK Divide ratio is 16                    */
    I2S_CLKDIV32,             /**< CLK Divide ratio is 32                    */
    I2S_CLKDIV64,             /**< CLK Divide ratio is 64                    */
    I2S_CLKDIV128,            /**< CLK Divide ratio is 128                   */
    I2S_CLKDIV256            /**< CLK Divide ratio is 256                   */
} I2S_Clkdiv;


/**
 *   \brief FSYNC Error.
 *
 *
 *   Set FSYNC error bit as enable or disable
 */

typedef enum
{
    I2S_FSERROR_DISABLE,    /**< Frame-sync error disable                    */
    I2S_FSERROR_ENABLE        /**< Frame-sync error Enable                    */
} I2S_FsErr;

/**
 *   \brief Over or under run error.
 *
 *
 *   Set Over or under run bit enable or disable.
 */
typedef enum
{
    I2S_OUERROR_DISABLE,    /**< Overrun or under-run error disable            */
    I2S_OUERROR_ENABLE        /**< Overrun or under-run error Enable            */
} I2S_OuErr;

/**
@}*/


/**************************************************************************\
*               I2S global data structures                                *
\**************************************************************************/
/**
\addtogroup CSL_I2S_DATASTRUCT
@{*/


/**
 *   \brief Object structure.
 *
 *   object structure for handler
 */
typedef struct {
    I2S_Instance          i2sNum;       /**< Instance Id                     */
    I2S_OpMode              opMode;        /**< Mode of data transfer DMA/I2S   */
    I2S_ChanType          chType;       /**< I2s Channel Type MONO/STERIO    */
    ioport CSL_I2sRegs    *hwRegs;      /**< Pointer to I2S HW registers     */
    Uint16                  configured;   /**< Check for device configuration  */
    I2S_DatapackType      datapack;     /**< Data pack bit                   */
    I2S_Fsdiv              fsDiv;        /**< FSDIV value                     */
    I2S_WordLen              wordLen;        /**< Number of bits in a word          */
    I2S_LoopbackType      loopBackMode;    /**< Loopback mode enable/disable    */
    Bool                  firstRead;    /**< Flag to indicate first receive  */
}CSL_I2sObj;


/**
 *   \brief Configuration structure.
 *
 *   Hardware register configuration structure
 */
typedef struct{
    I2S_MonoType        dataType;        /**< Type of the data to be tx.rx   */
    I2S_LoopbackType    loopBackMode;    /**< Loopback mode enable/disable   */
    I2S_FsyncPol        fsPol;            /**< Left Channel transmission polarity */
    I2S_ClkPol            clkPol;            /**< Clock polarity                    */
    I2S_DataDelay        datadelay;        /**< I2S data delay                 */
    I2S_DatapackType    datapack;         /**< Data pack bit                  */
    I2S_SignextType        signext;          /**< sign of the data to be tx/rx   */
    I2S_WordLen            wordLen;        /**< Number of bits in a word         */
    I2S_Mode            i2sMode;        /**< I2S device operation mode      */
    I2S_DataFormat        dataFormat;        /**< Data format                    */
    I2S_Fsdiv            fsDiv;            /**< FSDIV value                    */
    I2S_Clkdiv            clkDiv;            /**< Clock divisor                    */
    I2S_FsErr            FError;            /**< Frame-sync error reporting enable/disable    */
    I2S_OuErr            OuError;        /**< Overrun or under-run error reporting enable/disable    */
}I2S_Config;

/**
 *   \brief Pointet to the handler.
 *
 *   Object structure pointer to the handler
 */
typedef CSL_I2sObj *CSL_I2sHandle;

/**
@}*/



/**
@}*/


/*****************************************************************************\
 * I2S function declarations                                                  *
\*****************************************************************************/

/** @addtogroup CSL_I2S_FUNCTION
@{*/

/** ===========================================================================
 *   @n@b I2S_open
 *
 *   @b Description
 *   @n This function returns the handle to the I2S
 *      instance. This handle is passed to all other CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            i2sInstNum    Instance number to open.
            opMode        Mode of operation (intrrupt/Polling/DMA)
            chType        Channel Type (Stereo/Mono)
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_I2sHandle
 *   @n                     Valid I2S handle will be returned if
 *                          handler value is not equal to NULL.
 *
 *   <b> Pre Condition </b>
 *   @n
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is returned in the status variable. If status
 *              returned is
 *   @li            NULL               - InValid I2S handle is returned
 *   @n   2.    CSL_DMA_ChannelObj object structure is populated
 *
 *   @b Modifies
 *   @n    1. CSL_I2sObj     I2S object structure
 *   @n    2.
 *
 *   @b Example
 *   @verbatim

            I2S_Instance         i2sInstNum;
            I2S_OpMode             opMode;
            I2S_ChanType          chType;
            ...

            hI2S = I2S_open(I2S_Instance    i2sInstNum,
                               I2S_OpMode        opMode,
                               I2S_ChanType     chType);
            ...
    @endverbatim
 *  ===========================================================================
 */

CSL_I2sHandle I2S_open(I2S_Instance        i2sInstNum,
                       I2S_OpMode        opMode,
                       I2S_ChanType     chType);

/** ===========================================================================
 *   @n@b I2S_setup
 *
 *   @b Description
 *   @n It configures the I2S Controller registers of particular handle as per
 *      the values passed in the I2S config structure.
 *
 *   @b Arguments
 *   @verbatim
            i2sHandle       Handle to the I2S.
            i2sHwConfig     Pointer to I2S Config structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Congig Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  I2S_open has to call, before calling I2S_setup
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n 1. I2S registers will be updated as per config parameter
 *   @n 2. CSL_I2sObj Object will be updated to store some info
 *              as passed config parameter.
 *
 *   @b Example
 *   @verbatim
            CSL_I2sHandle   hI2S;
            I2S_Config      hwConfig;
            Int16           status;

            status = I2S_setup(hI2S, &hwConfig);
            ...

     @endverbatim
 *  ===========================================================================
 */

CSL_Status I2S_setup(CSL_I2sHandle    i2sHandle,
                    I2S_Config         *i2sHwConfig);

/** ===========================================================================
 *   @n@b I2S_close
 *
 *   @b Description
 *   @n This function closes the specified handle to I2S.
 *
 *   @b Arguments
 *   @verbatim
            i2sHandle          Handle to the I2S
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_I2sHandle   hI2S;
            CSL_status   status;

            status = I2S_close(hI2S);
     @endverbatim
 *  ===========================================================================
 */

CSL_Status I2S_close(CSL_I2sHandle    i2sHandle);

/** ===========================================================================
 *   @n@b I2S_read
 *
 *   @b Description
 *   @n This function Read data form specified device.
 *      This function introduces a delay during the first data receive operation.
 *      Data latency is calculated as per the I2S hardware specification. This
 *      delay is applicable only to polled mode operation. For interrupt mode
 *      this function should be called only after getting the receive interrupt
 *      from the I2S.
 *
 *   @b Arguments
 *   @verbatim
            i2sHandle          Handle to the I2S
            readBuff          Pointer to the read buffer.
            buffLen           Maximum read buffer size.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Read successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  This function has to call after I2S_open function call.
 *
 *   <b> Post Condition </b>
 *   @n  SPI_close can be call after this function call.
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_I2sHandle   hI2S;
            Uint16          readBuff[size];
            Uint16          buffLen;
            CSL_status   status;

            status = I2S_read(hI2S, &readBuff, buffLen);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status I2S_read (CSL_I2sHandle    i2sHandle,
                    Uint16 *readBuff, Uint16 buffLen);

/** ===========================================================================
 *   @n@b I2S_write
 *
 *   @b Description
 *   @n This function Write data to specified device.
 *
 *   @b Arguments
 *   @verbatim
            i2sHandle         Handle to the I2S
            writeBuff         Pointer to the write buffer.
            buffLen           Maximum read buffer size.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Write successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  This function has to call after I2S_open function call.
 *
 *   <b> Post Condition </b>
 *   @n  SPI_close can be call after this function call.
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_I2sHandle   hI2S;
            Uint16          writeBuff[size];
            Uint16          buffLen;
            CSL_status   status;

            status = I2S_write(hI2S, &writeBuff, buffLen);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status I2S_write(CSL_I2sHandle    i2sHandle,
                    Uint16 *writeBuff, Uint16 buffLen);

/** ===========================================================================
 *   @n@b I2S_reset
 *
 *   @b Description
 *   @n This function Reset control and hardware specific register.
 *
 *   @b Arguments
 *   @verbatim
            i2sHandle         Handle to the I2S
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Reset successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  This function should call after the I2S_config call.
 *
 *   <b> Post Condition </b>
 *   @n  I2S_close function can be call after this function call.
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_I2sHandle   hI2S;
            CSL_status   status;

            status = I2S_reset(hI2S);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status I2S_reset(CSL_I2sHandle    hI2S);

/** ===========================================================================
 *   @n@b I2S_transEnable
 *
 *   @b Description
 *   @n  This function Enable or Disable transmission/receive bit
 *
 *   @b Arguments
 *   @verbatim
            i2sHandle         Handle to the I2S
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Reset successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  This fuction calls before read/write function to enable transnission
 *
 *   <b> Post Condition </b>
 *   @n  This fuction calls before read/write function to disable transnission
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_I2sHandle   hI2S;
            Uint16            enableBit
            CSL_status       status;

            status = I2S_transEnable(CSL_I2sHandle    i2sHandle,
                                    Uint16 enableBit);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status I2S_transEnable(CSL_I2sHandle    i2sHandle, Uint16 enableBit);
/**
@} */


/*****************************************************************************\
 * I2S Global variable                                                   *
\*****************************************************************************/
/** @addtogroup CSL_I2S_GLOBALS
 @{ */
static CSL_I2sObj I2S_InstanceNum[4];
/**< Object to store the information of I2S Handle for each channel */
/**
@} */


#ifdef __cplusplus
}
#endif
#endif /* _CSL_I2S_H_ */




