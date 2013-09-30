/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                         (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
*
*                                       80x86/80x88 Specific code
*                                          LARGE MEMORY MODEL
*
*                                           Open Watcom 1.6
*
* File         : OS_CPU_C.C
* By           : Jean J. Labrosse
* Updated By   : Miro Samek
*********************************************************************************************************
*/

#define  OS_CPU_GLOBALS
#include "ucos_ii.h"

#include <string.h>                    /* for memcpy()                                                 */

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
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookBegin (void)
{
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
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookEnd (void)
{
}
#endif

/*$PAGE*/
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
#if OS_CPU_HOOKS_EN > 0
void  OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
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
#if OS_CPU_HOOKS_EN > 0
void  OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
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
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
/* defined in the application
void  OSTaskIdleHook (void)
{
}
*/
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

#if OS_CPU_HOOKS_EN > 0
void  OSTaskStatHook (void)
{
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : Interrupts are enabled when your task starts executing. You can change this by setting the
*              PSW to 0x0002 instead.  In this case, interrupts would be disabled upon task startup.  The
*              application code would be responsible for enabling interrupts at the beginning of the task
*              code.  You will need to modify OSTaskIdle() and OSTaskStat() so that they enable
*              interrupts.  Failure to do this will make your system crash!
*********************************************************************************************************
*/

OS_STK  *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    INT16U *stk;
    struct SREGS sregs;

    segread(&sregs);                        /* read the current value of the segment registers         */

    opt    = opt;                           /* 'opt' is not used, prevent warning                      */
    stk    = (INT16U *)ptos;                /* Load stack pointer                                      */
    *stk-- = (INT16U)FP_SEG(pdata);         /* Simulate call to function with argument                 */
    *stk-- = (INT16U)FP_OFF(pdata);
    *stk-- = (INT16U)FP_SEG(task);
    *stk-- = (INT16U)FP_OFF(task);
    *stk-- = (INT16U)0x0202;                /* SW = Interrupts enabled                                 */
    *stk-- = (INT16U)FP_SEG(task);          /* Put pointer to task   on top of stack                   */
    *stk-- = (INT16U)FP_OFF(task);
    *stk-- = (INT16U)0xAAAA;                /* AX = 0xAAAA                                             */
    *stk-- = (INT16U)0xCCCC;                /* CX = 0xCCCC                                             */
    *stk-- = (INT16U)0xDDDD;                /* DX = 0xDDDD                                             */
    *stk-- = (INT16U)0xBBBB;                /* BX = 0xBBBB                                             */
    *stk-- = (INT16U)0x0000;                /* SP = 0x0000                                             */
    *stk-- = (INT16U)0x1111;                /* BP = 0x1111                                             */
    *stk-- = (INT16U)0x2222;                /* SI = 0x2222                                             */
    *stk-- = (INT16U)0x3333;                /* DI = 0x3333                                             */
    *stk-- = (INT16U)0x4444;                /* ES = 0x4444                                             */
    *stk   = sregs.ds;                      /* DS = Current value of DS                                */
    return ((OS_STK *)stk);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                        INITIALIZE A TASK'S STACK FOR FLOATING POINT EMULATION
*
* Description: This function MUST be called BEFORE calling either OSTaskCreate() or OSTaskCreateExt() in
*              order to initialize the task's stack to allow the task to use the Borland floating-point
*              emulation.  The returned pointer MUST be used in the task creation call.
*
*              Ex.:   OS_STK TaskStk[1000];
*
*
*                     void main (void)
*                     {
*                         OS_STK *ptos;
*                         OS_STK *pbos;
*                         INT32U  size;
*
*
*                         OSInit();
*                         .
*                         .
*                         ptos  = &TaskStk[999];
*                         pbos  = &TaskStk[0];
*                         psize = 1000;
*                         OSTaskStkInit_FPE_x86(&ptos, &pbos, &size);
*                         OSTaskCreate(Task, (void *)0, ptos, 10);
*                         .
*                         .
*                         OSStart();
*                     }
*
* Arguments  : pptos         is the pointer to the task's top-of-stack pointer which would be passed to
*                            OSTaskCreate() or OSTaskCreateExt().
*
*              ppbos         is the pointer to the new bottom of stack pointer which would be passed to
*                            OSTaskCreateExt().
*
*              psize         is a pointer to the size of the stack (in number of stack elements).  You
*                            MUST allocate sufficient stack space to leave at least 384 bytes for the
*                            floating-point emulation.
*
* Returns    : The new size of the stack once memory is allocated to the floating-point emulation.
*
* Note(s)    : 1) _SS  is a Borland 'pseudo-register' and returns the contents of the Stack Segment (SS)
*              2) The pointer to the top-of-stack (pptos) will be modified so that it points to the new
*                 top-of-stack.
*              3) The pointer to the bottom-of-stack (ppbos) will be modified so that it points to the new
*                 bottom-of-stack.
*              4) The new size of the stack is adjusted to reflect the fact that memory was reserved on
*                 the stack for the floating-point emulation.
*********************************************************************************************************
*/

/*$PAGE*/
void  OSTaskStkInit_FPE_x86 (OS_STK **pptos, OS_STK **ppbos, INT32U *psize)
{
    INT32U   lin_tos;                                 /* 'Linear' version of top-of-stack    address   */
    INT32U   lin_bos;                                 /* 'Linear' version of bottom-of-stack address   */
    INT16U   seg;
    INT16U   off;
    INT32U   bytes;
    struct SREGS sregs;

    segread(&sregs);                        /* read the current value of the segment registers         */


    seg      = FP_SEG(*pptos);                        /* Decompose top-of-stack pointer into seg:off   */
    off      = FP_OFF(*pptos);
    lin_tos  = ((INT32U)seg << 4) + (INT32U)off;      /* Convert seg:off to linear address             */
    bytes    = *psize * sizeof(OS_STK);               /* Determine how many bytes for the stack        */
    lin_bos  = (lin_tos - bytes + 15) & 0xFFFFFFF0L;  /* Ensure paragraph alignment for BOS            */

    seg      = (INT16U)(lin_bos >> 4);                /* Get new 'normalized' segment                  */
    *ppbos   = (OS_STK *)MK_FP(seg, 0x0000);          /* Create 'normalized' BOS pointer               */
    memcpy(*ppbos, MK_FP(sregs.ss, 0), 384u);         /* Copy FP emulation memory to task's stack      */
    bytes    = bytes - 16;                            /* Loose 16 bytes because of alignment           */
    *pptos   = (OS_STK *)MK_FP(seg, (INT16U)bytes);   /* Determine new top-of-stack                    */
    *ppbos   = (OS_STK *)MK_FP(seg, 384u);            /* Determine new bottom-of-stack                 */
    bytes    = bytes - 384u;
    *psize   = bytes / sizeof(OS_STK);                /* Determine new stack size                      */
}

/*$PAGE*/
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
#if (OS_CPU_HOOKS_EN > 0) && (OS_TASK_SW_HOOK_EN > 0)
void  OSTaskSwHook (void)
{
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
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
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
#if (OS_CPU_HOOKS_EN > 0) && (OS_TIME_TICK_HOOK_EN > 0)
/* defined in the application
void  OSTimeTickHook (void)
{
}
*/
#endif
