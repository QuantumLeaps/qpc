/*============================================================================
* QP/C Real-Time Embedded Framework (RTEF)
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
*
* This software is dual-licensed under the terms of the open source GNU
* General Public License version 3 (or any later version), or alternatively,
* under the terms of one of the closed source Quantum Leaps commercial
* licenses.
*
* The terms of the open source GNU General Public License version 3
* can be found at: <www.gnu.org/licenses/gpl-3.0>
*
* The terms of the closed source Quantum Leaps commercial licenses
* can be found at: <www.state-machine.com/licensing>
*
* Redistributions in source code must retain this top-level comment block.
* Plagiarizing this software to sidestep the license obligations is illegal.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
============================================================================*/
/*!
* @date Last updated on: 2022-05-13
* @version Last updated for: @ref qpcpp_7_0_1
*
* @file
* @brief QV/C port to ARM Cortex-M, IAR-ARM toolset
*/
/* This QV port is part of the interanl QP implementation */
#define QP_IMPL 1U
#include "qf_port.h"

#if (__ARM_ARCH == 6) /* ARMv6-M? */

/* hand-optimized quick LOG2 in assembly (no CLZ instruction in ARMv6-M) */
uint_fast8_t QF_qlog2(uint32_t x) {
    static uint8_t const log2LUT[16] = {
        0U, 1U, 2U, 2U, 3U, 3U, 3U, 3U,
        4U, 4U, 4U, 4U, 4U, 4U, 4U, 4U
    };
    uint_fast8_t n;
    __asm (
        "MOVS    %[n],#0\n"
#if (QF_MAX_ACTIVE > 16U)
        "LSRS    r2,r0,#16\n"
        "BEQ.N   QF_qlog2_1\n"
        "MOVS    %[n],#16\n"
        "MOVS    r0,r2\n"
    "QF_qlog2_1:\n"
#endif
#if (QF_MAX_ACTIVE > 8U)
        "LSRS    r2,r0,#8\n"
        "BEQ.N   QF_qlog2_2\n"
        "ADDS    %[n],%[n],#8\n"
        "MOVS    r0,r2\n"
    "QF_qlog2_2:\n"
#endif
        "LSRS    r2,r0,#4\n"
        "BEQ.N   QF_qlog2_3\n"
        "ADDS    %[n],%[n],#4\n"
        "MOVS    r0,r2\n"
    "QF_qlog2_3:"
        : [n]"=r"(n)
    );
    return n + log2LUT[x];
}

#else /* ARMv7-M or higher */

#define SCnSCB_ICTR  ((uint32_t volatile *)0xE000E004)
#define SCB_SYSPRI   ((uint32_t volatile *)0xE000ED14)
#define NVIC_IP      ((uint32_t volatile *)0xE000E400)

/*
* Initialize the exception priorities and IRQ priorities to safe values.
*
* Description:
* On Cortex-M3/M4/M7, this QV port disables interrupts by means of the
* BASEPRI register. However, this method cannot disable interrupt
* priority zero, which is the default for all interrupts out of reset.
* The following code changes the SysTick priority and all IRQ priorities
* to the safe value QF_BASEPRI, wich the QF critical section can disable.
* This avoids breaching of the QF critical sections in case the
* application programmer forgets to explicitly set priorities of all
* "kernel aware" interrupts.
*
* The interrupt priorities established in QV_init() can be later
* changed by the application-level code.
*/
void QV_init(void) {
    uint32_t n;

    /* set exception priorities to QF_BASEPRI...
    * SCB_SYSPRI1: Usage-fault, Bus-fault, Memory-fault
    */
    SCB_SYSPRI[1] = (SCB_SYSPRI[1]
        | (QF_BASEPRI << 16) | (QF_BASEPRI << 8) | QF_BASEPRI);

    /* SCB_SYSPRI2: SVCall */
    SCB_SYSPRI[2] = (SCB_SYSPRI[2] | (QF_BASEPRI << 24));

    /* SCB_SYSPRI3:  SysTick, PendSV, Debug */
    SCB_SYSPRI[3] = (SCB_SYSPRI[3]
        | (QF_BASEPRI << 24) | (QF_BASEPRI << 16) | QF_BASEPRI);

    /* set all implemented IRQ priories to QF_BASEPRI... */
    n = 8U + ((*SCnSCB_ICTR & 0x7U) << 3); /* (# NVIC_PRIO registers)/4 */
    do {
        --n;
        NVIC_IP[n] = (QF_BASEPRI << 24) | (QF_BASEPRI << 16)
                     | (QF_BASEPRI << 8) | QF_BASEPRI;
    } while (n != 0);
}

#endif /* ARMv7-M or higher */

