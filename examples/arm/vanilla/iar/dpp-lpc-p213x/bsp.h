/*****************************************************************************
* Product: Board Support Package for LPC-P123X
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Jun 05, 2008
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
                                                                /* OSC [Hz] */
#define FOSC              14745600
                                                           /* Core clk [Hz] */
#define CCLK              (FOSC*4)
                                /* Peripheral clk -- the same as core clock */
#define PCLK              CCLK
                                              /* Sys timer tick per seconds */
#define BSP_TICKS_PER_SEC 100

#include <iolpc2138.h>      /* IAR-compiler header file for the LPC2138 I/O */

#include "lpc_data_type.h"
#include "drv_hd44780.h"
#include "drv_hd44780_cnfg.h"

void BSP_init(void);
void BSP_displyPhilStat(uint8_t n, char const *stat);
void BSP_busyDelay(void);          /* to artificially extend RTC processing */

int __low_level_init(void);

#ifdef Q_SPY
    extern QSTimeCtr QS_tickTime;                /* keeps timetsamp at tick */
#endif

#endif                                                             /* bsp_h */


