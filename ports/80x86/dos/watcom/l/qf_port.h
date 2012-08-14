/*****************************************************************************
* Product:  QF/C port, 80x86, Vanilla, Open Watcom compiler
* Last Updated for Version: 4.3.00
* Date of the Last Update:  Oct 31, 2011
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2011 Quantum Leaps, LLC. All rights reserved.
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
#ifndef qf_port_h
#define qf_port_h

                 /* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE               63

                                            /* DOS interrupt disable/enable */
#define QF_INT_DISABLE()            _disable()
#define QF_INT_ENABLE()             _enable()

                                         /* DOS critical section entry/exit */
/* QF_CRIT_STAT_TYPE not defined */
#define QF_CRIT_ENTRY(dummy)        _disable()
#define QF_CRIT_EXIT(dummy)         _enable()

                                                      /* DOS ISR entry/exit */
#define QF_ISR_ENTRY()              _enable()
#define QF_ISR_EXIT() do {\
    _disable(); \
    outp(0x20, 0x20); \
} while (0)


#include <i86.h>                                /* for _disable()/_enable() */

#include "qep_port.h"                                           /* QEP port */
#include "qvanilla.h"                   /* The "Vanilla" cooperative kernel */
#include "qf.h"                 /* QF platform-independent public interface */

#endif                                                         /* qf_port_h */
