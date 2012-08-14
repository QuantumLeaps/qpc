#ifndef _vxworks_intLib_h
#define _vxworks_intLib_h

#include "windDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

int intLock(void);

void intUnlock(int lockKey);

#ifdef __cplusplus
}
#endif

#endif    /* !_vxworks_intLib_h */

