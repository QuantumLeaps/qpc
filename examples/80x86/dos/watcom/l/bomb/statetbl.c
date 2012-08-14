/*****************************************************************************
* Product: Generic State-Table event processor
* Date of the Last Update:  Oct 22, 2007
*
* Copyright (C) 2002-2007 Miro Samek. All rights reserved.
*
* Contact information:
* e-mail: miro@quantum-leaps.com
*****************************************************************************/
#include "bsp.h"          /* for the C99-standard exact-width integer types */
#include "statetbl.h"

              /* see also embedded-systems-friendly assertions in Chapter 6 */
#include <assert.h>

/*..........................................................................*/
void StateTable_ctor(StateTable *me,
                     Tran const *table, uint8_t n_states, uint8_t n_signals,
                     Tran initial)
{
    me->state_table = table;
    me->n_states    = n_states;
    me->n_signals   = n_signals;
    me->initial     = initial;
}
/*..........................................................................*/
void StateTable_init(StateTable *me) {
    me->state = me->n_states;
    (*me->initial)(me, (Event *)0);          /* top-most initial transition */

    assert(me->state < me->n_states);/* the initial tran. must change state */
}
/*..........................................................................*/
void StateTable_dispatch(StateTable *me, Event const *e) {
    Tran t;

    assert(e->sig < me->n_signals);          /* require the signal in range */

    t = me->state_table[me->state*me->n_signals + e->sig];
    (*t)(me, e);                         /* execute the transition function */

    assert(me->state < me->n_states);   /* ensure that state stays in range */
}
/*..........................................................................*/
void StateTable_empty(StateTable *me, Event const *e) {
    (void)me;               /* void compiler warning about unused parameter */
    (void)e;                /* void compiler warning about unused parameter */
}
