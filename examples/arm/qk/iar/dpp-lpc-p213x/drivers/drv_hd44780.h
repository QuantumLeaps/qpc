/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2003
 *
 *    File name   : config.h
 *    Description : Define common defenition
 *
 *    History :
 *    1. Data        : Feb 10, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 1.1 $
 **************************************************************************/

#ifndef  __DRV_HD44780
#define  __DRV_HD44780

typedef LPC_INT8U HD44780_XY_DEF;
typedef LPC_INT8S HD44780_STRING_DEF;

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
/* MCU interface definition */
#define HD44780_4_BIT             0
#define HD44780_8_BIT             1
/* Number of line 1/2 definition */
#define HD44780_1_LINE            0
#define HD44780_2_LINE            1
/* Dot 5x8 or 5x10 definition */
#define HD44780_DOT_5_8           0
#define HD44780_DOT_5_10          1
/* Dispaly On/Off definition */
#define HD44780_DISPLAY_OFF       0
#define HD44780_DISPLAY_ON        1
/* Cursor On/Off definition */
#define HD44780_CURSOR_OFF        0
#define HD44780_CURSOR_ON         1
/* Cursor Blink definition */
#define HD44780_CURSOR_NORM       0
#define HD44780_CURSOR_BLINK      1

/* Delay macro */
#define HD44780_BUS_DLY()      for(int dly = 8;dly;--dly)

typedef struct
{
  LPC_INT8U  DiplayPos;
  LPC_INT16U AC_Direction  : 1;
  LPC_INT16U DisplayShift  : 1;
  LPC_INT16U Interface     : 1;
  LPC_INT16U Line          : 1;
  LPC_INT16U DotMode       : 1;
  LPC_INT16U DisplayOn     : 1;
  LPC_INT16U CursorOn      : 1;
  LPC_INT16U CursorBlink   : 1;
} HD44780_CTRL_DEF;

typedef enum
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
HD44780_ERROR_CODE_DEF HD44780_DisplayShift (LPC_BOOL DisplayOn,
                                             LPC_INT8S DiplayShift);
HD44780_ERROR_CODE_DEF HD44780_CursorPosSet (LPC_BOOL CursorOn,
                   LPC_BOOL CursorBlink, HD44780_XY_DEF X, HD44780_XY_DEF Y);
HD44780_ERROR_CODE_DEF HD44780_RdCGRAM (HD44780_STRING_DEF * CG_Data,
                                        LPC_INT8U CGRAM_Add);
HD44780_ERROR_CODE_DEF HD44780_WrCGRAM (const HD44780_STRING_DEF * CG_Data,
                                        LPC_INT8U CGRAM_Add);
HD44780_ERROR_CODE_DEF HD44780_StrShow(HD44780_XY_DEF X, HD44780_XY_DEF Y,
                                       const HD44780_STRING_DEF * DataStr);
HD44780_ERROR_CODE_DEF HD44780_CharShow(HD44780_XY_DEF X, HD44780_XY_DEF Y,
                                        char DataChar);

/* backlight contol */
#define LIGHT_ON        1
#define LIGHT_OFF       0

#define LIGHT_SLOW      1
#define LIGHT_FAST      0

#define LIGHT_OFF_VALUE 0
#define LIGHT_ON_VALUE  0x3FF

void LightCntr (LPC_BOOL Slow,LPC_BOOL On);
void LightInit(void);


extern void Dly100us(void *arg);

#endif  /* __DRV_HD44780 */
