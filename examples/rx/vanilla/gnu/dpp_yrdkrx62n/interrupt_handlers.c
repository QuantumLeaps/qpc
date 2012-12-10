/***********************************************************************
* This file has been modified for the DPP application by Quantum Leaps
* from the generated file 'interrupt_handlers.c'.
*
*  1. added definitions of the ISRs used in the DPP application.
*  2. added Q_ERROR() assertions to exception handlers.
*
*  DATE: May 18, 2012
***********************************************************************/
#include "qp_port.h"
#include "bsp.h"
#include "dpp.h"

#include "interrupt_handlers.h"

Q_DEFINE_THIS_FILE

// Exception(Supervisor Instruction)
void INT_Excep_SuperVisorInst(void) { Q_ERROR(); }

// Exception(Undefined Instruction)
void INT_Excep_UndefinedInst(void)  { Q_ERROR(); }

// Exception(Floating Point)
void INT_Excep_FloatingPoint(void)  { Q_ERROR(); }

// NMI
void INT_NonMaskableInterrupt(void) { Q_ERROR(); }

// Dummy
void Dummy(void){/* brk(); */}

// BRK
void INT_Excep_BRK(void){/* wait(); */}
//;0x0000  Reserved

//;0x0004  Reserved

//;0x0008  Reserved

//;0x000C  Reserved

//;0x0010  Reserved

//;0x0014  Reserved

//;0x0018  Reserved

//;0x001C  Reserved

//;0x0020  Reserved

//;0x0024  Reserved

//;0x0028  Reserved

//;0x002C  Reserved

//;0x0030  Reserved

//;0x0034  Reserved

//;0x0038  Reserved

//;0x003C  Reserved

//;0x0040  BUSERR
void  INT_Excep_BUSERR(void){ }
//;0x0044  Reserved

//;0x0048  Reserved

//;0x004C  Reserved

//;0x0050  Reserved

//;0x0054 FCUERR
void  INT_Excep_FCU_FCUERR(void){ }
//;0x0058  Reserved

//;0x005C  FRDYI
void  INT_Excep_FCU_FRDYI(void){ }
//;0x0060  Reserved

//;0x0064  Reserved

//;0x0068  Reserved

//;0x006C  Reserved

void  INT_Excep_ICU_SWINT(void){ }
//;0x0070  CMTU0_CMT0
void  INT_Excep_CMTU0_CMI0(void) {
    QF_ISR_ENTRY();  /* inform the QF Vanilla kernel about entering the ISR */

#ifdef Q_SPY
    QS_tickTime_ += QS_tickPeriod_;       /* account for the clock rollover */
#endif
    QF_TICK(&QS_Excep_CMTU0_CMT0);         /* process all armed time events */

    QF_ISR_EXIT();    /* inform the QF Vanilla kernel about exiting the ISR */
}
//;0x0074  CMTU0_CMT1
void  INT_Excep_CMTU0_CMI1(void){ }
//;0x0078  CMTU1_CMT2
void  INT_Excep_CMTU1_CMI2(void){ }
//;0x007C  CMTU1_CMT3
void  INT_Excep_CMTU1_CMI3(void){ }
//;0x0080  Reserved
void  INT_Excep_ETHER_EINT(void){ }
//;0x0084  Reserved

//;0x0088  Reserved

//;0x008C  Reserved

//;0x0090  Reserved
void  INT_Excep_USB0_D0FIFO0(void){ }
//;0x0094  Reserved
void  INT_Excep_USB0_D1FIFO0(void){ }
//;0x0098  Reserved
void  INT_Excep_USB0_USBI0(void){ }
//;0x009C  Reserved

//;0x00A0  Reserved
void  INT_Excep_USB1_D0FIFO1(void){ }
//;0x00A4  Reserved
void  INT_Excep_USB1_D1FIFO1(void){ }
//;0x00A8  Reserved
void  INT_Excep_USB1_USBI1(void){ }
//;0x00AC  Reserved

//;0x00B0  Reserved
void  INT_Excep_RSPI0_SPEIO(void){ }
//;0x00B4  Reserved
void  INT_Excep_RSPI0_SPRI0(void){ }
//;0x00B8  Reserved
void  INT_Excep_RSPI0_SPTI0(void){ }
//;0x00BC  Reserved
void  INT_Excep_RSPI0_SPII0(void){ }
//;0x00C0  Reserved
void  INT_Excep_RSPI1_SPEI1(void){ }
//;0x00C4  Reserved
void  INT_Excep_RSPI1_SPRI1(void){ }
//;0x00C8  Reserved
void  INT_Excep_RSPI1_SPTI1(void){ }
//;0x00CC  Reserved
void  INT_Excep_RSPI1_SPII1(void){ }
//;0x00D0  Reserved

//;0x00D4  Reserved

//;0x00D8  Reserved

//;0x00DC  Reserved

//;0x00E0  Reserved
void  INT_Excep_CAN0_ERS0(void){ }
//;0x00E4  Reserved
void  INT_Excep_CAN0_RXF0(void){ }
//;0x00E8  Reserved
void  INT_Excep_CAN0_TXF0(void){ }
//;0x00EC  Reserved
void  INT_Excep_CAN0_RXM0(void){ }
//;0x00F0  Reserved
void  INT_Excep_CAN0_TXM0(void){ }
//;0x00F4  Reserved

//;0x00F8  Reserved
void  INT_Excep_RTC_PRD(void){ }
//;0x00FC  Reserved
void  INT_Excep_RTC_CUP(void){ }
//;0x0100  IRQ0
void  INT_Excep_IRQ0(void){ }
//;0x0104 IRQ1
void  INT_Excep_IRQ1(void){ }
//;0x0108 IRQ2
void  INT_Excep_IRQ2(void){ }
//;0x010C IRQ3
void  INT_Excep_IRQ3(void){ }
//;0x0110 IRQ4
void  INT_Excep_IRQ4(void){ }
//;0x0114 IRQ5
void  INT_Excep_IRQ5(void){ }
//;0x0118 IRQ6
void  INT_Excep_IRQ6(void){ }
//;0x011C IRQ7
void  INT_Excep_IRQ7(void){ }
//;0x0120 IRQ8
void  INT_Excep_IRQ8(void) {
    QF_ISR_ENTRY();  /* inform the QF Vanilla kernel about entering the ISR */

    QACTIVE_POST(AO_Table, Q_NEW(QEvt, PAUSE_SIG),
                 &QS_Excep_IRQ8);
    QF_ISR_EXIT();    /* inform the QF Vanilla kernel about exiting the ISR */
}
//;0x0124 IRQ9
void  INT_Excep_IRQ9(void) {
    QF_ISR_ENTRY();  /* inform the QF Vanilla kernel about entering the ISR */

    QACTIVE_POST(AO_Philo[1], Q_NEW(QEvt, MAX_PUB_SIG), /* for testing... */
                 &QS_Excep_IRQ9);
    QF_ISR_EXIT();    /* inform the QF Vanilla kernel about exiting the ISR */
}
//;0x0128 IRQ10
void  INT_Excep_IRQ10(void) {
    QF_ISR_ENTRY();  /* inform the QF Vanilla kernel about entering the ISR */

    QACTIVE_POST(AO_Table, Q_NEW(QEvt, MAX_PUB_SIG),    /* for testing... */
                 &QS_Excep_IRQ10);
    QF_ISR_EXIT();    /* inform the QF Vanilla kernel about exiting the ISR */
}
//;0x012C IRQ11
void  INT_Excep_IRQ11(void){ }
//;0x0130 IRQ12
void  INT_Excep_IRQ12(void){ }
//;0x0134 IRQ13
void  INT_Excep_IRQ13(void){ }
//;0x0138 IRQ14
void  INT_Excep_IRQ14(void){ }
//;0x013C IRQ15
void  INT_Excep_IRQ15(void){ }
//;0x0140  Reserved

//;0x0144  Reserved

//;0x0148  Reserved

//;0x014C  Reserved

//;0x0150  Reserved

//;0x0154  Reserved

//;0x0158  Reserved

//;0x015C  Reserved

//;0x0160  Reserved

//;0x0164  Reserved

//;0x0168  Reserved
void  INT_Excep_USBR_USBR0(void){ }
//;0x016C  Reserved
void  INT_Excep_USBR_USBR1(void){ }
//;0x0170  Reserved
void  INT_Excep_RTC_ALM(void){ }
//;0x0174  Reserved

//;0x0178  Reserved

//;0x017C  Reserved

//;0x0180  WDT_WOVI
void  INT_Excep_WDT_WOVI(void){ }
//;0x0184  Reserved

//;0x0188  AD0_ADI0
void  INT_Excep_AD0_ADI0(void){ }
//;0x018C  AD1_ADI1
void  INT_Excep_AD1_ADI1(void){ }
//;0x0190  AD2_ADI2

//;0x0194  AD3_ADI3

//;0x0198  Reserved
void  INT_Excep_S12AD_ADI12_0(void){ }
//;0x019C  Reserved

//;0x01A0  TPU0_TGI0A

//;0x01A4  TPU0_TGI0B

//;0x01A8  TPU0_TGI0C

//;0x01AC  TPU0_TGI0D

//;0x01B0  TPU0_TCI0V

//;0x01B4  Reserved

//;0x01B8  Reserved

//;0x01BC  TPU1_TGI1A

//;0x01C0  TPU1_TGI1B

//;0x01C4  Reserved

//;0x01C8  Reserved
void  INT_Excep_MTU0_TGIA0(void){ }
//;0x01CC  TPU1_TCI1V
void  INT_Excep_MTU0_TGIB0(void){ }
//;0x01D0  TPU1_TCI1U
void  INT_Excep_MTU0_TGICO(void){ }
//;0x01D4  TPU2_TGI2A
void  INT_Excep_MTU0_TGID0(void){ }
//;0x01D8  TPU2_TGI2B
void  INT_Excep_MTU0_TCIV0(void){ }
//;0x01DC  Reserved
void  INT_Excep_MTU0_TGIE0(void){ }
//;0x01E0  TPU2_TCI2V
void  INT_Excep_MTU0_TGIF0(void){ }
//;0x01E4  TPU2_TCI2U
void  INT_Excep_MTU1_TGIA1(void){ }
//;0x01E8  TPU3_TGI3A
void  INT_Excep_MTU1_TGIB1(void){ }
//;0x01EC  TPU3_TGI3B
void  INT_Excep_MTU1_TCIV1(void){ }
//;0x01F0  TPU3_TGI3C
void  INT_Excep_MTU1_TCIU1(void){ }
//;0x01F4  TPU3_TGI3D
void  INT_Excep_MTU2_TGIA2(void){ }
//;0x01F8  TPU3_TCI3V
void  INT_Excep_MTU2_TGIB2(void){ }
//;0x01FC  TPU4_TGI4A
void  INT_Excep_MTU2_TCIV2(void){ }
//;0x0200  TPU4_TGI4B
void  INT_Excep_MTU2_TCIU2(void){ }
//;0x0204  Reserved
void  INT_Excep_MTU3_TGIA3(void){ }
//;0x0208  Reserved
void  INT_Excep_MTU3_TGIB3(void){ }
//;0x020C TPU4_TCI4V
void  INT_Excep_MTU3_TGIC3(void){ }
//;0x0210 TPU4_TCI4U
void  INT_Excep_MTU3_TGID3(void){ }
//;0x0214  TPU5_TGI5A
void  INT_Excep_MTU3_TCIV3(void){ }
//;0x0218  TPU5_TGI5B
void  INT_Excep_MTU4_TGIA4(void){ }
//;0x021C  Reserved
void  INT_Excep_MTU4_TGIB4(void){ }
//;0x0220  TPU5_TCI5V
void  INT_Excep_MTU4_TGIC4(void){ }
//;0x0224  TPU5_TCI5U
void  INT_Excep_MTU4_TGID4(void){ }
//;0x0228  TPU6_TGI6A
void  INT_Excep_MTU4_TCIV4(void){ }
//;0x022C  TPU6_TGI6B
void  INT_Excep_MTU5_TGIU5(void){ }
//;0x0230  TPU6_TGI6C
void  INT_Excep_MTU5_TGIV5(void){ }
//;0x0234  TPU6_TGI6D
void  INT_Excep_MTU5_TGIW5(void){ }
//;0x0238  TPU6_TCI6V
void  INT_Excep_MTU6_TGIA6(void){ }
//;0x023C  Reserved
void  INT_Excep_MTU6_TGIB6(void){ }
//;0x0240  Reserved
void  INT_Excep_MTU6_TGIC6(void){ }
//;0x0244  TPU7_TGI7A
void  INT_Excep_MTU6_TGID6(void){ }
//;0x0248  TPU7_TGI7B
void  INT_Excep_MTU6_TCIV6(void){ }
//;0x024C  Reserved
void  INT_Excep_MTU6_TGIE6(void){ }
//;0x0250  Reserved
void  INT_Excep_MTU6_TGIF6(void){ }
//;0x0254  TPU7_TCI7V
void  INT_Excep_MTU7_TGIA7(void){ }
//;0x0258  TPU7_TCI7U
void  INT_Excep_MTU7_TGIB7(void){ }
//;0x025C  TPU8_TGI8A
void  INT_Excep_MTU7_TCIV7(void){ }
//;0x0260  TPU8_TGI8B
void  INT_Excep_MTU7_TCIU7(void){ }
//;0x0264  Reserved
void  INT_Excep_MTU8_TGIA8(void){ }
//;0x0268  TPU8_TCI8V
void  INT_Excep_MTU8_TGIB8(void){ }
//;0x026C  TPU8_TCI8U
void  INT_Excep_MTU8_TCIV8(void){ }
//;0x0270  TPU9_TGI9A
void  INT_Excep_MTU8_TCIU8(void){ }
//;0x0274  TPU9_TGI9B
void  INT_Excep_MTU9_TGIA9(void){ }
//;0x0278  TPU9_TGI9C
void  INT_Excep_MTU9_TGIB9(void){ }
//;0x027C  TPU9_TGI9D
void  INT_Excep_MTU9_TGIC9(void){ }
//;0x0280  TPU9_TCI9V
void  INT_Excep_MTU9_TGID9(void){ }
//;0x0284  TPU10_TGI10A
void  INT_Excep_MTU9_TCIV9(void){ }
//;0x0288  TPU10_TGI10B
void  INT_Excep_MTU10_TGIA10(void){ }
//;0x028C  Reserved
void  INT_Excep_MTU10_TGIB10(void){ }
//;0x0290  Reserved
void  INT_Excep_MTU10_TGIC10(void){ }
//;0x0294  TPU10_TCI10V
void  INT_Excep_MTU10_TGID10(void){ }
//;0x0298  TPU10_TCI10U
void  INT_Excep_MTU10_TCIV10(void){ }
//;0x029C  TPU11_TGI11A
void  INT_Excep_MTU11_TGIU11(void){ }
//;0x02A0  TPU11_TGI11B
void  INT_Excep_MTU11_TGIV11(void){ }
//;0x02A4  Reserved
void  INT_Excep_MTU11_TGIW11(void){ }
//;0x02A8  TPU11_TCI11V
void  INT_Excep_POE_OEI1(void){ }
//;0x02AC  TPU11_TCI11U
void  INT_Excep_POE_OEI2(void){ }
//;0x02B0  Reserved
void  INT_Excep_POE_OEI3(void){ }
//;0x02B4  Reserved
void  INT_Excep_POE_OEI4(void){ }
//;0x02B8  TMR0_CMI0A
void  INT_Excep_TMR0_CMIA0(void){ }
//;0x02BC  TMR0_CMI0B
void  INT_Excep_TMR0_CMIB0(void){ }
//;0x02C0  TMR0_OV0I
void  INT_Excep_TMR0_OVI0(void){ }
//;0x02C4  TMR1_CMI1A
void  INT_Excep_TMR1_CMIA1(void){ }
//;0x02C8  TMR1_CMI1B
void  INT_Excep_TMR1_CMIB1(void){ }
//;0x02CC  TMR1_OV1I
void  INT_Excep_TMR1_OVI1(void){ }
//;0x02D0 TMR2_CMI2A
void  INT_Excep_TMR2_CMIA2(void){ }
//;0x02D4  TMR2_CMI2B
void  INT_Excep_TMR2_CMIB2(void){ }
//;0x02D8  TMR2_OV2I
void  INT_Excep_TMR2_OVI2(void){ }
//;0x02DC  TMR3_CMI3A
void  INT_Excep_TMR3_CMIA3(void){ }
//;0x02E0 TMR3_CMI3B
void  INT_Excep_TMR3_CMIB3(void){ }
//;0x02E4  TMR3_OV3I
void  INT_Excep_TMR3_OVI3(void){ }
//;0x02E8  Reserved

//;0x02EC  Reserved

//;0x02F0  Reserved

//;0x02F4  Reserved

//;0x02F8  Reserved

//;0x02FC  Reserved

//;0x0300  Reserved

//;0x0304  Reserved

//;0x0308  Reserved

//;0x030C  Reserved

//;0x0310  Reserved

//;0x0314  Reserved

//;0x0318  DMAC_DMTEND0
void  INT_Excep_DMACA_DMAC0I(void){ }
//;0x031C  DMAC_DMTEND1
void  INT_Excep_DMACA_DMAC1I(void){ }
//;0x0320  DMAC_DMTEND2
void  INT_Excep_DMACA_DMAC2I(void){ }
//;0x0324  DMAC_DMTEND3
void  INT_Excep_DMACA_DMAC3I(void){ }
//;0x0328  Reserved
void  INT_Excep_EXDMAC_EXDMAC_0I(void){ }
//;0x032C  Reserved
void  INT_Excep_EXDMAC_EXDMAC_1I(void){ }
//;0x0330  Reserved

//;0x0334  Reserved

//;0x0338  Reserved

//;0x033C  Reserved

//;0x0340  Reserved

//;0x0344  Reserved

//;0x0348  Reserved

//;0x034C  Reserved

//;0x0350  Reserved

//;0x0354  Reserved

//;0x0358  SCI0_ERI0
void  INT_Excep_SCI0_ERI0(void){ }
//;0x035C  SCI0_RXI0
void  INT_Excep_SCI0_RXI0(void){ }
//;0x0360  SCI0_TXI0
void  INT_Excep_SCI0_TXI0(void){ }
//;0x0364  SCI0_TEI0
void  INT_Excep_SCI0_TEI0(void){ }
//;0x0368  SCI1_ERI1
void  INT_Excep_SCI1_ERI1(void){ }
//;0x036C  SCI1_RXI1
void  INT_Excep_SCI1_RXI1(void){ }
//;0x0370  SCI1_TXI1
void  INT_Excep_SCI1_TXI1(void){ }
//;0x0374  SCI1_TEI1
void  INT_Excep_SCI1_TEI1(void){ }
//;0x0378  SCI2_ERI2
void  INT_Excep_SCI2_ERI2(void){ }
//;0x037C  SCI2_RXI2
void  INT_Excep_SCI2_RXI2(void){ }
//;0x0380  SCI2_TXI2
void  INT_Excep_SCI2_TXI2(void){ }
//;0x0384  SCI2_TEI2
void  INT_Excep_SCI2_TEI2(void){ }
//;0x0388  SCI3_ERI3
void  INT_Excep_SCI3_ERI3(void){ }
//;0x038C  SCI3_RXI3
void  INT_Excep_SCI3_RXI3(void){ }
//;0x0390  SCI3_TXI3
void  INT_Excep_SCI3_TXI3(void){ }
//;0x0394  SCI3_TEI3
void  INT_Excep_SCI3_TEI3(void){ }
//;0x0398  SCI4_ERI4

//;0x039C  SCI4_RXI4

//;0x03A0  SCI4_TXI4

//;0x03A4  SCI4_TEI4

//;0x03A8  SCI5_ERI5
void  INT_Excep_SCI5_ERI5(void){ }
//;0x03AC  SCI5_RXI5
void  INT_Excep_SCI5_RXI5(void){ }
//;0x03B0  SCI5_TXI5
void  INT_Excep_SCI5_TXI5(void){ }
//;0x03B4  SCI5_TEI5
void  INT_Excep_SCI5_TEI5(void){ }
//;0x03B8  SCI6_ERI6
void  INT_Excep_SCI6_ERI6(void){ }
//;0x03BC  SCI6_RXI6
void  INT_Excep_SCI6_RXI6(void){ }
//;0x03C0  SCI6_TXI6
void  INT_Excep_SCI6_TXI6(void){ }
//;0x03C4  SCI6_TEI6
void  INT_Excep_SCI6_TEI6(void){ }
//;0x03C8  Reserved

//;0x03CC  Reserved

//;0x03D0  Reserved

//;0x03D4  Reserved

//;0x03D8  RIIC0_ICEEI0
void  INT_Excep_RIIC0_ICEEI0(void){ }
//;0x03DC  RIIC0_ICRXI0
void  INT_Excep_RIIC0_ICRXI0(void){ }
//;0x03E0  RIIC0_ICTXI0
void  INT_Excep_RIIC0_ICTXI0(void){ }
//;0x03E4  RIIC0_ICTEI0
void  INT_Excep_RIIC0_ICTEI0(void){ }
//;0x03E8  RIIC1_ICEEI1
void  INT_Excep_RIIC1_ICEEI1(void){ }
//;0x03EC  RIIC1_ICRXI1
void  INT_Excep_RIIC1_ICRXI1(void){ }
//;0x03F0  RIIC1_ICTXI1
void  INT_Excep_RIIC1_ICTXI1(void){ }
//;0x03F4  RIIC1_ICTEI1
void  INT_Excep_RIIC1_ICTEI1(void){ }
//;0x03F8  Reserved

//;0x03FC  Reserved




