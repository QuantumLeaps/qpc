/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : drv_hd44780_l.h
 *    Description : STR912F HD44780 low level I/O function driver
 *
 *    History :
 *    1. Date        : September 29, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16170 $
 **************************************************************************/
#ifndef  __DRV_HD44780_L_H
#define  __DRV_HD44780_L_H

#define LCD_LIGHT                 GPIO_Pin_5
#define LCD_RS                    GPIO_Pin_7
#define LCD_E                     GPIO_Pin_4
#define LCD_RW                    GPIO_Pin_6
#define LCD_DATA                  GPIO_Pin_All

#define LCD_LIGHT_ON()            GPIO_WriteBit(GPIO9,LCD_LIGHT,Bit_SET)
#define LCD_LIGHT_OFF()           GPIO_WriteBit(GPIO9,LCD_LIGHT,Bit_RESET)

/* Delay macro */
#define HD44780_BUS_DLY()         for(volatile int dly = 30;dly;--dly)

void HD44780_IO_Init (void);
void HD44780SetRS (uint8_t Data);
void HD44780SetE (uint8_t Data);
void HD44780WrIO (uint8_t);
#if HD4780_WR > 0
void HD44780SetRW (uint8_t Data);
uint8_t HD44780RdIO (void);
#endif //HD4780_WR > 0

#endif  /* __DRV_HD44780_L_H */
