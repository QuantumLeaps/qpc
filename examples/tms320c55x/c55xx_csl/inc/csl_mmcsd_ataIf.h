/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_mmcsd_ataIf.h
 *
 *  @brief Functional layer API header file for MMCSD ATA file system interface
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 20-May-2009 Created
 * 08-Mar-2010 Added get_mod_date and get_mod_time routines  KR030810
 * 21-Mar-2010 Added MMC_readNextWord1 routine               KR032110
 * 11-Apr-2010 Added write sector cache in  MMC_readWriteControl      KR041110
 *             added write sector cache flush to MMC_issueReadCommand KR041110
 * 06-Apr-2010 - Incorporated Frank's comments
 * 26-Apr-2010 - Added support for 'ATA_No_Swap' used by the ATA FS
 * 30-Apr-2010 - Incorporated internal review comments
 * ============================================================================
 */

#ifndef CSL_MMCSD_ATAIF_H
#define CSL_MMCSD_ATAIF_H

#include "ata.h"
#include "csl_types.h"

/** \addtogroup CSL_MMCSD_SYMBOL
* @{ */

/** MMCSD Sector size in bytes                                               */
#define CSL_MMCSD_PAGE_SIZE     (512u)

/** Macro to enable the control of MMCSD word swap using 'ATA_No_Swap'       */
#define USE_ATA_NO_SWAP         (TRUE)

/**
@} */


/**
\addtogroup CSL_MMCSD_DATASTRUCT
@{*/

/**
 * \brief Structure to hold the media ATA state values specific to MMCSD
 */
typedef struct AtaMMCState
{
    /** Handle to the MMC/SD CSL module                                      */
    void        *hMmcSd;
    /** Pointer to the ata state structure                                   */
    AtaState    *pDrive;
    /** Pointer kept for the future use                                      */
    void        *pMmcSdptr;
    /** Pointer to the read data buffer                                      */
    AtaUint16   *pReadBuf;
    /** Pointer to the write data buffer                                     */
    Uint16      *pWriteBuf;
    /** Number of the current sector read                                    */
    Uint32      currentsectorR;
    /** value of the current word read                                       */
    Uint32      currentwordR;
    /** Number of the current sector written                                 */
    Uint32      currentsectorWr;
    /** value of the current word written                                    */
    Uint32      currentwordWr;
    /** MMCSD call back pointer                                              */
    void (*mmcsdCallBack)(void);
} AtaMMCState;

/**
@} */

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
                               int          byteSwap);

/** @addtogroup CSL_MMCSD_FUNCTION
 @{ */

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
AtaError MMC_initState(AtaState *pDrive);

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
                               AtaUint16    sectorCount);

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
                           AtaUint16    *pWord);

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
                            AtaUint16    word_delta);

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
                             AtaUint16    words);

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
                          int          byteSwap);

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
                           AtaUint16    sectorCount);

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
                            AtaUint16    sectorCount);

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
                       unsigned int    *pBuf);

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
                       unsigned int    *pBuf);

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
AtaError MMC_writeSectorFlush(void *pAtaMediaState);

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
int MMC_commandReadyStatusCheck(void *pAtaMediaState);

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
AtaError MMC_dataReadyStatusCheck(void *pAtaMediaState);

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
AtaError MMC_busyStatusCheck (void *pAtaMediaState);

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
                           AtaSerialID    *pSerID);

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
AtaUint16 MMC_file_system_format (void *pAtaMediaState);

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
AtaError MMC_reset(void *pAtaMediaState);

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
AtaUint16 MMC_get_time (void);

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
AtaUint16 MMC_get_mod_time (void);

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
AtaUint16 MMC_get_date (void);

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
AtaUint16 MMC_get_mod_date (void);

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
AtaError  MMC_flushFat (void *pAtaMediaState);

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
static AtaError getAtaErrorNo(CSL_Status    result);

/**
@} */

#endif    /* CSL_MMCSD_ATAIF_H */

