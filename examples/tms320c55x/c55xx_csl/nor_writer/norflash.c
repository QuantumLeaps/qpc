/*
 *  Copyright 2010 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  Flash Setup
 *
 */

#include "norflash.h"
#include "usbstk5515_emif.h"
#include "usbstk5515.h"

/* Constant table containing end address of each sector */
static Uint32 sector_end[FLASH_SECTORS];

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_init( )                                                        *
 *                                                                          *
 *      Setup Flash                                                         *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 norflash_init( )
{
    Int16 i, j;

    /* Reset Counter value */
    SYS_PRCNTR = 0x020;
    /* Reset all Modules */
    SYS_PRCNTRLR = 0x00fb;
    for(i=0; i<50; i++);

    /* Config EMIF - ASYNC-Wait Config Regsiters */
    EMIF_AWCCR1 = 0x0080;
    EMIF_AWCCR2  = 0x00E4;

    /* Populate sector table */
    for (i = 0; i < FLASH_SECTORS_LOW; i++)
    {
        sector_end[i] = FLASH_BASE +(((i + 1) * FLASH_SECTORSIZE_LOW) - 1);
    }
    for (j = 0; j < FLASH_SECTORS_HIGH; j++)
    {
        sector_end[i] = (FLASH_BASE + (FLASH_SECTORS_LOW * FLASH_SECTORSIZE_LOW )) +(((j + 1) * FLASH_SECTORSIZE_HIGH) - 1);
        i++;
    }
    return 0;
}
Uint32 erase_timeout = 0x08000000;

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_erase( start, length )                                         *
 *      Erase Flash containing address ( start ) to ( start + length ).     *
 *      Flash can only erase entire sectors containing the range.           *
 *                                                                          *
 *      start  <- starting address                                          *
 *      length <- length in bytes                                           *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 norflash_erase( Uint32 start, Uint32 length )
{
    Int16 i;
    Uint8 *pdata;
    Uint32 sector_base,end;

    length = length / 2;             // Convert length to words
    end = start + length - 1;        // Calculate end of range
    start &= 0xFFFFFFFE;             // Align to 16-bit words

  /* Walk through each sector, erase any sectors within range */
    sector_base = FLASH_BASE;
    for (i = 0; i < FLASH_SECTORS; i++)
    {
        if ( ( ( sector_base >= start ) || ( sector_end[i] >= start ) ) &&
             ( ( sector_base <= end )   || ( sector_end[i] <= end ) ) )
        {
            /* Start sector erase sequence */
            FLASH_CTL555 = FLASH_CMD_AA;
            FLASH_CTL2AA = FLASH_CMD_55 ;
            FLASH_CTL555 = FLASH_ERASE;
            FLASH_CTL555 = FLASH_CMD_AA;
            FLASH_CTL2AA = FLASH_CMD_55 ;

            /* Start erase at sector address */
            pdata = (Uint8 *)sector_base;
            *pdata = FLASH_ERASE_SECTOR;

            /* Wait for erase to complete */
            while (1)
                if (*pdata & 0x80)
                    break;

            /* Put back in read mode */
            *((Uint8 *)FLASH_BASE) = FLASH_RESET;
        }

        /* Advance to next sector */
        sector_base = sector_end[i] + 1;
    }
    *((Uint8 *)FLASH_BASE) = FLASH_RESET;
    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _FLASH_getId( )                                                         *
 *                                                                          *
 *      Get IDs                                                             *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Uint16 _FLASH_getId( Uint16* id)
{
    Uint32 addr = FLASH_BASE;

    volatile Uint16* addr16  = ( Uint16* )addr;
    volatile Uint16* pmfgid  = ( Uint16* )addr;
    volatile Uint16* pdevid1 = ( Uint16* )( addr + 0x1 );

    FLASH_CTL555 = FLASH_RESET;
    USBSTK5515_waitusec(10);

    /* Configure to read manufacturer ID */
    FLASH_CTL555 = FLASH_CMD_AA;
    FLASH_CTL2AA = FLASH_CMD_55;
    FLASH_CTL555 = FLASH_ID;

    /* Insert small delay for device to respond */
    USBSTK5515_waitusec(10);

    *id++ = *pmfgid;          // Read MFG_ID
    *id++ = *pdevid1;         // Read DEV_ID

    *addr16 = FLASH_RESET;
    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_read( src, dst, length )                                         *
 *      Read from Flash address ( src ) to the data at non-Flash address    *
 *      ( dst ) for ( length ) bytes.                                       *
 *                                                                          *
 *      src     <- source address                                           *
 *      dest    <- destination address                                      *
 *      length  <- length in bytes                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 norflash_read( Uint32 src, Uint32 dst, Uint32 length )
{
    Uint32 i;
    Uint16* psrc16 = ( Uint16* )src;
    Uint16* pdst16 = ( Uint16* )dst;

    /*
     *  Set to Read Mode
     */
    FLASH_BASE_PTR8 = FLASH_RESET;

    /*
     *  Read Data to Buffer
     */
    for ( i = 0 ; i < length ; i += 2 )
        *pdst16++ = *psrc16++;   // Read one word

    return 0;
}

Uint32 write_timeout = 0x00010000;

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _FLASH_write( src, dst, length )                                        *
 *      Write to Flash address [dst] the data at a non-Flash address [src]  *
 *      for [length] #bytes.                                                *
 *                                                                          *
 *      src     <- source address                                           *
 *      dest    <- destination address                                      *
 *      length  <- length in bytes                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 norflash_write( Uint32 src, Uint32 dst, Uint32 length )
{
    Uint32 i;
    Uint16* psrc16;
    Uint16* pdst16;

    psrc16 = ( Uint16* )src;
    pdst16 = ( Uint16* )dst;

    for ( i = 0 ; i < length ; i += 2 )
    {
        /* Program one 16-bit word */
        FLASH_CTL555 = FLASH_CMD_AA;
        FLASH_CTL2AA = FLASH_CMD_55;
        FLASH_CTL555 = FLASH_PROGRAM;
        *pdst16 = *psrc16;

         /* Wait for programming to complete */
        // Wait for operation to complete
        while(1)
            if (*pdst16 == *psrc16)
                break;
        pdst16++;
        psrc16++;
    }

    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *    waitWhileProgramming( )                                               *
 *                                                                          *
 *      Wait while programming ( SPANSION Flash )                           *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 _FLASH_waitWhileProgramming( Uint32 addr, Uint16 data, Int32 timeout )
{
    Uint16* pdata = ( Uint16* )addr;

    while ( timeout-- > 0 )
        if ( *pdata == data )  /* Good, programming completed */
            return 0;

    /* Timeout occured */
    FLASH_BASE_PTR8 = FLASH_RESET;
    return 1;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _FLASH_waitWhileErasing( )                                              *
 *                                                                          *
 *      Wait while erasing ( SPANSION Flash )                               *
 *                                                                          *
 * ------------------------------------------------------------------------ */

Int16 _FLASH_waitWhileErasing( Uint32 addr, Int32 timeout )
{
    Uint8* pdata = ( Uint8* )addr;

    while ( timeout-- > 0 )
        if ( *pdata == 0xFF )
            /* Good, erase completed */
            return 0;

    /* Timeout occured */
    FLASH_BASE_PTR8 = FLASH_RESET;
    return 1;
}
