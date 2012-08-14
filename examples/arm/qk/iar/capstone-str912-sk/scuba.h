#ifndef __scuba_h
/** \file scuba.h

 This file contains the prototype definitions for scuba.c

 $NOTES: $
 -    This code has been formatted for Doxygen generation

 $REVISIONS: $
 - 01 Jan 2008
    -     Original writing.

 $COPYRIGHT: $
 -    Copyright 2008 by Netrino, LLC. All rights reserved.
**/
#define __scuba_h

                 /** maximum allowed ascent rate in mm/min */
#define ASCENT_RATE_LIMIT    15000

#define depth_change_in_mm(ascent_rate)  ((ascent_rate) / (2 * 60))
uint32_t gas_rate_in_cl(int32_t depth_in_mm);
uint32_t gas_to_surface_in_cl(int32_t depth_in_mm);

#endif
