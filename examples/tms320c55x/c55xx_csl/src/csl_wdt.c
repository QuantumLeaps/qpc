/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_wdt.c
 *
 *  @brief WDT functional layer API definition file
 *
 *  Path: \(CSLPATH)/src
 */

/* ============================================================================
 * Revision History
 * ================
 * 17-Oct-2008 Added WDT for CSL.
 * 22-Oct-2008 Added Doxygen comments.
 * 22-Dec-2009 Modified as per new register layer definition
 * ============================================================================
 */

#include "csl_wdt.h"

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
                         CSL_Status        *status)
{
    CSL_WdtHandle    hWdt;
    ioport volatile CSL_SysRegs  *sysRegs;

    *status = CSL_SOK;
    hWdt    = NULL;

    if(NULL == wdtObj)
    {
        *status = CSL_ESYS_BADHANDLE;
        return (hWdt);
    }

    if((wdtNum < WDT_INST_0) || (wdtNum >= WDT_INST_INVALID))
    {
        *status = CSL_ESYS_INVPARAMS;
        return (hWdt);
    }

    hWdt = wdtObj;
    /* Set the system control register to enable WDT */
    sysRegs = (CSL_SysRegs *)CSL_SYSCTRL_REGS;

    switch(wdtNum)
    {
        case WDT_INST_0:
            hWdt->wdtInstance = wdtNum;
            hWdt->hwRegs      = CSL_WDT_REGS;
            CSL_FINS(sysRegs->PCGCR1, SYS_PCGCR1_TMR2CG,
                                      CSL_SYS_PCGCR1_TMR2CG_ACTIVE);
            break;

        default:
            *status = CSL_ESYS_INVPARAMS;
    }

    return (hWdt);
}

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
CSL_Status WDTIM_start(CSL_WdtHandle    hWdt)
{
    CSL_WdtRegsOvly regs;

    if(NULL == hWdt)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    regs = hWdt->hwRegs;
    /* Set the Disable lock register sequence number to unlock */
    regs->WDENLOK = CSl_WDT_WDENLOK_FIRST_SEQ;
    regs->WDENLOK = CSl_WDT_WDENLOK_SECOND_SEQ;
    regs->WDENLOK = CSl_WDT_WDENLOK_THIRD_SEQ;
    /* Set the zero'th bit to 1 for Disable register */
    CSL_FINS(regs->WDEN, WDT_WDEN_EN, CSL_WDT_WDEN_EN_ENABLE);

    return (CSL_SOK);
}

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
CSL_Status WDTIM_stop(CSL_WdtHandle    hWdt)
{
    CSL_WdtRegsOvly regs;

    if(NULL == hWdt)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    regs = hWdt->hwRegs;
    /* Set the Disable lock register sequence number to unlock */
    regs->WDENLOK = CSl_WDT_WDENLOK_FIRST_SEQ;
    regs->WDENLOK = CSl_WDT_WDENLOK_SECOND_SEQ;
    regs->WDENLOK = CSl_WDT_WDENLOK_THIRD_SEQ;
    /* Set the zero'th bit to 1 for Disable register */
    CSL_FINS(regs->WDEN, WDT_WDEN_EN, CSL_WDT_WDEN_EN_DISABLE);

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b WDTIM_getCnt
 *
 *   @b Description
 *   @n This function is to get condown value from Kick register
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
CSL_Status WDTIM_getCnt(CSL_WdtHandle    hWdt, Uint32 *timeCnt)
{
    CSL_WdtRegsOvly regs;

    if(NULL == hWdt)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    regs = hWdt->hwRegs;
    /* Receive Counter value from Kick register */
    *timeCnt = regs->WDKICK;

    return (CSL_SOK);
}

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
CSL_Status WDTIM_close(CSL_WdtHandle    hWdt)
{

    ioport volatile CSL_SysRegs  *sysRegs;

    if(NULL == hWdt)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    sysRegs = (CSL_SysRegs *)CSL_SYSCTRL_REGS;

    switch(hWdt->wdtInstance)
    {
        case WDT_INST_0:
            CSL_FINS(sysRegs->PCGCR1, SYS_PCGCR1_TMR2CG,
                                      CSL_SYS_PCGCR1_TMR2CG_DISABLED);
            break;
    }

    hWdt->hwRegs = NULL;
    hWdt         = NULL;

    return (CSL_SOK);
}


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
CSL_Status WDTIM_service(CSL_WdtHandle    hWdt)
{
    CSL_WdtRegsOvly regs;

    if(NULL == hWdt)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    regs = hWdt->hwRegs;
    /* Unlock sequence  for kick lock register */
    regs->WDKCKLK = CSl_WDT_WDKCKLK_FIRST_SEQ;
    regs->WDKCKLK = CSl_WDT_WDKCKLK_SECOND_SEQ;
    /* Set zero'th bit to 1                        */
    regs->WDKICK  = CSl_WDT_WDKICK_RESET;

    return (CSL_SOK);
}

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
                        WDTIM_Config    *hwConfig)
{
    CSL_WdtRegsOvly regs;

    if(NULL == hWdt)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    if(NULL == hwConfig)
    {
        return (CSL_ESYS_INVPARAMS);
    }

    regs = hWdt->hwRegs;

    /* Set the count lock register unlock sequence */
    regs->WDSVLR = CSl_WDT_WDSVLR_FIRST_SEQ;
    regs->WDSVLR = CSl_WDT_WDSVLR_SECOND_SEQ;
    /* Set Count value to register      */
    regs->WDSVR = hwConfig->counter;

    /* Set the Prescale lock register unlock sequence */
    regs->WDPSLR = CSl_WDT_WDPSLR_FIRST_SEQ;
    regs->WDPSLR = CSl_WDT_WDPSLR_SECOND_SEQ;
    /* Set Count value to register      */
    regs->WDPS = hwConfig->prescale;

    return (CSL_SOK);
}

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
                           WDTIM_Config     *getConfig)
{
    CSL_WdtRegsOvly regs;

    if(NULL == hWdt)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    if(NULL == getConfig)
    {
        return (CSL_ESYS_INVPARAMS);
    }

    regs = hWdt->hwRegs;
    /* Retrieve value from hardware register to config structure */
    getConfig->counter  = regs->WDSVR;
    getConfig->prescale = regs->WDPS;

    return (CSL_SOK);
}

