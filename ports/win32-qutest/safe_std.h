/**
* @file
* @brief "safe" <stdio.h> and <string.h> facilities
* @ingroup qpspy
* @cond
******************************************************************************
* Last updated for version 6.9.0
* Last updated on  2020-08-24
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
* <info@state-machine.com>
******************************************************************************
* @endcond
*/
#ifndef SAFE_STD_H
#define SAFE_STD_H

#include <stdio.h>
#include <string.h>

/* portable "safe" facilities from <stdio.h> and <string.h> ................*/
#ifdef _WIN32 /* Windows OS? */

#define MEMMOVE_S(dest_, num_, src_, count_) \
    memmove_s(dest_, num_, src_, count_)

#define STRNCPY_S(dest_, destsiz_, src_) \
    strncpy_s(dest_, destsiz_, src_, _TRUNCATE)

#define STRCAT_S(dest_, destsiz_, src_) \
    strcat_s(dest_, destsiz_, src_)

#define SNPRINTF_S(buf_, bufsiz_, format_, ...) \
    _snprintf_s(buf_, bufsiz_, _TRUNCATE, format_, ##__VA_ARGS__)

#define PRINTF_S(format_, ...) \
    printf_s(format_, ##__VA_ARGS__)

#define FPRINTF_S(fp_, format_, ...) \
    fprintf_s(fp_, format_, ##__VA_ARGS__)

#ifdef _MSC_VER
#define FREAD_S(buf_, bufsiz_, elsiz_, count_, fp_) \
    fread_s(buf_, bufsiz_, elsiz_, count_, fp_)
#else
#define FREAD_S(buf_, bufsiz_, elsiz_, count_, fp_) \
    fread(buf_, elsiz_, count_, fp_)
#endif /* _MSC_VER */

#define FOPEN_S(fp_, fName_, mode_) \
if (fopen_s(&fp_, fName_, mode_) != 0) { \
    fp_ = (FILE *)0; \
} else (void)0

#define LOCALTIME_S(tm_, time_) \
    localtime_s(tm_, time_)

#else /* other OS (Linux, MacOS, etc.) .....................................*/

#define MEMMOVE_S(dest_, num_, src_, count_) \
    memmove(dest_, src_, count_)

#define STRNCPY_S(dest_, destsiz_, src_) do { \
    strncpy(dest_, src_, destsiz_);           \
    dest_[(destsiz_) - 1] = '\0';             \
} while (false)

#define STRCAT_S(dest_, destsiz_, src_) \
    strcat(dest_, src_)

#define SNPRINTF_S(buf_, bufsiz_, format_, ...) \
    snprintf(buf_, bufsiz_, format_, ##__VA_ARGS__)

#define PRINTF_S(format_, ...) \
    printf(format_, ##__VA_ARGS__)

#define FPRINTF_S(fp_, format_, ...) \
    fprintf(fp_, format_, ##__VA_ARGS__)

#define FREAD_S(buf_, bufsiz_, elsiz_, count_, fp_) \
    fread(buf_, elsiz_, count_, fp_)

#define FOPEN_S(fp_, fName_, mode_) \
    (fp_ = fopen(fName_, mode_))

#define LOCALTIME_S(tm_, time_) \
    memcpy(tm_, localtime(time_), sizeof(struct tm))

#endif /* _WIN32 */

#endif /* SAFE_STD_H */
