/*-------------------------------------------------------------------------*
 * File:  okaya_lcd.h
 *-------------------------------------------------------------------------*
 * Description:
 *     Prototypes for the LCD Driver in the Glyph API.
 * Manufacturer: OKAYA
 * Dimensions: 68 x 102
 * Communications Required: 3-Wire SPI with a 4th Wire independent
 *                          Transmit Only
 *-------------------------------------------------------------------------*/
#ifndef __OKAYA_LCD
#define __OKAYA_LCD

/******************************************************************************
Includes “Glyph Include”
******************************************************************************/
#include "Glyph.h"

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
* Calling Functions : OKAYA_Write
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
T_glyphError OKAYA_Open(T_glyphHandle aHandle, uint32_t aAddress) ;
T_glyphError OKAYA_Close(T_glyphHandle aHandle) ;
T_glyphError OKAYA_Write(T_glyphHandle aHandle, uint32_t aRegister, uint32_t aValue) ;
T_glyphError OKAYA_Read(T_glyphHandle aHandle, uint32_t aRegister, uint32_t *aValue) ;

/******************************************************************************
Prototypes for the LCD API
******************************************************************************/
void OKAYA_Config(T_glyphHandle aHandle) ;
void OKAYA_SetSystemBiasBooster(T_glyphHandle aHandle, int8_t cValue0To17) ;
void OKAYA_SetVO_Range(T_glyphHandle aHandle, int32_t nValue0to254) ;
void OKAYA_SetFrameRate(T_glyphHandle aHandle, int32_t nRate55To137) ;
void OKAYA_SetPage(T_glyphHandle aHandle, int8_t cValue0To9) ;
void OKAYA_SetChar(T_glyphHandle aHandle, int8_t cValue0To101) ;
void OKAYA_SetLine(T_glyphHandle aHandle, int8_t cValue0To66) ;
void OKAYA_Send8bitsData(T_glyphHandle aHandle, int8_t cData) ;
void OKAYA_Send16bitsCommand(T_glyphHandle aHandle, int32_t nCommand) ;

#endif /* __OKAYA_LCD */

