/*****************************************************************************
* This is a stub for compiling QP port to VxWorks.
* Assembled from "VxWorks Reference Manual : Libraries".
*
* NOTE: This is NOT the original VxWorks header file!
* Also, this file contains only the minimum facilities for compiling
* the QP port, but it by no means complete.
*****************************************************************************/
#ifndef _vxworks_taskLib_h
#define _vxworks_taskLib_h

#include "windDefs.h"

#define VX_FP_TASK        (0x01000000)
#define VX_NO_STACK_FILL  (0x0100)
#define VX_PRIVATE_ENV    (0x0080)
#define VX_UNBREAKABLE    (0x0002)


#ifdef __cplusplus
extern "C" {
#endif

typedef int TASK_ID;

#define TASK_ID_NULL (0)

STATUS taskActivate
    (
    int tid /* task ID of task to activate */
    );

STATUS taskDelete
    (
    int tid /* task ID of task to delete */
    );

STATUS taskDeleteForce
    (
    int tid /* task ID of task to delete */
    );

STATUS taskSuspend
    (
    int tid /* task ID of task to suspend */
    );

STATUS taskResume
    (
    int tid /* task ID of task to resume */
    );

void taskExit
    (
    int code /* code stored in TCB for delete hooks */
    );

STATUS taskLock (void);

STATUS taskUnlock (void);

STATUS taskPriorityGet
    (
    int   tid,      /* task ID */
    int * pPriority /* return priority here */
    );

STATUS taskPrioritySet
    (
    int tid,        /* task ID */
    int newPriority /* new priority */
    );

STATUS taskPriorityGet
    (
    int   tid,      /* task ID */
    int * pPriority /* return priority here */
    );

int taskIdSelf (void);

int taskSpawn
    (
    char *  name,      /* name of new task (stored at pStackBase) */
    int     priority,  /* priority of new task */
    int     options,   /* task option word */
    size_t  stackSize, /* size (bytes) of stack needed plus name */
    FUNCPTR entryPt,   /* entry point of new task */
    int     arg1,      /* 1st of 10 req'd task args to pass to func */
    int     arg2,
    int     arg3,
    int     arg4,
    int     arg5,
    int     arg6,
    int     arg7,
    int     arg8,
    int     arg9,
    int     arg10
    );

#ifdef __cplusplus
}
#endif

#endif    /* !_vxworks_taskLib_h */
