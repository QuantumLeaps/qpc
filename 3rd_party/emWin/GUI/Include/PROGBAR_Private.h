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
File        : PROGBAR_Private.h
Purpose     : Internal header file
---------------------------END-OF-HEADER------------------------------
*/

#ifndef PROGBAR_PRIVATE_H
#define PROGBAR_PRIVATE_H

#include "GUI_Private.h"
#include "WM_Intern.h"
#include "PROGBAR.h"
#include "WIDGET.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define PROGBAR_SF_HORIZONTAL PROGBAR_CF_HORIZONTAL
#define PROGBAR_SF_VERTICAL   PROGBAR_CF_VERTICAL
#define PROGBAR_SF_USER       PROGBAR_CF_USER

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  WIDGET_DRAW_ITEM_FUNC * pfDrawSkin;
} PROGBAR_SKIN_PRIVATE;

typedef struct {
  const GUI_FONT * pFont;
  GUI_COLOR aBarColor[2];
  GUI_COLOR aTextColor[2];
  PROGBAR_SKIN_PRIVATE SkinPrivate;
} PROGBAR_PROPS;

typedef struct {
  WIDGET Widget;
  int v;
  WM_HMEM hpText;
  I16 XOff, YOff;
  I16 TextAlign;
  int Min, Max;
  PROGBAR_PROPS Props;
  WIDGET_SKIN const * pWidgetSkin;
  U8 Flags;
} PROGBAR_Obj;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define PROGBAR_INIT_ID(p) p->Widget.DebugId = PROGBAR_ID
#else
  #define PROGBAR_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  PROGBAR_Obj * PROGBAR_LockH(PROGBAR_Handle h);
  #define PROGBAR_LOCK_H(h)   PROGBAR_LockH(h)
#else
  #define PROGBAR_LOCK_H(h)   (PROGBAR_Obj *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Public data (internal defaults)
*
**********************************************************************
*/
extern PROGBAR_PROPS PROGBAR__DefaultProps;

extern const WIDGET_SKIN PROGBAR__SkinClassic;
extern       WIDGET_SKIN PROGBAR__Skin;

extern WIDGET_SKIN const * PROGBAR__pSkinDefault;

/*********************************************************************
*
*       Public functions (internal)
*
**********************************************************************
*/
char * PROGBAR__GetTextLocked(const PROGBAR_Obj * pObj);
void   PROGBAR__GetTextRect  (const PROGBAR_Obj * pObj, GUI_RECT * pRect, const char * pText);
int    PROGBAR__Value2Pos    (const PROGBAR_Obj * pObj, int v);

#endif /* GUI_WINSUPPORT */
#endif /* PROGBAR_PRIVATE_H */

/*************************** End of file ****************************/
