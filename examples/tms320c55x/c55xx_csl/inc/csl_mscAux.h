/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_mscAux.h
 *
 *  @brief USB MSC functional layer Auxiliary header file
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 25-Oct-2008 Created
 * 08-May-2009 Modified to fix the string desc ASCII to UNICODE conversion issue
 * 28-May-2009 Modified as per the review comments
 * ============================================================================
 */

#ifndef _CSL_MSCAUX_H_
#define _CSL_MSCAUX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csl_usbAux.h"
#include "csl_msc.h"

#include <stdio.h>

/** @addtogroup CSL_MSC_FUNCTION
 @{ */

/** ============================================================================
 *   @n@b MSC_SetSenseKeys
 *
 *   @b Description
 *   @n This function sets the sense key data
 *
 *   @b Arguments
 *   @verbatim
            senseData      - Sense key data array
            senseKey       - Sense key data
            addSenseKey    - Sense key additional data
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  None
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Sets sense key data
 *
 *   @b Modifies
 *   @n SEnse key data array
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
             MSC_SetSenseKeys(pMscHandle->senseData,
                              CSL_MSC_SCSI_SENSEKEY_ILLEGAL_REQUEST,
                              CSL_MSC_SCSI_ASC_INVALID_COMMAND_OPERATION_CODE);
     @endverbatim
 *  ============================================================================
 */
static inline
void MSC_SetSenseKeys(Uint16    *senseData,
                      Uint16    senseKey,
                      Uint16    addSenseKey)
{
   senseData[CSL_MSC_SSD_2] = senseKey;
   senseData[CSL_MSC_SSD_7] = addSenseKey;
}

/** ============================================================================
 *   @n@b MSC_HandleStateReset
 *
 *   @b Description
 *   @n Handles the reset condition
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbOutEp       - Out end point handle
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n status returned by the MUSB module
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Changes the state to wait for CBW
 *
 *   @b Modifies
 *   @n Storage state variable
 *
 *   @b Example
 *   @verbatim
             CSL_MscInitStructApp    mscAppHandle;
             CSL_MscObject           *pMscHandle;
             pUsbEpHandle            hUbOutEp;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             MSC_HandleStateReset(pMscHandle, hUbOutEp);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status MSC_HandleStateReset(CSL_MscObject    *pMscHandle,
                                pUsbEpHandle     hUsbOutEp)
{
    CSL_Status    status;

    status = USB_abortTransaction(hUsbOutEp);
    status = USB_postTransaction(hUsbOutEp, CSL_MSC_STATE_RESET_DATA_SIZE,
                                 &pMscHandle->cbw[0], CSL_USB_IOFLAG_NONE);
    pMscHandle->storageState = CSL_MSC_STORAGE_STATE_WAIT_FOR_CBW;

    return(status);
}

/** ============================================================================
 *   @n@b MSC_HandleStartStopUnit
 *
 *   @b Description
 *   @n Starts or stops the Unit
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbInEp        - In end point handle
            lunNum          - Logical unit number
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
             MSC_HandleStartStopUnit(pMscHandle, hUbInEp, lunNum);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status MSC_HandleStartStopUnit(CSL_MscObject    *pMscHandle,
                                   pUsbEpHandle     hUsbInEp,
                                   Uint16           lunNum)
{
    CSL_MscMediaStatus    mediaEjectStat;
    CSL_Status            status;
    Uint16                loadEject;

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

    loadEject = pMscHandle->cbw[CSL_MSC_CBW_9] >> CSL_MSC_8BIT_SHIFT;
    pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;

    pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_CSW;

    if(loadEject == 0x02) /* If the Eject Command */
    {
        if (pMscHandle->lun[lunNum].mediaState & CSL_MSC_MEDIA_LOCKED)
        {
            MSC_SetSenseKeys(pMscHandle->senseData,
                CSL_MSC_SCSI_SENSEKEY_UNIT_ATTENTION,
                CSL_MSC_SCSI_ASC_MEDIA_REMOVAL_PREVENTED);
            pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;
        }
        else
        {
            mediaEjectStat = pMscHandle->mediaEject(lunNum);
            if (mediaEjectStat == CSL_MSC_MEDIACCESS_SUCCESS)
            {
                /* Change the media state to not present or not bad */
                pMscHandle->lun[lunNum].mediaState &=~(CSL_MSC_MEDIA_PRESENT | CSL_MSC_MEDIA_BAD);
                MSC_SetSenseKeys(pMscHandle->senseData,
                    CSL_MSC_SCSI_SENSEKEY_NO_SENSE,
                    CSL_MSC_SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION);
            }
            else
            {
                pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;
                MSC_SetSenseKeys(pMscHandle->senseData,
                    CSL_MSC_SCSI_SENSEKEY_ILLEGAL_REQUEST,
                    CSL_MSC_SCSI_ASC_INVALID_COMMAND_OPERATION_CODE);
            }
        }
    }
    else
    {
        MSC_SetSenseKeys(pMscHandle->senseData,
            CSL_MSC_SCSI_SENSEKEY_NO_SENSE,
            CSL_MSC_SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION);
        /* Change the media state to not present or not bad .. PRASAD*/
        pMscHandle->lun[lunNum].mediaState &=~(CSL_MSC_MEDIA_PRESENT | CSL_MSC_MEDIA_BAD);
    }

    status = USB_postTransaction(hUsbInEp,13,&pMscHandle->csw[0],CSL_USB_IOFLAG_NONE);

    return(status);
}

/** ============================================================================
 *   @n@b MSC_HandleStateSendCSW
 *
 *   @b Description
 *   @n Handles send CSW state
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbOutEp       - Out end point handle
            hUsbInEp        - In end point handle
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  status returned by the MUSB module
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Changes the state to wait for CBW
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

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             MSC_HandleStateSendCSW(pMscHandle, hUbOutEp, hUbInEp);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status MSC_HandleStateSendCSW(CSL_MscObject    *pMscHandle,
                                  pUsbEpHandle     hUsbOutEp,
                                  pUsbEpHandle     hUsbInEp)
{
    CSL_Status    status;

    if(USB_isTransactionDone(hUsbInEp, &status))
    {
        pMscHandle->activityPresentFlag = FALSE;
        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_WAIT_FOR_CBW;
        status |= USB_postTransaction(hUsbOutEp, CSL_MSC_CBW_DATA_SIZE,
                                  &pMscHandle->cbw[0], CSL_USB_IOFLAG_NONE);
    }

    return(status);
}

/** ============================================================================
 *   @n@b MSC_HandleStateSendData
 *
 *   @b Description
 *   @n Sends the command status word to the host device
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbInEp        - In end point handle
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
             pUsbEpHandle            hUbInEp;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             MSC_HandleStateSendData(pMscHandle, hUbInEp);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status MSC_HandleStateSendData(CSL_MscObject    *pMscHandle,
                                   pUsbEpHandle     hUsbInEp)
{
    CSL_Status    status;

    if(USB_isTransactionDone(hUsbInEp, &status))
    {
        status |= USB_postTransaction(hUsbInEp, CSL_MSC_CSW_DATA_SIZE,
                                    &pMscHandle->csw[0], CSL_USB_IOFLAG_NONE);
        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_CSW;
    }

    return(status);
}

/** ============================================================================
 *   @n@b MSC_HandleStateSendStall
 *
 *   @b Description
 *   @n Sends the stall signal to the host
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbInEp        - In end point handle
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
             pUsbEpHandle            hUbInEp;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             MSC_HandleStateSendStall(pMscHandle, hUbInEp);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status MSC_HandleStateSendStall(CSL_MscObject    *pMscHandle,
                                    pUsbEpHandle     hUsbInEp)
{
    CSL_Status    status;

    if(USB_isTransactionDone(hUsbInEp, &status))
    {
        status |= USB_postTransaction(hUsbInEp, CSL_MSC_CSW_DATA_SIZE,
                                     &pMscHandle->csw[0], CSL_USB_IOFLAG_NONE);
        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_CSW;
    }

    return(status);
}

/** ============================================================================
 *   @n@b MSC_HandleStateSendingShortPkt
 *
 *   @b Description
 *   @n Handles sending short packet storage state
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbInEp        - In end point handle
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  status returned by the MUSB module
 *
 *   <b> Pre Condition </b>
 *   @n  MSC module should be opened using MSC_Open/MSC_OpenFullspeed function
 *
 *   <b> Post Condition </b>
 *   @n  Changes the state to sending stall
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

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             MSC_HandleStateSendingShortPkt(pMscHandle, hUbInEp);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status MSC_HandleStateSendingShortPkt(CSL_MscObject    *pMscHandle,
                                          pUsbEpHandle     hUsbInEp)
{
    CSL_Status    status;

    pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_STALL;

    status = USB_stallEndpt(hUsbInEp);

    /* Dummy data transfer to intimate MSC_bulk function */
    status |= USB_postTransaction(hUsbInEp, 0,
                                  &pMscHandle->csw[0],
                                  CSL_USB_IOFLAG_NOT);

    return (status);
}

/** ============================================================================
 *   @n@b MSC_HandleModeSense6
 *
 *   @b Description
 *   @n This function handles the Mode Sense 6 Command
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
             MSC_HandleModeSense6(pMscHandle, hUbInEp, lunNum);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status MSC_HandleModeSense6(CSL_MscObject    *pMscHandle,
                                pUsbEpHandle     hUsbInEp,
                                Uint16           logicalUnit)
{
    CSL_Status    status;
    Uint16        modeSenseLen;
    Uint16        smallerLen;

    /* Check whether the data direction is proper or not */
    if(((pMscHandle->cbw[CSL_MSC_CBW_6] &
          CSL_MSC_CBW_DIRBIT_MASK) != CSL_MSC_CBW_DATADIR_IN) &&
        (pMscHandle->cbwDataTransferLength != 0))
    {
        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_PHASE_ERROR;

        /* Stall the end point */
        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_STALL;
        status = USB_stallEndpt(&pMscHandle->bulkInEpObj);
        status = USB_stallEndpt(&pMscHandle->bulkOutEpObj);

        /* Dummy data transfer to intimate MSC_bulk function */
        status = USB_postTransaction(&pMscHandle->bulkInEpObj,
                                     0,
                                     pMscHandle->lbaBuffer,
                                     CSL_USB_IOFLAG_NOT);
        return(status);
    }

    if (pMscHandle->cbwDataTransferLength > 0)
    {
        modeSenseLen = pMscHandle->modeSenseData[0];

        /* Getting smaller of the two values */
        if(modeSenseLen < pMscHandle->cbwDataTransferLength)
        {
            smallerLen = modeSenseLen;
        }
        else
        {
            smallerLen = (Uint16)(pMscHandle->cbwDataTransferLength &
                                  CSL_MSC_16BIT_MASK);
        }

        /* Setting the Data Residue :- Transfer Length - Data transferred */
        pMscHandle->cbwDataTransferLength -= (Uint32)smallerLen;
        pMscHandle->csw[CSL_MSC_CSW_4] =
              (Uint16)(pMscHandle->cbwDataTransferLength & CSL_MSC_16BIT_MASK);
        pMscHandle->csw[CSL_MSC_CSW_5] =
              (Uint16)(pMscHandle->cbwDataTransferLength >> CSL_MSC_16BIT_SHIFT);
        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;
        status = USB_postTransaction(hUsbInEp, smallerLen,
                                    &pMscHandle->modeSenseData[1],
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
 *   @n@b MSC_HandleModeSense10
 *
 *   @b Description
 *   @n This function handles the Mode Sense 10 Command
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
             MSC_HandleModeSense6(pMscHandle, hUbInEp, lunNum);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status MSC_HandleModeSense10(CSL_MscObject    *pMscHandle,
                                 pUsbEpHandle     hUsbInEp,
                                 Uint16           logicalUnit)
{
    CSL_Status    status;
    Uint16        modeSenseLen;
    Uint16        smallerLen;

    /* Check whether the data direction is proper or not */
    if(((pMscHandle->cbw[CSL_MSC_CBW_6] &
          CSL_MSC_CBW_DIRBIT_MASK) != CSL_MSC_CBW_DATADIR_IN) &&
        (pMscHandle->cbwDataTransferLength != 0))
    {
        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_PHASE_ERROR;

        /* Stall the end point */
        pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_STALL;
        status = USB_stallEndpt(&pMscHandle->bulkInEpObj);
        status = USB_stallEndpt(&pMscHandle->bulkOutEpObj);

        /* Dummy data transfer to intimate MSC_bulk function */
        status = USB_postTransaction(&pMscHandle->bulkInEpObj,
                                     0,
                                     pMscHandle->lbaBuffer,
                                     CSL_USB_IOFLAG_NOT);
        return(status);
    }

    if (pMscHandle->cbwDataTransferLength > 0)
    {
        modeSenseLen = pMscHandle->modeSenseData[0];

        /* Getting smaller of the two values */
        if(modeSenseLen < pMscHandle->cbwDataTransferLength)
        {
            smallerLen = modeSenseLen;
        }
        else
        {
            smallerLen = (Uint16)(pMscHandle->cbwDataTransferLength &
                                  CSL_MSC_16BIT_MASK);
        }

        /* Setting the Data Residue :- Transfer Length - Data transferred */
        pMscHandle->cbwDataTransferLength -= (Uint32)smallerLen;
        pMscHandle->csw[CSL_MSC_CSW_4] =
              (Uint16)(pMscHandle->cbwDataTransferLength & CSL_MSC_16BIT_MASK);
        pMscHandle->csw[CSL_MSC_CSW_5] =
              (Uint16)(pMscHandle->cbwDataTransferLength >> CSL_MSC_16BIT_SHIFT);
        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;
        status = USB_postTransaction(hUsbInEp, smallerLen,
                                    &pMscHandle->modeSenseData[1],
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
 *   @n@b MSC_HandleInquiry
 *
 *   @b Description
 *   @n This function handles Inquiry command
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
             MSC_HandleInquiry(pMscHandle, hUbInEp, lunNum);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status MSC_HandleInquiry(CSL_MscObject    *pMscHandle,
                             pUsbEpHandle     hUsbInEp,
                             Uint16           logicalUnit)
{
    CSL_Status    status;
    Uint16        cbwRespLen;
    Uint16        smallerLen;

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
        cbwRespLen = pMscHandle->lun[logicalUnit].scsiInquiryData[0];
        /* Getting smaller of the two values */
        if(cbwRespLen < pMscHandle->cbwDataTransferLength)
        {
            smallerLen = cbwRespLen;
            pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_SHORT_PKT;
        }
        else
        {
            smallerLen = (Uint16)(pMscHandle->cbwDataTransferLength &
                                  CSL_MSC_16BIT_MASK);
            pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_DATA;
        }

        /* Setting the Data Residue :- Transfer Length - Data transferred */
        pMscHandle->cbwDataTransferLength -= (Uint32)smallerLen;
        pMscHandle->csw[CSL_MSC_CSW_4] =
             (Uint16)(pMscHandle->cbwDataTransferLength & CSL_MSC_16BIT_MASK);
        pMscHandle->csw[CSL_MSC_CSW_5] =
             (Uint16)(pMscHandle->cbwDataTransferLength >> CSL_MSC_16BIT_SHIFT);
        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;

        status = USB_postTransaction(hUsbInEp, smallerLen,
                                 &pMscHandle->lun[logicalUnit].scsiInquiryData[1],
                                 CSL_USB_IOFLAG_NOSHORT);
    }
    else
    {
        status = MSC_sendCswWithPhaseError(pMscHandle, hUsbInEp);
    }

    return(status);
}

/** ============================================================================
 *   @n@b MSC_HandleRequestSense
 *
 *   @b Description
 *   @n This handles the Request Sense  SCSI Command
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbInEp        - In end point handle
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
             pUsbEpHandle            hUbInEp;
             Uint16                  mscAppBuffer[420];
             CSL_Status              status;

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             MSC_HandleRequestSense(pMscHandle, hUbInEp);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status MSC_HandleRequestSense(CSL_MscObject    *pMscHandle,
                                  pUsbEpHandle     hUsbInEp)
{
    CSL_Status    status;
    Uint16        cbwRespLen;
    Uint16        smallerLen;

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
        cbwRespLen = pMscHandle->senseData[0];

        /* Getting smaller of the two values */
        if(cbwRespLen < pMscHandle->cbwDataTransferLength)
        {
            pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_SHORT_PKT;
            smallerLen = cbwRespLen;
        }
        else
        {
            pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_DATA;

            smallerLen= (Uint16)(pMscHandle->cbwDataTransferLength &
                                 CSL_MSC_16BIT_MASK);
        }

        /* Setting the Data Residue :- Transfer Length - Data transferred */
        pMscHandle->cbwDataTransferLength -= (Uint32)smallerLen;
        pMscHandle->csw[CSL_MSC_CSW_4] =
               (Uint16)(pMscHandle->cbwDataTransferLength & CSL_MSC_16BIT_SHIFT);
        pMscHandle->csw[CSL_MSC_CSW_5] =
               (Uint16)(pMscHandle->cbwDataTransferLength >> CSL_MSC_16BIT_SHIFT);
        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;
        status = USB_postTransaction(hUsbInEp, smallerLen,
                                     &pMscHandle->senseData[1],
                                     CSL_USB_IOFLAG_NOSHORT);
    }
    else
    {
        status = MSC_sendCswWithPhaseError(pMscHandle, hUsbInEp);
    }

    return(status);
}

/** ============================================================================
 *   @n@b MSC_HandleVerify10
 *
 *   @b Description
 *   @n This handles the Verify 10 SCSI Command
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbInEp        - In end point handle
            logicalUnit     - Logical unit number
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  Status returned by the MUSB module
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
             MSC_HandleVerify10(pMscHandle, hUbInEp, lunNum);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status MSC_HandleVerify10(CSL_MscObject    *pMscHandle,
                              pUsbEpHandle     hUsbInEp,
                              Uint16           logicalUnit)
{
    CSL_Status    status;
    Uint16        verifyLen;

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

    verifyLen = ((pMscHandle->cbw[CSL_MSC_CBW_11] & CSL_MSC_8BIT_MASK) <<
                 CSL_MSC_8BIT_SHIFT)|((pMscHandle->cbw[CSL_MSC_CBW_11] &
                 CSL_MSC_8BIT_HIGH_MASK) >> CSL_MSC_8BIT_SHIFT);

    if((pMscHandle->lun[logicalUnit].verifyFlag == CSL_MSC_VERIFY_PASSED) ||
       (verifyLen == 0))
    {
        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_PASSED;
    }
    else
    {
        pMscHandle->csw[CSL_MSC_CSW_6] = CSL_MSC_CSW_STATUS_COMMAND_FAILED;
        MSC_SetSenseKeys(pMscHandle->senseData,
            CSL_MSC_SCSI_SENSEKEY_MISCOMPARE,
            CSL_MSC_SCSI_ASC_MISCOMPARE_VERIFY);
    }

    pMscHandle->storageState = CSL_MSC_STORAGE_STATE_SENDING_CSW;
    status = USB_postTransaction(hUsbInEp, CSL_MSC_CSW_DATA_SIZE,
                                 &pMscHandle->csw[0], CSL_USB_IOFLAG_NOSHORT);

    return(status);
}

/** ============================================================================
 *   @n@b  MSC_reqUnknown
 *
 *   @b Description
 *   @n This function handles the case where the request sent by the host is unknown
 *   unit number
 *
 *   @b Arguments
 *   @verbatim
            devNum   - USB device number
            usbSetup - USB setup structure pointer
            hInEp    - USB IN endpoint handle
            hOutEp   - USB OUT endpoint handle
            pMsc     - Handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_MSC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  MSC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends acknowledgement
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
static inline
CSL_MscRequestRet MSC_reqUnknown(CSL_UsbDevNum         devNum,
                                 CSL_UsbSetupStruct    *usbSetup,
                                 pUsbEpHandle          hInEp,
                                 pUsbEpHandle          hOutEp,
                                 void                  *pMsc)
{
    CSL_MscRequestRet    retStat;

    /* STALL the endpoint - the request is either not known or not supported */
    retStat = CSL_MSC_REQUEST_STALL;

    return(retStat);
}

/** ============================================================================
 *   @n@b MSC_lookupReqHandler
 *
 *   @b Description
 *   @n This functions maps the request sent by host to the Msc request table
 *   and returns the corresponding request handler address
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
 *   @n  Request table should be initialized and MSC_Open/MSC_OpenFullspeed
 *       function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns the request handler pointer
 *
 *   @b Modifies
 *   @n None
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

             // Assign values to all the members in mscAppHandle struct
             mscAppHandle.pMscObj = &mscAppBuffer[0];
             .....
             .....
             status = MSC_Open(&mscAppHandle);
             ....
             ....
             pCtrlHandle->fpRequestHandler = MSC_lookupReqHandler(request,
                                                       pCtrlHandle->mscReqTable);
     @endverbatim
 *  ============================================================================
 */
static inline
fpMSC_REQ_HANDLER MSC_lookupReqHandler(Uint16                  request,
                                       CSL_MscRequestStruct    *pUSB_ReqTable)
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
    return(MSC_reqUnknown);
}

/** ============================================================================
 *   @n@b  MSC_reqSetAddress
 *
 *   @b Description
 *   @n This function handles the request to set the device address
 *
 *   @b Arguments
 *   @verbatim
            devNum   - USB device number
            usbSetup - USB setup structure pointer
            hInEp    - USB IN endpoint handle
            hOutEp   - USB OUT endpoint handle
            pMsc     - Handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li                   CSL_MSC_REQUEST_DONE - Request handled successfully
 *
 *   <b> Pre Condition </b>
 *   @n  MSC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets the device address
 *
 *   @b Modifies
 *   @n USB FADDR_POWER register
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
static inline
CSL_MscRequestRet MSC_reqSetAddress(CSL_UsbDevNum         devNum,
                                    CSL_UsbSetupStruct    *usbSetup,
                                    pUsbEpHandle          hInEp,
                                    pUsbEpHandle          hOutEp,
                                    void                  *pMsc)
{
    USB_setDevAddr(devNum, (Uint16)(usbSetup->wValue));

    return(CSL_MSC_REQUEST_DONE);
}

/** ============================================================================
 *   @n@b  MSC_reqSetConfiguration
 *
 *   @b Description
 *   @n This function handles the request to set the device configuration
 *
 *   @b Arguments
 *   @verbatim
            devNum   - USB device number
            usbSetup - USB setup structure pointer
            hInEp    - USB IN endpoint handle
            hOutEp   - USB OUT endpoint handle
            pMsc     - Handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_MSC_REQUEST_SEND_ACK - Send acknowledgement for request
 *   @li        CSL_MSC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  MSC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets the configuration
 *
 *   @b Modifies
 *   @n USB registers
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
static inline
CSL_MscRequestRet MSC_reqSetConfiguration(CSL_UsbDevNum         devNum,
                                          CSL_UsbSetupStruct    *usbSetup,
                                          pUsbEpHandle          hInEp,
                                          pUsbEpHandle          hOutEp,
                                          void                  *pMsc)
{
    CSL_MscRequestRet    retStat;

    pMscClassHandle      pMscClassHdl;
    CSL_MscCtrlObject    *pCtrlHandle;

    pMscClassHdl = (pMscClassHandle)(pMsc);
    pCtrlHandle  = &pMscClassHdl->ctrlHandle;

    if((usbSetup->wValue == FALSE) || (usbSetup->wValue == TRUE))
    {
        pCtrlHandle->curConfigStat = usbSetup->wValue;

        USB_setConfiguration(devNum,usbSetup->wValue);

        retStat   = CSL_MSC_REQUEST_SEND_ACK;
    }
    else
    {
        /* configuration not supported, STALL the endpoint */
        retStat = CSL_MSC_REQUEST_STALL;
    }

    return(retStat);
}

/** ============================================================================
 *   @n@b  MSC_reqClearFeature
 *
 *   @b Description
 *   @n This function handles the request to clear the feature set
 *
 *   @b Arguments
 *   @verbatim
            devNum   - USB device number
            usbSetup - USB setup structure pointer
            hInEp    - USB IN endpoint handle
            hOutEp   - USB OUT endpoint handle
            pMsc     - Handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_MSC_REQUEST_SEND_ACK - Send acknowledgement for request
 *   @li        CSL_MSC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  MSC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Clears the feature
 *
 *   @b Modifies
 *   @n USB registers
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
static inline
CSL_MscRequestRet MSC_reqClearFeature(CSL_UsbDevNum         devNum,
                                      CSL_UsbSetupStruct    *usbSetup,
                                      pUsbEpHandle          hInEp,
                                      pUsbEpHandle          hOutEp,
                                      void                  *pMsc)
{
    CSL_MscRequestRet    retStat;
    pUsbEpHandle         hEPx;
    Uint16               endpt;  /* this is USB logical endpoint */

    retStat = CSL_MSC_REQUEST_SEND_ACK;

    switch(usbSetup->wValue)
    {
        case CSL_USB_FEATURE_ENDPOINT_STALL:
              endpt = (usbSetup->wIndex) & CSL_MSC_8BIT_MASK;
              hEPx = USB_epNumToHandle(devNum, endpt);
            USB_clearEndptStall(hEPx);
            break;

        case CSL_USB_FEATURE_REMOTE_WAKEUP:
            USB_setRemoteWakeup(devNum, (CSL_UsbBoolean)FALSE);
             break;

        default:
            /* Unsupported Feature. STALL the endpoint */
            retStat = CSL_MSC_REQUEST_STALL;
            break;
    }

    return(retStat);
}

/** ============================================================================
 *   @n@b  MSC_reqGetStatus
 *
 *   @b Description
 *   @n This function handles the get status request from host
 *
 *   @b Arguments
 *   @verbatim
            devNum   - USB device number
            usbSetup - USB setup structure pointer
            hInEp    - USB IN endpoint handle
            hOutEp   - USB OUT endpoint handle
            pMsc     - Handle to the MSC Class Obj
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
 *   @n  Sends the status
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
static inline
CSL_MscRequestRet MSC_reqGetStatus(CSL_UsbDevNum         devNum,
                                   CSL_UsbSetupStruct    *usbSetup,
                                   pUsbEpHandle          hInEp,
                                   pUsbEpHandle          hOutEp,
                                   void                  *pMsc)
{
    CSL_MscRequestRet     retStat;
    pMscClassHandle       pMscClassHdl;
    CSL_MscCtrlObject     *pCtrlHandle;
    pUsbEpHandle          hEPx;
    CSL_Status            status;
    Uint16                endpt;   /* this is USB logical endpoint */

    pMscClassHdl = (pMscClassHandle)(pMsc);
    pCtrlHandle  = &pMscClassHdl->ctrlHandle;
    retStat      = CSL_MSC_REQUEST_GET_ACK;

    switch(usbSetup->bmRequestType - CSL_MSC_REQUEST_TYPE_BASE)
    {
        /* Device Status to be returned */
        case CSL_MSC_REQUEST_TYPE_DEVICE_STATUS:
            pCtrlHandle->ctrlBuffer[1] =
            (((Uint16)USB_getRemoteWakeupStat(devNum))<<1) |
             CSL_MSC_CURRDEV_STAT;
            USB_postTransaction(hInEp, 2, &pCtrlHandle->ctrlBuffer,
                                CSL_USB_IOFLAG_NONE);
            break;

        /* Interface status is to be returned */
        case CSL_MSC_REQUEST_TYPE_INTERFACE_STATUS:
            pCtrlHandle->ctrlBuffer[1] = CSL_MSC_CURRDEV_STAT;
            USB_postTransaction(hInEp, 2, &pCtrlHandle->ctrlBuffer,
                                CSL_USB_IOFLAG_NONE);
            break;

        /* Endpoint status to be returned */
        case CSL_MSC_REQUEST_TYPE_EP_STATUS:

            endpt  =  usbSetup->wIndex & 0xFF;
            hEPx   =  USB_epNumToHandle(devNum, endpt);
            pCtrlHandle->ctrlBuffer[1] = (Uint16)USB_getEndptStall(hEPx,
                                                                   &status);
            USB_postTransaction(hInEp, 2, &pCtrlHandle->ctrlBuffer,
                                CSL_USB_IOFLAG_NONE);
            break;

        default:
            /* STALL the endpoint */
            retStat = CSL_MSC_REQUEST_STALL;
            break;
    }

    return(retStat);
}

/** ============================================================================
 *   @n@b  MSC_reqSetFeature
 *
 *   @b Description
 *   @n This function handles the request from host to set feature
 *
 *   @b Arguments
 *   @verbatim
            devNum   - USB device number
            usbSetup - USB setup structure pointer
            hInEp    - USB IN endpoint handle
            hOutEp   - USB OUT endpoint handle
            pMsc     - Handle to the MSC Class Obj
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
 *   @n  Sets the feature
 *
 *   @b Modifies
 *   @n USB regiters
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
static inline
CSL_MscRequestRet MSC_reqSetFeature(CSL_UsbDevNum         devNum,
                                    CSL_UsbSetupStruct    *usbSetup,
                                    pUsbEpHandle          hInEp,
                                    pUsbEpHandle          hOutEp,
                                    void                  *pMsc)
{
    CSL_MscRequestRet    retStat;
    pUsbEpHandle         hEPx;
    Uint16               endpt;        /* this is USB logical endpoint */

    retStat = CSL_MSC_REQUEST_SEND_ACK;

    switch(usbSetup->wValue)
    {
        case CSL_USB_FEATURE_ENDPOINT_STALL:
            /* updated set and clear endpoint stall to work with logical endpoint num */
            endpt = (usbSetup->wIndex) & CSL_MSC_8BIT_MASK;
            hEPx = USB_epNumToHandle(devNum, endpt);
            USB_stallEndpt(hEPx);
            break;

        case CSL_USB_FEATURE_REMOTE_WAKEUP:
            USB_setRemoteWakeup(devNum, (CSL_UsbBoolean)TRUE);
            break;

        default:
            /* Feature not supported, STALL the endpoint */
            retStat = CSL_MSC_REQUEST_STALL;
            break;
    }

  return(retStat);
}

/** ============================================================================
 *   @n@b  MSC_reqGetConfiguration
 *
 *   @b Description
 *   @n This function handles the request from host to get configuration
 *
 *   @b Arguments
 *   @verbatim
            devNum   - USB device number
            usbSetup - USB setup structure pointer
            hInEp    - USB IN endpoint handle
            hOutEp   - USB OUT endpoint handle
            pMsc     - Handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_MSC_REQUEST_GET_ACK  - Get acknowledgement for request
 *
 *   <b> Pre Condition </b>
 *   @n  MSC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends the configuration
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
static inline
CSL_MscRequestRet MSC_reqGetConfiguration(CSL_UsbDevNum         devNum,
                                          CSL_UsbSetupStruct    *usbSetup,
                                          pUsbEpHandle          hInEp,
                                          pUsbEpHandle          hOutEp,
                                          void                  *pMsc)
{
    pMscClassHandle       pMscClassHdl;
    CSL_MscCtrlObject*    pCtrlHandle;

    pMscClassHdl = (pMscClassHandle)(pMsc);
    pCtrlHandle  = &pMscClassHdl->ctrlHandle;

    /* Send the current Configuration Value */
    pCtrlHandle->ctrlBuffer[1] = pCtrlHandle->curConfigStat;
    USB_postTransaction(hInEp, 1, (void*)&pCtrlHandle->ctrlBuffer,
                        CSL_USB_IOFLAG_NONE | CSL_USB_IOFLAG_NOSHORT);

    return(CSL_MSC_REQUEST_GET_ACK);
}

/** ============================================================================
 *   @n@b  MSC_reqGetMaxLUN
 *
 *   @b Description
 *   @n This function handles the request from host to send maximum logical
 *   unit number
 *
 *   @b Arguments
 *   @verbatim
            devNum   - USB device number
            usbSetup - USB setup structure pointer
            hInEp    - USB IN endpoint handle
            hOutEp   - USB OUT endpoint handle
            pMsc     - Handle to the MSC Class Obj
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
 *   @n  Sends the max lun number
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
static inline
CSL_MscRequestRet MSC_reqGetMaxLUN(CSL_UsbDevNum         devNum,
                                   CSL_UsbSetupStruct    *usbSetup,
                                   pUsbEpHandle          hInEp,
                                   pUsbEpHandle          hOutEp,
                                   void                  *pMsc)
{
    pMscClassHandle       pMscClassHdl;
    CSL_MscCtrlObject*    pCtrlHandle;
    CSL_MscRequestRet     retStat;

    pMscClassHdl = (pMscClassHandle)(pMsc);
    pCtrlHandle  = &pMscClassHdl->ctrlHandle;
    pCtrlHandle->ctrlBuffer[1] = pMscClassHdl->mscHandle.noOfLun;

    /*
     * Verify the setup packet fields
     * wValue - 0
     * wIndex - interface number, set 0
     * wLength - 1
     */
    if ((usbSetup->wIndex == 0) &&
        (usbSetup->wLength == 1) &&
        (usbSetup->wValue == 0))
    {
        USB_postTransaction(hInEp, 1, &pCtrlHandle->ctrlBuffer[0],
                            CSL_USB_IOFLAG_NONE);

        /* Receive a 0 length packet for acknowledgement */
        retStat = CSL_MSC_REQUEST_GET_ACK;
    }
    else
    {
        retStat = CSL_MSC_REQUEST_STALL;
    }

    return (retStat);
}

/** ============================================================================
 *   @n@b  MSC_reqGetInterface
 *
 *   @b Description
 *   @n This function handles the request from host to get interface
 *   unit number
 *
 *   @b Arguments
 *   @verbatim
            devNum   - USB device number
            usbSetup - USB setup structure pointer
            hInEp    - USB IN endpoint handle
            hOutEp   - USB OUT endpoint handle
            pMsc     - Handle to the MSC Class Obj
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
 *   @n  Sends interface info
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
static inline
CSL_MscRequestRet MSC_reqGetInterface(CSL_UsbDevNum         devNum,
                                      CSL_UsbSetupStruct    *usbSetup,
                                      pUsbEpHandle          hInEp,
                                      pUsbEpHandle          hOutEp,
                                      void                  *pMsc)
{
    pMscClassHandle       pMscClassHdl;
    CSL_MscCtrlObject     *pCtrlHandle;
    CSL_MscRequestRet     retStat;

    pMscClassHdl = (pMscClassHandle)(pMsc);
    pCtrlHandle  = &pMscClassHdl->ctrlHandle;
    retStat      = CSL_MSC_REQUEST_GET_ACK;

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
        /*  Interface specified doesn't exist, STALL the endpoint */
        retStat = CSL_MSC_REQUEST_STALL;
    }

    return retStat;
}

/** ============================================================================
 *   @n@b  MSC_reqSetInterface
 *
 *   @b Description
 *   @n This function handles the request from host to set interface
 *   unit number
 *
 *   @b Arguments
 *   @verbatim
            devNum   - USB device number
            usbSetup - USB setup structure pointer
            hInEp    - USB IN endpoint handle
            hOutEp   - USB OUT endpoint handle
            pMsc     - Handle to the MSC Class Obj
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @li        CSL_MSC_REQUEST_SEND_ACK - Send acknowledgement for request
 *   @li        CSL_MSC_REQUEST_STALL    - Request stall signal (Error)
 *
 *   <b> Pre Condition </b>
 *   @n  MSC_lookupReqHandler should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sends acknowledgement
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
static inline
CSL_MscRequestRet MSC_reqSetInterface(CSL_UsbDevNum         devNum,
                                      CSL_UsbSetupStruct    *usbSetup,
                                      pUsbEpHandle          hInEp,
                                      pUsbEpHandle          hOutEp,
                                      void                  *pMsc)
{
    CSL_MscRequestRet    retStat;

    if(usbSetup->wIndex == 0)
    {
        if (usbSetup->wValue == 0)
        {
            USB_setConfiguration(devNum,usbSetup->wValue);
            retStat = CSL_MSC_REQUEST_SEND_ACK;
        }
    }
    else
    {
        /* configuration not supported, STALL the endpoint */
        retStat = CSL_MSC_REQUEST_STALL;
    }

    return(retStat);
}

/** ============================================================================
 *   @n@b MSC_verifyCBW
 *
 *   @b Description
 *   @n Verifies the CBW received from host
 *
 *   @b Arguments
 *   @verbatim
            pMscHandle      - Handle to Msc class object
            hUsbOutEp       - Out end point handle
            hUsbInEp        - In end point handle
     @endverbatim
 *
 *   <b> Return Value </b>   CSL_Status
 *   @n         CSL_SOK       - CBW is valid
 *   @n         CSL_ESYS_FAIL - CBW is invalid
 *
 *   <b> Pre Condition </b>
 *   @n  MSC_Open should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
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
             MSC_verifyCBW(pMscHandle, hUbOutEp, hUbInEp);
     @endverbatim
 *  ============================================================================
 */
static inline
CSL_Status  MSC_verifyCBW(CSL_MscObject    *pMscHandle,
                          pUsbEpHandle     hUsbOutEp,
                          pUsbEpHandle     hUsbInEp)
{
    Uint32        cbwSignature;
    CSL_Status    status;
    Uint16        cbwSize;
    Uint16        logicalUnit;

    cbwSignature = 0;
    status       = CSL_SOK;
    cbwSize      = 0;
    logicalUnit  = 0;

    if ((pMscHandle != NULL) && (hUsbOutEp != NULL) && (hUsbInEp != NULL))
    {
        cbwSignature =     ((Uint32)(pMscHandle->cbw[CSL_MSC_CBW_1]) <<
                                  CSL_MSC_16BIT_SHIFT) |
                                  (Uint32)pMscHandle->cbw[CSL_MSC_CBW_0];

        if (cbwSignature != CSL_MSC_CBW_SIGNATURE)
        {
            status = CSL_ESYS_FAIL;
        }
        else
        {
            cbwSize = USB_getDataCountReadFromFifo(hUsbOutEp);
            if (cbwSize != CSL_MSC_CBW_DATA_SIZE)
            {
                status = CSL_ESYS_FAIL;
            }
            else
            {
                /* Getting the logical Unit ready */
                logicalUnit = pMscHandle->cbw[CSL_MSC_CBW_6] >> CSL_MSC_8BIT_SHIFT;

                if (logicalUnit > pMscHandle->noOfLun)
                {
                    status = CSL_ESYS_FAIL;
                }
            }
        }
    }
    else
    {
        status = CSL_ESYS_FAIL;
    }

    return (status);
}

/**
@} */

#ifdef __cplusplus
}
#endif

#endif    // _CSL_MSCAUX_H_

