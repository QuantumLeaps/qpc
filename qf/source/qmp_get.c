/**
* \file
* \ingroup qf
* \brief QMPool_get() and QF_getPoolMin() implementation.
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-02-19
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
#include "qassert.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

Q_DEFINE_THIS_MODULE("qmp_get")

/****************************************************************************/
/**
* \description
* The only parameter \a me is a pointer to the ::QMPool from which the
* block is requested. The function returns a pointer to the allocated
* memory block or NULL if no free blocks are available.
*
* \arguments
* \arg[in,out] \c me     pointer (see \ref derivation)
* \arg[in]     \c nTicks number of clock ticks (at the associated rate)
*                        to rearm the time event with.
*
* A allocated block must be returned to the same pool from which it has
* been allocated.
*
* This function can be called from any task level or ISR level.
*
* \note The memory pool \a me must be initialized before any events can
* be requested from it. Also, the QMPool_get() function uses internally a
* QF critical section, so you should be careful not to call it from within
* a critical section when nesting of critical section is not supported.
*
* \sa QMPool_put()
*/
void *QMPool_get(QMPool * const me, uint_fast16_t const margin) {
    QFreeBlock *fb;
    QF_CRIT_STAT_

    QF_CRIT_ENTRY_();

    /* have more free than margin? */
    if (me->nFree > (QMPoolCtr)margin) {
        fb = (QFreeBlock *)me->free_head; /* get a free block */

        /* free block must be available */
        Q_ASSERT_ID(110, fb != (QFreeBlock *)0);

        me->free_head = fb->next;/* adjust list head to the next free block */
        --me->nFree; /* one less free block */

        if (me->nMin > me->nFree) { /* is it the new minimum? */
            me->nMin = me->nFree;   /* remember the new minimum */
        }

        QS_BEGIN_NOCRIT_(QS_QF_MPOOL_GET, QS_priv_.mpObjFilter, me->start)
            QS_TIME_();         /* timestamp */
            QS_OBJ_(me->start); /* the memory managed by this pool */
            QS_MPC_(me->nFree); /* the number of free blocks in the pool */
            QS_MPC_(me->nMin);  /* min # free blocks ever in the pool */
        QS_END_NOCRIT_()
    }
    /* don't have above the margin at this point */
    else {
        fb = (QFreeBlock *)0;

        QS_BEGIN_NOCRIT_(QS_QF_MPOOL_GET_ATTEMPT,
                         QS_priv_.mpObjFilter, me->start)
            QS_TIME_();            /* timestamp */
            QS_OBJ_(me->start);    /* the memory managed by this pool */
            QS_MPC_(me->nFree);    /* the number of free blocks in the pool */
            QS_MPC_(margin);       /* the requested margin */
        QS_END_NOCRIT_()
    }
    QF_CRIT_EXIT_();

    return fb;  /* return the pointer to memory block or NULL to the caller */
}

/****************************************************************************/
/**
* \description
* This function obtains the minimum number of free blocks in the given
* event pool since this pool has been initialized by a call to QF_poolInit().
*
* \arguments
* \arg[in] \c poolId  event pool ID in the range 1..QF_maxPool_, where
*             QF_maxPool_ is the number of event pools initialized
*             with the function QF_poolInit().
*
* \returns the minimum number of unused blocks in the given event pool.
*/
uint_fast16_t QF_getPoolMin(uint_fast8_t const poolId) {
    uint_fast16_t min;
    QF_CRIT_STAT_

    /** \pre the poolId must be in range */
    Q_REQUIRE_ID(200, ((uint_fast8_t)1 <= poolId)
                      && (poolId <= QF_maxPool_));

    QF_CRIT_ENTRY_();
    min = (uint_fast16_t)QF_pool_[poolId - (uint_fast8_t)1].nMin;
    QF_CRIT_EXIT_();

    return min;
}
