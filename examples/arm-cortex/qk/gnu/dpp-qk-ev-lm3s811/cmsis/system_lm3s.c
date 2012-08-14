/*****************************************************************************
* Modified by Quantum Leaps on 09-Mar-2010
* www.state-machine.com
*
* Rewrote SystemInit() to correctly handle various Stellaris device classes,
* such as: Sandstorm, Fury, DustDevil, and Tempest
*****************************************************************************/

/*****************************************************************************
*
* system_lm3s.c - System initialization file for CMSIS application on a
*                 Luminary Micro Stellaris microcontroller.
*
* Copyright (c) 2009 Luminary Micro, Inc.  All rights reserved.
* Software License Agreement
*
* Luminary Micro, Inc. (LMI) is supplying this software for use solely and
* exclusively on LMI's microcontroller products.
*
* The software is owned by LMI and/or its suppliers, and is protected under
* applicable copyright laws. All rights are reserved. You may not combine
* this software with "viral" open-source software in order to form a larger
* program.  Any use in violation of the foregoing restrictions may subject
* the user to criminal sanctions under applicable laws, as well as to civil
* liability for the breach of the terms and conditions of this license.
*
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS
* SOFTWARE. LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*
* This is part of revision 32 of the Stellaris CMSIS Package.
*****************************************************************************/

#include <stdint.h>
#include "lm3s_cmsis.h"
#include "lm3s_config.h"

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

/*----------------------------------------------------------------------------
*  Clock Variable definitions
*/
uint32_t SystemFrequency = 0;      /*!< System Clock Frequency (Core Clock) */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemFrequency variable.
 */
void SystemInit (void) {
#ifdef CLOCK_SETUP
    uint32_t volatile i;
    uint32_t ulRCC, ulRCC2, ulPLL, ulClk;

    SYSCTL->RCC2 = 0x07802810;                         /* set default value */
    SYSCTL->RCC  = 0x078E3AD1;                         /* set default value */

                       /* set value with BYPASS, PWRDN set, USESYSDIV reset */
    SYSCTL->RCC  = (RCC_Val  | (1UL << 11) | (1UL << 13)) & ~(1UL << 22);

                                        /* set value with BYPASS, PWRDN set */
    SYSCTL->RCC2 = (RCC2_Val | (1UL << 11) | (1UL << 13));
    for (i = 0; i < 1000; i++) {                            /* wait a while */
        __NOP();
    }
                                  /* set value with BYPASS, USESYSDIV reset */
    SYSCTL->RCC  = (RCC_Val  | (1UL << 11)) & ~(1UL << 22);

                                                   /* set value with BYPASS */
    SYSCTL->RCC2 = (RCC2_Val | (1UL << 11));
    for (i = 0; i < 1000; i++) {  /* wait a while */
        __NOP();
    }

    SYSCTL->RCC  = (RCC_Val | (1UL << 11));        /* set value with BYPASS */

    if ((((RCC_Val  & (1UL << 13)) == 0) && ((RCC2_Val & (1UL << 31)) ==0)) ||
        (((RCC2_Val & (1UL << 13)) == 0) && ((RCC2_Val & (1UL << 31)) !=0)))
    {
                                                /* wait until PLL is locked */
        while ((SYSCTL->RIS & (1UL << 6)) != (1UL << 6)) {
            __NOP();
        }
    }

    SYSCTL->RCC  = (RCC_Val);                                  /* set value */
    SYSCTL->RCC2 = (RCC2_Val);                                 /* set value */
    for (i = 0; i < 1000; i++) {                            /* wait a while */
        __NOP();
    }

#endif                                                /* #ifdef CLOCK_SETUP */

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

    /* Set the computed clock rate */
    SystemFrequency = ulClk;
}

