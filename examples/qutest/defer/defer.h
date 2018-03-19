/*****************************************************************************
* Product: Deferred Event state pattern example
* Last Updated for Version: 6.2.0
* Date of the Last Update:  2018-03-12
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2005-2018 Quantum Leaps, LLC. All rights reserved.
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
#ifndef defer_h
#define defer_h

enum { BSP_TICKS_PER_SEC = 100 };

/*..........................................................................*/
enum TServerSignals {
    NEW_REQUEST_SIG = Q_USER_SIG, /* the new request signal */
    RECEIVED_SIG,                 /* the request has been received */
    AUTHORIZED_SIG,               /* the request has been authorized */
    TERMINATE_SIG                 /* terminate the application */
};

/*..........................................................................*/
typedef struct {
    QEvt super;       /* inherit QEvt */
    uint8_t ref_num;  /* reference number of the request */
} RequestEvt;

/*..........................................................................*/
void TServer_ctor(void);
extern QActive * const AO_TServer;

#endif /* defer_h */
