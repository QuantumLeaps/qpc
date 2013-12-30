/*****************************************************************************
* Product: QP/C
* Last Updated for Version: 5.2.0
* Date of the Last Update:  Nov 30, 2013
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
              /** \brief Type returned from a state/action-handler function */
typedef uint8_t QState;

                             /** \brief pointer to a state-handler function */
typedef QState (*QStateHandler)(void * const me, QEvt const * const e);

                           /** \brief pointer to an action-handler function */
typedef QState (*QActionHandler)(void * const me);

/** \brief State object for the ::QMsm class (Meta State Machine).
*
* This class groups together the attributes of a ::QMsm state, such as the
* parent state (state nesting), the associated state handler function and
* the exit action handler function. These attributes are used inside the
* QMsm_dispatch() and QMsm_init() functions.
*
* \note The ::QMState class is only intended for the QM code generator
* and should not be used in hand-crafted code.
*/
typedef struct QMStateTag {
    struct QMStateTag const *parent;                      /**< parent state */
    QStateHandler     const stateHandler;      /**<  state handler function */
    QActionHandler    const exitAction;   /**< exit action handler function */
} QMState;

/** \brief Attribute of for the ::QMsm class (Meta State Machine).
*
* This union represents possible values stored in the 'state' and 'temp'
* attributes of the ::QMsm class.
*/
typedef union QMAttrTag {
    QMState        const *obj;               /**< pointer to QMState object */
    QActionHandler const *act;                /**< array of action handlers */
    QStateHandler  fun;            /**< pointer to a state handler function */
} QMAttr;

/****************************************************************************/
/** virtual table for the ::QMsm class */
typedef struct QMsmVtblTag QMsmVtbl;

/** \brief Meta State Machine
*
* QMsm represents the most fundamental State Machine in QP. The application-
* level state machines derived directly from QMsm typically require the use
* of the QM modeling tool, but are the fastest and need the least run-time
* support (the smallest event-processor taking up the least code space).
*
* QMsm is also the base class for the QFsm and QHsm state machines, which
* can be coded and maintained by hand (as well as by QM), but aren't as fast
* and requrie significantly more run-time code (0.5-1KB) to execute.
*
* \note QMsm is not intended to be instantiated directly, but rather serves
* as the base structure for derivation of state machines in the application
* code.
*
* The following example illustrates how to derive a state machine structure
* from QMsm. Please note that the QMsm member 'super' is defined as the
* _first_ member of the derived struct.
* \include qep_qmsm.c
*
* \sa \ref derivation
*/
typedef struct QMsmTag {
    QMsmVtbl const *vptr;                       /**< \brief virtual pointer */
    QMAttr state;         /**< \brief current active state (state-variable) */
    QMAttr temp; /**< \brief temporary: transition chain, target state, etc.*/
} QMsm;

/** \brief Virtual table for the ::QMsm class
*
* The ::QMsm class provides two virtual functions: init() and dispatch().
*/
struct QMsmVtblTag {
    /** Triggers the top-most initial transition in a SM. */
    void (*init)(QMsm * const me, QEvt const * const e);

    /** Dispatches an event to a SM. */
    void (*dispatch)(QMsm * const me, QEvt const * const e);
};

/** \brief Protected "constructor" of a ::QMsm.
*
* Performs the first step of QMsm initialization by assigning the initial
* pseudostate to the currently active state of the state machine.
*
* \note Must be called only ONCE before QMSM_INIT().
*
* The following example illustrates how to invoke QMsm_ctor() in the
* "constructor" of a derived state machine:
* \include qep_qmsm_ctor.c
*/
void QMsm_ctor(QMsm * const me, QStateHandler initial);

/** \brief Polymorphically executes the top-most initial transition in a SM.
*
* \param me_ pointer the state machine structure derived from QMsm
* \param e_ constant pointer the ::QEvt or a structure derived from ::QEvt
* \note Must be called only ONCE after the SM "constructor".
*
* The following example illustrates how to initialize a SM, and dispatch
* events to it:
* \include qep_qmsm_use.c
*/
#define QMSM_INIT(me_, e_) ((*(me_)->vptr->init)((me_), (e_)))

/** \brief Implementation of the top-most initial transition in ::QMsm.
*/
void QMsm_init_(QMsm * const me, QEvt const * const e);

/** \brief Polymorphically dispatches an event to a SM.
*
* Processes one event at a time in Run-to-Completion fashion. The argument
* \a me_ is the pointer the state machine structure derived from ::QMsm.
* The argument \a e is a constant pointer the ::QEvt or a structure
* derived from ::QEvt.
*
* \note Must be called after the "constructor" and after QMSM_INIT().
*
* \sa example for QMsm_init() \n \ref derivation
*/
#define QMSM_DISPATCH(me_, e_) ((*(me_)->vptr->dispatch)((me_), (e_)))

/** \brief Implementation of disparching events to QMsm.
*/
void QMsm_dispatch_(QMsm * const me, QEvt const * const e);

extern QActionHandler const QMsm_emptyAction_[1];


/****************************************************************************/
/** \brief Hierarchical State Machine
*
* QHsm represents a Hierarchical Finite State Machine (HSM) with full
* support for hierarchical nesting of states, entry/exit actions,
* and initial transitions in any composite state. QHsm inherits QMsm
* "as is" without adding new attributes, so it is typedef'ed as QMsm.
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
typedef QMsm QHsm;

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
void QHsm_ctor(QHsm * const me, QStateHandler initial);

/** \brief Obtain the current active state from a HSM (read only).
*/
#define QHsm_state(me_) (Q_STATE_CAST((me_)->state.fun))

/** \brief Implementation of the top-most initial transition in QHsm.
*/
void QHsm_init_(QHsm * const me, QEvt const * const e);

/** \brief Implementation of dispatching events to QHsm.
*/
void QHsm_dispatch_(QHsm * const me, QEvt const * const e);

/** \brief Tests if a given state is part of the current active state
* configuration
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
*/
QState QHsm_top(void const * const me, QEvt const * const e);


/****************************************************************************/
/** \brief Non-hierarchical Finite State Machine
*
* QFsm represents a traditional non-hierarchical Finite State Machine (FSM)
* without state hierarchy, but with entry/exit actions. QFsm inherits QMsm
* "as is" without adding new attributes, so it is typedef'ed as QMsm.
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
typedef QMsm QFsm;

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
void QFsm_ctor(QFsm * const me, QStateHandler initial);

/** \brief Implementation of the top-most initial transition in QFsm.
*/
void QFsm_init_(QFsm * const me, QEvt const * const e);

/** \brief Implementation of disparching events to QFsm.
*/
void QFsm_dispatch_(QFsm * const me, QEvt const * const e);


/****************************************************************************/
/** \brief obtain the current QEP version number string
*
* version of QEP as a constant 5-character string of the form X.Y.Z,
* where X is a 1-digit major version number, Y is a 1-digit minor
* version number, and Z is a 1-digit release number.
*/
#define QEP_getVersion() (QP_VERSION_STR)

/****************************************************************************/

enum QHandlerReturnValues {
    Q_RET_SUPER,       /**< \brief event passed to the superstate to handle */
    Q_RET_HANDLED,     /**< \brief event handled (internal transition)      */
    Q_RET_UNHANDLED,   /**< \brief event unhandled                          */
    Q_RET_IGNORED,     /**< \brief event ignored (bubbled up to top)        */
    Q_RET_TRAN,        /**< \brief regular transition taken                 */
    Q_RET_ENTRY,       /**< \brief entry action executed                    */
    Q_RET_EXIT,        /**< \brief exit action executed                     */
    Q_RET_INITIAL      /**< \brief initial transition taken                 */
};


/** \brief Perform cast to QStateHandler.
*
* This macro encapsulates the cast of a specific state handler function
* pointer to QStateHandler, which violates MISRA-C 2004 rule 11.4(advisory).
* This macro helps to localize this deviation.
* \include qep_qhsm_ctor.c
*/
#define Q_STATE_CAST(handler_)  ((QStateHandler)(handler_))

/** \brief Perform cast to QActionHandler.
*
* This macro encapsulates the cast of a specific action handler function
* pointer to QActionHandler, which violates MISRA-C 2004 rule 11.4(advisory).
* This macro helps to localize this deviation.
*/
#define Q_ACTION_CAST(action_)  ((QActionHandler)(action_))


#ifdef Q_SPY
    /** \brief Macro to call in a QM action-handler when it executes
    * an entry action. Applicable only to QMSMs.
    */
    #define QM_ENTRY(state_) \
        (((QMsm *)me)->temp.obj = (state_), (QState)Q_RET_ENTRY)

    /** \brief Macro to call in a QM action-handler when it executes
    * an exit action. Applicable only to QMSMs.
    */
    #define QM_EXIT(state_) \
        (((QMsm *)me)->temp.obj = (state_), (QState)Q_RET_EXIT)
#else
    #define QM_ENTRY(dummy) ((QState)Q_RET_ENTRY)

    #define QM_EXIT(dummy)  ((QState)Q_RET_EXIT)
#endif

/** \brief Macro to call in a QM state-handler when it executes a regular
* transition. Applicable only to QMSMs.
*/
#define QM_TRAN(target_, act_) \
    (((((QMsm *)me)->state.obj = (target_)), \
      (((QMsm *)me)->temp.act = (act_))), (QState)Q_RET_TRAN)

/** \brief Macro to call in a QM state-handler when it executes an initial
* transition. Applicable only to QMSMs.
*/
#define QM_INITIAL(target_, act_) \
    (((((QMsm *)me)->state.obj = (target_)), \
      (((QMsm *)me)->temp.act = (act_))), (QState)Q_RET_INITIAL)

/** \brief Macro to call in a QM state-handler when it handled an event.
* Applicable only to QMSMs.
*/
#define QM_HANDLED()     ((QState)Q_RET_HANDLED)

/** \brief Macro to call in a QM state-handler when when it attempts to
* handle an event but a guard condition evaluates to false and there is
* no other explicit way of handling the event. Applicable only to QMSMs.
*/
#define QM_UNHANDLED()   ((QState)Q_RET_UNHANDLED)
/** \brief Macro to call in a QM state-handler when it designates the
* superstate to handle an event. Applicable only to QMSMs.
*/
#define QM_SUPER()       ((QState)Q_RET_SUPER)


/** \brief Macro to call in a state-handler when it executes a regular
* or and initial transition. Applicable to both HSMs and FSMs.
* \include qep_qtran.c
*/
#define Q_TRAN(target_)  \
    (((QMsm *)me)->temp.fun = Q_STATE_CAST(target_), (QState)Q_RET_TRAN)

/** \brief Macro to call in a state-handler when it designates the
* superstate of a given state. Applicable only to HSMs.
* \include qep_qtran.c
*/
#define Q_SUPER(super_)  \
    (((QMsm *)me)->temp.fun = Q_STATE_CAST(super_),  (QState)Q_RET_SUPER)

/** \brief Macro to call in a state-handler when it handles an event.
*  Applicable to both HSMs and FSMs.
*/
#define Q_HANDLED()      ((QState)Q_RET_HANDLED)

/** \brief Macro to call in a state-handler when it attempts to handle
* an event but a guard condition evaluates to false and there is no other
* explicit way of handling the event. Applicable only to HSMs.
*/
#define Q_UNHANDLED()    ((QState)Q_RET_UNHANDLED)

/** \brief Macro to call in a non-hierarchical state-handler when it
* ignores (does not handle) an event. Applicable only to FSMs.
*/
#define Q_IGNORED()      ((QState)Q_RET_IGNORED)


/** \brief QEP reserved signals */
enum QReservedSignals {
    Q_ENTRY_SIG = 1,                   /**< signal for coding entry actions */
    Q_EXIT_SIG,                         /**< signal for coding exit actions */
    Q_INIT_SIG,                  /**< signal for coding initial transitions */
    Q_USER_SIG      /**< first signal that can be used in user applications */
};

#endif                                                             /* qep_h */

