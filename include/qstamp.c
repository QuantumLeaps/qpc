/**
* @file
* @brief Application build time-stamp
* @note
* This module needs to be re-compiled in every new software build. To achive
* this, it is recommended to delete the object file (qstamp.o, or qstamp.obj)
* in the build directory before each build. (Most development tools allow
* you to specify a pre-build command, which is the ideal place to delete
* the qstamp object file.)
* @cond
******************************************************************************
* Last updated for version 5.6.0
* Last updated on  2015-12-18
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
* http://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/

#include "qpc.h"

extern char_t const Q_BUILD_DATE[12];
extern char_t const Q_BUILD_TIME[9];

/*! the calendar date of the last translation of the form: "Mmm dd yyyy" */
char_t const Q_BUILD_DATE[12] = __DATE__;

/*! the time of the last translation of the form: "hh:mm:ss" */
char_t const Q_BUILD_TIME[9] = __TIME__;
