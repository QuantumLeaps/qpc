/*****************************************************************************
* Product: ANSI C library facilities for PC-Lint
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Jan 13, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This software is be distributed and modified under the terms of the GNU
* General Public License (GPL) as published by the Free Software Foundation
* and appearing in the file GPL.TXT included in the packaging of this file.
* Please note that GPL requires that all works based on this software must
* also be made publicly available under the terms of the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef __stdlib_h
#define __stdlib_h

#ifndef NULL
#define NULL  0
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned size_t;
#endif

#ifndef _DIV_T
#define _DIV_T
typedef struct {
        int  quot;
        int  rem;
} div_t;
#endif

#ifndef _LDIV_T
#define _LDIV_T
typedef struct {
        long quot;
        long rem;
} ldiv_t;
#endif

#ifndef _WCHAR_T
#define _WCHAR_T
typedef char wchar_t;
#endif

/* Old typedef */
typedef void (*atexit_t)(void);

/* Maximum value returned by "rand" function */
#define RAND_MAX 0x7FFFU

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define MB_CUR_MAX 1

#ifdef __cplusplus
extern "C" {
#endif

void   abort(void);
int    __abs__(int);

#ifdef __cplusplus
    inline int  abs(int __x) { return __abs__(__x); }
#else
    int abs(int __x);
#   define abs(x_)   __abs__(x_)
#endif

int    atexit(void (*__func)(void));
double atof(const char *__s);
int    atoi(const char *__s);
long   atol(const char *__s);
void  *bsearch(const void *__key, const void *__base,
               size_t __nelem, size_t __width,
               int (*fcmp)(const void *, const void *));
void  *calloc(size_t __nitems, size_t __size);
div_t  div(int __numer, int __denom);
void   exit(int __status);
void   free(void *__block);
char  *getenv(const char *__name);
long   labs(long __x);
ldiv_t ldiv(long __numer, long __denom);
void * malloc(size_t __size);
int    mblen(const char *__s, size_t __n);
size_t mbstowcs(wchar_t *__pwcs, const char *__s, size_t __n);
int    mbtowc(wchar_t *__pwc, const char *__s, size_t __n);
void   qsort(void *__base, size_t __nelem, size_t __width,
             int (*__fcmp)(const void *, const void *));
int    rand(void);
void  *realloc(void *__block, size_t __size);
void   srand(unsigned __seed);
double strtod(const char *__s, char **__endptr);
long   strtol(const char *__s, char **__endptr, int __radix);
long double _strtold(const char *__s, char **__endptr);
unsigned long strtoul(const char *__s, char **__endptr,
                      int __radix);
int    system(const char *__command);
size_t wcstombs(char *__s, const wchar_t *__pwcs, size_t __n);
int    wctomb(char *__s, wchar_t __wc);

#ifdef __cplusplus
}
#endif

#endif                                                        /* __stdlib_h */

