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
#include "win32_gui.h"
#include <stdlib.h>

/*--------------------------------------------------------------------------*/
HWND CreateCustDialog(HINSTANCE hInst, int iDlg, HWND hParent,
                      WNDPROC lpfnWndProc, LPCTSTR lpWndClass)
{
    WNDCLASSEX wndclass;
    HWND       hWnd;

    wndclass.cbSize        = sizeof(wndclass);
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = lpfnWndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = DLGWINDOWEXTRA;
    wndclass.hInstance     = hInst;
    wndclass.hIcon         = NULL;
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = lpWndClass;
    wndclass.hIconSm       = NULL;

    RegisterClassEx(&wndclass);

    hWnd = CreateDialog(hInst, MAKEINTRESOURCE(iDlg), hParent, NULL);

    /* NOTE: WM_INITDIALOG provides stimulus for initializing dialog controls.
    * Dialog box procedures typically use this message to initialize controls
    * and carry out any other initialization tasks that affect the appearance
    * of the dialog box.
    */
    SendMessage(hWnd, WM_INITDIALOG, (WPARAM)0, (LPARAM)0);

    return hWnd;
}

/*--------------------------------------------------------------------------*/
void OwnerDrawnButton_init(OwnerDrawnButton * const me,
                           HBITMAP hBitmapUp, HBITMAP hBitmapDwn,
                           HCURSOR hCursor)
{
    me->hBitmapUp   = hBitmapUp;
    me->hBitmapDown = hBitmapDwn;
    me->hCursor     = hCursor;
}
/*..........................................................................*/
void OwnerDrawnButton_xtor(OwnerDrawnButton * const me) {
    DeleteObject(me->hBitmapUp);
    DeleteObject(me->hBitmapDown);
}
/*..........................................................................*/
enum OwnerDrawnButtonAction OwnerDrawnButton_draw(
                               OwnerDrawnButton * const me,
                               LPDRAWITEMSTRUCT lpdis)
{
    enum OwnerDrawnButtonAction ret = BTN_NOACTION;

    if ((lpdis->itemAction & ODA_DRAWENTIRE) != 0U) {
        if (me->hCursor != NULL) {
           SetClassLong(lpdis->hwndItem, GCL_HCURSOR, (LONG)me->hCursor);
           me->hCursor = NULL;                       /* mark the cursor set */
        }
        DrawBitmap(lpdis->hDC, me->hBitmapUp,
                   lpdis->rcItem.left, lpdis->rcItem.top);
        ret = BTN_PAINTED;
    }
    else if ((lpdis->itemAction & ODA_SELECT) != 0U) {
        if ((lpdis->itemState & ODS_SELECTED) != 0U) {
            DrawBitmap(lpdis->hDC, me->hBitmapDown,
                       lpdis->rcItem.left, lpdis->rcItem.top);
            ret = BTN_DEPRESSED;
        }
        else {
            /* NOTE: the bitmap for button "UP" look will be
            * drawn in the ODA_DRAWENTIRE action
            */
            ret = BTN_RELEASED;
        }
    }
    return ret;
}

/*--------------------------------------------------------------------------*/
void GraphicDisplay_init(GraphicDisplay * const me,
                    UINT width,  UINT xScale,
                    UINT height, UINT yScale,
                    HWND hItem,  BYTE const bgColor[3])
{
    HDC hDC;
    BITMAPINFO bi24BitInfo;

    me->width  = width;
    me->xScale = xScale;
    me->height = height;
    me->yScale = yScale;

    me->bgColor[0] = bgColor[0];
    me->bgColor[1] = bgColor[1];
    me->bgColor[2] = bgColor[2];

    me->hItem = hItem;

    bi24BitInfo.bmiHeader.biBitCount    = 3U*8U;             /* 3 RGB bytes */
    bi24BitInfo.bmiHeader.biCompression = BI_RGB;              /* RGB color */
    bi24BitInfo.bmiHeader.biPlanes      = 1U;
    bi24BitInfo.bmiHeader.biSize        = sizeof(bi24BitInfo.bmiHeader);
    bi24BitInfo.bmiHeader.biWidth       = me->width  * me->xScale;
    bi24BitInfo.bmiHeader.biHeight      = me->height * me->yScale;

    hDC = CreateCompatibleDC(NULL);
    me->hBitmap = CreateDIBSection(hDC, &bi24BitInfo, DIB_RGB_COLORS,
                                   (void **)&me->bits, 0, 0);
    DeleteDC(hDC);

    GraphicDisplay_clear(me);
    GraphicDisplay_redraw(me);
}
/*..........................................................................*/
void GraphicDisplay_xtor(GraphicDisplay * const me) {
    DeleteObject(me->hBitmap);
}
/*..........................................................................*/
void GraphicDisplay_clear(GraphicDisplay * const me) {
    UINT n;
    BYTE r = me->bgColor[0];
    BYTE g = me->bgColor[1];
    BYTE b = me->bgColor[2];
    BYTE *bits = me->bits;

    for (n = me->width*me->xScale * me->height * me->yScale;
         n != 0U;
         --n, bits += 3)
    {
        bits[0] = b;
        bits[1] = g;
        bits[2] = r;
    }
}
/*..........................................................................*/
void GraphicDisplay_setPixel(GraphicDisplay * const me, UINT x, UINT y,
                        BYTE const color[3])
{
    UINT sx, sy;
    BYTE *pixelRGB = &me->bits[3*(me->xScale*x
                  + me->xScale*me->width * me->yScale*(me->height - 1U - y))];
    BYTE r = color[0];
    BYTE g = color[1];
    BYTE b = color[2];
    for (sy = me->yScale; sy != 0U;
         --sy, pixelRGB += me->xScale*me->width*3U)
    {
        for (sx = 3U*me->xScale; sx != 0U; sx -= 3U) {
            pixelRGB[sx - 3U] = b;
            pixelRGB[sx - 2U] = g;
            pixelRGB[sx - 1U] = r;
        }
    }
}
/*..........................................................................*/
void GraphicDisplay_clearPixel(GraphicDisplay * const me, UINT x, UINT y) {
    GraphicDisplay_setPixel(me, x, y, me->bgColor);
}
/*..........................................................................*/
void GraphicDisplay_redraw(GraphicDisplay * const me) {
    SendMessage(me->hItem, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)me->hBitmap);
}

/* SegmentDisplay ----------------------------------------------------------*/
void SegmentDisplay_init(SegmentDisplay * const me,
                         UINT segmentNum, UINT bitmapNum)
{
    UINT n;

    me->hSegment = (HWND *)calloc(segmentNum, sizeof(HWND));
    me->segmentNum = segmentNum;
    for (n = 0U; n < segmentNum; ++n) {
        me->hSegment[n] = NULL;
    }
    me->hBitmap = (HBITMAP *)calloc(bitmapNum, sizeof(HBITMAP));
    me->bitmapNum = bitmapNum;
    for (n = 0U; n < bitmapNum; ++n) {
        me->hBitmap[n] = NULL;
    }
}
/*..........................................................................*/
void SegmentDisplay_xtor(SegmentDisplay * const me) {
    UINT n;

    for (n = 0U; n < me->segmentNum; ++n) {
        DeleteObject(me->hSegment[n]);
    }
    free(me->hSegment);

    for (n = 0U; n < me->bitmapNum; ++n) {
        DeleteObject(me->hBitmap[n]);
    }
    free(me->hBitmap);
}
/*..........................................................................*/
BOOL SegmentDisplay_initSegment(SegmentDisplay * const me,
                                UINT segmentNum, HWND hSegment)
{
    if ((segmentNum < me->segmentNum) && (hSegment != NULL)) {
        me->hSegment[segmentNum] = hSegment;
        return TRUE;
    }
    else {
        return FALSE;
    }
}
/*..........................................................................*/
BOOL SegmentDisplay_initBitmap(SegmentDisplay * const me,
                               UINT bitmapNum, HBITMAP hBitmap)
{
    if ((bitmapNum < me->bitmapNum) && (hBitmap != NULL)) {
        me->hBitmap[bitmapNum] = hBitmap;
        return TRUE;
    }
    else {
        return FALSE;
    }
}
/*..........................................................................*/
BOOL SegmentDisplay_setSegment(SegmentDisplay * const me,
                               UINT segmentNum, UINT bitmapNum)
{
    if ((segmentNum < me->segmentNum) && (bitmapNum < me->bitmapNum)) {
        SendMessage(me->hSegment[segmentNum], STM_SETIMAGE,
                IMAGE_BITMAP,
                (LPARAM)me->hBitmap[bitmapNum]);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

/*--------------------------------------------------------------------------*/
/* DrawBitmap() function adapted from the book "Programming Windows" by
* Charles Petzold.
*/
void DrawBitmap(HDC hdc, HBITMAP hBitmap,
                int xStart, int yStart)
{
    BITMAP bm;
    POINT  ptSize, ptOrg;
    HDC    hdcMem = CreateCompatibleDC(hdc);

    SelectObject(hdcMem, hBitmap);
    SetMapMode(hdcMem, GetMapMode(hdc));

    GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bm);
    ptSize.x = bm.bmWidth;
    ptSize.y = bm.bmHeight;
    DPtoLP(hdc, &ptSize, 1);

    ptOrg.x = 0;
    ptOrg.y = 0;
    DPtoLP(hdcMem, &ptOrg, 1);

    BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y,
           hdcMem, ptOrg.x, ptOrg.y, SRCCOPY);
    DeleteDC(hdcMem);
}


