/*****************************************************************************
* Product: DPP example, BSP for QUTest
* Last Updated for Version: 6.2.0
* Date of the Last Update:  2018-03-16
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
* https://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_MODULE("bsp")

static uint32_t l_rnd;      /* random seed */

enum {
    BSP_DISPLAY_PHIL_STAT = QS_USER,
    BSP_DISPLAY_PAUSED,
    BSP_RANDOM,
};

/*..........................................................................*/
void BSP_init(int argc, char **argv) {
    Q_ALLEGE(QS_INIT(argc <= 1 ? (void *)0 : argv[1]) != (uint8_t)0);

    QS_FUN_DICTIONARY(&BSP_displayPaused);
    QS_FUN_DICTIONARY(&BSP_random);

    QS_USR_DICTIONARY(BSP_DISPLAY_PHIL_STAT);
    QS_USR_DICTIONARY(BSP_DISPLAY_PAUSED);
    QS_USR_DICTIONARY(BSP_RANDOM);
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    QS_TEST_PROBE_DEF(&BSP_displayPaused)

    QS_TEST_PROBE(
        Q_ASSERT_ID(100, 0);
    )
    QS_BEGIN(BSP_DISPLAY_PAUSED, (void *)0) /* application-specific record */
        QS_U8(0, paused);
    QS_END()
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char_t const *stat) {
    (void)n;
    (void)stat;
    QS_BEGIN(BSP_DISPLAY_PHIL_STAT, (void *)0) /* application-specific record */
        QS_U8(0, n);
        QS_STR(stat);
    QS_END()
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    QS_TEST_PROBE_DEF(&BSP_random)
    uint32_t rnd = 123U;

    QS_TEST_PROBE(
        rnd = qs_tp_;
    )
    QS_BEGIN(BSP_RANDOM, (void *)0) /* application-specific record */
        QS_U32(0, rnd);
    QS_END()
    return rnd;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
