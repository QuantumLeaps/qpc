/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_dat.c
 *
 *  @brief DAT functional layer API defenition file
 *
 *  Path: \(CSLPATH)/src
 */

/* ============================================================================
 * Revision History
 * ================
 * 16-Sept-2008 Added DAT of CSL.
 * ============================================================================
 */

#include "csl_dat.h"

/** ===========================================================================
 *   @n@b DAT_open
 *
 *   @b Description
 *   @n This function returns the handle to the DAT
 *      instance. This handle is passed to all other CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            chanNum        channel number to use.
            chanPriority   channel priority.
            pDatChanObj    pointer to dat channel object.
            status         Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_DAT_Handle
 *   @n                     Valid DAT handle will be returned if
 *                          status value is equal to CSL_SOK.
 *
 *   <b> Pre Condition </b>
 *   @n  Before Calling this API User must configure the system control register
 *       to enabling the clock for DMA Controller.
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is returned in the status variable. If status
 *              returned is
 *   @li            CSL_SOK            - Valid DAT handle is returned
 *   @li            CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   @b Modifies
 *   @n    1. The status variable
 *
 *   @b Example
 *   @verbatim

            CSL_status           status;
            CSL_DATChanNum       chanNum;
            Uint16               chanPriority;
            CSL_DAT_ChannelObj   pDatChanObj
            CSL_DAT_Handle       hDAT;
            ...
            chanNum = CSL_DAT_CHAN0;
            chanNum = CSL_DAT_CHAN_PRIORITY_HIGH;

            hDAT = DAT_open(chanNum, chanPriority, &pDatChanObj, &status);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_DAT_Handle DAT_open (
    CSL_DATChanNum      chanNum,
    Uint16              chanPriority,
    CSL_DAT_ChannelObj  *pDatChanObj,
    CSL_Status          *status
)
{
    CSL_DAT_Handle    hDAT;

    if((chanNum >= CSL_DAT_CHAN_INV) || (pDatChanObj == NULL))
    {
          hDAT = NULL;
        *status = CSL_ESYS_INVPARAMS;
    }
    else
    {

        *status                 = CSL_SOK;
        pDatChanObj->chanNum    = chanNum;
        pDatChanObj->chanPrio   = chanPriority;
        pDatChanObj->isChanFree = CSL_DAT_CHANNEL_BUSY;
        /* set the deafault values for channel direction and triggering */
        pDatChanObj->chanPrio  = chanPriority;

        switch( chanNum )
        {
        case CSL_DAT_CHAN0:
        case CSL_DAT_CHAN1:
        case CSL_DAT_CHAN2:
        case CSL_DAT_CHAN3:
            pDatChanObj->dmaRegs    = CSL_DMA0_REGS;
            pDatChanObj->datInstNum = CSL_DAT_ENGINE0;
            hDAT                    = pDatChanObj;
            break;

        case CSL_DAT_CHAN4:
        case CSL_DAT_CHAN5:
        case CSL_DAT_CHAN6:
        case CSL_DAT_CHAN7:
            pDatChanObj->dmaRegs    = CSL_DMA1_REGS;
            pDatChanObj->datInstNum = CSL_DAT_ENGINE1;
            hDAT                    = pDatChanObj;
            break;

        case CSL_DAT_CHAN8:
        case CSL_DAT_CHAN9:
        case CSL_DAT_CHAN10:
        case CSL_DAT_CHAN11:
            pDatChanObj->dmaRegs    = CSL_DMA2_REGS;
            pDatChanObj->datInstNum = CSL_DAT_ENGINE2;
            hDAT                    = pDatChanObj;
            break;

        case CSL_DAT_CHAN12:
        case CSL_DAT_CHAN13:
        case CSL_DAT_CHAN14:
        case CSL_DAT_CHAN15:
            pDatChanObj->dmaRegs    = CSL_DMA3_REGS;
            pDatChanObj->datInstNum = CSL_DAT_ENGINE3;
            hDAT                    = pDatChanObj;
            break;

        }
    }
    return hDAT;
}

/** ===========================================================================
 *   @n@b DAT_close
 *
 *   @b Description
 *   @n This function closes the specified handle to DAT.
 *
 *   @b Arguments
 *   @verbatim
            hDAT            Handle to the DAT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  DAT_open() API must be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  All the registers of this DMA channel will be reset.
 *
 *   @b Example
 *   @verbatim
            CSL_status           status;
            CSL_DATChanNum       chanNum;
            Uint16               chanPriority;
            CSL_DAT_ChannelObj   pDatChanObj
            CSL_DAT_Handle       hDAT;
            ...
            chanNum = CSL_DAT_CHAN0;
            chanNum = CSL_DAT_CHAN_PRIORITY_HIGH;

            hDAT = DAT_open(chanNum, chanPriority, &pDatChanObj, &status);

            status = DAT_Close(hDAT);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DAT_close (
    CSL_DAT_Handle         hDAT
)
{
    Uint16 chanNum;

    if(hDAT ==  NULL)
    {
        return CSL_ESYS_BADHANDLE;
    }
    else
    {
        chanNum = hDAT->chanNum;

        while(chanNum >= CSL_DMA_PER_CNT)
        {
            chanNum = chanNum - CSL_DMA_PER_CNT;
        }

        switch(chanNum)
        {
        case CSL_DAT_CHAN0:
            hDAT->dmaRegs->DMACH0SSAL = CSL_DMA_DMACH0SSAL_RESETVAL;
            hDAT->dmaRegs->DMACH0SSAU = CSL_DMA_DMACH0SSAU_RESETVAL;
            hDAT->dmaRegs->DMACH0DSAL = CSL_DMA_DMACH0DSAL_RESETVAL;
            hDAT->dmaRegs->DMACH0DSAU = CSL_DMA_DMACH0DSAU_RESETVAL;
            hDAT->dmaRegs->DMACH0TCR1  = CSL_DMA_DMACH0TCR1_RESETVAL;
            hDAT->dmaRegs->DMACH0TCR2  = CSL_DMA_DMACH0TCR2_RESETVAL;
            break;
        case CSL_DAT_CHAN1:
            hDAT->dmaRegs->DMACH1SSAL = CSL_DMA_DMACH1SSAL_RESETVAL;
            hDAT->dmaRegs->DMACH1SSAU = CSL_DMA_DMACH1SSAU_RESETVAL;
            hDAT->dmaRegs->DMACH1DSAL = CSL_DMA_DMACH1DSAL_RESETVAL;
            hDAT->dmaRegs->DMACH1DSAU = CSL_DMA_DMACH1DSAU_RESETVAL;
            hDAT->dmaRegs->DMACH1TCR1  = CSL_DMA_DMACH1TCR1_RESETVAL;
            hDAT->dmaRegs->DMACH1TCR2  = CSL_DMA_DMACH1TCR2_RESETVAL;
            break;
        case CSL_DAT_CHAN2:
            hDAT->dmaRegs->DMACH2SSAL = CSL_DMA_DMACH2SSAL_RESETVAL;
            hDAT->dmaRegs->DMACH2SSAU = CSL_DMA_DMACH2SSAU_RESETVAL;
            hDAT->dmaRegs->DMACH2DSAL = CSL_DMA_DMACH2DSAL_RESETVAL;
            hDAT->dmaRegs->DMACH2DSAU = CSL_DMA_DMACH2DSAU_RESETVAL;
            hDAT->dmaRegs->DMACH2TCR1  = CSL_DMA_DMACH2TCR1_RESETVAL;
            hDAT->dmaRegs->DMACH2TCR2  = CSL_DMA_DMACH2TCR2_RESETVAL;
            break;
        case CSL_DAT_CHAN3:
            hDAT->dmaRegs->DMACH3SSAL = CSL_DMA_DMACH3SSAL_RESETVAL;
            hDAT->dmaRegs->DMACH3SSAU = CSL_DMA_DMACH3SSAU_RESETVAL;
            hDAT->dmaRegs->DMACH3DSAL = CSL_DMA_DMACH3DSAL_RESETVAL;
            hDAT->dmaRegs->DMACH3DSAU = CSL_DMA_DMACH3DSAU_RESETVAL;
            hDAT->dmaRegs->DMACH3TCR1  = CSL_DMA_DMACH3TCR1_RESETVAL;
            hDAT->dmaRegs->DMACH3TCR2  = CSL_DMA_DMACH3TCR2_RESETVAL;
            break;
        }

        hDAT->chanNum           = CSL_DAT_CHAN_INV;
        hDAT->chanPrio          = CSL_DAT_CHAN_PRIORITY_LOW;
        hDAT->isChanFree        = CSL_DAT_CHANNEL_FREE;
        hDAT->dmaRegs           = NULL;
    }
    return CSL_SOK;
}

/** ===========================================================================
 *   @n@b DAT_copy
 *
 *   @b Description
 *   @n It configures the DMA Controller registers of particular handle by some
 *      default values and trigger the transfer.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA.
            srcAddr         source location .
            destAddr        destination location .
            dataLength      length of data to transfer.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Parameters are invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  DAT_open() API must be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  DAT_wait() API must be called after this.
 *
 *   @b Example
 *   @verbatim

      #define CSL_DAT_BUFFER_LENGTH 100

            CSL_DAT_ChannelObj   pDatChanObj
            CSL_DAT_Handle       hDAT;
            CSL_DAT_TxType       type;
            CSL_DATChanNum       chanNum;
            Uint16               chanPriority;
            Uint32               srcAddr;
            Uint32               destAddr;
            Uint16               dataLength;
            CSL_status           status;

     Uint16 srcBuffer[CSL_DAT_BUFFER_LENGTH]
     Uint16 destBuffer[CSL_DAT_BUFFER_LENGTH]
                ...
            chanNum = CSL_DAT_CHAN0;
            chanNum = CSL_DAT_CHAN_PRIORITY_HIGH;

            hDAT = DAT_open(chanNum, chanPriority, &pDatChanObj, &status);
            ...
            srcAddr      = srcBuffer;
            destAddr     = destBuffer;
            dataLength   = CSL_DAT_BUFFER_LENGTH;
            status = DAT_copy(hDAT, srcAddr, destAddr, dataLength);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status DAT_copy (
    CSL_DAT_Handle            hDAT,
    Uint32                    srcAddr,
    Uint32                    destAddr,
    Uint16                    dataLength
)
{
    Uint16           chanNum;
    Uint16           srcAddrLSB;
    Uint16           srcAddrMSB;
    Uint16           destAddrLSB;
    Uint16           destAddrMSB;

    if(hDAT ==  NULL)
    {
        return CSL_ESYS_BADHANDLE;
    }

    if(dataLength < CSL_DMA_MIN_TX_SIZE)
    {
        return CSL_ESYS_INVPARAMS;
    }

    if((srcAddr >= CSL_DMA_DARAM_START_ADDR)
                  && (srcAddr <= CSL_DMA_DARAM_END_ADDR))
    {
        srcAddr  = (srcAddr << CSL_DMA_ADDR_SHIFT)
                                     + CSL_DMA_DARAM_ADDR_OFFSET;
    }
    else if((srcAddr >= CSL_DMA_SARAM_START_ADDR)
                   && (srcAddr <= CSL_DMA_SARAM_END_ADDR))
    {
        srcAddr  = (srcAddr << CSL_DMA_ADDR_SHIFT)
                                     + CSL_DMA_SARAM_ADDR_OFFSET;
    }
    else
    {
        srcAddr  =  srcAddr;
    }

    if((destAddr >= CSL_DMA_DARAM_START_ADDR)
                  && (destAddr <= CSL_DMA_DARAM_END_ADDR))
    {
        destAddr  = (destAddr << CSL_DMA_ADDR_SHIFT)
                                        + CSL_DMA_DARAM_ADDR_OFFSET;
    }
    else if((destAddr >= CSL_DMA_SARAM_START_ADDR)
                   && (destAddr <= CSL_DMA_SARAM_END_ADDR))
    {
        destAddr  = (destAddr << CSL_DMA_ADDR_SHIFT)
                                        + CSL_DMA_SARAM_ADDR_OFFSET;
    }
    else
    {
        destAddr  =  destAddr;
    }
    srcAddrLSB = srcAddr & CSL_DMA_UINT16_MASK;
    srcAddrMSB = (Uint16)(srcAddr >> CSL_DMA_UINT16_NUMBER_BITS);
    destAddrLSB = destAddr & CSL_DMA_UINT16_MASK;
    destAddrMSB = (Uint16)(destAddr >> CSL_DMA_UINT16_NUMBER_BITS);

    chanNum        = hDAT->chanNum;
    while(chanNum >= CSL_DMA_PER_CNT)
    {
        chanNum = chanNum - CSL_DMA_PER_CNT;
    }

    switch((CSL_DATChanNum)chanNum)
    {
    case CSL_DAT_CHAN0:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_EN, CSL_DAT_CHANNEL_DISABLE);
        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDAT->dmaRegs->DMACH0SSAL,
                                DMA_DMACH0SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH0SSAU,
                                DMA_DMACH0SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH0DSAL,
                                DMA_DMACH0DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH0DSAU,
                                DMA_DMACH0DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_BURSTMODE,
                                CSL_DAT_TXBURST_WORD_LENGTH);
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_SRCAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_DSTAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR1,
                                DMA_DMACH0TCR1_LENGTH, dataLength);
        /* start the transfer */
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR2,
                       DMA_DMACH0TCR2_EN, CSL_DAT_CHANNEL_ENABLE);
        break;

    case CSL_DAT_CHAN1:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_EN, CSL_DAT_CHANNEL_DISABLE);
        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDAT->dmaRegs->DMACH1SSAL,
                                DMA_DMACH1SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH1SSAU,
                                DMA_DMACH1SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH1DSAL,
                                DMA_DMACH1DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH1DSAU,
                                DMA_DMACH1DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_BURSTMODE,
                                CSL_DAT_TXBURST_WORD_LENGTH);
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_SRCAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_DSTAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR1,
                                DMA_DMACH1TCR1_LENGTH, dataLength);
        /* start the transfer */
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR2,
                       DMA_DMACH1TCR2_EN, CSL_DAT_CHANNEL_ENABLE);
        break;

    case CSL_DAT_CHAN2:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_EN, CSL_DAT_CHANNEL_DISABLE);
        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDAT->dmaRegs->DMACH2SSAL,
                                DMA_DMACH2SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH2SSAU,
                                DMA_DMACH2SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH2DSAL,
                                DMA_DMACH2DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH2DSAU,
                                DMA_DMACH2DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_BURSTMODE,
                                CSL_DAT_TXBURST_WORD_LENGTH);
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_SRCAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_DSTAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR1,
                                DMA_DMACH2TCR1_LENGTH, dataLength);
        /* start the transfer */
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR2,
                       DMA_DMACH2TCR2_EN, CSL_DAT_CHANNEL_ENABLE);
        break;

    case CSL_DAT_CHAN3:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_EN, CSL_DAT_CHANNEL_DISABLE);
        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDAT->dmaRegs->DMACH3SSAL,
                                DMA_DMACH3SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH3SSAU,
                                DMA_DMACH3SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH3DSAL,
                                DMA_DMACH3DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH3DSAU,
                                DMA_DMACH3DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_BURSTMODE,
                                CSL_DAT_TXBURST_WORD_LENGTH);
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_SRCAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_DSTAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR1,
                                DMA_DMACH3TCR1_LENGTH, dataLength);
        /* start the transfer */
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR2,
                       DMA_DMACH3TCR2_EN, CSL_DAT_CHANNEL_ENABLE);
        break;
    }

    return CSL_SOK;
}

/** ===========================================================================
 *   @n@b DAT_fill
 *
 *   @b Description
 *   @n It configures the DMA Controller registers of particular handle by some
 *      default values and fill the destination location with passed data value.
 *
 *   @b Arguments
 *   @verbatim
            hDMA            Handle to the DMA.
            destAddr        destination location.
            pdataValue      pointer to the data.
            dataLength      length of data to transfer.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Parameters are invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  DAT_open() API must be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  DAT_wait() API must be called after this.
 *
 *   @b Example
 *   @verbatim

      #define CSL_DAT_BUFFER_LENGTH 100

            CSL_DAT_ChannelObj   pDatChanObj
            CSL_DAT_Handle       hDAT;
            CSL_DAT_TxType       type;
            CSL_DATChanNum       chanNum;
            Uint16               chanPriority;

            Uint32           srcAddr;
            Uint32           destAddr;
            Uint16           dataLength;
            CSL_status       status;
            Uint32           pdataValue;

 Uint16 destBuffer[CSL_DAT_BUFFER_LENGTH]
            ...
            chanNum = CSL_DAT_CHAN0;
            chanNum = CSL_DAT_CHAN_PRIORITY_HIGH;

            hDAT = DAT_open(chanNum, chanPriority, &pDatChanObj, &status);
            ...
            destAddr     = destBuffer;
            dataLength   = CSL_DAT_BUFFER_LENGTH;
            pdataValue   = 0xABCD;

            status = DAT_fill(hDAT, destAddr, dataLength, &pdataValue);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status DAT_fill (
    CSL_DAT_Handle            hDAT,
    Uint32                    destAddr,
    Uint16                    dataLength,
    Uint32                    *pdataValue
)
{
    Uint16           chanNum;
    Uint16           srcAddrLSB;
    Uint16           srcAddrMSB;
    Uint16           destAddrLSB;
    Uint16           destAddrMSB;
    Uint32           srcAddr;

    if(hDAT ==  NULL)
    {
        return CSL_ESYS_BADHANDLE;
    }

    if((pdataValue ==  NULL) || (dataLength < CSL_DMA_MIN_TX_SIZE))
    {
        return CSL_ESYS_INVPARAMS;
    }

    srcAddr = (Uint32)pdataValue;
    if((srcAddr >= CSL_DMA_DARAM_START_ADDR)
                  && (srcAddr <= CSL_DMA_DARAM_END_ADDR))
    {
        srcAddr  = (srcAddr << CSL_DMA_ADDR_SHIFT)
                                     + CSL_DMA_DARAM_ADDR_OFFSET;
    }
    else if((srcAddr >= CSL_DMA_SARAM_START_ADDR)
                   && (srcAddr <= CSL_DMA_SARAM_END_ADDR))
    {
        srcAddr  = (srcAddr << CSL_DMA_ADDR_SHIFT)
                                     + CSL_DMA_SARAM_ADDR_OFFSET;
    }
    else
    {
        srcAddr  =  srcAddr;
    }

    if((destAddr >= CSL_DMA_DARAM_START_ADDR)
                  && (destAddr <= CSL_DMA_DARAM_END_ADDR))
    {
        destAddr  = (destAddr << CSL_DMA_ADDR_SHIFT)
                                        + CSL_DMA_DARAM_ADDR_OFFSET;
    }
    else if((destAddr >= CSL_DMA_SARAM_START_ADDR)
                   && (destAddr <= CSL_DMA_SARAM_END_ADDR))
    {
        destAddr  = (destAddr << CSL_DMA_ADDR_SHIFT)
                                        + CSL_DMA_SARAM_ADDR_OFFSET;
    }
    else
    {
        destAddr  =  destAddr;
    }
    srcAddrLSB = srcAddr & CSL_DMA_UINT16_MASK;
    srcAddrMSB = (Uint16)(srcAddr
                             >> CSL_DMA_UINT16_NUMBER_BITS);
    destAddrLSB = destAddr & CSL_DMA_UINT16_MASK;
    destAddrMSB = (Uint16)(destAddr >> CSL_DMA_UINT16_NUMBER_BITS);

    chanNum        = hDAT->chanNum;
    while(chanNum >= CSL_DMA_PER_CNT)
    {
        chanNum = chanNum - CSL_DMA_PER_CNT;
    }

    switch((CSL_DATChanNum)chanNum)
    {
    case CSL_DAT_CHAN0:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_EN, CSL_DAT_CHANNEL_DISABLE);
        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDAT->dmaRegs->DMACH0SSAL,
                                DMA_DMACH0SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH0SSAU,
                                DMA_DMACH0SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH0DSAL,
                                DMA_DMACH0DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH0DSAU,
                                DMA_DMACH0DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_BURSTMODE,
                                CSL_DAT_TXBURST_WORD_LENGTH);
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_SRCAMODE,
                                CSL_DMA_ADDR_MODE_FIXED);
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR2,
                                DMA_DMACH0TCR2_DSTAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR1,
                                DMA_DMACH0TCR1_LENGTH, dataLength);
        /* start the transfer */
        CSL_FINS(hDAT->dmaRegs->DMACH0TCR2,
                       DMA_DMACH0TCR2_EN, CSL_DAT_CHANNEL_ENABLE);
        break;

    case CSL_DAT_CHAN1:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_EN, CSL_DAT_CHANNEL_DISABLE);
        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDAT->dmaRegs->DMACH1SSAL,
                                DMA_DMACH1SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH1SSAU,
                                DMA_DMACH1SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH1DSAL,
                                DMA_DMACH1DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH1DSAU,
                                DMA_DMACH1DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_BURSTMODE,
                                CSL_DAT_TXBURST_WORD_LENGTH);
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_SRCAMODE,
                                CSL_DMA_ADDR_MODE_FIXED);
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR2,
                                DMA_DMACH1TCR2_DSTAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR1,
                                DMA_DMACH1TCR1_LENGTH, dataLength);
        /* start the transfer */
        CSL_FINS(hDAT->dmaRegs->DMACH1TCR2,
                       DMA_DMACH1TCR2_EN, CSL_DAT_CHANNEL_ENABLE);
        break;

    case CSL_DAT_CHAN2:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_EN, CSL_DAT_CHANNEL_DISABLE);
        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDAT->dmaRegs->DMACH2SSAL,
                                DMA_DMACH2SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH2SSAU,
                                DMA_DMACH2SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH2DSAL,
                                DMA_DMACH2DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH2DSAU,
                                DMA_DMACH2DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_BURSTMODE,
                                CSL_DAT_TXBURST_WORD_LENGTH);
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_SRCAMODE,
                                CSL_DMA_ADDR_MODE_FIXED);
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR2,
                                DMA_DMACH2TCR2_DSTAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR1,
                                DMA_DMACH2TCR1_LENGTH, dataLength);
        /* start the transfer */
        CSL_FINS(hDAT->dmaRegs->DMACH2TCR2,
                       DMA_DMACH2TCR2_EN, CSL_DAT_CHANNEL_ENABLE);
        break;

    case CSL_DAT_CHAN3:
        /* Disable the channel enable bit before configuring other DMA Regisers*/
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_EN, CSL_DAT_CHANNEL_DISABLE);
        /* Configure the DMA Registers for passed DMA Handle */
        CSL_FINS(hDAT->dmaRegs->DMACH3SSAL,
                                DMA_DMACH3SSAL_SSAL, srcAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH3SSAU,
                                DMA_DMACH3SSAU_SSAU, srcAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH3DSAL,
                                DMA_DMACH3DSAL_DSAL, destAddrLSB);
        CSL_FINS(hDAT->dmaRegs->DMACH3DSAU,
                                DMA_DMACH3DSAU_DSAU, destAddrMSB);
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_BURSTMODE,
                                CSL_DAT_TXBURST_WORD_LENGTH);
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_SRCAMODE,
                                CSL_DMA_ADDR_MODE_FIXED);
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR2,
                                DMA_DMACH3TCR2_DSTAMODE,
                                CSL_DMA_ADDR_MODE_INCREMENT);
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR1,
                                DMA_DMACH3TCR1_LENGTH, dataLength);
        /* start the transfer */
        CSL_FINS(hDAT->dmaRegs->DMACH3TCR2,
                       DMA_DMACH3TCR2_EN, CSL_DAT_CHANNEL_ENABLE);
        break;
    }

    return CSL_SOK;
}

/** ===========================================================================
 *   @n@b DAT_wait
 *
 *   @b Description
 *   @n This function wait till the completion of Data Transfer.
 *
 *   @b Arguments
 *   @verbatim
            hDAT            Handle to the DAT
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  DAT_open() API must be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Example
 *   @verbatim
      #define CSL_DAT_BUFFER_LENGTH 100

            CSL_DAT_ChannelObj   pDatChanObj
            CSL_DAT_Handle       hDAT;
            CSL_DAT_TxType       type;
            CSL_DATChanNum       chanNum;
            Uint16               chanPriority;

            Uint32           srcAddr;
            Uint32           destAddr;
            Uint16           dataLength;
            CSL_status       status;
            Uint32           pdataValue;

     Uint16 destBuffer[CSL_DAT_BUFFER_LENGTH]
            ...
            chanNum = CSL_DAT_CHAN0;
            chanNum = CSL_DAT_CHAN_PRIORITY_HIGH;

            hDAT = DAT_open(chanNum, chanPriority, &pDatChanObj, &status);
            ...
            destAddr     = destBuffer;
            dataLength   = CSL_DAT_BUFFER_LENGTH;
            pdataValue   = 0xABCD;

            status   = DAT_fill(hDAT, destAddr, dataLength, &pdataValue);

            status = DAT_wait(hDAT);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status DAT_wait (
    CSL_DAT_Handle         hDAT
)
{
    Bool           status;
    Uint16         chanNum;
    Uint16         timeOut;

    status = TRUE;
    if(hDAT ==  NULL)
    {
        return CSL_ESYS_BADHANDLE;
    }
    chanNum = hDAT->chanNum;

    while(chanNum >= CSL_DMA_PER_CNT)
    {
        chanNum = chanNum - CSL_DMA_PER_CNT;
    }

    switch((CSL_DATChanNum)chanNum)
    {
    case CSL_DAT_CHAN0:
        timeOut = CSL_DAT_TIME_OUT;
        while( status && timeOut-- )
        {
            status = CSL_FEXT(hDAT->dmaRegs->DMACH0TCR2, DMA_DMACH0TCR2_EN);
        }
        break;

    case CSL_DAT_CHAN1:
        timeOut = CSL_DAT_TIME_OUT;
        while( status && timeOut-- )
        {
            status = CSL_FEXT(hDAT->dmaRegs->DMACH1TCR2, DMA_DMACH1TCR2_EN);
        }
        break;

    case CSL_DAT_CHAN2:
        timeOut = CSL_DAT_TIME_OUT;
        while( status && timeOut-- )
        {
            status = CSL_FEXT(hDAT->dmaRegs->DMACH2TCR2, DMA_DMACH2TCR2_EN);
        }
        break;

    case CSL_DAT_CHAN3:
        timeOut = CSL_DAT_TIME_OUT;
        while( status && timeOut-- )
        {
            status = CSL_FEXT(hDAT->dmaRegs->DMACH3TCR2, DMA_DMACH3TCR2_EN);
        }
        break;

    }
    return CSL_SOK;
}














































































































































































































































