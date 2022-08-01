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
* @date Last updated on: 2022-07-30
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QEP/C port to Win32 with GNU or Visual Studio C/C++ compilers
*/
#ifndef QEP_PORT_H
#define QEP_PORT_H

#include <stdint.h>  /* Exact-width types. WG14/N843 C99 Standard */
#include <stdbool.h> /* Boolean type.      WG14/N843 C99 Standard */

#ifdef __GNUC__

    /*! no-return function specifier (GCC-ARM compiler) */
    #define Q_NORETURN   __attribute__ ((noreturn)) void

#elif (defined _MSC_VER) && (defined __cplusplus)

    /* no-return function specifier (Microsoft Visual Studio C++ compiler) */
    #define Q_NORETURN   [[ noreturn ]] void

    /*
    * This is the case where QP/C is compiled by the Microsoft Visual C++
    * compiler in the C++ mode, which can happen when qep_port.h is included
    * in a C++ module, or the compilation is forced to C++ by the option /TP.
    *
    * The following pragma suppresses the level-4 C++ warnings C4510, C4512,
    * and C4610, which warn that default constructors and assignment operators
    * could not be generated for structures QMState and QMTranActTable.
    *
    * The QP/C source code cannot be changed to avoid these C++ warnings
    * because the structures QMState and QMTranActTable must remain PODs
    * (Plain Old Datatypes) to be initializable statically with constant
    * initializers.
    */
    #pragma warning (disable: 4510 4512 4610)

#endif

#include "qep.h"     /* QEP platform-independent public interface */

#if (defined __cplusplus) && (defined _MSC_VER)
    #pragma warning (default: 4510 4512 4610)
#endif

#endif /* QEP_PORT_H */
