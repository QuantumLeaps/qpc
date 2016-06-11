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
File        : GUI_SPRITE_Private.h
Purpose     : Private header file for sprites
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_SPRITE_PRIVATE_H
#define GUI_SPRITE_PRIVATE_H

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define SPRITE_LOCK_H(h) (GUI_SPRITE_OBJ *)GUI_LOCK_H(h)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
//
// The sprite object
//
typedef struct {
  GUI_DEVICE       * pDevice;
  GUI_RECT           Rect;
  GUI_HMEM           hColors;
  U16                Flags;
  const GUI_BITMAP * pBM;
  void            (* pCB)(GUI_HSPRITE hSprite, int Cmd); // Callback routine for animated sprites
  GUI_HMEM           hContext;
} GUI_SPRITE_OBJ;


#endif // GUI_SPRITE_PRIVATE_H

/*************************** End of file ****************************/
