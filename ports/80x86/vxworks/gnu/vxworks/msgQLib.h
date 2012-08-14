#ifndef _vxworks_msgQLib_h
#define _vxworks_msgQLib_h

#include "windDefs.h"

#define MSG_PRI_NORMAL     0x00
#define MSG_PRI_URGENT     0x01
#define MSG_Q_FIFO         0x00
#define MSG_Q_PRIORITY     0x01

typedef unsigned MSG_Q_ID;

#ifdef __cplusplus
extern "C" {
#endif

MSG_Q_ID msgQCreate(int maxMsg,
            unsigned maxLength,
            int options);

STATUS msgQDelete(MSG_Q_ID qid);

int msgQReceive(MSG_Q_ID qid,
        char *buffer,
        unsigned nBytes,
        int timeout);

STATUS msgQSend(MSG_Q_ID qid,
        char *buffer,
        unsigned nBytes,
        int timeout,
        int priority);

int msgQNumMsgs(MSG_Q_ID qid);

#ifdef __cplusplus
}
#endif

#endif    /* !_vxworks_msgQLib_h */
