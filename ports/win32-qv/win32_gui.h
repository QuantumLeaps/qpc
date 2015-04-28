/**
* @file
* @brief Win32 GUI facilities for building realistic embedded front panels
* @ingroup ports
* @cond
******************************************************************************
* Last Update: 2015-03-02
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the following MIT License (MIT).
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
* Contact information:
* Web  : http://www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef win32_gui_h
#define win32_gui_h

#define WIN32_LEAN_AND_MEAN
#include <windows.h>  /* Win32 API */

/* create the custom dialog hosting the embedded front panel ...............*/
HWND CreateCustDialog(HINSTANCE hInst, int iDlg, HWND hParent,
                      WNDPROC lpfnWndProc, LPCTSTR lpWndClass);

/* OwnerDrawnButton "class" ................................................*/
typedef struct OwnerDrawnButtonTag {
    HBITMAP hBitmapUp;
    HBITMAP hBitmapDown;
    HCURSOR hCursor;
} OwnerDrawnButton;

enum OwnerDrawnButtonAction {
    BTN_NOACTION,
    BTN_PAINTED,
    BTN_DEPRESSED,
    BTN_RELEASED
};

void OwnerDrawnButton_init(OwnerDrawnButton * const me,
                           HBITMAP hBitmapUp, HBITMAP hBitmapDwn,
                           HCURSOR hCursor);
void OwnerDrawnButton_xtor(OwnerDrawnButton * const me);
enum OwnerDrawnButtonAction OwnerDrawnButton_draw(
                               OwnerDrawnButton * const me,
                               LPDRAWITEMSTRUCT lpdis);

/* GraphicDisplay "class" for drawing graphic displays
* with up to 24-bit color...
*/
typedef struct GraphicDisplayTag {
    UINT    width;
    UINT    xScale;
    UINT    height;
    UINT    yScale;
    HBITMAP hBitmap;
    HWND    hItem;
    BYTE   *bits;
    BYTE    bgColor[3];
} GraphicDisplay;

void GraphicDisplay_init(GraphicDisplay * const me,
                UINT width,  UINT xScale,
                UINT height, UINT yScale,
                HWND hItem,  BYTE const bgColor[3]);
void GraphicDisplay_xtor(GraphicDisplay * const me);
void GraphicDisplay_clear(GraphicDisplay * const me);
void GraphicDisplay_setPixel(GraphicDisplay * const me, UINT x, UINT y,
                 BYTE const color[3]);
void GraphicDisplay_clearPixel(GraphicDisplay * const me, UINT x, UINT y);
void GraphicDisplay_redraw(GraphicDisplay * const me);

/* SegmentDisplay "class" for drawing segment displays, LEDs, etc...........*/
typedef struct SegmentDisplayTag {
    HWND    *hSegment;    /* array of segment controls */
    UINT     segmentNum;  /* number of segments */
    HBITMAP *hBitmap;     /* array of bitmap handles */
    UINT     bitmapNum;   /* number of bitmaps */
} SegmentDisplay;

void SegmentDisplay_init(SegmentDisplay * const me,
                         UINT segNum, UINT bitmapNum);
void SegmentDisplay_xtor(SegmentDisplay * const me);
BOOL SegmentDisplay_initSegment(SegmentDisplay * const me,
                         UINT segmentNum, HWND hSegment);
BOOL SegmentDisplay_initBitmap(SegmentDisplay * const me,
                         UINT bitmapNum, HBITMAP hBitmap);
BOOL SegmentDisplay_setSegment(SegmentDisplay * const me,
                         UINT segmentNum, UINT bitmapNum);

/* useful helper functions .................................................*/
void DrawBitmap(HDC hdc, HBITMAP hBitmap, int xStart, int yStart);

#endif /* win32_gui_h */
