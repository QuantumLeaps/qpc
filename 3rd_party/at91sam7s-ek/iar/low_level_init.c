/*****************************************************************************
* Product: AT91SAM7S low-level initialization, IAR-ARM toolset
* Last Updated for Version: 5.4.0
* Date of the Last Update:  2015-03-07
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. state-machine.com.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web  : https://state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
#include <stdint.h>
#include "AT91SAM7S64.h" /* AT91SAMT7S64 definitions */

/* adjust this constant if you change the clock speed */
#define MCK_FREQ  (18432000U * 73U / 14U / 2U)

__arm int __low_level_init(void); /* prototype */
uint32_t get_MCK_FREQ(void);      /* prototype */

/*............................................................................
* __low_level_init() is invoked by the standard IAR startup sequence after
* cstartup, but before initializing the segments in RAM. The function
* gives the application a chance to perform early initializations of
* the hardware. This function cannot use any static variables, because these
* have not yet been initialized in RAM.
*
* The value returned by __low_level_init() determines whether or not data
* segments should be initialized by __seqment_init. If __low_level_init()
* returns 0, the data segments will NOT be initialized. For more information
* see the "IAR ARM C/C++ Compiler Reference Guide".
*/
__arm int __low_level_init(void) {
    AT91PS_PMC pPMC;

    /* Set flash wait sate FWS and FMCN */
    AT91C_BASE_MC->MC_FMR = AT91C_MC_FWS_1FWS
        | ((AT91C_MC_FMCN) & ((MCK_FREQ + MCK_FREQ/2U)/1000000U << 16));

    AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;  /* Disable the watchdog */

    /* Enable the Main Oscillator:
    * set OSCOUNT to 6, which gives Start up time = 8 * 6 / SCK = 1.4ms
    * (SCK = 32768Hz)
    */
    pPMC = AT91C_BASE_PMC;
    pPMC->PMC_MOR = ((6U << 8) & AT91C_CKGR_OSCOUNT) | AT91C_CKGR_MOSCEN;
    while ((pPMC->PMC_SR & AT91C_PMC_MOSCS) == 0U) {   /* wait startup time */
    }

    /* Set the PLL and Divider from the 18.432MHz oscillator:
    * - div by 14 Fin  = (18.432 / 14) = 1.31657 MHz
    * - Mul 72+1: Fout = (1.316 * 73)  = 96.1097 MHz
    * - Div by 2: Mck  = 96.068 / 2    = 48.0550 MHz
    * for 48 MHz the error is 0.11%, which is good enough for USB
    * PLLCOUNT pll startup time estimate at : 0.844 ms
    * PLLCOUNT 28 = 0.000844/(1/32768)
    */
    pPMC->PMC_PLLR = ((AT91C_CKGR_DIV & 14U)
                      | (AT91C_CKGR_PLLCOUNT & (28U << 8))
                      | (AT91C_CKGR_MUL & (72U << 16)))
                      | (AT91C_CKGR_USBDIV & (1U << 28));
    while ((pPMC->PMC_SR & AT91C_PMC_LOCK) == 0U) {    /* wait startup time */
    }
    while ((pPMC->PMC_SR & AT91C_PMC_MCKRDY) == 0U) {
    }

    /* Select Master Clock and CPU Clock select the PLL clock / 2 */
    pPMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
    while ((pPMC->PMC_SR & AT91C_PMC_MCKRDY) == 0U) {
    }

    pPMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK;
    while ((pPMC->PMC_SR & AT91C_PMC_MCKRDY) == 0U) {
    }

    return 1;  /* proceed with the segment initialization */
}

/*..........................................................................*/
/* obtain the CPU clock speed set in low_level_init() */
uint32_t get_MCK_FREQ(void) {
    return MCK_FREQ;
}
