/***********************************************************************/
/*                                                                     */
/*  FILE        :intprg.c                                              */
/*  DATE        :Fri, Apr 13, 2007                                     */
/*  DESCRIPTION :define the top address of the interrupt vectors.      */
/*  CPU GROUP   :26A                                                   */
/*                                                                     */
/*  *NOTE*                                                             */
/*  This file is excluded from the project because it is meant to be   */
/*  a template.  When the user wants to setup an interrupt, they can   */
/*  come to this file and copy the interrupt they want out of it.      */
/*                                                                     */
/***********************************************************************/

/***************************************************************************
 *  COMPILER for M16C/60
 *  Copyright(c) 2004 Renesas Technology Corp.
 *  And Renesas Solutions Corp.,All Rights Reserved.
 *  intprg.c : define variable vector table for M16C/26A,M16C/26T
 *
 *
 * $Date: 2006/09/13 05:32:05 $
 * $Revision: 1.6 $
 **************************************************************************/
#include "sfr26a.h"
#include "cstartdef.h"

// BRK                (software int 0)
#pragma interrupt    _brk(vect=0)
void _brk(void){}

// vector 1 reserved
// vector 2 reserved
// vector 3 reserved

// INT3                (software int 4)
#pragma interrupt    _int3(vect=4)
void _int3(void){}

// vector 5 reserved
// vector 6 reserved
// vector 7 reserved

// INT5                (software int 8)
#pragma interrupt    _int5(vect=8)
void _int5(void){}

// INT4                (software int 9)
#pragma interrupt    _int4(vect=9)
void _int4(void){}

// UART2 Bus Collision        (software int 10)
#pragma interrupt    _uart2_bus_collision(vect=10)
void _uart2_bus_collision(void){}

// DMA0                (software int 11)
#pragma interrupt    _dma0(vect=11)
void _dma0(void){}

// DMA1                (software int 12)
#pragma interrupt    _dma1(vect=12)
void _dma1(void){}

// input key            (software int 13)
#pragma interrupt    _input_key(vect=13)
void _input_key(void){}

// A-D                (software int 14)
#pragma interrupt    _ad_converter(vect=14)
void _ad_converter(void){}

// uart2 trance/NACK2        (software int 15)
#pragma interrupt    _uart2_trance(vect=15)
void _uart2_trance(void){}

// uart2 receive/ACK2        (software int 16)
#pragma interrupt    _uart2_receive(vect=16)
void _uart2_receive(void){}

#if defined (__STANDARD_IO__) && (defined(__FOUSB__) ||  defined(__E8__))
// uart0 can't be used
#else
// uart0 trance            (software int 17)
#pragma interrupt    _uart0_trance(vect=17)
void _uart0_trance(void){}
#endif

// uart0 receive        (software int 18)
#pragma interrupt    _uart0_receive(vect=18)
void _uart0_receive(void){}

#if defined(__STANDARD_IO__) || defined(__FOUSB__) || defined(__E8__)
// uart1 can't be used
#else
// uart1 trance            (software int 19)
#pragma interrupt    _uart1_trance(vect=19)
void _uart1_trance(void){}
#endif

// uart1 receive        (software int 20)
#pragma interrupt    _uart1_receive(vect=20)
void _uart1_receive(void){}

// TIMER A0            (software int 21)
#pragma interrupt    _timer_a0(vect=21)
void _timer_a0(void){}

// TIMER A1            (software int 22)
#pragma interrupt    _timer_a1(vect=22)
void _timer_a1(void){}

// TIMER A2            (software int 23)
#pragma interrupt    _timer_a2(vect=23)
void _timer_a2(void){}

// TIMER A3            (software int 24)
#pragma interrupt    _timer_a3(vect=24)
void _timer_a3(void){}

// TIMER A4            (software int 25)
#pragma interrupt    _timer_a4(vect=25)
void _timer_a4(void){}

// timer b0            (software int 26)
#pragma interrupt    _timer_b0(vect=26)
void _timer_b0(void){}

// timer b1            (software int 27)
#pragma interrupt    _timer_b1(vect=27)
void _timer_b1(void){}

// timer b2            (software int 28)
#pragma interrupt    _timer_b2(vect=28)
void _timer_b2(void){}

// int0                (software int 29)
#pragma interrupt    _int0(vect=29)
void _int0(void){}

// int1                (software int 30)
#pragma interrupt    _int1(vect=30)
void _int1(void){}

// int2                (software int 31)
#pragma interrupt    _int2(vect=31)
void _int2(void){}

// software int 32 for user or MR30
// software int 33 for user or MR30
// software int 34 for user or MR30
// software int 35 for user or MR30
// software int 36 for user or MR30
// software int 37 for user or MR30
// software int 38 for user or MR30
// software int 39 for user or MR30
// software int 40 for user or MR30
// software int 41 for user or MR30
// software int 42 for user or MR30
// software int 43 for user or MR30
// software int 44 for user or MR30
// software int 45 for user or MR30
// software int 46 for user or MR30
// software int 47 for user or MR30
// software int 48 for user
// software int 49 for user
// software int 50 for user
// software int 51 for user
// software int 52 for user
// software int 53 for user
// software int 54 for user
// software int 55 for user
// software int 56 for user
// software int 57 for user
// software int 58 for user
// software int 59 for user
// software int 60 for user
// software int 61 for user
// software int 62 for user
// software int 63 for user

