/***************************************************
 *  Startup for M32C
 *  Copyright(c) 2004 Renesas Technology Corp.
 *  And Renesas Solutions Corp.,All Rights Reserved.
 *
 *  allocate heap area
 *
 *
 * $Date: 2005/09/21 02:26:59 $
 * $Revision: 1.4 $
 **************************************************/
#include "typedefine.h"
#include "heapdef.h"
#pragma SECTION    bss    heap

_UBYTE heap_area[__HEAPSIZE__];
