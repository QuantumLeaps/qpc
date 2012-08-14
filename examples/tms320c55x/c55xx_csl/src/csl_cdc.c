/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008, 2011
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_cdc.c
 *
 *  @brief USB CDC functional layer API source file
 *
 *  Path: \(CSLPATH)\ src
 */

/* ============================================================================
 * Revision History
 * ================
 * 02-May-2011 Created
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

#include <csl_usbAux.h>
#include <csl_cdc.h>

/** ============================================================================
 *   @n@b CDC_Open
 *
 *   @b Description
 *   @n This function Opens the CDC module. It initializes CDC and Ctrl handles
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
 *   @li        CSL_ESYS_BADHANDLE        - Invalid CDC handle
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes CDC and Ctrl handles
 *
 *   @b Modifies
 *   @n CDC and Ctrl handle data
 *
 *   @b Example
 *   @verbatim
            CSL_CdcInitStructApp    cdcAppHandle;
            pCdcAppClassHandle      pAppClassHandle;
            Uint16                  cdcAppBuffer[420];
            CSL_Status              status;

            pAppClassHandle = &mscAppHandle;

            // Assign values to all the members in cdcAppHandle struct
            cdcAppHandle.pCdcObj = &cdcAppBuffer[0];
            .....
            .....
            status = CDC_Open(pAppClassHandle);
     @endverbatim
 *  ============================================================================
 */
CSL_Status CDC_Open(pCdcAppClassHandle    pAppClassHandle)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(pAppClassHandle != NULL)
    {
        status = CDC_Setup(pAppClassHandle, TRUE);
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

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
CSL_Status CDC_OpenFullspeed(pCdcAppClassHandle    pAppClassHandle)
{
    CSL_Status    status;

    status = CSL_SOK;

    if(pAppClassHandle != NULL)
    {
        status = CDC_Setup(pAppClassHandle, FALSE);
        USB_setFullSpeedMode(CSL_USB_EP0_PACKET_SIZE);
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

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
                            Bool                  usbSpeedCfg)
{
    pCdcClassHandle       pHandle;
    CSL_Status            status;
    Uint16                eventMask;
    Uint16                maxPktSize;

    status  = CSL_SOK;
    pHandle = (pCdcClassHandle)(pAppClassHandle->pCdcObj);

    if(pAppClassHandle != NULL)
    {
        pHandle->ctrlHandle.devNum = CSL_USB0;

        /* Initializing all default values */
        pHandle->ctrlHandle.suspendFlag   = 0;
        pHandle->ctrlHandle.curConfigStat = 0x0;

        /* hEpObjArray needs to be setup */
        pHandle->ctrlHandle.hEpObjArray[0] = &pHandle->ctrlHandle.ctrlOutEpObj;
        pHandle->ctrlHandle.hEpObjArray[1] = &pHandle->ctrlHandle.ctrlInEpObj;
        pHandle->ctrlHandle.hEpObjArray[2] = &pHandle->cdcHandle.bulkOutEpObj;
        pHandle->ctrlHandle.hEpObjArray[3] = &pHandle->cdcHandle.bulkInEpObj;
        pHandle->ctrlHandle.hEpObjArray[4] = &pHandle->cdcHandle.intrOutEpObj;
        pHandle->ctrlHandle.hEpObjArray[5] = &pHandle->cdcHandle.intrInEpObj;

        pHandle->ctrlHandle.usbSpeedCfg = usbSpeedCfg;

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

        /* Initialized the Interrupt Endpoint OUT */
        eventMask = (CSL_USB_EVENT_RESET | CSL_USB_EVENT_EOT);
        status = USB_initEndptObj(pHandle->ctrlHandle.devNum,
                                  &pHandle->cdcHandle.intrInEpObj,
                                  (CSL_UsbEpNum)(CSL_CDC_INTR_IN_EP+CSL_USB_IN_EP0),
                                  CSL_USB_INTR,
                                  maxPktSize,
                                  eventMask,
                                  pAppClassHandle->intrHandler);
        if(status != CSL_SOK)
        {
            return (status);
        }

        /* Initialized the Bulk Endpoint OUT */
        status = USB_initEndptObj(pHandle->ctrlHandle.devNum,
                                  &pHandle->cdcHandle.bulkOutEpObj,
                                  (CSL_UsbEpNum)CSL_CDC_BULK_OUT_EP,
                                  CSL_USB_BULK,
                                  maxPktSize,
                                  CSL_USB_EVENT_EOT,
                                  pAppClassHandle->bulkHandler);
        if(status != CSL_SOK)
        {
            return (status);
        }

        /* Initialized the Bulk Endpoint IN */
        eventMask = (CSL_USB_EVENT_RESET | CSL_USB_EVENT_EOT);
        status = USB_initEndptObj(pHandle->ctrlHandle.devNum,
                                  &pHandle->cdcHandle.bulkInEpObj,
                                  (CSL_UsbEpNum)(CSL_CDC_BULK_IN_EP+CSL_USB_IN_EP0),
                                  CSL_USB_BULK,
                                  maxPktSize,
                                  eventMask,
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
 *   @n@b CDC_Close
 *
 *   @b Description
 *   @n This function close the Cdc module.
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
CSL_Status CDC_Close(pCdcAppClassHandle    pAppClassHandle)
{
    pCdcClassHandle       pCdcClassHdl;
    CSL_CdcCtrlObject     *pCtrlHandle;
    CSL_Status            status;

    pCdcClassHdl = (pCdcClassHandle)(pAppClassHandle->pCdcObj);
    pCtrlHandle  = &pCdcClassHdl->ctrlHandle;
    status       = CSL_SOK;

    if(pAppClassHandle != NULL)
    {
        status = USB_disconnectDev(pCtrlHandle->devNum);
        pAppClassHandle->pCdcObj = NULL;
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

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
            CSL_MCdcInitStructApp    cdcAppHandle;
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
Uint16 CDC_GetactivityFlagStatus(void    *pCDC)
{
   pCdcClassHandle    pCdcClassHdl;
   CSL_CdcObject      *pCdcHandle;

   pCdcClassHdl = (pCdcClassHandle)(pCDC);
   pCdcHandle   = &pCdcClassHdl->cdcHandle;

   return(pCdcHandle->activityPresentFlag);
}

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
Uint16 CDC_GetsuspendFlagStatus(void    *pCDC)
{
   pCdcClassHandle       pCdcClassHdl;
   CSL_CdcCtrlObject     *pCtrlHandle;

   pCdcClassHdl = (pCdcClassHandle)(pCDC);
   pCtrlHandle  = &pCdcClassHdl->ctrlHandle;

   return(pCtrlHandle->suspendFlag);
}

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
CSL_CdcRequestRet CDC_eventHandler(pUsbContext pContext, CSL_CdcClassStruct *hCdcClass)
{
    Uint16 txBuf[20], saveIndex;
    CSL_UsbSetupStruct usbSetup;
    CSL_CdcObject *pCdcObj = &hCdcClass->cdcHandle;

    saveIndex = usbRegisters->INDEX_TESTMODE;
        CSL_FINS(usbRegisters->INDEX_TESTMODE,
                 USB_INDEX_TESTMODE_EPSEL, CSL_USB_EP0);

    switch ((pContext->usbSetup.bmRequestType<<8)|pContext->usbSetup.bRequest)
    {
        case CSL_CDC_REQUEST_SEND_ENCAPSULATED_COMMAND:

            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_TXPKTRDY, TRUE);
            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_DATAEND, TRUE);
            break;

        case CSL_CDC_REQUEST_GET_ENCAPSULATED_RESPONSE:

            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_TXPKTRDY, TRUE);
            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_DATAEND, TRUE);
            break;

        case CSL_CDC_REQUEST_SET_COMM_FEATURE:

            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_TXPKTRDY, TRUE);
            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_DATAEND, TRUE);
            break;

        case CSL_CDC_REQUEST_GET_COMM_FEATURE:

            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_TXPKTRDY, TRUE);
            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_DATAEND, TRUE);
            break;

        case CSL_CDC_REQUEST_CLEAR_COMM_FEATURE:

            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_TXPKTRDY, TRUE);
            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_DATAEND, TRUE);
            break;

        case CSL_CDC_REQUEST_SET_LINE_CODING:

            if((usbRegisters->PERI_CSR0_INDX & CSL_USB_PERI_CSR0_INDX_RXPKTRDY_MASK)
                == CSL_USB_PERI_CSR0_INDX_RXPKTRDY_MASK)
            {
                /* Service the RXPKTRDY after reading the FIFO */
                CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_SERV_RXPKTRDY, TRUE);
            }
            // send ACK
            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_TXPKTRDY, TRUE);
            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_DATAEND, TRUE);

            // wait for the line coding from host
            while (!((usbRegisters->PERI_CSR0_INDX & CSL_USB_PERI_CSR0_INDX_RXPKTRDY_MASK)
                == CSL_USB_PERI_CSR0_INDX_RXPKTRDY_MASK));
            // get the line coding
            USB_getSetupPacket(CSL_USB0, &usbSetup, TRUE);
            if((usbRegisters->PERI_CSR0_INDX & CSL_USB_PERI_CSR0_INDX_RXPKTRDY_MASK)
                == CSL_USB_PERI_CSR0_INDX_RXPKTRDY_MASK)
            {
                /* Service the RXPKTRDY after reading the FIFO */
                CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_SERV_RXPKTRDY, TRUE);
            }
            // save it to the line coding structure
            pCdcObj->lineCoding.dwBaudRate = (usbSetup.bRequest<<8) | usbSetup.bmRequestType;
            pCdcObj->lineCoding.dwBaudRate |= ((Uint32)usbSetup.wValue<<16);
            pCdcObj->lineCoding.wCharFormat = usbSetup.wIndex&0xFF;
            pCdcObj->lineCoding.wParityType = usbSetup.wIndex>>8;
            pCdcObj->lineCoding.wDataBits = usbSetup.wLength&0xFF;

            break;

        case CSL_CDC_REQUEST_GET_LINE_CODING:

            // send the current line coding to the host
            // first the baud rate
            txBuf[0] = pCdcObj->lineCoding.dwBaudRate&0xFFFF;
            txBuf[1] = pCdcObj->lineCoding.dwBaudRate>>16;
            // second the char format and parity
            txBuf[2] = (pCdcObj->lineCoding.wParityType<<8) | pCdcObj->lineCoding.wCharFormat;
            // last the bits per character
            txBuf[3] = pCdcObj->lineCoding.wDataBits;
            USB_postTransaction(pContext->hEpObjArray[1], 7, (void*)txBuf,
                                CSL_USB_IOFLAG_NONE | CSL_USB_IOFLAG_NOSHORT);
            break;

        case CSL_CDC_REQUEST_SET_CONTROL_LINE_STATE:

            // save it to control line state
            pCdcObj->controlLineState = pContext->usbSetup.wValue&0x03;

            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_TXPKTRDY, TRUE);
            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_DATAEND, TRUE);
            break;

        case CSL_CDC_REQUEST_SEND_BREAK:

            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_TXPKTRDY, TRUE);
            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_DATAEND, TRUE);
            break;

        default:

            break;
    }

    usbRegisters->INDEX_TESTMODE  = saveIndex;

    return CSL_CDC_REQUEST_DONE;
}

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
CSL_Status CDC_bulkOutHandler(pUsbContext pContext, CSL_CdcInitStructApp *cdcAppHandle)
{
    pUsbEpHandle tempEpH;
    Uint16 tempWord, usb_income_num_bytes;
    CSL_Status status, txRxStatus;
    Uint16 bytesRem;

    status = CSL_SOK;

    // read the RX packet size
    // validate the RX packet first
    if (USB_isValidDataInFifoOut(&pContext->pEpStatus[CSL_CDC_BULK_OUT_EP]))
    {
        // get the EP2 handle
        tempEpH = USB_epNumToHandle(CSL_USB0, CSL_CDC_BULK_OUT_EP);
        usb_income_num_bytes = USB_getDataCountReadFromFifo( tempEpH );
    }

    // make sure we have enough space to save the RX data
    if ((cdcAppHandle->rxEndIdx-cdcAppHandle->rxStartIdx+usb_income_num_bytes)<CSL_USB_CDC_DATA_BUF_SIZE)
    {
        if (usb_income_num_bytes==1)
        {
            status = USB_postTransaction(pContext->hEpObjArray[2], usb_income_num_bytes,
                                         &tempWord,
                                         CSL_USB_OUT_TRANSFER);
            if(status != CSL_SOK)
            {
                printf("USB RX Transaction failed\n");
            }

            /* Check the transaction status */
            txRxStatus = USB_isTransactionDone(pContext->hEpObjArray[2], &status);
            if(txRxStatus != CSL_USB_TRUE)
            {
                bytesRem = USB_bytesRemaining(pContext->hEpObjArray[2], &status);
                if(bytesRem)
                {
                    printf("Bytes remaining: %d\n", bytesRem);
                }
            }
        } else
        {
            printf("USB RX multiple bytes\n");
        }

        // copy the character read into the receiving buffer
        cdcAppHandle->rxBufferPtr[cdcAppHandle->rxEndIdx] = tempWord&0xFF;
        // increase the rxEndIdx
        cdcAppHandle->rxEndIdx += usb_income_num_bytes;
        if (cdcAppHandle->rxEndIdx>=CSL_USB_CDC_DATA_BUF_SIZE)
            cdcAppHandle->rxEndIdx = 0;
    } else
    {
        printf("USB RX Buffer is overflow\n");
    }

    return status;
}

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
CSL_Status CDC_bulkInHandler(pUsbContext pContext, CSL_CdcInitStructApp *cdcAppHandle)
{
    CSL_Status status, txRxStatus;
    Uint16 bytesRem, txStartIdxSave, txEndIdxSave, i;

    status = CSL_SOK;

    // Is the TX EP free?
    if (cdcAppHandle->txWorkBufIdx==0)
    {
        // is there anything need to send out
        txStartIdxSave = cdcAppHandle->txStartIdx;
        txEndIdxSave = cdcAppHandle->txEndIdx;
        if (txEndIdxSave==txStartIdxSave)
        {
            // nothing to transmit, then return
            return status;
        } else if (txEndIdxSave>txStartIdxSave)
        {
            // for no wrap around case
            // pack and copy the data from txBufferPtr to txWorkBufPtr
            for (i=0; i<(txEndIdxSave-txStartIdxSave); i++)
            {
                if (i%2)
                    cdcAppHandle->txWorkBufPtr[i/2] = (cdcAppHandle->txBufferPtr[txStartIdxSave+i]<<8);
                else
                    cdcAppHandle->txWorkBufPtr[i/2] = cdcAppHandle->txBufferPtr[txStartIdxSave+i]&0xFF;
            }
            // send whatever in the TX working buffer to host
            status = USB_postTransaction(pContext->hEpObjArray[3], txEndIdxSave-txStartIdxSave,
                                         cdcAppHandle->txWorkBufPtr, CSL_USB_IN_TRANSFER);
            if(status != CSL_SOK)
            {
                printf("USB TX Transaction failed\n");
            }

            /* Check the transaction status */
            txRxStatus = USB_isTransactionDone(pContext->hEpObjArray[3], &status);
            if(txRxStatus != CSL_USB_TRUE)
            {
                bytesRem = USB_bytesRemaining(pContext->hEpObjArray[3], &status);
                if(bytesRem)
                {
                    printf("Bytes remaining: %d\n", bytesRem);
                }
            }

            // update the txStartIdx
            cdcAppHandle->txStartIdx = txEndIdxSave;
        } else if (txEndIdxSave<txStartIdxSave)
        {
            // for wrap around case
            // send the TX buffer (rxStartIdx - CSL_USB_CDC_DATA_BUF_SIZE) to host
            // pack and copy the data from txBufferPtr to txWorkBufPtr
            for (i=0; i<(CSL_USB_CDC_DATA_BUF_SIZE-txStartIdxSave); i++)
            {
                if (i%2)
                    cdcAppHandle->txWorkBufPtr[i/2] = (cdcAppHandle->txBufferPtr[txStartIdxSave+i]<<8);
                else
                    cdcAppHandle->txWorkBufPtr[i/2] = cdcAppHandle->txBufferPtr[txStartIdxSave+i]&0xFF;
            }
            // send whatever in the TX working buffer to host
            status = USB_postTransaction(pContext->hEpObjArray[3], CSL_USB_CDC_DATA_BUF_SIZE-txStartIdxSave,
                                         cdcAppHandle->txWorkBufPtr, CSL_USB_IN_TRANSFER);
            if(status != CSL_SOK)
            {
                printf("USB TX Transaction failed\n");
            }

            /* Check the transaction status */
            txRxStatus = USB_isTransactionDone(pContext->hEpObjArray[3], &status);
            if(txRxStatus != CSL_USB_TRUE)
            {
                bytesRem = USB_bytesRemaining(pContext->hEpObjArray[3], &status);
                if(bytesRem)
                {
                    printf("Bytes remaining: %d\n", bytesRem);
                }
            }

            // update the txStartIdx
            cdcAppHandle->txStartIdx = 0;

            // send the TX buffer (0 - txEndIdx) to host
            // pack and copy the data from txBufferPtr to txWorkBufPtr
            for (i=0; i<txEndIdxSave; i++)
            {
                if (i%2)
                    cdcAppHandle->txWorkBufPtr[i/2] = (cdcAppHandle->txBufferPtr[txStartIdxSave+i]<<8);
                else
                    cdcAppHandle->txWorkBufPtr[i/2] = cdcAppHandle->txBufferPtr[txStartIdxSave+i]&0xFF;
            }
            // send whatever in the TX working buffer to host
            status = USB_postTransaction(pContext->hEpObjArray[3], txEndIdxSave,
                                         cdcAppHandle->txWorkBufPtr, CSL_USB_IN_TRANSFER);
            if(status != CSL_SOK)
            {
                printf("USB TX Transaction failed\n");
            }

            /* Check the transaction status */
            txRxStatus = USB_isTransactionDone(pContext->hEpObjArray[3], &status);
            if(txRxStatus != CSL_USB_TRUE)
            {
                bytesRem = USB_bytesRemaining(pContext->hEpObjArray[3], &status);
                if(bytesRem)
                {
                    printf("Bytes remaining: %d\n", bytesRem);
                }
            }

            // update the txStartIdx
            cdcAppHandle->txStartIdx = txEndIdxSave;
        }
    }

    return status;
}

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
Uint16 CDC_getData(pUsbContext pContext, CSL_CdcInitStructApp *cdcAppHandle, Uint16 *data)
{
    Uint16 rxEndIdxSave, rxStartIdxSave, i, tempSize;

    tempSize = 0;
    // is there data in the RX buffer?
    rxStartIdxSave = cdcAppHandle->rxStartIdx;
    rxEndIdxSave = cdcAppHandle->rxEndIdx;
    if (rxEndIdxSave>rxStartIdxSave)
    {
        // no wrap around case
        tempSize = rxEndIdxSave-rxStartIdxSave;
    } else if (rxEndIdxSave<rxStartIdxSave)
    {
        // wrap around case
        tempSize = rxEndIdxSave+CSL_USB_CDC_DATA_BUF_SIZE-rxStartIdxSave;
    } else
    {
        // no data in RX buffer
        tempSize = 0;
    }

    // copy data from RX buffer to data buffer
    for (i=0; i<tempSize; i++)
    {
        data[i] = cdcAppHandle->rxBufferPtr[cdcAppHandle->rxStartIdx]&0xFF;
        // increase the rxStartIdx
        cdcAppHandle->rxStartIdx++;
        if (cdcAppHandle->rxStartIdx>=CSL_USB_CDC_DATA_BUF_SIZE)
            cdcAppHandle->rxStartIdx = 0;
    }

    return tempSize;
}

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
Uint16 CDC_putData(pUsbContext pContext, CSL_CdcInitStructApp *cdcAppHandle, Uint16 *data, Uint16 size)
{
    CSL_Status status, txRxStatus;
    Uint16 bytesRem, i;
    Uint16 txEndIdxSave, txStartIdxSave;

    // the data size cannot larger than CSL_USB_CDC_DATA_BUF_SIZE
    if (size>CSL_USB_CDC_DATA_BUF_SIZE)
        return 0;
    // is there anything in the TX buffer?
    txStartIdxSave = cdcAppHandle->txStartIdx;
    txEndIdxSave = cdcAppHandle->txEndIdx;
    if (txEndIdxSave==txStartIdxSave)
    {
        // make sure TX working buffer is free too
        if (cdcAppHandle->txWorkBufIdx>0)
        {
            return 0;
        }

        // copy data from data buffer to TX working buffer
        for (i=0; i<size; i++)
        {
            if (cdcAppHandle->txWorkBufIdx%2)
            {
                cdcAppHandle->txWorkBufPtr[cdcAppHandle->txWorkBufIdx/2] = (cdcAppHandle->txWorkBufPtr[cdcAppHandle->txWorkBufIdx/2]&0x00FF)|(data[i]<<8);
            }
            else
            {
                cdcAppHandle->txWorkBufPtr[cdcAppHandle->txWorkBufIdx/2] = (data[i]&0xFF);
            }
            // increase the txWorkBufIdx
            cdcAppHandle->txWorkBufIdx++;
            if (cdcAppHandle->txWorkBufIdx>=CSL_USB_CDC_DATA_BUF_SIZE)
                cdcAppHandle->txWorkBufIdx = 0;
        }

        // send it to host
        status = USB_postTransaction(pContext->hEpObjArray[3], size,
                                     cdcAppHandle->txWorkBufPtr, CSL_USB_IN_TRANSFER);
        if(status != CSL_SOK)
        {
            printf("USB TX Transaction failed\n");
        }

        /* Check the transaction status */
        txRxStatus = USB_isTransactionDone(pContext->hEpObjArray[3], &status);
        if(txRxStatus != CSL_USB_TRUE)
        {
            bytesRem = USB_bytesRemaining(pContext->hEpObjArray[3], &status);
            if(bytesRem)
            {
                printf("Bytes remaining: %d\n", bytesRem);
            }
        }

        // update the txStartIdx
        cdcAppHandle->txStartIdx = cdcAppHandle->txEndIdx;
        // clear the txWorkBufIdx
        cdcAppHandle->txWorkBufIdx = 0;
    } else
    {
        // TX buffer is not empty
        // make sure the TX has enough space for the data
        if (txEndIdxSave>txStartIdxSave)
        {
            if ((txEndIdxSave-txStartIdxSave+size)>CSL_USB_CDC_DATA_BUF_SIZE)
                return 0;
        }
        else
        {
            if ((txStartIdxSave-txEndIdxSave+size)>CSL_USB_CDC_DATA_BUF_SIZE)
                return 0;
        }
        // copy data from data buffer to TX buffer
        for (i=0; i<size; i++)
        {
            cdcAppHandle->txBufferPtr[cdcAppHandle->txEndIdx] = (cdcAppHandle->txBufferPtr[cdcAppHandle->txEndIdx/2]&0xFF00)|(data[i]&0xFF);
            // increase the txEndIdx
            cdcAppHandle->txEndIdx++;
            if (cdcAppHandle->txEndIdx>=CSL_USB_CDC_DATA_BUF_SIZE)
                cdcAppHandle->txEndIdx = 0;
        }
    }

    return size;
}

