/**
* \file
* \brief QS functions for internal use inside QP components
* \ingroup qs
* \cond
******************************************************************************
* Product: QS/C
* Last updated for version 5.3.0
* Last updated on  2014-03-27
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* \endcond
*/
#include "qs_port.h" /* QS port */
#include "qs_pkg.h"

/****************************************************************************/
/*! output uint8_t data element without format information */
/** \note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u8_(uint8_t d) {
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */

    ++QS_priv_.used; /* 1 byte about to be added */
    QS_INSERT_ESC_BYTE(d)

    QS_priv_.head   = head;    /* save the head */
    QS_priv_.chksum = chksum;  /* save the checksum */
}

/****************************************************************************/
/** \note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u8u8_(uint8_t d1, uint8_t d2) {
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */

    QS_priv_.used += (QSCtr)2; /* 2 bytes are about to be added */
    QS_INSERT_ESC_BYTE(d1)
    QS_INSERT_ESC_BYTE(d2)

    QS_priv_.head   = head;    /* save the head */
    QS_priv_.chksum = chksum;  /* save the checksum */
}

/****************************************************************************/
/**
* \note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u16_(uint16_t d) {
    uint8_t b      = (uint8_t)d;
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */

    QS_priv_.used += (QSCtr)2; /* 2 bytes are about to be added */

    QS_INSERT_ESC_BYTE(b)

    d >>= 8;
    b = (uint8_t)d;
    QS_INSERT_ESC_BYTE(b)

    QS_priv_.head   = head;    /* save the head */
    QS_priv_.chksum = chksum;  /* save the checksum */
}

/****************************************************************************/
/** \note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_u32_(uint32_t d) {
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */
    int_fast8_t i;

    QS_priv_.used += (QSCtr)4; /* 4 bytes are about to be added */
    for (i = (int_fast8_t)4; i != (int_fast8_t)0; --i) {
        uint8_t b = (uint8_t)d;
        QS_INSERT_ESC_BYTE(b)
        d >>= 8;
    }

    QS_priv_.head   = head;    /* save the head */
    QS_priv_.chksum = chksum;  /* save the checksum */
}

/****************************************************************************/
/**
* \note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_str_(char_t const *s) {
    uint8_t b      = (uint8_t)(*s);
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */
    QSCtr   used = QS_priv_.used;     /* put in a temporary (register) */

    while (b != (uint8_t)(0)) {
        chksum = (uint8_t)(chksum + b); /* update checksum */
        QS_INSERT_BYTE(b)  /* ASCII characters don't need escaping */
        QS_PTR_INC_(s);
        b = (uint8_t)(*s);
        ++used;
    }
    QS_INSERT_BYTE((uint8_t)0)  /* zero-terminate the string */
    ++used;

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
    QS_priv_.used   = used;   /* save # of used buffer space */
}

/****************************************************************************/
/**
* \note This function is only to be used through macros, never in the
* client code directly.
*/
void QS_str_ROM_(char_t const Q_ROM *s) {
    uint8_t b      = (uint8_t)Q_ROM_BYTE(*s);
    uint8_t chksum = QS_priv_.chksum; /* put in a temporary (register) */
    uint8_t *buf = QS_priv_.buf;      /* put in a temporary (register) */
    QSCtr   head = QS_priv_.head;     /* put in a temporary (register) */
    QSCtr   end  = QS_priv_.end;      /* put in a temporary (register) */
    QSCtr   used = QS_priv_.used;     /* put in a temporary (register) */

    while (b != (uint8_t)(0)) {
        chksum = (uint8_t)(chksum + b); /* update checksum */
        QS_INSERT_BYTE(b)  /* ASCII characters don't need escaping */
        QS_PTR_INC_(s);
        b = (uint8_t)Q_ROM_BYTE(*s);
        ++used;
    }
    QS_INSERT_BYTE((uint8_t)0) /* zero-terminate the string */
    ++used;

    QS_priv_.head   = head;   /* save the head */
    QS_priv_.chksum = chksum; /* save the checksum */
    QS_priv_.used   = used;   /* save # of used buffer space */
}
