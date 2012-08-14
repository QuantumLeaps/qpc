/*****************************************************************************
 *   gpio.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.09.01  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#ifndef __GPIO_H
#define __GPIO_H

#define PORT0        0
#define PORT1        1
#define PORT2        2
#define PORT3        3

void GPIO_IRQHandler(void);
void GPIOInit( void );
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir );
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal );
void GPIOSetInterrupt( uint32_t portNum, uint32_t bitPosi, uint32_t sense,
        uint32_t single, uint32_t event );
void GPIOIntEnable( uint32_t portNum, uint32_t bitPosi );
void GPIOIntDisable( uint32_t portNum, uint32_t bitPosi );
uint32_t GPIOIntStatus( uint32_t portNum, uint32_t bitPosi );
void GPIOIntClear( uint32_t portNum, uint32_t bitPosi );

#endif /* end __GPIO_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
