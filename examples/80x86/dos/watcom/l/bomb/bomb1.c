/*****************************************************************************
* Product:  Time Bomb Example with "Nested Switch Statement"
* Last Updated for Version: 4.1.01
* Date of the Last Update:  Nov 04, 2009
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2009 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "bsp.h"

enum BombSignals {                          /* all signals for the Bomb FSM */
    UP_SIG,
    DOWN_SIG,
    ARM_SIG,
    TICK_SIG
};

enum BombStates {                            /* all states for the Bomb FSM */
    SETTING_STATE,
    TIMING_STATE
};

typedef struct EventTag {
    uint16_t sig;                                    /* signal of the event */
    /* add event parameters by derivation from the Event structure... */
} Event;

typedef struct TickEvtTag {
    Event super;                         /* derive from the Event structure */
    uint8_t fine_time;                           /* the fine 1/10 s counter */
} TickEvt;

typedef struct Bomb1Tag {                                   /* the Bomb FSM */
    uint8_t state;                             /* the scalar state-variable */
    uint8_t timeout;                    /* number of seconds till explosion */
    uint8_t code;              /* currently entered code to disarm the bomb */
    uint8_t defuse;                /* secret defuse code to disarm the bomb */
} Bomb1;

void Bomb1_ctor(Bomb1 *me, uint8_t defuse);            /* the "constructor" */
void Bomb1_init(Bomb1 *me);                              /* init() function */
void Bomb1_dispatch(Bomb1 *me, Event const *e);

                                     /* macro for taking a state transition */
#define TRAN(target_)  (me->state = (uint8_t)(target_))

                                        /* the initial value of the timeout */
#define INIT_TIMEOUT   10

/*..........................................................................*/
void Bomb1_ctor(Bomb1 *me, uint8_t defuse) {
    me->defuse = defuse;    /* the defuse code is assigned at instantiation */
}
/*..........................................................................*/
void Bomb1_init(Bomb1 *me) {
    me->timeout = INIT_TIMEOUT;  /* timeout is initialized in initial tran. */
    TRAN(SETTING_STATE);
}
/*..........................................................................*/
void Bomb1_dispatch(Bomb1 *me, Event const *e) {
    switch (me->state) {
        case SETTING_STATE: {
            switch (e->sig) {
                case UP_SIG: {
                    if (me->timeout < 60) {
                        ++me->timeout;
                        BSP_display(me->timeout);
                    }
                    break;
                }
                case DOWN_SIG: {
                    if (me->timeout > 1) {
                        --me->timeout;
                        BSP_display(me->timeout);
                    }
                    break;
                }
                case ARM_SIG: {
                    me->code = 0;
                    TRAN(TIMING_STATE);           /* transition to "timing" */
                    break;
                }
            }
            break;
        }
        case TIMING_STATE: {
            switch (e->sig) {
                case UP_SIG: {
                    me->code <<= 1;
                    me->code |= 1;
                    break;
                }
                case DOWN_SIG: {
                    me->code <<= 1;
                    break;
                }
                case ARM_SIG: {
                    if (me->code == me->defuse) {
                        TRAN(SETTING_STATE);     /* transition to "setting" */
                    }
                    break;
                }
                case TICK_SIG: {
                    if (((TickEvt const *)e)->fine_time == 0) {
                        --me->timeout;
                        BSP_display(me->timeout);
                        if (me->timeout == 0) {
                            BSP_boom();                 /* destroy the bomb */
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}

/* Test harness ------------------------------------------------------------*/
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>                                          /* for _exit() */

static Bomb1 l_bomb;                                       /* time bomb FSM */

/*..........................................................................*/
void main() {

    Bomb1_ctor(&l_bomb, 0x0D);       /* the secret defuse code, 1101 binary */

    printf("Time Bomb (Nested switch)\n"
           "Press 'u'   for UP   event\n"
           "Press 'd'   for DOWN event\n"
           "Press 'a'   for ARM  event\n"
           "Press <Esc> to quit.\n");

    Bomb1_init(&l_bomb);                     /* take the initial transition */

    for (;;) {                                                /* event loop */
        static TickEvt tick_evt = { TICK_SIG, 0};

        delay(100);                                         /* 100 ms delay */

        if (++tick_evt.fine_time == 10) {
            tick_evt.fine_time = 0;
        }
        printf("T(%1d)%c", tick_evt.fine_time,
                             (tick_evt.fine_time == 0) ? '\n' : ' ');

        Bomb1_dispatch(&l_bomb, (Event *)&tick_evt); /* dispatch tick event */

        if (_kbhit()) {
            static Event const up_evt   = { UP_SIG   };
            static Event const down_evt = { DOWN_SIG };
            static Event const arm_evt  = { ARM_SIG  };
            Event const *e = (Event *)0;

            switch (_getch()) {
                case 'u': {                                     /* UP event */
                    printf("\nUP  : ");
                    e = &up_evt;                   /* generate the UP event */
                    break;
                }
                case 'd': {                                   /* DOWN event */
                    printf("\nDOWN: ");
                    e = &down_evt;               /* generate the DOWN event */
                    break;
                }
                case 'a': {                                    /* ARM event */
                    printf("\nARM : ");
                    e = &arm_evt;                 /* generate the ARM event */
                    break;
                }
                case '\33': {                                  /* <Esc> key */
                    printf("\nESC : Bye! Bye!");
                    fflush(stdout);
                    _exit(0);
                    break;
                }
            }

            if (e != (Event *)0) {             /* keyboard event available? */
                Bomb1_dispatch(&l_bomb, e);           /* dispatch the event */
            }
        }
    }
}
