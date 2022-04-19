/*****************************************************************************
* This is a stub for compiling QP port to VxWorks.
* Assembled from "VxWorks Reference Manual : Libraries".
*
* NOTE: This is NOT the original VxWorks header file!
* Also, this file contains only the minimum facilities for compiling
* the QP port, but it by no means complete.
*****************************************************************************/
#ifndef _vxworks_msgQLib_h
#define _vxworks_msgQLib_h

#include "windDefs.h"

#define MSG_PRI_NORMAL (0)
#define MSG_PRI_URGENT (1)

#define MSG_PRI_NORMAL (0)
#define MSG_PRI_URGENT (1)

typedef unsigned MSG_Q_ID;

#ifdef __cplusplus
extern "C" {
#endif

MSG_Q_ID msgQCreate
    (
    size_t maxMsgs,       /* max messages that can be queued */
    size_t maxMsgLength,  /* max bytes in a message */
    int options           /* message queue options */
    );

STATUS msgQDelete
    (
    MSG_Q_ID msgQId       /* message queue to delete */
    );

STATUS msgQSend
    (
    MSG_Q_ID msgQId,      /* message queue on which to send */
    char *   buffer,      /* message to send */
    size_t   nBytes,      /* length of message */
    unsigned int timeout, /* ticks to wait */
    int      priority     /* MSG_PRI_NORMAL or MSG_PRI_URGENT */
    )

int msgQReceive
    (
    MSG_Q_ID msgQId,      /* message queue from which to receive */
    char *   buffer,      /* buffer to receive message */
    size_t   maxNBytes,   /* length of buffer */
    unsigned int timeout  /* ticks to wait */
    )

int msgQNumMsgs
    (
    MSG_Q_ID msgQId       /* message queue to examine */
    )

#ifdef __cplusplus
}
#endif

#endif    /* !_vxworks_msgQLib_h */
