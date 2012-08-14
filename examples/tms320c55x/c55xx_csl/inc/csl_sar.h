/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_sar.h
 *
 *  @brief SAR functional layer API header file
 *
 *  Path: \\(CSLPATH)\\inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 22-Sept-2008 Added a new API function - SAR_getStatus()
 * ============================================================================
 */

/** @defgroup CSL_SAR_API SAR
 *
 * @section Introduction
 *
 * @subsection SAR Overview
 * 10 bit Sucessive Approximation analog to digital module converts an analog input voltage
 * to a corresponding digital value.It supports six channels.
 * @subsection References
 *    -# C5505_Spec_1.16.pdf
 *
 */

#ifndef _CSL_SAR_H_
#define _CSL_SAR_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Header Files */
#include <cslr.h>
#include <soc.h>
#include <csl_error.h>
#include <csl_types.h>
#include <csl_general.h>


/**
@defgroup CSL_SAR_SYMBOL  SAR Symbols Defined
@ingroup CSL_SAR_API
*/
/**
@defgroup CSL_SAR_DATASTRUCT  SAR Data Structures
@ingroup CSL_SAR_API
*/
/**
@defgroup CSL_SAR_FUNCTION  SAR Functions
@ingroup CSL_SAR_API
*/
/**
@defgroup CSL_SAR_ENUM  SAR Enumerated Data Types
@ingroup CSL_SAR_API
*/


/** @addtogroup CSL_SAR_SYMBOL
 @{ */

/*****************************************************************************\
          SAR global macro declarations
\*****************************************************************************/

/**  Maximum SAR Clock Divider value                          */
#define CSL_SAR_MAX_CLKDIV 127

/**  Number of SAR channels available                         */
#define CSL_SAR_NUM_CHANNEL 6

/**  Number of GPAIN available                                 */
#define CSL_SAR_NUM_GPAIN   4

/**  SAR ADC is busy in conversion                             */
# define CSL_SAR_ADC_BUSY  1

/**  SAR Conversion is completed and ADC data is available     */
# define CSL_SAR_DATA_AVAILABLE  0

/**  To start ADC conversion                                   */
# define CSL_SAR_ADC_START  1

/**  Sar module is in use                                       */
#define CSL_SAR_OPEN  1

/**  Sar module is not in use                                   */
#define CSL_SAR_CLOSE  0


/**
@} */

/**************************************************************************\
* SAR global typedef declarations                                         *
\**************************************************************************/

/** @addtogroup CSL_SAR_ENUM
 @{ */

/** \brief  Enumeration for SAR channel selection */
typedef enum {
    /** SAR Channel Number  0                    */
    CSL_SAR_CHAN_0   = 0,
    /** SAR Channel Number  1                    */
    CSL_SAR_CHAN_1  =  1,
    /** SAR Channel Number  2                    */
    CSL_SAR_CHAN_2  =  2,
    /** SAR Channel Number  3                    */
    CSL_SAR_CHAN_3  =  3,
    /** SAR Channel Number  4                    */
    CSL_SAR_CHAN_4  =  4,
    /** SAR Channel Number  5                    */
    CSL_SAR_CHAN_5  =  5,
    /** No channel Allocated                      */
    CSL_SAR_NO_CHAN =  6
   } CSL_SarChanSel;



/** \brief  Enumeration for Selection mode of SAR -
     To configure Status bit in SAR A/D Reference and Pin Register */
typedef enum {
    /**  Polling Mode                           */
    CSL_SAR_POLLING   = 0,
    /**  Interrupt Mode                         */
    CSL_SAR_INTERRUPT = 1,
    /**  DMA mode                               */
     CSL_SAR_DMA      = 2
   } CSL_SarOpMode;


/** \brief  Enumeration for SAR Single Cycle Operation
        To configure SingleCyc bit in SAR A/D Control Register
*/
typedef enum {
    /** Continously perform Conversion          */
    CSL_SAR_CONTINUOUS_CONVERSION = 0,
    /** Single Conversion                       */
     CSL_SAR_SINGLE_CONVERSION    = 1
   } CSL_SarChanCyc;

/** \brief  Enumeration for SAR Multi Channel Operation
        To configure MultiCh bit in SAR A/D Control Register
*/
typedef enum {
    /** No dischage of array                    */
     CSL_SAR_NO_DISCHARGE   = 0,
    /** Discharge of array                      */
    CSL_SAR_DISCHARGE_ARRAY = 1
} CSL_SarMultiCh;


/** \brief  Enumeration for SAR Reference voltage selection
*/
typedef enum {
/** Select Reference voltage 0.8 volts           */
    CSL_SAR_REF_0_8v = 0,
/** Select Reference voltage 1 volts             */
    CSL_SAR_REF_1v   = 1 ,
/** Select Reference voltage as Input voltage    */
    CSL_SAR_REF_VIN  = 2
} CSL_RefVoltage;


/** \brief  Enumeration for SAR GPAIN Pin Index
*/
typedef enum {
/**  GPAIN  0                                   */
    CSL_SAR_GPO_0 = 0,
/**  GPAIN  1                                   */
    CSL_SAR_GPO_1 = 1,
/**  GPAIN  2                                   */
    CSL_SAR_GPO_2 = 2,
/**  GPAIN  3                                   */
    CSL_SAR_GPO_3 = 3
} CSL_SarGPOPinSel;


/** \brief  Enumeration for SAR General Purpose Pin Direction
*/
typedef enum {
 /** GPAIN as i/p                               */
 CSL_SAR_GPO_IN  = 0,
 /** GPAIN as o/p                               */
 CSL_SAR_GPO_OUT = 1
} CSL_SarGPODir;

/** \brief  Enumeration for Selecting type of A/D conversion
*/
typedef enum
{
/** For Measuring keypad voltage                */
 CSL_KEYPAD_MEAS        = 0,
/** For Measuring battery voltage               */
 CSL_BATTRY_MEAS        = 1,
/** For Measuring Internal Voltage              */
 CSL_INT_VOLT_MEAS      = 2,
/** For Volume control Measurement              */
 CSL_VOLUME_CTRL        = 3,
/** For Measuring X-Position of LCD             */
 CSL_TOUCH_SCREEN_XAXIS = 4,
/** For Measuring Y-Postion of LCD              */
 CSL_TOUCH_SCREEN_YAXIS = 5,
/** For Measuring pen press down                */
 CSL_PEN_PRESS_DOWN     = 6
}CSL_SARMeasParam;


/**
@} */


/**
\addtogroup CSL_SAR_DATASTRUCT
@{ */
/**
 *  \brief Input parameters for setup API
 */
typedef struct {
    /** \brief system clock divisor for SAR                              */
    Uint16 SysClkDiv;
    /** \brief Interrupt Mode - Polling or Interrupt mode -
    to configure status bit in SAR A/D Reference and Pin Register  */
    CSL_SarOpMode OpMode;
       /** \brief MultiCh type - MultiChannel operation -
    to configure MultiCh bit in SAR A/D Control Register           */
    CSL_SarMultiCh MultiCh;
    /** \brief Refvoltage Selection - To select Analog Ref voltage */
    CSL_RefVoltage RefVoltage;
 } CSL_SarChSetup;

/**
 *  \brief structure parameters for SAR handler
 */
typedef struct{
    /** \brief BaseAddress of SAR Registers                        */
    CSL_SarRegsOvly baseAddr;
    /** \brief Channel Number                                      */
    Uint16 chanNo;
    /** \brief SAR Status                                          */
    Uint16 status;
}CSL_SarHandleObj;

/** \brief Pointer to SAR object handler                            */
typedef CSL_SarHandleObj*  SAR_Handle;

/**
 * \brief structure parameters for SAR Register Configuration API
 */
typedef struct{
    /** SAR A/D Control Register                                    */
    volatile Uint16 SARCTRL;
    /** SAR A/D Clock Control Register                              */
    volatile Uint16 SARCLKCTRL;
    /** SAR A/D Reference and Pin Register                          */
    volatile Uint16 SARPINCTRL;
    /** SAR A/D GPO Control Register                                */
    volatile Uint16 SARGPOCTRL;
}CSL_SarChConfig;

/**
@} */

/*******************************************************************************
 * SAR function declarations
 ******************************************************************************/

/** @addtogroup CSL_SAR_FUNCTION
 @{ */
/** ============================================================================
 *   @n@b SAR_init
 *
 *   @b Description
 *   @n This is the initialization function for the SAR CSL. The function
 *      must be called before calling any other API from this CSL.
 *
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK - Always returns
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n It enables SAR module and resets the SAR registers to
 *      their default values
 *
 *   @b Modifies
 *   @n SAR A/D Registers
 *
 *   @b Example
 *   @verbatim
            SAR_init(NULL);
     @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_init(
    void
);


/** ============================================================================
 *   @n@b SAR_deInit
 *
 *   @b Description
 *   @n This is the de-initialization function for the SAR CSL.
 *
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li    CSL_SOK - SAR_deInit is successful
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_init should be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  It disable the SAR module
 *
 *   @b Modifies
 *   @n status variable
 *
 *   @b Example
 *   @verbatim
            SAR_deInit(NULL);
     @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_deInit (
   void
);


/** ============================================================================
 *   @n@b SAR_chanOpen
 *
 *   @b Description
 *   @n This function populates the SAR object structure information ,
 *
 *   @b Arguments
 *   @verbatim
 *              SarObj - Pointer to SAR Object structure
 *              chanNo - Channel Number to be selected
 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @n     CSL_SOK - SAR_chanOpen is successful
 *   @n     CSL_ESYS_BADHANDLE - SAR obj structure is NULL
 *   @n     CSL_ESYS_INVPARAMS - Invalid channel number requested
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_init should be called before this API call
 *
 *   <b> Post Condition </b>
 *   @n  SAR object structure is populated
 *
 *
 *   @b Modifies
 *   @n  SAR object structure
 *
 *   @b Example
 *   @verbatim
            CSL_SarHandleObj    SarObj;
            CSL_SarChanSel      chanSel;
            SAR_Handle          hSar;
            CSL_Status          status;
            ...
            SAR_init(NULL);
            ....
            status = CSL_sarOpen(&Sarobj,chanSel);
            hSar = &SarObj;
            ...

    @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_chanOpen (
CSL_SarHandleObj* Sarobj,
CSL_SarChanSel   chanSel
    );


/** ============================================================================
 *   @n@b SAR_chanClose
 *
 *   @b Description
 *   @n This function closes the allocated channel .
 *
 *   @b Arguments
 *   @verbatim
            hSar    Pointer to SAR object structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li    CSL_SOK             - SAR_chanClose is successful
 *   @li    CSL_ESYS_BADHANDLE  - SAR obj structure is NULL
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_chanOpen API should be called before this API call
 *
 *   <b> Post Condition </b>
 *   @n  The particular channel will be free
 *
 *   @b Modifies
 *   @n    1. The status variable
 *   @b Example
 *   @verbatim
            CSL_SarHandleObj    SarObj;
            CSL_SarHandle   hSar;
            CSL_status       status;
            CSL_SarChanSel      chanSel;

            SAR_init(NULL);
            ....
            status = CSL_sarOpen(&Sarobj,chanSel);
            hSar = &SarObj;
            ...

            status = SAR_chanClose(hSar);
     @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_chanClose (
    CSL_SarHandleObj        *hsar
);



/** ============================================================================
 *   @n@b SAR_chanInit
 *
 *   @b Description
 *   @n Dummy function .
 *
 *   @b Arguments
 *   @verbatim
            hSar     pointer to Sar object handler
 *   @endverbatim
 *
 *  <b> Return Value </b>  CSL_Status
 *   @li    CSL_SOK            - SAR_chanInit call is successful
 *   @li    CSL_ESYS_BADHANDLE - SAR obj structure is NULL
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter

 *   <b> Pre Condition </b>
 *   @n  SAR_chanOpen API should be called before this API call
 *
 *
 *   <b> Post Condition </b>
 *
 *
 *
 *   @b Modifies
 *   @n    1. The status variable
 *
 *   @b Example
 *   @verbatim
            CSL_SarHandleObj    SarObj;
            CSL_SarChanSel      chanNo;
            SAR_Handle          hSar;
            CSL_Status          status
            ...
            status = SAR_chanOpen(&Sarobj,chanNo);
            hSar = &SarObj;
            status = SAR_chanInit(hSar);
            ...

    @endverbatim
 *  ============================================================================
 */

CSL_Status  SAR_chanInit(
SAR_Handle      hSar
 );

/** ============================================================================
 *   @n@b SAR_chanConfig
 *
 *   @b Description
 *   @n It provides Registers fields to configure SAR A/D Hardware Registers
 *        It configures the SAR registers as per the values passed
 *      in the corresponding SAR A/D Registers
 *
 *   @b Arguments
 *   @verbatim
            SarHandle      Pointer to SAR object structure
            ConfigParam    Pointer to Config structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li    CSL_SOK            - Config call is successful
 *   @li    CSL_ESYS_BADHANDLE - SAR obj structure is NULL
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_chanOpen API should be called before this API call
 *
 *   <b> Post Condition </b>
 *   @n SAR Registers will be configured according to Hardware config structure
 *      parameters
 *
 *   @b Modifies
 *   @n    1. The status variable
 *         2. SAR A/D Registers
 *
 *   @b Example
 *   @verbatim
         CSL_SarHandleObj SarObj;
         CSL_Status     status;
         CSL_SarHandle  hSar ;
         CSL_SarChConfig  ConfigParam;
         ...
         status = SAR_chanOpen(&Sarobj,chanNo);
         hSar = &SarObj;
         status = SAR_chanConfig(hSar,&ConfigParam);
         ...
    @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_chanConfig (
    CSL_SarHandleObj            *pSarHandle,
    CSL_SarChConfig             *pConfigParam
);

/** ============================================================================
 *   @n@b SAR_getConfig
 *
 *   @b Description
 *   @n It populates the values of the SAR registers in config structure

 *   @b Arguments
 *   @verbatim
            SarHandle      Pointer to SAR Object structure
            ConfigParam    Register specific parameters
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li    CSL_SOK            - SAR_getConfig call is successful
 *   @li    CSL_ESYS_BADHANDLE - SAR obj structure is NULL
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_chanOpen API should be called before this API call
 *
 *   <b> Post Condition </b>
 *   @n It populates the SAR register values in config structure

 *   @b Modifies
 *   @n    1. The status variable
 *
 *   @b Example
 *   @verbatim
         CSL_SarHandleObj SarObj;
         CSL_Status     status;
         CSL_SarHandle  SarHandle ;
         CSL_SarChConfig  ConfigParam;
         ...
         status = SAR_chanOpen(&Sarobj,chanNo);
         hSar = &SarObj;
         status = SAR_getConfig(SarHandle,&ConfigParam);
         ...
    @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_getConfig (
    CSL_SarHandleObj            *pSarHandle,
    CSL_SarChConfig             *pConfigParam
);

/** ============================================================================
 *   @n@b SAR_chanSetup
 *
 *   @b Description
 *   @n    It provides the setup parameters to the user to setup SAR A/D Registers
 *        accordingly .It setup the values in SAR registers as per
 *        the values passed in the hardware setup structure accordingly.
 *   @b Arguments
 *   @verbatim
            SarHandle     Pointer to SAR Object structure
            SarParam      Module specific setup parameters
 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li    CSL_SOK             - SAR_chanSetup API call is successful
 *   @li    CSL_ESYS_BADHANDLE - SAR obj structure is NULL
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_chanOpen API should be called before this API call
 *
 *   <b> Post Condition </b>
 *   @n SAR Registers will be configured according to Hardware setup parameters
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. SAR A/D Registers
 *   @b Example
 *   @verbatim
         CSL_SarHandleObj SarObj;
         CSL_Status     status;
         CSL_SarHandle  SarHandle;
         CSL_SarChSetup  SarParam;
         ...
         status = SAR_chanOpen(&Sarobj,chanNo);
         SarHandle = &SarObj;
         status = SAR_chanSetup(SarHandle,&SarParam);
         ...
    @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_chanSetup(
    CSL_SarHandleObj            *pSarHandle,
    CSL_SarChSetup              *pSarParam
);



/** ============================================================================
 *   @n@b SAR_chanCycSet
 *
 *   @b Description
 *   @n It configures the SAR module in single conversion or
 *   continuous conversion mode as per the values passed
 *   in the CSL_SarChanCyc enum variables.
 *
 *   @b Arguments
 *   @verbatim
            hSar            Pointer to SAR Object structure
            cycSelect       variable for selecting conversion type
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li    CSL_SOK - SAR_chanCycSet API call is successful
 *   @li    CSL_ESYS_BADHANDLE  - Sar Object handle is NULL
 *   @li    CSL_ESYS_INVPARAMS  - Ivalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_chanSetup API should be called before this API
 *
 *   <b> Post Condition </b>
 *   @n  SAR conversion type are configured according
 *       to the parameter
 *
 *   @b Modifies
 *   @n SAR A/D Registers
 *
 *   @b Example
 *   @verbatim
            CSL_SarHandle    hSar;
            CSL_SarChanCyc   cycSelect;
            CSL_status       status;
            CSL_SarChSetup  SarParam;
            ...

            status = SAR_chanOpen(&Sarobj,chanNo);
            SarHandle = &SarObj;
            status = SAR_chanSetup(SarHandle,&SarParam);
            status = SAR_chanCycSet(hSar, cycSelect);
     @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_chanCycSet (
    CSL_SarHandleObj          *hSar,
    CSL_SarChanCyc            cycSelect
);

/** ============================================================================
 *   @n@b SAR_startConversion
 *
 *   @b Description
 *   @n It sets ADC start bit in SAR control Register and start SAR A/D
 *       conversion.
 *
 *   @b Arguments
 *   @verbatim
           hSar    Pointer to SAR Object structure
 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li    CSL_SOK - SAR_startConversion API call is successful.
 *   @li    CSL_ESYS_BADHANDLE - SAR obj structure is NULL
 *
 *   <b> Pre Condition </b>
 *   @n  Any of SAR-chanSetup, SAR_chanConfig and SAR_A2DMeasParamSet
 *       must be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  SAR module will start A/D Conversion
 *
 *   @b Modifies
 *   @n The hardware registers of SAR.
 *
 *   @b Example
 *   @verbatim
        CSL_SarHandle hSar;
        ...

        status = SAR_startConversion (hSar);
     @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_startConversion (
    CSL_SarHandleObj        *hSar
    );

/** ============================================================================
 *   @n@b SAR_stopConversion
 *
 *   @b Description
 *   @n Stops SAR Module A/D conversion
 *
 *   @b Arguments
 *   @verbatim
            hSar   Pointer to SAR Object structure
 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li  CSL_SOK - SAR_stopConversion API call is successful
 *   @li  CSL_ESYS_BADHANDLE - SAR obj structure is NULL
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_startConversion API must be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  SAR module stops conversion
 *
 *   @b Modifies
 *   @n SAR hardware registers
 *
 *   @b Example
 *   @verbatim
        CSL_SarHandle           hSar;
        CSL_Status              status
        ...

        status = SAR_stopConversion (hSar);
     @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_stopConversion (
    CSL_SarHandleObj    *hSar
   );

/** ============================================================================
 *   @n@b SAR_A2DMeasParamSet();
 *
 *   @b Description
 *   @n This function initializes the device registers according to the type
 *   of A/D conversion
 *
 *   @b Arguments
 *   @verbatim
            hSar      Pointer to SAR Object structure
            param     Enum variable for type of A/D conversion
            chanNo    Pointer to the channel number
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li    CSL_SOK - SAR_A2DMeasParamSet API call is successful
 *   @li    CSL_ESYS_BADHANDLE - SAR obj structure is NULL
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_chanSetup API must be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  The registers will be configured according to input A/D type.
 *
 *   @b Modifies
 *   @n Hardware registers of the specified SAR instance.
 *
 *   @b Example
 *   @verbatim
        SAR_Handle          hSar;
        CSL_SarChSetup      SetupParam;
        CSL_SARMeasParam    param;
        CSL_SarChanSel      chanNo
        CSL_Status          status;
        ...
        status = SAR_chanSetup(hSar,&setupParam);
        ...
        status = SAR_A2DMeasParamSet (hSar, param , &chanNo);
     @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_A2DMeasParamSet (
    CSL_SarHandleObj            *hSar,
    CSL_SARMeasParam            param,
    CSL_SarChanSel*             chanNo
);

/** ============================================================================
 *   @n@b SAR_readData
 *
 *   @b Description
 *   @n It retrives the analog value converted in digital format (A/D output )
 *
 *   @b Arguments
 *   @verbatim
            hSar     Pointer to SAR Object structure
            readData Pointer to the output of A/D converted result
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li    CSL_SOK  - SAR_readData API call is successful
 *   @li    CSL_ESYS_BADHANDLE - SAR obj structure is NULL
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_A2DMeasParamSet or (SAR_chanCoonfig API ) and SAR_startConversion
 *       , SAR_readData must be called before this API
 *
 *   <b> Post Condition </b>
 *   @n  The output of A/D conversion will be populated in the output buffer
 *       passed in the API
 *
 *   @b Modifies
 *   @n readData variable
 *
 *   @b Example
 *   @verbatim
            CSL_SarHandle   hSar;
            Uint32          readData;
            CSL_SarChSetup  SetupParam;
            ...
            status = SAR_chanSetup(hSar,&setupParam);
            status = SAR_A2DMeasParamSet (hSar, param , &chanNo);
            Or
            status = SAR_chanConfig(hSar,&configParam);

            status = SAR_startConversion(hSar);
            while(CSL_SAR_DATA_AVAILABLE != SAR_getStatus(hSar));
            status = SAR_readData(hSar, &readData);
     @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_readData (
    CSL_SarHandleObj    *hSar,
    Uint16              *buffer
);


/** ============================================================================
 *   @n@b SAR_GPODirSet
 *
 *   @b Description
 *   @n This function is useful for configuring GPAIN pin
 *      as i/p or o/p
 *
 *   @b Arguments
 *   @verbatim
            hSar         Pointer to SAR object structure
            Index        GPAIN Pin Index
            dir          Pin Direction i/p or o/p
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li    CSL_SOK             - SAR_GPODataSet
 *   @li    CSL_ESYS_BADHANDLE  - Invalid handler
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_chanOpen must be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  Sets the direction for the given GPAIN pin
 *
 *   @b Modifies
 *   @n SAR A/D GPO Register
 *
 *   @b Example
 *   @verbatim
            CSL_SarHandle     hSar;
            CSL_SarGPOPinSel  Index;
            CSL_SarGPODir     dir;
            ...
            status = SAR_chanOpen(SarObj,chanSel);

            status = SAR_GPODirSet(hSar,Index,dir);
     @endverbatim
 *  ============================================================================
 */
CSL_Status SAR_GPODirSet (
    CSL_SarHandleObj            *hSar,
    CSL_SarGPOPinSel             Index,
    CSL_SarGPODir                dir
  );




/** ============================================================================
 *   @n@b SAR_getStatus
 *
 *   @b Description
 *   @n This function is useful for checking busy bit in SAR A/D Data Register
 *      It should be called in while loop till return value becomes zero
 *      before calling SAR_readData
 *   @b Arguments
 *   @verbatim
            hSar         Pointer to SAR object structure
            status       status variable
 *   @endverbatim
 *
 *   <b> Return Value </b>  Bool busyBit status
 *    CSL_SAR_DATA_AVAILABLE(0)     - ADC data is available
 *    CSL_SAR_ADC_BUSY(1) - ADC conversion is going on
 *
 *   <b> Pre Condition </b>
 *   @n SAR_A2DMeasParamSet or (SAR_chanCoonfig API ) and SAR_startConversion
 *     must be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  Reads the busy bit- check whether ADC data is available or not
 *       and when return value is CSL_SAR_DATA_AVAILABLE ,SAR_readData should
 *       be called
 *
 *   @b Modifies
 *   @n return value
 *      status variable
 *
 *   @b Example
 *   @verbatim
            CSL_SarHandle   hSar;
            Uint32          readData;
            CSL_SarChSetup  SetupParam;
            ...
            status = SAR_chanSetup(hSar,&setupParam);
            status = SAR_A2DMeasParamSet (hSar, param , &chanNo);
            Or
            status = SAR_chanConfig(hSar,&configParam);

            status = SAR_startConversion(hSar);
            while(CSL_SAR_DATA_AVAILABLE != SAR_getStatus(hSar,&status));
            status = SAR_readData(hSar, &readData);
     @endverbatim
 *  ============================================================================
 */
Bool SAR_getStatus(
    CSL_SarHandleObj            *hSar,
    CSL_Status                  *status
   );


/**
@} */

#ifdef __cplusplus
}
#endif

#endif /* _CSL_SAR_H_ */

