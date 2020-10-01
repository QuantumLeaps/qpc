/**
* @file
* @brief QV/C port to ARM Cortex-M, GNU-ARM toolset
* @cond
******************************************************************************
* Last updated for version 6.9.1
* Last updated on  2020-09-23
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
/* This QV port is part of the interanl QP implementation */
#define QP_IMPL 1U
#include "qf_port.h"

#if (__ARM_ARCH == 6) /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */

/*
* Hand-optimized quick LOG2 in assembly (M0/M0+ have no CLZ instruction)
*
* NOTE:
* The inline GNU assembler does not accept mnemonics MOVS, LSRS and ADDS,
* but for Cortex-M0/M0+/M1 the mnemonics MOV, LSR and ADD always set the
* condition flags in the PSR.
*/
__attribute__ ((naked, optimize("-fno-stack-protector")))
uint_fast8_t QF_qlog2(uint32_t x) {
__asm volatile (
    "  MOV     r1,#0            \n"
#if (QF_MAX_ACTIVE > 16U)
    "  LSR     r2,r0,#16        \n"
    "  BEQ     QF_qlog2_1       \n"
    "  MOV     r1,#16           \n"
    "  MOV     r0,r2            \n"
    "QF_qlog2_1:                \n"
#endif
#if (QF_MAX_ACTIVE > 8U)
    "  LSR     r2,r0,#8         \n"
    "  BEQ     QF_qlog2_2       \n"
    "  ADD     r1, r1,#8        \n"
    "  MOV     r0, r2           \n"
    "QF_qlog2_2:                \n"
#endif
    "  LSR     r2,r0,#4         \n"
    "  BEQ     QF_qlog2_3       \n"
    "  ADD     r1,r1,#4         \n"
    "  MOV     r0,r2            \n"
    "QF_qlog2_3:                \n"
    "  LDR     r2,=QF_qlog2_LUT \n"
    "  LDRB    r0,[r2,r0]       \n"
    "  ADD     r0,r1,r0         \n"
    "  BX      lr               \n"
    "  .align                   \n"
    "QF_qlog2_LUT:              \n"
    "  .byte 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4"
    );
}

#else /* NOT Cortex-M0/M0+/M1(v6-M, v6S-M)? */

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
    SCB_SYSPRI[1] |= (QF_BASEPRI << 16) | (QF_BASEPRI << 8) | QF_BASEPRI;

    /* SCB_SYSPRI2: SVCall */
    SCB_SYSPRI[2] |= (QF_BASEPRI << 24);

    /* SCB_SYSPRI3:  SysTick, PendSV, Debug */
    SCB_SYSPRI[3] |= (QF_BASEPRI << 24) | (QF_BASEPRI << 16) | QF_BASEPRI;

    /* set all implemented IRQ priories to QF_BASEPRI... */
    n = 8U + ((*SCnSCB_ICTR & 0x7U) << 3); /* (# NVIC_PRIO registers)/4 */
    do {
        --n;
        NVIC_IP[n] = (QF_BASEPRI << 24) | (QF_BASEPRI << 16)
                     | (QF_BASEPRI << 8) | QF_BASEPRI;
    } while (n != 0);
}

#endif /* NOT Cortex-M0/M0+/M1(v6-M, v6S-M)? */

