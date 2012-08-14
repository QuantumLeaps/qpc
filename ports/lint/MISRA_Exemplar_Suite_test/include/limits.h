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
#ifndef __limits_h
#define __limits_h

#define CHAR_BIT            8

#if ('\x80' < 0)
#define CHAR_MAX            127
#define CHAR_MIN            (-128)
#else
#define CHAR_MAX            255
#define CHAR_MIN            0
#endif

#define SCHAR_MAX           127
#define SCHAR_MIN           (-128)
#define UCHAR_MAX           255

#define SHRT_MAX            0x7FFF
#define SHRT_MIN            ((int)0x8000)
#define USHRT_MAX           0xFFFFU

#define INT_MAX             0x7FFF
#define INT_MIN             ((int)0x8000)
#define UINT_MAX            0xFFFFU

#define LONG_MAX            0x7FFFFFFFL
#define LONG_MIN            ((long)0x80000000L)
#define ULONG_MAX           0xFFFFFFFFUL

#define MB_LEN_MAX          1

#endif                                                        /* __limits_h */

