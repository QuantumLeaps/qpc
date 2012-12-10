/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name       : r_glyph_lcd_register.c
* Version      : 1.0
* Description  : This ties the R_GLYPH_XxxXxx functions to the Glyph code.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 01.11.2011 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Glyph code header file. */
#include "Glyph\Glyph.h"
/* Used for LCD. */
#include "Glyph\Drivers\ST7579_LCD.h"
/* Used for communicating with LCD. */
#include "r_glyph.h"

/***********************************************************************************************************************
* Function Name: GlyphCommOpen
* Description  : Assign the communications workspace.
* Arguments    : a_handle -
*                    The Glyph handle to setup for the communications.
*                a_address -
*                    Which LCD to communicate to.
* Return Value : GLYPH_ERROR_NONE -
*                    Success
*                GLYPH_ERROR_ILLEGAL_OPERATION -
*                    Error
* ID           : 58.0
***********************************************************************************************************************/
T_glyphError GlyphCommOpen(T_glyphHandle aHandle, int32_t aAddress)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;

    switch (aAddress) {
        case 0:
            /* Set function pointers for Glyph code. */
            p_gw->iCommAPI->iOpen = R_GLYPH_Open;
            p_gw->iCommAPI->iCommandSend = R_GLYPH_CommandSend ;
            p_gw->iCommAPI->iDataSend = R_GLYPH_DataSend ;
            break ;
        default:
            return GLYPH_ERROR_ILLEGAL_OPERATION ;
    }

    return p_gw->iCommAPI->iOpen(aHandle) ;
}

/***********************************************************************************************************************
* Function Name: GlyphLCDOpen
* Description  : Assign ST7579 LCD to a_handle.
* Arguments    : a_handle -
*                    The Glyph handle to setup for the LCD.
*                a_address -
*                    Which LCD to communicate to.
* Return Value : GLYPH_ERROR_NONE -
*                    Success
*                GLYPH_ERROR_ILLEGAL_OPERATION -
*                    Error
* ID           : 59.0
***********************************************************************************************************************/
T_glyphError GlyphLCDOpen(T_glyphHandle aHandle, int32_t aAddress)
{
    T_glyphWorkspace *p_gw = (T_glyphWorkspace *)aHandle;

    switch (aAddress) {
        case 0:
            /* Assign ST7579 functions. */
            p_gw->iLCDAPI->iOpen = ST7579_Open ;
            p_gw->iLCDAPI->iWrite = ST7579_Write ;
            p_gw->iLCDAPI->iRead = ST7579_Read ;
            p_gw->iLCDAPI->iClose = ST7579_Close ;
            break ;
        default:
            return GLYPH_ERROR_ILLEGAL_OPERATION ;
    }

    return GLYPH_ERROR_NONE ;
}

