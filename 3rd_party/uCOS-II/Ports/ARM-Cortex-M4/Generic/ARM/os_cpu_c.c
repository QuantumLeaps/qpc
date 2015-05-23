/*============================================================================
* Modified from the original to interoperate with CMSIS standard as follows:
* - renamed OS_CPU_SysTickHandler to CMSIS-compatible name SysTick_Handler
* - added expected configuration of the Cortex-M4F FPU to OSInitHookEnd()
* - removed dependency on "lib_def.h"
* - added definition of CPU_STK typdef
* - added definitions of DEF_DISABLED and DEF_ENABLED
* - replaced the use of macros DEF_BIT_FIELD() and DEF_BIT_MASK()
*   with the simple code equivalents
*
* Quantum Leaps, LLC. www.state-machine.com
* 2015-03-24
*===========================================================================*/
/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*
*                           (c) Copyright 2009-2013; Micrium, Inc.; Weston, FL
*                    All rights reserved.  Protected by international copyright laws.
*
*                                           ARM Cortex-M4 Port
*
* File      : OS_CPU_C.C
* Version   : V2.92.09
* By        : JJL
*             BAN
*             JBL
*
* LICENSING TERMS:
* ---------------
*           uC/OS-II is provided in source form for FREE short-term evaluation, for educational use or
*           for peaceful research.  If you plan or intend to use uC/OS-II in a commercial application/
*           product then, you need to contact Micrium to properly license uC/OS-II for its use in your
*           application/product.   We provide ALL the source code for your convenience and to help you
*           experience uC/OS-II.  The fact that the source is provided does NOT mean that you can use
*           it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*
*           Please help us continue to provide the embedded community with the finest software available.
*           Your honesty is greatly appreciated.
*
*           You can contact us at www.micrium.com, or by phone at +1 (954) 217-2036.
*
* For       : ARMv7 Cortex-M4
* Mode      : Thumb-2 ISA
* Toolchain : RealView Development Suite
*             RealView Microcontroller Development Kit (MDK)
*             ARM Developer Suite (ADS)
*             Keil uVision
*********************************************************************************************************
*/

#define   OS_CPU_GLOBALS


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  DEF_DISABLED      0u
#define  DEF_ENABLED       1u

#include  <ucos_ii.h>
#include  <os_cpu.h>       /* QL was: #include  <lib_def.h> */
typedef  INT32U            CPU_STK;  /* QL added: Defines CPU stack data type.              */


/*
*********************************************************************************************************
*                                          LOCAL VARIABLES
*********************************************************************************************************
*/

#if OS_TMR_EN > 0u
static  INT16U  OSTmrCtr;
#endif


/*
*********************************************************************************************************
*                                          SYS TICK DEFINES
*********************************************************************************************************
*/

#define  OS_CPU_CM4_NVIC_ST_CTRL    (*((volatile INT32U *)0xE000E010uL)) /* SysTick Ctrl & Status Reg. */
#define  OS_CPU_CM4_NVIC_ST_RELOAD  (*((volatile INT32U *)0xE000E014uL)) /* SysTick Reload  Value Reg. */
#define  OS_CPU_CM4_NVIC_ST_CURRENT (*((volatile INT32U *)0xE000E018uL)) /* SysTick Current Value Reg. */
#define  OS_CPU_CM4_NVIC_ST_CAL     (*((volatile INT32U *)0xE000E01CuL)) /* SysTick Cal     Value Reg. */
#define  OS_CPU_CM4_NVIC_SHPRI1     (*((volatile INT32U *)0xE000ED18uL)) /* System Handlers  4 to  7 Prio.       */
#define  OS_CPU_CM4_NVIC_SHPRI2     (*((volatile INT32U *)0xE000ED1CuL)) /* System Handlers  8 to 11 Prio.       */
#define  OS_CPU_CM4_NVIC_SHPRI3     (*((volatile INT32U *)0xE000ED20uL)) /* System Handlers 12 to 15 Prio.       */
#define  OS_CPU_CM4_NVIC_CPACR      (*((volatile INT32U *)0xE000ED88uL)) /* Coprocessor Access Control Reg.      */
#define  OS_CPU_CM4_SCB_FPCCR       (*((volatile INT32U *)0xE000EF34uL)) /* Floating-Point Context Control Reg.  */


#define  OS_CPU_CM4_NVIC_ST_CTRL_COUNT                    0x00010000uL   /* Count flag.                */
#define  OS_CPU_CM4_NVIC_ST_CTRL_CLK_SRC                  0x00000004uL   /* Clock Source.              */
#define  OS_CPU_CM4_NVIC_ST_CTRL_INTEN                    0x00000002uL   /* Interrupt enable.          */
#define  OS_CPU_CM4_NVIC_ST_CTRL_ENABLE                   0x00000001uL   /* Counter mode.              */
#define  OS_CPU_CM4_NVIC_PRIO_MIN                               0xFFu    /* Min handler prio.          */

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSInitHookBegin (void)
{
    INT32U   size;
    OS_STK  *pstk;

                                                                /* Clear exception stack for stack checking.            */
    pstk = &OS_CPU_ExceptStk[0];
    size = OS_CPU_EXCEPT_STK_SIZE;
    while (size > 0u) {
        size--;
       *pstk++ = (OS_STK)0;
    }

                                                                /* Align the ISR stack to 8-bytes                       */
    OS_CPU_ExceptStkBase = (OS_STK *)&OS_CPU_ExceptStk[OS_CPU_EXCEPT_STK_SIZE];
    OS_CPU_ExceptStkBase = (OS_STK *)((OS_STK)(OS_CPU_ExceptStkBase) & 0xFFFFFFF8);

#if OS_TMR_EN > 0u
    OSTmrCtr = 0u;
#endif
}
#endif


/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSInitHookEnd (void)
{
#if (OS_CPU_ARM_FP_EN > 0u)
    /* Enable access to Floating-point coprocessor.         */
    OS_CPU_CM4_NVIC_CPACR |= ((3UL << 10*2) | (3UL << 11*2));

    /* Explictily Disable the automatic FPU state preservation as well as the FPU lazy stacking */
    OS_CPU_CM4_SCB_FPCCR &= ~((1U << 31) | (1U << 30));
#endif
}
#endif


/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTaskCreateHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskCreateHook(ptcb);
#else
    (void)ptcb;                                  /* Prevent compiler warning                           */
#endif
}
#endif


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTaskDelHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskDelHook(ptcb);
#else
    (void)ptcb;                                  /* Prevent compiler warning                           */
#endif
}
#endif


/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTaskIdleHook (void)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskIdleHook();
#endif
}
#endif


/*
*********************************************************************************************************
*                                            TASK RETURN HOOK
*
* Description: This function is called if a task accidentally returns.  In other words, a task should
*              either be an infinite loop or delete itself when done.
*
* Arguments  : ptcb      is a pointer to the task control block of the task that is returning.
*
* Note(s)    : none
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0u
void  OSTaskReturnHook (OS_TCB  *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskReturnHook(ptcb);
#else
    (void)ptcb;
#endif
}
#endif


/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0u
void  OSTaskStatHook (void)
{
#if OS_APP_HOOKS_EN > 0u
    App_TaskStatHook();
#endif
}
#endif


/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              p_arg         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_xxx).
*
* Returns    : Always returns the location of the new top-of-stack once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : 1) Interrupts are enabled when your task starts executing.
*              2) All tasks run in Thread mode, using process stack.
*********************************************************************************************************
*/

OS_STK *OSTaskStkInit (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt)
{
    OS_STK *p_stk;


    (void)opt;                                                  /* 'opt' is not used, prevent warning                   */
    p_stk      = ptos + 1u;                                     /* Load stack pointer                                   */
                                                                /* Align the stack to 8-bytes.                          */
    p_stk      = (OS_STK *)((OS_STK)(p_stk) & 0xFFFFFFF8u);
                                                                /* Registers stacked as if auto-saved on exception      */
    *(--p_stk) = (OS_STK)0x01000000uL;                          /* xPSR                                                 */
    *(--p_stk) = (OS_STK)task;                                  /* Entry Point                                          */
    *(--p_stk) = (OS_STK)OS_TaskReturn;                         /* R14 (LR)                                             */
    *(--p_stk) = (OS_STK)0x12121212uL;                          /* R12                                                  */
    *(--p_stk) = (OS_STK)0x03030303uL;                          /* R3                                                   */
    *(--p_stk) = (OS_STK)0x02020202uL;                          /* R2                                                   */
    *(--p_stk) = (OS_STK)0x01010101uL;                          /* R1                                                   */
    *(--p_stk) = (OS_STK)p_arg;                                 /* R0 : argument                                        */

                                                                /* Remaining registers saved on process stack           */
    *(--p_stk) = (OS_STK)0x11111111uL;                          /* R11                                                  */
    *(--p_stk) = (OS_STK)0x10101010uL;                          /* R10                                                  */
    *(--p_stk) = (OS_STK)0x09090909uL;                          /* R9                                                   */
    *(--p_stk) = (OS_STK)0x08080808uL;                          /* R8                                                   */
    *(--p_stk) = (OS_STK)0x07070707uL;                          /* R7                                                   */
    *(--p_stk) = (OS_STK)0x06060606uL;                          /* R6                                                   */
    *(--p_stk) = (OS_STK)0x05050505uL;                          /* R5                                                   */
    *(--p_stk) = (OS_STK)0x04040404uL;                          /* R4                                                   */

#if (OS_CPU_ARM_FP_EN == DEF_ENABLED)
    if ((opt & OS_TASK_OPT_SAVE_FP) != (INT16U)0) {
        *--p_stk = (CPU_STK)0x02000000u;                        /* FPSCR                                                  */
                                                                /* Initialize S0-S31 floating point registers             */
        *--p_stk = (CPU_STK)0x41F80000u;                        /* S31                                                    */
        *--p_stk = (CPU_STK)0x41F00000u;                        /* S30                                                    */
        *--p_stk = (CPU_STK)0x41E80000u;                        /* S29                                                    */
        *--p_stk = (CPU_STK)0x41E00000u;                        /* S28                                                    */
        *--p_stk = (CPU_STK)0x41D80000u;                        /* S27                                                    */
        *--p_stk = (CPU_STK)0x41D00000u;                        /* S26                                                    */
        *--p_stk = (CPU_STK)0x41C80000u;                        /* S25                                                    */
        *--p_stk = (CPU_STK)0x41C00000u;                        /* S24                                                    */
        *--p_stk = (CPU_STK)0x41B80000u;                        /* S23                                                    */
        *--p_stk = (CPU_STK)0x41B00000u;                        /* S22                                                    */
        *--p_stk = (CPU_STK)0x41A80000u;                        /* S21                                                    */
        *--p_stk = (CPU_STK)0x41A00000u;                        /* S20                                                    */
        *--p_stk = (CPU_STK)0x41980000u;                        /* S19                                                    */
        *--p_stk = (CPU_STK)0x41900000u;                        /* S18                                                    */
        *--p_stk = (CPU_STK)0x41880000u;                        /* S17                                                    */
        *--p_stk = (CPU_STK)0x41800000u;                        /* S16                                                    */
        *--p_stk = (CPU_STK)0x41700000u;                        /* S15                                                    */
        *--p_stk = (CPU_STK)0x41600000u;                        /* S14                                                    */
        *--p_stk = (CPU_STK)0x41500000u;                        /* S13                                                    */
        *--p_stk = (CPU_STK)0x41400000u;                        /* S12                                                    */
        *--p_stk = (CPU_STK)0x41300000u;                        /* S11                                                    */
        *--p_stk = (CPU_STK)0x41200000u;                        /* S10                                                    */
        *--p_stk = (CPU_STK)0x41100000u;                        /* S9                                                     */
        *--p_stk = (CPU_STK)0x41000000u;                        /* S8                                                     */
        *--p_stk = (CPU_STK)0x40E00000u;                        /* S7                                                     */
        *--p_stk = (CPU_STK)0x40C00000u;                        /* S6                                                     */
        *--p_stk = (CPU_STK)0x40A00000u;                        /* S5                                                     */
        *--p_stk = (CPU_STK)0x40800000u;                        /* S4                                                     */
        *--p_stk = (CPU_STK)0x40400000u;                        /* S3                                                     */
        *--p_stk = (CPU_STK)0x40000000u;                        /* S2                                                     */
        *--p_stk = (CPU_STK)0x3F800000u;                        /* S1                                                     */
        *--p_stk = (CPU_STK)0x00000000u;                        /* S0                                                     */
    }
#endif

    return (p_stk);
}


/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
#if (OS_CPU_HOOKS_EN > 0u) && (OS_TASK_SW_HOOK_EN > 0u)
void  OSTaskSwHook (void)
{

#if (OS_CPU_ARM_FP_EN > 0u)
    if ((OSTCBCur->OSTCBOpt & OS_TASK_OPT_SAVE_FP) != (INT16U)0) {
        OS_CPU_FP_Reg_Push(OSTCBCur->OSTCBStkPtr);
    }

    if ((OSTCBHighRdy->OSTCBOpt & OS_TASK_OPT_SAVE_FP) != (INT16U)0) {
        OS_CPU_FP_Reg_Pop(OSTCBHighRdy->OSTCBStkPtr);
    }
#endif

#if OS_APP_HOOKS_EN > 0u
    App_TaskSwHook();
#endif
}
#endif


/*
*********************************************************************************************************
*                                           OS_TCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0u
void  OSTCBInitHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0u
    App_TCBInitHook(ptcb);
#else
    (void)ptcb;                                  /* Prevent compiler warning                           */
#endif
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if (OS_CPU_HOOKS_EN > 0u) && (OS_TIME_TICK_HOOK_EN > 0u)
void  OSTimeTickHook (void)
{
#if OS_APP_HOOKS_EN > 0u
    App_TimeTickHook();
#endif

#if OS_TMR_EN > 0u
    OSTmrCtr++;
    if (OSTmrCtr >= (OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC)) {
        OSTmrCtr = 0;
        OSTmrSignal();
    }
#endif
}
#endif


/*
*********************************************************************************************************
*                                          SYS TICK HANDLER
*
* Description: Handle the system tick (SysTick) interrupt, which is used to generate the uC/OS-II tick
*              interrupt.
*
* Arguments  : None.
*
* Note(s)    : 1) This function MUST be placed on entry 15 of the Cortex-M3 vector table.
*********************************************************************************************************
*/

void  SysTick_Handler (void)  /* QL was: void  OS_CPU_SysTickHandler (void) */
{
    OS_CPU_SR  cpu_sr;


    OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */

    OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}


/*
*********************************************************************************************************
*                                         INITIALIZE SYS TICK
*
* Description: Initialize the SysTick.
*
* Arguments  : cnts         Number of SysTick counts between two OS tick interrupts.
*
* Note(s)    : 1) This function MUST be called after OSStart() & after processor initialization.
*********************************************************************************************************
*/

void  OS_CPU_SysTickInit (INT32U  cnts)
{
    INT32U  prio;


    OS_CPU_CM4_NVIC_ST_RELOAD = cnts - 1u;

                                                            /* Set SysTick handler prio.                              */
    prio  = OS_CPU_CM4_NVIC_SHPRI3;
    prio &= 0x00FFFFFF;                     /* QL was: DEF_BIT_FIELD(24, 0); */
    prio |= OS_CPU_CFG_SYSTICK_PRIO << 24;  /* QL was: DEF_BIT_MASK(OS_CPU_CFG_SYSTICK_PRIO, 24); */

    OS_CPU_CM4_NVIC_SHPRI3 = prio;

                                                            /* Enable timer.                                          */
    OS_CPU_CM4_NVIC_ST_CTRL |= OS_CPU_CM4_NVIC_ST_CTRL_CLK_SRC |
                               OS_CPU_CM4_NVIC_ST_CTRL_ENABLE;
                                                            /* Enable timer interrupt.                                */
    OS_CPU_CM4_NVIC_ST_CTRL |= OS_CPU_CM4_NVIC_ST_CTRL_INTEN;
}
