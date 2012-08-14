/*****************************************************************************
* Product: ANSI C library facilities for PC-Lint
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Jan 13, 2012
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
#ifndef errno_h
#define errno_h

extern int errno;

#define EDOM    33
#define ERANGE  34
#define EFPOS   35
#define EILSEQ  36

void perror(char const *s);
char *strerror(int errnum);

#endif                                                           /* errno_h */

