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

File    : SEGGER_SYSVIEW_Config_embOS.c
Purpose : Sample setup configuration of SystemView with embOS.
Revision: $Rev: 21319 $
*/
#include "RTOS.h"
#include "SEGGER_SYSVIEW.h"
#include "SEGGER_SYSVIEW_embOS.h"

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/
#define DEMCR         (*(volatile U32*) (0xE000EDFCuL))  // Debug Exception and Monitor Control Register
#define TRACEENA_BIT  (1uL << 24)                        // Trace enable bit
#define DWT_CTRL      (*(volatile U32*) (0xE0001000uL))  // DWT Control Register
#define NOCYCCNT_BIT  (1uL << 25)                        // Cycle counter support bit
#define CYCCNTENA_BIT (1uL << 0)                         // Cycle counter enable bit
//
// If events will be recorded without a debug probe (J-Link) attached,
// enable the cycle counter
//
#define ENABLE_DWT_CYCCNT (SEGGER_SYSVIEW_POST_MORTEM_MODE || SEGGER_SYSVIEW_USE_INTERNAL_RECORDER)

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
/*********************************************************************
*
*       _cbSendSystemDesc()
*
*  Function description
*    Sends SystemView description strings.
*/
static void _cbSendSystemDesc(void) {
  SEGGER_SYSVIEW_SendSysDesc("N=" SEGGER_SYSVIEW_APP_NAME ",O=embOS,D=" SEGGER_SYSVIEW_DEVICE_NAME );
#ifdef SEGGER_SYSVIEW_SYSDESC0
  SEGGER_SYSVIEW_SendSysDesc(SEGGER_SYSVIEW_SYSDESC0);
#endif
#ifdef SEGGER_SYSVIEW_SYSDESC1
  SEGGER_SYSVIEW_SendSysDesc(SEGGER_SYSVIEW_SYSDESC1);
#endif
#ifdef SEGGER_SYSVIEW_SYSDESC2
  SEGGER_SYSVIEW_SendSysDesc(SEGGER_SYSVIEW_SYSDESC2);
#endif
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/
/*********************************************************************
*
*       SEGGER_SYSVIEW_Conf()
*
* Function description
*   Configure and initialize SystemView and register it with embOS.
*
* Additional information
*   If enabled, SEGGER_SYSVIEW_Conf() will also immediately start
*   recording events with SystemView.
*/
void SEGGER_SYSVIEW_Conf(void) {
#if ENABLE_DWT_CYCCNT
  //
  // If no debugger is connected, the DWT must be enabled by the application
  //
  if ((DEMCR & TRACEENA_BIT) == 0) {
    DEMCR |= TRACEENA_BIT;
  }
#endif
  //
  //  The cycle counter must be activated in order
  //  to use time related functions.
  //
  if ((DWT_CTRL & NOCYCCNT_BIT) == 0) {       // Cycle counter supported?
    if ((DWT_CTRL & CYCCNTENA_BIT) == 0) {    // Cycle counter not enabled?
      DWT_CTRL |= CYCCNTENA_BIT;              // Enable Cycle counter
    }
  }
  SEGGER_SYSVIEW_Init(SEGGER_SYSVIEW_TIMESTAMP_FREQ, SEGGER_SYSVIEW_CPU_FREQ,
                      &SYSVIEW_X_OS_TraceAPI, _cbSendSystemDesc);
  OS_TRACE_SetAPI(&embOS_TraceAPI_SYSVIEW);  // Configure embOS to use SYSVIEW.
#if SEGGER_SYSVIEW_START_ON_INIT
  SEGGER_SYSVIEW_Start();                    // Start recording to catch system initialization.
#endif
}

/*************************** End of file ****************************/
