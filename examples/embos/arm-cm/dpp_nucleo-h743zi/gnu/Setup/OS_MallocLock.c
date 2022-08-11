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

----------------------------------------------------------------------
File    : OS_MallocLock.c
Purpose : embOS implementation of thread safe malloc function locking
          Has to be included in the project if thread safe heap
          management is required
--------  END-OF-HEADER  ---------------------------------------------
*/

#include <malloc.h>
#include "RTOS.h"

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/

#ifdef __cplusplus
  extern "C" {
#endif

/*********************************************************************
*
*       Declare functions to avoid "no prototype" warning
*/
void __malloc_lock ( struct _reent *_r );
void __malloc_unlock ( struct _reent *_r );

#ifdef __cplusplus
  }
#endif

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       __malloc_lock() replaces the dummy from the library
*/
void __malloc_lock ( struct _reent *_r ) {
  OS_USEPARA(_r);
  OS_MallocLock();
}

/*********************************************************************
*
*       __malloc_unlock() replaces the dummy from the library
*/
void __malloc_unlock ( struct _reent *_r ) {
  OS_USEPARA(_r);
  OS_MallocUnlock();
}

/****** End Of File *************************************************/
