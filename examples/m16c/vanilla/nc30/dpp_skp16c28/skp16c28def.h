/*****************************************************************************
* Product: SKP16C26 board definitions
* Last Updated for Version: 4.0.02
* Date of the Last Update:  Aug 25, 2008
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
#ifndef SKP16C26DEF_H
#define SKP16C26DEF_H

                                                 /* peripheral clock speeds */
#define MAIN_OSC_HZ         10000000
#define SUB_OSC_HZ          32768

                                                                /* switches */
#define SW1                 p8_3
#define SW2                 p8_2
#define SW3                 p8_1
#define SW1_DDR             pd8_3
#define SW2_DDR             pd8_2
#define SW3_DDR             pd8_1

                                                                    /* LEDs */
#define LED0                p8_0
#define LED1                p7_4
#define LED2                p7_2

#define LED0_DDR            pd8_0
#define LED1_DDR            pd7_4
#define LED2_DDR            pd7_2

#define LED_ON              0
#define LED_OFF             1
#define LEDS_ON             0x00
#define LEDS_OFF            0x0F
#define SET_BIT_HIGH        1
#define SET_BIT_LOW         0
#define SET_BYTE_HIGH       0xFF
#define SET_BYTE_LOW        0x00


#endif                                                     /* SKP16C26DEF_H */