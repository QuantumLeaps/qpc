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
File        : CHOOSECOLOR.h
Purpose     : Message box interface
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef CHOOSECOLOR_H
#define CHOOSECOLOR_H

#include "WM.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define CHOOSECOLOR_CF_MOVEABLE FRAMEWIN_CF_MOVEABLE

#define CHOOSECOLOR_CI_FRAME 0
#define CHOOSECOLOR_CI_FOCUS 1

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       CHOOSECOLOR_PROPS
*/
typedef struct {
  unsigned  aBorder[2];
  unsigned  aSpace[2];
  unsigned  aButtonSize[2];
  GUI_COLOR aColor[2];
} CHOOSECOLOR_PROPS;

/*********************************************************************
*
*       CHOOSECOLOR_CONTEXT
*/
typedef struct {
  U32               LastColor;
  const GUI_COLOR * pColor;
  unsigned          NumColors;
  unsigned          NumColorsPerLine;
  int               SelOld;
  int               Sel;
  WM_HWIN           hParent;
  CHOOSECOLOR_PROPS Props;
} CHOOSECOLOR_CONTEXT;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
WM_HWIN CHOOSECOLOR_Create(WM_HWIN           hParent,
                           int               xPos,
                           int               yPos,
                           int               xSize,
                           int               ySize,
                           const GUI_COLOR * pColor,
                           unsigned          NumColors,
                           unsigned          NumColorsPerLine,
                           int               Sel,
                           const char      * sCaption,
                           int               Flags);

int  CHOOSECOLOR_GetSel(WM_HWIN hObj);
void CHOOSECOLOR_SetSel(WM_HWIN hObj, int Sel);

void CHOOSECOLOR_SetDefaultColor     (unsigned Index, GUI_COLOR Color);
void CHOOSECOLOR_SetDefaultSpace     (unsigned Index, unsigned Space);
void CHOOSECOLOR_SetDefaultBorder    (unsigned Index, unsigned Border);
void CHOOSECOLOR_SetDefaultButtonSize(unsigned Index, unsigned ButtonSize);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void CHOOSECOLOR_Callback(WM_MESSAGE * pMsg);

#if defined(__cplusplus)
  }
#endif

#endif /* GUI_WINSUPPORT */

#endif /* CHOOSECOLOR_H */

/*************************** End of file ****************************/
