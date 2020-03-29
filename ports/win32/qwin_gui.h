/**
* @file
* @brief QWIN GUI facilities for building realistic embedded front panels
* @cond
******************************************************************************
* Last updated for version 6.7.0
* Last updated on  2019-12-18
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef QWIN_GUI_H
#define QWIN_GUI_H

#ifndef QWIN_GUI
    #error The pre-processor macro QWIN_GUI must be defined
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>  /* Win32 API */

#ifdef __cplusplus
extern "C" {
#endif

/* create the custom dialog hosting the embedded front panel ...............*/
HWND CreateCustDialog(HINSTANCE hInst, int iDlg, HWND hParent,
                      WNDPROC lpfnWndProc, LPCTSTR lpWndClass);

/* OwnerDrawnButton "class" ................................................*/
typedef struct {
    UINT    itemID;
    HBITMAP hBitmapUp;
    HBITMAP hBitmapDown;
    HCURSOR hCursor;
    int     isDepressed;
} OwnerDrawnButton;

enum OwnerDrawnButtonAction {
    BTN_NOACTION,
    BTN_PAINTED,
    BTN_DEPRESSED,
    BTN_RELEASED
};

void OwnerDrawnButton_init(OwnerDrawnButton * const me,
                           UINT itemID,
                           HBITMAP hBitmapUp, HBITMAP hBitmapDwn,
                           HCURSOR hCursor);
void OwnerDrawnButton_xtor(OwnerDrawnButton * const me);
enum OwnerDrawnButtonAction OwnerDrawnButton_draw(
                                OwnerDrawnButton * const me,
                                LPDRAWITEMSTRUCT lpdis);
void OwnerDrawnButton_set(OwnerDrawnButton * const me,
                          int isDepressed);
BOOL OwnerDrawnButton_isDepressed(OwnerDrawnButton const * const me);

/* GraphicDisplay "class" for drawing graphic displays
* with up to 24-bit color...
*/
typedef struct {
    HDC     src_hDC;
    int     src_width;
    int     src_height;
    HDC     dst_hDC;
    int     dst_width;
    int     dst_height;
    HWND    hItem;
    HBITMAP hBitmap;
    BYTE   *bits;
    BYTE    bgColor[3];
} GraphicDisplay;

void GraphicDisplay_init(GraphicDisplay * const me,
                UINT width,  UINT height,
                UINT itemID, BYTE const bgColor[3]);
void GraphicDisplay_xtor(GraphicDisplay * const me);
void GraphicDisplay_clear(GraphicDisplay * const me);
void GraphicDisplay_redraw(GraphicDisplay * const me);
#define GraphicDisplay_setPixel(me_, x_, y_, color_) do { \
    BYTE *pixelRGB = &(me_)->bits[3*((x_) \
          + (me_)->src_width * ((me_)->src_height - 1U - (y_)))]; \
    pixelRGB[0] = (color_)[0]; \
    pixelRGB[1] = (color_)[1]; \
    pixelRGB[2] = (color_)[2]; \
} while (false)

#define GraphicDisplay_clearPixel(me_, x_, y_) do { \
    BYTE *pixelRGB = &(me_)->bits[3*((x_) \
          + (me_)->src_width * ((me_)->src_height - 1U - (y_)))]; \
    pixelRGB[0] = (me_)->bgColor[0]; \
    pixelRGB[1] = (me_)->bgColor[1]; \
    pixelRGB[2] = (me_)->bgColor[2]; \
} while (false)

/* SegmentDisplay "class" for drawing segment displays, LEDs, etc...........*/
typedef struct {
    HWND    *hSegment;    /* array of segment controls */
    UINT     segmentNum;  /* number of segments */
    HBITMAP *hBitmap;     /* array of bitmap handles */
    UINT     bitmapNum;   /* number of bitmaps */
} SegmentDisplay;

void SegmentDisplay_init(SegmentDisplay * const me,
                         UINT segNum, UINT bitmapNum);
void SegmentDisplay_xtor(SegmentDisplay * const me);
BOOL SegmentDisplay_initSegment(SegmentDisplay * const me,
                         UINT segmentNum, UINT segmentID);
BOOL SegmentDisplay_initBitmap(SegmentDisplay * const me,
                         UINT bitmapNum, HBITMAP hBitmap);
BOOL SegmentDisplay_setSegment(SegmentDisplay * const me,
                         UINT segmentNum, UINT bitmapNum);

/* useful helper functions .................................................*/
void DrawBitmap(HDC hdc, HBITMAP hBitmap, int xStart, int yStart);

#ifdef __cplusplus
}
#endif

#endif /* QWIN_GUI_H */
