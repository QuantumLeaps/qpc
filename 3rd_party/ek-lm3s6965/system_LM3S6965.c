/*************************************************************************//**
 * @file     system_LM3S6965.c
 * @brief    CMSIS Cortex-M# Core Device System File for Device LM3S6965
 * @version  CMSIS v4.00
 * @date     07 March 2015
 *
 * @note     Created from the CMSIS template for the specified device
 *           Quantum Leaps, www.state-machine.com
 *
 ****************************************************************************/
/* Copyright (c) 2012 ARM LIMITED
 *
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - Neither the name of ARM nor the names of its contributors may be used
 *   to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ---------------------------------------------------------------------------*/

#include "LM3S6965.h" /* the device specific header (TI) */

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
#define CFG_RCC_SYSDIV 4

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
#define CFG_RCC_XTAL 14

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

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *--------------------------------------------------------------------------*/
uint32_t SystemCoreClock;  /*!< System Clock Frequency (Core Clock) */

/*----------------------------------------------------------------------------
* Define clocks
*/
static const uint32_t l_pulXtals[] = {
    1000000,
    1843200,
    2000000,
    2457600,
    3579545,
    3686400,
    4000000,
    4096000,
    4915200,
    5000000,
    5120000,
    6000000,
    6144000,
    7372800,
    8000000,
    8192000,
    10000000,
    12000000,
    12288000,
    13560000,
    14318180,
    16000000,
    16384000
};

/**
 * Update the SystemCoreClock variable based on internal regiser settings
 *
 * @param  none
 * @return none
 */
void SystemCoreClockUpdate (void) {
    uint32_t ulRCC, ulRCC2, ulPLL, ulClk;

    /* Read RCC and RCC2.  For Sandstorm-class devices (which do not have
    * RCC2), the RCC2 read will return 0, which indicates that RCC2 is
    * disabled (since the SYSCTL_RCC2_USERCC2 bit is clear).
    */
    ulRCC  = SYSCTL->RCC;
    ulRCC2 = SYSCTL->RCC2;

    /* Get the base clock rate */
    switch((ulRCC2 & 0x80000000) ?
           (ulRCC2 & 0x00000070) :
           (ulRCC  & 0x00000030))
    {
        /* The main oscillator is the clock source.  Determine its rate from
        * the crystal setting field.
        */
        case 0x00: {
            ulClk = l_pulXtals[(ulRCC & 0x000007C0) >> 6];
            break;
        }

        /* The internal oscillator is the source clock */
        case 0x10: {
            /* See if this is a Sandstorm-class or Fury-class device. */
            if (CLASS_IS_SANDSTORM) {
                /* The internal oscillator on a Sandstorm-class device is
                * 15 MHz +/- 50%.
                */
                ulClk = 15000000;
            }
            else if((CLASS_IS_FURY && REVISION_IS_A2) ||
                    (CLASS_IS_DUSTDEVIL && REVISION_IS_A0))
            {
                /* The internal oscillator on a rev A2 Fury-class device and
                * a rev A0 Dustdevil-class device is 12 MHz +/- 30%.
                */
                ulClk = 12000000;
            }
            else {
                /* The internal oscillator on all other devices is 16 MHz. */
                ulClk = 16000000;
            }
            break;
        }

        /* The internal oscillator divided by four is the source clock. */
        case 0x20: {
            /* See if this is a Sandstorm-class or Fury-class device. */
            if (CLASS_IS_SANDSTORM) {
                /* The internal oscillator on a Sandstorm-class device is
                * 15 MHz +/- 50%.
                */
                ulClk = 15000000 / 4;
            }
            else if ((CLASS_IS_FURY      && REVISION_IS_A2) ||
                     (CLASS_IS_DUSTDEVIL && REVISION_IS_A0))
            {
                /* The internal oscillator on a rev A2 Fury-class device and
                * a rev A0 Dustdevil-class device is 12 MHz +/- 30%.
                */
                ulClk = 12000000 / 4;
            }
            else {
                /* The internal oscillator on a Tempest-class device is 16MHz
                */
                ulClk = 16000000 / 4;
            }
            break;
        }

        /* The internal 30 KHz oscillator is the source clock. */
        case 0x30: {
            /* The internal 30 KHz oscillator has an accuracy of +/- 30%. */
            ulClk = 30000;
            break;
        }

        /* The 4.19 MHz clock from the hibernate module is the clock source */
        case 0x60: {
            ulClk = 4194304;
            break;
        }

        /* The 32 KHz clock from the hibernate module is the source clock. */
        case 0x70: {
            ulClk = 32768;
            break;
        }

        /* An unknown setting, so return a zero clock (that is, an unknown
        * clock rate).
        */
        default: {
            return;
        }
    }

    /* See if the PLL is being used. */
    if (((ulRCC2 & 0x80000000) && !(ulRCC2 & 0x00000800)) ||
       (!(ulRCC2 & 0x80000000) && !(ulRCC  & 0x00000800)))
    {
        /* Get the PLL configuration. */
        ulPLL = SYSCTL->PLLCFG;

        /* See if this is a Sandstorm-class or Fury-class device. */
        if (CLASS_IS_SANDSTORM) {
            /* Compute the PLL output frequency based on its input frequency.
            * The formula for a Sandstorm-class devices is
            * "(xtal * (f + 2)) / (r + 2)".
            */
            ulClk = ((ulClk * (((ulPLL & 0x00003FE0) >> 5) + 2)) /
                     (((ulPLL & 0x000001F) >> 0) + 2));
        }
        else {
            /* Compute the PLL output frequency based on its input frequency.
            * The formula for a Fury-class device is
            * "(xtal * f) / ((r + 1) * 2)".
            */
            ulClk = ((ulClk * ((ulPLL & 0x00003FE0) >> 5)) /
                     ((((ulPLL & 0x000001F) >> 0) + 1) * 2));
        }

        /* See if the optional output divide by 2 is being used. */
        if (ulPLL & 0x00004000) {
            ulClk /= 2;
        }

        /* See if the optional output divide by 4 is being used. */
        if (ulPLL & 0x00008000) {
            ulClk /= 4;
        }
    }

    /* See if the system divider is being used. */
    if (ulRCC & 0x00400000) {
        /* Adjust the clock rate by the system clock divider. */
        if (ulRCC2 & 0x80000000) {
            if ((ulRCC2   & 0x40000000) &&
               (((ulRCC2  & 0x80000000) &&
                 !(ulRCC2 & 0x00000800)) ||
                (!(ulRCC2 & 0x80000000) &&
                 !(ulRCC  & 0x00000800))))

            {
                ulClk = ((ulClk * 2) / (((ulRCC2 & (0x1F800000 | 0x00400000))
                                          >> (23 - 1)) + 1));
            }
            else {
                ulClk /= (((ulRCC2 & 0x1F800000) >> 23) + 1);
            }
        }
        else {
            ulClk /= (((ulRCC & 0x07800000) >> 23) + 1);
        }
    }

    SystemCoreClock = ulClk; /* Set the computed clock rate */
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
void SystemInit (void) {
#ifdef CLOCK_SETUP
    uint32_t volatile i;

    SYSCTL->RCC2 = 0x07802810; /* set default value */
    SYSCTL->RCC  = 0x078E3AD1; /* set default value */

    /* set value with BYPASS, PWRDN set, USESYSDIV reset */
    SYSCTL->RCC  = (RCC_Val  | (1UL << 11) | (1UL << 13)) & ~(1UL << 22);

    /* set value with BYPASS, PWRDN set */
    SYSCTL->RCC2 = (RCC2_Val | (1UL << 11) | (1UL << 13));
    for (i = 0; i < 1000; i++) {  /* wait a while */
        __NOP();
    }

    /* set value with BYPASS, USESYSDIV reset */
    SYSCTL->RCC  = (RCC_Val  | (1UL << 11)) & ~(1UL << 22);

    /* set value with BYPASS */
    SYSCTL->RCC2 = (RCC2_Val | (1UL << 11));
    for (i = 0; i < 1000; i++) {  /* wait a while */
        __NOP();
    }

    SYSCTL->RCC  = (RCC_Val | (1UL << 11)); /* set value with BYPASS */

    if ((((RCC_Val  & (1UL << 13)) == 0) && ((RCC2_Val & (1UL << 31)) ==0)) ||
        (((RCC2_Val & (1UL << 13)) == 0) && ((RCC2_Val & (1UL << 31)) !=0)))
    {
        /* wait until PLL is locked */
        while ((SYSCTL->RIS & (1UL << 6)) != (1UL << 6)) {
            __NOP();
        }
    }

    SYSCTL->RCC  = (RCC_Val);      /* set value */
    SYSCTL->RCC2 = (RCC2_Val);     /* set value */
    for (i = 0; i < 1000; i++) {   /* wait a while */
        __NOP();
    }
#endif /* #ifdef CLOCK_SETUP */

    /* Set the SystemCoreClock variable based on internal regiser settings */
    SystemCoreClockUpdate();
}

/****** End Of File *********************************************************/
