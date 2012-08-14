/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_msc.h
 *
 *  @brief USB MSC functional layer API header file
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 07-Oct-2008 Created
 * 08-May-2009 Modified to fix the string desc ASCII to UNICODE conversion issue
 * 28-May-2009 Modified as per the review comments
 * 10-May-2010 Added fullspeed mode support
 * ============================================================================
 */

/** @defgroup CSL_MSC_API MSC
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 * USB Mass Storage Class layer provides interface for the host machine to
 * communicate with the storage media through USB. Storage media like NAND
 * and MMCSD can be exposed as the drives to the windows PC using Msc module.
 * This module uses the functions provided by the MUSB module to communicate
 * with the end points and uses functions provided by the ATA-IF to communicate
 * with the storage media.
 *
 * @subsection References
 * USB MSC driver
 * usb_20.pdf
 */

#ifndef _CSL_MSC_H_
#define _CSL_MSC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <csl_error.h>
#include <csl_types.h>
#include <cslr_usb.h>
#include <csl_usb.h>
#include <soc.h>
#include <csl_general.h>


/**
@defgroup CSL_MSC_SYMBOL  USB MSC Symbols Defined
@ingroup CSL_MSC_API
*/
/**
@defgroup CSL_MSC_DATASTRUCT  USB MSC Data Structures
@ingroup CSL_MSC_API
*/
/**
@defgroup CSL_MSC_FUNCTION  USB MSC Functions
@ingroup CSL_MSC_API
*/
/**
@defgroup CSL_MSC_ENUM USB MSC Enumerated Data Types
@ingroup CSL_MSC_API
*/


/*****************************************************************************\
          USB MSC global macro declarations                                   *
\*****************************************************************************/
/**
@addtogroup CSL_MSC_SYMBOL
@{
*/
/** \brief  USB MSC Command Block Word definitions                           */
/** USB MSC Command Block Word 0                                             */
#define CSL_MSC_CBW_0                                          (1)
/** USB MSC Command Block Word 1                                             */
#define CSL_MSC_CBW_1                                          (2)
/** USB MSC Command Block Word 2                                             */
#define CSL_MSC_CBW_2                                          (3)
/** USB MSC Command Block Word 3                                             */
#define CSL_MSC_CBW_3                                          (4)
/** USB MSC Command Block Word 4                                             */
#define CSL_MSC_CBW_4                                          (5)
/** USB MSC Command Block Word 5                                             */
#define CSL_MSC_CBW_5                                          (6)
/** USB MSC Command Block Word 6                                             */
#define CSL_MSC_CBW_6                                          (7)
/** USB MSC Command Block Word 7                                             */
#define CSL_MSC_CBW_7                                          (8)
/** USB MSC Command Block Word 8                                             */
#define CSL_MSC_CBW_8                                          (9)
/** USB MSC Command Block Word 9                                             */
#define CSL_MSC_CBW_9                                          (10)
/** USB MSC Command Block Word 10                                            */
#define CSL_MSC_CBW_10                                         (11)
/** USB MSC Command Block Word 11                                            */
#define CSL_MSC_CBW_11                                         (12)
/** USB MSC Command Block Word 12                                            */
#define CSL_MSC_CBW_12                                         (13)
/** USB MSC Command Block Word 13                                            */
#define CSL_MSC_CBW_13                                         (14)
/** USB MSC Command Block Word 14                                            */
#define CSL_MSC_CBW_14                                         (15)
/** USB MSC Command Block Word 15                                            */
#define CSL_MSC_CBW_15                                         (16)

/** \brief  USB MSC Command Status Word definitions                          */
/** USB MSC Command Status Word 2                                            */
#define CSL_MSC_CSW_2                                          (3)
/** USB MSC Command Status Word 3                                            */
#define CSL_MSC_CSW_3                                          (4)
/** USB MSC Command Status Word 4                                            */
#define CSL_MSC_CSW_4                                          (5)
/** USB MSC Command Status Word 5                                            */
#define CSL_MSC_CSW_5                                          (6)
/** USB MSC Command Status Word 6                                            */
#define CSL_MSC_CSW_6                                          (7)

/** Valid signature for CBW                                                  */
#define CSL_MSC_CBW_SIGNATURE                                  (0x43425355)
/** Valid signature for CSW                                                  */
#define CSL_MSC_CSW_SIGNATURE                                  (0x53425355)
/** Value of CBW flag byte for DATA IN transaction                           */
#define CSL_MSC_CBW_DATADIR_IN                                 (0x80)
/** Value of CBW flag byte for DATA OUT transaction                          */
#define CSL_MSC_CBW_DATADIR_OUT                                (0x0)
/** Mask for direction bit in CBW flags                                      */
#define CSL_MSC_CBW_DIRBIT_MASK                                (0x80)

/** \brief  USB MSC media state definitions                                  */
/** Storage media available for the operations                               */
#define CSL_MSC_MEDIA_PRESENT                                  (0x1)
/** Storage media locked                                                     */
#define CSL_MSC_MEDIA_LOCKED                                   (0x2)
/** Storage media write operation is in pending                              */
#define CSL_MSC_MEDIA_WRITEPENDING                             (0x4)
/** Bad storage media                                                        */
#define CSL_MSC_MEDIA_BAD                                       (0x8)
/** Enable Storage media write                                               */
#define CSL_MSC_MEDIA_WRITEENABLE                              (0x0001)

/** \brief  USB MSC Descriptor Type Values                                   */
/** USB MSC device descriptor type macro                                     */
#define CSL_MSC_DEVICE_DESCRIPTOR_TYPE                         (0x01)
/** USB MSC device qualifier descriptor type macro                           */
#define CSL_MSC_DEVICE_QUAL_DESCRIPTOR_TYPE                    (0x06)
/** USB MSC configuration descriptor type macro                              */
#define CSL_MSC_CONFIGURATION_DESCRIPTOR_TYPE                  (0x02)
/** USB MSC other speed configuration descriptor type                        */
#define CSL_MSC_OTHERSPEED_CFG_DESCRIPTOR_TYPE                 (0x07)
/** USB MSC string descriptor type macro                                     */
#define CSL_MSC_STRING_DESCRIPTOR_TYPE                         (0x03)
/** USB MSC interface descriptor type macro                                  */
#define CSL_MSC_INTERFACE_DESCRIPTOR_TYPE                      (0x04)
/** USB MSC end point descriptor type macro                                  */
#define CSL_MSC_ENDPOINT_DESCRIPTOR_TYPE                       (0x05)
/** USB MSC power descriptor type macro                                      */
#define CSL_MSC_POWER_DESCRIPTOR_TYPE                          (0x06)
/** USB MSC Human Interface Device descriptor type macro                     */
#define CSL_MSC_HID_DESCRIPTOR_TYPE                            (0x21)

/** \brief  USB MSC Defined Feature selectors                                */
/** Msc host request to set/clear end point stall                            */
#define CSL_MSC_FEATURE_ENDPOINT_STALL                         (0x0000)
/** Msc host request to set/reset remote wakeup feature                      */
#define CSL_MSC_FEATURE_REMOTE_WAKEUP                          (0x0001)

/** \brief  USB MSC status and size definitions                              */
/** MSC verification passed                                                  */
#define CSL_MSC_VERIFY_PASSED                                  (0x0)
/** MSC verification failed                                                  */
#define CSL_MSC_VERIFY_FAILED                                   (0x1)
/** MSC logical unit is removable                                            */
#define CSL_MSC_UNIT_REMOVEABLE                                   (0x1)
/** Current USB device status                                                */
#define CSL_MSC_CURRDEV_STAT                                   (0x01)
/** Current USB interface status                                             */
#define CSL_MSC_CURRINTRF_STAT                                 (0x00)
/** No end point object is available                                         */
#define CSL_MSC_NO_ENDPTOBJECT                                   (5)
/** Device descriptor length in words                                        */
#define CSL_MSC_DEVDESC_LEN                                       (10)
/** Configuration descriptor length in words                                */
#define CSL_MSC_CFGDESC_TOTAL_LEN                               (17)
/** Device qualifier descriptor length in words                              */
#define CSL_MSC_DEVQUALDESC_LEN                                   (6)
/** Configuration descriptor length in words                                 */
#define CSL_MSC_CONFDESC_LEN                                   (6)
/** Interface descriptor length in words                                     */
#define CSL_MSC_MS_INTDESC_LEN                                   (6)
/** Bulk out end point descriptor length in words                            */
#define CSL_MSC_MS_BULKOUT_ENDPTDESC_LEN                       (5)
/** Bulk in  end point descriptor length in words                            */
#define CSL_MSC_MS_BULKIN_ENDPTDESC_LEN                           (5)
/** String descriptor language Id length in words                            */
#define CSL_MSC_STRDESC_LANGID_LEN                             (4)
/** SCSI enquiry data length in words                                        */
#define CSL_MSC_SCSI_INQUIRY_LEN                               (50)
/** Sense data length in words                                               */
#define CSL_MSC_SENSEDATA_LEN                                   (15)
/** Maximum logical unit number                                              */
#define CSL_MSC_LUN_MAX                                           (4)
/** Command block word size in words                                         */
#define CSL_MSC_CBWSIZE                                        (17)
/** Command status word size in words                                        */
#define CSL_MSC_CSWSIZE                                        (8)
/** MSC read capacity                                                        */
#define CSL_MSC_READCAP                                        (6)
/** Control buffer length in words                                           */
#define CSL_MSC_CTRLBUFF_LEN                                   (5)
/** SCSI inquiry configuration length                                        */
#define CSL_MSC_SCSI_INQUIRY_CONF_LEN                           (24)
/** Mass storage call definition macro                                       */
#define CSL_MSC_DEVICE_CLASS_STORAGE                           (0x08)
/** SCSI data protect macro                                                  */
#define CSL_MSC_SCSI_DATA_PROTECT                                (0x0007)

/** \brief  Symbolic constants for USB MSC requests                          */
/** Host request to get status                                               */
#define CSL_MSC_REQUEST_GET_STATUS                               (0x8000)
/** Host request to clear feature                                            */
#define CSL_MSC_REQUEST_CLEAR_FEATURE                              (0x0001)
/** Host request to set feature                                              */
#define CSL_MSC_REQUEST_SET_FEATURE                               (0x0003)
/** Host request to set address                                              */
#define CSL_MSC_REQUEST_SET_ADDRESS                               (0x0005)
/** Host request to get descriptor                                           */
#define CSL_MSC_REQUEST_GET_DESCRIPTOR                           (0x8006)
/** Host request to set descriptor                                           */
#define CSL_MSC_REQUEST_SET_DESCRIPTOR                           (0x0007)
/** Host request to get configuration                                        */
#define CSL_MSC_REQUEST_GET_CONFIGURATION                       (0x8008)
/** Host request to set configuration                                        */
#define CSL_MSC_REQUEST_SET_CONFIGURATION                       (0x0009)
/** Host request to get interface                                            */
#define CSL_MSC_REQUEST_GET_INTERFACE                           (0x800A)
/** Host request to set interface                                            */
#define CSL_MSC_REQUEST_SET_INTERFACE                           (0x000B)
/** Host request for the synchronization frame                               */
#define CSL_MSC_REQUEST_SYNC_FRAME                               (0x800C)
/** Host request for bulk only reset                                         */
#define CSL_MSC_REQUEST_BOT_MSC_RESET                           (0x20FF)
/** Host request to get maximum logical unit number                          */
#define CSL_MSC_REQUEST_GET_MAX_LUN                               (0xA0FE)
/** Base value for host request to get status                                */
#define CSL_MSC_REQUEST_TYPE_BASE                               (0x80)
/** Host request to get device status                                        */
#define CSL_MSC_REQUEST_TYPE_DEVICE_STATUS                       (0x0)
/** Host request to get interface status                                     */
#define CSL_MSC_REQUEST_TYPE_INTERFACE_STATUS                   (0x1)
/** Host request to get end point status                                     */
#define CSL_MSC_REQUEST_TYPE_EP_STATUS                           (0x2)

/** \brief  MSC Status query API return values                               */
/** Return value for USB device suspended case                               */
#define CSL_MSC_DEVICE_SUSPENDED                               (0x01)
/** Return value for USB device not suspended case                           */
#define CSL_MSC_DEVICE_NOT_SUSPENDED                           (0x0)
/** Return value for Logical unit locked case                                */
#define CSL_MSC_LUN_LOCKED                                     (0x01)
/** Return value for Logical unit not locked case                            */
#define CSL_MSC_LUN_NOT_LOCKED                                 (0x0)
/** Return value for activity present case                                   */
#define CSL_MSC_ACTIVITY_PRESENT                               (0x01)
/** Return value for activity not present case                               */
#define CSL_MSC_ACTIVITY_NOT_PRESENT                           (0x0)

/** MSC 16 bit Mask value                                                    */
#define CSL_MSC_16BIT_MASK                                     (0xFFFF)
/** MSC 16 bit shift value                                                   */
#define CSL_MSC_16BIT_SHIFT                                    (16)

/** MSC 24 bit shift value                                                   */
#define CSL_MSC_24BIT_SHIFT                                    (24)

/** MSC 8 bit Mask value                                                     */
#define CSL_MSC_8BIT_MASK                                      (0xFF)
/** MSC 8 bit Mask value                                                     */
#define CSL_MSC_8BIT_HIGH_MASK                                 (0xFF00)
/** MSC 8 bit shift value                                                    */
#define CSL_MSC_8BIT_SHIFT                                     (8)
/** MSC Unicode data size                                                    */
#define CSL_MSC_UNICODE_SIZE                                   (66)

/** Device descriptor length in bytes                                        */
#define CSL_MSC_DEVICE_DESC_DEFLEN                             (18)
/** Default value for element 0 in device descriptor array                   */
#define CSL_MSC_DEVICE_DESC0_DEFVAL                            (0x0000)
/** Default value for element 2 in device descriptor array                   */
#define CSL_MSC_DEVICE_DESC2_DEFVAL                            (0x0200)
/** Default value for element 3 in device descriptor array                   */
#define CSL_MSC_DEVICE_DESC3_DEFVAL                            (0x0000)
/** Default value for element 4 in device descriptor array                   */
#define CSL_MSC_DEVICE_DESC4_DEFVAL                            (0x4000)
/** Default value for element 5 in device descriptor array                   */
#define CSL_MSC_DEVICE_DESC5_DEFVAL                            (0x0451)
/** Default value for element 6 in device descriptor array                   */
#define CSL_MSC_DEVICE_DESC6_DEFVAL                            (0x9010)
/** Default value for element 7 in device descriptor array                   */
#define CSL_MSC_DEVICE_DESC7_DEFVAL                            (0x0001)
/** Default value for element 8 in device descriptor array                   */
#define CSL_MSC_DEVICE_DESC8_DEFVAL                            (0x0201)
/** Default value for element 9 in device descriptor array                   */
#define CSL_MSC_DEVICE_DESC9_DEFVAL                            (0x0103)

/** Device qualifier descriptor length in bytes                              */
#define CSL_MSC_DEVICE_QUAL_DESC_DEFLEN                        (10)
/** Default value for element 0 in device qualifier descriptor array         */
#define CSL_MSC_DEVICE_QUAL_DESC0_DEFVAL                       (0x0000)
/** Default value for element 2 in device qualifier descriptor array         */
#define CSL_MSC_DEVICE_QUAL_DESC2_DEFVAL                       (0x0200)
/** Default value for element 3 in device qualifier descriptor array         */
#define CSL_MSC_DEVICE_QUAL_DESC3_DEFVAL                       (0x0000)
/** Default value for element 4 in device qualifier descriptor array         */
#define CSL_MSC_DEVICE_QUAL_DESC4_DEFVAL                       (0x4000)
/** Default value for element 5 in device qualifier descriptor array         */
#define CSL_MSC_DEVICE_QUAL_DESC5_DEFVAL                       (0x0001)

/** Configuration descriptor length in bytes                                 */
#define CSL_MSC_CONF_DESC_DEFLEN                               (9)
/** Default value for element 0 in configuration descriptor array            */
#define CSL_MSC_CONF_DESC0_DEFVAL                              (0x0000)
/** Default value for element 2 in configuration descriptor array            */
#define CSL_MSC_CONF_DESC2_DEFVAL                              (32)
/** Default value for element 3 in configuration descriptor array            */
#define CSL_MSC_CONF_DESC3_DEFVAL                              (0x0101)
/** Default value for element 4 in configuration descriptor array            */
#define CSL_MSC_CONF_DESC4_DEFVAL                              (0xC004)
/** Default value for element 5 in configuration descriptor array            */
#define CSL_MSC_CONF_DESC5_DEFVAL                              (50)

/** Interface descriptor length in bytes                                     */
#define CSL_MSC_INTERFACE_DESC_DEFLEN                          (9)
/** Default value for element 0 in interface descriptor array                */
#define CSL_MSC_INTERFACE_DESC0_DEFVAL                         (0x0000)
/** Default value for element 2 in interface descriptor array                */
#define CSL_MSC_INTERFACE_DESC2_DEFVAL                         (0x0000)
/** Default value for element 3 in interface descriptor array                */
#define CSL_MSC_INTERFACE_DESC3_DEFVAL  ((CSL_MSC_DEVICE_CLASS_STORAGE<<8) | 2)
/** Default value for element 4 in interface descriptor array                */
#define CSL_MSC_INTERFACE_DESC4_DEFVAL                         (0x5006)
/** Default value for element 5 in interface descriptor array                */
#define CSL_MSC_INTERFACE_DESC5_DEFVAL                         (0x05)

/** Bulk Out end point descriptor length in bytes                            */
#define CSL_MSC_BULKOUTEP_DESC_DEFLEN                          (7)
/** Default value for element 0 in bulk out end point descriptor array       */
#define CSL_MSC_BULKOUTEP_DESC0_DEFVAL                         (0x0000)
/** Default value for element 2 in bulk out end point descriptor array       */
#define CSL_MSC_BULKOUTEP_DESC2_DEFVAL                         (0x0202)
/** Default value for element 3 in bulk out end point descriptor array       */
#define CSL_MSC_BULKOUTEP_DESC3_DEFVAL                         (0x0040)
/** Default value for element 4 in bulk out end point descriptor array       */
#define CSL_MSC_BULKOUTEP_DESC4_DEFVAL                         (0x00)

/** Bulk In end point descriptor length in bytes                             */
#define CSL_MSC_BULKINEP_DESC_DEFLEN                           (7)
/** Default value for element 0 in bulk in end point descriptor array        */
#define CSL_MSC_BULKINEP_DESC0_DEFVAL                          (0x0000)
/** Default value for element 2 in bulk in end point descriptor array        */
#define CSL_MSC_BULKINEP_DESC2_DEFVAL                          (0x0282)
/** Default value for element 3 in bulk in end point descriptor array        */
#define CSL_MSC_BULKINEP_DESC3_DEFVAL                          (0x0040)
/** Default value for element 4 in bulk in end point descriptor array        */
#define CSL_MSC_BULKINEP_DESC4_DEFVAL                          (0x00)

/** Default value for element 0 in highspeed configuration descriptor array  */
#define CSL_MSC_HS_CFG_DESC0_DEFVAL                            (0x0000)
/** Default value for element 2 in highspeed configuration descriptor array  */
#define CSL_MSC_HS_CFG_DESC2_DEFVAL                            (32)
/** Default value for element 3 in highspeed configuration descriptor array  */
#define CSL_MSC_HS_CFG_DESC3_DEFVAL                            (0x0101)
/** Default value for element 4 in highspeed configuration descriptor array  */
#define CSL_MSC_HS_CFG_DESC4_DEFVAL                            (0xC004)
/** Default value for element 5 in highspeed configuration descriptor array  */
#define CSL_MSC_HS_CFG_DESC5_DEFVAL                            (0x0932)
/** Default value for element 6 in highspeed configuration descriptor array  */
#define CSL_MSC_HS_CFG_DESC6_DEFVAL  (CSL_MSC_INTERFACE_DESCRIPTOR_TYPE)
/** Default value for element 7 in highspeed configuration descriptor array  */
#define CSL_MSC_HS_CFG_DESC7_DEFVAL                            (0x0200)
/** Default value for element 8 in highspeed configuration descriptor array  */
#define CSL_MSC_HS_CFG_DESC8_DEFVAL  (0x0600 | CSL_MSC_DEVICE_CLASS_STORAGE)
/** Default value for element 9 in highspeed configuration descriptor array  */
#define CSL_MSC_HS_CFG_DESC9_DEFVAL                            (0x0550)
/** Default value for element 10 in highspeed configuration descriptor array */
#define CSL_MSC_HS_CFG_DESC10_DEFVAL ((CSL_MSC_ENDPOINT_DESCRIPTOR_TYPE<<8) | 7)
/** Default value for element 11 in highspeed configuration descriptor array */
#define CSL_MSC_HS_CFG_DESC11_DEFVAL                           (0x0202)
/** Default value for element 12 in highspeed configuration descriptor array */
#define CSL_MSC_HS_CFG_DESC12_DEFVAL                           (0x0200)
/** Default value for element 13 in highspeed configuration descriptor array */
#define CSL_MSC_HS_CFG_DESC13_DEFVAL                           (0x0700)
/** Default value for element 14 in highspeed configuration descriptor array */
#define CSL_MSC_HS_CFG_DESC14_DEFVAL (0x8100 | CSL_MSC_ENDPOINT_DESCRIPTOR_TYPE)
/** Default value for element 15 in highspeed configuration descriptor array */
#define CSL_MSC_HS_CFG_DESC15_DEFVAL                           (0x0002)
/** Default value for element 16 in highspeed configuration descriptor array */
#define CSL_MSC_HS_CFG_DESC16_DEFVAL                           (0x0002)

/** Default value for element 0 in fullspeed configuration descriptor array  */
#define CSL_MSC_FS_CFG_DESC0_DEFVAL                    (0x0000)
/** Default value for element 2 in fullspeed configuration descriptor array  */
#define CSL_MSC_FS_CFG_DESC2_DEFVAL                    (32)
/** Default value for element 3 in fullspeed configuration descriptor array  */
#define CSL_MSC_FS_CFG_DESC3_DEFVAL                    (0x0101)
/** Default value for element 4 in fullspeed configuration descriptor array  */
#define CSL_MSC_FS_CFG_DESC4_DEFVAL                    (0xC004)
/** Default value for element 5 in fullspeed configuration descriptor array  */
#define CSL_MSC_FS_CFG_DESC5_DEFVAL                    (0x0932)
/** Default value for element 6 in fullspeed configuration descriptor array  */
#define CSL_MSC_FS_CFG_DESC6_DEFVAL  (CSL_MSC_INTERFACE_DESCRIPTOR_TYPE)
/** Default value for element 7 in fullspeed configuration descriptor array  */
#define CSL_MSC_FS_CFG_DESC7_DEFVAL                    (0x0200)
/** Default value for element 8 in fullspeed configuration descriptor array  */
#define CSL_MSC_FS_CFG_DESC8_DEFVAL  (0x0600 | CSL_MSC_DEVICE_CLASS_STORAGE)
/** Default value for element 9 in fullspeed configuration descriptor array  */
#define CSL_MSC_FS_CFG_DESC9_DEFVAL                    (0x0550)
/** Default value for element 10 in fullspeed configuration descriptor array */
#define CSL_MSC_FS_CFG_DESC10_DEFVAL ((CSL_MSC_ENDPOINT_DESCRIPTOR_TYPE<<8) | 7)
/** Default value for element 11 in fullspeed configuration descriptor array */
#define CSL_MSC_FS_CFG_DESC11_DEFVAL                   (0x0202)
/** Default value for element 12 in fullspeed configuration descriptor array */
#define CSL_MSC_FS_CFG_DESC12_DEFVAL                   (0x0040)
/** Default value for element 13 in fullspeed configuration descriptor array */
#define CSL_MSC_FS_CFG_DESC13_DEFVAL                   (0x0700)
/** Default value for element 14 in fullspeed configuration descriptor array */
#define CSL_MSC_FS_CFG_DESC14_DEFVAL (0x8100 | CSL_MSC_ENDPOINT_DESCRIPTOR_TYPE)
/** Default value for element 15 in fullspeed configuration descriptor array */
#define CSL_MSC_FS_CFG_DESC15_DEFVAL                   (0x4002)
/** Default value for element 16 in fullspeed configuration descriptor array */
#define CSL_MSC_FS_CFG_DESC16_DEFVAL                   (0x0000)

/** String descriptor language Id length in bytes                            */
#define CSL_MSC_LANGID_DEFLEN                                  (4)
/** Default value for element 0 in string descriptor language Id array       */
#define CSL_MSC_LANGID0_DEFVAL                                 (0x0000)
/** Default value for element 2 in string descriptor language Id array       */
#define CSL_MSC_LANGID2_DEFVAL                                 (0x0409)
/** Default value for element 3 in string descriptor language Id array       */
#define CSL_MSC_LANGID3_DEFVAL                                 (0x0000)

/** Default value for element 0 in sense data array                          */
#define CSL_MSC_SENSE_DATA0_DEFVAL                             (26)
/** Default value for element 1 in sense data array                          */
#define CSL_MSC_SENSE_DATA1_DEFVAL                             (0)
/** Default value for element 2 in sense data array                          */
#define CSL_MSC_SENSE_DATA2_DEFVAL                             (0x0070)
/** Default value for element 3 in sense data array                          */
#define CSL_MSC_SENSE_DATA3_DEFVAL                             (0x0000)
/** Default value for element 4 in sense data array                          */
#define CSL_MSC_SENSE_DATA4_DEFVAL                             (0x0000)
/** Default value for element 5 in sense data array                          */
#define CSL_MSC_SENSE_DATA5_DEFVAL                             ((11<<8) | 0)

/** Default value for element 0 in mode sense data array                     */
#define CSL_MSC_MODESENSE_DATA0_DEFVAL                         (0x08)
/** Default value for element 1 in mode sense data array                     */
#define CSL_MSC_MODESENSE_DATA1_DEFVAL                         (0x0)
/** Default value for element 2 in mode sense data array                     */
#define CSL_MSC_MODESENSE_DATA2_DEFVAL                         (0x03)
/** Default value for element 3 in mode sense data array                     */
#define CSL_MSC_MODESENSE_DATA3_DEFVAL                         (0x0)
/** Default value for element 4 in mode sense data array                     */
#define CSL_MSC_MODESENSE_DATA4_DEFVAL                         (0x0)


/** Read capacity 0 default value                                            */
#define CSL_MSC_READ_CAPACITY0_DEFVAL                          (8)

/** Command status word 0 default value                                      */
#define CSL_MSC_CSW0_DEFVAL                                    (0)
/** Command status word 1 default value                                      */
#define CSL_MSC_CSW1_DEFVAL                                    (0x5355)
/** Command status word 2 default value                                      */
#define CSL_MSC_CSW2_DEFVAL                                    (0x5342)

/** \brief  MSC Storage state macros                                         */
/** Storage state set to reset                                               */
#define CSL_MSC_STORAGE_STATE_RESET                               (0)
/** Storage state set to wait for Command Block Word                         */
#define CSL_MSC_STORAGE_STATE_WAIT_FOR_CBW                     (1)
/** Storage state set to sending stall signal due to some error              */
#define CSL_MSC_STORAGE_STATE_SENDING_STALL                    (2)
/** Storage state set to sending data                                        */
#define CSL_MSC_STORAGE_STATE_SENDING_DATA                     (3)
/** Storage state set to sending CSW                                         */
#define CSL_MSC_STORAGE_STATE_SENDING_CSW                      (4)
/** Storage state set to read                                                */
#define CSL_MSC_STORAGE_STATE_READ                               (5)
/** Storage state set to write                                               */
#define CSL_MSC_STORAGE_STATE_WRITE                               (6)
/** Storage state set to sending short packet                                */
#define CSL_MSC_STORAGE_STATE_SENDING_SHORT_PKT                (7)

/** State reset data size                                                    */
#define CSL_MSC_STATE_RESET_DATA_SIZE                          (31)
/** Command Block Word length in bytes                                       */
#define CSL_MSC_CBW_DATA_SIZE                                  (31)
/** Command Status Word length in bytes                                      */
#define CSL_MSC_CSW_DATA_SIZE                                  (13)

/** \brief  MSC SCSI commands                                                */
/** SCSI test unit ready command                                             */
#define CSL_MSC_SCSI_COMMAND_TEST_UNIT_READY                   (0x00)
/** SCSI request sense command                                               */
#define CSL_MSC_SCSI_COMMAND_REQUEST_SENSE                       (0x03)
/** SCSI format the logical unit command                                     */
#define CSL_MSC_SCSI_COMMAND_FORMAT_UNIT                       (0x04)
/** SCSI data inquiry command                                                */
#define CSL_MSC_SCSI_COMMAND_INQUIRY                           (0x12)
/** SCSI mode select 6 command                                               */
#define CSL_MSC_SCSI_COMMAND_MODE_SELECT_6                       (0x15)
/** SCSI reserve 6 command                                                   */
#define CSL_MSC_SCSI_COMMAND_RESERVE_6                           (0x16)
/** SCSI release 6 command                                                   */
#define CSL_MSC_SCSI_COMMAND_RELEASE_6                           (0x17)
/** SCSI mode sense 6 command                                                */
#define CSL_MSC_SCSI_COMMAND_MODE_SENSE_6                       (0x1A)
/** SCSI mode sense 10 command                                               */
#define CSL_MSC_SCSI_COMMAND_MODE_SENSE_10                       (0x5A)
/** SCSI start/stop logical unit command                                     */
#define CSL_MSC_SCSI_COMMAND_START_STOP_UNIT                   (0x1B)
/** SCSI prevent/allow media removal command                                 */
#define CSL_MSC_SCSI_COMMAND_PREVENT_ALLOW_MEDIA_REMOVAL       (0x1E)
/** SCSI read capacity command                                               */
#define CSL_MSC_SCSI_COMMAND_READ_CAPACITY                       (0x25)
/** SCSI read 10 command                                                     */
#define CSL_MSC_SCSI_COMMAND_READ_10                           (0x28)
/** SCSI write 10 command                                                    */
#define CSL_MSC_SCSI_COMMAND_WRITE_10                           (0x2A)
/** SCSI verify 10 command                                                   */
#define CSL_MSC_SCSI_COMMAND_VERIFY_10                           (0x2F)
/** SCSI synchronize cache command                                           */
#define CSL_MSC_SCSI_COMMAND_SYNCHRONIZE_CACHE                   (0x35)
/** SCSI write buffer command                                                */
#define CSL_MSC_SCSI_COMMAND_WRITE_BUFFER                       (0x3B)
/** SCSI persistent reserve in command                                       */
#define CSL_MSC_SCSI_COMMAND_PERSISTENT_RESERVE_IN               (0x5E)
/** SCSI persistent reserve out command                                      */
#define CSL_MSC_SCSI_COMMAND_PERSISTENT_RESERVE_OUT               (0x5F)

/** \brief  MSC SCSI sense keys                                              */
/** SCSI no sense data                                                       */
#define CSL_MSC_SCSI_SENSEKEY_NO_SENSE                           (0x00)
/** SCSI sense key error recovered                                           */
#define CSL_MSC_SCSI_SENSEKEY_RECOVERED_ERROR                   (0x01)
/** SCSI sense key not ready                                                 */
#define CSL_MSC_SCSI_SENSEKEY_NOT_READY                           (0x02)
/** SCSI sense key media error has occurred                                  */
#define CSL_MSC_SCSI_SENSEKEY_MEDIA_ERROR                       (0x03)
/** SCSI sense key hardware error has occurred                               */
#define CSL_MSC_SCSI_SENSEKEY_HARDWARE_ERROR                   (0x04)
/** SCSI sense key illegal request                                           */
#define CSL_MSC_SCSI_SENSEKEY_ILLEGAL_REQUEST                   (0x05)
/** SCSI sense key unit attention                                            */
#define CSL_MSC_SCSI_SENSEKEY_UNIT_ATTENTION                   (0x06)
/** SCSI sense key data protect                                              */
#define CSL_MSC_SCSI_SENSEKEY_DATA_PROTECT                       (0x07)
/** SCSI sense key blank check                                               */
#define CSL_MSC_SCSI_SENSEKEY_BLANK_CHECK                       (0x08)
/** SCSI sense key vendor specific data                                      */
#define CSL_MSC_SCSI_SENSEKEY_VENDOR_SPECIFIC                   (0x09)
/** SCSI sense key copy aborted                                              */
#define CSL_MSC_SCSI_SENSEKEY_COPY_ABORTED                       (0x0A)
/** SCSI sense key aborted command                                           */
#define CSL_MSC_SCSI_SENSEKEY_ABORTED_COMMAND                   (0x0B)
/** SCSI sense key copy equal                                                */
#define CSL_MSC_SCSI_SENSEKEY_EQUAL                               (0x0C)
/** SCSI sense key volume overflow                                           */
#define CSL_MSC_SCSI_SENSEKEY_VOLUME_OVERFLOW                   (0x0D)
/** SCSI sense key data miss compare                                         */
#define CSL_MSC_SCSI_SENSEKEY_MISCOMPARE                       (0x0E)
/** SCSI sense key reserved                                                  */
#define CSL_MSC_SCSI_SENSEKEY_RESERVED                           (0x0F)
/** SCSI sense key verify data miss compare                                  */
#define CSL_MSC_SCSI_ASC_MISCOMPARE_VERIFY                        (0x001D)

/** \brief  MSC SCSI additional sense keys                                   */
/** SCSI no additional sense information                                     */
#define CSL_MSC_SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION       (0x0000)
/** SCSI sense key media not present                                         */
#define CSL_MSC_SCSI_ASC_MEDIA_NOT_PRESENT                       (0x003A)
/** SCSI sense key invalid command operation code                            */
#define CSL_MSC_SCSI_ASC_INVALID_COMMAND_OPERATION_CODE           (0x0020)
/** SCSI sense key media may have changed                                    */
#define CSL_MSC_SCSI_ASC_MEDIA_MAY_HAVE_CHANGED               (0x0028)
/** SCSI sense key manual intervention is required for media                 */
#define CSL_MSC_SCSI_ASC_MEDIA_MANUALINTERVENTION_REQUIRED    (0x0004)
/** SCSI sense key media out of range                                        */
#define CSL_MSC_SCSI_ASC_MEDIA_OUTOFRANGE                        (0x0021)
/** SCSI sense key media write protected                                     */
#define CSL_MSC_SCSI_ASC_WRITE_PROTECTED                         (0x0027)
/** SCSI sense key media removal prevented                                   */
#define CSL_MSC_SCSI_ASC_MEDIA_REMOVAL_PREVENTED                 (0x0253)

/** \brief  MSC CSW status macros                                            */
/** CSW status command passed                                                */
#define CSL_MSC_CSW_STATUS_COMMAND_PASSED                       (0x00)
/** CSW status command failed                                                */
#define CSL_MSC_CSW_STATUS_COMMAND_FAILED                       (0x01)
/** CSW status phase error                                                   */
#define CSL_MSC_CSW_STATUS_PHASE_ERROR                           (0x02)

/** Read capacity word 1                                                     */
#define CSL_MSC_RC_1                                           (2)
/** Read capacity word 2                                                     */
#define CSL_MSC_RC_2                                           (3)
/** Read capacity word 3                                                     */
#define CSL_MSC_RC_3                                           (4)
/** Read capacity word 4                                                     */
#define CSL_MSC_RC_4                                           (5)

/** Sense key data word 2                                                    */
#define CSL_MSC_SSD_2                                          (3)
/** Sense key data word 7                                                    */
#define CSL_MSC_SSD_7                                          (8)

/** SCSI data array element 0 default value                                  */
#define CSL_SCSI_DATA0_DEFVAL                                  (96)
/** SCSI data array element 1 default value                                  */
#define CSL_SCSI_DATA1_DEFVAL                                  (0x0)
/** SCSI data array element 2 default value                                  */
#define CSL_SCSI_DATA2_DEFVAL                                  (0x8000)
/** SCSI data array element 3 default value                                  */
#define CSL_SCSI_DATA3_DEFVAL                                  (0x0200)
/** SCSI data array element 4 default value                                  */
#define CSL_SCSI_DATA4_DEFVAL                                  (92)
/** SCSI data array element 5 default value                                  */
#define CSL_SCSI_DATA5_DEFVAL                                  (0x0000)
/** SCSI data array element 30 default value                                 */
#define CSL_SCSI_DATA30_DEFVAL                                 (0x0000)
/** SCSI data array element 31 default value                                 */
#define CSL_SCSI_DATA31_DEFVAL                                 (0x2002)
/** SCSI data array element 32 default value                                 */
#define CSL_SCSI_DATA32_DEFVAL                                 (0x3802)
/** SCSI data array element NULL value                                       */
#define CSL_SCSI_DATA_NULL_VAL                                 (0x0000)
/** SCSI data index size 1 (Used for  array initialization in for loop)      */
#define CSL_SCSI_DATA_SIZE1                                    (6)
/** SCSI data index size 2 (Used for  array initialization in for loop)      */
#define CSL_SCSI_DATA_SIZE2                                    (30)
/** SCSI data index size 3 (Used for  array initialization in for loop)      */
#define CSL_SCSI_DATA_SIZE3                                    (33)
/** SCSI data index size 4 (Used for  array initialization in for loop)      */
#define CSL_SCSI_DATA_SIZE4                                    (50)

/** Error Code base                                                          */
#define CSL_MSC_ERROR_BASE                             (CSL_EUSB_FIRST)
/** Returned when the MSC request handler is invalid                         */
#define CSL_MSC_INVALID_REQUEST_HANDLER                (CSL_MSC_ERROR_BASE - 1)


/** \brief MSC Request arguments                                              */
#define CSL_MSC_REQUEST_ARGS     CSL_UsbDevNum         devNum,           \
                                 CSL_UsbSetupStruct    *usbSetup,        \
                                 pUsbEpHandle          hInEp,            \
                                 pUsbEpHandle          hOutEp,           \
                                 void                  *pMsc

/**
@} */


/**************************************************************************\
* USB MSC global typedef declarations                                      *
\**************************************************************************/

/** @addtogroup CSL_MSC_ENUM
 @{ */

/**
 *  \brief This Enum defines the Msc request return values
 */
typedef enum {
    /* Don't call request again until new SETUP                              */
    CSL_MSC_REQUEST_DONE = 0,
    /* STALL the control endpoint                                            */
    CSL_MSC_REQUEST_STALL,
    /* Send a 0 length IN packet                                             */
    CSL_MSC_REQUEST_SEND_ACK,
    /* Prepare to receive 0 length OUT packet                                */
    CSL_MSC_REQUEST_GET_ACK,
    /* Notify handler when IN data has been transmitted                      */
    CSL_MSC_REQUEST_DATA_IN,
    /* Notify handler when OUT data has been received                        */
    CSL_MSC_REQUEST_DATA_OUT
} CSL_MscRequestRet;

/**
 *  \brief This Enum defines the Msc media access status
 */
typedef enum {
    /** Media over flow error                                                */
    CSL_MSC_MEDIACCESS_OVERFLOW,
    /** Media Access success                                                 */
    CSL_MSC_MEDIACCESS_SUCCESS,
    /** Media not present on the system                                      */
    CSL_MSC_MEDIACCESS_NOTPRESENT,
    /** Media is write protected                                             */
    CSL_MSC_MEDIACCESS_WRITEPROTECT,
    /** Media is found as bad                                                */
    CSL_MSC_MEDIACCESS_BADMEDIA
} CSL_MscMediaStatus;

/**
 *  \brief This Enum defines the Msc media lock status
 */
typedef enum {
    /** Lock the media                                                       */
    CSL_MSC_UNLOCK = 0,
    /** Unlock the media                                                     */
    CSL_MSC_LOCK
} CSL_MscMediaLockStatus;

/**
@} */


/**************************************************************************\
* USB MSC global data structure declarations                               *
\**************************************************************************/

/**
\addtogroup CSL_MSC_DATASTRUCT
@{ */

/**
 *  \brief MSC control request structure
 *
 *  This structure holds the mapping of the Control Request to the
 *  Control Request handler
 */
typedef struct CSL_MscRequestStruct {
    /** \brief  MSC control request                                          */
    WORD                 request;
    /** \brief  MSC control request handler pointer                          */
    CSL_MscRequestRet    (*fpRequestHandler)(CSL_MSC_REQUEST_ARGS);
} CSL_MscRequestStruct;

/**
 *  \brief MSC application data structure
 *
 *  Holds the structure used by the application to fill in
 *  application specific data handler
 */
typedef struct CSL_MscInitStructApp {
    /** \brief  Pointer to MSC Class Handle defined in MSC Module            */
    void                    *pMscObj;
    /** \brief  Device Descriptor units                                           */
    Uint16                  pId;
    /** \brief  Device Descriptor units                                          */
    Uint16                  vId;
    /** \brief  Logical unit number                                              */
    Uint16                  numLun;
    /** \brief  User specific string descriptor pointer                          */
    char                    **strDescrApp;
    /** \brief  Buffer ptr passed by the system to the module in order to
                enable data transfer to/from the media/USB API layer.        */
    Uint16                  *lbaBufferApp;
    /** \brief  Pointer to the request Table which is specified by user      */
    CSL_MscRequestStruct    *mscReqTableApp;
    /** \brief  Pointer to the Bulk Function Handler which is
                needed during Configuration of the Bulk Endpoint             */
    void                    (*bulkHandler)();
    /** \brief  Pointer to the Control Function Handler which is
                needed during Configuration of the Control Endpoint          */
    void                    (*ctrlHandler)();
    /** \brief  Function for Media Read                                          */
    CSL_MscMediaStatus      (*readMediaApp)(Uint16, Uint16*, Uint32, Uint16);
    /** \brief  Function for Media Write                                     */
    CSL_MscMediaStatus      (*writeMediaApp)(Uint16, Uint16*, Uint32, Uint16);
    /** \brief  Function checks for Media Present    etc                       */
    CSL_MscMediaStatus      (*mediaGetPresentStateApp)(Uint16);
    /** \brief  Function Handler for Init Media                                  */
    CSL_MscMediaStatus      (*mediaInitApp)(Uint16);
    /** \brief  Function to eject Media                                          */
    CSL_MscMediaStatus      (*mediaEjectApp)(Uint16);
    /** \brief  Function to lock Media                                         */
    CSL_MscMediaStatus      (*mediaLockUnitApp)(Uint16,CSL_MscMediaLockStatus);
    /** \brief  Function to get the size of the media                         */
    Uint32                  (*getMediaSizeApp)(Uint16);
} CSL_MscInitStructApp;

/** \brief  MSc application class handle                                     */
typedef CSL_MscInitStructApp  *pMscAppClassHandle;

/**
 *  \brief MSC application Logical unit attribute structure
 *
 *  Holds the attributes of every logical unit. This structure
 *  should be filled by the application.
 */
typedef struct CSL_MscLunAttribApp {
    /** \brief  Logical Block Size (size of each LB)                         */
    Uint32    lbaSizeApp;
    /** \brief  Total Number Of LB in the Media                                  */
    Uint32    mediaSizeApp;
    /** \brief  Removable or Non removable Media                             */
    Uint16    removeableApp;
    /** \brief  SCSI Inquiry Data  - User Configurable Items                 */
    Uint16    scsiInquiryConfData [CSL_MSC_SCSI_INQUIRY_CONF_LEN];
    /** \brief  Media State                                                      */
    Uint16    mediaStateApp;
    /** \brief  Device is Readable and/or Writable                             */
    Uint16    readwriteApp;
} CSL_MscLunAttribApp;

/** \brief  MSc application Lun attribute handle                             */
typedef CSL_MscLunAttribApp   *pMscAppLunAttribHandle;

/** \brief  MSC request handler function pointer                             */
typedef  CSL_MscRequestRet (*fpMSC_REQ_HANDLER)(CSL_MSC_REQUEST_ARGS);

/**
 *  \brief MSC Logical unit structure
 *
 *  Holds all the components for the logical unit
 *  This structure is filled with the data given by the application
 *  through CSL_MscLunAttribApp structure
 */
typedef struct CSL_MscLogicalUnit {
    /** \brief  SCSI Inquiry Data                                            */
    Uint16    scsiInquiryData[CSL_MSC_SCSI_INQUIRY_LEN];
    /** \brief  Number of logical blocks in the media
      *  During init this filed is set to value -1                           */
    Uint32    mediaSize;
    /** \brief  the current LBA                                              */
    Uint32    currLba;
    /** \brief  Logical Block Size                                           */
    Uint32    lbaSize;
    /** \brief  Removable or NonRemovable Media                              */
    Uint16    removeable;
    /** \brief  Media State                                                  */
    Uint16    mediaState;
    /** \brief  Device is Readable and/or writeable                          */
    Uint16    readwrite;
    /** \brief  Contains number of LB that has to be read/written            */
    Uint16    lbaCnt;
    /** \brief  Contains status of previous write in this variable           */
    Uint16    verifyFlag;
} CSL_MscLogicalUnit;

/**
 *  \brief MSC Mass Storage Object structure
 *
 *  Holds all the components for the MSC Mass Storage Object
 */
typedef struct CSL_MscObject {
    /** \brief  Function to Read from the Media                              */
    CSL_MscMediaStatus    (*readMedia)(Uint16 , Uint16* , Uint32, Uint16 );
    /** \brief  Function to write to the Media                               */
    CSL_MscMediaStatus    (*writeMedia)(Uint16 , Uint16* , Uint32, Uint16 );
    /** \brief  Function to know the status of Media - Present or Not        */
    CSL_MscMediaStatus    (*mediaGetPresentState)(Uint16 );
    /** \brief  Function to initialize the Media                             */
    CSL_MscMediaStatus    (*mediaInit)(Uint16 );
    /** \brief  Function to Eject Media                                      */
    CSL_MscMediaStatus    (*mediaEject)(Uint16 );
    /** \brief  Function to Lock Media                                       */
    CSL_MscMediaStatus    (*mediaLockUnit)(Uint16,CSL_MscMediaLockStatus);
    /** \brief  Function to get the size of the media                        */
    Uint32                (*getMediaSize)(Uint16 );
    /** \brief  Data buffer pointer used to transfer data to/from Media/usb  */
    Uint16                *lbaBuffer;
    /** \brief  CBW transfer length                                          */
    Uint32                cbwDataTransferLength;
     /** \brief  Data pertaining to Logical Units Supported                   */
    CSL_MscLogicalUnit    lun[CSL_MSC_LUN_MAX];
    /** \brief  Sense Data Array                                             */
    Uint16                senseData[CSL_MSC_SENSEDATA_LEN];
    /** \brief  Flag to Indicate whether any Media has been Accessed         */
    Uint16                activityPresentFlag;
    /** \brief  Array used to store the elements of CSW                      */
    Uint16                csw[CSL_MSC_CSWSIZE];
    /** \brief  Array used to store Read Capacity                            */
    Uint16                readCapacity[CSL_MSC_READCAP];
    /** \brief  Array used to store the elements of CBW                      */
    Uint16                cbw[CSL_MSC_CBWSIZE];
    /** \brief  Bulk In Endpoint Object                                      */
    CSL_UsbEpObj          bulkInEpObj;
    /** \brief  Bulk Out Endpoint Object                                     */
    CSL_UsbEpObj          bulkOutEpObj;
    /** \brief  Maximum No.of Logical Units                                  */
    Uint16                noOfLun;
    /** \brief  The State the Class is in                                    */
    Uint16                storageState;
    /** \brief  Buffer to hold Mode Sense request data                       */
    Uint16                modeSenseData[5];
    /** \brief  Flag to indicate whether the write media is required or not   */
    Bool                  writeToMedia;
} CSL_MscObject;

/**
 *  \brief MSC Control Object structure
 *
 *  Holds all the components for the MSC Control Object
 */
typedef struct CSL_MscCtrlObject {
    /** \brief  Control In Endpoint Object                                   */
    CSL_UsbEpObj             ctrlInEpObj;
    /** \brief  Object of Type Device Number                                 */
    CSL_UsbDevNum            devNum;
    /** \brief  Control Out Endpoint Object                                  */
    CSL_UsbEpObj             ctrlOutEpObj;
    /** \brief  Flag Used internally by APIs                                 */
      Uint16                   suspendFlag;
      /** \brief  NULL terminated array of Endpoint Objects                    */
    pUsbEpHandle             hEpObjArray[CSL_MSC_NO_ENDPTOBJECT];
    /** \brief  Setup Structure                                              */
    CSL_UsbSetupStruct       usbSetup;
    /** \brief  Dummy descriptor array                                       */
    Uint16                   highSpeedCfgDescr[CSL_MSC_CFGDESC_TOTAL_LEN];
    /** \brief  other speed configuration descriptor array                   */
    Uint16                   fullSpeedCfgDescr[CSL_MSC_CFGDESC_TOTAL_LEN];
    /** \brief  Device descriptor                                            */
    Uint16                   deviceDescr[CSL_MSC_DEVDESC_LEN];
    /** \brief  Device qualifier descriptor                                  */
    Uint16                   deviceQualDescr[CSL_MSC_DEVQUALDESC_LEN];
    /** \brief  Configuration descriptor                                     */
    Uint16                   confDescr[CSL_MSC_CONFDESC_LEN];
    /** \brief  Mass storage Interface descriptor                            */
    Uint16                   massStorageIntDescr[CSL_MSC_MS_INTDESC_LEN];
    /** \brief  Mass Storage Bulk Out Endpoint Descriptor                    */
    Uint16 massStorageBulkOutEndptDescr[CSL_MSC_MS_BULKOUT_ENDPTDESC_LEN];
    /** \brief  Mass Storage Bulk In Endpoint Link                           */
    CSL_UsbDataStruct        massStorageInterfcLnk;
    /** \brief  Mass Storage Bulk In Endpoint Descriptor                     */
    Uint16 massStorageBulkInEndptDescr[CSL_MSC_MS_BULKIN_ENDPTDESC_LEN];
    /** \brief  Mass Storage Bulk Out Endpoint Link                          */
    CSL_UsbDataStruct        massStorageBulkOutEndptLnk;
    /** \brief  String Descriptor Language ID                                */
    Uint16                   stringDescrLangid[CSL_MSC_STRDESC_LANGID_LEN];
    /** \brief  Current Device Configuration Status                          */
    Uint16                   curConfigStat;
    /** \brief  Mass Storage Bulk In Endpoint Link                           */
    CSL_UsbDataStruct        massStorageBulkInEndptLnk;
    /** \brief  Array used for General Purpose                               */
    Uint16                   ctrlBuffer[CSL_MSC_CTRLBUFF_LEN];
    /** \brief  Configuration Descriptor Link                                */
    CSL_UsbDataStruct        configDescrLnk;
    /** \brief  String descriptor pointer                                    */
    char                     **strDescr;
    /** \brief  Pointer to the request Table                                 */
    CSL_MscRequestStruct     *mscReqTable;
    /** \brief  Request handler function pointer                             */
    CSL_MscRequestRet        (*fpRequestHandler)(CSL_MSC_REQUEST_ARGS);
    /** \brief  USB speed configuration
                usbSpeedCfg - 1; High speed
                usbSpeedCfg - 0; Full speed                                  */
    Bool                     usbSpeedCfg;
    /** \brief  Pointer to hold the configuration descriptor for the speed
                under operation                                              */
    Uint16                   *pCfgDescr;
    /** \brief  Pointer to hold the configuration descriptor for the other
                speed which is supported by the device                       */
    Uint16                   *pOtherSpeedCfgDescr;
} CSL_MscCtrlObject;

/**
 *  \brief MSC Control Object structure
 *
 *  Holds all the components for the MSC Control Object
 */
typedef struct CSL_MscClassStruct {
    /** \brief  Handle to Control Object                                      */
    CSL_MscCtrlObject    ctrlHandle;
    /** \brief  Handle to Bulk Transfer Object                                */
    CSL_MscObject        mscHandle;
} CSL_MscClassStruct;

/** \brief  MSc class handle                                                  */
typedef CSL_MscClassStruct  *pMscClassHandle;

/**
@} */

/**************************************************************************\
* USB MSC function declarations                                            *
\**************************************************************************/

/** @addtogroup CSL_MSC_FUNCTION
 @{ */

/** ============================================================================
 *   @n@b MSC_Open
 *
 *   @b Description
 *   @n This function Opens the Msc module. It initializes Msc and Ctrl handles
 *      with the data given by the application.
 *
 *      NOTE: THIS FUNCTION CONFIGURES USB MSC MODULE IN HIGH SPEED MODE
 *
 *   @b Arguments
 *   @verbatim
            pAppClassHandle      Application handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li        CSL_SOK                   - Returned for success
 *   @li        CSL_ESYS_BADHANDLE        - Invalid Msc handle
 *   @li        CSL_ESYS_BADMEDIA         - Media is Bad - Format needed
 *   @li        CSL_ESYS_MEDIA_NOTPRESENT - Media is not present
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes Msc and Ctrl handles
 *
 *   @b Modifies
 *   @n Msc and Ctrl handle data
 *
 *   @b Example
 *   @verbatim
            CSL_MscInitStructApp    mscAppHandle;
            pMscAppClassHandle      pAppClassHandle;
            Uint16                  mscAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &mscAppHandle;

            // Assign values to all the members in mscAppHandle struct
            mscAppHandle.pMscObj = &mscAppBuffer[0];
            .....
            .....
            status = MSC_Open(pAppClassHandle);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_Open(pMscAppClassHandle    pAppClassHandle);

/** ============================================================================
 *   @n@b MSC_OpenFullspeed
 *
 *   @b Description
 *   @n This function Opens the Msc module for fullspeed mode operation. It
 *      initializes Msc and Ctrl handles with the data given by the application
 *
 *      NOTE: THIS FUNCTION CONFIGURES USB MSC MODULE IN FULLSPEED MODE
 *
 *   @b Arguments
 *   @verbatim
            pAppClassHandle      Application handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li        CSL_SOK                   - Returned for success
 *   @li        CSL_ESYS_BADHANDLE        - Invalid Msc handle
 *   @li        CSL_ESYS_BADMEDIA         - Media is Bad - Format needed
 *   @li        CSL_ESYS_MEDIA_NOTPRESENT - Media is not present
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes Msc and Ctrl handles
 *
 *   @b Modifies
 *   @n Msc and Ctrl handle data
 *
 *   @b Example
 *   @verbatim
            CSL_MscInitStructApp    mscAppHandle;
            pMscAppClassHandle      pAppClassHandle;
            Uint16                  mscAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &mscAppHandle;

            // Assign values to all the members in mscAppHandle struct
            mscAppHandle.pMscObj = &mscAppBuffer[0];
            .....
            .....
            status = MSC_OpenFullspeed(pAppClassHandle);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_OpenFullspeed(pMscAppClassHandle    pAppClassHandle);

/** ============================================================================
 *   @n@b MSC_Setup
 *
 *   @b Description
 *   @n This function used to setup the Msc and Ctrl handles. This is an internal
 *      function and is called from MSC_Open or MSC_OpenFullspeed functions.
 *
 *   @b Arguments
 *   @verbatim
            pAppClassHandle      Application handle to the MSC Class Obj
            usbSpeedCfg          USB speed selection parameter
                                 0 - USB MSC operates in fullspeed mode
                                 1 - USB MSC operates in highspeed mode
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li        CSL_SOK                   - Returned for success
 *   @li        CSL_ESYS_BADHANDLE        - Invalid Msc handle
 *   @li        CSL_ESYS_BADMEDIA         - Media is Bad - Format needed
 *   @li        CSL_ESYS_MEDIA_NOTPRESENT - Media is not present
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes Msc and Ctrl handles
 *
 *   @b Modifies
 *   @n Msc and Ctrl handle data
 *
 *   @b Example
 *   @verbatim
            CSL_MscInitStructApp    mscAppHandle;
            pMscAppClassHandle      pAppClassHandle;
            Uint16                  mscAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &mscAppHandle;

            // Assign values to all the members in mscAppHandle struct
            mscAppHandle.pMscObj = &mscAppBuffer[0];
            .....
            .....
            // Setup USB for highspeed
            status = MSC_Setup(pAppClassHandle, TRUE);
     @endverbatim
 *  ============================================================================
 */
static CSL_Status MSC_Setup(pMscAppClassHandle    pAppClassHandle,
                            Bool                  usbSpeedCfg);

/** ============================================================================
 *   @n@b MSC_Close
 *
 *   @b Description
 *   @n This function Opens the Msc module.
 *
 *   @b Arguments
 *   @verbatim
            pAppClassHandle       Application handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK              - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE   - Invalid Msc class handle
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Closes the Msc module
 *
 *   @b Modifies
 *   @n Msc and Ctl handles
 *
 *   @b Example
 *   @verbatim
            CSL_MscInitStructApp    mscAppHandle;
            pMscAppClassHandle      pAppClassHandle;
            Uint16                  mscAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &mscAppHandle;

            // Assign values to all the members in mscAppHandle struct
            mscAppHandle.pMscObj = &mscAppBuffer[0];
            .....
            .....
            status = MSC_Open(pAppClassHandle);
            ....
            ....
            status = MSC_Close(pAppClassHandle);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_Close(pMscAppClassHandle    pAppClassHandle);

/** ============================================================================
 *   @n@b MSC_SetLunAttr
 *
 *   @b Description
 *   @n This function initializes the logical unit data structures.
 *
 *   @b Arguments
 *   @verbatim
            pAppClassHandle      Handle of the MSC User Application Object
            lunHandle            Handle to the application Lun Object
            lunNum               Logical Unit Number
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                    CSL_SOK              - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE   - Invalid Msc class handle
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Initializes the logical unit
 *
 *   @b Modifies
 *   @n Logical unit data structures
 *
 *   @b Example
 *   @verbatim
            CSL_MscInitStructApp    mscAppHandle;
            pMscAppClassHandle      pAppClassHandle;
            Uint16                  mscAppBuffer[420];
            CSL_Status              status;
            CSL_MscLunAttribApp     lunObj;

            pAppClassHandle = &mscAppHandle;

            // Assign values to all the members in mscAppHandle struct
            mscAppHandle.pMscObj = &mscAppBuffer[0];
            .....
            .....
            status = MSC_Open(pAppClassHandle);
            ....
            ....
            //Assign values to Lun Object structure
            lunObj.removeableApp = 0;
            lunObj.lbaSizeApp    = 512;
            ....
            ....
            status = MSC_SetLunAttr(&mscAppHandle, &lunObj, 0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_SetLunAttr(pMscAppClassHandle      pAppClassHandle,
                          CSL_MscLunAttribApp     *lunHandle,
                          Uint16                  lunNum);

/** ============================================================================
 *   @n@b MSC_Ctrl
 *
 *   @b Description
 *   @n This function handles all USB traffic on the Control Endpoint.
 *      Detects the events on the Control Endpoint and takes
 *      the appropriate action
 *
 *   @b Arguments
 *   @verbatim
            pMSC        Handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li       CSL_SOK                          - Returned for success
 *   @li       CSL_ESYS_BADHANDLE               - Invalid Msc class handle
 *   @li       CSL_MSC_INVALID_REQUEST_HANDLER  - Invalid request class handler
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Control end point events are serviced
 *
 *   @b Modifies
 *   @n USB Registers.
 *
 *   @b Example
 *   @verbatim
            CSL_MscInitStructApp    mscAppHandle;
            pMscAppClassHandle      pAppClassHandle;
            Uint16                  mscAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &mscAppHandle;

            // Assign values to all the members in mscAppHandle struct
            mscAppHandle.pMscObj = &mscAppBuffer[0];
            .....
            .....
            status = MSC_Open(pAppClassHandle);
            ....
            ....
            status = MSC_Ctrl(pAppClassHandle->pMscObj);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_Ctrl(void    *pMSC);

/** ============================================================================
 *   @n@b MSC_Bulk
 *
 *   @b Description
 *   @n This function handles all UBS traffic on the Bulk Endpoint
 *      This maintains the state of the MSC application.
 *      Provides Media Access Data to the low level USB module for transfer.
 *
 *   @b Arguments
 *   @verbatim
            pMSC        Handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li         CSL_SOK              - Returned for success
 *   @li         CSL_ESYS_BADHANDLE   - Invalid Msc class handle
 *   @li         CSL_ESYS_INVPARAMS   - Returned for invalid storage state
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Transfers data on bulk end points
 *
 *   @b Modifies
 *   @n USB data buffeters
 *
 *   @b Example
 *   @verbatim
            CSL_MscInitStructApp    mscAppHandle;
            pMscAppClassHandle      pAppClassHandle;
            Uint16                  mscAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &mscAppHandle;

            // Assign values to all the members in mscAppHandle struct
            mscAppHandle.pMscObj = &mscAppBuffer[0];
            .....
            .....
            status = MSC_Open(pAppClassHandle);
            ....
            ....
            status = MSC_Bulk(pAppClassHandle->pMscObj);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_Bulk(void    *pMSC);

/** ============================================================================
 *   @n@b MSC_GetLunLockedStatus
 *
 *   @b Description
 *   @n This function gets the Locked/Unlocked Status of the Lun.
 *
 *   @b Arguments
 *   @verbatim
            pMSC        Handle to the MSC Class Obj
            lunNum      Logical Unit Number
     @endverbatim
 *
 *   <b> Return Value </b> - Lock/Unlock status of the Lun
 *   @li                     '1' - if the lun is locked
 *   @li                     '0' - if the lun is unlocked
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Returns the Locked/Unlocked status of Lun.
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            Uint16                  lockStatus;
            CSL_MscInitStructApp    mscAppHandle;
            pMscAppClassHandle      pAppClassHandle;
            Uint16                  mscAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &mscAppHandle;

            // Assign values to all the members in mscAppHandle struct
            mscAppHandle.pMscObj = &mscAppBuffer[0];
            .....
            .....
            status = MSC_Open(pAppClassHandle);
            ....
            ....
            lockStatus = MSC_GetLunLockedStatus(pAppClassHandle->pMscObj, 0);
     @endverbatim
 *  ============================================================================
 */
Uint16 MSC_GetLunLockedStatus(void      *pMSC,
                              Uint16    lunNum);

/** ============================================================================
 *   @n@b MSC_GetactivityFlagStatus
 *
 *   @b Description
 *   @n This function gives the status of the activity Flag
 *
 *   @b Arguments
 *   @verbatim
            pMSC        Handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  - Status of the activity flag
 *   @li                     '1' - If the activity is present
 *   @li                     '0' - If the activity is not present
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Returns Activity flag status
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            Uint16                  activityFlagStat;
            CSL_MscInitStructApp    mscAppHandle;
            pMscAppClassHandle      pAppClassHandle;
            Uint16                  mscAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &mscAppHandle;

            // Assign values to all the members in mscAppHandle struct
            mscAppHandle.pMscObj = &mscAppBuffer[0];
            .....
            .....
            status = MSC_Open(pAppClassHandle);
            ....
            ....
            activityFlagStat = MSC_GetactivityFlagStatus(pAppClassHandle->pMscObj);
     @endverbatim
 *  ============================================================================
 */
Uint16 MSC_GetactivityFlagStatus(void    *pMSC);

/** ============================================================================
 *   @n@b MSC_GetsuspendFlagStatus
 *
 *   @b Description
 *   @n This function gives the status of the suspend Flag
 *
 *   @b Arguments
 *   @verbatim
            pMSC        Handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  - Status of the suspend flag
 *   @li                     '1' - If the device is suspended
 *   @li                     '0' - If the device is not suspended
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Returns suspend flag status
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            Uint16                  suspendFlagStat;
            CSL_MscInitStructApp    mscAppHandle;
            pMscAppClassHandle      pAppClassHandle;
            Uint16                  mscAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &mscAppHandle;

            // Assign values to all the members in mscAppHandle struct
            mscAppHandle.pMscObj = &mscAppBuffer[0];
            .....
            .....
            status = MSC_Open(pAppClassHandle);
            ....
            ....
            suspendFlagStat = MSC_GetsuspendFlagStatus(pAppClassHandle->pMscObj);
     @endverbatim
 *  ============================================================================
 */
Uint16 MSC_GetsuspendFlagStatus(void    *pMSC);

/** ============================================================================
 *   @n@b MSC_initCtrlDesc
 *
 *   @b Description
 *   @n This function initializes the control object
 *
 *   @b Arguments
 *   @verbatim
            pCtrlHandle        Handle to the CTL Object
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK              - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE   - Invalid Ctrl handle
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes control object
 *
 *   @b Modifies
 *   @n Control object structure
 *
 *   @b Example
 *   @verbatim
            CSL_MscCtrlObject    ctrlObj;
            CSL_Status           status;

            status = MSC_initCtrlDesc(&ctrlObj);

     @endverbatim
 *  ============================================================================
 */
static CSL_Status MSC_initCtrlDesc(CSL_MscCtrlObject    *pCtrlHandle);

/** ============================================================================
 *   @n@b MSC_initMscVal
 *
 *   @b Description
 *   @n This function initializes the MSC object
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle        Handle to the Msc Object
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK              - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE   - Invalid Msc class handle
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes MSC object
 *
 *   @b Modifies
 *   @n MSC object structure
 *
 *   @b Example
 *   @verbatim
            CSL_MscObject    mscObj;
            CSL_Status       status;

            status = MSC_initMscVal(&mscObj);

     @endverbatim
 *  ============================================================================
 */
static CSL_Status MSC_initMscVal(CSL_MscObject    *pMscHandle);

/** ============================================================================
 *   @n@b MSC_HandleStateWaitForCBW
 *
 *   @b Description
 *   @n Handles the Wait for CBW state
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbOutEp       - Out end point handle
            hUsbInEp        - In end point handle
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n status returned by the MUSB module
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Changes the state to sending data or stall
 *
 *   @b Modifies
 *   @n storage state variable
 *
 *   @b Example
 *   @verbatim
             CSL_MscInitStructApp    mscAppHandle;
             CSL_MscObject           *pMscHandle;
             pUsbEpHandle            hUbOutEp;
             pUsbEpHandle            hUbInEp;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;
             pMscAppClassHandle      pAppClassHandle;

             pAppClassHandle = &mscAppHandle;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(pAppClassHandle);
             ....
             ....
             MSC_HandleStateWaitForCBW(pMscHandle, hUbOutEp, hUbInEp);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_HandleStateWaitForCBW(CSL_MscObject    *pMscHandle,
                                     pUsbEpHandle     hUsbOutEp,
                                     pUsbEpHandle     hUsbInEp);

/** ============================================================================
 *   @n@b MSC_ExecuteRead
 *
 *   @b Description
 *   @n This function executes the read operation. Data read from the storage
 *   media is sent to the host
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc object
            lunNum          - Logical unit number
            first           - Flag to indicate first write operation
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Status returned by the MUSB module
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Writes the data to the media
 *
 *   @b Modifies
 *   @n Data buffers
 *
 *   @b Example
 *   @verbatim
             CSL_MscRequestRet       (*fpRequestHandler)(CSL_MSC_REQUEST_ARGS)
             CSL_MscInitStructApp    mscAppHandle;
             CSL_MscObject           *pMscHandle;
             CSL_MscCtrlObject       *pCtrlHandle;
             pUsbEpHandle            hUbOutEp;
             Uint16                  lunNum;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;
             Uint16                  request;
             pMscAppClassHandle      pAppClassHandle;

             pAppClassHandle = &mscAppHandle;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(pAppClassHandle);
             ....
             ....
             status = MSC_ExecuteRead(pMscHandle, lunNum, FALSE);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_ExecuteRead(CSL_MscObject    *pMscHandle,
                           Uint16           lunNum,
                           Uint16           first);

/** ============================================================================
 *   @n@b MSC_ExecuteWrite
 *
 *   @b Description
 *   @n This function executes the write operation. Data sent by the host is
 *   is written to the storage media.
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc object
            lunNum          - Logical unit number
            first           - Flag to indicate first write operation
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Status returned by the MUSB module
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Writes the data to the media
 *
 *   @b Modifies
 *   @n Data buffers
 *
 *   @b Example
 *   @verbatim
             CSL_MscRequestRet       (*fpRequestHandler)(CSL_MSC_REQUEST_ARGS)
             CSL_MscInitStructApp    mscAppHandle;
             CSL_MscObject           *pMscHandle;
             CSL_MscCtrlObject       *pCtrlHandle;
             pUsbEpHandle            hUbOutEp;
             Uint16                  lunNum;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;
             Uint16                  request;
             pMscAppClassHandle      pAppClassHandle;

             pAppClassHandle = &mscAppHandle;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(pAppClassHandle);
             ....
             ....
             status = MSC_ExecuteWrite(pMscHandle, lunNum, FALSE);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_ExecuteWrite(CSL_MscObject    *pMscHandle,
                            Uint16           lunNum,
                            Uint16           first);

/** ============================================================================
 *   @n@b MSC_HandleTestUnitReady
 *
 *   @b Description
 *   @n This handles the Test Unit Ready SCSI Command
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbInEp        - In end point handle
            logicalUnit     - Logical unit number
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  status returned by the MUSB module
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Changes the state to sending CSW
 *
 *   @b Modifies
 *   @n storage state variable
 *
 *   @b Example
 *   @verbatim
             CSL_MscInitStructApp    mscAppHandle;
             CSL_MscObject           *pMscHandle;
             pUsbEpHandle            hUbOutEp;
             Uint16                  lunNum;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             MSC_HandleTestUnitReady(pMscHandle, hUbInEp, lunNum);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_HandleTestUnitReady(CSL_MscObject    *pMscHandle,
                                   pUsbEpHandle     hUsbInEp,
                                   Uint16           logicalUnit);

/** ============================================================================
 *   @n@b MSC_HandlePreventAllowMediaRemoval
 *
 *   @b Description
 *   @n This handles the Prevent Allow Media Removal SCSI Command
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbInEp        - In end point handle
            logicalUnit     - Logical unit number
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  status returned by the MUSB module
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Changes the state to sending CSW
 *
 *   @b Modifies
 *   @n storage state variable
 *
 *   @b Example
 *   @verbatim
             CSL_MscInitStructApp    mscAppHandle;
             CSL_MscObject           *pMscHandle;
             pUsbEpHandle            hUbOutEp;
             Uint16                  lunNum;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             MSC_HandlePreventAllowMediaRemoval(pMscHandle, hUbInEp, lunNum);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_HandlePreventAllowMediaRemoval(CSL_MscObject    *pMscHandle,
                                              pUsbEpHandle     hUsbInEp,
                                              Uint16           logicalUnit);

/** ============================================================================
 *   @n@b MSC_HandleReadCapacity
 *
 *   @b Description
 *   @n This handles the Read Capacity SCSI Command
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbInEp        - In end point handle
            logicalUnit     - Logical unit number
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  status returned by the MUSB module
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Changes the state to sending data
 *
 *   @b Modifies
 *   @n storage state variable
 *
 *   @b Example
 *   @verbatim
             CSL_MscInitStructApp    mscAppHandle;
             CSL_MscObject           *pMscHandle;
             pUsbEpHandle            hUbOutEp;
             Uint16                  lunNum;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             MSC_HandleReadCapacity(pMscHandle, hUbInEp, lunNum);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_HandleReadCapacity(CSL_MscObject    *pMscHandle,
                                  pUsbEpHandle     hUsbInEp,
                                  Uint16           logicalUnit);

/** ============================================================================
 *   @n@b  MSC_reqGetDescriptor
 *
 *   @b Description
 *   @n This function handles the request from host to get the descriptor
 *
 *   @b Arguments
 *   @verbatim
            CSL_MSC_REQUEST_ARGS    - MSC request arguments
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_MSC_REQUEST_GET_ACK  - Get acknowledgement for request\
 *   @li        CSL_MSC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  MSC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends the descriptor
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_MscInitStructApp    mscAppHandle;
             CSL_MscObject           *pMscHandle;
             CSL_MscCtrlObject       *pCtrlHandle;
             pUsbEpHandle            hUbOutEp;
             Uint16                  lunNum;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;
             Uint16                  request;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             pCtrlHandle->fpRequestHandler = MSC_lookupReqHandler(request,
                                                       pCtrlHandle->mscReqTable);

             pCtrlHandle->fpRequestHandler(pCtrlHandle->devNum,
                                           &pCtrlHandle->usbSetup,hCtrlEpIn,hCtrlEpOut,pMSC);
     @endverbatim
 *  ============================================================================
 */
CSL_MscRequestRet MSC_reqGetDescriptor(CSL_UsbDevNum         devNum,
                                       CSL_UsbSetupStruct    *usbSetup,
                                       pUsbEpHandle          hInEp,
                                       pUsbEpHandle          hOutEp,
                                       void                  *pMsc);

/** ============================================================================
 *   @n@b  MSC_reqBotMscReset
 *
 *   @b Description
 *   @n This function handles the Bulk Only Mass storage Reset request from host
 *
 *   @b Arguments
 *   @verbatim
         devNum   - USB device instance number
         usbSetup - USB setup packet structure
         hInEp    - USB IN end point handle
         hOutEp   - USB OUT end point handle
         pMsc     - Pointer to the MSC Class Obj structure

     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_MSC_REQUEST_GET_ACK  - Get acknowledgement for request
 *   @li        CSL_MSC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  MSC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Resets storage state
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_MscInitStructApp    mscAppHandle;
             CSL_MscObject           *pMscHandle;
             CSL_MscCtrlObject       *pCtrlHandle;
             pUsbEpHandle            hUbOutEp;
             Uint16                  lunNum;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;
             Uint16                  request;

             Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             pCtrlHandle->fpRequestHandler = MSC_lookupReqHandler(request,
                                                       pCtrlHandle->mscReqTable);

             pCtrlHandle->fpRequestHandler(pCtrlHandle->devNum,
                                           &pCtrlHandle->usbSetup,hCtrlEpIn,hCtrlEpOut,pMSC);
     @endverbatim
 *  ============================================================================
 */
CSL_MscRequestRet MSC_reqBotMscReset(CSL_UsbDevNum         devNum,
                                     CSL_UsbSetupStruct    *usbSetup,
                                     pUsbEpHandle          hInEp,
                                     pUsbEpHandle          hOutEp,
                                     void                  *pMsc);

/** ============================================================================
 *   @n@b MSC_sendCswWithPhaseError
 *
 *   @b Description
 *   @n This function sends CSW with phase error. This function is used to
 *      handle different error conditions by the SCSI commands.
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbInEp        - In end point handle
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n status returned by the MUSB module
 *
 *   <b> Pre Condition </b>
 *   @n  Error detected during the data transfer
 *
 *   <b> Post Condition </b>
 *   @n  Sends CSW with phase error
 *
 *   @b Modifies
 *   @n CSW buffer
 *
 *   @b Example
 *   @verbatim
             CSL_MscInitStructApp    mscAppHandle;
             CSL_MscObject           *pMscHandle;
             pUsbEpHandle            hUbOutEp;
             pUsbEpHandle            hUbInEp;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;
             pMscAppClassHandle      pAppClassHandle;

             pAppClassHandle = &mscAppHandle;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(pAppClassHandle);
             ....
             ....
             MSC_sendCswWithPhaseError(pMscHandle, hUbInEp);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_sendCswWithPhaseError(CSL_MscObject    *pMscHandle,
                                     pUsbEpHandle     hUsbInEp);

/** ============================================================================
 *   @n@b MSC_handleDataDirMisMatch
 *
 *   @b Description
 *   @n Function to handle the data direction miss during USB data transfers.
 *      This function stalls the end points and updates the CSW status with
 *      phase error.
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle     - Handle to Msc class object
            dataDir        - Actual Data direction expected;
                             should be 0x80 for IN and 0x0 for OUT
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n Status returned by the MUSB module
 *
 *   <b> Pre Condition </b>
 *   @n  Error detected during the data transfer
 *
 *   <b> Post Condition </b>
 *   @n  Stalls the IN/OUT end points
 *
 *   @b Modifies
 *   @n CSW buffer
 *
 *   @b Example
 *   @verbatim
             CSL_MscInitStructApp    mscAppHandle;
             CSL_MscObject           *pMscHandle;
             pUsbEpHandle            hUbOutEp;
             pUsbEpHandle            hUbInEp;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;
             pMscAppClassHandle      pAppClassHandle;

             pAppClassHandle = &mscAppHandle;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(pAppClassHandle);
             ....
             ....
             MSC_handleDataDirMisMatch(pMscHandle, CSL_MSC_CBW_DATADIR_IN);
     @endverbatim
 *  ============================================================================
 */
CSL_Status MSC_handleDataDirMisMatch(CSL_MscObject    *pMscHandle,
                                      Uint16           dataDir);
/**
@} */



#ifdef __cplusplus
}
#endif

#endif    //_CSL_MSC_H_

