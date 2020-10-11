/**
* @file
* @brief QXK/C port to ARM Cortex-M, IAR-ARM toolset
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-10-11
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
/* This QXK port is part of the interanl QP implementation */
#define QP_IMPL 1U
#include "qf_port.h"
#include "qxk_pkg.h"

/* prototypes --------------------------------------------------------------*/
void PendSV_Handler(void);
void NMI_Handler(void);

#define SCnSCB_ICTR  ((uint32_t volatile *)0xE000E004)
#define SCB_SYSPRI   ((uint32_t volatile *)0xE000ED14)
#define NVIC_IP      ((uint32_t volatile *)0xE000E400)
#define NVIC_ICSR    0xE000ED04

/*
* Initialize the exception priorities and IRQ priorities to safe values.
*
* Description:
* On Cortex-M3/M4/M7, this QXK port disables interrupts by means of the
* BASEPRI register. However, this method cannot disable interrupt
* priority zero, which is the default for all interrupts out of reset.
* The following code changes the SysTick priority and all IRQ priorities
* to the safe value QF_BASEPRI, wich the QF critical section can disable.
* This avoids breaching of the QF critical sections in case the
* application programmer forgets to explicitly set priorities of all
* "kernel aware" interrupts.
*
* The interrupt priorities established in QXK_init() can be later
* changed by the application-level code.
*/
void QXK_init(void) {

#if (__ARM_ARCH != 6) /* NOT Cortex-M0/M0+/M1(v6-M, v6S-M) */

    uint32_t n;

    /* set exception priorities to QF_BASEPRI...
    * SCB_SYSPRI1: Usage-fault, Bus-fault, Memory-fault
    */
    SCB_SYSPRI[1] |= (QF_BASEPRI << 16) | (QF_BASEPRI << 8) | QF_BASEPRI;

    /* SCB_SYSPRI2: SVCall */
    SCB_SYSPRI[2] |= (QF_BASEPRI << 24);

    /* SCB_SYSPRI3:  SysTick, PendSV, Debug */
    SCB_SYSPRI[3] |= (QF_BASEPRI << 24) | (QF_BASEPRI << 16) | QF_BASEPRI;

    /* set all implemented IRQ priories to QF_BASEPRI... */
    n = 8U + ((*SCnSCB_ICTR & 0x7U) << 3); /* (# NVIC_PRIO registers)/4 */
    do {
        --n;
        NVIC_IP[n] = (QF_BASEPRI << 24) | (QF_BASEPRI << 16)
                     | (QF_BASEPRI << 8) | QF_BASEPRI;
    } while (n != 0);

#endif /* NOT Cortex-M0/M0+/M1(v6-M, v6S-M) */

    /* SCB_SYSPRI3: PendSV set to the lowest priority 0xFF */
    SCB_SYSPRI[3] |= (0xFFU << 16);
}

/*****************************************************************************
* Initialize the private stack of an extended QXK thread.
*
* NOTE: the function aligns the stack to the 8-byte boundary for compatibility
* with the AAPCS. Additionally, the function pre-fills the stack with the
* known bit pattern (0xDEADBEEF).
*
* NOTE: QXK_stackInit_() must be called before the QXK kernel is made aware
* of this thread. In that case the kernel cannot use the thread yet, so no
* critical section is needed.
*****************************************************************************/
void QXK_stackInit_(void *thr, QXThreadHandler const handler,
                    void * const stkSto, uint_fast16_t const stkSize)
{
    /* round down the stack top to the 8-byte boundary
    * NOTE: ARM Cortex-M stack grows down from hi -> low memory
    */
    uint32_t *sp =
        (uint32_t *)((((uint32_t)stkSto + stkSize) >> 3U) << 3U);
    uint32_t *sp_limit;

    /* synthesize the ARM Cortex-M exception stack frame...*/
    *(--sp) = (1U << 24);    /* xPSR  (just the THUMB bit) */
    *(--sp) = (uint32_t)handler;         /* PC (the thread handler) */
    *(--sp) = (uint32_t)&QXK_threadRet_; /* LR (return from thread) */
    *(--sp) = 0x0000000CU;   /* R12 */
    *(--sp) = 0x00000003U;   /* R3  */
    *(--sp) = 0x00000002U;   /* R2  */
    *(--sp) = 0x00000001U;   /* R1  */
    *(--sp) = (uint32_t)thr; /* R0 parameter to the handler (thread object) */
    *(--sp) = 0x0000000BU;   /* R11 */
    *(--sp) = 0x0000000AU;   /* R10 */
    *(--sp) = 0x00000009U;   /* R9  */
    *(--sp) = 0x00000008U;   /* R8  */
    *(--sp) = 0x00000007U;   /* R7  */
    *(--sp) = 0x00000006U;   /* R6  */
    *(--sp) = 0x00000005U;   /* R5  */
    *(--sp) = 0x00000004U;   /* R4  */

#if (__ARM_FP != 0)          /* if VFP available... */
    *(--sp) = 0xFFFFFFFDU;   /* exception return with VFP state */
    *(--sp) = 0xAAAAAAAAU;   /* stack "aligner" */
#endif                       /* VFP available */

    /* save the top of the stack in the thread's attibute */
    ((QActive *)thr)->osObject = sp;

    /* pre-fill the unused part of the stack with 0xDEADBEEF */
    sp_limit = (uint32_t *)(((((uint32_t)stkSto - 1U) >> 3U) + 1U) << 3U);
    for (; sp >= sp_limit; --sp) {
        *sp = 0xDEADBEEFU;
    }
}

/* NOTE: keep in synch with the QXK_Attr struct in "qxk.h" !!! */
#define QXK_CURR       0
#define QXK_NEXT       4
#define QXK_ACT_PRIO   8
#define QXK_IDLE_THR   12

/* NOTE: keep in synch with the QXK_Attr struct in "qxk.h" !!! */
/*Q_ASSERT_COMPILE(QXK_CURR == offsetof(QXK_Attr, curr));*/
/*Q_ASSERT_COMPILE(QXK_NEXT == offsetof(QXK_Attr, next));*/
/*Q_ASSERT_COMPILE(QXK_ACT_PRIO == offsetof(QXK_Attr, actPrio));*/

/* NOTE: keep in synch with the QActive struct in "qf.h/qxk.h" !!! */
#define QACTIVE_OSOBJ    28
#define QACTIVE_DYN_PRIO 36

/* NOTE: keep in synch with the QActive struct in "qf.h/qxk.h" !!! */
/*Q_ASSERT_COMPILE(QACTIVE_OSOBJ == offsetof(QActive, osObject));*/
/*Q_ASSERT_COMPILE(QACTIVE_DYN_PRIO == offsetof(QActive, dynPrio));*/

/* helper macros to "stringify" values */
#define VAL(x) #x
#define STRINGIFY(x) VAL(x)

/*****************************************************************************
* The PendSV_Handler exception handler is used for handling context switch
* and asynchronous preemption in QXK. The use of the PendSV exception is
* the recommended and most efficient method for performing context switches
* with ARM Cortex-M.
*
* The PendSV exception should have the lowest priority in the whole system
* (0xFF, see QXK_init). All other exceptions and interrupts should have higher
* priority. For example, for NVIC with 2 priority bits all interrupts and
* exceptions must have numerical value of priority lower than 0xC0. In this
* case the interrupt priority levels available to your applications are (in
* the order from the lowest urgency to the highest urgency): 0x80, 0x40, 0x00.
*
* Also, *all* "kernel aware" ISRs in the QXK application must call the
* QXK_ISR_EXIT() macro, which triggers PendSV when it detects a need for
* a context switch or asynchronous preemption.
*
* Due to tail-chaining and its lowest priority, the PendSV exception will be
* entered immediately after the exit from the *last* nested interrupt (or
* exception). In QXK, this is exactly the time when the QXK activator needs to
* handle the asynchronous preemption.
*****************************************************************************/
__stackless
void PendSV_Handler(void) {
__asm (

    /* Prepare some constants before entering a critical section... */
    "  LDR     r3,=QXK_attr_    \n"
    "  LDR     r2,=" STRINGIFY(NVIC_ICSR) "\n" /* Interrupt Control and State */
    "  MOVS    r1,#1            \n"
    "  LSLS    r1,r1,#27        \n" /* r0 := (1 << 27) (UNPENDSVSET bit) */

    /*<<<<<<<<<<<<<<<<<<<<<<< CRITICAL SECTION BEGIN <<<<<<<<<<<<<<<<<<<<<<<<*/
#if (__ARM_ARCH == 6)               /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  CPSID   i                \n" /* disable interrupts (set PRIMASK) */
#else
    "  MOVS    r0,#" STRINGIFY(QF_BASEPRI) "\n"
    "  CPSID   i                \n" /* selectively disable interrutps with BASEPRI */
    "  MSR     BASEPRI,r0       \n" /* apply the workaround the Cortex-M7 erraturm */
    "  CPSIE   i                \n" /* 837070, see SDEN-1068427. */
#endif                              /* M3/M4/M7 */

    /* The PendSV exception handler can be preempted by an interrupt,
    * which might pend PendSV exception again. The following write to
    * ICSR[27] un-pends any such spurious instance of PendSV.
    */
    "  STR     r1,[r2]          \n" /* ICSR[27] := 1 (unpend PendSV) */

    /* Check QXK_attr_.next, which contains the pointer to the next thread
    * to run, which is set in QXK_ISR_EXIT(). This pointer must not be NULL.
    */
    "  LDR     r0,[r3,#" STRINGIFY(QXK_NEXT) "]\n" /* r1 := QXK_attr_.next */
    "  CMP     r0,#0            \n" /* is (QXK_attr_.next == 0)? */
    "  BEQ     PendSV_return    \n" /* branch if (QXK_attr_.next == 0) */

    /* Load pointers into registers... */
    "  MOV     r12,r0           \n" /* save QXK_attr_.next in r12 */
    "  LDR     r2,[r0,#" STRINGIFY(QACTIVE_OSOBJ) "]\n" /* r2 := QXK_attr_.next->osObject */
    "  LDR     r1,[r3,#" STRINGIFY(QXK_CURR) "]\n" /* r1 := QXK_attr_.curr */

    "  CMP     r1,#0            \n" /* (QXK_attr_.curr != 0)? */
    "  BNE     PendSV_save_ex   \n" /* branch if (current thread is extended) */

    "  CMP     r2,#0            \n" /* (QXK_attr_.next->osObject != 0)? */
    "  BNE     PendSV_save_ao   \n" /* branch if (next tread is extended) */

    "PendSV_activate:           \n"
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  PUSH    {r0,lr}          \n" /* ... push lr (EXC_RETURN) plus stack-aligner */
#endif                              /* VFP available */
    /* The QXK activator must be called in a thread context, while this code
    * executes in the handler contex of the PendSV exception. The switch
    * to the Thread mode is accomplished by returning from PendSV using
    * a fabricated exception stack frame, where the return address is
    * QXK_activate_().
    *
    * NOTE: the QXK activator is called with interrupts DISABLED and also
    * it returns with interrupts DISABLED.
    */
    "  MOVS    r3,#1            \n"
    "  LSLS    r3,r3,#24        \n" /* r3 := (1 << 24), set the T bit  (new xpsr) */
    "  LDR     r2,=QXK_activate_\n" /* address of QXK_activate_ */
    "  SUBS    r2,r2,#1         \n" /* align Thumb-address at halfword (new pc) */
    "  LDR     r1,=QXK_thread_ret\n" /* return address after the call   (new lr) */

    "  SUB     sp,sp,#(8*4)     \n" /* reserve space for exception stack frame */
    "  ADD     r0,sp,#(5*4)     \n" /* r0 := 5 registers below the top of stack */
    "  STM     r0!,{r1-r3}      \n" /* save xpsr,pc,lr */

    "  MOVS    r0,#6            \n"
    "  MVNS    r0,r0            \n" /* r0 := ~6 == 0xFFFFFFF9 */
#if (__ARM_ARCH != 6)               /* NOT Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  DSB                      \n" /* ARM Erratum 838869 */
#endif                              /* NOT (v6-M, v6S-M) */
    "  BX      r0               \n" /* exception-return to the QXK activator */

    /*=========================================================================
    * Saving AO-thread before crossing to eXtended-thread
    * expected register contents:
    * r0  -> QXK_attr_.next
    * r1  -> QXK_attr_.curr
    * r2  -> QXK_attr_.next->osObject (SP)
    * r3  -> &QXK_attr_
    * r12 -> QXK_attr_.next
    */
    "PendSV_save_ao:            \n"
#if (__ARM_ARCH == 6)               /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  SUB     sp,sp,#(8*4)     \n" /* make room for 8 registers r4-r11 */
    "  MOV     r0,sp            \n" /* r0 := temporary stack pointer */
    "  STMIA   r0!,{r4-r7}      \n" /* save the low registers */
    "  MOV     r4,r8            \n" /* move the high registers to low registers... */
    "  MOV     r5,r9            \n"
    "  MOV     r6,r10           \n"
    "  MOV     r7,r11           \n"
    "  STMIA   r0!,{r4-r7}      \n" /* save the high registers */
    "  MOV     r0,r12           \n" /* restore QXK_attr_.next in r0 */
#else                               /* M3/M4/M7 */
    "  PUSH    {r4-r11}         \n" /* save r4-r11 on top of the exception frame */
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  TST     lr,#(1 << 4)     \n" /* is it return with the VFP exception frame? */
    "  IT      EQ               \n" /* if lr[4] is zero... */
    "  VSTMDBEQ sp!,{s16-s31}   \n" /* ... save VFP registers s16..s31 */

    "  PUSH    {r0,lr}          \n" /* save the "aligner" and the EXC_RETURN value */
#endif                              /* VFP available */
#endif                              /* M3/M4/M7 */

    "  CMP     r2,#0            \n"
    "  BNE     PendSV_restore_ex\n" /* branch if (QXK_attr_.next->osObject != 0) */
    /* otherwise continue to restoring next AO-thread... */

    /*-------------------------------------------------------------------------
    * Restoring AO-thread after crossing from eXtended-thread
    * expected register contents:
    * r1  -> QXK_attr_.curr
    * r2  -> QXK_attr_.next->osObject (SP)
    * r3  -> &QXK_attr_
    * r12 -> QXK_attr_.next
    */
    "PendSV_restore_ao:         \n"
    "  MOVS    r0,#0            \n"
    "  STR     r0,[r3,#" STRINGIFY(QXK_CURR) "]\n" /* QXK_attr_.curr := 0 */
    /* don't clear QXK_attr_.next, as it might be needed for AO activation */

#if (__ARM_ARCH == 6)               /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  MOV     r0,sp            \n" /* r0 := top of stack */
    "  MOV     r2,r0            \n"
    "  ADDS    r2,r2,#(4*4)     \n" /* point r2 to the 4 high registers r7-r11 */
    "  LDMIA   r2!,{r4-r7}      \n" /* pop the 4 high registers into low registers */
    "  MOV     r8,r4            \n" /* move low registers into high registers */
    "  MOV     r9,r5            \n"
    "  MOV     r10,r6           \n"
    "  MOV     r11,r7           \n"
    "  LDMIA   r0!,{r4-r7}      \n" /* pop the low registers */
    "  ADD     sp,sp,#(8*4)     \n" /* remove 8 registers from the stack */

    "  MOVS    r2,#6            \n"
    "  MVNS    r2,r2            \n" /* r2 := ~6 == 0xFFFFFFF9 */
    "  MOV     lr,r2            \n" /* make sure MSP is used */
#else                               /* M3/M4/M7 */
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  POP     {r0,lr}          \n" /* restore alighner and EXC_RETURN into lr */
    "  TST     lr,#(1 << 4)     \n" /* is it return to the VFP exception frame? */
    "  IT      EQ               \n" /* if EXC_RETURN[4] is zero... */
    "  VLDMIAEQ sp!,{s16-s31}   \n" /* ... restore VFP registers s16..s31 */
#else
    "  BIC     lr,lr,#(1 << 2)  \n" /* make sure MSP is used */
#endif                              /* VFP available */
    "  POP     {r4-r11}         \n" /* restore r4-r11 from the next thread's stack */
#endif                              /* M3/M4/M7 */

    "  MOV     r0,r12           \n" /* r0 := QXK_attr_.next */
    "  MOVS    r2,#" STRINGIFY(QACTIVE_DYN_PRIO) "\n" /* r2 := offset of .dynPrio */
    "  LDRB    r0,[r0,r2]       \n" /* r0 := QXK_attr_.next->dynPrio */
    "  LDRB    r2,[r3,#" STRINGIFY(QXK_ACT_PRIO) "]\n" /* r2 := QXK_attr_.actPrio */
    "  CMP     r2,r0            \n"
    "  BCC     PendSV_activate  \n" /* if (next->dynPrio > topPrio) activate the next AO */

    /* otherwise no activation needed... */
    "  MOVS    r0,#0            \n"
    "  STR     r0,[r3,#" STRINGIFY(QXK_NEXT) "]\n" /* QXK_attr_.next := 0 (clear the next) */

#ifdef QXK_ON_CONTEXT_SW
    "  MOVS    r0,r1            \n" /* r0 := QXK_attr_.curr */
    "  MOV     r1,r12           \n" /* r1 := QXK_attr_.next */
    "  LDR     r2,[r3,#" STRINGIFY(QXK_IDLE_THR) "]\n" /* r2 := idle thr ptr */
    "  CMP     r1,r2            \n"
    "  BNE     PendSV_onContextSw1 \n" /* if (next != idle) call onContextSw */
    "  MOVS    r1,#0            \n" /* otherwise, next := NULL */
    "PendSV_onContextSw1:        \n"
    "  PUSH    {r1,lr}          \n" /* save the aligner + exception lr */
    "  BL      QXK_onContextSw  \n" /* call QXK_onContextSw() */
    "  POP     {r1,r2}          \n" /* restore the aligner + lr into r2 */
    "  MOV     lr,r2            \n" /* restore the exception lr */
#endif /* QXK_ON_CONTEXT_SW */

    /* re-enable interrupts and return from PendSV */
    "PendSV_return:             \n"
#if (__ARM_ARCH == 6)               /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  CPSIE   i                \n" /* enable interrupts (clear PRIMASK) */
#else                               /* M3/M4/M7 */
    "  MOVS    r0,#0            \n"
    "  MSR     BASEPRI,r0       \n" /* enable interrupts (clear BASEPRI) */
    "  DSB                      \n" /* ARM Erratum 838869 */
#endif                              /* M3/M4/M7 */
    /*>>>>>>>>>>>>>>>>>>>>>>>> CRITICAL SECTION END >>>>>>>>>>>>>>>>>>>>>>>>>*/
    "  BX      lr               \n" /* return to the preempted AO-thread */

    /*-------------------------------------------------------------------------
    * Saving extended-thread before crossing to AO-thread
    * expected register contents:
    * r0  -> QXK_attr_.next
    * r1  -> QXK_attr_.curr
    * r2  -> QXK_attr_.next->osObject (SP)
    * r3  -> &QXK_attr_
    * r12 -> QXK_attr_.next
    */
    "PendSV_save_ex:            \n"
    "  MRS     r0,PSP           \n" /* r0 := Process Stack Pointer */
#if (__ARM_ARCH == 6)               /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  SUBS    r0,r0,#(8*4)     \n" /* make room for 8 registers r4-r11 */
    "  MOVS    r1,r0            \n" /* r1 := temporary PSP (do not clobber r0!) */
    "  STMIA   r1!,{r4-r7}      \n" /* save the low registers */
    "  MOV     r4,r8            \n" /* move the high registers to low registers... */
    "  MOV     r5,r9            \n"
    "  MOV     r6,r10           \n"
    "  MOV     r7,r11           \n"
    "  STMIA   r1!,{r4-r7}      \n" /* save the high registers */
    /* NOTE: at this point r0 holds the top of stack */

    "  LDR     r1,[r3,#" STRINGIFY(QXK_CURR) "]\n" /* r1 := QXK_attr_.curr (restore value) */
#else                               /* M3/M4/M7 */
    "  ISB                      \n" /* reset pipeline after fetching PSP */
    "  STMDB   r0!,{r4-r11}     \n" /* save r4-r11 on top of the exception frame */
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  TST     lr,#(1 << 4)     \n" /* is it return with the VFP exception frame? */
    "  IT      EQ               \n" /* if lr[4] is zero... */
    "  VSTMDBEQ r0!,{s16-s31}   \n" /* ... save VFP registers s16..s31 */
    "  STMDB   r0!,{r1,lr}      \n" /* save the "aligner" and the EXC_RETURN value */
#endif                              /* VFP available */
#endif                              /* M3/M4/M7 */

    /* store the SP of the current extended-thread */
    "  STR     r0,[r1,#" STRINGIFY(QACTIVE_OSOBJ) "]\n" /* QXK_attr_.curr->osObject := r0 */
    "  MOV     r0,r12           \n" /* QXK_attr_.next (restore value) */

    "  CMP     r2,#0            \n"
    "  BEQ     PendSV_restore_ao\n" /* branch if (QXK_attr_.next->osObject == 0) */
    /* otherwise continue to restoring next extended-thread... */

    /*-------------------------------------------------------------------------
    * Restoring extended-thread after crossing from AO-thread
    * expected register contents:
    * r0  -> QXK_attr_.next
    * r1  -> QXK_attr_.curr
    * r2  -> QXK_attr_.next->osObject (SP)
    * r3  -> &QXK_attr_
    * r12 -> QXK_attr_.next
    */
    "PendSV_restore_ex:         \n"
#ifdef QXK_ON_CONTEXT_SW
    "  MOVS    r0,r1            \n" /* r0 := QXK_attr_.curr */
    "  MOV     r1,r12           \n" /* r1 := QXK_attr_.next */
    "  LDR     r2,[r3,#" STRINGIFY(QXK_IDLE_THR) "]\n" /* r2 := idle thr ptr */
    "  CMP     r0,r2            \n"
    "  BNE     PendSV_onContextSw2 \n" /* if (curr != idle) call onContextSw */
    "  MOVS    r0,#0            \n" /* otherwise, curr := NULL */
    "PendSV_onContextSw2:       \n"
    "  LDR     r3,=QXK_onContextSw \n"
    "  BLX     r3               \n" /* call QXK_onContextSw() */

    /* restore the AAPCS-clobbered registers after a functin call...  */
    "  LDR     r3,=QXK_attr_    \n"
    "  LDR     r0,[r3,#" STRINGIFY(QXK_NEXT) "]\n" /* r0 := QXK_attr_.next */
    "  LDR     r2,[r0,#" STRINGIFY(QACTIVE_OSOBJ) "]\n" /* r2 := QXK_attr_.curr->osObject */
#endif /* QXK_ON_CONTEXT_SW */

    "  STR     r0,[r3,#" STRINGIFY(QXK_CURR) "]\n" /* QXK_attr_.curr := r0 (QXK_attr_.next) */
    "  MOVS    r0,#0            \n"
    "  STR     r0,[r3,#" STRINGIFY(QXK_NEXT) "]\n" /* QXK_attr_.next := 0 */

    /* exit the critical section */
#if (__ARM_ARCH == 6)               /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  CPSIE   i                \n" /* enable interrupts (clear PRIMASK) */

    "  MOVS    r0,r2            \n" /* r2 := top of stack */
    "  ADDS    r0,r0,#(4*4)     \n" /* point r0 to the 4 high registers r7-r11 */
    "  LDMIA   r0!,{r4-r7}      \n" /* pop the 4 high registers into low registers */
    "  MOV     r8,r4            \n" /* move low registers into high registers */
    "  MOV     r9,r5            \n"
    "  MOV     r10,r6           \n"
    "  MOV     r11,r7           \n"
    "  LDMIA   r2!,{r4-r7}      \n" /* pop the low registers */
    "  MOVS    r2,r0            \n" /* r2 := holds the new top of stack */

    "  MOVS    r1,#2            \n"
    "  MVNS    r1,r1            \n" /* r1 := ~2 == 0xFFFFFFFD */
    "  MOV     lr,r1            \n" /* make sure PSP is used */
#else                               /* M3/M4/M7 */
    "  MOVS    r1,#0            \n"
    "  MSR     BASEPRI,r1       \n" /* enable interrupts (clear BASEPRI) */
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  LDMIA   r2!,{r1,lr}      \n" /* restore aligner and EXC_RETURN into lr */
    "  TST     lr,#(1 << 4)     \n" /* is it return to the VFP exception frame? */
    "  IT      EQ               \n" /* if lr[4] is zero... */
    "  VLDMIAEQ r2!,{s16-s31}   \n" /* ... restore VFP registers s16..s31 */
#else
    "  ORR     lr,lr,#(1 << 2)  \n" /* make sure PSP is used */
#endif                              /* VFP available */
    "  LDMIA   r2!,{r4-r11}     \n" /* restore r4-r11 from the next thread's stack */
#endif                              /* M3/M4/M7 */

    /* set the PSP to the next thread's SP */
    "  MSR     PSP,r2           \n" /* Process Stack Pointer := r2 */

#if (__ARM_ARCH != 6)               /* NOT Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  DSB                      \n" /* ARM Erratum 838869 */
#endif                              /* NOT (v6-M, v6S-M) */
    "  BX      lr               \n" /* return to the next extended-thread */
    );
}

/*****************************************************************************
* QXK_thread_ret is a helper function executed when the QXK activator returns.
*
* NOTE: QXK_thread_ret does not execute in the PendSV context!
* NOTE: QXK_thread_ret executes entirely with interrupts DISABLED.
*****************************************************************************/
__stackless
void QXK_thread_ret(void) {
__asm (

    /* After the QXK activator returns, we need to resume the preempted
    * thread. However, this must be accomplished by a return-from-exception,
    * while we are still in the thread context. The switch to the exception
    * contex is accomplished by triggering the NMI exception.
    * NOTE: The NMI exception is triggered with nterrupts DISABLED,
    * because QXK activator disables interrutps before return.
    */

    /* before triggering the NMI exception, make sure that the
    * VFP stack frame will NOT be used...
    */
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  MRS     r0,CONTROL       \n" /* r0 := CONTROL */
    "  BICS    r0,r0,#4         \n" /* r0 := r0 & ~4 (FPCA bit) */
    "  MSR     CONTROL,r0       \n" /* CONTROL := r0 (clear CONTROL[2] FPCA bit) */
    "  ISB                      \n" /* ISB after MSR CONTROL (ARM AN321,Sect.4.16) */
#endif                              /* VFP available */

    /* trigger NMI to return to preempted task...
    * NOTE: The NMI exception is triggered with nterrupts DISABLED
    */
    "  LDR     r0,=0xE000ED04   \n" /* Interrupt Control and State Register */
    "  MOVS    r1,#1            \n"
    "  LSLS    r1,r1,#31        \n" /* r1 := (1 << 31) (NMI bit) */
    "  STR     r1,[r0]          \n" /* ICSR[31] := 1 (pend NMI) */
    "  B       .                \n" /* wait for preemption by NMI */
    );
}

/*****************************************************************************
* The NMI_Handler exception handler is used for returning back to the
* interrupted task. The NMI exception simply removes its own interrupt
* stack frame from the stack and returns to the preempted task using the
* interrupt stack frame that must be at the top of the stack.
*
* NOTE: The NMI exception is entered with interrupts DISABLED, so it needs
* to re-enable interrupts before it returns to the preempted task.
*****************************************************************************/
__stackless
void NMI_Handler(void) {
__asm (

    "  ADD     sp,sp,#(8*4)     \n" /* remove one 8-register exception frame */

#if (__ARM_ARCH == 6)               /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    "  CPSIE   i                \n" /* enable interrupts (clear PRIMASK) */
    "  BX      lr               \n" /* return to the preempted task */
#else                               /* M3/M4/M7 */
    "  MOVS    r0,#0            \n"
    "  MSR     BASEPRI,r0       \n" /* enable interrupts (clear BASEPRI) */
#if (__ARM_FP != 0)                 /* if VFP available... */
    "  POP     {r0,lr}          \n" /* pop stack aligner and EXC_RETURN to LR */
    "  DSB                      \n" /* ARM Erratum 838869 */
#endif                              /* no VFP */
    "  BX      lr               \n" /* return to the preempted task */
#endif                              /* M3/M4/M7 */
    );
}

#if (__ARM_ARCH == 6) /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */

/*****************************************************************************
* hand-optimized quick LOG2 in assembly (M0/M0+ have no CLZ instruction)
*****************************************************************************/
uint_fast8_t QF_qlog2(uint32_t x) {
    static uint8_t const log2LUT[16] = {
        0U, 1U, 2U, 2U, 3U, 3U, 3U, 3U,
        4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U
    };
    uint_fast8_t n;
    __asm (
        "MOVS    %[n],#0\n"
#if (QF_MAX_ACTIVE > 16U)
        "LSRS    r2,r0,#16\n"
        "BEQ.N   QF_qlog2_1\n"
        "MOVS    %[n],#16\n"
        "MOVS    r0,r2\n"
    "QF_qlog2_1:\n"
#endif
#if (QF_MAX_ACTIVE > 8U)
        "LSRS    r2,r0,#8\n"
        "BEQ.N   QF_qlog2_2\n"
        "ADDS    %[n],%[n],#8\n"
        "MOVS    r0,r2\n"
    "QF_qlog2_2:\n"
#endif
        "LSRS    r2,r0,#4\n"
        "BEQ.N   QF_qlog2_3\n"
        "ADDS    %[n],%[n],#4\n"
        "MOVS    r0,r2\n"
    "QF_qlog2_3:"
        : [n]"=r"(n)
    );
    return n + log2LUT[x];
}

#endif /* Cortex-M0/M0+/M1(v6-M, v6S-M)? */

