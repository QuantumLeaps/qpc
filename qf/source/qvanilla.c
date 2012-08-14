/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 4.5.00
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
#include "qf_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qvanilla")

/**
* \file
* \ingroup qf
* \brief "vanilla" cooperative kernel,
* QActive_start(), QActive_stop(), and QF_run() implementation.
*/

/* Package-scope objects ---------------------------------------------------*/
#if (QF_MAX_ACTIVE <= 8)
    QPSet8  QF_readySet_;                 /* QF-ready set of active objects */
#else
    QPSet64 QF_readySet_;                 /* QF-ready set of active objects */
#endif

/*..........................................................................*/
void QF_init(void) {
    /* nothing to do for the "vanilla" kernel */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup();                                     /* cleanup callback */
    /* nothing else to do for the "vanilla" kernel */
}
/*..........................................................................*/
int16_t QF_run(void) {

    QF_onStartup();                                     /* startup callback */

    for (;;) {                                       /* the background loop */
        QEvt const *e;
        QActive *a;
        uint8_t p;

        QF_INT_DISABLE();

#if (QF_MAX_ACTIVE <= 8)
        if (QPSet8_notEmpty(&QF_readySet_)) {
            QPSet8_findMax(&QF_readySet_, p);
#else
        if (QPSet64_notEmpty(&QF_readySet_)) {
            QPSet64_findMax(&QF_readySet_, p);
#endif
            a = QF_active_[p];
            QF_INT_ENABLE();

            e = QActive_get_(a);          /* get the next event for this AO */
            QF_ACTIVE_DISPATCH_(&a->super, e);        /* dispatch to the AO */
            QF_gc(e); /* determine if event is garbage and collect it if so */
        }
        else {
            QF_onIdle();                                      /* see NOTE01 */
        }
    }

    return (int16_t)0; /* this unreachable return is to make compiler happy */
}
/*..........................................................................*/
void QActive_start(QActive * const me, uint8_t prio,
                   QEvt const *qSto[], uint32_t qLen,
                   void *stkSto, uint32_t stkSize,
                   QEvt const *ie)
{
    Q_REQUIRE(((uint8_t)0 < prio) && (prio <= (uint8_t)QF_MAX_ACTIVE)
              && (stkSto == (void *)0));   /* does not need per-actor stack */

    (void)stkSize;         /* avoid the "unused parameter" compiler warning */
    QEQueue_init(&me->eQueue, qSto, (QEQueueCtr)qLen);/* initialize QEQueue */
    me->prio = prio;           /* set the QF priority of this active object */
    QF_add_(me);                     /* make QF aware of this active object */
    QF_ACTIVE_INIT_(&me->super, ie);          /* execute initial transition */

    QS_FLUSH();                       /* flush the trace buffer to the host */
}
/*..........................................................................*/
void QActive_stop(QActive * const me) {
    QF_remove_(me);
}

/*****************************************************************************
* NOTE01:
* QF_onIdle() must be called with interrupts DISABLED because the
* determination of the idle condition (no events in the queues) can change
* at any time by an interrupt posting events to a queue. The QF_onIdle()
* MUST enable interrups internally, perhaps at the same time as putting the
* CPU into a power-saving mode.
*/
