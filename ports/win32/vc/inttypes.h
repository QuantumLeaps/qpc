/***
*inttypes.h - WG14/N843 C99 Standard, Section 7.8
*
*
*Purpose:
*       This header is part of the ANSI C99 standard library, but
*       is not included in the Visual C++ distribution.
*
*       Currently, this file contains only the conversion length
*       specifiers for 64-bit integer types (see C99 Section 7.8).
*       [ANSI]
*
*       [Public]
*
****/

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _INC_INTTYPES
#define _INC_INTTYPES

/* fprintf macros for format specifiers */
#define PRId64 "I64d"
#define PRIi64 "I64i"
#define PRIo64 "I64o"
#define PRIu64 "I64u"
#define PRIx64 "I64x"
#define PRIX64 "I64X"

/* fscanf macros for format specifiers */
#define SCNd64 "I64d"
#define SCNi64 "I64i"
#define SCNo64 "I64o"
#define SCNu64 "I64u"
#define SCNx64 "I64x"

#endif  /* _INC_INTTYPES */
