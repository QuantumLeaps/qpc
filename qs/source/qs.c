/*****************************************************************************
* Product: QS/C
* Last Updated for Version: 5.1.0
* Date of the Last Update:  Sep 18, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
#include "qs_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qs")

/**
* \file
* \ingroup qs
* \brief QS internal variables definitions and
* core QS functions implementations.
*/

/*..........................................................................*/
QSPriv QS_priv_;                                         /* QS private data */

/*..........................................................................*/
void QS_initBuf(uint8_t sto[], uint32_t stoSize) {
    uint8_t *buf = &sto[0];

    Q_REQUIRE(stoSize > (uint32_t)8);  /* the storage size at least 7 bytes */

    QS_priv_.buf  = buf;
    QS_priv_.end  = (QSCtr)stoSize;
    QS_priv_.seq  = (uint8_t)0;
    QS_priv_.head = (QSCtr)0;
    QS_priv_.tail = (QSCtr)0;
    QS_priv_.used = (QSCtr)0;

    QS_beginRec((uint8_t)QS_EMPTY);
    QS_endRec();
    QS_beginRec((uint8_t)QS_QP_RESET);
    QS_endRec();
}
/*..........................................................................*/
void QS_filterOn(uint8_t rec) {
    if (rec == QS_ALL_RECORDS) {
        uint8_t i;
        for (i = (uint8_t)0;
             i < (uint8_t)(sizeof(QS_priv_.glbFilter) - 1U);
             ++i)
        {
            QS_priv_.glbFilter[i] = (uint8_t)0xFF;
        }
        /* never turn the last 3 records on (0x7D, 0x7E, 0x7F) */
        QS_priv_.glbFilter[sizeof(QS_priv_.glbFilter) - 1U] = (uint8_t)0x1F;
    }
    else {
        Q_ASSERT(rec < QS_ESC);/*so that record numbers don't need escaping */
        QS_priv_.glbFilter[rec >> 3] |=
            (uint8_t)(1U << (rec & (uint8_t)7));
    }
}
/*..........................................................................*/
void QS_filterOff(uint8_t rec) {
    if (rec == QS_ALL_RECORDS) {
        uint8_t *glbFilter = &QS_priv_.glbFilter[0];

        /* the following unrolled loop is designed to stop collecting trace
        * very fast in order to prevent overwriting the interesting data.
        * The code assumes that the size of QS_priv_.glbFilter[] is 16.
        */
        Q_ASSERT_COMPILE(sizeof(QS_priv_.glbFilter) == 16U);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);

        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);

        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);

        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;  QS_PTR_INC_(glbFilter);
        *glbFilter = (uint8_t)0;
    }
    else {
        Q_ASSERT(rec < QS_ESC);/*so that record numbers don't need escaping */
        QS_priv_.glbFilter[rec >> 3] &=
            (uint8_t)(~(uint8_t)(1U << (rec & (uint8_t)0x07)));
    }
}
/*..........................................................................*/
void QS_beginRec(uint8_t rec) {
    uint8_t b      = (uint8_t)(QS_priv_.seq + (uint8_t)1);
    uint8_t chksum = (uint8_t)0;                      /* reset the checksum */
    uint8_t *buf   = QS_priv_.buf;         /* put in a temporary (register) */
    QSCtr   head   = QS_priv_.head;        /* put in a temporary (register) */
    QSCtr   end    = QS_priv_.end;         /* put in a temporary (register) */

    QS_priv_.seq = b;                 /* store the incremented sequence num */
    QS_priv_.used += (QSCtr)2;                 /* 2 bytes about to be added */

    QS_INSERT_ESC_BYTE(b)

    chksum = (uint8_t)(chksum + rec);                    /* update checksum */
    QS_INSERT_BYTE(rec)                  /* rec byte does not need escaping */

    QS_priv_.head   = head;                                /* save the head */
    QS_priv_.chksum = chksum;                          /* save the checksum */
}
/*..........................................................................*/
void QS_endRec(void) {
    uint8_t b    = (uint8_t)(~QS_priv_.chksum);
    uint8_t *buf = QS_priv_.buf;           /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;
    QSCtr   end  = QS_priv_.end;

    QS_priv_.used += (QSCtr)2;                 /* 2 bytes about to be added */

    if ((b != QS_FRAME) && (b != QS_ESC)) {
        QS_INSERT_BYTE(b)
    }
    else {
        QS_INSERT_BYTE(QS_ESC)
        QS_INSERT_BYTE(b ^ QS_ESC_XOR)
        ++QS_priv_.used;                        /* account for the ESC byte */
    }

    QS_INSERT_BYTE(QS_FRAME)                 /* do not escape this QS_FRAME */

    QS_priv_.head = head;                                  /* save the head */
    if (QS_priv_.used > end) {                /* overrun over the old data? */
        QS_priv_.used = end;                    /* the whole buffer is used */
        QS_priv_.tail = head;             /* shift the tail to the old data */
    }
}
/*..........................................................................*/
void QS_u8(uint8_t format, uint8_t d) {
    uint8_t chksum = QS_priv_.chksum;      /* put in a temporary (register) */
    uint8_t *buf   = QS_priv_.buf;         /* put in a temporary (register) */
    QSCtr   head   = QS_priv_.head;        /* put in a temporary (register) */
    QSCtr   end    = QS_priv_.end;         /* put in a temporary (register) */

    QS_priv_.used += (QSCtr)2;                 /* 2 bytes about to be added */

    QS_INSERT_ESC_BYTE(format)
    QS_INSERT_ESC_BYTE(d)

    QS_priv_.head   = head;                                /* save the head */
    QS_priv_.chksum = chksum;                          /* save the checksum */
}
/*..........................................................................*/
void QS_u16(uint8_t format, uint16_t d) {
    uint8_t chksum = QS_priv_.chksum;      /* put in a temporary (register) */
    uint8_t *buf   = QS_priv_.buf;         /* put in a temporary (register) */
    QSCtr   head   = QS_priv_.head;        /* put in a temporary (register) */
    QSCtr   end    = QS_priv_.end;         /* put in a temporary (register) */

    QS_priv_.used += (QSCtr)3;                 /* 3 bytes about to be added */

    QS_INSERT_ESC_BYTE(format)

    format = (uint8_t)d;
    QS_INSERT_ESC_BYTE(format)

    d >>= 8;
    format = (uint8_t)d;
    QS_INSERT_ESC_BYTE(format)

    QS_priv_.head   = head;                                /* save the head */
    QS_priv_.chksum = chksum;                          /* save the checksum */
}
/*..........................................................................*/
void QS_u32(uint8_t format, uint32_t d) {
    uint8_t chksum = QS_priv_.chksum;      /* put in a temporary (register) */
    uint8_t *buf   = QS_priv_.buf;         /* put in a temporary (register) */
    QSCtr   head   = QS_priv_.head;        /* put in a temporary (register) */
    QSCtr   end    = QS_priv_.end;         /* put in a temporary (register) */
    int_t   i;

    QS_priv_.used += (QSCtr)5;                 /* 5 bytes about to be added */
    QS_INSERT_ESC_BYTE(format)                    /* insert the format byte */

    for (i = (int_t)4; i != (int_t)0; --i) {      /* insert 4 bytes of data */
        format = (uint8_t)d;
        QS_INSERT_ESC_BYTE(format)
        d >>= 8;
    }

    QS_priv_.head   = head;                                /* save the head */
    QS_priv_.chksum = chksum;                          /* save the checksum */
}


