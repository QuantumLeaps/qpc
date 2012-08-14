/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_wdt.h
 *
 *  @brief I2S functional layer API header file
 *
 *  Path: \(CSLPATH)/inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 17-Oct-2008 Created
 * 22-Dec-2009 Modified as per new register layer definition
 * ============================================================================
 */

/** @defgroup CSL_WDT_API WDT
 *
 * @section Introduction
 *
 * @subsection WDT Overview
 * C5505 includes three 32-bit timer with 16-bit prescaler that can divide the
 * CPU clock and uses this prescale value as a watchdog timer clock.
 *
*/

#ifndef _CSL_WDT_H_
#define _CSL_WDT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "soc.h"
#include "csl_error.h"
#include "csl_types.h"
#include "csl_general.h"

/**
@defgroup CSL_WDT_SYMBOL  WDT Symbols Defined
@ingroup CSL_WDT_API
*/

/**
@defgroup CSL_WDT_DATASTRUCT  WDT Data Structures
@ingroup CSL_WDT_API
*/

/**
@defgroup CSL_WDT_FUNCTION  WDT Functions
@ingroup CSL_WDT_API
*/

/**
@defgroup CSL_WDT_ENUM WDT Enumerated Data Types
@ingroup CSL_WDT_API
*/

/**
@defgroup CSL_WDT_GLOBALS WDT Global declarations
@ingroup CSL_WDT_API
*/

/*****************************************************************************\
*         WDT global macro declarations
\*****************************************************************************/
/**
@addtogroup CSL_WDT_SYMBOL
@{
*/

/** Kick Lock register First unlock sequence                                */
#define CSl_WDT_WDKCKLK_FIRST_SEQ            0x5555
/** Kick Lock register Second unlock sequence                                */
#define CSl_WDT_WDKCKLK_SECOND_SEQ            0xAAAA
/** Counter Lock register First unlock sequence                                */
#define CSl_WDT_WDSVLR_FIRST_SEQ            0x6666
/** Counter Lock register Second unlock sequence                               */
#define CSl_WDT_WDSVLR_SECOND_SEQ            0xBBBB
/** Disable Lock register First unlock sequence                                */
#define CSl_WDT_WDENLOK_FIRST_SEQ            0x7777
/** Disable Lock register Second unlock sequence                               */
#define CSl_WDT_WDENLOK_SECOND_SEQ            0xCCCC
/** Disable Lock register Third unlock sequence                                */
#define CSl_WDT_WDENLOK_THIRD_SEQ            0xDDDD
/** Prescale Lock register First unlock sequence                               */
#define CSl_WDT_WDPSLR_FIRST_SEQ            0x5A5A
/** Prescale Lock register Second unlock sequence                              */
#define CSl_WDT_WDPSLR_SECOND_SEQ            0xA5A5
/** Kick register Reset bit for service                                     */
#define CSl_WDT_WDKICK_RESET                0x0001
/**
@}*/

/**************************************************************************\
* WDT global enum declaration                                             *
\**************************************************************************/
/**
@addtogroup CSL_WDT_ENUM
@{*/


/**
 *  \brief WDT Instance Number.
 *
 *  This is the Instance Ids for each WDT module and unique to each module
 */
typedef enum{
    WDT_INST_0 = 0,                    /**< WDT Instance 0                    */
    WDT_INST_INVALID                   /**< WDT invalid Instance              */
}WDT_Instance;

/**
@}*/


/**************************************************************************\
*               WDT global data structures                                *
\**************************************************************************/
/**
\addtogroup CSL_WDT_DATASTRUCT
@{*/


/**
 *   \brief Object structure.
 *
 *   object structure for WDT handler
 */
typedef struct {
    WDT_Instance          wdtInstance;  /**< WDT Instance Id                   */
    CSL_WdtRegsOvly          hwRegs;       /**< Pointer to WDT HW registers       */
}CSL_WdtObj;

/**
 *   \brief WDT CSL handle.
 */
typedef CSL_WdtObj *CSL_WdtHandle;


/**
 *   \brief Configuration structure.
 *
 *   Hardware register configuration structure for WDT
 */
typedef struct {
    Uint16    counter;                    /**< Counter value for the WDT         */
    Uint16    prescale;                    /**< Prescale value for the WDT        */
}WDTIM_Config;

/**
@}*/

/*****************************************************************************\
 * WDT function declarations                                                  *
\*****************************************************************************/

/** @addtogroup CSL_WDT_FUNCTION
@{*/

/** ===========================================================================
 *   @n@b WDTIM_open
 *
 *   @b Description
 *   @n This is the initialization function for the GPT CSL. The function
 *      must be called before calling any other API from this CSL. This
 *      function is idem-potent. Currently, the function just returns handler
 *      to application.
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                        CSL_SOK - Always returns
 *
 *                            CSL_Handle
 *                              Pointer to the object structure.
 *   <b> Pre Condition </b>
 *   @n  This should be call first before calling any other WDT Function.
 *
 *   <b> Post Condition </b>
 *   @n  The status is updated in the status variable. If status
 *        returned is
 *   @li        CSL_SOK            - Valid GPT handle is returned
 *   @li        CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   @b Modifies
 *   @n 1. The status variable
 *   @n 2. The object structure handler.
 *
 *   @b Example
 *   @verbatim
            CSL_WdtHandle    hWdt;
            WDT_Instance    wdtNum;
            CSL_WdtObj        wdtObj;
              CSL_Status         status;
            ....
            ....
            instance = WDT_INST_0.

            hWdt = WDTIM_open(WDT_INST_0, &wdtObj, &status);
     @endverbatim
 *  ===========================================================================
 */
CSL_WdtHandle WDTIM_open(WDT_Instance    wdtNum,
                        CSL_WdtObj        *wdtObj,
                        CSL_Status        *status);

/** ===========================================================================
 *   @n@b WDTIM_start
 *
 *   @b Description
 *   @n This function start to load value from counter register to
 *      kick register.
 *
 *   @b Arguments
 *   @verbatim
            hWdt            Handle to the WDT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  WDTIM_config() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function WDTIM_stop() function can call to
 *       stop loading from counter register to kick register.
 *
 *   @b Modifies
 *   @n    None
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_WdtHandle          hWdt;
            ...

           status = WDTIM_config(hWdt, &hwConfig);
           status = WDTIM_start(hWdt);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status WDTIM_start(CSL_WdtHandle    hWdt);

/** ===========================================================================
 *   @n@b GPT_stop
 *
 *   @b Description
 *   @n This function stop to load value from counter register to
 *      kick register.
 *
 *   @b Arguments
 *   @verbatim
            hWdt            Handle to the WDT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  GPT_start() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function  WDTIM_close() API should call to close
 *      WDT operation.
 *
 *   @b Modifies
 *   @n    None
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_WdtHandle          hWdt;
            ...

           status = WDTIM_start(hWdt);
           status = WDTIM_stop(hWdt);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status WDTIM_stop(CSL_WdtHandle    hWdt);

/** ===========================================================================
 *   @n@b WDTIM_getCnt
 *
 *   @b Description
 *   @n This function is to get countdown value from Kick register
 *
 *   @b Arguments
 *   @verbatim
            hWdt            Handle to the WDT
            timeCnt         Pointer to the countdown value
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  WDTIM_start() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function  WDTIM_close() API should call to close
 *      wdt operation.
 *
 *   @b Modifies
 *   @n    None
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_WdtHandle          hWdt;
            Uint32               timeCnt;
            ...

           status = WDTIM_start(hWdt);
           status = WDTIM_getCnt(hWdt, &timeCnt);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status WDTIM_getCnt(CSL_WdtHandle    hWdt, Uint32 *timeCnt);

/** ===========================================================================
 *   @n@b WDTIM_close
 *
 *   @b Description
 *   @n This function close WDT operation.
 *
 *   @b Arguments
 *   @verbatim
            hWdt            Handle to the WDT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  WDTIM_open() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function no other function can call.
 *
 *   @b Modifies
 *   @n    None
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_WdtHandle         hWdt;
            ...

           hWdt = WDTIM_open(WDT_INST_0, &wdtObj, &status);
           status = WDTIM_close(hWdt);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status WDTIM_close(CSL_WdtHandle    hWdt);

/** ===========================================================================
 *   @n@b WDTIM_service
 *
 *   @b Description
 *   @n This function Service watch dog timer. Without service function WDT
 *      reset the target device.
 *
 *   @b Arguments
 *   @verbatim
            hWdt            Handle to the WDT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  WDTIM_start() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n    None
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_WdtHandle         hWdt;
            ...

           status = WDTIM_start(hWdt);
           status = WDTIM_service(hWdt);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status WDTIM_service(CSL_WdtHandle    hWdt);

/** ===========================================================================
 *   @n@b WDTIM_config
 *
 *   @b Description
 *   @n This function is to Configure the WDT hardware related registers
 *
 *   @b Arguments
 *   @verbatim
            hWdt            Handle to the WDT
            hwConfig        configuration structure.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  WDTIM_open() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function  WDTIM_start() API should call to start
 *      wdt operation.
 *
 *   @b Modifies
 *   @n    This function modifies the WDT object structure.
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_WdtHandle          hWdt;
            WDTIM_Config         hwConfig;
            ...
            hwConfig.counter  = 0xFFFF;
            hwConfig.prescale = 0x00FF;

           hWdt = WDTIM_open(WDT_INST_0, &wdtObj, &status);
           status = WDTIM_config(hWdt, &hwConfig);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status WDTIM_config(CSL_WdtHandle    hWdt,
                        WDTIM_Config    *hwConfig);

/** ===========================================================================
 *   @n@b WDTIM_getConfig
 *
 *   @b Description
 *   @n This function is to Get Configure of the previous set configuration
 *
 *   @b Arguments
 *   @verbatim
            hWdt            Handle to the WDT
            getConfig        configuration structure.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  WDTIM_config() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function  WDTIM_Close() API can be call to close
 *      wdt operation.
 *
 *   @b Modifies
 *   @n    This function modifies the WDT object structure.
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_WdtHandle          hWdt;
            WDTIM_Config         getConfig;
            ...
            hwConfig.counter  = 0xFFFF;
            hwConfig.prescale = 0x00FF;

           status = WDTIM_config(hWdt, &hwConfig);
           status = WDTIM_getConfig(hWdt, &getConfig);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status WDTIM_getConfig(CSL_WdtHandle    hWdt,
                           WDTIM_Config     *getConfig);

/**
@}*/
#ifdef __cplusplus
}
#endif
#endif /* _CSL_WDT_H_ */



