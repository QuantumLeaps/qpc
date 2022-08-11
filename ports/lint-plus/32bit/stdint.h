/*!
* @file
* @brief Selected exact-width and fast minimum-width integer types
* for 32-bit CPU architecture (e.g., ARM Cortex-M).
*
* @description
* This header is part of the ANSI C99 standard library to define the
* standard exact-width integer types (see C99 Section 7.18.1.1).
* If the compiler does not provide the stdint.h header file, you can
* either create one in the QP port directory, or you can typedef the
* 8 exact-width integer types directly in the qep_port.h header file.
*
* @note The version included in the QP documentation contains only the 8
* exact-width types and 6 fast minimum-width types actually used in QP.
* The actual definition of the integer types is platform dependent.
*/
#ifndef STDINT_H
#define STDINT_H

/*lint -save */
/*lint -e9093   M3:R21.2(R) name matches a pattern reserved to the compiler */
/*lint -e586    B18:R5.2(b) keyword 'short'/'long' is deprecated */

/* Exact-width types. WG14/N843 C99 Standard, Section 7.18.1.1 */
typedef signed char        int8_t;   /*!< exact-width  8-bit signed   int  */
typedef signed short       int16_t;  /*!< exact-width 16-bit signed   int  */
typedef signed long        int32_t;  /*!< exact-width 32-bit signed   int  */
typedef signed long long   int64_t;  /*!< exact-width 64-bit signed   int  */

typedef unsigned char      uint8_t;  /*!< exact-width  8-bit unsigned int  */
typedef unsigned short     uint16_t; /*!< exact-width 16-bit unsigned int  */
typedef unsigned long      uint32_t; /*!< exact-width 32-bit unsigned int  */
typedef unsigned long long uint64_t; /*!< exact-width 64-bit unsigned int  */

/* Fastest minimum-width types. WG14/N843 C99 Standard, Section 7.18.1.3 */
typedef signed   int  int_fast8_t;   /*!< fast at-least  8-bit signed   int */
typedef unsigned int  uint_fast8_t;  /*!< fast at-least  8-bit unsigned int */
typedef signed   int  int_fast16_t;  /*!< fast at-least 16-bit signed   int */
typedef unsigned int  uint_fast16_t; /*!< fast at-least 16-bit unsigned int */
typedef signed   long int_fast32_t;  /*!< fast at-least 32-bit signed   int */
typedef unsigned long uint_fast32_t; /*!< fast at-least 32-bit unsigned int */

/*lint -restore */

#endif /* STDINT_H */

