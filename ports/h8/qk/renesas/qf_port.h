/*****************************************************************************
* Product:  QF/C, H8, QK port, Renesas H8 compiler
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Sep 01, 2008
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

                        /* interrupt locking policy for Renesas H8 compiler */
#define QF_INT_DISABLE()        set_imask_ccr(1)
#define QF_INT_ENABLE()         set_imask_ccr(0)

                         /* critical section policy for Renesas H8 compiler */
#define QF_INT_KEY_TYPE         uint8_t
#define QF_INT_LOCK(key_)       do { \
    (key_) = get_imask_ccr(); \
    set_imask_ccr(1); \
} while (0)
#define QF_INT_UNLOCK(key_)     set_imask_ccr(key_)


#include <machine.h>      /* prototypes for get_imask_ccr()/set_imask_ccr() */


#include "qep_port.h"                                           /* QEP port */
#include "qk_port.h"                                             /* QK port */
#include "qf.h"                 /* QF platform-independent public interface */


/*****************************************************************************
* NOTE00:
* The maximum number of active objects QF_MAX_ACTIVE can be increased to 63
* inclusive. The lower limit used here is only to conserve some RAM.
*/

#endif                                                         /* qf_port_h */
