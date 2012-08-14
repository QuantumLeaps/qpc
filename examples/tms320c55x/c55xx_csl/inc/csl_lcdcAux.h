/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_lcdcAux.h
 *
 *  @brief LCDC functional layer Auxilliary header file
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 05-Sept-2008 Added LCDC of CSL.
 * 06-May-2009 Removed the Raster mode support for C5505 PG1.4
 * 26-May-2009 Modified based on the review comments
 * ============================================================================
 */

#ifndef _CSL_LCDCAUX_H_
#define _CSL_LCDCAUX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <csl_lcdc.h>

/*****************************************************************************\
        CSL LCDC  mandatory inline functions
\*****************************************************************************/

/** @addtogroup CSL_LCDC_FUNCTION
 @{ */

/** ===========================================================================
 *   @n@b CSL_lcdcEnableLiddDMA
 *
 *   @b Description
 *   @n This function enables the DMA Transfer for LIDD Controller.
 *
 *   @b Arguments
 *   @verbatim

            hLcdc            Handle to LCDC instance

     @endverbatim
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n 1: LCD_open() API should be called before this.
 *   @n 2: LCD_setup() should be called for making use of LIDD controller.
 *   @n 4: LCD_configDMA() should be called to make sure the DMA setup.
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

        CSL_LcdcHandle       hLcdc;

        CSL_lcdcEnableLiddDMA (hLcdc);

     @endverbatim
 * ===========================================================================
 */
static inline
void CSL_lcdcEnableLiddDMA (
    /** Pointer to the object that holds reference to the
     *  instance of LCDC requested after the call
     */
    CSL_LcdcHandle        hLcdc
)
{
    CSL_FINST(hLcdc->regs->LCDLIDDCR, LCDC_LCDLIDDCR_LIDD_DMA_EN, ENABLE);
}


/** ===========================================================================
 *   @n@b CSL_lcdcDisableLiddDMA
 *
 *   @b Description
 *   @n This function disables the DMA Transfer for LIDD Controller.
 *
 *   @b Arguments
 *   @verbatim

            hLcdc            Handle to LCDC instance

     @endverbatim
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

        CSL_LcdcHandle       hLcdc;

        CSL_lcdcDisableLiddDMA (hLcdc);

     @endverbatim
 * ===========================================================================
 */
static inline
void CSL_lcdcDisableLiddDMA (
    /** Pointer to the object that holds reference to the
     *  instance of LCDC requested after the call
     */
    CSL_LcdcHandle        hLcdc
)
{
    CSL_FINST(hLcdc->regs->LCDLIDDCR, LCDC_LCDLIDDCR_LIDD_DMA_EN, DISABLE);
}

/** ===========================================================================
 *   @n@b CSL_lcdcLiddWriteCsAddr
 *
 *   @b Description
 *   @n This function sets the LIDD CS 0 or 1 Address Read/Write Register.
 *
 *   @b Arguments
 *   @verbatim

            hLcdc            Handle to LCDC instance
            cs0cs1           chip select 0 or 1
            arg              variable to hold the value

     @endverbatim
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

        CSL_lcdcLiddWriteCsAddr (hLcdc, cs0cs1, arg);

     @endverbatim
 * ===========================================================================
 */
static inline
void CSL_lcdcLiddWriteCsAddr (
    /** Pointer to the object that holds reference to the
     *  instance of LCDC requested after the call
     */
    CSL_LcdcHandle        hLcdc,
    CSL_LcdcChipSelect    cs0cs1,
    Uint32                arg
)
{
    if (CSL_LCDC_LIDD_CS0 == cs0cs1 )
    {
        CSL_FINS (hLcdc->regs->LCDLIDDCS0ADDR,
                               LCDC_LCDLIDDCS0ADDR_ADR_INDX, arg);
    }
    else
    {
        CSL_FINS (hLcdc->regs->LCDLIDDCS1ADDR,
                               LCDC_LCDLIDDCS1ADDR_ADR_INDX, arg);
    }
}


/** ===========================================================================
 *   @n@b CSL_lcdcLiddWriteCsData
 *
 *   @b Description
 *   @n This function sets the LIDD CS 0 or 1 Data Read/Write Register.
 *
 *   @b Arguments
 *   @verbatim

            hLcdc            Handle to LCDC instance
            cs0cs1           chip select 0 or 1
            arg              variable to hold the value

     @endverbatim
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

        CSL_lcdcLiddWriteCsData (hLcdc, cs0cs1, arg);

     @endverbatim
 * ===========================================================================
 */
static inline
void CSL_lcdcLiddWriteCsData (
    /** Pointer to the object that holds reference to the
     *  instance of LCDC requested after the call
     */
    CSL_LcdcHandle        hLcdc,
    CSL_LcdcChipSelect    cs0cs1,
    Uint16                arg
)
{
    if (CSL_LCDC_LIDD_CS0 == cs0cs1 )
    {
        CSL_FINS (hLcdc->regs->LCDLIDDCS0DATA,
                               LCDC_LCDLIDDCS0DATA_DATA, arg);
    }
    else
    {
        CSL_FINS (hLcdc->regs->LCDLIDDCS1DATA,
                               LCDC_LCDLIDDCS1DATA_DATA, arg);
    }
}

/** ===========================================================================
 *   @n@b CSL_lcdcGetLiddCsData
 *
 *   @b Description
 *   @n This function gets the of LIDD CS0or1 DATA Register value.
 *
 *   @b Arguments
 *   @verbatim

            hLcdc            Handle to LCDC instance
            cs0cs1           chip select 0 or 1

     @endverbatim
 *
 *   <b> Return Value </b>  Uint16
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

        Uint16     status;

        status = CSL_lcdcGetLiddCsData (hLcdc, cs0cs1);

     @endverbatim
 * ===========================================================================
 */
static inline
Uint16 CSL_lcdcGetLiddCsData (
    /** Pointer to the object that holds reference to the
     *  instance of LCDC requested after the call
     */
    CSL_LcdcHandle        hLcdc,
    CSL_LcdcChipSelect    cs0cs1
)
{
    if (CSL_LCDC_LIDD_CS0 == cs0cs1 )
    {
        return (hLcdc->regs->LCDLIDDCS0DATA);
    }
    else
    {
        return (hLcdc->regs->LCDLIDDCS1DATA);
    }
}


/** ===========================================================================
 *   @n@b CSL_lcdcGetLiddCsAddr
 *
 *   @b Description
 *   @n This function gets the of LIDD CS0or1 Address Register value.
 *
 *   @b Arguments
 *   @verbatim

            hLcdc           Handle to LCDC instance
            cs0cs1           chip select 0 or 1

     @endverbatim
 *
 *   <b> Return Value </b>  Uint16
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

        Uint16     status;

        status = CSL_lcdcGetLiddCsAddr (hLcdc, cs0cs1, &response);

     @endverbatim
 * ===========================================================================
 */
static inline
Uint16 CSL_lcdcGetLiddCsAddr (
    /** Pointer to the object that holds reference to the
     *  instance of LCDC requested after the call
     */
    CSL_LcdcHandle        hLcdc,
    CSL_LcdcChipSelect    cs0cs1
)
{
    if (CSL_LCDC_LIDD_CS0 == cs0cs1 )
    {
        return (hLcdc->regs->LCDLIDDCS0ADDR);
    }
    else
    {
        return (hLcdc->regs->LCDLIDDCS1ADDR);
    }
}

/** ===========================================================================
 *   @n@b CSL_lcdcGetVersion
 *
 *   @b Description
 *   @n This function gets the Version of the LCDC module.
 *
 *   @b Arguments
 *   @verbatim

            hLcdc           Handle to LCDC instance
            responseMinor   Placeholder to return status of minor revision.
            responseMajor   Placeholder to return status of major revision.

     @endverbatim
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

        CSL_lcdcGetVersion (hLcdc, &responseMinor, &responseMajor);

     @endverbatim
 * ===========================================================================
 */
static inline
void CSL_lcdcGetVersion (
    /** Pointer to the object that holds reference to the
     *  instance of LCDC requested after the call
     */
    CSL_LcdcHandle        hLcdc,
    /** Placeholder to return the status. @a void* casted */
    void                 *responseMinor,
    void                 *responseMajor
)
{
    *(Uint16*)responseMinor = CSL_LCDC_REGS->LCDREVMIN;
    *(Uint16*)responseMajor = CSL_LCDC_REGS->LCDREVMAJ;
}

/** ===========================================================================
 *   @n@b LCD_getSetup
 *
 *   @b Description
 *   @n It gets the configuration details from the LCDC Registers.
 *
 *   @b Arguments
 *   @verbatim
            hLcdc            Handle to the LCDC.
            getSetup         Pointer to DMA setup structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - setup Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim
            CSL_LcdcHandle      hLcdc;
            CSL_LcdcHwSetup     getSetup;
            CSL_status          status;

NOTE: for setup parameter refer the function prologue of LCD_setup() function.

            status = LCD_getSetup(hLcdc, &getSetup);
            ...

     @endverbatim
 *  ===========================================================================
 */
static inline
CSL_Status LCD_getSetup (
               CSL_LcdcHandle         hLcdc,
               CSL_LcdcHwSetup*       getSetup  )
{
    Uint16    tempVar;

    if((hLcdc !=  NULL) && (getSetup != NULL))
    {
        /* passed parameters are ok */
    }
    else
    {
        if(hLcdc ==  NULL)
        {
            return CSL_ESYS_BADHANDLE;
        }
        else
        {
            return CSL_ESYS_INVPARAMS;
        }
    }

    /* Get configured information  for LIDD controller */
    (getSetup->config).fdoneIntEn = (CSL_LcdcFdoneCtl)
       CSL_FEXT (hLcdc->regs->LCDLIDDCR, LCDC_LCDLIDDCR_DONE_INT_EN);

    (getSetup->config).modeSel = (CSL_LcdcLiddMode)
       CSL_FEXT (hLcdc->regs->LCDLIDDCR, LCDC_LCDLIDDCR_LIDD_MODE_SEL);

    (getSetup->config).clkDiv =
       CSL_FEXT (hLcdc->regs->LCDCR, LCDC_LCDCR_CLKDIV);

    (getSetup->config).dmaCs0Cs1 = (CSL_LcdcChipSelect)
       CSL_FEXT (hLcdc->regs->LCDLIDDCR, LCDC_LCDLIDDCR_DMA_CS0_CS1);

    tempVar = (hLcdc->regs->LCDLIDDCR) &
                         CSL_LCD_LCDLIDDCR_POLARITY_CONTROL_MASK;
    tempVar = tempVar >> CSL_LCDC_LCDLIDDCR_ALEPOL_SHIFT;
    (getSetup->config).polControl = tempVar;

    (getSetup->config).dmaEnable = (CSL_LcdcDmaCtl)
       CSL_FEXT (hLcdc->regs->LCDLIDDCR, LCDC_LCDLIDDCR_LIDD_DMA_EN);

    /* Get configured LIDD chip selct configuration register Fields */
    if(CSL_LCDC_LIDD_NOT_USE_CS1 == (getSetup->useCs1))
    {
        /* Get configured LIDD CS0 configuration register fields*/
        (getSetup->timingCs0).rStrobe =
                       CSL_FEXT (hLcdc->regs->LCDLIDDCS0CONFIG0,
                                        LCDC_LCDLIDDCS0CONFIG0_R_STROBE);

        (getSetup->timingCs0).rHold =
                       CSL_FEXT (hLcdc->regs->LCDLIDDCS0CONFIG0,
                                        LCDC_LCDLIDDCS0CONFIG0_R_HOLD);

        (getSetup->timingCs0).ta =
                      CSL_FEXT (hLcdc->regs->LCDLIDDCS0CONFIG0,
                                        LCDC_LCDLIDDCS0CONFIG0_TA);

        tempVar = CSL_FEXT (hLcdc->regs->LCDLIDDCS0CONFIG1,
                                 LCDC_LCDLIDDCS0CONFIG1_R_SU1);
        (getSetup->timingCs0).rSu = (tempVar << 4) +
                             CSL_FEXT (hLcdc->regs->LCDLIDDCS0CONFIG0,
                                           LCDC_LCDLIDDCS0CONFIG0_R_SU0);

        (getSetup->timingCs0).wSu =
                           CSL_FEXT (hLcdc->regs->LCDLIDDCS0CONFIG1,
                                           LCDC_LCDLIDDCS0CONFIG1_W_SU);

        (getSetup->timingCs0).wStrobe =
                           CSL_FEXT (hLcdc->regs->LCDLIDDCS0CONFIG1,
                                       LCDC_LCDLIDDCS0CONFIG1_W_STROBE);

        (getSetup->timingCs0).wHold =
                           CSL_FEXT (hLcdc->regs->LCDLIDDCS0CONFIG1,
                                         LCDC_LCDLIDDCS0CONFIG1_W_HOLD);
    }
    else
    {
        /* Get configured LIDD CS1 configuration register Fields */
        (getSetup->timingCs1).rStrobe =
                           CSL_FEXT (hLcdc->regs->LCDLIDDCS1CONFIG0,
                                        LCDC_LCDLIDDCS1CONFIG0_R_STROBE);

        (getSetup->timingCs1).rHold =
                           CSL_FEXT (hLcdc->regs->LCDLIDDCS1CONFIG0,
                                        LCDC_LCDLIDDCS1CONFIG0_R_HOLD);

        (getSetup->timingCs1).ta =
                          CSL_FEXT (hLcdc->regs->LCDLIDDCS1CONFIG0,
                                            LCDC_LCDLIDDCS1CONFIG0_TA);

        tempVar = CSL_FEXT (hLcdc->regs->LCDLIDDCS1CONFIG1,
                                LCDC_LCDLIDDCS1CONFIG1_R_SU1);
        (getSetup->timingCs1).rSu = (tempVar << 4) +
                           CSL_FEXT (hLcdc->regs->LCDLIDDCS1CONFIG0,
                                          LCDC_LCDLIDDCS1CONFIG0_R_SU0);

        (getSetup->timingCs1).wSu =
                           CSL_FEXT (hLcdc->regs->LCDLIDDCS1CONFIG1,
                                          LCDC_LCDLIDDCS1CONFIG1_W_SU);

        (getSetup->timingCs1).wStrobe =
                           CSL_FEXT (hLcdc->regs->LCDLIDDCS1CONFIG1,
                                      LCDC_LCDLIDDCS1CONFIG1_W_STROBE);

        (getSetup->timingCs1).wHold =
                           CSL_FEXT (hLcdc->regs->LCDLIDDCS1CONFIG1,
                                      LCDC_LCDLIDDCS1CONFIG1_W_HOLD);
    }

    return CSL_SOK;
}

/** ===========================================================================
 *   @n@b LCD_getConfigDMA
 *
 *   @b Description
 *   @n It get configured value from LCD registers of DMA Purpose.
 *
 *   @b Arguments
 *   @verbatim
            hLcdc            Handle to the LCDC.
            pgetConfigDma    Pointer to DMA Config structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Congig Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim
            CSL_LcdcHandle      hLcdc;
            CSL_LcdcConfigDma   pgetConfigDma;
            CSL_status          status;

NOTE: for setup parameter refer the function prologue of LCD_configDMA() function.

            status = LCD_getConfigDMA(hLcdc, &pgetConfigDma);
            ...

     @endverbatim
 *  ===========================================================================
 */
static inline
CSL_Status LCD_getConfigDMA (
               CSL_LcdcHandle         hLcdc,
               CSL_LcdcConfigDma*     pgetConfigDma )
{
    Uint32      tmpAddr;
    Uint16      tmpAddrLSB;
    Uint16      tmpAddrMSB;

    if((hLcdc !=  NULL) && (pgetConfigDma != NULL))
    {
        /* passed parameters are ok */
    }
    else
    {
        if(hLcdc ==  NULL)
        {
            return CSL_ESYS_BADHANDLE;
        }
        else
        {
            return CSL_ESYS_INVPARAMS;
        }
    }

    pgetConfigDma->burstSize = (CSL_LcdcDmaBurst)
              CSL_FEXT (hLcdc->regs->LCDDMACR, LCDC_LCDDMACR_BURST_SIZE);

    pgetConfigDma->eofIntEn = (CSL_LcdcEofIntCtl)
              CSL_FEXT (hLcdc->regs->LCDDMACR, LCDC_LCDDMACR_EOF_INTEN);

    pgetConfigDma->bigEndian = (CSL_LcdcEndianess)
              CSL_FEXT (hLcdc->regs->LCDDMACR, LCDC_LCDDMACR_BIGENDIAN);

    pgetConfigDma->frameMode = (CSL_LcdcFrameMode)
              CSL_FEXT (hLcdc->regs->LCDDMACR, LCDC_LCDDMACR_FRAME_MODE);

    /* DMA frame buffer 0 Base Address Set */
    tmpAddrLSB = hLcdc->regs->LCDDMAFB0BAR0;
    tmpAddrMSB = hLcdc->regs->LCDDMAFB0BAR1;
    tmpAddr = tmpAddrMSB;
    tmpAddr = tmpAddr << CSL_LCD_UINT16_NUMBER_BITS;
    tmpAddr |= tmpAddrLSB;
    tmpAddr  = ((tmpAddr - CSL_LCD_SARAM_DMA_ADDR_OFFSET) >> CSL_LCD_DMA_SHIFT);
    pgetConfigDma->fb0Base = tmpAddr;

    /* DMA frame buffer 0 Ceil Address Set*/
    tmpAddrLSB = hLcdc->regs->LCDDMAFB0CAR0;
    tmpAddrMSB = hLcdc->regs->LCDDMAFB0CAR1;
    tmpAddr = tmpAddrMSB;
    tmpAddr = tmpAddr << CSL_LCD_UINT16_NUMBER_BITS;
    tmpAddr |= tmpAddrLSB;
    tmpAddr  = ((tmpAddr - CSL_LCD_SARAM_DMA_ADDR_OFFSET) >> CSL_LCD_DMA_SHIFT);
    pgetConfigDma->fb0Ceil = tmpAddr;

    /* DMA frame buffer 1 Base Address Set */
    tmpAddrLSB = hLcdc->regs->LCDDMAFB1BAR0;
    tmpAddrMSB = hLcdc->regs->LCDDMAFB1BAR1;
    tmpAddr = tmpAddrMSB;
    tmpAddr = tmpAddr << CSL_LCD_UINT16_NUMBER_BITS;
    tmpAddr |= tmpAddrLSB;
    tmpAddr  = ((tmpAddr - CSL_LCD_SARAM_DMA_ADDR_OFFSET) >> CSL_LCD_DMA_SHIFT);
    pgetConfigDma->fb1Base = tmpAddr;

    /* DMA frame buffer 1 Ceil Address Set*/
    tmpAddrLSB = hLcdc->regs->LCDDMAFB1CAR0;
    tmpAddrMSB = hLcdc->regs->LCDDMAFB1CAR1;
    tmpAddr = tmpAddrMSB;
    tmpAddr = tmpAddr << CSL_LCD_UINT16_NUMBER_BITS;
    tmpAddr |= tmpAddrLSB;
    tmpAddr  = ((tmpAddr - CSL_LCD_SARAM_DMA_ADDR_OFFSET) >> CSL_LCD_DMA_SHIFT);
    pgetConfigDma->fb1Ceil = tmpAddr;

    return CSL_SOK;
}

/**
@} */

#endif    //_CSL_LCDCAUX_H_
