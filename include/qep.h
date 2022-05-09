/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2021-12-23
* @version Last updated for: @ref qpc_7_0_0
*
* @file
* @brief Public QEP/C interface
*
* @tr{RQP001} @tr{RQP101}
*/
#ifndef QEP_H
#define QEP_H

/*==========================================================================*/
/*! The current QP version as a decimal constant XXYZ, where XX is a 2-digit
* major version number, Y is a 1-digit minor version number, and Z is
* a 1-digit release number.
*/
#define QP_VERSION      700U

/*! The current QP version number string of the form XX.Y.Z, where XX is
* a 2-digit major version number, Y is a 1-digit minor version number,
* and Z is a 1-digit release number.
*/
#define QP_VERSION_STR  "7.0.0"

/*! Encrypted  current QP release (7.0.0) and date (2022-01-31) */
#define QP_RELEASE      0x7CCAAA13U

/*==========================================================================*/
/* typedefs for basic numerical types; MISRA-C 2012 Dir 4.6(A). */

/*! typedef for line numbers in assertions and return from QF_run() */
typedef signed int int_t;

/*! typedef for enumerations used for event signals */
typedef signed int enum_t;

/*! IEEE 754 32-bit floating point number, MISRA-C 2012 Dir 4.6(A)
* @note QP does not use floating-point types anywhere in the internal
* implementation, except in QS software tracing, where utilities for
* output of floating-point numbers are provided for application-specific
* trace records.
*/
typedef float float32_t;

/*! IEEE 754 64-bit floating point number, MISRA-C 2012 Dir 4.6(A)
* @note QP does not use floating-point types anywhere in the internal
* implementation, except in QS software tracing, where utilities for
* output of floating-point numbers are provided for application-specific
* trace records.
*/
typedef double float64_t;

/*! the current QP version number string in ROM, based on #QP_VERSION_STR */
extern char const QP_versionStr[7];

/*==========================================================================*/
#ifndef Q_SIGNAL_SIZE
    #define Q_SIGNAL_SIZE 2U
#endif
#if (Q_SIGNAL_SIZE == 1U)
    typedef uint8_t QSignal;
#elif (Q_SIGNAL_SIZE == 2U)
    /*! The size (in bytes) of the signal of an event. Valid values:
    * 1U, 2U, or 4U; default 2U
    * @details
    * This macro can be defined in the QEP port file (qep_port.h) to
    * configure the ::QSignal type. When the macro is not defined, the
    * default of 2 bytes is applied.
    *
    * @tr{RQP002A}
    */
    typedef uint16_t QSignal;
#elif (Q_SIGNAL_SIZE == 4U)
    typedef uint32_t QSignal;
#else
    #error "Q_SIGNAL_SIZE defined incorrectly, expected 1U, 2U, or 4U"
#endif

/*==========================================================================*/
/*! Event class
* @details
* ::QEvt represents events without parameters and serves as the base class
* for derivation of events with parameters.
*
* @tr{RQP001} @tr{RQP004}
* @tr{AQP210}
*
* @usage
* The following example illustrates how to add an event parameter by
* derivation of the ::QEvt class. Please note that the ::QEvt member
* super is defined as the FIRST member of the derived struct.
* @include qep_qevt.c
*/
typedef struct {
    /*! @public signal of the event instance
    * @tr{RQP002}
    */
    QSignal sig;

    /*! @private pool ID (0 for immutable event)
    *  @tr{RQP003}
    */
    uint8_t poolId_;

    /*! @private reference counter
    *  @tr{RQP003}
    */
    uint8_t volatile refCtr_;
} QEvt;

#ifdef Q_EVT_CTOR /* Shall the constructor for the ::QEvt class be provided? */

/*! Custom event constructor
* @public @memberof ::QEvt
* @tr{RQP005}
*/
QEvt *QEvt_ctor(QEvt * const me, enum_t const sig);

#endif

/*==========================================================================*/
/*! Perform downcast of an event onto a subclass of ::QEvt @p class_
* @details
* ::QEvt represents events without parameters and serves as the base class
* This macro encapsulates the downcast of ::QEvt pointers, which violates
* MISRA-C 2012 Rule 11.3(R) "A cast shall not be performed between a
* pointer to object type and a pointer to a different object type". This
* macro helps to localize this deviation.
*
* @tr{RQP003}
* @tr{PQA11_3}
*/
#define Q_EVT_CAST(class_) ((class_ const *)e)

/*! Perform cast from unsigned integer pointer @p uintptr_ to pointer
* of type @p type_.
* @details
* This macro encapsulates the cast to (type_ *), which QP ports or
* application might use to access embedded hardware registers.
*/
#define Q_UINT2PTR_CAST(type_, uintptr_)  ((type_ *)(uintptr_))

/*==========================================================================*/
/*! typedef of the return type from a state/action-handler functions. */
typedef uint_fast8_t QState;

/* forward declaration */
typedef struct QXThread QXThread;

/*! Pointer to a state-handler function. */
typedef QState (*QStateHandler)(void * const me, QEvt const * const e);

/*! Pointer to an action-handler function. */
typedef QState (*QActionHandler)(void * const me);

/*! Pointer to a thread-handler function. */
typedef void (*QXThreadHandler)(QXThread * const me);

/*! Perform cast to ::QStateHandler.
* @details
* This macro encapsulates the cast of a specific state handler function
* pointer to ::QStateHandler, which violates MISRA:C-2012 Rule 11.1(req)
* "Conversions shall not be performed between a pointer to function and
* any other type". This macro helps to localize this deviation.
*
* @tr{PQP11_1} @tr{PQA11_1}
*
* @usage
* @include qep_qhsm_ctor.c
*/
#define Q_STATE_CAST(handler_)  ((QStateHandler)(handler_))

/*! Perform cast to ::QActionHandler.
* @details
* This macro encapsulates the cast of a specific action handler function
* pointer to ::QActionHandler, which violates MISRA:C-2012 Rule 11.1(R)
* "Conversions shall not be performed between a pointer to function and
* any other type". This macro helps to localize this deviation.
*
* @tr{PQP11_1} @tr{PQA11_1}
*/
#define Q_ACTION_CAST(action_)  ((QActionHandler)(action_))

/* forward declarations... */
struct QMState;
struct QHsmVtable;
typedef struct QMTranActTable QMTranActTable;

/*! Attribute of for the ::QHsm class (Hierarchical State Machine).
* @details
* This union represents possible values stored in the 'state' and 'temp'
* attributes of the ::QHsm class.
*/
union QHsmAttr {
    QStateHandler   fun;         /*!< @private pointer to a state-handler */
    QActionHandler  act;         /*!< @private pointer to an action-handler */
    QXThreadHandler thr;         /*!< @private pointer to an thread-handler */
    struct QMState const *obj;   /*!< @private pointer to QMState object */
    QMTranActTable const *tatbl; /*!< @private transition-action table */
};

/*==========================================================================*/
/*! Hierarchical State Machine class
* @details
* ::QHsm represents a Hierarchical State Machine (HSM) with full support for
* hierarchical nesting of states, entry/exit actions, initial transitions,
* and transitions to history in any composite state. This class is designed
* for ease of manual coding of HSMs in C, but it is also supported by the
* QM modeling tool.
* <br>
* ::QHsm is also the base class for the ::QMsm state machine, which provides
* a superior efficiency, but requires the use of the QM modeling tool to
* generate code.
*
* @note ::QHsm is not intended to be instantiated directly, but rather serves
* as the base class for derivation of state machines in the application
* code.
*
* @tr{RQP103}
* @tr{AQP211}
*
* @usage
* The following example illustrates how to derive a state machine class
* from ::QHsm. Please note that the ::QHsm member `super` is defined as the
* FIRST member of the derived class.
* @include qep_qhsm.c
*/
typedef struct {
    struct QHsmVtable const *vptr; /*!< @private virtual pointer @tr{RQP102} */
    union QHsmAttr state; /*!< @private current active state (state-var) */
    union QHsmAttr temp;  /*!< @private temporary: target/act-table, etc. */
} QHsm;

/*! Virtual table for the ::QHsm class. */
/*! @tr{RQP102} */
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
    * @param[in,out] me_ pointer (see @ref oop)
    * @param[in]     e_  constant pointer the ::QEvt or a class derived from
    *                    ::QEvt (see @ref oop)
    * @param[in]     qs_id_ QS local filter ID
    *
    * @note Must be called only ONCE after the SM "constructor".
    *
    * @tr{RQP102}
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

    /*! Implementation of the top-most initial tran. in ::QHsm subclass.
    * @private @memberof QHsm
    */
    void QHsm_init_(QHsm * const me, void const * const e,
                    uint_fast8_t const qs_id);
#else

    #define QHSM_INIT(me_, par_, dummy) do { \
        Q_ASSERT((me_)->vptr);               \
        (*(me_)->vptr->init)((me_), (par_)); \
    } while (false)

    /*! Implementation of the top-most initial tran. in ::QHsm subclass */
    void QHsm_init_(QHsm * const me, void const * const e);

#endif /* Q_SPY */

#ifdef Q_SPY
    /*! Polymorphically dispatches an event to a HSM
    * @details
    * Processes one event at a time in Run-to-Completion fashion.
    *
    * @param[in,out] me_ pointer (see @ref oop)
    * @param[in]     e_  constant pointer the ::QEvt or a class
    *                    derived from ::QEvt (see @ref oop)
    * @note Must be called after the "constructor" and after QHSM_INIT().
    *
    * @tr{RQP102}
    */
    #define QHSM_DISPATCH(me_, e_, qs_id_) \
        ((*(me_)->vptr->dispatch)((me_), (e_), (qs_id_)))

    /*! Implementation of dispatching events to a ::QHsm subclass
    * @private @memberof QHsm
    */
    void QHsm_dispatch_(QHsm * const me, QEvt const * const e,
                        uint_fast8_t const qs_id);

    /*! Implementation of getting the state handler in a ::QHsm subclass
    * @private @memberof QHsm
    */
    QStateHandler QHsm_getStateHandler_(QHsm * const me);
#else

    #define QHSM_DISPATCH(me_, e_, dummy) \
        ((*(me_)->vptr->dispatch)((me_), (e_)))

    void QHsm_dispatch_(QHsm * const me, QEvt const * const e);

#endif /* Q_SPY */

/*! Perform upcast from a subclass of ::QHsm to the base class ::QHsm
* @details
* Upcasting from a subclass to superclass is a very frequent and __safe__
* operation in object-oriented programming and object-oriented languages
* (such as C++) perform such upcasting automatically. However, OOP is
* implemented in C just as a set of coding conventions (see @ref oop),
* and the C compiler does not "know" that certain types are related by
* inheritance. Therefore for C, the upcast must be performed explicitly.
* Unfortunately, pointer casting violates the advisory MISRA-C 2012
* Rule 11.3(req) "A cast shall not be performed between a pointer to object
* type and a pointer to a different object type". This macro encapsulates
* this deviation and provides a descriptive name for the reason of this cast.
*/
#define Q_HSM_UPCAST(ptr_) ((QHsm *)(ptr_))

/*! Obtain the current active state from a HSM (read only).
* @public @memberof QHsm
* @param[in] me pointer (see @ref oop)
* @returns the current active state of a ::QHsm class
* @note this function is used in QM for auto-generating code for state history
*/
static inline QStateHandler QHsm_state(QHsm * const me) {
    return Q_STATE_CAST(me->state.fun);
}

/*! Obtain the current active child state of a given parent in ::QHsm
* @public @memberof QHsm
* @param[in] me     pointer (see @ref oop)
* @param[in] parent pointer to the parent state-handler
* @returns the current active child state-handler of a given parent
* @note this function is used in QM for auto-generating code for state history
*/
QStateHandler QHsm_childState(QHsm * const me,
                              QStateHandler const parent);

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

/*==========================================================================*/
/*! QM State Machine implementation strategy
* @extends QHsm
* @details
* ::QMsm (QM State Machine) provides a more efficient state machine
* implementation strategy than ::QHsm, but requires the use of the QM
* modeling tool, but are the fastest and need the least run-time
* support (the smallest event-processor taking up the least code space).
*
* @note
* ::QMsm is not intended to be instantiated directly, but rather serves as
* the base class for derivation of state machines in the application code.
*
* @tr{RQP104}
*
* @usage
* The following example illustrates how to derive a state machine class
* from ::QMsm. Please note that the ::QMsm member `super` is defined as the
* _first_ member of the derived struct.
* @include qep_qmsm.c
*/
typedef struct {
    QHsm super; /*!< @protected inherits ::QHsm */
} QMsm;

/*! State object for the ::QMsm class (QM State Machine).
* @details
* This class groups together the attributes of a ::QMsm state, such as the
* parent state (state nesting), the associated state handler function and
* the exit action handler function. These attributes are used inside the
* QMsm_dispatch() and QMsm_init() functions.
*
* @tr{RQP104}
*
* @attention
* The ::QMState class is only intended for the QM code generator and should
* not be used in hand-crafted code.
*/
struct QMState {
    struct QMState const *superstate;  /*!< @private superstate of the state */
    QStateHandler  const stateHandler; /*!< @private state handler */
    QActionHandler const entryAction;  /*!< @private entry action handler */
    QActionHandler const exitAction;   /*!< @private exit action handler */
    QActionHandler const initAction;   /*!< @private init action handler */
};
typedef struct QMState QMState;

/*! Transition-Action Table for the Meta State Machine. */
struct QMTranActTable {
    struct QMState const *target;      /*!< @private */
    QActionHandler const act[1];       /*!< @private */
};

/* QMsm public operations... */
/*! Obtain the current active state from a MSM (read only)
* @public @memberof QMsm
* @param[in] me     pointer (see @ref oop)
* @returns the current active state-object
* @note this function is used in QM for auto-generating code for state history
*/
static inline QMState const *QMsm_stateObj(QHsm * const me) {
    return me->state.obj;
}

/*! Obtain the current active child state of a given parent in ::QMsm
* @public @memberof QMsm
* @param[in] me     pointer (see @ref oop)
* @param[in] parent pointer to the parent state-object
* @returns the current active child state-object of a given parent
* @note this function is used in QM for auto-generating code for state history
*/
QMState const *QMsm_childStateObj(QHsm const * const me,
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
*
* @tr{RQP103} @tr{RQP120E}
*
* @usage
* @include qep_qtran.c
*/
#define Q_TRAN(target_)  \
    ((Q_HSM_UPCAST(me))->temp.fun = Q_STATE_CAST(target_), (QState)Q_RET_TRAN)

/*! Macro to call in a state-handler when it executes a transition
* to history. Applicable only to HSMs.
*
* @tr{RQP103} @tr{RQP120H}
*
* @usage
* @include qep_qhist.c
*/
#define Q_TRAN_HIST(hist_)  \
    ((Q_HSM_UPCAST(me))->temp.fun = (hist_), (QState)Q_RET_TRAN_HIST)

/*! Macro to call in a state-handler when it designates the
* superstate of a given state. Applicable only to ::QHsm subclasses.
*
* @tr{RQP103}
*
* @usage
* @include qep_qsuper.c
*/
#define Q_SUPER(super_)  \
    ((Q_HSM_UPCAST(me))->temp.fun = Q_STATE_CAST(super_), (QState)Q_RET_SUPER)

/*! Macro to call in a state-handler when it handles an event.
* @tr{RQP103} @tr{RQP120B} @tr{RQP120C}
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

/*==========================================================================*/
/*! All possible values returned from state/action handlers
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
      = (QMTranActTable const *)(tatbl_), (QState)Q_RET_TRAN)

/*! Macro to call in a QM state-handler when it executes an initial
* transition. Applicable only to ::QMsm subclasses.
*/
#define QM_TRAN_INIT(tatbl_) ((Q_HSM_UPCAST(me))->temp.tatbl \
    = (QMTranActTable const *)(tatbl_), (QState)Q_RET_TRAN_INIT)

/*! Macro to call in a QM state-handler when it executes a transition
* to history. Applicable only to ::QMsm subclasses.
*/
#define QM_TRAN_HIST(history_, tatbl_)                                      \
    ((((Q_HSM_UPCAST(me))->state.obj  = (history_)),                        \
      ((Q_HSM_UPCAST(me))->temp.tatbl = (QMTranActTable const *)(tatbl_))), \
     (QState)Q_RET_TRAN_HIST)

/*! Macro to call in a QM state-handler when it executes a transition
* to the submachine via an entry point.
*/
#define QM_TRAN_EP(tatbl_) ((Q_HSM_UPCAST(me))->temp.tatbl \
    = (QMTranActTable const *)(tatbl_), (QState)Q_RET_TRAN_EP)

/*! Macro to call in a QM state-handler when it executes a transition
* to exit point. Applicable only to ::QMsm subclasses.
*/
#define QM_TRAN_XP(xp_, tatbl_)                                             \
    ((((Q_HSM_UPCAST(me))->state.act  = (xp_)),                             \
      ((Q_HSM_UPCAST(me))->temp.tatbl = (QMTranActTable const *)(tatbl_))), \
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
*! Applicable to subclasses of QP::QMsm.
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
