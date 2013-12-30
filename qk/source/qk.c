/*****************************************************************************
* Product: QK/C
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Dec 24, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
* \brief #QK_readySet_, #QK_currPrio_, and #QK_intNest_ definitions and
* QF_init(), QF_run(), QF_stop(), QActive_start(), QActive_stop()
* implementations.
*/

/* Public-scope objects ----------------------------------------------------*/
#if (QF_MAX_ACTIVE <= 8)
    QPSet8  QK_readySet_;                                   /* QK ready-set */
#else
    QPSet64 QK_readySet_;                                   /* QK ready-set */
#endif

uint8_t volatile QK_currPrio_;
uint_t  volatile QK_intNest_;

/*..........................................................................*/
static void initialize(void);

/*..........................................................................*/
void QF_init(void) {                                          /* see NOTE01 */
    extern uint_t QF_maxPool_;
    extern QTimeEvt QF_timeEvtHead_[QF_MAX_TICK_RATE];

    QK_intNest_  = (uint_t)0;                           /* no nesting level */
    QK_currPrio_ = (uint8_t)(QF_MAX_ACTIVE + 1);        /* scheduler locked */
#ifndef QK_NO_MUTEX
    QK_ceilingPrio_ = (uint8_t)0;
#endif
    QF_maxPool_  = (uint_t)0;
    QF_bzero(&QK_readySet_,       (uint_t)sizeof(QK_readySet_));
    QF_bzero(&QF_timeEvtHead_[0], (uint_t)sizeof(QF_timeEvtHead_));
    QF_bzero(&QF_active_[0],      (uint_t)sizeof(QF_active_));

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
int_t QF_run(void) {
    QF_INT_DISABLE();
    initialize();
    QF_onStartup();                                     /* startup callback */
    QF_INT_ENABLE();

    for ( ; ; ) {                                       /* the QK idle loop */
        QK_onIdle();                      /* invoke the QK on-idle callback */
    }

#ifdef __GNUC__                                            /* GNU compiler? */
    return (int_t)0;
#endif
}
/*..........................................................................*/
void QActive_start_(QActive *me, uint_t prio,
                    QEvt const *qSto[], uint_t qLen,
                    void *stkSto, uint_t stkSize,
                    QEvt const *ie)
{
    Q_REQUIRE(((uint_t)0 < prio) && (prio <= (uint_t)QF_MAX_ACTIVE));

    QEQueue_init(&me->eQueue, qSto, qLen); /* initialize the built-in queue */
    me->prio = (uint8_t)prio;
    QF_add_(me);                     /* make QF aware of this active object */

#if defined(QK_TLS) || defined(QK_EXT_SAVE)
    me->osObject = (uint8_t)stkSize;  /* osObject contains the thread flags */
    me->thread = stkSto;/* contains the pointer to the thread-local storage */
#else
    Q_ASSERT((stkSto == (void *)0) && (stkSize == (uint_t)0));
#endif

    QMSM_INIT(&me->super, ie);                /* execute initial transition */

    QS_FLUSH();                       /* flush the trace buffer to the host */
}
/*..........................................................................*/
void QActive_stop(QActive *me) {
    QF_remove_(me);                /* remove this active object from the QF */
}

/*****************************************************************************
* NOTE01:
* The QF_init() function clears the internal QF variables, so that the
* framework can start correctly even if the startup code fails to clear
* the uninitialized data (as is required by the C Standard).
*/
