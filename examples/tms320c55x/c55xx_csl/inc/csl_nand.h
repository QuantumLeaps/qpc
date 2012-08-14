/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_nand.h
 *
 *  @brief NAND functional layer API header file
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 21-Aug-2008 Created
 * 07-Oct-2008 Modified According to new Register layer files
 * ============================================================================
 */

/** @defgroup CSL_NAND_API NAND
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 *  NAND flash is a memory device based on Electrically Erasable-Programmable
 *  Read only memory (EEPROM) technology. There are two basic types of nand
 *  devices: Big Block and Small Block. Difference between these two types is
 *  physical page size of the device. Normally page size is 512 bytes for
 *  a small block nand and 2048 bytes for a big block nand.
 *  As the nand flash is an external memory device it is interfaced to the
 *  processor using EMIF(External Memory InterFace) module.
 *
 *  @subsection References
 *
 */

#ifndef _CSL_NAND_H_
#define _CSL_NAND_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_types.h>
#include <cslr_emif.h>
#include <soc.h>
#include <csl_general.h>


/**
@defgroup CSL_NAND_SYMBOL  NAND Symbols Defined
@ingroup CSL_NAND_API
*/
/**
@defgroup CSL_NAND_DATASTRUCT  NAND Data Structures
@ingroup CSL_NAND_API
*/
/**
@defgroup CSL_NAND_FUNCTION  NAND Functions
@ingroup CSL_NAND_API
*/
/**
@defgroup CSL_NAND_ENUM NAND Enumerated Data Types
@ingroup CSL_NAND_API
*/


/**
@addtogroup CSL_NAND_SYMBOL
@{
*/
/*****************************************************************************\
          NAND global macro declarations                                      *
\*****************************************************************************/

/**
 * \brief NAND device standard commands
 */
/** Command to access low page                                               */
#define CSL_NAND_SB_CMD_LO_PAGE                      (0x00u)
/** Command to access high page                                              */
#define CSL_NAND_SB_CMD_HI_PAGE                      (0x01u)
/** Command to access spare area - not found in Big Block NANDs              */
#define CSL_NAND_SB_CMD_SPARE_AREA                   (0x50u)
/** Command to access device Id                                              */
#define CSL_NAND_CMD_DEVID                           (0x90u)
/** Command to access device address                                         */
#define CSL_NAND_CMD_DEVADD                          (0x00u)
/** Command to perform a reset                                               */
#define CSL_NAND_CMD_RESET                           (0xFFu)
/** Command to perform a set                                                 */
#define CSL_NAND_CMD_SET                             (0x00u)
/** Command to start programming NAND                                        */
#define CSL_NAND_CMD_PGRM_START                      (0x80u)
/** Command to stop programming NAND                                         */
#define CSL_NAND_CMD_PGRM_END                        (0x10u)
/** Command to erase block                                                   */
#define CSL_NAND_CMD_BLK_ERASE_CMD1                  (0x60u)
/** Command to erase block                                                   */
#define CSL_NAND_CMD_BLK_ERASE_CMD2                  (0xD0u)
/** Command to put NAND in ready state                                       */
#define CSL_NAND_CMD_READY                           (0x40u)
/** Command to get the status of NAND                                        */
#define CSL_NAND_CMD_STATUS                          (0x70u)
/** Command to read the start of NAND                                        */
#define CSL_NAND_CMD_READ_START                      (0x00u)
/** Command to read the end of NAND                                          */
#define CSL_NAND_CMD_READ_END                        (0x30u)

/**
 * \brief Big block NAND extra commands
 */
/** Command to perform a random data input <b>(Big block only command)</b>   */
#define CSL_NAND_BB_CMD_RANDOM_PGRM_START            (0x85u)
/** Command to start a random data output  <b>(Big block only command)</b>   */
#define CSL_NAND_BB_CMD_RANDOM_READ_START            (0x05u)
/** Command to end a random data output    <b>(Big block only command)</b>   */
#define CSL_NAND_BB_CMD_RANDOM_READ_END              (0xE0u)
/** Command to perform a Copy-back read    <b>(Big block only command)</b>   */
#define CSL_NAND_BB_CMD_COPY_BACK_READ               (0x35u)


/** First bit-position for NAND device flags                                 */
#define CSL_NAND_FLAG_FIRSTBITPOS                    (0u)
/** Write protected bit-flag                                                 */
#define CSL_NAND_MEDIA_WRITE_PROTECTED ((1L) << (CSL_NAND_FLAG_FIRSTBITPOS + 0))
/** Last bit-position for NAND device flags                                  */
#define CSL_NAND_FLAG_LASTBITPOS                     (1u)

/**
 * \brief NAND Status Bit definitions - Big Block
 *
 */
/** Program/erase operation passed                                           */
#define CSL_NAND_BB_CMD_PGRM_STATUS_PASS             (0x0000)
/** NAND Ready for command after a cache operation                           */
#define CSL_NAND_BB_CMD_CACHE_PGRM_READY             (0x0020u)
/** NAND Ready for commands after an operation                               */
#define CSL_NAND_BB_CMD_CMD_STATUS_READY             (0x0040u)
/** NAND device not write protected status                                   */
#define CSL_NAND_BB_CMD_WP_STATUS_OPEN               (0x0080u)
/** Status check for any Big block NAND operation                            */
#define CSL_NAND_BB_CMD_STATUS_READY    (CSL_NAND_BB_CMD_CMD_STATUS_READY | \
                                         CSL_NAND_BB_CMD_PGRM_STATUS_PASS)
/** BIg block nand command status success                                    */
#define CSL_NAND_BB_CMD_STATUS_SUCCESS               (CSL_NAND_BB_CMD_STATUS_READY)

/**
 * \brief NAND Status Bit definitions - Small Block
 *
 */
/** Program/erase operation passed                                           */
#define CSL_NAND_SB_CMD_PGRM_STATUS_PASS             (0x0000)
/** NAND Ready for commands after an operation                               */
#define CSL_NAND_SB_CMD_CMD_STATUS_READY             (0x0040u)
/** Not write protected                                                      */
#define CSL_NAND_SB_CMD_WP_STATUS_OPEN               (0x0080u)
/** Status check for any Small block NAND operation                          */
#define CSL_NAND_SB_CMD_STATUS_READY    (CSL_NAND_SB_CMD_CMD_STATUS_READY | \
                                         CSL_NAND_SB_CMD_PGRM_STATUS_PASS)
/** Small block nand command status success                                  */
#define CSL_NAND_SB_CMD_STATUS_SUCCESS               (CSL_NAND_SB_CMD_STATUS_READY)


/**
 * \brief Extended ID masks
 *
 */
/** Page size mask of extended ID                                            */
#define CSL_NAND_PAGE_SIZE_MASK                      (0x03u)
/** Block size mask of extended ID                                           */
#define CSL_NAND_BLOCK_SIZE_MASK                     (0x30u)
/** Redundant area size mask of extended ID                                  */
#define CSL_NAND_RED_AREA_SIZE_MASK                  (0x04u)
/** Organization (8/16 bit) mask of extended ID                              */
#define CSL_NAND_ORGANIZATION_MASK                   (0x40u)

/**
 * \brief ECC options for NAND Banks
 *
 */
/** Enable ECC calculation for bank 1 - maps to CS2ECC                       */
#define CSL_NAND_ECC_BANK1_ENABLE                    (0x100u)
/** Enable ECC calculation for bank 2 - maps to CS3ECC                       */
#define CSL_NAND_ECC_BANK2_ENABLE                    (0x200u)
/** Enable ECC calculation for bank 3 - maps to CS4ECC                       */
#define CSL_NAND_ECC_BANK3_ENABLE                    (0x400u)
/** Enable ECC calculation for bank 4 - maps to CS5ECC                       */
#define CSL_NAND_ECC_BANK4_ENABLE                    (0x800u)

/**
 * \brief Big block NAND extra features
 */
/** Big-Block NAND has cache programming command/feature                     */
#define CSL_NAND_BB_CACHE_PROG                      (0x01u)
/** Big-Block NAND has copy-back command/feature                             */
#define CSL_NAND_BB_COPY_BACK                       (0x02u)
/** Big-Block NAND has random read/random output command/feature             */
#define CSL_NAND_BB_RANDOM_PAGE_READ                (0x04u)
/** Big-Block NAND has random write/random input command/feature             */
#define CSL_NAND_BB_RANDOM_PAGE_WRITE               (0x08u)
/** Extra commands/options/features only for Big Block NAND devices          */
#define CSL_NAND_BB_OPTIONS                   (CSL_NAND_BB_CACHE_PROG       |\
                                               CSL_NAND_BB_COPY_BACK        |\
                                               CSL_NAND_BB_RANDOM_PAGE_READ |\
                                               CSL_NAND_BB_RANDOM_PAGE_WRITE )
/** NAND options for Bigblock MLC NAND flash                                 */
#define CSL_NAND_BB_MLC_NAND                    0x10

/** Maximum time-out for NAND                                                */
#define CSL_NAND_MAX_TIMEOUT                        (0x7FFFFFFFu)
/** NAND general delay                                                       */
#define CSL_NAND_DELAY                              (100)
/** NAND read status time out                                                */
#define CSL_NAND_READ_STATUS_TIMEOUT                (100000u)
/** Big block NAND page size                                                 */
#define CSL_NAND_BB_PAGE_SIZE                       (2048u)
/** NAND reset counter register value                                        */
#define CSL_NAND_RESET_COUNT_VAL                    (0x20)
/** NAND invalid interrupt number                                            */
#define CSL_NAND_INV_INTR_NUM                       (0xFFFF)

/** \brief Default values of NAND Async Wait Config structure                */
/** Default value for wait polarity                                          */
#define CSL_NAND_ASYNCWAITCFG_WAITPOL_DEFAULT       (0x00)
/** Default value for Nand port                                              */
#define CSL_NAND_ASYNCWAITCFG_NANDPORT_DEFAULT      (0x00)
/** Default value maximum exteneded wait cycles                              */
#define CSL_NAND_ASYNCWAITCFG_WAITCYCLE_DEFAULT     (0x80)

/** \brief Default values of NAND Async Config structure                     */
/** Default value for strobe mode select bit                                 */
#define CSL_NAND_ASYNCCFG_SELECTSTROBE_DEFAULT      (0x00)
/** Default value for extended wait mode bit                                 */
#define CSL_NAND_ASYNCCFG_WEMODE_DEFAULT            (0x00)
/** Default value for write setup cycles                                     */
#define CSL_NAND_ASYNCCFG_WSETUP_DEFAULT            (0x0F)
/** Default value for write strobe cycles                                    */
#define CSL_NAND_ASYNCCFG_WSTROBE_DEFAULT           (0x1F)
/** Default value for write hold cycles                                      */
#define CSL_NAND_ASYNCCFG_WHOLD_DEFAULT             (0x07)
/** Default value for read setup cycles                                      */
#define CSL_NAND_ASYNCCFG_RSETUP_DEFAULT            (0x0F)
/** Default value for read strobe cycles                                     */
#define CSL_NAND_ASYNCCFG_RSTROBE_DEFAULT           (0x3F)
/** Default value for read hold cycles                                       */
#define CSL_NAND_ASYNCCFG_RHOLD_DEFAULT             (0x07)
/** Default value for turn around cycles                                     */
#define CSL_NAND_ASYNCCFG_TAROUND_DEFAULT           (0x03)
/** Default value for bus width                                              */
#define CSL_NAND_ASYNCCFG_ASIZE_DEFAULT             (0x00)

/**
 * \brief EMIF CSL Bit position macros
 */
/** LSB bit position for CS2 ECC1 register                                   */
#define CSL_EMIF_NCS2ECC1_LSB                       (0)
/** MSB bit position for CS2 ECC1 register                                   */
#define CSL_EMIF_NCS2ECC1_MSB                       (11)
/** LSB bit position for CS2 ECC2 register                                   */
#define CSL_EMIF_NCS2ECC2_LSB                       (0)
/** MSB bit position for CS2 ECC2 register                                   */
#define CSL_EMIF_NCS2ECC2_MSB                       (11)
/** LSB bit position for CS3 ECC1 register                                   */
#define CSL_EMIF_NCS3ECC1_LSB                       (0)
/** MSB bit position for CS3 ECC1 register                                   */
#define CSL_EMIF_NCS3ECC1_MSB                       (11)
/** LSB bit position for CS3 ECC2 register                                   */
#define CSL_EMIF_NCS3ECC2_LSB                       (0)
/** MSB bit position for CS3 ECC2 register                                   */
#define CSL_EMIF_NCS3ECC2_MSB                       (11)
/** LSB bit position for CS4 ECC1 register                                   */
#define CSL_EMIF_NCS4ECC1_LSB                       (0)
/** MSB bit position for CS4 ECC1 register                                   */
#define CSL_EMIF_NCS4ECC1_MSB                       (11)
/** LSB bit position for CS4 ECC2 register                                   */
#define CSL_EMIF_NCS4ECC2_LSB                       (0)
/** MSB bit position for CS4 ECC2 register                                   */
#define CSL_EMIF_NCS4ECC2_MSB                       (11)
/** LSB bit position for CS5 ECC1 register                                   */
#define CSL_EMIF_NCS5ECC1_LSB                       (0)
/** MSB bit position for CS5 ECC1 register                                   */
#define CSL_EMIF_NCS5ECC1_MSB                       (11)
/** LSB bit position for CS5 ECC2 register                                   */
#define CSL_EMIF_NCS5ECC2_LSB                       (0)
/** MSB bit position for CS5 ECC2 register                                   */
#define CSL_EMIF_NCS5ECC2_MSB                       (11)

/** Enables NAND Word Acces                                                  */
#define CSL_NAND_WORD_ACCESS                         (0)
/** Enables NAND Low Byte Acces                                              */
#define CSL_NAND_HIGHBYTE_ACCESS                     (1)
/** Enables NAND High Byte Acces                                             */
#define CSL_NAND_LOWBYTE_ACCESS                      (2)


/**
 * \brief NAND module specific errors
 */

/**
  *  \brief Macro to enable disable byte access
  *  byte = 0 - Enables word access
  *  byte = 1 - Enables High byte access
  *  byte = 2 - Enables Low byte access
  */
#define CSL_NAND_CHANGE_ACCESSTYPE(byte)                                                \
                     CSL_FINS(CSL_SYSCTRL_REGS->ESCR, SYS_ESCR_BYTEMODE, byte);

/** NAND Time out Error                                                      */
#define CSL_NAND_E_TIMEOUT                          (CSL_ENAND_FIRST - 0)

#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

/** Macro for writing a word to nand device                                  */
#define CSL_NAND_WRITEWORD(hNand, data)                                      \
        if(hNand->nandWidth == CSL_NAND_8_BIT)                               \
        {                                                                    \
            *(CSL_VUint8*)hNand->bank.CExDATA = (Uint8)((data >> 8) & 0xFF); \
            *(CSL_VUint8*)hNand->bank.CExDATA = (Uint8)(data & 0xFFu);       \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            *(CSL_VUint16*)hNand->bank.CExDATA = (Uint16)data;               \
        }

/** Macro for reading a word from nand device                                */
#define CSL_NAND_READWORD(hNand, data)                                    \
        if(hNand->nandWidth == CSL_NAND_8_BIT)                            \
        {                                                                 \
            data  = ((*(CSL_VUint8*)hNand->bank.CExDATA) & 0xFFu);        \
            data  = ((*(CSL_VUint8*)hNand->bank.CExDATA) | (data << 8));  \
        }                                                                 \
        else                                                              \
        {                                                                 \
            data = (*(CSL_VUint16*)hNand->bank.CExDATA);                  \
        }
#else

/** Macro for writing a word to nand device                                  */
#define CSL_NAND_WRITEWORD(hNand, data)                                      \
        if(hNand->nandWidth == CSL_NAND_8_BIT)                               \
        {                                                                    \
            *(CSL_VUint8*)hNand->bank.CExDATA = (Uint8)(data & 0xFFu);       \
            *(CSL_VUint8*)hNand->bank.CExDATA = (Uint8)((data >> 8) & 0xFF); \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            *(CSL_VUint16*)hNand->bank.CExDATA = (Uint16)data;               \
        }

/** Macro for reading a word from nand device                                */
#define CSL_NAND_READWORD(hNand, data)                                  \
        if(hNand->nandWidth == CSL_NAND_8_BIT)                          \
        {                                                               \
            data  = ((*(CSL_VUint8*)hNand->bank.CExDATA)  & 0xFFu);     \
            data |= ((*(CSL_VUint8*)hNand->bank.CExDATA)  << 8);        \
        }                                                               \
        else                                                            \
        {                                                               \
            data = (*(CSL_VUint16*)hNand->bank.CExDATA);                \
        }

#endif


/** Macro for writing a byte to nand device                                  */
#define CSL_NAND_WRITEBYTE(hNand ,data)                                 \
        if(hNand->nandWidth == CSL_NAND_8_BIT)                          \
        {                                                               \
            *(CSL_VUint8*)hNand->bank.CExDATA = (Uint8)(data);          \
        }                                                               \
        else                                                            \
        {                                                               \
            *(CSL_VUint16*)hNand->bank.CExDATA = (Uint16)data;          \
        }

/** Macro for reading a byte from nand device                                */
#define CSL_NAND_READBYTE(hNand ,addr)                                  \
        if(hNand->nandWidth == CSL_NAND_8_BIT)                          \
        {                                                               \
            *addr  = (*(CSL_VUint8*)hNand->bank.CExDATA);               \
        }                                                               \
        else                                                            \
        {                                                               \
            *addr = (*(CSL_VUint16*)hNand->bank.CExDATA);               \
        }

/** Macro to read a command from command latch address                       */
#define CSL_NANDGETCOMMAND(hNand,cmd)                                   \
         cmd = (*(CSL_VUint16*)hNand->bank.CExCLE);                     \

/** Macro to read address from command latch address                         */
#define CSL_NANDGETADDRESS(hNand,addr)                                  \
         addr = *(CSL_VUint16*)hNand->bank.CExALE;                      \

/** Macro to read nand async wait config register                            */
#define CSL_NANDGETWAITCFG(hNand,value)                                 \
         value = (Uint32)((hNand->regs->AWCCR1) | (hNand->regs->AWCCR2 << 16));


/** Macro to read nand async 1 config register                               */
#define CSL_NANDGETCS2ASYNCCFG(hNand,value)                             \
         value = (Uint32)(hNand->regs->ACS2CR1 |                        \
                          (hNand->regs->ACS2CR2 << 16));                \

/** Macro to read nand async 2 config register                               */
#define CSL_NANDGETCS3ASYNCCFG(hNand,value)                             \
         value = (Uint32)(hNand->regs->ACS3CR1 |                        \
                          (hNand->regs->ACS3CR2 << 16));                \

/** Macro to read nand async 3 config register                               */
#define CSL_NANDGETCS4ASYNC3CFG(hNand,value)                            \
         value = (Uint32)(hNand->regs->ACS4CR1 |                        \
                          (hNand->regs->ACS4CR2 << 16));                \

/** Macro to read nand async 4 config register                               */
#define CSL_NANDGETCS5ASYNC4CFG(hNand,value)                            \
         value = (Uint32)(hNand->regs->ACS5CR1 |                        \
                          (hNand->regs->ACS5CR2 << 16));                \

/** Macro to read nand control register                                      */
#define CSL_NANDGETCTRL(hNand,value)                                    \
         value = (Uint16)(CSL_FEXTR(hNand->regs->NANDFCR, 13, 0));

/** Macro to read nand interrupt raw register                                */
#define CSL_NANDGETINTRAW(hNand,intrStatus)                             \
         intrStatus = (Uint16)(CSL_FEXTR(hNand->regs->EIRR, 5, 0));

/** Macro to read nand interrupt mask register                               */
#define CSL_NANDGETINTMASK(hNand,intrStatus)                            \
         intrStatus = (Uint16)(CSL_FEXTR(hNand->regs->EIMR, 5, 0));

/** Macro to read nand interrupt mask set register                           */
#define CSL_NANDGETINTMASKSET(hNand,intrStatus)                         \
         intrStatus = (Uint16)(CSL_FEXTR(hNand->regs->EIMSR, 5, 0));

/** Macro to read nand interrupt mask clear register                         */
#define CSL_NANDGETINTMASKCLEAR(hNand,intrStatus)                       \
         intrStatus = (Uint16)(CSL_FEXTR(hNand->regs->EIMCR, 5, 0));

/**
@} */


/**************************************************************************\
* NAND global typedef declarations                                         *
\**************************************************************************/

/** @addtogroup CSL_NAND_ENUM
 @{ */

/**
 * \brief NAND Width enumeration
 */
typedef enum {
    /** External Device Bus Width 8 bit                                      */
    CSL_NAND_8_BIT  = (0u),
    /** External Device Bus Width 16 bit                                     */
    CSL_NAND_16_BIT = (1u)
} CSL_NandWidth;

/**
 * \brief Type of NAND detected.
 */
typedef enum {
    /** No NAND detected                                                     */
    CSL_NAND_NONE = 0u,
    /** Big Block NAND detected                                              */
    CSL_NAND_BIG_BLOCK,
    /** Small Block NAND detected                                            */
    CSL_NAND_SMALL_BLOCK,
    /** Unknown/Invalid NAND                                                 */
    CSL_NAND_INVALID
} CSL_NandType;

/**
 * \brief NAND Page size enumeration
 */
typedef enum {
    /** Page size is 256 + 8bytes                                            */
    CSL_NAND_PAGESZ_256  = (0u),
    /** Page size is 512 + 16bytes                                           */
    CSL_NAND_PAGESZ_512  = (1u),
    /** Page size is 1024 + 32 bytes                                         */
    CSL_NAND_PAGESZ_1024 = (2u),
    /** Page size is 2048 + 64 bytes                                         */
    CSL_NAND_PAGESZ_2048 = (3u)
} CSL_NandPageSize;

/**
 * \brief   NAND Asynchronous Bank number
 */
typedef enum {
    /** NAND Bank 0 - EMIF CS2                                               */
    CSL_NAND_BANK_0 = (0u),
    /** NAND Bank 1 - EMIF CS3                                               */
    CSL_NAND_BANK_1 = (1u),
    /** NAND Bank 2 - EMIF CS4                                               */
    CSL_NAND_BANK_2 = (2u),
    /** NAND Bank 3 - EMIF CS5                                               */
    CSL_NAND_BANK_3 = (3u),
    /** NAND Bank Max (Book-keep)                                            */
    CSL_NAND_BANK_MAX
} CSL_NandBankNo;

/**
 * \brief   NAND Chip selection
 */
typedef enum {
    /** NAND Chip selsect 2                                                  */
    CSL_NAND_CS2 = (0u),
    /** NAND Chip selsect 3                                                  */
    CSL_NAND_CS3 = (1u),
    /** NAND Chip selsect 4                                                  */
    CSL_NAND_CS4 = (2u),
    /** NAND Chip selsect 5                                                  */
    CSL_NAND_CS5 = (3u)
} CSL_NandChipSelect;

/**
 * \brief   NAND Ports
 */
typedef enum {
    /** NAND RDY0                                                            */
    CSL_NAND_RDY0 = (0u),
    /** NAND RDY1                                                            */
    CSL_NAND_RDY1 = (1u),
    /** ASYNC RDY0                                                           */
    CSL_ASYNC_RDY0 = (2u),
    /** ASYNC RDY1                                                           */
    CSL_ASYNC_RDY1 = (3u)
} CSL_NandPort;

/**
 * \brief   NAND wait polarity
 */
typedef enum {
    /** NAND wait polarity low                                               */
    CSL_NAND_WP_LOW = (0u),
    /** NAND wait polarity high                                              */
    CSL_NAND_WP_HIGH = (1u)
} CSL_NandWaitPol;

/**
 * \brief   NAND Operating mode
 */
typedef enum {
    /** NAND polled mode of operation                                        */
    CSL_NAND_OPMODE_POLLED = (0u),
    /** NAND DMA mode of operation                                           */
    CSL_NAND_OPMODE_DMA = (1u),
    /** NAND Interrupt mode operation                                        */
    CSL_NAND_OPMODE_INTRRUPT = (2u)
} CSL_NandOpMode;

/**
 * \brief   EMIF Access width
 */
typedef enum {
    /** Emif 16-bit word access                                              */
    CSL_NAND_EMIF_16BIT = (0u),
    /** Emif 8-bit word access that selects the high byte                    */
    CSL_NAND_EMIF_8BIT_HIGH = (1u),
    /** Emif 8-bit word access that selects the low byte                     */
    CSL_NAND_EMIF_8BIT_LOW = (2u)
} CSL_NandEmifAccess;

/**
 * \brief   NAND instance number
 */
typedef enum {
    /** NAND hardware instance 0                                             */
    CSL_NAND_INST_0 = (0u),
    /** Invalid NAND hardware instance                                       */
    CSL_NAND_INST_INVALID = (1u)
} CSL_NandInsId;


/**
@} */

/**************************************************************************\
* NAND global data structure declarations                                  *
\**************************************************************************/

/**
\addtogroup CSL_NAND_DATASTRUCT
@{ */

/**
 * \brief Nand Async wait config structure
 */
typedef struct CSL_NandAsyncWaitCfg {
    /** Wait polarity for nand ports                                         */
    CSL_NandWaitPol       waitPol;
    /** Nand port Id                                                         */
    CSL_NandPort          nandPort;
    /** EMIF maximum extended wait cycles                                    */
    Uint16              waitCycles;
} CSL_NandAsyncWaitCfg;

/**
 * \brief Nand Async config structure
 */
typedef struct CSL_NandAsyncCfg {
    /** Strobe mode selection                                                */
    Uint16    selectStrobe;
    /** Enable extended wait mode                                            */
    Uint16    ewMode;
    /** Write strobe setup cycles                                            */
    Uint16    w_setup;
    /** Write strobe duration cycles                                         */
    Uint16    w_strobe;
    /** Write strobe hold cycle                                              */
    Uint16    w_hold;
    /** Read strobe setup cycles                                             */
    Uint16    r_setup;
    /** Read strobe duration cycles                                          */
    Uint16    r_strobe;
    /** Read strobe hold cycle                                               */
    Uint16    r_hold;
    /** Turn around cycles                                                   */
    Uint16    turnAround;
    /** Asynchronous memory size                                             */
    Uint16    aSize;
} CSL_NandAsyncCfg;

/**
 * \brief Current Asynchronous NAND Bank configuration.
 */
typedef struct CSL_NandAsyncBank {
    /** Current NAND Data register                                           */
    volatile Uint32     CExDATA;
    /** Current NAND Address latch register                                  */
    volatile Uint32     CExALE;
    /** Current NAND Command latch register                                  */
    volatile Uint32     CExCLE;
    /** Current NAND ECC register                                            */
    volatile Uint32     CExNUM;
} CSL_NandAsyncBank;

/**
 * \brief Nand Hw configuration structure
 */
typedef struct CSL_NandConfig {
    /** External Device Width.                                               */
    CSL_NandWidth          nandWidth;
    /** NAND page Size                                                       */
    CSL_NandPageSize       nandPageSize;
    /** Asynchronous bank                                                    */
    CSL_NandBankNo         nandBankNo;
    /** Byte mode of Emif access                                             */
    CSL_NandEmifAccess     emifAccess;
    /** NAND Operating mode                                                  */
    CSL_NandOpMode         nandOpMode;
    /** Type of the nand device                                              */
    CSL_NandType           nandType;
    /** Nand chip select Id                                                  */
    CSL_NandChipSelect     chipSelect;
    /** Nand Async wait config structure                                     */
    CSL_NandAsyncWaitCfg   *asyncWaitCfg;
    /** Nand Async config structure                                          */
    CSL_NandAsyncCfg       *asyncCfg;
} CSL_NandConfig;

/**
 * \brief This object contains the reference to the instance of nand device
 *
 *  This object is initialized during nand initialization and passed as
 *  the parameter to all nand CSL APIs
 */
typedef struct CSL_NandObj {
    /** EMIF register overlay structure                                      */
    CSL_EmifRegsOvly      regs;
    /** CPU system control register overlay structure                        */
    CSL_SysRegsOvly       sysCtrlRegs;
    /** Instance Id of NAND on the SoC.                                      */
    Uint16                instId;
    /** Width of NAND (8 or 16 bit)                                          */
    CSL_NandWidth         nandWidth;
    /** Interrupt number of NAND                                             */
    Int                   intrNumNand;
    /** Current NAND Asynchronous bank                                       */
    CSL_NandAsyncBank     bank;
    /** NAND page Size                                                       */
    CSL_NandPageSize      nandPageSize;
    /** NAND Address cycles for setting page/block address                   */
    Uint16                addrCycles;
    /** NAND Operating mode                                                  */
    CSL_NandOpMode        nandOpMode;
    /** Type of the nand device                                              */
    CSL_NandType          nandType;
    /** Nand chip select Id                                                  */
    CSL_NandChipSelect    chipSelect;
    /** Wait pin selected for NAND                                           */
    Uint8                 waitPin;
} CSL_NandObj;

/**  \brief Nand object structure pointer                                    */
typedef CSL_NandObj *CSL_NandHandle;

/**
@} */


/**************************************************************************\
* NAND function declarations                                               *
\**************************************************************************/

/** @addtogroup CSL_NAND_FUNCTION
 @{ */
/** ============================================================================
 *   @n@b NAND_init
 *
 *   @b Description
 *   @n This is the initialization function for the nand CSL.
 *      It populates nand object structure information. This API should
 *      be called before calling any other nand CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            nandObj       Pointer to nand object structure
            nandInstId    Nand Hardware instance number
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand init is successful
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Nand object structure is populated
 *
 *   @b Modifies
 *   @n Nand object structure
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            CSL_Status     status;

            instId = 0;
            status = NAND_init(&nandObj, instId);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_init(CSL_NandObj    *nandObj,
                     Uint16         nandInstId);

/** ============================================================================
 *   @n@b NAND_setup
 *
 *   @b Description
 *   @n This API configures the nand hardware registers.
 *
 *   @b Arguments
 *   @verbatim
            hNand         Handle to the nand object
            nandConfig    Nand configuration structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand setup is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid config structure
 *
 *   <b> Pre Condition </b>
 *   @n  Nand init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Configures nand registers
 *
 *   @b Modifies
 *   @n nand registers
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj            nandObj;
            Uint16                 instId;
            CSL_Status             status;
            CSL_NandConfig         nandCfg;
            CSL_NandHandle         hNand;
            CSL_NandAsyncWaitCfg   asyncWaitConfig;
            CSL_NandAsyncCfg       asyncConfig;

            instId = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ...
            nandCfg.nandWidth = CSL_NAND_8_BIT;
            ...
            ...
            nandCfg.asyncWaitCfg = &asyncWaitConfig;
            nandCfg.asyncCfg     = &asyncConfig;
            status = NAND_setup(hNand, &nandCfg);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_setup(CSL_NandHandle      hNand,
                      CSL_NandConfig      *nandConfig);

/** ============================================================================
 *   @n@b NAND_getBankInfo
 *
 *   @b Description
 *   @n This API gives the bank details of the nand device. This API populates
 *      the base and offset address locations for the NAND flash based on the
 *      bank number passed as the parameter. Bank number should be decide based
 *      on the EMIF chip select connected to NAND flash as shown below.
 *      CSL_NAND_BANK_0 - EMIF_CS2
 *      CSL_NAND_BANK_1 - EMIF_CS3
 *      CSL_NAND_BANK_2 - EMIF_CS4
 *      CSL_NAND_BANK_3 - EMIF_CS5
 *
 *      NOTE: This API sets ALE and CLE address offset to default values assuming
 *      A11 address bit is connected to ALE signal of NAND flash and A12 address
 *      bit is connected to CLE signal of NAND flash.
 *      Use NAND_setLatchEnableOffset() to change the default offset values.
 *
 *   @b Arguments
 *   @verbatim
            hNand      Handle to the nand object
            bank       Pointer to nand bank info structure
            bankNum    Nand bank number
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand get bank info is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  Nand init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Populates nand bank info structure
 *
 *   @b Modifies
 *   @n Nand bank info structure
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            Uint16         bankNum;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            bankNum = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_getBankInfo(hNand, (CSL_NandAsyncBank*)&hNand->bank, bankNum);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_getBankInfo(CSL_NandHandle      hNand,
                            CSL_NandAsyncBank   *bank,
                            Uint16              bankNum);

/** ============================================================================
 *   @n@b NAND_setLatchEnableOffset
 *
 *   @b Description
 *   @n This API sets the CLE and ALE address offset.
 *      Any of the EMIF address lines can be used to drive ALE and CLE of the
 *      NAND flash. ALE and CLE address offset are decided based of the address
 *      lines used to drive ALE and CLE pins. This API should be called after
 *      calling NAND_getBankInfo() API to modify the ALE and CLE address from
 *      the default value.
 *
 *      NOTE: Addresses passed should be offset values from the base address.
 *      For example assume that EMIF A11 line is used to drive ALE pin and
 *      EMIF A12 line is used to driver CLE pin. Then the address offset for
 *      ALE will be 0x1000 and address offset for CLE will be 0x2000.
 *
 *      Calling this API is mandatory in case of NAND ALE and CLE connected to
 *      pins other than A11 and A12 respectively. Otherwise NAND CSL APIs
 *      will not function properly.
 *
 *   @b Arguments
 *   @verbatim
            hNand         Handle to the nand object
            addrOffset    ALE offset location
            cmdOffset     CLE offset location
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Set address offset is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  Nand init should be called successfully
 *   @n  NAND_getBankInfo() should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets the ALE and CLE address offset
 *
 *   @b Modifies
 *   @n NAND handle
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            Uint16         bankNum;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            bankNum = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_getBankInfo(hNand, (CSL_NandAsyncBank*)&hNand->bank, bankNum);
            status = NAND_setLatchEnableOffset(hNand, 0x2000, 0x4000);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_setLatchEnableOffset(CSL_NandHandle    hNand,
                                     Uint32            addrOffset,
                                     Uint32            cmdOffset);

/** ============================================================================
 *   @n@b NAND_isStatusWriteProtected
 *
 *   @b Description
 *   @n This is the API used to check if the nand device is write protected.
 *      It is not possible write any data to nand if it is write-protected.
 *      So this API must be called before calling the write API to check the
 *      nand status.
 *
 *   @b Arguments
 *   @verbatim
            hNand         Handle to the nand object
            WPstatus      Write protect status pointer
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_init and NAND_getBankInfo should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Write protect status is returned in status variable
 *   @li    TRUE     - NAND device is write protected
 *   @li    FALSE    - NAND device is not write protected
 *
 *   @b Modifies
 *   @n Write protect status variable
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            Uint16         bankNum;
            CSL_Status     status;
            CSL_NandHandle hNand;
            Uint16         wpstatus;

            instId  = 0;
            bankNum = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_getBankInfo(hNand, (CSL_NandAsyncBank*)&hNand->bank, bankNum);
             ....
             ....
            status = NAND_isStatusWriteProtected(hNand,&wpstatus);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_isStatusWriteProtected(CSL_NandHandle      hNand,
                                       Uint16              *WPstatus);

/** ============================================================================
 *   @n@b NAND_sendCommand
 *
 *   @b Description
 *   @n Any command to the nand device should be sent through this API.
 *      nand device is having specific commands for each operation.
 *
 *   @b Arguments
 *   @verbatim
            hNand          Handle to the nand object
            cmd            Command to the nand device
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand send command is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_init and NAND_getBankInfo should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends command to the nand device
 *
 *   @b Modifies
 *   @n Writes command to the command latch address
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            Uint16         bankNum;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            bankNum = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_getBankInfo(hNand, (CSL_NandAsyncBank*)&hNand->bank, bankNum);
            ....
            ....
            status = NAND_sendCommand(hNand,CSL_NAND_CMD_READ_START);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_sendCommand(CSL_NandHandle    hNand,
                            CSL_VUint16       cmd);

/** ============================================================================
 *   @n@b NAND_checkCommandStatus
 *
 *   @b Description
 *   @n This API checks the status of any command to the nand device.
 *      This API must be called after sending any command to the nand device.
 *
 *   @b Arguments
 *   @verbatim
            hNand    Handle to the nand object
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand status check is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_NAND_E_TIMEOUT - Time-out has occured
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_sendCommand API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns time-out condition(If occurs)
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_NandHandle    hNand;
            CSL_Status        status;

            status = NAND_sendCommand(hNand, CSL_NAND_CMD_READ_START);

            if(status == CSL_SOK)
            {
                status = NAND_checkCommandStatus(hNand);
            }
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_checkCommandStatus(CSL_NandHandle    hNand);

/** ============================================================================
 *   @n@b NAND_setAddress
 *
 *   @b Description
 *   @n This API used to set address of nand page to read/write data from/to
 *      nand device. Page address must be set to a valid nand page before
 *      calling read/write APIs.
 *
 *   @b Arguments
 *   @verbatim
            hNand          Handle to the nand object
            addr           Addres value to set
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand set address is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_init and NAND_getBankInfo should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets nand address
 *
 *   @b Modifies
 *   @n Writes address to the address latch
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            Uint16         bankNum;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            bankNum = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_getBankInfo(hNand, (CSL_NandAsyncBank*)&hNand->bank, bankNum);
            ....
            ....
            status = NAND_setAddress(hNand,0x01);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_setAddress(CSL_NandHandle    hNand,
                           Uint16            addr);

/** ============================================================================
 *   @n@b NAND_enableHwECC
 *
 *   @b Description
 *   @n This API enables hardware ECC(Error Check and  Correction) of the
 *      nand device. ECC is used to check and correct errors occurred during
 *      nand data transfer.
 *
 *   @b Arguments
 *   @verbatim
            hNand      Handle to the nand object
            csInput    Nand chip selection input
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand enable HW ECC is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  Nand init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Enables nand hardware error check and correction
 *
 *   @b Modifies
 *   @n Nand ECC registers
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj         nandObj;
            Uint16              instId;
            CSL_Status          status;
            CSL_NandHandle      hNand;
            CSL_NandChipSelect  csInput;

            instId = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            csInput = CSL_NAND_CE0;
            status = NAND_enableHwECC(hNand,csInput);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_enableHwECC(CSL_NandHandle        hNand,
                            CSL_NandChipSelect    csInput);

/** ============================================================================
 *   @n@b NAND_disableHwECC
 *
 *   @b Description
 *   @n This API disables hardware ECC of the nand device.
 *
 *   @b Arguments
 *   @verbatim
            hNand      Handle to the nand object
            csInput    Nand chip selection input
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand disable HW ECC is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_enableHwECC API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Disables nand hardware error check and correction
 *
 *   @b Modifies
 *   @n Nand ECC registers
 *
 *   @b Example
 *   @verbatim
            CSL_Status          status;
            CSL_NandHandle      hNand;
            CSL_NandChipSelect  csInput;

            csInput = CSL_NAND_CE0;

            status = NAND_enableHwECC(hNand, csInput);
            ...
            ...
            status = NAND_disableHwECC(hNand, csInput);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_disableHwECC(CSL_NandHandle        hNand,
                             CSL_NandChipSelect    csInput);

/** ============================================================================
 *   @n@b NAND_readECC
 *
 *   @b Description
 *   @n This API is used to read the information generated by ECC hardware.
 *      Hardware ECC must be enabled before calling this API.
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
            eccBuffer    ECC Buffer pointer
            csInput      Chip select used for calculating Ecc
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand read is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_enableHwECC API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Populates ECC data into eccBuffer
 *
 *   @b Modifies
 *   @n eccBuffer
 *
 *   @b Example
 *   @verbatim
            CSL_Status           status;
            CSL_NandHandle       hNand;
            Uint16               eccBuffer[8];
            CSL_NandChipSelect   csInput;

            csInput = CSL_NAND_CE0;

            status = NAND_enableHwECC(hNand, csInput);
            ...
            ...
            status = NAND_readECC(hNand, eccBuffer, csInput);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_readECC(CSL_NandHandle        hNand,
                        Uint16                *eccBuffer,
                        CSL_NandChipSelect    csInput);

/** ============================================================================
 *   @n@b NAND_readNBytes
 *
 *   @b Description
 *   @n This API reads N number of bytes from a nand physical page.
 *      Number of bytes to be read is sent as an argument to this API.
 *      A read command should be sent and nand page address should be set
 *      before calling this API.
 *
 *   @b Arguments
 *   @verbatim
            hNand          Handle to the nand object
            readRequest    Number of bytes to read
            pReadBuffer    Read buffer pointer
            pack           Data pack flag
            spare          Spare area flag
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand read is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_sendCommand and NAND_setAddress APIs should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Reads the data from nand
 *
 *   @b Modifies
 *   @n Copies data from nand to read buffer
 *
 *   @b Example
 *   @verbatim
            #define  NAND_BUF_SIZE      (1024)

            CSL_Status     status;
            CSL_NandHandle hNand;
            Uint32         address;
            Uint16         nandReadBuf[NAND_BUF_SIZE];

            address = 0x40;
            status = NAND_sendCommand(hNand,CSL_NAND_CMD_READ_START);

            // Set column address
            status = NAND_setAddress(hNand,0x00);
            status = NAND_setAddress(hNand,0x00);
            // Set row address
            status = NAND_setAddress(hNand,0x40);
            status = NAND_setAddress(hNand,0x00);
            status = NAND_setAddress(hNand,0x00);
            ....
            ....
            status = NAND_readNBytes(hNand,NAND_BUF_SIZE,nandreadBuf,0,0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_readNBytes(CSL_NandHandle    hNand,
                           Uint16            readRequest,
                           Uint16            *pReadBuffer,
                           Uint16            pack,
                           Bool                 spare);

/** ============================================================================
 *   @n@b NAND_writeNBytes
 *
 *   @b Description
 *   @n This API writes N number of bytes to a nand physical page.
 *      Number of bytes to be written is sent as an argument to this API.
 *      A page program command should be sent and nand page address
 *      should be set before calling this API. nand device should not be
 *      write-protected for proper functioning of this API.
 *
 *   @b Arguments
 *   @verbatim
            hNand          Handle to the nand object
            writeRequest   Number of bytes to write
            pWriteBuffer   Write buffer pointer
            val            If a value needs to be wriiten directly without
                           using write buffer
            spare          Spare area flag
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand write is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_sendCommand and NAND_setAddress APIs should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Writes the data to nand
 *
 *   @b Modifies
 *   @n Nand write registers
 *
 *   @b Example
 *   @verbatim
            #define  NAND_BUF_SIZE      (1024)

            CSL_Status     status;
            CSL_NandHandle hNand;
            Uint32         address;
            Uint16         nandWriteBuf[NAND_BUF_SIZE];

            address = 0x40;
            status = NAND_sendCommand(hNand,CSL_NAND_CMD_READ_START);
            // Set column address
            status = NAND_setAddress(hNand,0x00);
            status = NAND_setAddress(hNand,0x00);

            // Set row address
            status = NAND_setAddress(hNand,0x40);
            status = NAND_setAddress(hNand,0x00);
            status = NAND_setAddress(hNand,0x00);
            ....
            ....
            status = NAND_writeNBytes(hNand,NAND_BUF_SIZE,nandWriteBuf,0,0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_writeNBytes(CSL_NandHandle     hNand,
                            Uint16             writeRequest,
                            Uint16             *pWriteBuffer,
                            Uint16             val,
                            Bool               spare);

/** ============================================================================
 *   @n@b NAND_hasRandomRead
 *
 *   @b Description
 *   @n This API is used to check whether a nand device is having random
 *      page read or not. If nand is having random read we can set pointer
 *      to any address in the page and retrieve the data from there.
 *
 *   @b Arguments
 *   @verbatim
            nandOptions    Extra commands for big block nand
            RRstatus       Random read status pointer
     @endverbatim
 *
 *   <b> Return Value </b>  None
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  nandOptions information needs to be computed from nand device Id
 *
 *   <b> Post Condition </b>
 *   @n  Random read status is returned in status variable
 *   @li                TRUE     - NAND device is having random read
 *   @li                FALSE    - NAND device is not having random read
 *
 *   @b Modifies
 *   @n Random read status variable
 *
 *   @b Example
 *   @verbatim
             Uint32    nandOptions;
             Uint16    RRstatus;

             //get the nandOptions information from nand device Id
             NAND_hasRandomRead(nandOptions, &RRstatus);
     @endverbatim
 *  ============================================================================
 */
void NAND_hasRandomRead(Uint32    nandOptions,
                        Uint16    *RRstatus);

/** ============================================================================
 *   @n@b NAND_hasRandomWrite
 *
 *   @b Description
 *   @n This API is used to check whether a nand device is having random
 *      page write or not. If nand is having random write we can set pointer
 *      to any address in the page and write the data from there.
 *
 *   @b Arguments
 *   @verbatim
            nandOptions    Extra commands for big block nand
            RWstatus       Random write status pointer
     @endverbatim
 *
 *   <b> Return Value </b>  None
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  nandOptions information needs to be computed from nand device Id
 *
 *   <b> Post Condition </b>
 *   @n  Random write status is returned in status variable
 *   @li                TRUE     - NAND device is having random write
 *   @li                FALSE    - NAND device is not having random write
 *
 *   @b Modifies
 *   @n Random write status variable
 *
 *   @b Example
 *   @verbatim
             Uint32    nandOptions;
             Uint16    RWstatus;

             //get the nandOptions information from nand device Id
             NAND_hasRandomWrite(nandOptions, &RWstatus);
     @endverbatim
 *  ============================================================================
 */
void NAND_hasRandomWrite(Uint32    nandOptions,
                         Uint16    *RWstatus);

/** ============================================================================
 *   @n@b NAND_hasCacheProg
 *
 *   @b Description
 *   @n This API checks whether a nand device supports cache programming
 *      feature or not.
 *
 *   @b Arguments
 *   @verbatim
            nandOptions    Extra commands for big block nand
            CPstatus       Cache program status pointer
     @endverbatim
 *
 *   <b> Return Value </b>  None
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  nandOptions information needs to be computed from nand device Id
 *
 *   <b> Post Condition </b>
 *   @n  Cache program status is returned in status variable
 *   @li                TRUE     - NAND device is having cache programming
 *   @li                FALSE    - NAND device is not having cache programming
 *
 *   @b Modifies
 *   @n Cache program status variable
 *
 *   @b Example
 *   @verbatim
             Uint32    nandOptions;
             Uint16    CPstatus;

             //get the nandOptions information from nand device Id
             NAND_hasCacheProg(nandOptions, &CPstatus);
     @endverbatim
 *  ============================================================================
 */
void NAND_hasCacheProg(Uint32    nandOptions,
                       Uint16    *CPstatus);

/** ============================================================================
 *   @n@b NAND_hasCopyBack
 *
 *   @b Description
 *   @n This API checks whether a nand device supports copy back
 *      feature or not.
 *
 *   @b Arguments
 *   @verbatim
            nandOptions    Extra commands for big block nand
            CBstatus       Copy back status pointer
     @endverbatim
 *
 *   <b> Return Value </b>  None
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  nandOptions information needs to be computed from nand device Id
 *
 *   <b> Post Condition </b>
 *   @n  Copy back status is returned in status variable
 *   @li                TRUE     - NAND device support copy back feature
 *   @li                FALSE    - NAND device does not support copy back feature
 *
 *   @b Modifies
 *   @n Copy back status variable
 *
 *   @b Example
 *   @verbatim
            Uint32    nandOptions;
            Uint16    CBstatus;

            //get the nandOptions information from nand device Id
            NAND_hasCopyBack(nandOptions, &CBstatus);
     @endverbatim
 *  ============================================================================
 */
void NAND_hasCopyBack(Uint32    nandOptions,
                      Uint16    *CBstatus);

/** ============================================================================
 *   @n@b NAND_intrEnable
 *
 *   @b Description
 *   @n This API enables nand interrupts. This API is used in interrupt
 *      mode of operation of the device.
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand interrupt enable is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Enables nand interrupts
 *
 *   @b Modifies
 *   @n Nand interrupt registers
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_intrEnable(hNand);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrEnable(CSL_NandHandle    hNand);

/** ============================================================================
 *   @n@b NAND_intrDisable
 *
 *   @b Description
 *   @n This API is used to disable nand interrupts.
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Nand interrupt disable is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_intrEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Disables nand interrupts
 *
 *   @b Modifies
 *   @n Nand interrupt registers
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            CSL_Status     status;
            CSL_NandHandle hNand;

            instId  = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_intrEnable(hNand);
            ...
            ...
            status = NAND_intrDisable(hNand);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrDisable(CSL_NandHandle    hNand);

/** ============================================================================
 *   @n@b NAND_getIntrNum
 *
 *   @b Description
 *   @n This API returns the interrupt number of a particular nand
 *      hardware instance.
 *
 *   @b Arguments
 *   @verbatim
            nandInstId    Nand Hardware instance number
     @endverbatim
 *
 *   <b> Return Value </b>  Int16
 *   @li                    Interrupt number of the nand device.
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Returns the interrupt number of nand instance
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            int16    intrNum;
            Uint16   instId;

            instId = 0;
            intrNum = NAND_getIntrNum(instId);
     @endverbatim
 *  ============================================================================
 */
Int16 NAND_getIntrNum(Uint16    nandInstId);

/** ============================================================================
 *   @n@b NAND_intrReadAT
 *
 *   @b Description
 *   @n Reads Asynchronous time-out bit of nand interrupt raw register
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
            ATstatus     Asynchronous time-out status variable
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_intrEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Nand asynchronous time-out bit status is returned in status variable
 *
 *   @b Modifies
 *   @n Asynchronous time-out status variable
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            CSL_Status     status;
            CSL_NandHandle hNand;
            Uint16         ATstatus;

            instId  = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_intrEnable(hNand);
            ...
            ...
            status = NAND_intrReadAT(hNand,&ATstatus);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrReadAT(CSL_NandHandle    hNand,
                           Uint16            *ATstatus);

/** ============================================================================
 *   @n@b NAND_intrReadLT
 *
 *   @b Description
 *   @n Reads Line-trap bit of nand interrupt raw register
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
            LTstatus     Line trap status variable
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_intrEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Nand line trap bit status is returned in status variable
 *
 *   @b Modifies
 *   @n Line trap status variable
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            CSL_Status     status;
            CSL_NandHandle hNand;
            Uint16         LTstatus;

            instId  = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_intrEnable(hNand);
            ...
            ...
            status = NAND_intrReadLT(hNand,&LTstatus);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrReadLT(CSL_NandHandle    hNand,
                           Uint16            *LTstatus);

/** ============================================================================
 *   @n@b NAND_intrReadWR
 *
 *   @b Description
 *   @n Reads wait-rise bit of nand interrupt raw register
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
            WRstatus     Wait-raise status variable
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_intrEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Nand wait-rise bit status is returned in status variable
 *
 *   @b Modifies
 *   @n Wait-rise status variable
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
             Uint16         instId;
             CSL_Status     status;
             CSL_NandHandle hNand;
             Uint16         WRstatus;

             instId  = 0;
             status = NAND_init(&nandObj, instId);
             hNand  = &nandObj;
             ....
             ....
            status = NAND_intrEnable(hNand);
            ...
            ...
            status = NAND_intrReadWR(hNand,&WRstatus);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrReadWR(CSL_NandHandle    hNand,
                           Uint16            *WRstatus);

/** ============================================================================
 *   @n@b NAND_intrClear
 *
 *   @b Description
 *   @n Clears a nand interrupt
 *
 *   @b Arguments
 *   @verbatim
            hNand        Handle to the nand object
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_intrEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Clears nand interrupt
 *
 *   @b Modifies
 *   @n Nand interrupt mask register
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj    nandObj;
            Uint16         instId;
            CSL_Status     status;
            CSL_NandHandle hNand;
            Uint16         WRstatus;

            instId  = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ....
            ....
            status = NAND_intrEnable(hNand);
            ...
            ...
            status = NAND_intrClear(hNand);
     @endverbatim
 *  ============================================================================
 */
CSL_Status NAND_intrClear(CSL_NandHandle    hNand);

/** NAND query function                                                      */
/** ============================================================================
 *   @n@b NAND_getSetup
 *
 *   @b Description
 *   @n This API returns configuration of the nand hardware registers.
 *
 *   @b Arguments
 *   @verbatim
            hNand            Handle to the nand object
            getNandConfig    Nand configuration structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  NAND_setup should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns the nand configuration
 *
 *   @b Modifies
 *   @n nand get config structure
 *
 *   @b Example
 *   @verbatim
            CSL_NandObj            nandObj;
            Uint16                 instId;
            CSL_Status             status;
            CSL_NandConfig         nandCfg;
            CSL_NandConfig         getNandConfig;
            CSL_NandHandle         hNand;
            CSL_NandAsyncWaitCfg   asyncWaitConfig;
            CSL_NandAsyncCfg       asyncConfig;
            CSL_NandAsyncWaitCfg   getAsyncWaitConfig;
            CSL_NandAsyncCfg       getAsyncConfig;

            instId = 0;
            status = NAND_init(&nandObj, instId);
            hNand  = &nandObj;
            ...
            nandCfg.nandWidth = CSL_NAND_8_BIT;
            ...
            ...
            nandCfg.asyncWaitCfg = &asyncWaitConfig;
            nandCfg.asyncCfg     = &asyncConfig;
            status = NAND_setup(hNand, &nandCfg);
            nandGetCfg.asyncWaitCfg = &getAsyncWaitConfig;
            nandGetCfg.asyncCfg     = &getAsyncConfig;
            status = NAND_getSetup(hNand, &nandGetCfg);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status NAND_getSetup(CSL_NandHandle      hNand,
                         CSL_NandConfig      *getNandConfig)
{
    volatile ioport Uint16    *asyncCfg0Addr;
    volatile ioport Uint16    *asyncCfg1Addr;
    Int16         loopCount;
    Uint16        tempVal;
    CSL_Status    result;

    result = CSL_SOK;

    if((hNand != NULL) && (getNandConfig != NULL))
    {
        getNandConfig->nandWidth    = hNand->nandWidth;
        getNandConfig->nandPageSize = hNand->nandPageSize;
        getNandConfig->nandOpMode   = hNand->nandOpMode;
        getNandConfig->nandType     = hNand->nandType;

        /* Get the chip select */
        tempVal = CSL_FEXTR(hNand->regs->NANDFCR,
                            CSL_EMIF_NANDFCR_CS5_USE_NAND_SHIFT,
                            CSL_EMIF_NANDFCR_CS2_USE_NAND_SHIFT);

        loopCount = -1;
        while(tempVal)
        {
            loopCount++;
            tempVal = (tempVal >> 1);
        }

        getNandConfig->chipSelect =  (CSL_NandChipSelect)loopCount;

        /* Get async wait config register setting */
        switch(getNandConfig->chipSelect)
        {
            case CSL_NAND_CS2:
                /* CS2 */
                getNandConfig->asyncWaitCfg->nandPort =                            \
                            (CSL_NandPort)CSL_FEXT(hNand->regs->AWCCR2,            \
                                                   EMIF_AWCCR2_CS2_WAIT);

                asyncCfg0Addr = &(hNand->regs->ACS2CR1);
                asyncCfg1Addr = &(hNand->regs->ACS2CR2);
                break;

            case CSL_NAND_CS3:
                /* CS3 */
                getNandConfig->asyncWaitCfg->nandPort =                            \
                            (CSL_NandPort)CSL_FEXT(hNand->regs->AWCCR2,            \
                                                   EMIF_AWCCR2_CS3_WAIT);

                asyncCfg0Addr = &(hNand->regs->ACS3CR1);
                asyncCfg1Addr = &(hNand->regs->ACS3CR2);
                break;

            case CSL_NAND_CS4:
                /* CS4 */
                getNandConfig->asyncWaitCfg->nandPort =                            \
                            (CSL_NandPort)CSL_FEXT(hNand->regs->AWCCR2,            \
                                                   EMIF_AWCCR2_CS4_WAIT);

                asyncCfg0Addr = &(hNand->regs->ACS4CR1);
                asyncCfg1Addr = &(hNand->regs->ACS4CR2);
                break;

            case CSL_NAND_CS5:
                /* CS5 */
                getNandConfig->asyncWaitCfg->nandPort =                            \
                            (CSL_NandPort)CSL_FEXT(hNand->regs->AWCCR2,            \
                                                   EMIF_AWCCR2_CS5_WAIT);

                asyncCfg0Addr = &(hNand->regs->ACS5CR1);
                asyncCfg1Addr = &(hNand->regs->ACS5CR2);
                break;

            default:
                break;
        }

        /* Get the wait polarity */
        getNandConfig->asyncWaitCfg->waitPol =                                     \
                      (CSL_NandWaitPol)CSL_FEXT(hNand->regs->AWCCR2,               \
                                                EMIF_AWCCR2_WP0);

        /* Get the extended wait cycles */
        getNandConfig->asyncWaitCfg->waitCycles = CSL_FEXT(hNand->regs->AWCCR1,    \
                                                             EMIF_AWCCR1_MEWC);


        /* Get async configuration register values */
        /* Get strobe mode select bit */
        getNandConfig->asyncCfg->selectStrobe = CSL_FEXT(*asyncCfg1Addr,           \
                                                         EMIF_ACS2CR2_SS);
        /* Get extended wait mode bit */
        getNandConfig->asyncCfg->ewMode = CSL_FEXT(*asyncCfg1Addr,                 \
                                                   EMIF_ACS2CR2_EW);
        /* Get write srobe setup cycles */
        getNandConfig->asyncCfg->w_setup = CSL_FEXT(*asyncCfg1Addr,                \
                                                    EMIF_ACS2CR2_WSETUP);
        /* Get write duration cycles */
        getNandConfig->asyncCfg->w_strobe = CSL_FEXT(*asyncCfg1Addr,               \
                                                     EMIF_ACS2CR2_WSTROBE);
        /* Get write hold cycles */
        getNandConfig->asyncCfg->w_hold = CSL_FEXT(*asyncCfg1Addr,                 \
                                                   EMIF_ACS2CR2_WHOLD);
        /* Get read strobe setup cycles */
        getNandConfig->asyncCfg->r_setup = ((CSL_FEXT(*asyncCfg1Addr,              \
                                            EMIF_ACS2CR2_RSETUPMSB) << 3) |        \
                                            (CSL_FEXT(*asyncCfg0Addr,              \
                                            EMIF_ACS2CR1_RSETUPLSB) & 0x7));
        /* Get read strobe duration cycles */
        getNandConfig->asyncCfg->r_strobe = CSL_FEXT(*asyncCfg0Addr,               \
                                                     EMIF_ACS2CR1_RSTROBE);
        /* Get read strobe hold cycles */
        getNandConfig->asyncCfg->r_hold = CSL_FEXT(*asyncCfg0Addr,                 \
                                                   EMIF_ACS2CR1_RHOLD);
        /* Get turn around cycles */
        getNandConfig->asyncCfg->turnAround = CSL_FEXT(*asyncCfg0Addr,             \
                                                       EMIF_ACS2CR1_TA);
        /* Get asynchronous memory size */
        getNandConfig->asyncCfg->aSize = CSL_FEXT(*asyncCfg0Addr,                  \
                                                  EMIF_ACS2CR1_ASIZE);

        /* emif Byte access */
        getNandConfig->emifAccess =                                                \
                  (CSL_NandEmifAccess)CSL_FEXT(hNand->sysCtrlRegs->ESCR,           \
                                               SYS_ESCR_BYTEMODE);
    }
    else
    {
        result = CSL_ESYS_INVPARAMS;
    }

    return(result);
}

/**
@} */



#ifdef __cplusplus
}
#endif

#endif    // _CSL_NAND_H_

