/*****************************************************************************
 *   clkconfig.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.08.20  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __CLKCONFIG_H
#define __CLKCONFIG_H

#define WDTCLK_SRC_IRC_OSC            0
#define WDTCLK_SRC_MAIN_CLK            1
#define WDTCLK_SRC_WDT_OSC            2

#define CLKOUTCLK_SRC_IRC_OSC        0
#define CLKOUTCLK_SRC_SYS_OSC        1
#define CLKOUTCLK_SRC_WDT_OSC        2
#define CLKOUTCLK_SRC_MAIN_CLK        3

void WDT_CLK_Setup(uint32_t timer_num);
void CLKOUT_Setup(uint32_t timer_num);
#endif /* end __CLKCONFIG_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
