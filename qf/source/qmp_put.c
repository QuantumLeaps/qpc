/**
* \file
* \ingroup qf
* \brief QMPool_put() implementation.
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-02-17
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

Q_DEFINE_THIS_MODULE("qmp_put")

/****************************************************************************/
/**
* \description
* Recycle a memory block to the fixed block-size memory pool.
*
* \arguments
* \arg[in,out] \c me   pointer (see \ref derivation)
* \arg[in]     \c b    pointer to the memory block that is being recycled
*
* \attention
* The recycled block must be allocated from the __same__ memory pool
* to which it is returned.
*
* \note This function can be called from any task level or ISR level.
*
* \sa QMPool_get()
*/
void QMPool_put(QMPool * const me, void *b) {
    QF_CRIT_STAT_

    /** \pre # free blocks cannot exceed the total # blocks and
    * the block pointer must be from this pool.
    */
    Q_REQUIRE_ID(100, (me->nFree < me->nTot)
                      && QF_PTR_RANGE_(b, me->start, me->end));

    QF_CRIT_ENTRY_();
    ((QFreeBlock *)b)->next = (QFreeBlock *)me->free_head;/* link into list */
    me->free_head = b;      /* set as new head of the free list */
    ++me->nFree;            /* one more free block in this pool */

    QS_BEGIN_NOCRIT_(QS_QF_MPOOL_PUT, QS_priv_.mpObjFilter, me->start)
        QS_TIME_();         /* timestamp */
        QS_OBJ_(me->start); /* the memory managed by this pool */
        QS_MPC_(me->nFree); /* the number of free blocks in the pool */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
}
