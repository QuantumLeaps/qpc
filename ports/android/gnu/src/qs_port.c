/*****************************************************************************
* Product:  QS/C, port to Android NDK
* Last Updated for Version: 4.5.03
* Date of the Last Update:  Nov 04, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
#include "qs_pkg.h"
#include "qassert.h"

#include "qspy.h"
#include <time.h>
#include <limits.h>

Q_DEFINE_THIS_MODULE("qs_port")

#define USEC_IN_NSEC    1000UL
#define USEC_IN_SEC     1000000UL

/* Number of seconds that can represented in 1 microseconds resolution
*  for 32 bit unsigned integer.
*/
#define ROLL_OVER_SEC  ((ULONG_MAX - USEC_IN_SEC) / USEC_IN_SEC)

/*..........................................................................*/
void QS_onFlush(void) {
    uint16_t nBytes = 1024U;
    uint8_t const *block;
    QF_CRIT_ENTRY(dummy);
    while ((block = QS_getBlock(&nBytes)) != (uint8_t *)0) {
        QF_CRIT_EXIT(dummy);
        QSPY_parse(block, nBytes);
        nBytes = 1024U;
    }
}

/* The QS_onGetTime assumes 4 byte time counter configuration */
Q_ASSERT_COMPILE(sizeof(QSTimeCtr) == sizeof(uint32_t));

/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {
    struct timespec time;

    clock_gettime(CLOCK_REALTIME, &time);
    /* convert sec + nanosec to uint32_t timestamp */
    return (QSTimeCtr) ((time.tv_sec % ROLL_OVER_SEC) * USEC_IN_SEC
                        + (time.tv_nsec / USEC_IN_NSEC));
}

