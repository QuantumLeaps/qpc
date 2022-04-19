/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2021 SEGGER Microcontroller GmbH                  *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: V5.14.0.0                                        *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
File    : OS_Syscalls.c
Purpose : Newlib Syscalls callback functions.
          Only _sbrk() implemented for embOS.
          All others unchanged (default empty functions).
--------  END-OF-HEADER  ---------------------------------------------
*/

#include <errno.h>
#undef   errno
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>

/*********************************************************************
*
*       External data
*
**********************************************************************
*/
extern int errno;

extern char __heap_start__;  // Has to be defined in the linker file.
extern char __heap_end__;

/*********************************************************************
*
*       environ
*
*       A pointer to a list of environment variables and their values.
*       Not needed (supported) by embOS, this empty list is adequate
*/
char*  __env[1] = { 0 };
char** environ  = __env;

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/
int     _close        (int file)                                   __attribute__((weak));
int     _execve       (char* pName, char** pArgv, char** pEnv)     __attribute__((weak));
void    _exit         (int __status)                               __attribute__((weak));
int     _fork         (void)                                       __attribute__((weak));
int     _fstat        (int file, struct stat* pSt)                 __attribute__((weak));
int     _getpid       (void)                                       __attribute__((weak));
int     _gettimeofday (struct timeval* pTp, struct timezone* pTzp) __attribute__((weak));
int     _isatty       (int file)                                   __attribute__((weak));
int     _kill         (int pid, int sig)                           __attribute__((weak));
int     _link         (char* pOld, char* pNew)                     __attribute__((weak));
int     _lseek        (int file, int ptr, int dir)                 __attribute__((weak));
int     _open         (const char* pName, int flags, int mode)     __attribute__((weak));
int     _read         (int file, char* p, int len)                 __attribute__((weak));
caddr_t _sbrk         (int incr);
int     _stat         (const char* pFile, struct stat* pSt)        __attribute__((weak));
clock_t _times        (struct tms* pBuf)                           __attribute__((weak));
int     _unlink       (char* pName)                                __attribute__((weak));
int     _wait         (int* pStatus)                               __attribute__((weak));
int     _write        (int file, char* p, int len)                 __attribute__((weak));

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _close()
*
*  Function description
*    Close a file.
*    Not needed (supported) by embOS, minimal implementation
*/
int _close(int file) {
  (void) file;  /* Not used, avoid warning */
  return -1;
}

/*********************************************************************
*
*       _execve()
*
*  Function description
*    Transfer control to a new process.
*    Not needed (supported) by embOS, minimal implementation
*/
int _execve(char* pName, char** pArgv, char** pEnv) {
  (void) pName;  /* Not used, avoid warning */
  (void) pArgv;  /* Not used, avoid warning */
  (void) pEnv;   /* Not used, avoid warning */
  errno = ENOMEM;
  return -1;
}

/*********************************************************************
*
*       _exit()
*
*  Function description
*    Exit a program without cleaning up files.
*/
void _exit(int __status) {
  (void) __status;  /* Not used, avoid warning */
  while(1);
}

/*********************************************************************
*
*       _fork()
*
*  Function description
*    Create a new process.
*    Not needed (supported) by embOS, minimal implementation
*/
int _fork(void) {
  errno = EAGAIN;
  return -1;
}

/*********************************************************************
*
*       _fstat()
*
*  Function description
*    Status of an open file.
*    Not needed (supported) by embOS, minimal implementation
*    For consistency with other minimal implementations in these examples,
*    all files are regarded as character special devices. The sys/stat.h header file
*    required is distributed in the include subdirectory for this C library.
*/
int _fstat(int file, struct stat* pSt) {
  (void) file;  /* Not used, avoid warning */
  pSt->st_mode = S_IFCHR;
  return 0;
}

/*********************************************************************
*
*       _getpid()
*
*  Function description
*    Process-ID; this is sometimes used to generate strings unlikely to conflict with other processes.
*    Not needed (supported) by embOS, minimal implementation
*/
int _getpid(void) {
  return 1;
}

/*********************************************************************
*
*       _gettimeofday()
*
*  Function description
*    Not supported by embOS, minimal implementation
*/
int _gettimeofday(struct timeval* pTp, struct timezone* pTzp) {
  (void) pTp;   /* Not used, avoid warning */
  (void) pTzp;  /* Not used, avoid warning */
  errno = ENOSYS;
  return -1;
}

/*********************************************************************
*
*       _isatty()
*
*  Function description
*    Query whether output stream is a terminal.
*    Not needed (supported) by embOS, minimal implementation
*    For consistency with the other minimal implementations, which only support output to stdout,
*    this minimal implementation is suggested
*/
int _isatty(int file) {
  (void) file;  /* Not used, avoid warning */

  return 1;
}

/*********************************************************************
*
*       _kill()
*
*  Function description
*    Send a signal.
*    Not needed (supported) by embOS, minimal implementation
*/
int _kill(int pid, int sig) {
  (void) pid;  /* Not used, avoid warning */
  (void) sig;  /* Not used, avoid warning */
  errno = EINVAL;
  return -1;
}

/*********************************************************************
*
*       _link()
*
*  Function description
*    Establish a new name for an existing file.
*    Not needed (supported) by embOS, minimal implementation
*/
int _link(char* pOld, char* pNew) {
  (void) pOld;  /* Not used, avoid warning */
  (void) pNew;  /* Not used, avoid warning */
  errno = EMLINK;
  return -1;
}

/*********************************************************************
*
*       _lseek()
*
*  Function description
*    Set position in a file.
*    Not needed (supported) by embOS, minimal implementation
*/
int _lseek(int file, int ptr, int dir) {
  (void) file;  /* Not used, avoid warning */
  (void) ptr;   /* Not used, avoid warning */
  (void) dir;   /* Not used, avoid warning */
  return 0;
}

/*********************************************************************
*
*       _open()
*
*  Function description
*    Open a file.
*    Not needed (supported) by embOS, minimal implementation
*/
int _open(const char* pName, int flags, int mode) {
  (void) pName;  /* Not used, avoid warning */
  (void) flags;  /* Not used, avoid warning */
  (void) mode;   /* Not used, avoid warning */
  return -1;
}

/*********************************************************************
*
*       _read()
*
*  Function description
*    Read from a file.
*    Not needed (supported) by embOS, minimal implementation
*/
int _read(int file, char* p, int len) {
  (void) file;  /* Not used, avoid warning */
  (void) p;     /* Not used, avoid warning */
  (void) len;   /* Not used, avoid warning */
  return 0;
}

/*********************************************************************
*
*       _sbrk()
*
*  Function description
*    Increase program data space. As malloc and related functions depend on this,
*    it is required to have a working implementation.
*    __heap_start__ and __heap_end__ have to be defined in the linker file
*/
caddr_t _sbrk(int incr) {
  static char* heap_end;
  char*        prev_heap_end;

  if (heap_end == NULL) {
    heap_end = & __heap_start__;
  }
  prev_heap_end = heap_end;
  if (heap_end + incr > &__heap_end__) {
      /* Some of the libstdc++-v3 tests rely upon detecting
         out of memory errors, so do not abort here. */
      errno = ENOMEM;
      return (caddr_t) -1;
  }
  heap_end += incr;
  return (caddr_t) prev_heap_end;
}

/*********************************************************************
*
*       _stat()
*
*  Function description
*    Status of a file (by name).
*    Not needed (supported) by embOS, minimal implementation
*/
int _stat(const char* pFile, struct stat* pSt) {
  (void) pFile;  /* Not used, avoid warning */
  pSt->st_mode = S_IFCHR;
  return 0;
}


/*********************************************************************
*
*       _times()
*
*  Function description
*    Timing information for current process.
*    Not needed (supported) by embOS, minimal implementation
*/
clock_t _times(struct tms* pBuf) {
  (void) pBuf;  /* Not used, avoid warning */
  return -1;
}

/*********************************************************************
*
*       _unlink()
*
*  Function description
*    Remove a file's directory entry.
*    Not needed (supported) by embOS, minimal implementation
*/
int _unlink(char* pName) {
  (void) pName;  /* Not used, avoid warning */
  errno = ENOENT;
  return -1;
}

/*********************************************************************
*
*       _wait()
*
*  Function description
*    Wait for a child process.
*    Not needed (supported) by embOS, minimal implementation
*/
int _wait(int* pStatus) {
  (void) pStatus;  /* Not used, avoid warning */
  errno = ECHILD;
  return -1;
}

/*********************************************************************
*
*       _write()
*
*  Function description
*    Write to a file.
*    libc subroutines will use this system routine for output to all files,
*    including stdouts if you need to generate any output,
*    for example to a serial port for debugging, you should make your minimal write capable of doing this.
*    The following minimal implementation is an incomplete example; it relies on a outbyte subroutine
*    (not shown; typically, you must write this in assembler from examples provided by your hardware manufacturer)
*    to actually perform the output.
*    Not needed (supported) by embOS, minimal implementation
*/
int _write(int file, char* p, int len) {
  int todo;

  (void) file;  /* Not used, avoid warning */
  (void) p;     /* Not used, avoid warning */
  (void) len;   /* Not used, avoid warning */
  for (todo = 0; todo < len; todo++) {
    /* outbyte (*p++); */ /* Not supported, has to be implemented if needed */
  }
  return len;
}

/*************************** End of file ****************************/
