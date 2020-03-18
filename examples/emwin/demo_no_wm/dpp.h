/*****************************************************************************
* Product: DPP example
* Last updated for version 6.8.0
* Last updated on  2020-01-22
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#ifndef DPP_H
#define DPP_H

enum DPPSignals {
   HUNGRY_SIG = Q_USER_SIG, /* sent when Philosopher becomes hungry */
   DONE_SIG,                /* sent by Philosopher when done eating */
   EAT_SIG,                 /* sent by Table to let a philosopher eat */
   PAUSE_SIG,               /* sent by the user to pause the feast */
   MAX_PUB_SIG,                                /* the last published signal */

   MOUSE_CHANGE_SIG,        /* mouse change (move or click) event */

   KEY_UP_REL_SIG,          /* hardkey UP released */
   KEY_UP_PRESS_SIG,        /* hardkey UP pressed */
   KEY_LEFT_REL_SIG,        /* hardkey LEFT released */
   KEY_LEFT_PRESS_SIG,      /* hardkey LEFT pressed */
   KEY_CENTER_REL_SIG,      /* hardkey CENTER released */
   KEY_CENTER_PRESS_SIG,    /* hardkey CENTER pressed */
   KEY_RIGHT_REL_SIG,       /* hardkey RIGHT released */
   KEY_RIGHT_PRESS_SIG,     /* hardkey RIGHT pressed */
   KEY_DOWN_REL_SIG,        /* hardkey DOWN released */
   KEY_DOWN_PRESS_SIG,      /* hardkey DOWN pressed */
   KEY_POWER_REL_SIG,       /* hardkey POWER released */
   KEY_POWER_PRESS_SIG,     /* hardkey POWER pressed */

   MAX_SIG                  /* the last signal */
};

typedef struct {
    QEvt super;             /* derives from QEvt */
    uint8_t philoNum;       /* philosopher number */
} TableEvt;

typedef struct {
    QEvt super;             /* derives from QEvt */

    /* payload: GUI_PID_STATE */
    int x, y;
    uint8_t Pressed;
    uint8_t Layer;
} MouseEvt;

/* number of philosophers */
enum { N_PHILO = 5 };

void Philo_ctor(void);
void Table_ctor(void);

extern QActive * const AO_Philo[N_PHILO];  /* "opaque" pointers to Philo AO */
extern QActive * const AO_Table;           /* "opaque" pointer  to Table AO */

#endif /* DPP_H */
