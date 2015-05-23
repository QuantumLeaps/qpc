/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co KG                  *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2014  SEGGER Microcontroller GmbH & Co KG         *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: 4.00                                             *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : JLINKMEM.h
Purpose : Header file for J-Link ARM communication using memory
---------------------------END-OF-HEADER------------------------------
*/

#ifndef JLINKMEM_H
#define JLINKMEM_H             // Avoid multiple inclusion

void JLINKMEM_Process(void);
void JLINKMEM_SetpfOnRx(void (* pf)(unsigned char Data));
void JLINKMEM_SetpfOnTx(void (* pf)(void));
void JLINKMEM_SetpfGetNextChar(OS_INT (* pf)(void));
void JLINKMEM_SendChar(unsigned char Data);

#endif                         // Avoid multiple inclusion

/*************************** end of file ****************************/

