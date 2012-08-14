/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY
* DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved. */

/******************************************************************************
* File Name : font_winfreesystem14x16.c
* Version : 1.00
* Tool-Chain : Glyph - The Generic API for Graphics LCD API
* H/W Platform : RX62N Group
* Description : An 14 x 16 Font.
******************************************************************************
* History :
* : 21.07.2010 1.00 First Release
******************************************************************************/
#include <stdint.h>

// const uint8_t FontWinFreeSystem14x16_char_0[] = {};
// const uint8_t FontWinFreeSystem14x16_char_1[] = {};
// const uint8_t FontWinFreeSystem14x16_char_2[] = {};
// const uint8_t FontWinFreeSystem14x16_char_3[] = {};
// const uint8_t FontWinFreeSystem14x16_char_4[] = {};
// const uint8_t FontWinFreeSystem14x16_char_5[] = {};
// const uint8_t FontWinFreeSystem14x16_char_6[] = {};
// const uint8_t FontWinFreeSystem14x16_char_7[] = {};
// const uint8_t FontWinFreeSystem14x16_char_8[] = {};
// const uint8_t FontWinFreeSystem14x16_char_9[] = {};
// const uint8_t FontWinFreeSystem14x16_char_10[] = {};
// const uint8_t FontWinFreeSystem14x16_char_11[] = {};
// const uint8_t FontWinFreeSystem14x16_char_12[] = {};
// const uint8_t FontWinFreeSystem14x16_char_13[] = {};
// const uint8_t FontWinFreeSystem14x16_char_14[] = {};
// const uint8_t FontWinFreeSystem14x16_char_15[] = {};
// const uint8_t FontWinFreeSystem14x16_char_16[] = {};
// const uint8_t FontWinFreeSystem14x16_char_17[] = {};
// const uint8_t FontWinFreeSystem14x16_char_18[] = {};
// const uint8_t FontWinFreeSystem14x16_char_19[] = {};
// const uint8_t FontWinFreeSystem14x16_char_20[] = {};
// const uint8_t FontWinFreeSystem14x16_char_21[] = {};
// const uint8_t FontWinFreeSystem14x16_char_22[] = {};
// const uint8_t FontWinFreeSystem14x16_char_23[] = {};
// const uint8_t FontWinFreeSystem14x16_char_24[] = {};
// const uint8_t FontWinFreeSystem14x16_char_25[] = {};
// const uint8_t FontWinFreeSystem14x16_char_26[] = {};
// const uint8_t FontWinFreeSystem14x16_char_27[] = {};
// const uint8_t FontWinFreeSystem14x16_char_28[] = {};
// const uint8_t FontWinFreeSystem14x16_char_29[] = {};
// const uint8_t FontWinFreeSystem14x16_char_30[] = {};
// const uint8_t FontWinFreeSystem14x16_char_31[] = {};
const uint8_t FontWinFreeSystem14x16_char_32[] = {
    0x03, 0x0C, // width=3, height=12
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_33[] = {
    0x03, 0x0C, // width=3, height=12
    0x00, 0xFC, 0x00,
    0x00, 0x02, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_34[] = {
    0x04, 0x0C, // width=4, height=12
    0x00, 0x0C, 0x00, 0x0C,
    0x00, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_35[] = {
    0x06, 0x0C, // width=6, height=12
    0x80, 0xA0, 0xF8, 0xA0, 0xF8, 0x20,
    0x00, 0x03, 0x00, 0x03, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_36[] = {
    0x06, 0x0C, // width=6, height=12
    0x30, 0x48, 0xFC, 0x48, 0x90, 0x00,
    0x01, 0x02, 0x07, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_37[] = {
    0x09, 0x0C, // width=9, height=12
    0x08, 0x14, 0x14, 0xC8, 0x30, 0x8C, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x01, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_38[] = {
    0x08, 0x0C, // width=8, height=12
    0x00, 0xC0, 0x38, 0x64, 0x98, 0x80, 0x40, 0x00,
    0x00, 0x01, 0x02, 0x02, 0x01, 0x01, 0x02, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_39[] = {
    0x03, 0x0C, // width=3, height=12
    0x00, 0x10, 0x0C,
    0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_40[] = {
    0x04, 0x0C, // width=4, height=12
    0xE0, 0x18, 0x04, 0x00,
    0x01, 0x06, 0x08, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_41[] = {
    0x04, 0x0C, // width=4, height=12
    0x00, 0x04, 0x18, 0xE0,
    0x00, 0x08, 0x06, 0x01,
};
const uint8_t FontWinFreeSystem14x16_char_42[] = {
    0x04, 0x0C, // width=4, height=12
    0x14, 0x08, 0x14, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_43[] = {
    0x06, 0x0C, // width=6, height=12
    0x40, 0x40, 0xF0, 0x40, 0x40, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_44[] = {
    0x03, 0x0C, // width=3, height=12
    0x00, 0x00, 0x00,
    0x08, 0x06, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_45[] = {
    0x07, 0x0C, // width=7, height=12
    0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_46[] = {
    0x03, 0x0C, // width=3, height=12
    0x00, 0x00, 0x00,
    0x00, 0x02, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_47[] = {
    0x03, 0x0C, // width=3, height=12
    0x00, 0xF0, 0x0C,
    0x03, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_48[] = {
    0x06, 0x0C, // width=6, height=12
    0xF8, 0x04, 0x04, 0x04, 0xF8, 0x00,
    0x01, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_49[] = {
    0x06, 0x0C, // width=6, height=12
    0x00, 0x08, 0xFC, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_50[] = {
    0x06, 0x0C, // width=6, height=12
    0x08, 0x84, 0x44, 0x44, 0x38, 0x00,
    0x03, 0x02, 0x02, 0x02, 0x02, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_51[] = {
    0x06, 0x0C, // width=6, height=12
    0x08, 0x04, 0x24, 0x24, 0xD8, 0x00,
    0x01, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_52[] = {
    0x06, 0x0C, // width=6, height=12
    0xC0, 0xB0, 0x88, 0xFC, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_53[] = {
    0x06, 0x0C, // width=6, height=12
    0x3C, 0x24, 0x24, 0x24, 0xC4, 0x00,
    0x01, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_54[] = {
    0x06, 0x0C, // width=6, height=12
    0xF8, 0x44, 0x24, 0x24, 0xC8, 0x00,
    0x01, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_55[] = {
    0x06, 0x0C, // width=6, height=12
    0x04, 0x04, 0xC4, 0x34, 0x0C, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_56[] = {
    0x06, 0x0C, // width=6, height=12
    0xD8, 0x24, 0x24, 0x24, 0xD8, 0x00,
    0x01, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_57[] = {
    0x06, 0x0C, // width=6, height=12
    0x38, 0x44, 0x44, 0x24, 0xF8, 0x00,
    0x01, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_58[] = {
    0x03, 0x0C, // width=3, height=12
    0x00, 0x10, 0x00,
    0x00, 0x02, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_59[] = {
    0x03, 0x0C, // width=3, height=12
    0x00, 0x10, 0x00,
    0x08, 0x06, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_60[] = {
    0x06, 0x0C, // width=6, height=12
    0x00, 0x40, 0xA0, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_61[] = {
    0x05, 0x0C, // width=5, height=12
    0xA0, 0xA0, 0xA0, 0xA0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_62[] = {
    0x06, 0x0C, // width=6, height=12
    0x00, 0x10, 0xA0, 0x40, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_63[] = {
    0x06, 0x0C, // width=6, height=12
    0x00, 0x08, 0xC4, 0x24, 0x18, 0x00,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_64[] = {
    0x0B, 0x0C, // width=11, height=12
    0xE0, 0x10, 0xC8, 0x24, 0x14, 0x94, 0x64, 0x14, 0x88, 0x70, 0x00,
    0x03, 0x04, 0x09, 0x0A, 0x0A, 0x09, 0x0A, 0x02, 0x01, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_65[] = {
    0x07, 0x0C, // width=7, height=12
    0x00, 0xC0, 0xB0, 0x8C, 0xB0, 0xC0, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
};
const uint8_t FontWinFreeSystem14x16_char_66[] = {
    0x07, 0x0C, // width=7, height=12
    0x00, 0xFC, 0x24, 0x24, 0x24, 0xD8, 0x00,
    0x00, 0x03, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_67[] = {
    0x08, 0x0C, // width=8, height=12
    0x00, 0xF8, 0x04, 0x04, 0x04, 0x04, 0x08, 0x00,
    0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_68[] = {
    0x08, 0x0C, // width=8, height=12
    0x00, 0xFC, 0x04, 0x04, 0x04, 0x08, 0xF0, 0x00,
    0x00, 0x03, 0x02, 0x02, 0x02, 0x01, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_69[] = {
    0x07, 0x0C, // width=7, height=12
    0x00, 0xFC, 0x24, 0x24, 0x24, 0x24, 0x00,
    0x00, 0x03, 0x02, 0x02, 0x02, 0x02, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_70[] = {
    0x06, 0x0C, // width=6, height=12
    0x00, 0xFC, 0x24, 0x24, 0x24, 0x04,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_71[] = {
    0x08, 0x0C, // width=8, height=12
    0x00, 0xF8, 0x04, 0x04, 0x04, 0x44, 0xC8, 0x00,
    0x00, 0x01, 0x02, 0x02, 0x02, 0x01, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_72[] = {
    0x08, 0x0C, // width=8, height=12
    0x00, 0xFC, 0x20, 0x20, 0x20, 0x20, 0xFC, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_73[] = {
    0x03, 0x0C, // width=3, height=12
    0x00, 0xFC, 0x00,
    0x00, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_74[] = {
    0x05, 0x0C, // width=5, height=12
    0x00, 0x00, 0x00, 0xFC, 0x00,
    0x01, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_75[] = {
    0x07, 0x0C, // width=7, height=12
    0x00, 0xFC, 0x20, 0x30, 0xC8, 0x04, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_76[] = {
    0x06, 0x0C, // width=6, height=12
    0x00, 0xFC, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x03, 0x02, 0x02, 0x02, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_77[] = {
    0x09, 0x0C, // width=9, height=12
    0x00, 0xFC, 0x18, 0x60, 0x80, 0x60, 0x18, 0xFC, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_78[] = {
    0x08, 0x0C, // width=8, height=12
    0x00, 0xFC, 0x0C, 0x30, 0xC0, 0x00, 0xFC, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_79[] = {
    0x08, 0x0C, // width=8, height=12
    0x00, 0xF8, 0x04, 0x04, 0x04, 0x04, 0xF8, 0x00,
    0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_80[] = {
    0x07, 0x0C, // width=7, height=12
    0x00, 0xFC, 0x24, 0x24, 0x24, 0x18, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_81[] = {
    0x08, 0x0C, // width=8, height=12
    0x00, 0xF8, 0x04, 0x04, 0x84, 0x04, 0xF8, 0x00,
    0x00, 0x01, 0x02, 0x02, 0x02, 0x03, 0x03, 0x04,
};
const uint8_t FontWinFreeSystem14x16_char_82[] = {
    0x07, 0x0C, // width=7, height=12
    0x00, 0xFC, 0x24, 0x24, 0x24, 0xD8, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_83[] = {
    0x07, 0x0C, // width=7, height=12
    0x00, 0x98, 0x24, 0x24, 0x24, 0xC8, 0x00,
    0x00, 0x01, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_84[] = {
    0x05, 0x0C, // width=5, height=12
    0x04, 0x04, 0xFC, 0x04, 0x04,
    0x00, 0x00, 0x03, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_85[] = {
    0x08, 0x0C, // width=8, height=12
    0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00,
    0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_86[] = {
    0x07, 0x0C, // width=7, height=12
    0x0C, 0x70, 0x80, 0x00, 0x80, 0x70, 0x0C,
    0x00, 0x00, 0x01, 0x02, 0x01, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_87[] = {
    0x09, 0x0C, // width=9, height=12
    0x0C, 0x70, 0x80, 0x40, 0x3C, 0x40, 0x80, 0x70, 0x0C,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_88[] = {
    0x07, 0x0C, // width=7, height=12
    0x00, 0x0C, 0xD0, 0x20, 0xD0, 0x0C, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_89[] = {
    0x07, 0x0C, // width=7, height=12
    0x04, 0x18, 0x60, 0x80, 0x60, 0x18, 0x04,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_90[] = {
    0x07, 0x0C, // width=7, height=12
    0x00, 0x04, 0x84, 0x64, 0x14, 0x0C, 0x00,
    0x00, 0x03, 0x02, 0x02, 0x02, 0x02, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_91[] = {
    0x03, 0x0C, // width=3, height=12
    0x00, 0xFC, 0x04,
    0x00, 0x0F, 0x08,
};
const uint8_t FontWinFreeSystem14x16_char_92[] = {
    0x03, 0x0C, // width=3, height=12
    0x0C, 0xF0, 0x00,
    0x00, 0x00, 0x03,
};
const uint8_t FontWinFreeSystem14x16_char_93[] = {
    0x03, 0x0C, // width=3, height=12
    0x04, 0xFC, 0x00,
    0x08, 0x0F, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_94[] = {
    0x06, 0x0C, // width=6, height=12
    0x40, 0x30, 0x0C, 0x30, 0x40, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_95[] = {
    0x06, 0x0C, // width=6, height=12
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
};
const uint8_t FontWinFreeSystem14x16_char_96[] = {
    0x03, 0x0C, // width=3, height=12
    0x18, 0x04, 0x00,
    0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_97[] = {
    0x05, 0x0C, // width=5, height=12
    0x90, 0x50, 0x50, 0xE0, 0x00,
    0x01, 0x02, 0x02, 0x01, 0x02,
};
const uint8_t FontWinFreeSystem14x16_char_98[] = {
    0x06, 0x0C, // width=6, height=12
    0xFC, 0x20, 0x10, 0x10, 0xE0, 0x00,
    0x03, 0x01, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_99[] = {
    0x05, 0x0C, // width=5, height=12
    0xE0, 0x10, 0x10, 0x20, 0x00,
    0x01, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_100[] = {
    0x06, 0x0C, // width=6, height=12
    0xE0, 0x10, 0x10, 0x20, 0xFC, 0x00,
    0x01, 0x02, 0x02, 0x01, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_101[] = {
    0x05, 0x0C, // width=5, height=12
    0xE0, 0x50, 0x50, 0x60, 0x00,
    0x01, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_102[] = {
    0x04, 0x0C, // width=4, height=12
    0x10, 0xF8, 0x14, 0x04,
    0x00, 0x03, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_103[] = {
    0x06, 0x0C, // width=6, height=12
    0xE0, 0x10, 0x10, 0x20, 0xF0, 0x00,
    0x01, 0x0A, 0x0A, 0x09, 0x07, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_104[] = {
    0x06, 0x0C, // width=6, height=12
    0xFC, 0x20, 0x10, 0x10, 0xE0, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_105[] = {
    0x02, 0x0C, // width=2, height=12
    0xF4, 0x00,
    0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_106[] = {
    0x02, 0x0C, // width=2, height=12
    0xF4, 0x00,
    0x07, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_107[] = {
    0x05, 0x0C, // width=5, height=12
    0xFC, 0x40, 0xA0, 0x10, 0x00,
    0x03, 0x00, 0x00, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_108[] = {
    0x02, 0x0C, // width=2, height=12
    0xFC, 0x00,
    0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_109[] = {
    0x08, 0x0C, // width=8, height=12
    0xF0, 0x10, 0x10, 0xE0, 0x10, 0x10, 0xE0, 0x00,
    0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_110[] = {
    0x06, 0x0C, // width=6, height=12
    0xF0, 0x20, 0x10, 0x10, 0xE0, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_111[] = {
    0x06, 0x0C, // width=6, height=12
    0xE0, 0x10, 0x10, 0x10, 0xE0, 0x00,
    0x01, 0x02, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_112[] = {
    0x06, 0x0C, // width=6, height=12
    0xF0, 0x20, 0x10, 0x10, 0xE0, 0x00,
    0x0F, 0x01, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_113[] = {
    0x06, 0x0C, // width=6, height=12
    0xE0, 0x10, 0x10, 0x20, 0xF0, 0x00,
    0x01, 0x02, 0x02, 0x01, 0x0F, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_114[] = {
    0x04, 0x0C, // width=4, height=12
    0xF0, 0x20, 0x10, 0x00,
    0x03, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_115[] = {
    0x05, 0x0C, // width=5, height=12
    0x20, 0x50, 0x50, 0xA0, 0x00,
    0x01, 0x02, 0x02, 0x01, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_116[] = {
    0x04, 0x0C, // width=4, height=12
    0x10, 0xFC, 0x10, 0x00,
    0x00, 0x03, 0x02, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_117[] = {
    0x05, 0x0C, // width=5, height=12
    0xF0, 0x00, 0x00, 0xF0, 0x00,
    0x01, 0x02, 0x02, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_118[] = {
    0x06, 0x0C, // width=6, height=12
    0x30, 0xC0, 0x00, 0xC0, 0x30, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_119[] = {
    0x08, 0x0C, // width=8, height=12
    0x30, 0xC0, 0x00, 0xF0, 0x00, 0xC0, 0x30, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_120[] = {
    0x06, 0x0C, // width=6, height=12
    0x10, 0xA0, 0x40, 0xA0, 0x10, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x03, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_121[] = {
    0x05, 0x0C, // width=5, height=12
    0xF0, 0x00, 0xC0, 0x30, 0x00,
    0x08, 0x07, 0x01, 0x00, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_122[] = {
    0x05, 0x0C, // width=5, height=12
    0x10, 0x90, 0x50, 0x30, 0x00,
    0x03, 0x02, 0x02, 0x02, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_123[] = {
    0x03, 0x0C, // width=3, height=12
    0x40, 0xB8, 0x04,
    0x00, 0x07, 0x08,
};
const uint8_t FontWinFreeSystem14x16_char_124[] = {
    0x03, 0x0C, // width=3, height=12
    0x00, 0xFC, 0x00,
    0x00, 0x0F, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_125[] = {
    0x03, 0x0C, // width=3, height=12
    0x04, 0xB8, 0x40,
    0x08, 0x07, 0x00,
};
const uint8_t FontWinFreeSystem14x16_char_126[] = {
    0x07, 0x0C, // width=7, height=12
    0x40, 0x20, 0x20, 0x40, 0x40, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
// const uint8_t FontWinFreeSystem14x16_char_127[] = {};
// const uint8_t FontWinFreeSystem14x16_char_128[] = {};
// const uint8_t FontWinFreeSystem14x16_char_129[] = {};
// const uint8_t FontWinFreeSystem14x16_char_130[] = {};
// const uint8_t FontWinFreeSystem14x16_char_131[] = {};
// const uint8_t FontWinFreeSystem14x16_char_132[] = {};
// const uint8_t FontWinFreeSystem14x16_char_133[] = {};
// const uint8_t FontWinFreeSystem14x16_char_134[] = {};
// const uint8_t FontWinFreeSystem14x16_char_135[] = {};
// const uint8_t FontWinFreeSystem14x16_char_136[] = {};
// const uint8_t FontWinFreeSystem14x16_char_137[] = {};
// const uint8_t FontWinFreeSystem14x16_char_138[] = {};
// const uint8_t FontWinFreeSystem14x16_char_139[] = {};
// const uint8_t FontWinFreeSystem14x16_char_140[] = {};
// const uint8_t FontWinFreeSystem14x16_char_141[] = {};
// const uint8_t FontWinFreeSystem14x16_char_142[] = {};
// const uint8_t FontWinFreeSystem14x16_char_143[] = {};
// const uint8_t FontWinFreeSystem14x16_char_144[] = {};
// const uint8_t FontWinFreeSystem14x16_char_145[] = {};
// const uint8_t FontWinFreeSystem14x16_char_146[] = {};
// const uint8_t FontWinFreeSystem14x16_char_147[] = {};
// const uint8_t FontWinFreeSystem14x16_char_148[] = {};
// const uint8_t FontWinFreeSystem14x16_char_149[] = {};
// const uint8_t FontWinFreeSystem14x16_char_150[] = {};
// const uint8_t FontWinFreeSystem14x16_char_151[] = {};
// const uint8_t FontWinFreeSystem14x16_char_152[] = {};
// const uint8_t FontWinFreeSystem14x16_char_153[] = {};
// const uint8_t FontWinFreeSystem14x16_char_154[] = {};
// const uint8_t FontWinFreeSystem14x16_char_155[] = {};
// const uint8_t FontWinFreeSystem14x16_char_156[] = {};
// const uint8_t FontWinFreeSystem14x16_char_157[] = {};
// const uint8_t FontWinFreeSystem14x16_char_158[] = {};
// const uint8_t FontWinFreeSystem14x16_char_159[] = {};
// const uint8_t FontWinFreeSystem14x16_char_160[] = {};
// const uint8_t FontWinFreeSystem14x16_char_161[] = {};
// const uint8_t FontWinFreeSystem14x16_char_162[] = {};
// const uint8_t FontWinFreeSystem14x16_char_163[] = {};
// const uint8_t FontWinFreeSystem14x16_char_164[] = {};
// const uint8_t FontWinFreeSystem14x16_char_165[] = {};
// const uint8_t FontWinFreeSystem14x16_char_166[] = {};
// const uint8_t FontWinFreeSystem14x16_char_167[] = {};
// const uint8_t FontWinFreeSystem14x16_char_168[] = {};
// const uint8_t FontWinFreeSystem14x16_char_169[] = {};
// const uint8_t FontWinFreeSystem14x16_char_170[] = {};
// const uint8_t FontWinFreeSystem14x16_char_171[] = {};
// const uint8_t FontWinFreeSystem14x16_char_172[] = {};
// const uint8_t FontWinFreeSystem14x16_char_173[] = {};
// const uint8_t FontWinFreeSystem14x16_char_174[] = {};
// const uint8_t FontWinFreeSystem14x16_char_175[] = {};
// const uint8_t FontWinFreeSystem14x16_char_176[] = {};
// const uint8_t FontWinFreeSystem14x16_char_177[] = {};
// const uint8_t FontWinFreeSystem14x16_char_178[] = {};
// const uint8_t FontWinFreeSystem14x16_char_179[] = {};
// const uint8_t FontWinFreeSystem14x16_char_180[] = {};
// const uint8_t FontWinFreeSystem14x16_char_181[] = {};
// const uint8_t FontWinFreeSystem14x16_char_182[] = {};
// const uint8_t FontWinFreeSystem14x16_char_183[] = {};
// const uint8_t FontWinFreeSystem14x16_char_184[] = {};
// const uint8_t FontWinFreeSystem14x16_char_185[] = {};
// const uint8_t FontWinFreeSystem14x16_char_186[] = {};
// const uint8_t FontWinFreeSystem14x16_char_187[] = {};
// const uint8_t FontWinFreeSystem14x16_char_188[] = {};
// const uint8_t FontWinFreeSystem14x16_char_189[] = {};
// const uint8_t FontWinFreeSystem14x16_char_190[] = {};
// const uint8_t FontWinFreeSystem14x16_char_191[] = {};
// const uint8_t FontWinFreeSystem14x16_char_192[] = {};
// const uint8_t FontWinFreeSystem14x16_char_193[] = {};
// const uint8_t FontWinFreeSystem14x16_char_194[] = {};
// const uint8_t FontWinFreeSystem14x16_char_195[] = {};
// const uint8_t FontWinFreeSystem14x16_char_196[] = {};
// const uint8_t FontWinFreeSystem14x16_char_197[] = {};
// const uint8_t FontWinFreeSystem14x16_char_198[] = {};
// const uint8_t FontWinFreeSystem14x16_char_199[] = {};
// const uint8_t FontWinFreeSystem14x16_char_200[] = {};
// const uint8_t FontWinFreeSystem14x16_char_201[] = {};
// const uint8_t FontWinFreeSystem14x16_char_202[] = {};
// const uint8_t FontWinFreeSystem14x16_char_203[] = {};
// const uint8_t FontWinFreeSystem14x16_char_204[] = {};
// const uint8_t FontWinFreeSystem14x16_char_205[] = {};
// const uint8_t FontWinFreeSystem14x16_char_206[] = {};
// const uint8_t FontWinFreeSystem14x16_char_207[] = {};
// const uint8_t FontWinFreeSystem14x16_char_208[] = {};
// const uint8_t FontWinFreeSystem14x16_char_209[] = {};
// const uint8_t FontWinFreeSystem14x16_char_210[] = {};
// const uint8_t FontWinFreeSystem14x16_char_211[] = {};
// const uint8_t FontWinFreeSystem14x16_char_212[] = {};
// const uint8_t FontWinFreeSystem14x16_char_213[] = {};
// const uint8_t FontWinFreeSystem14x16_char_214[] = {};
// const uint8_t FontWinFreeSystem14x16_char_215[] = {};
// const uint8_t FontWinFreeSystem14x16_char_216[] = {};
// const uint8_t FontWinFreeSystem14x16_char_217[] = {};
// const uint8_t FontWinFreeSystem14x16_char_218[] = {};
// const uint8_t FontWinFreeSystem14x16_char_219[] = {};
// const uint8_t FontWinFreeSystem14x16_char_220[] = {};
// const uint8_t FontWinFreeSystem14x16_char_221[] = {};
// const uint8_t FontWinFreeSystem14x16_char_222[] = {};
// const uint8_t FontWinFreeSystem14x16_char_223[] = {};
// const uint8_t FontWinFreeSystem14x16_char_224[] = {};
// const uint8_t FontWinFreeSystem14x16_char_225[] = {};
// const uint8_t FontWinFreeSystem14x16_char_226[] = {};
// const uint8_t FontWinFreeSystem14x16_char_227[] = {};
// const uint8_t FontWinFreeSystem14x16_char_228[] = {};
// const uint8_t FontWinFreeSystem14x16_char_229[] = {};
// const uint8_t FontWinFreeSystem14x16_char_230[] = {};
// const uint8_t FontWinFreeSystem14x16_char_231[] = {};
// const uint8_t FontWinFreeSystem14x16_char_232[] = {};
// const uint8_t FontWinFreeSystem14x16_char_233[] = {};
// const uint8_t FontWinFreeSystem14x16_char_234[] = {};
// const uint8_t FontWinFreeSystem14x16_char_235[] = {};
// const uint8_t FontWinFreeSystem14x16_char_236[] = {};
// const uint8_t FontWinFreeSystem14x16_char_237[] = {};
// const uint8_t FontWinFreeSystem14x16_char_238[] = {};
// const uint8_t FontWinFreeSystem14x16_char_239[] = {};
// const uint8_t FontWinFreeSystem14x16_char_240[] = {};
// const uint8_t FontWinFreeSystem14x16_char_241[] = {};
// const uint8_t FontWinFreeSystem14x16_char_242[] = {};
// const uint8_t FontWinFreeSystem14x16_char_243[] = {};
// const uint8_t FontWinFreeSystem14x16_char_244[] = {};
// const uint8_t FontWinFreeSystem14x16_char_245[] = {};
// const uint8_t FontWinFreeSystem14x16_char_246[] = {};
// const uint8_t FontWinFreeSystem14x16_char_247[] = {};
// const uint8_t FontWinFreeSystem14x16_char_248[] = {};
// const uint8_t FontWinFreeSystem14x16_char_249[] = {};
// const uint8_t FontWinFreeSystem14x16_char_250[] = {};
// const uint8_t FontWinFreeSystem14x16_char_251[] = {};
// const uint8_t FontWinFreeSystem14x16_char_252[] = {};
// const uint8_t FontWinFreeSystem14x16_char_253[] = {};
// const uint8_t FontWinFreeSystem14x16_char_254[] = {};
// const uint8_t FontWinFreeSystem14x16_char_255[] = {};

const uint8_t * const FontWinFreeSystem14x16_table[256] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    FontWinFreeSystem14x16_char_32,
    FontWinFreeSystem14x16_char_33,
    FontWinFreeSystem14x16_char_34,
    FontWinFreeSystem14x16_char_35,
    FontWinFreeSystem14x16_char_36,
    FontWinFreeSystem14x16_char_37,
    FontWinFreeSystem14x16_char_38,
    FontWinFreeSystem14x16_char_39,
    FontWinFreeSystem14x16_char_40,
    FontWinFreeSystem14x16_char_41,
    FontWinFreeSystem14x16_char_42,
    FontWinFreeSystem14x16_char_43,
    FontWinFreeSystem14x16_char_44,
    FontWinFreeSystem14x16_char_45,
    FontWinFreeSystem14x16_char_46,
    FontWinFreeSystem14x16_char_47,
    FontWinFreeSystem14x16_char_48,
    FontWinFreeSystem14x16_char_49,
    FontWinFreeSystem14x16_char_50,
    FontWinFreeSystem14x16_char_51,
    FontWinFreeSystem14x16_char_52,
    FontWinFreeSystem14x16_char_53,
    FontWinFreeSystem14x16_char_54,
    FontWinFreeSystem14x16_char_55,
    FontWinFreeSystem14x16_char_56,
    FontWinFreeSystem14x16_char_57,
    FontWinFreeSystem14x16_char_58,
    FontWinFreeSystem14x16_char_59,
    FontWinFreeSystem14x16_char_60,
    FontWinFreeSystem14x16_char_61,
    FontWinFreeSystem14x16_char_62,
    FontWinFreeSystem14x16_char_63,
    FontWinFreeSystem14x16_char_64,
    FontWinFreeSystem14x16_char_65,
    FontWinFreeSystem14x16_char_66,
    FontWinFreeSystem14x16_char_67,
    FontWinFreeSystem14x16_char_68,
    FontWinFreeSystem14x16_char_69,
    FontWinFreeSystem14x16_char_70,
    FontWinFreeSystem14x16_char_71,
    FontWinFreeSystem14x16_char_72,
    FontWinFreeSystem14x16_char_73,
    FontWinFreeSystem14x16_char_74,
    FontWinFreeSystem14x16_char_75,
    FontWinFreeSystem14x16_char_76,
    FontWinFreeSystem14x16_char_77,
    FontWinFreeSystem14x16_char_78,
    FontWinFreeSystem14x16_char_79,
    FontWinFreeSystem14x16_char_80,
    FontWinFreeSystem14x16_char_81,
    FontWinFreeSystem14x16_char_82,
    FontWinFreeSystem14x16_char_83,
    FontWinFreeSystem14x16_char_84,
    FontWinFreeSystem14x16_char_85,
    FontWinFreeSystem14x16_char_86,
    FontWinFreeSystem14x16_char_87,
    FontWinFreeSystem14x16_char_88,
    FontWinFreeSystem14x16_char_89,
    FontWinFreeSystem14x16_char_90,
    FontWinFreeSystem14x16_char_91,
    FontWinFreeSystem14x16_char_92,
    FontWinFreeSystem14x16_char_93,
    FontWinFreeSystem14x16_char_94,
    FontWinFreeSystem14x16_char_95,
    FontWinFreeSystem14x16_char_96,
    FontWinFreeSystem14x16_char_97,
    FontWinFreeSystem14x16_char_98,
    FontWinFreeSystem14x16_char_99,
    FontWinFreeSystem14x16_char_100,
    FontWinFreeSystem14x16_char_101,
    FontWinFreeSystem14x16_char_102,
    FontWinFreeSystem14x16_char_103,
    FontWinFreeSystem14x16_char_104,
    FontWinFreeSystem14x16_char_105,
    FontWinFreeSystem14x16_char_106,
    FontWinFreeSystem14x16_char_107,
    FontWinFreeSystem14x16_char_108,
    FontWinFreeSystem14x16_char_109,
    FontWinFreeSystem14x16_char_110,
    FontWinFreeSystem14x16_char_111,
    FontWinFreeSystem14x16_char_112,
    FontWinFreeSystem14x16_char_113,
    FontWinFreeSystem14x16_char_114,
    FontWinFreeSystem14x16_char_115,
    FontWinFreeSystem14x16_char_116,
    FontWinFreeSystem14x16_char_117,
    FontWinFreeSystem14x16_char_118,
    FontWinFreeSystem14x16_char_119,
    FontWinFreeSystem14x16_char_120,
    FontWinFreeSystem14x16_char_121,
    FontWinFreeSystem14x16_char_122,
    FontWinFreeSystem14x16_char_123,
    FontWinFreeSystem14x16_char_124,
    FontWinFreeSystem14x16_char_125,
    FontWinFreeSystem14x16_char_126,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
