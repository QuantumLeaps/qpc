#ifndef _vxworks_tickLib_h
#define _vxworks_tickLib_h

#include "windDefs.h"

typedef unsigned long ULONG;

#ifdef __cplusplus
extern "C" {
#endif

void tickAnnounce(void);

void tickSet(ULONG ticks /* new time in ticks */);

ULONG tickGet(void);


#ifdef __cplusplus
}
#endif

#endif    /* !_vxworks_tickLib_h */

