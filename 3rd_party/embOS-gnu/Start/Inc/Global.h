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

File    : Global.h
Purpose : Global types
          In case your application already has a Global.h, you should
          merge the files. In order to use Segger code, the types
          U8, U16, U32, I8, I16, I32 need to be defined in Global.h;
          additional definitions do not hurt.
Revision: $Rev: 22731 $
*/

#ifndef GLOBAL_H            // Guard against multiple inclusion
#define GLOBAL_H

#define U8    unsigned char
#define I8    signed char
#define U16   unsigned short
#define I16   signed short
#if defined(__x86_64__) || defined(__aarch64__)
#define U32   unsigned int
#define I32   int
#else
#define U32   unsigned long
#define I32   signed long
#endif

//
// CC_NO_LONG_SUPPORT can be defined to compile test
// without long support for compilers that do not
// support C99 and its long type.
//
#ifdef CC_NO_LONG_SUPPORT
  #define PTR_ADDR  U32
#else  // Supports long type.
  #if defined(_MSC_VER)
    //
    // Microsoft VC6 and newer.
    // Older versions exist (_MSC_VER <= 1200) but are
    // not even officially listed by Microsoft and might
    // not not work with these defines.
    //
    #define U64   unsigned __int64
    #define U128  unsigned __int128
    #define I64   __int64
    #define I128  __int128
    #if (_MSC_VER <= 1200)
      #define U64_C(x) x##UI64
    #else
      #define U64_C(x) x##ULL
    #endif
  #else
    //
    // C99 compliant compiler.
    //
    #define U64   unsigned long long
    #define I64   signed long long
    #define U64_C(x) x##ULL
  #endif
  //
  // Types for pointer<->address conversions.
  //
  #if (defined(_WIN64) || defined(__LP64__))  // 64-bit symbols used by Visual Studio and GCC, maybe others as well.
    #define PTR_ADDR  U64
  #else
    #define PTR_ADDR  U32
  #endif
#endif  // Supports long type.

#endif                      // Avoid multiple inclusion

/*************************** End of file ****************************/
