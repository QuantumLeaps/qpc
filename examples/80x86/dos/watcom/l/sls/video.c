/*****************************************************************************
* Product:  VGA screen output
* Last Updated for Version: 4.1.01
* Date of the Last Update:  Nov 03, 2009
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
#include "qp_port.h"
#include "video.h"

#include <conio.h>
#include <stdlib.h>

Q_DEFINE_THIS_FILE

#define VIDEO_BASE   0xB800
#define VIDEO_WIDTH  80
#define VIDEO_HEIGHT 25

#define OFF(x_, y_)  ((((y_) * VIDEO_WIDTH) + (x_)) * 2)

/*..........................................................................*/
void Video_clearScreen(uint8_t bgColor) {
    system("CLS");
    Video_clearRect(0,  0, VIDEO_WIDTH, VIDEO_HEIGHT, bgColor);
}
/*..........................................................................*/
void Video_clearRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                     uint8_t bgColor)
{
    uint8_t far *pscrX = (uint8_t far *)MK_FP(VIDEO_BASE,
                                   (uint16_t)(((y1 * VIDEO_WIDTH) + x1) * 2));
    x2 -= x1;
    y2 -= y1;
    for (x1 = 0; x1 < x2; ++x1, pscrX += 2) {                /* loop over x */
        uint8_t far *pscrXY = pscrX;
        for (y1 = 0; y1 < y2; ++y1, pscrXY += 2*VIDEO_WIDTH) {
            pscrXY[0] = 0x20;                     /* Put space in video RAM */
            pscrXY[1] = bgColor;        /* Put video attribute in video RAM */
        }
    }
}
/*..........................................................................*/
void Video_drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                    uint8_t fgColor, uint8_t line)
{
    uint8_t far *pscr = (uint8_t far *)MK_FP(VIDEO_BASE, 0);
    uint16_t off;
    uint8_t  xy;

    static uint8_t const line_char[][6] = {
        { ' ',   ' ',  ' ',  ' ',  ' ',  ' ' },
        { 0xDA, 0xBF, 0xC0, 0xD9, 0xC4, 0xB3 },
        { 0xC9, 0xBB, 0xC8, 0xBC, 0xCD, 0xBA }
    };
    enum {
        UL_CORNER,
        UR_CORNER,
        LL_CORNER,
        LR_CORNER,
        HOR_LINE,
        VER_LINE
    };

    Q_REQUIRE(line < Q_DIM(line_char));

    off = OFF(x1, y1);
    pscr[off]      = line_char[line][UL_CORNER];
    pscr[off + 1] &= ~0x0F;                   /* clear the foreground color */
    pscr[off + 1] |= fgColor;           /* Put video attribute in video RAM */

    off = OFF(x2 - 1, y1);
    pscr[off]      = line_char[line][UR_CORNER];
    pscr[off + 1] &= ~0x0F;                   /* clear the foreground color */
    pscr[off + 1] |= fgColor;           /* Put video attribute in video RAM */

    off = OFF(x1, y2 - 1);
    pscr[off]      = line_char[line][LL_CORNER];
    pscr[off + 1] &= ~0x0F;                   /* clear the foreground color */
    pscr[off + 1] |= fgColor;           /* Put video attribute in video RAM */

    off = OFF(x2 - 1, y2 - 1);
    pscr[off]      = line_char[line][LR_CORNER];
    pscr[off + 1] &= ~0x0F;                   /* clear the foreground color */
    pscr[off + 1] |= fgColor;           /* Put video attribute in video RAM */

    for (xy = x1 + 1; xy < x2 - 1; ++xy) {
        off = OFF(xy, y1);
        pscr[off]      = line_char[line][HOR_LINE];
        pscr[off + 1] &= ~0x0F;               /* clear the foreground color */
        pscr[off + 1] |= fgColor;       /* Put video attribute in video RAM */

        off = OFF(xy, y2 - 1);
        pscr[off]      = line_char[line][HOR_LINE];
        pscr[off + 1] &= ~0x0F;               /* clear the foreground color */
        pscr[off + 1] |= fgColor;       /* Put video attribute in video RAM */
    }
    for (xy = y1 + 1; xy < y2 - 1; ++xy) {
        off = OFF(x1, xy);
        pscr[off]      = line_char[line][VER_LINE];
        pscr[off + 1] &= ~0x0F;               /* clear the foreground color */
        pscr[off + 1] |= fgColor;       /* Put video attribute in video RAM */

        off = OFF(x2 - 1, xy);
        pscr[off]      = line_char[line][VER_LINE];
        pscr[off + 1] &= ~0x0F;               /* clear the foreground color */
        pscr[off + 1] |= fgColor;       /* Put video attribute in video RAM */
    }
}
/*..........................................................................*/
void Video_printStrAt(uint8_t x, uint8_t y, uint8_t color,
                      char const *str)
{
                               /* calculate position on the video RAM (VGA) */
    uint8_t far *pscr = (uint8_t far *)MK_FP(VIDEO_BASE,
                                     (uint16_t)(((y * VIDEO_WIDTH) + x) * 2));
    while (*str != (uint8_t)0) {
        pscr[0] = *str++;                     /* Put character in video RAM */
        pscr[1] &= ~0x0F;                     /* clear the foreground color */
        pscr[1] |= color;               /* Put video attribute in video RAM */
        pscr += 2;
    }
}
/*..........................................................................*/
void Video_printNumAt(uint8_t x, uint8_t y, uint8_t color, uint32_t num) {
    uint8_t far *pscr = (uint8_t far *)MK_FP(VIDEO_BASE,
                                     (uint16_t)(((y * VIDEO_WIDTH) + x) * 2));

    pscr[3*2] = (char)('0' + (num % 10));
    pscr[3*2 + 1] |= color;
    num /= 10;
    pscr[2*2] = (char)('0' + (num % 10));
    pscr[2*2 + 1] |= color;
    num /= 10;
    if (num == 0) {
        if (pscr[2*2] == '0') {
            pscr[2*2] = 0x20;
        }
    }
    else {
        pscr[1*2] = (char)('0' + (num % 10));
        pscr[1*2 + 1] |= color;
        num /= 10;
        if (num == 0) {
            if (pscr[1*2] == '0') {
                pscr[1*2] = 0x20;
            }
        }
        else {
            pscr[0] = (char)('0' + num % 10);
            pscr[0 + 1] |= color;
            if (pscr[0] == '0') {
                pscr[0] = 0x20;
            }
        }
    }
}
/*..........................................................................*/
void Video_drawBitmapAt(uint8_t x, uint8_t y,
                        uint8_t const *bitmap, uint8_t width, uint8_t height)
{
    uint8_t far *pscrX = (uint8_t far *)MK_FP(VIDEO_BASE,
                                    (uint16_t)(((y * VIDEO_WIDTH) + x) << 1));
    uint8_t ix, iy;
    uint8_t w = width;
    uint8_t h = height;

                                                    /* perform the clipping */
    if (x > VIDEO_WIDTH) {
        x = VIDEO_WIDTH;
    }
    if (y > VIDEO_HEIGHT) {
        y = VIDEO_HEIGHT;
    }
    if (w > VIDEO_WIDTH - x) {
        w = VIDEO_WIDTH - x;
    }
    if (h > VIDEO_HEIGHT - y) {
        h = VIDEO_HEIGHT - y;
    }

    for (ix = 0; ix < w; ++ix, pscrX += 2) {                 /* loop over x */
        uint8_t far *pscrXY = pscrX;
        for (iy = 0; iy < h; ++iy, pscrXY += 2*VIDEO_WIDTH) {
            static uint8_t const pixel[2] = { 0x20, 0xDB };
            uint8_t byte = bitmap[ix + (iy >> 3)*width];
            *pscrXY = pixel[(byte >> (iy & 0x7)) & 1];
        }
    }
}
/*..........................................................................*/
void Video_drawStringAt(uint8_t x, uint8_t y, char const *str) {
    static uint8_t const font5x7[95][5] = {
        { 0x00, 0x00, 0x00, 0x00, 0x00 },                            /* ' ' */
        { 0x00, 0x00, 0x4F, 0x00, 0x00 },                              /* ! */
        { 0x00, 0x07, 0x00, 0x07, 0x00 },                              /* " */
        { 0x14, 0x7F, 0x14, 0x7F, 0x14 },                              /* # */
        { 0x24, 0x2A, 0x7F, 0x2A, 0x12 },                              /* $ */
        { 0x23, 0x13, 0x08, 0x64, 0x62 },                              /* % */
        { 0x36, 0x49, 0x55, 0x22, 0x50 },                              /* & */
        { 0x00, 0x05, 0x03, 0x00, 0x00 },                              /* ' */
        { 0x00, 0x1C, 0x22, 0x41, 0x00 },                              /* ( */
        { 0x00, 0x41, 0x22, 0x1C, 0x00 },                              /* ) */
        { 0x14, 0x08, 0x3E, 0x08, 0x14 },                              /* * */
        { 0x08, 0x08, 0x3E, 0x08, 0x08 },                              /* + */
        { 0x00, 0x50, 0x30, 0x00, 0x00 },                              /* , */
        { 0x08, 0x08, 0x08, 0x08, 0x08 },                              /* - */
        { 0x00, 0x60, 0x60, 0x00, 0x00 },                              /* . */
        { 0x20, 0x10, 0x08, 0x04, 0x02 },                              /* / */
        { 0x3E, 0x51, 0x49, 0x45, 0x3E },                              /* 0 */
        { 0x00, 0x42, 0x7F, 0x40, 0x00 },                              /* 1 */
        { 0x42, 0x61, 0x51, 0x49, 0x46 },                              /* 2 */
        { 0x21, 0x41, 0x45, 0x4B, 0x31 },                              /* 3 */
        { 0x18, 0x14, 0x12, 0x7F, 0x10 },                              /* 4 */
        { 0x27, 0x45, 0x45, 0x45, 0x39 },                              /* 5 */
        { 0x3C, 0x4A, 0x49, 0x49, 0x30 },                              /* 6 */
        { 0x01, 0x71, 0x09, 0x05, 0x03 },                              /* 7 */
        { 0x36, 0x49, 0x49, 0x49, 0x36 },                              /* 8 */
        { 0x06, 0x49, 0x49, 0x29, 0x1E },                              /* 9 */
        { 0x00, 0x36, 0x36, 0x00, 0x00 },                              /* : */
        { 0x00, 0x56, 0x36, 0x00, 0x00 },                              /* ; */
        { 0x08, 0x14, 0x22, 0x41, 0x00 },                              /* < */
        { 0x14, 0x14, 0x14, 0x14, 0x14 },                              /* = */
        { 0x00, 0x41, 0x22, 0x14, 0x08 },                              /* > */
        { 0x02, 0x01, 0x51, 0x09, 0x06 },                              /* ? */
        { 0x32, 0x49, 0x79, 0x41, 0x3E },                              /* @ */
        { 0x7E, 0x11, 0x11, 0x11, 0x7E },                              /* A */
        { 0x7F, 0x49, 0x49, 0x49, 0x36 },                              /* B */
        { 0x3E, 0x41, 0x41, 0x41, 0x22 },                              /* C */
        { 0x7F, 0x41, 0x41, 0x22, 0x1C },                              /* D */
        { 0x7F, 0x49, 0x49, 0x49, 0x41 },                              /* E */
        { 0x7F, 0x09, 0x09, 0x09, 0x01 },                              /* F */
        { 0x3E, 0x41, 0x49, 0x49, 0x7A },                              /* G */
        { 0x7F, 0x08, 0x08, 0x08, 0x7F },                              /* H */
        { 0x00, 0x41, 0x7F, 0x41, 0x00 },                              /* I */
        { 0x20, 0x40, 0x41, 0x3F, 0x01 },                              /* J */
        { 0x7F, 0x08, 0x14, 0x22, 0x41 },                              /* K */
        { 0x7F, 0x40, 0x40, 0x40, 0x40 },                              /* L */
        { 0x7F, 0x02, 0x0C, 0x02, 0x7F },                              /* M */
        { 0x7F, 0x04, 0x08, 0x10, 0x7F },                              /* N */
        { 0x3E, 0x41, 0x41, 0x41, 0x3E },                              /* O */
        { 0x7F, 0x09, 0x09, 0x09, 0x06 },                              /* P */
        { 0x3E, 0x41, 0x51, 0x21, 0x5E },                              /* Q */
        { 0x7F, 0x09, 0x19, 0x29, 0x46 },                              /* R */
        { 0x46, 0x49, 0x49, 0x49, 0x31 },                              /* S */
        { 0x01, 0x01, 0x7F, 0x01, 0x01 },                              /* T */
        { 0x3F, 0x40, 0x40, 0x40, 0x3F },                              /* U */
        { 0x1F, 0x20, 0x40, 0x20, 0x1F },                              /* V */
        { 0x3F, 0x40, 0x38, 0x40, 0x3F },                              /* W */
        { 0x63, 0x14, 0x08, 0x14, 0x63 },                              /* X */
        { 0x07, 0x08, 0x70, 0x08, 0x07 },                              /* Y */
        { 0x61, 0x51, 0x49, 0x45, 0x43 },                              /* Z */
        { 0x00, 0x7F, 0x41, 0x41, 0x00 },                              /* [ */
        { 0x02, 0x04, 0x08, 0x10, 0x20 },                              /* \ */
        { 0x00, 0x41, 0x41, 0x7F, 0x00 },                              /* ] */
        { 0x04, 0x02, 0x01, 0x02, 0x04 },                              /* ^ */
        { 0x40, 0x40, 0x40, 0x40, 0x40 },                              /* _ */
        { 0x00, 0x01, 0x02, 0x04, 0x00 },                              /* ` */
        { 0x20, 0x54, 0x54, 0x54, 0x78 },                              /* a */
        { 0x7F, 0x48, 0x44, 0x44, 0x38 },                              /* b */
        { 0x38, 0x44, 0x44, 0x44, 0x20 },                              /* c */
        { 0x38, 0x44, 0x44, 0x48, 0x7F },                              /* d */
        { 0x38, 0x54, 0x54, 0x54, 0x18 },                              /* e */
        { 0x08, 0x7E, 0x09, 0x01, 0x02 },                              /* f */
        { 0x0C, 0x52, 0x52, 0x52, 0x3E },                              /* g */
        { 0x7F, 0x08, 0x04, 0x04, 0x78 },                              /* h */
        { 0x00, 0x44, 0x7D, 0x40, 0x00 },                              /* i */
        { 0x20, 0x40, 0x44, 0x3D, 0x00 },                              /* j */
        { 0x7F, 0x10, 0x28, 0x44, 0x00 },                              /* k */
        { 0x00, 0x41, 0x7F, 0x40, 0x00 },                              /* l */
        { 0x7C, 0x04, 0x18, 0x04, 0x78 },                              /* m */
        { 0x7C, 0x08, 0x04, 0x04, 0x78 },                              /* n */
        { 0x38, 0x44, 0x44, 0x44, 0x38 },                              /* o */
        { 0x7C, 0x14, 0x14, 0x14, 0x08 },                              /* p */
        { 0x08, 0x14, 0x14, 0x18, 0x7C },                              /* q */
        { 0x7C, 0x08, 0x04, 0x04, 0x08 },                              /* r */
        { 0x48, 0x54, 0x54, 0x54, 0x20 },                              /* s */
        { 0x04, 0x3F, 0x44, 0x40, 0x20 },                              /* t */
        { 0x3C, 0x40, 0x40, 0x20, 0x7C },                              /* u */
        { 0x1C, 0x20, 0x40, 0x20, 0x1C },                              /* v */
        { 0x3C, 0x40, 0x30, 0x40, 0x3C },                              /* w */
        { 0x44, 0x28, 0x10, 0x28, 0x44 },                              /* x */
        { 0x0C, 0x50, 0x50, 0x50, 0x3C },                              /* y */
        { 0x44, 0x64, 0x54, 0x4C, 0x44 },                              /* z */
        { 0x00, 0x08, 0x36, 0x41, 0x00 },                              /* { */
        { 0x00, 0x00, 0x7F, 0x00, 0x00 },                              /* | */
        { 0x00, 0x41, 0x36, 0x08, 0x00 },                              /* } */
        { 0x02, 0x01, 0x02, 0x04, 0x02 },                              /* ~ */
    };

    while (*str != '\0') {
        Video_drawBitmapAt(x, y, font5x7[*str - ' '], 5, 8);
        ++str;
        x += 6;
    }
}
