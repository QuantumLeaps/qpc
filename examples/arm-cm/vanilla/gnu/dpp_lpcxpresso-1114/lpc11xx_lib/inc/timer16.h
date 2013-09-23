/*****************************************************************************
 *   timer16.h:  Header file for NXP LPC1xxx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TIMER16_H
#define __TIMER16_H

/* For 16-bit timer, make sure that TIME_INTERVAL should be no
greater than 0xFFFF. */

// CodeRed - variable name changed in CMSIS 1.3
//#define TIME_INTERVAL    (SystemAHBFrequency/1000 - 1)
#define TIME_INTERVAL    ((SystemCoreClock/LPC_SYSCON->SYSAHBCLKDIV)/100 - 1)
/* depending on the SystemFrequency and SystemAHBFrequency setting,
if SystemFrequency = 60Mhz, SystemAHBFrequency = 1/4 SystemAHBFrequency,
10mSec = 150.000-1 counts */

void delayMs(uint8_t timer_num, uint32_t delayInMs);
void TIMER16_0_IRQHandler(void);
void TIMER16_1_IRQHandler(void);
void enable_timer16(uint8_t timer_num);
void disable_timer16(uint8_t timer_num);
void reset_timer16(uint8_t timer_num);
void init_timer16(uint8_t timer_num, uint32_t timerInterval);

#endif /* end __TIMER16_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
