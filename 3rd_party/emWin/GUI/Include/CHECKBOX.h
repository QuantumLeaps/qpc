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
File        : CHECKBOX.h
Purpose     : CHECKBOX include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "WM.h"
#include "DIALOG_Type.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"

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

/*********************************************************************
*
*       Color indices
*/
#define CHECKBOX_CI_DISABLED 0
#define CHECKBOX_CI_ENABLED  1

/*********************************************************************
*
*       Bitmap indices
*/
#define CHECKBOX_BI_INACTIV_UNCHECKED 0
#define CHECKBOX_BI_ACTIV_UNCHECKED   1
#define CHECKBOX_BI_INACTIV_CHECKED   2
#define CHECKBOX_BI_ACTIV_CHECKED     3
#define CHECKBOX_BI_INACTIV_3STATE    4
#define CHECKBOX_BI_ACTIV_3STATE      5

/*********************************************************************
*
*       Skinning property indices
*/
#define CHECKBOX_SKINFLEX_PI_ENABLED     0
#define CHECKBOX_SKINFLEX_PI_DISABLED    1

/*********************************************************************
*
*       Public Types
*
**********************************************************************
*/
typedef WM_HMEM CHECKBOX_Handle;

typedef struct {
  GUI_COLOR aColorFrame[3];
  GUI_COLOR aColorInner[2];
  GUI_COLOR ColorCheck;
  int       ButtonSize;
} CHECKBOX_SKINFLEX_PROPS;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/
CHECKBOX_Handle CHECKBOX_Create        (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int Id, int Flags);
CHECKBOX_Handle CHECKBOX_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id);
CHECKBOX_Handle CHECKBOX_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int NumExtraBytes);
CHECKBOX_Handle CHECKBOX_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void CHECKBOX_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Managing default values
*
**********************************************************************
*/

int              CHECKBOX_GetDefaultAlign     (void);
GUI_COLOR        CHECKBOX_GetDefaultBkColor   (void);
const GUI_FONT * CHECKBOX_GetDefaultFont      (void);
int              CHECKBOX_GetDefaultSpacing   (void);
GUI_COLOR        CHECKBOX_GetDefaultTextColor (void);
int              CHECKBOX_GetUserData         (CHECKBOX_Handle hObj, void * pDest, int NumBytes);
void             CHECKBOX_SetDefaultAlign     (int Align);
void             CHECKBOX_SetDefaultBkColor   (GUI_COLOR Color);
GUI_COLOR        CHECKBOX_SetDefaultFocusColor(GUI_COLOR Color);
void             CHECKBOX_SetDefaultFont      (const GUI_FONT * pFont);
void             CHECKBOX_SetDefaultImage     (const GUI_BITMAP * pBitmap, unsigned int Index);
void             CHECKBOX_SetDefaultSpacing   (int Spacing);
void             CHECKBOX_SetDefaultTextColor (GUI_COLOR Color);

#define          CHECKBOX_GetDefaultTextAlign CHECKBOX_GetDefaultAlign
#define          CHECKBOX_SetDefaultTextAlign CHECKBOX_SetDefaultAlign

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/
GUI_COLOR          CHECKBOX_GetBkColor   (CHECKBOX_Handle hObj);
GUI_COLOR          CHECKBOX_GetBoxBkColor(CHECKBOX_Handle hObj, int Index);
GUI_COLOR          CHECKBOX_GetFocusColor(CHECKBOX_Handle hObj);
const GUI_FONT   * CHECKBOX_GetFont      (CHECKBOX_Handle hObj);
const GUI_BITMAP * CHECKBOX_GetImage     (CHECKBOX_Handle hObj, unsigned int Index);
int                CHECKBOX_GetState     (CHECKBOX_Handle hObj);
int                CHECKBOX_GetText      (CHECKBOX_Handle hObj, char * pBuffer, int MaxLen);
int                CHECKBOX_GetTextAlign (CHECKBOX_Handle hObj);
GUI_COLOR          CHECKBOX_GetTextColor (CHECKBOX_Handle hObj);
int                CHECKBOX_IsChecked    (CHECKBOX_Handle hObj);
void               CHECKBOX_SetBkColor   (CHECKBOX_Handle hObj, GUI_COLOR Color);
GUI_COLOR          CHECKBOX_SetBoxBkColor(CHECKBOX_Handle hObj, GUI_COLOR Color, int Index);
GUI_COLOR          CHECKBOX_SetFocusColor(CHECKBOX_Handle hObj, GUI_COLOR Color);
void               CHECKBOX_SetFont      (CHECKBOX_Handle hObj, const GUI_FONT * pFont);
void               CHECKBOX_SetImage     (CHECKBOX_Handle hObj, const GUI_BITMAP * pBitmap, unsigned int Index);
void               CHECKBOX_SetNumStates (CHECKBOX_Handle hObj, unsigned NumStates);
void               CHECKBOX_SetSpacing   (CHECKBOX_Handle hObj, unsigned Spacing);
void               CHECKBOX_SetState     (CHECKBOX_Handle hObj, unsigned State);
void               CHECKBOX_SetText      (CHECKBOX_Handle hObj, const char * pText);
void               CHECKBOX_SetTextAlign (CHECKBOX_Handle hObj, int Align);
void               CHECKBOX_SetTextColor (CHECKBOX_Handle hObj, GUI_COLOR Color);
int                CHECKBOX_SetUserData  (CHECKBOX_Handle hObj, const void * pSrc, int NumBytes);

/*********************************************************************
*
*       Member functions: Skinning
*
**********************************************************************
*/
void CHECKBOX_GetSkinFlexProps      (CHECKBOX_SKINFLEX_PROPS * pProps, int Index);
void CHECKBOX_SetSkinClassic        (CHECKBOX_Handle hObj);
void CHECKBOX_SetSkin               (CHECKBOX_Handle hObj, WIDGET_DRAW_ITEM_FUNC * pfDrawSkin);
int  CHECKBOX_DrawSkinFlex          (const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
void CHECKBOX_SetSkinFlexProps      (const CHECKBOX_SKINFLEX_PROPS * pProps, int Index);
void CHECKBOX_SetDefaultSkinClassic (void);
int  CHECKBOX_GetSkinFlexButtonSize (CHECKBOX_Handle hObj);
void CHECKBOX_SetSkinFlexButtonSize (CHECKBOX_Handle hObj, int ButtonSize);
WIDGET_DRAW_ITEM_FUNC * CHECKBOX_SetDefaultSkin(WIDGET_DRAW_ITEM_FUNC * pfDrawSkin);

#define CHECKBOX_SKIN_FLEX    CHECKBOX_DrawSkinFlex

/*********************************************************************
*
*       Macros for compatibility
*
**********************************************************************
*/

#define CHECKBOX_Check(hObj)   CHECKBOX_SetState(hObj, 1)
#define CHECKBOX_Uncheck(hObj) CHECKBOX_SetState(hObj, 0)

#define CHECKBOX_BI_INACTIV    CHECKBOX_BI_INACTIV_CHECKED
#define CHECKBOX_BI_ACTIV      CHECKBOX_BI_ACTIV_CHECKED

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // CHECKBOX_H

/*************************** End of file ****************************/
