/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : drv_hd44780_cnfg.h
 *    Description : HD44780 Driver common definitions
 *
 *    History :
 *    1. Date        : Feb 10, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 15135 $
 **************************************************************************/

#ifndef  __DRV_HD44780_CNFG_H
#define  __DRV_HD44780_CNFG_H

/* HD44780 DRV Default setup data */
// The R/W signal is connected when is > 0 to LCD module
#define HD4780_WR                 1
// Interface type 4/8bit
#define HD44780_BUS_WIDTH         8
/* Horizontal Size of display 8/16 symbols */
#define HD44780_HORIZONTAL_SIZE   16
/* Address counter direction
 for correct operation of HD44780_StrShow must be HD44780_AC_INCREMENT */
#define HD44780_AC_DIR            HD44780_AC_INCREMENT
/* Auto Display shift or cursor move */
#define HD44780_MOVE              HD44780_CURSOR_MOVE
/* MCU interface */
#if HD44780_BUS_WIDTH == 4
#define HD44780_INTERFACE         HD44780_4_BIT
#elif HD44780_BUS_WIDTH == 8
#define HD44780_INTERFACE         HD44780_8_BIT
#else
#error MCU to LCD interface width muts be 4 or 8 bits
#endif
/* Number of line */
#define HD44780_LINE              HD44780_2_LINE
/* Dot Matrix 5x8 or 5x10 */
#define HD44780_DOT_MATRIX        HD44780_DOT_5_8
/* Display On/Off */
#define HD44780_DISPLAY_STATE     HD44780_DISPLAY_ON
/* Cursor On/Off*/
#define HD44780_CURSOR_STATE      HD44780_CURSOR_OFF
/* Cursor Blink*/
#define HD44780_CURSOR_MODE       HD44780_CURSOR_NORM

#endif  /* __DRV_HD44780_CNFG_H */
