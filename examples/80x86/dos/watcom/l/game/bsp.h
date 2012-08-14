/*****************************************************************************
* Product: "Fly'n'Shoot" game, BSP for 80x86, Vanilla/DOS, Open Watcom
* Last Updated for Version: 4.1.03
* Date of the Last Update:  Feb 18, 2010
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2010 Quantum Leaps, LLC. All rights reserved.
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

#define BSP_TICKS_PER_SEC    30
#define BSP_SCREEN_WIDTH     80
#define BSP_SCREEN_HEIGHT    16

void BSP_init(int argc, char *argv[]);
void BSP_drawBitmap(uint8_t const *bitmap,
                    uint8_t width,          /* with of the bitmap in pixels */
                    uint8_t height);      /* height of the bitmap in pixels */
void BSP_drawNString(uint8_t x,                              /* x in pixels */
                     uint8_t y,                      /* y position in chars */
                     char const *str);
void BSP_updateScore(uint16_t score);

void BSP_displayOn(void);
void BSP_displayOff(void);

#endif                                                             /* bsp_h */
