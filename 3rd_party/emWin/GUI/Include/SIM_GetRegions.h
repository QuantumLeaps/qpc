/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.32 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only  be used  in accordance  with  a license  and should  not be  re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : SIM_GetRegions.h
Purpose     : Interface definition of bitmap region functions
---------------------------END-OF-HEADER------------------------------
*/

#ifndef SIM_GETREGIONS_H
#define SIM_GETREGIONS_H

#include <windows.h>

/*********************************************************************
*
*       Interface
*
**********************************************************************
*/
int  SIM_GetRegions     (HBITMAP hBmp, DWORD rgbTrans, int MaxNumRegions);
void SIM_GetRegionRect  (int Index, RECT * pRect);
int  SIM_AddBitmapRegion(HWND hWnd, int Index, int xOff, int yOff);
void SIM_FreeRegions    (void);

#endif

/*************************** End of file ****************************/
