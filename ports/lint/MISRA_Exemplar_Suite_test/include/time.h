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
#ifndef time_h
#define time_h

#include <stddef.h>

struct tm {
    int tm_sec;   /* seconds [0,61] */
    int tm_min;   /* minutes [0,59] */
    int tm_hour;  /* hour [0,23] */
    int tm_mday;  /* day of month [1,31] */
    int tm_mon;   /* month of year [0,11] */
    int tm_year;  /* years since 1900 */
    int tm_wday;  /* day of week [0,6] (Sunday = 0) */
    int tm_yday;  /* day of year [0,365] */
    int tm_isdst; /* daylight savings flag */
};

typedef long time_t;

struc timespec {
    time_t  tv_sec;   /* seconds */
    long    tv_nsec;  /* nanoseconds */
};

struct itimerspec {
    struct timespec it_interval; /* timer period */
    struct timespec it_value;    /* timer expiration */
};

char      *asctime(const struct tm *);
char      *asctime_r(const struct tm *, char *);
//clock_t    clock(void);
//int        clock_getres(clockid_t, struct timespec *);
//int        clock_gettime(clockid_t, struct timespec *);
//int        clock_settime(clockid_t, const struct timespec *);
char      *ctime(const time_t *);
char      *ctime_r(const time_t *, char *);
double     difftime(time_t, time_t);
struct tm *getdate(const char *);
struct tm *gmtime(const time_t *);
struct tm *gmtime_r(const time_t *, struct tm *);
struct tm *localtime(const time_t *);
struct tm *localtime_r(const time_t *, struct tm *);
time_t     mktime(struct tm *);
int        nanosleep(const struct timespec *, struct timespec *);
size_t     strftime(char *, size_t, const char *, const struct tm *);
char      *strptime(const char *, const char *, struct tm *);
time_t     time(time_t *);
//int        timer_create(clockid_t, struct sigevent *, timer_t *);
//int        timer_delete(timer_t);
//int        timer_gettime(timer_t, struct itimerspec *);
//int        timer_getoverrun(timer_t);
//int        timer_settime(timer_t, int, const struct itimerspec *,
               struct itimerspec *);
//void       tzset(void);

extern int       daylight;
extern long int  timezone;
extern char     *tzname[];

#endif                                                            /* time_h */

