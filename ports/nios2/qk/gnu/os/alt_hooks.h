/*****************************************************************************
* Product: QK/C port to Altera Nios II, GNU compiler
* Last Updated for Version: 4.1.02
* Date of the Last Update:  Feb 03, 2010
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2010 Quantum Leaps, LLC. All rights reserved.
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
#ifndef __ALT_HOOKS_H__
#define __ALT_HOOKS_H__

#define ALT_OS_TIME_TICK()  QF_tick()
#define ALT_OS_INIT()       ((void)0)
#define ALT_OS_STOP()       ((void)0)

#define ALT_OS_INT_ENTER()  (++QK_intNest_)

#define ALT_OS_INT_EXIT()   \
    if ((--QK_intNest_) == (alt_u8)0) { \
        QK_scheduleExt_(0xFFFFFFFFUL); \
    } else ((void)0)

extern alt_u8 QK_intNest_;
void QF_tick(void);
void QK_scheduleExt_(alt_irq_context intUnlockKey);

#endif                                                   /* __ALT_HOOKS_H__ */
