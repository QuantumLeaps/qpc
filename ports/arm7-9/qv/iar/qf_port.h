/**
* @file
* @brief QF/C port to ARM7-9, cooperative QV kernel, IAR-ARM toolset
* @cond
******************************************************************************
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-04-08
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. state-machine.com.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
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
* along with this program. If not, see <www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com/licensing
* Email: info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef QF_PORT_H
#define QF_PORT_H

/* The maximum number of active objects in the application, see NOTE1 */
#define QF_MAX_ACTIVE           32U

/* The maximum number of system clock tick rates */
#define QF_MAX_TICK_RATE        2U

/* fast unconditional interrupt disabling/enabling for ARM state, NOTE2 */
#define QF_INT_DISABLE()        __asm("MSR cpsr_c,#(0x1F | 0x80)")
#define QF_INT_ENABLE()         __asm("MSR cpsr_c,#(0x1F)")

/* QF critical section entry/exit... */
#if (__CPU_MODE__ == 1) /* THUMB mode? */

    /* QF interrupt disabling/enabling policy */
    #define QF_CRIT_STAT_TYPE   unsigned int
    #define QF_CRIT_ENTRY(stat_) ((stat_) = QF_int_disable_SYS())
    #define QF_CRIT_EXIT(stat_)  QF_int_enable_SYS(stat_)

    QF_CRIT_STAT_TYPE QF_int_disable_SYS(void);
    void QF_int_enable_SYS(QF_CRIT_STAT_TYPE stat);

#elif (__CPU_MODE__ == 2) /* ARM mode? */

    #define QF_CRIT_STAT_TYPE   unsigned int
    #define QF_CRIT_ENTRY(stat_) do { \
        (stat_) = __get_CPSR(); \
        QF_INT_DISABLE(); \
    } while (false)
    #define QF_CRIT_EXIT(stat_) __set_CPSR(stat_)

    #include <intrinsics.h> /* for  __get_CPSR()/__set_CPSR() */

#else

    #error Incorrect __CPU_MODE__. Must be ARM or THUMB.

#endif

void QF_reset(void);
void QF_undef(void);
void QF_swi(void);
void QF_pAbort(void);
void QF_dAbort(void);
void QF_reserved(void);

#include "qep_port.h" /* QEP port */
#include "qv_port.h"  /* QV cooperative kernel port */
#include "qf.h"       /* QF platform-independent public interface */

/*****************************************************************************
* NOTE1:
* The maximum number of active objects QF_MAX_ACTIVE can be increased
* up to 63, if necessary. Here it is set to a lower level to save some RAM.
*
* NOTE2:
* The disabling/enabling of interrutps is only defined for the ARM state.
* The policy is to disable only the IRQ and NOT to disable the FIQ, which
* means that FIQ is a "QF-unaware" from the kernel perspective. This means
* that FIQ has "zero latency", but it also means that FIQ *cannot* call any
* QP services. Specifically FIQ cannot post or publish events.
*/

#endif /* QF_PORT_H */
