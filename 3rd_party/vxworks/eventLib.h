/*****************************************************************************
* This is a stub for compiling QP port to VxWorks.
* Assembled from "VxWorks Reference Manual : Libraries".
*
* NOTE: This is NOT the original VxWorks header file!
* Also, this file contains only the minimum facilities for compiling
* the QP port, but it by no means complete.
*****************************************************************************/
#ifndef _vxworks_eventLib_h
#define _vxworks_eventLib_h

#include "windDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVENTS_WAIT_ANY      (0x1)
#define EVENTS_WAIT_ALL      (0x0)

#define EVENTS_RETURN_ALL    (0x2)
#define EVENTS_KEEP_UNWANTED (0x4)
#define EVENTS_FETCH         (0x80)

STATUS eventReceive
    (
    UINT32   events,          /* events task is waiting to occur */
    UINT8    options,         /* user options */
    int      timeout,         /* ticks to wait */
    UINT32 * pEventsReceived  /* events occured are returned through this */
    );

STATUS eventSend
    (
    int    taskId,            /* task events will be sent to */
    UINT32 events             /* events to send */
    );

STATUS eventClear (void);

#ifdef __cplusplus
}
#endif

#endif    /* !_vxworks_eventLib_h */
