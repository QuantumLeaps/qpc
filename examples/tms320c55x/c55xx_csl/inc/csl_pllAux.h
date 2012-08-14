/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_pllAux.h
 *
 * @brief PLL functional layer inline header file
 *       -Extra Inline functions
 *
 *  Path: \(CSLPATH)/inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 21-Aug-2008 Created
 * ============================================================================
 */


#ifndef _CSL_PLLAUX_H_
#define _CSL_PLLAUX_H_

#include <csl_pll.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CSL_PLL_FUNCTION
@{*/

/** ============================================================================
 *   @n@b PLL_getTestLockMonStatus
 *
 *   @b Description
 *   @n This API is used to get the status of TEST LOCK bit in PLL CNTL2 reg
 *
 *   @b Arguments
 *   @verbatim
        hPll        Handle to the pll
     @endverbatim
 *
 *   <b> Return Value </b>  Bool
 *   @li                    TRUE             -  Test lock bit is set
 *
 *   @li                    FALSE            -  Test lock bit is not set.
 *
 *   <b> Pre Condition </b>
 *   @n  PLL_init and PLL_config should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n hPll variable
 *
 *   @b Example
 *   @verbatim
         CSL_Status              status;
         Bool                    bStatus;
         PLL_Obj                 pllObj;
         PLL_Config              configInfo;
         PLL_Handle              hPll;
         Uint32                  pllInstId;
         pllInstId = 0;
         status = PLL_init(&pllObj,pllInstId);
         hPll = &pllObj;
         .......
         Configure the PLL for 12.288MHz
         configInfo.PLLCNTL1  = 0x82ed; //Give proper value
         configInfo.PLLCNTL2  = 0x8000;
         configInfo.PLLINCNTL = 0x0806;
         configInfo.PLLOUTCNTL = 0x0200;
         status = PLL_config(hPll, &configInfo);
         bStatus = PLL_getTestLockMonStatus(hPll);
     @endverbatim
 *  ============================================================================
 */
static inline
Bool PLL_getTestLockMonStatus(PLL_Handle hPll)
{
    Uint16 TstLckMon = 0;

    TstLckMon = ((hPll->sysAddr->CGCR2 & (0x0008u)) >> (0x0003u));
    if( TstLckMon )
        return TRUE;
    return FALSE;
}

/** ============================================================================
 *   @n@b PLL_getConfig
 *
 *   @b Description
 *   @n This API is used to retrieve the current configuration of the pll.
 *
 *   @b Arguments
 *   @verbatim
        hPll        Handle to the pll
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            -  PLL config information is available
 *
 *   @li                    CSL_ESYS_BADHANDLE -  Handle is invalid
 *
 *   @li                    CSL_ESYS_INVPARAMS -  Pointer to the config structure is NULL
 *
 *   <b> Pre Condition </b>
 *   @n  PLL_init and PLL_config should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n pConfig variable
 *
 *   @b Example
 *   @verbatim
         CSL_Status              status;
         Bool                    bStatus;
         PLL_Obj                 pllObj;
         PLL_Config              configInfo;
         PLL_Config              gconfig;
         PLL_Handle              hPll;
         Uint32                  pllInstId;
         pllInstId = 0;
         status = PLL_init(&pllObj,pllInstId);
         hPll = &pllObj;
         .......
         Configure the PLL for 12.288MHz
         configInfo.PLLCNTL1 = 0x82ed;
         configInfo.PLLINCNTL = 0x8000;
         configInfo.PLLCNTL2 = 0x0806;
         configInfo.PLLOUTCNTL = 0x0200;
         status = PLL_config(hPll, &configInfo);
         status = PLL_getConfig(hPll,&gconfig);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status PLL_getConfig(PLL_Handle hPll,PLL_Config *pConfig)
{
       CSL_Status status = CSL_SOK;

    if(NULL == hPll)
    {
        status = CSL_ESYS_BADHANDLE;
        return status;
    }

    if(NULL == pConfig)
    {
        status = CSL_ESYS_INVPARAMS;
        return status;
    }

    pConfig->PLLCNTL1   = hPll->sysAddr->CGCR1;
    pConfig->PLLCNTL2   = hPll->sysAddr->CGCR2;
    pConfig->PLLINCNTL  = hPll->sysAddr->CGICR;
    pConfig->PLLOUTCNTL = hPll->sysAddr->CGOCR;
    return status;
}

/**
@} */
#endif// _CSL_PLLAUX_H_
