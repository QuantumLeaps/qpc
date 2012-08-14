/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_sar.c
 *
 *  @brief SAR functional layer API source file
 *
 *  Path:  \\(CSLPATH)\\src
 */

/* ============================================================================
 * Revision History
 * ================
 * 22-Sept-2008 Added a new API function - SAR_getStatus()
 * 07-Oct-2008 Updated according to csl_sysctrl file
 * 22-Oct-2008 Updated according to new cslr_sar.h file
 * ============================================================================
 */
#include "csl_sar.h"


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
 *      their default value
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

CSL_Status SAR_init (
void
)
{
    volatile Uint16 i;
    /* Enable the master clock in PCGCR Register */
    CSL_FINST (CSL_SYSCTRL_REGS->PCGCR1,SYS_PCGCR1_SYSCLKDIS,ACTIVE );

    /* Enable the Analog Register Clock Control module */
    CSL_FINST (CSL_SYSCTRL_REGS->PCGCR2,SYS_PCGCR2_ANAREGCG, ACTIVE);

    /* Enable SAR clock module */
    CSL_FINST (CSL_SYSCTRL_REGS->PCGCR2, SYS_PCGCR2_SARCG, ACTIVE);


    /* Reset SAR A/D Registers */
    CSL_SAR_REGS->SARCTRL = CSL_ANACTRL_SARCTRL_RESETVAL
                            | CSL_ANACTRL_SARCTRL_CHSEL_MASK;

    CSL_SAR_REGS->SARCLKCTRL = CSL_ANACTRL_SARCLKCTRL_RESETVAL
                            & CSL_ANACTRL_SARCLKCTRL_ADCCLKDIV_MASK;

    CSL_SAR_REGS->SARPINCTRL = CSL_ANACTRL_SARPINCTRL_RESETVAL;
    CSL_SAR_REGS->SARGPOCTRL = CSL_ANACTRL_SARGPOCTRL_RESETVAL;

    /* provide delay after reset */
    for(i=0;i<100;i++);

    /* Configuring CLOCKOUT pin */
    CSL_FINST (CSL_SYSCTRL_REGS->CCSSR, SYS_CCSSR_SRC, MODE9);

    return CSL_SOK;
}

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
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  It disable the SAR module
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            SAR_deInit(NULL);
     @endverbatim
 *  ============================================================================
 */

CSL_Status SAR_deInit(void)
{
    /* Disable the Analog Register Clock Control module */
    CSL_FINST (CSL_SYSCTRL_REGS->PCGCR2, SYS_PCGCR2_ANAREGCG, DISABLED);

    /* Disable SAR clock module */
    CSL_FINST (CSL_SYSCTRL_REGS->PCGCR2, SYS_PCGCR2_SARCG, DISABLED);

   return CSL_SOK;
}

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
 *   @li    CSL_SOK             - SAR_chanInit call is successful
 *   @li    CSL_ESYS_BADHANDLE - Invalid handler

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
)
{
    if(NULL == hSar)
    {
        return CSL_ESYS_BADHANDLE;
    }
    /* Dummy Function */
    return CSL_SOK;
}


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
CSL_SarHandleObj *SarObj,
CSL_SarChanSel   chanSel
)
{
    if(NULL == SarObj)
    {
        return CSL_ESYS_BADHANDLE;
    }

    if( chanSel < 0 || chanSel > CSL_SAR_CHAN_5 )
    {
        return CSL_ESYS_INVPARAMS;
    }

    /* Select the channel */
    CSL_FINS (CSL_SAR_REGS->SARCTRL, ANACTRL_SARCTRL_CHSEL, chanSel);
    /* Set multich discharge of array for every conversion*/
    CSL_FINST (CSL_SAR_REGS->SARCTRL, ANACTRL_SARCTRL_MULTCH, SET);

    SarObj->chanNo = chanSel;
    SarObj->baseAddr = CSL_SAR_REGS;
    SarObj->status = CSL_SAR_OPEN;

    return CSL_SOK;

}

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
 *   @li    CSL_ESYS_BADHANDLE  - Invalid handler
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

CSL_Status  SAR_chanClose (
SAR_Handle      hSar
)
{

    hSar->chanNo = CSL_SAR_NO_CHAN;
    if(NULL == hSar)
    {
        return CSL_ESYS_BADHANDLE;
    }

    // stop conversion
    if(CSL_SAR_ADC_START ==
    CSL_FEXT (hSar->baseAddr->SARCTRL,ANACTRL_SARCTRL_ADCSTRT) )
    {

        /* Stop SAR A/D conversion */
        CSL_FINST (hSar->baseAddr->SARCTRL,ANACTRL_SARCTRL_ADCSTRT,
            CLEAR);
    }

    /* Power Down the Analog */
    CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_SARPWRUP,
        CLEAR);
    /* Power down the current bias circuit */
    CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_PWRUPBIAS,
        CLEAR);

    hSar->baseAddr = NULL ;
    hSar->status = CSL_SAR_CLOSE;
    return CSL_SOK;

}

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
 *   @li    CSL_SOK             - Config call is successful
 *   @li    CSL_ESYS_BADHANDLE - SAR obj structure or configParam is NULL
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
SAR_Handle                      hSar,
CSL_SarChConfig             *pConfigParam
)
{
    if(NULL == hSar )
    {
        return CSL_ESYS_BADHANDLE;
    }

    if( NULL == pConfigParam )
    {
        return CSL_ESYS_INVPARAMS;
    }

    /* Configuring SAR A/D Control Register */
    hSar->baseAddr->SARCTRL = ( pConfigParam->SARCTRL &
                    ~((Uint16)CSL_ANACTRL_SARCTRL_ADCSTRT_MASK));

    /* Configuring SAR A/D Clock Control Register */
    hSar->baseAddr->SARCLKCTRL = pConfigParam->SARCLKCTRL ;

    /* For configuring SAR A/D Reference and Pin Register */
    hSar->baseAddr->SARPINCTRL = ( pConfigParam->SARPINCTRL
                    | CSL_ANACTRL_SARPINCTRL_SARPWRUP_MASK
                    | CSL_ANACTRL_SARPINCTRL_PWRUPBIAS_MASK );

    /* Configuring SAR A/D GPO Control Register */
    hSar->baseAddr->SARGPOCTRL = pConfigParam->SARGPOCTRL;

    return CSL_SOK;
}

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
 *   @li    CSL_ESYS_BADHANDLE - SAR obj structure or configParam is NULL
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
SAR_Handle                  hSar,
CSL_SarChConfig             *pConfigParam
)
{
    if(NULL == hSar )
    {
        return CSL_ESYS_BADHANDLE;
    }

    if ( NULL == pConfigParam )
    {
        return CSL_ESYS_INVPARAMS;
    }
    /* Get SAR A/D Control Register values */
    pConfigParam->SARCTRL =  hSar->baseAddr->SARCTRL ;

    /* Get SAR A/D Clock Control Register values */
    pConfigParam->SARCLKCTRL = hSar->baseAddr->SARCLKCTRL ;

    /* Get SAR A/D Reference and Pin Register values */
    pConfigParam->SARPINCTRL = hSar->baseAddr->SARPINCTRL ;

    /* Get SAR A/D GPO Control Register values */
    pConfigParam->SARGPOCTRL = hSar->baseAddr->SARGPOCTRL ;

    return CSL_SOK;
}

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
 *   @li    CSL_ESYS_BADHANDLE - SAR obj structure or SarParam is NULL
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

CSL_Status  SAR_chanSetup(
SAR_Handle       hSar,
CSL_SarChSetup *pSarParam
)
{
    Uint16 clkDivider=0;
    if(NULL == hSar )
    {
        return CSL_ESYS_BADHANDLE;
    }

    if (NULL == pSarParam)
    {
        return CSL_ESYS_INVPARAMS;
    }

    clkDivider = pSarParam->SysClkDiv ;
    if(clkDivider > CSL_SAR_MAX_CLKDIV )
    {
         return CSL_ESYS_INVPARAMS;
    }

    /* Reset the SAR clock control Register */
    hSar->baseAddr->SARCLKCTRL = CSL_ANACTRL_SARCLKCTRL_RESETVAL;
    /* Configuring ClkDivider value */
    CSL_FINS (hSar->baseAddr->SARCLKCTRL, ANACTRL_SARCLKCTRL_ADCCLKDIV,
           clkDivider);
    /* Enabling PwrUp SAR A/D module */
    CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_SARPWRUP,SET );
    /* Enabling PwrUpBias SAR A/D module */
    CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_PWRUPBIAS,SET );
    /* Disabling SAR A/D AVddMeas bit */
    CSL_FINST (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_AVDDMEAS,CLEAR );

    /* To configure the status bit in SAR A/D Reference and Pin Register */
    switch (pSarParam->OpMode)
    {

    case CSL_SAR_POLLING:

        CSL_FINST (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_STATUSMASK ,
            CLEAR);
        break;

    case CSL_SAR_INTERRUPT:
        /* Masking ADC and chan Select bit in pin and Reference
        control Regsiter */
        CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_STATUSMASK,
            SET);
        break;

    case CSL_SAR_DMA:
        /* Masking ADC and chan Select bit in pin and Reference
        control Regsiter */
        CSL_FINST (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_STATUSMASK,
            SET);

        break;

    default:
    return CSL_ESYS_INVPARAMS;

    }

    if( CSL_SAR_REF_VIN != pSarParam->RefVoltage
    && CSL_SAR_REF_0_8v != pSarParam->RefVoltage
    && CSL_SAR_REF_1v != pSarParam->RefVoltage)
    {
        return CSL_ESYS_INVPARAMS;
    }
    /* Selection of Analog Reference voltage */
    if( CSL_SAR_REF_VIN == pSarParam->RefVoltage )
    {
        /* Set Reference volatge to VDD_SAR */
        CSL_FINST (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_REFAVDDSEL,
            SET);

        /* Disable the Reference buffer */
        CSL_FINST (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_REFBUFFEN,
            CLEAR);

    }
    else
    {
        /* Set Reference volatge based on bandgap voltage */
        CSL_FINST (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_REFAVDDSEL,
            CLEAR);
        /* Enable the Reference buffer */
        CSL_FINST (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_REFBUFFEN,
            SET);
        /* Set bandgap voltage  */
        CSL_FINS (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_REFLVSEL,
            pSarParam->RefVoltage);
    }


    /* Configuring SAR A/D GndSwOn bit */
    CSL_FINST (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_GNDON, CLEAR);
    /* Configuring SAR A/D Half bit */
    CSL_FINST (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_HALF, CLEAR);

    /* Configuring MultiCh operation */
    if( pSarParam->MultiCh != CSL_SAR_NO_DISCHARGE
    && pSarParam->MultiCh != CSL_SAR_DISCHARGE_ARRAY )
    {
        return CSL_ESYS_INVPARAMS;
    }

    CSL_FINS (hSar->baseAddr->SARCTRL ,ANACTRL_SARCTRL_MULTCH,
              pSarParam->MultiCh);

    return CSL_SOK;
}

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
 *   @li    CSL_ESYS_BADHANDLE  - Invalid handle
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
SAR_Handle           hSar,
CSL_SarChanCyc       cycSelect
)

{
    if(NULL == hSar)
    {
        return CSL_ESYS_BADHANDLE;
    }

    if( (CSL_SAR_CONTINUOUS_CONVERSION != cycSelect
    && CSL_SAR_SINGLE_CONVERSION != cycSelect))
    {
        return CSL_ESYS_INVPARAMS;
    }
    /* Configuring SAR singleCyc operation */
    CSL_FINS (hSar->baseAddr->SARCTRL, ANACTRL_SARCTRL_SNGCYC, cycSelect);

    return CSL_SOK;
}


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
 *   @li    CSL_ESYS_BADHANDLE - Invalid handle
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
SAR_Handle            hSar
)
{
    if(NULL == hSar)
    {
        return CSL_ESYS_BADHANDLE;
    }
    /* Start SAR A/D conversion */
    CSL_FINST (hSar->baseAddr->SARCTRL, ANACTRL_SARCTRL_ADCSTRT, SET);
    return CSL_SOK;
}

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
 *   @li  CSL_ESYS_BADHANDLE - Invalid handle
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
SAR_Handle             hSar
)

{
    if(NULL == hSar)
    {
        return CSL_ESYS_BADHANDLE;
    }
    /* Check whether conversion start bit is set or not */
    if(CSL_SAR_ADC_START ==
    CSL_FEXT (hSar->baseAddr->SARCTRL, ANACTRL_SARCTRL_ADCSTRT) )
    {
        /* Stop SAR A/D conversion */
        CSL_FINST (hSar->baseAddr->SARCTRL, ANACTRL_SARCTRL_ADCSTRT, CLEAR);
    }

    return CSL_SOK;
    }


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
 *   @li    CSL_ESYS_BADHANDLE - Invalid handler
 *
 *   <b> Pre Condition </b>
 *   @n  SAR_A2DMeasParamSet or (SAR_chanCoonfig API ) and SAR_startConversion
 *       must be called before this API
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
            status = SAR_readData(hSar, &readData);
     @endverbatim
 *  ============================================================================
 */

CSL_Status SAR_readData (
SAR_Handle           hSar,
Uint16               *readData
)

{
    if(NULL == hSar)
    {
        return CSL_ESYS_BADHANDLE;
    }
    if(NULL == readData)
    {
        return CSL_ESYS_INVPARAMS;
    }
    *readData = CSL_FEXT (hSar->baseAddr->SARDATA, ANACTRL_SARDATA_ADCDAT);
    return CSL_SOK;
}


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
 *   @li    CSL_ESYS_BADHANDLE - Invalid handler
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
SAR_Handle                   hSar,
CSL_SARMeasParam             param,
CSL_SarChanSel*              chanNo
)
{

    CSL_Status status;
    if(NULL == hSar)
    {
        return CSL_ESYS_BADHANDLE;
    }

    switch(param)
    {
        case CSL_KEYPAD_MEAS:
        {
            *chanNo = CSL_SAR_CHAN_3;
            CSL_FINST (CSL_SAR_REGS->SARCTRL, ANACTRL_SARCTRL_CHSEL, AIN3);
            /* Enabling SAR A/D AVddMeas bit */
            CSL_FINST (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_AVDDMEAS,
                CLEAR);

            /* Disabling SAR A/D GndSwOn bit */
            CSL_FINST (hSar->baseAddr->SARPINCTRL ,ANACTRL_SARPINCTRL_GNDON,
                CLEAR);
            /* Disabling SAR A/D Half bit */
            CSL_FINST (hSar->baseAddr->SARPINCTRL, ANACTRL_SARPINCTRL_HALF,
                CLEAR);

            status = CSL_SOK;
            break;
        }

        case CSL_BATTRY_MEAS:
        {
            *chanNo = CSL_SAR_CHAN_1;
            CSL_FINST (CSL_SAR_REGS->SARCTRL,ANACTRL_SARCTRL_CHSEL,AIN1);
            /* Disable GPO0En pin */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO0EN,
                CLEAR);

            /* Channel 0 is grounded */
            CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_GNDON,SET);

            /* Input the VIn voltage without dividing by 2 */
        CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_HALF,CLEAR);

            /* Reference voltage is based on bandgap voltage */
            CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_REFAVDDSEL,
                CLEAR);
            status = CSL_SOK;
            break;
        }
        case CSL_INT_VOLT_MEAS:
        {
            *chanNo = CSL_SAR_CHAN_3;
            CSL_FINST (CSL_SAR_REGS->SARCTRL,ANACTRL_SARCTRL_CHSEL,AIN3);

            /* Disable GPAIN1 output driver  */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO1EN,
                CLEAR);
            /* Enable Analog voltage Meas on Channel 3 */
            CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_AVDDMEAS,
                SET);
            /* Disable PenIrqEn */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_PENIRQEN,
                CLEAR);
            /* Divide the input voltage by 2 */
            CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_HALF,
                SET);
            /* Disable Bandgap based Reference voltage */
            CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_REFAVDDSEL,
            CLEAR);

            status = CSL_SOK;
            break;
        }

        case CSL_VOLUME_CTRL:
        {
            *chanNo = CSL_SAR_CHAN_5;
            CSL_FINST (CSL_SAR_REGS->SARCTRL, ANACTRL_SARCTRL_CHSEL, AIN5);

            /* Disable GPAIN3 outpin driver */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO3EN,
                CLEAR);

            /* Use GPO2En as General Purpose Output Pin */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO2EN,
                SET);

            /* Drive GPO2Data low means GPO2 grounded */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO2,
                CLEAR);

            /* Directly Fed Vin voltage to ADC */
            CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_HALF,
                CLEAR);

            /* Bandgap base Reference voltage */
            CSL_FINST (hSar->baseAddr->SARPINCTRL,ANACTRL_SARPINCTRL_REFAVDDSEL,
            CLEAR);

            status = CSL_SOK;
            break;
        }

        case CSL_TOUCH_SCREEN_XAXIS:
        {
            *chanNo = CSL_SAR_CHAN_5;
            CSL_FINST (CSL_SAR_REGS->SARCTRL,ANACTRL_SARCTRL_CHSEL,AIN5);

            /* Enable Touch screen mode */
            CSL_FINST (hSar->baseAddr->SARPINCTRL,
                ANACTRL_SARPINCTRL_TOUCHSCREENMODE,SET);

            /* Disable GPAIN3 output drive  */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO3EN,
                CLEAR);

            /* Disable GPAIN2 output drive  */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO2EN,
                CLEAR);


            /* Enable GPAIN1 as general purpose output */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO1EN,
                SET);
            /* Drive GPAIN1 pin as high */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO1,
                SET);

            /* Enable GPAIN0 as general purpose output */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO0EN,
                SET);
            /* Drive GPAIN0 pin as high */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO0,
                CLEAR);

            /* Disable PenIrqEn */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_PENIRQEN,
                CLEAR);


            status = CSL_SOK;
            break;
        }

        case CSL_TOUCH_SCREEN_YAXIS:
        {
            *chanNo = CSL_SAR_CHAN_3;
            CSL_FINST (CSL_SAR_REGS->SARCTRL,ANACTRL_SARCTRL_CHSEL,AIN3);

            /* Enable Touch screen mode */
            CSL_FINST (hSar->baseAddr->SARPINCTRL,
                ANACTRL_SARPINCTRL_TOUCHSCREENMODE,SET);

            /* Enable GPAIN3 as general purpose output */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO3EN,
                SET);
            /* Drive GPAIN3 pin as high */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO3,SET);

            /* Enable GPAIN2 as general purpose output */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO2EN,
                SET);
            /* Make GPAIN2 pin grounded  */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO2,
            CLEAR);

            /* Disable GPAIN1 output drive */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO1EN,
                CLEAR);
            /* Disable GPAIN0 output drive */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO0EN,
                CLEAR);

            /* Disable PenIrqEn */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_PENIRQEN,
                CLEAR);

            status = CSL_SOK;
            break;
        }


        case CSL_PEN_PRESS_DOWN:
        {

            *chanNo = CSL_SAR_CHAN_5;
            CSL_FINST (CSL_SAR_REGS->SARCTRL,ANACTRL_SARCTRL_CHSEL,AIN5);

            /* Disable GPAIN3 output drive  */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO3EN,
                CLEAR);

            /* Enable GPAIN2 as general purpose output */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO2EN,
                SET);
            /* Make GPAIN2 pin grounded  */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO2,
                CLEAR);

            /* Disable GPAIN1 output drive */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO1EN,
                CLEAR);
            /* Disable GPAIN0 output drive */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO0EN,
                CLEAR);

            /* Enaable PenIrqEn */
            CSL_FINST (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_PENIRQEN,
                SET);
            status = CSL_SOK;
            break;
        }

        default:
        status = CSL_ESYS_INVPARAMS;
        break;
    }
    return status;
}

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
)
{
    if(NULL == hSar)
    {
        return CSL_ESYS_BADHANDLE;
    }

    if(CSL_SAR_GPO_IN != dir && CSL_SAR_GPO_OUT != dir)
    {
        return CSL_ESYS_INVPARAMS;
    }
    switch(Index)
    {
        case CSL_SAR_GPO_0:

        CSL_FINS (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO0EN, dir);
        break;

        case CSL_SAR_GPO_1:

        CSL_FINS (hSar->baseAddr->SARGPOCTRL,ANACTRL_SARGPOCTRL_GPO1EN, dir);
        break;

        case CSL_SAR_GPO_2:

        CSL_FINS (hSar->baseAddr->SARGPOCTRL, ANACTRL_SARGPOCTRL_GPO2EN, dir);
        break;

        case CSL_SAR_GPO_3:

        CSL_FINS (hSar->baseAddr->SARGPOCTRL, ANACTRL_SARGPOCTRL_GPO3EN, dir);
        break;

        default:
        return CSL_ESYS_INVPARAMS;

    }
    return CSL_SOK;
}

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
)
{
    volatile Bool busyBit = CSL_SAR_ADC_BUSY ;
    if(NULL == hSar)
    {
        *status = CSL_ESYS_BADHANDLE;
    }
    busyBit = CSL_FEXT (hSar->baseAddr->SARDATA, ANACTRL_SARDATA_ADCBSY) ;
    *status = CSL_SOK;
    return busyBit;
}
