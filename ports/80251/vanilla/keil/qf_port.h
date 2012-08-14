/*****************************************************************************
* Product:  QF/C, 80251, Vanilla port, Keil compiler
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Aug 11, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
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


         /* various QF object sizes configuration for this port, see NOTE00 */
#define QF_MAX_ACTIVE               8
#define QF_EVENT_SIZ_SIZE           1
#define QF_EQUEUE_CTR_SIZE          1
#define QF_MPOOL_SIZ_SIZE           1
#define QF_MPOOL_CTR_SIZE           1
#define QF_TIMEEVT_CTR_SIZE         2

                          /* interrupt locking policy for 80x51, see NOTE01 */
/* QF_INT_KEY_TYPE not defined, simple policy used, see NOTE01 */
#define QF_INT_LOCK(key_)           (EA = 0)
#define QF_INT_UNLOCK(key_)         (EA = 1)

#include "qep_port.h"                                           /* QEP port */
#include "qvanilla.h"                       /* "Vanilla" cooperative kernel */
#include "qf.h"                 /* QF platform-independent public interface */

sbit EA = 0xA8 ^ 7;                /* global interrupt enable bit for 80x51 */

  /* shadow of the PCON register for atomic transition to Idle mode, NOTE02 */
extern volatile uint8_t bdata QF_pcon;

/*****************************************************************************
* NOTE00:
* The maximum number of active objects QF_MAX_ACTIVE can be increased to 63
* inclusive. The lower limit used here is only to conserve some RAM.
*
* NOTE01:
* The 80x51 hardware supports interrupt prioritization and does *not*
* disable interrupts upon the entry to the interrupt (the global interrupt
* enable bit EA is *not* cleared). For this reason interrupts generally
* run with interrupts enabled. This is conveniet for using the simple policy
* of unconditional unlocking of interrupts upon exit from a critical section,
* because there is no risk of nesting critical sections. (You should be
* careful not to introduce nesting of critical sections by not disabling
* interrupts before calling any QF services, such as QF_publish().)
* Instead, you can always prioritize an interrupt to a higher level to
* prevent preemptions by other interrupts.
*
* NOTE02:
* The low-power mode must be entered with interrupts locked to avoid
* an interrupt to preempt the transition to the low-power mode. However,
* the 80x51 architecture does not provide an *atomic* transition to the Idle
* mode with simultaneous unlocking interrupts. Instead, here the PCON
* register is shadowed in the QF_pcon bdata variable. The idle mode
* is set first in the shadow register QF_pcon. At the end of every interrupt,
* the ISR copies the PCON value into the QF_pcon shadow (the 80x51 clears the
* IDL and PD bits in the PCON register upon interrupt entry). Later in
* QF_onIdle(), the PCON register is restored from the shadow QF_pcon, which
* the 80x51 performs *atomically* as a single machine instruction (such as
* MOV 87H,20H). If the interrupt gets serviced between unlocking interrupts
* and restoring PCON from the shadow, the IDL/PD bits will be cleared in the
* shadow, so the machine will *not* go to the Idle?PD mode. Only if the
* IDL/PD bits survive in the shadow, the Idle/PD mode is entered.
*/

#endif                                                         /* qf_port_h */
