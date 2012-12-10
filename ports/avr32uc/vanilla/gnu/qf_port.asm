//////////////////////////////////////////////////////////////////////////////
// Adapted from the ASF file "exception.S" for the QP port, Vanilla kernel
//
// Changes:
//
// * modified the exception handlers to jump to the QP assertion handler
//   Q_onAssert() instead of getting stuck in endless loops.
//
// * modified the interrupt hanlders (_int0, _int1, _int2, _int3) to call
//   user-defined interrupt service routines (ISRs) as pure C functions,
//   instead of __attribute__((__interrupt__)) functions.
//
// Sep 24, 2012
// Quantum Leaps, LLC
// www.state-machine.com
//////////////////////////////////////////////////////////////////////////////
/**
 * \file
 *
 * \brief Exception and interrupt vectors mapping for the INTC Software Driver.
 *
 * Copyright (c) 2009-2011 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#if !__AVR32_UC__
  #error Implementation for the AVR32UC architecture only.
#endif

#include <avr32/io.h>

//! @{
//! \verbatim

.section  .exception, "ax", @progbits

//////////////////////////////////////////////////////////////////////////////
// Start of Exception Vector Table
//
// EVBA must be aligned with a power of two strictly greater than the
// EVBA-relative offset of the last vector.
//
.balign 0x200

// Export symbol
.global _evba
.type   _evba, @function
_evba:

.org  0x000       // Unrecoverable Exception
    bral _handle_Unrecoverable_Exception

.org  0x004       // TLB Multiple Hit
    bral _handle_TLB_Multiple_Hit

.org  0x008       // Bus Error Data Fetch
    bral _handle_Bus_Error_Data_Fetch

.org  0x00C        // Bus Error Instruction Fetch
    bral _handle_Bus_Error_Instruction_Fetch

.org  0x010       // NMI
    bral _handle_NMI:

.org  0x014       // Instruction Address
    bral _handle_Instruction_Address

.org  0x018       // ITLB Protection
    bral _handle_ITLB_Protection

.org  0x01C       // Breakpoint
    bral _handle_Breakpoint

.org  0x020       // Illegal Opcode
    bral _handle_Illegal_Opcode

.org  0x024       // Unimplemented Instruction
    bral _handle_Unimplemented_Instruction

.org  0x028       // Privilege Violation
    bral _handle_Privilege_Violation

.org  0x02C       // Floating-Point: UNUSED IN AVR32UC and AVR32AP
    bral _handle_Floating_Point

.org  0x030        // Coprocessor Absent: UNUSED IN AVR32UC
    bral _handle_Coprocessor_Absent

.org  0x034       // Data Address (Read)
    bral _handle_Data_Address_Read

.org  0x038       // Data Address (Write)
    bral _handle_Data_Address_Write

.org  0x03C       // DTLB Protection (Read)
    bral _handle_DTLB_Protection_Read

.org  0x040       // DTLB Protection (Write)
    bral _handle_DTLB_Protection_Write

.org  0x044       // DTLB Modified: UNUSED IN AVR32UC
    bral _handle_DTLB_Modified

.org  0x050       // ITLB Miss
    bral _handle_ITLB_Miss

.org  0x060       // DTLB Miss (Read)
    bral _handle_DTLB_Miss_Read

.org  0x070       // DTLB Miss (Write)
    bral _handle_DTLB_Miss_Write

.org  0x100       // Supervisor Call
    bral _handle_Supervisor_Call


.balign 4

_handle_Unrecoverable_Exception:
    mov     R11, 0x000
    bral    call_onAssert

_handle_TLB_Multiple_Hit:
    mov     R11, 0x004
    bral    call_onAssert

_handle_Bus_Error_Data_Fetch:
    mov     R11, 0x008
    bral    call_onAssert

_handle_Bus_Error_Instruction_Fetch:
    mov     R11, 0x00C
    bral    call_onAssert

_handle_NMI:
    mov     R11, 0x010
    bral    call_onAssert

_handle_Instruction_Address:
    mov     R11, 0x014
    bral    call_onAssert

_handle_ITLB_Protection:
    mov     R11, 0x018
    bral    call_onAssert

_handle_Breakpoint:
    mov     R11, 0x01C
    bral    call_onAssert

_handle_Illegal_Opcode:
    mov     R11, 0x020
    bral    call_onAssert

_handle_Unimplemented_Instruction:
    mov     R11, 0x024
    bral    call_onAssert

_handle_Privilege_Violation:
    mov     R11, 0x028
    bral    call_onAssert

_handle_Floating_Point:
    mov     R11, 0x02C
    bral    call_onAssert

_handle_Coprocessor_Absent:
    mov     R11, 0x030
    bral    call_onAssert

_handle_Data_Address_Read:
    mov     R11, 0x034
    bral    call_onAssert

_handle_Data_Address_Write:
    mov     R11, 0x038
    bral    call_onAssert

_handle_DTLB_Protection_Read:
    mov     R11, 0x03C
    bral    call_onAssert

_handle_DTLB_Protection_Write:
    mov     R11, 0x040
    bral    call_onAssert

_handle_DTLB_Modified:
    mov     R11, 0x044
    bral    call_onAssert

_handle_ITLB_Miss:
    mov     R11, 0x050
    bral    call_onAssert

_handle_DTLB_Miss_Read:
    mov     R11, 0x060
    bral    call_onAssert

_handle_DTLB_Miss_Write:
    mov     R11, 0x070
    bral    call_onAssert

_handle_Supervisor_Call:
    mov     R11, 0x100
    bral    call_onAssert

.extern Q_onAssert         // QP assertion handler

call_onAssert:
    lddpc   R12, exc_ptr   // pointer to the "EXCEPTION\0" string
    bral    Q_onAssert     // jump to the QP assertion handler

exc_ptr:
    .word   exc_str
exc_str:
    .ascii  "EXCEPTION\0"

//////////////////////////////////////////////////////////////////////////////
// Low-lever interrupt handlers (_int0, _int1, _int2, and _int3):
//
// * The AVR32UC CPU pushes automatically R8-R12, LR, PC, and SR onto the
//   system stack upon interrupt entry. Therefore any of these registers
//   can be used safely in the interrupt handler. The handlers can also
//   safely call C functions, which can clobber R8-R12, LR, PC, and SR.
//
// * All interrupt handlders call the C function _get_interrupt_handler(),
//   which takes the current priority level (\priority) in R12 and returns
//   (in R12) a pointer to a user-defined interrupt service routine (ISR).
//
// * The user-defined ISR is called as a regular C function and returns
//   to this handler, which then returns with the 'rete' instruction.
//
.balign 4
.global _int0
.global _int1
.global _int2
.global _int3

.type   _int0, @function
.type   _int1, @function
.type   _int2, @function
.type   _int3, @function

_int0:
    mov     R12, 0                  // the paramter for _get_interrupt_handler
    bral    call_get_interrupt_handler

_int1:
    mov     R12, 1                  // the paramter for _get_interrupt_handler
    bral    call_get_interrupt_handler

_int2:
    mov     R12, 2                  // the paramter for _get_interrupt_handler
    bral    call_get_interrupt_handler

_int3:
    mov     R12, 3                  // the paramter for _get_interrupt_handler

call_get_interrupt_handler:
    rcall   _get_interrupt_handler  // return the pointer to the ISR in R12
    breq    int_done                // branch if return value (R12) == 0

    icall   R12                     // call the user ISR (normal C function!)

int_done:
    rete

//! \endverbatim
//! @}
