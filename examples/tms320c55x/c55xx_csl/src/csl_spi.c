/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_spi.c
 *
 *  @brief SPI functional layer API defenition file
 *
 *  Path: \(CSLPATH)/src
 */

/* ============================================================================
 * Revision History
 * ================
 * 1-Sept-2008 Added spi for CSL.
 * 1-Sept-2008 Updated for Doxygen.
 * 1-Sept-2008 Added Return Types for Error checking for API's parameter.
 * ============================================================================
 */

#include "csl_spi.h"

/** ===========================================================================
 *   @n@b SPI_init
 *
 *   @b Description
 *   @n This function intialize and activate the SPI module
 *
 *   @b Arguments
 *   @verbatim
            None
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_SOK
 *   @n                     Return value CSL_SOK for success
 *
 *   <b> Pre Condition </b>
 *   @n  This functionality has to call before any function call.
 *
 *   <b> Post Condition </b>
 *   @n   None
 *
 *   @b Modifies
 *   @n    None
 *
 *   @b Example
 *   @verbatim

            void

            result = SPI_init();
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_init(void)
{
    volatile Uint16 delay;
    ioport volatile CSL_SysRegs    *sysRegs;

    sysRegs = (CSL_SysRegs *)CSL_SYSCTRL_REGS;
    CSL_FINS(sysRegs->PCGCR1, SYS_PCGCR1_SPICG, CSL_SYS_PCGCR1_SPICG_ACTIVE);

    /* Value of 'Reset Counter' */
    CSL_FINS(sysRegs->PSRCR, SYS_PSRCR_COUNT, 0x20);

    CSL_FINS(sysRegs->PRCR, SYS_PRCR_PG4_RST, CSL_SYS_PRCR_PG4_RST_RST);

    for(delay = 0; delay < 100; delay++);

#ifdef VC5505_EZDSP
    CSL_FINS(sysRegs->EBSR, SYS_EBSR_PPMODE, CSL_SYS_EBSR_PPMODE_MODE6);
#else
    CSL_FINS(sysRegs->EBSR, SYS_EBSR_PPMODE, CSL_SYS_EBSR_PPMODE_MODE5);
#endif

    return(CSL_SOK);
}

/** ===========================================================================
 *   @n@b SPI_open
 *
 *   @b Description
 *   @n This function returns the handle to the SPI
 *      instance. This handle is passed to all other CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            Mode        chip select mode.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_SpiHandle
 *   @n                     Valid SPI handle will be returned if
 *                          handler value is not equal to NULL.
 *
 *   <b> Pre Condition </b>
 *   @n   SPI_init function has to call before call this function.
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is returned in the status variable. If status
 *              returned is
 *   @li            NULL               - InValid SPI handle is returned
 *   @n   2.
 *
 *   @b Modifies
 *   @n    1. CSL_SpiObj     SPI object structure
 *   @n    2.
 *
 *   @b Example
 *   @verbatim

            SPI_HwMode         csNum;
            SPI_OperMode     opMode(Polling/Interrupt)
            ...

            hSpi = SPI_open(SPI_HwMode    csNum, SPI_OperMode opMode);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_SpiHandle SPI_open(SPI_HwMode    csNum, SPI_OperMode opMode)
{
    CSL_SpiHandle hSpi;

    hSpi = &SPI_Instance;

    if(hSpi->configured == TRUE)
    {
        hSpi->configured = FALSE;
        hSpi = NULL;
        return (hSpi);
    }

    hSpi->mode = csNum;
    hSpi->opMode = opMode;
    return(hSpi);
}

/** ===========================================================================
 *   @n@b SPI_close
 *
 *   @b Description
 *   @n This function closes the specified handle to SPI.
 *
 *   @b Arguments
 *   @verbatim
            spiHandle          Handle to the spi
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  SPI_open() function has to call before call this function.
 *
 *   <b> Post Condition </b>
 *   @n  There should not be any function call after this function call.
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_SpiHandle   hSpi;
            CSL_status   status;

            status = SPI_close(hSpi);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_close(CSL_SpiHandle hSpi)
{
    if(NULL == hSpi)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    if(TRUE == hSpi->configured)
    {
        hSpi->configured = FALSE;
        hSpi = NULL;
    }

    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b SPI_config
 *
 *   @b Description
 *   @n It configures the SPI Controller registers of particular handle as per
 *      the values passed in the SPI config structure.
 *
 *   @b Arguments
 *   @verbatim
            spiHandle       Handle to the spi.
            spiHwConfig     Pointer to spi Config structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Congig Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  SPI_open function should call before call this function.
 *
 *   <b> Post Condition </b>
 *   @n  SPI_close function can call after this function call.
 *
 *   @b Modifies
 *   @n 1. SPI registers will be updated as per config parameter
 *   @n 2. CSL_SpiObj Object will be updated to store some info
 *              as passed config parameter.
 *
 *   @b Example
 *   @verbatim
            CSL_SpiHandle   hSpi;
            SPI_Config      spiHwConfig;
            Int16           status;

            status = SPI_config(hSpi, &spiHwConfig);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_config   (CSL_SpiHandle hSpi,
                        SPI_Config         *spiHwConfig)

{
    Int16 status;
    Uint16 sysClkDiv, spicmd2;
    volatile Uint16 delay;

    status = CSL_SOK;
    if(NULL == hSpi)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    if(NULL == spiHwConfig)
    {
        return (CSL_ESYS_INVPARAMS);
    }

    /* Disable the serial Data clock */
    CSL_FSET(CSL_SPI_REGS->SPICCR, CSL_SPI_SPICCR_CLKEN_SHIFT,
        CSL_SPI_SPICCR_CLKEN_SHIFT, CSL_SPI_SPICCR_CLKEN_DISABLED);
    //CSL_FINST(CSL_SPI_REGS->SPICCR, SPI_SPICCR_CLKEN, DISABLED);
    CSL_FINST(CSL_SPI_REGS->SPICCR, SPI_SPICCR_RST, RELEASE);

    /* Clock division must be gater or equal to 2 */
    if(spiHwConfig->spiClkDiv > 2)
    {
        sysClkDiv = spiHwConfig->spiClkDiv - 1;
    }
    else
    {
        sysClkDiv = 2; /* Default clock division is 2 */
    }

    /* Set Clock  division */
    CSL_FINS(CSL_SPI_REGS->SPICDR, SPI_SPICDR_CLKDV, sysClkDiv);

    /* Set Wordlength bit 0,1,2......31 */
    spicmd2 = spiHwConfig->wLen << CSL_SPI_SPICMD2_CLEN_SHIFT;
    //CSL_FINS(CSL_SPI_REGS->SPICMD2, SPI_SPICMD2_CLEN, spiHwConfig->wLen);

    /* Set the frame length bits 0,1,2 ......4095 */
    CSL_FINS(CSL_SPI_REGS->SPICMD1, SPI_SPICMD1_FLEN, spiHwConfig->frLen-1);

    /* Enable or Disable word count IRQ */
    CSL_FINS(CSL_SPI_REGS->SPICMD1, SPI_SPICMD1_CIRQ, spiHwConfig->wcEnable);

    for(delay = 0; delay < 16; delay++);
    /* Enable the serial Data clock */
    CSL_SPI_REGS->SPICCR =
          (Uint16)(CSL_SPI_SPICCR_CLKEN_ENABLED << CSL_SPI_SPICCR_CLKEN_SHIFT);

    /* Enable or Disable frame count IRQ */
    CSL_FINS(CSL_SPI_REGS->SPICMD1, SPI_SPICMD1_FIRQ, spiHwConfig->fcEnable);

    /* Select active CS for transfer */
    spicmd2 |= spiHwConfig->csNum << CSL_SPI_SPICMD2_CSNUM_SHIFT;
    CSL_FSET(CSL_SPI_REGS->SPICMD2, 15, 0, spicmd2);
    //CSL_FINS(CSL_SPI_REGS->SPICMD2, SPI_SPICMD2_CSNUM, spiHwConfig->csNum);

    /* Set Data delay, cs pol, clk pol and clpck pkase bit as per chip select */
    switch(spiHwConfig->csNum)
    {
        case CSL_SPI_SPICMD2_CSNUM_CS0:
            CSL_FINS(CSL_SPI_REGS->SPIDCR1, SPI_SPIDCR1_DD0,
                                            spiHwConfig->dataDelay);
            CSL_FINS(CSL_SPI_REGS->SPIDCR1, SPI_SPIDCR1_CSP0,
                                            spiHwConfig->csPol);
            CSL_FINS(CSL_SPI_REGS->SPIDCR1, SPI_SPIDCR1_CKP0,
                                            spiHwConfig->clkPol);
            CSL_FINS(CSL_SPI_REGS->SPIDCR1, SPI_SPIDCR1_CKPH0,
                                            spiHwConfig->clkPh);
            break;
        case CSL_SPI_SPICMD2_CSNUM_CS1:
            CSL_FINS(CSL_SPI_REGS->SPIDCR1, SPI_SPIDCR1_DD1,
                                            spiHwConfig->dataDelay);
            CSL_FINS(CSL_SPI_REGS->SPIDCR1, SPI_SPIDCR1_CSP1,
                                            spiHwConfig->csPol);
            CSL_FINS(CSL_SPI_REGS->SPIDCR1, SPI_SPIDCR1_CKP1,
                                            spiHwConfig->clkPol);
            CSL_FINS(CSL_SPI_REGS->SPIDCR1, SPI_SPIDCR1_CKPH1,
                                            spiHwConfig->clkPh);
            break;
        case CSL_SPI_SPICMD2_CSNUM_CS2:
            CSL_FINS(CSL_SPI_REGS->SPIDCR2, SPI_SPIDCR2_DD2,
                                            spiHwConfig->dataDelay);
            CSL_FINS(CSL_SPI_REGS->SPIDCR2, SPI_SPIDCR2_CSP2,
                                            spiHwConfig->csPol);
            CSL_FINS(CSL_SPI_REGS->SPIDCR2, SPI_SPIDCR2_CKP2,
                                            spiHwConfig->clkPol);
            CSL_FINS(CSL_SPI_REGS->SPIDCR2, SPI_SPIDCR2_CKPH2,
                                            spiHwConfig->clkPh);
            break;
        case CSL_SPI_SPICMD2_CSNUM_CS3:
            CSL_FINS(CSL_SPI_REGS->SPIDCR2, SPI_SPIDCR2_DD3,
                                            spiHwConfig->dataDelay);
            CSL_FINS(CSL_SPI_REGS->SPIDCR2, SPI_SPIDCR2_CSP3,
                                            spiHwConfig->csPol);
            CSL_FINS(CSL_SPI_REGS->SPIDCR2, SPI_SPIDCR2_CKP3,
                                            spiHwConfig->clkPol);
            CSL_FINS(CSL_SPI_REGS->SPIDCR2, SPI_SPIDCR2_CKPH3,
                                            spiHwConfig->clkPh);
            break;
        default:
            status = CSL_ESYS_INVPARAMS;
    }

    if(CSL_ESYS_INVPARAMS != status)
    {
        hSpi->configured = TRUE;
    }
    else
    {
        hSpi->configured = FALSE;
    }

    return (status);
}


/** ===========================================================================
 *   @n@b SPI_deInit
 *
 *   @b Description
 *   @n This function unintialize and activate the SPI module
 *
 *   @b Arguments
 *   @verbatim
            void    No parameter
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_SOK
 *   @n                     Return value CSL_SOK for success
 *
 *   <b> Pre Condition </b>
 *   @n  This function call call after SPI_init function.
 *
 *   <b> Post Condition </b>
 *   @n   SPI_close function can be call after this function call.
 *
 *   @b Modifies
 *   @n    1. Set the register to activate SPI module
 *   @n    2.
 *
 *   @b Example
 *   @verbatim

            void

            result = SPI_deInit();
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_deInit(void)
{
    ioport volatile CSL_SysRegs    *sysRegs;

    if(TRUE == SPI_Instance.configured)
    {
        SPI_Instance.configured = FALSE;
    }

    sysRegs = (CSL_SysRegs *)CSL_SYSCTRL_REGS;
    /*Disable the SPI clock                                                 */
    CSL_FINS(sysRegs->PCGCR1, SYS_PCGCR1_SPICG, CSL_SYS_PCGCR1_SPICG_DISABLED);

    return (CSL_SOK);
}


/** ===========================================================================
 *   @n@b SPI_read
 *
 *   @b Description
 *   @n This function Read data form specified device.
 *
 *   @b Arguments
 *   @verbatim
            hSpi              Pointer to handler
            readBuff          Pointer to the read buffer.
            buffLen           Maximum read buffer size.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Read successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  This function can call by SPI_dataTransaction function.
 *
 *   <b> Post Condition </b>
 *   @n   SPI_deInit can be call after this function call.
 *
 *   @b Modifies
 *   @n
 *
 *   @b Example
 *   @verbatim
            CSL_SpiHandle     hSpi
               Uint16          readBuff[size];
            Uint16          buffLen;
            CSL_status   status;

            status = SPI_read(hSpi,&readBuff, buffLen);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_read    (CSL_SpiHandle hSpi,
                    Uint16    *readBuffer,
                    Uint16    bufLen)
{
    Int16     getWLen;
    volatile Uint16     bufIndex;
    Int16     spiStatusReg;
    volatile Int16     spiBusyStatus;
    volatile Int16     spiWcStaus;
    volatile Uint16 delay;

    bufIndex = 0;
    if((NULL == readBuffer) || (0 == bufLen))
    {
        return (CSL_ESYS_INVPARAMS);
    }

    /* Read the word length form the register */
    getWLen = CSL_FEXT(CSL_SPI_REGS->SPICMD2, SPI_SPICMD2_CLEN) + 1;

    if(getWLen >= SPI_MAX_WORD_LEN)
    {
        return (CSL_ESYS_INVPARAMS);
    }

    /* Read Word length set by the user */
    while(bufIndex < bufLen)
    {
        /* Set command for reading buffer */
        CSL_FINS(CSL_SPI_REGS->SPICMD2, SPI_SPICMD2_CMD,
                                        CSL_SPI_SPICMD2_CMD_READ);

        for(delay = 0; delay < 100; delay++);

        do
        {
            spiStatusReg = CSL_SPI_REGS->SPISTAT1;
            spiBusyStatus = (spiStatusReg & CSL_SPI_SPISTAT1_BSY_MASK);
            spiWcStaus = (spiStatusReg & CSL_SPI_SPISTAT1_CC_MASK);
        }while((spiBusyStatus == CSL_SPI_SPISTAT1_BSY_BUSY) &&
                (spiWcStaus != CSL_SPI_SPISTAT1_CC_MASK));

        if(getWLen == 16)
        {
            readBuffer[bufIndex] = CSL_SPI_REGS->SPIDR1;
            bufIndex++;
        }
        else if(getWLen == 8)
        {
            readBuffer[bufIndex] = (CSL_SPI_REGS->SPIDR1 & 0xFF);
            bufIndex++;
        }
    }
    return (CSL_SOK);
}


/** ===========================================================================
 *   @n@b SPI_write
 *
 *   @b Description
 *   @n This function Write data to specified device.
 *
 *   @b Arguments
 *   @verbatim
             hSpi              Pointer to handler
            writeBuff         Pointer to the write buffer.
            buffLen           Maximum read buffer size.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Write successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *       This function can call by SPI_dataTransaction function.
 *
 *   <b> Post Condition </b>
 *   @n   SPI_deInit can be call after this function call.
 *
 *   @b Modifies
 *   @n
 *
 *   @b Example
 *   @verbatim
            CSL_SpiHandle     hSpi
             Uint16          writeBuff[size];
            Uint16          buffLen;
            CSL_status       status;

            status = SPI_write(hSpi,&writeBuff, buffLen);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_write (CSL_SpiHandle hSpi,
                        Uint16    *writeBuffer,
                        Uint16    bufLen)
{
    Uint16     getWLen;
    volatile Uint16     bufIndex;
    Uint16     spiStatusReg;
    volatile Uint16     spiBusyStatus;
    volatile Uint16     spiWcStaus;
    volatile Uint16     delay;

    bufIndex = 0;
    if((NULL == writeBuffer) || (0 == bufLen))
    {
        return (CSL_ESYS_INVPARAMS);
    }

    /* Read the word length form the register */
    getWLen = CSL_FEXT(CSL_SPI_REGS->SPICMD2, SPI_SPICMD2_CLEN) + 1;

    if(getWLen >= SPI_MAX_WORD_LEN)
    {
        return (CSL_ESYS_INVPARAMS);
    }

    /* Write Word length set by the user */
    while(bufIndex < bufLen)
    {
        /* Write to registers more then 16 bit word length */
        if(getWLen == 16)
        {
            CSL_SPI_REGS->SPIDR2 = writeBuffer[bufIndex];
            CSL_SPI_REGS->SPIDR1 = 0x0000;
            bufIndex ++;
        }
        /* Write to register less then or equal to 16 bit word length */
        else if(getWLen == 8)
        {
            CSL_SPI_REGS->SPIDR2 = (Uint16)(writeBuffer[bufIndex] << 0x08);
            CSL_SPI_REGS->SPIDR1 = 0x0000;
            bufIndex++;
        }

        /* Set command for Writting to registers */
        CSL_FINS(CSL_SPI_REGS->SPICMD2, SPI_SPICMD2_CMD, SPI_WRITE_CMD);

        for(delay = 0; delay < 100; delay++);

        do
        {
            spiStatusReg = CSL_SPI_REGS->SPISTAT1;
            spiBusyStatus = (spiStatusReg & CSL_SPI_SPISTAT1_BSY_MASK);
            spiWcStaus = (spiStatusReg & CSL_SPI_SPISTAT1_CC_MASK);
        }while((spiBusyStatus == CSL_SPI_SPISTAT1_BSY_BUSY) &&
                (spiWcStaus != CSL_SPI_SPISTAT1_CC_MASK));
    }
    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b SPI_dataTransaction
 *
 *   @b Description
 *   @n This function read or Write data to specified device.
 *
 *   @b Arguments
 *   @verbatim
            spiHandle         Handle to the spi
            writeBuff         Pointer to the write buffer.
            buffLen           Maximum read buffer size.
            readOrWrite          Read or write command.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - transation success
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *
 *   <b> Pre Condition </b>
 *       This function can after SPI_open function.
 *
 *   <b> Post Condition </b>
 *   @n   SPI_deInit can be call after this function call.
 *
 *   @b Modifies
 *   @n
 *
 *   @b Example
 *   @verbatim
            CSL_SpiHandle   hSpi;
            Uint16          writeBuff[size];
            Uint16          buffLen;
            SPI_Command     readOrWrite
            CSL_status   status;
            ...
            ...
            status = SPI_dataTransaction(hSpi, &rwBuffer,
                                           rwBufLen, readOrWrite);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SPI_dataTransaction(CSL_SpiHandle hSpi,
                                Uint16          *rwBuffer,
                                Uint16          rwBufLen,
                                SPI_Command     readOrWrite)
{
    CSL_Status    status;

    if(NULL == hSpi)
    {
        return (CSL_ESYS_BADHANDLE);
    }

    if((NULL == rwBuffer) || (0 == rwBufLen))
    {
        return (CSL_ESYS_INVPARAMS);
    }

    if(FALSE == hSpi->configured)
    {
        return (CSL_ESYS_INVPARAMS);
    }

    /* Read data from registers                                */
    if(SPI_READ_CMD == readOrWrite)
    {
        status = SPI_read(hSpi, rwBuffer, rwBufLen);
    }
    /* Write data to registers                                */
    else if(SPI_WRITE_CMD == readOrWrite)
    {
        status = SPI_write(hSpi, rwBuffer, rwBufLen);
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

    return (status);
}





