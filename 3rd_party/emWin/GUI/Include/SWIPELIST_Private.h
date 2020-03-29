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
File        : SWIPELIST.h
Purpose     : SWIPELIST include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef SWIPELIST_PRIVATE_H
#define SWIPELIST_PRIVATE_H

#include "WM_Intern.h"
#include "SWIPELIST.h"
#include "GUI_ARRAY.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  WM_HWIN hWin;
  int     xPos;
  int     yPos;
} SWIPELIST_WIN;

typedef struct {
  GUI_ARRAY      WinArray;
  GUI_ARRAY      TextArray;
  int            TextSize;
  WM_HMEM        hDrawObj;
  GUI_COLOR      SepColor;
  int            SepSize;
  int            Pos;
  int            Size;
  int            BitmapAlign;
  int            TextAlign;
  U8             Flags;
  U32            UserData;
} SWIPELIST_ITEM;

typedef struct {
  const GUI_FONT * pSepFont;
  const GUI_FONT * pHeaderFont;
  const GUI_FONT * pTextFont;
  GUI_COLOR        aTextColor[5];
  GUI_COLOR        aBkColor[3];
  int              BitmapSpace;
  int              aBorderSize[4];
  U8               Flags;
  int              Threshold;
} SWIPELIST_PROPS;

typedef struct {
  WIDGET                  Widget;
  SWIPELIST_PROPS         Props;
  WIDGET_DRAW_ITEM_FUNC * pfDrawItem;
  GUI_ARRAY               ItemArray;
  int                     Pos;
  int                     Size;
  int                     FirstVisible;
  int                     LastVisible;
  int                     Sel;
  int                     ReleasedItem;
} SWIPELIST_OBJ;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define SWIPELIST_INIT_ID(p) p->Widget.DebugId = SWIPELIST_ID
#else
  #define SWIPELIST_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  SWIPELIST_OBJ * SWIPELIST_LockH(SWIPELIST_Handle h);
  #define SWIPELIST_LOCK_H(h)   SWIPELIST_LockH(h)
#else
  #define SWIPELIST_LOCK_H(h)   (SWIPELIST_OBJ *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Module internal data
*
**********************************************************************
*/
extern SWIPELIST_PROPS SWIPELIST__DefaultProps;
extern GUI_COLOR       SWIPELIST__DefaultSepColor;
extern int             SWIPELIST__DefaultSepSize;
extern int             SWIPELIST__DefaultTextAlign;

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/
void SWIPELIST__SetDrawObj   (SWIPELIST_Handle hObj, int Index, int Align, GUI_DRAW_HANDLE hDrawObj);
void SWIPELIST__CalcItemPos  (GUI_ARRAY ItemArray, int StartIndex, int EndIndex, int AddSize);
int  SWIPELIST__CalcItemSize (SWIPELIST_Handle hObj, int ItemIndex);

#endif   /* if GUI_WINSUPPORT */
#endif   /* SWIPELIST_PRIVATE_H */
