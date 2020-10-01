/*****************************************************************************
* Product: DPP example, BSP for QUTest
* Last Updated for Version: 6.9.1
* Date of the Last Update:  2020-09-16
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
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
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_MODULE("bsp")

static uint32_t l_rnd;      /* random seed */

enum {
    BSP_CALL = QS_USER,
};

/*..........................................................................*/
void BSP_init(int argc, char **argv) {
    Q_ALLEGE(QS_INIT(argc <= 1 ? (void *)0 : argv[1]) != 0U);

    QS_FUN_DICTIONARY(&BSP_displayPaused);
    QS_FUN_DICTIONARY(&BSP_displayPhilStat);
    QS_FUN_DICTIONARY(&BSP_random);
    QS_FUN_DICTIONARY(&BSP_randomSeed);
    QS_FUN_DICTIONARY(&QHsm_top);

    QS_USR_DICTIONARY(BSP_CALL);

    BSP_randomSeed(1234U);
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    QS_TEST_PROBE_DEF(&BSP_displayPaused)

    QS_TEST_PROBE(
        Q_ASSERT_ID(100, 0);
    )
    QS_BEGIN_ID(BSP_CALL, 0U) /* app-specific record */
        QS_FUN(&BSP_displayPaused);
        QS_U8(0, paused);
    QS_END()
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char_t const *stat) {
    QS_BEGIN_ID(BSP_CALL, 0U) /* app-specific record */
        QS_FUN(&BSP_displayPhilStat);
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
    QS_BEGIN_ID(BSP_CALL, 0U) /* app-specific record */
        QS_FUN(&BSP_random);
        QS_U32(0, rnd);
    QS_END()
    return rnd;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    QS_TEST_PROBE_DEF(&BSP_randomSeed)

    QS_TEST_PROBE(
        seed = qs_tp_;
    )
    l_rnd = seed;
    QS_BEGIN_ID(BSP_CALL, 0U) /* app-specific record */
        QS_FUN(&BSP_randomSeed);
        QS_U32(0, seed);
    QS_END()
}
