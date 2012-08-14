/*****************************************************************************
* Product: Generic State-Table event processor
* Date of the Last Update:  Oct 22, 2007
*
* Copyright (C) 2002-2007 Miro Samek. All rights reserved.
*
* Contact information:
* e-mail: miro@quantum-leaps.com
*****************************************************************************/
#ifndef statetbl_h
#define statetbl_h

typedef struct EventTag {
    uint16_t sig;                                    /* signal of the event */
    /* add event parameters by derivation from the Event structure */
} Event;

struct StateTableTag;                                /* forward declaration */

typedef void (*Tran)(struct StateTableTag *me, Event const *e);

typedef struct StateTableTag {
    uint8_t state;                              /* the current active state */
    Tran const *state_table;                             /* the State-Table */
    uint8_t n_states;                                   /* number of states */
    uint8_t n_signals;                                 /* number of signals */
    Tran initial;                                 /* the initial transition */
} StateTable;

void StateTable_ctor(StateTable *me,
                     Tran const *table, uint8_t n_states, uint8_t n_signals,
                     Tran initial);
void StateTable_init(StateTable *me);
void StateTable_dispatch(StateTable *me, Event const *e);
void StateTable_empty(StateTable *me, Event const *e);

        /* macro for taking a state transition inside a transition function */
#define TRAN(target_)  (((StateTable *)me)->state = (uint8_t)(target_))

#endif                                                        /* statetbl_h */
