/*
 *  Copyright 2010 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  NOR Flash Writer
 *
 */

#include "stdio.h"
#include "norflash.h"

Uint16 tx[0x4000];
Uint16 rx[0x4000];

Int8 fileName[256];
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_writer( )                                                      *
 *      Supports upto 32KB files                                            *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 norflash_writer( )
{
    Int32 j = 0;
    Uint16 id[3], errors;
    Int32    fileSize = 0;
    FILE    *fPtr;
    Uint16    *ramPtr;

    /* Get the file */
    // Read the filename from host
    printf("Enter the file Name:\r\n");
    scanf("%s", fileName);
    fflush(stdin);

    // Open a File from the hard drive
    printf("Opening file...\r\n");
    fPtr = fopen(fileName, "rb");
    //fPtr = fopen("C:\\2010\\0222-USBSTK5515_demo\\EZ_Demo.bin", "rb");
    if(fPtr == NULL)
    {
        printf("ERROR: File %s Open failed\r\n", fileName);
        return 1;
    }
    // Initialize size to 0
    fileSize = 0;

    // Get file size
    fseek(fPtr,0,SEEK_END);
    fileSize = ftell(fPtr);

    // Setup pointer in RAM for temporary storage of data
    ramPtr = &tx[0];

    if(fileSize == 0)            // Check if file was found
    {
        printf("ERROR: File read failed.. Closing program.\r\n");
        fclose (fPtr);
        return 1;
    }
        fseek(fPtr,0,SEEK_SET);

    if (fileSize != fread(ramPtr, 1, fileSize, fPtr)) // Read file to ram and check if read properly
    {
        printf("WARNING: File Size mismatch.\r\n");
        return 1;
    }
    fseek(fPtr,0,SEEK_SET);

    /* Convert byte format for NOR Flash */
    for(j=0;j<fileSize/2;j++)
    {
        *ramPtr = (*ramPtr << 8) + *(ramPtr + 1); // fread function only writes to lower byte of word
        tx[j] = *ramPtr;
        ramPtr += 2;
    }

    /* Initialize Flash */
    norflash_init();

    _FLASH_getId( &id[0] );
    printf("Manufacture ID is: %x \nDevice ID is:     %x\n", id[0], id[1] & 0xFF);

    /* Erase fileSize bytes*/
    printf( "     Erasing Flash %d bytes\n", fileSize );
    norflash_erase( FLASH_BASE , fileSize );

    /* Write File */
    printf( "     Writing File: %d bytes\n", fileSize );
    errors = norflash_write( ( Uint32 )tx + 1, FLASH_BASE + 1, fileSize );  // Avoiding boot signature

    /* Read and Compare */
    printf( "     Reading and comparing File: %d bytes\n", fileSize );
    errors = norflash_read( FLASH_BASE, ( Uint32 )rx, fileSize );
    /* Check pattern */
    for ( j = 1 ; j < (fileSize / 2) ; j ++ )  // Checking one word at a time
        if ( tx[j] != rx[j] )
        {
            printf( "     Error at %d \n", j );
            return 1;
        }
    // Write boot signature after data verified
    norflash_write( ( Uint32 )tx , FLASH_BASE , 1 );

    /* Test Passed */

    return errors;
}
