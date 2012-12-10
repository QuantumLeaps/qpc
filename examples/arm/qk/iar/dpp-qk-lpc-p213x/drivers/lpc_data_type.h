/*************************************************************************
;    Copyright (c) 2004, BU MMS China, Philps Semiconductor
;    All rights reserved.
;
;    file name : LPC2000_LIB.H
;    description: give some basic definitions which will be used
;    in the drivers
;
;    History     :
;
;*************************************************************************/
#ifndef __LPC_DATA_TYPE_H
#define __LPC_DATA_TYPE_H

/************************************************************************
;   Data  types definition
*************************************************************************/
typedef unsigned char   LPC_BOOL;
typedef unsigned char   LPC_INT8U;
typedef signed   char   LPC_INT8S;
typedef unsigned short  LPC_INT16U;
typedef signed   short  LPC_INT16S;
typedef unsigned long   LPC_INT32U;
typedef signed   long   LPC_INT32S;


#define  no_err            0

#ifndef TRUE
#define TRUE               1
#endif

#ifndef FALSE
#define FALSE              0
#endif

#endif /* __LPC_DATA_TYPE_H */


