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
File        : SIM.h
Purpose     : Contains macros for compatibility
----------------------------------------------------------------------
*/

#ifndef SIM_H
#define SIM_H

#include "LCD_SIM.h"
//#include "GUI_SIM_NoWin32.h"

/********************************************************************
*
*              Publics for Hardkey simulation
*
*********************************************************************
*/
typedef void SIM_HARDKEY_CB(int KeyIndex, int State);

int              SIM_HARDKEY_GetNum(void);
int              SIM_HARDKEY_GetState(unsigned int i);
SIM_HARDKEY_CB*  SIM_HARDKEY_SetCallback(unsigned int KeyIndex, SIM_HARDKEY_CB* pfCallback);
int              SIM_HARDKEY_SetMode (unsigned int KeyIndex, int Mode);
int              SIM_HARDKEY_SetState(unsigned int KeyIndex, int State);

/********************************************************************
*
*              Publics for LCD
*
*********************************************************************
*/
#define SIM_SetLCDPos(x, y)                 SIM_GUI_SetLCDPos(x, y)
#define SIM_SetTransColor(Color)            SIM_GUI_SetTransColor(Color)
#define SIM_SetLCDColorBlack(Index, Color)  SIM_GUI_SetLCDColorBlack(Index, Color)
#define SIM_SetLCDColorWhite(Index, Color)  SIM_GUI_SetLCDColorWhite(Index, Color)
#define SIM_SetMag(MagX, MagY)              SIM_GUI_SetMag(MagX, MagY)
#define SIM_GetMagX()                       SIM_GUI_GetMagX()
#define SIM_GetMagY()                       SIM_GUI_GetMagY()

/********************************************************************
*
*              Publics used by GUI_X module
*
*********************************************************************
*/
#define SIM_Delay(ms)     SIM_GUI_Delay(ms)
#define SIM_ExecIdle()    SIM_GUI_ExecIdle()
#define SIM_GetTime()     SIM_GUI_GetTime()
#define SIM_GetKey()      SIM_GUI_GetKey()
#define SIM_WaitKey()     SIM_GUI_WaitKey()
#define SIM_StoreKey(Key) SIM_GUI_StoreKey(Key)

/********************************************************************
*
*              Publics for logging, warning, errorout
*
*********************************************************************
*/
#define SIM_Log(s)                          SIM_GUI_Log(s)
#define SIM_Log1(s, p0)                     SIM_GUI_Log1(s, p0)
#define SIM_Log2(s, p0, p1)                 SIM_GUI_Log2(s, p0, p1)
#define SIM_Log3(s, p0, p1, p2)             SIM_GUI_Log3(s, p0, p1, p2)
#define SIM_Log4(s, p0, p1, p2,p3)          SIM_GUI_Log4(s, p0, p1, p2,p3)
#define SIM_Warn(s)                         SIM_GUI_Warn(s)
#define SIM_Warn1(s, p0)                    SIM_GUI_Warn1(s, p0)
#define SIM_Warn2(s, p0, p1)                SIM_GUI_Warn2(s, p0, p1)
#define SIM_Warn3(s, p0, p1, p2)            SIM_GUI_Warn3(s, p0, p1, p2)
#define SIM_Warn4(s, p0, p1, p2, p3)        SIM_GUI_Warn4(s, p0, p1, p2, p3)
#define SIM_ErrorOut(s)                     SIM_GUI_ErrorOut(s)
#define SIM_ErrorOut1(s, p0)                SIM_GUI_ErrorOut1(s, p0)
#define SIM_ErrorOut2(s, p0, p1)            SIM_GUI_ErrorOut2(s, p0, p1)
#define SIM_ErrorOut3(s, p0, p1, p2)        SIM_GUI_ErrorOut3(s, p0, p1, p2)
#define SIM_ErrorOut4(s, p0, p1, p2, p3)    SIM_GUI_ErrorOut4(s, p0, p1, p2, p3)
#define SIM_EnableMessageBoxOnError(Status) SIM_GUI_EnableMessageBoxOnError(Status)

/********************************************************************
*
*              Commandline support
*
*********************************************************************
*/

#define SIM_GetCmdLine() SIM_GUI_GetCmdLine()

/********************************************************************
*
*              Multitasking support
*
*********************************************************************
*/

#define SIM_CreateTask(pName, pFunc) SIM_GUI_CreateTask(pName, pFunc)
#define SIM_Start()                  SIM_GUI_Start()
#define SIM_GetTaskID()              SIM_GUI_GetTaskID()
#define SIM_Lock()                   SIM_GUI_Lock()
#define SIM_Unlock()                 SIM_GUI_Unlock()
#define SIM_InitOS()                 SIM_GUI_InitOS()

#endif /* LCD_H */




