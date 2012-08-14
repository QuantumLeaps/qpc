#ifndef _vxworks_taskLib_h
#define _vxworks_taskLib_h

#include "windDefs.h"

#define VX_FP_TASK         0x8
#define VX_NO_STACK_FILL   0x100
#define VX_PRIVATE_ENV     0x80
#define VX_UNBREAKABLE     0x2


#ifdef __cplusplus
extern "C" {
#endif

STATUS taskActivate(int tid);

#define taskHookInit()

STATUS taskCreateHookAdd(FUNCPTR createHook);

STATUS taskSwitchHookAdd(FUNCPTR switchHook);

STATUS taskDeleteHookAdd(FUNCPTR deleteHook);

STATUS taskCreateHookDelete(FUNCPTR createHook);

STATUS taskSwitchHookDelete(FUNCPTR switchHook);

STATUS taskDeleteHookDelete(FUNCPTR deleteHook);

STATUS taskDelay (int ticks);

STATUS taskDelete(int tid);

STATUS taskDeleteForce(int tid);

void taskExit__(int status);

int taskIdDefault(int tid);

int taskIdSelf(void);

STATUS taskIdVerify(int tid);

STATUS taskLock(void);

STATUS taskPriorityGet(int tid,
               int *pPriority);

STATUS taskPrioritySet(int tid,
               int priority);

STATUS taskRestart(int tid);

STATUS taskResume(int tid);

STATUS taskSafe(void);

int taskSpawn(const char *name,
          int priority,
          int options,
          int stacksize,
          FUNCPTR entry,
          int arg1,
          int arg2,
          int arg3,
          int arg4,
          int arg5,
          int arg6,
          int arg7,
          int arg8,
          int arg9,
          int arg10);

STATUS taskSuspend(int tid);

STATUS taskUnlock(void);

STATUS taskUnsafe(void);

#ifdef __cplusplus
}
#endif

#endif    /* !_vxworks_taskLib_h */

