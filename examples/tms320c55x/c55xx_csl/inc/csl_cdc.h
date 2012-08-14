/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008, 2011
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_cdc.h
 *
 *  @brief USB CDC functional layer API header file
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 02-May-2011 Created
 * ============================================================================
 */

/** @defgroup CSL_CDC_API CDC
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 * USB Communication Device Class layer provides interface for the host machine to
 * communicate with the virtual COM port through USB.
 *
 * @subsection References
 * USB CDC driver
 * usb_20.pdf
 * usbcdc11.pdf
 */

#ifndef _CSL_CDC_H_
#define _CSL_CDC_H_

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
@defgroup CSL_CDC_SYMBOL  USB CDC Symbols Defined
@ingroup CSL_CDC_API
*/
/**
@defgroup CSL_CDC_DATASTRUCT  USB CDC Data Structures
@ingroup CSL_CDC_API
*/
/**
@defgroup CSL_CDC_FUNCTION  USB CDC Functions
@ingroup CSL_CDC_API
*/
/**
@defgroup CSL_CDC_ENUM USB CDC Enumerated Data Types
@ingroup CSL_CDC_API
*/


/*****************************************************************************\
          USB CDC global macro declarations                                   *
\*****************************************************************************/
/**
@addtogroup CSL_CDC_SYMBOL
@{
*/
/** Common Constants                                                        */
#define CSL_CDC_PORT_MAX                                1
#define CSL_CDC_NO_ENDPTOBJECT                            8

/** Error Code base                                                          */
#define CSL_CDC_ERROR_BASE                             (CSL_EUSB_FIRST)
/** Returned when the CDC request handler is invalid                         */
#define CSL_CDC_INVALID_REQUEST_HANDLER                (CSL_CDC_ERROR_BASE - 1)

/** \brief Symbolic constants for USB CDC Endpoints                          */
#define CSL_CDC_INTR_IN_EP                                    CSL_USB_EP1
#define CSL_CDC_BULK_OUT_EP                                    CSL_USB_EP2
#define CSL_CDC_BULK_IN_EP                                    CSL_USB_EP3

#define CSL_USB_CDC_DATA_BUF_SIZE                            512

/** \brief Symbolic constants for USB CDC requests                          */
/** Host request to issue a command in the format of the supported control protocol  */
/** (bmRequestType<<8) | bRequest                                            */
#define CSL_CDC_REQUEST_SEND_ENCAPSULATED_COMMAND            (0x2100)
/** Host requests a response in the format of the supported control protocol   */
#define CSL_CDC_REQUEST_GET_ENCAPSULATED_RESPONSE            (0xA101)
/** Host controls the settings for a particular communication feature    */
#define CSL_CDC_REQUEST_SET_COMM_FEATURE                    (0x2102)
/** Device returns the current settings for the communication feature  */
#define CSL_CDC_REQUEST_GET_COMM_FEATURE                    (0xA103)
//** Host clears the settings for a particular communication feature.  */
#define CSL_CDC_REQUEST_CLEAR_COMM_FEATURE                    (0x2104)
/** Host request to set line coding                                               */
#define CSL_CDC_REQUEST_SET_LINE_CODING                        (0x2120)
/** Host request to get line coding                                               */
#define CSL_CDC_REQUEST_GET_LINE_CODING                        (0xA121)
/** Host request to set line coding state                                               */
#define CSL_CDC_REQUEST_SET_CONTROL_LINE_STATE                (0x2122)
//** Sends special carrier modulation used to specify RS-232 style break    */
#define CSL_CDC_REQUEST_SEND_BREAK                            (0x2123)

/** \brief CDC Request arguments                                              */
#define CSL_CDC_REQUEST_ARGS     CSL_UsbDevNum         devNum,           \
                                 CSL_UsbSetupStruct    *usbSetup,        \
                                 pUsbEpHandle          hInEp,            \
                                 pUsbEpHandle          hOutEp,           \
                                 void                  *pCdc

/**
@} */


/**************************************************************************\
* USB CDC global typedef declarations                                      *
\**************************************************************************/

/** @addtogroup CSL_CDC_ENUM
 @{ */

/**
 *  \brief This Enum defines the Cdc request return values
 */
typedef enum {
    /* Don't call request again until new SETUP                              */
    CSL_CDC_REQUEST_DONE = 0,
    /* STALL the control endpoint                                            */
    CSL_CDC_REQUEST_STALL,
    /* Send a 0 length IN packet                                             */
    CSL_CDC_REQUEST_SEND_ACK,
    /* Prepare to receive 0 length OUT packet                                */
    CSL_CDC_REQUEST_GET_ACK,
    /* Notify handler when IN data has been transmitted                      */
    CSL_CDC_REQUEST_DATA_IN,
    /* Notify handler when OUT data has been received                        */
    CSL_CDC_REQUEST_DATA_OUT
} CSL_CdcRequestRet;

/**
 *  \brief This Enum defines the Cdc media access status
 */
typedef enum {
    /** Media over flow error                                                */
    CSL_CDC_MEDIACCESS_OVERFLOW,
    /** Media Access success                                                 */
    CSL_CDC_MEDIACCESS_SUCCESS,
    /** Media not present on the system                                      */
    CSL_CDC_MEDIACCESS_NOTPRESENT,
    /** Media is write protected                                             */
    CSL_CDC_MEDIACCESS_WRITEPROTECT,
    /** Media is found as bad                                                */
    CSL_CDC_MEDIACCESS_BADMEDIA
} CSL_CdcMediaStatus;

/**
@} */


/**************************************************************************\
* USB CDC global data structure declarations                               *
\**************************************************************************/

/**
\addtogroup CSL_CDC_DATASTRUCT
@{ */


/**
 *  \brief CDC line coding structure
 *
 *  This structure holds the line coding for the CDC
 */
typedef struct {
    Uint32 dwBaudRate;
    Uint16 wCharFormat;
    Uint16 wParityType;
    Uint16 wDataBits;
} CSL_CdcLineCoding;

/**
 *  \brief CDC application data structure
 *
 *  Holds the structure used by the application to fill in
 *  application specific data handler
 */
typedef struct CSL_CdcInitStructApp {
    /** \brief  Pointer to CDC Class Handle defined in CDC Module           */
    void                    *pCdcObj;
    /** \brief  Device Descriptor units                                          */
    Uint16                  pId;
    /** \brief  Device Descriptor units                                         */
    Uint16                  vId;
    /** \brief  Buffer ptr passed by the system to the module in order to
                save the received data from the host.                        */
    Uint16                  *rxBufferPtr;
    Uint16                    rxStartIdx;
    Uint16                    rxEndIdx;
    /** \brief  Buffer ptr passed by the system to the module in order to
                save the transmitting data to the host.                        */
    Uint16                  *txBufferPtr;
    Uint16                    txStartIdx;
    Uint16                    txEndIdx;
    /** \brief  Buffer ptr passed by the system to the module for USB
                transmitting working buffer.                        */
    Uint16                  *txWorkBufPtr;
    Uint16                    txWorkBufIdx;
    /** \brief  Pointer to the Bulk Function Handler which is
                needed during Configuration of the Bulk Endpoint            */
    void                    (*bulkHandler)();
    /** \brief  Pointer to the Control Function Handler which is
                needed during Configuration of the Control Endpoint            */
    void                    (*ctrlHandler)();
    /** \brief  Pointer to the Interrupt Function Handler which is
                needed during Configuration of the Interrupt Endpoint        */
    void                    (*intrHandler)();
    /** \brief  Pointer to the Data Bulk IN Callback Function                 */
    void                    (*bulkInCallback)();
    /** \brief  Pointer to the Data Bulk OUT Callback Function                 */
    void                    (*bulkOutCallback)();
    /** \brief  Pointer to the Communication Interrupt IN Callback Function */
    void                    (*intrInCallback)();
} CSL_CdcInitStructApp;

/** \brief  MSc application class handle                                     */
typedef CSL_CdcInitStructApp  *pCdcAppClassHandle;

/**
 *  \brief CDC application Logical unit attribute structure
 *
 *  Holds the attributes of every logical unit. This structure
 *  should be filled by the application.
 */
/** \brief  CDC request handler function pointer                             */
typedef  CSL_CdcRequestRet (*fpCDC_REQ_HANDLER)(CSL_CDC_REQUEST_ARGS);

/**
 *  \brief CDC Mass Storage Object structure
 *
 *  Holds all the components for the CDC Object
 */
typedef struct CSL_CdcObject {
    /** \brief  Data buffer pointer used to transfer data to/from Media/usb  */
    Uint16                *lbaBuffer;
    /** \brief  Flag to Indicate whether any Media has been Accessed         */
    Uint16                activityPresentFlag;
    /** \brief  Array used to store the elements of CSW                      */
    /** \brief  Bulk In Endpoint Object                                      */
    CSL_UsbEpObj          bulkInEpObj;
    /** \brief  Bulk Out Endpoint Object                                     */
    CSL_UsbEpObj          bulkOutEpObj;
    /** \brief  Bulk In Endpoint Object                                      */
    CSL_UsbEpObj          intrInEpObj;
    /** \brief  Bulk Out Endpoint Object                                     */
    CSL_UsbEpObj          intrOutEpObj;
    /** \brief Line Coding Object                                            */
    CSL_CdcLineCoding    lineCoding;
    /** \brief Control Line State Object                                    */
    Uint16                controlLineState;
} CSL_CdcObject;

/**
 *  \brief CDC Control Object structure
 *
 *  Holds all the components for the CDC Control Object
 */
typedef struct CSL_CdcCtrlObject {
    /** \brief  Control In Endpoint Object                                   */
    CSL_UsbEpObj             ctrlInEpObj;
    /** \brief  Object of Type Device Number                                 */
    CSL_UsbDevNum            devNum;
    /** \brief  Control Out Endpoint Object                                  */
    CSL_UsbEpObj             ctrlOutEpObj;
    /** \brief  Flag Used internally by APIs                                 */
      Uint16                   suspendFlag;
      /** \brief  NULL terminated array of Endpoint Objects                    */
    pUsbEpHandle             hEpObjArray[CSL_USB_ENDPOINT_COUNT];
    /** \brief  Setup Structure                                              */
    CSL_UsbSetupStruct       usbSetup;
    /** \brief  Dummy descriptor array                                       */
    /** \brief  Current Device Configuration Status                          */
    Uint16                   curConfigStat;
    /** \brief  USB speed configuration
                usbSpeedCfg - 1; High speed
                usbSpeedCfg - 0; Full speed                                  */
    Bool                     usbSpeedCfg;
} CSL_CdcCtrlObject;

/**
 *  \brief CDC Control Object structure
 *
 *  Holds all the components for the CDC Control Object
 */
typedef struct CSL_CdcClassStruct {
    /** \brief  Handle to Control Object                                      */
    CSL_CdcCtrlObject    ctrlHandle;
    /** \brief  Handle to CDC Transfer Object                                */
    CSL_CdcObject        cdcHandle;
} CSL_CdcClassStruct;

/** \brief  CDC class handle                                                  */
typedef CSL_CdcClassStruct  *pCdcClassHandle;

extern CSL_UsbContext     gUsbContext;
extern CSL_UsbRegsOvly    usbRegisters;

/**
@} */

/**************************************************************************\
* USB CDC function declarations                                            *
\**************************************************************************/

/** @addtogroup CSL_CDC_FUNCTION
 @{ */

/** ============================================================================
 *   @n@b CDC_Open
 *
 *   @b Description
 *   @n This function Opens the Cdc module. It initializes Cdc and Ctrl handles
 *      with the data given by the application.
 *
 *      NOTE: THIS FUNCTION CONFIGURES USB CDC MODULE IN HIGH SPEED MODE
 *
 *   @b Arguments
 *   @verbatim
            pAppClassHandle      Application handle to the CDC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li        CSL_SOK                   - Returned for success
 *   @li        CSL_ESYS_BADHANDLE        - Invalid Cdc handle
 *   @li        CSL_ESYS_BADMEDIA         - Media is Bad - Format needed
 *   @li        CSL_ESYS_MEDIA_NOTPRESENT - Media is not present
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes Cdc and Ctrl handles
 *
 *   @b Modifies
 *   @n Cdc and Ctrl handle data
 *
 *   @b Example
 *   @verbatim
            CSL_CdcInitStructApp    cdcAppHandle;
            pCdcAppClassHandle      pAppClassHandle;
            Uint16                  cdcAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &cdcAppHandle;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcAppBuffer[0];
            .....
            .....
            status = CDC_Open(pAppClassHandle);
     @endverbatim
 *  ============================================================================
 */
CSL_Status CDC_Open(pCdcAppClassHandle    pAppClassHandle);

/** ============================================================================
 *   @n@b CDC_OpenFullspeed
 *
 *   @b Description
 *   @n This function Opens the Cdc module for fullspeed mode operation. It
 *      initializes Cdc and Ctrl handles with the data given by the application
 *
 *      NOTE: THIS FUNCTION CONFIGURES USB CDC MODULE IN FULLSPEED MODE
 *
 *   @b Arguments
 *   @verbatim
            pAppClassHandle      Application handle to the CDC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li        CSL_SOK                   - Returned for success
 *   @li        CSL_ESYS_BADHANDLE        - Invalid Cdc handle
 *   @li        CSL_ESYS_BADMEDIA         - Media is Bad - Format needed
 *   @li        CSL_ESYS_MEDIA_NOTPRESENT - Media is not present
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes Cdc and Ctrl handles
 *
 *   @b Modifies
 *   @n Cdc and Ctrl handle data
 *
 *   @b Example
 *   @verbatim
            CSL_CdcInitStructApp    cdcAppHandle;
            pCdcAppClassHandle      pAppClassHandle;
            Uint16                  cdcAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &cdcAppHandle;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcAppBuffer[0];
            .....
            .....
            status = CDC_OpenFullspeed(pAppClassHandle);
     @endverbatim
 *  ============================================================================
 */
CSL_Status CDC_OpenFullspeed(pCdcAppClassHandle    pAppClassHandle);

/** ============================================================================
 *   @n@b CDC_Setup
 *
 *   @b Description
 *   @n This function used to setup the Cdc and Ctrl handles. This is an internal
 *      function and is called from CDC_Open or CDC_OpenFullspeed functions.
 *
 *   @b Arguments
 *   @verbatim
            pAppClassHandle      Application handle to the CDC Class Obj
            usbSpeedCfg          USB speed selection parameter
                                 0 - USB CDC operates in fullspeed mode
                                 1 - USB CDC operates in highspeed mode
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li        CSL_SOK                   - Returned for success
 *   @li        CSL_ESYS_BADHANDLE        - Invalid Cdc handle
 *   @li        CSL_ESYS_BADMEDIA         - Media is Bad - Format needed
 *   @li        CSL_ESYS_MEDIA_NOTPRESENT - Media is not present
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes Cdc and Ctrl handles
 *
 *   @b Modifies
 *   @n Cdc and Ctrl handle data
 *
 *   @b Example
 *   @verbatim
            CSL_CdcInitStructApp    cdcAppHandle;
            pCdcAppClassHandle      pAppClassHandle;
            Uint16                  cdcAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &cdcAppHandle;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcAppBuffer[0];
            .....
            .....
            // Setup USB for highspeed
            status = CDC_Setup(pAppClassHandle, TRUE);
     @endverbatim
 *  ============================================================================
 */
static CSL_Status CDC_Setup(pCdcAppClassHandle    pAppClassHandle,
                            Bool                  usbSpeedCfg);

/** ============================================================================
 *   @n@b CDC_Close
 *
 *   @b Description
 *   @n This function Opens the Cdc module.
 *
 *   @b Arguments
 *   @verbatim
            pAppClassHandle       Application handle to the CDC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK              - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE   - Invalid Cdc class handle
 *
 *   <b> Pre Condition </b>
 *   @n  CDC module should be opened using CDC_Open/CDC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Closes the Cdc module
 *
 *   @b Modifies
 *   @n Cdc and Ctl handles
 *
 *   @b Example
 *   @verbatim
            CSL_CdcInitStructApp    cdcAppHandle;
            pCdcAppClassHandle      pAppClassHandle;
            Uint16                  cdcAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &cdcAppHandle;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcAppBuffer[0];
            .....
            .....
            status = CDC_Open(pAppClassHandle);
            ....
            ....
            status = CDC_Close(pAppClassHandle);
     @endverbatim
 *  ============================================================================
 */
CSL_Status CDC_Close(pCdcAppClassHandle    pAppClassHandle);

/** ============================================================================
 *   @n@b CDC_GetactivityFlagStatus
 *
 *   @b Description
 *   @n This function gives the status of the activity Flag
 *
 *   @b Arguments
 *   @verbatim
            pCDC        Handle to the CDC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  - Status of the activity flag
 *   @li                     '1' - If the activity is present
 *   @li                     '0' - If the activity is not present
 *
 *   <b> Pre Condition </b>
 *   @n  CDC module should be opened using CDC_Open/CDC_OpenFullspeed function
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
            CSL_CdcInitStructApp    cdcAppHandle;
            pCdcAppClassHandle      pAppClassHandle;
            Uint16                  cdcAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &cdcAppHandle;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcAppBuffer[0];
            .....
            .....
            status = CDC_Open(pAppClassHandle);
            ....
            ....
            activityFlagStat = CDC_GetactivityFlagStatus(pAppClassHandle->pCdcObj);
     @endverbatim
 *  ============================================================================
 */
Uint16 CDC_GetactivityFlagStatus(void    *pCDC);

/** ============================================================================
 *   @n@b CDC_GetsuspendFlagStatus
 *
 *   @b Description
 *   @n This function gives the status of the suspend Flag
 *
 *   @b Arguments
 *   @verbatim
            pCDC        Handle to the CDC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  - Status of the suspend flag
 *   @li                     '1' - If the device is suspended
 *   @li                     '0' - If the device is not suspended
 *
 *   <b> Pre Condition </b>
 *   @n  CDC module should be opened using CDC_Open/CDC_OpenFullspeed function
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
            CSL_CdcInitStructApp    cdcAppHandle;
            pCdcAppClassHandle      pAppClassHandle;
            Uint16                  cdcAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &cdcAppHandle;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcAppBuffer[0];
            .....
            .....
            status = CDC_Open(pAppClassHandle);
            ....
            ....
            suspendFlagStat = CDC_GetsuspendFlagStatus(pAppClassHandle->pCdcObj);
     @endverbatim
 *  ============================================================================
 */
Uint16 CDC_GetsuspendFlagStatus(void    *pCDC);

/** ============================================================================
 *   @n@b CDC_eventHandler
 *
 *   @b Description
 *   @n This function handles all the CDC ACM related requests
 *
 *   @b Arguments
 *   @verbatim
            pContext         Pointer to the USBContext structure
            hCdcClass        Handle to the CDC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  - Status of the request (CSL_CdcRequestRet)
 *   @li    CSL_CDC_REQUEST_DONE (0) - CDC request process done
 *   @li    CSL_CDC_REQUEST_STALL (1) - STALL the control endpoint
 *   @li    CSL_CDC_REQUEST_SEND_ACK (2) - Prepare to receive 0 length OUT packet
 *   @li    CSL_CDC_REQUEST_DATA_IN (0) - Notify handler when IN data has been transmitted
 *   @li    CSL_CDC_REQUEST_DATA_OUT (0) - Notify handler when OUT data has been received
 *
 *   <b> Pre Condition </b>
 *   @n  CDC module should be opened using CDC_Open/CDC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Returns request processing status
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_CdcInitStructApp    cdcAppHandle;
            CSL_CdcClassStruct      cdcClassStruct;
            CSL_Status              status;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcClassStruct;
            .....
            .....
            status = CDC_Open(pAppClassHandle);
            ....
            ....

            in USB ISR:
            ....
            ....
            if(pContext->dwIntSourceL & CSL_USB_TX_RX_INT_EP0)
            {
                // call USB core EP0 event handler to process
                USB_coreEventProcessEp0(pContext);

                // call the CDC event handler for further processing
                CDC_eventHandler(pContext, &cdcClassStruct);
            }
            ....
            ....
     @endverbatim
 *  ============================================================================
 */
CSL_CdcRequestRet CDC_eventHandler(pUsbContext pContext, CSL_CdcClassStruct *hCdcClass);

/** ============================================================================
 *   @n@b CDC_bulkOutHandler
 *
 *   @b Description
 *   @n This function handles all the CDC ACM bulk OUT data transactions
 *
 *   @b Arguments
 *   @verbatim
            pContext         Pointer to the USBContext structure
            hCdcClass        Handle to the CDC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  - Status of the processing (CSL_Status)
 *   @li    CSL_SOK                    (0)         Success
 *   @li    CSL_ESYS_FAIL              (-1)        Generic failure
 *   @li    CSL_ESYS_BADHANDLE         (-5)        Handle passed to CSL was invalid
 *   @li    CSL_ESYS_INVPARAMS         (-6)        Invalid parameters
 *
 *   <b> Pre Condition </b>
 *   @n  CDC module should be opened using CDC_Open/CDC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Returns request processing status
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_CdcInitStructApp    cdcAppHandle;
            CSL_CdcClassStruct      cdcClassStruct;
            CSL_Status              status;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcClassStruct;
            .....
            .....
            status = CDC_Open(pAppClassHandle);
            ....
            ....

            in USB ISR:
            ....
            ....
            if (pContext->dwIntSourceL & (1<<(CSL_CDC_BULK_OUT_EP+CSL_USB_IN_EP0)))
            {
                // Handle the CDC BULK OUT
                CDC_bulkOutHandler(pContext, &CDC_AppHandle);
            }
            ....
            ....
     @endverbatim
 *  ============================================================================
 */
CSL_Status CDC_bulkOutHandler(pUsbContext pContext, CSL_CdcInitStructApp *cdcAppHandle);

/** ============================================================================
 *   @n@b CDC_bulkInHandler
 *
 *   @b Description
 *   @n This function handles all the CDC ACM bulk IN data transactions
 *
 *   @b Arguments
 *   @verbatim
            pContext         Pointer to the USBContext structure
            hCdcClass        Handle to the CDC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>  - Status of the processing (CSL_Status)
 *   @li    CSL_SOK                    (0)         Success
 *   @li    CSL_ESYS_FAIL              (-1)        Generic failure
 *   @li    CSL_ESYS_BADHANDLE         (-5)        Handle passed to CSL was invalid
 *   @li    CSL_ESYS_INVPARAMS         (-6)        Invalid parameters
 *
 *   <b> Pre Condition </b>
 *   @n  CDC module should be opened using CDC_Open/CDC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Returns request processing status
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_CdcInitStructApp    cdcAppHandle;
            CSL_CdcClassStruct      cdcClassStruct;
            CSL_Status              status;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcClassStruct;
            .....
            .....
            status = CDC_Open(pAppClassHandle);
            ....
            ....

            in USB ISR:
            ....
            ....
            if (pContext->dwIntSourceL & (1<<CSL_CDC_BULK_IN_EP))
            {
                // Handle the CDC BULK IN
                ///CDC_bulkInHandler(pContext, &CDC_AppHandle);
            }
            ....
            ....
     @endverbatim
 *  ============================================================================
 */
CSL_Status CDC_bulkInHandler(pUsbContext pContext, CSL_CdcInitStructApp *cdcAppHandle);

/** ============================================================================
 *   @n@b CDC_getData
 *
 *   @b Description
 *   @n This function receives data from the BULK OUT
 *
 *   @b Arguments
 *   @verbatim
            pContext         Pointer to the USBContext structure
            hCdcClass        Handle to the CDC Class Obj
            data            the data to be transmitted
     @endverbatim
 *
 *   <b> Return Value </b>  - number of byte received
 *   @li    0 - means the BULK OUT has no data avaiable
 *   @li    N > 0 - means N byte data has been received
 *
 *   <b> Pre Condition </b>
 *   @n  CDC module should be opened using CDC_Open/CDC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Returns request processing status
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_CdcInitStructApp    cdcAppHandle;
            CSL_CdcClassStruct      cdcClassStruct;
            CSL_Status              status;
            Uint16                    uartData[80]
            Uint16                    uartdataSize;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcClassStruct;
            .....
            .....
            status = CDC_Open(pAppClassHandle);
            ....
            ....
            // Echo the input from the host back to the host
            // Get data from the host
            uartDataSize = CDC_getData(pContext, &CDC_AppHandle, uartData);
            // Echo it back, if there is an input
            if (uartDataSize>0)
            {
                do
                {
                    // CDC_putData will return 0 if the TX is not ready for the transaction
                    retNum = CDC_putData(pContext, &CDC_AppHandle, uartData, uartDataSize);
                } while (retNum==0);
            }
            ....
            ....
     @endverbatim
 *  ============================================================================
 */
 Uint16 CDC_getData(pUsbContext pContext, CSL_CdcInitStructApp *cdcAppHandle, Uint16 *data);

/** ============================================================================
 *   @n@b CDC_putData
 *
 *   @b Description
 *   @n This function sends out the data to the BULK IN
 *
 *   @b Arguments
 *   @verbatim
            pContext         Pointer to the USBContext structure
            hCdcClass        Handle to the CDC Class Obj
            data            the data to be transmitted
            size            the data size
     @endverbatim
 *
 *   <b> Return Value </b>  - number of byte sent out
 *   @li    0 - means the BULK IN is not ready for transmission
 *   @li    N > 0 - means the transmission is done and N byte data has been sent out
 *
 *   <b> Pre Condition </b>
 *   @n  CDC module should be opened using CDC_Open/CDC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Returns request processing status
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_CdcInitStructApp    cdcAppHandle;
            CSL_CdcClassStruct      cdcClassStruct;
            CSL_Status              status;
            Uint16                    uartData[80]
            Uint16                    uartdataSize;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcClassStruct;
            .....
            .....
            status = CDC_Open(pAppClassHandle);
            ....
            ....
            // Echo the input from the host back to the host
            // Get data from the host
            uartDataSize = CDC_getData(pContext, &CDC_AppHandle, uartData);
            // Echo it back, if there is an input
            if (uartDataSize>0)
            {
                do
                {
                    // CDC_putData will return 0 if the TX is not ready for the transaction
                    retNum = CDC_putData(pContext, &CDC_AppHandle, uartData, uartDataSize);
                } while (retNum==0);
            }
            ....
            ....
     @endverbatim
 *  ============================================================================
 */
Uint16 CDC_putData(pUsbContext pContext, CSL_CdcInitStructApp *cdcAppHandle, Uint16 *data, Uint16 size);

/**
@} */



#ifdef __cplusplus
}
#endif

#endif    //_CSL_CDC_H_

