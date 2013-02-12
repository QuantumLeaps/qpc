/*****************************************************************************
* Product:  QF/C, port to TMS320C55x, TI C5500 compiler
* Last Updated for Version: 4.5.03
* Date of the Last Update:  Jan 16, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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

/*Q_DEFINE_THIS_MODULE("qf_port")*/

/* Global objects ----------------------------------------------------------*/
enum_t       QF_maxSignal_;
QSubscrList *QF_subscrList_;

/*..........................................................................*/
void bzero(uint8_t *ptr, uint16_t len) {
    while (len-- != (uint16_t)0) {
        *ptr++ = (uint8_t)0;
    }
}
/*..........................................................................*/
void QF_zero(void) {                                          /* see NOTE01 */
    extern uint8_t QF_intLockNest_;

    QF_intLockNest_     = (uint8_t)0;
    QF_maxPool_         = (uint8_t)0;
    QF_timeEvtListHead_ = (QTimeEvt *)0;
    bzero((uint8_t *)QF_active_, sizeof(QActive*)*(QF_MAX_ACTIVE + 1));
}
/*..........................................................................*/
void QF_psInit(QSubscrList * const subscrSto, uint32_t const maxSignal) {
    QF_subscrList_ = subscrSto;
    QF_maxSignal_  = maxSignal;
    bzero((uint8_t *)subscrSto, maxSignal*sizeof(QSubscrList));
}

/*****************************************************************************
* NOTE01:
* The standard TI startup code (c_int00) does NOT zero the uninitialized
* variables, as required by the C-standard. Since QF relies on the clearing
* of the static uninitialized variables, the critical QF objects are cleared
* explicitly in this port.
*/
