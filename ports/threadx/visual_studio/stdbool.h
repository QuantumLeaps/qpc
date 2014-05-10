/***
*stdbool.h - WG14/N843 C99 Standard, Section 7.16
*
*
*Purpose:
*       This header is part of the ANSI C99 standard library, but
*       is not included in the Visual C++ distribution.
*
*       [Public]
*
****/

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _INC_STDBOOL
#define _INC_STDBOOL

#ifndef __cplusplus

typedef int bool;
#define true  ((bool)1)
#define false ((bool)0)

#endif /* !__cplusplus */

#define __bool_true_false_are_defined 1

#endif  /* _INC_STBOOL */
