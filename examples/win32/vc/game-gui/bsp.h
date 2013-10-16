/*****************************************************************************
* Product: "Fly 'n' Shoot" game example, Win32-GUI
* Last Updated for Version: 5.0.0
* Date of the Last Update:  Aug 23, 2013
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
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef bsp_h
#define bsp_h

#define BSP_TICKS_PER_SEC    33U
#define BSP_SCREEN_WIDTH     96U
#define BSP_SCREEN_HEIGHT    16U

void BSP_init(void);
void BSP_drawBitmap(uint8_t const *bitmap);
void BSP_drawNString(uint8_t x,                              /* x in pixels */
                     uint8_t y,                      /* y position in chars */
                     char const *str);
void BSP_updateScore(uint16_t score);

void BSP_displayOn(void);
void BSP_displayOff(void);

void BSP_playerTrigger(void);
void BSP_moveShipUp(void);
void BSP_moveShipDown(void);

void BSP_terminate(int result);

/* Windows-GUI does not use the regular main() */
#define main()  main_gui()

#endif                                                             /* bsp_h */
