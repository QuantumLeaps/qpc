/*****************************************************************************
* Product: Board Support Package (BSP) for the Calculator example
* Last Updated for Version: 4.1.05
* Date of the Last Update:  Aug 27, 2010
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2010 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "bsp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISP_WIDTH  9

static char l_display[DISP_WIDTH + 1];            /* the calculator display */
static int  l_len;                        /* number of displayed characters */

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
        memmove(&l_display[0], &l_display[1], DISP_WIDTH - 1);
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
    printf("\n[%s] ", l_display);
}
/*..........................................................................*/
void BSP_exit(void) {
    printf("\nBye! Bye!");
    fflush(stdout);
    _exit(0);
}
/*..........................................................................*/
double BSP_get_value(void) {
    return strtod(l_display, (char **)0);
}
/*..........................................................................*/
int BSP_eval(double operand1, int oper, double operand2) {
    int ok = 1;
    double result;
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
                strcpy(l_display, " Error 0 ");    /* error: divide by zero */
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
            sprintf(l_display, "%9.6g", result);
        }
        else {
            strcpy(l_display, " Error 1 ");          /* error: out of range */
            ok = 0;
        }
    }
    return ok;
}
/*..........................................................................*/
void BSP_message(char const *msg) {
    printf(msg);
}
/*..........................................................................*/
/* this function is used by the QP embedded systems-friendly assertions */
void Q_onAssert(char const * const file, int line) {
    printf("Assertion failed in %s, line %d", file, line);
    _exit(-1);
}
