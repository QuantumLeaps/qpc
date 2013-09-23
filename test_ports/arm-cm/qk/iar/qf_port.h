/*****************************************************************************
* Product: QF/C, Cortex-M, QK port, IAR compiler
* Last Updated for Version: 5.0.0
* Date of the Last Update:  Sep 16, 2013
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
#ifndef qf_port_h
#define qf_port_h

      /* The maximum number of active objects in the application, see NOTE1 */
#define QF_MAX_ACTIVE               32
                                   /* The number of system clock tick rates */
#define QF_MAX_TICK_RATE            2

                                  /* QF interrupt disable/enable, see NOTE2 */
#define QF_INT_DISABLE()            __set_BASEPRI(QF_BASEPRI)
#define QF_INT_ENABLE()             __set_BASEPRI(0U)

                                          /* QF critical section entry/exit */
/* QF_CRIT_STAT_TYPE not defined: unconditional interrupt unlocking" policy */
#define QF_CRIT_ENTRY(dummy)        QF_INT_DISABLE()
#define QF_CRIT_EXIT(dummy)         QF_INT_ENABLE()
#define QF_CRIT_EXIT_NOP()          __no_operation()

#if (__CORE__ != __ARM6M__)                       /* not Cortex-M0/M0+/M1 ? */

    /* NOTE: keep in synch with the value defined in "qk_port.s", see NOTE3 */
    #define QF_BASEPRI  (0xFFU >> 2)

              /* Cortex-M3/M4/M4F provide the CLZ instruction for fast LOG2 */
    #define QF_LOG2(n_) ((uint8_t)(32U - __CLZ(n_)))
#else
    /* NOTE: keep in synch with the value defined in "qk_port.s", see NOTE4 */
    #define QF_BASEPRI  (0xFFU >> 1)
#endif

/* QF-aware interrupt priority for CMSIS function NVIC_SetPriority(), NOTE5 */
#define QF_AWARE_ISR_CMSIS_PRI  (QF_BASEPRI >> (8 - __NVIC_PRIO_BITS))

#include <intrinsics.h>                          /* IAR intrinsic functions */
#include "qep_port.h"                                           /* QEP port */
#include "qk_port.h"                                             /* QK port */
#include "qf.h"                 /* QF platform-independent public interface */

/*****************************************************************************
* NOTE1:
* The maximum number of active objects QF_MAX_ACTIVE can be increased
* up to 63, if necessary. Here it is set to a lower level to save some RAM.
*
* NOTE2:
* The interrupt disable/enable policy uses the BASEPRI register in Cortex-M
* to disable interrupts only with priority lower than the level specified by
* the QF_BASEPRI macro. The interrupts with priorities above QF_BASEPRI (i.e.,
* with numerical priority values lower than QF_BASEPRI) are not disabled in
* this method. These free-running interrupts are not allowed to call any QF
* services, because QF is not aware of these interrupts. Coversely, only
* "QF-aware" interrupts, with numerical values of priorities eqal to or
* higher than QF_BASEPRI, can call QF services.
*
* NOTE3:
* For Cortex-M3/M4/M4F, the macro QF_BASEPRI leaves the top 2 priority bits
* empty for QF-aware interrupts. This is the highest-possible priority
* (lowest possible numerical value) for the guaranteed 3 priority bits
* implemented in the NVIC.
*
* NOTE4:
* For Cortex-M0/M0+, the macro QF_BASEPRI leaves only 1 top priority bit
* empty for the QF-aware interrupts. This means that interrupt priority
* levels are allocated as follows: 0:QF-unaware, 1,2:QF-aware, 3:PendSV.
*
* NOTE5:
* The QF_AWARE_ISR_CMSIS_PRI macro is useful as an offset for enumerating
* the QF-aware interrupt priority levels in the applications, whereas the
* numerical values of the QF-aware interrupts must be greater or equal to
* QF_AWARE_ISR_CMSIS_PRI. The enumerated values based on
* QF_AWARE_ISR_CMSIS_PRI can be passed directly to the CMSIS function
* NVIC_SetPriority(), which shifts them by (8 - __NVIC_PRIO_BITS) into the
* correct bit position, while __NVIC_PRIO_BITS is the CMSIS macro defining
* the number of implemented priority bits in the NVIC. Please note that
* the macro QF_AWARE_ISR_CMSIS_PRI is intended only for applications and
* is not used inside the QF port, which remains generic and not dependent
* on the number of implemented priority bits in the NVIC.
*/

#endif                                                         /* qf_port_h */
