/*****************************************************************************
* Product:  Initialization for emWin/uC/GUI, Win32 simulation VC++
* Last Updated for Version: 5.1.1
* Date of the Last Update:  Nov 08, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          https://state-machine.com
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
