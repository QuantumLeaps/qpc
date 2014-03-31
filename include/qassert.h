/**
* \file
* \brief customizable and memory-efficient assertions for embedded systems.
* \cond
******************************************************************************
* Product: QP/C/C++/nano
* Last updated for version 5.3.0
* Last updated on  2014-03-01
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* \endcond
*/
#ifndef qassert_h
#define qassert_h

/**
* \note
* This header file can be used in C, C++, and mixed C/C++ programs.
*
* \note The preprocessor switch #Q_NASSERT disables checking assertions.
* However, it is generally __not__ advisable to disable assertions,
* __especially__ in the production code. Instead, the assertion handler
* Q_onAssert() should be very carefully designed and tested.
*/

#ifdef Q_NASSERT /* Q_NASSERT defined--assertion checking disabled */

    #define Q_DEFINE_THIS_FILE
    #define Q_DEFINE_THIS_MODULE(name_)
    #define Q_ASSERT(test_)             ((void)0)
    #define Q_ASSERT_ID(id_, test_)     ((void)0)
    #define Q_ALLEGE(test_)             ((void)(test_))
    #define Q_ALLEGE_ID(id_, test_)     ((void)(test_))
    #define Q_ERROR()                   ((void)0)
    #define Q_ERROR_ID(id_)             ((void)0)

#else  /* Q_NASSERT not defined--assertion checking enabled */

    /*! Define the file name (with `__FILE__`) for assertions in this file. */
    /**
    * \description
    * Macro to be placed at the top of each C/C++ module to define the
    * single instance of the file name string to be used in reporting
    * assertions in this module.
    *
    * \note The file name string literal is defined by means of the standard
    * preprocessor macro `__FILE__`. However, please note that, depending
    * on the compiler, the `__FILE__` macro might contain the whole path name
    * to the file, which might be inconvenient to log assertions.
    * \note This macro should __not__ be terminated by a semicolon.
    * \sa Q_DEFINE_THIS_MODULE()
    */
    #define Q_DEFINE_THIS_FILE \
        static char_t const Q_ROM Q_this_module_[] = __FILE__;

    /*! Define the user-specified module name for assertions in this file. */
    /**
    * \description
    * Macro to be placed at the top of each C/C++ module to define the
    * single instance of the module name string to be used in reporting
    * assertions in this module. This macro takes the user-supplied argument
    * \a name_ instead of `__FILE__` to precisely control the name of the
    * module.
    *
    * \arguments
    * \arg[in] \c name_ string constant representing the module name
    *
    * \note This macro should __not__ be terminated by a semicolon.
    */
    #define Q_DEFINE_THIS_MODULE(name_) \
        static char_t const Q_ROM Q_this_module_[] = name_;

    /*! General purpose assertion. */
    /**
    * \description
    * Makes sure the \a test_ argument is TRUE. Calls the Q_onAssert()
    * callback if the \a test_ expression evaluates to FALSE. This
    * macro identifies the assertion location within the file by means
    * of the standard `__LINE__` macro.
    *
    * \arguments
    * \arg[in] \c test_ Boolean expression
    *
    * \note the \a test_ is __not__ evaluated if assertions are disabled
    * with the #Q_NASSERT switch.
    */
    #define Q_ASSERT(test_) ((test_) \
        ? (void)0 : Q_onAssert(&Q_this_module_[0], (int_t)__LINE__))

    /*! General purpose assertion with user-specified assertion-id. */
    /**
    * \description
    * Makes sure the \c test_ argument is TRUE. Calls the Q_onAssert()
    * callback if the \c test_ evaluates to FALSE. This assertion takes the
    * user-supplied argument \c id_ to identify the location of this
    * assertion within the file. This avoids the volatility of using line
    * numbers, which change whenever a line of code is added or removed
    * upstream from the assertion.
    *
    * \arguments
    * \arg[in] \c id_   ID number (unique within the module) of the assertion
    * \arg[in] \c test_ Boolean expression
    *
    * \note the \c test_ expression is __not__ evaluated if assertions are
    * disabled with the #Q_NASSERT switch.
    */
    #define Q_ASSERT_ID(id_, test_) ((test_) \
        ? (void)0 : Q_onAssert(&Q_this_module_[0], (int_t)(id_)))

    /*! General purpose assertion that __always__ evaluates the \a test_
    * expression. */
    /**
    * \description
    * Like the Q_ASSERT() macro, except it __always__ evaluates the \a test_
    * expression even when assertions are disabled with the #Q_NASSERT macro.
    * However, when the #Q_NASSERT macro is defined, the Q_onAssert()
    * callback is __not__ called, even if \a test_ evaluates to FALSE.
    *
    * \arguments
    * \arg[in] \c test_ Boolean expression (__always__ evaluated)
    *
    * \sa #Q_ALLEGE_ID
    */
    #define Q_ALLEGE(test_)    Q_ASSERT(test_)

    /*! General purpose assertion with user-specified assertion-id that
    * __always__ evaluates the \a test_ expression. */
    /**
    * \description
    * Like the Q_ASSERT_ID() macro, except it __always__ evaluates the
    * \a test_ expression even when assertions are disabled with the
    * #Q_NASSERT macro. However, when the #Q_NASSERT macro is defined, the
    * Q_onAssert() callback is __not__ called, even if \a test_ evaluates
    * to FALSE.
    *
    * \arguments
    * \arg[in] \c id_   ID number (unique within the module) of the assertion
    * \arg[in] \c test_ Boolean expression
    */
    #define Q_ALLEGE_ID(id_, test_) Q_ASSERT_ID(id_, test_)

    /*! Assertion for a wrong path through the code. */
    /**
    * \description
    * Calls the Q_onAssert() callback if ever executed.
    *
    * \note Does noting if assertions are disabled with the #Q_NASSERT switch.
    */
    #define Q_ERROR() \
        Q_onAssert(&Q_this_module_[0], (int_t)__LINE__)

    /*! Assertion with user-specified assertion-id for a wrong path. */
    /**
    * \description
    * Calls the Q_onAssert() callback if ever executed. This assertion
    * takes the user-supplied argument \a id_ to identify the location of
    * this assertion within the file. This avoids the volatility of using
    * line numbers, which change whenever a line of code is added or removed
    * upstream from the assertion.
    *
    * \arguments
    * \arg[in] \c id_   ID number (unique within the module) of the assertion
    *
    * \note Does noting if assertions are disabled with the #Q_NASSERT switch.
    */
    #define Q_ERROR_ID(id_) \
        Q_onAssert(&Q_this_module_[0], (int_t)(id_))

#endif /* Q_NASSERT */

#ifdef __cplusplus
    extern "C" {
#endif

/*! Callback function invoked in case of any assertion failure. */
/**
* \description
* This is an application-specific callback function needs to be defined in
* the application to perform the clean system shutdown and perhaps a reset.
*
* \arguments
* \arg[in] \c module name of the file/module in which the assertion failed
*                    (constant ROM-based, zero-terminated C string)
* \arg[in] \c line   line number or user-specified ID-number to identify
*                    the location of the failing assertion within the file.
*
* \note This callback function should _not_ return, as continuation after
* an assertion failure does not make sense.
*
* \note The Q_onAssert() function is the last line of defense after the
* system failure and its implementation shouild be very __carefully__
* designed and __tested__ under various fault conditions, including but
* not limited to: stack overflow, stack corruption, or calling Q_onAssert()
* from an interrupt.
*
* \note It is typically a __bad idea__ to implement Q_onAssert() as an
* endless loop that ties up the CPU. During debuggin, Q_onAssert() is an
* ideal place to put a breakpoint.
*
* Called by the following macros: #Q_ASSERT, #Q_REQUIRE, #Q_ENSURE,
* #Q_ERROR, #Q_ALLEGE as well as #Q_ASSERT_ID, #Q_REQUIRE_ID, #Q_ENSURE_ID,
* #Q_ERROR_ID, and #Q_ALLEGE_ID.
*/
void Q_onAssert(char_t const Q_ROM * const file, int_t line);

#ifdef __cplusplus
    }
#endif

/*! Assertion for checking preconditions. */
/**
* \description
* This macro is equivalent to #Q_ASSERT, except the name provides a better
* documentation of the intention of this assertion.
*
* \arguments
* \arg[in] \c test_ Boolean expression
*/
#define Q_REQUIRE(test_)         Q_ASSERT(test_)

/*! Assertion for checking preconditions with user-specified assertion-id. */
/**
* \description
* Equivalent to #Q_ASSERT_ID, except the macro name provides a better
* documentation of the intention of this assertion.
*
* \arguments
* \arg[in] \c id_   ID number (unique within the module) of the assertion
* \arg[in] \c test_ Boolean expression
*/
#define Q_REQUIRE_ID(id_, test_) Q_ASSERT_ID(id_, test_)

/*! Assertion for checking postconditions. */
/** Equivalent to #Q_ASSERT, except the macro name provides a better
* documentation of the intention of this assertion.
*
* \arguments
* \arg[in] \c test_ Boolean expression
*/
#define Q_ENSURE(test_) Q_ASSERT(test_)

/*! Assertion for checking postconditions with user-specified assertion-id. */
/**
* \description
* Equivalent to #Q_ASSERT_ID, except the name provides a better documentation
* of the intention of this assertion.
*
* \arguments
* \arg[in] \c id_   ID number (unique within the module) of the assertion
* \arg[in] \c test_ Boolean expression
*/
#define Q_ENSURE_ID(id_, test_) Q_ASSERT_ID(id_, test_)

/*! Assertion for checking invariants. */
/**
* \description
* Equivalent to #Q_ASSERT, except the macro name provides a better
* documentation of the intention of this assertion.
*
* \arguments
* \arg[in] \c test_ Boolean expression
*/
#define Q_INVARIANT(test_) Q_ASSERT(test_)

/*! Assertion for checking invariants with user-specified assertion-id. */
/**
* \description
* Equivalent to #Q_ASSERT_ID, except the macro name provides a better
* documentation of the intention of this assertion.
*
* \arguments
* \arg[in] \c id_   ID number (unique within the module) of the assertion
* \arg[in] \c test_ Boolean expression
*/
#define Q_INVARIANT_ID(id_, test_) Q_ASSERT_ID(id_, test_)

/*! Compile-time assertion. */
/**
* \description
* This type of assertion deliberately causes a compile-time error when
* the \c test_ evaluates to FALSE. The macro exploits the fact that in C/C++
* a dimension of an array cannot be negative. The compile-time assertion has
* no runtime side effects.
*
* \arguments
* \arg[in] \c test_ Compile-time Boolean expression
*/
#define Q_ASSERT_COMPILE(test_) \
    extern int_t Q_assert_compile[(test_) ? 1 : -1]

#endif /* qassert_h */
