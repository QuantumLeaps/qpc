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
File    : xmtx2.c
Purpose : xmtx file interface -- thread locking and unlocking
          functions for file i/o, adapted to embOS
--------- END-OF-HEADER ----------------------------------------------
*/

#include <yvals.h>
#include "RTOS.h"

_STD_BEGIN

#if _MULTI_THREAD

#if _DLIB_FILE_DESCRIPTOR

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       __iar_file_Mtxinit()
*/
void __iar_file_Mtxinit(__iar_Rmtx *m) {
  OS__iar_system_Mtxinit(m);    // Mapped to system mutex
}

/*********************************************************************
*
*       __iar_file_Mtxdst()
*/
void __iar_file_Mtxdst(__iar_Rmtx *m) {
  OS__iar_system_Mtxdst(m);     // Mapped to system mutex
}

/*********************************************************************
*
*       __iar_file_Mtxlock()
*/
void __iar_file_Mtxlock(__iar_Rmtx *m) {
  OS__iar_system_Mtxlock(m);    // Mapped to system mutex
}

/*********************************************************************
*
*       __iar_file_Mtxunlock()
*/
void __iar_file_Mtxunlock(__iar_Rmtx *m) {
  OS__iar_system_Mtxunlock(m);  // Mapped to system mutex
}
#endif /* _DLIB_FILE_DESCRIPTOR */

#endif /* _MULTI_THREAD */

_STD_END

/****** End Of File *************************************************/
