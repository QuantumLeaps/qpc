/*****************************************************************************
* Product: QHsmTst example
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "bsp.h"
#include "qhsmtst.h"

#include <yfuns.h>                                  /* for the terminal I/O */

/*..........................................................................*/
void main (void) {
    QHsmTst_ctor();           /* instantiate the QHsmTst test state machine */

    BSP_init();                                     /* initialize the board */

    QHsm_init(the_hsm, 0);                        /* take the initial tran. */

    for (;;) {           /* interactive testing all kinds of transitions... */
        static uint8_t buf[] = "\nX:";
        __read(0, &buf[1], 1);
        if ('A' <= buf[1] && buf[1] <= 'I') {                  /* in range? */
            QEvt e;
            __write(1, buf, 3);

            BSP_LED_on();
            e.sig = buf[1] - 'A' + A_SIG;
            QHsm_dispatch(the_hsm, &e);               /* dispatch the event */
            BSP_LED_off();
        }
    }
}
