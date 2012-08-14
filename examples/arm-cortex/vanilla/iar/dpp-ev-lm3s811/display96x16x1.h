//*****************************************************************************
//
// display96x16x1.h - Prototypes for the driver for the 96x16 monochrome
//                    graphical OLED displays found on the ek-lm3s811 board.
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

#ifndef __DISPLAY96X16X1_H__
#define __DISPLAY96X16X1_H__

//*****************************************************************************
//
// Prototypes for the driver APIs.
//
//*****************************************************************************
extern void Display96x16x1Clear(void);
extern void Display96x16x1StringDraw(const char *pcStr, unsigned long ulX,
                                   unsigned long ulY);
extern void Display96x16x1ImageDraw(const unsigned char *pucImage,
                                  unsigned long ulX, unsigned long ulY,
                                  unsigned long ulWidth,
                                  unsigned long ulHeight);
extern void Display96x16x1Init(unsigned char bFast);
extern void Display96x16x1DisplayOn(void);
extern void Display96x16x1DisplayOff(void);

#endif // __DISPLAY96X16X1_H__
