/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2021 SEGGER Microcontroller GmbH                  *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
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
*       OS version: V5.14.0.0                                        *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : SEGGER_SYSVIEW_embOS.h
Purpose : Interface between embOS and System View.
Revision: $Rev: 9599 $
*/

#ifndef SYSVIEW_EMBOS_H
#define SYSVIEW_EMBOS_H

#include "RTOS.h"
#include "SEGGER_SYSVIEW.h"

// embOS trace API that targets SYSVIEW
extern const OS_TRACE_API          embOS_TraceAPI_SYSVIEW;

// Services provided to SYSVIEW by embOS
extern const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI;

#endif

/*************************** End of file ****************************/
