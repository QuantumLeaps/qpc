/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_pll.c
 *
 *  @brief PLL functional layer API source file
 *
 *  Path: \(CSLPATH)\src\
 */

/* ============================================================================
 * Revision History
 * ================
 * 22-Aug-2008 Created
 * ============================================================================
 */

#include "csl_pll.h"
#include "csl_pllAux.h"

/** ============================================================================
 *   @n@b PLL_init
 *   @b Description
 *   @n This is the initialization function for the pll CSL. The function
 *      must be called before calling any other API from this CSL. This
 *      will initialize the PLL object.
 *
 *   @b Arguments
 *   @verbatim
            pllObj          Pointer to PLL object.
            pllInstId       Instance number of the PLL.
    @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                                  CSL_SOK - Init call is successful
 *   @li                               CSL_ESYS_INVPARAMS- Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n   PLL object structure is populated
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
         PLL_Obj                 pllObj;
         CSL_Status              status;
         Uint32                  pllInstId;
         pllInstId = 0;
         status = PLL_init(&pllObj,pllInstId);
     @endverbatim
 *  ============================================================================
 */
CSL_Status PLL_init( PLL_Obj * pllObj,
                     Uint32 pllInstId)

{
    CSL_Status    status;

    status = CSL_ESYS_INVPARAMS;

    if(NULL != pllObj)
    {
        switch (pllInstId)
        {
            case CSL_PLL_INST_0:
                pllObj->instId = pllInstId;
                pllObj->sysAddr = CSL_SYSCTRL_REGS;
                status = CSL_SOK;
                break;
           /* Invalid instance number */
            default:
                break;
        }
     }

    return (status);
}

/** ============================================================================
 *   @n@b PLL_config
 *
 *   @b Description
 *   @n This API is used to configure the PLL
 *
 *   @b Arguments
 *   @verbatim
           hPll             Handle to the pll
           pconfigInfo      pointer to PLL_config structure.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Configuring the pll is successful
 *
 *   @li                    CSL_ESYS_BADHANDLE  - The handle passed is invalid
 *
 *   @li                    CSL_ESYS_INVPARAMS  - The pconfigInfo is NULL
 *
 *   <b> Pre Condition </b>
 *   @n  PLL_init should be successfully called.
 *
 *   <b> Post Condition </b>
 *   @n  Configures the PLL registers.
 *
 *   @b Modifies
 *   @n hPll variable
 *
 *   @b Example
 *   @verbatim
         CSL_Status              status;
         PLL_Obj                 pllObj;
         PLL_Config              configInfo;
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
     @endverbatim
 *  ============================================================================
 */

CSL_Status PLL_config(PLL_Handle hPll,
                      PLL_Config *pconfigInfo)
{
    Uint16 timeout = TIMEOUT;
    CSL_Status status = CSL_SOK;

    if(NULL == hPll)
    {
        status = CSL_ESYS_BADHANDLE;
        return status;
    }

    if(NULL == pconfigInfo)
    {
        status = CSL_ESYS_INVPARAMS;
        return status;
    }

    hPll->pllConfig = pconfigInfo;

    /* Force to BYPASS mode */
    CSL_FINST(hPll->sysAddr->CCR2, SYS_CCR2_SYSCLKSEL, BYPASS);

    /* Set CLR_CTRL = 0 in CGCR1 */
    CSL_FINST(hPll->sysAddr->CGCR1, SYS_CGCR1_CLR_CNTL, CLEAR);

    hPll->sysAddr->CGICR = pconfigInfo->PLLINCNTL;
    hPll->sysAddr->CGOCR = pconfigInfo->PLLOUTCNTL;
    hPll->sysAddr->CGCR2 = pconfigInfo->PLLCNTL2;

    /*
     * Set PLL_PWRDN = 0, PLL_STANDBY = 0, CLR_CNTL = 1 and program MH in CGCR1
     * according to your required settings
     */
    CSL_FINST(hPll->sysAddr->CGCR1, SYS_CGCR1_PLL_PWRDN, POWERED);
    CSL_FINST(hPll->sysAddr->CGCR1, SYS_CGCR1_PLL_STANDBY, ACTIVE);
    CSL_FINST(hPll->sysAddr->CGCR1, SYS_CGCR1_CLR_CNTL, SET);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

    hPll->sysAddr->CGCR1 |= ((CSL_SYS_CGCR1_VP_MASK | CSL_SYS_CGCR1_VS_MASK) &
                             pconfigInfo->PLLCNTL1);
#else

    hPll->sysAddr->CGCR1 |= (CSL_SYS_CGCR1_MH_MASK & pconfigInfo->PLLCNTL1);

#endif

    while (!PLL_getTestLockMonStatus(hPll) && timeout--) ;

    /* Select pll */
    CSL_FINST(hPll->sysAddr->CCR2, SYS_CCR2_SYSCLKSEL, LOCK);

    return (status);
}

/** ============================================================================
 *   @n@b PLL_enable
 *
 *   @b Description
 *   @n This API is used to enable the PLL
 *
 *   @b Arguments
 *   @verbatim
            hPll        Handle to the pll
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             -  Enabling the PLL is successful
 *
 *   @li                    CSL_ESYS_BADHANDLE  - The handle passed is invalid
 *
 *   <b> Pre Condition </b>
 *   @n  PLL_init and PLL_config should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Pll is enabled
 *
 *   @b Modifies
 *   @n hPll variable
 *
 *   @b Example
 *   @verbatim
         CSL_Status              status;
         PLL_Obj                 pllObj;
         PLL_Config              configInfo;
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
         status = PLL_enable(hPll);

     @endverbatim
 *  ============================================================================
 */

CSL_Status PLL_enable(PLL_Handle    hPll)
{
    CSL_Status    status;
    Uint16        timeout;

    status  = CSL_SOK;
    timeout = TIMEOUT;

    if(NULL == hPll)
    {
        status = CSL_ESYS_BADHANDLE;
        return (status);
    }

    /* Enable the PLL */
    CSL_FINST(hPll->sysAddr->CGCR1, SYS_CGCR1_PLL_PWRDN, POWERED);

    while (!PLL_getTestLockMonStatus(hPll) && timeout--) ;

    /* Select pll */
    CSL_FINST(hPll->sysAddr->CCR2, SYS_CCR2_SYSCLKSEL, LOCK);

    return (status);
}

/** ============================================================================
 *   @n@b PLL_bypass
 *
 *   @b Description
 *   @n This API is used to Bypass the PLL.
 *
 *   @b Arguments
 *   @verbatim
            hPll        Handle to the pll
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             -  Bypassing the PLL is successful
 *
 *   @li                    CSL_ESYS_BADHANDLE  - The handle passed is invalid
 *
 *   <b> Pre Condition </b>
 *   @n  PLL_init and PLL_config should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Pll is bypassed
 *
 *   @b Modifies
 *   @n hPll variable
 *
 *   @b Example
 *   @verbatim
         CSL_Status              status;
         PLL_Obj                 pllObj;
         PLL_Config              configInfo;
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
         status = PLL_bypass(hPll);
     @endverbatim
 *  ============================================================================
 */
CSL_Status PLL_bypass(PLL_Handle hPll)
{

    CSL_Status status;

    status = CSL_SOK;

    if(NULL == hPll)
    {
        status = CSL_ESYS_BADHANDLE;
        return status;
    }

    /* Bypass the PLL */
    CSL_FINST(hPll->sysAddr->CCR2, SYS_CCR2_SYSCLKSEL, BYPASS);
    CSL_FINST(hPll->sysAddr->CGCR1, SYS_CGCR1_PLL_PWRDN, POWERDWN);

    return (status);
}

/** ============================================================================
 *   @n@b PLL_reset
 *
 *   @b Description
 *   @n Resets all the PLL registers.
 *
 *   @b Arguments
 *   @verbatim
            hPll        Handle to the pll
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             -         Resetting the PLL is successful.
 *
 *   @li                    CSL_ESYS_BADHANDLE  - The handle passed is invalid
 *
 *
 *   <b> Pre Condition </b>
 *   @n  PLL_init should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  PLL registers are resetted.
 *
 *   @b Modifies
 *   @n hPll variable
 *
 *   @b Example
 *   @verbatim
            CSL_Status              status;
             PLL_Obj                 pllObj;
            Uint32                  pllInstId;
            PLL_Handle              hPll;
            pllInstId = 0;
            status = PLL_init(&pllObj,pllInstId);
              .....
            hPll = &pllObj;
            status = PLL_reset(hPll);
     @endverbatim
 *  ============================================================================
 */
CSL_Status PLL_reset(PLL_Handle hPll)
{
    CSL_Status status;

    status = CSL_SOK;

    if(NULL == hPll)
    {
        status = CSL_ESYS_BADHANDLE;
        return (status);
    }

    /* Bypass the PLL */
    CSL_FINST(hPll->sysAddr->CCR2, SYS_CCR2_SYSCLKSEL, BYPASS);

    /* Reset PLL register */
    hPll->sysAddr->CGCR1 = CSL_SYS_CGCR1_RESETVAL;
    hPll->sysAddr->CGICR = CSL_SYS_CGICR_RESETVAL;
    hPll->sysAddr->CGOCR = CSL_SYS_CGOCR_RESETVAL;
    hPll->sysAddr->CGCR2 = CSL_SYS_CGCR2_RESETVAL;

    return (status);
}

