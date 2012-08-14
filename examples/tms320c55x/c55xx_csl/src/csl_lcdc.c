/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_lcdc.c
 *
 *  @brief LCD functional layer API defenition file
 *
 *  Path: \(CSLPATH)/src
 */

/* ============================================================================
 * Revision History
 * ================
 * 05-Sept-2008 Added LCDC of CSL.
 * 06-May-2009 Removed the Raster mode support for C5505 PG1.4
 * 26-May-2009 Modified based on the review comments
 * ============================================================================
 */

#include "csl_lcdc.h"
#include "csl_lcdcAux.h"

/** ===========================================================================
 *   @n@b LCD_init
 *
 *   @b Description
 *   @n This is the initialization function for the LCD CSL. The function
 *      must be called before calling any other API from this CSL. This
 *      function is for enabling the clock to the LCD Controller.
 *      Currently, the function just return status CSL_SOK. as of now no
 *      checking is done inside the definition of function , but in near
 *      future it can be implemented.
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                        CSL_SOK - Always returns
 *
 *   <b> Pre Condition </b>
 *   @n  This must be first API to use LCDC module.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_status       status;

             status = LCD_init();

     @endverbatim
 *  ===========================================================================
 */
CSL_Status LCD_init(void)
{
    /*enable the corresponding LCD clock from PCGCR Registers*/
    CSL_FINST(CSL_SYSCTRL_REGS->PCGCR2, SYS_PCGCR2_LCDCG, ACTIVE);
    /* set the reset clock cycle */
    CSL_FINS(CSL_SYSCTRL_REGS->PSRCR,
                               SYS_PSRCR_COUNT, CSL_LCD_RESET_CLOCK_CYCLE);
    CSL_FINST(CSL_SYSCTRL_REGS->PRCR, SYS_PRCR_PG4_RST, RST);

    return CSL_SOK;
}

/** ===========================================================================
 *   @n@b LCD_open
 *
 *   @b Description
 *   @n This function returns the handle to the LCDC.
 *      and handle is passed to all other CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            instanceNum   instance number for LCDC
            pLcdcObj      pointer to LCDC object.
            status        Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_LcdcHandle
 *   @n                     Valid LCDC handle will be returned if
 *                          status value is equal to CSL_SOK.
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_init() API should be called before LCD_open().
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is returned in the status variable. If status
 *              returned is
 *   @li            CSL_SOK            - Valid LCDC handle is returned
 *   @li            CSL_ESYS_INVPARAMS  - lcdc object pointer is invalid.
 *   @n   2.    CSL_LcdcObj object structure is populated
 *
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. CSL_LcdcObj object structure
 *
 *   @b Example
 *   @verbatim

            Uint16               instanceNum;
            CSL_status           status;
            CSL_LcdcObj          pLcdcObj
            CSL_LcdcHandle       hLcdc;
            ...
            instanceNum = CSL_LCDC_INSTANCE_0
            hLcdc = LCD_open(instanceNum, &pLcdcObj, &status);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_LcdcHandle LCD_open (
                   Uint16       instanceNum,
                   CSL_LcdcObj  *pLcdcObj,
                   CSL_Status   *status     )
{
    CSL_LcdcHandle    hLcdc;

    if((instanceNum < CSL_LCDC_PER_CNT) && (pLcdcObj != NULL))
    {
        pLcdcObj->perNum = instanceNum;
        pLcdcObj->regs   = CSL_LCDC_REGS;
        hLcdc            = pLcdcObj;
        *status          = CSL_SOK;
    }
    else
    {
        hLcdc   = NULL;
        *status = CSL_ESYS_INVPARAMS;
    }
    return hLcdc;
}

/** ===========================================================================
 *   @n@b LCD_setup
 *
 *   @b Description
 *   @n It configures the LCDC register specific to LIDD controller to draw
 *      the images or text on LCD panel.
 *
 *   @b Arguments
 *   @verbatim
            hLcdc            Handle to the LCDC.
            setup            Pointer to LCDC setup structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - setup Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n  LCDC Registers will be populated.
 *
 *   @b Example
 *   @verbatim
            Uint16               instanceNum;
            CSL_status           status;
            CSL_LcdcObj          pLcdcObj
            CSL_LcdcHandle       hLcdc;
            CSL_LcdcHwSetup      setup;
            ...
     CSL_LcdcConfigLidd       configLIDD;
     CSL_LcdcLiddTiming       timingCs0LIDD;
     CSL_LcdcLiddTiming       timingCs1LIDD;

.........\\\\\\\\\\\\\  SETUP for LIDD Controller

    configLIDD.clkDiv      = 0x3;
    configLIDD.fdoneIntEn  = CSL_LCDC_LIDD_FDONE_DISABLE ;
    configLIDD.dmaCs0Cs1   = CSL_LCDC_LIDD_CS0;
    configLIDD.dmaEnable   = CSL_LCDC_LIDD_DMA_DISABLE;
    configLIDD.polControl  = 0x0000;
    configLIDD.modeSel     = CSL_LCDC_LIDD_ASYNC_MPU80;

    timingCs0LIDD.wSu      = 0x1F;
    timingCs0LIDD.wStrobe  = 0x3F ;
    timingCs0LIDD.wHold    = 0x0F;
    timingCs0LIDD.rSu      = 0x1F;
    timingCs0LIDD.rStrobe  = 0x3F;
    timingCs0LIDD.rHold    = 0x0F;
    timingCs0LIDD.ta       = 0x01;

    timingCs1LIDD.wSu      = 0x1F;
    timingCs1LIDD.wStrobe  = 0x3F ;
    timingCs1LIDD.wHold    = 0x0F;
    timingCs1LIDD.rSu      = 0x1F;
    timingCs1LIDD.rStrobe  = 0x3F;
    timingCs1LIDD.rHold    = 0x0F;
    timingCs1LIDD.ta       = 0x01;

    setup.config        = configLIDD;
    setup.useCs1        = CSL_LCDC_LIDD_NOT_USE_CS1;
    setup.timingCs0        = timingCs0LIDD;
    setup.extendSetup   = NULL;
||||||||||||*******************************************************||||||||||||

            instanceNum = CSL_LCDC_INSTANCE_0
            hLcdc = LCD_open(instanceNum, &pLcdcObj, &status);

            status = LCD_setup(hLcdc, &setup);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status LCD_setup (
               CSL_LcdcHandle         hLcdc,
               CSL_LcdcHwSetup*       setup  )
{
    Uint16    tempVar;

    if((hLcdc !=  NULL) && (setup != NULL))
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

    /* clear the all data of the status register */
    CSL_FINS(hLcdc->regs->LCDSR, LCDC_LCDSR_EOF1, CSL_LCD_ENABLE);
    CSL_FINS(hLcdc->regs->LCDSR, LCDC_LCDSR_EOF0, CSL_LCD_ENABLE);
    CSL_FINS(hLcdc->regs->LCDSR, LCDC_LCDSR_PL, CSL_LCD_ENABLE);
    CSL_FINS(hLcdc->regs->LCDSR, LCDC_LCDSR_FUF, CSL_LCD_ENABLE);
    CSL_FINS(hLcdc->regs->LCDSR, LCDC_LCDSR_ABC, CSL_LCD_ENABLE);
    CSL_FINS(hLcdc->regs->LCDSR, LCDC_LCDSR_SYNC, CSL_LCD_ENABLE);
    CSL_FINS(hLcdc->regs->LCDSR, LCDC_LCDSR_DONE, CSL_LCD_ENABLE);

    /* set the lcd controller mode to LIDD */
    CSL_FINST (hLcdc->regs->LCDCR, LCDC_LCDCR_MODESEL, LIDD);

        /* configure the registers required for LIDD controller */
/*         CSL_FINST (hLcdc->regs->LCDRASTCR0,
                                 LCDC_LCDRASTCR0_LCDEN, DISABLE); */
        /* configure LIDD control register */
        tempVar = (setup->config).polControl;
        CSL_FINS (hLcdc->regs->LCDLIDDCR, LCDC_LCDLIDDCR_DONE_INT_EN,
                              (setup->config).fdoneIntEn);

        CSL_FINS (hLcdc->regs->LCDLIDDCR, LCDC_LCDLIDDCR_LIDD_MODE_SEL,
                                  (setup->config).modeSel);

        CSL_FINS (hLcdc->regs->LCDCR, LCDC_LCDCR_CLKDIV,
                                  (setup->config).clkDiv);

        CSL_FINS (hLcdc->regs->LCDLIDDCR, LCDC_LCDLIDDCR_DMA_CS0_CS1,
                                 (setup->config).dmaCs0Cs1);

        hLcdc->regs->LCDLIDDCR &= ~CSL_LCDC_LCDLIDDCR_CS1_E1_POL_MASK;
        hLcdc->regs->LCDLIDDCR &= ~CSL_LCDC_LCDLIDDCR_CS0_E0_POL_MASK;
        hLcdc->regs->LCDLIDDCR &= ~CSL_LCDC_LCDLIDDCR_WS_DIR_POL_MASK;
        hLcdc->regs->LCDLIDDCR &= ~CSL_LCDC_LCDLIDDCR_RS_EN_POL_MASK;
        hLcdc->regs->LCDLIDDCR &= ~CSL_LCDC_LCDLIDDCR_ALEPOL_MASK;
        hLcdc->regs->LCDLIDDCR |=
                     (tempVar << CSL_LCDC_LCDLIDDCR_ALEPOL_SHIFT);

        CSL_FINS (hLcdc->regs->LCDLIDDCR, LCDC_LCDLIDDCR_LIDD_DMA_EN,
                                 (setup->config).dmaEnable);

        /* configure LIDD chip selct configuration register */
        if(CSL_LCDC_LIDD_NOT_USE_CS1 == (setup->useCs1))
        {
            /* configure LIDD CS0 configuration register */
            tempVar = (setup->timingCs0).rSu;

            CSL_FINS (hLcdc->regs->LCDLIDDCS0CONFIG0,
                                   LCDC_LCDLIDDCS0CONFIG0_R_STROBE,
                                  (setup->timingCs0).rStrobe);

            CSL_FINS (hLcdc->regs->LCDLIDDCS0CONFIG0,
                                   LCDC_LCDLIDDCS0CONFIG0_R_HOLD,
                                   (setup->timingCs0).rHold);

            CSL_FINS (hLcdc->regs->LCDLIDDCS0CONFIG0,
                                   LCDC_LCDLIDDCS0CONFIG0_TA,
                                   (setup->timingCs0).ta);
            if(tempVar >= 16)
            {
                tempVar = tempVar - 15 ;
                tempVar = tempVar >> 4;
                hLcdc->regs->LCDLIDDCS0CONFIG0 |=
                             CSL_LCDC_LCDLIDDCS0CONFIG0_R_SU0_MASK;

                CSL_FINS (hLcdc->regs->LCDLIDDCS0CONFIG1,
                                       LCDC_LCDLIDDCS0CONFIG1_R_SU1, tempVar);
            }
            else
            {
                CSL_FINS (hLcdc->regs->LCDLIDDCS0CONFIG0,
                                      LCDC_LCDLIDDCS0CONFIG0_R_SU0, tempVar);
            }
            CSL_FINS (hLcdc->regs->LCDLIDDCS0CONFIG1,
                                   LCDC_LCDLIDDCS0CONFIG1_W_SU,
                                   (setup->timingCs0).wSu);

            CSL_FINS (hLcdc->regs->LCDLIDDCS0CONFIG1,
                                   LCDC_LCDLIDDCS0CONFIG1_W_STROBE,
                                   (setup->timingCs0).wStrobe);

            CSL_FINS (hLcdc->regs->LCDLIDDCS0CONFIG1,
                                   LCDC_LCDLIDDCS0CONFIG1_W_HOLD,
                                   (setup->timingCs0).wHold);
        }
        else
        {
            /* configure LIDD CS1 configuration register */
            tempVar = (setup->timingCs1).rSu;
            CSL_FINS (hLcdc->regs->LCDLIDDCS1CONFIG0,
                                   LCDC_LCDLIDDCS1CONFIG0_R_STROBE,
                                   (setup->timingCs1).rStrobe);

            CSL_FINS (hLcdc->regs->LCDLIDDCS1CONFIG0,
                                   LCDC_LCDLIDDCS1CONFIG0_R_HOLD,
                                   (setup->timingCs1).rHold);

            CSL_FINS (hLcdc->regs->LCDLIDDCS1CONFIG0,
                                   LCDC_LCDLIDDCS1CONFIG0_TA,
                                   (setup->timingCs1).ta);
            if(tempVar >= 16)
            {
                tempVar = tempVar - 15 ;
                tempVar = tempVar >> 4;
                hLcdc->regs->LCDLIDDCS1CONFIG0 |=
                             CSL_LCDC_LCDLIDDCS1CONFIG0_R_SU0_MASK;

                CSL_FINS (hLcdc->regs->LCDLIDDCS1CONFIG1,
                                       LCDC_LCDLIDDCS1CONFIG1_R_SU1, tempVar);
            }
            else
            {
                CSL_FINS (hLcdc->regs->LCDLIDDCS1CONFIG0,
                                    LCDC_LCDLIDDCS1CONFIG0_R_SU0, tempVar);
            }
            CSL_FINS (hLcdc->regs->LCDLIDDCS1CONFIG1,
                                   LCDC_LCDLIDDCS1CONFIG1_W_SU,
                                   (setup->timingCs1).wSu);

            CSL_FINS (hLcdc->regs->LCDLIDDCS1CONFIG1,
                                   LCDC_LCDLIDDCS1CONFIG1_W_STROBE,
                                   (setup->timingCs1).wStrobe);

            CSL_FINS (hLcdc->regs->LCDLIDDCS1CONFIG1,
                                   LCDC_LCDLIDDCS1CONFIG1_W_HOLD,
                                   (setup->timingCs1).wHold);
        }

    return CSL_SOK;
}

/** ===========================================================================
 *   @n@b LCD_configDMA
 *
 *   @b Description
 *   @n It configures the LCDC register specific to DMA configuration and used
 *      to put the bulky data on LCD panel.
 *
 *   @b Arguments
 *   @verbatim
            hLcdc            Handle to the LCDC.
            pconfigDma       Pointer to DMA Config structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Congig Parameter is invalid.
 *   @li                    CSL_ESYS_NOTSUPPORTED
 *                                    - Buffer memory is not in SARAM Portion.
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() and LCD_setup() API should be called before this and
 *       memory buffer must be in the SARAM section.
 *
 *   <b> Post Condition </b>
 *   @n  For star of data Transfer enable the dma bit in LIDD register
 *
 *   @b Modifies
 *   @n  LCDC DMA Related Registers will be populated.
 *
 *   @b Example
 *   @verbatim

#define LCD_MAX_BUFFER_SIZE      (0x0FFF)

            Uint16               instanceNum;
            CSL_status           status;
            CSL_LcdcObj          pLcdcObj
            CSL_LcdcHandle       hLcdc;
            CSL_LcdcHwSetup      setup;
            CSL_LcdcConfigDma    pConfigDma;

    Uint16 Buf[LCD_MAX_BUFFER_SIZE];
                ...
     CSL_LcdcConfigLidd       configLIDD;
     CSL_LcdcLiddTiming       timingCs0LIDD;
     CSL_LcdcLiddTiming       timingCs1LIDD;
 *
.........\\\\\\\\\\\\\  SETUP for LIDD Controller

    configLIDD.clkDiv      = 0x3;
    configLIDD.fdoneIntEn  = CSL_LCDC_LIDD_FDONE_DISABLE ;
    configLIDD.dmaCs0Cs1   = CSL_LCDC_LIDD_CS0;
    configLIDD.dmaEnable   = CSL_LCDC_LIDD_DMA_DISABLE;
    configLIDD.polControl  = 0x0000;
    configLIDD.modeSel     = CSL_LCDC_LIDD_ASYNC_MPU80;

    timingCs0LIDD.wSu      = 0x1F;
    timingCs0LIDD.wStrobe  = 0x3F ;
    timingCs0LIDD.wHold    = 0x0F;
    timingCs0LIDD.rSu      = 0x1F;
    timingCs0LIDD.rStrobe  = 0x3F;
    timingCs0LIDD.rHold    = 0x0F;
    timingCs0LIDD.ta       = 0x01;

    timingCs1LIDD.wSu      = 0x1F;
    timingCs1LIDD.wStrobe  = 0x3F ;
    timingCs1LIDD.wHold    = 0x0F;
    timingCs1LIDD.rSu      = 0x1F;
    timingCs1LIDD.rStrobe  = 0x3F;
    timingCs1LIDD.rHold    = 0x0F;
    timingCs1LIDD.ta       = 0x01;

    setup.config        = configLIDD;
    setup.useCs1        = CSL_LCDC_LIDD_NOT_USE_CS1;
    setup.timingCs0        = timingCs0LIDD;
    setup.extendSetup   = NULL;

||||||||||||*******************************************************||||||||||||

            instanceNum = CSL_LCDC_INSTANCE_0
            hLcdc = LCD_open(instanceNum, &pLcdcObj, &status);

            status = LCD_setup(hLcdc, &setup);

        pConfigDma.burstSize  = CSL_LCDC_DMA_BURST4;
        pConfigDma.eofIntEn   = CSL_LCDC_EOFINT_DISABLE;
        pConfigDma.bigEndian  = CSL_LCDC_ENDIANESS_LITTLE;
        pConfigDma.frameMode  = CSL_LCDC_FBMODE_SINGLE;
        pConfigDma.fb0Base    = (Uint32)Buf;
        pConfigDma.fb0Ceil    = (Uint32)&Buf[LCD_MAX_BUFFER_SIZE];
        pConfigDma.fb1Base    = (Uint32)NULL;
        pConfigDma.fb1Ceil    = (Uint32)NULL;

            status = LCD_configDMA(hLcdc, &pconfigDma);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status LCD_configDMA (
               CSL_LcdcHandle         hLcdc,
               CSL_LcdcConfigDma*     pconfigDma )
{
    Uint32      tmpAddrBase;
    Uint32      tmpAddrCeil;
    Uint16      tmpAddrLSB;
    Uint16      tmpAddrMSB;

    if((hLcdc !=  NULL) && (pconfigDma != NULL))
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

    CSL_FINS(hLcdc->regs->LCDDMACR, LCDC_LCDDMACR_BURST_SIZE,
                                                     pconfigDma->burstSize);

    CSL_FINS(hLcdc->regs->LCDDMACR, LCDC_LCDDMACR_EOF_INTEN,
                                                      pconfigDma->eofIntEn);

    CSL_FINS(hLcdc->regs->LCDDMACR, LCDC_LCDDMACR_BIGENDIAN,
                                                     pconfigDma->bigEndian);

    CSL_FINS(hLcdc->regs->LCDDMACR, LCDC_LCDDMACR_FRAME_MODE,
                                                     pconfigDma->frameMode);

    if(CSL_LCDC_FBMODE_SINGLE ==
           CSL_FEXT(hLcdc->regs->LCDDMACR, LCDC_LCDDMACR_FRAME_MODE))
    {
        /* calculation of fb0Base address which can be seen by DMA */
        tmpAddrBase = pconfigDma->fb0Base;
        tmpAddrCeil = pconfigDma->fb0Ceil;

        if((tmpAddrBase >= CSL_LCD_SARAM_START_ADDR)
                      && (tmpAddrBase <= CSL_LCD_SARAM_END_ADDR))
        {
            tmpAddrBase  = (tmpAddrBase << CSL_LCD_DMA_SHIFT)
                                   + CSL_LCD_SARAM_DMA_ADDR_OFFSET;
        }
        else
        {
            return CSL_ESYS_NOTSUPPORTED;
        }

        /* calculation of fb0Ceil address which can be seen by DMA */
        if((tmpAddrCeil >= CSL_LCD_SARAM_START_ADDR)
                      && (tmpAddrCeil <= CSL_LCD_SARAM_END_ADDR))
        {
            tmpAddrCeil  = (tmpAddrCeil << CSL_LCD_DMA_SHIFT)
                                   + CSL_LCD_SARAM_DMA_ADDR_OFFSET;
        }
        else
        {
            return CSL_ESYS_NOTSUPPORTED;
        }
        /* DMA frame buffer 0 Base Address Set */
        tmpAddrLSB = tmpAddrBase & CSL_LCD_UINT16_MASK;
        tmpAddrMSB = (Uint16)(tmpAddrBase >> CSL_LCD_UINT16_NUMBER_BITS);
        hLcdc->regs->LCDDMAFB0BAR0 = tmpAddrLSB;
        hLcdc->regs->LCDDMAFB0BAR1 = tmpAddrMSB;
        /* DMA frame buffer 0 Ceil Address Set*/
        tmpAddrLSB = tmpAddrCeil & CSL_LCD_UINT16_MASK;
        tmpAddrMSB = (Uint16)(tmpAddrCeil >> CSL_LCD_UINT16_NUMBER_BITS);
        hLcdc->regs->LCDDMAFB0CAR0 = tmpAddrLSB;
        hLcdc->regs->LCDDMAFB0CAR1 = tmpAddrMSB;
    }
    else
    {
        /* calculation of fb0Base address which can be seen by DMA */
        tmpAddrBase = pconfigDma->fb0Base;
        tmpAddrCeil = pconfigDma->fb0Ceil;

        if((tmpAddrBase >= CSL_LCD_SARAM_START_ADDR)
                      && (tmpAddrBase <= CSL_LCD_SARAM_END_ADDR))
        {
            tmpAddrBase  = (tmpAddrBase << CSL_LCD_DMA_SHIFT)
                                   + CSL_LCD_SARAM_DMA_ADDR_OFFSET;
        }
        else
        {
            return CSL_ESYS_NOTSUPPORTED;
        }

        /* calculation of fb0Ceil address which can be seen by DMA */
        if((tmpAddrCeil >= CSL_LCD_SARAM_START_ADDR)
                      && (tmpAddrCeil <= CSL_LCD_SARAM_END_ADDR))
        {
            tmpAddrCeil  = (tmpAddrCeil << CSL_LCD_DMA_SHIFT)
                                   + CSL_LCD_SARAM_DMA_ADDR_OFFSET;
        }
        else
        {
            return CSL_ESYS_NOTSUPPORTED;
        }
        /* DMA frame buffer 0 Base Address Set */
        tmpAddrLSB = tmpAddrBase & CSL_LCD_UINT16_MASK;
        tmpAddrMSB = (Uint16)(tmpAddrBase >> CSL_LCD_UINT16_NUMBER_BITS);
        hLcdc->regs->LCDDMAFB0BAR0 = tmpAddrLSB;
        hLcdc->regs->LCDDMAFB0BAR1 = tmpAddrMSB;
        /* DMA frame buffer 0 Ceil Address Set*/
        tmpAddrLSB = tmpAddrCeil & CSL_LCD_UINT16_MASK;
        tmpAddrMSB = (Uint16)(tmpAddrCeil >> CSL_LCD_UINT16_NUMBER_BITS);
        hLcdc->regs->LCDDMAFB0CAR0 = tmpAddrLSB;
        hLcdc->regs->LCDDMAFB0CAR1 = tmpAddrMSB;

        /* calculation of fb1Base address which can be seen by DMA */
        tmpAddrBase = pconfigDma->fb1Base;
        tmpAddrCeil = pconfigDma->fb1Ceil;

        if((tmpAddrBase >= CSL_LCD_SARAM_START_ADDR)
                      && (tmpAddrBase <= CSL_LCD_SARAM_END_ADDR))
        {
            tmpAddrBase  = (tmpAddrBase << CSL_LCD_DMA_SHIFT)
                                   + CSL_LCD_SARAM_DMA_ADDR_OFFSET;
        }
        else
        {
            return CSL_ESYS_NOTSUPPORTED;
        }

        /* calculation of fb1Ceil address which can be seen by DMA */
        if((tmpAddrCeil >= CSL_LCD_SARAM_START_ADDR)
                      && (tmpAddrCeil <= CSL_LCD_SARAM_END_ADDR))
        {
            tmpAddrCeil  = (tmpAddrCeil << CSL_LCD_DMA_SHIFT)
                                   + CSL_LCD_SARAM_DMA_ADDR_OFFSET;
        }
        else
        {
            return CSL_ESYS_NOTSUPPORTED;
        }
        /* DMA frame buffer 1 Base Address Set */
        tmpAddrLSB = tmpAddrBase & CSL_LCD_UINT16_MASK;
        tmpAddrMSB = (Uint16)(tmpAddrBase >> CSL_LCD_UINT16_NUMBER_BITS);
        hLcdc->regs->LCDDMAFB1BAR0 = tmpAddrLSB;
        hLcdc->regs->LCDDMAFB1BAR1 = tmpAddrMSB;
        /* DMA frame buffer 1 Ceil Address Set*/
        tmpAddrLSB = tmpAddrCeil & CSL_LCD_UINT16_MASK;
        tmpAddrMSB = (Uint16)(tmpAddrCeil >> CSL_LCD_UINT16_NUMBER_BITS);
        hLcdc->regs->LCDDMAFB1CAR0 = tmpAddrLSB;
        hLcdc->regs->LCDDMAFB1CAR1 = tmpAddrMSB;
    }
    return CSL_SOK;
}

/** ===========================================================================
 *   @n@b LCD_close
 *
 *   @b Description
 *   @n This function closes the specified handle to LCDC.
 *
 *   @b Arguments
 *   @verbatim
            hLcdc            Handle to the LCDC
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  No other API of LCDC should be call after this.
 *
 *   @b Modifies
 *   @n LCDC Register Overlay structure will be assigned to NULL.
 *
 *   @b Example
 *   @verbatim
            Uint16               instanceNum;
            CSL_status           status;
            CSL_LcdcObj          pLcdcObj
            CSL_LcdcHandle       hLcdc;
            ...

            instanceNum = CSL_LCDC_INSTANCE_0
            hLcdc = LCD_open(instanceNum, &pLcdcObj, &status);

            status = LCD_close(hLcdc);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status LCD_close(
               CSL_LcdcHandle     hLcdc)
{
    if(hLcdc ==  NULL)
    {
        return CSL_ESYS_BADHANDLE;
    }
    else
    {
        hLcdc->perNum    = CSL_LCDC_INSTANCE_INV;
        hLcdc->regs      = NULL;
        return CSL_SOK;
    }
}

