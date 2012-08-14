/*****************************************************************************
* Product:  QF/C, TMS320C55x, QK port, TI-C5500 compiler
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Feb 12, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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

     /* The maximum number of active objects in the application, see NOTE01 */
#define QF_MAX_ACTIVE               8

#define QF_EVENT_SIZ_SIZE           2
#define QF_EQUEUE_CTR_SIZE          2
#define QF_MPOOL_SIZ_SIZE           2
#define QF_MPOOL_CTR_SIZE           2
#define QF_TIMEEVT_CTR_SIZE         2

                                     /* QF interrupt disable/enable, NOTE02 */
#define QF_INT_DISABLE()            _disable_interrupts()
#define QF_INT_ENABLE()             _enable_interrupts()

                                          /* QF critical section entry/exit */
#define QF_CRIT_STAT_TYPE           unsigned int
#define QF_CRIT_ENTRY(stat_)        ((stat_) = _disable_interrupts())
#define QF_CRIT_EXIT(stat_)         _restore_interrupts(stat_)


#include "qep_port.h"                                           /* QEP port */
#include "qk_port.h"                                            /* QK  port */
#include "qf.h"                 /* QF platform-independent public interface */

void QF_zero(void);                           /* zero the .bss QF variables */
void bzero(uint8_t *ptr, uint16_t len);    /* helper to clear other objects */

/*****************************************************************************
* NOTE01:
* The maximum number of active objects QF_MAX_ACTIVE could be increased to
* 63 inclusive. Here, the lower limit is used only to save some RAM.
*
* NOTE02: (TMS320C55x Optimizing C/C++ Compiler User’s Guide, Section 6.6.4)
* On some versions of the hardware there is a latency between setting the
* INTM bit and when interrupts are actually disabled, therefore it is NOT
* advisible to use inline assembly for setting/clearing the INTM mask.
* Using the intrinsics not only allows you to enable and disable the
* interrupts with C code but guarantees that any necessary timing issues
* are handled automatically.
*
* NOTE03:
* The QF_CRIT_STAT_TYPE macro is not defined, which means that this QP port
* uses the policy of "unconditional critical section exit". This policy does
* NOT allow nesting of critical sections.
*/

#endif                                                         /* qf_port_h */
