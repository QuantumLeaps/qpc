/*****************************************************************************
* Product:  Initialization for emWin/uC/GUI, Win32 simulation VC++ 6.0
* Last Updated for Version: 4.0.00
* Date of the Last Update:  May 31, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "SIM.h"
#include "GUI.h"

void SIM_X_Init() {
    if (((LCD_GetDevCap(LCD_DEVCAP_XSIZE) * LCD_GetXMag()) == 320)
        && ((LCD_GetDevCap(LCD_DEVCAP_YSIZE) * LCD_GetYMag()) == 240)
        && (LCD_GetNumLayers() == 1))
    {
        SIM_SetLCDPos(132, 46);    /* the position of the LCD in the bitmap */
        SIM_SetTransColor(0xFF0000);        /* define the transparent color */
        SIM_SetLCDColorBlack(0, 0x000000);      /* define the "black" color */
        SIM_SetLCDColorWhite(0, 0xC0C0C0);      /* define the "white" color */
    }
}
