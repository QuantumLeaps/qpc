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
* File Name       : r_glyph_config.h
* Version      : 1.0
* Description  : Configures Glyph hook code.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 01.11.2011 1.00     First Release
***********************************************************************************************************************/

#ifndef GLYPH_CONFIG_HEADER_FILE
#define GLYPH_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
**********************************************************************************************************************/
/* Choose which RSPI channel should be used to communicate with the LCD.
   On the RDKRX62N it is channel 0. */
#define GLYPH_RSPI_CHANNEL      (0)

/* The Process ID to use when locking the RSPI peripheral. */
#define GLYPH_RSPI_PID          (0x474C5950)

/* If the code cannot get a lock on the RSPI peripheral then there is an option to yield the current task if a RTOS
   is present. This define specifies how many milliseconds to yield. */
//#define GLYPH_LOCK_DELAY_MS     (1)

#endif /* GLYPH_CONFIG_HEADER_FILE */
