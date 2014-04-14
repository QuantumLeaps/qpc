/**
* \file
* \brief ::QF_pool_[] definition and QF_poolInit() implementation.
* \ingroup qf
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
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qf_pool")

/* Package-scope objects ****************************************************/
QF_EPOOL_TYPE_ QF_pool_[QF_MAX_EPOOL]; /* allocate the event pools */
uint_fast8_t QF_maxPool_;              /* number of initialized event pools */

/****************************************************************************/
/**
* \description
* This function initializes one event pool at a time and must be called
* exactly once for each event pool before the pool can be used.
*
* \arguments
* \arg[in] \c poolSto  pointer to the storage for the event pool
* \arg[in] \c poolSize size of the storage for the pool in bytes
* \arg[in] \c evtSize  the block-size of the pool in bytes, which determines
*             the maximum size of events that can be allocated from the pool.
*
* \note
* You might initialize many event pools by making many consecutive calls
* to the QF_poolInit() function. However, for the simplicity of the internal
* implementation, you must initialize event pools in the ascending order of
* the event size.
*
* Many RTOSes provide fixed block-size heaps, a.k.a. memory pools that can
* be adapted for QF event pools. In case such support is missing, QF provides
* a native QF event pool implementation. The macro #QF_EPOOL_TYPE_ determines
* the type of event pool used by a particular QF port. See structure ::QMPool
* for more information.
*
* \note The actual number of events available in the pool might be actually
* less than (\a poolSize / \a evtSize) due to the internal alignment
* of the blocks that the pool might perform. You can always check the
* capacity of the pool by calling QF_getPoolMin().
*
* \note The dynamic allocation of events is optional, meaning that you
* might choose not to use dynamic events. In that case calling QF_poolInit()
* and using up memory for the memory blocks is unnecessary.
*
* \sa QF initialization example for QF_init()
*/
void QF_poolInit(void * const poolSto, uint_fast16_t const poolSize,
                 uint_fast16_t const evtSize)
{
    /** \pre cannot exceed the number of available memory pools */
    Q_REQUIRE_ID(100, QF_maxPool_ < (uint_fast8_t)Q_DIM(QF_pool_));
    /** \pre please initialize event pools in ascending order of evtSize: */
    Q_REQUIRE_ID(101, (QF_maxPool_ == (uint_fast8_t)0)
        || (QF_EPOOL_EVENT_SIZE_(QF_pool_[QF_maxPool_ - (uint_fast8_t)1])
            < evtSize));

    /* perform the platform-dependent initialization of the pool */
    QF_EPOOL_INIT_(QF_pool_[QF_maxPool_],
                   poolSto, poolSize, evtSize);
    ++QF_maxPool_; /* one more pool */
}
