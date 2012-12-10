/***
*stdint.h - WG14/N843 C99 Standard, Section 7.18
*
*
*Purpose:
*       This header is part of the ANSI C99 standard library, but
*       is not included in the Visual C++ distribution.
*
*       Currently, this file contains only the 6 most basic
*       exact-width integer types (see C99 Section 7.18.1.1).
*       [ANSI]
*
*       [Public]
*
****/

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _INC_STDINT
#define _INC_STDINT

#ifdef  __cplusplus
extern "C" {
#endif

/* Exact-width types. WG14/N843 C99 Standard, Section 7.18.1.1 */
typedef signed   char      int8_t;
typedef unsigned char      uint8_t;
typedef signed   short     int16_t;
typedef unsigned short     uint16_t;
typedef signed   long      int32_t;
typedef unsigned long      uint32_t;
typedef signed   long long int64_t;
typedef unsigned long long uint64_t;

#ifdef  __cplusplus
}
#endif

#endif  /* _INC_STDINT */
