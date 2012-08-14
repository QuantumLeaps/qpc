/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name       : resetprg.c
* Version      : 1.0
* Device(s)    : RX62x
* Description  : Defines post-reset routines that are used to configure the MCU prior to the main program starting.
*                This is were the program counter starts on power-up or reset.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 26.10.2011 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

#include <machine.h>     /* Defines machine level functions used in this file */
#include <stdint.h>
#include "iodefine.h"
#include "stacksct.h"    /* Defines the size of the stack which configured in this file */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define PSW_init  (0x00000000)
#define FPSW_init (0x00000100)

#define RUN_IN_USER_MODE    (0)
/***********************************************************************************************************************
Pre-processor Directives
***********************************************************************************************************************/
/* Declare the contents of the function 'Change_PSW_PM_to_UserMode' as
   assembler to the compiler */
#pragma inline_asm Change_PSW_PM_to_UserMode

/* Set this as the entry point from a power-on reset */
#pragma entry PowerON_Reset_PC

/***********************************************************************************************************************
External function Prototypes
***********************************************************************************************************************/
/* Functions to setup I/O library */
//extern void _INIT_IOLIB(void);
//extern void _CLOSEALL(void);

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* Power-on reset function declaration */
void PowerON_Reset_PC(void);

#if RUN_IN_USER_MODE==1
    #if __RENESAS_VERSION__ < 0x01010000
    /* MCU usermode switcher function declaration */
    static void Change_PSW_PM_to_UserMode(void);
    #endif
#endif

/* Main program function declaration */
void main(void);
static void operating_frequency_set(void);

/***********************************************************************************************************************
* Function name: PowerON_Reset_PC
* Description  : This function is the MCU's entry point from a power-on reset.
*                The following steps are taken in the startup code:
*                1. The User Stack Pointer (USP) and Interrupt Stack Pointer (ISP) are both set immediately after entry
*                   to this function. The USP and ISP stack sizes are set in the file stacksct.h.
*                   Default sizes are USP=4K and ISP=1K.
*                2. The interrupt vector base register is set to point to the beginning of the relocatable interrupt
*                   vector table.
*                3. The MCU is setup for floating point operations by setting the initial value of the Floating Point
*                   Status Word (FPSW).
*                4. The MCU operating frequency is set by configuring the Clock Generation Circuit (CGC) in
*                   operating_frequency_set.
*                5. Calls are made to functions to setup the C runtime environment which involves initializing all
*                   initialed data, zeroing all uninitialized variables, and configuring STDIO if used
*                   (calls to _INITSCT and _INIT_IOLIB).
*                6. Board-specific hardware setup, including configuring I/O pins on the MCU, in hardware_setup.
*                7. Global interrupts are enabled by setting the I bit in the Program Status Word (PSW), and the stack
*                   is switched from the ISP to the USP.  The initial Interrupt Priority Level is set to zero, enabling
*                   any interrupts with a priority greater than zero to be serviced.
*                8. The processor is optionally switched to user mode.  To run in user mode, set the macro
*                   RUN_IN_USER_MODE above to a 1.
*                9. The bus error interrupt is enabled to catch any accesses to invalid or reserved areas of memory.
*
*                Once this initialization is complete, the user's main() function is called.  It should not return.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void PowerON_Reset_PC(void)
{
    /* Stack pointers are setup prior to calling this function - see comments above */

    /* Initialise the MCU processor word */
#if __RENESAS_VERSION__ >= 0x01010000
    set_intb((void *)__sectop("C$VECT"));
#else
    set_intb((unsigned long)__sectop("C$VECT"));
#endif

    /* Initialize FPSW for floating-point operations */
#ifdef __ROZ
#define _ROUND 0x00000001  /* Let FPSW RMbits=01 (round to zero) */
#else
#define _ROUND 0x00000000  /* Let FPSW RMbits=00 (round to nearest) */
#endif
#ifdef __DOFF
#define _DENOM 0x00000100  /* Let FPSW DNbit=1 (denormal as zero) */
#else
#define _DENOM 0x00000000  /* Let FPSW DNbit=0 (denormal as is) */
#endif
    set_fpsw(FPSW_init | _ROUND | _DENOM);

    /* Switch to high-speed operation */
    operating_frequency_set();

    /* Initialize C runtime environment */
    _INITSCT();

    /* Comment this out if not using I/O lib */
    //_INIT_IOLIB();

    /* Configure the MCU and YRDK hardware */
    //hardware_setup();

    /* Change the MCU's usermode from supervisor to user */
    nop();
    set_psw(PSW_init);
#if RUN_IN_USER_MODE==1
    /* Use chg_pmusr() intrinsic if possible. */
    #if __RENESAS_VERSION__ >= 0x01010000
    chg_pmusr() ;
    #else
    Change_PSW_PM_to_UserMode();
    #endif
#endif

    /* Enable the bus error interrupt to catch accesses to illegal/reserved areas of memory */
    /* The ISR for this interrupt can be found in vecttbl.c in the function "BusErrorISR" */
    /* Clear any pending interrupts */
    IR(BSC,BUSERR) = 0;
    /* Make this the highest priority interrupt (adjust as necessary for your application */
    IPR(BSC,BUSERR) = 0x0F;
    /* Enable the interrupt in the ICU*/
    IEN(BSC,BUSERR) = 1;
    /* Enable illegal address interrupt in the BSC */
    BSC.BEREN.BIT.IGAEN = 1;

    /* Call the main program function (should not return) */
    main();

    /* Comment this out if not using I/O lib - cleans up open files */
    //_CLOSEALL();

    /* Invoke a break interrupt */
    brk();
}

/***********************************************************************************************************************
* Function name: operating_frequency_set
* Description  : Configures the clock settings for each of the device clocks
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void operating_frequency_set(void)
{

    /*
    Clock Description              Frequency
    ----------------------------------------
    Input Clock Frequency..............12MHz
    Internal Clock Frequency...........96MHz
    Peripheral Clock Frequency.........48MHz
    External Bus Clock Frequency.......12MHz */

    /* SCKCR - System Clock Control Register
    b27:b23 ICK[3:0] = 0, system clock = EXTAL * 8 = 96 MHz
    b23     PSTOP1 = 0, BCLK pin output disabled
    b22     PSTOP0 = 0, SDCLK pin output disabled
    b19:b16 BCK[3:0] = 3, BCLK set to lowest multiplier (x1)to reduce power
    b11:b8  PCK[3:0] = 1, PCK = EXTAL * 4 = 48 MHz
    */
    SYSTEM.SCKCR.LONG = (unsigned long)0x00030100;

}

/***********************************************************************************************************************
* Function name: Change_PSW_PM_to_UserMode
* Description  : Assembler function, used to change the MCU's usermode from supervisor to user.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
#if RUN_IN_USER_MODE==1
    #if __RENESAS_VERSION__ < 0x01010000
static void Change_PSW_PM_to_UserMode(void)
{
    MVFC   PSW,R1
    OR     #00100000h,R1
    PUSH.L R1
    MVFC   PC,R1
    ADD    #10,R1
    PUSH.L R1
    RTE
    NOP
    NOP
}
    #endif
#endif