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

#ifndef  __DRV_HD44780_CNFG
#define  __DRV_HD44780_CNFG

/* HD44780 DRV Default setup data */
/* Horizontal Size of dispaly 8/16 symbols */
#define HD44780_HORIZONTAL_SIZE   16
/* Address conuter direction
 for correct operation of HD44780_StrShow must be HD44780_AC_INCREMENT */
#define HD44780_AC_DIR  HD44780_AC_INCREMENT
/* Auto Display shift or cursor move */
#define HD44780_MOVE  HD44780_CURSOR_MOVE
/* MCU interface */
#define HD44780_INTERFACE  HD44780_4_BIT
/* Number of line */
#define HD44780_LINE  HD44780_2_LINE
/* Dot Matrix 5x8 or 5x10 */
#define HD44780_DOT_MATRIX  HD44780_DOT_5_8
/* Dispaly On/Off */
#define HD44780_DISPLAY_STATE HD44780_DISPLAY_ON
/* Cursor On/Off*/
#define HD44780_CURSOR_STATE HD44780_CURSOR_OFF
/* Cursor Blink*/
#define HD44780_CURSOR_MODE HD44780_CURSOR_NORM

#endif  /* __DRV_HD44780_CNFG */
