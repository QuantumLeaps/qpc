/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2017 SEGGER Microcontroller GmbH & Co. KG         *
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
*       OS version: 4.34.1                                           *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : SEGGER_SYSVIEW_embOS.h
Purpose : Interface between embOS and System View.
Revision: $Rev: 3734 $
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
