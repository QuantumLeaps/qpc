/*****************************************************************************
* Product:  QF/C, port to FreeRTOS.org
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qf_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qf_port")

/* Global objects ----------------------------------------------------------*/

/*..........................................................................*/
char const Q_ROM * Q_ROM_VAR QF_getPortVersion(void) {
    static const char Q_ROM Q_ROM_VAR version[] =  "4.4.02";
    return version;
}
/*..........................................................................*/
void QF_init(void) {
    /* no special initialization required by FreeRTOS.org */
}
/*..........................................................................*/
int16_t QF_run(void) {
    vTaskStartScheduler();              /* start the FreeRTOS.org scheduler */
    Q_ERROR();            /* the FreeRTOS.org scheduler should never return */
    return (int16_t)0;   /* this unreachable return keeps the comiler happy */
}
/*..........................................................................*/
void QF_stop(void) {
}
/*..........................................................................*/
static __arm void task_function(void *pvParameters) { /* FreeRTOS signature */
    ((QActive *)pvParameters)->running = (uint8_t)1;  /* enable thread-loop */
    while (((QActive *)pvParameters)->running) {
        QEvt const *e = QActive_get_((QActive *)pvParameters);
        QF_ACTIVE_DISPATCH_(&((QActive *)pvParameters)->super, e);
        QF_gc(e);    /* check if the event is garbage, and collect it if so */
    }

    QF_remove_((QActive *)pvParameters);    /* remove AO from the framework */
    vTaskDelete(((QActive *)pvParameters)->thread); /* delete FreeRTOS task */
}
/*..........................................................................*/
void QActive_start(QActive *me, uint8_t prio,
                   QEvt const *qSto[], uint32_t qLen,
                   void *stkSto, uint32_t stkSize,
                   QEvt const *ie)
{
    portBASE_TYPE err;
    unsigned portBASE_TYPE freeRTOS_prio;

    Q_REQUIRE((qSto == (QEvt const **)0) && (qLen > 0)
              && (stkSto == (void *)0) && (stkSize > 0));

    me->eQueue = xQueueCreate(qLen, sizeof(QEvt*)); /* create event queue */
    Q_ASSERT(me->eQueue != (xQueueHandle)0);      /* FreeRTOS queue created */
    me->prio = prio;                                /* save the QF priority */
    QF_add_(me);                     /* make QF aware of this active object */
    QF_ACTIVE_INIT_(&me->super, ie);          /* execute initial transition */

    QS_FLUSH();                       /* flush the trace buffer to the host */

    freeRTOS_prio = tskIDLE_PRIORITY + me->prio;  /* FreeRTOS task priority */
                                 /* create the FreeRTOS.org task for the AO */
    err = xTaskCreate(&task_function,                  /* the task function */
              (signed portCHAR *)"AO",              /* the name of the task */
              (unsigned portSHORT)stkSize/sizeof(portSTACK_TYPE),  /* stack */
              me,                           /* the 'pvParameters' parameter */
              freeRTOS_prio,                      /* FreeRTOS task priority */
              &me->thread);                                  /* task handle */
    Q_ASSERT(err == pdPASS);                   /* FreeRTOS.org task created */
}
/*..........................................................................*/
void QActive_stop(QActive *me) {
    me->running = (uint8_t)0;                       /* stop the thread loop */
    vQueueDelete(me->eQueue);                          /* cleanup the queue */
}
/*..........................................................................*/
void QActive_postFIFO(QActive *me, QEvt const *e) {
    portBASE_TYPE err;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();
    if (e->dynamic_ != (uint8_t)0) {
        ++((QEvt *)e)->dynamic_;
    }

    QS_BEGIN_NOLOCK_(QS_QF_ACTIVE_POST_FIFO, QS_aoObj_, me)
        QS_TIME_();                                            /* timestamp */
        QS_SIG_(e->sig);                         /* the signal of the event */
        QS_OBJ_(me);                                  /* this active object */
        QS_U8_(e->dynamic_);          /* the dynamic attribute of the event */
        QS_EQC_(0);                     /* number of free entries (unknown) */
        QS_EQC_(0);                 /* min number of free entries (unknown) */
    QS_END_NOLOCK_()

    QF_CRIT_EXIT_();

    if (ulInterruptNesting == (unsigned portLONG)0) {        /* task level? */
        err = xQueueSendToBack(me->eQueue, &e, (portTickType)0);
        Q_ASSERT(err == pdPASS);
    }
    else {                                             /* must be ISR level */
        portBASE_TYPE xHigherPriorityTaskWoken;
        err = xQueueSendToBackFromISR(me->eQueue, &e,
                                      &xHigherPriorityTaskWoken);
        Q_ASSERT(err == pdPASS);
        if (xHigherPriorityTaskWoken == pdTRUE) { /* higher-prio task woken?*/
            vPortYieldFromISR();                    /* invoke the scheduelr */
        }
    }
}
/*..........................................................................*/
void QActive_postLIFO(QActive *me, QEvt const *e) {
    portBASE_TYPE err;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();
    if (e->dynamic_ != (uint8_t)0) {
        ++((QEvt *)e)->dynamic_;
    }

    QS_BEGIN_NOLOCK_(QS_QF_ACTIVE_POST_LIFO, QS_aoObj_, me)
        QS_TIME_();                                            /* timestamp */
        QS_SIG_(e->sig);                         /* the signal of the event */
        QS_OBJ_(me);                                  /* this active object */
        QS_U8_(e->dynamic_);          /* the dynamic attribute of the event */
        QS_EQC_(0);                     /* number of free entries (unknown) */
        QS_EQC_(0);                 /* min number of free entries (unknown) */
    QS_END_NOLOCK_()

    QF_CRIT_EXIT_();

    if (ulInterruptNesting == (unsigned portLONG)0) {        /* task level? */
        err = xQueueSendToFront(me->eQueue, &e, (portTickType)0);
        Q_ASSERT(err == pdPASS);
    }
    else {                                             /* must be ISR level */
        portBASE_TYPE xHigherPriorityTaskWoken;
        err = xQueueSendToFrontFromISR(me->eQueue, &e,
                                       &xHigherPriorityTaskWoken);
        Q_ASSERT(err == pdPASS);
        if (xHigherPriorityTaskWoken == pdTRUE) { /* higher-prio task woken?*/
            vPortYieldFromISR();                    /* invoke the scheduelr */
        }
    }
}
/*..........................................................................*/
QEvt const *QActive_get_(QActive *me) {
    QEvt const *e;
    QS_CRIT_STAT_

              /* wait indefinitely, INCLUDE_vTaskSuspend must be set to '1' */
    Q_ALLEGE(xQueueReceive(me->eQueue, &e, portMAX_DELAY) == pdPASS);

    QS_BEGIN_(QS_QF_ACTIVE_GET, QS_aoObj_, me)
        QS_TIME_();                                            /* timestamp */
        QS_SIG_(e->sig);                        /* the signal of this event */
        QS_OBJ_(me);                                  /* this active object */
        QS_U8_(e->dynamic_);          /* the dynamic attribute of the event */
        QS_EQC_(0);                     /* number of free entries (unknown) */
    QS_END_()

    return e;
}
