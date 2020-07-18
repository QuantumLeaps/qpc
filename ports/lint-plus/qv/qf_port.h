/**
* @file
* @brief QF/C port example for QV, generic C compiler.
* @ingroup qv
* @cond
******************************************************************************
* Last updated for version 6.8.2
* Last updated on  2020-07-17
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef QF_PORT_H
#define QF_PORT_H

/* interrupt disabling mechanism */
#define QF_INT_DISABLE()            intDisable()
#define QF_INT_ENABLE()             intEnable()

void intDisable(void);
void intEnable(void);

/* QF critical section mechansim */
#define QF_CRIT_STAT_TYPE           crit_stat_t
#define QF_CRIT_ENTRY(stat_)        ((stat_) = critEntry())
#define QF_CRIT_EXIT(stat_)         critExit(stat_)

typedef unsigned int crit_stat_t;
crit_stat_t critEntry(void);
void critExit(crit_stat_t stat);

#include "qep_port.h"  /* QEP port */
#include "qv_port.h"   /* QV port */
#include "qf.h"        /* QF platform-independent public interface */

#endif /* QF_PORT_H */

