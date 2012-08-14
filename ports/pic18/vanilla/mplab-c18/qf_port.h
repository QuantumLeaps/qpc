/*****************************************************************************
* Product:  QF/C, PIC18, vanilla kernel, MPLAB-C18 compiler
* Last Updated for Version: 4.4.01
* Date of the Last Update:  Mar 22, 2012
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
#define QF_MAX_ACTIVE           8

#define QF_EVENT_SIZ_SIZE       1
#define QF_EQUEUE_CTR_SIZE      1
#define QF_MPOOL_SIZ_SIZE       1
#define QF_MPOOL_CTR_SIZE       1
#define QF_TIMEEVT_CTR_SIZE     2

                         /* QF interrupt disabling and enabling, see NOTE02 */
#define QF_INT_DISABLE()    (INTCON &= 0b00111111)
#define QF_INT_ENABLE()     (INTCON |= 0b11000000)

                              /* QF critical section entry/exit, see NOTE03 */
#define QF_CRIT_STAT_TYPE       unsigned char

#define QF_CRIT_ENTRY(stat_)    do { \
    (stat_) = (INTCON & 0b11000000); \
    QF_INT_DISABLE(); \
} while (0)

#define QF_CRIT_EXIT(stat_)     (INTCON |= (stat_))

                                /* INTCON SFR declaration for PIC18 devices */
extern volatile near unsigned char INTCON;

#include "qep_port.h"                                           /* QEP port */
#include "qvanilla.h"                       /* "Vanilla" cooperative kernel */
#include "qf.h"                 /* QF platform-independent public interface */


/*****************************************************************************
* NOTE01:
* The maximum number of active objects QF_MAX_ACTIVE can be increased up to
* 63, inclusive. The lower limit of 8 is used here to save RAM and also
* because the internal scheduler algorithms perform slighlty better for this
* lower limit.
*
* NOTE02:
* The task-level interrupt disabling/enabling policy assumes that interrupt
* priorities are allowed (IPEN=1) and that high-priority interrupts can nest
* on low-priority interrupts. In this mode, the bit INTCON<7> is
* interpreted as GIEH (high-priority interrupt enable) and INTCON<6> is
* interpreted as GIEL (low-priority interrupt enable). The task-level
* interrupt locking policy clears both GIEH and GIEL bits and unlocking
* sets both GIEH and GIEL.
*
* NOTE03:
* The ISR-level interrupt policy allows interrupt nesting. The critical
* section entry saves the current interrupt status (the bits GIEH and GIEL)
* and clears both GIEH and GIEL. The unlocking policy restores the saved
* state of the GIEH and GIEL bits. In particular, this critical section
* implementation will never enable interrupts inside high-level ISRs, and
* will enable only the high-level interrupts inside the low-level ISRs.
* You should not enable interrupts explicitly inside ISRs.
*
* Please note that setting IPEN=1 does not necessarily mean that interrupts
* will nest. You can always configure the priorities of all interrupts to
* high, which is equivalent to running in the compatibility mode.
*/

#endif                                                         /* qf_port_h */
