/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_mmcsd_ataIf.c
 *
 *  @brief Functional layer API source file for MMCSD ATA file system interface
 *
 *  Path: \(CSLPATH)\ src
 */

/* ============================================================================
 * Revision History
 * ================
 * 20-May-2009 Created
 * 26-Jun-2009 Code modified as per review comments
 * 03-Mar-2010 Added get_mod_time() and get_mod_date() routines  KR030810
 * 21-Mar-2010 Added MMC_readNextWord1 routine                   KR032110
 * 06-Apr-2010 - Incorporated Frank's comments
 * 11-Apr-2010 Added write sector cache in  MMC_readWriteControl      KR041110
 *             added write sector cache flush to MMC_issueReadCommand KR041110
 * 13-Apr-2010 simplified MMC_AtaReadNextWord1 routine                KR041310
 * 26-Apr-2010 - Added support for 'ATA_No_Swap' used by the ATA FS
 * 30-Apr-2010 - Incorporated internal review comments
 * ============================================================================
 */

#include "csl_mmcsd.h"
#include "csl_mmcsd_ataIf.h"

#define ENABLE_CACHE 1

#pragma DATA_ALIGN(gReadBuf ,4);
Uint16 gReadBuf[CSL_MMCSD_PAGE_SIZE/2] = {0xbb,};

Uint16       gWordCount = 256;
AtaSector    gNextPhySector = 0;

#ifdef ENABLE_CACHE
#pragma DATA_ALIGN(gWriteBuf ,4);
Uint16 gWriteBuf[2*CSL_MMCSD_PAGE_SIZE];

AtaSector cached_phySector0,cached_phySector1,cached_phySector2,cached_phySector3;
AtaUint16 nbuf_cached=0;
#endif

#ifdef USE_ATA_NO_SWAP
/*
 * This variable controls the DMA word swap and MMCSD endian mode.
 *  ATA_No_Swap = 0  - MMCSD will be configured for little endian mode.
 *                   - SW word swap will be done before MMCSD write and after
 *                     MMCSD read in DMA mode.
 *
 *  ATA_No_Swap = 1  - MMCSD will be configured for big endian mode
 *                   - No SW word swap will be done during DMA data transfers.
 *
 * NOTE: Comment the macro 'USE_ATA_NO_SWAP' defined in csl_mmcsd_ataIf.h to
 * disable the usage of 'ATA_No_Swap'.
 */
AtaUint16    ATA_No_Swap = 0;

#endif

/** ===========================================================================
 *   @n@b MMC_initState
 *
 *   @b Description
 *   @n This function initializes ATA state structure. Initializes all function
 *      pointers in the ATA state structure with appropriate function address.
 *      This function should be called by the application programs before
 *      transferring the control over the media to ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            pDrive    ATA state structure pointer
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes ATA state structure
 *
 *   @b Modifies
 *   @n ATA state structure
 *
 *   @b Example
 *   @verbatim
            AtaState       ataState;
            AtaError       ataErr;

            ataErr = MMC_initState(&ataState);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_initState(AtaState *pDrive)
{
    AtaMMCState    *ataMMCState;

    if ((pDrive == NULL) || (pDrive->pAtaMediaState == NULL))
    {
        return (ATA_ERROR_INVALID_PARAM);
    }

    ataMMCState = pDrive->pAtaMediaState;

    ataMMCState->pDrive = pDrive;

    pDrive->AtaReset                   = MMC_reset;
    pDrive->AtaBusyStatusCheck         = MMC_busyStatusCheck;
    pDrive->AtaGetSerialID             = MMC_getSerialID;
    pDrive->AtaIssueReadCommand        = MMC_issueReadCommand;
    pDrive->AtaCommandReadyStatusCheck = MMC_commandReadyStatusCheck;
    pDrive->AtaDataReadyStatusCheck    = MMC_dataReadyStatusCheck;
    pDrive->AtaReadNextWord            = MMC_readNextWord;
    pDrive->AtaReadNextWord1           = MMC_readNextWord1;
    pDrive->AtaReadNextNWords          = MMC_readNextNWords;
  //pDrive->AtaWriteSector             = MMC_writeSector;        //KR041110
    pDrive->AtaWriteSector             = MMC_readWriteControl;   //KR041110
    pDrive->AtaWriteSectorFlush        = MMC_writeSectorFlush;
    pDrive->AtaFileSystemType          = MMC_file_system_format;
    pDrive->AtaFlushFat                   = MMC_flushFat;
    pDrive->get_date                   = MMC_get_date;
    pDrive->get_time                   = MMC_get_time;
    pDrive->get_mod_date               = MMC_get_mod_date; //KR030810
    pDrive->get_mod_time               = MMC_get_mod_time; //KR030810

    return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_readWriteControl  //KR041110
 *
 *   @b Description
 *   @n This function is used to cache and write data to the media. This function
 *      can write 1 to 4 sectors each sector containing 512 bytes
 *
 *   @b Arguments
 *   @verbatim
            phySector         Sector number from which data needs to be read
            pAtaMediaState    ATA media state structure
            pWord             Write data buffer
            last_sector       Flag to flush cache
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *   @li        ATA_ERROR_TIMEOUT       - Time out Error Occurred
 *   @li        ATA_ERROR_UNSUPPORTED   - MMC/SD hardware is not supported
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Writes Data to MMC/SD card
 *
 *   @b Modifies
 *   @n MMCSD Sector data
 *
 *   @b Example
 *   @verbatim
             #define CSL_MMCSD_BUF_SIZE (256u)

            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaSector      phySector;
            AtaUint16      writeBuf[CSL_MMCSD_BUF_SIZE];

            pAtaState = &ataState;

            phySector = 0;
            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            ataErr = pAtaState->AtaWriteSector(phySector,
                                               ataState->pAtaMediaState,
                                               writeBuf, FALSE);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_readWriteControl (AtaSector    phySector,
                               void         *pAtaMediaState,
                               AtaUint16    *pWord,
                               int          last_sector)
{
  AtaError ataResult;
  AtaUint16 idx;

  ataResult = ATA_ERROR_NONE;

#ifdef ENABLE_CACHE
  if (ATA_No_Swap)
  {
    if (nbuf_cached==0)
    {
      if (last_sector)
      {
        ataResult=MMC_writeSector(phySector,pAtaMediaState,pWord,0);
        if (ataResult!=ATA_ERROR_NONE)
        {
          return(ataResult);
        }
      }
      else
      {
        cached_phySector0=phySector;
        for (idx=0;idx<CSL_MMCSD_PAGE_SIZE/2;idx++) gWriteBuf[idx]=pWord[idx];
        nbuf_cached++;
      }
    }
    else
    {
      if (nbuf_cached==1)
      {
        if (phySector!=cached_phySector0+1)
        {
          ataResult=MMC_writeSector (cached_phySector0,pAtaMediaState,gWriteBuf,0);
          if (ataResult!=ATA_ERROR_NONE)
          {
            return(ataResult);
          }
          ataResult=MMC_writeSector (phySector,pAtaMediaState,pWord,0);
          if (ataResult!=ATA_ERROR_NONE)
          {
            return(ataResult);
          }
          nbuf_cached=0;
        }
        else
        {
          cached_phySector1=phySector;
          for (idx=0;idx<CSL_MMCSD_PAGE_SIZE/2;idx++) gWriteBuf[idx+CSL_MMCSD_PAGE_SIZE/2]=pWord[idx];
          if (last_sector)
          {
            ataResult=MMC_writeNSectors(pAtaMediaState,gWriteBuf,cached_phySector0,2);
            if (ataResult!=ATA_ERROR_NONE)
            {
              return(ataResult);
            }
            nbuf_cached=0;
          }
          else
          {
            nbuf_cached++;
          }
        }
        //nbuf_cached=0;
      } //if (nbuf_cached==1)
      else
      {
        if (nbuf_cached==2)
        {
          if (phySector!=cached_phySector1+1)
          {
            ataResult=MMC_writeNSectors(pAtaMediaState,gWriteBuf,cached_phySector0,2);
            if (ataResult!=ATA_ERROR_NONE)
            {
              return(ataResult);
            }
            ataResult=MMC_writeSector (phySector,pAtaMediaState,pWord,0);
            if (ataResult!=ATA_ERROR_NONE)
            {
              return(ataResult);
            }
            nbuf_cached=0;
          }
          else
          {
            cached_phySector2=phySector;
            for (idx=0;idx<CSL_MMCSD_PAGE_SIZE/2;idx++) gWriteBuf[idx+CSL_MMCSD_PAGE_SIZE]=pWord[idx];
            if (last_sector)
            {
              ataResult=MMC_writeNSectors(pAtaMediaState,gWriteBuf,cached_phySector0,3);
              if (ataResult!=ATA_ERROR_NONE)
              {
                return(ataResult);
              }
              nbuf_cached=0;
            }
            else
            {
              nbuf_cached++;
            }
          }
          //nbuf_cached=0;
        } //if (nbuf_cached==2)
        else
        {
          if (nbuf_cached==3)
          {
            if (phySector!=cached_phySector2+1)
            {
              ataResult=MMC_writeNSectors(pAtaMediaState,gWriteBuf,cached_phySector0,3);
              if (ataResult!=ATA_ERROR_NONE)
              {
                return(ataResult);
              }
              ataResult=MMC_writeSector (phySector,pAtaMediaState,pWord,0);
              if (ataResult!=ATA_ERROR_NONE)
              {
                return(ataResult);
              }
            }
            else
            {
              cached_phySector3=phySector;
              for (idx=0;idx<CSL_MMCSD_PAGE_SIZE/2;idx++) gWriteBuf[idx+3*CSL_MMCSD_PAGE_SIZE/2]=pWord[idx];
              ataResult=MMC_writeNSectors(pAtaMediaState,gWriteBuf,cached_phySector0,4);
              if (ataResult!=ATA_ERROR_NONE)
              {
                return(ataResult);
              }
            }
            nbuf_cached=0;

          } //if (nbuf_cached==3)
        } //if (nbuf_cached!=2)
      } //if (nbuf_cached!=1)
    }
  }
  else
  {
    ataResult=MMC_writeSector (phySector,pAtaMediaState,pWord,last_sector);
    if (ataResult!=ATA_ERROR_NONE)
    {
      return(ataResult);
    }
  }
#else
    ataResult=MMC_writeSector (phySector,pAtaMediaState,pWord,last_sector);
    if (ataResult!=ATA_ERROR_NONE)
    {
      return(ataResult);
    }
#endif

  return(ataResult);

}

/** ===========================================================================
 *   @n@b MMC_issueReadCommand
 *
 *   @b Description
 *   @n This function is used to fetch all normal data from the media.
 *      A sector is always assumed to be 256 16-bit words by the high-level API
 *
 *      Programs calling this function should assign proper value to ATA_No_Swap
 *      in case of using that varaible.
 *
 *   @b Arguments
 *   @verbatim
            phySector         Sector number from which data needs to be read
            pAtaMediaState    ATA media state structure
            sectorCount       Number of sectors to read
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *   @li        ATA_ERROR_TIMEOUT       - Time out Error Occurred
 *   @li        ATA_ERROR_UNSUPPORTED   - MMC/SD hardware is not supported
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Reads Data from MMC/SD card
 *
 *   @b Modifies
 *   @n Global Read buffer
 *
 *   @b Example
 *   @verbatim
            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaSector      phySector;
            AtaUint16      sectCnt;

            pAtaState = &ataState;

            phySector = 0;
            sectCnt   = 16;
            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            ataErr = pAtaState->AtaIssueReadCommand(phySector,
                                                    ataState->pAtaMediaState,
                                                    sectCnt);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_issueReadCommand (AtaSector    phySector,
                               void         *pAtaMediaState,
                               AtaUint16    sectorCount)
{
    CSL_Status         result;
    AtaError           ataResult;
    AtaMMCState        *pMediaState;
    CSL_MmcsdHandle    hMmcSd;
    AtaUint16          ATA_No_Swap_sav;

#ifdef USE_ATA_NO_SWAP
    CSL_MmcsdEndianMode    readEndianMode;
#endif

    result    = CSL_SOK;
    ataResult = ATA_ERROR_NONE;

//////////////////////////////////////////////////////////////////////////////
// flush write sector cache KR041110
//////////////////////////////////////////////////////////////////////////////
#ifdef ENABLE_CACHE
    if (nbuf_cached==1) {
      ATA_No_Swap_sav=ATA_No_Swap;
      ATA_No_Swap=1;
      ataResult=MMC_writeSector (cached_phySector0,pAtaMediaState,gWriteBuf,0);
      if (ataResult!=ATA_ERROR_NONE)
      {
        return(ataResult);
      }
      nbuf_cached=0;
      ATA_No_Swap=ATA_No_Swap_sav;
    }
    else
    {
      if (nbuf_cached==2)
      {
        ATA_No_Swap_sav=ATA_No_Swap;
        ATA_No_Swap=1;
        ataResult=MMC_writeNSectors(pAtaMediaState,gWriteBuf,cached_phySector0,2);
        if (ataResult!=ATA_ERROR_NONE)
        {
          return(ataResult);
        }
        nbuf_cached=0;
        ATA_No_Swap=ATA_No_Swap_sav;
      }
      else
      {
        if (nbuf_cached==3)
        {
          ATA_No_Swap_sav=ATA_No_Swap;
          ATA_No_Swap=1;
          ataResult=MMC_writeNSectors(pAtaMediaState,gWriteBuf,cached_phySector0,3);
          if (ataResult!=ATA_ERROR_NONE)
          {
            return(ataResult);
          }
          nbuf_cached=0;
          ATA_No_Swap=ATA_No_Swap_sav;

        }
      }
    }
#endif
//////////////////////////////////////////////////////////////////////////////

    pMediaState = (AtaMMCState *)pAtaMediaState;

    if ((NULL == pMediaState)         ||
        (NULL == pMediaState->hMmcSd) ||
        (0 == sectorCount))
    {
        ataResult = ATA_ERROR_INVALID_PARAM;
        return (ataResult);
    }
    else
    {
        hMmcSd = (CSL_MmcsdHandle)pMediaState->hMmcSd;

        if (phySector > (hMmcSd->cardObj->totalSectors - 1))
        {
            ataResult = ATA_ERROR_INVALID_PARAM;
                return (ataResult);
        }
        else
        {

#ifdef USE_ATA_NO_SWAP

            readEndianMode  = hMmcSd->readEndianMode;

            if (1 == ATA_No_Swap)
            {
                hMmcSd->readEndianMode  = CSL_MMCSD_ENDIAN_BIG;
            }
            else
            {
                hMmcSd->readEndianMode  = CSL_MMCSD_ENDIAN_LITTLE;
            }
#endif
            /* Keep track of the word count and sector number */
            gWordCount = 0;
            gNextPhySector = phySector + 1;

            if(hMmcSd->cardObj->sdHcDetected == TRUE)
            {
                /* For the SDHC card Block addressing will be used.
                   Sector address will be same as sector number */
                result = MMC_read(hMmcSd, phySector,
                                  CSL_MMCSD_PAGE_SIZE, gReadBuf);
            }
            else
            {
                /* For the SD card Byte addressing will be used.
                  Sector address will be product of  sector number and sector size */
                result = MMC_read(hMmcSd, ((phySector)*(CSL_MMCSD_PAGE_SIZE)),
                                  CSL_MMCSD_PAGE_SIZE, gReadBuf);
            }

#ifdef USE_ATA_NO_SWAP
            hMmcSd->readEndianMode  = readEndianMode;
#endif

            if (result != CSL_SOK)
            {
                ataResult = getAtaErrorNo(result);
                    return(ataResult);
            }
        }
    }

    return(ataResult);
}

/** ===========================================================================
 *   @n@b MMC_readNextWord
 *
 *   @b Description
 *   @n This function is used to read a word next to the previously read word
 *      from the media. This function reads the words from the buffered data
 *      and send to the application. After reading 256 words from the global
 *      read buffer, this functions reads 256 words from the media and stores
 *      in the global read buffer. MMC_issueReadCommand functions should be
 *      called before calling this function for first time.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
            pWord             Pointer to store the data word
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *   @li        ATA_ERROR_TIMEOUT       - Time out Error Occurred
 *   @li        ATA_ERROR_UNSUPPORTED   - MMC/SD hardware is not supported
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *   @n  MMC_issueReadCommand should be called when calling MMC_readNextWord
 *       function for first time
 *
 *   <b> Post Condition </b>
 *   @n  Copies a data word into pWord
 *
 *   @b Modifies
 *   @n Data in pWord
 *
 *   @b Example
 *   @verbatim

            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaUint16      data;
            AtaSector      phySector;
            AtaUint16      sectCnt;

            pAtaState = &ataState;

            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            ataErr = pAtaState->AtaIssueReadCommand(phySector,
                                                    ataState->pAtaMediaState,
                                                    sectCnt);

            ataErr = pAtaState->AtaReadNextWord(ataState->pAtaMediaState,
                                                &data);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_readNextWord (void         *pAtaMediaState,
                           AtaUint16    *pWord)
{
    AtaMMCState    *pMediaState;
    AtaError       ataResult;

    ataResult = ATA_ERROR_NONE;

    if ((NULL == pAtaMediaState) ||
        (NULL == pWord))
    {
        ataResult = ATA_ERROR_INVALID_PARAM;
        return (ataResult);
    }

    pMediaState = (AtaMMCState *)pAtaMediaState;

    if(256 == gWordCount)
    {
        ataResult = MMC_issueReadCommand(gNextPhySector, pMediaState, 1);
    }

    *pWord = gReadBuf[gWordCount++];

    return (ataResult);
}

/** ===========================================================================
 *   @n@b MMC_readNextWord1
 *
 *   @b Description
 *   @n This function is used to read a word next to the previously read word
 *      from the media. This function reads the words from the buffered data
 *      and send to the application. After readinmg 256 words from the global
 *      read buffer, this functions reads 256 words from the media and stores
 *      in the global read buffer. MMC_issueReadCommand functions should be
 *      called before calling this function for first time.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
            pWord             Pointer to store the data word
            word_delta        word increment
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *   @li        ATA_ERROR_TIMEOUT       - Time out Error Occurred
 *   @li        ATA_ERROR_UNSUPPORTED   - MMC/SD hardware is not supported
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *   @n  MMC_issueReadCommand should be called when calling MMC_readNextWord
 *       function for first time
 *
 *   <b> Post Condition </b>
 *   @n  Copies a data word into pWord
 *
 *   @b Modifies
 *   @n Data in pWord
 *
 *   @b Example
 *   @verbatim

            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaUint16      data;
            AtaSector      phySector;
            AtaUint16      sectCnt;

            pAtaState = &ataState;

            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            ataErr = pAtaState->AtaIssueReadCommand(phySector,
                                                    ataState->pAtaMediaState,
                                                    sectCnt);

            ataErr = pAtaState->AtaReadNextWord(ataState->pAtaMediaState,
                                                &data);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_readNextWord1 (void         *pAtaMediaState,  // KR032110
                            AtaUint16    *pWord,
                            AtaUint16    word_delta)
{
    AtaMMCState  *pMediaState;
    AtaError     ataResult;
    AtaUint16    temp;

    ataResult = ATA_ERROR_NONE;

    if (NULL == pAtaMediaState)
    {
        ataResult = ATA_ERROR_INVALID_PARAM;
        return (ataResult);
    }

    pMediaState = (AtaMMCState *)pAtaMediaState;

/*
    if(gWordCount+word_delta>=256)
    {
      temp=gWordCount+word_delta-256;
      ataResult = MMC_issueReadCommand(gNextPhySector, pMediaState, 1);
      gWordCount=temp;
    } else
    {
      gWordCount+=word_delta;
    }

    *pWord = gReadBuf[gWordCount++];
*/
    // KR041310
    if (word_delta>=256)
    {
      temp=word_delta-256;
      ataResult = MMC_issueReadCommand(gNextPhySector, pMediaState, 1);
      gWordCount=temp;
    }
    else
    {
      gWordCount=word_delta;
    }

    *pWord = gReadBuf[gWordCount++];

    return (ataResult);
}


/** ===========================================================================
 *   @n@b MMC_readNextNWords
 *
 *   @b Description
 *   @n This function is used to read N words next to the previously read word
 *      from the media. This function calls MMC_readNextWord API N number of
 *      times to read the data words. High level APIs should be aware of this
 *      and use MMC_readNextWord and MMC_readNextNWords APIs with care.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
            pData             Pointer to store the data words
            word              Number of words to read
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *   @li        ATA_ERROR_TIMEOUT       - Time out Error Occurred
 *   @li        ATA_ERROR_UNSUPPORTED   - MMC/SD hardware is not supported
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Reads data from media
 *
 *   @b Modifies
 *   @n Global Read buffer
 *
 *   @b Example
 *   @verbatim
             #define CSL_MMCSD_BUF_SIZE (16u)

            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaUint16      dataBuf[CSL_MMCSD_BUF_SIZE];
            AtaUint16      words;

            pAtaState = &ataState;

            words  = CSL_MMCSD_BUF_SIZE;
            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            ataErr = pAtaState->AtaReadNextNWords(ataState->pAtaMediaState,
                                                  dataBuf, words);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_readNextNWords (void         *pAtaMediaState,
                             AtaUint16    *pData,
                             AtaUint16    words)
{
    AtaError       result;
    Uint16         index;
    AtaMMCState    *pMediaState;
    //Uint16         wordCount;

    result = ATA_ERROR_NONE;

    if ((NULL == pAtaMediaState) ||
        (NULL == pData) ||
        (0 == words))
    {
        return (ATA_ERROR_INVALID_PARAM);
    }

    pMediaState = (AtaMMCState *)pAtaMediaState;
    //wordCount = pMediaState->pDrive->CurrentWord;

    for(index = 0; index < words; index++)
    {
        result = MMC_readNextWord(pMediaState, (pData + index));
        if (result != ATA_ERROR_NONE)
        {
            break;
        }
    }

    return(result);
}

/** ===========================================================================
 *   @n@b MMC_writeSector
 *
 *   @b Description
 *   @n This function is used to write data to the media. This function can
 *      write 512 bytes of data at one time.
 *
 *      Programs calling this function should assign proper value to ATA_No_Swap
 *      in case of using that varaible.
 *
 *   @b Arguments
 *   @verbatim
            phySector         Sector number from which data needs to be read
            pAtaMediaState    ATA media state structure
            pWord             Write data buffer
            byteSwap          Flag to request for the byte swap
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *   @li        ATA_ERROR_TIMEOUT       - Time out Error Occurred
 *   @li        ATA_ERROR_UNSUPPORTED   - MMC/SD hardware is not supported
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Writes Data to MMC/SD card
 *
 *   @b Modifies
 *   @n MMCSD Sector data
 *
 *   @b Example
 *   @verbatim
             #define CSL_MMCSD_BUF_SIZE (256u)

            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaSector      phySector;
            AtaUint16      writeBuf[CSL_MMCSD_BUF_SIZE];

            pAtaState = &ataState;

            phySector = 0;
            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            ataErr = pAtaState->AtaWriteSector(phySector,
                                               ataState->pAtaMediaState,
                                               writeBuf, FALSE);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_writeSector (AtaSector    phySector,
                          void         *pAtaMediaState,
                          AtaUint16    *pWord,
                          int          byteSwap)
{
    CSL_Status         result;
    AtaError           ataResult;
    AtaMMCState        *pMediaState;
    CSL_MmcsdHandle    hMmcSd;

#ifdef USE_ATA_NO_SWAP
    CSL_MmcsdEndianMode    writeEndianMode;
#endif

    result    = CSL_SOK;
    ataResult = ATA_ERROR_NONE;

    pMediaState = (AtaMMCState *)(pAtaMediaState);

    if((NULL == pAtaMediaState)      ||
       (NULL == pMediaState->hMmcSd) ||
       (NULL == pWord))
    {
        return (ATA_ERROR_INVALID_PARAM);
    }

    hMmcSd = (CSL_MmcsdHandle)pMediaState->hMmcSd;

    if(phySector > (hMmcSd->cardObj->totalSectors - 1))
    {
        ataResult = ATA_ERROR_INVALID_PARAM;
        return (ataResult);
    }
    else
    {
#ifdef USE_ATA_NO_SWAP

        writeEndianMode = hMmcSd->writeEndianMode;

        if (1 == ATA_No_Swap)
        {
            hMmcSd->writeEndianMode = CSL_MMCSD_ENDIAN_BIG;
        }
        else
        {
            hMmcSd->writeEndianMode = CSL_MMCSD_ENDIAN_LITTLE;
        }
#endif

        /* Write Data to media */
        if(hMmcSd->cardObj->sdHcDetected == TRUE)
        {
            /* For the SDHC card Block addressing will be used.
               Sector address will be same as sector number */

            result = MMC_write(hMmcSd, phySector, CSL_MMCSD_PAGE_SIZE, pWord);
        }
        else
        {
            /* For the SD card Byte addressing will be used.
              Sector address will be product of  sector number and sector size */
            result = MMC_write(hMmcSd, ((phySector)*(CSL_MMCSD_PAGE_SIZE)),
                               CSL_MMCSD_PAGE_SIZE, pWord);
        }

#ifdef USE_ATA_NO_SWAP
            hMmcSd->writeEndianMode = writeEndianMode;
#endif

        if(result != CSL_SOK)
        {
            ataResult = getAtaErrorNo(result);
            return(ataResult);
        }
    }

    return(ataResult);
}

/** ===========================================================================
 *   @n@b MMC_readNSectors
 *
 *   @b Description
 *   @n This function is used to read N sectors from MMC/SD card.
 *      Data buffer should be large enough to hold the data read
 *      from multiple sectors. Each sector size will be 512 bytes
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
            pReadBuf          Pointer to store read data
            startSector       Starting sector number to read
            sectorCount       Number of sectors to read
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *   @li        ATA_ERROR_TIMEOUT       - Time out Error Occurred
 *   @li        ATA_ERROR_UNSUPPORTED   - MMC/SD hardware is not supported
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Reads data from media
 *
 *   @b Modifies
 *   @n Read buffer
 *
 *   @b Example
 *   @verbatim
             #define CSL_MMCSD_BUF_SIZE (256u)

            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaError       ataErr;
            AtaUint16      dataBuf[CSL_MMCSD_BUF_SIZE];
            AtaUint16      words;
            Uint16         sectNum;

            ataState->pAtaMediaState = &ataMediaState;
            words   = CSL_MMCSD_BUF_SIZE;
            sectNum = 0;

            ataErr = MMC_initState(&ataState);

            ataErr = MMC_readNSectors(ataState->pAtaMediaStatee, dataBuf,
                                      words, sectNum, 8);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_readNSectors (void         *pAtaMediaState,
                           AtaUint16    *pReadBuf,
                           AtaSector    startSector,
                           AtaUint16    sectorCount)
{
    AtaError           ataResult;
    CSL_Status         result;
    CSL_MmcsdHandle    hMmcSd;
    AtaMMCState        *pMediaState;
    Uint32             validSectors;

    ataResult = ATA_ERROR_NONE;
    result    = CSL_SOK;

    pMediaState = (AtaMMCState *)(pAtaMediaState);

    if ((NULL == pMediaState)         ||
        (NULL == pMediaState->hMmcSd) ||
        (NULL == pReadBuf)            ||
        (0 == sectorCount))
    {
        return (ATA_ERROR_INVALID_PARAM);
    }

    hMmcSd = (CSL_MmcsdHandle)pMediaState->hMmcSd;

    validSectors = hMmcSd->cardObj->totalSectors - startSector;

    if((startSector > (hMmcSd->cardObj->totalSectors - 1)) ||
       (sectorCount > validSectors))
    {
        ataResult = ATA_ERROR_INVALID_PARAM;
        return (ataResult);
    }
    else
    {
        if(hMmcSd->cardObj->sdHcDetected == TRUE)
        {
            /* For the SDHC card Block addressing will be used.
               Sector address will be same as sector number */
            result = MMC_read(hMmcSd, startSector,
                              ((CSL_MMCSD_PAGE_SIZE)*(sectorCount)), pReadBuf);
        }
        else
        {
            /* For the SD card Byte addressing will be used.
              Sector address will be product of  sector number and sector size */
            result = MMC_read(hMmcSd, ((startSector)*(CSL_MMCSD_PAGE_SIZE)),
                              ((CSL_MMCSD_PAGE_SIZE)*(sectorCount)), pReadBuf);
        }

        if (result != CSL_SOK)
        {
            ataResult = getAtaErrorNo(result);
        }
    }

    return(ataResult);
}

/** ===========================================================================
 *   @n@b MMC_writeNSectors
 *
 *   @b Description
 *   @n This function is used to write N number of sectors to MMC/SD card.
 *      Each sector size will be 512 bytes
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
            pWriteBuf         Pointer to write data buffer
            startSector       Starting sector number to write
            sectorCount       Number of sectors to write
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *   @li        ATA_ERROR_TIMEOUT       - Time out Error Occurred
 *   @li        ATA_ERROR_UNSUPPORTED   - MMC/SD hardware is not supported
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Writes data to media
 *
 *   @b Modifies
 *   @n Data in the MMC/SD card
 *
 *   @b Example
 *   @verbatim
             #define CSL_MMCSD_BUF_SIZE (256u)

            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaError       ataErr;
            AtaUint16      dataBuf[CSL_MMCSD_BUF_SIZE];
            AtaUint16      words;
            Uint16         sectNum;

            ataState->pAtaMediaState = &ataMediaState;
            words   = CSL_MMCSD_BUF_SIZE;
            sectNum = 0;

            ataErr = MMC_initState(&ataState);

            ataErr = MMC_writeNSectors(ataState->pAtaMediaState, dataBuf,
                                       words, sectNum, 8);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_writeNSectors (void         *pAtaMediaState,
                            AtaUint16    *pWriteBuf,
                            AtaSector    startSector,
                            AtaUint16    sectorCount)
{
    AtaError           ataResult;
    CSL_Status         result;
    CSL_MmcsdHandle    hMmcSd;
    AtaMMCState        *pMediaState;
    Uint32             validSectors;

    ataResult = ATA_ERROR_NONE;
    result    = CSL_SOK;

    pMediaState = (AtaMMCState *)(pAtaMediaState);

    if ((NULL == pMediaState)         ||
        (NULL == pMediaState->hMmcSd) ||
        (NULL == pWriteBuf)           ||
        (0 == sectorCount))
    {
        return (ATA_ERROR_INVALID_PARAM);
    }

    hMmcSd = (CSL_MmcsdHandle)pMediaState->hMmcSd;

    validSectors = hMmcSd->cardObj->totalSectors - startSector;

    if((startSector > (hMmcSd->cardObj->totalSectors - 1)) ||
       (sectorCount > validSectors))
    {
        ataResult = ATA_ERROR_INVALID_PARAM;
        return (ataResult);
    }
    else
    {
        /* Write Data to media */
        if(hMmcSd->cardObj->sdHcDetected == TRUE)
        {
            /* For the SDHC card Block addressing will be used.
               Sector address will be same as sector number */
            result = MMC_write(hMmcSd, startSector,
                               ((CSL_MMCSD_PAGE_SIZE)*(sectorCount)), pWriteBuf);
        }
        else
        {
            /* For the SD card Byte addressing will be used.
              Sector address will be product of  sector number and sector size */
            result = MMC_write(hMmcSd, ((startSector)*(CSL_MMCSD_PAGE_SIZE)),
                               ((CSL_MMCSD_PAGE_SIZE)*(sectorCount)), pWriteBuf);
        }

        if (result != CSL_SOK)
        {
            ataResult = getAtaErrorNo(result);
        }
    }

    return(ataResult);
}

/** ===========================================================================
 *   @n@b MMC_read_CID
 *
 *   @b Description
 *   @n This function is used to read the MMCSD card identification information.
 *      CID information stored in the internal MMCSD handle is copied to the
 *      given data buffer. This function stores 8 words each of length 16bit
 *      into the given buffer.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
            pBuf              Pointer to data buffer to store CID information
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_Open and MMC_sendAllCID/SD_sendAllCID APIs should be called
 *       successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Writes CID data to given buffer
 *
 *   @b Modifies
 *   @n Data in pBuf
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj    pMmcsdContObj;
            CSL_MmcsdHandle         mmcsdHandle;
            CSL_MMCCardIdObj         sdCardIdObj;
            AtaMMCState             ataMediaState;
            AtaState                ataState;
            AtaError                ataErr;
            unsigned int            csdBuf[8];
            CSL_Status              status;

            ataState->pAtaMediaState = &ataMediaState;

            //Open MMCSD module
            mmcsdHandle = MMC_open(&pMmcsdContObj, CSL_MMCSD0_INST,
                                   CSL_MMCSD_OPMODE_POLLED, &status);
            //Configure MMCSD module
            ....
            ....
            //Read the CID information form card and store it in MMCSD handle
            status = SD_sendAllCID(mmcsdHandle, &sdCardIdObj);

            //Read the CID data from the internal handle
             ataErr = MMC_read_CID(ataState->pAtaMediaState, csdBuf);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_read_CID (void            *pAtaMediaState,
                       unsigned int    *pBuf)
{
    CSL_MmcsdHandle    hMmcSd;
    AtaMMCState        *pMediaState;
       Int16              offset;
    Uint16             index;

    pMediaState = (AtaMMCState *)(pAtaMediaState);

    if ((NULL == pMediaState)         ||
        (NULL == pMediaState->hMmcSd) ||
        (NULL == pBuf))
    {
        return (ATA_ERROR_INVALID_PARAM);
    }

    hMmcSd = (CSL_MmcsdHandle)pMediaState->hMmcSd;

    index = 0;
    for(offset = 7; offset >= 0; offset--)
    {
        pBuf[index++] = (*((unsigned int*)(&(hMmcSd->csdSliceInfo)) + offset));
    }

    return (ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_read_CSD
 *
 *   @b Description
 *   @n This function is used to read the MMCSD card specific information.
 *      CSD information stored in the internal MMCSD handle is copied to the
 *      given data buffer. This function stores 8 words each of length 16bit
 *      into the given buffer.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
            pBuf              Pointer to data buffer to store CSD information
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_Open and MMC_getCardCsd/SD_getCardCsd APIs should be called
 *       successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Writes CSD data to given buffer
 *
 *   @b Modifies
 *   @n Data in pBuf
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj    pMmcsdContObj;
            CSL_MmcsdHandle         mmcsdHandle;
            CSL_MMCCardCsdObj         sdCardCsdObj;
            AtaMMCState             ataMediaState;
            AtaState                ataState;
            AtaError                ataErr;
            unsigned int            csdBuf[8];
            CSL_Status              status;

            ataState->pAtaMediaState = &ataMediaState;

            //Open MMCSD module
            mmcsdHandle = MMC_open(&pMmcsdContObj, CSL_MMCSD0_INST,
                                   CSL_MMCSD_OPMODE_POLLED, &status);
            //Configure MMCSD module
            ....
            ....
            //Read the CSD information form card and store it in MMCSD handle
            status = SD_getCardCsd(mmcsdHandle, &sdCardCsdObj);

            //Read the CSD data from the internal handle
             ataErr = MMC_read_CSD(ataState->pAtaMediaState, csdBuf);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_read_CSD (void            *pAtaMediaState,
                       unsigned int    *pBuf)
{
    CSL_MmcsdHandle    hMmcSd;
    AtaMMCState        *pMediaState;
       Int16              offset;
    Uint16             index;


    pMediaState = (AtaMMCState *)(pAtaMediaState);

    if ((NULL == pMediaState)         ||
        (NULL == pMediaState->hMmcSd) ||
        (NULL == pBuf))
    {
        return (ATA_ERROR_INVALID_PARAM);
    }

    hMmcSd = (CSL_MmcsdHandle)pMediaState->hMmcSd;

    index = 0;
    for(offset = 7; offset >= 0; offset--)
    {
        pBuf[index++] = (*((unsigned int*)(&(hMmcSd->csdSliceInfo)) + offset));
    }

    return (ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_writeSectorFlush
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;

            pAtaState = &ataState;

            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            ataErr = pAtaState->AtaWriteSectorFlush(ataState->pAtaMediaState);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_writeSectorFlush(void *pAtaMediaState)
{
    return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_commandReadyStatusCheck
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            int            status;

            pAtaState = &ataState;

            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            status = pAtaState->AtaCommandReadyStatusCheck(ataState->pAtaMediaState);

     @endverbatim
 *  ===========================================================================
 */
int MMC_commandReadyStatusCheck(void *pAtaMediaState)
{
    return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_dataReadyStatusCheck
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;

            pAtaState = &ataState;

            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            ataErr = pAtaState->AtaDataReadyStatusCheck(ataState->pAtaMediaState);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_dataReadyStatusCheck(void *pAtaMediaState)
{
    return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_busyStatusCheck
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li                    ATA_ERROR_NONE - Returned for success
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;

            pAtaState = &ataState;

            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            ataErr = pAtaState->AtaBusyStatusCheck(ataState->pAtaMediaState);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_busyStatusCheck (void *pAtaMediaState)
{
    return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_getSerialID
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
            pID               Pointer to store the serial Id
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li                    ATA_ERROR_NONE - Returned for success

 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaSerialID    serId;

            pAtaState= &ataState;

            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            retVal = pAtaState->AtaGetSerialID(ataState->pAtaMediaState,
                                               &serId);

     @endverbatim
 *  ===========================================================================
 */
AtaUint16 MMC_getSerialID (void           *pAtaMediaState,
                           AtaSerialID    *pSerID)
{
    return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_file_system_format
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaUint16      retVal;

            pAtaState = &ataState;

            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            retVal = pAtaState->AtaFileSystemType(ataState->pAtaMediaState);

     @endverbatim
 *  ===========================================================================
 */
AtaUint16 MMC_file_system_format (void *pAtaMediaState)
{
    return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_reset
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li                    ATA_ERROR_NONE - Returned for success
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;

            pAtaState = &ataState;

            ataState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            ataErr = pAtaState->AtaReset(ataState->pAtaMediaState);

     @endverbatim
 *  ===========================================================================
 */
AtaError MMC_reset(void *pAtaMediaState)
{
    return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_get_time
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            None
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaUint16      time;

            pAtaState = &ataState;

            ataErr = MMC_initState(&ataState);

            time   = pAtaState->get_time();

     @endverbatim
 *  ===========================================================================
 */
AtaUint16 MMC_get_time (void)
{
  return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_get_mod_time
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            None
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaUint16      time;

            pAtaState = &ataState;

            ataErr = MMC_initState(&ataState);

            time   = pAtaState->get_mod_time();

     @endverbatim
 *  ===========================================================================
 */
AtaUint16 MMC_get_mod_time (void)
{
  return(ATA_ERROR_NONE);
}


/** ===========================================================================
 *   @n@b MMC_get_date
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            None
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaUint16      date;

            pAtaState = &ataState;

            ataErr = MMC_initState(&ataState);

            date   = pAtaState->get_date();

     @endverbatim
 *  ===========================================================================
 */
AtaUint16 MMC_get_date (void)
{
  return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_get_mod_date
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            None
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;
            AtaUint16      date;

            pAtaState = &ataState;

            ataErr = MMC_initState(&ataState);

            date   = pAtaState->get_mod_date();

     @endverbatim
 *  ===========================================================================
 */
AtaUint16 MMC_get_mod_date (void)
{
  return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b MMC_flushFat
 *
 *   @b Description
 *   @n This function always returns success. No functionality is implemented.
 *      This function address is assigned to the function pointer provided by
 *      ATA file system.
 *
 *   @b Arguments
 *   @verbatim
            pAtaMediaState    ATA media state structure
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_NONE          - Returned for success
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_initState API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            AtaMMCState    ataMediaState;
            AtaState       ataState;
            AtaState       *pAtaState;
            AtaError       ataErr;

            pAtaState = *ataState;
            pAtaState->pAtaMediaState = &ataMediaState;

            ataErr = MMC_initState(&ataState);

            ataErr = pAtaState->AtaFlushFat(pAtaState->pAtaMediaState);

     @endverbatim
 *  ===========================================================================
 */
AtaError  MMC_flushFat (void *pAtaMediaState)
{
    return(ATA_ERROR_NONE);
}

/** ===========================================================================
 *   @n@b getAtaErrorNo
 *
 *   @b Description
 *   @n This function checks the return value send by the MMCSD CSL module
 *      and returns the corresponding error value to the ATA File system.
 *      This function is called in case of any error occurred
 *
 *   @b Arguments
 *   @verbatim
            result    Result returned by the CSL MMCSD module
     @endverbatim
 *
 *   <b> Return Value </b>  AtaError
 *   @li        ATA_ERROR_INVALID_PARAM - Returned for Invalid input parameters
 *   @li        ATA_ERROR_TIMEOUT       - Time out Error Occurred
 *   @li        ATA_ERROR_UNSUPPORTED   - MMC/SD hardware is not supported
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Returns ATA Error codes
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_Status    result;
            AtaError      ataErr;

            result = MMC_init();

            ataErr = getAtaErrorNo(result);

     @endverbatim
 *  ===========================================================================
 */
static AtaError getAtaErrorNo(CSL_Status    result)
{
    if (result == CSL_EMMCSD_TIMEOUT)
    {
        return(ATA_ERROR_TIMEOUT);
    }
    else if ((result == CSL_ESYS_INVPARAMS) || (result == CSL_ESYS_BADHANDLE))
    {
        return (ATA_ERROR_INVALID_PARAM);
    }
    else
    {
        return(ATA_ERROR_UNSUPPORTED);
    }
}

