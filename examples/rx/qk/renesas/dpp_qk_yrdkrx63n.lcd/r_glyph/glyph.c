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
* File Name : Glyph.c
* Version : 1.00
* Device(s) : Generic
* Tool-Chain : Glyph - The Generic API for Graphics LCD.
* H/W Platform : Embedded Controls
* Description : This is the Glyph API Library definitions.
*               The Generic API for Graphics LCD (nicknamed "Glyph") is a
*               reusable software design to support multiple projects where
*               the LCD size and resolution may change, but the basic feature
*               set will stay consistent.  Glyph is a standard library
*               interface for use with any graphic LCD.
******************************************************************************
* History :
* : 20.07.2010 1.00 First Release
******************************************************************************/

/******************************************************************************
Includes “Glyph Includes”
******************************************************************************/
#include "Config.h"
#include "Glyph.h"

/******************************************************************************
Private Prototypes for the Glyph LCD API
******************************************************************************/
T_glyphError GlyphLCDOpen(T_glyphHandle *aHandle, int32_t aAddress) ;
T_glyphError GlyphCommOpen(T_glyphHandle *aHandle, int32_t aAddress) ;

/******************************************************************************
* ID : 1.0
* Outline : GlyphOpen
* Include : Glyph.h
* Function Name: GlyphOpen
* Description : Open the Glyph API.
* Argument : aHandle - handle passed in and initialized.
*          : aAddress - The numerical definition of the communications
*                       and LCD Display set used for this instance of
*                       the passed in handle.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphOpen(T_glyphHandle *aHandle, int32_t aAddress)
{
    T_glyphError error = GLYPH_ERROR_ILLEGAL_OPERATION ;
    //T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;
    T_glyphWorkspace *p_glyph ;

    p_glyph = (T_glyphWorkspace *)malloc(sizeof(T_glyphWorkspace)) ;
    p_glyph->iCommAPI = (T_Comm_API *)malloc(sizeof(T_Comm_API)) ;
    p_glyph->iLCDAPI = (T_LCD_API *)malloc(sizeof(T_LCD_API)) ;

    *aHandle = (T_glyphHandle)p_glyph ;

    /* Open the communications and the LCD */
    if (GlyphCommOpen(*aHandle, aAddress) == GLYPH_ERROR_NONE) {
        if (GlyphLCDOpen(*aHandle, aAddress) == GLYPH_ERROR_NONE) {
            error = p_glyph->iLCDAPI->iOpen(*aHandle, aAddress) ;
        }
    }

    return error ;
}

/******************************************************************************
* ID : 1.1
* Outline : GlyphClose
* Include : Glyph.h
* Function Name: GlyphClose
* Description : Close the Glyph API.
* Argument : aHandle - handle created and returned.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphClose(T_glyphHandle *aHandle)
{
    T_glyphError error = GLYPH_ERROR_ILLEGAL_OPERATION ;
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)*aHandle;

    GlyphClearScreen(*aHandle) ;

    error = p_glyph->iLCDAPI->iClose(aHandle);
    if (error == GLYPH_ERROR_NONE) {
        free((void *)p_glyph->iLCDAPI) ;
        p_glyph->iLCDAPI = 0 ;
        free((void *)p_glyph->iCommAPI) ;
        p_glyph->iCommAPI = 0 ;
        free((void *)p_glyph) ;
        p_glyph = 0 ;
    }

    *aHandle = (T_glyphHandle)p_glyph ;

    return error ;
}

/******************************************************************************
* ID : 2.0
* Outline : GlyphWrite
* Include : Glyph.h
* Function Name: GlyphWrite
* Description : Write to the LCD using a Glyph Register value, to the Glyph
* data storage or run an LCD Glyph Command.
* Argument : aHandle - handle created and returned.
*          : aAddress - may be used for communications.
* Return Value : 0=success, not 0= error
* Calling Functions : main, GlyphSetX, GlyphSetY, GlyphSetXY,
*                     GlyphSetFont, GlyphSetDrawMode,
*                     GlyphChar, GlyphString, GlyphClearScreen,
*                     GlyphInvertScreen, GlyphNormalScreen, GlyphSleep,
*                     GlyphWake, GlyphDrawTestPattern, GlyphDrawBlock,
*                     GlyphEraseBlock, GlyphSetContrast, GlyphSetContrastBoost
******************************************************************************/
T_glyphError GlyphWrite(T_glyphHandle aHandle, uint32_t aRegister, uint32_t aValue)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    return p_glyph->iLCDAPI->iWrite(aHandle, aRegister, aValue);
}

/******************************************************************************
* ID : 3.0
* Outline : GlyphRead
* Include : Glyph.h
* Function Name: GlyphRead
* Description : Read from the Glyph data storage using a Glyph Register value.
* Argument : aHandle - handle created and returned.
*          : aAddress - may be used for communications.
* Return Value : 0=success, not 0= error
* Calling Functions : main, GlyphGetStatus, GlyphGetX, GlyphGetY, GlyphGetXY,
*                     GlyphGetFont, GlyphGetFont
******************************************************************************/
T_glyphError GlyphRead(T_glyphHandle aHandle, uint32_t aRegister, uint32_t *aValue)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    return p_glyph->iLCDAPI->iRead(aHandle, aRegister, aValue) ;
}

/******************************************************************************
* ID : 3.1
* Outline : GlyphGetStatus
* Include : Glyph.h
* Function Name: GlyphGetStatus
* Description : Returns the status.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aStatus - GLYPH_STATUS_BUSY or GLYPH_STATUS_READY
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphGetStatus(T_glyphHandle aHandle, T_glyphStatusBits *aStatus)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    return p_glyph->iLCDAPI->iRead(aHandle, GLYPH_STATUS, (uint32_t *)aStatus) ;
}

/******************************************************************************
* ID : 4.0
* Outline : GlyphSetX
* Include : Glyph.h
* Function Name: GlyphSetX
* Description : Sets the value of the X position in the given Glyph Handle.
* This position is used as the left position of the next Glyph on the screen.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aX - The given X position.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphSetX(T_glyphHandle aHandle, uint32_t aX)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    return p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_X, aX);
}

/******************************************************************************
* ID : 5.0
* Outline : GlyphSetY
* Include : Glyph.h
* Function Name: GlyphSetY
* Description : Sets the value of the Y position in the given Glyph Handle.
* This position is used as the left position of the next Glyph on the screen.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aY - The given Y position.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphSetY(T_glyphHandle aHandle, uint32_t aY)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    return p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_Y, aY);
}

/******************************************************************************
* ID : 6.0
* Outline : GlyphSetXY
* Include : Glyph.h
* Function Name: GlyphSetXY
* Description : Sets the value of the X and Y positions in the given Glyph
* Handle.  This position is used as the top left position of the next Glyph
* on the screen.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aX - The given X position.
*          : aY - The given Y position.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphSetXY(T_glyphHandle aHandle, uint32_t aX, uint32_t aY)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;
    T_glyphError error ;

    error = p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_X, aX);
    if (error == GLYPH_ERROR_NONE)
    {
        error = p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_Y, aY);
    }

    return error ;
}

/******************************************************************************
* ID : 7.0
* Outline : GlyphGetXY
* Include : Glyph.h
* Function Name: GlyphGetXY
* Description : Gets the value of the X and Y positions in the given Glyph
* Handle. The filled values are the current top left position of the next
* character to be placed onto the screen.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aX - The X position to obtain.
*          : aY - The Y position to obtain.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphGetXY(T_glyphHandle aHandle, uint32_t *aX, uint32_t *aY)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;
    T_glyphError error ;

    error = p_glyph->iLCDAPI->iRead(aHandle, GLYPH_CHAR_X, aX);
    if (error == GLYPH_ERROR_NONE)
    {
        error = p_glyph->iLCDAPI->iRead(aHandle, GLYPH_CHAR_Y, aY);
    }

    return error ;
}

/******************************************************************************
* ID : 8.0
* Outline : GlyphSetFont
* Include : Glyph.h
* Function Name: GlyphSetFont
* Description : Set a selected font value from the font enumeration to be used as the
* next font for the next character placed onto the screen.  This font will
* stay set for every character from now on until you decide to set it again.
*
* The font must have been previously turned on before compile time.  Fonts
* can be turned on and off and then recompiled using the following file:
* GlyphUseFont.h
* which is to be stored inside of your project root directory.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aFont - A value from the Font Enumeration.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphSetFont(T_glyphHandle aHandle, T_glyphFont aFont)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    return p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_FONT, (uint32_t)aFont);
}

/******************************************************************************
* ID : 9.0
* Outline : GlyphGetFont
* Include : Glyph.h
* Function Name: GlyphGetFont
* Description : Retrieve the enumeration value for the currently selected font.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aFont - A value from the Font Enumeration retrieved.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphGetFont(T_glyphHandle aHandle, T_glyphFont *aFont)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    return p_glyph->iLCDAPI->iRead(aHandle, GLYPH_FONT, (uint32_t *)aFont) ;
}

/******************************************************************************
* ID : 10.0
* Outline : GlyphSetDrawMode
* Include : Glyph.h
* Function Name: GlyphSetDrawMode
* Description : The draw mode function. Causes a draw procedure to happen on
 * the LCD display.  The procedure is chosen using the T_glyphDrawMode
 * enumeration in the Glyph.h file.  Operations are described as:
 *   GLYPH_CMD_NOP            -- Does Nothing
 *   GLYPH_CMD_SCREEN_CLEAR   -- Clears the screen to blank no pixels on.
 *   GLYPH_CMD_SCREEN_INVERT  -- Inverts the entire screen.  Black is white.
 *   GLYPH_CMD_SCREEN_REGULAR -- Used on startup to create a normal screen.
 *   GLYPH_CMD_SCREEN_SLEEP   -- Puts the LCD in a low power blank screen.
 *   GLYPH_CMD_SCREEN_WAKE    -- Restores the LCD from low power sleep.
 *   GLYPH_CMD_TEST_PATTERN   -- Displays a test patter.
 *   GLYPH_CMD_DRAW_BLOCK     -- Fills X to X2 and Y to Y2
 *   GLYPH_CMD_ERASE_BLOCK    -- Erases X to X2 and Y to Y2
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aMode - A value from the Draw mode Enumeration.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphSetDrawMode(T_glyphHandle aHandle, T_glyphDrawMode aMode)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    return p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_DRAW_CMD, aMode) ;
}

/******************************************************************************
* ID : 11.0
* Outline : GlyphChar
* Include : Glyph.h
* Function Name: GlyphChar
* Description : Draws the given character in the currently chosen font at
* the current X and Y top left starting pixel on the LCD Screen.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aChar - The given character.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphChar(T_glyphHandle aHandle, uint32_t aChar)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    return p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_DRAW_CHAR, aChar) ;
}

/******************************************************************************
* ID : 12.0
* Outline : GlyphString
* Include : Glyph.h
* Function Name: GlyphString
* Description : Draws the given character string in the currently chosen font at the
* current X and Y top left starting pixel on the LCD Screen.
*
* Each character of the string is set from the start of the last character
* by the number of aSpacing given.  So if the font is say 8 pixels wide,
* a good number to use here might be 10 pixels which would give you
* two pixels between each character in the string.
*
* You must count the number of characters you wish to put to the LCD
* screen and enter that number in the aLength variable.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aString - The given character string.
*          : aLength - The count of characters in the string
*                      you wish to display on the LCD.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphString(T_glyphHandle aHandle, uint8_t * aString, uint32_t aLength)
{
    uint32_t nIndex = 0 ;

    nIndex = 0 ;
    while (nIndex < aLength)
    {
        GlyphChar(aHandle, aString[nIndex]) ;
        nIndex++ ;
    }

    return GLYPH_ERROR_NONE ;
}

/******************************************************************************
* ID : 13.0
* Outline : GlyphGetVersionInfo
* Include : Glyph.h
* Function Name: GlyphGetVersionInfo
* Description : Fills the given T_glyphVersionInfo with version data.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aInfo - The given version info structure to fill.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphGetVersionInfo(T_glyphHandle aHandle, T_glyphVersionInfo *aInfo)
{
    uint32_t nVersionValue = 0 ;
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    if (p_glyph->iLCDAPI->iRead(aHandle, GLYPH_API_MAJOR_VERSION, &nVersionValue) == GLYPH_ERROR_NONE) {
        aInfo->nVersionAPIMajor = nVersionValue ;
        if (p_glyph->iLCDAPI->iRead(aHandle, GLYPH_API_MINOR_VERSION, &nVersionValue) == GLYPH_ERROR_NONE) {
            aInfo->nVersionAPIMinor = nVersionValue ;
            if (p_glyph->iLCDAPI->iRead(aHandle, GLYPH_IMPLEMENTATION_ID, &nVersionValue) == GLYPH_ERROR_NONE) {
                aInfo->nImplementationID = nVersionValue ;
                if (p_glyph->iLCDAPI->iRead(aHandle, GLYPH_IMPL_MAJOR_VERSION, &nVersionValue) == GLYPH_ERROR_NONE) {
                    aInfo->nVersionIMPLMajor = nVersionValue ;
                    if (p_glyph->iLCDAPI->iRead(aHandle, GLYPH_IMPL_MINOR_VERSION, &nVersionValue) == GLYPH_ERROR_NONE) {
                        aInfo->nVersionIMPLMinor = nVersionValue ;
                        sprintf((char *)aInfo->strVersionAPI, "API version %d.0%d",
                                aInfo->nVersionAPIMajor,
                                aInfo->nVersionAPIMinor) ;
                        sprintf((char *)aInfo->strVersionIMPL, "Implementation version %d.%d%d",
                                aInfo->nVersionIMPLMajor,
                                aInfo->nImplementationID,
                                aInfo->nVersionIMPLMinor) ;

                        return GLYPH_ERROR_NONE ;
                    }
                }
            }
        }
    }

    return GLYPH_ERROR_ILLEGAL_OPERATION ;
}

/******************************************************************************
* ID : 14.0
* Outline : GlyphClearScreen
* Include : Glyph.h
* Function Name: GlyphClearScreen
* Description : Clears the LCD Screen to white.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphClearScreen(T_glyphHandle aHandle)
{
    return GlyphSetDrawMode(aHandle, GLYPH_CMD_SCREEN_CLEAR) ;
}

/******************************************************************************
* ID : 15.0
* Outline : GlyphInvertScreen
* Include : Glyph.h
* Function Name: GlyphInvertScreen
* Description : Inverts every pixel on the screen from white to dark and dark to white.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphInvertScreen(T_glyphHandle aHandle)
{
    return GlyphSetDrawMode(aHandle, GLYPH_CMD_SCREEN_INVERT) ;
}

/******************************************************************************
* ID : 16.0
* Outline : GlyphNormalScreen
* Include : Glyph.h
* Function Name: GlyphNormalScreen
* Description : Sets up the normal LCD Screen right after initialization
* startup.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphNormalScreen(T_glyphHandle aHandle)
{
    return GlyphSetDrawMode(aHandle, GLYPH_CMD_SCREEN_REGULAR) ;
}

/******************************************************************************
* ID : 17.0
* Outline : GlyphSleep
* Include : Glyph.h
* Function Name: GlyphSleep
* Description : Puts the LCD in a low power blank screen.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphSleep(T_glyphHandle aHandle)
{
    return GlyphSetDrawMode(aHandle, GLYPH_CMD_SCREEN_SLEEP) ;
}

/******************************************************************************
* ID : 18.0
* Outline : GlyphSleep
* Include : Glyph.h
* Function Name: GlyphSleep
* Description : Takes the LCD out of low power sleep mode and redraws the
* current image.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphWake(T_glyphHandle aHandle)
{
    return GlyphSetDrawMode(aHandle, GLYPH_CMD_SCREEN_WAKE) ;
}

/******************************************************************************
* ID : 19.0
* Outline : GlyphDrawTestPattern
* Include : Glyph.h
* Function Name: GlyphDrawTestPattern
* Description : Draws a test image onto the LCD Screen.
* current image.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphDrawTestPattern(T_glyphHandle aHandle)
{
    return GlyphSetDrawMode(aHandle, GLYPH_CMD_TEST_PATTERN) ;
}

/******************************************************************************
* ID : 20.0
* Outline : GlyphDrawBlock
* Include : Glyph.h
* Function Name: GlyphDrawBlock
* Description : Draws a set of dark pixels from the X and Y position to the
* X2 and Y2 position.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aX1 - the given X position.
*          : aY1 - the given Y position.
*          : aX2 - the given X2 position.
*          : aY2 - the given Y2 position.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphDrawBlock(T_glyphHandle aHandle, uint32_t aX1, uint32_t aY1, uint32_t aX2, uint32_t aY2)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    if (p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_X, aX1) == GLYPH_ERROR_NONE) {
        if (p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_Y, aY1) == GLYPH_ERROR_NONE) {
            if (p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_X2, aX2) == GLYPH_ERROR_NONE) {
                if (p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_Y2, aY2) == GLYPH_ERROR_NONE) {
                    return GlyphSetDrawMode(aHandle, GLYPH_CMD_DRAW_BLOCK) ;
                }
            }
        }
    }

    return GLYPH_ERROR_ILLEGAL_OPERATION ;
}

/******************************************************************************
* ID : 21.0
* Outline : GlyphEraseBlock
* Include : Glyph.h
* Function Name: GlyphEraseBlock
* Description : Clears to a set of white pixels from the X and Y position to the
* X2 and Y2 position.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : aX1 - the given X position.
*          : aY1 - the given Y position.
*          : aX2 - the given X2 position.
*          : aY2 - the given Y2 position.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphEraseBlock(T_glyphHandle aHandle, uint32_t aX1, uint32_t aY1, uint32_t aX2, uint32_t aY2)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    if (p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_X, aX1) == GLYPH_ERROR_NONE) {
        if (p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_Y, aY1) == GLYPH_ERROR_NONE) {
            if (p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_X2, aX2) == GLYPH_ERROR_NONE) {
                if (p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CHAR_Y2, aY2) == GLYPH_ERROR_NONE) {
                    return GlyphSetDrawMode(aHandle, GLYPH_CMD_ERASE_BLOCK) ;
                }
            }
        }
    }

    return GLYPH_ERROR_ILLEGAL_OPERATION ;
}

/******************************************************************************
* ID : 22.0
* Outline : GlyphSetContrast
* Include : Glyph.h
* Function Name: GlyphSetContrast
* Description : Sets the contrast of the LCD Display.  Any Positive number
* is ok. Check the documentation of your LCD and study the LCD Driver Code
* for correct values.  If the number is too large the contrast will not be
* changed.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : nContrast - A Contrast setting value.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphSetContrast(T_glyphHandle aHandle, int32_t nContrast)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    return p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CONTRAST, (uint32_t)nContrast) ;
}

/******************************************************************************
* ID : 23.0
* Outline : GlyphSetContrastBoost
* Include : Glyph.h
* Function Name: GlyphSetContrastBoost
* Description : Sets the boost for the contrast of the LCD Display.  Any
* Positive number is ok.  Check the documentation of your LCD and study the
* LCD Driver Code for correct values.  If the number is too large the contrast
* boost will not be changed.
* Argument : aHandle - the Glyph handle setup by the LCD and Communications.
*          : cContrastBoost - A Contrast Booster setting value.
* Return Value : 0=success, not 0= error
* Calling Functions : main
******************************************************************************/
T_glyphError GlyphSetContrastBoost(T_glyphHandle aHandle, uint8_t cContrastBoost)
{
    T_glyphWorkspace *p_glyph = (T_glyphWorkspace *)aHandle;

    return p_glyph->iLCDAPI->iWrite(aHandle, GLYPH_CONTRAST_BOOST, (uint32_t)cContrastBoost) ;
}
