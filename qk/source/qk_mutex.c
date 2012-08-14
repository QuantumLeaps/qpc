/*****************************************************************************
* Product: QK/C
* Last Updated for Version: 4.4.02
* Date of the Last Update:  Apr 13, 2012
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
#include "qk_pkg.h"
/*#include "qassert.h"*/

/*Q_DEFINE_THIS_MODULE("qk_mutex")*/

/**
* \file
* \ingroup qk
* \brief QK_mutexLock()/QK_mutexUnlock() implementation.
*/

#ifdef QK_NO_MUTEX
    #error "qk_mutex.c included in the build when QK_NO_MUTEX defined"
#endif

/* package-scope objects ---------------------------------------------------*/
uint8_t QK_ceilingPrio_;                     /* ceiling priority of a mutex */

/*..........................................................................*/
QMutex QK_mutexLock(uint8_t prioCeiling) {
    uint8_t mutex;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();
    mutex = QK_ceilingPrio_;  /* the original QK priority ceiling to return */
    if (QK_ceilingPrio_ < prioCeiling) {
        QK_ceilingPrio_ = prioCeiling;     /* raise the QK priority ceiling */
    }

    QS_BEGIN_NOCRIT_(QS_QK_MUTEX_LOCK, (void *)0, (void *)0)
        QS_TIME_();                                            /* timestamp */
        QS_U8_(mutex);                             /* the original priority */
        QS_U8_(QK_ceilingPrio_);            /* the current priority ceiling */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
    return mutex;
}
/*..........................................................................*/
void QK_mutexUnlock(QMutex mutex) {
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QK_MUTEX_UNLOCK, (void *)0, (void *)0)
        QS_TIME_();                                            /* timestamp */
        QS_U8_(mutex);                             /* the original priority */
        QS_U8_(QK_ceilingPrio_);            /* the current priority ceiling */
    QS_END_NOCRIT_()

    if (QK_ceilingPrio_ > mutex) {
        QK_ceilingPrio_ = mutex;      /* restore the saved priority ceiling */
        mutex = QK_schedPrio_();          /* reuse 'mutex' to hold priority */
        if (mutex != (uint8_t)0) {
            QK_sched_(mutex);
        }
    }
    QF_CRIT_EXIT_();
}
