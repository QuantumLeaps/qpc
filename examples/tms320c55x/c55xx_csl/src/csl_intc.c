/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_intc.c
 *
 *  @brief INTC functional layer API source file
 *
 *  Path: \\(CSLPATH)\\src
 */

/* ============================================================================
 * Revision History
 * ================
 *
 * ============================================================================
 */
#include "csl_intc.h"

/** ============================================================================
 *   @n@b IRQ_init
 *
 *   @b Description
 *   @n This is the initialization function for INTC module. This function
 *      initializes the CSL INTC data structures
 *
 *
 *   @b Arguments
 *   @verbatim
       dispatchTable - Dispatch table
       biosPresent   - DspBios is present or not
 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK - IRQ_init is successful
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid parameter
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Initializes CSL data structures
 *
 *
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. Update CSL_IrqDataObj structure
 *
 *   @b Example
 *   @verbatim
            CSL_Status            status;
            CSL_IRQ_Dispatch     dispatchTable
            ...

            status = IRQ_init(&dispatchTable,0);
            ...

    @endverbatim
 *  ============================================================================
 */

CSL_Status IRQ_init (
CSL_IRQ_Dispatch *dispatchTable,
Uint16            biosPresent
)
{
    int i;

    if( INV == dispatchTable )
    {
        return CSL_ESYS_INVPARAMS;
    }

    CSL_IRQ_DATA.IrqDispatchTable = dispatchTable;

    for (i=0; i<=IRQ_EVENT_CNT-1; i++)
        CSL_IRQ_DATA.IrqIntTable[i] = i;

    for(i=0; i<= IRQ_EVENT_CNT-1; i++)
        CSL_IRQ_DATA.IrqEventTable[i] = IRQ_MASK32(i);

    CSL_IRQ_DATA.biosPresent = biosPresent;
    return CSL_SOK;
}

/** ============================================================================
 *   @n@b IRQ_clear
 *
 *   @b Description
 *   @n This function acknowledge the interrupt by clearing
 *      the corresponding Interrupt flag .
 *
 *   @b Arguments
 *   @verbatim
            eventId          Event Id for the peripheral in IFR Register
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - INTC_close is successful
 *   @li                    CSL_ESYS_INVPARAMS  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  IRQ_setVecs and IRQ_plug API should be called
 *     before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  IFR Register bit will be cleared
 *
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. IFR Register
 *   @b Example
 *   @verbatim
            Uint16           EventId;
            CSL_status       status;

            ...

            status = IRQ_clear(EventId);
     @endverbatim
 *  ============================================================================
 */

CSL_Status IRQ_clear (
Uint16          EventId
)
{
    Uint16 bit;
    Bool old_intm;

    /* Wrong Event Id */
    if (SINT31_EVENT < EventId)
    {
        return CSL_ESYS_INVPARAMS;
    }

    old_intm = IRQ_globalDisable();

    bit = EventId;

    if(EventId > RCV2_EVENT)
    {
        /* IFR1 Register */
        bit = bit - XMT3_EVENT;
        CSL_FINSR(CSL_CPU_REGS->IFR1, bit, bit,CSL_INTC_BIT_SET);
    }
    else
    {
        /* IFR0 Register */
        CSL_FINSR(CSL_CPU_REGS->IFR0, bit, bit,CSL_INTC_BIT_SET);
    }

    IRQ_globalRestore(old_intm);
    return CSL_SOK;
}

/** ============================================================================
 *   @n@b IRQ_clearAll
 *
 *   @b Description
 *   @n This function clears all the interrupts. Both IFR0 and IFR1 are cleared
 *      by this function.
 *
 *   @b Arguments
 *   @verbatim
            None
     @endverbatim
 *
 *   <b> Return Value </b>  None
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Clears All the interrupt
 *
 *   @b Modifies
 *   @n    Interrupt Flag Registers
 *
 *   @b Example
 *   @verbatim
            IRQ_clearAll();
     @endverbatim
 *  ============================================================================
 */
void IRQ_clearAll(void)
{
    /* Clear IFR0 */
    CSL_CPU_REGS->IFR0 = CSL_INTC_IFR_RESET;

    /* Clear IFR1 */
    CSL_CPU_REGS->IFR1 = CSL_INTC_IFR_RESET;
}

/** ============================================================================
 *   @n@b IRQ_config
 *
 *   @b Description
 *   @n This API function is used to update ISR function
 *      and its arguments passed in config structure for
 *      the corresponding event in dispatch table
 *   @b Arguments
 *   @verbatim
            EventId         Id for peripheral in IFR and IER Registers .

            config          Config structure

 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - IRQ_config call is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  IRQ_init API should be called before this API call
 *
 *   <b> Post Condition </b>
 *   @n  It updates CSL_IrqDataObj structure
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. Hardware Registers
 *
 *   @b Example
 *   @verbatim
         interrupt void ISR_routine(void);
         CSL_Status     status;
         // Adress for interrupt vector table
         extern void VECSTART(void);
         IRQ_Config     config;
         Uint16         EventId;
         status = IRQ_init();
         ...
         // to set the interrupt vector table address
         IRQ_setVecs((Uint32)&VECSTART);
         ...
         config.funcAddr = &ISR_routine;
         status = IRQ_config(EventId,&config);
         ...
    @endverbatim
 *  ============================================================================
 */

CSL_Status IRQ_config (
Uint16     EventId,
CSL_IRQ_Config *config
)
{
    if(NULL == config)
    {
        return CSL_ESYS_INVPARAMS;
    }

    /* Wrong Event Id */
    if(SINT31_EVENT < EventId)
    {
        return CSL_ESYS_INVPARAMS;
    }

    CSL_IRQ_DATA.IrqDispatchTable[EventId].funcAddr
    = config->funcAddr;

    CSL_IRQ_DATA.IrqDispatchTable[EventId].funcArg
    = config->funcArg;

    CSL_IRQ_DATA.IrqDispatchTable[EventId].ierMask = config->ierMask ;

    CSL_IRQ_DATA.IrqDispatchTable[EventId].cacheCtrl = config->cacheCtrl;

    return CSL_SOK;
}



/** ============================================================================
 *   @n@b IRQ_getConfig
 *
 *   @b Description
 *   @n It reads the configuration values (function address,arguments etc)
 *   from global IRQ data object structure
 *   @b Arguments
 *   @verbatim
          EventId         Id for peripheral in IFR and IER Registers

          config          Config structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK           - IRQ_getConfig is successfull
 *
 *   @li                    CSL_ESYS_BADHANDLE - The handle is passed is
 *                                                invalid
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  IRQ_config should be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  The configuration structure will be populated
 *
 *   @b Modifies
 *   @n 1.status
 *      2.config structure
 *   @b Example
 *   @verbatim
        // Global IRQ object structure
         CSL_IrqDataObj  CSL_IrqData;
         CSL_Status     status;
         IRQ_Config     config;
         Uint16         EventId;
            ...
            status = IRQ_config(EventId,&config);
             ...
            status = IRQ_getConfig(EventId,&config);
     @endverbatim
 *  ============================================================================
 */

CSL_Status IRQ_getConfig (
Uint16          EventId,
CSL_IRQ_Config  *config
)
{
    if(NULL == config)
    {
        return CSL_ESYS_INVPARAMS;
    }
    /* Wrong Event Id */
    if(SINT31_EVENT < EventId)
    {
        return CSL_ESYS_INVPARAMS;
    }
    config->funcAddr =
    (IRQ_IsrPtr)CSL_IRQ_DATA.IrqDispatchTable[EventId].funcAddr;

    config->funcArg
    = CSL_IRQ_DATA.IrqDispatchTable[EventId].funcArg ;

    config->ierMask = CSL_IRQ_DATA.IrqDispatchTable[EventId].ierMask;

    config->cacheCtrl = CSL_IRQ_DATA.IrqDispatchTable[EventId].cacheCtrl;

    return CSL_SOK;
}

/** ============================================================================
 *   @n@b IRQ_disable
 *
 *   @b Description
 *   @n It disables the corresponding interrupt in IER Register and
 *      also return the previous bit mask value
 *
 *   @b Arguments
 *   @verbatim
            EventId         Id for peripheral in IFR and IER Registers

 *   @endverbatim
 *
 *   <b> Return Value </b>
 *         IER Register value before enabling
 *
 *   <b> Pre Condition </b>
 *   @n IRQ_setVecs andIRQ_plug API should be called
 *     before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  It disabled the corresponding interrupt bit in IER Register
 *
 *   @b Modifies
 *   @n IER h/w Registers
 *
 *   @b Example
 *   @verbatim
            Uint16         EventId;
            int            old_IER;

            old_IER = IRQ_disable(EventId);
     @endverbatim
 *  ============================================================================
 */

int IRQ_disable (
Uint16          EventId
)
{
    Uint16 bit;
    int old_flag;
    Bool old_intm;
    /* Wrong Event Id */
    if(SINT31_EVENT < EventId)
    {
        return CSL_ESYS_INVPARAMS;
    }

    bit = EventId;
    old_intm = IRQ_globalDisable();
    /* EventId greater than 15 */
    if (EventId > RCV2_EVENT)
    {

        bit = bit - XMT3_EVENT;
        old_flag = CSL_FEXTR(CSL_CPU_REGS->IER1,bit,bit) ;
        CSL_FINSR(CSL_CPU_REGS->IER1, bit,bit,CSL_INTC_BIT_RESET);
    }
    /* EventId less than 15 */
    else
    {
        old_flag = CSL_FEXTR(CSL_CPU_REGS->IER0,bit,bit) ;
        CSL_FINSR(CSL_CPU_REGS->IER0, bit,bit,CSL_INTC_BIT_RESET);
    }
    IRQ_globalRestore(old_intm);

    return old_flag;
}

/** ============================================================================
 *   @n@b IRQ_disableAll
 *
 *   @b Description
 *   @n This function disables all the interrupts avaible on C5505 DSP. Both
 *      IER0 and IER1 are cleared by this function
 *
 *   @b Arguments
 *   @verbatim
            None

 *   @endverbatim
 *
 *   <b> Return Value </b>
 *         None
 *
 *   <b> Pre Condition </b>
 *   @n None
 *
 *   <b> Post Condition </b>
 *   @n  Disables all the interrupts
 *
 *   @b Modifies
 *   @n Interrupt Enable Registers
 *
 *   @b Example
 *   @verbatim

            IRQ_disableAll();
     @endverbatim
 *  ============================================================================
 */
void IRQ_disableAll (void)
{
    /* Clear IER0 */
    CSL_CPU_REGS->IER0 = CSL_CPU_IER0_RESETVAL;

    /* Clear IER1 */
    CSL_CPU_REGS->IER1 = CSL_CPU_IER1_RESETVAL;
}

/** ============================================================================
 *   @n@b IRQ_enable
 *
 *   @b Description
 *   @n It enables the corresponding interrupt bit in IER Register and
 *      also return the previous bit mask value
 *
 *   @b Arguments
 *   @verbatim
             EventId         Id for peripheral in IFR and IER Registers
 *   @endverbatim
 *
 *   <b> Return Value </b>
 *              IER Register value before enabling
 *
 *   <b> Pre Condition </b>
 *   @n IRQ_setVecs andIRQ_plug API should be called
 *     before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  It set the corresponding interrupt bit to 1 in IER Register
 *
 *   @b Modifies
 *   @n IER CPU Registers
 *
 *   @b Example
 *   @verbatim
            Uint16         EventId;
            int            old_IER;

            old_IER = IRQ_enable(EventId);
     @endverbatim
 *  ============================================================================
 */

int IRQ_enable (
Uint16          EventId
)
{
    Uint16 bit;
    int old_flag;
    Bool old_intm;
    /* Wrong Event Id */
    if (SINT31_EVENT < EventId)
    {
        return CSL_ESYS_INVPARAMS;
    }

    bit= EventId ;
    old_intm = IRQ_globalDisable();
    if (EventId > RCV2_EVENT)
    {
        bit = bit - XMT3_EVENT;
        old_flag = CSL_FEXTR (CSL_CPU_REGS->IER1,bit,bit);
        CSL_FINSR (CSL_CPU_REGS->IER1, bit,bit,CSL_INTC_BIT_SET);
    }
    else
    {
        old_flag = CSL_FEXTR (CSL_CPU_REGS->IER0,bit,bit);
        CSL_FINSR (CSL_CPU_REGS->IER0, bit, bit,CSL_INTC_BIT_SET);
    }

    IRQ_globalRestore(old_intm);

    return old_flag;
}

/** ============================================================================
 *   @n@b IRQ_restore
 *
 *   @b Description
 *   @n It restores the given value in IER Register passed in API
 *
 *   @b Arguments
 *   @verbatim
             EventId         Id for peripheral in IFR and IER Registers
             value           bit value - 1 or 0
 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK      always returns
 *
 *   <b> Pre Condition </b>
 *   @n IRQ_enable or IRQ_disable should be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  It configures the given bit value in IER Register
 *
 *   @b Modifies
 *   @n IER CPU Registers
 *
 *   @b Example
 *   @verbatim
            Uint16         EventId;
            int             value;
            CSL_Status     status;
            value = IRQ_enable(EventId);
            ..
            status = IRQ_restore(EventId,value);
     @endverbatim
 *  ============================================================================
 */

CSL_Status IRQ_restore(
Uint16 EventId,
int value
)
{
    Uint16 bit;
    Bool old_intm;

    /* Wrong Event Id */
    if(SINT31_EVENT < EventId)
    {
        return CSL_ESYS_INVPARAMS;
    }

    bit = EventId;
    old_intm = IRQ_globalDisable();
    if(EventId > RCV2_EVENT)
    {
        bit = bit - XMT3_EVENT;
        CSL_FINSR (CSL_CPU_REGS->IER1, bit, bit,value);
    }
    else
    {
        CSL_FINSR (CSL_CPU_REGS->IER0, bit, bit,value);
    }
    IRQ_globalRestore(old_intm);

    return CSL_SOK;
}

/** ============================================================================
 *   @n@b IRQ_getArg
 *
 *   @b Description
 *   @n It gets the ISR function arguments correspond to eventId
 *
 *   @b Arguments
 *   @verbatim
             EventId         Id for peripheral in IFR and IER Registers
 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - IRQ_enable is successful
 *   @li                    CSL_ESYS_BADHANDLE  - The handle is passed is
 *                                                invalid
 *   @li                    CSL_ESYS_INVPARAMS -  Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n IRQ_init,IRQ_config should be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n

 *   @b Modifies
 *   @n arg variable
 *
 *   @b Example
 *   @verbatim
            Uint16         EventId;
            Uint32         arg;
            CSL_Status     status;

            status = IRQ_getArg(EventId,&arg);
     @endverbatim
 *  ============================================================================
 */

CSL_Status IRQ_getArg(
Uint16      EventId,
Uint32      *arg
)
{
    if(NULL == arg)
    {
        return CSL_ESYS_INVPARAMS;
    }

    /* Wrong Event Id */
    if(SINT31_EVENT < EventId)
    {
        return CSL_ESYS_INVPARAMS;
    }
    *arg = CSL_IRQ_DATA.IrqDispatchTable[EventId].funcArg ;

    return CSL_SOK;
}

/** ============================================================================
 *   @n@b IRQ_setArg
 *
 *   @b Description
 *   @n It sets the ISR function arguments correspond to the eventId
 *
 *   @b Arguments
 *   @verbatim
             EventId         Id for peripheral in IFR and IMR Registers
             val             value for ISR arguments
 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - IRQ_enable is successful
 *   @li                    CSL_ESYS_BADHANDLE  - The handle is passed is
 *                                                invalid
 *   @li                    CSL_ESYS_INVPARAMS -  Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n IRQ_init should be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n

 *   @b Modifies
 *   @n status
 *
 *   @b Example
 *   @verbatim
            // Global IRQ object structure
            CSL_IrqDataObj  CSL_IrqData;
            Uint16         EventId;
            Uint32         val;
            CSL_Status     status;

            status = IRQ_setArg(EventId,val);
     @endverbatim
 *  ============================================================================
 */

CSL_Status IRQ_setArg(
Uint16 EventId,
Uint32 val
)
{
    /* Wrong Event Id */
    if (SINT31_EVENT < EventId)
    {
        return CSL_ESYS_INVPARAMS;
    }
    CSL_IRQ_DATA.IrqDispatchTable[EventId].funcArg = val;

    return CSL_SOK;
}


/** ============================================================================
 *   @n@b IRQ_map
 *
 *   @b Description
 *   @n It initialize the interrupt table with the event mask value for
 *   the corresponding event id
 *   @b Arguments
 *   @verbatim
             EventId         Id for peripheral in IFR and IER Registers
 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - IRQ_enable is successful
 *   @li                    CSL_ESYS_BADHANDLE  - The handle is passed is
 *                                                invalid
 *   @li                    CSL_ESYS_INVPARAMS -  Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n IRQ_init should be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n

 *   @b Modifies
 *   @n CSL_IrqData structure
 *
 *   @b Example
 *   @verbatim
            Uint16         EventId;
            CSL_Status     status;

            status = IRQ_map(EventId);
     @endverbatim
 *  ============================================================================
 */

CSL_Status IRQ_map(
Uint16 EventId
)
{
    /* Wrong Event Id */
    if (SINT31_EVENT < EventId)
    {
        return CSL_ESYS_INVPARAMS;
    }
    CSL_IRQ_DATA.IrqIntTable[EventId] = IRQ_MASK32(EventId);

    return CSL_SOK;
}

/** ============================================================================
 *   @n@b IRQ_setVecs
 *
 *   @b Description
 *   @n It stores the Interrupt vector table address in Interrupt vector
 *     pointer DSP and Interrupt vector pointer host Registers
 *
 *   @b Arguments
 *   @verbatim
             Ivpd           Interrupt Vector Pointer Address
 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - IRQ_enable is successful
 *   @li                    CSL_ESYS_BADHANDLE  - The handle is passed is
 *                                                invalid
 *   @li                    CSL_ESYS_INVPARAMS -  Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n None
 *
 *   <b> Post Condition </b>
 *   @n  It sets IVPD and IVPH CPU Registers

 *   @b Modifies
 *   @n IVPD and IVPH CPU Registers
 *
 *   @b Example
 *   @verbatim
           // Interrupt Vector Table Address
            Uint32         Ivpd;
            CSL_Status     status;

            status = IRQ_setVecs(Ivpd);
     @endverbatim
 *  ============================================================================
 */

CSL_Status IRQ_setVecs(
Uint32 Ivpd
)
{
    Bool old_intm;
    Uint16 val = (Uint16)((Ivpd & CSL_CPU_IVPD_MASK) >> CSL_CPU_IVPD_SHIFT);
    old_intm = IRQ_globalDisable();
    CSL_CPU_REGS->IVPD = val;
    CSL_CPU_REGS->IVPH = val;
    IRQ_globalRestore(old_intm);

    return CSL_SOK;
}

/** ============================================================================
 *   @n@b IRQ_test
 *
 *   @b Description
 *   @n It gets the status bit for the particular Interupt from IFR Registers
 *
 *   @b Arguments
 *   @verbatim
             EventId           Interrupt Vector Pointer Address
 *           IntStatus         to store Interrupt Status bit in IFR Register
 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - IRQ_enable is successful
 *   @li                    CSL_ESYS_BADHANDLE  - The handle is passed is
 *                                                invalid
 *   @li                    CSL_ESYS_INVPARAMS -  Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n IRQ_init should be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  It stores the IFR bit value in IntStatus variable

 *   @b Modifies
 *   @n 1. IntStatus - to store Interrupt flag bit
 *   @n
 *
 *   @b Example
 *   @verbatim
            Uint16         EventId;
            CSL_Status     status;
            bool           IntStatus;

            status = IRQ_test(EventId,&IntStatus);
     @endverbatim
 *  ============================================================================
 */

CSL_Status IRQ_test(
Uint16   EventId,
Bool    *IntStatus
)
{
    Uint16 bit;
    Bool old_intm;
    if(NULL == IntStatus)
    {
    return CSL_ESYS_INVPARAMS;
    }
    /* Wrong Event Id */
    if(SINT31_EVENT < EventId)
    {
        return CSL_ESYS_INVPARAMS;
    }
    bit= EventId ;
    old_intm = IRQ_globalDisable();

    if(EventId > RCV2_EVENT)
    {
        bit = bit - XMT3_EVENT;
        *IntStatus = CSL_FEXTR(CSL_CPU_REGS->IFR1, bit, bit);
    }
    else
    {
        *IntStatus = CSL_FEXTR(CSL_CPU_REGS->IFR0, bit, bit);
    }
    IRQ_globalRestore(old_intm);

    return CSL_SOK;
}

/** ============================================================================
 *   @n@b IRQ_globalDisable
 *
 *   @b Description
 *   @n It disables the interrupt globally by disabling INTM bit and also
 *      return the previous mask value for INTM bit
 *
 *   @b Arguments
 *   @verbatim

 *   @endverbatim
 *
 *   <b> Return Value </b>  Bool
 *   @li                    Old INTM bit value
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  set INTM bit to 1 in ST1 CPU Register

 *   @b Modifies
 *      1. ST1 CPU Register
 *   @b Example
 *   @verbatim
            Bool oldMask;
            ....
            oldMask = IRQ_globalDisable();
     @endverbatim
 *  ============================================================================
 */

Bool IRQ_globalDisable()
{
    Bool value;
    value = CSL_FEXT(CSL_CPU_REGS->ST1_55, CPU_ST1_55_INTM);
    asm("\tNOP                      ;====> CODE AUTO-GENERATED by CSL");
    #ifdef ALGEBRAIC
    asm("\tBIT (ST1,#ST1_INTM) = #1   ;====> CODE AUTO-GENERATED by CSL");
    #else
    asm("\tBSET INTM                ;====> CODE AUTO-GENERATED by CSL");
    #endif

    return value;
}

/** ============================================================================
 *   @n@b IRQ_globalEnable
 *
 *   @b Description
 *   @n It enables the interrupt globally by enabling INTM bit and also
 *      return the previous mask value for INTM bit
 *
 *   @b Arguments
 *   @verbatim

 *   @endverbatim
 *
 *   <b> Return Value </b>  Bool
 *   @li                    Old INTM bit value
 *
 *   <b> Pre Condition </b>
 *   @n None
 *
 *   <b> Post Condition </b>
 *   @n  set INTM bit to 0 in ST1 CPU Register

 *   @b Modifies
 *      1.ST1 CPU Register
 *   @b Example
 *   @verbatim
            Bool oldMask;
            ....
            oldMask = IRQ_globalEnable();
     @endverbatim
 *  ============================================================================
 */

Bool IRQ_globalEnable()
{
    Bool value;
    value = CSL_FEXT(CSL_CPU_REGS->ST1_55, CPU_ST1_55_INTM);
    asm("\tNOP                      ;====> CODE AUTO-GENERATED by CSL");
    #ifdef ALGEBRAIC
    asm("\tBIT (ST1,#ST1_INTM) = #0    ;====> CODE AUTO-GENERATED by CSL");
    #else
    asm("\tBCLR INTM                ;====> CODE AUTO-GENERATED by CSL");
    #endif
    return value;
}

/** ============================================================================
 *   @n@b IRQ_globalRestore
 *
 *   @b Description
 *   @n It sets INTM bit to the value passed in the API
 *
 *   @b Arguments
 *   @verbatim
            val -  INTM bit value
 *   @endverbatim
 *
 *   <b> Return Value </b>  void
 *
 *   <b> Pre Condition </b>
 *   @n  IRQ_globalDisable or IRQ_globalEnable should be called
 *
 *   <b> Post Condition </b>
 *   @n  set or clear INTM bit in ST1 CPU Register

 *   @b Modifies
 *   1. ST1 CPU Register
 *   @b Example
 *   @verbatim
            Bool val;
            ....
            IRQ_globalRestore(val);
     @endverbatim
 *  ============================================================================
 */

void IRQ_globalRestore(
Bool value
)
{
    if(value)
    {
        asm("\tNOP                      ;====> CODE AUTO-GENERATED by CSL");
        #ifdef ALGEBRAIC
        asm("\tBIT (ST1,#ST1_INTM) = #1   ;====> CODE AUTO-GENERATED by CSL");
        #else
        asm("\tBSET INTM                ;====> CODE AUTO-GENERATED by CSL");
        #endif
    }

    else
    {
        asm("\tNOP                      ;====> CODE AUTO-GENERATED by CSL");
        #ifdef ALGEBRAIC
        asm("\tBIT (ST1,#ST1_INTM) = #0   ;====> CODE AUTO-GENERATED by CSL");
        #else
        asm("\tBCLR INTM                ;====> CODE AUTO-GENERATED by CSL");
        #endif
    }
}


