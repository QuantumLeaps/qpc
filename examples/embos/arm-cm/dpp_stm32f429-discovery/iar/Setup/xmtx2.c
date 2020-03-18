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
Purpose : xmtx file interface -- thread locking and unlocking
          functions for file i/o
*/

#include <yvals.h>
#include "RTOS.h"

#if ((_MULTI_THREAD != 0) && (_DLIB_FILE_DESCRIPTOR != 0))

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
__ATTRIBUTES void __iar_file_Mtxinit(__iar_Rmtx* m) {
  OS__iar_system_Mtxinit(m);
}

/*********************************************************************
*
*       __iar_file_Mtxdst()
*/
__ATTRIBUTES void __iar_file_Mtxdst(__iar_Rmtx* m) {
  OS__iar_system_Mtxdst(m);
}

/*********************************************************************
*
*       __iar_file_Mtxlock()
*/
__ATTRIBUTES void __iar_file_Mtxlock(__iar_Rmtx* m) {
  OS__iar_system_Mtxlock(m);
}

/*********************************************************************
*
*       __iar_file_Mtxunlock()
*/
__ATTRIBUTES void __iar_file_Mtxunlock(__iar_Rmtx* m) {
  OS__iar_system_Mtxunlock(m);
}

#endif  // ((_MULTI_THREAD != 0) && (_DLIB_FILE_DESCRIPTOR != 0))

/*************************** End of file ****************************/
