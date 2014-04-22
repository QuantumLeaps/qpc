/**
* \file
* \brief QF_newX_() and QEvt_ctor() definitions.
* \ingroup qf
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-04-09
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
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qf_new")

/****************************************************************************/
/**
* \description
* Allocates an event dynamically from one of the QF event pools.
*
* \arguments
* \arg[in] \c evtSize the size (in bytes) of the event to allocate
* \arg[in] \c margin  the number of un-allocated events still available
*             in a given event pool after the allocation completes
* \arg[in] \c sig the signal to be assigned to the allocated event
*
* \returns pointer to the newly allocated event. This pointer can be NULL
* only if margin!=0 and the event cannot be allocated with the specified
* margin still available in the given pool.
*
* \note The internal QF function QF_newX_() raises an assertion when
* the margin argument is 0 and allocation of the event turns out to be
* impossible due to event pool depletion, or incorrect (too big) size
* of the requested event.
*
* \note The application code should not call this function directly.
* The only allowed use is thorough the macros Q_NEW() or Q_NEW_X().
*/
QEvt *QF_newX_(uint_fast16_t const evtSize,
               uint_fast16_t const margin, enum_t const sig)
{
    QEvt *e;
    uint_fast8_t idx;
    QS_CRIT_STAT_

    /* find the pool index that fits the requested event size ... */
    for (idx = (uint_fast8_t)0; idx < QF_maxPool_; ++idx) {
        if (evtSize <= QF_EPOOL_EVENT_SIZE_(QF_pool_[idx])) {
            break;
        }
    }
    /* cannot run out of registered pools */
    Q_ASSERT_ID(110, idx < QF_maxPool_);

    QS_BEGIN_(QS_QF_NEW, (void *)0, (void *)0)
        QS_TIME_();             /* timestamp */
        QS_EVS_(evtSize);       /* the size of the event */
        QS_SIG_((QSignal)sig);  /* the signal of the event */
    QS_END_()

    QF_EPOOL_GET_(QF_pool_[idx], e, margin); /* get e -- platform-dependent */

    /* was e allocated correctly? */
    if (e != (QEvt *)0) {
        e->sig = (QSignal)sig;      /* set signal for this event */
        e->poolId_ = (uint8_t)(idx + (uint_fast8_t)1); /* store the pool ID */
        e->refCtr_ = (uint8_t)0;    /* set the reference counter to 0 */
    }
    /* event cannot be allocated */
    else {
        /* must tolerate bad alloc. */
        Q_ASSERT_ID(120, margin != (uint_fast16_t)0);
    }
    return e; /* can't be NULL if we can't tolerate bad allocation */
}

#ifdef Q_EVT_CTOR  /* Provide the constructor for the ::QEvt class? */

/****************************************************************************/
/**
* \description
* Constructor for the ::QEvt class provided when the switch #Q_EVT_CTOR
* is defined.
*
* \arguments
* \arg[in,out] \c me   pointer (see \ref derivation)
* \arg[in]     \c sig  signal to be assigned to the event
*/
QEvt *QEvt_ctor(QEvt * const me, enum_t const sig) {
    /** \pre the me pointer must be valid */
    Q_REQUIRE_ID(200, me != (QEvt *)0);
    me->sig = (QSignal)sig;
    return me;
}

#endif /* Q_EVT_CTOR */


