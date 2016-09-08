/** @file pinmux.c
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


/* Include Files */

#include "pinmux.h"

/*LDRA_INSPECTWINDOW 50 */
/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 76 S MR: 19.12 REVIEWED " Needs usage of multiple ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
/*SAFETYMCUSW 76 S MR: 19.12 REVIEWED " Needs usage of multiple ## in the macro " */
/*SAFETYMCUSW 76 S MR: 19.12 REVIEWED " Needs usage of multiple ## in the macro " */
/*SAFETYMCUSW 76 S MR: 19.12 REVIEWED " Needs usage of multiple ## in the macro " */
/*SAFETYMCUSW 76 S MR: 19.12 REVIEWED " Needs usage of multiple ## in the macro " */
/*SAFETYMCUSW 76 S MR: 19.12 REVIEWED " Needs usage of multiple ## in the macro " */
/*SAFETYMCUSW 76 S MR: 19.12 REVIEWED " Needs usage of multiple ## in the macro " */
/*SAFETYMCUSW 76 S MR: 19.12 REVIEWED " Needs usage of multiple ## in the macro " */
#define PINMUX_SET(REG, PINNUM, MUX)                                \
            (pinMuxReg->PINMMR##REG = (pinMuxReg->PINMMR##REG & PINMUX_PIN_##PINNUM##_MASK) | (PINMUX_PIN_##PINNUM##_##MUX))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_GATE_EMIF_CLK_ENABLE(state)      \
            (pinMuxReg->PINMMR29 = (pinMuxReg->PINMMR29 & PINMUX_GATE_EMIF_CLK_MASK) | (PINMUX_GATE_EMIF_CLK_##state))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_GIOB_DISABLE_HET2_ENABLE(state)  \
            (pinMuxReg->PINMMR29 = (pinMuxReg->PINMMR29 & PINMUX_GIOB_DISABLE_HET2_MASK) | (PINMUX_GIOB_DISABLE_HET2_##state))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ALT_ADC_TRIGGER_SELECT(num)  \
            (pinMuxReg->PINMMR30 = (pinMuxReg->PINMMR30 & PINMUX_ALT_ADC_TRIGGER_MASK) | (PINMUX_ALT_ADC_TRIGGER_##num))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETHERNET_SELECT(interface)   \
            (pinMuxReg->PINMMR29 = (pinMuxReg->PINMMR29 & PINMUX_ETHERNET_MASK) | (PINMUX_ETHERNET_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM1_EQEPERR_ENABLE(interface)     \
            (pinMuxReg->PINMMR41 = (pinMuxReg->PINMMR41 & PINMUX_ETPWM1_MASK) | (PINMUX_ETPWM1_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM2_EQEPERR_ENABLE(interface)     \
            (pinMuxReg->PINMMR41 = (pinMuxReg->PINMMR41 & PINMUX_ETPWM2_MASK) | (PINMUX_ETPWM2_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM3_EQEPERR_ENABLE(interface)     \
            (pinMuxReg->PINMMR41 = (pinMuxReg->PINMMR41 & PINMUX_ETPWM3_MASK) | (PINMUX_ETPWM3_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM4_EQEPERR_ENABLE(interface)     \
            (pinMuxReg->PINMMR41 = (pinMuxReg->PINMMR41 & PINMUX_ETPWM4_MASK) | (PINMUX_ETPWM4_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM5_EQEPERR_ENABLE(interface)     \
            (pinMuxReg->PINMMR42 = (pinMuxReg->PINMMR42 & PINMUX_ETPWM5_MASK) | (PINMUX_ETPWM5_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM6_EQEPERR_ENABLE(interface)     \
            (pinMuxReg->PINMMR42 = (pinMuxReg->PINMMR42 & PINMUX_ETPWM6_MASK) | (PINMUX_ETPWM6_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM7_EQEPERR_ENABLE(interface)     \
            (pinMuxReg->PINMMR42 = (pinMuxReg->PINMMR42 & PINMUX_ETPWM7_MASK) | (PINMUX_ETPWM7_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM_TZ1_ENABLE(interface)      \
            (pinMuxReg->PINMMR46 = (pinMuxReg->PINMMR46 & PINMUX_TZ1_SHIFT) | (PINMUX_TZ1_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM_TZ2_ENABLE(interface)      \
            (pinMuxReg->PINMMR46 = (pinMuxReg->PINMMR46 & PINMUX_TZ2_SHIFT) | (PINMUX_TZ2_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM_TZ3_ENABLE(interface)      \
            (pinMuxReg->PINMMR47 = (pinMuxReg->PINMMR47 & PINMUX_TZ3_SHIFT) | (PINMUX_TZ3_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM_EPWM1SYNCI_ENABLE(interface)       \
            (pinMuxReg->PINMMR47 = (pinMuxReg->PINMMR47 & PINMUX_EPWM1SYNCI_SHIFT) | (PINMUX_EPWM1SYNCI_##interface))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM_TIME_BASE_SYNC_ENABLE(state)       \
            (pinMuxReg->PINMMR36 = (pinMuxReg->PINMMR36 & PINMUX_ETPWM_TIME_BASE_SYNC_MASK) | (PINMUX_ETPWM_TIME_BASE_SYNC_##state))

/*SAFETYMCUSW 125 S MR: 19.13 REVIEWED " Needs usage of ## in the macro " */
/*SAFETYMCUSW 78 S MR: 19.10 REVIEWED " Macro parameter used for concatenation " */
#define PINMUX_ETPWM_TBCLK_SYNC_ENABLE(state)       \
            (pinMuxReg->PINMMR37 = (pinMuxReg->PINMMR37 & PINMUX_ETPWM_TBCLK_SYNC_MASK) | (PINMUX_ETPWM_TBCLK_SYNC_##state))




/* USER CODE BEGIN (0) */
/* USER CODE END */

/* SourceId : PINMUX_SourceId_001 */
/* DesignId : PINMUX_DesignId_001 */
/* Requirements : HL_SR325 */
void muxInit(void){

/* USER CODE BEGIN (1) */
/* USER CODE END */

    /* Enable Pin Muxing */
    kickerReg->KICKER0 = 0x83E70B13U;
    kickerReg->KICKER1 = 0x95A4F1E0U;

/* USER CODE BEGIN (2) */
/* USER CODE END */

    pinMuxReg->PINMMR0 =    PINMUX_PIN_1_GIOB_3 | PINMUX_PIN_2_GIOA_0 | PINMUX_PIN_3_MIBSPI3NCS_3 | PINMUX_PIN_4_MIBSPI3NCS_2;

    pinMuxReg->PINMMR1 =    PINMUX_PIN_5_GIOA_1 | PINMUX_PIN_6_HET1_11;

    pinMuxReg->PINMMR2 =    PINMUX_PIN_9_GIOA_2 | PINMUX_PIN_14_GIOA_5;

    pinMuxReg->PINMMR3 =    PINMUX_PIN_15_HET1_22 | PINMUX_PIN_16_GIOA_6;

    pinMuxReg->PINMMR4 =    PINMUX_PIN_22_GIOA_7 | PINMUX_PIN_23_HET1_01 | PINMUX_PIN_24_HET1_03;

    pinMuxReg->PINMMR5 =    PINMUX_PIN_25_HET1_0 | PINMUX_PIN_30_HET1_02 | PINMUX_PIN_31_HET1_05;

    pinMuxReg->PINMMR6 =    PINMUX_PIN_33_HET1_07 | PINMUX_PIN_35_HET1_09;

    pinMuxReg->PINMMR7 =    PINMUX_PIN_37_MIBSPI3NCS_1 | PINMUX_PIN_38_HET1_06;

    pinMuxReg->PINMMR8 =    PINMUX_PIN_39_HET1_13 | PINMUX_PIN_40_MIBSPI1NCS_2 | PINMUX_PIN_41_HET1_15;

    pinMuxReg->PINMMR9 = ((~(pinMuxReg->PINMMR9 >> 18U) & 0x00000001U ) << 18U) | PINMUX_PIN_54_MIBSPI3NENA | PINMUX_PIN_55_MIBSPI3NCS_0;

    pinMuxReg->PINMMR10 =   PINMUX_PIN_86_AD1EVT;

    pinMuxReg->PINMMR11 =   PINMUX_PIN_91_HET1_24;

    pinMuxReg->PINMMR12 =   PINMUX_PIN_92_HET1_26 | PINMUX_PIN_96_MIBSPI1NENA | PINMUX_PIN_97_MIBSPI5NENA;

    pinMuxReg->PINMMR13 =   PINMUX_PIN_98_MIBSPI5SOMI_0 | PINMUX_PIN_99_MIBSPI5SIMO_0 | PINMUX_PIN_100_MIBSPI5CLK | PINMUX_PIN_105_MIBSPI1NCS_0;

    pinMuxReg->PINMMR14 =   PINMUX_PIN_106_HET1_08 | PINMUX_PIN_107_HET1_28;

    pinMuxReg->PINMMR15 =   0x01010101U;

    pinMuxReg->PINMMR16 =   0x01010101U;

    pinMuxReg->PINMMR17 =   PINMUX_PIN_118_HET1_10 | PINMUX_PIN_124_HET1_12;

    pinMuxReg->PINMMR18 =   PINMUX_PIN_125_HET1_14 | PINMUX_PIN_126_GIOB_0;

    pinMuxReg->PINMMR19 =   PINMUX_PIN_127_HET1_30;

    pinMuxReg->PINMMR20 =   PINMUX_PIN_130_MIBSPI1NCS_1;

    pinMuxReg->PINMMR21 =   PINMUX_PIN_133_GIOB_1;

    pinMuxReg->PINMMR22 =   0x01010101U;

    pinMuxReg->PINMMR23 =   ((~(pinMuxReg->PINMMR5 >> 1U) & 0x00000001U ) << 8U) | ((~(pinMuxReg->PINMMR5 >> 9U) & 0x00000001U ) << 16U) | ((~(pinMuxReg->PINMMR5 >> 17U) & 0x00000001U ) << 24U);

    pinMuxReg->PINMMR24 =   ((~(pinMuxReg->PINMMR20 >> 17U) & 0x00000001U ) << 16U)  | ((~(pinMuxReg->PINMMR8 >> 9U) & 0x00000001U ) << 24U);

    /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
    pinMuxReg->PINMMR25 =   ((~(pinMuxReg->PINMMR12 >> 17U) & 0x00000001U ) << 8U) | ((~(pinMuxReg->PINMMR7 >> 9U) & 0x00000001U ) << 16U) | ((~(pinMuxReg->PINMMR0 >> 26U) & 0x00000001U ) << 24U);

    /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
    pinMuxReg->PINMMR26 =   ((~(pinMuxReg->PINMMR0 >> 18U) & 0x00000001U ) << 0U) | ((~(pinMuxReg->PINMMR9 >> 10U) & 0x00000001U ) << 8U);

    pinMuxReg->PINMMR27 =   PINMUX_PIN_32_MIBSPI5NCS_0;

    pinMuxReg->PINMMR29 =   0x01010101U;

    pinMuxReg->PINMMR30 = 0x01010100U;

    pinMuxReg->PINMMR31 = 0x01010101U;

    pinMuxReg->PINMMR32 = 0x00010101U;

    pinMuxReg->PINMMR33 =   PINMUX_PIN_36_HET1_04 | PINMUX_PIN_51_MIBSPI3SOMI | PINMUX_PIN_52_MIBSPI3SIMO | PINMUX_PIN_53_MIBSPI3CLK;

    pinMuxReg->PINMMR34 =   PINMUX_PIN_139_HET1_16 | PINMUX_PIN_140_HET1_18 | PINMUX_PIN_141_HET1_20;


/* USER CODE BEGIN (3) */
/* USER CODE END */

    PINMUX_GATE_EMIF_CLK_ENABLE(OFF);
    PINMUX_GIOB_DISABLE_HET2_ENABLE(OFF);
    PINMUX_ALT_ADC_TRIGGER_SELECT(1);

/* USER CODE BEGIN (4) */
/* USER CODE END */

    /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
    PINMUX_SET(0,1,GIOB_3);
    /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
    PINMUX_SET(0,2,GIOA_0);
    PINMUX_SET(1,5,GIOA_1);
    PINMUX_SET(3,15,HET1_22);
    PINMUX_SET(18,125,HET1_14);
    PINMUX_SET(18,126,GIOB_0);
    PINMUX_SET(21,133,GIOB_1);

/* USER CODE BEGIN (5) */
/* USER CODE END */

    PINMUX_ETPWM1_EQEPERR_ENABLE(EQEPERR12);
    PINMUX_ETPWM2_EQEPERR_ENABLE(EQEPERR12);
    PINMUX_ETPWM3_EQEPERR_ENABLE(EQEPERR12);
    PINMUX_ETPWM4_EQEPERR_ENABLE(EQEPERR12);
    PINMUX_ETPWM5_EQEPERR_ENABLE(EQEPERR12);
    PINMUX_ETPWM6_EQEPERR_ENABLE(EQEPERR12);
    PINMUX_ETPWM7_EQEPERR_ENABLE(EQEPERR12);
    PINMUX_ETPWM_TIME_BASE_SYNC_ENABLE(OFF);
    PINMUX_ETPWM_TZ1_ENABLE(ASYNC);
    PINMUX_ETPWM_TZ2_ENABLE(ASYNC);
    PINMUX_ETPWM_TZ3_ENABLE(ASYNC);
    PINMUX_ETPWM_EPWM1SYNCI_ENABLE(ASYNC);

/* USER CODE BEGIN (6) */
/* USER CODE END */

    /* Disable Pin Muxing */
    kickerReg->KICKER0 = 0x00000000U;
    kickerReg->KICKER1 = 0x00000000U;

/* USER CODE BEGIN (7) */
/* USER CODE END */
}

/** @fn void pinmuxGetConfigValue(pinmux_config_reg_t *config_reg, config_value_type_t type)
*   @brief Get the initial or current values of the configuration registers
*
*   @param[in] *config_reg: pointer to the struct to which the initial or current
*                           value of the configuration registers need to be stored
*   @param[in] type:    whether initial or current value of the configuration registers need to be stored
*                       - InitialValue: initial value of the configuration registers will be stored
*                                       in the struct pointed by config_reg
*                       - CurrentValue: initial value of the configuration registers will be stored
*                                       in the struct pointed by config_reg
*
*   This function will copy the initial or current value (depending on the parameter 'type')
*   of the configuration registers to the struct pointed by config_reg
*
*/
/* SourceId : PINMUX_SourceId_002 */
/* DesignId : PINMUX_DesignId_002 */
/* Requirements : HL_SR328 */
void pinmuxGetConfigValue(pinmux_config_reg_t *config_reg, config_value_type_t type)
{
    if (type == InitialValue)
    {/* Do not pass Initial Value as parameter */
    }
    else
    {
    /*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
        config_reg->CONFIG_PINMMR0  = pinMuxReg->PINMMR0;
        config_reg->CONFIG_PINMMR1  = pinMuxReg->PINMMR1;
        config_reg->CONFIG_PINMMR2  = pinMuxReg->PINMMR2;
        config_reg->CONFIG_PINMMR3  = pinMuxReg->PINMMR3;
        config_reg->CONFIG_PINMMR4  = pinMuxReg->PINMMR4;
        config_reg->CONFIG_PINMMR5  = pinMuxReg->PINMMR5;
        config_reg->CONFIG_PINMMR6  = pinMuxReg->PINMMR6;
        config_reg->CONFIG_PINMMR7  = pinMuxReg->PINMMR7;
        config_reg->CONFIG_PINMMR8  = pinMuxReg->PINMMR8;
        config_reg->CONFIG_PINMMR9  = pinMuxReg->PINMMR9;
        config_reg->CONFIG_PINMMR10 = pinMuxReg->PINMMR10;
        config_reg->CONFIG_PINMMR11 = pinMuxReg->PINMMR11;
        config_reg->CONFIG_PINMMR12 = pinMuxReg->PINMMR12;
        config_reg->CONFIG_PINMMR13 = pinMuxReg->PINMMR13;
        config_reg->CONFIG_PINMMR14 = pinMuxReg->PINMMR14;
        config_reg->CONFIG_PINMMR15 = pinMuxReg->PINMMR15;
        config_reg->CONFIG_PINMMR16 = pinMuxReg->PINMMR16;
        config_reg->CONFIG_PINMMR17 = pinMuxReg->PINMMR17;
        config_reg->CONFIG_PINMMR18 = pinMuxReg->PINMMR18;
        config_reg->CONFIG_PINMMR19 = pinMuxReg->PINMMR19;
        config_reg->CONFIG_PINMMR20 = pinMuxReg->PINMMR20;
        config_reg->CONFIG_PINMMR21 = pinMuxReg->PINMMR21;
        config_reg->CONFIG_PINMMR22 = pinMuxReg->PINMMR22;
        config_reg->CONFIG_PINMMR23 = pinMuxReg->PINMMR23;
        config_reg->CONFIG_PINMMR24 = pinMuxReg->PINMMR24;
        config_reg->CONFIG_PINMMR25 = pinMuxReg->PINMMR25;
        config_reg->CONFIG_PINMMR26 = pinMuxReg->PINMMR26;
        config_reg->CONFIG_PINMMR27 = pinMuxReg->PINMMR27;
        config_reg->CONFIG_PINMMR28 = pinMuxReg->PINMMR28;
        config_reg->CONFIG_PINMMR29 = pinMuxReg->PINMMR29;
        config_reg->CONFIG_PINMMR30 = pinMuxReg->PINMMR30;
        config_reg->CONFIG_PINMMR31 = pinMuxReg->PINMMR31;
        config_reg->CONFIG_PINMMR32 = pinMuxReg->PINMMR32;
        config_reg->CONFIG_PINMMR33 = pinMuxReg->PINMMR33;
        config_reg->CONFIG_PINMMR34 = pinMuxReg->PINMMR34;
        config_reg->CONFIG_PINMMR35 = pinMuxReg->PINMMR35;
        config_reg->CONFIG_PINMMR36 = pinMuxReg->PINMMR36;
        config_reg->CONFIG_PINMMR37 = pinMuxReg->PINMMR37;
        config_reg->CONFIG_PINMMR38 = pinMuxReg->PINMMR38;
        config_reg->CONFIG_PINMMR39 = pinMuxReg->PINMMR39;
        config_reg->CONFIG_PINMMR40 = pinMuxReg->PINMMR40;
        config_reg->CONFIG_PINMMR41 = pinMuxReg->PINMMR41;
        config_reg->CONFIG_PINMMR42 = pinMuxReg->PINMMR42;
        config_reg->CONFIG_PINMMR43 = pinMuxReg->PINMMR43;
        config_reg->CONFIG_PINMMR44 = pinMuxReg->PINMMR44;
        config_reg->CONFIG_PINMMR45 = pinMuxReg->PINMMR45;
        config_reg->CONFIG_PINMMR46 = pinMuxReg->PINMMR46;
        config_reg->CONFIG_PINMMR47 = pinMuxReg->PINMMR47;
    }
}

/* USER CODE BEGIN (8) */
/* USER CODE END */
