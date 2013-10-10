//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
//****************************************************************************
// Modified by Quantum Leaps on 09-Mar-2010
// www.state-machine.com
//
// - Added Stellaris DID0 register macros (Stellaris classes and versions)
// - Configured for EK-LM3S811 board:
//     PLL driven system clock at 20MHz (200 MHz / 10)
//     6 MHz oscillator
//****************************************************************************

//****************************************************************************
//
// lm3_config.h - Configuration header file for CMSIS application for
//                Luminary Micro LM3S Stellaris microcontrollers.
//
// Copyright (c) 2009 Luminary Micro, Inc.  All rights reserved.
// Software License Agreement
//
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
//
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws. All rights are reserved. You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
//
// THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS
// SOFTWARE. LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
// INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 32 of the Stellaris CMSIS Package.
//
//****************************************************************************

#ifndef __LM3S_CONFIG_H__
#define __LM3S_CONFIG_H__

//
// This file can be used by the Keil uVision configuration wizard to set
// the following system clock configuration values.  Or the value of the
// macros can be directly edited below if not using the uVision configuration
// wizard.
//
//--------------------- Clock Configuration ----------------------------------
//
//  <e> Clock Configuration
//          <i> Uncheck this box to skip the clock configuration.
//
// The following controls whether the system clock is configured in the
// SystemInit() function.  If it is defined to be 1 then the system clock
// will be configured according to the macros in the rest of this file.
// If it is defined to be 0, then the system clock configuration is bypassed.
//
#define CLOCK_SETUP 1

//********************************* RCC **************************************
//
//  <h> Run-Mode Clock Configuration (RCC)

//      <o> SYSDIV: System Clock Divisor <2-16>
//          <i> Specifies the divisor used to generate the system clock from
//          <i> either the PLL output of 200 MHz, or the chosen oscillator.
//
// The following value is the system clock divisor.  This will be applied if
// USESYSDIV (see below) is enabled.  The valid range of dividers is 2-16.
//
#define CFG_RCC_SYSDIV 10

//      <q> USESYSDIV: Enable System Clock Divider
//          <i> Check this box to use the System Clock Divider
//
// The following controls whether the system clock divider is used.  If the
// value is 1, then the system clock divider is used, and the value of the
// system divider is defined by SYSDIV (see above).  If the value is 0, then
// the system clock divider is not used.
//
#define CFG_RCC_USESYSDIV 1

//      <q> USEPWMDIV: Enable PWM Clock Divider
//          <i> Check this box to use the PWM Clock Divider
//
// The following controls whether the PWM clock divider is used.  If the
// value is 1, then the PWM clock divider is used, and the value of the
// PWM divider is defined by PWMDIV (see below).  If the value is 0, then
// the PWM clock divider is not used.
//
#define CFG_RCC_USEPWMDIV 0

//      <o> PWMDIV: PWM Unit Clock Divisor
//              <0=> 0: SysClk / 2
//              <1=> 1: SysClk / 4
//              <2=> 2: SysClk / 8
//              <3=> 3: SysClk / 16
//              <4=> 4: SysClk / 32
//              <5=> 5: SysClk / 64
//              <6=> 6: SysClk / 64
//              <7=> 7: SysClk / 64 (default)
//          <i> Specifies the divisor used to generate the PWM time base,
//          <i> from the System Clock
//
// The following value determines the PWM clock divider.  It is used if
// USEPWMDIV is enabled (see above).  Otherwise the PWM clock is the same as
// the system clock.  The value of the divider is determined by the table
// above.
//
#define CFG_RCC_PWMDIV 7

//      <q> PWRDN: PLL Power Down
//          <i> Check this box to disable the PLL.  You must also choose
//          <i> PLL Bypass.
//
// If the following value is 1, then the PLL is powered down.  Keep this value
// as 1 if you do not need to use the PLL.  In this case, BYPASS (see below)
// must also be set to 1.  If you are using the PLL, then this value must be
// set to 0.
//
#define CFG_RCC_PWRDN 0

//      <q> BYPASS: PLL Bypass
//          <i> Check this box to not use the PLL for the System Clock
//
// Set the following value to 1 to bypass the PLL and not use it for the
// system clock.  You must set this to 1 if PWRDN (above) is set to 1.  Set
// this to 0 if you are using the PLL.
//
#define CFG_RCC_BYPASS 0

//      <o> XTAL: Crystal Value
//              < 0=>  0: 1.0000 MHz  (can not be used with PLL)
//              < 1=>  1: 1.8432 MHz  (can not be used with PLL)
//              < 2=>  2: 2.0000 MHz  (can not be used with PLL)
//              < 3=>  3: 2.4576 MHz  (can not be used with PLL)
//              < 4=>  4: 3.579545 MHz
//              < 5=>  5: 3.6864 MHz
//              < 6=>  6: 4.0000 MHz
//              < 7=>  7: 4.096 MHz
//              < 8=>  8: 4.9152 MHz
//              < 9=>  9: 5.0000 MHz
//              <10=> 10: 5.12 MHz
//              <11=> 11: 6.0000 MHz (default)
//              <12=> 12: 6.144 MHz
//              <13=> 13: 7.3728 MHz
//              <14=> 14: 8.0000 MHz
//              <15=> 15: 8.192 MHz
//          <i> This is the crystal frequency used for the main oscillator
//
// This value defines the crystal frequency for the main oscillator, according
// to the table in the comments above.  If an external crystal is used, then
// this value must be set to match the value of the crystal.
//
#define CFG_RCC_XTAL 11

//      <o> OSCSRC: Oscillator Source
//              <0=> 0: MOSC Main oscillator
//              <1=> 1: IOSC Internal oscillator (default)
//              <2=> 2: IOSC/4 Internal oscillator / 4 (this is necessary
//                      if used as input to PLL)
//              <3=> 3: 30kHz 30-KHz internal oscillator
//          <i> Chooses the oscillator that is used for the system clock,
//          <i> or the PLL input.
//
// The following value chooses the oscillator source according to the table in
// the comments above.
//
#define CFG_RCC_OSCSRC 0

//      <q> IOSCDIS: Internal Oscillator Disable
//          <i> Check this box to turn off the internal oscillator
//
// Set the following value to 1 to turn off the internal oscillator.  This
// value can be set to 1 if you are not using the internal oscillator.
//
#define CFG_RCC_IOSCDIS 1

//      <q> MOSCDIS: Main Oscillator Disable
//          <i> Check this box to turn off the main oscillator
//
// Set the following value to 1 to turn off the main oscillator.  This
// value can be set to 1 if you are not using the main oscillator.
//
#define CFG_RCC_MOSCDIS 0

//  </h>

//********************************* RCC2 *************************************
//
//   <h> Run-Mode Clock Configuration 2 (RCC2)

//      <q> USERCC2: Use RCC2
//          <i> Check this box to override some fields in RCC.  RCC2 provides
//          <i> more bits for the system clock divider, and provides an
//          <i> additional oscillator source.  If you do not need these
//          <i> additional features, then leave this box unchecked.
//
// Set the following value to 1 to use the RCC2 register.  The RCC2 register
// overrides some of the fields in the RCC register if it is used.
//
#define CFG_RCC2_USERCC2 0

//      <o> SYSDIV2: System Clock Divisor <2-64>
//          <i> Specifies the divisor used to generate the system clock from
//          <i> either the PLL output of 200 MHz, or the oscillator.
//
// The following value is the system clock divisor.  This will be applied if
// USESYSDIV in RCC is enabled.  The valid range of dividers is 2-64.
//
#define CFG_RCC_SYSDIV2 2

//      <q> PWRDN2: Power Down PLL
//          <i> Check this box to disable the PLL.  You must also choose
//          <i> PLL Bypass.
//
// If the following value is 1, then the PLL is powered down.  Keep this value
// as 1 if you do not need to use the PLL.  In this case, BYPASS2 (see below)
// must also be set to 1.  If you are using the PLL, then this value must be
// set to 0.
//
#define CFG_RCC_PWRDN2 1

//      <q> BYPASS2: Bypass PLL
//          <i> Check this box to not use the PLL for the System Clock
//
// Set the following value to 1 to bypass the PLL and not use it for the
// system clock.  You must set this to 1 if PWRDN2 (above) is set to 1.  Set
// this to 0 if you are using the PLL.
//
#define CFG_RCC_BYPASS2 1

//      <o> OSCSRC2: Oscillator Source
//              <0=> 0: MOSC Main oscillator
//              <1=> 1: IOSC Internal oscillator (default)
//              <2=> 2: IOSC/4 Internal oscillator / 4 (this is necessary
//                      if used as input to PLL)
//              <3=> 3: 30kHz 30-kHz internal oscillator
//              <7=> 7: 32kHz 32.768-kHz external oscillator
//          <i> The oscillator that is used for the system clock, or the PLL
//              input.
//
// The following value chooses the oscillator source according to the table in
// the comments above.
//
#define CFG_RCC_OSCSRC2 1

//  </h>
//
//  </e>

//-------- <<< end of configuration section >>> ------------------------------

//
// The following macros are used to program the RCC and RCC2 registers in
// the SystemInit() function.  Edit the macros above to change these values.
//
#define RCC_Val                        \
(                                      \
    ((CFG_RCC_SYSDIV - 1)   << 23) |   \
    (CFG_RCC_USESYSDIV      << 22) |   \
    (CFG_RCC_USEPWMDIV      << 20) |   \
    (CFG_RCC_PWMDIV         << 17) |   \
    (CFG_RCC_PWRDN          << 13) |   \
    (CFG_RCC_BYPASS         << 11) |   \
    (CFG_RCC_XTAL           << 6)  |   \
    (CFG_RCC_OSCSRC         << 4)  |   \
    (CFG_RCC_IOSCDIS        << 1)  |   \
    (CFG_RCC_MOSCDIS        << 1)\
)

#define RCC2_Val                       \
(                                      \
    (CFG_RCC2_USERCC2      << 31) |    \
    ((CFG_RCC_SYSDIV2 - 1)  << 23) |   \
    (CFG_RCC_PWRDN2         << 13) |   \
    (CFG_RCC_BYPASS2        << 11) |   \
    (CFG_RCC_OSCSRC2        << 4)\
)

//
// The following macros are used to distinguish among various Stellaris
// device classes and version numbers based on the SYSCTL->DID0 register.
//

#define CLASS_IS_SANDSTORM (((SYSCTL->DID0 & 0x70000000) == 0x00000000) || \
                            ((SYSCTL->DID0 & 0x70FF0000) == 0x10000000))

#define CLASS_IS_FURY       ((SYSCTL->DID0 & 0x70FF0000) == 0x10010000)
#define CLASS_IS_DUSTDEVIL  ((SYSCTL->DID0 & 0x70FF0000) == 0x10030000)
#define CLASS_IS_TEMPEST    ((SYSCTL->DID0 & 0x70FF0000) == 0x10040000)

#define REVISION_IS_A0      ((SYSCTL->DID0 & 0x0000FFFF) == 0x00000000)
#define REVISION_IS_A1      ((SYSCTL->DID0 & 0x0000FFFF) == 0x00000001)
#define REVISION_IS_A2      ((SYSCTL->DID0 & 0x0000FFFF) == 0x00000002)

#define REVISION_IS_B0      ((SYSCTL->DID0 & 0x0000FFFF) == 0x00000100)
#define REVISION_IS_B1      ((SYSCTL->DID0 & 0x0000FFFF) == 0x00000101)

#define REVISION_IS_C1      ((SYSCTL->DID0 & 0x0000FFFF) == 0x00000201)
#define REVISION_IS_C2      ((SYSCTL->DID0 & 0x0000FFFF) == 0x00000202)

#endif
