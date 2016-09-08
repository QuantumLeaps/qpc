/** @file pinmux.h
*   @brief PINMUX Driver Implementation File
*   @date 02-Mar-2016
*   @version 04.05.02
*
*/

/*
* Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


#ifndef __PINMUX_H__
#define __PINMUX_H__

#include "reg_pinmux.h"

#ifdef __cplusplus
extern "C" {
#endif


#define PINMUX_PIN_3_SHIFT                  16U
#define PINMUX_PIN_4_SHIFT                  24U
#define PINMUX_PIN_6_SHIFT                  8U
#define PINMUX_PIN_9_SHIFT                  0U
#define PINMUX_PIN_14_SHIFT                 24U
#define PINMUX_PIN_16_SHIFT                 16U
#define PINMUX_PIN_22_SHIFT                 0U
#define PINMUX_PIN_23_SHIFT                 16U
#define PINMUX_PIN_24_SHIFT                 24U
#define PINMUX_PIN_25_SHIFT                 0U
#define PINMUX_PIN_30_SHIFT                 8U
#define PINMUX_PIN_31_SHIFT                 16U
#define PINMUX_PIN_32_SHIFT                 0U
#define PINMUX_PIN_33_SHIFT                 0U
#define PINMUX_PIN_33_SHIFT                 0U
#define PINMUX_PIN_35_SHIFT                 16U
#define PINMUX_PIN_36_SHIFT                 0U
#define PINMUX_PIN_37_SHIFT                 8U
#define PINMUX_PIN_38_SHIFT                 16U
#define PINMUX_PIN_39_SHIFT                 0U
#define PINMUX_PIN_40_SHIFT                 8U
#define PINMUX_PIN_41_SHIFT                 16U
#define PINMUX_PIN_51_SHIFT                 8U
#define PINMUX_PIN_52_SHIFT                 16U
#define PINMUX_PIN_53_SHIFT                 24U
#define PINMUX_PIN_54_SHIFT                 8U
#define PINMUX_PIN_55_SHIFT                 16U
#define PINMUX_PIN_86_SHIFT                 0U
#define PINMUX_PIN_91_SHIFT                 24U
#define PINMUX_PIN_92_SHIFT                 0U
#define PINMUX_PIN_96_SHIFT                 16U
#define PINMUX_PIN_97_SHIFT                 24U
#define PINMUX_PIN_98_SHIFT                 0U
#define PINMUX_PIN_99_SHIFT                 8U
#define PINMUX_PIN_100_SHIFT                16U
#define PINMUX_PIN_105_SHIFT                24U
#define PINMUX_PIN_106_SHIFT                0U
#define PINMUX_PIN_107_SHIFT                8U
#define PINMUX_PIN_118_SHIFT                0U
#define PINMUX_PIN_124_SHIFT                16U
#define PINMUX_PIN_127_SHIFT                8U
#define PINMUX_PIN_130_SHIFT                16U
#define PINMUX_PIN_139_SHIFT                0U
#define PINMUX_PIN_140_SHIFT                8U
#define PINMUX_PIN_141_SHIFT                16U
#define PINMUX_PIN_133_SHIFT                8U
#define PINMUX_PIN_1_SHIFT                  0U
#define PINMUX_PIN_2_SHIFT                  8U
#define PINMUX_PIN_5_SHIFT                  0U
#define PINMUX_PIN_15_SHIFT                 8U
#define PINMUX_PIN_125_SHIFT                8U
#define PINMUX_PIN_126_SHIFT                24U


#define PINMUX_GATE_EMIF_CLK_SHIFT          8U
#define PINMUX_GIOB_DISABLE_HET2_SHIFT      16U
#define PINMUX_ALT_ADC_TRIGGER_SHIFT        0U
#define PINMUX_ETHERNET_SHIFT               24U
#define PINMUX_ETPWM1_SHIFT                 0U
#define PINMUX_ETPWM2_SHIFT                 8U
#define PINMUX_ETPWM3_SHIFT                 16U
#define PINMUX_ETPWM4_SHIFT                 24U
#define PINMUX_ETPWM5_SHIFT                 0U
#define PINMUX_ETPWM6_SHIFT                 8U
#define PINMUX_ETPWM7_SHIFT                 16U
#define PINMUX_ETPWM_TIME_BASE_SYNC_SHIFT   24U
#define PINMUX_ETPWM_TBCLK_SYNC_SHIFT       0U
#define PINMUX_TZ1_SHIFT                    16U
#define PINMUX_TZ2_SHIFT                    24U
#define PINMUX_TZ3_SHIFT                    0U
#define PINMUX_EPWM1SYNCI_SHIFT             8U

#define PINMUX_PIN_3_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_PIN_3_SHIFT))
#define PINMUX_PIN_4_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_PIN_4_SHIFT))
#define PINMUX_PIN_6_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_PIN_6_SHIFT))
#define PINMUX_PIN_9_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_PIN_9_SHIFT))
#define PINMUX_PIN_14_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_14_SHIFT))
#define PINMUX_PIN_16_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_16_SHIFT))
#define PINMUX_PIN_22_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_22_SHIFT))
#define PINMUX_PIN_23_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_23_SHIFT))
#define PINMUX_PIN_24_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_24_SHIFT))
#define PINMUX_PIN_25_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_25_SHIFT))
#define PINMUX_PIN_30_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_30_SHIFT))
#define PINMUX_PIN_31_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_31_SHIFT))
#define PINMUX_PIN_32_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_32_SHIFT))
#define PINMUX_PIN_33_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_33_SHIFT))
#define PINMUX_PIN_35_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_35_SHIFT))
#define PINMUX_PIN_36_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_36_SHIFT))
#define PINMUX_PIN_37_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_37_SHIFT))
#define PINMUX_PIN_38_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_38_SHIFT))
#define PINMUX_PIN_39_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_39_SHIFT))
#define PINMUX_PIN_40_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_40_SHIFT))
#define PINMUX_PIN_41_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_41_SHIFT))
#define PINMUX_PIN_51_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_51_SHIFT))
#define PINMUX_PIN_52_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_52_SHIFT))
#define PINMUX_PIN_53_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_53_SHIFT))
#define PINMUX_PIN_54_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_54_SHIFT))
#define PINMUX_PIN_55_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_55_SHIFT))
#define PINMUX_PIN_86_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_86_SHIFT))
#define PINMUX_PIN_91_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_91_SHIFT))
#define PINMUX_PIN_92_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_92_SHIFT))
#define PINMUX_PIN_96_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_96_SHIFT))
#define PINMUX_PIN_97_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_97_SHIFT))
#define PINMUX_PIN_98_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_98_SHIFT))
#define PINMUX_PIN_99_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_99_SHIFT))
#define PINMUX_PIN_100_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_100_SHIFT))
#define PINMUX_PIN_105_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_105_SHIFT))
#define PINMUX_PIN_106_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_106_SHIFT))
#define PINMUX_PIN_107_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_107_SHIFT))
#define PINMUX_PIN_118_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_118_SHIFT))
#define PINMUX_PIN_124_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_124_SHIFT))
#define PINMUX_PIN_127_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_127_SHIFT))
#define PINMUX_PIN_130_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_130_SHIFT))
#define PINMUX_PIN_139_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_139_SHIFT))
#define PINMUX_PIN_140_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_140_SHIFT))
#define PINMUX_PIN_141_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_141_SHIFT))
#define PINMUX_PIN_133_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_133_SHIFT))
#define PINMUX_PIN_1_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_PIN_1_SHIFT))
#define PINMUX_PIN_2_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_PIN_2_SHIFT))
#define PINMUX_PIN_5_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_PIN_5_SHIFT))
#define PINMUX_PIN_15_MASK                  (~(uint32)((uint32)0xFFU << PINMUX_PIN_15_SHIFT))
#define PINMUX_PIN_125_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_125_SHIFT))
#define PINMUX_PIN_126_MASK                 (~(uint32)((uint32)0xFFU << PINMUX_PIN_126_SHIFT))

#define PINMUX_GATE_EMIF_CLK_MASK           (~(uint32)((uint32)0xFFU << PINMUX_GATE_EMIF_CLK_SHIFT))
#define PINMUX_GIOB_DISABLE_HET2_MASK       (~(uint32)((uint32)0xFFU << PINMUX_GIOB_DISABLE_HET2_SHIFT))
#define PINMUX_ALT_ADC_TRIGGER_MASK         (~(uint32)((uint32)0xFFU << PINMUX_ALT_ADC_TRIGGER_SHIFT))
#define PINMUX_ETHERNET_MASK                (~(uint32)((uint32)0xFFU << PINMUX_ETHERNET_SHIFT))

#define PINMUX_ETPWM1_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_ETPWM1_SHIFT))
#define PINMUX_ETPWM2_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_ETPWM2_SHIFT))
#define PINMUX_ETPWM3_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_ETPWM3_SHIFT))
#define PINMUX_ETPWM4_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_ETPWM4_SHIFT))
#define PINMUX_ETPWM5_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_ETPWM5_SHIFT))
#define PINMUX_ETPWM6_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_ETPWM6_SHIFT))
#define PINMUX_ETPWM7_MASK                   (~(uint32)((uint32)0xFFU << PINMUX_ETPWM7_SHIFT))
#define PINMUX_ETPWM_TIME_BASE_SYNC_MASK     (~(uint32)((uint32)0xFFU << PINMUX_ETPWM_TIME_BASE_SYNC_SHIFT))
#define PINMUX_ETPWM_TBCLK_SYNC_MASK         (~(uint32)((uint32)0xFFU << PINMUX_ETPWM_TBCLK_SYNC_SHIFT))
#define PINMUX_TZ1_MASK                      (~(uint32)((uint32)0xFFU << PINMUX_TZ1_SHIFT))
#define PINMUX_TZ2_MASK                      (~(uint32)((uint32)0xFFU << PINMUX_TZ2_SHIFT))
#define PINMUX_TZ3_MASK                      (~(uint32)((uint32)0xFFU << PINMUX_TZ3_SHIFT))
#define PINMUX_EPWM1SYNCI_MASK               (~(uint32)((uint32)0xFFU << PINMUX_EPWM1SYNCI_SHIFT))


#define PINMUX_PIN_3_MIBSPI3NCS_3            ((uint32)((uint32)0x1U <<  PINMUX_PIN_3_SHIFT))
#define PINMUX_PIN_3_I2C_SCL                 ((uint32)((uint32)0x2U <<  PINMUX_PIN_3_SHIFT))
#define PINMUX_PIN_3_HET1_29                 ((uint32)((uint32)0x4U <<  PINMUX_PIN_3_SHIFT))
#define PINMUX_PIN_3_nTZ1                    ((uint32)((uint32)0x8U <<  PINMUX_PIN_3_SHIFT))

#define PINMUX_PIN_4_MIBSPI3NCS_2            ((uint32)((uint32)0x1U <<  PINMUX_PIN_4_SHIFT))
#define PINMUX_PIN_4_I2C_SDA                 ((uint32)((uint32)0x2U <<  PINMUX_PIN_4_SHIFT))
#define PINMUX_PIN_4_HET1_27                 ((uint32)((uint32)0x4U <<  PINMUX_PIN_4_SHIFT))
#define PINMUX_PIN_4_nTZ2                    ((uint32)((uint32)0x8U <<  PINMUX_PIN_4_SHIFT))

#define PINMUX_PIN_6_HET1_11                 ((uint32)((uint32)0x1U <<  PINMUX_PIN_6_SHIFT))
#define PINMUX_PIN_6_MIBSPI3NCS_4            ((uint32)((uint32)0x2U <<  PINMUX_PIN_6_SHIFT))
#define PINMUX_PIN_6_HET2_18                 ((uint32)((uint32)0x4U <<  PINMUX_PIN_6_SHIFT))
#define PINMUX_PIN_6_ETPWM1SYNCO             ((uint32)((uint32)0x20U <<  PINMUX_PIN_6_SHIFT))

#define PINMUX_PIN_9_GIOA_2                  ((uint32)((uint32)0x1U <<  PINMUX_PIN_9_SHIFT))
#define PINMUX_PIN_9_HET2_0                  ((uint32)((uint32)0x8U <<  PINMUX_PIN_9_SHIFT))
#define PINMUX_PIN_9_EQEP2I                  ((uint32)((uint32)0x10U <<  PINMUX_PIN_9_SHIFT))

#define PINMUX_PIN_14_GIOA_5                 ((uint32)((uint32)0x1U <<  PINMUX_PIN_14_SHIFT))
#define PINMUX_PIN_14_EXTCLKIN               ((uint32)((uint32)0x2U <<  PINMUX_PIN_14_SHIFT))
#define PINMUX_PIN_14_ETPWM1A                ((uint32)((uint32)0x4U <<  PINMUX_PIN_14_SHIFT))

#define PINMUX_PIN_16_GIOA_6                 ((uint32)((uint32)0x1U <<  PINMUX_PIN_16_SHIFT))
#define PINMUX_PIN_16_HET2_4                 ((uint32)((uint32)0x2U <<  PINMUX_PIN_16_SHIFT))
#define PINMUX_PIN_16_ETPWM1B                ((uint32)((uint32)0x4U <<  PINMUX_PIN_16_SHIFT))

#define PINMUX_PIN_22_GIOA_7                 ((uint32)((uint32)0x1U <<  PINMUX_PIN_22_SHIFT))
#define PINMUX_PIN_22_HET2_6                 ((uint32)((uint32)0x2U <<  PINMUX_PIN_22_SHIFT))
#define PINMUX_PIN_22_ETPWM2A                ((uint32)((uint32)0x4U <<  PINMUX_PIN_22_SHIFT))

#define PINMUX_PIN_23_HET1_01                ((uint32)((uint32)0x1U <<  PINMUX_PIN_23_SHIFT))
#define PINMUX_PIN_23_SPI4NENA               ((uint32)((uint32)0x2U <<  PINMUX_PIN_23_SHIFT))
#define PINMUX_PIN_23_HET2_8                 ((uint32)((uint32)0x10U <<  PINMUX_PIN_23_SHIFT))
#define PINMUX_PIN_23_EQEP2A                 ((uint32)((uint32)0x20U <<  PINMUX_PIN_23_SHIFT))

#define PINMUX_PIN_24_HET1_03                ((uint32)((uint32)0x1U <<  PINMUX_PIN_24_SHIFT))
#define PINMUX_PIN_24_SPI4NCS_0              ((uint32)((uint32)0x2U <<  PINMUX_PIN_24_SHIFT))
#define PINMUX_PIN_24_HET2_10                ((uint32)((uint32)0x10U <<  PINMUX_PIN_24_SHIFT))
#define PINMUX_PIN_24_EQEP2B                 ((uint32)((uint32)0x20U <<  PINMUX_PIN_24_SHIFT))

#define PINMUX_PIN_25_HET1_0                 ((uint32)((uint32)0x1U <<  PINMUX_PIN_25_SHIFT))
#define PINMUX_PIN_25_SPI4CLK                ((uint32)((uint32)0x2U <<  PINMUX_PIN_25_SHIFT))
#define PINMUX_PIN_25_ETPWM2B                ((uint32)((uint32)0x4U <<  PINMUX_PIN_25_SHIFT))

#define PINMUX_PIN_30_HET1_02                ((uint32)((uint32)0x1U <<  PINMUX_PIN_30_SHIFT))
#define PINMUX_PIN_30_SPI4SIMO               ((uint32)((uint32)0x2U <<  PINMUX_PIN_30_SHIFT))
#define PINMUX_PIN_30_ETPWM3A                ((uint32)((uint32)0x4U <<  PINMUX_PIN_30_SHIFT))

#define PINMUX_PIN_31_HET1_05                ((uint32)((uint32)0x1U <<  PINMUX_PIN_31_SHIFT))
#define PINMUX_PIN_31_SPI4SOMI               ((uint32)((uint32)0x2U <<  PINMUX_PIN_31_SHIFT))
#define PINMUX_PIN_31_HET2_12                ((uint32)((uint32)0x4U <<  PINMUX_PIN_31_SHIFT))
#define PINMUX_PIN_31_ETPWM3B                ((uint32)((uint32)0x8U <<  PINMUX_PIN_31_SHIFT))

#define PINMUX_PIN_32_MIBSPI5NCS_0           ((uint32)((uint32)0x1U <<  PINMUX_PIN_32_SHIFT))
#define PINMUX_PIN_32_ETPWM4A                ((uint32)((uint32)0x4U <<  PINMUX_PIN_32_SHIFT))

#define PINMUX_PIN_33_HET1_07                ((uint32)((uint32)0x1U <<  PINMUX_PIN_33_SHIFT))
#define PINMUX_PIN_33_HET2_14                ((uint32)((uint32)0x8U <<  PINMUX_PIN_33_SHIFT))
#define PINMUX_PIN_33_ETPWM7B                ((uint32)((uint32)0x10U <<  PINMUX_PIN_33_SHIFT))

#define PINMUX_PIN_35_HET1_09                ((uint32)((uint32)0x1U <<  PINMUX_PIN_35_SHIFT))
#define PINMUX_PIN_35_HET2_16                ((uint32)((uint32)0x2U <<  PINMUX_PIN_35_SHIFT))
#define PINMUX_PIN_35_ETPWM7A                ((uint32)((uint32)0x10U <<  PINMUX_PIN_35_SHIFT))

#define PINMUX_PIN_36_HET1_04                ((uint32)((uint32)0x1U <<  PINMUX_PIN_36_SHIFT))
#define PINMUX_PIN_36_ETPWM4B                ((uint32)((uint32)0x2U <<  PINMUX_PIN_36_SHIFT))

#define PINMUX_PIN_37_MIBSPI3NCS_1           ((uint32)((uint32)0x1U <<  PINMUX_PIN_37_SHIFT))
#define PINMUX_PIN_37_HET1_25                ((uint32)((uint32)0x2U <<  PINMUX_PIN_37_SHIFT))
#define PINMUX_PIN_37_MDCLK                  ((uint32)((uint32)0x4U <<  PINMUX_PIN_37_SHIFT))

#define PINMUX_PIN_38_HET1_06                ((uint32)((uint32)0x1U <<  PINMUX_PIN_38_SHIFT))
#define PINMUX_PIN_38_SCIRX                  ((uint32)((uint32)0x2U <<  PINMUX_PIN_38_SHIFT))
#define PINMUX_PIN_38_ETPWM5A                ((uint32)((uint32)0x4U <<  PINMUX_PIN_38_SHIFT))

#define PINMUX_PIN_39_HET1_13                ((uint32)((uint32)0x1U <<  PINMUX_PIN_39_SHIFT))
#define PINMUX_PIN_39_SCITX                  ((uint32)((uint32)0x2U <<  PINMUX_PIN_39_SHIFT))
#define PINMUX_PIN_39_ETPWM5B                ((uint32)((uint32)0x4U <<  PINMUX_PIN_39_SHIFT))

#define PINMUX_PIN_40_MIBSPI1NCS_2           ((uint32)((uint32)0x1U <<  PINMUX_PIN_40_SHIFT))
#define PINMUX_PIN_40_HET1_19                ((uint32)((uint32)0x2U <<  PINMUX_PIN_40_SHIFT))
#define PINMUX_PIN_40_MDIO                   ((uint32)((uint32)0x4U <<  PINMUX_PIN_40_SHIFT))

#define PINMUX_PIN_41_HET1_15                ((uint32)((uint32)0x1U <<  PINMUX_PIN_41_SHIFT))
#define PINMUX_PIN_41_MIBSPI1NCS_4           ((uint32)((uint32)0x2U <<  PINMUX_PIN_41_SHIFT))
#define PINMUX_PIN_41_ECAP1                  ((uint32)((uint32)0x4U <<  PINMUX_PIN_41_SHIFT))

#define PINMUX_PIN_51_MIBSPI3SOMI            ((uint32)((uint32)0x1U <<  PINMUX_PIN_51_SHIFT))
#define PINMUX_PIN_51_AWM_EXT_ENA            ((uint32)((uint32)0x2U <<  PINMUX_PIN_51_SHIFT))
#define PINMUX_PIN_51_ECAP2                  ((uint32)((uint32)0x4U <<  PINMUX_PIN_51_SHIFT))

#define PINMUX_PIN_52_MIBSPI3SIMO            ((uint32)((uint32)0x1U <<  PINMUX_PIN_52_SHIFT))
#define PINMUX_PIN_52_AWM_EXT_SEL_0          ((uint32)((uint32)0x2U <<  PINMUX_PIN_52_SHIFT))
#define PINMUX_PIN_52_ECAP3                  ((uint32)((uint32)0x4U <<  PINMUX_PIN_52_SHIFT))

#define PINMUX_PIN_53_MIBSPI3CLK             ((uint32)((uint32)0x1U <<  PINMUX_PIN_53_SHIFT))
#define PINMUX_PIN_53_AWM_EXT_SEL_1          ((uint32)((uint32)0x2U <<  PINMUX_PIN_53_SHIFT))
#define PINMUX_PIN_53_EQEP1A                 ((uint32)((uint32)0x4U <<  PINMUX_PIN_53_SHIFT))

#define PINMUX_PIN_54_MIBSPI3NENA            ((uint32)((uint32)0x1U <<  PINMUX_PIN_54_SHIFT))
#define PINMUX_PIN_54_MIBSPI3NCS_5           ((uint32)((uint32)0x2U <<  PINMUX_PIN_54_SHIFT))
#define PINMUX_PIN_54_HET1_31                ((uint32)((uint32)0x4U <<  PINMUX_PIN_54_SHIFT))
#define PINMUX_PIN_54_EQEP1B                 ((uint32)((uint32)0x8U <<  PINMUX_PIN_54_SHIFT))

#define PINMUX_PIN_55_MIBSPI3NCS_0           ((uint32)((uint32)0x1U <<  PINMUX_PIN_55_SHIFT))
#define PINMUX_PIN_55_AD2EVT                 ((uint32)((uint32)0x2U <<  PINMUX_PIN_55_SHIFT))
#define PINMUX_PIN_55_GIOB_2                 ((uint32)((uint32)0x4U <<  PINMUX_PIN_55_SHIFT))
#define PINMUX_PIN_55_EQEP1I                 ((uint32)((uint32)0x8U <<  PINMUX_PIN_55_SHIFT))

#define PINMUX_PIN_86_AD1EVT                 ((uint32)((uint32)0x1U <<  PINMUX_PIN_86_SHIFT))
#define PINMUX_PIN_86_MII_RX_ER              ((uint32)((uint32)0x2U <<  PINMUX_PIN_86_SHIFT))
#define PINMUX_PIN_86_RMII_RX_ER             ((uint32)((uint32)0x4U <<  PINMUX_PIN_86_SHIFT))

#define PINMUX_PIN_91_HET1_24                ((uint32)((uint32)0x1U <<  PINMUX_PIN_91_SHIFT))
#define PINMUX_PIN_91_MIBSPI1NCS_5           ((uint32)((uint32)0x2U <<  PINMUX_PIN_91_SHIFT))
#define PINMUX_PIN_91_MII_RXD_0              ((uint32)((uint32)0x4U <<  PINMUX_PIN_91_SHIFT))
#define PINMUX_PIN_91_RMII_RXD_0             ((uint32)((uint32)0x8U <<  PINMUX_PIN_91_SHIFT))

#define PINMUX_PIN_92_HET1_26                ((uint32)((uint32)0x1U <<  PINMUX_PIN_92_SHIFT))
#define PINMUX_PIN_92_MII_RXD_1              ((uint32)((uint32)0x2U <<  PINMUX_PIN_92_SHIFT))
#define PINMUX_PIN_92_RMII_RXD_1             ((uint32)((uint32)0x4U <<  PINMUX_PIN_92_SHIFT))

#define PINMUX_PIN_96_MIBSPI1NENA            ((uint32)((uint32)0x1U <<  PINMUX_PIN_96_SHIFT))
#define PINMUX_PIN_96_HET1_23                ((uint32)((uint32)0x2U <<  PINMUX_PIN_96_SHIFT))
#define PINMUX_PIN_96_MII_RXD_2              ((uint32)((uint32)0x4U <<  PINMUX_PIN_96_SHIFT))
#define PINMUX_PIN_96_ECAP4                  ((uint32)((uint32)0x10U <<  PINMUX_PIN_96_SHIFT))

#define PINMUX_PIN_97_MIBSPI5NENA            ((uint32)((uint32)0x1U <<  PINMUX_PIN_97_SHIFT))
#define PINMUX_PIN_97_MII_RXD_3              ((uint32)((uint32)0x4U <<  PINMUX_PIN_97_SHIFT))
#define PINMUX_PIN_97_MIBSPI5SOMI_1          ((uint32)((uint32)0x10U <<  PINMUX_PIN_97_SHIFT))
#define PINMUX_PIN_97_ECAP5                  ((uint32)((uint32)0x20U <<  PINMUX_PIN_97_SHIFT))

#define PINMUX_PIN_98_MIBSPI5SOMI_0          ((uint32)((uint32)0x1U <<  PINMUX_PIN_98_SHIFT))
#define PINMUX_PIN_98_MII_TXD_0              ((uint32)((uint32)0x4U <<  PINMUX_PIN_98_SHIFT))
#define PINMUX_PIN_98_RMII_TXD_0             ((uint32)((uint32)0x8U <<  PINMUX_PIN_98_SHIFT))

#define PINMUX_PIN_99_MIBSPI5SIMO_0          ((uint32)((uint32)0x1U <<  PINMUX_PIN_99_SHIFT))
#define PINMUX_PIN_99_MII_TXD_1              ((uint32)((uint32)0x4U <<  PINMUX_PIN_99_SHIFT))
#define PINMUX_PIN_99_RMII_TXD_1             ((uint32)((uint32)0x8U <<  PINMUX_PIN_99_SHIFT))
#define PINMUX_PIN_99_MIBSPI5SOMI_2          ((uint32)((uint32)0x10U <<  PINMUX_PIN_99_SHIFT))

#define PINMUX_PIN_100_MIBSPI5CLK            ((uint32)((uint32)0x1U <<  PINMUX_PIN_100_SHIFT))
#define PINMUX_PIN_100_MII_TXEN              ((uint32)((uint32)0x4U <<  PINMUX_PIN_100_SHIFT))
#define PINMUX_PIN_100_RMII_TXEN             ((uint32)((uint32)0x8U <<  PINMUX_PIN_100_SHIFT))

#define PINMUX_PIN_105_MIBSPI1NCS_0          ((uint32)((uint32)0x1U <<  PINMUX_PIN_105_SHIFT))
#define PINMUX_PIN_105_MIBSPI1SOMI_1         ((uint32)((uint32)0x2U <<  PINMUX_PIN_105_SHIFT))
#define PINMUX_PIN_105_MII_TXD_2             ((uint32)((uint32)0x4U <<  PINMUX_PIN_105_SHIFT))
#define PINMUX_PIN_105_ECAP6                 ((uint32)((uint32)0x10U <<  PINMUX_PIN_105_SHIFT))

#define PINMUX_PIN_106_HET1_08               ((uint32)((uint32)0x1U <<  PINMUX_PIN_106_SHIFT))
#define PINMUX_PIN_106_MIBSPI1SIMO_1         ((uint32)((uint32)0x2U <<  PINMUX_PIN_106_SHIFT))
#define PINMUX_PIN_106_MII_TXD_3             ((uint32)((uint32)0x4U <<  PINMUX_PIN_106_SHIFT))

#define PINMUX_PIN_107_HET1_28               ((uint32)((uint32)0x1U <<  PINMUX_PIN_107_SHIFT))
#define PINMUX_PIN_107_MII_RXCLK             ((uint32)((uint32)0x2U <<  PINMUX_PIN_107_SHIFT))
#define PINMUX_PIN_107_RMII_REFCLK           ((uint32)((uint32)0x4U <<  PINMUX_PIN_107_SHIFT))
#define PINMUX_PIN_107_MII_RX_AVCLK4         ((uint32)((uint32)0x8U <<  PINMUX_PIN_107_SHIFT))

#define PINMUX_PIN_118_HET1_10               ((uint32)((uint32)0x1U <<  PINMUX_PIN_118_SHIFT))
#define PINMUX_PIN_118_MII_TX_CLK            ((uint32)((uint32)0x2U <<  PINMUX_PIN_118_SHIFT))
#define PINMUX_PIN_118_MII_TX_AVCLK4         ((uint32)((uint32)0x8U <<  PINMUX_PIN_118_SHIFT))
#define PINMUX_PIN_118_nTZ3                  ((uint32)((uint32)0x10U <<  PINMUX_PIN_118_SHIFT))

#define PINMUX_PIN_124_HET1_12               ((uint32)((uint32)0x1U <<  PINMUX_PIN_124_SHIFT))
#define PINMUX_PIN_124_MII_CRS               ((uint32)((uint32)0x2U <<  PINMUX_PIN_124_SHIFT))
#define PINMUX_PIN_124_RMII_CRS_DV           ((uint32)((uint32)0x4U <<  PINMUX_PIN_124_SHIFT))

#define PINMUX_PIN_127_HET1_30               ((uint32)((uint32)0x1U <<  PINMUX_PIN_127_SHIFT))
#define PINMUX_PIN_127_MII_RX_DV             ((uint32)((uint32)0x2U <<  PINMUX_PIN_127_SHIFT))
#define PINMUX_PIN_127_EQEP2S                ((uint32)((uint32)0x8U <<  PINMUX_PIN_127_SHIFT))

#define PINMUX_PIN_130_MIBSPI1NCS_1          ((uint32)((uint32)0x1U <<  PINMUX_PIN_130_SHIFT))
#define PINMUX_PIN_130_HET1_17               ((uint32)((uint32)0x2U <<  PINMUX_PIN_130_SHIFT))
#define PINMUX_PIN_130_MII_COL               ((uint32)((uint32)0x4U <<  PINMUX_PIN_130_SHIFT))
#define PINMUX_PIN_130_EQEP1S                ((uint32)((uint32)0x10U <<  PINMUX_PIN_130_SHIFT))

#define PINMUX_PIN_139_HET1_16               ((uint32)((uint32)0x1U <<  PINMUX_PIN_139_SHIFT))
#define PINMUX_PIN_139_ETPWM1SYNCI           ((uint32)((uint32)0x2U <<  PINMUX_PIN_139_SHIFT))
#define PINMUX_PIN_139_ETPWM1SYNCO           ((uint32)((uint32)0x4U <<  PINMUX_PIN_139_SHIFT))

#define PINMUX_PIN_140_HET1_18               ((uint32)((uint32)0x1U <<  PINMUX_PIN_140_SHIFT))
#define PINMUX_PIN_140_ETPWM6A               ((uint32)((uint32)0x2U <<  PINMUX_PIN_140_SHIFT))

#define PINMUX_PIN_141_HET1_20               ((uint32)((uint32)0x1U <<  PINMUX_PIN_141_SHIFT))
#define PINMUX_PIN_141_ETPWM6B               ((uint32)((uint32)0x2U <<  PINMUX_PIN_141_SHIFT))

#define PINMUX_PIN_133_GIOB_1                ((uint32)((uint32)0x1U <<  PINMUX_PIN_133_SHIFT))

#define PINMUX_PIN_1_GIOB_3                  ((uint32)((uint32)0x1U <<  PINMUX_PIN_1_SHIFT))

#define PINMUX_PIN_2_GIOA_0                  ((uint32)((uint32)0x1U <<  PINMUX_PIN_2_SHIFT))

#define PINMUX_PIN_5_GIOA_1                  ((uint32)((uint32)0x1U <<  PINMUX_PIN_5_SHIFT))

#define PINMUX_PIN_15_HET1_22                ((uint32)((uint32)0x1U <<  PINMUX_PIN_15_SHIFT))

#define PINMUX_PIN_125_HET1_14               ((uint32)((uint32)0x1U <<  PINMUX_PIN_125_SHIFT))

#define PINMUX_PIN_126_GIOB_0                ((uint32)((uint32)0x1U <<  PINMUX_PIN_126_SHIFT))

#define PINMUX_GATE_EMIF_CLK_ON              ((uint32)((uint32)0x0 <<  PINMUX_GATE_EMIF_CLK_SHIFT))
#define PINMUX_GIOB_DISABLE_HET2_ON          ((uint32)((uint32)0x1U <<  PINMUX_GIOB_DISABLE_HET2_SHIFT))
#define PINMUX_GATE_EMIF_CLK_OFF             ((uint32)((uint32)0x1U <<  PINMUX_GATE_EMIF_CLK_SHIFT))
#define PINMUX_GIOB_DISABLE_HET2_OFF         ((uint32)((uint32)0x0 <<  PINMUX_GIOB_DISABLE_HET2_SHIFT))
#define PINMUX_ALT_ADC_TRIGGER_1             ((uint32)((uint32)0x1U <<  PINMUX_ALT_ADC_TRIGGER_SHIFT))
#define PINMUX_ALT_ADC_TRIGGER_2             ((uint32)((uint32)0x2U <<  PINMUX_ALT_ADC_TRIGGER_SHIFT))
#define PINMUX_ETHERNET_MII                  ((uint32)((uint32)0x0 <<  PINMUX_ETHERNET_SHIFT))
#define PINMUX_ETHERNET_RMII                 ((uint32)((uint32)0x1U <<  PINMUX_ETHERNET_SHIFT))

#define PINMUX_ETPWM1_EQEPERR12               ((uint32)((uint32)0x1U <<  PINMUX_ETPWM1_SHIFT))
#define PINMUX_ETPWM1_EQEPERR1                ((uint32)((uint32)0x2U <<  PINMUX_ETPWM1_SHIFT))
#define PINMUX_ETPWM1_EQEPERR2                ((uint32)((uint32)0x4U <<  PINMUX_ETPWM1_SHIFT))
#define PINMUX_ETPWM2_EQEPERR12               ((uint32)((uint32)0x1U <<  PINMUX_ETPWM2_SHIFT))
#define PINMUX_ETPWM2_EQEPERR1                ((uint32)((uint32)0x2U <<  PINMUX_ETPWM2_SHIFT))
#define PINMUX_ETPWM2_EQEPERR2                ((uint32)((uint32)0x4U <<  PINMUX_ETPWM2_SHIFT))
#define PINMUX_ETPWM3_EQEPERR12               ((uint32)((uint32)0x1U <<  PINMUX_ETPWM3_SHIFT))
#define PINMUX_ETPWM3_EQEPERR1                ((uint32)((uint32)0x2U <<  PINMUX_ETPWM3_SHIFT))
#define PINMUX_ETPWM3_EQEPERR2                ((uint32)((uint32)0x4U <<  PINMUX_ETPWM3_SHIFT))
#define PINMUX_ETPWM4_EQEPERR12               ((uint32)((uint32)0x1U <<  PINMUX_ETPWM4_SHIFT))
#define PINMUX_ETPWM4_EQEPERR1                ((uint32)((uint32)0x2U <<  PINMUX_ETPWM4_SHIFT))
#define PINMUX_ETPWM4_EQEPERR2                ((uint32)((uint32)0x4U <<  PINMUX_ETPWM4_SHIFT))
#define PINMUX_ETPWM5_EQEPERR12               ((uint32)((uint32)0x1U <<  PINMUX_ETPWM5_SHIFT))
#define PINMUX_ETPWM5_EQEPERR1                ((uint32)((uint32)0x2U <<  PINMUX_ETPWM5_SHIFT))
#define PINMUX_ETPWM5_EQEPERR2                ((uint32)((uint32)0x4U <<  PINMUX_ETPWM5_SHIFT))
#define PINMUX_ETPWM6_EQEPERR12               ((uint32)((uint32)0x1U <<  PINMUX_ETPWM6_SHIFT))
#define PINMUX_ETPWM6_EQEPERR1                ((uint32)((uint32)0x2U <<  PINMUX_ETPWM6_SHIFT))
#define PINMUX_ETPWM6_EQEPERR2                ((uint32)((uint32)0x4U <<  PINMUX_ETPWM6_SHIFT))
#define PINMUX_ETPWM7_EQEPERR12               ((uint32)((uint32)0x1U <<  PINMUX_ETPWM7_SHIFT))
#define PINMUX_ETPWM7_EQEPERR1                ((uint32)((uint32)0x2U <<  PINMUX_ETPWM7_SHIFT))
#define PINMUX_ETPWM7_EQEPERR2                ((uint32)((uint32)0x4U <<  PINMUX_ETPWM7_SHIFT))
#define PINMUX_ETPWM_TIME_BASE_SYNC_ON        ((uint32)((uint32)0x2U <<  PINMUX_ETPWM_TIME_BASE_SYNC_SHIFT))
#define PINMUX_ETPWM_TBCLK_SYNC_ON            ((uint32)((uint32)0x2U <<  PINMUX_ETPWM_TBCLK_SYNC_SHIFT))
#define PINMUX_ETPWM_TIME_BASE_SYNC_OFF       ((uint32)((uint32)0x0 <<  PINMUX_ETPWM_TIME_BASE_SYNC_SHIFT))
#define PINMUX_ETPWM_TBCLK_SYNC_OFF           ((uint32)((uint32)0x0 <<  PINMUX_ETPWM_TBCLK_SYNC_SHIFT))
#define PINMUX_TZ1_ASYNC                      ((uint32)((uint32)0x1U <<  PINMUX_TZ1_SHIFT))
#define PINMUX_TZ1_SYNC                       ((uint32)((uint32)0x2U <<  PINMUX_TZ1_SHIFT))
#define PINMUX_TZ1_FILTERED                   ((uint32)((uint32)0x4U <<  PINMUX_TZ1_SHIFT))
#define PINMUX_TZ2_ASYNC                      ((uint32)((uint32)0x1U <<  PINMUX_TZ2_SHIFT))
#define PINMUX_TZ2_SYNC                       ((uint32)((uint32)0x2U <<  PINMUX_TZ2_SHIFT))
#define PINMUX_TZ2_FILTERED                   ((uint32)((uint32)0x4U <<  PINMUX_TZ2_SHIFT))
#define PINMUX_TZ3_ASYNC                      ((uint32)((uint32)0x1U <<  PINMUX_TZ3_SHIFT))
#define PINMUX_TZ3_SYNC                       ((uint32)((uint32)0x2U <<  PINMUX_TZ3_SHIFT))
#define PINMUX_TZ3_FILTERED                   ((uint32)((uint32)0x4U <<  PINMUX_TZ3_SHIFT))
#define PINMUX_EPWM1SYNCI_ASYNC               ((uint32)((uint32)0x1U <<  PINMUX_EPWM1SYNCI_SHIFT))
#define PINMUX_EPWM1SYNCI_SYNC                ((uint32)((uint32)0x2U <<  PINMUX_EPWM1SYNCI_SHIFT))
#define PINMUX_EPWM1SYNCI_FILTERED            ((uint32)((uint32)0x4U <<  PINMUX_EPWM1SYNCI_SHIFT))

typedef struct pinmux_config_reg
{
    uint32 CONFIG_PINMMR0;
    uint32 CONFIG_PINMMR1;
    uint32 CONFIG_PINMMR2;
    uint32 CONFIG_PINMMR3;
    uint32 CONFIG_PINMMR4;
    uint32 CONFIG_PINMMR5;
    uint32 CONFIG_PINMMR6;
    uint32 CONFIG_PINMMR7;
    uint32 CONFIG_PINMMR8;
    uint32 CONFIG_PINMMR9;
    uint32 CONFIG_PINMMR10;
    uint32 CONFIG_PINMMR11;
    uint32 CONFIG_PINMMR12;
    uint32 CONFIG_PINMMR13;
    uint32 CONFIG_PINMMR14;
    uint32 CONFIG_PINMMR15;
    uint32 CONFIG_PINMMR16;
    uint32 CONFIG_PINMMR17;
    uint32 CONFIG_PINMMR18;
    uint32 CONFIG_PINMMR19;
    uint32 CONFIG_PINMMR20;
    uint32 CONFIG_PINMMR21;
    uint32 CONFIG_PINMMR22;
    uint32 CONFIG_PINMMR23;
    uint32 CONFIG_PINMMR24;
    uint32 CONFIG_PINMMR25;
    uint32 CONFIG_PINMMR26;
    uint32 CONFIG_PINMMR27;
    uint32 CONFIG_PINMMR28;
    uint32 CONFIG_PINMMR29;
    uint32 CONFIG_PINMMR30;
    uint32 CONFIG_PINMMR31;
    uint32 CONFIG_PINMMR32;
    uint32 CONFIG_PINMMR33;
    uint32 CONFIG_PINMMR34;
    uint32 CONFIG_PINMMR35;
    uint32 CONFIG_PINMMR36;
    uint32 CONFIG_PINMMR37;
    uint32 CONFIG_PINMMR38;
    uint32 CONFIG_PINMMR39;
    uint32 CONFIG_PINMMR40;
    uint32 CONFIG_PINMMR41;
    uint32 CONFIG_PINMMR42;
    uint32 CONFIG_PINMMR43;
    uint32 CONFIG_PINMMR44;
    uint32 CONFIG_PINMMR45;
    uint32 CONFIG_PINMMR46;
    uint32 CONFIG_PINMMR47;
}pinmux_config_reg_t;

/**
 *  @defgroup IOMM IOMM
 *  @brief I/O Multiplexing and Control Module.
 *
 *  The IOMM contains memory-mapped registers (MMR) that control device-specific multiplexed functions.
 *  The safety and diagnostic features of the IOMM are:
 *  - Kicker mechanism to protect the MMRs from accidental writes
 *  - Master-id checker to only allow the CPU to write to the MMRs
 *  - Error indication for access violations
 *
 *  Related Files
 *   - reg_pinmux.h
 *   - pinmux.h
 *   - pinmux.c
 *  @addtogroup IOMM
 *  @{
 */

/** @fn void muxInit(void)
*   @brief Initializes the PINMUX Driver
*
*   This function initializes the PINMUX module and configures the selected
*   pinmux settings as per the user selection in the GUI
*/
void muxInit(void);
void pinmuxGetConfigValue(pinmux_config_reg_t *config_reg, config_value_type_t type);
/* USER CODE BEGIN (0) */
/* USER CODE END */

/**@}*/
#ifdef __cplusplus
}
#endif /*extern "C" */
#endif
