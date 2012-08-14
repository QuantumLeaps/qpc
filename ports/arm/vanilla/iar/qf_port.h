/*****************************************************************************
* Product:  QF/C, ARM, Vanilla port, IAR-compiler
* Last Updated for Version: 4.3.00
* Date of the Last Update:  Nov 06, 2011
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

           /* fast unconditional interrupt disabling/enabling for ARM state */
#define QF_INT_DISABLE()           __asm("MSR cpsr_c,#(0x1F | 0x80 | 0x40)")
#define QF_INT_ENABLE()            __asm("MSR cpsr_c,#(0x1F)")

#if (__CPU_MODE__ == 1)                                      /* THUMB mode? */
                                          /* QF interrupt locking/unlocking */
    #define QF_CRIT_STAT_TYPE       unsigned long
    #define QF_CRIT_ENTRY(stat_)    ((stat_) = QF_int_lock_SYS())
    #define QF_CRIT_EXIT(stat_)     QF_int_unlock_SYS(stat_)

    QF_CRIT_STAT_TYPE QF_int_lock_SYS(void);
    void QF_int_unlock_SYS(QF_CRIT_STAT_TYPE stat);

#elif (__CPU_MODE__ == 2)                                      /* ARM mode? */

    #define QF_CRIT_STAT_TYPE         unsigned long
    #define QF_CRIT_ENTRY(stat_)      do { \
        (stat_) = __get_CPSR(); \
        QF_INT_DISABLE(); \
    } while (0)
    #define QF_CRIT_EXIT(stat_)       __set_CPSR(stat_)

    #include <intrinsics.h>               /* for  __get_CPSR()/__set_CPSR() */

#else

    #error Incorrect __CPU_MODE__. Must be ARM or THUMB.

#endif

void QF_reset(void);
void QF_undef(void);
void QF_swi(void);
void QF_pAbort(void);
void QF_dAbort(void);
void QF_reserved(void);
void QF_irq(void);
void QF_fiq(void);

void BSP_irq(void);
void BSP_fiq(void);

#include "qep_port.h"                                           /* QEP port */
#include "qvanilla.h"                       /* "Vanilla" cooperative kernel */
#include "qf.h"                 /* QF platform-independent public interface */

#endif                                                         /* qf_port_h */
