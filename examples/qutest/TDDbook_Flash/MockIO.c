/*****************************************************************************
* Purpose: example Mock for IO (Chapter 10 from TDDfEC)
* Last Updated for Version: 6.3.1
* Date of the Last Update:  2018-05-25
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
#include "qpc.h" /* for QUTEST */
#include "IO.h"  /* mocked interface */

enum {
    IO_CALL = QS_USER1,
};

/*..........................................................................*/
void MockIO_Init(void) {
    /* dictionaries... */
    QS_USR_DICTIONARY(IO_CALL);

    QS_FUN_DICTIONARY(&IO_Read);
    QS_FUN_DICTIONARY(&IO_Write);
}
/*..........................................................................*/
ioData IO_Read(ioAddress offset) {
    QS_TEST_PROBE_DEF(&IO_Read)
    ioData ret = (ioData)0;
    QS_TEST_PROBE(
        ret = (ioData)qs_tp_;
    )
    QS_BEGIN(IO_CALL, (void *)0) /* user-specific record */
        QS_FUN(&IO_Read);
        QS_I16(0, ret);
        QS_U32(0, offset);
    QS_END()
    return ret;
}
/*..........................................................................*/
void IO_Write(ioAddress offset, ioData data) {
    QS_BEGIN(IO_CALL, (void *)0) /* user-specific record */
        QS_FUN(&IO_Write);
        QS_U32(0, offset);
        QS_I16(0, data);
    QS_END()
}
