/******************************************************************************
 *   clkconfig.c:  clock configuration C file for NXP LPC13xx Family
 *   Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.08.20  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#include "LPC13xx.h"
#include "clkconfig.h"

/*****************************************************************************
** Function name:        WDT_CLK_Setup
**
** Descriptions:        Configure WDT clock.
** parameters:        clock source: irc_osc(0), main_clk(1), wdt_osc(2).
**
** Returned value:    None
**
*****************************************************************************/
void WDT_CLK_Setup ( uint32_t clksrc )
{
  /* Watchdog configuration. */
  /* Freq = 0.5Mhz, div_sel is 0, divided by 2. WDT_OSC should be 250khz */
  LPC_SYSCON->WDTOSCCTRL = (0x1<<5)|0x00;
  LPC_SYSCON->WDTCLKSEL = clksrc;        /* Select clock source */
  LPC_SYSCON->WDTCLKUEN = 0x01;          /* Update clock */
  LPC_SYSCON->WDTCLKUEN = 0x00;          /* Toggle update register once */
  LPC_SYSCON->WDTCLKUEN = 0x01;
  while ( !(LPC_SYSCON->WDTCLKUEN & 0x01) );  /* Wait until updated */
  LPC_SYSCON->WDTCLKDIV = 1;            /* Divided by 1 */
  LPC_SYSCON->PDRUNCFG &= ~(0x1<<6);    /* Let WDT clock run */
  return;
}

/*****************************************************************************
** Function name:        CLKOUT_Setup
**
** Descriptions:        Configure CLKOUT for reference clock check.
** parameters:        clock source: irc_osc(0), sys_osc(1), wdt_osc(2),
**                main_clk(3).
**
** Returned value:    None
**
*****************************************************************************/
void CLKOUT_Setup ( uint32_t clksrc )
{
  /* debug PLL after configuration. */
  LPC_SYSCON->CLKOUTCLKSEL = clksrc;    /* Select Main clock */
  LPC_SYSCON->CLKOUTUEN = 0x01;        /* Update clock */
  LPC_SYSCON->CLKOUTUEN = 0x00;        /* Toggle update register once */
  LPC_SYSCON->CLKOUTUEN = 0x01;
  while ( !(LPC_SYSCON->CLKOUTUEN & 0x01) );        /* Wait until updated */
  LPC_SYSCON->CLKOUTDIV = 1;            /* Divided by 1 */
  return;
}

