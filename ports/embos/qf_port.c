//============================================================================
// QP/C Real-Time Embedded Framework (RTEF)
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com>
// <info@state-machine.com>
//============================================================================
//! @date Last updated on: 2023-11-15
//! @version Last updated for: @ref qpc_7_3_1
//!
//! @file
//! @brief QF/C port to embOS RTOS kernel, generic C11 compiler
//!
#define QP_IMPL           // this is QP implementation
#include "qp_port.h"      // QP port
#include "qp_pkg.h"       // QP package-level interface
#include "qsafe.h"        // QP Functional Safety (FuSa) subsystem
#ifdef Q_SPY              // QS software tracing enabled?
    #include "qs_port.h"  // QS port
    #include "qs_pkg.h"   // QS package-scope internal interface
#else
    #include "qs_dummy.h" // disable the QS software tracing
#endif // Q_SPY

Q_DEFINE_THIS_MODULE("qf_port")

//............................................................................
// define __TARGET_FPU_VFP symbol depending on the compiler...
#if defined (__CC_ARM)          // ARM Compiler
    // in ARM Compiler __TARGET_FPU_VFP is a pre-defined symbol
#elif defined (__ICCARM__)      // IAR Compiler
    #if defined __ARMVFP__
        #define __TARGET_FPU_VFP 1
    #endif
#elif defined (__GNUC__)        // GNU Compiler
    #if defined (__VFP_FP__) && !defined(__SOFTFP__)
        #define __TARGET_FPU_VFP 1
    #endif
#endif

//............................................................................
static void thread_function(void *pVoid); // prototype
static void thread_function(void *pVoid) { // embOS signature
    QActive * const act = (QActive *)pVoid;

#ifdef __TARGET_FPU_VFP
    // does the task use the FPU? see NOTE1
    if ((act->osObject & TASK_USES_FPU) != 0U) {
        OS_ExtendTaskContext_VFP();
    }
#endif  // __TARGET_FPU_VFP

    // event-loop
    for (;;) {
        QEvt const *e = QActive_get_(act);
        // dispatch event (virtual call)
        (*act->super.vptr->dispatch)(&act->super, e, act->prio);
        QF_gc(e); // check if the event is garbage, and collect it if so
    }
}
//............................................................................
void QF_init(void) {
    OS_InitKern();  // initialize embOS
    OS_InitHW();    // initialize the hardware used by embOS
}
//............................................................................
int QF_run(void) {
    QF_onStartup();  // QF callback to configure and start interrupts

    // produce the QS_QF_RUN trace record
    QS_CRIT_STAT
    QS_CRIT_ENTRY();
    QS_BEGIN_PRE_(QS_QF_RUN, 0U)
    QS_END_PRE_()
    QS_CRIT_EXIT();

    OS_Start(); // start embOS multitasking

    return 0; // dummy return to make the compiler happy
}
//............................................................................
void QF_stop(void) {
    QF_onCleanup();  // cleanup callback
}

//............................................................................
void QActive_start_(QActive * const me, QPrioSpec const prioSpec,
                    QEvt const * * const qSto, uint_fast16_t const qLen,
                    void * const stkSto, uint_fast16_t const stkSize,
                    void const * const par)
{
    // create the embOS message box for the AO
    OS_MAILBOX_Create(&me->eQueue,
                (OS_U16)sizeof(QEvt *),
                (OS_UINT)qLen,
                (void *)&qSto[0]);

    me->prio  = (uint8_t)(prioSpec & 0xFFU); // QF-priority of the AO
    me->pthre = 0U; // preemption-threshold (not used for AO registration)
    QActive_register_(me); // make QF aware of this AO

    // top-most initial tran. (virtual call)
    (*me->super.vptr->init)(&me->super, par, me->prio);
    QS_FLUSH(); // flush the trace buffer to the host

    // The embOS priority of the AO thread can be specified in two ways:
    //
    // 1. Implictily based on the AO's priority (embOS uses the same
    //    priority numbering scheme as QP). This option is chosen when
    //    the higher-byte of the prioSpec parameter is set to zero.
    //
    // 2. Explicitly as the higher-byte of the prioSpec parameter.
    //    This option is chosen when the prioSpec parameter is not-zero.
    //    For example, Q_PRIO(10U, 5U) will explicitly specify AO priority
    //    as 10 and embOS priority as 5.
    //
    //    NOTE: The explicit embOS priority is NOT sanity-checked,
    //    so it is the responsibility of the application to ensure that
    //    it is consistent with the AO's priority. An example of
    //    inconsistent setting would be assigning embOS priorities that
    //    would result in a different relative priritization of AO's threads
    //    than indicated by the AO priorities assigned.
    //
    OS_PRIO embos_prio = (prioSpec >> 8U);
    if (embos_prio == 0U) {
        embos_prio = me->prio;
    }

    // create an embOS task for the AO
    OS_TASK_CreateEx(&me->thread,
#if (OS_TRACKNAME != 0)
        me->thread.Name, // the configured task name
#elif
        "AO",            // a generic AO task name
#endif
        embos_prio,      // embOS priority
        &thread_function,
        (void OS_STACKPTR *)stkSto,
        (OS_UINT)stkSize,
        0U, // no AOs at the same prio
        (void *)me);
}
//............................................................................
void QActive_setAttr(QActive *const me, uint32_t attr1, void const *attr2) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    switch (attr1) {
        case TASK_NAME_ATTR: {
#if (OS_TRACKNAME != 0)
            Q_ASSERT_INCRIT(300, me->thread.Name == (char *)0);
            me->thread.Name = (char const *)attr2;
#endif
            break;
        }
        case TASK_USES_FPU:
            me->osObject = attr1;
            break;
        // ...
        default:
            break;
    }
    QF_CRIT_EXIT();
}
//............................................................................
bool QActive_post_(QActive * const me, QEvt const * const e,
                   uint_fast16_t const margin, void const * const sender)
{
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    uint_fast16_t nFree =
        (uint_fast16_t)(me->eQueue.maxMsg - me->eQueue.nofMsg);

    bool status;
    if (margin == QF_NO_MARGIN) {
        if (nFree > 0U) {
            status = true; // can post
        }
        else {
            status = false; // cannot post
            Q_ERROR_INCRIT(510); // must be able to post the event
        }
    }
    else if (nFree > (QEQueueCtr)margin) {
        status = true; // can post
    }
    else {
        status = false; // cannot post
    }

    if (status) { // can post the event?

        QS_BEGIN_PRE_(QS_QF_ACTIVE_POST, me->prio)
            QS_TIME_PRE_();      // timestamp
            QS_OBJ_PRE_(sender); // the sender object
            QS_SIG_PRE_(e->sig); // the signal of the event
            QS_OBJ_PRE_(me);     // this active object (recipient)
            QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // pool-Id&ref-Count
            QS_EQC_PRE_(nFree);  // # free entries
            QS_EQC_PRE_(0U);     // min # free entries (unknown)
        QS_END_PRE_()

        if (QEvt_getPoolNum_(e) != 0U) { // is it a pool event?
            QEvt_refCtr_inc_(e); // increment the reference counter
        }
        QF_CRIT_EXIT();

        char err = OS_MAILBOX_Put(&me->eQueue, (OS_CONST_PTR void *)&e);

        QF_CRIT_ENTRY();
        // posting to the embOS mailbox must succeed, see NOTE3
        Q_ASSERT_INCRIT(520, err == '\0');
    }
    else {

        QS_BEGIN_PRE_(QS_QF_ACTIVE_POST_ATTEMPT, me->prio)
            QS_TIME_PRE_();      // timestamp
            QS_OBJ_PRE_(sender); // the sender object
            QS_SIG_PRE_(e->sig); // the signal of the event
            QS_OBJ_PRE_(me);     // this active object (recipient)
            QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // pool-Id&ref-Count
            QS_EQC_PRE_(nFree);  // # free entries
            QS_EQC_PRE_(margin); // margin requested
        QS_END_PRE_()

    }
    QF_CRIT_EXIT();

    return status;
}
//............................................................................
void QActive_postLIFO_(QActive * const me, QEvt const * const e) {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();

    QS_BEGIN_PRE_(QS_QF_ACTIVE_POST_LIFO, me->prio)
        QS_TIME_PRE_();          // timestamp
        QS_SIG_PRE_(e->sig);     // the signal of this event
        QS_OBJ_PRE_(me);         // this active object
        QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // pool-Id&ref-Count
        QS_EQC_PRE_(me->eQueue.maxMsg - me->eQueue.nofMsg); // # free
        QS_EQC_PRE_(0U);         // min # free entries (unknown)
    QS_END_PRE_()

    if (QEvt_getPoolNum_(e) != 0U) { // is it a pool event?
        QEvt_refCtr_inc_(e); // increment the reference counter
    }
    QF_CRIT_EXIT();

    char err = OS_MAILBOX_PutFront(&me->eQueue, (OS_CONST_PTR void *)&e);

    QF_CRIT_ENTRY();
    // posting to the embOS mailbox must succeed, see NOTE3
    Q_ASSERT_INCRIT(610, err == '\0');
    QF_CRIT_EXIT();
}
//............................................................................
QEvt const *QActive_get_(QActive * const me) {
    QEvt const *e;
    OS_MAILBOX_GetBlocked(&me->eQueue, (void *)&e);

    QS_CRIT_STAT
    QS_CRIT_ENTRY();
    QS_BEGIN_PRE_(QS_QF_ACTIVE_GET, me->prio)
        QS_TIME_PRE_();          // timestamp
        QS_SIG_PRE_(e->sig);     // the signal of this event
        QS_OBJ_PRE_(me);         // this active object
        QS_2U8_PRE_(QEvt_getPoolNum_(e), e->refCtr_); // pool-Id&ref-Count
        QS_EQC_PRE_(me->eQueue.maxMsg - me->eQueue.nofMsg);// # free
    QS_END_PRE_()
    QS_CRIT_EXIT();

    return e;
}

//============================================================================
// NOTE1:
// In case of hardware-supported floating point unit (FPU), a task must
// preserve the FPU registers across the context switch. However, this
// additional overhead is necessary only for tasks that actually use the
// FPU. In this QP-embOS port, an active object task that uses the FPU is
// designated by the QF_TASK_USES_FPU attribute, which can be set with the
// QF_setEmbOsTaskAttr() function. The task attributes must be set *before*
// calling QACTIVE_START(). The task attributes are saved in QActive.osObject
// member.
//
// NOTE3:
// The event posting to embOS mailbox occurs OUTSIDE critical section,
// which means that the remaining margin of available slots in the queue
// cannot be guaranteed. The problem is that interrupts and other tasks can
// preempt the event posting after checking the margin, but before actually
// posting the event to the queue.
//
