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
* File Name : OKAYA_LCD.c
* Version : 1.00
* Device(s) : OKAYA LCD Controller
* Tool-Chain : Glyph - The Generic API for Graphics LCD API version 1.00
* H/W Platform : RX62N Group
* Description : A collection of functions used to operate an OKAYA 96x64
*               LCD display with the ST7579 controller chip.
*               (Okaya Graphic LCD P/N  SE9664WRF-004-I02Q)
* Communications Required: 3-Wire SPI with a 4th Wire independent
*                          Transmit Only
******************************************************************************
* History :
* : 21.07.2010 1.00 First Release
******************************************************************************/
#include <stdint.h>

/******************************************************************************
Includes “OKAYA Includes”
******************************************************************************/
#include "OKAYA_LCD.h"

/******************************************************************************
* ID : 30.0
* Outline : OKAYA_Open
* Include : OKAYA_LCD.h
* Function Name: OKAYA_Open
* Description : Assign values to the handle variables and configure LCD.
* Argument : aHandle - the Glyph handle to setup for the LCD.
*          : aAddress - may be used for some communications.
* Return Value : 0=success, not 0= error
* Calling Functions : GlyphOpen
******************************************************************************/
T_glyphError OKAYA_Open(T_glyphHandle aHandle, uint32_t aAddress)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;

    p_gw->iLCDAPI->iAddress = aAddress;

    p_gw->iLCDAPI->iFont = USE_DEFAULT_FONT ;

    /* FP_STATUS */
    p_gw->iLCDAPI->iStatusRegister = 0x01 ;

    p_gw->iLCDAPI->iFP_API_MAJOR_VERSION = 0x01 ;
    p_gw->iLCDAPI->iFP_API_MINOR_VERSION = 0x00 ;
    p_gw->iLCDAPI->iFP_IMPLEMENTATION_ID = 0x01 ;
    p_gw->iLCDAPI->iFP_IMPL_MAJOR_VERSION = VERSION_MAJOR ;
    p_gw->iLCDAPI->iFP_IMPL_MINOR_VERSION = VERSION_MINOR ;
    p_gw->iLCDAPI->iMAX_HEIGHT = 64 ;
    p_gw->iLCDAPI->iMAX_WIDTH = 96 ;

    /* Set Default Contrast */
    p_gw->iLCDAPI->iContrast = 95 ;
    p_gw->iLCDAPI->iContrastBoost = 5 ;
    p_gw->iLCDAPI->iFrameRate = 137 ;

    /* Turn on the LCD commands */
    OKAYA_Config(aHandle) ;

    return GLYPH_ERROR_NONE ;
}

/******************************************************************************
* ID : 30.2
* Outline : OKAYA_Close
* Include : OKAYA_LCD.h
* Function Name: OKAYA_Close
* Description : Clear all settings in the global OKAYA_LCD.
* Argument : aHandle - the Glyph handle to setup for the LCD and Communications.
* Return Value : 0=success, not 0= error
* Calling Functions : GlyphOpen
******************************************************************************/
T_glyphError OKAYA_Close(T_glyphHandle aHandle)
{
    return GLYPH_ERROR_NONE ;
}

/******************************************************************************
* ID : 31.0
* Outline : OKAYA_Write
* Include : OKAYA_LCD.h
* Function Name: OKAYA_Write
* Description : Write to the LCD using a Glyph Register value, to the Glyph
* data storage or run an LCD Glyph Command.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aRegister - specifies what to do.
*          : aValue - data if required
* Return Value : 0=success, not 0= error
* Calling Functions : GlyphWrite
******************************************************************************/
T_glyphError OKAYA_Write(T_glyphHandle aHandle, uint32_t aRegister, uint32_t aValue)
{
    uint32_t pCounter;
    uint32_t column;
    uint32_t line;
    const uint8_t *p_char;
    uint8_t width;
    uint8_t height;
    const uint8_t *p_charData;
    uint32_t page;
    T_glyphError error = GLYPH_ERROR_ILLEGAL_OPERATION ;
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;

    p_gw->iLCDAPI->iStatusRegister = GLYPH_STATUS_BUSY ;

    switch (aRegister) {
        case GLYPH_CHAR_X:
            p_gw->iLCDAPI->iCharX_Position = aValue ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_CHAR_Y:
            p_gw->iLCDAPI->iCharY_Position = aValue ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_FONT:
            p_gw->iLCDAPI->iCharFont = aValue;
            switch (aValue) {
            #ifdef USE_GLYPH_FONT_BITMAP
                case GLYPH_FONT_BITMAP:
                    p_gw->iLCDAPI->iFont = Bitmaps_table ;
                    break ;
            #endif
            #ifdef USE_GLYPH_FONT_HELVR10
                case GLYPH_FONT_HELVR10:
                    p_gw->iLCDAPI->iFont = FontHelvr10_table ;
                    break ;
            #endif
            #ifdef USE_GLYPH_FONT_8_BY_16
                case GLYPH_FONT_8_BY_16:
                    p_gw->iLCDAPI->iFont = Font8x16_table ;
                    break ;
            #endif
            #ifdef USE_GLYPH_FONT_8_BY_8
                case GLYPH_FONT_8_BY_8:
                    p_gw->iLCDAPI->iFont = Font8x8_table ;
                    break ;
            #endif
            #ifdef USE_GLYPH_FONT_WINFREE
                case GLYPH_FONT_WINFREESYSTEM14_BY_16:
                    p_gw->iLCDAPI->iFont = FontWinFreeSystem14x16_table ;
                    break ;
            #endif
            #ifdef USE_GLYPH_FONT_5_BY_7
                case GLYPH_FONT_5_BY_7:
                    p_gw->iLCDAPI->iFont = Fontx5x7_table ;
                    break ;
            #endif
            #ifdef USE_GLYPH_FONT_6_BY_13
                case GLYPH_FONT_6_BY_13:
                    p_gw->iLCDAPI->iFont = Fontx6x13_table ;
                    break ;
            #endif
            }
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_DRAW_CMD:
            switch (aValue)  {
                case 1:
                    OKAYA_SetLine(p_gw, 0);
                    for (pCounter=0;pCounter<8;pCounter++)  {
                        OKAYA_SetPage(aHandle, pCounter);
                        OKAYA_SetChar(aHandle, 0);
                        for (column=0; column<128; column++)    {
                            p_gw->iCommAPI->iDataSend(0x00);
                        }
                    }
                    OKAYA_SetPage(aHandle, 0);
                    OKAYA_SetChar(aHandle, 0);
                    break;
                case 2:
                    /* use reverse video to detect pixel changes */
                    OKAYA_Send16bitsCommand(aHandle, LCD_DISPLAY_REVERSE);
                    break;
                case 3:
                    /* use normal video to detect pixel changes */
                    OKAYA_Send16bitsCommand(aHandle, LCD_DISPLAY_NORMAL);
                    break;
                case 6:
                    for (pCounter=0;pCounter<8;pCounter++)  {
                        OKAYA_SetLine(aHandle, 0);
                        OKAYA_SetChar(aHandle, 0);
                        OKAYA_SetPage(aHandle, pCounter);
                        for (column=0;column<16;column++)   {
                            p_gw->iCommAPI->iDataSend(0xFF);
                            p_gw->iCommAPI->iDataSend(0x01);
                            p_gw->iCommAPI->iDataSend(0x01);
                            p_gw->iCommAPI->iDataSend(0x01);
                            p_gw->iCommAPI->iDataSend(0x01);
                            p_gw->iCommAPI->iDataSend(0x01);
                            p_gw->iCommAPI->iDataSend(0x01);
                            p_gw->iCommAPI->iDataSend(0x01);
                        }
                    }
                    break;
                case 7:
                    for (line=p_gw->iLCDAPI->iCharY_Position; line<=p_gw->iLCDAPI->iCharY2_Position; line+=8) {
                        OKAYA_SetPage(aHandle, line/8);
                        OKAYA_SetChar(aHandle, p_gw->iLCDAPI->iCharX_Position);
                        for (column=p_gw->iLCDAPI->iCharX_Position; column<=p_gw->iLCDAPI->iCharX2_Position; column++)    {
                            p_gw->iCommAPI->iDataSend(0xFF);
                        }
                    }
                    break;
                case 8:
                    for (line=p_gw->iLCDAPI->iCharY_Position; line<=p_gw->iLCDAPI->iCharY2_Position; line+=8) {
                        OKAYA_SetPage(aHandle, line/8);
                        OKAYA_SetChar(aHandle, p_gw->iLCDAPI->iCharX_Position);
                        for (column=p_gw->iLCDAPI->iCharX_Position; column<=p_gw->iLCDAPI->iCharX2_Position; column++)    {
                            p_gw->iCommAPI->iDataSend(0x00);
                        }
                    }
                    break;
            }
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_DRAW_CHAR:
            p_char = p_gw->iLCDAPI->iFont[aValue];
            width = p_char[0];
            height = p_char[1];
            p_charData = &p_char[2];
            for (page=0; page<height; page+=8)  {
                OKAYA_SetPage(aHandle, (p_gw->iLCDAPI->iCharY_Position+page)>>3);
                OKAYA_SetChar(aHandle,  p_gw->iLCDAPI->iCharX_Position);
                for (column=0; column<width; column++, p_charData++)    {
                    p_gw->iCommAPI->iDataSend(*p_charData);
                }
            }
            p_gw->iLCDAPI->iCharX_Position += width;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_CHAR_ERASE:
            p_char = p_gw->iLCDAPI->iFont[aValue];
            width = p_char[0];
            height = p_char[1];
            for (page=0; page<height; page+=8)  {
                OKAYA_SetPage(aHandle, (p_gw->iLCDAPI->iCharY_Position+page)>>3);
                OKAYA_SetChar(aHandle, p_gw->iLCDAPI->iCharX_Position);
                for (column=0; column<width; column++, p_charData++)    {
                    p_gw->iCommAPI->iDataSend(0x00);
                }
            }
            p_gw->iLCDAPI->iCharX_Position += width;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_INVERT_CHAR:
            p_char = (uint8_t *)p_gw->iLCDAPI->iFont[aValue];
            width = p_char[0];
            height = p_char[1];
            p_charData = &p_char[2];
            for (page=0; page<height; page+=8)  {
                OKAYA_SetPage(aHandle, (p_gw->iLCDAPI->iCharY_Position+page)>>3);
                OKAYA_SetChar(aHandle, p_gw->iLCDAPI->iCharX_Position);
                for (column=0; column<width; column++, p_charData++)    {
                    // Output the inverted values to invert character
                    p_gw->iCommAPI->iDataSend(*p_charData ^ 0xFF);
                }
            }
            p_gw->iLCDAPI->iCharX_Position += width;
            error = GLYPH_ERROR_NONE;
            break;
        case GLYPH_CHAR_X2:
            p_gw->iLCDAPI->iCharX2_Position = aValue ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_CHAR_Y2:
            p_gw->iLCDAPI->iCharY2_Position = aValue ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_FRAME_RATE:
            p_gw->iLCDAPI->iFrameRate = (int32_t)aValue ;
            OKAYA_SetFrameRate(aHandle, p_gw->iLCDAPI->iFrameRate) ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_CONTRAST:
            p_gw->iLCDAPI->iContrast = (int32_t)aValue ;
            OKAYA_SetVO_Range(aHandle, p_gw->iLCDAPI->iContrast) ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_CONTRAST_BOOST:
            p_gw->iLCDAPI->iContrastBoost = (int8_t)aValue ;
            OKAYA_SetSystemBiasBooster(aHandle, p_gw->iLCDAPI->iContrastBoost) ;
            error = GLYPH_ERROR_NONE ;
            break ;
    }

    p_gw->iLCDAPI->iStatusRegister = GLYPH_STATUS_READY ;
    return error ;
}

/******************************************************************************
* ID : 32.0
* Outline : OKAYA_Read
* Include : OKAYA_LCD.h
* Function Name: OKAYA_Read
* Description : Read data from the glyph data storage using a Glyph Register
*               value.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aRegister - specifies what to receive.
*          : aValue - data received.
* Return Value : 0=success, not 0= error
* Calling Functions : GlyphRead
******************************************************************************/
T_glyphError OKAYA_Read(T_glyphHandle aHandle, uint32_t aRegister, uint32_t *aValue)
{
    T_glyphError error = GLYPH_ERROR_ILLEGAL_OPERATION ;
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;

    p_gw->iLCDAPI->iStatusRegister = GLYPH_STATUS_BUSY ;

    switch (aRegister) {
        case GLYPH_STATUS:
            *aValue = GLYPH_STATUS_BUSY ;
            if (p_gw->iLCDAPI->iStatusRegister == 0)
            {
                *aValue = GLYPH_STATUS_READY ;
            }
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_CHAR_X:
            *aValue = p_gw->iLCDAPI->iCharX_Position ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_CHAR_Y:
            *aValue = p_gw->iLCDAPI->iCharY_Position ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_FONT:
            *aValue = p_gw->iLCDAPI->iCharFont ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_API_MAJOR_VERSION:
            *aValue = p_gw->iLCDAPI->iFP_API_MAJOR_VERSION ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_API_MINOR_VERSION:
            *aValue = p_gw->iLCDAPI->iFP_API_MINOR_VERSION ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_IMPLEMENTATION_ID:
            *aValue = p_gw->iLCDAPI->iFP_IMPLEMENTATION_ID ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_IMPL_MAJOR_VERSION:
            *aValue = p_gw->iLCDAPI->iFP_IMPL_MAJOR_VERSION ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_IMPL_MINOR_VERSION:
            *aValue = p_gw->iLCDAPI->iFP_IMPL_MINOR_VERSION ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_CHAR_X2:
            *aValue = p_gw->iLCDAPI->iCharX2_Position ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_CHAR_Y2:
            *aValue = p_gw->iLCDAPI->iCharY2_Position ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_FRAME_RATE:
            *aValue = (uint32_t)p_gw->iLCDAPI->iFrameRate ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_CONTRAST:
            *aValue = (uint32_t)p_gw->iLCDAPI->iContrast ;
            error = GLYPH_ERROR_NONE ;
            break ;
        case GLYPH_CONTRAST_BOOST:
            *aValue = (uint32_t)p_gw->iLCDAPI->iContrastBoost ;
            error = GLYPH_ERROR_NONE ;
            break ;
    }

    p_gw->iLCDAPI->iStatusRegister = GLYPH_STATUS_READY ;
    return error ;
}

/******************************************************************************
* ID : 39.0
* Outline : OKAYA_SetSystemBiasBooster
* Include : OKAYA_LCD.h
* Function Name: OKAYA_SetSystemBiasBooster
* Description : Set the LCD system bias.  Below is a sample Bias chart.
*
* VDD2=2.4V                             Bias
*             1/4     1/5     1/6     1/7       1/8     1/9     1/10    1/11
* Booster X3  3~4.5   3~5.2   3.3~6.3 3.5~6     X       X       X       X
* Booster X4  3~4.5   3~5.7   3.3~7.4 3.5~8.55  3.5~8.1 4~8.1   X       X
* Booster X5  3~4.5   3~5.7   3.3~7.4 3.5~8.8   3.5~9.1 4~9.1 4.2~9.6 4.6~9.6
*
* X: Do not select this setting.
*
* The bias setting will have to be set manually and permanently in this
* function
*
* This function also sets booster efficiency.  The setting of booster
* efficiency is hard programmed here and must be manually changed.
* Efficiency is set at level 2.  The efficiency levels are:
* for booster efficiency level 1: 0x9D, 0x9E, 0x9F
* for booster efficiency level 2: 0x99, 0x9A, 0x9B
* for booster efficiency level 3: 0x95, 0x96, 0x97
* for booster efficiency level 4: 0x91, 0x92, 0x93
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : cValue0To17 - booster set value from 0 to 17 is valid.
* Return Value : none
* Calling Functions : OKAYA_Write
******************************************************************************/
void OKAYA_SetSystemBiasBooster(T_glyphHandle aHandle, int8_t cValue0To17)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;

    if (cValue0To17 <= 17) {
        /* Select bias and boost settings from value */
        char cBias = 0 ;
        char cBoost = 0 ;
        switch (cValue0To17) {
            case 0:
                /* Bias level 1/4 Boost Times 3. */
                cBias = 0x17 ;
                cBoost = 0x99 ;
                break ;
            case 3:
                /* Bias level 1/5 Boost Times 3. */
                cBias = 0x16 ;
                cBoost = 0x99 ;
                break ;
            case 6:
                /* Bias level 1/6 Boost Times 3. */
                cBias = 0x15 ;
                cBoost = 0x99 ;
                break ;
            case 9:
                /* Bias level 1/7 Boost Times 3. */
                cBias = 0x14 ;
                cBoost = 0x99 ;
                break ;
            case 1:
                /* Bias level 1/4 Boost Times 4. */
                cBias = 0x17 ;
                cBoost = 0x9A ;
                break ;
            case 4:
                /* Bias level 1/5 Boost Times 4. */
                cBias = 0x16 ;
                cBoost = 0x9A ;
                break ;
            case 7:
                /* Bias level 1/6 Boost Times 4. */
                cBias = 0x15 ;
                cBoost = 0x9A ;
                break ;
            case 10:
                /* Bias level 1/7 Boost Times 4. */
                cBias = 0x14 ;
                cBoost = 0x9A ;
                break ;
            case 12:
                /* Bias level 1/8 Boost Times 4. */
                cBias = 0x13 ;
                cBoost = 0x9A ;
                break ;
            case 14:
                /* Bias level 1/9 Boost Times 4. */
                cBias = 0x12 ;
                cBoost = 0x9A ;
                break ;
            case 2:
                /* Bias level 1/4 Boost Times 5. */
                cBias = 0x17 ;
                cBoost = 0x9B ;
                break ;
            case 5:
                /* Bias level 1/5 Boost Times 5. */
                cBias = 0x16 ;
                cBoost = 0x9B ;
                break ;
            case 8:
                /* Bias level 1/6 Boost Times 5. */
                cBias = 0x15 ;
                cBoost = 0x9B ;
                break ;
            case 11:
                /* Bias level 1/7 Boost Times 5. */
                cBias = 0x14 ;
                cBoost = 0x9B ;
                break ;
            case 13:
                /* Bias level 1/8 Boost Times 5. */
                cBias = 0x13 ;
                cBoost = 0x9B ;
                break ;
            case 15:
                /* Bias level 1/9 Boost Times 5. */
                cBias = 0x12 ;
                cBoost = 0x9B ;
                break ;
            case 16:
                /* Bias level 1/10 Boost Times 5. */
                cBias = 0x11 ;
                cBoost = 0x9B ;
                break ;
            case 17:
                /* Bias level 1/11 Boost Times 5. */
                cBias = 0x10 ;
                cBoost = 0x9B ;
                break ;
            default:
                /* Bias level 7 Boost Times 3. */
                cBias = 0x14 ;
                cBoost = 0x99 ;
                break ;
        }

        /* Set Bias */
        /* Use Function Set 1 H[1:0]=(0,1) */
        /* Original Development hardcoded this as CommandSend(0x021) */
        p_gw->iCommAPI->iCommandSend(LCD_FUNCTION_ONE) ;
        p_gw->iCommAPI->iCommandSend(cBias) ;

        /* Set Booster */
        /* Use Function Set 3 H[1:0]=(1,1) */
        /* Original Development hardcoded this as CommandSend(0x023) */
        p_gw->iCommAPI->iCommandSend(LCD_FUNCTION_THREE) ;
        /* Set default efficiency and multiplyer on voltage */
        p_gw->iCommAPI->iCommandSend(cBoost) ;
    }
}

/******************************************************************************
* ID : 40.0
* Outline : OKAYA_SetVO_Range
* Include : OKAYA_LCD.h
* Function Name: OKAYA_SetVO_Range
* Description : Set the LCD VO Range.  VO is used as a Contrast Control for the LCD
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : nValue0to254 - contrast set value from 0 to 254 is valid.
* Return Value : none
* Calling Functions : OKAYA_Write
******************************************************************************/
void OKAYA_SetVO_Range(T_glyphHandle aHandle, int32_t nValue0to254)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;

    if (nValue0to254 <= 254) {
        char cByteToSend = 0 ;
        char cLowHigh = 0 ;

        if (nValue0to254 > 127) {
            cByteToSend = ((nValue0to254 - 127) | 0x80) & 0x00FF ;
            cLowHigh = 1 ;
        }
        else {
            cByteToSend = (nValue0to254 | 0x80) & 0x00FF ;
        }

        /* Use Function Set 0 H[1:0]=(0,0) */
        /* Original Development hardcoded this as CommandSend(0x020) */
        p_gw->iCommAPI->iCommandSend(LCD_FUNCTION_ZERO) ;
        /* Set VO range to Low if 0x04 or High if 0x05 */
        /* here low would be PRS=0 and high would be PRS=1 */
        if (cLowHigh == 0) p_gw->iCommAPI->iCommandSend(0x04) ;
        if (cLowHigh == 1) p_gw->iCommAPI->iCommandSend(0x05) ;

        /* Use Function Set 1 H[1:0]=(0,1) */
        /* Original Development hardcoded this as CommandSend(0x021) */
        p_gw->iCommAPI->iCommandSend(LCD_FUNCTION_ONE) ;
        /* Set PRS=0 to 2.94 V or PRS=1 2.95V to 6.75V */
        /* the value here can be 0 to 127 or 0x00 to 0x7F */
        p_gw->iCommAPI->iCommandSend(cByteToSend) ;
    }
}

/******************************************************************************
* ID : 41.0
* Outline : OKAYA_Config
* Include : OKAYA_LCD.h
* Function Name: OKAYA_Config
* Description : Used to configure the LCD with preset settings.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
* Return Value : none
* Calling Functions : OKAYA_Open
******************************************************************************/
void OKAYA_Config(T_glyphHandle aHandle)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;
    int i = 0 ;

    /* Inialize Display */
    OKAYA_SetSystemBiasBooster(aHandle, p_gw->iLCDAPI->iContrastBoost) ;
    OKAYA_SetVO_Range(aHandle, p_gw->iLCDAPI->iContrast) ;
    for (i=0;i<1000;i++){}
    OKAYA_SetFrameRate(aHandle, p_gw->iLCDAPI->iFrameRate) ;

    OKAYA_SetPage(aHandle, 0) ;
    OKAYA_SetChar(aHandle, 0) ;
}

/******************************************************************************
* ID : 42.0
* Outline : OKAYA_Send16bitsCommand
* Include : OKAYA_LCD.h
* Function Name: OKAYA_Send16bitsCommand
* Description : Send 2 commands from the same integer.  For example:
*                       if you wanted to send 0x28 and then 0xD0 just send
*                       0x28D0 using this command.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : nCommand - 2 commands to send.
* Return Value : none
* Calling Functions : none
******************************************************************************/
void OKAYA_Send16bitsCommand(T_glyphHandle aHandle, int32_t nCommand)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;
    int8_t cCommandOne = (int8_t)((nCommand >> 8) & 0x00FF) ;
    int8_t cCommandTwo = (int8_t)(nCommand & 0x00FF) ;

    p_gw->iCommAPI->iCommandSend(cCommandOne) ;
    p_gw->iCommAPI->iCommandSend(cCommandTwo) ;
}

/******************************************************************************
* ID : 43.0
* Outline : OKAYA_Send8bitsData
* Include : OKAYA_LCD.h
* Function Name: OKAYA_Send8bitsData
* Description : Send 8 bits of data to the LCD RAM at the current location.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : cData - 8 bits of data to send.
* Return Value : none
* Calling Functions : none
******************************************************************************/
void OKAYA_Send8bitsData(T_glyphHandle aHandle, int8_t cData)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;

    p_gw->iCommAPI->iDataSend(cData) ;
}

/******************************************************************************
* ID : 44.0
* Outline : OKAYA_SetFrameRate
* Include : OKAYA_LCD.h
* Function Name: OKAYA_SetFrameRate
* Description : Set the Frame Rate to one of:
* 55Hz
* 65Hz
* 68Hz
* 70Hz
* 73Hz
* 76Hz
* 80Hz
* 137Hz
* Currently set at 73 hertz.  Must be manually set and recompliled
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : nRate55To137 - Frame rate to set from 55 to 137 see above
* Return Value : none
* Calling Functions : OKAYA_Config
******************************************************************************/
void OKAYA_SetFrameRate(T_glyphHandle aHandle, int32_t nRate55To137)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;

    /* Use Function Set 3 H[1:0]=(1,1) */
    /* Original Development hardcoded this as CommandSend(0x023) */
    p_gw->iCommAPI->iCommandSend(LCD_FUNCTION_THREE) ;

    switch(nRate55To137) {
        case 55:
            /* Set frame rate of 55 */
            p_gw->iCommAPI->iCommandSend(0x08) ;
            break ;
        case 65:
            /* Set frame rate of 65 */
            p_gw->iCommAPI->iCommandSend(0x09) ;
            break ;
        case 68:
            /* Set frame rate of 68 */
            p_gw->iCommAPI->iCommandSend(0x0A) ;
            break ;
        case 70:
            /* Set frame rate of 70 */
            p_gw->iCommAPI->iCommandSend(0x0B) ;
            break ;
        case 73:
            /* Set frame rate of 73(Default) */
            p_gw->iCommAPI->iCommandSend(0x0C) ;
            break ;
        case 76:
            /* Set frame rate of 76 */
            p_gw->iCommAPI->iCommandSend(0x0D) ;
            break ;
        case 80:
            /* Set frame rate of 80 */
            p_gw->iCommAPI->iCommandSend(0x0E) ;
            break ;
        case 137:
            /* Set frame rate of 137 */
            p_gw->iCommAPI->iCommandSend(0x0F) ;
            break ;
        default:
            /* Set frame rate of 73(Default) */
            p_gw->iCommAPI->iCommandSend(0x0C) ;
    }
}

/******************************************************************************
* ID : 45.0
* Outline : OKAYA_SetPage
* Include : OKAYA_LCD.h
* Function Name: OKAYA_SetPage
* Description : Set the current page to the current set of 8 rows where:
*                   page 0 is row 0 to 7
*                   page 1 is row 8 to 15
*                   page 2 is row 16 to 23
*                   page 3 is row 24 to 31
*                   page 4 is row 32 to 39
*                   page 5 is row 40 to 47
*                   page 6 is row 48 to 55
*                   page 7 is row 56 to 63
*                   page 8 is row 64, 65, 66
*                   page 9 is row 67
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : cValue0To9 - page number to set valid from 0 to 9.
* Return Value : none
* Calling Functions : OKAYA_Write
******************************************************************************/
void OKAYA_SetPage(T_glyphHandle aHandle, int8_t cValue0To9)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;
    char cValueToSend = cValue0To9 | 0x40 ;

    if (cValue0To9 <= 9) {
        cValueToSend &= 0x4F ;

        /* Use Function Set 0 H[1:0]=(0,0) */
        /* Original Development hardcoded this as CommandSend(0x020) */
        p_gw->iCommAPI->iCommandSend(LCD_FUNCTION_ZERO) ;
        /* Set Ram Page of 0 to 9 where 0x40 is 0 and 0x49 is nine */
        p_gw->iCommAPI->iCommandSend(cValueToSend) ;
    }
}

/******************************************************************************
* ID : 46.0
* Outline : OKAYA_SetChar
* Include : OKAYA_LCD.h
* Function Name: OKAYA_SetChar
* Description : Set the current column of the current page that the next batch of
* data will write to.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : cValue0To101 - column number to set valid from 0 to 101.
* Return Value : none
* Calling Functions : OKAYA_Write
******************************************************************************/
void OKAYA_SetChar(T_glyphHandle aHandle, int8_t cValue0To101)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;
    char cValueToSend = cValue0To101 | 0x80 ;

    if (cValue0To101 <= 101) {
        cValueToSend &= 0xFF ;

        /* Use Function Set 0 H[1:0]=(0,0) */
        /* Original Development hardcoded this as CommandSend(0x020) */
        p_gw->iCommAPI->iCommandSend(LCD_FUNCTION_ZERO) ;
        /* Set Ram Page of 0 to 9 where 0x40 is 0 and 0x49 is nine */
        p_gw->iCommAPI->iCommandSend(cValueToSend) ;
    }
}

/******************************************************************************
* ID : 47.0
* Outline : OKAYA_SetLine
* Include : OKAYA_LCD.h
* Function Name: OKAYA_SetLine
* Description : Set the current line to become the top line of the LCD Display.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : cValue0To66 - line number to set valid from 0 to 66.
* Return Value : none
* Calling Functions : OKAYA_Write
******************************************************************************/
void OKAYA_SetLine(T_glyphHandle aHandle, int8_t cValue0To66)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;

    if (cValue0To66 <= 66) {
        int8_t cData[10] = "" ;
        int8_t cValueToSendLow = 0 ;
        int8_t cValueToSendHigh = 0 ;

        cData[0] = 0 ;
        cData[1] = 0 ;
        cData[2] = 0 ;
        cData[3] = 0 ;
        cData[4] = 0 ;
        cData[5] = 0 ;
        cData[6] = 0 ;

        /*  binary 1111 1111 becomes 0000 0001 */
        cData[0] = 0x01 & cValue0To66 ;
        /*  binary 1111 1111 becomes 0000 0010 */
        cData[1] = 0x02 & cValue0To66 ;
        /*  binary 1111 1111 becomes 0000 0100 */
        cData[2] = 0x04 & cValue0To66 ;
        /*  binary 1111 1111 becomes 0000 1000 */
        cData[3] = 0x08 & cValue0To66 ;
        /* binary 1111 1111 becomes 0001 0000 */
        cData[4] = 0x010 & cValue0To66 ;
        /* binary 1111 1111 becomes 0010 0000 */
        cData[5] = 0x020 & cValue0To66 ;
        /* binary 1111 1111 becomes 0100 0000 */
        cData[6] = 0x040 & cValue0To66 ;

        /* binary 10 becomes binary 1 */
        cData[1] = cData[1] >> 1 ;
        /* binary 100 becomes binary 1 */
        cData[2] = cData[2] >> 2 ;
        /* binary 1000 becomes binary 1 */
        cData[3] = cData[3] >> 3 ;
        /* binary 1 0000 becomes binary 1 */
        cData[4] = cData[4] >> 4 ;
        /* binary 10 0000 becomes binary 1 */
        cData[5] = cData[5] >> 5 ;
        /* binary 100 0000 becomes binary 1 */
        cData[6] = cData[6] >> 6 ;

        cValueToSendLow = cData[6] | 0x04 ;
        cValueToSendHigh = cData[0] | cData[1] | cData[2] | cData[3] | cData[4] | cData[5] | 0x040 ;

        /* Use Function Set 1 H[1:0]=(0,1) */
        /* Original Development hardcoded this as CommandSend(0x021) */
        p_gw->iCommAPI->iCommandSend(LCD_FUNCTION_ONE) ;

        /* Set Ram Start Line of 0 to 66 using a high and low value and two command sends */
        p_gw->iCommAPI->iCommandSend(cValueToSendHigh) ;
        p_gw->iCommAPI->iCommandSend(cValueToSendLow) ;
    }
}


