/*****************************************************************************
* Product: Win32 GUI facilities for building realistic embedded front panels
* Date of the Last Update:  Aug 16, 2013
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
#ifndef win32_gui_h
#define win32_gui_h

#define WIN32_LEAN_AND_MEAN
#include <windows.h>                                           /* Win32 API */

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

/* GraphicDisplay class for drawing graphic displays with up to 24-bit color*/
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
    HWND    *hSegment;                         /* array of segment controls */
    UINT     segmentNum;                              /* number of segments */
    HBITMAP *hBitmap;                            /* array of bitmap handles */
    UINT     bitmapNum;                                /* number of bitmaps */
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

#endif                                                       /* win32_gui_h */
