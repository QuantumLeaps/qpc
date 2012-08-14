/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_i2c.c
 *
 *  @brief I2C functional layer API source file
 *
 *  Path: \(CSLPATH)\ src
 */

/* ============================================================================
 * Revision History
 * ================
 * 08-Oct-2008 Created
 * 23-Oct-2008 Updated for code review comments
 * ============================================================================
 */

#include "csl_i2c.h"

CSL_I2cObj    gI2cObj[CSL_I2C_PER_CNT];
pI2cHandle    i2cHandle;

/** ============================================================================
 *   @n@b I2C_init
 *
 *   @b Description
 *   @n Initializes the I2C CSL module
 *
 *   @b Arguments
 *   @verbatim
            instanceNum      I2C Hardware instance number
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes I2C module
 *
 *   @b Modifies
 *   @n I2C Object structure
 *
 *   @b Example
 *   @verbatim
            CSL_Status    status;
            status = I2C_init(CSL_I2C0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_init(Uint16    instanceNum)
{
    CSL_Status         status;
    volatile Uint16    looper;

    status = CSL_SOK;

    switch(instanceNum)
    {
        case CSL_I2C0 :
            i2cHandle = &gI2cObj[CSL_I2C0];
            i2cHandle->i2cRegs = CSL_I2C_0_REGS;
            i2cHandle->sysCtrlRegs = CSL_SYSCTRL_REGS;

            /* Enable I2C module in Idle PCGCR */
            CSL_FINST(i2cHandle->sysCtrlRegs->PCGCR1,
                      SYS_PCGCR1_I2CCG, ACTIVE);

            /* Reset I2C module */
            CSL_FINS(i2cHandle->sysCtrlRegs->PSRCR, SYS_PSRCR_COUNT,
                     CSL_I2C_RESET_COUNT_VAL);

            CSL_FINST(i2cHandle->sysCtrlRegs->PRCR, SYS_PRCR_I2C_RST, RST);

            /* Give some delay for the device to reset */
            for(looper = 0; looper < CSL_I2C_RESET_DELAY; looper++)
            {;}

            break;

        default :
            status = CSL_ESYS_INVPARAMS;
            break;
    }

    return(status);
}


/** ============================================================================
 *   @n@b I2C_config
 *
 *   @b Description
 *   @n  Configures the I2C module. Using this function application can pass the
 *   value of the each register to be configured.
 *
 *   @b Arguments
 *   @verbatim
            i2cConfig      I2C config structure pointer
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Configures the I2C registers
 *
 *   @b Modifies
 *   @n I2C registers
 *
 *   @b Example
 *   @verbatim
            CSL_Status       status;
            CSL_I2cConfig    i2cConfig;

            i2cConfig.icoar  = CSL_I2C_ICOAR_DEFVAL;
            i2cConfig.icimr  = CSL_I2C_ICIMR_DEFVAL;
            i2cConfig.icclkl = CSL_I2C_ICCLK_DEFVAL;
            i2cConfig.icclkh = CSL_I2C_ICCLK_DEFVAL;
            i2cConfig.iccnt  = CSL_I2C_DATA_SIZE;
            i2cConfig.icsar  = CSL_I2C_ICSAR_DEFVAL;
            i2cConfig.icmdr  = CSL_I2C_ICMDR_WRITE_DEFVAL;
            i2cConfig.icemdr = CSL_I2C_ICEMDR_DEFVAL;
            i2cConfig.icpsc  = CSL_I2C_ICPSC_DEFVAL;

            status = I2C_init(CSL_I2C0);
            ....
            status = I2C_config(&i2cConfig);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_config(CSL_I2cConfig    *i2cConfig)
{

    if(i2cConfig != NULL)
    {
        /* Configure I2C Own Address register */
        CSL_FINS(i2cHandle->i2cRegs->ICOAR, I2C_ICOAR_OADDR,
                 i2cConfig->icoar);

        /* Configure I2C interrupt mask register */
        CSL_FINSR(i2cHandle->i2cRegs->ICIMR, CSL_I2C_ICIMR_AAS_SHIFT,
                  CSL_I2C_ICIMR_AL_SHIFT, i2cConfig->icimr);

        /* Clear I2C interrupt status register */
        i2cHandle->i2cRegs->ICSTR = CSL_I2C_ICSTR_RESET_VALUE;

        /* Configure I2C clock low register */
        CSL_FINS(i2cHandle->i2cRegs->ICCLKL, I2C_ICCLKL_ICCL,
                 i2cConfig->icclkl);

        /* Configure I2C clock high register */
        CSL_FINS(i2cHandle->i2cRegs->ICCLKH, I2C_ICCLKH_ICCH,
                 i2cConfig->icclkh);

        /* Configure I2C count register */
        CSL_FINS(i2cHandle->i2cRegs->ICCNT, I2C_ICCNT_ICDC, i2cConfig->iccnt);

        /* Configure I2C slave address register */
        CSL_FINS(i2cHandle->i2cRegs->ICSAR, I2C_ICSAR_SADDR, i2cConfig->icsar);

        /* Configure I2C prescaler register */
        CSL_FINS(i2cHandle->i2cRegs->ICPSC, I2C_ICPSC_IPSC, i2cConfig->icpsc);

        /* Configure I2C extended mode address register */
        CSL_FINSR(i2cHandle->i2cRegs->ICEMDR, CSL_I2C_ICEMDR_IGNACK_SHIFT,
                  CSL_I2C_ICEMDR_BCM_SHIFT, i2cConfig->icemdr);

        /* Configure I2C mode address register */
        i2cHandle->i2cRegs->ICMDR = i2cConfig->icmdr;
    }
    else
    {
        return(CSL_ESYS_INVPARAMS);
    }

    return(CSL_SOK);
}


/** ============================================================================
 *   @n@b I2C_getConfig
 *
 *   @b Description
 *   @n Gets the I2C configuration parameters
 *
 *   @b Arguments
 *   @verbatim
            i2cgetConfig      I2C Configuration structure pointer
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_config should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Populates the I2C config structure
 *
 *   @b Modifies
 *   @n Structure pointed by getI2cConfig
 *
 *   @b Example
 *   @verbatim
            CSL_Status       status;
            CSL_I2cConfig    i2cConfig;
            CSL_I2cConfig    i2cgetConfig

            i2cConfig.icoar  = CSL_I2C_ICOAR_DEFVAL;
            i2cConfig.icimr  = CSL_I2C_ICIMR_DEFVAL;
            i2cConfig.icclkl = CSL_I2C_ICCLK_DEFVAL;
            i2cConfig.icclkh = CSL_I2C_ICCLK_DEFVAL;
            i2cConfig.iccnt  = CSL_I2C_DATA_SIZE;
            i2cConfig.icsar  = CSL_I2C_ICSAR_DEFVAL;
            i2cConfig.icmdr  = CSL_I2C_ICMDR_WRITE_DEFVAL;
            i2cConfig.icemdr = CSL_I2C_ICEMDR_DEFVAL;
            i2cConfig.icpsc  = CSL_I2C_ICPSC_DEFVAL;

            status = I2C_init(CSL_I2C0);
            ....
            status = I2C_config(&i2cConfig);
            ....
            ....
            status = I2C_getConfig(&i2cgetConfig);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_getConfig(CSL_I2cConfig    *i2cgetConfig)
{

    if(i2cgetConfig != NULL)
    {
        /* Read I2C Own Address register */
        i2cgetConfig->icoar = CSL_FEXT(i2cHandle->i2cRegs->ICOAR,
                                       I2C_ICOAR_OADDR);

        /* Read I2C interrupt mask register */
        i2cgetConfig->icimr = CSL_FEXTR(i2cHandle->i2cRegs->ICIMR,
                                        CSL_I2C_ICIMR_AAS_SHIFT,
                                        CSL_I2C_ICIMR_AL_SHIFT);

        /* Read I2C clock low register */
        i2cgetConfig->icclkl = CSL_FEXT(i2cHandle->i2cRegs->ICCLKL,
                                        I2C_ICCLKL_ICCL);

        /* Read I2C clock high register */
        i2cgetConfig->icclkh = CSL_FEXT(i2cHandle->i2cRegs->ICCLKH,
                                        I2C_ICCLKH_ICCH);

        /* Read I2C count register */
        i2cgetConfig->iccnt = CSL_FEXT(i2cHandle->i2cRegs->ICCNT,
                                       I2C_ICCNT_ICDC);

        /* Read I2C slave address register */
        i2cgetConfig->icsar = CSL_FEXT(i2cHandle->i2cRegs->ICSAR,
                                       I2C_ICSAR_SADDR);

        /* Read I2C mode address register */
        i2cgetConfig->icmdr = i2cHandle->i2cRegs->ICMDR;

        /* Read I2C extended mode address register */
        i2cgetConfig->icemdr = CSL_FEXTR(i2cHandle->i2cRegs->ICEMDR,
                                         CSL_I2C_ICEMDR_IGNACK_SHIFT,
                                         CSL_I2C_ICEMDR_BCM_SHIFT);

        /* Read I2C prescaler register */
        i2cgetConfig->icpsc = CSL_FEXT(i2cHandle->i2cRegs->ICPSC,
                                       I2C_ICPSC_IPSC);
    }
    else
    {
        return(CSL_ESYS_INVPARAMS);
    }

    return(CSL_SOK);
}


/** ============================================================================
 *   @n@b I2C_setup
 *
 *   @b Description
 *   @n Sets the I2C hardware registers.
 *      This functions task is same as that of I2C_config, but this function
 *      provides more flexible interface to the application. Application can
 *      pass the individual configuration values rather than the register value.
 *
 *   @b Arguments
 *   @verbatim
            i2cSetup      I2C setup structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets the I2C register values
 *
 *   @b Modifies
 *   @n I2C registers
 *
 *   @b Example
 *   @verbatim
            #define CSL_I2C_SYS_CLK        (12)    // In MHz
            #define CSL_I2C_BUS_FREQ       (10)    // In KHz

            CSL_Status       status;
            CSL_I2cSetup     i2cSetup;

            i2cSetup.addrMode    = CSL_I2C_ADDR_7BIT;
            i2cSetup.bitCount    = CSL_I2C_BC_8BITS;
            i2cSetup.loopBack    = CSL_I2C_LOOPBACK_DISABLE;
            i2cSetup.freeMode    = CSL_I2C_FREEMODE_DISABLE;
            i2cSetup.repeatMode  = CSL_I2C_REPEATMODE_DISABLE;
            i2cSetup.ownAddr     = CSL_I2C_OWN_ADDR;
            i2cSetup.sysInputClk = CSL_I2C_SYS_CLK;
            i2cSetup.i2cBusFreq  = CSL_I2C_BUS_FREQ;

            status = I2C_init(CSL_I2C0);
            ....
            status = I2C_setup(&i2cSetup);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_setup(CSL_I2cSetup    *i2cSetup)
{
    Uint16    pscValue;
    Uint16    clock;
    Uint16    d;

    if(i2cSetup != NULL)
    {
        /* Set I2C address mode */
        CSL_FINS(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_XA, i2cSetup->addrMode);

        /* Set I2C Bit count value */
        CSL_FINS(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_BC, i2cSetup->bitCount);

        /* Enable/Disable loopback mode */
        CSL_FINS(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_DLB, i2cSetup->loopBack);

        /* Enable/Disable free running mode */
        CSL_FINS(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_FREE,
                 i2cSetup->freeMode);

        /* Enable/Disable repeat mode */
        CSL_FINS(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_RM,
                 i2cSetup->repeatMode);

        /* Set the I2C own address */
        CSL_FINS(i2cHandle->i2cRegs->ICOAR, I2C_ICOAR_OADDR,
                 i2cSetup->ownAddr);

        /* calculating the IPSC value */
        pscValue = (i2cSetup->sysInputClk) / CSL_I2C_MODULE_CLOCK_FREQ;

        /* Adjust the Prescaler value */
        if(pscValue > (i2cSetup->sysInputClk % CSL_I2C_MODULE_CLOCK_FREQ))
        {
            pscValue -= CSL_I2C_PSC_ADJUST_VAL;
        }

        if(pscValue == CSL_I2C_PSC0)
        {
            d = CSL_I2C_PSC0_DVAL;
        }
        else if(pscValue == CSL_I2C_PSC1)
        {
            d = CSL_I2C_PSC1_DVAL;
        }
        else
        {
            d = CSL_I2C_PSC2TOFF_DVAL;
        }

        CSL_FINS(i2cHandle->i2cRegs->ICPSC, I2C_ICPSC_IPSC, pscValue);

        /* calculating the ICCLKL and ICCLKH register values */
        clock = (((i2cSetup->sysInputClk * CSL_I2C_CLK_MULT * CSL_I2C_CLK_MULT) /                      \
                 ((pscValue + CSL_I2C_PSC_ADJUST_VAL) * i2cSetup->i2cBusFreq *                         \
                 CSL_I2C_CLK_MULT * 2u)) - (d)) ;

        /* Configure clock low register */
        CSL_FINS(i2cHandle->i2cRegs->ICCLKL, I2C_ICCLKL_ICCL, clock);
        /* Configure clock high register */
        CSL_FINS(i2cHandle->i2cRegs->ICCLKH, I2C_ICCLKH_ICCH, clock);

        /* Bring the I2C out of reset */
        CSL_FINST(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_IRS, SET);
    }
    else
    {
        return(CSL_ESYS_INVPARAMS);
    }

    return(CSL_SOK);
}


/** ============================================================================
 *   @n@b I2C_write
 *
 *   @b Description
 *   @n Transmits the I2C data to I2C device.
 *
 *   @b Arguments
 *   @verbatim
            i2cWrBuf           I2C Data buffer pointer
            dataLength         Number bytes to transmit
            slaveAddr          Address of the slave device
            masterMode         Master mode flag
            startStopFlag      Flag for the start and stop bits
            timeout            Time out variable
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK               - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS    - Invalid parameter
 *   @li                    CSL_I2C_BUS_BUSY_ERR  - Busy Bit Error
 *   @li                    CSL_I2C_TIMEOUT_ERROR - Time out Error has occured
 *   @li                    CSL_I2C_NACK_ERR      - No acknowledgement Error
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init and I2C_config/I2C_setup should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Transmits the I2C data
 *
 *   @b Modifies
 *   @n I2C transmit register
 *
 *   @b Example
 *   @verbatim
            #define CSL_I2C_SYS_CLK          (12)    // In MHz
            #define CSL_I2C_BUS_FREQ         (10)    // In KHz
            #define CSL_I2C_DATA_SIZE        (16)
            #define CSL_I2C_EEPROM_ADDR      (0x50)

            Uint16           i2cDataBuf[CSL_I2C_DATA_SIZE];
            CSL_Status       status;
            CSL_I2cSetup     i2cSetup;

            i2cSetup.addrMode    = CSL_I2C_ADDR_7BIT;
            i2cSetup.bitCount    = CSL_I2C_BC_8BITS;
            i2cSetup.loopBack    = CSL_I2C_LOOPBACK_DISABLE;
            i2cSetup.freeMode    = CSL_I2C_FREEMODE_DISABLE;
            i2cSetup.repeatMode  = CSL_I2C_REPEATMODE_DISABLE;
            i2cSetup.ownAddr     = CSL_I2C_OWN_ADDR;
            i2cSetup.sysInputClk = CSL_I2C_SYS_CLK;
            i2cSetup.i2cBusFreq  = CSL_I2C_BUS_FREQ;

            status = I2C_init(CSL_I2C0);
            ....
            status = I2C_setup(&i2cSetup);
            ....
            ....
            status = I2C_write(i2cDataBuf, CSL_I2C_DATA_SIZE,
                               CSL_I2C_EEPROM_ADDR, TRUE, 1,
                               CSL_I2C_MAX_TIMEOUT);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_write(Uint16    *i2cWrBuf,
                     Uint16    dataLength,
                     Uint16    slaveAddr,
                     Bool      masterMode,
                     Uint16    startStopFlag,
                     Uint16    timeout)
{
    Uint16        looper;
    Uint16        dataCount;
    Uint16        statusByte;

    if((i2cWrBuf != NULL) && (dataLength !=0))
    {
        /* check for bus busy */
        for(looper = 0; looper < timeout; looper++)
        {
            statusByte = CSL_FEXT(i2cHandle->i2cRegs->ICSTR, I2C_ICSTR_BB);
            if(statusByte == FALSE)
            {
                break;
            }
        }

        if(looper >= timeout)
        {
            /* bus busy timeout error */
            return(CSL_I2C_BUS_BUSY_ERR);
        }

        /* Set the Tx mode */
        CSL_FINST(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_TRX, SET);

        /* Set the data length */
        CSL_FINS(i2cHandle->i2cRegs->ICCNT, I2C_ICCNT_ICDC, dataLength);

        if(masterMode == TRUE)
        {
            /* Set the slave address */
            CSL_FINS(i2cHandle->i2cRegs->ICSAR, I2C_ICSAR_SADDR, slaveAddr);

            /* Enable Master mode */
            CSL_FINST(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_MST, SET);

            /* Set the stop bit */
            if((startStopFlag & CSL_I2C_STOP) == CSL_I2C_STOP)
            {
                CSL_FINST(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_STP, SET);
            }

            /* Set the start bit */
            if((startStopFlag & CSL_I2C_START) == CSL_I2C_START)
            {
                CSL_FINST(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_STT, SET);
            }
        }
        else
        {
            /* Disable Master mode */
            CSL_FINST(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_MST, CLEAR);
        }

        for(dataCount = 0; dataCount < dataLength; dataCount++)
        {
            /* Check for ICXRDY status */
            for(looper = 0; looper < timeout; looper++)
            {
                statusByte = CSL_FEXT(i2cHandle->i2cRegs->ICSTR,
                                      I2C_ICSTR_ICXRDY);
                if(statusByte == TRUE)
                {
                    break;
                }
            }

            if(looper >= timeout)
            {
                return(CSL_I2C_TIMEOUT_ERROR);
            }

            /* Write data to the data Tx register */
            CSL_FINS(i2cHandle->i2cRegs->ICDXR, I2C_ICDXR_D, *i2cWrBuf++);

            for(looper = 0; looper < timeout; looper++)
            {
                /* Check for NACK status */
                statusByte = CSL_FEXT(i2cHandle->i2cRegs->ICSTR,
                                      I2C_ICSTR_NACK);
                if(statusByte == FALSE)
                {
                    break;
                }
            }

            if(looper >= timeout)
            {
                return(CSL_I2C_NACK_ERR);
            }
        }
    }
    else
    {
        return(CSL_ESYS_INVPARAMS);
    }

    return(CSL_SOK);
}


/** ============================================================================
 *   @n@b I2C_read
 *
 *   @b Description
 *   @n Receives the I2C data from the I2C device.
 *
 *   @b Arguments
 *   @verbatim
            i2cRdBuf           I2C Data buffer pointer
            dataLength         Number bytes to receive
            slaveAddr          Address of the slave device
            masterMode         Master mode flag
            startStopFlag      Flag for the start and stop bits
            timeout            Time out variable
            checkBus           Check Bus busy flag
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li       CSL_SOK                      - Returned for success
 *   @li       CSL_ESYS_INVPARAMS           - Invalid parameter
 *   @li       CSL_I2C_BUS_BUSY_ERR         - Busy Bit Error
 *   @li       CSL_I2C_TIMEOUT_ERROR        - Time out Error has occured
 *   @li       CSL_I2C_RECEIVE_OVERFLOW_ERR - Recieve Register over flow error
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init and I2C_config/I2C_setup should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Receives the I2C data
 *
 *   @b Modifies
 *   @n I2C data buffer
 *
 *   @b Example
 *   @verbatim
            #define CSL_I2C_SYS_CLK          (12)    // In MHz
            #define CSL_I2C_BUS_FREQ         (10)    // In KHz
            #define CSL_I2C_DATA_SIZE        (16)
            #define CSL_I2C_EEPROM_ADDR      (0x50)
            #define CSL_I2CEEPROM_ADDR_SIZE  (2)

            Uint16           i2cDataBuf[CSL_I2C_DATA_SIZE];
            CSL_Status       status;
            CSL_I2cSetup     i2cSetup;

            i2cSetup.addrMode    = CSL_I2C_ADDR_7BIT;
            i2cSetup.bitCount    = CSL_I2C_BC_8BITS;
            i2cSetup.loopBack    = CSL_I2C_LOOPBACK_DISABLE;
            i2cSetup.freeMode    = CSL_I2C_FREEMODE_DISABLE;
            i2cSetup.repeatMode  = CSL_I2C_REPEATMODE_DISABLE;
            i2cSetup.ownAddr     = CSL_I2C_OWN_ADDR;
            i2cSetup.sysInputClk = CSL_I2C_SYS_CLK;
            i2cSetup.i2cBusFreq  = CSL_I2C_BUS_FREQ;

            status = I2C_init(CSL_I2C0);
            ....
            status = I2C_setup(&i2cSetup);
            ....
            ....
            status = I2C_read(i2cDataBuf, CSL_I2C_DATA_SIZE, CSL_I2C_EEPROM_ADDR,
                              TRUE, 1, CSL_I2C_MAX_TIMEOUT, FALSE);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_read(Uint16    *i2cRdBuf,
                    Uint16    dataLength,
                    Uint16    slaveAddr,
                    Bool      masterMode,
                    Uint16    startStopFlag,
                    Uint16    timeout,
                    Bool      checkBus)
{
    Uint16        looper;
    Uint16        dataCount;
    Uint16        statusByte;

    if((i2cRdBuf != NULL) && (dataLength !=0))
    {
        /* Set the Rx mode */
        CSL_FINST(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_TRX, CLEAR);

        /* Set the data length */
        CSL_FINS(i2cHandle->i2cRegs->ICCNT, I2C_ICCNT_ICDC, dataLength);

        if(masterMode == TRUE)
        {
            /* Set the slave address */
            CSL_FINS(i2cHandle->i2cRegs->ICSAR, I2C_ICSAR_SADDR, slaveAddr);

            /* Enable Master mode */
            CSL_FINST(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_MST, SET);

            /* Set the stop bit */
            if((startStopFlag & CSL_I2C_STOP) == CSL_I2C_STOP)
            {
                CSL_FINST(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_STP, SET);
            }

            /* Set the start bit */
            if((startStopFlag & CSL_I2C_START) == CSL_I2C_START)
            {
                CSL_FINST(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_STT, SET);
            }
        }
        else
        {
            /* Disable Master mode */
            CSL_FINST(i2cHandle->i2cRegs->ICMDR, I2C_ICMDR_MST, CLEAR);
        }

        if(checkBus == TRUE)
        {
            /* check for bus busy */
            for(looper = 0; looper < timeout; looper++)
            {
                statusByte = CSL_FEXT(i2cHandle->i2cRegs->ICSTR, I2C_ICSTR_BB);
                if (statusByte == FALSE)
                {
                    break;
                }
            }

            if(looper >= timeout)
            {
                /* bus busy timeout error */
                return(CSL_I2C_BUS_BUSY_ERR);
            }
        }

        for(dataCount = 0; dataCount < dataLength; dataCount++)
        {
            /* Check for ICRRDY status */
            for(looper = 0; looper < timeout; looper++)
            {
                statusByte = CSL_FEXT(i2cHandle->i2cRegs->ICSTR,
                                      I2C_ICSTR_ICRRDY);
                if(statusByte == TRUE)
                {
                    break;
                }
            }

            if(looper >= timeout)
            {
                return(CSL_I2C_TIMEOUT_ERROR);
            }

            /* Read data from the data Rx register */
            *i2cRdBuf++ = CSL_FEXT(i2cHandle->i2cRegs->ICDRR, I2C_ICDRR_D);

            /* Check for Overflow status */
            statusByte = CSL_FEXT(i2cHandle->i2cRegs->ICSTR, I2C_ICSTR_RSFULL);
            if(statusByte == TRUE)
            {
                return(CSL_I2C_RECEIVE_OVERFLOW_ERR);
            }
        }
    }
    else
    {
        return(CSL_ESYS_INVPARAMS);
    }

    return(CSL_SOK);
}


/** ============================================================================
 *   @n@b I2C_eventEnable
 *
 *   @b Description
 *   @n Enables the I2C events for interrupt generation.
 *      I2C has several events which can generate interrupt to the CPU.
 *      Application can enable any of the event using this function.
 *
 *   @b Arguments
 *   @verbatim
            i2cEvent      Event to be enabled
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Enables I2C interrupt for a particular event
 *
 *   @b Modifies
 *   @n I2C Interrupt enable register
 *
 *   @b Example
 *   @verbatim
            CSL_Status       status;

            status = I2C_init(CSL_I2C0);
            ....
            ....
            status = I2C_eventEnable(CSL_I2C_EVENT_ICXRDY);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_eventEnable(CSL_I2cEvent    i2cEvent)
{
    CSL_Status    status;

    status = CSL_SOK;

    switch(i2cEvent)
    {
        /* Enable Arbitration loss Event */
        case CSL_I2C_EVENT_AL:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_AL, ENABLE);
            break;

        /* Enable No acknowledgement Event */
        case CSL_I2C_EVENT_NACK:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_NACK, ENABLE);
            break;

        /* Enable Access ready Event */
        case CSL_I2C_EVENT_ARDY:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_ARDY, ENABLE);
            break;

        /* Enable Receive ready Event */
        case CSL_I2C_EVENT_ICRRDY:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_ICRRDY, ENABLE);
            break;

        /* Enable Transmit ready Event */
        case CSL_I2C_EVENT_ICXRDY:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_ICXRDY, ENABLE);
            break;

        /* Enable Stop condition detected Event */
        case CSL_I2C_EVENT_SCD:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_SCD, ENABLE);
            break;

        /* Enable Address as slave Event */
        case CSL_I2C_EVENT_AAS:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_AAS, ENABLE);
            break;

        default :
            status = CSL_ESYS_INVPARAMS;
            break;
    }

    return(status);
}


/** ============================================================================
 *   @n@b I2C_eventDisable
 *
 *   @b Description
 *   @n Disables the I2C events.
 *      I2C has several events which can generate interrupt to the CPU.
 *      Application can disable any of the event using this function.
 *
 *   @b Arguments
 *   @verbatim
            i2cEvent      Event to be disabled
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init and I2C_eventEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Disables I2C interrupt for a particular event
 *
 *   @b Modifies
 *   @n I2C Interrupt enable register
 *
 *   @b Example
 *   @verbatim
            CSL_Status       status;

            status = I2C_init(CSL_I2C0);
            ....
            ....
            status = I2C_eventEnable(CSL_I2C_EVENT_ICXRDY);
            ....
            ....
            status = I2C_eventDisable(CSL_I2C_EVENT_ICXRDY);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_eventDisable(CSL_I2cEvent    i2cEvent)
{
    CSL_Status    status;

    status = CSL_SOK;

    switch(i2cEvent)
    {
        /* Disable Arbitration loss Event */
        case CSL_I2C_EVENT_AL:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_AL, DISABLE);
            break;

        /* Disable No acknowledgement Event */
        case CSL_I2C_EVENT_NACK:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_NACK, DISABLE);
            break;

        /* Disable Access ready Event */
        case CSL_I2C_EVENT_ARDY:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_ARDY, DISABLE);
            break;

        /* Disable Receive ready Event */
        case CSL_I2C_EVENT_ICRRDY:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_ICRRDY, DISABLE);
            break;

        /* Disable Transmit ready Event */
        case CSL_I2C_EVENT_ICXRDY:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_ICXRDY, DISABLE);
            break;

        /* Disable Stop condition detected Event */
        case CSL_I2C_EVENT_SCD:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_SCD, DISABLE);
            break;

        /* Disable Address as slave Event */
        case CSL_I2C_EVENT_AAS:
            CSL_FINST(i2cHandle->i2cRegs->ICIMR, I2C_ICIMR_AAS, DISABLE);
            break;

        default :
            status = CSL_ESYS_INVPARAMS;
            break;
    }

    return(status);
}


/** ============================================================================
 *   @n@b I2C_setCallback
 *
 *   @b Description
 *   @n Sets the call back functions for different I2C interrupts.
 *      These callback functions are called from the Interrupt service
 *      routine based on the interrupt generated by a particular event.
 *
 *   @b Arguments
 *   @verbatim
            i2cIsrAddr      I2C ISR address structure pointer
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Sets the call functions
 *
 *   @b Modifies
 *   @n Callback pointer array
 *
 *   @b Example
 *   @verbatim
            void alCallback(void);      // Arbitration Loss event callback
            void nackCallback(void);    // No acknowledgement event callback
            void ardyCallback(void);    // register access ready event callback
            void rrdyCallback(void);    // receive data ready event callback
            void xrdyCallback(void);    // transmit data ready event callback
            void scdCallback(void);     // stop condition detected event callback
            void aasCallback(void);     // Address As Slave interrupt callback

            CSL_I2cIsrAddr    i2cIsrAddr;
            CSL_Status        status;

            i2cIsrAddr.alAddr   = alCallback;
            i2cIsrAddr.nackAddr = nackCallback;
            i2cIsrAddr.ardyAddr = ardyCallback;
            i2cIsrAddr.rrdyAddr = rrdyCallback;
            i2cIsrAddr.xrdyAddr = xrdyCallback;
            i2cIsrAddr.scdAddr  = scdCallback;
            i2cIsrAddr.aasAddr  = aasCallback;

            status = I2C_setCallback(i2cIsrAddr);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_setCallback(CSL_I2cIsrAddr    *i2cIsrAddr)
{

    if(i2cIsrAddr != NULL)
    {
        i2cHandle->I2C_isrDispatchTable[CSL_I2C_EVENT_AL] =
                                                  i2cIsrAddr->alAddr;
        i2cHandle->I2C_isrDispatchTable[CSL_I2C_EVENT_NACK] =
                                                  i2cIsrAddr->nackAddr;
        i2cHandle->I2C_isrDispatchTable[CSL_I2C_EVENT_ARDY] =
                                                  i2cIsrAddr->ardyAddr;
        i2cHandle->I2C_isrDispatchTable[CSL_I2C_EVENT_ICRRDY] =
                                                  i2cIsrAddr->rrdyAddr;
        i2cHandle->I2C_isrDispatchTable[CSL_I2C_EVENT_ICXRDY] =
                                                  i2cIsrAddr->xrdyAddr;
        i2cHandle->I2C_isrDispatchTable[CSL_I2C_EVENT_SCD] =
                                                  i2cIsrAddr->scdAddr;
        i2cHandle->I2C_isrDispatchTable[CSL_I2C_EVENT_AAS] =
                                                  i2cIsrAddr->aasAddr;
    }
    else
    {
        return(CSL_ESYS_INVPARAMS);
    }

    return(CSL_SOK);
}


/** ============================================================================
 *   @n@b I2C_getEventId
 *
 *   @b Description
 *   @n Returns the I2C event ID. This event represent the I2C interrupt
 *      in the I2C interrupt vector register
 *
 *   @b Arguments
 *   @verbatim
            None
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  I2C Event Id
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Returns I2C event Id
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            Int16    i2cEvent;

            i2cEvent = I2C_getEventId();
     @endverbatim
 *  ============================================================================
 */
Int16 I2C_getEventId(void)
{
    Int16    eventId;

    /* Read the event generated the interrupt from interrupt vector register */
    eventId = (Int16)CSL_FEXT(i2cHandle->i2cRegs->ICIVR, I2C_ICIVR_INTCODE);

    return(eventId);
}

