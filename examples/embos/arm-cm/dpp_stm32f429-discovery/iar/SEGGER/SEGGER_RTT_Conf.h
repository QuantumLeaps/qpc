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

---------------------------END-OF-HEADER------------------------------
File    : SEGGER_RTT_Conf.h
Purpose : Implementation of SEGGER real-time transfer (RTT) which
          allows real-time communication on targets which support
          debugger memory accesses while the CPU is running.
Revision: $Rev: 13430 $

*/

#ifndef SEGGER_RTT_CONF_H
#define SEGGER_RTT_CONF_H

#ifdef __IAR_SYSTEMS_ICC__
  #include <intrinsics.h>
#endif
#ifdef __RENESAS__
  #include <machine.h>
#endif

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#define SEGGER_RTT_MAX_NUM_UP_BUFFERS             (3)     // Max. number of up-buffers (T->H) available on this target    (Default: 3)
#define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS           (3)     // Max. number of down-buffers (H->T) available on this target  (Default: 3)

#define BUFFER_SIZE_UP                            (1024)  // Size of the buffer for terminal output of target, up to host (Default: 1k)
#define BUFFER_SIZE_DOWN                          (16)    // Size of the buffer for terminal input to target from host (Usually keyboard input) (Default: 16)

#define SEGGER_RTT_PRINTF_BUFFER_SIZE             (64u)    // Size of buffer for RTT printf to bulk-send chars via RTT     (Default: 64)

#define SEGGER_RTT_MODE_DEFAULT                   SEGGER_RTT_MODE_NO_BLOCK_SKIP // Mode for pre-initialized terminal channel (buffer 0)

/*********************************************************************
*
*       RTT memcpy configuration
*
*       memcpy() is good for large amounts of data,
*       but the overhead is big for small amounts, which are usually stored via RTT.
*       With SEGGER_RTT_MEMCPY_USE_BYTELOOP a simple byte loop can be used instead.
*
*       SEGGER_RTT_MEMCPY() can be used to replace standard memcpy() in RTT functions.
*       This is may be required with memory access restrictions,
*       such as on Cortex-A devices with MMU.
*/
#define SEGGER_RTT_MEMCPY_USE_BYTELOOP              0 // 0: Use memcpy/SEGGER_RTT_MEMCPY, 1: Use a simple byte-loop
//
// Example definition of SEGGER_RTT_MEMCPY to external memcpy with GCC toolchains and Cortex-A targets
//
//#if ((defined __SES_ARM) || (defined __CROSSWORKS_ARM) || (defined __GNUC__)) && (defined (__ARM_ARCH_7A__))
//  #define SEGGER_RTT_MEMCPY(pDest, pSrc, NumBytes)      SEGGER_memcpy((pDest), (pSrc), (NumBytes))
//#endif

//
// Target is not allowed to perform other RTT operations while string still has not been stored completely.
// Otherwise we would probably end up with a mixed string in the buffer.
// If using  RTT from within interrupts, multiple tasks or multi processors, define the SEGGER_RTT_LOCK() and SEGGER_RTT_UNLOCK() function here.
//
// SEGGER_RTT_MAX_INTERRUPT_PRIORITY can be used in the sample lock routines on Cortex-M3/4.
// Make sure to mask all interrupts which can send RTT data, i.e. generate SystemView events, or cause task switches.
// When high-priority interrupts must not be masked while sending RTT data, SEGGER_RTT_MAX_INTERRUPT_PRIORITY needs to be adjusted accordingly.
// (Higher priority = lower priority number)
// Default value for embOS: 128u
// Default configuration in FreeRTOS: configMAX_SYSCALL_INTERRUPT_PRIORITY: ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
// In case of doubt mask all interrupts: 1 << (8 - BASEPRI_PRIO_BITS) i.e. 1 << 5 when 3 bits are implemented in NVIC
// or define SEGGER_RTT_LOCK() to completely disable interrupts.
//

#define SEGGER_RTT_MAX_INTERRUPT_PRIORITY         (0x20)   // Interrupt priority to lock on SEGGER_RTT_LOCK on Cortex-M3/4 (Default: 0x20)

/*********************************************************************
*
*       RTT lock configuration for SEGGER Embedded Studio,
*       Rowley CrossStudio and GCC
*/
#if (defined __SES_ARM) || (defined __CROSSWORKS_ARM) || (defined __GNUC__) || (defined __clang__) || (defined __SES_RISCV)
  #if (defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_8M_BASE__))
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                    unsigned int LockState;                                         \
                                  __asm volatile ("mrs   %0, primask  \n\t"                         \
                                                  "movs  r1, $1       \n\t"                         \
                                                  "msr   primask, r1  \n\t"                         \
                                                  : "=r" (LockState)                                \
                                                  :                                                 \
                                                  : "r1"                                            \
                                                  );

    #define SEGGER_RTT_UNLOCK()   __asm volatile ("msr   primask, %0  \n\t"                         \
                                                  :                                                 \
                                                  : "r" (LockState)                                 \
                                                  :                                                 \
                                                  );                                                \
                                }
  #elif (defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_8M_MAIN__))
    #ifndef   SEGGER_RTT_MAX_INTERRUPT_PRIORITY
      #define SEGGER_RTT_MAX_INTERRUPT_PRIORITY   (0x20)
    #endif
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                    unsigned int LockState;                                         \
                                  __asm volatile ("mrs   %0, basepri  \n\t"                         \
                                                  "mov   r1, %1       \n\t"                         \
                                                  "msr   basepri, r1  \n\t"                         \
                                                  : "=r" (LockState)                                \
                                                  : "i"(SEGGER_RTT_MAX_INTERRUPT_PRIORITY)          \
                                                  : "r1"                                            \
                                                  );

    #define SEGGER_RTT_UNLOCK()   __asm volatile ("msr   basepri, %0  \n\t"                         \
                                                  :                                                 \
                                                  : "r" (LockState)                                 \
                                                  :                                                 \
                                                  );                                                \
                                }

  #elif defined(__ARM_ARCH_7A__)
    #define SEGGER_RTT_LOCK() {                                                \
                                 unsigned int LockState;                       \
                                 __asm volatile ("mrs r1, CPSR \n\t"           \
                                                 "mov %0, r1 \n\t"             \
                                                 "orr r1, r1, #0xC0 \n\t"      \
                                                 "msr CPSR_c, r1 \n\t"         \
                                                 : "=r" (LockState)            \
                                                 :                             \
                                                 : "r1"                        \
                                                 );

    #define SEGGER_RTT_UNLOCK() __asm volatile ("mov r0, %0 \n\t"              \
                                                "mrs r1, CPSR \n\t"            \
                                                "bic r1, r1, #0xC0 \n\t"       \
                                                "and r0, r0, #0xC0 \n\t"       \
                                                "orr r1, r1, r0 \n\t"          \
                                                "msr CPSR_c, r1 \n\t"          \
                                                :                              \
                                                : "r" (LockState)              \
                                                : "r0", "r1"                   \
                                                );                             \
                            }
  #elif defined(__riscv) || defined(__riscv_xlen)
    #define SEGGER_RTT_LOCK()  {                                               \
                                 unsigned int LockState;                       \
                                 __asm volatile ("csrr  %0, mstatus  \n\t"     \
                                                 "csrci mstatus, 8   \n\t"     \
                                                 "andi  %0, %0,  8   \n\t"     \
                                                 : "=r" (LockState)            \
                                                 :                             \
                                                 :                             \
                                                );

  #define SEGGER_RTT_UNLOCK()    __asm volatile ("csrs  mstatus, %0  \n\t"     \
                                                 :                             \
                                                 : "r"  (LockState)            \
                                                 :                             \
                                                );                             \
                               }
  #else
    #define SEGGER_RTT_LOCK()
    #define SEGGER_RTT_UNLOCK()
  #endif
#endif

/*********************************************************************
*
*       RTT lock configuration for IAR EWARM
*/
#ifdef __ICCARM__
  #if (defined (__ARM6M__)          && (__CORE__ == __ARM6M__))             ||                      \
      (defined (__ARM8M_BASELINE__) && (__CORE__ == __ARM8M_BASELINE__))
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                  unsigned int LockState;                                           \
                                  LockState = __get_PRIMASK();                                      \
                                  __set_PRIMASK(1);

    #define SEGGER_RTT_UNLOCK()   __set_PRIMASK(LockState);                                         \
                                }
  #elif (defined (__ARM7EM__)         && (__CORE__ == __ARM7EM__))          ||                      \
        (defined (__ARM7M__)          && (__CORE__ == __ARM7M__))           ||                      \
        (defined (__ARM8M_MAINLINE__) && (__CORE__ == __ARM8M_MAINLINE__))  ||                      \
        (defined (__ARM8M_MAINLINE__) && (__CORE__ == __ARM8M_MAINLINE__))
    #ifndef   SEGGER_RTT_MAX_INTERRUPT_PRIORITY
      #define SEGGER_RTT_MAX_INTERRUPT_PRIORITY   (0x20)
    #endif
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                  unsigned int LockState;                                           \
                                  LockState = __get_BASEPRI();                                      \
                                  __set_BASEPRI(SEGGER_RTT_MAX_INTERRUPT_PRIORITY);

    #define SEGGER_RTT_UNLOCK()   __set_BASEPRI(LockState);                                         \
                                }
  #endif
#endif

/*********************************************************************
*
*       RTT lock configuration for IAR RX
*/
#ifdef __ICCRX__
  #define SEGGER_RTT_LOCK()   {                                                                     \
                                unsigned long LockState;                                            \
                                LockState = __get_interrupt_state();                                \
                                __disable_interrupt();

  #define SEGGER_RTT_UNLOCK()   __set_interrupt_state(LockState);                                   \
                              }
#endif

/*********************************************************************
*
*       RTT lock configuration for IAR RL78
*/
#ifdef __ICCRL78__
  #define SEGGER_RTT_LOCK()   {                                                                     \
                                __istate_t LockState;                                               \
                                LockState = __get_interrupt_state();                                \
                                __disable_interrupt();

  #define SEGGER_RTT_UNLOCK()   __set_interrupt_state(LockState);                                   \
                              }
#endif

/*********************************************************************
*
*       RTT lock configuration for KEIL ARM
*/
#ifdef __CC_ARM
  #if (defined __TARGET_ARCH_6S_M)
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                  unsigned int LockState;                                           \
                                  register unsigned char PRIMASK __asm( "primask");                 \
                                  LockState = PRIMASK;                                              \
                                  PRIMASK = 1u;                                                     \
                                  __schedule_barrier();

    #define SEGGER_RTT_UNLOCK()   PRIMASK = LockState;                                              \
                                  __schedule_barrier();                                             \
                                }
  #elif (defined(__TARGET_ARCH_7_M) || defined(__TARGET_ARCH_7E_M))
    #ifndef   SEGGER_RTT_MAX_INTERRUPT_PRIORITY
      #define SEGGER_RTT_MAX_INTERRUPT_PRIORITY   (0x20)
    #endif
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                  unsigned int LockState;                                           \
                                  register unsigned char BASEPRI __asm( "basepri");                 \
                                  LockState = BASEPRI;                                              \
                                  BASEPRI = SEGGER_RTT_MAX_INTERRUPT_PRIORITY;                      \
                                  __schedule_barrier();

    #define SEGGER_RTT_UNLOCK()   BASEPRI = LockState;                                              \
                                  __schedule_barrier();                                             \
                                }
  #endif
#endif

/*********************************************************************
*
*       RTT lock configuration for TI ARM
*/
#ifdef __TI_ARM__
  #if defined (__TI_ARM_V6M0__)
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                  unsigned int LockState;                                           \
                                  LockState = __get_PRIMASK();                                      \
                                  __set_PRIMASK(1);

    #define SEGGER_RTT_UNLOCK()   __set_PRIMASK(LockState);                                         \
                                }
  #elif (defined (__TI_ARM_V7M3__) || defined (__TI_ARM_V7M4__))
    #ifndef   SEGGER_RTT_MAX_INTERRUPT_PRIORITY
      #define SEGGER_RTT_MAX_INTERRUPT_PRIORITY   (0x20)
    #endif
    #define SEGGER_RTT_LOCK()   {                                                                   \
                                  unsigned int LockState;                                           \
                                  LockState = _set_interrupt_priority(SEGGER_RTT_MAX_INTERRUPT_PRIORITY);

    #define SEGGER_RTT_UNLOCK()   _set_interrupt_priority(LockState);                               \
                                }
  #endif
#endif

/*********************************************************************
*
*       RTT lock configuration for CCRX
*/
#ifdef __RX
  #define SEGGER_RTT_LOCK()   {                                                                     \
                                unsigned long LockState;                                            \
                                LockState = get_psw() & 0x010000;                                   \
                                clrpsw_i();

  #define SEGGER_RTT_UNLOCK()   set_psw(get_psw() | LockState);                                     \
                              }
#endif

/*********************************************************************
*
*       RTT lock configuration fallback
*/
#ifndef   SEGGER_RTT_LOCK
  #define SEGGER_RTT_LOCK()                // Lock RTT (nestable)   (i.e. disable interrupts)
#endif

#ifndef   SEGGER_RTT_UNLOCK
  #define SEGGER_RTT_UNLOCK()              // Unlock RTT (nestable) (i.e. enable previous interrupt lock state)
#endif

#endif
/*************************** End of file ****************************/
