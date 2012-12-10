/*****************************************************************************
* Product:  QF/C, port to Android NDK, gcc
* Last Updated for Version: 4.5.03
* Date of the Last Update:  Nov 04, 2012
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
pthread_mutex_t QF_pThreadMutex_;


/*..........................................................................*/
const char *QF_getPortVersion(void) {
    static const char Q_ROM version[] = "4.5.02";
    return version;
}

/*..........................................................................*/
static void *thread_routine(void *arg) {    /* the expected POSIX signature */
    QActive *act = (QActive*)arg;
    do {
        QEvt const *e = QActive_get_((QActive *)arg); /* wait for the event */
        QF_ACTIVE_DISPATCH_(&act->super, e);             /* dispatch to  SM */
        QF_gc(e);    /* check if the event is garbage, and collect it if so */
    } while (act->thread.running != 0U);

    QF_remove_(act);           /* remove this object from any subscriptions */
    pthread_cond_destroy(&act->osObject); /* cleanup the condition variable */
    return (void *)0;                                     /* return success */
}
/*..........................................................................*/
void QActive_start(QActive *me, uint8_t prio,
                  QEvt const *qSto[], uint32_t qLen,
                  void *stkSto, uint32_t stkSize,
                  QEvt const *ie)
{
    pthread_attr_t attr;
    struct sched_param param;

    Q_REQUIRE(stkSto == (void *)0);  /* p-threads allocate stack internally */

    QEQueue_init(&me->eQueue, qSto, (QEQueueCtr)qLen);
    Q_ALLEGE(pthread_cond_init(&me->osObject, 0) == 0);

    me->prio = prio;
    QF_add_(me);                     /* make QF aware of this active object */
    QF_ACTIVE_INIT_(&me->super, ie);      /* execute the initial transition */

    QS_FLUSH();                       /* flush the trace buffer to the host */

    Q_ALLEGE(pthread_attr_init(&attr) == 0);
    Q_ALLEGE(pthread_attr_setschedpolicy(&attr, SCHED_FIFO) == 0);

    param.sched_priority = prio
                           + (sched_get_priority_max(SCHED_FIFO)
                              - QF_MAX_ACTIVE - 3);

    Q_ALLEGE(pthread_attr_setschedparam(&attr, &param) == 0);
    Q_ALLEGE(pthread_attr_setdetachstate(&attr,
                                         PTHREAD_CREATE_JOINABLE) == 0);

    me->thread.running = 1U;
    Q_ALLEGE(pthread_create(&me->thread.thread, &attr,
                            &thread_routine, me) == 0);
    Q_ALLEGE(pthread_attr_destroy(&attr) == 0);
}
/*..........................................................................*/
void QActive_stop(QActive *me) {
    me->thread.running = 0U;
}
