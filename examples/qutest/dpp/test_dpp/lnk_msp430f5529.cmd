/* ============================================================================ */
/* Copyright (c) 2017, Texas Instruments Incorporated                           */
/*  All rights reserved.                                                        */
/*                                                                              */
/*  Redistribution and use in source and binary forms, with or without          */
/*  modification, are permitted provided that the following conditions          */
/*  are met:                                                                    */
/*                                                                              */
/*  *  Redistributions of source code must retain the above copyright           */
/*     notice, this list of conditions and the following disclaimer.            */
/*                                                                              */
/*  *  Redistributions in binary form must reproduce the above copyright        */
/*     notice, this list of conditions and the following disclaimer in the      */
/*     documentation and/or other materials provided with the distribution.     */
/*                                                                              */
/*  *  Neither the name of Texas Instruments Incorporated nor the names of      */
/*     its contributors may be used to endorse or promote products derived      */
/*     from this software without specific prior written permission.            */
/*                                                                              */
/*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" */
/*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,       */
/*  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      */
/*  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR            */
/*  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,       */
/*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,         */
/*  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; */
/*  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,    */
/*  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR     */
/*  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,              */
/*  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                          */
/* ============================================================================ */

/******************************************************************************/
/* lnk_msp430f5529.cmd - LINKER COMMAND FILE FOR LINKING MSP430F5529 PROGRAMS     */
/*                                                                            */
/*   Usage:  lnk430 <obj files...>    -o <out file> -m <map file> lnk.cmd     */
/*           cl430  <src files...> -z -o <out file> -m <map file> lnk.cmd     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* These linker options are for command line linking only.  For IDE linking,  */
/* you should set your linker options in Project Properties                   */
/* -c                                               LINK USING C CONVENTIONS  */
/* -stack  0x0100                                   SOFTWARE STACK SIZE       */
/* -heap   0x0100                                   HEAP AREA SIZE            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* Version: 1.203                                                             */
/*----------------------------------------------------------------------------*/

/****************************************************************************/
/* Specify the system memory map                                            */
/****************************************************************************/

MEMORY
{
    SFR                     : origin = 0x0000, length = 0x0010
    PERIPHERALS_8BIT        : origin = 0x0010, length = 0x00F0
    PERIPHERALS_16BIT       : origin = 0x0100, length = 0x0100
    RAM                     : origin = 0x2400, length = 0x2000
    USBRAM                  : origin = 0x1C00, length = 0x0800
    INFOA                   : origin = 0x1980, length = 0x0080
    INFOB                   : origin = 0x1900, length = 0x0080
    INFOC                   : origin = 0x1880, length = 0x0080
    INFOD                   : origin = 0x1800, length = 0x0080
    FLASH                   : origin = 0x4400, length = 0xBB80
    FLASH2                  : origin = 0x10000,length = 0x14400
    INT00                   : origin = 0xFF80, length = 0x0002
    INT01                   : origin = 0xFF82, length = 0x0002
    INT02                   : origin = 0xFF84, length = 0x0002
    INT03                   : origin = 0xFF86, length = 0x0002
    INT04                   : origin = 0xFF88, length = 0x0002
    INT05                   : origin = 0xFF8A, length = 0x0002
    INT06                   : origin = 0xFF8C, length = 0x0002
    INT07                   : origin = 0xFF8E, length = 0x0002
    INT08                   : origin = 0xFF90, length = 0x0002
    INT09                   : origin = 0xFF92, length = 0x0002
    INT10                   : origin = 0xFF94, length = 0x0002
    INT11                   : origin = 0xFF96, length = 0x0002
    INT12                   : origin = 0xFF98, length = 0x0002
    INT13                   : origin = 0xFF9A, length = 0x0002
    INT14                   : origin = 0xFF9C, length = 0x0002
    INT15                   : origin = 0xFF9E, length = 0x0002
    INT16                   : origin = 0xFFA0, length = 0x0002
    INT17                   : origin = 0xFFA2, length = 0x0002
    INT18                   : origin = 0xFFA4, length = 0x0002
    INT19                   : origin = 0xFFA6, length = 0x0002
    INT20                   : origin = 0xFFA8, length = 0x0002
    INT21                   : origin = 0xFFAA, length = 0x0002
    INT22                   : origin = 0xFFAC, length = 0x0002
    INT23                   : origin = 0xFFAE, length = 0x0002
    INT24                   : origin = 0xFFB0, length = 0x0002
    INT25                   : origin = 0xFFB2, length = 0x0002
    INT26                   : origin = 0xFFB4, length = 0x0002
    INT27                   : origin = 0xFFB6, length = 0x0002
    INT28                   : origin = 0xFFB8, length = 0x0002
    INT29                   : origin = 0xFFBA, length = 0x0002
    INT30                   : origin = 0xFFBC, length = 0x0002
    INT31                   : origin = 0xFFBE, length = 0x0002
    INT32                   : origin = 0xFFC0, length = 0x0002
    INT33                   : origin = 0xFFC2, length = 0x0002
    INT34                   : origin = 0xFFC4, length = 0x0002
    INT35                   : origin = 0xFFC6, length = 0x0002
    INT36                   : origin = 0xFFC8, length = 0x0002
    INT37                   : origin = 0xFFCA, length = 0x0002
    INT38                   : origin = 0xFFCC, length = 0x0002
    INT39                   : origin = 0xFFCE, length = 0x0002
    INT40                   : origin = 0xFFD0, length = 0x0002
    INT41                   : origin = 0xFFD2, length = 0x0002
    INT42                   : origin = 0xFFD4, length = 0x0002
    INT43                   : origin = 0xFFD6, length = 0x0002
    INT44                   : origin = 0xFFD8, length = 0x0002
    INT45                   : origin = 0xFFDA, length = 0x0002
    INT46                   : origin = 0xFFDC, length = 0x0002
    INT47                   : origin = 0xFFDE, length = 0x0002
    INT48                   : origin = 0xFFE0, length = 0x0002
    INT49                   : origin = 0xFFE2, length = 0x0002
    INT50                   : origin = 0xFFE4, length = 0x0002
    INT51                   : origin = 0xFFE6, length = 0x0002
    INT52                   : origin = 0xFFE8, length = 0x0002
    INT53                   : origin = 0xFFEA, length = 0x0002
    INT54                   : origin = 0xFFEC, length = 0x0002
    INT55                   : origin = 0xFFEE, length = 0x0002
    INT56                   : origin = 0xFFF0, length = 0x0002
    INT57                   : origin = 0xFFF2, length = 0x0002
    INT58                   : origin = 0xFFF4, length = 0x0002
    INT59                   : origin = 0xFFF6, length = 0x0002
    INT60                   : origin = 0xFFF8, length = 0x0002
    INT61                   : origin = 0xFFFA, length = 0x0002
    INT62                   : origin = 0xFFFC, length = 0x0002
    RESET                   : origin = 0xFFFE, length = 0x0002
}

/****************************************************************************/
/* Specify the sections allocation into memory                              */
/****************************************************************************/

SECTIONS
{
    .bss        : {} > RAM                  /* Global & static vars              */
    .data       : {} > RAM                  /* Global & static vars              */
    .TI.noinit  : {} > RAM                  /* For #pragma noinit                */
    .sysmem     : {} > RAM                  /* Dynamic memory allocation area    */
    .stack      : {} > RAM (HIGH)           /* Software system stack             */

#ifndef __LARGE_CODE_MODEL__
    .text       : {} > FLASH                /* Code                              */
#else
    .text       : {} >> FLASH2 | FLASH      /* Code                              */
#endif
    .text:_isr  : {} > FLASH                /* ISR Code space                    */
    .cinit      : {} > FLASH                /* Initialization tables             */
#ifndef __LARGE_DATA_MODEL__
    .const      : {} > FLASH                /* Constant data                     */
#else
    .const      : {} >> FLASH | FLASH2      /* Constant data                     */
#endif
    .cio        : {} > RAM                  /* C I/O Buffer                      */

    .pinit      : {} > FLASH                /* C++ Constructor tables            */
    .binit      : {} > FLASH                /* Boot-time Initialization tables   */
    .init_array : {} > FLASH                /* C++ Constructor tables            */
    .mspabi.exidx : {} > FLASH              /* C++ Constructor tables            */
    .mspabi.extab : {} > FLASH              /* C++ Constructor tables            */
#ifdef __TI_COMPILER_VERSION__
  #if __TI_COMPILER_VERSION__ >= 15009000
    #ifndef __LARGE_CODE_MODEL__
    .TI.ramfunc : {} load=FLASH, run=RAM, table(BINIT)
    #else
    .TI.ramfunc : {} load=FLASH | FLASH2, run=RAM, table(BINIT)
    #endif
  #endif
#endif

    .infoA     : {} > INFOA              /* MSP430 INFO FLASH Memory segments */
    .infoB     : {} > INFOB
    .infoC     : {} > INFOC
    .infoD     : {} > INFOD

    /* MSP430 Interrupt vectors          */
    .int00       : {}               > INT00
    .int01       : {}               > INT01
    .int02       : {}               > INT02
    .int03       : {}               > INT03
    .int04       : {}               > INT04
    .int05       : {}               > INT05
    .int06       : {}               > INT06
    .int07       : {}               > INT07
    .int08       : {}               > INT08
    .int09       : {}               > INT09
    .int10       : {}               > INT10
    .int11       : {}               > INT11
    .int12       : {}               > INT12
    .int13       : {}               > INT13
    .int14       : {}               > INT14
    .int15       : {}               > INT15
    .int16       : {}               > INT16
    .int17       : {}               > INT17
    .int18       : {}               > INT18
    .int19       : {}               > INT19
    .int20       : {}               > INT20
    .int21       : {}               > INT21
    .int22       : {}               > INT22
    .int23       : {}               > INT23
    .int24       : {}               > INT24
    .int25       : {}               > INT25
    .int26       : {}               > INT26
    .int27       : {}               > INT27
    .int28       : {}               > INT28
    .int29       : {}               > INT29
    .int30       : {}               > INT30
    .int31       : {}               > INT31
    .int32       : {}               > INT32
    .int33       : {}               > INT33
    .int34       : {}               > INT34
    .int35       : {}               > INT35
    .int36       : {}               > INT36
    .int37       : {}               > INT37
    .int38       : {}               > INT38
    .int39       : {}               > INT39
    .int40       : {}               > INT40
    RTC          : { * ( .int41 ) } > INT41 type = VECT_INIT
    PORT2        : { * ( .int42 ) } > INT42 type = VECT_INIT
    TIMER2_A1    : { * ( .int43 ) } > INT43 type = VECT_INIT
    TIMER2_A0    : { * ( .int44 ) } > INT44 type = VECT_INIT
    USCI_B1      : { * ( .int45 ) } > INT45 type = VECT_INIT
    USCI_A1      : { * ( .int46 ) } > INT46 type = VECT_INIT
    PORT1        : { * ( .int47 ) } > INT47 type = VECT_INIT
    TIMER1_A1    : { * ( .int48 ) } > INT48 type = VECT_INIT
    TIMER1_A0    : { * ( .int49 ) } > INT49 type = VECT_INIT
    DMA          : { * ( .int50 ) } > INT50 type = VECT_INIT
    USB_UBM      : { * ( .int51 ) } > INT51 type = VECT_INIT
    TIMER0_A1    : { * ( .int52 ) } > INT52 type = VECT_INIT
    TIMER0_A0    : { * ( .int53 ) } > INT53 type = VECT_INIT
    ADC12        : { * ( .int54 ) } > INT54 type = VECT_INIT
    USCI_B0      : { * ( .int55 ) } > INT55 type = VECT_INIT
    USCI_A0      : { * ( .int56 ) } > INT56 type = VECT_INIT
    WDT          : { * ( .int57 ) } > INT57 type = VECT_INIT
    TIMER0_B1    : { * ( .int58 ) } > INT58 type = VECT_INIT
    TIMER0_B0    : { * ( .int59 ) } > INT59 type = VECT_INIT
    COMP_B       : { * ( .int60 ) } > INT60 type = VECT_INIT
    UNMI         : { * ( .int61 ) } > INT61 type = VECT_INIT
    SYSNMI       : { * ( .int62 ) } > INT62 type = VECT_INIT
    .reset       : {}               > RESET  /* MSP430 Reset vector         */
}

/****************************************************************************/
/* Include peripherals memory map                                           */
/****************************************************************************/

-l msp430f5529.cmd

