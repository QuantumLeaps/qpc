/*****************************************************************************
* Product: QF/C
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jul 26, 2012
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

Q_DEFINE_THIS_MODULE("qf_pool")

/**
* \file
* \ingroup qf
* \brief QF_pool_[] definition and QF_poolInit() implementation.
*/

/* Package-scope objects ---------------------------------------------------*/
QF_EPOOL_TYPE_ QF_pool_[QF_MAX_EPOOL];          /* allocate the event pools */
uint8_t QF_maxPool_;                   /* number of initialized event pools */

/*..........................................................................*/
void QF_poolInit(void * const poolSto, uint32_t const poolSize,
                 uint32_t const evtSize)
{
                      /* cannot exceed the number of available memory pools */
    Q_REQUIRE(QF_maxPool_ < (uint8_t)Q_DIM(QF_pool_));
            /* please initialize event pools in ascending order of evtSize: */
    Q_REQUIRE((QF_maxPool_ == (uint8_t)0)
             || (QF_EPOOL_EVENT_SIZE_(QF_pool_[QF_maxPool_ - (uint8_t)1])
                 < (QEvtSize)evtSize));
                /* perfom the platform-dependent initialization of the pool */
    QF_EPOOL_INIT_(QF_pool_[QF_maxPool_], poolSto, poolSize, evtSize);
    ++QF_maxPool_;                                         /* one more pool */
}
