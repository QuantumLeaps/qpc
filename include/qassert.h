/*****************************************************************************
* Product: QP/C/C++
* Last Updated for Version: 4.5.03
* Date of the Last Update:  Jan 16, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
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
#ifndef qassert_h
#define qassert_h

/**
* \file
* \ingroup qep qf qk qs
* \brief Customizable QP assertions.
*
* Defines customizable and memory-efficient assertions applicable to
* embedded systems. This header file can be used in C, C++, and mixed C/C++
* programs.
*
* \note The preprocessor switch Q_NASSERT disables checking assertions.
* In particular macros #Q_ASSERT, #Q_REQUIRE, #Q_ENSURE, #Q_INVARIANT,
* #Q_ERROR as well as  #Q_ASSERT_ID, #Q_REQUIRE_ID, #Q_ENSURE_ID,
* #Q_INVARIANT_ID, and #Q_ERROR_ID do NOT evaluate the test condition
* passed as the argument to these macros. One notable exception is the
* macro #Q_ALLEGE, that still evaluates the test condition, but does
* not report assertion failures when the switch Q_NASSERT is defined.
*/
#ifdef Q_NASSERT          /* Q_NASSERT defined--assertion checking disabled */

    #define Q_DEFINE_THIS_FILE
    #define Q_DEFINE_THIS_MODULE(name_)
    #define Q_ASSERT(test_)             ((void)0)
    #define Q_ASSERT_ID(id_, test_)     ((void)0)
    #define Q_ALLEGE(test_)             ((void)(test_))
    #define Q_ALLEGE_ID(id_, test_)     ((void)(test_))
    #define Q_ERROR()                   ((void)0)
    #define Q_ERROR_ID(id_)             ((void)0)

#else                  /* Q_NASSERT not defined--assertion checking enabled */

    /** Place this macro at the top of each C/C++ module to define the file
    * name string using __FILE__ (NOTE: __FILE__ might contain lengthy path
    * name). This file name will be used in reporting assertions in this file.
    */
    #define Q_DEFINE_THIS_FILE \
        static char_t const Q_ROM Q_ROM_VAR l_this_file[] = __FILE__;

    /** Place this macro at the top of each C/C++ module to define the module
    * name as the argument \a name_. This file name will be used in reporting
    * assertions in this file.
    */
    #define Q_DEFINE_THIS_MODULE(name_) \
        static char_t const Q_ROM Q_ROM_VAR l_this_file[] = name_;

    /** General purpose assertion that makes sure the \a test_ argument is
    * TRUE. Calls the Q_onAssert() callback if the \a test_ evaluates
    * to FALSE.
    * \note the \a test_ is NOT evaluated if assertions are disabled with
    * the Q_NASSERT switch.
    * \sa #Q_ASSERT_ID
    */
    #define Q_ASSERT(test_) \
        ((test_) ? (void)0 : Q_onAssert(&l_this_file[0], (int_t)__LINE__))

    /** General purpose assertion that makes sure the \a test_ argument is
    * TRUE. Calls the Q_onAssert() callback if the \a test_ evaluates
    * to FALSE. The argument \a id_ is the ID number (unique within
    * the file) of the assertion. This assertion style is better suited
    * for unit testig, because it avoids the volatility of line numbers
    * for identifying assertions.
    * \note the \a test_ is NOT evaluated if assertions are disabled with
    * the Q_NASSERT switch.
    * \sa #Q_ASSERT
    */
    #define Q_ASSERT_ID(id_, test_) \
        ((test_) ? (void)0 : Q_onAssert(&l_this_file[0], (int_t)(id_))

    /** General purpose assertion that ALWAYS evaluates the \a test_
    * argument and calls the Q_onAssert() callback if the \a test_
    * evaluates to FALSE.
    * \note the \a test_ argument IS always evaluated even when assertions
    * are disabled with the Q_NASSERT macro. When the Q_NASSERT macro is
    * defined, the Q_onAssert() callback is NOT called, even if the
    * \a test_ evaluates to FALSE.
    * \sa #Q_ALLEGE_ID
    */
    #define Q_ALLEGE(test_)    Q_ASSERT(test_)

    /** General purpose assertion that ALWAYS evaluates the \a test_
    * argument and calls the Q_onAssert() callback if the \a test_
    * evaluates to FALSE. This assertion style is better suited
    * for unit testing, because it avoids the volatility of line numbers
    * for identifying assertions.
    * \note the \a test_ argument IS always evaluated even when assertions
    * are disabled with the Q_NASSERT macro. When the Q_NASSERT macro is
    * defined, the Q_onAssert() callback is NOT called, even if the
    * \a test_ evaluates to FALSE.
    * \sa #Q_ALLEGE
    */
    #define Q_ALLEGE_ID(id_, test_) Q_ASSERT_ID(id_, test_)

    /** Assertion that always calls the Q_onAssert() callback if
    * ever executed.
    * \note can be disabled with the Q_NASSERT switch.
    * \sa #Q_ERROR_ID
    */
    #define Q_ERROR() \
        Q_onAssert(&l_this_file[0], (int_t)__LINE__)

    /** Assertion that always calls the Q_onAssert() callback if
    * ever executed. This assertion style is better suited for unit
    * testing, because it avoids the volatility of line numbers for
    * identifying assertions.
    * \note can be disabled with the Q_NASSERT switch.
    * \sa #Q_ERROR
    */
    #define Q_ERROR_ID(id_) \
        Q_onAssert(l_this_file, (int_t)(id_))

#endif                                                         /* Q_NASSERT */

#ifdef __cplusplus
    extern "C" {
#endif

/** \brief Type for line numbers.
*
* This typedef specifies strong type for line numbers. The use of this type,
* rather than plain 'int', is in compliance with MISRA-C 2004 Rule 6.3(adv).
*/
typedef int int_t;

/** callback invoked in case the condition passed to #Q_ASSERT, #Q_REQUIRE,
* #Q_ENSURE, #Q_ERROR, #Q_ALLEGE as well as #Q_ASSERT_ID, #Q_REQUIRE_ID,
* #Q_ENSURE_ID, #Q_ERROR_ID, and #Q_ALLEGE_ID evaluates to FALSE.
*
* \param file file name where the assertion failed
* \param line line number at which the assertion failed
*/
void Q_onAssert(char_t const Q_ROM * const Q_ROM_VAR file, int_t line);

#ifdef __cplusplus
    }
#endif

/** Assertion that checks for a precondition. This macro is equivalent to
* #Q_ASSERT, except the name provides a better documentation of the
* intention of this assertion.
* \sa #Q_REQUIRE_ID
*/
#define Q_REQUIRE(test_)         Q_ASSERT(test_)

/** Assertion that checks for a precondition. This macro is equivalent to
* #Q_ASSERT_ID, except the name provides a better documentation of the
* intention of this assertion.
* \sa #Q_REQUIRE
*/
#define Q_REQUIRE_ID(id_, test_) Q_ASSERT_ID(id_, test_)

/** Assertion that checks for a postcondition. This macro is equivalent to
* #Q_ASSERT, except the name provides a better documentation of the
* intention of this assertion.
* \sa #Q_ENSURE_ID
*/
#define Q_ENSURE(test_)          Q_ASSERT(test_)

/** Assertion that checks for a postcondition. This macro is equivalent to
* #Q_ASSERT_ID, except the name provides a better documentation of the
* intention of this assertion.
* \sa #Q_ENSURE
*/
#define Q_ENSURE_ID(id_, test_)  Q_ASSERT_ID(id_, test_)

/** Assertion that checks for an invariant. This macro is equivalent to
* #Q_ASSERT, except the name provides a better documentation of the
* intention of this assertion.
* \sa #Q_INVARIANT_ID
*/
#define Q_INVARIANT(test_)       Q_ASSERT(test_)

/** Assertion that checks for an invariant. This macro is equivalent to
* #Q_ASSERT_ID, except the name provides a better documentation of the
* intention of this assertion.
* \sa #Q_INVARIANT
*/
#define Q_INVARIANT_ID(id_, test_) Q_ASSERT_ID(id_, test_)

/** Compile-time assertion exploits the fact that in C/C++ a dimension of
* an array cannot be negative. The following declaration causes a compilation
* error if the compile-time expression (\a test_) is not TRUE. The assertion
* has no runtime side effects.
*/
#define Q_ASSERT_COMPILE(test_) \
    extern int_t Q_assert_compile[(test_) ? 1 : -1]

#endif                                                         /* qassert_h */
