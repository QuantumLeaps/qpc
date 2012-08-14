/*****************************************************************************
* Product: QF/C port to Altera Nios II, QK kernel, GNU compiler
* Last Updated for Version: 4.1.02
* Date of the Last Update:  Jan 28, 2010
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
#ifndef qf_port_h
#define qf_port_h

                 /* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE               63
                                                     /* QF critical section */
#define QF_INT_KEY_TYPE             alt_irq_context
#define QF_INT_LOCK(key_)           ((key_) = alt_irq_disable_all())
#define QF_INT_UNLOCK(key_)         alt_irq_enable_all(key_)


#include "sys/alt_irq.h"  /* for alt_irq_disable_all()/alt_irq_enable_all() */

#include "qep_port.h"                                           /* QEP port */
#include "qk_port.h"                                             /* QK port */
#include "qf.h"                 /* QF platform-independent public interface */

#endif                                                         /* qf_port_h */
