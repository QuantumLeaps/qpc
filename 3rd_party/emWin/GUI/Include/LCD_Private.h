/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2019  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.10 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only  be used  in accordance  with  a license  and should  not be  re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCD_Private.h
Purpose     : To be used only by the display drivers
----------------------------------------------------------------------
*/

#ifndef LCD_Private_H
#define LCD_Private_H

#include "LCDConf.h"
#include "LCD_Protected.h"
#include "GUI.h"

/*********************************************************************
*
*       API functions
*/
extern const struct tLCDDEV_APIList_struct * /* const */ LCD_aAPI[GUI_NUM_LAYERS];

/*********************************************************************
*
*       Support for Segment/COMLUTs
*/
#define LCD_TYPE_SEGTRANS U16
#define LCD_TYPE_COMTRANS U16

#ifdef LCD_LUT_COM
  extern LCD_TYPE_COMTRANS LCD__aLine2Com0[LCD_YSIZE];
#endif

#ifdef LCD_LUT_SEG
  extern LCD_TYPE_COMTRANS LCD__aCol2Seg0[LCD_XSIZE];
#endif

/*********************************************************************
*
*       Support for multiple display controllers
*/
#define DECLARE_PROTOTYPES(DISTX) \
  void     LCD_##DISTX##_SetPixelIndex(int x, int y, int PixelIndex); \
  unsigned LCD_##DISTX##_GetPixelIndex(int x, int y); \
  void     LCD_##DISTX##_XorPixel     (int x, int y); \
  void     LCD_##DISTX##_DrawHLine    (int x0, int y,  int x1); \
  void     LCD_##DISTX##_DrawVLine    (int x, int y0,  int y1); \
  void     LCD_##DISTX##_FillRect     (int x0, int y0, int x1, int y1); \
  void     LCD_##DISTX##_DrawBitmap   (int x0, int y0, int xsize, int ysize, int BitsPerPixel, int BytesPerLine,  const U8 * pData, int Diff, const LCD_PIXELINDEX * pTrans); \
  void     LCD_##DISTX##_SetOrg       (int x, int y); \
  void     LCD_##DISTX##_On           (void); \
  void     LCD_##DISTX##_Off          (void); \
  int      LCD_##DISTX##_Init         (void); \
  void     LCD_##DISTX##_SetLUTEntry  (U8 Pos, LCD_COLOR Color); \
  void   * LCD_##DISTX##_GetDevFunc   (int Index); \
  void     LCD_##DISTX##_ReInit       (void)

DECLARE_PROTOTYPES(DIST0);
DECLARE_PROTOTYPES(DIST1);
DECLARE_PROTOTYPES(DIST2);
DECLARE_PROTOTYPES(DIST3);

#endif /* Avoid multiple inclusion */

/*************************** End of file ****************************/
