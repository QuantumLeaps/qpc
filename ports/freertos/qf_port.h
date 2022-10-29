/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-07-30
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QF/C port to FreeRTOS 10.x
*/
#ifndef QF_PORT_H
#define QF_PORT_H

/* FreeRTOS event queue and thread types */
#define QF_EQUEUE_TYPE        QueueHandle_t
#define QF_OS_OBJECT_TYPE     StaticQueue_t
#define QF_THREAD_TYPE        StaticTask_t

/* The maximum number of active objects in the application, see NOTE1 */
#define QF_MAX_ACTIVE         32U

/* QF interrupt disabling/enabling (task level) */
#define QF_INT_DISABLE()      taskDISABLE_INTERRUPTS()
#define QF_INT_ENABLE()       taskENABLE_INTERRUPTS()

/* QF critical section for FreeRTOS (task level), see NOTE2 */
/* #define QF_CRIT_STAT_TYPE not defined */
#define QF_CRIT_ENTRY(stat_)  taskENTER_CRITICAL()
#define QF_CRIT_EXIT(stat_)   taskEXIT_CRITICAL()

#include "FreeRTOS.h"  /* FreeRTOS master include file, see NOTE3 */
#include "task.h"      /* FreeRTOS task management */
#include "queue.h"     /* FreeRTOS queue management */

#include "qep_port.h"  /* QEP port */
#include "qequeue.h"   /* QF event queue (for deferring events) */
#include "qmpool.h"    /* QF memory pool (for event pools) */
#include "qf.h"        /* QF platform-independent public interface */

/* the "FromISR" versions of the QF APIs, see NOTE3 */
#ifdef Q_SPY

#define QACTIVE_POST_FROM_ISR(me_, e_, pxHigherPrioTaskWoken_, sender_) \
    ((void)QActive_postFromISR_((me_), (e_), QF_NO_MARGIN,              \
                                (pxHigherPrioTaskWoken_), (sender_)))

#define QACTIVE_POST_X_FROM_ISR(me_, e_, margin_,                \
                                pxHigherPrioTaskWoken_, sender_) \
    (QActive_postFromISR_((me_), (e_), (margin_),                \
                          (pxHigherPrioTaskWoken_), (sender_)))

#define QACTIVE_PUBLISH_FROM_ISR(e_, pxHigherPrioTaskWoken_, sender_) \
    (QActive_publishFromISR_((e_), (pxHigherPrioTaskWoken_),          \
                        (void const *)(sender_)))

#define QTIMEEVT_TICK_FROM_ISR(tickRate_, pxHigherPrioTaskWoken_, sender_) \
    (QTimeEvt_tickFromISR_((tickRate_), (pxHigherPrioTaskWoken_), (sender_)))

#else /* ndef Q_SPY */

#define QACTIVE_POST_FROM_ISR(me_, e_, pxHigherPrioTaskWoken_, dummy) \
    ((void)QActive_postFromISR_((me_), (e_), QF_NO_MARGIN,              \
                                (pxHigherPrioTaskWoken_), (void *)0))

#define QACTIVE_POST_X_FROM_ISR(me_, e_, margin_,                \
                                pxHigherPrioTaskWoken_, dummy) \
    (QActive_postFromISR_((me_), (e_), (margin_),                \
                          (pxHigherPrioTaskWoken_), (void *)0))

#define QACTIVE_PUBLISH_FROM_ISR(e_, pxHigherPrioTaskWoken_, dummy) \
    (QActive_publishFromISR_((e_), (pxHigherPrioTaskWoken_),          \
                        (void *)0))

#define QTIMEEVT_TICK_FROM_ISR(tickRate_, pxHigherPrioTaskWoken_, dummy) \
    (QTimeEvt_tickFromISR_((tickRate_), (pxHigherPrioTaskWoken_), (void *)0))

#endif /* Q_SPY */

/* this function only to be used through macros QACTIVE_POST_FROM_ISR()
* and QACTIVE_POST_X_FROM_ISR().
*/
bool QActive_postFromISR_(QActive * const me, QEvt const * const e,
                          uint_fast16_t const margin,
                          BaseType_t * const pxHigherPriorityTaskWoken,
                          void const * const sender);

void QActive_publishFromISR_(QEvt const * const e,
                          BaseType_t * const pxHigherPriorityTaskWoken,
                          void const * const sender);

void QTimeEvt_tickFromISR_(uint_fast8_t const tickRate,
                          BaseType_t * const pxHigherPriorityTaskWoken,
                          void const * const sender);

#define QF_TICK_FROM_ISR(pxHigherPrioTaskWoken_, sender_) \
    QTIMEEVT_TICK_FROM_ISR(0U, pxHigherPrioTaskWoken_, sender_)

#ifdef Q_EVT_CTOR /* Shall the ctor for the ::QEvt class be provided? */

    #define Q_NEW_FROM_ISR(evtT_, sig_, ...)                  \
        (evtT_##_ctor((evtT_ *)QF_newXFromISR_(sizeof(evtT_), \
                      QF_NO_MARGIN, 0), (sig_), ##__VA_ARGS__))

    #define Q_NEW_X_FROM_ISR(e_, evtT_, margin_, sig_, ...) do { \
        (e_) = (evtT_ *)QF_newXFromISR_(sizeof(evtT_),           \
                                 (margin_), 0);                  \
        if ((e_) != (evtT_ *)0) {                                \
            evtT_##_ctor((e_), (sig_), ##__VA_ARGS__);           \
        }                                                        \
     } while (false)

#else

    #define Q_NEW_FROM_ISR(evtT_, sig_)                         \
        ((evtT_ *)QF_newXFromISR_((uint_fast16_t)sizeof(evtT_), \
                                  QF_NO_MARGIN, (sig_)))

    #define Q_NEW_X_FROM_ISR(e_, evtT_, margin_, sig_) ((e_) = \
        (evtT_ *)QF_newXFromISR_((uint_fast16_t)sizeof(evtT_), \
                                 (margin_), (sig_)))

#endif /* Q_EVT_CTOR */

void QF_gcFromISR(QEvt const * const e);

/* this function only to be used through macros Q_NEW_FROM_ISR() and
* Q_NEW_X_FROM_ISR().
*/
QEvt *QF_newXFromISR_(uint_fast16_t const evtSize,
                      uint_fast16_t const margin, enum_t const sig);

void *QMPool_getFromISR(QMPool * const me, uint_fast16_t const margin,
                        uint_fast8_t const qs_id);
void QMPool_putFromISR(QMPool * const me, void *b, uint_fast8_t const qs_id);

enum FreeRTOS_TaskAttrs {
    TASK_NAME_ATTR
};

/* FreeRTOS hooks prototypes (not provided by FreeRTOS) */
#if (configUSE_IDLE_HOOK > 0)
    void vApplicationIdleHook(void);
#endif
#if (configUSE_TICK_HOOK > 0)
    void vApplicationTickHook(void);
#endif
#if (configCHECK_FOR_STACK_OVERFLOW > 0)
    void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
#endif
#if (configSUPPORT_STATIC_ALLOCATION > 0)
    void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                       StackType_t **ppxIdleTaskStackBuffer,
                                       uint32_t *pulIdleTaskStackSize);
#endif

/*==========================================================================*/
/* interface used only inside QF, but not in applications */

#ifdef QP_IMPL
    #define FREERTOS_TASK_PRIO(qp_prio_) \
        ((UBaseType_t)((qp_prio_) + tskIDLE_PRIORITY))

    /* FreeRTOS scheduler locking for QF_publish_() (task context only) */
    #define QF_SCHED_STAT_      \
        UBaseType_t curr_prio;  \
        TaskHandle_t curr_task;
    #define QF_SCHED_LOCK_(prio_) do {                              \
         curr_task = xTaskGetCurrentTaskHandle();                   \
         curr_prio = uxTaskPriorityGet(curr_task);                  \
         if (FREERTOS_TASK_PRIO(prio_) > curr_prio) {               \
             vTaskPrioritySet(curr_task, FREERTOS_TASK_PRIO(prio_));\
         }                                                          \
         else {                                                     \
             curr_prio = tskIDLE_PRIORITY;                          \
         }                                                          \
    } while (0)

    #define QF_SCHED_UNLOCK_()                                      \
         if (curr_prio != tskIDLE_PRIORITY) {                       \
             vTaskPrioritySet(curr_task, curr_prio);                \
         } else ((void)0)

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_, qs_id_) \
        ((e_) = (QEvt *)QMPool_get(&(p_), (m_), (qs_id_)))
    #define QF_EPOOL_PUT_(p_, e_, qs_id_) \
        (QMPool_put(&(p_), (e_), (qs_id_)))

#endif /* ifdef QP_IMPL */

/*==========================================================================*/
/* NOTE1:
* The maximum number of active objects QF_MAX_ACTIVE can be increased to 64,
* inclusive, but it can be reduced to save some memory. Also, the number of
* active objects cannot exceed the number of FreeRTOS task priorities,
* because each QP active object requires a unique priority level.
*
* NOTE2:
* The critical section definition applies only to the FreeRTOS "task level"
* APIs. The "FromISR" APIs are defined separately.
*
* NOTE3:
* The design of FreeRTOS requires using different APIs inside the ISRs
* (the "FromISR" variant) than at the task level. Accordingly, this port
* provides the "FromISR" variants for QP functions and "FROM_ISR" variants
* for QP macros to be used inside ISRs. ONLY THESE "FROM_ISR" VARIANTS
* ARE ALLOWED INSIDE ISRs AND CALLING THE TASK-LEVEL APIs IS AN ERROR.
*/

#endif /* QF_PORT_H */

