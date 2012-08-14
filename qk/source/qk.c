/*****************************************************************************
* Product: QK/C
* Last Updated for Version: 4.4.02
* Date of the Last Update:  May 17, 2012
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
#include "qk_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qk")

/**
* \file
* \ingroup qk
* \brief QK_readySet_, QK_currPrio_, and QK_intNest_ definitions and
* QK_getVersion(), QF_init(), QF_run(), QF_stop(),
* QActive_start(), QActive_stop() implementations.
*/

/* Public-scope objects ----------------------------------------------------*/
#if (QF_MAX_ACTIVE <= 8)
    QPSet8  QK_readySet_;                                   /* QK ready-set */
#else
    QPSet64 QK_readySet_;                                   /* QK ready-set */
#endif
                                      /* start with the QK scheduler locked */
uint8_t QK_currPrio_ = (uint8_t)(QF_MAX_ACTIVE + 1);
uint8_t QK_intNest_;                       /* start with nesting level of 0 */

/*..........................................................................*/
char_t const Q_ROM * Q_ROM_VAR QK_getVersion(void) {
    return QF_getVersion();
}
/*..........................................................................*/
void QF_init(void) {
    QK_init();                              /* might be defined in assembly */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup();                                     /* cleanup callback */
    /* nothing else to do for the QK preemptive kernel */
}
/*..........................................................................*/
static void initialize(void) {
    uint8_t p;

    QK_currPrio_ = (uint8_t)0;     /* set the priority for the QK idle loop */
    p = QK_schedPrio_();
    if (p != (uint8_t)0) {
        QK_sched_(p);                 /* process all events produced so far */
    }
}
/*..........................................................................*/
int16_t QF_run(void) {
    QF_INT_DISABLE();
    initialize();
    QF_onStartup();                                     /* startup callback */
    QF_INT_ENABLE();

    for (;;) {                                          /* the QK idle loop */
        QK_onIdle();                      /* invoke the QK on-idle callback */
    }

    return (int16_t)0; /* this unreachable return is to make compiler happy */
}
/*..........................................................................*/
void QActive_start(QActive *me, uint8_t prio,
                   QEvt const *qSto[], uint32_t qLen,
                   void *stkSto, uint32_t stkSize,
                   QEvt const *ie)
{
    Q_REQUIRE(((uint8_t)0 < prio) && (prio <= (uint8_t)QF_MAX_ACTIVE));

    QEQueue_init(&me->eQueue, qSto, (QEQueueCtr)qLen);
    me->prio = prio;
    QF_add_(me);                     /* make QF aware of this active object */

#if defined(QK_TLS) || defined(QK_EXT_SAVE)
    me->osObject = (uint8_t)stkSize;  /* osObject contains the thread flags */
    me->thread = stkSto;/* contains the pointer to the thread-local storage */
#else
    Q_ASSERT((stkSto == (void *)0) && (stkSize == (uint32_t)0));
#endif

    QF_ACTIVE_INIT_(&me->super, ie);          /* execute initial transition */

    QS_FLUSH();                       /* flush the trace buffer to the host */
}
/*..........................................................................*/
void QActive_stop(QActive *me) {
    QF_remove_(me);                /* remove this active object from the QF */
}
