/*****************************************************************************
* Product:  QF/C port, 80x86, Vanilla, Open Watcom compiler
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Nov 30, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef qf_port_h
#define qf_port_h

                 /* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE               63

                                            /* DOS interrupt disable/enable */
#define QF_INT_DISABLE()            _disable()
#define QF_INT_ENABLE()             _enable()

                                         /* DOS critical section entry/exit */
/* QF_CRIT_STAT_TYPE not defined */
#define QF_CRIT_ENTRY(dummy)        _disable()
#define QF_CRIT_EXIT(dummy)         _enable()

                                                      /* DOS ISR entry/exit */
#define QF_ISR_ENTRY()              _enable()
#define QF_ISR_EXIT() do {\
    _disable(); \
    outp(0x20, 0x20); \
} while (0)


#include <i86.h>                                /* for _disable()/_enable() */

#include "qep_port.h"                                           /* QEP port */
#include "qvanilla.h"                   /* The "Vanilla" cooperative kernel */
#include "qf.h"                 /* QF platform-independent public interface */

#endif                                                         /* qf_port_h */
