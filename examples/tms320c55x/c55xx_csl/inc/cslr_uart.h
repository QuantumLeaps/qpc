/********************************************************************
* Copyright (C) 2003-2008 Texas Instruments Incorporated.
* All Rights Reserved
*********************************************************************
* file: cslr_uart.h
*
* Brief: This file contains the Register Description for uart
*
*********************************************************************/
#ifndef _CSLR_UART_H_
#define _CSLR_UART_H_

#include <cslr.h>

#include <tistdtypes.h>
#include <csl_general.h>


/* Minimum unit = 2 bytes */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 THR;
    volatile Uint16 RSVD0;
    volatile Uint16 IER;
    volatile Uint16 RSVD1;
    volatile Uint16 FCR;
    volatile Uint16 RSVD2;
    volatile Uint16 LCR;
    volatile Uint16 RSVD3;
    volatile Uint16 MCR;
    volatile Uint16 RSVD4;
    volatile Uint16 LSR;
    volatile Uint16 RSVD5[3];
    volatile Uint16 SCR;
    volatile Uint16 RSVD6;
    volatile Uint16 DLL;
    volatile Uint16 RSVD7;
    volatile Uint16 DLH;
    volatile Uint16 RSVD8[5];
    volatile Uint16 PWREMU_MGMT;
} CSL_UartRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* THR */


#define CSL_UART_THR_DATA_MASK           (0x00FFu)
#define CSL_UART_THR_DATA_SHIFT          (0x0000u)
#define CSL_UART_THR_DATA_RESETVAL       (0x0000u)

#define CSL_UART_THR_RESETVAL            (0x0000u)

/* RBR */


#define CSL_UART_RBR_DATA_MASK           (0x00FFu)
#define CSL_UART_RBR_DATA_SHIFT          (0x0000u)
#define CSL_UART_RBR_DATA_RESETVAL       (0x0000u)

#define CSL_UART_RBR_RESETVAL            (0x0000u)

/* IER */



#define CSL_UART_IER_ELSI_MASK           (0x0004u)
#define CSL_UART_IER_ELSI_SHIFT          (0x0002u)
#define CSL_UART_IER_ELSI_RESETVAL       (0x0000u)
/*----ELSI Tokens----*/
#define CSL_UART_IER_ELSI_DISABLE        (0x0000u)
#define CSL_UART_IER_ELSI_ENABLE         (0x0001u)

#define CSL_UART_IER_ETBEI_MASK          (0x0002u)
#define CSL_UART_IER_ETBEI_SHIFT         (0x0001u)
#define CSL_UART_IER_ETBEI_RESETVAL      (0x0000u)
/*----ETBEI Tokens----*/
#define CSL_UART_IER_ETBEI_DISABLE       (0x0000u)
#define CSL_UART_IER_ETBEI_ENABLE        (0x0001u)

#define CSL_UART_IER_ERBI_MASK           (0x0001u)
#define CSL_UART_IER_ERBI_SHIFT          (0x0000u)
#define CSL_UART_IER_ERBI_RESETVAL       (0x0000u)
/*----ERBI Tokens----*/
#define CSL_UART_IER_ERBI_DISABLE        (0x0000u)
#define CSL_UART_IER_ERBI_ENABLE         (0x0001u)

#define CSL_UART_IER_RESETVAL            (0x0000u)

/* FCR */


#define CSL_UART_FCR_RXFIFTL_MASK        (0x00C0u)
#define CSL_UART_FCR_RXFIFTL_SHIFT       (0x0006u)
#define CSL_UART_FCR_RXFIFTL_RESETVAL    (0x0000u)
/*----RXFIFTL Tokens----*/
#define CSL_UART_FCR_RXFIFTL_CHAR1       (0x0000u)
#define CSL_UART_FCR_RXFIFTL_CHAR4       (0x0001u)
#define CSL_UART_FCR_RXFIFTL_CHAR8       (0x0002u)
#define CSL_UART_FCR_RXFIFTL_CHAR14      (0x0003u)


#define CSL_UART_FCR_DMAMODE1_MASK       (0x0008u)
#define CSL_UART_FCR_DMAMODE1_SHIFT      (0x0003u)
#define CSL_UART_FCR_DMAMODE1_RESETVAL   (0x0000u)
/*----DMAMODE1 Tokens----*/
#define CSL_UART_FCR_DMAMODE1_DISABLE    (0x0000u)
#define CSL_UART_FCR_DMAMODE1_ENABLE     (0x0001u)

#define CSL_UART_FCR_TXCLR_MASK          (0x0004u)
#define CSL_UART_FCR_TXCLR_SHIFT         (0x0002u)
#define CSL_UART_FCR_TXCLR_RESETVAL      (0x0000u)
/*----TXCLR Tokens----*/
#define CSL_UART_FCR_TXCLR_CLR           (0x0001u)

#define CSL_UART_FCR_RXCLR_MASK          (0x0002u)
#define CSL_UART_FCR_RXCLR_SHIFT         (0x0001u)
#define CSL_UART_FCR_RXCLR_RESETVAL      (0x0000u)
/*----RXCLR Tokens----*/
#define CSL_UART_FCR_RXCLR_CLR           (0x0001u)

#define CSL_UART_FCR_FIFOEN_MASK         (0x0001u)
#define CSL_UART_FCR_FIFOEN_SHIFT        (0x0000u)
#define CSL_UART_FCR_FIFOEN_RESETVAL     (0x0000u)
/*----FIFOEN Tokens----*/
#define CSL_UART_FCR_FIFOEN_DISABLE      (0x0000u)
#define CSL_UART_FCR_FIFOEN_ENABLE       (0x0001u)

#define CSL_UART_FCR_RESETVAL            (0x0000u)

/* IIR */


#define CSL_UART_IIR_FIFOEN_MASK         (0x00C0u)
#define CSL_UART_IIR_FIFOEN_SHIFT        (0x0006u)
#define CSL_UART_IIR_FIFOEN_RESETVAL     (0x0000u)
/*----FIFOEN Tokens----*/
#define CSL_UART_IIR_FIFOEN_DISABLED     (0x0000u)
#define CSL_UART_IIR_FIFOEN_ENABLED      (0x0001u)


#define CSL_UART_IIR_INTID_MASK          (0x000Eu)
#define CSL_UART_IIR_INTID_SHIFT         (0x0001u)
#define CSL_UART_IIR_INTID_RESETVAL      (0x0000u)
/*----INTID Tokens----*/
#define CSL_UART_IIR_INTID_RSV0          (0x0000u)
#define CSL_UART_IIR_INTID_THRE          (0x0001u)
#define CSL_UART_IIR_INTID_RDA           (0x0002u)
#define CSL_UART_IIR_INTID_RLS           (0x0003u)
#define CSL_UART_IIR_INTID_RSV4          (0x0004u)
#define CSL_UART_IIR_INTID_RSV5          (0x0005u)
#define CSL_UART_IIR_INTID_CTI           (0x0006u)
#define CSL_UART_IIR_INTID_RSV7          (0x0007u)

#define CSL_UART_IIR_IPEND_MASK          (0x0001u)
#define CSL_UART_IIR_IPEND_SHIFT         (0x0000u)
#define CSL_UART_IIR_IPEND_RESETVAL      (0x0001u)
/*----IPEND Tokens----*/
#define CSL_UART_IIR_IPEND_PENDING       (0x0000u)
#define CSL_UART_IIR_IPEND_NONE          (0x0001u)

#define CSL_UART_IIR_RESETVAL            (0x0001u)

/* LCR */


#define CSL_UART_LCR_DLAB_MASK           (0x0080u)
#define CSL_UART_LCR_DLAB_SHIFT          (0x0007u)
#define CSL_UART_LCR_DLAB_RESETVAL       (0x0000u)
/*----DLAB Tokens----*/
#define CSL_UART_LCR_DLAB_DLABON         (0x0000u)
#define CSL_UART_LCR_DLAB_DLABOFF        (0x0001u)

#define CSL_UART_LCR_BC_MASK             (0x0040u)
#define CSL_UART_LCR_BC_SHIFT            (0x0006u)
#define CSL_UART_LCR_BC_RESETVAL         (0x0000u)
/*----BC Tokens----*/
#define CSL_UART_LCR_BC_DISABLE          (0x0000u)
#define CSL_UART_LCR_BC_ENABLE           (0x0001u)

#define CSL_UART_LCR_SP_MASK             (0x0020u)
#define CSL_UART_LCR_SP_SHIFT            (0x0005u)
#define CSL_UART_LCR_SP_RESETVAL         (0x0000u)
/*----SP Tokens----*/
#define CSL_UART_LCR_SP_DISABLE          (0x0000u)
#define CSL_UART_LCR_SP_ENABLE           (0x0001u)

#define CSL_UART_LCR_EPS_MASK            (0x0010u)
#define CSL_UART_LCR_EPS_SHIFT           (0x0004u)
#define CSL_UART_LCR_EPS_RESETVAL        (0x0000u)
/*----EPS Tokens----*/
#define CSL_UART_LCR_EPS_ODD             (0x0000u)
#define CSL_UART_LCR_EPS_EVEN            (0x0001u)

#define CSL_UART_LCR_PEN_MASK            (0x0008u)
#define CSL_UART_LCR_PEN_SHIFT           (0x0003u)
#define CSL_UART_LCR_PEN_RESETVAL        (0x0000u)
/*----PEN Tokens----*/
#define CSL_UART_LCR_PEN_DISABLE         (0x0000u)
#define CSL_UART_LCR_PEN_ENABLE          (0x0001u)

#define CSL_UART_LCR_STB_MASK            (0x0004u)
#define CSL_UART_LCR_STB_SHIFT           (0x0002u)
#define CSL_UART_LCR_STB_RESETVAL        (0x0000u)
/*----STB Tokens----*/
#define CSL_UART_LCR_STB_1STOPBIT        (0x0000u)
#define CSL_UART_LCR_STB_MULTSTOPBIT     (0x0001u)

#define CSL_UART_LCR_WLS_MASK            (0x0003u)
#define CSL_UART_LCR_WLS_SHIFT           (0x0000u)
#define CSL_UART_LCR_WLS_RESETVAL        (0x0000u)
/*----WLS Tokens----*/
#define CSL_UART_LCR_WLS_BITS5           (0x0000u)
#define CSL_UART_LCR_WLS_BITS6           (0x0001u)
#define CSL_UART_LCR_WLS_BITS7           (0x0002u)
#define CSL_UART_LCR_WLS_BITS8           (0x0003u)

#define CSL_UART_LCR_RESETVAL            (0x0000u)

/* MCR */


#define CSL_UART_MCR_AFE_MASK            (0x0020u)
#define CSL_UART_MCR_AFE_SHIFT           (0x0005u)
#define CSL_UART_MCR_AFE_RESETVAL        (0x0000u)
/*----AFE Tokens----*/
#define CSL_UART_MCR_AFE_DISABLE         (0x0000u)
#define CSL_UART_MCR_AFE_ENABLE          (0x0001u)

#define CSL_UART_MCR_LOOP_MASK           (0x0010u)
#define CSL_UART_MCR_LOOP_SHIFT          (0x0004u)
#define CSL_UART_MCR_LOOP_RESETVAL       (0x0000u)
/*----LOOP Tokens----*/
#define CSL_UART_MCR_LOOP_DISABLE        (0x0000u)
#define CSL_UART_MCR_LOOP_ENABLE         (0x0001u)


#define CSL_UART_MCR_RTS_MASK            (0x0002u)
#define CSL_UART_MCR_RTS_SHIFT           (0x0001u)
#define CSL_UART_MCR_RTS_RESETVAL        (0x0000u)
/*----RTS Tokens----*/
#define CSL_UART_MCR_RTS_CTSEN           (0x0000u)
#define CSL_UART_MCR_RTS_CTSRTSEN        (0x0001u)


#define CSL_UART_MCR_RESETVAL            (0x0000u)

/* LSR */


#define CSL_UART_LSR_RXFIFOE_MASK        (0x0080u)
#define CSL_UART_LSR_RXFIFOE_SHIFT       (0x0007u)
#define CSL_UART_LSR_RXFIFOE_RESETVAL    (0x0000u)

#define CSL_UART_LSR_TEMT_MASK           (0x0040u)
#define CSL_UART_LSR_TEMT_SHIFT          (0x0006u)
#define CSL_UART_LSR_TEMT_RESETVAL       (0x0001u)

#define CSL_UART_LSR_THRE_MASK           (0x0020u)
#define CSL_UART_LSR_THRE_SHIFT          (0x0005u)
#define CSL_UART_LSR_THRE_RESETVAL       (0x0001u)

#define CSL_UART_LSR_BI_MASK             (0x0010u)
#define CSL_UART_LSR_BI_SHIFT            (0x0004u)
#define CSL_UART_LSR_BI_RESETVAL         (0x0000u)

#define CSL_UART_LSR_FE_MASK             (0x0008u)
#define CSL_UART_LSR_FE_SHIFT            (0x0003u)
#define CSL_UART_LSR_FE_RESETVAL         (0x0000u)

#define CSL_UART_LSR_PE_MASK             (0x0004u)
#define CSL_UART_LSR_PE_SHIFT            (0x0002u)
#define CSL_UART_LSR_PE_RESETVAL         (0x0000u)

#define CSL_UART_LSR_OE_MASK             (0x0002u)
#define CSL_UART_LSR_OE_SHIFT            (0x0001u)
#define CSL_UART_LSR_OE_RESETVAL         (0x0000u)

#define CSL_UART_LSR_DR_MASK             (0x0001u)
#define CSL_UART_LSR_DR_SHIFT            (0x0000u)
#define CSL_UART_LSR_DR_RESETVAL         (0x0000u)

#define CSL_UART_LSR_RESETVAL            (0x0060u)

/* SCR */


#define CSL_UART_SCR_DATA_MASK           (0x00FFu)
#define CSL_UART_SCR_DATA_SHIFT          (0x0000u)
#define CSL_UART_SCR_DATA_RESETVAL       (0x0000u)

#define CSL_UART_SCR_RESETVAL            (0x0000u)

/* DLL */


#define CSL_UART_DLL_DLL_MASK            (0x00FFu)
#define CSL_UART_DLL_DLL_SHIFT           (0x0000u)
#define CSL_UART_DLL_DLL_RESETVAL        (0x0000u)

#define CSL_UART_DLL_RESETVAL            (0x0000u)

/* DLH */


#define CSL_UART_DLH_DLH_MASK            (0x00FFu)
#define CSL_UART_DLH_DLH_SHIFT           (0x0000u)
#define CSL_UART_DLH_DLH_RESETVAL        (0x0000u)

#define CSL_UART_DLH_RESETVAL            (0x0000u)

/* PWREMU_MGMT */


#define CSL_UART_PWREMU_MGMT_UTRST_MASK  (0x4000u)
#define CSL_UART_PWREMU_MGMT_UTRST_SHIFT (0x000Eu)
#define CSL_UART_PWREMU_MGMT_UTRST_RESETVAL (0x0000u)
/*----UTRST Tokens----*/
#define CSL_UART_PWREMU_MGMT_UTRST_RESET (0x0000u)
#define CSL_UART_PWREMU_MGMT_UTRST_ENABLE (0x0001u)

#define CSL_UART_PWREMU_MGMT_URRST_MASK  (0x2000u)
#define CSL_UART_PWREMU_MGMT_URRST_SHIFT (0x000Du)
#define CSL_UART_PWREMU_MGMT_URRST_RESETVAL (0x0000u)
/*----URRST Tokens----*/
#define CSL_UART_PWREMU_MGMT_URRST_RESET (0x0000u)
#define CSL_UART_PWREMU_MGMT_URRST_ENABLE (0x0001u)


#define CSL_UART_PWREMU_MGMT_FREE_MASK   (0x0001u)
#define CSL_UART_PWREMU_MGMT_FREE_SHIFT  (0x0000u)
#define CSL_UART_PWREMU_MGMT_FREE_RESETVAL (0x0000u)
/*----FREE Tokens----*/
#define CSL_UART_PWREMU_MGMT_FREE_STOP   (0x0000u)
#define CSL_UART_PWREMU_MGMT_FREE_RUN    (0x0001u)

#define CSL_UART_PWREMU_MGMT_RESETVAL    (0x0000u)

#endif
