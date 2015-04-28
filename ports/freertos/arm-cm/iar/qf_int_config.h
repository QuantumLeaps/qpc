/**
* @file
* @brief QF/C port to FreeRTOS.org (version 8), ARM Cortex-M
* @ingroup ports
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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qf_int_config_h
#define qf_int_config_h

/*****************************************************************************
* NOTE:
* The following FreeRTOS configuration constant
* configMAX_SYSCALL_INTERRUPT_PRIORITY determines which Cortex-M3/M4 interrupt
* priorities can be disabled ("kernel-aware" interrupts) and which will never
* be disabled ("kernel-unaware" interrupts).
*
* Specifically, any interrupt priorities with numerical values lower than
* configMAX_SYSCALL_INTERRUPT_PRIORITY will NOT be disabled. Such interrupts
* are called "kernel-unaware" interrupts, as they CANNOT call any FreeRTOS
* (or QP) services.
*
* It is very important to keep configMAX_SYSCALL_INTERRUPT_PRIORITY
* consistent between the QP port and the application. Therefore, this
* configuation constant is defined in this separate file, which is then
* included in the QF port as well as in the FreeRTOSConfig.h in the
* application.
*
* It is ctitical that configMAX_SYSCALL_INTERRUPT_PRIORITYnd is NOT
* re-defined in the application.
*
* The current configMAX_SYSCALL_INTERRUPT_PRIORITY setting leaves the top 2
* priority bits empty for "kernel-aware" interrupts. This is the highest-
* possible priority (lowest possible numerical value) for the guaranteed
* 3 priority bits implemented in the NVIC.
*/
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (0xFFU >> 2)

#endif /* qf_int_config_h */