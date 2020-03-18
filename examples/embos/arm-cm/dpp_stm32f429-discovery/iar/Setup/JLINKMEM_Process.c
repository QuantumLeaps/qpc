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

----------------------------------------------------------------------
File    : JLINKMEM_Process.c
Purpose : Data handler for ARM J-Link communication over memory

Layout of communication area:

  +----------+  TOS - TX_SIZE - RX_SIZE - 6
  |          |
  | RX_BUF   |
  |          |
  +----------+  TOS - TX_SIZE - 6
  | RX_CNT   |
  +----------+  TOS - TX_SIZE - 5
  | HOST_ACT |
  +----------+  TOS - TX_SIZE - 4
  | TX_CNT   |
  +----------+  TOS - TX_SIZE - 3
  |          |
  | TX_BUF   |
  |          |
  +----------+  TOS - 3
  | TX_SIZE  |
  +----------+  TOS - 2
  | RX_SIZE  |
  +----------+  TOS - 1
  | PROT_ID  |
  +----------+  TOS

TOS       Initial top of stack as defined by linker settings (top of CSTACK)
PROT_ID   Magic number indicating the start of communication area
RX_SIZE   Size of receiving buffer in bytes
TX_SIZE   Size of sending buffer in bytes
TX_BUF    Sending buffer
TX_CNT    Number of bytes in sending buffer
HOST_ACT  Set to oen by host to indicate it is still active
RX_CNT    Number of bytes in the receiving buffer
RX_BUF    Receiving buffer

---------------------------END-OF-HEADER------------------------------
*/

#include "RTOS.h"
#include "JLINKMEM.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

/*********************************************************************
*
*       Sizes of receiving and sending buffer.
*
* NOTE:
*   Before you change any of these values make sure OS_Start()
*   reserves enough bytes for the communication area.
*/
#define RX_BUF_SIZE   8
#define TX_BUF_SIZE   18

/*********************************************************************
*
*       Defines, non- configurable
*
**********************************************************************
*/

#define U8  unsigned char
#define U16 unsigned short
#define U32 unsigned int

#define MAGIC_NUM   0x52 /* Identifies the start of our communication area. */

#define PROT_ID     (*(volatile U8 *)(_BaseAddr - 1))  /* Id of the protocol. Always set to MAGIC_NUM */
#define RX_SIZE     (*(volatile U8 *)(_BaseAddr - 2))  /* Size of receiving buffer in bytes */
#define TX_SIZE     (*(volatile U8 *)(_BaseAddr - 3))  /* Size of sending buffer in bytes */
#define TX_CNT      (*(volatile U8 *)(_BaseAddr - TX_BUF_SIZE - 4))  /* Stores the number of bytes we send to host */
/* Set by host to a non-null value when it connects to target.
 * The target sets this to null when it detects a communication timeout.
 */

#define HOST_CON    (*(volatile U8 *)(_BaseAddr - TX_BUF_SIZE - 5))
#define RX_CNT      (*(volatile U8 *)(_BaseAddr - TX_BUF_SIZE - 6)) /* Stores the number of bytes the host sent to us */

#define TX_TIMEOUT  1000 /* Time to wait for host to fetch the data from sending buffer (in ms) */

#define VTOR_ADDR   (*(volatile OS_U32*) (0xE000ED08)) /* Vector table base register address */

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

/* Pointer to user callbacks */
void (* _pfOnRx) (U8 Data);
U8 (* _pfOnTx) (void);
OS_INT (* _pfGetNextChar) (void);

static U32  _BaseAddr;
static int  _TxIsPending;    /* Set when there is a character waiting to be sent */
static U8   _TxPendingData;  /* Holds tha character waiting to be sent */
static int  _IsInited;       /* Set when the communication is initialised */
static U8*  _pRxBuf;         /* Start of receiving buffer */
static U8*  _pTxBuf;         /* Start of sending buffer */

/* Supervises the connection to host */
static int  _TxTimeout;      /* Set when the host fails to fetch our data */
static int  _TxTimeoutTimer;
static int  _TxBufLocked;    /* Serialises the access to our sending buffer */

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _RetriggerTimer
*
* Function description
*/
static void _RetriggerTimer(void) {
  _TxTimeoutTimer = TX_TIMEOUT;
}

/*********************************************************************
*
*       _HandleTxTimeout
*
* Function description
*   Sets the connection to host to inactive when the host fails to
*   fetch the data we send to it. Called when the send timeout
*   expires.
*/
static void _StopTimer(void) {
  _TxTimeoutTimer = 0;
}

/*********************************************************************
*
*       _Init
*
* Function description
*   Performs initalisation of memory locations used for communication.
*/
static void _Init(void) {
  OS_U32 VectorTableBaseAddr;
  /* On Cortex-M initial top of stack is stored at address defined by VTOR */
  VectorTableBaseAddr  = VTOR_ADDR;
  VectorTableBaseAddr &= ~(0x3FuL);  // Mask b6..b0 as those might not be zero ARM DDI 0403D chapter B3.2.5 page B3-716
  _BaseAddr = (*(OS_U32*)VectorTableBaseAddr);
  HOST_CON  = 0;
  RX_SIZE   = RX_BUF_SIZE;
  TX_SIZE   = TX_BUF_SIZE;
  RX_CNT    = 0;
  TX_CNT    = 0;
  PROT_ID   = MAGIC_NUM;
  _pTxBuf   = (U8 *)(_BaseAddr - TX_BUF_SIZE - 3);
  _pRxBuf   = (U8 *)(_BaseAddr - TX_BUF_SIZE - RX_BUF_SIZE - 6);
  _TxIsPending = 0;
}

/*********************************************************************
*
*       _LockTxBuf
*
* Function description
*   Gains exclusive access to sending buffer.
*
* Return value
*   1   Sending buffer locked
*   0   Sending buffer couldn't be locked as already in use.
*/
static int _LockTxBuf(void) {
  int Locked = 0;

  OS_INT_Disable();
  if (!_TxBufLocked) {
    _TxBufLocked = 1;
    Locked = 1;
  }
  OS_INT_EnableConditional();
  return Locked;
}

/*********************************************************************
*
*       _UnlockTxBuf
*
* Function description
*   Releases the exclusive access to sending buffer.
*/
static void _UnlockTxBuf(void) {
  _TxBufLocked = 0;
}

/*********************************************************************
*
*       _Receive
*
*  Function description
*    Performs Command / data read from host
*/
static void _Receive(void) {
  int i;
  U8 *pBuf;

  if (RX_CNT > 0) {    /* Data received ? */
    if (_pfOnRx != NULL) {
      pBuf = _pRxBuf + (RX_BUF_SIZE - RX_CNT);
      for (i = 0; i < RX_CNT; ++i) {
        _pfOnRx(*pBuf);
        ++pBuf;
      }
    }
    RX_CNT = 0;
  }
}

/*********************************************************************
*
*       _FillTxBuf
*
*  Function description
*    Stores bytes in the sending buffer.
*/
static void _FillTxBuf(U8 Data) {
  int Cnt = 0;
  OS_INT Byte;
  U8 *pBuf;

  pBuf = _pTxBuf;
  *pBuf = Data;
  ++pBuf;
  ++Cnt;
  if (_pfGetNextChar != NULL) {
    /* Get more bytes from the communication state machine
     * until the sending buffer is full.
     */
    while (1) {
      if (Cnt >= TX_BUF_SIZE) {
        break;
      }
      Byte = _pfGetNextChar();
      if (Byte < 0) {
        break;
      }
      *pBuf = (U8)Byte;
      ++pBuf;
      ++Cnt;
    }
  }
  OS_INT_Disable();
  _RetriggerTimer();
  TX_CNT = (U8)Cnt;
  OS_INT_EnableConditional();
}

/*********************************************************************
*
*       _DropTxData
*
*  Function description
*    Empties the sending buffer of embOS.
*/
static void _DropTxData(void) {
  if (_pfGetNextChar != NULL) {
    while (_pfGetNextChar() >= 0) {
      ;
    }
  }
}

/*********************************************************************
*
*       _Send
*
*  Function description
*    Sends data back to host if embOSView is ready to receive data.
*/
static void _Send(void) {
  if (TX_CNT == 0) {        /* Can we send data? */
    _StopTimer();
    if (_TxIsPending) {
      _FillTxBuf(_TxPendingData);
      _TxIsPending = 0;
    } else {
      if (_pfOnTx != NULL) {
        if (_LockTxBuf()) {
          _pfOnTx();
          _UnlockTxBuf();
        }
      }
    }
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       JLINKMEM_Process
*
*  Function description
*    This function should be called more or less regularily to allow
*    memory reads while the application progam is running.
*    The more often it is called, the higher the transfer speed.
*/
void JLINKMEM_Process(void) {
  if (OS_IsRunning()) {         /* No communication until the embOS starts */
    if (!_IsInited) {
      _Init();
      _IsInited = 1;
    }
    if (HOST_CON) {             /* Do nothing until the host connects to us */
      //
      // Handle Timeout timer
      //
      if (_TxTimeoutTimer != 0) {
        _TxTimeoutTimer--;
        if (_TxTimeoutTimer == 0) {
          _TxTimeout = 1;
        }
      }

      if (_TxTimeout) {
        HOST_CON = 0;
        _TxTimeout = 0;
        _TxIsPending = 0;
        _DropTxData();
        RX_CNT = 0;               /* Drop all bytes form receiving buffer. */
      } else {
        _Receive();
        _Send();
      }
    }
  }
}

/*********************************************************************
*
*       JLINKMEM_SendChar
*
*  Function description
*    Send data to the host. This function is non-blocking.
*    If data can not be send it is stored in a buffer
*    and sent later, when the handler is called.
*/
void JLINKMEM_SendChar(U8 Data) {
  if (OS_IsRunning()) {         /* No communication until the embOS starts */
    if (!_IsInited) {
      _Init();
      _IsInited = 1;
    }
    if (HOST_CON) {             /* Do nothing until embOSView connects to us */
      if (TX_CNT == 0) {
        if (_LockTxBuf()) {
          _FillTxBuf(Data);
          _UnlockTxBuf();
        } else {
          _TxIsPending = 1;
          _TxPendingData = Data;
        }
      } else {
        _TxIsPending = 1;
        _TxPendingData = Data;
      }
    } else {
      //
      // Host not connected, drop characters
      //
      OS_TASK_EnterRegion();
      OS_COM_ClearTxActive();
      OS_TASK_LeaveRegion();
    }
  } else {
    //
    // embOS not started, drop characters
    //
    OS_TASK_EnterRegion();
    OS_COM_ClearTxActive();
    OS_TASK_LeaveRegion();
  }
}

/*********************************************************************
*
*       JLINKMEM_SetpfOnRx
*/
void JLINKMEM_SetpfOnRx(void (* pf)(U8 Data)) {
  _pfOnRx = pf;
}

/*********************************************************************
*
*       JLINKMEM_SetpfOnTx
*/
void JLINKMEM_SetpfOnTx(U8 (* pf)(void)) {
  _pfOnTx = pf;
}

/*********************************************************************
*
*       JLINKMEM_SetpfGetNextChar
*/
void JLINKMEM_SetpfGetNextChar(OS_INT (* pf)(void)) {
  _pfGetNextChar = pf;
}

/*************************** end of file ****************************/
