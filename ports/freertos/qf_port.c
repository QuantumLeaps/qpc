/*****************************************************************************
* Product: QF/C port to FreeRTOS.org
* Last updated for version 5.3.1
* Last updated on  2014-09-24
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#include "qassert.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qf_port")

/* global varibles used by this FreeRTOS port */
FreeRTOSExtras FreeRTOS_extras;

/*..........................................................................*/
void QF_init(void) {
    FreeRTOS_extras.isrNest = (BaseType_t)0;
}
/*..........................................................................*/
int_t QF_run(void) {
    QF_onStartup();  /* the startup callback (configure/enable interrupts) */
    vTaskStartScheduler(); /* start the FreeRTOS.org scheduler */
    Q_ERROR();       /* the FreeRTOS.org scheduler should never return */
    return (int_t)0; /* dummy return to make the compiler happy */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup(); /* cleanup callback */
}
/*..........................................................................*/
static void task_function(void *pvParameters) { /* FreeRTOS signature */
    QActive *act = (QActive *)pvParameters;

    while (act->thread != (TaskHandle_t)0) {
        QEvt const *e = QActive_get_(act);
        QMSM_DISPATCH(&act->super, e);
        QF_gc(e); /* check if the event is garbage, and collect it if so */
    }

    QF_remove_(act); /* remove this object from QF */
    vTaskDelete((TaskHandle_t)0); /* delete this FreeRTOS task */
}
/*..........................................................................*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    portBASE_TYPE err;

    Q_REQUIRE((qSto != (QEvt const **)0) /* queue storage must be provided */
        && (qLen > (uint_fast16_t)0)     /* queue size must be provided */
        && (stkSto == (void *)0)         /* no stack storage */
        && (stkSize > (uint_fast16_t)0));/* stack size must be provided */

    /* create the event queue for the AO */
    QEQueue_init(&me->eQueue, qSto, qLen);

    me->prio = prio;  /* save the QF priority */
    QF_add_(me);      /* make QF aware of this active object */
    QMSM_INIT(&me->super, ie); /* execute initial transition */

    /* create the FreeRTOS.org task for the AO */
    err = xTaskCreate(&task_function,   /* the task function */
              (const char *)"AO",       /* the name of the task */
              (uint16_t)stkSize/sizeof(portSTACK_TYPE), /* stack size */
              (void *)me,               /* the 'pvParameters' parameter */
              (UBaseType_t)(prio + tskIDLE_PRIORITY),  /* FreeRTOS priority */
              &me->thread);             /* task handle */
    Q_ENSURE(err == pdPASS);   /* FreeRTOS task must be created */
    me->osObject = me->thread; /* OS-Object for FreeRTOS is the task handle */
}
/*..........................................................................*/
void QActive_stop(QActive * const me) {
    me->thread = (TaskHandle_t)0; /* stop the thread loop */
}
