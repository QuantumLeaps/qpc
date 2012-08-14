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
* File Name : GlyphOpenDrivers.c
* Version : 1.00
* Device(s) : Communications Drivers and LCD Display Drivers
* Tool-Chain : Glyph - The Generic API for Graphics RSPI API
* H/W Platform : RX62N Group CPU
* Description : Sets functions in the Handle.
******************************************************************************
* History :
* : 21.07.2010 1.00 First Release
******************************************************************************/

/******************************************************************************
Includes <system include> “Glyph Includes”
******************************************************************************/
#include <stdint.h>
#include "Glyph.h"

/******************************************************************************
Includes “Glyph Communication (0) Include”
******************************************************************************/
#include "YRDKRX62N_RSPI0.h"

/******************************************************************************
Includes “Glyph LCD Display (0) Include”
******************************************************************************/
#include "okaya_lcd.h"

/******************************************************************************
* ID : 58.0
* Outline : GlyphCommOpen
* Include : RX62N_RSPI0.h
* Function Name: GlyphCommOpen
* Description : Assign the communications workspace.
* Argument : aHandle - the Glyph handle to setup for the LCD and Communications.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphCommOpen(T_glyphHandle aHandle, int32_t aAddress)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;


    switch (aAddress) {
        case 0:
             p_gw->iCommAPI->iOpen        = YRDKRX62N_RSPIOpen;
             p_gw->iCommAPI->iCommandSend = YRDKRX62N_CommandSend;
             p_gw->iCommAPI->iDataSend    = YRDKRX62N_DataSend;
             break;


        default:
            return GLYPH_ERROR_ILLEGAL_OPERATION;
    }

    return p_gw->iCommAPI->iOpen(aHandle);
}

/******************************************************************************
* ID : 59.0
* Outline : GlyphLCDOpen
* Include : OKAYA_LCD.h
* Function Name: GlyphLCDOpen
* Description : Assign the global OKAYA_LCD to aHandle.
* Argument : aHandle - the Glyph handle to setup for the LCD.
* Return Value : 0=success, not 0= error
* Calling Functions : GlyphOpen
******************************************************************************/
T_glyphError GlyphLCDOpen(T_glyphHandle aHandle, int32_t aAddress)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;


    switch (aAddress) {
        case 0:
             p_gw->iLCDAPI->iOpen  = OKAYA_Open;
             p_gw->iLCDAPI->iWrite = OKAYA_Write;
             p_gw->iLCDAPI->iRead  = OKAYA_Read;
             p_gw->iLCDAPI->iClose = OKAYA_Close;
             break;


        default:
            return GLYPH_ERROR_ILLEGAL_OPERATION;
    }

    return GLYPH_ERROR_NONE;
}
