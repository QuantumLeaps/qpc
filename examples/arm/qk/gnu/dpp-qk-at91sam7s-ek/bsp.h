/*****************************************************************************
* Product: Board Support Package for the AT91SAM7S-EK Evaluaiton Board
* Last Updated for Version: 4.3.00
* Date of the Last Update:  Nov 08, 2011
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
#ifndef bsp_h
#define bsp_h

                                                         /* Main Clock [Hz] */
#define MAINCK            18432000
                                     /* Maseter Clock (PLLRC div by 2) [Hz] */
#define MCK               47923200
                                             /* System clock tick rate [Hz] */
#define BSP_TICKS_PER_SEC 100

#include <AT91SAM7S64.H>                        /* AT91SAMT7S64 definitions */

void BSP_init(void);
void BSP_displyPhilStat(uint8_t n, char const *stat);
void BSP_busyDelay(void);          /* to artificially extend RTC processing */

void low_level_init(void (*reset_addr)(), void (*return_addr)());

#ifdef Q_SPY

extern uint8_t const QS_tickIRQ;

#endif

#endif                                                             /* bsp_h */


