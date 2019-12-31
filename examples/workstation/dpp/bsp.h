/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jul 04, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          <www.state-machine.com/licensing>
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef BSP_H
#define BSP_H

#define BSP_TICKS_PER_SEC    50U

void BSP_init(int argc, char *argv[]);
void BSP_displayPaused(uint8_t paused);
void BSP_displayPhilStat(uint8_t n, char_t const *stat);
void BSP_terminate(int16_t result);

void BSP_randomSeed(uint32_t seed);                          /* random seed */
uint32_t BSP_random(void);                       /* pseudo-random generator */

#endif                                                             /* BSP_H */
