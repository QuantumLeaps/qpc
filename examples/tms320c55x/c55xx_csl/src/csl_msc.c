/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_msc.c
 *
 *  @brief USB MSC functional layer API source file
 *
 *  Path: \(CSLPATH)\ src
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

#include <string.h>

#include <csl_msc.h>
#include <csl_mscAux.h>

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
CSL_Status MSC_Open(pMscAppClassHandle    pAppClassHandle)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(pAppClassHandle != NULL)
    {
        status = MSC_Setup(pAppClassHandle, TRUE);
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

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
CSL_Status MSC_OpenFullspeed(pMscAppClassHandle    pAppClassHandle)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(pAppClassHandle != NULL)
    {
        status = MSC_Setup(pAppClassHandle, FALSE);
        USB_setFullSpeedMode(CSL_USB_EP0_PACKET_SIZE);
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

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
                            Bool                  usbSpeedCfg)
{
    pMscClassHandle       pHandle;
    CSL_MscMediaStatus    mediaStatus;
    CSL_Status            status;
    Uint16                initLun;
    Uint16                eventMask;
    Uint16                maxPktSize;

    status  = CSL_SOK;
    pHandle = (pMscClassHandle)(pAppClassHandle->pMscObj);

    if(pAppClassHandle != NULL)
    {
        /* Initilialising CSL_MscCtrlObject */
        if(pAppClassHandle->numLun > 0)
        {
            pHandle->mscHandle.noOfLun = pAppClassHandle->numLun - 1;
        }
        pHandle->ctrlHandle.devNum = CSL_USB0;

        /* Initializing all default values */
        pHandle->ctrlHandle.suspendFlag   = 0;
        pHandle->ctrlHandle.curConfigStat = 0x0;

        /* hEpObjArray needs to be setup */
        pHandle->ctrlHandle.hEpObjArray[0] = &pHandle->ctrlHandle.ctrlOutEpObj;
        pHandle->ctrlHandle.hEpObjArray[1] = &pHandle->ctrlHandle.ctrlInEpObj;
        pHandle->ctrlHandle.hEpObjArray[2] = &pHandle->mscHandle.bulkOutEpObj;
        pHandle->ctrlHandle.hEpObjArray[3] = &pHandle->mscHandle.bulkInEpObj;
        pHandle->ctrlHandle.hEpObjArray[4] = NULL;

        /* Copying the Application Specific values to the MSC Class Handle */
        pAppClassHandle->strDescrApp[0] =
                         (char*)&pHandle->ctrlHandle.stringDescrLangid[0];

        pHandle->ctrlHandle.strDescr    = &pAppClassHandle->strDescrApp[0];
        pHandle->ctrlHandle.mscReqTable = &pAppClassHandle->mscReqTableApp[0];
        pHandle->ctrlHandle.usbSpeedCfg = usbSpeedCfg;

        /* Initializing the default values for all the Control descriptors */
        status = MSC_initCtrlDesc(&pHandle->ctrlHandle);
        if(status != CSL_SOK)
        {
            return (status);
        }

        /* Copying  user's VID and PID into the device descriptor */
        pHandle->ctrlHandle.deviceDescr[5] = pAppClassHandle->vId;
        pHandle->ctrlHandle.deviceDescr[6] = pAppClassHandle->pId;

        /* Initializing CSL_MscObject */
        pHandle->mscHandle.lbaBuffer     = pAppClassHandle->lbaBufferApp;
        pHandle->mscHandle.readMedia     = pAppClassHandle->readMediaApp;
        pHandle->mscHandle.writeMedia    = pAppClassHandle->writeMediaApp;
        pHandle->mscHandle.mediaGetPresentState =
                                   pAppClassHandle->mediaGetPresentStateApp;
        pHandle->mscHandle.mediaInit     = pAppClassHandle->mediaInitApp;
        pHandle->mscHandle.mediaEject    = pAppClassHandle->mediaEjectApp;
        pHandle->mscHandle.mediaLockUnit = pAppClassHandle->mediaLockUnitApp;
        pHandle->mscHandle.getMediaSize  = pAppClassHandle->getMediaSizeApp;

        /* Initializing the default values for all the MSC stuff */
        status = MSC_initMscVal(&pHandle->mscHandle);
        if(status != CSL_SOK)
        {
            return (status);
        }

        /* Initialized the Control Endpoint OUT 0 */
        eventMask = (CSL_USB_EVENT_RESET | CSL_USB_EVENT_SETUP |
                     CSL_USB_EVENT_SUSPEND | CSL_USB_EVENT_RESUME |
                     CSL_USB_EVENT_RESET | CSL_USB_EVENT_EOT);

        if(usbSpeedCfg == TRUE)
        {
            maxPktSize = CSL_USB_EP1_PACKET_SIZE_HS;
        }
        else
        {
            maxPktSize = CSL_USB_EP1_PACKET_SIZE_FS;
        }

        status = USB_initEndptObj(pHandle->ctrlHandle.devNum,
                                  &pHandle->ctrlHandle.ctrlOutEpObj,
                                   CSL_USB_OUT_EP0,
                                     CSL_USB_CTRL,
                                    CSL_USB_EP0_PACKET_SIZE,
                                    eventMask,
                                    pAppClassHandle->ctrlHandler);
        if(status != CSL_SOK)
        {
            return (status);
        }

        /* Initialized the Control Endpoint IN 0 */
        status = USB_initEndptObj(pHandle->ctrlHandle.devNum,
                                  &pHandle->ctrlHandle.ctrlInEpObj,
                                  CSL_USB_IN_EP0,
                                  CSL_USB_CTRL,
                                  CSL_USB_EP0_PACKET_SIZE,
                                  CSL_USB_EVENT_EOT,
                                  pAppClassHandle->ctrlHandler);
        if(status != CSL_SOK)
        {
            return (status);
        }

        /* Initialized the Bulk Endpoint IN 1 */
        eventMask = (CSL_USB_EVENT_RESET | CSL_USB_EVENT_EOT);
        status = USB_initEndptObj(pHandle->ctrlHandle.devNum,
                                  &pHandle->mscHandle.bulkInEpObj,
                                  CSL_USB_IN_EP1,
                                  CSL_USB_BULK,
                                  maxPktSize,
                                  eventMask,
                                  pAppClassHandle->bulkHandler);
        if(status != CSL_SOK)
        {
            return (status);
        }

        /* Initialized the Bulk Endpoint OUT 2 */
        status = USB_initEndptObj(pHandle->ctrlHandle.devNum,
                                  &pHandle->mscHandle.bulkOutEpObj,
                                  CSL_USB_OUT_EP2,
                                  CSL_USB_BULK,
                                  maxPktSize,
                                  CSL_USB_EVENT_EOT,
                                  pAppClassHandle->bulkHandler);
        if(status != CSL_SOK)
        {
            return (status);
        }

        /* USB API setParams called for setup */
        status = USB_setParams(CSL_USB0, pHandle->ctrlHandle.hEpObjArray, 0);
        if(status != CSL_SOK)
        {
            return (status);
        }

        /* Initializing the logical Units */
        for(initLun = 0; initLun <= pHandle->mscHandle.noOfLun; initLun++)
        {
            mediaStatus = pHandle->mscHandle.mediaInit(initLun);
            /*set the size of the media */
            pHandle->mscHandle.lun[initLun].mediaSize =
                                  (pHandle->mscHandle.getMediaSize(initLun) - 1);
            if(mediaStatus == CSL_MSC_MEDIACCESS_SUCCESS)
            {
                pHandle->mscHandle.lun[initLun].mediaState =
                                                CSL_MSC_MEDIA_PRESENT;
            }
            else if(mediaStatus == CSL_MSC_MEDIACCESS_BADMEDIA)
            {
                pHandle->mscHandle.lun[initLun].mediaState = CSL_MSC_MEDIA_BAD;
                status = CSL_ESYS_BADMEDIA;
            }
            else if(mediaStatus == CSL_MSC_MEDIACCESS_NOTPRESENT)
            {
                pHandle->mscHandle.lun[initLun].mediaState = 0;
                status = CSL_ESYS_MEDIA_NOTPRESENT;
            }
            else
            {
                /* This else is used for future purposes if media init
                returns some other errors */
                pHandle->mscHandle.lun[initLun].mediaState = 0;
            }
        }

        if(status == CSL_SOK)
        {
            /* Media Access Success; Connect the Device */
            USB_connectDev(pHandle->ctrlHandle.devNum);
        }
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

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
CSL_Status MSC_Close(pMscAppClassHandle    pAppClassHandle)
{
    pMscClassHandle       pMscClassHdl;
    CSL_MscCtrlObject     *pCtrlHandle;
    CSL_Status            status;

    pMscClassHdl = (pMscClassHandle)(pAppClassHandle->pMscObj);
    pCtrlHandle  = &pMscClassHdl->ctrlHandle;
    status       = CSL_SOK;

    if(pAppClassHandle != NULL)
    {
        status = USB_disconnectDev(pCtrlHandle->devNum);
        pAppClassHandle->pMscObj = NULL;
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

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
                          Uint16                  lunNum)
{
    pMscClassHandle    pHandle;
    CSL_MscObject      *pMscObj;
    CSL_Status         status;
    Uint16             copyCntdst;
    Uint16             copyCntsrc;

    pHandle = (pMscClassHandle)(pAppClassHandle->pMscObj);
    pMscObj = &pHandle->mscHandle;
    status  = CSL_SOK;

    if((pAppClassHandle != NULL) && (lunHandle != NULL))
    {
        /* lunNo = pMscObj->noOfLun; */
        pMscObj->lun[lunNum].removeable = lunHandle->removeableApp;
        pMscObj->lun[lunNum].readwrite  = lunHandle->readwriteApp;
        /* LBA of last block on media */
        pMscObj->lun[lunNum].mediaSize  = lunHandle->mediaSizeApp - 1;
        pMscObj->lun[lunNum].mediaState = 0;
        pMscObj->lun[lunNum].lbaSize    = lunHandle->lbaSizeApp;

        /* Default values of the first few words */
        pMscObj->lun[lunNum].scsiInquiryData[0] = CSL_SCSI_DATA0_DEFVAL;
        pMscObj->lun[lunNum].scsiInquiryData[1] = CSL_SCSI_DATA1_DEFVAL;
        pMscObj->lun[lunNum].scsiInquiryData[3] = CSL_SCSI_DATA3_DEFVAL;
        pMscObj->lun[lunNum].scsiInquiryData[4] = CSL_SCSI_DATA4_DEFVAL;
        pMscObj->lun[lunNum].scsiInquiryData[5] = CSL_SCSI_DATA5_DEFVAL;

        /* copying the Configurable Items of SCSI Inquiry data */
        copyCntsrc = 0;
        for(copyCntdst = CSL_SCSI_DATA_SIZE1;                                 \
            copyCntdst < CSL_SCSI_DATA_SIZE2; copyCntdst++, copyCntsrc++)
        {
            pMscObj->lun[lunNum].scsiInquiryData[copyCntdst] =
                                 lunHandle->scsiInquiryConfData[copyCntsrc];
        }

        /* bytes 56-57 */
        pMscObj->lun[lunNum].scsiInquiryData[30] = CSL_SCSI_DATA30_DEFVAL;
        /* Version descriptor 1 = RBC */
        pMscObj->lun[lunNum].scsiInquiryData[31] = CSL_SCSI_DATA31_DEFVAL;
        /* Version descriptor 2 = RBC r10a */
        pMscObj->lun[lunNum].scsiInquiryData[32] = CSL_SCSI_DATA32_DEFVAL;

        for (copyCntdst = CSL_SCSI_DATA_SIZE3;                                 \
             copyCntdst < CSL_SCSI_DATA_SIZE4; copyCntdst++)
        {
            pMscObj->lun[lunNum].scsiInquiryData[copyCntdst] =
                                            CSL_SCSI_DATA_NULL_VAL;
        }

        /* Setting the RMB bit of SCSI Inquiry Data */
        if (pMscObj->lun[lunNum].removeable)
        {
            pMscObj->lun[lunNum].scsiInquiryData[2] = CSL_SCSI_DATA2_DEFVAL;
        }
        else
        {
            pMscObj->lun[lunNum].scsiInquiryData[2] = CSL_SCSI_DATA_NULL_VAL;
        }
        pMscObj->lun[lunNum].verifyFlag = CSL_MSC_VERIFY_PASSED;
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

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
CSL_Status MSC_Ctrl(void    *pMSC)
{
    pMscClassHandle       pMscClassHdl;
    CSL_MscCtrlObject*    pCtrlHandle;
    CSL_MscRequestRet     reqHandlerRet;
    CSL_UsbEventMask      usbCtrlEvents;
    pUsbEpHandle          hCtrlEpIn;
    pUsbEpHandle          hCtrlEpOut;
    CSL_Status            status;
    Uint16                request;

    if(pMSC == NULL)
    {
        return(CSL_ESYS_BADHANDLE);
    }

    pMscClassHdl  = (pMscClassHandle)(pMSC);
    pCtrlHandle   = &pMscClassHdl->ctrlHandle;
    hCtrlEpIn     = &pCtrlHandle->ctrlInEpObj;
    hCtrlEpOut    = &pCtrlHandle->ctrlOutEpObj;
    reqHandlerRet = CSL_MSC_REQUEST_DONE;

    pCtrlHandle->fpRequestHandler = MSC_reqUnknown;
    /* Finding out which Endpoint caused this function to be called by responding */
    usbCtrlEvents = (USB_getEvents(hCtrlEpOut, &status) |
                     USB_getEvents(hCtrlEpIn, &status));

    /* Check for the RESET event, abort Endpoint Activities and Reconfigure
    the USB Module */
    if(usbCtrlEvents & CSL_USB_EVENT_RESET)
    {
        pCtrlHandle->suspendFlag = FALSE;
        /* Stop all Data transfer Activities */
        status |= USB_abortAllTransaction(pCtrlHandle->devNum);
        /* Reinitialize the Device */
        status |= USB_setParams(pCtrlHandle->devNum,
                                pCtrlHandle->hEpObjArray, FALSE);
        /* Reset device Config No */
        pCtrlHandle->curConfigStat = FALSE;
    }

    if(usbCtrlEvents & CSL_USB_EVENT_SUSPEND)
    {
        pCtrlHandle->suspendFlag = TRUE;
    }

    if(usbCtrlEvents & CSL_USB_EVENT_RESUME)
    {
        pCtrlHandle->suspendFlag = FALSE;
    }

     /* If Event Received is Setup Packet,read the Packet and Lookup the
     Request Table for the Appropriate Request Handler */
     if((usbCtrlEvents & CSL_USB_EVENT_SETUP) == CSL_USB_EVENT_SETUP)
     {

        status |= USB_getSetupPacket(pCtrlHandle->devNum,
                                    &pCtrlHandle->usbSetup, FALSE);
        request = ((((pCtrlHandle->usbSetup.bmRequestType ) <<
                   CSL_MSC_8BIT_SHIFT) & 0xF000) |
                   pCtrlHandle->usbSetup.bRequest);

        pCtrlHandle->fpRequestHandler = MSC_lookupReqHandler(request,
                                                    pCtrlHandle->mscReqTable);

        if(reqHandlerRet == CSL_MSC_REQUEST_DONE)
        {
            reqHandlerRet = (pCtrlHandle->fpRequestHandler)(pCtrlHandle->devNum,
                             &pCtrlHandle->usbSetup,hCtrlEpIn,hCtrlEpOut,pMSC);
        }

        switch(reqHandlerRet)
        {
            /* The Request Handler Routine successfully completed the task,
            so 0 byte ACK is sent */
            case CSL_MSC_REQUEST_SEND_ACK:
                status |= USB_postTransaction(hCtrlEpIn, 0, NULL,
                                              CSL_USB_IOFLAG_NONE);
                pCtrlHandle->fpRequestHandler = MSC_reqUnknown;
                break;

            /* For Future Use */
            case CSL_MSC_REQUEST_DATA_OUT:
                break;

            /* The Request Handler Routine successfully completed the task,
            so 0 byte ACK is received */
            case CSL_MSC_REQUEST_GET_ACK:
                status |= USB_postTransaction(hCtrlEpOut, 0, NULL,
                                              CSL_USB_IOFLAG_NONE);
                pCtrlHandle->fpRequestHandler = MSC_reqUnknown;
                break;

            /* For Future Use */
            case CSL_MSC_REQUEST_DATA_IN:
                break;

            /* Request Handler is Confused about the Setup Packet, so Stall
            the Control Endpoints */
            case CSL_MSC_REQUEST_STALL:
                status |= USB_stallEndpt(hCtrlEpOut);
                status |= USB_stallEndpt(hCtrlEpIn);
                break;

            case CSL_MSC_REQUEST_DONE:
                pCtrlHandle->fpRequestHandler = MSC_reqUnknown;
                break;

            default:
                status = CSL_MSC_INVALID_REQUEST_HANDLER;
                break;
        }

        /* Clear the flags if New Setup Packet is Received */
        if(pCtrlHandle->usbSetup.newPacket)
        {
            pCtrlHandle->usbSetup.newPacket = FALSE;
        }
    }

    return(status);
}

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
CSL_Status MSC_Bulk(void    *pMSC)
{
    pMscClassHandle    pMscClassHdl;
    CSL_MscObject      *pMscHandle;
    pUsbEpHandle       hUsbInEp;
    pUsbEpHandle       hUsbOutEp;
    CSL_Status         status;
    Uint16             logicalUnit;

    status = CSL_SOK;

    if(pMSC != NULL)
    {
        pMscClassHdl = (pMscClassHandle)(pMSC);
        pMscHandle   = &pMscClassHdl->mscHandle;
        hUsbInEp     = &pMscHandle->bulkInEpObj;
        hUsbOutEp    = &pMscHandle->bulkOutEpObj;

        if(USB_getEvents(hUsbInEp, &status) & CSL_USB_EVENT_RESET)
        {
            pMscHandle->storageState = CSL_MSC_STORAGE_STATE_RESET;
        }

        logicalUnit = pMscHandle->cbw[CSL_MSC_CBW_6] >> CSL_MSC_8BIT_SHIFT;

        /* Check the storage state */
        switch(pMscHandle->storageState)
        {
            case CSL_MSC_STORAGE_STATE_RESET:
                status = MSC_HandleStateReset(pMscHandle, hUsbOutEp);
                break;

            case CSL_MSC_STORAGE_STATE_WAIT_FOR_CBW:
                status = MSC_HandleStateWaitForCBW(pMscHandle, hUsbOutEp, hUsbInEp);
                break;

            case CSL_MSC_STORAGE_STATE_SENDING_CSW:
                status = MSC_HandleStateSendCSW(pMscHandle, hUsbOutEp, hUsbInEp);
                break;

            case CSL_MSC_STORAGE_STATE_SENDING_DATA:
                status = MSC_HandleStateSendData(pMscHandle, hUsbInEp);
                break;

            case CSL_MSC_STORAGE_STATE_SENDING_STALL:
                status = MSC_HandleStateSendStall(pMscHandle, hUsbInEp);
                break;

            case CSL_MSC_STORAGE_STATE_READ:
                status = MSC_ExecuteRead(pMscHandle, logicalUnit, FALSE);
                break;

            case CSL_MSC_STORAGE_STATE_WRITE:
                status = MSC_ExecuteWrite(pMscHandle, logicalUnit, FALSE);
                break;

            case CSL_MSC_STORAGE_STATE_SENDING_SHORT_PKT:
                status = MSC_HandleStateSendingShortPkt(pMscHandle, hUsbInEp);
                break;

            default:
                status = CSL_ESYS_INVPARAMS;
                break;
        }
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

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
                              Uint16    lunNum)
{
   pMscClassHandle    pMscClassHdl;
   CSL_MscObject      *pMscHandle;

   pMscClassHdl = (pMscClassHandle)(pMSC);
   pMscHandle   = &pMscClassHdl->mscHandle;

   if (pMscHandle->lun[lunNum].mediaState & CSL_MSC_MEDIA_LOCKED)
   {
       return(TRUE);
   }
   else
   {
       return(FALSE);
   }
}

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
Uint16 MSC_GetactivityFlagStatus(void    *pMSC)
{
   pMscClassHandle    pMscClassHdl;
   CSL_MscObject      *pMscHandle;

   pMscClassHdl = (pMscClassHandle)(pMSC);
   pMscHandle   = &pMscClassHdl->mscHandle;

   return(pMscHandle->activityPresentFlag);
}

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
Uint16 MSC_GetsuspendFlagStatus(void    *pMSC)
{
   pMscClassHandle       pMscClassHdl;
   CSL_MscCtrlObject     *pCtrlHandle;

   pMscClassHdl = (pMscClassHandle)(pMSC);
   pCtrlHandle  = &pMscClassHdl->ctrlHandle;

   return(pCtrlHandle->suspendFlag);
}

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
static CSL_Status MSC_initCtrlDesc(CSL_MscCtrlObject    *pCtrlHandle)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(pCtrlHandle != NULL)
    {
        /*
         * Descriptors are stored in little endian format,
         * because that is the way USB sends data
         */
        /***** Setting up the device descriptor *****/
        /* field for xfer_byte_cnt  */
        pCtrlHandle->deviceDescr[0] = CSL_MSC_DEVICE_DESC0_DEFVAL;
        /* bLength, bDescriptorType */
        pCtrlHandle->deviceDescr[1] = (CSL_MSC_DEVICE_DESCRIPTOR_TYPE <<
                                       CSL_MSC_8BIT_SHIFT) |
                                       CSL_MSC_DEVICE_DESC_DEFLEN;
        /* bcdUSB */
        pCtrlHandle->deviceDescr[2] = CSL_MSC_DEVICE_DESC2_DEFVAL;
        /* bDeviceClass, bDeviceSubClass */
        pCtrlHandle->deviceDescr[3] = CSL_MSC_DEVICE_DESC3_DEFVAL;
        /* bDeviceProtocol, bMaxPacketSize0 */
        pCtrlHandle->deviceDescr[4] = CSL_MSC_DEVICE_DESC4_DEFVAL;
        /* idVendor = Texas Instruments (should be OEM) */
        pCtrlHandle->deviceDescr[5] = CSL_MSC_DEVICE_DESC5_DEFVAL;
        /* idProduct = needs to be allocated */
        pCtrlHandle->deviceDescr[6] = CSL_MSC_DEVICE_DESC6_DEFVAL;
        /* bcdDevice */
        pCtrlHandle->deviceDescr[7] = CSL_MSC_DEVICE_DESC7_DEFVAL;
        /* iManufacturer, iProductName */
        pCtrlHandle->deviceDescr[8] = CSL_MSC_DEVICE_DESC8_DEFVAL;
        /* iSerialNumber, bNumConfigurations */
        pCtrlHandle->deviceDescr[9] = CSL_MSC_DEVICE_DESC9_DEFVAL;

        /***** Setting up the device qualifier descriptor *****/
        pCtrlHandle->deviceQualDescr[0] = CSL_MSC_DEVICE_QUAL_DESC0_DEFVAL;
        /* bLength, bDescriptorType */
        pCtrlHandle->deviceQualDescr[1] = (CSL_MSC_DEVICE_QUAL_DESCRIPTOR_TYPE <<
                                           CSL_MSC_8BIT_SHIFT) |
                                           CSL_MSC_DEVICE_QUAL_DESC_DEFLEN;
        /* bcdUSB */
        pCtrlHandle->deviceQualDescr[2] = CSL_MSC_DEVICE_QUAL_DESC2_DEFVAL;
        /* bDeviceClass, bDeviceSubClass */
        pCtrlHandle->deviceQualDescr[3] = CSL_MSC_DEVICE_QUAL_DESC3_DEFVAL;
        /* bDeviceProtocol, bMaxPacketSize0 */
        pCtrlHandle->deviceQualDescr[4] = CSL_MSC_DEVICE_QUAL_DESC4_DEFVAL;
        /* bNumConfigurations, bReserved */
        pCtrlHandle->deviceQualDescr[5] = CSL_MSC_DEVICE_QUAL_DESC5_DEFVAL;

        /***** Setting up the configuration Descriptor *****/
        pCtrlHandle->confDescr[0] = CSL_MSC_CONF_DESC0_DEFVAL;
        /* bLength, bDescriptorType */
        pCtrlHandle->confDescr[1] = (CSL_MSC_CONFIGURATION_DESCRIPTOR_TYPE <<
                                     CSL_MSC_8BIT_SHIFT) |
                                     CSL_MSC_CONF_DESC_DEFLEN;
        /* wTotalLength */
        pCtrlHandle->confDescr[2] = CSL_MSC_CONF_DESC2_DEFVAL;
        /* bNumInterfaces, bConfigurationValue */
        pCtrlHandle->confDescr[3] = CSL_MSC_CONF_DESC3_DEFVAL;
        /* iConfiguration, bmAttributes */
        pCtrlHandle->confDescr[4] = CSL_MSC_CONF_DESC4_DEFVAL;
        /* bMaxPower */
        pCtrlHandle->confDescr[5] = USB_getMaxPower(CSL_USB0);

        /***** Setting up the Mass Storage Interface Descriptor *****/
        pCtrlHandle->massStorageIntDescr[0] = CSL_MSC_INTERFACE_DESC0_DEFVAL;
        /* bLength, bDescriptorType */
        pCtrlHandle->massStorageIntDescr[1] =
                   (CSL_MSC_INTERFACE_DESCRIPTOR_TYPE << CSL_MSC_8BIT_SHIFT) |
                    CSL_MSC_INTERFACE_DESC_DEFLEN;
        /* bInterfaceNumber, bAlternateSetting */
        pCtrlHandle->massStorageIntDescr[2] = CSL_MSC_INTERFACE_DESC2_DEFVAL;
        /* bNumEndpoints, bInterfaceClass */
        pCtrlHandle->massStorageIntDescr[3] = CSL_MSC_INTERFACE_DESC3_DEFVAL;
        /* bInterfaceSubClass, bInterfaceProtocol */
        pCtrlHandle->massStorageIntDescr[4] = CSL_MSC_INTERFACE_DESC4_DEFVAL;
        /* iInterface */
        pCtrlHandle->massStorageIntDescr[5] = CSL_MSC_INTERFACE_DESC5_DEFVAL;

        /***** Setting up the Mass Storage Bulk Out Endpoint Descriptor *****/
        pCtrlHandle->massStorageBulkOutEndptDescr[0] =
                                CSL_MSC_BULKOUTEP_DESC0_DEFVAL;
        /* bLength, bDescriptorType */
        pCtrlHandle->massStorageBulkOutEndptDescr[1] =
                   (CSL_MSC_ENDPOINT_DESCRIPTOR_TYPE << CSL_MSC_8BIT_SHIFT) |
                    CSL_MSC_BULKOUTEP_DESC_DEFLEN;
        /* bEndpointAddress, bmAttributes */
        pCtrlHandle->massStorageBulkOutEndptDescr[2] =
                                CSL_MSC_BULKOUTEP_DESC2_DEFVAL;
        /* wMaxPacketSize */
        pCtrlHandle->massStorageBulkOutEndptDescr[3] =
                                CSL_MSC_BULKOUTEP_DESC3_DEFVAL;
        /* bInterval */
        pCtrlHandle->massStorageBulkOutEndptDescr[4] =
                                CSL_MSC_BULKOUTEP_DESC4_DEFVAL;

        /***** Setting up the Mass Storage Bulk In Endpoint Descriptor *****/
        pCtrlHandle->massStorageBulkInEndptDescr[0] =
                                CSL_MSC_BULKINEP_DESC0_DEFVAL;
        /* bLength, bDescriptorType */
        pCtrlHandle->massStorageBulkInEndptDescr[1] =
               (CSL_MSC_ENDPOINT_DESCRIPTOR_TYPE << CSL_MSC_8BIT_SHIFT) |
                CSL_MSC_BULKINEP_DESC_DEFLEN;
        /* bEndpointAddress, bmAttributes */
        pCtrlHandle->massStorageBulkInEndptDescr[2] =
                                CSL_MSC_BULKINEP_DESC2_DEFVAL;
        /* wMaxPacketSize */
        pCtrlHandle->massStorageBulkInEndptDescr[3] =
                                CSL_MSC_BULKINEP_DESC3_DEFVAL;
        /* bInterval */
        pCtrlHandle->massStorageBulkInEndptDescr[4] =
                                CSL_MSC_BULKINEP_DESC4_DEFVAL;

        /***** Configuration descriptor for high speed operation *****/
        /*
         * This descriptor array contains following descriptors
         *  - Configuration descriptor
         *  - Interface descriptor
         *  - Endpoint(OUT) descriptor
         *  - Endpoint(IN) descriptor
         *
         * Host reads interface and endpoint descriptors as part of
         * configuration descriptor.
         */
        pCtrlHandle->highSpeedCfgDescr[0]  = CSL_MSC_HS_CFG_DESC0_DEFVAL;

        /* START OF CONFIGURATION DESCRIPTOR */
        /* bLength, bDescriptorType */
        pCtrlHandle->highSpeedCfgDescr[1]  = (CSL_MSC_CONFIGURATION_DESCRIPTOR_TYPE <<
                                             CSL_MSC_8BIT_SHIFT) |
                                             CSL_MSC_CONF_DESC_DEFLEN;
        /* wTotalLength */
        pCtrlHandle->highSpeedCfgDescr[2]  = CSL_MSC_HS_CFG_DESC2_DEFVAL;
        /* bNumInterfaces, bConfigurationValue */
        pCtrlHandle->highSpeedCfgDescr[3]  = CSL_MSC_HS_CFG_DESC3_DEFVAL;
        /* iConfiguration, bmAttributes */
        pCtrlHandle->highSpeedCfgDescr[4]  = CSL_MSC_HS_CFG_DESC4_DEFVAL;
        /* bMaxPower, bLength - Interface Descriptor */
        pCtrlHandle->highSpeedCfgDescr[5]  = ((USB_getMaxPower(CSL_USB0) & 0xFF) |
                                              (CSL_MSC_INTERFACE_DESC_DEFLEN << 8));
        /* END OF CONFIGURATION DESCRIPTOR;  START OF INTERFACE DESCRIPTOR */

        /* bDescriptorType - Interface Descriptor, bInterfaceNumber */
        pCtrlHandle->highSpeedCfgDescr[6]  = CSL_MSC_HS_CFG_DESC6_DEFVAL;
        /* bAlternateSetting, bNumEndpoints */
        pCtrlHandle->highSpeedCfgDescr[7]  = CSL_MSC_HS_CFG_DESC7_DEFVAL;
        /* bInterfaceClass - Mass Storage, bInterfaceSubClass - SCSI command set */
        pCtrlHandle->highSpeedCfgDescr[8]  = CSL_MSC_HS_CFG_DESC8_DEFVAL;
        pCtrlHandle->highSpeedCfgDescr[9]  = CSL_MSC_HS_CFG_DESC9_DEFVAL;
        /* bInterfaceProtocol - Bulk-Only Transport, iInterface */
        /* END OF INTERFACE DESCRIPTOR */

        /* START OF ENDPOINT(OUT) DESCRIPTOR */
        /* bLength - Endpoint Descr, bDescriptorType - Endpoint Descr */
        pCtrlHandle->highSpeedCfgDescr[10] = CSL_MSC_HS_CFG_DESC10_DEFVAL;
        /* bEndpointAddress - 2(OUT endpoint), bmAttributes - Bulk */
        pCtrlHandle->highSpeedCfgDescr[11] = CSL_MSC_HS_CFG_DESC11_DEFVAL;
        /* wMaxPacketSize */
        pCtrlHandle->highSpeedCfgDescr[12] = CSL_MSC_HS_CFG_DESC12_DEFVAL;
        /* bInterval, bLength - Endpoint Descr */
        pCtrlHandle->highSpeedCfgDescr[13] = CSL_MSC_HS_CFG_DESC13_DEFVAL;
        /* bDescriptorType - Endpoint Descr, bEndpointAddress - 1(IN endpoint) */
        pCtrlHandle->highSpeedCfgDescr[14] = CSL_MSC_HS_CFG_DESC14_DEFVAL;
        /* bmAttributes - Bulk, wMaxPacketSize(lsB) */
        pCtrlHandle->highSpeedCfgDescr[15] = CSL_MSC_HS_CFG_DESC15_DEFVAL;
        /* wMaxPacketSize(msB), bInterval */
        pCtrlHandle->highSpeedCfgDescr[16] = CSL_MSC_HS_CFG_DESC16_DEFVAL;

        /***** Configuration descriptor for full speed operation *****/
        /*
         * This descriptor array contains following descriptors
         *  - Configuration descriptor
         *  - Interface descriptor
         *  - Endpoint(OUT) descriptor
         *  - Endpoint(IN) descriptor
         *
         * Host reads interface and endpoint descriptors as part of
         * configuration descriptor.
         */
        pCtrlHandle->fullSpeedCfgDescr[0]  = CSL_MSC_FS_CFG_DESC0_DEFVAL;

        /* START OF CONFIGURATION DESCRIPTOR */
        /* bLength, bDescriptorType */
        pCtrlHandle->fullSpeedCfgDescr[1]  = (CSL_MSC_CONFIGURATION_DESCRIPTOR_TYPE <<
                                             CSL_MSC_8BIT_SHIFT) |
                                             CSL_MSC_CONF_DESC_DEFLEN;
        /* wTotalLength */
        pCtrlHandle->fullSpeedCfgDescr[2]  = CSL_MSC_FS_CFG_DESC2_DEFVAL;
        /* bNumInterfaces, bConfigurationValue */
        pCtrlHandle->fullSpeedCfgDescr[3]  = CSL_MSC_FS_CFG_DESC3_DEFVAL;
        /* iConfiguration, bmAttributes */
        pCtrlHandle->fullSpeedCfgDescr[4]  = CSL_MSC_FS_CFG_DESC4_DEFVAL;
        /* bMaxPower, bLength - Interface Descriptor */
        pCtrlHandle->fullSpeedCfgDescr[5]  = ((USB_getMaxPower(CSL_USB0) & 0xFF) |
                                              (CSL_MSC_INTERFACE_DESC_DEFLEN << 8));
        /* END OF CONFIGURATION DESCRIPTOR;  START OF INTERFACE DESCRIPTOR */

        /* bDescriptorType - Interface Descriptor, bInterfaceNumber */
        pCtrlHandle->fullSpeedCfgDescr[6]  = CSL_MSC_FS_CFG_DESC6_DEFVAL;
        /* bAlternateSetting, bNumEndpoints */
        pCtrlHandle->fullSpeedCfgDescr[7]  = CSL_MSC_FS_CFG_DESC7_DEFVAL;
        /* bInterfaceClass - Mass Storage, bInterfaceSubClass - SCSI command set */
        pCtrlHandle->fullSpeedCfgDescr[8]  = CSL_MSC_FS_CFG_DESC8_DEFVAL;
        /* bInterfaceProtocol - Bulk-Only Transport, iInterface */
        pCtrlHandle->fullSpeedCfgDescr[9]  = CSL_MSC_FS_CFG_DESC9_DEFVAL;
        /* END OF INTERFACE DESCRIPTOR */

        /* START OF ENDPOINT(OUT) DESCRIPTOR */
        /* bLength - Endpoint Descr, bDescriptorType - Endpoint Descr */
        pCtrlHandle->fullSpeedCfgDescr[10] = CSL_MSC_FS_CFG_DESC10_DEFVAL;
        /* bEndpointAddress - 2(OUT endpoint), bmAttributes - Bulk */
        pCtrlHandle->fullSpeedCfgDescr[11] = CSL_MSC_FS_CFG_DESC11_DEFVAL;
        /* wMaxPacketSize */
        pCtrlHandle->fullSpeedCfgDescr[12] = CSL_MSC_FS_CFG_DESC12_DEFVAL;
        /* bInterval, bLength - Endpoint Descr */
        pCtrlHandle->fullSpeedCfgDescr[13] = CSL_MSC_FS_CFG_DESC13_DEFVAL;
        /* END OF ENDPOINT(OUT) DESCRIPTOR;  START OF ENDPOINT(IN) DESCRIPTOR */

        /* bDescriptorType - Endpoint Descr, bEndpointAddress - 1(IN endpoint) */
        pCtrlHandle->fullSpeedCfgDescr[14] = CSL_MSC_FS_CFG_DESC14_DEFVAL;
        /* bmAttributes - Bulk, wMaxPacketSize(lsB) */
        pCtrlHandle->fullSpeedCfgDescr[15] = CSL_MSC_FS_CFG_DESC15_DEFVAL;
        /* wMaxPacketSize(msB), bInterval */
        pCtrlHandle->fullSpeedCfgDescr[16] = CSL_MSC_FS_CFG_DESC16_DEFVAL;
        /* END OF ENDPOINT(IN) DESCRIPTOR */

        /***** Mass Storage Bulk In Endpoint Descriptor Link *****/
        /* length in bytes */
        pCtrlHandle->massStorageBulkInEndptLnk.bytes =
                                CSL_MSC_BULKINEP_DESC_DEFLEN;
        pCtrlHandle->massStorageBulkInEndptLnk.pBuffer =
                        (Uint16 *)&pCtrlHandle->massStorageBulkInEndptDescr[0];
        /* pointer to next descriptor link */
        pCtrlHandle->massStorageBulkInEndptLnk.pNextBuffer =  NULL;

        /* Mass Storage Bulk Out Endpoint Descriptor Link */
        /* length in bytes */
        pCtrlHandle->massStorageBulkOutEndptLnk.bytes =
                                CSL_MSC_BULKOUTEP_DESC_DEFLEN;
        pCtrlHandle->massStorageBulkOutEndptLnk.pBuffer =
                (Uint16 *)&pCtrlHandle->massStorageBulkOutEndptDescr[0];
        /* pointer to next descriptor link */
        pCtrlHandle->massStorageBulkOutEndptLnk.pNextBuffer =
                                &pCtrlHandle->massStorageBulkInEndptLnk;

        /***** Mass Storage Interface Descriptor Link *****/
        /* length in bytes */
        pCtrlHandle->massStorageInterfcLnk.bytes   =
                            CSL_MSC_INTERFACE_DESC_DEFLEN;
        /* pointer to this descriptor */
        pCtrlHandle->massStorageInterfcLnk.pBuffer =
                             (Uint16 *)&pCtrlHandle->massStorageIntDescr[0];
        /* pointer to next descriptor link */
        pCtrlHandle->massStorageInterfcLnk.pNextBuffer =
                              &pCtrlHandle->massStorageBulkOutEndptLnk;


        /***** Configuration Descriptor Link *****/
        /* length in bytes */
        pCtrlHandle->configDescrLnk.bytes   = CSL_MSC_CONF_DESC_DEFLEN;
        /* pointer to this descriptor */
        pCtrlHandle->configDescrLnk.pBuffer =
                            (Uint16 *)&pCtrlHandle->confDescr[0];
        /* pointer to next descriptor link */
        pCtrlHandle->configDescrLnk.pNextBuffer =
                               &pCtrlHandle->massStorageInterfcLnk ;


        /***** String Langid *****/
        /* field for xfer_byte_cnt */
        pCtrlHandle->stringDescrLangid[0] = CSL_MSC_LANGID0_DEFVAL;
        /* bLength, bDescriptorType */
        pCtrlHandle->stringDescrLangid[1] = (CSL_MSC_STRING_DESCRIPTOR_TYPE <<
                                             CSL_MSC_8BIT_SHIFT) |
                                             CSL_MSC_LANGID_DEFLEN;
        /* LANGID (English) */
        pCtrlHandle->stringDescrLangid[2] = CSL_MSC_LANGID2_DEFVAL;
        pCtrlHandle->stringDescrLangid[3] = CSL_MSC_LANGID3_DEFVAL;


        /*
         * Configure the default and other speed configuration descriptor
         * pointers. When USB is configured for high-speed, full-speed
         * configuration descriptor will be the other speed configuration
         * descriptor and vice versa.
         */
        if(pCtrlHandle->usbSpeedCfg == TRUE)
        {
            /* High speed operation */
            pCtrlHandle->pCfgDescr           = &pCtrlHandle->highSpeedCfgDescr[0];
            pCtrlHandle->pOtherSpeedCfgDescr = &pCtrlHandle->fullSpeedCfgDescr[0];
        }
        else
        {
            /* Full speed operation */
            pCtrlHandle->pCfgDescr           = &pCtrlHandle->fullSpeedCfgDescr[0];
            pCtrlHandle->pOtherSpeedCfgDescr = &pCtrlHandle->highSpeedCfgDescr[0];
        }

        /* Set the descriptor type for the other speed configuration descriptor */
        pCtrlHandle->pOtherSpeedCfgDescr[1] = (CSL_MSC_OTHERSPEED_CFG_DESCRIPTOR_TYPE <<
                                               CSL_MSC_8BIT_SHIFT) |
                                               CSL_MSC_CONF_DESC_DEFLEN;
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

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
static CSL_Status MSC_initMscVal(CSL_MscObject    *pMscHandle)
{
    Uint16       copyCnt;
    CSL_Status   status;

    status = CSL_SOK;

    if(pMscHandle != NULL)
    {
        pMscHandle->storageState = 0;
        pMscHandle->activityPresentFlag =0;

        /* Sense Data */
        /* size */
        pMscHandle->senseData[0] = CSL_MSC_SENSE_DATA0_DEFVAL;
        /* Dummy for USB_postTransaction */
        pMscHandle->senseData[1] = CSL_MSC_SENSE_DATA1_DEFVAL;
        /* Error code = 0x70 (0-1) */
        pMscHandle->senseData[2] = CSL_MSC_SENSE_DATA2_DEFVAL;
        /* Sense key (2-3) */
        pMscHandle->senseData[3] = CSL_MSC_SENSE_DATA3_DEFVAL;
        /* bytes 4-5 */
        pMscHandle->senseData[4] = CSL_MSC_SENSE_DATA4_DEFVAL;
        /* Additional length = 18 (6-7) */
        pMscHandle->senseData[5] = CSL_MSC_SENSE_DATA5_DEFVAL;

        pMscHandle->readCapacity[0] = CSL_MSC_READ_CAPACITY0_DEFVAL;

        for(copyCnt = 6; copyCnt < 15; copyCnt++)
        {
            pMscHandle->senseData[copyCnt]  = 0x0000;
        }

        /* CSW */
        /* Dummy for USB_postTransaction */
        pMscHandle->csw[0] = CSL_MSC_CSW0_DEFVAL;
        pMscHandle->csw[1] = CSL_MSC_CSW1_DEFVAL;
        pMscHandle->csw[2] = CSL_MSC_CSW2_DEFVAL;
        for (copyCnt = 3; copyCnt < 8; copyCnt++)
        {
            pMscHandle->csw[copyCnt] = 0;
        }

        pMscHandle->modeSenseData[0] = CSL_MSC_MODESENSE_DATA0_DEFVAL;
        pMscHandle->modeSenseData[1] = CSL_MSC_MODESENSE_DATA1_DEFVAL;
        pMscHandle->modeSenseData[2] = CSL_MSC_MODESENSE_DATA2_DEFVAL;
        pMscHandle->modeSenseData[3] = CSL_MSC_MODESENSE_DATA3_DEFVAL;
        pMscHandle->modeSenseData[4] = CSL_MSC_MODESENSE_DATA4_DEFVAL;
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

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
                                     pUsbEpHandle     hUsbInEp)
{
    CSL_Status   status;
    Uint16       logicalUnit;
    Uint16       scsiCommand;

      /* Check if the transaction is Done */
    if(USB_isTransactionDone(hUsbOutEp, &status))
    {
        status = MSC_verifyCBW(pMscHandle, hUsbOutEp, hUsbInEp);
        if (status != CSL_SOK)
        {
            status = USB_stallEndpt(hUsbInEp);

            /* Dummy data transfer to intimate MSC_bulk function */
            status |= USB_postTransaction(hUsbInEp, 0,
                                          &pMscHandle->csw[0],
                                          CSL_USB_IOFLAG_NOT);
        }
        else
        {
            pMscHandle->activityPresentFlag = 1;

            pMscHandle->csw[CSL_MSC_CSW_2] = pMscHandle->cbw[CSL_MSC_CBW_2];
            pMscHandle->csw[CSL_MSC_CSW_3] = pMscHandle->cbw[CSL_MSC_CBW_3];
            pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;

            pMscHandle->cbwDataTransferLength =
            ((Uint32)(pMscHandle->cbw[CSL_MSC_CBW_5]) << CSL_MSC_16BIT_SHIFT)
                                      + (Uint32)pMscHandle->cbw[CSL_MSC_CBW_4];

            pMscHandle->csw[CSL_MSC_CSW_4] =
                     (Uint16)(pMscHandle->cbwDataTransferLength & CSL_MSC_16BIT_MASK);
            pMscHandle->csw[CSL_MSC_CSW_5] =
                     (Uint16)(pMscHandle->cbwDataTransferLength >> CSL_MSC_16BIT_SHIFT);

            /* Getting the logical Unit ready */
            logicalUnit = pMscHandle->cbw[CSL_MSC_CBW_6] >> CSL_MSC_8BIT_SHIFT;
            /* Parsing the SCSI command */
            scsiCommand = pMscHandle->cbw[CSL_MSC_CBW_7] >> CSL_MSC_8BIT_SHIFT;

            switch (scsiCommand)
            {
                case CSL_MSC_SCSI_COMMAND_INQUIRY:
                    status = MSC_HandleInquiry(pMscHandle, hUsbInEp, logicalUnit);
                    break;

                case CSL_MSC_SCSI_COMMAND_REQUEST_SENSE:
                    status = MSC_HandleRequestSense(pMscHandle, hUsbInEp);
                    break;

                case CSL_MSC_SCSI_COMMAND_TEST_UNIT_READY:
                    status = MSC_HandleTestUnitReady(pMscHandle, hUsbInEp,
                                                      logicalUnit);
                    break;

                case CSL_MSC_SCSI_COMMAND_PREVENT_ALLOW_MEDIA_REMOVAL:
                    status = MSC_HandlePreventAllowMediaRemoval(pMscHandle,
                                                                hUsbInEp,
                                                                    logicalUnit);
                    break;

                case CSL_MSC_SCSI_COMMAND_READ_CAPACITY:
                    status = MSC_HandleReadCapacity(pMscHandle, hUsbInEp,
                                                      logicalUnit);
                    break;

                case CSL_MSC_SCSI_COMMAND_READ_10:
                    status = MSC_ExecuteRead(pMscHandle, logicalUnit, TRUE);
                    break;

                case CSL_MSC_SCSI_COMMAND_WRITE_10:
                    status = MSC_ExecuteWrite(pMscHandle, logicalUnit, TRUE);
                    break;

                case CSL_MSC_SCSI_COMMAND_VERIFY_10:
                    status = MSC_HandleVerify10(pMscHandle, hUsbInEp, logicalUnit);
                    break;

                case CSL_MSC_SCSI_COMMAND_START_STOP_UNIT:
                    status = MSC_HandleStartStopUnit(pMscHandle, hUsbInEp,
                                                     logicalUnit);
                    break;

                case CSL_MSC_SCSI_COMMAND_MODE_SENSE_6:
                    status = MSC_HandleModeSense6(pMscHandle, hUsbInEp,
                                                  logicalUnit);
                    break;

                case CSL_MSC_SCSI_COMMAND_MODE_SENSE_10:
                    status = MSC_HandleModeSense10(pMscHandle, hUsbInEp,
                                                   logicalUnit);
                    break;
                default:
                    MSC_SetSenseKeys(pMscHandle->senseData,
                                  CSL_MSC_SCSI_SENSEKEY_ILLEGAL_REQUEST,
                                  CSL_MSC_SCSI_ASC_INVALID_COMMAND_OPERATION_CODE);

                    if(pMscHandle->cbw[CSL_MSC_CBW_6] & 0x80)
                    {
                        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;
                        status = USB_postTransaction(hUsbInEp, 0, &pMscHandle->csw[0],
                                                     CSL_USB_IOFLAG_NOSHORT);

                        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_SHORT_PKT;
                    }
                    else
                    {
                        if (pMscHandle->cbwDataTransferLength == 0)
                        {
                              pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;
                            pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_CSW;

                            /* Sending CSW with error */
                            status = USB_postTransaction(hUsbInEp,
                                                         CSL_MSC_CSW_DATA_SIZE,
                                                         &pMscHandle->csw[0],
                                                         CSL_USB_IOFLAG_NONE);
                        }
                        else
                        {
                            /* Stall the end point */
                            status  = USB_stallEndpt(hUsbInEp);
                            status |= USB_stallEndpt(hUsbOutEp);

                            pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_STALL;

                            /* Dummy data transfer to intimate MSC_bulk function */
                            status |= USB_postTransaction(hUsbInEp, 0,
                                                         &pMscHandle->csw[0],
                                                         CSL_USB_IOFLAG_NOT);
                        }
                    }
                    break;
            }
        }
    }

    return(status);
}

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
                           Uint16           first)
{
    Uint32                mediaLba;
    Uint16                mediaLbaCount;
    Uint16                tempData;
    CSL_MscMediaStatus    mediaStat;
    CSL_Status            status;

    status = CSL_SOK;

    /* Check whether the data direction is proper or not */
    if(((pMscHandle->cbw[CSL_MSC_CBW_6] &
          CSL_MSC_CBW_DIRBIT_MASK) != CSL_MSC_CBW_DATADIR_IN) &&
        (pMscHandle->cbwDataTransferLength != 0))
    {
        status = MSC_handleDataDirMisMatch(pMscHandle, CSL_MSC_CBW_DATADIR_IN);

        return(status);
    }

    if(first)
    {
        /*
         * Check if host sent valid transfer length parameter.
         * Send CSW with phase error in case of invalid transfer length
         */
        if (pMscHandle->cbwDataTransferLength != 0)
        {
            mediaLba  = ((Uint32)(pMscHandle->cbw[CSL_MSC_CBW_8] >>
                      CSL_MSC_8BIT_SHIFT)) << CSL_MSC_24BIT_SHIFT;
            mediaLba |= ((Uint32)(pMscHandle->cbw[CSL_MSC_CBW_9] &
                      CSL_MSC_8BIT_MASK)) << CSL_MSC_16BIT_SHIFT;
            mediaLba |= ((Uint32)(pMscHandle->cbw[CSL_MSC_CBW_9] >>
                      CSL_MSC_8BIT_SHIFT)) << CSL_MSC_8BIT_SHIFT;
            mediaLba |= ((Uint32)(pMscHandle->cbw[CSL_MSC_CBW_10] &
                      CSL_MSC_8BIT_MASK));
            mediaLbaCount = (pMscHandle->cbw[CSL_MSC_CBW_11] << CSL_MSC_8BIT_SHIFT)|
                         (pMscHandle->cbw[CSL_MSC_CBW_11] >> CSL_MSC_8BIT_SHIFT);

            pMscHandle->lun[lunNum].currLba = mediaLba;
            pMscHandle->lun[lunNum].lbaCnt  = mediaLbaCount;
            MSC_SetSenseKeys(pMscHandle->senseData,
                CSL_MSC_SCSI_SENSEKEY_NO_SENSE,
                CSL_MSC_SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION);

            if (mediaLbaCount == 0)
            {
                pMscHandle->csw[CSL_MSC_CSW_4] =
                 (Uint16)(pMscHandle->cbwDataTransferLength & CSL_MSC_16BIT_MASK);
                pMscHandle->csw[CSL_MSC_CSW_5] =
                 (Uint16)(pMscHandle->cbwDataTransferLength >> CSL_MSC_16BIT_SHIFT);

                pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;

                /* Stall the end point */
                pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_STALL;
                status = USB_stallEndpt(&pMscHandle->bulkInEpObj);

                /* Dummy data transfer to intimate MSC_bulk function */
                status = USB_postTransaction(&pMscHandle->bulkInEpObj,
                                             0,
                                             pMscHandle->lbaBuffer,
                                             CSL_USB_IOFLAG_NOT);

                return(status);
            }
        }
        else
        {
            status = MSC_sendCswWithPhaseError(pMscHandle,
                                               &pMscHandle->bulkInEpObj);

            return(status);
        }
    }

    if(USB_isTransactionDone(&pMscHandle->bulkInEpObj, &status))
    {
        /* Check if host requested less data than the device
           intends to send */
        if((pMscHandle->lun[lunNum].lbaCnt != 0) &&
           ((pMscHandle->cbwDataTransferLength == 0)))
        {
            status = MSC_sendCswWithPhaseError(pMscHandle,
                                               &pMscHandle->bulkInEpObj);
            return(status);
        }

        /* Check if media is present */
        mediaStat = CSL_MSC_MEDIACCESS_NOTPRESENT;

        /* Read the sector if the MEdia is present or media is bad*/
        if(pMscHandle->lun[lunNum].mediaState & CSL_MSC_MEDIA_PRESENT ||
           pMscHandle->lun[lunNum].mediaState & CSL_MSC_MEDIA_BAD)
        {
            mediaStat = pMscHandle->readMedia(lunNum, &pMscHandle->lbaBuffer[0],
                                             pMscHandle->lun[lunNum].currLba,
                                             pMscHandle->lun[lunNum].lbaCnt);
        }

         /* Process the state of the Media Sector */
         if(mediaStat == CSL_MSC_MEDIACCESS_SUCCESS)
         {
            if(pMscHandle->lun[lunNum].lbaSize < pMscHandle->cbwDataTransferLength)
            {
                tempData = pMscHandle->lun[lunNum].lbaSize;
            }
            else
            {
                tempData = (Uint16)(pMscHandle->cbwDataTransferLength &
                                    CSL_MSC_16BIT_MASK);
            }

            pMscHandle->cbwDataTransferLength -= (Uint32)tempData;
            pMscHandle->csw[CSL_MSC_CSW_4] =
             (Uint16)(pMscHandle->cbwDataTransferLength & CSL_MSC_16BIT_MASK);
            pMscHandle->csw[CSL_MSC_CSW_5] =
             (Uint16)(pMscHandle->cbwDataTransferLength >> CSL_MSC_16BIT_SHIFT);

            /* Setting the Storage State */
            pMscHandle->storageState = CSL_MSC_STORAGE_STATE_READ;
            pMscHandle->lun[lunNum].currLba++;

            if(pMscHandle->lun[lunNum].lbaCnt > 0)
            {
                pMscHandle->lun[lunNum].lbaCnt--;
            }

            if(pMscHandle->lun[lunNum].lbaCnt == 0)
            {
                /* Check if host requested more data than the device
                   intends to send */
                if (pMscHandle->cbwDataTransferLength != 0)
                {
                    pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;

                    /* Stall the end point */
                    pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_STALL;
                    status = USB_stallEndpt(&pMscHandle->bulkInEpObj);

                    /* Dummy data transfer to intimate MSC_bulk function */
                    status = USB_postTransaction(&pMscHandle->bulkInEpObj,
                                                 0,
                                                 pMscHandle->lbaBuffer,
                                                 CSL_USB_IOFLAG_NOT);
                    return(status);
                }
                else
                {
                    /* Change the Storage State to Sending Data */
                    pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;
                    pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_DATA;
                }
            }

            /* Posting the Data Read from the media */
            status = USB_postTransaction(&pMscHandle->bulkInEpObj,
                                         pMscHandle->lun[lunNum].lbaSize,
                                         pMscHandle->lbaBuffer,
                                         CSL_USB_IOFLAG_NOSHORT);
        }
        else
        {
            /* Setting the Data Residue */
            pMscHandle->csw[CSL_MSC_CSW_4] =
             (Uint16)(pMscHandle->cbwDataTransferLength & CSL_MSC_16BIT_MASK);
            pMscHandle->csw[CSL_MSC_CSW_5] =
             (Uint16)(pMscHandle->cbwDataTransferLength >> CSL_MSC_16BIT_SHIFT);
            pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;
            /* Change the Storage State to Sending Data */
            pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_DATA;

            if(mediaStat == CSL_MSC_MEDIACCESS_OVERFLOW)
            {
                MSC_SetSenseKeys(pMscHandle->senseData,
                    CSL_MSC_SCSI_SENSEKEY_ILLEGAL_REQUEST,
                    CSL_MSC_SCSI_ASC_MEDIA_OUTOFRANGE);
            }
            else if(mediaStat == CSL_MSC_MEDIACCESS_NOTPRESENT)
            {
                MSC_SetSenseKeys(&pMscHandle->senseData[0],
                    CSL_MSC_SCSI_SENSEKEY_NOT_READY,
                    CSL_MSC_SCSI_ASC_MEDIA_NOT_PRESENT);
                /*Change the media state to not present or not bad .. PRASAD*/
                pMscHandle->lun[lunNum].mediaState &=~(CSL_MSC_MEDIA_PRESENT | CSL_MSC_MEDIA_BAD);
            }
            else
            {
                if(mediaStat == CSL_MSC_MEDIACCESS_BADMEDIA)
                {
                    MSC_SetSenseKeys(pMscHandle->senseData,
                        CSL_MSC_SCSI_SENSEKEY_NOT_READY,
                        CSL_MSC_SCSI_ASC_MEDIA_MANUALINTERVENTION_REQUIRED);
                    pMscHandle->lun[lunNum].mediaState &= ~CSL_MSC_MEDIA_PRESENT;
                    /* Change mediaStat to BAD */
                    pMscHandle->lun[lunNum].mediaState |= CSL_MSC_MEDIA_BAD;
                }
            }

            /* Posting a zero byte Data Transfer when an error condition occurs */
            status = USB_postTransaction(&pMscHandle->bulkInEpObj, 0,
                                         &pMscHandle->csw[0],
                                         CSL_USB_IOFLAG_NOSHORT);
        }
    }

    return(status);
}

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
                            Uint16           first)
{
    Uint32                mediaLba;
    Uint16                mediaLbaCount;
    Uint16                tempData;
    CSL_Status            status;
    CSL_MscMediaStatus    mediaStat;

    status = CSL_SOK;

    /* Check whether the data direction is proper or not */
    if(((pMscHandle->cbw[CSL_MSC_CBW_6] &
          CSL_MSC_CBW_DIRBIT_MASK) != CSL_MSC_CBW_DATADIR_OUT) &&
        (pMscHandle->cbwDataTransferLength != 0))
    {
        status = MSC_handleDataDirMisMatch(pMscHandle, CSL_MSC_CBW_DATADIR_OUT);

        return(status);
    }

    if(first)
    {
        /*
         * Check if host sent valid transfer length parameter.
         * Send CSW with phase error in case of invalid transfer length
         */
        if (pMscHandle->cbwDataTransferLength != 0)
        {
            mediaLba  = ((Uint32)(pMscHandle->cbw[CSL_MSC_CBW_8] >>
                         CSL_MSC_8BIT_SHIFT)) << CSL_MSC_24BIT_SHIFT;
            mediaLba |= ((Uint32)(pMscHandle->cbw[CSL_MSC_CBW_9] &
                         CSL_MSC_8BIT_MASK)) << CSL_MSC_16BIT_SHIFT;
            mediaLba |= ((Uint32)(pMscHandle->cbw[CSL_MSC_CBW_9] >>
                         CSL_MSC_8BIT_SHIFT)) << CSL_MSC_8BIT_SHIFT;
            mediaLba |= ((Uint32)(pMscHandle->cbw[CSL_MSC_CBW_10] &
                         CSL_MSC_8BIT_MASK));
            mediaLbaCount = (pMscHandle->cbw[CSL_MSC_CBW_11] << CSL_MSC_8BIT_SHIFT)|
                            (pMscHandle->cbw[CSL_MSC_CBW_11] >> CSL_MSC_8BIT_SHIFT);
            pMscHandle->lun[lunNum].currLba = mediaLba;
            pMscHandle->lun[lunNum].lbaCnt = mediaLbaCount;
            pMscHandle->storageState = CSL_MSC_STORAGE_STATE_WRITE;
            pMscHandle->lun[lunNum].verifyFlag = CSL_MSC_VERIFY_PASSED;
            pMscHandle->writeToMedia = TRUE;
            pMscHandle->lun[lunNum].mediaState &= (~CSL_MSC_MEDIA_WRITEPENDING);

            MSC_SetSenseKeys(pMscHandle->senseData,
                CSL_MSC_SCSI_SENSEKEY_NO_SENSE,
                CSL_MSC_SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION);
        }
        else
        {
            status = MSC_sendCswWithPhaseError(pMscHandle,
                                               &pMscHandle->bulkInEpObj);
            return (status);
        }
    }

    if(USB_isTransactionDone(&pMscHandle->bulkOutEpObj, &status))
    {
        if (pMscHandle->writeToMedia == TRUE)
        {
            if(pMscHandle->lun[lunNum].mediaState & CSL_MSC_MEDIA_WRITEPENDING)
            {
                pMscHandle->lun[lunNum].mediaState &= (~CSL_MSC_MEDIA_WRITEPENDING);
                /* Checking verify Command Flag if it already Set */
                if (pMscHandle->lun[lunNum].verifyFlag == CSL_MSC_VERIFY_PASSED)
                {
                    if (!(pMscHandle->lun[lunNum].readwrite &
                        CSL_MSC_MEDIA_WRITEENABLE))
                    {
                        MSC_SetSenseKeys(pMscHandle->senseData,
                            CSL_MSC_SCSI_SENSEKEY_ILLEGAL_REQUEST,
                            CSL_MSC_SCSI_ASC_INVALID_COMMAND_OPERATION_CODE);
                        pMscHandle->lun[lunNum].verifyFlag = CSL_MSC_VERIFY_FAILED;
                        pMscHandle->csw[CSL_MSC_CSW_4] =
                                      (Uint16)(pMscHandle->cbwDataTransferLength &
                                               CSL_MSC_16BIT_MASK);
                        pMscHandle->csw[CSL_MSC_CSW_5] =
                                      (Uint16)(pMscHandle->cbwDataTransferLength >>
                                               CSL_MSC_16BIT_SHIFT);
                        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;
                    }

                    if(pMscHandle->lun[lunNum].verifyFlag == CSL_MSC_VERIFY_PASSED)
                    {
                        /* Default value for mediaStat is not present */
                        mediaStat = CSL_MSC_MEDIACCESS_NOTPRESENT;
                        /* If media is bad or present issue Write command */
                        if(pMscHandle->lun[lunNum].mediaState & CSL_MSC_MEDIA_PRESENT ||
                           pMscHandle->lun[lunNum].mediaState & CSL_MSC_MEDIA_BAD)
                        {
                            mediaStat = pMscHandle->writeMedia(lunNum,
                                                   &pMscHandle->lbaBuffer[0],
                                                   pMscHandle->lun[lunNum].currLba,
                                                   pMscHandle->lun[lunNum].lbaCnt);
                        }

                        switch (mediaStat)
                        {
                            case CSL_MSC_MEDIACCESS_OVERFLOW:
                                  MSC_SetSenseKeys(pMscHandle->senseData,
                                      CSL_MSC_SCSI_SENSEKEY_ILLEGAL_REQUEST,
                                      CSL_MSC_SCSI_ASC_MEDIA_OUTOFRANGE);
                                  pMscHandle->csw[CSL_MSC_CSW_4] =
                                    (Uint16)(pMscHandle->cbwDataTransferLength &
                                             CSL_MSC_16BIT_MASK);
                                  pMscHandle->csw[CSL_MSC_CSW_5] =
                                    (Uint16)(pMscHandle->cbwDataTransferLength >>
                                             CSL_MSC_16BIT_SHIFT);
                                  pMscHandle->csw[CSL_MSC_CSW_6] =
                                             CSL_MSC_CSW_STATUS_COMMAND_FAILED;
                                  pMscHandle->lun[lunNum].verifyFlag =
                                             CSL_MSC_VERIFY_FAILED;
                                  break;

                            case CSL_MSC_MEDIACCESS_SUCCESS:
                                  pMscHandle->cbwDataTransferLength -= 512;
                                  pMscHandle->csw[CSL_MSC_CSW_4] =
                                    (Uint16)(pMscHandle->cbwDataTransferLength &
                                             CSL_MSC_16BIT_MASK);
                                  pMscHandle->csw[CSL_MSC_CSW_5] =
                                    (Uint16)(pMscHandle->cbwDataTransferLength >>
                                             CSL_MSC_16BIT_SHIFT);
                                  pMscHandle->csw[CSL_MSC_CSW_6] =
                                             CSL_MSC_CSW_STATUS_COMMAND_PASSED;
                                  pMscHandle->lun[lunNum].verifyFlag =
                                             CSL_MSC_VERIFY_PASSED;
                                  pMscHandle->lun[lunNum].currLba++;

                                  break;

                            case CSL_MSC_MEDIACCESS_NOTPRESENT:
                                  MSC_SetSenseKeys(pMscHandle->senseData,
                                      CSL_MSC_SCSI_SENSEKEY_NOT_READY,
                                      CSL_MSC_SCSI_ASC_MEDIA_NOT_PRESENT);
                                  pMscHandle->csw[CSL_MSC_CSW_4] =
                                    (Uint16)(pMscHandle->cbwDataTransferLength &
                                             CSL_MSC_16BIT_MASK);
                                  pMscHandle->csw[CSL_MSC_CSW_5] =
                                    (Uint16)(pMscHandle->cbwDataTransferLength >>
                                             CSL_MSC_16BIT_SHIFT);
                                  pMscHandle->csw[CSL_MSC_CSW_6] =
                                             CSL_MSC_CSW_STATUS_COMMAND_FAILED;
                                  pMscHandle->lun[lunNum].verifyFlag =
                                             CSL_MSC_VERIFY_FAILED;
                                  /* Change the media state to not present or not bad */
                                  pMscHandle->lun[lunNum].mediaState &=
                                        ~(CSL_MSC_MEDIA_PRESENT | CSL_MSC_MEDIA_BAD);

                                  break;

                            case CSL_MSC_MEDIACCESS_WRITEPROTECT:
                                  MSC_SetSenseKeys(pMscHandle->senseData,
                                      CSL_MSC_SCSI_DATA_PROTECT,
                                      CSL_MSC_SCSI_ASC_WRITE_PROTECTED);
                                  pMscHandle->csw[CSL_MSC_CSW_4] =
                                    (Uint16)(pMscHandle->cbwDataTransferLength &
                                             CSL_MSC_16BIT_MASK);
                                  pMscHandle->csw[CSL_MSC_CSW_5] =
                                    (Uint16)(pMscHandle->cbwDataTransferLength >>
                                             CSL_MSC_16BIT_SHIFT);
                                  pMscHandle->csw[CSL_MSC_CSW_6] =
                                             CSL_MSC_CSW_STATUS_COMMAND_FAILED;
                                  pMscHandle->lun[lunNum].verifyFlag =
                                             CSL_MSC_VERIFY_FAILED;
                                  /* Change the media state to not present or not bad */
                                  pMscHandle->lun[lunNum].mediaState &=
                                        ~(CSL_MSC_MEDIA_PRESENT | CSL_MSC_MEDIA_BAD);

                                  break;

                            /* Check for bad media */
                            case CSL_MSC_MEDIACCESS_BADMEDIA:
                                  MSC_SetSenseKeys(pMscHandle->senseData,
                                      CSL_MSC_SCSI_SENSEKEY_NOT_READY,
                                      CSL_MSC_SCSI_ASC_MEDIA_MANUALINTERVENTION_REQUIRED);
                                  pMscHandle->csw[CSL_MSC_CSW_4] =
                                    (Uint16)(pMscHandle->cbwDataTransferLength &
                                             CSL_MSC_16BIT_MASK);
                                  pMscHandle->csw[CSL_MSC_CSW_5] =
                                    (Uint16)(pMscHandle->cbwDataTransferLength >>
                                             CSL_MSC_16BIT_SHIFT);
                                  pMscHandle->csw[CSL_MSC_CSW_6] =
                                             CSL_MSC_CSW_STATUS_COMMAND_FAILED;
                                  pMscHandle->lun[lunNum].verifyFlag =
                                             CSL_MSC_VERIFY_FAILED;
                                  pMscHandle->lun[lunNum].mediaState &=
                                             ~CSL_MSC_MEDIA_PRESENT;
                                  pMscHandle->lun[lunNum].mediaState |=
                                             CSL_MSC_MEDIA_BAD;

                                  break;
                            default:
                                  break;
                         }
                    }
                }
            }
        }

        if(pMscHandle->lun[lunNum].lbaCnt > 0)
        {
            /* Check if host wants to receive less amount of data than
               the device intends to send */
            if(pMscHandle->cbwDataTransferLength == 0)
            {
                status = MSC_sendCswWithPhaseError(pMscHandle,
                                                   &pMscHandle->bulkInEpObj);
            }
            else
            {
                pMscHandle->lun[lunNum].mediaState |= CSL_MSC_MEDIA_WRITEPENDING;
                pMscHandle->lun[lunNum].lbaCnt--;
                if(pMscHandle->lun[lunNum].lbaSize <
                   pMscHandle->cbwDataTransferLength)
                {
                    tempData = pMscHandle->lun[lunNum].lbaSize;
                }
                else
                {
                    tempData = (Uint16)((pMscHandle->cbwDataTransferLength) &
                                         CSL_MSC_16BIT_MASK);
                }

                status = USB_postTransaction(&pMscHandle->bulkOutEpObj, tempData,
                                             &pMscHandle->lbaBuffer[0],
                                             CSL_USB_IOFLAG_NOSHORT);
            }
        }
        else /* lbaCount == 0 */
        {
            /* Check if host intends to send more data than
               the device intends to receive. If this condition
               is true, extra data received from the host need
               not be transferred to the media. This will be
               controlled by the flag 'pMscHandle->writeToMedia' */
            if (pMscHandle->cbwDataTransferLength != 0)
            {
                /* Using 'writeToMedia' flag to set the data residue for
                   first time */
                if (pMscHandle->writeToMedia == TRUE)
                {
                    pMscHandle->writeToMedia = FALSE;
                      pMscHandle->csw[CSL_MSC_CSW_4] =
                        (Uint16)(pMscHandle->cbwDataTransferLength &
                                 CSL_MSC_16BIT_MASK);
                      pMscHandle->csw[CSL_MSC_CSW_5] =
                        (Uint16)(pMscHandle->cbwDataTransferLength >>
                                 CSL_MSC_16BIT_SHIFT);
                      pMscHandle->csw[CSL_MSC_CSW_6] =
                                 CSL_MSC_CSW_STATUS_COMMAND_PASSED;
                }

                if(pMscHandle->lun[lunNum].lbaSize <
                   pMscHandle->cbwDataTransferLength)
                {
                    tempData = pMscHandle->lun[lunNum].lbaSize;
                }
                else
                {
                    tempData = (Uint16)((pMscHandle->cbwDataTransferLength) &
                                         CSL_MSC_16BIT_MASK);
                }

                pMscHandle->cbwDataTransferLength -= tempData;

                status = USB_postTransaction(&pMscHandle->bulkOutEpObj, tempData,
                                             &pMscHandle->lbaBuffer[0],
                                             CSL_USB_IOFLAG_NOSHORT);
            }
            else
            {
                pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_CSW;
                status = USB_postTransaction(&pMscHandle->bulkInEpObj,
                                             CSL_MSC_CSW_DATA_SIZE,
                                             &pMscHandle->csw[0],
                                             CSL_USB_IOFLAG_NONE);
            }
        }
      }

    return(status);
}

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
                                   Uint16           logicalUnit)
{
    CSL_Status            status;
    CSL_MscMediaStatus    mediaPresentStat;
    CSL_MscMediaStatus    mediaInitStat;

    if (pMscHandle->cbwDataTransferLength != 0)
    {
        /* Stall the end point */
        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_STALL;
        status = USB_stallEndpt(&pMscHandle->bulkOutEpObj);
        status = USB_stallEndpt(&pMscHandle->bulkInEpObj);


        /* Dummy data transfer to intimate MSC_bulk function */
        status |= USB_postTransaction(&pMscHandle->bulkInEpObj,
                                     0,
                                     pMscHandle->lbaBuffer,
                                     CSL_USB_IOFLAG_NOT);

        return(status);
    }

    /* Check if the media is now present */
    mediaPresentStat = pMscHandle->mediaGetPresentState(logicalUnit);

    pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;

    if(mediaPresentStat == CSL_MSC_MEDIACCESS_SUCCESS)
    {
        /* Check if media present */
        if(!(pMscHandle->lun[logicalUnit].mediaState & CSL_MSC_MEDIA_PRESENT))
        {

            mediaInitStat = pMscHandle->mediaInit(logicalUnit);

            /* Get the size of the media */
            pMscHandle->lun[logicalUnit].mediaSize =
                           (pMscHandle->getMediaSize(logicalUnit))-1;

            if(mediaInitStat == CSL_MSC_MEDIACCESS_SUCCESS)
            {
              MSC_SetSenseKeys(pMscHandle->senseData,
                  CSL_MSC_SCSI_SENSEKEY_UNIT_ATTENTION,
                  CSL_MSC_SCSI_ASC_MEDIA_MAY_HAVE_CHANGED);
              pMscHandle->lun[logicalUnit].mediaState |= CSL_MSC_MEDIA_PRESENT;
              pMscHandle->lun[logicalUnit].mediaState &= ~CSL_MSC_MEDIA_BAD;
            }
            else if(mediaInitStat == CSL_MSC_MEDIACCESS_NOTPRESENT)
            {
              MSC_SetSenseKeys(pMscHandle->senseData,
                  CSL_MSC_SCSI_SENSEKEY_NOT_READY,
                  CSL_MSC_SCSI_ASC_MEDIA_NOT_PRESENT);
              /* Change the media state to not present or not bad */
              pMscHandle->lun[logicalUnit].mediaState &=
                               ~(CSL_MSC_MEDIA_PRESENT | CSL_MSC_MEDIA_BAD);
            }
            else
            {
                if (mediaInitStat == CSL_MSC_MEDIACCESS_BADMEDIA)
                {
                    pMscHandle->csw[CSL_MSC_CSW_6] =
                                CSL_MSC_CSW_STATUS_COMMAND_PASSED;
                    MSC_SetSenseKeys(pMscHandle->senseData,
                        CSL_MSC_SCSI_SENSEKEY_UNIT_ATTENTION,
                        CSL_MSC_SCSI_ASC_MEDIA_MAY_HAVE_CHANGED);
                    pMscHandle->lun[logicalUnit].mediaState &=
                                ~CSL_MSC_MEDIA_PRESENT;
                    /* Change mediaState to BAD */
                    pMscHandle->lun[logicalUnit].mediaState |=
                                CSL_MSC_MEDIA_BAD;
                }
            }
        }
        else
        {
            MSC_SetSenseKeys(pMscHandle->senseData,
                CSL_MSC_SCSI_SENSEKEY_NO_SENSE,
                CSL_MSC_SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION);
            pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;
        }
    }
    else if(mediaPresentStat == CSL_MSC_MEDIACCESS_NOTPRESENT)
    {
         MSC_SetSenseKeys(pMscHandle->senseData,
             CSL_MSC_SCSI_SENSEKEY_NOT_READY,
             CSL_MSC_SCSI_ASC_MEDIA_NOT_PRESENT);
         /* Change the media state to not present or not bad */
         pMscHandle->lun[logicalUnit].mediaState &=
                     ~(CSL_MSC_MEDIA_PRESENT | CSL_MSC_MEDIA_BAD);
    }
    else
    {
        if (mediaPresentStat == CSL_MSC_MEDIACCESS_BADMEDIA)
        {
            pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;
            MSC_SetSenseKeys(pMscHandle->senseData,
                CSL_MSC_SCSI_SENSEKEY_NO_SENSE,
                CSL_MSC_SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION);
            /* Change media state to BAD */
            pMscHandle->lun[logicalUnit].mediaState |= CSL_MSC_MEDIA_BAD;
            pMscHandle->lun[logicalUnit].mediaState &= ~CSL_MSC_MEDIA_PRESENT;
        }
    }

    status = USB_postTransaction(hUsbInEp, CSL_MSC_CSW_DATA_SIZE,
                                 &pMscHandle->csw[0], CSL_USB_IOFLAG_NOSHORT);
    pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_CSW;

    return(status);
}

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
                                              Uint16           logicalUnit)
{
    CSL_MscMediaStatus    mediaPresentStat;
    CSL_MscMediaStatus    mediaLockStat;
    CSL_Status            status;
    Uint16                preventAllowStat;

    if (pMscHandle->cbwDataTransferLength != 0)
    {
        /* Stall the end point */
        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_STALL;
        status = USB_stallEndpt(&pMscHandle->bulkOutEpObj);
        status = USB_stallEndpt(&pMscHandle->bulkInEpObj);


        /* Dummy data transfer to intimate MSC_bulk function */
        status |= USB_postTransaction(&pMscHandle->bulkInEpObj,
                                     0,
                                     pMscHandle->lbaBuffer,
                                     CSL_USB_IOFLAG_NOT);

        return(status);
    }

    /* Default Status is Passed  */
    pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;
    /* Setting Default Sense keys */
    MSC_SetSenseKeys(pMscHandle->senseData,
        CSL_MSC_SCSI_SENSEKEY_NO_SENSE,
        CSL_MSC_SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION);

    /* Getting Prevent/Allow Opcode from the CBW */
    preventAllowStat = pMscHandle->cbw[CSL_MSC_CBW_9] >> CSL_MSC_8BIT_SHIFT;

    /* If the Device is not removable please mark the command as illegal request */
    if(!(pMscHandle->lun[logicalUnit].removeable))
    {
        MSC_SetSenseKeys(pMscHandle->senseData,
            CSL_MSC_SCSI_SENSEKEY_ILLEGAL_REQUEST,
            CSL_MSC_SCSI_ASC_INVALID_COMMAND_OPERATION_CODE);
        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;
    }
    else
    {
        /* Prevent media removal */
        if(preventAllowStat == TRUE)
        {
            /* Check if media is present */
            mediaPresentStat = pMscHandle->mediaGetPresentState(logicalUnit);

            if((mediaPresentStat == CSL_MSC_MEDIACCESS_SUCCESS) ||
               (mediaPresentStat == CSL_MSC_MEDIACCESS_BADMEDIA))
            {
                mediaLockStat = pMscHandle->mediaLockUnit(logicalUnit,
                                                          CSL_MSC_LOCK);
                if(mediaLockStat == CSL_MSC_MEDIACCESS_SUCCESS)
                {
                    pMscHandle->lun[logicalUnit].mediaState |=
                                                 CSL_MSC_MEDIA_LOCKED;
                }
                else
                {
                    MSC_SetSenseKeys(pMscHandle->senseData,
                        CSL_MSC_SCSI_SENSEKEY_ILLEGAL_REQUEST,
                        CSL_MSC_SCSI_ASC_INVALID_COMMAND_OPERATION_CODE);
                    pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;
                }
            }
            else
            {
                MSC_SetSenseKeys(pMscHandle->senseData,
                    CSL_MSC_SCSI_SENSEKEY_NOT_READY,
                    CSL_MSC_SCSI_ASC_MEDIA_NOT_PRESENT);
                /* Change the media state to not present or not bad */
                pMscHandle->lun[logicalUnit].mediaState &=
                               ~(CSL_MSC_MEDIA_PRESENT | CSL_MSC_MEDIA_BAD);
                pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;
            }
        }
        /* Allow media removal */
        else
        {
            mediaLockStat = pMscHandle->mediaLockUnit(logicalUnit, CSL_MSC_UNLOCK);
            if(mediaLockStat == CSL_MSC_MEDIACCESS_SUCCESS)
            {
                pMscHandle->lun[logicalUnit].mediaState &=
                                             ~CSL_MSC_MEDIA_LOCKED;
            }
            else
            {
                MSC_SetSenseKeys(pMscHandle->senseData,
                    CSL_MSC_SCSI_SENSEKEY_ILLEGAL_REQUEST,
                    CSL_MSC_SCSI_ASC_INVALID_COMMAND_OPERATION_CODE);
                pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;
            }
        }
    }

    status = USB_postTransaction(hUsbInEp, CSL_MSC_CSW_DATA_SIZE,
                                 &pMscHandle->csw[0], CSL_USB_IOFLAG_NONE);
    pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_CSW;

    return(status);
}

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
                                  Uint16           logicalUnit)
{
    CSL_Status    status;
    Uint16        capacityHi;
    Uint16        capacityLo;
    Uint16        smallerLen;
    CSL_MscMediaStatus    mediaPresentStat;

    /* Check whether the data direction is proper or not */
    if(((pMscHandle->cbw[CSL_MSC_CBW_6] &
          CSL_MSC_CBW_DIRBIT_MASK) != CSL_MSC_CBW_DATADIR_IN) &&
        (pMscHandle->cbwDataTransferLength != 0))
    {
        status = MSC_handleDataDirMisMatch(pMscHandle, CSL_MSC_CBW_DATADIR_IN);

        return(status);
    }

    if (pMscHandle->cbwDataTransferLength > 0)
    {
        capacityHi = (pMscHandle->lun[logicalUnit].mediaSize >>
                      CSL_MSC_16BIT_SHIFT) & CSL_MSC_16BIT_MASK;
        capacityLo = pMscHandle->lun[logicalUnit].mediaSize & CSL_MSC_16BIT_MASK;

        pMscHandle->readCapacity[CSL_MSC_RC_1] =
           (capacityHi >> CSL_MSC_8BIT_SHIFT) | (capacityHi << CSL_MSC_8BIT_SHIFT);
        pMscHandle->readCapacity[CSL_MSC_RC_2] =
           (capacityLo >> CSL_MSC_8BIT_SHIFT) | (capacityLo << CSL_MSC_8BIT_SHIFT);

        /* CapacityHi and capacityLo is reused for the LBA size */
        capacityHi = (pMscHandle->lun[logicalUnit].lbaSize >>
                      CSL_MSC_16BIT_SHIFT) & CSL_MSC_16BIT_MASK;
        capacityLo = pMscHandle->lun[logicalUnit].lbaSize & CSL_MSC_16BIT_MASK;

        pMscHandle->readCapacity[CSL_MSC_RC_3] =
           (capacityHi >> CSL_MSC_8BIT_SHIFT) | (capacityHi << CSL_MSC_8BIT_SHIFT);
        pMscHandle->readCapacity[CSL_MSC_RC_4] =
           (capacityLo >> CSL_MSC_8BIT_SHIFT) | (capacityLo << CSL_MSC_8BIT_SHIFT);

        /* Getting the smaller of the two values */
        if((Uint32)pMscHandle->readCapacity[0] < pMscHandle->cbwDataTransferLength)
        {
            smallerLen = pMscHandle->readCapacity[0];
        }
        else
        {
            smallerLen = (Uint16)(pMscHandle->cbwDataTransferLength &
                                  CSL_MSC_16BIT_MASK);
        }

        /* Calculating the Data Residue */
        pMscHandle->cbwDataTransferLength -= (Uint32)smallerLen;
        pMscHandle->csw[CSL_MSC_CSW_4] =
                (Uint16)(pMscHandle->cbwDataTransferLength & CSL_MSC_16BIT_MASK);
        pMscHandle->csw[CSL_MSC_CSW_5] =
                (Uint16)(pMscHandle->cbwDataTransferLength >> CSL_MSC_16BIT_SHIFT);

        /* Default is failure */
        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;

        if(pMscHandle->lun[logicalUnit].mediaState & CSL_MSC_MEDIA_PRESENT)
        {
            mediaPresentStat = pMscHandle->mediaGetPresentState(logicalUnit);
            if(mediaPresentStat == CSL_MSC_MEDIACCESS_SUCCESS)
            {
                pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;
                MSC_SetSenseKeys(pMscHandle->senseData,
                    CSL_MSC_SCSI_SENSEKEY_NO_SENSE,
                    CSL_MSC_SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION);
            }
            else if(mediaPresentStat == CSL_MSC_MEDIACCESS_BADMEDIA)
            {
                pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;
                MSC_SetSenseKeys(pMscHandle->senseData,
                    CSL_MSC_SCSI_SENSEKEY_NO_SENSE,
                    CSL_MSC_SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION);
                pMscHandle->lun[logicalUnit].mediaState &= ~CSL_MSC_MEDIA_PRESENT;
                pMscHandle->lun[logicalUnit].mediaState |= CSL_MSC_MEDIA_BAD;
            }
            else if(mediaPresentStat == CSL_MSC_MEDIACCESS_NOTPRESENT)
            {
                MSC_SetSenseKeys(pMscHandle->senseData,
                    CSL_MSC_SCSI_SENSEKEY_NOT_READY,
                    CSL_MSC_SCSI_ASC_MEDIA_NOT_PRESENT);
                /* Change the media state to not present or not bad */
                pMscHandle->lun[logicalUnit].mediaState &=
                                ~(CSL_MSC_MEDIA_PRESENT | CSL_MSC_MEDIA_BAD);
            }
        }
        else if(pMscHandle->lun[logicalUnit].mediaState & CSL_MSC_MEDIA_BAD)
        {
            pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;
            MSC_SetSenseKeys(pMscHandle->senseData,
                CSL_MSC_SCSI_SENSEKEY_NO_SENSE,
                CSL_MSC_SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION);
        }
        else
        {
            MSC_SetSenseKeys(pMscHandle->senseData,
                CSL_MSC_SCSI_SENSEKEY_NOT_READY,
                CSL_MSC_SCSI_ASC_MEDIA_NOT_PRESENT);
        }

        status = USB_postTransaction(hUsbInEp, smallerLen,
                                     &pMscHandle->readCapacity[1],
                                     CSL_USB_IOFLAG_NOSHORT);
        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_DATA;
    }
    else
    {
        status = MSC_sendCswWithPhaseError(pMscHandle, hUsbInEp);
    }

    return(status);
}


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
                                       void                  *pMsc)
{
    Uint16    tempLen;
    Uint16    strLen;
    Uint16    inOutFlag;
    Uint16    count;
    Uint16    temp1;
    Uint16    temp2;
    Uint16    unicodeString[CSL_MSC_UNICODE_SIZE];
    pMscClassHandle       pMSCClassHdl;
    CSL_MscCtrlObject     *pCtrlHandle;
    Uint16    descrLen;

    pMSCClassHdl = (pMscClassHandle)(pMsc);
    pCtrlHandle  = &pMSCClassHdl->ctrlHandle;
    count = 0;

    switch(usbSetup->wValue >> CSL_MSC_8BIT_SHIFT)
    {
        case CSL_USB_DEVICE_DESCRIPTOR_TYPE:
            tempLen = pCtrlHandle->deviceDescr[1]&0xFF;
            /* select the smaller of two  */
            tempLen =
                (tempLen < usbSetup->wLength) ? tempLen : usbSetup->wLength;
            USB_postTransaction(hInEp, tempLen,
                                (void*)&pCtrlHandle->deviceDescr[0],
                                CSL_USB_IOFLAG_NONE);
            break;

        case CSL_USB_DEVICE_QUAL_DESCRIPTOR_TYPE:
            tempLen = pCtrlHandle->deviceQualDescr[1]&0xFF;
            /* select the smaller of two  */
            tempLen =
                (tempLen < usbSetup->wLength) ? tempLen : usbSetup->wLength;
            USB_postTransaction(hInEp, tempLen,
                                (void*)&pCtrlHandle->deviceQualDescr[0],
                                CSL_USB_IOFLAG_NONE);
            break;

        case CSL_USB_CONFIGURATION_DESCRIPTOR_TYPE:
             inOutFlag = (CSL_USB_IOFLAG_LNK | CSL_USB_IOFLAG_CAT);
             tempLen = pCtrlHandle->pCfgDescr[2];
             if(tempLen == usbSetup->wLength)
             {
                inOutFlag |= CSL_USB_IOFLAG_NOSHORT;
            }

              /* select the smaller of two */
             tempLen =
                 (tempLen < usbSetup->wLength) ? tempLen : usbSetup->wLength;

             USB_postTransaction(hInEp, tempLen, pCtrlHandle->pCfgDescr,
                                 inOutFlag);
             break;

        case CSL_USB_OTHERSPEED_CFG_DESCRIPTOR_TYPE:

             inOutFlag = (CSL_USB_IOFLAG_LNK | CSL_USB_IOFLAG_CAT);
             tempLen = pCtrlHandle->pOtherSpeedCfgDescr[2];
             if(tempLen == usbSetup->wLength)
             {
                inOutFlag |= CSL_USB_IOFLAG_NOSHORT;
            }

             /* select the smaller of two */
             tempLen =
                 (tempLen < usbSetup->wLength) ? tempLen : usbSetup->wLength;

             USB_postTransaction(hInEp, tempLen,
                                 pCtrlHandle->pOtherSpeedCfgDescr,
                                 inOutFlag);
             break;

        case CSL_USB_STRING_DESCRIPTOR_TYPE:
             memcpy(unicodeString,0,sizeof(unicodeString));
            /* LANGID Language Codes */
             if((usbSetup->wValue & 0xFF) == 0)
             {
                 tempLen = pCtrlHandle->stringDescrLangid[1] & CSL_MSC_8BIT_MASK;
                   /* select the smaller of two */
                   tempLen =
                      (tempLen < usbSetup->wLength) ? tempLen : usbSetup->wLength;
                   USB_postTransaction(hInEp, tempLen,
                                       &pCtrlHandle->stringDescrLangid[0],
                                       CSL_USB_IOFLAG_NONE);
             }
             else
             {
                 /* strlen of the string requested */
                 strLen = strlen(pCtrlHandle->strDescr[usbSetup->wValue &
                                                       CSL_MSC_8BIT_MASK]);
                /* Descriptor length equal to two time of string
                   length plus two bytes for sending descriptor length
                   and descriptor type */
                descrLen = 2*(strLen) + 2;

                   /* select the smallest of two */
                   tempLen =
                       (descrLen < usbSetup->wLength) ? descrLen : usbSetup->wLength;

                /* Convert the ASCII string to UNICODE string */
                   for(count = 0; count < strLen; count++)
                   {
                    temp1 = (pCtrlHandle->strDescr[usbSetup->wValue &
                                          CSL_MSC_8BIT_MASK][count]);
                    temp2 =  0x00;
                    unicodeString[count + 2] = (temp2 << CSL_MSC_8BIT_SHIFT) | temp1;
                   }

                   unicodeString[1] = ((CSL_USB_STRING_DESCRIPTOR_TYPE <<
                                       CSL_MSC_8BIT_SHIFT) | descrLen);

                   USB_postTransaction(hInEp, tempLen , &unicodeString[0],
                                       CSL_USB_IOFLAG_NONE);
             }
            break;

        default:
            return(CSL_MSC_REQUEST_STALL);
    }

    return(CSL_MSC_REQUEST_GET_ACK);
}

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
                                     void                  *pMsc)
{
    pMscClassHandle     pMscClassHdl;
    CSL_MscObject       *pMscHandle;
    pUsbEpHandle        hUsbInEp;
    pUsbEpHandle        hUsbOutEp;
    CSL_Status          status;
    CSL_MscRequestRet   retStatus;

    status    = CSL_SOK;
    retStatus = CSL_MSC_REQUEST_STALL;

    if(pMsc != NULL)
    {
        pMscClassHdl = (pMscClassHandle)(pMsc);
        pMscHandle   = &pMscClassHdl->mscHandle;
        hUsbOutEp    = &pMscHandle->bulkOutEpObj;
        hUsbInEp     = &pMscHandle->bulkInEpObj;

        status = USB_abortTransaction(hUsbInEp);
        status = USB_abortTransaction(hUsbOutEp);

        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_RESET;

        status = MSC_HandleStateReset(pMscHandle, hUsbOutEp);
        if (status == CSL_SOK)
        {
            retStatus = CSL_MSC_REQUEST_SEND_ACK;
        }
    }

    return(retStatus);
}

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
                                     pUsbEpHandle     hUsbInEp)
{
    CSL_Status    status;

    status = CSL_SOK;

    if ((pMscHandle != NULL) && (hUsbInEp != NULL))
    {
        pMscHandle->csw[CSL_MSC_CSW_4] =
             (Uint16)(pMscHandle->cbwDataTransferLength & CSL_MSC_16BIT_MASK);
        pMscHandle->csw[CSL_MSC_CSW_5] =
             (Uint16)(pMscHandle->cbwDataTransferLength >> CSL_MSC_16BIT_SHIFT);

        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_PHASE_ERROR;
        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_CSW;

        /* Sending CSW with phase error */
        status = USB_postTransaction(hUsbInEp, CSL_MSC_CSW_DATA_SIZE,
                                     &pMscHandle->csw[0],
                                     CSL_USB_IOFLAG_NOSHORT);
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

    return (status);
}

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
                                      Uint16           dataDir)
{
    CSL_Status    status;

    status = CSL_SOK;

    if (pMscHandle != NULL)
    {
        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_PHASE_ERROR;

        /* Stall the end points */
        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_STALL;

        if (dataDir == CSL_MSC_CBW_DATADIR_IN)
        {
            status  = USB_stallEndpt(&pMscHandle->bulkInEpObj);
            status |= USB_stallEndpt(&pMscHandle->bulkOutEpObj);
        }
        else
        {
            status = USB_stallEndpt(&pMscHandle->bulkInEpObj);
        }

        /* Dummy data transfer to intimate MSC_bulk function */
        status |= USB_postTransaction(&pMscHandle->bulkInEpObj, 0,
                                      pMscHandle->lbaBuffer,
                                      CSL_USB_IOFLAG_NOT);
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return (status);
}

