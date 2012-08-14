/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_mem.h
 *
 *  @brief MEMORY functional layer API header file
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 04-May-2009 Created for retention mode support on C5505 PG1.4
 * 26-May-2009 Code modified as per the review comments
 * 14-Dec-2009 Added support for partial retention mode for chip C5515
 * ============================================================================
 */

/** @defgroup CSL_MEM_API MEMORY
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 * VC5505 DSP is having two types of internal random access memories:
 * Single Access RAM(SARAM) and Dual Access RAM(DARAM).
 * C5505 features 32Kx16-bit of DARAM. The DARAM is composed of 8 blocks of
 * 4Kx16- bit each. The DARAM is located in the byte address
 * range 0000h-0FFFFh. It can be accessed by program, data and DMA busses.
 * C5505 features 128Kx16-bit of on-chip SARAM. The SARAM is composed of 32
 * blocks of 4Kx16-bit each. The SARAM is located in the byte address range
 * 010000h-04FFFFh. It can be accessed by the program, data and DMA busses.
 *
 * CSL MEMORY module can be used to enable or disable the memory retention
 * mode for SARAM and DARAM. With memory retention enabled memory goes to
 * low power mode while maintaining its contents. When the memory is placed
 * in retention mode no accesses is allowed. Accessing the data after enabling
 * memory retention mode will result in data corruption. DARAM or SARAM can be
 * independently configured for the memory retention mode. Enabling partial
 * memory retention mode for SARAM or DARAM is not possible.
 *
 * @subsection References
 * C5505_Spec_1.16.pdf
 */

#ifndef _CSL_MEM_H_
#define _CSL_MEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_types.h>
#include <soc.h>
#include <csl_general.h>

/**
@defgroup CSL_MEM_SYMBOL  MEMORY Symbols Defined
@ingroup CSL_MEM_API
*/
/**
@defgroup CSL_MEM_DATASTRUCT  MEMORY Data Structures
@ingroup CSL_MEM_API
*/
/**
@defgroup CSL_MEM_FUNCTION  MEMORY Functions
@ingroup CSL_MEM_API
*/
/**
@defgroup CSL_MEM_ENUM MEMORY Enumerated Data Types
@ingroup CSL_MEM_API
*/


/*****************************************************************************\
          MEMORY global macro declarations
\*****************************************************************************/

/** @addtogroup CSL_MEM_SYMBOL
 @{ */

/** Number of banks in DARAM memory                                          */
#define CSL_DARAM_BANK_COUNT          (8)
/** Number of banks in SARAM memory                                          */
#define CSL_SARAM_BANK_COUNT          (32)

/** Maximum allowed bank number for DARAM memory                             */
#define CSL_DARAM_BANK_NUM_MAX        (CSL_DARAM_BANK_COUNT - 1)
/** Maximum allowed bank number for SARAM memory                             */
#define CSL_SARAM_BANK_NUM_MAX        (CSL_SARAM_BANK_COUNT - 1)

/** Macro to indicate DARAM Bank 0                                           */
#define CSL_MEM_DARAM_BANK0           (0)
/** Macro to indicate DARAM Bank 7                                           */
#define CSL_MEM_DARAM_BANK7           (7u)

/** Macro to indicate SARAM Bank 0                                           */
#define CSL_MEM_SARAM_BANK0           (0)
/** Macro to indicate SARAM Bank 8                                           */
#define CSL_MEM_SARAM_BANK8           (8u)
/** Macro to indicate SARAM Bank 16                                          */
#define CSL_MEM_SARAM_BANK16          (16u)
/** Macro to indicate SARAM Bank 24                                          */
#define CSL_MEM_SARAM_BANK24          (24u)
/** Macro to indicate SARAM Bank 31                                          */
#define CSL_MEM_SARAM_BANK31          (31u)

/** Macro value to enable to memory retention mode for all the blocks        */
#define CSL_MEM_ENABLE_ALL_SLEEP      (0xAAAAu)
/** Macro value to disable to memory retention mode for all the blocks       */
#define CSL_MEM_DISABLE_ALL_SLEEP     (0xFFFFu)

/** Macro value to enable to  memory retention mode for single bank          */
#define CSL_MEM_ENABLE_BANK_SLEEP     (0x0002)
/** Macro value to disable to  memory retention mode for single bank         */
#define CSL_MEM_DISABLE_BANK_SLEEP    (0x0003)

/** Mask for sleep mode control register bits of a memory bank               */
#define CSL_MEM_SLEEPMODE_BIT_MASK    (0x0003)

/** Maximum value of bank mask for DARAM memory                              */
#define CSL_MEM_DARAM_BANKMASK        (0xFFu)

/** Macro to make the bank mask based on the bank number                     */
#define CSL_MEM_MAKE_BANKMASK(bankNumber)    ((Uint32)1 << bankNumber)

/**
@} */

/**************************************************************************\
* MEMORY global typedef declarations                                       *
\**************************************************************************/

/** @addtogroup CSL_MEM_ENUM
 @{ */

/**
 * \brief
 *
 *   This enum holds the values to represent the type of the CPU memory.
 *   Application program can send a request to a specific memory
 *   using this enum.
 */
typedef enum
{
    CSL_MEM_DARAM,    /* Memory type is DARAM */
    CSL_MEM_SARAM,    /* Memory type is SARAM */
    CSL_MEM_INVALID      /* Invalid Memory type  */
} CSL_MemType;

/**
 * \brief
 *
 *   This enum holds the values to switch the mSDRAM output clock state
 */
typedef enum
{
    CSL_MEM_MSDARAM_CLOCK_ON,    /* MSDARAM clock will be set to ON  */
    CSL_MEM_MSDARAM_CLOCK_OFF    /* MSDARAM clock will be set to OFF */
} CSL_MEMmSDRAMClock;

/**
@} */


/**************************************************************************\
* MEMORY function declarations                                              *
\**************************************************************************/

/** @addtogroup CSL_MEM_FUNCTION
 @{ */

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
CSL_Status MEM_init(void);

/** ===========================================================================
 *   @n@b MEM_enableRetentionMode
 *
 *   @b Description
 *   @n This function enables the retention mode for whole DARAM or SARAM.
 *      In the memory retention mode memory goes to low power mode while
 *      maintaining its contents. When the memory is placed in retention mode,
 *      no accesses is allowed. Memory retention mode is enabled by
 *      configuring the RAM Sleep Mode Control Register.
 *      DARAM or SARAM can be independently configured for the memory
 *      retention mode using this API.
 *
 *      NOTE: This function enables memory retention mode for the entire
 *      DARAM or SARAM. This function can be used on both chips C5505
 *      and C5515.
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
CSL_Status MEM_enableRetentionMode (CSL_MemType    memType);

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
 *      NOTE: This function disables memory retention mode for the entire
 *      DARAM or SARAM. This function can be used on both chips C5505
 *      and C5515.
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
CSL_Status MEM_disableRetentionMode (CSL_MemType    memType);


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
                                           Uint32         bankMask);

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
                                            Uint16         bankNum,
                                            Uint32         bankMask);

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
CSL_Status MEM_setmSDRAMClock (CSL_MEMmSDRAMClock    clockSwitch);

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
Bool MEM_getmSDRAMClock (void);

#endif


/**
@} */

#ifdef __cplusplus
}
#endif

#endif    // _CSL_MEM_H_

