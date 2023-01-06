/**
* @file
* @brief Various macros for configuring QP/C (typically used as
* command-line options)
*/

/*! The preprocessor switch to disable checking assertions
*
* @description
* When defined, Q_NASSERT disables the following macros #Q_ASSERT,
* #Q_REQUIRE, #Q_ENSURE, #Q_INVARIANT, #Q_ERROR as well as
* #Q_ASSERT_ID, #Q_REQUIRE_ID, #Q_ENSURE_ID, #Q_INVARIANT_ID, and
* #Q_ERROR_ID do _not_ evaluate the test condition passed as the
* argument to these macros.
*
* @note The notable exceptions are the macros #Q_ALLEGE and
* #Q_ALLEGE_ID, that still evaluate the test condition, but do not
* report assertion failures when the switch #Q_NASSERT is defined.
*/
#define Q_NASSERT

/*! Enable the QActive_stop() API in the QF port.
*
* @description
* Defining this macro enables the QActive_stop() API in a given port.
* This feature should be used with caution, as stopping and re-starting
* active objects **cleanly** can be tricky.
*/
#define QF_ACTIVE_STOP

/*! The preprocessor switch to activate the QS software tracing
* instrumentation in the code
*
* @description
* When defined, Q_SPY activates the QS software tracing instrumentation.
* When Q_SPY is not defined, the QS instrumentation in the code does
* not generate any code.
*/
#define Q_SPY

/*! The preprocessor switch to activate the QUTest unit testing
* instrumentation in the code
*
* @note
* This macro requires that #Q_SPY be defined as well.
*/
#define Q_UTEST

/*! The preprocessor switch to enable constructor in the ::QEvt class
* instrumentation in the code
*
* @tr{RQP005}
*/
#define Q_EVT_CTOR

/*! This macro enables calling the context-switch callback
* QF_onContextSw() in all build-in kernels (QV, QK, QXK)
*/
#define QF_ON_CONTEXT_SW

/*! Macro defined only for the internal QP implementation. It should
* be not defined for the application-level code
*/
#define QP_IMPL

