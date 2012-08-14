/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_dma.c
 *
 *  @brief DMA functional layer API defenition file
 *
 *  Path: \(CSLPATH)/src
 */

/* ============================================================================
 * Revision History
 * ================
 * 25-Aug-2008 Added DMA for CSL.
 * 25-Aug-2008 Updated for Doxygen.
 * 28-Aug-2008 Added Return Types for Error checking for API's parameter.
 * 01-Oct-2008 Added additional parameter CSL_DMA_ChannelObj pointer to the
 *             DMA_open() API and removed global declaration of
 *             CSL_DMA_ChannelObj variable.
 * 08-May-2009 Added DMA_swapWords API for C5505 PG1.4
 * 26-May-2009 Modified as per the review comments
 * 17-Dec-2009 Added Support for Ping-Pong mode for chip C5515
 * ============================================================================
 */

#include "csl_dma.h"

/** ===========================================================================
 *   @n@b DMA_init
 *
 *   @b Description
 *   @n This is the initialization function for the DMA CSL. The function
 *      must be called before calling any other API from this CSL. This
 *      function is for enabling the clock to the DMA Controller.
 *      Currently, the function just return status CSL_SOK.
 *      This function should be called only once during system initialization
 *      time. This function gates the clock for all the four DMA modules.
 *      Application programs may disable the clocks for DMA modules which are
 *      are not in use.
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                        CSL_SOK - Always returns
 *
 *   <b> Pre Condition </b>
 *   @n  This should be call first before calling any other DMA Function.
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

            status = DMA_init();
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_init (void)
{
    /* Enable the corresponding DMA clock from PCGCR Registers */
    CSL_FINST(CSL_SYSCTRL_REGS->PCGCR1, SYS_PCGCR1_DMA0CG, ACTIVE);
    CSL_FINST(CSL_SYSCTRL_REGS->PCGCR2, SYS_PCGCR2_DMA1CG, ACTIVE);
    CSL_FINST(CSL_SYSCTRL_REGS->PCGCR2, SYS_PCGCR2_DMA2CG, ACTIVE);
    CSL_FINST(CSL_SYSCTRL_REGS->PCGCR2, SYS_PCGCR2_DMA3CG, ACTIVE);

    /* Set the reset clock cycle */
    CSL_FINS(CSL_SYSCTRL_REGS->PSRCR, SYS_PSRCR_COUNT,
             CSL_DMA_RESET_CLOCK_CYCLE);
    CSL_FINST(CSL_SYSCTRL_REGS->PRCR, SYS_PRCR_DMA_RST, RST);

#if (defined(CHIP_VC5505) || defined(CHIP_VC5504))
    // enable all the DMA interrupts
    CSL_SYSCTRL_REGS->DMAIER = 0xFFFF;
#endif
    return CSL_SOK;
}

/** ===========================================================================
 *   @n@b DMA_open
 *
 *   @b Description
 *   @n This function returns the handle to the DMA
 *      instance. This handle is passed to all other CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            chanNum     channel number to use.
            pDmaChanObj pointer to dma channel object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_DMA_Handle
 *   @n                     Valid DMA handle will be returned if
 *                          status value is equal to CSL_SOK.
 *
 *   <b> Pre Condition </b>
 *   @n  DMA_init() API should be called before DMA_open().
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is updated in the status variable. If status
 *              returned is
 *   @li            CSL_SOK            - Valid DMA handle is returned
 *   @li            CSL_ESYS_INVPARAMS - Invalid parameter
 *   @n   2.    CSL_DMA_ChannelObj object structure is populated
 *
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. CSL_DMA_ChannelObj object structure
 *
 *   @b Example
 *   @verbatim

            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj
            CSL_DMA_Handle       hDMA;
            ...

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_DMA_Handle DMA_open (
    CSL_DMAChanNum       chanNum,
    CSL_DMA_ChannelObj   *pDmaChanObj,
    CSL_Status           *status
)
{
    CSL_DMA_Handle    hDMA;

    if((chanNum >= CSL_DMA_CHAN_INV) || (pDmaChanObj == NULL))
    {
          hDMA = NULL;
          if(status != NULL)
          {
            *status = CSL_ESYS_INVPARAMS;
        }
    }
    else if(status == NULL)
    {
        hDMA = NULL;
    }
    else
    {
        *status = CSL_SOK;
        pDmaChanObj->chanNum    = chanNum;
        pDmaChanObj->isChanFree = CSL_DMA_CHANNEL_BUSY;
        /* set the deafault values for channel direction and triggering */
        pDmaChanObj->chanDir    = CSL_DMA_READ;
        pDmaChanObj->trigger    = CSL_DMA_SOFTWARE_TRIGGER;
        pDmaChanObj->trfType    = CSL_DMA_TRANSFER_MEMORY;

        switch( chanNum )
        {
        case CSL_DMA_CHAN0:
        case CSL_DMA_CHAN1:
        case CSL_DMA_CHAN2:
        case CSL_DMA_CHAN3:
            pDmaChanObj->dmaRegs    = CSL_DMA0_REGS;
            pDmaChanObj->dmaInstNum = CSL_DMA_ENGINE0;
            hDMA                    = pDmaChanObj;
            break;

        case CSL_DMA_CHAN4:
        case CSL_DMA_CHAN5:
        case CSL_DMA_CHAN6:
        case CSL_DMA_CHAN7:
            pDmaChanObj->dmaRegs    = CSL_DMA1_REGS;
            pDmaChanObj->dmaInstNum = CSL_DMA_ENGINE1;
            hDMA                    = pDmaChanObj;
            break;

        case CSL_DMA_CHAN8:
        case CSL_DMA_CHAN9:
        case CSL_DMA_CHAN10:
        case CSL_DMA_CHAN11:
            pDmaChanObj->dmaRegs       = CSL_DMA2_REGS;
            pDmaChanObj->dmaInstNum    = CSL_DMA_ENGINE2;
            hDMA                       = pDmaChanObj;
            break;

        case CSL_DMA_CHAN12:
        case CSL_DMA_CHAN13:
        case CSL_DMA_CHAN14:
        case CSL_DMA_CHAN15:
            pDmaChanObj->dmaRegs    = CSL_DMA3_REGS;
            pDmaChanObj->dmaInstNum = CSL_DMA_ENGINE3;
            hDMA                    = pDmaChanObj;
            break;

        }
    }
    return hDMA;
}

/** ===========================================================================
 *   @n@b DMA_close
 *
 *   @b Description
 *   @n This function closes the specified handle to DMA.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  DMA_open() must be called before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  After calling this function no other API should be call
 *       for the same dma Handle.
 *
 *   @b Modifies
 *   @n    1. The DMA register will assigned to reset values for this handle.
 *   @n    2. CSL_DMA_ChannelObj object structure
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj
            CSL_DMA_Handle       hDMA;
            ...

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

            status = DMA_Close(hDMA);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_close (
    CSL_DMA_Handle         hDMA
)
{
    if(hDMA ==  NULL)
    {
        return CSL_ESYS_BADHANDLE;
    }
    else
    {
        hDMA->isChanFree = CSL_DMA_CHANNEL_FREE;
        hDMA->dmaRegs    = NULL;
        return CSL_SOK;
    }
}

/** ===========================================================================
 *   @n@b DMA_config
 *
 *   @b Description
 *   @n It configures the DMA Controller registers of particular handle as per
 *      the values passed in the DMA config structure.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA.
            Config          Pointer to DMA Config structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Congig Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  DMA_open() must be called successfully before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n 1. DMA registers will be updated as per config parameter
 *   @n 2. CSL_DMA_ChannelObj Object will be updated to store some info
 *              as passed config parameter.
 *
 *   @b Example
 *   @verbatim

        #define CSL_DMA_BUFFER_SIZE 1024

            CSL_DMA_Handle       hDMA;
            CSL_DMA_Config       pConfig;
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj

            Uint16 dmaSRCBuff[CSL_DMA_BUFFER_SIZE];
            Uint16 dmaDESTBuff[CSL_DMA_BUFFER_SIZE];
            ...

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
            pConfig.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
#endif

            pConfig.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            pConfig.burstLen     = CSL_DMA_TXBURST_8WORD;
            pConfig.trigger      = CSL_DMA_SOFTWARE_TRIGGER;
            pConfig.dmaEvt       = CSL_DMA_EVT_NONE;
            pConfig.dmaInt       = CSL_DMA_INTERRUPT_DISABLE;
            pConfig.chanDir      = CSL_DMA_READ;
            pConfig.trfType      = CSL_DMA_TRANSFER_MEMORY;
            pConfig.dataLen      = CSL_DMA_BUFFER_SIZE * 2;
            pConfig.srcAddr      = (Uint32)dmaSRCBuff;
            pConfig.destAddr     = (Uint32)dmaDESTBuff;

            status = DMA_Config(hDMA, &pConfig);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_config (
    CSL_DMA_Handle            hDMA,
    CSL_DMA_Config            *pConfig
)
{
    Uint16              chanNum;
    Uint16           srcAddrLSB;
    Uint16           srcAddrMSB;
    Uint16           destAddrLSB;
    Uint16           destAddrMSB;
    Uint16           src_addrMode;
    Uint16           dest_addrMode;
    Uint32           src_address;
    Uint32           dest_address;

    if((hDMA !=  NULL) && (pConfig != NULL))
    {
        chanNum = hDMA->chanNum;
        while(chanNum >= CSL_DMA_PER_CNT)
        {
            chanNum = chanNum - CSL_DMA_PER_CNT;
        }
        /* passed parameters are corect  */
    }
    else
    {
        if(hDMA == NULL)
        {
            return CSL_ESYS_BADHANDLE;
        }
        else
        {
            return CSL_ESYS_INVPARAMS;
        }
    }

    hDMA->trigger = pConfig->trigger;
    hDMA->chanDir = pConfig->chanDir;
    hDMA->trfType = pConfig->trfType;
    hDMA->dmaInt  = pConfig->dmaInt;

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

    if(pConfig->pingPongMode == CSL_DMA_PING_PONG_ENABLE)
    {
        hDMA->pingPongEnabled = TRUE;
    }
    else
    {
        hDMA->pingPongEnabled = FALSE;
    }

#endif

    /* Assign address passed from config parameter to local variable */
    src_address = pConfig->srcAddr;
    dest_address = pConfig->destAddr;

    /* calculation of address which can be seen by DMA */
    if(CSL_DMA_TRANSFER_MEMORY == (pConfig->trfType))
    {
        if((src_address >= CSL_DMA_DARAM_START_ADDR)
                      && (src_address <= CSL_DMA_DARAM_END_ADDR))
        {
            src_address  = (src_address << CSL_DMA_ADDR_SHIFT)
                                         + CSL_DMA_DARAM_ADDR_OFFSET;
        }
        else if((src_address >= CSL_DMA_SARAM_START_ADDR)
                       && (src_address <= CSL_DMA_SARAM_END_ADDR))
        {
            src_address  = (src_address << CSL_DMA_ADDR_SHIFT)
                                         + CSL_DMA_SARAM_ADDR_OFFSET;
        }
        else
        {
            src_address  =  (pConfig->srcAddr);
        }

        if((dest_address >= CSL_DMA_DARAM_START_ADDR)
                      && (dest_address <= CSL_DMA_DARAM_END_ADDR))
        {
            dest_address  = (dest_address << CSL_DMA_ADDR_SHIFT)
                                            + CSL_DMA_DARAM_ADDR_OFFSET;
        }
        else if((dest_address >= CSL_DMA_SARAM_START_ADDR)
                       && (dest_address <= CSL_DMA_SARAM_END_ADDR))
        {
            dest_address  = (dest_address << CSL_DMA_ADDR_SHIFT)
                                            + CSL_DMA_SARAM_ADDR_OFFSET;
        }
        else
        {
            dest_address  =  (pConfig->destAddr);
        }

        srcAddrLSB = src_address & CSL_DMA_UINT16_MASK;
        srcAddrMSB = (Uint16)(src_address >> CSL_DMA_UINT16_NUMBER_BITS);
        destAddrLSB = dest_address & CSL_DMA_UINT16_MASK;
        destAddrMSB = (Uint16)(dest_address >> CSL_DMA_UINT16_NUMBER_BITS);
        src_addrMode   =  (Uint16)(CSL_DMA_ADDR_MODE_INCREMENT);
        dest_addrMode  =  (Uint16)(CSL_DMA_ADDR_MODE_INCREMENT);
    }
    else
    {
        /* Reading data from a peripheral */
        if((hDMA->chanDir == CSL_DMA_READ) )
        {
            if((dest_address >= CSL_DMA_DARAM_START_ADDR)
                          && (dest_address <= CSL_DMA_DARAM_END_ADDR))
            {
                dest_address  = (dest_address << CSL_DMA_ADDR_SHIFT) +
                                                 CSL_DMA_DARAM_ADDR_OFFSET;
            }
            else if((dest_address >= CSL_DMA_SARAM_START_ADDR)
                           && (dest_address <= CSL_DMA_SARAM_END_ADDR))
            {
                dest_address  = (dest_address << CSL_DMA_ADDR_SHIFT)
                                                + CSL_DMA_SARAM_ADDR_OFFSET;
            }
            else
            {
                dest_address  =  (pConfig->destAddr);
            }
            srcAddrLSB = src_address & CSL_DMA_UINT16_MASK;
            srcAddrMSB = (Uint16)(src_address >> CSL_DMA_UINT16_NUMBER_BITS);
            destAddrLSB = dest_address & CSL_DMA_UINT16_MASK;
            destAddrMSB = (Uint16)(dest_address >> CSL_DMA_UINT16_NUMBER_BITS);
            /* Set the destination and source address modes.
             * For read, src. is fixed and dest. is incremented.
             */
            src_addrMode   =  (Uint16)(CSL_DMA_ADDR_MODE_FIXED);
            dest_addrMode  =  (Uint16)(CSL_DMA_ADDR_MODE_INCREMENT);
        }
        /* Writing data to a peripheral */
        if(hDMA->chanDir == CSL_DMA_WRITE)
        {
            if((src_address >= CSL_DMA_DARAM_START_ADDR)
                          && (src_address <= CSL_DMA_DARAM_END_ADDR))
            {
                src_address  = (src_address << CSL_DMA_ADDR_SHIFT)
                                               + CSL_DMA_DARAM_ADDR_OFFSET;
            }
            else if((src_address >= CSL_DMA_SARAM_START_ADDR)
                           && (src_address <= CSL_DMA_SARAM_END_ADDR))
            {
                src_address  = (src_address << CSL_DMA_ADDR_SHIFT)
                                                + CSL_DMA_SARAM_ADDR_OFFSET;
            }
            else
            {
                src_address  =  (pConfig->srcAddr);
            }
            srcAddrLSB = src_address & CSL_DMA_UINT16_MASK;
            srcAddrMSB = (Uint16)(src_address >> CSL_DMA_UINT16_NUMBER_BITS);
            destAddrLSB = dest_address & CSL_DMA_UINT16_MASK;
            destAddrMSB = (Uint16)(dest_address >> CSL_DMA_UINT16_NUMBER_BITS);
            /* Set the destination and source address modes.
             * For write, src is incremented and dest. is fixed.
             */
            src_addrMode   =  (Uint16)(CSL_DMA_ADDR_MODE_INCREMENT);
            dest_addrMode  =  (Uint16)(CSL_DMA_ADDR_MODE_FIXED);
        }
    }

    switch(chanNum)
    {
    case CSL_DMA_CHAN0:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        hDMA->dmaRegs->DMACH0TCR2 &= ~CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;

        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDMA->dmaRegs->DMACH0SSAL,
                                DMA_DMACH0SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDMA->dmaRegs->DMACH0SSAU,
                                DMA_DMACH0SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDMA->dmaRegs->DMACH0DSAL,
                                DMA_DMACH0DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDMA->dmaRegs->DMACH0DSAU,
                                DMA_DMACH0DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDMA->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_AUTORLD, pConfig->autoMode);
        CSL_FINS(hDMA->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_BURSTMODE, pConfig->burstLen);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
        CSL_FINS(hDMA->dmaRegs->DMACH0TCR2,
                 DMA_DMACH0TCR2_EPPMODE, pConfig->pingPongMode);
#endif

        CSL_FINS(hDMA->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_SRCAMODE, src_addrMode);
        CSL_FINS(hDMA->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_DSTAMODE, dest_addrMode);
        CSL_FINS(hDMA->dmaRegs->DMACH0TCR1,
                                DMA_DMACH0TCR1_LENGTH, pConfig->dataLen);
        break;

    case CSL_DMA_CHAN1:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        hDMA->dmaRegs->DMACH1TCR2 &= ~CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;

        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDMA->dmaRegs->DMACH1SSAL,
                                DMA_DMACH1SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDMA->dmaRegs->DMACH1SSAU,
                                DMA_DMACH1SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDMA->dmaRegs->DMACH1DSAL,
                                DMA_DMACH1DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDMA->dmaRegs->DMACH1DSAU,
                                DMA_DMACH1DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDMA->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_AUTORLD, pConfig->autoMode);
        CSL_FINS(hDMA->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_BURSTMODE, pConfig->burstLen);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
        CSL_FINS(hDMA->dmaRegs->DMACH1TCR2,
                 DMA_DMACH1TCR2_EPPMODE, pConfig->pingPongMode);
#endif

        CSL_FINS(hDMA->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_SRCAMODE, src_addrMode);
        CSL_FINS(hDMA->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_DSTAMODE, dest_addrMode);
        CSL_FINS(hDMA->dmaRegs->DMACH1TCR1,
                                DMA_DMACH1TCR1_LENGTH, pConfig->dataLen);
        break;

    case CSL_DMA_CHAN2:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        hDMA->dmaRegs->DMACH2TCR2 &= ~CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;

        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDMA->dmaRegs->DMACH2SSAL,
                                DMA_DMACH2SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDMA->dmaRegs->DMACH2SSAU,
                                DMA_DMACH2SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDMA->dmaRegs->DMACH2DSAL,
                                DMA_DMACH2DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDMA->dmaRegs->DMACH2DSAU,
                                DMA_DMACH2DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDMA->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_AUTORLD, pConfig->autoMode);
        CSL_FINS(hDMA->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_BURSTMODE, pConfig->burstLen);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
        CSL_FINS(hDMA->dmaRegs->DMACH2TCR2,
                 DMA_DMACH2TCR2_EPPMODE, pConfig->pingPongMode);
#endif

        CSL_FINS(hDMA->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_SRCAMODE, src_addrMode);
        CSL_FINS(hDMA->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_DSTAMODE, dest_addrMode);
        CSL_FINS(hDMA->dmaRegs->DMACH2TCR1,
                                DMA_DMACH2TCR1_LENGTH, pConfig->dataLen);
        break;

    case CSL_DMA_CHAN3:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        hDMA->dmaRegs->DMACH3TCR2 &= ~CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;

        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDMA->dmaRegs->DMACH3SSAL,
                                DMA_DMACH3SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDMA->dmaRegs->DMACH3SSAU,
                                DMA_DMACH3SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDMA->dmaRegs->DMACH3DSAL,
                                DMA_DMACH3DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDMA->dmaRegs->DMACH3DSAU,
                                DMA_DMACH3DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDMA->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_AUTORLD, pConfig->autoMode);
        CSL_FINS(hDMA->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_BURSTMODE, pConfig->burstLen);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
        CSL_FINS(hDMA->dmaRegs->DMACH3TCR2,
                 DMA_DMACH3TCR2_EPPMODE, pConfig->pingPongMode);
#endif

        CSL_FINS(hDMA->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_SRCAMODE, src_addrMode);
        CSL_FINS(hDMA->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_DSTAMODE, dest_addrMode);
        CSL_FINS(hDMA->dmaRegs->DMACH3TCR1,
                                DMA_DMACH3TCR1_LENGTH, pConfig->dataLen);
        break;
    }

    /* set the dma event as well as interrupt in DMAIER and DMAIFR register */
    switch(hDMA->chanNum)
    {
    case CSL_DMA_CHAN0:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA0CESR1,
                                    SYS_DMA0CESR1_CH0EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA0CH0IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA0CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH0IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
       CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA0CH0IE, pConfig->dmaInt);
 #endif
       break;

    case CSL_DMA_CHAN1:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA0CESR1,
                                    SYS_DMA0CESR1_CH1EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA0CH1IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA0CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH1IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA0CH1IE, pConfig->dmaInt);
#endif
        break;
    case CSL_DMA_CHAN2:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA0CESR2,
                                    SYS_DMA0CESR2_CH2EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA0CH2IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA0CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH2IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA0CH2IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN3:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA0CESR2,
                                    SYS_DMA0CESR2_CH3EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA0CH3IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA0CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH3IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA0CH3IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN4:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA1CESR1,
                                    SYS_DMA1CESR1_CH0EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA1CH0IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA1CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH0IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA1CH0IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN5:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA1CESR1,
                                    SYS_DMA1CESR1_CH1EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA1CH1IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA1CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH1IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA1CH1IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN6:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA1CESR2,
                                    SYS_DMA1CESR2_CH2EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA1CH2IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA1CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH2IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA1CH2IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN7:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA1CESR2,
                                    SYS_DMA1CESR2_CH3EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA1CH3IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA1CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH3IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA1CH3IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN8:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA2CESR1,
                                    SYS_DMA2CESR1_CH0EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA2CH0IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA2CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH0IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA2CH0IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN9:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA2CESR1,
                                    SYS_DMA2CESR1_CH1EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA2CH1IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA2CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH1IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA2CH1IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN10:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA2CESR2,
                                    SYS_DMA2CESR2_CH2EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA2CH2IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA2CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH2IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA2CH2IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN11:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA2CESR2,
                                    SYS_DMA2CESR2_CH3EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA2CH3IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA2CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH3IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA2CH3IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN12:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA3CESR1,
                                    SYS_DMA3CESR1_CH0EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA3CH0IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA3CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH0IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA3CH0IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN13:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA3CESR1,
                                    SYS_DMA3CESR1_CH1EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA3CH1IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA3CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH1IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA3CH1IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN14:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA3CESR2,
                                    SYS_DMA3CESR2_CH2EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA3CH2IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA3CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH2IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA3CH2IE, pConfig->dmaInt);
#endif
        break;

    case CSL_DMA_CHAN15:
        CSL_FINS(CSL_SYSCTRL_REGS->DMA3CESR2,
                                    SYS_DMA3CESR2_CH3EVT, pConfig->dmaEvt);
        //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
        //                            SYS_DMAIFR_DMA3CH3IF, SET);
        CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA3CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH3IF_SET);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
        CSL_FINS(CSL_SYSCTRL_REGS->DMAIER,
                                    SYS_DMAIER_DMA3CH3IE, pConfig->dmaInt);
#endif
        break;
    }

    /* Enable the interrupt bit in DMA control register */
    switch(chanNum)
    {
    case CSL_DMA_CHAN0:
        CSL_FINS(hDMA->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_INTEN, pConfig->dmaInt);
        break;

    case CSL_DMA_CHAN1:
            CSL_FINS(hDMA->dmaRegs->DMACH1TCR2,
                                    DMA_DMACH1TCR2_INTEN, pConfig->dmaInt);
        break;

    case CSL_DMA_CHAN2:
        CSL_FINS(hDMA->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_INTEN, pConfig->dmaInt);
        break;

    case CSL_DMA_CHAN3:
        CSL_FINS(hDMA->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_INTEN, pConfig->dmaInt);
        break;
    }

    return CSL_SOK;
}

/** ===========================================================================
 *   @n@b DMA_getConfig
 *
 *   @b Description
 *   @n Gets the config information for the different operations of DMA.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA
            Config          Pointer to DMA Config structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Both Parametrs are valid.
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - invalid Congig Pointer.
 *
 *   <b> Pre Condition </b>
 *   @n  DMA_open() must be called successfully before this function call.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n   1.    The Config pointer will be updated to store the DMA Register
 *              information as per DMA Handle passed.
 *
 *   @b Example
 *   @verbatim
            CSL_DMA_Handle       hDMA;
            CSL_DMA_Config       pConfig;
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

            status = DMA_getConfig(hDMA, &pConfig);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_getConfig (
    CSL_DMA_Handle            hDMA,
    CSL_DMA_Config            *pConfig
)
{
    Uint16              chanNum;
    Uint16           srcAddrLSB;
    Uint16           srcAddrMSB;
    Uint16           destAddrLSB;
    Uint16           destAddrMSB;
    Uint32           srcAddr;
    Uint32           destAddr;

    if((hDMA !=  NULL) && (pConfig != NULL))
    {
        chanNum = hDMA->chanNum;
        while(chanNum >= CSL_DMA_PER_CNT)
        {
            chanNum = chanNum - CSL_DMA_PER_CNT;
        }
        /* passed parameters are correct */
    }
    else
    {
        if(hDMA ==  NULL)
        {
            return CSL_ESYS_BADHANDLE;
        }
        else
        {
            return CSL_ESYS_INVPARAMS;
        }
    }

    pConfig->trigger  =  hDMA->trigger;
    pConfig->chanDir  =  hDMA->chanDir;
    pConfig->trfType  =  hDMA->trfType;
    switch(chanNum)
    {
    case CSL_DMA_CHAN0:
        /* Get address detail for source and destination buffer */
        srcAddrLSB  = CSL_FEXT(hDMA->dmaRegs->DMACH0SSAL, DMA_DMACH0SSAL_SSAL);
        srcAddrMSB  = CSL_FEXT(hDMA->dmaRegs->DMACH0SSAU, DMA_DMACH0SSAU_SSAU);
        destAddrLSB = CSL_FEXT(hDMA->dmaRegs->DMACH0DSAL, DMA_DMACH0DSAL_DSAL);
        destAddrMSB = CSL_FEXT(hDMA->dmaRegs->DMACH0DSAU, DMA_DMACH0DSAU_DSAU);
        /* Get control information for selected channel */
        pConfig->dataLen = (Uint16)
                     CSL_FEXT(hDMA->dmaRegs->DMACH0TCR1, DMA_DMACH0TCR1_LENGTH);
        pConfig->autoMode = (CSL_DMAAutoReloadMode)
                    CSL_FEXT(hDMA->dmaRegs->DMACH0TCR2, DMA_DMACH0TCR2_AUTORLD);
        pConfig->burstLen = (CSL_DMATxBurstLen)
                  CSL_FEXT(hDMA->dmaRegs->DMACH0TCR2, DMA_DMACH0TCR2_BURSTMODE);
        pConfig->dmaInt = (CSL_DMAInterruptState)
                    CSL_FEXT(hDMA->dmaRegs->DMACH0TCR2, DMA_DMACH0TCR2_INTEN);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
        pConfig->pingPongMode = (CSL_DMAPingPongMode)
              CSL_FEXT(hDMA->dmaRegs->DMACH0TCR2, DMA_DMACH0TCR2_EPPMODE);
#endif
        break;

    case CSL_DMA_CHAN1:
        /* Get address detail for source and destination buffer */
        srcAddrLSB  = CSL_FEXT(hDMA->dmaRegs->DMACH1SSAL, DMA_DMACH1SSAL_SSAL);
        srcAddrMSB  = CSL_FEXT(hDMA->dmaRegs->DMACH1SSAU, DMA_DMACH1SSAU_SSAU);
        destAddrLSB = CSL_FEXT(hDMA->dmaRegs->DMACH1DSAL, DMA_DMACH1DSAL_DSAL);
        destAddrMSB = CSL_FEXT(hDMA->dmaRegs->DMACH1DSAU, DMA_DMACH1DSAU_DSAU);
        /* Get control information for selected channel */
        pConfig->dataLen = (Uint16)
                     CSL_FEXT(hDMA->dmaRegs->DMACH1TCR1, DMA_DMACH1TCR1_LENGTH);
        pConfig->autoMode = (CSL_DMAAutoReloadMode)
                    CSL_FEXT(hDMA->dmaRegs->DMACH1TCR2, DMA_DMACH1TCR2_AUTORLD);
        pConfig->burstLen = (CSL_DMATxBurstLen)
                  CSL_FEXT(hDMA->dmaRegs->DMACH1TCR2, DMA_DMACH1TCR2_BURSTMODE);
        pConfig->dmaInt = (CSL_DMAInterruptState)
                    CSL_FEXT(hDMA->dmaRegs->DMACH1TCR2, DMA_DMACH1TCR2_INTEN);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
        pConfig->pingPongMode = (CSL_DMAPingPongMode)
              CSL_FEXT(hDMA->dmaRegs->DMACH1TCR2, DMA_DMACH1TCR2_EPPMODE);
#endif
        break;

    case CSL_DMA_CHAN2:
        /* Get address detail for source and destination buffer */
        srcAddrLSB  = CSL_FEXT(hDMA->dmaRegs->DMACH2SSAL, DMA_DMACH2SSAL_SSAL);
        srcAddrMSB  = CSL_FEXT(hDMA->dmaRegs->DMACH2SSAU, DMA_DMACH2SSAU_SSAU);
        destAddrLSB = CSL_FEXT(hDMA->dmaRegs->DMACH2DSAL, DMA_DMACH2DSAL_DSAL);
        destAddrMSB = CSL_FEXT(hDMA->dmaRegs->DMACH2DSAU, DMA_DMACH2DSAU_DSAU);
        /* Get control information for selected channel */
        pConfig->dataLen = (Uint16)
                     CSL_FEXT(hDMA->dmaRegs->DMACH2TCR1, DMA_DMACH2TCR1_LENGTH);
        pConfig->autoMode = (CSL_DMAAutoReloadMode)
                    CSL_FEXT(hDMA->dmaRegs->DMACH2TCR2, DMA_DMACH2TCR2_AUTORLD);
        pConfig->burstLen = (CSL_DMATxBurstLen)
                  CSL_FEXT(hDMA->dmaRegs->DMACH2TCR2, DMA_DMACH2TCR2_BURSTMODE);
        pConfig->dmaInt = (CSL_DMAInterruptState)
                    CSL_FEXT(hDMA->dmaRegs->DMACH2TCR2, DMA_DMACH2TCR2_INTEN);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
        pConfig->pingPongMode = (CSL_DMAPingPongMode)
              CSL_FEXT(hDMA->dmaRegs->DMACH2TCR2, DMA_DMACH2TCR2_EPPMODE);
#endif

        break;

    case CSL_DMA_CHAN3:
        /* Get address detail for source and destination buffer */
        srcAddrLSB  = CSL_FEXT(hDMA->dmaRegs->DMACH3SSAL, DMA_DMACH3SSAL_SSAL);
        srcAddrMSB  = CSL_FEXT(hDMA->dmaRegs->DMACH3SSAU, DMA_DMACH3SSAU_SSAU);
        destAddrLSB = CSL_FEXT(hDMA->dmaRegs->DMACH3DSAL, DMA_DMACH3DSAL_DSAL);
        destAddrMSB = CSL_FEXT(hDMA->dmaRegs->DMACH3DSAU, DMA_DMACH3DSAU_DSAU);
        /* Get control information for selected channel */
        pConfig->dataLen = (Uint16)
                     CSL_FEXT(hDMA->dmaRegs->DMACH3TCR1, DMA_DMACH3TCR1_LENGTH);
        pConfig->autoMode = (CSL_DMAAutoReloadMode)
                    CSL_FEXT(hDMA->dmaRegs->DMACH3TCR2, DMA_DMACH3TCR2_AUTORLD);
        pConfig->burstLen = (CSL_DMATxBurstLen)
                  CSL_FEXT(hDMA->dmaRegs->DMACH3TCR2, DMA_DMACH3TCR2_BURSTMODE);
        pConfig->dmaInt = (CSL_DMAInterruptState)
                    CSL_FEXT(hDMA->dmaRegs->DMACH3TCR2, DMA_DMACH3TCR2_INTEN);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
        pConfig->pingPongMode = (CSL_DMAPingPongMode)
              CSL_FEXT(hDMA->dmaRegs->DMACH3TCR2, DMA_DMACH3TCR2_EPPMODE);
#endif

        break;
    }

    /* get information about event type */
    switch(hDMA->chanNum)
    {
    case CSL_DMA_CHAN0:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA0CESR1, SYS_DMA0CESR1_CH0EVT);
        break;

    case CSL_DMA_CHAN1:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA0CESR1, SYS_DMA0CESR1_CH1EVT);
        break;
    case CSL_DMA_CHAN2:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA0CESR2, SYS_DMA0CESR2_CH2EVT);
        break;

    case CSL_DMA_CHAN3:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA0CESR2, SYS_DMA0CESR2_CH3EVT);
        break;

    case CSL_DMA_CHAN4:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA1CESR1, SYS_DMA1CESR1_CH0EVT);
        break;

    case CSL_DMA_CHAN5:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA1CESR1, SYS_DMA1CESR1_CH1EVT);
        break;

    case CSL_DMA_CHAN6:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA1CESR2, SYS_DMA1CESR2_CH2EVT);
        break;

    case CSL_DMA_CHAN7:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA1CESR2, SYS_DMA1CESR2_CH3EVT);
        break;

    case CSL_DMA_CHAN8:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA2CESR1, SYS_DMA2CESR1_CH0EVT);
        break;

    case CSL_DMA_CHAN9:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA2CESR1, SYS_DMA2CESR1_CH1EVT);
        break;

    case CSL_DMA_CHAN10:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA2CESR2, SYS_DMA2CESR2_CH2EVT);
        break;

    case CSL_DMA_CHAN11:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA2CESR2, SYS_DMA2CESR2_CH3EVT);
        break;

    case CSL_DMA_CHAN12:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA3CESR1, SYS_DMA3CESR1_CH0EVT);
        break;

    case CSL_DMA_CHAN13:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA3CESR1, SYS_DMA3CESR1_CH1EVT);
        break;

    case CSL_DMA_CHAN14:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA3CESR2, SYS_DMA3CESR2_CH2EVT);
        break;

    case CSL_DMA_CHAN15:
        pConfig->dmaEvt = (CSL_DMAEvtType)
            CSL_FEXT(CSL_SYSCTRL_REGS->DMA3CESR2, SYS_DMA3CESR2_CH3EVT);
        break;
    }

    /* calculation of address which can be seen by CPU */
    srcAddr = srcAddrMSB;
    srcAddr = srcAddr << CSL_DMA_UINT16_NUMBER_BITS;
    srcAddr |= srcAddrLSB;

    destAddr = destAddrMSB;
    destAddr = destAddr << CSL_DMA_UINT16_NUMBER_BITS;
    destAddr |= destAddrLSB;

    if(CSL_DMA_TRANSFER_MEMORY == (hDMA->trfType))
    {
        if(srcAddr >= CSL_DMA_SARAM_ADDR_OFFSET)
        {
            srcAddr  = ((srcAddr - CSL_DMA_SARAM_ADDR_OFFSET)
                                           >> CSL_DMA_ADDR_SHIFT);
        }
        else if(srcAddr >= CSL_DMA_DARAM_ADDR_OFFSET)
        {
            srcAddr  = ((srcAddr - CSL_DMA_DARAM_ADDR_OFFSET)
                                           >> CSL_DMA_ADDR_SHIFT);
        }
        else
        {
            srcAddr  =  srcAddr;
        }

        if(destAddr >= CSL_DMA_SARAM_ADDR_OFFSET)
        {
            destAddr  = ((destAddr - CSL_DMA_SARAM_ADDR_OFFSET)
                                             >> CSL_DMA_ADDR_SHIFT);
        }
        else if(destAddr >= CSL_DMA_DARAM_ADDR_OFFSET)
        {
            destAddr  = ((destAddr - CSL_DMA_DARAM_ADDR_OFFSET)
                                             >> CSL_DMA_ADDR_SHIFT);
        }
        else
        {
            destAddr  =  destAddr;
        }
    }

    else
    {
        /* Reading data from a peripheral */
        if((hDMA->chanDir == CSL_DMA_READ) )
        {
            if(destAddr >= CSL_DMA_SARAM_ADDR_OFFSET)
            {
                destAddr  = ((destAddr - CSL_DMA_SARAM_ADDR_OFFSET)
                                                 >> CSL_DMA_ADDR_SHIFT);
            }
            else if(destAddr >= CSL_DMA_DARAM_ADDR_OFFSET)
            {
                destAddr  = ((destAddr - CSL_DMA_DARAM_ADDR_OFFSET)
                                                 >> CSL_DMA_ADDR_SHIFT);
            }
            else
            {
                destAddr  =  destAddr;
            }
        }
        /* Writing data to a peripheral */
        if(hDMA->chanDir == CSL_DMA_WRITE)
        {
            if(srcAddr >= CSL_DMA_SARAM_ADDR_OFFSET)
            {
                srcAddr  = ((srcAddr - CSL_DMA_SARAM_ADDR_OFFSET)
                                               >> CSL_DMA_ADDR_SHIFT);
            }
            else if(srcAddr >= CSL_DMA_DARAM_ADDR_OFFSET)
            {
                srcAddr  = ((srcAddr - CSL_DMA_DARAM_ADDR_OFFSET)
                                               >> CSL_DMA_ADDR_SHIFT);
            }
            else
            {
                srcAddr  =  srcAddr;
            }
        }
    }
    pConfig->srcAddr  = srcAddr;
    pConfig->destAddr = destAddr;

    return CSL_SOK;
}

/** ===========================================================================
 *   @n@b DMA_start
 *
 *   @b Description
 *   @n This function trigger to DMA for data transfer.
 *
 *   @b Arguments
 *   @verbatim

            hDMA            Handle to the DMA

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Start successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n DMA_config API must be called before calling this API for same handle.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *    @n Destination buffer will be modified after this call.
 *
 *   @b Example
 *   @verbatim

        #define CSL_DMA_BUFFER_SIZE 1024

            CSL_DMA_Handle       hDMA;
            CSL_DMA_Config       pConfig;
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj

        Uint16 dmaSRCBuff[CSL_DMA_BUFFER_SIZE];
        Uint16 dmaDESTBuff[CSL_DMA_BUFFER_SIZE];

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
            pConfig.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
#endif

            pConfig.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            pConfig.burstLen     = CSL_DMA_TXBURST_8WORD;
            pConfig.trigger      = CSL_DMA_SOFTWARE_TRIGGER;
            pConfig.dmaEvt       = CSL_DMA_EVT_NONE;
            pConfig.dmaInt       = CSL_DMA_INTERRUPT_DISABLE;
            pConfig.chanDir      = CSL_DMA_READ;
            pConfig.trfType      = CSL_DMA_TRANSFER_MEMORY;
            pConfig.dataLen      = CSL_DMA_BUFFER_SIZE * 2;
            pConfig.srcAddr      = (Uint32)dmaSRCBuff;
            pConfig.destAddr     = (Uint32)dmaDESTBuff;

            status = DMA_Config(hDMA, &pConfig);

            status = DMA_start (hDMA);

     @endverbatim
 * ===========================================================================
 */
CSL_Status DMA_start (
    /** Pointer to the object that holds reference to the
     *  DMA handle object.
     */
    CSL_DMA_Handle            hDMA
)
{
    Uint16      chanNum;

    if(hDMA ==  NULL)
    {
        return CSL_ESYS_BADHANDLE;
    }
    else
    {
        chanNum = hDMA->chanNum;

        while(chanNum >= CSL_DMA_PER_CNT)
        {
            chanNum = chanNum - CSL_DMA_PER_CNT;
        }

        if(CSL_DMA_EVENT_TRIGGER == (hDMA->trigger))
        {
            switch(chanNum)
            {
            case CSL_DMA_CHAN0:
              hDMA->dmaRegs->DMACH0TCR2 |= CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;
              break;
            case CSL_DMA_CHAN1:
              hDMA->dmaRegs->DMACH1TCR2 |= CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;
              break;
            case CSL_DMA_CHAN2:
              hDMA->dmaRegs->DMACH2TCR2 |= CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;
              break;
            case CSL_DMA_CHAN3:
              hDMA->dmaRegs->DMACH3TCR2 |= CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;
              break;
            }
        }
        else
        {
            switch(chanNum)
            {
            case CSL_DMA_CHAN0:
                CSL_FINS(hDMA->dmaRegs->DMACH0TCR2,
                               DMA_DMACH0TCR2_EN, CSL_DMA_CHANNEL_ENABLE);
                break;
            case CSL_DMA_CHAN1:
                CSL_FINS(hDMA->dmaRegs->DMACH1TCR2,
                               DMA_DMACH1TCR2_EN, CSL_DMA_CHANNEL_ENABLE);
                break;
            case CSL_DMA_CHAN2:
                CSL_FINS(hDMA->dmaRegs->DMACH2TCR2,
                               DMA_DMACH2TCR2_EN, CSL_DMA_CHANNEL_ENABLE);
                break;
            case CSL_DMA_CHAN3:
                CSL_FINS(hDMA->dmaRegs->DMACH3TCR2,
                               DMA_DMACH3TCR2_EN, CSL_DMA_CHANNEL_ENABLE);
                break;
            }
        }
        return CSL_SOK;
    }
}

/** ===========================================================================
 *   @n@b DMA_stop
 *
 *   @b Description
 *   @n This function set the DMA controller for not transfering the data.
 *
 *   @b Arguments
 *   @verbatim

            hDMA            Handle to the DMA

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Stop successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n 1. DMA_open() must be called successfully before this function call
 *      2. Call DMA_config() for the handle return by DMA_open()
 *      3. call DMA_start() and in the mean while call DMA_stop() if transfer
 *          is not required.
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

        #define CSL_DMA_BUFFER_SIZE 1024

            CSL_DMA_Handle       hDMA;
            CSL_DMA_Config       pConfig;
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj

        Uint16 dmaSRCBuff[CSL_DMA_BUFFER_SIZE];
        Uint16 dmaDESTBuff[CSL_DMA_BUFFER_SIZE];

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
            pConfig.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
#endif

            pConfig.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            pConfig.burstLen     = CSL_DMA_TXBURST_8WORD;
            pConfig.trigger      = CSL_DMA_SOFTWARE_TRIGGER;
            pConfig.dmaEvt       = CSL_DMA_EVT_NONE;
            pConfig.dmaInt       = CSL_DMA_INTERRUPT_DISABLE;
            pConfig.chanDir      = CSL_DMA_READ;
            pConfig.trfType      = CSL_DMA_TRANSFER_MEMORY;
            pConfig.dataLen      = CSL_DMA_BUFFER_SIZE * 2;
            pConfig.srcAddr      = (Uint32)dmaSRCBuff;
            pConfig.destAddr     = (Uint32)dmaDESTBuff;

            status = DMA_Config(hDMA, &pConfig);

            status = DMA_start (hDMA);

            status = DMA_stop (hDMA);

     @endverbatim
 * ===========================================================================
 */
CSL_Status DMA_stop (
    /** Pointer to the object that holds reference to the
     *  DMA handle object.
     */
    CSL_DMA_Handle            hDMA
)
{
    Uint16      chanNum;

    if(hDMA ==  NULL)
    {
        return CSL_ESYS_BADHANDLE;
    }
    else
    {
        chanNum = hDMA->chanNum;

        while(chanNum >= CSL_DMA_PER_CNT)
        {
            chanNum = chanNum - CSL_DMA_PER_CNT;
        }

        if(CSL_DMA_EVENT_TRIGGER == (hDMA->trigger))
        {
            switch(chanNum)
            {
            case CSL_DMA_CHAN0:
              hDMA->dmaRegs->DMACH0TCR2 &= ~CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;
              break;
            case CSL_DMA_CHAN1:
              hDMA->dmaRegs->DMACH1TCR2 &= ~CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;
              break;
            case CSL_DMA_CHAN2:
              hDMA->dmaRegs->DMACH2TCR2 &= ~CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;
              break;
            case CSL_DMA_CHAN3:
              hDMA->dmaRegs->DMACH3TCR2 &= ~CSL_DMA_DMATCR2_DMASTART_SYNC_MASK;
              break;
            }
        }
        else
        {
            switch(chanNum)
            {
            case CSL_DMA_CHAN0:
                CSL_FINS(hDMA->dmaRegs->DMACH0TCR2,
                               DMA_DMACH0TCR2_EN, CSL_DMA_CHANNEL_DISABLE);
                break;
            case CSL_DMA_CHAN1:
                CSL_FINS(hDMA->dmaRegs->DMACH1TCR2,
                               DMA_DMACH1TCR2_EN, CSL_DMA_CHANNEL_DISABLE);
                break;
            case CSL_DMA_CHAN2:
                CSL_FINS(hDMA->dmaRegs->DMACH2TCR2,
                               DMA_DMACH2TCR2_EN, CSL_DMA_CHANNEL_DISABLE);
                break;
            case CSL_DMA_CHAN3:
                CSL_FINS(hDMA->dmaRegs->DMACH3TCR2,
                               DMA_DMACH3TCR2_EN, CSL_DMA_CHANNEL_DISABLE);
                break;
            }
        }
        return CSL_SOK;
    }
}

/** ===========================================================================
 *   @n@b DMA_reset
 *
 *   @b Description
 *   @n Function reset all the register as per passed handle.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Reset successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n 1. DMA_open() must be called successfully before this function call
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  DMA Register will be populated with the reset values.
 *
 *   @b Example
 *   @verbatim
            CSL_DMA_Handle       hDMA;
            CSL_status           status;
            CSL_DMAChanNum       chanNum;
            CSL_DMA_ChannelObj   pDmaChanObj

            chanNum = CSL_DMA_CHAN0;
            hDMA = DMA_open(chanNum, &pDmaChanObj, &status);

            status = DMA_reset(hDMA);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_reset (
    CSL_DMA_Handle         hDMA
)
{
    Uint16      chanNum;

    if(hDMA ==  NULL)
    {
        return CSL_ESYS_BADHANDLE;
    }
    else
    {
        chanNum = hDMA->chanNum;
        while(chanNum >= CSL_DMA_PER_CNT)
        {
            chanNum = chanNum - CSL_DMA_PER_CNT;
        }
        switch(chanNum)
        {
        case CSL_DMA_CHAN0:
            hDMA->dmaRegs->DMACH0SSAL = CSL_DMA_DMACH0SSAL_RESETVAL;
            hDMA->dmaRegs->DMACH0SSAU = CSL_DMA_DMACH0SSAU_RESETVAL;
            hDMA->dmaRegs->DMACH0DSAL = CSL_DMA_DMACH0DSAL_RESETVAL;
            hDMA->dmaRegs->DMACH0DSAU = CSL_DMA_DMACH0DSAU_RESETVAL;
            hDMA->dmaRegs->DMACH0TCR1  = CSL_DMA_DMACH0TCR1_RESETVAL;
            hDMA->dmaRegs->DMACH0TCR2  = CSL_DMA_DMACH0TCR2_RESETVAL;
            break;

        case CSL_DMA_CHAN1:
            hDMA->dmaRegs->DMACH1SSAL = CSL_DMA_DMACH1SSAL_RESETVAL;
            hDMA->dmaRegs->DMACH1SSAU = CSL_DMA_DMACH1SSAU_RESETVAL;
            hDMA->dmaRegs->DMACH1DSAL = CSL_DMA_DMACH1DSAL_RESETVAL;
            hDMA->dmaRegs->DMACH1DSAU = CSL_DMA_DMACH1DSAU_RESETVAL;
            hDMA->dmaRegs->DMACH1TCR1  = CSL_DMA_DMACH1TCR1_RESETVAL;
            hDMA->dmaRegs->DMACH1TCR2  = CSL_DMA_DMACH1TCR2_RESETVAL;
            break;

        case CSL_DMA_CHAN2:
            hDMA->dmaRegs->DMACH2SSAL = CSL_DMA_DMACH2SSAL_RESETVAL;
            hDMA->dmaRegs->DMACH2SSAU = CSL_DMA_DMACH2SSAU_RESETVAL;
            hDMA->dmaRegs->DMACH2DSAL = CSL_DMA_DMACH2DSAL_RESETVAL;
            hDMA->dmaRegs->DMACH2DSAU = CSL_DMA_DMACH2DSAU_RESETVAL;
            hDMA->dmaRegs->DMACH2TCR1  = CSL_DMA_DMACH2TCR1_RESETVAL;
            hDMA->dmaRegs->DMACH2TCR2  = CSL_DMA_DMACH2TCR2_RESETVAL;
             break;

        case CSL_DMA_CHAN3:
            hDMA->dmaRegs->DMACH3SSAL = CSL_DMA_DMACH3SSAL_RESETVAL;
            hDMA->dmaRegs->DMACH3SSAU = CSL_DMA_DMACH3SSAU_RESETVAL;
            hDMA->dmaRegs->DMACH3DSAL = CSL_DMA_DMACH3DSAL_RESETVAL;
            hDMA->dmaRegs->DMACH3DSAU = CSL_DMA_DMACH3DSAU_RESETVAL;
            hDMA->dmaRegs->DMACH3TCR1  = CSL_DMA_DMACH3TCR1_RESETVAL;
            hDMA->dmaRegs->DMACH3TCR2  = CSL_DMA_DMACH3TCR2_RESETVAL;
            break;
        }

        /* Reset the channel event source register */
        switch(hDMA->chanNum)
        {
        case CSL_DMA_CHAN0:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA0CESR1,
                                        SYS_DMA0CESR1_CH0EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA0CH0IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA0CH0IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA0CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH0IF_SET);
            break;

        case CSL_DMA_CHAN1:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA0CESR1,
                                        SYS_DMA0CESR1_CH1EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA0CH1IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA0CH1IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA0CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH1IF_SET);
            break;
        case CSL_DMA_CHAN2:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA0CESR2,
                                        SYS_DMA0CESR2_CH2EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA0CH2IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA0CH2IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA0CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH2IF_SET);
            break;

        case CSL_DMA_CHAN3:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA0CESR2,
                                        SYS_DMA0CESR2_CH3EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA0CH3IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA0CH3IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA0CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA0CH3IF_SET);
            break;

        case CSL_DMA_CHAN4:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA1CESR1,
                                        SYS_DMA1CESR1_CH0EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA1CH0IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA1CH0IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA1CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH0IF_SET);
            break;

        case CSL_DMA_CHAN5:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA1CESR1,
                                        SYS_DMA1CESR1_CH1EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA1CH1IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA1CH1IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA1CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH1IF_SET);
            break;

        case CSL_DMA_CHAN6:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA1CESR2,
                                        SYS_DMA1CESR2_CH2EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA1CH2IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA1CH2IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA1CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH2IF_SET);
            break;

        case CSL_DMA_CHAN7:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA1CESR2,
                                        SYS_DMA1CESR2_CH3EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA1CH3IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA1CH3IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA1CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA1CH3IF_SET);
            break;

        case CSL_DMA_CHAN8:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA2CESR1,
                                        SYS_DMA2CESR1_CH0EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA2CH0IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA2CH0IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA2CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH0IF_SET);
            break;

        case CSL_DMA_CHAN9:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA2CESR1,
                                        SYS_DMA2CESR1_CH1EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA2CH1IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA2CH1IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA2CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH1IF_SET);
            break;

        case CSL_DMA_CHAN10:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA2CESR2,
                                        SYS_DMA2CESR2_CH2EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA2CH2IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA2CH2IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA2CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH2IF_SET);
            break;

        case CSL_DMA_CHAN11:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA2CESR2,
                                        SYS_DMA2CESR2_CH3EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA3CH3IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA3CH3IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA2CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA2CH3IF_SET);
            break;

        case CSL_DMA_CHAN12:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA3CESR1,
                                        SYS_DMA3CESR1_CH0EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA3CH0IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA3CH0IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA3CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH0IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH0IF_SET);
            break;

        case CSL_DMA_CHAN13:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA3CESR1,
                                        SYS_DMA3CESR1_CH1EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA3CH1IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA3CH1IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA3CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH1IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH1IF_SET);
            break;

        case CSL_DMA_CHAN14:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA3CESR2,
                                        SYS_DMA3CESR2_CH2EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA3CH2IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA3CH2IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA3CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH2IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH2IF_SET);
            break;

        case CSL_DMA_CHAN15:
            CSL_FINST(CSL_SYSCTRL_REGS->DMA3CESR2,
                                        SYS_DMA3CESR2_CH3EVT, RESETVAL);
 #if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            CSL_FINST(CSL_SYSCTRL_REGS->DMAIER,
                                        SYS_DMAIER_DMA3CH3IE, DISABLE);
#endif
            //CSL_FINST(CSL_SYSCTRL_REGS->DMAIFR,
            //                            SYS_DMAIFR_DMA3CH3IF, SET);
            CSL_FSET(CSL_SYSCTRL_REGS->DMAIFR, CSL_SYS_DMAIFR_DMA3CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH3IF_SHIFT, CSL_SYS_DMAIFR_DMA3CH3IF_SET);
            break;
        }
        return CSL_SOK;
    }
}

/** ===========================================================================
 *   @n@b DMA_swapWords
 *
 *   @b Description
 *   @n This function swaps words in a given buffer
 *
 *   @b Arguments
 *   @verbatim
            dataBuffer    Pointer to the buffer which words needs to be swaped
            dataLength    Length of the valid data buffer in the buffer
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Reset successful
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid parameters
 *
 *   <b> Pre Condition </b>
 *   @n None
 *
 *   <b> Post Condition </b>
 *    @n Swaps the words in the given buffer
 *
 *   @b Modifies
 *   @n  Order of data in the given buffer
 *
 *   @b Example
 *   @verbatim
            #define DMA_BUF_SIZE (256)

            Uint16        dmaBuf [DMA_BUF_SIZE];
            CSL_Status    status;

            status = DMA_swapWords (dmaBuf, DMA_BUF_SIZE);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DMA_swapWords (Uint16    *dataBuffer,
                          Uint32    dataLength)
{
    CSL_Status    status;
    Uint32        looper;
    Uint16        tempData;

    status = CSL_SOK;

    /* Check the input parameters */
    if ((dataBuffer != NULL) && (dataLength > 0))
    {
        /* Swap the words in the given buffer */
        for (looper = 0; looper < dataLength; looper += 2)
        {
            tempData = dataBuffer[looper];
            dataBuffer[looper] = dataBuffer[looper + 1];
            dataBuffer[looper + 1] = tempData;
        }
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

    return (status);
}

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

/** ===========================================================================
 *   @n@b DMA_getLastTransferStatus
 *
 *   @b Description
 *   @n This function returns status of the last trasfer during Ping-Pong
 *      buffer mode. This function will return a 1 or 0.
 *      In ping-pong buffer mode with reload enabled, DMA data transfer keeps
 *      switching between Ping and Pong data buffers. This function is used to
 *      know which buffer is transferred by DMA in the last transfer.
 *      0 - Ping Set Transfer completed
 *      1 - Pong Set Transfer completed
 *
 *   @b Arguments
 *   @verbatim
            hDMA      Handle to the DMA
            status    Status variable to update function Success/Failure status
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    0 - Ping Set Transfer completed
 *   @li                    1 - Pong Set Transfer completed
 *
 *   <b> Pre Condition </b>
 *   @n DMA_start() must be called successfully before this function call
 *
 *   <b> Post Condition </b>
 *   @n Returns last transfer status
 *
 *   @b Modifies
 *   @n none
 *
 *   @b Example
 *   @verbatim
            CSL_DMA_Handle    hDMA;
            CSL_Status        status;
            Bool              txStatus;

            status = DMA_start(hDMA);

            status = DMA_getLastTransferStatus(hDMA, &status);
     @endverbatim
 *  ===========================================================================
 */
Bool DMA_getLastTransferType (CSL_DMA_Handle    hDMA,
                              CSL_Status        *status)
{
    Bool      txferStatus;
    Uint16    chanNum;

    *status = CSL_SOK;

    if(hDMA ==  NULL)
    {
        *status = CSL_ESYS_BADHANDLE;
    }
    else
    {
        chanNum = hDMA->chanNum;

        while(chanNum >= CSL_DMA_PER_CNT)
        {
            chanNum = chanNum - CSL_DMA_PER_CNT;
        }

        switch(chanNum)
        {
            case CSL_DMA_CHAN0:
                txferStatus = CSL_FEXT(hDMA->dmaRegs->DMACH0TCR2,
                                       DMA_DMACH0TCR2_LTSTATUS);
                 break;
            case CSL_DMA_CHAN1:
                txferStatus = CSL_FEXT(hDMA->dmaRegs->DMACH1TCR2,
                                       DMA_DMACH1TCR2_LTSTATUS);
                 break;

            case CSL_DMA_CHAN2:
                txferStatus = CSL_FEXT(hDMA->dmaRegs->DMACH2TCR2,
                                       DMA_DMACH2TCR2_LTSTATUS);
                 break;

            case CSL_DMA_CHAN3:
                txferStatus = CSL_FEXT(hDMA->dmaRegs->DMACH3TCR2,
                                       DMA_DMACH3TCR2_LTSTATUS);
                 break;
        }
    }

    return (txferStatus);
}

#endif

