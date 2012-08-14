/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_audioClassAux.h
 *
 *  @brief USB Audio Class functional layer Auxilliary header file
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

#ifndef _CSL_AUDIOCLASSAUX_H_
#define _CSL_AUDIOCLASSAUX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csl_usbAux.h"
#include "csl_audioClass.h"

/** @addtogroup CSL_AC_FUNCTION
 @{ */

/** ============================================================================
 *   @n@b  AC_reqUnknown
 *
 *   @b Description
 *   @n This function handles the case where the request sent by the host is unknown
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
static inline
CSL_AcRequestRet AC_reqUnknown(CSL_UsbDevNum         devNum,
                               CSL_UsbSetupStruct    *usbSetup,
                               pUsbEpHandle          hInEp,
                               pUsbEpHandle          hOutEp,
                               void                  *pAcObj)
{
    CSL_AcRequestRet    retStat;

    /* STALL the endpoint - the request is either not known or not supported */
    retStat = CSL_AC_REQUEST_STALL;

    return(retStat);
}

/** ============================================================================
 *   @n@b AC_lookupReqHandler
 *
 *   @b Description
 *   @n This functions maps the request sent by host to the Ac request table
 *   and returns the correspending request handler address
 *
 *   @b Arguments
 *   @verbatim
            request        -  Request sent by the host
            pUSB_ReqTable  -  Request table structure pointer
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Request handler pointer
 *
 *   <b> Pre Condition </b>
 *   @n  Request table should be initialized and AC_Open function should be
 *       called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns the request handler pointer
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_AcRequestRet       (*fpRequestHandler)(CSL_AC_REQUEST_ARGS)
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
     @endverbatim
 *  ============================================================================
 */
static inline
fpAC_REQ_HANDLER AC_lookupReqHandler(Uint16                 request,
                                     CSL_AcRequestStruct    *pUSB_ReqTable)
{
    Uint16    index;

    /* parse thru the end of request handler table */
    for(index = 0; pUSB_ReqTable[index].fpRequestHandler != NULL; index++)
    {
        /* if request handler exists return a pointer to the request handler routine */
        if(pUSB_ReqTable[index].request == request)
          {
            return(pUSB_ReqTable[index].fpRequestHandler);
          }
    }

    /* if request handler does not exist return a pointer to the USB_reqUnknown
    routine */
    return(AC_reqUnknown);
}

/** ============================================================================
 *   @n@b  AC_reqSetAddress
 *
 *   @b Description
 *   @n This function handles the request to set the device address
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
 *   @li                   CSL_AC_REQUEST_DONE - Request hadled successfully
 *
 *   <b> Pre Condition </b>
 *   @n  AC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets the device address
 *
 *   @b Modifies
 *   @n USB FADDR_POWER register
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
static inline
CSL_AcRequestRet AC_reqSetAddress(CSL_UsbDevNum         devNum,
                                  CSL_UsbSetupStruct    *usbSetup,
                                  pUsbEpHandle          hInEp,
                                  pUsbEpHandle          hOutEp,
                                  void                  *pAcObj)
{
    USB_setDevAddr(devNum, (Uint16)(usbSetup->wValue));

    return(CSL_AC_REQUEST_DONE);
}

/** ============================================================================
 *   @n@b  AC_reqSetConfiguration
 *
 *   @b Description
 *   @n This function handles the request to set the device configuration
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
 *   @n  Sets the configuration
 *
 *   @b Modifies
 *   @n USB registers
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
static inline
CSL_AcRequestRet AC_reqSetConfiguration(CSL_UsbDevNum         devNum,
                                        CSL_UsbSetupStruct    *usbSetup,
                                        pUsbEpHandle          hInEp,
                                        pUsbEpHandle          hOutEp,
                                        void                  *pAcObj)
{
    CSL_AcRequestRet    retStat;

    pAcClassHandle      pAcClassHdl;
    CSL_AcCtrlObject    *pCtrlHandle;

    pAcClassHdl = (pAcClassHandle)(pAcObj);
    pCtrlHandle  = &pAcClassHdl->ctrlHandle;

    if((usbSetup->wValue == FALSE) || (usbSetup->wValue == TRUE))
    {
        pCtrlHandle->curConfigStat = usbSetup->wValue;

        USB_setConfiguration(devNum,usbSetup->wValue);

        retStat   = CSL_AC_REQUEST_SEND_ACK;
    }
    else
    {
        /* configuration not supported, STALL the endpoint */
        retStat = CSL_AC_REQUEST_STALL;
    }

    return(retStat);
}

/** ============================================================================
 *   @n@b  AC_reqClearFeature
 *
 *   @b Description
 *   @n This function handles the request to clear the feature set
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
 *   @n  Clears the feature
 *
 *   @b Modifies
 *   @n USB registers
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
static inline
CSL_AcRequestRet AC_reqClearFeature(CSL_UsbDevNum         devNum,
                                    CSL_UsbSetupStruct    *usbSetup,
                                    pUsbEpHandle          hInEp,
                                    pUsbEpHandle          hOutEp,
                                    void                  *pAcObj)
{
    CSL_AcRequestRet    retStat;
    pUsbEpHandle         hEPx;
    Uint16               endpt;  /* this is USB logical endpoint */

    retStat = CSL_AC_REQUEST_SEND_ACK;

    switch(usbSetup->wValue)
    {
        case CSL_USB_FEATURE_ENDPOINT_STALL:
              endpt = (usbSetup->wIndex) & CSL_AC_8BIT_MASK;
              hEPx = USB_epNumToHandle(devNum, endpt);
            USB_clearEndptStall(hEPx);
            break;

        case CSL_USB_FEATURE_REMOTE_WAKEUP:
            USB_setRemoteWakeup(devNum, (CSL_UsbBoolean)FALSE);
             break;

        default:
            /* Unsupported Feature. STALL the endpoint */
            retStat = CSL_AC_REQUEST_STALL;
            break;
    }

    return(retStat);
}

/** ============================================================================
 *   @n@b  AC_reqGetStatus
 *
 *   @b Description
 *   @n This function handles the get status request from host
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
 *   @n  Sends the status
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
static inline
CSL_AcRequestRet AC_reqGetStatus(CSL_UsbDevNum         devNum,
                                 CSL_UsbSetupStruct    *usbSetup,
                                 pUsbEpHandle          hInEp,
                                 pUsbEpHandle          hOutEp,
                                 void                  *pAcObj)
{
    CSL_AcRequestRet     retStat;
    pAcClassHandle       pAcClassHdl;
    CSL_AcCtrlObject     *pCtrlHandle;
    pUsbEpHandle          hEPx;
    CSL_Status            status;
    Uint16                endpt;   /* this is USB logical endpoint */

    pAcClassHdl = (pAcClassHandle)(pAcObj);
    pCtrlHandle  = &pAcClassHdl->ctrlHandle;
    retStat      = CSL_AC_REQUEST_GET_ACK;

    switch(usbSetup->bmRequestType - CSL_AC_REQUEST_TYPE_BASE)
    {
        /* Device Status to be returned */
        case CSL_AC_REQUEST_TYPE_DEVICE_STATUS:
            pCtrlHandle->ctrlBuffer[1] =
            (((Uint16)USB_getRemoteWakeupStat(devNum))<<1) |
             CSL_AC_CURRDEV_STAT;
            USB_postTransaction(hInEp, 2, &pCtrlHandle->ctrlBuffer,
                                CSL_USB_IOFLAG_NONE);
            break;

        /* Interface status is to be returned */
        case CSL_AC_REQUEST_TYPE_INTERFACE_STATUS:
            pCtrlHandle->ctrlBuffer[1] = CSL_AC_CURRDEV_STAT;
            USB_postTransaction(hInEp, 2, &pCtrlHandle->ctrlBuffer,
                                CSL_USB_IOFLAG_NONE);
            break;

        /* Endpoint status to be returned */
        case CSL_AC_REQUEST_TYPE_EP_STATUS:

            endpt  =  usbSetup->wIndex & 0xFF;
            hEPx   =  USB_epNumToHandle(devNum, endpt);
            pCtrlHandle->ctrlBuffer[1] = (Uint16)USB_getEndptStall(hEPx,
                                                                   &status);
            USB_postTransaction(hInEp, 2, &pCtrlHandle->ctrlBuffer,
                                CSL_USB_IOFLAG_NONE);
            break;

        default:
            /* STALL the endpoint */
            retStat = CSL_AC_REQUEST_STALL;
            break;
    }

    return(retStat);
}

/** ============================================================================
 *   @n@b  AC_reqSetFeature
 *
 *   @b Description
 *   @n This function handles the request from host to set feature
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
 *   @n  Sets the feature
 *
 *   @b Modifies
 *   @n USB regiters
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
static inline
CSL_AcRequestRet AC_reqSetFeature(CSL_UsbDevNum         devNum,
                                  CSL_UsbSetupStruct    *usbSetup,
                                  pUsbEpHandle          hInEp,
                                  pUsbEpHandle          hOutEp,
                                  void                  *pAcObj)
{
    CSL_AcRequestRet    retStat;
    pUsbEpHandle         hEPx;
    Uint16               endpt;        /* this is USB logical endpoint */

    retStat = CSL_AC_REQUEST_SEND_ACK;

    switch(usbSetup->wValue)
    {
        case CSL_USB_FEATURE_ENDPOINT_STALL:
            /* updated set and clear endpoint stall to work with logical endpoint num */
            endpt = (usbSetup->wIndex) & CSL_AC_8BIT_MASK;
            hEPx = USB_epNumToHandle(devNum, endpt);
            USB_stallEndpt(hEPx);
            break;

        case CSL_USB_FEATURE_REMOTE_WAKEUP:
            USB_setRemoteWakeup(devNum, (CSL_UsbBoolean)TRUE);
            break;

        default:
            /* Feature not supported, STALL the endpoint */
            retStat = CSL_AC_REQUEST_STALL;
            break;
    }

  return(retStat);
}

/** ============================================================================
 *   @n@b  AC_reqGetConfiguration
 *
 *   @b Description
 *   @n This function handles the request from host to get configuration
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
 *
 *   <b> Pre Condition </b>
 *   @n  AC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends the configuration
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
static inline
CSL_AcRequestRet AC_reqGetConfiguration(CSL_UsbDevNum         devNum,
                                        CSL_UsbSetupStruct    *usbSetup,
                                        pUsbEpHandle          hInEp,
                                        pUsbEpHandle          hOutEp,
                                        void                  *pAcObj)
{
    pAcClassHandle       pAcClassHdl;
    CSL_AcCtrlObject*    pCtrlHandle;

    pAcClassHdl = (pAcClassHandle)(pAcObj);
    pCtrlHandle  = &pAcClassHdl->ctrlHandle;

    /* Send the current Configuration Value */
    pCtrlHandle->ctrlBuffer[1] = pCtrlHandle->curConfigStat;
    USB_postTransaction(hInEp, 1, (void*)&pCtrlHandle->ctrlBuffer,
                        CSL_USB_IOFLAG_NONE | CSL_USB_IOFLAG_NOSHORT);

    return(CSL_AC_REQUEST_GET_ACK);
}

/** ============================================================================
 *   @n@b  AC_reqGetMaxLUN
 *
 *   @b Description
 *   @n This function handles the request from host to send maximum logical
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
 *   @li        CSL_AC_REQUEST_GET_ACK  - Get acknowledgement for request\
 *   @li        CSL_AC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  AC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends the max lun number
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
static inline
CSL_AcRequestRet AC_reqGetMaxLUN(CSL_UsbDevNum         devNum,
                                 CSL_UsbSetupStruct    *usbSetup,
                                 pUsbEpHandle          hInEp,
                                 pUsbEpHandle          hOutEp,
                                 void                  *pAcObj)
{
    pAcClassHandle       pAcClassHdl;
    CSL_AcCtrlObject*    pCtrlHandle;
    CSL_AcRequestRet     retStat;

    pAcClassHdl = (pAcClassHandle)(pAcObj);
    pCtrlHandle  = &pAcClassHdl->ctrlHandle;
    pCtrlHandle->ctrlBuffer[1] = pAcClassHdl->acHandle.noOfLun;

    if(pAcClassHdl->acHandle.noOfLun > 0)
    {
        USB_postTransaction(hInEp, 1, &pCtrlHandle->ctrlBuffer[0],
                            CSL_USB_IOFLAG_NONE);

        /*Receive a 0 length packet for acknowledgement */
        retStat = CSL_AC_REQUEST_GET_ACK;
    }
    else
    {
        retStat = CSL_AC_REQUEST_STALL;
    }

    return (retStat);
}

/** ============================================================================
 *   @n@b  AC_reqGetInterface
 *
 *   @b Description
 *   @n This function handles the request from host to get interafce
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
 *   @li        CSL_AC_REQUEST_GET_ACK  - Get acknowledgement for request\
 *   @li        CSL_AC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  AC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends interface info
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
static inline
CSL_AcRequestRet AC_reqGetInterface(CSL_UsbDevNum         devNum,
                                    CSL_UsbSetupStruct    *usbSetup,
                                    pUsbEpHandle          hInEp,
                                    pUsbEpHandle          hOutEp,
                                    void                  *pAcObj)
{
    pAcClassHandle       pAcClassHdl;
    CSL_AcCtrlObject     *pCtrlHandle;
    CSL_AcRequestRet     retStat;

    pAcClassHdl = (pAcClassHandle)(pAcObj);
    pCtrlHandle  = &pAcClassHdl->ctrlHandle;
    retStat      = CSL_AC_REQUEST_GET_ACK;

    /* Compare the Interface with the bNumInterfaces byte of Configuration Descriptor */
    if(usbSetup->wIndex == 0)
    {
        /* Send the current Interface Value */
        pCtrlHandle->ctrlBuffer[1] = 0;
        USB_postTransaction(hInEp, 1, &pCtrlHandle->ctrlBuffer[0],
                              CSL_USB_IOFLAG_NONE);
    }
    else
    {
        /*  Interface specified doent exist, STALL the endpoint */
        retStat = CSL_AC_REQUEST_STALL;
    }

    return retStat;
}

/**
@} */

#ifdef __cplusplus
}
#endif

#endif    // _CSL_AUDIOCLASSAUX_H_

