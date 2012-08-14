/********************************************************************
* Copyright (C) 2003-2008 Texas Instruments Incorporated.
* All Rights Reserved
*********************************************************************
* file: cslr_tim.h
*
* Brief: This file contains the Register Description for tim
*
*********************************************************************/
#ifndef _CSLR_TIM_H_
#define _CSLR_TIM_H_

#include <cslr.h>

#include <tistdtypes.h>
#include <csl_general.h>


/* Minimum unit = 2 bytes */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 TCR;
    volatile Uint16 RSVD0;
    volatile Uint16 TIMPRD1;
    volatile Uint16 TIMPRD2;
    volatile Uint16 TIMCNT1;
    volatile Uint16 TIMCNT2;
    //volatile Uint16 TIMINT;
} CSL_TimRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* TCR */

#define CSL_TIM_TCR_TIMEN_MASK           (0x8000u)
#define CSL_TIM_TCR_TIMEN_SHIFT          (0x000Fu)
#define CSL_TIM_TCR_TIMEN_RESETVAL       (0x0000u)
/*----TIMEN Tokens----*/
#define CSL_TIM_TCR_TIMEN_DISABLE        (0x0000u)
#define CSL_TIM_TCR_TIMEN_ENABLE         (0x0001u)


#define CSL_TIM_TCR_PSCDIV_MASK          (0x003Cu)
#define CSL_TIM_TCR_PSCDIV_SHIFT         (0x0002u)
#define CSL_TIM_TCR_PSCDIV_RESETVAL      (0x0000u)

#define CSL_TIM_TCR_AUTORELOAD_MASK      (0x0002u)
#define CSL_TIM_TCR_AUTORELOAD_SHIFT     (0x0001u)
#define CSL_TIM_TCR_AUTORELOAD_RESETVAL  (0x0000u)
/*----AUTORELOAD Tokens----*/
#define CSL_TIM_TCR_AUTORELOAD_DISABLE   (0x0000u)
#define CSL_TIM_TCR_AUTORELOAD_ENABLE    (0x0001u)

#define CSL_TIM_TCR_START_MASK           (0x0001u)
#define CSL_TIM_TCR_START_SHIFT          (0x0000u)
#define CSL_TIM_TCR_START_RESETVAL       (0x0000u)

#define CSL_TIM_TCR_RESETVAL             (0x0000u)

/* TIMPRD1 */

#define CSL_TIM_TIMPRD1_PRD1_MASK        (0xFFFFu)
#define CSL_TIM_TIMPRD1_PRD1_SHIFT       (0x0000u)
#define CSL_TIM_TIMPRD1_PRD1_RESETVAL    (0x0000u)

#define CSL_TIM_TIMPRD1_RESETVAL         (0x0000u)

/* TIMPRD2 */

#define CSL_TIM_TIMPRD2_PRD2_MASK        (0xFFFFu)
#define CSL_TIM_TIMPRD2_PRD2_SHIFT       (0x0000u)
#define CSL_TIM_TIMPRD2_PRD2_RESETVAL    (0x0000u)

#define CSL_TIM_TIMPRD2_RESETVAL         (0x0000u)

/* TIMCNT1 */

#define CSL_TIM_TIMCNT1_TIM1_MASK        (0xFFFFu)
#define CSL_TIM_TIMCNT1_TIM1_SHIFT       (0x0000u)
#define CSL_TIM_TIMCNT1_TIM1_RESETVAL    (0x0000u)

#define CSL_TIM_TIMCNT1_RESETVAL         (0x0000u)

/* TIMCNT2 */

#define CSL_TIM_TIMCNT2_TIM2_MASK        (0xFFFFu)
#define CSL_TIM_TIMCNT2_TIM2_SHIFT       (0x0000u)
#define CSL_TIM_TIMCNT2_TIM2_RESETVAL    (0x0000u)

#define CSL_TIM_TIMCNT2_RESETVAL         (0x0000u)

/* TIMINT */


//#define CSL_TIM_TIMINT_INT_MASK          (0x0001u)
//#define CSL_TIM_TIMINT_INT_SHIFT         (0x0000u)
//#define CSL_TIM_TIMINT_INT_RESETVAL      (0x0000u)
/*----INT Tokens----*/
//#define CSL_TIM_TIMINT_INT_CLEAR         (0x0000u)
//#define CSL_TIM_TIMINT_INT_SET           (0x0001u)

//#define CSL_TIM_TIMINT_RESETVAL          (0x0000u)

#endif
