/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co KG                  *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2014  SEGGER Microcontroller GmbH & Co KG         *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
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
*       OS version: 4.00                                             *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : RTOS.h
Purpose : Include file for the OS,
          to be included in every C-module accessing OS-routines

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
#include <string.h>         // required for memset() etc.
#include <intrinsics.h>     // required for __disable_interrupt() etc.

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
#define OS_PORT_REVISION          (0u)  // Port specific revision
#define OS_PTR_OP_IS_ATOMIC       (1)
#define OS_U32_OP_IS_ATOMIC       (1)
#define OS_GetTime()              (OS_Time)
#define OS_GetTime32()            (OS_Time)
#define OS_I32                    int
#define OS_SIZEOF_INT             (4)                        // Size of integer in bytes
#define OS_BIG_ENDIAN             (1 - __LITTLE_ENDIAN__)
#define OS_SUPPORT_SYSSTACK_INFO  (1)                        // Option to supply more info  to viewer
#define OS_SUPPORT_INTSTACK_INFO  (0)                        // Not used
#define OS_SUPPORT_OS_ALLOC       (1)                        // Define whether OS_Alloc is supported
#define OS_STACKFILL_CHAR         (0xcd)                     // To be compatible with IAR stack check plugin
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

/*****  End of configuration settings *******************************/

#if defined (__ARM7M__) && (__CORE__ == __ARM7M__)           // Cortex-M3
  #define OS_DI() OS_DisableInt()
  #define OS_EI() OS_EnableInt()
#elif defined (__ARM6M__) && (__CORE__ == __ARM6M__)         // Cortex-M0
  #define OS_DI() __disable_interrupt();                     // Use IAR Intrinsic functions
  #define OS_EI() __enable_interrupt();
#elif defined (__ARM7EM__) && (__CORE__ == __ARM7EM__)       // Cortex-M4/M4F
  #define OS_DI() OS_DisableInt()
  #define OS_EI() OS_EnableInt()
#else
  #error "Please check that __ARM7M__ or __ARM7EM__or __ARM6M__ is defined!"
#endif

#define OS_IPL_EI_DEFAULT 0                                  // Modification requires
#define OS_IPL_DI_DEFAULT 128                                // modification in RTOS*.s assembly files !

#define OS_U32 unsigned

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
#if (OS_CPU_HAS_VFP)
  // Cortex M4 / M4F with floating point unit.
  // The link register is part of the task context, because it contains information
  // whether extended stack frame is used or not.
  // It also fills up the stack frame to be 8 byte aligned.
typedef struct {
  OS_U32  Counters;
  OS_U32  OS_REG_R4;
  OS_U32  OS_REG_R5;
  OS_U32  OS_REG_R6;
  OS_U32  OS_REG_R7;
  OS_U32  OS_REG_R8;
  OS_U32  OS_REG_R9;
  OS_U32  OS_REG_R10;
  OS_U32  OS_REG_R11;
  OS_U32  OS_REG_LR;
  OS_U32  OS_REG_R0;
  OS_U32  OS_REG_R1;
  OS_U32  OS_REG_R2;
  OS_U32  OS_REG_R3;
  OS_U32  OS_REG_R12;
  OS_U32  OS_REG_R14;
  OS_U32  OS_REG_PC;
  OS_U32  OS_REG_XPSR;
} OS_REGS_BASE;

// The extended stack frame may be stored on exception entry.
// The FP registers are saved when extended task context is used
// for the specific task.
typedef struct {
  OS_U32  OS_REG_S0;
  OS_U32  OS_REG_S1;
  OS_U32  OS_REG_S2;
  OS_U32  OS_REG_S3;
  OS_U32  OS_REG_S4;
  OS_U32  OS_REG_S5;
  OS_U32  OS_REG_S6;
  OS_U32  OS_REG_S7;
  OS_U32  OS_REG_S8;
  OS_U32  OS_REG_S9;
  OS_U32  OS_REG_S10;
  OS_U32  OS_REG_S11;
  OS_U32  OS_REG_S12;
  OS_U32  OS_REG_S13;
  OS_U32  OS_REG_S14;
  OS_U32  OS_REG_S15;
  OS_U32  OS_REG_FPSCR;
  OS_U32  OS_REG_RES;
} OS_REGS_INT;

typedef struct {
  OS_REGS_BASE Base;
  OS_REGS_INT  Int;
} OS_REGS;

#else
  // Cortex M0 / M3 /M4 without floating point unit
typedef struct {
  OS_U32  Counters;
  OS_U32  OS_REG_R4;
  OS_U32  OS_REG_R5;
  OS_U32  OS_REG_R6;
  OS_U32  OS_REG_R7;
  OS_U32  OS_REG_R8;
  OS_U32  OS_REG_R9;
  OS_U32  OS_REG_R10;
  OS_U32  OS_REG_R11;
  OS_U32  OS_REG_LR;
  OS_U32  OS_REG_R0;
  OS_U32  OS_REG_R1;
  OS_U32  OS_REG_R2;
  OS_U32  OS_REG_R3;
  OS_U32  OS_REG_R12;
  OS_U32  OS_REG_R14;
  OS_U32  OS_REG_PC;
  OS_U32  OS_REG_XPSR;
} OS_REGS_BASE;

typedef struct {
  OS_REGS_BASE Base;
} OS_REGS;

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
void              OS_ARM_AssignISRSource   (int ISRIndex, int Source);
void              OS_ARM_EnableISRSource   (int SourceIndex);
void              OS_ARM_DisableISRSource  (int SourceIndex);
void              OS_DisableInt            (void);
void              OS_EnableInt             (void);
#if defined (__ARM6M__) && (__CORE__ == __ARM6M__)        // Cortex-M0
void              OS_memcpy                (void * pDest, const void * pSrc, int NumBytes);
#endif

#define OS_IDLE()                  // Overrides call of OS_Idle()
#define OS_EI_ON_LEAVE()  OS_EI()  // Required for CPUs which do not restore DI-flag by RETI.
#define OS_DI_ON_LEAVE_NESTABLE()  // With Cortex M it is not required to disable Ints in OS_LeaveNestableInterrupt()
#define OS_EI_ON_LEAVE_NESTABLE()  // With Cortex M it is not required to enable Ints in OS_LeaveNestableInterrupt()
#define OS_ENABLE_INTS_SAVE_IPL()  // Not required with Cortex-M, avoid call of OS_EI()

/*****  Declare functions used in the interrupt vector table in ROM */
void __program_start  (void);      // Program entry
void OS_Systick       (void);      // In RTOSInit w/o CMSIS
void OS_COM_IsrHandler(void);      // In RTOSInit w/o CMSIS

void HardFault_Handler(void);      // May be implemented in In RTOSInit, allows capture of HardFault exceptions
void OS_HardFaultHandler(void);    // In embOS libraries for Cortex M3/M4/M4F

/*********************************************************************
*
*       Thread locking, thread safe system libraries
*/
#if ((__VER__ >= 441) && (__VER__ < 6000000))
  void OS_InitSysLocks(void);
  #define OS_INIT_SYS_LOCKS()      OS_InitSysLocks()
#elif (__VER__ > 6000000)
  #define OS_INIT_SYS_LOCKS()
  /* embOS specific system locking functions called from the IAR hook functions */
  void OS__iar_system_Mtxinit(__iar_Rmtx *m);
  void OS__iar_system_Mtxdst(__iar_Rmtx *m);
  void OS__iar_system_Mtxlock(__iar_Rmtx *m);
  void OS__iar_system_Mtxunlock(__iar_Rmtx *m);
#endif  /* (__VER__ >= 441) && (__VER__ < 6000000) */

/*********************************************************************
*
*       Thread local storage
*/
#if (__VER__ >= 6000000)
  #define OS_TLS_PTR void _DLIB_TLS_MEMORY*
  #define OS_SUPPORT_TLS  (1)
  void OS_TLS_Init(void);          // Initialize Thread Local Storage, OS_TLS.c, CPU specific
  #define OS_ExtendTaskContext_TLS    OS_TLS_Init

  #if defined (__ARM7EM__) && (__CORE__ == __ARM7EM__) && defined (__ARMVFP__)

    typedef struct {
      unsigned int aVfpRegs[16];
      unsigned int VfpStatus;
    } OS_VFP_REGS_TEMP;

    void  OS_ExtendTaskContext_VFP     (void);
    void  OS_ExtendTaskContext_TLS_VFP (void);
    void  OS_VFP_SaveRegsTemp          (OS_VFP_REGS_TEMP* pVfpRegs);
    void  OS_VFP_RestoreRegsTemp       (const OS_VFP_REGS_TEMP* pVfpRegs);

    /* Helper for ISR-handler: Store temporary VFP registers onto stack   */
    #define OS_VFP_Save() {                    \
      OS_VFP_REGS_TEMP VfpRegsTemp;            \
      OS_VFP_SaveRegsTemp(&VfpRegsTemp)

    /* Helper for ISR-handler: Restore temporary VFP registers from stack */
    #define OS_VFP_Restore()                   \
      OS_VFP_RestoreRegsTemp(&VfpRegsTemp);    \
    }

  #endif
#endif

#define OS_TASK_HAS_PTLS  (1)      // pTLS always in task structure to get compatible TCBs for all versions

/*********************************************************************
*
*       Global interrupt disable/enable support
*       Implemented here, using intrinsics (works for M0/M3/M4 and M4F)
*/
#define OS_INTERRUPT_MaskGlobal()      __disable_interrupt()
#define OS_INTERRUPT_UnmaskGlobal()    __enable_interrupt()

#define OS_INTERRUPT_PreserveGlobal(p) (*(p) = (OS_U32) __get_PRIMASK())

#define OS_INTERRUPT_RestoreGlobal(p)  __set_PRIMASK(*(p))

#define OS_INTERRUPT_PreserveAndMaskGlobal(p)  { \
  (*(p) = (OS_U32) __get_PRIMASK());             \
  __disable_interrupt();                         \
}

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
This file (original name OS_RAW.h) is part of RTOS.h, the include
file for the OS.
*/

#if (!defined(OS_LIBMODE_XR) && !defined(OS_LIBMODE_R)  &&   \
     !defined(OS_LIBMODE_S)  && !defined(OS_LIBMODE_SP) &&   \
     !defined(OS_LIBMODE_D)  && !defined(OS_LIBMODE_DP) &&   \
     !defined(OS_LIBMODE_DT))
#include "OS_Config.h"
#endif

#define OS_VERSION_GENERIC (40000u)       /*  Generic version 4.00 */

#ifndef   OS_PORT_REVISION
  #define OS_PORT_REVISION 0u
#endif

#define OS_VERSION (OS_VERSION_GENERIC + (OS_PORT_REVISION * 25u))


/****** Thread local storage, may be enabled in CPU specific part ***/

#ifndef   OS_SUPPORT_TLS
  #define OS_SUPPORT_TLS    0
#endif

/****** embOS software timer may be excluded when compiling sources */

#ifndef   OS_SUPPORT_TIMER
  #define OS_SUPPORT_TIMER  1
#endif

/****** embOS tickless support may be excluded when compiling sources */

#ifndef   OS_SUPPORT_TICKLESS
  #define OS_SUPPORT_TICKLESS  1
#endif

/****** embOS default debug level is 1, may be overwritten in project settings */

#ifndef   OS_DEBUG_LEVEL
  #define OS_DEBUG_LEVEL    1
#endif

/*********************************************************************
*
*       Defines for library types
*
**********************************************************************
*/

/* If library type is not selected yet (on project level,
   for example), then select DP by default
*/
#if (!defined(OS_LIBMODE_XR) && !defined(OS_LIBMODE_R)  &&   \
     !defined(OS_LIBMODE_S)  && !defined(OS_LIBMODE_SP) &&   \
     !defined(OS_LIBMODE_D)  && !defined(OS_LIBMODE_DP) &&   \
     !defined(OS_LIBMODE_DT))
#define OS_LIBMODE_DP
#endif

#if defined(OS_LIBMODE_XR)                        // Extremely small release - without Round robin support
  #define OS_CHECKSTACK       0
  #define OS_DEBUG            0
  #define OS_PROFILE          0u
  #define OS_SUPPORT_TICKSTEP 0
  #define OS_TRACE            0u
  #define OS_RR_SUPPORTED     0
  #define OS_TRACKNAME        0
  #define OS_SUPPORT_SAVE_RESTORE_HOOK 0
  #define OS_CreateTask   OS_CreateTask_XR
  #define OS_CreateTaskEx OS_CreateTaskEx_XR
  #define OS_LIBMODE "XR"
  #ifndef   OS_SUPPORT_STAT
    #define OS_SUPPORT_STAT   0
  #endif
  #define OS_SUPPORT_PTLS     0
#elif defined(OS_LIBMODE_R)                       // Release build
  #define OS_CHECKSTACK       0
  #define OS_DEBUG            0
  #define OS_PROFILE          0u
  #define OS_SUPPORT_TICKSTEP 0
  #define OS_TRACE            0u
  #define OS_CreateTask   OS_CreateTask_R
  #define OS_CreateTaskEx OS_CreateTaskEx_R
  #define OS_LIBMODE "R"
  #ifndef   OS_SUPPORT_STAT
    #define OS_SUPPORT_STAT   0
  #endif
  #define OS_SUPPORT_PTLS     OS_SUPPORT_TLS
#elif defined(OS_LIBMODE_S)                       // Release build with stack check
  #define OS_CHECKSTACK       1
  #define OS_DEBUG            0
  #define OS_PROFILE          0u
  #define OS_SUPPORT_TICKSTEP 0
  #define OS_TRACE            0u
  #define OS_CreateTask   OS_CreateTask_S
  #define OS_CreateTaskEx OS_CreateTaskEx_S
  #define OS_LIBMODE "S"
  #ifndef   OS_SUPPORT_STAT
    #define OS_SUPPORT_STAT   0
  #endif
  #define OS_SUPPORT_PTLS     OS_SUPPORT_TLS
#elif defined(OS_LIBMODE_SP)                      // Release build with stack check and profiling
  #define OS_CHECKSTACK       1
  #define OS_DEBUG            0
  #define OS_PROFILE          1u
  #define OS_SUPPORT_TICKSTEP 1
  #define OS_TRACE            0u
  #define OS_CreateTask   OS_CreateTask_SP
  #define OS_CreateTaskEx OS_CreateTaskEx_SP
  #define OS_LIBMODE "SP"
  #ifndef   OS_SUPPORT_STAT
    #define OS_SUPPORT_STAT   1
  #endif
  #define OS_SUPPORT_PTLS     OS_SUPPORT_TLS
#elif defined(OS_LIBMODE_D)                       // Debug build
  #define OS_CHECKSTACK       1
  #define OS_DEBUG            OS_DEBUG_LEVEL
  #define OS_PROFILE          0u
  #define OS_SUPPORT_TICKSTEP 1
  #define OS_TRACE            0u
  #define OS_CreateTask   OS_CreateTask_D
  #define OS_CreateTaskEx OS_CreateTaskEx_D
  #define OS_LIBMODE "D"
  #ifndef   OS_SUPPORT_STAT
    #define OS_SUPPORT_STAT   1
  #endif
  #define OS_SUPPORT_PTLS     OS_SUPPORT_TLS
#elif defined(OS_LIBMODE_DP)                      // Debug build with profiling
  #define OS_CHECKSTACK       1
  #define OS_PROFILE          1u
  #define OS_DEBUG            OS_DEBUG_LEVEL
  #define OS_SUPPORT_TICKSTEP 1
  #define OS_TRACE            0u
  #define OS_CreateTask   OS_CreateTask_DP
  #define OS_CreateTaskEx OS_CreateTaskEx_DP
  #define OS_LIBMODE "DP"
  #ifndef   OS_SUPPORT_STAT
    #define OS_SUPPORT_STAT   1
  #endif
  #define OS_SUPPORT_PTLS     OS_SUPPORT_TLS
#elif defined(OS_LIBMODE_DT)                      // Debug build with profiling and trace
  #define OS_CHECKSTACK       1
  #define OS_DEBUG            OS_DEBUG_LEVEL
  #define OS_PROFILE          1u
  #define OS_SUPPORT_TICKSTEP 1
  #define OS_TRACE            1u
  #define OS_CreateTask   OS_CreateTask_DT
  #define OS_CreateTaskEx OS_CreateTaskEx_DT
  #define OS_LIBMODE "DT"
  #ifndef   OS_SUPPORT_STAT
    #define OS_SUPPORT_STAT   1
  #endif
  #define OS_SUPPORT_PTLS     OS_SUPPORT_TLS
#else
  #error Please define library type used !
#endif

/*********************************************************************
*
*       OS compile time switches
*
**********************************************************************

These compile time switches can be modified when recompiling
the library. Usually, the values are defined in OSCHIP.h, which is then
merged with OS_RAW.h to form RTOS.h.
If the values are not defined in OSCHIP.h, the default values
below are used.
*/

/* Make it possible to place const data in RAM via
   define and therefore via command line. This is required
   for some smaller CPUs in small memory models
   It is defined to be "const" by default, but can be defined to nothing
   by simply adding a -DOS_CONST_DATA on the commandline
*/

#ifndef   OS_CONST_DATA
  #define OS_CONST_DATA const  /* Default      */
#else
  #undef  OS_CONST_DATA
  #define OS_CONST_DATA        /* define blank */
#endif

#ifndef   OS_CONST_PTR
  #define OS_CONST_PTR  const  /* Default: const pointer declared as const  */
#else
  #undef  OS_CONST_PTR         /* May have been overwritten for CPUs where "const" memory addresses ROM instead of RAM */
  #define OS_CONST_PTR         /* define blank */
#endif

#ifndef   OS_ROM_DATA
  #define OS_ROM_DATA
#endif

#ifndef   OS_COPYSTRING       /* Used in OSCom.c */
  #define OS_COPYSTRING       OS_MEMCPY
#endif

#ifndef   OS_INTERWORK
  #define OS_INTERWORK
#endif

#ifndef   OS_SCHEDULER_ACTIVATED_BY_EXCEPTION
  #define OS_SCHEDULER_ACTIVATED_BY_EXCEPTION   (0)
#endif

#ifndef   OS_STACK_ADR
  #define OS_STACK_ADR OS_U32
#endif

#ifndef   OS_MEMSET
  #define OS_MEMSET(a,v,s) memset((a),((int) (v)),(s))
#endif

#ifndef   OS_MEMCPY
  #define OS_MEMCPY(dest,src,cnt) memcpy((dest),(src),(cnt))
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

#ifndef   OS_SUPPORT_TICKSTEP
  #define OS_SUPPORT_TICKSTEP 0
#endif

#ifndef   OS_TRACKNAME
  #define OS_TRACKNAME (1)
#endif

#ifndef   OS_TRACE
  #define OS_TRACE (0u)
#endif

#ifndef   OS_SUPPORT_SAVE_RESTORE_HOOK
  #define OS_SUPPORT_SAVE_RESTORE_HOOK  (1)
#endif

#ifndef   OS_RR_SUPPORTED
  #define OS_RR_SUPPORTED (1)
#endif

#ifndef   OS_STACK_AT_BOTTOM
  #define OS_STACK_AT_BOTTOM (0)
#endif

/*  Settings for NON-ANSI compiler syntax
    The default behaviour complies with IAR.
*/
#ifndef   OS_COMPILER_LOCATION_LEFT
  #define OS_COMPILER_LOCATION_LEFT 1  /* (req. for IAR) */
#endif

#ifndef   OS_COMPILER_STORAGE_MODIFIER_LEFT
  #define OS_COMPILER_STORAGE_MODIFIER_LEFT 1  /* default for all except KEIL */
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
  #define OS_IDLE() OS_Idle(); \
                    OS_ERROR(OS_ERR_IDLE_RETURNS);   /* We should never arrive here, since the Idle loop should not return */
#endif

#ifndef   OS_SUSPEND_TASK_ON_TERMINATE      // Sets the task state of a terminated task to "suspended". Primarily used in the OS simulation, because the windows scheduler might re-activate a suspended task
  #define OS_SUSPEND_TASK_ON_TERMINATE (0)
#endif

#ifndef   OS_INIT_EXPLICITLY
  #define OS_INIT_EXPLICITLY  (0)           // If enabled, OS_InitKern() initializes Non-Zero globals. Required only for faulty compiler (or startup-code) which do not perform proper init of globals during startup.
#endif

#ifndef   OS_TASK_LIST_HAS_PPREV
  #define OS_TASK_LIST_HAS_PPREV   OS_RR_SUPPORTED
#else
  #if (OS_TASK_LIST_HAS_PPREV == 0) && OS_RR_SUPPORTED
    #error  "Invalid combination of config switches"
  #endif
#endif

#ifndef   OS_SUPPORT_CALL_ISR
  #define OS_SUPPORT_CALL_ISR (1)
#endif

#ifndef   OS_SUPPORT_POWER
  #define OS_SUPPORT_POWER (1)
#endif

#ifndef   OS_SUPPORT_TICK_HOOK
  #define OS_SUPPORT_TICK_HOOK  (1)
#endif

#ifndef   OS_POWER_NUM_COUNTERS
  #define OS_POWER_NUM_COUNTERS (5u)
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

#ifndef   OS_TASK_HAS_PTLS                  // May be defined in OSChip.h to generate equal task structure for different embOS versions, even when TLS is not supported
  #define OS_TASK_HAS_PTLS OS_SUPPORT_PTLS
#endif

#ifndef   OS_TLS_PTR
  #define OS_TLS_PTR void*
#endif

#ifndef   OS_USE_IN_INTERRUPT_FUNCTION      // OS_InInterrupt() normally is a macro, but may be defined as function if necessary
  #define OS_USE_IN_INTERRUPT_FUNCTION (0)
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
#ifndef   OS_PTR_OP_IS_ATOMIC
  #error  "OS_PTR_OP_IS_ATOMIC should be defined in OSChip.h"
#endif

#ifndef   OS_U32_OP_IS_ATOMIC
  #error  "OS_U32_OP_IS_ATOMIC should be defined in OSChip.h"
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
#ifndef OS_BIG_ENDIAN
  #error "Please define OS_BIG_ENDIAN (OSChip.h)"
#endif

#if (OS_STACK_AT_BOTTOM != 0) && (OS_STACK_AT_BOTTOM != 1)
  /*
  *  Only 0 or 1 is allowed, because OS_STACK_AT_BOTTOM is used for calculations!
  */
  #error "OS_STACK_AT_BOTTOM has to be defined to 0 or 1"
#endif

#if OS_SUPPORT_INT_PRIORITY
  #ifndef   OS_IPL_EI_DEFAULT
    #error "Please define OS_IPL_EI_DEFAULT (OSChip.h)"
  #endif
  #ifndef   OS_IPL_DI_DEFAULT
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
  #define OS_INTPRIO           unsigned int
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

/* Defines a true integer. This type is guaranteed
   a) to have at least 8 bits,
   b) to compile and execute best on the target CPU
   It will normally be the same as an int, but on most
   8-bit CPUs it will be replaced by a character because
   the CPU can deal with 8 bits more efficient than with
   16 bit integers.
   Use with care !
*/
#ifndef   OS_INT
  #define OS_INT       int
#endif

#ifndef   OS_UINT
  #define OS_UINT      unsigned OS_INT
#endif

#ifndef   OS_TIME
  #define OS_TIME      int
#endif

#ifndef   OS_STAT
  #define OS_STAT      OS_U8
#endif

#ifndef   OS_PRIO
  #define OS_PRIO      OS_U8
#endif

#ifndef   OS_BOOL
  #define OS_BOOL      OS_U8
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

/*********************************************************************
*
*       Error codes
*
**********************************************************************
*/

/* User 0..99  ***********************************/

/* Port 100..109 *********************************/
#define OS_ERR_ISR_INDEX                      (100)
#define OS_ERR_ISR_VECTOR                     (101)
#define OS_ERR_ISR_PRIO                       (102)
#define OS_ERR_WRONG_STACK                    (103)
#define OS_ERR_ISR_NO_HANDLER                 (104)  /* No interrupt handler was defined for this interrupt */
#define OS_ERR_TLS_INIT                       (105)  /* OS_TLS_Init() called multiple times from one task   */
#define OS_ERR_MB_BUFFER_SIZE                 (106)  /* For 16bit CPUs, the maximum buffer size for a mailbox (64KB) exceeded */

/* OS generic ************************************/
#define OS_ERR_EXTEND_CONTEXT                 (116)  // OS_ExtendTaskContext called multiple times from one task
#define OS_ERR_TIMESLICE                      (117)  // A timeslice value of 0 (illegal) was set. (Legal since embOS version 3.86f)
#define OS_ERR_INTERNAL                       (118)  // OS_ChangeTask called without RegionCnt set (or other internal error)
#define OS_ERR_IDLE_RETURNS                   (119)  // Idle loop should not return
#define OS_ERR_STACK                          (120)  // Stack overflow or invalid stack

/* Counting semaphore overflow */
#define OS_ERR_CSEMA_OVERFLOW                 (121)

/* Power management module */
#define OS_ERR_POWER_OVER                     (122)
#define OS_ERR_POWER_UNDER                    (123)
#define OS_ERR_POWER_INDEX                    (124)

/* System/interrupt stack */
#define OS_ERR_SYS_STACK                      (125)  // embOS system stack overflow
#define OS_ERR_INT_STACK                      (126)  // Interrupt stack overflow

/* invalid or non-initialized data structures */
#define OS_ERR_INV_TASK                       (128)
#define OS_ERR_INV_TIMER                      (129)
#define OS_ERR_INV_MAILBOX                    (130)
#define OS_ERR_INV_CSEMA                      (132)
#define OS_ERR_INV_RSEMA                      (133)

/* Using GetMail1 or PutMail1 or GetMailCond1 or PutMailCond1 on
   a non-1 byte mailbox  */
#define OS_ERR_MAILBOX_NOT1                   (135)

/* Waitable objects deleted with waiting tasks or occupied by task */
#define OS_ERR_MAILBOX_DELETE                 (136)
#define OS_ERR_CSEMA_DELETE                   (137)
#define OS_ERR_RSEMA_DELETE                   (138)

/* internal errors, please contact SEGGER Microcontrollersysteme */
#define OS_ERR_MAILBOX_NOT_IN_LIST            (140)
#define OS_ERR_TASKLIST_CORRUPT               (142)

/* Queue errors */
#define OS_ERR_QUEUE_INUSE                    (143)
#define OS_ERR_QUEUE_NOT_INUSE                (144)
#define OS_ERR_QUEUE_INVALID                  (145)
#define OS_ERR_QUEUE_DELETE                   (146)

/* not matching routine calls or macro usage */
#define OS_ERR_UNUSE_BEFORE_USE               (150)
#define OS_ERR_LEAVEREGION_BEFORE_ENTERREGION (151)
#define OS_ERR_LEAVEINT                       (152)
#define OS_ERR_DICNT                          (153)
#define OS_ERR_INTERRUPT_DISABLED             (154)
#define OS_ERR_TASK_ENDS_WITHOUT_TERMINATE    (155)
#define OS_ERR_RESOURCE_OWNER                 (156)
#define OS_ERR_REGIONCNT                      (157)

#define OS_ERR_ILLEGAL_IN_ISR                 (160)  /* Not a legal system call during interrupt */
#define OS_ERR_ILLEGAL_IN_TIMER               (161)  /* Not a legal system call during timer */
#define OS_ERR_ILLEGAL_OUT_ISR                (162)  /* Not a legal system call outside interrupt */
#define OS_ERR_NOT_IN_ISR                     (163)  /* OS_EnterInterrupt() has been called, but CPU is not in ISR state */
#define OS_ERR_IN_ISR                         (164)  /* OS_EnterInterrupt() has not been called, but CPU is in ISR state */

#define OS_ERR_INIT_NOT_CALLED                (165)  /* OS_InitKern() was not called */

#define OS_ERR_CPU_STATE_ISR_ILLEGAL          (166)
#define OS_ERR_CPU_STATE_ILLEGAL              (167)
#define OS_ERR_CPU_STATE_UNKNOWN              (168)

/* Double used data structures */
#define OS_ERR_2USE_TASK                      (170)
#define OS_ERR_2USE_TIMER                     (171)
#define OS_ERR_2USE_MAILBOX                   (172)
#define OS_ERR_2USE_BSEMA                     (173)
#define OS_ERR_2USE_CSEMA                     (174)
#define OS_ERR_2USE_RSEMA                     (175)
#define OS_ERR_2USE_MEMF                      (176)

/* Communication errors (OSCOM) */
#define OS_ERR_NESTED_RX_INT                  (180)

/* Fixed block memory pool  */
#define OS_ERR_MEMF_INV                       (190)
#define OS_ERR_MEMF_INV_PTR                   (191)
#define OS_ERR_MEMF_PTR_FREE                  (192)
#define OS_ERR_MEMF_RELEASE                   (193)
#define OS_ERR_MEMF_POOLADDR                  (194)
#define OS_ERR_MEMF_BLOCKSIZE                 (195)

/* Task suspend / resume errors */
#define OS_ERR_SUSPEND_TOO_OFTEN              (200)
#define OS_ERR_RESUME_BEFORE_SUSPEND          (201)

/* Other task related errors */
#define OS_ERR_TASK_PRIORITY                  (202)

/* Event object */
#define OS_ERR_EVENT_INVALID                  (210)
#define OS_ERR_EVENT_DELETE                   (212)

/* Waitlist (checked build) */
#define OS_ERR_WAITLIST_RING                  (220)
#define OS_ERR_WAITLIST_PREV                  (221)
#define OS_ERR_WAITLIST_NEXT                  (222)

/* Tick Hook */
#define OS_ERR_TICKHOOK_INVALID               (223)
#define OS_ERR_TICKHOOK_FUNC_INVALID          (224)

/* Other potential problems discovered in checked build */
#define OS_ERR_NOT_IN_REGION                  (225)

/* Cache related */
#define OS_ERR_NON_ALIGNED_INVALIDATE         (230)

/* OS special build, or internal */
#define OS_ERR_TRIAL_LIMIT                    (254)

/*********************************************************************
*
*       Trace identifier
*
**********************************************************************

Identifier from 0 to 99 are reserved for the OS
Even when not all of those are currently used,
they may be defined in the future

*/

#define OS_TRACE_ID_DEACTIVATE      (1u)
#define OS_TRACE_ID_ACTIVATE        (2u)
#define OS_TRACE_ID_TIMERCALLBACK   (3u)

/* Task group */
#define OS_TRACE_ID_DELAY           (10u)
#define OS_TRACE_ID_DELAYUNTIL      (11u)
#define OS_TRACE_ID_SETPRIORITY     (12u)
#define OS_TRACE_ID_WAKETASK        (13u)
#define OS_TRACE_ID_CREATETASK      (14u)
#define OS_TRACE_ID_TERMINATETASK   (15u)
#define OS_TRACE_ID_SUSPENDTASK     (16u)
#define OS_TRACE_ID_RESUMETASK      (17u)
#define OS_TRACE_ID_CREATETASK_EX   (18u)
#define OS_TRACE_ID_YIELD           (19u)

/* Event group */
#define OS_TRACE_ID_WAITEVENT       (20u)
#define OS_TRACE_ID_WAITEVENT_TIMED (21u)
#define OS_TRACE_ID_SIGNALEVENT     (22u)
#define OS_TRACE_ID_CLEAREVENTS     (23u)

/* Mailbox group  */
#define OS_TRACE_ID_CREATEMB        (30u)
#define OS_TRACE_ID_CLEARMB         (31u)
#define OS_TRACE_ID_DELETEMB        (32u)
#define OS_TRACE_ID_PUTMAIL         (33u)
#define OS_TRACE_ID_GETMAIL         (34u)
#define OS_TRACE_ID_PUTMAILCOND     (35u)
#define OS_TRACE_ID_GETMAILCOND     (36u)
#define OS_TRACE_ID_PUTMAIL_TIMED   (37u)
#define OS_TRACE_ID_GETMAIL_TIMED   (38u)
#define OS_TRACE_ID_WAITMAIL        (39u)

#define OS_TRACE_ID_PUTMAIL1        (40u)
#define OS_TRACE_ID_GETMAIL1        (41u)
#define OS_TRACE_ID_PUTMAILCOND1    (42u)
#define OS_TRACE_ID_GETMAILCOND1    (43u)
#define OS_TRACE_ID_PUTMAIL_TIMED1  (44u)
#define OS_TRACE_ID_GETMAIL_TIMED1  (45u)

#define OS_TRACE_ID_PUTMAIL_F       (46u)
#define OS_TRACE_ID_PUTMAIL_F1      (47u)
#define OS_TRACE_ID_PUTMAIL_FCOND   (48u)
#define OS_TRACE_ID_PUTMAIL_FCOND1  (49u)

/* Resource group */
#define OS_TRACE_ID_CREATERSEMA     (50u)
#define OS_TRACE_ID_USE             (51u)
#define OS_TRACE_ID_UNUSE           (52u)
#define OS_TRACE_ID_REQUEST         (53u)
#define OS_TRACE_ID_GETSEMAVALUE    (54u)
#define OS_TRACE_ID_DELETE_RSEMA    (55u)

#define OS_TRACE_ID_WAITMAIL_TIMED  (59u)

/* Counting Semaphore group */
#define OS_TRACE_ID_CREATECSEMA     (60u)
#define OS_TRACE_ID_DELETECSEMA     (61u)
#define OS_TRACE_ID_SIGNALCSEMA     (62u)
#define OS_TRACE_ID_WAITCSEMA       (63u)
#define OS_TRACE_ID_WAITCSEMATIMED  (64u)
#define OS_TRACE_ID_SIGNALCSEMA_MAX (65u)
#define OS_TRACE_ID_SET_CSEMAVALUE  (66u)
#define OS_TRACE_ID_CSEMAREQUEST    (67u)

/* Timer group    */
#define OS_TRACE_ID_CREATETIMER     (70u)
#define OS_TRACE_ID_DELETETIMER     (71u)
#define OS_TRACE_ID_STARTTIMER      (72u)
#define OS_TRACE_ID_STOPTIMER       (73u)
#define OS_TRACE_ID_RETRIGGERTIMER  (74u)
#define OS_TRACE_ID_SETTIMERPERIOD  (75u)
#define OS_TRACE_ID_CREATETIMER_EX  (76u)

/* Heap type memory management  */
#define OS_TRACE_ID_MALLOC          (80u)
#define OS_TRACE_ID_FREE            (81u)
#define OS_TRACE_ID_REALLOC         (82u)

/* Fixed block type memory management  */
#define OS_TRACE_ID_MEMF_CREATE     (90u)
#define OS_TRACE_ID_MEMF_DELETE     (91u)
#define OS_TRACE_ID_MEMF_ALLOC      (92u)


/*********************************************************************
*
*       Assertions
*
**********************************************************************

  Assertions are used to generate code in the debug version
  of the OS in order catch programming faults like
  bad pointers or uninitialized data structures
*/

#if OS_DEBUG
  #define OS_ASSERT(Exp, ErrCode)  { if (!(Exp)) {OS_Error(ErrCode); }}
#else
  #define OS_ASSERT(Exp, ErrCode)
#endif

#define OS_ASSERT_INT()           OS_ASSERT((OS_InInt != 0u),           OS_ERR_ILLEGAL_OUT_ISR)
#define OS_ASSERT_NOINT()         OS_ASSERT((OS_InInt == 0u),           OS_ERR_ILLEGAL_IN_ISR)
#define OS_ASSERT_NOTIMER()       OS_ASSERT((OS_InTimer == 0u),         OS_ERR_ILLEGAL_IN_TIMER)
#define OS_ASSERT_DICnt()         OS_ASSERT(((OS_DICnt & 0xf0u) == 0u), OS_ERR_DICNT)

#define OS_ASSERT_INIT_CALLED()   OS_ASSERT((OS_InitCalled != 0u),      OS_ERR_INIT_NOT_CALLED)

#if OS_DEBUG
  //
  // OS_ASSERT_CPU_IN_ISR_MODE is typically called from OS_EnterInterrupt() and checks the hardware sate of the CPU
  //
  #define OS_ASSERT_CPU_IN_ISR_MODE()  OS_AssertCPUInISRMode()
#else
  #define OS_ASSERT_CPU_IN_ISR_MODE()
#endif

#if OS_DEBUG
  #define OS_ERROR(ErrCode) OS_Error(ErrCode)
#else
  #define OS_ERROR(ErrCode)
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

#ifndef OS_SADDR
  #ifdef SADDR
    #define OS_SADDR SADDR
  #else
    #define OS_SADDR
  #endif
#endif

/* pointer modifier for data items */
#ifndef   OS_DATAPTR
  #define OS_DATAPTR
#endif

#ifndef   OS_RSEMAPTR
  #define OS_RSEMAPTR
#endif

#ifndef   OS_CSEMAPTR
  #define OS_CSEMAPTR
#endif

#ifndef   OS_MBPTR
  #define OS_MBPTR
#endif

#ifndef   OS_TCBPTR
  #define OS_TCBPTR
#endif

#ifndef   OS_TIMERPTR
  #define OS_TIMERPTR
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

typedef struct OS_TASK_STRUCT         OS_TASK;
typedef struct OS_WAIT_LIST_STRUCT    OS_WAIT_LIST;
typedef struct OS_WAIT_OBJ_STRUCT     OS_WAIT_OBJ;
typedef struct OS_WAIT_OBJ_EX_STRUCT  OS_WAIT_OBJ_EX;

typedef void voidRoutine(void);
typedef void OS_RX_CALLBACK(OS_U8 Data);
typedef void OS_ON_TERMINATE_FUNC(OS_CONST_PTR OS_TASK * pTask);

typedef void OS_TICK_HOOK_ROUTINE(void);

typedef struct OS_GLOBAL_STRUCT       OS_GLOBAL;

#if OS_SUPPORT_TIMER
typedef void OS_TIMERROUTINE(void);
typedef void OS_TIMER_EX_ROUTINE(void * pVoid);
typedef void OS_TIMER_HANDLER (OS_GLOBAL* pGlobal);
#endif

/**********************************************************************
*
*       OS_WAIT_OBJ
*       OS_WAIT_LIST
*       OS_WAIT_OBJ_EX
*/
struct OS_WAIT_OBJ_STRUCT {
  OS_WAIT_LIST * pWaitList;
};

struct OS_WAIT_OBJ_EX_STRUCT {
  OS_WAIT_OBJ WaitObj;
  int v;
};

struct OS_WAIT_LIST_STRUCT {
  OS_WAIT_LIST * pNext;    /* Needs to be first element ! */
  OS_WAIT_LIST * pPrev;
  OS_WAIT_OBJ  * pWaitObj;
  OS_TASK      * pTask;
};


/**********************************************************************
*
*       OS_EXTEND_TASK_CONTEXT
*
*  This structure is used to define a save and restore function for
*  extension of the task context.
*  A pointer to this structure is part of the task control block
*  It is initialized by OS_ExtendTaskContext();
*/
typedef struct OS_EXTEND_TASK_CONTEXT_STRUCT {
  void (*pfSave)   (      void OS_STACKPTR * pStack);
  void (*pfRestore)(const void OS_STACKPTR * pStack);
} OS_EXTEND_TASK_CONTEXT;


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
  OS_TASK * pNext;                        // Points to the TCB of the next task in the list (with equal or lower priority). Needs to be first element !
  OS_REGS OS_STACKPTR * pStack;           // Typically contains the stack pointer if the task is suspended.                 Needs to be second element !
  // End of assembly relevant section
  OS_WAIT_LIST * pWaitList;               // Points to a waitable object if task is suspended.
  OS_TIME        Timeout;
  OS_STAT        Stat;
  OS_PRIO        Priority;
  OS_TASK_EVENT  Events;                  // Event storage
  OS_TASK_EVENT  EventMask;               // Event mask
  //
  // PPrev is required only in builds with doubly-linked task lists
  //
  #if OS_TASK_LIST_HAS_PPREV
    OS_TASK      * pPrev;                   //
  #endif
  //
  // Elements required with "Track-name" functionality only. Typically available in all builds, but can be disabled at compile time
  //
  #if OS_TRACKNAME
    OS_ROM_DATA const char * Name;
  #endif
  //
  // Elements required with "Stack-check" functionality only. Available in stack-check and debug builds.
  //
  #if OS_CHECKSTACK
    OS_UINT  StackSize;            // Stack size in bytes. Not required for functionality, just for analysis
    OS_U8 OS_STACKPTR* pStackBot;  // First byte of stack. Not required for functionality, just for analysis
  #endif
  //
  // Elements required with profiling or debug builds
  //
  #if (OS_SUPPORT_STAT)
    OS_U32 NumActivations;         // Counts how many times task has been activated
    OS_U32 NumPreemptions;         // Counts how many times task has been preempted
  #endif
  //
  // Elements required with profiling builds
  //
  #if OS_PROFILE
    OS_U32 ExecTotal;              // Time spent executing
    OS_U32 ExecLast;               // Time spent executing (Reference)
    OS_U32 Load;                   // Profiling builds only:
  #endif
  //
  // Elements required with Round-robin functionality only. Typically available in all builds, but can be disabled at compile time
  //
  #if OS_RR_SUPPORTED
    OS_U8  TimeSliceRem;
    OS_U8  TimeSliceReload;
  #endif
  //
  // Optional Save & Restore hook support (usually only for bigger CPUs)
  //
  #if OS_SUPPORT_SAVE_RESTORE_HOOK
    const OS_EXTEND_TASK_CONTEXT * pExtendContext;
  #endif
  //
  // Optional thread local storage pointer
  //
  #if OS_TASK_HAS_PTLS
     #if (defined(OS_LIBMODE_XR) && (OS_SUPPORT_PTLS == 0))
       // XR-Build may not support TLS
     #else
       OS_TLS_PTR pTLS;
     #endif
  #endif
  //
  // Elements required with debug builds
  //
  #if OS_DEBUG
    OS_U8 Id;          // Debug builds only: Id of this control block.
  #endif
  //
  // Allow port specific extension to the task control block. Not used in most ports
  //
  #ifdef OS_TCB_CPU_EX
    OS_TCB_CPU_EX
  #endif
};

#if OS_SUPPORT_TIMER
/**********************************************************************
*
*       OS_TIMER
*/
typedef struct OS_timer OS_TIMER;
struct OS_timer {
  OS_TIMER * pNext;
  voidRoutine* Hook;
  OS_TIME Time;
  OS_TIME Period;
  char    Active;
#if OS_DEBUG
  char Id;
#endif
};

/**********************************************************************
*
*       OS_TIMER_EX
*/
typedef struct {
  OS_TIMER Timer;
  OS_TIMER_EX_ROUTINE * pfUser;
  void * pData;
} OS_TIMER_EX;

#endif // OS_SUPPORT_TIMER

/**********************************************************************
*
*       OS_TICK_HOOK
*/
typedef struct OS_TICK_HOOK_STRUCT OS_TICK_HOOK;
struct OS_TICK_HOOK_STRUCT {
  OS_TICK_HOOK*          pNext;
  OS_TICK_HOOK_ROUTINE * pfUser;
};

/**********************************************************************
*
*       OS_ON_TERMINATE_HOOK
*/
typedef struct OS_ON_TERMINATE_HOOK_STRUCT OS_ON_TERMINATE_HOOK;
struct OS_ON_TERMINATE_HOOK_STRUCT {
  OS_ON_TERMINATE_HOOK * pNext;
  OS_ON_TERMINATE_FUNC * pfUser;
};

/**********************************************************************
*
*       OS_RSEMA
*/
typedef struct OS_RSEMA_STRUCT OS_RSEMA;
struct OS_RSEMA_STRUCT {
  OS_WAIT_OBJ WaitObj;
  OS_TASK * pTask;                /* Owner */
  OS_U8 UseCnt;
  OS_RSEMA * pNext;
#if OS_DEBUG
  char Id;
#endif
};

/**********************************************************************
*
*       OS_CSEMA
*/
typedef struct OS_CSEMA_STRUCT OS_CSEMA;
struct OS_CSEMA_STRUCT {
  OS_WAIT_OBJ WaitObj;
  OS_UINT Cnt;
#if OS_DEBUG
  OS_CSEMA * pNext;
  char Id;
#endif
};

/**********************************************************************
*
*       OS_MAILBOX
*/
typedef struct OS_MAILBOX_STRUCT OS_MAILBOX;
struct OS_MAILBOX_STRUCT {
  OS_WAIT_OBJ WaitObj;
#if OS_LINK_MAILBOXES
  OS_MAILBOX * pNext;
#endif
  char *pData;
  OS_UINT nofMsg;
  OS_UINT maxMsg;
  OS_UINT iRd;
  OS_U16  sizeofMsg;
#if OS_DEBUG
  char Id;
#endif
};

/**********************************************************************
*
*       OS_Q
*/
typedef struct OS_Q_STRUCT OS_Q;
struct OS_Q_STRUCT {
  OS_WAIT_OBJ WaitObj;
  OS_Q*    pNext;          /* ptr to next queue (for debugging / monitoring) */
  OS_U8*   pData;
  OS_UINT  Size;
  OS_UINT  MsgCnt;
  OS_UINT  offFirst;
  OS_UINT  offLast;
  OS_BOOL  InUse;
  OS_UINT  InProgressCnt;
#if OS_DEBUG
  char Id;
#endif
};

/**********************************************************************
*
*       OS_MEMF
*/
#ifndef   OS_MEMF_MAX_ITEMS
  #define OS_MEMF_MAX_ITEMS 20
#endif

#define OS_MEMF_SIZEOF_BLOCKCONTROL 0  /* sizeof(int) in future version for debugging */

typedef struct OS_MEMF_STRUCT OS_MEMF;
struct OS_MEMF_STRUCT {
  OS_WAIT_OBJ WaitObj;
  OS_MEMF* pNext;          /* ptr to next memory pool */
  void*    pPool;
  OS_UINT  NumBlocks;
  OS_UINT  BlockSize;
  OS_UINT  NumFreeBlocks;
  OS_UINT  MaxUsed;
  void*    pFree;
  #if OS_DEBUG
    int  aiPurpose[OS_MEMF_MAX_ITEMS];
    char Id;
  #endif
};

/**********************************************************************
*
*       OS_EVENT
*/
typedef struct  OS_EVENT_STRUCT OS_EVENT;

/****** Reset modes for OS_EVENT objects *****************************/

typedef enum {
  OS_EVENT_RESET_MODE_SEMIAUTO = 0 ,  // Same as previous mode, mix from automatic an manual reset
  OS_EVENT_RESET_MODE_MANUAL,         // Manual reset, event remains set when waiting task is resumed, has to be reset by task
  OS_EVENT_RESET_MODE_AUTO,           // automatic reset, event is automatically cleared when waiting task is resumed
  OS_EVENT_RESET_MODE_COUNT           // Number of modes
} OS_EVENT_RESET_MODE;

struct OS_EVENT_STRUCT {
  OS_WAIT_OBJ         WaitObj;
  OS_U8               Signaled;
  OS_EVENT_RESET_MODE ResetMode;
#if OS_DEBUG
  char Id;
#endif
};

/**********************************************************************
*
*       OS_TRACE_ENTRY
*/
typedef struct {
  OS_U32  Time;
  void    *pCurrentTask;
  void    *p;
  OS_U32  v;
  OS_U8   iRout;
} OS_TRACE_ENTRY;


/*********************************************************************
*
*       Globals
*
**********************************************************************
*/

/*
    Handle DEFinition (storage is actually allocted) versus DEClaration
    (reference) of RTOS variables depending upon who includes this header file.
*/
#ifdef OSGLOBAL_C
  #define OS_EXTERN        /* Declare variables if included by RTOSKERN.C */
  #define OS_EXTERN_INIT(Var, Val) (Var)=(Val)
#else
  #define OS_EXTERN extern /* Define variables if included by anyone else */
  #define OS_EXTERN_INIT(Var, Val) (Var)
#endif

/*****  Mark start of memory pragma area ****************************/

/* Some compilers can not deal with memory attributes and need pragmas */
#if defined(__ghs_zda)
  #pragma ghs startzda
#endif

/********************************************************************/

typedef union {          // Violates MISRA rule 18.4. We need a union for efficient scheduling
  int Dummy;             // Make sure a full integer (32 bit on 32 bit CPUs) is used.
  struct {
    OS_U8 Region;
    OS_U8 DI;
  } Cnt;
} OS_COUNTERS;

#ifndef PENDING_DUMMY_BYTES
  #define PENDING_DUMMY_BYTES OS_U8 aDummy[2];
#endif

typedef union {          // Violates MISRA rule 18.4. We need a union for efficient scheduling
  OS_U32 All;            // Make sure a full integer (32 bit on 32 bit CPUs) is used.
  struct {
    OS_U8 RoundRobin;
    OS_U8 TaskSwitch;
    PENDING_DUMMY_BYTES
  } Flag;
} OS_PENDING;

struct OS_GLOBAL_STRUCT {
  OS_COUNTERS       Counters;
  OS_PENDING        Pending;
  OS_TASK      *    pCurrentTask;      // Pointer to current Task
#if OS_SUPPORT_INT_PRIORITY
  OS_IPL_DI_TYPE    Ipl_DI;
  OS_IPL_EI_TYPE    Ipl_EI;
#endif
  OS_TASK      *    pTask;             // Linked list of all Tasks
  OS_TASK volatile * volatile pActiveTask;
#if OS_SUPPORT_TIMER
  OS_TIMER     *    pTimer;            // Linked list of all active Timers
  OS_TIMER     *    pCurrentTimer;     // Actual expired timer which called callback
  OS_TIMER_HANDLER* pfCheckTimer;      // Timer handler function, set when OS_StartTimer() is called
#endif // OS_SUPPORT_TIMER
  volatile          OS_I32 Time;
  OS_TIME           TimeDex;
#if OS_RR_SUPPORTED
  OS_U8             TimeSlice;
  OS_U8             TimeSliceAtStart;
#endif
#if OS_SUPPORT_TICKLESS
  OS_TIME           TicklessFactor;
  OS_BOOL           TicklessExpired;
  voidRoutine       *pfEndTicklessMode;
#endif
};

/* Translation macros for compatibility with older versions */
#define OS_pTask         OS_Global.pTask
#define OS_pCurrentTask  OS_Global.pCurrentTask
#define OS_pActiveTask   OS_Global.pActiveTask
#define OS_pTimer        OS_Global.pTimer
#define OS_pCurrentTimer OS_Global.pCurrentTimer
#define OS_Counters      OS_Global.Counters
#define OS_Pending       OS_Global.Pending
#define OS_Ipl_DI        OS_Global.Ipl_DI
#define OS_Ipl_EI        OS_Global.Ipl_EI
#define OS_Time          OS_Global.Time
#define OS_TimeDex       OS_Global.TimeDex
#if OS_RR_SUPPORTED
  #define OS_TimeSlice           OS_Global.TimeSlice
  #define OS_TimeSliceAtStart    OS_Global.TimeSliceAtStart
#endif

#if OS_COMPILER_STORAGE_MODIFIER_LEFT  /* default */

  OS_EXTERN OS_SADDR  OS_U8    OS_EXTERN_INIT(OS_IntMSInc, 1u);
  OS_EXTERN OS_SADDR  volatile OS_U8  OS_Status;
  OS_EXTERN OS_SADDR  OS_BOOL  OS_Running;
  OS_EXTERN OS_SADDR  OS_U8    OS_InitialSuspendCnt;
  OS_EXTERN OS_SADDR  OS_U8    OS_InInt;

  #if OS_SUPPORT_TICKSTEP
    OS_EXTERN OS_SADDR  volatile OS_U8   OS_TickStep;
    OS_EXTERN OS_SADDR  volatile int     OS_TickStepTime;
  #endif
  OS_EXTERN OS_SADDR        OS_UINT OS_TicksPerMS;
  OS_EXTERN OS_SADDR        int     OS_IntTicksInc;
  #if OS_PROFILE
    OS_EXTERN OS_SADDR OS_U32 OS_TS_ExecStart;
    OS_EXTERN OS_SADDR OS_U8  ProfilingOn;
  #endif
  #if OS_DEBUG
    OS_EXTERN OS_SADDR OS_U8 OS_InTimer;
    OS_EXTERN OS_SADDR OS_U8 OS_InitCalled;
  #endif

  #if OS_SUPPORT_POWER
    OS_EXTERN OS_SADDR OS_U8 OS_POWER_Levels[OS_POWER_NUM_COUNTERS];
  #endif

  /****** Variables for CPU load measurement are declared global. This allows showing them in a debugger ****/
  OS_EXTERN OS_SADDR volatile OS_INT OS_CPU_Load;
  OS_EXTERN OS_SADDR volatile OS_I32 OS_IdleCnt;

  /*** Pointers ***
    Unfortunately there is no real standard on how to define/declare
    pointers properly with extended memory attributes. There are
    currently 2 different standards, which we both support.
  */

  #if OS_COMPILER_LOCATION_LEFT
    extern    OS_SADDR OS_GLOBAL     OS_Global;              /* Major OS variables, initialized in OSGlobal.c */
    OS_EXTERN OS_SADDR OS_RSEMA      * OS_pRSema;
    OS_EXTERN OS_SADDR OS_Q          * OS_pQHead;
    #if OS_LINK_MAILBOXES
      OS_EXTERN OS_SADDR OS_MAILBOX  * OS_pMailbox;
    #endif
    #if OS_DEBUG
      OS_EXTERN OS_SADDR OS_CSEMA    * OS_pCSema;
    #endif
    OS_EXTERN   OS_SADDR OS_MEMF      * OS_pMEMF;            /* linked list of all fixed size memory pools */
    OS_EXTERN   OS_SADDR OS_TICK_HOOK * OS_pTickHook;        /* linked list of all tick hook functions */
    #if OS_SUPPORT_PTLS
      OS_EXTERN   OS_SADDR OS_TLS_PTR OS_pTLS;               /* Global pointer to thread local storage */
    #endif
  #else
    extern      OS_GLOBAL            OS_SADDR OS_Global;     /* Major OS variables, initialized in OSGlobal.c */
    OS_EXTERN   OS_RSEMA           * OS_SADDR OS_pRSema;
    OS_EXTERN   OS_Q               * OS_SADDR OS_pQHead;

    #if OS_LINK_MAILBOXES
      OS_EXTERN OS_MAILBOX         * OS_SADDR OS_pMailbox;
    #endif

    #if OS_DEBUG
      OS_EXTERN  OS_CSEMA          * OS_SADDR OS_pCSema;
    #endif
    OS_EXTERN OS_MEMF              * OS_SADDR OS_pMEMF;          /* linked list of all fixed size memory pools */
    OS_EXTERN OS_TICK_HOOK         * OS_SADDR OS_pTickHook;      /* linked list of all tick hook functions */
    #if OS_SUPPORT_PTLS
      OS_EXTERN OS_TLS_PTR           OS_SADDR OS_pTLS;           /* Global pointer to thread local storage */
    #endif
  #endif

#else   /*  OS_COMPILER_STORAGE_MODIFIER_LEFT == 0, needed for Keil 166 */
  #error "OS_COMPILER_STORAGE_MODIFIER_LEFT == 0 not supported in this version."
#endif  /*  OS_COMPILER_STORAGE_MODIFIER_LEFT  */

/***** Mark end of memory pragma area *******************************/
#if defined(__ghs_zda)
  #pragma ghs endzda
#endif

/********************************************************************/

#define OS_RegionCnt OS_Global.Counters.Cnt.Region
#define OS_DICnt     OS_Global.Counters.Cnt.DI

/*********************************************************************
*
*       OS_CREATETASK macro
*       OS_CREATETASK_EX macro
*/
#if OS_RR_SUPPORTED
  #define CTPARA_TIMESLICE ,2u
#else
  #define CTPARA_TIMESLICE
#endif

#if OS_TRACKNAME
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

#if OS_TRACKNAME
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
#if OS_TRACKNAME
  #define OS_CREATE_TASK_PARA_NAME      OS_ROM_DATA const char* pName,
#else
  #define OS_CREATE_TASK_PARA_NAME
#endif

#if OS_RR_SUPPORTED
  #define OS_CREATE_TASK_PARA_TS   ,OS_UINT TimeSlice
#else
  #define OS_CREATE_TASK_PARA_TS
#endif

void OS_CreateTask  ( OS_TASK * pTask,
                      OS_CREATE_TASK_PARA_NAME
                      OS_PRIO Priority,
                      void (*pRoutine)(void),
                      void OS_STACKPTR *pStack,
                      OS_UINT StackSize
                      OS_CREATE_TASK_PARA_TS
        );

void OS_CreateTaskEx  ( OS_TASK * pTask,
                        OS_CREATE_TASK_PARA_NAME
                        OS_PRIO Priority,
                        void (*pRoutine)(void * pVoid),
                        void OS_STACKPTR *pStack,
                        OS_UINT StackSize
                        OS_CREATE_TASK_PARA_TS,
                        void * pContext
        );

#if OS_SUPPORT_SAVE_RESTORE_HOOK
void OS_ExtendTaskContext(const OS_EXTEND_TASK_CONTEXT * pExtendContext);
#endif

/*********************************************************************
*
*       Task related routines
*
**********************************************************************
*/
void     OS_AddOnTerminateHook       (OS_ON_TERMINATE_HOOK * pHook, OS_ON_TERMINATE_FUNC * pfUser);
void     OS_Delay                    (OS_TIME ms);            /* OSKern.c              */
void     OS_DelayUntil               (OS_TIME t);             /* OSKern.c              */
void     OS_Delayus                  (OS_U16  us);            /* OS_Delayus.c          */
OS_U8    OS_GetSuspendCnt            (OS_CONST_PTR OS_TASK* pTask);  /* OS_GetSuspendCnt.c    */
void     OS_GetState                 (OS_TASK* pTask);
OS_BOOL  OS_IsTask                   (OS_CONST_PTR OS_TASK* pTask);  /* OSIsTask.c            */
void     OS_Resume                   (OS_TASK* pTask);        /* OS_Suspend.c          */
void     OS_ResumeAllSuspendedTasks  (void);                  /* OS_ResumeAllTasks.c   */
void     OS_SetTaskName              (OS_TASK * pTask, OS_ROM_DATA const char* s);
void     OS_SetPriority              (OS_TASK* pTask, OS_U8 Priority);   /* OSSetP.c   */
void     OS_SetInitialSuspendCnt     (OS_U8 SuspendCnt);      /* OS_SetSuspendCnt.c    */
void     OS_Suspend                  (OS_TASK* pTask);        /* OS_Suspend.c          */
void     OS_SuspendAllTasks          (void);                  /* OS_SuspendAllTasks.c  */
void     OS_TerminateTask            (OS_TASK* pTask);        /* OSTerm.c              */
void     OS_TerminateError           (void);                  /* OSTermE.c             */
void     OS_WakeTask                 (OS_TASK* pTask);        /* OSWake.c              */
void     OS_Yield                    (void);                  /* OS_Yield              */
#if OS_SUSPEND_TASK_ON_TERMINATE           /* Set task state of a terminated task to "suspended" */
  #define OS_SUSPEND_TERMINATED_TASK(pTask) (pTask->Stat = 0x01)
#else
  #define OS_SUSPEND_TERMINATED_TASK(pTask)
#endif

/*********************************************************************
*
*       Info routines (OSInfo.c)
*
**********************************************************************
*/

OS_ROM_DATA const char* OS_GetCPU(void);
OS_ROM_DATA const char* OS_GetLibMode(void);
OS_ROM_DATA const char* OS_GetModel(void);
OS_ROM_DATA const char* OS_GetLibName(void);

OS_UINT OS_GetVersion(void);
#define OS_GetType() OS_GetLibMode()

#define OS_TASKID OS_TASK *
#define OS_GetTaskID() OS_Global.pCurrentTask

#define OS_GetpCurrentTask()    OS_Global.pCurrentTask
#define OS_IsRunning()          OS_Running

#if OS_USE_IN_INTERRUPT_FUNCTION   // OS_InInterrupt() may be implemented as function if required
  OS_BOOL OS_InInterruptFunc(void);
  #define OS_InInterrupt           OS_InInterruptFunc
#else
  #define OS_InInterrupt()         OS_InInt
#endif

#if OS_SUPPORT_TIMER
  #define OS_GetpCurrentTimer()   OS_Global.pCurrentTimer
  #define OS_GetpCurrentTimerEx() ((OS_TIMER_EX*)OS_Global.pCurrentTimer)
#endif  // OS_SUPPORT_TIMER

#if OS_TRACKNAME
  OS_ROM_DATA const char* OS_GetTaskName(OS_CONST_PTR OS_TASK OS_TCBPTR *pTask);
#else
  #define OS_GetTaskName(pt)  ("n/a")
#endif

#if OS_CHECKSTACK
  void              OS_CheckStack(void);              /* internal usage */
  unsigned int      OS_GetStackSpace(OS_TASK * pTask);
  unsigned int      OS_GetStackUsed (OS_TASK * pTask);
  unsigned int      OS_GetStackSize (OS_TASK OS_TCBPTR * pTask);
  void OS_STACKPTR* OS_GetStackBase (OS_TASK OS_TCBPTR *pTask);
#else
  #define OS_CheckStack()
  #define OS_GetStackSpace(pt) 0u
  #define OS_GetStackUsed(pt)  0u
  #define OS_GetStackSize(pt)  0u
  #define OS_GetStackBase(pt)  0u
#endif

#if OS_SUPPORT_SYSSTACK_INFO
  void OS_STACKPTR* OS_GetSysStackBase (void); /* RTOS asm part or OS_StackInfo.c */
  unsigned int      OS_GetSysStackSize (void); /* RTOS asm part or OS_StackInfo.c */
#else
  #define OS_GetSysStackBase()   0
  #define OS_GetSysStackSize()   0u
#endif

#if OS_CHECKSTACK && OS_SUPPORT_SYSSTACK_INFO
  unsigned int OS_GetSysStackSpace(void); /* OS_SysStackInfo.c               */
  unsigned int OS_GetSysStackUsed (void); /* OS_SysStackInfo.c               */
#else
  #define OS_GetSysStackSpace()  0u
  #define OS_GetSysStackUsed()   0u
#endif

#if OS_SUPPORT_INTSTACK_INFO
  void OS_STACKPTR* OS_GetIntStackBase(void);  /* RTOS asm part or OS_StackInfo.c */
  unsigned int      OS_GetIntStackSize(void);  /* RTOS asm part or OS_StackInfo.c */
#else
  #define OS_GetIntStackBase()   0
  #define OS_GetIntStackSize()   0u
#endif

#if OS_CHECKSTACK && OS_SUPPORT_INTSTACK_INFO
  unsigned int OS_GetIntStackSpace(void); /* OS_IntStackInfo.c               */
  unsigned int OS_GetIntStackUsed (void); /* OS_IntStackInfo.c               */
#else
  #define      OS_GetIntStackSpace()  0u
  #define      OS_GetIntStackUsed()   0u
#endif

#if OS_RR_SUPPORTED
  OS_U8 OS_SetTimeSlice(OS_TASK * pTask, OS_U8 TimeSlice);
  OS_U8 OS_GetTimeSliceRem(OS_CONST_PTR OS_TASK OS_TCBPTR * pTask);
  #define OS_GetTimeSliceReload(pTask)  (pTask)->TimeSliceReload
#else
  #define OS_GetTimeSliceRem(pTask)     0u
  #define OS_GetTimeSliceReload(pTask)  0u
#endif

OS_PRIO OS_GetPriority(OS_CONST_PTR OS_TASK * pTask);  /* OS_GetPrio.c       */
#define OS_GET_PRIORITY(pt) (pt)->Priority

int     OS_GetNumTasks(void);              /* OS_GetNumTasks.c   */

OS_TASK* OS_TaskIndex2Ptr(int TaskIndex);  /* OS_TaskIndex2Ptr.c */

#if OS_PROFILE >=1
  OS_U32 OS_STAT_GetExecTime_Cycles (OS_TASK *pTask);  /* OSStat.c   */
  int    OS_STAT_GetLoad            (OS_TASK *pTask);  /* OSStatGL.c */
  void   OS_STAT_NotifyReadyStart   (OS_TASK *pTask);  /* OSStat.c   */
  void   OS_STAT_Sample(void);                         /* OSStat.c   */
#else
  #define OS_STAT_NotifyExecEnd()
  #define OS_STAT_GetExecTime_Cycles(pTask)   0u
  #define OS_STAT_GetLoad(pTask)              0
  #define OS_STAT_Sample()
  #define OS_STAT_SetClocksPerUnit(ClocksPerUnit)
  #define OS_STAT_NOTIFYREADYSTART(pTask)
#endif


#if OS_SUPPORT_STAT
  OS_U32 OS_STAT_GetNumActivations  (OS_TASK *pTask);  /* OSStat.c   */
  OS_U32 OS_STAT_GetNumPreemptions  (OS_TASK *pTask);  /* OSStat.c   */
#else
  #define OS_STAT_GetNumActivations(pTask)    0u
  #define OS_STAT_GetNumPreemptions(pTask)    0u
#endif

/*********************************************************************
*
*       Size info routines (OS_GetSize.c)
*
**********************************************************************
*/

int OS_WAIT_OBJ_GetSize(void);
int OS_WAIT_OBJ_EX_GetSize(void);
int OS_WAIT_LIST_GetSize(void);
int OS_EXTEND_TASK_CONTEXT_GetSize(void);
int OS_TASK_GetSize(void);
int OS_REGS_GetSize(void);
int OS_TICK_HOOK_GetSize(void);
int OS_RSEMA_GetSize(void);
int OS_CSEMA_GetSize(void);
int OS_MAILBOX_GetSize(void);
int OS_Q_GetSize(void);
int OS_MEMF_GetSize(void);
int OS_EVENT_GetSize(void);
int OS_TRACE_ENTRY_GetSize(void);

#if OS_SUPPORT_TIMER
  int OS_TIMER_GetSize(void);
  int OS_TIMER_EX_GetSize(void);
#endif

/*********************************************************************
*
*       Critical regions (OsKern.c)
*
**********************************************************************
*/

//
// For some GCC compiler it is necessary to use an OS_EnterRegion() function
// instead of the macro. To do so please add the following line to OSCHIP.h:
// #define OS_EnterRegion OS_EnterRegionFunc
//
#ifndef OS_EnterRegion
  #if OS_DEBUG > 1
    #define OS_EnterRegion() {if (OS_RegionCnt == 0xFF) OS_Error(OS_ERR_REGIONCNT); else OS_RegionCnt++; }
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

void OS_EnterRegionFunc(void);           /* in OS_EnterRegionFunc.c */
void OS_LeaveRegion(void);

/*********************************************************************
*
*       Interrupt save/disable/restore macros
*
**********************************************************************
*/

#define OS_IncDI()       { OS_ASSERT_DICnt(); OS_DI(); OS_DICnt++; }
#define OS_DecRI()       { OS_ASSERT_DICnt();                                      \
                           if (--OS_DICnt == 0u) { /*lint !e548 No else required*/ \
                             OS_EI();                                              \
                           }                                                       \
                         }

#define OS_RESTORE_I()   { OS_ASSERT_DICnt();                                      \
                           if (OS_DICnt == 0u) {   /*lint !e548 No else required*/ \
                             OS_EI();                                              \
                           }                                                       \
                         }

void OS_RestoreI(void);  /* OS_Kern.c */

/*********************************************************************
*
*       Global interrupt save/disable/restore macros
*
*       The global OS_INTERRUPT_ functions are declared as MACRO.
*       If not defined in OSChip.h, these macros are translated
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
  OS_INTERWORK void OS_InterruptMaskGlobal(void);
  #define OS_INTERRUPT_MaskGlobal() OS_InterruptMaskGlobal()
#endif

#ifndef OS_INTERRUPT_UnmaskGlobal
  OS_INTERWORK void OS_InterruptUnmaskGlobal(void);
  #define OS_INTERRUPT_UnmaskGlobal() OS_InterruptUnmaskGlobal()
#endif

#ifndef OS_INTERRUPT_PreserveGlobal
  OS_INTERWORK void OS_InterruptPreserveGlobal(OS_U32 *pSave);
  #define OS_INTERRUPT_PreserveGlobal(p) OS_InterruptPreserveGlobal((p))
#endif

#ifndef OS_INTERRUPT_RestoreGlobal
  OS_INTERWORK void OS_InterruptRestoreGlobal(OS_U32 *pSave);
  #define OS_INTERRUPT_RestoreGlobal(p) OS_InterruptRestoreGlobal((p))
#endif

#ifndef OS_INTERRUPT_PreserveAndMaskGlobal
  OS_INTERWORK void OS_InterruptPreserveAndMaskGlobal(OS_U32 *pSave);
  #define OS_INTERRUPT_PreserveAndMaskGlobal(p) OS_InterruptPreserveAndMaskGlobal((p))
#endif

/*********************************************************************
*
*       ISR (Interrupt service routine) support
*
**********************************************************************
*/

#if OS_DEBUG
  #define OS_MARK_IN_ISR()         {OS_InInt++;}
  #define OS_MARK_OUTOF_ISR()      {if (!OS_InInt--) {OS_Error(OS_ERR_LEAVEINT);}}
#else
  #define OS_MARK_IN_ISR()         {OS_InInt++;}
  #define OS_MARK_OUTOF_ISR()      {OS_InInt--;}
#endif

#if OS_SUPPORT_CALL_ISR            // Not allowed for some CPUs
void OS_CallISR        (void (*pRoutine)(void));
void OS_CallNestableISR(void (*pRoutine)(void));
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

  #if OS_INTERRUPTS_ARE_NESTABLE_ON_ENTRY
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

  #if OS_SWITCH_FROM_INT_MODIFIES_STACK
    #define OS_HANDLE_REGION_CNT_ON_LI() \
      else {                             \
        OS_RegionCnt--;                  \
      }
  #else
    #define OS_HANDLE_REGION_CNT_ON_LI() \
      OS_RegionCnt--;
  #endif

  #ifndef   OS_DI_ON_LEAVE_NESTABLE
    #define OS_DI_ON_LEAVE_NESTABLE() OS_DI()
  #endif

  #ifndef   OS_EI_ON_LEAVE_NESTABLE
    #define OS_EI_ON_LEAVE_NESTABLE() OS_EI_ON_LEAVE()
  #endif

  #define OS_SWITCH_FROM_INT_IF_REQUIRED()                               \
    if ((OS_RegionCnt == (OS_U8) 1u) && (OS_Global.Pending.All != 0u)) { \
      OS_SwitchFromInt();                                                \
    }

  #define OS_EnterInterrupt() {   \
    OS_MARK_IN_ISR();             \
    OS_ASSERT_CPU_IN_ISR_MODE();  \
    OS_DI_ON_ENTRY();         /* Disable low-prio interrupts if necessary: On CPUs, that keep interrupts enabled on ISR entyr. We disable all further low-prio interrupts. */    \
    OS_EI_HP_ON_ENTRY();      /* Enable high-prio interrupts if necessary: Some CPUs (such as M16C) disable all interrupts on ISR entry. We need to enable high prio Ints, using the global Int-Enable */     \
    OS_RegionCnt++;               \
    OS_DICnt++;                   \
  }

  #define OS_LeaveInterrupt() {                                                 \
    OS_MARK_OUTOF_ISR();                                                        \
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
    OS_MARK_OUTOF_ISR();                \
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
  void OS__EnterIntStack(void);
  #define OS_EnterIntStack() {OS_DI(); OS__EnterIntStack(); OS_RESTORE_I(); }
#endif

#ifndef OS_LeaveIntStack
  void OS__LeaveIntStack(void);
  #define OS_LeaveIntStack() {OS_DI(); OS__LeaveIntStack(); }
#endif

void OS_SetFastIntPriorityLimit(OS_UINT Priority);

#if OS_DEBUG
  void OS_AssertCPUInISRMode(void);                   /* OSKern.c   */
#endif



/*********************************************************************
*
*       Resource semaphores
*
**********************************************************************
*/

int      OS_Use             (OS_RSEMA * pRSema);                  /* OSRsem.c      */
int      OS_UseTimed        (OS_RSEMA * pRSema, OS_TIME TimeOut); /* OS_UseTimed.c */
void     OS_Unuse           (OS_RSEMA * pRSema);                  /* OSRsem.c      */
char     OS_Request         (OS_RSEMA * pRSema);                  /* OSRsemRQ.c    */
int      OS_GetSemaValue    (OS_CONST_PTR OS_RSEMA * pRSema);     /* OSRSemGV.c    */
OS_TASK* OS_GetResourceOwner(OS_CONST_PTR OS_RSEMA * pRSema);     /* OSRsemGO.c    */

void     OS_CreateRSema     (OS_RSEMA * pRSema);                  /* OSRsem.c      */
void     OS_DeleteRSema     (OS_RSEMA * pRSema);                  /* OSDelRS.c     */

#define  OS_CREATERSEMA(ps) OS_CreateRSema(ps)

/*********************************************************************
*
*       Counting semaphores
*
**********************************************************************
*/

void    OS_CreateCSema    (OS_CSEMA * pCSema, OS_UINT InitValue);  /* OSCSEM.c             */
void    OS_DeleteCSema    (OS_CSEMA * pCSema);                     /* OSDELCS.c            */
int     OS_GetCSemaValue  (OS_CONST_PTR OS_CSEMA *pCSema);         /* OSCSEMGV.c           */
OS_U8   OS_SetCSemaValue  (OS_CSEMA * pCSema, OS_UINT Value);      /* OS_SetCSemaValue.c   */
void    OS_SignalCSema    (OS_CSEMA * pCSema);                     /* OSCSEM.c             */
void    OS_SignalCSemaMax (OS_CSEMA * pCSema, OS_UINT MaxValue);   /* OS_CSEMA_SignalMax.c */
void    OS_WaitCSema      (OS_CSEMA * pCSema);                     /* OSCSEM.c             */
OS_BOOL OS_WaitCSemaTimed (OS_CSEMA * pCSema, OS_TIME TimeOut);    /* OSCSEMT.c            */
OS_BOOL OS_CSemaRequest   (OS_CSEMA * pCSema);                     /* OSCSEMRQ.c           */

#define OS_CREATECSEMA(ps) OS_CreateCSema((ps), 0)

/*********************************************************************
*
*       Mailboxes
*
**********************************************************************
*/
void OS_CreateMB          (OS_MAILBOX * pMB, OS_U16 sizeofMsg, OS_UINT maxnofMsg, void* Buffer);    /* initialize mailbox */
void OS_ClearMB           (OS_MAILBOX * pMB);
void OS_PutMail           (OS_MAILBOX * pMB, OS_CONST_PTR void* pMail);
char OS_PutMailCond       (OS_MAILBOX * pMB, OS_CONST_PTR void* pMail);
void OS_PutMailFront      (OS_MAILBOX * pMB, OS_CONST_PTR void* pMail);
char OS_PutMailFrontCond  (OS_MAILBOX * pMB, OS_CONST_PTR void* pMail);
void OS_PutMail1          (OS_MAILBOX * pMB, OS_CONST_PTR char* pMail);
char OS_PutMailCond1      (OS_MAILBOX * pMB, OS_CONST_PTR char* pMail);
void OS_PutMailFront1     (OS_MAILBOX * pMB, OS_CONST_PTR char* pMail);
char OS_PutMailFrontCond1 (OS_MAILBOX * pMB, OS_CONST_PTR char* pMail);
void OS_GetMail           (OS_MAILBOX * pMB, void* pDest);
char OS_GetMailCond       (OS_MAILBOX * pMB, void* pDest);
void OS_GetMail1          (OS_MAILBOX * pMB, char* pDest);
char OS_GetMailCond1      (OS_MAILBOX * pMB, char* pDest);
char OS_GetMailTimed      (OS_MAILBOX * pMB, void* pDest, OS_TIME Timeout);
void OS_DeleteMB          (OS_MAILBOX * pMB);
char OS_PeekMail          (OS_CONST_PTR OS_MAILBOX * pMB, void* pDest);
void OS_WaitMail          (OS_MAILBOX * pMB);
char OS_WaitMailTimed     (OS_MAILBOX * pMB, OS_TIME Timeout);

#if OS_DEBUG == 0
  #define OS_GetMessageCnt(pMB) (*(pMB)).nofMsg
#else
  OS_UINT OS_GetMessageCnt(OS_MAILBOX * pMB);   /* get no. of available Messages */
#endif

/*********************************************************************
*
*       Message Queues (OSQ.c)
*
**********************************************************************
*/

void    OS_Q_Create        (OS_Q* pQ, void*pData, OS_UINT Size);
void    OS_Q_Clear         (OS_Q* pQ);                                /* OSQCL.c  */
void    OS_Q_Delete        (OS_Q* pQ);                                /* OSQDEL.c */
int     OS_Q_GetMessageCnt (OS_CONST_PTR OS_Q* pQ);                   /* OSQGMC.c */
int     OS_Q_Put           (OS_Q* pQ, OS_CONST_PTR void* pSrc, OS_UINT Size);
void    OS_Q_PutBlocked    (OS_Q* pQ, OS_CONST_PTR void* pSrc, OS_UINT Size);
char    OS_Q_PutTimed      (OS_Q* pQ, OS_CONST_PTR void* pSrc, OS_UINT Size, OS_TIME Timeout);
int     OS_Q_GetPtr        (OS_Q* pQ, void**ppData);
int     OS_Q_GetPtrCond    (OS_Q* pQ, void**ppData);                  /* OSQGPC.c */
int     OS_Q_GetPtrTimed   (OS_Q* pQ, void**ppData, OS_TIME Timeout); /* OSQGPT.c */
void    OS_Q_Purge         (OS_Q* pQ);
OS_BOOL OS_Q_IsInUse       (OS_CONST_PTR OS_Q* pQ);
int     OS_Q_GetMessageSize(OS_CONST_PTR OS_Q* pQ);
int     OS_Q_PeekPtr       (OS_CONST_PTR OS_Q* pQ, void**ppData);

/*********************************************************************
*
*       Events
*
**********************************************************************
*/

OS_TASK_EVENT OS_ClearEvents          (OS_TASK * pTask);                          /* OSEVENCL.c  */
OS_TASK_EVENT OS_GetEventsOccurred    (OS_CONST_PTR OS_TASK * pTask);             /* OSEVENGE.c  */
void          OS_SignalEvent          (OS_TASK_EVENT Event, OS_TASK * pTask);     /* OSENENS.c   */
OS_TASK_EVENT OS_WaitEvent            (OS_TASK_EVENT EventMask);                  /* OSEVENW.c   */
OS_TASK_EVENT OS_WaitEventTimed       (OS_TASK_EVENT EventMask, OS_TIME TimeOut); /* OSEVENT.c   */
OS_TASK_EVENT OS_WaitSingleEvent      (OS_TASK_EVENT EventMask);                  /* OSEVENWS.c  */
OS_TASK_EVENT OS_WaitSingleEventTimed (OS_TASK_EVENT EventMask, OS_TIME TimeOut); /* OSEVENWST.c */

/*********************************************************************
*
*       Timers(OSTIME.c)
*
**********************************************************************
*/

#if OS_SUPPORT_TIMER

#ifdef OS_SIZEOF_INT
  #if OS_SIZEOF_INT == 2
    #define OS_TIMER_MAX_TIME 0x7F00
  #elif OS_SIZEOF_INT == 4
    #define OS_TIMER_MAX_TIME 0x7FFFFF00
  #else
    #error "OS_SIZEOF_INT not correctly defined"
  #endif
#endif

void    OS_CreateTimer    (OS_TIMER * pTimer, OS_TIMERROUTINE* Callback, OS_TIME Timeout);
void    OS_RetriggerTimer (OS_TIMER * pTimer);
void    OS_StartTimer     (OS_TIMER * pTimer);
void    OS_StopTimer      (OS_TIMER * pTimer);
void    OS_DeleteTimer    (OS_TIMER * pTimer);                  /* OSTIMED.c  */
OS_TIME OS_GetTimerPeriod (OS_CONST_PTR OS_TIMER * pTimer);     /* OSTIMEGP.c */
OS_BOOL OS_GetTimerStatus (OS_CONST_PTR OS_TIMER * pTimer);     /* OSTIMEGS.c */
OS_TIME OS_GetTimerValue  (OS_CONST_PTR OS_TIMER * pTimer);     /* OSTIMEGV.c */
void    OS_SetTimerPeriod (OS_TIMER * pTimer, OS_TIME Period);  /* OSTIMES.c  */

#define OS_CREATETIMER(pTimer,c,d)          \
        OS_CreateTimer((pTimer), (c), (d)); \
        OS_StartTimer(pTimer);

/*********************************************************************
*
*       Extended timers (OSTIMERX.c)
*
**********************************************************************
*/
void    OS_CreateTimerEx (OS_TIMER_EX * pTimerEx, OS_TIMER_EX_ROUTINE* Callback, OS_TIME Timeout, void * pData);

#define OS_RetriggerTimerEx(pTimerEx)         OS_RetriggerTimer(&(pTimerEx)->Timer)
#define OS_StartTimerEx(pTimerEx)             OS_StartTimer(&(pTimerEx)->Timer)
#define OS_StopTimerEx(pTimerEx)              OS_StopTimer(&(pTimerEx)->Timer)
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
*       Heap type memory management (OS_Alloc.c)
*
**********************************************************************

  This functions might not be implemented in all OS ports.
  Therefore declaration depends on condition OS_SUPPORT_OS_ALLOC
  which has to be defined in CPU specific part

*/

#if OS_SUPPORT_OS_ALLOC
  void* OS_malloc   (unsigned int Size);
  void  OS_free     (void* pMemBlock);
  void* OS_realloc  (void* pMemBlock, unsigned int NewSize);
#endif

/*********************************************************************
*
*       Fixed Block memory management
*
**********************************************************************
*/

void    OS_MEMF_Create           (OS_MEMF* pMEMF, void* pPool, OS_UINT NumBlocks, OS_UINT BlockSize);
void    OS_MEMF_Delete           (OS_MEMF* pMEMF);
void*   OS_MEMF_Alloc            (OS_MEMF* pMEMF, int Purpose);
void*   OS_MEMF_AllocTimed       (OS_MEMF* pMEMF, OS_TIME Timeout, int Purpose);
void*   OS_MEMF_Request          (OS_MEMF* pMEMF, int Purpose);
void    OS_MEMF_Release          (OS_MEMF* pMEMF, void* pMemBlock);
void    OS_MEMF_FreeBlock        (void* pMemBlock);
int     OS_MEMF_GetNumFreeBlocks (OS_CONST_PTR OS_MEMF* pMEMF);
OS_BOOL OS_MEMF_IsInPool         (OS_CONST_PTR OS_MEMF* pMEMF, OS_CONST_PTR void* pMemBlock);
int     OS_MEMF_GetMaxUsed       (OS_CONST_PTR OS_MEMF* pMEMF);
int     OS_MEMF_GetNumBlocks     (OS_CONST_PTR OS_MEMF* pMEMF);
int     OS_MEMF_GetBlockSize     (OS_CONST_PTR OS_MEMF* pMEMF);

/*********************************************************************
*
*       Event object module
*
**********************************************************************
*/

/****** Simple OS_EVENT functions ************************************/

void                OS_EVENT_Create      (OS_EVENT* pEvent);
void                OS_EVENT_CreateEx    (OS_EVENT* pEvent, OS_EVENT_RESET_MODE Mode);
void                OS_EVENT_Delete      (OS_EVENT* pEvent);
OS_BOOL             OS_EVENT_Get         (OS_CONST_PTR OS_EVENT* pEvent);
OS_EVENT_RESET_MODE OS_EVENT_GetResetMode(OS_EVENT* pEvent);
void                OS_EVENT_Pulse       (OS_EVENT* pEvent);
void                OS_EVENT_Reset       (OS_EVENT* pEvent);
void                OS_EVENT_Set         (OS_EVENT* pEvent);
void                OS_EVENT_SetResetMode(OS_EVENT* pEvent, OS_EVENT_RESET_MODE Mode);
void                OS_EVENT_Wait        (OS_EVENT* pEvent);
char                OS_EVENT_WaitTimed   (OS_EVENT* pEvent, OS_TIME Timeout);

/*********************************************************************
*
*       Timing support (OSTiming.c)
*
**********************************************************************

 New in V3.04, but not yet documented to the end user. Great for
 run-time measurement.
*/

#define OS_TIMING OS_U32
void    OS_Timing_Start(OS_TIMING* pCycle);
void    OS_Timing_End(OS_TIMING* pCycle);
OS_U32  OS_Timing_Getus(OS_CONST_PTR OS_TIMING* pCycle);
#define OS_Timing_GetCycles(pPara) (*(pPara))

OS_TIME OS_GetNumIdleTicks(void);                              /* OS_GetNumIdleTicks.c */
void    OS_AdjustTime(OS_TIME Time);                           /* OS_AdjustTime.c      */
void    OS_StartTicklessMode(OS_TIME Time, voidRoutine* Hook); /* OS_Tickless.c        */

/*********************************************************************
*
*       OS Power control module OS_Power.c
*
**********************************************************************
*/
#if OS_SUPPORT_POWER
#define OS_POWER_LEVEL0  (0x0001)
#define OS_POWER_LEVEL1  (0x0002)
#define OS_POWER_LEVEL2  (0x0004)
#define OS_POWER_LEVEL3  (0x0008)
#define OS_POWER_LEVEL4  (0x0010)
#define OS_POWER_LEVEL5  (0x0020)
#define OS_POWER_LEVEL6  (0x0040)
#define OS_POWER_LEVEL8  (0x0080)

void    OS_POWER_UsageInc(OS_UINT Index);
void    OS_POWER_UsageDec(OS_UINT Index);
OS_UINT OS_POWER_GetMask(void);
#endif

/*********************************************************************
*
*       CPU load measurement
*
**********************************************************************
*/

void OS_AddLoadMeasurement(int Period, OS_U8 AutoAdjust, OS_I32 DefaultMaxValue);
int  OS_GetLoadMeasurement(void);

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
*       Advanced profiling support (Module OsKern.c)
*
**********************************************************************
*/
#if OS_PROFILE >= 1
  void OS_EnableProfiling(int Period);
  #define OS_DisableProfiling() { OS_ProfilingOn=0; }
#else
  #define OS_EnableProfiling(Period);
  #define OS_DisableProfiling()
#endif

/*********************************************************************
*
*       OS_TICK_HANDLE and sub-macros
*
**********************************************************************
*/
#if ((OS_U32_OP_IS_ATOMIC == 0) || (OS_PTR_OP_IS_ATOMIC == 0))
  #define OS_SET_TIME(t)      \
    {                         \
      OS_DI();                \
      OS_Global.Time = t;     \
      OS_RESTORE_I();         \
    }
#else
  #define OS_SET_TIME(t)    OS_Global.Time = (t);
#endif

#if OS_RR_SUPPORTED
  #define OS_HANDLE_RR()                        \
    if (OS_TimeSlice) {                         \
      if (--OS_TimeSlice==0) {                  \
        OS_Global.Pending.Flag.RoundRobin = 1;  \
      }                                         \
    }
#else
  #define OS_HANDLE_RR()
#endif

#if OS_SUPPORT_TICK_HOOK
  #define OS_HANDLE_TICK_HOOK()               \
  {                                           \
    OS_TICK_HOOK* pHook;                      \
    pHook = OS_pTickHook;                     \
    while (pHook) {                           \
      (*(pHook->pfUser))();                   \
      pHook = pHook->pNext;                   \
    }                                         \
  }
#else
  #define OS_HANDLE_TICK_HOOK()
#endif

#define OS_TICK_HANDLE() {                                                                   \
  OS_I32        Time;                                                                        \
  OS_ASSERT_INT();            /* Ensure OS_EnterInterrupt() or related was called */            \
  Time = OS_Global.Time + 1;  /* Compute new OS_Time */                                         \
  OS_SET_TIME(Time);          /* Update the global time variable as atomic operation */         \
  if ( OS_SUB_TIME((OS_TIME)Time, OS_Global.TimeDex) >= 0) {  /* Check if the delay is expired */   \
    OS_Global.Pending.Flag.Taskswitch = 1;                                                   \
  }                                                                                          \
  OS_HANDLE_RR();                                                                            \
  OS_HANDLE_TICK_HOOK();                                                                     \
}

/*********************************************************************
*
*       Trace support (OSTrace.c)
*
**********************************************************************

Trace support is enabled by defining OS_TRACE 1.
This is automatically done, when OS_LIBMODE_T is defined.

*/

#if OS_TRACE
  /* Declare trace function prototypes */
  void OS_TraceVoid (OS_U8 id);
  void OS_TracePtr  (OS_U8 id, void* p);
  void OS_TraceData (OS_U8 id, int v);
  void OS_TraceDataPtr (OS_U8 id, int v, void*p);
  void OS_TraceU32Ptr  (OS_U8 id, OS_U32 p0, void*p1);
  void OS_TraceEnable(void);
  void OS_TraceDisable(void);
  void OS_TraceEnableAll(void);                              /* OSTrace1.c */
  void OS_TraceDisableAll(void);                             /* OSTrace1.c */
  void OS_TraceEnableId(OS_U8 id);                           /* OSTrace1.c */
  void OS_TraceDisableId(OS_U8 id);                          /* OSTrace1.c */
  void OS_TraceEnableFilterId(OS_U8 FilterIndex, OS_U8 id);  /* OSTrace1.c */
  void OS_TraceDisableFilterId(OS_U8 FilterIndex, OS_U8 id); /* OSTrace1.c */

  /* Trace function macros, used in API functions */
  #define OS_TRACE_VOID(id)            OS_TraceVoid(id)
  #define OS_TRACE_PTR(id, p)          OS_TracePtr ((id), (p))
  #define OS_TRACE_DATA(id, v)         OS_TraceData((id), ((int) (v)))
  #define OS_TRACE_DATA_PTR(id, v, p)  OS_TraceDataPtr((id), ((int) (v)), (p))
  #define OS_TRACE_U32_PTR(id, p0, p1) OS_TraceU32Ptr((id), (p0), (p1))

  #define OS_TRACE_START()                    { OS_EnterRegion();                                  }
  #define OS_TRACE_START_VOID(id)             { OS_EnterRegion(); OS_TraceVoid((id));              }
  #define OS_TRACE_START_PTR(id, p)           { OS_EnterRegion(); OS_TracePtr((id), (p));          }
  #define OS_TRACE_START_DATA(id, v)          { OS_EnterRegion(); OS_TraceData((id), ((int) (v)));         }
  #define OS_TRACE_START_DATA_PTR(id, v, p)   { OS_EnterRegion(); OS_TraceDataPtr((id), ((int) (v)), (p)); }
  #define OS_TRACE_START_U32_PTR(id, p0, p1)  { OS_EnterRegion(); OS_TraceU32Ptr((id), (u), (p));  }
  #define OS_TRACE_END()                      { OS_LeaveRegion();                                  }

#else
  /* Replace trace functions by empty macros if trace is not enabled */
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
  /* Trace function macros, used in API functions. */
  /* Expand to nothing if trace is disabled       */
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
*        RTOSInit and misc.
*
**********************************************************************

The routines declared below are available on all targets.

*/

/* Routines in RTOSInit.c */
void   OS_InitHW(void);
void   OS_Idle(void);
OS_U32 OS_ConvertCycles2us(OS_U32 Cycles);
OS_U32 OS_GetTime_Cycles(void);
void   OS_COM_Init(void);
void   OS_COM_Send1(unsigned char c);

/* Routines in OS_Error.c, delivered as source file */
void   OS_Error(int ErrCode);

OS_INTERWORK void  OS_StartASM(void);               /* RTOS ASM part   */
OS_INTERWORK int   OS_SwitchFromInt(void);          /* RTOS ASM part   */

void   OS_InitKern(void);             /* OSKern.c        */
OS_U8  OS_OnTx(void);                 /* OSComRxTx1.c, Returns 1 on last byte */
OS_INT OS_COM_GetNextChar(void);      /* OSComRxTx1.c, Returns the character to send, or -1  */
void   OS_OnRx(OS_U8 Data);           /* OSComRxTx1.c    */
void   OS_Send1(OS_U8 c);             /* not implemented */
void   OS_SendString(OS_ROM_DATA const char* s);  /* OSSend.c        */
OS_RX_CALLBACK* OS_SetRxCallback(OS_RX_CALLBACK* cb);
OS_U8  OS_EvaPacketEx(const OS_U8 pSrc[], OS_U16 SrcLen, OS_U8** pReturn);  /* OS_EvaPacketEx.c */

void   OS_COM_ClearTxActive(void);   /* OSCom2.c */

#define OS_Start()  OS_Running = 1u; OS_StartASM()

/*********************************************************************
*
*        RTOS misc. optional
*
**********************************************************************

The routines declared below are available on some targets
only. (Because they do not make much sense on others)
*/

/* Timing routines. Their existance depends on the CPU. In general,
  8-bit CPUs require both routines, where 16-bit CPUs require one
  and 32-bit CPUs require none of these.
*/
#ifndef OS_GetTime
  OS_TIME OS_GetTime(void);
#endif
#ifndef OS_GetTime32
  #if (OS_SIZEOF_INT == 4)
    #define OS_GetTime32() (OS_Global.Time)
  #else
    OS_I32 OS_GetTime32(void);
  #endif
#endif

void OS__di(void);
void OS__ei(void);
void OS__SaveIPL(void);
void OS__RestIPL(void);
void OS_SIM_Init(void);

/* Standard tick handler which increment OS time variable by one on each interrupt */
void OS_TICK_Handle(void);       /* New version of OS_TickHandler() without Enter- Leave-Interrupt  */
void OS_TICK_HandleEx(void);     /* OS_TICK_HandleEx.c */
void OS_TICK_HandleNoHook(void); /* Alternate fster tick hanlder without tick-hook-function */
void OS_TICK_Config(unsigned FractPerInt, unsigned FractPerTick);
void OS_TICK_AddHook   (OS_TICK_HOOK* pHook, OS_TICK_HOOK_ROUTINE * pfUser);
void OS_TICK_RemoveHook(OS_CONST_PTR OS_TICK_HOOK* pHook);

/* Routines for the PC-version running in native mode (non-windows) */
void OS_SetIntFreq(OS_UINT f);
void OS_Exit(int code);
void OS_UseDos(void);
void OS_UnuseDos(void);
int  OS_GetKey(void);
int  OS_WaitKey(void);

/* Compatibility with manual */
#define OS_CREATEMB          OS_CreateMB
#define OS_GetUseCnt         OS_GetSemaValue
#define OS_WaitCSema_Timed   OS_WaitCSemaTimed
#define OS_WaitEvent_Timed   OS_WaitEventTimed
#define OS_GetEventsOccured  OS_GetEventsOccurred

/* Compatibility with pre V3.60e versions, renamed functions */
#define OS_HandleTick      OS_TICK_Handle
#define OS_HandleTickEx    OS_TICK_HandleEx
#define OS_AddTickHook     OS_TICK_AddHook
#define OS_RemoveTickHook  OS_TICK_RemoveHook

/* Compatibility with pre V3.82v versions, renamed functions */
#define OS_Terminate       OS_TerminateTask

/********************************************************************/

#ifdef __cplusplus
  }
#endif

#endif /* RTOS_H_INCLUDED */

/****** End Of File *************************************************/
