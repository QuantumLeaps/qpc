/**
* \file
* \ingroup qk
* \brief QK_mutexLock() and QK_mutexUnlock() definitions.
* \cond
******************************************************************************
* Product: QK/C
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
#include "qf_port.h"
#include "qk_pkg.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

#ifdef QK_NO_MUTEX
    #error "qk_mutex.c included in the build when QK_NO_MUTEX defined"
#endif

/* Package-scope objects ****************************************************/
uint_fast8_t volatile QK_ceilingPrio_; /* ceiling priority of a mutex */

/****************************************************************************/
/**
* \description
* Lock the QK scheduler up to the given priority level.
*
* \arguments
* \arg[in] \c prioCeiling  priority ceiling to lock the mutex
*
* \returns the previous value of the mutex priority ceiling
*
* \note This function should be always paired with QK_mutexUnlock(). The
* code between QK_mutexLock() and QK_mutexUnlock() should be kept to the
* minimum.
*
* \usage
* \include qk_mux.c
*/
QMutex QK_mutexLock(uint_fast8_t prioCeiling) {
    QMutex mutex;
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();
    mutex = QK_ceilingPrio_; /* the original QK priority ceiling to return */
    if (QK_ceilingPrio_ < prioCeiling) {
        QK_ceilingPrio_ = prioCeiling; /* raise the QK priority ceiling */
    }

    QS_BEGIN_NOCRIT_(QS_QK_MUTEX_LOCK, (void *)0, (void *)0)
        QS_TIME_();                       /* timestamp */
        QS_U8_((uint8_t)mutex);           /* the original priority */
        QS_U8_((uint8_t)QK_ceilingPrio_); /* the current priority ceiling */
    QS_END_NOCRIT_()

    QF_CRIT_EXIT_();
    return mutex;
}

/****************************************************************************/
/**
* \description
* Unlock the QK scheduler up to the saved priority level.
*
* \arguments
* \arg[in] \c mutex  previous priority level to unlock the mutex
*
* \description
* \note This function should be always paired with QK_mutexLock(). The
* code between QK_mutexLock() and QK_mutexUnlock() should be kept to the
* minimum.
*
* \usage
* \include qk_mux.c
*/
void QK_mutexUnlock(QMutex mutex) {
    QF_CRIT_STAT_
    QF_CRIT_ENTRY_();

    QS_BEGIN_NOCRIT_(QS_QK_MUTEX_UNLOCK, (void *)0, (void *)0)
        QS_TIME_();                       /* timestamp */
        QS_U8_((uint8_t)mutex);           /* the original priority */
        QS_U8_((uint8_t)QK_ceilingPrio_); /* the current priority ceiling */
    QS_END_NOCRIT_()

    if (QK_ceilingPrio_ > mutex) {
        QK_ceilingPrio_ = mutex;  /* restore the saved priority ceiling */
        mutex = QK_schedPrio_();  /* reuse 'mutex' to hold priority */
        if (mutex != (QMutex)0) {
            QK_sched_(mutex);
        }
    }
    QF_CRIT_EXIT_();
}
