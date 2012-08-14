/*****************************************************************************
* Product: ANSI C library facilities for PC-Lint
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Jan 21, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This software is be distributed and modified under the terms of the GNU
* General Public License (GPL) as published by the Free Software Foundation
* and appearing in the file GPL.TXT included in the packaging of this file.
* Please note that GPL requires that all works based on this software must
* also be made publicly available under the terms of the GPL ("Copyleft").
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
#ifndef __setjmp_h
#define __setjmp_h

typedef struct __jmp_buf {
    unsigned  j_reg0;
    unsigned  j_reg1;
    unsigned  j_reg2;
    unsigned  j_reg3;
    unsigned  j_reg4;
    unsigned  j_reg5;
    unsigned  j_reg6;
    unsigned  j_reg7;
} jmp_buf[1];

#ifdef __cplusplus
extern "C" {
#endif

void longjmp(jmp_buf __jmpb, int __retval);
int  setjmp(jmp_buf __jmpb);

#ifdef __cplusplus
}
#endif

#endif                                                        /* __setjmp_h */

