/*****************************************************************************
* Product:  QF/C, M32C, vanilla port, NC308-compiler
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Oct 29, 2008
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
#define QF_MAX_ACTIVE           8
#define QF_EVENT_SIZ_SIZE       2
#define QF_EQUEUE_CTR_SIZE      1
#define QF_MPOOL_SIZ_SIZE       2
#define QF_MPOOL_CTR_SIZE       2
#define QF_TIMEEVT_CTR_SIZE     2

                 /* interrupt locking policy for NC308 compiler, see NOTE01 */
/* QF_INT_KEY_TYPE not defined, see NOTE01 */
#define QF_INT_LOCK(key_)       _asm("FCLR I")
#define QF_INT_UNLOCK(key_)     _asm("FSET I")


#include "qep_port.h"                                           /* QEP port */
#include "qk_port.h"                                             /* QK port */
#include "qf.h"                 /* QF platform-independent public interface */


/*****************************************************************************
* NOTE00:
* The maximum number of active objects QF_MAX_ACTIVE can be increased to 63
* inclusive. The lower limit used here is only to conserve some RAM.
*
* NOTE01:
* The M32C CPU supports interrupt prioritization. Therefore it
* is safe to unlock interrupts inside ISRs. By assigning priorities to
* interrupts you have full control over interrupt nesting. In particular,
* you can avoid interrupt nesting by assigning the same priority level to
* all interrupts.
*
* The simple policy of unconditional unlocking of interrupts upon exit from
* a critical section precludes nesting of critical sections. This policy
* means that you *must* unlock interrupts inside every ISR before invoking
* any QF service, such as QF_postFIFO(), QF_publish(), or QF_tick(). In other
* words, you need to call QF_INT_UNLOCK() after the entry to the ISR, and
* call QF_INT_LOCK() before returning from the ISR.
*/

#endif                                                         /* qf_port_h */
