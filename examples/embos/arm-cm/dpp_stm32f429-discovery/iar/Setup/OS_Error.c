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

-------------------------- END-OF-HEADER -----------------------------
Purpose : embOS error handler.
          Feel free to modify this file according to your needs.
*/

#include "RTOS.h"

/*********************************************************************
*
*       Error Codes
*
**********************************************************************

  OS_OK                                  =   (0u),  // No error, everything ok.
// User 1..99  ***********************************

// Port 100..109 *********************************
  OS_ERR_ISR_INDEX                       = (100u),  // Index value out of bounds during interrupt controller initialization or interrupt installation.
  OS_ERR_ISR_VECTOR                      = (101u),  // Default interrupt handler called, but interrupt vector not initialized.
  OS_ERR_ISR_PRIO                        = (102u),  // Wrong interrupt priority.
  OS_ERR_WRONG_STACK                     = (103u),  // Wrong stack used before main().
  OS_ERR_ISR_NO_HANDLER                  = (104u),  // No interrupt handler was defined for this interrupt.
  OS_ERR_TLS_INIT                        = (105u),  // OS_TLS_Init() called multiple times from one task.
  OS_ERR_MB_BUFFER_SIZE                  = (106u),  // For 16bit CPUs, the maximum buffer size for a mailbox (64KB) exceeded.

// OS generic ************************************
  OS_ERR_EXTEND_CONTEXT                  = (116u),  // OS_ExtendTaskContext() called multiple times from one task.
  OS_ERR_INTERNAL                        = (118u),  // OS_ChangeTask() called without Region Counter set (or other internal error).
  OS_ERR_IDLE_RETURNS                    = (119u),  // OS_Idle() must not return.
  OS_ERR_TASK_STACK                      = (120u),  // Task stack overflow or invalid task stack.

// Semaphore value overflow
  OS_ERR_SEMAPHORE_OVERFLOW              = (121u),  // Semaphore value overflow.

// Peripheral Power management module
  OS_ERR_POWER_OVER                      = (122u),  // Counter overflows when calling OS_POWER_UsageInc().
  OS_ERR_POWER_UNDER                     = (123u),  // Counter underflows when calling OS_POWER_UsageDec().
  OS_ERR_POWER_INDEX                     = (124u),  // Index to high, exceeds (OS_POWER_NUM_COUNTERS - 1).

// System/interrupt stack
  OS_ERR_SYS_STACK                       = (125u),  // System stack overflow.
  OS_ERR_INT_STACK                       = (126u),  // Interrupt stack overflow.

// invalid or non-initialized data structures
  OS_ERR_INV_TASK                        = (128u),  // Task control block invalid, not initialized or overwritten.
  OS_ERR_INV_TIMER                       = (129u),  // Timer control block invalid, not initialized or overwritten.
  OS_ERR_INV_MAILBOX                     = (130u),  // Mailbox control block invalid, not initialized or overwritten.
  OS_ERR_INV_SEMAPHORE                   = (132u),  // Control block for semaphore invalid, not initialized or overwritten.
  OS_ERR_INV_MUTEX                       = (133u),  // Control block for mutex invalid, not initialized or overwritten.

// Using GetMail1 or PutMail1 or GetMailCond1 or PutMailCond1 on
// a non-1 byte mailbox
  OS_ERR_MAILBOX_NOT1                    = (135u),  // One of the following 1-byte mailbox functions has been used on a multibyte mailbox: OS_MAILBOX_Get1(), OS_MAILBOX_GetBlocked1(), OS_MAILBOX_GetTimed1(), OS_MAILBOX_Put1(), OS_MAILBOX_PutBlocked1(), OS_MAILBOX_PutFront1(), OS_MAILBOX_PutFrontBlocked1() or OS_MAILBOX_PutTimed1().

// Waitable objects deleted with waiting tasks or occupied by task
  OS_ERR_MAILBOX_DELETE                  = (136u),  // OS_MAILBOX_Delete() was called on a mailbox with waiting tasks.
  OS_ERR_SEMAPHORE_DELETE                = (137u),  // OS_SEMAPHORE_Delete() was called on a semaphore with waiting tasks.
  OS_ERR_MUTEX_DELETE                    = (138u),  // OS_MUTEX_Delete() was called on a mutex which is claimed by a task.

// internal errors, please contact SEGGER Microcontroller Systems
  OS_ERR_MAILBOX_NOT_IN_LIST             = (140u),  // The mailbox is not in the list of mailboxes as expected. Possible reasons may be that one mailbox data structure was overwritten.
  OS_ERR_TASKLIST_CORRUPT                = (142u),  // The OS internal task list is destroyed.

// Queue errors
  OS_ERR_QUEUE_INUSE                     = (143u),  // Queue in use.
  OS_ERR_QUEUE_NOT_INUSE                 = (144u),  // Queue not in use.
  OS_ERR_QUEUE_INVALID                   = (145u),  // Queue invalid.
  OS_ERR_QUEUE_DELETE                    = (146u),  // A queue was deleted by a call of OS_QUEUE_Delete()  while tasks are waiting at the queue.

// Mailbox errors
  OS_ERR_MB_INUSE                        = (147u),  // Mailbox in use.
  OS_ERR_MB_NOT_INUSE                    = (148u),  // Mailbox not in use.

// Message size
  OS_ERR_MESSAGE_SIZE_ZERO               = (149u),  // Attempt to store a message with size of zero.

// Not matching routine calls or macro usage
  OS_ERR_UNUSE_BEFORE_USE                = (150u),  // OS_MUTEX_Unlock() has been called on a mutex that hasn't been locked before.
  OS_ERR_LEAVEREGION_BEFORE_ENTERREGION  = (151u),  // OS_TASK_LeaveRegion() has been called before OS_TASK_EnterRegion().
  OS_ERR_LEAVEINT                        = (152u),  // Error in OS_INT_Leave().
  OS_ERR_DICNT                           = (153u),  // The interrupt disable counter ( OS_Global.Counters.Cnt.DI ) is out of range (0-15).
  OS_ERR_INTERRUPT_DISABLED              = (154u),  // OS_TASK_Delay() or OS_TASK_DelayUntil() called from inside a critical region with interrupts disabled.
  OS_ERR_TASK_ENDS_WITHOUT_TERMINATE     = (155u),  // Task routine returns without 0S_TASK_Terminate().
  OS_ERR_MUTEX_OWNER                     = (156u),  // OS_MUTEX_Unlock() has been called from a task which does not own the mutex.
  OS_ERR_REGIONCNT                       = (157u),  // The Region counter overflows (>255).
  OS_ERR_DELAYUS_INTERRUPT_DISABLED      = (158u),  // OS_TASK_Delayus() called with interrupts disabled.
  OS_ERR_MUTEX_OVERFLOW                  = (159u),  // OS_MUTEX_Lock(), OS_MUTEX_LockBlocked() or OS_MUTEX_LockTimed() has been called to often from the same task.

  OS_ERR_ILLEGAL_IN_ISR                  = (160u),  // Illegal function call in an interrupt service routine: A routine that must not be called from within an ISR has been called from within an ISR.
  OS_ERR_ILLEGAL_IN_TIMER                = (161u),  // Illegal function call in a software timer: A routine that must not be called from within a software timer has been called from within a timer.
  OS_ERR_ILLEGAL_OUT_ISR                 = (162u),  // Not a legal API outside interrupt.
  OS_ERR_NOT_IN_ISR                      = (163u),  // OS_INT_Enter() has been called, but CPU is not in ISR state.
  OS_ERR_IN_ISR                          = (164u),  // OS_INT_Enter() has not been called, but CPU is in ISR state.

  OS_ERR_INIT_NOT_CALLED                 = (165u),  // OS_Init() was not called.

  OS_ERR_CPU_STATE_ISR_ILLEGAL           = (166u),  // embOS API called from ISR with high priority.
  OS_ERR_CPU_STATE_ILLEGAL               = (167u),  // CPU runs in illegal mode.
  OS_ERR_CPU_STATE_UNKNOWN               = (168u),  // CPU runs in unknown mode or mode could not be read.

  OS_ERR_TICKLESS_WITH_FRACTIONAL_TICK   = (169u),  // OS_TICKLESS_AdjustTime() was called despite OS_TICK_Config() has been called before.

// Double used data structures
  OS_ERR_2USE_TASK                       = (170u),  // Task control block has been initialized by calling a create function twice.
  OS_ERR_2USE_TIMER                      = (171u),  // Timer control block has been initialized by calling a create function twice.
  OS_ERR_2USE_MAILBOX                    = (172u),  // Mailbox control block has been initialized by calling a create function twice.
  OS_ERR_2USE_SEMAPHORE                  = (174u),  // Semaphore has been initialized by calling a create function twice.
  OS_ERR_2USE_MUTEX                      = (175u),  // Mutex has been initialized by  calling a create function twice.
  OS_ERR_2USE_MEMF                       = (176u),  // Fixed size memory pool has been initialized by calling a create function twice.
  OS_ERR_2USE_QUEUE                      = (177u),  // Queue has been initialized by calling a create function twice.
  OS_ERR_2USE_EVENT                      = (178u),  // Event object has been initialized by calling a create function twice.
  OS_ERR_2USE_WATCHDOG                   = (179u),  // Watchdog has been initialized by calling a create function twice.

// Communication errors
  OS_ERR_NESTED_RX_INT                   = (180u),  // OS_Rx interrupt handler for embOSView is nested. Disable nestable interrupts.

// Spinlock
  OS_ERR_SPINLOCK_INV_CORE               = (185u),  // Invalid core ID specified for accessing a OS_SPINLOCK_SW struct.

// Fixed block memory pool
  OS_ERR_MEMF_INV                        = (190u),  // Fixed size memory block control structure not created before use.
  OS_ERR_MEMF_INV_PTR                    = (191u),  // Pointer to memory block does not belong to memory pool on Release.
  OS_ERR_MEMF_PTR_FREE                   = (192u),  // Pointer to memory block is already free when calling OS_MEMPOOL_Release(). Possibly, same pointer was released twice.
  OS_ERR_MEMF_RELEASE                    = (193u),  // OS_MEMPOOL_Release() was called for a memory pool, that had no memory block allocated (all available blocks were already free before).
  OS_ERR_MEMF_POOLADDR                   = (194u),  // OS_MEMPOOL_Create() was called with a memory pool base address which is not located at a word aligned base address.
  OS_ERR_MEMF_BLOCKSIZE                  = (195u),  // OS_MEMPOOL_Create() was called with a data block size which is not a multiple of processors word size.

// Task suspend / resume errors
  OS_ERR_SUSPEND_TOO_OFTEN               = (200u),  // Nested call of  OS_TASK_Suspend() exceeded OS_MAX_SUSPEND_CNT.
  OS_ERR_RESUME_BEFORE_SUSPEND           = (201u),  // OS_TASK_Resume() called on a task that was not suspended.

// Other task related errors
  OS_ERR_TASK_PRIORITY                   = (202u),  // OS_TASK_Create()  was called with a task priority which is already assigned to another task. This error can only occur when embOS was compiled without round-robin support.
  OS_ERR_TASK_PRIORITY_INVALID           = (203u),  // The value 0 was used as task priority.

// Timer related errors
  OS_ERR_TIMER_PERIOD_INVALID            = (205u),  // The value 0 was used as timer period.

// Event object
  OS_ERR_EVENT_INVALID                   = (210u),  // An OS_EVENT object was used before it was created.
  OS_ERR_EVENT_DELETE                    = (212u),  // An OS_EVENT object was deleted with waiting tasks.

// Waitlist (checked build)
  OS_ERR_WAITLIST_RING                   = (220u),  // This error should not occur. Please contact the support.
  OS_ERR_WAITLIST_PREV                   = (221u),  // This error should not occur. Please contact the support.
  OS_ERR_WAITLIST_NEXT                   = (222u),  // This error should not occur. Please contact the support.

// Tick Hook
  OS_ERR_TICKHOOK_INVALID                = (223u),  // Invalid tick hook.
  OS_ERR_TICKHOOK_FUNC_INVALID           = (224u),  // Invalid tick hook function.

// Other potential problems discovered in checked build
  OS_ERR_NOT_IN_REGION                   = (225u),  // A function was called without declaring the necessary critical region.

// API context check
  OS_ERR_ILLEGAL_IN_MAIN                 = (226u),  // Not a legal API call from main().
  OS_ERR_ILLEGAL_IN_TASK                 = (227u),  // Not a legal API after OS_Start().
  OS_ERR_ILLEGAL_AFTER_OSSTART           = (228u),  // Not a legal API after OS_Start().

// Cache related
  OS_ERR_NON_ALIGNED_INVALIDATE          = (230u),  // Cache invalidation needs to be cache line aligned.

// Available hardware
  OS_ERR_HW_NOT_AVAILABLE                = (234u),  // Hardware unit is not implemented or enabled.

// System timer config related
  OS_ERR_NON_TIMERCYCLES_FUNC            = (235u),  // OS_TIME_ConfigSysTimer() has not been called. Callback function for timer counter value has not been set.
  OS_ERR_NON_TIMERINTPENDING_FUNC        = (236u),  // OS_TIME_ConfigSysTimer() has not been called. Callback function for timer interrupt pending flag has not been set.

// embOS MPU related
  OS_ERR_MPU_NOT_PRESENT                 = (240u),  // MPU unit not present in the device.
  OS_ERR_MPU_INVALID_REGION              = (241u),  // Invalid MPU region index number.
  OS_ERR_MPU_INVALID_SIZE                = (242u),  // Invalid MPU region size.
  OS_ERR_MPU_INVALID_PERMISSION          = (243u),  // Invalid MPU region permission.
  OS_ERR_MPU_INVALID_ALIGNMENT           = (244u),  // Invalid MPU region alignment.
  OS_ERR_MPU_INVALID_OBJECT              = (245u),  // OS object is directly accessible from the task which is not allowed.

// Buffer to small to keep a backup copy of the CSTACK
  OS_ERR_CONFIG_OSSTOP                   = (250u),  // OS_Stop() is called without using OS_ConfigStop() before.
  OS_ERR_OSSTOP_BUFFER                   = (251u),  // Buffer is too small to hold a copy of the main() stack.

// OS version mismatch between library and RTOS.h
  OS_ERR_VERSION_MISMATCH                = (253u)   // OS library and RTOS have different version numbers. Please ensure both are from the same embOS shipment.

*/

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       OS_Error()
*
*  Function description
*    Run-time error reaction
*
*    When this happens, an application error occurred and was detected by
*    embOS.
*
*    This routine can be modified to suit your needs, e.g. a red LED could
*    light up. When using an emulator, you may set a breakpoint here.
*    In the release builds of the library (R, XR), this routine is not
*    required (as no checks are performed).
*    In the stack check builds (S/SP), only error 120 may occur.
*    In the debug builds(D/DP,DT), all of the listed errors may occur.
*
*  Parameters
*    ErrCode: embOS error code
*/
void OS_Error(OS_STATUS ErrCode) {
  OS_TASK_EnterRegion();           // Avoid further task switches
  OS_Global.Counters.Cnt.DI = 0u;  // Allow interrupts so we can communicate with embOSView
  OS_INT_Enable();
  OS_Status = ErrCode;
  while (OS_Status) {
    // Endless loop may be left by setting OS_Status to 0
  }
}

/*************************** End of file ****************************/
