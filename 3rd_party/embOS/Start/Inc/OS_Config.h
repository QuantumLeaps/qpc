/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2019 SEGGER Microcontroller GmbH                  *
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
*       OS version: 5.06.1                                           *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
Purpose : Configuration settings for the OS build and embOSView
*/

#ifndef OS_CONFIG_H
#define OS_CONFIG_H

/*********************************************************************
*
*       Configuration for RTOS build and embOSView communication
*
*  In your application program, you need to let the compiler know
*  which build of embOS you are using. This is done by adding the
*  corresponding define to your preprocessor settings and linking the
*  appropriate library file.
*
*  OS_LIBMODE_XR    Extremely small release build without Round robin
*  OS_LIBMODE_R     Release build
*  OS_LIBMODE_S     Release build with stack check
*  OS_LIBMODE_SP    Release build with stack check and profiling
*  OS_LIBMODE_D     Debug build
*  OS_LIBMODE_DP    Debug build with profiling
*  OS_LIBMODE_DT    Debug build with trace
*
*  If no preprocessor setting is used, this file will select default
*  modes for debug and release configurations of your project.
*/

#if (defined(DEBUG) && (DEBUG == 1))
  #define OS_LIBMODE_DP
#else
  #define OS_LIBMODE_R
  #define OS_VIEW_IFSELECT  OS_VIEW_DISABLED  // embOSView communication is disabled per default in release configuration
#endif

/*********************************************************************
*
*       Additional embOS compile time configuration defines when using
*       embOS sources in your project or rebuilding the embOS libraries
*       can be added here, e.g.:
*       #define OS_SUPPORT_TICKLESS  0  // Disable tickless support
*/

#endif  // OS_CONFIG_H

/*************************** End of file ****************************/
