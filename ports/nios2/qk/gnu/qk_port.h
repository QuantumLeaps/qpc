/*****************************************************************************
* Product: QK/C port to Altera Nios II, GNU compiler
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
#ifndef qk_port_h
#define qk_port_h

                                      /* QK thread-local storage for NewLib */
#define QK_TLS(act_) \
    if ((act_)->thread != (void *)0) { \
        _impure_ptr = (struct _reent *)(act_)->thread; \
    } else ((void)0)

extern alt_u8 ALT_malloc_ceiling;  /* priority ceiling for malloc, see NOTE */
extern alt_u8 ALT_env_ceiling;        /* priority ceiling for env, see NOTE */

#include "qk.h"                 /* QK platform-independent public interface */

#endif                                                         /* qk_port_h */

/*****************************************************************************
* NOTE: ALT_malloc_ceiling and ALT_env_ceiling are the priority ceilings for
* the malloc mutex and env mutex, respectively. These variables are
* initialized to the maximum possible value, which means that the QK
* scheduler is effectively locked during the access to the heap or
* environment variables. However, the application can easily lower either of
* the priority ceilings by setting ALT_malloc_ceiling  or ALT_env_ceiling
* to a different value, which means that only tasks with priorities below
* the ceiling are affected. This allows to be much more selective.
*/
