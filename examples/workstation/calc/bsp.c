/*****************************************************************************
* Product: Board Support Package (BSP) for the Calculator example
* Last updated for version 6.9.0
* Last updated on  2020-06-24
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
*****************************************************************************/
#include "qpc.h"
#include "bsp.h"

#include "safe_std.h" /* portable "safe" <stdio.h>/<string.h> facilities */
#include <stdlib.h>   /* for exit() */

#define DISP_WIDTH      15

/* helper macros to "stringify" values */
#define VAL(x) #x
#define STRINGIFY(x) VAL(x)

static char l_display[DISP_WIDTH + 1]; /* the calculator display */
static int  l_len;  /* number of displayed characters */

/*..........................................................................*/
void BSP_clear(void) {
    memset(l_display, ' ', DISP_WIDTH - 1);
    l_display[DISP_WIDTH - 1] = '0';
    l_display[DISP_WIDTH] = '\0';
    l_len = 0;
}
/*..........................................................................*/
void BSP_insert(int keyId) {
    if (l_len == 0) {
        l_display[DISP_WIDTH - 1] = (char)keyId;
        ++l_len;
    }
    else if (l_len < (DISP_WIDTH - 1)) {
        MEMMOVE_S(&l_display[0], DISP_WIDTH, &l_display[1], DISP_WIDTH - 1);
        l_display[DISP_WIDTH - 1] = (char)keyId;
        ++l_len;
    }
}
/*..........................................................................*/
void BSP_negate(void) {
    BSP_clear();
    l_display[DISP_WIDTH - 2] = '-';
}
/*..........................................................................*/
void BSP_display(void) {
    PRINTF_S("\n[%" STRINGIFY(DISP_WIDTH) "s] ", l_display);
}
/*..........................................................................*/
void BSP_exit(void) {
    PRINTF_S("\n%s\n", "Bye! Bye!");
    fflush(stdout);
    QF_onCleanup();
    exit(0);
}
/*..........................................................................*/
double BSP_get_value(void) {
    return strtod(l_display, (char **)0);
}
/*..........................................................................*/
int BSP_eval(double operand1, int oper, double operand2) {
    int ok = 1;
    double result = 0.0;
    switch (oper) {
        case KEY_PLUS: {
            result = operand1 + operand2;
            break;
        }
        case KEY_MINUS: {
            result = operand1 - operand2;
            break;
        }
        case KEY_MULT: {
            result = operand1 * operand2;
            break;
        }
        case KEY_DIVIDE: {
            if ((operand2 < -1e-30) || (1e-30 < operand2)) {
                result = operand1 / operand2;
            }
            else {
                /* error: divide by zero */
                STRNCPY_S(l_display, DISP_WIDTH, " Error 0 ");
                ok = 0;
            }
            break;
        }
    }
    if (ok) {
        if ((-0.000001 < result) && (result < 0.000001)) {
            result = 0.0;
        }
        if ((-99999999.0 < result) && (result < 99999999.0)) {
            SNPRINTF_S(l_display, DISP_WIDTH, "%10.7g", result);
        }
        else {
            /* error: out of range */
            STRNCPY_S(l_display, DISP_WIDTH, " Error 1 ");
            ok = 0;
        }
    }
    return ok;
}
/*..........................................................................*/
void BSP_message(char const *msg) {
    PRINTF_S("%s", msg);
}

/*..........................................................................*/
void QF_onStartup(void) {
    QF_consoleSetup();
}
/*..........................................................................*/
void QF_onCleanup(void) {
    QF_consoleCleanup();
}
/*..........................................................................*/
void QF_onClockTick(void) {
}

/*..........................................................................*/
/* this function is used by the QP embedded systems-friendly assertions */
Q_NORETURN Q_onAssert(char const * const module, int_t const loc) {
    FPRINTF_S(stderr, "Assertion failed in %s:%d", module, loc);
    QF_onCleanup();
    exit(-1);
}
