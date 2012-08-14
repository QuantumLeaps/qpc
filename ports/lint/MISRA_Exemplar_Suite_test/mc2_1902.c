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
 * 19.2 Non-standard characters should not occur in header file names in
 *      #include directives.
 */

#include "mc2_types.h"
#include "mc2_header.h"

/* Following six statements are not compliant */

#include <mc2_1902'.h>
#include <.\mc2_1902.h>
/*???
#include </* file */mc2_1902.h>
*/

#include "mc2_1902'.h"
#include ".\mc2_1902.h"
/*???
#include "/* file */mc2_1902.h"
*/

void mc2_1902 ( void )
{
   int8_t mc2_1902_a = 0;
   use_int8 ( mc2_1902_a );
}

/* end of mc2_1902.c */
