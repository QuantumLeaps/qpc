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
/* Code written for Renesas by Future Designs, Inc. www.teamfdi.com */

/******************************************************************************
* File Name : font_x5x7.c
* Version : 1.00
* Tool-Chain : Glyph - The Generic API for Graphics LCD API
* H/W Platform : RX62N Group
* Description : An 5 x 7 Font.
******************************************************************************
* History :
* : 21.07.2010 1.00 First Release
******************************************************************************/

/******************************************************************************
Includes “Glyph config Include”
******************************************************************************/
#include "..\Config.h"

const uint8_t Fontx5x7_char_0[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x3F, 0x3F, 0x3F, 0x00,
};
const uint8_t Fontx5x7_char_1[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x1C, 0x3E, 0x1C, 0x08,
};
const uint8_t Fontx5x7_char_2[] = {
    0x05, 0x07, // width=5, height=7
    0x2A, 0x15, 0x2A, 0x15, 0x00,
};
const uint8_t Fontx5x7_char_3[] = {
    0x05, 0x07, // width=5, height=7
    0x0F, 0x12, 0x7F, 0x10, 0x00,
};
const uint8_t Fontx5x7_char_4[] = {
    0x05, 0x07, // width=5, height=7
    0x0F, 0x05, 0x78, 0x28, 0x00,
};
const uint8_t Fontx5x7_char_5[] = {
    0x05, 0x07, // width=5, height=7
    0x07, 0x7D, 0x28, 0x50, 0x00,
};
const uint8_t Fontx5x7_char_6[] = {
    0x05, 0x07, // width=5, height=7
    0x07, 0x04, 0x78, 0x28, 0x00,
};
const uint8_t Fontx5x7_char_7[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x02, 0x05, 0x02, 0x00,
};
const uint8_t Fontx5x7_char_8[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x12, 0x17, 0x12, 0x00,
};
const uint8_t Fontx5x7_char_9[] = {
    0x05, 0x07, // width=5, height=7
    0x0F, 0x02, 0x74, 0x4F, 0x00,
};
const uint8_t Fontx5x7_char_10[] = {
    0x05, 0x07, // width=5, height=7
    0x07, 0x18, 0x77, 0x10, 0x00,
};
const uint8_t Fontx5x7_char_11[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x08, 0x0F, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_12[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x08, 0x78, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_13[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x00, 0x78, 0x08, 0x08,
};
const uint8_t Fontx5x7_char_14[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x00, 0x0F, 0x08, 0x08,
};
const uint8_t Fontx5x7_char_15[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x08, 0x7F, 0x08, 0x08,
};
const uint8_t Fontx5x7_char_16[] = {
    0x05, 0x07, // width=5, height=7
    0x02, 0x02, 0x02, 0x02, 0x02,
};
const uint8_t Fontx5x7_char_17[] = {
    0x05, 0x07, // width=5, height=7
    0x04, 0x04, 0x04, 0x04, 0x04,
};
const uint8_t Fontx5x7_char_18[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x08, 0x08, 0x08, 0x08,
};
const uint8_t Fontx5x7_char_19[] = {
    0x05, 0x07, // width=5, height=7
    0x10, 0x10, 0x10, 0x10, 0x10,
};
const uint8_t Fontx5x7_char_20[] = {
    0x05, 0x07, // width=5, height=7
    0x20, 0x20, 0x20, 0x20, 0x20,
};
const uint8_t Fontx5x7_char_21[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x00, 0x7F, 0x08, 0x08,
};
const uint8_t Fontx5x7_char_22[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x08, 0x7F, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_23[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x08, 0x0F, 0x08, 0x08,
};
const uint8_t Fontx5x7_char_24[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x08, 0x78, 0x08, 0x08,
};
const uint8_t Fontx5x7_char_25[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x00, 0x7F, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_26[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x24, 0x2A, 0x31, 0x00,
};
const uint8_t Fontx5x7_char_27[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x31, 0x2A, 0x24, 0x00,
};
const uint8_t Fontx5x7_char_28[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x3C, 0x04, 0x3C, 0x00,
};
const uint8_t Fontx5x7_char_29[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x34, 0x1C, 0x16, 0x00,
};
const uint8_t Fontx5x7_char_30[] = {
    0x05, 0x07, // width=5, height=7
    0x28, 0x1C, 0x2A, 0x22, 0x00,
};
const uint8_t Fontx5x7_char_31[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x00, 0x08, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_32[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x00, 0x00, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_33[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x00, 0x2F, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_34[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x07, 0x00, 0x07, 0x00,
};
const uint8_t Fontx5x7_char_35[] = {
    0x05, 0x07, // width=5, height=7
    0x14, 0x3E, 0x14, 0x3E, 0x14,
};
const uint8_t Fontx5x7_char_36[] = {
    0x05, 0x07, // width=5, height=7
    0x04, 0x2A, 0x3E, 0x2A, 0x10,
};
const uint8_t Fontx5x7_char_37[] = {
    0x05, 0x07, // width=5, height=7
    0x13, 0x08, 0x04, 0x32, 0x00,
};
const uint8_t Fontx5x7_char_38[] = {
    0x05, 0x07, // width=5, height=7
    0x14, 0x2A, 0x14, 0x20, 0x00,
};
const uint8_t Fontx5x7_char_39[] = {
    0x05, 0x07, // width=5, height=7
    0x04, 0x03, 0x01, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_40[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x1E, 0x21, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_41[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x21, 0x1E, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_42[] = {
    0x05, 0x07, // width=5, height=7
    0x2A, 0x1C, 0x2A, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_43[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x08, 0x3E, 0x08, 0x08,
};
const uint8_t Fontx5x7_char_44[] = {
    0x05, 0x07, // width=5, height=7
    0x40, 0x30, 0x10, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_45[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x08, 0x08, 0x08, 0x00,
};
const uint8_t Fontx5x7_char_46[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x30, 0x30, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_47[] = {
    0x05, 0x07, // width=5, height=7
    0x10, 0x08, 0x04, 0x02, 0x00,
};
const uint8_t Fontx5x7_char_48[] = {
    0x05, 0x07, // width=5, height=7
    0x1E, 0x21, 0x1E, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_49[] = {
    0x05, 0x07, // width=5, height=7
    0x22, 0x3F, 0x20, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_50[] = {
    0x05, 0x07, // width=5, height=7
    0x22, 0x31, 0x29, 0x26, 0x00,
};
const uint8_t Fontx5x7_char_51[] = {
    0x05, 0x07, // width=5, height=7
    0x11, 0x25, 0x25, 0x1B, 0x00,
};
const uint8_t Fontx5x7_char_52[] = {
    0x05, 0x07, // width=5, height=7
    0x0C, 0x0A, 0x3F, 0x08, 0x00,
};
const uint8_t Fontx5x7_char_53[] = {
    0x05, 0x07, // width=5, height=7
    0x17, 0x25, 0x25, 0x19, 0x00,
};
const uint8_t Fontx5x7_char_54[] = {
    0x05, 0x07, // width=5, height=7
    0x1E, 0x25, 0x25, 0x18, 0x00,
};
const uint8_t Fontx5x7_char_55[] = {
    0x05, 0x07, // width=5, height=7
    0x01, 0x31, 0x0D, 0x03, 0x00,
};
const uint8_t Fontx5x7_char_56[] = {
    0x05, 0x07, // width=5, height=7
    0x1A, 0x25, 0x25, 0x1A, 0x00,
};
const uint8_t Fontx5x7_char_57[] = {
    0x05, 0x07, // width=5, height=7
    0x06, 0x29, 0x29, 0x1E, 0x00,
};
const uint8_t Fontx5x7_char_58[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x36, 0x36, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_59[] = {
    0x05, 0x07, // width=5, height=7
    0x40, 0x36, 0x16, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_60[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x14, 0x22, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_61[] = {
    0x05, 0x07, // width=5, height=7
    0x14, 0x14, 0x14, 0x14, 0x00,
};
const uint8_t Fontx5x7_char_62[] = {
    0x05, 0x07, // width=5, height=7
    0x22, 0x14, 0x08, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_63[] = {
    0x05, 0x07, // width=5, height=7
    0x02, 0x29, 0x06, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_64[] = {
    0x05, 0x07, // width=5, height=7
    0x1E, 0x21, 0x2D, 0x0E, 0x00,
};
const uint8_t Fontx5x7_char_65[] = {
    0x05, 0x07, // width=5, height=7
    0x3E, 0x09, 0x09, 0x3E, 0x00,
};
const uint8_t Fontx5x7_char_66[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x25, 0x25, 0x1A, 0x00,
};
const uint8_t Fontx5x7_char_67[] = {
    0x05, 0x07, // width=5, height=7
    0x1E, 0x21, 0x21, 0x12, 0x00,
};
const uint8_t Fontx5x7_char_68[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x21, 0x21, 0x1E, 0x00,
};
const uint8_t Fontx5x7_char_69[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x25, 0x25, 0x21, 0x00,
};
const uint8_t Fontx5x7_char_70[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x05, 0x05, 0x01, 0x00,
};
const uint8_t Fontx5x7_char_71[] = {
    0x05, 0x07, // width=5, height=7
    0x1E, 0x21, 0x29, 0x3A, 0x00,
};
const uint8_t Fontx5x7_char_72[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x04, 0x04, 0x3F, 0x00,
};
const uint8_t Fontx5x7_char_73[] = {
    0x05, 0x07, // width=5, height=7
    0x21, 0x3F, 0x21, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_74[] = {
    0x05, 0x07, // width=5, height=7
    0x10, 0x20, 0x20, 0x1F, 0x00,
};
const uint8_t Fontx5x7_char_75[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x0C, 0x12, 0x21, 0x00,
};
const uint8_t Fontx5x7_char_76[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x20, 0x20, 0x20, 0x00,
};
const uint8_t Fontx5x7_char_77[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x06, 0x06, 0x3F, 0x00,
};
const uint8_t Fontx5x7_char_78[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x06, 0x18, 0x3F, 0x00,
};
const uint8_t Fontx5x7_char_79[] = {
    0x05, 0x07, // width=5, height=7
    0x1E, 0x21, 0x21, 0x1E, 0x00,
};
const uint8_t Fontx5x7_char_80[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x09, 0x09, 0x06, 0x00,
};
const uint8_t Fontx5x7_char_81[] = {
    0x05, 0x07, // width=5, height=7
    0x1E, 0x31, 0x21, 0x5E, 0x00,
};
const uint8_t Fontx5x7_char_82[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x09, 0x19, 0x26, 0x00,
};
const uint8_t Fontx5x7_char_83[] = {
    0x05, 0x07, // width=5, height=7
    0x12, 0x25, 0x29, 0x12, 0x00,
};
const uint8_t Fontx5x7_char_84[] = {
    0x05, 0x07, // width=5, height=7
    0x01, 0x3F, 0x01, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_85[] = {
    0x05, 0x07, // width=5, height=7
    0x1F, 0x20, 0x20, 0x1F, 0x00,
};
const uint8_t Fontx5x7_char_86[] = {
    0x05, 0x07, // width=5, height=7
    0x0F, 0x30, 0x30, 0x0F, 0x00,
};
const uint8_t Fontx5x7_char_87[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x18, 0x18, 0x3F, 0x00,
};
const uint8_t Fontx5x7_char_88[] = {
    0x05, 0x07, // width=5, height=7
    0x33, 0x0C, 0x0C, 0x33, 0x00,
};
const uint8_t Fontx5x7_char_89[] = {
    0x05, 0x07, // width=5, height=7
    0x07, 0x38, 0x07, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_90[] = {
    0x05, 0x07, // width=5, height=7
    0x31, 0x29, 0x25, 0x23, 0x00,
};
const uint8_t Fontx5x7_char_91[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x21, 0x21, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_92[] = {
    0x05, 0x07, // width=5, height=7
    0x02, 0x04, 0x08, 0x10, 0x00,
};
const uint8_t Fontx5x7_char_93[] = {
    0x05, 0x07, // width=5, height=7
    0x21, 0x21, 0x3F, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_94[] = {
    0x05, 0x07, // width=5, height=7
    0x02, 0x01, 0x02, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_95[] = {
    0x05, 0x07, // width=5, height=7
    0x20, 0x20, 0x20, 0x20, 0x00,
};
const uint8_t Fontx5x7_char_96[] = {
    0x05, 0x07, // width=5, height=7
    0x01, 0x03, 0x04, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_97[] = {
    0x05, 0x07, // width=5, height=7
    0x18, 0x24, 0x14, 0x3C, 0x00,
};
const uint8_t Fontx5x7_char_98[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x24, 0x24, 0x18, 0x00,
};
const uint8_t Fontx5x7_char_99[] = {
    0x05, 0x07, // width=5, height=7
    0x18, 0x24, 0x24, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_100[] = {
    0x05, 0x07, // width=5, height=7
    0x18, 0x24, 0x24, 0x3F, 0x00,
};
const uint8_t Fontx5x7_char_101[] = {
    0x05, 0x07, // width=5, height=7
    0x18, 0x34, 0x2C, 0x08, 0x00,
};
const uint8_t Fontx5x7_char_102[] = {
    0x05, 0x07, // width=5, height=7
    0x08, 0x3E, 0x09, 0x02, 0x00,
};
const uint8_t Fontx5x7_char_103[] = {
    0x05, 0x07, // width=5, height=7
    0x28, 0x54, 0x54, 0x4C, 0x00,
};
const uint8_t Fontx5x7_char_104[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x04, 0x04, 0x38, 0x00,
};
const uint8_t Fontx5x7_char_105[] = {
    0x05, 0x07, // width=5, height=7
    0x24, 0x3D, 0x20, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_106[] = {
    0x05, 0x07, // width=5, height=7
    0x20, 0x40, 0x3D, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_107[] = {
    0x05, 0x07, // width=5, height=7
    0x3F, 0x08, 0x14, 0x20, 0x00,
};
const uint8_t Fontx5x7_char_108[] = {
    0x05, 0x07, // width=5, height=7
    0x21, 0x3F, 0x20, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_109[] = {
    0x05, 0x07, // width=5, height=7
    0x3C, 0x08, 0x0C, 0x38, 0x00,
};
const uint8_t Fontx5x7_char_110[] = {
    0x05, 0x07, // width=5, height=7
    0x3C, 0x04, 0x04, 0x38, 0x00,
};
const uint8_t Fontx5x7_char_111[] = {
    0x05, 0x07, // width=5, height=7
    0x18, 0x24, 0x24, 0x18, 0x00,
};
const uint8_t Fontx5x7_char_112[] = {
    0x05, 0x07, // width=5, height=7
    0x7C, 0x24, 0x24, 0x18, 0x00,
};
const uint8_t Fontx5x7_char_113[] = {
    0x05, 0x07, // width=5, height=7
    0x18, 0x24, 0x24, 0x7C, 0x00,
};
const uint8_t Fontx5x7_char_114[] = {
    0x05, 0x07, // width=5, height=7
    0x3C, 0x04, 0x04, 0x08, 0x00,
};
const uint8_t Fontx5x7_char_115[] = {
    0x05, 0x07, // width=5, height=7
    0x28, 0x2C, 0x34, 0x14, 0x00,
};
const uint8_t Fontx5x7_char_116[] = {
    0x05, 0x07, // width=5, height=7
    0x04, 0x1F, 0x24, 0x20, 0x00,
};
const uint8_t Fontx5x7_char_117[] = {
    0x05, 0x07, // width=5, height=7
    0x1C, 0x20, 0x20, 0x3C, 0x00,
};
const uint8_t Fontx5x7_char_118[] = {
    0x05, 0x07, // width=5, height=7
    0x1C, 0x20, 0x1C, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_119[] = {
    0x05, 0x07, // width=5, height=7
    0x3C, 0x30, 0x30, 0x3C, 0x00,
};
const uint8_t Fontx5x7_char_120[] = {
    0x05, 0x07, // width=5, height=7
    0x24, 0x18, 0x18, 0x24, 0x00,
};
const uint8_t Fontx5x7_char_121[] = {
    0x05, 0x07, // width=5, height=7
    0x0C, 0x50, 0x20, 0x1C, 0x00,
};
const uint8_t Fontx5x7_char_122[] = {
    0x05, 0x07, // width=5, height=7
    0x24, 0x34, 0x2C, 0x24, 0x00,
};
const uint8_t Fontx5x7_char_123[] = {
    0x05, 0x07, // width=5, height=7
    0x04, 0x1E, 0x21, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_124[] = {
    0x05, 0x07, // width=5, height=7
    0x00, 0x3F, 0x00, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_125[] = {
    0x05, 0x07, // width=5, height=7
    0x21, 0x1E, 0x04, 0x00, 0x00,
};
const uint8_t Fontx5x7_char_126[] = {
    0x05, 0x07, // width=5, height=7
    0x02, 0x01, 0x02, 0x01, 0x00,
};
// const uint8_t Fontx5x7_char_127[] = {};
// const uint8_t Fontx5x7_char_128[] = {};
// const uint8_t Fontx5x7_char_129[] = {};
// const uint8_t Fontx5x7_char_130[] = {};
// const uint8_t Fontx5x7_char_131[] = {};
// const uint8_t Fontx5x7_char_132[] = {};
// const uint8_t Fontx5x7_char_133[] = {};
// const uint8_t Fontx5x7_char_134[] = {};
// const uint8_t Fontx5x7_char_135[] = {};
// const uint8_t Fontx5x7_char_136[] = {};
// const uint8_t Fontx5x7_char_137[] = {};
// const uint8_t Fontx5x7_char_138[] = {};
// const uint8_t Fontx5x7_char_139[] = {};
// const uint8_t Fontx5x7_char_140[] = {};
// const uint8_t Fontx5x7_char_141[] = {};
// const uint8_t Fontx5x7_char_142[] = {};
// const uint8_t Fontx5x7_char_143[] = {};
// const uint8_t Fontx5x7_char_144[] = {};
// const uint8_t Fontx5x7_char_145[] = {};
// const uint8_t Fontx5x7_char_146[] = {};
// const uint8_t Fontx5x7_char_147[] = {};
// const uint8_t Fontx5x7_char_148[] = {};
// const uint8_t Fontx5x7_char_149[] = {};
// const uint8_t Fontx5x7_char_150[] = {};
// const uint8_t Fontx5x7_char_151[] = {};
// const uint8_t Fontx5x7_char_152[] = {};
// const uint8_t Fontx5x7_char_153[] = {};
// const uint8_t Fontx5x7_char_154[] = {};
// const uint8_t Fontx5x7_char_155[] = {};
// const uint8_t Fontx5x7_char_156[] = {};
// const uint8_t Fontx5x7_char_157[] = {};
// const uint8_t Fontx5x7_char_158[] = {};
// const uint8_t Fontx5x7_char_159[] = {};
// const uint8_t Fontx5x7_char_160[] = {};
// const uint8_t Fontx5x7_char_161[] = {};
// const uint8_t Fontx5x7_char_162[] = {};
// const uint8_t Fontx5x7_char_163[] = {};
// const uint8_t Fontx5x7_char_164[] = {};
// const uint8_t Fontx5x7_char_165[] = {};
// const uint8_t Fontx5x7_char_166[] = {};
// const uint8_t Fontx5x7_char_167[] = {};
// const uint8_t Fontx5x7_char_168[] = {};
// const uint8_t Fontx5x7_char_169[] = {};
// const uint8_t Fontx5x7_char_170[] = {};
// const uint8_t Fontx5x7_char_171[] = {};
// const uint8_t Fontx5x7_char_172[] = {};
// const uint8_t Fontx5x7_char_173[] = {};
// const uint8_t Fontx5x7_char_174[] = {};
// const uint8_t Fontx5x7_char_175[] = {};
// const uint8_t Fontx5x7_char_176[] = {};
// const uint8_t Fontx5x7_char_177[] = {};
// const uint8_t Fontx5x7_char_178[] = {};
// const uint8_t Fontx5x7_char_179[] = {};
// const uint8_t Fontx5x7_char_180[] = {};
// const uint8_t Fontx5x7_char_181[] = {};
// const uint8_t Fontx5x7_char_182[] = {};
// const uint8_t Fontx5x7_char_183[] = {};
// const uint8_t Fontx5x7_char_184[] = {};
// const uint8_t Fontx5x7_char_185[] = {};
// const uint8_t Fontx5x7_char_186[] = {};
// const uint8_t Fontx5x7_char_187[] = {};
// const uint8_t Fontx5x7_char_188[] = {};
// const uint8_t Fontx5x7_char_189[] = {};
// const uint8_t Fontx5x7_char_190[] = {};
// const uint8_t Fontx5x7_char_191[] = {};
// const uint8_t Fontx5x7_char_192[] = {};
// const uint8_t Fontx5x7_char_193[] = {};
// const uint8_t Fontx5x7_char_194[] = {};
// const uint8_t Fontx5x7_char_195[] = {};
// const uint8_t Fontx5x7_char_196[] = {};
// const uint8_t Fontx5x7_char_197[] = {};
// const uint8_t Fontx5x7_char_198[] = {};
// const uint8_t Fontx5x7_char_199[] = {};
// const uint8_t Fontx5x7_char_200[] = {};
// const uint8_t Fontx5x7_char_201[] = {};
// const uint8_t Fontx5x7_char_202[] = {};
// const uint8_t Fontx5x7_char_203[] = {};
// const uint8_t Fontx5x7_char_204[] = {};
// const uint8_t Fontx5x7_char_205[] = {};
// const uint8_t Fontx5x7_char_206[] = {};
// const uint8_t Fontx5x7_char_207[] = {};
// const uint8_t Fontx5x7_char_208[] = {};
// const uint8_t Fontx5x7_char_209[] = {};
// const uint8_t Fontx5x7_char_210[] = {};
// const uint8_t Fontx5x7_char_211[] = {};
// const uint8_t Fontx5x7_char_212[] = {};
// const uint8_t Fontx5x7_char_213[] = {};
// const uint8_t Fontx5x7_char_214[] = {};
// const uint8_t Fontx5x7_char_215[] = {};
// const uint8_t Fontx5x7_char_216[] = {};
// const uint8_t Fontx5x7_char_217[] = {};
// const uint8_t Fontx5x7_char_218[] = {};
// const uint8_t Fontx5x7_char_219[] = {};
// const uint8_t Fontx5x7_char_220[] = {};
// const uint8_t Fontx5x7_char_221[] = {};
// const uint8_t Fontx5x7_char_222[] = {};
// const uint8_t Fontx5x7_char_223[] = {};
// const uint8_t Fontx5x7_char_224[] = {};
// const uint8_t Fontx5x7_char_225[] = {};
// const uint8_t Fontx5x7_char_226[] = {};
// const uint8_t Fontx5x7_char_227[] = {};
// const uint8_t Fontx5x7_char_228[] = {};
// const uint8_t Fontx5x7_char_229[] = {};
// const uint8_t Fontx5x7_char_230[] = {};
// const uint8_t Fontx5x7_char_231[] = {};
// const uint8_t Fontx5x7_char_232[] = {};
// const uint8_t Fontx5x7_char_233[] = {};
// const uint8_t Fontx5x7_char_234[] = {};
// const uint8_t Fontx5x7_char_235[] = {};
// const uint8_t Fontx5x7_char_236[] = {};
// const uint8_t Fontx5x7_char_237[] = {};
// const uint8_t Fontx5x7_char_238[] = {};
// const uint8_t Fontx5x7_char_239[] = {};
// const uint8_t Fontx5x7_char_240[] = {};
// const uint8_t Fontx5x7_char_241[] = {};
// const uint8_t Fontx5x7_char_242[] = {};
// const uint8_t Fontx5x7_char_243[] = {};
// const uint8_t Fontx5x7_char_244[] = {};
// const uint8_t Fontx5x7_char_245[] = {};
// const uint8_t Fontx5x7_char_246[] = {};
// const uint8_t Fontx5x7_char_247[] = {};
// const uint8_t Fontx5x7_char_248[] = {};
// const uint8_t Fontx5x7_char_249[] = {};
// const uint8_t Fontx5x7_char_250[] = {};
// const uint8_t Fontx5x7_char_251[] = {};
// const uint8_t Fontx5x7_char_252[] = {};
// const uint8_t Fontx5x7_char_253[] = {};
// const uint8_t Fontx5x7_char_254[] = {};
// const uint8_t Fontx5x7_char_255[] = {};

const unsigned char *Fontx5x7_table[256] = {
    Fontx5x7_char_0,
    Fontx5x7_char_1,
    Fontx5x7_char_2,
    Fontx5x7_char_3,
    Fontx5x7_char_4,
    Fontx5x7_char_5,
    Fontx5x7_char_6,
    Fontx5x7_char_7,
    Fontx5x7_char_8,
    Fontx5x7_char_9,
    Fontx5x7_char_10,
    Fontx5x7_char_11,
    Fontx5x7_char_12,
    Fontx5x7_char_13,
    Fontx5x7_char_14,
    Fontx5x7_char_15,
    Fontx5x7_char_16,
    Fontx5x7_char_17,
    Fontx5x7_char_18,
    Fontx5x7_char_19,
    Fontx5x7_char_20,
    Fontx5x7_char_21,
    Fontx5x7_char_22,
    Fontx5x7_char_23,
    Fontx5x7_char_24,
    Fontx5x7_char_25,
    Fontx5x7_char_26,
    Fontx5x7_char_27,
    Fontx5x7_char_28,
    Fontx5x7_char_29,
    Fontx5x7_char_30,
    Fontx5x7_char_31,
    Fontx5x7_char_32,
    Fontx5x7_char_33,
    Fontx5x7_char_34,
    Fontx5x7_char_35,
    Fontx5x7_char_36,
    Fontx5x7_char_37,
    Fontx5x7_char_38,
    Fontx5x7_char_39,
    Fontx5x7_char_40,
    Fontx5x7_char_41,
    Fontx5x7_char_42,
    Fontx5x7_char_43,
    Fontx5x7_char_44,
    Fontx5x7_char_45,
    Fontx5x7_char_46,
    Fontx5x7_char_47,
    Fontx5x7_char_48,
    Fontx5x7_char_49,
    Fontx5x7_char_50,
    Fontx5x7_char_51,
    Fontx5x7_char_52,
    Fontx5x7_char_53,
    Fontx5x7_char_54,
    Fontx5x7_char_55,
    Fontx5x7_char_56,
    Fontx5x7_char_57,
    Fontx5x7_char_58,
    Fontx5x7_char_59,
    Fontx5x7_char_60,
    Fontx5x7_char_61,
    Fontx5x7_char_62,
    Fontx5x7_char_63,
    Fontx5x7_char_64,
    Fontx5x7_char_65,
    Fontx5x7_char_66,
    Fontx5x7_char_67,
    Fontx5x7_char_68,
    Fontx5x7_char_69,
    Fontx5x7_char_70,
    Fontx5x7_char_71,
    Fontx5x7_char_72,
    Fontx5x7_char_73,
    Fontx5x7_char_74,
    Fontx5x7_char_75,
    Fontx5x7_char_76,
    Fontx5x7_char_77,
    Fontx5x7_char_78,
    Fontx5x7_char_79,
    Fontx5x7_char_80,
    Fontx5x7_char_81,
    Fontx5x7_char_82,
    Fontx5x7_char_83,
    Fontx5x7_char_84,
    Fontx5x7_char_85,
    Fontx5x7_char_86,
    Fontx5x7_char_87,
    Fontx5x7_char_88,
    Fontx5x7_char_89,
    Fontx5x7_char_90,
    Fontx5x7_char_91,
    Fontx5x7_char_92,
    Fontx5x7_char_93,
    Fontx5x7_char_94,
    Fontx5x7_char_95,
    Fontx5x7_char_96,
    Fontx5x7_char_97,
    Fontx5x7_char_98,
    Fontx5x7_char_99,
    Fontx5x7_char_100,
    Fontx5x7_char_101,
    Fontx5x7_char_102,
    Fontx5x7_char_103,
    Fontx5x7_char_104,
    Fontx5x7_char_105,
    Fontx5x7_char_106,
    Fontx5x7_char_107,
    Fontx5x7_char_108,
    Fontx5x7_char_109,
    Fontx5x7_char_110,
    Fontx5x7_char_111,
    Fontx5x7_char_112,
    Fontx5x7_char_113,
    Fontx5x7_char_114,
    Fontx5x7_char_115,
    Fontx5x7_char_116,
    Fontx5x7_char_117,
    Fontx5x7_char_118,
    Fontx5x7_char_119,
    Fontx5x7_char_120,
    Fontx5x7_char_121,
    Fontx5x7_char_122,
    Fontx5x7_char_123,
    Fontx5x7_char_124,
    Fontx5x7_char_125,
    Fontx5x7_char_126,
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
