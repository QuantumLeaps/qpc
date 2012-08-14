/********************************************************************
* Copyright (C) 2003-2008 Texas Instruments Incorporated.
* All Rights Reserved
*********************************************************************
* file: cslr_i2c.h
*
* Brief: This file contains the Register Description for i2c
*
*********************************************************************/
#ifndef _CSLR_I2C_H_
#define _CSLR_I2C_H_

#include <cslr.h>

#include <tistdtypes.h>
#include <csl_general.h>


/* Minimum unit = 2 bytes */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 ICOAR;
    volatile Uint16 RSVD0[3];
    volatile Uint16 ICIMR;
    volatile Uint16 RSVD1[3];
    volatile Uint16 ICSTR;
    volatile Uint16 RSVD2[3];
    volatile Uint16 ICCLKL;
    volatile Uint16 RSVD3[3];
    volatile Uint16 ICCLKH;
    volatile Uint16 RSVD4[3];
    volatile Uint16 ICCNT;
    volatile Uint16 RSVD5[3];
    volatile Uint16 ICDRR;
    volatile Uint16 RSVD6[3];
    volatile Uint16 ICSAR;
    volatile Uint16 RSVD7[3];
    volatile Uint16 ICDXR;
    volatile Uint16 RSVD8[3];
    volatile Uint16 ICMDR;
    volatile Uint16 RSVD9[3];
    volatile Uint16 ICIVR;
    volatile Uint16 RSVD10[3];
    volatile Uint16 ICEMDR;
    volatile Uint16 RSVD11[3];
    volatile Uint16 ICPSC;
    volatile Uint16 RSVD12[3];
    volatile Uint16 ICPID1;
    volatile Uint16 RSVD13[3];
    volatile Uint16 ICPID2;
} CSL_I2cRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* ICOAR */


#define CSL_I2C_ICOAR_OADDR_MASK         (0x03FFu)
#define CSL_I2C_ICOAR_OADDR_SHIFT        (0x0000u)
#define CSL_I2C_ICOAR_OADDR_RESETVAL     (0x0000u)

#define CSL_I2C_ICOAR_RESETVAL           (0x0000u)

/* ICIMR */


#define CSL_I2C_ICIMR_AAS_MASK           (0x0040u)
#define CSL_I2C_ICIMR_AAS_SHIFT          (0x0006u)
#define CSL_I2C_ICIMR_AAS_RESETVAL       (0x0000u)
/*----AAS Tokens----*/
#define CSL_I2C_ICIMR_AAS_DISABLE        (0x0000u)
#define CSL_I2C_ICIMR_AAS_ENABLE         (0x0001u)

#define CSL_I2C_ICIMR_SCD_MASK           (0x0020u)
#define CSL_I2C_ICIMR_SCD_SHIFT          (0x0005u)
#define CSL_I2C_ICIMR_SCD_RESETVAL       (0x0000u)
/*----SCD Tokens----*/
#define CSL_I2C_ICIMR_SCD_DISABLE        (0x0000u)
#define CSL_I2C_ICIMR_SCD_ENABLE         (0x0001u)

#define CSL_I2C_ICIMR_ICXRDY_MASK        (0x0010u)
#define CSL_I2C_ICIMR_ICXRDY_SHIFT       (0x0004u)
#define CSL_I2C_ICIMR_ICXRDY_RESETVAL    (0x0000u)
/*----ICXRDY Tokens----*/
#define CSL_I2C_ICIMR_ICXRDY_DISABLE     (0x0000u)
#define CSL_I2C_ICIMR_ICXRDY_ENABLE      (0x0001u)

#define CSL_I2C_ICIMR_ICRRDY_MASK        (0x0008u)
#define CSL_I2C_ICIMR_ICRRDY_SHIFT       (0x0003u)
#define CSL_I2C_ICIMR_ICRRDY_RESETVAL    (0x0000u)
/*----ICRRDY Tokens----*/
#define CSL_I2C_ICIMR_ICRRDY_DISABLE     (0x0000u)
#define CSL_I2C_ICIMR_ICRRDY_ENABLE      (0x0001u)

#define CSL_I2C_ICIMR_ARDY_MASK          (0x0004u)
#define CSL_I2C_ICIMR_ARDY_SHIFT         (0x0002u)
#define CSL_I2C_ICIMR_ARDY_RESETVAL      (0x0000u)
/*----ARDY Tokens----*/
#define CSL_I2C_ICIMR_ARDY_DISABLE       (0x0000u)
#define CSL_I2C_ICIMR_ARDY_ENABLE        (0x0001u)

#define CSL_I2C_ICIMR_NACK_MASK          (0x0002u)
#define CSL_I2C_ICIMR_NACK_SHIFT         (0x0001u)
#define CSL_I2C_ICIMR_NACK_RESETVAL      (0x0000u)
/*----NACK Tokens----*/
#define CSL_I2C_ICIMR_NACK_DISABLE       (0x0000u)
#define CSL_I2C_ICIMR_NACK_ENABLE        (0x0001u)

#define CSL_I2C_ICIMR_AL_MASK            (0x0001u)
#define CSL_I2C_ICIMR_AL_SHIFT           (0x0000u)
#define CSL_I2C_ICIMR_AL_RESETVAL        (0x0000u)
/*----AL Tokens----*/
#define CSL_I2C_ICIMR_AL_DISABLE         (0x0000u)
#define CSL_I2C_ICIMR_AL_ENABLE          (0x0001u)

#define CSL_I2C_ICIMR_RESETVAL           (0x0000u)

/* ICSTR */


#define CSL_I2C_ICSTR_SDIR_MASK          (0x4000u)
#define CSL_I2C_ICSTR_SDIR_SHIFT         (0x000Eu)
#define CSL_I2C_ICSTR_SDIR_RESETVAL      (0x0000u)
/*----SDIR Tokens----*/
#define CSL_I2C_ICSTR_SDIR_CLEAR         (0x0000u)
#define CSL_I2C_ICSTR_SDIR_SET           (0x0001u)

#define CSL_I2C_ICSTR_NACKSNT_MASK       (0x2000u)
#define CSL_I2C_ICSTR_NACKSNT_SHIFT      (0x000Du)
#define CSL_I2C_ICSTR_NACKSNT_RESETVAL   (0x0000u)
/*----NACKSNT Tokens----*/
#define CSL_I2C_ICSTR_NACKSNT_CLEAR      (0x0000u)
#define CSL_I2C_ICSTR_NACKSNT_SET        (0x0001u)

#define CSL_I2C_ICSTR_BB_MASK            (0x1000u)
#define CSL_I2C_ICSTR_BB_SHIFT           (0x000Cu)
#define CSL_I2C_ICSTR_BB_RESETVAL        (0x0000u)
/*----BB Tokens----*/
#define CSL_I2C_ICSTR_BB_CLEAR           (0x0000u)
#define CSL_I2C_ICSTR_BB_SET             (0x0001u)

#define CSL_I2C_ICSTR_RSFULL_MASK        (0x0800u)
#define CSL_I2C_ICSTR_RSFULL_SHIFT       (0x000Bu)
#define CSL_I2C_ICSTR_RSFULL_RESETVAL    (0x0000u)
/*----RSFULL Tokens----*/
#define CSL_I2C_ICSTR_RSFULL_CLEAR       (0x0000u)
#define CSL_I2C_ICSTR_RSFULL_SET         (0x0001u)

#define CSL_I2C_ICSTR_XSMT_MASK          (0x0400u)
#define CSL_I2C_ICSTR_XSMT_SHIFT         (0x000Au)
#define CSL_I2C_ICSTR_XSMT_RESETVAL      (0x0001u)
/*----XSMT Tokens----*/
#define CSL_I2C_ICSTR_XSMT_CLEAR         (0x0000u)
#define CSL_I2C_ICSTR_XSMT_SET           (0x0001u)

#define CSL_I2C_ICSTR_AAS_MASK           (0x0200u)
#define CSL_I2C_ICSTR_AAS_SHIFT          (0x0009u)
#define CSL_I2C_ICSTR_AAS_RESETVAL       (0x0000u)
/*----AAS Tokens----*/
#define CSL_I2C_ICSTR_AAS_CLEAR          (0x0000u)
#define CSL_I2C_ICSTR_AAS_SET            (0x0001u)

#define CSL_I2C_ICSTR_AD0_MASK           (0x0100u)
#define CSL_I2C_ICSTR_AD0_SHIFT          (0x0008u)
#define CSL_I2C_ICSTR_AD0_RESETVAL       (0x0000u)
/*----AD0 Tokens----*/
#define CSL_I2C_ICSTR_AD0_CLEAR          (0x0000u)
#define CSL_I2C_ICSTR_AD0_SET            (0x0001u)


#define CSL_I2C_ICSTR_SCD_MASK           (0x0020u)
#define CSL_I2C_ICSTR_SCD_SHIFT          (0x0005u)
#define CSL_I2C_ICSTR_SCD_RESETVAL       (0x0000u)
/*----SCD Tokens----*/
#define CSL_I2C_ICSTR_SCD_CLEAR          (0x0000u)
#define CSL_I2C_ICSTR_SCD_SET            (0x0001u)

#define CSL_I2C_ICSTR_ICXRDY_MASK        (0x0010u)
#define CSL_I2C_ICSTR_ICXRDY_SHIFT       (0x0004u)
#define CSL_I2C_ICSTR_ICXRDY_RESETVAL    (0x0001u)
/*----ICXRDY Tokens----*/
#define CSL_I2C_ICSTR_ICXRDY_CLEAR       (0x0000u)
#define CSL_I2C_ICSTR_ICXRDY_SET         (0x0001u)

#define CSL_I2C_ICSTR_ICRRDY_MASK        (0x0008u)
#define CSL_I2C_ICSTR_ICRRDY_SHIFT       (0x0003u)
#define CSL_I2C_ICSTR_ICRRDY_RESETVAL    (0x0000u)
/*----ICRRDY Tokens----*/
#define CSL_I2C_ICSTR_ICRRDY_CLEAR       (0x0000u)
#define CSL_I2C_ICSTR_ICRRDY_SET         (0x0001u)

#define CSL_I2C_ICSTR_ARDY_MASK          (0x0004u)
#define CSL_I2C_ICSTR_ARDY_SHIFT         (0x0002u)
#define CSL_I2C_ICSTR_ARDY_RESETVAL      (0x0000u)
/*----ARDY Tokens----*/
#define CSL_I2C_ICSTR_ARDY_CLEAR         (0x0000u)
#define CSL_I2C_ICSTR_ARDY_SET           (0x0001u)

#define CSL_I2C_ICSTR_NACK_MASK          (0x0002u)
#define CSL_I2C_ICSTR_NACK_SHIFT         (0x0001u)
#define CSL_I2C_ICSTR_NACK_RESETVAL      (0x0000u)
/*----NACK Tokens----*/
#define CSL_I2C_ICSTR_NACK_CLEAR         (0x0000u)
#define CSL_I2C_ICSTR_NACK_SET           (0x0001u)

#define CSL_I2C_ICSTR_AL_MASK            (0x0001u)
#define CSL_I2C_ICSTR_AL_SHIFT           (0x0000u)
#define CSL_I2C_ICSTR_AL_RESETVAL        (0x0000u)
/*----AL Tokens----*/
#define CSL_I2C_ICSTR_AL_CLEAR           (0x0000u)
#define CSL_I2C_ICSTR_AL_SET             (0x0001u)

#define CSL_I2C_ICSTR_RESETVAL           (0x0410u)

/* ICCLKL */

#define CSL_I2C_ICCLKL_ICCL_MASK         (0xFFFFu)
#define CSL_I2C_ICCLKL_ICCL_SHIFT        (0x0000u)
#define CSL_I2C_ICCLKL_ICCL_RESETVAL     (0x0000u)

#define CSL_I2C_ICCLKL_RESETVAL          (0x0000u)

/* ICCLKH */

#define CSL_I2C_ICCLKH_ICCH_MASK         (0xFFFFu)
#define CSL_I2C_ICCLKH_ICCH_SHIFT        (0x0000u)
#define CSL_I2C_ICCLKH_ICCH_RESETVAL     (0x0000u)

#define CSL_I2C_ICCLKH_RESETVAL          (0x0000u)

/* ICCNT */

#define CSL_I2C_ICCNT_ICDC_MASK          (0xFFFFu)
#define CSL_I2C_ICCNT_ICDC_SHIFT         (0x0000u)
#define CSL_I2C_ICCNT_ICDC_RESETVAL      (0x0000u)

#define CSL_I2C_ICCNT_RESETVAL           (0x0000u)

/* ICDRR */


#define CSL_I2C_ICDRR_D_MASK             (0x00FFu)
#define CSL_I2C_ICDRR_D_SHIFT            (0x0000u)
#define CSL_I2C_ICDRR_D_RESETVAL         (0x0000u)

#define CSL_I2C_ICDRR_RESETVAL           (0x0000u)

/* ICSAR */


#define CSL_I2C_ICSAR_SADDR_MASK         (0x03FFu)
#define CSL_I2C_ICSAR_SADDR_SHIFT        (0x0000u)
#define CSL_I2C_ICSAR_SADDR_RESETVAL     (0x03FFu)

#define CSL_I2C_ICSAR_RESETVAL           (0x03FFu)

/* ICDXR */


#define CSL_I2C_ICDXR_D_MASK             (0x00FFu)
#define CSL_I2C_ICDXR_D_SHIFT            (0x0000u)
#define CSL_I2C_ICDXR_D_RESETVAL         (0x0000u)

#define CSL_I2C_ICDXR_RESETVAL           (0x0000u)

/* ICMDR */

#define CSL_I2C_ICMDR_NACKMOD_MASK       (0x8000u)
#define CSL_I2C_ICMDR_NACKMOD_SHIFT      (0x000Fu)
#define CSL_I2C_ICMDR_NACKMOD_RESETVAL   (0x0000u)
/*----NACKMOD Tokens----*/
#define CSL_I2C_ICMDR_NACKMOD_CLEAR      (0x0000u)
#define CSL_I2C_ICMDR_NACKMOD_SET        (0x0001u)

#define CSL_I2C_ICMDR_FREE_MASK          (0x4000u)
#define CSL_I2C_ICMDR_FREE_SHIFT         (0x000Eu)
#define CSL_I2C_ICMDR_FREE_RESETVAL      (0x0000u)
/*----FREE Tokens----*/
#define CSL_I2C_ICMDR_FREE_CLEAR         (0x0000u)
#define CSL_I2C_ICMDR_FREE_SET           (0x0001u)

#define CSL_I2C_ICMDR_STT_MASK           (0x2000u)
#define CSL_I2C_ICMDR_STT_SHIFT          (0x000Du)
#define CSL_I2C_ICMDR_STT_RESETVAL       (0x0000u)
/*----STT Tokens----*/
#define CSL_I2C_ICMDR_STT_CLEAR          (0x0000u)
#define CSL_I2C_ICMDR_STT_SET            (0x0001u)


#define CSL_I2C_ICMDR_STP_MASK           (0x0800u)
#define CSL_I2C_ICMDR_STP_SHIFT          (0x000Bu)
#define CSL_I2C_ICMDR_STP_RESETVAL       (0x0000u)
/*----STP Tokens----*/
#define CSL_I2C_ICMDR_STP_CLEAR          (0x0000u)
#define CSL_I2C_ICMDR_STP_SET            (0x0001u)

#define CSL_I2C_ICMDR_MST_MASK           (0x0400u)
#define CSL_I2C_ICMDR_MST_SHIFT          (0x000Au)
#define CSL_I2C_ICMDR_MST_RESETVAL       (0x0000u)
/*----MST Tokens----*/
#define CSL_I2C_ICMDR_MST_CLEAR          (0x0000u)
#define CSL_I2C_ICMDR_MST_SET            (0x0001u)

#define CSL_I2C_ICMDR_TRX_MASK           (0x0200u)
#define CSL_I2C_ICMDR_TRX_SHIFT          (0x0009u)
#define CSL_I2C_ICMDR_TRX_RESETVAL       (0x0000u)
/*----TRX Tokens----*/
#define CSL_I2C_ICMDR_TRX_CLEAR          (0x0000u)
#define CSL_I2C_ICMDR_TRX_SET            (0x0001u)

#define CSL_I2C_ICMDR_XA_MASK            (0x0100u)
#define CSL_I2C_ICMDR_XA_SHIFT           (0x0008u)
#define CSL_I2C_ICMDR_XA_RESETVAL        (0x0000u)
/*----XA Tokens----*/
#define CSL_I2C_ICMDR_XA_CLEAR           (0x0000u)
#define CSL_I2C_ICMDR_XA_SET             (0x0001u)

#define CSL_I2C_ICMDR_RM_MASK            (0x0080u)
#define CSL_I2C_ICMDR_RM_SHIFT           (0x0007u)
#define CSL_I2C_ICMDR_RM_RESETVAL        (0x0000u)
/*----RM Tokens----*/
#define CSL_I2C_ICMDR_RM_CLEAR           (0x0000u)
#define CSL_I2C_ICMDR_RM_SET             (0x0001u)

#define CSL_I2C_ICMDR_DLB_MASK           (0x0040u)
#define CSL_I2C_ICMDR_DLB_SHIFT          (0x0006u)
#define CSL_I2C_ICMDR_DLB_RESETVAL       (0x0000u)
/*----DLB Tokens----*/
#define CSL_I2C_ICMDR_DLB_CLEAR          (0x0000u)
#define CSL_I2C_ICMDR_DLB_SET            (0x0001u)

#define CSL_I2C_ICMDR_IRS_MASK           (0x0020u)
#define CSL_I2C_ICMDR_IRS_SHIFT          (0x0005u)
#define CSL_I2C_ICMDR_IRS_RESETVAL       (0x0000u)
/*----IRS Tokens----*/
#define CSL_I2C_ICMDR_IRS_CLEAR          (0x0000u)
#define CSL_I2C_ICMDR_IRS_SET            (0x0001u)

#define CSL_I2C_ICMDR_STB_MASK           (0x0010u)
#define CSL_I2C_ICMDR_STB_SHIFT          (0x0004u)
#define CSL_I2C_ICMDR_STB_RESETVAL       (0x0000u)
/*----STB Tokens----*/
#define CSL_I2C_ICMDR_STB_CLEAR          (0x0000u)
#define CSL_I2C_ICMDR_STB_SET            (0x0001u)

#define CSL_I2C_ICMDR_FDF_MASK           (0x0008u)
#define CSL_I2C_ICMDR_FDF_SHIFT          (0x0003u)
#define CSL_I2C_ICMDR_FDF_RESETVAL       (0x0000u)
/*----FDF Tokens----*/
#define CSL_I2C_ICMDR_FDF_CLEAR          (0x0000u)
#define CSL_I2C_ICMDR_FDF_SET            (0x0001u)

#define CSL_I2C_ICMDR_BC_MASK            (0x0007u)
#define CSL_I2C_ICMDR_BC_SHIFT           (0x0000u)
#define CSL_I2C_ICMDR_BC_RESETVAL        (0x0000u)
/*----BC Tokens----*/
#define CSL_I2C_ICMDR_BC_8BITS           (0x0000u)
#define CSL_I2C_ICMDR_BC_RSV             (0x0001u)
#define CSL_I2C_ICMDR_BC_2BITS           (0x0002u)
#define CSL_I2C_ICMDR_BC_3BITS           (0x0003u)
#define CSL_I2C_ICMDR_BC_4BITS           (0x0004u)
#define CSL_I2C_ICMDR_BC_5BITS           (0x0005u)
#define CSL_I2C_ICMDR_BC_6BITS           (0x0006u)
#define CSL_I2C_ICMDR_BC_7BITS           (0x0007u)

#define CSL_I2C_ICMDR_RESETVAL           (0x0000u)

/* ICIVR */


#define CSL_I2C_ICIVR_INTCODE_MASK       (0x0007u)
#define CSL_I2C_ICIVR_INTCODE_SHIFT      (0x0000u)
#define CSL_I2C_ICIVR_INTCODE_RESETVAL   (0x0000u)
/*----INTCODE Tokens----*/
#define CSL_I2C_ICIVR_INTCODE_NONE       (0x0000u)
#define CSL_I2C_ICIVR_INTCODE_AL         (0x0001u)
#define CSL_I2C_ICIVR_INTCODE_NACK       (0x0002u)
#define CSL_I2C_ICIVR_INTCODE_ARDY       (0x0003u)
#define CSL_I2C_ICIVR_INTCODE_RDR        (0x0004u)
#define CSL_I2C_ICIVR_INTCODE_TDR        (0x0005u)
#define CSL_I2C_ICIVR_INTCODE_SCD        (0x0006u)
#define CSL_I2C_ICIVR_INTCODE_AAS        (0x0007u)

#define CSL_I2C_ICIVR_RESETVAL           (0x0000u)

/* ICEMDR */


#define CSL_I2C_ICEMDR_IGNACK_MASK       (0x0002u)
#define CSL_I2C_ICEMDR_IGNACK_SHIFT      (0x0001u)
#define CSL_I2C_ICEMDR_IGNACK_RESETVAL   (0x0000u)
/*----IGNACK Tokens----*/
#define CSL_I2C_ICEMDR_IGNACK_CLEAR      (0x0000u)
#define CSL_I2C_ICEMDR_IGNACK_SET        (0x0001u)

#define CSL_I2C_ICEMDR_BCM_MASK          (0x0001u)
#define CSL_I2C_ICEMDR_BCM_SHIFT         (0x0000u)
#define CSL_I2C_ICEMDR_BCM_RESETVAL      (0x0001u)
/*----BCM Tokens----*/
#define CSL_I2C_ICEMDR_BCM_CLEAR         (0x0000u)
#define CSL_I2C_ICEMDR_BCM_SET           (0x0001u)

#define CSL_I2C_ICEMDR_RESETVAL          (0x0001u)

/* ICPSC */


#define CSL_I2C_ICPSC_IPSC_MASK          (0x00FFu)
#define CSL_I2C_ICPSC_IPSC_SHIFT         (0x0000u)
#define CSL_I2C_ICPSC_IPSC_RESETVAL      (0x0000u)

#define CSL_I2C_ICPSC_RESETVAL           (0x0000u)

/* ICPID1 */

#define CSL_I2C_ICPID1_CLASS_MASK        (0xFF00u)
#define CSL_I2C_ICPID1_CLASS_SHIFT       (0x0008u)
#define CSL_I2C_ICPID1_CLASS_RESETVAL    (0x0001u)

#define CSL_I2C_ICPID1_REVISION_MASK     (0x00FFu)
#define CSL_I2C_ICPID1_REVISION_SHIFT    (0x0000u)
#define CSL_I2C_ICPID1_REVISION_RESETVAL (0x0006u)

#define CSL_I2C_ICPID1_RESETVAL          (0x0106u)

/* ICPID2 */


#define CSL_I2C_ICPID2_TYPE_MASK         (0x00FFu)
#define CSL_I2C_ICPID2_TYPE_SHIFT        (0x0000u)
#define CSL_I2C_ICPID2_TYPE_RESETVAL     (0x0005u)

#define CSL_I2C_ICPID2_RESETVAL          (0x0005u)

#endif
