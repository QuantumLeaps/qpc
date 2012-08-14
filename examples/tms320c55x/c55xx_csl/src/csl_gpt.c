/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_gpt.c
 *
 *  @brief Functional layer API header file for GPT CSL
 *
 *  Path: \(CSLPATH)/src
 */

/* ============================================================================
 * Revision History
 * ================
 * 28-Sept-2008 Added GPT for CSL.
 * ============================================================================
 */


#include "csl_gpt.h"
/** ===========================================================================
 *   @n@b GPT_open
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
 *   @n  This should be call first before calling any other GPT Function.
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
            CSL_Handle        hgpt;
            CSL_Instance    instance;
            CSL_GptObj        gptObj;
              CSL_Status         status;
            ....
            ....
            instance = GPT_0.

            hgpt = GPT_open(GPT_0, &gptObj, &status);
     @endverbatim
 *  ===========================================================================
 */
CSL_Handle GPT_open(CSL_Instance    instance,
                    CSL_GptObj        *gptObj,
                      CSL_Status         *status)
{
    CSL_Handle hGpt;
    ioport volatile CSL_SysRegs  *sysRegs;

    *status = CSL_SOK;
    hGpt = NULL;

    if(NULL == gptObj)
    {
        *status = CSL_ESYS_BADHANDLE;
        return (hGpt);
    }

    if((instance < GPT_0) || (instance >= GPT_INVALID))
    {
        *status = CSL_ESYS_INVPARAMS;
        return (hGpt);
    }

    hGpt = gptObj;
    sysRegs = (CSL_SysRegs *)CSL_SYSCTRL_REGS;
    switch (instance)
    {
        case GPT_0:
            hGpt->Instance     = GPT_0;
            hGpt->regs        = (CSL_TimRegsOvly)(CSL_TIM_0_REGS);
            CSL_FINS(sysRegs->PCGCR1, SYS_PCGCR1_TMR0CG,
                                        CSL_SYS_PCGCR1_TMR0CG_ACTIVE);
            break;
        case GPT_1:
            hGpt->Instance     = GPT_1;
            hGpt->regs        = (CSL_TimRegsOvly)(CSL_TIM_1_REGS);
            CSL_FINS(sysRegs->PCGCR1, SYS_PCGCR1_TMR1CG,
                                        CSL_SYS_PCGCR1_TMR1CG_ACTIVE);
            break;
        case GPT_2:
            hGpt->Instance     = GPT_2;
            hGpt->regs        = (CSL_TimRegsOvly)(CSL_TIM_2_REGS);
            CSL_FINS(sysRegs->PCGCR1, SYS_PCGCR1_TMR2CG,
                                        CSL_SYS_PCGCR1_TMR2CG_ACTIVE);
            break;
        default:
            *status = CSL_ESYS_INVPARAMS;
    }

    return (hGpt);
}

/** ===========================================================================
 *   @n@b GPT_reset
 *
 *   @b Description
 *   @n This function reset specific register to GPT.
 *
 *   @b Arguments
 *   @verbatim
            hGpt            Handle to the GPT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  GPT_open() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function  GPT_close() API should call to close
 *      gpt operation.
 *
 *   @b Modifies
 *   @n    1. The object structure handler.
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_Handle              hGpt;
            ...

           hgpt = GPT_open(GPT_0, &gptObj, &status);
           status = GPT_reset(hgpt);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status GPT_reset(CSL_Handle hGpt)
{
    CSL_TimRegsOvly regPtr;
    ioport  volatile Uint16 *iafrReg;

    if(NULL == hGpt)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    regPtr = hGpt->regs;

    /* Reset Time control register                         */
    regPtr->TCR     = CSL_TIM_TCR_RESETVAL;
    /* Reset Time counter register 1                       */
    regPtr->TIMCNT1 = CSL_TIM_TIMCNT1_RESETVAL;
    /* Reset Time counter register 2                       */
    regPtr->TIMCNT2 = CSL_TIM_TIMCNT2_RESETVAL;
    /* Reset Time Period register 1                        */
    regPtr->TIMPRD1 = CSL_TIM_TIMPRD1_RESETVAL;
    /* Reset Time Period register 2                        */
    regPtr->TIMPRD2 = CSL_TIM_TIMPRD2_RESETVAL;
    /* Reset Time Interrupt register                       */
    //CSL_FINS(regPtr->TIMINT, TIM_TIMINT_INT, CSL_TIM_TIMINT_INT_SET);

    /* Reset Interrupt Aggregation Flag Registers          */
    iafrReg = (ioport  volatile Uint16 *)CSL_IAFR_REGS;
    switch(hGpt->Instance)
    {
        case GPT_0:
            (*iafrReg) = CSL_IAFR_TIMER_FLAG_0_RESETVAL;
            break;
        case GPT_1:
            (*iafrReg) = CSL_IAFR_TIMER_FLAG_1_RESETVAL;
            break;
        case GPT_2:
            (*iafrReg) = CSL_IAFR_TIMER_FLAG_2_RESETVAL;
            break;
        default:
            //(*iafrReg) = CSL_IAFR_TIMER_FLAG_0_1_2_RESETVAL;
            return CSL_ESYS_INVPARAMS;
    }

    return (CSL_SOK);
}


/** ===========================================================================
 *   @n@b GPT_close
 *
 *   @b Description
 *   @n This function close GPT operation.
 *
 *   @b Arguments
 *   @verbatim
            hGpt            Handle to the GPT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  GPT_open() must be called before this function call.
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
            CSL_Handle              hGpt;
            ...

           hgpt = GPT_open(GPT_0, &gptObj, &status);
           status = GPT_close(hgpt);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status GPT_close(CSL_Handle hGpt)
{
    ioport volatile CSL_SysRegs  *sysRegs;

    if(NULL == hGpt)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    sysRegs = (CSL_SysRegs *)CSL_SYSCTRL_REGS;

    /* Disable Module related timmer registers                      */
    switch(hGpt->Instance)
    {
        case GPT_0:
            CSL_FINS(sysRegs->PCGCR1, SYS_PCGCR1_TMR0CG,
                                        CSL_SYS_PCGCR1_TMR0CG_DISABLED);
            break;
        case GPT_1:
            CSL_FINS(sysRegs->PCGCR1, SYS_PCGCR1_TMR1CG,
                                        CSL_SYS_PCGCR1_TMR1CG_DISABLED);
            break;
        case GPT_2:
            CSL_FINS(sysRegs->PCGCR1, SYS_PCGCR1_TMR2CG,
                                        CSL_SYS_PCGCR1_TMR2CG_DISABLED);
            break;
    }

    /* Set the handler value to NULL                                      */
    hGpt->regs = NULL;
    hGpt = NULL;

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b GPT_start
 *
 *   @b Description
 *   @n This function start to load value from period register to
 *      count down register.
 *
 *   @b Arguments
 *   @verbatim
            hGpt            Handle to the GPT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  GPT_config() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function GPT_stop() function can call to
 *       stop loading from period register to countdown register.
 *
 *   @b Modifies
 *   @n    None
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_Handle              hGpt;
            ...

           hgpt = GPT_config(hGpt, &hwConfig);
           status = GPT_start(hgpt);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status GPT_start(CSL_Handle hGpt)
{
    CSL_TimRegsOvly regPtr;

    if(NULL == hGpt)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    regPtr = hGpt->regs;
    /* Enable the start timer                                    */
    CSL_FINS(regPtr->TCR, TIM_TCR_START, CSL_TIM_TCR_START_MASK);

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b GPT_stop
 *
 *   @b Description
 *   @n This function stop to load value from period register to
 *      count down register.
 *
 *   @b Arguments
 *   @verbatim
            hGpt            Handle to the GPT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  GPT_config() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function  GPT_close() API should call to close
 *      gpt operation.
 *
 *   @b Modifies
 *   @n    None
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_Handle              hGpt;
            ...

           status = GPT_start(hgpt);
           status = GPT_stop(hgpt);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status GPT_stop(CSL_Handle hGpt)
{
    CSL_TimRegsOvly regPtr;

    if(NULL == hGpt)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    regPtr = hGpt->regs;

    /* Stop the start bit                                                       */
    CSL_FINS(regPtr->TCR, TIM_TCR_START, CSL_TIM_TCR_START_RESETVAL);
    /* Disable the auto reload bit                                            */
    //CSL_FINS(regPtr->TCR, TIM_TCR_AUTORELOAD, CSL_TIM_TCR_AUTORELOAD_DISABLE);

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b GPT_getCnt
 *
 *   @b Description
 *   @n This function is to get condown value from countdown register
 *
 *   @b Arguments
 *   @verbatim
            hGpt            Handle to the GPT
            timeCnt         Pointer to the countdown value
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
 *   @n  After calling this function  GPT_close() API should call to close
 *      gpt operation.
 *
 *   @b Modifies
 *   @n    None
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_Handle              hGpt;
            Uint32               timeCnt;
            ...

           status = GPT_start(hgpt);
           status = GPT_getCnt(hgpt, &timeCnt);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status GPT_getCnt(CSL_Handle hGpt, Uint32 *timeCnt)
{
    Uint16        tim1;
    Uint16        tim2;
    CSL_TimRegsOvly regPtr;

    if(NULL == hGpt)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    regPtr = hGpt->regs;

    tim1 = regPtr->TIMCNT1;
    tim2 = regPtr->TIMCNT2;
    /* Calculate time counter                                                 */
    *timeCnt = ((Uint32)tim2 << 16u)|tim1;

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b GPT_config
 *
 *   @b Description
 *   @n This function is to Configure the GPT haware related registers
 *
 *   @b Arguments
 *   @verbatim
            hGpt            Handle to the GPT
            hwConfig        configuration structure.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  GPT_open() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function  GPT_reset() API should call to close
 *      gpt operation.
 *
 *   @b Modifies
 *   @n    This function modifies the GPT object structure.
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_Handle              hGpt;
             CSL_Config          hwConfig;
            ...
            hwConfig.autoLoad         = GPT_AUTO_ENABLE;
            hwConfig.ctrlTim         = GPT_TIMER_ENABLE;
            hwConfig.preScaleDiv     = GPT_PRE_SC_DIV_3;
            hwConfig.prdLow         = 0xFFFF;
            hwConfig.prdHigh         = 0x0000;

           hgpt = GPT_open(GPT_0, &gptObj, &status);
           status = GPT_config(hgpt, &hwConfig);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status GPT_config(CSL_Handle hGpt, CSL_Config *hwConfig)
{
    CSL_TimRegsOvly regPtr;

    if(NULL == hGpt)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    if(NULL == hwConfig)
    {
        return (CSL_ESYS_INVPARAMS);
    }

    regPtr = hGpt->regs;

    /* Set Timer Disable                                                        */
    CSL_FINS(regPtr->TCR, TIM_TCR_TIMEN, CSL_TIM_TCR_TIMEN_DISABLE);
    /* Set the Pre-scale devider                                                */
    CSL_FINS(regPtr->TCR, TIM_TCR_PSCDIV, hwConfig->preScaleDiv);
    /* Auto load enable or disable                                                */
    CSL_FINS(regPtr->TCR, TIM_TCR_AUTORELOAD, hwConfig->autoLoad);
    /* Enable/Disable pre-scale control timer                                    */
    CSL_FINS(regPtr->TCR, TIM_TCR_TIMEN, hwConfig->ctrlTim);

    /* Set GPT counter register to default value as 0                            */
    regPtr->TIMCNT1 = CSL_TIM_TIMCNT1_RESETVAL;
    regPtr->TIMCNT2 = CSL_TIM_TIMCNT2_RESETVAL;

    /* Set period register                                                         */
    regPtr->TIMPRD1 = hwConfig->prdLow;
    regPtr->TIMPRD2 = hwConfig->prdHigh;

    return (CSL_SOK);
}



