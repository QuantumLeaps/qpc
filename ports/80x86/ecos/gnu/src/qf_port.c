/*****************************************************************************
* Product:  QF/C port to 80x86, eCos, gnu
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


/* local objects -----------------------------------------------------------*/
static cyg_handle_t l_tick_alarmH;

/*..........................................................................*/
char const Q_ROM * Q_ROM_VAR QF_getPortVersion(void) {
    static const char Q_ROM Q_ROM_VAR version[] =  "4.4.02";
    return version;
}
/*..........................................................................*/
void QF_init(void) {
}
/*..........................................................................*/
void QF_stop(void) {
    cyg_alarm_disable(l_tick_alarmH);
    cyg_alarm_delete (l_tick_alarmH);

    QF_onCleanup();                                     /* cleanup callback */
}
/*..........................................................................*/
int16_t QF_run(void) {
    static cyg_alarm alarm;             /* memory for the eCos alarm object */
    cyg_handle_t counterH;
    cyg_handle_t clockH = cyg_real_time_clock();

    cyg_clock_to_counter(clockH, &counterH);
    cyg_alarm_create(counterH,
                     ((*)(cyg_handle_t, cyg_addrword_t))&QF_tick,
                     0,
                     &l_tick_alarmH,
                     &alarm);

    QF_onStartup();                       /* configure and start interrupts */

    /**
     * kick off the QF tick ...
     */
    cyg_alarm_initialize(l_tick_alarmH, cyg_current_time() + 1, 1);

    return (int16_t)0;                                    /* return success */
}
/*..........................................................................*/
static void thread_function(cyg_addrword_t data) {
    QActive *me = (QActive *)data;

    me->running = (uint8_t)1;               /* allow the thread-loop to run */

    do {
        /* get the next event for this active object */
        QEvt const *e = (QEvt *)cyg_mbox_get(me->eQueue.handle);

        QF_ACTIVE_DISPATCH_(me, e);   /* dispatch to the active object's SM */

        QF_gc(e);    /* check if the event is garbage, and collect it if so */
    } while (me->running);

    QActive_unsubscribeAll_(me);            /* unsubscribe from all signals */
    QF_remove_(me);            /* remove this object from any subscriptions */
}
/*..........................................................................*/
void QActive_start(QActive *me, uint8_t prio,
                   QEvt const *qSto[], uint32_t qLen,
                   void *stkSto, uint32_t stkSize,
                   QEvt const *ie)
{
    /**
     * create and assign the mail box to the QActive object...
     */
    cyg_mbox_create(&me->eQueue.handle, &me->eQueue.mbox);
    Q_ASSERT(me->eQueue.handle != 0);

    me->prio = prio;
    QF_add_(me);                     /* make QF aware of this active object */
    QF_ACTIVE_INIT_(&me->super, ie);          /* execute initial transition */

    QS_FLUSH();                       /* flush the trace buffer to the host */

    /**
     * create the actual eCos thread, note this ASSIGNS me->thread
     * with the newly created thread's handle ...
     */
    cyg_thread_create(QF_MAX_ACTIVE - prio,
                     &thread_function,
                     (cyg_addrword_t)me,
                     "AO",                          /* active object thread */
                     stkSto, stkSize,
                     &me->thread.hanlde, &me->thread.thread);

    /**
     * crank it up - this will invoke thread_function()
     */
    cyg_thread_resume(me->thread.handle);
}
/*..........................................................................*/
#ifndef Q_SPY
void QActive_postFIFO(QActive *me, QEvt const *e) {
#else
void QActive_postFIFO(QActive *me, QEvt const *e, void const *sender) {
#endif

    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_FIFO, QS_aoObj_, me)
        QS_TIME_();                                            /* timestamp */
        QS_OBJ_(sender);                               /* the sender object */
        QS_SIG_(e->sig);                         /* the signal of the event */
        QS_OBJ_(me);                      /* this active object (recipient) */
        QS_U8_(QF_EVT_POOL_ID_(e));             /* the pool Id of the event */
        QS_U8_(QF_EVT_REF_CTR_(e));           /* the ref count of the event */
        QS_EQC_(0);                     /* number of free entries (unknown) */
        QS_EQC_(0);                 /* min number of free entries (unknown) */
    QS_END_NOCRIT_()

    if (QF_EVT_POOL_ID_(e) != (uint8_t)0) {          /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e);          /* increment the reference counter */
    }
    QF_CRIT_EXIT_();
    Q_ALLEGE(cyg_mbox_tryput(me->eQueue.handle, e));
}
/*..........................................................................*/
void QActive_postLIFO(QActive *me, QEvt const *e) {
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QF_ACTIVE_POST_LIFO, QS_aoObj_, me)
        QS_TIME_();                                            /* timestamp */
        QS_SIG_(e->sig);                        /* the signal of this event */
        QS_OBJ_(me);                                  /* this active object */
        QS_U8_(QF_EVT_POOL_ID_(e));             /* the pool Id of the event */
        QS_U8_(QF_EVT_REF_CTR_(e));           /* the ref count of the event */
        QS_EQC_(0);                     /* number of free entries (unknown) */
        QS_EQC_(0);                 /* min number of free entries (unknown) */
    QS_END_NOCRIT_()

    if (QF_EVT_POOL_ID_(e) != (uint8_t)0) {          /* is it a pool event? */
        QF_EVT_REF_CTR_INC_(e);          /* increment the reference counter */
    }
    QF_CRIT_EXIT_();
    Q_ALLEGE(cyg_mbox_tryput(me->eQueue.handle, e));
}
/*..........................................................................*/
QEvt const * QActive_get_(QActive *me) {
    QEvt const * e  = (QEvt const *) cyg_mbox_get(me->eQueue.handle);
    QS_CRIT_STAT_

    Q_ASSERT(e != (QEvt *)0);

    QS_BEGIN_(QS_QF_ACTIVE_GET, QS_aoObj_, me)
        QS_TIME_();                                            /* timestamp */
        QS_SIG_(e->sig);                        /* the signal of this event */
        QS_OBJ_(me);                                  /* this active object */
        QS_U8_(QF_EVT_POOL_ID_(e));             /* the pool Id of the event */
        QS_U8_(QF_EVT_REF_CTR_(e));           /* the ref count of the event */
        QS_EQC_(0);                     /* number of free entries (unknown) */
    QS_END_()

    return e;
}
/*..........................................................................*/
void QActive_stop_(QActive *me) {
    ((QActive *)me)->running = (uint8_t)0;          /* stop the thread loop */
}

