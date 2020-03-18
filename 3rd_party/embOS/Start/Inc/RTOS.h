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
File    : RTOS.h
Purpose : Include file for the OS,
          to be included in every C-module accessing OS-routines
Literature:
  [1]  ARM Processor Cortex-M7 (AT610) and Cortex-M7 with FPU (AT611) Software Developers Errata Notice (\\fileserver\Techinfo\Company\ARM\CPU\Cortex-M7 (Pelican)\cortex_m7_software_developers_errata_notice_r0_v4.pdf)

NOTE    : NONE OF THE DEFINITIONS IN THIS FILE MAY BE MODIFIED
          as long as embOS libraries are used.
          Any modification, direct or indirect, may cause malfunction.
          Modifications can only be done when the libraries are
          recompiled or the embOS sources are used in the project.

--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef RTOS_H_INCLUDED        /* Avoid multiple inclusion          */
#define RTOS_H_INCLUDED

/*********************************************************************
*
*     Includes
*/
#include <string.h>         // Required for memset() etc.
#include <intrinsics.h>     // Required for __disable_interrupt() etc.

#pragma language=extended

/*********************************************************************
*
*     Chip specifics for Cortex M cores and IAR compiler
*
**********************************************************************
*/

/*********************************************************************
*
*     Configuration
*/
#define OS_PORT_REVISION          (1u)                       // Port specific revision
#define OS_PTR_OP_IS_ATOMIC       (1)
#define OS_U32_OP_IS_ATOMIC       (1)
#define OS_TIME_GetTicks()        (OS_Global.Time)
#define OS_TIME_GetTicks32()      (OS_Global.Time)
#define OS_I64                    long long
#define OS_SUPPORT_SYSSTACK_INFO  (1)                        // Option to supply more info to embOSView
#define OS_SUPPORT_INTSTACK_INFO  (0)                        // Not used
#define OS_SP_ADJUST              (0u)
#define OS_INT_EnterIntStack()                               // No special stack switching required
#define OS_INT_LeaveIntStack()                               // No special stack switching required
#define OS_DEINIT()               OS_ARMv7M_DeInit()
#if (defined __ARM6M__) && (__CORE__ == __ARM6M__)
  #define OS_MEMCPY               OS_memcpy                  // Overwrite IAR Library memcpy() because it might not work correctly
#endif

//
// "cpsie i" followed by "cpsid i" won't give interrupts a chance to be executed on Cortex-M23
//
#if (defined(__ARM8M_BASELINE__) && (__CORE__ == __ARM8M_BASELINE__))
  #define OS_DUMMY_STATEMENT()  __DSB()
#endif

/*****  Core specific configuration *********************************/

#define OS_SWITCH_FROM_INT_MODIFIES_STACK   (0)
#define OS_INTERRUPTS_ARE_NESTABLE_ON_ENTRY (1)
#define OS_SCHEDULER_ACTIVATED_BY_EXCEPTION (1)
#define OS_SUPPORT_INT_PRIORITY             (0)              // Variable interrupt priorities for OS not supported by now

#define OS_CODE_SECTION_ATTRIBUTE

//
// Erratum 837070, ARM-EPM-064408 v4.0 [1]: With Cortex-M7 r0p1, increasing priority
// using a write to BASEPRI does not take effect immediately. For a workaround, interrupts
// should globally be disabled while accessing BASEPRI. Since other CPUs and revisions are
// not affected, this define may be used to switch on this workaround.
// When modifiying this define, the respective define in RTOS_CM3.s / RTOS_CM4F.s must be changed as well.
//
#ifndef   USE_ERRATUM_837070
  #define USE_ERRATUM_837070      (0)                        // 0: do not use workaround (default), 1: use workaround
#endif

//
// With the above workaround, zero-latency ISRs will be enabled after each call to
// OS_INT_Disable() or OS_INT_Restore(). If this is not desired, this define may be used
// to preserve and restore the state of PRIMASK with these calls.
// When modifiying this define, the respective define in RTOS_CM3.s / RTOS_CM4F.s must be changed as well.
//
#if (USE_ERRATUM_837070 == 1)
  #ifndef   OS_PRESERVE_PRIMASK
    #define OS_PRESERVE_PRIMASK     (0)                      // 0: PRIMASK is not preserved (default), 1: PRIMASK is preserved
  #endif
#endif

/*****  End of configuration settings *******************************/
//
// The NOPs (__no_operation()) should flush the pipeline to ensure that
// interrupts are disabled before the next instruction is executed.
//
#if (defined (__ARM6M__)          && (__CORE__ == __ARM6M__))          \
 || (defined (__ARM8M_BASELINE__) && (__CORE__ == __ARM8M_BASELINE__))
  //
  // Cortex-M0/M0+/M1/M23
  //
  #define OS_INT_Disable() __disable_interrupt()
  #define OS_INT_Enable()  __enable_interrupt()
#elif ((defined (__ARM7M__)           && (__CORE__ == __ARM7M__))            \
   ||  (defined (__ARM7EM__)          && (__CORE__ == __ARM7EM__))           \
   ||  (defined (__ARM8M_MAINLINE__)  && (__CORE__ == __ARM8M_MAINLINE__))   \
   ||  (defined (__ARM8EM_MAINLINE__) && (__CORE__ == __ARM8EM_MAINLINE__))) \
   && (USE_ERRATUM_837070 == 0)
  //
  // Cortex-M3/M4/M4F/M7/M7F/M33 w/o workaround
  //
  #define OS_INT_Disable() {          \
    __set_BASEPRI(OS_IPL_DI_DEFAULT); \
    __no_operation();                 \
    __no_operation();                 \
  }
  #define OS_INT_Enable() __set_BASEPRI(OS_IPL_EI_DEFAULT)
#elif ((defined (__ARM7M__)           && (__CORE__ == __ARM7M__))            \
   ||  (defined (__ARM7EM__)          && (__CORE__ == __ARM7EM__))           \
   ||  (defined (__ARM8M_MAINLINE__)  && (__CORE__ == __ARM8M_MAINLINE__))   \
   ||  (defined (__ARM8EM_MAINLINE__) && (__CORE__ == __ARM8EM_MAINLINE__))) \
   && (USE_ERRATUM_837070 == 1)
  //
  // Cortex-M3/M4/M4F/M7/M7F/M33 with workaround
  //
  #if (OS_PRESERVE_PRIMASK == 1)
    #define OS_INT_Disable() {            \
      OS_U32 primask;                     \
      primask = (OS_U32) __get_PRIMASK(); \
      __disable_interrupt();              \
      __set_BASEPRI(OS_IPL_DI_DEFAULT);   \
      __set_PRIMASK(primask);             \
      __no_operation();                   \
      __no_operation();                   \
    }
  #else
    #define OS_INT_Disable() {            \
      __disable_interrupt();              \
      __set_BASEPRI(OS_IPL_DI_DEFAULT);   \
      __enable_interrupt();               \
      __no_operation();                   \
      __no_operation();                   \
    }
  #endif
  #define OS_INT_Enable() __set_BASEPRI(OS_IPL_EI_DEFAULT)
#else
  #error "Please check that __ARM6M__, __ARM7M__, __ARM7EM__, __ARM8M_BASELINE__, __ARM8M_MAINLINE__ or __ARM8EM_MAINLINE__ is defined!"
#endif

#define OS_IPL_EI_DEFAULT 0     // Modification requires
#define OS_IPL_DI_DEFAULT 128   // modification in RTOS*.s assembly files !

#if ((defined (__ARM7EM__)          && (__CORE__ == __ARM7EM__))           \
 ||  (defined (__ARM8M_MAINLINE__)  && (__CORE__ == __ARM8M_MAINLINE__))   \
 ||  (defined (__ARM8EM_MAINLINE__) && (__CORE__ == __ARM8EM_MAINLINE__))) \
 && defined (__ARMVFP__)
  #define OS_CPU_HAS_VFP  (1)
#else
  #define OS_CPU_HAS_VFP  (0)
#endif

#define OS_U32 unsigned long

/*********************************************************************
*
*       OS_REGS
*
*  Register structure on stack. Not required by application, just a debugging help.
*/
typedef struct {
  OS_U32 Counters;
  OS_U32 OS_REG_R4;
  OS_U32 OS_REG_R5;
  OS_U32 OS_REG_R6;
  OS_U32 OS_REG_R7;
  OS_U32 OS_REG_R8;
  OS_U32 OS_REG_R9;
  OS_U32 OS_REG_R10;
  OS_U32 OS_REG_R11;
  OS_U32 OS_REG_LR;
  OS_U32 OS_REG_R0;
  OS_U32 OS_REG_R1;
  OS_U32 OS_REG_R2;
  OS_U32 OS_REG_R3;
  OS_U32 OS_REG_R12;
  OS_U32 OS_REG_R14;
  OS_U32 OS_REG_PC;
  OS_U32 OS_REG_XPSR;
} OS_REGS_BASE;

typedef struct {
  OS_REGS_BASE Base;
} OS_REGS;

//
// Main Context used for OS_Stop()
//
#if (OS_CPU_HAS_VFP == 0)
typedef struct {
  OS_U32 OS_REG_CONTROL;
  OS_U32 OS_REG_SP;
  OS_U32 OS_REG_LR;
  OS_U32 OS_REG_R4;
  OS_U32 OS_REG_R5;
  OS_U32 OS_REG_R6;
  OS_U32 OS_REG_R7;
  OS_U32 OS_REG_R8;
  OS_U32 OS_REG_R9;
  OS_U32 OS_REG_R10;
  OS_U32 OS_REG_R11;
  void*  pBuffer;
  OS_U32 BufferSize;
} OS_MAIN_CONTEXT;
#else
typedef struct {
  OS_U32 OS_REG_CONTROL;
  OS_U32 OS_REG_SP;
  OS_U32 OS_REG_LR;
  OS_U32 OS_REG_R4;
  OS_U32 OS_REG_R5;
  OS_U32 OS_REG_R6;
  OS_U32 OS_REG_R7;
  OS_U32 OS_REG_R8;
  OS_U32 OS_REG_R9;
  OS_U32 OS_REG_R10;
  OS_U32 OS_REG_R11;
  OS_U32 OS_REG_S16;
  OS_U32 OS_REG_S17;
  OS_U32 OS_REG_S18;
  OS_U32 OS_REG_S19;
  OS_U32 OS_REG_S20;
  OS_U32 OS_REG_S21;
  OS_U32 OS_REG_S22;
  OS_U32 OS_REG_S23;
  OS_U32 OS_REG_S24;
  OS_U32 OS_REG_S25;
  OS_U32 OS_REG_S26;
  OS_U32 OS_REG_S27;
  OS_U32 OS_REG_S28;
  OS_U32 OS_REG_S29;
  OS_U32 OS_REG_S30;
  OS_U32 OS_REG_S31;
  void*  pBuffer;
  OS_U32 BufferSize;
} OS_MAIN_CONTEXT;
#endif

/*********************************************************************
*
*       Port specific (non OS generic) functions
*/
#ifdef __cplusplus
  extern "C" {
#endif

extern const OS_U32 OS_JLINKMEM_BufferSize;
typedef void( *intfunc )( void );
typedef union { intfunc __fun; void * __ptr; } intvec_elem;
extern const intvec_elem __vector_table[];
#define __Vectors __vector_table

#define OS_USEPARA(para)         (void) (para)

/*********************************************************************
*
*       Core specific implementation
*/
typedef void      OS_ISR_HANDLER(void);
void              OS_ARM_ISRInit           (OS_U32 IsVectorTableInRAM, OS_U32 NumInterrupts, OS_ISR_HANDLER* VectorTableBaseAddr[], OS_ISR_HANDLER* RAMVectorTableBaseAddr[]) OS_CODE_SECTION_ATTRIBUTE;
OS_ISR_HANDLER*   OS_ARM_InstallISRHandler (int ISRIndex, OS_ISR_HANDLER* pISRHandler)                                                                                        OS_CODE_SECTION_ATTRIBUTE;
void              OS_ARM_EnableISR         (int ISRIndex)                                                                                                                     OS_CODE_SECTION_ATTRIBUTE;
void              OS_ARM_DisableISR        (int ISRIndex)                                                                                                                     OS_CODE_SECTION_ATTRIBUTE;
int               OS_ARM_ISRSetPrio        (int ISRIndex, int Prio)                                                                                                           OS_CODE_SECTION_ATTRIBUTE;

#if defined (__ARM6M__) && (__CORE__ == __ARM6M__)        // Cortex-M0/M0+/M1
void              OS_memcpy                (void * pDest, const void * pSrc, int NumBytes)                                                                                    OS_CODE_SECTION_ATTRIBUTE;
#endif

#define OS_IDLE()                          // Overrides call of OS_Idle()
#define OS_EI_ON_LEAVE()  OS_INT_Enable()  // Required for CPUs which do not restore DI-flag by RETI.
#define OS_ENABLE_INTS_SAVE_IPL()          // Not required with Cortex-M, avoid call of OS_INT_Enable()

/*****  Declare functions used in RTOSInit.c *****/
void SysTick_Handler (void) OS_CODE_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Thread locking, thread safe system libraries
*/
#if ((__VER__ >= 441) && (__VER__ < 6000000))
  void OS_InitSysLocks         (void)          OS_CODE_SECTION_ATTRIBUTE;
  #define OS_INIT_SYS_LOCKS()      OS_InitSysLocks()
#elif ((__VER__ > 6000000) && (__VER__ < 8000000))
  #define OS_INIT_SYS_LOCKS()
  /* embOS specific system locking functions called from the IAR hook functions */
  void OS__iar_system_Mtxinit  (__iar_Rmtx *m) OS_CODE_SECTION_ATTRIBUTE;
  void OS__iar_system_Mtxdst   (__iar_Rmtx *m) OS_CODE_SECTION_ATTRIBUTE;
  void OS__iar_system_Mtxlock  (__iar_Rmtx *m) OS_CODE_SECTION_ATTRIBUTE;
  void OS__iar_system_Mtxunlock(__iar_Rmtx *m) OS_CODE_SECTION_ATTRIBUTE;
#elif (__VER__ >= 8000000)
  #include <DLib_Threads.h>
  #define OS_INIT_SYS_LOCKS()  __iar_Initlocks()
  /* embOS specific system locking functions called from the IAR hook functions */
  void OS__iar_system_Mtxinit  (__iar_Rmtx *m) OS_CODE_SECTION_ATTRIBUTE;
  void OS__iar_system_Mtxdst   (__iar_Rmtx *m) OS_CODE_SECTION_ATTRIBUTE;
  void OS__iar_system_Mtxlock  (__iar_Rmtx *m) OS_CODE_SECTION_ATTRIBUTE;
  void OS__iar_system_Mtxunlock(__iar_Rmtx *m) OS_CODE_SECTION_ATTRIBUTE;
#endif

/*********************************************************************
*
*       Compiler specific implementation for thread local storage
*/
#define OS_TASK_HAS_PTLS  (1)      // pTLS always in task structure, to get compatible TCBs for all versions
#define OS_SUPPORT_TLS    (1)      // We need the OS_TLS_Delete() function in case current task is terminated and occupies a reent structure

void OS_TASK_SetContextExtensionTLS(void) OS_CODE_SECTION_ATTRIBUTE;
//
// Macros for compatibilty with older embOS version
//
#define OS_ExtendTaskContext_TLS    OS_TASK_SetContextExtensionTLS
#define OS_InitReent                OS_TASK_SetContextExtensionTLS
//
// OS_ExtendTaskContext_TLS_VFP is no longer necessary because VFP is now automatically handled by embOS
//
#define OS_ExtendTaskContext_TLS_VFP  OS_TASK_SetContextExtensionTLS

#if (__VER__ >= 6000000 && __VER__ < 8000000)
  #define OS_TLS_PTR void _DLIB_TLS_MEMORY*
#endif

/*********************************************************************
*
*       Support for ARM VFP
*/
#if (OS_CPU_HAS_VFP == 1)
//
// Defines for compatibility with older embOS versions
// Not longer necessary because VFP is now automatically handled by embOS
//
#define OS_ExtendTaskContext_VFP()
#define OS_VFP_Save()
#define OS_VFP_Restore()
#endif

/*********************************************************************
*
*       Global interrupt disable/enable support
*/
#define OS_INT_DisableAll()             __disable_interrupt()
#define OS_INT_EnableAll()              __enable_interrupt()
#define OS_INT_PreserveAll(p)           (*(p) = (OS_U32) __get_PRIMASK())
#define OS_INT_RestoreAll(p)            __set_PRIMASK(*(p))
#define OS_INT_PreserveAndDisableAll(p) { \
  (*(p) = (OS_U32) __get_PRIMASK());      \
  __disable_interrupt();                  \
}

/*********************************************************************
*
*       embOS interrupt preserve/restore support
*/
#if (defined (__ARM6M__)          && (__CORE__ == __ARM6M__))          \
 || (defined (__ARM8M_BASELINE__) && (__CORE__ == __ARM8M_BASELINE__))
  //
  // Cortex-M0/M0+/M1/M23
  //
  #define OS_INT_Preserve(p)  (*(p) = (OS_U32) __get_PRIMASK())
  #define OS_INT_Restore(p)   __set_PRIMASK(*(p))
#elif ((defined (__ARM7M__)           && (__CORE__ == __ARM7M__))            \
   ||  (defined (__ARM7EM__)          && (__CORE__ == __ARM7EM__))           \
   ||  (defined (__ARM8M_MAINLINE__)  && (__CORE__ == __ARM8M_MAINLINE__))   \
   ||  (defined (__ARM8EM_MAINLINE__) && (__CORE__ == __ARM8EM_MAINLINE__))) \
   && (USE_ERRATUM_837070 == 0)
  //
  // Cortex-M3/M4/M4F/M7/M7F/M33 w/o workaround
  //
  #define OS_INT_Preserve(p)  (*(p) = (OS_U32) __get_BASEPRI())
  #define OS_INT_Restore(p)   { \
    __set_BASEPRI(*(p));        \
    __no_operation();           \
    __no_operation();           \
  }
#elif ((defined (__ARM7M__)           && (__CORE__ == __ARM7M__))            \
   ||  (defined (__ARM7EM__)          && (__CORE__ == __ARM7EM__))           \
   ||  (defined (__ARM8M_MAINLINE__)  && (__CORE__ == __ARM8M_MAINLINE__))   \
   ||  (defined (__ARM8EM_MAINLINE__) && (__CORE__ == __ARM8EM_MAINLINE__))) \
   && (USE_ERRATUM_837070 == 1)
  //
  // Cortex-M3/M4/M4F/M7/M7F/M33 with workaround
  //
  #define OS_INT_Preserve(p)   (*(p) = (OS_U32) __get_BASEPRI())
  #if (OS_PRESERVE_PRIMASK == 1)
    #define OS_INT_Restore(p)  {          \
      OS_U32 primask;                     \
      primask = (OS_U32) __get_PRIMASK(); \
      __disable_interrupt();              \
      __set_BASEPRI(*(p));                \
      __set_PRIMASK(primask);             \
      __no_operation();                   \
      __no_operation();                   \
    }
  #else
    #define OS_INT_Restore(p)  {          \
      __disable_interrupt();              \
      __set_BASEPRI(*(p));                \
      __enable_interrupt();               \
      __no_operation();                   \
      __no_operation();                   \
    }
  #endif
#endif

/*********************************************************************
*
*       DMB (Data Memory Barrier support)
*/
#define OS_DATA_MEMORY_BARRIER()  __DMB();  // IAR intrinsic to issue a full memory barrier

/*********************************************************************
*
*       SystemView
*/
#define SEGGER_SYSVIEW_DWT_IS_ENABLED()  ((*(volatile OS_U32*) (0xE0001000u)) & 0x01)

extern unsigned int SEGGER_SYSVIEW_TickCnt;

/*********************************************************************
*
*       Remap identifiers
*
**********************************************************************

Renaming OS_ChangeTask will generate an error when using sources and
assembly files are compiled with different settings.
Renaming OS_Init will generate an error when a library with wrong
data model is used in a project

*/
#if (OS_CPU_HAS_VFP)
  #define OS_ChangeTask OS_ChangeTask_VFP
  #define OS_Init       OS_Init_VFP
#else
  #define OS_ChangeTask OS_ChangeTask_STD
  #define OS_Init       OS_Init_STD
#endif

/********************************************************************/

#ifdef __cplusplus
  }
#endif

/****** End of chip / compiler specific part ************************/
/*********************************************************************
*
*       Generic portion of embOS starts here
*
**********************************************************************
This file (original name OS_Raw.h) is part of RTOS.h, the include
file for embOS.
*/

//
// embOS version
//
#define OS_VERSION_GENERIC    (50600u)

#ifndef   OS_PORT_REVISION
  #define OS_PORT_REVISION    (0u)
#endif

#define OS_VERSION            (OS_VERSION_GENERIC + (OS_PORT_REVISION * 25u))

/*********************************************************************
*
*       embOS compile time switches
*
*  These compile time switches can be modified when recompiling
*  the library or using the embOS sources in your project.
*  The switches are divided into two parts:
*  1. Switches which you may modify for e.g. enabling/disabling embOS features
*     These defines can be set as project option or in OS_Config.h.
*  2. Switches which you must not modify. These switches are used to build the embOS
*     generic sources for a specific core and compiler.
*     Usually, the values are defined in OSCHIP.h, which is then
*     merged with OS_Raw.h to form RTOS.h.
*     If the values are not defined in OSCHIP.h, the default values
*     below are used.
**********************************************************************
*/

//
//  If no embOS library mode is defined OS_Config.h is included.
//  OS_Config.h defines the library mode dependent on the Define "DEBUG"
//
#if (!defined(OS_LIBMODE_XR) && !defined(OS_LIBMODE_R)  &&   \
     !defined(OS_LIBMODE_S)  && !defined(OS_LIBMODE_SP) &&   \
     !defined(OS_LIBMODE_D)  && !defined(OS_LIBMODE_DP) &&   \
     !defined(OS_LIBMODE_DT) && !defined(OS_LIBMODE_SAFE))
  #include "OS_Config.h"
#endif

//
// Thread local storage, may be enabled in CPU specific part
//
#ifndef   OS_SUPPORT_TLS
  #define OS_SUPPORT_TLS  (0)
#endif

/*********************************************************************
*
*       Defines for library types
*
*  Depending on the used library mode some defines have default values
*
**********************************************************************
*/
#if defined(OS_LIBMODE_XR)                     // Extremely small release - without Round-robin support
  #define OS_STACKCHECK_DEFAULT                (0)
  #define OS_STACKCHECK_LIMIT_DEFAULT          (100u)  // 100%
  #define OS_DEBUG_DEFAULT                     (0)
  #define OS_PROFILE_DEFAULT                   (0)
  #define OS_SUPPORT_TICKSTEP_DEFAULT          (0)
  #define OS_TRACE_DEFAULT                     (0)
  #define OS_RR_SUPPORTED_DEFAULT              (0)
  #define OS_TRACKNAME_DEFAULT                 (0)
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT (0)
  #define OS_TASK_Create                       OS_TASK_Create_XR
  #define OS_TASK_CreateEx                     OS_TASK_CreateEx_XR
  #define OS_LIBMODE                           "XR"
  #define OS_SUPPORT_STAT_DEFAULT              (0)
  #define OS_SUPPORT_PTLS_DEFAULT              (0)
  #define OS_INIT_EXPLICITLY_DEFAULT           (0)
#elif defined(OS_LIBMODE_R)                    // Release build
  #define OS_STACKCHECK_DEFAULT                (0)
  #define OS_STACKCHECK_LIMIT_DEFAULT          (100u)  // 100%
  #define OS_DEBUG_DEFAULT                     (0)
  #define OS_PROFILE_DEFAULT                   (0)
  #define OS_SUPPORT_TICKSTEP_DEFAULT          (0)
  #define OS_TRACE_DEFAULT                     (0)
  #define OS_RR_SUPPORTED_DEFAULT              (1)
  #define OS_TRACKNAME_DEFAULT                 (1)
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT (1)
  #define OS_TASK_Create                       OS_TASK_Create_R
  #define OS_TASK_CreateEx                     OS_TASK_CreateEx_R
  #define OS_LIBMODE                           "R"
  #define OS_SUPPORT_STAT_DEFAULT              (0)
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
  #define OS_INIT_EXPLICITLY_DEFAULT           (0)
#elif defined(OS_LIBMODE_S)                    // Release build with stack check
  #define OS_STACKCHECK_DEFAULT                (1)
  #define OS_STACKCHECK_LIMIT_DEFAULT          (100u)  // 100%
  #define OS_DEBUG_DEFAULT                     (0)
  #define OS_PROFILE_DEFAULT                   (0)
  #define OS_SUPPORT_TICKSTEP_DEFAULT          (0)
  #define OS_TRACE_DEFAULT                     (0)
  #define OS_RR_SUPPORTED_DEFAULT              (1)
  #define OS_TRACKNAME_DEFAULT                 (1)
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT (1)
  #define OS_TASK_Create                       OS_TASK_Create_S
  #define OS_TASK_CreateEx                     OS_TASK_CreateEx_S
  #define OS_LIBMODE                           "S"
  #define OS_SUPPORT_STAT_DEFAULT              (0)
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
  #define OS_INIT_EXPLICITLY_DEFAULT           (0)
#elif defined(OS_LIBMODE_SP)                   // Release build with stack check and profiling
  #define OS_STACKCHECK_DEFAULT                (1)
  #define OS_STACKCHECK_LIMIT_DEFAULT          (100u)  // 100%
  #define OS_DEBUG_DEFAULT                     (0)
  #define OS_PROFILE_DEFAULT                   (1)
  #define OS_SUPPORT_TICKSTEP_DEFAULT          (1)
  #define OS_TRACE_DEFAULT                     (0)
  #define OS_RR_SUPPORTED_DEFAULT              (1)
  #define OS_TRACKNAME_DEFAULT                 (1)
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT (1)
  #define OS_TASK_Create                       OS_TASK_Create_SP
  #define OS_TASK_CreateEx                     OS_TASK_CreateEx_SP
  #define OS_LIBMODE                           "SP"
  #define OS_SUPPORT_STAT_DEFAULT              (1)
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
  #define OS_INIT_EXPLICITLY_DEFAULT           (0)
#elif defined(OS_LIBMODE_D)                    // Debug build
  #define OS_STACKCHECK_DEFAULT                (1)
  #define OS_STACKCHECK_LIMIT_DEFAULT          (100u)  // 100%
  #define OS_DEBUG_DEFAULT                     (1)
  #define OS_PROFILE_DEFAULT                   (0)
  #define OS_SUPPORT_TICKSTEP_DEFAULT          (1)
  #define OS_TRACE_DEFAULT                     (0)
  #define OS_RR_SUPPORTED_DEFAULT              (1)
  #define OS_TRACKNAME_DEFAULT                 (1)
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT (1)
  #define OS_TASK_Create                       OS_TASK_Create_D
  #define OS_TASK_CreateEx                     OS_TASK_CreateEx_D
  #define OS_LIBMODE                           "D"
  #define OS_SUPPORT_STAT_DEFAULT              (1)
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
  #define OS_INIT_EXPLICITLY_DEFAULT           (0)
#elif defined(OS_LIBMODE_DP)                   // Debug build with profiling
  #define OS_STACKCHECK_DEFAULT                (1)
  #define OS_STACKCHECK_LIMIT_DEFAULT          (100u)  // 100%
  #define OS_DEBUG_DEFAULT                     (1)
  #define OS_PROFILE_DEFAULT                   (1)
  #define OS_SUPPORT_TICKSTEP_DEFAULT          (1)
  #define OS_TRACE_DEFAULT                     (0)
  #define OS_RR_SUPPORTED_DEFAULT              (1)
  #define OS_TRACKNAME_DEFAULT                 (1)
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT (1)
  #define OS_TASK_Create                       OS_TASK_Create_DP
  #define OS_TASK_CreateEx                     OS_TASK_CreateEx_DP
  #define OS_LIBMODE                           "DP"
  #define OS_SUPPORT_STAT_DEFAULT              (1)
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
  #define OS_INIT_EXPLICITLY_DEFAULT           (0)
#elif defined(OS_LIBMODE_DT)                   // Debug build with profiling and trace
  #define OS_STACKCHECK_DEFAULT                (1)
  #define OS_STACKCHECK_LIMIT_DEFAULT          (100u)  // 100%
  #define OS_DEBUG_DEFAULT                     (1)
  #define OS_PROFILE_DEFAULT                   (1)
  #define OS_SUPPORT_TICKSTEP_DEFAULT          (1)
  #define OS_TRACE_DEFAULT                     (1)
  #define OS_RR_SUPPORTED_DEFAULT              (1)
  #define OS_TRACKNAME_DEFAULT                 (1)
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT (1)
  #define OS_TASK_Create                       OS_TASK_Create_DT
  #define OS_TASK_CreateEx                     OS_TASK_CreateEx_DT
  #define OS_LIBMODE                           "DT"
  #define OS_SUPPORT_STAT_DEFAULT              (1)
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
  #define OS_INIT_EXPLICITLY_DEFAULT           (0)
#elif defined(OS_LIBMODE_SAFE)                 // Based on OS_LIBMODE_DP with additional safety features, used with certificated embOS
  #define OS_STACKCHECK_DEFAULT                (2)    // Extended stack check with configurable limit
  #define OS_STACKCHECK_LIMIT_DEFAULT          (70u)  // OS_Error() is called when the stack is filled more than 70% of it's size
  #define OS_DEBUG_DEFAULT                     (1)
  #define OS_PROFILE_DEFAULT                   (1)
  #define OS_SUPPORT_TICKSTEP_DEFAULT          (0)
  #define OS_TRACE_DEFAULT                     (0)
  #define OS_RR_SUPPORTED_DEFAULT              (1)
  #define OS_TRACKNAME_DEFAULT                 (1)
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT (1)
  #define OS_TASK_Create                       OS_TASK_Create_SAFE
  #define OS_TASK_CreateEx                     OS_TASK_CreateEx_SAFE
  #define OS_LIBMODE                           "SAFE"
  #define OS_SUPPORT_STAT_DEFAULT              (0)
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
  #define OS_INIT_EXPLICITLY_DEFAULT           (1)
#else
  #error "Please define which embOS library mode should be used by setting OS_LIBMODE_*!"
#endif

/*********************************************************************
*
*       User configurable compile time switches
*
*  Compile time switches which can be changed by the user to
*  enable/disable embOS features.
*  You must not modify them here but add according defines to
*  OS_Config.h or to your project settings.
*
*  If defines are not yet set use defaults.
*
**********************************************************************
*/
#ifndef   OS_DEBUG
  #define OS_DEBUG                          OS_DEBUG_DEFAULT
#endif

#ifndef   OS_CHECKSTACK
  #define OS_CHECKSTACK                     OS_STACKCHECK_DEFAULT
#endif

#ifndef   OS_STACKCHECK_LIMIT
  #define OS_STACKCHECK_LIMIT               OS_STACKCHECK_LIMIT_DEFAULT
#endif

#ifndef   OS_PROFILE
  #define OS_PROFILE                        OS_PROFILE_DEFAULT
#endif

#ifndef   OS_SUPPORT_TICKSTEP
  #define OS_SUPPORT_TICKSTEP               OS_SUPPORT_TICKSTEP_DEFAULT
#endif

#ifndef   OS_TRACE
  #define OS_TRACE                          OS_TRACE_DEFAULT
#endif

#ifndef   OS_TRACE_RECORD_API_END
  #define OS_TRACE_RECORD_API_END           OS_PROFILE_DEFAULT
#endif

#ifndef   OS_RR_SUPPORTED
  #define OS_RR_SUPPORTED                   OS_RR_SUPPORTED_DEFAULT
#endif

#ifndef   OS_TRACKNAME
  #define OS_TRACKNAME                      OS_TRACKNAME_DEFAULT
#endif

#ifndef   OS_SUPPORT_SAVE_RESTORE_HOOK
  #define OS_SUPPORT_SAVE_RESTORE_HOOK      OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT
#endif

#ifndef   OS_SUPPORT_STAT
  #define OS_SUPPORT_STAT                   OS_SUPPORT_STAT_DEFAULT
#endif

#ifndef   OS_SUPPORT_PTLS
  #define OS_SUPPORT_PTLS                   OS_SUPPORT_PTLS_DEFAULT
#endif

#ifndef   OS_INIT_EXPLICITLY
  #define OS_INIT_EXPLICITLY                OS_INIT_EXPLICITLY_DEFAULT
#endif

#ifndef   OS_SUPPORT_TIMER
  #define OS_SUPPORT_TIMER                  (1)
#endif

#ifndef   OS_SUPPORT_TICKLESS
  #define OS_SUPPORT_TICKLESS               (1)
#endif

#ifndef   OS_SUPPORT_PERIPHERAL_POWER_CTRL
  #define OS_SUPPORT_PERIPHERAL_POWER_CTRL  (1)
#endif

#ifndef   OS_POWER_NUM_COUNTERS
  #define OS_POWER_NUM_COUNTERS             (5u)
#endif

#ifndef   OS_SPINLOCK_MAX_CORES
  #define OS_SPINLOCK_MAX_CORES             (4u)
#endif

#ifndef   OS_SUPPORT_OS_ALLOC
  #define OS_SUPPORT_OS_ALLOC               (1)
#endif

/*********************************************************************
*
*       Fixed CPU/compiler specific compile time switches
*
*  You must not change any of the following defines!!
*  They are already set in OSCHIP.h according to the used core and compiler
*
**********************************************************************
*/

/*********************************************************************
*
*       Memory attributes
*
**********************************************************************
*/

//
// Make it possible to place const data in RAM via
// define and therefore via command line. This is required
// for some smaller CPUs in small memory models
// It is defined to be "const" by default, but can be defined to nothing
// by simply adding a -DOS_CONST_DATA on the commandline
//

#ifndef   OS_CONST_DATA
  #define OS_CONST_DATA  const
#else
  #undef  OS_CONST_DATA
  #define OS_CONST_DATA
#endif

#ifndef   OS_CONST_PTR
  #define OS_CONST_PTR   const  // Default: const pointer declared as const
#else
  #undef  OS_CONST_PTR          // May have been overwritten for CPUs where "const" memory addresses ROM instead of RAM
  #define OS_CONST_PTR
#endif

#ifndef   OS_ROM_DATA
  #define OS_ROM_DATA
#endif

#ifndef   OS_CODE_SECTION_ATTRIBUTE
  #define OS_CODE_SECTION_ATTRIBUTE
#endif

#ifndef   OS_SECURE_ATTRIBUTE
  #define OS_SECURE_ATTRIBUTE
#endif

#ifndef   OS_RAM_SECTION_ATTRIBUTE
  #define OS_RAM_SECTION_ATTRIBUTE
#endif

#ifndef   OS_STACKPTR
  #define OS_STACKPTR
#endif

#ifndef   OS_SADDR
  #define OS_SADDR
#endif

/*********************************************************************
*
*       C Standard API Macros
*
**********************************************************************
*/
#ifndef   OS_MEMSET
  #define OS_MEMSET(a,v,s)        memset((a),((int) (v)),(size_t)(s))
#endif

#ifndef   OS_MEMCPY
  #define OS_MEMCPY(dest,src,cnt) memcpy((dest),(src),(size_t)(cnt))
#endif

#ifndef   OS_STRLEN
  #define OS_STRLEN(s)            strlen(s)
#endif

#ifndef   OS_COPYSTRING
  #define OS_COPYSTRING           OS_MEMCPY
#endif

/*********************************************************************
*
*       Core / compiler specific settings
*
**********************************************************************
*/
//
#ifndef   OS_USEPARA
  #define OS_USEPARA(para)                   (para)=(para)
#endif

#ifndef   OS_TASK_HAS_PTLS
  #define OS_TASK_HAS_PTLS                   OS_SUPPORT_PTLS  // May be defined in OSCHIPh to generate equal task structure for different embOS versions, even when TLS is not supported
#endif

#ifndef   OS_TLS_PTR
  #define OS_TLS_PTR                         void*
#endif

#ifndef   OS_STACK_AT_BOTTOM
  #define OS_STACK_AT_BOTTOM                 (0)
#endif

#ifndef   OS_DUMMY_STATEMENT
  #define OS_DUMMY_STATEMENT()
#endif

#ifndef   OS_ALIGN_PTR
  #define OS_ALIGN_PTR                       (1)
#endif

#ifndef   OS_ALIGN_INT
  #define OS_ALIGN_INT                       (1)
#endif

#ifndef   OS_PTR_TO_VALUE
  #define OS_PTR_TO_VALUE(p)                 (p)
#endif

#ifndef   OS_DATA_MEMORY_BARRIER
  #define OS_DATA_MEMORY_BARRIER()
#endif

#ifndef   OS_SUPPORT_SYSSTACK_INFO
  #define OS_SUPPORT_SYSSTACK_INFO           (0)
#endif

#ifndef   OS_SUPPORT_INTSTACK_INFO
  #define OS_SUPPORT_INTSTACK_INFO           (0)
#endif

#ifndef   OS_INTERWORK
  #define OS_INTERWORK
#endif

#ifndef   OS_SUPPORT_MPU
  #define OS_SUPPORT_MPU                     (0)
#endif

#ifndef   OS_USE_REGIONCNT_API
  #define OS_USE_REGIONCNT_API               (0)
#endif

/*********************************************************************
*
*       Interrupt specific settings
*
**********************************************************************
*/
#ifndef   OS_SCHEDULER_ACTIVATED_BY_EXCEPTION
  #define OS_SCHEDULER_ACTIVATED_BY_EXCEPTION   (0)
#endif

#ifndef   OS_SUPPORT_CALL_ISR
  #define OS_SUPPORT_CALL_ISR                   (1)
#endif

#ifndef   OS_SUPPORT_INT_PRIORITY
  #define OS_SUPPORT_INT_PRIORITY               (1)
#endif

#ifndef   OS_IPL_DI_TYPE
  #define OS_IPL_DI_TYPE                        unsigned int
#endif

#ifndef   OS_IPL_EI_TYPE
  #define OS_IPL_EI_TYPE                        unsigned int
#endif

#ifndef   OS_SUPPORT_ENTER_INTERRUPT                 // Some CPUs may implement a special version of OS_INT_Call() written in assembler (For example F16)
  #define OS_SUPPORT_ENTER_INTERRUPT            (1)  // When Set to 0, OS_INT_Enter() is not defined, the generic version of OS_INT_Call is also not defined then
#endif

#ifndef   OS_DISALLOW_EI_IN_CHANGETASK
  #define OS_DISALLOW_EI_IN_CHANGETASK          (0)  // Used for embOS ports (like embOS Xtensa GCC) where nested interrupts are not possible
#endif

#ifndef   OS_SWITCH_FROM_INT_MODIFIES_STACK
  #define OS_SWITCH_FROM_INT_MODIFIES_STACK     (0)
#endif

#ifndef   OS_INTERRUPTS_ARE_NESTABLE_ON_ENTRY
  #define OS_INTERRUPTS_ARE_NESTABLE_ON_ENTRY   (0)
#endif
//
// Typically no need to change, only used for Renesas SH2A
//
#ifndef   OS_INTPRIO_DISABLE
  #define OS_INTPRIO_DISABLE()                  { OS_INTPRIO_SET(OS_Ipl_DI); }
#endif
#ifndef   OS_INTPRIO_GET_DISABLE
  #define OS_INTPRIO_GET_DISABLE(v)             { OS_INTPRIO_GET(v); OS_INTPRIO_DISABLE(); }
#endif
#ifndef   OS_INTPRIO
  #define OS_INTPRIO                            unsigned int
#endif

/*********************************************************************
*
*       Misc.  settings
*
**********************************************************************
*/
#ifndef OS_IDLE
  #if (OS_DEBUG != 0)
    #define OS_IDLE() OS_Idle(); \
                      OS_Error(OS_ERR_IDLE_RETURNS);   // We should never arrive here, since the Idle loop should not return
  #else
    #define OS_IDLE() OS_Idle();
  #endif
#endif

#ifndef   OS_SUSPEND_TASK_ON_TERMINATE  // Sets the task state of a terminated task to "suspended". Primarily used in the OS simulation, because the windows scheduler might re-activate a suspended task
  #define OS_SUSPEND_TASK_ON_TERMINATE (0)
#endif

#ifndef   OS_TASK_LIST_HAS_PPREV
  #define OS_TASK_LIST_HAS_PPREV  OS_RR_SUPPORTED
#else
  #if (OS_TASK_LIST_HAS_PPREV == 0) && (OS_RR_SUPPORTED != 0)
    #error  "Invalid combination of config switches"
  #endif
#endif

/*********************************************************************
*
*       Check Compile time switches
*
**********************************************************************
*/

/*********************************************************************
*
*       Optimizations for pointer and word (32-bit) operations.
*
*  These are atomic on most 32-bit CPUs, which allows us to keep the code
*  efficient. On some smaller CPUs, these operations are not atomic,
*  which may require interrupts to be disabled or similar code overhead.
*
*  In case of doubt, set to 0 which is the safe, but maybe not optimum value.
*/
#ifndef  OS_PTR_OP_IS_ATOMIC
  #error "OS_PTR_OP_IS_ATOMIC should be defined in OSCHIP.h"
#endif

#ifndef  OS_U32_OP_IS_ATOMIC
  #error "OS_U32_OP_IS_ATOMIC should be defined in OSCHIP.h"
#endif

#ifndef    OS_SIZEOF_INT
  #define  OS_SIZEOF_INT  4u
#endif

#if (OS_STACK_AT_BOTTOM != 0) && (OS_STACK_AT_BOTTOM != 1)
  //
  //  Only 0 or 1 is allowed, because OS_STACK_AT_BOTTOM is used for calculations!
  //
  #error "OS_STACK_AT_BOTTOM has to be defined to 0 or 1"
#endif

#if (OS_SUPPORT_INT_PRIORITY != 0)
  #ifndef OS_IPL_EI_DEFAULT
    #error "Please define OS_IPL_EI_DEFAULT (OSCHIP.h)"
  #endif
  #ifndef OS_IPL_DI_DEFAULT
    #error "Please define OS_IPL_DI_DEFAULT (OSCHIP.h)"
  #endif
#endif

#if (OS_POWER_NUM_COUNTERS == 0u)
  #error "OS_POWER_NUM_COUNTERS has to be defined > 0"
#endif

#if (OS_SPINLOCK_MAX_CORES == 0u)
  #error "OS_SPINLOCK_MAX_CORES has to be defined > 0"
#endif

/*********************************************************************
*
*       Basic type defines
*
**********************************************************************
*/
#ifndef   OS_I8
  #define OS_I8     signed char
#endif

#ifndef   OS_U8
  #define OS_U8     unsigned char
#endif

#ifndef   OS_I16
  #define OS_I16    signed short
#endif

#ifndef   OS_U16
  #define OS_U16    unsigned short
#endif

#ifndef   OS_I32
  #define OS_I32    long
#endif

#ifndef   OS_U32
  #define OS_U32    unsigned OS_I32
#endif

#ifdef    OS_I64
  #ifndef   OS_U64
    #define OS_U64  unsigned OS_I64
  #endif
#endif

/*********************************************************************
*
*       embOS type defines
*
**********************************************************************
*/
#ifndef   OS_INT
  #define OS_INT        int  // Defines an integer. This type is guaranteed to have at least 16 bits.
#endif

#ifndef   OS_UINT
  #define OS_UINT       unsigned OS_INT
#endif

#ifndef   OS_TIME
  #define OS_TIME       int
#endif

#ifndef   OS_STAT
  #define OS_STAT       OS_U8
#endif

#ifndef   OS_BOOL
  #define OS_BOOL       OS_U8
#endif

#ifndef   OS_SPINLOCK
  #define OS_SPINLOCK   volatile OS_UINT
#endif

#ifndef   OS_STACK_ADR
  #define OS_STACK_ADR  OS_U32
#endif

//
// Since version 3.80k, the size of task events may be modified up to unsigned (which is 16bit or 32bit, depending on the CPU)
// If not defined in OSCHIP.h, the chip specific part of RTOS.h,
// OS_TASKEVENT defaults to OS_U8 to be compatible to older versions.
// Since version 3.86f of embOS, OS_TASKEVENT defaults to OS_U32 for 32bit CPUs (when not overwritten in OSCHIP.h)
//
#ifndef OS_TASKEVENT
  #if (OS_SIZEOF_INT >= 4u)
    #define OS_TASKEVENT  OS_U32
  #else
    #define OS_TASKEVENT  OS_U8
  #endif
#endif

//
// Since version 4.04, the size of task priority may be modified up to unsigned int (which is 16bit or 32bit, depending on the CPU)
// If not defined in OSCHIP.h OS_PRIO is set to OS_U32 when the size of an int is 32bit and is set to OS_U8
// when the size of an int is 16bit.
//
#ifndef OS_PRIO
  #if (OS_SIZEOF_INT >= 4u)
    #define OS_PRIO OS_U32
  #else
    #define OS_PRIO OS_U8
  #endif
#endif

/*********************************************************************
*
*       Error codes
*
**********************************************************************
*/

typedef enum {
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

// internal errors, please contact SEGGER Microcontroller
  OS_ERR_MAILBOX_NOT_IN_LIST             = (140u),  // The mailbox is not in the list of mailboxes as expected. Possible reasons may be that one mailbox data structure was overwritten.
  OS_ERR_TASKLIST_CORRUPT                = (142u),  // The OS internal task list is destroyed.

// Queue errors
  OS_ERR_QUEUE_INUSE                     = (143u),  // Queue in use.
  OS_ERR_QUEUE_NOT_INUSE                 = (144u),  // Queue not in use.
  OS_ERR_QUEUE_INVALID                   = (145u),  // Queue invalid.
  OS_ERR_QUEUE_DELETE                    = (146u),  // A queue was deleted by a call of OS_QUEUE_Delete() while tasks are waiting at the queue.

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
  OS_ERR_MUTEX_OVERFLOW                  = (159u),  // OS_MUTEX_Lock(), OS_MUTEX_LockBlocked() or OS_MUTEX_LockTimed() has been called too often from the same task.

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
  OS_ERR_SUSPEND_TOO_OFTEN               = (200u),  // Nested call of OS_TASK_Suspend() exceeded OS_MAX_SUSPEND_CNT.
  OS_ERR_RESUME_BEFORE_SUSPEND           = (201u),  // OS_TASK_Resume() called on a task that was not suspended.

// Other task related errors
  OS_ERR_TASK_PRIORITY                   = (202u),  // OS_TASK_Create() was called with a task priority which is already assigned to another task. This error can only occur when embOS was compiled without round-robin support.
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
  OS_ERR_NON_TIMERCYCLES_FUNC            = (235u),  // Callback function for timer counter value has not been set. Required by OS_TIME_Getus().
  OS_ERR_NON_TIMERINTPENDING_FUNC        = (236u),  // Callback function for timer interrupt pending flag has not been set. Required by OS_TIME_Getus().

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
} OS_STATUS;

/*********************************************************************
*
*       Assertions
*
*  Assertions are used to generate code in the debug version
*  of embOS in order catch programming faults like
*  bad pointers or uninitialized data structures
*
**********************************************************************
*/

#if (OS_DEBUG != 0)
  #define OS_ASSERT(Exp, ErrCode)  { if (!(Exp)) {OS_Error(ErrCode); }}
#else
  #define OS_ASSERT(Exp, ErrCode)
#endif

#define OS_ASSERT_DICNT()        OS_ASSERT(((OS_Global.Counters.Cnt.DI & 0xF0u) == 0u), OS_ERR_DICNT)
#define OS_ASSERT_INIT_CALLED()  OS_ASSERT((OS_InitCalled != 0u),                       OS_ERR_INIT_NOT_CALLED)

#if (OS_DEBUG != 0)
  //
  // OS_ASSERT_CPU_IN_ISR_MODE is typically called from OS_INT_Enter() and checks the hardware state of the CPU
  //
  #define OS_ASSERT_CPU_IN_ISR_MODE()  OS_AssertCPUInISRMode()
#else
  #define OS_ASSERT_CPU_IN_ISR_MODE()
#endif

/*********************************************************************
*
*       Trace identifiers for embOSView and SystemView
*
**********************************************************************
*/
//
// Identifier from 0 to 99 and 128 to 255 are reserved for the OS.
// Identifier from 100 to 127 are reserved for the application.
// Even when not all of those are currently used, they may be defined in the future
//
#define OS_TRACE_ID_DEACTIVATE                      (1u)
#define OS_TRACE_ID_ACTIVATE                        (2u)
#define OS_TRACE_ID_TIMERCALLBACK                   (3u)
#define OS_TRACE_ID_TASK_DELAY                     (10u)
#define OS_TRACE_ID_TASK_DELAYUNTIL                (11u)
#define OS_TRACE_ID_TASK_SETPRIORITY               (12u)
#define OS_TRACE_ID_TASK_WAKE                      (13u)
#define OS_TRACE_ID_TASK_CREATE                    (14u)
#define OS_TRACE_ID_TASK_TERMINATE                 (15u)
#define OS_TRACE_ID_TASK_SUSPEND                   (16u)
#define OS_TRACE_ID_TASK_RESUME                    (17u)
#define OS_TRACE_ID_TASK_CREATEEX                  (18u)
#define OS_TRACE_ID_TASK_YIELD                     (19u)
#define OS_TRACE_ID_TASKEVENT_GETBLOCKED           (20u)
#define OS_TRACE_ID_TASKEVENT_GETTIMED             (21u)
#define OS_TRACE_ID_TASKEVENT_SET                  (22u)
#define OS_TRACE_ID_TASKEVENT_CLEAR                (23u)
#define OS_TRACE_ID_MAILBOX_CREATE                 (30u)
#define OS_TRACE_ID_MAILBOX_CLEAR                  (31u)
#define OS_TRACE_ID_MAILBOX_DELETE                 (32u)
#define OS_TRACE_ID_MAILBOX_PUTBLOCKED             (33u)
#define OS_TRACE_ID_MAILBOX_GETBLOCKED             (34u)
#define OS_TRACE_ID_MAILBOX_PUT                    (35u)
#define OS_TRACE_ID_MAILBOX_GET                    (36u)
#define OS_TRACE_ID_MAILBOX_PUTTIMED               (37u)
#define OS_TRACE_ID_MAILBOX_GETTIMED               (38u)
#define OS_TRACE_ID_MAILBOX_WAITBLOCKED            (39u)
#define OS_TRACE_ID_MAILBOX_PUTBLOCKED1            (40u)
#define OS_TRACE_ID_MAILBOX_GETBLOCKED1            (41u)
#define OS_TRACE_ID_MAILBOX_PUT1                   (42u)
#define OS_TRACE_ID_MAILBOX_GET1                   (43u)
#define OS_TRACE_ID_MAILBOX_PUTTIMED1              (44u)
#define OS_TRACE_ID_MAILBOX_GETTIMED1              (45u)
#define OS_TRACE_ID_MAILBOX_PUTFB                  (46u)
#define OS_TRACE_ID_MAILBOX_PUTFB1                 (47u)
#define OS_TRACE_ID_MAILBOX_PUTFRONT               (48u)
#define OS_TRACE_ID_MAILBOX_PUTFRONT1              (49u)
#define OS_TRACE_ID_MUTEX_CREATE                   (50u)
#define OS_TRACE_ID_MUTEX_LOCKBLOCKED              (51u)
#define OS_TRACE_ID_MUTEX_UNLOCK                   (52u)
#define OS_TRACE_ID_MUTEX_LOCK                     (53u)
#define OS_TRACE_ID_MUTEX_GETVALUE                 (54u)
#define OS_TRACE_ID_MUTEX_DELETE                   (55u)
#define OS_TRACE_ID_MUTEX_ISMUTEX                  (56u)
#define OS_TRACE_ID_MAILBOX_WAITTIMED              (59u)
#define OS_TRACE_ID_SEMAPHORE_CREATE               (60u)
#define OS_TRACE_ID_SEMAPHORE_DELETE               (61u)
#define OS_TRACE_ID_SEMAPHORE_GIVE                 (62u)
#define OS_TRACE_ID_SEMAPHORE_TAKEBLOCKED          (63u)
#define OS_TRACE_ID_SEMAPHORE_TAKETIMED            (64u)
#define OS_TRACE_ID_SEMAPHORE_GIVEMAX              (65u)
#define OS_TRACE_ID_SEMAPHORE_SETVALUE             (66u)
#define OS_TRACE_ID_SEMAPHORE_TAKE                 (67u)
#define OS_TRACE_ID_TIMER_CREATE                   (70u)
#define OS_TRACE_ID_TIMER_DELETE                   (71u)
#define OS_TRACE_ID_TIMER_START                    (72u)
#define OS_TRACE_ID_TIMER_STOP                     (73u)
#define OS_TRACE_ID_TIMER_RESTART                  (74u)
#define OS_TRACE_ID_TIMER_SETPERIOD                (75u)
#define OS_TRACE_ID_TIMER_CREATEEX                 (76u)
#define OS_TRACE_ID_HEAP_MALLOC                    (80u)
#define OS_TRACE_ID_HEAP_FREE                      (81u)
#define OS_TRACE_ID_HEAP_REALLOC                   (82u)
#define OS_TRACE_ID_MEMPOOL_CREATE                 (90u)
#define OS_TRACE_ID_MEMPOOL_DELETE                 (91u)
#define OS_TRACE_ID_MEMPOOL_ALLOCBLOCKED           (92u)
#define OS_TRACE_ID_TASK_DELAYUS                  (131u)
#define OS_TRACE_ID_TASK_SUSPENDALL               (132u)
#define OS_TRACE_ID_TASK_RESUMEALL                (133u)
#define OS_TRACE_ID_TASKEVENT_GETSIBLOCKED        (134u)
#define OS_TRACE_ID_TASKEVENT_GETSITIMED          (135u)
#define OS_TRACE_ID_EVENT_PULSE                   (136u)
#define OS_TRACE_ID_EVENT_RESET                   (137u)
#define OS_TRACE_ID_EVENT_SET                     (138u)
#define OS_TRACE_ID_EVENT_GETBLOCKED              (139u)
#define OS_TRACE_ID_EVENT_GETTIMED                (140u)
#define OS_TRACE_ID_EVENT_CREATE                  (141u)
#define OS_TRACE_ID_EVENT_CREATEEX                (142u)
#define OS_TRACE_ID_EVENT_DELETE                  (143u)
#define OS_TRACE_ID_MAILBOX_PEEK                  (144u)
#define OS_TRACE_ID_QUEUE_CREATE                  (145u)
#define OS_TRACE_ID_QUEUE_DELETE                  (146u)
#define OS_TRACE_ID_QUEUE_CLEAR                   (147u)
#define OS_TRACE_ID_QUEUE_PUTBLOCKED              (148u)
#define OS_TRACE_ID_QUEUE_PUT                     (149u)
#define OS_TRACE_ID_QUEUE_PUTTIMED                (150u)
#define OS_TRACE_ID_QUEUE_GETPTRBLOCKED           (151u)
#define OS_TRACE_ID_QUEUE_GETPTR                  (152u)
#define OS_TRACE_ID_QUEUE_GETPTRTIMED             (153u)
#define OS_TRACE_ID_QUEUE_PEEKPTR                 (154u)
#define OS_TRACE_ID_QUEUE_PURGE                   (155u)
#define OS_TRACE_ID_MEMPOOL_ALLOCTIMED            (156u)
#define OS_TRACE_ID_MEMPOOL_FREE                  (157u)
#define OS_TRACE_ID_MEMPOOL_FREEEX                (158u)
#define OS_TRACE_ID_MEMPOOL_ALLOC                 (159u)
#define OS_TRACE_ID_MUTEX_LOCKTIMED               (163u)
#define OS_TRACE_ID_TASK_SETNAME                  (167u)
#define OS_TRACE_ID_TICKLESS_ADJUSTTIME           (168u)
#define OS_TRACE_ID_EVENT_GET                     (169u)
#define OS_TRACE_ID_EVENT_GETRESETMODE            (170u)
#define OS_TRACE_ID_EVENT_SETRESETMODE            (171u)
#define OS_TRACE_ID_TICKLESS_GETNUMIDLETICKS      (172u)
#define OS_TRACE_ID_TASK_GETNUMTASKS              (173u)
#define OS_TRACE_ID_TASK_GETPRIORITY              (174u)
#define OS_TRACE_ID_TASK_GETSUSPENDCNT            (175u)
#define OS_TRACE_ID_MAILBOX_GETPTRBLOCKED         (176u)
#define OS_TRACE_ID_POWER_USAGEINC                (177u)
#define OS_TRACE_ID_POWER_USAGEDEC                (178u)
#define OS_TRACE_ID_POWER_GETMASK                 (179u)
#define OS_TRACE_ID_TASK_SETINITIALSUSPENDCNT     (180u)
#define OS_TRACE_ID_TIME_GETUS                    (181u)
#define OS_TRACE_ID_TIME_GETUS64                  (182u)
#define OS_TRACE_ID_TICK_ADDHOOK                  (185u)
#define OS_TRACE_ID_TICK_REMOVEHOOK               (186u)
#define OS_TRACE_ID_TICKLESS_START                (187u)
#define OS_TRACE_ID_TICKLESS_STOP                 (188u)
#define OS_TRACE_ID_SEMAPHORE_GETVALUE            (189u)
#define OS_TRACE_ID_TASKEVENT_GET                 (190u)
#define OS_TRACE_ID_TASK_ISTASK                   (191u)
#define OS_TRACE_ID_QUEUE_GETMSGCNT               (192u)
#define OS_TRACE_ID_QUEUE_GETMSGSIZE              (193u)
#define OS_TRACE_ID_QUEUE_ISUNUSE                 (194u)
#define OS_TRACE_ID_TASK_SETTIMESLICE             (195u)
#define OS_TRACE_ID_MUTEX_GETOWNER                (196u)
#define OS_TRACE_ID_TIMER_GETPERIOD               (197u)
#define OS_TRACE_ID_TIMER_GETSTATUS               (198u)
#define OS_TRACE_ID_TIMER_GETREMAININGPERIOD      (199u)
#define OS_TRACE_ID_TIME_STARTMEASUREMENT         (200u)
#define OS_TRACE_ID_TIME_STOPMEASUREMENT          (201u)
#define OS_TRACE_ID_TIME_GETRESULTUS              (202u)
#define OS_TRACE_ID_MAILBOX_GETPTR                (203u)
#define OS_TRACE_ID_MAILBOX_PURGE                 (204u)
#define OS_TRACE_ID_QUEUE_PUTBLOCKEDEX            (205u)
#define OS_TRACE_ID_QUEUE_PUTEX                   (206u)
#define OS_TRACE_ID_QUEUE_PUTTIMEDEX              (207u)
#define OS_TRACE_ID_SPINLOCK_CREATE               (208u)
#define OS_TRACE_ID_SPINLOCK_SW_CREATE            (209u)
#define OS_TRACE_ID_SPINLOCK_LOCK                 (210u)
#define OS_TRACE_ID_SPINLOCK_SW_LOCK              (211u)
#define OS_TRACE_ID_SPINLOCK_UNLOCK               (212u)
#define OS_TRACE_ID_SPINLOCK_SW_UNLOCK            (213u)
#define OS_TRACE_ID_TIMER_TRIGGER                 (214u)
#define OS_TRACE_ID_TASK_ADDTERMINATEHOOK         (215u)
#define OS_TRACE_ID_TASK_REMOVEALLTERMINATEHOOKS  (216u)
#define OS_TRACE_ID_TASK_REMOVETERMINATEHOOK      (217u)
#define OS_TRACE_ID_TASKEVENT_CLEAREX             (218u)
#define OS_TRACE_ID_EVENT_GETMASK                 (219u)
#define OS_TRACE_ID_EVENT_SETMASK                 (220u)
#define OS_TRACE_ID_EVENT_GETMASKBLOCKED          (221u)
#define OS_TRACE_ID_EVENT_GETMASKTIMED            (222u)
#define OS_TRACE_ID_WD_ADD                        (223u)
#define OS_TRACE_ID_WD_CHECK                      (224u)
#define OS_TRACE_ID_WD_CONFIG                     (225u)
#define OS_TRACE_ID_WD_REMOVE                     (226u)
#define OS_TRACE_ID_WD_TRIGGER                    (227u)
#define OS_TRACE_ID_EVENT_SETMASKMODE             (228u)
#define OS_TRACE_ID_EVENT_GETMASKMODE             (229u)
#define OS_TRACE_ID_CONFIGSTOP                    (230u)
#define OS_TRACE_ID_STOP                          (231u)
#define OS_TRACE_ID_STACK_SETCHECKLIMIT           (232u)
#define OS_TRACE_ID_STACK_GETCHECKLIMIT           (233u)
#define OS_TRACE_ID_DEBUG_SETOBJNAME              (234u)
#define OS_TRACE_ID_DEBUG_GETOBJNAME              (235u)
#define OS_TRACE_ID_TASK_SETDEFAULTSTARTHOOK      (236u)
#define OS_TRACE_ID_DEINIT                        (237u)
#define OS_TRACE_ID_DEBUG_REMOVEOBJNAME           (238u)
#define OS_TRACE_ID_TIME_GETCYCLES                (239u)
#define OS_TRACE_ID_TIME_CVT_CYCL_TO_USEC         (240u)
#define OS_TRACE_ID_TIME_CONVERT_USEC_TO_CYCLES   (241u)
#define OS_TRACE_ID_TIME_CVT_CYCL_TO_NSEC         (242u)
#define OS_TRACE_ID_TIME_CONVERT_NSEC_TO_CYCLES   (243u)

//
// SystemView API IDs start at offset 32 whereas embOSView IDs starts at offset 0.
// The first 32 SystemView IDs are generic IDs.
// This offset is added for SystemView in order to maintain compatibility with embOSView.
//
#define OS_TRACE_API_OFFSET                   (32u)

//
// SystemView and embOSView trace macros (could also be used for any other trace tool)
//
#if (OS_PROFILE != 0)
  #define TRACE_RECORD_TASK_INFO(pTask)                                      if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskInfo((pTask)); }
  #define TRACE_RECORD_API_VOID(Id)                                          if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordVoid ((Id) + OS_TRACE_API_OFFSET);}                                                                                  OS_TRACE_Void((Id))
  #define TRACE_RECORD_API_U32(Id, Para0)                                    if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32  ((Id) + OS_TRACE_API_OFFSET, (OS_U32)(Para0));}                                                                 OS_TRACE_Data((Id), (int)(Para0))
  #define TRACE_RECORD_API_U32X2(Id, Para0, Para1)                           if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x2((Id) + OS_TRACE_API_OFFSET, (OS_U32)(Para0), (OS_U32)(Para1));}                                                OS_TRACE_Data((Id), (int)(Para0))
  #define TRACE_RECORD_API_U32X3(Id, Para0, Para1, Para2)                    if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x3((Id) + OS_TRACE_API_OFFSET, (OS_U32)(Para0), (OS_U32)(Para1), (OS_U32)(Para2));}                               OS_TRACE_Data((Id), (int)(Para0))
  #define TRACE_RECORD_API_U32X4(Id, Para0, Para1, Para2, Para3)             if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x4((Id) + OS_TRACE_API_OFFSET, (OS_U32)(Para0), (OS_U32)(Para1), (OS_U32)(Para2), (OS_U32)(Para3));}              OS_TRACE_Data((Id), (int)(Para0))
  #define TRACE_RECORD_API_PTR(Id, Para0)                                    if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32  ((Id) + OS_TRACE_API_OFFSET, OS_Global.pTrace->pfPtrToId((OS_U32)(Para0)));}                                    OS_TRACE_Ptr((Id), (Para0))
  #define TRACE_RECORD_API_PTR_U32(Id, Para0, Para1)                         if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x2((Id) + OS_TRACE_API_OFFSET, OS_Global.pTrace->pfPtrToId((OS_U32)(Para0)), (OS_U32)(Para1));}                   OS_TRACE_PtrU32((Id),  (Para0), (OS_U32)(Para1))
  #define TRACE_RECORD_API_PTR_U32X2(Id, Para0, Para1, Para2)                if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x3((Id) + OS_TRACE_API_OFFSET, OS_Global.pTrace->pfPtrToId((OS_U32)(Para0)), (OS_U32)(Para1), (OS_U32)(Para2));}  OS_TRACE_PtrU32((Id),  (Para0), (OS_U32)(Para1))
  #define TRACE_RECORD_API_PTR_U32X3(Id, Para0, Para1, Para2, Para3)         if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x4((Id) + OS_TRACE_API_OFFSET, OS_Global.pTrace->pfPtrToId((OS_U32)(Para0)), (OS_U32)(Para1), (OS_U32)(Para2), (OS_U32)(Para3)); } OS_TRACE_PtrU32((Id),  (Para0), (OS_U32)(Para1))
  #define TRACE_RECORD_API_PTR_U32X4(Id, Para0, Para1, Para2, Para3, Para4)  if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x5((Id) + OS_TRACE_API_OFFSET, OS_Global.pTrace->pfPtrToId((OS_U32)(Para0)), (OS_U32)(Para1), (OS_U32)(Para2), (OS_U32)(Para3), (OS_U32)(Para4)); } OS_TRACE_PtrU32((Id),  (Para0), (OS_U32)(Para1))
  #define TRACE_ON_ISR_ENTER()                                               if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordEnterISR(); }
  #define TRACE_ON_ISR_EXIT()                                                if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordExitISR(); }
  #define TRACE_ON_ISR_EXIT_TO_SCHEDULER()                                   if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordExitISRToScheduler(); }
  #define TRACE_ON_TASK_START_EXEC(TaskId)                                   if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskStartExec((OS_U32)(TaskId)); }; OS_TRACE_Void(OS_TRACE_ID_ACTIVATE)
  #define TRACE_ON_TASK_STOP_EXEC()                                          if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskStopExec(); }
  #define TRACE_ON_TASK_START_READY(TaskId)                                  if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskStartReady((OS_U32)(TaskId)); }
  #define TRACE_ON_TASK_STOP_READY(TaskId, Para0)                            if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskStopReady((OS_U32)(TaskId), (unsigned int)(Para0)); }
  #define TRACE_ON_TASK_CREATE(TaskId)                                       if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskCreate((OS_U32)(TaskId)); }
  #define TRACE_ON_IDLE()                                                    if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordIdle(); }
  #define TRACE_ON_TIMER_ENTER(TimerId)                                      if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordEnterTimer((OS_U32)(TimerId)); }; OS_TracePtr(OS_TRACE_ID_TIMERCALLBACK, TimerId)
  #define TRACE_ON_TIMER_EXIT()                                              if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordExitTimer(); }
#if (OS_TRACE_RECORD_API_END != 0)
  #define TRACE_RECORD_API_END(Id)                                           if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordEndCall((Id) + OS_TRACE_API_OFFSET); }
  #define TRACE_RECORD_API_END_U32(Id, Para0)                                if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordEndCallU32((Id) + OS_TRACE_API_OFFSET, (OS_U32)(Para0)); }
#else
  #define TRACE_RECORD_API_END(Id)
  #define TRACE_RECORD_API_END_U32(Id, Para0)
#endif
  #define TRACE_ON_TASK_TERMINATED(TaskId)                                   if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskTerminate((OS_U32)(TaskId)); }
  #define TRACE_RECORD_OBJNAME(Id, Para0)                                    if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordObjName((OS_U32)Id, Para0); }
#else
  #define TRACE_RECORD_TASK_INFO(pTask)
  #define TRACE_RECORD_API_VOID(Id)                                          OS_TRACE_Void((Id))
  #define TRACE_RECORD_API_U32(Id, Para0)                                    OS_TRACE_Data((Id), (int)(Para0))
  #define TRACE_RECORD_API_U32X2(Id, Para0, Para1)                           OS_TRACE_Data((Id), (int)(Para0))
  #define TRACE_RECORD_API_U32X3(Id, Para0, Para1, Para2)                    OS_TRACE_Data((Id), (int)(Para0))
  #define TRACE_RECORD_API_U32X4(Id, Para0, Para1, Para2, Para3)             OS_TRACE_Data((Id), (int)(Para0))
  #define TRACE_RECORD_API_PTR(Id, Para0)                                    OS_TRACE_Ptr((Id), (Para0));
  #define TRACE_RECORD_API_PTR_U32(Id, Para0, Para1)                         OS_TRACE_U32Ptr((Id), (OS_U32)(Para1), (Para0))
  #define TRACE_RECORD_API_PTR_U32X2(Id, Para0, Para1, Para2)                OS_TRACE_U32Ptr((Id), (OS_U32)(Para1), (Para0))
  #define TRACE_RECORD_API_PTR_U32X3(Id, Para0, Para1, Para2, Para3)         OS_TRACE_U32Ptr((Id), (OS_U32)(Para1), (Para0))
  #define TRACE_RECORD_API_PTR_U32X4(Id, Para0, Para1, Para2, Para3, Para4)  OS_TRACE_U32Ptr((Id), (OS_U32)(Para1), (Para0))
  #define TRACE_ON_ISR_ENTER()
  #define TRACE_ON_ISR_EXIT()
  #define TRACE_ON_ISR_EXIT_TO_SCHEDULER()
  #define TRACE_ON_TASK_START_EXEC(TaskId)                                   OS_TRACE_Void(OS_TRACE_ID_ACTIVATE)
  #define TRACE_ON_TASK_STOP_EXEC()
  #define TRACE_ON_TASK_START_READY(TaskId)
  #define TRACE_ON_TASK_STOP_READY(TaskId, Para0)
  #define TRACE_ON_TASK_CREATE(TaskId)
  #define TRACE_ON_IDLE()
  #define TRACE_ON_TIMER_ENTER(TimerId)                                      OS_TracePtr(OS_TRACE_ID_TIMERCALLBACK, TimerId)
  #define TRACE_ON_TIMER_EXIT()
  #define TRACE_RECORD_API_END(Id)
  #define TRACE_RECORD_API_END_U32(Id, Para0)
  #define TRACE_ON_TASK_TERMINATED(TaskId)
  #define TRACE_RECORD_OBJNAME(Id, Para0)
#endif

#ifdef __cplusplus
  extern "C" {
#endif

/*********************************************************************
*
*       embOSView settings
*
*  These defines shall be used in RTOSInit.c, e.g.:
*    #ifndef   OS_VIEW_IFSELECT
*      #define OS_VIEW_IFSELECT  OS_VIEW_IF_JLINK
*    #endif
**********************************************************************
*/
#define OS_VIEW_DISABLED     (0u)  // embOSView communication is disabled
#define OS_VIEW_IF_UART      (1u)  // If set, UART will be used for communication
#define OS_VIEW_IF_JLINK     (2u)  // If set, J-Link will be used for communication
#define OS_VIEW_IF_ETHERNET  (3u)  // If set, Ethernet will be used for communication

/*********************************************************************
*
*       Data structures and typedef definitions
*
**********************************************************************
*/
typedef struct OS_GLOBAL_STRUCT       OS_GLOBAL;
typedef struct OS_TASK_STRUCT         OS_TASK;
typedef struct OS_WAIT_LIST_STRUCT    OS_WAIT_LIST;
typedef struct OS_WAIT_OBJ_STRUCT     OS_WAIT_OBJ;
typedef struct OS_WD_STRUCT           OS_WD;


typedef void voidRoutine         (void);
typedef void OS_RX_CALLBACK      (OS_U8 Data);
typedef void OS_WD_RESET_CALLBACK(OS_CONST_PTR OS_WD* pWD);
typedef void OS_ON_TERMINATE_FUNC(OS_CONST_PTR OS_TASK* pTask);
typedef void OS_TICK_HOOK_ROUTINE(void);
#if (OS_SUPPORT_TIMER != 0)
typedef void OS_TIMERROUTINE     (void);
typedef void OS_TIMER_EX_ROUTINE (void* pVoid);
typedef void OS_TIMER_HANDLER    (OS_GLOBAL* pGlobal);
#endif

/**********************************************************************
*
*       OS_WAIT_OBJ
*       OS_WAIT_LIST
*/
struct OS_WAIT_OBJ_STRUCT {
  OS_WAIT_LIST* pWaitList;
};

struct OS_WAIT_LIST_STRUCT {
  OS_WAIT_LIST* pNext;  // Needs to be first element!
  OS_WAIT_LIST* pPrev;
  OS_WAIT_OBJ*  pWaitObj;
  OS_TASK*      pTask;
};

/*********************************************************************
*
*       Memory protection unit support
*
**********************************************************************
*/
typedef enum {
  OS_MPU_OBJTYPE_INVALID,
  OS_MPU_OBJTYPE_TASK,
  OS_MPU_OBJTYPE_MUTEX,
  OS_MPU_OBJTYPE_SEMA,
  OS_MPU_OBJTYPE_EVENT,
  OS_MPU_OBJTYPE_QUEUE,
  OS_MPU_OBJTYPE_MAILBOX,
  OS_MPU_OBJTYPE_SWTIMER,
  OS_MPU_OBJTYPE_MEMPOOL,
  OS_MPU_OBJTYPE_WATCHDOG
} OS_MPU_OBJTYPE;

#if (OS_SUPPORT_MPU != 0)

//
// embOS MPU macros and typedefs
//
#ifndef   MAX_MPU_REGIONS
  #define MAX_MPU_REGIONS  (16u)
#endif

typedef enum {
  OS_MPU_INVALID,    // 0x00
  OS_MPU_NOACCESS,   // 0x01
  OS_MPU_READONLY,   // 0x02
  OS_MPU_READWRITE   // 0x03
} OS_MPU_DATA_PERMISSION;

typedef enum {
  OS_MPU_EXECUTION_ALLOWED,        // 0x00
  OS_MPU_EXECUTION_DISALLOWED = 4  // 0x04
} OS_MPU_CODE_PERMISSION;

#define OS_MPU_DATA_PERMISSION_MASK  (0x03u)

typedef enum {
  OS_MPU_THREAD_STATE_PRIVILEGED,
  OS_MPU_THREAD_STATE_UNPRIVILEGED
} OS_MPU_THREAD_STATE;

typedef enum {
  OS_MPU_ERROR_NONE,
  OS_MPU_ERROR_INVALID_REGION,
  OS_MPU_ERROR_INVALID_OBJECT,
  OS_MPU_ERROR_INVALID_API,
  OS_MPU_ERROR_HARDFAULT,
  OS_MPU_ERROR_MEMFAULT,
  OS_MPU_ERROR_BUSFAULT,
  OS_MPU_ERROR_USAGEFAULT,
  OS_MPU_ERROR_SVC
} OS_MPU_ERRORCODE;

typedef enum {
  OS_MPU_SVC_OS_CALL,
  OS_MPU_SVC_DEVICE_DRIVER
} OS_MPU_SVC_NUMBER;

typedef struct {
  OS_MPU_ERRORCODE Error;
  OS_U32           pTask;
  OS_U32           PC;
  OS_U32           SP;
  OS_U32           r0;
  OS_U32           r1;
  OS_U32           r2;
  OS_U32           r3;
  OS_U32           r12;
} OS_MPU_DEBUG;

typedef struct {
  OS_U32 BaseAddr;
  OS_U32 Size;
  OS_U32 Permissions;
  OS_U32 Attributes;
} OS_MPU_REGION;

typedef struct {
  OS_U32         BaseAddr;
  OS_MPU_OBJTYPE Type;
} OS_MPU_OBJ;

typedef struct {
  OS_U32 ROM_BaseAddr;
  OS_U32 ROM_Size;
  OS_U32 RAM_BaseAddr;
  OS_U32 RAM_Size;
  OS_U32 OS_BaseAddr;
  OS_U32 OS_Size;
} OS_MPU_MEM_CONFIG;

typedef struct {
  const OS_MPU_OBJ*    pAllowedObjects;           // Objects to which the task has access
  OS_MPU_REGION        Regions[MAX_MPU_REGIONS];  // Regions to have the task access
  OS_MPU_THREAD_STATE  ThreadState;               // Current task privileged state
#ifdef OS_LIBMODE_SAFE
  void*                pBuffer;                   // Pointer to a buffer which holds a copy of the MPU hardware register for sanity check
#endif
} OS_MPU;

typedef struct OS_MPU_API_LIST_STRUCT {
  void   (*pfEnable)              (void);
  void   (*pfSwitchToUnprivState) (voidRoutine* pRoutine);
  void   (*pfEnterPrivilegedState)(void);
  void   (*pfLeavePrivilegedState)(void);
  OS_U32 (*pfGetPrivilegedState)  (void);
  void   (*pfCallDeviceDriver)    (OS_U32 Id, void* Param);
  void*  (*pfSaveAll)             (void* pStack);
  void*  (*pfRestoreAll)          (const void* pStack);
#ifdef OS_LIBMODE_SAFE
  OS_BOOL (*pfSanityCheck)         (const OS_TASK* pTask);
#endif
} OS_MPU_API_LIST;

typedef void (*OS_MPU_DEVICE_DRIVER_FUNC) (void* p);
typedef void (*OS_MPU_ERROR_CALLBACK)     (OS_TASK* pTask, OS_MPU_ERRORCODE ErrorCode);

#ifndef   OS_MPU_ASSERT_PRIVSTATE
  #define OS_MPU_ASSERT_PRIVSTATE()  OS_MPU_AssertPrivilegedState()
#endif

#else
  #define OS_MPU_ASSERT_PRIVSTATE()
#endif // OS_SUPPORT_MPU

/**********************************************************************
*
*       OS_EXTEND_TASK_CONTEXT
*
*  This structure is used to define a save and restore function for
*  extension of the task context.
*  A pointer to this structure is part of the task control block
*  It is initialized by OS_TASK_SetContextExtension();
*
*       OS_EXTEND_TASK_CONTEXT_LINK
*
*  This structure is used to have unlimited task context extensions as
*  linked list. The last pNext item points to NULL.
*/
typedef struct OS_EXTEND_TASK_CONTEXT_STRUCT {
  void OS_STACKPTR* (*pfSave)   (      void OS_STACKPTR* pStack);
  void OS_STACKPTR* (*pfRestore)(const void OS_STACKPTR* pStack);
} OS_EXTEND_TASK_CONTEXT;

typedef struct OS_EXTEND_TASK_CONTEXT_LINK_STRUCT {
  const OS_EXTEND_TASK_CONTEXT*              pTaskContext;
  struct OS_EXTEND_TASK_CONTEXT_LINK_STRUCT* pNext;
} OS_EXTEND_TASK_CONTEXT_LINK;

/**********************************************************************
*
*       OS_TASK
*
*  This structure (referred to as "task control block" or TCB) holds all relevant information
*  about a single task. Note that some elements are optional, depending on the compile time
*  options, especially the type of build
*/
struct OS_TASK_STRUCT {
  //
  // Elements required for all builds
  //
  // Start of assembly relevant section. Do not move these elements
  OS_TASK*              pNext;           // Points to the TCB of the next task in the list (with equal or lower priority). Needs to be first element !
  OS_REGS OS_STACKPTR*  pStack;          // Typically contains the stack pointer if the task is suspended.                 Needs to be second element !
  // End of assembly relevant section
  OS_WAIT_LIST*         pWaitList;       // Points to a waitable object if task is suspended.
  OS_TIME               Timeout;
  OS_STAT               Stat;
  OS_PRIO               Priority;
  OS_PRIO               BasePrio;
  volatile OS_TASKEVENT Events;          // Event storage
  OS_TASKEVENT          EventMask;       // Event mask
  //
  // pPrev is required only in builds with doubly-linked task lists (Round-robin)
  //
#if (OS_TASK_LIST_HAS_PPREV != 0)
  OS_TASK*              pPrev;
#endif
  //
  // Elements required with "Track-name" functionality only. Typically available in all builds, but can be disabled at compile time
  //
#if (OS_TRACKNAME != 0)
  OS_ROM_DATA const char* Name;
#endif
  //
  // Elements required with "Stack-check" functionality only. Available in stack-check and debug builds.
  //
#if ((OS_CHECKSTACK != 0) || (OS_SUPPORT_MPU != 0))
  OS_UINT               StackSize;       // Stack size in bytes. Not required for functionality, just for analysis
  OS_U8 OS_STACKPTR*    pStackBot;       // First byte of stack. Not required for functionality, just for analysis
#endif
  //
  // Elements required with profiling or debug builds
  //
#if (OS_SUPPORT_STAT != 0)
  OS_U32                NumActivations;  // Counts how many times task has been activated
  OS_U32                NumPreemptions;  // Counts how many times task has been preempted
#endif
  //
  // Elements required with profiling builds
  //
#if (OS_PROFILE != 0)
  OS_U32                ExecTotal;       // Time spent executing
  OS_U32                ExecLast;        // Time spent executing (Reference)
  OS_U32                Load;            // Profiling builds only:
#endif
  //
  // Elements required with Round-robin functionality only. Typically available in all builds, but can be disabled at compile time
  //
#if (OS_RR_SUPPORTED != 0)
  OS_U8                 TimeSliceRem;
  OS_U8                 TimeSliceReload;
#endif
  //
  // Optional Save & Restore hook support (usually only for bigger CPUs)
  //
#if (OS_SUPPORT_SAVE_RESTORE_HOOK != 0)
  OS_EXTEND_TASK_CONTEXT_LINK pExtendContext;
#endif
  //
  // Optional thread local storage pointer
  //
#if (OS_TASK_HAS_PTLS != 0)
   #if (defined(OS_LIBMODE_XR) && (OS_SUPPORT_PTLS == 0))
     // XR-Build may not support TLS
   #else
     OS_TLS_PTR         pTLS;
   #endif
#endif
  //
  // Optional embOS MPU
  //
#if (OS_SUPPORT_MPU != 0)
  OS_MPU                MPU_Config;
#endif
  //
  // Elements required with debug builds
  //
#if (OS_DEBUG != 0)
  OS_U8                 Id;              // Debug builds only: Id of this control block.
#endif
  //
  // Allow port specific extension to the task control block. Not used in most ports.
  //
#ifdef OS_TCB_CPU_EX
  OS_TCB_CPU_EX
#endif
};

#if (OS_SUPPORT_TIMER != 0)

/**********************************************************************
*
*       OS_TIMER
*/
typedef struct OS_TIMER_STRUCT OS_TIMER;
struct OS_TIMER_STRUCT {
  OS_TIMER*        pNext;
  OS_TIMERROUTINE* Hook;
  OS_TIME          Time;
  OS_TIME          Period;
  char             Active;
#if (OS_DEBUG != 0)
  OS_U8            Id;
#endif
};

/**********************************************************************
*
*       OS_TIMER_EX
*/
typedef struct {
  OS_TIMER             Timer;
  OS_TIMER_EX_ROUTINE* pfUser;
  void*                pData;
} OS_TIMER_EX;

#endif // OS_SUPPORT_TIMER

/**********************************************************************
*
*       OS_TICK_HOOK
*/
typedef struct OS_TICK_HOOK_STRUCT OS_TICK_HOOK;
struct OS_TICK_HOOK_STRUCT {
  OS_TICK_HOOK*         pNext;
  OS_TICK_HOOK_ROUTINE* pfUser;
};

/**********************************************************************
*
*       OS_ON_TERMINATE_HOOK
*/
typedef struct OS_ON_TERMINATE_HOOK_STRUCT OS_ON_TERMINATE_HOOK;
struct OS_ON_TERMINATE_HOOK_STRUCT {
  OS_ON_TERMINATE_HOOK* pNext;
  OS_ON_TERMINATE_FUNC* pfUser;
};

/**********************************************************************
*
*       OS_MUTEX
*/
typedef struct OS_MUTEX_STRUCT OS_MUTEX;
struct OS_MUTEX_STRUCT {
  OS_WAIT_OBJ    WaitObj;
  OS_TASK*       pTask;
  volatile OS_U8 UseCnt;
  OS_MUTEX*      pNext;
#if (OS_DEBUG != 0)
  OS_U8          Id;
#endif
};

/**********************************************************************
*
*       OS_SEMAPHORE
*/
typedef struct OS_SEMAPHORE_STRUCT OS_SEMAPHORE;
struct OS_SEMAPHORE_STRUCT {
  OS_WAIT_OBJ      WaitObj;
  volatile OS_UINT Cnt;
#if (OS_DEBUG != 0)
  OS_SEMAPHORE*    pNext;
  OS_U8            Id;
#endif
};

/**********************************************************************
*
*       OS_MAILBOX
*/
typedef struct OS_MAILBOX_STRUCT OS_MAILBOX;
struct OS_MAILBOX_STRUCT {
  OS_WAIT_OBJ      WaitObj;
  char*            pData;
  volatile OS_UINT nofMsg;
  OS_UINT          maxMsg;
  OS_UINT          iRd;
  OS_U16           sizeofMsg;
  OS_BOOL          InUse;
#if (OS_DEBUG != 0)
  OS_MAILBOX*      pNext;
  OS_U8            Id;
#endif
};

/**********************************************************************
*
*       OS_QUEUE
*/
typedef struct OS_QUEUE_STRUCT OS_QUEUE;
struct OS_QUEUE_STRUCT {
  OS_WAIT_OBJ WaitObj;
  OS_U8*      pData;
  OS_UINT     Size;
  OS_UINT     MsgCnt;
  OS_UINT     offFirst;
  OS_UINT     offLast;
  OS_BOOL     InUse;
  OS_UINT     InProgressCnt;
#if (OS_DEBUG != 0)
  OS_QUEUE*   pNext;
  OS_U8       Id;
#endif
};

/**********************************************************************
*
*       OS_QUEUE_SRCLIST
*/
typedef struct {
  OS_CONST_PTR void* pSrc;
  OS_UINT            Size;
} OS_QUEUE_SRCLIST;

/**********************************************************************
*
*       OS_MEMPOOL
*/
//
// Both defines are obsolete, we just keep it for compatibility
//
#define OS_MEMF_SIZEOF_BLOCKCONTROL     (0)
#define OS_MEMPOOL_SIZEOF_BLOCKCONTROL  (0)

typedef struct OS_MEMPOOL_STRUCT OS_MEMPOOL;
struct OS_MEMPOOL_STRUCT {
  OS_WAIT_OBJ      WaitObj;
  OS_MEMPOOL*      pNext;
  void*            pPool;
  OS_UINT          NumBlocks;
  OS_UINT          BlockSize;
  volatile OS_UINT NumFreeBlocks;
  OS_UINT          MaxUsed;
  void*            pFree;
#if (OS_DEBUG != 0)
  OS_U8            Id;
#endif
};

/**********************************************************************
*
*       OS_EVENT
*/
typedef struct OS_EVENT_STRUCT OS_EVENT;

typedef enum {
  OS_EVENT_RESET_MODE_SEMIAUTO = 0,  // Same as previous mode, mix from automatic an manual reset
  OS_EVENT_RESET_MODE_MANUAL,        // Manual reset, event remains set when waiting task is resumed, has to be reset by task
  OS_EVENT_RESET_MODE_AUTO           // automatic reset, event is automatically cleared when waiting task is resumed
} OS_EVENT_RESET_MODE;

typedef enum {
  OS_EVENT_MASK_MODE_OR_LOGIC  = 0,  // OR logic is used for event mask bits (default)
  OS_EVENT_MASK_MODE_AND_LOGIC = 4   // AND logic is used for event mask bits
} OS_EVENT_MASK_MODE;

struct OS_EVENT_STRUCT {
  OS_WAIT_OBJ           WaitObj;
  volatile OS_TASKEVENT Signaled;
  OS_EVENT_RESET_MODE   ResetMode;
  OS_EVENT_MASK_MODE    MaskMode;
#if (OS_DEBUG != 0)
  OS_EVENT*             pNext;
  OS_U8                 Id;
#endif
};

/**********************************************************************
*
*       OS_SPINLOCK / OS_SPINLOCK_SW
*/
typedef struct {
  volatile OS_U32 Entering[OS_SPINLOCK_MAX_CORES];
  volatile OS_U32 Number[OS_SPINLOCK_MAX_CORES];
} OS_SPINLOCK_SW;

/**********************************************************************
*
*       OS_WD - Watchdog support
*/
struct OS_WD_STRUCT {
  OS_WD*  pNext;
  OS_TIME Period;
  OS_TIME TimeDex;
};

/**********************************************************************
*
*       OS_TRACE_ENTRY
*/
typedef struct {
  OS_U32                      Time;
  void*                       pCurrentTask;
  volatile OS_CONST_PTR void* p;
  OS_U32                      v;
  OS_U8                       iRout;
} OS_TRACE_ENTRY;

/**********************************************************************
*
*       OS human readable object identifiers
*/
typedef struct OS_OBJNAME_STRUCT OS_OBJNAME;
struct OS_OBJNAME_STRUCT {
  OS_OBJNAME*             pNext;
  OS_CONST_PTR void*      pOSObjID;
  OS_ROM_DATA const char* sName;
};

/*********************************************************************
*
*       OS Counters
*
**********************************************************************
*/
typedef union {
  int Dummy;             // Make sure a full integer (32 bit on 32 bit CPUs) is used.
  struct {
    volatile OS_U8 Region;
    volatile OS_U8 DI;
  } Cnt;
} OS_COUNTERS;

#ifndef   PENDING_DUMMY_BYTES
  #define PENDING_DUMMY_BYTES OS_U8 aDummy[2];
#endif

typedef union {
  OS_U32 All;            // Make sure a full integer (32 bit on 32 bit CPUs) is used.
  struct {
    OS_U8 RoundRobin;
    OS_U8 TaskSwitch;
    PENDING_DUMMY_BYTES
  } Flag;
} OS_PENDING;

/*********************************************************************
*
*       Trace
*
**********************************************************************
*/
typedef struct {
  //
  // OS specific Trace Events
  //
  void  (*pfRecordEnterISR)          (void);
  void  (*pfRecordExitISR)           (void);
  void  (*pfRecordExitISRToScheduler)(void);
  void  (*pfRecordTaskInfo)          (const OS_TASK* pTask);
  void  (*pfRecordTaskCreate)        (OS_U32 TaskId);
  void  (*pfRecordTaskStartExec)     (OS_U32 TaskId);
  void  (*pfRecordTaskStopExec)      (void);
  void  (*pfRecordTaskStartReady)    (OS_U32 TaskId);
  void  (*pfRecordTaskStopReady)     (OS_U32 TaskId, unsigned int Reason);
  void  (*pfRecordIdle)              (void);
  //
  // Generic Trace Event logging (used by OS API)
  //
  void  (*pfRecordVoid)              (unsigned int Id);
  void  (*pfRecordU32)               (unsigned int Id, OS_U32 Para0);
  void  (*pfRecordU32x2)             (unsigned int Id, OS_U32 Para0, OS_U32 Para1);
  void  (*pfRecordU32x3)             (unsigned int Id, OS_U32 Para0, OS_U32 Para1, OS_U32 Para2);
  void  (*pfRecordU32x4)             (unsigned int Id, OS_U32 Para0, OS_U32 Para1, OS_U32 Para2, OS_U32 Para3);
  OS_U32(*pfPtrToId)                 (OS_U32 Ptr);
  //
  // Additional Trace Event logging
  //
  void  (*pfRecordEnterTimer)        (OS_U32 TimerID);
  void  (*pfRecordExitTimer)         (void);
  void  (*pfRecordEndCall)           (unsigned int Id);
  void  (*pfRecordEndCallU32)        (unsigned int Id, OS_U32 Para0);
  void  (*pfRecordTaskTerminate)     (OS_U32 TaskId);
  void  (*pfRecordU32x5)             (unsigned int Id, OS_U32 Para0, OS_U32 Para1, OS_U32 Para2, OS_U32 Para3, OS_U32 Para4);
  void  (*pfRecordObjName)           (OS_U32 Id, OS_CONST_PTR char* Para0);
} OS_TRACE_API;

/*********************************************************************
*
*        Cycle precise system time
*
**********************************************************************
*/
#define OS_TIMER_DOWNCOUNTING  (0u)
#define OS_TIMER_UPCOUNTING    (1u)

typedef struct {
  OS_U32       TimerFreq;                      // e.g. 48000000 for 48MHz
  OS_U32       IntFreq;                        // typ. 1000 for 1 KHz system tick
  OS_BOOL      IsUpCounter;                    // 0: Down counter. Interrupt on underflow. 1: Up counter, interrupt on compare
  unsigned int (*pfGetTimerCycles)    (void);  // Callback function for reading HW timer value
  unsigned int (*pfGetTimerIntPending)(void);  // Callback function for reading timer interrupt pending state
} OS_SYSTIMER_CONFIG;

typedef struct {
  OS_SYSTIMER_CONFIG Config;
  //
  // Computed (derived) values
  //
  OS_U32 CyclesPerInt;
  OS_U32 Cycle2usMul;
  OS_U32 Cycle2usDiv;
} OS_SYSTIMER_SETTINGS;

/*********************************************************************
*
*       OS_GLOBAL
*
**********************************************************************
*/
struct OS_GLOBAL_STRUCT {
  OS_COUNTERS                  Counters;
  OS_PENDING                   Pending;
  OS_TASK*                     pCurrentTask;   // Pointer to current Task
#if (OS_SUPPORT_INT_PRIORITY != 0)
  OS_IPL_DI_TYPE               Ipl_DI;
  OS_IPL_EI_TYPE               Ipl_EI;
#endif
  OS_TASK*                     pTask;          // Linked list of all Tasks
  OS_TASK volatile * volatile  pActiveTask;    // We really must have a volatile pointer to the volatile variable pActiveTask
#if (OS_SUPPORT_TIMER != 0)
  OS_TIMER*                    pTimer;         // Linked list of all active Timers
  OS_TIMER*                    pCurrentTimer;  // Actual expired timer which called callback
  OS_TIMER_HANDLER*            pfCheckTimer;   // Timer handler function, set when OS_TIMER_Start() is called
#endif // OS_SUPPORT_TIMER
  volatile OS_I32              Time;
  OS_TIME                      TimeDex;
#ifdef OS_U64
  OS_U64                       TickCnt;
#endif
#if (OS_RR_SUPPORTED != 0)
  OS_U8                        TimeSlice;
  OS_U8                        TimeSliceAtStart;
#endif
#if (OS_SUPPORT_TICKLESS != 0)
  OS_TIME                      TicklessFactor;
  OS_BOOL                      TicklessExpired;
  voidRoutine*                 pfEndTicklessMode;
#endif
#if (OS_PROFILE != 0)
  const OS_TRACE_API*          pTrace;
  OS_U8                        ProfilingOn;
#endif
#if ((OS_DEBUG != 0) || (OS_TRACE != 0))
  OS_U8                        InInt;          // Used in Debug builds of embOS only
#endif
#if OS_SUPPORT_MPU
  OS_MPU_MEM_CONFIG            MemConfig;      // RAM/ROM memory settings
#if (OS_DEBUG != 0)
  OS_MPU_DEBUG                 MPUDebug;       // MPU debug info
#endif
#endif
};

/*********************************************************************
*
*       Globals
*
**********************************************************************
*/

//
// Handle Definition (storage is actually allocted) versus Declaration
// (reference) of RTOS variables depending upon who includes this header file.
//
#ifdef OSGLOBAL_C
  #define OS_EXTERN         // Define variables if included by OS_Global.c
#else
  #define OS_EXTERN extern  // Declare variables if included by anyone else
#endif

//
// Global embOS variables
// Some embOS variables are placed with OS_SADDR in a near memory region which speeds up the read/write access.
//
extern    OS_SADDR OS_GLOBAL               OS_Global                        OS_RAM_SECTION_ATTRIBUTE;  // Major OS variables in structure OS_Global
OS_EXTERN          volatile OS_STATUS      OS_Status                        OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN OS_SADDR OS_BOOL                 OS_Running                       OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN OS_SADDR OS_U8                   OS_InitialSuspendCnt             OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN OS_SADDR OS_MUTEX*               OS_pMutexRoot                    OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN OS_SADDR OS_MEMPOOL*             OS_pMEMFRoot                     OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN OS_SADDR OS_TICK_HOOK*           OS_pTickHookRoot                 OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN OS_SADDR OS_MAIN_CONTEXT*        OS_pMainContext                  OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN OS_SADDR OS_WD*                  OS_pWDRoot                       OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN OS_SADDR volatile OS_INT         OS_CPU_Load                      OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN OS_SADDR volatile OS_I32         OS_IdleCnt                       OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN          voidRoutine*            OS_pfTaskStartHook               OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
#if (OS_TRACKNAME != 0)
OS_EXTERN          OS_OBJNAME*             OS_pObjNameRoot                  OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
#endif
#if (OS_SUPPORT_TICKSTEP != 0)
OS_EXTERN          volatile OS_U8          OS_TickStep                      OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN          volatile int            OS_TickStepTime                  OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
#endif
#if (OS_SUPPORT_SAVE_RESTORE_HOOK != 0)
OS_EXTERN OS_SADDR OS_EXTEND_TASK_CONTEXT* OS_pDefaultTaskContextExtension  OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
#endif
#if (OS_PROFILE != 0)
OS_EXTERN OS_SADDR OS_U32                  OS_TS_ExecStart                  OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
#endif
#if (OS_SUPPORT_PTLS != 0)
OS_EXTERN OS_SADDR OS_TLS_PTR              OS_pTLS                          OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required, Global pointer to thread local storage
#endif
#if (OS_CHECKSTACK == 2)
extern    OS_SADDR volatile OS_U8          OS_StackCheckLimit               OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
#endif
#if (OS_DEBUG != 0)
OS_EXTERN          OS_U8                   OS_InTimer                       OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN          OS_U8                   OS_InitCalled                    OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN          OS_QUEUE*               OS_pQRoot                        OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN          OS_MAILBOX*             OS_pMailboxRoot                  OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN          OS_SEMAPHORE*           OS_pSemaRoot                     OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
OS_EXTERN          OS_EVENT*               OS_pEventRoot                    OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required
#endif
OS_EXTERN          OS_SYSTIMER_SETTINGS    OS_SysTimer_Settings             OS_RAM_SECTION_ATTRIBUTE;  //lint !e9075 MISRA C:2012 Rule 8.4, required

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/

/*********************************************************************
*
*       Task related routines
*
**********************************************************************
*/

/*********************************************************************
*
*       OS_TASK_CREATE macro
*       OS_TASK_CREATEEX macro
*/
#if (OS_RR_SUPPORTED != 0)
  #define CTPARA_TIMESLICE ,2u
#else
  #define CTPARA_TIMESLICE
#endif

#if (OS_TRACKNAME != 0)
  #define OS_TASK_CREATE(pTask, pName, Priority, pRoutine, pStack) \
  OS_TASK_Create ((pTask),                                         \
                  (pName),                                         \
                  (OS_PRIO) (Priority),                            \
                  (pRoutine),                                      \
                  (void OS_STACKPTR*)(pStack),                     \
                  sizeof(pStack)                                   \
                  CTPARA_TIMESLICE                                 \
                )
#else
  #define OS_TASK_CREATE(pTask, pName, Priority, pRoutine, pStack) \
  OS_TASK_Create ((pTask),                                         \
                  (OS_PRIO) (Priority),                            \
                  (pRoutine),                                      \
                  (void OS_STACKPTR*)(pStack),                     \
                  sizeof(pStack)                                   \
                  CTPARA_TIMESLICE                                 \
                )
#endif

#if (OS_TRACKNAME != 0)
  #define OS_TASK_CREATEEX(pTask, pName, Priority, pRoutine, pStack, pContext) \
  OS_TASK_CreateEx  ((pTask),                                                  \
                     (pName),                                                  \
                     (Priority),                                               \
                     (pRoutine),                                               \
                     (void OS_STACKPTR*)(pStack),                              \
                     sizeof(pStack)                                            \
                     CTPARA_TIMESLICE,                                         \
                     (pContext)                                                \
                )
#else
  #define OS_TASK_CREATEEX(pTask, pName, Priority, pRoutine, pStack, pContext) \
  OS_TASK_CreateEx  ((pTask),                                                  \
                     (Priority),                                               \
                     (pRoutine),                                               \
                     (void OS_STACKPTR*)(pStack),                              \
                     sizeof(pStack)                                            \
                     CTPARA_TIMESLICE,                                         \
                     (pContext)                                                \
                )
#endif

/*********************************************************************
*
*       OS_TASK_Create
*       OS_TASK_CreateEx
*/
#if (OS_TRACKNAME != 0)
  #define OS_CREATE_TASK_PARA_NAME  OS_ROM_DATA const char* pName,
#else
  #define OS_CREATE_TASK_PARA_NAME
#endif

#if (OS_RR_SUPPORTED != 0)
  #define OS_CREATE_TASK_PARA_TS   ,OS_UINT TimeSlice
#else
  #define OS_CREATE_TASK_PARA_TS
#endif

void OS_TASK_Create  ( OS_TASK* pTask,
                       OS_CREATE_TASK_PARA_NAME
                       OS_PRIO Priority,
                       void (*pRoutine)(void),
                       void OS_STACKPTR* pStack,
                       OS_UINT StackSize
                       OS_CREATE_TASK_PARA_TS
                     ) OS_CODE_SECTION_ATTRIBUTE;

void OS_TASK_CreateEx( OS_TASK* pTask,
                       OS_CREATE_TASK_PARA_NAME
                       OS_PRIO Priority,
                       void (*pRoutine)(void* pVoid),
                       void OS_STACKPTR* pStack,
                       OS_UINT StackSize
                       OS_CREATE_TASK_PARA_TS,
                       void* pContext
                     ) OS_CODE_SECTION_ATTRIBUTE;

void     OS_TASK_AddTerminateHook          (OS_ON_TERMINATE_HOOK* pHook, OS_ON_TERMINATE_FUNC* pfUser)                                            OS_CODE_SECTION_ATTRIBUTE;
void     OS_TASK_RemoveAllTerminateHooks   (void)                                                                                                 OS_CODE_SECTION_ATTRIBUTE;
void     OS_TASK_RemoveTerminateHook       (OS_CONST_PTR OS_ON_TERMINATE_HOOK* pHook)                                                             OS_CODE_SECTION_ATTRIBUTE;
void     OS_TASK_Delay                     (OS_TIME t)                                                                                            OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_TASK_DelayUntil                (OS_TIME t)                                                                                            OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_TASK_Delayus                   (OS_U16  us)                                                                                           OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int      OS_TASK_GetNumTasks               (void)                                                                                                 OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_PRIO  OS_TASK_GetPriority               (OS_CONST_PTR OS_TASK* pTask)                                                                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U8    OS_TASK_GetSuspendCnt             (OS_CONST_PTR OS_TASK* pTask)                                                                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL  OS_TASK_IsTask                    (OS_CONST_PTR OS_TASK* pTask)                                                                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_TASK_Resume                    (OS_TASK* pTask)                                                                                       OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_TASK_ResumeAll                 (void)                                                                                                 OS_CODE_SECTION_ATTRIBUTE;
void     OS_TASK_SetDefaultStartHook       (voidRoutine* pfHook)                                                                                  OS_CODE_SECTION_ATTRIBUTE;
void     OS_TASK_SetName                   (OS_TASK* pTask, OS_ROM_DATA const char* s)                                                            OS_CODE_SECTION_ATTRIBUTE;
void     OS_TASK_SetPriority               (OS_TASK* pTask, OS_PRIO Priority)                                                                     OS_CODE_SECTION_ATTRIBUTE;
void     OS_TASK_SetInitialSuspendCnt      (OS_U8 SuspendCnt)                                                                                     OS_CODE_SECTION_ATTRIBUTE;
void     OS_TASK_Suspend                   (OS_TASK* pTask)                                                                                       OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_TASK_SuspendAll                (void)                                                                                                 OS_CODE_SECTION_ATTRIBUTE;
OS_TASK* OS_TASK_Index2Ptr                 (int TaskIndex)                                                                                        OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_TASK_Terminate                 (OS_TASK* pTask)                                                                                       OS_CODE_SECTION_ATTRIBUTE;
void     OS_TASK_Wake                      (OS_TASK* pTask)                                                                                       OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_TASK_Yield                     (void)                                                                                                 OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#if (OS_SUPPORT_SAVE_RESTORE_HOOK != 0)
void     OS_TASK_SetContextExtension       (OS_CONST_PTR OS_EXTEND_TASK_CONTEXT* pExtendContext)                                                  OS_CODE_SECTION_ATTRIBUTE;
void     OS_TASK_AddContextExtension       (OS_EXTEND_TASK_CONTEXT_LINK* pExtendContextLink, OS_CONST_PTR OS_EXTEND_TASK_CONTEXT* pExtendContext) OS_CODE_SECTION_ATTRIBUTE;
void     OS_TASK_SetDefaultContextExtension(OS_CONST_PTR OS_EXTEND_TASK_CONTEXT* pExtendContext)                                                  OS_CODE_SECTION_ATTRIBUTE;
#endif
#if (OS_RR_SUPPORTED != 0)
OS_U8    OS_TASK_SetTimeSlice              (OS_TASK* pTask, OS_U8 TimeSlice)                                                                      OS_CODE_SECTION_ATTRIBUTE;
OS_U8    OS_TASK_GetTimeSliceRem           (OS_CONST_PTR OS_TASK* pTask)                                                                          OS_CODE_SECTION_ATTRIBUTE;
#else
  #define OS_TASK_SetTimeSlice(pTask, TimeSlice)  (0u)
  #define OS_TASK_GetTimeSliceRem(pTask)          (0u)
#endif
#if (OS_TRACKNAME != 0)
OS_ROM_DATA const char* OS_TASK_GetName    (OS_CONST_PTR OS_TASK* pTask)                                                                          OS_CODE_SECTION_ATTRIBUTE;
#else
  #define OS_TASK_GetName(pt)  ("n/a")
#endif

#define OS_IsRunning()   OS_Running
#define OS_TASK_GetID()  OS_Global.pCurrentTask

#if (OS_SUSPEND_TASK_ON_TERMINATE != 0)  // Set task state of a terminated task to "suspended"
  #define OS_SUSPEND_TERMINATED_TASK(pTask) ((pTask)->Stat = 0x01)
#else
  #define OS_SUSPEND_TERMINATED_TASK(pTask)
#endif

/*********************************************************************
*
*       Software timer
*
**********************************************************************
*/
#if (OS_SUPPORT_TIMER != 0)

#if (OS_SIZEOF_INT == 2u)
  #define OS_TIMER_MAX_TIME  (0x7F00)
#elif (OS_SIZEOF_INT == 4u)
  #define OS_TIMER_MAX_TIME  (0x7FFFFF00)
#else
  #error "OS_TIMER_MAX_TIME not correctly defined"
#endif


void    OS_TIMER_Create            (OS_TIMER* pTimer, OS_TIMERROUTINE* Callback, OS_TIME Period) OS_CODE_SECTION_ATTRIBUTE;
void    OS_TIMER_Delete            (OS_TIMER* pTimer)                                            OS_CODE_SECTION_ATTRIBUTE;
OS_TIME OS_TIMER_GetPeriod         (OS_CONST_PTR OS_TIMER* pTimer)                               OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TIME OS_TIMER_GetRemainingPeriod(OS_CONST_PTR OS_TIMER* pTimer)                               OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL OS_TIMER_GetStatus         (OS_CONST_PTR OS_TIMER* pTimer)                               OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_TIMER_Restart           (OS_TIMER* pTimer)                                            OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_TIMER_SetPeriod         (OS_TIMER* pTimer, OS_TIME Period)                            OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_TIMER_Start             (OS_TIMER* pTimer)                                            OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_TIMER_Stop              (OS_TIMER* pTimer)                                            OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_TIMER_Trigger           (OS_TIMER* pTimer)                                            OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

#define OS_TIMER_CREATE(pTimer, c, d)        \
        OS_TIMER_Create((pTimer), (c), (d)); \
        OS_TIMER_Start(pTimer);

#define OS_TIMER_GetCurrent()       OS_Global.pCurrentTimer

/*********************************************************************
*
*       Extended timer
*
**********************************************************************
*/
void OS_TIMER_CreateEx (OS_TIMER_EX* pTimerEx, OS_TIMER_EX_ROUTINE* Callback, OS_TIME Period, void* pData) OS_CODE_SECTION_ATTRIBUTE;

#define OS_TIMER_DeleteEx(pTimerEx)              OS_TIMER_Delete(&(pTimerEx)->Timer)
#define OS_TIMER_GetPeriodEx(pTimerEx)           OS_TIMER_GetPeriod(&(pTimerEx)->Timer)
#define OS_TIMER_GetRemainingPeriodEx(pTimerEx)  OS_TIMER_GetRemainingPeriod(&(pTimerEx)->Timer)
#define OS_TIMER_GetStatusEx(pTimerEx)           OS_TIMER_GetStatus(&(pTimerEx)->Timer)
#define OS_TIMER_RestartEx(pTimerEx)             OS_TIMER_Restart(&(pTimerEx)->Timer)
#define OS_TIMER_SetPeriodEx(pTimerEx,Period)    OS_TIMER_SetPeriod(&(pTimerEx)->Timer, (Period))
#define OS_TIMER_StartEx(pTimerEx)               OS_TIMER_Start(&(pTimerEx)->Timer)
#define OS_TIMER_StopEx(pTimerEx)                OS_TIMER_Stop(&(pTimerEx)->Timer)
#define OS_TIMER_TriggerEx(pTimerEx)             OS_TIMER_Trigger(&(pTimerEx)->Timer)

#define OS_TIMER_CREATEEX(pTimerEx, cb, Period, pData)          \
        OS_TIMER_CreateEx((pTimerEx), (cb), (Period), (pData)); \
        OS_TIMER_StartEx(pTimerEx)

#define OS_TIMER_GetCurrentEx()                  ((OS_TIMER_EX*)OS_Global.pCurrentTimer)

#endif // OS_SUPPORT_TIMER

/*********************************************************************
*
*       Mutex
*
**********************************************************************
*/
void     OS_MUTEX_Create     (OS_MUTEX* pMutex)                  OS_CODE_SECTION_ATTRIBUTE;
void     OS_MUTEX_Delete     (OS_MUTEX* pMutex)                  OS_CODE_SECTION_ATTRIBUTE;
OS_BOOL  OS_MUTEX_IsMutex    (OS_CONST_PTR OS_MUTEX* pMutex)     OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int      OS_MUTEX_GetValue   (OS_CONST_PTR OS_MUTEX* pMutex)     OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASK* OS_MUTEX_GetOwner   (OS_CONST_PTR OS_MUTEX* pMutex)     OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char     OS_MUTEX_Lock       (OS_MUTEX* pMutex)                  OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_MUTEX_Unlock     (OS_MUTEX* pMutex)                  OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int      OS_MUTEX_LockBlocked(OS_MUTEX* pMutex)                  OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int      OS_MUTEX_LockTimed  (OS_MUTEX* pMutex, OS_TIME Timeout) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

/*********************************************************************
*
*       Semaphore
*
**********************************************************************
*/
void    OS_SEMAPHORE_Create     (OS_SEMAPHORE* pSema, OS_UINT InitValue) OS_CODE_SECTION_ATTRIBUTE;
OS_BOOL OS_SEMAPHORE_Take       (OS_SEMAPHORE* pSema)                    OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_SEMAPHORE_Delete     (OS_SEMAPHORE* pSema)                    OS_CODE_SECTION_ATTRIBUTE;
int     OS_SEMAPHORE_GetValue   (OS_CONST_PTR OS_SEMAPHORE* pSema)       OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U8   OS_SEMAPHORE_SetValue   (OS_SEMAPHORE* pSema, OS_UINT Value)     OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_SEMAPHORE_Give       (OS_SEMAPHORE* pSema)                    OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_SEMAPHORE_GiveMax    (OS_SEMAPHORE* pSema, OS_UINT MaxValue)  OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_SEMAPHORE_TakeBlocked(OS_SEMAPHORE* pSema)                    OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL OS_SEMAPHORE_TakeTimed  (OS_SEMAPHORE* pSema, OS_TIME Timeout)   OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;


#define OS_SEMAPHORE_CREATE(ps) OS_SEMAPHORE_Create((ps), 0)

/*********************************************************************
*
*       Mailbox
*
**********************************************************************
*/
void    OS_MAILBOX_Clear           (OS_MAILBOX* pMB)                                                    OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_MAILBOX_Create          (OS_MAILBOX* pMB, OS_U16 sizeofMsg, OS_UINT maxnofMsg, void* Buffer) OS_CODE_SECTION_ATTRIBUTE;
void    OS_MAILBOX_Delete          (OS_MAILBOX* pMB)                                                    OS_CODE_SECTION_ATTRIBUTE;
char    OS_MAILBOX_Get             (OS_MAILBOX* pMB, void* pDest)                                       OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_MAILBOX_Get1            (OS_MAILBOX* pMB, char* pDest)                                       OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_MAILBOX_GetBlocked      (OS_MAILBOX* pMB, void* pDest)                                       OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_MAILBOX_GetBlocked1     (OS_MAILBOX* pMB, char* pDest)                                       OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_MAILBOX_GetTimed        (OS_MAILBOX* pMB, void* pDest, OS_TIME Timeout)                      OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_MAILBOX_GetTimed1       (OS_MAILBOX* pMB, char* pDest, OS_TIME Timeout)                      OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_MAILBOX_GetPtr          (OS_MAILBOX* pMB, void** ppDest)                                     OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_MAILBOX_GetPtrBlocked   (OS_MAILBOX* pMB, void** ppDest)                                     OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_MAILBOX_Peek            (OS_CONST_PTR OS_MAILBOX* pMB, void* pDest)                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_MAILBOX_Purge           (OS_MAILBOX* pMB)                                                    OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_MAILBOX_Put             (OS_MAILBOX* pMB, OS_CONST_PTR void* pMail)                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_MAILBOX_Put1            (OS_MAILBOX* pMB, OS_CONST_PTR char* pMail)                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_MAILBOX_PutBlocked      (OS_MAILBOX* pMB, OS_CONST_PTR void* pMail)                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_MAILBOX_PutBlocked1     (OS_MAILBOX* pMB, OS_CONST_PTR char* pMail)                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_MAILBOX_PutFront        (OS_MAILBOX* pMB, OS_CONST_PTR void* pMail)                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_MAILBOX_PutFront1       (OS_MAILBOX* pMB, OS_CONST_PTR char* pMail)                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_MAILBOX_PutFrontBlocked (OS_MAILBOX* pMB, OS_CONST_PTR void* pMail)                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_MAILBOX_PutFrontBlocked1(OS_MAILBOX* pMB, OS_CONST_PTR char* pMail)                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL OS_MAILBOX_PutTimed        (OS_MAILBOX* pMB, OS_CONST_PTR void* pMail, OS_TIME Timeout)         OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL OS_MAILBOX_PutTimed1       (OS_MAILBOX* pMB, OS_CONST_PTR char* pMail, OS_TIME Timeout)         OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_MAILBOX_WaitBlocked     (OS_MAILBOX* pMB)                                                    OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_MAILBOX_WaitTimed       (OS_MAILBOX* pMB, OS_TIME Timeout)                                   OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#if ((OS_DEBUG == 0) && (OS_SUPPORT_MPU == 0))
  #define OS_MAILBOX_GetMessageCnt(pMB) (*(pMB)).nofMsg
#else
OS_UINT OS_MAILBOX_GetMessageCnt   (const OS_MAILBOX* pMB)                                              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#endif

/*********************************************************************
*
*       Message Queue
*
**********************************************************************
*/
void    OS_QUEUE_Clear         (OS_QUEUE* pQ)                                                                           OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_QUEUE_Create        (OS_QUEUE* pQ, void* pData, OS_UINT Size)                                                OS_CODE_SECTION_ATTRIBUTE;
void    OS_QUEUE_Delete        (OS_QUEUE* pQ)                                                                           OS_CODE_SECTION_ATTRIBUTE;
int     OS_QUEUE_GetMessageCnt (OS_CONST_PTR OS_QUEUE* pQ)                                                              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_QUEUE_Put           (OS_QUEUE* pQ, OS_CONST_PTR void* pSrc, OS_UINT Size)                                    OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_QUEUE_PutBlocked    (OS_QUEUE* pQ, OS_CONST_PTR void* pSrc, OS_UINT Size)                                    OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_QUEUE_PutTimed      (OS_QUEUE* pQ, OS_CONST_PTR void* pSrc, OS_UINT Size, OS_TIME Timeout)                   OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_QUEUE_PutEx         (OS_QUEUE* pQ, OS_CONST_PTR OS_QUEUE_SRCLIST* pSrcList, OS_UINT NumSrc)                  OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_QUEUE_PutBlockedEx  (OS_QUEUE* pQ, OS_CONST_PTR OS_QUEUE_SRCLIST* pSrcList, OS_UINT NumSrc)                  OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_QUEUE_PutTimedEx    (OS_QUEUE* pQ, OS_CONST_PTR OS_QUEUE_SRCLIST* pSrcList, OS_UINT NumSrc, OS_TIME Timeout) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_QUEUE_GetPtrBlocked (OS_QUEUE* pQ, void** ppData)                                                            OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_QUEUE_GetPtr        (OS_QUEUE* pQ, void** ppData)                                                            OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_QUEUE_GetPtrTimed   (OS_QUEUE* pQ, void** ppData, OS_TIME Timeout)                                           OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_QUEUE_Purge         (OS_QUEUE* pQ)                                                                           OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL OS_QUEUE_IsInUse       (OS_CONST_PTR OS_QUEUE* pQ)                                                              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_QUEUE_GetMessageSize(OS_CONST_PTR OS_QUEUE* pQ)                                                              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_QUEUE_PeekPtr       (OS_CONST_PTR OS_QUEUE* pQ, void** ppData)                                               OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

#define OS_Q_SIZEOF_HEADER     (sizeof(OS_INT))

/*********************************************************************
*
*       Task event
*
**********************************************************************
*/
OS_TASKEVENT OS_TASKEVENT_Clear           (OS_TASK* pTask)                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASKEVENT OS_TASKEVENT_ClearEx         (OS_TASK* pTask, OS_TASKEVENT EventMask)  OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASKEVENT OS_TASKEVENT_Get             (OS_CONST_PTR OS_TASK* pTask)             OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void         OS_TASKEVENT_Set             (OS_TASK* pTask, OS_TASKEVENT Event)      OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASKEVENT OS_TASKEVENT_GetBlocked      (OS_TASKEVENT EventMask)                  OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASKEVENT OS_TASKEVENT_GetTimed        (OS_TASKEVENT EventMask, OS_TIME Timeout) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASKEVENT OS_TASKEVENT_GetSingleBlocked(OS_TASKEVENT EventMask)                  OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASKEVENT OS_TASKEVENT_GetSingleTimed  (OS_TASKEVENT EventMask, OS_TIME Timeout) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

/*********************************************************************
*
*       Event object
*
**********************************************************************
*/
void                OS_EVENT_Create        (OS_EVENT* pEvent)                                           OS_CODE_SECTION_ATTRIBUTE;
void                OS_EVENT_CreateEx      (OS_EVENT* pEvent, unsigned int Mode)                        OS_CODE_SECTION_ATTRIBUTE;
void                OS_EVENT_Delete        (OS_EVENT* pEvent)                                           OS_CODE_SECTION_ATTRIBUTE;
OS_BOOL             OS_EVENT_Get           (OS_CONST_PTR OS_EVENT* pEvent)                              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                OS_EVENT_GetBlocked    (OS_EVENT* pEvent)                                           OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASKEVENT        OS_EVENT_GetMask       (OS_EVENT* pEvent, OS_TASKEVENT EventMask)                   OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASKEVENT        OS_EVENT_GetMaskBlocked(OS_EVENT* pEvent, OS_TASKEVENT EventMask)                   OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_EVENT_MASK_MODE  OS_EVENT_GetMaskMode   (OS_CONST_PTR OS_EVENT* pEvent)                              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASKEVENT        OS_EVENT_GetMaskTimed  (OS_EVENT* pEvent, OS_TASKEVENT EventMask, OS_TIME Timeout)  OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_EVENT_RESET_MODE OS_EVENT_GetResetMode  (OS_CONST_PTR OS_EVENT* pEvent)                              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char                OS_EVENT_GetTimed      (OS_EVENT* pEvent, OS_TIME Timeout)                          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                OS_EVENT_Pulse         (OS_EVENT* pEvent)                                           OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                OS_EVENT_Reset         (OS_EVENT* pEvent)                                           OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                OS_EVENT_Set           (OS_EVENT* pEvent)                                           OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                OS_EVENT_SetMask       (OS_EVENT* pEvent, OS_TASKEVENT EventMask)                   OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                OS_EVENT_SetMaskMode   (OS_EVENT* pEvent, OS_EVENT_MASK_MODE MaskMode)              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                OS_EVENT_SetResetMode  (OS_EVENT* pEvent, OS_EVENT_RESET_MODE ResetMode)            OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

/*********************************************************************
*
*       Heap type memory management
*
**********************************************************************
*/
#if (OS_SUPPORT_OS_ALLOC != 0)
void* OS_HEAP_malloc (unsigned int Size)                     OS_CODE_SECTION_ATTRIBUTE;
void  OS_HEAP_free   (void* pMemBlock)                       OS_CODE_SECTION_ATTRIBUTE;
void* OS_HEAP_realloc(void* pMemBlock, unsigned int NewSize) OS_CODE_SECTION_ATTRIBUTE;
#endif

/*********************************************************************
*
*       Fixed Block memory management
*
**********************************************************************
*/
void*   OS_MEMPOOL_Alloc           (OS_MEMPOOL* pMEMF)                                                    OS_CODE_SECTION_ATTRIBUTE;
void*   OS_MEMPOOL_AllocBlocked    (OS_MEMPOOL* pMEMF)                                                    OS_CODE_SECTION_ATTRIBUTE;
void*   OS_MEMPOOL_AllocTimed      (OS_MEMPOOL* pMEMF, OS_TIME Timeout)                                   OS_CODE_SECTION_ATTRIBUTE;
void    OS_MEMPOOL_Create          (OS_MEMPOOL* pMEMF, void* pPool, OS_UINT NumBlocks, OS_UINT BlockSize) OS_CODE_SECTION_ATTRIBUTE;
void    OS_MEMPOOL_Delete          (OS_MEMPOOL* pMEMF)                                                    OS_CODE_SECTION_ATTRIBUTE;
void    OS_MEMPOOL_Free            (void* pMemBlock)                                                      OS_CODE_SECTION_ATTRIBUTE;
void    OS_MEMPOOL_FreeEx          (OS_MEMPOOL* pMEMF, void* pMemBlock)                                   OS_CODE_SECTION_ATTRIBUTE;
int     OS_MEMPOOL_GetNumFreeBlocks(OS_CONST_PTR OS_MEMPOOL* pMEMF)                                       OS_CODE_SECTION_ATTRIBUTE;
int     OS_MEMPOOL_GetBlockSize    (OS_CONST_PTR OS_MEMPOOL* pMEMF)                                       OS_CODE_SECTION_ATTRIBUTE;
int     OS_MEMPOOL_GetMaxUsed      (OS_CONST_PTR OS_MEMPOOL* pMEMF)                                       OS_CODE_SECTION_ATTRIBUTE;
int     OS_MEMPOOL_GetNumBlocks    (OS_CONST_PTR OS_MEMPOOL* pMEMF)                                       OS_CODE_SECTION_ATTRIBUTE;
OS_BOOL OS_MEMPOOL_IsInPool        (OS_CONST_PTR OS_MEMPOOL* pMEMF, OS_CONST_PTR void* pMemBlock)         OS_CODE_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Stack check
*
**********************************************************************
*/
#if (OS_CHECKSTACK != 0)
  unsigned int      OS_STACK_GetTaskStackSpace(OS_CONST_PTR OS_TASK* pTask) OS_CODE_SECTION_ATTRIBUTE;
  unsigned int      OS_STACK_GetTaskStackUsed (OS_CONST_PTR OS_TASK* pTask) OS_CODE_SECTION_ATTRIBUTE;
  unsigned int      OS_STACK_GetTaskStackSize (OS_CONST_PTR OS_TASK* pTask) OS_CODE_SECTION_ATTRIBUTE;
  void OS_STACKPTR* OS_STACK_GetTaskStackBase (OS_CONST_PTR OS_TASK* pTask) OS_CODE_SECTION_ATTRIBUTE;
#else
  #define OS_STACK_GetTaskStackSpace(pt)  (0u)
  #define OS_STACK_GetTaskStackUsed(pt)   (0u)
  #define OS_STACK_GetTaskStackSize(pt)   (0u)
  #define OS_STACK_GetTaskStackBase(pt)   (0u)
#endif

#if (OS_SUPPORT_SYSSTACK_INFO != 0)
  void OS_STACKPTR* OS_STACK_GetSysStackBase (void) OS_CODE_SECTION_ATTRIBUTE;
  unsigned int      OS_STACK_GetSysStackSize (void) OS_CODE_SECTION_ATTRIBUTE;
#else
  #define OS_STACK_GetSysStackBase()  (0)
  #define OS_STACK_GetSysStackSize()  (0u)
#endif

#if ((OS_CHECKSTACK != 0) && (OS_SUPPORT_SYSSTACK_INFO != 0))
  unsigned int OS_STACK_GetSysStackSpace(void) OS_CODE_SECTION_ATTRIBUTE;
  unsigned int OS_STACK_GetSysStackUsed (void) OS_CODE_SECTION_ATTRIBUTE;
#else
  #define OS_STACK_GetSysStackSpace()  (0u)
  #define OS_STACK_GetSysStackUsed()   (0u)
#endif

#if (OS_SUPPORT_INTSTACK_INFO != 0)
  void OS_STACKPTR* OS_STACK_GetIntStackBase(void) OS_CODE_SECTION_ATTRIBUTE;
  unsigned int      OS_STACK_GetIntStackSize(void) OS_CODE_SECTION_ATTRIBUTE;
#else
  #define OS_STACK_GetIntStackBase()   (0)
  #define OS_STACK_GetIntStackSize()   (0u)
#endif

#if ((OS_CHECKSTACK != 0) && (OS_SUPPORT_INTSTACK_INFO != 0))
  unsigned int OS_STACK_GetIntStackSpace(void) OS_CODE_SECTION_ATTRIBUTE;
  unsigned int OS_STACK_GetIntStackUsed (void) OS_CODE_SECTION_ATTRIBUTE;
#else
  #define OS_STACK_GetIntStackSpace()  (0u)
  #define OS_STACK_GetIntStackUsed()   (0u)
#endif

#if (OS_CHECKSTACK == 2)
  void  OS_STACK_SetCheckLimit(OS_U8 Limit);
  OS_U8 OS_STACK_GetCheckLimit(void);
#endif

/*********************************************************************
*
*       Interrupt save/disable/restore macros
*
**********************************************************************
*/
#define OS_INT_IncDI()  { OS_ASSERT_DICNT(); OS_INT_Disable(); OS_Global.Counters.Cnt.DI++; }
#define OS_INT_DecRI()  { OS_ASSERT_DICNT();                                                        \
                           OS_Global.Counters.Cnt.DI--;                                             \
                           if (OS_Global.Counters.Cnt.DI == 0u) {  /*lint !e548 No else required*/  \
                             OS_INT_Enable();                                                       \
                           }                                                                        \
                         }

#define OS_RESTORE_I()   { OS_ASSERT_DICNT();                                                       \
                           if (OS_Global.Counters.Cnt.DI == 0u) {  /*lint !e548 No else required*/  \
                             OS_INT_Enable();                                                       \
                           }                                                                        \
                         }

void    OS_INT_EnableConditional(void) OS_CODE_SECTION_ATTRIBUTE;
OS_BOOL OS_INT_InInterrupt      (void) OS_CODE_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Global interrupt save/disable/restore macros
*
*  Some OS_INT_ functions are declared as MACRO.
*  If not defined in OSCHIP.h, these macros are translated
*  to a function call and the function is declared here.
*  The functions have to be implemented in the CPU specific
*  sources OS_Priv.c or specific assembly file.
*  The macros may be overwritten in OSCHIP.h to be expanded
*  to intrinsic calls if applicable. This is a very
*  efficient implementation.
*
**********************************************************************
*/
#ifndef OS_INT_DisableAll
  OS_INTERWORK void OS_INT_DisableAllFunc(void) OS_CODE_SECTION_ATTRIBUTE;
  #define OS_INT_DisableAll() OS_INT_DisableAllFunc()
#endif

#ifndef OS_INT_EnableAll
  OS_INTERWORK void OS_INT_EnableAllFunc(void) OS_CODE_SECTION_ATTRIBUTE;
  #define OS_INT_EnableAll() OS_INT_EnableAllFunc()
#endif

#ifndef OS_INT_PreserveAll
  OS_INTERWORK void OS_INT_PreserveAllFunc(OS_U32 *pSave) OS_CODE_SECTION_ATTRIBUTE;
  #define OS_INT_PreserveAll(p) OS_INT_PreserveAllFunc((p))
#endif

#ifndef OS_INT_RestoreAll
  OS_INTERWORK void OS_INT_RestoreAllFunc(OS_U32 *pSave) OS_CODE_SECTION_ATTRIBUTE;
  #define OS_INT_RestoreAll(p) OS_INT_RestoreAllFunc((p))
#endif

#ifndef OS_INT_PreserveAndDisableAll
  OS_INTERWORK void OS_INT_PreserveAndDisableAllFunc(OS_U32 *pSave) OS_CODE_SECTION_ATTRIBUTE;
  #define OS_INT_PreserveAndDisableAll(p) OS_INT_PreserveAndDisableAllFunc((p))
#endif

/*********************************************************************
*
*       Interrupt state preserve/restore macros
*
*  These macros preserve and restore the embOS interrupt status.
*  It can be used to temporarily disabled embOS interrupts without
*  changing the interrupt status which means that embOS interrupts
*  are again disabled when they were disabled before.
*  The actual macro is defined in OSCHIP.h.
*  For example:
*  OS_U32 p;
*  OS_INT_Preserve(&p);
*  OS_INT_Disable();
*  DoSomething();
*  OS_INT_Restore(&p);
*/
#ifndef OS_INT_Preserve
  OS_INTERWORK void OS_INT_PreserveFunc(OS_U32* p) OS_CODE_SECTION_ATTRIBUTE;
  #define OS_INT_Preserve(p) OS_INT_PreserveFunc(p)
#endif

#ifndef OS_INT_Restore
  OS_INTERWORK void OS_INT_RestoreFunc(OS_U32* p) OS_CODE_SECTION_ATTRIBUTE;
  #define OS_INT_Restore(p) OS_INT_RestoreFunc(p)
#endif

/*********************************************************************
*
*       ISR (Interrupt service routine) support
*
**********************************************************************
*/
#if (OS_DEBUG != 0)
  #ifndef   OS_MARK_IN_ISR
    #define OS_MARK_IN_ISR()          { OS_Global.InInt++; }
  #endif
  #define OS_MARK_OUTOF_ISR()         { if (OS_Global.InInt == 0u) {OS_Error(OS_ERR_LEAVEINT);} OS_Global.InInt--; }
  #define OS_MARK_OUTOF_ISR_SWITCH()  { if (OS_Global.InInt == 0u) {OS_Error(OS_ERR_LEAVEINT);} OS_Global.InInt--; }
#else
  #ifndef   OS_MARK_IN_ISR
    #define OS_MARK_IN_ISR()
  #endif
  #define OS_MARK_OUTOF_ISR()         { }
  #define OS_MARK_OUTOF_ISR_SWITCH()
#endif

#if (OS_SUPPORT_CALL_ISR != 0)      // Not allowed for some CPUs
void OS_INT_Call        (void (*pRoutine)(void)) OS_CODE_SECTION_ATTRIBUTE;
void OS_INT_CallNestable(void (*pRoutine)(void)) OS_CODE_SECTION_ATTRIBUTE;
#endif

#if (OS_SUPPORT_ENTER_INTERRUPT == 0) || ((OS_SWITCH_FROM_INT_MODIFIES_STACK == 0) && (OS_INTERRUPTS_ARE_NESTABLE_ON_ENTRY != 0) && (OS_SCHEDULER_ACTIVATED_BY_EXCEPTION == 0))
  //
  // For CPUs without separate interrupt stack which do not disable interrupts on entry,
  // OS_Enter- / Leave- Interrupt() is not defined.
  // OS_INT_Call() has to be used
  //
#else

  #ifndef   OS_ENABLE_INTS_SAVE_IPL
    #define OS_ENABLE_INTS_SAVE_IPL() OS_INT_Enable()
  #endif

  #ifndef   OS_RESTORE_IPL
    #define OS_RESTORE_IPL()
  #endif

  #if (OS_INTERRUPTS_ARE_NESTABLE_ON_ENTRY != 0)
    #define OS_DI_ON_ENTRY() OS_INT_Disable()
  #else
    #define OS_DI_ON_ENTRY()
  #endif

  #ifndef   OS_EI_HP_ON_ENTRY
    #define OS_EI_HP_ON_ENTRY()      // Enable high-prio interrupts if necessary: Some CPUs (such as M16C) disable all interrupts on ISR entry. We need to enable high prio Ints, using the global Int-Enable
  #endif

  #ifndef   OS_EI_ON_LEAVE
    #define OS_EI_ON_LEAVE()         // Required for CPUs which do not restore DI-flag by RETI. Currently only CM3.
  #endif

  #if (OS_SWITCH_FROM_INT_MODIFIES_STACK != 0)
    #define OS_HANDLE_REGION_CNT_ON_LI() \
        OS_Global.Counters.Cnt.Region--; \
      }
  #else
    #define OS_HANDLE_REGION_CNT_ON_LI() \
      } OS_Global.Counters.Cnt.Region--;
  #endif

  #ifndef   OS_DI_ON_LEAVE_NESTABLE
    #define OS_DI_ON_LEAVE_NESTABLE() OS_INT_Disable()
  #endif

  #ifndef   OS_EI_ON_LEAVE_NESTABLE
    #define OS_EI_ON_LEAVE_NESTABLE() OS_EI_ON_LEAVE()
  #endif

  #define OS_SWITCH_FROM_INT_IF_REQUIRED()                                                \
    if ((OS_Global.Counters.Cnt.Region == (OS_U8) 1u) && (OS_Global.Pending.All != 0u)) { \
      OS_MARK_OUTOF_ISR_SWITCH();                                                         \
      OS_SwitchFromInt();                                                                 \
    } else {                                                                              \
      OS_MARK_OUTOF_ISR();

  #if (OS_PROFILE != 0)
    #define OS_TRACE_ISR_EXIT()                                                            \
      if ((OS_Global.Counters.Cnt.Region == (OS_U8)1u) && (OS_Global.Pending.All != 0u)) { \
        TRACE_ON_ISR_EXIT_TO_SCHEDULER();                                                  \
      } else {                                                                             \
        TRACE_ON_ISR_EXIT();                                                               \
      }
  #else
    #define OS_TRACE_ISR_EXIT()
  #endif

  #define OS_INT_Enter() {           \
    OS_MARK_IN_ISR();                \
    OS_ASSERT_CPU_IN_ISR_MODE();     \
    OS_DI_ON_ENTRY();         /* Disable low-prio interrupts if necessary: On CPUs, that keep interrupts enabled on ISR entyr. We disable all further low-prio interrupts. */                             \
    OS_EI_HP_ON_ENTRY();      /* Enable high-prio interrupts if necessary: Some CPUs (such as M16C) disable all interrupts on ISR entry. We need to enable high prio Ints, using the global Int-Enable */ \
    OS_Global.Counters.Cnt.Region++; \
    OS_Global.Counters.Cnt.DI++;     \
    TRACE_ON_ISR_ENTER();            \
  }

  #define OS_INT_Leave() {                                                                       \
    OS_TRACE_ISR_EXIT();                                                                         \
    OS_Global.Counters.Cnt.DI--; /* Must have been zero initially ! (We could put =0 instead) */ \
    OS_SWITCH_FROM_INT_IF_REQUIRED()                                                             \
    OS_HANDLE_REGION_CNT_ON_LI();                                                                \
    OS_EI_ON_LEAVE();                                                                            \
  }

  #define OS_INT_EnterNestable() { \
    OS_MARK_IN_ISR();              \
    OS_IntEnterRegion();           \
    OS_ENABLE_INTS_SAVE_IPL();     \
    OS_ASSERT_CPU_IN_ISR_MODE();   \
    TRACE_ON_ISR_ENTER();          \
  }

  #define OS_INT_LeaveNestable() {   \
    OS_TRACE_ISR_EXIT();             \
    OS_DI_ON_LEAVE_NESTABLE();       \
    OS_SWITCH_FROM_INT_IF_REQUIRED() \
    OS_HANDLE_REGION_CNT_ON_LI();    \
    OS_RESTORE_IPL();                \
    OS_EI_ON_LEAVE_NESTABLE();       \
  }


//
// OS_LeaveInterruptNoSwitch() and OS_LeaveNestableInterruptNoSwitch() are obsolete but kept here for compatibility reason
//
  #define OS_LeaveInterruptNoSwitch() { \
    OS_MARK_OUTOF_ISR();                \
    OS_Global.Counters.Cnt.DI--;        \
    OS_Global.Counters.Cnt.Region--;    \
    OS_EI_ON_LEAVE();                   \
  }

  #define OS_LeaveNestableInterruptNoSwitch() { \
    OS_INT_Disable();                           \
    OS_MARK_OUTOF_ISR();                        \
    OS_Global.Counters.Cnt.Region--;            \
    OS_RESTORE_IPL();                           \
    OS_EI_ON_LEAVE_NESTABLE();                  \
  }
#endif

#ifndef OS_INT_EnterIntStack
  void OS__EnterIntStack(void) OS_CODE_SECTION_ATTRIBUTE;
  #define OS_INT_EnterIntStack() {OS_INT_Disable(); OS__EnterIntStack(); OS_RESTORE_I(); }
#endif

#ifndef OS_INT_LeaveIntStack
  void OS__LeaveIntStack(void) OS_CODE_SECTION_ATTRIBUTE;
  #define OS_INT_LeaveIntStack() {OS_INT_Disable(); OS__LeaveIntStack(); }
#endif

void OS_INT_SetPriorityThreshold(OS_UINT Priority) OS_CODE_SECTION_ATTRIBUTE;

#if (OS_DEBUG != 0)
  void OS_AssertCPUInISRMode(void) OS_CODE_SECTION_ATTRIBUTE;
#endif

/*********************************************************************
*
*       Critical regions
*
**********************************************************************
*/

//
// For some compiler it is necessary to use an OS_TASK_EnterRegion() function
// instead of the macro. To do so please add the following line to OSCHIP.h:
// #define OS_TASK_EnterRegion OS_EnterRegionFunc
//
#ifndef OS_TASK_EnterRegion
  #if (OS_DEBUG > 1)
    #define OS_TASK_EnterRegion() {if (OS_Global.Counters.Cnt.Region == 0xFFu) OS_Error(OS_ERR_REGIONCNT); else OS_Global.Counters.Cnt.Region++; }
  #else
    #define OS_TASK_EnterRegion() {OS_Global.Counters.Cnt.Region++; }
  #endif
#endif

//
// For some compiler (e.g. IAR EWRX with optimization) it is necessary to use an OS_EnterRegionFunc() function
// instead of the macro in OS_INT_EnterNestable(). To do so please add the following line to OSCHIP.h:
// #define OS_IntEnterRegion OS_EnterRegionFunc
//
#ifndef   OS_IntEnterRegion
  #define OS_IntEnterRegion() {OS_Global.Counters.Cnt.Region++;}
#endif

void OS_EnterRegionFunc (void) OS_CODE_SECTION_ATTRIBUTE;
void OS_TASK_LeaveRegion(void) OS_CODE_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Timing support
*
**********************************************************************
*/
#define OS_TIMING OS_U32
void    OS_TIME_StartMeasurement (OS_TIMING* pCycle)              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_TIME_StopMeasurement  (OS_TIMING* pCycle)              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U32  OS_TIME_GetResultus      (OS_CONST_PTR OS_TIMING* pCycle) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#define OS_TIME_GetResult(pPara) (*(pPara))

//
// With these defines it is possible to use port specific timing routines.
// But in most cases these default implementations will simply work.
// In order to use them it must just be ensured OS_TIME_ConfigSysTimer()
// is called and the according callback functions are implemented in the BSP.
// Also a 64 bit (long long) data type must be available.
// With your own OS_TIME_GETCYCLES() implementation please ensure
// interrupts are disabled since the embOS source code does not disable interrupts
// any more. The default OS__TIME_GetCycles() disables interrupts correctly.
//
#ifndef   OS_TIME_GETCYCLES
  #define OS_TIME_GETCYCLES                 OS__TIME_GetCycles
#endif
#ifndef   OS_TIME_CONVERTCYCLES2NS
  #define OS_TIME_CONVERTCYCLES2NS(Cycles)  OS__TIME_ConvertCycles2ns(Cycles)
#endif
#ifndef   OS_TIME_CONVERTCYCLES2US
  #define OS_TIME_CONVERTCYCLES2US(Cycles)  OS__TIME_ConvertCycles2us(Cycles)
#endif
#ifndef   OS_TIME_CONVERTNS2CYCLES
  #define OS_TIME_CONVERTNS2CYCLES(ns)      OS__TIME_Convertns2Cycles(ns)
#endif
#ifndef   OS_TIME_CONVERTUS2CYCLES
  #define OS_TIME_CONVERTUS2CYCLES(us)      OS__TIME_Convertus2Cycles(us)
#endif

//
// Timing routines. Their existence depends on the CPU. In general,
// 8-bit CPUs require both routines, where 16-bit CPUs require one
// and 32-bit CPUs require none of these.
//
#ifndef OS_TIME_GetTicks
  OS_TIME OS_TIME_GetTicks(void) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#endif
#ifndef OS_TIME_GetTicks32
  #if (OS_SIZEOF_INT == 4)
    #define OS_TIME_GetTicks32() (OS_Global.Time)
  #else
    OS_I32 OS_TIME_GetTicks32(void) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
  #endif
#endif

void   OS_TIME_ConfigSysTimer  (const OS_SYSTIMER_CONFIG* pConfig) OS_CODE_SECTION_ATTRIBUTE;
#ifdef OS_U64
OS_U64 OS_TIME_ConvertCycles2ns(OS_U32 Cycles)                     OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U64 OS_TIME_ConvertCycles2us(OS_U32 Cycles)                     OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U64 OS_TIME_Convertns2Cycles(OS_U32 ns)                         OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U64 OS_TIME_Convertus2Cycles(OS_U32 us)                         OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U64 OS_TIME_GetCycles       (void)                              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U32 OS_TIME_Getus           (void)                              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U64 OS_TIME_Getus64         (void)                              OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#define OS_TIME_GetInts()  OS_Global.TickCnt
#endif

/*********************************************************************
*
*       MPU support
*
**********************************************************************
*/
#if (OS_SUPPORT_MPU != 0)
void                OS_MPU_AssertPrivilegedState(void)                                                                                                           OS_CODE_SECTION_ATTRIBUTE;
void                OS_MPU_Enable               (void)                                                                                                           OS_CODE_SECTION_ATTRIBUTE;
void                OS_MPU_EnableEx             (OS_CONST_PTR OS_MPU_API_LIST* pAPIList)                                                                         OS_CODE_SECTION_ATTRIBUTE;
void                OS_MPU_ConfigMem            (OS_U32 ROM_BaseAddr, OS_U32 ROM_Size, OS_U32 RAM_BaseAddr, OS_U32 RAM_Size, OS_U32 OS_BaseAddr, OS_U32 OS_Size) OS_CODE_SECTION_ATTRIBUTE;
void                OS_MPU_SetDeviceDriverList  (OS_CONST_PTR OS_MPU_DEVICE_DRIVER_FUNC* pList)                                                                  OS_CODE_SECTION_ATTRIBUTE;
void                OS_MPU_SetAllowedObjects    (OS_TASK* pTask, OS_CONST_PTR OS_MPU_OBJ* pObjList)                                                              OS_CODE_SECTION_ATTRIBUTE;
void                OS_MPU_AddRegion            (OS_TASK* pTask, OS_U32 BaseAddr, OS_U32 Size, OS_U32 Permissions, OS_U32 Attributes)                            OS_CODE_SECTION_ATTRIBUTE;
void                OS_MPU_SetErrorCallback     (OS_MPU_ERROR_CALLBACK pFunc)                                                                                    OS_CODE_SECTION_ATTRIBUTE;
void                OS_MPU_SwitchToUnprivState  (void)                                                                                                           OS_CODE_SECTION_ATTRIBUTE;
void                OS_MPU_SwitchToUnprivStateEx(voidRoutine* pRoutine, void OS_STACKPTR* pStack, OS_UINT StackSize)                                             OS_CODE_SECTION_ATTRIBUTE;
void                OS_MPU_CallDeviceDriver     (OS_U32 Index, void* Param)                                                                                      OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_MPU_THREAD_STATE OS_MPU_GetThreadState       (void)                                                                                                           OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                OS_MPU_ExtendTaskContext    (void)                                                                                                           OS_CODE_SECTION_ATTRIBUTE;
#ifdef OS_LIBMODE_SAFE
void                OS_MPU_SetSanityCheckBuffer (OS_TASK* pTask, void* p)                                                                                        OS_CODE_SECTION_ATTRIBUTE;
OS_BOOL             OS_MPU_SanityCheck          (void)                                                                                                           OS_CODE_SECTION_ATTRIBUTE;
#endif
#endif // OS_SUPPORT_MPU

/*********************************************************************
*
*       System tick
*
**********************************************************************
*/
void OS_TICK_Handle      (void)                                                OS_CODE_SECTION_ATTRIBUTE;
void OS_TICK_HandleEx    (void)                                                OS_CODE_SECTION_ATTRIBUTE;
void OS_TICK_HandleNoHook(void)                                                OS_CODE_SECTION_ATTRIBUTE;  // Alternate faster tick handler without tick-hook-function
void OS_TICK_Config      (OS_U32 TickFreq, OS_U32 IntFreq)                     OS_CODE_SECTION_ATTRIBUTE;
void OS_TICK_AddHook     (OS_TICK_HOOK* pHook, OS_TICK_HOOK_ROUTINE* pfUser)   OS_CODE_SECTION_ATTRIBUTE;
void OS_TICK_RemoveHook  (OS_CONST_PTR OS_TICK_HOOK* pHook)                    OS_CODE_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Tickless support
*
**********************************************************************
*/
#if (OS_SUPPORT_TICKLESS != 0)
OS_TIME OS_TICKLESS_GetNumIdleTicks(void)                                OS_CODE_SECTION_ATTRIBUTE;
void    OS_TICKLESS_AdjustTime     (OS_TIME Time)                        OS_CODE_SECTION_ATTRIBUTE;
void    OS_TICKLESS_Start          (OS_TIME Time, voidRoutine* Callback) OS_CODE_SECTION_ATTRIBUTE;
void    OS_TICKLESS_Stop           (void)                                OS_CODE_SECTION_ATTRIBUTE;
#endif

/*********************************************************************
*
*       embOS peripheral power control
*
**********************************************************************
*/
#if (OS_SUPPORT_PERIPHERAL_POWER_CTRL != 0)
void    OS_POWER_UsageInc(OS_UINT Index) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_POWER_UsageDec(OS_UINT Index) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_UINT OS_POWER_GetMask (void)          OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#endif

/*********************************************************************
*
*       Spinlock API
*
**********************************************************************
*/
void OS_SPINLOCK_Create   (OS_SPINLOCK*    pSpinlock)             OS_CODE_SECTION_ATTRIBUTE;
void OS_SPINLOCK_Lock     (OS_SPINLOCK*    pSpinlock)             OS_CODE_SECTION_ATTRIBUTE;
void OS_SPINLOCK_Unlock   (OS_SPINLOCK*    pSpinlock)             OS_CODE_SECTION_ATTRIBUTE;
void OS_SPINLOCK_SW_Create(OS_SPINLOCK_SW* pSpinlock)             OS_CODE_SECTION_ATTRIBUTE;
void OS_SPINLOCK_SW_Lock  (OS_SPINLOCK_SW* pSpinlock, OS_UINT Id) OS_CODE_SECTION_ATTRIBUTE;
void OS_SPINLOCK_SW_Unlock(OS_SPINLOCK_SW* pSpinlock, OS_UINT Id) OS_CODE_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Watchdog API
*
**********************************************************************
*/
void OS_WD_Add    (OS_WD* pWD, OS_TIME Timeout)                                    OS_CODE_SECTION_ATTRIBUTE;
void OS_WD_Check  (void)                                                           OS_CODE_SECTION_ATTRIBUTE;
void OS_WD_Config (voidRoutine* pfTriggerFunc, OS_WD_RESET_CALLBACK* pfResetFunc)  OS_CODE_SECTION_ATTRIBUTE;
void OS_WD_Remove (OS_CONST_PTR OS_WD* pWD)                                        OS_CODE_SECTION_ATTRIBUTE;
void OS_WD_Trigger(OS_WD* pWD)                                                     OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

/*********************************************************************
*
*       Object name API
*
**********************************************************************
*/
#if (OS_TRACKNAME != 0)
void                    OS_DEBUG_SetObjName   (OS_OBJNAME* pObjName, OS_CONST_PTR void* pOSObjID, OS_CONST_PTR char* sName) OS_CODE_SECTION_ATTRIBUTE;
OS_ROM_DATA const char* OS_DEBUG_GetObjName   (OS_CONST_PTR void* pOSObjID)                                                 OS_CODE_SECTION_ATTRIBUTE;
void                    OS_DEBUG_RemoveObjName(OS_CONST_PTR OS_OBJNAME* pObjName)                                           OS_CODE_SECTION_ATTRIBUTE;
#else
  #define OS_DEBUG_SetObjName(a,b,c)
  #define OS_DEBUG_GetObjName(a)      NULL
  #define OS_DEBUG_RemoveObjName(a)
#endif

/*********************************************************************
*
*       BSP related (RTOSInit)
*
**********************************************************************
*/
void   OS_InitHW          (void)            OS_CODE_SECTION_ATTRIBUTE;
void   OS_Idle            (void)            OS_CODE_SECTION_ATTRIBUTE;
OS_U32 OS_GetTime_Cycles  (void)            OS_CODE_SECTION_ATTRIBUTE;
OS_U32 OS_ConvertCycles2us(OS_U32 Cycles)   OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

/*********************************************************************
*
*       embOSView communication
*
**********************************************************************
*/
#ifndef OS_LIBMODE_SAFE
void            OS_COM_Init          (void)                                                     OS_CODE_SECTION_ATTRIBUTE;
void            OS_COM_ClearTxActive (void)                                                     OS_CODE_SECTION_ATTRIBUTE;
OS_U16          OS_COM_EvaPacketEx   (OS_CONST_PTR OS_U8* pSrc, OS_U16 SrcLen, OS_U8** pReturn) OS_CODE_SECTION_ATTRIBUTE;
OS_INT          OS_COM_GetNextChar   (void)                                                     OS_CODE_SECTION_ATTRIBUTE;
void            OS_COM_OnRx          (OS_U8 Data)                                               OS_CODE_SECTION_ATTRIBUTE;
OS_U8           OS_COM_OnTx          (void)                                                     OS_CODE_SECTION_ATTRIBUTE;
void            OS_COM_Send1         (unsigned char c)                                          OS_CODE_SECTION_ATTRIBUTE;
void            OS_COM_SendString    (OS_ROM_DATA const char* s)                                OS_CODE_SECTION_ATTRIBUTE;
void            OS_COM_SendStringFast(OS_ROM_DATA const char* s)                                OS_CODE_SECTION_ATTRIBUTE;
OS_RX_CALLBACK* OS_COM_SetRxCallback (OS_RX_CALLBACK* cb)                                       OS_CODE_SECTION_ATTRIBUTE;
#endif

/*********************************************************************
*
*       Profiling
*
**********************************************************************
*/
#if (OS_PROFILE != 0)
  int    OS_STAT_GetLoad            (const OS_TASK* pTask)        OS_CODE_SECTION_ATTRIBUTE;
  void   OS_STAT_Sample             (void)                        OS_CODE_SECTION_ATTRIBUTE;
  void   OS_STAT_Enable             (void)                        OS_CODE_SECTION_ATTRIBUTE;
  void   OS_STAT_Disable            (void)                        OS_CODE_SECTION_ATTRIBUTE;
  OS_U32 OS_STAT_GetExecTime        (OS_CONST_PTR OS_TASK* pTask) OS_CODE_SECTION_ATTRIBUTE;
  void   OS_EnableProfiling         (int Period)                  OS_CODE_SECTION_ATTRIBUTE;
  #define OS_DisableProfiling()     { OS_ProfilingOn = 0; }
#else
  #define OS_STAT_GetExecTime(pTask)  (0u)
  #define OS_STAT_GetLoad(pTask)      (0)
  #define OS_STAT_Sample()
  #define OS_EnableProfiling(Period)
  #define OS_DisableProfiling()
#endif

#if (OS_SUPPORT_STAT != 0)
  OS_U32 OS_STAT_GetNumActivations(OS_CONST_PTR OS_TASK* pTask) OS_CODE_SECTION_ATTRIBUTE;
  OS_U32 OS_STAT_GetNumPreemptions(OS_CONST_PTR OS_TASK* pTask) OS_CODE_SECTION_ATTRIBUTE;
#endif

/*********************************************************************
*
*       CPU load measurement
*
**********************************************************************
*/
#ifndef OS_LIBMODE_SAFE
void OS_STAT_AddLoadMeasurement  (int Period, OS_U8 AutoAdjust, OS_I32 DefaultMaxValue)                                              OS_CODE_SECTION_ATTRIBUTE;
void OS_STAT_AddLoadMeasurementEx(int Period, OS_U8 AutoAdjust, OS_I32 DefaultMaxValue, void OS_STACKPTR* pStack, OS_UINT StackSize) OS_CODE_SECTION_ATTRIBUTE;
int  OS_STAT_GetLoadMeasurement  (void)                                                                                              OS_CODE_SECTION_ATTRIBUTE;
#endif

#if (OS_U32_OP_IS_ATOMIC == 0)
  #define OS_INC_IDLE_CNT() \
  {                         \
    OS_INT_Disable();       \
    OS_IdleCnt++;           \
    OS_RESTORE_I();         \
  }
#else
  #define OS_INC_IDLE_CNT() (OS_IdleCnt++)
#endif

/*********************************************************************
*
*       Trace support (embOSView)
*
**********************************************************************

Trace support is enabled by defining OS_TRACE = 1.
This is automatically done, when OS_LIBMODE_T is defined.

*/
  void OS_TRACE_SetAPI          (OS_CONST_PTR OS_TRACE_API* pTraceAPI) OS_CODE_SECTION_ATTRIBUTE;
#if (OS_TRACE != 0)
  //
  // Declare trace function prototypes
  //
  void OS_TRACE_Void            (OS_U8 id)                                            OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_Ptr             (OS_U8 id, volatile OS_CONST_PTR void* p)             OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_Data            (OS_U8 id, int v)                                     OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_DataPtr         (OS_U8 id, int v, volatile OS_CONST_PTR void* p)      OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_U32Ptr          (OS_U8 id, OS_U32 p0, volatile OS_CONST_PTR void* p1) OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_PtrU32          (OS_U8 id, volatile OS_CONST_PTR void* p0, OS_U32 p1) OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_Enable          (void)                                                OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_Disable         (void)                                                OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_EnableAll       (void)                                                OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_DisableAll      (void)                                                OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_EnableId        (OS_U8 id)                                            OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_DisableId       (OS_U8 id)                                            OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_EnableFilterId  (OS_U8 FilterIndex, OS_U8 id)                         OS_CODE_SECTION_ATTRIBUTE;
  void OS_TRACE_DisableFilterId (OS_U8 FilterIndex, OS_U8 id)                         OS_CODE_SECTION_ATTRIBUTE;
#else
  //
  // Replace trace functions by empty macros if trace is not enabled
  //
  #define OS_TRACE_Void(id)
  #define OS_TRACE_Ptr(id, p)
  #define OS_TRACE_Data(id, v)
  #define OS_TRACE_DataPtr(id, v, p)
  #define OS_TRACE_U32Ptr(id, p0, p1)
  #define OS_TRACE_PtrU32(id, p0, p1)
  #define OS_TRACE_Enable()
  #define OS_TRACE_Disable()
  #define OS_TRACE_EnableAll()
  #define OS_TRACE_DisableAll()
  #define OS_TRACE_EnableId(id)
  #define OS_TRACE_DisableId(id)
  #define OS_TRACE_EnableFilterId(FilterIndex, id)
  #define OS_TRACE_DisableFilterId(FilterIndex, id)
#endif

/*********************************************************************
*
*       Debug error handler
*
**********************************************************************
*/
void OS_Error(OS_STATUS ErrCode) OS_CODE_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Assembler API
*
**********************************************************************
*/
OS_INTERWORK void OS_StartASM     (void)                                       OS_CODE_SECTION_ATTRIBUTE;
OS_INTERWORK void OS_StopASM      (void)                                       OS_CODE_SECTION_ATTRIBUTE;
OS_INTERWORK int  OS_SwitchFromInt(void)                                       OS_CODE_SECTION_ATTRIBUTE;
#if (OS_SUPPORT_MPU != 0)
OS_INTERWORK void OS_MPU_ErrorASM (OS_TASK* pTask, OS_MPU_ERRORCODE ErrorCode) OS_CODE_SECTION_ATTRIBUTE;
#endif

/*********************************************************************
*
*       embOS init/start/stop
*
**********************************************************************
*/
void OS_Init      (void)                                               OS_CODE_SECTION_ATTRIBUTE;
void OS_DeInit    (void)                                               OS_CODE_SECTION_ATTRIBUTE;
void OS_Stop      (void)                                               OS_CODE_SECTION_ATTRIBUTE;
void OS_ConfigStop(OS_MAIN_CONTEXT* pContext, void* Addr, OS_U32 Size) OS_CODE_SECTION_ATTRIBUTE;

#ifndef   OS_DEINIT
  #define OS_DEINIT()
#endif

#if (OS_DEBUG != 0)
  #define OS_Start()                           \
    OS_MPU_ASSERT_PRIVSTATE();                 \
    OS_ASSERT_INIT_CALLED();                   \
    if (OS_INFO_GetVersion() != OS_VERSION) {  \
      OS_Error(OS_ERR_VERSION_MISMATCH);       \
    }                                          \
    OS_INT_Disable();                          \
    if (OS_Running == 1u) {                    \
      OS_Error(OS_ERR_ILLEGAL_AFTER_OSSTART);  \
    }                                          \
    OS_Running = 1u;                           \
    OS_StartASM()
#else
  #define OS_Start()                           \
    OS_INT_Disable();                          \
    OS_Running = 1u;                           \
    OS_StartASM()
#endif

/*********************************************************************
*
*       Info routines
*
**********************************************************************
*/
OS_ROM_DATA const char* OS_INFO_GetCPU     (void) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_ROM_DATA const char* OS_INFO_GetLibMode (void) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_ROM_DATA const char* OS_INFO_GetModel   (void) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_ROM_DATA const char* OS_INFO_GetLibName (void) OS_CODE_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_UINT                 OS_INFO_GetVersion (void) OS_CODE_SECTION_ATTRIBUTE;
#define OS_INFO_GetTimerFreq()  OS_SysTimer_Settings.Config.TimerFreq

/*********************************************************************
*
*        Compatibility with manual and older embOS versions
*
**********************************************************************
*/
//
// Compatibility with manual
//
#define OS_CREATEMB                 OS_MAILBOX_Create
#define OS_GetUseCnt                OS_MUTEX_GetValue
#define OS_WaitCSema_Timed          OS_SEMAPHORE_TakeTimed
#define OS_WaitEvent_Timed          OS_TASKEVENT_GetTimed
#define OS_GetEventsOccured         OS_TASKEVENT_Get
//
// Compatibility with pre V3.60e versions, renamed functions
//
#define OS_HandleTick               OS_TICK_Handle
#define OS_HandleTickEx             OS_TICK_HandleEx
#define OS_AddTickHook              OS_TICK_AddHook
#define OS_RemoveTickHook           OS_TICK_RemoveHook
//
// Compatibility with pre V3.82v versions, renamed functions
//
#define OS_Terminate                OS_TASK_Terminate
//
// Compatibility with pre V4.30 versions, renamed function
//
#define OS_AddOnTerminateHook       OS_TASK_AddTerminateHook
#define OS_RemoveOnTerminateHooks   OS_TASK_RemoveAllTerminateHooks
#define OS_ResumeAllSuspendedTasks  OS_TASK_ResumeAll
//
// Translation macros for compatibility with older versions
//
#define OS_RegionCnt                OS_Global.Counters.Cnt.Region
#define OS_DICnt                    OS_Global.Counters.Cnt.DI
#define OS_pTask                    OS_Global.pTask
#define OS_pCurrentTask             OS_Global.pCurrentTask
#define OS_pActiveTask              OS_Global.pActiveTask
#define OS_pTimer                   OS_Global.pTimer
#define OS_pCurrentTimer            OS_Global.pCurrentTimer
#define OS_Counters                 OS_Global.Counters
#define OS_Pending                  OS_Global.Pending
#define OS_Ipl_DI                   OS_Global.Ipl_DI
#define OS_Ipl_EI                   OS_Global.Ipl_EI
#define OS_Time                     OS_Global.Time
#define OS_TimeDex                  OS_Global.TimeDex
#if (OS_RR_SUPPORTED != 0)
  #define OS_TimeSlice              OS_Global.TimeSlice
  #define OS_TimeSliceAtStart       OS_Global.TimeSliceAtStart
#endif
#define OS_GetType()                OS_GetLibMode()
#define OS_TASKID                   OS_TASK*
#define OS_GetpCurrentTask()        OS_Global.pCurrentTask

//
// Compatibility with pre V5.00 versions, renamed functions and types
//
#define OS_Config_Stop                                                        OS_ConfigStop
#define OS_InitKern                                                           OS_Init
#define OS_AddExtendTaskContext                                               OS_TASK_AddContextExtension
#define OS_AddTerminateHook                                                   OS_TASK_AddTerminateHook
#define OS_CREATETASK(a,b,c,d,e)                                              OS_TASK_CREATE(a,b,d,c,e)
#define OS_CreateTask                                                         OS_TASK_Create
#define OS_CREATETASK_EX(a,b,c,d,e,f)                                         OS_TASK_CREATEEX(a,b,d,c,e,f)
#define OS_CreateTaskEx                                                       OS_TASK_CreateEx
#define OS_Delay                                                              OS_TASK_Delay
#define OS_DelayUntil                                                         OS_TASK_DelayUntil
#define OS_Delayus                                                            OS_TASK_Delayus
#define OS_GetTaskName                                                        OS_TASK_GetName
#define OS_GetNumTasks                                                        OS_TASK_GetNumTasks
#define OS_GetPriority                                                        OS_TASK_GetPriority
#define OS_GetSuspendCnt                                                      OS_TASK_GetSuspendCnt
#define OS_GetTaskID                                                          OS_TASK_GetID
#define OS_GetTimeSliceRem                                                    OS_TASK_GetTimeSliceRem
#define OS_IsTask                                                             OS_TASK_IsTask
#define OS_TaskIndex2Ptr                                                      OS_TASK_Index2Ptr
#define OS_RemoveAllTerminateHooks                                            OS_TASK_RemoveAllTerminateHooks
#define OS_RemoveTerminateHook                                                OS_TASK_RemoveTerminateHook
#define OS_Resume                                                             OS_TASK_Resume
#define OS_ResumeAllTasks                                                     OS_TASK_ResumeAll
#define OS_ExtendTaskContext                                                  OS_TASK_SetContextExtension
#define OS_SetDefaultTaskContextExtension                                     OS_TASK_SetDefaultContextExtension
#define OS_SetDefaultTaskStartHook                                            OS_TASK_SetDefaultStartHook
#define OS_SetInitialSuspendCnt                                               OS_TASK_SetInitialSuspendCnt
#define OS_SetTaskName                                                        OS_TASK_SetName
#define OS_SetPriority                                                        OS_TASK_SetPriority
#define OS_SetTimeSlice                                                       OS_TASK_SetTimeSlice
#define OS_Suspend                                                            OS_TASK_Suspend
#define OS_SuspendAllTasks                                                    OS_TASK_SuspendAll
#define OS_TerminateTask                                                      OS_TASK_Terminate
#define OS_WakeTask                                                           OS_TASK_Wake
#define OS_Yield                                                              OS_TASK_Yield
#define OS_CREATETIMER                                                        OS_TIMER_CREATE
#define OS_CreateTimer                                                        OS_TIMER_Create
#define OS_CREATETIMER_EX                                                     OS_TIMER_CREATEEX
#define OS_CreateTimerEx                                                      OS_TIMER_CreateEx
#define OS_DeleteTimer                                                        OS_TIMER_Delete
#define OS_DeleteTimerEx                                                      OS_TIMER_DeleteEx
#define OS_GetpCurrentTimer                                                   OS_TIMER_GetCurrent
#define OS_GetpCurrentTimerEx                                                 OS_TIMER_GetCurrentEx
#define OS_GetTimerPeriod                                                     OS_TIMER_GetPeriod
#define OS_GetTimerPeriodEx                                                   OS_TIMER_GetPeriodEx
#define OS_GetTimerValue                                                      OS_TIMER_GetRemainingPeriod
#define OS_GetTimerValueEx                                                    OS_TIMER_GetRemainingPeriodEx
#define OS_GetTimerStatus                                                     OS_TIMER_GetStatus
#define OS_GetTimerStatusEx                                                   OS_TIMER_GetStatusEx
#define OS_RetriggerTimer                                                     OS_TIMER_Restart
#define OS_RetriggerTimerEx                                                   OS_TIMER_RestartEx
#define OS_SetTimerPeriod                                                     OS_TIMER_SetPeriod
#define OS_SetTimerPeriodEx                                                   OS_TIMER_SetPeriodEx
#define OS_StartTimer                                                         OS_TIMER_Start
#define OS_StartTimerEx                                                       OS_TIMER_StartEx
#define OS_StopTimer                                                          OS_TIMER_Stop
#define OS_StopTimerEx                                                        OS_TIMER_StopEx
#define OS_TriggerTimer                                                       OS_TIMER_Trigger
#define OS_TriggerTimerEx                                                     OS_TIMER_TriggerEx
#define OS_ClearEvents                                                        OS_TASKEVENT_Clear
#define OS_ClearEventsEx                                                      OS_TASKEVENT_ClearEx
#define OS_GetEventsOccurred                                                  OS_TASKEVENT_Get
#define OS_WaitEvent                                                          OS_TASKEVENT_GetBlocked
#define OS_WaitSingleEvent                                                    OS_TASKEVENT_GetSingleBlocked
#define OS_WaitSingleEventTimed                                               OS_TASKEVENT_GetSingleTimed
#define OS_WaitEventTimed                                                     OS_TASKEVENT_GetTimed
#define OS_SignalEvent(a,b)                                                   OS_TASKEVENT_Set(b,a)
#define OS_EVENT_Wait                                                         OS_EVENT_GetBlocked
#define OS_EVENT_WaitMask                                                     OS_EVENT_GetMaskBlocked
#define OS_EVENT_WaitMaskTimed                                                OS_EVENT_GetMaskTimed
#define OS_EVENT_WaitTimed                                                    OS_EVENT_GetTimed
#define OS_CreateRSema                                                        OS_MUTEX_Create
#define OS_CREATERSEMA                                                        OS_MUTEX_Create
#define OS_DeleteRSema                                                        OS_MUTEX_Delete
#define OS_GetResourceOwner                                                   OS_MUTEX_GetOwner
#define OS_GetSemaValue                                                       OS_MUTEX_GetValue
#define OS_Request                                                            OS_MUTEX_Lock
#define OS_Use                                                                OS_MUTEX_LockBlocked
#define OS_UseTimed                                                           OS_MUTEX_LockTimed
#define OS_Unuse                                                              OS_MUTEX_Unlock
#define OS_CREATECSEMA                                                        OS_SEMAPHORE_CREATE
#define OS_CreateCSema                                                        OS_SEMAPHORE_Create
#define OS_DeleteCSema                                                        OS_SEMAPHORE_Delete
#define OS_GetCSemaValue                                                      OS_SEMAPHORE_GetValue
#define OS_SignalCSema                                                        OS_SEMAPHORE_Give
#define OS_SignalCSemaMax                                                     OS_SEMAPHORE_GiveMax
#define OS_SetCSemaValue                                                      OS_SEMAPHORE_SetValue
#define OS_CSemaRequest                                                       OS_SEMAPHORE_Take
#define OS_WaitCSema                                                          OS_SEMAPHORE_TakeBlocked
#define OS_WaitCSemaTimed                                                     OS_SEMAPHORE_TakeTimed
#define OS_ClearMB                                                            OS_MAILBOX_Clear
#define OS_CreateMB                                                           OS_MAILBOX_Create
#define OS_DeleteMB                                                           OS_MAILBOX_Delete
#define OS_GetMailCond                                                        OS_MAILBOX_Get
#define OS_GetMailCond1                                                       OS_MAILBOX_Get1
#define OS_GetMail                                                            OS_MAILBOX_GetBlocked
#define OS_GetMail1                                                           OS_MAILBOX_GetBlocked1
#define OS_GetMessageCnt                                                      OS_MAILBOX_GetMessageCnt
#define OS_GetMailTimed                                                       OS_MAILBOX_GetTimed
#define OS_GetMailTimed1                                                      OS_MAILBOX_GetTimed1
#define OS_Mail_GetPtrCond                                                    OS_MAILBOX_GetPtr
#define OS_Mail_GetPtr                                                        OS_MAILBOX_GetPtrBlocked
#define OS_PeekMail                                                           OS_MAILBOX_Peek
#define OS_Mail_Purge                                                         OS_MAILBOX_Purge
#define OS_PutMailCond                                                        OS_MAILBOX_Put
#define OS_PutMailCond1                                                       OS_MAILBOX_Put1
#define OS_PutMail                                                            OS_MAILBOX_PutBlocked
#define OS_PutMail1                                                           OS_MAILBOX_PutBlocked1
#define OS_PutMailFrontCond                                                   OS_MAILBOX_PutFront
#define OS_PutMailFrontCond1                                                  OS_MAILBOX_PutFront1
#define OS_PutMailFront                                                       OS_MAILBOX_PutFrontBlocked
#define OS_PutMailFront1                                                      OS_MAILBOX_PutFrontBlocked1
#define OS_PutMailTimed                                                       OS_MAILBOX_PutTimed
#define OS_PutMailTimed1                                                      OS_MAILBOX_PutTimed1
#define OS_WaitMail                                                           OS_MAILBOX_WaitBlocked
#define OS_WaitMailTimed                                                      OS_MAILBOX_WaitTimed
#define OS_Q_Clear                                                            OS_QUEUE_Clear
#define OS_Q_Create                                                           OS_QUEUE_Create
#define OS_Q_Delete                                                           OS_QUEUE_Delete
#define OS_Q_GetMessageCnt                                                    OS_QUEUE_GetMessageCnt
#define OS_Q_GetMessageSize                                                   OS_QUEUE_GetMessageSize
#define OS_Q_GetPtrCond                                                       OS_QUEUE_GetPtr
#define OS_Q_GetPtr                                                           OS_QUEUE_GetPtrBlocked
#define OS_Q_GetPtrTimed                                                      OS_QUEUE_GetPtrTimed
#define OS_Q_IsInUse                                                          OS_QUEUE_IsInUse
#define OS_Q_PeekPtr                                                          OS_QUEUE_PeekPtr
#define OS_Q_Purge                                                            OS_QUEUE_Purge
#define OS_Q_Put                                                              OS_QUEUE_Put
#define OS_Q_PutEx                                                            OS_QUEUE_PutEx
#define OS_Q_PutBlocked                                                       OS_QUEUE_PutBlocked
#define OS_Q_PutBlockedEx                                                     OS_QUEUE_PutBlockedEx
#define OS_Q_PutTimed                                                         OS_QUEUE_PutTimed
#define OS_Q_PutTimedEx                                                       OS_QUEUE_PutTimedEx
#define OS_CallISR                                                            OS_INT_Call
#define OS_CallNestableISR                                                    OS_INT_CallNestable
#define OS_EnterInterrupt                                                     OS_INT_Enter
#define OS_EnterIntStack                                                      OS_INT_EnterIntStack
#define OS_EnterNestableInterrupt                                             OS_INT_EnterNestable
#define OS_InInterrupt                                                        OS_INT_InInterrupt
#define OS_LeaveInterrupt                                                     OS_INT_Leave
#define OS_LeaveIntStack                                                      OS_INT_LeaveIntStack
#define OS_LeaveNestableInterrupt                                             OS_INT_LeaveNestable
#define OS_DecRI                                                              OS_INT_DecRI
#define OS_DI                                                                 OS_INT_Disable
#define OS_INTERRUPT_MaskGlobal                                               OS_INT_DisableAll
#define OS_EI                                                                 OS_INT_Enable
#define OS_INTERRUPT_UnmaskGlobal                                             OS_INT_EnableAll
#define OS_RestoreI                                                           OS_INT_EnableConditional
#define OS_IncDI                                                              OS_INT_IncDI
#define OS_INT_PRIO_PRESERVE                                                  OS_INT_Preserve
#define OS_INTERRUPT_PreserveGlobal                                           OS_INT_PreserveAll
#define OS_INTERRUPT_PreserveAndMaskGlobal                                    OS_INT_PreserveAndDisableAll
#define OS_INT_PRIO_RESTORE                                                   OS_INT_Restore
#define OS_INTERRUPT_RestoreGlobal                                            OS_INT_RestoreAll
#define OS_EnterRegion                                                        OS_TASK_EnterRegion
#define OS_LeaveRegion                                                        OS_TASK_LeaveRegion
#define OS_GetTime                                                            OS_TIME_GetTicks
#define OS_GetTime32                                                          OS_TIME_GetTicks32
#define OS_Config_SysTimer                                                    OS_TIME_ConfigSysTimer
#define OS_Timing_GetCycles                                                   OS_TIME_GetResult
#define OS_Timing_Getus                                                       OS_TIME_GetResultus
#define OS_GetTime_us                                                         OS_TIME_Getus
#define OS_GetTime_us64                                                       OS_TIME_Getus64
#define OS_Timing_Start                                                       OS_TIME_StartMeasurement
#define OS_Timing_End                                                         OS_TIME_StopMeasurement
#define OS_AdjustTime                                                         OS_TICKLESS_AdjustTime
#define OS_GetNumIdleTicks                                                    OS_TICKLESS_GetNumIdleTicks
#define OS_StartTicklessMode                                                  OS_TICKLESS_Start
#define OS_StopTicklessMode                                                   OS_TICKLESS_Stop
#define OS_free                                                               OS_HEAP_free
#define OS_malloc                                                             OS_HEAP_malloc
#define OS_realloc                                                            OS_HEAP_realloc
#define OS_MEMF_Request(a,b)                                                  OS_MEMPOOL_Alloc(a)
#define OS_MEMF_Alloc(a,b)                                                    OS_MEMPOOL_AllocBlocked(a)
#define OS_MEMF_AllocTimed(a,b,c)                                             OS_MEMPOOL_AllocTimed(a,b)
#define OS_MEMF_Create                                                        OS_MEMPOOL_Create
#define OS_MEMF_Delete                                                        OS_MEMPOOL_Delete
#define OS_MEMF_FreeBlock                                                     OS_MEMPOOL_Free
#define OS_MEMF_Release                                                       OS_MEMPOOL_FreeEx
#define OS_MEMF_GetBlockSize                                                  OS_MEMPOOL_GetBlockSize
#define OS_MEMF_GetMaxUsed                                                    OS_MEMPOOL_GetMaxUsed
#define OS_MEMF_GetNumBlocks                                                  OS_MEMPOOL_GetNumBlocks
#define OS_MEMF_GetNumFreeBlocks                                              OS_MEMPOOL_GetNumFreeBlocks
#define OS_MEMF_IsInPool                                                      OS_MEMPOOL_IsInPool
#define OS_GetObjName                                                         OS_DEBUG_GetObjName
#define OS_SetObjName                                                         OS_DEBUG_SetObjName
#define OS_AddLoadMeasurement                                                 OS_STAT_AddLoadMeasurement
#define OS_STAT_GetTaskExecTime                                               OS_STAT_GetExecTime
#define OS_GetLoadMeasurement                                                 OS_STAT_GetLoadMeasurement
#define OS_ClearTxActive                                                      OS_COM_ClearTxActive
#define OS_GetNextChar                                                        OS_COM_GetNextChar
#define OS_OnRx                                                               OS_COM_OnRx
#define OS_OnTx                                                               OS_COM_OnTx
#define OS_SendString                                                         OS_COM_SendString
#define OS_SetRxCallback                                                      OS_COM_SetRxCallback
#define OS_TraceEnable                                                        OS_TRACE_Enable
#define OS_TraceEnableAll                                                     OS_TRACE_EnableAll
#define OS_TraceEnableId                                                      OS_TRACE_EnableId
#define OS_TraceEnableFilterId                                                OS_TRACE_EnableFilterId
#define OS_TraceDisable                                                       OS_TRACE_Disable
#define OS_TraceDisableAll                                                    OS_TRACE_DisableAll
#define OS_TraceDisableId                                                     OS_TRACE_DisableId
#define OS_TraceDisableFilterId                                               OS_TRACE_DisableFilterId
#define OS_TraceData                                                          OS_TRACE_Data
#define OS_TraceDataPtr                                                       OS_TRACE_DataPtr
#define OS_TracePtr                                                           OS_TRACE_Ptr
#define OS_SetTraceAPI                                                        OS_TRACE_SetAPI
#define OS_TraceU32Ptr                                                        OS_TRACE_U32Ptr
#define OS_TraceVoid                                                          OS_TRACE_Void
#define OS_MPU_AddSanityCheckBuffer                                           OS_MPU_SetSanityCheckBuffer
#define OS_GetIntStackBase                                                    OS_STACK_GetIntStackBase
#define OS_GetIntStackSize                                                    OS_STACK_GetIntStackSize
#define OS_GetIntStackSpace                                                   OS_STACK_GetIntStackSpace
#define OS_GetIntStackUsed                                                    OS_STACK_GetIntStackUsed
#define OS_GetStackBase                                                       OS_STACK_GetTaskStackBase
#define OS_GetStackSize                                                       OS_STACK_GetTaskStackSize
#define OS_GetStackSpace                                                      OS_STACK_GetTaskStackSpace
#define OS_GetStackUsed                                                       OS_STACK_GetTaskStackUsed
#define OS_GetSysStackBase                                                    OS_STACK_GetSysStackBase
#define OS_GetSysStackSize                                                    OS_STACK_GetSysStackSize
#define OS_GetSysStackSpace                                                   OS_STACK_GetSysStackSpace
#define OS_GetSysStackUsed                                                    OS_STACK_GetSysStackUsed
#define OS_SetStackCheckLimit                                                 OS_STACK_SetCheckLimit
#define OS_GetStackCheckLimit                                                 OS_STACK_GetCheckLimit
#define OS_GetCPU                                                             OS_INFO_GetCPU
#define OS_GetLibMode                                                         OS_INFO_GetLibMode
#define OS_GetLibName                                                         OS_INFO_GetLibName
#define OS_GetModel                                                           OS_INFO_GetModel
#define OS_GetVersion                                                         OS_INFO_GetVersion
#define OS_RSEMA                                                              OS_MUTEX
#define OS_CSEMA                                                              OS_SEMAPHORE
#define OS_Q                                                                  OS_QUEUE
#define OS_MEMF                                                               OS_MEMPOOL
#define OS_Q_SRCLIST                                                          OS_QUEUE_SRCLIST
#define OS_TASK_EVENT                                                         OS_TASKEVENT

/*********************************************************************
*
*        Debug info for thread awareness
*
**********************************************************************
*
* Field offsets required by remote debugger for thread-awareness,
* i.e. for walking the task list.
*/
typedef struct OS_DEBUG_INFO_STRUCT
{
  //
  // Offsets in OS_GLOBAL struct
  //
  OS_U8 OS_GLOBAL_pCurrentTask_Offs;      // Offset of pCurrentTask
  OS_U8 OS_GLOBAL_pTask_Offs;             // Offset of pTask
  OS_U8 OS_GLOBAL_pActiveTask_Offs;       // Offset of pActiveTask
  OS_U8 OS_GLOBAL_pTimer_Offs;            // Offset of pTimer
  OS_U8 OS_GLOBAL_pCurrentTimer_Offs;     // Offset of pCurrentTimer
  OS_U8 OS_GLOBAL_Time_Offs;              // Offset of Time
  OS_U8 OS_GLOBAL_TimeSlice_Offs;         // Offset of TimeSlice
  OS_U8 OS_GLOBAL_TimeSliceAtStart_Offs;  // Offset of TimeSliceAtStart
  //
  // Offsets in OS_TASK struct
  //
  OS_U8 OS_TASK_pNext_Offs;               // Offset of pNext
  OS_U8 OS_TASK_pStack_Offs;              // Offset of pStack
  OS_U8 OS_TASK_Timeout_Offs;             // Offset of Timeout
  OS_U8 OS_TASK_Stat_Offs;                // Offset of Stat
  OS_U8 OS_TASK_Priority_Offs;            // Offset of Priority
  OS_U8 OS_TASK_BasePrio_Offs;            // Offset of BasePrio
  OS_U8 OS_TASK_Events_Offs;              // Offset of Events
  OS_U8 OS_TASK_EventMask_Offs;           // Offset of EventMask
  OS_U8 OS_TASK_Name_Offs;                // Offset of Name
  OS_U8 OS_TASK_StackSize_Offs;           // Offset of StackSize
  OS_U8 OS_TASK_pStackBot_Offs;           // Offset of pStackBot
  OS_U8 OS_TASK_NumActivations_Offs;      // Offset of NumActivations
  OS_U8 OS_TASK_NumPreemptions_Offs;      // Offset of NumPreemptions
  OS_U8 OS_TASK_ExecTotal_Offs;           // Offset of ExecTotal
  OS_U8 OS_TASK_ExecLast_Offs;            // Offset of ExecLast
  OS_U8 OS_TASK_Load_Offs;                // Offset of Load
  OS_U8 OS_TASK_TimeSliceRem_Offs;        // Offset of TimeSliceRem
  OS_U8 OS_TASK_TimeSliceReload_Offs;     // Offset of TimeSliceReload
} OS_DEBUG_INFO;

/********************************************************************/

#ifdef __cplusplus
  }
#endif

#endif /* RTOS_H_INCLUDED */

/*************************** End of file ****************************/
