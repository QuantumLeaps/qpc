/*-------------------------------------------------------------------------*
 * File:  Glyph.h
 *-------------------------------------------------------------------------*
 * Description:
 *     Glyph - The Generic API for Graphics LCD.
 * Glyph API Library Main Header
 *-------------------------------------------------------------------------*/
#ifndef GLYPH_LIB_GLYPH_HEADER_FILE
#define GLYPH_LIB_GLYPH_HEADER_FILE

/******************************************************************************
Includes “Glyph config Include”
******************************************************************************/
#include "Config.h"

//#include <stdbool.h>
//#include <stdlib.h>
//#include <stdio.h>

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/* Global Fonts to all users of API at all times                             */
extern const uint8_t * FontHelvr10_table[256];
// const uint8_t * Bitmaps_table[256];
extern const uint8_t * Font8x16_table[256];
extern const uint8_t * Font8x8_table[256];
extern const uint8_t * FontWinFreeSystem14x16_table[256];
extern const uint8_t * Fontx5x7_table[256];
extern const uint8_t * Fontx6x13_table[256];

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef void *T_glyphHandle;

/******************************************************************************
* Outline : T_glyphError  Enum
* Description :  The Glyph API Error Identifiers Enumeration.
* Calling Functions : All
******************************************************************************/
typedef enum {
    GLYPH_ERROR_NONE=0,
    GLYPH_ERROR_ILLEGAL_OPERATION
} T_glyphError ;

/******************************************************************************
* Outline : T_glyphStatusBits  Enum
* Description : The Glyph API Status Values Enumeration.
* Calling Functions : ???_Read, ???_Write
******************************************************************************/
typedef enum {
    GLYPH_STATUS_READY=0,
    GLYPH_STATUS_BUSY=2,
    GLYPH_STATUS_FULL=4
} T_glyphStatusBits ;

/******************************************************************************
* Outline : T_glyphFont  Enum
* Description :  The Glyph API Font Values Enumeration.
* Calling Functions : ???_Read, ???_Write
******************************************************************************/
typedef enum {
    GLYPH_FONT_8_BY_8,
    GLYPH_FONT_8_BY_16,
    GLYPH_FONT_5_BY_7,
    GLYPH_FONT_6_BY_13,
    GLYPH_FONT_WINFREESYSTEM14_BY_16,
    GLYPH_FONT_HELVR10,
    GLYPH_FONT_BITMAP
} T_glyphFont ;

/******************************************************************************
* Outline : T_glyphDrawMode  Enum
* Description :  The Glyph API Draw Modes Enumeration.
* Calling Functions : ???_Write
******************************************************************************/
typedef enum {
    GLYPH_CMD_NOP=0,
    GLYPH_CMD_SCREEN_CLEAR,
    GLYPH_CMD_SCREEN_INVERT,
    GLYPH_CMD_SCREEN_REGULAR,
    GLYPH_CMD_SCREEN_SLEEP,
    GLYPH_CMD_SCREEN_WAKE,
    GLYPH_CMD_TEST_PATTERN,
    GLYPH_CMD_DRAW_BLOCK,
    GLYPH_CMD_ERASE_BLOCK
} T_glyphDrawMode ;

/******************************************************************************
* Outline : T_glyphRegisters  Enum
* Description :  The Glyph API Registers.  These are used by GlyphRead and
* GlyphWrite to specify a function to run.
* Calling Functions : ???_Read, ???_Write
******************************************************************************/
typedef enum {
    GLYPH_STATUS=0,
    GLYPH_CHAR_X,
    GLYPH_CHAR_Y,
    GLYPH_FONT,
    GLYPH_DRAW_CHAR,
    GLYPH_DRAW_CMD,
    GLYPH_CHAR_ERASE,
    GLYPH_INVERT_CHAR,
    GLYPH_API_MAJOR_VERSION,
    GLYPH_API_MINOR_VERSION,
    GLYPH_IMPLEMENTATION_ID,
    GLYPH_IMPL_MAJOR_VERSION,
    GLYPH_IMPL_MINOR_VERSION,
    GLYPH_CHAR_X2,
    GLYPH_CHAR_Y2,
    GLYPH_RESOLUTION_X,
    GLYPH_RESOLUTION_Y,
    GLYPH_GRANULARITY_X,
    GLYPH_GRANULARITY_Y,
    GLYPH_FRAME_RATE,
    GLYPH_CONTRAST,
    GLYPH_CONTRAST_BOOST
} T_glyphRegisters ;

/******************************************************************************
* Outline : T_glyphVersionInfo  Structure
* Description :  The Glyph API Version Structure. Used to house a call to Get
* Version.
* Calling Functions : ???_Read
******************************************************************************/
typedef struct _glyph_version_info
{
    uint8_t strVersionAPI[50] ;
    uint8_t strVersionIMPL[50] ;
    int32_t nVersionAPIMajor ;
    int32_t nVersionAPIMinor ;
    int32_t nVersionIMPLMajor ;
    int32_t nVersionIMPLMinor ;
    int32_t nImplementationID ;
} T_glyphVersionInfo ;

/******************************************************************************
* Outline : T_LCD_API  Structure
* Description :  The Glyph API LCD API Structure. Used by the LCD Driver.
* Calling Functions : ???_LCD_???
******************************************************************************/
typedef struct {
    /* Adding Font-Decal Section*/
    const uint8_t ** iFont;

    /* Define Global Variables */
    uint32_t iCharX_Position;
    uint32_t iCharY_Position;
    uint32_t iCharX2_Position;
    uint32_t iCharY2_Position;
    uint32_t iCharFont;
    uint32_t iFP_API_MAJOR_VERSION;
    uint32_t iFP_API_MINOR_VERSION;
    uint32_t iFP_IMPLEMENTATION_ID;
    uint32_t iFP_IMPL_MAJOR_VERSION;
    uint32_t iFP_IMPL_MINOR_VERSION;
    uint32_t iMAX_HEIGHT;
    uint32_t iMAX_WIDTH;

    uint16_t iStatusRegister ;
    int32_t iAddress;

    int32_t iContrast ;
    int8_t iContrastBoost ;
    int32_t iFrameRate ;

    T_glyphError (*iOpen)(T_glyphHandle aHandle, uint32_t aAddress);
    T_glyphError (*iClose)(T_glyphHandle aHandle);
    T_glyphError (*iRead)(T_glyphHandle aHandle, uint32_t aRegister, uint32_t *Value);
    T_glyphError (*iWrite)(T_glyphHandle aHandle, uint32_t aRegister, uint32_t Value);
} T_LCD_API ;

/******************************************************************************
* Outline : T_Comm_API  Structure
* Description :  The Glyph API Comm API Structure. Used by the Communications
* Driver.
* Calling Functions : GlyphCommOpen
******************************************************************************/
typedef struct {
    T_glyphError (*iOpen)(T_glyphHandle aHandle);
    void (*iCommandSend)(int8_t cCommand);
    void (*iDataSend)(int8_t cData);
} T_Comm_API ;

/******************************************************************************
* Outline : T_glyphWorkspace  Structure
* Description :  The Glyph API Workspace Structure. Used by the Communications
* and LCD Drivers.
* Calling Functions : GlyphCommOpen, ???_LCDOpen, GlyphOpen, ???_Read,
*                     ???_Write
******************************************************************************/
typedef struct {
    T_LCD_API *iLCDAPI ;
    T_Comm_API *iCommAPI ;
} T_glyphWorkspace;

/******************************************************************************
Prototypes for the Glyph API Minimum Access functions
******************************************************************************/
T_glyphError GlyphOpen(T_glyphHandle *aHandle, int32_t aAddress);
T_glyphError GlyphClose(T_glyphHandle *aHandle);
T_glyphError GlyphWrite(T_glyphHandle aHandle, uint32_t aRegister, uint32_t aValue);
T_glyphError GlyphRead(T_glyphHandle aHandle, uint32_t aRegister, uint32_t *aValue);

/******************************************************************************
Prototypes for the Glyph API Full Access functions
******************************************************************************/
T_glyphError GlyphGetStatus(T_glyphHandle aHandle, T_glyphStatusBits *aStatus);
T_glyphError GlyphSetX(T_glyphHandle aHandle, uint32_t aX);
T_glyphError GlyphSetY(T_glyphHandle aHandle, uint32_t aY);
T_glyphError GlyphSetXY(T_glyphHandle aHandle, uint32_t aX, uint32_t aY);
T_glyphError GlyphGetXY(T_glyphHandle aHandle, uint32_t *aX, uint32_t *aY);
T_glyphError GlyphSetFont(T_glyphHandle aHandle, T_glyphFont aFont);
T_glyphError GlyphGetFont(T_glyphHandle aHandle, T_glyphFont *aFont);
T_glyphError GlyphSetDrawMode(T_glyphHandle aHandle, T_glyphDrawMode aMode);
T_glyphError GlyphChar(T_glyphHandle aHandle, uint32_t aChar);
T_glyphError GlyphString(T_glyphHandle aHandle, uint8_t * aString, uint32_t aLength);
T_glyphError GlyphGetVersionInfo(T_glyphHandle aHandle, T_glyphVersionInfo *aInfo);
T_glyphError GlyphClearScreen(T_glyphHandle aHandle);
T_glyphError GlyphInvertScreen(T_glyphHandle aHandle);
T_glyphError GlyphNormalScreen(T_glyphHandle aHandle);
T_glyphError GlyphSleep(T_glyphHandle aHandle);
T_glyphError GlyphWake(T_glyphHandle aHandle);
T_glyphError GlyphDrawTestPattern(T_glyphHandle aHandle);
T_glyphError GlyphDrawBlock(T_glyphHandle aHandle, uint32_t aX1, uint32_t aY1, uint32_t aX2, uint32_t aY2);
T_glyphError GlyphEraseBlock(T_glyphHandle aHandle, uint32_t aX1, uint32_t aY1, uint32_t aX2, uint32_t aY2);
T_glyphError GlyphSetContrast(T_glyphHandle aHandle, int32_t nContrast) ;
T_glyphError GlyphSetContrastBoost(T_glyphHandle aHandle, uint8_t cContrastBoost) ;

#endif /* GLYPH_LIB_GLYPH_HEADER_FILE */

