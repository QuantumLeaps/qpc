/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @defgroup CSL_MMCSD_API MMCSD
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * MMCSD CSL provides set of APIs to configure and utilize the functionalities
 * provided by the C5505/C5515 MMCSD hardware controller.
 * C5505/C5515 MMCSD hardware controller provides interface for MMC,SD and SDIO
 * cards. MMCSD CSL supports the MMC and SD and SDHC cards.
 *
 * @subsection aaa Terms and Abbreviations
 *   -# CSL:  Chip Support Library
 *   -# API:  Application Programmer Interface
 *
 * @subsection References
 *    -# sprufo6.pdf
 *
 */

/** @file csl_mmcsd.h
 *
 *  @brief Functional layer API header file for MMC/SD CSL
 *
 *  Path: \(CSLPATH)/inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 16-Sep-2008 Added Header File of MMC/SD CSL.
 * 16-Jun-2009 Added DMA mode support
 * 26-Jun-2009 Code modified as per review comments
 * 26-Jun-2009 Added SDHC card support
 * 25-Feb-2010 Added second callback support  KR022510
 * 10-Mar-2010 Pedro updates
 * 05-Apr-2010 - Incorporated Frank's comments
 * 06-Apr-2010 - Removed MMC_setDmaCallback() API as the functionality of
 *               DMA callback function is replaced by write done callback function
 * 06-Apr-2010 - Renamed write done callback function to data transfer callback
 *               since it is used by both MMC write and read APIs. Renamed
 *               all the related functions and data structures
 * 08-Apr-2010 - Formatted the code to have uniform style
 * 10-Apr-2010 - Added doxygen comments for few enums and structures
 * 12-Apr-2010 - Merged the code with CSL PG2.0 modifications
 *               Added Enum CSL_MmcsdEndianMode to choose the endian mode of MMCSD
 *               Added API prototype for MMC_setEndianMode()
 * 16-Apr-2010 - Added API SD_configurePullup() to configure SD card pull-up
 *               resistor
 * 19-Apr-2010 - Added APIs
 *             - MMC_setBlockLength()
 *             - MMC_setWriteBlkEraseCnt()
 *             - SD_setBusWidth()
 *             - MMC_getCardStatus()
 * 30-Apr-2010 - Incorporated internal review comments
 * ============================================================================
 */

#ifndef _CSL_MMCSD_H_
#define _CSL_MMCSD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <soc.h>
#include <csl_types.h>
#include <csl_error.h>
#include <csl_dma.h>
#include <tistdtypes.h>

/**
\defgroup CSL_MMCSD_API MMCSD
   \internal Based on <b>BCG MMC/SD Controller Module Specification </b>
*/
/**
@defgroup CSL_MMCSD_SYMBOL MMC/SD Symbols Defined
@ingroup CSL_MMCSD_API
*/

/**
@defgroup CSL_MMCSD_DATASTRUCT MMC/SD Data Structures
@ingroup CSL_MMCSD_API
*/

/**
@defgroup CSL_MMCSD_ENUM  MMC/SD Enumerated Data Types
@ingroup CSL_MMCSD_API
*/

/**
@defgroup CSL_MMCSD_FUNCTION  MMC/SD Functions
@ingroup CSL_MMCSD_API
*/

/**
@defgroup CSL_MMCSDC_INLINE_FUNCTION  MMC/SD Inline Functions
@ingroup CSL_MMCSD_API
*/

/*****************************************************************************\
          MMCSD global macro declarations
\*****************************************************************************/

/** \addtogroup CSL_MMCSD_SYMBOL
* @{ */

/** stuff bits value for 16 bit variable                                     */
#define CSL_MMCSD_STUFF_BITS                    (0x0000)
/** Card Echo pattern for SD high capacity card                              */
#define CSL_SD_HC_ECHO_PATTERN                (0xAAu)
/** Card VHS bit response value for CMD8                                     */
#define CSL_SD_HC_VHS_ECHO                    (0x100u)
/** Max shift value for Uint16 data type variable                            */
#define CSL_MMCSD_SHIFT_MAX                   (16)
/** Max Mask value for Uint16 data type variable                             */
#define CSL_MMCSD_MASK_MAX                    (0xFFFF)
/** bit value in PCGCR for activate the mmcsd controller                     */
#define CSL_MMCSD_ACTIVE                      (0x0)
/** reset mmcsd bit field                                                    */
#define CSL_MMCSD_RESET                       (0x1u)
/** reset count for mmcsd                                                    */
#define CSL_MMCSD_RESET_COUNT                 (0x20u)
/** hash define value for enabling the particular bit                        */
#define CSL_MMCSD_ENABLE                      (0x1u)
/** hash define value for disabling the particular bit                       */
#define CSL_MMCSD_DISABLE                     (0x0)
/** ISR function ID for MMCSD0 Transmitting the data                         */
#define CSL_MMCSD_ISR_TXMT                    (0x0)
/** ISR function ID for MMCSD0 Receiving the data                            */
#define CSL_MMCSD_ISR_RCV                     (0x1)
/** Time delay                                                               */
#define CSL_MMCSD_DELAY_TIME                  (100U)
/** Time out duration for response                                           */
#define CSL_MMCSD_RESPONSE_TIMEOUT            (0xFFFF)
/** Time out duration for data read/write                                    */
#define CSL_MMCSD_DATA_RW_TIMEOUT             (0xFFFF)
/** minimum clock division for which both card can respond                   */
#define CSL_MMCSD_CLK_DIV_INIT                (124)
/** This bit is used to check whether the MMCSD is in busy state             */
#define CSL_MMCSD_BUSY_STATE                   (0x01)
/** This bit is used to check whether the MMCSD FIFO is empty                */
#define CSL_MMCSD_FIFO_EMPTY                   (0x20)
/** These bits are used to check whether the MMCSD is in reset state         */
#define CSL_MMCSD_CMDDAT_RESET                  (0x03)
/** The response for card detection when SD Card is detected                 */
#define CSL_SD_CARD_DETECTED                  (0x0120)
/** The response for card detection when a MMC Card is detected              */
#define CSL_MMC_CARD_DETECTED                  (0x8000)
/** This bit is used to check whether the MMCSD has responded to a command   */
#define CSL_MMCSD_RESP_DONE                      (0x04)
/** Number of bytes in block of mmc or Sd card                               */
#define CSL_MMCSD_BLOCK_LENGTH                (512u)
/** max value of block length for MMCSD                                      */
#define CSL_MMCSD_BLOCK_LEN_MAX                  (0x0FFFu)
/** Bits that are used to check whether data can be read from the card       */
#define CSL_MMCSD_READ_READY                  (0x0400)
/** Bits that are used to check whether data can be written to the card      */
#define CSL_MMCSD_WRITE_READY                 (0x0200)
/**Size of dispatch table for MMCSD Interrupts                               */
#define CSL_MMCSD_DISPATCH_TABLE_SIZE          (12U)
/** Macro to compare the NULL values                                         */
#define CSL_MMCSD_NULL                        (0)
/** Number of repsonse register avaialable                                   */
#define CSL_MMCSD_RESP_COUNT                  (8)
/** Maximum clock rate value accepted by the MMC clock control register      */
#define CSL_MMC_MAX_CLOCK_RATE                (0xFFu)
/** Argument for the MMCSD CMD8                                              */
#define CSL_MMC_CMD8_ARG                      (0x1AAu)
/** Argument for the MMCSD CMD1                                              */
#define CSL_MMC_CMD1_ARG                      (0x00ff8000u)
/** Argument for the MMCSD ACMD41 with HSC bit set                           */
#define CSL_MMC_ACMD41_ARG_HCS                (0x40ff8000u)
/** Argument for the MMCSD ACMD41 with HSC bit not set                       */
#define CSL_MMC_ACMD41_ARG_NOHCS              (0x00ff8000u)
/** Argument value to set the bus width to 4 bit                             */
#define CSL_MMC_BUSWIDTH_4BIT                 (0x00000002u)
/** Argument value to set the bus width to 1 bit                             */
#define CSL_MMC_BUSWIDTH_1BIT                 (0x00000001u)


/** Number of cycles ACMD41 will be repeated during card indentification     */
#define CSL_MMSCD_ACMD41_RETRY_COUNT          (1000u)
/** Number of cycles CMD1 will be repeated during card indentification       */
#define CSL_MMSCD_CMD1_RETRY_COUNT            (1000u)
/** Number of cycles read/write re-issue will be repeated during data txfers */
#define CSL_MMSCD_READ_WRITE_RETRY_COUNT      (1000u)

/** Write initialize command                                                 */
#define CSL_MMCSD_WRITE_INIT_CMD              (0x0210)
/** Write block command                                                      */
#define CSL_MMCSD_WRITE_BLOCK_CMD              (0x2A18)
/** Read single block command                                                */
#define CSL_MMCSD_READ_BLOCK_CMD              (0xA211)
/** Write multiple block command                                             */
#define CSL_MMCSD_WRITE_MULTIPLE_BLOCK_CMD    (0x2A19u)
/** Read multiple block command                                              */
#define CSL_MMCSD_READ_MULTIPLE_BLOCK_CMD     (0xA212)
/** All send CID  command                                                    */
#define CSL_MMCSD_ALL_SEND_CID_CMD              (0x0402)
/** init card  command                                                       */
#define CSL_MMCSD_CARD_INIT0_CMD              (0x4000)
/** select card command for SD HC                                            */
#define CSL_MMCSD_SDHC_CARD_INIT_CMD          (0x0208)
/** select card command for SD                                               */
#define CSL_MMCSD_SD_CARD_INIT_CMD              (0x0237)
/** select card command for MMC                                              */
#define CSL_MMCSD_MMCS_CARD_INIT_CMD          (0x0601)
/** Set block length command                                                 */
#define CSL_MMCSD_SET_BLKLEN_CMD              (0x0210)
/** MMCSD stop command                                                       */
#define CSL_MMCSD_STOP_CMD                      (0x038C)
/** MMCCMD1 recommended value for command 0                                  */
#define CSL_MMCSD_CMD_0                       (0x0000)
/** MMCCMD1 recommended value for command 1                                  */
#define CSL_MMCSD_CMD_1                        (0x0601)
/** MMCCMD1 recommended value for command 2                                  */
#define CSL_MMCSD_CMD_2                        (0x0402)
/** MMCCMD1 recommended value for command 3                                  */
#define CSL_MMCSD_CMD_3                        (0x0203)
/** MMCCMD1 recommended value for command 6                                  */
#define CSL_MMCSD_CMD_6                        (0x0206)
/** MMCCMD1 recommended value for command 7                                  */
#define CSL_MMCSD_CMD_7                        (0x0307)
/** MMCCMD1 recommended value for command 8                                  */
#define CSL_MMCSD_CMD_8                        (0x0208)
/** MMCCMD1 recommended value for command 9                                  */
#define CSL_MMCSD_CMD_9                        (0x0409)
/** MMCCMD1 recommended value for command 13                                 */
#define CSL_MMCSD_CMD_13                        (0x020D)
/** MMCCMD1 recommended value for Application command 23                     */
#define CSL_MMCSD_ACMD_23                        (0x0217)
/** MMCCMD1 recommended value for application command 41                     */
#define CSL_MMCSD_CMD_41                       (0x0629)
/** MMCCMD1 recommended value for application command 42                     */
#define CSL_MMCSD_ACMD_42                       (0x022A)
/** MMCCMD1 recommended value for command 55                                 */
#define CSL_MMCSD_CMD_55                        (0x0237)
/** MMCSD response for ACMD41                                                */
#define CSL_MMCSD_CMD41_RESP                    (0x80FF8000u)
/** MMCSD response for CMD1                                                  */
#define CSL_MMCSD_CMD1_RESP                    (0x80FF8000u)
/** MMCSD response for SDHC card                                             */
#define CSL_MMCSD_SDHC_RESP                    (0x40000000u)
/** Number of cycles required to detect the card type                        */
#define CSL_MMCSD_CARDCHECK_COUNT             (3u)

/**
@} */

/*****************************************************************************\
          MMCSD global typedef declarations
\*****************************************************************************/

/**\addtogroup CSL_MMCSD_ENUM
@{*/

/**
 *  \brief  Hardware Instance Id to select the MMCSD IO registers
 *
 *  This instance IDs are used to open an MMCSD CSL instance using MMC_open()
 */
typedef enum {
    CSL_MMCSD0_INST    = (0U), /**< MMCSD instance number 0                  */
    CSL_MMCSD1_INST    = (1U), /**< MMCSD instance number 1                  */
    CSL_MMCSD_INST_INV = (2U)  /**< Invalid instance number                  */
} CSL_MmcsdInstId;

/**
 *  \brief  Response type from MMCSD
 *
 *  Depending on the command value sent to the card, the card responds with
 *  different response values.The type of response is defined in this structure.
 */
typedef enum {
    CSL_MMCSD_RESPTYPE_NO   = 0,  /**< Card sends no response                */
    CSL_MMCSD_RESPTYPE_R1   = 1U, /**< Card sends response R1                */
    CSL_MMCSD_RESPTYPE_R2   = 2U, /**< Card sends response R2                */
    CSL_MMCSD_RESPTYPE_R3   = 3U, /**< Card sends response R3                */
    CSL_MMCSD_RESPTYPE_R4   = 4U, /**< Card sends response R4                */
    CSL_MMCSD_RESPTYPE_R5   = 5U, /**< Card sends response R5                */
    CSL_MMCSD_RESPTYPE_R6   = 6U  /**< Card sends response R6                */
} CSL_MmcsdRespType;

/**
 *  \brief  Response busy signal from the card.
 */
typedef enum {
    CSL_MMCSD_RESPBUSY_NO   = 0,  /**< No response busy signal is expected   */
    CSL_MMCSD_RESPBUSY_YES  = 1U  /**< Response busy signal is expected      */
} CSL_MmcsdRespBusy;

/**
 *  \brief  Hardware mode of operation of the device.
 *
 *  MMC/SD operates in two modes as defined by this enum
 */
typedef enum {
    CSL_MMCSD_LINECHR_PUSHPULL  = 0,  /**< MMCSD push-pull configuration     */
    CSL_MMCSD_LINECHR_OPENDRAIN = 1U  /**< MMCSD open drain configuration    */
} CSL_MmcsdLineChr;

/**
 *  \brief  Different command types in MMCSD standard
 *
 *  This structure identifies the type of command that is sent to the
 *  MMC/SD Controller.
 */
typedef enum {
    CSL_MMCSD_CMDTYPE_BC    = 0,  /**< Broadcast commands - no repsonse      */
    CSL_MMCSD_CMDTYPE_BCR   = 1U, /**< Broadcast commands with repsonse      */
    CSL_MMCSD_CMDTYPE_AC    = 2U, /**< Addressed commands - no data transfer */
    CSL_MMCSD_CMDTYPE_ADTC  = 3U  /**< Addressed commands with data transfer */
} CSL_MmcsdCmdType;

/**
 *  \brief  Command Index for MMC and SD Cards.
 *
 * NOTE: Commands specific to SD card are having SD_CARD suffixes,
 *       commands specific to MMC card are having MMC_CARD suffixes
 *       and all others are applicable for both MMC and SD card.
 */
typedef enum {
    CSL_MMCSD_GO_IDLE_STATE                 = 0,
    /**< MMCSD CMD0 - Command to reset all the cards to idle state           */
    CSL_MMCSD_SEND_OP_COND_MMC_CARD         = 1U,
    /**< MMC CMD1 - Command to send operation condition register to MMC card */
    CSL_MMCSD_ALL_SEND_CID                  = 2U,
    /**< MMCSD CMD2 - Command to request the card to send its CID number     */
    CSL_MMCSD_RELATIVE_ADDR                 = 3U,
    /**< MMCSD CMD3
         MMC - Command to set the realative card address
         SD - Command to request the card to send new realative card address */
    CSL_MMCSD_SET_DSR                       = 4U,
    /**< MMCSD CMD4 - Command to configure Driver Stage Register(DSR)        */
    CSL_MMCSD_CMD_IDX_RSV_5                 = 5U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_SWITCH_MMC_CARD           = 6U,
    /**< MMC CMD6 - Command to switch the mode of operation of the MMC card  */
    CSL_MMCSD_SELECT_DESELECT_CARD          = 7U,
    /**< MMCSD CMD7 - Command to select/deselect the MMC/SD card             */
    CSL_MMCSD_CMD_SEND_IF_COND_EXT_CSD      = 8U,
    /**< MMCSD CMD8
         MMC - Command to request the extended CSR register from the card
         SD - Command to send the card interface condition                   */
    CSL_MMCSD_SEND_CSD                      = 9U,
    /**< MMCSD CMD9 - Command to request Card Specific Data register         */
    CSL_MMCSD_SEND_CID                      = 10U,
    /**< MMCSD CMD10 - Command to read Card Identification Data register     */
    CSL_MMCSD_READ_DAT_UNTIL_STOP_MMC_CARD  = 11U,
    /**< MMC CMD11 - Command to read data stream until stop commad is sent   */
    CSL_MMCSD_STOP_TRANSMISSION             = 12U,
    /**< MMCSD CMD12 - Command to force the card to stop the data transfer   */
    CSL_MMCSD_SEND_STATUS                   = 13U,
    /**< MMCSD CMD13 - Command to request the card to send status register   */
    CSL_MMCSD_CMD_IDX_RSV_14                = 14U,
    /**< Reserved command                                                    */
    CSL_MMCSD_GO_INACTIVE_STATE             = 15U,
    /**< MMCSD CMD15 - Command to set the card to inactive state             */
    CSL_MMCSD_SET_BLOCKLEN                  = 16U,
    /**< MMCSD CMD16 - Command to set block length                           */
    CSL_MMCSD_READ_SINGLE_BLOCK             = 17U,
    /**< MMCSD CMD17 - Command to read single block of data from the card    */
    CSL_MMCSD_READ_MULTIPLE_BLOCK           = 18U,
    /**< MMCSD CMD18 - Command to read multiple blocks of data from the card */
    CSL_MMCSD_CMD_IDX_RSV_19                = 19U,
    /**< Reserved command                                                    */
    CSL_MMCSD_WRITE_DAT_UNTIL_STOP_MMC_CARD = 20U,
    /**< MMC CMD20 - Command to write data stream until stop commad is sent  */
    CSL_MMCSD_CMD_IDX_RSV_21                = 21U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_22                = 22U,
    /**< Reserved command                                                    */
    CSL_MMCSD_SET_BLOCK_COUNT_MMC_CARD      = 23U,
    /**< MMC CMD23 - Command to set the number of blocks which are going to
         be transferred in the immediately succeeding multiple block read or
         write command.                                                      */
    CSL_MMCSD_WRITE_BLOCK                   = 24U,
    /**< MMCSD CMD24 - Command to write single block of data to the card     */
    CSL_MMCSD_WRITE_MULTIPLE_BLOCK          = 25U,
    /**< MMCSD CMD25 - Command to write multiple blocks of data to the card  */
    CSL_MMCSD_PROGRAM_CID_MMC_CARD          = 26U,
    /**< MMC CMD26 - Command to program CID register                         */
    CSL_MMCSD_PROGRAM_CSD                   = 27U,
    /**< MMCSD CMD27 - Command to program CSD register                       */
    CSL_MMCSD_SET_WRITE_PROT                = 28U,
    /**< MMCSD CMD28 - Command to set the write protection bit of the card.
         This comamnd is applicable only to the cards which supports write
         protection feature                                                  */
    CSL_MMCSD_CLR_WRITE_PROT                = 29U,
    /**< MMCSD CMD29 - Command to clear the write protection bit of the card.
         This comamnd is applicable only to the cards which supports write
         protection feature                                                  */
    CSL_MMCSD_SEND_WRITE_PROT               = 30U,
    /**< MMCSD CMD30 - Command to read the write protection bit of the card.
         This comamnd is applicable only to the cards which supports write
         protection feature                                                  */
    CSL_MMCSD_CMD_IDX_RSV_31                = 31U,
    /**< Reserved command                                                    */
    CSL_MMCSD_ERASE_WR_BLK_START_SD_CARD    = 32U,
    /**< SD CMD32 - Command to Set the address of the first write block
         to be erased.                                                       */
    CSL_MMCSD_ERASE_WR_BLK_END_SD_CARD      = 33U,
    /**< SD CMD33 - Command to Set the address of the last write block
         to be erased.                                                       */
    CSL_MMCSD_CMD_IDX_RSV_34                = 34U,
    /**< Reserved command                                                    */
    CSL_MMCSD_ERASE_GROUP_START_MMC_CARD    = 35U,
    /**< MMC CMD35 - Command to set the address of the first erase group
         within a range to be selected for erase                             */
    CSL_MMCSD_ERASE_GROUP_END_MMC_CARD      = 36U,
    /**< MMC CMD36 - Command to set the address of the last erase group
         within a range to be selected for erase                             */
    CSL_MMCSD_CMD_IDX_RSV_37                = 37U,
    /**< Reserved command                                                    */
    CSL_MMCSD_ERASE                         = 38U,
    /**< MMCSD CMD38 - Command to erases all previously selected write blocks*/
    CSL_MMCSD_FAST_IO_MMC_CARD              = 39U,
    /**< MMC CMD39 - Command to write and read 8 bit (register) data fields  */
    CSL_MMCSD_GO_IRQ_STATE_MMC_CARD         = 40U,
    /**< MMC CMD40 - Command to set the system into interrupt mode           */
    CSL_MMCSD_CMD_IDX_RSV_41                = 41U,
    /**< Reserved command                                                    */
    CSL_MMCSD_LOCK_UNLOCK                   = 42U,
    /**< MMCSD CMD42 - Command to set/reset password or lock/unlock the card */
    CSL_MMCSD_CMD_IDX_RSV_43                = 43U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_44                = 44U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_45                = 45U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_46                = 46U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_47                = 47U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_48                = 48U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_49                = 49U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_50                = 50U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_51                = 51U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_52                = 52U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_53                = 53U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_54                = 54U,
    /**< Reserved command                                                    */
    CSL_MMCSD_APP_CMD                       = 55U,
    /**< MMCSD CMD55 - Command to Indicates to the card that the next command
         is an application specific command rather than a standard command   */
    CSL_MMCSD_GEN_CMD                       = 56U,
    /**< MMCSD CMD56 - Command either to transfer a data block to the card or
         to get a data block from the card for general purpose/application
         specific commands                                                   */
    CSL_MMCSD_CMD_IDX_RSV_57                = 57U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_58                = 58U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_59                = 59U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_60                = 60U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_61                = 61U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_62                = 62U,
    /**< Reserved command                                                    */
    CSL_MMCSD_CMD_IDX_RSV_63                = 63U
    /**< Reserved command                                                    */
} CSL_MmcsdCmdIndex;

/**
 *  \brief  Direction of data transfer
 */
typedef enum {
    CSL_MMCSD_DIR_WRITE     = 0,  /**< MMCSD data phase is write operation   */
    CSL_MMCSD_DIR_READ      = 1U, /**< MMCSD data phase is read operation    */
    CSL_MMCSD_DIR_DONTCARE  = 2U  /**< MMCSD data phase is involved          */
} CSL_MmcsdDir;

/**
 *  \brief  Type of card that is inserted
 */
typedef enum
{
    CSL_CARD_NONE = 0,   /**< No card detected                               */
    CSL_SD_CARD   = 1U,  /**< SD card detected                               */
    CSL_MMC_CARD  = 2U   /**< MMC card detected                              */
} CSL_CardType;

/**
 *  \brief  Event responses in MMCSD
 *
 */
typedef enum {
    CSL_MMCSD_EVENT_TRNDNE              = (1U << 12U),
    /**< Last byte of a transfer has been completed - CRC check not complete */
    CSL_MMCSD_EVENT_DATED               = (1U << 11U),
    /**< DAT3 edge detected.                                                 */
    CSL_MMCSD_EVENT_READ                = (1U << 10U),
    /**< Data available with controller for reading                          */
    CSL_MMCSD_EVENT_WRITE               = (1U << 9U),
    /**< Controller is ready to accept new data for data transmit            */
    CSL_MMCSD_EVENT_ERROR_CMDCRC        = (1U << 7U),
    /**< Error detected in the CRC during command response phase             */
    CSL_MMCSD_EVENT_ERROR_DATACRC       = ((1U << 6U)|(1U << 5U)),
    /**< Error detected in the CRC during data transfer                      */
    CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT    = ((1U) << (4U)),
    /**< Timeout detected during command response phase                      */
    CSL_MMCSD_EVENT_ERROR_DATATIMEOUT   = ((1U) << (3U)),
    /**< Timeout detected during data transfer                               */
    CSL_MMCSD_EVENT_EOFCMD              = (1U << 2U),
    /**< For commands with response, an end of command + Response;
         For commands without response, an end of Command                    */
    CSL_MMCSD_EVENT_CARD_EXITBUSY       = ((1U) << (1U)),
    /**< Card has exited busy state                                          */
    CSL_MMCSD_EVENT_BLOCK_XFERRED       = 1U
    /**< Block transfer done - CRC check and programming completed           */
} CSL_MmcsdEvent;

/**
 * \brief Enum variable for the selection of mmcsd interrupts.
 *
 */
typedef enum
{
    CSL_MMCSD_EDATDNE_INTERRUPT = 1U,
    /**< Data done (DATDNE) interrupt enable.                                */
    CSL_MMCSD_EBSYDNE_INTERRUPT = (1U << 1U),
    /**< Busy done (BSYDNE) interrupt enable.                                */
    CSL_MMCSD_ERSPDNE_INTERRUPT = (1U << 2U),
    /**< Command/response done (RSPDNE) interrupt enable.                    */
    CSL_MMCSD_ETOUTRD_INTERRUPT = (1U << 3U),
    /**< Read-data time-out event (TOUTRD) interrupt enable.                 */
    CSL_MMCSD_ETOUTRS_INTERRUPT = (1U << 4U),
    /**< Response time-out event (TOUTRS) interrupt enable.                  */
    CSL_MMCSD_ECRCWR_INTERRUPT  = (1U << 5U),
    /**< Write-data CRC error (CRCWR) interrupt enable.                      */
    CSL_MMCSD_ECRCRD_INTERRUPT  = (1U << 6U),
    /**< Read-data CRC error (CRCRD) interrupt enable.                       */
    CSL_MMCSD_ECRCRS_INTERRUPT  = (1U << 7U),
    /**< Response CRC error (CRCRS) interrupt enable.                        */
    CSL_MMCSD_RSV_BIT           = (1U << 8U),
    /**< This bit is reserved.                                               */
    CSL_MMCSD_EDXRDY_INTERRUPT  = (1U << 9U),
    /**< Data transmit register (MMCDXR) ready interrupt enable.             */
    CSL_MMCSD_EDRRDY_INTERRUPT  = (1U << 10U),
    /**< Data receive register ready (DRRDY) interrupt enable.               */
    CSL_MMCSD_EDATED_INTERRUPT  = (1U << 11U),
    /**< DAT3 edge detect (DATED) interrupt enable.                          */
    CSL_MMCSD_ETRNDNE_INTERRUPT = (1U << 12U),
    /**< Transfer done (TRNDNE) interrupt enable.                            */
    CSL_MMCSD_INTERRUPT_NONE    = (1U << 13U)
    /**< No interrupt enable                                                 */
} CSL_MMCSDEventType;

/**
 * \brief
 *  Enum to select the operating mode of the CSL MMCSD module
 */
typedef enum
{
    CSL_MMCSD_OPMODE_POLLED    = 0,
    /**< MMCSD operates in poll mode                                         */
    CSL_MMCSD_OPMODE_INTERRUPT = 1U,
    /**< MMCSD operates in interrupt mode                                    */
    CSL_MMCSD_OPMODE_DMA       = 2U,
    /**< MMCSD operates in DMA mode                                          */
    CSL_MMCSD_OPMODE_NONE      = 3U
    /**< Invalid Enum value for boundary checking                            */
} CSL_MMCSDOpMode;

/**
 *  \brief  MMC/SD endianness
 *
 *  This enum holds the values to set endianness of the MMC/SD controller
 */
typedef enum {
    CSL_MMCSD_ENDIAN_LITTLE = 0,    /**< Selects MMCSD little endian mode    */
    CSL_MMCSD_ENDIAN_BIG    = 1U    /**< Selects MMCSD big endian mode       */
} CSL_MmcsdEndianMode;

/**
 *  \brief  Error type as received by the MMC on response for a command
 */
#define CSL_MMCSD_EVENT_ERROR   (CSL_MMCSD_EVENT_ERROR_CMDCRC |     \
                                 CSL_MMCSD_EVENT_ERROR_DATACRC |    \
                                 CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT | \
                                 CSL_MMCSD_EVENT_ERROR_DATATIMEOUT)

/**
 *  \brief  Error type for command response timeout and CRC error
 */
#define CSL_MMCSD_CMD_TOUT_CRC_ERROR   (CSL_MMCSD_EVENT_ERROR_CMDCRC |     \
                                        CSL_MMCSD_EVENT_ERROR_CMDTIMEOUT)

/**
 *  \brief  Error type for data timeout and CRC error
 */
#define CSL_MMCSD_DATA_TOUT_CRC_ERROR   (CSL_MMCSD_EVENT_ERROR_DATACRC |    \
                                         CSL_MMCSD_EVENT_ERROR_DATATIMEOUT)
/**
@} */

/**
\addtogroup CSL_MMCSD_DATASTRUCT
@{*/

/**
 *  \brief  Command structure for the card.
 *
 *    This structure has parameters that is used to send a command to the
 *  MMC/SD device.
 */
typedef struct {
    Uint16               idx;
    /**< Index of the command                                                */
    CSL_MmcsdRespType    respType;
    /**< Response expected for the command                                   */
    CSL_MmcsdRespBusy    busy;
    /**< Response property with respect to busy signal                       */
    CSL_MmcsdCmdType     type;
    /**< Protocol type of the command                                        */
    CSL_MmcsdDir         dir;
    /**< Direction for the data incase of data transferred involved          */
    CSL_MmcsdLineChr     lineChr;
    /**< Line characteristics of the command                                 */
} CSL_MmcsdCmd;

/**
 *  \brief  Response array received from the controller
 *
 *    This object holds the information of the response received for a command
 *    sent to the card.
 */
typedef struct {
    Uint16    response[CSL_MMCSD_RESP_COUNT];
    /**< The response will interpreted as:
         response[0] - R2[15:0]
         response[1] - R2[31:16]
         response[2] - R2[47:32]
         response[3] - R2[63:48]
         response[4] - R2[79:64]
         response[5] - R2[95:80]
         response[6] - R2[111:96] or R1/R1B/R3/R4/R5/R6[23:8]
         response[7] - R2[127:112] or R1/R1B/R3/R4/R5/R6[39:24]              */
} CSL_MmcsdResponse;

/**
 * \brief Structure to store the some default configuration for MMCSD
 *        controller
 */
typedef struct {
    Uint16    dmaEnable;
    /**< Enable/disable DMA for data read/write                              */
    Uint16    dat3EdgeDetection;
    /**< Set level of edge detection for DAT3 pin                            */
    Uint16    enableClkPin;
    /**< Memory clk reflected on CLK Pin                                     */
    Uint16    cdiv;
    /**< MMC function clk to memory clk divide down                          */
    Uint16    rspTimeout;
    /**< Number of memory clks to wait before response timeout               */
    Uint16    dataTimeout;
    /**< Number of memory clks to wait before data timeout                   */
    Uint16    blockLen;
    /**< Block Length - must be same as CSD                                  */
} CSL_MMCSetupNative;

/**
 * \brief MMC config structure for setting MMC/SD Control register.
 *
 */
typedef struct {
    Uint16    mmcctl;  /**< MMCSD control register value                     */
    Uint16    mmcclk;  /**< MMCSD clock control register value               */
    Uint16    mmcim;   /**< required interrupt field for MMCSD               */
    Uint16    mmctor;  /**< mmc response time out register                   */
    Uint16    mmctod;  /**< mmc data read time out register                  */
    Uint16    mmcblen; /**< no of bytes in a block of mmc or sd card         */
    Uint16    mmcnblk; /**< total no of blocks for read or write             */
} CSL_MMCConfig;

/**
 * \brief Structure to store the CID Information for MMC and SD card.
 *
 */
typedef struct
{
    Uint16    slice127_112;  /**< MMCSD Response Register 7                  */
    Uint16    slice111_96;   /**< MMCSD Response Register 6                  */
    Uint16    slice95_80;    /**< MMCSD Response Register 5                  */
    Uint16    slice79_64;    /**< MMCSD Response Register 4                  */
    Uint16    slice63_48;    /**< MMCSD Response Register 3                  */
    Uint16    slice47_32;    /**< MMCSD Response Register 2                  */
    Uint16    slice31_16;    /**< MMCSD Response Register 1                  */
    Uint16    slice15_0;     /**< MMCSD Response Register 0                  */
} CSL_MMCSDCidStruct;

/**
 * \brief Card identification object.
 *
 */
typedef struct {
    Uint16    mfgId;          /**< 8 bit Manufacturer ID                     */
    Uint16    oemAppId;       /**< 16 bit OEM and application ID             */
    Uint8     productName[6]; /**< 40 or 48 bit Product name                 */
    Uint16    productRev;     /**< 8 bit Product Revision Number             */
    Uint32    serialNumber;   /**< 32 bit Serial Number                      */
    Uint16    month;          /**< 4 bit Manufacturing Date (Month)          */
    Uint16    year;           /**< 4 bit Manufacturing Date (Year) in case of
    MMC (Year 0 = 1997). SD has an 8 bit year field.(Year 0 = 2000)          */
    Uint16    checksum;       /**< 7 bit crc                                 */
} CSL_MMCCardIdObj;

/**
 * \brief Structure to store the CSD Information for MMC and SD card.
 *
 */
typedef struct
{
    Uint16    slice127_112; /**< MMCSD Response Register 7                   */
    Uint16    slice111_96;  /**< MMCSD Response Register 6                   */
    Uint16    slice95_80;   /**< MMCSD Response Register 5                   */
    Uint16    slice79_64;   /**< MMCSD Response Register 4                   */
    Uint16    slice63_48;   /**< MMCSD Response Register 3                   */
    Uint16    slice47_32;   /**< MMCSD Response Register 2                   */
    Uint16    slice31_16;   /**< MMCSD Response Register 1                   */
    Uint16    slice15_0;    /**< MMCSD Response Register 0                   */
} CSL_MMCSDCsdStruct;

/**
 * \brief CSD Information for MMC and SD card.
 *
 */
typedef struct {
    Uint16 csdStruct;          /**< CSD version; 0 - 1.0, 1 - 2.0            */
    Uint16    mmcProt;         /**< MMC protocol version                     */
    Uint16    taac;            /**< TAAC                                     */
    Uint16    nsac;            /**< NSAC                                     */
    Uint16    tranSpeed;       /**< Max data transmission speed              */
    Uint16    ccc;             /**< Card command classes                     */
    Uint16    readBlLen;       /**< Maximum Read Block Length                */
    Uint16    readBlPartial;   /**< Indicates if partial read blocks allowed */
    Uint16    writeBlkMisalign;/**< Flag indicates write block misalignment  */
    Uint16    readBlkMisalign; /**< Flag indicates read block misalignment   */
    Uint16    dsrImp;          /**< Flag indicates whether card has DSR reg  */
    Uint16    cSize;           /**< Device size                              */
    Uint16    vddRCurrMin;     /**< Max. Read Current @ Vdd Min              */
    Uint16    vddRCurrMax;     /**< Max. Read Current @ Vdd Max              */
    Uint16    vddWCurrMin;     /**< Max. Write Current @ Vdd Min             */
    Uint16    vddWCurrMax;     /**< Max. Write Current @ Vdd Max             */
    Uint16    cSizeMult;       /**< Device size multiplier                   */
    Uint16    eraseBlkEnable;  /**< Erase single block enable                */
    Uint16    eraseGrpSize;    /**< Erase sector group size                  */
    Uint16    eraseGrpMult;    /**< Erase group multiplier                   */
    Uint16    wpGrpSize;       /**< Write protect group size                 */
    Uint16    wpGrpEnable;     /**< Write protect enable flag                */
    Uint16    defaultEcc;      /**< Manufacturer Default ECC                 */
    Uint16    r2wFactor;       /**< Stream write factor                      */
    Uint16    writeBlLen;      /**< Maximum write block length               */
    Uint16    writeBlPartial;  /**< Indicates if partial write blocks allowed*/
    Uint16    contProtApp;     /**< Content protection application           */
    Uint16    fileFmtGrp;      /**< File format group                        */
    Uint16    copyFlag;        /**< Copy flag                                */
    Uint16    permWriteProtect;/**< Dis/en-able permanent write protection   */
    Uint16    tmpWriteProtect; /**< Dis/en-able temporary write protection   */
    Uint16    fileFmt;         /**< File format                              */
    Uint16    ecc;             /**< ECC code                                 */
    Uint16    crc;             /**< Cyclic redundancy check                  */
} CSL_MMCCardCsdObj;

/**
 * \brief structure for extra feature of MMC and SD Object.
 *
 */
typedef struct {
    Uint16    securitySysId;   /**< Security System ID                       */
    Uint16    securitySysVers; /**< Security System Version                  */
    Uint16    maxLicenses;     /**< Maximum number of storable licenses      */
    Uint32    xStatus;         /**< Extended status bits                     */
} CSL_MMCCardXCsdObj;

/**
 * \brief structure for card specific information.
 *
 */
typedef struct {
    Uint16                rca;
    /**< MMC card: User assigned relative card address (RCA).
         SD card: Relative card address (RCA) published by the card          */
    Uint16                ST0;
    /**< Last read ST0 register value                                        */
    Uint16                ST1;
    /**< Last read ST1 register value                                        */
    Uint16                cardIndex;
    /**< MMC module assigned card index for card                             */
    Uint32                maxXfrRate;
    /**< Maximum transfer rate                                               */
    Uint32                readAccessTime;
    /**< TAAC - exp * mantissa                                               */
    Uint32 cardCapacity;
    /**< Total memory available on card
       Note: For CSD Version 1.0 (CSL_MMCCardCsdObj.csdStruct = 0) capacity
             will be in bytes
             For CSD Version 2.0 (CSL_MMCCardCsdObj.csdStruct = 1) capacity
             will be in KBytes                                               */
    Uint32                blockLength;
    /**< Block length of card                                                */
    Uint32                totalSectors;
    /**< Total number of sectors on card                                     */
    Uint32                lastAddrRead;
    /**< Last address read from memory card                                  */
    Uint32                lastAddrWritten;
    /**< Last Address written to memory card                                 */
    CSL_CardType          cardType;
    /**< Type of the card - MMC or SD                                        */
    CSL_MMCCardIdObj      *cid;
    /**< Manufacturers Card ID                                               */
    CSL_MMCCardCsdObj     *csd;
    /**< Card specific data                                                  */
    CSL_MMCCardXCsdObj    *xcsd;
    /**< Extended CSD                                                        */
    Bool                  sdHcDetected;
    /**< Flag to indicate detection of SD HC card                            */
    Bool                  cardAtaFsOpen;
    /**< Flag to indicate whether ATAFS open on card                         */
    Uint16                cardMscStatus;
    /**< MSC status for card                                                 */
} CSL_MMCCardObj;

/**
 * \brief  call back function for MMCSD.
 *
 */
typedef void (* CSL_MMCCallBackPtr)(void);

/**
 * \brief  Data transfer call back function pointer.
 *
 * DATA TRANSFER CALLBACK FUNCTION IMPLEMENTED BY THE APPLICATION PROGRAMS
 * SHOULD WAIT FOR THE INTERRUPT AND DISABLE ALL THE MMC INTERRUPTS AFTER
 * GETTING INTERRUPT FROM MMCSD CONTROLLER.
 */
typedef void (*CSL_MMCDataTxferCallBackPtr)(void *mmcsdHandle);/* KR022510 */

/**
 * \brief structure array of isr function for MMCSD.
 *
 */
typedef struct {
   CSL_MMCCallBackPtr isr[CSL_MMCSD_DISPATCH_TABLE_SIZE];
   /**< Isr function for MMCSD                                               */
} CSL_MMCCallBackObj;

/**
 * \brief DMA configuration structure for MMCSD module.
 *
 * Contains configuration parameters for DMA hardware registers.
 */
typedef struct
{
    Bool                     pingPongEnable;
    /**< Ping-pong mode configuration for DMA. This mode is applicable only
         to C5515 DSP. This parameter is available for C5505 CSL also.
         No need to set this parameter for C5505 DSP. CSL APIs ignore this
         parameter while configuring the DMA for C5505 DSP.
         pingPongEnable = 0; Disables DMA Ping-Pong mode
         pingPongEnable = 1; Enables DMA Ping-Pong mode                      */
    CSL_DMAAutoReloadMode    autoMode;    /**< Auto reloadmode bit           */
    CSL_DMATxBurstLen        burstLen;    /**< Length of the transmit burst  */
    CSL_DMAInterruptState    dmaInt;      /**< State of DMA interrupt        */
    CSL_DMAChanDir           chanDir;     /**< Channel direction - Rd/Wr     */
} CSL_MmcsdDmaConfig;

/**
 * \brief structure for card specific information.
 *
 */
typedef struct {
    CSL_MmcsdRegsOvly              mmcRegs;
    /**< Pointer to MMC regs structure                                       */
    CSL_MMCCardObj                 *cardObj;
    /**< Pointer to card Objects                                             */
    Uint16                         numCardsActive;
    /**< Number of cards active/ready                                        */
    CSL_MMCCallBackObj             *callBackTbl;
    /**< Pointer to call back table obj                                      */
    CSL_MMCSDOpMode                opMode;
    /**< Operating mode of MMCSD                                             */
    CSL_DMA_Handle                 hDmaWrite;
    /**< Handle to DMA write channel                                         */
    CSL_DMA_Handle                 hDmaRead;
    /**< Handle to DMA read channel                                          */
    CSL_DMA_Config                 dmaWriteCfg;
    /**< DMA config structure for write                                      */
    CSL_DMA_Config                 dmaReadCfg;
    /**< DMA config structure for read                                       */
    CSL_MMCDataTxferCallBackPtr    dataTransferCallback;
    /**< Call back function for data tranfers                                */
    Bool                           isCallbackSet;
    /**< Flag to enable callback request                                     */
    CSL_MMCSDCidStruct             cidSliceInfo;
    /**< CID slice information strcuture                                     */
    CSL_MMCSDCsdStruct             csdSliceInfo;
    /**< CSD slice information strcuture                                     */
    CSL_MmcsdEndianMode            readEndianMode;
    /**< Endian mode for read operation                                      */
    CSL_MmcsdEndianMode            writeEndianMode;
    /**< Endian mode for write operation                                     */
    Uint16                         blockLen;
    /**< Block length used for memory card                                   */
} CSL_MMCControllerObj;

/**
 * \brief MMCSD Handle
 *
 */
typedef CSL_MMCControllerObj     *CSL_MmcsdHandle;

/**
@} */

/******************************************************************************
 * CSL MMCSD function declarations
 *****************************************************************************/
/** @addtogroup CSL_MMCSD_FUNCTION
 @{ */

/** ===========================================================================
 *   @n@b MMC_init
 *
 *   @b Description
 *   @n This is the initialization function for the MMCSD CSL. The function
 *      must be called before calling any other API from MMCSD CSL. This
 *      function is for enabling the clock to the MMCSD Controller and routing
 *      MMCSD pins to the external serial port.
 *      This function should be called only once during system initialization
 *      time. This function gates the clock for the two MMCSD modules.
 *      Application programs may disable the clocks for MMCSD module which is
 *      not in use.
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li            CSL_SOK            - MMCSD initialization successfully
 *
 *   <b> Pre Condition </b>
 *   @n  This should be call first before calling any other MMCSD Function.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_status       status;

            status = MMC_init();
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_init(void);

/** ===========================================================================
 *   @n@b MMC_open
 *
 *   @b Description
 *   @n This function opens the CSL MMCSD module and returns handle to the
 *      MMCSD object structure. Parameter 'opMode' is provided to select the
 *      operating mode of the MMCSD module. Opmode information is stored in
 *      the MMCSD handle and is used in data transfer APIs.
 *
 *      NOTE: THIS API SETS THE ENDIAN MODE OF MMCSD READ AND WRITE OPERATIONS
 *            TO LITTLE ENDIAN. USE MMC_setEndianMode() TO CHANGE ENDIAN MODE.
 *
 *   @b Arguments
 *   @verbatim
            pMmcsdContObj    MMCSD Controller object structure
            instId           Instance number for MMCSD
            opMode           Operating mode of the MMCSD module
            status           Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_MmmcsdHandle
 *   @n                     Valid MMCSD handle will be returned if
 *                          status value is equal to CSL_SOK.
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_init() API should be called before MMC_open().
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is returned in the status variable. If status
 *              returned is
 *   @li            CSL_SOK            - Valid MMCSD handle is returned
 *   @li            CSL_ESYS_INVPARAMS - resource is already in use
 *   @n   2.    CSL_MmmcsdObj object structure is populated
 *
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. CSL_MmmcsdObj object structure
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;
            status =  MMC_init();
            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_MmcsdHandle MMC_open(CSL_MMCControllerObj    *pMmcsdContObj,
                         CSL_MmcsdInstId         instId,
                         CSL_MMCSDOpMode         opMode,
                         CSL_Status              *status);

/** ===========================================================================
 *   @n@b MMC_close
 *
 *   @b Description
 *   @n This function closes the specified handle to MMCSD.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd    Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully before this.
 *
 *   <b> Post Condition </b>
 *   @n  Closes the MMC handle
 *
 *   @b Modifies
 *   @n MMCSD handle will be assigned to NULL.
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_close(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_close(CSL_MmcsdHandle    hMmcsd);

/** ===========================================================================
 *   @n@b MMC_setCardType
 *
 *   @b Description
 *   @n This API just updates the software structure and does not update any
 *      hardware registers
 *
 *   @b Arguments
 *   @verbatim
            pCardObj    Pointer to the card object.
            cardType    type of card MMC or SD.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - successful
 *   @li                    CSL_ESYS_INVPARAMS - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            CSL_MMCCardObj        cardObj;
            CSL_CardType          cardType;

            cardType = CSL_SD_CARD;

            status = MMC_setCardType(&cardObj, cardType);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setCardType(CSL_MMCCardObj    *pCardObj,
                           CSL_CardType      cardType);

/** ===========================================================================
 *   @n@b MMC_setCardPtr
 *
 *   @b Description
 *   @n This function will set the card object information address into the
 *      MMCSD handle. Implementation is limited only to the software layer.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            CSL_MMCCardObj    Pointer to the MMCSD Card Object.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n Card object structure
 *
 *   @b Example
 *   @verbatim
            CSL_MMCCardObj        mmcCardObj;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_setCardPtr(hMmcsd, &mmcCardObj);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setCardPtr(CSL_MmcsdHandle    hMmcsd,
                          CSL_MMCCardObj     *pMmcCardObj);

/** ===========================================================================
 *   @n@b MMC_setCallBack
 *
 *   @b Description
 *   @n This function sets the address of call back routines which will be
 *      called from MMCSD ISR.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd       Handle to the MMCSD.
            pFunction    Pointer to the isr function.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None.
 *
 *   @b Example
 *   @verbatim
            #define MMCSD_ISR_FUNCTION_MAX (4u)
            void mmcsd0_tx_isr(void);
            void mmcsd0_rcv_isr(void);
            void mmcsd1_tx_isr(void);
            void mmcsd1_rcv_isr(void);

            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            CSL_MMCCallBackObj    callBckFun;

            instId = CSL_MMCSD0_INST;
            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            callBckFun.isr[CSL_MMCSD_ISR_TXMT] = mmcsd_tx_isr;
            callBckFun.isr[CSL_MMCSD_ISR_RCV]  = mmcsd_rcv_isr;

            status = MMC_setCallBack(hMmcsd, &callBckFun);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setCallBack(CSL_MmcsdHandle       hMmcsd,
                           CSL_MMCCallBackObj    *pFunction);

/** ===========================================================================
 *   @n@b MMC_getNumberOfCards
 *
 *   @b Description
 *   @n Get the number of cards information from the MMC handler.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd           Handle to the MMCSD
            pActiveNoCard    pointer to store number of active card info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            Uint16                activeNoCard;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;
            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_getNumberOfCards(hMmcsd, &activeNoCard);

     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_getNumberOfCards(CSL_MmcsdHandle    hMmcsd,
                                Uint16             *pActiveNoCard);

/** ===========================================================================
 *   @n@b MMC_clearResponse
 *
 *   @b Description
 *   @n This function clears all eight response registers of MMCSD.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd    Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully before this.
 *
 *   <b> Post Condition </b>
 *   @n  Clears response
 *
 *   @b Modifies
 *   @n MMCSD Response Registers.
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_clearResponse(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_clearResponse(CSL_MmcsdHandle    hMmcsd);

/** ===========================================================================
 *   @n@b MMC_intEnable
 *
 *   @b Description
 *   @n This function will enable or disable interrupts for MMCSD depending on
 *      the passed mask value for interrupt. MMC interrupt mask register will
 *      be configured by this API. This function is useful to enable the
 *      multiple MMC interrupts in one function call.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd       Handle to the MMCSD
            maskValue    mask value to specify the interrupt state.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully before this.
 *       Before enabling interrupt of any type make sure the isr function
 *       is plugged in ISR.
 *
 *   <b> Post Condition </b>
 *   @n  Enables MMC interrupts
 *
 *   @b Modifies
 *   @n MMC interrupt mask register
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            Uint16                maskValue;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);
            NOTE: Use the bit mask values of MMCIM register defined
                  cslr_mmcsd.h file for making the mask value.

            maskValue = CSL_MMCSD_MMCIM_ETRNDNE_MASK |
                        CSL_MMCSD_MMCIM_EDATED_MASK;

            status = MMC_intEnable(hMmcsd, maskValue);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_intEnable(CSL_MmcsdHandle    hMmcsd,
                         Uint16             maskValue);

/** ===========================================================================
 *   @n@b MMC_eventEnable
 *
 *   @b Description
 *   @n This function enables the MMC events depending on the 'mmcsdEvent'
 *      passed as a parameter. This function is useful to enable single
 *      interrupt of MMC interrupt mask register in one function call.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        Handle to the MMCSD
            mmcsdEvent    Event type.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully.
 *       Before enabling interrupt of any type make sure the isr function
 *       is plugged in ISR.
 *
 *   <b> Post Condition </b>
 *   @n  Interrupt will be enabled for particular event.
 *
 *   @b Modifies
 *   @n MMC interrupt mask register
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_MMCSDEventType    mmcsdEvent;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);
            eventType = CSL_MMCSD_EDATDNE_INTERRUPT;

            status = MMC_eventEnable(hMmcsd, mmcsdEvent);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_eventEnable(CSL_MmcsdHandle       hMmcsd,
                           CSL_MMCSDEventType    mmcsdEvent);

/** ===========================================================================
 *   @n@b MMC_eventDisable
 *
 *   @b Description
 *   @n This function disables the MMC events depending on the 'mmcsdEvent'
 *      passed as a parameter. This function is useful to disable single
 *      interrupt of MMC interrupt mask register in one function call.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        Handle to the MMCSD
            mmcsdEvent    Event type.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n
 *
 *   @b Modifies
 *   @n Interrupt will be disabled for particular event.
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_MMCSDEventType    mmcsdEvent;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);
            mmcsdEvent = CSL_MMCSD_EDATDNE_INTERRUPT;

            status = MMC_eventDisable(hMmcsd, mmcsdEvent);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_eventDisable(CSL_MmcsdHandle       hMmcsd,
                            CSL_MMCSDEventType    mmcsdEvent);

/** ===========================================================================
 *   @n@b MMC_drrdy
 *
 *   @b Description
 *   @n This function returns data receive ready status. Value returned by this
 *      API is useful to decide whether data is available in MMC/SD controller
 *      FIFO for reading or not.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd     Handle to the MMCSD
            pStatus    Status of API success or failed.
     @endverbatim
 *
 *   <b> Return Value </b>  Bool
 *   @li                    TRUE  - Data is available for reading
 *   @li                    FALSE - Data is not available for reading
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Bool                  drrdyStatus;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            drrdyStatus = MMC_drrdy(hMmcsd, &status);
     @endverbatim
 *  ===========================================================================
 */
Bool MMC_drrdy(CSL_MmcsdHandle    hMmcsd,
               CSL_Status         *pStatus);

/** ===========================================================================
 *   @n@b MMC_dxrdy
 *
 *   @b Description
 *   @n This function returns data transmit ready status. Value returned by this
 *      API is useful to decide whether data can be written to MMCSD controller
 *      FIFO or not.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd     Handle to the MMCSD
            pStatus    Status of API success or failed
     @endverbatim
 *
 *   <b> Return Value </b>  Bool
 *   @li                    TRUE  - MMCSD FIFO is ready to accept data
 *   @li                    FALSE - MMCSD FIFO is not ready to accept data
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Bool                  dxrdyStatus;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            dxrdyStatus = MMC_dxrdy(hMmcsd, &status);
     @endverbatim
 *  ===========================================================================
 */
Bool MMC_dxrdy(CSL_MmcsdHandle    hMmcsd,
               CSL_Status         *pStatus);

/** ===========================================================================
 *   @n@b MMC_saveStatus
 *
 *   @b Description
 *   @n This function stores the value of MMCST0 and MMCST1 registers in the
 *      card object strutcure present in the MMCSD handle.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd    Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  Saves the value of MMCST0 and MMCST1
 *
 *   @b Modifies
 *   @n Card object structure
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            Uint16                maskValue;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_saveStatus(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_saveStatus(CSL_MmcsdHandle    hMmcsd);

/** ===========================================================================
 *   @n@b MMC_getStatus
 *
 *   @b Description
 *   @n This function return the status of particular field of MMCST0 and MMCST1
 *      register according to the passed mask value. Value of MMCST1 will be
 *      stored in upper 16 bits and value of MMCST0 will be stored in lower
 *      16 bits of the returned value.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd       Handle to the MMCSD
            maskValue    Mask value to specify the field.
            pStatus      Status of API success or failed.
     @endverbatim
 *
 *   <b> Return Value </b>  Uint32
 *   @li                   status - MMCST0 and MMCST1 register value.
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            Uint16                maskValue;
            CSL_status            status;
            Uint32                mmcStatus;
            Uint32                maskValue;

            instId = CSL_MMCSD0_INST;
            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            // Get the value of mmcst0 register
            maskValue = 0x0000FFFF;

            mmcStatus = MMC_getStatus(hMmcsd, maskValue, &status);
     @endverbatim
 *  ===========================================================================
 */
Uint32 MMC_getStatus(CSL_MmcsdHandle    hMmcsd,
                     Uint32             maskValue,
                     CSL_Status         *pStatus);

/** ===========================================================================
 *   @n@b MMC_setupNative
 *
 *   @b Description
 *   @n This function will set initial configuration to all the MMCSD registers.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd      Handle to the MMCSD.
            pMmcInit    Pointer to CSL_MMCSetupNative state variable.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  MMCSD Registers will be populated.
 *
 *   @b Modifies
 *   @n MMCSD Registers
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            CSL_MMCConfig         mmcInit;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            Note: CSL_MMCConfig field are following.
            mmcInit.dmaEnable         = CSL_MMCSD_MMCCMD2_DMATRIG_NO;
            mmcInit.dat3EdgeDetection = CSL_MMCSD_MMCCTL_DATEG_DISABLE;
            mmcInit.cdiv              = CSL_MMCSD_CLK_DIV_INIT;
            mmcInit.rspTimeout        = CSL_MMCSD_RESPONSE_TIMEOUT;
            mmcInit.dataTimeout       = CSL_MMCSD_DATA_RW_TIMEOUT;
            mmcInit.blockLen          = CSL_MMCSD_MIN_BLOCK_LENGTH;

            status = MMC_setupNative(hMmcsd, &mmcInit);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setupNative(CSL_MmcsdHandle       hMmcsd,
                           CSL_MMCSetupNative    *pMmcInit);

/** ===========================================================================
 *   @n@b MMC_config
 *
 *   @b Description
 *   @n Function to configure the MMCSD registers. Register configuration will
 *      be done as per the configuration parameters passed to the function.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        Handle to the MMCSD.
            pMmcConfig    Pointer to the MMCSD config parameter.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n MMCSD Register will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            CSL_MMCConfig         mmcConfig;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            Note: CSL_MMCConfig field are following.
            mmcConfig.mmcblen = CSL_MMCSD_BLOCK_LENGTH;
            mmcConfig.mmcclk  = 0x10 |CSL_MMCSD_MMCCLK_CLKEN_ENABLE;
            mmcConfig.mmcctl  = CSL_MMCSD_MMCCTL_DATEG_F_EDGE;
            mmcConfig.mmcim   = 0x0;
            mmcConfig.mmcnblk = 0x1;
            mmcConfig.mmctod  = CSL_MMCSD_DATA_RW_TIMEOUT;
            mmcConfig.mmctor  = CSL_MMCSD_RESPONSE_TIMEOUT;

            status = MMC_config(hMmcsd, &mmcConfig);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_config(CSL_MmcsdHandle    hMmcsd,
                      CSL_MMCConfig      *pMmcConfig);

/** ===========================================================================
 *   @n@b MMC_getConfig
 *
 *   @b Description
 *   @n This function will fetch the information present in the MMCSD
 *      registers and update to passed the config parameter.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        Handle to the MMCSD.
            pMmcConfig    Pointer to the MMCSD config Parameter.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            CSL_MMCConfig         pMmcConfig)

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_getConfig(hMmcsd, &pMmcConfig);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_getConfig(CSL_MmcsdHandle    hMmcsd,
                         CSL_MMCConfig      *pMmcConfig);

/** ===========================================================================
 *   @n@b MMC_sendCmd
 *
 *   @b Description
 *   @n This is used to send a command to the memory card. Lower 16 bits of the
 *      parameter 'cmd' should contain the command that needs to be written to
 *      MMCCMD1 register. Upper 16 bits of the 'cmd' should contain the data
 *      that needs to be written to MMCCMD2 register.
 *
 *      NOTE:SETTING THE MMCCMD2 VALUE TO '1' IS ALLOWED ONLY TO DATA WRITE
 *      COMMANDS THAT USES DMA FOR DATA TRANSFER. FOR OTHER COMMANDS UPPER
 *      16 BITS OF 'CMD' SHOULD ALWAYS BE ZERO.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        Handle to the MMCSD.
            cmd           Command index.
            arg           Argument.
            waitForRsp    Wait cycle for response.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Close successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Uint32                cmd;
            Uint32                arg;
            Uint16                waitForRsp;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            cmd        = CSL_MMCSD_WRITE_INIT_CMD;
            arg        = CSL_MMCSD_ARG_RESET;
            waitForRsp = CSL_MMCSD_RESPONSE_TIMEOUT;

            status = MMC_sendCmd(hMmcsd, cmd, arg, waitForRsp);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_sendCmd(CSL_MmcsdHandle    hMmcsd,
                       Uint32             cmd,
                       Uint32             arg,
                       Uint16             waitForRsp);

/** ===========================================================================
 *   @n@b MMC_sendGoIdle
 *
 *   @b Description
 *   @n This function broadcasts Go Idle command for Setting all cards
 *      to the idle state.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd    Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n This should be called just after MMC_open() API.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_sendGoIdle(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_sendGoIdle(CSL_MmcsdHandle    hMmcsd);

/** ===========================================================================
 *   @n@b MMC_stop
 *
 *   @b Description
 *   @n Function to stop the MMC/SD transfer and receive events. This function
 *      sends CMD12 to the memory card.
 *
 *   NOTE: STOP command should be sent to the card when it is in 'data'(read)
 *   or 'rcv'(write) state. Sending STOP command while the card in any other
 *   state will result in command timeout.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *   @n  Card should be addressed.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_stop(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_stop(CSL_MmcsdHandle    hMmcsd);

/** ===========================================================================
 *   @n@b MMC_deselectCard
 *
 *   @b Description
 *   @n This function will send command for deselecting particular card which
 *      is assigned in card object.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pMmcCardObj       Pointer to the MMCSD Card Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *   @n  RCA value should be properly stored in handle hMmcsd.
 *
 *   <b> Post Condition </b>
 *   @n  Deselects the card
 *
 *   @b Modifies
 *   @n MMCSD card object structure.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          mmcCardObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_deselectCard(hMmcsd, &mmcCardObj);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_deselectCard(CSL_MmcsdHandle    hMmcsd,
                            CSL_MMCCardObj     *pMmcCardObj);

/** ===========================================================================
 *   @n@b MMC_selectCard
 *
 *   @b Description
 *   @n This function detects which card (MMC/SD/SDHC) is inserted. Information
 *      of the card detected will be populated to the card object structure
 *      passed as parameter.
 *
 *      MMC object structure passed to this function should global structure
 *      variable which persists till end of the program since it is stored by
 *      MMCSD CSL for reference in other APIs.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pMmcCardObj       Pointer to the MMCSD Card Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MMCCardObj        mmcCardObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &mmcCardObj);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_selectCard(CSL_MmcsdHandle    hMmcsd,
                          CSL_MMCCardObj     *pMmcCardObj);

/** ===========================================================================
 *   @n@b MMC_sendOpCond
 *
 *   @b Description
 *   @n This function will set the clock to the SD or MMC Card according to
 *      passed parameter of Card Type.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD
            clkDivValue       Clock Div value for MMC or SD card.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *
 *   <b> Pre Condition </b>
 *   @n  1:MMC_open() API should be called successfully.
 *   @n  2:MMC_selectCard() API should be called before this so that it will do
 *       some needful work for particular card detected.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          mmcCardObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &mmcCardObj);

            pMmmcCardObj->cardType = CSL_MMC_CARD;

            clkDivValue = 0x4 //as MMC clock max is 20MHz and Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_sendOpCond(CSL_MmcsdHandle    hMmcsd,
                             Uint16             clkDivValue);

/** ===========================================================================
 *   @n@b MMC_setRca
 *
 *   @b Description
 *   @n This function sets the relative card address for MMC Card. Relative
 *      card address is stored in CSL_MMCCardObj which can used by other
 *      MMCSD CSL APIs.
 *
 *      Application programs should use only this function to set the RCA of
 *      MMC card. RCA passed to this function will be stored in the MMCSD
 *      handle and is used by other MMCSD CSL APIs.
 *
 *      NOTE: THIS FUNCTION IS SPECIFIC TO MMC CARD AND SHOULD NOT BE USED WITH
 *      THE SD CARD. FOR MMC CARD CMD3(SET_RELATIVE_ADDR) IS USED TO SET THE
 *      CARD ADDRESS AND FOR SD CARD CMD3(SEND_RELATIVE_ADDR) IS USED TO REQUEST
 *      THE CARD ADDRESS.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pMmcCardObj       Pointer to the MMC Card Info.
            rCardAddr         Relative card address.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:MMC_open() API should be called successfully.
 *   @n  2:MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3: If Card detected is MMC then MMC_sendOpCond() for specific to MMC.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          mmcCardObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;
            Uint16                  rCardAddr;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &mmcCardObj);

            clkDivValue = 0x4 //as MMC clock max is 20MHz and Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);
            rCardAddr = 0x100;

            status = MMC_setRca(hMmcsd, &mmcCardObj, rCardAddr);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setRca(CSL_MmcsdHandle     hMmcsd,
                      CSL_MMCCardObj      *pMmcCardObj,
                      Uint16              rCardAddr);

/** ===========================================================================
 *   @n@b SD_sendRca
 *
 *   @b Description
 *   @n This function sends request for relative card address(RCA) of SD Card.
 *      RCA value published by SD card will be populated to card address
 *      parameter. RCA is also stored in CSL_MMCCardObj to use in other MMCSD
 *      CSL APIs.
 *
 *      Application programs should use only this function to requesting the
 *      RCA from SD card. RCA published by the SD card will be stored in the
 *      MMCSD handle and is used by other MMCSD CSL APIs.
 *
 *      NOTE: THIS FUNCTION IS SPECIFIC TO SD CARD AND SHOULD NOT BE USED WITH
 *      THE MMC CARD. FOR MMC CARD CMD3(SET_RELATIVE_ADDR) IS USED TO SET THE
 *      CARD ADDRESS AND FOR SD CARD CMD3(SEND_RELATIVE_ADDR) IS USED TO REQUEST
 *      THE CARD ADDRESS.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pSdCardObj        Pointer to the SD Card Info.
            pRCardAddr        Pointer to store relative card address.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:MMC_open() API should be called successfully.
 *   @n  2:MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3:If Card detected is SD then MMC_sendOpCond() for specific to SD.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n pRCardAddr variable will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          sdCardObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;
            Uint16                  rCardAddr;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &sdCardObj);

            clkDivValue = 0x1; //As SD clock max is 50MHz and Function clock is 100MHz

            status = MMC_sendOpCond(hMmcsd, clkDivValue);
            status = SD_sendRca(hMmcsd, &sdCardObj, &rCardAddr);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SD_sendRca(CSL_MmcsdHandle     hMmcsd,
                      CSL_MMCCardObj      *pSdCardObj,
                      Uint16              *pRCardAddr);

/** ===========================================================================
 *   @n@b MMC_sendAllCID
 *
 *   @b Description
 *   @n This will fetch the Information about MMC Card Id and update into the
 *      passed id structure.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd           Handle to the MMCSD.
            pMmcCardIdObj    Pointer to the MMC Card ID Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1.MMC_open() API should be called successfully.
 *   @n  2.MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3.If Card detected is MMC then MMC_sendOpCond() for specific to MMC.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n pMmcCardIdObj will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          mmcCardObj;
            CSL_MMCCardIdObj        mmcCardIdObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &mmcCardObj);

            clkDivValue = 0x4 //As MMC clock max is 20MHz and
                                Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = MMC_sendAllCID(hMmcsd, &mmcCardIdObj);
            For example following value will be populated.
            pMmcCardIdObj->mfgId         = ;
            pMmcCardIdObj->serialNumber  = ;
            pMmcCardIdObj->monthCode     = ;
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_sendAllCID(CSL_MmcsdHandle     hMmcsd,
                          CSL_MMCCardIdObj    *pMmcCardIdObj);

/** ===========================================================================
 *   @n@b SD_sendAllCID
 *
 *   @b Description
 *   @n This will fetch the Information about SD Card Id and update into the
 *      passed id structure.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd           Handle to the MMCSD.
            pSdCardIdObj     Pointer to the SD Card ID Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:MMC_open() API should be called successfully.
 *   @n  2:MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3:If Card detected is SD then MMC_sendOpCond() specific to SD.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n pSdCardIdObj object will be populated.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          sdCardObj;
            CSL_MMCCardIdObj        sdCardIdObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &sdCardObj);

            clkDivValue = 0x1 //As SD clock max is 50MHz and
                                Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = SD_sendAllCID(hMmcsd, &sdCardIdObj);

            For example following value will be populated.
            sdCardIdObj.mfgId         = ;
            sdCardIdObj.serialNumber  = ;
            sdCardIdObj.monthCode     = ;
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SD_sendAllCID(CSL_MmcsdHandle     hMmcsd,
                         CSL_MMCCardIdObj    *pSdCardIdObj);

/** ===========================================================================
 *   @n@b MMC_getCardCsd
 *
 *   @b Description
 *   @n This function will update the CSD Structure information of MMC Card.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pMmcCardCsdObj    Pointer to the MMC Card CSD Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1. MMC_open() API should be called successfully.
 *   @n  2. MMC_selectCard() API should be called before this so that it will do
 *          some needful work for particular card detected.
 *   @n  3. if Card detected is MMC then MMC_sendOpCond()for specific to MMC.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n CSD object structure
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle         hMmcsd;
            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MMCCardObj          mmcCardObj;
            CSL_MMCCardCsdObj       mmcCardCsdObj;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint16                  clkDivValue;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &mmcCardObj);

            clkDivValue = 0x4 //as MMC clock max is 20MHz and
                                Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = MMC_getCardCsd(hMmcsd, &mmcCardCsdObj);

            For example following value will be populated.
            pMmcCardCsdObj->readBlLen         = ;
            pMmcCardCsdObj->cSize  = ;

            These value will be useful to fetch the card specific information
            as shown below. These figure are tested with 256Mbyte MMC Card

            blockLength  = 512 Bytes
            cardCapacity = 0x260636672 Bytes
            totalSectors = 0x509056 sector

     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_getCardCsd(CSL_MmcsdHandle      hMmcsd,
                          CSL_MMCCardCsdObj    *pMmcCardCsdObj);

/** ===========================================================================
 *   @n@b SD_getCardCsd
 *
 *   @b Description
 *   @n This function will update the CSD Structure info of SD Card.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD.
            pSdCardCsdObj     Pointer to the SD Card CSD Info.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - Command Reponse Timeout
 *   @li                    CSL_EMMCSD_CRCERR   - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  1:MMC_open() API should be called successfully.
 *   @n  2:MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3:If Card detected is SD then MMC_sendOpCond()for specific to SD.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MMCCardObj        sdCardObj;
            CSL_MMCCardCsdObj     sdCardCsdObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Uint16                clkDivValue;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &sdCardObj);

            clkDivValue = 0x1 //As SD clock max is 50MHz and Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = SD_getCardCsd(hMmcsd, &sdCardCsdObj);
            For example following value will be populated.
            sdCardCsdObj.readBlLen         = ;
            sdCardCsdObj.cSize  = ;
            These values will be useful to fetch the card specific information
            like these-----
            These figure are tested with 1GB Ultra SD Card
            blockLength      = 512 Bytes
            cardCapacity     = 0x1015808000 Bytes
            totalSectors     = 0x1984000 sector
     @endverbatim
 *  ===========================================================================
 */
CSL_Status SD_getCardCsd(CSL_MmcsdHandle      hMmcsd,
                         CSL_MMCCardCsdObj    *pSdCardCsdObj);

/** ===========================================================================
 *   @n@b MMC_read
 *
 *   @b Description
 *   @n This function reads data from the memory card at a given address
 *      and populates the data to read buffer.
 *
 *      'cardAddr' represents the address location in the card to be accessed.
 *      For the standard capacity cards this parameter will be a value in
 *      multiples of 512. That means to read a sector number 1 cardAddr
 *      will be 1*512, to read a sector number 2 cardAddr will be 2*512
 *      and so on.
 *      For the high capacity cards 'cardAddr' parameter will be same as the
 *      sector number. That means to read a sector number 1 cardAddr will be
 *      1, to read a sector number 2 cardAddr will be 2 and so on.
 *      Reason for this is high capacity cards uses BLOCK addressing instead
 *      of the BYTE addressing used by the standard capacity cards to increase
 *      the addressable memory capacity.
 *
 *      Detection of high capacity cards can be verified using the flag
 *      'sdHcDetected' in the MMCSD card object structure.
 *      sdHcDetected = TRUE      - SD card is high capacity card
 *      sdHcDetected = FALSE     - SD card is standard capacity card
 *
 *      NOTE: CARE SHOULD BE TAKEN WHILE PASSING THE 'cardAddr' PARAMETER.
 *      USING BYTE ADDRESSING WITH HIGH CAPACITY CARDS WILL LEAD TO OUT OF
 *      SECTOR BOUNDARY ACCESS AND RESULTS IN CODE HANGING FOR MMCSD RESPONSE.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'readEndianMode' in the MMC handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during MMC open. Endian mode can
 *      be configured using MMC_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *      NOTE: When transferring data in DMA mode, word swap will be done for
 *      the data buffer when MMCSD is configured for little endian mode.
 *      No word swap is required in upper SW layers.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd         Handle to the MMCSD.
            cardAddr       Location to read from.
            noOfBytes      Number of bytes to read.
            pReadBuffer    Pointer to a buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - card Response time out
 *
 *   <b> Pre Condition </b>
 *   @n  1. MMC_open() API should be called successfully.
 *   @n  2. MMC_selectCard() API should be called before this so that it will do
 *          some needful work for particular card detected.
 *   @n  3. MMC_sendOpCond()for specific to card.
 *   @n  4. If card detected is SD then call SD_getCardCsd() specific to SD.
 *          If card detected is MMC then call MMC_getCardCsd() specific to MMC
 *
 *   <b> Post Condition </b>
 *   @n  Populates data to read buffer.
 *
 *   @b Modifies
 *   @n Read data buffer.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MMCCardObj        sdCardObj;
            CSL_MMCCardCsdObj     sdCardCsdObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Uint16                clkDivValue;
            Uint16                noOfBytes;
            Uint16                cardAddr;
            Uint16                readBuffer[256];

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &sdCardObj);

            clkDivValue = 0x1 //as SD clock max is 50MHz and Function clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = SD_getCardCsd(hMmcsd, &sdCardCsdObj);
            For example following value will be populated.
            sdCardCsdObj.readBlLen = ;
            sdCardCsdObj.cSize     = ;
            These values will be useful to fetch the card specific information
            like these-----
            These figure are tested with 1GB SD Card
            blockLength      = 512 Bytes
            cardCapacity     = 0x1015808000 Bytes
            totalSectors     = 0x1984000 sector

            cardAddr = 0x200;
            noOfBytes = 512;

            status = MMC_read(hMmcsd, cardAddr, noOfBytes, readBuffer);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_read(CSL_MmcsdHandle    hMmcsd,
                    Uint32             cardAddr,
                    Uint16             noOfBytes,
                    Uint16             *pReadBuffer);

/** ===========================================================================
 *   @n@b MMC_write
 *
 *   @b Description
 *   @n This function writes data in the write data buffer to the memory card
 *      at a given address.
 *
 *      'cardAddr' represents the address location in the card to be accessed.
 *      For the standard capacity cards this parameter will be a value in
 *      multiples of 512. That means to write a sector number 1 cardAddr
 *      will be 1*512, to write a sector number 2 cardAddr will be 2*512
 *      and so on.
 *      For the high capacity cards 'cardAddr' parameter will be same as the
 *      sector number. That means to write a sector number 1 cardAddr will be
 *      1, to write a sector number 2 cardAddr will be 2 and so on.
 *      Reason for this is high capacity cards uses BLOCK addressing instead
 *      of the BYTE addressing used by the standard capacity cards to increase
 *      the addressable memory capacity.
 *
 *      Detection of high capacity cards can be verified using the flag
 *      'sdHcDetected' in the MMCSD card object structure.
 *      sdHcDetected = TRUE      - SD card is high capacity card
 *      sdHcDetected = FALSE     - SD card is standard capacity card
 *
 *      NOTE: CARE SHOULD BE TAKEN WHILE PASSING THE 'cardAddr' PARAMETER.
 *      USING BYTE ADDRESSING WITH HIGH CAPACITY CARDS WILL LEAD TO OUT OF
 *      SECTOR BOUNDARY ACCESS AND RESULTS IN CODE HANGING FOR MMCSD RESPONSE.
 *
 *      NOTE: Endian mode of the data transfer depends on the parameter
 *      'writeEndianMode' in the MMC handle. Default value for the endian mode
 *      is 'LITTLE ENDIAN' which will be set during MMC open. Endian mode can
 *      be configured using MMC_setEndianMode(). DO NOT CHANGE THE ENDIAN MODE
 *      WITH OUT USING THIS API.
 *
 *      NOTE: When transferring data in DMA mode, word swap will be done for
 *      the data buffer when MMCSD is configured for little endian mode.
 *      No word swap is required in upper SW layers.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD
            cardAddr          location to read from.
            noOfBytes         no of bytes to read.
            pWriteBuffer      pointer to a buffer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid Parameter
 *   @li                    CSL_EMMCSD_TIMEOUT  - card Response time out
 *
 *   <b> Pre Condition </b>
 *   @n  1.MMC_open() API should be called successfully.
 *   @n  2.MMC_selectCard() API should be called before this so that it will do
 *         some needful work for particular card detected.
 *   @n  3. MMC_sendOpCond()for specific to card .
 *   @n  4. If Card detected is SD then call SD_getCardCsd() specific to SD.
 *          If Card detected is SD then call MMC_getCardCsd() specific to MMC
 *
 *   <b> Post Condition </b>
 *   @n  Writes the data to card
 *
 *   @b Modifies
 *   @n MMC/SD card will populated with the passed buffer values.
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle       hMmcsd;
            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MMCCardObj        sdCardObj;
            CSL_MMCCardCsdObj     sdCardCsdObj;
            CSL_MmcsdInstId       instId;
            CSL_status            status;
            Uint16                clkDivValue;
            Uint16                noOfBytes;
            Uint16                cardAddr;
            Uint16                writeBuffer[256];

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            status = MMC_selectCard(hMmcsd, &sdCardObj);

            sdCardObj.cardType = CSL_SD_CARD;
             clkDivValue = 0x1 //as SD clock max is 50MHz and Function
                                 clock is 100MHz
            status = MMC_sendOpCond(hMmcsd, clkDivValue);

            status = SD_getCardCsd(hMmcsd, &sdCardCsdObj);
            For example following value will be populated.
            pSdCardCsdObj->readBlLen         = ;
            pSdCardCsdObj->cSize  = ;
            These value will be useful to fetch the card specific information
            These figure are tested with 1GB Ultra SD Card
            blockLength  = 512 Bytes
            cardCapacity = 0x1015808000 Bytes
            totalSectors = 0x1984000 sector

            cardAddr = 0x200;
            noOfBytes = 512;

            status = MMC_write(hMmcsd, cardAddr, noOfBytes, &writeBuffer);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_write(CSL_MmcsdHandle    hMmcsd,
                     Uint32             cardAddr,
                     Uint16             noOfBytes,
                     Uint16             *pWriteBuffer);

/** ===========================================================================
 *   @n@b MMC_setDmaHandle
 *
 *   @b Description
 *   @n This function stores the DMA handles sent by the application program.
 *      DMA handle stored in MMCSD handle will be used by the MMC read/write
 *      APIs to transfer the data in DMA mode. This API requires that MMCSD
 *      operating mode should be set to DMA using MMC_open function.
 *      DMA channel handles should be sent by the application in proper order.
 *      DMA channel needs to opened before calling this API. This channels
 *      will be closed by the MMC_close API.
 *
 *      NOTE:This API should not be called when MMCSD is operating Polled mode.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd       MMCSD handle
            hDmaWrite    Handle to DMA channel used for MMCSD write operation
            hDmaRead     Handle to DMA channel used for MMCSD read operation
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid DMA handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() and DMA_open() functions should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n   Stores the DMA handle in MMC handle
 *
 *   @b Modifies
 *   @n    MMC handle
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_DMA_Handle        dmaWrHandle;
            CSL_DMA_Handle        dmaRdHandle;
            CSL_DMA_ChannelObj    dmaWrChanObj;
            CSL_DMA_ChannelObj    dmaRdChanObj;

            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            dmaWrHandle = DMA_open(&dmaWrChanObj, CSL_DMA_CHAN0);

            dmaRdHandle = DMA_open(&dmaRdChanObj, CSL_DMA_CHAN1);

            // Set DMA handle
            status = MMC_setDmaHandle(hMmcsd, dmaWrHandle, dmaRdHandle);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setDmaHandle (CSL_MmcsdHandle    hMmcsd,
                             CSL_DMA_Handle     hDmaWrite,
                             CSL_DMA_Handle     hDmaRead);

/** ===========================================================================
 *   @n@b MMC_setDataTransferCallback
 *
 *   @b Description
 *   @n This function stores the data transfer call back function pointer in the
 *      MMCSD handle. This callback pointer is used by the MMCSD write and read
 *      APIs to issue callback during data transfer operations. This is to
 *      facilitate application/driver programs handle MMCSD interrupt in their
 *      own way depending on their requirements.
 *      Callback function is used when MMCSD is operating in DMA or interrupt
 *      mode.
 *      During DMA mode operation, call back function will be called after
 *      starting the DMA data transfer.
 *      During interrupt mode operation call back function will be called
 *      1. For checking read/write command response
 *      2. For checking transit/receive ready bit
 *      3. For checking data done bit
 *
 *      In both the operating modes callback function should wait for the
 *      MMCSD interrupt. Upon receiving the interrupt, all the MMCSD events
 *      should be disabled. MMCSD handle will be passed to the callback
 *      function as a parameter.
 *
 *      Setting the callback function is optional for DMA mode but is
 *      mandatory for interrupt mode operation. MMC_read/MMC_write APIs
 *      return with error in case of no callback is set and MMCSD is
 *      configured for interrupt mode.
 *
 *      NOTE: Same callback pointer will be used by write and read APIs.
 *            Callback function is applicable to DMA and interrupt mode only.
 *            Should not call this API when MMCSD is operation in polled mode.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd                  MMCSD handle
            dataTransferCallback    Transfer done callback pointer
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Stores the transfer done callback pointer in MMC handle
 *
 *   @b Modifies
 *   @n MMC handle
 *
 *   @b Example
 *   @verbatim
            CSL_MmcsdHandle                hMmcsd;
            CSL_MMCDataTxferCallBackPtr    dataTransferCallback;
            CSL_MmcsdInstId                instId;
            CSL_status                     status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = MMC_setDataTransferCallback(hMmcsd, dataTransferCallback);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setDataTransferCallback(CSL_MmcsdHandle                hMmcsd,
                                       CSL_MMCDataTxferCallBackPtr    dataTransferCallback);

/** ===========================================================================
 *   @n@b MMC_setDmaChanConfig
 *
 *   @b Description
 *   @n This function is used to set the DMA channel configurations.
 *      DMA channel configuration values sent by the application are stored
 *      in the MMCSD handle and used by the MMC write and read functions.
 *      DMA read and write channel configurations can be set independently
 *      with only one channel at a time using this function. Direction of
 *      the DMA channel is identified using the member 'chanDir' of
 *      CSL_MMCDmaConfig structure. Operating mode of the MMCSD module
 *      needs to be set to DMA mode for using this API.
 *
 *      Members of CSL_MMCDmaConfig structure are subset of CSL_DMA_Config
 *      structure. Other configuration values DMA source address, destination
 *      address, DMA data length etc are not configurable by MMCSD application
 *      program and are set by the MMCSD read/write API depending on the
 *      request from file system.
 *
 *      Only 4word and 8word DMA burst length is allowed for MMCSD (burstLen).
 *      Any other burst length will be ignored and DMA will be configured to
 *      8word burst length by default.
 *
 *      This API is an optional one. Default DMA configurations are assigned by
 *      the MMC_open function. No need to call this API until and unless
 *      a different DMA configurations are required by the application.
 *
 *      NOTE:This API should not be called when MMCSD is operating Polled mode.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd       MMCSD handle
            dmaConfig    DMA configuration structure
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid configuration parameters
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n   Stores the DMA configuration values in MMC handle
 *
 *   @b Modifies
 *   @n    MMC handle
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj    pMmcsdContObj;
            CSL_MmcsdHandle         hMmcsd;;
            CSL_MmcsdDmaConfig      dmaWriteCfg;
            CSL_MmcsdDmaConfig      dmaReadCfg;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            dmaWriteCfg.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            dmaWriteCfg.burstLen     = CSL_DMA_TXBURST_8WORD;
            dmaWriteCfg.chanDir      = CSL_DMA_WRITE;
            dmaWriteCfg.dmaInt       = CSL_DMA_INTERRUPT_ENABLE;

            // Set the DMA write channel configuration
            status = MMC_setDmaChanConfig(hMmcsd, dmaWriteCfg);

            dmaReadCfg.autoMode     = CSL_DMA_AUTORELOAD_DISABLE;
            dmaReadCfg.burstLen     = CSL_DMA_TXBURST_8WORD;
            dmaReadCfg.chanDir      = CSL_DMA_READ;
            dmaReadCfg.dmaInt       = CSL_DMA_INTERRUPT_ENABLE;

            // Set the DMA read channel configuration
            status = MMC_setDmaChanConfig(hMmcsd, dmaReadCfg);

    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setDmaChanConfig (CSL_MmcsdHandle       hMmcsd,
                                 CSL_MmcsdDmaConfig    dmaConfig);

/** ===========================================================================
 *   @n@b MMC_setEndianMode
 *
 *   @b Description
 *   @n This function is used to set endian mode of the MMC/SD controller.
 *      Endian mode set using this API decides the way in which data is
 *      transferred from FIFO to storage device.
 *
 *      Default endianness of MMCSD controller:
 *      Chip C5504 and C5505: LITTLE ENDIAN.
 *      Chip C5514 and C5515: BIG ENDIAN.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd             MMCSD handle
            writeEndianMode    Endian mode for MMC/SD write operation
            readEndianMode     Endian mode for MMC/SD read operation
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n   Sets MMC/SD endian mode for write and operations
 *
 *   @b Modifies
 *   @n    MMC/SD control register
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = MMC_setEndianMode(hMmcsd, CSL_MMCSD_ENDIAN_LITTLE,
                                      CSL_MMCSD_ENDIAN_LITTLE);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setEndianMode (CSL_MmcsdHandle        hMmcsd,
                              CSL_MmcsdEndianMode    writeEndianMode,
                              CSL_MmcsdEndianMode    readEndianMode);

/** ===========================================================================
 *   @n@b MMC_setBlockLength
 *
 *   @b Description
 *   @n This function is used to set block length of the memory card using
 *      command 16. In the case of a Standard Capacity SD Memory Card or
 *      MMC card, command 16 sets the block length(in bytes) for all following
 *      block commands (read, write, lock). Default block length is fixed to
 *      512 bytes. In the case of a High Capacity SD memory card, block length
 *      set by CMD16 command does not affect the memory read and write
 *      commands. Always 512 bytes fixed block length is used.
 *
 *      This API does not verify the value of block length passed
 *      as parameter. Calling functions should make sure that the block length
 *      passed is proper with respect to the card under operation.
 *
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd      MMCSD handle
            blockLen    Length of the memory card block
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li            CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets MMC/SD block length
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj  mmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_status            status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = MMC_setBlockLength(hMmcsd, CSL_MMCSD_BLOCK_LENGTH);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setBlockLength (CSL_MmcsdHandle    hMmcsd,
                               Uint32             blockLen);

/** ===========================================================================
 *   @n@b MMC_setWriteBlkEraseCnt
 *
 *   @b Description
 *   @n Function to set the number of write blocks to be preerased before
 *      writing. Set write block erase count command need to be used for faster
 *      multiple block write command. Calling this function is not mandatory.
 *      In case of calling this API for better performance, it should be called
 *      just before calling MMC_write function.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd      MMCSD handle
            blkCnt      Number of blocks
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li            CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets preerase block count
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdHandle         hMmcsd;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = MMC_setWriteBlkEraseCnt(hMmcsd, 16);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_setWriteBlkEraseCnt(CSL_MmcsdHandle    hMmcsd,
                                   Uint32             blkCnt);

/** ===========================================================================
 *   @n@b SD_setBusWidth
 *
 *   @b Description
 *   @n Function to set the bus with for SD card. This function sends ACMD6 to
 *      the SD card to configure the bus width. This function should be called
 *      after selecting the card using CMD7.
 *
 *      NOTE: This function is specific to SD card.Should not be used with MMC.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd        MMCSD handle
            busWidth      bus width of the card; 0 - 1 bit, 1 - 4 bit
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *   @li            CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li            CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets SD card bus width
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdHandle         hMmcsd;
            CSL_MmcsdInstId         instId;
            CSL_status              status;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = SD_setBusWidth(hMmcsd, 1);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SD_setBusWidth(CSL_MmcsdHandle    hMmcsd,
                          Uint8              busWidth);

/** ===========================================================================
 *   @n@b MMC_getCardStatus
 *
 *   @b Description
 *   @n Function to get the status of the memory card. This function sends
 *      CMD13 to memory card to get the status. Memory card status will have
 *      the useful information to determine the card status and errors if any
 *      occurred. Refer to MMC/SD specification for the details of fields
 *      contained in the MMC/SD status.
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd         MMCSD handle
            pCardStatus    Pointer to store the status sent by the card
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *   @li            CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li            CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Gets card status
 *
 *   @b Modifies
 *   @n Card status pointer
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdHandle         hMmcsd;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint32                  cardStatus;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);

            status = MMC_getCardStatus(hMmcsd, &cardStatus);
    @endverbatim
 *  ===========================================================================
 */
CSL_Status MMC_getCardStatus(CSL_MmcsdHandle    hMmcsd,
                             Uint32             *pCardStatus);

/** ===========================================================================
 *   @n@b SD_configurePullup
 *
 *   @b Description
 *   @n Function to Connenct/Disconnect the SD card internal pull-up resistors.
 *      This function is specific to SD card; cannot be used with MMC card
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd         MMCSD handle
            pullupCfg      Flag indicate the pull-up configuration
                             0 - Disconnects the pull-up resistor
                             1 - Connects the pull-up resistor
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li            CSL_SOK            - Returned for Success
 *   @li            CSL_ESYS_BADHANDLE - Invalid MMSDC handle
 *   @li            CSL_ESYS_INVPARAMS - Invalid input parameters
 *   @li            CSL_EMMCSD_TIMEOUT - Command Reponse Timeout
 *   @li            CSL_EMMCSD_CRCERR  - Command CRC Error
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() function should be called successfully
 *   @n  Card should be selected
 *
 *   <b> Post Condition </b>
 *   @n  Connencts/Disconnects SD card pull-up resistor
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            CSL_MMCControllerObj    mmcsdContObj;
            CSL_MmcsdHandle         hMmcsd;
            CSL_MmcsdInstId         instId;
            CSL_status              status;
            Uint32                  cardStatus;

            instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&mmcsdContObj, instId, CSL_MMCSD_OPMODE_DMA,
                              &status);
            ....

            //Select the card

            ....

            status = SD_configurePullup(hMmcsd, 0);
    @endverbatim
 *  ===========================================================================
 */
CSL_Status SD_configurePullup(CSL_MmcsdHandle    hMmcsd,
                              Bool               pullupCfg);

/**
@} */

/******************************************************************************
 * CSL MMCSD Inline function
 *****************************************************************************/
/** @addtogroup CSL_MMCSDC_INLINE_FUNCTION
 @{ */

/** ===========================================================================
 *   @n@b MMC_getEventId
 *
 *   @b Description
 *   @n Function return the events of MMCSD(Status of MMCSD Event).
 *
 *   @b Arguments
 *   @verbatim
            hMmcsd            Handle to the MMCSD
            maskValue         mask value to specify the interrupt state.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  MMC_open() API should be called successfully before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_MMCControllerObj  pMmcsdContObj;
            CSL_MmcsdHandle       hMmcsd;
            CSL_MmcsdInstId       instId;
            CSL_MMCSDEventType    eventId;
            CSL_status            status;

               instId = CSL_MMCSD0_INST;

            hMmcsd = MMC_open(&pMmcsdContObj, instId, CSL_MMCSD_OPMODE_POLLED,
                              &status);

            eventId = MMC_getEventId(hMmcsd);
     @endverbatim
 *  ===========================================================================
 */
static inline
CSL_MMCSDEventType MMC_getEventId(
                          CSL_MmcsdHandle     hMmcsd)
{

    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_DATDNE_MASK)
        return CSL_MMCSD_EDATDNE_INTERRUPT;
    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_BSYDNE_MASK)
        return CSL_MMCSD_EBSYDNE_INTERRUPT;
    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_RSPDNE_MASK)
        return CSL_MMCSD_ERSPDNE_INTERRUPT;
    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_TOUTRD_MASK)
        return CSL_MMCSD_ETOUTRD_INTERRUPT;
    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_TOUTRS_MASK)
        return CSL_MMCSD_ETOUTRS_INTERRUPT;
    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_CRCWR_MASK)
        return CSL_MMCSD_ECRCWR_INTERRUPT;
    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_CRCRD_MASK)
        return CSL_MMCSD_ECRCRD_INTERRUPT;
    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_CRCRS_MASK)
        return CSL_MMCSD_ECRCRS_INTERRUPT;
    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_DXRDY_MASK)
        return CSL_MMCSD_EDXRDY_INTERRUPT;
    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_DRRDY_MASK)
        return CSL_MMCSD_EDRRDY_INTERRUPT;
    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_DATED_MASK)
        return CSL_MMCSD_EDATED_INTERRUPT;
    if(hMmcsd->mmcRegs->MMCST0 & CSL_MMCSD_MMCST0_TRNDNE_MASK)
        return CSL_MMCSD_ETRNDNE_INTERRUPT;

    return CSL_MMCSD_INTERRUPT_NONE;
}
/**
@} */

#ifdef __cplusplus
}
#endif
#endif /* _CSL_MMCSD_H_ */

