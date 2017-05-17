/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2017 SEGGER Microcontroller GmbH & Co. KG         *
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
*       OS version: 4.34.1                                           *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : OS_Error.c
Purpose : OS error handler. Not required in "R" (release) builds
          Feel free to modify this file acc. to your target system.
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "RTOS.h"

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       (OS_Error)
*
**********************************************************************

 Run-time error reaction

 When this happens, a fatal error has occurred and the kernel
 can not continue. In Linux, the equivalent would be a
 "kernel panic"

 This routine can be modified to suit your needs ...
 E.g. a red LED could light up. When using an
 emulator, you may set a breakpoint here.
 In the release builds of the library (R), this routine is not required
 (as no checks are performed).
 In the stack check builds (S/SP), only error 120 may occur.
 In the debug builds(D/DP), all of the listed errors may occur.

 The following are the current errorcodes:
 (Please refer to the documentation for more info)

*** Port 100..119 ********************************
OS_ERR_ISR_INDEX                      (100 = 0x64)
OS_ERR_ISR_VECTOR                     (101 = 0x65)
OS_ERR_ISR_PRIO                       (102 = 0x66)
OS_ERR_WRONG_STACK                    (103 = 0x67)
OS_ERR_ISR_NO_HANDLER                 (104 = 0x68)  *** No interrupt handler was defined for this interrupt
OS_ERR_TLS_INIT                       (105 = 0x69)  *** OS_TLS_Init() called multiple times from one task
OS_ERR_MB_BUFFER_SIZE                 (106 = 0x6A)  *** For 16bit CPUs, the maximum buffer size for a mailbox (64KB) exceeded

*** OS generic ***********************************

OS_ERR_EXTEND_CONTEXT                 (116 = 0x74)  *** OS_ExtendTaskContext called multiple times from one task
OS_ERR_TIMESLICE                      (117 = 0x75)  *** A timeslice value of 0 (illegal) was set.
OS_ERR_INTERNAL                       (118 = 0x76)  *** OS_ChangeTask called without RegionCnt set (or other internal error)
OS_ERR_IDLE_RETURNS                   (119 = 0x77)  *** Idle loop should not return

*** Stack overflow ******************************
OS_ERR_STACK                          (120 = 0x78)  *** Stack overflow or invalid stack

*** Counting semaphore overflow ******************
OS_ERR_CSEMA_OVERFLOW                 (121 = 0x79)

*** Power handling module ************************
OS_ERR_POWER_OVER                     (122 = 0x7A)
OS_ERR_POWER_UNDER                    (123 = 0x7B)
OS_ERR_POWER_INDEX                    (124 = 0x7C)

*** System/interrupt stack ***********************
OS_ERR_SYS_STACK                      (125 = 0x7D)  *** embOS system stack overflow
OS_ERR_INT_STACK                      (126 = 0x7E)  *** Interrupt stack overflow

*** Invalid or non-initialized data structures ***
OS_ERR_INV_TASK                       (128 = 0x80)
OS_ERR_INV_TIMER                      (129 = 0x81)
OS_ERR_INV_MAILBOX                    (130 = 0x82)
OS_ERR_INV_CSEMA                      (132 = 0x84)
OS_ERR_INV_RSEMA                      (133 = 0x85)

*** Using GetMail1, PutMail1, GetMailCond1 or PutMailCond1
*** on a non-1 byte mailbox  *********************
OS_ERR_MAILBOX_NOT1                   (135 = 0x87)

*** Waitable objects deleted with waiting tasks or occupied by task ***
OS_ERR_MAILBOX_DELETE                 (136 = 0x88)
OS_ERR_CSEMA_DELETE                   (137 = 0x89)
OS_ERR_RSEMA_DELETE                   (138 = 0x8A)

*** Internal errors, please contact SEGGER Microcontroller ***
OS_ERR_MAILBOX_NOT_IN_LIST            (140 = 0x8C)
OS_ERR_TASKLIST_CORRUPT               (142 = 0x8E)

*** Queue errors *********************************
OS_ERR_QUEUE_INUSE                    (143 = 0x8F)
OS_ERR_QUEUE_NOT_INUSE                (144 = 0x90)
OS_ERR_QUEUE_INVALID                  (145 = 0x91)
OS_ERR_QUEUE_DELETE                   (146 = 0x92)

*** Mailbox errors *******************************
OS_ERR_MB_INUSE                       (147 = 0x93)
OS_ERR_MB_NOT_INUSE                   (148 = 0x94)

*** Not matching routine calls or macro usage ****
OS_ERR_UNUSE_BEFORE_USE               (150 = 0x96)
OS_ERR_LEAVEREGION_BEFORE_ENTERREGION (151 = 0x97)
OS_ERR_LEAVEINT                       (152 = 0x98)
OS_ERR_DICNT                          (153 = 0x99)
OS_ERR_INTERRUPT_DISABLED             (154 = 0x9A)
OS_ERR_TASK_ENDS_WITHOUT_TERMINATE    (155 = 0x9B)
OS_ERR_RESOURCE_OWNER                 (156 = 0x9C)
OS_ERR_REGIONCNT                      (157 = 0x9D)

*** Not a legal system call during interrupt *****
OS_ERR_ILLEGAL_IN_ISR                 (160 = 0xA0)

*** Not a legal system call during timer *********
OS_ERR_ILLEGAL_IN_TIMER               (161 = 0xA1)

*** Not a legal system call outside interrupt ****
OS_ERR_ILLEGAL_OUT_ISR                (162 = 0xA2)

*** Illegal states or function calls *************
OS_ERR_NOT_IN_ISR                     (163 = 0xA3)  *** OS_EnterInterrupt() has been called, but CPU is not in ISR state
OS_ERR_IN_ISR                         (164 = 0xA4)  *** OS_EnterInterrupt() has not been called, but CPU is in ISR state

OS_ERR_INIT_NOT_CALLED                (165 = 0xA5)  *** OS_InitKern() was not called

OS_ERR_CPU_STATE_ISR_ILLEGAL          (166 = 0xA6)  *** OS-function called from ISR with high priority
OS_ERR_CPU_STATE_ILLEGAL              (167 = 0xA7)  *** CPU runs in illegal mode
OS_ERR_CPU_STATE_UNKNOWN              (168 = 0xA8)  *** CPU runs in unknown mode or mode could not be read

*** Double used data structures ******************
OS_ERR_2USE_TASK                      (170 = 0xAA)
OS_ERR_2USE_TIMER                     (171 = 0xAB)
OS_ERR_2USE_MAILBOX                   (172 = 0xAC)
OS_ERR_2USE_BSEMA                     (173 = 0xAD)
OS_ERR_2USE_CSEMA                     (174 = 0xAE)
OS_ERR_2USE_RSEMA                     (175 = 0xAF)
OS_ERR_2USE_MEMF                      (176 = 0xB0)

*** Communication errors (OSCOM) *****************
OS_ERR_NESTED_RX_INT                  (180 = 0xB4)

*** Spinlock *************************************
OS_ERR_SPINLOCK_INV_CORE              (185 = 0xB9)

*** Fixed block memory pool  *********************
OS_ERR_MEMF_INV                       (190 = 0xBE)
OS_ERR_MEMF_INV_PTR                   (191 = 0xBF)
OS_ERR_MEMF_PTR_FREE                  (192 = 0xC0)
OS_ERR_MEMF_RELEASE                   (193 = 0xC1)
OS_ERR_MEMF_POOLADDR                  (194 = 0xC2)
OS_ERR_MEMF_BLOCKSIZE                 (195 = 0xC3)

*** Task suspend / resume errors *****************
OS_ERR_SUSPEND_TOO_OFTEN              (200 = 0xC8)
OS_ERR_RESUME_BEFORE_SUSPEND          (201 = 0xC9)

*** Other task related errors ********************
OS_ERR_TASK_PRIORITY                  (202 = 0xCA)
OS_ERR_TASK_PRIORITY_INVALID          (203 = 0xCB)

*** Event object *********************************
OS_ERR_EVENT_INVALID                  (210 = 0xD2)
OS_ERR_EVENT_DELETE                   (212 = 0xD4)

*** Waitlist (checked build) *********************
OS_ERR_WAITLIST_RING                  (220 = 0xDC)
OS_ERR_WAITLIST_PREV                  (221 = 0xDD)
OS_ERR_WAITLIST_NEXT                  (222 = 0xDE)

*** Tick Hook ************************************
OS_ERR_TICKHOOK_INVALID               (223 = 0xDF)
OS_ERR_TICKHOOK_FUNC_INVALID          (224 = 0xE0)

*** Other potential problems discovered in checked build
OS_ERR_NOT_IN_REGION                  (225 = 0xE1)

*** Cache related ********************************
OS_ERR_NON_ALIGNED_INVALIDATE         (230 = 0xE6)  *** Cache invalidation needs to be cache line aligned

*** System timer config related ******************
OS_ERR_NON_TIMERCYCLES_FUNC           (235 = 0xEB)
OS_ERR_NON_TIMERINTPENDING_FUNC       (236 = 0xEC)

*** MPU related **********************************
OS_ERR_MPU_NOT_PRESENT                (240 = 0xF0)
OS_ERR_MPU_INVALID_REGION             (241 = 0xF1)
OS_ERR_MPU_INVALID_SIZE               (242 = 0xF2)
OS_ERR_MPU_INVALID_PERMISSION         (243 = 0xF3)
OS_ERR_MPU_INVALID_ALIGNMENT          (244 = 0xF4)
OS_ERR_MPU_INVALID_OBJECT             (245 = 0xF5)

*** OS version mismatch between library and RTOS.h
OS_ERR_VERSION_MISMATCH               (253 = 0xFD)

*** embOS special build, or internal *************
OS_ERR_TRIAL_LIMIT                    (254 = 0xFE)

*/

void OS_Error(int ErrCode) {
  OS_EnterRegion();      /* Avoid further task switches            */
  OS_DICnt = 0u;         /* Allow interrupts so we can communicate */
  OS_EI();
  OS_Status = (OS_U8) ErrCode;
  while (OS_Status) {
    /* Endless loop may be left by setting OS_Status to 0. */
  }
}

/****** End Of File *************************************************/
