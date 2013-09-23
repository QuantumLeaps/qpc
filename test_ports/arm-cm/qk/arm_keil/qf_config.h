/*****************************************************************************
* Product: QF/C, ARM Cortex-M, QK port, ARM-KEIL compiler
* Last Updated for Version: 5.0.0
* Date of the Last Update:  Sep 15, 2013
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef qf_config_h
#define qf_config_h

     /* The maximum number of active objects in the application, see NOTE01 */
#define QF_MAX_ACTIVE               32
                                   /* The number of system clock tick rates */
#define QF_MAX_TICK_RATE            2

                                             /* QF interrupt disable/enable */
#define QF_SET_BASEPRI(val)         __asm("msr BASEPRI," #val)
#define QF_INT_DISABLE()            QF_SET_BASEPRI(QF_BASEPRI)
#define QF_INT_ENABLE()             QF_SET_BASEPRI(0U)

                                          /* QF critical section entry/exit */
/* QF_CRIT_STAT_TYPE not defined: unconditional interrupt unlocking" policy */
#define QF_CRIT_ENTRY(dummy)        QF_INT_DISABLE()
#define QF_CRIT_EXIT(dummy)         QF_INT_ENABLE()
#define QF_CRIT_EXIT_NOP()          __nop()

#if (__TARGET_ARCH_THUMB != 3)   /* not Cortex-M0/M0+/M1 (not v6-M, v6S-M)? */
    #define QF_BASEPRI  (0xFFU >> 2)
    #define QF_LOG2(n_) ((uint8_t)(32U - __clz(n_)))
#else
    #define QF_BASEPRI  (0xFFU >> 1)
#endif
#define QF_MASKED_ISR   (QF_BASEPRI >> (__NVIC_PRIO_BITS - 1))

/*****************************************************************************
* NOTE01:
* The maximum number of active objects QF_MAX_ACTIVE can be increased
* up to 63, if necessary. Here it is set to a lower level to save some RAM.
*/

#endif                                                       /* qf_config_h */
