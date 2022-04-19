/*****************************************************************************
* This is a stub for compiling QP port to VxWorks.
* Assembled from "VxWorks Reference Manual : Libraries".
*
* NOTE: This is NOT the original VxWorks header file!
* Also, this file contains only the minimum facilities for compiling
* the QP port, but it by no means complete.
*****************************************************************************/
#ifndef _vxworks_windDefs_h
#define _vxworks_windDefs_h

typedef char INT8;
typedef short INT16;
typedef int INT32;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned long ULONG;
typedef int BOOL;
typedef int VOID;
typedef int STATUS;
typedef int ARGINT;
typedef int (*FUNCPTR) (); /* ptr to function returning int */
typedef VOID (*VOIDFUNCPTR) (); /* ptr to function returning VOID */

#define FALSE  (0)
#define TRUE   (!FALSE)

#define OK     (0)
#define ERROR  (-1)

#define NO_WAIT      (0)
#define WAIT_FOREVER (0xFFFFFFFFU)

#include <stddef.h> /* for size_t */

#endif    /* !_vxworks_windDefs_h */
