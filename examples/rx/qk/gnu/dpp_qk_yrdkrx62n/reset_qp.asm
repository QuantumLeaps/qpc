/*------------------------------------------------------------------------
!! This file has been modified for QP by Quantum Leaps from the
   generated file 'reset_program.asm'. To avoid confusion, this
   file has been renamed to 'reset_qp.asm'.

   1. removed initialization of the user stack (unused in QP)
   2. removed switching to User Mode (left in Supervisro Mode)
   3. removed enabling interrupts (left interrupts disabled)

   DATE: Mar 06,2012
------------------------------------------------------------------------*/

    /*reset_program.asm*/

    .list
    .section .text
    .global _PowerON_Reset    /*global Start routine */

    .extern _HardwareSetup  /*external Sub-routine to initialise Hardware*/
    .extern _data
    .extern _mdata
    .extern _ebss
    .extern _bss
    .extern _edata
    .extern _main
    //.extern _ustack     // user stack not used in QP
    .extern _istack
    .extern _rvectors
#if GDB_SIMULATOR_DEBUG
    .extern _exit
#endif


_PowerON_Reset :
/* initialise user stack pointer */
    //mvtc    #_ustack,USP
    mvtc    #00h,USP   // user stack not used in QP

/* initialise interrupt stack pointer */
    mvtc    #_istack,ISP

/* setup intb */
    mvtc    #_rvectors_start, intb    /* INTERRUPT VECTOR ADDRESS  definition    */

/* setup FPSW */
    mvtc    #100h, fpsw

/* load data section from ROM to RAM */

    mov     #_mdata,r2      /* src ROM address of data section in R2 */
    mov     #_data,r1       /* dest start RAM address of data section in R1 */
    mov     #_edata,r3      /* end RAM address of data section in R3 */
    sub    r1,r3            /* size of data section in R3 (R3=R3-R1) */
    smovf                   /* block copy R3 bytes from R2 to R1 */

/* bss initialisation : zero out bss */

    mov    #00h,r2      /* load R2 reg with zero */
    mov    #_ebss, r3  /* store the end address of bss in R3 */
    mov    #_bss, r1     /* store the start address of bss in R1 */
    sub   r1,r3           /* size of bss section in R3 (R3=R3-R1) */
    sstr.b
#ifndef GDB_SIMULATOR_DEBUG
/* call the hardware initialiser */
    bsr.a    _HardwareSetup
    nop

/* setup PSW */
    //mvtc    #10000h, psw    // do NOT enable interrupts

#ifdef CPPAPP
    bsr.a    __rx_init
#endif
#endif
/* start user program */
    bsr.a    _main
    bsr.a     _exit

#ifdef CPPAPP
    .global    _rx_run_preinit_array
    .type    _rx_run_preinit_array,@function
_rx_run_preinit_array:
    mov    #__preinit_array_start,r1
    mov    #__preinit_array_end,r2
    bra.a    _rx_run_inilist

    .global    _rx_run_init_array
    .type    _rx_run_init_array,@function
_rx_run_init_array:
    mov    #__init_array_start,r1
    mov    #__init_array_end,r2
    mov    #4, r3
    bra.a    _rx_run_inilist

    .global    _rx_run_fini_array
    .type    _rx_run_fini_array,@function
_rx_run_fini_array:
    mov    #__fini_array_start,r2
    mov    #__fini_array_end,r1
    mov    #-4, r3
    /* fall through */

_rx_run_inilist:
next_inilist:
    cmp    r1,r2
    beq.b    done_inilist
    mov.l    [r1],r4
    cmp    #-1, r4
    beq.b    skip_inilist
    cmp    #0, r4
    beq.b    skip_inilist
    pushm    r1-r3
    jsr    r4
    popm    r1-r3
skip_inilist:
    add    r3,r1
    bra.b    next_inilist
done_inilist:
    rts

    .section    .init,"ax"
    .balign 4

    .global __rx_init
__rx_init:

    .section    .fini,"ax"
    .balign 4

    .global __rx_fini
__rx_fini:
    bsr.a    _rx_run_fini_array

        .section .sdata
        .balign 4
        .global __gp
    .weak   __gp
__gp:

    .section .data
    .global ___dso_handle
    .weak   ___dso_handle
___dso_handle:
    .long    0

     .section        .init,"ax"
     bsr.a   _rx_run_preinit_array
     bsr.a   _rx_run_init_array
     rts

    .global __rx_init_end
__rx_init_end:

    .section        .fini,"ax"

    rts
    .global __rx_fini_end
__rx_fini_end:

#endif

/* call to exit*/
_exit:
    bra  _loop_here
_loop_here:
    bra _loop_here

    .text
    .end
