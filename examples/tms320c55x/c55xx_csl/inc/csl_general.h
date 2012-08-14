/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_general.h
 *
 *  @brief CSL general header file
 *
 *  This file contains the general CSL macros or definitions that are required
 *  to enable few functionalities specific to the chip revision. This file is
 *  included in all the cslr files. All the CSL applications should include
 *  this file.
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 07-Jan-2009 Created
 * 28-Apr-2010 Added new macro definitions
 * 30-June-2010 Added new macro definitions for platforms
 * ============================================================================
 */

#ifndef _CSL_GENERAL_H_
#define _CSL_GENERAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 ******************************************************
 * CAUTION: DEFINE EITHER THE MACROS IN PART1 OR PART2.
 * DO NOT DEFINE MACROS IN BOTH PARTS.
 ******************************************************
 */

/** ***************************************************************************
 * --------------------------------------------------
 * PART1: CHIP VERSION MACRO DEFINITION FOR PG1.4 CSL
 * --------------------------------------------------
 *
 * Below macros are used to enable the source code specific to chip versions
 * TMS320VC5505 and TMS320VC5504. Design revision(DesignRev) of the DIE ID
 * regsiter(0x1C43 & 0x1C44) will have the value '4' for this chip set.
 *
 * 'CHIP_VC5505' indicates TMS320VC5505 DSP.
 * 'CHIP_VC5504' indicates TMS320VC5504 DSP.
 *
 * DSPs TMS320VC5505 and TMS320VC5504 are having the same peripherals but
 * they differ in the internal memory available.
 *
 * Defining any of the below macros will enable the code for both the DSPs
 * TMS320VC5504 and TMS320VC5505. Current CSL software does not have the code
 * that is specific to TMS320VC5504 DSP.
 */

/* TMS320VC5505 */
//#define CHIP_VC5505

/*
 * Below macro was the used to indicate TMS320VC5505 in old
 * CSL version. Here it is provided for backward compatibility
 */
//#define CHIP_5505


/* TMS320VC5504 */
//#define CHIP_VC5504

/*
 * Below macro was the used to indicate TMS320VC5504 in old
 * CSL version. Here it is provided for backward compatibility
 */
//#define CHIP_5504

/******************************* END OF PART1 *******************************/


/* Mapping of the old macros to the new macros */
#ifdef CHIP_5505
#define CHIP_VC5505
#endif

#ifdef CHIP_5504
#define CHIP_VC5504
#endif



/** ***************************************************************************
 * --------------------------------------------------
 * PART2: CHIP VERSION MACRO DEFINITION FOR PG2.0 CSL
 * --------------------------------------------------
 *
 * Below macros are used to enable the source code specific to chip versions
 * TMS320C5505, TMS320C5515, TMS320C5504, and TMS320C5514.
 * Design revision(DesignRev) of the DIE ID regsiter(0x1C43 & 0x1C44) will have
 * the value '5' for this chip set.
 *
 * 'CHIP_C5505_C5515' indicates TMS320C5505 and TMS320C5515.
 * 'CHIP_C5504_C5514' indicates TMS320C5504 and TMS320C5514.
 *
 * DSPs TMS320C5505 and TMS320C5515 are similar but certain LDOs not bonded out
 * for TMS320C5505 DSP.
 * DSPs TMS320C5504 and TMS320C5514 are similar but certain LDOs not bonded out
 * for TMS320C5504 DSP.
 *
 * DSPs TMS320C55x5 and TMS320C55x4 are having the same peripherals but
 * they differ in the internal memory available. 'x' stands for 0 or 1.
 *
 * Defining any of the below macros will enable the code for all the DSPs
 * TMS320C5504, TMS320C5505, TMS320C5514 and TMS320C5515.
 * Current CSL software does not have the code that is specific to TMS320C5504
 * and TMS320C5514 DSPs.
 */

#if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))

/* TMS320C5505 and TMS320C5515 */
#define CHIP_C5505_C5515

/*
 * Below macro was the used to indicate TMS320C5505 and TMS320C5515 in old
 * CSL version. Here it is provided for backward compatibility
 */
#define CHIP_5515


/* TMS320C5504 and TMS320C5514 */
#define CHIP_C5504_C5514

/*
 * Below macro was the used to indicate TMS320C5504 and TMS320C5514 in old
 * CSL version. Here it is provided for backward compatibility
 */
//#define CHIP_5514

#endif

/******************************* END OF PART2 *******************************/

/** ***************************************************************************
 * --------------------------------------------------
 * PART3: EVM VERSION MACRO DEFINITION FOR PG2.0 CSL
 * --------------------------------------------------
 *
 * Below macros are used to enable the source code specific to EVM versions
 */

#if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))

#define C5515_EVM

#if (!(defined(C5515_EVM)))
#define C5515_EZDSP
#endif

#else

#define VC5505_EVM

#if (!(defined(VC5505_EVM)))
#define VC5505_EZDSP
#endif

#endif
/******************************* END OF PART3 *******************************/


/* Mapping of the old macros to the new macros */
#ifdef CHIP_5515
#define CHIP_C5505_C5515
#endif

#ifdef CHIP_5514
#define CHIP_C5504_C5514
#endif


#ifdef __cplusplus
}
#endif

#endif    // _CSL_GENERAL_H_

