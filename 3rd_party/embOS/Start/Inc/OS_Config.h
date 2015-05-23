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
File    : OS_Config.h
Purpose : Configuration settings for the OS build and OS_VIEW
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef OS_CONFIG_H                     /* Avoid multiple inclusion */
#define OS_CONFIG_H

/*********************************************************************
*
*       Default configuration
*
**********************************************************************
*/

#ifndef   DEBUG           // Should be overwritten by project settings
  #define DEBUG     (0)   // in debug builds
#endif

/*********************************************************************
*
*       Configuration for RTOS build and UART
*
*  One of the following builds needs to be selected for both DEBUG and Release builds:
*
*  OS_LIBMODE_XR    Extremely small release build without Round robin
*  OS_LIBMODE_R     Release build
*  OS_LIBMODE_S     Release build with stack check
*  OS_LIBMODE_SP    Release build with stack check and profiling
*  OS_LIBMODE_D     Debug build
*  OS_LIBMODE_DP    Debug build with profiling
*  OS_LIBMODE_DT    Debug build with trace
*/

#if DEBUG
  #define OS_LIBMODE_DP
#else
  #define OS_LIBMODE_R
  #define OS_VIEW_ENABLE (0)
#endif

/********************************************************************/

#endif                                  /* Avoid multiple inclusion */

/****** End Of File *************************************************/
