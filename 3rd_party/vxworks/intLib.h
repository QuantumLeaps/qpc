/*****************************************************************************
* This is a stub for compiling QP port to VxWorks.
* Assembled from "VxWorks Reference Manual : Libraries".
*
* NOTE: This is NOT the original VxWorks header file!
* Also, this file contains only the minimum facilities for compiling
* the QP port, but it by no means complete.
*****************************************************************************/
#ifndef _vxworks_intLib_h
#define _vxworks_intLib_h

#include "windDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOL intContext (void);

int intCount (void);

/* from intArchLib... */
int intLock (void);

void intUnlock
    (
    int lockKey /* lock-out key returned by preceding intLock() */
    );

#ifdef __cplusplus
}
#endif

#endif    /* !_vxworks_intLib_h */
