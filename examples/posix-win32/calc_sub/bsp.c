//============================================================================
// Board Support Package (BSP) for the Calculator example
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"
#include "bsp.h"

#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities
#include <stdlib.h>   // for exit()

#define DISP_WIDTH      15

// helper macros to "stringify" values
#define VAL(x) #x
#define STRINGIFY(x) VAL(x)

static char l_display[DISP_WIDTH + 1]; // the calculator display
static uint32_t l_len;  // number of displayed characters

//............................................................................
void BSP_clear(void) {
    memset(l_display, ' ', DISP_WIDTH - 1);
    l_display[DISP_WIDTH - 1] = '0';
    l_display[DISP_WIDTH] = '\0';
    l_len = 0;
}
//............................................................................
void BSP_insert(uint8_t keyId) {
    if (l_len == 0U) {
        l_display[DISP_WIDTH - 1] = (char)keyId;
        ++l_len;
    }
    else if (l_len < (DISP_WIDTH - 1)) {
        MEMMOVE_S(&l_display[0], DISP_WIDTH, &l_display[1], DISP_WIDTH - 1);
        l_display[DISP_WIDTH - 1] = (char)keyId;
        ++l_len;
    }
}
//............................................................................
void BSP_display(float64_t value) {
    SNPRINTF_S(l_display, Q_DIM(l_display),
        "%" STRINGIFY(DISP_WIDTH) ".6g", value);
}
//............................................................................
void BSP_display_error(char const *err) {
    STRNCPY_S(l_display, DISP_WIDTH, err);
}
//............................................................................
void BSP_negate(void) {
    BSP_clear();
    l_display[DISP_WIDTH - 2] = '-';
}
//............................................................................
void BSP_show_display(void) {
    PRINTF_S("\n[%" STRINGIFY(DISP_WIDTH) "s] ", l_display);
}
//............................................................................
void BSP_exit(void) {
    PRINTF_S("\n%s\n", "Bye! Bye!");
    fflush(stdout);
    QF_onCleanup();
    exit(0);
}
//............................................................................
float64_t BSP_get_value(void) {
    return strtod(l_display, (char **)0);
}
//............................................................................
void BSP_message(char const *msg) {
    PRINTF_S("%s", msg);
}

//............................................................................
void QF_onStartup(void) {
    QF_consoleSetup();
}
//............................................................................
void QF_onCleanup(void) {
    QF_consoleCleanup();
}
//............................................................................
void QF_onClockTick(void) {
}

//............................................................................
// this function is used by the QP embedded systems-friendly assertions
Q_NORETURN Q_onError(char const * const module, int_t const id) {
    FPRINTF_S(stderr, "ERROR in %s:%d", module, id);
    QF_onCleanup();
    exit(-1);
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}
