/**
* @file
* @brief "QUIT" QP Unit Internal Test
* @cond
******************************************************************************
* Last updated for version 6.9.2
* Last updated on  2021-01-12
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2021 Quantum Leaps, LLC. All rights reserved.
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
#ifndef QUIT_H
#define QUIT_H

/* macro to check an expectation */
#define EXPECT(cond_) ((cond_) \
        ? (void)0 : QUIT_fail_(#cond_, &Q_this_module_[0], __LINE__))

#ifdef __cplusplus
extern "C" {
#endif

void TEST(char const *title);
void QUIT_fail_(char const *cond, char const *module, int line);
void onRunTests(void); /* user-defined callback to run the tests */

#include "qf_port.h"   /* QF/C port from the port directory */
#include "qassert.h"   /* QP embedded systems-friendly assertions */

#ifdef Q_SPY /* software tracing enabled? */
#include "qs_port.h"   /* QS/C port from the port directory */
#else
#include "qs_dummy.h"  /* QS/C dummy (inactive) interface */
#endif

#ifdef __cplusplus
}
#endif

#endif /* QUIT_H */

