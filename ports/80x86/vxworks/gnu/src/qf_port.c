/*****************************************************************************
* Product:  QF/C port to VxWorks
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

#include "taskLib.h"                                     /* for taskSpawn() */
#include "msgQLib.h" /* msgQCreate()/msgQDelete(), msgQSend()/msgQReceive() */
#include "tickLib.h"                                  /* for tickAnnounce() */

Q_DEFINE_THIS_MODULE("qf_port")

/* local objects -----------------------------------------------------------*/

/*..........................................................................*/
void usrClock(void) {              /* called from the system clock tick ISR */
    tickAnnounce();                  /* perform all VxWorks tme bookkeeping */
    QF_tick();                               /* perform QF time bookkeeping */
}

/*..........................................................................*/
const char Q_ROM * Q_ROM_VAR QF_getPortVersion(void) {
    static char const Q_ROM Q_ROM_VAR version[] = "4.4.02";
    return version;
}
/*..........................................................................*/
void QF_init(void) {
}
/*..........................................................................*/
int16_t QF_run(void) {
    QF_onStartup();                                     /* startup callback */

    return (int16_t)0;                                    /* return success */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup();                                     /* cleanup callback */
}
/*..........................................................................*/
                                  /* use exactly the VxWorks task signature */
static void task_function(int me,     int dummy2, int dummy3, int dummy4,
                          int dummy5, int dummy6, int dummy7, int dummy8,
                          int dummy9, int dummy10)
{
    (void)dummy2;         /* avoid compiler warnings about unused arguments */
    (void)dummy3;
    (void)dummy4;
    (void)dummy5;
    (void)dummy6;
    (void)dummy7;
    (void)dummy8;
    (void)dummy9;
    (void)dummy10;

    ((QActive *)me)->running = (uint8_t)1;        /* enable the thread-loop */
    while (((QActive *)me)->running) {
        QEvt const *e = QActive_get_((QActive *)me);
        QF_ACTIVE_DISPATCH_(&((QActive *)me)->super, e);
        QF_gc(e);    /* check if the event is garbage, and collect it if so */
    }

    QF_remove_((QActive *)me);     /* remove this object from the framework */
}
/*..........................................................................*/
void QActive_start(QActive *me, uint8_t prio,
                    QEvt const *qSto[], uint32_t qLen,
                    void *stkSto, uint32_t stkSize,
                    QEvt const *ie)
{
    Q_REQUIRE((qSto == (QEvt const **)0)   /* VxWorks allocates Queue mem */
              && (stkSto == (void *)0));     /* VxWorks allocates Stack mem */

    me->eQueue = msgQCreate(qLen, sizeof(QEvt *), MSG_Q_FIFO);
    Q_ASSERT(me->eQueue != 0);                     /* VxWorks queue created */
    me->prio = prio;                                /* save the QF priority */
    QF_add_(me);                     /* make QF aware of this active object */
    QF_ACTIVE_INIT_(&me->super, ie);          /* execute initial transition */
    me->thread = taskSpawn((char *)0,
                         QF_MAX_ACTIVE - me->prio,      /* VxWorks priority */
                         VX_FP_TASK,     /* taks options -- need to adjust! */
                         (int)stkSize,
                         &task_function,
                         (int)me, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    Q_ASSERT(me->thread != ERROR);                  /* VxWorks task created */
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
    Q_ALLEGE(msgQSend(me->eQueue, (char *)&e, sizeof(e),
                      NO_WAIT, MSG_PRI_NORMAL) == OK);
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
    Q_ALLEGE(msgQSend(me->eQueue, (char *)&e, sizeof(e),
                      NO_WAIT, MSG_PRI_URGENT) == OK);
}
/*..........................................................................*/
QEvt const *QActive_get_(QActive *me) {
    QEvt const *e;
    QS_CRIT_STAT_

    Q_ALLEGE(msgQReceive(me->eQueue, (char *)&e, sizeof(e), WAIT_FOREVER)
             == sizeof(e));

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
void QActive_stop(QActive *me) {
    me->running = (uint8_t)0;        /* stop the loop inside QActive::run() */
    msgQDelete(me->eQueue);                     /* delete the VxWorks queue */
}
