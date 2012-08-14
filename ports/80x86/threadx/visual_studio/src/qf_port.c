/*****************************************************************************
* Product:  QF/C, port to ThreadX
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

/* Local objects -----------------------------------------------------------*/

/*..........................................................................*/
char const Q_ROM * Q_ROM_VAR QF_getPortVersion(void) {
    static const char Q_ROM Q_ROM_VAR version[] =  "4.4.00";
    return version;
}
/*..........................................................................*/
void QF_init(void) {
}
/*..........................................................................*/
int16_t QF_run(void) {
    static TX_TIMER qf_tick_timer;
    Q_ALLEGE(tx_timer_create(&qf_tick_timer,        /* ThreadX timer object */
                 "QF",                                 /* name of the timer */
                 (VOID (*)(ULONG))&QF_tick,          /* expiration function */
                 0,                            /* expiration function input */
                 1,                                        /* initial ticks */
                 1,                                     /* reschedule ticks */
                 TX_AUTO_ACTIVATE)          /* automatically activate timer */
             == TX_SUCCESS);

    return (int16_t)0;                                    /* return success */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup();                                     /* cleanup callback */
}
/*..........................................................................*/
static void thread_function(ULONG thread_input) {      /* ThreadX signature */
    QActive *act = (QActive *)thread_input;
    act->running = (uint8_t)1;  /* enable thread-loop */
    while (act->running) {
        QEvt const *e = QActive_get_(act);
        QF_ACTIVE_DISPATCH_(&act->super, e);
        QF_gc(e);    /* check if the event is garbage, and collect it if so */
    }

    QF_remove_(act);      /* remove this object from QF */
    Q_ALLEGE(tx_queue_delete(&act->eQueue) == TX_SUCCESS); /* cleanup queue */
    Q_ALLEGE(tx_thread_delete(&act->thread) == TX_SUCCESS);/*cleanup thread */
}
/*..........................................................................*/
void QActive_start(QActive *me, uint8_t prio,
                   QEvt const *qSto[], uint32_t qLen,
                   void *stkSto, uint32_t stkSize,
                   QEvt const *ie)
{
    UINT tx_prio; /* ThreadX priority corresponding to the QF priority prio */

                   /* allege that the ThreadX queue is created successfully */
    Q_ALLEGE(tx_queue_create(&me->eQueue,
                 "Q",
                 TX_1_ULONG,
                 (VOID *)qSto,
                 (ULONG)(qLen * sizeof(ULONG)))
             == TX_SUCCESS);

    me->prio = prio;                                /* save the QF priority */
    QF_add_(me);                     /* make QF aware of this active object */
    QF_ACTIVE_INIT_(&me->super, ie);          /* execute initial transition */

    QS_FLUSH();                       /* flush the trace buffer to the host */

                             /* convert QF priority to the ThreadX priority */
    tx_prio = QF_TX_PRIO_OFFSET + QF_MAX_ACTIVE - prio;

    Q_ALLEGE(tx_thread_create(&me->thread,  /* ThreadX thread control block */
                 "AO",                                       /* thread name */
                 &thread_function,                       /* thread function */
                 (ULONG)me,                                 /* thread input */
                 stkSto,                                     /* stack start */
                 stkSize,                            /* stack size in bytes */
                 tx_prio,                               /* ThreadX priority */
                 tx_prio,/*preemption threshold disabled (same as priority) */
                 TX_NO_TIME_SLICE,
                 TX_AUTO_START)
             == TX_SUCCESS);
}
/*..........................................................................*/
void QActive_stop(QActive *me) {
    me->running = (uint8_t)0;                       /* stop the thread loop */
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
    Q_ALLEGE(tx_queue_send(&me->eQueue, (VOID *)&e, TX_NO_WAIT)
             == TX_SUCCESS);
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
    Q_ALLEGE(tx_queue_front_send(&me->eQueue, (VOID *)&e, TX_NO_WAIT)
             == TX_SUCCESS);
}
/*..........................................................................*/
QEvt const *QActive_get_(QActive *me) {
    QEvt const *e;
    QS_CRIT_STAT_

    Q_ALLEGE(tx_queue_receive(&me->eQueue, (VOID *)&e, TX_WAIT_FOREVER)
             == TX_SUCCESS);

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
