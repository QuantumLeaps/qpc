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

#ifndef RTOS_H_INCLUDED     /* Avoid multiple inclusion             */
#define RTOS_H_INCLUDED

/*********************************************************************
*
*     Includes
*/
#include <string.h>         // Required for memset() etc.
#include <intrinsics.h>     // Required for __disable_interrupt() etc.

/*********************************************************************
*
*     Chip specifics for Cortex M cores and IAR compiler
*
**********************************************************************
*/

/*********************************************************************
*
*       Configuration
*/
#define OS_PORT_REVISION          (1u)                       // Port specific revision
#define OS_PTR_OP_IS_ATOMIC       (1)
#define OS_U32_OP_IS_ATOMIC       (1)
#define OS_GetTime()              (OS_Global.Time)
#define OS_GetTime32()            (OS_Global.Time)
#define OS_I64                    long long
#define OS_SIZEOF_INT             (4)                        // Size of integer in bytes
#define OS_SUPPORT_SYSSTACK_INFO  (1)                        // Option to supply more info to viewer
#define OS_SUPPORT_INTSTACK_INFO  (0)                        // Not used
#define OS_SUPPORT_OS_ALLOC       (1)                        // Define whether OS_Alloc is supported
#define OS_SP_ADJUST              (0)
#define OS_EnterIntStack()                                   // No special stack switching required
#define OS_LeaveIntStack()                                   // No special stack switching required
#if (defined __ARM6M__) && (__CORE__ == __ARM6M__)
  #define OS_MEMCPY               OS_memcpy                  // Overwrite IAR Library memcpy() because it might not work correctly
#endif

/*****  Core specific configuration *********************************/

#define OS_SWITCH_FROM_INT_MODIFIES_STACK   (0)
#define OS_INTERRUPTS_ARE_NESTABLE_ON_ENTRY (1)
#define OS_SCHEDULER_ACTIVATED_BY_EXCEPTION (1)
#define OS_SUPPORT_INT_PRIORITY             (0)              // Variable interrupt priorities for OS not supported by now

//
// Erratum 837070, ARM-EPM-064408 v4.0 [1]: With Cortex-M7 r0p1, increasing priority
// using a write to BASEPRI does not take effect immediately. Hence, we globally
// disable interrupts while accessing BASEPRI. Since other CPUs and revisions are
// not affected, this define may be used to switch off this workaround.
// When modifiying this define, the according define in RTOS_CM4F.s should be changed as well.
//
#ifndef USE_ERRATUM_837070
  #define USE_ERRATUM_837070      (1)                        // 1: use workaround (default), 0: do not use workaround
#endif

//
// With the above workaround, zero-latency ISRs will be enabled after each call to
// OS_DI() or OS_INT_PRIO_RESTORE(). If this is not desired, this define may be used
// to preserve and restore the state of PRIMASK with these calls.
// When modifiying this define, the according define in RTOS_CM4F.s should be changed as well.
//
#if (USE_ERRATUM_837070 == 1)
  #ifndef   OS_PRESERVE_PRIMASK
    #define OS_PRESERVE_PRIMASK     (0)                      // 0: PRIMASK is not preserved (default), 1: PRIMASK is preserved
  #endif
#endif

/*****  End of configuration settings *******************************/
#if defined (__ARM6M__) && (__CORE__ == __ARM6M__)                                     // Cortex-M0
  //
  // Use IAR Intrinsic functions.
  //
  #define OS_DI() __disable_interrupt();
  #define OS_EI() __enable_interrupt();
#elif ((defined(__ARM7M__) && (__CORE__ == __ARM7M__)) || (defined(__ARM7EM__) && (__CORE__ == __ARM7EM__) && (USE_ERRATUM_837070 == 0)))  // Cortex-M3, Cortex-M4/M4F/M7 w/o workaround
  //
  // Use IAR Intrinsic functions.
  // The NOPs should flush the pipeline to ensure that interrupts
  // are disabled before the next instruction is executed.
  //
  #define OS_DI() {                   \
    __set_BASEPRI(OS_IPL_DI_DEFAULT); \
    __no_operation();                 \
    __no_operation();                 \
  }
  #define OS_EI() __set_BASEPRI(OS_IPL_EI_DEFAULT)
#elif (defined(__ARM7EM__) && (__CORE__ == __ARM7EM__) && (USE_ERRATUM_837070 == 1))   // Cortex-M4/M4F/M7 with workaround
  //
  // Use IAR Intrinsic functions.
  // The NOPs should flush the pipeline to ensure that interrupts
  // are disabled before the next instruction is executed.
  //
  #if (OS_PRESERVE_PRIMASK == 1)
    #define OS_DI() {                       \
      OS_U32 primask;                       \
      primask = (OS_U32) __get_PRIMASK();   \
      __disable_interrupt();                \
      __set_BASEPRI(OS_IPL_DI_DEFAULT);     \
      __set_PRIMASK(primask);               \
      __no_operation();                     \
      __no_operation();                     \
    }
  #else
    #define OS_DI() {                       \
      __disable_interrupt();                \
      __set_BASEPRI(OS_IPL_DI_DEFAULT);     \
      __enable_interrupt();                 \
      __no_operation();                     \
      __no_operation();                     \
    }
  #endif
  #define OS_EI() __set_BASEPRI(OS_IPL_EI_DEFAULT)
#else
  #error "Please check that __ARM7M__ or __ARM7EM__or __ARM6M__ is defined!"
#endif

#define OS_IPL_EI_DEFAULT 0     // Modification requires
#define OS_IPL_DI_DEFAULT 128   // modification in RTOS*.s assembly files !

#define OS_U32 unsigned long

#if defined (__ARM7EM__) && (__CORE__ == __ARM7EM__) && defined (__ARMVFP__)
  #define OS_CPU_HAS_VFP  (1)
#else
  #define OS_CPU_HAS_VFP  (0)
#endif

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

#define OS_USEPARA(para)         (void) (para)

/*********************************************************************
*
*       Core specific implementation
*/
typedef void      OS_ISR_HANDLER(void);
OS_ISR_HANDLER*   OS_ARM_InstallISRHandler (int ISRIndex, OS_ISR_HANDLER* pISRHandler);
void              OS_ARM_EnableISR         (int ISRIndex);
void              OS_ARM_DisableISR        (int ISRIndex);
int               OS_ARM_ISRSetPrio        (int ISRIndex, int Prio);
#if defined (__ARM6M__) && (__CORE__ == __ARM6M__)        // Cortex-M0
void              OS_memcpy                (void * pDest, const void * pSrc, int NumBytes);
#endif

#define OS_IDLE()                  // Overrides call of OS_Idle()
#define OS_EI_ON_LEAVE()  OS_EI()  // Required for CPUs which do not restore DI-flag by RETI.
#define OS_ENABLE_INTS_SAVE_IPL()  // Not required with Cortex-M, avoid call of OS_EI()

/*****  Declare functions used in the interrupt vector table in ROM */
void __program_start  (void);  // Program entry
void OS_Systick       (void);  // In RTOSInit w/o CMSIS
void OS_COM_IsrHandler(void);  // In RTOSInit w/o CMSIS

void HardFault_Handler      (void);                  // Implemented in separate assembler module.
void SEGGER_HardFaultHandler(unsigned int* pStack);  // Implemented in separate "C" module.

/*********************************************************************
*
*       Thread locking, thread safe system libraries
*/
#if ((__VER__ >= 441) && (__VER__ < 6000000))
  void OS_InitSysLocks         (void);
  #define OS_INIT_SYS_LOCKS()  OS_InitSysLocks()
#elif ((__VER__ > 6000000) && (__VER__ < 8000000))
  #define OS_INIT_SYS_LOCKS()
  /* embOS specific system locking functions called from the IAR hook functions */
  void OS__iar_system_Mtxinit  (__iar_Rmtx *m);
  void OS__iar_system_Mtxdst   (__iar_Rmtx *m);
  void OS__iar_system_Mtxlock  (__iar_Rmtx *m);
  void OS__iar_system_Mtxunlock(__iar_Rmtx *m);
#elif (__VER__ >= 800)
  #include <DLib_Threads.h>
  #define OS_INIT_SYS_LOCKS()  __iar_Initlocks()
  /* embOS specific system locking functions called from the IAR hook functions */
  void OS__iar_system_Mtxinit  (__iar_Rmtx *m);
  void OS__iar_system_Mtxdst   (__iar_Rmtx *m);
  void OS__iar_system_Mtxlock  (__iar_Rmtx *m);
  void OS__iar_system_Mtxunlock(__iar_Rmtx *m);
#endif

/*********************************************************************
*
*       Thread local storage
*/
#define OS_SUPPORT_TLS                 (1)
#define OS_TASK_HAS_PTLS               (1)                       // pTLS always in task structure to get compatible TCBs for all versions
#define OS_ExtendTaskContext_TLS_VFP   OS_ExtendTaskContext_TLS  // OS_ExtendTaskContext_TLS_VFP is no longer necessary because VFP is now automatically handled by embOS

void OS_ExtendTaskContext_TLS(void);                             // Initialize Thread Local Storage, OS_TLS.c, CPU specific

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
#define OS_INTERRUPT_MaskGlobal()             __disable_interrupt()
#define OS_INTERRUPT_UnmaskGlobal()           __enable_interrupt()
#define OS_INTERRUPT_PreserveGlobal(p)        (*(p) = (OS_U32) __get_PRIMASK())
#define OS_INTERRUPT_RestoreGlobal(p)         __set_PRIMASK(*(p))
#define OS_INTERRUPT_PreserveAndMaskGlobal(p) { \
  (*(p) = (OS_U32) __get_PRIMASK());            \
  __disable_interrupt();                        \
}

/*********************************************************************
*
*       embOS interrupt preserve/restore support
*/
#if defined (__ARM6M__) && (__CORE__ == __ARM6M__)
  #define OS_INT_PRIO_PRESERVE(p)  (*(p) = (OS_U32) __get_PRIMASK())                                                                       // Cortex-M0/M0+/M1
  #define OS_INT_PRIO_RESTORE(p)   __set_PRIMASK(*(p))
#elif ((defined(__ARM7M__) && (__CORE__ == __ARM7M__)) || (defined(__ARM7EM__) && (__CORE__ == __ARM7EM__) && (USE_ERRATUM_837070 == 0)))  // Cortex-M3, Cortex-M4/M4F/M7 w/o workaround
  #define OS_INT_PRIO_PRESERVE(p)  (*(p) = (OS_U32) __get_BASEPRI())
  #define OS_INT_PRIO_RESTORE(p) {      \
    __set_BASEPRI(*(p));                \
    __no_operation();                   \
    __no_operation();                   \
  }
#elif (defined(__ARM7EM__) && (__CORE__ == __ARM7EM__) && (USE_ERRATUM_837070 == 1))                                                       // Cortex-M4/M4F/M7 with workaround
  #define OS_INT_PRIO_PRESERVE(p)  (*(p) = (OS_U32) __get_BASEPRI())
  #if (OS_PRESERVE_PRIMASK == 1)
    //
    // The NOPs should flush the pipeline to ensure that interrupts
    // are disabled before the next instruction is executed.
    //
    #define OS_INT_PRIO_RESTORE(p)  {     \
      OS_U32 primask;                     \
      primask = (OS_U32) __get_PRIMASK(); \
      __disable_interrupt();              \
      __set_BASEPRI(*(p));                \
      __set_PRIMASK(primask);             \
      __no_operation();                   \
      __no_operation();                   \
    }
  #else
    #define OS_INT_PRIO_RESTORE(p)  {     \
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
*       Remap identifiers
*
**********************************************************************

Renaming OS_ChangeTask will generate an error when using sources and
assembly files are compiled with different settings.
Renaming OS_InitKern will generate an error when a library with wrong
data model is used in a project

*/
#if (OS_CPU_HAS_VFP)
  #define OS_ChangeTask OS_ChangeTask_VFP
  #define OS_InitKern   OS_InitKern_VFP
#else
  #define OS_ChangeTask OS_ChangeTask_STD
  #define OS_InitKern   OS_InitKern_STD
#endif

/********************************************************************/

#ifdef __cplusplus
  }
#endif

/****** End of chip / compiler specific part ************************/
/*********************************************************************
*
*       Generic portion of the OS starts here
*
**********************************************************************
This file (original name OS_Raw.h) is part of RTOS.h, the include
file for the OS.
*/

#if (!defined(OS_LIBMODE_XR) && !defined(OS_LIBMODE_R)  &&   \
     !defined(OS_LIBMODE_S)  && !defined(OS_LIBMODE_SP) &&   \
     !defined(OS_LIBMODE_D)  && !defined(OS_LIBMODE_DP) &&   \
     !defined(OS_LIBMODE_DT))
  #include "OS_Config.h"
#endif

#define OS_VERSION_GENERIC    (43400u)  //  Generic version 4.34

#ifndef   OS_PORT_REVISION
  #define OS_PORT_REVISION     0u
#endif

#define OS_VERSION (OS_VERSION_GENERIC + (OS_PORT_REVISION * 25u))

//
// Thread local storage, may be enabled in CPU specific part
//
#ifndef   OS_SUPPORT_TLS
  #define OS_SUPPORT_TLS       0
#endif

//
// embOS software timer may be excluded when compiling sources
//
#ifndef   OS_SUPPORT_TIMER
  #define OS_SUPPORT_TIMER     1
#endif

//
// embOS tickless support may be excluded when compiling sources
//
#ifndef   OS_SUPPORT_TICKLESS
  #define OS_SUPPORT_TICKLESS  1
#endif

//
// embOS default debug level is 1, may be overwritten in project settings
//
#ifndef   OS_DEBUG_LEVEL
  #define OS_DEBUG_LEVEL       1
#endif

/*********************************************************************
*
*       Defines for library types
*
**********************************************************************
*/

//
// If library type is not selected yet (on project level, for example), then select DP by default
//
#if (!defined(OS_LIBMODE_XR) && !defined(OS_LIBMODE_R)  &&   \
     !defined(OS_LIBMODE_S)  && !defined(OS_LIBMODE_SP) &&   \
     !defined(OS_LIBMODE_D)  && !defined(OS_LIBMODE_DP) &&   \
     !defined(OS_LIBMODE_DT))
  #define OS_LIBMODE_DP
#endif

#if defined(OS_LIBMODE_XR)                     // Extremely small release - without Round robin support
  #define OS_CHECKSTACK_DEFAULT                0
  #define OS_DEBUG_DEFAULT                     0
  #define OS_PROFILE_DEFAULT                   0
  #define OS_SUPPORT_TICKSTEP_DEFAULT          0
  #define OS_TRACE_DEFAULT                     0
  #define OS_RR_SUPPORTED_DEFAULT              0
  #define OS_TRACKNAME_DEFAULT                 0
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT 0
  #define OS_CreateTask                        OS_CreateTask_XR
  #define OS_CreateTaskEx                      OS_CreateTaskEx_XR
  #define OS_LIBMODE                           "XR"
  #define OS_SUPPORT_STAT_DEFAULT              0
  #define OS_SUPPORT_PTLS_DEFAULT              0
#elif defined(OS_LIBMODE_R)                    // Release build
  #define OS_CHECKSTACK_DEFAULT                0
  #define OS_DEBUG_DEFAULT                     0
  #define OS_PROFILE_DEFAULT                   0
  #define OS_SUPPORT_TICKSTEP_DEFAULT          0
  #define OS_TRACE_DEFAULT                     0
  #define OS_RR_SUPPORTED_DEFAULT              1
  #define OS_TRACKNAME_DEFAULT                 1
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT 1
  #define OS_CreateTask                        OS_CreateTask_R
  #define OS_CreateTaskEx                      OS_CreateTaskEx_R
  #define OS_LIBMODE                           "R"
  #define OS_SUPPORT_STAT_DEFAULT              0
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
#elif defined(OS_LIBMODE_S)                    // Release build with stack check
  #define OS_CHECKSTACK_DEFAULT                1
  #define OS_DEBUG_DEFAULT                     0
  #define OS_PROFILE_DEFAULT                   0
  #define OS_SUPPORT_TICKSTEP_DEFAULT          0
  #define OS_TRACE_DEFAULT                     0
  #define OS_RR_SUPPORTED_DEFAULT              1
  #define OS_TRACKNAME_DEFAULT                 1
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT 1
  #define OS_CreateTask                        OS_CreateTask_S
  #define OS_CreateTaskEx                      OS_CreateTaskEx_S
  #define OS_LIBMODE                           "S"
  #define OS_SUPPORT_STAT_DEFAULT              0
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
#elif defined(OS_LIBMODE_SP)                   // Release build with stack check and profiling
  #define OS_CHECKSTACK_DEFAULT                1
  #define OS_DEBUG_DEFAULT                     0
  #define OS_PROFILE_DEFAULT                   1
  #define OS_SUPPORT_TICKSTEP_DEFAULT          1
  #define OS_TRACE_DEFAULT                     0
  #define OS_RR_SUPPORTED_DEFAULT              1
  #define OS_TRACKNAME_DEFAULT                 1
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT 1
  #define OS_CreateTask                        OS_CreateTask_SP
  #define OS_CreateTaskEx                      OS_CreateTaskEx_SP
  #define OS_LIBMODE                           "SP"
  #define OS_SUPPORT_STAT_DEFAULT              1
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
#elif defined(OS_LIBMODE_D)                       // Debug build
  #define OS_CHECKSTACK_DEFAULT                1
  #define OS_DEBUG_DEFAULT                     OS_DEBUG_LEVEL
  #define OS_PROFILE_DEFAULT                   0
  #define OS_SUPPORT_TICKSTEP_DEFAULT          1
  #define OS_TRACE_DEFAULT                     0
  #define OS_RR_SUPPORTED_DEFAULT              1
  #define OS_TRACKNAME_DEFAULT                 1
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT 1
  #define OS_CreateTask                        OS_CreateTask_D
  #define OS_CreateTaskEx                      OS_CreateTaskEx_D
  #define OS_LIBMODE                           "D"
  #define OS_SUPPORT_STAT_DEFAULT              1
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
#elif defined(OS_LIBMODE_DP)                   // Debug build with profiling
  #define OS_CHECKSTACK_DEFAULT                1
  #define OS_DEBUG_DEFAULT                     OS_DEBUG_LEVEL
  #define OS_PROFILE_DEFAULT                   1
  #define OS_SUPPORT_TICKSTEP_DEFAULT          1
  #define OS_TRACE_DEFAULT                     0
  #define OS_RR_SUPPORTED_DEFAULT              1
  #define OS_TRACKNAME_DEFAULT                 1
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT 1
  #define OS_CreateTask                        OS_CreateTask_DP
  #define OS_CreateTaskEx                      OS_CreateTaskEx_DP
  #define OS_LIBMODE                           "DP"
  #define OS_SUPPORT_STAT_DEFAULT              1
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
#elif defined(OS_LIBMODE_DT)                   // Debug build with profiling and trace
  #define OS_CHECKSTACK_DEFAULT                1
  #define OS_DEBUG_DEFAULT                     OS_DEBUG_LEVEL
  #define OS_PROFILE_DEFAULT                   1
  #define OS_SUPPORT_TICKSTEP_DEFAULT          1
  #define OS_TRACE_DEFAULT                     1
  #define OS_RR_SUPPORTED_DEFAULT              1
  #define OS_TRACKNAME_DEFAULT                 1
  #define OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT 1
  #define OS_CreateTask                        OS_CreateTask_DT
  #define OS_CreateTaskEx                      OS_CreateTaskEx_DT
  #define OS_LIBMODE                           "DT"
  #define OS_SUPPORT_STAT_DEFAULT              1
  #define OS_SUPPORT_PTLS_DEFAULT              OS_SUPPORT_TLS
#else
  #error Please define library type used !
#endif

//
// If defines are not set use defaults from the according library mode
//
#ifndef   OS_CHECKSTACK
  #define OS_CHECKSTACK                 OS_CHECKSTACK_DEFAULT
#endif
#ifndef   OS_DEBUG
  #define OS_DEBUG                      OS_DEBUG_DEFAULT
#endif
#ifndef   OS_PROFILE
  #define OS_PROFILE                    OS_PROFILE_DEFAULT
#endif
#ifndef   OS_SUPPORT_TICKSTEP
  #define OS_SUPPORT_TICKSTEP           OS_SUPPORT_TICKSTEP_DEFAULT
#endif
#ifndef   OS_TRACE
  #define OS_TRACE                      OS_TRACE_DEFAULT
#endif
#ifndef   OS_TRACE_RECORD_API_END
  #define OS_TRACE_RECORD_API_END       OS_PROFILE_DEFAULT
#endif
#ifndef   OS_RR_SUPPORTED
  #define OS_RR_SUPPORTED               OS_RR_SUPPORTED_DEFAULT
#endif
#ifndef   OS_TRACKNAME
  #define OS_TRACKNAME                  OS_TRACKNAME_DEFAULT
#endif
#ifndef   OS_SUPPORT_SAVE_RESTORE_HOOK
  #define OS_SUPPORT_SAVE_RESTORE_HOOK  OS_SUPPORT_SAVE_RESTORE_HOOK_DEFAULT
#endif
#ifndef   OS_SUPPORT_STAT
  #define OS_SUPPORT_STAT               OS_SUPPORT_STAT_DEFAULT
#endif
#ifndef   OS_SUPPORT_PTLS
  #define OS_SUPPORT_PTLS               OS_SUPPORT_PTLS_DEFAULT
#endif

/*********************************************************************
*
*       OS compile time switches
*
**********************************************************************

These compile time switches can be modified when recompiling
the library. Usually, the values are defined in OSCHIP.h, which is then
merged with OS_Raw.h to form RTOS.h.
If the values are not defined in OSCHIP.h, the default values
below are used.
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

#ifndef   OS_COPYSTRING
  #define OS_COPYSTRING  OS_MEMCPY
#endif

#ifndef   OS_INTERWORK
  #define OS_INTERWORK
#endif

#ifndef   OS_SCHEDULER_ACTIVATED_BY_EXCEPTION
  #define OS_SCHEDULER_ACTIVATED_BY_EXCEPTION   (0)
#endif

#ifndef   OS_STACK_ADR
  #define OS_STACK_ADR  OS_U32
#endif

#ifndef   OS_MEMSET
  #define OS_MEMSET(a,v,s) memset((a),((int) (v)),(size_t)(s))
#endif

#ifndef   OS_MEMCPY
  #define OS_MEMCPY(dest,src,cnt) memcpy((dest),(src),(size_t)(cnt))
#endif

#ifndef   OS_STRLEN
  #define OS_STRLEN(s) strlen(s)
#endif

#ifndef   OS_DUMMY_STATEMENT
  #define OS_DUMMY_STATEMENT()
#endif

#ifndef   OS_USEPARA
  #define OS_USEPARA(para) (para)=(para)
#endif

#ifndef   OS_STACK_AT_BOTTOM
  #define OS_STACK_AT_BOTTOM  (0)
#endif

//
// Settings for NON-ANSI compiler syntax
// The default behavior complies with IAR.
//
#ifndef   OS_COMPILER_LOCATION_LEFT
  #define OS_COMPILER_LOCATION_LEFT          1  // Required for IAR
#endif

#ifndef   OS_COMPILER_STORAGE_MODIFIER_LEFT
  #define OS_COMPILER_STORAGE_MODIFIER_LEFT  1  // Default for all except KEIL
#endif

#define OS_LINK_MAILBOXES (OS_DEBUG)

#ifndef   OS_CHECK
  #define OS_CHECK 0
#endif

#ifndef   OS_ALIGN_PTR
  #define OS_ALIGN_PTR (1)
#endif

#ifndef   OS_ALIGN_INT
  #define OS_ALIGN_INT (1)
#endif

#ifndef   OS_PTR_TO_VALUE
  #define OS_PTR_TO_VALUE(p) (p)
#endif

#ifndef   OS_IDLE
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

#ifndef   OS_INIT_EXPLICITLY            // If enabled, OS_InitKern() initializes Non-Zero globals. Required only for faulty compiler (or startup-code) which do not perform proper init of globals during startup.
  #define OS_INIT_EXPLICITLY           (0)
#endif

#ifndef   OS_TASK_LIST_HAS_PPREV
  #define OS_TASK_LIST_HAS_PPREV  OS_RR_SUPPORTED
#else
  #if (OS_TASK_LIST_HAS_PPREV == 0) && (OS_RR_SUPPORTED != 0)
    #error  "Invalid combination of config switches"
  #endif
#endif

#ifndef   OS_SUPPORT_CALL_ISR
  #define OS_SUPPORT_CALL_ISR     (1)
#endif

#ifndef   OS_SUPPORT_PERIPHERAL_POWER_CTRL
  #define OS_SUPPORT_PERIPHERAL_POWER_CTRL  (1)
#endif

#ifndef   OS_SUPPORT_TICK_HOOK
  #define OS_SUPPORT_TICK_HOOK    (1)
#endif

#ifndef   OS_POWER_NUM_COUNTERS
  #define OS_POWER_NUM_COUNTERS  (5u)
#endif

#ifndef   OS_SUPPORT_INT_PRIORITY
  #define OS_SUPPORT_INT_PRIORITY (1)
#endif

#ifndef   OS_IPL_DI_TYPE
  #define OS_IPL_DI_TYPE unsigned int
#endif

#ifndef   OS_IPL_EI_TYPE
  #define OS_IPL_EI_TYPE unsigned int
#endif

#ifndef   OS_SUPPORT_ENTER_INTERRUPT        // Some CPUs may implement a special version of OS_CallISR() written in assembler (For example F16)
  #define OS_SUPPORT_ENTER_INTERRUPT  (1)   // When Set to 0, OS_EnterInterrupt() is not defined, the generic version of OS_CallISR is also not defined then
#endif

#ifndef   OS_DISALLOW_EI_IN_CHANGETASK
  #define OS_DISALLOW_EI_IN_CHANGETASK  (0) // Used for embOS ports (like embOS Xtensa GCC) where nested interrupts are not possible
#endif

#ifndef   OS_TASK_HAS_PTLS                  // May be defined in OSChip.h to generate equal task structure for different embOS versions, even when TLS is not supported
  #define OS_TASK_HAS_PTLS OS_SUPPORT_PTLS
#endif

#ifndef   OS_TLS_PTR
  #define OS_TLS_PTR  void*
#endif

#ifndef   OS_SUPPORT_SYSSTACK_INFO
  #define OS_SUPPORT_SYSSTACK_INFO (0)
#endif

#ifndef   OS_SUPPORT_INTSTACK_INFO
  #define OS_SUPPORT_INTSTACK_INFO (0)
#endif

#ifndef   OS_SUPPORT_MPU
  #define OS_SUPPORT_MPU           (0)
#endif

#ifndef   OS_SECTION_ATTRIBUTE
  #define OS_SECTION_ATTRIBUTE
#endif

#ifndef   OS_SECURE_ATTRIBUTE
  #define OS_SECURE_ATTRIBUTE
#endif

#ifndef   OS_DATA_MEMORY_BARRIER
  #define OS_DATA_MEMORY_BARRIER()
#endif

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

/*********************************************************************
*
*       Check Compile time switches
*
**********************************************************************
*/
#ifndef OS_SIZEOF_INT
  #error "Please define OS_SIZEOF_INT (OSChip.h)"
#endif

#if (OS_STACK_AT_BOTTOM != 0) && (OS_STACK_AT_BOTTOM != 1)
  //
  //  Only 0 or 1 is allowed, because OS_STACK_AT_BOTTOM is used for calculations!
  //
  #error "OS_STACK_AT_BOTTOM has to be defined to 0 or 1"
#endif

#if (OS_SUPPORT_INT_PRIORITY != 0)
  #ifndef OS_IPL_EI_DEFAULT
    #error "Please define OS_IPL_EI_DEFAULT (OSChip.h)"
  #endif
  #ifndef OS_IPL_DI_DEFAULT
    #error "Please define OS_IPL_DI_DEFAULT (OSChip.h)"
  #endif
#endif

//
// OS_INTPRIO:  Typically no need to change
//
#ifndef   OS_INTPRIO_DISABLE
  #define OS_INTPRIO_DISABLE()      { OS_INTPRIO_SET(OS_Ipl_DI); }
#endif
#ifndef   OS_INTPRIO_GET_DISABLE
  #define OS_INTPRIO_GET_DISABLE(v) { OS_INTPRIO_GET(v); OS_INTPRIO_DISABLE(); }
#endif
#ifndef   OS_INTPRIO
  #define OS_INTPRIO  unsigned int
#endif

/*********************************************************************
*
*       Memory attributes
*
**********************************************************************
*/
#ifndef   OS_STACKPTR
  #define OS_STACKPTR
#endif

#ifndef   OS_SADDR
  #define OS_SADDR
#endif

/*********************************************************************
*
*       Basic type defines
*
**********************************************************************
*/
#ifndef   OS_I8
  #define OS_I8 signed char
#endif

#ifndef   OS_U8
  #define OS_U8 unsigned char
#endif

#ifndef   OS_I16
  #define OS_I16 signed short
#endif

#ifndef   OS_U16
  #define OS_U16 unsigned short
#endif

#ifndef   OS_I32
  #define OS_I32 long
#endif

#ifndef   OS_U32
  #define OS_U32 unsigned OS_I32
#endif

#ifdef    OS_I64
  #ifndef   OS_U64
    #define OS_U64 unsigned OS_I64
  #endif
#endif

//
// Defines an integer. This type is guaranteed to have at least 16 bits.
//
#ifndef   OS_INT
  #define OS_INT   int
#endif

#ifndef   OS_UINT
  #define OS_UINT  unsigned OS_INT
#endif

#ifndef   OS_TIME
  #define OS_TIME  int
#endif

#ifndef   OS_STAT
  #define OS_STAT  OS_U8
#endif

#ifndef   OS_BOOL
  #define OS_BOOL  OS_U8
#endif

//
// Since version 3.80k, the size of task events may be modified up to unsigned (which is 16bit or 32bit, depending on the CPU)
// If not defined in OSCHIP.h, the chip specific part of RTOS.h,
// OS_TASK_EVENT defaults to OS_U8 to be compatible to older versions.
// Since version 3.86f of embOS, OS_TASK_EVENT defaults to OS_U32 for 32bit CPUs (when not overwritten in OSCHIP.h)
//
#ifndef   OS_TASK_EVENT
  #if (OS_SIZEOF_INT >= 4)
    #define OS_TASK_EVENT OS_U32
  #else
    #define OS_TASK_EVENT OS_U8
  #endif
#endif

//
// Since version 4.04, the size of task priority may be modified up to unsigned (which is 16bit or 32bit, depending on the CPU)
// If not defined in OSCHIP.h OS_PRIO is set to OS_U32 when the size of an int is 32bit and is set to OS_U8
// when the size of an int is 16bit.
//
#ifndef OS_PRIO
  #if (OS_SIZEOF_INT >= 4)
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

// User 0..99  ***********************************

// Port 100..109 *********************************
#define OS_ERR_ISR_INDEX                      (100)
#define OS_ERR_ISR_VECTOR                     (101)
#define OS_ERR_ISR_PRIO                       (102)
#define OS_ERR_WRONG_STACK                    (103)
#define OS_ERR_ISR_NO_HANDLER                 (104)  // No interrupt handler was defined for this interrupt
#define OS_ERR_TLS_INIT                       (105)  // OS_TLS_Init() called multiple times from one task
#define OS_ERR_MB_BUFFER_SIZE                 (106)  // For 16bit CPUs, the maximum buffer size for a mailbox (64KB) exceeded

// OS generic ************************************
#define OS_ERR_EXTEND_CONTEXT                 (116)  // OS_ExtendTaskContext called multiple times from one task
#define OS_ERR_TIMESLICE                      (117)  // A timeslice value of 0 (illegal) was set. (Legal since embOS version 3.86f)
#define OS_ERR_INTERNAL                       (118)  // OS_ChangeTask called without RegionCnt set (or other internal error)
#define OS_ERR_IDLE_RETURNS                   (119)  // Idle loop should not return
#define OS_ERR_STACK                          (120)  // Stack overflow or invalid stack

// Counting semaphore overflow
#define OS_ERR_CSEMA_OVERFLOW                 (121)

// Peripheral Power management module
#define OS_ERR_POWER_OVER                     (122)
#define OS_ERR_POWER_UNDER                    (123)
#define OS_ERR_POWER_INDEX                    (124)

// System/interrupt stack
#define OS_ERR_SYS_STACK                      (125)  // embOS system stack overflow
#define OS_ERR_INT_STACK                      (126)  // Interrupt stack overflow

// invalid or non-initialized data structures
#define OS_ERR_INV_TASK                       (128)
#define OS_ERR_INV_TIMER                      (129)
#define OS_ERR_INV_MAILBOX                    (130)
#define OS_ERR_INV_CSEMA                      (132)
#define OS_ERR_INV_RSEMA                      (133)

// Using GetMail1 or PutMail1 or GetMailCond1 or PutMailCond1 on
// a non-1 byte mailbox
#define OS_ERR_MAILBOX_NOT1                   (135)

// Waitable objects deleted with waiting tasks or occupied by task
#define OS_ERR_MAILBOX_DELETE                 (136)
#define OS_ERR_CSEMA_DELETE                   (137)
#define OS_ERR_RSEMA_DELETE                   (138)

// internal errors, please contact SEGGER Microcontroller Systems
#define OS_ERR_MAILBOX_NOT_IN_LIST            (140)
#define OS_ERR_TASKLIST_CORRUPT               (142)

// Queue errors
#define OS_ERR_QUEUE_INUSE                    (143)
#define OS_ERR_QUEUE_NOT_INUSE                (144)
#define OS_ERR_QUEUE_INVALID                  (145)
#define OS_ERR_QUEUE_DELETE                   (146)

// Mailbox errors
#define OS_ERR_MB_INUSE                       (147)
#define OS_ERR_MB_NOT_INUSE                   (148)

// Not matching routine calls or macro usage
#define OS_ERR_UNUSE_BEFORE_USE               (150)
#define OS_ERR_LEAVEREGION_BEFORE_ENTERREGION (151)
#define OS_ERR_LEAVEINT                       (152)
#define OS_ERR_DICNT                          (153)
#define OS_ERR_INTERRUPT_DISABLED             (154)
#define OS_ERR_TASK_ENDS_WITHOUT_TERMINATE    (155)
#define OS_ERR_RESOURCE_OWNER                 (156)
#define OS_ERR_REGIONCNT                      (157)

#define OS_ERR_ILLEGAL_IN_ISR                 (160)  // Not a legal system call during interrupt
#define OS_ERR_ILLEGAL_IN_TIMER               (161)  // Not a legal system call during timer
#define OS_ERR_ILLEGAL_OUT_ISR                (162)  // Not a legal system call outside interrupt
#define OS_ERR_NOT_IN_ISR                     (163)  // OS_EnterInterrupt() has been called, but CPU is not in ISR state
#define OS_ERR_IN_ISR                         (164)  // OS_EnterInterrupt() has not been called, but CPU is in ISR state

#define OS_ERR_INIT_NOT_CALLED                (165)  // OS_InitKern() was not called

#define OS_ERR_CPU_STATE_ISR_ILLEGAL          (166)
#define OS_ERR_CPU_STATE_ILLEGAL              (167)
#define OS_ERR_CPU_STATE_UNKNOWN              (168)

// Double used data structures
#define OS_ERR_2USE_TASK                      (170)
#define OS_ERR_2USE_TIMER                     (171)
#define OS_ERR_2USE_MAILBOX                   (172)
#define OS_ERR_2USE_BSEMA                     (173)
#define OS_ERR_2USE_CSEMA                     (174)
#define OS_ERR_2USE_RSEMA                     (175)
#define OS_ERR_2USE_MEMF                      (176)

// Communication errors
#define OS_ERR_NESTED_RX_INT                  (180)

// Spinlock
#define OS_ERR_SPINLOCK_INV_CORE              (185)

// Fixed block memory pool
#define OS_ERR_MEMF_INV                       (190)
#define OS_ERR_MEMF_INV_PTR                   (191)
#define OS_ERR_MEMF_PTR_FREE                  (192)
#define OS_ERR_MEMF_RELEASE                   (193)
#define OS_ERR_MEMF_POOLADDR                  (194)
#define OS_ERR_MEMF_BLOCKSIZE                 (195)

// Task suspend / resume errors
#define OS_ERR_SUSPEND_TOO_OFTEN              (200)
#define OS_ERR_RESUME_BEFORE_SUSPEND          (201)

// Other task related errors
#define OS_ERR_TASK_PRIORITY                  (202)
#define OS_ERR_TASK_PRIORITY_INVALID          (203)

// Event object
#define OS_ERR_EVENT_INVALID                  (210)
#define OS_ERR_EVENT_DELETE                   (212)

// Waitlist (checked build)
#define OS_ERR_WAITLIST_RING                  (220)
#define OS_ERR_WAITLIST_PREV                  (221)
#define OS_ERR_WAITLIST_NEXT                  (222)

// Tick Hook
#define OS_ERR_TICKHOOK_INVALID               (223)
#define OS_ERR_TICKHOOK_FUNC_INVALID          (224)

// Other potential problems discovered in checked build
#define OS_ERR_NOT_IN_REGION                  (225)

// Cache related
#define OS_ERR_NON_ALIGNED_INVALIDATE         (230)

// System timer config related
#define OS_ERR_NON_TIMERCYCLES_FUNC           (235)
#define OS_ERR_NON_TIMERINTPENDING_FUNC       (236)

// embOS MPU related
#define OS_ERR_MPU_NOT_PRESENT                (240)
#define OS_ERR_MPU_INVALID_REGION             (241)
#define OS_ERR_MPU_INVALID_SIZE               (242)
#define OS_ERR_MPU_INVALID_PERMISSION         (243)
#define OS_ERR_MPU_INVALID_ALIGNMENT          (244)
#define OS_ERR_MPU_INVALID_OBJECT             (245)

// OS version mismatch between library and RTOS.h
#define OS_ERR_VERSION_MISMATCH               (253)

// OS special build, or internal
#define OS_ERR_TRIAL_LIMIT                    (254)

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
#define OS_TRACE_ID_DEACTIVATE                 (1u)
#define OS_TRACE_ID_ACTIVATE                   (2u)
#define OS_TRACE_ID_TIMERCALLBACK              (3u)
#define OS_TRACE_ID_DELAY                     (10u)
#define OS_TRACE_ID_DELAYUNTIL                (11u)
#define OS_TRACE_ID_SETPRIORITY               (12u)
#define OS_TRACE_ID_WAKETASK                  (13u)
#define OS_TRACE_ID_CREATETASK                (14u)
#define OS_TRACE_ID_TERMINATETASK             (15u)
#define OS_TRACE_ID_SUSPENDTASK               (16u)
#define OS_TRACE_ID_RESUMETASK                (17u)
#define OS_TRACE_ID_CREATETASK_EX             (18u)
#define OS_TRACE_ID_YIELD                     (19u)
#define OS_TRACE_ID_WAITEVENT                 (20u)
#define OS_TRACE_ID_WAITEVENT_TIMED           (21u)
#define OS_TRACE_ID_SIGNALEVENT               (22u)
#define OS_TRACE_ID_CLEAREVENTS               (23u)
#define OS_TRACE_ID_CREATEMB                  (30u)
#define OS_TRACE_ID_CLEARMB                   (31u)
#define OS_TRACE_ID_DELETEMB                  (32u)
#define OS_TRACE_ID_PUTMAIL                   (33u)
#define OS_TRACE_ID_GETMAIL                   (34u)
#define OS_TRACE_ID_PUTMAILCOND               (35u)
#define OS_TRACE_ID_GETMAILCOND               (36u)
#define OS_TRACE_ID_PUTMAIL_TIMED             (37u)
#define OS_TRACE_ID_GETMAIL_TIMED             (38u)
#define OS_TRACE_ID_WAITMAIL                  (39u)
#define OS_TRACE_ID_PUTMAIL1                  (40u)
#define OS_TRACE_ID_GETMAIL1                  (41u)
#define OS_TRACE_ID_PUTMAILCOND1              (42u)
#define OS_TRACE_ID_GETMAILCOND1              (43u)
#define OS_TRACE_ID_PUTMAIL_TIMED1            (44u)
#define OS_TRACE_ID_GETMAIL_TIMED1            (45u)
#define OS_TRACE_ID_PUTMAIL_F                 (46u)
#define OS_TRACE_ID_PUTMAIL_F1                (47u)
#define OS_TRACE_ID_PUTMAIL_FCOND             (48u)
#define OS_TRACE_ID_PUTMAIL_FCOND1            (49u)
#define OS_TRACE_ID_CREATERSEMA               (50u)
#define OS_TRACE_ID_USE                       (51u)
#define OS_TRACE_ID_UNUSE                     (52u)
#define OS_TRACE_ID_REQUEST                   (53u)
#define OS_TRACE_ID_GETSEMAVALUE              (54u)
#define OS_TRACE_ID_DELETE_RSEMA              (55u)
#define OS_TRACE_ID_WAITMAIL_TIMED            (59u)
#define OS_TRACE_ID_CREATECSEMA               (60u)
#define OS_TRACE_ID_DELETECSEMA               (61u)
#define OS_TRACE_ID_SIGNALCSEMA               (62u)
#define OS_TRACE_ID_WAITCSEMA                 (63u)
#define OS_TRACE_ID_WAITCSEMATIMED            (64u)
#define OS_TRACE_ID_SIGNALCSEMA_MAX           (65u)
#define OS_TRACE_ID_SET_CSEMAVALUE            (66u)
#define OS_TRACE_ID_CSEMAREQUEST              (67u)
#define OS_TRACE_ID_CREATETIMER               (70u)
#define OS_TRACE_ID_DELETETIMER               (71u)
#define OS_TRACE_ID_STARTTIMER                (72u)
#define OS_TRACE_ID_STOPTIMER                 (73u)
#define OS_TRACE_ID_RETRIGGERTIMER            (74u)
#define OS_TRACE_ID_SETTIMERPERIOD            (75u)
#define OS_TRACE_ID_CREATETIMER_EX            (76u)
#define OS_TRACE_ID_MALLOC                    (80u)
#define OS_TRACE_ID_FREE                      (81u)
#define OS_TRACE_ID_REALLOC                   (82u)
#define OS_TRACE_ID_MEMF_CREATE               (90u)
#define OS_TRACE_ID_MEMF_DELETE               (91u)
#define OS_TRACE_ID_MEMF_ALLOC                (92u)
#define OS_TRACE_ID_PREEMPT                  (128u)
#define OS_TRACE_ID_TASK_READY               (129u)
#define OS_TRACE_ID_TICK                     (130u)
#define OS_TRACE_ID_DELAYUS                  (131u)
#define OS_TRACE_ID_SUSPENDALL               (132u)
#define OS_TRACE_ID_RESUMEALL                (133u)
#define OS_TRACE_ID_WAITSEVENT               (134u)
#define OS_TRACE_ID_WAITSEVENT_TIMED         (135u)
#define OS_TRACE_ID_EVENTPULSE               (136u)
#define OS_TRACE_ID_EVENTRESET               (137u)
#define OS_TRACE_ID_EVENTSET                 (138u)
#define OS_TRACE_ID_EVENTWAIT                (139u)
#define OS_TRACE_ID_EVENTWAIT_TIMED          (140u)
#define OS_TRACE_ID_CREATE_EVENT             (141u)
#define OS_TRACE_ID_CREATE_EVENT_EX          (142u)
#define OS_TRACE_ID_DELETE_EVENT             (143u)
#define OS_TRACE_ID_PEEKMAIL                 (144u)
#define OS_TRACE_ID_CREATE_Q                 (145u)
#define OS_TRACE_ID_DELETE_Q                 (146u)
#define OS_TRACE_ID_Q_CLEAR                  (147u)
#define OS_TRACE_ID_Q_PUT                    (148u)
#define OS_TRACE_ID_Q_PUT_COND               (149u)
#define OS_TRACE_ID_Q_PUT_TIMED              (150u)
#define OS_TRACE_ID_Q_GET                    (151u)
#define OS_TRACE_ID_Q_GET_COND               (152u)
#define OS_TRACE_ID_Q_GET_TIMED              (153u)
#define OS_TRACE_ID_Q_PEEK                   (154u)
#define OS_TRACE_ID_Q_PURGE                  (155u)
#define OS_TRACE_ID_MEMF_ALLOCTIMED          (156u)
#define OS_TRACE_ID_MEMF_FREE                (157u)
#define OS_TRACE_ID_MEMF_RELEASE             (158u)
#define OS_TRACE_ID_MEMF_REQUEST             (159u)
#define OS_TRACE_ID_TS_CFG                   (160u)
#define OS_TRACE_ID_START_TRACE              (161u)
#define OS_TRACE_ID_OBJECTNAME               (162u)
#define OS_TRACE_ID_USE_TIMED                (163u)
#define OS_TRACE_ID_DEFINE_ISR               (164u)
#define OS_TRACE_ID_ISR_BEGIN                (165u)
#define OS_TRACE_ID_ISR_RESUME               (166u)
#define OS_TRACE_ID_SETTASKNAME              (167u)
#define OS_TRACE_ID_ADJUSTTIME               (168u)
#define OS_TRACE_ID_EVENTGET                 (169u)
#define OS_TRACE_ID_GETRESETMODE             (170u)
#define OS_TRACE_ID_SETRESETMODE             (171u)
#define OS_TRACE_ID_GETNUMIDLETICKS          (172u)
#define OS_TRACE_ID_GETNUMTASKS              (173u)
#define OS_TRACE_ID_GETTASKPRIO              (174u)
#define OS_TRACE_ID_GETSUSPENDCNT            (175u)
#define OS_TRACE_ID_MAIL_GETPTR              (176u)
#define OS_TRACE_ID_POWER_USAGEINC           (177u)
#define OS_TRACE_ID_POWER_USAGEDEC           (178u)
#define OS_TRACE_ID_POWER_GETMASK            (179u)
#define OS_TRACE_ID_SETINIT_SUSPENDCNT       (180u)
#define OS_TRACE_ID_GETTIME_US               (181u)
#define OS_TRACE_ID_GETTIME_US64             (182u)
#define OS_TRACE_ID_TICK_HANDLE_EX           (183u)
#define OS_TRACE_ID_TICK_HANDLE_NOHOOK       (184u)
#define OS_TRACE_ID_TICK_ADDHOOK             (185u)
#define OS_TRACE_ID_TICK_REMOVEHOOK          (186u)
#define OS_TRACE_ID_START_TICKLESS           (187u)
#define OS_TRACE_ID_STOP_TICKLESS            (188u)
#define OS_TRACE_ID_GETCSEMA_VALUE           (189u)
#define OS_TRACE_ID_GETEVENTS_OCCURRED       (190u)
#define OS_TRACE_ID_ISTASK                   (191u)
#define OS_TRACE_ID_Q_GETMSGCNT              (192u)
#define OS_TRACE_ID_Q_GETMSGSIZE             (193u)
#define OS_TRACE_ID_Q_ISUNUSE                (194u)
#define OS_TRACE_ID_SET_TIMESLICE            (195u)
#define OS_TRACE_ID_GET_RESOURCEOWNER        (196u)
#define OS_TRACE_ID_GET_TIMERPERIOD          (197u)
#define OS_TRACE_ID_GET_TIMERSTATUS          (198u)
#define OS_TRACE_ID_GET_TIMERVALUE           (199u)
#define OS_TRACE_ID_TIMING_START             (200u)
#define OS_TRACE_ID_TIMING_END               (201u)
#define OS_TRACE_ID_TIMING_GETUS             (202u)
#define OS_TRACE_ID_MAIL_GETPTRCOND          (203u)
#define OS_TRACE_ID_MAIL_PURGE               (204u)
#define OS_TRACE_ID_Q_PUT_EX                 (205u)
#define OS_TRACE_ID_Q_PUT_COND_EX            (206u)
#define OS_TRACE_ID_Q_PUT_TIMED_EX           (207u)
#define OS_TRACE_ID_SPINLOCK_CREATE          (208u)
#define OS_TRACE_ID_SPINLOCK_SW_CREATE       (209u)
#define OS_TRACE_ID_SPINLOCK_LOCK            (210u)
#define OS_TRACE_ID_SPINLOCK_SW_LOCK         (211u)
#define OS_TRACE_ID_SPINLOCK_UNLOCK          (212u)
#define OS_TRACE_ID_SPINLOCK_SW_UNLOCK       (213u)
#define OS_TRACE_ID_TRIGGERTIMER             (214u)
#define OS_TRACE_ID_ADDTERMINATEHOOK         (215u)
#define OS_TRACE_ID_REMOVEALLTERMINATEHOOKS  (216u)
#define OS_TRACE_ID_REMOVETERMINATEHOOK      (217u)
#define OS_TRACE_ID_CLEAREVENTS_EX           (218u)
#define OS_TRACE_ID_EVENTGETMASK             (219u)
#define OS_TRACE_ID_EVENTSETMASK             (220u)
#define OS_TRACE_ID_EVENTWAITMASK            (221u)
#define OS_TRACE_ID_EVENTWAITMASKTIMED       (222u)
#define OS_TRACE_ID_WD_ADD                   (223u)
#define OS_TRACE_ID_WD_CHECK                 (224u)
#define OS_TRACE_ID_WD_CONFIG                (225u)
#define OS_TRACE_ID_WD_REMOVE                (226u)
#define OS_TRACE_ID_WD_TRIGGER               (227u)
#define OS_TRACE_ID_EVENTSETMASKMODE         (228u)
#define OS_TRACE_ID_EVENTGETMASKMODE         (229u)

//
// SystemView API IDs start at offset 32 whereas embOSView IDs starts at offset 0.
// This offset is added for SystemView in order to maintain compatibility with embOSView.
//
#define OS_TRACE_API_OFFSET                   (32u)

//
// SystemView trace macros (could also be used for any other trace tool)
//
#if (OS_PROFILE != 0)
  #define TRACE_RECORD_TASK_INFO(pTask)                                      if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskInfo((pTask)); }
  #define TRACE_RECORD_API_VOID(Id)                                          if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordVoid ((Id) + OS_TRACE_API_OFFSET); }
  #define TRACE_RECORD_API_U32(Id, Para0)                                    if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32  ((Id) + OS_TRACE_API_OFFSET, (OS_U32)(Para0)); }
  #define TRACE_RECORD_API_PTR(Id, Para0)                                    if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32  ((Id) + OS_TRACE_API_OFFSET, OS_Global.pTrace->pfPtrToId((OS_U32)(Para0))); }
  #define TRACE_RECORD_API_U32X2(Id, Para0, Para1)                           if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x2((Id) + OS_TRACE_API_OFFSET, (OS_U32)(Para0), (OS_U32)(Para1)); }
  #define TRACE_RECORD_API_PTR_U32(Id, Para0, Para1)                         if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x2((Id) + OS_TRACE_API_OFFSET, OS_Global.pTrace->pfPtrToId((OS_U32)(Para0)), (OS_U32)(Para1)); }
  #define TRACE_RECORD_API_U32X3(Id, Para0, Para1, Para2)                    if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x3((Id) + OS_TRACE_API_OFFSET, (OS_U32)(Para0), (OS_U32)(Para1), (OS_U32)(Para2)); }
  #define TRACE_RECORD_API_PTR_U32X2(Id, Para0, Para1, Para2)                if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x3((Id) + OS_TRACE_API_OFFSET, OS_Global.pTrace->pfPtrToId((OS_U32)(Para0)), (OS_U32)(Para1), (OS_U32)(Para2)); }
  #define TRACE_RECORD_API_U32X4(Id, Para0, Para1, Para2, Para3)             if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x4((Id) + OS_TRACE_API_OFFSET, (OS_U32)(Para0), (OS_U32)(Para1), (OS_U32)(Para2), (OS_U32)(Para3)); }
  #define TRACE_RECORD_API_PTR_U32X3(Id, Para0, Para1, Para2, Para3)         if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x4((Id) + OS_TRACE_API_OFFSET, OS_Global.pTrace->pfPtrToId((OS_U32)(Para0)), (OS_U32)(Para1), (OS_U32)(Para2), (OS_U32)(Para3)); }
  #define TRACE_ON_ISR_ENTER()                                               if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordEnterISR(); }
  #define TRACE_ON_ISR_EXIT()                                                if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordExitISR(); }
  #define TRACE_ON_ISR_EXIT_TO_SCHEDULER()                                   if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordExitISRToScheduler(); }
  #define TRACE_ON_TASK_START_EXEC(TaskId)                                   if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskStartExec((OS_U32)(TaskId)); }
  #define TRACE_ON_TASK_STOP_EXEC()                                          if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskStopExec(); }
  #define TRACE_ON_TASK_START_READY(TaskId)                                  if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskStartReady((OS_U32)(TaskId)); }
  #define TRACE_ON_TASK_STOP_READY(TaskId, Para0)                            if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskStopReady((OS_U32)(TaskId), (unsigned int)(Para0)); }
  #define TRACE_ON_TASK_CREATE(TaskId)                                       if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskCreate((OS_U32)(TaskId)); }
  #define TRACE_ON_IDLE()                                                    if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordIdle(); }
  #define TRACE_ON_TIMER_ENTER(TimerId)                                      if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordEnterTimer((OS_U32)(TimerId)); }
  #define TRACE_ON_TIMER_EXIT()                                              if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordExitTimer(); }
#if (OS_TRACE_RECORD_API_END != 0)
  #define TRACE_RECORD_API_END(Id)                                           if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordEndCall((Id) + OS_TRACE_API_OFFSET); }
  #define TRACE_RECORD_API_END_U32(Id, Para0)                                if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordEndCallU32((Id) + OS_TRACE_API_OFFSET, (OS_U32)(Para0)); }
#else
  #define TRACE_RECORD_API_END(Id)
  #define TRACE_RECORD_API_END_U32(Id, Para0)
#endif
  #define TRACE_ON_TASK_TERMINATED(TaskId)                                   if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordTaskTerminate((OS_U32)(TaskId)); }
  #define TRACE_RECORD_API_PTR_U32X4(Id, Para0, Para1, Para2, Para3, Para4)  if (OS_Global.pTrace != NULL) {OS_Global.pTrace->pfRecordU32x5((Id) + OS_TRACE_API_OFFSET, OS_Global.pTrace->pfPtrToId((OS_U32)(Para0)), (OS_U32)(Para1), (OS_U32)(Para2), (OS_U32)(Para3), (OS_U32)(Para4)); }
#else
  #define TRACE_RECORD_TASK_INFO(pTask)
  #define TRACE_RECORD_API_VOID(Id)
  #define TRACE_RECORD_API_U32(Id, Para0)
  #define TRACE_RECORD_API_PTR(Id, Para0)
  #define TRACE_RECORD_API_U32X2(Id, Para0, Para1)
  #define TRACE_RECORD_API_PTR_U32(Id, Para0, Para1)
  #define TRACE_RECORD_API_U32X3(Id, Para0, Para1, Para2)
  #define TRACE_RECORD_API_PTR_U32X2(Id, Para0, Para1, Para2)
  #define TRACE_RECORD_API_U32X4(Id, Para0, Para1, Para2, Para3)
  #define TRACE_RECORD_API_PTR_U32X3(Id, Para0, Para1, Para2, Para3)
  #define TRACE_ON_ISR_ENTER()
  #define TRACE_ON_ISR_EXIT()
  #define TRACE_ON_ISR_EXIT_TO_SCHEDULER()
  #define TRACE_ON_TASK_START_EXEC(TaskId)
  #define TRACE_ON_TASK_STOP_EXEC()
  #define TRACE_ON_TASK_START_READY(TaskId)
  #define TRACE_ON_TASK_STOP_READY(TaskId, Para0)
  #define TRACE_ON_TASK_CREATE(TaskId)
  #define TRACE_ON_IDLE()
  #define TRACE_ON_TIMER_ENTER(TimerId)
  #define TRACE_ON_TIMER_EXIT()
  #define TRACE_RECORD_API_END(Id)
  #define TRACE_RECORD_API_END_U32(Id, Para0)
  #define TRACE_ON_TASK_TERMINATED(TaskId)
  #define TRACE_RECORD_API_PTR_U32X4(Id, Para0, Para1, Para2, Para3, Para4)
#endif

#define OS_TRACE_API_VOID(Id)        { OS_TRACE_VOID(Id);                \
                                       TRACE_RECORD_API_VOID(Id);        \
                                     }

#define OS_TRACE_API_U32(Id, Para0)  { OS_TRACE_DATA(Id, Para0);         \
                                       TRACE_RECORD_API_U32(Id, Para0);  \
                                     }


#define OS_TRACE_API_PTR(Id, Para0)  { OS_TRACE_PTR(Id, Para0);          \
                                       TRACE_RECORD_API_PTR(Id, Para0);  \
                                     }

/*********************************************************************
*
*       Assertions
*
**********************************************************************

  Assertions are used to generate code in the debug version
  of the OS in order catch programming faults like
  bad pointers or uninitialized data structures
*/

#if (OS_DEBUG != 0)
  #define OS_ASSERT(Exp, ErrCode)  { if (!(Exp)) {OS_Error(ErrCode); }}
#else
  #define OS_ASSERT(Exp, ErrCode)
#endif

#define OS_ASSERT_INT()          OS_ASSERT((OS_Global.InInt != 0u),    OS_ERR_ILLEGAL_OUT_ISR)
#define OS_ASSERT_NOINT()        OS_ASSERT((OS_Global.InInt == 0u),    OS_ERR_ILLEGAL_IN_ISR)
#define OS_ASSERT_NOTIMER()      OS_ASSERT((OS_InTimer == 0u),         OS_ERR_ILLEGAL_IN_TIMER)
#define OS_ASSERT_DICnt()        OS_ASSERT(((OS_DICnt & 0xF0u) == 0u), OS_ERR_DICNT)

#define OS_ASSERT_INIT_CALLED()  OS_ASSERT((OS_InitCalled != 0u),      OS_ERR_INIT_NOT_CALLED)

#if (OS_DEBUG != 0)
  //
  // OS_ASSERT_CPU_IN_ISR_MODE is typically called from OS_EnterInterrupt() and checks the hardware sate of the CPU
  //
  #define OS_ASSERT_CPU_IN_ISR_MODE()  OS_AssertCPUInISRMode()
#else
  #define OS_ASSERT_CPU_IN_ISR_MODE()
#endif

#ifdef __cplusplus
  extern "C" {
#endif

/*********************************************************************
*
*       Data structures
*
**********************************************************************
*/
typedef struct OS_GLOBAL_STRUCT       OS_GLOBAL;
typedef struct OS_TASK_STRUCT         OS_TASK;
typedef struct OS_WAIT_LIST_STRUCT    OS_WAIT_LIST;
typedef struct OS_WAIT_OBJ_STRUCT     OS_WAIT_OBJ;

typedef void voidRoutine         (void);
typedef void OS_RX_CALLBACK      (OS_U8 Data);
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
  OS_MPU_OBJTYPE_RSEMA,
  OS_MPU_OBJTYPE_CSEMA,
  OS_MPU_OBJTYPE_EVENT,
  OS_MPU_OBJTYPE_QUEUE,
  OS_MPU_OBJTYPE_MAILBOX,
  OS_MPU_OBJTYPE_SWTIMER,
  OS_MPU_OBJTYPE_MEMF,
  OS_MPU_OBJTYPE_TIMING
} OS_MPU_OBJTYPE;

#if (OS_SUPPORT_MPU != 0)

//
// embOS MPU macros and typedefs
//
#ifndef   MAX_MPU_REGIONS
  #define MAX_MPU_REGIONS  16u
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

#define OS_MPU_DATA_PERMISSION_MASK  0x03u

typedef enum {
  OS_MPU_THREAD_STATE_PRIVILEGED,
  OS_MPU_THREAD_STATE_UNPRIVILEGED
} OS_MPU_THREAD_STATE;

typedef enum {
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

typedef struct OS_MPU_DEBUG_STRUCT {
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

typedef struct OS_MPU_REGION_STRUCT {
  OS_U32 BaseAddr;
  OS_U32 Size;
  OS_U32 Permissions;
  OS_U32 Attributes;
} OS_MPU_REGION;

typedef struct OS_MPU_OBJ_STRUCT {
  OS_U32         BaseAddr;
  OS_MPU_OBJTYPE Type;
} OS_MPU_OBJ;

typedef struct OS_MPU_MEM_CONFIG_STRUCT {
  OS_U32 ROM_BaseAddr;
  OS_U32 ROM_Size;
  OS_U32 RAM_BaseAddr;
  OS_U32 RAM_Size;
  OS_U32 OS_BaseAddr;
  OS_U32 OS_Size;
} OS_MPU_MEM_CONFIG;

typedef struct OS_MPU_STRUCT {
  const OS_MPU_OBJ*    pAllowedObjects;           // Objects to which the task has access
  OS_MPU_REGION        Regions[MAX_MPU_REGIONS];  // Regions to have the task access
  OS_MPU_THREAD_STATE  ThreadState;               // Current task privileged state
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
} OS_MPU_API_LIST;

typedef void (*OS_MPU_DEVICE_DRIVER_FUNC) (void* p);
typedef void (*OS_MPU_ERROR_CALLBACK)     (OS_TASK* pTask, OS_MPU_ERRORCODE ErrorCode);

//
// Generic embOS-MPU API
//
void                OS_MPU_Enable               (void)                                                                                                                       OS_SECTION_ATTRIBUTE;
void                OS_MPU_EnableEx             (const OS_MPU_API_LIST* pAPIList)                                                                                            OS_SECTION_ATTRIBUTE;
void                OS_MPU_ConfigMem            (OS_U32 ROM_BaseAddr, OS_U32 ROM_Size, OS_U32 RAM_BaseAddr, OS_U32 RAM_Size, OS_U32 Unsecure_BaseAddr, OS_U32 Unsecure_Size) OS_SECTION_ATTRIBUTE;
void                OS_MPU_SetDeviceDriverList  (const OS_MPU_DEVICE_DRIVER_FUNC* p)                                                                                         OS_SECTION_ATTRIBUTE;
void                OS_MPU_SetAllowedObjects    (OS_TASK* pTask, const OS_MPU_OBJ* pObjList)                                                                                 OS_SECTION_ATTRIBUTE;
void                OS_MPU_AddRegion            (OS_TASK* pTask, OS_U32 BaseAddr, OS_U32 Size, OS_U32 Permissions, OS_U32 Attributes)                                        OS_SECTION_ATTRIBUTE;
void                OS_MPU_SetErrorCallback     (OS_MPU_ERROR_CALLBACK pFunc)                                                                                                OS_SECTION_ATTRIBUTE;
void                OS_MPU_SwitchToUnprivState  (void)                                                                                                                       OS_SECTION_ATTRIBUTE;
void                OS_MPU_SwitchToUnprivStateEx(voidRoutine* pRoutine, void OS_STACKPTR* pStack, OS_UINT StackSize)                                                         OS_SECTION_ATTRIBUTE;
void                OS_MPU_CallDeviceDriver     (OS_U32 Index, void* Param)                                                                                                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_MPU_THREAD_STATE OS_MPU_GetThreadState       (void)                                                                                                                       OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                OS_MPU_ExtendTaskContext    (void)                                                                                                                       OS_SECTION_ATTRIBUTE;
void                OS_MPU_Error                (OS_TASK* pTask, OS_MPU_ERRORCODE ErrorCode)                                                                                 OS_SECTION_ATTRIBUTE;

#endif // OS_SUPPORT_MPU

/**********************************************************************
*
*       OS_EXTEND_TASK_CONTEXT
*
*  This structure is used to define a save and restore function for
*  extension of the task context.
*  A pointer to this structure is part of the task control block
*  It is initialized by OS_ExtendTaskContext();
*
*       OS_EXTEND_TASK_CONTEXT_LINK
*
* This structure is used to have unlimited task context extensions as
* linked list. The last pNext item points to NULL.
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
  OS_TASK*             pNext;          // Points to the TCB of the next task in the list (with equal or lower priority). Needs to be first element !
  OS_REGS OS_STACKPTR* pStack;         // Typically contains the stack pointer if the task is suspended.                 Needs to be second element !
  // End of assembly relevant section
  OS_WAIT_LIST*        pWaitList;      // Points to a waitable object if task is suspended.
  OS_TIME              Timeout;
  OS_STAT              Stat;
  OS_PRIO              Priority;
  OS_PRIO              BasePrio;
  OS_TASK_EVENT        Events;         // Event storage
  OS_TASK_EVENT        EventMask;      // Event mask
  //
  // pPrev is required only in builds with doubly-linked task lists (Round Robin)
  //
#if (OS_TASK_LIST_HAS_PPREV != 0)
  OS_TASK*            pPrev;
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
  OS_UINT             StackSize;       // Stack size in bytes. Not required for functionality, just for analysis
  OS_U8 OS_STACKPTR*  pStackBot;       // First byte of stack. Not required for functionality, just for analysis
#endif
  //
  // Elements required with profiling or debug builds
  //
#if (OS_SUPPORT_STAT != 0)
  OS_U32              NumActivations;  // Counts how many times task has been activated
  OS_U32              NumPreemptions;  // Counts how many times task has been preempted
#endif
  //
  // Elements required with profiling builds
  //
#if (OS_PROFILE != 0)
  OS_U32              ExecTotal;       // Time spent executing
  OS_U32              ExecLast;        // Time spent executing (Reference)
  OS_U32              Load;            // Profiling builds only:
#endif
  //
  // Elements required with Round-robin functionality only. Typically available in all builds, but can be disabled at compile time
  //
#if (OS_RR_SUPPORTED != 0)
  OS_U8              TimeSliceRem;
  OS_U8              TimeSliceReload;
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
     OS_TLS_PTR      pTLS;
   #endif
#endif
  //
  // Optional embOS MPU
  //
#if OS_SUPPORT_MPU
  OS_MPU             MPU_Config;
#endif
  //
  // Elements required with debug builds
  //
#if (OS_DEBUG != 0)
  OS_U8             Id;                // Debug builds only: Id of this control block.
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
*       OS_RSEMA
*/
typedef struct OS_RSEMA_STRUCT OS_RSEMA;
struct OS_RSEMA_STRUCT {
  OS_WAIT_OBJ WaitObj;
  OS_TASK*    pTask;  // Owner
  OS_U8       UseCnt;
  OS_RSEMA*   pNext;
#if (OS_DEBUG != 0)
  OS_U8       Id;
#endif
};

/**********************************************************************
*
*       OS_CSEMA
*/
typedef struct OS_CSEMA_STRUCT OS_CSEMA;
struct OS_CSEMA_STRUCT {
  OS_WAIT_OBJ WaitObj;
  OS_UINT     Cnt;
#if (OS_DEBUG != 0)
  OS_CSEMA*   pNext;
  OS_U8       Id;
#endif
};

/**********************************************************************
*
*       OS_MAILBOX
*/
typedef struct OS_MAILBOX_STRUCT OS_MAILBOX;
struct OS_MAILBOX_STRUCT {
  OS_WAIT_OBJ      WaitObj;
#if (OS_LINK_MAILBOXES != 0)
  OS_MAILBOX*      pNext;
#endif
  char*            pData;
  volatile OS_UINT nofMsg;
  OS_UINT          maxMsg;
  OS_UINT          iRd;
  OS_U16           sizeofMsg;
  OS_BOOL          InUse;
#if (OS_DEBUG != 0)
  OS_U8            Id;
#endif
};

/**********************************************************************
*
*       OS_Q
*/
typedef struct OS_Q_STRUCT OS_Q;
struct OS_Q_STRUCT {
  OS_WAIT_OBJ WaitObj;
  OS_Q*       pNext;  // Ptr to next queue (for debugging / monitoring)
  OS_U8*      pData;
  OS_UINT     Size;
  OS_UINT     MsgCnt;
  OS_UINT     offFirst;
  OS_UINT     offLast;
  OS_BOOL     InUse;
  OS_UINT     InProgressCnt;
#if (OS_DEBUG != 0)
  OS_U8       Id;
#endif
};

/**********************************************************************
*
*       OS_Q_SRCLIST
*/
typedef struct {
  OS_CONST_PTR void* pSrc;
  OS_UINT            Size;
} OS_Q_SRCLIST;

/**********************************************************************
*
*       OS_MEMF
*/
#ifndef   OS_MEMF_MAX_ITEMS
  #define OS_MEMF_MAX_ITEMS  20
#endif

#define OS_MEMF_SIZEOF_BLOCKCONTROL  0  // sizeof(int) in future version for debugging

typedef struct OS_MEMF_STRUCT OS_MEMF;
struct OS_MEMF_STRUCT {
  OS_WAIT_OBJ WaitObj;
  OS_MEMF*    pNext;  // Ptr to next memory pool
  void*       pPool;
  OS_UINT     NumBlocks;
  OS_UINT     BlockSize;
  OS_UINT     NumFreeBlocks;
  OS_UINT     MaxUsed;
  void*       pFree;
#if (OS_DEBUG != 0)
  int         aiPurpose[OS_MEMF_MAX_ITEMS];
  OS_U8       Id;
#endif
};

/**********************************************************************
*
*       OS_EVENT
*/
typedef struct OS_EVENT_STRUCT OS_EVENT;

/****** Reset modes for OS_EVENT objects *****************************/

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
  OS_WAIT_OBJ         WaitObj;
  OS_TASK_EVENT       Signaled;
  OS_EVENT_RESET_MODE ResetMode;
  OS_EVENT_MASK_MODE  MaskMode;
#if (OS_DEBUG != 0)
  OS_U8               Id;
#endif
};

/**********************************************************************
*
*       OS_SPINLOCK / OS_SPINLOCK_SW
*/
#define   OS_SPINLOCK            volatile OS_UINT

#ifndef   OS_SPINLOCK_MAX_CORES
  #define OS_SPINLOCK_MAX_CORES  4u
#endif

typedef struct {
  volatile OS_U32 Entering[OS_SPINLOCK_MAX_CORES];
  volatile OS_U32 Number[OS_SPINLOCK_MAX_CORES];
} OS_SPINLOCK_SW;

/**********************************************************************
*
*       OS_WD - Watchdog support
*/
typedef struct OS_WD_STRUCT OS_WD;
struct OS_WD_STRUCT {
  OS_WD*  pNext;
  OS_TIME Reload;
  OS_TIME Timedex;
};

/**********************************************************************
*
*       OS_TRACE_ENTRY
*/
typedef struct {
  OS_U32  Time;
  void*   pCurrentTask;
  void*   p;
  OS_U32  v;
  OS_U8   iRout;
} OS_TRACE_ENTRY;

/*********************************************************************
*
*       SysTimer Config
*
**********************************************************************
*/
typedef struct  {
  OS_U32       TimerFreq;                      // e.g. 48000000 for 48MHz
  OS_U32       TickFreq;                       // typ. 1000 for 1 KHz system tick
  OS_BOOL      IsUpCounter;                    // 0: Down counter. Interrupt on underflow. 1: Up counter, interrupt on compare
  unsigned int (*pfGetTimerCycles)    (void);  // Callback function for reading HW timer value
  unsigned int (*pfGetTimerIntPending)(void);  // Callback function for reading timer interrupt pending state
} OS_SYSTIMER_CONFIG;

/*********************************************************************
*
*       Globals
*
**********************************************************************
*/

//
// Handle DEFinition (storage is actually allocted) versus DEClaration
// (reference) of RTOS variables depending upon who includes this header file.
//
#ifdef OSGLOBAL_C
  #define OS_EXTERN         // Define variables if included by OS_Kern.c
#else
  #define OS_EXTERN extern  // Declare variables if included by anyone else
#endif

/*****  Mark start of memory pragma area ****************************/

/* Some compilers can not deal with memory attributes and need pragmas */
#if defined(__ghs_zda)
  #pragma ghs startzda
#endif

/********************************************************************/

typedef union {
  int Dummy;             // Make sure a full integer (32 bit on 32 bit CPUs) is used.
  struct {
    volatile OS_U8 Region;
    volatile OS_U8 DI;
  } Cnt;
} OS_COUNTERS;

#ifndef PENDING_DUMMY_BYTES
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

typedef struct {
  //
  // Specific Trace Events
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
  // Generic Trace Event logging
  //
  void  (*pfRecordVoid)              (unsigned int Id);
  void  (*pfRecordU32)               (unsigned int Id, OS_U32 Para0);
  void  (*pfRecordU32x2)             (unsigned int Id, OS_U32 Para0, OS_U32 Para1);
  void  (*pfRecordU32x3)             (unsigned int Id, OS_U32 Para0, OS_U32 Para1, OS_U32 Para2);
  void  (*pfRecordU32x4)             (unsigned int Id, OS_U32 Para0, OS_U32 Para1, OS_U32 Para2, OS_U32 Para3);
  OS_U32(*pfPtrToId)                 (OS_U32 Ptr);
  void  (*pfRecordEnterTimer)        (OS_U32 TimerID);
  void  (*pfRecordExitTimer)         (void);
  void  (*pfRecordEndCall)           (unsigned int Id);
  void  (*pfRecordEndCallU32)        (unsigned int Id, OS_U32 Para0);
  void  (*pfRecordTaskTerminate)     (OS_U32 TaskId);
  void  (*pfRecordU32x5)             (unsigned int Id, OS_U32 Para0, OS_U32 Para1, OS_U32 Para2, OS_U32 Para3, OS_U32 Para4);
} OS_TRACE_API;

void OS_SetTraceAPI(const OS_TRACE_API* pTraceAPI) OS_SECTION_ATTRIBUTE;

struct OS_GLOBAL_STRUCT {
  OS_COUNTERS         Counters;
  OS_PENDING          Pending;
  OS_TASK*            pCurrentTask;      // Pointer to current Task
#if (OS_SUPPORT_INT_PRIORITY != 0)
  OS_IPL_DI_TYPE      Ipl_DI;
  OS_IPL_EI_TYPE      Ipl_EI;
#endif
  OS_TASK*            pTask;             // Linked list of all Tasks
  OS_TASK volatile* volatile pActiveTask;
#if (OS_SUPPORT_TIMER != 0)
  OS_TIMER*           pTimer;            // Linked list of all active Timers
  OS_TIMER*           pCurrentTimer;     // Actual expired timer which called callback
  OS_TIMER_HANDLER*   pfCheckTimer;      // Timer handler function, set when OS_StartTimer() is called
#endif // OS_SUPPORT_TIMER
  volatile OS_I32     Time;
  OS_TIME             TimeDex;
#ifdef OS_U64
  OS_U64              TickCnt;
#endif
#if (OS_RR_SUPPORTED != 0)
  OS_U8               TimeSlice;
  OS_U8               TimeSliceAtStart;
#endif
#if (OS_SUPPORT_TICKLESS != 0)
  OS_TIME             TicklessFactor;
  OS_BOOL             TicklessExpired;
  voidRoutine*        pfEndTicklessMode;
#endif
#if (OS_PROFILE != 0)
  const OS_TRACE_API* pTrace;
  OS_U8               ProfilingOn;
#endif
#if ((OS_DEBUG != 0) || (OS_TRACE != 0))
  OS_U8               InInt;             // Used in Debug builds of embOS only
#endif
#if OS_SUPPORT_MPU
  OS_MPU_MEM_CONFIG   MemConfig;        // RAM/ROM memory settings
#if (OS_DEBUG != 0)
  OS_MPU_DEBUG        MPUDebug;
#endif
#endif
};

//
// Translation macros for compatibility with older versions
//
#define OS_pTask               OS_Global.pTask
#define OS_pCurrentTask        OS_Global.pCurrentTask
#define OS_pActiveTask         OS_Global.pActiveTask
#define OS_pTimer              OS_Global.pTimer
#define OS_pCurrentTimer       OS_Global.pCurrentTimer
#define OS_Counters            OS_Global.Counters
#define OS_Pending             OS_Global.Pending
#define OS_Ipl_DI              OS_Global.Ipl_DI
#define OS_Ipl_EI              OS_Global.Ipl_EI
#define OS_Time                OS_Global.Time
#define OS_TimeDex             OS_Global.TimeDex
#if (OS_RR_SUPPORTED != 0)
  #define OS_TimeSlice         OS_Global.TimeSlice
  #define OS_TimeSliceAtStart  OS_Global.TimeSliceAtStart
#endif

#if (OS_COMPILER_STORAGE_MODIFIER_LEFT != 0)  // default

  OS_EXTERN OS_SADDR  volatile OS_U8   OS_Status;            //lint !e9075 MISRA C:2012 Rule 8.4, required
  OS_EXTERN OS_SADDR           OS_BOOL OS_Running;           //lint !e9075 MISRA C:2012 Rule 8.4, required
  OS_EXTERN OS_SADDR           OS_U8   OS_InitialSuspendCnt; //lint !e9075 MISRA C:2012 Rule 8.4, required

  #if (OS_SUPPORT_TICKSTEP != 0)
  OS_EXTERN OS_SADDR  volatile OS_U8   OS_TickStep;          //lint !e9075 MISRA C:2012 Rule 8.4, required
  OS_EXTERN OS_SADDR  volatile int     OS_TickStepTime;      //lint !e9075 MISRA C:2012 Rule 8.4, required
  #endif
  OS_EXTERN OS_SADDR           OS_UINT OS_TicksPerMS;        //lint !e9075 MISRA C:2012 Rule 8.4, required
  OS_EXTERN OS_SADDR           int     OS_IntTicksInc;       //lint !e9075 MISRA C:2012 Rule 8.4, required
  #if (OS_PROFILE != 0)
  OS_EXTERN OS_SADDR           OS_U32  OS_TS_ExecStart;      //lint !e9075 MISRA C:2012 Rule 8.4, required
  #endif
  #if (OS_DEBUG != 0)
  OS_EXTERN OS_SADDR           OS_U8   OS_InTimer;           //lint !e9075 MISRA C:2012 Rule 8.4, required
  OS_EXTERN OS_SADDR           OS_U8   OS_InitCalled;        //lint !e9075 MISRA C:2012 Rule 8.4, required
  #endif

  //***** Variables for CPU load measurement are declared global. This allows showing them in a debugger
  OS_EXTERN OS_SADDR volatile  OS_INT   OS_CPU_Load;         //lint !e9075 MISRA C:2012 Rule 8.4, required
  OS_EXTERN OS_SADDR volatile  OS_I32   OS_IdleCnt;          //lint !e9075 MISRA C:2012 Rule 8.4, required

  //
  // Pointers ***
  //  Unfortunately there is no real standard on how to define/declare
  //  pointers properly with extended memory attributes. There are
  //  currently 2 different standards, which we both support.
  //

  #if (OS_COMPILER_LOCATION_LEFT != 0)
    extern      OS_SADDR   OS_GLOBAL                          OS_Global;     // Major OS variables
    OS_EXTERN   OS_SADDR   OS_RSEMA*                          OS_pRSema;     //lint !e9075 MISRA C:2012 Rule 8.4, required
    OS_EXTERN   OS_SADDR   OS_Q*                              OS_pQHead;     //lint !e9075 MISRA C:2012 Rule 8.4, required
    #if (OS_LINK_MAILBOXES != 0)
      OS_EXTERN OS_SADDR   OS_MAILBOX*                        OS_pMailbox;   //lint !e9075 MISRA C:2012 Rule 8.4, required
    #endif
    #if (OS_DEBUG != 0)
      OS_EXTERN OS_SADDR   OS_CSEMA*                          OS_pCSema;     //lint !e9075 MISRA C:2012 Rule 8.4, required
    #endif
    OS_EXTERN   OS_SADDR   OS_MEMF*                           OS_pMEMF;      /* linked list of all fixed size memory pools */ /*lint !e9075 MISRA C:2012 Rule 8.4, required */
    OS_EXTERN   OS_SADDR   OS_TICK_HOOK*                      OS_pTickHook;  /* linked list of all tick hook functions */     /*lint !e9075 MISRA C:2012 Rule 8.4, required */
    #if (OS_SUPPORT_PTLS != 0)
      OS_EXTERN OS_SADDR   OS_TLS_PTR                         OS_pTLS;       // Global pointer to thread local storage
    #endif
    #if (OS_SUPPORT_SAVE_RESTORE_HOOK != 0)
      OS_EXTERN OS_SADDR   OS_EXTEND_TASK_CONTEXT*            OS_pDefaultTaskContextExtension;  //lint !e9075 MISRA C:2012 Rule 8.4, required
    #endif
  #else
    extern                 OS_GLOBAL                 OS_SADDR OS_Global;     // Major OS variables
    OS_EXTERN              OS_RSEMA*                 OS_SADDR OS_pRSema;
    OS_EXTERN              OS_Q*                     OS_SADDR OS_pQHead;

    #if (OS_LINK_MAILBOXES != 0)
      OS_EXTERN            OS_MAILBOX*               OS_SADDR OS_pMailbox;
    #endif

    #if (OS_DEBUG != 0)
      OS_EXTERN            OS_CSEMA*                 OS_SADDR OS_pCSema;
    #endif
    OS_EXTERN              OS_MEMF*                  OS_SADDR OS_pMEMF;      // linked list of all fixed size memory pools
    OS_EXTERN              OS_TICK_HOOK*             OS_SADDR OS_pTickHook;  // linked list of all tick hook functions
    #if (OS_SUPPORT_PTLS != 0)
      OS_EXTERN            OS_TLS_PTR                OS_SADDR OS_pTLS;       // Global pointer to thread local storage
    #endif
    #if (OS_SUPPORT_SAVE_RESTORE_HOOK != 0)
      OS_EXTERN            OS_EXTEND_TASK_CONTEXT*   OS_SADDR OS_pDefaultTaskContextExtension;
    #endif
  #endif

#else   //  OS_COMPILER_STORAGE_MODIFIER_LEFT == 0, needed for Keil 166
  #error "OS_COMPILER_STORAGE_MODIFIER_LEFT == 0 not supported in this version."
#endif  //  OS_COMPILER_STORAGE_MODIFIER_LEFT

//**** Mark end of memory pragma area *******************************
#if defined(__ghs_zda)
  #pragma ghs endzda
#endif

//*******************************************************************

#define OS_RegionCnt  OS_Global.Counters.Cnt.Region
#define OS_DICnt      OS_Global.Counters.Cnt.DI

/*********************************************************************
*
*       OS_CREATETASK macro
*       OS_CREATETASK_EX macro
*/
#if (OS_RR_SUPPORTED != 0)
  #define CTPARA_TIMESLICE ,2u
#else
  #define CTPARA_TIMESLICE
#endif

#if (OS_TRACKNAME != 0)
  #define OS_CREATETASK(pTask, Name, Hook, Priority, pStack)  \
  OS_CreateTask ((pTask),                                     \
                 (Name),                                      \
                 (OS_PRIO) (Priority),                        \
                 (Hook),                                      \
                 (void OS_STACKPTR*)(pStack),                 \
                 sizeof(pStack)                               \
                 CTPARA_TIMESLICE                             \
               )
#else
  #define OS_CREATETASK(pTask, Name, Hook, Priority, pStack)  \
  OS_CreateTask ((pTask),                                     \
                 (OS_PRIO) (Priority),                        \
                 (Hook),                                      \
                 (void OS_STACKPTR*)(pStack),                 \
                 sizeof(pStack)                               \
                 CTPARA_TIMESLICE                             \
               )
#endif

#if (OS_TRACKNAME != 0)
  #define OS_CREATETASK_EX(pTask, Name, Hook, Priority, pStack, pContext) \
  OS_CreateTaskEx  ((pTask),                                              \
                    (Name),                                               \
                    (Priority),                                           \
                    (Hook),                                               \
                    (void OS_STACKPTR*)(pStack),                          \
                    sizeof(pStack)                                        \
                    CTPARA_TIMESLICE,                                     \
                    (pContext)                                            \
               )
#else
  #define OS_CREATETASK_EX(pTask, Name, Hook, Priority, pStack, pContext) \
  OS_CreateTaskEx  ((pTask),                                              \
                    (Priority),                                           \
                    (Hook),                                               \
                    (void OS_STACKPTR*)(pStack),                          \
                    sizeof(pStack)                                        \
                    CTPARA_TIMESLICE,                                     \
                    (pContext)                                            \
               )
#endif

/*********************************************************************
*
*       OS_CreateTask
*       OS_CreateTaskEx
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

void OS_CreateTask  ( OS_TASK* pTask,
                      OS_CREATE_TASK_PARA_NAME
                      OS_PRIO Priority,
                      void (*pRoutine)(void),
                      void OS_STACKPTR* pStack,
                      OS_UINT StackSize
                      OS_CREATE_TASK_PARA_TS
                    ) OS_SECTION_ATTRIBUTE;

void OS_CreateTaskEx( OS_TASK* pTask,
                      OS_CREATE_TASK_PARA_NAME
                      OS_PRIO Priority,
                      void (*pRoutine)(void* pVoid),
                      void OS_STACKPTR* pStack,
                      OS_UINT StackSize
                      OS_CREATE_TASK_PARA_TS,
                      void* pContext
                    ) OS_SECTION_ATTRIBUTE;

#if (OS_SUPPORT_SAVE_RESTORE_HOOK != 0)
void OS_ExtendTaskContext   (const OS_EXTEND_TASK_CONTEXT* pExtendContext)                                                  OS_SECTION_ATTRIBUTE;
void OS_AddExtendTaskContext(OS_EXTEND_TASK_CONTEXT_LINK* pExtendContextLink, const OS_EXTEND_TASK_CONTEXT* pExtendContext) OS_SECTION_ATTRIBUTE;
#endif

/*********************************************************************
*
*       Task related routines
*
**********************************************************************
*/
void     OS_AddTerminateHook         (OS_ON_TERMINATE_HOOK* pHook, OS_ON_TERMINATE_FUNC* pfUser)  OS_SECTION_ATTRIBUTE;
void     OS_RemoveAllTerminateHooks  (void)                                                       OS_SECTION_ATTRIBUTE;
void     OS_RemoveTerminateHook      (const OS_ON_TERMINATE_HOOK* pHook)                          OS_SECTION_ATTRIBUTE;
void     OS_Delay                    (OS_TIME ms)                                                 OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_DelayUntil               (OS_TIME t)                                                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_Delayus                  (OS_U16  us)                                                 OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int      OS_GetNumTasks              (void)                                                       OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_PRIO  OS_GetPriority              (OS_CONST_PTR OS_TASK * pTask)                               OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U8    OS_GetSuspendCnt            (OS_CONST_PTR OS_TASK* pTask)                                OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL  OS_IsTask                   (OS_CONST_PTR OS_TASK* pTask)                                OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_Resume                   (OS_TASK* pTask)                                             OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_ResumeAllTasks           (void)                                                       OS_SECTION_ATTRIBUTE;
void     OS_SetTaskName              (OS_TASK* pTask, OS_ROM_DATA const char* s)                  OS_SECTION_ATTRIBUTE;
void     OS_SetPriority              (OS_TASK* pTask, OS_PRIO Priority)                           OS_SECTION_ATTRIBUTE;
void     OS_SetInitialSuspendCnt     (OS_U8 SuspendCnt)                                           OS_SECTION_ATTRIBUTE;
void     OS_Suspend                  (OS_TASK* pTask)                                             OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_SuspendAllTasks          (void)                                                       OS_SECTION_ATTRIBUTE;
OS_TASK* OS_TaskIndex2Ptr            (int TaskIndex)                                              OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_TerminateTask            (OS_TASK* pTask)                                             OS_SECTION_ATTRIBUTE;
void     OS_WakeTask                 (OS_TASK* pTask)                                             OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_Yield                    (void)                                                       OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#if (OS_SUSPEND_TASK_ON_TERMINATE != 0)  // Set task state of a terminated task to "suspended"
  #define OS_SUSPEND_TERMINATED_TASK(pTask) ((pTask)->Stat = 0x01)
#else
  #define OS_SUSPEND_TERMINATED_TASK(pTask)
#endif

/*********************************************************************
*
*       Info routines
*
**********************************************************************
*/

OS_ROM_DATA const char* OS_GetCPU    (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_ROM_DATA const char* OS_GetLibMode(void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_ROM_DATA const char* OS_GetModel  (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_ROM_DATA const char* OS_GetLibName(void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_UINT                 OS_GetVersion(void) OS_SECTION_ATTRIBUTE;

#define OS_GetType()            OS_GetLibMode()
#define OS_TASKID               OS_TASK*
#define OS_GetTaskID()          OS_Global.pCurrentTask
#define OS_GetpCurrentTask()    OS_Global.pCurrentTask
#define OS_IsRunning()          OS_Running

OS_BOOL OS_InInterrupt(void);

#if (OS_SUPPORT_TIMER != 0)
  #define OS_GetpCurrentTimer()   OS_Global.pCurrentTimer
  #define OS_GetpCurrentTimerEx() ((OS_TIMER_EX*)OS_Global.pCurrentTimer)
#endif  // OS_SUPPORT_TIMER

#if (OS_TRACKNAME != 0)
  OS_ROM_DATA const char* OS_GetTaskName(OS_CONST_PTR OS_TASK* pTask) OS_SECTION_ATTRIBUTE;
#else
  #define OS_GetTaskName(pt)  ("n/a")
#endif

#if (OS_CHECKSTACK != 0)
  unsigned int      OS_GetStackSpace(OS_CONST_PTR OS_TASK* pTask) OS_SECTION_ATTRIBUTE;
  unsigned int      OS_GetStackUsed (OS_CONST_PTR OS_TASK* pTask) OS_SECTION_ATTRIBUTE;
  unsigned int      OS_GetStackSize (OS_CONST_PTR OS_TASK* pTask) OS_SECTION_ATTRIBUTE;
  void OS_STACKPTR* OS_GetStackBase (OS_CONST_PTR OS_TASK* pTask) OS_SECTION_ATTRIBUTE;
#else
  #define OS_GetStackSpace(pt) 0u
  #define OS_GetStackUsed(pt)  0u
  #define OS_GetStackSize(pt)  0u
  #define OS_GetStackBase(pt)  0u
#endif

#if (OS_SUPPORT_SYSSTACK_INFO != 0)
  void OS_STACKPTR* OS_GetSysStackBase (void) OS_SECTION_ATTRIBUTE;
  unsigned int      OS_GetSysStackSize (void) OS_SECTION_ATTRIBUTE;
#else
  #define OS_GetSysStackBase()   0
  #define OS_GetSysStackSize()   0u
#endif

#if ((OS_CHECKSTACK != 0) && (OS_SUPPORT_SYSSTACK_INFO != 0))
  unsigned int OS_GetSysStackSpace(void) OS_SECTION_ATTRIBUTE;
  unsigned int OS_GetSysStackUsed (void) OS_SECTION_ATTRIBUTE;
#else
  #define OS_GetSysStackSpace()  0u
  #define OS_GetSysStackUsed()   0u
#endif

#if (OS_SUPPORT_INTSTACK_INFO != 0)
  void OS_STACKPTR* OS_GetIntStackBase(void) OS_SECTION_ATTRIBUTE;
  unsigned int      OS_GetIntStackSize(void) OS_SECTION_ATTRIBUTE;
#else
  #define OS_GetIntStackBase()   0
  #define OS_GetIntStackSize()   0u
#endif

#if ((OS_CHECKSTACK != 0) && (OS_SUPPORT_INTSTACK_INFO != 0))
  unsigned int OS_GetIntStackSpace(void) OS_SECTION_ATTRIBUTE;
  unsigned int OS_GetIntStackUsed (void) OS_SECTION_ATTRIBUTE;
#else
  #define      OS_GetIntStackSpace()  0u
  #define      OS_GetIntStackUsed()   0u
#endif

#if (OS_RR_SUPPORTED != 0)
  OS_U8 OS_SetTimeSlice(OS_TASK * pTask, OS_U8 TimeSlice) OS_SECTION_ATTRIBUTE;
  OS_U8 OS_GetTimeSliceRem(OS_CONST_PTR OS_TASK* pTask) OS_SECTION_ATTRIBUTE;
  #define OS_GetTimeSliceReload(pTask)  (pTask)->TimeSliceReload
#else
  #define OS_GetTimeSliceRem(pTask)     0u
  #define OS_GetTimeSliceReload(pTask)  0u
#endif

#if (OS_PROFILE >= 1)
  int    OS_STAT_GetLoad            (const OS_TASK *pTask) OS_SECTION_ATTRIBUTE;
  void   OS_STAT_Sample             (void)                 OS_SECTION_ATTRIBUTE;
  void   OS_STAT_Enable             (void)                 OS_SECTION_ATTRIBUTE;
  void   OS_STAT_Disable            (void)                 OS_SECTION_ATTRIBUTE;
  OS_U32 OS_STAT_GetTaskExecTime    (const OS_TASK *pTask) OS_SECTION_ATTRIBUTE;
#else
  #define OS_STAT_GetTaskExecTime(pTask)  0u
  #define OS_STAT_GetLoad(pTask)          0
  #define OS_STAT_Sample()
#endif

#if (OS_SUPPORT_STAT != 0)
  OS_U32 OS_STAT_GetNumActivations  (const OS_TASK *pTask) OS_SECTION_ATTRIBUTE;
  OS_U32 OS_STAT_GetNumPreemptions  (const OS_TASK *pTask) OS_SECTION_ATTRIBUTE;
#else
  #define OS_STAT_GetNumActivations(pTask)    0u
  #define OS_STAT_GetNumPreemptions(pTask)    0u
#endif

/*********************************************************************
*
*       Size info routines
*
**********************************************************************
*/
int OS_WAIT_OBJ_GetSize           (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_WAIT_LIST_GetSize          (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_EXTEND_TASK_CONTEXT_GetSize(void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_TASK_GetSize               (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_REGS_GetSize               (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_TICK_HOOK_GetSize          (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_RSEMA_GetSize              (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_CSEMA_GetSize              (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_MAILBOX_GetSize            (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_Q_GetSize                  (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_MEMF_GetSize               (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_EVENT_GetSize              (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int OS_TRACE_ENTRY_GetSize        (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

#if (OS_SUPPORT_TIMER != 0)
  int OS_TIMER_GetSize            (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
  int OS_TIMER_EX_GetSize         (void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#endif

/*********************************************************************
*
*       Critical regions
*
**********************************************************************
*/

//
// For some compiler it is necessary to use an OS_EnterRegion() function
// instead of the macro. To do so please add the following line to OSCHIP.h:
// #define OS_EnterRegion OS_EnterRegionFunc
//
#ifndef OS_EnterRegion
  #if (OS_DEBUG > 1)
    #define OS_EnterRegion() {if (OS_RegionCnt == 0xFFu) OS_Error(OS_ERR_REGIONCNT); else OS_RegionCnt++; }
  #else
    #define OS_EnterRegion() {OS_RegionCnt++; }
  #endif
#endif

//
// For some compiler (e.g. IAR EWRX with optimization) it is necessary to use an OS_EnterRegion() function
// instead of the macro for OS_EnterNestableInterrupt(). To do so please add the following line to OSCHIP.h:
// #define OS_IntEnterRegion OS_EnterRegionFunc
//
#ifndef   OS_IntEnterRegion
  #define OS_IntEnterRegion() {OS_RegionCnt++;}
#endif

void OS_EnterRegionFunc(void) OS_SECTION_ATTRIBUTE;
void OS_LeaveRegion    (void) OS_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Interrupt save/disable/restore macros
*
**********************************************************************
*/
#define OS_IncDI()       { OS_ASSERT_DICnt(); OS_DI(); OS_DICnt++; }
#define OS_DecRI()       { OS_ASSERT_DICnt();                                      \
                           OS_DICnt--;                                             \
                           if (OS_DICnt == 0u) {  /*lint !e548 No else required*/  \
                             OS_EI();                                              \
                           }                                                       \
                         }

#define OS_RESTORE_I()   { OS_ASSERT_DICnt();                                      \
                           if (OS_DICnt == 0u) {  /*lint !e548 No else required*/  \
                             OS_EI();                                              \
                           }                                                       \
                         }

void OS_RestoreI(void) OS_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Global interrupt save/disable/restore macros
*
*       The global OS_INTERRUPT_ functions are declared as MACRO.
*       If not defined in OSCHIP.h, these macros are translated
*       to a fucntion call and the function is declared here.
*       The functions have to be implemented in the CPU specific
*       sources OS_Priv.c or specific assembly file.
*       The macros may be overwritten in OSChip to be expanded
*       to intrinsic calls if applicable. This might be a very
*       efficient implementation.
*
**********************************************************************
*/
#ifndef OS_INTERRUPT_MaskGlobal
  OS_INTERWORK void OS_InterruptMaskGlobal(void) OS_SECTION_ATTRIBUTE;
  #define OS_INTERRUPT_MaskGlobal() OS_InterruptMaskGlobal()
#endif

#ifndef OS_INTERRUPT_UnmaskGlobal
  OS_INTERWORK void OS_InterruptUnmaskGlobal(void) OS_SECTION_ATTRIBUTE;
  #define OS_INTERRUPT_UnmaskGlobal() OS_InterruptUnmaskGlobal()
#endif

#ifndef OS_INTERRUPT_PreserveGlobal
  OS_INTERWORK void OS_InterruptPreserveGlobal(OS_U32 *pSave) OS_SECTION_ATTRIBUTE;
  #define OS_INTERRUPT_PreserveGlobal(p) OS_InterruptPreserveGlobal((p))
#endif

#ifndef OS_INTERRUPT_RestoreGlobal
  OS_INTERWORK void OS_InterruptRestoreGlobal(OS_U32 *pSave) OS_SECTION_ATTRIBUTE;
  #define OS_INTERRUPT_RestoreGlobal(p) OS_InterruptRestoreGlobal((p))
#endif

#ifndef OS_INTERRUPT_PreserveAndMaskGlobal
  OS_INTERWORK void OS_InterruptPreserveAndMaskGlobal(OS_U32 *pSave) OS_SECTION_ATTRIBUTE;
  #define OS_INTERRUPT_PreserveAndMaskGlobal(p) OS_InterruptPreserveAndMaskGlobal((p))
#endif

/*********************************************************************
*
*       Interrupt state preserve/restore macros
*
*       These macros preserve and restore the embOS interrupt status.
*       It can be used to temporarily disabled embOS interrupts without
*       changing the interrupt status which means that embOS interrupts
*       are again disabled when they were disabled before.
*       The actual macro is defined in OSCHIP.h.
*       For example:
*       OS_U32 p;
*       OS_INT_PRIO_PRESERVE(&p);
*       OS_DI();
*       DoSomething();
*       OS_INT_PRIO_RESTORE(&p);
*/
#ifndef OS_INT_PRIO_PRESERVE
  OS_INTERWORK void OS_IntPrioPreserve(OS_U32* p) OS_SECTION_ATTRIBUTE;
  #define OS_INT_PRIO_PRESERVE(p) OS_IntPrioPreserve(p)
#endif

#ifndef OS_INT_PRIO_RESTORE
  OS_INTERWORK void OS_IntPrioRestore(OS_U32* p) OS_SECTION_ATTRIBUTE;
  #define OS_INT_PRIO_RESTORE(p) OS_IntPrioRestore(p)
#endif

/*********************************************************************
*
*       ISR (Interrupt service routine) support
*
**********************************************************************
*/
#if ((OS_DEBUG != 0) || (OS_TRACE != 0))
  #define OS_MARK_IN_ISR()            { TRACE_ON_ISR_ENTER(); OS_Global.InInt++; }
  #define OS_MARK_OUTOF_ISR()         { TRACE_ON_ISR_EXIT();  if (OS_Global.InInt == 0u) {OS_Error(OS_ERR_LEAVEINT);} OS_Global.InInt--; }
  #define OS_MARK_OUTOF_ISR_SWITCH()  { TRACE_ON_ISR_EXIT_TO_SCHEDULER(); if (OS_Global.InInt == 0u) {OS_Error(OS_ERR_LEAVEINT);} OS_Global.InInt--; }
#else
  #define OS_MARK_IN_ISR()            { TRACE_ON_ISR_ENTER(); }
  #define OS_MARK_OUTOF_ISR()         { TRACE_ON_ISR_EXIT(); }
  #define OS_MARK_OUTOF_ISR_SWITCH()  { TRACE_ON_ISR_EXIT_TO_SCHEDULER(); }
#endif

#if (OS_SUPPORT_CALL_ISR != 0)      // Not allowed for some CPUs
void OS_CallISR        (void (*pRoutine)(void)) OS_SECTION_ATTRIBUTE;
void OS_CallNestableISR(void (*pRoutine)(void)) OS_SECTION_ATTRIBUTE;
#endif

#if (OS_SUPPORT_ENTER_INTERRUPT == 0) || ((OS_SWITCH_FROM_INT_MODIFIES_STACK == 0) && (OS_INTERRUPTS_ARE_NESTABLE_ON_ENTRY != 0) && (OS_SCHEDULER_ACTIVATED_BY_EXCEPTION == 0))
  //
  // FOR CPUs without separate interrupt stack which do not disable interrupts on entry,
  // OS_Enter- / Leave- Interrupt() is not defined.
  // OS_CallISR() has to be used
  //
#else

  #ifndef   OS_ENABLE_INTS_SAVE_IPL
    #define OS_ENABLE_INTS_SAVE_IPL() OS_EI()
  #endif

  #ifndef   OS_RESTORE_IPL
    #define OS_RESTORE_IPL()
  #endif

  #if (OS_INTERRUPTS_ARE_NESTABLE_ON_ENTRY != 0)
    #define OS_DI_ON_ENTRY() OS_DI()
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
        OS_RegionCnt--;                  \
      }
  #else
    #define OS_HANDLE_REGION_CNT_ON_LI() \
      } OS_RegionCnt--;
  #endif

  #ifndef   OS_DI_ON_LEAVE_NESTABLE
    #define OS_DI_ON_LEAVE_NESTABLE() OS_DI()
  #endif

  #ifndef   OS_EI_ON_LEAVE_NESTABLE
    #define OS_EI_ON_LEAVE_NESTABLE() OS_EI_ON_LEAVE()
  #endif

  #define OS_SWITCH_FROM_INT_IF_REQUIRED()                               \
    if ((OS_RegionCnt == (OS_U8) 1u) && (OS_Global.Pending.All != 0u)) { \
      OS_MARK_OUTOF_ISR_SWITCH();                                        \
      OS_SwitchFromInt();                                                \
    } else {                                                             \
      OS_MARK_OUTOF_ISR();

  #define OS_EnterInterrupt() {   \
    OS_MARK_IN_ISR();             \
    OS_ASSERT_CPU_IN_ISR_MODE();  \
    OS_DI_ON_ENTRY();         /* Disable low-prio interrupts if necessary: On CPUs, that keep interrupts enabled on ISR entyr. We disable all further low-prio interrupts. */    \
    OS_EI_HP_ON_ENTRY();      /* Enable high-prio interrupts if necessary: Some CPUs (such as M16C) disable all interrupts on ISR entry. We need to enable high prio Ints, using the global Int-Enable */     \
    OS_RegionCnt++;               \
    OS_DICnt++;                   \
  }

  #define OS_LeaveInterrupt() {                                                 \
    OS_DICnt--; /* Must have been zero initially ! (We could put =0 instead) */ \
    OS_SWITCH_FROM_INT_IF_REQUIRED()                                            \
    OS_HANDLE_REGION_CNT_ON_LI();                                               \
    OS_EI_ON_LEAVE();                                                           \
  }

  #define OS_LeaveInterruptNoSwitch() { \
    OS_MARK_OUTOF_ISR();                \
    OS_DICnt--;                         \
    OS_RegionCnt--;                     \
    OS_EI_ON_LEAVE();                   \
  }

  #define OS_EnterNestableInterrupt() { \
    OS_MARK_IN_ISR();                   \
    OS_IntEnterRegion();                \
    OS_ENABLE_INTS_SAVE_IPL();          \
    OS_ASSERT_CPU_IN_ISR_MODE();        \
  }

  #define OS_LeaveNestableInterrupt() { \
    OS_DI_ON_LEAVE_NESTABLE();          \
    OS_SWITCH_FROM_INT_IF_REQUIRED()    \
    OS_HANDLE_REGION_CNT_ON_LI();       \
    OS_RESTORE_IPL();                   \
    OS_EI_ON_LEAVE_NESTABLE();          \
  }

  #define OS_LeaveNestableInterruptNoSwitch() { \
    OS_DI();                                    \
    OS_MARK_OUTOF_ISR();                        \
    OS_RegionCnt--;                             \
    OS_RESTORE_IPL();                           \
    OS_EI_ON_LEAVE_NESTABLE();                  \
  }
#endif

#ifndef OS_EnterIntStack
  void OS__EnterIntStack(void) OS_SECTION_ATTRIBUTE;
  #define OS_EnterIntStack() {OS_DI(); OS__EnterIntStack(); OS_RESTORE_I(); }
#endif

#ifndef OS_LeaveIntStack
  void OS__LeaveIntStack(void) OS_SECTION_ATTRIBUTE;
  #define OS_LeaveIntStack() {OS_DI(); OS__LeaveIntStack(); }
#endif

void OS_SetFastIntPriorityLimit(OS_UINT Priority) OS_SECTION_ATTRIBUTE;

#if (OS_DEBUG != 0)
  void OS_AssertCPUInISRMode(void) OS_SECTION_ATTRIBUTE;
#endif

/*********************************************************************
*
*       Resource semaphores
*
**********************************************************************
*/
void     OS_CreateRSema     (OS_RSEMA* pRSema)                  OS_SECTION_ATTRIBUTE;
void     OS_DeleteRSema     (OS_RSEMA* pRSema)                  OS_SECTION_ATTRIBUTE;
int      OS_Use             (OS_RSEMA* pRSema)                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int      OS_UseTimed        (OS_RSEMA* pRSema, OS_TIME TimeOut) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void     OS_Unuse           (OS_RSEMA* pRSema)                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char     OS_Request         (OS_RSEMA* pRSema)                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int      OS_GetSemaValue    (OS_CONST_PTR OS_RSEMA* pRSema)     OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASK* OS_GetResourceOwner(OS_CONST_PTR OS_RSEMA* pRSema)     OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

#define  OS_CREATERSEMA(ps) OS_CreateRSema(ps)

/*********************************************************************
*
*       Counting semaphores
*
**********************************************************************
*/
void    OS_CreateCSema   (OS_CSEMA* pCSema, OS_UINT InitValue) OS_SECTION_ATTRIBUTE;
void    OS_DeleteCSema   (OS_CSEMA* pCSema)                    OS_SECTION_ATTRIBUTE;
int     OS_GetCSemaValue (OS_CONST_PTR OS_CSEMA* pCSema)       OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U8   OS_SetCSemaValue (OS_CSEMA* pCSema, OS_UINT Value)     OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_SignalCSema   (OS_CSEMA* pCSema)                    OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_SignalCSemaMax(OS_CSEMA* pCSema, OS_UINT MaxValue)  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_WaitCSema     (OS_CSEMA* pCSema)                    OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL OS_WaitCSemaTimed(OS_CSEMA* pCSema, OS_TIME TimeOut)   OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL OS_CSemaRequest  (OS_CSEMA* pCSema)                    OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

#define OS_CREATECSEMA(ps) OS_CreateCSema((ps), 0)

/*********************************************************************
*
*       Mailboxes
*
**********************************************************************
*/
void    OS_CreateMB         (OS_MAILBOX* pMB, OS_U16 sizeofMsg, OS_UINT maxnofMsg, void* Buffer) OS_SECTION_ATTRIBUTE;
void    OS_DeleteMB         (OS_MAILBOX* pMB)                                                    OS_SECTION_ATTRIBUTE;
void    OS_ClearMB          (OS_MAILBOX* pMB)                                                    OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_PutMail          (OS_MAILBOX* pMB, OS_CONST_PTR void* pMail)                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_PutMailCond      (OS_MAILBOX* pMB, OS_CONST_PTR void* pMail)                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_PutMailFront     (OS_MAILBOX* pMB, OS_CONST_PTR void* pMail)                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_PutMailFrontCond (OS_MAILBOX* pMB, OS_CONST_PTR void* pMail)                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL OS_PutMailTimed     (OS_MAILBOX* pMB, OS_CONST_PTR void* pMail, OS_TIME Timeout)         OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_PutMail1         (OS_MAILBOX* pMB, OS_CONST_PTR char* pMail)                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_PutMailCond1     (OS_MAILBOX* pMB, OS_CONST_PTR char* pMail)                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_PutMailFront1    (OS_MAILBOX* pMB, OS_CONST_PTR char* pMail)                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_PutMailFrontCond1(OS_MAILBOX* pMB, OS_CONST_PTR char* pMail)                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL OS_PutMailTimed1    (OS_MAILBOX* pMB, OS_CONST_PTR char* pMail, OS_TIME Timeout)         OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_GetMail          (OS_MAILBOX* pMB, void* pDest)                                       OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_GetMailCond      (OS_MAILBOX* pMB, void* pDest)                                       OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_GetMail1         (OS_MAILBOX* pMB, char* pDest)                                       OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_GetMailCond1     (OS_MAILBOX* pMB, char* pDest)                                       OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_GetMailTimed     (OS_MAILBOX* pMB, void* pDest, OS_TIME Timeout)                      OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_GetMailTimed1    (OS_MAILBOX* pMB, char* pDest, OS_TIME Timeout)                      OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_PeekMail         (OS_CONST_PTR OS_MAILBOX* pMB, void* pDest)                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_WaitMail         (OS_MAILBOX* pMB)                                                    OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_WaitMailTimed    (OS_MAILBOX* pMB, OS_TIME Timeout)                                   OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_Mail_GetPtr      (OS_MAILBOX* pMB, void** ppDest)                                     OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_Mail_GetPtrCond  (OS_MAILBOX* pMB, void** ppDest)                                     OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_Mail_Purge       (OS_MAILBOX* pMB)                                                    OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

#if (OS_DEBUG == 0)
  #define OS_GetMessageCnt(pMB) (*(pMB)).nofMsg
#else
  OS_UINT OS_GetMessageCnt(const OS_MAILBOX* pMB) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;  // Get number of available messages
#endif

/*********************************************************************
*
*       Message Queues
*
**********************************************************************
*/
void    OS_Q_Create        (OS_Q* pQ, void* pData, OS_UINT Size)                                            OS_SECTION_ATTRIBUTE;
void    OS_Q_Delete        (OS_Q* pQ)                                                                       OS_SECTION_ATTRIBUTE;
void    OS_Q_Clear         (OS_Q* pQ)                                                                       OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_Q_GetMessageCnt (OS_CONST_PTR OS_Q* pQ)                                                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_Q_Put           (OS_Q* pQ, OS_CONST_PTR void* pSrc, OS_UINT Size)                                OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_Q_PutBlocked    (OS_Q* pQ, OS_CONST_PTR void* pSrc, OS_UINT Size)                                OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_Q_PutTimed      (OS_Q* pQ, OS_CONST_PTR void* pSrc, OS_UINT Size, OS_TIME Timeout)               OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_Q_PutEx         (OS_Q* pQ, OS_CONST_PTR OS_Q_SRCLIST* pSrcList, OS_UINT NumSrc)                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_Q_PutBlockedEx  (OS_Q* pQ, OS_CONST_PTR OS_Q_SRCLIST* pSrcList, OS_UINT NumSrc)                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char    OS_Q_PutTimedEx    (OS_Q* pQ, OS_CONST_PTR OS_Q_SRCLIST* pSrcList, OS_UINT NumSrc, OS_TIME Timeout) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_Q_GetPtr        (OS_Q* pQ, void** ppData)                                                        OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_Q_GetPtrCond    (OS_Q* pQ, void** ppData)                                                        OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_Q_GetPtrTimed   (OS_Q* pQ, void** ppData, OS_TIME Timeout)                                       OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_Q_Purge         (OS_Q* pQ)                                                                       OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL OS_Q_IsInUse       (OS_CONST_PTR OS_Q* pQ)                                                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_Q_GetMessageSize(OS_CONST_PTR OS_Q* pQ)                                                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
int     OS_Q_PeekPtr       (OS_CONST_PTR OS_Q* pQ, void** ppData)                                           OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

#define OS_Q_SIZEOF_HEADER (sizeof(OS_INT))

/*********************************************************************
*
*       Events
*
**********************************************************************
*/
OS_TASK_EVENT OS_ClearEvents         (OS_TASK* pTask)                           OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASK_EVENT OS_ClearEventsEx       (OS_TASK* pTask, OS_TASK_EVENT EventMask)  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASK_EVENT OS_GetEventsOccurred   (OS_CONST_PTR OS_TASK* pTask)              OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void          OS_SignalEvent         (OS_TASK_EVENT Event, OS_TASK* pTask)      OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASK_EVENT OS_WaitEvent           (OS_TASK_EVENT EventMask)                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASK_EVENT OS_WaitEventTimed      (OS_TASK_EVENT EventMask, OS_TIME TimeOut) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASK_EVENT OS_WaitSingleEvent     (OS_TASK_EVENT EventMask)                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASK_EVENT OS_WaitSingleEventTimed(OS_TASK_EVENT EventMask, OS_TIME TimeOut) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

/*********************************************************************
*
*       Software timer
*
**********************************************************************
*/
#if (OS_SUPPORT_TIMER != 0)

#ifdef OS_SIZEOF_INT
  #if (OS_SIZEOF_INT == 2)
    #define OS_TIMER_MAX_TIME 0x7F00
  #elif (OS_SIZEOF_INT == 4)
    #define OS_TIMER_MAX_TIME 0x7FFFFF00
  #else
    #error "OS_SIZEOF_INT not correctly defined"
  #endif
#endif

void    OS_CreateTimer   (OS_TIMER* pTimer, OS_TIMERROUTINE* Callback, OS_TIME Timeout) OS_SECTION_ATTRIBUTE;
void    OS_DeleteTimer   (OS_TIMER* pTimer)                                             OS_SECTION_ATTRIBUTE;
void    OS_RetriggerTimer(OS_TIMER* pTimer)                                             OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_StartTimer    (OS_TIMER* pTimer)                                             OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_StopTimer     (OS_TIMER* pTimer)                                             OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_TriggerTimer  (OS_TIMER* pTimer)                                             OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TIME OS_GetTimerPeriod(OS_CONST_PTR OS_TIMER* pTimer)                                OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_BOOL OS_GetTimerStatus(OS_CONST_PTR OS_TIMER* pTimer)                                OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TIME OS_GetTimerValue (OS_CONST_PTR OS_TIMER* pTimer)                                OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_SetTimerPeriod(OS_TIMER* pTimer, OS_TIME Period)                             OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

#define OS_CREATETIMER(pTimer,c,d)          \
        OS_CreateTimer((pTimer), (c), (d)); \
        OS_StartTimer(pTimer);

/*********************************************************************
*
*       Extended timers
*
**********************************************************************
*/
void    OS_CreateTimerEx (OS_TIMER_EX* pTimerEx, OS_TIMER_EX_ROUTINE* Callback, OS_TIME Timeout, void* pData) OS_SECTION_ATTRIBUTE;

#define OS_RetriggerTimerEx(pTimerEx)         OS_RetriggerTimer(&(pTimerEx)->Timer)
#define OS_StartTimerEx(pTimerEx)             OS_StartTimer(&(pTimerEx)->Timer)
#define OS_StopTimerEx(pTimerEx)              OS_StopTimer(&(pTimerEx)->Timer)
#define OS_TriggerTimerEx(pTimerEx)           OS_TriggerTimer(&(pTimerEx)->Timer)
#define OS_DeleteTimerEx(pTimerEx)            OS_DeleteTimer(&(pTimerEx)->Timer)
#define OS_GetTimerPeriodEx(pTimerEx)         OS_GetTimerPeriod(&(pTimerEx)->Timer)
#define OS_GetTimerStatusEx(pTimerEx)         OS_GetTimerStatus(&(pTimerEx)->Timer)
#define OS_GetTimerValueEx(pTimerEx)          OS_GetTimerValue(&(pTimerEx)->Timer)
#define OS_SetTimerPeriodEx(pTimerEx,Period)  OS_SetTimerPeriod(&(pTimerEx)->Timer, (Period))

#define OS_CREATETIMER_EX(pTimerEx,cb,Timeout,pData)            \
        OS_CreateTimerEx((pTimerEx), (cb), (Timeout), (pData)); \
        OS_StartTimerEx(pTimerEx)

#endif // OS_SUPPORT_TIMER

/*********************************************************************
*
*       Heap type memory management
*
**********************************************************************

  This functions might not be implemented in all OS ports.
  Therefore declaration depends on condition OS_SUPPORT_OS_ALLOC
  which has to be defined in CPU specific part

*/
#if (OS_SUPPORT_OS_ALLOC != 0)
  void* OS_malloc   (unsigned int Size)                     OS_SECTION_ATTRIBUTE;
  void  OS_free     (void* pMemBlock)                       OS_SECTION_ATTRIBUTE;
  void* OS_realloc  (void* pMemBlock, unsigned int NewSize) OS_SECTION_ATTRIBUTE;
#endif

/*********************************************************************
*
*       Fixed Block memory management
*
**********************************************************************
*/
void    OS_MEMF_Create          (OS_MEMF* pMEMF, void* pPool, OS_UINT NumBlocks, OS_UINT BlockSize) OS_SECTION_ATTRIBUTE;
void    OS_MEMF_Delete          (OS_MEMF* pMEMF)                                                    OS_SECTION_ATTRIBUTE;
void*   OS_MEMF_Alloc           (OS_MEMF* pMEMF, int Purpose)                                       OS_SECTION_ATTRIBUTE;
void*   OS_MEMF_AllocTimed      (OS_MEMF* pMEMF, OS_TIME Timeout, int Purpose)                      OS_SECTION_ATTRIBUTE;
void*   OS_MEMF_Request         (OS_MEMF* pMEMF, int Purpose)                                       OS_SECTION_ATTRIBUTE;
void    OS_MEMF_Release         (OS_MEMF* pMEMF, void* pMemBlock)                                   OS_SECTION_ATTRIBUTE;
void    OS_MEMF_FreeBlock       (void* pMemBlock)                                                   OS_SECTION_ATTRIBUTE;
int     OS_MEMF_GetNumFreeBlocks(OS_CONST_PTR OS_MEMF* pMEMF)                                       OS_SECTION_ATTRIBUTE;
OS_BOOL OS_MEMF_IsInPool        (OS_CONST_PTR OS_MEMF* pMEMF, OS_CONST_PTR void* pMemBlock)         OS_SECTION_ATTRIBUTE;
int     OS_MEMF_GetMaxUsed      (OS_CONST_PTR OS_MEMF* pMEMF)                                       OS_SECTION_ATTRIBUTE;
int     OS_MEMF_GetNumBlocks    (OS_CONST_PTR OS_MEMF* pMEMF)                                       OS_SECTION_ATTRIBUTE;
int     OS_MEMF_GetBlockSize    (OS_CONST_PTR OS_MEMF* pMEMF)                                       OS_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Event object module
*
**********************************************************************
*/
void                      OS_EVENT_Create       (OS_EVENT* pEvent)                                           OS_SECTION_ATTRIBUTE;
void                      OS_EVENT_CreateEx     (OS_EVENT* pEvent, unsigned int Mode)                        OS_SECTION_ATTRIBUTE;
void                      OS_EVENT_Delete       (OS_EVENT* pEvent)                                           OS_SECTION_ATTRIBUTE;
OS_BOOL                   OS_EVENT_Get          (OS_CONST_PTR OS_EVENT* pEvent)                              OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASK_EVENT             OS_EVENT_GetMask      (OS_EVENT* pEvent, OS_TASK_EVENT EventMask)                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_EVENT_MASK_MODE        OS_EVENT_GetMaskMode  (OS_CONST_PTR OS_EVENT* pEvent)                              OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_EVENT_RESET_MODE       OS_EVENT_GetResetMode (OS_CONST_PTR OS_EVENT* pEvent)                              OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                      OS_EVENT_Pulse        (OS_EVENT* pEvent)                                           OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                      OS_EVENT_Reset        (OS_EVENT* pEvent)                                           OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                      OS_EVENT_Set          (OS_EVENT* pEvent)                                           OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                      OS_EVENT_SetMask      (OS_EVENT* pEvent, OS_TASK_EVENT EventMask)                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                      OS_EVENT_SetMaskMode  (OS_EVENT* pEvent, OS_EVENT_MASK_MODE MaskMode)              OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                      OS_EVENT_SetResetMode (OS_EVENT* pEvent, OS_EVENT_RESET_MODE ResetMode)            OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void                      OS_EVENT_Wait         (OS_EVENT* pEvent)                                           OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASK_EVENT             OS_EVENT_WaitMask     (OS_EVENT* pEvent, OS_TASK_EVENT EventMask)                  OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
char                      OS_EVENT_WaitTimed    (OS_EVENT* pEvent, OS_TIME Timeout)                          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_TASK_EVENT             OS_EVENT_WaitMaskTimed(OS_EVENT* pEvent, OS_TASK_EVENT EventMask, OS_TIME Timeout) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

/*********************************************************************
*
*       Timing and tickless support
*
**********************************************************************
*/
#define OS_TIMING OS_U32
void    OS_Timing_Start(OS_TIMING* pCycle)              OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_Timing_End  (OS_TIMING* pCycle)              OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U32  OS_Timing_Getus(OS_CONST_PTR OS_TIMING* pCycle) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#define OS_Timing_GetCycles(pPara) (*(pPara))

OS_TIME OS_GetNumIdleTicks  (void)                                OS_SECTION_ATTRIBUTE;
void    OS_AdjustTime       (OS_TIME Time)                        OS_SECTION_ATTRIBUTE;
void    OS_StartTicklessMode(OS_TIME Time, voidRoutine *Callback) OS_SECTION_ATTRIBUTE;
void    OS_StopTicklessMode (void)                                OS_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       OS peripheral power control
*
**********************************************************************
*/
#if (OS_SUPPORT_PERIPHERAL_POWER_CTRL != 0)
void    OS_POWER_UsageInc(OS_UINT Index) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
void    OS_POWER_UsageDec(OS_UINT Index) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_UINT OS_POWER_GetMask (void)          OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#endif

/*********************************************************************
*
*       Spinlock API
*
**********************************************************************
*/
void OS_SPINLOCK_Create   (OS_SPINLOCK*    pSpinlock)             OS_SECTION_ATTRIBUTE;
void OS_SPINLOCK_Lock     (OS_SPINLOCK*    pSpinlock)             OS_SECTION_ATTRIBUTE;
void OS_SPINLOCK_Unlock   (OS_SPINLOCK*    pSpinlock)             OS_SECTION_ATTRIBUTE;
void OS_SPINLOCK_SW_Create(OS_SPINLOCK_SW* pSpinlock)             OS_SECTION_ATTRIBUTE;
void OS_SPINLOCK_SW_Lock  (OS_SPINLOCK_SW* pSpinlock, OS_UINT Id) OS_SECTION_ATTRIBUTE;
void OS_SPINLOCK_SW_Unlock(OS_SPINLOCK_SW* pSpinlock, OS_UINT Id) OS_SECTION_ATTRIBUTE;

/*********************************************************************
*
*       Watchdog API
*
**********************************************************************
*/
void OS_WD_Add    (OS_WD* pWD, OS_TIME Timeout)                          OS_SECTION_ATTRIBUTE;
void OS_WD_Check  (void)                                                 OS_SECTION_ATTRIBUTE;
void OS_WD_Config (voidRoutine* pfTriggerFunc, voidRoutine* pfResetFunc) OS_SECTION_ATTRIBUTE;
void OS_WD_Remove (OS_CONST_PTR OS_WD* pWD)                              OS_SECTION_ATTRIBUTE;
void OS_WD_Trigger(OS_WD* pWD)                                           OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;

/*********************************************************************
*
*       CPU load measurement
*
**********************************************************************
*/
void OS_AddLoadMeasurement(int Period, OS_U8 AutoAdjust, OS_I32 DefaultMaxValue) OS_SECTION_ATTRIBUTE;
int  OS_GetLoadMeasurement(void)                                                 OS_SECTION_ATTRIBUTE;

#if (OS_U32_OP_IS_ATOMIC == 0)
  #define OS_INC_IDLE_CNT() \
  {                         \
    OS_DI();                \
    OS_IdleCnt++;           \
    OS_RESTORE_I();         \
  }
#else
  #define OS_INC_IDLE_CNT() (OS_IdleCnt++)
#endif

/*********************************************************************
*
*       Advanced profiling support
*
**********************************************************************
*/
#if (OS_PROFILE >= 1)
  void OS_EnableProfiling(int Period) OS_SECTION_ATTRIBUTE;
  #define OS_DisableProfiling() { OS_ProfilingOn = 0; }
#else
  #define OS_EnableProfiling(Period);
  #define OS_DisableProfiling()
#endif

/*********************************************************************
*
*       Trace support (embOSView)
*
**********************************************************************

Trace support is enabled by defining OS_TRACE = 1.
This is automatically done, when OS_LIBMODE_T is defined.

*/

#if (OS_TRACE != 0)
  //
  // Declare trace function prototypes
  //
  void OS_TraceVoid            (OS_U8 id)                      OS_SECTION_ATTRIBUTE;
  void OS_TracePtr             (OS_U8 id, void *p)             OS_SECTION_ATTRIBUTE;
  void OS_TraceData            (OS_U8 id, int v)               OS_SECTION_ATTRIBUTE;
  void OS_TraceDataPtr         (OS_U8 id, int v, void *p)      OS_SECTION_ATTRIBUTE;
  void OS_TraceU32Ptr          (OS_U8 id, OS_U32 p0, void *p1) OS_SECTION_ATTRIBUTE;
  void OS_TraceEnable          (void)                          OS_SECTION_ATTRIBUTE;
  void OS_TraceDisable         (void)                          OS_SECTION_ATTRIBUTE;
  void OS_TraceEnableAll       (void)                          OS_SECTION_ATTRIBUTE;
  void OS_TraceDisableAll      (void)                          OS_SECTION_ATTRIBUTE;
  void OS_TraceEnableId        (OS_U8 id)                      OS_SECTION_ATTRIBUTE;
  void OS_TraceDisableId       (OS_U8 id)                      OS_SECTION_ATTRIBUTE;
  void OS_TraceEnableFilterId  (OS_U8 FilterIndex, OS_U8 id)   OS_SECTION_ATTRIBUTE;
  void OS_TraceDisableFilterId (OS_U8 FilterIndex, OS_U8 id)   OS_SECTION_ATTRIBUTE;
  //
  // Trace function macros, used in API functions
  //
  #define OS_TRACE_VOID(id)                   OS_TraceVoid   (id)
  #define OS_TRACE_PTR(id, p)                 OS_TracePtr    ((id), (p))
  #define OS_TRACE_DATA(id, v)                OS_TraceData   ((id), ((int) (v)))
  #define OS_TRACE_DATA_PTR(id, v, p)         OS_TraceDataPtr((id), ((int) (v)), (p))
  #define OS_TRACE_U32_PTR(id, p0, p1)        OS_TraceU32Ptr ((id), (p0), (p1))

  #define OS_TRACE_START()                    { OS_EnterRegion();                                          }
  #define OS_TRACE_START_VOID(id)             { OS_EnterRegion(); OS_TraceVoid((id));                      }
  #define OS_TRACE_START_PTR(id, p)           { OS_EnterRegion(); OS_TracePtr((id), (p));                  }
  #define OS_TRACE_START_DATA(id, v)          { OS_EnterRegion(); OS_TraceData((id), ((int) (v)));         }
  #define OS_TRACE_START_DATA_PTR(id, v, p)   { OS_EnterRegion(); OS_TraceDataPtr((id), ((int) (v)), (p)); }
  #define OS_TRACE_START_U32_PTR(id, p0, p1)  { OS_EnterRegion(); OS_TraceU32Ptr((id), (u), (p));          }
  #define OS_TRACE_END()                      { OS_LeaveRegion();                                          }
#else
  //
  // Replace trace functions by empty macros if trace is not enabled
  //
  #define OS_TraceVoid(id)
  #define OS_TracePtr(id, p)
  #define OS_TraceData(id, v)
  #define OS_TraceDataPtr(id, v, p)
  #define OS_TraceU32Ptr(id, p0, p1)
  #define OS_TraceEnable()
  #define OS_TraceDisable()
  #define OS_TraceEnableAll()
  #define OS_TraceDisableAll()
  #define OS_TraceEnableId(id)
  #define OS_TraceDisableId(id)
  #define OS_TraceEnableFilterId(FilterIndex, id)
  #define OS_TraceDisableFilterId(FilterIndex, id)
  //
  // Trace function macros, used in API functions.
  // Expand to nothing if trace is disabled.
  //
  #define OS_TRACE_VOID(id)
  #define OS_TRACE_PTR(id, p)
  #define OS_TRACE_DATA(id, v)
  #define OS_TRACE_DATA_PTR(id, v, p)
  #define OS_TRACE_U32_PTR(id, p0, p1)

  #define OS_TRACE_START()
  #define OS_TRACE_START_VOID(id)
  #define OS_TRACE_START_PTR(id, p)
  #define OS_TRACE_START_DATA(id, v)
  #define OS_TRACE_START_DATA_PTR(id, v, p)
  #define OS_TRACE_START_U32_PTR(id, p0, p1)
  #define OS_TRACE_END()
#endif

/*********************************************************************
*
*       Extended task context
*
**********************************************************************
*/
void OS_SetDefaultTaskContextExtension(const OS_EXTEND_TASK_CONTEXT* pExtendContext) OS_SECTION_ATTRIBUTE;

/*********************************************************************
*
*        RTOSInit and misc.
*
**********************************************************************

The routines declared below are available on all targets.

*/

//
// Defines for embOSView communication
//
// These defines shall be used in RTOSInit.c, e.g.:
//   #ifndef   OS_VIEW_IFSELECT
//     #define OS_VIEW_IFSELECT  OS_VIEW_IF_JLINK
//   #endif
//
#define OS_VIEW_DISABLED      0u  // embOSView communication is disabled
#define OS_VIEW_IF_UART       1u  // If set, UART will be used for communication
#define OS_VIEW_IF_JLINK      2u  // If set, J-Link will be used for communication
#define OS_VIEW_IF_ETHERNET   3u  // If set, Ethernet will be used for communication
//
// Routines in RTOSInit.c
//
void   OS_InitHW          (void)            OS_SECTION_ATTRIBUTE;
void   OS_Idle            (void)            OS_SECTION_ATTRIBUTE;
OS_U32 OS_ConvertCycles2us(OS_U32 Cycles)   OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
OS_U32 OS_GetTime_Cycles  (void)            OS_SECTION_ATTRIBUTE;
void   OS_COM_Init        (void)            OS_SECTION_ATTRIBUTE;
void   OS_COM_Send1       (unsigned char c) OS_SECTION_ATTRIBUTE;
//
// Routines in OS_Error.c, delivered as source file
//
void  OS_Error(int ErrCode) OS_SECTION_ATTRIBUTE;

OS_INTERWORK void  OS_StartASM         (void)                                               OS_SECTION_ATTRIBUTE;
OS_INTERWORK int   OS_SwitchFromInt    (void)                                               OS_SECTION_ATTRIBUTE;
void               OS_InitKern         (void)                                               OS_SECTION_ATTRIBUTE;
OS_U8              OS_OnTx             (void)                                               OS_SECTION_ATTRIBUTE;
OS_INT             OS_COM_GetNextChar  (void)                                               OS_SECTION_ATTRIBUTE;
void               OS_OnRx             (OS_U8 Data)                                         OS_SECTION_ATTRIBUTE;
void               OS_Send1            (OS_U8 c)                                            OS_SECTION_ATTRIBUTE;
void               OS_SendString       (OS_ROM_DATA const char* s)                          OS_SECTION_ATTRIBUTE;
void               OS_SendStringFast   (OS_ROM_DATA const char* s)                          OS_SECTION_ATTRIBUTE;
OS_RX_CALLBACK*    OS_SetRxCallback    (OS_RX_CALLBACK* cb)                                 OS_SECTION_ATTRIBUTE;
OS_U8              OS_EvaPacketEx      (const OS_U8 pSrc[], OS_U16 SrcLen, OS_U8** pReturn) OS_SECTION_ATTRIBUTE;
void               OS_COM_ClearTxActive(void)                                               OS_SECTION_ATTRIBUTE;

#if (OS_DEBUG != 0)
  #define OS_Start()                      \
    if (OS_GetVersion() != OS_VERSION) {  \
      OS_Error(OS_ERR_VERSION_MISMATCH);  \
    }                                     \
    OS_Running = 1u;                      \
    OS_StartASM()
#else
  #define OS_Start()                      \
    OS_Running = 1u;                      \
    OS_StartASM()
#endif

/*********************************************************************
*
*        RTOS misc. optional
*
**********************************************************************

The routines declared below are available on some targets
only. (Because they do not make much sense on others)
*/

//
// Timing routines. Their existence depends on the CPU. In general,
// 8-bit CPUs require both routines, where 16-bit CPUs require one
//  and 32-bit CPUs require none of these.
//
#ifndef OS_GetTime
  OS_TIME OS_GetTime(void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#endif
#ifndef OS_GetTime32
  #if (OS_SIZEOF_INT == 4)
    #define OS_GetTime32() (OS_Global.Time)
  #else
    OS_I32 OS_GetTime32(void) OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
  #endif
#endif

#ifdef OS_U64
  void   OS_Config_SysTimer(const OS_SYSTIMER_CONFIG *pConfig) OS_SECTION_ATTRIBUTE;                      // Routine for setting up values for OS_GetTime_us()
  OS_U32 OS_GetTime_us     (void)                              OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;  // Routine for calculating current system time in usec
  OS_U64 OS_GetTime_us64   (void)                              OS_SECTION_ATTRIBUTE OS_SECURE_ATTRIBUTE;
#endif

void OS__di     (void) OS_SECTION_ATTRIBUTE;
void OS__ei     (void) OS_SECTION_ATTRIBUTE;
void OS__SaveIPL(void) OS_SECTION_ATTRIBUTE;
void OS__RestIPL(void) OS_SECTION_ATTRIBUTE;
void OS_SIM_Init(void) OS_SECTION_ATTRIBUTE;

//
// Standard tick handler which increment OS time variable by one on each interrupt
//
void OS_TICK_Handle      (void)                                               OS_SECTION_ATTRIBUTE;  // New version of OS_TickHandler() without Enter- Leave-Interrupt
void OS_TICK_HandleEx    (void)                                               OS_SECTION_ATTRIBUTE;
void OS_TICK_HandleNoHook(void)                                               OS_SECTION_ATTRIBUTE;  // Alternate faster tick handler without tick-hook-function
void OS_TICK_Config      (unsigned FractPerInt, unsigned FractPerTick)        OS_SECTION_ATTRIBUTE;
void OS_TICK_AddHook     (OS_TICK_HOOK* pHook, OS_TICK_HOOK_ROUTINE * pfUser) OS_SECTION_ATTRIBUTE;
void OS_TICK_RemoveHook  (OS_CONST_PTR OS_TICK_HOOK* pHook)                   OS_SECTION_ATTRIBUTE;

//
// Routines for the PC-version running in native mode (non-windows)
//
void OS_SetIntFreq(OS_UINT f) OS_SECTION_ATTRIBUTE;
void OS_Exit      (int code)  OS_SECTION_ATTRIBUTE;
void OS_UseDos    (void)      OS_SECTION_ATTRIBUTE;
void OS_UnuseDos  (void)      OS_SECTION_ATTRIBUTE;
int  OS_GetKey    (void)      OS_SECTION_ATTRIBUTE;
int  OS_WaitKey   (void)      OS_SECTION_ATTRIBUTE;

/*********************************************************************
*
*        Compatibility with manual and older embOS versions
*
**********************************************************************
*/
//
// Compatibility with manual
//
#define OS_CREATEMB                 OS_CreateMB
#define OS_GetUseCnt                OS_GetSemaValue
#define OS_WaitCSema_Timed          OS_WaitCSemaTimed
#define OS_WaitEvent_Timed          OS_WaitEventTimed
#define OS_GetEventsOccured         OS_GetEventsOccurred
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
#define OS_Terminate                OS_TerminateTask
//
// Compatibility with pre V4.30 versions, renamed function
//
#define OS_AddOnTerminateHook       OS_AddTerminateHook
#define OS_RemoveOnTerminateHooks   OS_RemoveAllTerminateHooks
#define OS_ResumeAllSuspendedTasks  OS_ResumeAllTasks

/*********************************************************************
*
*        Debug info for thread awareness
*
**********************************************************************
*
* Field offsets required by remote debugger for thread-awareness,
* i.e. for walking the task list.
*/
struct OS_DEBUG_INFO_STRUCT
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
};

typedef struct OS_DEBUG_INFO_STRUCT OS_DEBUG_INFO;

/********************************************************************/

#ifdef __cplusplus
  }
#endif

#endif /* RTOS_H_INCLUDED */

/*************************** End of file ****************************/
