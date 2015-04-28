/*********************************************************************
* This file has been modified from the original as follows:
* - added call to BSP_onIdle() to the OS_Idle() function
* - changed conditional compilation in OS_Idle() to
*   (defined NDEBUG) instead of (DEBUG == 0)
* - deleted WFI mode, because it can lock-up hardware
*   (such as STM32F4-Discovery board).
*
* Quantum Leaps, www.state-machine.com
* 2015-04-23
**********************************************************************/
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
File    : RTOSInit_STM32F4x_CMSIS.c for STM32F4x

Purpose : Initializes and handles the hardware for embOS as far
          as required by embOS
          Feel free to modify this file acc. to your target system.
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "RTOS.h"
#include "stm32f4xx.h"     // Device specific header file, contains CMSIS

/*********************************************************************
*
*       Configuration
*
**********************************************************************
*/

/*********************************************************************
*
*       Clock frequency settings
*/
#ifndef   OS_FSYS                   /* CPU Main clock frequency     */
  #define OS_FSYS SystemCoreClock
#endif

#ifndef   OS_PCLK_TIMER             /* Peripheral clock for timer   */
  #define OS_PCLK_TIMER OS_FSYS     /* May vary from CPU clock      */
#endif                              /* depending on CPU             */

#ifndef   OS_PCLK_UART              /* Peripheral clock for UART    */
  #define OS_PCLK_UART OS_FSYS      /* May vary from CPU clock      */
#endif                              /* depending on CPU             */

#ifndef   OS_TICK_FREQ
  #define OS_TICK_FREQ (1000u)
#endif

/*********************************************************************
*
*       Configuration of communication to OSView
*/
#ifndef   OS_VIEW_ENABLE            // Global enable of communication
  #define OS_VIEW_ENABLE    (1)     // Default: on
#endif

#ifndef   OS_VIEW_USE_UART          // If set, UART will be used for communication
  #define OS_VIEW_USE_UART (0)      // Default: 0 => memory access is used
#endif                              // if OS_VIEW_ENABLE is on

/****** End of configurable options *********************************/

/*********************************************************************
*
*       Check configuration
*
**********************************************************************
*/
#ifndef   DEBUG     /* Should normally be defined as project option */
  #define DEBUG  (0)
#endif

/*********************************************************************
*
*       JLINKMEM and UART settings for OSView
*
*       Automatically generated from configuration settings
*/
#define OS_USE_JLINKMEM   (OS_VIEW_ENABLE && (OS_VIEW_USE_UART == 0))

#define OS_UART_USED      (OS_VIEW_ENABLE && (OS_VIEW_USE_UART != 0) && (0))

#if OS_USE_JLINKMEM
  #include "JLINKMEM.h"
#endif

/****** MAP UART initialization function ****************************/

#if  (OS_UART_USED || OS_USE_JLINKMEM)
  #define OS_COM_INIT() OS_COM_Init()
#else
  #define OS_COM_INIT()
#endif

/*********************************************************************
*
*       Vector table
*/
#ifdef __ICCARM__
  #define __Vectors    __vector_table
#else
  extern unsigned char __Vectors;
#endif

/*********************************************************************
*
*       Local defines (sfrs used in RTOSInit.c)
*
**********************************************************************
*/
#define NVIC_VTOR         (*(volatile OS_U32*) (0xE000ED08uL))
#define NVIC_HFSR         (*(volatile OS_U32*) (0xE000ED2CuL))

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

#if OS_USE_JLINKMEM
  // Size of the communication buffer for JLINKMEM
const OS_U32 OS_JLINKMEM_BufferSize = 32u;
#else
const OS_U32 OS_JLINKMEM_BufferSize = 0;     // Communication not used
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       SysTick_Handler()
*
* Function description
*   This is the code that gets called when the processor receives a
*   _SysTick exception. SysTick is used as OS timer tick.
*/
#ifdef __cplusplus
extern "C" {
#endif
void SysTick_Handler(void);      // Avoid warning, Systick_Handler is not prototyped in any CMSIS header
#ifdef __cplusplus
}
#endif
void SysTick_Handler(void) {
  OS_EnterNestableInterrupt();
  OS_TICK_Handle();
  #if OS_USE_JLINKMEM
    JLINKMEM_Process();
  #endif
  OS_LeaveNestableInterrupt();
}

/*********************************************************************
*
*       OS_InitHW()
*
*       Initialize the hardware (timer) required for the OS to run.
*       May be modified, if an other timer should be used
*/
void OS_InitHW(void) {
  OS_IncDI();
  //
  // We assume, the PLL and core clock was already set by the SystemInit() function
  // which was called from the startup code
  // Therefore, we don't have to initialize any hardware here,
  // we just ensure that the system clock variable is updated and then
  // set the periodic system timer tick for embOS.
  //
  SystemCoreClockUpdate();                             // Update the system clock variable (might not have been set before)
  if (SysTick_Config (OS_PCLK_TIMER / OS_TICK_FREQ)) { // Setup SysTick Timer for 1 msec interrupts
    while (1);                                         // Handle Error
  }
  //
  // Initialize NVIC vector base address. Might be necessary for RAM targets or application not running from 0
  //
  NVIC_VTOR = (OS_U32)&__Vectors;
  //
  // Set the interrupt priority for the system timer to 2nd lowest level to ensure the timer can preempt PendSV handler
  //
  NVIC_SetPriority(SysTick_IRQn, (1u << __NVIC_PRIO_BITS) - 2u);

  OS_COM_INIT();
  OS_DecRI();
}

/*********************************************************************
*
*       OS_Idle()
*
*       Please note:
*       This is basically the "core" of the idle loop.
*       This core loop can be changed, but:
*       The idle loop does not have a stack of its own, therefore no
*       functionality should be implemented that relies on the stack
*       to be preserved. However, a simple program loop can be programmed
*       (like toggeling an output or incrementing a counter)
*/
void OS_Idle(void) {     // Idle loop: No task is ready to execute
  extern void BSP_onIdle(void); // prototype

  while (1) {
    BSP_onIdle(); // idle callback to the application
  }
}

/*********************************************************************
*
*       OS_GetTime_Cycles()
*
*       This routine is required for task-info via OSView or high
*       resolution time measurement functions.
*       It returns the system time in timer clock cycles.
*/
OS_U32 OS_GetTime_Cycles(void) {
  OS_U32 Time;
  OS_U32 Cnt;

  Time = OS_GetTime32();
  Cnt  = (OS_PCLK_TIMER/1000u) - SysTick->VAL;
  //
  // Check if timer interrupt pending ...
  //
  if (SCB->ICSR & SCB_ICSR_PENDSTSET_Msk) {
    Cnt = (OS_PCLK_TIMER/1000u) - SysTick->VAL;     // Interrupt pending, re-read timer and adjust result
    Time++;
  }
  return ((OS_PCLK_TIMER/1000u) * Time) + Cnt;
}

/*********************************************************************
*
*       OS_ConvertCycles2us()
*
*       Convert Cycles into micro seconds.
*
*       If your clock frequency is not a multiple of 1 MHz,
*       you may have to modify this routine in order to get proper
*       diagnostics.
*
*       This routine is required for profiling or high resolution time
*       measurement only. It does not affect operation of the OS.
*/
OS_U32 OS_ConvertCycles2us(OS_U32 Cycles) {
  return Cycles/(OS_PCLK_TIMER/1000000u);
}

/*********************************************************************
*
*       Optional communication with embOSView
*
**********************************************************************
*/

#if OS_USE_JLINKMEM                    // Communication via JTAG / SWD

static void _JLINKMEM_OnRx(OS_U8 Data);
static void _JLINKMEM_OnTx(void);
static OS_INT _JLINKMEM_GetNextChar(void);

/*********************************************************************
*
*       _JLINKMEM_OnRx()
*/
static void _JLINKMEM_OnRx(OS_U8 Data) {
  OS_OnRx(Data);
}

/*********************************************************************
*
*       _JLINKMEM_OnTx()
*/
static void _JLINKMEM_OnTx(void) {
  OS_OnTx();
}

/*********************************************************************
*
*       _JLINKMEM_GetNextChar()
*/
static OS_INT _JLINKMEM_GetNextChar(void) {
  return OS_COM_GetNextChar();
}

/*********************************************************************
*
*       OS_COM_Init()
*       Initialize memory access for OSView
*/
void OS_COM_Init(void) {
  JLINKMEM_SetpfOnRx(_JLINKMEM_OnRx);
  JLINKMEM_SetpfOnTx(_JLINKMEM_OnTx);
  JLINKMEM_SetpfGetNextChar(_JLINKMEM_GetNextChar);
}

/*********************************************************************
*
*       OS_COM_Send1()
*       Send one character via memory
*/
void OS_COM_Send1(OS_U8 c) {
  JLINKMEM_SendChar(c);
}

#elif OS_UART_USED   // Communication via UART, can not be implemented generic

  #error "OS_UART currently not supported."

  //
  // Device specific versions of the 3 communication functions
  // OS_COM_IsrHandler(void)
  // OS_COM_Send1(OS_U8 c)
  // OS_COM_Init(void)
  // have to be implemented when a UART shall be used for communication to embOSView.
  // Samples can be found in our device specific projects.

#else  /*  No communication or UART selected, using dummies */

void OS_COM_Send1(OS_U8 c) {
  OS_USEPARA(c);           /* Avoid compiler warning */
  OS_COM_ClearTxActive();  /* Let the OS know that Tx is not busy */
}

#endif /*  OS_UART_USED  */

/****** Final check of configuration ********************************/

#ifndef OS_UART_USED
  #error "OS_UART_USED has to be defined"
#endif

#if (OS_TICK_FREQ != 1000u)
  #error "OS_TICK_FREQ != 1000, ensure OS_GetTime_Cycles() and OS_ConvertCycles2us() work."
#endif

/*********************************************************************
*
*       HardFault_Handler()
*
*       Using a debugger and setting a breakpoint here allows to analyze
*       the cause of the hardfault.
*       Normally, the CPU should not arrive here.
*
*   NOTE:
*       Do not modify the code. The function might fail if local
*       variables are used.
*/
void HardFault_Handler(void) {
  //
  // In case we received a hard fault because
  // of a breakpoint instruction we return.
  // This may happen with the IAR compiler when using semihosting
  // for printf outputs.
  //
  //
  // In case we received a hard fault because
  // of a breakpoint instruction, we return.
  // This may happen when using semihosting for printf.
  //
  if (NVIC_HFSR & (1uL << 31)) {
    NVIC_HFSR |=  (1uL << 31);   // Reset hardfault status
    OS_HardFaultHandler();
    return;                      // Return to interrupted application
  }
  //
  // Otherwise halt execution
  //
  while(1) {
  }
}

/****** End Of File *************************************************/
