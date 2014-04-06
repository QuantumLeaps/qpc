/**
* \file
* \ingroup qep qf qk qs
* \brief Boolean type and constansts. WG14/N843 C99 Standard, Section 7.16
*
* \description
* This header is part of the ANSI C99 standard library to define the
* standard Boolean type as well as the 'true' and 'false' constansts.
*
*/
#ifndef stdbool_h
#define stdbool_h

/*lint -save -e960       MISRA-C:2004 19.4, Disallowed definition of 'bool' */

#ifndef __cplusplus

typedef int   bool;                         /*!< standard Boolean data type */
#define true  ((bool)1)                       /*!< standard 'true' constant */
#define false ((bool)0)                      /*!< standard 'false' constant */

#endif /* !__cplusplus */

#define __bool_true_false_are_defined 1

/*lint -restore */

#endif                                                         /* stdbool_h */

