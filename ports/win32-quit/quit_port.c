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
* @date Last updated on: 2022-07-24
* @version Last updated for: @ref qpc_7_0_1
*
* @file
* @brief QUIT (QP Unit Internal Test) port to Win32 with GNU or VisualC++
*/
#include "quit.h"

#include <stdio.h>  /* for printf_s() */
#include <stdlib.h> /* for exit() */

#ifndef NDEBUG
#define WIN32_LEAN_AND_MEAN
#include <windows.h> /* for DebugBreak */
#endif

//Q_DEFINE_THIS_MODULE("quit_port")

/*..........................................................................*/
static int l_test_count;

/*..........................................................................*/
void QUIT_fail_(char const *cond, char const *file, int line) {
    printf_s(" FAILED in %s:%d\n"
        "%s\n"
        "---------------------------------------------\n"
        "%d test(s)\n"
        "FAILED\n",
        file, line, cond, l_test_count);

#ifndef NDEBUG
    DebugBreak();
#endif

    exit(l_test_count);
}
/*..........................................................................*/
void TEST(char const *title) {
    if (l_test_count > 0) {
        printf_s(" PASSED\n");
    }
    printf_s("test: \"%s\" ...", title);
    ++l_test_count;
}
/*..........................................................................*/
int main(void) {
    printf_s("\n%s\n", "QP Unit Internal Testing -- QUIT");

    onRunTests();

    if (l_test_count > 0) {
        printf_s("%s\n", " PASSED");
    }
    printf_s("---------------------------------------------\n"
        "%d test(s)\nOK\n", l_test_count);

    return 0; /* success */
}

/*==========================================================================*/
Q_NORETURN Q_onAssert(char const * const module, int_t const location) {
    QUIT_fail_("Q_onAssert", module, location);
    for (;;) { /* explicitly make it "noreturn" */
    }
}

/*==========================================================================*/
#ifdef Q_SPY
/*..........................................................................*/
void QS_onCleanup(void) {
}
/*..........................................................................*/
void QS_onReset(void) {
}
/*..........................................................................*/
void QS_onFlush(void) {
}
/*..........................................................................*/
QSTimeCtr QS_onGetTime(void) {
    return (QSTimeCtr)0U;
}
/*..........................................................................*/
void QS_onCommand(uint8_t cmdId, uint32_t param1,
    uint32_t param2, uint32_t param3)
{
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif
