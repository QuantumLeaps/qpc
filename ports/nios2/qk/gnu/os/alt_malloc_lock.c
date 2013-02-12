/*****************************************************************************
* Product: QK/C port to Altera Nios II, GNU compiler
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
#include "qp_port.h"
#include "qk_pkg.h"
#include <reent.h>

Q_DEFINE_THIS_MODULE("alt_malloc_lock")

                                             /* priority ceiling for malloc */
alt_u8 ALT_malloc_ceiling = QF_MAX_ACTIVE + 1;

static alt_u8 l_mutexStack[8];
static alt_u8 l_mutexStackTop = (alt_u8)0;

/*..........................................................................*/
void __malloc_lock(struct _reent *_r) {
    alt_irq_context ctx = alt_irq_disable_all();

    Q_ASSERT(l_mutexStackTop < sizeof(l_mutexStack));  /* no stack overflow */

    l_mutexStack[l_mutexStackTop++] = QK_ceilingPrio_;
                       /* lock the QK scheduler up to the specified ceiling */
    if (QK_ceilingPrio_ < ALT_malloc_ceiling) {
        QK_ceilingPrio_ = ALT_malloc_ceiling;
    }

    alt_irq_enable_all(ctx);
}

/*..........................................................................*/
void __malloc_unlock(struct _reent *_r) {
    alt_u8 mutex;
    alt_irq_context ctx = alt_irq_disable_all();

    Q_ASSERT(0 < l_mutexStackTop);            /* the stack must not be empty */

    mutex = l_mutexStack[--l_mutexStackTop];
    if (QK_ceilingPrio_ > mutex) {
        QK_ceilingPrio_ = mutex;      /* restore the saved priority ceiling */
        mutex = QK_schedPrio_();     /* reuse the mutex variable as priority */
        if (mutex != (alt_u8)0) {
            QK_sched_(mutex);
        }
    }
    alt_irq_enable_all(ctx);
}
