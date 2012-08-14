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
#ifndef __stdio_h
#define __stdio_h

#ifndef NULL
#define NULL  0
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned size_t;
#endif

#define BUFSIZ          128
#define FILENAME_MAX    260
#define FOPEN_MAX       8
#define _IOFBF          0
#define _IOLBF          1
#define _IONBF          2
#define L_tmpnam        16
#define TMP_MAX         256

#define EOF             (-1)
#define SEEK_SET        0
#define SEEK_CUR        1
#define SEEK_END        2

typedef struct {                                    /* Dummy typedef for FILE */
    int             dummy1;
    unsigned        dummy2;
    char            dummy3;
} FILE;

extern FILE *stderr;
extern FILE *stdin;
extern FILE *stdout;

typedef long fpos_t;

#ifdef __cplusplus
extern "C" {
#endif

void    clearerr(FILE *__stream);
int     fclose(FILE *__stream);
int     fflush(FILE *__stream);
int     fgetc(FILE *__stream);
int     fgetpos(FILE *__stream, fpos_t *__pos);
char   *fgets(char *__s, int __n, FILE *__stream);
FILE   *fopen(const char *__path, const char *__mode);
int     fprintf(FILE *__stream, const char *__format, ...);
int     fputc(int __c, FILE *__stream);
int     fputs(const char *__s, FILE *__stream);
size_t  fread(void *__ptr, size_t __size, size_t __n, FILE *__stream);
FILE   *freopen(const char *__path, const char *__mode, FILE *__stream);
int     fscanf(FILE *__stream, const char *__format, ...);
int     fseek(FILE *__stream, long __offset, int __whence);
int     fsetpos(FILE *__stream, const fpos_t *__pos);
long    ftell(FILE *__stream);
size_t  fwrite(const void *__ptr, size_t __size, size_t __n, FILE *__stream);
char   *gets(char *__s);
void    perror(const char *__s);
int     printf(const char *__format, ...);
int     puts(const char *__s);
int     remove(const char *__path);
int     rename(const char *__oldname,const char *__newname);
void    rewind(FILE *__stream);
int     scanf(const char *__format, ...);
void    setbuf(FILE *__stream, char *__buf);
int     setvbuf(FILE *__stream, char *__buf,
                       int __type, size_t __size);
int     sprintf(char *__buffer, const char *__format, ...);
int     sscanf(const char *__buffer,
                      const char *__format, ...);
char   *strerror(int __errnum);
FILE   *tmpfile(void);
char   *tmpnam(char *__s);
int     ungetc(int __c, FILE *__stream);
int     vfprintf(FILE *__stream, const char *__format, void *__arglist);
int     vfscanf(FILE *__stream, const char *__format, void *__arglist);
int     vprintf(const char *__format, void *__arglist);
int     vscanf(const char *__format, void *__arglist);
int     vsprintf(char *__buffer, const char *__format, void *__arglist);
int     vsscanf(const char *__buffer, const char *__format,
                void *__arglist);
int     unlink(const char *__path);
int     getc(FILE *__fp);

int     getchar(void);
int     putchar(const int __c);

int     putc(const int __c, FILE *__fp);
int     feof(FILE *__fp);
int     ferror(FILE *__fp);

#ifdef  __cplusplus
}
#endif


#endif                                                         /* __stdio_h */

