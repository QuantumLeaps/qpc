/*-------------------------------------------------------------------------*
 * File:  preamble.h
 *-------------------------------------------------------------------------*
 * Description:
 *     Basic definitions or all simple types and constants.
 *     Used for compatibility between compilers and hardware.
 *-------------------------------------------------------------------------*/
#ifndef __GLYPH__PREAMBLE_H
#define __GLYPH__PREAMBLE_H

/***************************************************************************
 * SECTION:  CONSTANTS
 ***************************************************************************/
#define ON          1
#define OFF         0
#define ENABLE      1
#define DISABLE     0
#define YES         1
#define NO          0
#define TRUE        1
#define FALSE       0
#define GOOD        1
#define BAD         0
#define HIGH        1
#define LOW         0

/***************************************************************************
 * SECTION:  TYPES
 ***************************************************************************/
#if 1
#include <stdint.h>
#else
typedef signed char int8_t ;
typedef unsigned char uint8_t ;
typedef signed short int16_t ;
typedef unsigned short uint16_t ;
typedef signed long int32_t ;
typedef unsigned long uint32_t ;
typedef signed long long int64_t ;
typedef unsigned long long uint64_t ;
//typedef unsigned int _Bool ;
#endif

/***************************************************************************
 * SECTION:  BIT DEFINITIONS
 ***************************************************************************/
#define b0      0x01
#define b1      0x02
#define b2      0x04
#define b3      0x08
#define b4      0x10
#define b5      0x20
#define b6      0x40
#define b7      0x80

#endif  /* __GLYPH__PREAMBLE_H */
/*-------------------------------------------------------------------------*
 * End of File:  Preamble.H
 *-------------------------------------------------------------------------*/


