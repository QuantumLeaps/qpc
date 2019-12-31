/**
* @file
* @brief Macros for casting strongly-typed integer constants.
*
* @description This header file provides the standard C99 and C++11
* macros of the form UINT8_C(), UINT_FAST8_C(), etc. for building
* strongly-typed constants of standard integers from <stdint.h>.
*
* @cond
******************************************************************************
* Last updated for version 6.6.0
* Last updated on  2019-07-30
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
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
#ifndef STDINT_C_H
#define STDINT_C_H

#ifndef UINT8_C
#define UINT8_C(x_)        ((uint8_t)(x_))
#endif

#ifndef INT8_C
#define INT8_C(x_)         ((int8_t)(x_))
#endif

#ifndef UINT_FAST8_C
#define UINT_FAST8_C(x_)   ((uint_fast8_t)(x_))
#endif

#ifndef INT_FAST8_C
#define INT_FAST8_C(x_)    ((int_fast8_t)(x_))
#endif

#ifndef UINT16_C
#define UINT16_C(x_)       ((uint16_t)(x_))
#endif

#ifndef INT16_C
#define INT16_C(x_)        ((int16_t)(x_))
#endif

#ifndef UINT_FAST16_C
#define UINT_FAST16_C(x_)  ((uint_fast16_t)(x_))
#endif

#ifndef UINT32_C
#define UINT32_C(x_)       ((uint32_t)(x_))
#endif

#ifndef INT32_C
#define INT32_C(x_)        ((int32_t)(x_))
#endif

#ifndef UINT_FAST32_C
#define UINT_FAST32_C(x_)  ((uint_fast32_t)(x_))
#endif

#endif /* STDINT_C_H */
