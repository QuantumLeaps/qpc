/*****************************************************\
 *  Copyright 2003, Texas Instruments Incorporated.  *
 *  All rights reserved.                             *
 *  Restricted rights to use, duplicate or disclose  *
 *  this   code   are  granted   through  contract.  *
\*****************************************************/

/* Standard Types File: [last revised: 2004-01-14] */

#ifndef _TISTDTYPES_H_
#define _TISTDTYPES_H_

/*
    The purpose of this header file is to consolidate all the primitive "C"
    data types into one file. This file is expected to be included in the
    basic types file exported by other software components, for example CSL.
 */

#ifndef _TI_STD_TYPES
#define _TI_STD_TYPES

#ifndef TRUE

typedef int        Bool;
#define TRUE        ((Bool) 1)
#define FALSE        ((Bool) 0)

#endif

typedef int             Int;
typedef unsigned int    Uns;    /* deprecated type */
typedef char            Char;
typedef char *          String;
typedef void *          Ptr;

typedef unsigned long    Uint32;
typedef unsigned short    Uint16;
typedef unsigned char    Uint8;

 /* Signed integer definitions (32bit, 16bit, 8bit) follow... */
typedef long        Int32;
typedef short        Int16;
typedef char        Int8;

#endif /* _TI_STD_TYPES */


#endif /* _TISTDTYPES_H_ */

