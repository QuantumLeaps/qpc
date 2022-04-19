/*****************************************************************************
* This is a stub for compiling QP port to VxWorks.
* Assembled from "VxWorks Reference Manual : Libraries".
*
* NOTE: This is NOT the original VxWorks header file!
* Also, this file contains only the minimum facilities for compiling
* the QP port, but it by no means complete.
*****************************************************************************/
#ifndef _vxworks_tickLib_h
#define _vxworks_tickLib_h

#include "windDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

STATUS tickAnnounceHookAdd
    (
    FUNCPTR tick_hook  /* tick hook function */
    );

void tickAnnounce (void);

void tickSet
    (
    unsigned int ticks /* new time in ticks */
    );

unsigned int tickGet (void);

#ifdef __cplusplus
}
#endif

#endif    /* !_vxworks_tickLib_h */
