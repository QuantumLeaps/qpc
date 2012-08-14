/********************************************************************
* Copyright (C) 2003-2008 Texas Instruments Incorporated.
* All Rights Reserved
*********************************************************************
* file: cslr_usb.h
*
* Brief: This file contains the Register Description for usb
*
*********************************************************************/
#ifndef _CSLR_USB_H_
#define _CSLR_USB_H_

#include <cslr.h>

#include <tistdtypes.h>
#include <csl_general.h>


/* Minimum unit = 2 bytes */

/**************************************************************************\
* Register Overlay Structure for EPTRG
\**************************************************************************/
typedef struct  {
    volatile Uint16 TXFUNCADDR;
    volatile Uint16 TXHUBADDR_TXHUBPORT;
    volatile Uint16 RSVD0[2];
    volatile Uint16 RXFUNCADDR;
    volatile Uint16 RXHUBADDR_RXHUBPORT;
    volatile Uint16 RSVD36[2];
} CSL_UsbEptrgRegs;

/**************************************************************************\
* Register Overlay Structure for EPCSR
\**************************************************************************/
typedef struct  {
    volatile Uint16 TXMAXP;
    volatile Uint16 PERI_TXCSR;
    volatile Uint16 RSVD0[2];
    volatile Uint16 RXMAXP;
    volatile Uint16 PERI_RXCSR;
    volatile Uint16 RSVD1[2];
    volatile Uint16 RXCOUNT;
    volatile Uint16 RSVD41[7];
} CSL_UsbEpcsrRegs;

/**************************************************************************\
* Register Overlay Structure for CHANNEL
\**************************************************************************/
typedef struct  {
    volatile Uint16 TXGCR1;
    volatile Uint16 TXGCR2;
    volatile Uint16 RSVD0[6];
    volatile Uint16 RXGCR1;
    volatile Uint16 RXGCR2;
    volatile Uint16 RSVD1[2];
    volatile Uint16 RXHPCR1A;
    volatile Uint16 RXHPCR2A;
    volatile Uint16 RSVD2[2];
    volatile Uint16 RXHPCR1B;
    volatile Uint16 RXHPCR2B;
    volatile Uint16 RSVD46[14];
} CSL_UsbChannelRegs;

/**************************************************************************\
* Register Overlay Structure for CdmaScheTblWord
\**************************************************************************/
typedef struct  {
    volatile Uint16 ENTRYLSW;
    volatile Uint16 ENTRYMSW;
    volatile Uint16 RSVD49[2];
} CSL_UsbCdmaschetblwordRegs;

/**************************************************************************\
* Register Overlay Structure for QMMemRegR
\**************************************************************************/
typedef struct  {
    volatile Uint16 QMEMRBASE1;
    volatile Uint16 QMEMRBASE2;
    volatile Uint16 RSVD0[2];
    volatile Uint16 QMEMRCTRL1;
    volatile Uint16 QMEMRCTRL2;
    volatile Uint16 RSVD63[10];
} CSL_UsbQmmemregrRegs;

/**************************************************************************\
* Register Overlay Structure for QMQN
\**************************************************************************/
typedef struct  {
    volatile Uint16 RSVD0[12];
    volatile Uint16 CTRL1D;
    volatile Uint16 CTRL2D;
    volatile Uint16 RSVD65[2];
} CSL_UsbQmqnRegs;

/**************************************************************************\
* Register Overlay Structure for QMQNS
\**************************************************************************/
typedef struct  {
    volatile Uint16 QSTATA;
    volatile Uint16 RSVD0[3];
    volatile Uint16 QSTAT1B;
    volatile Uint16 QSTAT2B;
    volatile Uint16 RSVD1[2];
    volatile Uint16 QSTAT1C;
    volatile Uint16 RSVD67[7];
} CSL_UsbQmqnsRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint16 REVID1;                    //0x8000
    volatile Uint16 REVID2;                    //0x8001
    volatile Uint16 RSVD0[2];                //0x8002/3
    volatile Uint16 CTRLR;                    //0x8004
    volatile Uint16 RSVD1[3];                //0x8005/6/7
    volatile Uint16 STATR;                    //0x8008
    volatile Uint16 RSVD2[3];                //0x8009/A/B
    volatile Uint16 EMUR;                    //0x800C
    volatile Uint16 RSVD3[3];                //0x800D/E/F
    volatile Uint16 MODE1;                    //0x8010
    volatile Uint16 MODE2;                    //0x8011
    volatile Uint16 RSVD4[2];                //0x8012/13
    volatile Uint16 AUTOREQ;                //0x8014
    volatile Uint16 RSVD5[3];                //0x8015/16/17
    volatile Uint16 SPRFIXTIME1;            //0x8018
    volatile Uint16 SPRFIXTIME2;            //0x8019
    volatile Uint16 RSVD6[2];                //0x801A/1B
    volatile Uint16 TEARDOWN1;                //0x801C
    volatile Uint16 TEARDOWN2;                //0x801D
    volatile Uint16 RSVD7[2];                //0x801E/1F
    volatile Uint16 INTSRCR1;                //0x8020
    volatile Uint16 INTSRCR2;                //0x8021
    volatile Uint16 RSVD8[2];                //0x8022/23
    volatile Uint16 INTSETR1;                //0x8024
    volatile Uint16 INTSETR2;                //0x8025
    volatile Uint16 RSVD9[2];                //0x8026/27
    volatile Uint16 INTCLRR1;                //0x8028
    volatile Uint16 INTCLRR2;                //0x8029
    volatile Uint16 RSVD10[2];                //0x802A/2B
    volatile Uint16 INTMSKR1;                //0x802C
    volatile Uint16 INTMSKR2;                //0x802D
    volatile Uint16 RSVD11[2];                //0x802E/2F
    volatile Uint16 INTMSKSETR1;            //0x8030
    volatile Uint16 INTMSKSETR2;            //0x8031
    volatile Uint16 RSVD12[2];                //0x8032/33
    volatile Uint16 INTMSKCLRR1;            //0x8034
    volatile Uint16 INTMSKCLRR2;            //0x8035
    volatile Uint16 RSVD13[2];                //0x8036/37
    volatile Uint16 INTMASKEDR1;            //0x8038
    volatile Uint16 INTMASKEDR2;            //0x8039
    volatile Uint16 RSVD14[2];                //0x803A/3B
    volatile Uint16 EOIR;                    //0x803C
    volatile Uint16 RSVD15[3];                //0x803D/3E/3F
    volatile Uint16 INTVECTR1;                //0x8040
    volatile Uint16 INTVECTR2;                //0x8041
    volatile Uint16 RSVD16[14];                //0x8042-804F
    volatile Uint16 GREP1SZR1;                //0x8050
    volatile Uint16 GREP1SZR2;                //0x8051
    volatile Uint16 RSVD17[2];                //0x8052/53
    volatile Uint16 GREP2SZR1;                //0x8054
    volatile Uint16 GREP2SZR2;                //0x8055
    volatile Uint16 RSVD18[2];                //0x8056/57
    volatile Uint16 GREP3SZR1;                //0x8058
    volatile Uint16 GREP3SZR2;                //0x8059
    volatile Uint16 RSVD19[2];                //0x805A/5B
    volatile Uint16 GREP4SZR1;                //0x805C
    volatile Uint16 GREP4SZR2;                //0x805D

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

    volatile Uint16 RSVD20[931];            //0x805E - 0x83FF

#else

    volatile Uint16 RSVD20[930];            //0x805E - 0x83FF

#endif

    volatile Uint16 FADDR_POWER;            //0x8400
    volatile Uint16 INTRTX;                    //0x8401
    volatile Uint16 RSVD21[2];                //0x8402/03
    volatile Uint16 INTRRX;                    //0x8404
    volatile Uint16 INTRTXE;                //0x8405
    volatile Uint16 RSVD22[2];                //0x8406/07
    volatile Uint16 INTRRXE;                //0x8408
    volatile Uint16 INTRUSB_INTRUSBE;        //0x8409
    volatile Uint16 RSVD23[2];                //0x840A/0B
    volatile Uint16 FRAME;                    //0x840C
    volatile Uint16 INDEX_TESTMODE;            //0x840D
    volatile Uint16 RSVD24[2];                //0x840E/0F
    volatile Uint16 TXMAXP_INDX;            //0x8410
    volatile Uint16 PERI_CSR0_INDX;            //0x8411
    volatile Uint16 RSVD25[2];                //0x8412/13
    volatile Uint16 RXMAXP_INDX;            //0x8414
    volatile Uint16 PERI_RXCSR_INDX;        //0x8415
    volatile Uint16 RSVD26[2];                //0x8416/17
    volatile Uint16 COUNT0_INDX;            //0x8418
    volatile Uint16 RSVD27[4];                //0x8419/1A/1B/1C
    volatile Uint16 CONFIGDATA_INDX;        //0x841D
    volatile Uint16 RSVD28[2];                //0x841E/1F
    volatile Uint16 FIFO0R1;                //0x8420
    volatile Uint16 FIFO0R2;                //0x8421
    volatile Uint16 RSVD29[2];                //0x8422/23
    volatile Uint16 FIFO1R1;                //0x8424
    volatile Uint16 FIFO1R2;                //0x8425
    volatile Uint16 RSVD30[2];                //0x8426/27
    volatile Uint16 FIFO2R1;                //0x8428
    volatile Uint16 FIFO2R2;                //0x8429
    volatile Uint16 RSVD31[2];                //0x842A/2B
    volatile Uint16 FIFO3R1;                //0x842C
    volatile Uint16 FIFO3R2;                //0x842D
    volatile Uint16 RSVD32[2];                //0x842E/2F
    volatile Uint16 FIFO4R1;                //0x8430
    volatile Uint16 FIFO4R2;                //0x8431
    volatile Uint16 RSVD33[46];                //0x8432 - 5F
    volatile Uint16 DEVCTL;                    //0x8460
    volatile Uint16 TXFIFOSZ_RXFIFOSZ;        //0x8461
    volatile Uint16 RSVD34[2];                //0x8462/63
    volatile Uint16 TXFIFOADDR;                //0x8464
    volatile Uint16 RXFIFOADDR;                //0x8465
    volatile Uint16 RSVD35[6];                //0x8466 - 6B
    volatile Uint16 HWVERS;                    //0x846C
    volatile Uint16 RSVD37[19];                //0x846D - 7F
    CSL_UsbEptrgRegs EPTRG[5];                //0x8480 - A7
    volatile Uint16 RSVD38[89];                //0x84A8 - 500
    volatile Uint16 PERI_CSR0;                //0x8501
    volatile Uint16 RSVD39[6];                //0x8502 - 507
    volatile Uint16 COUNT0;                    //0x8508
    volatile Uint16 RSVD40[4];                //0x8509 - 50C
    volatile Uint16 CONFIGDATA;                //0x850D
    volatile Uint16 RSVD42[2];                //0x850E/F
    CSL_UsbEpcsrRegs EPCSR[4];                //0x8510 - 0x854f

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

    volatile Uint16 RSVD43[2735];

#else

    volatile Uint16 RSVD43[2736];            //0x8550 - 0x8FFF

#endif

    volatile Uint16 DMAREVID1;                //0x9000
    volatile Uint16 DMAREVID2;                //0x9001
    volatile Uint16 RSVD44[2];                //0x9002/3
    volatile Uint16 TDFDQ;                    //0x9004
    volatile Uint16 RSVD45[3];                //0x9005/6/7
    volatile Uint16 DMAEMU;                    //0x9008
    volatile Uint16 RSVD47[2039];            //0x9009
    CSL_UsbChannelRegs CHANNEL[4];            //0x9800 - 0x987F
    volatile Uint16 RSVD48[1920];            //0x9880 - 0x9FFF
    volatile Uint16 DMA_SCHED_CTRL1;        //0xA000
    volatile Uint16 DMA_SCHED_CTRL2;        //0xA001
    volatile Uint16 RSVD50[2046];            //0xA002 - A7FF
    CSL_UsbCdmaschetblwordRegs CDMASCHETBLWORD[64];//0xA800 - 0xA8FF
    volatile Uint16 RSVD51[5888];            //0xA900 - 0xBFFF
    volatile Uint16 QMGRREVID1;                //0xC000
    volatile Uint16 QMGRREVID2;                //0xC001
    volatile Uint16 RSVD52[6];                //0xC002 - 7
    volatile Uint16 DIVERSION1;                //0xC008
    volatile Uint16 DIVERSION2;                //0xC009
    volatile Uint16 RSVD53[22];                //0xC00A - 1F
    volatile Uint16 FDBSC0;                    //0xC020
    volatile Uint16 FDBSC1;                    //0xC021
    volatile Uint16 RSVD54[2];                //0xC022/23
    volatile Uint16 FDBSC2;                    //0xC024
    volatile Uint16 FDBSC3;                    //0xC025
    volatile Uint16 RSVD55[2];                //0xC026/7
    volatile Uint16 FDBSC4;                    //0xC028
    volatile Uint16 FDBSC5;                    //0xC029
    volatile Uint16 RSVD56[2];                //0xC02A/2B
    volatile Uint16 FDBSC6;                    //0xC02C
    volatile Uint16 FDBSC7;                    //0xC02D
    volatile Uint16 RSVD57[82];                //0xC02E - 7F
    volatile Uint16 LRAM0BASE1;                //0xC080
    volatile Uint16 LRAM0BASE2;                //0xC081
    volatile Uint16 RSVD58[2];                //0xC082/83
    volatile Uint16 LRAM0SIZE;                //0xC084
    volatile Uint16 RSVD59[3];                //0xC085/6/7
    volatile Uint16 LRAM1BASE1;                //0xC088
    volatile Uint16 LRAM1BASE2;                //0xC089
    volatile Uint16 RSVD60[6];                //0xC08A - 8F
    volatile Uint16 PEND0;                    //0xC090
    volatile Uint16 PEND1;                    //0xC091
    volatile Uint16 RSVD61[2];                //0xC092/93
    volatile Uint16 PEND2;                    //0xC094
    volatile Uint16 PEND3;                    //0xC094
    volatile Uint16 RSVD62[2];                //0xC096/97
    volatile Uint16 PEND4;                    //0xC098
    volatile Uint16 PEND5;                    //0xC099
    volatile Uint16 RSVD64[3942];            //0xC09A - 0xCFFF
    CSL_UsbQmmemregrRegs QMMEMREGR[16];        //0xD000 - 0xD0FF
    volatile Uint16 RSVD66[3840];            //0xD100 - 0xDFFF
    CSL_UsbQmqnRegs QMQN[64];                //0xE000 - 0xE3FF
    volatile Uint16 RSVD68[1024];            //0xE400 - 0xE800
    CSL_UsbQmqnsRegs QMQNS[64];                //0xE800 - 0xEC00
} CSL_UsbRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* TXFUNCADDR */


#define CSL_USB_TXFUNCADDR_FUNCADDR_MASK (0x007Fu)
#define CSL_USB_TXFUNCADDR_FUNCADDR_SHIFT (0x0000u)
#define CSL_USB_TXFUNCADDR_FUNCADDR_RESETVAL (0x0000u)

#define CSL_USB_TXFUNCADDR_RESETVAL      (0x0000u)

/* TXHUBADDR_TXHUBPORT */


#define CSL_USB_TXHUBADDR_TXHUBPORT_HUBPORT_MASK (0x7F00u)
#define CSL_USB_TXHUBADDR_TXHUBPORT_HUBPORT_SHIFT (0x0008u)
#define CSL_USB_TXHUBADDR_TXHUBPORT_HUBPORT_RESETVAL (0x0000u)

#define CSL_USB_TXHUBADDR_TXHUBPORT_MULT_TRANS_MASK (0x0080u)
#define CSL_USB_TXHUBADDR_TXHUBPORT_MULT_TRANS_SHIFT (0x0007u)
#define CSL_USB_TXHUBADDR_TXHUBPORT_MULT_TRANS_RESETVAL (0x0000u)

#define CSL_USB_TXHUBADDR_TXHUBPORT_HUBADDR_MASK (0x007Fu)
#define CSL_USB_TXHUBADDR_TXHUBPORT_HUBADDR_SHIFT (0x0000u)
#define CSL_USB_TXHUBADDR_TXHUBPORT_HUBADDR_RESETVAL (0x0000u)

#define CSL_USB_TXHUBADDR_TXHUBPORT_RESETVAL (0x0000u)

/* RXFUNCADDR */


#define CSL_USB_RXFUNCADDR_FUNCADDR_MASK (0x007Fu)
#define CSL_USB_RXFUNCADDR_FUNCADDR_SHIFT (0x0000u)
#define CSL_USB_RXFUNCADDR_FUNCADDR_RESETVAL (0x0000u)

#define CSL_USB_RXFUNCADDR_RESETVAL      (0x0000u)

/* RXHUBADDR_RXHUBPORT */


#define CSL_USB_RXHUBADDR_RXHUBPORT_HUBPORT_MASK (0x7F00u)
#define CSL_USB_RXHUBADDR_RXHUBPORT_HUBPORT_SHIFT (0x0008u)
#define CSL_USB_RXHUBADDR_RXHUBPORT_HUBPORT_RESETVAL (0x0000u)

#define CSL_USB_RXHUBADDR_RXHUBPORT_MULT_TRANS_MASK (0x0080u)
#define CSL_USB_RXHUBADDR_RXHUBPORT_MULT_TRANS_SHIFT (0x0007u)
#define CSL_USB_RXHUBADDR_RXHUBPORT_MULT_TRANS_RESETVAL (0x0000u)

#define CSL_USB_RXHUBADDR_RXHUBPORT_HUBADDR_MASK (0x007Fu)
#define CSL_USB_RXHUBADDR_RXHUBPORT_HUBADDR_SHIFT (0x0000u)
#define CSL_USB_RXHUBADDR_RXHUBPORT_HUBADDR_RESETVAL (0x0000u)

#define CSL_USB_RXHUBADDR_RXHUBPORT_RESETVAL (0x0000u)

/* TXMAXP */


#define CSL_USB_TXMAXP_MAXPAYLOAD_MASK   (0x07FFu)
#define CSL_USB_TXMAXP_MAXPAYLOAD_SHIFT  (0x0000u)
#define CSL_USB_TXMAXP_MAXPAYLOAD_RESETVAL (0x0000u)

#define CSL_USB_TXMAXP_RESETVAL          (0x0000u)

/* PERI_TXCSR */

#define CSL_USB_PERI_TXCSR_AUTOSET_MASK  (0x8000u)
#define CSL_USB_PERI_TXCSR_AUTOSET_SHIFT (0x000Fu)
#define CSL_USB_PERI_TXCSR_AUTOSET_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_ISO_MASK      (0x4000u)
#define CSL_USB_PERI_TXCSR_ISO_SHIFT     (0x000Eu)
#define CSL_USB_PERI_TXCSR_ISO_RESETVAL  (0x0000u)

#define CSL_USB_PERI_TXCSR_MODE_MASK     (0x2000u)
#define CSL_USB_PERI_TXCSR_MODE_SHIFT    (0x000Du)
#define CSL_USB_PERI_TXCSR_MODE_RESETVAL (0x0001u)

#define CSL_USB_PERI_TXCSR_DMAEN_MASK    (0x1000u)
#define CSL_USB_PERI_TXCSR_DMAEN_SHIFT   (0x000Cu)
#define CSL_USB_PERI_TXCSR_DMAEN_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_FRCDATATOG_MASK (0x0800u)
#define CSL_USB_PERI_TXCSR_FRCDATATOG_SHIFT (0x000Bu)
#define CSL_USB_PERI_TXCSR_FRCDATATOG_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_DMAMODE_MASK  (0x0400u)
#define CSL_USB_PERI_TXCSR_DMAMODE_SHIFT (0x000Au)
#define CSL_USB_PERI_TXCSR_DMAMODE_RESETVAL (0x0000u)


#define CSL_USB_PERI_TXCSR_CLRDATATOG_MASK (0x0040u)
#define CSL_USB_PERI_TXCSR_CLRDATATOG_SHIFT (0x0006u)
#define CSL_USB_PERI_TXCSR_CLRDATATOG_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_SENTSTALL_MASK (0x0020u)
#define CSL_USB_PERI_TXCSR_SENTSTALL_SHIFT (0x0005u)
#define CSL_USB_PERI_TXCSR_SENTSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_SENDSTALL_MASK (0x0010u)
#define CSL_USB_PERI_TXCSR_SENDSTALL_SHIFT (0x0004u)
#define CSL_USB_PERI_TXCSR_SENDSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_FLUSHFIFO_MASK (0x0008u)
#define CSL_USB_PERI_TXCSR_FLUSHFIFO_SHIFT (0x0003u)
#define CSL_USB_PERI_TXCSR_FLUSHFIFO_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_UNDERRUN_MASK (0x0004u)
#define CSL_USB_PERI_TXCSR_UNDERRUN_SHIFT (0x0002u)
#define CSL_USB_PERI_TXCSR_UNDERRUN_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_FIFONOTEMPTY_MASK (0x0002u)
#define CSL_USB_PERI_TXCSR_FIFONOTEMPTY_SHIFT (0x0001u)
#define CSL_USB_PERI_TXCSR_FIFONOTEMPTY_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_TXPKTRDY_MASK (0x0001u)
#define CSL_USB_PERI_TXCSR_TXPKTRDY_SHIFT (0x0000u)
#define CSL_USB_PERI_TXCSR_TXPKTRDY_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_RESETVAL      (0x2000u)

/* RXMAXP */


#define CSL_USB_RXMAXP_MAXPAYLOAD_MASK   (0x07FFu)
#define CSL_USB_RXMAXP_MAXPAYLOAD_SHIFT  (0x0000u)
#define CSL_USB_RXMAXP_MAXPAYLOAD_RESETVAL (0x0000u)

#define CSL_USB_RXMAXP_RESETVAL          (0x0000u)

/* PERI_RXCSR */

#define CSL_USB_PERI_RXCSR_AUTOCLEAR_MASK (0x8000u)
#define CSL_USB_PERI_RXCSR_AUTOCLEAR_SHIFT (0x000Fu)
#define CSL_USB_PERI_RXCSR_AUTOCLEAR_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_ISO_MASK      (0x4000u)
#define CSL_USB_PERI_RXCSR_ISO_SHIFT     (0x000Eu)
#define CSL_USB_PERI_RXCSR_ISO_RESETVAL  (0x0000u)

#define CSL_USB_PERI_RXCSR_DMAEN_MASK    (0x2000u)
#define CSL_USB_PERI_RXCSR_DMAEN_SHIFT   (0x000Du)
#define CSL_USB_PERI_RXCSR_DMAEN_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_DISNYET_MASK  (0x1000u)
#define CSL_USB_PERI_RXCSR_DISNYET_SHIFT (0x000Cu)
#define CSL_USB_PERI_RXCSR_DISNYET_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_DMAMODE_MASK  (0x0800u)
#define CSL_USB_PERI_RXCSR_DMAMODE_SHIFT (0x000Bu)
#define CSL_USB_PERI_RXCSR_DMAMODE_RESETVAL (0x0000u)


#define CSL_USB_PERI_RXCSR_CLRDATATOG_MASK (0x0080u)
#define CSL_USB_PERI_RXCSR_CLRDATATOG_SHIFT (0x0007u)
#define CSL_USB_PERI_RXCSR_CLRDATATOG_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_SENTSTALL_MASK (0x0040u)
#define CSL_USB_PERI_RXCSR_SENTSTALL_SHIFT (0x0006u)
#define CSL_USB_PERI_RXCSR_SENTSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_SENDSTALL_MASK (0x0020u)
#define CSL_USB_PERI_RXCSR_SENDSTALL_SHIFT (0x0005u)
#define CSL_USB_PERI_RXCSR_SENDSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_FLUSHFIFO_MASK (0x0010u)
#define CSL_USB_PERI_RXCSR_FLUSHFIFO_SHIFT (0x0004u)
#define CSL_USB_PERI_RXCSR_FLUSHFIFO_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_DATAERROR_MASK (0x0008u)
#define CSL_USB_PERI_RXCSR_DATAERROR_SHIFT (0x0003u)
#define CSL_USB_PERI_RXCSR_DATAERROR_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_OVERRUN_MASK  (0x0004u)
#define CSL_USB_PERI_RXCSR_OVERRUN_SHIFT (0x0002u)
#define CSL_USB_PERI_RXCSR_OVERRUN_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_FIFOFULL_MASK (0x0002u)
#define CSL_USB_PERI_RXCSR_FIFOFULL_SHIFT (0x0001u)
#define CSL_USB_PERI_RXCSR_FIFOFULL_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_RXPKTRDY_MASK (0x0001u)
#define CSL_USB_PERI_RXCSR_RXPKTRDY_SHIFT (0x0000u)
#define CSL_USB_PERI_RXCSR_RXPKTRDY_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_RESETVAL      (0x0000u)

/* RXCOUNT */


#define CSL_USB_RXCOUNT_EPRXCOUNT_MASK   (0x1FFFu)
#define CSL_USB_RXCOUNT_EPRXCOUNT_SHIFT  (0x0000u)
#define CSL_USB_RXCOUNT_EPRXCOUNT_RESETVAL (0x0000u)

#define CSL_USB_RXCOUNT_RESETVAL         (0x0000u)

/* TXGCR1 */


#define CSL_USB_TXGCR1_TX_DEFAULT_QMGR_MASK (0x3000u)
#define CSL_USB_TXGCR1_TX_DEFAULT_QMGR_SHIFT (0x000Cu)
#define CSL_USB_TXGCR1_TX_DEFAULT_QMGR_RESETVAL (0x0000u)

#define CSL_USB_TXGCR1_TX_DEFAULT_QNUM_MASK (0x0FFFu)
#define CSL_USB_TXGCR1_TX_DEFAULT_QNUM_SHIFT (0x0000u)
#define CSL_USB_TXGCR1_TX_DEFAULT_QNUM_RESETVAL (0x0000u)

#define CSL_USB_TXGCR1_RESETVAL          (0x0000u)

/* TXGCR2 */

#define CSL_USB_TXGCR2_TX_ENABLE_MASK    (0x8000u)
#define CSL_USB_TXGCR2_TX_ENABLE_SHIFT   (0x000Fu)
#define CSL_USB_TXGCR2_TX_ENABLE_RESETVAL (0x0000u)
/*----TX_ENABLE Tokens----*/
#define CSL_USB_TXGCR2_TX_ENABLE_DISABLE (0x0000u)
#define CSL_USB_TXGCR2_TX_ENABLE_ENABLE  (0x0001u)

#define CSL_USB_TXGCR2_TX_TEARDOWN_MASK  (0x4000u)
#define CSL_USB_TXGCR2_TX_TEARDOWN_SHIFT (0x000Eu)
#define CSL_USB_TXGCR2_TX_TEARDOWN_RESETVAL (0x0000u)


#define CSL_USB_TXGCR2_RESETVAL          (0x0000u)

/* RXGCR1 */



#define CSL_USB_RXGCR1_RX_DEFAULTT_RQ_QNUM_MASK (0x0FFFu)
#define CSL_USB_RXGCR1_RX_DEFAULTT_RQ_QNUM_SHIFT (0x0000u)
#define CSL_USB_RXGCR1_RX_DEFAULTT_RQ_QNUM_RESETVAL (0x0000u)

#define CSL_USB_RXGCR1_RESETVAL          (0x0000u)

/* RXGCR2 */

#define CSL_USB_RXGCR2_RX_ENABLE_MASK    (0x8000u)
#define CSL_USB_RXGCR2_RX_ENABLE_SHIFT   (0x000Fu)
#define CSL_USB_RXGCR2_RX_ENABLE_RESETVAL (0x0000u)
/*----RX_ENABLE Tokens----*/
#define CSL_USB_RXGCR2_RX_ENABLE_DISABLE (0x0000u)
#define CSL_USB_RXGCR2_RX_ENABLE_ENABLE  (0x0001u)

#define CSL_USB_RXGCR2_RX_TEARDOWN_MASK  (0x4000u)
#define CSL_USB_RXGCR2_RX_TEARDOWN_SHIFT (0x000Eu)
#define CSL_USB_RXGCR2_RX_TEARDOWN_RESETVAL (0x0000u)


#define CSL_USB_RXGCR2_RX_ERROR_HANDLING_MASK (0x0100u)
#define CSL_USB_RXGCR2_RX_ERROR_HANDLING_SHIFT (0x0008u)
#define CSL_USB_RXGCR2_RX_ERROR_HANDLING_RESETVAL (0x0000u)
/*----RX_ERROR_HANDLING Tokens----*/
#define CSL_USB_RXGCR2_RX_ERROR_HANDLING_DROPPCKT (0x0000u)
#define CSL_USB_RXGCR2_RX_ERROR_HANDLING_RETRYPCKT (0x0001u)

#define CSL_USB_RXGCR2_RX_SOP_OFFSET_MASK (0x00FFu)
#define CSL_USB_RXGCR2_RX_SOP_OFFSET_SHIFT (0x0000u)
#define CSL_USB_RXGCR2_RX_SOP_OFFSET_RESETVAL (0x0000u)

#define CSL_USB_RXGCR2_RESETVAL          (0x0000u)

/* RXHPCR1A */



#define CSL_USB_RXHPCR1A_RX_HOST_FDQ0_QNUM_MASK (0x0FFFu)
#define CSL_USB_RXHPCR1A_RX_HOST_FDQ0_QNUM_SHIFT (0x0000u)
#define CSL_USB_RXHPCR1A_RX_HOST_FDQ0_QNUM_RESETVAL (0x0000u)

#define CSL_USB_RXHPCR1A_RESETVAL        (0x0000u)

/* RXHPCR2A */



#define CSL_USB_RXHPCR2A_RX_HOST_FDQ1_QNUM_MASK (0x0FFFu)
#define CSL_USB_RXHPCR2A_RX_HOST_FDQ1_QNUM_SHIFT (0x0000u)
#define CSL_USB_RXHPCR2A_RX_HOST_FDQ1_QNUM_RESETVAL (0x0000u)

#define CSL_USB_RXHPCR2A_RESETVAL        (0x0000u)

/* RXHPCR1B */



#define CSL_USB_RXHPCR1B_RX_HOST_FDQ2_QNUM_MASK (0x0FFFu)
#define CSL_USB_RXHPCR1B_RX_HOST_FDQ2_QNUM_SHIFT (0x0000u)
#define CSL_USB_RXHPCR1B_RX_HOST_FDQ2_QNUM_RESETVAL (0x0000u)

#define CSL_USB_RXHPCR1B_RESETVAL        (0x0000u)

/* RXHPCR2B */



#define CSL_USB_RXHPCR2B_RX_HOST_FDQ3_QNUM_MASK (0x0FFFu)
#define CSL_USB_RXHPCR2B_RX_HOST_FDQ3_QNUM_SHIFT (0x0000u)
#define CSL_USB_RXHPCR2B_RX_HOST_FDQ3_QNUM_RESETVAL (0x0000u)

#define CSL_USB_RXHPCR2B_RESETVAL        (0x0000u)

/* ENTRYLSW */

#define CSL_USB_ENTRYLSW_ENTRY1_RXTX_MASK (0x8000u)
#define CSL_USB_ENTRYLSW_ENTRY1_RXTX_SHIFT (0x000Fu)
#define CSL_USB_ENTRYLSW_ENTRY1_RXTX_RESETVAL (0x0000u)
/*----ENTRY1_RXTX Tokens----*/
#define CSL_USB_ENTRYLSW_ENTRY1_RXTX_XMTCH (0x0000u)
#define CSL_USB_ENTRYLSW_ENTRY1_RXTX_RCVCH (0x0001u)


#define CSL_USB_ENTRYLSW_ENTRY1_CHANNEL_MASK (0x1F00u)
#define CSL_USB_ENTRYLSW_ENTRY1_CHANNEL_SHIFT (0x0008u)
#define CSL_USB_ENTRYLSW_ENTRY1_CHANNEL_RESETVAL (0x0000u)

#define CSL_USB_ENTRYLSW_ENTRY0_RXTX_MASK (0x0080u)
#define CSL_USB_ENTRYLSW_ENTRY0_RXTX_SHIFT (0x0007u)
#define CSL_USB_ENTRYLSW_ENTRY0_RXTX_RESETVAL (0x0000u)
/*----ENTRY0_RXTX Tokens----*/
#define CSL_USB_ENTRYLSW_ENTRY0_RXTX_XMTCH (0x0000u)
#define CSL_USB_ENTRYLSW_ENTRY0_RXTX_RCVCH (0x0001u)


#define CSL_USB_ENTRYLSW_ENTRY0_CHANNEL_MASK (0x001Fu)
#define CSL_USB_ENTRYLSW_ENTRY0_CHANNEL_SHIFT (0x0000u)
#define CSL_USB_ENTRYLSW_ENTRY0_CHANNEL_RESETVAL (0x0000u)

#define CSL_USB_ENTRYLSW_RESETVAL        (0x0000u)

/* ENTRYMSW */

#define CSL_USB_ENTRYMSW_ENTRY3_RXTX_MASK (0x8000u)
#define CSL_USB_ENTRYMSW_ENTRY3_RXTX_SHIFT (0x000Fu)
#define CSL_USB_ENTRYMSW_ENTRY3_RXTX_RESETVAL (0x0000u)
/*----ENTRY3_RXTX Tokens----*/
#define CSL_USB_ENTRYMSW_ENTRY3_RXTX_XMTCH (0x0000u)
#define CSL_USB_ENTRYMSW_ENTRY3_RXTX_RCVCH (0x0001u)


#define CSL_USB_ENTRYMSW_ENTRY3_CHANNEL_MASK (0x1F00u)
#define CSL_USB_ENTRYMSW_ENTRY3_CHANNEL_SHIFT (0x0008u)
#define CSL_USB_ENTRYMSW_ENTRY3_CHANNEL_RESETVAL (0x0000u)

#define CSL_USB_ENTRYMSW_ENTRY2_RXTX_MASK (0x0080u)
#define CSL_USB_ENTRYMSW_ENTRY2_RXTX_SHIFT (0x0007u)
#define CSL_USB_ENTRYMSW_ENTRY2_RXTX_RESETVAL (0x0000u)
/*----ENTRY2_RXTX Tokens----*/
#define CSL_USB_ENTRYMSW_ENTRY2_RXTX_XMTCH (0x0000u)
#define CSL_USB_ENTRYMSW_ENTRY2_RXTX_RCVCH (0x0001u)


#define CSL_USB_ENTRYMSW_ENTRY2_CHANNEL_MASK (0x001Fu)
#define CSL_USB_ENTRYMSW_ENTRY2_CHANNEL_SHIFT (0x0000u)
#define CSL_USB_ENTRYMSW_ENTRY2_CHANNEL_RESETVAL (0x0000u)

#define CSL_USB_ENTRYMSW_RESETVAL        (0x0000u)

/* QMEMRBASE1 */

#define CSL_USB_QMEMRBASE1_REG_MASK      (0xFFFFu)
#define CSL_USB_QMEMRBASE1_REG_SHIFT     (0x0000u)
#define CSL_USB_QMEMRBASE1_REG_RESETVAL  (0x0000u)

#define CSL_USB_QMEMRBASE1_RESETVAL      (0x0000u)

/* QMEMRBASE2 */

#define CSL_USB_QMEMRBASE2_REG_MASK      (0xFFFFu)
#define CSL_USB_QMEMRBASE2_REG_SHIFT     (0x0000u)
#define CSL_USB_QMEMRBASE2_REG_RESETVAL  (0x0000u)

#define CSL_USB_QMEMRBASE2_RESETVAL      (0x0000u)

/* QMEMRCTRL1 */


#define CSL_USB_QMEMRCTRL1_DESC_SIZE_MASK (0x0F00u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_SHIFT (0x0008u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_RESETVAL (0x0000u)
/*----DESC_SIZE Tokens----*/
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_SIZE32 (0x0000u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_SIZE64 (0x0001u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_SIZE128 (0x0002u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_SIZE256 (0x0003u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_SIZE512 (0x0004u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_SIZE1K (0x0005u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_SIZE2K (0x0006u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_SIZE4K (0x0007u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_SIZE8K (0x0008u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_RSV1 (0x0009u)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_RSV2 (0x000au)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_RSV3 (0x000bu)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_RSV4 (0x000cu)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_RSV5 (0x000du)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_RSV6 (0x000eu)
#define CSL_USB_QMEMRCTRL1_DESC_SIZE_RSV7 (0x000fu)


#define CSL_USB_QMEMRCTRL1_REG_SIZE_MASK (0x0007u)
#define CSL_USB_QMEMRCTRL1_REG_SIZE_SHIFT (0x0000u)
#define CSL_USB_QMEMRCTRL1_REG_SIZE_RESETVAL (0x0000u)
/*----REG_SIZE Tokens----*/
#define CSL_USB_QMEMRCTRL1_REG_SIZE_SIZE32 (0x0000u)
#define CSL_USB_QMEMRCTRL1_REG_SIZE_SIZE64 (0x0001u)
#define CSL_USB_QMEMRCTRL1_REG_SIZE_SIZE128 (0x0002u)
#define CSL_USB_QMEMRCTRL1_REG_SIZE_SIZE256 (0x0003u)
#define CSL_USB_QMEMRCTRL1_REG_SIZE_SIZE512 (0x0004u)
#define CSL_USB_QMEMRCTRL1_REG_SIZE_SIZE1K (0x0005u)
#define CSL_USB_QMEMRCTRL1_REG_SIZE_SIZE2K (0x0006u)
#define CSL_USB_QMEMRCTRL1_REG_SIZE_SIZE4K (0x0007u)

#define CSL_USB_QMEMRCTRL1_RESETVAL      (0x0000u)

/* QMEMRCTRL2 */


#define CSL_USB_QMEMRCTRL2_START_INDEX_MASK (0x3FFFu)
#define CSL_USB_QMEMRCTRL2_START_INDEX_SHIFT (0x0000u)
#define CSL_USB_QMEMRCTRL2_START_INDEX_RESETVAL (0x0000u)

#define CSL_USB_QMEMRCTRL2_RESETVAL      (0x0000u)

/* CTRL1D */

#define CSL_USB_CTRL1D_DESC_PTR_MASK     (0xFFE0u)
#define CSL_USB_CTRL1D_DESC_PTR_SHIFT    (0x0005u)
#define CSL_USB_CTRL1D_DESC_PTR_RESETVAL (0x0000u)

#define CSL_USB_CTRL1D_DESC_SIZE_MASK    (0x001Fu)
#define CSL_USB_CTRL1D_DESC_SIZE_SHIFT   (0x0000u)
#define CSL_USB_CTRL1D_DESC_SIZE_RESETVAL (0x0000u)

#define CSL_USB_CTRL1D_RESETVAL          (0x0000u)

/* CTRL2D */

#define CSL_USB_CTRL2D_DESC_PTR_MASK     (0xFFFFu)
#define CSL_USB_CTRL2D_DESC_PTR_SHIFT    (0x0000u)
#define CSL_USB_CTRL2D_DESC_PTR_RESETVAL (0x0000u)

#define CSL_USB_CTRL2D_RESETVAL          (0x0000u)

/* QSTATA */


#define CSL_USB_QSTATA_QUEUE_ENTRY_COUNT_MASK (0x3FFFu)
#define CSL_USB_QSTATA_QUEUE_ENTRY_COUNT_SHIFT (0x0000u)
#define CSL_USB_QSTATA_QUEUE_ENTRY_COUNT_RESETVAL (0x0000u)

#define CSL_USB_QSTATA_RESETVAL          (0x0000u)

/* QSTAT1B */

#define CSL_USB_QSTAT1B_QUEUE_BYTE_COUNT_MASK (0xFFFFu)
#define CSL_USB_QSTAT1B_QUEUE_BYTE_COUNT_SHIFT (0x0000u)
#define CSL_USB_QSTAT1B_QUEUE_BYTE_COUNT_RESETVAL (0x0000u)

#define CSL_USB_QSTAT1B_RESETVAL         (0x0000u)

/* QSTAT2B */


#define CSL_USB_QSTAT2B_QUEUE_BYTE_COUNT_MASK (0x0FFFu)
#define CSL_USB_QSTAT2B_QUEUE_BYTE_COUNT_SHIFT (0x0000u)
#define CSL_USB_QSTAT2B_QUEUE_BYTE_COUNT_RESETVAL (0x0000u)

#define CSL_USB_QSTAT2B_RESETVAL         (0x0000u)

/* QSTAT1C */


#define CSL_USB_QSTAT1C_PACKET_SIZE_MASK (0x3FFFu)
#define CSL_USB_QSTAT1C_PACKET_SIZE_SHIFT (0x0000u)
#define CSL_USB_QSTAT1C_PACKET_SIZE_RESETVAL (0x0000u)

#define CSL_USB_QSTAT1C_RESETVAL         (0x0000u)

/* REVID1 */

#define CSL_USB_REVID1_REVLSB_MASK       (0xFFFFu)
#define CSL_USB_REVID1_REVLSB_SHIFT      (0x0000u)
#define CSL_USB_REVID1_REVLSB_RESETVAL   (0x0800u)

#define CSL_USB_REVID1_RESETVAL          (0x0800u)

/* REVID2 */

#define CSL_USB_REVID2_REVMSB_MASK       (0xFFFFu)
#define CSL_USB_REVID2_REVMSB_SHIFT      (0x0000u)
#define CSL_USB_REVID2_REVMSB_RESETVAL   (0x4EA1u)

#define CSL_USB_REVID2_RESETVAL          (0x4EA1u)

/* CTRLR */


#define CSL_USB_CTRLR_RNDIS_MASK         (0x0010u)
#define CSL_USB_CTRLR_RNDIS_SHIFT        (0x0004u)
#define CSL_USB_CTRLR_RNDIS_RESETVAL     (0x0000u)
/*----RNDIS Tokens----*/
#define CSL_USB_CTRLR_RNDIS_DISABLED     (0x0000u)
#define CSL_USB_CTRLR_RNDIS_ENABLED      (0x0001u)

#define CSL_USB_CTRLR_UINT_MASK          (0x0008u)
#define CSL_USB_CTRLR_UINT_SHIFT         (0x0003u)
#define CSL_USB_CTRLR_UINT_RESETVAL      (0x0000u)
/*----UINT Tokens----*/
#define CSL_USB_CTRLR_UINT_DISABLED      (0x0000u)
#define CSL_USB_CTRLR_UINT_ENABLED       (0x0001u)


#define CSL_USB_CTRLR_CLKFACK_MASK       (0x0002u)
#define CSL_USB_CTRLR_CLKFACK_SHIFT      (0x0001u)
#define CSL_USB_CTRLR_CLKFACK_RESETVAL   (0x0000u)
/*----CLKFACK Tokens----*/
#define CSL_USB_CTRLR_CLKFACK_DISABLED   (0x0000u)
#define CSL_USB_CTRLR_CLKFACK_ENABLED    (0x0001u)

#define CSL_USB_CTRLR_RESET_MASK         (0x0001u)
#define CSL_USB_CTRLR_RESET_SHIFT        (0x0000u)
#define CSL_USB_CTRLR_RESET_RESETVAL     (0x0000u)
/*----RESET Tokens----*/
#define CSL_USB_CTRLR_RESET_NONE         (0x0000u)
#define CSL_USB_CTRLR_RESET_START        (0x0001u)

#define CSL_USB_CTRLR_RESETVAL           (0x0000u)

/* STATR */


#define CSL_USB_STATR_DRVVBUS_MASK       (0x0001u)
#define CSL_USB_STATR_DRVVBUS_SHIFT      (0x0000u)
#define CSL_USB_STATR_DRVVBUS_RESETVAL   (0x0000u)
/*----DRVVBUS Tokens----*/
#define CSL_USB_STATR_DRVVBUS_LOGIC0     (0x0000u)
#define CSL_USB_STATR_DRVVBUS_LOGIC1     (0x0001u)

#define CSL_USB_STATR_RESETVAL           (0x0000u)

/* EMUR */


#define CSL_USB_EMUR_RTSEL_MASK          (0x0004u)
#define CSL_USB_EMUR_RTSEL_SHIFT         (0x0002u)
#define CSL_USB_EMUR_RTSEL_RESETVAL      (0x0000u)
/*----RTSEL Tokens----*/
#define CSL_USB_EMUR_RTSEL_ENABLE        (0x0000u)
#define CSL_USB_EMUR_RTSEL_NOEFFECT      (0x0001u)

#define CSL_USB_EMUR_SOFT_MASK           (0x0002u)
#define CSL_USB_EMUR_SOFT_SHIFT          (0x0001u)
#define CSL_USB_EMUR_SOFT_RESETVAL       (0x0001u)
/*----SOFT Tokens----*/
#define CSL_USB_EMUR_SOFT_NOEFFECT       (0x0000u)
#define CSL_USB_EMUR_SOFT_ENABLE         (0x0001u)

#define CSL_USB_EMUR_FREERUN_MASK        (0x0001u)
#define CSL_USB_EMUR_FREERUN_SHIFT       (0x0000u)
#define CSL_USB_EMUR_FREERUN_RESETVAL    (0x0001u)
/*----FREERUN Tokens----*/
#define CSL_USB_EMUR_FREERUN_NOEFFECT    (0x0000u)
#define CSL_USB_EMUR_FREERUN_ENABLE      (0x0001u)

#define CSL_USB_EMUR_RESETVAL            (0x0003u)

/* MODE1 */


#define CSL_USB_MODE1_TX4MODE_MASK       (0x3000u)
#define CSL_USB_MODE1_TX4MODE_SHIFT      (0x000Cu)
#define CSL_USB_MODE1_TX4MODE_RESETVAL   (0x0000u)
/*----TX4MODE Tokens----*/
#define CSL_USB_MODE1_TX4MODE_TRANS      (0x0000u)
#define CSL_USB_MODE1_TX4MODE_RNDIS      (0x0001u)
#define CSL_USB_MODE1_TX4MODE_CDC        (0x0002u)
#define CSL_USB_MODE1_TX4MODE_GENRNDIS   (0x0003u)


#define CSL_USB_MODE1_TX3MODE_MASK       (0x0300u)
#define CSL_USB_MODE1_TX3MODE_SHIFT      (0x0008u)
#define CSL_USB_MODE1_TX3MODE_RESETVAL   (0x0000u)
/*----TX3MODE Tokens----*/
#define CSL_USB_MODE1_TX3MODE_TRANS      (0x0000u)
#define CSL_USB_MODE1_TX3MODE_RNDIS      (0x0001u)
#define CSL_USB_MODE1_TX3MODE_CDC        (0x0002u)
#define CSL_USB_MODE1_TX3MODE_GENRNDIS   (0x0003u)


#define CSL_USB_MODE1_TX2MODE_MASK       (0x0030u)
#define CSL_USB_MODE1_TX2MODE_SHIFT      (0x0004u)
#define CSL_USB_MODE1_TX2MODE_RESETVAL   (0x0000u)
/*----TX2MODE Tokens----*/
#define CSL_USB_MODE1_TX2MODE_TRANS      (0x0000u)
#define CSL_USB_MODE1_TX2MODE_RNDIS      (0x0001u)
#define CSL_USB_MODE1_TX2MODE_CDC        (0x0002u)
#define CSL_USB_MODE1_TX2MODE_GENRNDIS   (0x0003u)


#define CSL_USB_MODE1_TX1MODE_MASK       (0x0003u)
#define CSL_USB_MODE1_TX1MODE_SHIFT      (0x0000u)
#define CSL_USB_MODE1_TX1MODE_RESETVAL   (0x0000u)
/*----TX1MODE Tokens----*/
#define CSL_USB_MODE1_TX1MODE_TRANS      (0x0000u)
#define CSL_USB_MODE1_TX1MODE_RNDIS      (0x0001u)
#define CSL_USB_MODE1_TX1MODE_CDC        (0x0002u)
#define CSL_USB_MODE1_TX1MODE_GENRNDIS   (0x0003u)

#define CSL_USB_MODE1_RESETVAL           (0x0000u)

/* MODE2 */


#define CSL_USB_MODE2_RX4MODE_MASK       (0x3000u)
#define CSL_USB_MODE2_RX4MODE_SHIFT      (0x000Cu)
#define CSL_USB_MODE2_RX4MODE_RESETVAL   (0x0000u)
/*----RX4MODE Tokens----*/
#define CSL_USB_MODE2_RX4MODE_TRANS      (0x0000u)
#define CSL_USB_MODE2_RX4MODE_RNDIS      (0x0001u)
#define CSL_USB_MODE2_RX4MODE_CDC        (0x0002u)
#define CSL_USB_MODE2_RX4MODE_GENRNDIS   (0x0003u)


#define CSL_USB_MODE2_RX3MODE_MASK       (0x0300u)
#define CSL_USB_MODE2_RX3MODE_SHIFT      (0x0008u)
#define CSL_USB_MODE2_RX3MODE_RESETVAL   (0x0000u)
/*----RX3MODE Tokens----*/
#define CSL_USB_MODE2_RX3MODE_TRANS      (0x0000u)
#define CSL_USB_MODE2_RX3MODE_RNDIS      (0x0001u)
#define CSL_USB_MODE2_RX3MODE_CDC        (0x0002u)
#define CSL_USB_MODE2_RX3MODE_GENRNDIS   (0x0003u)


#define CSL_USB_MODE2_RX2MODE_MASK       (0x0030u)
#define CSL_USB_MODE2_RX2MODE_SHIFT      (0x0004u)
#define CSL_USB_MODE2_RX2MODE_RESETVAL   (0x0000u)
/*----RX2MODE Tokens----*/
#define CSL_USB_MODE2_RX2MODE_TRANS      (0x0000u)
#define CSL_USB_MODE2_RX2MODE_RNDIS      (0x0001u)
#define CSL_USB_MODE2_RX2MODE_CDC        (0x0002u)
#define CSL_USB_MODE2_RX2MODE_GENRNDIS   (0x0003u)


#define CSL_USB_MODE2_RX1MODE_MASK       (0x0003u)
#define CSL_USB_MODE2_RX1MODE_SHIFT      (0x0000u)
#define CSL_USB_MODE2_RX1MODE_RESETVAL   (0x0000u)
/*----RX1MODE Tokens----*/
#define CSL_USB_MODE2_RX1MODE_TRANS      (0x0000u)
#define CSL_USB_MODE2_RX1MODE_RNDIS      (0x0001u)
#define CSL_USB_MODE2_RX1MODE_CDC        (0x0002u)
#define CSL_USB_MODE2_RX1MODE_GENRNDIS   (0x0003u)

#define CSL_USB_MODE2_RESETVAL           (0x0000u)

/* AUTOREQ */


#define CSL_USB_AUTOREQ_RX4_AUTOREQ_MASK (0x00C0u)
#define CSL_USB_AUTOREQ_RX4_AUTOREQ_SHIFT (0x0006u)
#define CSL_USB_AUTOREQ_RX4_AUTOREQ_RESETVAL (0x0000u)
/*----RX4_AUTOREQ Tokens----*/
#define CSL_USB_AUTOREQ_RX4_AUTOREQ_NOAUTO (0x0000u)
#define CSL_USB_AUTOREQ_RX4_AUTOREQ_AUTO (0x0001u)
#define CSL_USB_AUTOREQ_RX4_AUTOREQ_RSV  (0x0002u)
#define CSL_USB_AUTOREQ_RX4_AUTOREQ_AUTOALWYS (0x0003u)

#define CSL_USB_AUTOREQ_RX3_AUTOREQ_MASK (0x0030u)
#define CSL_USB_AUTOREQ_RX3_AUTOREQ_SHIFT (0x0004u)
#define CSL_USB_AUTOREQ_RX3_AUTOREQ_RESETVAL (0x0000u)
/*----RX3_AUTOREQ Tokens----*/
#define CSL_USB_AUTOREQ_RX3_AUTOREQ_NOAUTO (0x0000u)
#define CSL_USB_AUTOREQ_RX3_AUTOREQ_AUTO (0x0001u)
#define CSL_USB_AUTOREQ_RX3_AUTOREQ_RSV  (0x0002u)
#define CSL_USB_AUTOREQ_RX3_AUTOREQ_AUTOALWYS (0x0003u)

#define CSL_USB_AUTOREQ_RX2_AUTOREQ_MASK (0x000Cu)
#define CSL_USB_AUTOREQ_RX2_AUTOREQ_SHIFT (0x0002u)
#define CSL_USB_AUTOREQ_RX2_AUTOREQ_RESETVAL (0x0000u)
/*----RX2_AUTOREQ Tokens----*/
#define CSL_USB_AUTOREQ_RX2_AUTOREQ_NOAUTO (0x0000u)
#define CSL_USB_AUTOREQ_RX2_AUTOREQ_AUTO (0x0001u)
#define CSL_USB_AUTOREQ_RX2_AUTOREQ_RSV  (0x0002u)
#define CSL_USB_AUTOREQ_RX2_AUTOREQ_AUTOALWYS (0x0003u)

#define CSL_USB_AUTOREQ_RX1_AUTOREQ_MASK (0x0003u)
#define CSL_USB_AUTOREQ_RX1_AUTOREQ_SHIFT (0x0000u)
#define CSL_USB_AUTOREQ_RX1_AUTOREQ_RESETVAL (0x0000u)
/*----RX1_AUTOREQ Tokens----*/
#define CSL_USB_AUTOREQ_RX1_AUTOREQ_NOAUTO (0x0000u)
#define CSL_USB_AUTOREQ_RX1_AUTOREQ_AUTO (0x0001u)
#define CSL_USB_AUTOREQ_RX1_AUTOREQ_RSV  (0x0002u)
#define CSL_USB_AUTOREQ_RX1_AUTOREQ_AUTOALWYS (0x0003u)

#define CSL_USB_AUTOREQ_RESETVAL         (0x0000u)

/* SPRFIXTIME1 */

#define CSL_USB_SPRFIXTIME1_SRPFIXTIME_MASK (0xFFFFu)
#define CSL_USB_SPRFIXTIME1_SRPFIXTIME_SHIFT (0x0000u)
#define CSL_USB_SPRFIXTIME1_SRPFIXTIME_RESETVAL (0xDE80u)

#define CSL_USB_SPRFIXTIME1_RESETVAL     (0xDE80u)

/* SPRFIXTIME2 */

#define CSL_USB_SPRFIXTIME2_SRPFIXTIME_MASK (0xFFFFu)
#define CSL_USB_SPRFIXTIME2_SRPFIXTIME_SHIFT (0x0000u)
#define CSL_USB_SPRFIXTIME2_SRPFIXTIME_RESETVAL (0x0280u)

#define CSL_USB_SPRFIXTIME2_RESETVAL     (0x0280u)

/* TEARDOWN1 */


#define CSL_USB_TEARDOWN1_RX_TDOWN_MASK  (0x001Eu)
#define CSL_USB_TEARDOWN1_RX_TDOWN_SHIFT (0x0001u)
#define CSL_USB_TEARDOWN1_RX_TDOWN_RESETVAL (0x0000u)
/*----RX_TDOWN Tokens----*/
#define CSL_USB_TEARDOWN1_RX_TDOWN_DISABLE (0x0000u)
#define CSL_USB_TEARDOWN1_RX_TDOWN_ENABLE (0x0001u)


#define CSL_USB_TEARDOWN1_RESETVAL       (0x0000u)

/* TEARDOWN2 */


#define CSL_USB_TEARDOWN2_TX_TDOWN_MASK  (0x001Eu)
#define CSL_USB_TEARDOWN2_TX_TDOWN_SHIFT (0x0001u)
#define CSL_USB_TEARDOWN2_TX_TDOWN_RESETVAL (0x0000u)
/*----TX_TDOWN Tokens----*/
#define CSL_USB_TEARDOWN2_TX_TDOWN_DISABLE (0x0000u)
#define CSL_USB_TEARDOWN2_TX_TDOWN_ENABLE (0x0001u)


#define CSL_USB_TEARDOWN2_RESETVAL       (0x0000u)

/* INTSRCR1 */


#define CSL_USB_INTSRCR1_RX_MASK         (0x1E00u)
#define CSL_USB_INTSRCR1_RX_SHIFT        (0x0009u)
#define CSL_USB_INTSRCR1_RX_RESETVAL     (0x0000u)


#define CSL_USB_INTSRCR1_TX_MASK         (0x001Fu)
#define CSL_USB_INTSRCR1_TX_SHIFT        (0x0000u)
#define CSL_USB_INTSRCR1_TX_RESETVAL     (0x0000u)

#define CSL_USB_INTSRCR1_RESETVAL        (0x0000u)

/* INTSRCR2 */


#define CSL_USB_INTSRCR2_USB_MASK        (0x01FFu)
#define CSL_USB_INTSRCR2_USB_SHIFT       (0x0000u)
#define CSL_USB_INTSRCR2_USB_RESETVAL    (0x0000u)

#define CSL_USB_INTSRCR2_RESETVAL        (0x0000u)

/* INTSETR1 */


#define CSL_USB_INTSETR1_RX_MASK         (0x1F00u)
#define CSL_USB_INTSETR1_RX_SHIFT        (0x0008u)
#define CSL_USB_INTSETR1_RX_RESETVAL     (0x0000u)


#define CSL_USB_INTSETR1_TX_MASK         (0x001Fu)
#define CSL_USB_INTSETR1_TX_SHIFT        (0x0000u)
#define CSL_USB_INTSETR1_TX_RESETVAL     (0x0000u)

#define CSL_USB_INTSETR1_RESETVAL        (0x0000u)

/* INTSETR2 */


#define CSL_USB_INTSETR2_USB_MASK        (0x01FFu)
#define CSL_USB_INTSETR2_USB_SHIFT       (0x0000u)
#define CSL_USB_INTSETR2_USB_RESETVAL    (0x0000u)

#define CSL_USB_INTSETR2_RESETVAL        (0x0000u)

/* INTCLRR1 */


#define CSL_USB_INTCLRR1_RX_MASK         (0x1F00u)
#define CSL_USB_INTCLRR1_RX_SHIFT        (0x0008u)
#define CSL_USB_INTCLRR1_RX_RESETVAL     (0x0000u)


#define CSL_USB_INTCLRR1_TX_MASK         (0x001Fu)
#define CSL_USB_INTCLRR1_TX_SHIFT        (0x0000u)
#define CSL_USB_INTCLRR1_TX_RESETVAL     (0x0000u)

#define CSL_USB_INTCLRR1_RESETVAL        (0x0000u)

/* INTCLRR2 */


#define CSL_USB_INTCLRR2_USB_MASK        (0x01FFu)
#define CSL_USB_INTCLRR2_USB_SHIFT       (0x0000u)
#define CSL_USB_INTCLRR2_USB_RESETVAL    (0x0000u)

#define CSL_USB_INTCLRR2_RESETVAL        (0x0000u)

/* INTMSKR1 */


#define CSL_USB_INTMSKR1_RX_MASK         (0x1F00u)
#define CSL_USB_INTMSKR1_RX_SHIFT        (0x0008u)
#define CSL_USB_INTMSKR1_RX_RESETVAL     (0x0000u)


#define CSL_USB_INTMSKR1_TX_MASK         (0x001Fu)
#define CSL_USB_INTMSKR1_TX_SHIFT        (0x0000u)
#define CSL_USB_INTMSKR1_TX_RESETVAL     (0x0000u)

#define CSL_USB_INTMSKR1_RESETVAL        (0x0000u)

/* INTMSKR2 */


#define CSL_USB_INTMSKR2_USB_MASK        (0x01FFu)
#define CSL_USB_INTMSKR2_USB_SHIFT       (0x0000u)
#define CSL_USB_INTMSKR2_USB_RESETVAL    (0x0000u)

#define CSL_USB_INTMSKR2_RESETVAL        (0x0000u)

/* INTMSKSETR1 */


#define CSL_USB_INTMSKSETR1_RX_MASK      (0x1F00u)
#define CSL_USB_INTMSKSETR1_RX_SHIFT     (0x0008u)
#define CSL_USB_INTMSKSETR1_RX_RESETVAL  (0x0000u)


#define CSL_USB_INTMSKSETR1_TX_MASK      (0x001Fu)
#define CSL_USB_INTMSKSETR1_TX_SHIFT     (0x0000u)
#define CSL_USB_INTMSKSETR1_TX_RESETVAL  (0x0000u)

#define CSL_USB_INTMSKSETR1_RESETVAL     (0x0000u)

/* INTMSKSETR2 */


#define CSL_USB_INTMSKSETR2_USB_MASK     (0x01FFu)
#define CSL_USB_INTMSKSETR2_USB_SHIFT    (0x0000u)
#define CSL_USB_INTMSKSETR2_USB_RESETVAL (0x0000u)

#define CSL_USB_INTMSKSETR2_RESETVAL     (0x0000u)

/* INTMSKCLRR1 */


#define CSL_USB_INTMSKCLRR1_RX_MASK      (0x1F00u)
#define CSL_USB_INTMSKCLRR1_RX_SHIFT     (0x0008u)
#define CSL_USB_INTMSKCLRR1_RX_RESETVAL  (0x0000u)


#define CSL_USB_INTMSKCLRR1_TX_MASK      (0x001Fu)
#define CSL_USB_INTMSKCLRR1_TX_SHIFT     (0x0000u)
#define CSL_USB_INTMSKCLRR1_TX_RESETVAL  (0x0000u)

#define CSL_USB_INTMSKCLRR1_RESETVAL     (0x0000u)

/* INTMSKCLRR2 */


#define CSL_USB_INTMSKCLRR2_USB_MASK     (0x01FFu)
#define CSL_USB_INTMSKCLRR2_USB_SHIFT    (0x0000u)
#define CSL_USB_INTMSKCLRR2_USB_RESETVAL (0x0000u)

#define CSL_USB_INTMSKCLRR2_RESETVAL     (0x0000u)

/* INTMASKEDR1 */


#define CSL_USB_INTMASKEDR1_RX_MASK      (0x1F00u)
#define CSL_USB_INTMASKEDR1_RX_SHIFT     (0x0008u)
#define CSL_USB_INTMASKEDR1_RX_RESETVAL  (0x0000u)


#define CSL_USB_INTMASKEDR1_TX_MASK      (0x001Fu)
#define CSL_USB_INTMASKEDR1_TX_SHIFT     (0x0000u)
#define CSL_USB_INTMASKEDR1_TX_RESETVAL  (0x0000u)

#define CSL_USB_INTMASKEDR1_RESETVAL     (0x0000u)

/* INTMASKEDR2 */


#define CSL_USB_INTMASKEDR2_USB_MASK     (0x01FFu)
#define CSL_USB_INTMASKEDR2_USB_SHIFT    (0x0000u)
#define CSL_USB_INTMASKEDR2_USB_RESETVAL (0x0000u)

#define CSL_USB_INTMASKEDR2_RESETVAL     (0x0000u)

/* EOIR */


#define CSL_USB_EOIR_EOI_VECTOR_MASK     (0x00FFu)
#define CSL_USB_EOIR_EOI_VECTOR_SHIFT    (0x0000u)
#define CSL_USB_EOIR_EOI_VECTOR_RESETVAL (0x0000u)

#define CSL_USB_EOIR_RESETVAL            (0x0000u)

/* INTVECTR1 */

#define CSL_USB_INTVECTR1_VECTOR_MASK    (0xFFFFu)
#define CSL_USB_INTVECTR1_VECTOR_SHIFT   (0x0000u)
#define CSL_USB_INTVECTR1_VECTOR_RESETVAL (0x0000u)

#define CSL_USB_INTVECTR1_RESETVAL       (0x0000u)

/* INTVECTR2 */

#define CSL_USB_INTVECTR2_VECTOR_MASK    (0xFFFFu)
#define CSL_USB_INTVECTR2_VECTOR_SHIFT   (0x0000u)
#define CSL_USB_INTVECTR2_VECTOR_RESETVAL (0x0000u)

#define CSL_USB_INTVECTR2_RESETVAL       (0x0000u)

/* GREP1SZR1 */

#define CSL_USB_GREP1SZR1_SIZE_MASK      (0xFFFFu)
#define CSL_USB_GREP1SZR1_SIZE_SHIFT     (0x0000u)
#define CSL_USB_GREP1SZR1_SIZE_RESETVAL  (0x0000u)

#define CSL_USB_GREP1SZR1_RESETVAL       (0x0000u)

/* GREP1SZR2 */


#define CSL_USB_GREP1SZR2_SIZE_MASK      (0x0001u)
#define CSL_USB_GREP1SZR2_SIZE_SHIFT     (0x0000u)
#define CSL_USB_GREP1SZR2_SIZE_RESETVAL  (0x0000u)

#define CSL_USB_GREP1SZR2_RESETVAL       (0x0000u)

/* GREP2SZR1 */

#define CSL_USB_GREP2SZR1_SIZE_MASK      (0xFFFFu)
#define CSL_USB_GREP2SZR1_SIZE_SHIFT     (0x0000u)
#define CSL_USB_GREP2SZR1_SIZE_RESETVAL  (0x0000u)

#define CSL_USB_GREP2SZR1_RESETVAL       (0x0000u)

/* GREP2SZR2 */


#define CSL_USB_GREP2SZR2_SIZE_MASK      (0x0001u)
#define CSL_USB_GREP2SZR2_SIZE_SHIFT     (0x0000u)
#define CSL_USB_GREP2SZR2_SIZE_RESETVAL  (0x0000u)

#define CSL_USB_GREP2SZR2_RESETVAL       (0x0000u)

/* GREP3SZR1 */

#define CSL_USB_GREP3SZR1_SIZE_MASK      (0xFFFFu)
#define CSL_USB_GREP3SZR1_SIZE_SHIFT     (0x0000u)
#define CSL_USB_GREP3SZR1_SIZE_RESETVAL  (0x0000u)

#define CSL_USB_GREP3SZR1_RESETVAL       (0x0000u)

/* GREP3SZR2 */


#define CSL_USB_GREP3SZR2_SIZE_MASK      (0x0001u)
#define CSL_USB_GREP3SZR2_SIZE_SHIFT     (0x0000u)
#define CSL_USB_GREP3SZR2_SIZE_RESETVAL  (0x0000u)

#define CSL_USB_GREP3SZR2_RESETVAL       (0x0000u)

/* GREP4SZR1 */

#define CSL_USB_GREP4SZR1_SIZE_MASK      (0xFFFFu)
#define CSL_USB_GREP4SZR1_SIZE_SHIFT     (0x0000u)
#define CSL_USB_GREP4SZR1_SIZE_RESETVAL  (0x0000u)

#define CSL_USB_GREP4SZR1_RESETVAL       (0x0000u)

/* GREP4SZR2 */


#define CSL_USB_GREP4SZR2_SIZE_MASK      (0x0001u)
#define CSL_USB_GREP4SZR2_SIZE_SHIFT     (0x0000u)
#define CSL_USB_GREP4SZR2_SIZE_RESETVAL  (0x0000u)

#define CSL_USB_GREP4SZR2_RESETVAL       (0x0000u)

/* FADDR_POWER */

#define CSL_USB_FADDR_POWER_ISOUPDATE_MASK (0x8000u)
#define CSL_USB_FADDR_POWER_ISOUPDATE_SHIFT (0x000Fu)
#define CSL_USB_FADDR_POWER_ISOUPDATE_RESETVAL (0x0000u)

#define CSL_USB_FADDR_POWER_SOFTCONN_MASK (0x4000u)
#define CSL_USB_FADDR_POWER_SOFTCONN_SHIFT (0x000Eu)
#define CSL_USB_FADDR_POWER_SOFTCONN_RESETVAL (0x0000u)

#define CSL_USB_FADDR_POWER_HSEN_MASK    (0x2000u)
#define CSL_USB_FADDR_POWER_HSEN_SHIFT   (0x000Du)
#define CSL_USB_FADDR_POWER_HSEN_RESETVAL (0x0001u)

#define CSL_USB_FADDR_POWER_HSMODE_MASK  (0x1000u)
#define CSL_USB_FADDR_POWER_HSMODE_SHIFT (0x000Cu)
#define CSL_USB_FADDR_POWER_HSMODE_RESETVAL (0x0000u)

#define CSL_USB_FADDR_POWER_RESET_MASK   (0x0800u)
#define CSL_USB_FADDR_POWER_RESET_SHIFT  (0x000Bu)
#define CSL_USB_FADDR_POWER_RESET_RESETVAL (0x0000u)

#define CSL_USB_FADDR_POWER_RESUME_MASK  (0x0400u)
#define CSL_USB_FADDR_POWER_RESUME_SHIFT (0x000Au)
#define CSL_USB_FADDR_POWER_RESUME_RESETVAL (0x0000u)

#define CSL_USB_FADDR_POWER_SUSPENDM_MASK (0x0200u)
#define CSL_USB_FADDR_POWER_SUSPENDM_SHIFT (0x0009u)
#define CSL_USB_FADDR_POWER_SUSPENDM_RESETVAL (0x0000u)

#define CSL_USB_FADDR_POWER_ENSUSPM_MASK (0x0100u)
#define CSL_USB_FADDR_POWER_ENSUSPM_SHIFT (0x0008u)
#define CSL_USB_FADDR_POWER_ENSUSPM_RESETVAL (0x0000u)


#define CSL_USB_FADDR_POWER_FUNCADDR_MASK (0x007Fu)
#define CSL_USB_FADDR_POWER_FUNCADDR_SHIFT (0x0000u)
#define CSL_USB_FADDR_POWER_FUNCADDR_RESETVAL (0x0000u)

#define CSL_USB_FADDR_POWER_RESETVAL     (0x2000u)

/* INTRTX */


#define CSL_USB_INTRTX_EP4TX_MASK        (0x0010u)
#define CSL_USB_INTRTX_EP4TX_SHIFT       (0x0004u)
#define CSL_USB_INTRTX_EP4TX_RESETVAL    (0x0000u)

#define CSL_USB_INTRTX_EP3TX_MASK        (0x0008u)
#define CSL_USB_INTRTX_EP3TX_SHIFT       (0x0003u)
#define CSL_USB_INTRTX_EP3TX_RESETVAL    (0x0000u)

#define CSL_USB_INTRTX_EP2TX_MASK        (0x0004u)
#define CSL_USB_INTRTX_EP2TX_SHIFT       (0x0002u)
#define CSL_USB_INTRTX_EP2TX_RESETVAL    (0x0000u)

#define CSL_USB_INTRTX_EP1TX_MASK        (0x0002u)
#define CSL_USB_INTRTX_EP1TX_SHIFT       (0x0001u)
#define CSL_USB_INTRTX_EP1TX_RESETVAL    (0x0000u)

#define CSL_USB_INTRTX_EP0_MASK          (0x0001u)
#define CSL_USB_INTRTX_EP0_SHIFT         (0x0000u)
#define CSL_USB_INTRTX_EP0_RESETVAL      (0x0000u)

#define CSL_USB_INTRTX_RESETVAL          (0x0000u)

/* INTRRX */


#define CSL_USB_INTRRX_EP4RX_MASK        (0x0010u)
#define CSL_USB_INTRRX_EP4RX_SHIFT       (0x0004u)
#define CSL_USB_INTRRX_EP4RX_RESETVAL    (0x0000u)

#define CSL_USB_INTRRX_EP3RX_MASK        (0x0008u)
#define CSL_USB_INTRRX_EP3RX_SHIFT       (0x0003u)
#define CSL_USB_INTRRX_EP3RX_RESETVAL    (0x0000u)

#define CSL_USB_INTRRX_EP2RX_MASK        (0x0004u)
#define CSL_USB_INTRRX_EP2RX_SHIFT       (0x0002u)
#define CSL_USB_INTRRX_EP2RX_RESETVAL    (0x0000u)

#define CSL_USB_INTRRX_EP1RX_MASK        (0x0002u)
#define CSL_USB_INTRRX_EP1RX_SHIFT       (0x0001u)
#define CSL_USB_INTRRX_EP1RX_RESETVAL    (0x0000u)


#define CSL_USB_INTRRX_RESETVAL          (0x0000u)

/* INTRTXE */


#define CSL_USB_INTRTXE_EP4TX_MASK       (0x0010u)
#define CSL_USB_INTRTXE_EP4TX_SHIFT      (0x0004u)
#define CSL_USB_INTRTXE_EP4TX_RESETVAL   (0x0001u)

#define CSL_USB_INTRTXE_EP3TX_MASK       (0x0008u)
#define CSL_USB_INTRTXE_EP3TX_SHIFT      (0x0003u)
#define CSL_USB_INTRTXE_EP3TX_RESETVAL   (0x0001u)

#define CSL_USB_INTRTXE_EP2TX_MASK       (0x0004u)
#define CSL_USB_INTRTXE_EP2TX_SHIFT      (0x0002u)
#define CSL_USB_INTRTXE_EP2TX_RESETVAL   (0x0001u)

#define CSL_USB_INTRTXE_EP1TX_MASK       (0x0002u)
#define CSL_USB_INTRTXE_EP1TX_SHIFT      (0x0001u)
#define CSL_USB_INTRTXE_EP1TX_RESETVAL   (0x0001u)

#define CSL_USB_INTRTXE_EP0_MASK         (0x0001u)
#define CSL_USB_INTRTXE_EP0_SHIFT        (0x0000u)
#define CSL_USB_INTRTXE_EP0_RESETVAL     (0x0001u)

#define CSL_USB_INTRTXE_RESETVAL         (0x001Fu)

/* INTRRXE */


#define CSL_USB_INTRRXE_EP4RX_MASK       (0x0010u)
#define CSL_USB_INTRRXE_EP4RX_SHIFT      (0x0004u)
#define CSL_USB_INTRRXE_EP4RX_RESETVAL   (0x0001u)

#define CSL_USB_INTRRXE_EP3RX_MASK       (0x0008u)
#define CSL_USB_INTRRXE_EP3RX_SHIFT      (0x0003u)
#define CSL_USB_INTRRXE_EP3RX_RESETVAL   (0x0001u)

#define CSL_USB_INTRRXE_EP2RX_MASK       (0x0004u)
#define CSL_USB_INTRRXE_EP2RX_SHIFT      (0x0002u)
#define CSL_USB_INTRRXE_EP2RX_RESETVAL   (0x0001u)

#define CSL_USB_INTRRXE_EP1RX_MASK       (0x0002u)
#define CSL_USB_INTRRXE_EP1RX_SHIFT      (0x0001u)
#define CSL_USB_INTRRXE_EP1RX_RESETVAL   (0x0001u)


#define CSL_USB_INTRRXE_RESETVAL         (0x001Eu)

/* INTRUSB_INTRUSBE */

#define CSL_USB_INTRUSB_INTRUSBE_VBUSERR_E_MASK (0x8000u)
#define CSL_USB_INTRUSB_INTRUSBE_VBUSERR_E_SHIFT (0x000Fu)
#define CSL_USB_INTRUSB_INTRUSBE_VBUSERR_E_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_SESSREQ_E_MASK (0x4000u)
#define CSL_USB_INTRUSB_INTRUSBE_SESSREQ_E_SHIFT (0x000Eu)
#define CSL_USB_INTRUSB_INTRUSBE_SESSREQ_E_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_DISCON_E_MASK (0x2000u)
#define CSL_USB_INTRUSB_INTRUSBE_DISCON_E_SHIFT (0x000Du)
#define CSL_USB_INTRUSB_INTRUSBE_DISCON_E_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_CONN_E_MASK (0x1000u)
#define CSL_USB_INTRUSB_INTRUSBE_CONN_E_SHIFT (0x000Cu)
#define CSL_USB_INTRUSB_INTRUSBE_CONN_E_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_SOF_E_MASK (0x0800u)
#define CSL_USB_INTRUSB_INTRUSBE_SOF_E_SHIFT (0x000Bu)
#define CSL_USB_INTRUSB_INTRUSBE_SOF_E_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_RESET_BABBLE_E_MASK (0x0400u)
#define CSL_USB_INTRUSB_INTRUSBE_RESET_BABBLE_E_SHIFT (0x000Au)
#define CSL_USB_INTRUSB_INTRUSBE_RESET_BABBLE_E_RESETVAL (0x0001u)

#define CSL_USB_INTRUSB_INTRUSBE_RESUME_E_MASK (0x0200u)
#define CSL_USB_INTRUSB_INTRUSBE_RESUME_E_SHIFT (0x0009u)
#define CSL_USB_INTRUSB_INTRUSBE_RESUME_E_RESETVAL (0x0001u)

#define CSL_USB_INTRUSB_INTRUSBE_SUSPEND_E_MASK (0x0100u)
#define CSL_USB_INTRUSB_INTRUSBE_SUSPEND_E_SHIFT (0x0008u)
#define CSL_USB_INTRUSB_INTRUSBE_SUSPEND_E_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_VBUSERR_MASK (0x0080u)
#define CSL_USB_INTRUSB_INTRUSBE_VBUSERR_SHIFT (0x0007u)
#define CSL_USB_INTRUSB_INTRUSBE_VBUSERR_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_SESSREQ_MASK (0x0040u)
#define CSL_USB_INTRUSB_INTRUSBE_SESSREQ_SHIFT (0x0006u)
#define CSL_USB_INTRUSB_INTRUSBE_SESSREQ_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_DISCON_MASK (0x0020u)
#define CSL_USB_INTRUSB_INTRUSBE_DISCON_SHIFT (0x0005u)
#define CSL_USB_INTRUSB_INTRUSBE_DISCON_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_CONN_MASK (0x0010u)
#define CSL_USB_INTRUSB_INTRUSBE_CONN_SHIFT (0x0004u)
#define CSL_USB_INTRUSB_INTRUSBE_CONN_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_SOF_MASK (0x0008u)
#define CSL_USB_INTRUSB_INTRUSBE_SOF_SHIFT (0x0003u)
#define CSL_USB_INTRUSB_INTRUSBE_SOF_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_RESET_BABBLE_MASK (0x0004u)
#define CSL_USB_INTRUSB_INTRUSBE_RESET_BABBLE_SHIFT (0x0002u)
#define CSL_USB_INTRUSB_INTRUSBE_RESET_BABBLE_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_RESUME_MASK (0x0002u)
#define CSL_USB_INTRUSB_INTRUSBE_RESUME_SHIFT (0x0001u)
#define CSL_USB_INTRUSB_INTRUSBE_RESUME_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_SUSPEND_MASK (0x0001u)
#define CSL_USB_INTRUSB_INTRUSBE_SUSPEND_SHIFT (0x0000u)
#define CSL_USB_INTRUSB_INTRUSBE_SUSPEND_RESETVAL (0x0000u)

#define CSL_USB_INTRUSB_INTRUSBE_RESETVAL (0x0600u)

/* FRAME */


#define CSL_USB_FRAME_FRAMENUMBER_MASK   (0x07FFu)
#define CSL_USB_FRAME_FRAMENUMBER_SHIFT  (0x0000u)
#define CSL_USB_FRAME_FRAMENUMBER_RESETVAL (0x0000u)

#define CSL_USB_FRAME_RESETVAL           (0x0000u)

/* INDEX_TESTMODE */

#define CSL_USB_INDEX_TESTMODE_FORCE_HOST_MASK (0x8000u)
#define CSL_USB_INDEX_TESTMODE_FORCE_HOST_SHIFT (0x000Fu)
#define CSL_USB_INDEX_TESTMODE_FORCE_HOST_RESETVAL (0x0000u)

#define CSL_USB_INDEX_TESTMODE_FIFO_ACCESS_MASK (0x4000u)
#define CSL_USB_INDEX_TESTMODE_FIFO_ACCESS_SHIFT (0x000Eu)
#define CSL_USB_INDEX_TESTMODE_FIFO_ACCESS_RESETVAL (0x0000u)

#define CSL_USB_INDEX_TESTMODE_FORCE_FS_MASK (0x2000u)
#define CSL_USB_INDEX_TESTMODE_FORCE_FS_SHIFT (0x000Du)
#define CSL_USB_INDEX_TESTMODE_FORCE_FS_RESETVAL (0x0000u)

#define CSL_USB_INDEX_TESTMODE_FORCE_HS_MASK (0x1000u)
#define CSL_USB_INDEX_TESTMODE_FORCE_HS_SHIFT (0x000Cu)
#define CSL_USB_INDEX_TESTMODE_FORCE_HS_RESETVAL (0x0000u)

#define CSL_USB_INDEX_TESTMODE_TEST_PACKET_MASK (0x0800u)
#define CSL_USB_INDEX_TESTMODE_TEST_PACKET_SHIFT (0x000Bu)
#define CSL_USB_INDEX_TESTMODE_TEST_PACKET_RESETVAL (0x0000u)

#define CSL_USB_INDEX_TESTMODE_TEST_K_MASK (0x0400u)
#define CSL_USB_INDEX_TESTMODE_TEST_K_SHIFT (0x000Au)
#define CSL_USB_INDEX_TESTMODE_TEST_K_RESETVAL (0x0000u)

#define CSL_USB_INDEX_TESTMODE_TEST_J_MASK (0x0200u)
#define CSL_USB_INDEX_TESTMODE_TEST_J_SHIFT (0x0009u)
#define CSL_USB_INDEX_TESTMODE_TEST_J_RESETVAL (0x0000u)

#define CSL_USB_INDEX_TESTMODE_TEST_SE0_NAK_MASK (0x0100u)
#define CSL_USB_INDEX_TESTMODE_TEST_SE0_NAK_SHIFT (0x0008u)
#define CSL_USB_INDEX_TESTMODE_TEST_SE0_NAK_RESETVAL (0x0000u)


#define CSL_USB_INDEX_TESTMODE_EPSEL_MASK (0x000Fu)
#define CSL_USB_INDEX_TESTMODE_EPSEL_SHIFT (0x0000u)
#define CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL (0x0000u)

#define CSL_USB_INDEX_TESTMODE_RESETVAL  (0x0000u)

/* TXMAXP_INDX */


#define CSL_USB_TXMAXP_INDX_MAXPAYLOAD_MASK (0x07FFu)
#define CSL_USB_TXMAXP_INDX_MAXPAYLOAD_SHIFT (0x0000u)
#define CSL_USB_TXMAXP_INDX_MAXPAYLOAD_RESETVAL (0x0000u)

#define CSL_USB_TXMAXP_INDX_RESETVAL     (0x0000u)

/* PERI_CSR0_INDX */


#define CSL_USB_PERI_CSR0_INDX_FLUSHFIFO_MASK (0x0100u)
#define CSL_USB_PERI_CSR0_INDX_FLUSHFIFO_SHIFT (0x0008u)
#define CSL_USB_PERI_CSR0_INDX_FLUSHFIFO_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_INDX_SERV_SETUPEND_MASK (0x0080u)
#define CSL_USB_PERI_CSR0_INDX_SERV_SETUPEND_SHIFT (0x0007u)
#define CSL_USB_PERI_CSR0_INDX_SERV_SETUPEND_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_INDX_SERV_RXPKTRDY_MASK (0x0040u)
#define CSL_USB_PERI_CSR0_INDX_SERV_RXPKTRDY_SHIFT (0x0006u)
#define CSL_USB_PERI_CSR0_INDX_SERV_RXPKTRDY_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_INDX_SENDSTALL_MASK (0x0020u)
#define CSL_USB_PERI_CSR0_INDX_SENDSTALL_SHIFT (0x0005u)
#define CSL_USB_PERI_CSR0_INDX_SENDSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_INDX_SETUPEND_MASK (0x0010u)
#define CSL_USB_PERI_CSR0_INDX_SETUPEND_SHIFT (0x0004u)
#define CSL_USB_PERI_CSR0_INDX_SETUPEND_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_INDX_DATAEND_MASK (0x0008u)
#define CSL_USB_PERI_CSR0_INDX_DATAEND_SHIFT (0x0003u)
#define CSL_USB_PERI_CSR0_INDX_DATAEND_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_INDX_SENTSTALL_MASK (0x0004u)
#define CSL_USB_PERI_CSR0_INDX_SENTSTALL_SHIFT (0x0002u)
#define CSL_USB_PERI_CSR0_INDX_SENTSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_INDX_TXPKTRDY_MASK (0x0002u)
#define CSL_USB_PERI_CSR0_INDX_TXPKTRDY_SHIFT (0x0001u)
#define CSL_USB_PERI_CSR0_INDX_TXPKTRDY_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_INDX_RXPKTRDY_MASK (0x0001u)
#define CSL_USB_PERI_CSR0_INDX_RXPKTRDY_SHIFT (0x0000u)
#define CSL_USB_PERI_CSR0_INDX_RXPKTRDY_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_INDX_RESETVAL  (0x0000u)

/* PERI_TXCSR_INDX */

#define CSL_USB_PERI_TXCSR_INDX_AUTOSET_MASK (0x8000u)
#define CSL_USB_PERI_TXCSR_INDX_AUTOSET_SHIFT (0x000Fu)
#define CSL_USB_PERI_TXCSR_INDX_AUTOSET_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_INDX_ISO_MASK (0x4000u)
#define CSL_USB_PERI_TXCSR_INDX_ISO_SHIFT (0x000Eu)
#define CSL_USB_PERI_TXCSR_INDX_ISO_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_INDX_MODE_MASK (0x2000u)
#define CSL_USB_PERI_TXCSR_INDX_MODE_SHIFT (0x000Du)
#define CSL_USB_PERI_TXCSR_INDX_MODE_RESETVAL (0x0001u)

#define CSL_USB_PERI_TXCSR_INDX_DMAEN_MASK (0x1000u)
#define CSL_USB_PERI_TXCSR_INDX_DMAEN_SHIFT (0x000Cu)
#define CSL_USB_PERI_TXCSR_INDX_DMAEN_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_INDX_FRCDATATOG_MASK (0x0800u)
#define CSL_USB_PERI_TXCSR_INDX_FRCDATATOG_SHIFT (0x000Bu)
#define CSL_USB_PERI_TXCSR_INDX_FRCDATATOG_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_INDX_DMAMODE_MASK (0x0400u)
#define CSL_USB_PERI_TXCSR_INDX_DMAMODE_SHIFT (0x000Au)
#define CSL_USB_PERI_TXCSR_INDX_DMAMODE_RESETVAL (0x0000u)


#define CSL_USB_PERI_TXCSR_INDX_CLRDATATOG_MASK (0x0040u)
#define CSL_USB_PERI_TXCSR_INDX_CLRDATATOG_SHIFT (0x0006u)
#define CSL_USB_PERI_TXCSR_INDX_CLRDATATOG_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_INDX_SENTSTALL_MASK (0x0020u)
#define CSL_USB_PERI_TXCSR_INDX_SENTSTALL_SHIFT (0x0005u)
#define CSL_USB_PERI_TXCSR_INDX_SENTSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_INDX_SENDSTALL_MASK (0x0010u)
#define CSL_USB_PERI_TXCSR_INDX_SENDSTALL_SHIFT (0x0004u)
#define CSL_USB_PERI_TXCSR_INDX_SENDSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_INDX_FLUSHFIFO_MASK (0x0008u)
#define CSL_USB_PERI_TXCSR_INDX_FLUSHFIFO_SHIFT (0x0003u)
#define CSL_USB_PERI_TXCSR_INDX_FLUSHFIFO_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_INDX_UNDERRUN_MASK (0x0004u)
#define CSL_USB_PERI_TXCSR_INDX_UNDERRUN_SHIFT (0x0002u)
#define CSL_USB_PERI_TXCSR_INDX_UNDERRUN_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_INDX_FIFONOTEMPTY_MASK (0x0002u)
#define CSL_USB_PERI_TXCSR_INDX_FIFONOTEMPTY_SHIFT (0x0001u)
#define CSL_USB_PERI_TXCSR_INDX_FIFONOTEMPTY_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_INDX_TXPKTRDY_MASK (0x0001u)
#define CSL_USB_PERI_TXCSR_INDX_TXPKTRDY_SHIFT (0x0000u)
#define CSL_USB_PERI_TXCSR_INDX_TXPKTRDY_RESETVAL (0x0000u)

#define CSL_USB_PERI_TXCSR_INDX_RESETVAL (0x2000u)

/* RXMAXP_INDX */


#define CSL_USB_RXMAXP_INDX_MAXPAYLOAD_MASK (0x07FFu)
#define CSL_USB_RXMAXP_INDX_MAXPAYLOAD_SHIFT (0x0000u)
#define CSL_USB_RXMAXP_INDX_MAXPAYLOAD_RESETVAL (0x0000u)

#define CSL_USB_RXMAXP_INDX_RESETVAL     (0x0000u)

/* PERI_RXCSR_INDX */

#define CSL_USB_PERI_RXCSR_INDX_AUTOCLEAR_MASK (0x8000u)
#define CSL_USB_PERI_RXCSR_INDX_AUTOCLEAR_SHIFT (0x000Fu)
#define CSL_USB_PERI_RXCSR_INDX_AUTOCLEAR_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_ISO_MASK (0x4000u)
#define CSL_USB_PERI_RXCSR_INDX_ISO_SHIFT (0x000Eu)
#define CSL_USB_PERI_RXCSR_INDX_ISO_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_DMAEN_MASK (0x2000u)
#define CSL_USB_PERI_RXCSR_INDX_DMAEN_SHIFT (0x000Du)
#define CSL_USB_PERI_RXCSR_INDX_DMAEN_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_DISNYET_MASK (0x1000u)
#define CSL_USB_PERI_RXCSR_INDX_DISNYET_SHIFT (0x000Cu)
#define CSL_USB_PERI_RXCSR_INDX_DISNYET_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_DMAMODE_MASK (0x0800u)
#define CSL_USB_PERI_RXCSR_INDX_DMAMODE_SHIFT (0x000Bu)
#define CSL_USB_PERI_RXCSR_INDX_DMAMODE_RESETVAL (0x0000u)


#define CSL_USB_PERI_RXCSR_INDX_CLRDATATOG_MASK (0x0080u)
#define CSL_USB_PERI_RXCSR_INDX_CLRDATATOG_SHIFT (0x0007u)
#define CSL_USB_PERI_RXCSR_INDX_CLRDATATOG_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_SENTSTALL_MASK (0x0040u)
#define CSL_USB_PERI_RXCSR_INDX_SENTSTALL_SHIFT (0x0006u)
#define CSL_USB_PERI_RXCSR_INDX_SENTSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_SENDSTALL_MASK (0x0020u)
#define CSL_USB_PERI_RXCSR_INDX_SENDSTALL_SHIFT (0x0005u)
#define CSL_USB_PERI_RXCSR_INDX_SENDSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_FLUSHFIFO_MASK (0x0010u)
#define CSL_USB_PERI_RXCSR_INDX_FLUSHFIFO_SHIFT (0x0004u)
#define CSL_USB_PERI_RXCSR_INDX_FLUSHFIFO_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_DATAERROR_MASK (0x0008u)
#define CSL_USB_PERI_RXCSR_INDX_DATAERROR_SHIFT (0x0003u)
#define CSL_USB_PERI_RXCSR_INDX_DATAERROR_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_OVERRUN_MASK (0x0004u)
#define CSL_USB_PERI_RXCSR_INDX_OVERRUN_SHIFT (0x0002u)
#define CSL_USB_PERI_RXCSR_INDX_OVERRUN_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_FIFOFULL_MASK (0x0002u)
#define CSL_USB_PERI_RXCSR_INDX_FIFOFULL_SHIFT (0x0001u)
#define CSL_USB_PERI_RXCSR_INDX_FIFOFULL_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_RXPKTRDY_MASK (0x0001u)
#define CSL_USB_PERI_RXCSR_INDX_RXPKTRDY_SHIFT (0x0000u)
#define CSL_USB_PERI_RXCSR_INDX_RXPKTRDY_RESETVAL (0x0000u)

#define CSL_USB_PERI_RXCSR_INDX_RESETVAL (0x0000u)

/* COUNT0_INDX */


#define CSL_USB_COUNT0_INDX_EP0RXCOUNT_MASK (0x007Fu)
#define CSL_USB_COUNT0_INDX_EP0RXCOUNT_SHIFT (0x0000u)
#define CSL_USB_COUNT0_INDX_EP0RXCOUNT_RESETVAL (0x0000u)

#define CSL_USB_COUNT0_INDX_RESETVAL     (0x0000u)

/* RXCOUNT_INDX */


#define CSL_USB_RXCOUNT_INDX_EPRXCOUNT_MASK (0x1FFFu)
#define CSL_USB_RXCOUNT_INDX_EPRXCOUNT_SHIFT (0x0000u)
#define CSL_USB_RXCOUNT_INDX_EPRXCOUNT_RESETVAL (0x0000u)

#define CSL_USB_RXCOUNT_INDX_RESETVAL    (0x0000u)

/* CONFIGDATA_INDX */



#define CSL_USB_CONFIGDATA_INDX_MPTXE_MASK (0x0040u)
#define CSL_USB_CONFIGDATA_INDX_MPTXE_SHIFT (0x0006u)
#define CSL_USB_CONFIGDATA_INDX_MPTXE_RESETVAL (0x0000u)
/*----MPTXE Tokens----*/
#define CSL_USB_CONFIGDATA_INDX_MPTXE_NOAUTO (0x0000u)
#define CSL_USB_CONFIGDATA_INDX_MPTXE_AUTO (0x0001u)

#define CSL_USB_CONFIGDATA_INDX_BIGENDIAN_MASK (0x0020u)
#define CSL_USB_CONFIGDATA_INDX_BIGENDIAN_SHIFT (0x0005u)
#define CSL_USB_CONFIGDATA_INDX_BIGENDIAN_RESETVAL (0x0000u)
/*----BIGENDIAN Tokens----*/
#define CSL_USB_CONFIGDATA_INDX_BIGENDIAN_LITTLE (0x0000u)
#define CSL_USB_CONFIGDATA_INDX_BIGENDIAN_BIG (0x0001u)

#define CSL_USB_CONFIGDATA_INDX_HBRXE_MASK (0x0010u)
#define CSL_USB_CONFIGDATA_INDX_HBRXE_SHIFT (0x0004u)
#define CSL_USB_CONFIGDATA_INDX_HBRXE_RESETVAL (0x0000u)
/*----HBRXE Tokens----*/
#define CSL_USB_CONFIGDATA_INDX_HBRXE_NOHIGHBW (0x0000u)
#define CSL_USB_CONFIGDATA_INDX_HBRXE_HIGHBW (0x0001u)

#define CSL_USB_CONFIGDATA_INDX_HBTXE_MASK (0x0008u)
#define CSL_USB_CONFIGDATA_INDX_HBTXE_SHIFT (0x0003u)
#define CSL_USB_CONFIGDATA_INDX_HBTXE_RESETVAL (0x0000u)
/*----HBTXE Tokens----*/
#define CSL_USB_CONFIGDATA_INDX_HBTXE_NOHIGHBW (0x0000u)
#define CSL_USB_CONFIGDATA_INDX_HBTXE_HIGHBW (0x0001u)

#define CSL_USB_CONFIGDATA_INDX_DYNFIFO_MASK (0x0004u)
#define CSL_USB_CONFIGDATA_INDX_DYNFIFO_SHIFT (0x0002u)
#define CSL_USB_CONFIGDATA_INDX_DYNFIFO_RESETVAL (0x0000u)
/*----DYNFIFO Tokens----*/
#define CSL_USB_CONFIGDATA_INDX_DYNFIFO_NODYMFIFO (0x0000u)
#define CSL_USB_CONFIGDATA_INDX_DYNFIFO_DYMFIFO (0x0001u)

#define CSL_USB_CONFIGDATA_INDX_SOFTCONE_MASK (0x0002u)
#define CSL_USB_CONFIGDATA_INDX_SOFTCONE_SHIFT (0x0001u)
#define CSL_USB_CONFIGDATA_INDX_SOFTCONE_RESETVAL (0x0000u)
/*----SOFTCONE Tokens----*/
#define CSL_USB_CONFIGDATA_INDX_SOFTCONE_NOSFTCON (0x0000u)
#define CSL_USB_CONFIGDATA_INDX_SOFTCONE_SFTCON (0x0001u)

#define CSL_USB_CONFIGDATA_INDX_UTMIDATAWIDTH_MASK (0x0001u)
#define CSL_USB_CONFIGDATA_INDX_UTMIDATAWIDTH_SHIFT (0x0000u)
#define CSL_USB_CONFIGDATA_INDX_UTMIDATAWIDTH_RESETVAL (0x0000u)
/*----UTMIDATAWIDTH Tokens----*/
#define CSL_USB_CONFIGDATA_INDX_UTMIDATAWIDTH_EIGHT (0x0000u)
#define CSL_USB_CONFIGDATA_INDX_UTMIDATAWIDTH_SIXTEEN (0x0001u)

#define CSL_USB_CONFIGDATA_INDX_RESETVAL (0x0000u)

/* FIFO0R1 */

#define CSL_USB_FIFO0R1_DATA_MASK        (0xFFFFu)
#define CSL_USB_FIFO0R1_DATA_SHIFT       (0x0000u)
#define CSL_USB_FIFO0R1_DATA_RESETVAL    (0x0000u)

#define CSL_USB_FIFO0R1_RESETVAL         (0x0000u)

/* FIFO0R2 */

#define CSL_USB_FIFO0R2_DATA_MASK        (0xFFFFu)
#define CSL_USB_FIFO0R2_DATA_SHIFT       (0x0000u)
#define CSL_USB_FIFO0R2_DATA_RESETVAL    (0x0000u)

#define CSL_USB_FIFO0R2_RESETVAL         (0x0000u)

/* FIFO1R1 */

#define CSL_USB_FIFO1R1_DATA_MASK        (0xFFFFu)
#define CSL_USB_FIFO1R1_DATA_SHIFT       (0x0000u)
#define CSL_USB_FIFO1R1_DATA_RESETVAL    (0x0000u)

#define CSL_USB_FIFO1R1_RESETVAL         (0x0000u)

/* FIFO1R2 */

#define CSL_USB_FIFO1R2_DATA_MASK        (0xFFFFu)
#define CSL_USB_FIFO1R2_DATA_SHIFT       (0x0000u)
#define CSL_USB_FIFO1R2_DATA_RESETVAL    (0x0000u)

#define CSL_USB_FIFO1R2_RESETVAL         (0x0000u)

/* FIFO2R1 */

#define CSL_USB_FIFO2R1_DATA_MASK        (0xFFFFu)
#define CSL_USB_FIFO2R1_DATA_SHIFT       (0x0000u)
#define CSL_USB_FIFO2R1_DATA_RESETVAL    (0x0000u)

#define CSL_USB_FIFO2R1_RESETVAL         (0x0000u)

/* FIFO2R2 */

#define CSL_USB_FIFO2R2_DATA_MASK        (0xFFFFu)
#define CSL_USB_FIFO2R2_DATA_SHIFT       (0x0000u)
#define CSL_USB_FIFO2R2_DATA_RESETVAL    (0x0000u)

#define CSL_USB_FIFO2R2_RESETVAL         (0x0000u)

/* FIFO3R1 */

#define CSL_USB_FIFO3R1_DATA_MASK        (0xFFFFu)
#define CSL_USB_FIFO3R1_DATA_SHIFT       (0x0000u)
#define CSL_USB_FIFO3R1_DATA_RESETVAL    (0x0000u)

#define CSL_USB_FIFO3R1_RESETVAL         (0x0000u)

/* FIFO3R2 */

#define CSL_USB_FIFO3R2_DATA_MASK        (0xFFFFu)
#define CSL_USB_FIFO3R2_DATA_SHIFT       (0x0000u)
#define CSL_USB_FIFO3R2_DATA_RESETVAL    (0x0000u)

#define CSL_USB_FIFO3R2_RESETVAL         (0x0000u)

/* FIFO4R1 */

#define CSL_USB_FIFO4R1_DATA_MASK        (0xFFFFu)
#define CSL_USB_FIFO4R1_DATA_SHIFT       (0x0000u)
#define CSL_USB_FIFO4R1_DATA_RESETVAL    (0x0000u)

#define CSL_USB_FIFO4R1_RESETVAL         (0x0000u)

/* FIFO4R2 */

#define CSL_USB_FIFO4R2_DATA_MASK        (0xFFFFu)
#define CSL_USB_FIFO4R2_DATA_SHIFT       (0x0000u)
#define CSL_USB_FIFO4R2_DATA_RESETVAL    (0x0000u)

#define CSL_USB_FIFO4R2_RESETVAL         (0x0000u)

/* DEVCTL */


#define CSL_USB_DEVCTL_BDEVICE_MASK      (0x0080u)
#define CSL_USB_DEVCTL_BDEVICE_SHIFT     (0x0007u)
#define CSL_USB_DEVCTL_BDEVICE_RESETVAL  (0x0001u)
/*----BDEVICE Tokens----*/
#define CSL_USB_DEVCTL_BDEVICE_ADEVICE   (0x0000u)
#define CSL_USB_DEVCTL_BDEVICE_BDEVICE   (0x0001u)

#define CSL_USB_DEVCTL_FSDEV_MASK        (0x0040u)
#define CSL_USB_DEVCTL_FSDEV_SHIFT       (0x0006u)
#define CSL_USB_DEVCTL_FSDEV_RESETVAL    (0x0000u)

#define CSL_USB_DEVCTL_LSDEV_MASK        (0x0020u)
#define CSL_USB_DEVCTL_LSDEV_SHIFT       (0x0005u)
#define CSL_USB_DEVCTL_LSDEV_RESETVAL    (0x0000u)

#define CSL_USB_DEVCTL_VBUS_MASK         (0x0018u)
#define CSL_USB_DEVCTL_VBUS_SHIFT        (0x0003u)
#define CSL_USB_DEVCTL_VBUS_RESETVAL     (0x0000u)
/*----VBUS Tokens----*/
#define CSL_USB_DEVCTL_VBUS_BELOW        (0x0000u)
#define CSL_USB_DEVCTL_VBUS_BELOWAVALID  (0x0001u)
#define CSL_USB_DEVCTL_VBUS_BELOWVBUSVALID (0x0002u)
#define CSL_USB_DEVCTL_VBUS_ABOVEVBUSVALID (0x0003u)

#define CSL_USB_DEVCTL_HOSTMODE_MASK     (0x0004u)
#define CSL_USB_DEVCTL_HOSTMODE_SHIFT    (0x0002u)
#define CSL_USB_DEVCTL_HOSTMODE_RESETVAL (0x0000u)

#define CSL_USB_DEVCTL_HOSTREQ_MASK      (0x0002u)
#define CSL_USB_DEVCTL_HOSTREQ_SHIFT     (0x0001u)
#define CSL_USB_DEVCTL_HOSTREQ_RESETVAL  (0x0000u)

#define CSL_USB_DEVCTL_SESSION_MASK      (0x0001u)
#define CSL_USB_DEVCTL_SESSION_SHIFT     (0x0000u)
#define CSL_USB_DEVCTL_SESSION_RESETVAL  (0x0000u)

#define CSL_USB_DEVCTL_RESETVAL          (0x0080u)

/* TXFIFOSZ_RXFIFOSZ */


#define CSL_USB_TXFIFOSZ_RXFIFOSZ_RXDPB_MASK (0x1000u)
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_RXDPB_SHIFT (0x000Cu)
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_RXDPB_RESETVAL (0x0000u)
/*----RXDPB Tokens----*/
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_RXDPB_SINGLE (0x0000u)
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_RXDPB_DOUBLE (0x0001u)

#define CSL_USB_TXFIFOSZ_RXFIFOSZ_RCVSZ_MASK (0x0F00u)
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_RCVSZ_SHIFT (0x0008u)
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_RCVSZ_RESETVAL (0x0000u)


#define CSL_USB_TXFIFOSZ_RXFIFOSZ_TXDPB_MASK (0x0010u)
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_TXDPB_SHIFT (0x0004u)
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_TXDPB_RESETVAL (0x0000u)
/*----TXDPB Tokens----*/
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_TXDPB_SINGLE (0x0000u)
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_TXDPB_DOUBLE (0x0001u)

#define CSL_USB_TXFIFOSZ_RXFIFOSZ_TXSZ_MASK (0x000Fu)
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_TXSZ_SHIFT (0x0000u)
#define CSL_USB_TXFIFOSZ_RXFIFOSZ_TXSZ_RESETVAL (0x0000u)

#define CSL_USB_TXFIFOSZ_RXFIFOSZ_RESETVAL (0x0000u)

/* TXFIFOADDR */


#define CSL_USB_TXFIFOADDR_ADDR_MASK     (0x1FFFu)
#define CSL_USB_TXFIFOADDR_ADDR_SHIFT    (0x0000u)
#define CSL_USB_TXFIFOADDR_ADDR_RESETVAL (0x0000u)

#define CSL_USB_TXFIFOADDR_RESETVAL      (0x0000u)

/* RXFIFOADDR */


#define CSL_USB_RXFIFOADDR_ADDR_MASK     (0x1FFFu)
#define CSL_USB_RXFIFOADDR_ADDR_SHIFT    (0x0000u)
#define CSL_USB_RXFIFOADDR_ADDR_RESETVAL (0x0000u)

#define CSL_USB_RXFIFOADDR_RESETVAL      (0x0000u)

/* HWVERS */


#define CSL_USB_HWVERS_REVMAJ_MASK       (0x7C00u)
#define CSL_USB_HWVERS_REVMAJ_SHIFT      (0x000Au)
#define CSL_USB_HWVERS_REVMAJ_RESETVAL   (0x0000u)

#define CSL_USB_HWVERS_REVMIN_MASK       (0x03FFu)
#define CSL_USB_HWVERS_REVMIN_SHIFT      (0x0000u)
#define CSL_USB_HWVERS_REVMIN_RESETVAL   (0x0000u)

#define CSL_USB_HWVERS_RESETVAL          (0x0000u)

/* PERI_CSR0 */


#define CSL_USB_PERI_CSR0_FLUSHFIFO_MASK (0x0100u)
#define CSL_USB_PERI_CSR0_FLUSHFIFO_SHIFT (0x0008u)
#define CSL_USB_PERI_CSR0_FLUSHFIFO_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_SERV_SETUPEND_MASK (0x0080u)
#define CSL_USB_PERI_CSR0_SERV_SETUPEND_SHIFT (0x0007u)
#define CSL_USB_PERI_CSR0_SERV_SETUPEND_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_SERV_RXPKTRDY_MASK (0x0040u)
#define CSL_USB_PERI_CSR0_SERV_RXPKTRDY_SHIFT (0x0006u)
#define CSL_USB_PERI_CSR0_SERV_RXPKTRDY_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_SENDSTALL_MASK (0x0020u)
#define CSL_USB_PERI_CSR0_SENDSTALL_SHIFT (0x0005u)
#define CSL_USB_PERI_CSR0_SENDSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_SETUPEND_MASK  (0x0010u)
#define CSL_USB_PERI_CSR0_SETUPEND_SHIFT (0x0004u)
#define CSL_USB_PERI_CSR0_SETUPEND_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_DATAEND_MASK   (0x0008u)
#define CSL_USB_PERI_CSR0_DATAEND_SHIFT  (0x0003u)
#define CSL_USB_PERI_CSR0_DATAEND_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_SENTSTALL_MASK (0x0004u)
#define CSL_USB_PERI_CSR0_SENTSTALL_SHIFT (0x0002u)
#define CSL_USB_PERI_CSR0_SENTSTALL_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_TXPKTRDY_MASK  (0x0002u)
#define CSL_USB_PERI_CSR0_TXPKTRDY_SHIFT (0x0001u)
#define CSL_USB_PERI_CSR0_TXPKTRDY_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_RXPKTRDY_MASK  (0x0001u)
#define CSL_USB_PERI_CSR0_RXPKTRDY_SHIFT (0x0000u)
#define CSL_USB_PERI_CSR0_RXPKTRDY_RESETVAL (0x0000u)

#define CSL_USB_PERI_CSR0_RESETVAL       (0x0000u)

/* COUNT0 */


#define CSL_USB_COUNT0_EP0RXCOUNT_MASK   (0x007Fu)
#define CSL_USB_COUNT0_EP0RXCOUNT_SHIFT  (0x0000u)
#define CSL_USB_COUNT0_EP0RXCOUNT_RESETVAL (0x0000u)

#define CSL_USB_COUNT0_RESETVAL          (0x0000u)

/* CONFIGDATA */



#define CSL_USB_CONFIGDATA_MPTXE_MASK    (0x0040u)
#define CSL_USB_CONFIGDATA_MPTXE_SHIFT   (0x0006u)
#define CSL_USB_CONFIGDATA_MPTXE_RESETVAL (0x0000u)

#define CSL_USB_CONFIGDATA_BIGENDIAN_MASK (0x0020u)
#define CSL_USB_CONFIGDATA_BIGENDIAN_SHIFT (0x0005u)
#define CSL_USB_CONFIGDATA_BIGENDIAN_RESETVAL (0x0000u)

#define CSL_USB_CONFIGDATA_HBRXE_MASK    (0x0010u)
#define CSL_USB_CONFIGDATA_HBRXE_SHIFT   (0x0004u)
#define CSL_USB_CONFIGDATA_HBRXE_RESETVAL (0x0000u)

#define CSL_USB_CONFIGDATA_HBTXE_MASK    (0x0008u)
#define CSL_USB_CONFIGDATA_HBTXE_SHIFT   (0x0003u)
#define CSL_USB_CONFIGDATA_HBTXE_RESETVAL (0x0000u)

#define CSL_USB_CONFIGDATA_DYNFIFO_MASK  (0x0004u)
#define CSL_USB_CONFIGDATA_DYNFIFO_SHIFT (0x0002u)
#define CSL_USB_CONFIGDATA_DYNFIFO_RESETVAL (0x0000u)

#define CSL_USB_CONFIGDATA_SOFTCONE_MASK (0x0002u)
#define CSL_USB_CONFIGDATA_SOFTCONE_SHIFT (0x0001u)
#define CSL_USB_CONFIGDATA_SOFTCONE_RESETVAL (0x0000u)

#define CSL_USB_CONFIGDATA_UTMIDATAWIDTH_MASK (0x0001u)
#define CSL_USB_CONFIGDATA_UTMIDATAWIDTH_SHIFT (0x0000u)
#define CSL_USB_CONFIGDATA_UTMIDATAWIDTH_RESETVAL (0x0000u)

#define CSL_USB_CONFIGDATA_RESETVAL      (0x0000u)

/* DMAREVID1 */

#define CSL_USB_DMAREVID1_REV_MASK       (0xFFFFu)
#define CSL_USB_DMAREVID1_REV_SHIFT      (0x0000u)
#define CSL_USB_DMAREVID1_REV_RESETVAL   (0x1900u)

#define CSL_USB_DMAREVID1_RESETVAL       (0x1900u)

/* DMAREVID2 */

#define CSL_USB_DMAREVID2_REV_MASK       (0xFFFFu)
#define CSL_USB_DMAREVID2_REV_SHIFT      (0x0000u)
#define CSL_USB_DMAREVID2_REV_RESETVAL   (0x0053u)

#define CSL_USB_DMAREVID2_RESETVAL       (0x0053u)

/* TDFDQ */


#define CSL_USB_TDFDQ_TD_DESC_QMGR_MASK  (0x3000u)
#define CSL_USB_TDFDQ_TD_DESC_QMGR_SHIFT (0x000Cu)
#define CSL_USB_TDFDQ_TD_DESC_QMGR_RESETVAL (0x0000u)

#define CSL_USB_TDFDQ_TD_DESC_QNUM_MASK  (0x0FFFu)
#define CSL_USB_TDFDQ_TD_DESC_QNUM_SHIFT (0x0000u)
#define CSL_USB_TDFDQ_TD_DESC_QNUM_RESETVAL (0x0000u)

#define CSL_USB_TDFDQ_RESETVAL           (0x0000u)

/* DMAEMU */


#define CSL_USB_DMAEMU_SOFT_MASK         (0x0002u)
#define CSL_USB_DMAEMU_SOFT_SHIFT        (0x0001u)
#define CSL_USB_DMAEMU_SOFT_RESETVAL     (0x0000u)

#define CSL_USB_DMAEMU_FREE_MASK         (0x0001u)
#define CSL_USB_DMAEMU_FREE_SHIFT        (0x0000u)
#define CSL_USB_DMAEMU_FREE_RESETVAL     (0x0000u)

#define CSL_USB_DMAEMU_RESETVAL          (0x0000u)

/* DMA_SCHED_CTRL1 */


#define CSL_USB_DMA_SCHED_CTRL1_LAST_ENTRY_MASK (0x00FFu)
#define CSL_USB_DMA_SCHED_CTRL1_LAST_ENTRY_SHIFT (0x0000u)
#define CSL_USB_DMA_SCHED_CTRL1_LAST_ENTRY_RESETVAL (0x0000u)

#define CSL_USB_DMA_SCHED_CTRL1_RESETVAL (0x0000u)

/* DMA_SCHED_CTRL2 */

#define CSL_USB_DMA_SCHED_CTRL2_ENABLE_MASK (0x8000u)
#define CSL_USB_DMA_SCHED_CTRL2_ENABLE_SHIFT (0x000Fu)
#define CSL_USB_DMA_SCHED_CTRL2_ENABLE_RESETVAL (0x0000u)
/*----ENABLE Tokens----*/
#define CSL_USB_DMA_SCHED_CTRL2_ENABLE_ENABLED (0x0000u)
#define CSL_USB_DMA_SCHED_CTRL2_ENABLE_DISABLED (0x0001u)


#define CSL_USB_DMA_SCHED_CTRL2_RESETVAL (0x0000u)

/* QMGRREVID1 */

#define CSL_USB_QMGRREVID1_REV_MASK      (0xFFFFu)
#define CSL_USB_QMGRREVID1_REV_SHIFT     (0x0000u)
#define CSL_USB_QMGRREVID1_REV_RESETVAL  (0x1200u)

#define CSL_USB_QMGRREVID1_RESETVAL      (0x1200u)

/* QMGRREVID2 */

#define CSL_USB_QMGRREVID2_REV_MASK      (0xFFFFu)
#define CSL_USB_QMGRREVID2_REV_SHIFT     (0x0000u)
#define CSL_USB_QMGRREVID2_REV_RESETVAL  (0x0052u)

#define CSL_USB_QMGRREVID2_RESETVAL      (0x0052u)

/* DIVERSION1 */


#define CSL_USB_DIVERSION1_SOURCE_QNUM_MASK (0x3FFFu)
#define CSL_USB_DIVERSION1_SOURCE_QNUM_SHIFT (0x0000u)
#define CSL_USB_DIVERSION1_SOURCE_QNUM_RESETVAL (0x0000u)

#define CSL_USB_DIVERSION1_RESETVAL      (0x0000u)

/* DIVERSION2 */

#define CSL_USB_DIVERSION2_HEAD_TAIL_MASK (0x8000u)
#define CSL_USB_DIVERSION2_HEAD_TAIL_SHIFT (0x000Fu)
#define CSL_USB_DIVERSION2_HEAD_TAIL_RESETVAL (0x0000u)
/*----HEAD_TAIL Tokens----*/
#define CSL_USB_DIVERSION2_HEAD_TAIL_HEAD (0x0000u)
#define CSL_USB_DIVERSION2_HEAD_TAIL_TAIL (0x0001u)


#define CSL_USB_DIVERSION2_DEST_QNUM_MASK (0x003Fu)
#define CSL_USB_DIVERSION2_DEST_QNUM_SHIFT (0x0000u)
#define CSL_USB_DIVERSION2_DEST_QNUM_RESETVAL (0x0000u)

#define CSL_USB_DIVERSION2_RESETVAL      (0x0000u)

/* FDBSC0 */

#define CSL_USB_FDBSC0_FDBQ1_STARVE_CNT_MASK (0xFF00u)
#define CSL_USB_FDBSC0_FDBQ1_STARVE_CNT_SHIFT (0x0008u)
#define CSL_USB_FDBSC0_FDBQ1_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC0_FDBQ0_STARVE_CNT_MASK (0x00FFu)
#define CSL_USB_FDBSC0_FDBQ0_STARVE_CNT_SHIFT (0x0000u)
#define CSL_USB_FDBSC0_FDBQ0_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC0_RESETVAL          (0x0000u)

/* FDBSC1 */

#define CSL_USB_FDBSC1_FDBQ3_STARVE_CNT_MASK (0xFF00u)
#define CSL_USB_FDBSC1_FDBQ3_STARVE_CNT_SHIFT (0x0008u)
#define CSL_USB_FDBSC1_FDBQ3_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC1_FDBQ2_STARVE_CNT_MASK (0x00FFu)
#define CSL_USB_FDBSC1_FDBQ2_STARVE_CNT_SHIFT (0x0000u)
#define CSL_USB_FDBSC1_FDBQ2_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC1_RESETVAL          (0x0000u)

/* FDBSC2 */

#define CSL_USB_FDBSC2_FDBQ5_STARVE_CNT_MASK (0xFF00u)
#define CSL_USB_FDBSC2_FDBQ5_STARVE_CNT_SHIFT (0x0008u)
#define CSL_USB_FDBSC2_FDBQ5_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC2_FDBQ4_STARVE_CNT_MASK (0x00FFu)
#define CSL_USB_FDBSC2_FDBQ4_STARVE_CNT_SHIFT (0x0000u)
#define CSL_USB_FDBSC2_FDBQ4_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC2_RESETVAL          (0x0000u)

/* FDBSC3 */

#define CSL_USB_FDBSC3_FDBQ7_STARVE_CNT_MASK (0xFF00u)
#define CSL_USB_FDBSC3_FDBQ7_STARVE_CNT_SHIFT (0x0008u)
#define CSL_USB_FDBSC3_FDBQ7_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC3_FDBQ6_STARVE_CNT_MASK (0x00FFu)
#define CSL_USB_FDBSC3_FDBQ6_STARVE_CNT_SHIFT (0x0000u)
#define CSL_USB_FDBSC3_FDBQ6_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC3_RESETVAL          (0x0000u)

/* FDBSC4 */

#define CSL_USB_FDBSC4_FDBQ9_STARVE_CNT_MASK (0xFF00u)
#define CSL_USB_FDBSC4_FDBQ9_STARVE_CNT_SHIFT (0x0008u)
#define CSL_USB_FDBSC4_FDBQ9_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC4_FDBQ8_STARVE_CNT_MASK (0x00FFu)
#define CSL_USB_FDBSC4_FDBQ8_STARVE_CNT_SHIFT (0x0000u)
#define CSL_USB_FDBSC4_FDBQ8_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC4_RESETVAL          (0x0000u)

/* FDBSC5 */

#define CSL_USB_FDBSC5_FDBQ11_STARVE_CNT_MASK (0xFF00u)
#define CSL_USB_FDBSC5_FDBQ11_STARVE_CNT_SHIFT (0x0008u)
#define CSL_USB_FDBSC5_FDBQ11_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC5_FDBQ10_STARVE_CNT_MASK (0x00FFu)
#define CSL_USB_FDBSC5_FDBQ10_STARVE_CNT_SHIFT (0x0000u)
#define CSL_USB_FDBSC5_FDBQ10_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC5_RESETVAL          (0x0000u)

/* FDBSC6 */

#define CSL_USB_FDBSC6_FDBQ13_STARVE_CNT_MASK (0xFF00u)
#define CSL_USB_FDBSC6_FDBQ13_STARVE_CNT_SHIFT (0x0008u)
#define CSL_USB_FDBSC6_FDBQ13_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC6_FDBQ12_STARVE_CNT_MASK (0x00FFu)
#define CSL_USB_FDBSC6_FDBQ12_STARVE_CNT_SHIFT (0x0000u)
#define CSL_USB_FDBSC6_FDBQ12_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC6_RESETVAL          (0x0000u)

/* FDBSC7 */

#define CSL_USB_FDBSC7_FDBQ15_STARVE_CNT_MASK (0xFF00u)
#define CSL_USB_FDBSC7_FDBQ15_STARVE_CNT_SHIFT (0x0008u)
#define CSL_USB_FDBSC7_FDBQ15_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC7_FDBQ14_STARVE_CNT_MASK (0x00FFu)
#define CSL_USB_FDBSC7_FDBQ14_STARVE_CNT_SHIFT (0x0000u)
#define CSL_USB_FDBSC7_FDBQ14_STARVE_CNT_RESETVAL (0x0000u)

#define CSL_USB_FDBSC7_RESETVAL          (0x0000u)

/* LRAM0BASE1 */

#define CSL_USB_LRAM0BASE1_REGION0_BASE_MASK (0xFFFFu)
#define CSL_USB_LRAM0BASE1_REGION0_BASE_SHIFT (0x0000u)
#define CSL_USB_LRAM0BASE1_REGION0_BASE_RESETVAL (0x0000u)

#define CSL_USB_LRAM0BASE1_RESETVAL      (0x0000u)

/* LRAM0BASE2 */

#define CSL_USB_LRAM0BASE2_REGION0_BASE_MASK (0xFFFFu)
#define CSL_USB_LRAM0BASE2_REGION0_BASE_SHIFT (0x0000u)
#define CSL_USB_LRAM0BASE2_REGION0_BASE_RESETVAL (0x0000u)

#define CSL_USB_LRAM0BASE2_RESETVAL      (0x0000u)

/* LRAM0SIZE */


#define CSL_USB_LRAM0SIZE_REGION0_SIZE_MASK (0x3FFFu)
#define CSL_USB_LRAM0SIZE_REGION0_SIZE_SHIFT (0x0000u)
#define CSL_USB_LRAM0SIZE_REGION0_SIZE_RESETVAL (0x0000u)

#define CSL_USB_LRAM0SIZE_RESETVAL       (0x0000u)

/* LRAM1BASE1 */

#define CSL_USB_LRAM1BASE1_REGION1_BASE_MASK (0xFFFFu)
#define CSL_USB_LRAM1BASE1_REGION1_BASE_SHIFT (0x0000u)
#define CSL_USB_LRAM1BASE1_REGION1_BASE_RESETVAL (0x0000u)

#define CSL_USB_LRAM1BASE1_RESETVAL      (0x0000u)

/* LRAM1BASE2 */

#define CSL_USB_LRAM1BASE2_REGION1_BASE_MASK (0xFFFFu)
#define CSL_USB_LRAM1BASE2_REGION1_BASE_SHIFT (0x0000u)
#define CSL_USB_LRAM1BASE2_REGION1_BASE_RESETVAL (0x0000u)

#define CSL_USB_LRAM1BASE2_RESETVAL      (0x0000u)

/* PEND0 */

#define CSL_USB_PEND0_QPEND_MASK         (0xFFFFu)
#define CSL_USB_PEND0_QPEND_SHIFT        (0x0000u)
#define CSL_USB_PEND0_QPEND_RESETVAL     (0x0000u)

#define CSL_USB_PEND0_RESETVAL           (0x0000u)

/* PEND1 */

#define CSL_USB_PEND1_QPEND_MASK         (0xFFFFu)
#define CSL_USB_PEND1_QPEND_SHIFT        (0x0000u)
#define CSL_USB_PEND1_QPEND_RESETVAL     (0x0000u)

#define CSL_USB_PEND1_RESETVAL           (0x0000u)

/* PEND2 */

#define CSL_USB_PEND2_QPEND_MASK         (0xFFFFu)
#define CSL_USB_PEND2_QPEND_SHIFT        (0x0000u)
#define CSL_USB_PEND2_QPEND_RESETVAL     (0x0000u)

#define CSL_USB_PEND2_RESETVAL           (0x0000u)

/* PEND3 */

#define CSL_USB_PEND3_QPEND_MASK         (0xFFFFu)
#define CSL_USB_PEND3_QPEND_SHIFT        (0x0000u)
#define CSL_USB_PEND3_QPEND_RESETVAL     (0x0000u)

#define CSL_USB_PEND3_RESETVAL           (0x0000u)

/* PEND4 */

#define CSL_USB_PEND4_QPEND_MASK         (0xFFFFu)
#define CSL_USB_PEND4_QPEND_SHIFT        (0x0000u)
#define CSL_USB_PEND4_QPEND_RESETVAL     (0x0000u)

#define CSL_USB_PEND4_RESETVAL           (0x0000u)

/* PEND5 */

#define CSL_USB_PEND5_QPEND_MASK         (0xFFFFu)
#define CSL_USB_PEND5_QPEND_SHIFT        (0x0000u)
#define CSL_USB_PEND5_QPEND_RESETVAL     (0x0000u)

#define CSL_USB_PEND5_RESETVAL           (0x0000u)

#endif
