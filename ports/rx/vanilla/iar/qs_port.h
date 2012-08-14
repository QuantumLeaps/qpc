/*****************************************************************************
* Product: QS/C port, Renesas RX
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Mar 06, 2012
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
#ifndef qs_port_h
#define qs_port_h

#define QS_TIME_SIZE            4
#define QS_OBJ_PTR_SIZE         4
#define QS_FUN_PTR_SIZE         4

/*****************************************************************************
* NOTE:
* QS might be used with or without other QP components, in which case
* the separate definitions of the macros Q_ROM, Q_ROM_VAR, QF_CRIT_STAT_TYPE,
* QF_CRIT_ENTRY, and QF_CRIT_EXIT are needed. In this port QS is configured
* to be used with the other QP component, by simply including "qf_port.h"
* *before* "qs.h".
*/
#include "qf_port.h"                                      /* use QS with QF */
#include "qs.h"                 /* QS platform-independent public interface */

#endif                                                        /* qs_port_h  */
