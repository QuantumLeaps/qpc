/*****************************************************************************
* Product: RSK-M16C26A board definitions
* Last Updated for Version: 4.0.02
* Date of the Last Update:  Aug 29, 2008
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
#ifndef RSKM16C26ADEF_H
#define RSKM16C26ADEF_H

                                                 /* peripheral clock speeds */
#define f1_CLK_SPEED        20000000
#define fc_CLK_SPEED        32768

                                                                /* switches */
#define SW1                 p8_3
#define SW2                 p8_4
#define SW3                 p1_7
#define SW1_DDR             pd8_3
#define SW2_DDR             pd8_4
#define SW3_DDR             pd1_7

                                                                    /* LEDs */
#define LED0                p10_4
#define LED1                p10_5
#define LED2                p10_6
#define LED3                p10_7
#define LED0_DDR            pd10_4
#define LED1_DDR            pd10_5
#define LED2_DDR            pd10_6
#define LED3_DDR            pd10_7

#define LED_ON              0
#define LED_OFF             1
#define SET_BIT_HIGH        1
#define SET_BIT_LOW         0

#endif                                                   /* RSKM16C26ADEF_H */