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
File        : WINDOW_Private.h
Purpose     : WINDOW private header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef WINDOW_PRIVATE_H
#define WINDOW_PRIVATE_H

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/

typedef struct {
  WIDGET Widget;
  WM_CALLBACK * cb;
  WM_HWIN hFocusedChild;
  WM_DIALOG_STATUS * pDialogStatus;
  GUI_COLOR BkColor;
} WINDOW_OBJ;

/*********************************************************************
*
*       Externals
*
**********************************************************************
*/

extern GUI_COLOR WINDOW__DefaultBkColor;

#endif   /* GUI_WINSUPPORT */
#endif   /* WINDOW_PRIVATE_H */

/*************************** End of file ****************************/
