// tab space = 4
/*********************************************************************
* DISCLAIMER:                                                        *
* The software supplied by Renesas Technology America Inc. is        *
* intended and supplied for use on Renesas Technology products.      *
* This software is owned by Renesas Technology America, Inc. or      *
* Renesas Technology Corporation and is protected under applicable   *
* copyright laws. All rights are reserved.                           *
*                                                                    *
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, *
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED         *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE *
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND       *
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT      *
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS          *
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL, *
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR        *
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE *
* USE OR APPLICATION OF THIS SOFTWARE.                               *
*********************************************************************/

/*********************************************************************
*                                                                    *
*   FILE NAME: LCD.c                                                 *
*                                                                    *
* DESCRIPTION: Driver for ACM0802C LCD Module on Renesas' SKP boards.*
*              (8 characters by 2 lines)                             *
*                                                                    *
*                                                                    *
*********************************************************************/

/*********************************************************************
Revision History
   1.0 May 21, 2004
       Added the ability to set the starting position for the
       DisplayString function. Also, the LCD position will not be
       set if the current position is already correct.
   0.4 Jan 5, 2004
       Initial Version
*********************************************************************/

/* sfr26a.h provides a structure to access all of the device registers. */
#include "sfr28.h"

/* SKP16C28Adef.h provides common defines for widely used items. */
#include "skp16c28def.h"

/* LCD.h provides defines for LCD interface. */
#include "lcd.h"

/* Used for Renesas Logo bit mapping */
const char logo_map[8*8] =
{
    0x03,0x00,0x00,0x00,0x07,0x07,0x07,0x00,    // R
    0x1E,0x03,0x03,0x1E,0x18,0x0C,0x03,0x00,    // R
    0x0F,0x10,0x10,0x1F,0x10,0x10,0x0F,0x00,    // E
    0x11,0x19,0x1D,0x15,0x17,0x13,0x11,0x00,    // N
    0x0F,0x10,0x10,0x1F,0x10,0x10,0x0F,0x00,    // E
    0x0F,0x10,0x10,0x0E,0x01,0x01,0x1E,0x00,    // S
    0x04,0x0A,0x0A,0x11,0x11,0x11,0x17,0x00,    // A
    0x0F,0x10,0x10,0x0E,0x01,0x01,0x1E,0x00,    // S
};

/*****************************************************************************
Name:              InitDisplay
Parameters:     none
Returns:        none
Description:    Intializes the LCD display.
*****************************************************************************/
void InitDisplay( void )
{
    int i;

    // initial port directions
    prc2=1;                            // unprotect as Port 9 is used
    PORT_DDR = PORT_DDR_VALUE;

    EN_PIN = HIGH;
    EN_PIN_DDR = HIGH;                // set port that controls EN as output
    RS_PIN = HIGH;
    RS_PIN_DDR = HIGH;                // set port that controls RS as output

    EN_PIN = LOW;

    LCD_write(CTRL_WR,0x33);
    DisplayDelay(20);
    LCD_write(CTRL_WR,0x32);
    DisplayDelay(20);
    LCD_write(CTRL_WR,FUNCTION_SET);    /* reset sequence */
    LCD_write(CTRL_WR,FUNCTION_SET);
    LCD_write(CTRL_WR,LCD_CURSOR_OFF);
    LCD_write(CTRL_WR,LCD_CLEAR);
    LCD_write(CTRL_WR,LCD_HOME_L1);

    // Map the Renesas logo characters into CG RAM
    LCD_write(CTRL_WR,0x40);
    for( i=0;i<64;i++)
        LCD_write(DATA_WR,logo_map[i]);

}

/*****************************************************************************
Name:            DisplayString
Parameters:      position    Line number of display
                string        Pointer to data to be written to display.
                            Last character should be null.
Returns:        none
Description:    This function controls LCD writes to line 1 or 2 of the LCD.
                You need to use the defines LCD_LINE1 and LCD_LINE2 in order
                to specify the starting position.
                For example, to start at the 2nd position on line 1...
                   DisplayString(LCD_LINE1 + 1, "Hello")
*****************************************************************************/
void DisplayString(unsigned char position, _far char * string)
{
    static unsigned char next_pos = 0xFF;

    /* Set line position if needed. We don't want to if we don't need
       to because LCD control operations take longer than LCD data
       operations. */
    if( next_pos != position)
    {
        if(position < LCD_LINE2)
        {
            /* Display on Line 1 */
              LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L1 + position));
        }
        else
        {
            /* Display on Line 2 */
              LCD_write(CTRL_WR,
                        (unsigned char)(LCD_HOME_L2 + position - LCD_LINE2));
        }
        next_pos = position;         // set position index to known value
    }

    do
    {
        LCD_write(DATA_WR,*string++);
        next_pos++;                  // increment position index
    }
    while(*string);
}

/*****************************************************************************
Name:           LCD_write
Parameters:     value - the value to write
                data_or_ctrl - To write value as DATA or CONTROL
                                1 = DATA
                                0 = CONTROL
Returns:        none
Description:    Writes data to display. Sends command to display.
*****************************************************************************/
void LCD_write(unsigned char data_or_ctrl, unsigned char value)
{

    RS_PIN = data_or_ctrl;           // RS SELECT (HIGH=DATA, LOW=CTRL

    /* Write upper nibble first */
    DATA_PORT &= 0xF0;               // Clear lower part of port
    DATA_PORT |= (value & 0xF0)>>4;  // OR in upper nibble
    EN_PIN = HIGH;                   // EN enable chip (HIGH)
    DisplayDelay(0);                 // We only need a very little delay
    EN_PIN = LOW;                    // Latch data by dropping EN
    DisplayDelay(0);                 // We only need a very little delay

    if(data_or_ctrl == CTRL_WR)
        DisplayDelay(1);             // extra delay needed for control writes

    /* Write lower nibble second */
    DATA_PORT &= 0xF0;               // Clear lower part of port
    DATA_PORT |= (value & 0x0F) ;    // write to port
    EN_PIN = HIGH;
    DisplayDelay(0);                 // We only need a very little delay
    EN_PIN = LOW;                    // Latch data by dropping EN
    DisplayDelay(1);                 // needed to put delay in between writes

    if(data_or_ctrl == CTRL_WR)
        DisplayDelay(40);            // extra delay needed for control writes
}

/*****************************************************************************
Name:          DisplayDelay
Parameters:    unit
Returns:       none
Description:   Delay routine for LCD display.
*****************************************************************************/
void DisplayDelay(unsigned long int units){

    unsigned long int counter = units * 0x100;

    while(counter--){
        _asm ("NOP");
        _asm ("NOP");
        _asm ("NOP");
    }
}
