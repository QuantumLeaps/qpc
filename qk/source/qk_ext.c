/**
* \file
* \brief "Extended" QK scheduler QK_schedExt_() definition.
* \ingroup qk
* \cond
******************************************************************************
* Product: QK/C
* Last updated for version 5.3.0
* Last updated on  2014-02-24
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
******************************************************************************
* \endcond
*/
#include "qf_port.h"
#include "qk_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

/****************************************************************************/
/**
* \description
* The "extended" QK scheduler performs all the steps of the regular scheduler
* QK_sched_() and additionally switches the Thread-Local Storage (TLS) and
* handles the extended context-switch.
*
* \arguments
* \arg[in] \c p  priority of the next AO to schedule
*
* \note The "extended" QK scheduler needs to be called only to handle
* "asynchronous" preemption, under the assumption that neither the ISRs
* nor the QK idle loop use TLS or the co-processors requiring
* extended context switch (see [PSiCC2] Section 10.4.3).
*
* \note QK_schedExt_() must be always called with interrupts DISABLED.
*
* \note The extended scheduler might enable interrupts internally,
* but always returns with interrupts DISABLED.
*/
void QK_schedExt_(uint_fast8_t p) {
    uint_fast8_t pin = QK_currPrio_; /* save the initial priority */

    /* thread-local storage used? */
#ifdef QK_TLS
    uint_fast8_t pprev = pin;
#endif
    QActive *a;

    /* extended context-switch used? */
#ifdef QK_EXT_SAVE
    /* aren't we preempting the idle loop? (idle loop has prio==0)  */
    if (pin != (uint_fast8_t)0) {
        a = QF_active_[pin]; /* the pointer to the preempted AO */
        QK_EXT_SAVE(a);      /* save the extended context */
    }
#endif

    /* loop until have ready-to-run AOs of higher priority than the initial */
    do {
        QEvt const *e;
        a = QF_active_[p]; /* obtain the pointer to the AO */

        QK_currPrio_ = p;  /* this becomes the current task priority */

    /* thread-local storage used? */
#ifdef QK_TLS
        /* are we changing threads? */
        if (p != pprev) {
            QK_TLS(a); /* switch new thread-local storage */
            pprev = p;
        }
#endif
        QS_BEGIN_NOCRIT_(QS_QK_SCHEDULE, QS_priv_.aoObjFilter, a)
            QS_TIME_();           /* timestamp */
            QS_U8_((uint8_t)p);   /* the priority of the AO */
            QS_U8_((uint8_t)pin); /* the preempted priority */
        QS_END_NOCRIT_()

        QF_INT_ENABLE(); /* unconditionally enable interrupts */

        /* perform the run-to-completion (RTS) step...
        * 1. retrieve the event from the AO's event queue, which by this
        *    time must be non-empty and QActive_get_() asserts it.
        * 2. dispatch the event to the AO's state machine.
        * 3. determine if event is garbage and collect it if so
        */
        e = QActive_get_(a);
        QMSM_DISPATCH(&a->super, e);
        QF_gc(e);

        QF_INT_DISABLE(); /* disable interrupts */

        /* find new highest-priority AO ready to run... */
#if (QF_MAX_ACTIVE <= 8)
        QPSet8_findMax(&QK_readySet_, p);
#else
        QPSet64_findMax(&QK_readySet_, p);
#endif
        /* is the new priority below the current preemption threshold? */
        if (p <= pin) {
            p = (uint_fast8_t)0;
        }
#ifndef QK_NO_MUTEX
        /* is the new priority below the mutex ceiling? */
        else if (p <= QK_ceilingPrio_) {
            p = (uint_fast8_t)0;
        }
        else {
            /* empty */
        }
#endif
    } while (p != (uint_fast8_t)0);

    QK_currPrio_ = pin; /* restore the initial priority */

#if defined(QK_TLS) || defined(QK_EXT_RESTORE)
    /* aren't we preempting the idle loop? (idle loop has prio==0) */
    if (pin != (uint_fast8_t)0) {
        a = QF_active_[pin]; /* the pointer to the preempted AO */

        /* thread-local storage used? */
#ifdef QK_TLS
        QK_TLS(a); /* restore the original TLS */
#endif

        /* extended context-switch used? */
#ifdef QK_EXT_RESTORE
        QK_EXT_RESTORE(a); /* restore the extended context */
#endif
    }
#endif
}
