/*
*  Copyright 2004 by Texas Instruments Incorporated.
*  All rights reserved. Property of Texas Instruments Incorporated.
*  Restricted rights to use, duplicate or disclose this code are
*  granted through contract.
*
*/
/********************************************************************************/
/* $Id: ata.h,2.4+ $                                                            */
/*                                                                                */
/* This is proprietary information, not to be published -- TI INTERNAL DATA        */
/* Copyright (C) 2000, Texas Instruments, Inc.  All Rights Reserved.            */
/*                                                                                */
/* Author: Susmit Pal                                                            */
/* Date: November 6, 2002                                                        */
/* Modified Date: November 28, 2002                                                */
/* Last Modified Date: February 11, 2003                                        */
/* Mar 21, 2010 - added AtaReadNextWord1 routine to AtaState  KR032110          */
/* Apr 26, 2010 - added NULL definition                       KR042610          */
/********************************************************************************/
/*SUSMIT - ATA-FS2.4+ Customized */


#ifndef _ata_h
#define _ata_h 1

#include <tistdtypes.h>

#define ATA_MAX_LONG_NAME_SIZE      261
#define ATA_MEDIA_ID_WORDS          16

#define ATA_ATTR_READ_ONLY          0x01
#define ATA_ATTR_HIDDEN              0x02
#define ATA_ATTR_SYSTEM              0x04
#define ATA_ATTR_VOLUME              0x08
#define ATA_ATTR_DIRECTORY          0x10
#define ATA_ATTR_ARCHIVE          0x20

#define ATA_ERROR_NONE              0x0000
#define ATA_ERROR_UNSUPPORTED      0x0001
#define ATA_ERROR_NESTED          0x0002
#define ATA_ERROR_TIMEOUT          0x0004
#define ATA_ERROR_BAD_MEDIA          0x0008
#define ATA_ERROR_EOF              0x0010
#define ATA_ERROR_FILE_NOT_FOUND  0x0020
#define ATA_ERROR_ID_NOT_FOUND      0x0040
#define ATA_ERROR_MEDIA_NOT_FOUND 0x0080
#define ATA_ERROR_MEDIA_REMOVED      0x0100
#define ATA_ERROR_DISK_FULL          0x0200

/* Added for checking invalid parameters: PR Mistral */
#define ATA_ERROR_INVALID_PARAM   0x0300
#define ATA_ERROR_INVALID_MODE    0x0301

/*ADD NEW MEDIA ID HERE*/
#define MMC        1
#define SD        2
#define NAND    3
#define CF        4
#define IDE        5
#define MEMSTICK 6 /* Renamed from MS, as it clashes with RTC's MilliSecond Reg.*/

// added for check null pointer KR042110
#ifndef NULL
#define NULL ((void*)0)
#endif

#if 0
typedef unsigned int  AtaUint16;
typedef unsigned long AtaUint32;
typedef AtaUint32     AtaSector;
typedef AtaUint32     AtaFileSize;
#endif

/*typedef changed by ra*/
typedef Uint16        AtaUint16;
typedef Uint32        AtaUint32;
typedef Uint32        AtaSector;
typedef Uint32        AtaFileSize;


#ifndef DISABLE_FAT32
typedef AtaUint32     AtaCluster;
typedef AtaUint32     AtaDirEntry;
#else
typedef AtaUint16     AtaCluster;
typedef AtaUint16     AtaDirEntry;
#endif
/*typedef int           AtaError;*//*SUSMIT - This should be unsigned int*/
typedef AtaUint16     AtaError;
typedef AtaUint16     AtaSerialID[ATA_MEDIA_ID_WORDS];

#define ATA_FAT_TYPE_12           0x01
#define ATA_FAT_TYPE_16           0x04         /* 16-bit FAT. Partitions smaller than 32MB */
#define ATA_FAT_TYPE_32           0x0B         /* 32-bit FAT. Partitions up to 2047GB */


typedef struct AtaStateStruct
{
  AtaSector    BootPhySector;
  AtaUint16    FatType;
  AtaUint16    WordsPerLogSector;
  AtaUint16    LogSectorsPerCluster;
  AtaUint16    RootDirEntries;
  AtaUint32    LogSectorsPerFat;
  AtaSector    FirstFatSector;
  AtaSector    RootDirSector;
  AtaSector    FirstDataSector;
  AtaSector    TotalSectors;
  AtaUint16    Data;
  AtaUint16    CurrentWord;
  AtaSector    CurrentPhySector;
  AtaUint16    WordsPerCluster;
  AtaUint16    NumberOfFats;
  AtaUint16 MFWFlag;
  AtaError  Err_Status;
#ifdef ATA_HDD
/*SUSMIT - Support for media which can support multi-block operations, user configurable*/
  AtaUint16    Multi_block_Flag;
/*SUSMIT - Support for media which can support multi-block operations, user configurable*/
#endif /* ATA_HDD */
  void      *pAtaMediaState;
  AtaError  (*AtaReset)(void *pAtaMediaState);
  int       (*AtaCommandReadyStatusCheck)(void *pAtaMediaState);
  AtaError  (*AtaBusyStatusCheck)(void *pAtaMediaState);
  AtaUint16    (*AtaGetSerialID)(void *pAtaMediaState, AtaSerialID *pID);
  AtaError  (*AtaIssueReadCommand)(AtaSector PhySector, void *pAtaMediaState, AtaUint16 SectorCount);
  AtaError  (*AtaDataReadyStatusCheck)(void *pAtaMediaState);
  AtaError  (*AtaReadNextWord)(void *pAtaMediaState, AtaUint16 *pWord);

  AtaError  (*AtaReadNextWord1)(void *pAtaMediaState, AtaUint16 *pWord,AtaUint16 word_delta); //KR032110

  AtaError  (*AtaReadNextNWords)(void *pAtaMediaState, AtaUint16 *pWord, AtaUint16 Count);
  AtaError  (*AtaWriteSector)(AtaSector PhySector, void *pAtaMediaState, AtaUint16 *pWord, int ByteSwap);
  AtaError  (*AtaWriteSectorFlush)(void *pAtaMediaState);
  AtaError  (*AtaInitAtaMediaState)(void *pAtaState1);
  AtaError   (*AtaFlushFat)(void *pAtaMediaState);
  void      (*AtaErrorCallback)(unsigned short Error);
/*SUSMIT - Support for media which has floppy like filesystem with only a bootsector but no MBR*/
  AtaUint16 (*AtaFileSystemType)(void *pAtaMediaState);
/*SUSMIT - Support for media which has floppy like filesystem with only a bootsector but no MBR*/
#ifdef ATA_HDD
/*SUSMIT - Support for media which can support multi-block operations, user configurable*/
  AtaError  (*AtaReadSectorB)(AtaSector PhySector, void *pAtaMediaState, AtaUint16 *pWord, AtaUint16 ByteSwap);
  AtaError  (*AtaWriteSectorB)(AtaSector PhySector, void *pAtaMediaState, AtaUint16 *pWord, AtaUint16 ByteSwap);
  AtaError  (*AtaWriteFlushB)(void *pAtaMediaState);
  AtaUint16 (*AtaReadWordB)(AtaSector PhySector, void *pAtaMediaState, AtaUint16 ByteOffset);
/*SUSMIT - Support for media which can support multi-block operations, user configurable*/
#endif /* ATA_HDD */
  /*-----------------2002-08-20 1:02PM----------------
   * The following globals should be referenced
   * through the state structure
   * Also, it should be noted which functions use these
   * so that they can be sure to be initialized before
   * those functions are called.
   * These will anyway be initilized before ATA_systemInit
   * --------------------------------------------------*/
  AtaUint16 (*get_mod_time)();
  AtaUint16 (*get_mod_date)();
  AtaUint16 (*get_time)();
  AtaUint16 (*get_date)();
  AtaUint16 *_AtaWriteBuffer;
  AtaSector _AtaWriteCurrentPhySector;
} AtaState;

/* File Descriptor */
typedef struct AtaFileStruct
{
  char Filename[9];               /* unpacked characters for filename (00XX, where XX is the ASCII character */
                                  /* null terminated (last character is 0000) */
  char Ext[4];                    /* unpacked characters for extension (00XX, where XX is the ASCII character */
                                  /* null terminated (last character is 0000) */
  AtaUint16    Attributes;        /* file attribute byte in LSB */
  AtaUint16    Time;              /* time record from FDB */
  AtaUint16    Date;              /* date record from FDB */
  AtaUint16    reserved1;         /* was unsigned int ByteSwap; byte swap mode for AtaRead and AtaWrite */
  AtaCluster   StartCluster;      /* starting cluster number */
  AtaFileSize  Size;              /* current file size in bytes */
  AtaCluster   Cluster;           /* current cluster number (initialized to StartCluster) */
  AtaUint16    WordInCluster;     /* current byte offset from start of cluster (initialized to 0) */
  AtaCluster   NextBrokenCluster; /* next cluster where fragment occurs (for Read and Write) */
  AtaCluster   PrevBrokenCluster; /* first cluster where contiguous cluster chain started (used for Write) */
  AtaSector    CurrentDirSector;  /* sector of start of current directory entries  (only used for root dir)*/
  AtaDirEntry  CurrentDirEntry;   /* entry number in current directory */
  AtaDirEntry CurrentDirSectorTag; /* number of clusters from start of dir of CurrentDirSector */
  AtaCluster StartDirCluster;   /* directory starting cluster number */
/*  AtaCluster   CurrentDirCluster;*/ /* cluster where current directory is located */
 /*SUSMIT_ DIRECTORY BRWOSING BUGFIX*/
  AtaFileSize  CurrentByte;       /* current byte in the file */
  AtaState     *pDrive;           /* pointer to drive state structure */
} AtaFile;


/*-----------------2002-08-20 4:14PM----------------
 * These file names are no longer valid.  I created
 * several new files to aide with the vector.
 * --------------------------------------------------*/
/* -----------------5/16/2000 4:18PM-----------------
Ata.c (included in ldknl)
 --------------------------------------------------*/
/* SUSMIT - 25 FEB 2003 */
/*AtaError ATA_systemInit(AtaState *pAtaDrive);*/
AtaError ATA_systemInit(AtaState *pAtaDrive, AtaUint16 disk_type);
/* SUSMIT - 25 FEB 2003 */
AtaUint16 ATA_getSerialID(AtaState *pAtaDrive, AtaSerialID *pID);

/* -----------------5/16/2000 4:19PM-----------------
AtaRead.c (included in ldknl)
 --------------------------------------------------*/
AtaError ATA_read(AtaFile *pAtaFile, AtaUint16 *Data, AtaUint16 Words);
AtaError ATA_readLittleEndian(AtaFile *pAtaFile, AtaUint16 *Data, AtaUint16 Words);
/*SUSMIT - Absolute Seek*/
AtaError ATA_seek(AtaFile *pAtaFile, AtaFileSize OffsetFromStart);
/*SUSMIT - Relative Seek*/
AtaError ATA_seek_relative(AtaFile *pAtaFile, long OffsetFromCurrent);
AtaError ATA_sleep(AtaState *pAtaDrive);

/* -----------------5/16/2000 4:19PM-----------------
AtaDir.c
 --------------------------------------------------*/
AtaError ATA_fileInit(AtaState *pAtaDrive, AtaFile *pAtaFile);
AtaError ATA_cdRoot(AtaFile *pAtaFile);
AtaError ATA_cd(AtaFile *pAtaFile);
AtaError ATA_findFirst(AtaFile *pAtaFile);
AtaError ATA_findNext(AtaFile *pAtaFile);
int ATA_isDir(AtaFile *pAtaFile);
void     ATA_errorCallback(unsigned short Error);

/* -----------------5/16/2000 4:19PM-----------------
AtaLong.c
 --------------------------------------------------*/
AtaError ATA_getLongName(AtaFile *pAtaFile, char *LongName, AtaUint16 StartOffset, AtaUint16 Length);

/* -----------------5/16/2000 4:19PM-----------------
AtaTell.c
 --------------------------------------------------*/
AtaError ATA_tell(AtaFile *pAtaFile, AtaFileSize *pOffsetInFile);

/* -----------------5/16/2000 4:20PM-----------------
AtaWrite.c
 --------------------------------------------------*/
AtaError ATA_create(AtaFile *pAtaFile);
AtaError ATA_createDirectory(AtaFile *pAtaFile);
AtaError ATA_delete(AtaFile *pAtaFile);
AtaError ATA_rename(AtaFile *pAtaFile);
AtaError ATA_write(AtaFile *pAtaFile, AtaUint16 *Data, AtaUint16 Words);
AtaError ATA_close(AtaFile *pAtaFile);

/* -----------------07/27/2000 10:53PM---------------
AtaFormat.c (Does exist now! - Susmit)
 --------------------------------------------------*/
AtaError ATA_format(AtaState *pDrive, int flag);

/*SUSMIT_UPDATE
Adding some more user level APIs.
Added in AtaDir.c*/
AtaError ATA_setFileName(AtaFile *pAtaFile, char *name, char *ext);
AtaError ATA_setDirectoryName(AtaFile *pAtaFile, char *dirname);
/*Added in AtaDisk.c*/
AtaUint32 ATA_diskSize(AtaState *pAtaState, AtaError *ret_stat);
AtaUint32 ATA_diskUsed(AtaState *pAtaState, AtaError *ret_stat);
AtaUint32 ATA_diskFree(AtaState *pAtaState, AtaError *ret_stat);

/*SUSMIT - new requirements*/
AtaError ATA_setDateTime(AtaFile *pAtaFile);
AtaError ATA_setAttr(AtaFile *pAtaFile, AtaUint16 Attr);
AtaError ATA_vol(AtaState *pAtaDrive, char *volname);
AtaError ATA_label(AtaState *pAtaDrive, char *volname);
AtaError ATA_setLongFileName( AtaFile *pAtaFile, char *LongName );
AtaError ATA_createLong(AtaFile *pAtaFile, char *longname);
AtaError ATA_deleteLong(AtaFile *pAtaFile);
AtaError ATA_renameLong(AtaFile *pAtaFile, char *longnamenew);
AtaError ATA_setLongDirectoryName( AtaFile *pAtaFile, char *LongName );
AtaError ATA_createDirectoryLong(AtaFile *pAtaFile, char *longname);
/*SUSMIT - new requirements*/
/*SUSMIT - USB_MSC Requirements*/
AtaError ATA_readSector(AtaSector PhySector, AtaState* pAtaState, AtaUint16 *Word, int ByteSwap);
AtaError ATA_writeSector(AtaSector PhySector, AtaState* pAtaState, AtaUint16 *Word, int ByteSwap);
/*SUSMIT - USB_MSC Requirements*/
/*SUSMIT - Configurable MFW Requirements*/
void ATA_enableMFW(AtaState *pAtaState);
#ifdef ATA_HDD
void ATA_enableMultiBlock(AtaState *pAtaState);
#endif /* ATA_HDD */
/*SUSMIT - Configurable MFW Requirements*/
/*SUSMIT - Byte Level File Handling Requirements*/
AtaError ATA_read_b(AtaFile *pAtaFile, AtaUint16 *Data, AtaUint16 bytes);
AtaError ATA_seek_b(AtaFile *pAtaFile, AtaUint32 ByteOffsetFromStart);
AtaError ATA_write_b(AtaFile *pAtaFile, AtaUint16 *Data, AtaUint16 bytes);
AtaError ATA_tell_b(AtaFile *pAtaFile, AtaFileSize *pByteOffsetInFile);
/*SUSMIT - Byte Level File Handling Requirements*/
#endif

