/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_audioClass.h
 *
 *  @brief USB Audio Class functional layer API header file
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 10-Dec-2008 Created
 * 21-Jan-2009 Modified for code review comments
 * ============================================================================
 */

/** @defgroup CSL_AC_API AC
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 * The USB audio module provides a set of API’s that allow user Applications to
 * integrate support for the USB Audio Class. The USB audio module takes
 * care of the communication between USB and the audio device.
 * USB isochronous end points are used for communicating with the host machine.
 * Audio Class module implements interrupt mode of transfer for sending and
 * receiving data from the end points.
 * @subsection References
 * USB Specification Ver 2.0
 * Universal Serial Bus Device Class Definition for Audio Devices Ver 1.0
 */

#ifndef _CSL_AUDIOCLASS_H_
#define _CSL_AUDIOCLASS_H_

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
@defgroup CSL_AC_SYMBOL  USB AC Symbols Defined
@ingroup CSL_AC_API
*/
/**
@defgroup CSL_AC_DATASTRUCT  USB AC Data Structures
@ingroup CSL_AC_API
*/
/**
@defgroup CSL_AC_FUNCTION  USB AC Functions
@ingroup CSL_AC_API
*/
/**
@defgroup CSL_AC_ENUM USB AC Enumerated Data Types
@ingroup CSL_AC_API
*/


/*****************************************************************************\
          USB AC global macro declarations                                    *
\*****************************************************************************/
/**
@addtogroup CSL_AC_SYMBOL
@{
*/
/** \brief  USB AC Descriptor Type Values                                    */
/** USB AC device descriptor type macro                                      */
#define CSL_AC_DEVICE_DESCRIPTOR_TYPE                         (0x01)
/** USB AC configuration descriptor type macro                               */
#define CSL_AC_CONFIGURATION_DESCRIPTOR_TYPE                  (0x02)
/** USB AC string descriptor type macro                                      */
#define CSL_AC_STRING_DESCRIPTOR_TYPE                         (0x03)
/** USB AC interface descriptor type macro                                   */
#define CSL_AC_INTERFACE_DESCRIPTOR_TYPE                      (0x04)
/** USB AC end point descriptor type macro                                   */
#define CSL_AC_ENDPOINT_DESCRIPTOR_TYPE                       (0x05)
/** USB AC power descriptor type macro                                       */
#define CSL_AC_POWER_DESCRIPTOR_TYPE                          (0x06)
/** USB AC Human Interface Device descriptor type macro                      */
#define CSL_AC_HID_DESCRIPTOR_TYPE                            (0x21)

/** \brief  USB AC Defined Feature selectors                                 */
/** Ac host request to set/clear end point stall                             */
#define CSL_AC_FEATURE_ENDPOINT_STALL                         (0x0000)
/** Ac host request to set/reset remote wakeup feature                       */
#define CSL_AC_FEATURE_REMOTE_WAKEUP                          (0x0001)

/** Maximum allowed length of device descriptor                              */
#define CSL_AC_DEVICE_DESCR_MAX_LEN                           (20)
/** Maximum allowed length of device qualifier descriptor                    */
#define CSL_AC_DEV_QUAL_DESCR_MAX_LEN                         (12)
/** Maximum allowed length of configuration descriptor                       */
#define CSL_AC_CONFIG_DESCR_MAX_LEN                           (124)
/** Maximum allowed length of interface descriptor                           */
#define CSL_AC_INTER_DESCR_MAX_LEN                            (12)
/** Maximum allowed length of string descriptor language Id                  */
#define CSL_AC_STRING_DESCR_LANGID_MAX_LEN                    (12)
/** Macro for Mono type audio data                                           */
#define CSL_AC_AUDIO_TYPE_MONO                                (0x01)
/** Macro for Stereo type audio data                                         */
#define CSL_AC_AUDIO_TYPE_STEREO                              (0x02)

/** \brief  USB AC status and size definitions                               */
/** AC verification passed                                                   */
#define CSL_AC_VERIFY_PASSED                                  (0x0)
/** AC verification failed                                                   */
#define CSL_AC_VERIFY_FAILED                                  (0x1)
/** AC logical unit is removable                                             */
#define CSL_AC_UNIT_REMOVEABLE                                  (0x1)
/** Current USB device status                                                */
#define CSL_AC_CURRDEV_STAT                                   (0x01)
/** Current USB interface status                                             */
#define CSL_AC_CURRINTRF_STAT                                 (0x00)
/** No end point object is available                                         */
#define CSL_AC_NO_ENDPTOBJECT                                  (5)
/** Device descriptor length in words                                        */
#define CSL_AC_DEVDESC_LEN                                      (10)
/** Configuration descriptor length in words                                 */
#define CSL_AC_CONFDESC_LEN                                   (6)
/** Interface descriptor length in words                                     */
#define CSL_AC_MS_INTDESC_LEN                                  (6)
/** Iso out end point descriptor length in words                             */
#define CSL_AC_MS_ISOOUT_ENDPTDESC_LEN                          (5)
/** Iso in  end point descriptor length in words                             */
#define CSL_AC_MS_ISOIN_ENDPTDESC_LEN                          (5)
/** String descriptor language Id length in words                            */
#define CSL_AC_STRDESC_LANGID_LEN                             (4)
/** SCSI enquiry data length in words                                        */
#define CSL_AC_SCSI_INQUIRY_LEN                               (50)
/** Sense data length in words                                               */
#define CSL_AC_SENSEDATA_LEN                                  (15)
/** Maximum logical unit number                                              */
#define CSL_AC_LUN_MAX                                          (4)
/** Command block word size in words                                         */
#define CSL_AC_CBWSIZE                                        (17)
/** Command status word size in words                                        */
#define CSL_AC_CSWSIZE                                        (8)
/** AC read capacity                                                         */
#define CSL_AC_READCAP                                        (6)
/** Control buffer length in words                                           */
#define CSL_AC_CTRLBUFF_LEN                                   (5)
/** SCSI inquiry configuration length                                        */
#define CSL_AC_SCSI_INQUIRY_CONF_LEN                          (24)
/** Mass storage call definition macro                                       */
#define CSL_AC_DEVICE_CLASS_STORAGE                           (0x08)
/** SCSI data protect macro                                                  */
#define CSL_AC_SCSI_DATA_PROTECT                               (0x0007)
/** USB Audio Class specific request macro                                   */
#define CSL_AC_REQUEST_AUDIO_CLASS                            (0x20)
/** Length of buffer to service get request from host                        */
#define CSL_AC_GET_BUF_LEN                                    (0x04)

/** \brief  Symbolic constants for USB AC requests                           */
/** Host request to get status                                               */
#define CSL_AC_REQUEST_GET_STATUS                              (0x8000)
/** Host request to clear feature                                            */
#define CSL_AC_REQUEST_CLEAR_FEATURE                             (0x0001)
/** Host request to set current                                              */
#define CSL_AC_REQUEST_SET_CUR                                (0x2001)
/** Host request to get current                                              */
#define CSL_AC_REQUEST_GET_CUR                                (0xA081)
/** Host request to set minimum                                              */
#define CSL_AC_REQUEST_SET_MIN                                (0x2002)
/** Host request to get minimum                                              */
#define CSL_AC_REQUEST_GET_MIN                                (0xA082)
/** Host request to set maximum                                              */
#define CSL_AC_REQUEST_SET_MAX                                (0x2003)
/** Host request to get maximum                                              */
#define CSL_AC_REQUEST_GET_MAX                                (0xA083)
/** Host request to set resolution                                           */
#define CSL_AC_REQUEST_SET_RES                                (0x2004)
/** Host request to get resolution                                           */
#define CSL_AC_REQUEST_GET_RES                                (0xA084)
/** Audio Class Volume controlBuffer Length                                  */
#define CSL_AC_VOLUME_CTRL_BUF_LEN                            (6)
/** Audio Class Feature Unit Mute Control request ID                         */
#define USB_AC_FEATURE_UNIT_MUTE_CTRL                         (0x01)
/** Audio Class Feature Unit volume Control request ID                       */
#define USB_AC_FEATURE_UNIT_VOLUME_CTRL                        (0x02)
/** Audio Class Feature Unit left/right volume Control request ID            */
#define USB_AC_FEATURE_UNIT_LEFT_RIGHT_VOLUME                 (0x00)
/** Audio Class Feature Unit left volume Control request ID                  */
#define USB_AC_FEATURE_UNIT_LEFT_VOLUME                       (0x01)
/** Audio Class Feature Unit right volume Control request ID                 */
#define USB_AC_FEATURE_UNIT_RIGHT_VOLUME                      (0x02)

/** Host request to set feature                                              */
#define CSL_AC_REQUEST_SET_FEATURE                              (0x0003)
/** Host request to set address                                              */
#define CSL_AC_REQUEST_SET_ADDRESS                              (0x0005)
/** Host request to get descriptor                                           */
#define CSL_AC_REQUEST_GET_DESCRIPTOR                          (0x8006)
/** Host request to set descriptor                                           */
#define CSL_AC_REQUEST_SET_DESCRIPTOR                          (0x0007)
/** Host request to get configuration                                        */
#define CSL_AC_REQUEST_GET_CONFIGURATION                      (0x8008)
/** Host request to set configuration                                        */
#define CSL_AC_REQUEST_SET_CONFIGURATION                      (0x0009)
/** Host request to get interface                                            */
#define CSL_AC_REQUEST_GET_INTERFACE                          (0x800A)
/** Host request to set interface                                            */
#define CSL_AC_REQUEST_SET_INTERFACE                          (0x000B)
/** Host request for the synchronization frame                               */
#define CSL_AC_REQUEST_SYNC_FRAME                              (0x800C)
/** Host request for Iso only reset                                          */
#define CSL_AC_REQUEST_ISO_ONLY_RESET                          (0x20FF)
/** Host request to get maximum logical unit number                          */
#define CSL_AC_REQUEST_GET_MAX_LUN                              (0xA0FE)
/** Base value for host request to get status                                */
#define CSL_AC_REQUEST_TYPE_BASE                              (0x80)
/** Host request to get device status                                        */
#define CSL_AC_REQUEST_TYPE_DEVICE_STATUS                      (0x0)
/** Host request to get interface status                                     */
#define CSL_AC_REQUEST_TYPE_INTERFACE_STATUS                  (0x1)
/** Host request to get end point status                                     */
#define CSL_AC_REQUEST_TYPE_EP_STATUS                          (0x2)

/** \brief  AC Status query API return values                                */
/** Return value for USB device suspended case                               */
#define CSL_AC_DEVICE_SUSPENDED                               (0x01)
/** Return value for USB device not suspended case                           */
#define CSL_AC_DEVICE_NOT_SUSPENDED                           (0x0)
/** Return value for Logical unit locked case                                */
#define CSL_AC_LUN_LOCKED                                     (0x01)
/** Return value for Logical unit not locked case                            */
#define CSL_AC_LUN_NOT_LOCKED                                 (0x0)
/** Return value for activity present case                                   */
#define CSL_AC_ACTIVITY_PRESENT                               (0x01)
/** Return value for activity not present case                               */
#define CSL_AC_ACTIVITY_NOT_PRESENT                           (0x0)

/** AC 16 bit Mask value                                                     */
#define CSL_AC_16BIT_MASK                                     (0xFFFF)
/** AC 16 bit shift value                                                    */
#define CSL_AC_16BIT_SHIFT                                    (16)

/** AC 24 bit shift value                                                    */
#define CSL_AC_24BIT_SHIFT                                    (24)

/** AC 8 bit Mask value                                                      */
#define CSL_AC_8BIT_MASK                                      (0xFF)
/** AC 8 bit Mask value                                                      */
#define CSL_AC_8BIT_HIGH_MASK                                 (0xFF00)
/** AC 8 bit shift value                                                     */
#define CSL_AC_8BIT_SHIFT                                     (8)
/** AC Unicode data size                                                     */
#define CSL_AC_UNICODE_SIZE                                   (66)

/** Default value for element 0 in sense data array                          */
#define CSL_AC_SENSE_DATA0_DEFVAL                             (26)
/** Default value for element 1 in sense data array                          */
#define CSL_AC_SENSE_DATA1_DEFVAL                             (0)
/** Default value for element 2 in sense data array                          */
#define CSL_AC_SENSE_DATA2_DEFVAL                             (0x0070)
/** Default value for element 3 in sense data array                          */
#define CSL_AC_SENSE_DATA3_DEFVAL                             (0x0000)
/** Default value for element 4 in sense data array                          */
#define CSL_AC_SENSE_DATA4_DEFVAL                             (0x0000)
/** Default value for element 5 in sense data array                          */
#define CSL_AC_SENSE_DATA5_DEFVAL                             ((11<<8) | 0)

/** Read capacity 0 default value                                            */
#define CSL_AC_READ_CAPACITY0_DEFVAL                          (8)

/** Command status word 0 default value                                      */
#define CSL_AC_CSW0_DEFVAL                                    (0)
/** Command status word 1 default value                                      */
#define CSL_AC_CSW1_DEFVAL                                    (0x5355)
/** Command status word 2 default value                                      */
#define CSL_AC_CSW2_DEFVAL                                    (0x5342)

/** Error Code base                                                          */
#define CSL_AC_ERROR_BASE                             (CSL_EUSB_FIRST)
/** Returned when the AC request handler is invalid                          */
#define CSL_AC_INVALID_REQUEST_HANDLER                (CSL_AC_ERROR_BASE - 1)

/** \brief AC Request arguments                                              */
#define CSL_AC_REQUEST_ARGS     CSL_UsbDevNum         devNum,           \
                                CSL_UsbSetupStruct    *usbSetup,        \
                                pUsbEpHandle          hInEp,            \
                                pUsbEpHandle          hOutEp,           \
                                void                  *pAcObj

/**
@} */


/**************************************************************************\
* USB AC global typedef declarations                                       *
\**************************************************************************/

/** @addtogroup CSL_AC_ENUM
 @{ */

/**
 *  \brief This Enum defines the Audio request return values
 */
typedef enum {
    /* Don't call request again until new SETUP                              */
    CSL_AC_REQUEST_DONE = 0,
    /* STALL the control endpoint                                            */
    CSL_AC_REQUEST_STALL,
    /* Send a 0 length IN packet                                             */
    CSL_AC_REQUEST_SEND_ACK,
    /* Prepare to receive 0 length OUT packet                                */
    CSL_AC_REQUEST_GET_ACK,
    /* Notify handler when IN data has been transmitted                      */
    CSL_AC_REQUEST_DATA_IN,
    /* Notify handler when OUT data has been received                        */
    CSL_AC_REQUEST_DATA_OUT
} CSL_AcRequestRet;

/**
 *  \brief This Enum defines the AC media access status
 */
typedef enum {
    /** Media over flow error                                                */
    CSL_AC_MEDIACCESS_OVERFLOW,
    /** Media Access success                                                 */
    CSL_AC_MEDIACCESS_SUCCESS,
    /** Media not present on the system                                      */
    CSL_AC_MEDIACCESS_NOTPRESENT,
    /** Media is write protected                                             */
    CSL_AC_MEDIACCESS_WRITEPROTECT,
    /** Media is found as bad                                                */
    CSL_AC_MEDIACCESS_BADMEDIA
} CSL_AcMediaStatus;

/**
 *  \brief This Enum defines the AC media lock status
 */
typedef enum {
    /** Lock the media                                                       */
    CSL_AC_UNLOCK = 0,
    /** Unlock the media                                                     */
    CSL_AC_LOCK
} CSL_AcMediaLockStatus;

/**
 *  \brief This Enum defines types of descriptors used by the USB audio class
 */
typedef enum {
    /** Device descriptor Id                                                 */
    CSL_AC_DEVICE_DESCR = 0,
    /** Device qualifier descriptor Id                                       */
    CSL_AC_DEVICE_QUAL_DESCR,
    /** Configuration descriptor Id                                          */
    CSL_AC_CONFIG_DESCR,
    /** Interface descriptor Id                                              */
    CSL_AC_INTERFACE_DESCR,
    /** String descriptor language Id                                        */
    CSL_AC_STRING_LANGID_DESC
} CSL_AcDescrId;

/**
@} */

/**************************************************************************\
* USB AC global data structure declarations                                *
\**************************************************************************/

/**
\addtogroup CSL_AC_DATASTRUCT
@{ */

/**
 *  \brief Audio Class control request structure
 *
 *  This structure holds the mapping of the Control Request to the
 *  Control Request handler
 */
typedef struct CSL_AcRequestStruct {
    /** \brief  AC control request                                           */
    WORD                request;
    /** \brief  AC control request handler pointer                           */
    CSL_AcRequestRet    (*fpRequestHandler)(CSL_AC_REQUEST_ARGS);
} CSL_AcRequestStruct;

/**
 *  \brief Audio Class application data structure
 *
 *  Holds the structure used by the application to fill in
 *  application specific data handler
 */
typedef struct CSL_AcInitStructApp {
    /** \brief  Pointer to Audio Class Handle defined in AC Module           */
    void                   *pAcObj;
    /** \brief  Device Descriptor units                                           */
    Uint16                 pId;
    /** \brief  Device Descriptor units                                          */
    Uint16                 vId;
    /** \brief  Logical unit number                                              */
    Uint16                 numLun;
    /** \brief  Size of the Tx packet for Isochronous end points              */
    Uint16                 txPktSize;
    /** \brief  Size of the Rx packet for Isochronous end points              */
    Uint16                 rxPktSize;
    /** \brief  User specific string descriptor pointer                          */
    char                   **strDescrApp;
    /** \brief  Buffer ptr passed by the system to the module in order to
                enable data transfer to/from the media/USB API layer.        */
    Uint16                 *lbaBufferApp;
    /** \brief  Pointer to the request Table which is specified by user      */
    CSL_AcRequestStruct    *acReqTableApp;
    /** \brief  Pointer to the Iso Function Handler which is
                needed during Configuration of the Iso Endpoint              */
    void                   (*isoHandler)();
    /** \brief  Pointer to the Control Function Handler which is
                needed during Configuration of the Control Endpoint          */
    void                   (*ctrlHandler)();
    /** \brief  Function to Play Audio data                                      */
    CSL_AcMediaStatus      (*playAudioApp)(Uint16 , Uint16* , Uint16*, void*);
    /** \brief  Function to stop Audio playback                               */
    CSL_AcMediaStatus      (*stopAudioApp)(Uint16 , void* );
    /** \brief  Function for record audio data                                */
    CSL_AcMediaStatus      (*recordAudioApp)(Uint16 , Uint16* , Uint16*, void*);
    /** \brief  Function checks for Media Present    etc                       */
    CSL_AcMediaStatus      (*mediaGetPresentStateApp)(Uint16);
    /** \brief  Function Handler for Init Media                                  */
    CSL_AcMediaStatus      (*mediaInitApp)(Uint16);
    /** \brief  Function to eject Media                                          */
    CSL_AcMediaStatus      (*mediaEjectApp)(Uint16);
    /** \brief  Function to lock Media                                         */
    CSL_AcMediaStatus      (*mediaLockUnitApp)(Uint16,CSL_AcMediaLockStatus);
    /** \brief  Function to get the size of the media                         */
    Uint32                 (*getMediaSizeApp)(Uint16);
} CSL_AcInitStructApp;

/** \brief  Audio Class application class handle                             */
typedef CSL_AcInitStructApp  *pAcAppClassHandle;

/**
 *  \brief AC application Logical unit attribute structure
 *
 *  Holds the attributes of every logical unit. This structure
 *  should be filled by the application.
 */
typedef struct CSL_AcLunAttribApp {
    /** \brief  Logical Block Size (size of each LB)                         */
    Uint32    lbaSizeApp;
    /** \brief  Total Number Of LB in the Media                                  */
    Uint32    mediaSizeApp;
    /** \brief  Removable or Non removable Media                             */
    Uint16    removeableApp;
    /** \brief  SCSI Inquiry Data  - User Configurable Items                 */
    Uint16    scsiInquiryConfData [CSL_AC_SCSI_INQUIRY_CONF_LEN];
    /** \brief  Media State                                                      */
    Uint16    mediaStateApp;
    /** \brief  Device is Readable and/or Writable                             */
    Uint16    readwriteApp;
} CSL_AcLunAttribApp;

/** \brief  Audio Class application Lun attribute handle                     */
typedef CSL_AcLunAttribApp   *pAcAppLunAttribHandle;

/** \brief  Audio Class request handler function pointer                     */
typedef  CSL_AcRequestRet (*fpAC_REQ_HANDLER)(CSL_AC_REQUEST_ARGS);

/**
 *  \brief Audio Class Logical unit structure
 *
 *  Holds all the components for the logical unit
 *  This structure is filled with the data given by the application
 *  through CSL_AcLunAttribApp structure
 */
typedef struct CSL_AcLogicalUnit {
    /** \brief  SCSI Inquiry Data                                            */
    Uint16    scsiInquiryData[CSL_AC_SCSI_INQUIRY_LEN];
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
} CSL_AcLogicalUnit;

/**
 *  \brief USB Audio Class Object structure
 *
 *  Holds all the components for the Audio Class Object
 */
typedef struct CSL_AcObject {
    /** \brief  Function to Record the audio data                            */
    CSL_AcMediaStatus    (*recordAudio)(Uint16 , Uint16* , Uint16*, void*);
    /** \brief  Function to Play the audio data                              */
    CSL_AcMediaStatus    (*playAudio)(Uint16 , Uint16* , Uint16*, void*);
    /** \brief  Function to stop Audio playback                               */
    CSL_AcMediaStatus    (*stopAudio)(Uint16 , void*);
    /** \brief  Function to know the status of Media - Present or Not        */
    CSL_AcMediaStatus    (*mediaGetPresentState)(Uint16 );
    /** \brief  Function to initialize the Media                             */
    CSL_AcMediaStatus    (*mediaInit)(Uint16 );
    /** \brief  Function to Eject Media                                      */
    CSL_AcMediaStatus    (*mediaEject)(Uint16 );
    /** \brief  Function to Lock Media                                       */
    CSL_AcMediaStatus    (*mediaLockUnit)(Uint16,CSL_AcMediaLockStatus);
    /** \brief  Function to get the size of the media                        */
    Uint32               (*getMediaSize)(Uint16 );
    /** \brief  Data buffer pointer used to transfer data to/from Media/usb  */
    Uint16               *lbaBuffer;
    /** \brief  CBW transfer length                                          */
    Uint32                cbwDataTransferLength;
     /** \brief  Data pertaining to Logical Units Supported                   */
    CSL_AcLogicalUnit    lun[CSL_AC_LUN_MAX];
    /** \brief  Sense Data Array                                             */
    Uint16               senseData[CSL_AC_SENSEDATA_LEN];
    /** \brief  Flag to Indicate whether any Media has been Accessed         */
    Uint16               activityPresentFlag;
    /** \brief  Array used to store the elements of CSW                      */
    Uint16               csw[CSL_AC_CSWSIZE];
    /** \brief  Array used to store Read Capacity                            */
    Uint16               readCapacity[CSL_AC_READCAP];
    /** \brief  Array used to store the elements of CBW                      */
    Uint16               cbw[CSL_AC_CBWSIZE];
    /** \brief  Iso In Endpoint Object                                       */
    CSL_UsbEpObj         isoInEpObj;
    /** \brief  Iso Out Endpoint Object                                      */
    CSL_UsbEpObj         isoOutEpObj;
    /** \brief  Maximum No.of Logical Units                                  */
    Uint16               noOfLun;
    /** \brief  The State the Class is in                                    */
    Uint16               mediaState;
    /** \brief  Flag to track the status of audio playback                   */
    Uint16               playBackActive;
    /** \brief  Flag to track the init status of the media                   */
    Uint16               mediaInitDone;
    /** \brief  Buffer to hold the temporary data                            */
    Uint16               tempBuffer[4];
} CSL_AcObject;

/**
 *  \brief USB AC Control Object structure
 *
 *  Holds all the components for the Audio Class Control Object
 */
typedef struct CSL_AcCtrlObject {
    /** \brief  Control In Endpoint Object                                   */
    CSL_UsbEpObj           ctrlInEpObj;
    /** \brief  Object of Type Device Number                                 */
    CSL_UsbDevNum          devNum;
    /** \brief  Control Out Endpoint Object                                  */
    CSL_UsbEpObj           ctrlOutEpObj;
    /** \brief  Flag Used internally by APIs                                 */
      Uint16                 suspendFlag;
      /** \brief  NULL terminated array of Endpoint Objects                    */
    pUsbEpHandle           hEpObjArray[CSL_AC_NO_ENDPTOBJECT];
    /** \brief  Setup Structure                                              */
    CSL_UsbSetupStruct     usbSetup;
    /** \brief  Buffer to hold the left volume information                   */
    Uint16                 leftVolBuf[CSL_AC_VOLUME_CTRL_BUF_LEN];
    /** \brief  Buffer to hold the right volume information                  */
    Uint16                 rightVolBuf[CSL_AC_VOLUME_CTRL_BUF_LEN];
    /** \brief  Buffer to hold the left and right volume information         */
    Uint16                 leftRightVolBuf[2*CSL_AC_VOLUME_CTRL_BUF_LEN];
    /** \brief  Buffer to hold the mute ctrl information                     */
    Uint16                 muteCtrlBuf[CSL_AC_VOLUME_CTRL_BUF_LEN];
    /** \brief  Buffer to service get minimum request                        */
    Uint16                 getMinBuffer[CSL_AC_GET_BUF_LEN];
    /** \brief  Buffer to service get maximum request                        */
    Uint16                 getMaxBuffer[CSL_AC_GET_BUF_LEN];
    /** \brief  Buffer to service get resolution request                     */
    Uint16                 getResBuffer[CSL_AC_GET_BUF_LEN];
    /** \brief  Dummy descriptor array                                       */
    Uint16                 dummydesc[63];
    /** \brief  Device qualifier descriptor (Used in Full soeed mode)        */
    Uint16                 deviceQualifierDescr[CSL_AC_DEV_QUAL_DESCR_MAX_LEN];
    /** \brief  Device descriptor                                            */
    Uint16                 deviceDescr[CSL_AC_DEVICE_DESCR_MAX_LEN];
    /** \brief  Configuration descriptor                                     */
    Uint16                 confDescr[CSL_AC_CONFIG_DESCR_MAX_LEN];
    /** \brief  Audio Class Interface descriptor                             */
    Uint16                 audioClassIntDescr[CSL_AC_INTER_DESCR_MAX_LEN];
    /** \brief  Audio Class Iso Out Endpoint Descriptor                      */
    Uint16 audioClassIsoOutEndptDescr[CSL_AC_MS_ISOOUT_ENDPTDESC_LEN];
    /** \brief  Audio Class Interface descriptor Link                        */
    CSL_UsbDataStruct      audioClassInterfcLnk;
    /** \brief  Audio Class ISo In Endpoint Descriptor                       */
    Uint16 audioClassIsoInEndptDescr[CSL_AC_MS_ISOIN_ENDPTDESC_LEN];
    /** \brief  Audio Class Iso Out Endpoint Link                            */
    CSL_UsbDataStruct      audioClassIsoOutEndptLnk;
    /** \brief  String Descriptor Language ID                                */
    Uint16                 stringDescrLangid[CSL_AC_STRING_DESCR_LANGID_MAX_LEN];
    /** \brief  Current Device Configuration Status                          */
    Uint16                 curConfigStat;
    /** \brief  Audio Class Iso In Endpoint Link                             */
    CSL_UsbDataStruct      audioClassIsoInEndptLnk;
    /** \brief  Array used for General Purpose                               */
    Uint16                 ctrlBuffer[CSL_AC_CTRLBUFF_LEN];
    /** \brief  Configuration Descriptor Link                                */
    CSL_UsbDataStruct      configDescrLnk;
    /** \brief  String descriptor pointer                                    */
    char                   **strDescr;
    /** \brief  Pointer to the request Table                                 */
    CSL_AcRequestStruct    *acReqTable;
    /** \brief  Request handler function pointer                             */
    CSL_AcRequestRet        (*fpRequestHandler)(CSL_AC_REQUEST_ARGS);
} CSL_AcCtrlObject;

/**
 *  \brief AC Control Object structure
 *
 *  Holds all the components for the AC Control Object
 */
typedef struct CSL_AcClassStruct {
    /** \brief  Handle to Control Object                                      */
    CSL_AcCtrlObject    ctrlHandle;
    /** \brief  Handle to Iso Transfer Object                                 */
    CSL_AcObject        acHandle;
} CSL_AcClassStruct;

/** \brief  Audio Class class handle                                          */
typedef CSL_AcClassStruct  *pAcClassHandle;

/**
@} */

/**************************************************************************\
* USB AC function declarations                                            *
\**************************************************************************/

/** @addtogroup CSL_AC_FUNCTION
 @{ */


/** ============================================================================
 *   @n@b AC_Open
 *
 *   @b Description
 *   @n This function Opens the Audio class module.
 *      It initializes Ac and Ctrl handles with the data given
 *      by the application
 *
 *   @b Arguments
 *   @verbatim
            pAppClassHandle      Application handle to the Audio Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK              - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE   - Invalid Audio class handle
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes Ac and Ctrl handles
 *
 *   @b Modifies
 *   @n Ac and Ctrl handle data
 *
 *   @b Example
 *   @verbatim
            CSL_AcInitStructApp    acAppHandle;
            pAcAppClassHandle      pAppClassHandle;
            Uint16                 acAppBuffer[420];
            CSL_Status             status;

            pAppClassHandle = &acAppHandle;

            // Assign values to all the members in acAppHandle struct
            acAppHandle.pAcObj = &acAppBuffer[0];
            .....
            .....
            status = AC_Open(pAppClassHandle);
     @endverbatim
 *  ============================================================================
 */
CSL_Status AC_Open(pAcAppClassHandle    pAppClassHandle);

/** ============================================================================
 *   @n@b AC_Close
 *
 *   @b Description
 *   @n This function Closes the audio class module.
 *
 *   @b Arguments
 *   @verbatim
            pAppClassHandle       Application handle to the Audio Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK              - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE   - Invalid Audio class handle
 *
 *   <b> Pre Condition </b>
 *   @n  AC_Open function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Closes the Audio Class module
 *
 *   @b Modifies
 *   @n Ac and Ctl handles
 *
 *   @b Example
 *   @verbatim
            CSL_AcInitStructApp    acAppHandle;
            pAcAppClassHandle      pAppClassHandle;
            Uint16                 acAppBuffer[420];
            CSL_Status             status;

            pAppClassHandle = &acAppHandle;

            // Assign values to all the members in acAppHandle struct
            acAppHandle.pAcObj = &acAppBuffer[0];
            .....
            .....
            status = AC_Open(pAppClassHandle);
            ....
            ....
            status = AC_Close(pAppClassHandle);
     @endverbatim
 *  ============================================================================
 */
CSL_Status AC_Close(pAcAppClassHandle    pAppClassHandle);

/** ============================================================================
 *   @n@b AC_Ctrl
 *
 *   @b Description
 *   @n This function handles all USB traffic on the Control Endpoint.
 *      Detects the events on the Control Endpoint and takes
 *      the appropriate action
 *
 *   @b Arguments
 *   @verbatim
            pAcObj        Handle to the Audio Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li       CSL_SOK                          - Returned for success
 *   @li       CSL_ESYS_BADHANDLE               - Invalid Audio class handle
 *   @li       CSL_AC_INVALID_REQUEST_HANDLER   - Invalid request class handler
 *
 *   <b> Pre Condition </b>
 *   @n  AC_Open function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Control end point events are serviced
 *
 *   @b Modifies
 *   @n USB Registers.
 *
 *   @b Example
 *   @verbatim
            CSL_AcInitStructApp    acAppHandle;
            pAcAppClassHandle      pAppClassHandle;
            Uint16                 acAppBuffer[420];
            CSL_Status             status;

            pAppClassHandle = &acAppHandle;

            // Assign values to all the members in acAppHandle struct
            acAppHandle.pAcObj = &acAppBuffer[0];
            .....
            .....
            status = AC_Open(pAppClassHandle);
            ....
            ....
            status = AC_Ctrl(pAppClassHandle->pAcObj);
     @endverbatim
 *  ============================================================================
 */
CSL_Status AC_Ctrl(void    *pAcObj);

/** ============================================================================
 *   @n@b AC_Iso
 *
 *   @b Description
 *   @n This function handles all UBS traffic on the Isochronous Endpoint
 *      This maintains the state of the Audio class application.
 *      Provides Media Access Data to the low level USB module for transfer.
 *
 *   @b Arguments
 *   @verbatim
            pAcObj        Handle to the Audio Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li         CSL_SOK              - Returned for success
 *   @li         CSL_ESYS_BADHANDLE   - Invalid Audio class handle
 *   @li         CSL_ESYS_INVPARAMS   - Returned for invalid parameters
 *
 *   <b> Pre Condition </b>
 *   @n  AC_Open function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Transfers data on iso end points
 *
 *   @b Modifies
 *   @n USB data buffeters
 *
 *   @b Example
 *   @verbatim
            CSL_AcInitStructApp    acAppHandle;
            pAcAppClassHandle      pAppClassHandle;
            Uint16                 acAppBuffer[420];
            CSL_Status             status;

            pAppClassHandle = &acAppHandle;

            // Assign values to all the members in acAppHandle struct
            acAppHandle.pAcObj = &acAppBuffer[0];
            .....
            .....
            status = AC_Open(pAppClassHandle);
            ....
            ....
            status = AC_Iso(pAppClassHandle->pAcObj);
     @endverbatim
 *  ============================================================================
 */
CSL_Status AC_Iso(void    *pAcObj);

/** ============================================================================
 *   @n@b AC_GetactivityFlagStatus
 *
 *   @b Description
 *   @n This function gives the status of the activity Flag
 *
 *   @b Arguments
 *   @verbatim
            pAcObj        Handle to the Audio Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  - Status of the activity flag
 *   @li                     '1' - If the activity is present
 *   @li                     '0' - If the activity is not present
 *
 *   <b> Pre Condition </b>
 *   @n  AC_Open function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns Activity flag status
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            Uint16                 activityFlagStat;
            CSL_AcInitStructApp    acAppHandle;
            pAcAppClassHandle      pAppClassHandle;
            Uint16                 acAppBuffer[420];
            CSL_Status             status;

            pAppClassHandle = &acAppHandle;

            // Assign values to all the members in acAppHandle struct
            acAppHandle.pAcObj = &acAppBuffer[0];
            .....
            .....
            status = AC_Open(pAppClassHandle);
            ....
            ....
            activityFlagStat = AC_GetactivityFlagStatus(pAppClassHandle->pAcObj);
     @endverbatim
 *  ============================================================================
 */
Uint16 AC_GetactivityFlagStatus(void    *pAcObj);

/** ============================================================================
 *   @n@b AC_GetsuspendFlagStatus
 *
 *   @b Description
 *   @n This function gives the status of the suspend Flag
 *
 *   @b Arguments
 *   @verbatim
            pAcObj        Handle to the Audio Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  - Status of the suspend flag
 *   @li                     '1' - If the device is suspended
 *   @li                     '0' - If the device is not suspended
 *
 *   <b> Pre Condition </b>
 *   @n  AC_Open function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns suspend flag status
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            Uint16                 suspendFlagStat;
            CSL_AcInitStructApp    acAppHandle;
            pAcAppClassHandle      pAppClassHandle;
            Uint16                 acAppBuffer[420];
            CSL_Status             status;

            pAppClassHandle = &acAppHandle;

            // Assign values to all the members in acAppHandle struct
            acAppHandle.pAcObj = &acAppBuffer[0];
            .....
            .....
            status = AC_Open(pAppClassHandle);
            ....
            ....
            suspendFlagStat = AC_GetsuspendFlagStatus(pAppClassHandle->pAcObj);
     @endverbatim
 *  ============================================================================
 */
Uint16 AC_GetsuspendFlagStatus(void    *pAcObj);

/** ============================================================================
 *   @n@b AC_initDescriptors
 *
 *   @b Description
 *   @n This function initializes the descriptors used by the USB audio class
 *
 *   @b Arguments
 *   @verbatim
            pAcObj      Audio Class object
            descBuf     Descriptor buffer pointer
            descrId     Id of the Descriptor to initialize
            descrLen    Length of the descriptor to initialize (in bytes)
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid Ctrl handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes descriptors
 *
 *   @b Modifies
 *   @n Descriptor buffers
 *
 *   @b Example
 *   @verbatim
            CSL_AcInitStructApp    acAppHandle;
            pAcAppClassHandle      pAppClassHandle;
            Uint16                 acAppBuffer[420];
            CSL_Status             status;

            pAppClassHandle = &acAppHandle;

            // Assign values to all the members in acAppHandle struct
            acAppHandle.pAcObj = &acAppBuffer[0];

            status = AC_initDescriptors(pAppClassHandle->pAcObj,
                                        CSL_AC_DEVICE_DESCR, 18);
     @endverbatim
 *  ============================================================================
 */
CSL_Status AC_initDescriptors(void             *pAcObj,
                              Uint16           *descBuf,
                              CSL_AcDescrId    descrId,
                              Uint16           descrLen);

/** ============================================================================
 *   @n@b AC_initAcVal
 *
 *   @b Description
 *   @n This function initializes the Audio Class object
 *
 *   @b Arguments
 *   @verbatim
            pAcHandle        Handle to the Audio class Object
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK              - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE   - Invalid Audio class handle
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes Audio Class object
 *
 *   @b Modifies
 *   @n Audio Class object structure
 *
 *   @b Example
 *   @verbatim
            CSL_AcObject    acObj;
            CSL_Status      status;

            status = AC_initAcVal(&acObj);

     @endverbatim
 *  ============================================================================
 */
static CSL_Status AC_initAcVal(CSL_AcObject    *pAcHandle);

/** ============================================================================
 *   @n@b  AC_reqGetDescriptor
 *
 *   @b Description
 *   @n This function handles the request from host to get the descriptor
 *
 *   @b Arguments
 *   @verbatim
            CSL_AC_REQUEST_ARGS    - AC request arguments
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_AC_REQUEST_GET_ACK  - Get acknowledgement for request\
 *   @li        CSL_AC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  AC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends the descriptor
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_AcInitStructApp    acAppHandle;
             CSL_AcObject           *pAcHandle;
             CSL_AcCtrlObject       *pCtrlHandle;
             pUsbEpHandle           hUbOutEp;
             Uint16                 lunNum;
             Uint16                 acAppBuffer[420];
             CSL_Status             status;
             Uint16                 request;

             // Assign values to all the members in acAppHandle struct
             acAppHandle.pAcObj = &acAppBuffer[0];
             .....
             .....
             status = AC_Open(&acAppHandle);
             ....
             ....
             pCtrlHandle->fpRequestHandler = AC_lookupReqHandler(request,
                                                       pCtrlHandle->acReqTable);

             pCtrlHandle->fpRequestHandler(pCtrlHandle->devNum,
                                           &pCtrlHandle->usbSetup,hCtrlEpIn,hCtrlEpOut,pAC);
     @endverbatim
 *  ============================================================================
 */
CSL_AcRequestRet AC_reqGetDescriptor(CSL_UsbDevNum         devNum,
                                     CSL_UsbSetupStruct    *usbSetup,
                                     pUsbEpHandle          hInEp,
                                     pUsbEpHandle          hOutEp,
                                     void                  *pAcObj);

/** ============================================================================
 *   @n@b  AC_reqSetCurrent
 *
 *   @b Description
 *   @n This function handles the request from host to set current
 *
 *   @b Arguments
 *   @verbatim
            devNum       - USB device number
            usbSetup     - USb setup structure
            hInEp        - USB ISO In end point handle
            hOutEp       - USB ISO Out end point object
            pAcObj       - USB Audio Class object
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_AC_REQUEST_SEND_ACK - Send acknowledgement for request
 *   @li        CSL_AC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  AC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends acknowledgement
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_AcInitStructApp    acAppHandle;
             CSL_AcObject           *pAcHandle;
             CSL_AcCtrlObject       *pCtrlHandle;
             pUsbEpHandle           hUbOutEp;
             Uint16                 lunNum;
             Uint16                 acAppBuffer[420];
             CSL_Status             status;
             Uint16                 request;

             // Assign values to all the members in acAppHandle struct
             acAppHandle.pAcObj = &acAppBuffer[0];
             .....
             .....
             status = AC_Open(&acAppHandle);
             ....
             ....
             pCtrlHandle->fpRequestHandler = AC_lookupReqHandler(request,
                                                       pCtrlHandle->acReqTable);

             pCtrlHandle->fpRequestHandler(pCtrlHandle->devNum,
                                           &pCtrlHandle->usbSetup,hCtrlEpIn,hCtrlEpOut,pAcObj);
     @endverbatim
 *  ============================================================================
 */
CSL_AcRequestRet AC_reqSetCurrent(CSL_UsbDevNum           devNum,
                                    CSL_UsbSetupStruct    *usbSetup,
                                    pUsbEpHandle          hInEp,
                                    pUsbEpHandle          hOutEp,
                                    void                  *pAcObj);

/** ============================================================================
 *   @n@b  AC_reqGetCurrent
 *
 *   @b Description
 *   @n This function handles the request from host to get current
 *
 *   @b Arguments
 *   @verbatim
            devNum       - USB device number
            usbSetup     - USb setup structure
            hInEp        - USB ISO In end point handle
            hOutEp       - USB ISO Out end point object
            pAcObj       - USB Audio Class object
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_AC_REQUEST_GET_ACK  - Get acknowledgement for request
 *   @li        CSL_AC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  AC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends requested current value
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_AcInitStructApp    acAppHandle;
             CSL_AcObject           *pAcHandle;
             CSL_AcCtrlObject       *pCtrlHandle;
             pUsbEpHandle           hUbOutEp;
             Uint16                 lunNum;
             Uint16                 acAppBuffer[420];
             CSL_Status             status;
             Uint16                 request;

             // Assign values to all the members in acAppHandle struct
             acAppHandle.pAcObj = &acAppBuffer[0];
             .....
             .....
             status = AC_Open(&acAppHandle);
             ....
             ....
             pCtrlHandle->fpRequestHandler = AC_lookupReqHandler(request,
                                                       pCtrlHandle->acReqTable);

             pCtrlHandle->fpRequestHandler(pCtrlHandle->devNum,
                                           &pCtrlHandle->usbSetup,hCtrlEpIn,hCtrlEpOut,pAcObj);
     @endverbatim
 *  ============================================================================
 */
CSL_AcRequestRet AC_reqGetCurrent(CSL_UsbDevNum         devNum,
                                  CSL_UsbSetupStruct    *usbSetup,
                                  pUsbEpHandle          hInEp,
                                  pUsbEpHandle          hOutEp,
                                  void                  *pAcObj);

/** ============================================================================
 *   @n@b  AC_reqGetMinimum
 *
 *   @b Description
 *   @n This function handles the get minimum request from host
 *
 *   @b Arguments
 *   @verbatim
            devNum       - USB device number
            usbSetup     - USb setup structure
            hInEp        - USB ISO In end point handle
            hOutEp       - USB ISO Out end point object
            pAcObj       - USB Audio Class object
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_AC_REQUEST_GET_ACK  - Get acknowledgement for request
 *   @li        CSL_AC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  AC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends requested minimum value
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_AcInitStructApp    acAppHandle;
             CSL_AcObject           *pAcHandle;
             CSL_AcCtrlObject       *pCtrlHandle;
             pUsbEpHandle           hUbOutEp;
             Uint16                 lunNum;
             Uint16                 acAppBuffer[420];
             CSL_Status             status;
             Uint16                 request;

             // Assign values to all the members in acAppHandle struct
             acAppHandle.pAcObj = &acAppBuffer[0];
             .....
             .....
             status = AC_Open(&acAppHandle);
             ....
             ....
             pCtrlHandle->fpRequestHandler = AC_lookupReqHandler(request,
                                                       pCtrlHandle->acReqTable);

             pCtrlHandle->fpRequestHandler(pCtrlHandle->devNum,
                                           &pCtrlHandle->usbSetup,hCtrlEpIn,hCtrlEpOut,pAcObj);
     @endverbatim
 *  ============================================================================
 */
CSL_AcRequestRet AC_reqGetMinimum(CSL_UsbDevNum         devNum,
                                  CSL_UsbSetupStruct    *usbSetup,
                                  pUsbEpHandle          hInEp,
                                  pUsbEpHandle          hOutEp,
                                  void                  *pAcObj);

/** ============================================================================
 *   @n@b  AC_reqGetMaximum
 *
 *   @b Description
 *   @n This function handles the get maximum request from host
 *
 *   @b Arguments
 *   @verbatim
            devNum       - USB device number
            usbSetup     - USb setup structure
            hInEp        - USB ISO In end point handle
            hOutEp       - USB ISO Out end point object
            pAcObj       - USB Audio Class object
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_AC_REQUEST_GET_ACK  - Get acknowledgement for request
 *   @li        CSL_AC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  AC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends requested maximum value
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_AcInitStructApp    acAppHandle;
             CSL_AcObject           *pAcHandle;
             CSL_AcCtrlObject       *pCtrlHandle;
             pUsbEpHandle           hUbOutEp;
             Uint16                 lunNum;
             Uint16                 acAppBuffer[420];
             CSL_Status             status;
             Uint16                 request;

             // Assign values to all the members in acAppHandle struct
             acAppHandle.pAcObj = &acAppBuffer[0];
             .....
             .....
             status = AC_Open(&acAppHandle);
             ....
             ....
             pCtrlHandle->fpRequestHandler = AC_lookupReqHandler(request,
                                                       pCtrlHandle->acReqTable);

             pCtrlHandle->fpRequestHandler(pCtrlHandle->devNum,
                                           &pCtrlHandle->usbSetup,hCtrlEpIn,hCtrlEpOut,pAcObj);
     @endverbatim
 *  ============================================================================
 */
CSL_AcRequestRet AC_reqGetMaximum(CSL_UsbDevNum         devNum,
                                  CSL_UsbSetupStruct    *usbSetup,
                                  pUsbEpHandle          hInEp,
                                  pUsbEpHandle          hOutEp,
                                  void                  *pAcObj);

/** ============================================================================
 *   @n@b  AC_reqGetResolution
 *
 *   @b Description
 *   @n This function handles the get resolution request from host
 *
 *   @b Arguments
 *   @verbatim
            devNum       - USB device number
            usbSetup     - USb setup structure
            hInEp        - USB ISO In end point handle
            hOutEp       - USB ISO Out end point object
            pAcObj       - USB Audio Class object
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_AC_REQUEST_GET_ACK  - Get acknowledgement for request
 *   @li        CSL_AC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  AC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends requested resolution value
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_AcInitStructApp    acAppHandle;
             CSL_AcObject           *pAcHandle;
             CSL_AcCtrlObject       *pCtrlHandle;
             pUsbEpHandle           hUbOutEp;
             Uint16                 lunNum;
             Uint16                 acAppBuffer[420];
             CSL_Status             status;
             Uint16                 request;

             // Assign values to all the members in acAppHandle struct
             acAppHandle.pAcObj = &acAppBuffer[0];
             .....
             .....
             status = AC_Open(&acAppHandle);
             ....
             ....
             pCtrlHandle->fpRequestHandler = AC_lookupReqHandler(request,
                                                       pCtrlHandle->acReqTable);

             pCtrlHandle->fpRequestHandler(pCtrlHandle->devNum,
                                           &pCtrlHandle->usbSetup,hCtrlEpIn,hCtrlEpOut,pAcObj);
     @endverbatim
 *  ============================================================================
 */
CSL_AcRequestRet AC_reqGetResolution(CSL_UsbDevNum         devNum,
                                     CSL_UsbSetupStruct    *usbSetup,
                                     pUsbEpHandle          hInEp,
                                     pUsbEpHandle          hOutEp,
                                     void                  *pAcObj);

/** ============================================================================
 *   @n@b  AC_reqSetInterface
 *
 *   @b Description
 *   @n This function handles the request from host to set interafce
 *   unit number
 *
 *   @b Arguments
 *   @verbatim
             devNum       - USB device number
             usbSetup     - USb setup structure
             hInEp        - USB ISO In end point handle
             hOutEp       - USB ISO Out end point object
             pAcObj       - USB Audio Class object
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_AC_REQUEST_SEND_ACK - Send acknowledgement for request
 *   @li        CSL_AC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  AC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends acknowledgement
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_AcInitStructApp    acAppHandle;
             CSL_AcObject           *pAcHandle;
             CSL_AcCtrlObject       *pCtrlHandle;
             pUsbEpHandle           hUbOutEp;
             Uint16                 lunNum;
             Uint16                 acAppBuffer[420];
             CSL_Status             status;
             Uint16                 request;

             // Assign values to all the members in acAppHandle struct
             acAppHandle.pAcObj = &acAppBuffer[0];
             .....
             .....
             status = AC_Open(&acAppHandle);
             ....
             ....
             pCtrlHandle->fpRequestHandler = AC_lookupReqHandler(request,
                                                       pCtrlHandle->acReqTable);

             pCtrlHandle->fpRequestHandler(pCtrlHandle->devNum,
                                           &pCtrlHandle->usbSetup,hCtrlEpIn,hCtrlEpOut,pAcObj);
     @endverbatim
 *  ============================================================================
 */
CSL_AcRequestRet AC_reqSetInterface(CSL_UsbDevNum         devNum,
                                    CSL_UsbSetupStruct    *usbSetup,
                                    pUsbEpHandle          hInEp,
                                    pUsbEpHandle          hOutEp,
                                    void                  *pAcObj);

/**
@} */



#ifdef __cplusplus
}
#endif

#endif    // _CSL_AUDIOCLASS_H_

