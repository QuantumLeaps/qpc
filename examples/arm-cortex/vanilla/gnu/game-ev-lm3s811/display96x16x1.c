//*****************************************************************************
// Modified for CMSIS by Quantum Leaps on Feb-17-2010
// www.state-machine.com
//*****************************************************************************

//*****************************************************************************
//
// display96x16x1.c - Driver for the 96x16 monochrome graphical OLED
//                    displays used on the ek-lm3s811 board.
//
// Copyright (c) 2006-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 5570 of the EK-LM3S811 Firmware Package.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include "lm3s_cmsis.h"
#include "display96x16x1.h"

//*****************************************************************************
// NOTE:
//
// This driver supports both the OSRAM/SSD0303 display used on the original
// ek-lm3s811 boards (orange display) and also the RIT/SSD1300 display
// used on later boards (blue display).  It determines which display is in use
// by looking at bit 12 of register DID1 which is set for all later kits but
// not the original version. This bit is specific to eval kits.  If you wish to
// use this driver on your own production board, build with #define OSRAM_ONLY
// or #define RIT_ONLY to select support for a single display and remove the
// DID1 check.
//
//*****************************************************************************

//*****************************************************************************
//
// The I2C slave address of the SSD controllers on the OLED displays.
//
//*****************************************************************************
#define SSD_ADDR            0x3d

//*****************************************************************************
//
// I2C Master commands.
//
//*****************************************************************************
#define I2C_MASTER_CMD_SINGLE_SEND              0x00000007
#define I2C_MASTER_CMD_SINGLE_RECEIVE           0x00000007
#define I2C_MASTER_CMD_BURST_SEND_START         0x00000003
#define I2C_MASTER_CMD_BURST_SEND_CONT          0x00000001
#define I2C_MASTER_CMD_BURST_SEND_FINISH        0x00000005
#define I2C_MASTER_CMD_BURST_SEND_ERROR_STOP    0x00000004
#define I2C_MASTER_CMD_BURST_RECEIVE_START      0x0000000b
#define I2C_MASTER_CMD_BURST_RECEIVE_CONT       0x00000009
#define I2C_MASTER_CMD_BURST_RECEIVE_FINISH     0x00000005
#define I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP 0x00000005

//*****************************************************************************
//
// A 5x7 font (in a 6x8 cell, where the sixth column is omitted from this
// table) for displaying text on the OLED display.  The data is organized as
// bytes from the left column to the right column, with each byte containing
// the top row in the LSB and the bottom row in the MSB.
//
//*****************************************************************************
static const unsigned char g_pucFont[95][5] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00 }, // " "
    { 0x00, 0x00, 0x4f, 0x00, 0x00 }, // !
    { 0x00, 0x07, 0x00, 0x07, 0x00 }, // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 }, // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 }, // $
    { 0x23, 0x13, 0x08, 0x64, 0x62 }, // %
    { 0x36, 0x49, 0x55, 0x22, 0x50 }, // &
    { 0x00, 0x05, 0x03, 0x00, 0x00 }, // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00 }, // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00 }, // )
    { 0x14, 0x08, 0x3e, 0x08, 0x14 }, // *
    { 0x08, 0x08, 0x3e, 0x08, 0x08 }, // +
    { 0x00, 0x50, 0x30, 0x00, 0x00 }, // ,
    { 0x08, 0x08, 0x08, 0x08, 0x08 }, // -
    { 0x00, 0x60, 0x60, 0x00, 0x00 }, // .
    { 0x20, 0x10, 0x08, 0x04, 0x02 }, // /
    { 0x3e, 0x51, 0x49, 0x45, 0x3e }, // 0
    { 0x00, 0x42, 0x7f, 0x40, 0x00 }, // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 }, // 2
    { 0x21, 0x41, 0x45, 0x4b, 0x31 }, // 3
    { 0x18, 0x14, 0x12, 0x7f, 0x10 }, // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 }, // 5
    { 0x3c, 0x4a, 0x49, 0x49, 0x30 }, // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 }, // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 }, // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1e }, // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00 }, // :
    { 0x00, 0x56, 0x36, 0x00, 0x00 }, // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00 }, // <
    { 0x14, 0x14, 0x14, 0x14, 0x14 }, // =
    { 0x00, 0x41, 0x22, 0x14, 0x08 }, // >
    { 0x02, 0x01, 0x51, 0x09, 0x06 }, // ?
    { 0x32, 0x49, 0x79, 0x41, 0x3e }, // @
    { 0x7e, 0x11, 0x11, 0x11, 0x7e }, // A
    { 0x7f, 0x49, 0x49, 0x49, 0x36 }, // B
    { 0x3e, 0x41, 0x41, 0x41, 0x22 }, // C
    { 0x7f, 0x41, 0x41, 0x22, 0x1c }, // D
    { 0x7f, 0x49, 0x49, 0x49, 0x41 }, // E
    { 0x7f, 0x09, 0x09, 0x09, 0x01 }, // F
    { 0x3e, 0x41, 0x49, 0x49, 0x7a }, // G
    { 0x7f, 0x08, 0x08, 0x08, 0x7f }, // H
    { 0x00, 0x41, 0x7f, 0x41, 0x00 }, // I
    { 0x20, 0x40, 0x41, 0x3f, 0x01 }, // J
    { 0x7f, 0x08, 0x14, 0x22, 0x41 }, // K
    { 0x7f, 0x40, 0x40, 0x40, 0x40 }, // L
    { 0x7f, 0x02, 0x0c, 0x02, 0x7f }, // M
    { 0x7f, 0x04, 0x08, 0x10, 0x7f }, // N
    { 0x3e, 0x41, 0x41, 0x41, 0x3e }, // O
    { 0x7f, 0x09, 0x09, 0x09, 0x06 }, // P
    { 0x3e, 0x41, 0x51, 0x21, 0x5e }, // Q
    { 0x7f, 0x09, 0x19, 0x29, 0x46 }, // R
    { 0x46, 0x49, 0x49, 0x49, 0x31 }, // S
    { 0x01, 0x01, 0x7f, 0x01, 0x01 }, // T
    { 0x3f, 0x40, 0x40, 0x40, 0x3f }, // U
    { 0x1f, 0x20, 0x40, 0x20, 0x1f }, // V
    { 0x3f, 0x40, 0x38, 0x40, 0x3f }, // W
    { 0x63, 0x14, 0x08, 0x14, 0x63 }, // X
    { 0x07, 0x08, 0x70, 0x08, 0x07 }, // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 }, // Z
    { 0x00, 0x7f, 0x41, 0x41, 0x00 }, // [
    { 0x02, 0x04, 0x08, 0x10, 0x20 }, // "\"
    { 0x00, 0x41, 0x41, 0x7f, 0x00 }, // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 }, // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40 }, // _
    { 0x00, 0x01, 0x02, 0x04, 0x00 }, // `
    { 0x20, 0x54, 0x54, 0x54, 0x78 }, // a
    { 0x7f, 0x48, 0x44, 0x44, 0x38 }, // b
    { 0x38, 0x44, 0x44, 0x44, 0x20 }, // c
    { 0x38, 0x44, 0x44, 0x48, 0x7f }, // d
    { 0x38, 0x54, 0x54, 0x54, 0x18 }, // e
    { 0x08, 0x7e, 0x09, 0x01, 0x02 }, // f
    { 0x0c, 0x52, 0x52, 0x52, 0x3e }, // g
    { 0x7f, 0x08, 0x04, 0x04, 0x78 }, // h
    { 0x00, 0x44, 0x7d, 0x40, 0x00 }, // i
    { 0x20, 0x40, 0x44, 0x3d, 0x00 }, // j
    { 0x7f, 0x10, 0x28, 0x44, 0x00 }, // k
    { 0x00, 0x41, 0x7f, 0x40, 0x00 }, // l
    { 0x7c, 0x04, 0x18, 0x04, 0x78 }, // m
    { 0x7c, 0x08, 0x04, 0x04, 0x78 }, // n
    { 0x38, 0x44, 0x44, 0x44, 0x38 }, // o
    { 0x7c, 0x14, 0x14, 0x14, 0x08 }, // p
    { 0x08, 0x14, 0x14, 0x18, 0x7c }, // q
    { 0x7c, 0x08, 0x04, 0x04, 0x08 }, // r
    { 0x48, 0x54, 0x54, 0x54, 0x20 }, // s
    { 0x04, 0x3f, 0x44, 0x40, 0x20 }, // t
    { 0x3c, 0x40, 0x40, 0x20, 0x7c }, // u
    { 0x1c, 0x20, 0x40, 0x20, 0x1c }, // v
    { 0x3c, 0x40, 0x30, 0x40, 0x3c }, // w
    { 0x44, 0x28, 0x10, 0x28, 0x44 }, // x
    { 0x0c, 0x50, 0x50, 0x50, 0x3c }, // y
    { 0x44, 0x64, 0x54, 0x4c, 0x44 }, // z
    { 0x00, 0x08, 0x36, 0x41, 0x00 }, // {
    { 0x00, 0x00, 0x7f, 0x00, 0x00 }, // |
    { 0x00, 0x41, 0x36, 0x08, 0x00 }, // }
    { 0x02, 0x01, 0x02, 0x04, 0x02 }, // ~
};

//*****************************************************************************
//
// The sequence of commands used to initialize the SSD0303 controller found on
// the OSRAM displays used with earlier ek-lm3s811 boards.  Each command is
// described as follows:  there is a byte specifying the number of bytes in the
// I2C transfer, followed by that many bytes of command data.
//
//*****************************************************************************
#ifndef RIT_ONLY
static const unsigned char g_pucOSRAMInit[] =
{
    //
    // Turn off the panel
    //
    0x04, 0x80, 0xae, 0x80, 0xe3,

    //
    // Set lower column address
    //
    0x04, 0x80, 0x04, 0x80, 0xe3,

    //
    // Set higher column address
    //
    0x04, 0x80, 0x12, 0x80, 0xe3,

    //
    // Set contrast control register
    //
    0x06, 0x80, 0x81, 0x80, 0x2b, 0x80, 0xe3,

    //
    // Set segment re-map
    //
    0x04, 0x80, 0xa1, 0x80, 0xe3,

    //
    // Set display start line
    //
    0x04, 0x80, 0x40, 0x80, 0xe3,

    //
    // Set display offset
    //
    0x06, 0x80, 0xd3, 0x80, 0x00, 0x80, 0xe3,

    //
    // Set multiplex ratio
    //
    0x06, 0x80, 0xa8, 0x80, 0x0f, 0x80, 0xe3,

    //
    // Set the display to normal mode
    //
    0x04, 0x80, 0xa4, 0x80, 0xe3,

    //
    // Non-inverted display
    //
    0x04, 0x80, 0xa6, 0x80, 0xe3,

    //
    // Set the page address
    //
    0x04, 0x80, 0xb0, 0x80, 0xe3,

    //
    // Set COM output scan direction
    //
    0x04, 0x80, 0xc8, 0x80, 0xe3,

    //
    // Set display clock divide ratio/oscillator frequency
    //
    0x06, 0x80, 0xd5, 0x80, 0x72, 0x80, 0xe3,

    //
    // Enable mono mode
    //
    0x06, 0x80, 0xd8, 0x80, 0x00, 0x80, 0xe3,

    //
    // Set pre-charge period
    //
    0x06, 0x80, 0xd9, 0x80, 0x22, 0x80, 0xe3,

    //
    // Set COM pins hardware configuration
    //
    0x06, 0x80, 0xda, 0x80, 0x12, 0x80, 0xe3,

    //
    // Set VCOM deslect level
    //
    0x06, 0x80, 0xdb, 0x80, 0x0f, 0x80, 0xe3,

    //
    // Set DC-DC on
    //
    0x06, 0x80, 0xad, 0x80, 0x8b, 0x80, 0xe3,

    //
    // Turn on the panel
    //
    0x04, 0x80, 0xaf, 0x80, 0xe3,
};
#endif

//*****************************************************************************
//
// The sequence of commands used to initialize the SSD1300 controller as found
// on the RIT displays used with later ek-lm3s811 boards.
//
//*****************************************************************************
#ifndef OSRAM_ONLY
static const unsigned char g_pucRITInit[] =
{
    //New Display display

    //
    // Turn off the panel
    //
    0x04, 0x80, 0xae, 0x80, 0xe3,

    //
    // Internal dc/dc on/off
    //
    0x06, 0x80, 0xad, 0x80, 0x8a, 0x80, 0xe3,

    //
    // Multiplex ratio
    //
    0x06, 0x80, 0xa8, 0x80, 0x1f, 0x80, 0xe3,

    //
    // COM out scan direction
    //
    0x04, 0x80, 0xc8, 0x80, 0xe3,

    //
    // Segment map
    //
    0x04, 0x80, 0xa0, 0x80, 0xe3,


    //
    // Set area color mode
    //
    0x04, 0x80, 0xd8, 0x80, 0xe3,

    //
    // Low power save mode
    //
    0x04, 0x80, 0x05, 0x80, 0xe3,

    //
    // Start line
    //
    0x04, 0x80, 0x40, 0x80, 0xe3,

    //
    // Contrast setting
    //
    0x06, 0x80, 0x81, 0x80, 0x5d, 0x80, 0xe3,

    //
    // Pre-charge/discharge
    //
    0x06, 0x80, 0xd9, 0x80, 0x11, 0x80, 0xe3,

    //
    // Set display clock
    //
    0x06, 0x80, 0xd5, 0x80, 0x01, 0x80, 0xe3,

    //
    // Display offset
    //
    0x06, 0x80, 0xd3, 0x80, 0x00, 0x80, 0xe3,

    //
    // Display off
    //
    0x04, 0x80, 0xaf, 0x80, 0xe3,
};
#endif

//*****************************************************************************
//
// Macro used to select the appropriate display initialization commands.
//
//*****************************************************************************
#ifdef OSRAM_ONLY
#define g_pucDisplayInit g_pucOSRAMInit
#define SIZE_INIT_CMDS (sizeof(g_pucOSRAMInit))
#else
#ifdef RIT_ONLY
#define g_pucDisplayInit g_pucRITInit
#define SIZE_INIT_CMDS (sizeof(g_pucRITInit))
#else
#define g_pucDisplayInit (g_ucDisplayIsRIT ? g_pucRITInit : g_pucOSRAMInit)
#define SIZE_INIT_CMDS (g_ucDisplayIsRIT ? sizeof(g_pucRITInit) :             \
                        sizeof(g_pucOSRAMInit))
#endif
#endif

//*****************************************************************************
//
// The sequence of commands used to set the cursor to the first column of the
// first and second rows of the display for each of the supported displays.
//
//*****************************************************************************
#ifndef RIT_ONLY
static const unsigned char g_pucOSRAMRow1[] =
{
    0xb0, 0x80, 0x04, 0x80, 0x12, 0x40
};
static const unsigned char g_pucOSRAMRow2[] =
{
    0xb1, 0x80, 0x04, 0x80, 0x12, 0x40
};
#endif
#ifndef OSRAM_ONLY
static const unsigned char g_pucRITRow1[] =
{
    0xb0, 0x80, 0x04, 0x80, 0x10, 0x40
};
static const unsigned char g_pucRITRow2[] =
{
    0xb1, 0x80, 0x04, 0x80, 0x10, 0x40
};
#endif

//*****************************************************************************
//
// The number of bytes in the cursor row command.
//
//*****************************************************************************
#define SIZE_CURSOR_ROW_COMMAND 6

//*****************************************************************************
//
// Macros used to select the appropriate cursor row setting commands.
//
//*****************************************************************************
#ifdef OSRAM_ONLY
#define g_pucRow1 g_pucOSRAMRow1
#define g_pucRow2 g_pucOSRAMRow2
#else
#ifdef RIT_ONLY
#define g_pucRow1 g_pucRITRow1
#define g_pucRow2 g_pucRITRow2
#else
#define g_pucRow1 (g_ucDisplayIsRIT ? g_pucRITRow1 : g_pucOSRAMRow1)
#define g_pucRow2 (g_ucDisplayIsRIT ? g_pucRITRow2 : g_pucOSRAMRow2)
#endif
#endif

//*****************************************************************************
//
// The number of non-displayed columns to the left of the display area.
//
//*****************************************************************************
#ifdef OSRAM_ONLY
//
// If built for the OSRAM display only, there are 4 non-displayed columns.
//
#define g_ucColumnAdjust 36
#else
#ifdef RIT_ONLY
//
// If built for the OSRAM display only, there are 36 non-displayed columns.
//
#define g_ucColumnAdjust 4
#else
//
// If built to sense the display type and handle whichever is found, we allocate
// variables to store the number of non-displayed columns and a flag indicating
// which display is in use.
//
static unsigned char g_ucDisplayIsRIT;
static unsigned char g_ucColumnAdjust;
#endif
#endif

//*****************************************************************************
//
// The inter-byte delay required by the display OLED controller.
//
//*****************************************************************************
static unsigned long g_ulDelay;

//*****************************************************************************
void
SysCtlDelay(unsigned long ulCount)
{
    __asm("    subs    r0, #1\n"
          "    bne.n   SysCtlDelay\n"
          "    bx      lr");
}

//*****************************************************************************
//
//! \internal
//!
//! Start a transfer to the SSD0303 or SSD1300 controller.
//!
//! \param ucChar is the first byte to be written to the controller.
//!
//! This function will start a transfer to the display controller via the I2C
//! bus.
//!
//! The data is written in a polled fashion; this function will not return
//! until the byte has been written to the controller.
//!
//! \return None.
//
//*****************************************************************************
static void
Display96x16x1WriteFirst(unsigned char ucChar)
{
    //
    // Set the slave address.
    //
    I2C0_MASTER->MSA  = (SSD_ADDR << 1);

    //
    // Write the first byte to the controller.
    //
    I2C0_MASTER->MDR = ucChar;

    //
    // Start the transfer.
    //
    I2C0_MASTER->MCS = I2C_MASTER_CMD_BURST_SEND_START;
}

//*****************************************************************************
//
//! \internal
//!
//! Write a byte to the SSD0303 or SSD1300 controller.
//!
//! \param ucChar is the byte to be transmitted to the controller.
//!
//! This function continues a transfer to the display controller by writing
//! another byte over the I2C bus.  This must only be called after calling
//! Display96x16x1WriteFirst(), but before calling Display96x16x1WriteFinal().
//!
//! The data is written in a polled faashion; this function will not return
//! until the byte has been written to the controller.
//!
//! \return None.
//
//*****************************************************************************
static void
Display96x16x1WriteByte(unsigned char ucChar)
{
    //
    // Wait until the current byte has been transferred.
    //
    while(I2C0_MASTER->MRIS == 0)
    {
    }

    //
    // Provide the required inter-byte delay.
    //
    SysCtlDelay(g_ulDelay);

    //
    // Write the next byte to the controller.
    //
    I2C0_MASTER->MDR = ucChar;

    //
    // Continue the transfer.
    //
    I2C0_MASTER->MCS = I2C_MASTER_CMD_BURST_SEND_CONT;
}

//*****************************************************************************
//
//! \internal
//!
//! Write a sequence of bytes to the SSD0303 or SD1300 controller.
//!
//! This function continues a transfer to the display controller by writing a
//! sequence of bytes over the I2C bus.  This must only be called after calling
//! Display96x16x1WriteFirst(), but before calling Display96x16x1WriteFinal().
//!
//! The data is written in a polled fashion; this function will not return
//! until the entire byte sequence has been written to the controller.
//!
//! \return None.
//
//*****************************************************************************
static void
Display96x16x1WriteArray(const unsigned char *pucBuffer, unsigned long ulCount)
{
    //
    // Loop while there are more bytes left to be transferred.
    //
    while(ulCount != 0)
    {
        //
        // Wait until the current byte has been transferred.
        //
        while(I2C0_MASTER->MRIS == 0)
        {
        }

        //
        // Provide the required inter-byte delay.
        //
        SysCtlDelay(g_ulDelay);

        //
        // Write the next byte to the controller.
        //
        I2C0_MASTER->MDR = *pucBuffer++;
        ulCount--;

        //
        // Continue the transfer.
        //
        I2C0_MASTER->MCS = I2C_MASTER_CMD_BURST_SEND_CONT;
    }
}

//*****************************************************************************
//
//! \internal
//!
//! Finish a transfer to the SSD0303 or SD1300 controller.
//!
//! \param ucChar is the final byte to be written to the controller.
//!
//! This function will finish a transfer to the display controller via the I2C
//! bus.  This must only be called after calling Display96x16x1WriteFirst().
//!
//! The data is written in a polled fashion; this function will not return
//! until the byte has been written to the controller.
//!
//! \return None.
//
//*****************************************************************************
static void
Display96x16x1WriteFinal(unsigned char ucChar)
{
    //
    // Wait until the current byte has been transferred.
    //
    while(I2C0_MASTER->MRIS == 0)
    {
    }

    //
    // Provide the required inter-byte delay.
    //
    SysCtlDelay(g_ulDelay);

    //
    // Write the final byte to the controller.
    //
    I2C0_MASTER->MDR = ucChar;

    //
    // Finish the transfer.
    //
    I2C0_MASTER->MCS = I2C_MASTER_CMD_BURST_SEND_FINISH;

    //
    // Wait until the final byte has been transferred.
    //
    while(I2C0_MASTER->MRIS == 0)
    {
    }

    //
    // Provide the required inter-byte delay.
    //
    SysCtlDelay(g_ulDelay);
}

//*****************************************************************************
//
//! Clears the OLED display.
//!
//! This function will clear the display.  All pixels in the display will be
//! turned off.
//!
//! \return None.
//
//*****************************************************************************
void
Display96x16x1Clear(void)
{
    unsigned long ulIdx;

    //
    // Move the display cursor to the first column of the first row.
    //
    Display96x16x1WriteFirst(0x80);
    Display96x16x1WriteArray(g_pucRow1, SIZE_CURSOR_ROW_COMMAND);

    //
    // Fill this row with zeros.
    //
    for(ulIdx = 0; ulIdx < 95; ulIdx++)
    {
        Display96x16x1WriteByte(0x00);
    }
    Display96x16x1WriteFinal(0x00);

    //
    // Move the display cursor to the first column of the second row.
    //
    Display96x16x1WriteFirst(0x80);
    Display96x16x1WriteArray(g_pucRow2, SIZE_CURSOR_ROW_COMMAND);

    //
    // Fill this row with zeros.
    //
    for(ulIdx = 0; ulIdx < 95; ulIdx++)
    {
        Display96x16x1WriteByte(0x00);
    }
    Display96x16x1WriteFinal(0x00);
}

//*****************************************************************************
//
//! Displays a string on the OLED display.
//!
//! \param pcStr is a pointer to the string to display.
//! \param ulX is the horizontal position to display the string, specified in
//! columns from the left edge of the display.
//! \param ulY is the vertical position to display the string, specified in
//! eight scan line blocks from the top of the display (that is, only 0 and 1
//! are valid).
//!
//! This function will draw a string on the display.  Only the ASCII characters
//! between 32 (space) and 126 (tilde) are supported; other characters will
//! result in random data being draw on the display (based on whatever appears
//! before/after the font in memory).  The font is mono-spaced, so characters
//! such as ``i'' and ``l'' have more white space around them than characters
//! such as ``m'' or ``w''.
//!
//! If the drawing of the string reaches the right edge of the display, no more
//! characters will be drawn.  Therefore, special care is not required to avoid
//! supplying a string that is ``too long'' to display.
//!
//! \return None.
//
//*****************************************************************************
void
Display96x16x1StringDraw(const char *pcStr, unsigned long ulX, unsigned long ulY)
{

    //
    // Move the display cursor to the requested position on the display.
    //
    Display96x16x1WriteFirst(0x80);
    Display96x16x1WriteByte((ulY == 0) ? 0xb0 : 0xb1);
    Display96x16x1WriteByte(0x80);
    Display96x16x1WriteByte((ulX + g_ucColumnAdjust) & 0x0f);
    Display96x16x1WriteByte(0x80);
    Display96x16x1WriteByte(0x10 | (((ulX + g_ucColumnAdjust) >> 4) & 0x0f));
    Display96x16x1WriteByte(0x40);

    //
    // Loop while there are more characters in the string.
    //
    while(*pcStr != 0)
    {
        //
        // See if there is enough space on the display for this entire
        // character.
        //
        if(ulX <= 90)
        {
            //
            // Write the contents of this character to the display.
            //
            Display96x16x1WriteArray(g_pucFont[*pcStr - ' '], 5);

            //
            // See if this is the last character to display (either because the
            // right edge has been reached or because there are no more
            // characters).
            //
            if((ulX == 90) || (pcStr[1] == 0))
            {
                //
                // Write the final column of the display.
                //
                Display96x16x1WriteFinal(0x00);

                //
                // The string has been displayed.
                //
                return;
            }

            //
            // Write the inter-character padding column.
            //
            Display96x16x1WriteByte(0x00);
        }
        else
        {
            //
            // Write the portion of the character that will fit onto the
            // display.
            //
            Display96x16x1WriteArray(g_pucFont[*pcStr - ' '], 95 - ulX);
            Display96x16x1WriteFinal(g_pucFont[*pcStr - ' '][95 - ulX]);

            //
            // The string has been displayed.
            //
            return;
        }

        //
        // Advance to the next character.
        //
        pcStr++;

        //
        // Increment the X coordinate by the six columns that were just
        // written.
        //
        ulX += 6;
    }
}

//*****************************************************************************
//
//! Displays an image on the OLED display.
//!
//! \param pucImage is a pointer to the image data.
//! \param ulX is the horizontal position to display this image, specified in
//! columns from the left edge of the display.
//! \param ulY is the vertical position to display this image, specified in
//! eight scan line blocks from the top of the display (that is, only 0 and 1
//! are valid).
//! \param ulWidth is the width of the image, specified in columns.
//! \param ulHeight is the height of the image, specified in eight row blocks
//! (that is, only 1 and 2 are valid).
//!
//! This function will display a bitmap graphic on the display.  The image to
//! be displayed must be a multiple of eight scan lines high (that is, one row)
//! and will be drawn at a vertical position that is a multiple of eight scan
//! lines (that is, scan line zero or scan line eight, corresponding to row
//! zero or row one).
//!
//! The image data is organized with the first row of image data appearing left
//! to right, followed immediately by the second row of image data.  Each byte
//! contains the data for the eight scan lines of the column, with the top scan
//! line being in the least significant bit of the byte and the bottom scan
//! line being in the most significant bit of the byte.
//!
//! For example, an image four columns wide and sixteen scan lines tall would
//! be arranged as follows (showing how the eight bytes of the image would
//! appear on the display):
//!
//! \verbatim
//!     +-------+  +-------+  +-------+  +-------+
//!     |   | 0 |  |   | 0 |  |   | 0 |  |   | 0 |
//!     | B | 1 |  | B | 1 |  | B | 1 |  | B | 1 |
//!     | y | 2 |  | y | 2 |  | y | 2 |  | y | 2 |
//!     | t | 3 |  | t | 3 |  | t | 3 |  | t | 3 |
//!     | e | 4 |  | e | 4 |  | e | 4 |  | e | 4 |
//!     |   | 5 |  |   | 5 |  |   | 5 |  |   | 5 |
//!     | 0 | 6 |  | 1 | 6 |  | 2 | 6 |  | 3 | 6 |
//!     |   | 7 |  |   | 7 |  |   | 7 |  |   | 7 |
//!     +-------+  +-------+  +-------+  +-------+
//!
//!     +-------+  +-------+  +-------+  +-------+
//!     |   | 0 |  |   | 0 |  |   | 0 |  |   | 0 |
//!     | B | 1 |  | B | 1 |  | B | 1 |  | B | 1 |
//!     | y | 2 |  | y | 2 |  | y | 2 |  | y | 2 |
//!     | t | 3 |  | t | 3 |  | t | 3 |  | t | 3 |
//!     | e | 4 |  | e | 4 |  | e | 4 |  | e | 4 |
//!     |   | 5 |  |   | 5 |  |   | 5 |  |   | 5 |
//!     | 4 | 6 |  | 5 | 6 |  | 6 | 6 |  | 7 | 6 |
//!     |   | 7 |  |   | 7 |  |   | 7 |  |   | 7 |
//!     +-------+  +-------+  +-------+  +-------+
//! \endverbatim
//!
//! \return None.
//
//*****************************************************************************
void
Display96x16x1ImageDraw(const unsigned char *pucImage, unsigned long ulX,
                      unsigned long ulY, unsigned long ulWidth,
                      unsigned long ulHeight)
{
    //
    // The first few columns of the LCD buffer are not displayed, so increment
    // the X coorddinate by this amount to account for the non-displayed frame
    // buffer memory.
    //
    ulX += g_ucColumnAdjust;

    //
    // Loop while there are more rows to display.
    //
    while(ulHeight--)
    {
        //
        // Write the starting address within this row.
        //
        Display96x16x1WriteFirst(0x80);
        Display96x16x1WriteByte((ulY == 0) ? 0xb0 : 0xb1);
        Display96x16x1WriteByte(0x80);
        Display96x16x1WriteByte(ulX & 0x0f);
        Display96x16x1WriteByte(0x80);
        Display96x16x1WriteByte(0x10 | ((ulX >> 4) & 0x0f));
        Display96x16x1WriteByte(0x40);

        //
        // Write this row of image data.
        //
        Display96x16x1WriteArray(pucImage, ulWidth - 1);
        Display96x16x1WriteFinal(pucImage[ulWidth - 1]);

        //
        // Advance to the next row of the image.
        //
        pucImage += ulWidth;
        ulY++;
    }
}

//*****************************************************************************
//
//! Initialize the OLED display.
//!
//! \param bFast is a boolean that is \e true if the I2C interface should be
//! run at 400 kbps and \e false if it should be run at 100 kbps.
//!
//! This function initializes the I2C interface to the OLED display and
//! configures the SSD0303 or SSD1300 controller on the panel.
//!
//! \return None.
//
//*****************************************************************************
void
Display96x16x1Init(unsigned char bFast)
{
    unsigned long ulTmp;

    //
    // Enable the I2C and GPIO port B blocks as they are needed by this driver.
    //
    SYSCTL->RCGC1 |= (1 << 12);                    /* enable clock to I2C0  */
    SYSCTL->RCGC2 |= (1 <<  1);                    /* enable clock to GPIOB */

#if (!(defined OSRAM_ONLY) && !(defined RIT_ONLY))
    //
    // Read SysCtl DID1 register to determine whether this is an older board
    // with the OSRAM display or a newer one with the RIT model.
    //
    g_ucDisplayIsRIT = (SYSCTL->DID1 & (1 << 12)) ? 1 : 0;

    //
    // Set the correct number of non-displayed columns given the display type
    // we are using.
    //
    g_ucColumnAdjust = g_ucDisplayIsRIT ? 4 : 36;
#endif

    //
    // If using the RIT display, we need to enable power by pulling PD7 high.
    //
#ifndef OSRAM_ONLY
#ifndef RIT_ONLY
    if(g_ucDisplayIsRIT)
    {
#endif
        SYSCTL->RCGC2 |= (1 <<  3);                /* enable clock to GPIOD */
        SysCtlDelay(1);            /* wait a tiny bit after enabling clocks */

        GPIOD->DIR |= (1 << 7);       /* set GPIOD-pin7 direction to output */
        GPIOD->DATA_Bits[1 << 7] = (1 << 7);       /* drive GPIOD-pin7 high */
#ifndef RIT_ONLY
    }
#endif
#endif

    //
    // Configure the I2C SCL and SDA pins for I2C operation.
    //
    ulTmp = (1 << 2) | (1 << 3);
    GPIOB->DIR   &= ~ulTmp;
    GPIOB->AFSEL |= ulTmp;
    GPIOB->DR2R  |= ulTmp;      /* set 2mA drive, DR4R and DR8R are cleared */
    GPIOB->SLR   &= ~ulTmp;
    GPIOB->ODR   |= ulTmp;
    GPIOB->PUR   |= ulTmp;              /* set weak pull-up; PDR is cleared */
    GPIOB->DEN   |= ulTmp;
    GPIOB->AMSEL &= ~ulTmp;

    //
    // Initialize the I2C master.
    //
    I2C0_MASTER->MCR |= (1 << 4);                      /* I2C master enable */
    if (bFast) {
        ulTmp = 400000;
    }
    else {
        ulTmp = 100000;
    }
    I2C0_MASTER->MTPR = ((SystemFrequency + (2 * 10 * ulTmp) - 1)
                          / (2 * 10 * ulTmp)) - 1;

    //
    // Compute the inter-byte delay for the display controller.  This delay is
    // dependent upon the I2C bus clock rate; the slower the clock the longer
    // the delay required.
    //
    // The derivation of this formula is based on a measured delay of
    // SysCtlDelay(1700) for a 100 kHz I2C bus with the CPU running at 50 MHz
    // (referred to as C).  To scale this to the delay for a different CPU
    // speed (since this is just a CPU-based delay loop) is:
    //
    //           f(CPU)
    //     C * ----------
    //         50,000,000
    //
    // To then scale this to the actual I2C rate (since it won't always be
    // precisely 100 kHz):
    //
    //           f(CPU)     100,000
    //     C * ---------- * -------
    //         50,000,000    f(I2C)
    //
    // This equation will give the inter-byte delay required for any
    // configuration of the I2C master.  But, as arranged it is impossible to
    // directly compute in 32-bit arithmetic (without loosing a lot of
    // accuracy).  So, the equation is simplified.
    //
    // Since f(I2C) is generated by dividing down from f(CPU), replace it with
    // the equivalent (where TPR is the value programmed into the Master Timer
    // Period Register of the I2C master, with the 1 added back):
    //
    //                        100,000
    //           f(CPU)       -------
    //     C * ---------- *    f(CPU)
    //         50,000,000   ------------
    //                      2 * 10 * TPR
    //
    // Inverting the dividend in the last term:
    //
    //           f(CPU)     100,000 * 2 * 10 * TPR
    //     C * ---------- * ----------------------
    //         50,000,000          f(CPU)
    //
    // The f(CPU) now cancels out.
    //
    //         100,000 * 2 * 10 * TPR
    //     C * ----------------------
    //               50,000,000
    //
    // Since there are no clock frequencies left in the equation, this equation
    // also works for 400 kHz bus operation as well, since the 100,000 in the
    // numerator becomes 400,000 but C is 1/4, which cancel out each other.
    // Reducing the constants gives:
    //
    //         TPR              TPR             TPR
    //     C * ---   =   1700 * ---   =   340 * ---   = 68 * TPR
    //         25               25               5
    //
    // Note that the constant C is actually a bit larger than it needs to be in
    // order to provide some safety margin.
    //
    g_ulDelay = 68 * (I2C0_MASTER->MTPR + 1);

    //
    // Initialize the display controller.  Loop through the initialization
    // sequence doing a single I2C transfer for each command.
    //
    for(ulTmp = 0; ulTmp < SIZE_INIT_CMDS; ulTmp += g_pucDisplayInit[ulTmp] + 1)
    {
        //
        // Send this command.
        //
        Display96x16x1WriteFirst(g_pucDisplayInit[ulTmp + 1]);
        Display96x16x1WriteArray(g_pucDisplayInit + ulTmp + 2,
                                 g_pucDisplayInit[ulTmp] - 2);
        Display96x16x1WriteFinal(g_pucDisplayInit[ulTmp
                                 + g_pucDisplayInit[ulTmp]]);
    }

    //
    // Clear the frame buffer.
    //
    Display96x16x1Clear();
}

//*****************************************************************************
//
//! Turns on the OLED display.
//!
//! This function will turn on the OLED display, causing it to display the
//! contents of its internal frame buffer.
//!
//! \return None.
//
//*****************************************************************************
void
Display96x16x1DisplayOn(void)
{
    unsigned long ulIdx;

    //
    // Re-initialize the display controller.  Loop through the initialization
    // sequence doing a single I2C transfer for each command.
    //
    for(ulIdx = 0; ulIdx < SIZE_INIT_CMDS;
        ulIdx += g_pucDisplayInit[ulIdx] + 1)
    {
        //
        // Send this command.
        //
        Display96x16x1WriteFirst(g_pucDisplayInit[ulIdx + 1]);
        Display96x16x1WriteArray(g_pucDisplayInit + ulIdx + 2,
                               g_pucDisplayInit[ulIdx] - 2);
        Display96x16x1WriteFinal(g_pucDisplayInit[ulIdx + g_pucDisplayInit[ulIdx]]);
    }
}

//*****************************************************************************
//
//! Turns off the OLED display.
//!
//! This function will turn off the OLED display.  This will stop the scanning
//! of the panel and turn off the on-chip DC-DC converter, preventing damage to
//! the panel due to burn-in (it has similar characters to a CRT in this
//! respect).
//!
//! \return None.
//
//*****************************************************************************
void
Display96x16x1DisplayOff(void)
{
    //
    // Turn off the DC-DC converter and the display.
    //
    Display96x16x1WriteFirst(0x80);
    Display96x16x1WriteByte(0xae);
    Display96x16x1WriteByte(0x80);
    Display96x16x1WriteByte(0xad);
    Display96x16x1WriteByte(0x80);
    Display96x16x1WriteFinal(0x8a);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
