/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 5.1.1
* Date of the Last Update:  Nov 08, 2013
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
*                          https://state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef dpp_h
#define dpp_h

enum DPPSignals {
   HUNGRY_SIG = Q_USER_SIG,         /* sent when Philosopher becomes hungry */
   DONE_SIG,                        /* sent by Philosopher when done eating */
   EAT_SIG,                       /* sent by Table to let a philosopher eat */
   PAUSE_SIG,                        /* sent by the user to pause the feast */
   MAX_PUB_SIG,                                /* the last published signal */

   MOUSE_CHANGE_SIG,                  /* mouse change (move or click) event */

   KEY_UP_REL_SIG,                                   /* hardkey UP released */
   KEY_UP_PRESS_SIG,                                  /* hardkey UP pressed */
   KEY_LEFT_REL_SIG,                               /* hardkey LEFT released */
   KEY_LEFT_PRESS_SIG,                              /* hardkey LEFT pressed */
   KEY_CENTER_REL_SIG,                           /* hardkey CENTER released */
   KEY_CENTER_PRESS_SIG,                          /* hardkey CENTER pressed */
   KEY_RIGHT_REL_SIG,                             /* hardkey RIGHT released */
   KEY_RIGHT_PRESS_SIG,                            /* hardkey RIGHT pressed */
   KEY_DOWN_REL_SIG,                               /* hardkey DOWN released */
   KEY_DOWN_PRESS_SIG,                              /* hardkey DOWN pressed */
   KEY_POWER_REL_SIG,                             /* hardkey POWER released */
   KEY_POWER_PRESS_SIG,                            /* hardkey POWER pressed */

   MAX_SIG                                               /* the last signal */
};

                                                  /* number of philosophers */
typedef struct TableEvtTag {
    QEvent super;                                    /* derives from QEvent */
    uint8_t philoNum;                                 /* philosopher number */
} TableEvt;

typedef struct MouseEvtTag {                                 /* mouse event */
    QEvent super;                                    /* derives from QEvent */
    int xPos;
    int yPos;
    uint8_t buttonStates;
} MouseEvt;

enum { N_PHILO = 5 };                             /* number of philosophers */

void Philo_ctor(void);
void Table_ctor(void);

extern QActive * const AO_Philo[N_PHILO];  /* "opaque" pointers to Philo AO */
extern QActive * const AO_Table;           /* "opaque" pointer  to Table AO */

#endif                                                             /* dpp_h */
