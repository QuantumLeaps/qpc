/*****************************************************************************
* Product: Board Support Package (BSP) for the Time Bomb example
* Last Updated for Version: 4.1.01
* Date of the Last Update:  Nov 04, 2009
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2009 Quantum Leaps, LLC. All rights reserved.
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

/*..........................................................................*/
void BSP_display(uint8_t timeout) {
    printf("[%3d] ", (int)timeout);
    fflush(stdout);
}
/*..........................................................................*/
void BSP_boom(void) {
    printf("BOOM!!!");
    fflush(stdout);
    _exit(0);
}
/*..........................................................................*/
/* this function is used by the QP embedded systems-friendly assertions */
void Q_onAssert(char const * const file, int line) {
    printf("Assertion failed in %s, line %d", file, line);
    fflush(stdout);
    _exit(-1);
}
