/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_nand.c
 *
 *  @brief NAND functional layer API source file
 *
 *  Path: \(CSLPATH)\src\nand\src
 */

/* ============================================================================
 * Revision History
 * ================
 * 21-Aug-2008 Created
 * 07-Oct-2008 Modified According to new Register layer files
 * ============================================================================
 */

#include <csl_nand.h>

/** ============================================================================
 *   @n@b NAND_init
 *
 *   @b Description
 *   @n This is the initialization function for the nand CSL.
 *      It populates nand object structure information. This API should
 *      be called before calling any other nand CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            nandObj       Pointer to nand object structure
            nandInstId    Nand Hardware instance number
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand init is successful
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Nand object structure is populated
 *
 *   @b Modifies
 *   @n Nand object structure
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            CSL_Status     status;

            instId = 0;
            status = NAND_init(&nandObj, instId);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_init(CSL_NandObj    *nandObj,
                     Uint16         nandInstId)
{
    CSL_Status         result;
    volatile Uint16    looper;

    result = CSL_ESYS_INVPARAMS;

    if(nandObj != NULL)
    {
        nandObj->instId      = nandInstId;
        nandObj->sysCtrlRegs = CSL_SYSCTRL_REGS;

        /* Reset EMIF module */
        CSL_FINS(nandObj->sysCtrlRegs->PSRCR, SYS_PSRCR_COUNT,
                 CSL_NAND_RESET_COUNT_VAL);

        /* reset Emif Module */
        CSL_FINST(nandObj->sysCtrlRegs->PRCR, SYS_PRCR_PG1_RST, RST);

        /* Give some delay for the device to reset */
        for(looper = 0; looper < CSL_NAND_DELAY; looper++){;}

        /* Enable EMIF module in Idle PCGCR */
        CSL_FINST(nandObj->sysCtrlRegs->PCGCR1, SYS_PCGCR1_EMIFCG, ACTIVE);

        switch(nandObj->instId)
        {
            case CSL_NAND_INST_0:
                nandObj->regs = CSL_EMIF_REGS;
                result = CSL_SOK;
                break;

            default:
                break;
        }
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_setup
 *
 *   @b Description
 *   @n This API configures the nand hardware registers.
 *
 *   @b Arguments
 *   @verbatim
            hNand         Handle to the nand object
            nandConfig    Nand configuration structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand setup is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid config structure
 *
 *   <b> Pre Condition </b>
 *   @n  Nand init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Configures nand registers
 *
 *   @b Modifies
 *   @n nand registers
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj            nandObj;
            Uint16                 instId;
            CSL_Status             status;
            CSL_NandConfig         nandCfg;
            CSL_NandHandle         hNand;
            CSL_NandAsyncWaitCfg   asyncWaitConfig;
            CSL_NandAsyncCfg       asyncConfig;

            instId = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ...
            nandCfg.nandWidth = CSL_NAND_8_BIT;
            ...
            ...
            nandCfg.asyncWaitCfg = &asyncWaitConfig;
            nandCfg.asyncCfg     = &asyncConfig;
            status = NAND_setup(hNand, &nandCfg);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_setup(CSL_NandHandle      hNand,
                      CSL_NandConfig      *nandConfig)
{
    CSL_Status                result;
    volatile ioport Uint16    *asyncCfg0Addr;
    volatile ioport Uint16    *asyncCfg1Addr;

    result = CSL_SOK;

    if(hNand != NULL)
    {
        if(nandConfig != NULL)
        {
            hNand->nandWidth    = nandConfig->nandWidth;
            hNand->nandPageSize = nandConfig->nandPageSize;
            hNand->nandOpMode   = nandConfig->nandOpMode;
            hNand->nandType     = nandConfig->nandType;
            hNand->chipSelect   = nandConfig->chipSelect;

            /* Set EMIF access to 16 bit */
            CSL_FINST(hNand->sysCtrlRegs->ESCR, SYS_ESCR_BYTEMODE, FULL);

            /* Configure async wait config register */
            /* Set the chip select */
            switch(nandConfig->chipSelect)
            {
                case CSL_NAND_CS2:
                    /* CS2 */
                    CSL_FINS(hNand->regs->AWCCR2, EMIF_AWCCR2_CS2_WAIT,
                             nandConfig->asyncWaitCfg->nandPort);

                    /* Set the chip select in nand flash control register */
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS2_USE_NAND,
                              NAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS3_USE_NAND,
                              NONAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS4_USE_NAND,
                             NONAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS5_USE_NAND,
                              NONAND);

                    asyncCfg0Addr = &hNand->regs->ACS2CR1;
                    asyncCfg1Addr = &hNand->regs->ACS2CR2;

                    break;

                case CSL_NAND_CS3:
                    /* CS3 */
                    CSL_FINS(hNand->regs->AWCCR2, EMIF_AWCCR2_CS3_WAIT,
                             nandConfig->asyncWaitCfg->nandPort);

                    /* Set the chip select in nand flash control register */
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS2_USE_NAND,
                              NONAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS3_USE_NAND,
                              NAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS4_USE_NAND,
                              NONAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS5_USE_NAND,
                              NONAND);

                    asyncCfg0Addr = &hNand->regs->ACS3CR1;
                    asyncCfg1Addr = &hNand->regs->ACS3CR2;

                    break;

                case CSL_NAND_CS4:
                    /* CS4 */
                    CSL_FINS(hNand->regs->AWCCR2, EMIF_AWCCR2_CS4_WAIT,
                             nandConfig->asyncWaitCfg->nandPort);

                    /* Set the chip select in nand flash control register */
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS2_USE_NAND,
                              NONAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS3_USE_NAND,
                              NONAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS4_USE_NAND,
                              NAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS5_USE_NAND,
                              NONAND);

                    asyncCfg0Addr = &hNand->regs->ACS4CR1;
                    asyncCfg1Addr = &hNand->regs->ACS4CR2;

                    break;

                case CSL_NAND_CS5:
                    /* CS5 */
                    CSL_FINS(hNand->regs->AWCCR2, EMIF_AWCCR2_CS5_WAIT,
                             nandConfig->asyncWaitCfg->nandPort);

                    /* Set the chip select in nand flash control register */
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS2_USE_NAND,
                              NONAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS3_USE_NAND,
                              NONAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS4_USE_NAND,
                              NONAND);
                    CSL_FINST(hNand->regs->NANDFCR, EMIF_NANDFCR_CS5_USE_NAND,
                              NAND);

                    asyncCfg0Addr = &hNand->regs->ACS5CR1;
                    asyncCfg1Addr = &hNand->regs->ACS5CR2;

                    break;

                default:
                    result = CSL_ESYS_INVPARAMS;
                    break;
            }

            /* Store the wait pin configuration */
            hNand->waitPin = (1 << nandConfig->asyncWaitCfg->nandPort);

            /* Set the wait polarity */
            CSL_FINS(hNand->regs->AWCCR2, EMIF_AWCCR2_WP0,
                     nandConfig->asyncWaitCfg->waitPol);
            CSL_FINS(hNand->regs->AWCCR2, EMIF_AWCCR2_WP1,
                     nandConfig->asyncWaitCfg->waitPol);
            CSL_FINS(hNand->regs->AWCCR2, EMIF_AWCCR2_WP2,
                     nandConfig->asyncWaitCfg->waitPol);
            CSL_FINS(hNand->regs->AWCCR2, EMIF_AWCCR2_WP3,
                     nandConfig->asyncWaitCfg->waitPol);

            /* Set the extended wait cycles */
            CSL_FINS(hNand->regs->AWCCR1, EMIF_AWCCR1_MEWC,
                     nandConfig->asyncWaitCfg->waitCycles);

            /* Configure async configuration register */
            /* Set strobe mode select bit */
            CSL_FINS(*asyncCfg1Addr, EMIF_ACS2CR2_SS,
                     nandConfig->asyncCfg->selectStrobe);
            /* Set extended wait mode bit */
            CSL_FINS(*asyncCfg1Addr, EMIF_ACS2CR2_EW,
                     nandConfig->asyncCfg->ewMode);
            /* Set write srobe setup cycles */
            CSL_FINS(*asyncCfg1Addr, EMIF_ACS2CR2_WSETUP,
                     nandConfig->asyncCfg->w_setup);
            /* Set write duration cycles */
            CSL_FINS(*asyncCfg1Addr, EMIF_ACS2CR2_WSTROBE,
                     nandConfig->asyncCfg->w_strobe);
            /* Set write hold cycles */
            CSL_FINS(*asyncCfg1Addr, EMIF_ACS2CR2_WHOLD,
                     nandConfig->asyncCfg->w_hold);
            /* Set read strobe setup cycles */
            CSL_FINS(*asyncCfg1Addr, EMIF_ACS2CR2_RSETUPMSB,
                     (nandConfig->asyncCfg->r_setup >> 0x3));
            CSL_FINS(*asyncCfg0Addr, EMIF_ACS2CR1_RSETUPLSB,
                     (nandConfig->asyncCfg->r_setup & 0x7));
            /* Set read strobe duration cycles */
            CSL_FINS(*asyncCfg0Addr, EMIF_ACS2CR1_RSTROBE,
                     nandConfig->asyncCfg->r_strobe);
            /* Set read strobe hold cycles */
            CSL_FINS(*asyncCfg0Addr, EMIF_ACS2CR1_RHOLD,
                     nandConfig->asyncCfg->r_hold);
            /* Set turn around cycles */
            CSL_FINS(*asyncCfg0Addr, EMIF_ACS2CR1_TA,
                     nandConfig->asyncCfg->turnAround);
            /* Set asynchronous memory size */
            CSL_FINS(*asyncCfg0Addr, EMIF_ACS2CR1_ASIZE,
                     nandConfig->asyncCfg->aSize);

            /* emif Byte access */
            CSL_FINS(hNand->sysCtrlRegs->ESCR, SYS_ESCR_BYTEMODE,
                     nandConfig->emifAccess);
        }
        else
        {
            result = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_getBankInfo
 *
 *   @b Description
 *   @n This API gives the bank details of the nand device. This API populates
 *      the base and offset address locations for the NAND flash based on the
 *      bank number passed as the parameter. Bank number should be decide based
 *      on the EMIF chip select connected to NAND flash as shown below.
 *      CSL_NAND_BANK_0 - EMIF_CS2
 *      CSL_NAND_BANK_1 - EMIF_CS3
 *      CSL_NAND_BANK_2 - EMIF_CS4
 *      CSL_NAND_BANK_3 - EMIF_CS5
 *
 *      NOTE: This API sets ALE and CLE address offset to default values assuming
 *      A11 address bit is connected to ALE signal of NAND flash and A12 address
 *      bit is connected to CLE signal of NAND flash.
 *      Use NAND_setLatchEnableOffset() to change the default offset values.
 *
 *   @b Arguments
 *   @verbatim
            hNand      Handle to the nand object
            bank       Pointer to nand bank info structure
            bankNum    Nand bank number
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand get bank info is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  Nand init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Populates nand bank info structure
 *
 *   @b Modifies
 *   @n Nand bank info structure
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            Uint16         bankNum;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            bankNum = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_getBankInfo(hNand, (CSL_NandAsyncBank*)&hNand->bank, bankNum);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_getBankInfo(CSL_NandHandle      hNand,
                            CSL_NandAsyncBank   *bank,
                            Uint16              bankNum)
{
    CSL_Status result;

    result = CSL_SOK;

    if(hNand != NULL)
    {
        if(bank != NULL)
        {
            switch(bankNum)
            {
                case CSL_NAND_BANK_0:
                    bank->CExDATA = (Uint32)CSL_EMIF_CS2_DATA_BASE_ADDR;
                    break;

                case CSL_NAND_BANK_1:
                    bank->CExDATA = (Uint32)CSL_EMIF_CS3_DATA_BASE_ADDR;
                    break;

                case CSL_NAND_BANK_2:
                    bank->CExDATA = (Uint32)CSL_EMIF_CS4_DATA_BASE_ADDR;
                    break;

                case CSL_NAND_BANK_3:
                    bank->CExDATA = (Uint32)CSL_EMIF_CS5_DATA_BASE_ADDR;
                    break;

                default:
                    result = CSL_ESYS_INVPARAMS;
                    break;
            }

            if(result == CSL_SOK)
            {
                bank->CExNUM = bankNum;

                /* A11 address bit is connected to  ALE signal of NAND flash */
                bank->CExALE  = (Uint32)(bank->CExDATA +
                                        (CSL_EMIF_CSx_ADDR_OFFSET));
                /* A12 address bit is  mapped to CLE signal of NAND flash */
                bank->CExCLE  = (Uint32)(bank->CExDATA +
                                         (CSL_EMIF_CSx_CMD_OFFSET));
            }
        }
        else
        {
            result = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_setLatchEnableOffset
 *
 *   @b Description
 *   @n This API sets the CLE and ALE address offset.
 *      Any of the EMIF address lines can be used to drive ALE and CLE of the
 *      NAND flash. ALE and CLE address offset are decided based of the address
 *      lines used to drive ALE and CLE pins. This API should be called after
 *      calling NAND_getBankInfo() API to modify the ALE and CLE address from
 *      the default value.
 *
 *      NOTE: Addresses passed should be offset values from the base address.
 *      For example assume that EMIF A11 line is used to drive ALE pin and
 *      EMIF A12 line is used to driver CLE pin. Then the address offset for
 *      ALE will be 0x1000 and address offset for CLE will be 0x2000.
 *
 *      Calling this API is mandatory in case of NAND ALE and CLE connected to
 *      pins other than A11 and A12 respectively. Otherwise NAND CSL APIs
 *      will not function properly.
 *
 *   @b Arguments
 *   @verbatim
            hNand         Handle to the nand object
            addrOffset    ALE offset location
            cmdOffset     CLE offset location
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Set address offset is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  Nand init should be called successfully
 *   @n  NAND_getBankInfo() should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets the ALE and CLE address offset
 *
 *   @b Modifies
 *   @n NAND handle
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            Uint16         bankNum;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            bankNum = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_getBankInfo(hNand, (CSL_NandAsyncBank*)&hNand->bank, bankNum);
            status = NAND_setLatchEnableOffset(hNand, 0x2000, 0x4000);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_setLatchEnableOffset(CSL_NandHandle    hNand,
                                     Uint32            addrOffset,
                                     Uint32            cmdOffset)
{
    CSL_Status result;

    result = CSL_SOK;

    if(hNand != NULL)
    {
        /* Set the ALE offset */
        hNand->bank.CExALE  = (Uint32)(hNand->bank.CExDATA + addrOffset);
        /* Set the CLE offset */
        hNand->bank.CExCLE  = (Uint32)(hNand->bank.CExDATA + cmdOffset);
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_isStatusWriteProtected
 *
 *   @b Description
 *   @n This is the API used to check if the nand device is write protected.
 *      It is not possible write any data to nand if it is write-protected.
 *      So this API must be called before calling the write API to check the
 *      nand status.
 *
 *   @b Arguments
 *   @verbatim
            hNand         Handle to the nand object
            WPstatus      Write protect status pointer
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Retutned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_init and NAND_getBankInfo should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Write protect status is returned in status variable
 *   @li    TRUE     - NAND device is write protected
 *   @li    FALSE    - NAND device is not write protected
 *
 *   @b Modifies
 *   @n Write protect status variable
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            Uint16         bankNum;
            CSL_Status     status;
            CSL_NandHandle hNand;
                    Uint16         wpstatus;

            instId  = 0;
            bankNum = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_getBankInfo(hNand, (CSL_NandAsyncBank*)&hNand->bank, bankNum);
             ....
             ....
            status = NAND_isStatusWriteProtected(hNand,&wpstatus);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_isStatusWriteProtected(CSL_NandHandle      hNand,
                                       Uint16              *WPstatus)
{
    volatile Uint32     timeOut;
    volatile Uint16     statusByte;
    Uint16              statusSuccess;
    CSL_Status            result;
    volatile Uint16     looper;

    result        = CSL_SOK;
    timeOut       = CSL_NAND_READ_STATUS_TIMEOUT;

    if(hNand != NULL)
    {
        if(WPstatus != NULL)
        {
            if(hNand->nandType == CSL_NAND_SMALL_BLOCK)
            {
                statusSuccess = CSL_NAND_SB_CMD_STATUS_SUCCESS;
            }
            else
            {
                statusSuccess = CSL_NAND_BB_CMD_STATUS_SUCCESS;
            }

            do
            {
                *(CSL_VUint16*)hNand->bank.CExCLE = CSL_NAND_CMD_STATUS;

                /* Wait for the device to provide response */
                for(looper = 0; looper < CSL_NAND_DELAY; looper++);

                 CSL_NAND_READWORD(hNand, statusByte);
                timeOut--;
            } while (((statusByte & statusSuccess) != statusSuccess) &&
                     (timeOut > 0));

            if(timeOut > 0)
            {
                *WPstatus = FALSE;
                return(result);
            }
            else
            {
                if(statusByte != CSL_NAND_BB_CMD_WP_STATUS_OPEN)
                {
                    *WPstatus = TRUE;
                }
                else
                {
                    *WPstatus = FALSE;
                }
            }
        }
        else
        {
            result = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return(result);
}

/** ============================================================================
 *   @n@b NAND_sendCommand
 *
 *   @b Description
 *   @n Any command to the nand device should be sent through this API.
 *      nand device is having specific commands for each operation.
 *
 *   @b Arguments
 *   @verbatim
            hNand          Handle to the nand object
            cmd            Command to the nand device
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand send command is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_init and NAND_getBankInfo should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends command to the nand device
 *
 *   @b Modifies
 *   @n Writes command to the command latch address
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            Uint16         bankNum;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            bankNum = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_getBankInfo(hNand, (CSL_NandAsyncBank*)&hNand->bank, bankNum);
             ....
             ....
            status = NAND_sendCommand(hNand,CSL_NAND_CMD_READ_START);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_sendCommand(CSL_NandHandle    hNand,
                            CSL_VUint16       cmd)
{
    CSL_Status result;

    result = CSL_SOK;

    if(NULL != hNand)
    {
        *(CSL_VUint16*)hNand->bank.CExCLE = cmd;
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_checkCommandStatus
 *
 *   @b Description
 *   @n This API checks the status of any command to the nand device.
 *      This API must be called after sending any command to the nand device.
 *
 *   @b Arguments
 *   @verbatim
            hNand    Handle to the nand object
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand status check is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_NAND_E_TIMEOUT - Time-out has occured
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_sendCommand API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns time-out condition(If occurs)
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_NandHandle    hNand;
            CSL_Status        status;

            status = NAND_sendCommand(hNand, CSL_NAND_CMD_READ_START);

            if(status == CSL_SOK)
            {
                status = NAND_checkCommandStatus(hNand);
            }
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_checkCommandStatus(CSL_NandHandle    hNand)
{
    CSL_Status         result;
    volatile Uint16    timeout;
    volatile Uint16    status;

    timeout = 0;
    result  = CSL_SOK;

    if(NULL != hNand)
    {
        /* Check for R/B status */
        do
        {
            status = CSL_FEXT(hNand->regs->NANDFSR1, EMIF_NANDFSR1_WAITSTAT);
            timeout++;

        }while(((status & hNand->waitPin) == FALSE) &&
                (timeout < CSL_NAND_MAX_TIMEOUT));

        if(timeout >= CSL_NAND_MAX_TIMEOUT)
        {
            result = CSL_NAND_E_TIMEOUT;
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_setAddress
 *
 *   @b Description
 *   @n This API used to set address of nand page to read/write data from/to
 *      nand device. Page address must be set to a valid nand page before
 *      calling read/write APIs.
 *
 *   @b Arguments
 *   @verbatim
            hNand          Handle to the nand object
            addr           Addres value to set
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand set address is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_init and NAND_getBankInfo should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets nand address
 *
 *   @b Modifies
 *   @n Writes address to the address latch
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            Uint16         bankNum;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            bankNum = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_getBankInfo(hNand, (CSL_NandAsyncBank*)&hNand->bank, bankNum);
             ....
             ....
            status = NAND_setAddress(hNand,0x01);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_setAddress(CSL_NandHandle    hNand,
                           Uint16            addr)
{
    CSL_Status    result;

    result = CSL_SOK;

    if (hNand != NULL)
    {
        if(hNand->nandWidth == CSL_NAND_8_BIT)
        {
            *(CSL_VUint8*)hNand->bank.CExALE = (Uint8)addr;
        }
        else if(hNand->nandWidth == CSL_NAND_16_BIT)
        {
            *(CSL_VUint16*)hNand->bank.CExALE = (Uint16)addr;
        }
        else
        {
            result = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_enableHwECC
 *
 *   @b Description
 *   @n This API enables hardware ECC(Error Check and  Correction) of the
 *      nand device. ECC is used to check and correct errors occurred during
 *      nand data transfer.
 *
 *   @b Arguments
 *   @verbatim
            hNand      Handle to the nand object
            csInput    Nand chip selection input
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand enable HW ECC is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  Nand init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Enables nand hardware error check and correction
 *
 *   @b Modifies
 *   @n Nand ECC registers
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj         nandObj;
            Uint16              instId;
            CSL_Status          status;
            CSL_NandHandle      hNand;
            CSL_NandChipSelect  csInput;

            instId = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            csInput = CSL_NAND_CE0;
            status = NAND_enableHwECC(hNand,csInput);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_enableHwECC(CSL_NandHandle        hNand,
                            CSL_NandChipSelect    csInput)
{
    CSL_Status    result;
    Uint16        tempVal;

    result = CSL_SOK;

    /* Save the EMIF access setting */
    tempVal = CSL_FEXT(hNand->sysCtrlRegs->ESCR, SYS_ESCR_BYTEMODE);

    /* Set EMIF access to 16 bit */
    CSL_FINST(hNand->sysCtrlRegs->ESCR, SYS_ESCR_BYTEMODE, FULL);

    if (hNand != NULL)
    {
        if(hNand->nandWidth == CSL_NAND_8_BIT)
        {
            /* 4 bit ECC possible only for 8 bit NAND devices */
            /* write the appropriate CS value */
            CSL_FINS(hNand->regs->NANDFCR, EMIF_NANDFCR_4BIT_ECC_SEL, csInput);
            /* Enable 4-Bit ECC for the selected CS */
            CSL_FINS(hNand->regs->NANDFCR, EMIF_NANDFCR_4BIT_ECC_START, TRUE);
        }
        else
        {
            /* 16 bit NAND device */
            /* only 1 bit ECC is possible for 16 bit NAND */
            switch(csInput)
            {
                case CSL_NAND_CS2:
                    /* CS2 */
                    CSL_FINS(hNand->regs->NANDFCR, EMIF_NANDFCR_CS2_ECC_START,
                             TRUE);
                    break;
                case CSL_NAND_CS3:
                    /* CS3 */
                    CSL_FINS(hNand->regs->NANDFCR, EMIF_NANDFCR_CS3_ECC_START,
                             TRUE);
                    break;
                case CSL_NAND_CS4:
                    /* CS4 */
                    CSL_FINS(hNand->regs->NANDFCR, EMIF_NANDFCR_CS4_ECC_START,
                             TRUE);
                    break;
                case CSL_NAND_CS5:
                    /* CS5 */
                    CSL_FINS(hNand->regs->NANDFCR, EMIF_NANDFCR_CS5_ECC_START,
                             TRUE);
                    break;
                default:
                    result = CSL_ESYS_INVPARAMS;
                    break;
            }
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    /* Restore EMIF access setting */
    CSL_FINS(hNand->sysCtrlRegs->ESCR, SYS_ESCR_BYTEMODE, tempVal);

    return (result);
}

/** ============================================================================
 *   @n@b NAND_disableHwECC
 *
 *   @b Description
 *   @n This API disables hardware ECC of the nand device.
 *
 *   @b Arguments
 *   @verbatim
            hNand      Handle to the nand object
            csInput    Nand chip selection input
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand disable HW ECC is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_enableHwECC API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Disables nand hardware error check and correction
 *
 *   @b Modifies
 *   @n Nand ECC registers
 *
 *   @b Example
 *   @verbatim
            CSL_Status          status;
            CSL_NandHandle      hNand;
            CSL_NandChipSelect  csInput;

            csInput = CSL_NAND_CE0;

            status = NAND_enableHwECC(hNand, csInput);
            ...
            ...
            status = NAND_disableHwECC(hNand, csInput);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_disableHwECC(CSL_NandHandle        hNand,
                             CSL_NandChipSelect    csInput)
{
    CSL_Status    result;
    Uint16        status;
    Uint16        tempVal;

    result = CSL_SOK;

    /* Save the EMIF access setting */
    tempVal = CSL_FEXT(hNand->sysCtrlRegs->ESCR, SYS_ESCR_BYTEMODE);

    /* Set EMIF access to 16 bit */
    CSL_FINST(hNand->sysCtrlRegs->ESCR, SYS_ESCR_BYTEMODE, FULL);

    if (hNand != NULL)
    {
        if(hNand->nandWidth == CSL_NAND_8_BIT)
        {
            /* 4 bit ECC possible only for 8 bit NAND devices */
            /* Stop 4-Bit ECC by reading the 4BIT ECC register */
            status = CSL_FEXT(hNand->regs->NAND4BITECC1,
                              EMIF_NAND4BITECC1_4BIT_ECC_VAL1);
        }
        else
        {
            /* 16 bit NAND device */
            switch(csInput)
            {
                case CSL_NAND_CS2:
                    /* CS2 ECC register read to stop the ECC */
                    status = CSL_FEXTR(hNand->regs->NCS2ECC1,
                                       CSL_EMIF_NCS2ECC1_MSB,
                                       CSL_EMIF_NCS2ECC1_LSB);
                    break;
                case CSL_NAND_CS3:
                    /* CS3 ECC register read to stop the ECC */
                    status = CSL_FEXTR(hNand->regs->NCS3ECC1,
                                       CSL_EMIF_NCS3ECC1_MSB,
                                       CSL_EMIF_NCS3ECC1_LSB);
                    break;
                case CSL_NAND_CS4:
                    /* CS4 ECC register read to stop the ECC */
                    status = CSL_FEXTR(hNand->regs->NCS4ECC1,
                                       CSL_EMIF_NCS4ECC1_MSB,
                                       CSL_EMIF_NCS4ECC1_LSB);
                    break;
                case CSL_NAND_CS5:
                    /* CS5 ECC register read to stop the ECC */
                    status = CSL_FEXTR(hNand->regs->NCS5ECC1,
                                       CSL_EMIF_NCS5ECC1_MSB,
                                       CSL_EMIF_NCS5ECC1_LSB);
                    break;
                default:
                    result = CSL_ESYS_INVPARAMS;
                    break;
            }
        }
    }
    else
    {
        /* Dummy instruction to avoid compilation warning */
        result = status;
        result = CSL_ESYS_BADHANDLE;
    }

    /* Restore EMIF access setting */
    CSL_FINS(hNand->sysCtrlRegs->ESCR, SYS_ESCR_BYTEMODE, tempVal);

    return (result);
}

/** ============================================================================
 *   @n@b NAND_readECC
 *
 *   @b Description
 *   @n This API is used to read the information generated by ECC hardware.
 *      Hardware ECC must be enabled before calling this API.
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
            eccBuffer    ECC Buffer pointer
            csInput      Chip select used for calculating Ecc
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand read is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_enableHwECC API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Populates ECC data into eccBuffer
 *
 *   @b Modifies
 *   @n eccBuffer
 *
 *   @b Example
 *   @verbatim
            CSL_Status           status;
            CSL_NandHandle       hNand;
            Uint16               eccBuffer[8];
            CSL_NandChipSelect   csInput;

            csInput = CSL_NAND_CE0;

            status = NAND_enableHwECC(hNand, csInput);
            ...
            ...
            status = NAND_readECC(hNand, eccBuffer, csInput);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_readECC(CSL_NandHandle        hNand,
                        Uint16                *eccBuffer,
                        CSL_NandChipSelect    csInput)
{
    CSL_Status    result;

    result = CSL_SOK;

    if(NULL != hNand)
    {
        if(NULL != eccBuffer)
        {
            if(hNand->nandWidth == CSL_NAND_8_BIT)
            {
                /* 4 bit ECC possible only for 8 bit NAND devices */
                eccBuffer[0] = CSL_FEXT(hNand->regs->NAND4BITECC1,
                                        EMIF_NAND4BITECC1_4BIT_ECC_VAL1);

                eccBuffer[1] = CSL_FEXT(hNand->regs->NAND4BITECC2,
                                        EMIF_NAND4BITECC2_4BIT_ECC_VAL2);

                eccBuffer[2] = CSL_FEXT(hNand->regs->NAND4BITECC3,
                                        EMIF_NAND4BITECC3_4BIT_ECC_VAL3);

                eccBuffer[3] = CSL_FEXT(hNand->regs->NAND4BITECC4,
                                        EMIF_NAND4BITECC4_4BIT_ECC_VAL4);

                eccBuffer[4] = CSL_FEXT(hNand->regs->NAND4BITECC5,
                                        EMIF_NAND4BITECC5_4BIT_ECC_VAL5);

                eccBuffer[5] = CSL_FEXT(hNand->regs->NAND4BITECC6,
                                        EMIF_NAND4BITECC6_4BIT_ECC_VAL6);

                eccBuffer[6] = CSL_FEXT(hNand->regs->NAND4BITECC7,
                                        EMIF_NAND4BITECC7_4BIT_ECC_VAL7);

                eccBuffer[7] = CSL_FEXT(hNand->regs->NAND4BITECC8,
                                        EMIF_NAND4BITECC8_4BIT_ECC_VAL8);
            }
            else
            {
                /* 16 bit NAND device */
                switch(csInput)
                {
                    case CSL_NAND_CS2:
                         eccBuffer[0] = CSL_FEXTR(hNand->regs->NCS2ECC1,
                                                  CSL_EMIF_NCS2ECC1_MSB,
                                                  CSL_EMIF_NCS2ECC1_LSB);

                         eccBuffer[1] = CSL_FEXTR(hNand->regs->NCS2ECC2,
                                                  CSL_EMIF_NCS2ECC2_MSB,
                                                  CSL_EMIF_NCS2ECC2_LSB);
                         break;
                    case CSL_NAND_CS3:
                         eccBuffer[0] = CSL_FEXTR(hNand->regs->NCS3ECC1,
                                                  CSL_EMIF_NCS3ECC1_MSB,
                                                  CSL_EMIF_NCS3ECC1_LSB);

                         eccBuffer[1] = CSL_FEXTR(hNand->regs->NCS3ECC2,
                                                  CSL_EMIF_NCS3ECC2_MSB,
                                                  CSL_EMIF_NCS3ECC2_LSB);
                    case CSL_NAND_CS4:
                         eccBuffer[0] = CSL_FEXTR(hNand->regs->NCS4ECC1,
                                                  CSL_EMIF_NCS4ECC1_MSB,
                                                  CSL_EMIF_NCS4ECC1_LSB);

                         eccBuffer[1] = CSL_FEXTR(hNand->regs->NCS5ECC1,
                                                  CSL_EMIF_NCS4ECC2_MSB,
                                                  CSL_EMIF_NCS4ECC2_LSB);
                         break;
                    case CSL_NAND_CS5:
                         eccBuffer[0] = CSL_FEXTR(hNand->regs->NCS5ECC1,
                                                  CSL_EMIF_NCS5ECC1_MSB,
                                                  CSL_EMIF_NCS5ECC1_LSB);

                         eccBuffer[1] = CSL_FEXTR(hNand->regs->NCS5ECC2,
                                                  CSL_EMIF_NCS5ECC2_MSB,
                                                  CSL_EMIF_NCS5ECC2_LSB);
                         break;
                    default:
                         result = CSL_ESYS_INVPARAMS;
                         break;
                }
            }
        }
        else
        {
            result = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_readNBytes
 *
 *   @b Description
 *   @n This API reads N number of bytes from a nand physical page.
 *      Number of bytes to be read is sent as an argument to this API.
 *      A read command should be sent and nand page address should be set
 *      before calling this API.
 *
 *   @b Arguments
 *   @verbatim
            hNand          Handle to the nand object
            readRequest    Number of bytes to read
            pReadBuffer    Read buffer pointer
            pack           Data pack flag
            spare          Spare area flag
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand read is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_sendCommand and NAND_setAddress APIs should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Reads the data from nand
 *
 *   @b Modifies
 *   @n Copies data from nand to read buffer
 *
 *   @b Example
 *   @verbatim
            #define  NAND_BUF_SIZE      (1024)

            CSL_Status     status;
            CSL_NandHandle hNand;
            Uint32         address;
            Uint16         nandReadBuf[NAND_BUF_SIZE];

            address = 0x40;
            status = NAND_sendCommand(hNand,CSL_NAND_CMD_READ_START);

            // Set column address
            status = NAND_setAddress(hNand,0x00);
            status = NAND_setAddress(hNand,0x00);
            // Set row address
            status = NAND_setAddress(hNand,0x40);
            status = NAND_setAddress(hNand,0x00);
            status = NAND_setAddress(hNand,0x00);
            ....
            ....
            status = NAND_readNBytes(hNand,NAND_BUF_SIZE,nandreadBuf,0,0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_readNBytes(CSL_NandHandle    hNand,
                           Uint16            readRequest,
                           Uint16            *pReadBuffer,
                           Uint16            pack,
                           Bool                 spare)
{
    CSL_Status    result;
    Uint16        index;
    Uint16        loopLimit;

    loopLimit = 0;
    result    = CSL_SOK;

    if (NULL != hNand)
    {
        if (pReadBuffer != NULL)
        {
            if (FALSE == pack)
            {
                if(spare == 0)
                {
                    loopLimit = readRequest/2;
                }
                else
                {
                    loopLimit = readRequest;
                }

                for (index = 0; index < loopLimit; index++)
                {
                    CSL_NAND_READWORD(hNand, pReadBuffer[index]);
                }
            }
            else
            {
                for (index = 0; index < (readRequest/2); index++)
                {
                    CSL_NAND_READWORD(hNand, pReadBuffer[index]);
                }
            }
        }
        else
        {
            result = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_writeNBytes
 *
 *   @b Description
 *   @n This API writes N number of bytes to a nand physical page.
 *      Number of bytes to be written is sent as an argument to this API.
 *      A page program command should be sent and nand page address
 *      should be set before calling this API. nand device should not be
 *      write-protected for proper functioning of this API.
 *
 *   @b Arguments
 *   @verbatim
            hNand          Handle to the nand object
            writeRequest   Number of bytes to write
            pWriteBuffer   Write buffer pointer
            val            If a value needs to be wriiten directly without
                           using write buffer
            spare          Spare area flag
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand write is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_sendCommand and NAND_setAddress APIs should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Writes the data to nand
 *
 *   @b Modifies
 *   @n Nand write registers
 *
 *   @b Example
 *   @verbatim
            #define  NAND_BUF_SIZE      (1024)

            CSL_Status     status;
            CSL_NandHandle hNand;
            Uint32         address;
            Uint16         nandWriteBuf[NAND_BUF_SIZE];

            address = 0x40;
            status = NAND_sendCommand(hNand,CSL_NAND_CMD_READ_START);
            // Set column address
            status = NAND_setAddress(hNand,0x00);
            status = NAND_setAddress(hNand,0x00);

            // Set row address
            status = NAND_setAddress(hNand,0x40);
            status = NAND_setAddress(hNand,0x00);
            status = NAND_setAddress(hNand,0x00);
            ....
            ....
            status = NAND_writeNBytes(hNand,NAND_BUF_SIZE,nandWriteBuf,0,0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_writeNBytes(CSL_NandHandle     hNand,
                            Uint16             writeRequest,
                            Uint16             *pWriteBuffer,
                            Uint16             val,
                            Bool               spare)
{
    CSL_Status result;
    volatile Uint16 countVar;
    volatile Uint16 loopLimit;

    result = CSL_SOK;

    if(NULL != hNand)
    {
        if(NULL == pWriteBuffer)
        {
            /* if a value 'val' needs to be directly written...*/
            for (countVar = 0; countVar < writeRequest; countVar++)
            {
                CSL_NAND_WRITEWORD(hNand, val);
            }
        }
        else
        {
            /* If the request is for 1 byte...*/
            if((1u) == writeRequest)
            {
                CSL_NAND_WRITEWORD(hNand, *pWriteBuffer);
            }
            else
            {
                /* Check whether number of bytes is multiple of 2 and then write
                 * as bytes into the data register.
                 */
                if((writeRequest % (2u)) != 0)
                {
                    for(countVar = 0; countVar < (writeRequest/2); countVar++)
                    {
                        CSL_NAND_WRITEWORD(hNand, pWriteBuffer[countVar]);
                    }

                    CSL_NAND_WRITEBYTE(hNand, (pWriteBuffer[countVar] & 0xFF));
                 }
                 else
                 {
                    if(spare == 0)
                    {
                        loopLimit = writeRequest/2;
                    }
                    else
                    {
                        loopLimit = writeRequest;
                    }

                    for (countVar = 0; countVar < loopLimit; countVar++)
                    {
                        CSL_NAND_WRITEWORD(hNand, pWriteBuffer[countVar]);
                    }
                }
            }
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_hasRandomRead
 *
 *   @b Description
 *   @n This API is used to check whether a nand device is having random
 *      page read or not. If nand is having random read we can set pointer
 *      to any address in the page and retrieve the data from there.
 *
 *   @b Arguments
 *   @verbatim
            nandOptions    Extra commands for big block nand
            RRstatus       Random read status pointer
     @endverbatim
 *
 *   <b> Return Value </b>  None
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  nandOptions information needs to be computed from nand device Id
 *
 *   <b> Post Condition </b>
 *   @n  Random read status is returned in status variable
 *   @li                TRUE     - NAND device is having random read
 *   @li                FALSE    - NAND device is not having random read
 *
 *   @b Modifies
 *   @n Random read status variable
 *
 *   @b Example
 *   @verbatim
             Uint32    nandOptions;
             Uint16    RRstatus;

             //get the nandOptions information from nand device Id
             NAND_hasRandomRead(nandOptions, &RRstatus);
     @endverbatim
 *  ============================================================================
 */
void NAND_hasRandomRead(Uint32    nandOptions,
                        Uint16    *RRstatus)
{
    Uint16    status;

    if(RRstatus != NULL)
    {
        status = (nandOptions & CSL_NAND_BB_RANDOM_PAGE_READ);

        if(status == CSL_NAND_BB_RANDOM_PAGE_READ)
        {
            *RRstatus = TRUE;
        }
        else
        {
            *RRstatus = FALSE;
        }
    }
}

/** ============================================================================
 *   @n@b NAND_hasRandomWrite
 *
 *   @b Description
 *   @n This API is used to check whether a nand device is having random
 *      page write or not. If nand is having random write we can set pointer
 *      to any address in the page and write the data from there.
 *
 *   @b Arguments
 *   @verbatim
            nandOptions    Extra commands for big block nand
            RWstatus       Random write status pointer
     @endverbatim
 *
 *   <b> Return Value </b>  None
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  nandOptions information needs to be computed from nand device Id
 *
 *   <b> Post Condition </b>
 *   @n  Random write status is returned in status variable
 *   @li                TRUE     - NAND device is having random write
 *   @li                FALSE    - NAND device is not having random write
 *
 *   @b Modifies
 *   @n Random write status variable
 *
 *   @b Example
 *   @verbatim
             Uint32    nandOptions;
             Uint16    RWstatus;

             //get the nandOptions information from nand device Id
             NAND_hasRandomWrite(nandOptions, &RWstatus);
     @endverbatim
 *  ============================================================================
 */
void NAND_hasRandomWrite(Uint32    nandOptions,
                         Uint16    *RWstatus)
{
    Uint16    status;

    if(RWstatus != NULL)
    {
        status = (nandOptions & CSL_NAND_BB_RANDOM_PAGE_WRITE);

        if(status == CSL_NAND_BB_RANDOM_PAGE_WRITE)
        {
            *RWstatus = TRUE;
        }
        else
        {
            *RWstatus = FALSE;
        }
    }
}

/** ============================================================================
 *   @n@b NAND_hasCacheProg
 *
 *   @b Description
 *   @n This API checks whether a nand device supports cache programming
 *      feature or not.
 *
 *   @b Arguments
 *   @verbatim
            nandOptions    Extra commands for big block nand
            CPstatus       Cache program status pointer
     @endverbatim
 *
 *   <b> Return Value </b>  None
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  nandOptions information needs to be computed from nand device Id
 *
 *   <b> Post Condition </b>
 *   @n  Cache program status is returned in status variable
 *   @li                TRUE     - NAND device is having cache programming
 *   @li                FALSE    - NAND device is not having cache programming
 *
 *   @b Modifies
 *   @n Cache program status variable
 *
 *   @b Example
 *   @verbatim
             Uint32    nandOptions;
             Uint16    CPstatus;

             //get the nandOptions information from nand device Id
             NAND_hasCacheProg(nandOptions, &CPstatus);
     @endverbatim
 *  ============================================================================
 */
void NAND_hasCacheProg(Uint32    nandOptions,
                       Uint16    *CPstatus)
{
    Uint16    status;

    if(CPstatus != NULL)
    {
        status = (nandOptions & CSL_NAND_BB_CACHE_PROG);

        if(status == CSL_NAND_BB_CACHE_PROG)
        {
            *CPstatus = TRUE;
        }
        else
        {
            *CPstatus = FALSE;
        }
    }
}

/** ============================================================================
 *   @n@b NAND_hasCopyBack
 *
 *   @b Description
 *   @n This API checks whether a nand device supports copy back
 *      feature or not.
 *
 *   @b Arguments
 *   @verbatim
            nandOptions    Extra commands for big block nand
            CBstatus       Copy back status pointer
     @endverbatim
 *
 *   <b> Return Value </b>  None
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  nandOptions information needs to be computed from nand device Id
 *
 *   <b> Post Condition </b>
 *   @n  Copy back status is returned in status variable
 *   @li                TRUE     - NAND device support copy back feature
 *   @li                FALSE    - NAND device does not support copy back feature
 *
 *   @b Modifies
 *   @n Copy back status variable
 *
 *   @b Example
 *   @verbatim
             Uint32    nandOptions;
             Uint16    CBstatus;

             //get the nandOptions information from nand device Id
             NAND_hasCopyBack(nandOptions, &CBstatus);
     @endverbatim
 *  ============================================================================
 */
void NAND_hasCopyBack(Uint32    nandOptions,
                      Uint16    *CBstatus)
{
    Uint16    status;

    if(CBstatus != NULL)
    {
        status = (nandOptions & CSL_NAND_BB_COPY_BACK);

        if(status == CSL_NAND_BB_COPY_BACK)
        {
            *CBstatus = TRUE;
        }
        else
        {
            *CBstatus = FALSE;
        }
    }
}

/** ============================================================================
 *   @n@b NAND_intrEnable
 *
 *   @b Description
 *   @n This API enables nand interrupts. This API is used in interrupt
 *      mode of operation of the device.
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand interrupt enable is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Enables nand interrupts
 *
 *   @b Modifies
 *   @n Nand interrupt registers
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_intrEnable(hNand);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrEnable(CSL_NandHandle    hNand)
{
    CSL_Status    result;

    result = CSL_SOK;

    if(hNand != NULL)
    {
        /** Enable Wait rise interrupt */
        CSL_FINS(hNand->regs->EIMSR, EMIF_EIMSR_WRMASKSET, TRUE);
        /** Enable line trap interrupt */
        CSL_FINS(hNand->regs->EIMSR, EMIF_EIMSR_LTMASKSET, TRUE);
        /** Enable asyncronous time-out interrupt */
        CSL_FINS(hNand->regs->EIMSR, EMIF_EIMSR_ATMASKSET, TRUE);
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_intrDisable
 *
 *   @b Description
 *   @n This API is used to disable nand interrupts.
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand interrupt disable is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_intrEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Disables nand interrupts
 *
 *   @b Modifies
 *   @n Nand interrupt registers
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_intrEnable(hNand);
            ...
            ...
            status = NAND_intrDisable(hNand);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrDisable(CSL_NandHandle    hNand)
{
    CSL_Status    result;

    result = CSL_SOK;

    if(hNand != NULL)
    {
        /** Disable Wait rise interrupt */
        CSL_FINS(hNand->regs->EIMCR, EMIF_EIMCR_WRMASKCLR, TRUE);
        /** Disable line trap interrupt */
        CSL_FINS(hNand->regs->EIMCR, EMIF_EIMCR_LTMASKCLR, TRUE);
        /** Disable asyncronous time-out interrupt */
        CSL_FINS(hNand->regs->EIMCR, EMIF_EIMCR_ATMASKCLR, TRUE);
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_getIntrNum
 *
 *   @b Description
 *   @n This API returns the interrupt number of a particular nand
 *      hardware instance.
 *
 *   @b Arguments
 *   @verbatim
            nandInstId    Nand Hardware instance number
     @endverbatim
 *
 *   <b> Return Value </b>  Int16
 *   @li                    Interrupt number of the nand device.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Returns the interrupt number of nand instance
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            int16    intrNum;
            Uint16   instId;

            instId = 0;
            intrNum = NAND_getIntrNum(instId);
     @endverbatim
 *  ============================================================================
 */
Int16 NAND_getIntrNum(Uint16    nandInstId)
{
    Int16    intrNum;

    switch(nandInstId)
    {
        case CSL_NAND_INST_0:
            intrNum = 0;
            break;
        default:
            intrNum = CSL_NAND_INV_INTR_NUM; // Invalid Interrupt number
            break;
    }
    return(intrNum);
}

/** ============================================================================
 *   @n@b NAND_intrReadAT
 *
 *   @b Description
 *   @n Reads Asynchronous time-out bit of nand interrupt raw register
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
            ATstatus     Asynchronous time-out status variable
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_intrEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Nand asynchronous time-out bit status is returned in status variable
 *
 *   @b Modifies
 *   @n Asynchronous time-out status variable
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
             Uint16         instId;
             CSL_Status     status;
             CSL_NandHandle hNand;
             Uint16         ATstatus;

             instId  = 0;
             status = NAND_init(&nandObj, instId);
             hNand  = &nandObj;
             ....
             ....
            status = NAND_intrEnable(hNand);
            ...
            ...
            status = NAND_intrReadAT(hNand,&ATstatus);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrReadAT(CSL_NandHandle    hNand,
                           Uint16            *ATstatus)
{
    CSL_Status    result;

    result = CSL_SOK;

    if(hNand != NULL)
    {
        if(ATstatus != NULL)
        {
            *ATstatus = CSL_FEXT(hNand->regs->EIRR, EMIF_EIRR_AT);
        }
        else
        {
            result = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_intrReadLT
 *
 *   @b Description
 *   @n Reads Line-trap bit of nand interrupt raw register
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
            LTstatus     Line trap status variable
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_intrEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Nand line trap bit status is returned in status variable
 *
 *   @b Modifies
 *   @n Line trap status variable
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
             Uint16         instId;
             CSL_Status     status;
             CSL_NandHandle hNand;
             Uint16         LTstatus;

             instId  = 0;
             status = NAND_init(&nandObj, instId);
             hNand  = &nandObj;
             ....
             ....
            status = NAND_intrEnable(hNand);
            ...
            ...
            status = NAND_intrReadLT(hNand,&LTstatus);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrReadLT(CSL_NandHandle    hNand,
                           Uint16            *LTstatus)
{
    CSL_Status    result;

    result = CSL_SOK;

    if(hNand != NULL)
    {
        if(LTstatus != NULL)
        {
            *LTstatus = CSL_FEXT(hNand->regs->EIRR, EMIF_EIRR_LT);
        }
        else
        {
            result = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_intrReadWR
 *
 *   @b Description
 *   @n Reads wait-rise bit of nand interrupt raw register
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
            WRstatus     Wait-raise status variable
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_intrEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Nand wait-rise bit status is returned in status variable
 *
 *   @b Modifies
 *   @n Wait-rise status variable
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
             Uint16         instId;
             CSL_Status     status;
             CSL_NandHandle hNand;
             Uint16         WRstatus;

             instId  = 0;
             status = NAND_init(&nandObj, instId);
             hNand  = &nandObj;
             ....
             ....
            status = NAND_intrEnable(hNand);
            ...
            ...
            status = NAND_intrReadWR(hNand,&WRstatus);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrReadWR(CSL_NandHandle    hNand,
                           Uint16            *WRstatus)
{
    CSL_Status    result;

    result = CSL_SOK;

    if(hNand != NULL)
    {
        if(WRstatus != NULL)
        {
            *WRstatus = CSL_FEXT(hNand->regs->EIRR, EMIF_EIRR_WR);
        }
        else
        {
            result = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

/** ============================================================================
 *   @n@b NAND_intrClear
 *
 *   @b Description
 *   @n Clears a nand interrupt
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_intrEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Clears nand interrupt
 *
 *   @b Modifies
 *   @n Nand interrupt mask register
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
             Uint16         instId;
             CSL_Status     status;
             CSL_NandHandle hNand;
             Uint16         WRstatus;

             instId  = 0;
             status = NAND_init(&nandObj, instId);
             hNand  = &nandObj;
             ....
             ....
            status = NAND_intrEnable(hNand);
            ...
            ...
            status = NAND_intrClear(hNand);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrClear(CSL_NandHandle    hNand)
{
    CSL_Status    result;

    result = CSL_SOK;

    if(hNand != NULL)
    {
        /** Clear Wait rise interrupt */
        CSL_FINS(hNand->regs->EIMR, EMIF_EIMR_WRMASKED, TRUE);
        /** Clear line trap interrupt */
        CSL_FINS(hNand->regs->EIMR, EMIF_EIMR_LTMASKED, TRUE);
        /** Clear asyncronous time-out interrupt */
        CSL_FINS(hNand->regs->EIMR, EMIF_EIMR_ATMASKED, TRUE);
    }
    else
    {
        result = CSL_ESYS_BADHANDLE;
    }

    return (result);
}

