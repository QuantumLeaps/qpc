/**
* @file
* @brief Boolean type and constansts. WG14/N843 C99 Standard, Section 7.16
*
* @description
* This header is part of the ANSI C99 standard library to define the
* standard Boolean type as well as the 'true' and 'false' constansts.
*/
#ifndef STDBOOL_H
#define STDBOOL_H

/*lint -save */
/*lint -e9071 M3-R21.1(r), defined macro is reserved to the compiler */
/*lint -e9093 the name is reserved to the compiler */

#ifndef __cplusplus

typedef _Bool   bool;     /*!< standard Boolean data type */

/*!< standard 'true' constant */
#define true  ((bool)!false)

/*!< standard 'false' constant */
#define false ((bool)0)

#endif /* !__cplusplus */

/*lint -restore */

#endif /* STDBOOL_H */

