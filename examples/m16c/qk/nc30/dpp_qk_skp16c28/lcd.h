/*****************************************************************
 * Driver for ACM0802C LCD Module (8 characters by 2 lines )
 * on the Renesas SKP boards - header file
 *
 *  Copyright (c) 2004 Renesas Technology America, Inc.
 *  All rights reserved.
 *****************************************************************/

#ifndef _SKP_LCD_H
#define _SKP_LCD_H

void InitDisplay( void );
void DisplayString(unsigned char position, _far char * string);
void LCD_write(unsigned char data_or_ctrl, unsigned char value);
void DisplayDelay(unsigned long int units);


/* The special bitmap characters are mapped to display values 0 to 7 */
#define RENESAS_LOGO "\x0\x1\x2\x3\x4\x5\x6\x7\x0"
//const    char RENESAS_LOGO[] = {0,1,2,3,4,5,6,7,0};


#define HIGH 1
#define LOW 0
#define DATA_PORT        p9     // Data bus port
#define RS_PIN           p6_0   // RS Register Select pin
#define RS_PIN_MASK      0x0    // bit mask from entire port
#define RS_PIN_DDR       pd6_0  // RS pin DDR
#define EN_PIN           p6_1   // Display Enable pin
#define EN_PIN_MASK      0x02   // bit mask from entire port
#define EN_PIN_DDR       pd6_1  // EN pin DDR

#define PORT_DDR         pd9    // Data bus direction register
#define PORT_DDR_VALUE   0x0f   // Can't read DDR reg, so have can't OR values

#define DATA_WR 1
#define CTRL_WR 0

#define MAXIMUM_LINES        2  // number of lines on the LCD display
#define NUMB_CHARS_PER_LINE  8  // Maximum charactors per line of LCD display

#define LCD_LINE1 0
#define LCD_LINE2 16

/****************************************************************************/
// LCD commands - use LCD_write function to write these commands to the LCD.
/****************************************************************************/
#define LCD_CLEAR        0x01   // Clear LCD display and home cursor
#define LCD_HOME_L1      0x80   // move cursor to line 1
#define LCD_HOME_L2      0xC0   // move cursor to line 2
#define CURSOR_MODE_DEC  0x04   // Cursor auto decrement after R/W
#define CURSOR_MODE_INC  0x06   // Cursor auto increment after R/W
#define FUNCTION_SET     0x28   // Setup, 4 bits,2 lines, 5X7
#define LCD_CURSOR_ON    0x0E   // Display ON with Cursor
#define LCD_CURSOR_OFF   0x0C   // Display ON with Cursor off
#define LCD_CURSOR_BLINK 0x0D   // Display on with blinking cursor
#define LCD_CURSOR_LEFT  0x10   // Move Cursor Left One Position
#define LCD_CURSOR_RIGHT 0x14   // Move Cursor Right One Position

#endif