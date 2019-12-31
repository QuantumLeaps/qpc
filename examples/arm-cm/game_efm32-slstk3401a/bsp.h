/*****************************************************************************
* Product: "Fly 'n' Shoot" game example on EFM32-SLSTK3401A board
* Last updated for version 5.8.1
* Last updated on  2016-12-12
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#ifndef BSP_H
#define BSP_H

#define BSP_TICKS_PER_SEC    33U
#define BSP_SCREEN_WIDTH     128U
#define BSP_SCREEN_HEIGHT    128U

void BSP_init(void);
void BSP_terminate(int16_t result);

void BSP_updateScreen(void);
void BSP_clearFB(void);
void BSP_clearWalls(void);
void BSP_paintString(uint8_t x, uint8_t y, char const *str);
void BSP_paintBitmap(uint8_t x, uint8_t y, uint8_t bmp_id);
void BSP_advanceWalls(uint8_t top, uint8_t bottom);
void BSP_updateScore(uint16_t score);

bool BSP_isThrottle(void); /* is the throttle button depressed? */
bool BSP_doBitmapsOverlap(uint8_t bmp_id1, uint8_t x1, uint8_t y1,
                          uint8_t bmp_id2, uint8_t x2, uint8_t y2);
bool BSP_isWallHit(uint8_t bmp_id, uint8_t x, uint8_t y);

void BSP_displayOn(void);
void BSP_displayOff(void);

void BSP_randomSeed(uint32_t seed); /* random seed */
uint32_t BSP_random(void);          /* pseudo-random generator */

extern QActive *the_Ticker0; /* "Ticker" active object for tick rate 0 */

#endif /* BSP_H */
