/*****************************************************************************
* Product: "Blinky Low-Power" example
* Last updated for version: 6.6.0
* Last updated on: 2019-12-10
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#ifndef BSP_H
#define BSP_H

/* The following ticks-per-second constants determine the speed of the app.
* The default (#if 1) is the SLOW speed for humans to see the blinking.
* Change the #if 1 into #if 0 for FAST speed appropriate for logic analyzers.
*/
#if 1
    #define BSP_TICKS0_PER_SEC   2U
    #define BSP_TICKS1_PER_SEC   20U
#else
    #define BSP_TICKS0_PER_SEC   200U
    #define BSP_TICKS1_PER_SEC   2000U
#endif

#if QF_MAX_TICK_RATE < 2
    #error this application requires 2 clock tick rates
#endif

void BSP_init(void);

void BSP_led0_on(void);
void BSP_led0_off(void);

void BSP_led1_on(void);
void BSP_led1_off(void);

void BSP_tickRate0_on(void);
void BSP_tickRate1_on(void);

#endif /* BSP_H */
