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
File    : xmtx.c
Purpose : xmtx system interface -- thread locking and unlocking
          functions, adapted to embOS
--------- END-OF-HEADER ----------------------------------------------
*/

#include <yvals.h>
#include "RTOS.h"

#if _MULTI_THREAD  // Used in multi thread supported libraries only

_STD_BEGIN

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       __iar_system_Mtxinit()
*/
void __iar_system_Mtxinit(__iar_Rmtx *m) {
  OS__iar_system_Mtxinit(m);
}

/*********************************************************************
*
*       __iar_system_Mtxdst()
*/
void __iar_system_Mtxdst(__iar_Rmtx *m) {
  OS__iar_system_Mtxdst(m);
}

/*********************************************************************
*
*       __iar_system_Mtxlock()
*/
void __iar_system_Mtxlock(__iar_Rmtx *m) {
  OS__iar_system_Mtxlock(m);
}

/*********************************************************************
*
*       __iar_system_Mtxunlock()
*/
void __iar_system_Mtxunlock(__iar_Rmtx *m) {
  OS__iar_system_Mtxunlock(m);
}

_STD_END

/********************************************************************/

#endif // _MULTI_THREAD

/****** End Of File *************************************************/
