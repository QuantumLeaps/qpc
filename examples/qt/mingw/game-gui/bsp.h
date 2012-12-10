/*****************************************************************************
* Product: "Fly'n'Shoot" game, BSP interface
* Last Updated for Version: 4.5.01
* Date of the Last Update:  Jun 14, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
#ifndef bsp_h
#define bsp_h

#define BSP_TICKS_PER_SEC    33U
#define BSP_SCREEN_WIDTH     96U
#define BSP_SCREEN_HEIGHT    16U

void BSP_init();
void BSP_terminate(int16_t result);
void BSP_drawBitmap(uint8_t const *bitmap);
void BSP_drawNString(uint8_t x,                              /* x in pixels */
                     uint8_t y,                      /* y position in chars */
                     char const *str);
void BSP_updateScore(uint16_t score);

void BSP_displayOn(void);
void BSP_displayOff(void);

void BSP_moveShipUp(void);
void BSP_moveShipDown(void);

#endif                                                             /* bsp_h */
