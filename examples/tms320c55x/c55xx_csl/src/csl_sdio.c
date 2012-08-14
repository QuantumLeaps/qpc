/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_sdio.c
 *
 *  @brief SDIO functional layer API definition file
 *
 *  Path: \(CSLPATH)\ src
 */

/* ============================================================================
 * Revision History
 * ================
 * 28-Dec-2009 Created
 * 25-Jan-2009 Formatted the code
 * ============================================================================
 */

#include <csl_sdio.h>

/** ===========================================================================
 *   @n@b SDIO_init
 *
 *   @b Description
 *   @n This is the initialization function for the SDIO CSL. The function
 *      must be called before calling any other API from this CSL. This
 *      function is for enabling the clock to the SDIO Controller.
 *      This function should be called only once during system initialization
 *      time. This function gates the clock for the two MMCSD modules.
 *      Application programs may disable the clocks for MMCSD module which is
 *      not in use.
 *
 *    NOTE: Calling this function is not required if MMC_init is called since
 *    both functions use the same MMCSD controller
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK - Always returns
 *
 *   <b> Pre Condition </b>
 *   @n  This should be called before calling any other SDIO Function.
 *
 *   <b> Post Condition </b>
 *   @n  Enables clock for MMCSD controller
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_status       status;

            status = SDIO_init();
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_init (void)
{
    CSL_Status    status;

    status = CSL_SOK;

    /* Enable clock for MMCSD0 and MMCSD1 module */
    CSL_FINST(CSL_SYSCTRL_REGS->PCGCR1, SYS_PCGCR1_MMCSD0CG, ACTIVE);
    CSL_FINST(CSL_SYSCTRL_REGS->PCGCR2, SYS_PCGCR1_MMCSD1CG, ACTIVE);

    /* Set the reset clock cycle */
    CSL_FINS(CSL_SYSCTRL_REGS->PSRCR, SYS_PSRCR_COUNT, CSL_SDIO_RESET_COUNT);
    CSL_FINST(CSL_SYSCTRL_REGS->PRCR, SYS_PRCR_PG3_RST, RST);

    /* Enable Serial port 0 pin for mmcsd0  */
    CSL_FINST(CSL_SYSCTRL_REGS->EBSR, SYS_EBSR_SP0MODE, MODE0);
    /* Enable Serial port 1 pin for mmcsd1  */
    CSL_FINST(CSL_SYSCTRL_REGS->EBSR, SYS_EBSR_SP1MODE, MODE0);

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_open
 *
 *   @b Description
 *   @n This function opens the CSL SDIO module and returns handle to the
 *      SDIO object structure.
 *
 *      NOTE: THIS API SETS THE ENDIAN MODE OF SDIO READ AND WRITE OPERATIONS
 *            TO LITTLE ENDIAN. USE SDIO_setEndianMode() TO CHANGE ENDIAN MODE.
 *
 *   @b Arguments
 *   @verbatim
            pSdioContObj    SDIO Controller object structure
            instId          Instance number for SDIO
            status          Status of the function
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_SdioHandle
 *   @n                     Valid SDIO handle will be returned if
 *                          status value is equal to CSL_SOK.
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_init() API should be called before SDIO_open().
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is returned in the status variable. If status
 *              returned is
 *   @li            CSL_SOK            - Valid SDIO handle is returned
 *   @li            CSL_ESYS_INVPARAMS - Resource is already in use
 *   @n   2.    CSL_SdioObj object structure is populated
 *
 *   @b Modifies
 *   @n    1. Status variable
 *   @n    2. CSL_SdioObj object structure
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio = SDIO_open(&sdioContObj, instId, &status);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_SdioHandle SDIO_open (CSL_SdioControllerObj    *pSdioContObj,
                          CSL_SdioInstId           instId,
                          CSL_Status               *status)
{
    CSL_SdioHandle    hSdio;

    hSdio = NULL;

    if(pSdioContObj == NULL)
    {
        if(status != NULL)
        {
            *status = CSL_ESYS_INVPARAMS;
        }

        hSdio = NULL;
    }
    else if(status == NULL)
    {
        hSdio = NULL;
    }
    else
    {
        switch (instId)
        {
            case CSL_SDIO_INST0:
            {
                *status                = CSL_SOK;
                pSdioContObj->sdioRegs = CSL_MMCSD0_REGS;
                hSdio                  = pSdioContObj;
                break;
            }

            case CSL_SDIO_INST1:
            {
                *status                = CSL_SOK;
                pSdioContObj->sdioRegs = CSL_MMCSD1_REGS;
                hSdio                  = pSdioContObj;
                break;
            }

            default:
            {
                *status                = CSL_ESYS_INVPARAMS;
                pSdioContObj->sdioRegs = NULL;
                hSdio                  = NULL;
                break;
            }
        }

        hSdio->readEndianMode  = CSL_SDIO_ENDIAN_LITTLE;
        hSdio->writeEndianMode = CSL_SDIO_ENDIAN_LITTLE;

        /* Set Endian mode for write operation */
        CSL_FINS (hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_PERMDX,
                  hSdio->writeEndianMode);

        /* Set Endian mode for read operation */
        CSL_FINS (hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_PERMDR,
                  hSdio->readEndianMode);
    }

    return (hSdio);
}

/** ===========================================================================
 *   @n@b SDIO_close
 *
 *   @b Description
 *   @n This function closes the specified handle to SDIO.
 *
 *   @b Arguments
 *   @verbatim
            hSdio            Handle to the SDIO
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully before this function.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n SDIO handle
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_close(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_close(CSL_SdioHandle    hSdio)
{
    CSL_Status    status;

    status = CSL_ESYS_BADHANDLE;

    if(NULL != hSdio)
    {
        hSdio->sdioRegs = NULL;
        hSdio           = NULL;
        status          = CSL_SOK;
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_config
 *
 *   @b Description
 *   @n This function configures the SDIO controller hardware registers.
 *
 *   @b Arguments
 *   @verbatim
            hSdio              Handle to the SDIO.
            pSdioConfig        Pointer to the SDIO config Parameter.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called before this function.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n SDIO Registers.
 *
 *   @b Example
 *   @verbatim

            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            CSL_SdioConfig           sdioConfig;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            // Initialize SDIO configuration structure
            sdioConfig.mmcctl  = .... ;
            ...
            ...
            sdioConfig.sdioien = .... ;


            // Configure SDIO
            status = SDIO_config(hSdio, &sdioConfig);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_config(CSL_SdioHandle     hSdio,
                       CSL_SdioConfig     *pSdioConfig)
{
    CSL_Status    status;

    status = CSL_ESYS_BADHANDLE;

    if(NULL != hSdio)
    {
        if(NULL != pSdioConfig)
        {
            hSdio->sdioRegs->MMCCTL     = pSdioConfig->mmcctl;
            hSdio->sdioRegs->MMCCLK     = pSdioConfig->mmcclk;
            hSdio->sdioRegs->MMCIM      = pSdioConfig->mmcim;
            hSdio->sdioRegs->MMCTOR     = pSdioConfig->mmctor;
            hSdio->sdioRegs->MMCTOD     = pSdioConfig->mmctod;
            hSdio->sdioRegs->MMCBLEN    = pSdioConfig->mmcblen;
            hSdio->sdioRegs->MMCNBLK    = pSdioConfig->mmcnblk;
            hSdio->sdioRegs->MMCFIFOCTL = pSdioConfig->mmcfifoctl;
            hSdio->sdioRegs->SDIOCTL    = pSdioConfig->sdioctl;
            hSdio->sdioRegs->SDIOIEN    = pSdioConfig->sdioien;

            status = CSL_SOK;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_detectCard
 *
 *   @b Description
 *   @n This function is used to detect type of the SDIO card inserted. This
 *      function sends SDIO specification defined command sequence to detect
 *      type of the card. Type of the card shall be populated in pSdioCardObj.
 *      This function also populates other fields of card object structure.
 *      Address pointed by 'pSdioCardObj' should be global structure variable
 *      which persists till end of the program since it is stored by SDIO CSL
 *      for reference in other APIs. This API returns success only incase of
 *      SDIO card inserted into the MMC/SD slot of the EVM. For the SDIO cards
 *      which returns OCR value as 0, card type will be set to
 *      'CSL_SDIO_INVALID_CARD'. Application/Driver programs should validate
 *      card type and ocr value updated in the card object structure to
 *      proceed further.
 *
 *   @b Arguments
 *   @verbatim
            hSdio              Handle to the SDIO.
            pSdioCardObj       Pointer to SDIO card object structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESYS_FAIL       - Card detection failed
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called before this function.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n pSdioCardObj
 *
 *   @b Example
 *   @verbatim

            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            CSL_SdioCardObj          sdioCardObj;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_detectCard(CSL_SdioHandle      hSdio,
                           CSL_SdioCardObj     *pSdioCardObj)
{
    Uint32            argument;
    Uint32            response;
    Uint32            ocr;
    CSL_Status        status;
    volatile Uint16   statusReg;
    Uint16            resp[8];
    Uint16            cmd5Timeout;
    Bool              mem;

    argument    = 0;
    response    = 0;
    ocr         = 0;
    status      = CSL_ESYS_BADHANDLE;
    statusReg   = 0;
    cmd5Timeout = CSL_SDIO_CMD_RETRY_COUNT;
    mem         = 0;

    if(NULL != hSdio)
    {
        if(NULL != pSdioCardObj)
        {
            pSdioCardObj->sdioCardType = CSL_SDIO_CARD_NONE;
            hSdio->pSdioCardObj = pSdioCardObj;

            CSL_FINST(hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_CMDRST, DISABLE);
            CSL_FINST(hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_DATRST, DISABLE);

            /* Set SDIO clock to 400KHz */
            CSL_FINST(hSdio->sdioRegs->MMCCLK, MMCSD_MMCCLK_CLKEN, DISABLE);
            CSL_FINS(hSdio->sdioRegs->MMCCLK, MMCSD_MMCCLK_CLKRT,
                     CSL_SDIO_CLK_DIV_INIT);

            hSdio->sdioRegs->MMCTOR = CSL_SDIO_RESPONSE_TIMEOUT;
            hSdio->sdioRegs->MMCTOD = CSL_SDIO_DATA_RW_TIMEOUT;

            CSL_FINST(hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_WIDTH, BIT1);

            CSL_FINST(hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_CMDRST, ENABLE);
            CSL_FINST(hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_DATRST, ENABLE);

            /* Send CMD0 */
            CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);

            CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     CSL_SDIO_STUFF_BITS);
            CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_SDIO_STUFF_BITS);

            hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_0;

            do
            {
                statusReg = hSdio->sdioRegs->MMCST0;
            } while ((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

            /* CMD 8 - Support for high capacity cards ( > 2GB capacity) */
            argument = CSL_SDIO_CMD_8_ARG;

            hSdio->sdioRegs->MMCRSP6 = CSL_MMCSD_MMCRSP6_RESETVAL;
            CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);

            CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     (argument & CSL_SDIO_MASK_WORD));
            CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_SDIO_RSHIFT(argument, CSL_SDIO_SHIFT_WORD));

            hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_8;

            do
            {
                statusReg = hSdio->sdioRegs->MMCST0;
                if((statusReg & CSL_SDIO_EVENT_ERROR_CMDCRC) != 0)
                {
                    return (CSL_ESDIO_CRCERR);
                }
            } while((statusReg & (CSL_SDIO_EVENT_EOFCMD |
                     CSL_SDIO_EVENT_ERROR)) == 0);

            if((statusReg & CSL_SDIO_EVENT_ERROR) != 0)
            {
                hSdio->cmd8Resp = 0;
            }
            else
            {
                if(CSL_SDIO_CMD_8_ARG == (hSdio->sdioRegs->MMCRSP6 & 0x1FF))
                {
                    hSdio->cmd8Resp = 1;
                }
                else
                {
                    /* Card with incompatible voltage range */
                    pSdioCardObj->sdioCardType = CSL_SDIO_INVALID_CARD;
                    return (CSL_ESYS_FAIL);
                }
            }

            /* Send CMD 5 */
            do
            {
                CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
                hSdio->sdioRegs->MMCRSP6 = CSL_MMCSD_MMCRSP6_RESETVAL;
                hSdio->sdioRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;

                CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                         CSL_SDIO_STUFF_BITS);
                CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                         CSL_SDIO_STUFF_BITS);

                /* Send CMD5 - Command 5 sends response R4 with out CRC */
                hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_5;

                do
                {
                    statusReg = hSdio->sdioRegs->MMCST0;
                    if((statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT) != 0)
                    {
                        return (CSL_ESDIO_TIMEOUT);
                    }

                } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

                resp[0] = hSdio->sdioRegs->MMCRSP6;
                resp[1] = hSdio->sdioRegs->MMCRSP7;

                response = CSL_SDIO_LSHIFT(resp[1], 16) | resp[0];

                cmd5Timeout--;

            } while ((response == 0) && (cmd5Timeout != 0));

            if(cmd5Timeout == 0)
            {
                return (CSL_ESDIO_TIMEOUT);
            }

            ocr = CSL_SDIO_GET_OCR(response);
            mem = CSL_SDIO_GET_MEM(response);

            if(ocr == 0)
            {
                pSdioCardObj->sdioCardType = CSL_SDIO_INVALID_CARD;
            }
            else
            {
                if(mem == 1)
                {
                    pSdioCardObj->sdioCardType = CSL_SDIO_COMBO_CARD;
                }
                else
                {
                    pSdioCardObj->sdioCardType = CSL_SDIO_IO_CARD;
                }

                pSdioCardObj->ocr       = ocr;
                pSdioCardObj->funCount  = CSL_SDIO_GET_FUNCNT(response);
                pSdioCardObj->cardReady = CSL_SDIO_GET_C(response);
            }

            status = CSL_SOK;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_initCard
 *
 *   @b Description
 *   @n This function is used to initialize the SDIO card inserted. This
 *      function sends SDIO specification defined command sequence to initialize
 *      the SDIO card. Proper OCR value should be sent to this function based
 *      on the OCR value received from the card during card detection.
 *      SDIO card shall be ready to use after successful execution of this
 *      function. This function also supports initializing memory portion of
 *      the COMBO SDIO card. Parameter 'initMem' is useful to select(initMem = 1)
 *      or skip(initMem = 0) the initialization of COMBO SDIO cards.
 *      This function returns with error incase of IO portion or memory(COMBO)
 *      portion of the card is not ready.
 *
 *   @b Arguments
 *   @verbatim
            hSdio        Handle to the SDIO.
            OpCondReg    Operation Conditions Regsiter(OCR) value
            initMem      Flag to select/skip memory initialization
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK                - successful
 *   @li                    CSL_ESYS_BADHANDLE     - Invalid handle
 *   @li                    CSL_ESDIO_TIMEOUT      - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR       - Response/Data CRC Error
 *   @li                    CSL_ESDIO_IO_NOTREADY  - Card IO is not ready
 *   @li                    CSL_ESDIO_MEM_NOTREADY - Card memory is not ready
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_detectCard() API should be called before this function.
 *
 *   <b> Post Condition </b>
 *   @n  Initializes SDIO card
 *
 *   @b Modifies
 *   @n SDIO card registers
 *
 *   @b Example
 *   @verbatim

            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            CSL_SdioCardObj          sdioCardObj;
            Uint32                   ocr;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_initCard(CSL_SdioHandle    hSdio,
                         Uint32            OpCondReg,
                         Bool              initMem)
{
    Uint32             argument;
    Uint32             response;
    CSL_Status         status;
    volatile Uint16    statusReg;
    volatile Uint16    sdioStatus;
    Uint16             cmdTimeout;
    Uint16             resp[8];

    argument   = 0;
    response   = 0;
    status     = CSL_ESYS_BADHANDLE;
    statusReg  = 0;
    sdioStatus = 0;
    cmdTimeout = CSL_SDIO_CMD_RETRY_COUNT;

    if(NULL != hSdio)
    {
        /*
         * Send CMD 5 to set proper OCR - This command retries until
         * the card IO becomes ready or timeout occurs
         */
        do
        {
            argument = OpCondReg;

            CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
            hSdio->sdioRegs->MMCRSP6 = CSL_MMCSD_MMCRSP6_RESETVAL;
            hSdio->sdioRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;

            CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     (argument & CSL_SDIO_MASK_WORD));
            CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_SDIO_RSHIFT(argument, CSL_SDIO_SHIFT_WORD));

            hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_5;

            do
            {
                statusReg = hSdio->sdioRegs->MMCST0;
                if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                {
                    return (CSL_ESDIO_TIMEOUT);
                }

            } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

            resp[0] = hSdio->sdioRegs->MMCRSP6;
            resp[1] = hSdio->sdioRegs->MMCRSP7;

            response = CSL_SDIO_LSHIFT(resp[1], 16) | resp[0];

            hSdio->pSdioCardObj->cardReady = CSL_SDIO_GET_C(response);

            cmdTimeout--;

        } while((0 == hSdio->pSdioCardObj->cardReady) && (0 != cmdTimeout));

        if(0 == cmdTimeout)
        {
            status = CSL_ESDIO_IO_NOTREADY;
        }
        else
        {
            /* Initialize memory in case of COMBO cards */
            if((CSL_SDIO_COMBO_CARD == hSdio->pSdioCardObj->sdioCardType) &&
               (TRUE == initMem))
            {
                if(hSdio->cmd8Resp == 0)
                {
                    /*
                     * Card not responded for CMD8 hence Ver2.00 or later
                     * SD Memory Card(voltage mismatch) or Ver1.X SD Memory Card
                     * or not SD Memory Card; ACMD41 needs to be send with HCS = 0;
                     */
                    argument = CSL_SDIO_CMD_41_ARG;
                }
                else
                {
                    /*
                     * Card responded for CMD8; So SD ver2.00 or later SD card.
                     * ACMD41 needs to be sent with HCS = 1
                     */
                    argument = CSL_SDIO_CMD_41_HCS_ARG;
                }

                cmdTimeout = CSL_SDIO_CMD_RETRY_COUNT;

                do
                {
                    /* CMD 55 */
                    CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);

                    CSL_FINS(hSdio->sdioRegs->MMCARG1,
                             MMCSD_MMCARG1_ARG1, CSL_SDIO_STUFF_BITS);
                    CSL_FINS(hSdio->sdioRegs->MMCARG2,
                             MMCSD_MMCARG2_ARG2, CSL_SDIO_STUFF_BITS);

                    hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_55;

                    do
                    {
                        statusReg = hSdio->sdioRegs->MMCST0;
                        if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
                        {
                            if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                            {
                                return (CSL_ESDIO_TIMEOUT);
                            }
                            else
                            {
                                return (CSL_ESDIO_CRCERR);
                            }
                        }
                    } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);


                    /* ACMD 41 */
                    hSdio->sdioRegs->MMCRSP6 = CSL_MMCSD_MMCRSP6_RESETVAL;
                    hSdio->sdioRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
                    CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);

                    CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                             (argument & CSL_SDIO_MASK_WORD));
                    CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                             CSL_SDIO_RSHIFT(argument, CSL_SDIO_SHIFT_WORD));

                    hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_41;

                    do
                    {
                        statusReg = hSdio->sdioRegs->MMCST0;
                        if((statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT) != 0)
                        {
                            return (CSL_ESDIO_TIMEOUT);
                        }
                    } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

                    resp[0] = hSdio->sdioRegs->MMCRSP6;
                    resp[1] = hSdio->sdioRegs->MMCRSP7;

                    response = CSL_SDIO_LSHIFT(resp[1], 16) | resp[0];

                    cmdTimeout--;

                } while (((response & CSL_SDIO_CMD41_RESP) !=
                           CSL_SDIO_CMD41_RESP) && (cmdTimeout != 0));

                if(0 == cmdTimeout)
                {
                    return (CSL_ESDIO_MEM_NOTREADY);
                }

                if((response & CSL_SDIO_SDHC_RESP) != 0)
                {
                    hSdio->pSdioCardObj->sdHcDetected = TRUE;
                }
                else
                {
                    hSdio->pSdioCardObj->sdHcDetected = FALSE;
                }
            }
            else
            {
                /* Get the card relative address - CMD 3 */
                CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
                hSdio->sdioRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;

                CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                         CSL_SDIO_STUFF_BITS);
                CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                         CSL_SDIO_STUFF_BITS);

                hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_3;

                do
                {
                    statusReg = hSdio->sdioRegs->MMCST0;
                    if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
                    {
                        if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                        {
                            return (CSL_ESDIO_TIMEOUT);
                        }
                        else
                        {
                            return (CSL_ESDIO_CRCERR);
                        }
                    }

                } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

                resp[0] = hSdio->sdioRegs->MMCRSP7;

                hSdio->pSdioCardObj->rca = resp[0];

                /* Select card - CMD 7 */
                argument = 0;
                argument = CSL_SDIO_LSHIFT(resp[0], 16);

                CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);

                CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                         (argument & CSL_SDIO_MASK_WORD));
                CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                         CSL_SDIO_RSHIFT(argument, CSL_SDIO_SHIFT_WORD));

                hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_7;

                sdioStatus = 0;
                do
                {
                    statusReg = hSdio->sdioRegs->MMCST0;

                    sdioStatus |= statusReg & CSL_SDIO_EVENT_EOFCMD;
                    sdioStatus |= statusReg & CSL_SDIO_EVENT_CARD_EXITBUSY;

                    if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
                    {
                        if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                        {
                            return (CSL_ESDIO_TIMEOUT);
                        }
                        else
                        {
                            return (CSL_ESDIO_CRCERR);
                        }
                    }
                } while(((sdioStatus & CSL_SDIO_EVENT_EOFCMD) == 0) ||
                        ((sdioStatus & CSL_SDIO_EVENT_CARD_EXITBUSY) == 0));
            }

            status = CSL_SOK;
        }
    }

    hSdio->pSdioCardObj->cardActive = TRUE;

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_resetCard
 *
 *   @b Description
 *   @n This function is used to reset the SDIO card. This function uses CMD52
 *      to reset SDIO card. After execution of this function SDIO card will
 *      not receive any commands except CMD5.
 *
 *   @b Arguments
 *   @verbatim
            hSdio              Handle to the SDIO.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called before this function.
 *
 *   <b> Post Condition </b>
 *   @n  Resets SDIO card
 *
 *   @b Modifies
 *   @n SDIO card registers
 *
 *   @b Example
 *   @verbatim

            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_resetCard(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_resetCard(CSL_SdioHandle    hSdio)
{
    Uint32             argument;
    Uint32             wrData;
    Uint32             regAddr;
    Uint32             rawFlag;
    Uint32             funNum;
    Uint32             rwFlag;
    CSL_Status         status;
    volatile Uint16    statusReg;

    argument   = 0;
    wrData     = 0;
    regAddr    = 0;
    rawFlag    = 0;
    funNum     = 0;
    rwFlag     = 0;
    status     = CSL_ESYS_BADHANDLE;
    statusReg  = 0;

    if(NULL != hSdio)
    {
        /* Reset IO: CMD 52 */
        wrData  = 0x08;
        regAddr = 0x06;
        rawFlag = 0;
        funNum  = 0;
        rwFlag  = 0;

        argument = 0;
        argument = CSL_SDIO_MAKE_CMD52_WRITE_ARG(wrData, regAddr,
                                                 rawFlag, funNum,
                                                 rwFlag);

        CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                 (argument & CSL_SDIO_MASK_WORD));
        CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                 CSL_SDIO_RSHIFT(argument, CSL_SDIO_SHIFT_WORD));

        hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_52;

        do
        {
            statusReg = hSdio->sdioRegs->MMCST0;
            if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
            {
                if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                {
                    return (CSL_ESDIO_TIMEOUT);
                }
                else
                {
                    return (CSL_ESDIO_CRCERR);
                }
            }

        } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

        status = CSL_SOK;
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_getRca
 *
 *   @b Description
 *   @n This function is used to read new relative card address from the card.
 *      This function sends CMD3 and reads relative card address. RCA read from
 *      the card will be populated to the pRCardAddr and card object structure
 *      in SDIO handle.
 *
 *   @b Arguments
 *   @verbatim
            hSdio            Handle to the SDIO.
            pRCardAddr       Relative card address.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n pRCardAddr variable will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint16                   rca;
            Uint32                   ocr;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            status = SDIO_getRca(hSdio, &rca);

     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_getRca(CSL_SdioHandle     hSdio,
                       Uint16             *pRCardAddr)
{
    CSL_Status         status;
    volatile Uint16    statusReg;

    status     = CSL_ESYS_BADHANDLE;
    statusReg  = 0;

    if(NULL != hSdio)
    {
        if(NULL != pRCardAddr)
        {
            hSdio->sdioRegs->MMCRSP7 = CSL_MMCSD_MMCRSP7_RESETVAL;
            CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     CSL_SDIO_STUFF_BITS);
            CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_SDIO_STUFF_BITS);
            hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_3;

            do
            {
                statusReg = hSdio->sdioRegs->MMCST0;
                if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_ESDIO_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_ESDIO_CRCERR);
                    }
                }
            } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

            *pRCardAddr = hSdio->sdioRegs->MMCRSP7;
            hSdio->pSdioCardObj->rca = *pRCardAddr;
            status = CSL_SOK;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_deselectCard
 *
 *   @b Description
 *   @n This function is used to deselect SDIO which has been selected earlier.
 *      CMD7 shall be sent to the SDIO card with the relative card address set
 *      to 0x0000.
 *
 *   @b Arguments
 *   @verbatim
            hSdio      Handle to the SDIO.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called.
 *   @n  3:Card should be active
 *
 *   <b> Post Condition </b>
 *   @n  Deselects the SDIO card
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            CSL_SdioCardObj          sdioCardObj;
            Uint32                   ocr;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            status = SDIO_deselectCard(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_deselectCard(CSL_SdioHandle    hSdio)
{
    CSL_Status         status;
    Uint16             deselectCmd;
    volatile Uint16    statusReg;
    Uint16             rca;

    status      = CSL_ESYS_BADHANDLE;
    deselectCmd = 0;
    statusReg   = 0;
    rca         = CSL_SDIO_CARD_DESELECT_RCA;

    if(NULL != hSdio)
    {
        CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                 CSL_SDIO_STUFF_BITS);
        CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2, rca);

        CSL_FINST(deselectCmd, MMCSD_MMCCMD1_DCLR, NO);
        CSL_FINST(deselectCmd, MMCSD_MMCCMD1_INITCK, NO);
        CSL_FINST(deselectCmd, MMCSD_MMCCMD1_WDATX, NO);
        CSL_FINST(deselectCmd, MMCSD_MMCCMD1_STRMTP, NO);
        CSL_FINST(deselectCmd, MMCSD_MMCCMD1_DTRW, NO);
        CSL_FINST(deselectCmd, MMCSD_MMCCMD1_RSPFMT, NORSP);
        CSL_FINST(deselectCmd, MMCSD_MMCCMD1_BSYEXP, NO);
        CSL_FINST(deselectCmd, MMCSD_MMCCMD1_PPLEN, OD);
        CSL_FINS(deselectCmd, MMCSD_MMCCMD1_CMD, 0x07);

        hSdio->sdioRegs->MMCCMD1 = deselectCmd;

        do
        {
            statusReg = hSdio->sdioRegs->MMCST0;
            if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
            {
                if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                {
                    return (CSL_ESDIO_TIMEOUT);
                }
                else
                {
                    return (CSL_ESDIO_CRCERR);
                }
            }

        } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

        hSdio->pSdioCardObj->cardActive = FALSE;
        status = CSL_SOK;
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_selectCard
 *
 *   @b Description
 *   @n This function is used to select SDIO card. CMD7 shall be sent to the
 *      SDIO card along with the relative card address to select the card.
 *      RCA value should be the one published by the card. Sending CMD7
 *      with invalid RCA will put the card into standby/disabled state.
 *
 *   @b Arguments
 *   @verbatim
            hSdio      Handle to the SDIO.
            rca        Relative card address.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called.
 *
 *   <b> Post Condition </b>
 *   @n  Selects the SDIO card
 *
 *   @b Modifies
 *   @n rca
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint16                   rca;
            Uint32                   ocr;
            CSL_SdioCardObj          sdioCardObj;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            rca = sdioCardObj.rca;

            status = SDIO_initCard(hSdio, ocr, TRUE);

            status = SDIO_selectCard(hSdio, rca);

     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_selectCard(CSL_SdioHandle    hSdio,
                           Uint16            rca)
{
    CSL_Status         status;
    volatile Uint16    statusReg;
    volatile Uint16    sdioStatus;

    status     = CSL_ESYS_BADHANDLE;
    statusReg  = 0;
    sdioStatus = 0;

    if(NULL != hSdio)
    {
        CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                 CSL_SDIO_STUFF_BITS);
        CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2, rca);

        hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_7;

        sdioStatus = 0;
        do
        {
            statusReg = hSdio->sdioRegs->MMCST0;

            sdioStatus |= statusReg & CSL_SDIO_EVENT_EOFCMD;
            sdioStatus |= statusReg & CSL_SDIO_EVENT_CARD_EXITBUSY;

            if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
            {
                if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                {
                    return (CSL_ESDIO_TIMEOUT);
                }
                else
                {
                    return (CSL_ESDIO_CRCERR);
                }
            }
        } while(((sdioStatus & CSL_SDIO_EVENT_EOFCMD) == 0) ||
                ((sdioStatus & CSL_SDIO_EVENT_CARD_EXITBUSY) == 0));

        hSdio->pSdioCardObj->cardActive = TRUE;
        hSdio->pSdioCardObj->rca        = rca;

        status = CSL_SOK;
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_sendCommand
 *
 *   @b Description
 *   @n This function is used to send commands to the SDIO card. This function
 *      sends given command to the card and waits for the response.
 *      Lower 16 bits of the parameter 'cmd' should contain the command that
 *      needs to be written to MMCCMD1 register. Upper 16 bits of the 'cmd'
 *      should contain the data that needs to be written to MMCCMD2 register.
 *
 *      NOTE:SETTING THE MMCCMD2 VALUE TO '1' IS ALLOWED ONLY FOR DATA WRITE
 *      COMMANDS THAT USES DMA FOR DATA TRANSFER. FOR OTHER COMMANDS UPPER
 *      16 BITS OF 'CMD' SHOULD ALWAYS BE ZERO.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            cmd            MMCCMD1 and MMCCMD2 register value.
            arg            Command argument.
            respTimeout    Timeout value for the response.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Sends command to the card
 *
 *   @b Modifies
 *   @n Card Command registers
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   cmd;
            Uint32                   arg;
            Uint16                   respTimeout;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            //Send Command 5
            cmd         = 0x205;
            arg         = 0;
            respTimeout = 0xFFFF;
            status = SDIO_sendCommand(hSdio, cmd, arg, respTimeout);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_sendCommand(CSL_SdioHandle    hSdio,
                            Uint32            cmd,
                            Uint32            arg,
                            Uint16            respTimeout)
{
    CSL_Status         status;
    volatile Uint16    statusReg;

    status    = CSL_ESYS_BADHANDLE;
    statusReg = 0;

    if(NULL != hSdio)
    {
        CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
        CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                 (Uint16)(arg & CSL_SDIO_MASK_WORD));
        CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                 (Uint16)((arg >> CSL_SDIO_SHIFT_WORD) & CSL_SDIO_MASK_WORD));

        hSdio->sdioRegs->MMCCMD1 = (Uint16)(cmd & CSL_SDIO_MASK_WORD);

        do
        {
            statusReg = hSdio->sdioRegs->MMCST0;
            if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
            {
                if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                {
                    return (CSL_ESDIO_TIMEOUT);
                }
                else
                {
                    return (CSL_ESDIO_CRCERR);
                }
            }

        } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

        /*
         * Configuring MMCCMD2 is applicable only to data write commands
         * that uses dma for data transfer.
         * For other commands upper 16 bits of 'cmd' should always be zero
         */
        hSdio->sdioRegs->MMCCMD2 = (Uint16)((cmd >> CSL_SDIO_SHIFT_WORD)
                                                  & CSL_SDIO_MASK_WORD);

        status = CSL_SOK;
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_getResponse
 *
 *   @b Description
 *   @n This function is used to read the response registers. This function
 *      reads response registers based on the response type and populates the
 *      response structure. Response registers are not cleared by this function.
 *
 *\n    Response type     Response bits       Resp buf Fields populated
 *\n    ==============    ============       =========================
 *\n    R1,R3,R4,R5,R6      [23:8]             pResponse->respBuf[6]
 *\n                        [39:24]            pResponse->respBuf[7]
 *\n
 *\n        R2              [15:0]             pResponse->respBuf[0]
 *\n                        [31:16]            pResponse->respBuf[1]
 *\n                        [47:32]            pResponse->respBuf[2]
 *\n                        [63:48]            pResponse->respBuf[3]
 *\n                        [79:64]            pResponse->respBuf[4]
 *\n                        [95:80]            pResponse->respBuf[5]
 *\n                        [111:96]           pResponse->respBuf[6]
 *\n                        [127:112]          pResponse->respBuf[7]
 *
 *
 * NOTE: IF RESPONSE TYPE SENT IS NOT RECOGNIZED BY THIS API ALL THE RESPONSE
 * REGISTERS ARE POPULATED IN THE SAME FORMAT MENTIONED ABOVE FOR R2 RESPONSE.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            respType       Type of the response
            pResponse      Pointer to response structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_sendCommand() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n Response structure
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   cmd;
            Uint32                   arg;
            Uint16                   respTimeout;
            CSL_SdioResponse         response;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            //Send Command 5
            cmd         = 0x205;
            arg         = 0;
            respTimeout = 0xFFFF;
            status = SDIO_sendCommand(hSdio, cmd, arg, respTimeout);

            status = SDIO_getResponse(hSdio, CSL_SDIO_RESPTYPE_R4, &response);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_getResponse(CSL_SdioHandle      hSdio,
                            CSL_SdioRespType    respType,
                            CSL_SdioResponse    *pResponse)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(NULL != hSdio)
    {
        if(NULL != pResponse)
        {
            switch (respType)
            {
                case CSL_SDIO_RESPTYPE_R1:
                case CSL_SDIO_RESPTYPE_R3:
                case CSL_SDIO_RESPTYPE_R4:
                case CSL_SDIO_RESPTYPE_R5:
                case CSL_SDIO_RESPTYPE_R6:
                    pResponse->respBuf[6] = hSdio->sdioRegs->MMCRSP6;
                    pResponse->respBuf[7] = hSdio->sdioRegs->MMCRSP7;
                    break;

                case CSL_SDIO_RESPTYPE_R2:
                    pResponse->respBuf[0] = hSdio->sdioRegs->MMCRSP0;
                    pResponse->respBuf[1] = hSdio->sdioRegs->MMCRSP1;
                    pResponse->respBuf[2] = hSdio->sdioRegs->MMCRSP2;
                    pResponse->respBuf[3] = hSdio->sdioRegs->MMCRSP3;
                    pResponse->respBuf[4] = hSdio->sdioRegs->MMCRSP4;
                    pResponse->respBuf[5] = hSdio->sdioRegs->MMCRSP5;
                    pResponse->respBuf[6] = hSdio->sdioRegs->MMCRSP6;
                    pResponse->respBuf[7] = hSdio->sdioRegs->MMCRSP7;

                default: /* Unknown response type */
                    pResponse->respBuf[0] = hSdio->sdioRegs->MMCRSP0;
                    pResponse->respBuf[1] = hSdio->sdioRegs->MMCRSP1;
                    pResponse->respBuf[2] = hSdio->sdioRegs->MMCRSP2;
                    pResponse->respBuf[3] = hSdio->sdioRegs->MMCRSP3;
                    pResponse->respBuf[4] = hSdio->sdioRegs->MMCRSP4;
                    pResponse->respBuf[5] = hSdio->sdioRegs->MMCRSP5;
                    pResponse->respBuf[6] = hSdio->sdioRegs->MMCRSP6;
                    pResponse->respBuf[7] = hSdio->sdioRegs->MMCRSP7;
                    break;
            }
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_clearResponse
 *
 *   @b Description
 *   @n This function is used to clear all the response registers.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n Response structure
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            CSL_SdioResponse         response;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_clearResponse(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_clearResponse(CSL_SdioHandle      hSdio)
{
    CSL_Status    status;

    status = CSL_ESYS_BADHANDLE;

    if(NULL !=  hSdio)
    {
        CSL_FINS(hSdio->sdioRegs->MMCRSP0, MMCSD_MMCRSP0_MMCRSP0,
                 CSL_MMCSD_MMCRSP0_RESETVAL);

        CSL_FINS(hSdio->sdioRegs->MMCRSP1, MMCSD_MMCRSP1_MMCRSP1,
                 CSL_MMCSD_MMCRSP1_RESETVAL);

        CSL_FINS(hSdio->sdioRegs->MMCRSP2, MMCSD_MMCRSP2_MMCRSP2,
                 CSL_MMCSD_MMCRSP2_RESETVAL);

        CSL_FINS(hSdio->sdioRegs->MMCRSP3, MMCSD_MMCRSP3_MMCRSP3,
                 CSL_MMCSD_MMCRSP3_RESETVAL);

        CSL_FINS(hSdio->sdioRegs->MMCRSP4, MMCSD_MMCRSP4_MMCRSP4,
                 CSL_MMCSD_MMCRSP4_RESETVAL);

        CSL_FINS(hSdio->sdioRegs->MMCRSP5, MMCSD_MMCRSP5_MMCRSP5,
                 CSL_MMCSD_MMCRSP5_RESETVAL);

        CSL_FINS(hSdio->sdioRegs->MMCRSP6, MMCSD_MMCRSP6_MMCRSP6,
                 CSL_MMCSD_MMCRSP6_RESETVAL);

        CSL_FINS(hSdio->sdioRegs->MMCRSP7, MMCSD_MMCRSP7_MMCRSP7,
                 CSL_MMCSD_MMCRSP7_RESETVAL);

        /* Clear the command index bits also in MMCCIDX register */
        CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX,
                 CSL_MMCSD_MMCCIDX_RESETVAL);

        status = CSL_SOK;
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_getStatus
 *
 *   @b Description
 *   @n This function reads MMCST0 and MMCST1 registers and returns the value.
 *      When mask value is set to 0xFFFFFFFF, value of MMCST0 will be stored
 *      in lower 16 bits and value of MMCST1 will be store in upper 16 bits.
 *      Value of a particular bit in the passed mask should be set to '1' to
 *      read the corresponding bit in the MMC status register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            maskValue      Mask value to specify the field.
            pStatus        Status of API Success or Failed.
     @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *                          Status  - MMCST0 and MMCST1 register value.
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   mmcstatus;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            mmcstatus = SDIO_getStatus(hSdio, 0xFFFF, &status);
     @endverbatim
 *  ===========================================================================
 */
Uint32 SDIO_getStatus(CSL_SdioHandle    hSdio,
                      Uint32            maskValue,
                      CSL_Status        *pStatus)
{
    Uint32    statusReg;

    statusReg = 0;

    if(NULL != hSdio)
    {
        *pStatus = CSL_SOK;

        statusReg  = hSdio->sdioRegs->MMCST1;
        statusReg  = statusReg << CSL_SDIO_SHIFT_WORD;
        statusReg |= hSdio->sdioRegs->MMCST0;
        statusReg &= maskValue;
    }
    else
    {
        *pStatus = CSL_ESYS_BADHANDLE;
    }

    return (statusReg);
}

/** ===========================================================================
 *   @n@b SDIO_getSdioStatus
 *
 *   @b Description
 *   @n This function reads SDIOST0 and SDIOIST registers and returns the value.
 *      When mask value is set to 0xFFFFFFFF, value of SDIOST0 will be stored
 *      in lower 16 bits and value of SDIOIST will be store in upper 16 bits.
 *      Value of a particular bit in the passed mask should be set to '1' to
 *      read the corresponding bit in the SDIO status register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            maskValue      Mask value to specify the field.
            pStatus        status of API Success of Failed.
     @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *                          Status  - SDIOST0 and SDIOIST register value.
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   sdiostatus;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            sdiostatus = SDIO_getSdioStatus(hSdio, 0xFFFF, &status);
     @endverbatim
 *  ===========================================================================
 */
Uint32 SDIO_getSdioStatus(CSL_SdioHandle    hSdio,
                          Uint32            maskValue,
                          CSL_Status        *pStatus)
{
    Uint32    statusReg;

    statusReg = 0;

    if(NULL != hSdio)
    {
        *pStatus = CSL_SOK;

        statusReg  = hSdio->sdioRegs->SDIOIST;
        statusReg  = statusReg << CSL_SDIO_SHIFT_WORD;
        statusReg |= hSdio->sdioRegs->SDIOST0;
        statusReg &= maskValue;
    }
    else
    {
        *pStatus = CSL_ESYS_BADHANDLE;
    }

    return (statusReg);
}

/** ===========================================================================
 *   @n@b SDIO_eventEnable
 *
 *   @b Description
 *   @n This function will enable the events in the MMC interrupt mask register
 *      and SDIO interrupt enable register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio         Handle to the SDIO.
            sdioEvent     Event type.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Enables the given event
 *
 *   @b Modifies
 *   @n MMC/SDIO Interrupt Mask Registers
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_eventEnable(hSdio, CSL_SDIO_EDRRDY_INTERRUPT);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_eventEnable(CSL_SdioHandle       hSdio,
                            CSL_SdioEventType    sdioEvent)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(NULL != hSdio)
    {
        switch (sdioEvent)
        {
            case CSL_SDIO_EDATDNE_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_EDATDNE, PERMIT);
                break;

            case CSL_SDIO_EBSYDNE_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_EBSYDNE, PERMIT);
                break;

            case CSL_SDIO_ERSPDNE_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ERSPDNE, PERMIT);
                break;

            case CSL_SDIO_ETOUTRD_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ETOUTRD, PERMIT);
                break;

            case CSL_SDIO_ETOUTRS_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ETOUTRS, PERMIT);
                break;

            case CSL_SDIO_ECRCWR_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ECRCWR, PERMIT);
                break;

            case CSL_SDIO_ECRCRD_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ECRCRD, PERMIT);
                break;

            case CSL_SDIO_ECRCRS_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ECRCRS, PERMIT);
                break;

            case CSL_SDIO_EDXRDY_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_EDXRDY, PERMIT);
                break;

            case CSL_SDIO_EDRRDY_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_EDRRDY, PERMIT);
                break;

            case CSL_SDIO_EDATED_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_EDATED, PERMIT);
                break;

            case CSL_SDIO_ETRNDNE_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ETRNDNE, PERMIT);
                break;

            case CSL_SDIO_MMCIM_ALL_INTERRUPT:
                hSdio->sdioRegs->MMCIM = CSL_SDIO_MMCIM_REG_MASK;
                break;

            case CSL_SDIO_READWAIT_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->SDIOIEN, MMCSD_SDIOIEN_RWSEN,
                          ENABLE);
                break;

            case CSL_SDIO_CARD_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->SDIOIEN, MMCSD_SDIOIEN_IOINTEN,
                          ENABLE);
                break;

            case CSL_SDIO_SDIOIEN_ALL_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->SDIOIEN, MMCSD_SDIOIEN_RWSEN,
                          ENABLE);
                CSL_FINST(hSdio->sdioRegs->SDIOIEN, MMCSD_SDIOIEN_IOINTEN,
                          ENABLE);
                break;

            default:
                status = CSL_ESYS_INVPARAMS;
                break;
        }
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_eventDisable
 *
 *   @b Description
 *   @n This function will disable the events in the MMC interrupt mask
 *      register and SDIO interrupt enable register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio         Handle to the SDIO.
            sdioEvent     Event type.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Disables the given event
 *
 *   @b Modifies
 *   @n MMC/SDIO Interrupt Mask Register
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_eventDisable(hSdio, CSL_SDIO_EDRRDY_INTERRUPT);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_eventDisable(CSL_SdioHandle       hSdio,
                             CSL_SdioEventType    sdioEvent)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(NULL != hSdio)
    {
        switch (sdioEvent)
        {
            case CSL_SDIO_EDATDNE_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_EDATDNE, PROHIBIT);
                break;

            case CSL_SDIO_EBSYDNE_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_EBSYDNE, PROHIBIT);
                break;

            case CSL_SDIO_ERSPDNE_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ERSPDNE, PROHIBIT);
                break;

            case CSL_SDIO_ETOUTRD_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ETOUTRD, PROHIBIT);
                break;

            case CSL_SDIO_ETOUTRS_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ETOUTRS, PROHIBIT);
                break;

            case CSL_SDIO_ECRCWR_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ECRCWR, PROHIBIT);
                break;

            case CSL_SDIO_ECRCRD_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ECRCRD, PROHIBIT);
                break;

            case CSL_SDIO_ECRCRS_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ECRCRS, PROHIBIT);
                break;

            case CSL_SDIO_EDXRDY_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_EDXRDY, PROHIBIT);
                break;

            case CSL_SDIO_EDRRDY_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_EDRRDY, PROHIBIT);
                break;

            case CSL_SDIO_EDATED_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_EDATED, PROHIBIT);
                break;

            case CSL_SDIO_ETRNDNE_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->MMCIM, MMCSD_MMCIM_ETRNDNE, PROHIBIT);
                break;

            case CSL_SDIO_MMCIM_ALL_INTERRUPT:
                hSdio->sdioRegs->MMCIM = ~(CSL_SDIO_MMCIM_REG_MASK);
                break;

            case CSL_SDIO_READWAIT_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->SDIOIEN, MMCSD_SDIOIEN_RWSEN,
                          DISABLE);
                break;

            case CSL_SDIO_CARD_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->SDIOIEN, MMCSD_SDIOIEN_IOINTEN,
                          DISABLE);
                break;

            case CSL_SDIO_SDIOIEN_ALL_INTERRUPT:
                CSL_FINST(hSdio->sdioRegs->SDIOIEN, MMCSD_SDIOIEN_RWSEN,
                          DISABLE);
                CSL_FINST(hSdio->sdioRegs->SDIOIEN, MMCSD_SDIOIEN_IOINTEN,
                          DISABLE);
                break;

            default:
                status = CSL_ESYS_INVPARAMS;
                break;
        }
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_readWaitEnable
 *
 *   @b Description
 *   @n This function is used to enable SDIO read-wait bit in SDIO control
 *      register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio             Handle to the SDIO.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Enables the sdio read-wait
 *
 *   @b Modifies
 *   @n SDIO control Register
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_readWaitEnable(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_readWaitEnable(CSL_SdioHandle    hSdio)
{
    CSL_Status    status;

    status = CSL_ESYS_BADHANDLE;

    if(NULL != hSdio)
    {
        status = CSL_SOK;

        CSL_FINST(hSdio->sdioRegs->SDIOCTL, MMCSD_SDIOCTL_RDWTRQ, ENABLE);
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_readWaitDisable
 *
 *   @b Description
 *   @n This function is used to disable SDIO read-wait bit in SDIO control
 *      register.
 *
 *   @b Arguments
 *   @verbatim
            hSdio             Handle to the SDIO.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Disables the sdio read-wait
 *
 *   @b Modifies
 *   @n SDIO control Register
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_readWaitDisable(hSdio);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_readWaitDisable(CSL_SdioHandle    hSdio)
{
    CSL_Status    status;

    status = CSL_ESYS_BADHANDLE;

    if(NULL != hSdio)
    {
        status = CSL_SOK;

        CSL_FINST(hSdio->sdioRegs->SDIOCTL, MMCSD_SDIOCTL_RDWTRQ, DISABLE);
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_readSingleByte
 *
 *   @b Description
 *   @n This function reads a single byte of data from the SDIO card.
 *      'readAddr' is address on the SDIO card from where data byte will be
 *      read. Command 52 (IO_RW_DIRECT Command) is used to read the data.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            readAddr       Location to read from.
            funNum         IO function number to read data
            pReadData      Pointer to populate data read
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Reads a byte of data from the SDIO card
 *
 *   @b Modifies
 *   @n Read data pointer will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint16                   rca;
            Uint32                   ocr;
            Uint8                    readData;
            Uint32                   readAddr;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            readAddr  = 0x0;
            funNum    = 0;

            status = SDIO_readSingleByte(hSdio, readAddr, funNum, &readData);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_readSingleByte(CSL_SdioHandle    hSdio,
                               Uint32            readAddr,
                               Uint8             funNum,
                               Uint8             *pReadData)
{
    Uint32             argument;
    Uint32             rawFlag;
    Uint32             rwFlag;
    CSL_Status         status;
    Uint16             response;
    volatile Uint16    statusReg;

    argument    = 0;
    rawFlag     = 0;
    rwFlag      = 0;
    status      = CSL_ESYS_BADHANDLE;
    response    = 0;
    statusReg   = 0;

    if(NULL != hSdio)
    {
        if((funNum <= CSL_SDIO_MAX_FUNCTION_COUNT) && (NULL != pReadData))
        {
            argument = 0;
            argument = CSL_SDIO_MAKE_CMD52_READ_ARG(readAddr, rawFlag,
                                                    funNum, rwFlag);

            CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);
            hSdio->sdioRegs->MMCRSP6 = CSL_MMCSD_MMCRSP6_RESETVAL;

            CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     (argument & CSL_SDIO_MASK_WORD));
            CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_SDIO_RSHIFT(argument, CSL_SDIO_SHIFT_WORD));

            hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_52;

            do
            {
                statusReg = hSdio->sdioRegs->MMCST0;
                if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_ESDIO_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_ESDIO_CRCERR);
                    }
                }

            } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

            response = hSdio->sdioRegs->MMCRSP6;
            *pReadData = response & CSL_SDIO_MASK_BYTE;

            status = CSL_SOK;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_writeSingleByte
 *
 *   @b Description
 *   @n This function writes a single byte of data to the SDIO card.
 *      'writeAddr' is address on the SDIO card to where data byte will be
 *      written. Command 52 (IO_RW_DIRECT Command) is used to write the data.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            writeAddr      Location to write data byte.
            funNum         IO function number to read data
            writeData      Data to be written
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Writes a byte of data to the SDIO card
 *
 *   @b Modifies
 *   @n SDIO card registers
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint16                   rca;
            Uint32                   ocr;
            Uint8                    writeData;
            Uint32                   writeAddr;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            writeAddr  = 0x02;
            writeData  = 0x02;
            funNum     = 0;

            status = SDIO_writeSingleByte(hSdio, writeAddr, funNum, writeData);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_writeSingleByte(CSL_SdioHandle    hSdio,
                                Uint32            writeAddr,
                                Uint8             funNum,
                                Uint8             writeData)
{
    Uint32             argument;
    Uint32             rawFlag;
    Uint32             rwFlag;
    CSL_Status         status;
    volatile Uint16    statusReg;

    argument   = 0;
    rawFlag    = 0;
    rwFlag     = 0x1;
    status     = CSL_ESYS_BADHANDLE;
    statusReg  = 0;

    if(NULL != hSdio)
    {
        if(funNum <= CSL_SDIO_MAX_FUNCTION_COUNT)
        {
            argument = 0;
            argument = CSL_SDIO_MAKE_CMD52_WRITE_ARG(writeData, writeAddr,
                                                     rawFlag, funNum, rwFlag);

            CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);

            CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     (argument & CSL_SDIO_MASK_WORD));
            CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_SDIO_RSHIFT(argument, CSL_SDIO_SHIFT_WORD));

            hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_52;

            do
            {
                statusReg = hSdio->sdioRegs->MMCST0;
                if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_ESDIO_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_ESDIO_CRCERR);
                    }
                }

            } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

            status = CSL_SOK;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_readBytes
 *
 *   @b Description
 *   @n This function reads requested number of bytes from the SDIO card.
 *      'readAddr' is address on the SDIO card from where the reading
 *      starts. This function reads multiple bytes of data from the given
 *      address and populates into read buffer. Maximum number of bytes
 *      that can be read using this function are 512.
 *
 *      Reading of data can be done from a fixed address(opCode = 0) or
 *      incrementing address(opCode = 1). Fixed mode data read is useful
 *      when I/O data is transferred using a FIFO inside SDIO card. Opcode
 *      should be 1 for the SDIO cards which doesn't have support for FIFO.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'readEndianMode' in the SDIO handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during SDIO open. Endian mode can
 *      be configured using SDIO_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            readAddr       Location to read from.
            funNum         IO function number to read data
            opCode         Bit to decide source address mode
                             - Fixed/Incrementing
            noOfBytes      no of bytes to read.
            pReadBuf       pointer to a buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Reads data from the SDIO card
 *
 *   @b Modifies
 *   @n readBuffer will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint16                   rca;
            Uint32                   ocr;
            Uint16                   readBuf[512];
            Uint32                   readAddr;
            Uint16                   noOfBytes;
            Bool                     opCode;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            readAddr  = 0x0;
            noOfBytes = 16;
            opCode    = 1;
            funNum    = 0;

            status = SDIO_readBytes(hSdio, readAddr, funNum, opCode,
                                    noOfBytes, readBuf);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_readBytes(CSL_SdioHandle    hSdio,
                          Uint32            readAddr,
                          Uint8             funNum,
                          Bool              opCode,
                          Uint16            noOfBytes,
                          Uint16            *pReadBuf)
{
    Uint32        argument;
    CSL_Status    status;
    Uint16        byteCount;
    Uint16        count;
    Uint16        bytesRemaining;
    Uint16        dataByte;
    volatile ioport Uint16 *pReadReg;
    volatile Uint16 statusReg;
    Bool          blkMode;
    Bool          rwFlag;
    Bool          byteMode;

    argument       = 0;
    status         = CSL_ESYS_BADHANDLE;
    byteCount      = 0;
    count          = 0;
    bytesRemaining = 0;
    dataByte       = 0;
    pReadReg       = NULL;
    statusReg      = 0;
    blkMode        = 0;
    rwFlag         = 0;
    byteMode       = 0;

    if(NULL != hSdio)
    {
        if((NULL != pReadBuf) &&
           ((noOfBytes > 0) && (noOfBytes <= CSL_SDIO_MAX_BYTE_COUNT)) &&
           (funNum <= CSL_SDIO_MAX_FUNCTION_COUNT))
        {
            bytesRemaining = noOfBytes;

            /* Set Endian mode */
            CSL_FINS (hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_PERMDR,
                      hSdio->readEndianMode);

            /* Set block count and block length */
            CSL_FINS(hSdio->sdioRegs->MMCNBLK, MMCSD_MMCNBLK_NBLK, 1);

            if(noOfBytes > CSL_SDIO_READ_FIFO_LEVEL)
            {
                CSL_FINS(hSdio->sdioRegs->MMCBLEN, MMCSD_MMCBLEN_BLEN, noOfBytes);
            }
            else
            {
                CSL_FINS(hSdio->sdioRegs->MMCBLEN, MMCSD_MMCBLEN_BLEN,
                         CSL_SDIO_READ_FIFO_LEVEL);
            }

            /* Reset FIFO */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFORST, RESET);

            /* Configure fifo for read */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFODIR, READ);

            /* Set FIFO access width */
            if(0 == noOfBytes%2)
            {
                CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 2BYTES);
                byteMode = 0;
            }
            else
            {
                CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 1BYTE);
                byteMode = 1;
            }

            /* Set FIFO threshold */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFOLEV, 256BIT);

            if(hSdio->readEndianMode == CSL_SDIO_ENDIAN_LITTLE)
            {
                pReadReg = &hSdio->sdioRegs->MMCDRR1;
            }
            else
            {
                pReadReg = &hSdio->sdioRegs->MMCDRR2;
            }

            /* Send CMD 53 for reading */
            blkMode = 0;
            rwFlag  = 0;

            if(noOfBytes == CSL_SDIO_MAX_BYTE_COUNT)
            {
                byteCount = 0;
            }
            else
            {
                byteCount = noOfBytes;
            }

            argument = 0;
            argument = CSL_SDIO_MAKE_CMD53_ARG(byteCount, readAddr, opCode,
                                               blkMode, funNum, rwFlag);

            CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);

            CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     (argument & CSL_SDIO_MASK_WORD));
            CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_SDIO_RSHIFT(argument, CSL_SDIO_SHIFT_WORD));

            hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_53_READ;

            do
            {
                statusReg = hSdio->sdioRegs->MMCST0;
                if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_ESDIO_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_ESDIO_CRCERR);
                    }
                }

            } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

            if(noOfBytes >= CSL_SDIO_READ_FIFO_LEVEL)
            {
                /*
                 * When read data length is multiple of FIFO level all the
                 * data will be read in this loop. Otherwise this loop will
                 * break after reading (N*CSL_SDIO_READ_FIFO_LEVEL) bytes.
                 * Where N = noOfBytes/(CSL_SDIO_READ_FIFO_LEVEL);
                 */
                do
                {
                    do
                    {
                        statusReg = hSdio->sdioRegs->MMCST0;
                        if((statusReg & CSL_SDIO_DATA_TOUT_CRC_ERROR) != 0)
                        {
                            if(statusReg & CSL_SDIO_EVENT_ERROR_DATATIMEOUT)
                            {
                                return (CSL_ESDIO_TIMEOUT);
                            }
                            else
                            {
                                return (CSL_ESDIO_CRCERR);
                            }
                        }

                    } while((statusReg & CSL_SDIO_READ_READY) !=
                             CSL_SDIO_READ_READY);

                    if(1 == byteMode)
                    {
                        for (count = 0; count < CSL_SDIO_READ_FIFO_LEVEL/2; count++)
                        {
                            *pReadBuf = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1) &
                                                 CSL_SDIO_MASK_BYTE;

                            dataByte = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1) &
                                                CSL_SDIO_MASK_BYTE;

                            *pReadBuf |= dataByte << CSL_SDIO_SHIFT_BYTE;

                            bytesRemaining -= 2;
                            pReadBuf++;
                        }
                    }
                    else
                    {
                        for (count = 0; count < CSL_SDIO_READ_FIFO_LEVEL/2; count++)
                        {
                            *pReadBuf++ = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1);

                            bytesRemaining -= 2;
                        }
                    }

                    if(bytesRemaining < CSL_SDIO_READ_FIFO_LEVEL)
                    {
                        break;
                    }

                } while (0 != bytesRemaining);
            }

            /*
             * This condition will become true when the read data length is not a
             * multiple of FIFO level. Data of length
             * (noOfBytes - N*CSL_SDIO_READ_FIFO_LEVEL) bytes will be read here.
             * Where N = noOfBytes/(CSL_SDIO_READ_FIFO_LEVEL);
             */
            if(0 != bytesRemaining)
            {
                /*
                 * No data is read till now. So need to check the data receive
                 * ready bit.
                 */
                if(bytesRemaining == noOfBytes)
                {
                    do
                    {
                        statusReg = hSdio->sdioRegs->MMCST0;
                        if((statusReg & CSL_SDIO_DATA_TOUT_CRC_ERROR) != 0)
                        {
                            if(statusReg & CSL_SDIO_EVENT_ERROR_DATATIMEOUT)
                            {
                                return (CSL_ESDIO_TIMEOUT);
                            }
                            else
                            {
                                return (CSL_ESDIO_CRCERR);
                            }
                        }

                    } while((statusReg & CSL_SDIO_READ_READY) !=
                             CSL_SDIO_READ_READY);
                }

                if(1 == byteMode)
                {
                    /* This loop will read the even number of bytes */
                    for (count = 0; count < bytesRemaining/2; count++)
                    {
                        *pReadBuf = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1) &
                                             CSL_SDIO_MASK_BYTE;

                        dataByte = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1) &
                                            CSL_SDIO_MASK_BYTE;

                        *pReadBuf |= dataByte << CSL_SDIO_SHIFT_BYTE;

                        pReadBuf++;
                    }

                    /* Read the last byte here */
                    *pReadBuf = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1) &
                                         CSL_SDIO_MASK_BYTE;
                }
                else
                {
                    for (count = 0; count < bytesRemaining/2; count++)
                    {
                        *pReadBuf++ = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1);
                    }
                }

                bytesRemaining = 0;
            }

            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFORST, RESET);
            status = CSL_SOK;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_writeBytes
 *
 *   @b Description
 *   @n This function writes requested number of bytes to the SDIO card.
 *      'writeAddr' is address on the SDIO card from where the writing
 *      starts. Maximum length of the data that can be written using
 *      this function is 512 bytes.
 *
 *      Writing of data can be done to a fixed address(opCode = 0) or
 *      incrementing address(opCode = 1). Fixed mode data write is useful
 *      when I/O data is transferred using a FIFO inside SDIO card. Opcode
 *      should be 1 for the SDIO cards which doesn't have support for FIFO.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'writeEndianMode' in the SDIO handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during SDIO open. Endian mode can
 *      be configured using SDIO_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            writeAddr      Location to write to.
            funNum         IO function number to write data
            opCode         Bit to decide destination address mode
                             - Fixed/Incrementing
            noOfBytes      Number of bytes to write.
            pWriteBuf      Pointer to write buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Writes data to the SDIO card
 *
 *   @b Modifies
 *   @n Data registers
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   ocr;
            Uint16                   writeBuf[512];
            Uint32                   writeAddr;
            Uint16                   noOfBytes;
            Bool                     opCode;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            writeAddr  = 0x07;
            noOfBytes  = 1;
            opCode     = 1;
            funNum     = 0;

            status = SDIO_writeBytes(hSdio, writeAddr, funNum, opCode,
                                     noOfBytes, writeBuf);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_writeBytes(CSL_SdioHandle    hSdio,
                           Uint32            writeAddr,
                           Uint8             funNum,
                           Bool              opCode,
                           Uint16            noOfBytes,
                           Uint16            *pWriteBuf)
{
    Uint32        argument;
    CSL_Status    status;
    Uint16        byteCount;
    Uint16        count;
    Uint16        bytesRemaining;
    volatile ioport Uint16 *pWriteReg;
    volatile Uint16 statusReg;
    Bool          blkMode;
    Bool          rwFlag;
    Bool          byteMode;

    argument       = 0;
    status         = CSL_ESYS_BADHANDLE;
    byteCount      = 0;
    count          = 0;
    bytesRemaining = 0;
    pWriteReg      = 0;
    statusReg      = 0;
    blkMode        = 0;
    rwFlag         = 0;
    byteMode       = 0;

    if(NULL != hSdio)
    {
        if((NULL != pWriteBuf) &&
           ((noOfBytes > 0) && (noOfBytes <= CSL_SDIO_MAX_BYTE_COUNT)) &&
           (funNum <= CSL_SDIO_MAX_FUNCTION_COUNT))
        {
            bytesRemaining = noOfBytes;

            /* Set Endian mode */
            CSL_FINS (hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_PERMDR,
                      hSdio->writeEndianMode);

            /* Set block count and block length */
            CSL_FINS(hSdio->sdioRegs->MMCNBLK, MMCSD_MMCNBLK_NBLK, 1);
            CSL_FINS(hSdio->sdioRegs->MMCBLEN, MMCSD_MMCBLEN_BLEN, noOfBytes);

            /* Reset FIFO */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFORST, RESET);
            /* Configure fifo for write */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFODIR, WRITE);

            /* Set FIFO access width */
            if(0 == noOfBytes%2)
            {
                CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 2BYTES);
            }
            else
            {
                CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 1BYTE);
                byteMode = 1;
            }

            /* Set FIFO threshold */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFOLEV, 256BIT);

            if(hSdio->readEndianMode == CSL_SDIO_ENDIAN_LITTLE)
            {
                pWriteReg = &hSdio->sdioRegs->MMCDXR1;
            }
            else
            {
                pWriteReg = &hSdio->sdioRegs->MMCDXR2;
            }

            /* Send CMD 53 */
            blkMode = 0;
            rwFlag  = 1;

            if(noOfBytes == CSL_SDIO_MAX_BYTE_COUNT)
            {
                byteCount = 0;
            }
            else
            {
                byteCount = noOfBytes;
            }

            argument = 0;
            argument = CSL_SDIO_MAKE_CMD53_ARG(byteCount, writeAddr, opCode,
                                               blkMode, funNum, rwFlag);

            CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);

            CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     (argument & CSL_SDIO_MASK_WORD));
            CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_SDIO_RSHIFT(argument, CSL_SDIO_SHIFT_WORD));

            hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_53_WRITE;

            CSL_FINS(hSdio->sdioRegs->MMCCMD2, MMCSD_MMCCMD2_DMATRIG, 1);

            if(noOfBytes >= CSL_SDIO_WRITE_FIFO_LEVEL)
            {
                /*
                 * When write data length is multiple of FIFO level all the
                 * data will be written in this loop. Otherwise this loop will
                 * break after writing (N*CSL_SDIO_WRITE_FIFO_LEVEL) bytes.
                 * Where N = noOfBytes/(CSL_SDIO_WRITE_FIFO_LEVEL);
                 */
                do
                {
                    do
                    {
                        statusReg = hSdio->sdioRegs->MMCST0;
                        if((statusReg & CSL_SDIO_EVENT_ERROR) != 0)
                        {
                            if(statusReg & CSL_SDIO_EVENT_ERROR_DATACRC)
                            {
                                return (CSL_ESDIO_CRCERR);
                            }
                            else
                            {
                                return (CSL_ESDIO_TIMEOUT);
                            }
                        }

                    } while((statusReg & CSL_SDIO_WRITE_READY) !=
                             CSL_SDIO_WRITE_READY);

                    if(1 == byteMode)
                    {
                        for (count = 0; count < CSL_SDIO_WRITE_FIFO_LEVEL/2; count++)
                        {
                            CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1,
                                     (*pWriteBuf & CSL_SDIO_MASK_BYTE));

                            CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1,
                                     ((*pWriteBuf >> CSL_SDIO_SHIFT_BYTE) &
                                       CSL_SDIO_MASK_BYTE));

                            bytesRemaining -= 2;
                            pWriteBuf++;
                        }
                    }
                    else
                    {
                        for (count = 0; count < CSL_SDIO_WRITE_FIFO_LEVEL/2; count++)
                        {
                            CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1, *pWriteBuf++);

                            bytesRemaining -= 2;
                        }
                    }

                    if(bytesRemaining < CSL_SDIO_WRITE_FIFO_LEVEL)
                    {
                        break;
                    }

                } while (0 != bytesRemaining);
            }

            /*
             * This condition will become true when the write data length is not a
             * multiple of FIFO level. Data of length
             * (noOfBytes - N*CSL_SDIO_WRITE_FIFO_LEVEL) bytes will be written here.
             * Where N = noOfBytes/(CSL_SDIO_WRITE_FIFO_LEVEL);
             */
            if(0 != bytesRemaining)
            {
                if(bytesRemaining == noOfBytes)
                {
                    do
                    {
                        statusReg = hSdio->sdioRegs->MMCST0;
                        if((statusReg & CSL_SDIO_EVENT_ERROR) != 0)
                        {
                            if(statusReg & CSL_SDIO_EVENT_ERROR_DATACRC)
                            {
                                return (CSL_ESDIO_CRCERR);
                            }
                            else
                            {
                                return (CSL_ESDIO_TIMEOUT);
                            }
                        }
                    } while((statusReg & CSL_SDIO_WRITE_READY) !=
                             CSL_SDIO_WRITE_READY);
                }

                if(1 == byteMode)
                {
                    /* This loop writes even number of bytes */
                    for (count = 0; count < bytesRemaining/2; count++)
                    {
                        CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1,
                                 (*pWriteBuf & CSL_SDIO_MASK_BYTE));

                        CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1,
                                 ((*pWriteBuf >> CSL_SDIO_SHIFT_BYTE) &
                                   CSL_SDIO_MASK_BYTE));

                        pWriteBuf++;
                    }

                    /* Write the last byte here */
                    CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1,
                             (*pWriteBuf & CSL_SDIO_MASK_BYTE));
                }
                else
                {
                    for (count = 0; count < bytesRemaining/2; count++)
                    {
                        CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1, *pWriteBuf++);
                    }
                }

                bytesRemaining = 0;
            }

            /* Wait for the Data done */
            do
            {
                statusReg = hSdio->sdioRegs->MMCST0;
                if((statusReg & CSL_SDIO_EVENT_ERROR) != 0)
                {
                    if(statusReg & CSL_SDIO_EVENT_ERROR_DATACRC)
                    {
                        return (CSL_ESDIO_CRCERR);
                    }
                    else
                    {
                        return (CSL_ESDIO_TIMEOUT);
                    }
                }
            } while((statusReg & CSL_SDIO_EVENT_BLOCK_XFERRED) !=
                     CSL_SDIO_EVENT_BLOCK_XFERRED);

            status = CSL_SOK;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_readBlocks
 *
 *   @b Description
 *   @n This function reads requested number of blocks of data from SDIO card.
 *      'readAddr' is address on the SDIO card from where the reading
 *      starts. Block size of IO function to read data should be configured
 *      before calling this function.
 *
 *      Reading of data can be done from a fixed address(opCode = 0) or
 *      incrementing address(opCode = 1). Fixed mode data read is useful
 *      when I/O data is transferred using a FIFO inside SDIO card. Opcode
 *      should be 1 for the SDIO cards which doesn't have support for FIFO.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'readEndianMode' in the SDIO handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during SDIO open. Endian mode can
 *      be configured using SDIO_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            readAddr       Location to read from.
            funNum         IO function number to read data
            opCode         Bit to decide source address mode
                           - Fixed/Incrementing
            noOfBlocks     Number of blocks to read.
            blockSize      Size of each block
            pReadBuf       pointer to a buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Reads data from the SDIO card
 *
 *   @b Modifies
 *   @n readBuffer will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   ocr;
            Uint16                   readBuf[512];
            Uint32                   readAddr;
            Uint16                   noOfBlocks;
            Bool                     opCode;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            readAddr   = 0x0;
            noOfBlocks = 2;
            opCode     = 1;
            funNum     = 0;

            status = SDIO_readBlocks(hSdio, readAddr, funNum, opCode,
                                     noOfBlocks, 256, readBuf);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_readBlocks(CSL_SdioHandle    hSdio,
                           Uint32            readAddr,
                           Uint8             funNum,
                           Bool              opCode,
                           Uint16            noOfBlocks,
                           Uint16            blockSize,
                           Uint16            *pReadBuf)
{
    Uint32        bytesRemaining;
    Uint32        noOfBytes;
    Uint32        argument;
    CSL_Status    status;
    Uint16        count;
    Uint16        dataByte;
    volatile ioport Uint16 *pReadReg;
    volatile Uint16 statusReg;
    Bool          blkMode;
    Bool          rwFlag;
    Bool          byteMode;

    bytesRemaining = 0;
    noOfBytes      = 0;
    argument       = 0;
    status         = CSL_ESYS_BADHANDLE;
    count          = 0;
    dataByte       = 0;
    pReadReg       = NULL;
    statusReg      = 0;
    blkMode        = 0;
    rwFlag         = 0;
    byteMode       = 0;

    if(NULL != hSdio)
    {
        if((NULL != pReadBuf) &&
           (noOfBlocks <= CSL_SDIO_MAX_BLOCK_COUNT) &&
           ((blockSize > 0) && (blockSize <= CSL_SDIO_MAX_BLOCK_SIZE)) &&
           (funNum <= CSL_SDIO_MAX_FUNCTION_COUNT))
        {
            noOfBytes = (noOfBlocks * blockSize);
            bytesRemaining = noOfBytes;

            /* Set Endian mode */
            CSL_FINS (hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_PERMDR,
                      hSdio->readEndianMode);

            /* Set block length */
            CSL_FINS(hSdio->sdioRegs->MMCBLEN, MMCSD_MMCBLEN_BLEN, blockSize);
            CSL_FINS(hSdio->sdioRegs->MMCNBLK, MMCSD_MMCNBLK_NBLK, noOfBlocks);

            /* Reset FIFO */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFORST, RESET);
            /* Configure fifo for read */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFODIR, READ);

            /* Set FIFO access width */
            if(0 == noOfBytes%2)
            {
                CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 2BYTES);
            }
            else
            {
                CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 1BYTE);
                byteMode = 1;
            }

            /* Set FIFO threshold */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFOLEV, 256BIT);

            if(hSdio->readEndianMode == CSL_SDIO_ENDIAN_LITTLE)
            {
                pReadReg = &hSdio->sdioRegs->MMCDRR1;
            }
            else
            {
                pReadReg = &hSdio->sdioRegs->MMCDRR2;
            }

            /* Send CMD 53 */
            blkMode   = 1;
            rwFlag    = 0;

            argument = 0;
            argument = CSL_SDIO_MAKE_CMD53_ARG(noOfBlocks, readAddr, opCode,
                                               blkMode, funNum, rwFlag);

            CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);

            CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     (argument & CSL_SDIO_MASK_WORD));
            CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_SDIO_RSHIFT(argument, CSL_SDIO_SHIFT_WORD));

            hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_53_READ;

            do
            {
                statusReg = hSdio->sdioRegs->MMCST0;
                if((statusReg & CSL_SDIO_CMD_TOUT_CRC_ERROR) != 0)
                {
                    if(statusReg & CSL_SDIO_EVENT_ERROR_CMDTIMEOUT)
                    {
                        return (CSL_ESDIO_TIMEOUT);
                    }
                    else
                    {
                        return (CSL_ESDIO_CRCERR);
                    }
                }

            } while((statusReg & CSL_SDIO_EVENT_EOFCMD) == 0);

            if(noOfBytes >= CSL_SDIO_READ_FIFO_LEVEL)
            {
                /*
                 * When read data length is multiple of FIFO level all the
                 * data will be read in this loop. Otherwise this loop will
                 * break after reading (N*CSL_SDIO_READ_FIFO_LEVEL) bytes.
                 * Where N = noOfBytes/(CSL_SDIO_READ_FIFO_LEVEL);
                 */
                do
                {
                    do
                    {
                        statusReg = hSdio->sdioRegs->MMCST0;
                        if((statusReg & CSL_SDIO_DATA_TOUT_CRC_ERROR) != 0)
                        {
                            if(statusReg & CSL_SDIO_EVENT_ERROR_DATATIMEOUT)
                            {
                                return (CSL_ESDIO_TIMEOUT);
                            }
                            else
                            {
                                return (CSL_ESDIO_CRCERR);
                            }
                        }

                    } while((statusReg & CSL_SDIO_READ_READY) !=
                             CSL_SDIO_READ_READY);

                    if(1 == byteMode)
                    {
                        for (count = 0; count < CSL_SDIO_READ_FIFO_LEVEL/2; count++)
                        {
                            *pReadBuf = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1) &
                                                 CSL_SDIO_MASK_BYTE;

                            dataByte = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1) &
                                                CSL_SDIO_MASK_BYTE;

                            *pReadBuf |= dataByte << CSL_SDIO_SHIFT_BYTE;

                            bytesRemaining -= 2;
                            pReadBuf++;
                        }
                    }
                    else
                    {
                        for (count = 0; count < CSL_SDIO_READ_FIFO_LEVEL/2; count++)
                        {
                            *pReadBuf++ = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1);

                            bytesRemaining -= 2;
                        }
                    }

                    if(bytesRemaining < CSL_SDIO_READ_FIFO_LEVEL)
                    {
                        break;
                    }

                } while (0 != bytesRemaining);
            }

            /*
             * This condition will become true when the read data length is not a
             * multiple of FIFO level. Data of length
             * (noOfBytes - N*CSL_SDIO_READ_FIFO_LEVEL) bytes will be read here.
             * Where N = noOfBytes/(CSL_SDIO_READ_FIFO_LEVEL);
             */
            if(0 != bytesRemaining)
            {
                if(bytesRemaining == noOfBytes)
                {
                    do
                    {
                        statusReg = hSdio->sdioRegs->MMCST0;
                        if((statusReg & CSL_SDIO_DATA_TOUT_CRC_ERROR) != 0)
                        {
                            if(statusReg & CSL_SDIO_EVENT_ERROR_DATATIMEOUT)
                            {
                                return (CSL_ESDIO_TIMEOUT);
                            }
                            else
                            {
                                return (CSL_ESDIO_CRCERR);
                            }
                        }

                    } while((statusReg & CSL_SDIO_READ_READY) != CSL_SDIO_READ_READY);
                }

                if(1 == byteMode)
                {
                    /* This loop will read the even number of bytes */
                    for (count = 0; count < bytesRemaining/2; count++)
                    {
                        *pReadBuf = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1) &
                                             CSL_SDIO_MASK_BYTE;

                        dataByte = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1) &
                                            CSL_SDIO_MASK_BYTE;

                        *pReadBuf |= dataByte << CSL_SDIO_SHIFT_BYTE;

                        pReadBuf++;
                    }

                    /* Read the last byte here */
                    *pReadBuf = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1) &
                                         CSL_SDIO_MASK_BYTE;
                }
                else
                {
                    for (count = 0; count < bytesRemaining/2; count++)
                    {
                        *pReadBuf++ = CSL_FEXT(*pReadReg, MMCSD_MMCDRR1_DRR1);
                    }
                }

                bytesRemaining = 0;
            }

            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFORST, RESET);
            status = CSL_SOK;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_writeBlocks
 *
 *   @b Description
 *   @n This function writes requested number of blocks of data to SDIO card.
 *      'writeAddr' is address on the SDIO card from where the writing
 *       starts. Block size of IO function to write data should be configured
 *      before calling this function.
 *
 *      Writing of data can be done to a fixed address(opCode = 0) or
 *      incrementing address(opCode = 1). Fixed mode data write is useful
 *      when I/O data is transferred using a FIFO inside SDIO card. Opcode
 *      should be 1 for the SDIO cards which doesn't have support for FIFO.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'writeEndianMode' in the SDIO handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during SDIO open. Endian mode can
 *      be configured using SDIO_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *   @b Arguments
 *   @verbatim
            hSdio          Handle to the SDIO.
            writeAddr      Location to write to.
            funNum         IO function number to write data
            opCode         Bit to decide destination address mode
                             - Fixed/Incrementing
            noOfBlocks     Number of blocks to write.
            blockSize      Size of each block
            pWriteBuf      Pointer to write buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_ESDIO_TIMEOUT   - Card Response time out
 *   @li                    CSL_ESDIO_CRCERR    - Response/Data CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:SDIO_open() API should be called successfully.
 *   @n  2:SDIO_initCard() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Writes data to the SDIO card
 *
 *   @b Modifies
 *   @n Data registers
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;
            Uint32                   ocr;
            Uint16                   writeBuf[512];
            Uint32                   writeAddr;
            Uint16                   noOfBlocks;
            Bool                     opCode;
            Uint8                    funNum;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_detectCard(hSdio, &sdioCardObj);

            status = SDIO_initCard(hSdio, ocr, TRUE);

            writeAddr  = 0x110;
            noOfBlocks = 2;
            opCode     = 1;
            funNum     = 0;

            status = SDIO_writeBlocks(hSdio, writeAddr, funNum, opCode,
                                      noOfBlocks, 256, writeBuf);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_writeBlocks(CSL_SdioHandle    hSdio,
                            Uint32            writeAddr,
                            Uint8             funNum,
                            Bool              opCode,
                            Uint16            noOfBlocks,
                            Uint16            blockSize,
                            Uint16            *pWriteBuf)
{
    Uint32        bytesRemaining;
    Uint32        noOfBytes;
    Uint32        argument;
    CSL_Status    status;
    Uint16        count;
    volatile ioport Uint16 *pWriteReg;
    volatile Uint16 statusReg;
    Bool          blkMode;
    Bool          rwFlag;
    Bool          byteMode;

    bytesRemaining = 0;
    noOfBytes      = 0;
    argument       = 0;
    status         = CSL_ESYS_BADHANDLE;
    count          = 0;
    pWriteReg      = NULL;
    statusReg      = 0;
    blkMode        = 0;
    rwFlag         = 0;
    byteMode       = 0;

    if(NULL != hSdio)
    {
        if((NULL != pWriteBuf) &&
           (noOfBlocks <= CSL_SDIO_MAX_BLOCK_COUNT) &&
           ((blockSize > 0) && (blockSize <= CSL_SDIO_MAX_BLOCK_SIZE)) &&
           (funNum <= CSL_SDIO_MAX_FUNCTION_COUNT))
        {
            noOfBytes      = (noOfBlocks * blockSize);
            bytesRemaining = noOfBytes;

            /* Set Endian mode */
            CSL_FINS (hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_PERMDR,
                      hSdio->writeEndianMode);

            /* Set block length */
            CSL_FINS(hSdio->sdioRegs->MMCBLEN, MMCSD_MMCBLEN_BLEN, blockSize);
            CSL_FINS(hSdio->sdioRegs->MMCNBLK, MMCSD_MMCNBLK_NBLK, noOfBlocks);

            /* Reset FIFO */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFORST, RESET);
            /* Configure fifo for read */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFODIR, WRITE);

            /* Set FIFO access width */
            if(0 == noOfBytes%2)
            {
                CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 2BYTES);
            }
            else
            {
                CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_ACCWD, 1BYTE);
                byteMode = 1;
            }

            /* Set FIFO threshold */
            CSL_FINST(hSdio->sdioRegs->MMCFIFOCTL, MMCSD_MMCFIFOCTL_FIFOLEV, 256BIT);

            if(hSdio->readEndianMode == CSL_SDIO_ENDIAN_LITTLE)
            {
                pWriteReg = &hSdio->sdioRegs->MMCDXR1;
            }
            else
            {
                pWriteReg = &hSdio->sdioRegs->MMCDXR2;
            }

            /* Send CMD 53 */
            blkMode   = 1;
            rwFlag    = 1;

            argument = 0;
            argument = CSL_SDIO_MAKE_CMD53_ARG(noOfBlocks, writeAddr, opCode,
                                               blkMode, funNum, rwFlag);

            CSL_FINS(hSdio->sdioRegs->MMCCIDX, MMCSD_MMCCIDX_CIDX, 0);

            CSL_FINS(hSdio->sdioRegs->MMCARG1, MMCSD_MMCARG1_ARG1,
                     (argument & CSL_SDIO_MASK_WORD));
            CSL_FINS(hSdio->sdioRegs->MMCARG2, MMCSD_MMCARG2_ARG2,
                     CSL_SDIO_RSHIFT(argument, CSL_SDIO_SHIFT_WORD));

            hSdio->sdioRegs->MMCCMD1 = CSL_SDIO_CMD_53_WRITE;

            if(noOfBytes >= CSL_SDIO_WRITE_FIFO_LEVEL)
            {
                /*
                 * When write data length is multiple of FIFO level all the
                 * data will be written in this loop. Otherwise this loop will
                 * break after writing (N*CSL_SDIO_WRITE_FIFO_LEVEL) bytes.
                 * Where N = noOfBytes/(CSL_SDIO_WRITE_FIFO_LEVEL);
                 */
                do
                {
                    do
                    {
                        statusReg = hSdio->sdioRegs->MMCST0;
                        if((statusReg & CSL_SDIO_EVENT_ERROR) != 0)
                        {
                            if(statusReg & CSL_SDIO_EVENT_ERROR_DATACRC)
                            {
                                return (CSL_ESDIO_CRCERR);
                            }
                            else
                            {
                                return (CSL_ESDIO_TIMEOUT);
                            }
                        }

                    } while((statusReg & CSL_SDIO_WRITE_READY) !=
                             CSL_SDIO_WRITE_READY);

                    if(1 == byteMode)
                    {
                        for (count = 0; count < CSL_SDIO_WRITE_FIFO_LEVEL/2; count++)
                        {
                            CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1,
                                     (*pWriteBuf & CSL_SDIO_MASK_BYTE));

                            CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1,
                                     ((*pWriteBuf >> CSL_SDIO_SHIFT_BYTE) &
                                       CSL_SDIO_MASK_BYTE));

                            bytesRemaining -= 2;
                            pWriteBuf++;
                        }
                    }
                    else
                    {
                        for (count = 0; count < CSL_SDIO_WRITE_FIFO_LEVEL/2; count++)
                        {
                            CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1, *pWriteBuf++);

                            bytesRemaining -= 2;
                        }
                    }

                    if(bytesRemaining < CSL_SDIO_WRITE_FIFO_LEVEL)
                    {
                        break;
                    }

                } while (0 != bytesRemaining);
            }

            /*
             * This condition will become true when the write data length is not a
             * multiple of FIFO level. Data of length
             * (noOfBytes - N*CSL_SDIO_WRITE_FIFO_LEVEL) bytes will be written here.
             * Where N = noOfBytes/(CSL_SDIO_WRITE_FIFO_LEVEL);
             */
            if(0 != bytesRemaining)
            {
                if(bytesRemaining == noOfBytes)
                {
                    do
                    {
                        statusReg = hSdio->sdioRegs->MMCST0;
                        if((statusReg & CSL_SDIO_EVENT_ERROR) != 0)
                        {
                            if(statusReg & CSL_SDIO_EVENT_ERROR_DATACRC)
                            {
                                return (CSL_ESDIO_CRCERR);
                            }
                            else
                            {
                                return (CSL_ESDIO_TIMEOUT);
                            }
                        }

                    } while((statusReg & CSL_SDIO_WRITE_READY) !=
                             CSL_SDIO_WRITE_READY);
                }

                if(1 == byteMode)
                {
                    /* This loop writes even number of bytes */
                    for (count = 0; count < bytesRemaining/2; count++)
                    {
                        CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1,
                                 (*pWriteBuf & CSL_SDIO_MASK_BYTE));

                        CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1,
                                 ((*pWriteBuf >> CSL_SDIO_SHIFT_BYTE) &
                                   CSL_SDIO_MASK_BYTE));

                        pWriteBuf++;
                    }

                    /* Write the last byte here */
                    CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1,
                             (*pWriteBuf & CSL_SDIO_MASK_BYTE));
                }
                else
                {
                    for (count = 0; count < bytesRemaining/2; count++)
                    {
                        CSL_FINS(*pWriteReg, MMCSD_MMCDXR1_DXR1, *pWriteBuf++);
                    }
                }

                bytesRemaining = 0;
            }

            /* Wait for the Data done */
            do
            {
                statusReg = hSdio->sdioRegs->MMCST0;
                if((statusReg & CSL_SDIO_EVENT_ERROR) != 0)
                {
                    if(statusReg & CSL_SDIO_EVENT_ERROR_DATACRC)
                    {
                        return (CSL_ESDIO_CRCERR);
                    }
                    else
                    {
                        return (CSL_ESDIO_TIMEOUT);
                    }
                }
            } while((hSdio->sdioRegs->MMCST0 & CSL_SDIO_EVENT_BLOCK_XFERRED) !=
                     CSL_SDIO_EVENT_BLOCK_XFERRED);

            status = CSL_SOK;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b SDIO_setEndianMode
 *
 *   @b Description
 *   @n This function is used to set endian mode of the SDIO controller.
 *      Endian mode set using this API decides the way in which data is
 *      transferred from FIFO to storage device.
 *
 *      Default endianness of SDIO controller:
 *      Chip C5504 and C5505: LITTLE ENDIAN.
 *      Chip C5514 and C5515: BIG ENDIAN.
 *
 *   @b Arguments
 *   @verbatim
            hSdio              Handle to the SDIO.
            writeEndianMode    Endian mode for SDIO write operation
            readEndianMode     Endian mode for SDIO read operation
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n   Sets SDIO endian mode for write and read operations
 *
 *   @b Modifies
 *   @n SDIO control register
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_setEndianMode(hSdio, CSL_SDIO_ENDIAN_LITTLE,
                                        CSL_SDIO_ENDIAN_LITTLE);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_setEndianMode (CSL_SdioHandle        hSdio,
                               CSL_SdioEndianMode    writeEndianMode,
                               CSL_SdioEndianMode    readEndianMode)
{
    CSL_Status    status;

    status = CSL_ESYS_BADHANDLE;

    if(NULL != hSdio)
    {
        if(((writeEndianMode == CSL_SDIO_ENDIAN_LITTLE) ||
           (writeEndianMode == CSL_SDIO_ENDIAN_BIG))    &&
           ((readEndianMode == CSL_SDIO_ENDIAN_LITTLE)  ||
           (readEndianMode == CSL_SDIO_ENDIAN_BIG)))
        {
            hSdio->readEndianMode  = readEndianMode;
            hSdio->writeEndianMode = writeEndianMode;

            /* Set Endian mode for write operation */
            CSL_FINS (hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_PERMDX,
                      writeEndianMode);

            /* Set Endian mode for read operation */
            CSL_FINS (hSdio->sdioRegs->MMCCTL, MMCSD_MMCCTL_PERMDR,
                      readEndianMode);

            status = CSL_SOK;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }

    return(status);
}

/** ===========================================================================
 *   @n@b SDIO_setClock
 *
 *   @b Description
 *   @n This function is used to set SDIO controller clock divider value. Clock
 *      controller register bits are configured by this function. Clock rate or
 *      clock divider value can range from 0 to 0xFF.
 *
 *   @b Arguments
 *   @verbatim
            hSdio        Handle to the SDIO.
            clockRate    Value of clock rate
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  SDIO_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets SDIO clock
 *
 *   @b Modifies
 *   @n SDIO clock control register
 *
 *   @b Example
 *   @verbatim
            CSL_SdioControllerObj    sdioContObj;
            CSL_SdioHandle           hSdio;
            CSL_SdioInstId           instId;
            CSL_status               status;

            instId = CSL_SDIO_INST0;

            status = SDIO_init();

            hSdio  = SDIO_open(&sdioContObj, instId, &status);

            status = SDIO_setClock(hSdio, 5);
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SDIO_setClock (CSL_SdioHandle    hSdio,
                          Uint16            clockRate)
{
    CSL_Status    status;
    Bool          clkState;

    status = CSL_ESYS_BADHANDLE;

    if(NULL != hSdio)
    {
        if(clockRate <= CSL_SDIO_MAX_CLKRATE)
        {
            /* Read the clock state */
            clkState = CSL_FEXT(hSdio->sdioRegs->MMCCLK, MMCSD_MMCCLK_CLKEN);
            /* Clock should be disabled while setting the clock rate */
            CSL_FINST(hSdio->sdioRegs->MMCCLK, MMCSD_MMCCLK_CLKEN, DISABLE);
            /* Configure the clock rate */
            CSL_FINS(hSdio->sdioRegs->MMCCLK, MMCSD_MMCCLK_CLKRT, clockRate);
            /* Restore the clock state */
            CSL_FINS(hSdio->sdioRegs->MMCCLK, MMCSD_MMCCLK_CLKEN, clkState);

            status = CSL_SOK;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }

    return (status);
}

