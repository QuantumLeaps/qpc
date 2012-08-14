/** \file scuba.c

 $NOTES: $
 -    This code has been formatted for Doxygen generation

 $REVISIONS: $
 - 01 Jan 2008
    -     Original writing.

 $COPYRIGHT: $
 -    Copyright 2008 by Netrino, LLC. All rights reserved.
**/

#include <stdint.h>
#include "scuba.h"

                  /** Respiratory minute volume = 1200 centiLitres / minute */
#define RMV     1200

          /** Respiratory half second volume = 10 centiLitres / half_second */
#define RHSV    (RMV / 2 / 60)

#define HALFSECS_TO_ASCEND_1000MM  (2 * 60 * 1000 / ASCENT_RATE_LIMIT)

/**
 FUNCTION: gas_rate_in_cl

 DESCRIPTION:
 This computes how much gas is consumed in a half second at a certain depth.

 PARAMETERS:
 -    The current depth in meters

 RETURNS:
 -    The number of centilitres in gas.

 NOTES:

**/
uint32_t gas_rate_in_cl(int32_t depth_in_mm) {
   /* Ambient pressure in centiBar; 10000mm of water = 1 bar = 100 centibar */
    uint32_t ambient_pressure_in_cb = 100 + (depth_in_mm / 100);

       /* Gas consumed at STP = RHSV * ambient pressure / standard pressure */
    return ((RHSV * ambient_pressure_in_cb) / 100);
}

/**
 FUNCTION: gas_to_surface_in_l

 DESCRIPTION:
 This computes how much gas at STP it would take to surface from the current
 depth, assuming no decompression stops and an ascent rate of
 ASCENT_RATE_LIMIT.

 It does this via numerical integration. The step size is 1 m.

 PARAMETERS:
 -    The current depth in meters

 RETURNS:
 -    The number of litres of gas at STP required to make it to the surface.

 NOTES:

**/
uint32_t gas_to_surface_in_cl(int32_t depth_in_mm) {
    uint32_t gas = 0;
    for (; depth_in_mm > 0; depth_in_mm -= 1000) {
                                            /* Ambient pressure in centiBar */
        uint32_t ambient_pressure_in_cb = 100 + (depth_in_mm / 100);
        gas += (RHSV * HALFSECS_TO_ASCEND_1000MM * ambient_pressure_in_cb)
                / 100;
    }
    return gas;
}
