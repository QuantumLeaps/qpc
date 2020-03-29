/*****************************************************************************
* Product: DPP example with lwIP
* Last Updated for Version: 5.1.1
* Date of the Last Update:  Oct 20, 2013
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          <www.state-machine.com/licensing>
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef DPP_H
#define DPP_H

enum DPPSignals {
   EAT_SIG = Q_USER_SIG,     /* published by Table to let a philosopher eat */
   DONE_SIG,                   /* published by Philosopher when done eating */
   BTN_DOWN_SIG,    /* published by ISR_SysTick when user button is pressed */
   BTN_UP_SIG,     /* published by ISR_SysTick when user button is released */

   DISPLAY_IPADDR_SIG,         /* published by lwIPMgr to display IP addres */
   DISPLAY_CGI_SIG,             /* published by lwIPMgr to display CGI text */
   DISPLAY_UDP_SIG,             /* published by lwIPMgr to display UDP text */
   MAX_PUB_SIG,                                /* the last published signal */

   HUNGRY_SIG,          /* posted direclty to Table from hungry Philosopher */
   SEND_UDP_SIG,         /* posted directly to lwIPMgr to send text via UDP */
   LWIP_DRIVER_GROUP,                           /* LwIP driver signal group */
   LWIP_DRIVER_END = LWIP_DRIVER_GROUP + 10,          /* reserve 10 signals */
   LWIP_SLOW_TICK_SIG,                 /* slow tick signal for LwIP manager */
   MAX_SIG                                               /* the last signal */
};

typedef struct TableEvtTag {
    QEvt super;                                    /* derives from QEvt */
    uint8_t philoNum;                                 /* philosopher number */
} TableEvt;

#define MAX_TEXT_LEN 16
typedef struct TextEvtTag {
    QEvt super;                                    /* derives from QEvt */
    char text[MAX_TEXT_LEN];                             /* text to deliver */
} TextEvt;

                                                  /* number of philosophers */
#define N_PHILO 5

void Philo_ctor(void);
void Table_ctor(void);
void LwIPMgr_ctor(void);

extern QActive * const AO_Philo[N_PHILO]; /* "opaque" pointers to Philo  AO */
extern QActive * const AO_Table;          /* "opaque" pointer to Table   AO */
extern QActive * const AO_LwIPMgr;        /* "opaque" pointer to LwIPMgr AO */

#endif  /* DPP_H */
