/*****************************************************************************
* Product: QP/C
* Last Updated for Version: 4.5.01
* Date of the Last Update:  Jun 13, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
#ifndef qep_h
#define qep_h

/**
* \file
* \ingroup qep qf qk
* \brief Public QEP/C interface.
*
* This header file must be included, perhaps indirectly, in all modules
* (*.c files) that use QEP/C
*/

#include "qevt.h"            /* QEP event processor needs the QEvt facility */


/****************************************************************************/
/** \brief obtain the current QEP version number string
*
* \return version of the QEP as a constant 6-character string of the form
* x.y.zz, where x is a 1-digit major version number, y is a 1-digit minor
* version number, and zz is a 2-digit release number.
*/
char_t const Q_ROM * Q_ROM_VAR QEP_getVersion(void);

/****************************************************************************/

                    /** \brief Type returned from  a state-handler function */
typedef uint8_t QState;

                               /** \brief pointer to state-handler function */
typedef QState (*QStateHandler)(void * const me, QEvt const * const e);

/** \brief Perform cast to QStateHandler.
*
* This macro encapsulates the cast of a specific state handler function
* pointer to QStateHandler, which violates MISRA-C 2004 rule 11.4(advisory).
* This macro helps to localize this deviation.
*/
#define Q_STATE_CAST(handler_)  ((QStateHandler)(handler_))

/****************************************************************************/
/** \brief Finite State Machine
*
* QFsm represents a traditional non-hierarchical Finite State Machine (FSM)
* without state hierarchy, but with entry/exit actions.
*
* \note QFsm is not intended to be instantiated directly, but rather serves
* as the base structure for derivation of state machines in the application
* code.
*
* The following example illustrates how to derive a state machine structure
* from QFsm. Please note that the QFsm member super is defined as the FIRST
* member of the derived struct.
* \include qep_qfsm.c
*
* \sa \ref derivation
*/
typedef struct QFsmTag {
    QStateHandler state;         /**< current active state (state-variable) */
    QStateHandler temp; /**< temporary state: target of tran. or superstate */
} QFsm;

/** \brief Protected "constructor" of a FSM.
*
* Performs the first step of FSM initialization by assigning the
* initial pseudostate to the currently active state of the state machine.
* \note Must be called only by the "constructors" of the derived state
* machines.
* \note Must be called only ONCE before QFsm_init().
*
* The following example illustrates how to invoke QFsm_ctor() in the
* "constructor" of a derived state machine:
* \include qep_qfsm_ctor.c
*/
#define QFsm_ctor(me_, initial_) do { \
    (me_)->state = Q_STATE_CAST(0); \
    (me_)->temp  = (initial_); \
} while (0)

/****************************************************************************/
/** \brief Performs the second step of FSM initialization by triggering the
* top-most initial transition.
*
* \param me pointer the state machine structure derived from QFsm
* \param e constant pointer the QEvt or a structure derived from QEvt
* \note Must be called only ONCE after the "constructor" QFsm_ctor().
*
* The following example illustrates how to initialize a FSM, and dispatch
* events to it:
* \include qep_qfsm_use.c
*/
void QFsm_init(QFsm * const me, QEvt const * const e);

/** \brief Dispatches an event to a FSM
*
* Processes one event at a time in Run-to-Completion fashion. The argument
* \a me is the pointer the state machine structure derived from QFsm.
* The argument \a e is a constant pointer the QEvt or a structure
* derived from QEvt.
*
* \note Must be called after the "constructor" QFsm_ctor() and QFsm_init().
*
* \sa example for QFsm_init() \n \ref derivation
*/
void QFsm_dispatch(QFsm * const me, QEvt const * const e);


/****************************************************************************/
/** \brief Hierarchical State Machine
*
* QHsm represents a Hierarchical Finite State Machine (HSM) with full
* support for hierarchical nesting of states, entry/exit actions,
* and initial transitions in any composite state.
*
* \note QHsm is not intended to be instantiated directly, but rather serves
* as the base structure for derivation of state machines in the application
* code.
*
* The following example illustrates how to derive a state machine structure
* from QHsm. Please note that the QHsm member super is defined as the FIRST
* member of the derived struct.
* \include qep_qhsm.c
*
* \sa \ref derivation
*/
typedef struct QFsmTag QHsm;


/* public methods */

/** \brief protected "constructor" of a HSM.
* Performs the first step of HSM initialization by assigning the
* initial pseudostate to the currently active state of the state machine.
*
* \note Must be called only by the "constructors" of the derived state
* machines.
* \note Must be called before QHsm_init().
*
* The following example illustrates how to invoke QHsm_ctor() in the
* "constructor" of a derived state machine:
* \include qep_qhsm_ctor.c
*/
#define QHsm_ctor(me_, initial_) do { \
    (me_)->state = Q_STATE_CAST(&QHsm_top); \
    (me_)->temp  = (initial_); \
} while (0)

/** \brief Obtain the current active state from a HSM (read only).
*/
#define QHsm_state(me_) Q_STATE_CAST((me_)->state)

/** \brief Performs the second step of HSM initialization by triggering the
* top-most initial transition.
*
* \param me pointer the state machine structure derived from QHsm
* \param e constant pointer the QEvt or a structure derived from QEvt
* \note Must be called only ONCE after the "constructor" QHsm_ctor().
*
* The following example illustrates how to initialize a HSM, and dispatch
* events to it:
* \include qep_qhsm_use.c
*/
void QHsm_init(QHsm * const me, QEvt const * const e);

/** \brief Dispatches an event to a HSM
*
* Processes one event at a time in Run-to-Completion fashion.
* \param me is the pointer the state machine structure derived from ::QHsm.
* \param e is a constant pointer the ::QEvt or a structure derived
* from ::QEvt.
*
* \note Must be called after the "constructor" QHsm_ctor() and QHsm_init().
*
* \sa example for QHsm_init() \n \ref derivation
*/
void QHsm_dispatch(QHsm * const me, QEvt const * const e);

/** \brief Tests if a given state is part of the current active state
* configuratioin
*
* \param me is the pointer the state machine structure derived from ::QHsm.
* \param state is a pointer to the state handler function, e.g., &QCalc_on.
*/
uint8_t QHsm_isIn(QHsm * const me, QStateHandler const state);

/* protected methods */

/** \brief the top-state.
*
* QHsm_top() is the ultimate root of state hierarchy in all HSMs derived
* from ::QHsm. This state handler always returns (QState)0, which means
* that it "handles" all events.
*
* \sa Example of the QCalc_on() state handler for Q_INIT().
*/
QState QHsm_top(void const * const me, QEvt const * const e);

/** \brief Value returned by a state-handler function when it handles
* the event.
*/
#define Q_RET_HANDLED       ((QState)0)

/** \brief Macro to specify the return of a state-handler function when
* it handles the event.
*
* You call that macro after the return statement (return Q_HANDLED();)
* Q_HANDLED() can be used both in the FSMs and HSMs.
*
* \include qepn_qfsm.c
*/
#define Q_HANDLED()         (Q_RET_HANDLED)

/** \brief Macro to specify the return of a state-handler function when
* it ignores (does not handle in any way) the event.
*/
#define Q_RET_IGNORED       ((QState)1)

/** \brief The macro returned from a non-hierarchical state-handler function
* when it ignores (does not handle) the event.
*
* You call that macro after the return statement (return Q_IGNORED();)
*
* \include qepn_qfsm.c
*/
#define Q_IGNORED()         (Q_RET_IGNORED)

/** \brief Value returned by a state-handler function when it takes a
* regular state transition.
*/
#define Q_RET_TRAN          ((QState)2)

/** \brief Designates a target for an initial or regular transition.
* Q_TRAN() can be used both in the FSMs and HSMs.
*
* \include qepn_qtran.c
*/
#define Q_TRAN(target_)  \
    (((QFsm *)me)->temp = Q_STATE_CAST(target_), Q_RET_TRAN)

/** \brief Value returned by a state-handler function when it forwards
* the event to the superstate to handle.
*/
#define Q_RET_SUPER         ((QState)3)

/** \brief Designates the superstate of a given state in an HSM.
*
* \include qepn_qhsm.c
*/
#define Q_SUPER(super_)  \
    (((QHsm *)me)->temp = Q_STATE_CAST(super_),  Q_RET_SUPER)

/** \brief Value returned by a state-handler function when a guard
* condition prevents it from handling the event.
*/
#define Q_RET_UNHANDLED    ((QState)4)

/** \brief Macro to specify the return of a state-handler function when
* it attempts to handle the event but a guard condition evaluates to false
* and there is no other explicit way of handling the event.
*/
#define Q_UNHANDLED()      (Q_RET_UNHANDLED)

/** \brief Perform downcast of an event onto a subclass of QEvt \a class_
*
* This macro encapsulates the downcast of QEvt pointers, which violates
* MISRA-C 2004 rule 11.4(advisory). This macro helps to localize this
* deviation.
*/
#define Q_EVT_CAST(class_) ((class_ const *)e)

/****************************************************************************/
/** \brief QEP reserved signals */
enum QReservedSignals {
    Q_ENTRY_SIG = 1,                   /**< signal for coding entry actions */
    Q_EXIT_SIG,                         /**< signal for coding exit actions */
    Q_INIT_SIG,                  /**< signal for coding initial transitions */
    Q_USER_SIG      /**< first signal that can be used in user applications */
};

#endif                                                             /* qep_h */
