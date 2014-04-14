/**
* \file
* \brief ::QF_subscrList_, ::QF_maxSignal_, and QF_psInit() definitions.
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

/* Package-scope objects ****************************************************/
QSubscrList *QF_subscrList_;
enum_t QF_maxSignal_;

/****************************************************************************/
/**
* \description
* This function initializes the publish-subscribe facilities of QF and must
* be called exactly once before any subscriptions/publications occur in
* the application.
*
* \arguments
* \arg[in] \c subscrSto pointer to the array of subscriber lists
* \arg[in] \c maxSignal the dimension of the subscriber array and at
*             the same time the maximum signal that can be published or
*             subscribed.
*
* The array of subscriber-lists is indexed by signals and provides a mapping
* between the signals and subscriber-lists. The subscriber-lists are bitmasks
* of type ::QSubscrList, each bit in the bit mask corresponding to the unique
* priority of an active object. The size of the ::QSubscrList bit mask
* depends on the value of the #QF_MAX_ACTIVE macro.
*
* \note The publish-subscribe facilities are optional, meaning that you
* might choose not to use publish-subscribe. In that case calling QF_psInit()
* and using up memory for the subscriber-lists is unnecessary.
*
* \sa ::QSubscrList
*
* \usage
* The following example shows the typical initialization sequence of QF:
* \include qf_main.c
*/
void QF_psInit(QSubscrList * const subscrSto, enum_t const maxSignal) {
    QF_subscrList_ = subscrSto;
    QF_maxSignal_  = maxSignal;

    /* zero the subscriber list, so that the framework can start correctly
    * even if the startup code fails to clear the uninitialized data
    * (as is required by the C Standard).
    */
    QF_bzero(subscrSto,
             (uint_fast16_t)((uint_fast16_t)maxSignal
                           * (uint_fast16_t)sizeof(QSubscrList)));
}

