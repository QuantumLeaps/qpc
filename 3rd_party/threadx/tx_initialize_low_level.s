;/**************************************************************************/
;/*                                                                        */
;/*            Copyright (c) 1996-2011 by Express Logic Inc.               */
;/*                                                                        */
;/*  This software is copyrighted by and is the sole property of Express   */
;/*  Logic, Inc.  All rights, title, ownership, or other interests         */
;/*  in the software remain the property of Express Logic, Inc.  This      */
;/*  software may only be used in accordance with the corresponding        */
;/*  license agreement.  Any unauthorized use, duplication, transmission,  */
;/*  distribution, or disclosure of this software is expressly forbidden.  */
;/*                                                                        */
;/*  This Copyright notice may not be removed or modified without prior    */
;/*  written consent of Express Logic, Inc.                                */
;/*                                                                        */
;/*  Express Logic, Inc. reserves the right to modify this software        */
;/*  without notice.                                                       */
;/*                                                                        */
;/*  Express Logic, Inc.                     info@expresslogic.com         */
;/*  11423 West Bernardo Court               http://www.expresslogic.com   */
;/*  San Diego, CA  92127                                                  */
;/*                                                                        */
;/**************************************************************************/
;
;
;/**************************************************************************/
;/**************************************************************************/
;/**                                                                       */
;/** ThreadX Component                                                     */
;/**                                                                       */
;/**   Initialize                                                          */
;/**                                                                       */
;/**************************************************************************/
;/**************************************************************************/
;
;#define TX_SOURCE_CODE
;
;
;/* Include necessary system files.  */
;
;#include "tx_api.h"
;#include "tx_initialize.h"
;#include "tx_thread.h"
;#include "tx_timer.h"
;
;
        EXTERN  _tx_thread_system_stack_ptr
        EXTERN  _tx_initialize_unused_memory
        EXTERN  _tx_thread_context_save
        EXTERN  _tx_thread_context_restore
        EXTERN  _tx_timer_interrupt
        EXTERN  __iar_program_start
        EXTERN  __tx_SVCallHandler
        EXTERN  __tx_PendSVHandler
        EXTERN  __tx_vectors
        EXTERN  __iar_program_start
;
;
SYSTEM_CLOCK      EQU   150000000
SYSTICK_CYCLES    EQU   ((SYSTEM_CLOCK / 100) -1)

    RSEG    FREE_MEM:DATA
    PUBLIC  __tx_free_memory_start
__tx_free_memory_start
    DS32    4
;
;
        SECTION `.text`:CODE:NOROOT(2)
        THUMB
;/**************************************************************************/
;/*                                                                        */
;/*  FUNCTION                                               RELEASE        */
;/*                                                                        */
;/*    _tx_initialize_low_level                          Cortex-M4/IAR     */
;/*                                                           5.1          */
;/*  AUTHOR                                                                */
;/*                                                                        */
;/*    William E. Lamie, Express Logic, Inc.                               */
;/*                                                                        */
;/*  DESCRIPTION                                                           */
;/*                                                                        */
;/*    This function is responsible for any low-level processor            */
;/*    initialization, including setting up interrupt vectors, setting     */
;/*    up a periodic timer interrupt source, saving the system stack       */
;/*    pointer for use in ISR processing later, and finding the first      */
;/*    available RAM memory address for tx_application_define.             */
;/*                                                                        */
;/*  INPUT                                                                 */
;/*                                                                        */
;/*    None                                                                */
;/*                                                                        */
;/*  OUTPUT                                                                */
;/*                                                                        */
;/*    None                                                                */
;/*                                                                        */
;/*  CALLS                                                                 */
;/*                                                                        */
;/*    None                                                                */
;/*                                                                        */
;/*  CALLED BY                                                             */
;/*                                                                        */
;/*    _tx_initialize_kernel_enter           ThreadX entry function        */
;/*                                                                        */
;/*  RELEASE HISTORY                                                       */
;/*                                                                        */
;/*    DATE              NAME                      DESCRIPTION             */
;/*                                                                        */
;/*  10-10-2010     William E. Lamie         Initial Version 5.0           */
;/*  07-15-2011     William E. Lamie         Modified comment(s),          */
;/*                                            resulting in version 5.1    */
;/*                                                                        */
;/**************************************************************************/
;VOID   _tx_initialize_low_level(VOID)
;{
    PUBLIC  _tx_initialize_low_level
_tx_initialize_low_level:
;
;    /* Ensure that interrupts are disabled.  */
;

        CPSID   i                                   ; Disable interrupts
;
;
;    /* Set base of available memory to end of non-initialised RAM area.  */
;
        LDR     r0, =__tx_free_memory_start         ; Get end of non-initialized RAM area
        LDR     r2, =_tx_initialize_unused_memory   ; Build address of unused memory pointer
        STR     r0, [r2, #0]                        ; Save first free memory address
;
;    /* Enable the cycle count register.  */
;
        LDR r0, =0xE0001000                     ; Build address of DWT register
        LDR r1, [r0]                            ; Pickup the current value
        ORR r1, r1, #1                          ; Set the CYCCNTENA bit
        STR r1, [r0]                            ; Enable the cycle count register
;
;    /* Setup Vector Table Offset Register.  */
;
        MOV r0, #0xE000E000                     ; Build address of NVIC registers
        LDR r1, =__tx_vectors                   ; Pickup address of vector table
        STR r1, [r0, #0xD08]                    ; Set vector table address
;
;    /* Set system stack pointer from vector value.  */
;
        LDR r0, =_tx_thread_system_stack_ptr    ; Build address of system stack pointer
        LDR r1, =__tx_vectors                   ; Pickup address of vector table
        LDR r1, [r1]                            ; Pickup reset stack pointer
        STR r1, [r0]                            ; Save system stack pointer
;
;    /* Configure SysTick for 100Hz clock, or 16384 cycles if no reference.  */
;
        MOV r0, #0xE000E000                     ; Build address of NVIC registers
        LDR r1, =SYSTICK_CYCLES
        STR r1, [r0, #0x14]                     ; Setup SysTick Reload Value
        MOV r1, #0x7                            ; Build SysTick Control Enable Value
        STR r1, [r0, #0x10]                     ; Setup SysTick Control
;
;    /* Configure handler priorities.  */
;
        LDR r1, =0x00000000                     ; Rsrv, UsgF, BusF, MemM
        STR r1, [r0, #0xD18]                    ; Setup System Handlers 4-7 Priority Registers

        LDR r1, =0xFF000000                     ; SVCl, Rsrv, Rsrv, Rsrv
        STR r1, [r0, #0xD1C]                    ; Setup System Handlers 8-11 Priority Registers
                                                ; Note: SVC must be lowest priority, which is 0xFF

        LDR r1, =0x40FF0000                     ; SysT, PnSV, Rsrv, DbgM
        STR r1, [r0, #0xD20]                    ; Setup System Handlers 12-15 Priority Registers
                                                ; Note: PnSV must be lowest priority, which is 0xFF

        LDR r0, =0xE000EF34                     ; Pickup FPCCR
        LDR r1, [r0]                            ;
        LDR r2, =0x3FFFFFFF                     ; Build mask to clear ASPEN and LSPEN
        AND r1, r1, r2                          ; Clear the ASPEN and LSPEN bits
        STR r1, [r0]                            ; Update FPCCR
;
;    /* Return to caller.  */
;
        BX  lr
;}
;
;

    PUBLIC  __stack_test
__stack_test:
    push    {r0}
    push    {r1}
    push    {r2}
    push    {r3}
    pop     {r3}
    pop     {r2}
    pop     {r1}
    pop     {r0}
    bx      lr


;/* Define shells for each of the unused vectors.  */
;

    PUBLIC  __tx_IntHandler
__tx_IntHandler:
; VOID InterruptHandler (VOID)
; {
        PUSH    {lr}
        BL  _tx_thread_context_save

;    /* Do interrupt handler work here */
;    /* .... */

        B       _tx_thread_context_restore
; }

        PUBLIC  __tx_SysTickHandler
__tx_SysTickHandler:
; VOID TimerInterruptHandler (VOID)
; {
;
        PUSH    {lr}
        BL      _tx_thread_context_save
        MOV r0, #0xE000E000                     ; Build address of NVIC registers
        LDR r1, [r0, #0x10]                     ; Clear SysTick interrupt
        BL      _tx_timer_interrupt
        B       _tx_thread_context_restore
; }

        END


