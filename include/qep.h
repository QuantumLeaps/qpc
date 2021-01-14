/**
* @file
* @brief Public QEP/C interface
* @ingroup qep
* @cond
******************************************************************************
* Last updated for version 6.9.2
* Last updated on  2020-12-16
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
******************************************************************************
* @endcond
*/
#ifndef QEP_H
#define QEP_H

/****************************************************************************/
/*! The current QP version as a decimal constant XXYZ, where XX is a 2-digit
* major version number, Y is a 1-digit minor version number, and Z is
* a 1-digit release number.
*/
#define QP_VERSION      692U

/*! The current QP version number string of the form XX.Y.Z, where XX is
* a 2-digit major version number, Y is a 1-digit minor version number,
* and Z is a 1-digit release number.
*/
#define QP_VERSION_STR  "6.9.2"

/*! Encrypted  current QP release (6.9.2) and date (2021-01-18) */
#define QP_RELEASE      0x82C286EBU


/****************************************************************************/
/* typedefs for basic numerical types; MISRA-C 2004 rule 6.3(req). */

/*! typedef for character strings. */
/**
* @description
* This typedef specifies character type for exclusive use in character
* strings. Use of this type, rather than plain 'char', is in compliance
* with the MISRA-C 2004 Rules 6.1(req), 6.3(adv).
*/
typedef char char_t;

/*! typedef for line numbers in assertions and return from QF_run() */
typedef int int_t;

/*! typedef for unsigned int promotions in expressions */
typedef unsigned uint_t;

/*! typedef for enumerations used for event signals */
typedef int enum_t;

/*! IEEE 754 32-bit floating point number, MISRA-C 2004 rule 6.3(req) */
/**
* @note QP does not use floating-point types anywhere in the internal
* implementation, except in QS software tracing, where utilities for
* output of floating-point numbers are provided for application-level
* trace records.
*/
typedef float float32_t;

/*! IEEE 754 64-bit floating point number, MISRA-C 2004 rule 6.3(req) */
/**
* @note QP does not use floating-point types anywhere in the internal
* implementation, except in QS software tracing, where utilities for
* output of floating-point numbers are provided for application-level
* trace records.
*/
typedef double float64_t;

/*! the current QP version number string in ROM, based on QP_VERSION_STR */
extern char_t const QP_versionStr[7];

/****************************************************************************/
#ifndef Q_SIGNAL_SIZE

    /*! The size (in bytes) of the signal of an event. Valid values:
    * 1U, 2U, or 4U; default 2U */
    /**
    * @description
    * This macro can be defined in the QEP port file (qep_port.h) to
    * configure the ::QSignal type. When the macro is not defined, the
    * default of 2 bytes is applied.
    */
    #define Q_SIGNAL_SIZE 2U
#endif
#if (Q_SIGNAL_SIZE == 1U)
    typedef uint8_t QSignal;
#elif (Q_SIGNAL_SIZE == 2U)
    /*! QSignal represents the signal of an event. */
    /**
    * @description
    * The relationship between an event and a signal is as follows. A signal
    * in UML is the specification of an asynchronous stimulus that triggers
    * reactions, and as such is an essential part of an event.
    * (The signal conveys the type of the occurrence-what happened?)
    * However, an event can also contain additional quantitative information
    * about the occurrence in form of event parameters.
    */
    typedef uint16_t QSignal;
#elif (Q_SIGNAL_SIZE == 4U)
    typedef uint32_t QSignal;
#else
    #error "Q_SIGNAL_SIZE defined incorrectly, expected 1U, 2U, or 4U"
#endif


/****************************************************************************/
/*! Event class */
/**
* @description
* QEvt represents events without parameters and serves as the base structure
* for derivation of events with parameters.
*
* @usage
* The following example illustrates how to add an event parameter by
* derivation of the QEvt structure. Please note that the QEvt member
* super is defined as the FIRST member of the derived struct.
* @include qep_qevt.c
*
* @sa @ref oop
*/
typedef struct {
    QSignal sig;              /*!< signal of the event instance */
    uint8_t poolId_;          /*!< pool ID (0 for static event) */
    uint8_t volatile refCtr_; /*!< reference counter */
} QEvt;

#ifdef Q_EVT_CTOR /* Shall the constructor for the QEvt class be provided? */

/*! Custom event constructor
* @public @memberof QEvt
*/
QEvt *QEvt_ctor(QEvt * const me, enum_t const sig);

#endif

/****************************************************************************/
/*! Perform upcast from a subclass of ::QHsm to the base class ::QHsm */
/**
* @description
* Upcasting from a subclass to superclass is a very frequent and __safe__
* operation in object-oriented programming and object-oriented languages
* (such as C++) perform such upcasting automatically. However, OOP is
* implemented in C just as a set of coding conventions (see @ref oop),
* and the C compiler does not "know" that certain types are related by
* inheritance. Therefore for C, the upcast must be performed explicitly.
* Unfortunately, pointer casting violates the advisory MISRA-C 2004 rule 11.4
* "cast pointer to pointer". This macro encapsulates this deviation and
* provides a descriptive name for the reason of this cast.
*/
#define Q_HSM_UPCAST(ptr_) ((QHsm *)(ptr_))

/*! Perform downcast of an event onto a subclass of QEvt @p class_ */
/**
* @description
* QEvt represents events without parameters and serves as the base structure
* This macro encapsulates the downcast of QEvt pointers, which violates
* MISRA-C 2004 rule 11.4(advisory). This macro helps to localize this
* deviation.
*/
#define Q_EVT_CAST(class_) ((class_ const *)e)

/*! Perform cast from unsigned integer pointer @p uintptr_ to pointer
* of type @p type_. */
/**
* @description
* This macro encapsulates the cast to (type_ *), which QP ports or
* application might use to access embedded hardware registers.
*/
#define Q_UINT2PTR_CAST(type_, uintptr_)  ((type_ *)(uintptr_))


/****************************************************************************/
/*! typedef of the return type from a state/action-handler function. */
typedef uint_fast8_t QState;

/* forward declaration */
typedef struct QXThread QXThread;

/*! Pointer to a state-handler function. */
typedef QState (*QStateHandler)(void * const me, QEvt const * const e);

/*! Pointer to an action-handler function. */
typedef QState (*QActionHandler)(void * const me);

/*! Pointer to a thread-handler function. */
typedef void (*QXThreadHandler)(QXThread * const me);

/*! Perform cast to ::QStateHandler. */
/**
* @description
* This macro encapsulates the cast of a specific state handler function
* pointer to ::QStateHandler, which violates MISRA:C-2012 rule 11.1(req).
* This macro helps to localize this deviation.
*
* @usage
* @include qep_qhsm_ctor.c
*/
#define Q_STATE_CAST(handler_)  ((QStateHandler)(handler_))

/*! Perform cast to QActionHandler. */
/**
* @description
* This macro encapsulates the cast of a specific action handler function
* pointer to ::QActionHandler, which violates MISRA:C-2012 rule 11.1(req).
* This macro helps to localize this deviation.
*/
#define Q_ACTION_CAST(action_)  ((QActionHandler)(action_))

/* forward declarations... */
struct QMState;
struct QHsmVtable;
typedef struct QMTranActTable QMTranActTable;

/*! Attribute of for the ::QHsm class (Hierarchical State Machine). */
/**
* @description
* This union represents possible values stored in the 'state' and 'temp'
* attributes of the ::QHsm class.
*/
union QHsmAttr {
    QStateHandler   fun;         /*!< pointer to a state-handler function */
    QActionHandler  act;         /*!< pointer to an action-handler function */
    QXThreadHandler thr;         /*!< pointer to an thread-handler function */
    struct QMState const *obj;   /*!< pointer to QMState object */
    QMTranActTable const *tatbl; /*!< transition-action table */
};

/****************************************************************************/
/*! Hierarchical State Machine class */
/**
* @description
* ::QHsm represents a Hierarchical State Machine (HSM) with full support for
* hierarchical nesting of states, entry/exit actions, initial transitions,
* and transitions to history in any composite state. This class is designed
* for ease of manual coding of HSMs in C, but it is also supported by the
* QM modeling tool.
* @n
* ::QHsm is also the base class for the ::QMsm state machine, which provides
* a superior efficiency, but requires the use of the QM modeling tool to
* generate code.
*
* @note ::QHsm is not intended to be instantiated directly, but rather serves
* as the base structure for derivation of state machines in the application
* code.
*
* @usage
* The following example illustrates how to derive a state machine structure
* from QHsm. Please note that the QHsm member super is defined as the FIRST
* member of the derived struct.
* @include qep_qhsm.c
*/
typedef struct {
    struct QHsmVtable const *vptr; /*!< virtual pointer */
    union QHsmAttr state; /*!< current active state (state-variable) */
    union QHsmAttr temp;  /*!< temporary: tran. chain, target state, etc. */
} QHsm;

/*! Virtual table for the ::QHsm class. */
struct QHsmVtable {
#ifdef Q_SPY
    /*! Triggers the top-most initial transition in the HSM. */
    void (*init)(QHsm * const me, void const * const e,
                 uint_fast8_t const qs_id);

    /*! Dispatches an event to the HSM. */
    void (*dispatch)(QHsm * const me, QEvt const * const e,
                     uint_fast8_t const qs_id);

    /*! Get the current state handler of the HSM. */
    QStateHandler (*getStateHandler)(QHsm * const me);
#else
    void (*init)(QHsm * const me, void const * const e);
    void (*dispatch)(QHsm * const me, QEvt const * const e);
#endif /* Q_SPY */

};

/* QHsm public operations... */

#ifdef Q_SPY
    /*! Polymorphically executes the top-most initial transition in a HSM
    * @public @memberof QHsm
    */
    /**
    * @param[in,out] me_ pointer (see @ref oop)
    * @param[in]     e_  constant pointer the ::QEvt or a class derived from
    *                    ::QEvt (see @ref oop)
    * @note Must be called only ONCE after the SM "constructor".
    *
    * @usage
    * The following example illustrates how to initialize a SM, and dispatch
    * events to it:
    * @include qep_qhsm_use.c
    */
    #define QHSM_INIT(me_, par_, qs_id_) do {          \
        Q_ASSERT((me_)->vptr);                         \
        (*(me_)->vptr->init)((me_), (par_), (qs_id_)); \
    } while (false)

    /*! Implementation of the top-most initial transition in ::QHsm subclass */
    void QHsm_init_(QHsm * const me, void const * const e,
                    uint_fast8_t const qs_id);
#else

    #define QHSM_INIT(me_, par_, dummy) do { \
        Q_ASSERT((me_)->vptr);               \
        (*(me_)->vptr->init)((me_), (par_)); \
    } while (false)

    /*! Implementation of the top-most initial transition in ::QHsm subclass */
    void QHsm_init_(QHsm * const me, void const * const e);

#endif /* Q_SPY */


#ifdef Q_SPY
    /*! Polymorphically dispatches an event to a HSM
    * @public @memberof QHsm
    */
    /**
    * @description
    * Processes one event at a time in Run-to-Completion fashion.
    *
    * @param[in,out] me_ pointer (see @ref oop)
    * @param[in]     e_  constant pointer the ::QEvt or a structure derived from
    *                     ::QEvt (see @ref oop)
    * @note Must be called after the "constructor" and after QHSM_INIT().
    */
    #define QHSM_DISPATCH(me_, e_, qs_id_) \
        ((*(me_)->vptr->dispatch)((me_), (e_), (qs_id_)))

    /*! Implementation of dispatching events to a ::QHsm subclass */
    void QHsm_dispatch_(QHsm * const me, QEvt const * const e,
                        uint_fast8_t const qs_id);

    /*! Implementation of getting the state handler in a ::QHsm subclass */
    QStateHandler QHsm_getStateHandler_(QHsm * const me);
#else

    #define QHSM_DISPATCH(me_, e_, dummy) \
        ((*(me_)->vptr->dispatch)((me_), (e_)))

    /*! Implementation of dispatching events to a ::QHsm subclass */
    void QHsm_dispatch_(QHsm * const me, QEvt const * const e);

#endif /* Q_SPY */

/*! Obtain the current active state from a HSM (read only).
* @public @memberof QHsm
*/
/**
* @param[in] me_ pointer (see @ref oop)
* @returns the current active state of a ::QHsm subclass
* @note this macro is used in QM for auto-generating code for state history
*/
#define QHsm_state(me_) (Q_STATE_CAST(Q_HSM_UPCAST(me_)->state.fun))

/*! Obtain the current active child state of a given parent in ::QHsm
* @public @memberof QHsm
*/
/**
* @param[in] me_     pointer (see @ref oop)
* @param[in] parent_ pointer to the parent state-handler
* @returns the current active child state-handler of a given parent
* @note this macro is used in QM for auto-generating code for state history
*/
#define QHsm_childState(me_, parent_) \
    QHsm_childState_(Q_HSM_UPCAST(me_), Q_STATE_CAST(parent_))

/*! Tests if a given state is part of the current active state
* configuration in ::QHsm subclasses.
* @public @memberof QHsm
*/
bool QHsm_isIn(QHsm * const me, QStateHandler const state);

/* QHsm protected operations... */
/*! Protected "constructor" of ::QHsm
* @protected @memberof QHsm
*/
void QHsm_ctor(QHsm * const me, QStateHandler initial);

/*! the top-state.
* @protected @memberof QHsm
*/
QState QHsm_top(void const * const me, QEvt const * const e);

/* QHsm private operations... */
/*! Helper function to obtain the current active child state of a parent
* @private @memberof QHsm
*/
QStateHandler QHsm_childState_(QHsm * const me,
                               QStateHandler const parent);


/****************************************************************************/
/*! QM State Machine implementation strategy
* @extends QHsm
*/
/**
* @description
* ::QMsm (QM State Machine) provides a more efficient state machine
* implementation strategy than ::QHsm, but requires the use of the QM
* modeling tool, but are the fastest and need the least run-time
* support (the smallest event-processor taking up the least code space).
*
* @note
* ::QMsm is not intended to be instantiated directly, but rather serves as
* the base structure for derivation of state machines in the application code.
*
* @usage
* The following example illustrates how to derive a state machine structure
* from ::QMsm. Please note that the ::QMsm member 'super' is defined as the
* _first_ member of the derived struct.
* @include qep_qmsm.c
*
* @sa @ref oop
*/
typedef struct {
    QHsm super; /*!< inherits ::QHsm */
} QMsm;

/*! State object for the ::QMsm class (QM State Machine). */
/**
* @description
* This class groups together the attributes of a ::QMsm state, such as the
* parent state (state nesting), the associated state handler function and
* the exit action handler function. These attributes are used inside the
* QMsm_dispatch() and QMsm_init() functions.
*
* @attention
* The ::QMState class is only intended for the QM code generator and should
* not be used in hand-crafted code.
*/
struct QMState {
    struct QMState const *superstate;   /*!< superstate of this state */
    QStateHandler  const stateHandler;  /*!< state handler function */
    QActionHandler const entryAction;   /*!< entry action handler function */
    QActionHandler const exitAction;    /*!< exit action handler function */
    QActionHandler const initAction;    /*!< init action handler function */
};
typedef struct QMState QMState;

/*! Transition-Action Table for the Meta State Machine. */
struct QMTranActTable {
    struct QMState const *target;
    QActionHandler const act[1];
};

/* QMsm public operations... */
/*! Obtain the current active state from a MSM (read only)
* @public @memberof QMsm
*/
/**
* @param[in] me_     pointer (see @ref oop)
* @returns the current active state-object
* @note this macro is used in QM for auto-generating code for state history
*/
#define QMsm_stateObj(me_) (Q_HSM_UPCAST(me_)->state.obj)

/*! Obtain the current active child state of a given parent in ::QMsm
* @public @memberof QMsm
*/
/**
* @param[in] me_     pointer (see @ref oop)
* @param[in] parent_ pointer to the parent state-object
* @returns the current active child state-object of a given parent
* @note this macro is used in QM for auto-generating code for state history
*/
#define QMsm_childStateObj(me_, parent_) \
    QMsm_childStateObj_(Q_HSM_UPCAST(me_), (parent_))

/*! Helper function to obtain the current active child state of a parent
* @public @memberof QMsm
*/
QMState const *QMsm_childStateObj_(QMsm const * const me,
                                   QMState const * const parent);

/*! Tests if a given state is part of the current active state
* configuration in a MSM.
* @public @memberof QMsm
*/
bool QMsm_isInState(QMsm const * const me, QMState const * const state);

/* QMsm protected operations... */
/*! Constructor of ::QMsm
* @protected @memberof QMsm
*/
void QMsm_ctor(QMsm * const me, QStateHandler initial);

/* QMsm private operations... */
/*! Implementation of the top-most initial transition in ::QMsm
* @private @memberof QMsm
*/
#ifdef Q_SPY
void QMsm_init_(QHsm * const me, void const * const e,
                uint_fast8_t const qs_id);
#else
void QMsm_init_(QHsm * const me, void const * const e);
#endif

/*! Implementation of disparching events to ::QMsm
* @private @memberof QMsm
*/
#ifdef Q_SPY
void QMsm_dispatch_(QHsm * const me, QEvt const * const e,
                    uint_fast8_t const qs_id);
#else
void QMsm_dispatch_(QHsm * const me, QEvt const * const e);
#endif

/*! Implementation of getting the state handler in a ::QMsm
* @private @memberof QMsm
*/
#ifdef Q_SPY
QStateHandler QMsm_getStateHandler_(QHsm * const me);
#endif

/*! Macro to call in a state-handler when it executes a regular
* or and initial transition. Applicable only to ::QHsm subclasses.
* @include qep_qtran.c
*/
#define Q_TRAN(target_)  \
    ((Q_HSM_UPCAST(me))->temp.fun = Q_STATE_CAST(target_), (QState)Q_RET_TRAN)

/*! Macro to call in a state-handler when it executes a transition
* to history. Applicable only to HSMs.
*
* @usage
* @include qep_qhist.c
*/
#define Q_TRAN_HIST(hist_)  \
    ((Q_HSM_UPCAST(me))->temp.fun = (hist_), (QState)Q_RET_TRAN_HIST)

/*! Macro to call in a state-handler when it designates the
* superstate of a given state. Applicable only to ::QHsm subclasses.
*
* @usage
* @include qep_qtran.c
*/
#define Q_SUPER(super_)  \
    ((Q_HSM_UPCAST(me))->temp.fun = Q_STATE_CAST(super_), (QState)Q_RET_SUPER)

/*! Macro to call in a state-handler when it handles an event.
*  Applicable to both HSMs and FSMs.
*/
#define Q_HANDLED()      ((QState)Q_RET_HANDLED)

/*! Macro to call in a state-handler when it attempts to handle
* an event but a guard condition evaluates to 'false' and there is no other
* explicit way of handling the event. Applicable only to ::QHsm subclasses.
*/
#define Q_UNHANDLED()    ((QState)Q_RET_UNHANDLED)

/*! Macro to provide strictly-typed zero-action to terminate action lists
*! in the transition-action-tables
*/
#define Q_ACTION_NULL    ((QActionHandler)0)


/****************************************************************************/
/*! All possible values returned from state/action handlers */
/**
* @note The order matters for algorithmic correctness.
*/
enum {
    /* unhandled and need to "bubble up" */
    Q_RET_SUPER,     /*!< event passed to superstate to handle */
    Q_RET_SUPER_SUB, /*!< event passed to submachine superstate */
    Q_RET_UNHANDLED, /*!< event unhandled due to a guard */

    /* handled and do not need to "bubble up" */
    Q_RET_HANDLED,   /*!< event handled (internal transition) */
    Q_RET_IGNORED,   /*!< event silently ignored (bubbled up to top) */

    /* entry/exit */
    Q_RET_ENTRY,     /*!< state entry action executed */
    Q_RET_EXIT,      /*!< state exit  action executed */

    /* no side effects */
    Q_RET_NULL,      /*!< return value without any effect */

    /* transitions need to execute transition-action table in ::QMsm */
    Q_RET_TRAN,      /*!< regular transition */
    Q_RET_TRAN_INIT, /*!< initial transition in a state or submachine */
    Q_RET_TRAN_EP,   /*!< entry-point transition into a submachine */

    /* transitions that additionally clobber me->state */
    Q_RET_TRAN_HIST, /*!< transition to history of a given state */
    Q_RET_TRAN_XP    /*!< exit-point transition out of a submachine */
};

#ifdef Q_SPY
    /*! Macro to call in a QM action-handler when it executes
    * an entry action. Applicable only to ::QMsm subclasses.
    */
    #define QM_ENTRY(state_) \
        ((Q_HSM_UPCAST(me))->temp.obj = (state_), (QState)Q_RET_ENTRY)

    /*! Macro to call in a QM action-handler when it executes
    * an exit action. Applicable only to ::QMsm subclasses.
    */
    #define QM_EXIT(state_) \
        ((Q_HSM_UPCAST(me))->temp.obj = (state_), (QState)Q_RET_EXIT)

#else
    #define QM_ENTRY(dummy) ((QState)Q_RET_ENTRY)

    #define QM_EXIT(dummy)  ((QState)Q_RET_EXIT)

#endif /* Q_SPY */

/*! Macro to call in a QM submachine exit-handler.
* Applicable only to subclasses of ::QMsm.
*/
#define QM_SM_EXIT(state_) \
    ((Q_HSM_UPCAST(me))->temp.obj = (state_), (QState)Q_RET_EXIT)

/*! Macro to call in a QM state-handler when it executes a regular
* transition. Applicable only to ::QMsm subclasses.
*/
#define QM_TRAN(tatbl_) ((Q_HSM_UPCAST(me))->temp.tatbl \
      = (QMTranActTable *)(tatbl_), (QState)Q_RET_TRAN)

/*! Macro to call in a QM state-handler when it executes an initial
* transition. Applicable only to ::QMsm subclasses.
*/
#define QM_TRAN_INIT(tatbl_) ((Q_HSM_UPCAST(me))->temp.tatbl \
    = (QMTranActTable *)(tatbl_), (QState)Q_RET_TRAN_INIT)

/*! Macro to call in a QM state-handler when it executes a transition
* to history. Applicable only to ::QMsm subclasses.
*/
#define QM_TRAN_HIST(history_, tatbl_)                                 \
    ((((Q_HSM_UPCAST(me))->state.obj = (history_)),                    \
       ((Q_HSM_UPCAST(me))->temp.tatbl = (QMTranActTable *)(tatbl_))), \
     (QState)Q_RET_TRAN_HIST)

/*! Macro to call in a QM state-handler when it executes a transition
* to the submachine via an entry point.
*/
#define QM_TRAN_EP(tatbl_) ((Q_HSM_UPCAST(me))->temp.tatbl \
    = (struct QMTranActTable *)(tatbl_), (QState)Q_RET_TRAN_EP)

/*! Macro to call in a QM state-handler when it executes a transition
* to exit point. Applicable only to ::QMsm subclasses.
*/
#define QM_TRAN_XP(xp_, tatbl_)                                        \
    ((((Q_HSM_UPCAST(me))->state.act = (xp_)),                         \
       ((Q_HSM_UPCAST(me))->temp.tatbl = (QMTranActTable *)(tatbl_))), \
     (QState)Q_RET_TRAN_XP)

/*! Macro to call in a QM state-handler when it handled an event.
* Applicable only to ::QMsm subclasses.
*/
#define QM_HANDLED()     ((QState)Q_RET_HANDLED)

/*! Macro to call in a QM state-handler when when it attempts to
* handle an event but a guard condition evaluates to 'false' and there is
* no other explicit way of handling the event. Applicable only to
* ::QMsm subclasses.
*/
#define QM_UNHANDLED()   ((QState)Q_RET_UNHANDLED)

/*! Macro to call in a QM state-handler when it designates the
* superstate to handle an event. Applicable only to QMSMs.
*/
#define QM_SUPER()       ((QState)Q_RET_SUPER)

/*! Macro to call in a QM submachine-handler when it designates the
* host state to handle an event. Applicable only to QMSMs.
*/
#define QM_SUPER_SUB(host_) \
    ((Q_HSM_UPCAST(me))->temp.obj = (host_), (QState)Q_RET_SUPER_SUB)

/*! Macro to provide strictly-typed zero-state to use for submachines.
*! Applicable to suclasses of QP::QMsm.
*/
#define QM_STATE_NULL    ((QMState *)0)

/*! QEP reserved signals */
enum {
    Q_ENTRY_SIG = 1,  /*!< signal for coding entry actions */
    Q_EXIT_SIG,       /*!< signal for coding exit actions */
    Q_INIT_SIG,       /*!< signal for coding initial transitions */
    Q_USER_SIG        /*!< first signal that can be used for user signals */
};

#endif /* QEP_H */

