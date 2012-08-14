/*****************************************************************************
* Product:  QK/C port, AVR-xmega, GNU compiler
* Last Updated for Version: 4.1.02
* Date of the Last Update:  Feb 05, 2010
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

                                            /* QK interrupt service routine */
#define QK_ISR(name_) \
ISR(name_) { \
void name_##_ISR(void); \
    uint8_t pmic_sta; \
    void name_##_ISR(void); \
    QK_intNest_ = 1; \
    name_##_ISR(); \
    cli(); \
    pmic_sta = PMIC.STATUS; \
    if ((pmic_sta == 0x01) || (pmic_sta == 0x02) || (pmic_sta == 0x04)) { \
        QK_intNest_ = 0; \
        if (QPSet8_notEmpty((QPSet8 *)&QK_readySet_)) { \
            QK_end_of_interrupt(); \
            QK_schedule_(); \
            __asm__ __volatile__ ("sei\n\t" \
                  "pop r31\n\t" \
                  "pop r30\n\t" \
                  "pop r27\n\t" \
                  "pop r26\n\t" \
                  "pop r25\n\t" \
                  "pop r24\n\t" \
                  "pop r23\n\t" \
                  "pop r22\n\t" \
                  "pop r21\n\t" \
                  "pop r20\n\t" \
                  "pop r19\n\t" \
                  "pop r18\n\t" \
                  "pop r0\n\t"  \
                  "out 0x3b,r0\n\t" \
                  "pop r0\n\t" \
                  "out 0x39,r0\n\t" \
                  "pop r0\n\t" \
                  "out 0x38,r0\n\t" \
                  "pop r0\n\t" \
                  "out 0x3f,r0\n\t" \
                  "pop r0\n\t" \
                  "pop r1\n\t" \
                  "ret"); \
        } \
    } \
    else { \
        sei(); \
    } \
} \
void name_##_ISR()

void QK_end_of_interrupt  (void);

#include "qk.h"                 /* QK platform-independent public interface */

#endif                                                         /* qk_port_h */
