/*****************************************************************************
* Product:  Time Bomb Example with "State Table"
* Last Updated for Version: 4.5.03
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
#include "statetbl.h"

enum BombSignals {                          /* all signals for the Bomb FSM */
    UP_SIG,
    DOWN_SIG,
    ARM_SIG,
    TICK_SIG,

    MAX_SIG                                        /* the number of signals */
};

enum BombStates {                            /* all states for the Bomb FSM */
    SETTING_STATE,
    TIMING_STATE,

    MAX_STATE                                       /* the number of states */
};

typedef struct TickEvtTag {
    Event super;                         /* derive from the Event structure */
    uint8_t fine_time;                           /* the fine 1/10 s counter */
} TickEvt;

typedef struct Bomb2Tag {                                   /* the Bomb FSM */
    StateTable super;               /* derive from the StateTable structure */
    uint8_t timeout;                    /* number of seconds till explosion */
    uint8_t defuse;                /* secret defuse code to disarm the bomb */
    uint8_t code;              /* currently entered code to disarm the bomb */
} Bomb2;

void Bomb2_ctor(Bomb2 *me, uint8_t defuse);            /* the "constructor" */

void Bomb2_initial     (Bomb2 *me, Event const *e);   /* initial transition */
void Bomb2_setting_UP  (Bomb2 *me, Event const *e);  /* transition function */
void Bomb2_setting_DOWN(Bomb2 *me, Event const *e);  /* transition function */
void Bomb2_setting_ARM (Bomb2 *me, Event const *e);  /* transition function */
void Bomb2_timing_UP   (Bomb2 *me, Event const *e);  /* transition function */
void Bomb2_timing_DOWN (Bomb2 *me, Event const *e);  /* transition function */
void Bomb2_timing_ARM  (Bomb2 *me, Event const *e);  /* transition function */
void Bomb2_timing_TICK (Bomb2 *me, Event const *e);  /* transition function */

                                        /* the initial value of the timeout */
#define INIT_TIMEOUT   10

/*..........................................................................*/
void Bomb2_ctor(Bomb2 *me, uint8_t defuse) {
    /* state table for Bomb state machine */
    static const Tran bomb2_state_table[MAX_STATE][MAX_SIG] = {
        { (Tran)&Bomb2_setting_UP,  (Tran)&Bomb2_setting_DOWN,
          (Tran)&Bomb2_setting_ARM, &StateTable_empty },
        { (Tran)&Bomb2_timing_UP,   (Tran)&Bomb2_timing_DOWN,
          (Tran)&Bomb2_timing_ARM,  (Tran)&Bomb2_timing_TICK  }
    };
    StateTable_ctor(&me->super,
                    &bomb2_state_table[0][0], MAX_STATE, MAX_SIG,
                    (Tran)&Bomb2_initial);      /* construct the superclass */
    me->defuse = defuse;                      /* set the secret defuse code */
}
/*..........................................................................*/
void Bomb2_initial(Bomb2 *me, Event const *e) {
    (void)e;           /* avoid the compiler warning about unused parameter */
    me->timeout = INIT_TIMEOUT;
    TRAN(SETTING_STATE);
}
/*..........................................................................*/
void Bomb2_setting_UP(Bomb2 *me, Event const *e) {
    (void)e;               /* avoid compiler warning about unused parameter */
    if (me->timeout < 60) {
        ++me->timeout;
        BSP_display(me->timeout);
    }
}
/*..........................................................................*/
void Bomb2_setting_DOWN(Bomb2 *me, Event const *e) {
    (void)e;               /* avoid compiler warning about unused parameter */
    if (me->timeout > 1) {
        --me->timeout;
        BSP_display(me->timeout);
    }
}
/*..........................................................................*/
void Bomb2_setting_ARM(Bomb2 *me, Event const *e) {
    (void)e;               /* avoid compiler warning about unused parameter */
    me->code = 0;
    TRAN(TIMING_STATE);                           /* transition to "timing" */
}
/*..........................................................................*/
void Bomb2_timing_UP(Bomb2 *me, Event const *e) {
    (void)e;               /* avoid compiler warning about unused parameter */
    me->code <<= 1;
    me->code |= 1;
}
/*..........................................................................*/
void Bomb2_timing_DOWN(Bomb2 *me, Event const *e) {
    (void)e;               /* avoid compiler warning about unused parameter */
    me->code <<= 1;
}
/*..........................................................................*/
void Bomb2_timing_ARM(Bomb2 *me, Event const *e) {
    (void)e;               /* avoid compiler warning about unused parameter */
    if (me->code == me->defuse) {
        TRAN(SETTING_STATE);                     /* transition to "setting" */
    }
}
/*..........................................................................*/
void Bomb2_timing_TICK(Bomb2 *me, Event const *e) {
    if (((TickEvt const *)e)->fine_time == 0) {
        --me->timeout;
        BSP_display(me->timeout);
        if (me->timeout == 0) {
            BSP_boom();                                 /* destroy the bomb */
        }
    }
}

/* Test harness ------------------------------------------------------------*/
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>                                          /* for _exit() */

static Bomb2 l_bomb;                                       /* time bomb FSM */

/*..........................................................................*/
int main() {

    Bomb2_ctor(&l_bomb, 0x0D);  /* instantiate with defuse code 1101 binary */

    printf("Time Bomb (State-Table)\n"
           "Press 'u'   for UP   event\n"
           "Press 'd'   for DOWN event\n"
           "Press 'a'   for ARM  event\n"
           "Press <Esc> to quit.\n");

    StateTable_init((StateTable *)&l_bomb);  /* take the initial transition */

    for (;;) {                                                /* event loop */
        static TickEvt tick_evt = { TICK_SIG, 0};

        delay(100);                                         /* 100 ms delay */

        if (++tick_evt.fine_time == 10) {
            tick_evt.fine_time = 0;
        }
        printf("T(%1d)%c", tick_evt.fine_time,
                             (tick_evt.fine_time == 0) ? '\n' : ' ');

        StateTable_dispatch((StateTable *)&l_bomb, (Event *)&tick_evt);

        if (kbhit()) {
            static Event const up_evt   = { UP_SIG   };
            static Event const down_evt = { DOWN_SIG };
            static Event const arm_evt  = { ARM_SIG  };
            Event const *e = (Event *)0;

            switch (getch()) {
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
                StateTable_dispatch((StateTable *)&l_bomb, e);
            }
        }
    }

    return 0;
}
