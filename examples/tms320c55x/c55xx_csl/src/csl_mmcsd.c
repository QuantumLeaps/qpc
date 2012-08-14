/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_mmcsd.c
 *
 *  @brief MMCSD functional layer API definition file
 *
 *  Path: \(CSLPATH)/src
 */

/* ============================================================================
 * Revision History
 * ================
 * 05-Sep-2008 Added source file of MMCSD CSL.
 * 16-Jun-2009 Added DMA mode support
 * 26-Jun-2009 Code modified as per review comments
 * 26-Jun-2009 Added SDHC card support
 * 02-Jul-2009 Removed magic numbers as per review comments
 * 25-Feb-2010 Added second callback to MMC_write due to excessive time spent
 *             in while loop KR022510
 * 04-Mar-2010 Added second callback to MMC_read due to excessive time spent
 *             in while loop
 * 05-Mar-2010 Changed all routines to test for errors and improved robustness
 *             of driver (pedro)
 * 13-Mar-2010 MMC_read  - free up more idle clocks by sync to data done
 *                         status/interrupt
 *                       - replaced writeDoneCallBack based on busy done at
 *                         beginning with while busy and FIFO not empty loop
 *                       - replaced DMAcallback and while not data done with
 *                         writeDoneCallback based on data done at end
 *             MMC_write - free up more idle clocks by sync to data done
 *                         status/interrupt
 *                       - replaced writeDoneCallBack based on busy done at
 *                         beginning with while FIFO not empty and busy
 *                       - replace dmaCallback with writeDoneCallBack based on
 *                         data done at end
 * 18-Mar-2010 MMC_setWriteDoneCallback - modified to work for poll and DMA mode
 *             MMC_read  - add additional status read in line 3798
 * 20-Mar-2010 MMC_read  - moved ReIssueReadFlag inside do while loop to avoid multiple block hang    KR032010
 *                       - modified multiple block at end and added Pedro's response                  KR032010
 *                       - added break after reIssueReadFlag                                          KR032010
 *                       - moved multiple block send stop command at end                              KR032010
 *                       - added Pedro's response                                                     KR032010
 *             MMC_write - moved ReIssueWriteFlag inside do while loop to avoid multiple block hang   KR032010
 *                       - modified multiple block at end and added Pedro's response                  KR032010
 *                       - added break (while) after reIssueWriteFlag                                 KR032010
 *                       - moved multiple block send stop command at end                              KR032010
 *                       - added Pedro's response                                                     KR032010
 *             MMC_write - split do while loop into two                                               KR032210
 * 05-Apr-2010 - Incorporated Frank's comments
 * 06-Apr-2010 - Removed MMC_setDmaCallback() API as the functionality of
 *               DMA callback function is replaced by write done callback function
 * 06-Apr-2010 - Renamed write done callback function to data transfer callback
 *               since it is used by both MMC write and read APIs. Renamed
 *               all the related functions and data structures
 * 07-Apr-2010 - Modified MMC_selectCard API to work with MMC and SD cards
 * 08-Apr-2010 - Formatted the code to have uniform style
 * 09-Apr-2010 - Corrected reading of the CSD and CID registers
 * 09-Apr-2010 MMC_write - Added wait for response after sending write command
 *                         In case of errors ReIssueWriteFlag will be set and loop
 *                         continues to re-try.
 *                       - Optimized the code by removing redundant instructions
 *             MMC_read  - Optimized the code by removing redundant instructions
 * 12-Apr-2010 - Removed the global variable 'gIsrMMCST0'.
 *               Modified MMC_read and MMC_write API to receive MMCST0 register
 *               as a return value from data transfer callback function.
 *             - Merged the code with CSL PG2.0 modifications
 *               DMA word swap will be based on the endian mode configuration
 *               rather than the global variable 'ATA_No_Swap'.
 *               Added API MMC_setEndianMode() to configure the endian mode
 *               of the MMC read/write operations
 * 15-Apr-2010 - Added interrupt mode support for Read and Write operations
 * 16-Apr-2010 - Added API SD_configurePullup() to configure SD card pull-up
 *               resistor
 * 18-Apr-2010 - Added time-out count for the read and write command re-tries
 *               in MMC_read and MMC_write APIs
 * 19-Apr-2010 - Added APIs
 *             - MMC_setBlockLength()
 *             - MMC_setWriteBlkEraseCnt()
 *             - SD_setBusWidth()
 *             - MMC_getCardStatus()
 * 20-Apr-2010 - Added time-out count for the STOP command in MMC_read and MMC_write
 * 25-Apr-2010
 *             MMC_sendAllCID - Removed sending the commands CMD0,CMD1
 *             SD_sendAllCID  - Removed sending the commands CMD0,CMD8,ACMD41
 *             SD_getCardCsd  - Removed sending the commands CMD0,CMD8,ACMD41,
 *                              CMD2, CMD3
 *             MMC_getCardCsd - Removed sending the commands CMD0,CMD1,CMD2,CMD3
 * 30-Apr-2010 - Incorporated internal review comments
 * 14-June-2010 - Added MMCSD interrupt enable and disable using IER to prevent
 *                     the possibble interrupt lost in the multiple thread situation
 * ============================================================================
 */

#include <csl_mmcsd.h>
#include <csl_intc.h>

CSL_Status MMC_checkReadDmaDone(void *hMmcSd);

/** ===========================================================================
 *   @n@b MMC_init
 *
 *   @b Description
 *   @n This is the initialization function for the MMCSD CSL. The function
 *      must be called before calling any other API from MMCSD CSL. This
 *      function is for enabling the clock to the MMCSD Controller and routing
 *      MMCSD pins to the external serial port.
 *      This function should be called only once during system initialization
 *      time. This function gates the clock for the two MMCSD modules.
 *      Application programs may disable the clocks for MMCSD module which is
 *      not in use.
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li            CSL_SOK            - MMCSD initialization successfully
 *
 *   <b> Pre Condition </b>
 *   @n  This should be call first before calling any other MMCSD Function.
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

            status = MMC_init();
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_init(void)
{
    /* Enable clock for MMCSD0  and MMCSD1 module */
    CSL_FINST(CSL_SYSCTRL_REGS->PCGCR1, SYS_PCGCR1_MMCSD0CG, ACTIVE);
    CSL_FINST(CSL_SYSCTRL_REGS->PCGCR2, SYS_PCGCR1_MMCSD1CG, ACTIVE);

    /* Set the reset clock cycle */
    CSL_FINS(CSL_SYSCTRL_REGS->PSRCR, SYS_PSRCR_COUNT, CSL_MMCSD_RESET_COUNT);
    CSL_FINST(CSL_SYSCTRL_REGS->PRCR, SYS_PRCR_PG3_RST, RST);

    /* Enable Serial port 0 pin for mmcsd0  */
    CSL_FINST(CSL_SYSCTRL_REGS->EBSR, SYS_EBSR_SP0MODE, MODE0);
    /* Enable Serial port 1 pin for mmcsd1  */
    CSL_FINST(CSL_SYSCTRL_REGS->EBSR, SYS_EBSR_SP1MODE, MODE0);

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b MMC_open
 *
 *   @b Description
 *   @n This function opens the CSL MMCSD module and returns handle to the
 *      MMCSD object structure. Parameter 'opMode' is provided to select the
 *      operating mode of the MMCSD module. Opmode information is stored in
 *      the MMCSD handle and is used in data transfer APIs.
 *
 *      NOTE: THIS API SETS THE ENDIAN MODE OF MMCSD READ AND WRITE OPERATIONS
 *            TO LITTLE ENDIAN. USE MMC_setEndianMode() TO CHANGE ENDIAN MODE.
 *
 *   @b Arguments
 *   @verbatim
            pMmcsdContObj    MMCSD Controller object structure
            instId           Instance number for MMCSD
            opMode           Operating mode of the MMCSD module
            status           Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_MmmcsdHandle
 *   @n                     Valid MMCSD handle will be returned if
 *                          status value is equal to CSL_SOK.
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_init() API should be called before MMC_open().
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is returned in the status variable. If status
 *              returned is
 *   @li            CSL_SOK            - Valid MMCSD handle is returned
 *   @li            CSL_ESYS_INVPARAMS - resource is already in use
 *   @n   2.    CSL_MmmcsdObj object structure is populated
 *
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. CSL_MmmcsdObj object structure
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;
            status =  MMC_init();
            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_MmcsdHandle MMC_open(CSL_MMCControllerObj    *pMmcsdContObj,
                         CSL_MmcsdInstId         instId,
                         CSL_MMCSDOpMode         opMode,
                         CSL_Status              *status)
{
    CSL_MmcsdHandle    hMmcsd;

    if((pMmcsdContObj == NULL)                 ||
       ((opMode != CSL_MMCSD_OPMODE_POLLED)    &&
        (opMode != CSL_MMCSD_OPMODE_INTERRUPT) &&
        (opMode != CSL_MMCSD_OPMODE_DMA)))
    {
        if(status != NULL)
        {
            *status = CSL_ESYS_INVPARAMS;
        }
        hMmcsd  = NULL;
    }
    else if(status == NULL)
    {
        hMmcsd = NULL;
    }
    else
    {
        switch (instId)
        {
            case CSL_MMCSD0_INST:
                *status                = CSL_SOK;
                pMmcsdContObj->mmcRegs = CSL_MMCSD0_REGS;
                hMmcsd                 = pMmcsdContObj;
                break;

            case CSL_MMCSD1_INST:
                *status                = CSL_SOK;
                pMmcsdContObj->mmcRegs = CSL_MMCSD1_REGS;
                hMmcsd                 = pMmcsdContObj;
                break;

            default:
                *status                = CSL_ESYS_INVPARAMS;
                pMmcsdContObj->mmcRegs = NULL;
                hMmcsd                 = NULL;
                break;
        }

        hMmcsd->opMode = opMode;

        hMmcsd->readEndianMode  = CSL_MMCSD_ENDIAN_LITTLE;
        hMmcsd->writeEndianMode = CSL_MMCSD_ENDIAN_LITTLE;
        hMmcsd->isCallbackSet   = FALSE;
        hMmcsd->blockLen        = 0;

        /* Set Endian mode for write operation */
        CSL_FINS (hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_PERMDX,
                  hMmcsd->writeEndianMode);

        /* Set Endian mode for read operation */
        CSL_FINS (hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_PERMDR,
                  hMmcsd->readEndianMode);

        if((CSL_SOK == *status) && (CSL_MMCSD_OPMODE_DMA == hMmcsd->opMode))
        {
            hMmcsd->hDmaWrite     = NULL;
            hMmcsd->hDmaRead      = NULL;

            /* Assign values for DMA config structures */
#if(defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
               hMmcsd->dmaWriteCfg.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
#endif
            hMmcsd->dmaWriteCfg.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            hMmcsd->dmaWriteCfg.burstLen     = CSL_DMA_TXBURST_8WORD;
            hMmcsd->dmaWriteCfg.chanDir      = CSL_DMA_WRITE;
            hMmcsd->dmaWriteCfg.dmaInt       = CSL_DMA_INTERRUPT_DISABLE;
            hMmcsd->dmaWriteCfg.trfType      = CSL_DMA_TRANSFER_IO_MEMORY;
            hMmcsd->dmaWriteCfg.trigger      = CSL_DMA_EVENT_TRIGGER;
            hMmcsd->dmaWriteCfg.srcAddr      = (Uint32)NULL;
            hMmcsd->dmaWriteCfg.dataLen      = 0;
            hMmcsd->dmaWriteCfg.destAddr     =
                                     (Uint32)&pMmcsdContObj->mmcRegs->MMCDXR1;

#if(defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
               hMmcsd->dmaReadCfg.pingPongMode = CSL_DMA_PING_PONG_DISABLE;
#endif
            hMmcsd->dmaReadCfg.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            hMmcsd->dmaReadCfg.burstLen     = CSL_DMA_TXBURST_8WORD;
            hMmcsd->dmaReadCfg.chanDir      = CSL_DMA_READ;
            hMmcsd->dmaReadCfg.dmaInt       = CSL_DMA_INTERRUPT_DISABLE;
            hMmcsd->dmaReadCfg.trfType      = CSL_DMA_TRANSFER_IO_MEMORY;
            hMmcsd->dmaReadCfg.trigger      = CSL_DMA_EVENT_TRIGGER;
            hMmcsd->dmaReadCfg.destAddr     = (Uint32)NULL;
            hMmcsd->dmaReadCfg.dataLen      = 0;
            hMmcsd->dmaReadCfg.srcAddr      =
                                      (Uint32)&pMmcsdContObj->mmcRegs->MMCDRR1;

            if(CSL_MMCSD0_INST == instId)
            {
                hMmcsd->dmaWriteCfg.dmaEvt = CSL_DMA_EVT_MMC_SD0_TX;
                hMmcsd->dmaReadCfg.dmaEvt  = CSL_DMA_EVT_MMC_SD0_RX;
            }
            else
            {
                hMmcsd->dmaWriteCfg.dmaEvt = CSL_DMA_EVT_MMC_SD1_TX;
                hMmcsd->dmaReadCfg.dmaEvt  = CSL_DMA_EVT_MMC_SD1_RX;
            }
        }
    }

    return (hMmcsd);
}

/** ===========================================================================
 *   @n@b MMC_close
 *
 *   @b Description
 *   @n This function closes the specified handle to MMCSD.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd    Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully before this.
 *
 *   <b> Post Condition </b>
 *   @n  Closes the MMC handle
 *
 *   @b Modifies
 *   @n MMCSD handle will be assigned to NULL.
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_close(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_close(CSL_MmcsdHandle    hMmcsd)
{
    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        if(hMmcsd->opMode == CSL_MMCSD_OPMODE_DMA)
        {
            /* Close the DMA channels */
            DMA_close(hMmcsd->hDmaWrite);
            DMA_close(hMmcsd->hDmaRead);
        }

        hMmcsd->mmcRegs = NULL;

        return (CSL_SOK);
    }
}

/** ===========================================================================
 *   @n@b MMC_setCardType
 *
 *   @b Description
 *   @n This API just updates the software structure and does not update any
 *      hardware registers
 *
 *   @b Arguments
 *   @verbatim
            pCardObj    Pointer to the card object.
            cardType    type of card MMC or SD.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - successful
 *   @li                    CSL_ESYS_INVPARAMS - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            CSL_MMCCardObj        cardObj;
            CSL_CardType          cardType;

            cardType = CSL_SD_CARD;

            status = MMC_setCardType(&cardObj, cardType);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setCardType(CSL_MMCCardObj    *pCardObj,
                           CSL_CardType      cardType)
{
    if(NULL == pCardObj)
    {
        return (CSL_ESYS_INVPARAMS);
    }
    else
    {
        if((cardType == CSL_SD_CARD) || (cardType == CSL_MMC_CARD))
        {
            pCardObj->cardType = cardType;
            return (CSL_SOK);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    }
}

/** ===========================================================================
 *   @n@b MMC_setCardPtr
 *
 *   @b Description
 *   @n This function will set the card object information address into the
 *      MMCSD handle. Implementation is limited only to the software layer.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            CSL_MMCCardObj    Pointer to the MMCSD Card Object.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n Card object structure
 *
 *   @b Example
 *   @verbatim
            CSL_MMCCardObj        mmcCardObj;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_setCardPtr(hMmcsd, &mmcCardObj);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setCardPtr(CSL_MmcsdHandle    hMmcsd,
                          CSL_MMCCardObj     *pMmcCardObj)
{
    if(hMmcsd == NULL)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        if(NULL == pMmcCardObj)
        {
            return (CSL_ESYS_INVPARAMS);
        }
        else
        {
            hMmcsd->cardObj = pMmcCardObj;
            return (CSL_SOK);
        }
    }
}

/** ===========================================================================
 *   @n@b MMC_setCallBack
 *
 *   @b Description
 *   @n This function sets the address of call back routines which will be
 *      called from MMCSD ISR.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd       Handle to the MMCSD.
            pFunction    Pointer to the isr function.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None.
 *
 *   @b Example
 *   @verbatim
            #define MMCSD_ISR_FUNCTION_MAX (4u)
            void mmcsd0_tx_isr(void);
            void mmcsd0_rcv_isr(void);
            void mmcsd1_tx_isr(void);
            void mmcsd1_rcv_isr(void);

            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            CSL_MMCCallBackObj    callBckFun;

            instId = CSL_MMCSD0_INST;
            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            callBckFun.isr[CSL_MMCSD_ISR_TXMT] = mmcsd_tx_isr;
            callBckFun.isr[CSL_MMCSD_ISR_RCV]  = mmcsd_rcv_isr;

            status = MMC_setCallBack(hMmcsd, &callBckFun);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setCallBack(CSL_MmcsdHandle       hMmcsd,
                           CSL_MMCCallBackObj    *pFunction)
{
    if((NULL != hMmcsd) && (NULL != pFunction))
    {
        hMmcsd->callBackTbl = pFunction;
        return (CSL_SOK);
    }
    else
    {
        if(NULL == hMmcsd)
        {
            return (CSL_ESYS_BADHANDLE);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    }
}

/** ===========================================================================
 *   @n@b MMC_getNumberOfCards
 *
 *   @b Description
 *   @n Get the number of cards information from the MMC handler.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd           Handle to the MMCSD
            pActiveNoCard    pointer to store number of active card info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            Uint16                activeNoCard;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;
            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_getNumberOfCards(hMmcsd, &activeNoCard);

     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_getNumberOfCards(CSL_MmcsdHandle    hMmcsd,
                                Uint16             *pActiveNoCard)
{
    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        if(NULL == pActiveNoCard)
        {
            return (CSL_ESYS_INVPARAMS);
        }
        else
        {
            *pActiveNoCard = hMmcsd->numCardsActive;
            return (CSL_SOK);
        }
    }
}

/** ===========================================================================
 *   @n@b MMC_clearResponse
 *
 *   @b Description
 *   @n This function clears all eight response registers of MMCSD.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd    Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully before this.
 *
 *   <b> Post Condition </b>
 *   @n  Clears response
 *
 *   @b Modifies
 *   @n MMCSD Response Registers.
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_clearResponse(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_clearResponse(CSL_MmcsdHandle    hMmcsd)
{
    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        CSL_FINS(hMmcsd->mmcRegs->MMCRSP0, MMCSD_MMCRSP0_MMCRSP0,
                                        CSL_MMCSD_MMCRSP0_RESETVAL);
        CSL_FINS(hMmcsd->mmcRegs->MMCRSP1, MMCSD_MMCRSP1_MMCRSP1,
                                        CSL_MMCSD_MMCRSP1_RESETVAL);
        CSL_FINS(hMmcsd->mmcRegs->MMCRSP2, MMCSD_MMCRSP2_MMCRSP2,
                                        CSL_MMCSD_MMCRSP2_RESETVAL);
        CSL_FINS(hMmcsd->mmcRegs->MMCRSP3, MMCSD_MMCRSP3_MMCRSP3,
                                        CSL_MMCSD_MMCRSP3_RESETVAL);
        CSL_FINS(hMmcsd->mmcRegs->MMCRSP4, MMCSD_MMCRSP4_MMCRSP4,
                                        CSL_MMCSD_MMCRSP4_RESETVAL);
        CSL_FINS(hMmcsd->mmcRegs->MMCRSP5, MMCSD_MMCRSP5_MMCRSP5,
                                        CSL_MMCSD_MMCRSP5_RESETVAL);
        CSL_FINS(hMmcsd->mmcRegs->MMCRSP6, MMCSD_MMCRSP6_MMCRSP6,
                                        CSL_MMCSD_MMCRSP6_RESETVAL);
        CSL_FINS(hMmcsd->mmcRegs->MMCRSP7, MMCSD_MMCRSP7_MMCRSP7,
                                        CSL_MMCSD_MMCRSP7_RESETVAL);

        /* Clear the command bit index bit also in MMCCIDX register also */
        CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX,
                                        CSL_MMCSD_MMCCIDX_RESETVAL);
        return (CSL_SOK);
    }
}

/** ===========================================================================
 *   @n@b MMC_ierEnable
 *
 *   @b Description
 *   @n This function will enable or the interrupt for MMCSD via IER0.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully before this.
 *
 *   <b> Post Condition </b>
 *   @n  Before enabling interrupt of any type make sure the isr function
 *        is plugged in ISR.
 *
 *   @b Modifies
 *   @n Interrupt will be enable for particular event.
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            Uint16                maskValue;
            CSL_status            status;

               instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_ierEnable(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_ierEnable(
               CSL_MmcsdHandle      hMmcsd)
{
    if(NULL == hMmcsd)
    {
        return CSL_ESYS_BADHANDLE;
    }
    else
    {
        if ((Uint16)(hMmcsd->mmcRegs)==0x3A00)
        {
            CSL_FINST(CSL_CPU_REGS->IER0, CPU_IER0_PROG0, ENABLE);
        } else
        {
            CSL_FINST(CSL_CPU_REGS->IER0, CPU_IER0_PROG2, ENABLE);
        }
        return CSL_SOK;
    }
}

/** ===========================================================================
 *   @n@b MMC_ierDisable
 *
 *   @b Description
 *   @n This function will disable the interrupt for MMCSD via IER0.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully before this.
 *
 *   <b> Post Condition </b>
 *   @n  Before enabling interrupt of any type make sure the isr function
 *        is plugged in ISR.
 *
 *   @b Modifies
 *   @n Interrupt will be enable for particular event.
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            Uint16                maskValue;
            CSL_status            status;

               instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_ierDisable(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_ierDisable(
               CSL_MmcsdHandle      hMmcsd)
{
    if(NULL == hMmcsd)
    {
        return CSL_ESYS_BADHANDLE;
    }
    else
    {
        if ((Uint16)(hMmcsd->mmcRegs)==0x3A00)
        {
            CSL_FINST(CSL_CPU_REGS->IER0, CPU_IER0_PROG0, DISABLE);
        } else
        {
            CSL_FINST(CSL_CPU_REGS->IER0, CPU_IER0_PROG2, DISABLE);
        }
        return CSL_SOK;
    }
}


/** ===========================================================================
 *   @n@b MMC_intEnable
 *
 *   @b Description
 *   @n This function will enable or disable interrupts for MMCSD depending on
 *      the passed mask value for interrupt. MMC interrupt mask register will
 *      be configured by this API. This function is useful to enable the
 *      multiple MMC interrupts in one function call.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd       Handle to the MMCSD
            maskValue    mask value to specify the interrupt state.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully before this.
 *       Before enabling interrupt of any type make sure the isr function
 *       is plugged in ISR.
 *
 *   <b> Post Condition </b>
 *   @n  Enables MMC interrupts
 *
 *   @b Modifies
 *   @n MMC interrupt mask register
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            Uint16                maskValue;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);
            NOTE: Use the bit mask values of MMCIM register defined
                  cslr_mmcsd.h file for making the mask value.

            maskValue = CSL_MMCSD_MMCIM_ETRNDNE_MASK |
                        CSL_MMCSD_MMCIM_EDATED_MASK;

            status = MMC_intEnable(hMmcsd, maskValue);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_intEnable(CSL_MmcsdHandle    hMmcsd,
                         Uint16             maskValue)
{
    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        hMmcsd->mmcRegs->MMCIM = maskValue;
        return (CSL_SOK);
    }
}

/** ===========================================================================
 *   @n@b MMC_eventEnable
 *
 *   @b Description
 *   @n This function enables the MMC events depending on the 'mmcsdEvent'
 *      passed as a parameter. This function is useful to enable single
 *      interrupt of MMC interrupt mask register in one function call.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        Handle to the MMCSD
            mmcsdEvent    Event type.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully.
 *       Before enabling interrupt of any type make sure the isr function
 *       is plugged in ISR.
 *
 *   <b> Post Condition </b>
 *   @n  Interrupt will be enabled for particular event.
 *
 *   @b Modifies
 *   @n MMC interrupt mask register
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_MMCSDEventType    mmcsdEvent;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);
            eventType = CSL_MMCSD_EDATDNE_INTERRUPT;

            status = MMC_eventEnable(hMmcsd, mmcsdEvent);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_eventEnable(CSL_MmcsdHandle       hMmcsd,
                           CSL_MMCSDEventType    mmcsdEvent)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(NULL == hMmcsd)
    {
        status = CSL_ESYS_BADHANDLE;
    }
    else
    {
        switch (mmcsdEvent)
        {
            case CSL_MMCSD_ETRNDNE_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ETRNDNE, PERMIT);
                break;

            case CSL_MMCSD_EDATED_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_EDATED, PERMIT);
                break;

            case CSL_MMCSD_EDRRDY_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_EDRRDY, PERMIT);
                break;

            case CSL_MMCSD_EDXRDY_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_EDXRDY, PERMIT);
                break;

            case CSL_MMCSD_ECRCRS_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ECRCRS, PERMIT);
                break;

            case CSL_MMCSD_ECRCRD_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ECRCRD, PERMIT);
                break;

            case CSL_MMCSD_ECRCWR_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ECRCWR, PERMIT);
                break;

            case CSL_MMCSD_ETOUTRS_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ETOUTRS, PERMIT);
                break;

            case CSL_MMCSD_ETOUTRD_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ETOUTRD, PERMIT);
                break;

            case CSL_MMCSD_ERSPDNE_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ERSPDNE, PERMIT);
                break;

            case CSL_MMCSD_EBSYDNE_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_EBSYDNE, PERMIT);
                break;

            case CSL_MMCSD_EDATDNE_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_EDATDNE, PERMIT);
                break;

            default:
                status = CSL_ESYS_INVPARAMS;
                break;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b MMC_eventDisable
 *
 *   @b Description
 *   @n This function disables the MMC events depending on the 'mmcsdEvent'
 *      passed as a parameter. This function is useful to disable single
 *      interrupt of MMC interrupt mask register in one function call.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        Handle to the MMCSD
            mmcsdEvent    Event type.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n
 *
 *   @b Modifies
 *   @n Interrupt will be disabled for particular event.
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_MMCSDEventType    mmcsdEvent;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);
            mmcsdEvent = CSL_MMCSD_EDATDNE_INTERRUPT;

            status = MMC_eventDisable(hMmcsd, mmcsdEvent);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_eventDisable(CSL_MmcsdHandle       hMmcsd,
                            CSL_MMCSDEventType    mmcsdEvent)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(NULL == hMmcsd)
    {
        status = CSL_ESYS_BADHANDLE;
    }
    else
    {
        switch (mmcsdEvent)
        {
            case CSL_MMCSD_ETRNDNE_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ETRNDNE, PROHIBIT);
                break;

            case CSL_MMCSD_EDATED_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_EDATED, PROHIBIT);
                break;

            case CSL_MMCSD_EDRRDY_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_EDRRDY, PROHIBIT);
                break;

            case CSL_MMCSD_EDXRDY_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_EDXRDY, PROHIBIT);
                break;

            case CSL_MMCSD_ECRCRS_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ECRCRS, PROHIBIT);
                break;

            case CSL_MMCSD_ECRCRD_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ECRCRD, PROHIBIT);
                break;

            case CSL_MMCSD_ECRCWR_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ECRCWR, PROHIBIT);
                break;

            case CSL_MMCSD_ETOUTRS_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ETOUTRS, PROHIBIT);
                break;

            case CSL_MMCSD_ETOUTRD_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ETOUTRD, PROHIBIT);
                break;

            case CSL_MMCSD_ERSPDNE_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_ERSPDNE, PROHIBIT);
                break;

            case CSL_MMCSD_EBSYDNE_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_EBSYDNE, PROHIBIT);
                break;

            case CSL_MMCSD_EDATDNE_INTERRUPT:
                CSL_FINST(hMmcsd->mmcRegs->MMCIM, MMCSD_MMCIM_EDATDNE, PROHIBIT);
                break;

            default:
                status = CSL_ESYS_INVPARAMS;
                break;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b MMC_drrdy
 *
 *   @b Description
 *   @n This function returns data receive ready status. Value returned by this
 *      API is useful to decide whether data is available in MMC/SD controller
 *      FIFO for reading or not.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd     Handle to the MMCSD
            pStatus    Status of API success or failed.
     @endverbatim
 *
 *   <b> Return Value </b>  Bool
 *   @li                    TRUE  - Data is available for reading
 *   @li                    FALSE - Data is not available for reading
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Bool                  drrdyStatus;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            drrdyStatus = MMC_drrdy(hMmcsd, &status);
     @endverbatim
 *  ===========================================================================
 */
Bool MMC_drrdy(CSL_MmcsdHandle    hMmcsd,
               CSL_Status         *pStatus)
{
    Bool status;

    status = FALSE;

    if(NULL == hMmcsd)
    {
        if(pStatus != NULL)
        {
            *pStatus = CSL_ESYS_BADHANDLE;
        }
    }
    else
    {
        if(pStatus != NULL)
        {
            *pStatus = CSL_SOK;
        }
        status = (Bool)CSL_FEXT(hMmcsd->mmcRegs->MMCST0, MMCSD_MMCST0_DRRDY);
    }

    return (status);
}

/** ===========================================================================
 *   @n@b MMC_dxrdy
 *
 *   @b Description
 *   @n This function returns data transmit ready status. Value returned by this
 *      API is useful to decide whether data can be written to MMCSD controller
 *      FIFO or not.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd     Handle to the MMCSD
            pStatus    Status of API success or failed
     @endverbatim
 *
 *   <b> Return Value </b>  Bool
 *   @li                    TRUE  - MMCSD FIFO is ready to accept data
 *   @li                    FALSE - MMCSD FIFO is not ready to accept data
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Bool                  dxrdyStatus;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            dxrdyStatus = MMC_dxrdy(hMmcsd, &status);
     @endverbatim
 *  ===========================================================================
 */
Bool MMC_dxrdy(CSL_MmcsdHandle    hMmcsd,
               CSL_Status         *pStatus)
{
    Bool status;

    if(NULL == hMmcsd)
    {
        if(pStatus != NULL)
        {
            *pStatus = CSL_ESYS_BADHANDLE;
        }
    }
    else
    {
        if(pStatus != NULL)
        {
            *pStatus = CSL_SOK;
        }
        status = (Bool)CSL_FEXT(hMmcsd->mmcRegs->MMCST0, MMCSD_MMCST0_DXRDY);
    }

    return (status);
}

/** ===========================================================================
 *   @n@b MMC_saveStatus
 *
 *   @b Description
 *   @n This function stores the value of MMCST0 and MMCST1 registers in the
 *      card object strutcure present in the MMCSD handle.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd    Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  Saves the value of MMCST0 and MMCST1
 *
 *   @b Modifies
 *   @n Card object structure
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            Uint16                maskValue;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_saveStatus(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_saveStatus(CSL_MmcsdHandle    hMmcsd)
{
    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        hMmcsd->cardObj->ST0 = hMmcsd->mmcRegs->MMCST0;
        hMmcsd->cardObj->ST1 = hMmcsd->mmcRegs->MMCST1;
        return (CSL_SOK);
    }
}

/** ===========================================================================
 *   @n@b MMC_getStatus
 *
 *   @b Description
 *   @n This function return the status of particular field of MMCST0 and MMCST1
 *      register according to the passed mask value. Value of MMCST1 will be
 *      stored in upper 16 bits and value of MMCST0 will be stored in lower
 *      16 bits of the returned value.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd       Handle to the MMCSD
            maskValue    Mask value to specify the field.
            pStatus      Status of API success or failed.
     @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *   @li                   status - MMCST0 and MMCST1 register value.
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            Uint16                maskValue;
            CSL_status            status;
            Uint32                mmcStatus;
            Uint32                maskValue;

            instId = CSL_MMCSD0_INST;
            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            // Get the value of mmcst0 register
            maskValue = 0x0000FFFF;

            mmcStatus = MMC_getStatus(hMmcsd, maskValue, &status);
     @endverbatim
 *  ===========================================================================
 */
Uint32 MMC_getStatus(CSL_MmcsdHandle    hMmcsd,
                     Uint32             maskValue,
                     CSL_Status         *pStatus)
{
    Uint32 status;

    if(NULL == hMmcsd)
    {
        *pStatus = CSL_ESYS_BADHANDLE;
    }
    else
    {
        *pStatus = CSL_SOK;
        status  = (Uint32)(hMmcsd->mmcRegs->MMCST1);
        status  = status << CSL_MMCSD_SHIFT_MAX;
        status |= (Uint32)(hMmcsd->mmcRegs->MMCST0);
        status &= maskValue;
    }

    return (status);
}

/** ===========================================================================
 *   @n@b MMC_setupNative
 *
 *   @b Description
 *   @n This function will set initial configuration to all the MMCSD registers.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd      Handle to the MMCSD.
            pMmcInit    Pointer to CSL_MMCSetupNative state variable.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  MMCSD Registers will be populated.
 *
 *   @b Modifies
 *   @n MMCSD Registers
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            CSL_MMCConfig         mmcInit;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            Note: CSL_MMCConfig field are following.
            mmcInit.dmaEnable         = CSL_MMCSD_MMCCMD2_DMATRIG_NO;
            mmcInit.dat3EdgeDetection = CSL_MMCSD_MMCCTL_DATEG_DISABLE;
            mmcInit.cdiv              = CSL_MMCSD_CLK_DIV_INIT;
            mmcInit.rspTimeout        = CSL_MMCSD_RESPONSE_TIMEOUT;
            mmcInit.dataTimeout       = CSL_MMCSD_DATA_RW_TIMEOUT;
            mmcInit.blockLen          = CSL_MMCSD_MIN_BLOCK_LENGTH;

            status = MMC_setupNative(hMmcsd, &mmcInit);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setupNative(CSL_MmcsdHandle       hMmcsd,
                           CSL_MMCSetupNative    *pMmcInit)
{
    volatile Uint16 delay;

    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        if(NULL == pMmcInit)
        {
            return (CSL_ESYS_INVPARAMS);
        }
        else
        {
            /*
             * Disbale the clock.
             * MMCSD controller has auto-clocking feature which automatically
             * enables the clock when there is a communication with the card
             */
            CSL_FINST(hMmcsd->mmcRegs->MMCCLK, MMCSD_MMCCLK_CLKEN, DISABLE);
            CSL_FINS(hMmcsd->mmcRegs->MMCCLK, MMCSD_MMCCLK_CLKRT,
                                                       pMmcInit->cdiv);
            CSL_FINS(hMmcsd->mmcRegs->MMCCLK, MMCSD_MMCCTL_DATEG,
                                              pMmcInit->dat3EdgeDetection);

            hMmcsd->mmcRegs->MMCTOR  = pMmcInit->rspTimeout;
            hMmcsd->mmcRegs->MMCTOD  = pMmcInit->dataTimeout;
            hMmcsd->mmcRegs->MMCBLEN = pMmcInit->blockLen;

            return (CSL_SOK);
        }
    }
}

/** ===========================================================================
 *   @n@b MMC_config
 *
 *   @b Description
 *   @n Function to configure the MMCSD registers. Register configuration will
 *      be done as per the configuration parameters passed to the function.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        Handle to the MMCSD.
            pMmcConfig    Pointer to the MMCSD config parameter.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n MMCSD Register will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            CSL_MMCConfig         mmcConfig;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            Note: CSL_MMCConfig field are following.
            mmcConfig.mmcblen = CSL_MMCSD_BLOCK_LENGTH;
            mmcConfig.mmcclk  = 0x10 |CSL_MMCSD_MMCCLK_CLKEN_ENABLE;
            mmcConfig.mmcctl  = CSL_MMCSD_MMCCTL_DATEG_F_EDGE;
            mmcConfig.mmcim   = 0x0;
            mmcConfig.mmcnblk = 0x1;
            mmcConfig.mmctod  = CSL_MMCSD_DATA_RW_TIMEOUT;
            mmcConfig.mmctor  = CSL_MMCSD_RESPONSE_TIMEOUT;

            status = MMC_config(hMmcsd, &mmcConfig);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_config(CSL_MmcsdHandle    hMmcsd,
                      CSL_MMCConfig      *pMmcConfig)
{
    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        if(NULL == pMmcConfig)
        {
            return (CSL_ESYS_INVPARAMS);
        }
        else
        {
            hMmcsd->mmcRegs->MMCCTL  = pMmcConfig->mmcctl;
            hMmcsd->mmcRegs->MMCCLK  = pMmcConfig->mmcclk;
            hMmcsd->mmcRegs->MMCIM   = pMmcConfig->mmcim;
            hMmcsd->mmcRegs->MMCTOR  = pMmcConfig->mmctor;
            hMmcsd->mmcRegs->MMCTOD  = pMmcConfig->mmctod;
            hMmcsd->mmcRegs->MMCBLEN = pMmcConfig->mmcblen;
            hMmcsd->mmcRegs->MMCNBLK = pMmcConfig->mmcnblk;
            return (CSL_SOK);
        }
    }
}

/** ===========================================================================
 *   @n@b MMC_getConfig
 *
 *   @b Description
 *   @n This function will fetch the information present in the MMCSD
 *      registers and update to passed the config parameter.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        Handle to the MMCSD.
            pMmcConfig    Pointer to the MMCSD config Parameter.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            CSL_MMCConfig         pMmcConfig)

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_getConfig(hMmcsd, &pMmcConfig);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_getConfig(CSL_MmcsdHandle    hMmcsd,
                         CSL_MMCConfig      *pMmcConfig)
{
    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        if(NULL == pMmcConfig)
        {
            return (CSL_ESYS_INVPARAMS);
        }
        else
        {
            pMmcConfig->mmcctl  = hMmcsd->mmcRegs->MMCCTL;
            pMmcConfig->mmcclk  = hMmcsd->mmcRegs->MMCCLK;
            pMmcConfig->mmcim   = hMmcsd->mmcRegs->MMCIM;
            pMmcConfig->mmctor  = hMmcsd->mmcRegs->MMCTOR;
            pMmcConfig->mmctod  = hMmcsd->mmcRegs->MMCTOD;
            pMmcConfig->mmcblen = hMmcsd->mmcRegs->MMCBLEN;
            pMmcConfig->mmcnblk = hMmcsd->mmcRegs->MMCNBLK;
            return (CSL_SOK);
        }
    }
}

/** ===========================================================================
 *   @n@b MMC_sendCmd
 *
 *   @b Description
 *   @n This is used to send a command to the memory card. Lower 16 bits of the
 *      parameter 'cmd' should contain the command that needs to be written to
 *      MMCCMD1 register. Upper 16 bits of the 'cmd' should contain the data
 *      that needs to be written to MMCCMD2 register.
 *
 *      NOTE:SETTING THE MMCCMD2 VALUE TO '1' IS ALLOWED ONLY TO DATA WRITE
 *      COMMANDS THAT USES DMA FOR DATA TRANSFER. FOR OTHER COMMANDS UPPER
 *      16 BITS OF 'CMD' SHOULD ALWAYS BE ZERO.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        Handle to the MMCSD.
            cmd           Command index.
            arg           Argument.
            waitForRsp    Wait cycle for response.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Close successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Uint32                cmd;
            Uint32                arg;
            Uint16                waitForRsp;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            cmd        = CSL_MMCSD_WRITE_INIT_CMD;
            arg        = CSL_MMCSD_ARG_RESET;
            waitForRsp = CSL_MMCSD_RESPONSE_TIMEOUT;

            status = MMC_sendCmd(hMmcsd, cmd, arg, waitForRsp);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_sendCmd(CSL_MmcsdHandle    hMmcsd,
                       Uint32             cmd,
                       Uint32             arg,
                       Uint16             waitForRsp)
{
    volatile Uint16    status;

    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                 (Uint16)(arg & CSL_MMCSD_MASK_MAX));
        CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                 (Uint16)((arg >> CSL_MMCSD_SHIFT_MAX) & CSL_MMCSD_MASK_MAX));

        hMmcsd->mmcRegs->MMCCMD1 = (Uint16)(cmd & CSL_MMCSD_MASK_MAX);

        do
        {
            status = hMmcsd->mmcRegs->MMCST0;
            if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
            {
                if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }
                else
                {
                    return (CSL_EMMCSD_CRCERR);
                }
            }
        } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

        /*
         * Configuring MMCCMD2 is applicable only to data write commands
         * that uses dma for data transfer.
         * For other commands upper 16 bits of 'cmd' should always be zero
         */
        hMmcsd->mmcRegs->MMCCMD2 = (Uint16)((cmd >> CSL_MMCSD_SHIFT_MAX)
                                                  & CSL_MMCSD_MASK_MAX);

        return (CSL_SOK);
    }
}

/** ===========================================================================
 *   @n@b MMC_sendGoIdle
 *
 *   @b Description
 *   @n This function broadcasts Go Idle command for Setting all cards
 *      to the idle state.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd    Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n This should be called just after MMC_open() API.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_sendGoIdle(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_sendGoIdle(CSL_MmcsdHandle    hMmcsd)
{
    volatile Uint16    goIdleCmd;

    goIdleCmd = 0;

    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
                 MMCSD_MMCARG1_ARG1, CSL_MMCSD_STUFF_BITS);
        CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
                 MMCSD_MMCARG2_ARG2, CSL_MMCSD_STUFF_BITS);

        /* Make the command value to be sent to the card */
        CSL_FINST(goIdleCmd, MMCSD_MMCCMD1_DCLR, CLEAR);
        CSL_FINST(goIdleCmd, MMCSD_MMCCMD1_INITCK, INIT);
        CSL_FINST(goIdleCmd, MMCSD_MMCCMD1_WDATX, NO);
        CSL_FINST(goIdleCmd, MMCSD_MMCCMD1_STRMTP, NO);
        CSL_FINST(goIdleCmd, MMCSD_MMCCMD1_DTRW, NO);
        CSL_FINST(goIdleCmd, MMCSD_MMCCMD1_RSPFMT, NORSP);
        CSL_FINST(goIdleCmd, MMCSD_MMCCMD1_BSYEXP, NO);
        CSL_FINST(goIdleCmd, MMCSD_MMCCMD1_PPLEN, OD);
        CSL_FINS(goIdleCmd,  MMCSD_MMCCMD1_CMD, CSL_MMCSD_GO_IDLE_STATE);

        hMmcsd->mmcRegs->MMCCMD1 = goIdleCmd;

        return (CSL_SOK);
    }
}

/** ===========================================================================
 *   @n@b MMC_stop
 *
 *   @b Description
 *   @n Function to stop the MMC/SD transfer and receive events. This function
 *      sends CMD12 to the memory card.
 *
 *   NOTE: STOP command should be sent to the card when it is in 'data'(read)
 *   or 'rcv'(write) state. Sending STOP command while the card in any other
 *   state will result in command timeout.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *   @n  Card should be addressed.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_stop(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_stop(CSL_MmcsdHandle    hMmcsd)
{
    Uint16 stopCmd;
    volatile Uint16 status;
    volatile Uint16 mmcStatus;

    stopCmd = 0;

    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
                         MMCSD_MMCARG1_ARG1, CSL_MMCSD_STUFF_BITS);
        CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
                         MMCSD_MMCARG2_ARG2, CSL_MMCSD_STUFF_BITS);

        /* Make the command value to be sent to the card */
        CSL_FINST(stopCmd, MMCSD_MMCCMD1_DCLR, NO);
        CSL_FINST(stopCmd, MMCSD_MMCCMD1_INITCK, NO);
        CSL_FINST(stopCmd, MMCSD_MMCCMD1_WDATX, NO);
        CSL_FINST(stopCmd, MMCSD_MMCCMD1_STRMTP, NO);
        CSL_FINST(stopCmd, MMCSD_MMCCMD1_DTRW, NO);
        /* CMD12 has R1b response, but it might not produce response */
        CSL_FINST(stopCmd, MMCSD_MMCCMD1_RSPFMT, R1);
        CSL_FINST(stopCmd, MMCSD_MMCCMD1_BSYEXP, BUSY);
        CSL_FINST(stopCmd, MMCSD_MMCCMD1_PPLEN, OD);
        CSL_FINS(stopCmd, MMCSD_MMCCMD1_CMD, CSL_MMCSD_STOP_TRANSMISSION);

        hMmcsd->mmcRegs->MMCCMD1 = stopCmd;

        mmcStatus = 0;
        do
        {
            status = hMmcsd->mmcRegs->MMCST0;

            mmcStatus |= status & CSL_MMCSD_EVENT_EOFCMD;
            mmcStatus |= status & CSL_MMCSD_EVENT_CARD_EXITBUSY;

            if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
            {
                if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }
                else
                {
                    return (CSL_EMMCSD_CRCERR);
                }
            }
        } while(((mmcStatus & CSL_MMCSD_EVENT_EOFCMD) == 0) ||
                ((mmcStatus & CSL_MMCSD_EVENT_CARD_EXITBUSY) == 0));

        return (CSL_SOK);
    }
}

/** ===========================================================================
 *   @n@b MMC_deselectCard
 *
 *   @b Description
 *   @n This function will send command for deselecting particular card which
 *      is assigned in card object.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pMmcCardObj       Pointer to the MMCSD Card Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *   @n  RCA value should be properly stored in handle hMmcsd.
 *
 *   <b> Post Condition </b>
 *   @n  Deselects the card
 *
 *   @b Modifies
 *   @n MMCSD card object structure.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          mmcCardObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_deselectCard(hMmcsd, &mmcCardObj);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_deselectCard(CSL_MmcsdHandle    hMmcsd,
                            CSL_MMCCardObj     *pMmcCardObj)
{
    Uint16 deselectCmd;
    volatile Uint16 status;
    Uint16   rca;

    deselectCmd = 0;
    rca         = 0;

    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        if(NULL == pMmcCardObj)
        {
            return (CSL_ESYS_INVPARAMS);
        }
        else
        {
            CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
                     MMCSD_MMCARG1_ARG1, CSL_MMCSD_STUFF_BITS);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
                     MMCSD_MMCARG2_ARG2, rca);

            /* Make the command value to be sent to the card */
            CSL_FINST(deselectCmd, MMCSD_MMCCMD1_DCLR, NO);
            CSL_FINST(deselectCmd, MMCSD_MMCCMD1_INITCK, NO);
            CSL_FINST(deselectCmd, MMCSD_MMCCMD1_WDATX, NO);
            CSL_FINST(deselectCmd, MMCSD_MMCCMD1_STRMTP, NO);
            CSL_FINST(deselectCmd, MMCSD_MMCCMD1_DTRW, NO);
            /* CMD7 has R1b response, But deselected card may not produce response */
            CSL_FINST(deselectCmd, MMCSD_MMCCMD1_RSPFMT, NORSP);
            CSL_FINST(deselectCmd, MMCSD_MMCCMD1_BSYEXP, NO);
            CSL_FINST(deselectCmd, MMCSD_MMCCMD1_PPLEN, OD);
            CSL_FINS(deselectCmd, MMCSD_MMCCMD1_CMD,
                     CSL_MMCSD_SELECT_DESELECT_CARD);
            hMmcsd->mmcRegs->MMCCMD1 = deselectCmd;

            do
            {
                status = hMmcsd->mmcRegs->MMCST0;
                if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_EMMCSD_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_EMMCSD_CRCERR);
                    }
                }
            } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

            pMmcCardObj->cardType  = CSL_CARD_NONE;
            hMmcsd->numCardsActive = 0;

            return (CSL_SOK);
        }
    }
}

/** ===========================================================================
 *   @n@b MMC_selectCard
 *
 *   @b Description
 *   @n This function detects which card (MMC/SD/SDHC) is inserted. Information
 *      of the card detected will be populated to the card object structure
 *      passed as parameter.
 *
 *      MMC object structure passed to this function should global structure
 *      variable which persists till end of the program since it is stored by
 *      MMCSD CSL for reference in other APIs.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pMmcCardObj       Pointer to the MMCSD Card Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MMCCardObj        mmcCardObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &mmcCardObj);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_selectCard(CSL_MmcsdHandle    hMmcsd,
                          CSL_MMCCardObj     *pMmcCardObj)
{
    Uint32             argument;
    Uint32             resp;
    volatile Uint16    delay;
    volatile Uint16    status;
    Uint16             cardCheck;
    Uint16             cmdRetryCnt;

    cmdRetryCnt = CSL_MMSCD_ACMD41_RETRY_COUNT;

    if((hMmcsd !=  NULL) && (pMmcCardObj != NULL))
    {
         /* Passed Parameter are correct */
    }
    else
    {
        if(NULL == hMmcsd)
        {
            return (CSL_ESYS_BADHANDLE);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    }

    hMmcsd->cardObj       = pMmcCardObj;
    pMmcCardObj->cardType = CSL_CARD_NONE;

    /* Place the MMCSD controller in RESET state */
    CSL_FINST(hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_CMDRST, DISABLE);
    CSL_FINST(hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_DATRST, DISABLE);

    /* Configure time-out registers */
    hMmcsd->mmcRegs->MMCTOR = CSL_MMCSD_RESPONSE_TIMEOUT;
    hMmcsd->mmcRegs->MMCTOD = CSL_MMCSD_DATA_RW_TIMEOUT;

    /* Configure the clock */
    /*
     * Disbale the clock.
     * MMCSD controller has auto-clocking feature which automatically
     * enables the clock when there is a communication with the card
     */
    CSL_FINST(hMmcsd->mmcRegs->MMCCLK, MMCSD_MMCCLK_CLKEN, DISABLE);
    CSL_FINS(hMmcsd->mmcRegs->MMCCLK, MMCSD_MMCCLK_CLKRT,
             CSL_MMCSD_CLK_DIV_INIT);

    /* Take the MMCSD controller out of RESET state */
    CSL_FINST(hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_CMDRST, ENABLE);
    CSL_FINST(hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_DATRST, ENABLE);

    /* Send CMD 0 */
    CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
             MMCSD_MMCARG1_ARG1, CSL_MMCSD_STUFF_BITS);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
             MMCSD_MMCARG2_ARG2, CSL_MMCSD_STUFF_BITS);
    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CARD_INIT0_CMD;
    do
    {
        status = hMmcsd->mmcRegs->MMCST0;
    } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

    /*
     * This loop runs for three times to detect the three differnt
     * cards supported; MMC/SD/SDHC
     */
    for(cardCheck = 0; cardCheck < 3; cardCheck++)
    {
        /*
         * Send SEND_IF_COND command - Support for high capacity cards
         * SEND_IF_COND (CMD8) is used to verify SD Memory Card(ver2.00)
         * interface operating condition.
         */
        if(0 == cardCheck)
        {
            /* CMD8 for SD HC card */
            argument = CSL_MMC_CMD8_ARG;
            hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
            CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     (Uint16)(argument & CSL_MMCSD_MASK_MAX));
            CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     (Uint16)((argument >>CSL_MMCSD_SHIFT_MAX)
                      & CSL_MMCSD_MASK_MAX));
            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_SDHC_CARD_INIT_CMD;
            do
            {
                status = hMmcsd->mmcRegs->MMCST0;
                if((status & CSL_MMCSD_EVENT_ERROR_CMDCRC) != 0)
                {
                    return (CSL_EMMCSD_CRCERR);
                }
            } while((status & (CSL_MMCSD_EVENT_EOFCMD |
                     CSL_MMCSD_EVENT_ERROR)) == 0);

            if((status & CSL_MMCSD_EVENT_ERROR) == 0)
            {
                if((CSL_SD_HC_ECHO_PATTERN | CSL_SD_HC_VHS_ECHO) ==
                   (hMmcsd->mmcRegs->MMCRSP6 & 0x1FF))
                {
                    argument = CSL_MMC_ACMD41_ARG_HCS;
                }
                else
                {
                    /* Card with incompatible voltage range */
                    pMmcCardObj->cardType  = CSL_CARD_NONE;
                    hMmcsd->numCardsActive = 0;
                    break;
                }
            }
            else
            {
                argument = CSL_MMC_ACMD41_ARG_NOHCS;

                /*
                 * Some of the cards which does not respond for CMD 8
                 * need to be reset for proper operation.
                 * Send CMD 0
                 */
                CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
                CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1, 0);
                CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2, 0);
                hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CARD_INIT0_CMD;
                do
                {
                    status = hMmcsd->mmcRegs->MMCST0;
                } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);
            }

        } /* End of if(cardCheck == 0) */
        else if(1 == cardCheck) /* Initialize the SD card */
        {
            /* Send command ACMD41 (CMD55 + CMD41)  */
            do
            {
                hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
                CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
                CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1, 0);
                CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2, 0);
                hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_SD_CARD_INIT_CMD;
                do
                {
                    status = hMmcsd->mmcRegs->MMCST0;
                    if((status & CSL_MMCSD_EVENT_ERROR_CMDCRC) != 0)
                    {
                        return (CSL_EMMCSD_CRCERR);
                    }
                } while(((status & (CSL_MMCSD_EVENT_EOFCMD |
                          CSL_MMCSD_EVENT_ERROR)) == 0));

                /* Check for errors - Card can be MMC */
                if((status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT) != 0)
                {
                    resp = 0;
                    break;
                }

                /* CMD 41 */
                hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
                CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
                CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                         (Uint16)(argument & CSL_MMCSD_MASK_MAX));
                CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                         (Uint16)((argument >> CSL_MMCSD_SHIFT_MAX) &
                                   CSL_MMCSD_MASK_MAX));
                hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_41;

                do
                {
                    status = hMmcsd->mmcRegs->MMCST0;
                } while(((status & (CSL_MMCSD_EVENT_EOFCMD |
                          CSL_MMCSD_EVENT_ERROR)) == 0));

                /* Check for errors - Card can be MMC */
                if((status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT) != 0)
                {
                    resp = 0;
                    break;
                }

                resp = hMmcsd->mmcRegs->MMCRSP7;
                resp = (resp << CSL_MMCSD_SHIFT_MAX) | (hMmcsd->mmcRegs->MMCRSP6);

                /* Check if card is MMC Card */
                if(resp == 0)
                {
                    break;
                }

                cmdRetryCnt--;
                if(cmdRetryCnt == 0)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }

            } while((resp & CSL_MMCSD_CMD41_RESP) != CSL_MMCSD_CMD41_RESP);

            if(resp != 0)
            {
                /* Check card is high capacity or standard capacity card */
                if((resp & CSL_MMCSD_SDHC_RESP) != 0)
                {
                    pMmcCardObj->sdHcDetected = TRUE;
                }
                else
                {
                    pMmcCardObj->sdHcDetected = FALSE;
                }

                pMmcCardObj->cardType  = CSL_SD_CARD;
                hMmcsd->numCardsActive = 1;
                break;
            }

        } /* End of if(cardCheck == 1) */
        else /* Initialize the MMC card */
        {
            /* CMD 1 */
            cmdRetryCnt = CSL_MMSCD_CMD1_RETRY_COUNT;
            argument = CSL_MMC_CMD1_ARG;

            do
            {
                /* send command   */
                hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
                CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
                CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                         (Uint16)(argument & CSL_MMCSD_MASK_MAX));
                CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                         (Uint16)((argument >>CSL_MMCSD_SHIFT_MAX)
                          & CSL_MMCSD_MASK_MAX));
                hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_MMCS_CARD_INIT_CMD;
                do
                {
                    status = hMmcsd->mmcRegs->MMCST0;
                    if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_EMMCSD_TIMEOUT);
                    }
                } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

                resp = hMmcsd->mmcRegs->MMCRSP7;
                resp = (resp << CSL_MMCSD_SHIFT_MAX) | (hMmcsd->mmcRegs->MMCRSP6);
                cmdRetryCnt--;
            } while((resp & CSL_MMCSD_CMD1_RESP) != CSL_MMCSD_CMD1_RESP);

            if(cmdRetryCnt != 0)
            {
                /* Check card is high capacity or standard capacity card */
                if((resp & CSL_MMCSD_SDHC_RESP) != 0)
                {
                    pMmcCardObj->sdHcDetected = TRUE;
                }
                else
                {
                    pMmcCardObj->sdHcDetected = FALSE;
                }

                pMmcCardObj->cardType  = CSL_MMC_CARD;
                hMmcsd->numCardsActive = 1;
            }
            else
            {
                pMmcCardObj->cardType  = CSL_CARD_NONE;
                hMmcsd->numCardsActive = 0;
            }
        }
    }

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b MMC_sendOpCond
 *
 *   @b Description
 *   @n This function will set the clock to the SD or MMC Card according to
 *      passed parameter of Card Type.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD
            clkDivValue       Clock Div value for MMC or SD card.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  1:MMC_open() API should be called successfully.
 *   @n  2:MMC_selectCard() API should be called before this so that it will do
 *       some needful work for particular card detected.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          mmcCardObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &mmcCardObj);

            pMmmcCardObj->cardType = CSL_MMC_CARD;

            clkDivValue = 0x4 //as MMC clock max is 20MHz and Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_sendOpCond(CSL_MmcsdHandle    hMmcsd,
                             Uint16             clkDivValue)
{
    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        if(clkDivValue <= CSL_MMC_MAX_CLOCK_RATE)
        {
            /* Set clock divisor specific to MMC Card */
            CSL_FINST(hMmcsd->mmcRegs->MMCCLK, MMCSD_MMCCLK_CLKEN, DISABLE);
            CSL_FINS(hMmcsd->mmcRegs->MMCCLK, MMCSD_MMCCLK_CLKRT, clkDivValue);

            if(CSL_MMC_CARD == hMmcsd->cardObj->cardType)
            {
                /* Set data line for MMC Card */
                CSL_FINST(hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_WIDTH, BIT1);
            }

            if(CSL_SD_CARD == hMmcsd->cardObj->cardType)
            {
                /* Set data line for MMC Card */
                CSL_FINST(hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_WIDTH, BIT4);
            }

            return (CSL_SOK);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    }
}

/** ===========================================================================
 *   @n@b MMC_setRca
 *
 *   @b Description
 *   @n This function sets the relative card address for MMC Card. Relative
 *      card address is stored in CSL_MMCCardObj which can used by other
 *      MMCSD CSL APIs.
 *
 *      Application programs should use only this function to set the RCA of
 *      MMC card. RCA passed to this function will be stored in the MMCSD
 *      handle and is used by other MMCSD CSL APIs.
 *
 *      NOTE: THIS FUNCTION IS SPECIFIC TO MMC CARD AND SHOULD NOT BE USED WITH
 *      THE SD CARD. FOR MMC CARD CMD3(SET_RELATIVE_ADDR) IS USED TO SET THE
 *      CARD ADDRESS AND FOR SD CARD CMD3(SEND_RELATIVE_ADDR) IS USED TO REQUEST
 *      THE CARD ADDRESS.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pMmcCardObj       Pointer to the MMC Card Info.
            rCardAddr         Relative card address.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:MMC_open() API should be called successfully.
 *   @n  2:MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3: If Card detected is MMC then MMC_sendOpCond() for specific to MMC.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          mmcCardObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;
            Uint16                  rCardAddr;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &mmcCardObj);

            clkDivValue = 0x4 //as MMC clock max is 20MHz and Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);
            rCardAddr = 0x100;

            status = MMC_setRca(hMmcsd, &mmcCardObj, rCardAddr);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setRca(CSL_MmcsdHandle     hMmcsd,
                      CSL_MMCCardObj      *pMmcCardObj,
                      Uint16              rCardAddr)
{
    Uint16 setRcaCmd;
    volatile Uint16 status;

    setRcaCmd = 0;

    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        if(NULL == pMmcCardObj)
        {
            return (CSL_ESYS_INVPARAMS);
        }
        else
        {
            CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
                     MMCSD_MMCARG1_ARG1, CSL_MMCSD_STUFF_BITS);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
                     MMCSD_MMCARG2_ARG2, rCardAddr);

            /* Make the command value to be sent to the card */
            CSL_FINST(setRcaCmd, MMCSD_MMCCMD1_DCLR, NO);
            CSL_FINST(setRcaCmd, MMCSD_MMCCMD1_INITCK, NO);
            CSL_FINST(setRcaCmd, MMCSD_MMCCMD1_WDATX, NO);
            CSL_FINST(setRcaCmd, MMCSD_MMCCMD1_STRMTP, NO);
            CSL_FINST(setRcaCmd, MMCSD_MMCCMD1_DTRW, NO);
            /* For MMC, CMD3 response is R1 */
            CSL_FINST(setRcaCmd, MMCSD_MMCCMD1_RSPFMT, R1);
            CSL_FINST(setRcaCmd, MMCSD_MMCCMD1_BSYEXP, NO);
            CSL_FINST(setRcaCmd, MMCSD_MMCCMD1_PPLEN, OD);
            CSL_FINS(setRcaCmd, MMCSD_MMCCMD1_CMD, CSL_MMCSD_RELATIVE_ADDR);
            hMmcsd->mmcRegs->MMCCMD1 = setRcaCmd;

            do
            {
                status = hMmcsd->mmcRegs->MMCST0;
                if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_EMMCSD_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_EMMCSD_CRCERR);
                    }
                }
            } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

            pMmcCardObj->rca = rCardAddr;
            hMmcsd->cardObj  = pMmcCardObj;

            return (CSL_SOK);
        }
    }
}

/** ===========================================================================
 *   @n@b SD_sendRca
 *
 *   @b Description
 *   @n This function sends request for relative card address(RCA) of SD Card.
 *      RCA value published by SD card will be populated to card address
 *      parameter. RCA is also stored in CSL_MMCCardObj to use in other MMCSD
 *      CSL APIs.
 *
 *      Application programs should use only this function to requesting the
 *      RCA from SD card. RCA published by the SD card will be stored in the
 *      MMCSD handle and is used by other MMCSD CSL APIs.
 *
 *      NOTE: THIS FUNCTION IS SPECIFIC TO SD CARD AND SHOULD NOT BE USED WITH
 *      THE MMC CARD. FOR MMC CARD CMD3(SET_RELATIVE_ADDR) IS USED TO SET THE
 *      CARD ADDRESS AND FOR SD CARD CMD3(SEND_RELATIVE_ADDR) IS USED TO REQUEST
 *      THE CARD ADDRESS.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pSdCardObj        Pointer to the SD Card Info.
            pRCardAddr        Pointer to store relative card address.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:MMC_open() API should be called successfully.
 *   @n  2:MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3:If Card detected is SD then MMC_sendOpCond() for specific to SD.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n pRCardAddr variable will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          sdCardObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;
            Uint16                  rCardAddr;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &sdCardObj);

            clkDivValue = 0x1; //As SD clock max is 50MHz and Function clock is 100MHz

            status = MMC_sendOpCond(hMmcsd, clkDivValue);
            status = SD_sendRca(hMmcsd, &sdCardObj, &rCardAddr);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SD_sendRca(CSL_MmcsdHandle     hMmcsd,
                      CSL_MMCCardObj      *pSdCardObj,
                      Uint16              *pRCardAddr)
{
    volatile Uint16 sendRcaCmd;
    volatile Uint16 status;

    sendRcaCmd = 0;

    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        if((NULL == pSdCardObj) || (NULL == pRCardAddr))
        {
            return (CSL_ESYS_INVPARAMS);
        }
        else
        {
            CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
                     MMCSD_MMCARG1_ARG1, CSL_MMCSD_STUFF_BITS);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
                     MMCSD_MMCARG2_ARG2, CSL_MMCSD_STUFF_BITS);

            /* Make the command value to be sent to the card */
            CSL_FINST(sendRcaCmd, MMCSD_MMCCMD1_DCLR, NO);
            CSL_FINST(sendRcaCmd, MMCSD_MMCCMD1_INITCK, NO);
            CSL_FINST(sendRcaCmd, MMCSD_MMCCMD1_WDATX, NO);
            CSL_FINST(sendRcaCmd, MMCSD_MMCCMD1_STRMTP, NO);
            CSL_FINST(sendRcaCmd, MMCSD_MMCCMD1_DTRW, NO);
            /* For SD, CMD3 response is R6 which is R1 for the controller */
            CSL_FINST(sendRcaCmd, MMCSD_MMCCMD1_RSPFMT, R1);
            CSL_FINST(sendRcaCmd, MMCSD_MMCCMD1_BSYEXP, NO);
            CSL_FINST(sendRcaCmd, MMCSD_MMCCMD1_PPLEN, OD);
            CSL_FINS(sendRcaCmd, MMCSD_MMCCMD1_CMD, CSL_MMCSD_RELATIVE_ADDR);
            hMmcsd->mmcRegs->MMCCMD1 = sendRcaCmd;

            do
            {
                status = hMmcsd->mmcRegs->MMCST0;
                if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_EMMCSD_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_EMMCSD_CRCERR);
                    }
                }
            } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

            *pRCardAddr = hMmcsd->mmcRegs->MMCRSP7;
            pSdCardObj->rca = *pRCardAddr;
            hMmcsd->cardObj = pSdCardObj;

            return (CSL_SOK);
        }
    }
}

/** ===========================================================================
 *   @n@b MMC_sendAllCID
 *
 *   @b Description
 *   @n This will fetch the Information about MMC Card Id and update into the
 *      passed id structure.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd           Handle to the MMCSD.
            pMmcCardIdObj    Pointer to the MMC Card ID Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1.MMC_open() API should be called successfully.
 *   @n  2.MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3.If Card detected is MMC then MMC_sendOpCond() for specific to MMC.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n pMmcCardIdObj will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          mmcCardObj;
            CSL_MMCCardIdObj        mmcCardIdObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &mmcCardObj);

            clkDivValue = 0x4 //As MMC clock max is 20MHz and
                                Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = MMC_sendAllCID(hMmcsd, &mmcCardIdObj);
            For example following value will be populated.
            pMmcCardIdObj->mfgId         = ;
            pMmcCardIdObj->serialNumber  = ;
            pMmcCardIdObj->monthCode     = ;
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_sendAllCID(CSL_MmcsdHandle     hMmcsd,
                          CSL_MMCCardIdObj    *pMmcCardIdObj)
{
    volatile Uint16       status;
    CSL_MMCSDCidStruct    mmcCid;

    if((hMmcsd !=  NULL) && (pMmcCardIdObj != NULL))
    {
         /* Passed Parameter are correct */
    }
    else
    {
        if(NULL == hMmcsd)
        {
            return (CSL_ESYS_BADHANDLE);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    }

    /* CMD 2 */
    hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
    CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
             CSL_MMCSD_STUFF_BITS);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
             CSL_MMCSD_STUFF_BITS);

    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_2;

    do
    {
        status = hMmcsd->mmcRegs->MMCST0;
        if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
        {
            if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
            {
                return (CSL_EMMCSD_TIMEOUT);
            }
            else
            {
                return (CSL_EMMCSD_CRCERR);
            }
        }
    } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

    /* Update the CID structure */
    mmcCid.slice15_0    = hMmcsd->mmcRegs->MMCRSP0;
    mmcCid.slice31_16   = hMmcsd->mmcRegs->MMCRSP1;
    mmcCid.slice47_32   = hMmcsd->mmcRegs->MMCRSP2;
    mmcCid.slice63_48   = hMmcsd->mmcRegs->MMCRSP3;
    mmcCid.slice79_64   = hMmcsd->mmcRegs->MMCRSP4;
    mmcCid.slice95_80   = hMmcsd->mmcRegs->MMCRSP5;
    mmcCid.slice111_96  = hMmcsd->mmcRegs->MMCRSP6;
    mmcCid.slice127_112 = hMmcsd->mmcRegs->MMCRSP7;

    hMmcsd->cidSliceInfo = mmcCid;

    pMmcCardIdObj->mfgId    = (mmcCid.slice127_112 >> 8) & 0xFF;

    pMmcCardIdObj->oemAppId = ((mmcCid.slice127_112 & 0xFF) << 8) |
                               (mmcCid.slice111_96 >> 8) & 0xFF;

    pMmcCardIdObj->productName[5] = (mmcCid.slice63_48 >> 8) & 0xFF;
    pMmcCardIdObj->productName[4] = (mmcCid.slice79_64 & 0xFF);
    pMmcCardIdObj->productName[3] = (mmcCid.slice79_64 >> 8) & 0xFF;
    pMmcCardIdObj->productName[2] = (mmcCid.slice95_80 & 0xFF);
    pMmcCardIdObj->productName[1] = (mmcCid.slice95_80 >> 8) & 0xFF;
    pMmcCardIdObj->productName[0] = (mmcCid.slice111_96 & 0xFF);

    pMmcCardIdObj->serialNumber = ((Uint32)mmcCid.slice47_32 << 16) |
                                    ((Uint32)mmcCid.slice31_16);
    pMmcCardIdObj->month        = (mmcCid.slice15_0 >> 12) & 0xF;
    pMmcCardIdObj->year         = (((mmcCid.slice15_0 >> 8) & 0xF) + 1997);
    pMmcCardIdObj->checksum     = (mmcCid.slice15_0  & 0xFE) >> 1;

    hMmcsd->cardObj->cid = pMmcCardIdObj;

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b SD_sendAllCID
 *
 *   @b Description
 *   @n This will fetch the Information about SD Card Id and update into the
 *      passed id structure.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd           Handle to the MMCSD.
            pSdCardIdObj     Pointer to the SD Card ID Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:MMC_open() API should be called successfully.
 *   @n  2:MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3:If Card detected is SD then MMC_sendOpCond() specific to SD.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n pSdCardIdObj object will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          sdCardObj;
            CSL_MMCCardIdObj        sdCardIdObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &sdCardObj);

            clkDivValue = 0x1 //As SD clock max is 50MHz and
                                Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = SD_sendAllCID(hMmcsd, &sdCardIdObj);

            For example following value will be populated.
            sdCardIdObj.mfgId         = ;
            sdCardIdObj.serialNumber  = ;
            sdCardIdObj.monthCode     = ;
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SD_sendAllCID(CSL_MmcsdHandle     hMmcsd,
                         CSL_MMCCardIdObj    *pSdCardIdObj)
{
    volatile Uint16       status;
    CSL_MMCSDCidStruct    sdCid;

    if((hMmcsd !=  NULL) && (pSdCardIdObj != NULL))
    {
         /* Passed Parameter are correct */
    }
    else
    {
        if(NULL == hMmcsd)
        {
            return (CSL_ESYS_BADHANDLE);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    }

    /* CMD 2 */
    hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
    CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
             CSL_MMCSD_STUFF_BITS);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
             CSL_MMCSD_STUFF_BITS);
    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_2;

    do
    {
        status = hMmcsd->mmcRegs->MMCST0;
        if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
        {
            if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
            {
                return (CSL_EMMCSD_TIMEOUT);
            }
            else
            {
                return (CSL_EMMCSD_CRCERR);
            }
        }
    } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

    /* Update the CID structure */
    sdCid.slice15_0    = hMmcsd->mmcRegs->MMCRSP0;
    sdCid.slice31_16   = hMmcsd->mmcRegs->MMCRSP1;
    sdCid.slice47_32   = hMmcsd->mmcRegs->MMCRSP2;
    sdCid.slice63_48   = hMmcsd->mmcRegs->MMCRSP3;
    sdCid.slice79_64   = hMmcsd->mmcRegs->MMCRSP4;
    sdCid.slice95_80   = hMmcsd->mmcRegs->MMCRSP5;
    sdCid.slice111_96  = hMmcsd->mmcRegs->MMCRSP6;
    sdCid.slice127_112 = hMmcsd->mmcRegs->MMCRSP7;

    hMmcsd->cidSliceInfo = sdCid;

    pSdCardIdObj->mfgId       = ((sdCid.slice127_112 & 0xFF00) >> 8) & 0xFF;

    pSdCardIdObj->oemAppId    = ((sdCid.slice127_112  & 0x00FF) << 8) |
                                 ((sdCid.slice111_96  & 0xFF00) >> 8);

    pSdCardIdObj->productName[5]  = '\0';
    pSdCardIdObj->productName[4]  = ((sdCid.slice79_64) & 0x00FF);
    pSdCardIdObj->productName[3]  = ((sdCid.slice79_64) >> 8);
    pSdCardIdObj->productName[2]  = ((sdCid.slice95_80) & 0x00FF);
    pSdCardIdObj->productName[1]  = ((sdCid.slice95_80) >> 8);
    pSdCardIdObj->productName[0]  = ((sdCid.slice111_96) & 0x00FF);

    pSdCardIdObj->serialNumber  = ((Uint32)(sdCid.slice63_48 & 0x00FF) << 16) |
                                   ((Uint32)sdCid.slice47_32 << 8) |
                                   ((Uint32)(sdCid.slice31_16 >> 8));

    pSdCardIdObj->month = ((sdCid.slice15_0 >> 8) & 0x0F);
    pSdCardIdObj->year  = ((sdCid.slice15_0 >> 12) |
                           ((sdCid.slice31_16 & 0x000F) << 4) + 2000);

    pSdCardIdObj->checksum = (sdCid.slice15_0 & 0x00FE) >> 1;

    hMmcsd->cardObj->cid = pSdCardIdObj;

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b MMC_getCardCsd
 *
 *   @b Description
 *   @n This function will update the CSD Structure information of MMC Card.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pMmcCardCsdObj    Pointer to the MMC Card CSD Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1. MMC_open() API should be called successfully.
 *   @n  2. MMC_selectCard() API should be called before this so that it will do
 *          some needful work for particular card detected.
 *   @n  3. if Card detected is MMC then MMC_sendOpCond()for specific to MMC.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n CSD object structure
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          mmcCardObj;
            CSL_MMCCardCsdObj       mmcCardCsdObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &mmcCardObj);

            clkDivValue = 0x4 //as MMC clock max is 20MHz and
                                Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = MMC_getCardCsd(hMmcsd, &mmcCardCsdObj);

            For example following value will be populated.
            pMmcCardCsdObj->readBlLen         = ;
            pMmcCardCsdObj->cSize  = ;

            These value will be useful to fetch the card specific information
            as shown below. These figure are tested with 256Mbyte MMC Card

            blockLength  = 512 Bytes
            cardCapacity = 0x260636672 Bytes
            totalSectors = 0x509056 sector

     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_getCardCsd(CSL_MmcsdHandle      hMmcsd,
                          CSL_MMCCardCsdObj    *pMmcCardCsdObj)
{
    Uint32                argument;
    Uint32                multVal;
    Uint32                blockNR;
    volatile Uint16       status;
    volatile Uint16       mmcStatus;
    CSL_MMCSDCsdStruct    mmcCsd;

    if((hMmcsd !=  NULL) && (pMmcCardCsdObj != NULL))
    {
         /* Passed Parameter are correct */
    }
    else
    {
        if(NULL == hMmcsd)
        {
            return (CSL_ESYS_BADHANDLE);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    }

    argument = hMmcsd->cardObj->rca;
    argument = argument << CSL_MMCSD_SHIFT_MAX;

    /* CMD 9 */
    hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
    CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
             (Uint16)(argument & CSL_MMCSD_MASK_MAX));
    CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
             (Uint16)((argument >>CSL_MMCSD_SHIFT_MAX) & CSL_MMCSD_MASK_MAX));
    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_9;

    do
    {
        status = hMmcsd->mmcRegs->MMCST0;
        if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
        {
            if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
            {
                return (CSL_EMMCSD_TIMEOUT);
            }
            else
            {
                return (CSL_EMMCSD_CRCERR);
            }
        }
    } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

    /* Update the CSD structure */
    mmcCsd.slice15_0    = hMmcsd->mmcRegs->MMCRSP0;
    mmcCsd.slice31_16   = hMmcsd->mmcRegs->MMCRSP1;
    mmcCsd.slice47_32   = hMmcsd->mmcRegs->MMCRSP2;
    mmcCsd.slice63_48   = hMmcsd->mmcRegs->MMCRSP3;
    mmcCsd.slice79_64   = hMmcsd->mmcRegs->MMCRSP4;
    mmcCsd.slice95_80   = hMmcsd->mmcRegs->MMCRSP5;
    mmcCsd.slice111_96  = hMmcsd->mmcRegs->MMCRSP6;
    mmcCsd.slice127_112 = hMmcsd->mmcRegs->MMCRSP7;

    hMmcsd->csdSliceInfo = mmcCsd;

    /* CMD 7 */
    hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
    CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
             (Uint16)(argument & CSL_MMCSD_MASK_MAX));
    CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
             (Uint16)((argument >>CSL_MMCSD_SHIFT_MAX) &
             CSL_MMCSD_MASK_MAX));
    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_7;

    mmcStatus = 0;
    do
    {
        status = hMmcsd->mmcRegs->MMCST0;

        mmcStatus |= status & CSL_MMCSD_EVENT_EOFCMD;
        mmcStatus |= status & CSL_MMCSD_EVENT_CARD_EXITBUSY;

        if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
        {
            if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
            {
                return (CSL_EMMCSD_TIMEOUT);
            }
            else
            {
                return (CSL_EMMCSD_CRCERR);
            }
        }
    } while(((mmcStatus & CSL_MMCSD_EVENT_EOFCMD) == 0) ||
            ((mmcStatus & CSL_MMCSD_EVENT_CARD_EXITBUSY) == 0));

    /* Update the value to the card Object */
    pMmcCardCsdObj->csdStruct        = (mmcCsd.slice127_112 >> 14) & 0x3;

    pMmcCardCsdObj->crc              = (mmcCsd.slice15_0 >> 1) & 0x7F;
    pMmcCardCsdObj->ecc              = (mmcCsd.slice15_0 >> 8) & 0x3;
    pMmcCardCsdObj->fileFmt          = (mmcCsd.slice15_0 >> 10) & 0x3;
    pMmcCardCsdObj->tmpWriteProtect  = (mmcCsd.slice15_0 >> 12) & 0x1;
    pMmcCardCsdObj->permWriteProtect = (mmcCsd.slice15_0 >> 13) & 0x1;
    pMmcCardCsdObj->copyFlag         = (mmcCsd.slice15_0 >> 14) & 0x1;
    pMmcCardCsdObj->fileFmtGrp       = (mmcCsd.slice15_0 >> 15) & 0x1;

    pMmcCardCsdObj->contProtApp      = mmcCsd.slice31_16 & 0x1;
    pMmcCardCsdObj->writeBlPartial   = (mmcCsd.slice31_16 >> 5) & 0x1;
    pMmcCardCsdObj->writeBlLen       = (mmcCsd.slice31_16 >> 6) & 0xF;
    pMmcCardCsdObj->r2wFactor        = (mmcCsd.slice31_16 >> 10) & 0x7;
    pMmcCardCsdObj->defaultEcc       = (mmcCsd.slice31_16 >> 13) & 0x3;
    pMmcCardCsdObj->wpGrpEnable      = (mmcCsd.slice31_16 >> 15) & 0x1;

    pMmcCardCsdObj->wpGrpSize        = (mmcCsd.slice47_32) & 0x1F;
    pMmcCardCsdObj->eraseGrpMult     = (mmcCsd.slice47_32 >> 5) & 0x1F;
    pMmcCardCsdObj->eraseGrpSize     = (mmcCsd.slice47_32 >> 10) & 0x1F;
    pMmcCardCsdObj->cSizeMult        = (mmcCsd.slice47_32 >> 15) & 0x1;

    pMmcCardCsdObj->cSizeMult        = pMmcCardCsdObj->cSizeMult |
                                       ((mmcCsd.slice63_48 & 0x3) << 1);
    pMmcCardCsdObj->vddWCurrMax      = (mmcCsd.slice63_48 >> 2) & 0x7;
    pMmcCardCsdObj->vddWCurrMin      = (mmcCsd.slice63_48 >> 5) & 0x7;
    pMmcCardCsdObj->vddRCurrMax      = (mmcCsd.slice63_48 >> 8) & 0x7;
    pMmcCardCsdObj->vddRCurrMin      = (mmcCsd.slice63_48 >> 11) & 0x7;
    pMmcCardCsdObj->cSize            = (mmcCsd.slice63_48 >> 14) & 0x3;

    pMmcCardCsdObj->cSize            = pMmcCardCsdObj->cSize |
                                       ((mmcCsd.slice79_64 & 0x3FF) << 2);
    pMmcCardCsdObj->dsrImp           = (mmcCsd.slice79_64 >> 12) & 0x1;
    pMmcCardCsdObj->readBlkMisalign  = (mmcCsd.slice79_64 >> 13) & 0x1;
    pMmcCardCsdObj->writeBlkMisalign = (mmcCsd.slice79_64 >> 14) & 0x1;
    pMmcCardCsdObj->readBlPartial    = (mmcCsd.slice79_64 >> 15) & 0x1;

    pMmcCardCsdObj->readBlLen        = (mmcCsd.slice95_80) & 0xF;
    pMmcCardCsdObj->ccc              = (mmcCsd.slice95_80 >> 4) & 0xFFF;

    pMmcCardCsdObj->tranSpeed        = mmcCsd.slice111_96 & 0xFF;
    pMmcCardCsdObj->nsac             = (mmcCsd.slice111_96 >> 8) & 0xFF;

    pMmcCardCsdObj->taac             = (mmcCsd.slice127_112) & 0xFF;
    pMmcCardCsdObj->mmcProt          = (mmcCsd.slice127_112 >> 10) & 0xF;

    hMmcsd->cardObj->blockLength     = 1 << pMmcCardCsdObj->readBlLen;
    multVal                          = (Uint32)(1 << (pMmcCardCsdObj->cSizeMult + 2));
    blockNR                          = (pMmcCardCsdObj->cSize + 1) * multVal;
    hMmcsd->cardObj->cardCapacity    = hMmcsd->cardObj->blockLength * blockNR;
    hMmcsd->cardObj->totalSectors    = blockNR;

    hMmcsd->cardObj->csd = pMmcCardCsdObj;

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b SD_getCardCsd
 *
 *   @b Description
 *   @n This function will update the CSD Structure info of SD Card.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pSdCardCsdObj     Pointer to the SD Card CSD Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:MMC_open() API should be called successfully.
 *   @n  2:MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3:If Card detected is SD then MMC_sendOpCond()for specific to SD.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MMCCardObj        sdCardObj;
            CSL_MMCCardCsdObj     sdCardCsdObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Uint16                clkDivValue;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &sdCardObj);

            clkDivValue = 0x1 //As SD clock max is 50MHz and Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = SD_getCardCsd(hMmcsd, &sdCardCsdObj);
            For example following value will be populated.
            sdCardCsdObj.readBlLen         = ;
            sdCardCsdObj.cSize  = ;
            These values will be useful to fetch the card specific information
            like these-----
            These figure are tested with 1GB Ultra SD Card
            blockLength      = 512 Bytes
            cardCapacity     = 0x1015808000 Bytes
            totalSectors     = 0x1984000 sector
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SD_getCardCsd(CSL_MmcsdHandle      hMmcsd,
                         CSL_MMCCardCsdObj    *pSdCardCsdObj)
{
    Uint32             argument;
    Uint32             multVal;
    Uint32             blockNR;
    volatile Uint16    status;
    CSL_MMCSDCsdStruct sdCsd;

    if((hMmcsd !=  NULL) && (pSdCardCsdObj != NULL))
    {
         /* Passed Parameter are correct */
    }
    else
    {
        if(NULL == hMmcsd)
        {
            return (CSL_ESYS_BADHANDLE);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    }

    /* CMD 9 */
    argument = hMmcsd->cardObj->rca;
    argument = argument << 16;
    hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
    CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
             (Uint16)(argument & CSL_MMCSD_MASK_MAX));
    CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
             (Uint16)((argument >>CSL_MMCSD_SHIFT_MAX) & CSL_MMCSD_MASK_MAX));
    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_9;

    do
    {
        status = hMmcsd->mmcRegs->MMCST0;
        if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
        {
            if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
            {
                return (CSL_EMMCSD_TIMEOUT);
            }
            else
            {
                return (CSL_EMMCSD_CRCERR);
            }
        }
    } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

    /* Update the CSD structure */
    sdCsd.slice15_0    = hMmcsd->mmcRegs->MMCRSP0;
    sdCsd.slice31_16   = hMmcsd->mmcRegs->MMCRSP1;
    sdCsd.slice47_32   = hMmcsd->mmcRegs->MMCRSP2;
    sdCsd.slice63_48   = hMmcsd->mmcRegs->MMCRSP3;
    sdCsd.slice79_64   = hMmcsd->mmcRegs->MMCRSP4;
    sdCsd.slice95_80   = hMmcsd->mmcRegs->MMCRSP5;
    sdCsd.slice111_96  = hMmcsd->mmcRegs->MMCRSP6;
    sdCsd.slice127_112 = hMmcsd->mmcRegs->MMCRSP7;

    hMmcsd->csdSliceInfo = sdCsd;

    /* CMD 7 */
    hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
    CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
             (Uint16)(argument & CSL_MMCSD_MASK_MAX));
    CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
             (Uint16)((argument >>CSL_MMCSD_SHIFT_MAX) & CSL_MMCSD_MASK_MAX));
    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_7;

    do
    {
        status = hMmcsd->mmcRegs->MMCST0;
        if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
        {
            if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
            {
                return (CSL_EMMCSD_TIMEOUT);
            }
            else
            {
                return (CSL_EMMCSD_CRCERR);
            }
        }
    } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

    /* Set bus width */
    /* CMD55  */
    hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
    CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG1,MMCSD_MMCARG1_ARG1,
             (Uint16)(argument & CSL_MMCSD_MASK_MAX));
    CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
             (Uint16)((argument >>CSL_MMCSD_SHIFT_MAX) & CSL_MMCSD_MASK_MAX));
    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_55;

    do
    {
        status = hMmcsd->mmcRegs->MMCST0;
        if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
        {
            if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
            {
                return (CSL_EMMCSD_TIMEOUT);
            }
            else
            {
                return (CSL_EMMCSD_CRCERR);
            }
        }
    } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

    /* ACMD 6 */
    argument = CSL_MMC_BUSWIDTH_4BIT;
    hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
    CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
    CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
             (Uint16)(argument & CSL_MMCSD_MASK_MAX));
    CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
             (Uint16)((argument >>CSL_MMCSD_SHIFT_MAX) & CSL_MMCSD_MASK_MAX));
    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_6;

    do
    {
        status = hMmcsd->mmcRegs->MMCST0;
        if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
        {
            if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
            {
                return (CSL_EMMCSD_TIMEOUT);
            }
            else
            {
                return (CSL_EMMCSD_CRCERR);
            }
        }
    } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

    /* Set the all data line for SD Card */
    CSL_FINST(hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_WIDTH, BIT4);

    pSdCardCsdObj->csdStruct = (sdCsd.slice127_112 >> 14) & 0x3;

    if(0 == pSdCardCsdObj->csdStruct)
    {
        /* CSD version is 1.0  - SD spec Version 1.01-1.10
           Version 2.00/Standard Capacity */
        pSdCardCsdObj->crc              = (sdCsd.slice15_0 >> 1) & 0x7F;
        pSdCardCsdObj->fileFmt          = (sdCsd.slice15_0 >> 10) & 0x3;
        pSdCardCsdObj->tmpWriteProtect  = (sdCsd.slice15_0 >> 12) & 0x1;
        pSdCardCsdObj->permWriteProtect = (sdCsd.slice15_0 >> 13) & 0x1;
        pSdCardCsdObj->copyFlag         = (sdCsd.slice15_0 >> 14) & 0x1;
        pSdCardCsdObj->fileFmtGrp       = (sdCsd.slice15_0 >> 15) & 0x1;

        pSdCardCsdObj->writeBlPartial   = (sdCsd.slice31_16 >> 5) & 0x1;
        pSdCardCsdObj->writeBlLen       = (sdCsd.slice31_16 >> 6) & 0xF;
        pSdCardCsdObj->r2wFactor        = (sdCsd.slice31_16 >> 10) & 0x7;
        pSdCardCsdObj->wpGrpEnable      = (sdCsd.slice31_16 >> 15) & 0x1;

        pSdCardCsdObj->wpGrpSize        = (sdCsd.slice47_32) & 0x7F;
        pSdCardCsdObj->eraseGrpSize     = (sdCsd.slice47_32 >> 7) & 0x7F;
        pSdCardCsdObj->eraseBlkEnable   = (sdCsd.slice47_32 >> 14) & 0x1;
        pSdCardCsdObj->cSizeMult        = (sdCsd.slice47_32 >> 15) & 0x1;

        pSdCardCsdObj->cSizeMult        = pSdCardCsdObj->cSizeMult  |
                                           ((sdCsd.slice63_48 & 0x3) << 1);
        pSdCardCsdObj->vddWCurrMax      = (sdCsd.slice63_48 >> 2) & 0x7;
        pSdCardCsdObj->vddWCurrMin      = (sdCsd.slice63_48 >> 5) & 0x7;
        pSdCardCsdObj->vddRCurrMax      = (sdCsd.slice63_48 >> 8) & 0x7;
        pSdCardCsdObj->vddRCurrMin      = (sdCsd.slice63_48 >> 11) & 0x7;
        pSdCardCsdObj->cSize            = (sdCsd.slice63_48 >> 14) & 0x3;

        pSdCardCsdObj->cSize            = pSdCardCsdObj->cSize |
                                          ((sdCsd.slice79_64 & 0x3FF) << 2);
        pSdCardCsdObj->dsrImp           = (sdCsd.slice79_64 >> 12) & 0x1;
        pSdCardCsdObj->readBlkMisalign  = (sdCsd.slice79_64 >> 13) & 0x1;
        pSdCardCsdObj->writeBlkMisalign = (sdCsd.slice79_64 >> 14) & 0x1;
        pSdCardCsdObj->readBlPartial    = (sdCsd.slice79_64 >> 15) & 0x1;

        pSdCardCsdObj->readBlLen        = (sdCsd.slice95_80) & 0xF;
        pSdCardCsdObj->ccc              = (sdCsd.slice95_80 >> 4) & 0xfff;

        pSdCardCsdObj->tranSpeed        = (sdCsd.slice111_96) & 0xFF;
        pSdCardCsdObj->nsac             = (sdCsd.slice111_96 >> 8) & 0xFF;

        pSdCardCsdObj->taac             = (sdCsd.slice127_112) & 0xFF;

        hMmcsd->cardObj->blockLength    = 1 << pSdCardCsdObj->readBlLen;
        multVal                         = (Uint32)(1 << (pSdCardCsdObj->cSizeMult + 2));
        blockNR                         = (pSdCardCsdObj->cSize + 1) * multVal;
        hMmcsd->cardObj->cardCapacity   = hMmcsd->cardObj->blockLength * blockNR;
        hMmcsd->cardObj->totalSectors   = blockNR;
    }
    else
    {
        /* CSD version is 2.0 - SD spec Version 2.00/High Capacity */
        pSdCardCsdObj->crc              = (sdCsd.slice15_0 >> 1) & 0x7F;
        pSdCardCsdObj->ecc              = (sdCsd.slice15_0 >> 8) & 0x3;
        pSdCardCsdObj->fileFmt          = (sdCsd.slice15_0 >> 10) & 0x3;
        pSdCardCsdObj->tmpWriteProtect  = (sdCsd.slice15_0 >> 12) & 0x1;
        pSdCardCsdObj->permWriteProtect = (sdCsd.slice15_0 >> 13) & 0x1;
        pSdCardCsdObj->copyFlag         = (sdCsd.slice15_0 >> 14) & 0x1;
        pSdCardCsdObj->fileFmtGrp       = (sdCsd.slice15_0 >> 15) & 0x1;

        pSdCardCsdObj->writeBlPartial   = (sdCsd.slice31_16 >> 5) & 0x1;
        pSdCardCsdObj->writeBlLen       = (sdCsd.slice31_16 >> 6) & 0xF;
        pSdCardCsdObj->r2wFactor        = (sdCsd.slice31_16 >> 10) & 0x7;
        pSdCardCsdObj->wpGrpEnable      = (sdCsd.slice31_16 >> 15) & 0x1;

        pSdCardCsdObj->wpGrpSize        = (sdCsd.slice47_32) & 0x7F;
        pSdCardCsdObj->eraseGrpSize     = (sdCsd.slice47_32 >> 7) & 0x7F;
        pSdCardCsdObj->eraseBlkEnable   = (sdCsd.slice47_32 >> 14) & 0x1;

        pSdCardCsdObj->cSize            = (sdCsd.slice63_48) & 0xFFFF;

        pSdCardCsdObj->cSize            = pSdCardCsdObj->cSize |
                                          (((Uint32)(sdCsd.slice79_64 & 0x3F)) << 16);
        pSdCardCsdObj->dsrImp           = (sdCsd.slice79_64 >> 12) & 0x1;
        pSdCardCsdObj->readBlkMisalign  = (sdCsd.slice79_64 >> 13) &0x1;
        pSdCardCsdObj->writeBlkMisalign = (sdCsd.slice79_64 >> 14) &0x1;
        pSdCardCsdObj->readBlPartial    = (sdCsd.slice79_64 >> 15) &0x1;

        pSdCardCsdObj->readBlLen        = (sdCsd.slice95_80) & 0xF;
        pSdCardCsdObj->ccc              = (sdCsd.slice95_80 >> 4) & 0xFFF;

        pSdCardCsdObj->tranSpeed        = (sdCsd.slice111_96) & 0xFF;
        pSdCardCsdObj->nsac             = (sdCsd.slice111_96 >> 8) & 0xFF;

        pSdCardCsdObj->taac             = (sdCsd.slice127_112) & 0xFF;

        /* Calculate the Block length parameter */
        hMmcsd->cardObj->blockLength    = 1 << pSdCardCsdObj->readBlLen;

        /* Calculate the card capacity */
        hMmcsd->cardObj->cardCapacity   =
                                    ((Uint32)(pSdCardCsdObj->cSize + 1) * 512);
        /* For SD Ver2.0 cards capacity will be KBytes */
        hMmcsd->cardObj->totalSectors   = (hMmcsd->cardObj->cardCapacity * 2);
    }

    hMmcsd->cardObj->csd = pSdCardCsdObj;

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b MMC_read
 *
 *   @b Description
 *   @n This function reads data from the memory card at a given address
 *      and populates the data to read buffer.
 *
 *      'cardAddr' represents the address location in the card to be accessed.
 *      For the standard capacity cards this parameter will be a value in
 *      multiples of 512. That means to read a sector number 1 cardAddr
 *      will be 1*512, to read a sector number 2 cardAddr will be 2*512
 *      and so on.
 *      For the high capacity cards 'cardAddr' parameter will be same as the
 *      sector number. That means to read a sector number 1 cardAddr will be
 *      1, to read a sector number 2 cardAddr will be 2 and so on.
 *      Reason for this is high capacity cards uses BLOCK addressing instead
 *      of the BYTE addressing used by the standard capacity cards to increase
 *      the addressable memory capacity.
 *
 *      Detection of high capacity cards can be verified using the flag
 *      'sdHcDetected' in the MMCSD card object structure.
 *      sdHcDetected = TRUE      - SD card is high capacity card
 *      sdHcDetected = FALSE     - SD card is standard capacity card
 *
 *      NOTE: CARE SHOULD BE TAKEN WHILE PASSING THE 'cardAddr' PARAMETER.
 *      USING BYTE ADDRESSING WITH HIGH CAPACITY CARDS WILL LEAD TO OUT OF
 *      SECTOR BOUNDARY ACCESS AND RESULTS IN CODE HANGING FOR MMCSD RESPONSE.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'readEndianMode' in the MMC handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during MMC open. Endian mode can
 *      be configured using MMC_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *      NOTE: When transferring data in DMA mode, word swap will be done for
 *      the data buffer when MMCSD is configured for little endian mode.
 *      No word swap is required in upper SW layers.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd         Handle to the MMCSD.
            cardAddr       Location to read from.
            noOfBytes      Number of bytes to read.
            pReadBuffer    Pointer to a buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - card Response time out
 *
 *   <b> Pre Condition </b>
 *   @n  1. MMC_open() API should be called successfully.
 *   @n  2. MMC_selectCard() API should be called before this so that it will do
 *          some needful work for particular card detected.
 *   @n  3. MMC_sendOpCond()for specific to card.
 *   @n  4. If card detected is SD then call SD_getCardCsd() specific to SD.
 *          If card detected is MMC then call MMC_getCardCsd() specific to MMC
 *
 *   <b> Post Condition </b>
 *   @n  Populates data to read buffer.
 *
 *   @b Modifies
 *   @n Read data buffer.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MMCCardObj        sdCardObj;
            CSL_MMCCardCsdObj     sdCardCsdObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Uint16                clkDivValue;
            Uint16                noOfBytes;
            Uint16                cardAddr;
            Uint16                readBuffer[256];

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &sdCardObj);

            clkDivValue = 0x1 //as SD clock max is 50MHz and Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = SD_getCardCsd(hMmcsd, &sdCardCsdObj);
            For example following value will be populated.
            sdCardCsdObj.readBlLen = ;
            sdCardCsdObj.cSize     = ;
            These values will be useful to fetch the card specific information
            like these-----
            These figure are tested with 1GB SD Card
            blockLength      = 512 Bytes
            cardCapacity     = 0x1015808000 Bytes
            totalSectors     = 0x1984000 sector

            cardAddr = 0x200;
            noOfBytes = 512;

            status = MMC_read(hMmcsd, cardAddr, noOfBytes, readBuffer);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_read(CSL_MmcsdHandle    hMmcsd,
                    Uint32             cardAddr,
                    Uint16             noOfBytes,
                    Uint16             *pReadBuffer)
{
    volatile Uint32       response;
    volatile Uint16    status;
    volatile Uint16    mmcStatus;
    volatile Uint16    count;
    Uint16             stopCmdRetryCnt;
    CSL_Status         result;
    Uint16             *pReadBuff;
    Uint16             rdCntMmc;
    Uint16             readRetryCount;
    Uint16             blkCnt;
    Uint16             reIssueReadFlag;
    Uint16             readCmd;
    Bool               reStartDma;
    Uint16               saved;

    reStartDma     = 0;
    readRetryCount = CSL_MMSCD_READ_WRITE_RETRY_COUNT;

    if((hMmcsd !=  NULL) && (pReadBuffer != NULL))
    {
        /* Set block length */
        if(noOfBytes != 0)
        {
            /*
             * For standard capacity memory cards, it is possible to set
             * the block length other than 512. Block length configured
             * using MMC_setBlockLength() API will be stored in MMCSD
             * handle.
             */
            if((hMmcsd->blockLen != 0) &&
               (hMmcsd->cardObj->sdHcDetected != TRUE))
            {
                blkCnt = (noOfBytes)/(hMmcsd->blockLen);
                CSL_FINS(hMmcsd->mmcRegs->MMCBLEN, MMCSD_MMCBLEN_BLEN,
                         hMmcsd->blockLen);
            }
            else
            {
                blkCnt = (noOfBytes)/(CSL_MMCSD_BLOCK_LENGTH);
                CSL_FINS(hMmcsd->mmcRegs->MMCBLEN, MMCSD_MMCBLEN_BLEN,
                         CSL_MMCSD_BLOCK_LENGTH);
            }

            CSL_FINS(hMmcsd->mmcRegs->MMCNBLK, MMCSD_MMCNBLK_NBLK, blkCnt);

            mmcStatus = CSL_MMCSD_BUSY_STATE;
            do
            {
                status = hMmcsd->mmcRegs->MMCST1;
                mmcStatus |= status & CSL_MMCSD_FIFO_EMPTY;

                if((status & CSL_MMCSD_BUSY_STATE) == 0)
                {
                    mmcStatus &= ~CSL_MMCSD_BUSY_STATE;
                }
            } while(((mmcStatus & CSL_MMCSD_FIFO_EMPTY) != CSL_MMCSD_FIFO_EMPTY) ||
                     ((mmcStatus & CSL_MMCSD_BUSY_STATE) == CSL_MMCSD_BUSY_STATE));
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }

        /* Set Endian mode */
        CSL_FINS (hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_PERMDR,
                  hMmcsd->readEndianMode);

        /* Reset FIFO */
        CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFORST, RESET);
        /* Configure FIFO for read */
        CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFODIR, READ);
        /* Set FIFO access width */
        if(CSL_MMCSD_OPMODE_POLLED == hMmcsd->opMode)
        {
            CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 2BYTES);
        }
        else
        {
            CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 4BYTES);
        }

        /* Set FIFO threshold */
        CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL,MMCSD_MMCFIFOCTL_FIFOLEV, 256BIT);

        if(blkCnt > 1)
        {
            readCmd = CSL_MMCSD_READ_MULTIPLE_BLOCK_CMD;
        }
        else
        {
            readCmd = CSL_MMCSD_READ_BLOCK_CMD;
        }

        if(CSL_MMCSD_OPMODE_POLLED == hMmcsd->opMode)
        {
            do
            {
                readRetryCount--;
                if(readRetryCount == 0)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }

                /*
                 * This flag enables the reading process to re-start
                 * right from sending the command incase of errors
                 * during the read operation
                 */
                reIssueReadFlag = 0; /* KR032010 */
                pReadBuff       = pReadBuffer;
                rdCntMmc        = 0;

                /* Configure the MMC argument registers */
                saved = IRQ_globalDisable();
                CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                         (Uint16)(cardAddr & CSL_MMCSD_MASK_MAX));
                CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                         (Uint16)((cardAddr >> CSL_MMCSD_SHIFT_MAX) &
                         CSL_MMCSD_MASK_MAX));
                IRQ_globalRestore(saved);

                /* Send Read command */
                hMmcsd->mmcRegs->MMCCMD1 = readCmd;

                do
                {
                    status = hMmcsd->mmcRegs->MMCST0;
                    if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                    {
                        reIssueReadFlag = 1;
                        break;
                    }

                } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

                if(reIssueReadFlag == 1)
                {
                    continue;
                }

                do
                {
                    do
                    {
                        status = hMmcsd->mmcRegs->MMCST0;
                        if((status & CSL_MMCSD_DATA_TOUT_CRC_ERROR) != 0)
                        {
                            reIssueReadFlag = 1;
                            break;
                        }

                    } while((status & CSL_MMCSD_READ_READY) != CSL_MMCSD_READ_READY);

                    if(reIssueReadFlag == 1)
                    {
                        break;
                    }

                    /*
                     * Read from MMCDRR1 register for little endian mode
                     * Read from MMCDRR2 register for big endian mode
                     */
                    if(hMmcsd->readEndianMode == CSL_MMCSD_ENDIAN_LITTLE)
                    {
                        /*
                         * Since FIFO level is configured for 256 bits,
                         * there will be 32 bytes available in the FIFO.
                         * Loop runs 16 times and 2bytes are read in each
                         * interation.
                         */
                        for(count = 0; count < 16; count++)
                        {
                            *pReadBuff++ = CSL_FEXT(hMmcsd->mmcRegs->MMCDRR1,
                                                    MMCSD_MMCDRR1_DRR1);

                            rdCntMmc += 2;
                        }
                    }
                    else
                    {
                        /*
                         * Since FIFO level is configured for 256 bits,
                         * there will be 32 bytes available in the FIFO.
                         * Loop runs 16 times and 2bytes are read in each
                         * interation.
                         */
                        for(count = 0; count < 16; count++)
                        {
                            *pReadBuff++ = CSL_FEXT(hMmcsd->mmcRegs->MMCDRR2,
                                                    MMCSD_MMCDRR2_DRR2);

                            rdCntMmc += 2;
                        }
                    }

                } while(rdCntMmc < noOfBytes);
            } while(reIssueReadFlag); /* End of re-issue read command loop */
        } /* if(CSL_MMCSD_OPMODE_POLLED == hMmcsd->opMode) */
        else if(CSL_MMCSD_OPMODE_DMA == hMmcsd->opMode)
        {
            if(NULL == hMmcsd->hDmaRead)
            {
                return (CSL_ESYS_INVPARAMS);
            }
            else
            {
                if(CSL_DMA_TXBURST_4WORD == hMmcsd->dmaReadCfg.burstLen)
                {
                    CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL,
                              MMCSD_MMCFIFOCTL_FIFOLEV, 128BIT);
                }

                /* Configure DMA channel */
                hMmcsd->dmaReadCfg.dataLen  = noOfBytes;
                hMmcsd->dmaReadCfg.destAddr = (Uint32)pReadBuffer;

                saved = IRQ_globalDisable();
                result = DMA_config(hMmcsd->hDmaRead, &hMmcsd->dmaReadCfg);
                IRQ_globalRestore(saved);
                if(result != CSL_SOK)
                {
                    return (result);
                }

                /* Start DMA data transfer */
                saved = IRQ_globalDisable();
                result = DMA_start(hMmcsd->hDmaRead);
                IRQ_globalRestore(saved);
                if(result != CSL_SOK)
                {
                    return (result);
                }

                do
                {
                    readRetryCount--;
                    if(readRetryCount == 0)
                    {
                        return (CSL_EMMCSD_TIMEOUT);
                    }

                    /*
                     * This flag enables the reading process to re-start
                     * right from sending the command incase of errors
                     * during the read operation
                     */
                    reIssueReadFlag = 0; /* KR032010 */

                    if(reStartDma == 1)
                    {
                        reStartDma = 0;

                        /*
                         * Stop DMA data transfer. Incase of data time-out errors
                         * during data transfer there is possibilty that DMA transfer
                         * is partially complete. To ensure proper start of the DMA,
                         * stop the current transfer.
                         */
                        saved = IRQ_globalDisable();
                        result = DMA_stop(hMmcsd->hDmaRead);
                        IRQ_globalRestore(saved);
                        if(result != CSL_SOK)
                        {
                            return (result);
                        }

                        /* Configure DMA channel */
                        hMmcsd->dmaReadCfg.dataLen  = noOfBytes;
                        hMmcsd->dmaReadCfg.destAddr = (Uint32)pReadBuffer;

                        saved = IRQ_globalDisable();
                        result = DMA_config(hMmcsd->hDmaRead, &hMmcsd->dmaReadCfg);
                        IRQ_globalRestore(saved);
                        if(result != CSL_SOK)
                        {
                            return (result);
                        }

                        /* Start DMA data transfer */
                        saved = IRQ_globalDisable();
                        result = DMA_start(hMmcsd->hDmaRead);
                        IRQ_globalRestore(saved);
                        if(result != CSL_SOK)
                        {
                            return (result);
                        }
                    }

                    /* Configure the MMC argument registers */
                    saved = IRQ_globalDisable();
                    CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                             (Uint16)(cardAddr & CSL_MMCSD_MASK_MAX));
                    CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                             (Uint16)((cardAddr >> CSL_MMCSD_SHIFT_MAX) &
                             CSL_MMCSD_MASK_MAX));
                    IRQ_globalRestore(saved);

                  saved = IRQ_globalDisable();
                  // disable the interrupt for SD0 or SD1 by setting the IER0
                  MMC_ierDisable(hMmcsd);
                  // enable the data transfer related interrupts; data CRC error, data timeout, data done
                  //MMC_intEnable(hMmcsd, (CSL_MMCSD_EVENT_ERROR_DATACRC|CSL_MMCSD_EVENT_ERROR_DATATIMEOUT|CSL_MMCSD_EVENT_BLOCK_XFERRED));
                  MMC_intEnable(hMmcsd, 0xf9);
                  IRQ_globalRestore(saved);

                    /* Send Read command */
                    hMmcsd->mmcRegs->MMCCMD1 = readCmd;

                    /*
                     * Added writeDone callback in place of DMA transfer callback
                     * and while data not done loop KR031310
                     * Renamed writeDone callback to transferDone callback
                     */
                    if(TRUE == hMmcsd->isCallbackSet)
                    {
                          // enable the interrupt for SD0 or SD1 by setting the IER0
                        saved = IRQ_globalDisable();
                        MMC_ierEnable(hMmcsd);
                        IRQ_globalRestore(saved);
                        /*
                         * Invoke the callback function.
                         * Callback function should wait for the MMCSD interrupt.
                         * After getting the interrupt it should disable all the
                         * interrupts.
                         */
                        hMmcsd->dataTransferCallback(hMmcsd);
                        status = hMmcsd->mmcRegs->MMCST0;
                        if((status & CSL_MMCSD_DATA_TOUT_CRC_ERROR) != 0)
                        {
                            if(hMmcsd->mmcRegs->MMCCMD1 ==
                               CSL_MMCSD_READ_MULTIPLE_BLOCK_CMD)
                            {
                                stopCmdRetryCnt = CSL_MMSCD_READ_WRITE_RETRY_COUNT;

                                /* Send stop command */
                                CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
                                         MMCSD_MMCARG1_ARG1,
                                         CSL_MMCSD_STUFF_BITS);
                                CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
                                         MMCSD_MMCARG2_ARG2,
                                         CSL_MMCSD_STUFF_BITS);
                                hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                                mmcStatus = 0;
                                do
                                {
                                    if(stopCmdRetryCnt == 0)
                                    {
                                        return (CSL_EMMCSD_TIMEOUT);
                                    }

                                    status = hMmcsd->mmcRegs->MMCST0;

                                    mmcStatus |= status & CSL_MMCSD_EVENT_EOFCMD;
                                    mmcStatus |= status & CSL_MMCSD_EVENT_CARD_EXITBUSY;

                                    if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                                    {
                                        hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                                        status = hMmcsd->mmcRegs->MMCST0;
                                        mmcStatus = 0;
                                        stopCmdRetryCnt--;
                                    }

                                } while(((mmcStatus & CSL_MMCSD_EVENT_EOFCMD) == 0) ||
                                        ((mmcStatus & CSL_MMCSD_EVENT_CARD_EXITBUSY) == 0) ||
                                        ((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0));
                            }

                            reIssueReadFlag = 1;
                            /*
                             * Since DMA data transfer is already triggered,
                             * DMA needs to be re-configured and re-started
                             * before re-issuing the read command
                             */
                            reStartDma = 1;
                        } // if there is an read error
                        else
                        {
                            // wait for the last read DMA to complete
                            if (MMC_checkReadDmaDone(hMmcsd))
                            {
                               reIssueReadFlag=1;
                            }
                        } // if there is not read error
                    } // if(TRUE == hMmcsd->isCallbackSet)
                    else
                    {
                        // if there is no callback function set, then use CPU polling
                        do
                        {
                            status = hMmcsd->mmcRegs->MMCST0;
                            if((status & CSL_MMCSD_DATA_TOUT_CRC_ERROR) != 0)
                            {
                                if(hMmcsd->mmcRegs->MMCCMD1 ==
                                   CSL_MMCSD_READ_MULTIPLE_BLOCK_CMD)
                                {
                                    stopCmdRetryCnt = CSL_MMSCD_READ_WRITE_RETRY_COUNT;

                                    /* Send stop command */
                                    saved = IRQ_globalDisable();
                                    CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
                                             MMCSD_MMCARG1_ARG1,
                                             CSL_MMCSD_STUFF_BITS);
                                    CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
                                             MMCSD_MMCARG2_ARG2,
                                             CSL_MMCSD_STUFF_BITS);
                                    IRQ_globalRestore(saved);

                                    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                                    mmcStatus = 0;
                                    do
                                    {
                                        if(stopCmdRetryCnt == 0)
                                        {
                                            return (CSL_EMMCSD_TIMEOUT);
                                        }

                                        status = hMmcsd->mmcRegs->MMCST0;

                                        mmcStatus |= status & CSL_MMCSD_EVENT_EOFCMD;
                                        mmcStatus |= status & CSL_MMCSD_EVENT_CARD_EXITBUSY;

                                        if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                                        {
                                            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                                            status = hMmcsd->mmcRegs->MMCST0;
                                            mmcStatus = 0;
                                            stopCmdRetryCnt--;
                                        }

                                    } while(((mmcStatus & CSL_MMCSD_EVENT_EOFCMD) == 0) ||
                                            ((mmcStatus & CSL_MMCSD_EVENT_CARD_EXITBUSY) == 0) ||
                                            ((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0));
                                }

                                reIssueReadFlag = 1;
                                /*
                                 * Since DMA data transfer is already triggered,
                                 * DMA needs to be re-configured and re-started
                                 * before re-issuing the read command
                                 */
                                reStartDma = 1;
                                break; /* KR032010 */
                            }

                        } while((status & 0x01) != 0x01);

                        // wait for the last read DMA to complete
                        if (MMC_checkReadDmaDone(hMmcsd))
                        {
                           reIssueReadFlag=1;
                        }
                    } /* if(FALSE==hMmcsd->isCallbackSet) */
                } while (reIssueReadFlag == 1); /* End of reissue read command loop */

                /* When MMCSD is configured for little endian mode DMA hardware
                   performs a word swap on the data buffers. To ensure that data
                   is written to and read from MMCSD in proper order, a word swap
                   introduced by the SW for the little endian configuration.
                   For read operation word swap is required after reading the data
                   from the memory card using DMA.
                 */
                if(hMmcsd->readEndianMode == CSL_MMCSD_ENDIAN_LITTLE)
                {
                    status = DMA_swapWords(pReadBuffer, noOfBytes/2);
                    if(status != CSL_SOK)
                    {
                        return (status);
                    }
                }
            }
        } /* else if(CSL_MMCSD_OPMODE_DMA == hMmcsd->opMode) */
        else if(CSL_MMCSD_OPMODE_INTERRUPT == hMmcsd->opMode)
        {
            /* Setting callback is mandatory for interrupt mode operation */
            if(TRUE != hMmcsd->isCallbackSet)
            {
                return (CSL_ESYS_INVPARAMS);
            }

            do
            {
                readRetryCount--;
                if(readRetryCount == 0)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }

                /*
                 * This flag enables the reading process to re-start
                 * right from sending the command incase of errors
                 * during the read operation
                 */
                reIssueReadFlag = 0; /* KR032010 */
                pReadBuff       = pReadBuffer;
                rdCntMmc        = 0;

                /* Configure the MMC argument registers */
                  saved = IRQ_globalDisable();
                CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                         (Uint16)(cardAddr & CSL_MMCSD_MASK_MAX));
                CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                         (Uint16)((cardAddr >> CSL_MMCSD_SHIFT_MAX) &
                         CSL_MMCSD_MASK_MAX));
                  IRQ_globalRestore(saved);

                /* Send Read command */
                hMmcsd->mmcRegs->MMCCMD1 = readCmd;

                /*
                 * Enable interrupts:
                 *  Response CRC
                 *  Response Timeout
                 *  Response done
                 */
                hMmcsd->mmcRegs->MMCIM = 0x0094;
                /*
                 * Invoke the callback function.
                 * Callback function should wait for the MMCSD interrupt.
                 * After getting the interrupt it should disable all the
                 * interrupts.
                 */
                hMmcsd->dataTransferCallback(hMmcsd);
                status = hMmcsd->mmcRegs->MMCST0;
                if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                {
                    reIssueReadFlag = 1;
                    continue;
                }

                do
                {
                    /*
                     * Enable interrupts:
                     *  Read Data CRC error
                     *  Read data Timeout errors
                     *  Data Receive ready
                     */
                    hMmcsd->mmcRegs->MMCIM = 0x0448;

                    /*
                     * Invoke the callback function.
                     * Callback function should wait for the MMCSD interrupt.
                     * After getting the interrupt it should disable all the
                     * interrupts.
                     */
                    hMmcsd->dataTransferCallback(hMmcsd);
                    status = hMmcsd->mmcRegs->MMCST0;
                    if(status & CSL_MMCSD_DATA_TOUT_CRC_ERROR)
                    {
                        reIssueReadFlag = 1;
                        break;
                    }

                    /*
                     * Read from MMCDRR1 register for little endian mode
                     * Read from MMCDRR2 register for big endian mode
                     */
                    if(hMmcsd->readEndianMode == CSL_MMCSD_ENDIAN_LITTLE)
                    {
                        /*
                         * Since FIFO level is configured for 256 bits,
                         * there will be 32 bytes available in the FIFO.
                         * Loop runs 16 times and 2bytes are read in each
                         * interation.
                         */
                        for(count = 0; count < 16; count++)
                        {
                            *pReadBuff++ = CSL_FEXT(hMmcsd->mmcRegs->MMCDRR1,
                                                    MMCSD_MMCDRR1_DRR1);

                            rdCntMmc += 2;
                        }
                    }
                    else
                    {
                        /*
                         * Since FIFO level is configured for 256 bits,
                         * there will be 32 bytes available in the FIFO.
                         * Loop runs 16 times and 2bytes are read in each
                         * interation.
                         */
                        for(count = 0; count < 16; count++)
                        {
                            *pReadBuff++ = CSL_FEXT(hMmcsd->mmcRegs->MMCDRR2,
                                                    MMCSD_MMCDRR2_DRR2);

                            rdCntMmc += 2;
                        }
                    }

                } while(rdCntMmc < noOfBytes);
            } while(reIssueReadFlag); /* End of re-issue read command loop */
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }

        hMmcsd->cardObj->lastAddrRead = cardAddr;

        /* Move multiple block stop from end KR032010 */
        if(hMmcsd->mmcRegs->MMCCMD1 == CSL_MMCSD_READ_MULTIPLE_BLOCK_CMD)
        {
            stopCmdRetryCnt = CSL_MMSCD_READ_WRITE_RETRY_COUNT;

            /* Send stop command */
             saved = IRQ_globalDisable();
            CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     CSL_MMCSD_STUFF_BITS);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_MMCSD_STUFF_BITS);
            IRQ_globalRestore(saved);

            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
            mmcStatus = 0;
            do
            {
                if(stopCmdRetryCnt == 0)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }

                status = hMmcsd->mmcRegs->MMCST0;

                mmcStatus |= status & CSL_MMCSD_EVENT_EOFCMD;
                mmcStatus |= status & CSL_MMCSD_EVENT_CARD_EXITBUSY;

                if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                {
                    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                    status = hMmcsd->mmcRegs->MMCST0;
                    mmcStatus = 0;
                    stopCmdRetryCnt--;
                }

            } while(((mmcStatus & CSL_MMCSD_EVENT_EOFCMD) == 0) ||
                    ((mmcStatus & CSL_MMCSD_EVENT_CARD_EXITBUSY) == 0) ||
                    ((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0));
        }

        /* Added Pedro's response KR032010 */
        response = (hMmcsd->mmcRegs->MMCRSP7 << 8);
        response = response << 8;
        response |= hMmcsd->mmcRegs->MMCRSP6;

        if((response & 0xFFFF00FF) != 0)
        {
            return (CSL_EMMCSD_READ_ERROR);
        }
    }
    else
    {
        if(NULL == hMmcsd)
        {
            return (CSL_ESYS_BADHANDLE);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    } /* if((hMmcsd !=  NULL) && (pReadBuffer != NULL)) */

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b MMC_write
 *
 *   @b Description
 *   @n This function writes data in the write data buffer to the memory card
 *      at a given address.
 *
 *      'cardAddr' represents the address location in the card to be accessed.
 *      For the standard capacity cards this parameter will be a value in
 *      multiples of 512. That means to write a sector number 1 cardAddr
 *      will be 1*512, to write a sector number 2 cardAddr will be 2*512
 *      and so on.
 *      For the high capacity cards 'cardAddr' parameter will be same as the
 *      sector number. That means to write a sector number 1 cardAddr will be
 *      1, to write a sector number 2 cardAddr will be 2 and so on.
 *      Reason for this is high capacity cards uses BLOCK addressing instead
 *      of the BYTE addressing used by the standard capacity cards to increase
 *      the addressable memory capacity.
 *
 *      Detection of high capacity cards can be verified using the flag
 *      'sdHcDetected' in the MMCSD card object structure.
 *      sdHcDetected = TRUE      - SD card is high capacity card
 *      sdHcDetected = FALSE     - SD card is standard capacity card
 *
 *      NOTE: CARE SHOULD BE TAKEN WHILE PASSING THE 'cardAddr' PARAMETER.
 *      USING BYTE ADDRESSING WITH HIGH CAPACITY CARDS WILL LEAD TO OUT OF
 *      SECTOR BOUNDARY ACCESS AND RESULTS IN CODE HANGING FOR MMCSD RESPONSE.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'writeEndianMode' in the MMC handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during MMC open. Endian mode can
 *      be configured using MMC_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *      NOTE: When transferring data in DMA mode, word swap will be done for
 *      the data buffer when MMCSD is configured for little endian mode.
 *      No word swap is required in upper SW layers.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD
            cardAddr          location to read from.
            noOfBytes         no of bytes to read.
            pWriteBuffer      pointer to a buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - card Response time out
 *
 *   <b> Pre Condition </b>
 *   @n  1.MMC_open() API should be called successfully.
 *   @n  2.MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3. MMC_sendOpCond()for specific to card .
 *   @n  4. If Card detected is SD then call SD_getCardCsd() specific to SD.
 *          If Card detected is SD then call MMC_getCardCsd() specific to MMC
 *
 *   <b> Post Condition </b>
 *   @n  Writes the data to card
 *
 *   @b Modifies
 *   @n MMC/SD card will populated with the passed buffer values.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MMCCardObj        sdCardObj;
            CSL_MMCCardCsdObj     sdCardCsdObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Uint16                clkDivValue;
            Uint16                noOfBytes;
            Uint16                cardAddr;
            Uint16                writeBuffer[256];

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &sdCardObj);

            sdCardObj.cardType = CSL_SD_CARD;
             clkDivValue = 0x1 //as SD clock max is 50MHz and Function
                                 clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = SD_getCardCsd(hMmcsd, &sdCardCsdObj);
            For example following value will be populated.
            pSdCardCsdObj->readBlLen         = ;
            pSdCardCsdObj->cSize  = ;
            These value will be useful to fetch the card specific information
            These figure are tested with 1GB Ultra SD Card
            blockLength  = 512 Bytes
            cardCapacity = 0x1015808000 Bytes
            totalSectors = 0x1984000 sector

            cardAddr = 0x200;
            noOfBytes = 512;

            status = MMC_write(hMmcsd, cardAddr, noOfBytes, &writeBuffer);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_write(CSL_MmcsdHandle    hMmcsd,
                     Uint32             cardAddr,
                     Uint16             noOfBytes,
                     Uint16             *pWriteBuffer)
{
    volatile Uint32       response;
    volatile Uint16    status;
    volatile Uint16    mmcStatus;
    volatile Uint16    looper;
    volatile Uint16    count;
    Uint16             stopCmdRetryCnt;
    CSL_Status         result;
    Uint16             *pWriteBuff;
    Uint16             wrCntMmc;
    Uint16             writeRetryCount;
    Uint16             blkCnt;
    Uint16             writeCmd;
    Bool               reIssueWriteFlag;
    Bool               reStartDma;
    Uint16               saved;

    reStartDma = 0;
    writeRetryCount = CSL_MMSCD_READ_WRITE_RETRY_COUNT;

    if((hMmcsd !=  NULL) && (pWriteBuffer != NULL))
    {
        /* Set block length */
        if(noOfBytes != 0)
        {
            /*
             * For standard capacity memory cards, it is possible to set
             * the block length other than 512. Block length configured
             * using MMC_setBlockLength() API will be stored in MMCSD
             * handle.
             */
            if((hMmcsd->blockLen != 0) &&
               (hMmcsd->cardObj->sdHcDetected != TRUE))
            {
                blkCnt = (noOfBytes)/(hMmcsd->blockLen);
                CSL_FINS(hMmcsd->mmcRegs->MMCBLEN, MMCSD_MMCBLEN_BLEN,
                         hMmcsd->blockLen);
            }
            else
            {
                blkCnt = (noOfBytes)/(CSL_MMCSD_BLOCK_LENGTH);
                CSL_FINS(hMmcsd->mmcRegs->MMCBLEN, MMCSD_MMCBLEN_BLEN,
                         CSL_MMCSD_BLOCK_LENGTH);
            }

            CSL_FINS(hMmcsd->mmcRegs->MMCNBLK, MMCSD_MMCNBLK_NBLK, blkCnt);

            /*
             * Added new callback to free up idle clocks consumed by
             * while busy loop       KR022410
             * Removed new callback and put while loop back due to
             * writeDonecallback at end KR031310
             */
            mmcStatus = CSL_MMCSD_BUSY_STATE;
            do
            {
                status = hMmcsd->mmcRegs->MMCST1;
                mmcStatus |= (status & CSL_MMCSD_FIFO_EMPTY);
                if((status & CSL_MMCSD_BUSY_STATE) == 0)
                {
                    mmcStatus &= ~CSL_MMCSD_BUSY_STATE;
                }
            } while(((mmcStatus & CSL_MMCSD_FIFO_EMPTY) != CSL_MMCSD_FIFO_EMPTY) ||
                     ((mmcStatus & CSL_MMCSD_BUSY_STATE) == CSL_MMCSD_BUSY_STATE));
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }

        pWriteBuff = pWriteBuffer;
        wrCntMmc = 0;

        /* Set Endian mode */
        CSL_FINS (hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_PERMDX,
                  hMmcsd->writeEndianMode);

        /* Reset FIFO */
        CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFORST, RESET);
        /* Configure fifo for write */
        CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFODIR, WRITE);
        /* Set FIFO access width */
        if(CSL_MMCSD_OPMODE_POLLED == hMmcsd->opMode)
        {
            CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 2BYTES);
        }
        else
        {
            CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 4BYTES);
        }

        /* Set FIFO threshold */
        CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL,MMCSD_MMCFIFOCTL_FIFOLEV, 256BIT);

        /* Check whether to send single block write or multi block write */
        if(blkCnt > 1)
        {
            /* Send multiple block write command */
            writeCmd = CSL_MMCSD_WRITE_MULTIPLE_BLOCK_CMD;
        }
        else  /* if(blkCnt > 1) */
        {
            /* Send single block write command */
            writeCmd = CSL_MMCSD_WRITE_BLOCK_CMD;
        }

        if(CSL_MMCSD_OPMODE_POLLED == hMmcsd->opMode)
        {
            do
            {
                writeRetryCount--;
                if(writeRetryCount == 0)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }
                reIssueWriteFlag = 0;  /* KR032010 */
                pWriteBuff = pWriteBuffer;
                wrCntMmc = 0;

                /* Configure the arg registers */
                 saved = IRQ_globalDisable();
                CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                         (Uint16)(cardAddr & CSL_MMCSD_MASK_MAX));
                CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                         (Uint16)((cardAddr >> CSL_MMCSD_SHIFT_MAX)
                          & CSL_MMCSD_MASK_MAX));
                IRQ_globalRestore(saved);

                /* Send Write command */
                hMmcsd->mmcRegs->MMCCMD1 = writeCmd;
#if 0
                do
                {
                    status = hMmcsd->mmcRegs->MMCST0;
                    if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                    {
                        reIssueWriteFlag = 1;
                        break;
                    }

                } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

                if(reIssueWriteFlag == 1)
                {
                    continue;
                }
#endif

                CSL_FINS(hMmcsd->mmcRegs->MMCCMD2, MMCSD_MMCCMD2_DMATRIG,1);

                do
                {
                    do
                    {
                        status = hMmcsd->mmcRegs->MMCST0;
                        if(status & CSL_MMCSD_EVENT_ERROR)
                        {
                            reIssueWriteFlag = 1;
                            break;
                        }
                    } while((status & CSL_MMCSD_WRITE_READY) != CSL_MMCSD_WRITE_READY);

                    if(reIssueWriteFlag == 1)
                    {
                        break;
                    }

                    /*
                     * Write to MMCDXR1 register for little endian mode
                     * Write to MMCDXR2 register for big endian mode
                     */
                    if(hMmcsd->writeEndianMode == CSL_MMCSD_ENDIAN_LITTLE)
                    {
                        /*
                         * Since FIFO level is configured for 256 bits,
                         * 32 bytes can be written to FIFO.
                         * Loop runs 16 times and 2bytes are written in each
                         * interation.
                         */
                        for(count = 0; count < 16; count++)
                        {
                            CSL_FINS(hMmcsd->mmcRegs->MMCDXR1,
                                     MMCSD_MMCDXR1_DXR1,*pWriteBuff++);

                            wrCntMmc += 2;
                        }
                    }
                    else
                    {
                        /*
                         * Since FIFO level is configured for 256 bits,
                         * 32 bytes can be written to FIFO.
                         * Loop runs 16 times and 2bytes are written in each
                         * interation.
                         */
                        for(count = 0; count < 16; count++)
                        {
                            CSL_FINS(hMmcsd->mmcRegs->MMCDXR2,
                                     MMCSD_MMCDXR2_DXR2,*pWriteBuff++);

                            wrCntMmc += 2;
                        }
                    }

                } while(wrCntMmc < noOfBytes);

                /* Re-issue the write command again, if the previous has errors */
                if(reIssueWriteFlag == 1)
                {
                    continue;
                }

                do
                {
                    status = hMmcsd->mmcRegs->MMCST0;
                    if(status & CSL_MMCSD_EVENT_ERROR)
                    {
                        if(hMmcsd->mmcRegs->MMCCMD1 ==
                           CSL_MMCSD_WRITE_MULTIPLE_BLOCK_CMD)
                        {
                            stopCmdRetryCnt = CSL_MMSCD_READ_WRITE_RETRY_COUNT;

                            /* Send stop command */
                             saved = IRQ_globalDisable();
                            CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
                                     MMCSD_MMCARG1_ARG1,
                                     CSL_MMCSD_STUFF_BITS);
                            CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
                                     MMCSD_MMCARG2_ARG2,
                                     CSL_MMCSD_STUFF_BITS);
                            IRQ_globalRestore(saved);

                            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                            mmcStatus = 0;
                            do
                            {
                                if(stopCmdRetryCnt == 0)
                                {
                                    return (CSL_EMMCSD_TIMEOUT);
                                }

                                status = hMmcsd->mmcRegs->MMCST0;

                                mmcStatus |= status & CSL_MMCSD_EVENT_EOFCMD;
                                mmcStatus |= status & CSL_MMCSD_EVENT_CARD_EXITBUSY;

                                if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                                {
                                    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                                    status = hMmcsd->mmcRegs->MMCST0;
                                    mmcStatus = 0;
                                    stopCmdRetryCnt--;
                                }

                            } while(((mmcStatus & CSL_MMCSD_EVENT_EOFCMD) == 0) ||
                                    ((mmcStatus & CSL_MMCSD_EVENT_CARD_EXITBUSY) == 0) ||
                                    ((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0));
                        }

                        reIssueWriteFlag = 1;
                        break;
                    }

                } while((status & 0x0001) != 0x0001); /* Wait on data done */
            } while (reIssueWriteFlag); /* End of re-issue write command loop */
        }
        else if(CSL_MMCSD_OPMODE_DMA == hMmcsd->opMode)
        {
            if(NULL == hMmcsd->hDmaWrite)
            {
                return (CSL_ESYS_INVPARAMS);
            }
            else
            {
                /* When MMCSD is configured for little endian mode DMA hardware
                   performs a word swap on the data buffers. To ensure that data
                   is written to and read from MMCSD in proper order, a word swap
                   introduced by the SW for the little endian configuration.
                   For write operation word swap is required before writing the
                   data to the memory card using DMA.
                 */
                if(hMmcsd->writeEndianMode == CSL_MMCSD_ENDIAN_LITTLE)
                {
                    result = DMA_swapWords(pWriteBuffer, noOfBytes/2);
                    if(result != CSL_SOK)
                    {
                        return (result);
                    }
                }

                if(CSL_DMA_TXBURST_4WORD == hMmcsd->dmaWriteCfg.burstLen)
                {
                    CSL_FINST(hMmcsd->mmcRegs->MMCFIFOCTL,
                              MMCSD_MMCFIFOCTL_FIFOLEV, 128BIT);
                }

                /* Configure DMA channel */
                hMmcsd->dmaWriteCfg.dataLen = noOfBytes;
                hMmcsd->dmaWriteCfg.srcAddr = (Uint32)pWriteBuffer;

                 saved = IRQ_globalDisable();
                result = DMA_config(hMmcsd->hDmaWrite, &(hMmcsd->dmaWriteCfg));
                IRQ_globalRestore(saved);
                if(result != CSL_SOK)
                {
                    return (result);
                }

                /* Start DMA data transfer */
                 saved = IRQ_globalDisable();
                result = DMA_start(hMmcsd->hDmaWrite);
                IRQ_globalRestore(saved);
                if(result != CSL_SOK)
                {
                    return (result);
                }

                do
                {
                    writeRetryCount--;
                    if(writeRetryCount == 0)
                    {
                        return (CSL_EMMCSD_TIMEOUT);
                    }
                    reIssueWriteFlag = 0; // KR032010

                    if(reStartDma == 1)
                    {
                        reStartDma = 0;

                        /*
                         * Stop DMA data transfer. In case of data time-out errors
                         * during data transfer there is possibilty that DMA transfer
                         * is partially complete. To ensure proper start of the DMA,
                         * stop the current transfer.
                         */
                         saved = IRQ_globalDisable();
                        result = DMA_stop(hMmcsd->hDmaWrite);
                        IRQ_globalRestore(saved);
                        if(result != CSL_SOK)
                        {
                            return (result);
                        }

                        /* Configure DMA channel */
                        hMmcsd->dmaWriteCfg.dataLen = noOfBytes;
                        hMmcsd->dmaWriteCfg.srcAddr = (Uint32)pWriteBuffer;

                         saved = IRQ_globalDisable();
                        result = DMA_config(hMmcsd->hDmaWrite, &(hMmcsd->dmaWriteCfg));
                        IRQ_globalRestore(saved);
                        if(result != CSL_SOK)
                        {
                            return (result);
                        }

                        /* Start DMA data transfer */
                         saved = IRQ_globalDisable();
                        result = DMA_start(hMmcsd->hDmaWrite);
                        IRQ_globalRestore(saved);
                        if(result != CSL_SOK)
                        {
                            return (status);
                        }
                    }

                    /* Configure the arg registers */
                     saved = IRQ_globalDisable();
                    CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                             (Uint16)(cardAddr & CSL_MMCSD_MASK_MAX));
                    CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                             (Uint16)((cardAddr >> CSL_MMCSD_SHIFT_MAX) &
                              CSL_MMCSD_MASK_MAX));
                    IRQ_globalRestore(saved);

                  saved = IRQ_globalDisable();
                    // disable the interrupt for SD0 or SD1 by setting the IER0
                  MMC_ierDisable(hMmcsd);
                  // enable the data transfer related interrupts; data CRC error, data timeout, data done
                  //MMC_intEnable(hMmcsd, (CSL_MMCSD_EVENT_ERROR_DATACRC|CSL_MMCSD_EVENT_ERROR_DATATIMEOUT|CSL_MMCSD_EVENT_BLOCK_XFERRED));
                  MMC_intEnable(hMmcsd, 0xf9);
                  IRQ_globalRestore(saved);

                    /* Send Write command */
                    hMmcsd->mmcRegs->MMCCMD1 = writeCmd;
#if 0
                    do
                    {
                        status = hMmcsd->mmcRegs->MMCST0;
                        if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                        {
                            reIssueWriteFlag = 1;
                            break;
                        }

                    } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

                    if(reIssueWriteFlag == 1)
                    {
                        continue;
                    }
#endif

                      /* Enable MMCSD DMA trigger events */
                      CSL_FINS(hMmcsd->mmcRegs->MMCCMD2, MMCSD_MMCCMD2_DMATRIG,1);

                    /* Replaced DMA callback with dataTransferCallback KR031310 */
                    if(TRUE == hMmcsd->isCallbackSet)
                    {
                          saved = IRQ_globalDisable();
                          MMC_ierEnable(hMmcsd);
                          IRQ_globalRestore(saved);

                        /*
                         * Invoke the callback function.
                         * Callback function should wait for the MMCSD interrupt.
                         * After getting the interrupt it should disable all the
                         * interrupts.
                         */
                        hMmcsd->dataTransferCallback(hMmcsd);
                        status = hMmcsd->mmcRegs->MMCST0;
                        if(status & CSL_MMCSD_EVENT_ERROR) /* if due to read command errors */
                        {
                            if(hMmcsd->mmcRegs->MMCCMD1 ==
                               CSL_MMCSD_WRITE_MULTIPLE_BLOCK_CMD)
                            {
                                stopCmdRetryCnt = CSL_MMSCD_READ_WRITE_RETRY_COUNT;

                                /* Send stop command */
                                saved = IRQ_globalDisable();
                                CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
                                         MMCSD_MMCARG1_ARG1,
                                         CSL_MMCSD_STUFF_BITS);
                                CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
                                         MMCSD_MMCARG2_ARG2,
                                         CSL_MMCSD_STUFF_BITS);
                                IRQ_globalRestore(saved);

                                hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                                mmcStatus = 0;
                                do
                                {
                                    if(stopCmdRetryCnt == 0)
                                    {
                                        return (CSL_EMMCSD_TIMEOUT);
                                    }

                                    status = hMmcsd->mmcRegs->MMCST0;

                                    mmcStatus |= status & CSL_MMCSD_EVENT_EOFCMD;
                                    mmcStatus |= status & CSL_MMCSD_EVENT_CARD_EXITBUSY;

                                    if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                                    {
                                        hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                                        status = hMmcsd->mmcRegs->MMCST0;
                                        mmcStatus = 0;
                                        stopCmdRetryCnt--;
                                    }

                                } while(((mmcStatus & CSL_MMCSD_EVENT_EOFCMD) == 0) ||
                                        ((mmcStatus & CSL_MMCSD_EVENT_CARD_EXITBUSY) == 0) ||
                                        ((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0));
                            }
                            reIssueWriteFlag = 1;
                            /*
                             * Since DMA data transfer is already triggered,
                             * DMA needs to be re-configured and re-started
                             * before re-issuing the write command
                             */
                            reStartDma = 1;
                        }
                    }
                    else
                    {
                        do
                        {
                            status = hMmcsd->mmcRegs->MMCST0;
                            if(status & CSL_MMCSD_EVENT_ERROR)
                            {
                                if(hMmcsd->mmcRegs->MMCCMD1 ==
                                   CSL_MMCSD_WRITE_MULTIPLE_BLOCK_CMD)
                                {
                                    stopCmdRetryCnt = CSL_MMSCD_READ_WRITE_RETRY_COUNT;

                                    /* Send stop command */
                                    saved = IRQ_globalDisable();
                                    CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
                                             MMCSD_MMCARG1_ARG1,
                                             CSL_MMCSD_STUFF_BITS);
                                    CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
                                             MMCSD_MMCARG2_ARG2,
                                             CSL_MMCSD_STUFF_BITS);
                                    IRQ_globalRestore(saved);

                                    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                                    mmcStatus = 0;
                                    do
                                    {
                                        if(stopCmdRetryCnt == 0)
                                        {
                                            return (CSL_EMMCSD_TIMEOUT);
                                        }

                                        status = hMmcsd->mmcRegs->MMCST0;

                                        mmcStatus |= status & CSL_MMCSD_EVENT_EOFCMD;
                                        mmcStatus |= status & CSL_MMCSD_EVENT_CARD_EXITBUSY;

                                        if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                                        {
                                            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                                            status = hMmcsd->mmcRegs->MMCST0;
                                            mmcStatus = 0;
                                            stopCmdRetryCnt--;
                                        }

                                    } while(((mmcStatus & CSL_MMCSD_EVENT_EOFCMD) == 0) ||
                                            ((mmcStatus & CSL_MMCSD_EVENT_CARD_EXITBUSY) == 0) ||
                                            ((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0));
                                }
                                reIssueWriteFlag = 1;
                                /*
                                 * Since DMA data transfer is already triggered,
                                 * DMA needs to be re-configured and re-started
                                 * before re-issuing the write command
                                 */
                                reStartDma = 1;
                                break;
                            }
                        } while((status & 0x0001) != 0x0001); /* Wait on data done */
                    }
                } while (reIssueWriteFlag); /* End of re-issue write command loop */

            } /* if(NULL == hMmcsd->hDmaWrite) */
        } /* if(CSL_MMCSD_OPMODE_DMA == hMmcsd->opMode) */
        else if(CSL_MMCSD_OPMODE_INTERRUPT == hMmcsd->opMode)
        {
            /* Setting callback function is mandatory for interrupt mode operation */
            if(TRUE != hMmcsd->isCallbackSet)
            {
                return (CSL_ESYS_INVPARAMS);
            }

            do
            {
                writeRetryCount--;
                if(writeRetryCount == 0)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }

                reIssueWriteFlag = 0;  /* KR032010 */
                pWriteBuff = pWriteBuffer;
                wrCntMmc = 0;

                /* Configure the arg registers */
                saved = IRQ_globalDisable();
                CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                         (Uint16)(cardAddr & CSL_MMCSD_MASK_MAX));
                CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                         (Uint16)((cardAddr >> CSL_MMCSD_SHIFT_MAX)
                          & CSL_MMCSD_MASK_MAX));
                IRQ_globalRestore(saved);
                /* Send Write command */
                hMmcsd->mmcRegs->MMCCMD1 = writeCmd;

#if 0
                /*
                 * Enable interrupts:
                 *  Response CRC error
                 *  Response Timeout error
                 *  Response done
                 */
                hMmcsd->mmcRegs->MMCIM = 0x0094;

                /*
                 * Invoke the callback function.
                 * Callback function should wait for the MMCSD interrupt.
                 * After getting the interrupt it should disable all the
                 * interrupts.
                 */
                hMmcsd->dataTransferCallback(hMmcsd);
                status = hMmcsd->mmcRegs->MMCST0;
                if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                {
                    reIssueWriteFlag = 1;
                    continue;
                }
#endif

                CSL_FINS(hMmcsd->mmcRegs->MMCCMD2, MMCSD_MMCCMD2_DMATRIG,1);

                do
                {
                    /*
                     * Enable interrupts:
                     *  Write Data CRC error
                     *  Data transmit ready
                     */
                    hMmcsd->mmcRegs->MMCIM = 0x0220;

                    /*
                     * Invoke the callback function.
                     * Callback function should wait for the MMCSD interrupt.
                     * After getting the interrupt it should disable all the
                     * interrupts.
                     */
                    hMmcsd->dataTransferCallback(hMmcsd);
                    status = hMmcsd->mmcRegs->MMCST0;
                    if(status & CSL_MMCSD_EVENT_ERROR)
                    {
                        reIssueWriteFlag = 1;
                        break;
                    }

                    /*
                     * Write to MMCDXR1 register for little endian mode
                     * Write to MMCDXR2 register for big endian mode
                     */
                    if(hMmcsd->writeEndianMode == CSL_MMCSD_ENDIAN_LITTLE)
                    {
                        /*
                         * Since FIFO level is configured for 256 bits,
                         * 32 bytes can be written to FIFO.
                         * Loop runs 16 times and 2bytes are written in each
                         * interation.
                         */
                        for(count = 0; count < 16; count++)
                        {
                            CSL_FINS(hMmcsd->mmcRegs->MMCDXR1,
                                     MMCSD_MMCDXR1_DXR1,*pWriteBuff++);

                            wrCntMmc += 2;
                        }
                    }
                    else
                    {
                        /*
                         * Since FIFO level is configured for 256 bits,
                         * 32 bytes can be written to FIFO.
                         * Loop runs 16 times and 2bytes are written in each
                         * interation.
                         */
                        for(count = 0; count < 16; count++)
                        {
                            CSL_FINS(hMmcsd->mmcRegs->MMCDXR2,
                                     MMCSD_MMCDXR2_DXR2,*pWriteBuff++);

                            wrCntMmc += 2;
                        }
                    }

                } while(wrCntMmc < noOfBytes);

                /* Re-issue the write command again, if the previous has errors */
                if(reIssueWriteFlag == 1)
                {
                    continue;
                }

                if((hMmcsd->mmcRegs->MMCST0 & 0x0001) != 0x0001)
                {
                    /*
                     * Enable interrupts:
                     *  Write Data CRC error
                     *  Data done
                     */
                    hMmcsd->mmcRegs->MMCIM = 0x0021;

                    /*
                     * Invoke the callback function.
                     * Callback function should wait for the MMCSD interrupt.
                     * After getting the interrupt it should disable all the
                     * interrupts.
                     */
                    hMmcsd->dataTransferCallback(hMmcsd);
                    status = hMmcsd->mmcRegs->MMCST0;
                    if(status & CSL_MMCSD_EVENT_ERROR)
                    {
                        if(hMmcsd->mmcRegs->MMCCMD1 ==
                           CSL_MMCSD_WRITE_MULTIPLE_BLOCK_CMD)
                        {
                            stopCmdRetryCnt = CSL_MMSCD_READ_WRITE_RETRY_COUNT;
                            /* Send stop command */
                            saved = IRQ_globalDisable();
                            CSL_FINS(hMmcsd->mmcRegs->MMCARG1,
                                     MMCSD_MMCARG1_ARG1,
                                     CSL_MMCSD_STUFF_BITS);
                            CSL_FINS(hMmcsd->mmcRegs->MMCARG2,
                                     MMCSD_MMCARG2_ARG2,
                                     CSL_MMCSD_STUFF_BITS);
                            IRQ_globalRestore(saved);

                            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                            mmcStatus = 0;
                            do
                            {
                                if(stopCmdRetryCnt == 0)
                                {
                                    return (CSL_EMMCSD_TIMEOUT);
                                }

                                status = hMmcsd->mmcRegs->MMCST0;

                                mmcStatus |= status & CSL_MMCSD_EVENT_EOFCMD;
                                mmcStatus |= status & CSL_MMCSD_EVENT_CARD_EXITBUSY;

                                if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                                {
                                    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                                    status = hMmcsd->mmcRegs->MMCST0;
                                    mmcStatus = 0;
                                    stopCmdRetryCnt--;
                                }

                            } while(((mmcStatus & CSL_MMCSD_EVENT_EOFCMD) == 0) ||
                                    ((mmcStatus & CSL_MMCSD_EVENT_CARD_EXITBUSY) == 0) ||
                                    ((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0));
                        }

                        reIssueWriteFlag = 1;
                        break;
                    }
                }
            } while (reIssueWriteFlag); /* End of re-issue write command loop */
        } /* if(CSL_MMCSD_OPMODE_INTERRUPT == hMmcsd->opMode) */
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }

        hMmcsd->cardObj->lastAddrWritten = cardAddr;

        /* Move multiple block stop command from end  KR032010 */
        if(hMmcsd->mmcRegs->MMCCMD1 == CSL_MMCSD_WRITE_MULTIPLE_BLOCK_CMD)
        {
            stopCmdRetryCnt = CSL_MMSCD_READ_WRITE_RETRY_COUNT;

            /* Send stop command */
            saved = IRQ_globalDisable();
            CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     CSL_MMCSD_STUFF_BITS);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_MMCSD_STUFF_BITS);
            IRQ_globalRestore(saved);

            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
            mmcStatus = 0;
            do
            {
                if(stopCmdRetryCnt == 0)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }

                status = hMmcsd->mmcRegs->MMCST0;

                mmcStatus |= status & CSL_MMCSD_EVENT_EOFCMD;
                mmcStatus |= status & CSL_MMCSD_EVENT_CARD_EXITBUSY;

                if(status & CSL_MMCSD_CMD_TOUT_CRC_ERROR)
                {
                    hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_STOP_CMD;
                    status = hMmcsd->mmcRegs->MMCST0;
                    mmcStatus = 0;
                    stopCmdRetryCnt--;
                }

            } while(((mmcStatus & CSL_MMCSD_EVENT_EOFCMD) == 0) ||
                    ((mmcStatus & CSL_MMCSD_EVENT_CARD_EXITBUSY) == 0) ||
                    ((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0));
        }

        /* Added Pedro's response  KR032010 */
        response = (hMmcsd->mmcRegs->MMCRSP7 << 8);
        response = response << 8;
        response |= hMmcsd->mmcRegs->MMCRSP6;
        if( (response & 0xFFFF00FF) != 0)
        {
            return (CSL_EMMCSD_WRITE_ERROR);
        }
    }
    else
    {
        if(NULL == hMmcsd)
        {
            return (CSL_ESYS_BADHANDLE);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    } /* if((hMmcsd !=  NULL) && (pWriteBuffer != NULL)) */

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b MMC_setDmaHandle
 *
 *   @b Description
 *   @n This function stores the DMA handles sent by the application program.
 *      DMA handle stored in MMCSD handle will be used by the MMC read/write
 *      APIs to transfer the data in DMA mode. This API requires that MMCSD
 *      operating mode should be set to DMA using MMC_open function.
 *      DMA channel handles should be sent by the application in proper order.
 *      DMA channel needs to opened before calling this API. This channels
 *      will be closed by the MMC_close API.
 *
 *      NOTE:This API should not be called when MMCSD is operating Polled mode.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd       MMCSD handle
            hDmaWrite    Handle to DMA channel used for MMCSD write operation
            hDmaRead     Handle to DMA channel used for MMCSD read operation
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid DMA handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() and DMA_open() functions should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n   Stores the DMA handle in MMC handle
 *
 *   @b Modifies
 *   @n    MMC handle
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_DMA_Handle        dmaWrHandle;
            CSL_DMA_Handle        dmaRdHandle;
            CSL_DMA_ChannelObj    dmaWrChanObj;
            CSL_DMA_ChannelObj    dmaRdChanObj;

            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            dmaWrHandle = DMA_open(&dmaWrChanObj, CSL_DMA_CHAN0);

            dmaRdHandle = DMA_open(&dmaRdChanObj, CSL_DMA_CHAN1);

            // Set DMA handle
            status = MMC_setDmaHandle(hMmcsd, dmaWrHandle, dmaRdHandle);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setDmaHandle (CSL_MmcsdHandle    hMmcsd,
                             CSL_DMA_Handle     hDmaWrite,
                             CSL_DMA_Handle     hDmaRead)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(NULL == hMmcsd)
    {
        status = CSL_ESYS_BADHANDLE;
    }
    else
    {
        if((NULL == hDmaWrite) || (NULL == hDmaRead) ||
           (hMmcsd->opMode != CSL_MMCSD_OPMODE_DMA))
        {
            status = CSL_ESYS_INVPARAMS;
        }
        else
        {
            hMmcsd->hDmaWrite = hDmaWrite;
            hMmcsd->hDmaRead  = hDmaRead;
        }
    }

    return(status);
}

/** ===========================================================================
 *   @n@b MMC_setDataTransferCallback
 *
 *   @b Description
 *   @n This function stores the data transfer call back function pointer in the
 *      MMCSD handle. This callback pointer is used by the MMCSD write and read
 *      APIs to issue callback during data transfer operations. This is to
 *      facilitate application/driver programs handle MMCSD interrupt in their
 *      own way depending on their requirements.
 *      Callback function is used when MMCSD is operating in DMA or interrupt
 *      mode.
 *      During DMA mode operation, call back function will be called after
 *      starting the DMA data transfer.
 *      During interrupt mode operation call back function will be called
 *      1. For checking read/write command response
 *      2. For checking transit/receive ready bit
 *      3. For checking data done bit
 *
 *      In both the operating modes callback function should wait for the
 *      MMCSD interrupt. Upon receiving the interrupt, all the MMCSD events
 *      should be disabled. MMCSD handle will be passed to the callback
 *      function as a parameter.
 *
 *      Setting the callback function is optional for DMA mode but is
 *      mandatory for interrupt mode operation. MMC_read/MMC_write APIs
 *      return with error in case of no callback is set and MMCSD is
 *      configured for interrupt mode.
 *
 *      NOTE: Same callback pointer will be used by write and read APIs.
 *            Callback function is applicable to DMA and interrupt mode only.
 *            Should not call this API when MMCSD is operation in polled mode.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd                  MMCSD handle
            dataTransferCallback    Transfer done callback pointer
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Stores the transfer done callback pointer in MMC handle
 *
 *   @b Modifies
 *   @n MMC handle
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle                hMmcsd;
            CSL_MMCDataTxferCallBackPtr    dataTransferCallback;
            CSL_MmcsdInstId                instId;
            CSL_status                     status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = MMC_setDataTransferCallback(hMmcsd, dataTransferCallback);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setDataTransferCallback(CSL_MmcsdHandle                hMmcsd,
                                       CSL_MMCDataTxferCallBackPtr    dataTransferCallback)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(NULL == hMmcsd)
    {
        status = CSL_ESYS_BADHANDLE;
    }
    else
    {
        if(NULL == dataTransferCallback)
        {
            hMmcsd->dataTransferCallback = NULL;
            hMmcsd->isCallbackSet = FALSE;
            status = CSL_ESYS_INVPARAMS;
        }
        else
        {
            hMmcsd->dataTransferCallback = dataTransferCallback;
            hMmcsd->isCallbackSet = TRUE;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b MMC_setDmaChanConfig
 *
 *   @b Description
 *   @n This function is used to set the DMA channel configurations.
 *      DMA channel configuration values sent by the application are stored
 *      in the MMCSD handle and used by the MMC write and read functions.
 *      DMA read and write channel configurations can be set independently
 *      with only one channel at a time using this function. Direction of
 *      the DMA channel is identified using the member 'chanDir' of
 *      CSL_MMCDmaConfig structure. Operating mode of the MMCSD module
 *      needs to be set to DMA mode for using this API.
 *
 *      Members of CSL_MMCDmaConfig structure are subset of CSL_DMA_Config
 *      structure. Other configuration values DMA source address, destination
 *      address, DMA data length etc are not configurable by MMCSD application
 *      program and are set by the MMCSD read/write API depending on the
 *      request from file system.
 *
 *      Only 4word and 8word DMA burst length is allowed for MMCSD (burstLen).
 *      Any other burst length will be ignored and DMA will be configured to
 *      8word burst length by default.
 *
 *      This API is an optional one. Default DMA configurations are assigned by
 *      the MMC_open function. No need to call this API until and unless
 *      a different DMA configurations are required by the application.
 *
 *      NOTE:This API should not be called when MMCSD is operating Polled mode.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd       MMCSD handle
            dmaConfig    DMA configuration structure
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid configuration parameters
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n   Stores the DMA configuration values in MMC handle
 *
 *   @b Modifies
 *   @n    MMC handle
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj    pMmcsdContObj;
            CSL_MmcsdHandle         hMmcsd;;
            CSL_MmcsdDmaConfig      dmaWriteCfg;
            CSL_MmcsdDmaConfig      dmaReadCfg;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            dmaWriteCfg.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            dmaWriteCfg.burstLen     = CSL_DMA_TXBURST_8WORD;
            dmaWriteCfg.chanDir      = CSL_DMA_WRITE;
            dmaWriteCfg.dmaInt       = CSL_DMA_INTERRUPT_ENABLE;

            // Set the DMA write channel configuration
            status = MMC_setDmaChanConfig(hMmcsd, dmaWriteCfg);

            dmaReadCfg.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            dmaReadCfg.burstLen     = CSL_DMA_TXBURST_8WORD;
            dmaReadCfg.chanDir      = CSL_DMA_READ;
            dmaReadCfg.dmaInt       = CSL_DMA_INTERRUPT_ENABLE;

            // Set the DMA read channel configuration
            status = MMC_setDmaChanConfig(hMmcsd, dmaReadCfg);

    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setDmaChanConfig (CSL_MmcsdHandle       hMmcsd,
                                 CSL_MmcsdDmaConfig    dmaConfig)
{
    CSL_Status    status;

    status = CSL_ESYS_INVPARAMS;

    if(NULL == hMmcsd)
    {
        status = CSL_ESYS_BADHANDLE;
    }
    else
    {
        if(hMmcsd->opMode != CSL_MMCSD_OPMODE_DMA)
        {
            return (status);
        }

        if((dmaConfig.autoMode != CSL_DMA_AUTORELOAD_DISABLE) &&
           (dmaConfig.autoMode != CSL_DMA_AUTORELOAD_ENABLE))
        {
            return (status);
        }

        if((dmaConfig.chanDir != CSL_DMA_WRITE) &&
           (dmaConfig.chanDir != CSL_DMA_READ))
        {
            return (status);
        }

        if((dmaConfig.dmaInt != CSL_DMA_INTERRUPT_DISABLE) &&
           (dmaConfig.dmaInt != CSL_DMA_INTERRUPT_ENABLE))
        {
            return (status);
        }

        if((dmaConfig.burstLen != CSL_DMA_TXBURST_8WORD) &&
           (dmaConfig.burstLen != CSL_DMA_TXBURST_4WORD))
        {
            dmaConfig.burstLen = CSL_DMA_TXBURST_8WORD;
        }

#if(defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
        if((dmaConfig.pingPongEnable != FALSE) &&
           (dmaConfig.pingPongEnable != TRUE))
        {
            return (status);
        }
#endif

        status = CSL_SOK;

        if(CSL_DMA_WRITE == dmaConfig.chanDir)
        {
            /* Set the DMA write channel configurations */
            hMmcsd->dmaWriteCfg.autoMode = dmaConfig.autoMode;
            hMmcsd->dmaWriteCfg.burstLen = dmaConfig.burstLen;
            hMmcsd->dmaWriteCfg.dmaInt   = dmaConfig.dmaInt;
#if(defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
            hMmcsd->dmaWriteCfg.pingPongMode =
                                 (CSL_DMAPingPongMode)dmaConfig.pingPongEnable;
#endif
        }
        else
        {
            /* Set the DMA read channel configurations */
            hMmcsd->dmaReadCfg.autoMode = dmaConfig.autoMode;
            hMmcsd->dmaReadCfg.burstLen = dmaConfig.burstLen;
            hMmcsd->dmaReadCfg.dmaInt   = dmaConfig.dmaInt;
#if(defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))
            hMmcsd->dmaReadCfg.pingPongMode =
                                 (CSL_DMAPingPongMode)dmaConfig.pingPongEnable;
#endif
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b MMC_setEndianMode
 *
 *   @b Description
 *   @n This function is used to set endian mode of the MMC/SD controller.
 *      Endian mode set using this API decides the way in which data is
 *      transferred from FIFO to storage device.
 *
 *      Default endianness of MMCSD controller:
 *      Chip C5504 and C5505: LITTLE ENDIAN.
 *      Chip C5514 and C5515: BIG ENDIAN.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd             MMCSD handle
            writeEndianMode    Endian mode for MMC/SD write operation
            readEndianMode     Endian mode for MMC/SD read operation
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n   Sets MMC/SD endian mode for write and operations
 *
 *   @b Modifies
 *   @n    MMC/SD control register
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = MMC_setEndianMode(hMmcsd, CSL_MMCSD_ENDIAN_LITTLE,
                                      CSL_MMCSD_ENDIAN_LITTLE);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setEndianMode (CSL_MmcsdHandle        hMmcsd,
                              CSL_MmcsdEndianMode    writeEndianMode,
                              CSL_MmcsdEndianMode    readEndianMode)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(NULL == hMmcsd)
    {
        status = CSL_ESYS_BADHANDLE;
    }
    else
    {
        if(((writeEndianMode != CSL_MMCSD_ENDIAN_LITTLE) &&
           (writeEndianMode != CSL_MMCSD_ENDIAN_BIG))    ||
           ((readEndianMode != CSL_MMCSD_ENDIAN_LITTLE)  &&
           (readEndianMode != CSL_MMCSD_ENDIAN_BIG)))
        {
            status = CSL_ESYS_INVPARAMS;
        }
        else
        {
            hMmcsd->readEndianMode  = readEndianMode;
            hMmcsd->writeEndianMode = writeEndianMode;

            /* Set Endian mode for write operation */
            CSL_FINS (hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_PERMDX,
                      writeEndianMode);

            /* Set Endian mode for read operation */
            CSL_FINS (hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_PERMDR,
                      readEndianMode);
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b MMC_setBlockLength
 *
 *   @b Description
 *   @n This function is used to set block length of the memory card using
 *      command 16. In the case of a Standard Capacity SD Memory Card or
 *      MMC card, command 16 sets the block length(in bytes) for all following
 *      block commands (read, write, lock). Default block length is fixed to
 *      512 bytes. In the case of a High Capacity SD memory card, block length
 *      set by CMD16 command does not affect the memory read and write
 *      commands. Always 512 bytes fixed block length is used.
 *
 *      This API does not verify the value of block length passed
 *      as parameter. Calling functions should make sure that the block length
 *      passed is proper with respect to the card under operation.
 *
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd      MMCSD handle
            blockLen    Length of the memory card block
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li            CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets MMC/SD block length
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = MMC_setBlockLength(hMmcsd, CSL_MMCSD_BLOCK_LENGTH);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setBlockLength (CSL_MmcsdHandle    hMmcsd,
                               Uint32             blockLen)
{
    volatile Uint16    status;

    if(NULL == hMmcsd)
    {
        return (CSL_ESYS_BADHANDLE);
    }
    else
    {
        /*
         * SDHC and SDXC cannot change block length, it is fixed to 512 bytes
         * for CMD17, CMD18, CMD24, CMD25, CMD32, and CMD33
         * On SD cards, the block length can be set smaller than 512
         */
        /* Send set block length command */
        CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                 (blockLen & CSL_MMCSD_MASK_MAX));
        CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                 ((blockLen >> CSL_MMCSD_SHIFT_MAX) & CSL_MMCSD_MASK_MAX));
        hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_SET_BLKLEN_CMD;
        do
        {
            status = hMmcsd->mmcRegs->MMCST0;
            if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
            {
                if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }
                else
                {
                    return (CSL_EMMCSD_CRCERR);
                }
            }
        } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

        hMmcsd->blockLen = blockLen;
    }

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b MMC_setWriteBlkEraseCnt
 *
 *   @b Description
 *   @n Function to set the number of write blocks to be preerased before
 *      writing. Set write block erase count command need to be used for faster
 *      multiple block write command. Calling this function is not mandatory.
 *      In case of calling this API for better performance, it should be called
 *      just before calling MMC_write function.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd      MMCSD handle
            blkCnt      Number of blocks
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li            CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets preerase block count
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdHandle         hMmcsd;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = MMC_setWriteBlkEraseCnt(hMmcsd, 16);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setWriteBlkEraseCnt(CSL_MmcsdHandle    hMmcsd,
                                   Uint32             blkCnt)
{
    volatile Uint16    status;

    if(NULL != hMmcsd)
    {
        /* Pre-erase block command - ACMD23 */
        /* Send CMD55  */
        CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
        CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1, 0);
        CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                 hMmcsd->cardObj->rca);
        hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_55;

        do
        {
            status = hMmcsd->mmcRegs->MMCST0;
            if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
            {
                if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }
                else
                {
                    return (CSL_EMMCSD_CRCERR);
                }
            }
        } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

        /* ACMD 23 */
        CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
        CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                 (Uint16)(blkCnt & CSL_MMCSD_MASK_MAX));
        CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                 (Uint16)((blkCnt >> CSL_MMCSD_SHIFT_MAX) & 0x7F));
        hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_ACMD_23;
        do
        {
            status = hMmcsd->mmcRegs->MMCST0;
            if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
            {
                if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                {
                    return (CSL_EMMCSD_TIMEOUT);
                }
                else
                {
                    return (CSL_EMMCSD_CRCERR);
                }
            }
        } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);
    }
    else
    {
        return (CSL_ESYS_BADHANDLE);
    }

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b SD_setBusWidth
 *
 *   @b Description
 *   @n Function to set the bus with for SD card. This function sends ACMD6 to
 *      the SD card to configure the bus width. This function should be called
 *      after selecting the card using CMD7.
 *
 *      NOTE: This function is specific to SD card.Should not be used with MMC.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        MMCSD handle
            busWidth      bus width of the card; 0 - 1 bit, 1 - 4 bit
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *   @li            CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li            CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets SD card bus width
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdHandle         hMmcsd;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = SD_setBusWidth(hMmcsd, 1);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SD_setBusWidth(CSL_MmcsdHandle    hMmcsd,
                          Uint8              busWidth)
{
    volatile Uint16    status;
    Uint32             argument;

    if(NULL != hMmcsd)
    {
        if((busWidth == 0) || (busWidth == 1))
        {
            /* Send CMD55  */
            CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1, 0);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     hMmcsd->cardObj->rca);
            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_55;

            do
            {
                status = hMmcsd->mmcRegs->MMCST0;
                if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_EMMCSD_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_EMMCSD_CRCERR);
                    }
                }
            } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

            /* Send ACMD 6 */
            if(busWidth == 1)
            {
                argument = 0x02;
            }
            else
            {
                argument = 0x0;
            }

            CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     (Uint16)(argument & CSL_MMCSD_MASK_MAX));
            CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     (Uint16)((argument >> CSL_MMCSD_SHIFT_MAX) &
                               CSL_MMCSD_MASK_MAX));
            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_6;

            do
            {
                status = hMmcsd->mmcRegs->MMCST0;
                if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_EMMCSD_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_EMMCSD_CRCERR);
                    }
                }
            } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

            /* Set the bus width in MMC control register */
            CSL_FINS(hMmcsd->mmcRegs->MMCCTL, MMCSD_MMCCTL_WIDTH, busWidth);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    }
    else
    {
        return (CSL_ESYS_BADHANDLE);
    }

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b MMC_getCardStatus
 *
 *   @b Description
 *   @n Function to get the status of the memory card. This function sends
 *      CMD13 to memory card to get the status. Memory card status will have
 *      the useful information to determine the card status and errors if any
 *      occurred. Refer to MMC/SD specification for the details of fields
 *      contained in the MMC/SD status.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd         MMCSD handle
            pCardStatus    Pointer to store the status sent by the card
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *   @li            CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li            CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Gets card status
 *
 *   @b Modifies
 *   @n Card status pointer
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdHandle         hMmcsd;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint32                  cardStatus;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = MMC_getCardStatus(hMmcsd, &cardStatus);
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_getCardStatus(CSL_MmcsdHandle    hMmcsd,
                             Uint32             *pCardStatus)
{
    volatile Uint16    status;

    if(NULL != hMmcsd)
    {
        if(pCardStatus != NULL)
        {
            /* Send CMD 13 */
            hMmcsd->mmcRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
            hMmcsd->mmcRegs->MMCRSP6 = CSL_MMCSD_MMCRSP6_RESETVAL;
            CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1, 0);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     hMmcsd->cardObj->rca);

            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_13;
            do
            {
                status = hMmcsd->mmcRegs->MMCST0;
                if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_EMMCSD_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_EMMCSD_CRCERR);
                    }
                }
            } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

            /* Read the card status from response register */
            *pCardStatus = hMmcsd->mmcRegs->MMCRSP7;
            *pCardStatus = (*pCardStatus << CSL_MMCSD_SHIFT_MAX) |
                            hMmcsd->mmcRegs->MMCRSP6;
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    }
    else
    {
        return (CSL_ESYS_BADHANDLE);
    }

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b SD_configurePullup
 *
 *   @b Description
 *   @n Function to Connenct/Disconnect the SD card internal pull-up resistors.
 *      This function is specific to SD card; cannot be used with MMC card
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd         MMCSD handle
            pullupCfg      Flag indicate the pull-up configuration
                             0 - Disconnects the pull-up resistor
                             1 - Connects the pull-up resistor
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *   @li            CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li            CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *   @n  Card should be selected
 *
 *   <b> Post Condition </b>
 *   @n  Connencts/Disconnects SD card pull-up resistor
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdHandle         hMmcsd;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint32                  cardStatus;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);
            ....

            //Select the card

            ....

            status = SD_configurePullup(hMmcsd, 0);
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SD_configurePullup(CSL_MmcsdHandle    hMmcsd,
                              Bool               pullupCfg)
{
    volatile Uint16    status;

    if(hMmcsd != NULL)
    {
        if((pullupCfg == 0) || (pullupCfg == 1))
        {
            /* Send CMD55  */
            CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1, 0);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     hMmcsd->cardObj->rca);
            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_CMD_55;

            do
            {
                status = hMmcsd->mmcRegs->MMCST0;
                if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_EMMCSD_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_EMMCSD_CRCERR);
                    }
                }
            } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);

            /* Send ACMD42 */
            CSL_FINS(hMmcsd->mmcRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG1, MMCSD_MMCARG1_ARG1, pullupCfg);
            CSL_FINS(hMmcsd->mmcRegs->MMCARG2, MMCSD_MMCARG2_ARG2, 0);
            hMmcsd->mmcRegs->MMCCMD1 = CSL_MMCSD_ACMD_42;

            do
            {
                status = hMmcsd->mmcRegs->MMCST0;
                if((status & CSL_MMCSD_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(status & CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_EMMCSD_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_EMMCSD_CRCERR);
                    }
                }
            } while((status & CSL_MMCSD_EVENT_EOFCMD) == 0);
        }
        else
        {
            return (CSL_ESYS_INVPARAMS);
        }
    }
    else
    {
        return (CSL_ESYS_BADHANDLE);
    }

    return (CSL_SOK);
}

// check for the DMA done for the last DMA transfer
CSL_Status MMC_checkReadDmaDone(void *hMmcSd)
{
    CSL_Status status;
    CSL_MmcsdHandle myHandle = hMmcSd;
    volatile Uint16 dmaStatus;

    if (hMmcSd==NULL)
    {
        status = CSL_ESYS_BADHANDLE;
    }
    else
    {
        // wait for the read DMA to complete
        do
        {
            switch (myHandle->hDmaRead->chanNum)
            {
            case 0:
                dmaStatus = CSL_FEXT(myHandle->hDmaRead->dmaRegs->DMACH0TCR2, DMA_DMACH0TCR2_STATUS);
                break;
            case 1:
                dmaStatus = CSL_FEXT(myHandle->hDmaRead->dmaRegs->DMACH1TCR2, DMA_DMACH0TCR2_STATUS);
                break;
            case 2:
                dmaStatus = CSL_FEXT(myHandle->hDmaRead->dmaRegs->DMACH2TCR2, DMA_DMACH0TCR2_STATUS);
                break;
            case 3:
                dmaStatus = CSL_FEXT(myHandle->hDmaRead->dmaRegs->DMACH3TCR2, DMA_DMACH0TCR2_STATUS);
                break;
            default:
                dmaStatus = 0;
            }
        } while (dmaStatus);
        status = CSL_SOK;
    }

    return (status);
}


