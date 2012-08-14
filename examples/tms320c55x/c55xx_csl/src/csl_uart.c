/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_uart.c
 *
 *  @brief UART functional layer API header file
 *
 *  Path: \(CSLPATH)\src
 */

/* ============================================================================
 * Revision History
 * ================
 * 8-Sep-2008 Created
 * ============================================================================
 */


#include "csl_uart.h"
#include "csl_uartAux.h"


/** ============================================================================
 *   @n@b UART_init
 *
 *   @b Description
 *   @n    This is the initialization function for the uart CSL. The function
 *      must be called before calling any other API from this CSL. This
 *      will initialize the uart object.
 *   @b Arguments
 *   @verbatim
        uartInstId  Instance number of the uart
        uartObj     Pointer to the uart object
        opmode      Operation mode of uart
     @endverbatim
 *
 *   <b> Return Value </b>    CSL_Status
 *   @li                CSL_SOK - Init call is successful
 *   @li                CSL_ESYS_INVPARAMS- Invalid parameter
 *   @li                CSL_UART_INVALID_INST_ID- Invalid instance id
 *   <b> Pre Condition </b>
 *   @n    None
 *   <b> Post Condition </b>
 *   @n    PLL object structure is populated
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
        CSL_Status              status;
        CSL_UartObj                uartObj;
        Uint32                  uartInstId;
        CSL_UartOpmode opmode = UART_POLLED;
        uartInstId = CSL_UART_INST_0;
        status = UART_init(&uartObj,uartInstId);
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_init(CSL_UartObj *  uartObj,Uint32 uartInstId,
                     CSL_UartOpmode opmode)
{
    CSL_Status status = CSL_SOK;

    if(NULL == uartObj)
    {
        return CSL_ESYS_INVPARAMS;
    }

    if(opmode ==  UART_OPMODE_OTHER)
    {
        return CSL_ESYS_INVPARAMS;
    }
       /* Set the base address  */
    switch ( uartInstId)
    {
        case CSL_UART_INST_0:
                   uartObj->insId = uartInstId;
                uartObj->sysAddr = CSL_SYSCTRL_REGS;
                uartObj->uartRegs = CSL_UART_REGS;
                uartObj->opmode = opmode;
                break;
        default:
                uartObj->insId = CSL_UART_INST_INVALID;
                status = CSL_EUART_INVALID_INST_ID;
                break;
    }
    if(CSL_UART_INST_INVALID != uartObj->insId)
    {
        /**Enable Master clock                                     */
        CSL_FINST(uartObj->sysAddr->PCGCR1,SYS_PCGCR1_SYSCLKDIS,ACTIVE);
        /**Enable uart peripheral clock gating                */
        CSL_FINST(uartObj->sysAddr->PCGCR1,SYS_PCGCR1_UARTCG,ACTIVE);
        /* changing parallel port mode to 5 - supports UART         */
        CSL_FINST(uartObj->sysAddr->EBSR,SYS_EBSR_PPMODE,MODE5);
        /**Resetting UART  module*/
        CSL_FINST(uartObj->sysAddr->PRCR, SYS_PRCR_PG4_RST,RST);

    }
    return status;
}


/** ============================================================================
 *   @n@b UART_setup
 *
 *   @b Description
 *   @n    The main function of this API is to setup the
 *        baud rate,configure fifo,enable transmitter and receiver UART
 *      setup word size,stop bits and parity.
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
        uartSetup     Pointer to the CSL_UartSetup structure.
     @endverbatim
 *   <b> Return Value </b>    CSL_Status
 *   @li                CSL_SOK - UART_setup call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid Handle
 *   @li                CSL_ESYS_INVPARAMS- Invalid uartSetup
 *   <b> Pre Condition </b>
 *   @n    UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n    Setup the UART for transmission and reception.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
        CSL_Status       status;
         CSL_UartObj         uartObj;
        Uint32           uartInstId;
        CSL_UartSetup          uartSetup;
        uartInstId = 0;

        status = UART_init(&uartObj,uartInstId);
        uartSetup.afeEnable = CSL_UART_NO_AFE;
        uartSetup.baud = 2400;
        uartSetup.clkInput = 60000000;
        uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
        uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
        uartSetup.parity =  CSL_UART_DISABLE_PARITY;
        uartSetup.rtsEnable = CSL_UART_NO_RTS;
        uartSetup.stopBits = 0;
        uartSetup.wordLength = CSL_UART_WORD8;

        status =  UART_setup(CSL_UartHandle hUart,&uartSetup);
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_setup(CSL_UartHandle hUart, CSL_UartSetup *uartSetup)
{

     CSL_Status status =CSL_SOK;
     Uint32 value;
     Uint32 clk_div;

     if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }
     if( NULL == uartSetup)
     {
         return CSL_ESYS_INVPARAMS;
     }
     /**Putting UART recvor and xmittor in reset state*/
    hUart->uartRegs->PWREMU_MGMT = CSL_UART_PWREMU_MGMT_RESETVAL;
    /* Write 0 to DLAB to access IER*/
    CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABON);
    /*Disable all interrupts*/
    hUart->uartRegs->IER = CSL_UART_IER_RESETVAL;
    /* Flushing buffer */
    value = (Uint32)(hUart->uartRegs->RBR);
        /* reset and possibly enable FIFOs */
    CSL_FINST(hUart->uartRegs->FCR, UART_FCR_FIFOEN, ENABLE);
    CSL_FINST(hUart->uartRegs->FCR, UART_FCR_RXCLR, CLR);
    CSL_FINST(hUart->uartRegs->FCR, UART_FCR_TXCLR, CLR);
    hUart->uartRegs->FCR = uartSetup->fifoControl;
    hUart->trigLevel =  CSL_FEXT(uartSetup->fifoControl,UART_FCR_RXFIFTL);
    clk_div = CSL_UART_CLK_DIVIDER(uartSetup->baud);

    /* Set DLL and DLM to values appropriate for the required baudrate */
    value = ( (Uint32) (uartSetup->clkInput) / (clk_div ) );

    /* Write 1 to DLAB to access DLL and DLH*/
    CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABOFF);

    if ( ( (Uint32)(uartSetup->clkInput) % ( clk_div ) )
                                >=   ( clk_div / 2 ) )
    {
        if ((value & 0xFF)  != 0xFF)
        {
            hUart->uartRegs->DLL = (Uint8)(value & 0xFF) + 1;
            hUart->uartRegs->DLH = (Uint8)((value & 0xFF00) >> 8);
        }
        else
        {
            hUart->uartRegs->DLL = (Uint8) ( 0x00 );
            hUart->uartRegs->DLH = (Uint8) ( ( ( value & 0xFF00 ) >> 8 ) + 0x1u );
        }

    }
    else
    {
        hUart->uartRegs->DLL = (Uint8)(value & 0xFF) ;
           hUart->uartRegs->DLH = (Uint8)((value & 0xFF00) >> 8);
    }

    CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB,DLABON);
    /*setup word size*/
    switch(uartSetup->wordLength)
    {
        case CSL_UART_WORD5:
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_WLS,BITS5);
            break;
        case CSL_UART_WORD6:
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_WLS,BITS6);
            break;
        case CSL_UART_WORD7:
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_WLS,BITS7);
            break;
        case CSL_UART_WORD8:
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_WLS,BITS8);
            break;
        default:
            return CSL_EUART_INVALID_WORDLENGTH;

    }

    /*  Stop bits generation */
    CSL_FINS(hUart->uartRegs->LCR,UART_LCR_STB,uartSetup->stopBits);

    /* Parity selection */
    switch(uartSetup->parity)
    {
        case CSL_UART_DISABLE_PARITY:
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_PEN,DISABLE);
            break;
        case CSL_UART_ODD_PARITY:
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_PEN,ENABLE);
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_EPS,ODD);
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_SP,DISABLE);
            break;
        case CSL_UART_EVEN_PARITY:
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_PEN,ENABLE);
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_EPS,EVEN);
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_SP,DISABLE);
            break;
        case CSL_UART_MARK_PARITY:
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_PEN,ENABLE);
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_EPS,ODD);
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_SP,ENABLE);
            break;
        case CSL_UART_SPACE_PARITY:
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_PEN,ENABLE);
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_EPS,EVEN);
            CSL_FINST(hUart->uartRegs->LCR,UART_LCR_SP,ENABLE);
            break;
        default:
            return CSL_ESYS_INVPARAMS;
    }

    /* setup loopback control */
    if(uartSetup->loopBackEnable)
    {
        CSL_FINST(hUart->uartRegs->MCR,UART_MCR_LOOP,ENABLE);
    }
    else
    {
        CSL_FINST(hUart->uartRegs->MCR,UART_MCR_LOOP,DISABLE);

    }
    if(uartSetup->afeEnable)
    {
        CSL_FINST(hUart->uartRegs->MCR,UART_MCR_AFE,ENABLE);
    }
    else
    {
        CSL_FINST(hUart->uartRegs->MCR,UART_MCR_AFE,DISABLE);
    }
    if(uartSetup->rtsEnable)
    {
        CSL_FINST(hUart->uartRegs->MCR,UART_MCR_RTS,CTSRTSEN);
    }
    else
    {
        CSL_FINST(hUart->uartRegs->MCR,UART_MCR_RTS,CTSEN);
    }

    /* UART Receiver enabled */
    CSL_FINST(hUart->uartRegs->PWREMU_MGMT,UART_PWREMU_MGMT_URRST,ENABLE);
    /* UART Transmitter enabled */
    CSL_FINST(hUart->uartRegs->PWREMU_MGMT,UART_PWREMU_MGMT_UTRST,ENABLE);

    CSL_FINST(hUart->uartRegs->PWREMU_MGMT,UART_PWREMU_MGMT_FREE,RUN);
    return status;
}

/** ============================================================================
 *   @n@b UART_config
 *
 *   @b Description
 *   @n    This is a low level API which is used to confgiure registers
 *      DLL,DLH,FCR,MCR,LCR.
 *   @b Arguments
 *   @verbatim
         hUart  Handle to the UART.
        pConfig     Pointer to the CSL_UartConfig structure.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK - UART_config call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   @li                CSL_ESYS_INVPARAMS- Invalid pointer to UART Config strucutre.
 *   <b> Pre Condition </b>
 *   @n  UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n  Registers are configured.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
        CSL_Status              status;
         CSL_UartObj                uartObj;
        CSL_UartHandle             hUart;
        Uint32                  uartInstId;
        CSL_UartConfig             Config;
        CSL_UartSetup              uartSetup;
        uartInstId  = 0;

        Config.FCR = CSL_UART_FIFO_DMA1_DISABLE_TRIG01;
        Config.DLL = 0x001b;
        Config.DLH = 0x0006;
        Config.LCR = 0x0003
        Config.MCR = 0x0000;

        status = UART_init(&uartObj,uartInstId);
        hUart = (CSL_UartHandle)&uartObj);
        status = UART_reset(hUart);
        status = UART_config(hUart,&Config);
        status = UART_resetOff(hUart);
     @endverbatim
 *  ============================================================================
 */

 CSL_Status UART_config(CSL_UartHandle hUart,CSL_UartConfig *uartConfig)
 {

    CSL_Status status = CSL_SOK;


    if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }

     if( NULL == uartConfig)
     {
         return CSL_ESYS_INVPARAMS;
     }
    /* Write 1 to DLAB to access DLL and DLH*/
     hUart->uartRegs->PWREMU_MGMT = CSL_UART_PWREMU_MGMT_RESETVAL;
     CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABOFF);
     hUart->uartRegs->DLL = uartConfig->DLL;
     hUart->uartRegs->DLH = uartConfig->DLH;
     hUart->uartRegs->LCR = uartConfig->LCR;
     CSL_FINST(hUart->uartRegs->FCR, UART_FCR_FIFOEN, ENABLE);
     CSL_FINST(hUart->uartRegs->FCR, UART_FCR_RXCLR, CLR);
     CSL_FINST(hUart->uartRegs->FCR, UART_FCR_TXCLR, CLR);
     hUart->uartRegs->FCR = uartConfig->FCR;
     hUart->trigLevel =  CSL_FEXT(uartConfig->IIR,UART_FCR_RXFIFTL);
     hUart->uartRegs->MCR = uartConfig->MCR;
     /*clear IER first to make sure UART work properly*/
     CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABON);

    return status;
}

/** ============================================================================
 *   @n@b UART_setupBaudRate
 *
 *   @b Description
 *   @n    This API is used to set the baud rate.
 *
 *   @b Arguments
 *   @verbatim
        hUart       Handle to the UART
        clkInput    Input clock frequency in Hz
        baudRate    Baud rate.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK - UART_setupBaudRate call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   <b> Pre Condition </b>
 *   @n  UART_init and UART_setup should be called successfully.
 *   <b> Post Condition </b>
 *   @n   DLL and DLH registers are set to clkdivisor value.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
        CSL_Status              status;
         CSL_UartObj                uartObj;
        CSL_UartHandle             hUart;
        Uint32                  uartInstId;
        CSL_UartSetup              uartSetup;
        uartInstId = 0;

        uartSetup.afeEnable = CSL_UART_NO_AFE;
        uartSetup.baud = 2400;
        uartSetup.clkInput = 60000000;
        uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
        uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
        uartSetup.parity =  CSL_UART_DISABLE_PARITY;
        uartSetup.rtsEnable = CSL_UART_NO_RTS;
        uartSetup.stopBits = 0;
        uartSetup.wordLength = CSL_UART_WORD8;

        status = UART_init(&uartObj,uartInstId);
        hUart  = (CSL_UartHandle)&uartObj;
        status = UART_setup(hUart,&uartSetup);
        status = UART_setupBaudRate(hUart,60000000,4800);
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_setupBaudRate(CSL_UartHandle hUart,Uint32 clkInput,Uint32 baudRate)
{
    Uint32 clk_div = 0;
    Uint32 value = 0;
    CSL_Status status = CSL_SOK;

     if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }

    /* Write 1 to DLAB to access DLL and DLH*/
    CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABOFF);
    clk_div = CSL_UART_CLK_DIVIDER(baudRate);

    /* Set DLL and DLM to values appropriate for the required baudrate */
    value = ( (Uint32) (clkInput) / (clk_div ) );

    if ( ( (Uint32) ( clkInput )% ( clk_div ) )
                                >=   ( clk_div / 2 ) )

    {
        if ((value & 0xFF)  != 0xFF)
        {
            hUart->uartRegs->DLL = (Uint8)(value & 0xFF) + 1;
            hUart->uartRegs->DLH = (Uint8)((value & 0xFF00) >> 8);
        }
        else
        {
            hUart->uartRegs->DLL = (Uint8) ( 0x00 );
            hUart->uartRegs->DLH = (Uint8) ( ( ( value & 0xFF00 ) >> 8 ) + 0x1u );
        }

    }
    else
    {
        hUart->uartRegs->DLL = (Uint8)(value & 0xFF) ;
           hUart->uartRegs->DLH = (Uint8)((value & 0xFF00) >> 8);
    }

       CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABON);

    return status;
}

/** ============================================================================
 *   @n@b UART_reset
 *
 *   @b Description
 *   @n    This API is used to reset the UART receiver and transmitter.
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK - UART_setupBaudRate call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   <b> Pre Condition </b>
 *   @n  UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n  uart receiver and transmitter are in reset state.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
          CSL_Status              status;
          CSL_UartObj                uartObj;
         CSL_UartHandle             hUart;
         CSL_UartConfig             Config;
         Uint32                  uartInstId;
         Bool                    brecvstatus;
         Bool                    bxmitstatus;
         uartInstId = 0;
         Config.FCR = CSL_UART_FIFO_DMA1_DISABLE_TRIG01;
         Config.DLL = 0x001b;
         Config.DLH = 0x0006;
         Config.LCR = 0x0003
         Config.MCR = 0x0000;
         status = UART_init(&uartObj,uartInstId);
         hUart  = CSL_UartHandle(&uartObj);
         status = UART_reset(hUart);
         status = UART_config(hUart,&Config);
         status = UART_resetOff(hUart);
         brecvstatus = UART_getRecvorResetStatus(hUart);
         bxmitstatus = UART_getXmitorResetState(hUart);

     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_reset(CSL_UartHandle hUart)
{
    volatile Uint32 cnt;
    CSL_Status status = CSL_SOK;

     if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }

    /* UART Receiver in reset state */
     CSL_FINST(hUart->uartRegs->PWREMU_MGMT,UART_PWREMU_MGMT_URRST,RESET);
    /* UART Transmitter in reset state */
     CSL_FINST(hUart->uartRegs->PWREMU_MGMT,UART_PWREMU_MGMT_UTRST,RESET);

     return status;

}

/** ============================================================================
 *   @n@b UART_resetOff
 *
 *   @b Description
 *   @n    This API is used to set the UART receiver and transmitter.
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK - UART_setupBaudRate call is successful
 *   @li                         CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   <b> Pre Condition </b>
 *   @n  UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n   uart receiver and transmitter are on state.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
        CSL_Status              status;
          CSL_UartObj                uartObj;
         CSL_UartHandle             hUart;
         Uint32                  uartInstId;
         Bool                    brecvstatus;
         Bool                    bxmitstatus;
         uartInstId = 0;
         config.FCR = CSL_UART_FIFO_DMA1_DISABLE_TRIG01;
         config.DLL = 0x001b;
         config.DLH = 0x0006;
         config.LCR = 0x0003
         config.MCR = 0x0000;
         status = UART_init(&uartObj,uartInstId);
         hUart  = CSL_UartHandle(&uartObj);
         status = UART_reset(hUart);
         status = UART_config(hUart,&Config);
         status = UART_resetOff(hUart);
         brecvstatus = UART_getRecvorResetStatus(hUart);
         bxmitstatus = UART_getXmitorResetState(hUart);
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_resetOff(CSL_UartHandle hUart)
{
    CSL_Status status = CSL_SOK;

     if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }

    /* UART Receiver out of reset */
    CSL_FINST(hUart->uartRegs->PWREMU_MGMT,UART_PWREMU_MGMT_URRST,ENABLE);
    /* UART Transmitter out of reset */
    CSL_FINST(hUart->uartRegs->PWREMU_MGMT,UART_PWREMU_MGMT_UTRST,ENABLE);
    /*I need more clarifications on the 1st bit of the power and emulation management register
       After that i have to code corresponding to that.
    */
    return status;

}
/** ============================================================================
 *   @n@b UART_eventEnable
 *
 *   @b Description
 *   @n    This API is used to enable events like Receiver data availabe,
 *        Transmitter holding register empty
 *
 *   @b Arguments
 *   @verbatim
         hUart  Handle to the UART.
         isrMask Interested particular events can be enabled by setting the
        correspoding bits in the variable
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_eventEnable call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   <b> Pre Condition </b>
 *   @n  UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n The corresponding events are enabled.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
         CSL_Status              status;
         CSL_UartObj                uartObj;
           CSL_UartSetup                 uartSetup;
         Uint16 isrMask = 0x01;
         Uint32 uartInstId = 0x0;
        uartSetup.afeEnable = CSL_UART_NO_AFE;
        uartSetup.baud = 2400;
        uartSetup.clkInput = 60000000;
        uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
        uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
        uartSetup.parity =  CSL_UART_DISABLE_PARITY;
        uartSetup.rtsEnable = CSL_UART_NO_RTS;
        uartSetup.stopBits = 0;
        uartSetup.wordLength = CSL_UART_WORD8;
        status = UART_init(&uartObj,uartInstId);
        status = UART_setup( &uartObj,&uartSetup);
        status = UART_eventEnable(&uartObj,CSL_UART_RECVOR_REG_DATA_INTERRUPT)
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_eventEnable(CSL_UartHandle hUart, CSL_UartEventType uartEvent)
{

    CSL_Status status = CSL_SOK;

     if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }

     CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABON);

     switch (uartEvent)
     {
         case CSL_UART_RECVOR_REG_DATA_INTERRUPT:
         {

             CSL_FINST(hUart->uartRegs->IER, UART_IER_ERBI,
                                                     ENABLE);
             break;
         }

         case CSL_UART_XMITOR_REG_EMPTY_INTERRUPT:
         {
               CSL_FINST(hUart->uartRegs->IER,UART_IER_ETBEI,
                                                     ENABLE);
             break;
         }

         case CSL_UART_RECVOR_LINE_STATUS_INTERRUPT:
         {

             CSL_FINST(hUart->uartRegs->IER,UART_IER_ELSI,
                                                     ENABLE);
             break;
         }

        default:
        {
            return CSL_ESYS_INVPARAMS;
        }
     }

    return status;
}


/** ============================================================================
 *   @n@b UART_eventDisable
 *
 *   @b Description
 *   @n This API is used to enable events like Receiver data availabe,
 *        Transmitter holding register empty
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
        isrMask Particular events can be disabled by setting correspoding
           bits in the variable
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_eventDisable call is successful
 *   @li                CSL_ESYS_BADHANDLE - Invalid UART Handle
 *   <b> Pre Condition </b>
 *   @n  UART_init should be called successfully.
 *   <b> Post Condition </b>
 *   @n  The correspoding events are  disabled.
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
          CSL_Status              status;
          CSL_UartObj                uartObj;
         Uint32                  uartInstId;
         CSL_UartSetup              uartSetup;
         Uint16 isrMask = 0x01;
         uartInstId = 0;

         uartSetup.afeEnable = CSL_UART_NO_AFE;
         uartSetup.baud = 2400;
         uartSetup.clkInput = 60000000;
         uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
         uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
         uartSetup.parity =  CSL_UART_DISABLE_PARITY;
         uartSetup.rtsEnable = CSL_UART_NO_RTS;
         uartSetup.stopBits = 0;
         uartSetup.wordLength = CSL_UART_WORD8;

         status = UART_init(&uartObj,uartInstId);
         status = UART_eventDisable(&uartObj,CSL_UART_RECVOR_REG_DATA_INTERRUPT);
         status = UART_setup( hUart,&uartSetup);
         status = UART_eventEnable(hUart,CSL_UART_XMITOR_REG_EMPTYINTERRUPT);

     @endverbatim
 *  ============================================================================
 */


CSL_Status  UART_eventDisable(CSL_UartHandle hUart,CSL_UartEventType uartEvent)
{
    CSL_Status status = CSL_SOK;


     if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }

    CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABON);


    switch (uartEvent)
    {
        case CSL_UART_RECVOR_REG_DATA_INTERRUPT:
        {
            CSL_FINST(hUart->uartRegs->IER, UART_IER_ERBI,
                                                DISABLE);
            break;
        }

        case CSL_UART_XMITOR_REG_EMPTY_INTERRUPT:
        {
            CSL_FINST(hUart->uartRegs->IER,UART_IER_ETBEI,
                                                  DISABLE);
            break;
        }

        case CSL_UART_RECVOR_LINE_STATUS_INTERRUPT:
        {

            CSL_FINST(hUart->uartRegs->IER,UART_IER_ELSI,
                                                 DISABLE);
            break;
        }

        default:
        {
            return CSL_ESYS_INVPARAMS;
        }
    }

    return status;
}
/** ============================================================================
 *   @n@b UART_read
 *
 *   @b Description
 *   @n UART_read attempts to read up to count characters
 *        from RBR(Recieve Buffer Register) into the buffer starting at pBuf
 *      in polled mode.
 *      In interrupt mode,only charater it will read .
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
           pBuf   Output buffer
          count  No of characters to read.
        timeout Timeout value
                0 - wait indefinitely for DR bit to get set.
                x - wait x value for DR bit to get set.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li          CSL_SOK -  UART_read call is successful.
 *   @li          CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   @li          CSL_ESYS_INVPARAMS- Invalid address of the  buffer.
 *   @li          CSL_EUART_TIMEOUT - Read had a timeout due to DR bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and UART_setup should be called successfully.
 *   <b> Post Condition </b>
 *   @n  pBuf contains count number of characters.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @n bBuf is modified
 *   @b Example
 *   @verbatim
          CSL_Status              status;
          CSL_UartObj                uartObj;
         Uint32                  uartInstId;
         CSL_UartSetup              uartSetup;
         CSL_UartHandle hUart;
         Char  buf[12];
         Uint16 N = 12;
         uartInstId = 0;

         uartSetup.afeEnable = CSL_UART_NO_AFE;
         uartSetup.baud = 2400;
         uartSetup.clkInput = 60000000;
         uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
         uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
         uartSetup.parity =  CSL_UART_DISABLE_PARITY;
         uartSetup.rtsEnable = CSL_UART_NO_RTS;
         uartSetup.stopBits = 0;
         uartSetup.wordLength = CSL_UART_WORD8;

         status = UART_init(&uartObj,uartInstId);
         hUart = (CSL_UartHandle)&uartObj;
         status = UART_setup( hUart,&uartSetup);
         status = UART_read(hUart,buf,N,1000) ;
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_read(CSL_UartHandle    hUart,
                     Char           *pBuf,
                     Uint16         count,
                     Uint32         timeout)
{

    Uint32 TimeOut  = timeout;
    CSL_Status status = CSL_SOK;

    if( NULL == hUart )
    {
         return CSL_ESYS_BADHANDLE;
    }

    if( NULL == pBuf )
    {

        return CSL_ESYS_INVPARAMS;

    }

       CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABON);

    if(UART_POLLED == hUart->opmode)
    {
        if(0 != timeout)
        {
            while( 0 != ( count--) )
            {
                while( ( !UART_getDataReadyStatus( hUart) ) && --TimeOut);
                if ( 0 == TimeOut )
                    return CSL_EUART_TIMEOUT;
                *pBuf++ = CSL_FEXT(hUart->uartRegs->RBR,UART_RBR_DATA);
                TimeOut = timeout;
            }
        }
        else
        {
            while( 0 != ( count--) )
            {
                while( !UART_getDataReadyStatus( hUart) );
                *pBuf++ = CSL_FEXT(hUart->uartRegs->RBR,UART_RBR_DATA);
            }

        }

        return status;
    }

    *pBuf++ = CSL_FEXT(hUart->uartRegs->RBR,UART_RBR_DATA);
    return status;
}
/** ============================================================================
 *   @n@b UART_write
 *
 *   @b Description
 *   @n UART_write writes up to count characters to the THR(Transmitter Holding Register)
 *        from the buffer starting at pBuf
 *        In interrupt mode maximum of 16 charaters it will write
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
        pBuf   Input buffer
        count  No of characters to write.
        timeout Timeout value
                0 - wait indefinitely for THRE bit to get set.
                x - wait x value for THRE bit to get set.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li               CSL_SOK -  UART_write call is successful
 *   @li               CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   @li               CSL_ESYS_INVPARAMS- Invalid address of the pBuf
 *   @li               CSL_EUART_TIMEOUT - Write had a timeout due to THRE bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and UART_setup should be called successfully.
 *   <b> Post Condition </b>
 *   @n  count number of characters are written to THR.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @b Example
 *   @verbatim
              CSL_Status              status;
              CSL_UartObj                uartObj;
            Uint32                  uartInstId;
            CSL_UartSetup                 uartSetup;
            CSL_UartHandle             hUart;
            Char                    buf[12];
            Uint16 N = 12;
            uartInstId = 0;

            uartSetup.afeEnable = CSL_UART_NO_AFE;
            uartSetup.baud = 2400;
            uartSetup.clkInput = 60000000;
            uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
            uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
            uartSetup.parity =  CSL_UART_DISABLE_PARITY;
            uartSetup.rtsEnable = CSL_UART_NO_RTS;
            uartSetup.stopBits = 0;
            uartSetup.wordLength = CSL_UART_WORD8;

            status = UART_init(&uartObj,uartInstId);
            hUart = (CSL_UartHandle)&uartObj;
            status =  UART_setup( hUart,&uartSetup);
            status = UART_write(hUart,buf,N,1000) ;
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_write( CSL_UartHandle hUart,
                       Char *pBuf, Uint16 count, Uint32 timeout)
{
    Uint16 fifosize,fifoMode ;
    Uint32 TimeOut  = timeout;
    CSL_Status status = CSL_SOK;

    if( NULL == hUart )
    {
        return CSL_ESYS_BADHANDLE;
    }

    if( NULL == pBuf )
    {
        return CSL_ESYS_INVPARAMS;
    }

    CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABON);

    if(UART_POLLED == hUart->opmode)
    {
        if( 0 != timeout )
        {
            while( 0 != ( count--) )
            {
                while( ( !UART_getXmitHoldRegEmptyStatus(hUart) ) && --TimeOut);
                if ( 0 == TimeOut )
                    return CSL_EUART_TIMEOUT;
                CSL_FSET(hUart->uartRegs->THR, 7, 0, (*pBuf)&CSL_UART_THR_DATA_MASK);
                pBuf++;
                TimeOut  = timeout;
            }
        }
        else
        {
            while( 0 != ( count--) )
            {
                while(!UART_getXmitHoldRegEmptyStatus(hUart));
                CSL_FSET(hUart->uartRegs->THR, 7, 0, (*pBuf)&CSL_UART_THR_DATA_MASK);
                pBuf++;

            }
        }

        return status;
    }

    fifoMode =  UART_getModeOfTransfer(hUart);
    if(CSL_UART_NON_FIFO_MODE == fifoMode)/*Non fifo mode*/
    {
        CSL_FSET(hUart->uartRegs->THR, 7, 0, (*pBuf)&CSL_UART_THR_DATA_MASK);
    }
    else if (CSL_UART_FIFO_MODE == fifoMode) /*Fifo mode*/
    {
        fifosize = ( count < 16 ) ? count : 16;
        while( 0 != ( fifosize--) )
        {
             CSL_FSET(hUart->uartRegs->THR, 7, 0, (*pBuf)&CSL_UART_THR_DATA_MASK);
              pBuf++;
        }
    }
    return status;
}

/** ============================================================================
 *   @n@b UART_fgetc
 *
 *   @b Description
 *   @n UART_fgetc reads a single character from the RBR into elem.
 *
 *   @b Arguments
 *   @verbatim
         hUart  Handle to the UART.
        elem   A size one buffer.
        timeout Timeout value
                0 - wait indefinitely for DR bit to get set.
                x - wait x value for DR bit to get set.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_fgetc call is successful
 *   @li                CSL_ESYS_BADHANDLE- Invalid UART Handle
 *   @li                CSL_ESYS_INVPARAMS- Invalid address of input buffer
 *   @li                CSL_EUART_TIMEOUT -  had a timeout due to DR bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and UART_setup should be called successfully.
 *   <b> Post Condition </b>
 *   @n elem will have a single character.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @n elem is modified
 *   @b Example
 *   @verbatim
              CSL_Status              status;
              CSL_UartObj                uartObj;
             Uint32                      uartInstId;
            CSL_UartSetup          uartSetup;
            CSL_UartHandle hUart;
            Char  buf[1];
            uartInstId = 0;

            uartSetup.afeEnable = CSL_UART_NO_AFE;
            uartSetup.baud = 2400;
            uartSetup.clkInput = 60000000;
            uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
            uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
            uartSetup.parity =  CSL_UART_DISABLE_PARITY;
            uartSetup.rtsEnable = CSL_UART_NO_RTS;
            uartSetup.stopBits = 0;
            uartSetup.wordLength = CSL_UART_WORD8;

            status = UART_init(&uartObj,uartInstId);
            hUart = (CSL_UartHandle)&uartObj;
            status =  UART_setup( hUart,&uartSetup);
            status = UART_fgetc(hUart,buf,200)
     @endverbatim
 *  ============================================================================
 */
CSL_Status UART_fgetc(CSL_UartHandle hUart, Char *elem, Uint32 timeout)
{
    CSL_Status status = CSL_SOK;

    if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }

    if( NULL == elem )
     {
        return CSL_ESYS_INVPARAMS;
     }

    CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABON);

    if(0 != timeout)
    {
        while( ( !UART_getDataReadyStatus( hUart) ) && --timeout );
        if ( 0 == timeout )
            return CSL_EUART_TIMEOUT;
        *elem = CSL_FEXT(hUart->uartRegs->RBR,UART_RBR_DATA);
    }
    else
    {
        while( !UART_getDataReadyStatus( hUart) );
        *elem = CSL_FEXT(hUart->uartRegs->RBR,UART_RBR_DATA);
    }

    return status;
}


/** ============================================================================
 *   @n@b UART_fputc
 *
 *   @b Description
 *   @n UART_fputc writes a single character to THR(Transmitter Holding Register).
 *
 *   @b Arguments
 *   @verbatim
        hUart  Handle to the UART.
        elem   Character to transmit
        timeout Timeout value
                0 - wait indefinitely for THRE bit to get set.
                x - wait x value for THRE bit to get set.
     @endverbatim
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_fputc call is successful
 *   @li                CSL_ESYS_BADHANDLE - Handle is invalid
 *   @li                CSL_EUART_TIMEOUT - had a timeout due to THRE bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and UART_setup should be called successfully.
 *   <b> Post Condition </b>
 *   @n A character will get transmitted.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @b Example
 *   @verbatim
              CSL_Status              status;
              CSL_UartObj                uartObj;
            Uint32                  uartInstId;
            CSL_UartSetup                 uartSetup;
            CSL_UartHandle hUart;
            Char  buf[1];
            uartInstId = 0;

            uartSetup.afeEnable = CSL_UART_NO_AFE;
            uartSetup.baud = 2400;
            uartSetup.clkInput = 60000000;
            uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
            uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
            uartSetup.parity =  CSL_UART_DISABLE_PARITY;
            uartSetup.rtsEnable = CSL_UART_NO_RTS;
            uartSetup.stopBits = 0;
            uartSetup.wordLength = CSL_UART_WORD8;

            status = UART_init(&uartObj,uartInstId);
            hUart = (CSL_UartHandle)&uartObj;
            status = UART_setup( hUart,&uartSetup);
            status = UART_fputc(hUart,buf[0],200)

     @endverbatim
 *  ============================================================================
 */
CSL_Status UART_fputc(CSL_UartHandle hUart, const Char elem, Uint32 timeout)
{
    CSL_Status status = CSL_SOK;

    if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }

    CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABON);

    if(0 != timeout)
    {
        while( ( !UART_getXmitHoldRegEmptyStatus(hUart) ) && --timeout );
        if (0 == timeout)
            return CSL_EUART_TIMEOUT;
        CSL_FSET(hUart->uartRegs->THR, 7, 0, elem&CSL_UART_THR_DATA_MASK);
    }
    else
    {
        while(!UART_getXmitHoldRegEmptyStatus(hUart));
        CSL_FSET(hUart->uartRegs->THR, 7, 0, elem&CSL_UART_THR_DATA_MASK);
    }

    return status;
}
/** ============================================================================
 *   @n@b UART_fgets
 *
 *   @b Description
 *   @n UART_fgets reads in at most one less than bufSize characters from RBR and stores them
 *      into the buffer pointed to by pBuf.Reading stops after a newline. If a newline is read, it is
 *      stored into the buffer. A '\0' is stored after the last character in the buffer.
 *   @b Arguments
 *   @verbatim
            hUart  Handle to the UART.
                pBuf   Output buffer
            bufSize  Size of output buffer
            timeout Timeout value
                    0 - wait indefinitely for DR bit to get set.
                    x - wait x value for DR bit to get set.
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_fgets call is successful
 *   @li                CSL_ESYS_BADHANDLE -  Invalid handle.
 *   @li                CSL_ESYS_INVPARAMS- Invalid output buffer
 *   @li                CSL_EUART_INVALID_LENGTH- Invalid string length.
 *   @li                CSL_EUART_TIMEOUT - had a timeout due to DR bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and UART_setup should be called successfully.
 *   <b> Post Condition </b>
 *   @n  Input buffer will have a string.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @n pBuf is modified
 *   @b Example
 *   @verbatim
             CSL_Status              status;
             CSL_UartObj                uartObj;
            Uint32                  uartInstId;
            CSL_UartSetup                 uartSetup;
            CSL_UartHandle hUart;
            Char  buf[12];
            Uint16 N = 12;
            uartInstId = 0;

            uartSetup.afeEnable = CSL_UART_NO_AFE;
            uartSetup.baud = 2400;
            uartSetup.clkInput = 60000000;
            uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
            uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
            uartSetup.parity =  CSL_UART_DISABLE_PARITY;
            uartSetup.rtsEnable = CSL_UART_NO_RTS;
            uartSetup.stopBits = 0;
            uartSetup.wordLength = CSL_UART_WORD8;

            status = UART_init(&uartObj,uartInstId);
            hUart = (CSL_UartHandle)&uartObj;
            status =  CSL_UartSetup( hUart,&uartSetup);
            status = UART_fgets(hUart,buf,N,100) ;

     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_fgets(CSL_UartHandle hUart,
                      Char * pBuf, Uint16 bufSize, Uint32 timeout)
{
    Uint32 TimeOut  = timeout;
    Uint16 count = bufSize - 1;
    CSL_Status status = CSL_SOK;

    if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }

    if( NULL == pBuf )
     {
        return CSL_ESYS_INVPARAMS;
     }

    if( 0 == bufSize )
    {
        return CSL_EUART_INVALID_BUFSIZE;
    }

    CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABON);
    if( 0 != timeout )
    {
        while (count--)
        {
            while( ( !UART_getDataReadyStatus( hUart) ) && TimeOut-- );
            if ( 0 == TimeOut )
                  return CSL_EUART_TIMEOUT;
            *pBuf = CSL_FEXT(hUart->uartRegs->RBR,UART_RBR_DATA);
            if ( '\n' == *pBuf )
            {
                pBuf++;
                break;
            }
            pBuf++;
            TimeOut  = timeout;
        }
        *pBuf = '\0';
    }
    else
    {
        while (count--)
        {
            while(!UART_getDataReadyStatus( hUart));
            *pBuf = CSL_FEXT(hUart->uartRegs->RBR,UART_RBR_DATA);
            if( '\n' == *pBuf )
            {
                pBuf++;
                break;
            }
            pBuf++;
        }
        *pBuf = '\0';
    }
    return status;
}

/** ============================================================================
 *   @n@b UART_fputs
 *
 *   @b Description
 *   @n UART_fputs writes the string pointed by pBuf to THR, without its trailing '\0'.
 *
 *   @b Arguments
 *   @verbatim

        hUart  Handle to the UART.
        pBuf   Input buffer
        timeout Timeout value
                0 - wait indefinitely for THRE bit to get set.
                x - wait x value for THRE bit to get set.

      @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                CSL_SOK -  UART_fputs call is successful
 *   @li                CSL_ESYS_BADHANDLE -  UART_fputs call is successful
 *   @li                CSL_ESYS_INVPARAMS - Invalid input buffer
 *   @li                CSL_EUART_TIMEOUT   - had a timeout due to THRE bit not set.
 *   <b> Pre Condition </b>
 *   @n  UART_init and UART_setup should be called successfully.
 *   <b> Post Condition </b>
 *   @n  A string will get transmitted.
 *
 *   @b Modifies
 *   @n Handle is modified
 *   @b Example
 *   @verbatim
              CSL_Status              status;
              CSL_UartObj                uartObj;
            Uint32                  uartInstId;
            CSL_UartSetup                 uartSetup;
            CSL_UartHandle             hUart;
            Char  buf[12] ="Test";
            uartInstId = 0;

            uartSetup.afeEnable = CSL_UART_NO_AFE;
            uartSetup.baud = 2400;
            uartSetup.clkInput = 60000000;
            uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
            uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
            uartSetup.parity =  CSL_UART_DISABLE_PARITY;
            uartSetup.rtsEnable = CSL_UART_NO_RTS;
            uartSetup.stopBits = 0;
            uartSetup.wordLength = CSL_UART_WORD8;

            status =  UART_init(&uartObj,uartInstId);
            hUart  = (CSL_UartHandle)&uartObj;
            status =  UART_setup( hUart,&uartSetup);
            status =  UART_fputs(hUart,buf);
     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_fputs(CSL_UartHandle hUart, const Char* pBuf, Uint32 timeout)
{
    Uint32 TimeOut  = timeout;
    CSL_Status status = CSL_SOK;

    if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }

    if( NULL == pBuf )
     {
        return CSL_ESYS_INVPARAMS;
     }

    CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABON);

    if( 0 != timeout )
    {
        while('\0' != *pBuf)
        {
            while( ( !UART_getXmitHoldRegEmptyStatus(hUart) ) && TimeOut-- );
            if (0 == TimeOut)
                return CSL_EUART_TIMEOUT;
            CSL_FSET(hUart->uartRegs->THR, 7, 0, (*pBuf)&CSL_UART_THR_DATA_MASK);
            pBuf++;
            TimeOut  = timeout;
        }
    }
    else
    {
        while( '\0' != *pBuf )
        {
            while(!UART_getXmitHoldRegEmptyStatus(hUart));
            CSL_FSET(hUart->uartRegs->THR, 7, 0, (*pBuf)&CSL_UART_THR_DATA_MASK);
            pBuf++;
        }
    }

  return status;
}

/** ============================================================================
 *   @n@b UART_getEventId
 *
 *   @b Description
 *   @n UART_getEventId returns the event id in the Handle.
 *
 *   @b Arguments
 *   @verbatim
        None
      @endverbatim
 *
 *   <b> Return Value </b> Uint16
 *   @li                IRQ_EVT_UART -UART_getEventId call is successful.
 *   <b> Pre Condition </b>
 *   @n  UART_init  should be called successfully.
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *   @b Example
 *   @verbatim
              CSL_Status              status;
              CSL_UartObj                uartObj;
            CSL_UartHandle             hUart;
            Uint16                  uartEvtId;
            Uint32 uartInstId = 0;
            status =  UART_init(&uartObj,uartInstId);
            hUart = (CSL_UartHandle)(&uartObj);
            uartEvtId =  UART_getEventId(hUart);
     @endverbatim
 *  ============================================================================
 */

 Int16 UART_getEventId(CSL_UartHandle hUart)
{
    Uint16 IID;

    if( NULL == hUart )
    {
         return CSL_ESYS_BADHANDLE;
    }
    IID = CSL_FEXT(hUart->uartRegs->IIR,UART_IIR_INTID);
    return IID;
}

/** ============================================================================
 *   @n@b UART_getConfig
 *
 *   @b Description
 *   @n UART_getEventId returns the event id in the Handle.
 *
 *   @b Arguments
 *   @verbatim
        None
      @endverbatim
 *
 *   <b> Return Value </b> Uint16
 *   @li               IRQ_EVT_UART -UART_getEventId call is successful.
 *   <b> Pre Condition </b>
 *   @n  UART_init and UART_setup  should be called successfully.
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *   @b Example
 *   @verbatim
              CSL_Status              status;
              CSL_UartObj                uartObj;
            Uint32                  uartInstId;
            Uint16                  uartEvtId;
            CSL_UartSetup              uartSetup;
            CSL_UartConfig             getconfig;
            uartInstId = 0;

            uartSetup.afeEnable = CSL_UART_NO_AFE;
            uartSetup.baud = 2400;
            uartSetup.clkInput = 60000000;
            uartSetup.fifoControl = CSL_UART_FIFO_DISABLE;
            uartSetup.loopBackEnable = CSL_UART_NO_LOOPBACK;
            uartSetup.parity =  CSL_UART_DISABLE_PARITY;
            uartSetup.rtsEnable = CSL_UART_NO_RTS;
            uartSetup.stopBits = 0;
            uartSetup.wordLength = CSL_UART_WORD8;

            status =  UART_init(&uartObj,uartInstId);
            status =  UART_Setup(hUart,&uartSetup);
            status =  UART_getConfig(hUart,&getconfig);
     @endverbatim
 *  ============================================================================
 */

 CSL_Status UART_getConfig(CSL_UartHandle hUart,CSL_UartConfig *config)
 {

     Uint8 temp;

     CSL_Status status = CSL_SOK;

     if( NULL == hUart )
     {
         return CSL_ESYS_BADHANDLE;
     }

     if( NULL == config)
     {
         return CSL_ESYS_INVPARAMS;
     }

    config->LCR =  hUart->uartRegs->LCR;
    temp = CSL_FEXT(config->LCR, UART_LCR_DLAB);

    CSL_FINST(hUart->uartRegs->LCR, UART_LCR_DLAB, DLABOFF);
    config->DLL = hUart->uartRegs->DLL;
    config->DLH = hUart->uartRegs->DLH;
    /*Restore the original value of DLAB*/
    CSL_FINS(hUart->uartRegs->LCR, UART_LCR_DLAB, temp);
    /*Note: FCR is a write only register, when read back, it will be IIR.*/
    /*Only FIFO enable bits was obtained from IIR here*/
    config->FCR  = hUart->uartRegs->FCR;
    config->MCR = hUart->uartRegs->MCR;
    return status;
}
/** ============================================================================
 *   @n@b UART_setCallback
 *
 *   @b Description
 *   @n UART_setCallback will set call back functions in isrDispatchTable.
 *
 *   @b Arguments
 *   @verbatim
         hUart - Handle to the UART.
         isrAddr - pointer to the     CSL_UartIsrAddr.
      @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                              CSL_SOK - UART_setCallback call is successful.
 *   <b> Pre Condition </b>
 *   @n  UART_init  should be called successfully.
 *   <b> Post Condition </b>
 *   @n  Setup call back functions in the UART_isrDispatchTable
 *
 *   @b Modifies
 *   @n hUart
 *   @b Example
 *   @verbatim
          CSL_Status              status;
          CSL_UartObj                uartObj;
         Uint32                  uartInstId;
         CSL_UartHandle              hUart;
         CSL_UartIsrAddr              isrAddr;
         uartInstId = 0;

         status =  UART_init(&uartObj,uartInstId);
         hUart  = (CSL_UartHandle)&uartObj;
         status =   UART_setCallback(hUart,&isrAddr);

     @endverbatim
 *  ============================================================================
 */

CSL_Status UART_setCallback(CSL_UartHandle hUart,CSL_UartIsrAddr *isrAddr)
{
    CSL_Status status = CSL_SOK;

    if( NULL == hUart )
    {
       return CSL_ESYS_BADHANDLE;
    }

    if( NULL == isrAddr)
    {
       return CSL_ESYS_INVPARAMS;
    }

    hUart->UART_isrDispatchTable[UART_EVT_LSI_IID]  = (Uint32)isrAddr->lsiAddr;
    hUart->UART_isrDispatchTable[UART_EVT_RBI_IID]  = (Uint32)isrAddr->rbiAddr;
    hUart->UART_isrDispatchTable[UART_EVT_TBEI_IID] = (Uint32)isrAddr->tbeiAddr;
    hUart->UART_isrDispatchTable[UART_EVT_CTOI_IID] = (Uint32) isrAddr->ctoi;
    return status;
}


