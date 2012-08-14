/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : drv_hd44780.h
 *    Description : HD44780 Driver definition
 *
 *    History :
 *    1. Date        : Feb 10, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : Nov 3, 2005
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *                      Add some definitions connected
 *                     with HD44780_StrShow, HD44780_CheckVisual,
 *                     HD44780_DisplayShift, HD44780_GetDDRamAdd functions
 *    3. Date        : Jan 4, 2006
 *       Author      : Stanimir Bonev
 *       Description : Modify
 *                      Macro HD44780_BUS_DLY
 *
 *    $Revision: 15135 $
 **************************************************************************/

#ifndef  __DRV_HD44780_H
#define  __DRV_HD44780_H

typedef uint8_t HD44780_XY_DEF;
typedef int8_t  HD44780_STRING_DEF;

/* Display address range definition */
#define HD44780_MIN_LINE1_ADD     0x00
#define HD44780_MAX_LINE1_ADD     0x27
#define HD44780_MIN_LINE2_ADD     0x40
#define HD44780_MAX_LINE2_ADD     0x67
/* Max DDRAM Address */
#define HD44780_MAX_DDRAM         0x67
/* Max CGRAM Address */
#define HD44780_MAX_CGRAM         0x3F
/* Address counter auto increment/decrement definition */
#define HD44780_AC_DECREMENT      0
#define HD44780_AC_INCREMENT      1
/* Auto Display shift or cursor move definition */
#define HD44780_CURSOR_MOVE       0
#define HD44780_DISPLAY_SHIFT     1
/* Number of line 1/2 definition */
#define HD44780_1_LINE            0
#define HD44780_2_LINE            1
/* Dot 5x8 or 5x10 definition */
#define HD44780_DOT_5_8           0
#define HD44780_DOT_5_10          1
/* Display On/Off definition */
#define HD44780_DISPLAY_OFF       0
#define HD44780_DISPLAY_ON        1
/* Cursor On/Off definition */
#define HD44780_CURSOR_OFF        0
#define HD44780_CURSOR_ON         1
/* Cursor Blink definition */
#define HD44780_CURSOR_NORM       0
#define HD44780_CURSOR_BLINK      1

typedef struct _HD44780_CTRL_DEF
{
  uint8_t  DisplayPos;
  uint16_t AC_Direction  : 1;
  uint16_t DisplayShift  : 1;
  uint16_t Line          : 1;
  uint16_t DotMode       : 1;
  uint16_t DisplayOn     : 1;
  uint16_t CursorOn      : 1;
  uint16_t CursorBlink   : 1;
} HD44780_CTRL_DEF;

typedef enum _HD44780_ERROR_CODE_DEF
{
  HD44780_OK = 0,
  HD44780_BUSY_TO_ERROR,
  HD44780_ERROR,
  HD44780_OUT_OF_VISUAL,
} HD44780_ERROR_CODE_DEF;

HD44780_ERROR_CODE_DEF HD44780_PowerUpInit (void);
HD44780_CTRL_DEF * HD44780_GetSetting(void);
HD44780_ERROR_CODE_DEF HD44780_SetMode (void);
HD44780_ERROR_CODE_DEF HD44780_ClearDisplay (void);
HD44780_ERROR_CODE_DEF HD44780_ReturnToHome (void);
HD44780_ERROR_CODE_DEF HD44780_DisplayShift (uint8_t DisplayOn, int8_t DisplayShift);
HD44780_ERROR_CODE_DEF HD44780_CursorPosSet (uint8_t CursorOn, uint8_t CursorBlink, HD44780_XY_DEF X, HD44780_XY_DEF Y);
HD44780_ERROR_CODE_DEF HD44780_RdCGRAM (HD44780_STRING_DEF * CG_Data, uint8_t CGRAM_Add);
HD44780_ERROR_CODE_DEF HD44780_WrCGRAM (const HD44780_STRING_DEF * CG_Data, uint8_t CGRAM_Add);
HD44780_ERROR_CODE_DEF HD44780_StrShow(HD44780_XY_DEF X, HD44780_XY_DEF Y,  const HD44780_STRING_DEF * DataStr);

HD44780_ERROR_CODE_DEF HD44780_DispHorBarInit(void);
HD44780_ERROR_CODE_DEF HD44780_DispHorBar(HD44780_XY_DEF X, HD44780_XY_DEF Y, uint8_t val, uint8_t max);

uint8_t HD44780_GetDisplayPosition (void);

extern void Dly100us(void *arg);

#endif  /* __DRV_HD44780_H */
