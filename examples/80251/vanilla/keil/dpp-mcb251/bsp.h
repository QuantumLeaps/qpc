/*****************************************************************************
* Product: DPP example, Board Support Package for MCB251
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Aug 11, 2008
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
#ifndef bsp_h
#define bsp_h

#include <reg251s.h>                         /* SFR declarations for 80251S */

#define BSP_CPU_HZ           11059000
#define BSP_TICKS_PER_SEC    20
#define BSP_PERIPHERAL_HZ    (11059000/12)

/* LEDs of the MCB251 board */
sbit LED0 = P1 ^ 0;
sbit LED1 = P1 ^ 1;
sbit LED2 = P1 ^ 2;
sbit LED3 = P1 ^ 3;
sbit LED4 = P1 ^ 4;
sbit LED5 = P1 ^ 5;
sbit LED6 = P1 ^ 6;

void BSP_init(void);
void BSP_displyPhilStat(uint8_t n, char const *stat);
void BSP_busyDelay(void);

#ifdef Q_SPY
    extern QSTimeCtr QS_tickTime;                /* keeps timetsamp at tick */
#endif

#endif                                                             /* bsp_h */

