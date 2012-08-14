/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_mem.c
 *
 *  @brief MEMORY functional layer API source file
 *
 *  Path: \(CSLPATH)\ src
 */

/* ============================================================================
 * Revision History
 * ================
 * 04-May-2009 Created for retention mode support on C5505 PG1.4
 * 26-May-2009 Code modified as per the review comments
 * 14-Dec-2009 Added support for partial retention mode for chip C5515
 * ============================================================================
 */

#include "csl_mem.h"

/** ===========================================================================
 *   @n@b MEM_init
 *
 *   @b Description
 *   @n This is the initialization function for the memory CSL module.
 *      No functionality is added for this API. This function always returns
 *      success. This function is provided for the future use.
 *
 *   @b Arguments
 *   @verbatim
        None
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li                    CSL_SOK
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

             CSL_Status status;

            status = MEM_init();

     @endverbatim
 *  ===========================================================================
 */
CSL_Status MEM_init(void)
{
    return (CSL_SOK);
}

/** ===========================================================================
 *   @n@b MEM_enableRetentionMode
 *
 *   @b Description
 *   @n This function enables the retention mode for DARAM or SARAM.
 *      In the memory retention mode memory goes to low power mode while
 *      maintaining its contents. When the memory is placed in retention mode,
 *      no accesses is allowed. Memory retention mode is enabled by
 *      configuring the RAM Sleep Mode Control Register.
 *      DARAM or SARAM can be independently configured for the memory
 *      retention mode using this API. Enabling partial memory retention
 *      mode for SARAM or DARAM is not possible.
 *
 *   @verbatim
            memType         Type of the CPU memory - DARAM/SARAM
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Enables memory retention mode for the memory specified
 *
 *   @b Modifies
 *   @n RAM Sleep Mode Control Register
 *
 *   @b Example
 *   @verbatim

            CSL_Status    status;

            status = MEM_enableRetentionMode (CSL_MEM_DARAM);

     @endverbatim
 *  ===========================================================================
 */
CSL_Status MEM_enableRetentionMode (CSL_MemType    memType)
{
    CSL_Status    status;

    status = CSL_SOK;


#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

    if (CSL_MEM_SARAM == memType)
    {
        /* Configure SARAM sleep mode control registers to
           enable memory retention for all the 32 banks */
        CSL_SYSCTRL_REGS->RAMSLPMDCNTLR2 = CSL_MEM_ENABLE_ALL_SLEEP;
        CSL_SYSCTRL_REGS->RAMSLPMDCNTLR3 = CSL_MEM_ENABLE_ALL_SLEEP;
        CSL_SYSCTRL_REGS->RAMSLPMDCNTLR4 = CSL_MEM_ENABLE_ALL_SLEEP;
        CSL_SYSCTRL_REGS->RAMSLPMDCNTLR5 = CSL_MEM_ENABLE_ALL_SLEEP;
    }
    else if (CSL_MEM_DARAM == memType)
    {
        /* Configure DARAM sleep mode control register to
           enable memory retention for all the 7 banks */
        CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1 = CSL_MEM_ENABLE_ALL_SLEEP;
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

#else

    if (CSL_MEM_SARAM == memType)
    {
        /* Configure SARAM SLPZVDD Bit */
        CSL_FINS(CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1,
                 SYS_RAMSLPMDCNTLR1_SARAMSLPZVDD, TRUE);
        /* Configure SARAM SLPZVSS Bit */
        CSL_FINS(CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1,
                 SYS_RAMSLPMDCNTLR1_SARAMSLPZVSS, FALSE);

    }
    else if (CSL_MEM_DARAM == memType)
    {
        /* Configure DARAM SLPZVDD Bit */
        CSL_FINS(CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1,
                 SYS_RAMSLPMDCNTLR1_DARAMSLPZVDD, TRUE);
        /* Configure DARAM SLPZVSS Bit */
        CSL_FINS(CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1,
                 SYS_RAMSLPMDCNTLR1_DARAMSLPZVSS, FALSE);
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

#endif

    return (status);
}

/** ===========================================================================
 *   @n@b MEM_disableRetentionMode
 *
 *   @b Description
 *   @n This function disables the memory retention mode for DARAM or SARAM.
 *      Memory retention mode is disabled by configuring the RAM Sleep Mode
 *      Control Register.
 *      Memory retention mode for DARAM or SARAM can be independently
 *      disabled using this API.
 *
 *   @verbatim
            memType         Type of the CPU memory - DARAM/SARAM
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Disables memory retention mode for the memory specified
 *
 *   @b Modifies
 *   @n RAM Sleep Mode Control Register
 *
 *   @b Example
 *   @verbatim

            CSL_Status    status;

            status = MEM_disableRetentionMode (CSL_MEM_DARAM);

     @endverbatim
 *  ===========================================================================
 */
CSL_Status MEM_disableRetentionMode (CSL_MemType    memType)
{
    CSL_Status    status;

    status = CSL_SOK;


#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

    if (CSL_MEM_SARAM == memType)
    {
        /* Configure SARAM sleep mode control registers to
           disable memory retention for all the 32 banks */
        CSL_SYSCTRL_REGS->RAMSLPMDCNTLR2 = CSL_MEM_DISABLE_ALL_SLEEP;
        CSL_SYSCTRL_REGS->RAMSLPMDCNTLR3 = CSL_MEM_DISABLE_ALL_SLEEP;
        CSL_SYSCTRL_REGS->RAMSLPMDCNTLR4 = CSL_MEM_DISABLE_ALL_SLEEP;
        CSL_SYSCTRL_REGS->RAMSLPMDCNTLR5 = CSL_MEM_DISABLE_ALL_SLEEP;
    }
    else if (CSL_MEM_DARAM == memType)
    {

        /* Configure DARAM sleep mode control register to
           disable memory retention for all the 7 banks */
        CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1 = CSL_MEM_DISABLE_ALL_SLEEP;
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

#else

    if (CSL_MEM_SARAM == memType)
    {
        /* Configure SARAM SLPZVDD Bit */
        CSL_FINS(CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1,
                 SYS_RAMSLPMDCNTLR1_SARAMSLPZVDD, TRUE);
        /* Configure SARAM SLPZVSS Bit */
        CSL_FINS(CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1,
                 SYS_RAMSLPMDCNTLR1_SARAMSLPZVSS, TRUE);
    }
    else if (CSL_MEM_DARAM == memType)
    {
        /* Configure DARAM SLPZVDD Bit */
        CSL_FINS(CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1,
                 SYS_RAMSLPMDCNTLR1_DARAMSLPZVDD, TRUE);
        /* Configure DARAM SLPZVSS Bit */
        CSL_FINS(CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1,
                 SYS_RAMSLPMDCNTLR1_DARAMSLPZVSS, TRUE);
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

#endif

    return (status);
}


#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

/** ===========================================================================
 *   @n@b MEM_enablePartialRetentionMode
 *
 *   @b Description
 *   @n This function enables retention mode for single or group of banks
 *      in DARAM or SARAM. In the memory retention mode memory goes to low
 *      power mode while maintaining its contents. When the memory is placed
 *      in retention mode, no accesses is allowed. Memory retention mode is
 *      enabled by configuring the RAM Sleep Mode Control Register.
 *      Any memory bank of DARAM or SARAM can be independently configured
 *      for the memory retention mode using this API. Enabling memory retention
 *      mode for a particular bank will effect that memory bank only and rest
 *      of the memory works normally.
 *
 *
 *      Usage of parameters 'bankNumber' and 'bankMask'
 *      This API provides two parameters to select a single block of memory
 *      or a group of memory banks to enable retention mode
 *
 *      bankNumber: Used to select a single block of memory. This number will
 *      be from 0 to 7 for DARAM and 0 to 31 for SARAM. This input is provided
 *      for to make bank number selection easy.
 *
 *      bankMask: This is a 32 bit number each bit represents one memory bank.
 *      For DARAM only 8 bits are valid. To enable memory retention mode for a
 *      memory bank set corresponding bit of 'bankMask' to '1'. For example
 *      to enable memory retention mode for SARAM banks 0, 5 and 12, value of
 *      'bankMask' should be 0x1021. Use the macro CSL_MEM_MAKE_BANKMASK to
 *      make the bankMask.
 *
 *      Set 'bankMask' to '0' while using 'bankNumber' for selecting a single
 *      bank. Set 'bankNumber' to '0' while using 'bankMask' for selecting
 *      a group of banks.
 *
 *      NOTE: PARTIAL MEMORY RETENTION MODE IS NOT SUPPORTED ON THE CHIP C5505
 *
 *   @verbatim
            memType         Type of the CPU memory - DARAM/SARAM
            bankNumber      Bank number - To select a single memory bank.
            bankMask        Bank mask - To select a group of memory banks.
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Enables memory retention mode for the memory specified
 *
 *   @b Modifies
 *   @n RAM Sleep Mode Control Register
 *
 *   @b Example
 *   @verbatim

            CSL_Status     status;
            Uint32         bankMask;

            bankMask = 0

            //Enable memory retention mode for SARAM bank 5

            status = MEM_enablePartialRetentionMode (CSL_MEM_SARAM, 5, 0);

            //Enable memory retention mode for SARAM banks 6, 25, 30.

            bankMask  = CSL_MEM_MAKE_BANKMASK(6);
            bankMask |= CSL_MEM_MAKE_BANKMASK(25);
            bankMask |= CSL_MEM_MAKE_BANKMASK(30);

            status = MEM_enablePartialRetentionMode (CSL_MEM_SARAM, 0,
                                                     bankMask);

     @endverbatim
 *  ===========================================================================
 */
CSL_Status MEM_enablePartialRetentionMode (CSL_MemType    memType,
                                           Uint16         bankNumber,
                                           Uint32         bankMask)
{
    CSL_Status    status;
    Uint16        regOffset;
    Uint16        regShift;
    Uint16        count;
    Uint16        maxBankCount;
    volatile ioport Uint16 *pSleepRegBaseAddr;

    status            = CSL_SOK;
    regOffset         = 0;
    regShift          = 0;
    count             = 0;
    maxBankCount      = 0;
    pSleepRegBaseAddr = NULL;

    /* Verify input parameters and assign register base address */
    if (CSL_MEM_SARAM == memType)
    {
        if ((0 == bankMask) && (bankNumber > CSL_SARAM_BANK_NUM_MAX))
        {
            status = CSL_ESYS_INVPARAMS;
        }
        else
        {
            pSleepRegBaseAddr = &CSL_SYSCTRL_REGS->RAMSLPMDCNTLR2;
            maxBankCount      = CSL_SARAM_BANK_COUNT;
        }
    }
    else if (CSL_MEM_DARAM == memType)
    {
        if (((0 == bankMask) && (bankNumber > CSL_DARAM_BANK_NUM_MAX)) ||
            (bankMask > CSL_MEM_DARAM_BANKMASK))
        {
            status = CSL_ESYS_INVPARAMS;
        }
        else
        {
            pSleepRegBaseAddr = &CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1;
            maxBankCount      = CSL_DARAM_BANK_COUNT;
        }
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

    /* Configure Sleep mode control registers for enabling retention mode */
    if (status == CSL_SOK)
    {
        if (0 == bankMask)
        {
            regOffset = bankNumber/8;
            regShift  = 2 * (bankNumber % 8);

            /* Clear sleep mode bits */
            *(pSleepRegBaseAddr + regOffset) &= ~(CSL_MEM_SLEEPMODE_BIT_MASK <<
                                                  regShift);

            /* Configure sleep mode bits */
            *(pSleepRegBaseAddr + regOffset) |= (CSL_MEM_ENABLE_BANK_SLEEP <<
                                                 regShift);
        }
        else
        {
            bankNumber = 0;

            for (count = 0; count < maxBankCount; count++)
            {
                if ((bankMask & 0x1) == 0x1)
                {
                    bankNumber = count;

                    regOffset = bankNumber/8;
                    regShift  = 2 * (bankNumber % 8);

                    /* Clear sleep mode bits */
                    *(pSleepRegBaseAddr + regOffset) &=
                                     ~(CSL_MEM_SLEEPMODE_BIT_MASK << regShift);

                    /* Configure sleep mode bits */
                    *(pSleepRegBaseAddr + regOffset) |=
                                       (CSL_MEM_ENABLE_BANK_SLEEP << regShift);
                }

                bankMask >>= 1;

                if (0 == bankMask)
                {
                    break;
                }
            }
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b MEM_disablePartialRetentionMode
 *
 *   @b Description
 *   @n This function disables retention mode for single or group of banks
 *      in DARAM or SARAM. Memory retention mode is disabled by configuring
 *      the RAM Sleep Mode Control Register.
 *      Any memory bank of DARAM or SARAM can be independently configured
 *      for disabling memory retention mode using this API.
 *
 *
 *      Usage of parameters 'bankNumber' and 'bankMask'
 *      This API provides two parameters to select a single block of memory
 *      or a group of memory banks to disable retention mode
 *
 *      bankNumber: Used to select a single block of memory. This number will
 *      be from 0 to 7 for DARAM and 0 to 31 for SARAM. This input is provided
 *      for to make bank number selection easy.
 *
 *      bankMask: This is a 32 bit number each bit represents one memory bank.
 *      For DARAM only 8 bits are valid. To disable memory retention mode for a
 *      memory bank set corresponding bit of 'bankMask' to '1'. For example
 *      to disable memory retention mode for SARAM banks 0, 5 and 12, value of
 *      'bankMask' should be 0x1021. Use the macro CSL_MEM_MAKE_BANKMASK to
 *      make the bankMask.
 *
 *      Set 'bankMask' to '0' while using 'bankNumber' for selecting a single
 *      bank. Set 'bankNumber' to '0' while using 'bankMask' for selecting
 *      a group of banks.
 *
 *      NOTE: PARTIAL MEMORY RETENTION MODE IS NOT SUPPORTED ON THE CHIP C5505
 *
 *   @verbatim
            memType         Type of the CPU memory - DARAM/SARAM
            bankNumber      Bank number - To select a single memory bank.
            bankMask        Bank mask - To select a group of memory banks.
     @end verbatim
 *
 *   <b> Return Value </b>
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Disables memory retention mode for the memory specified
 *
 *   @b Modifies
 *   @n RAM Sleep Mode Control Register
 *
 *   @b Example
 *   @verbatim

            CSL_Status     status;
            Uint32         bankMask;

            bankMask = 0

            //Disable memory retention mode for SARAM bank 5

            status = MEM_disablePartialRetentionMode (CSL_MEM_SARAM, 5, 0);

            //Disable memory retention mode for SARAM banks 6, 25, 30.

            bankMask  = CSL_MEM_MAKE_BANKMASK(6);
            bankMask |= CSL_MEM_MAKE_BANKMASK(25);
            bankMask |= CSL_MEM_MAKE_BANKMASK(30);

            status = MEM_disablePartialRetentionMode (CSL_MEM_SARAM, 0,
                                                     bankMask);

     @endverbatim
 *  ===========================================================================
 */
CSL_Status MEM_disablePartialRetentionMode (CSL_MemType    memType,
                                            Uint16         bankNumber,
                                            Uint32         bankMask)
{
    CSL_Status    status;
    Uint16        regOffset;
    Uint16        regShift;
    Uint16        count;
    Uint16        maxBankCount;
    volatile ioport Uint16 *pSleepRegBaseAddr;

    status            = CSL_SOK;
    regOffset         = 0;
    regShift          = 0;
    count             = 0;
    maxBankCount      = 0;
    pSleepRegBaseAddr = NULL;

    /* Verify input parameters and assign register base address */
    if (CSL_MEM_SARAM == memType)
    {
        if ((0 == bankMask) && (bankNumber > CSL_SARAM_BANK_NUM_MAX))
        {
            status = CSL_ESYS_INVPARAMS;
        }
        else
        {
            pSleepRegBaseAddr = &CSL_SYSCTRL_REGS->RAMSLPMDCNTLR2;
            maxBankCount      = CSL_SARAM_BANK_COUNT;
        }
    }
    else if (CSL_MEM_DARAM == memType)
    {
        if (((0 == bankMask) && (bankNumber > CSL_DARAM_BANK_NUM_MAX)) ||
            (bankMask > CSL_MEM_DARAM_BANKMASK))
        {
            status = CSL_ESYS_INVPARAMS;
        }
        else
        {
            pSleepRegBaseAddr = &CSL_SYSCTRL_REGS->RAMSLPMDCNTLR1;
            maxBankCount      = CSL_DARAM_BANK_COUNT;
        }
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

    /* Configure Sleep mode control registers for disabling retention mode */
    if (status == CSL_SOK)
    {
        if (0 == bankMask)
        {
            regOffset = bankNumber/8;
            regShift  = 2 * (bankNumber % 8);

            /* Configure sleep mode bits */
            *(pSleepRegBaseAddr + regOffset) |= (CSL_MEM_DISABLE_BANK_SLEEP <<
                                                 regShift);
        }
        else
        {
            bankNumber = 0;

            for (count = 0; count < maxBankCount; count++)
            {
                if ((bankMask & 0x1) == 0x1)
                {
                    bankNumber = count;

                    regOffset = bankNumber/8;
                    regShift  = 2 * (bankNumber % 8);

                    /* Configure sleep mode bits */
                    *(pSleepRegBaseAddr + regOffset) |=
                                       (CSL_MEM_DISABLE_BANK_SLEEP << regShift);
                }

                bankMask >>= 1;

                if (0 == bankMask)
                {
                    break;
                }
            }
        }
    }

    return (status);
}

/** ===========================================================================
 *   @n@b MEM_setmSDRAMClock
 *
 *   @b Description
 *   @n This function is used to make the mSDRAM Clock output switch ON and OFF
 *
 *   @verbatim
            clockSwitch    Value to switch the clock
     @end verbatim
 *
 *   <b> Return Value </b>
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Configures the mSDRAM output clock state
 *
 *   @b Modifies
 *   @n SDRAM clock control register
 *
 *   @b Example
 *   @verbatim

            CSL_Status     status;

            // Turn the mSDRAM output clock ON
            status = MEM_setmSDRAMClock (CSL_MEM_MSDARAM_CLOCK_ON);


     @endverbatim
 *  ===========================================================================
 */
CSL_Status MEM_setmSDRAMClock (CSL_MEMmSDRAMClock    clockSwitch)
{
    CSL_Status    status;

    status = CSL_SOK;

    if (CSL_MEM_MSDARAM_CLOCK_ON == clockSwitch)
    {
        CSL_FINST(CSL_SYSCTRL_REGS->SDRAMCCR, SYS_SDRAMCCR_SDCLK_EN, SDCLKON);
    }
    else if (CSL_MEM_MSDARAM_CLOCK_OFF == clockSwitch)
    {
        CSL_FINST(CSL_SYSCTRL_REGS->SDRAMCCR, SYS_SDRAMCCR_SDCLK_EN, SDCLKOFF);
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

    return (status);
}

/** ===========================================================================
 *   @n@b MEM_getmSDRAMClock
 *
 *   @b Description
 *   @n This function is used to get the value mSDRAM Clock output switch.
 *
 *   @verbatim
            None
     @end verbatim
 *
 *   <b> Return Value </b>  Bool
 *   @li                    TRUE  - mSDRAM Clock output switch is OFF
 *   @li                    FALSE - mSDRAM Clock output switch ON
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            Bool     clkStatus;

            clkStatus = MEM_getmSDRAMClock();

     @endverbatim
 *  ===========================================================================
 */
Bool MEM_getmSDRAMClock (void)
{
    Bool    bitValue;

    bitValue = CSL_FEXT(CSL_SYSCTRL_REGS->SDRAMCCR, SYS_SDRAMCCR_SDCLK_EN);

    return (bitValue);
}

#endif
