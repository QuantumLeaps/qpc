/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_intc.h
 *
 *  @brief INTC functional layer API header file
 *
 *  Path: \\(CSLPATH)\\inc
 *
 */
/* ============================================================================
 * Revision History
 * ================
 * 22-Sept-2008 - File created for CSL
 * ============================================================================
 */

/** @defgroup CSL_INTC_API INTC
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 * Interrupts are hardware or software driven signals that cause the DSP
 * to suspend its current program sequence and execute another task called
 * an interrupt service routine (ISR).
 *
 * @subsection References
 *    -# C5505_Spec_1.16.pdf
 *
 */

#ifndef _CSL_INTC_H_
#define _CSL_INTC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <soc.h>
#include <csl_error.h>
#include <csl_types.h>
#include <csl_general.h>

/**
@defgroup CSL_INTC_SYMBOL  INTC Symbols Defined
@ingroup CSL_INTC_API
*/
/**
@defgroup CSL_INTC_DATASTRUCT  INTC Data Structures
@ingroup CSL_INTC_API
*/
/**
@defgroup CSL_INTC_FUNCTION  INTC Functions
@ingroup CSL_INTC_API
*/

/**
@defgroup CSL_INTC_ENUM INTC Enumerated Data Types
@ingroup CSL_INTC_API
*/

/**
@addtogroup CSL_INTC_SYMBOL
@{ */

/*****************************************************************************\
          INTC global macro declarations
\*****************************************************************************/

/** Interrupt Vector Pointer Mask value                  */
#define CSL_CPU_IVPD_MASK       0xFFFFFF00
/** Interrupt Vector Pointer Shift value                 */
#define CSL_CPU_IVPD_SHIFT      8

/** Number of physical interrupts                                            */
#define IRQ_INT_CNT             (32u)

/** Number of Interrupt events                                               */
#define IRQ_EVENT_CNT           (32u)

/** Number of INTC pins                                                      */
#define CSL_INTC_NUM_PIN        (32u)

/** Set the single bit                                                       */
#define CSL_INTC_BIT_SET        (1u)

/** Reset the single bit                                                     */
#define CSL_INTC_BIT_RESET      (0)

/** Reset value for the interrupt flag register                              */
#define CSL_INTC_IFR_RESET      (0xFFFFu)

/** \brief  Events Numbers correspond to the bit position in
    IFR and IER Registers
    For event numbers (27 -31), IER1 and IFR1 bits are reserved
*/
/** Reset Interrupt           */
#define RESET_EVENT  0
/** Non Maskable Interrupt    */
#define NMI_EVENT    1
/** External User Interrupt 0 */
#define INT0_EVENT   2
/** External User Interrupt 1 */
#define INT1_EVENT   3
/** TIMER Interrupt           */
#define TINT_EVENT   4
/** Programmable transmit Interrupt 0 (I2S0 Tx or MMC/SD0 Interrupt)     */
#define PROG0_EVENT  5
/** UART Interrupt            */
#define UART_EVENT   6
/** Programmable Receive Interrupt 0 (I2S0 Rx or MMC/SD0 SDIO Interrupt) */
#define PROG1_EVENT  7
/** DMA Interrupt */
#define DMA_EVENT    8
/** Programmable transmit Interrupt 1 (I2S1 Tx or MMC/SD1 Interrupt)     */
#define PROG2_EVENT  9
/** CoProcessor Interrupt */
#define CoProc_EVENT 10
/** Programmable Receive Interrupt 1 (I2S1 Rx or MMC/SD1 SDIO Interrupt) */
#define PROG3_EVENT  11
/** LCD Interrupt */
#define LCD_EVENT    12
/** SAR Interrupt */
#define SAR_EVENT    13
/** I2S2 Transmit Interrupt */
#define XMT2_EVENT   14
/** I2S2 Receive Interrupt */
#define RCV2_EVENT   15
/** I2S3 Transmit Interrupt */
#define XMT3_EVENT   16
/** I2S3 Receive Interrupt */
#define RCV3_EVENT   17
/** Wakeup or RTC Interrupt */
#define RTC_EVENT    18
/** SPI Interrupt */
#define SPI_EVENT    19
/** USB Interrupt */
#define USB_EVENT    20
/** GPIO Interrupt */
#define GPIO_EVENT   21
/** EMIF Interrupt */
#define EMIF_EVENT   22
/** I2C Interrupt */
#define I2C_EVENT    23
/** Bus Error Interrupt */
#define BERR_EVENT   24
/** Emulation Interrupt DLOG */
#define DLOG_EVENT   25
/** Emulation Interrupt RTOS */
#define RTOS_EVENT   26
/** These event bits (27-31) are reserved in IFR and IER Register */
/** Emulation Interrupt RTDX Receive */
#define RTDXRCV_EVENT  27
/** Emulation Interrupt RTDX Transmit */
#define RTDXXMT_EVENT  28
/** Emulation monitor mode            */
#define EMUINT_EVENT  29
/** Software Interrupt 30             */
#define SINT30_EVENT  30
/** Software Interrupt 31             */
#define SINT31_EVENT  31



/** Allows to mask the bit "x" for eventId               */
#define IRQ_MASK32(x)          ((Uint32)(0x1ul<<x))
/** Allows to mask the bit "x" of IFR or IER registers   */
#define IRQ_MASK16(x)          ((Uint16)(0x1ul<<x))

/**  For Assembler */
#define  ALGEBRAIC 1

/** invalid pointer */
#define INV    ((void*)(-1))

/**
@} */

/**************************************************************************\
* INTC global typedef declarations                                        *
\**************************************************************************/

/** @addtogroup CSL_INTC_ENUM
 @{ */



/**
@} */

/**
\addtogroup CSL_INTC_DATASTRUCT
@{ */

/** \brief  ISR Routine
*/
typedef void (*IRQ_IsrPtr)(void);

/** \brief Config structure for INTC module
*/
typedef struct {
/** ISR Function Address              */
IRQ_IsrPtr funcAddr;
/** Arguments for ISR function        */
Uint32 funcArg;
/** IER Interrupt CPU Register        */
Uint32 ierMask;
/** cacheCtrl                         */
Uint32 cacheCtrl;
} CSL_IRQ_Config;


/** \brief INTC dispatch structure parameters
*/
typedef struct {
  /** ISR Function Address              */
  IRQ_IsrPtr funcAddr;
  /** Arguments for ISR function        */
  Uint32 funcArg;
  /** IER Interrupt CPU Register        */
  Uint32 ierMask;
  /** cacheCtrl                         */
  Uint32 cacheCtrl;
} CSL_IRQ_Dispatch;

/** \brief INTC Data object structure parameters
*/
typedef struct {
  /** Pointer to INTC dispatch structure */
  CSL_IRQ_Dispatch *IrqDispatchTable;
  /** Irq Interrupt table               */
  Uint32 IrqIntTable[IRQ_INT_CNT];
  /** Irq Event table                   */
  Uint32 IrqEventTable[IRQ_EVENT_CNT];
  /** DspBios is present or not */
  Uint16            biosPresent;
} CSL_IrqDataObj;

/** \brief  Definition for INTC Data Object Structure
*/
static CSL_IrqDataObj  CSL_IRQ_DATA;

/** \brief this is a pointer to @a CSL_IrqObj and is passed as the first
 * parameter to all INTC CSL APIs
 */
typedef CSL_IrqDataObj *IRQ_Handle;

/**
@} */

/*******************************************************************************
 * INTC function declarations
 ******************************************************************************/

/** @addtogroup CSL_INTC_FUNCTION
 @{ */

/** ============================================================================
 *   @n@b IRQ_plug
 *
 *   @b Description
 *   @n This function is used to register the ISR routine for the
 *      corresponding interrupt event.This is defined in assembly
 *      file - csl_irqplug.asm
 *
 *   @b Arguments
 *   @verbatim
       EventId      Interrupt Event Number
       funcAddr     ISR function Address
 *   @endverbatim
 *
 *
 *   <b> Pre Condition </b>
 *   @n  IRQ_setVecs API should be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  Register ISR with Interrupt event number
 *
 *
 *   @b Modifies
 *   @n  1. update vector table
 *   @n
 *
 *   @b Example
 *   @verbatim
            interrupt void ISR_routine(void);
            CSL_Status            status;
            Uint32                vectorAddress;
            // Adress for interrupt vector table
             extern void VECSTART(void); // defined in vector table
            ...

            // to set the interrupt vector table address
            status = IRQ_setVecs((Uint32)&VECSTART);
            IRQ_plug(SAR_EVENT,&ISR_routine);


            ...

    @endverbatim
 *  ============================================================================
 */

int  IRQ_plug(Uint16 EventId, IRQ_IsrPtr funcAddr);

/** ============================================================================
 *   @n@b IRQ_plug
 *
 *   @b Description
 *   @n This function is used to register the ISR routine for the
 *      corresponding interrupt event.This is defined in assembly
 *      file - csl_irqplug.asm
 *
 *   @b Arguments
 *   @verbatim
       EventId      Interrupt Event Number
       funcAddr     ISR function Address
 *   @endverbatim
 *
 *
 *   <b> Pre Condition </b>
 *   @n  IRQ_setVecs API should be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  Register ISR with Interrupt event number
 *
 *
 *   @b Modifies
 *   @n  1. update vector table
 *   @n
 *
 *   @b Example
 *   @verbatim
            interrupt void ISR_routine(void);
            CSL_Status            status;
            Uint32                vectorAddress;
            // Adress for interrupt vector table
             extern void VECSTART(void); // defined in vector table
            ...

            // to set the interrupt vector table address
            status = IRQ_setVecs((Uint32)&VECSTART);
            IRQ_plug(SAR_EVENT,&ISR_routine);


            ...

    @endverbatim
 *  ============================================================================
 */
int  _IRQ_plug(Uint16 EventId, IRQ_IsrPtr funcAddr);


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
 );

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
);

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
void IRQ_clearAll(void);

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
    Uint16         EventId,
    CSL_IRQ_Config *config
);



/** ============================================================================
 *   @n@b IRQ_getConfig
 *
 *   @b Description
 *   @n It reads the configuration values (function address,arguments etc)
 *   from global IRQ data object structure
 *
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
);

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
);

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
void IRQ_disableAll (void);

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
 *   @n IRQ_setVecs and IRQ_plug API should be called
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
Uint16         EventId
);


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
);



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
);


/** ============================================================================
 *   @n@b IRQ_map
 *
 *   @b Description
 *   @n It initialize the interrupt table with the event mask value for
 *   the corresponding event id
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
);

/** ============================================================================
 *   @n@b IRQ_setArg
 *
 *   @b Description
 *   @n It sets the ISR function arguments correspond to the eventId
 *
 *   @b Arguments
 *   @verbatim
             EventId         Id for peripheral in IFR and IER Registers
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
 *   @n CSL_IrqData structure
 *
 *   @b Example
 *   @verbatim
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
);


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
 *   @li                 Always returns CSL_SOK
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
);



/** ============================================================================
 *   @n@b IRQ_test
 *
 *   @b Description
 *   @n It reads the status bit for the particular event from IFR Registers
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
Uint16    EventId,
Bool     *IntStatus
);



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
Bool IRQ_globalDisable ();




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
 Bool IRQ_globalEnable ();


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
 void IRQ_globalRestore (Bool val);



/**
@} */

#ifdef __cplusplus
}
#endif

#endif /* _CSL_INTC_H_ */

