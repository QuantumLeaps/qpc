/********************************************************************
* Copyright (C) 2003-2008 Texas Instruments Incorporated.
* All Rights Reserved
*********************************************************************
* file: cslr_cpu.h
*
* Brief: This file contains the Register Description for cpu
*
*********************************************************************/
#ifndef _CSLR_CPU_H_
#define _CSLR_CPU_H_

#include <cslr.h>

#include <tistdtypes.h>
#include <csl_general.h>


/* Minimum unit = 2 bytes */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 IER0;
    volatile Uint16 IFR0;
    volatile Uint16 ST0_55;
    volatile Uint16 ST1_55;
    volatile Uint16 ST3_55;
    volatile Uint16 RSVD0[64];
    volatile Uint16 IER1;
    volatile Uint16 IFR1;
    volatile Uint16 RSVD1[2];
    volatile Uint16 IVPD;
    volatile Uint16 IVPH;
    volatile Uint16 ST2_55;
} CSL_CpuRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* IER0 */

#define CSL_CPU_IER0_RCV2_MASK           (0x8000u)
#define CSL_CPU_IER0_RCV2_SHIFT          (0x000Fu)
#define CSL_CPU_IER0_RCV2_RESETVAL       (0x0000u)
/*----RCV2 Tokens----*/
#define CSL_CPU_IER0_RCV2_DISABLE        (0x0000u)
#define CSL_CPU_IER0_RCV2_ENABLE         (0x0001u)

#define CSL_CPU_IER0_XMT2_MASK           (0x4000u)
#define CSL_CPU_IER0_XMT2_SHIFT          (0x000Eu)
#define CSL_CPU_IER0_XMT2_RESETVAL       (0x0000u)
/*----XMT2 Tokens----*/
#define CSL_CPU_IER0_XMT2_DISABLE        (0x0000u)
#define CSL_CPU_IER0_XMT2_ENABLE         (0x0001u)

#define CSL_CPU_IER0_SAR_MASK            (0x2000u)
#define CSL_CPU_IER0_SAR_SHIFT           (0x000Du)
#define CSL_CPU_IER0_SAR_RESETVAL        (0x0000u)
/*----SAR Tokens----*/
#define CSL_CPU_IER0_SAR_DISABLE         (0x0000u)
#define CSL_CPU_IER0_SAR_ENABLE          (0x0001u)

#define CSL_CPU_IER0_LCD_MASK            (0x1000u)
#define CSL_CPU_IER0_LCD_SHIFT           (0x000Cu)
#define CSL_CPU_IER0_LCD_RESETVAL        (0x0000u)
/*----LCD Tokens----*/
#define CSL_CPU_IER0_LCD_DISABLE         (0x0000u)
#define CSL_CPU_IER0_LCD_ENABLE          (0x0001u)

#define CSL_CPU_IER0_PROG3_MASK          (0x0800u)
#define CSL_CPU_IER0_PROG3_SHIFT         (0x000Bu)
#define CSL_CPU_IER0_PROG3_RESETVAL      (0x0000u)
/*----PROG3 Tokens----*/
#define CSL_CPU_IER0_PROG3_DISABLE       (0x0000u)
#define CSL_CPU_IER0_PROG3_ENABLE        (0x0001u)


#define CSL_CPU_IER0_PROG2_MASK          (0x0200u)
#define CSL_CPU_IER0_PROG2_SHIFT         (0x0009u)
#define CSL_CPU_IER0_PROG2_RESETVAL      (0x0000u)
/*----PROG2 Tokens----*/
#define CSL_CPU_IER0_PROG2_DISABLE       (0x0000u)
#define CSL_CPU_IER0_PROG2_ENABLE        (0x0001u)

#define CSL_CPU_IER0_DMA_MASK            (0x0100u)
#define CSL_CPU_IER0_DMA_SHIFT           (0x0008u)
#define CSL_CPU_IER0_DMA_RESETVAL        (0x0000u)
/*----DMA Tokens----*/
#define CSL_CPU_IER0_DMA_DISABLE         (0x0000u)
#define CSL_CPU_IER0_DMA_ENABLE          (0x0001u)

#define CSL_CPU_IER0_PROG1_MASK          (0x0080u)
#define CSL_CPU_IER0_PROG1_SHIFT         (0x0007u)
#define CSL_CPU_IER0_PROG1_RESETVAL      (0x0000u)
/*----PROG1 Tokens----*/
#define CSL_CPU_IER0_PROG1_DISABLE       (0x0000u)
#define CSL_CPU_IER0_PROG1_ENABLE        (0x0001u)

#define CSL_CPU_IER0_UART_MASK           (0x0040u)
#define CSL_CPU_IER0_UART_SHIFT          (0x0006u)
#define CSL_CPU_IER0_UART_RESETVAL       (0x0000u)
/*----UART Tokens----*/
#define CSL_CPU_IER0_UART_DISABLE        (0x0000u)
#define CSL_CPU_IER0_UART_ENABLE         (0x0001u)

#define CSL_CPU_IER0_PROG0_MASK          (0x0020u)
#define CSL_CPU_IER0_PROG0_SHIFT         (0x0005u)
#define CSL_CPU_IER0_PROG0_RESETVAL      (0x0000u)
/*----PROG0 Tokens----*/
#define CSL_CPU_IER0_PROG0_DISABLE       (0x0000u)
#define CSL_CPU_IER0_PROG0_ENABLE        (0x0001u)

#define CSL_CPU_IER0_TINT_MASK           (0x0010u)
#define CSL_CPU_IER0_TINT_SHIFT          (0x0004u)
#define CSL_CPU_IER0_TINT_RESETVAL       (0x0000u)
/*----TINT Tokens----*/
#define CSL_CPU_IER0_TINT_DISABLE        (0x0000u)
#define CSL_CPU_IER0_TINT_ENABLE         (0x0001u)

#define CSL_CPU_IER0_INT1_MASK           (0x0008u)
#define CSL_CPU_IER0_INT1_SHIFT          (0x0003u)
#define CSL_CPU_IER0_INT1_RESETVAL       (0x0000u)
/*----INT1 Tokens----*/
#define CSL_CPU_IER0_INT1_DISABLE        (0x0000u)
#define CSL_CPU_IER0_INT1_ENABLE         (0x0001u)

#define CSL_CPU_IER0_INT0_MASK           (0x0004u)
#define CSL_CPU_IER0_INT0_SHIFT          (0x0002u)
#define CSL_CPU_IER0_INT0_RESETVAL       (0x0000u)
/*----INT0 Tokens----*/
#define CSL_CPU_IER0_INT0_DISABLE        (0x0000u)
#define CSL_CPU_IER0_INT0_ENABLE         (0x0001u)


#define CSL_CPU_IER0_RESETVAL            (0x0000u)

/* IFR0 */

#define CSL_CPU_IFR0_RCV2_MASK           (0x8000u)
#define CSL_CPU_IFR0_RCV2_SHIFT          (0x000Fu)
#define CSL_CPU_IFR0_RCV2_RESETVAL       (0x0000u)
/*----RCV2 Tokens----*/
#define CSL_CPU_IFR0_RCV2_DISABLED       (0x0000u)
#define CSL_CPU_IFR0_RCV2_ENABLED        (0x0001u)

#define CSL_CPU_IFR0_XMT2_MASK           (0x4000u)
#define CSL_CPU_IFR0_XMT2_SHIFT          (0x000Eu)
#define CSL_CPU_IFR0_XMT2_RESETVAL       (0x0000u)
/*----XMT2 Tokens----*/
#define CSL_CPU_IFR0_XMT2_DISABLED       (0x0000u)
#define CSL_CPU_IFR0_XMT2_ENABLED        (0x0001u)

#define CSL_CPU_IFR0_SAR_MASK            (0x2000u)
#define CSL_CPU_IFR0_SAR_SHIFT           (0x000Du)
#define CSL_CPU_IFR0_SAR_RESETVAL        (0x0000u)
/*----SAR Tokens----*/
#define CSL_CPU_IFR0_SAR_DISABLED        (0x0000u)
#define CSL_CPU_IFR0_SAR_ENABLED         (0x0001u)

#define CSL_CPU_IFR0_LCD_MASK            (0x1000u)
#define CSL_CPU_IFR0_LCD_SHIFT           (0x000Cu)
#define CSL_CPU_IFR0_LCD_RESETVAL        (0x0000u)
/*----LCD Tokens----*/
#define CSL_CPU_IFR0_LCD_DISABLED        (0x0000u)
#define CSL_CPU_IFR0_LCD_ENABLED         (0x0001u)

#define CSL_CPU_IFR0_PROG3_MASK          (0x0800u)
#define CSL_CPU_IFR0_PROG3_SHIFT         (0x000Bu)
#define CSL_CPU_IFR0_PROG3_RESETVAL      (0x0000u)
/*----PROG3 Tokens----*/
#define CSL_CPU_IFR0_PROG3_DISABLED      (0x0000u)
#define CSL_CPU_IFR0_PROG3_ENABLED       (0x0001u)


#define CSL_CPU_IFR0_PROG2_MASK          (0x0200u)
#define CSL_CPU_IFR0_PROG2_SHIFT         (0x0009u)
#define CSL_CPU_IFR0_PROG2_RESETVAL      (0x0000u)
/*----PROG2 Tokens----*/
#define CSL_CPU_IFR0_PROG2_DISABLED      (0x0000u)
#define CSL_CPU_IFR0_PROG2_ENABLED       (0x0001u)

#define CSL_CPU_IFR0_DMA_MASK            (0x0100u)
#define CSL_CPU_IFR0_DMA_SHIFT           (0x0008u)
#define CSL_CPU_IFR0_DMA_RESETVAL        (0x0000u)
/*----DMA Tokens----*/
#define CSL_CPU_IFR0_DMA_DISABLED        (0x0000u)
#define CSL_CPU_IFR0_DMA_ENABLED         (0x0001u)

#define CSL_CPU_IFR0_PROG1_MASK          (0x0080u)
#define CSL_CPU_IFR0_PROG1_SHIFT         (0x0007u)
#define CSL_CPU_IFR0_PROG1_RESETVAL      (0x0000u)
/*----PROG1 Tokens----*/
#define CSL_CPU_IFR0_PROG1_DISABLED      (0x0000u)
#define CSL_CPU_IFR0_PROG1_ENABLED       (0x0001u)

#define CSL_CPU_IFR0_UART_MASK           (0x0040u)
#define CSL_CPU_IFR0_UART_SHIFT          (0x0006u)
#define CSL_CPU_IFR0_UART_RESETVAL       (0x0000u)
/*----UART Tokens----*/
#define CSL_CPU_IFR0_UART_DISABLED       (0x0000u)
#define CSL_CPU_IFR0_UART_ENABLED        (0x0001u)

#define CSL_CPU_IFR0_PROG0_MASK          (0x0020u)
#define CSL_CPU_IFR0_PROG0_SHIFT         (0x0005u)
#define CSL_CPU_IFR0_PROG0_RESETVAL      (0x0000u)
/*----PROG0 Tokens----*/
#define CSL_CPU_IFR0_PROG0_DISABLED      (0x0000u)
#define CSL_CPU_IFR0_PROG0_ENABLED       (0x0001u)

#define CSL_CPU_IFR0_TINT_MASK           (0x0010u)
#define CSL_CPU_IFR0_TINT_SHIFT          (0x0004u)
#define CSL_CPU_IFR0_TINT_RESETVAL       (0x0000u)
/*----TINT Tokens----*/
#define CSL_CPU_IFR0_TINT_DISABLED       (0x0000u)
#define CSL_CPU_IFR0_TINT_ENABLED        (0x0001u)

#define CSL_CPU_IFR0_INT1_MASK           (0x0008u)
#define CSL_CPU_IFR0_INT1_SHIFT          (0x0003u)
#define CSL_CPU_IFR0_INT1_RESETVAL       (0x0000u)
/*----INT1 Tokens----*/
#define CSL_CPU_IFR0_INT1_DISABLED       (0x0000u)
#define CSL_CPU_IFR0_INT1_ENABLED        (0x0001u)

#define CSL_CPU_IFR0_INT0_MASK           (0x0004u)
#define CSL_CPU_IFR0_INT0_SHIFT          (0x0002u)
#define CSL_CPU_IFR0_INT0_RESETVAL       (0x0000u)
/*----INT0 Tokens----*/
#define CSL_CPU_IFR0_INT0_DISABLED       (0x0000u)
#define CSL_CPU_IFR0_INT0_ENABLED        (0x0001u)


#define CSL_CPU_IFR0_RESETVAL            (0x0000u)

/* ST0_55 */

#define CSL_CPU_ST0_55_ACOV2_MASK        (0x8000u)
#define CSL_CPU_ST0_55_ACOV2_SHIFT       (0x000Fu)
#define CSL_CPU_ST0_55_ACOV2_RESETVAL    (0x0000u)
/*----ACOV2 Tokens----*/
#define CSL_CPU_ST0_55_ACOV2_OFF         (0x0000u)
#define CSL_CPU_ST0_55_ACOV2_ON          (0x0001u)

#define CSL_CPU_ST0_55_ACOV3_MASK        (0x4000u)
#define CSL_CPU_ST0_55_ACOV3_SHIFT       (0x000Eu)
#define CSL_CPU_ST0_55_ACOV3_RESETVAL    (0x0000u)
/*----ACOV3 Tokens----*/
#define CSL_CPU_ST0_55_ACOV3_OFF         (0x0000u)
#define CSL_CPU_ST0_55_ACOV3_ON          (0x0001u)

#define CSL_CPU_ST0_55_TC1_MASK          (0x2000u)
#define CSL_CPU_ST0_55_TC1_SHIFT         (0x000Du)
#define CSL_CPU_ST0_55_TC1_RESETVAL      (0x0001u)
/*----TC1 Tokens----*/
#define CSL_CPU_ST0_55_TC1_OFF           (0x0000u)
#define CSL_CPU_ST0_55_TC1_ON            (0x0001u)

#define CSL_CPU_ST0_55_TC2_MASK          (0x1000u)
#define CSL_CPU_ST0_55_TC2_SHIFT         (0x000Cu)
#define CSL_CPU_ST0_55_TC2_RESETVAL      (0x0001u)
/*----TC2 Tokens----*/
#define CSL_CPU_ST0_55_TC2_OFF           (0x0000u)
#define CSL_CPU_ST0_55_TC2_ON            (0x0001u)

#define CSL_CPU_ST0_55_CARRY_MASK        (0x0800u)
#define CSL_CPU_ST0_55_CARRY_SHIFT       (0x000Bu)
#define CSL_CPU_ST0_55_CARRY_RESETVAL    (0x0001u)
/*----CARRY Tokens----*/
#define CSL_CPU_ST0_55_CARRY_OFF         (0x0000u)
#define CSL_CPU_ST0_55_CARRY_ON          (0x0001u)

#define CSL_CPU_ST0_55_ACOV0_MASK        (0x0400u)
#define CSL_CPU_ST0_55_ACOV0_SHIFT       (0x000Au)
#define CSL_CPU_ST0_55_ACOV0_RESETVAL    (0x0000u)
/*----ACOV0 Tokens----*/
#define CSL_CPU_ST0_55_ACOV0_OFF         (0x0000u)
#define CSL_CPU_ST0_55_ACOV0_ON          (0x0001u)

#define CSL_CPU_ST0_55_ACVO1_MASK        (0x0200u)
#define CSL_CPU_ST0_55_ACVO1_SHIFT       (0x0009u)
#define CSL_CPU_ST0_55_ACVO1_RESETVAL    (0x0000u)
/*----ACVO1 Tokens----*/
#define CSL_CPU_ST0_55_ACVO1_OFF         (0x0000u)
#define CSL_CPU_ST0_55_ACVO1_ON          (0x0001u)

#define CSL_CPU_ST0_55_DP_MASK           (0x01FFu)
#define CSL_CPU_ST0_55_DP_SHIFT          (0x0000u)
#define CSL_CPU_ST0_55_DP_RESETVAL       (0x0000u)

#define CSL_CPU_ST0_55_RESETVAL          (0x3800u)

/* ST1_55 */

#define CSL_CPU_ST1_55_BRAF_MASK         (0x8000u)
#define CSL_CPU_ST1_55_BRAF_SHIFT        (0x000Fu)
#define CSL_CPU_ST1_55_BRAF_RESETVAL     (0x0000u)
/*----BRAF Tokens----*/
#define CSL_CPU_ST1_55_BRAF_OFF          (0x0000u)
#define CSL_CPU_ST1_55_BRAF_ON           (0x0001u)

#define CSL_CPU_ST1_55_CPL_MASK          (0x4000u)
#define CSL_CPU_ST1_55_CPL_SHIFT         (0x000Eu)
#define CSL_CPU_ST1_55_CPL_RESETVAL      (0x0000u)
/*----CPL Tokens----*/
#define CSL_CPU_ST1_55_CPL_OFF           (0x0000u)
#define CSL_CPU_ST1_55_CPL_ON            (0x0001u)

#define CSL_CPU_ST1_55_XF_MASK           (0x2000u)
#define CSL_CPU_ST1_55_XF_SHIFT          (0x000Du)
#define CSL_CPU_ST1_55_XF_RESETVAL       (0x0001u)
/*----XF Tokens----*/
#define CSL_CPU_ST1_55_XF_OFF            (0x0000u)
#define CSL_CPU_ST1_55_XF_ON             (0x0001u)


#define CSL_CPU_ST1_55_INTM_MASK         (0x0800u)
#define CSL_CPU_ST1_55_INTM_SHIFT        (0x000Bu)
#define CSL_CPU_ST1_55_INTM_RESETVAL     (0x0001u)
/*----INTM Tokens----*/
#define CSL_CPU_ST1_55_INTM_OFF          (0x0000u)
#define CSL_CPU_ST1_55_INTM_ON           (0x0001u)

#define CSL_CPU_ST1_55_M40_MASK          (0x0400u)
#define CSL_CPU_ST1_55_M40_SHIFT         (0x000Au)
#define CSL_CPU_ST1_55_M40_RESETVAL      (0x0000u)
/*----M40 Tokens----*/
#define CSL_CPU_ST1_55_M40_OFF           (0x0000u)
#define CSL_CPU_ST1_55_M40_ON            (0x0001u)

#define CSL_CPU_ST1_55_SATD_MASK         (0x0200u)
#define CSL_CPU_ST1_55_SATD_SHIFT        (0x0009u)
#define CSL_CPU_ST1_55_SATD_RESETVAL     (0x0000u)
/*----SATD Tokens----*/
#define CSL_CPU_ST1_55_SATD_OFF          (0x0000u)
#define CSL_CPU_ST1_55_SATD_ON           (0x0001u)

#define CSL_CPU_ST1_55_SXMD_MASK         (0x0100u)
#define CSL_CPU_ST1_55_SXMD_SHIFT        (0x0008u)
#define CSL_CPU_ST1_55_SXMD_RESETVAL     (0x0001u)
/*----SXMD Tokens----*/
#define CSL_CPU_ST1_55_SXMD_OFF          (0x0000u)
#define CSL_CPU_ST1_55_SXMD_ON           (0x0001u)

#define CSL_CPU_ST1_55_C16_MASK          (0x0080u)
#define CSL_CPU_ST1_55_C16_SHIFT         (0x0007u)
#define CSL_CPU_ST1_55_C16_RESETVAL      (0x0000u)
/*----C16 Tokens----*/
#define CSL_CPU_ST1_55_C16_OFF           (0x0000u)
#define CSL_CPU_ST1_55_C16_ON            (0x0001u)

#define CSL_CPU_ST1_55_FRCT_MASK         (0x0040u)
#define CSL_CPU_ST1_55_FRCT_SHIFT        (0x0006u)
#define CSL_CPU_ST1_55_FRCT_RESETVAL     (0x0000u)
/*----FRCT Tokens----*/
#define CSL_CPU_ST1_55_FRCT_OFF          (0x0000u)
#define CSL_CPU_ST1_55_FRCT_ON           (0x0001u)

#define CSL_CPU_ST1_55_C54CM_MASK        (0x0020u)
#define CSL_CPU_ST1_55_C54CM_SHIFT       (0x0005u)
#define CSL_CPU_ST1_55_C54CM_RESETVAL    (0x0001u)
/*----C54CM Tokens----*/
#define CSL_CPU_ST1_55_C54CM_OFF         (0x0000u)
#define CSL_CPU_ST1_55_C54CM_ON          (0x0001u)

#define CSL_CPU_ST1_55_ASM_MASK          (0x001Fu)
#define CSL_CPU_ST1_55_ASM_SHIFT         (0x0000u)
#define CSL_CPU_ST1_55_ASM_RESETVAL      (0x0000u)

#define CSL_CPU_ST1_55_RESETVAL          (0x2920u)

/* ST3_55 */






#define CSL_CPU_ST3_55_CBERR_MASK        (0x0080u)
#define CSL_CPU_ST3_55_CBERR_SHIFT       (0x0007u)
#define CSL_CPU_ST3_55_CBERR_RESETVAL    (0x0000u)
/*----CBERR Tokens----*/
#define CSL_CPU_ST3_55_CBERR_OFF         (0x0000u)
#define CSL_CPU_ST3_55_CBERR_ON          (0x0001u)

#define CSL_CPU_ST3_55_MPNMC_MASK        (0x0040u)
#define CSL_CPU_ST3_55_MPNMC_SHIFT       (0x0006u)
#define CSL_CPU_ST3_55_MPNMC_RESETVAL    (0x0000u)
/*----MPNMC Tokens----*/
#define CSL_CPU_ST3_55_MPNMC_OFF         (0x0000u)
#define CSL_CPU_ST3_55_MPNMC_ON          (0x0001u)

#define CSL_CPU_ST3_55_SATA_MASK         (0x0020u)
#define CSL_CPU_ST3_55_SATA_SHIFT        (0x0005u)
#define CSL_CPU_ST3_55_SATA_RESETVAL     (0x0000u)
/*----SATA Tokens----*/
#define CSL_CPU_ST3_55_SATA_OFF          (0x0000u)
#define CSL_CPU_ST3_55_SATA_ON           (0x0001u)



#define CSL_CPU_ST3_55_CLKOFF_MASK       (0x0004u)
#define CSL_CPU_ST3_55_CLKOFF_SHIFT      (0x0002u)
#define CSL_CPU_ST3_55_CLKOFF_RESETVAL   (0x0000u)
/*----CLKOFF Tokens----*/
#define CSL_CPU_ST3_55_CLKOFF_ENABLE     (0x0000u)
#define CSL_CPU_ST3_55_CLKOFF_DISABLE    (0x0001u)

#define CSL_CPU_ST3_55_SMUL_MASK         (0x0002u)
#define CSL_CPU_ST3_55_SMUL_SHIFT        (0x0001u)
#define CSL_CPU_ST3_55_SMUL_RESETVAL     (0x0000u)
/*----SMUL Tokens----*/
#define CSL_CPU_ST3_55_SMUL_OFF          (0x0000u)
#define CSL_CPU_ST3_55_SMUL_ON           (0x0001u)

#define CSL_CPU_ST3_55_SST_MASK          (0x0001u)
#define CSL_CPU_ST3_55_SST_SHIFT         (0x0000u)
#define CSL_CPU_ST3_55_SST_RESETVAL      (0x0000u)
/*----SST Tokens----*/
#define CSL_CPU_ST3_55_SST_OFF           (0x0000u)
#define CSL_CPU_ST3_55_SST_ON            (0x0001u)

#define CSL_CPU_ST3_55_RESETVAL          (0x1000u)

/* IER1 */


#define CSL_CPU_IER1_RTOS_MASK           (0x0400u)
#define CSL_CPU_IER1_RTOS_SHIFT          (0x000Au)
#define CSL_CPU_IER1_RTOS_RESETVAL       (0x0000u)
/*----RTOS Tokens----*/
#define CSL_CPU_IER1_RTOS_DISABLE        (0x0000u)
#define CSL_CPU_IER1_RTOS_ENABLE         (0x0001u)

#define CSL_CPU_IER1_DLOG_MASK           (0x0200u)
#define CSL_CPU_IER1_DLOG_SHIFT          (0x0009u)
#define CSL_CPU_IER1_DLOG_RESETVAL       (0x0000u)
/*----DLOG Tokens----*/
#define CSL_CPU_IER1_DLOG_DISABLE        (0x0000u)
#define CSL_CPU_IER1_DLOG_ENABLE         (0x0001u)

#define CSL_CPU_IER1_BERR_MASK           (0x0100u)
#define CSL_CPU_IER1_BERR_SHIFT          (0x0008u)
#define CSL_CPU_IER1_BERR_RESETVAL       (0x0000u)
/*----BERR Tokens----*/
#define CSL_CPU_IER1_BERR_DISABLE        (0x0000u)
#define CSL_CPU_IER1_BERR_ENABLE         (0x0001u)

#define CSL_CPU_IER1_I2C_MASK            (0x0080u)
#define CSL_CPU_IER1_I2C_SHIFT           (0x0007u)
#define CSL_CPU_IER1_I2C_RESETVAL        (0x0000u)
/*----I2C Tokens----*/
#define CSL_CPU_IER1_I2C_DISABLE         (0x0000u)
#define CSL_CPU_IER1_I2C_ENABLE          (0x0001u)

#define CSL_CPU_IER1_EMIF_MASK           (0x0040u)
#define CSL_CPU_IER1_EMIF_SHIFT          (0x0006u)
#define CSL_CPU_IER1_EMIF_RESETVAL       (0x0000u)
/*----EMIF Tokens----*/
#define CSL_CPU_IER1_EMIF_DISABLE        (0x0000u)
#define CSL_CPU_IER1_EMIF_ENABLE         (0x0001u)

#define CSL_CPU_IER1_GPIO_MASK           (0x0020u)
#define CSL_CPU_IER1_GPIO_SHIFT          (0x0005u)
#define CSL_CPU_IER1_GPIO_RESETVAL       (0x0000u)
/*----GPIO Tokens----*/
#define CSL_CPU_IER1_GPIO_DISABLE        (0x0000u)
#define CSL_CPU_IER1_GPIO_ENABLE         (0x0001u)

#define CSL_CPU_IER1_USB_MASK            (0x0010u)
#define CSL_CPU_IER1_USB_SHIFT           (0x0004u)
#define CSL_CPU_IER1_USB_RESETVAL        (0x0000u)
/*----USB Tokens----*/
#define CSL_CPU_IER1_USB_DISABLE         (0x0000u)
#define CSL_CPU_IER1_USB_ENABLE          (0x0001u)

#define CSL_CPU_IER1_SPI_MASK            (0x0008u)
#define CSL_CPU_IER1_SPI_SHIFT           (0x0003u)
#define CSL_CPU_IER1_SPI_RESETVAL        (0x0000u)
/*----SPI Tokens----*/
#define CSL_CPU_IER1_SPI_DISABLE         (0x0000u)
#define CSL_CPU_IER1_SPI_ENABLE          (0x0001u)

#define CSL_CPU_IER1_RTC_MASK            (0x0004u)
#define CSL_CPU_IER1_RTC_SHIFT           (0x0002u)
#define CSL_CPU_IER1_RTC_RESETVAL        (0x0000u)
/*----RTC Tokens----*/
#define CSL_CPU_IER1_RTC_DISABLE         (0x0000u)
#define CSL_CPU_IER1_RTC_ENABLE          (0x0001u)

#define CSL_CPU_IER1_RCV3_MASK           (0x0002u)
#define CSL_CPU_IER1_RCV3_SHIFT          (0x0001u)
#define CSL_CPU_IER1_RCV3_RESETVAL       (0x0000u)
/*----RCV3 Tokens----*/
#define CSL_CPU_IER1_RCV3_DISABLE        (0x0000u)
#define CSL_CPU_IER1_RCV3_ENABLE         (0x0001u)

#define CSL_CPU_IER1_XMT3_MASK           (0x0001u)
#define CSL_CPU_IER1_XMT3_SHIFT          (0x0000u)
#define CSL_CPU_IER1_XMT3_RESETVAL       (0x0000u)
/*----XMT3 Tokens----*/
#define CSL_CPU_IER1_XMT3_DISABLE        (0x0000u)
#define CSL_CPU_IER1_XMT3_ENABLE         (0x0001u)

#define CSL_CPU_IER1_RESETVAL            (0x0000u)

/* IFR1 */


#define CSL_CPU_IFR1_RTOS_MASK           (0x0400u)
#define CSL_CPU_IFR1_RTOS_SHIFT          (0x000Au)
#define CSL_CPU_IFR1_RTOS_RESETVAL       (0x0000u)
/*----RTOS Tokens----*/
#define CSL_CPU_IFR1_RTOS_DISABLED       (0x0000u)
#define CSL_CPU_IFR1_RTOS_ENABLED        (0x0001u)

#define CSL_CPU_IFR1_DLOG_MASK           (0x0200u)
#define CSL_CPU_IFR1_DLOG_SHIFT          (0x0009u)
#define CSL_CPU_IFR1_DLOG_RESETVAL       (0x0000u)
/*----DLOG Tokens----*/
#define CSL_CPU_IFR1_DLOG_DISABLED       (0x0000u)
#define CSL_CPU_IFR1_DLOG_ENABLED        (0x0001u)

#define CSL_CPU_IFR1_BERR_MASK           (0x0100u)
#define CSL_CPU_IFR1_BERR_SHIFT          (0x0008u)
#define CSL_CPU_IFR1_BERR_RESETVAL       (0x0000u)
/*----BERR Tokens----*/
#define CSL_CPU_IFR1_BERR_DISABLED       (0x0000u)
#define CSL_CPU_IFR1_BERR_ENABLED        (0x0001u)

#define CSL_CPU_IFR1_I2C_MASK            (0x0080u)
#define CSL_CPU_IFR1_I2C_SHIFT           (0x0007u)
#define CSL_CPU_IFR1_I2C_RESETVAL        (0x0000u)
/*----I2C Tokens----*/
#define CSL_CPU_IFR1_I2C_DISABLED        (0x0000u)
#define CSL_CPU_IFR1_I2C_ENABLED         (0x0001u)

#define CSL_CPU_IFR1_EMIF_MASK           (0x0040u)
#define CSL_CPU_IFR1_EMIF_SHIFT          (0x0006u)
#define CSL_CPU_IFR1_EMIF_RESETVAL       (0x0000u)
/*----EMIF Tokens----*/
#define CSL_CPU_IFR1_EMIF_DISABLED       (0x0000u)
#define CSL_CPU_IFR1_EMIF_ENABLED        (0x0001u)

#define CSL_CPU_IFR1_GPIO_MASK           (0x0020u)
#define CSL_CPU_IFR1_GPIO_SHIFT          (0x0005u)
#define CSL_CPU_IFR1_GPIO_RESETVAL       (0x0000u)
/*----GPIO Tokens----*/
#define CSL_CPU_IFR1_GPIO_DISABLED       (0x0000u)
#define CSL_CPU_IFR1_GPIO_ENABLED        (0x0001u)

#define CSL_CPU_IFR1_USB_MASK            (0x0010u)
#define CSL_CPU_IFR1_USB_SHIFT           (0x0004u)
#define CSL_CPU_IFR1_USB_RESETVAL        (0x0000u)
/*----USB Tokens----*/
#define CSL_CPU_IFR1_USB_DISABLED        (0x0000u)
#define CSL_CPU_IFR1_USB_ENABLED         (0x0001u)

#define CSL_CPU_IFR1_SPI_MASK            (0x0008u)
#define CSL_CPU_IFR1_SPI_SHIFT           (0x0003u)
#define CSL_CPU_IFR1_SPI_RESETVAL        (0x0000u)
/*----SPI Tokens----*/
#define CSL_CPU_IFR1_SPI_DISABLED        (0x0000u)
#define CSL_CPU_IFR1_SPI_ENABLED         (0x0001u)

#define CSL_CPU_IFR1_RTC_MASK            (0x0004u)
#define CSL_CPU_IFR1_RTC_SHIFT           (0x0002u)
#define CSL_CPU_IFR1_RTC_RESETVAL        (0x0000u)
/*----RTC Tokens----*/
#define CSL_CPU_IFR1_RTC_DISABLED        (0x0000u)
#define CSL_CPU_IFR1_RTC_ENABLED         (0x0001u)

#define CSL_CPU_IFR1_RCV3_MASK           (0x0002u)
#define CSL_CPU_IFR1_RCV3_SHIFT          (0x0001u)
#define CSL_CPU_IFR1_RCV3_RESETVAL       (0x0000u)
/*----RCV3 Tokens----*/
#define CSL_CPU_IFR1_RCV3_DISABLED       (0x0000u)
#define CSL_CPU_IFR1_RCV3_ENABLED        (0x0001u)

#define CSL_CPU_IFR1_XMT3_MASK           (0x0001u)
#define CSL_CPU_IFR1_XMT3_SHIFT          (0x0000u)
#define CSL_CPU_IFR1_XMT3_RESETVAL       (0x0000u)
/*----XMT3 Tokens----*/
#define CSL_CPU_IFR1_XMT3_DISABLED       (0x0000u)
#define CSL_CPU_IFR1_XMT3_ENABLED        (0x0001u)

#define CSL_CPU_IFR1_RESETVAL            (0x0000u)

/* IVPD */

#define CSL_CPU_IVPD_IVPD_MASK           (0xFFFFu)
#define CSL_CPU_IVPD_IVPD_SHIFT          (0x0000u)
#define CSL_CPU_IVPD_IVPD_RESETVAL       (0xFFFFu)

#define CSL_CPU_IVPD_RESETVAL            (0xFFFFu)

/* IVPH */

#define CSL_CPU_IVPH_IVPH_MASK           (0xFFFFu)
#define CSL_CPU_IVPH_IVPH_SHIFT          (0x0000u)
#define CSL_CPU_IVPH_IVPH_RESETVAL       (0xFFFFu)

#define CSL_CPU_IVPH_RESETVAL            (0xFFFFu)

/* ST2_55 */

#define CSL_CPU_ST2_55_ARMS_MASK         (0x8000u)
#define CSL_CPU_ST2_55_ARMS_SHIFT        (0x000Fu)
#define CSL_CPU_ST2_55_ARMS_RESETVAL     (0x0000u)
/*----ARMS Tokens----*/
#define CSL_CPU_ST2_55_ARMS_OFF          (0x0000u)
#define CSL_CPU_ST2_55_ARMS_ON           (0x0001u)


#define CSL_CPU_ST2_55_DBGM_MASK         (0x1000u)
#define CSL_CPU_ST2_55_DBGM_SHIFT        (0x000Cu)
#define CSL_CPU_ST2_55_DBGM_RESETVAL     (0x0001u)
/*----DBGM Tokens----*/
#define CSL_CPU_ST2_55_DBGM_OFF          (0x0000u)
#define CSL_CPU_ST2_55_DBGM_ON           (0x0001u)

#define CSL_CPU_ST2_55_EALLOW_MASK       (0x0800u)
#define CSL_CPU_ST2_55_EALLOW_SHIFT      (0x000Bu)
#define CSL_CPU_ST2_55_EALLOW_RESETVAL   (0x0000u)
/*----EALLOW Tokens----*/
#define CSL_CPU_ST2_55_EALLOW_OFF        (0x0000u)
#define CSL_CPU_ST2_55_EALLOW_ON         (0x0001u)

#define CSL_CPU_ST2_55_RDM_MASK          (0x0400u)
#define CSL_CPU_ST2_55_RDM_SHIFT         (0x000Au)
#define CSL_CPU_ST2_55_RDM_RESETVAL      (0x0000u)
/*----RDM Tokens----*/
#define CSL_CPU_ST2_55_RDM_OFF           (0x0000u)
#define CSL_CPU_ST2_55_RDM_ON            (0x0001u)


#define CSL_CPU_ST2_55_CDPLC_MASK        (0x0100u)
#define CSL_CPU_ST2_55_CDPLC_SHIFT       (0x0008u)
#define CSL_CPU_ST2_55_CDPLC_RESETVAL    (0x0000u)
/*----CDPLC Tokens----*/
#define CSL_CPU_ST2_55_CDPLC_OFF         (0x0000u)
#define CSL_CPU_ST2_55_CDPLC_ON          (0x0001u)

#define CSL_CPU_ST2_55_AR7LC_MASK        (0x0080u)
#define CSL_CPU_ST2_55_AR7LC_SHIFT       (0x0007u)
#define CSL_CPU_ST2_55_AR7LC_RESETVAL    (0x0000u)
/*----AR7LC Tokens----*/
#define CSL_CPU_ST2_55_AR7LC_OFF         (0x0000u)
#define CSL_CPU_ST2_55_AR7LC_ON          (0x0001u)

#define CSL_CPU_ST2_55_AR6LC_MASK        (0x0040u)
#define CSL_CPU_ST2_55_AR6LC_SHIFT       (0x0006u)
#define CSL_CPU_ST2_55_AR6LC_RESETVAL    (0x0000u)
/*----AR6LC Tokens----*/
#define CSL_CPU_ST2_55_AR6LC_OFF         (0x0000u)
#define CSL_CPU_ST2_55_AR6LC_ON          (0x0001u)

#define CSL_CPU_ST2_55_AR5LC_MASK        (0x0020u)
#define CSL_CPU_ST2_55_AR5LC_SHIFT       (0x0005u)
#define CSL_CPU_ST2_55_AR5LC_RESETVAL    (0x0000u)
/*----AR5LC Tokens----*/
#define CSL_CPU_ST2_55_AR5LC_OFF         (0x0000u)
#define CSL_CPU_ST2_55_AR5LC_ON          (0x0001u)

#define CSL_CPU_ST2_55_AR4LC_MASK        (0x0010u)
#define CSL_CPU_ST2_55_AR4LC_SHIFT       (0x0004u)
#define CSL_CPU_ST2_55_AR4LC_RESETVAL    (0x0000u)
/*----AR4LC Tokens----*/
#define CSL_CPU_ST2_55_AR4LC_OFF         (0x0000u)
#define CSL_CPU_ST2_55_AR4LC_ON          (0x0001u)

#define CSL_CPU_ST2_55_AR3LC_MASK        (0x0008u)
#define CSL_CPU_ST2_55_AR3LC_SHIFT       (0x0003u)
#define CSL_CPU_ST2_55_AR3LC_RESETVAL    (0x0000u)
/*----AR3LC Tokens----*/
#define CSL_CPU_ST2_55_AR3LC_OFF         (0x0000u)
#define CSL_CPU_ST2_55_AR3LC_ON          (0x0001u)

#define CSL_CPU_ST2_55_AR2LC_MASK        (0x0004u)
#define CSL_CPU_ST2_55_AR2LC_SHIFT       (0x0002u)
#define CSL_CPU_ST2_55_AR2LC_RESETVAL    (0x0000u)
/*----AR2LC Tokens----*/
#define CSL_CPU_ST2_55_AR2LC_OFF         (0x0000u)
#define CSL_CPU_ST2_55_AR2LC_ON          (0x0001u)

#define CSL_CPU_ST2_55_AR1LC_MASK        (0x0002u)
#define CSL_CPU_ST2_55_AR1LC_SHIFT       (0x0001u)
#define CSL_CPU_ST2_55_AR1LC_RESETVAL    (0x0000u)
/*----AR1LC Tokens----*/
#define CSL_CPU_ST2_55_AR1LC_OFF         (0x0000u)
#define CSL_CPU_ST2_55_AR1LC_ON          (0x0001u)

#define CSL_CPU_ST2_55_AR0LC_MASK        (0x0001u)
#define CSL_CPU_ST2_55_AR0LC_SHIFT       (0x0000u)
#define CSL_CPU_ST2_55_AR0LC_RESETVAL    (0x0000u)
/*----AR0LC Tokens----*/
#define CSL_CPU_ST2_55_AR0LC_OFF         (0x0000u)
#define CSL_CPU_ST2_55_AR0LC_ON          (0x0001u)

#define CSL_CPU_ST2_55_RESETVAL          (0x7000u)

#endif
