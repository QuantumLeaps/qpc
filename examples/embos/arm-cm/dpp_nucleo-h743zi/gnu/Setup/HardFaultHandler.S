;/*********************************************************************
;*                   (c) SEGGER Microcontroller GmbH                  *
;*                        The Embedded Experts                        *
;*                           www.segger.com                           *
;**********************************************************************
;
;-------------------------- END-OF-HEADER -----------------------------
;
;File    : HardFaultHandler.S
;Purpose : HardFault exception handler for IAR, Keil and GNU assembler.
;          Evaluates used stack (MSP, PSP) and passes appropriate stack
;          pointer to the HardFaultHandler "C"-routine.
;*/

#ifndef __IAR_SYSTEMS_ASM__
  #ifndef __CC_ARM
    #ifndef __clang__
      #ifndef __GNUC__
        #error "Unsupported assembler!"
      #endif
    #endif
  #endif
#endif

;/*********************************************************************
;*
;*     Forward declarations of segments used
;*
;**********************************************************************
;*/

#if   (defined(__IAR_SYSTEMS_ASM__))

        SECTION CODE:CODE:NOROOT(2)
        SECTION CSTACK:DATA:NOROOT(3)

#elif (defined(__CC_ARM))

        AREA    OSKERNEL, CODE, READONLY, ALIGN=2
        PRESERVE8

#endif

;/*********************************************************************
;*
;*     Publics
;*
;**********************************************************************
;*/

#if   (defined(__IAR_SYSTEMS_ASM__))

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  HardFault_Handler

#elif (defined(__CC_ARM))

        EXPORT  HardFault_Handler

#elif (defined(__clang__) || defined(__GNUC__))

        .global HardFault_Handler
        .type   HardFault_Handler, function

#endif

;/*********************************************************************
;*
;*     Externals, code
;*
;**********************************************************************
;*/

#if   (defined(__IAR_SYSTEMS_ASM__))

        EXTERN  HardFaultHandler

#elif (defined(__CC_ARM))

        IMPORT  HardFaultHandler

#elif (defined(__clang__) || defined(__GNUC__))

        .extern HardFaultHandler

#endif

;/*********************************************************************
;*
;*     CODE segment
;*
;**********************************************************************
;*/

#if (defined(__clang__) || defined(__GNUC__))

        .syntax unified
        .thumb
        .balign 4
        .text

#else

        THUMB

#endif

;/*********************************************************************
;*
;*       Global functions
;*
;**********************************************************************
;*/

;/*********************************************************************
;*
;*      HardFault_Handler()
;*
;*  Function description
;*    Evaluates the used stack (MSP, PSP) and passes the appropiate
;*    stack pointer to the HardFaultHandler "C"-routine.
;*
;*  Notes
;*    (1) Ensure that HardFault_Handler is part of the exception table
;*/
#if (defined(__clang__) || defined(__GNUC__))
HardFault_Handler:
#else
HardFault_Handler
#endif
#if ((defined(__IAR_SYSTEMS_ASM__) && ((defined(__ARM6M__) && (__CORE__ == __ARM6M__)) || (defined(__ARM8M_BASELINE__) && (__CORE__ == __ARM8M_BASELINE__)))) \
  || (defined(__CC_ARM)            && ( defined(__TARGET_ARCH_6S_M)                    ||  defined(__ARM_ARCH_8M_BASE__)))                                    \
  || (defined(__clang__)           && ( defined(__ARM_ARCH_6M__)                       ||  defined(__ARM_ARCH_8M_BASE__)))                                    \
  || (defined(__GNUC__)            && ( defined(__ARM_ARCH_6M__)                       ||  defined(__ARM_ARCH_8M_BASE__))))
        ;// This version is for Cortex-M0/M0+/M1/M23
        movs   R0, #4
        mov    R1, LR
        tst    R0, R1            ;// Check EXC_RETURN in Link register bit 2.
        bne    Uses_PSP
        mrs    R0, MSP           ;// Stacking was using MSP.
        b      Pass_StackPtr
#if (defined(__clang__) || defined(__GNUC__))
Uses_PSP:
#else
Uses_PSP
#endif
        mrs    R0, PSP           ;// Stacking was using PSP.
#if (defined(__GNUC__) || defined(__clang__))
Pass_StackPtr:
#else
Pass_StackPtr
#endif
#else
        ;// This version is for Cortex-M3/M4/M4F/M7/M7F/M33
        tst    LR, #4            ;// Check EXC_RETURN in Link register bit 2.
        ite    EQ
        mrseq  R0, MSP           ;// Stacking was using MSP.
        mrsne  R0, PSP           ;// Stacking was using PSP.
#endif
#if (defined(__CC_ARM))
        ALIGN
#endif
        ldr    R1,=HardFaultHandler
        bx     R1                ;// Stack pointer passed through R0.

#if (defined(__clang__) || defined(__GNUC__))
        .end
#else
        END
#endif

;/****** End Of File *************************************************/
