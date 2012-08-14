/*
 * Release Date: 17 JULY 2007
 *
 * MISRA-C:2004 Exemplar Suite
 *
 * Published by
 * MIRA Limited, Watling Street, Nuneaton, Warwickshire CV10 0TU UK
 * www.misra-c.com
 *
 * Copyright MIRA Limited, 2006, 2007.
 *
 * See file READ_ME.txt for full copyright, license and release information.
 */

/*
 * 19.3 The #include directive shall be followed by either a <filename> or
 *      "filename" sequence.
 */

#include "mc2_types.h"

/* Following two statements are compliant */

#define MY_HEADER "mc2_header.h"
#include MY_HEADER


/* Following statement is not compliant and may not compile */

/*???
#include mc2_1903_h.h
*/

void mc2_1903 ( void )
{
   int8_t mc2_1903_a = 0;
   use_int8 ( mc2_1903_a );
}

/* end of mc2_1903.c */
