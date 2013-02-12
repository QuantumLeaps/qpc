/*****************************************************************************
* Product: QF/C port to Altera Nios II, Vanilla kernel, GNU compiler
* Last Updated for Version: 4.5.03
* Date of the Last Update:  Jan 08, 2013
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
#ifndef qf_port_h
#define qf_port_h

     /* The maximum number of active objects in the application, see NOTE01 */
#define QF_MAX_ACTIVE               32
                    /* The maximum number of event pools in the application */
#define QF_MAX_EPOOL                6

                                             /* QF interrupt disable/enable */
#define QF_INT_DISABLE()            ((void)alt_irq_disable_all())
#define QF_INT_ENABLE()             alt_irq_enable_all(0xFFFFFFFFUL)

                                          /* QF critical section entry/exit */
#define QF_CRIT_STAT_TYPE           alt_irq_context
#define QF_CRIT_ENTRY(stat_)        ((stat_) = alt_irq_disable_all())
#define QF_CRIT_EXIT(stat_)         alt_irq_enable_all(stat_)


#include "sys/alt_irq.h"  /* for alt_irq_disable_all()/alt_irq_enable_all() */

#include "qep_port.h"                                           /* QEP port */
#include "qvanilla.h"                       /* "Vanilla" cooperative kernel */
#include "qf.h"                 /* QF platform-independent public interface */

/*****************************************************************************
* NOTE01:
* The maximum number of active objects QF_MAX_ACTIVE can be increased
* up to 63, if necessary. Here it is set to a lower level to save some RAM.
*/

#endif                                                         /* qf_port_h */
