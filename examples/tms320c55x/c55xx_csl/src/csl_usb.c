/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_usb.c
 *
 *  @brief USB MUSB functional layer API source file
 *
 *  Path: \(CSLPATH)\src
 */

/* ============================================================================
 * Revision History
 * ================
 * 05-Sep-2008 Created
 * 25-Dec-2008 Added Isochronous Transfer Support
 * 21-Jan-2009 Modified for code review comments
 * 17-Jul-2009 Modified USB_init API to support DMA mode
 * 28-Jul-2009 Removed the commented code as per the review comments
 * ============================================================================
 */

#include "csl_usb.h"
#include "csl_usbAux.h"

CSL_UsbRegsOvly    usbRegisters;
CSL_SysRegsOvly    sysCtrlRegs;
CSL_UsbContext     gUsbContext;
CSL_UsbTransfer    gCtrlTransfer;
CSL_UsbTransfer    gBulkInTransfer;
CSL_UsbTransfer    gBulkOutTransfer;
CSL_UsbTransfer    gIsoInTransfer;
CSL_UsbTransfer    gIsoOutTransfer;
CSL_UsbTransfer    gIntInTransfer;
CSL_UsbEpStatus    gEpStatus[CSL_USB_ENDPOINT_COUNT];
pUsbEpHandle       gUsbEpHandle[CSL_USB_ENDPOINT_COUNT];

/** ============================================================================
 *   @n@b USB_init
 *
 *   @b Description
 *   @n This function initializes USB module
 *
 *   @b Arguments
 *   @verbatim
            usbConfig        USB Config structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes the USB module
 *
 *   @b Modifies
 *   @n USB context structure
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_init(CSL_UsbConfig    *usbConfig)
{
    pUsbContext     pContext;
    CSL_Status      status;
    pUsbEpStatus    peps;
    DWORD           dwEndpoint;
    volatile Uint32 looper;

    // enable the USB LDO if it is C5504/05/14/15
#if (!(defined(CHIP_VC5505) || defined(CHIP_VC5504)))
            *(volatile ioport Uint16 *)(0x7004) = 0x0001;
#endif

    pContext = &gUsbContext;
    status   = CSL_ESYS_INVPARAMS;

    if((usbConfig != NULL) || (usbConfig->devNum == CSL_USB0))
    {
        if((usbConfig->opMode == CSL_USB_OPMODE_POLLED) ||
           (usbConfig->opMode == CSL_USB_OPMODE_DMA) ||
           (usbConfig->opMode == CSL_USB_OPMODE_DMA_TXONLY))
        {
            sysCtrlRegs = CSL_SYSCTRL_REGS;

            /* Reset USB module */
            CSL_FINS(sysCtrlRegs->PSRCR, SYS_PSRCR_COUNT,
                     CSL_USB_PER_RESTCOUNT);

            CSL_FINST(sysCtrlRegs->PRCR, SYS_PRCR_USB_RST, RST);

            /* Give some delay for the device to reset */
            for(looper = 0; looper < CSL_USB_DELAY; looper++){;}

            /* Enable USB module in Idle PCGCR */
            CSL_FINS(sysCtrlRegs->PCGCR2, SYS_PCGCR2_USBCG,
                     CSL_SYS_PCGCR2_USBCG_ACTIVE);

            usbRegisters = (CSL_UsbRegsOvly )CSL_USB_REGS;
            /* USB powered */
            sysCtrlRegs->USBSCR = CSL_USB_SYS_CTRL_REG_DEF_VAL;

            /* no clock will be stopped */
            sysCtrlRegs->CLKSTOP = CSL_USB_CLOCKSTOP_REG_DEF_VAL;

            /* Reset the USB controller.
             * Note. doing this will clear suspendm enable
             */
            usbRegisters->CTRLR = CSL_USB_CTRLR_RESET_MASK;

            for(looper = 0; looper < 90000; looper++){;}

            /* Set USB access to 16 bits */
            CSL_FINST(sysCtrlRegs->USBSCR, SYS_USBSCR_BYTEMODE, FULL);

            CSL_FINS(usbRegisters->FADDR_POWER, USB_FADDR_POWER_HSEN, TRUE);

            CSL_FINS(usbRegisters->DEVCTL, USB_DEVCTL_SESSION ,TRUE);

            gCtrlTransfer.fComplete    = TRUE;
            gIsoInTransfer.fComplete   = TRUE;
            gIsoOutTransfer.fComplete  = TRUE;
            gBulkInTransfer.fComplete  = TRUE;
            gBulkOutTransfer.fComplete = TRUE;
            gIntInTransfer.fComplete   = TRUE;

            /* Store the application call back function pointers */
            pContext->opMode                   = usbConfig->opMode;
            pContext->maxCurrent               = usbConfig->maxCurrent;
            pContext->remoteWkpStat            = CSL_USB_FALSE;
            pContext->suspendCallBackFxn       = usbConfig->appSuspendCallBack;
            pContext->wakeupCallBackFxn        = usbConfig->appWakeupCallBack;
            pContext->startTransferCallback     = usbConfig->startTransferCallback;
            pContext->completeTransferCallback = usbConfig->completeTransferCallback;

            pContext->ep0State                = CSL_USB_EP0_IDLE;
            pContext->pEpStatus               = gEpStatus;
            pContext->cableState              = CSL_USB_DEVICE_DETACH;
            pContext->fSetupPktCmd            = FALSE;
            pContext->fOutPhaseCmd            = FALSE;
            pContext->fWaitingOnFlagA         = FALSE;
            pContext->fWaitingOnFlagB         = FALSE;
            pContext->fWaitingOnEP0BUFAvail   = FALSE;
            pContext->fWaitingOnEP1InBUFAvail = FALSE;

            pContext->pFIFOBaseAddress = (DWORD*)(&(usbRegisters->FIFO0R1));

            USB_checkSpeed(pContext, &pContext->busSpeed);
            //pContext->busSpeed             = CSL_USB_BS_UNKNOWN_SPEED;
            pContext->cbOutEP0Buf          = 0;
            pContext->fMUSBIsReady         = FALSE;
            pContext->fSendHandshake       = TRUE;
            pContext->fEP0BUFAvailable     = TRUE;
            pContext->fEP1InBUFAvailable   = TRUE;
            pContext->cbOutEP0DataReceived = 0;

            // set the FIFO start address to be 8 (8*8 = 64 byte, reserved for EP0)
            pContext->dwFIFOStartAddr = CSL_USB_IN_EP_FIFO_START;

            /* memory address */
            peps = &pContext->pEpStatus[CSL_USB_EP1];

            peps->pFifoAddr = (DWORD*)(&(usbRegisters->FIFO1R1));

            peps = &pContext->pEpStatus[CSL_USB_EP2];
            peps->pFifoAddr = (DWORD*)(&(usbRegisters->FIFO2R1));

            peps = &pContext->pEpStatus[CSL_USB_EP3];

            peps->pFifoAddr = (DWORD*)(&(usbRegisters->FIFO3R1));

            peps = &pContext->pEpStatus[CSL_USB_EP4];
            peps->pFifoAddr = (DWORD*)(&(usbRegisters->FIFO4R1));

            /* Initialize peps items */
            for(dwEndpoint = 0; dwEndpoint < CSL_USB_ENDPOINT_COUNT; dwEndpoint++)
            {
                peps = &pContext->pEpStatus[dwEndpoint];

                peps->fStalled      = FALSE;
                peps->pTransfer     = NULL;
                peps->fSelected     = FALSE;
                peps->dwEndpoint    = dwEndpoint;
                peps->wUSBEvents    = CSL_USB_EVENT_NONE;
                peps->fInitialized  = TRUE;
                peps->hEventHandler = NULL;

                switch (dwEndpoint)
                {
                    case CSL_USB_EP0:
                        peps->dwPacketSizeAssigned = CSL_USB_EP0_PACKET_SIZE;
                        break;

                    case CSL_USB_EP1:
                        if (pContext->busSpeed == CSL_USB_BS_HIGH_SPEED)
                        {
                            peps->dwPacketSizeAssigned =  CSL_USB_EP1_PACKET_SIZE_HS;
                        }
                        else
                        {
                            peps->dwPacketSizeAssigned =  CSL_USB_EP1_PACKET_SIZE_FS;
                        }

                        break;

                    case CSL_USB_EP2:
                        if (pContext->busSpeed == CSL_USB_BS_HIGH_SPEED)
                        {
                            peps->dwPacketSizeAssigned =  CSL_USB_EP2_PACKET_SIZE_HS;
                        }
                        else
                        {
                            peps->dwPacketSizeAssigned =  CSL_USB_EP2_PACKET_SIZE_FS;
                        }

                        break;

                    case CSL_USB_EP3:
                        if (pContext->busSpeed == CSL_USB_BS_HIGH_SPEED)
                        {
                            peps->dwPacketSizeAssigned =  CSL_USB_EP3_PACKET_SIZE_HS;
                        }
                        else
                        {
                            peps->dwPacketSizeAssigned =  CSL_USB_EP3_PACKET_SIZE_FS;
                        }

                        break;

                    case CSL_USB_EP4:
                        if (pContext->busSpeed == CSL_USB_BS_HIGH_SPEED)
                        {
                            peps->dwPacketSizeAssigned =  CSL_USB_EP4_PACKET_SIZE_HS;
                        }
                        else
                        {
                            peps->dwPacketSizeAssigned =  CSL_USB_EP4_PACKET_SIZE_FS;
                        }

                        break;

                    default:
                        break;
                }
            }

            pContext->fInitialized = TRUE;
            status = CSL_SOK;
        }
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_resetDev
 *
 *   @b Description
 *   @n This function resets the USB device
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Resets the USB device
 *
 *   @b Modifies
 *   @n USB registers
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_resetDev(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_resetDev(CSL_UsbDevNum    devNum)
{
    CSL_Status      status;
    Uint16            saveIndex;
    pUsbContext     pContext;

    status    = CSL_SOK;
    saveIndex = 0;
    pContext  = &gUsbContext;

    if(devNum != CSL_USB0)
    {
        return(CSL_ESYS_INVPARAMS);
    }

    /* save the index register value */
    saveIndex = usbRegisters->INDEX_TESTMODE;

    pContext->fMUSBIsReady = TRUE;

    /* Flush the FIFOs to start */
    /* flush ep0 fifo */
    CSL_FINS(usbRegisters->INDEX_TESTMODE,
         USB_INDEX_TESTMODE_EPSEL,
         CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

    CSL_FINS(usbRegisters->INDEX_TESTMODE,
         USB_INDEX_TESTMODE_EPSEL,
         CSL_USB_EP0);

    CSL_FINS(usbRegisters->PERI_CSR0_INDX,
         USB_PERI_CSR0_INDX_FLUSHFIFO, TRUE);

    /* flush ep1 fifo */
    CSL_FINS(usbRegisters->INDEX_TESTMODE,
         USB_INDEX_TESTMODE_EPSEL,
         CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

    CSL_FINS(usbRegisters->INDEX_TESTMODE,
         USB_INDEX_TESTMODE_EPSEL,
         CSL_USB_EP1);

    CSL_FINS(usbRegisters->PERI_CSR0_INDX,
         USB_PERI_TXCSR_FLUSHFIFO, TRUE);

    CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
         USB_PERI_RXCSR_FLUSHFIFO, TRUE);

    /* flush ep2 fifo */
    CSL_FINS(usbRegisters->INDEX_TESTMODE,
         USB_INDEX_TESTMODE_EPSEL,
         CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

    CSL_FINS(usbRegisters->INDEX_TESTMODE,
         USB_INDEX_TESTMODE_EPSEL,
         CSL_USB_EP2);

    CSL_FINS(usbRegisters->PERI_CSR0_INDX,
         USB_PERI_TXCSR_FLUSHFIFO, TRUE);

    CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
         USB_PERI_RXCSR_FLUSHFIFO, TRUE);

    /* flush ep3 fifo */
    CSL_FINS(usbRegisters->INDEX_TESTMODE,
         USB_INDEX_TESTMODE_EPSEL,
         CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

    CSL_FINS(usbRegisters->INDEX_TESTMODE,
         USB_INDEX_TESTMODE_EPSEL,
         CSL_USB_EP3);

    CSL_FINS(usbRegisters->PERI_CSR0_INDX,
         USB_PERI_TXCSR_INDX_FLUSHFIFO, TRUE);

    CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
         USB_PERI_RXCSR_FLUSHFIFO, TRUE);

    /* flush ep4 fifo */
    CSL_FINS(usbRegisters->INDEX_TESTMODE,
         USB_INDEX_TESTMODE_EPSEL,
         CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

    CSL_FINS(usbRegisters->INDEX_TESTMODE,
         USB_INDEX_TESTMODE_EPSEL,
         CSL_USB_EP4);

    CSL_FINS(usbRegisters->PERI_CSR0_INDX,
         USB_PERI_TXCSR_INDX_FLUSHFIFO, TRUE);

    CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
         USB_PERI_RXCSR_FLUSHFIFO, TRUE);

    /* enable all tx, rx, and global interrupts in MUSB */
    /* enable ep0 intr */
    CSL_FINS(usbRegisters->INTRTXE,
             USB_INTRTXE_EP0, TRUE);
    /* enable ep1 intr */
    CSL_FINS(usbRegisters->INTRTXE,
             USB_INTRTXE_EP1TX, TRUE);
    /* enable ep2 intr*/
    CSL_FINS(usbRegisters->INTRRXE,
             USB_INTRRXE_EP2RX, TRUE);
    /* enable ep3 intr*/
    CSL_FINS(usbRegisters->INTRTXE,
             USB_INTRTXE_EP3TX, TRUE);
     /* enable generic MUSB interrupts
    Interrupts enabled : Disconnect, connect, Reset, Resume, Suspend */
    usbRegisters->INTRUSB_INTRUSBE |= CSL_USB_INTRUSB_INTRUSBE_DEFVAL;
    usbRegisters->INTMSKSETR1 = CSL_USB_INTMSKSETR1_DEFVAL;
    usbRegisters->INTMSKSETR2 = CSL_USB_INTMSKSETR2_DEFVAL;// | CSL_USB_GBL_INT_SOF;

    /* restore the index register */
    usbRegisters->INDEX_TESTMODE = saveIndex;

    return(status);
}

/** ============================================================================
 *   @n@b USB_initEndptObj
 *
 *   @b Description
 *   @n This function initializes an endpoint object.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            hEp           Handle to an endpoint object to be initialized.
            epNum         End point number
            xferType      Type of data transfer to be supported by the endpoint
            maxPktSize    Max data packet size supported by the endpoint.
            evMask        ORed combination of USB Interrupt Events to
                             be broadcasted to the associated event handler
            fxn           Associated event handler routine.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init and USB_epNumToHandle functions should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Initializes an end point object
 *
 *   @b Modifies
 *   @n End point object structure
 *
 *   @b Example
 *   @verbatim
            void      (*evtHandler)();

            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            pUsbEpHandle       hEp;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            .....
            hEp = USB_epNumToHandle(CSL_USB0, CSL_USB_IN_TRANSFER);
            .....
            .....
            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_initEndptObj(CSL_UsbDevNum        devNum,
                            pUsbEpHandle         hEp,
                            CSL_UsbEpNum         epNum,
                            CSL_UsbXferType      xferType,
                            Uint16               maxPktSize,
                            Uint16               evMask,
                            CSL_USB_EVENT_ISR    fxn)
{
    pUsbEpStatus    peps;
    CSL_Status      status;
    pUsbContext     pContext;
    Uint16          fifoSize;
    Uint16          saveIndex;
    DWORD           dwEndpoint;

    peps     = NULL;
    status   = CSL_SOK;
    pContext = &gUsbContext;

    if(hEp != NULL)
    {
        dwEndpoint = epNum % CSL_USB_IN_EP0;

        switch(devNum)
        {
            case CSL_USB0:
                /* init the Endpoint obj */
                hEp->epNum     = epNum;
                hEp->xferType  = xferType;
                hEp->maxPktSiz = maxPktSize;
                hEp->eventMask = evMask;
                hEp->fxn       = fxn;

                // determine the fifo size according to the
                // maximum packet size and the transfer type
                fifoSize = USB_findFifoSize(maxPktSize, xferType);
                // if the fifo size is 0, then return error
                if (fifoSize==0)
                    return CSL_ESYS_INVPARAMS;

                peps = &pContext->pEpStatus[hEp->epNum % CSL_USB_IN_EP0];

                if(peps != NULL)
                {
                    peps->hEventHandler    = fxn;
                    peps->fSelected           = TRUE;
                    peps->xferType         = xferType;
                    peps->dataReadFromFifo = 0;
                }

                if(dwEndpoint == CSL_USB_EP0)
                {
                    return(status);
                }

                /* save the index register value */
                saveIndex = usbRegisters->INDEX_TESTMODE;

                CSL_FINS(usbRegisters->INDEX_TESTMODE,
                         USB_INDEX_TESTMODE_EPSEL,
                          CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

                CSL_FINS(usbRegisters->INDEX_TESTMODE,
                         USB_INDEX_TESTMODE_EPSEL, dwEndpoint);

                switch(epNum)
                {
                    /* Configure Bulk IN End point */
                    case CSL_USB_IN_EP1:
                    case CSL_USB_IN_EP2:
                    case CSL_USB_IN_EP3:
                    case CSL_USB_IN_EP4:
                        CSL_FINS(usbRegisters->TXFIFOSZ_RXFIFOSZ,
                                 USB_TXFIFOSZ_RXFIFOSZ_TXSZ,
                                 CSL_USB_TXFIFOSZ_RXFIFOSZ_RESETVAL);

                        CSL_FINS(usbRegisters->TXFIFOSZ_RXFIFOSZ,
                                 USB_TXFIFOSZ_RXFIFOSZ_TXSZ, fifoSize);

                        CSL_FINS(usbRegisters->TXFIFOSZ_RXFIFOSZ,
                                 USB_TXFIFOSZ_RXFIFOSZ_TXDPB, FALSE);

                        // make sure we have enough FIFO space left (total 512*8 = 4096)
                        if ((pContext->dwFIFOStartAddr+(1<<fifoSize))<=CSL_USB_FIFO_SIZE)
                        {
                            // set the TX FIFO start address
                        CSL_FINS(usbRegisters->TXFIFOADDR,
                                USB_TXFIFOADDR_ADDR, pContext->dwFIFOStartAddr);
                            // reserve FIFO space for transmit
                        pContext->dwFIFOStartAddr += 1<<fifoSize;
                        } else
                        {
                            // we are out of the FIFO space for this EP
                            return CSL_ESYS_INVPARAMS;
                        }

                        /* Set maximum packet size */
                        CSL_FINS(usbRegisters->TXMAXP_INDX,
                                 USB_TXMAXP_INDX_MAXPAYLOAD, maxPktSize);

                        /* no autoset(D15=0); ISO for bulk(D14=0); no DMA(D12=0)
                           normal data toggle(D11=0) */
                        CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                                 USB_PERI_TXCSR_AUTOSET, FALSE);

                        CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                                 USB_PERI_TXCSR_ISO, FALSE);

                        CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                                 USB_PERI_TXCSR_DMAEN, FALSE);

                        CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                                 USB_PERI_TXCSR_FRCDATATOG, FALSE);

                        /* Enable Isochronous mode bit if request is for ISO IN */
                        if(xferType == CSL_USB_ISO)
                        {
                            if ((pContext->dwFIFOStartAddr+(1<<fifoSize))<=CSL_USB_FIFO_SIZE)
                            {
                                // always use double buffering for ISO, therefore double the reserved FIFO space
                                pContext->dwFIFOStartAddr += 1<<fifoSize;
                            } else
                            {
                                // we are out of the FIFO space for this EP
                                return CSL_ESYS_INVPARAMS;
                            }

                            // set the ISO update bit
                            CSL_FINS(usbRegisters->FADDR_POWER,
                                     USB_FADDR_POWER_ISOUPDATE, (Uint16)TRUE);

                            // set he ISO mode bit
                            CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                                     USB_PERI_TXCSR_INDX_ISO, TRUE);

                            /* Enable Double packet buffering */
                            CSL_FINS(usbRegisters->TXFIFOSZ_RXFIFOSZ,
                                     USB_TXFIFOSZ_RXFIFOSZ_TXDPB, TRUE);
                        }
                        break;

                    /* Configure Bulk OUT End point */
                    case CSL_USB_OUT_EP1:
                    case CSL_USB_OUT_EP2:
                    case CSL_USB_OUT_EP3:
                    case CSL_USB_OUT_EP4:
                        /* no double pkt buffering */
                        CSL_FINS(usbRegisters->TXFIFOSZ_RXFIFOSZ,
                                 USB_TXFIFOSZ_RXFIFOSZ_RCVSZ,
                                 CSL_USB_TXFIFOSZ_RXFIFOSZ_RCVSZ_RESETVAL);

                        CSL_FINS(usbRegisters->TXFIFOSZ_RXFIFOSZ,
                                 USB_TXFIFOSZ_RXFIFOSZ_RCVSZ, fifoSize);

                        // make sure we have enough FIFO space
                        if ((pContext->dwFIFOStartAddr+(1<<fifoSize))<=CSL_USB_FIFO_SIZE)
                        {
                            // set the RX FIFO start address
                        CSL_FINS(usbRegisters->RXFIFOADDR,
                               USB_RXFIFOADDR_ADDR, pContext->dwFIFOStartAddr);
                            // reserve FIFO space for receive
                        pContext->dwFIFOStartAddr += 1<<fifoSize;
                        } else
                        {
                            // we are out of the FIFO space for this EP
                            return CSL_ESYS_INVPARAMS;
                        }

                        /* Set maximum packet size */
                        CSL_FINS(usbRegisters->RXMAXP_INDX,
                                USB_RXMAXP_INDX_MAXPAYLOAD, maxPktSize);

                        /* no autoclear(D15=0), ISO for bulk(D14=0), no DMA(D13=0)
                        normal ping flow control(D12=0) */
                        CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
                                 USB_PERI_RXCSR_INDX_AUTOCLEAR, FALSE);

                        CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
                                 USB_PERI_RXCSR_INDX_ISO, FALSE);

                        CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
                                 USB_PERI_RXCSR_INDX_DMAEN, FALSE);

                        CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
                                 USB_PERI_RXCSR_INDX_DISNYET, FALSE);

                        /* Enable Isochronous mode bit if request is for ISO OUT */
                        if(xferType == CSL_USB_ISO)
                        {
                            // make sure we have enough FIFO space left
                            if ((pContext->dwFIFOStartAddr+(1<<fifoSize))<=CSL_USB_FIFO_SIZE)
                            {
                                // always use double buffering for ISO, therefore double the reserved FIFO space
                                pContext->dwFIFOStartAddr += 1<<fifoSize;
                            } else
                            {
                                // we are out of the FIFO space for this EP
                                return CSL_ESYS_INVPARAMS;
                            }

                            CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
                                     USB_PERI_RXCSR_INDX_ISO, TRUE);

                            /* Enable Double packet buffering */
                            CSL_FINS(usbRegisters->TXFIFOSZ_RXFIFOSZ,
                                     USB_TXFIFOSZ_RXFIFOSZ_RXDPB, TRUE);
                        }

                        break;

                    default:
                        /* Only Five end points(EP0 - EP4) are supported in this
                         * implementation
                         */
                        status = CSL_ESYS_INVPARAMS;
                        break;
                }

                /* restore the index register */
                usbRegisters->INDEX_TESTMODE = saveIndex;

                break;

            /* Currently USB1 and USB2 are not supported */
            case CSL_USB1:
            case CSL_USB2:
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
 *   @n@b USB_getEvents
 *
 *   @b Description
 *   @n This API reads and clears all the pending USB_EVENTS associated with a
 *      particular USB endpoint.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbEventMask
 *   @n                     ORed combination of all the pending USB_EVENTS
 *                            associated with a particular endpoint.
 *
 *   <b> Pre Condition </b>
 *   @n  End point object should be initialized
 *
 *   <b> Post Condition </b>
 *   @n  1. Clears and returns the end point events
 *   @n  2. Function call status is returned in status variable
 *   @li    CSL_SOK            - Returned for success
 *   @li    CSL_ESYS_BADHANDLE - Invalid end point handle
 *
 *   @b Modifies
 *   @n status variable
 *
 *   @b Example
 *   @verbatim
             void  (*evtHandler)();

             pUsbEpHandle     hEp;
            CSL_Status       status;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);
            USB_getEvents(hEp,
                          &status);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbEventMask USB_getEvents(pUsbEpHandle    hEp,
                               CSL_Status      *status)
{
    pUsbEpStatus          peps;
    CSL_UsbEventMask    usbEvents;
    pUsbContext         pContext;

    *status   = CSL_SOK;
    usbEvents = CSL_USB_EVENT_NONE;
    pContext  = &gUsbContext;
    peps      = NULL;

    if(hEp != NULL)
    {
        peps = &pContext->pEpStatus[hEp->epNum % CSL_USB_IN_EP0];

        if(peps != NULL)
        {
            usbEvents = (CSL_UsbEventMask)peps->wUSBEvents;
            /* clear up the events */
            peps->wUSBEvents = CSL_USB_EVENT_NONE;
        }
    }
    else
    {
        *status = CSL_ESYS_BADHANDLE;
    }

    return(usbEvents);
}

/** ============================================================================
 *   @n@b USB_peekEvents
 *
 *   @b Description
 *   @n This function read all the pending USB_EVENTS associated with
 *   a particular  USB endpoint
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>    CSL_UsbEventMask
 *   @n                       ORed combination of all the pending USB_EVENTS
 *                              associated with a particular endpoint.
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  End point object should be initialized
 *
 *   <b> Post Condition </b>
 *   @n  1. Returns the end point events
 *   @n  2. Function call status is returned in status variable
 *   @li    CSL_SOK            - Returned for success
 *   @li    CSL_ESYS_BADHANDLE - Invalid end point handle
 *
 *   @b Modifies
 *   @n status variable
 *
 *   @b Example
 *   @verbatim
             void  (*evtHandler)();

             CSL_UsbEpObj    epObj;
            CSL_Status      status;


            status = USB_initEndptObj(CSL_USB0,
                                      &epObj,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);
            USB_peekEvents(&epObj,
                           &status);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbEventMask USB_peekEvents(pUsbEpHandle    hEp,
                                CSL_Status      *status)
{
    pUsbEpStatus        peps;
    CSL_UsbEventMask     usbEvents;
    pUsbContext         pContext;

    *status   = CSL_SOK;
    usbEvents = CSL_USB_EVENT_NONE;
    pContext  = &gUsbContext;
    peps      = NULL;

    if(hEp != NULL)
    {
        peps = &pContext->pEpStatus[hEp->epNum % CSL_USB_IN_EP0];

        if(peps != NULL)
        {
            usbEvents = (CSL_UsbEventMask)peps->wUSBEvents;
        }
    }
    else
    {
        *status = CSL_ESYS_BADHANDLE;
    }

    return(usbEvents);
}

/** ============================================================================
 *   @n@b USB_getSetupPacket
 *
 *   @b Description
 *   @n This function read the setup packet from the setup data buffer.
 *      This function can be used to read the setup packet from the FIFO
 *      or we can read the setup packet from the USB context structure.
 *      We can choose one of two options using readFromFifo parameter.
 *      If readFromFifo is TRUE setup packet is read from the FIFO.
 *      If readFromFifo is FALSE setup packet is read from the context structure.
 *      Setup packet should be read from FIFO when this function is called from
 *      interrupt handler(In this case read packet is stored in context structure)
 *      and setup packet should be read from USB context structure when this
 *      function is called from MSC layer.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number
            usbSetup      Pointer to setup structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB Init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  USB setup structure is populated
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_UsbSetupStruct    usbSetup;
            CSL_Status            status;
            CSL_UsbConfig         usbConfig

            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_getSetupPacket(CSL_USB0, &usbSetup , TRUE);

     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_getSetupPacket(CSL_UsbDevNum         devNum,
                              CSL_UsbSetupStruct    *usbSetup,
                              Bool                  readFromFifo)
{
    CSL_Status    status;
    pUsbContext   pContext;
    Uint16           wBuffer[CSL_USB_SETUP_PACKET_LEN];

    pContext = &gUsbContext;
    status   = CSL_SOK;

    if(usbSetup != NULL)
    {
        switch(devNum)
        {
            case CSL_USB0:
                if(readFromFifo == TRUE)
                {
                    /* read setup packet from ep0 FIFO */
                    wBuffer[0] = CSL_FEXT(usbRegisters->FIFO0R1,
                                          USB_FIFO0R1_DATA);
                    wBuffer[1] = CSL_FEXT(usbRegisters->FIFO0R1,
                                          USB_FIFO0R1_DATA);
                    wBuffer[2] = CSL_FEXT(usbRegisters->FIFO0R1,
                                          USB_FIFO0R1_DATA);
                    wBuffer[3] = CSL_FEXT(usbRegisters->FIFO0R1,
                                          USB_FIFO0R1_DATA);

                    usbSetup->bmRequestType = wBuffer[0] & 0x00FF;
                    usbSetup->bRequest         = ((wBuffer[0] & 0xFF00) >> 8);
                    usbSetup->wValue         = wBuffer[1];
                    usbSetup->wIndex         = wBuffer[2];
                    usbSetup->wLength         = wBuffer[3];

                    pContext->cbOutEP0DataReceived = usbSetup->wLength;

                    break;
                }
                else
                {
                    *usbSetup = pContext->usbSetup;
                    usbSetup->newPacket     = 1;
                }

            /* Currently USB1 and USB2 are not supported */
            case CSL_USB1:
            case CSL_USB2:
            default :
                status = CSL_ESYS_INVPARAMS;
                break;
        }
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_postTransaction
 *
 *   @b Description
 *   @n This function transmits and receives USB data through an endpoint.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            byteCnt     Total number of bytes in the buffer pointed by *data
            Data        Pointer to a data buffer
            flags       ORed combination of USB Data Transfer Flags
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init end point should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Starts the data transfer
 *
 *   @b Modifies
 *   @n USB FIFO registers/data buffer
 *
 *   @b Example
 *   @verbatim
             void      (*evtHandler)();
             Uint16    dadaBuf[256];

             pUsbEpHandle    hEp;
            CSL_Status      status;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);

            status = USB_postTransaction(hEp, 256, dataBuf, CSL_USB_IOFLAG_NONE);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_postTransaction(pUsbEpHandle      hEp,
                               Uint16            byteCnt,
                               void              *data,
                               CSL_UsbIoFlags    flags)
{
    pUsbContext    pContext;
    pUsbEpStatus   peps;
    pUsbTransfer   pTransfer;
    CSL_UsbEpNum   epNum;
    CSL_Status     status;

    status   = CSL_SOK;
    pContext = &gUsbContext;

    if(hEp != NULL)
    {
        epNum = hEp->epNum;

        switch(epNum)
        {
            case CSL_USB_OUT_EP0: /* control out */
                pTransfer = &gCtrlTransfer;
                pTransfer->dwFlags = CSL_USB_OUT_TRANSFER;
                break;

            case CSL_USB_IN_EP0: /* control in */
                pTransfer = &gCtrlTransfer;
                pTransfer->dwFlags = CSL_USB_IN_TRANSFER;
                break;

            case CSL_USB_IN_EP1:
            case CSL_USB_IN_EP2:
            case CSL_USB_IN_EP3:
            case CSL_USB_IN_EP4:
                if(hEp->xferType == CSL_USB_BULK)
                {
                    /* USB BULK IN transfer */
                    pTransfer = &gBulkInTransfer;
                }
                else if (hEp->xferType == CSL_USB_ISO)
                {
                    /* USB Isochronous IN transfer */
                    pTransfer = &gIsoInTransfer;
                }
                else /* CSL_USB_INTR */
                {
                    /* USB Interrupt IN transfer */
                    pTransfer = &gIntInTransfer;
                }
                pTransfer->dwFlags = CSL_USB_IN_TRANSFER;

                break;

            case CSL_USB_OUT_EP1:
            case CSL_USB_OUT_EP2:
            case CSL_USB_OUT_EP3:
            case CSL_USB_OUT_EP4:
                if(hEp->xferType == CSL_USB_BULK)
                {
                    /* USB BULK OUT transfer */
                    pTransfer = &gBulkOutTransfer;
                }
                else
                {
                    /* USB Isochronous OUT transfer */
                    pTransfer = &gIsoOutTransfer;
                }
                pTransfer->dwFlags = CSL_USB_OUT_TRANSFER;

                break;

            default:
                status = CSL_ESYS_INVPARAMS;
                break;
        }

        peps = &pContext->pEpStatus[hEp->epNum % CSL_USB_IN_EP0];
        peps->xferType = hEp->xferType;

        if((peps->dwEndpoint == CSL_USB_EP0) && (byteCnt == 0))
        {
            return(CSL_SOK);
        }

        if(peps != NULL)
        {
            peps->pTransfer = pTransfer;
            /* check the last transfer */
            if((peps->pTransfer == NULL)              ||
               (peps->pTransfer->fComplete != TRUE)   ||
               (pContext->fMUSBIsReady != TRUE)       ||
               (pContext->fInitialized != TRUE))
            {
                return(CSL_ESYS_INVPARAMS);
            }

            pTransfer->dwEndpoint       = peps->dwEndpoint;
            pTransfer->pvBuffer         = data;
            pTransfer->cbBuffer         = byteCnt;
            pTransfer->cbTransferred    = 0;
            pTransfer->fComplete        = FALSE;
            pTransfer->ioFlags          = flags;

            /* Call application transfer callback function */
            status = pContext->startTransferCallback(pContext, peps);
        }
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_isTransactionDone
 *
 *   @b Description
 *   @n This function returns the status of the previously posted
 *      data transfer request.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbBoolean
 *   @li                    CSL_USB_TRUE  - If the previously posted transfer
 *                                          is completed.
 *   @li                    CSL_USB_FALSE - If the previously posted transfer
 *                                          is not completed.
 *
 *   <b> Pre Condition </b>
 *   @n  Post transaction API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  1. Returns the previous transaction status
 *   @n  2. Function call status is returned in status variable
 *   @li    CSL_SOK            - Returned for success
 *   @li    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   @b Modifies
 *   @n status variable
 *
 *   @b Example
 *   @verbatim
             void      (*evtHandler)();
             Uint16    dadaBuf[256];

             pUsbEpHandle      hEp;
            CSL_Status        status;
            CSL_UsbBoolean    tscStatus;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);

            status = USB_postTransaction(hEp, 256, dataBuf, CSL_USB_IOFLAG_NONE);
            ...
            ...
            tscStatus = USB_isTransactionDone(hEp ,&status);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbBoolean USB_isTransactionDone(pUsbEpHandle    hEp,
                                     CSL_Status      *status)
{
    pUsbEpStatus    peps;
    pUsbContext     pContext;

    *status  = CSL_SOK;
    pContext = &gUsbContext;
    peps     = NULL;

    if(hEp != NULL)
    {
        peps = &pContext->pEpStatus[hEp->epNum % CSL_USB_IN_EP0];
        /* check the last transfer */
        if(peps->pTransfer != NULL)
        {
            if(peps->pTransfer->fComplete != TRUE)
            {
                return(CSL_USB_FALSE);
            }
        }
        else
        {
            *status = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        *status = CSL_ESYS_BADHANDLE;
    }

    return(CSL_USB_TRUE);
}

/** ============================================================================
 *   @n@b USB_bytesRemaining
 *
 *   @b Description
 *   @n This API returns number of bytes waiting to be transferred from the
 *      previously posted data transfer request.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbByteCount
 *   @n                     Number of bytes waiting to be transferred
 *
 *   <b> Pre Condition </b>
 *   @n  Post transaction API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  1. Returns the number of bytes waiting
 *   @n  2. Function call status is returned in status variable
 *   @li    CSL_SOK            - Returned for success
 *   @li    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   @b Modifies
 *   @n status variable
 *
 *   @b Example
 *   @verbatim
             void      (*evtHandler)();
             Uint16    dadaBuf[256];

             pUsbEpHandle   hEp;
            CSL_Status     status;
            Uint16         byteCount;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);

            status = USB_postTransaction(hEp, 256, dataBuf, CSL_USB_IOFLAG_NONE);
            ...
            ...
            byteCount = USB_bytesRemaining(hEp ,&status);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbByteCount USB_bytesRemaining(pUsbEpHandle    hEp,
                                    CSL_Status      *status)
{
    pUsbEpStatus          peps;
    CSL_UsbByteCount    bytesRem;
    pUsbContext         pContext;

    *status  = CSL_SOK;
    bytesRem = 0;
    peps     = NULL;
    pContext = &gUsbContext;

    if(hEp != NULL)
    {
        peps = &pContext->pEpStatus[hEp->epNum % CSL_USB_IN_EP0];
        if((peps != NULL) && (peps->pTransfer != NULL))
        {
            bytesRem = (peps->pTransfer->cbBuffer -
                                        peps->pTransfer->cbTransferred);
        }
        else
        {
            *status = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        *status = CSL_ESYS_BADHANDLE;
    }

    return(bytesRem);
}

/** ============================================================================
 *   @n@b USB_stallEndpt
 *
 *   @b Description
 *   @n This function stalls an end point
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  End point init function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Stalls an end point associated with the passed handle
 *
 *   @b Modifies
 *   @n USB control registers
 *
 *   @b Example
 *   @verbatim
             void      (*evtHandler)();

             pUsbEpHandle   hEp;
            CSL_Status     status;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);            ...
            ...
            status = USB_stallEndpt(hEp);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_stallEndpt(pUsbEpHandle  hEp)
{
    pUsbEpStatus    peps;
    CSL_Status      status;
    Uint16          saveIndex;
    pUsbContext     pContext;

    status    = CSL_SOK;
    saveIndex = 0;
    peps      = NULL;
    pContext  = &gUsbContext;

    if(hEp != NULL)
    {
        if(hEp->epNum == CSL_USB_IN_EP0)    /* do nothing */
        {
            return(status);
        }

        peps = &pContext->pEpStatus[hEp->epNum % CSL_USB_IN_EP0];

        if((peps != NULL) && (peps->fInitialized == TRUE))
        {
            /* save the index register value */
            saveIndex = usbRegisters->INDEX_TESTMODE;
            peps->fStalled = TRUE;

            if(peps->dwEndpoint == CSL_USB_EP0)
            {
                /* Stall EP0 */
                CSL_FINS(usbRegisters->INDEX_TESTMODE,
                        USB_INDEX_TESTMODE_EPSEL,
                        CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

                CSL_FINS(usbRegisters->INDEX_TESTMODE,
                         USB_INDEX_TESTMODE_EPSEL,
                         CSL_USB_EP0);

                CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_SENDSTALL, TRUE);
            }
            else
            {
                if(hEp->epNum > CSL_USB_IN_EP0)
                {
                    /* Stall Tx Ep */
                    CSL_FINS(usbRegisters->INDEX_TESTMODE,
                             USB_INDEX_TESTMODE_EPSEL,
                             CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

                    CSL_FINS(usbRegisters->INDEX_TESTMODE,
                             USB_INDEX_TESTMODE_EPSEL,
                               peps->dwEndpoint);

                    CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                             USB_PERI_TXCSR_SENDSTALL, TRUE);
                }
                else
                {
                    /* Stall Rx Ep */
                    CSL_FINS(usbRegisters->INDEX_TESTMODE,
                             USB_INDEX_TESTMODE_EPSEL,
                             CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

                    CSL_FINS(usbRegisters->INDEX_TESTMODE,
                             USB_INDEX_TESTMODE_EPSEL,
                             peps->dwEndpoint);

                    CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
                             USB_PERI_RXCSR_SENDSTALL, TRUE);
                }
            }
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    /* restore the index register */
    usbRegisters->INDEX_TESTMODE = saveIndex;

    return(status);
}

/** ============================================================================
 *   @n@b USB_clearEndptStall
 *
 *   @b Description
 *   @n This function clears an end point stall
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_initEndptObj and USB_stallEndpt should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Clears an end point stall
 *
 *   @b Modifies
 *   @n USB control registers
 *
 *   @b Example
 *   @verbatim
              void      (*evtHandler)();

             pUsbEpHandle   hEp;
            CSL_Status     status;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);

            ....
            ....
            status = USB_stallEndpt(hEp);
            ....
            ....
            status = USB_clearEndptStall(hEp);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_clearEndptStall(pUsbEpHandle  hEp)
{
    CSL_Status      status;
    pUsbEpStatus    peps;
    pUsbContext     pContext;
    Uint16          endPtCount;
    Uint16          saveIndex;

    status     = CSL_SOK;
    peps       = NULL;
    endPtCount = CSL_USB_ENDPOINT_COUNT;
    pContext   = &gUsbContext;

    if(hEp != NULL)
    {
        peps = &pContext->pEpStatus[hEp->epNum % CSL_USB_IN_EP0];

        if((peps != NULL) && (peps->fInitialized == TRUE))
        {
            saveIndex = usbRegisters->INDEX_TESTMODE;

            endPtCount = hEp->epNum % CSL_USB_IN_EP0;
            peps->fStalled = FALSE;

            if(peps->dwEndpoint == CSL_USB_EP0)
            {
                /* Data toggle is self clearing for EP0 */
                CSL_FINS(usbRegisters->INDEX_TESTMODE,
                         USB_INDEX_TESTMODE_EPSEL,
                         CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

                CSL_FINS(usbRegisters->INDEX_TESTMODE,
                         USB_INDEX_TESTMODE_EPSEL,
                         CSL_USB_EP0);

                /* flush ep0 fifo */
                CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_FLUSHFIFO, TRUE);

                /* clear stall for EP0 */
                CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_SENTSTALL, FALSE);

                CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_TXCSR_SENDSTALL, FALSE);
            }
            else
            {
                if(hEp->epNum > CSL_USB_IN_EP0)
                {
                    CSL_FINS(usbRegisters->INDEX_TESTMODE,
                             USB_INDEX_TESTMODE_EPSEL,
                             CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

                    CSL_FINS(usbRegisters->INDEX_TESTMODE,
                             USB_INDEX_TESTMODE_EPSEL,
                             endPtCount);

                    /* flush Tx fifo */
                    CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                             USB_PERI_TXCSR_FLUSHFIFO, TRUE);

                    /* Set the data toggle bit to clear the data toggle */
                    CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                             USB_PERI_TXCSR_CLRDATATOG, TRUE);

                    /* clear the sentstall bit */
                    CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                             USB_PERI_TXCSR_SENTSTALL, FALSE);

                    CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                             USB_PERI_TXCSR_SENDSTALL, FALSE);
                }
                else
                {
                    CSL_FINS(usbRegisters->INDEX_TESTMODE,
                             USB_INDEX_TESTMODE_EPSEL,
                             CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

                    CSL_FINS(usbRegisters->INDEX_TESTMODE,
                             USB_INDEX_TESTMODE_EPSEL,
                             endPtCount);

                    /* flush Rx fifo */
                    CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
                             USB_PERI_RXCSR_FLUSHFIFO, TRUE);

                    /* Set the data toggle bit to clear the data toggle */
                    CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
                             USB_PERI_RXCSR_CLRDATATOG, TRUE);

                    /* Reset the RxStall bit */
                    CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
                             USB_PERI_RXCSR_SENTSTALL, FALSE);

                    CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
                             USB_PERI_RXCSR_SENDSTALL, FALSE);
                }
            }
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    /* restore the index register */
    usbRegisters->INDEX_TESTMODE = saveIndex;

    return(status);
}

/** ============================================================================
 *   @n@b USB_getEndptStall
 *
 *   @b Description
 *   @n This function checks if an endpoint is stalled.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
            status      Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbBoolean
 *   @li                    CSL_USB_TRUE  - If the end point is stalled.
 *   @li                    CSL_USB_FALSE - If the end point is not stalled
 *
 *   <b> Pre Condition </b>
 *   @n  Init End point API should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  1. Returns the end point stall status
 *   @n  2. Function call status is returned in status variable
 *   @li    CSL_SOK            - Returned for success
 *   @li    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   @b Modifies
 *   @n status variable
 *
 *   @b Example
 *   @verbatim
             void      (*evtHandler)();

             pUsbEpHandle      hEp;
            CSL_Status        status;
            CSL_UsbBoolean    epStall;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);
            ...
            ...
            epStall = USB_getEndptStall(hEp, &status);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbBoolean USB_getEndptStall(pUsbEpHandle    hEp,
                                 CSL_Status      *status)
{
    pUsbEpStatus      peps;
    CSL_UsbBoolean    fStalled;
    pUsbContext       pContext;

    *status  = CSL_SOK;
    fStalled = CSL_USB_FALSE;
    peps     = NULL;
    pContext = &gUsbContext;

    if(hEp != NULL)
    {
        peps = &pContext->pEpStatus[hEp->epNum % CSL_USB_IN_EP0];

        if(peps != NULL)
        {
            if(peps->dwEndpoint < CSL_USB_ENDPOINT_COUNT)
            {
                fStalled = (CSL_UsbBoolean)peps->fStalled;
            }
        }
        else
        {
            *status = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        *status = CSL_ESYS_BADHANDLE;
    }

    return(fStalled);
}

/** ============================================================================
 *   @n@b USB_epNumToHandle
 *
 *   @b Description
 *   @n This function returns handle to an end point
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            endPt         16-bit endpoint number as specified
                           in the USB specification.
                           0x00 -> Endpt 0 Out.
                           0x01 -> Endpt 1 Out.
                            0x80 -> Endpt 0 In.
                            0x81 -> Endpt 1 In.
     @endverbatim
 *
 *   <b> Return Value </b>  pUsbEpHandle
 *   @n                     Handle to the endpoint object if a valid
 *                          endpoint object exists, else NULL object.
 *
 *   <b> Pre Condition </b>
 *   @n  USB_init function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  returns End point handle
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig    usbConfig;
            CSL_Status       status;
            pUsbEpHandle     hEp;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            .....
            hEp = USB_epNumToHandle(CSL_USB0, CSL_USB_IN_TRANSFER);
     @endverbatim
 *  ============================================================================
 */
pUsbEpHandle USB_epNumToHandle(CSL_UsbDevNum    devNum,
                               Uint16           endpt)
{
    pUsbEpHandle    epHandle;
    Uint16             looper;
    Uint16            epNum;
    Bool            fValidEp;

    fValidEp = FALSE;
    epHandle = NULL;
    looper   = 0;

    if(endpt & CSL_USB_IN_TRANSFER) /* IN Endpoint */
    {
        endpt = (endpt & ~(CSL_USB_IN_TRANSFER));

        switch(endpt)
        {
            case CSL_USB_EP0:
                epNum    = CSL_USB_IN_EP0;
                fValidEp = TRUE;
                break;

            case CSL_USB_EP1:
                epNum    = CSL_USB_IN_EP1;
                fValidEp = TRUE;
                break;

            case CSL_USB_EP2:
                epNum    = CSL_USB_IN_EP2;
                fValidEp = TRUE;
                break;

            case CSL_USB_EP3:
                epNum    = CSL_USB_IN_EP3;
                fValidEp = TRUE;
                break;

            case CSL_USB_EP4:
                epNum    = CSL_USB_IN_EP4;
                fValidEp = TRUE;
                break;

            default:
                break;
        }
    }
    else            /* OUT Endpoint */
    {
        switch(endpt)
        {
            case CSL_USB_EP0:
                epNum    = CSL_USB_OUT_EP0;
                fValidEp = TRUE;
                break;

            case CSL_USB_EP1:
                epNum    = CSL_USB_OUT_EP1;
                fValidEp = TRUE;
                break;

            case CSL_USB_EP2:
                epNum    = CSL_USB_OUT_EP2;
                fValidEp = TRUE;
                break;

            case CSL_USB_EP3:
                epNum    = CSL_USB_OUT_EP3;
                fValidEp = TRUE;
                break;

            case CSL_USB_EP4:
                epNum    = CSL_USB_OUT_EP4;
                fValidEp = TRUE;
                break;

            default:
                break;
        }
    }

    if(fValidEp != TRUE)
    {
        return(NULL);
    }

    for(looper = 0; looper < CSL_USB_ENDPOINT_COUNT; looper++)
    {
        if(epNum == gUsbEpHandle[looper]->epNum)
        {
            epHandle = gUsbEpHandle[looper];
            break;
        }
    }

    return(epHandle);
}

/** ============================================================================
 *   @n@b USB_abortTransaction
 *
 *   @b Description
 *   @n This function terminates the data transfer in progress and
 *      free up the endpoint to post a new data transfer request.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_BADHANDLE - Invalid end point handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_postTransaction function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Terminates data transfer in progress
 *
 *   @b Modifies
 *   @n USB registers
 *
 *   @b Example
 *   @verbatim
             Uint16    dadaBuf[256];

             pUsbEpHandle    hEp;
            CSL_Status      status;

            status = USB_postTransaction(hEp, 256, dataBuf, CSL_USB_IOFLAG_NONE);
            .....
            .....
            status = USB_abortTransaction(hEp);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_abortTransaction(pUsbEpHandle    hEp)
{
    pUsbEpStatus    peps;
    CSL_Status      status;
    Uint16          saveIndex;
    pUsbContext     pContext;
    Uint16          tempVal;

    status   = CSL_SOK;
    peps     = NULL;
    pContext = &gUsbContext;

    if(hEp != NULL)
    {
        peps = &pContext->pEpStatus[hEp->epNum % CSL_USB_IN_EP0];

        if((peps != NULL) && (peps->fInitialized == TRUE))
        {
            if(peps->pTransfer != NULL)
            {
                peps->pTransfer->fComplete = TRUE;
                peps->pTransfer = NULL;
            }

            /* save the index register value */
            saveIndex = usbRegisters->INDEX_TESTMODE;

            /* Reset the end point select field */
            CSL_FINS(usbRegisters->INDEX_TESTMODE,
                     USB_INDEX_TESTMODE_EPSEL,
                     CSL_USB_INDEX_TESTMODE_EPSEL_RESETVAL);

            if(peps->dwEndpoint == CSL_USB_EP0)
            {
                /* flush ep0 fifo */
                CSL_FINS(usbRegisters->INDEX_TESTMODE,
                         USB_INDEX_TESTMODE_EPSEL,
                         CSL_USB_EP0);

                CSL_FINS(usbRegisters->PERI_CSR0_INDX,
                         USB_PERI_CSR0_INDX_FLUSHFIFO, TRUE);
            }
            else
            {
                if(hEp->epNum > CSL_USB_IN_EP0)
                {
                    /* flush Tx fifo */
                    CSL_FINS(usbRegisters->INDEX_TESTMODE,
                             USB_INDEX_TESTMODE_EPSEL,
                             peps->dwEndpoint);

                    tempVal = usbRegisters->PERI_CSR0_INDX;

                    tempVal |= CSL_USB_PERI_TXCSR_FLUSHFIFO_MASK;
                    tempVal &= ~(0x03);

                    usbRegisters->PERI_CSR0_INDX = tempVal;
                }
                else
                {
                    /* flush Rx fifo */
                    CSL_FINS(usbRegisters->INDEX_TESTMODE,
                             USB_INDEX_TESTMODE_EPSEL,
                             peps->dwEndpoint);

                    CSL_FINS(usbRegisters->PERI_RXCSR_INDX,
                             USB_PERI_RXCSR_FLUSHFIFO, TRUE);
                }
            }

            /* restore the index register */
            usbRegisters->INDEX_TESTMODE = saveIndex;
        }
        else
        {
            status = CSL_ESYS_INVPARAMS;
        }
    }
    else
    {
        status = CSL_ESYS_BADHANDLE;
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_abortAllTransaction
 *
 *   @b Description
 *   @n This function terminates all the data transfer in progress and
 *      free up the endpoints to post new data transfer requests.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_postTransaction function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Aborts all transactions in progress
 *
 *   @b Modifies
 *   @n USB registers
 *
 *   @b Example
 *   @verbatim
             Uint16    dadaBuf[256];

             pUsbEpHandle    hEp;
            CSL_Status      status;

            status = USB_postTransaction(hEp, 256, dataBuf, CSL_USB_IOFLAG_NONE);
            .....
            .....
            status = USB_abortAllTransaction(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_abortAllTransaction(CSL_UsbDevNum    devNum)
{
    pUsbEpHandle    epHandle;
    CSL_Status      status;
    Uint16          endPoint;

    status = CSL_SOK;

    if(devNum != CSL_USB0)
    {
        status = CSL_ESYS_INVPARAMS;
    }
    else
    {
        for(endPoint = 0; endPoint < CSL_USB_ENDPOINT_COUNT; endPoint++)
        {
            epHandle = gUsbEpHandle[endPoint];
            if(epHandle != NULL)
            {
                status = USB_abortTransaction(epHandle);
                if(status != CSL_SOK)
                {
                    break;
                }
            }
        }
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_connectDev
 *
 *   @b Description
 *   @n This function connects the USB module to upstream port
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init function should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Connects the USB module
 *
 *   @b Modifies
 *   @n USB FADDR_POWER register
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_connectDev(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_connectDev(CSL_UsbDevNum    devNum)
{
    CSL_Status    status;

    status = CSL_SOK;

    switch(devNum)
    {
        case CSL_USB0:
            CSL_FINS(usbRegisters->FADDR_POWER,
                     USB_FADDR_POWER_SOFTCONN, TRUE);
            break;

        /* Currently USB1 and USB2 are not supported */
        case CSL_USB1:
        case CSL_USB2:
        default:
             status = CSL_ESYS_INVPARAMS;
            break;
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_disconnectDev
 *
 *   @b Description
 *   @n This function disconnects the USB module from the upstream port
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_connectDev should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Disconnects the USB module.
 *
 *   @b Modifies
 *   @n USB FADDR_POWER register
 *
 *   @b Example
 *   @verbatim
             CSL_Status    status;

            status = USB_connectDev(CSL_USB0);
            ....
            ....
            status = USB_disconnectDev(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_disconnectDev(CSL_UsbDevNum    devNum)
{
    CSL_Status    status;

    status = CSL_SOK;

    switch(devNum)
    {
        case CSL_USB0:
            CSL_FINS(usbRegisters->FADDR_POWER,
                     USB_FADDR_POWER_SOFTCONN, FALSE);
            break;

        /* Currently USB1 and USB2 are not supported */
        case CSL_USB1:
        case CSL_USB2:
        default:
             status = CSL_ESYS_INVPARAMS;
            break;
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_setRemoteWakeup
 *
 *   @b Description
 *   @n This function sets or clears Remote Wakeup Feature.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            rmtWkpStat    Remote wake up status flag
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init function should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Sets the remote wake up status
 *
 *   @b Modifies
 *   @n Remote wakeup status variable in the USB global context structure
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_setRemoteWakeup(CSL_USB0, CSL_USB_TRUE);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_setRemoteWakeup(CSL_UsbDevNum     devNum,
                               CSL_UsbBoolean    rmtWkpStat)
{
    CSL_Status     status;
    pUsbContext    pContext;

    status   = CSL_SOK;
    pContext = &gUsbContext;

    switch(devNum)
    {
        case CSL_USB0:
             pContext->remoteWkpStat = rmtWkpStat;
            break;

        /* Currently USB1 and USB2 are not supported */
        case CSL_USB1:
        case CSL_USB2:
        default:
             status = CSL_ESYS_INVPARAMS;
            break;
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_getRemoteWakeupStat
 *
 *   @b Description
 *   @n This function returns the remote wake up feature status.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbBoolean
 *   @li                    CSL_USB_TRUE  - Remote wakeup feature is enabled.
 *   @li                    CSL_USB_FALSE - Remote wakeup feature is disabled.
 *
 *   <b> Pre Condition </b>
 *   @n  USB_setRemoteWakeup function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns remote wakeup status
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_Status        status;
            CSL_UsbBoolean    rmtWkpStat

            status = USB_setRemoteWakeup(CSL_USB0, CSL_USB_TRUE);

            if(status == CSL_SOK)
            {
                rmtWkpStat = USB_getRemoteWakeupStat(CSL_USB0);
            }
     @endverbatim
 *  ============================================================================
 */
CSL_UsbBoolean USB_getRemoteWakeupStat(CSL_UsbDevNum    devNum)
{
    pUsbContext    pContext;

    pContext = &gUsbContext;

    return(pContext->remoteWkpStat);
}

/** ============================================================================
 *   @n@b USB_issueRemoteWakeup
 *
 *   @b Description
 *   @n This function issues a remote wakeup signal to the host.
 *      NOTE: For remote wakeup protocol application program needs to call
 *      this function two times. In the first call resumeBitVal should be
 *      TRUE and in the second call resumeBitVal should be FALSE.
 *      Application should give a delay of 2 - 15 mSecs between the two calls.
 *      This is to avoid delays internal to the CSL module
 *
 *   @b Arguments
 *   @verbatim
            devNum           USB device number, Only USB0 is active currently.
            resumeBitVal     Value of the resume bit - TRUE or FALSE
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_setRemoteWakeup function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Issues a remote wake up signal.
 *
 *   @b Modifies
 *   @n USB FADDR POWER register
 *
 *   @b Example
 *   @verbatim
            CSL_Status    status;

            status = USB_setRemoteWakeup(CSL_USB0, CSL_USB_TRUE);

            if(status == CSL_SOK)
            {
                status = USB_issueRemoteWakeup(CSL_USB0);
            }
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_issueRemoteWakeup(CSL_UsbDevNum    devNum,
                                 Bool             resumeBitVal)
{
    CSL_Status     status;
    pUsbContext    pContext;

    status   = CSL_SOK;
    pContext = &gUsbContext;

    switch(devNum)
    {
        case CSL_USB0:
             if(pContext->remoteWkpStat == CSL_USB_TRUE)
             {
                /** Set/Reset USB Resume bit */
                CSL_FINS(usbRegisters->FADDR_POWER,
                         USB_FADDR_POWER_RESUME, resumeBitVal);
            }
            break;

        /* Currently USB1 and USB2 are not supported */
        case CSL_USB1:
        case CSL_USB2:
        default:
             status = CSL_ESYS_INVPARAMS;
            break;
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_setDevAddr
 *
 *   @b Description
 *   @n This function sets the device Address to USB controller
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            addr          Address to set
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Configures device address registers
 *
 *   @b Modifies
 *   @n Device address registers
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            Uint16             addr;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_setDevAddr(CSL_USB0, addr);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_setDevAddr(CSL_UsbDevNum    devNum,
                          Uint16            addr)
{
    CSL_Status    status;

    status = CSL_SOK;

    switch(devNum)
    {
        case CSL_USB0:
            CSL_FINS(usbRegisters->FADDR_POWER,
                     USB_FADDR_POWER_FUNCADDR, FALSE);
            CSL_FINS(usbRegisters->FADDR_POWER,
                     USB_FADDR_POWER_FUNCADDR, addr);
            break;

        /* Currently USB1 and USB2 are not supported */
        case CSL_USB1:
        case CSL_USB2:
        default :
            status = CSL_ESYS_INVPARAMS;
            break;
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_getFrameNo
 *
 *   @b Description
 *   @n This function reads current USB frame number
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_UsbFrameNum
 *   @n                     Current USB frame number
 *
 *   <b> Pre Condition </b>
 *   @n  USB init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns the frame number
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            Uint16             addr;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_getFrameNo(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_UsbFrameNum USB_getFrameNo(CSL_UsbDevNum    devNum)
{
    CSL_UsbFrameNum    frameNum;

    frameNum = CSL_FEXT(usbRegisters->FRAME, USB_FRAME_FRAMENUMBER);

    return(frameNum);
}

/** ============================================================================
 *   @n@b USB_suspendDevice
 *
 *   @b Description
 *   @n This function suspends the USB device. This function informs the
 *      application about the device suspend through a call back function.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Puts the USB device into suspend mode.
 *
 *   @b Modifies
 *   @n USB FADDR_POWER register
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            Uint16             addr;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            status = USB_suspendDevice(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_suspendDevice(CSL_UsbDevNum    devNum)
{
    CSL_Status      status;
    pUsbContext     pContext;

    status   = CSL_SOK;
    pContext = &gUsbContext;

    switch(devNum)
    {
        case CSL_USB0:
            /* Call application suspend call back function */
            pContext->suspendCallBackFxn(CSL_SOK);

            CSL_FINS(usbRegisters->FADDR_POWER,
                     USB_FADDR_POWER_ENSUSPM, TRUE);
            break;

        /* Currently USB1 and USB2 are not supported */
        case CSL_USB1:
        case CSL_USB2:
        default :
            status = CSL_ESYS_INVPARAMS;
            break;
    }

    return(status);
}


/** ============================================================================
 *   @n@b USB_selfwakeup
 *
 *   @b Description
 *   @n This function issues a self wakeup signal on the bus.
 *      This function can be mapped to an external event like pressing a button
 *      on the EVM. After responding to that event through a call back, USB
 *      device send a wakeup signal on the bus.
 *      NOTE: Application needs to send a delay as a parameter to this function.
 *      According to USB wakeup specification an approximate delay of
 *      10mSecs(Min 2 to Max 15 mSecs) is required to send a wakeup signal on
 *      the bus. This delay is introduced between setting the resume bit to '1'
 *      and restting it to '0'. Application needs to decide a delay such that
 *      it is maximum enough to satisfy USB wakeup protocol and minimum to
 *      satisfy the USB driver performance.
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            wakeupDelay   Delay between Resume bit set and reset
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  USB init be called successfully and an external event should occur.
 *
 *   <b> Post Condition </b>
 *   @n  Send a wakeup signal on the bus.
 *
 *   @b Modifies
 *   @n USB FADDR_POWER register
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            Uint16             addr;
            Uint32             wakeupDelay;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            ....
            ....
            //This function call should be mapped to an external event
            wakeupDelay = 1000000;
            status = USB_selfwakeup(CSL_USB0, wakeupDelay);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_selfwakeup(CSL_UsbDevNum    devNum,
                          Uint32           wakeupDelay)
{
    CSL_Status     status;
    pUsbContext    pContext;

    status   = CSL_SOK;
    pContext = &gUsbContext;

    switch(devNum)
    {
        case CSL_USB0:
            /* Call the application call back function */
            pContext->wakeupCallBackFxn(CSL_SOK);

            /** Set USB Resume bit */
            CSL_FINS(usbRegisters->FADDR_POWER,
                     USB_FADDR_POWER_RESUME, TRUE);

            for(; wakeupDelay > 0; wakeupDelay--);

            /** Reset the resume bit */
            CSL_FINS(usbRegisters->FADDR_POWER,
                     USB_FADDR_POWER_RESUME, FALSE);
            break;

        /* Currently USB1 and USB2 are not supported */
        case CSL_USB1:
        case CSL_USB2:
        default :
            status = CSL_ESYS_INVPARAMS;
            break;
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_initPLL
 *
 *   @b Description
 *   @n Initializes PLL for USB operation. This function initializes the PLL to
 *      100 MHz. For configuring PLL to any other frequency use PLL module.
 *
 *   @b Arguments
 *   @verbatim
                   None
     @endverbatim
 *
 *   <b> Return Value </b> None
 *   @n
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes PLL
 *
 *   @b Modifies
 *   @n PLL registers
 *
 *   @b Example
 *   @verbatim
            USB_initPLL();
     @endverbatim
 *  ============================================================================
 */
void USB_initPLL(void)
{
    Uint16    timeout;

    timeout = CSL_USB_PLL_TIMEOUT;

    /* bypass PLL */
    CSL_FINST(sysCtrlRegs->CCR2, SYS_CCR2_SYSCLKSEL, BYPASS);

    /* Configure the PLL for 100 MHz */
    sysCtrlRegs->CGICR = CSL_USB_PLL_PLLINCNTL_DEFVAL;
    sysCtrlRegs->CGOCR = CSL_USB_PLL_PLLOUTCNTL_DEFVAL;
    sysCtrlRegs->CGCR2 = CSL_USB_PLL_PLLCNTL2_DEFVAL;
    sysCtrlRegs->CGCR1 = CSL_USB_PLL_PLLCNTL1_DEFVAL;

    /* Busy wait until TESTLOCKMON is high or timeout */
    while(!((sysCtrlRegs->CGCR2) & (0x08)))
    {
        timeout--;
        if(timeout == 0)
        {
            break;
        }
    }

    /* Switch to PLL clk */
    CSL_FINST(sysCtrlRegs->CCR2, SYS_CCR2_SYSCLKSEL, LOCK);
}


/** ============================================================================
 *   @n@b USB_setParams
 *
 *   @b Description
 *   @n This function initializes the global end point handle array
 *
 *   @b Arguments
 *   @verbatim
            devNum        USB device number, Only USB0 is active currently.
            hEpObj        Pointer to a array of endpoint handle.
            pSofTmrCnt    8-bit counter value for the pre SOF timer.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes global end point handle array
 *
 *   @b Modifies
 *   @n Global end point handle array
 *
 *   @b Example
 *   @verbatim
            CSL_UsbEpObj    hEpObjArray[CSL_USB_ENDPOINT_COUNT];
            CSL_Status      status;

            status = USB_setParams(CSL_USB0, &hEpObjArray, 0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status USB_setParams(CSL_UsbDevNum    devNum,
                         pUsbEpHandle     hEpObj[],
                         Uint16           pSofTmrCnt)
{
    CSL_Status    status;
    Uint16        looper;

    status = CSL_SOK;

    if(devNum != CSL_USB0)
    {
        status = CSL_ESYS_INVPARAMS;
    }
    else
    {
        for (looper = 0; looper < CSL_USB_ENDPOINT_COUNT; looper++)
        {
            gUsbEpHandle[looper] = hEpObj[looper];

            if(hEpObj[looper] == NULL)
            {
                break;
            }
        }
    }

    return(status);
}

/** ============================================================================
 *   @n@b USB_getMaxPower
 *
 *   @b Description
 *   @n This function returns the maximum current drawn by the device.
 *      This is to inform the host about the device power requirement.
 *      This value is passed to the host through the configuration
 *      descriptor.
 *
 *   @b Arguments
 *   @verbatim
             None
     @endverbatim
 *
 *   <b> Return Value </b> Maximum current drawn by the device
 *
 *   <b> Pre Condition </b>
 *   @n  USB_init function should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Returns the maximum current drawn by the device
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            CSL_UsbConfig      usbConfig;
            CSL_Status         status;
            Uint16             maxPower;

            usbConfig.maxCurrent = 50;
            .....
            .....
            status = USB_init(&usbConfig);
            .....
            maxPower = USB_getMaxPower(CSL_USB0);
     @endverbatim
 *  ============================================================================
 */
WORD USB_getMaxPower(CSL_UsbDevNum    devNum)
{
    pUsbContext    pContext;

    pContext = &gUsbContext;

    if(devNum == CSL_USB0)
    {
        return(pContext->maxCurrent);
    }
    else
    {
        return(CSL_USB_INVALID_MAXPOWER);
    }
}

/** ============================================================================
 *   @n@b USB_setFullSpeedMode
 *
 *   @b Description
 *   @n This function is used to set the USB controller into full speed mode
 *      ep0DataSize is the size of the EP0 FIFO. This value should be within
 *      the range of values recommended by the USB specification.
 *
 *   @b Arguments
 *   @verbatim
             None
     @endverbatim
 *
 *   <b> Return Value </b> None
 *
 *   <b> Pre Condition </b>
 *   @n  USB Init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  USB will be set for full speed mode
 *
 *   @b Modifies
 *   @n USB registers
 *
 *   @b Example
 *   @verbatim
            CSL_UsbSetupStruct    usbSetup;
            CSL_Status            status;
            CSL_UsbConfig         usbConfig

            status = USB_init(&usbConfig);
            ....
            ....
            USB_setFullSpeedMode(0x20);

     @endverbatim
 *  ============================================================================
 */
void USB_setFullSpeedMode(Uint16    ep0DataSize)
{
    pUsbContext     pContext;
    pUsbEpStatus    peps;

    pContext = &gUsbContext;
    peps = &pContext->pEpStatus[CSL_USB_EP0];
    peps->dwPacketSizeAssigned = ep0DataSize;

    CSL_FINS(usbRegisters->FADDR_POWER, USB_FADDR_POWER_HSEN, FALSE);
}

/** ============================================================================
 *   @n@b USB_getDataCountReadFromFifo
 *
 *   @b Description
 *   @n This function returns the most recent data count read from the FIFO.
 *      This function is supposed to use for the OUT end point. This function
 *      always return 0 for IN end point.
 *
 *   @b Arguments
 *   @verbatim
            hEp         Handle to an initialized endpoint object.
     @endverbatim
 *
 *   <b> Return Value </b>  Data count read from FIFO
 *
 *   <b> Pre Condition </b>
 *   @n  End point object should be initialized
 *
 *   <b> Post Condition </b>
 *   @n  Terminates data transfer in progress
 *
 *   @b Modifies
 *   @n USB registers
 *
 *   @b Example
 *   @verbatim
             void  (*evtHandler)();

             pUsbEpHandle     hEp;
            CSL_Status       status;
            Uint16           fifoDataCount;

            status = USB_initEndptObj(CSL_USB0,
                                      hEp,
                                      CSL_USB_OUT_EP0,
                                      CSL_USB_CTRL,
                                      0x40,
                                      CSL_USB_EVENT_RESET,
                                      evtHandler);

            fifoDataCount = USB_getEvents(hEp);
     @endverbatim
 *  ============================================================================
 */
Uint16 USB_getDataCountReadFromFifo (pUsbEpHandle    hEp)
{
    pUsbEpStatus    peps;
    pUsbContext     pContext;
    Uint16          fifoDataCount;

    peps          = NULL;
    pContext      = &gUsbContext;
    fifoDataCount = 0;

    if (hEp != NULL)
    {
        peps = &pContext->pEpStatus[hEp->epNum % CSL_USB_IN_EP0];

        fifoDataCount = peps->dataReadFromFifo;
    }

    return (fifoDataCount);
}

// determine the fifo size according to the transfer type and the maximum packet size
Uint16 USB_findFifoSize(Uint16 maxPktSize, CSL_UsbXferType xferType)
{
    pUsbContext     pContext;
    Uint16            fifoSize;

    pContext = &gUsbContext;

    // if it is ISO mode, compute the correct fifo size
    // according to the USB speed and the maximum packet size
    if (xferType == CSL_USB_ISO)
    {
        // if it is high speed, the maximum packet size can be up to 1024 byte
        if (pContext->busSpeed)
        {
            // if the maximum packet size > 1024 byte, then return error
            if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_1024+3)))
            {
                // illegal maximum packet size
                return 0;
            } else
            {
                // if the maximum packet size > 512 byte, then assign 1024 byte
                if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_512+3)))
                    fifoSize = CSL_USB_TX_FIFO_SIZE_1024;
                else
                {
                    // if the maximum packet size > 256 byte, then assign 512 byte
                    if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_256+3)))
                        fifoSize = CSL_USB_TX_FIFO_SIZE_512;
                    else
                    {
                        // if the maximum packet size > 128 byte, then assign 256 byte
                        if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_128+3)))
                            fifoSize = CSL_USB_TX_FIFO_SIZE_256;
                        else
                        {
                            // if the maximum packet size > 64 byte, then assign 128 byte
                            if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_64+3)))
                                fifoSize = CSL_USB_TX_FIFO_SIZE_128;
                            else
                            {
                                // everything else will be using 64 byte
                                fifoSize = CSL_USB_TX_FIFO_SIZE_64;
                            }
                        }
                    }
                }
            }
        } else
        {
            // if it is full speed, then the maximum packet size can be 1023
            // if the maximum packet size > 1023 byte, then return error
            if (maxPktSize>1023)
            {
                // illegal maximum packet size
                return 0;
            } else
            {
                // if the maximum packet size > 512 byte, then assign 1024 byte
                if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_512+3)))
                    fifoSize = CSL_USB_TX_FIFO_SIZE_1024;
                else
                {
                    // if the maximum packet size > 256 byte, then assign 512 byte
                    if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_256+3)))
                        fifoSize = CSL_USB_TX_FIFO_SIZE_512;
                    else
                    {
                        // if the maximum packet size > 128 byte, then assign 256 byte
                        if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_128+3)))
                            fifoSize = CSL_USB_TX_FIFO_SIZE_256;
                        else
                        {
                            // if the maximum packet size > 64 byte, then assign 128 byte
                            if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_64+3)))
                                fifoSize = CSL_USB_TX_FIFO_SIZE_128;
                            else
                            {
                                // everything else will be using 64 byte
                                fifoSize = CSL_USB_TX_FIFO_SIZE_64;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        // if it is Interrupt mode, compute the correct fifo size
        // according to the USB speed and the maximum packet size
        if (xferType == CSL_USB_INTR)
        {
            // if it is high speed, the maximum packet size can be up to 1024 byte
            if (pContext->busSpeed)
            {
                // if the maximum packet size > 1025 byte, then return error
                if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_1024+3)))
                {
                    // illegal maximum packet size
                    return 0;
                } else
                {
                    // if the maximum packet size > 512 byte, then assign 1024 byte
                    if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_512+3)))
                        fifoSize = CSL_USB_TX_FIFO_SIZE_1024;
                    else
                    {
                        // if the maximum packet size > 256 byte, then assign 512 byte
                        if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_256+3)))
                            fifoSize = CSL_USB_TX_FIFO_SIZE_512;
                        else
                        {
                            // if the maximum packet size > 128 byte, then assign 256 byte
                            if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_128+3)))
                                fifoSize = CSL_USB_TX_FIFO_SIZE_256;
                            else
                            {
                                // if the maximum packet size > 64 byte, then assign 128 byte
                                if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_64+3)))
                                    fifoSize = CSL_USB_TX_FIFO_SIZE_128;
                                else
                                {
                                    // everything else will be using 64 byte
                                    fifoSize = CSL_USB_TX_FIFO_SIZE_64;
                                }
                            }
                        }
                    }
                }
            } else
            {
                // if it is full speed, then the maximum packet size can be 64
                fifoSize = CSL_USB_TX_FIFO_SIZE_64;
            }
        } else
        {
            // if it is Bulk mode, compute the correct fifo size
            // according to the USB speed and the maximum packet size
            if (xferType == CSL_USB_BULK)
            {
                // if it is high speed, the maximum packet size can be up to 512 byte
                if (pContext->busSpeed)
                {
                    // if the maximum packet size > 512 byte, then return error
                    if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_512+3)))
                    {
                        // illegal maximum packet size
                        return 0;
                    } else
                    {
                        // if the maximum packet size > 256 byte, then assign 512 byte
                        if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_256+3)))
                            fifoSize = CSL_USB_TX_FIFO_SIZE_512;
                        else
                        {
                            // if the maximum packet size > 128 byte, then assign 256 byte
                            if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_128+3)))
                                fifoSize = CSL_USB_TX_FIFO_SIZE_256;
                            else
                            {
                                // if the maximum packet size > 64 byte, then assign 128 byte
                                if (maxPktSize>(1<<(CSL_USB_TX_FIFO_SIZE_64+3)))
                                    fifoSize = CSL_USB_TX_FIFO_SIZE_128;
                                else
                                {
                                    // everything else will be using 64 byte
                                    fifoSize = CSL_USB_TX_FIFO_SIZE_64;
                                }
                            }
                        }
                    }
                } else
                {
                    // if it is full speed, then the maximum packet size can be 64
                    fifoSize = CSL_USB_TX_FIFO_SIZE_64;
                }
            } else
            {
                // if it is Control mode, then the maximum packet size can be 64
                fifoSize = CSL_USB_TX_FIFO_SIZE_64;
            }
        }
    }

    return fifoSize;
}

// processing all the EP0 events for enumeration
// call the CDC event handler for further processing
CSL_Status USB_coreEventProcessEp0(pUsbContext pContext)
{
    pUsbEpHandle hEPx;
    Uint16 inOutFlag, tempLen;
    Uint16 saveIndex, endpt;
    CSL_Status status;
    Uint16 curConfigureNum, curInterfaceNum;
    Uint16 UsbCtrlBuffer[8];

    status = CSL_SOK;

    saveIndex = usbRegisters->INDEX_TESTMODE;
        CSL_FINS(usbRegisters->INDEX_TESTMODE,
                 USB_INDEX_TESTMODE_EPSEL, CSL_USB_EP0);

    USB_getSetupPacket(CSL_USB0, &pContext->usbSetup, TRUE);

    if((usbRegisters->PERI_CSR0_INDX & CSL_USB_PERI_CSR0_INDX_RXPKTRDY_MASK)
        == CSL_USB_PERI_CSR0_INDX_RXPKTRDY_MASK)
    {
        // Service the RXPKTRDY after reading the FIFO
        USB_clearEpRxPktRdy(CSL_USB_EP0);

        /* GET DESCRIPTOR Req */
        switch(pContext->usbSetup.bRequest)
        {
            /* zero data */
            case CSL_USB_SET_FEATURE:
                switch(pContext->usbSetup.wValue)
                {
                    case CSL_USB_FEATURE_ENDPOINT_STALL:
                        /* updated set and clear endpoint stall
                         * to work with logical endpoint num
                         */
                        endpt = (pContext->usbSetup.wIndex) & 0xFF;
                        hEPx = USB_epNumToHandle(CSL_USB0, endpt);
                        if(!(USB_getEndptStall(hEPx, &status)))
                        {
                            USB_stallEndpt(hEPx);
                        }
                        break;

                    case CSL_USB_FEATURE_REMOTE_WAKEUP:
                        if(!(USB_getRemoteWakeupStat(CSL_USB0)))
                        {
                            USB_setRemoteWakeup(CSL_USB0, CSL_USB_TRUE);
                        }
                        break;

                    default:
                        break;
                }

                // Service the RXPKTRDY after reading the FIFO
                USB_clearEpRxPktRdy(CSL_USB_EP0);

                break;

            case CSL_USB_CLEAR_FEATURE:
                switch(pContext->usbSetup.wValue)
                {
                    case CSL_USB_FEATURE_ENDPOINT_STALL:
                        endpt = (pContext->usbSetup.wIndex) & 0xFF;
                        hEPx = USB_epNumToHandle(CSL_USB0, endpt);
                        if(USB_getEndptStall(hEPx, &status))
                        {
                            USB_clearEndptStall(hEPx);
                        }
                        break;

                    case CSL_USB_FEATURE_REMOTE_WAKEUP:
                        if(USB_getRemoteWakeupStat(CSL_USB0))
                        {
                            USB_setRemoteWakeup(CSL_USB0, CSL_USB_FALSE);
                        }
                        break;

                    default:
                         break;
                }

                // Service the RXPKTRDY after reading the FIFO
                USB_clearEpRxPktRdy(CSL_USB_EP0);

                break;

            case CSL_USB_SET_CONFIGURATION:
                curConfigureNum = pContext->usbSetup.wValue;
                USB_setConfiguration(CSL_USB0, pContext->usbSetup.wValue);

                // Service the RXPKTRDY after reading the FIFO
                USB_clearEpRxPktRdy(CSL_USB_EP0);

                break;

            case CSL_USB_SET_INTERFACE:
                curInterfaceNum = pContext->usbSetup.wValue;
                if (pContext->usbSetup.wValue==0)
                    USB_setConfiguration(CSL_USB0, 0);

                // Service the RXPKTRDY after reading the FIFO
                USB_clearEpRxPktRdy(CSL_USB_EP0);

                break;

            case CSL_USB_GET_DESCRIPTOR :

                switch(pContext->usbSetup.wValue >> 8)
                {
                    case CSL_USB_DEVICE_DESCRIPTOR_TYPE:
                        ///deviceDescPtr = (Uint16 *)deviceDesc;
                        status = USB_postTransaction(pContext->hEpObjArray[1],
                                         pContext->deviceDescPtr[0]&0xFF, pContext->deviceDescPtr,
                                         CSL_USB_IN_TRANSFER);
                        break;

                    case CSL_USB_CONFIGURATION_DESCRIPTOR_TYPE:
                        if(pContext->usbSetup.wLength == 0x0009)
                        {
                            ///cfgDescPtr = cfgDesc;
                            status = USB_postTransaction(pContext->hEpObjArray[1],
                                             9, pContext->cfgDescPtr,
                                             CSL_USB_IN_TRANSFER);
                        }
                        else
                        {
                            ///cfgDescPtr = cfgDesc;
                            status = USB_postTransaction(pContext->hEpObjArray[1],
                                             pContext->cfgDescPtr[1]&0xFF, pContext->cfgDescPtr,
                                             CSL_USB_IN_TRANSFER);
                        }

                        break;

                    case CSL_USB_OTHERSPEED_CFG_DESCRIPTOR_TYPE:
                        if(pContext->usbSetup.wLength == 0x0009)
                        {
                             inOutFlag = (CSL_USB_IOFLAG_LNK | CSL_USB_IOFLAG_CAT | CSL_USB_IOFLAG_NOSHORT);
                            ///cfgDescPtr = cfgDescFS;
                            status = USB_postTransaction(pContext->hEpObjArray[1],
                                             9, pContext->cfgDescFSPtr,
                                             inOutFlag);
                        }
                        else
                        {
                             inOutFlag = (CSL_USB_IOFLAG_LNK | CSL_USB_IOFLAG_CAT);
                             tempLen = pContext->cfgDescFSPtr[1]&0xFF;
                             if(tempLen == pContext->usbSetup.wLength)
                             {
                                inOutFlag |= CSL_USB_IOFLAG_NOSHORT;
                            }

                             /* select the smaller of two */
                             tempLen = (tempLen < pContext->usbSetup.wLength) ? tempLen : pContext->usbSetup.wLength;
                            ///cfgDescPtr = cfgDescFS;
                            status = USB_postTransaction(pContext->hEpObjArray[1],
                                             tempLen, pContext->cfgDescFSPtr,
                                             inOutFlag);
                        }
                         break;

                    case CSL_USB_STRING_DESCRIPTOR_TYPE:
                        if((pContext->usbSetup.wValue & 0xFF) == 0x00)
                        {
                            ///strDescPtr = (Uint16 *)strDesc[0];
                            status = USB_postTransaction(pContext->hEpObjArray[1],
                                             pContext->strDescPtr[0][0]&0xFF, pContext->strDescPtr[0],
                                             CSL_USB_IN_TRANSFER);
                        }
                        if((pContext->usbSetup.wValue & 0xFF) == 0x01)
                        {
                            ///strDescPtr = (Uint16 *)strDesc[1];
                            status = USB_postTransaction(pContext->hEpObjArray[1],
                                             pContext->strDescPtr[1][0]&0xFF, pContext->strDescPtr[1],
                                             CSL_USB_IN_TRANSFER);
                        }
                        if((pContext->usbSetup.wValue & 0xFF) == 0x02)
                        {
                            ///strDescPtr = (Uint16 *)strDesc[2];
                            status = USB_postTransaction(pContext->hEpObjArray[1],
                                             pContext->strDescPtr[2][0]&0xFF, pContext->strDescPtr[2],
                                             CSL_USB_IN_TRANSFER);
                        }
                        if((pContext->usbSetup.wValue & 0xFF) == 0x03)
                        {
                            ///strDescPtr = (Uint16 *)strDesc[3];
                            status = USB_postTransaction(pContext->hEpObjArray[1],
                                             pContext->strDescPtr[3][0]&0xFF, pContext->strDescPtr[3],
                                             CSL_USB_IN_TRANSFER);
                        }
                        break;

                    case CSL_USB_DEVICE_QUAL_DESCRIPTOR_TYPE:
                        ///deviceQualDescPtr = (Uint16 *)deviceQualDesc;
                        tempLen = pContext->deviceQualDescPtr[0]&0xFF;
                        /* select the smaller of two  */
                        tempLen = (tempLen < pContext->usbSetup.wLength) ? tempLen : pContext->usbSetup.wLength;
                        status = USB_postTransaction(pContext->hEpObjArray[1],
                                         tempLen, pContext->deviceQualDescPtr,
                                         CSL_USB_IN_TRANSFER);
                        break;

                    default:
                        break;
                }

                // send ZLP
                USB_sendEpZLP(CSL_USB_EP0);

                break;

            case CSL_USB_SET_ADDRESS :
                pContext->devAddr = pContext->usbSetup.wValue;
                //USB_setDevAddr(CSL_USB0, usbSetup.wValue);
                USB_postTransaction(pContext->hEpObjArray[1], 0, NULL,
                                    CSL_USB_IOFLAG_NONE);

                // Service the RXPKTRDY after reading the FIFO
                USB_clearEpRxPktRdy(CSL_USB_EP0);

                break;

        case CSL_USB_GET_CONFIGURATION:
            /* Send the current Configuration Value */
            UsbCtrlBuffer[0] = curConfigureNum;
            USB_postTransaction(pContext->hEpObjArray[1], 1, (void*)UsbCtrlBuffer,
                                CSL_USB_IOFLAG_NONE | CSL_USB_IOFLAG_NOSHORT);
            // send ZLP
            USB_sendEpZLP(CSL_USB_EP0);

            break;

        case CSL_USB_GET_INTERFACE:
            /* Send the current Configuration Value */
            UsbCtrlBuffer[0] = pContext->usbSetup.wValue;
            USB_postTransaction(pContext->hEpObjArray[1], 1, (void*)UsbCtrlBuffer,
                                CSL_USB_IOFLAG_NONE | CSL_USB_IOFLAG_NOSHORT);
            // send ZLP
            USB_sendEpZLP(CSL_USB_EP0);

            break;

        case CSL_USB_GET_STATUS:

            switch(pContext->usbSetup.bmRequestType)
            {
                /* Device Status to be returned */
                case CSL_USB_REQUEST_TYPE_DEVICE_STATUS:
                    // return the remote wake up status and the self power status
                    UsbCtrlBuffer[0] =
                    (((Uint16)USB_getRemoteWakeupStat(CSL_USB0))<<1)|((pContext->cfgDescPtr[3]>>14)&0x0001);
                    USB_postTransaction(pContext->hEpObjArray[1], 2, UsbCtrlBuffer,
                                        CSL_USB_IOFLAG_NONE);
                    break;

                /* Interface status is to be returned */
                case CSL_USB_REQUEST_TYPE_INTERFACE_STATUS:
                    // return 0x0000 (reserved for future use)
                    UsbCtrlBuffer[0] = 0x0000;
                    USB_postTransaction(pContext->hEpObjArray[1], 2, UsbCtrlBuffer,
                                        CSL_USB_IOFLAG_NONE);
                    break;

                /* Endpoint status to be returned */
                case CSL_USB_REQUEST_TYPE_ENDPOINT_STATUS:
                    // get endpoint from setup packet
                    endpt = (pContext->usbSetup.wIndex) & 0xFF;
                    // get the endpoint handle
                    hEPx   =  USB_epNumToHandle(CSL_USB0, endpt);
                    // return the stall status
                    UsbCtrlBuffer[0] = (Uint16)USB_getEndptStall(hEPx, &status);
                    USB_postTransaction(pContext->hEpObjArray[1], 2, UsbCtrlBuffer, CSL_USB_IOFLAG_NONE);
                    break;

                default:
                    /* STALL the endpoint */
                    // get endpoint from setup packet
                    endpt = (pContext->usbSetup.wIndex) & 0xFF;
                    // get the endpoint handle
                    hEPx   =  USB_epNumToHandle(CSL_USB0, endpt);
                    // stall the control points
                    status = USB_stallEndpt(&pContext->ctrlOutEpObj);
                    status = USB_stallEndpt(&pContext->ctrlInEpObj);
                    break;
            }

            // send ZLP
            USB_sendEpZLP(CSL_USB_EP0);

            break;

        default:

            break;
        }
    }
    else
    {
        if(pContext->usbSetup.bRequest == 0x05)
        {
            USB_setDevAddr(CSL_USB0, pContext->devAddr);
        }
    }

    usbRegisters->INDEX_TESTMODE  = saveIndex;

    return(status);
}

