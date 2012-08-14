/*****************************************************************************
* Product:  QF/C, Renesas RX, Vanilla port, IAR compiler
* Last Updated for Version: 4.3.00
* Date of the Last Update:  Nov 03, 2011
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
                    /* The maximum number of event pools in the application */
#define QF_MAX_EPOOL                6
                                         /* QF interrupt disabling/enabling */
#define QF_INT_DISABLE()            __disable_interrupt()
#define QF_INT_ENABLE()             __enable_interrupt()

                                          /* QF critical section entry/exit */
/* QF_CRIT_STAT_TYPE not defined: unconditional interrupt unlocking, NOTE01 */
#define QF_CRIT_ENTRY(dummy)        __disable_interrupt()
#define QF_CRIT_EXIT(dummy)         __enable_interrupt()

                                      /* QF ISR entry/exit, also see NOTE01 */
#define QF_ISR_ENTRY()             __enable_interrupt()
#define QF_ISR_EXIT()              __disable_interrupt()

#include <intrinsics.h>                          /* IAR intrinsic functions */

#include "qep_port.h"                                           /* QEP port */
#include "qvanilla.h"                       /* "Vanilla" cooperative kernel */
#include "qf.h"                 /* QF platform-independent public interface */

/*****************************************************************************
* NOTE01:
* This policy does not allow to nest critical sections. Therefore interrupts
* must be enabled in ISRs before calling any QP service. This should be no
* problem, because if interrupts nesting is not desired, the interrupts can
* always be prioritized at the same IPL.
*/

#endif                                                         /* qf_port_h */
