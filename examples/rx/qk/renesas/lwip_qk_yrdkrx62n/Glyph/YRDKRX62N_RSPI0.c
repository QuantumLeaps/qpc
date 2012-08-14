/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY
* DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved. */
/* Code written for Renesas by Future Designs, Inc. www.teamfdi.com */

/******************************************************************************
* File Name : YRDKRX62N_RSPI0.c
* Version : 1.00
* Device(s) : RSPI0 Communications
* Tool-Chain : Glyph - The Generic API for Graphics RSPI API version 1.00
* H/W Platform : RX62N Group CPU
* Description : Glyph API RX62N RSPI Transmitter 0 Operation channel 3
* Routines for RSPI code running on RX62N MCU, RSPI0.
* This program uses RSPI0.  The Chip Select for the flash device (LCD)
* is set to PC_2.
*       Manufacturer: Renesas
*       Communications Protocol: RSPI 3-Wire
*                                Transmitt only
*                                MASTER
*       Transmitter Number: 0
*       channel: 3
*       Chip Select Port: PC.2
*       RSPI Clock Speed: 25MHz
******************************************************************************
* History :
* : 21.07.2010 1.00 First Release
******************************************************************************/
#include <stdint.h>

/******************************************************************************
Includes “YRDKRX62N RSPI Includes”
******************************************************************************/
#include "iodefine.h"
#include "YRDKRX62N_RSPI0.h"

/******************************************************************************
Private Functions
******************************************************************************/
static void StartResetLCD(void) ;
static void EndResetLCD(void) ;

/******************************************************************************
* ID : 60.0
* Outline : YRDKRX62N_RSPIOpen
* Include : YRDKRX62N_RSPI0.h
* Function Name: YRDKRX62N_RSPIOpen
* Description : Open and setup the communications channel RSPI0.
* Argument : aHandle - the Glyph handle to setup for the LCD and Communications.
* Return Value : 0=success, not 0= error
* Calling Functions : GlyphCommOpen
******************************************************************************/
T_glyphError YRDKRX62N_RSPIOpen(T_glyphHandle aHandle)
{
    int i = 0 ;

    /* Set data direction for this bit to output */
    PORTC.DDR.BIT.B3 = 1 ;   // LCD GPIO for Reset LCD
    PORT5.DDR.BIT.B1 = 1 ;   // LCD RS A0

    StartResetLCD() ;
    for (i=0;i<1000000;i++){}
    EndResetLCD() ;
    for (i=0;i<1000000;i++){}

    YRDKRX62N_RSPIConfig() ;
    return GLYPH_ERROR_NONE ;
}

/******************************************************************************
* ID : 61.0
* Outline : YRDKRX62N_RSPIConfig
* Include : YRDKRX62N_RSPI0.h
* Function Name: YRDKRX62N_RSPIConfig
* Description : Setup and ready the RSPI0 for communications.
* Argument : none
* Return Value : none
* Calling Functions : YRDKRX62N_RSPIOpen
******************************************************************************/
void YRDKRX62N_RSPIConfig(void)
{
    MSTP(RSPI0) = 0 ;

    /* Select proper bank of pins for SPI0 */
    IOPORT.PFGSPI.BIT.RSPIS = 0 ;
    /* SCK (PC.5) is active */
    IOPORT.PFGSPI.BIT.RSPCKE = 1 ;

    /* SSL3 (PC.2) is inactive (toggled as GPIO instead) */
    IOPORT.PFGSPI.BIT.SSL3E = 0 ;
    /* MOSI (PC.6) is active */
    IOPORT.PFGSPI.BIT.MOSIE = 1 ;

    /* Set up chip select pin */
    /* Make it an output */
    PORTC.DDR.BIT.B2 = 1 ;
    /* Set level to inactive */
    PORTC.DR.BIT.B2 = 1 ;

    /* MISO as an input */
    PORTC.DDR.BIT.B7 = 1 ;
    /* Enable input buffer for peripheral */
    PORTC.DR.BIT.B7 = 1 ;

    /* MOSI as an output */
    PORTC.DDR.BIT.B6 = 1 ;
    /* Enable input buffer for peripheral */
    PORTC.DR.BIT.B6 = 1 ;

    /* SCK as an output */
    PORTC.DDR.BIT.B5 = 1 ;
    /* Set level to inactive */
    PORTC.DR.BIT.B5 = 1 ;

    /* Initialize SPI (per flowchart in hardware manual) */
    /* No loopback, CMos Output */
    RSPI0.SPPCR.BYTE = 0x00 ;
    /* Full Speed is 0x00  255 works */
    RSPI0.SPBR.BYTE = 0x00 ;
    /* 16-bit data 1 frame 1 chip select */
    RSPI0.SPDCR.BYTE = 0x00 ;
    /* 2 clock delay before next access to SPI device */
    RSPI0.SPCKD.BYTE = 0x00 ;
    /* 2 clock delay after de-asserting SSL */
    RSPI0.SSLND.BYTE = 0x00 ;
    /* 2 clock delay before next access to SPI device */
    RSPI0.SPND.BYTE = 0x00 ;
    /* No parity no idle interrupts */
    RSPI0.SPCR2.BYTE = 0x00 ;
    /* MSB first 8-bit data, keep SSL low */
    RSPI0.SPCMD0.WORD = 0x0700 ;
    /* Enable RSPI 3wire in master mode with RSPI Enable Transmit Only and Interupt */
    RSPI0.SPCR.BYTE = 0x6B ;
    /* SSL3A Polarity */
    RSPI0.SSLP.BYTE = 0x08 ;
    /* One frame. */
    RSPI0.SPSCR.BYTE = 0x00 ;
}

/******************************************************************************
* ID : 62.0
* Outline : YRDKRX62N_RSPIStart
* Include : YRDKRX62N_RSPI0.h
* Function Name: YRDKRX62N_RSPIStart
* Description : Sets the Chip Select Port to Low.
* Argument : none
* Return Value : none
* Calling Functions : YRDKRX62N_RSPITransferOutWords
******************************************************************************/
void YRDKRX62N_RSPIStart(void)
{
    /* Assert chip select */
    PORTC.DR.BIT.B2 = 0 ;
}

/******************************************************************************
* ID : 63.0
* Outline : YRDKRX62N_RSPIEnd
* Include : YRDKRX62N_RSPI0.h
* Function Name: YRDKRX62N_RSPIEnd
* Description : Sets the Chip Select Port to High.
* Argument : none
* Return Value : none
* Calling Functions : YRDKRX62N_RSPITransferOutWords
******************************************************************************/
void YRDKRX62N_RSPIEnd(void)
{
    /* De-assert chip select */
    PORTC.DR.BIT.B2 = 1 ;
}

/******************************************************************************
* ID : 64.0
* Outline : YRDKRX62N_RSPITransferOutWords
* Include : YRDKRX62N_RSPI0.h
* Function Name: YRDKRX62N_RSPITransferOutWords
* Description : Transmit out, the SPI Port 0, 32 Bits of data in Low and
* High format.
* Argument : sLowWord - first 8 bits to send
*          : sHighWord - second 8 bits to send
* Return Value : none
* Calling Functions : YRDKRX62N_RSPITransferOutWord8Bit
******************************************************************************/
void YRDKRX62N_RSPITransferOutWords(int16_t sLowWord, int16_t sHighWord)
{
    YRDKRX62N_RSPIStart() ;

    /* ensure transmit register is empty */
    while (RSPI0.SPSR.BIT.IDLNF) ;

    /* short 16 bits */
    RSPI0.SPDR.WORD.L = sLowWord ;
    /* short 16 bits */
    RSPI0.SPDR.WORD.H = sHighWord ;

    /* Wait for transfer to complete */
    while (RSPI0.SPSR.BIT.IDLNF) ;

    (void)RSPI0.SPDR.WORD.L;
    (void)RSPI0.SPDR.WORD.H;

    YRDKRX62N_RSPIEnd() ;
}

/******************************************************************************
* ID : 65.0
* Outline : YRDKRX62N_RSPITransferOutWord8Bit
* Include : YRDKRX62N_RSPI0.h
* Function Name: YRDKRX62N_RSPITransferOutWord8Bit
* Description : Transmit out, the SPI Port 0, 8 Bits of data.
* Argument : cData - 8 bits to send
* Return Value : none
* Calling Functions : T_Comm_API.iWrite
******************************************************************************/
void YRDKRX62N_RSPITransferOutWord8Bit(uint8_t cData)
{
    YRDKRX62N_RSPITransferOutWords(0, cData) ;
}

/******************************************************************************
* ID : 66.0
* Outline : SetSendCommand
* Include : YRDKRX62N_RSPI0.h
* Function Name: SetSendCommand
* Description : Set the RS line of the LCD.  The RS line is connected to
* output port P5.1.  Setting this output to 0 will allow data flow in the LCD.
* Argument : none
* Return Value : none
* Calling Functions : CommandSend, DataSend
******************************************************************************/
static void SetSendCommand(void)
{
    PORT5.DR.BIT.B1 = 0 ; // LCD RS A0
}

/******************************************************************************
* ID : 67.0
* Outline : SetSendData
* Include : YRDKRX62N_RSPI0.h
* Function Name: SetSendData
* Description : Clear the RS line of the LCD.  The RS line is connected to
* output port P5.1.  Setting this output to 1 will stop data flow in the LCD.
* When data flow is stopped in the LCD the data is internally copied to the
* data buffer for use.
* Argument : none
* Return Value : none
* Calling Functions : CommandSend, DataSend
******************************************************************************/
static void SetSendData(void)
{
    PORT5.DR.BIT.B1 = 1 ; // LCD RS A0
}

/******************************************************************************
* ID : 68.0
* Outline : StartResetLCD
* Include : YRDKRX62N_RSPI0.h
* Function Name: StartResetLCD
* Description : Set the A0 line of the LCD display.  This line is connected
* to output PC.3 of the RX62N.  This output is inversed.  Setting this output
* to 1 will cause the LCD to reset.
* Argument : none
* Return Value : none
* Calling Functions : ST7579_Config
******************************************************************************/
static void StartResetLCD(void)
{
    PORTC.DR.BIT.B3 = 1 ;
}

/******************************************************************************
* ID : 69.0
* Outline : EndResetLCD
* Include : YRDKRX62N_RSPI0.h
* Function Name: EndResetLCD
* Description : Clear the A0 line of the LCD display.  This line is connected
* to output PC.3 of the RX62N.  This output is inversed.  Setting this output
* to 0 will cause the LCD complete reset.  After a delay the LCD will be
* ready to utilize.
* Argument : none
* Return Value : none
* Calling Functions : ST7579_Config
******************************************************************************/
static void EndResetLCD(void)
{
    PORTC.DR.BIT.B3 = 0 ;
}

/******************************************************************************
* ID : 70.0
* Outline : YRDKRX62N_CommandSend
* Include : YRDKRX62N_RSPI0.h
* Function Name: YRDKRX62N_CommandSend
* Description : Send a command define to the LCD.  The defines for all commands
* available to send to the ST7579 LCD controller are defined in the header
* for this file RX62N_LCD.h.  The nCommand parameter should always be
* a define from that location.
* Argument : cCommand - 8 bits of data to be used as a command to the LCD
*          : aHandle - the Glyph handle setup by the LCD and Communications.
* Return Value : none
* Calling Functions : ST7579_Config, ST7579_SetSystemBiasBooster,
*                     ST7579_SetVO_Range, ST7579_SetFrameRate,
*                     ST7579_SetPage, ST7579_SetChar, ST7579_SetLine,
*                     ST7579_Send8bitsData, ST7579_Send16bitsCommand
******************************************************************************/
void YRDKRX62N_CommandSend(int8_t cCommand)
{
    /* Send Command To LCD */
    /* A0 = 0 */
    SetSendCommand() ;

    YRDKRX62N_RSPITransferOutWord8Bit(cCommand) ;

    /* A0 = 1 */
    SetSendData() ;
}

/******************************************************************************
* ID : 71.0
* Outline : YRDKRX62N_DataSend
* Include : YRDKRX62N_RSPI0.h
* Function Name: YRDKRX62N_DataSend
* Description : Send 8 bits of data to the LCD RAM at the current location.
* The current location is determined and set with local functions.
* Argument : cCommand - 8 bits of data to be sent to the LCD
*          : aHandle - the Glyph handle setup by the LCD and Communications.
* Return Value : none
* Calling Functions : ST7579_Config, ST7579_SetSystemBiasBooster,
*                     ST7579_SetVO_Range, ST7579_SetFrameRate,
*                     ST7579_SetPage, ST7579_SetChar, ST7579_SetLine,
*                     ST7579_Send8bitsData, ST7579_Send16bitsCommand
******************************************************************************/
void YRDKRX62N_DataSend(int8_t cData)
{
    /* A0 = 1 */
    SetSendData() ;

    YRDKRX62N_RSPITransferOutWord8Bit(cData) ;

    /* A0 = 1 */
    SetSendData() ;
}

