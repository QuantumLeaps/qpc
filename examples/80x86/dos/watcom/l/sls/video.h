/*****************************************************************************
* Product:  VGA screen output
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Sep 22, 2008
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
#ifndef video_h
#define video_h

/*..........................................................................*/
enum VideoColor {
    /* foreground */
    VIDEO_FGND_BLACK        = 0x00,
    VIDEO_FGND_BLUE         = 0x01,
    VIDEO_FGND_GREEN        = 0x02,
    VIDEO_FGND_CYAN         = 0x03,
    VIDEO_FGND_RED          = 0x04,
    VIDEO_FGND_PURPLE       = 0x05,
    VIDEO_FGND_BROWN        = 0x06,
    VIDEO_FGND_LIGHT_GRAY   = 0x07,
    VIDEO_FGND_DARK_GRAY    = 0x08,
    VIDEO_FGND_LIGHT_BLUE   = 0x09,
    VIDEO_FGND_LIGHT_GREEN  = 0x0A,
    VIDEO_FGND_LIGHT_CYAN   = 0x0B,
    VIDEO_FGND_LIGHT_RED    = 0x0C,
    VIDEO_FGND_LIGHT_PURPLE = 0x0D,
    VIDEO_FGND_YELLOW       = 0x0E,
    VIDEO_FGND_WHITE        = 0x0F,
    /* background */
    VIDEO_BGND_BLACK        = 0x00,
    VIDEO_BGND_BLUE         = 0x10,
    VIDEO_BGND_GREEN        = 0x20,
    VIDEO_BGND_CYAN         = 0x30,
    VIDEO_BGND_RED          = 0x40,
    VIDEO_BGND_PURPLE       = 0x50,
    VIDEO_BGND_BROWN        = 0x60,
    VIDEO_BGND_LIGHT_GRAY   = 0x70,

    VIDEO_BLINK             = 0x80
};
/*..........................................................................*/
void Video_clearScreen(uint8_t bgColor);
/*..........................................................................*/
void Video_clearRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                     uint8_t bgColor);
/*..........................................................................*/
void Video_drawRect (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                     uint8_t fgColor, uint8_t line);
/*..........................................................................*/
void Video_printStrAt(uint8_t x, uint8_t y, uint8_t color,
                      char const *str);
/*..........................................................................*/
void Video_printNumAt(uint8_t x, uint8_t y, uint8_t color, uint32_t num);
/*..........................................................................*/
void Video_drawBitmapAt(uint8_t x, uint8_t y,
                        uint8_t const *bitmap, uint8_t width, uint8_t height);
/*..........................................................................*/
void Video_drawStringAt(uint8_t x, uint8_t y, char const *str);

#endif                                                           /* video_h */

