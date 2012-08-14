/*****************************************************************************
* BSP for PICDEM 2 PLUS with PIC18F452
* Last Updated for Version: 4.4.01
* Date of the Last Update:  Mar 23, 2012
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
* Quantum Leaps Web sites:  http://www.quantum-leaps.com
*                           http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef bsp_h
#define bsp_h

#include <p18C452.h>               /* SFR declarations for PIC18F452 device */

                                                /* The Oscillator frequency */
#define BSP_FOSC_HZ          4000000UL

/*--------------------------------------------------------------------------*/
                                               /* the system tick rate [Hz] */
#define BSP_TICKS_PER_SEC    20UL

void BSP_init(void);
void BSP_displyPhilStat(uint8_t n, char const Q_ROM *stat);
#define BSP_busyDelay()      ((void)0)

#ifdef Q_SPY
    extern QSTimeCtr QS_tickTime;
    extern uint8_t const Q_ROM QS_ISR_hi;
    enum AppRecords {                 /* application-specific trace records */
        PHILO_STAT = QS_USER
    };

#endif

#endif                                                             /* bsp_h */

