/*****************************************************************************
* Product: GNU linker script for Stellaris LM3S811 MCUs
* Last Updated for Version: 4.1.03
* Date of the Last Update:  Mar 03, 2010
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

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)                                         /* entry Point */

MEMORY {                                           /* memory map of LM3S811 */
    ROM (rx)  : ORIGIN = 0x00000000, LENGTH = 64K
    RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 8K
}

/* The size of the stack used by the application. NOTE: you need to adjust  */
STACK_SIZE = 1000;

/* The size of the heap used by the application. NOTE: you need to adjust   */
HEAP_SIZE = 0;

SECTIONS {

    .isr_vector : {                 /* the vector table goes FIRST into ROM */
        KEEP(*(.isr_vector))                                /* vector table */
        . = ALIGN(4);
    } >ROM

    .text : {                                         /* code and constants */
        . = ALIGN(4);
        *(.text)                                   /* .text sections (code) */
        *(.text*)                                 /* .text* sections (code) */
        *(.rodata)           /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)         /* .rodata* sections (constants, strings, etc.) */

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
    } >ROM

    .preinit_array : {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >ROM

    .init_array : {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >ROM

    .fini_array : {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(.fini_array*))
        KEEP (*(SORT(.fini_array.*)))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >ROM

    _etext = .;                            /* global symbols at end of code */

    .data :  AT (_etext) {
        __data_load = LOADADDR (.data);
        __data_start = .;
        *(.data)                                          /* .data sections */
        *(.data*)                                        /* .data* sections */
        . = ALIGN(4);
        __data_end__ = .;
        _edata = __data_end__;
    } >RAM

    .bss : {
        __bss_start__ = . ;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;                     /* define a global symbol at bss end */
        __bss_end__ = .;
    } >RAM

    PROVIDE ( end = _ebss );
    PROVIDE ( _end = _ebss );
    PROVIDE ( __end__ = _ebss );

    .heap : {
        __heap_start__ = . ;
        . = . + HEAP_SIZE;
        . = ALIGN(4);
        __heap_end__ = . ;
    } >RAM

    .stack : {
        __stack_start__ = . ;
        . = . + STACK_SIZE;
        . = ALIGN(4);
        __c_stack_top__ = . ;
        __stack_end__ = . ;
    } >RAM

    /* Remove information from the standard libraries */
    /DISCARD/ : {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }
}
