/*****************************************************************************
* Product: QEP/C port to Win32
* Last updated for version 5.3.0
* Last updated on  2014-03-27
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
*****************************************************************************/
#ifndef qep_port_h
#define qep_port_h

#include <stdint.h>   /* Exact-width types. WG14/N843 C99 Standard */
#include <stdbool.h>  /* Boolean type.      WG14/N843 C99 Standard */

#if (defined __cplusplus) && (defined _MSC_VER)
/*
* This is the case where QP/C is compiled by the Microsoft Visual C++
* compiler in the C++ mode, which can happen when qep_port.h is included
* in a C++ module, or the compilation is forced to C++ by the option /TP.
*
* The following pragma suppresses the level-4 C++ warnings C4510, C4512, and
* C4610, which warn that default constructors and assignment operators could
* not be generated for structures QMState and QMTranActTable.
*
* The QP/C source code cannot be changed to avoid these C++ warnings, because
* the structures QMState and QMTranActTable must remain PODs (Plain Old
* Datatypes) to be initializable statically with constant initializers.
*/
#pragma warning (disable: 4510 4512 4610)
#endif

#include "qep.h"     /* QEP platform-independent public interface */

#if (defined __cplusplus) && (defined _MSC_VER)
#pragma warning (default: 4510 4512 4610)
#endif

#endif /* qep_port_h */
