/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name       : r_glyph.c
* Version      : 1.0
* Description  : Implements the IO functions needed for the Glyph code. These functions are 'tied' to the Glyph code
*                in r_glyph_register.c.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 01.11.2011 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/* Glyph code header file. */
#include "glyph.h"
/* MCU and board headers. */
#include "iodefine.h"
/* RSPI R-package header file. */
#include "r_rspi_rx600.h"
/* Glyph defines. */
#include "r_glyph.h"
/* Board defines. */
#include "yrdkrx63n.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Pin level for when transmitting command. */
#define GLYPH_RS_COMMAND    (0)
/* Pin level for when transmitting command. */
#define GLYPH_RS_DATA       (1)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
void glyph_send_byte(int8_t data);

/***********************************************************************************************************************
* Function Name: R_GLYPH_Open
* Description  : Open and setup communications to the LCD.
* Arguments    : a_handle -
*                    The Glpyh handle.
* Return Value : GLYPH_ERROR_NONE -
*                    Success
*                GLYPH_ERROR_ILLEGAL_OPERATION -
*                    Error
***********************************************************************************************************************/
T_glyphError R_GLYPH_Open(T_glyphHandle aHandle)
{
    /* Loop counter. */
    volatile uint32_t i;
    /* Number of cycles needed for delay. */
    uint32_t delay_cycles;

    /* Reset the LCD. There is an inverter on the board so setting the pin high gives a low signal to the LCD. */
    LCD_RESET = 1;

    /* These delays are done through for() loops since this function should only need to run once after reset and the
       times are not usually too long (e.g. 1.5us). If you are using an RTOS and wanted to do some useful work you
       could yield the current task instead of waiting. */

    /* This calculation figures out how many CPU cycles are needed for the delay. One time through the for() loop
       will likely take more than 1 cycle so this time is somewhat exagerated. */
    delay_cycles = GLYPH_RESET_LOW_TIME_NS / 10; //GLYPH_RESET_LOW_TIME_NS / (ICLK_HZ / 1e7);

    for (i = 0; i < delay_cycles; i++)
    {
        /* Hold the reset for the time specified in the LCD's datasheet. */
    }

    /* There is also a delay needed after the reset. */
    LCD_RESET = 0;

    /* This calculation figures out how many CPU cycles are needed for the delay. One time through the for() loop
       will likely take more than 1 cycle so this time is somewhat exagerated. */
    delay_cycles = GLYPH_RESET_HIGH_TIME_NS / 10; //GLYPH_RESET_HIGH_TIME_NS / (ICLK_HZ / 1e7);

    for (i = 0; i < delay_cycles; i++)
    {
        /* Hold the reset for the time specified in the LCD's datasheet. */
    }

    /* Initialize RSPI channel. */
    if (true == R_RSPI_Init(GLYPH_RSPI_CHANNEL))
    {
        /* Channel is initialized. */
        return GLYPH_ERROR_NONE;
    }
    else
    {
        /* An error occured initializing the peripheral. */
        return GLYPH_ERROR_ILLEGAL_OPERATION;
    }
} /* End of function R_GLYPH_Open() */


/***********************************************************************************************************************
* Function Name: R_GLYPH_CommandSend
* Description  : Send a command to the LCD.  The defines for all commandsa that are available to send to the ST7579 LCD
*                controller are defined in the header file st7579_lcd.h.  The nCommand parameter should always be
*                a define from that location.
* Arguments    : c_command -
*                    The command to send.
* Return Value : none
***********************************************************************************************************************/
void R_GLYPH_CommandSend(int8_t c_command)
{
    /* Let LCD know that this is a command transmission. */
    LCD_RS = GLYPH_RS_COMMAND;

    /* Send command. */
    glyph_send_byte(c_command);

} /* End of function R_GLYPH_CommandSend() */


/***********************************************************************************************************************
* Function Name: R_GLYPH_DataSend
* Description  : Send 8 bits of data to the LCD RAM at the current location. The current location is determined and
*                set with local functions.
* Arguments    : c_data -
*                    The data to send.
* Return Value : none
***********************************************************************************************************************/
void R_GLYPH_DataSend(int8_t c_data)
{
    /* Let LCD know that this is a data transmission. */
    LCD_RS = GLYPH_RS_DATA;

    /* Send data. */
    glyph_send_byte(c_data);

} /* End of function R_GLYPH_DataSend() */

/***********************************************************************************************************************
* Function Name: R_GLYPH_DataSend
* Description  : Actually sends the byte out the RSPI. This also takes care of locking the RSPI peripheral.
* Arguments    : data -
*                    The data to send.
* Return Value : none
***********************************************************************************************************************/
void glyph_send_byte(int8_t data)
{
    /* Attempt to lock the RSPI peripheral. */
    while (false == R_RSPI_Lock(GLYPH_RSPI_CHANNEL, GLYPH_RSPI_PID))
    {
        /* Wait until lock is given up. At this point another task must unlock the RSPI peripheral before we can
           lock it here. This means this other task must unlock from an ISR or from another RTOS task. */
#if defined(RTOS_PRESENT)
        /* If an RTOS is defined then the current task can yield and come back later. Right now it is time based and
           using FreeRTOS, but you could yield on a semaphore here as well. */
        vTaskDelay( GLYPH_LOCK_DELAY_MS / portTICK_RATE_MS );
#endif
    }

    /* The lock has been obtained. Now assert the LCD CS. */
    R_RSPI_Select(GLYPH_RSPI_CHANNEL, LCD_SELECTED, GLYPH_RSPI_PID);

    /* Send the data. */
    R_RSPI_Write(GLYPH_RSPI_CHANNEL, (const uint8_t *)&data, sizeof(data), GLYPH_RSPI_PID);

    /* Data is sent. Deselect the LCD. */
    R_RSPI_Deselect(GLYPH_RSPI_CHANNEL, LCD_SELECTED, GLYPH_RSPI_PID);

    /* Release the lock. */
    R_RSPI_Unlock(GLYPH_RSPI_CHANNEL, GLYPH_RSPI_PID);

}/* End of function glyph_send_byte() */
