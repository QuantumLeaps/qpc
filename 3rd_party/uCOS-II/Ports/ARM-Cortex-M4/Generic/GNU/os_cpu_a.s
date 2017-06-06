/*===========================================================================
* Modified from the original as follows:
* - changed to the GNU-AS syntax
* - renamed OS_CPU_PendSVHandler to CMSIS-compatible name PendSV_Handler
* - changed the conditional compilation for the FPU to the new syntax
*   #ifdef __ARMVFP__/#endif
*
* Quantum Leaps, LLC. https://state-machine.com
* 2017-06-05
*============================================================================
*/
/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*
*                         (c) Copyright 2009-2013; Micrium, Inc.* Weston, FL
*                    All rights reserved.  Protected by international copyright laws.
*
*                                           ARM Cortex-M4 Port
*
* File      : OS_CPU_A.ASM
* Version   : V2.92.09
* By        : JJL
*             BAN
*             JBL
*
* For       : ARMv7 Cortex-M4
* Mode      : Thumb-2 ISA
* Toolchain : IAR EWARM
*********************************************************************************************************
*/

/********************************************************************************************************
*                                     CODE GENERATION DIRECTIVES
********************************************************************************************************/

  .syntax unified
  .thumb

/********************************************************************************************************
*                                               EQUATES
********************************************************************************************************/

  .equ NVIC_INT_CTRL,0xE000ED04                                 /* Interrupt control state register. */
  .equ NVIC_SYSPRI14,0xE000ED22                                 /* System priority register (priority 14) */
  .equ NVIC_PENDSV_PRI,0xFF                                     /* PendSV priority value (lowest). */
  .equ NVIC_PENDSVSET,0x10000000                                /* Value to trigger PendSV exception. */


/********************************************************************************************************
*                                   FLOATING POINT REGISTERS PUSH
*                             void  OS_CPU_FP_Reg_Push (OS_STK  *stkPtr)
*
* Note(s) : 1) This function saves S0-S31, and FPSCR registers of the Floating Point Unit.
*
*           2) Pseudo-code is:
*              a) Get FPSCR register value;
*              b) Push value on process stack;
*              c) Push remaining regs S0-S31 on process stack;
*              d) Update OSTCBCur->OSTCBStkPtr;
********************************************************************************************************/

  .ifdef  __FPU_PRESENT      /* QL was: IF {FPU} != "SoftVFP" */
  .section .text.OS_CPU_FP_Reg_Push
  .global OS_CPU_FP_Reg_Push
  .type   OS_CPU_FP_Reg_Push, %function
OS_CPU_FP_Reg_Push:
    MRS     R1, PSP                                             /* PSP is process stack pointer */
    CBZ     R1, OS_CPU_FP_nosave                                /* Skip FP register save the first time */

    VMRS    R1, FPSCR
    STR R1, [R0, #-4]!
    VSTMDB  R0!, {S0-S31}
    LDR     R1, =OSTCBCur
    LDR     R2, [R1]
    STR     R0, [R2]
OS_CPU_FP_nosave:
    BX      LR
  .size     OS_CPU_FP_Reg_Push, . - OS_CPU_FP_Reg_Push
  .endif                     /* QL was: ENDIF */


/********************************************************************************************************
*                                   FLOATING POINT REGISTERS POP
*                             void  OS_CPU_FP_Reg_Pop (OS_STK  *stkPtr)
*
* Note(s) : 1) This function restores S0-S31, and FPSCR registers of the Floating Point Unit.
*
*           2) Pseudo-code is:
*              a) Restore regs S0-S31 of new process stack;
*              b) Restore FPSCR reg value
*              c) Update OSTCBHighRdy->OSTCBStkPtr pointer of new proces stack;
********************************************************************************************************/

  .ifdef  __FPU_PRESENT      /* QL was: IF {FPU} != "SoftVFP" */
  .section .text.OS_CPU_FP_Reg_Pop
  .global OS_CPU_FP_Reg_Pop
  .type   OS_CPU_FP_Reg_Pop, %function
OS_CPU_FP_Reg_Pop:
    VLDMIA  R0!, {S0-S31}
    LDMIA   R0!, {R1}
    VMSR    FPSCR, R1
    LDR     R1, =OSTCBHighRdy
    LDR     R2, [R1]
    STR     R0, [R2]
    BX      LR
  .size     OS_CPU_FP_Reg_Pop, . - OS_CPU_FP_Reg_Pop
  .endif                     /* QL was: ENDIF */


/********************************************************************************************************
*                                   CRITICAL SECTION METHOD 3 FUNCTIONS
*
* Description: Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*              would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*              disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
*              disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*              into the CPU's status register.
*
* Prototypes :     OS_CPU_SR  OS_CPU_SR_Save(void);
*                  void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
*
*
* Note(s)    : 1) These functions are used in general like this:
*
*                 void Task (void *p_arg)
*                 {
*                 #if OS_CRITICAL_METHOD == 3          // Allocate storage for CPU status register
*                     OS_CPU_SR  cpu_sr;
*                 #endif
*
*                          :
*                          :
*                     OS_ENTER_CRITICAL();             // cpu_sr = OS_CPU_SaveSR();
*                          :
*                          :
*                     OS_EXIT_CRITICAL();              // OS_CPU_RestoreSR(cpu_sr);
*                          :
*                          :
*                 }
********************************************************************************************************/
  .section .text.OS_CPU_SR_Save
  .global OS_CPU_SR_Save
  .type   OS_CPU_SR_Save, %function
OS_CPU_SR_Save:
    MRS     R0, PRIMASK                                         /* Set prio int mask to mask all (except faults) */
    CPSID   I
    BX      LR
  .size   OS_CPU_SR_Save, . - OS_CPU_SR_Save

    .section .text.OS_CPU_SR_Restore
    .global OS_CPU_SR_Restore
    .type   OS_CPU_SR_Restore, %function
OS_CPU_SR_Restore:
    MSR     PRIMASK, R0
    BX      LR
  .size     OS_CPU_SR_Restore, . - OS_CPU_SR_Restore


/********************************************************************************************************
*                                         START MULTITASKING
*                                      void OSStartHighRdy(void)
*
* Note(s) : 1) This function triggers a PendSV exception (essentially, causes a context switch) to cause
*              the first task to start.
*
*           2) OSStartHighRdy() MUST:
*              a) Setup PendSV exception priority to lowest;
*              b) Set initial PSP to 0, to tell context switcher this is first run;
*              c) Set the main stack to OS_CPU_ExceptStkBase
*              d) Set OSRunning to TRUE;
*              e) Trigger PendSV exception;
*              f) Enable interrupts (tasks will run with interrupts enabled).
********************************************************************************************************/

  .section .text.OSStartHighRdy
  .global OSStartHighRdy
  .type   OSStartHighRdy, %function
OSStartHighRdy:
    LDR     R0, =NVIC_SYSPRI14                                  /* Set the PendSV exception priority */
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]

    MOVS    R0, #0                                              /* Set the PSP to 0 for initial context switch call */
    MSR     PSP, R0

    LDR     R0, =OS_CPU_ExceptStkBase                           /* Initialize the MSP to the OS_CPU_ExceptStkBase */
    LDR     R1, [R0]
    MSR     MSP, R1

    LDR     R0, =OSRunning                                      /* OSRunning = TRUE */
    MOVS    R1, #1
    STRB    R1, [R0]

    LDR     R0, =NVIC_INT_CTRL                                  /* Trigger the PendSV exception (causes context switch) */
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]

    CPSIE   I                                                   /* Enable interrupts at processor level */

OSStartHang:
    B       OSStartHang                                         /* Should never get here */
  .size     OSStartHighRdy, . - OSStartHighRdy


/********************************************************************************************************
*                       PERFORM A CONTEXT SWITCH (From task level) - OSCtxSw()
*
* Note(s) : 1) OSCtxSw() is called when OS wants to perform a task context switch.  This function
*              triggers the PendSV exception which is where the real work is done.
********************************************************************************************************/

  .section .text.OSCtxSw
  .global OSCtxSw
  .type   OSCtxSw, %function
OSCtxSw:
    LDR     R0, =NVIC_INT_CTRL                                  /* Trigger the PendSV exception (causes context switch) */
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR
  .size     OSCtxSw, . - OSCtxSw


/********************************************************************************************************
*                   PERFORM A CONTEXT SWITCH (From interrupt level) - OSIntCtxSw()
*
* Note(s) : 1) OSIntCtxSw() is called by OSIntExit() when it determines a context switch is needed as
*              the result of an interrupt.  This function simply triggers a PendSV exception which will
*              be handled when there are no more interrupts active and interrupts are enabled.
********************************************************************************************************/

  .section .text.OSIntCtxSw
  .global OSIntCtxSw
  .type   OSIntCtxSw, %function
OSIntCtxSw:
    LDR     R0, =NVIC_INT_CTRL                                  /* Trigger the PendSV exception (causes context switch) */
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR
  .size     OSIntCtxSw, . - OSIntCtxSw


/********************************************************************************************************
*                                       HANDLE PendSV EXCEPTION
*                                   void OS_CPU_PendSVHandler(void)
*
* Note(s) : 1) PendSV is used to cause a context switch.  This is a recommended method for performing
*              context switches with Cortex-M3.  This is because the Cortex-M3 auto-saves half of the
*              processor context on any exception, and restores same on return from exception.  So only
*              saving of R4-R11 is required and fixing up the stack pointers.  Using the PendSV exception
*              this way means that context saving and restoring is identical whether it is initiated from
*              a thread or occurs due to an interrupt or exception.
*
*           2) Pseudo-code is:
*              a) Get the process SP, if 0 then skip (goto d) the saving part (first context switch);
*              b) Save remaining regs r4-r11 on process stack;
*              c) Save the process SP in its TCB, OSTCBCur->OSTCBStkPtr = SP;
*              d) Call OSTaskSwHook();
*              e) Get current high priority, OSPrioCur = OSPrioHighRdy;
*              f) Get current ready thread TCB, OSTCBCur = OSTCBHighRdy;
*              g) Get new process SP from TCB, SP = OSTCBHighRdy->OSTCBStkPtr;
*              h) Restore R4-R11 from new process stack;
*              i) Perform exception return which will restore remaining context.
*
*           3) On entry into PendSV handler:
*              a) The following have been saved on the process stack (by processor):
*                 xPSR, PC, LR, R12, R0-R3
*              b) Processor mode is switched to Handler mode (from Thread mode)
*              c) Stack is Main stack (switched from Process stack)
*              d) OSTCBCur      points to the OS_TCB of the task to suspend
*                 OSTCBHighRdy  points to the OS_TCB of the task to resume
*
*           4) Since PendSV is set to lowest priority in the system (by OSStartHighRdy() above), we
*              know that it will only be run when no other exception or interrupt is active, and
*              therefore safe to assume that context being switched out was using the process stack (PSP).
********************************************************************************************************/

  .section .text.PendSV_Handler
  .global PendSV_Handler
  .type   PendSV_Handler, %function
PendSV_Handler:    /* QL was: OS_CPU_PendSVHandler */
    CPSID   I                                                   /* Prevent interruption during context switch */
    MRS     R0, PSP                                             /* PSP is process stack pointer */
    CBZ     R0, OS_CPU_PendSVHandler_nosave                     /* Skip register save the first time */

    SUBS    R0, R0, #0x20                                       /* Save remaining regs r4-11 on process stack */
    STM     R0, {R4-R11}

    LDR     R1, =OSTCBCur                                       /* OSTCBCur->OSTCBStkPtr = SP; */
    LDR     R1, [R1]
    STR     R0, [R1]                                            /* R0 is SP of process being switched out */

                                                                /* At this point, entire context of process has been saved */
OS_CPU_PendSVHandler_nosave:
    PUSH    {R14}                                               /* Save LR exc_return value */
    LDR     R0, =OSTaskSwHook                                   /* OSTaskSwHook(); */
    BLX     R0
    POP     {R14}

    LDR     R0, =OSPrioCur                                      /* OSPrioCur = OSPrioHighRdy; */
    LDR     R1, =OSPrioHighRdy
    LDRB    R2, [R1]
    STRB    R2, [R0]

    LDR     R0, =OSTCBCur                                       /* OSTCBCur  = OSTCBHighRdy; */
    LDR     R1, =OSTCBHighRdy
    LDR     R2, [R1]
    STR     R2, [R0]

    LDR     R0, [R2]                                            /* R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr; */
    LDM     R0, {R4-R11}                                        /* Restore r4-11 from new process stack */
    ADDS    R0, R0, #0x20
    MSR     PSP, R0                                             /* Load PSP with new process SP */
    ORR     LR, LR, #0xF4                                       /* Ensure exception return uses process stack */
    CPSIE   I
    BX      LR                                                  /* Exception return will restore remaining context */
  .size     PendSV_Handler, . - PendSV_Handler

  .end
