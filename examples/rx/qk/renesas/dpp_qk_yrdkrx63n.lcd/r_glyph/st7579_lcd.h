/*-------------------------------------------------------------------------*
 * File:  ST7579_LCD.h
 *-------------------------------------------------------------------------*
 * Description:
 *     Prototypes for the LCD Driver in the Glyph API.
 * Manufacturer: OKAYA
 * Microcontroller: ST7579
 * Dimensions: 68 x 102
 * Communications Required: 3-Wire SPI with a 4th Wire independent
 *                          Transmit Only
 *-------------------------------------------------------------------------*/
#ifndef __GLYPH__ST7579_LCD_HEADER_FILE
#define __GLYPH__ST7579_LCD_HEADER_FILE

/******************************************************************************
Includes “Glyph Include”
******************************************************************************/
#include "..\Glyph.h"

/******************************************************************************
* Outline : Function Sets
* Description : These sets of defines are used to define the Function Set call
* for every command that is sent to the ST7579 Display controller.  When
* setting the display controller to the 4 different function sets of commands
* to send to the controller, setting the function set also sets the following:
* MX direction: normal or reverse    [0:1]
* MY direction: normal or reverse    [0:1]
* PD controller active or power down [0:1]
*
* If the preprocessor directive #if is set to 1 then every Function Set call
* will produce MY set to the reverse direction.  This is required for Glyph.
*
* Otherwise MY will be set to the normal direction and Glyph will not work.
* Argument : none
* Return Value : none
* Calling Functions : ST7579_Write
******************************************************************************/
#if 1
// MY reverse direction
#define LCD_FUNCTION_ZERO    0X28
#define LCD_FUNCTION_ONE     0X29
#define LCD_FUNCTION_TWO     0X2A
#define LCD_FUNCTION_THREE   0X2B
     // command sets
#define LCD_DISPLAY_REVERSE  0x280D
#define LCD_DISPLAY_NORMAL   0x280C
#else
// MY normal direction
#define LCD_FUNCTION_ZERO    0X20
#define LCD_FUNCTION_ONE     0X21
#define LCD_FUNCTION_TWO     0X22
#define LCD_FUNCTION_THREE   0X23
     // command sets
#define LCD_DISPLAY_REVERSE  0x200D
#define LCD_DISPLAY_NORMAL   0x200C
#endif

/******************************************************************************
Prototypes for the Glyph LCD API
******************************************************************************/
T_glyphError ST7579_Open(T_glyphHandle aHandle, uint32_t aAddress) ;
T_glyphError ST7579_Close(T_glyphHandle aHandle) ;
T_glyphError ST7579_Write(T_glyphHandle aHandle, uint32_t aRegister, uint32_t aValue) ;
T_glyphError ST7579_Read(T_glyphHandle aHandle, uint32_t aRegister, uint32_t *aValue) ;

/******************************************************************************
Prototypes for the LCD API
******************************************************************************/
void ST7579_Config(T_glyphHandle aHandle) ;
void ST7579_SetSystemBiasBooster(T_glyphHandle aHandle, int8_t cValue0To17) ;
void ST7579_SetVO_Range(T_glyphHandle aHandle, int32_t nValue0to254) ;
void ST7579_SetFrameRate(T_glyphHandle aHandle, int32_t nRate55To137) ;
void ST7579_SetPage(T_glyphHandle aHandle, int8_t cValue0To9) ;
void ST7579_SetChar(T_glyphHandle aHandle, int8_t cValue0To101) ;
void ST7579_SetLine(T_glyphHandle aHandle, int8_t cValue0To66) ;
void ST7579_Send8bitsData(T_glyphHandle aHandle, int8_t cData) ;
void ST7579_Send16bitsCommand(T_glyphHandle aHandle, int32_t nCommand) ;

#endif /* __GLYPH__ST7579_LCD_HEADER_FILE */

