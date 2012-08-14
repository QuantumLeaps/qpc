/*****************************************************************************
* Product:  QF/C, ColdFire, Vanilla port, IAR compiler
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Apr 25, 2008
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

                 /* The maximum number of active objects in the application */
#define QF_MAX_ACTIVE       63

#define QF_EVENT_SIZ_SIZE   2
#define QF_EQUEUE_CTR_SIZE  2
#define QF_MPOOL_SIZ_SIZE   2
#define QF_MPOOL_CTR_SIZE   2
#define QF_TIMEEVT_CTR_SIZE 4

                                          /* QF critical section entry/exit */
/* QF_INT_KEY_TYPE defined--"saving and restoring interrupt status" policy  */
#define QF_INT_KEY_TYPE     uint16_t
#define QF_INT_LOCK(key_)   do { \
    (key_) = __get_status_register(); \
    __disable_interrupts(); \
} while (0)
#define QF_INT_UNLOCK(key_) __set_status_register(key_)

#include <intrinsics.h>                          /* IAR intrinsic functions */

#include "qep_port.h"                                           /* QEP port */
#include "qvanilla.h"                       /* "Vanilla" cooperative kernel */
#include "qf.h"                 /* QF platform-independent public interface */

#endif                                                         /* qf_port_h */
