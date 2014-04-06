/**
* \file
* \brief Exact-width integer types. WG14/N843 C99 Standard, Section 7.18
*
* \description
* This header is part of the ANSI C99 standard library to define the
* standard exact-width integer types (see C99 Section 7.18.1.1).
* If the compiler does not provide the stdint.h header file, you can
* either create one in the QP port directory, or you can typedef the
* 8 exact-width integer types directly in the qep_port.h header file.
*
* \note The version included in the QP documentation contains only the 8
* exact-width types actually used in QP. The actual definition of the
* exact-width integer types is platform dependent.
*/
#ifndef stdint_h
#define stdint_h

/*lint -save -e1960    MISRA-C++:2008 Rule 17-0-2, Re-use of C++ identifier */

/* Exact-width types. WG14/N843 C99 Standard, Section 7.18.1.1 */
typedef signed char        int8_t;    /*!< exact-width  8-bit signed   int  */
typedef signed short       int16_t;   /*!< exact-width 16-bit signed   int  */
typedef signed long        int32_t;   /*!< exact-width 32-bit signed   int  */
typedef signed long long   int64_t;   /*!< exact-width 64-bit signed   int  */

typedef unsigned char      uint8_t;   /*!< exact-width  8-bit unsigned int  */
typedef unsigned short     uint16_t;  /*!< exact-width 16-bit unsigned int  */
typedef unsigned long      uint32_t;  /*!< exact-width 32-bit unsigned int  */
typedef unsigned long long uint64_t;  /*!< exact-width 64-bit unsigned int  */

/* Fastest minimum-width types. WG14/N843 C99 Standard, Section 7.18.1.3 */
typedef signed   int  int_fast8_t;   /*!< fast at-least  8-bit signed   int */
typedef unsigned int  uint_fast8_t;  /*!< fast at-least  8-bit unsigned int */
typedef signed   int  int_fast16_t;  /*!< fast at-least 16-bit signed   int */
typedef unsigned int  uint_fast16_t; /*!< fast at-least 16-bit unsigned int */
typedef signed   long int_fast32_t;  /*!< fast at-least 32-bit signed   int */
typedef unsigned long uint_fast32_t; /*!< fast at-least 32-bit unsigned int */

/*lint -restore */

#endif /* stdint_h */

