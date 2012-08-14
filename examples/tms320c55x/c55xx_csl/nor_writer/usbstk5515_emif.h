/*
 *  Copyright 2010 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  EMIF Header file
 *
 */


/* ------------------------------------------------------------------------ *
 *  Registers                                                               *
 * ------------------------------------------------------------------------ */

#define EMIF_REV                *(volatile ioport Uint16*)(0x1000)
#define EMIF_STATUS             *(volatile ioport Uint16*)(0x1001)
#define EMIF_AWCCR1             *(volatile ioport Uint16*)(0x1004)
#define EMIF_AWCCR2             *(volatile ioport Uint16*)(0x1005)
#define EMIF_ACS2CR1            *(volatile ioport Uint16*)(0x1010)
#define EMIF_ACS2CR2            *(volatile ioport Uint16*)(0x1011)
#define EMIF_ACS3CR1            *(volatile ioport Uint16*)(0x1014)
#define EMIF_ACS3CR2            *(volatile ioport Uint16*)(0x1015)
#define EMIF_ACS4CR1            *(volatile ioport Uint16*)(0x1018)
#define EMIF_ACS4CR2            *(volatile ioport Uint16*)(0x1019)
#define EMIF_ACS5CR1            *(volatile ioport Uint16*)(0x101C)
#define EMIF_ACS5CR2            *(volatile ioport Uint16*)(0x101D)
#define EMIF_EIRR               *(volatile ioport Uint16*)(0x1040)
#define EMIF_EIMR               *(volatile ioport Uint16*)(0x1044)
#define EMIF_EIMSR              *(volatile ioport Uint16*)(0x1048)
#define EMIF_EIMCR              *(volatile ioport Uint16*)(0x104C)
#define EMIF_NANDFCR            *(volatile ioport Uint16*)(0x1060)
#define EMIF_NANDFSR1           *(volatile ioport Uint16*)(0x1064)
#define EMIF_NANDFSR2           *(volatile ioport Uint16*)(0x1065)

