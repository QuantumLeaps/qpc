/*****************************************************************************
* Product: ANSI C library facilities for PC-Lint
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Jan 21, 2012
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
#ifndef string_h
#define string_h

#ifndef NULL
#define NULL  0
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned size_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void  *memchr  (const void *__s, int __c, size_t __n);
int    memcmp(const void *__s1, const void *__s2, size_t __n);
void  *memcpy(void *__dest, const void *__src, size_t __n);
void  *memmove(void *__dest, const void *__src, size_t __n);
void  *memset(void *__s, int __c, size_t __n);
char  *strcat(char *__dest, const char *__src);
char  *strchr(const char *__s, int __c);
int    strcmp(const char *__s1, const char *__s2);
int    strcoll(const char *__s1, const char *__s2);
char  *strcpy(char *__dest, const char *__src);
size_t strcspn(const char *__s1, const char *__s2);
char  *strerror(int __errnum);
size_t strlen(const char *__s);
char  *strncat(char *__dest, const char *__src, size_t __maxlen);
int    strncmp(const char *__s1, const char *__s2, size_t __maxlen);
char  *strncpy(char *__dest, const char *__src, size_t __maxlen);
char  *strpbrk(const char *__s1, const char *__s2);
char  *strrchr(const char *__s, int __c);
size_t strspn(const char *__s1, const char *__s2);
char  *strstr(const char *__s1, const char *__s2);
char  *strtok(char *__s1, const char *__s2);
size_t strxfrm(char *__s1, const char *__s2, size_t __n );
char  *_strerror(const char *__s);

#ifdef __cplusplus
}
#endif

#endif                                                          /* string_h */

