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
#ifndef __signal_h
#define __signal_h

typedef int sig_atomic_t;   /* Atomic entity type (ANSI) */
typedef void (*_CatcherPTR)();

                            /* Default action   */
#define SIG_DFL ((_CatcherPTR)0)

                            /* Ignore action    */
#define SIG_IGN ((_CatcherPTR)1)

                            /* Error return     */
#define SIG_ERR ((_CatcherPTR)-1)

#define SIGABRT 22

                            /* Floating point trap  */
#define SIGFPE  8

                            /* Illegal instruction  */
#define SIGILL  4

#define SIGINT  2

                            /* Memory access violation */
#define SIGSEGV 11

#define SIGTERM 15

#ifdef __cplusplus
extern "C" {
#endif

int raise(int __sig);

#ifdef __cplusplus
void (* signal(int __sig, void (*__func)(int)))(int);
}
#else
void (*signal(int __sig, void (*func)())) (int);
#endif

#endif                                                        /* __signal_h */

