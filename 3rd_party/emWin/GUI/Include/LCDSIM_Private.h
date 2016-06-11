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
File        : LCDSIM_Private.h
Purpose     : Declares LCD Private interface
----------------------------------------------------------------------
*/

#ifndef LCDSIM_PRIVATE_H
#define LCDSIM_PRIVATE_H

#include "LCD_SIM.h"

extern int LCDSIM_aLCDColorWhite[LCDSIM_MAX_DISPLAYS];
extern int LCDSIM_aLCDColorBlack[LCDSIM_MAX_DISPLAYS];

/********************************************************************
*
*       Memory offsets
*
*********************************************************************
*/
#define LCDSIM_OFF_XSIZE           32
#define LCDSIM_OFF_YSIZE           36
#define LCDSIM_OFF_VXSIZE          40
#define LCDSIM_OFF_VYSIZE          44
#define LCDSIM_OFF_CONTROLLER      48

#define LCDSIM_OFF_BPP             52
#define LCDSIM_OFF_NUMCOLORS       56

#define LCDSIM_OFF_MODIFY_CNT      60
#define LCDSIM_OFF_LUT_MODIFY_CNT  64

#define LCDSIM_OFF_FIXEDPALETTE    68
#define LCDSIM_OFF_DELTA_MODE      72

#define LCDSIM_OFF_XPOS_PID        80
#define LCDSIM_OFF_YPOS_PID        84
#define LCDSIM_OFF_KEYSTAT         88

#define LCDSIM_OFF_ORGX            92
#define LCDSIM_OFF_ORGY            96

#define LCDSIM_OFF_ALPHA          100

#define LCDSIM_OFF_XPOS_LAYER     104
#define LCDSIM_OFF_YPOS_LAYER     108

#define LCDSIM_OFF_VIS_LAYER      112

#define LCDSIM_OFF_COMPOSIT_COLOR 116
#define LCDSIM_OFF_COMPOSIT_SIZE  120

#define LCDSIM_OFF_CHROMA_MIN     124
#define LCDSIM_OFF_CHROMA_MAX     128

#define LCDSIM_OFF_TRANSMODE      132

#define LCDSIM_FRAMEBUFFER_SIZE 0x1000000

/********************************************************************
*
*       Touch Interface
*
*********************************************************************
*/
#if (_MSC_VER < 1600)

DECLARE_HANDLE(HTOUCHINPUT);

typedef unsigned long ULONG_PTR;

typedef struct tagTOUCHINPUT {
    LONG x;
    LONG y;
    HANDLE hSource;
    DWORD dwID;
    DWORD dwFlags;
    DWORD dwMask;
    DWORD dwTime;
    ULONG_PTR dwExtraInfo;
    DWORD cxContact;
    DWORD cyContact;
} TOUCHINPUT, *PTOUCHINPUT;
typedef TOUCHINPUT const * PCTOUCHINPUT;

#define TWF_FINETOUCH       (0x00000001)
#define TWF_WANTPALM        (0x00000002)

#define WM_TOUCH                        0x0240
#define WM_GESTURE                      0x0119
#define WM_GESTURENOTIFY                0x011A

#endif

typedef BOOL (__stdcall * T_RegisterTouchWindow)  (HWND hWnd, ULONG ulFlags);
typedef BOOL (__stdcall * T_GetTouchInputInfo)    (HTOUCHINPUT hTouchInput, UINT cInputs, PTOUCHINPUT pInputs, int cbSize);
typedef BOOL (__stdcall * T_CloseTouchInputHandle)(HTOUCHINPUT hTouchInput);

#define TOUCHEVENTF_MOVE            0x0001
#define TOUCHEVENTF_DOWN            0x0002
#define TOUCHEVENTF_UP              0x0004
#define TOUCHEVENTF_INRANGE         0x0008
#define TOUCHEVENTF_PRIMARY         0x0010
#define TOUCHEVENTF_NOCOALESCE      0x0020
#define TOUCHEVENTF_PEN             0x0040
#define TOUCHEVENTF_PALM            0x0080

/********************************************************************
*
*       Private Interface
*
*********************************************************************
*/
void SIM_GUI__MessageBox1(LPCTSTR lpText);
void SIM_GUI__OnMouse    (WPARAM wParam, LPARAM lParam, int DblClick, int LayerIndex);
int  SIM_GUI__OnTouch    (HWND hWnd, WPARAM wParam, LPARAM lParam, int LayerIndex);
void SIM_GUI__EnableTouch(HWND hWnd);

#endif /* Avoid multiple inclusion */




