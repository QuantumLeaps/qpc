/********************************************************************
* Copyright (C) 2003-2008 Texas Instruments Incorporated.
* All Rights Reserved
*********************************************************************
* file: cslr_wdt.h
*
* Brief: This file contains the Register Description for wdt
*
*********************************************************************/
#ifndef _CSLR_WDT_H_
#define _CSLR_WDT_H_

#include <cslr.h>

#include <tistdtypes.h>
#include <csl_general.h>


/* Minimum unit = 2 bytes */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 WDKCKLK;
    volatile Uint16 RSVD0;
    volatile Uint16 WDKICK;
    volatile Uint16 RSVD1;
    volatile Uint16 WDSVLR;
    volatile Uint16 RSVD2;
    volatile Uint16 WDSVR;
    volatile Uint16 RSVD3;
    volatile Uint16 WDENLOK;
    volatile Uint16 RSVD4;
    volatile Uint16 WDEN;
    volatile Uint16 RSVD5;
    volatile Uint16 WDPSLR;
    volatile Uint16 RSVD6;
    volatile Uint16 WDPS;
} CSL_WdtRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* WDKCKLK */

#define CSL_WDT_WDKCKLK_KICKLOK_MASK     (0xFFFFu)
#define CSL_WDT_WDKCKLK_KICKLOK_SHIFT    (0x0000u)
#define CSL_WDT_WDKCKLK_KICKLOK_RESETVAL (0x0000u)

#define CSL_WDT_WDKCKLK_RESETVAL         (0x0000u)

/* WDKICK */

#define CSL_WDT_WDKICK_KICK_MASK         (0xFFFFu)
#define CSL_WDT_WDKICK_KICK_SHIFT        (0x0000u)
#define CSL_WDT_WDKICK_KICK_RESETVAL     (0x0000u)

#define CSL_WDT_WDKICK_RESETVAL          (0x0000u)

/* WDSVLR */

#define CSL_WDT_WDSVLR_STVALLOK_MASK     (0xFFFFu)
#define CSL_WDT_WDSVLR_STVALLOK_SHIFT    (0x0000u)
#define CSL_WDT_WDSVLR_STVALLOK_RESETVAL (0x0000u)

#define CSL_WDT_WDSVLR_RESETVAL          (0x0000u)

/* WDSVR */

#define CSL_WDT_WDSVR_STRTVAL_MASK       (0xFFFFu)
#define CSL_WDT_WDSVR_STRTVAL_SHIFT      (0x0000u)
#define CSL_WDT_WDSVR_STRTVAL_RESETVAL   (0x0000u)

#define CSL_WDT_WDSVR_RESETVAL           (0x0000u)

/* WDENLOK */

#define CSL_WDT_WDENLOK_ENLOK_MASK       (0xFFFFu)
#define CSL_WDT_WDENLOK_ENLOK_SHIFT      (0x0000u)
#define CSL_WDT_WDENLOK_ENLOK_RESETVAL   (0x0000u)

#define CSL_WDT_WDENLOK_RESETVAL         (0x0000u)

/* WDEN */

#define CSL_WDT_WDEN_RSV_MASK            (0xFFFEu)
#define CSL_WDT_WDEN_RSV_SHIFT           (0x0001u)
#define CSL_WDT_WDEN_RSV_RESETVAL        (0x0000u)

#define CSL_WDT_WDEN_EN_MASK             (0x0001u)
#define CSL_WDT_WDEN_EN_SHIFT            (0x0000u)
#define CSL_WDT_WDEN_EN_RESETVAL         (0x0000u)
/*----EN Tokens----*/
#define CSL_WDT_WDEN_EN_DISABLE          (0x0000u)
#define CSL_WDT_WDEN_EN_ENABLE           (0x0001u)

#define CSL_WDT_WDEN_RESETVAL            (0x0000u)

/* WDPSLR */

#define CSL_WDT_WDPSLR_PSLOK_MASK        (0xFFFFu)
#define CSL_WDT_WDPSLR_PSLOK_SHIFT       (0x0000u)
#define CSL_WDT_WDPSLR_PSLOK_RESETVAL    (0x0000u)

#define CSL_WDT_WDPSLR_RESETVAL          (0x0000u)

/* WDPS */

#define CSL_WDT_WDPS_PS_MASK             (0xFFFFu)
#define CSL_WDT_WDPS_PS_SHIFT            (0x0000u)
#define CSL_WDT_WDPS_PS_RESETVAL         (0x0000u)

#define CSL_WDT_WDPS_RESETVAL            (0x0000u)

#endif
