/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_i2c.h
 *
 *  @brief I2C functional layer API header file
 *
 *  Path: \(CSLPATH)\ inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 08-Oct-2008 Created
 * 23-Oct-2008 Updated for code review comments
 * ============================================================================
 */

/** @defgroup CSL_I2C_API I2C
 *
 * @section Introduction
 * Inter Integrated Circiut(I2C) is a bus technology used to transfer the serial
 * data between the I2C compliant devices. It supports multiple masters and
 * multi slaves.
 *
 * @subsection xxx Overview
 * The I2C peripheral provides an interface between the VC5505 and other
 * devices that are compliant with the I2C-bus specification and connected by
 * way of an I2C-bus. External components that are attached to this two-wire
 * serial bus can transmit and receive data that is up to eight bits wide both
 * to and from the VC5505 through the I2C peripheral.
 * I2C CSL module provides flexible way to configure the I2C peripheral
 * registers. Application can configure the whole register directly using
 * config function or can pass individual register bit configuration values
 * using setup function.
 * I2C CSL module can work in polled, interrupt and DMA modes.
 *
 *  @subsection References
 *  TMS320VC5505I2C USERS GUIDE.pdf
 */

#ifndef _CSL_I2C_H_
#define _CSL_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cslr.h"
#include "csl_error.h"
#include "csl_types.h"
#include "cslr_i2c.h"
#include "soc.h"
#include "csl_general.h"


/**
@defgroup CSL_I2C_SYMBOL  I2C Symbols Defined
@ingroup CSL_I2C_API
*/
/**
@defgroup CSL_I2C_DATASTRUCT  I2C Data Structures
@ingroup CSL_I2C_API
*/
/**
@defgroup CSL_I2C_FUNCTION  I2C Functions
@ingroup CSL_I2C_API
*/
/**
@defgroup CSL_I2C_ENUM I2C Enumerated Data Types
@ingroup CSL_I2C_API
*/


/*****************************************************************************\
          I2C global macro declarations                                        *
\*****************************************************************************/
/**
@addtogroup CSL_I2C_SYMBOL
@{
*/

/**  \brief  I2C Instance number                                             */
#define CSL_I2C0                                 (0)

/** I2C event Id for interrupt operation                                     */
#define CSL_I2C_INT_EVTID                        (23)
/** I2C interrupt event count                                                */
#define CSL_I2C_EVT_COUNT                        (7)
/** I2C device reset count value                                             */
#define CSL_I2C_RESET_COUNT_VAL                  (0x20)
/** I2C reset delay value                                                    */
#define CSL_I2C_RESET_DELAY                      (500)

/** I2C module clock frequency - (This should be between 7MHz and 12MHz)     */
#define CSL_I2C_MODULE_CLOCK_FREQ                (12)

/** Clock Multiplier to convert the clock into Hz from KHz and MHz           */
#define CSL_I2C_CLK_MULT                         (1000)

/** I2C Prescaler value is zero                                              */
#define CSL_I2C_PSC0                             (0)
/** I2C Prescaler value is one                                               */
#define CSL_I2C_PSC1                             (1)
/** Adjustment for the Prescaler value                                       */
#define CSL_I2C_PSC_ADJUST_VAL                   (1)

/** I2C d value for the prescaler value zero                                 */
#define CSL_I2C_PSC0_DVAL                        (7)
/** I2C d value for the prescaler value one                                  */
#define CSL_I2C_PSC1_DVAL                        (6)
/** I2C d value for the prescaler value 2h to FFh                            */
#define CSL_I2C_PSC2TOFF_DVAL                    (5)

/**
 * \brief  I2C Driver Error codes
 */
/** Error Code base                                                          */
#define CSL_I2C_ERROR_BASE                       (CSL_EI2C_FIRST)
/** Returned when the I2C bus find that the bus is busy                      */
#define CSL_I2C_BUS_BUSY_ERR                     (CSL_I2C_ERROR_BASE - 1)
/** Returned when the I2C driver lost the bus arbitration                    */
#define CSL_I2C_ARBITRATION_LOSS_ERR             (CSL_I2C_ERROR_BASE - 2)
/** Returned when the I2C slave did not generate an acknowledge              */
#define CSL_I2C_NACK_ERR                         (CSL_I2C_ERROR_BASE - 3)
/** Returned in case of an transmit underflow error                          */
#define CSL_I2C_TRANSMIT_UNDERFLOW_ERR           (CSL_I2C_ERROR_BASE - 4)
/** Returned in case of an rcv overflow error                                */
#define CSL_I2C_RECEIVE_OVERFLOW_ERR             (CSL_I2C_ERROR_BASE - 5)
/** Returned in case of an Cancelling IO error                               */
#define CSL_I2C_CANCEL_IO_ERROR                     (CSL_I2C_ERROR_BASE - 6)
/** Returned in case of timeout error                                        */
#define CSL_I2C_TIMEOUT_ERROR                     (CSL_I2C_ERROR_BASE - 7)

/**
 * \brief  I2C Transaction flags
 */
/** Read from I2C bus (device)                                               */
#define CSL_I2C_READ                             (0x1)
/** Write to I2C bus (device)                                                */
#define CSL_I2C_WRITE                            (0x2)
/** If this flag is not set, default is 7 bit address                        */
#define CSL_I2C_ADDR_FORMAT_10_BIT               (0x10)
/** Generate Start - valid in master mode only                               */
#define CSL_I2C_START                            (0x100)
/** Generate Stop - valid in master mode only                                */
#define CSL_I2C_STOP                             (0x200)
/** Re-Start is generated by Master                                          */
#define CSL_I2C_RESTART                          (0x400)
/** Start Byte as per Phillips I2C specs                                     */
#define CSL_I2C_START_BYTE                       (0x800)
/** Free Data Format                                                         */
#define CSL_I2C_FREE_DATA_FORMAT                 (0x1000)
/** Repeat mode as per TI I2C specs                                          */
#define CSL_I2C_REPEAT                           (0x2000)
/** Ignore Bus Busy condition                                                */
#define CSL_I2C_IGNORE_BUS_BUSY                  (0x20000)

/** Default start stop flag                                                  */
#define CSL_I2C_DEFAULT_STTSTP                   (CSL_I2C_START | CSL_I2C_STOP)

/** I2C timeout value                                                        */
#define CSL_I2C_MAX_TIMEOUT                      (0xFFFF)

/** Default value of I2C Own address register                                */
#define CSL_I2C_ICOAR_DEFVAL                     (0x2F)
/** Default value of I2C Interrupt mask register                             */
#define CSL_I2C_ICIMR_DEFVAL                     (0x0000)
/** Default value of I2C Slave address register (EEPROM address)             */
#define CSL_I2C_ICSAR_DEFVAL                     (0x50)
/** I2C Prescaler register defval(For 100MHz sysClk and 10KHz Bus frequency) */
#define CSL_I2C_ICPSC_DEFVAL                     (0x07)
/** I2C Clock register defval (For 100MHz sysClk and 10KHz Bus frequency)    */
#define CSL_I2C_ICCLK_DEFVAL                     (0x026C)
/** Default value of I2C Mode register for write operation                   */
#define CSL_I2C_ICMDR_WRITE_DEFVAL               (0x0E20)
/** Default value of I2C Mode register for read operation                    */
#define CSL_I2C_ICMDR_READ_DEFVAL                (0x0C20)
/** Default value of I2C Extended Mode register                              */
#define CSL_I2C_ICEMDR_DEFVAL                    (0x0000)
/** I2C status register reset value                                          */
#define CSL_I2C_ICSTR_RESET_VALUE                (0xFFFF)

/** I2C event handler                                                        */
typedef void      (*CSL_I2C_EVENT_ISR)(void);

/** Macro to read I2C register                                               */
/** NOTE: Pass the name of the register for 'reg' field
  * EX : Read I2C mode register
  *      Uint16    mdrVal;
  *
  *      CSL_I2C0_READREG(ICMDR, mdrVal);
  *                                                                          */
#define CSL_I2C0_READREG(reg,val)                (val = CSL_I2C_0_REGS->##reg)

/** Macro to write I2C register                                              */
#define CSL_I2C0_WRITEREG(reg,val)               (CSL_I2C_0_REGS->##reg = val)

/** Macro to Set I2C start bit                                               */
#define CSL_I2C_SETSTART()                                                    \
        CSL_FINST(CSL_I2C_0_REGS->ICMDR, I2C_ICMDR_STT, SET);

/** Macro to Reset I2C start bit                                             */
#define CSL_I2C_RESETSTART()                                                  \
        CSL_FINST(CSL_I2C_0_REGS->ICMDR, I2C_ICMDR_STT, CLEAR);

/** Macro to Set I2C stop bit                                                */
#define CSL_I2C_SETSTOP()                                                     \
        CSL_FINST(CSL_I2C_0_REGS->ICMDR, I2C_ICMDR_STP, SET);

/** Macro to Reset I2C stop bit                                              */
#define CSL_I2C_RESETSTOP()                                                   \
        CSL_FINST(CSL_I2C_0_REGS->ICMDR, I2C_ICMDR_STP, CLEAR);

/**
@} */

/**************************************************************************\
* I2C global typedef declarations                                           *
\**************************************************************************/

/** @addtogroup CSL_I2C_ENUM
 @{ */

/**
 *  \brief Enum for I2C events
 */
typedef enum CSL_I2cEvent {
    /** Arbitration Loss event                                               */
    CSL_I2C_EVENT_AL,
    /** No ACKnowledgement event                                             */
    CSL_I2C_EVENT_NACK,
    /** register Access ReaDY event                                          */
    CSL_I2C_EVENT_ARDY,
    /** I2C Receive ReaDY event                                              */
    CSL_I2C_EVENT_ICRRDY,
    /** I2C tX ReaDY event                                                   */
    CSL_I2C_EVENT_ICXRDY,
    /** Stop Condition Detected event                                        */
    CSL_I2C_EVENT_SCD,
    /** Address As Slave event                                               */
    CSL_I2C_EVENT_AAS
} CSL_I2cEvent;

/**
 *  \brief Enum for I2C Address mode
 */
typedef enum CSL_I2cAddrMode {
    /** I2C operates in 7 - bit addressing mode                              */
    CSL_I2C_ADDR_7BIT = 0,
    /** I2C operates in 10 - bit addressing mode                             */
    CSL_I2C_ADDR_10BIT
} CSL_I2cAddrMode;

/**
 *  \brief Enum for I2C bit count value
 */
typedef enum CSL_I2cBitCount {
    /** I2C Reads/Writes 8 bits in each data word transferred                */
    CSL_I2C_BC_8BITS = 0,
    /** This is reserved value - CANNOT BE USED                              */
    CSL_I2C_BC_RSVD,
    /** I2C Reads/Writes 2 bits in each data word transferred                */
    CSL_I2C_BC_2BITS,
    /** I2C Reads/Writes 3 bits in each data word transferred                */
    CSL_I2C_BC_3BITS,
    /** I2C Reads/Writes 4 bits in each data word transferred                */
    CSL_I2C_BC_4BITS,
    /** I2C Reads/Writes 5 bits in each data word transferred                */
    CSL_I2C_BC_5BITS,
    /** I2C Reads/Writes 6 bits in each data word transferred                */
    CSL_I2C_BC_6BITS,
    /** I2C Reads/Writes 7 bits in each data word transferred                */
    CSL_I2C_BC_7BITS
} CSL_I2cBitCount;

/**
 *  \brief Enum for I2C loopback mode bit
 */
typedef enum CSL_I2cLoopback {
    /** I2C loopback mode disabled                                           */
    CSL_I2C_LOOPBACK_DISABLE = 0,
    /** I2C loopback mode enabled                                            */
    CSL_I2C_LOOPBACK_ENABLE
} CSL_I2cLoopback;

/**
 *  \brief Enum for I2C free mode bit
 */
typedef enum CSL_I2cFreeMode {
    /** I2C free running mode disabled                                       */
    CSL_I2C_FREEMODE_DISABLE = 0,
    /** I2C free running mode enabled                                        */
    CSL_I2C_FREEMODE_ENABLE
} CSL_I2cFreeMode;

/**
 *  \brief Enum for I2C master mode bit
 */
typedef enum CSL_I2cMasterMode {
    /** I2C master mode disabled                                             */
    CSL_I2C_MASTERMODE_DISABLE = 0,
    /** I2C master mode enabled                                              */
    CSL_I2C_MASTERMODE_ENABLE
} CSL_I2cMasterMode;

/**
 *  \brief Enum for I2C repeat mode bit
 */
typedef enum CSL_I2cRepeatMode {
    /** I2C repeat mode disabled                                             */
    CSL_I2C_REPEATMODE_DISABLE = 0,
    /** I2C repeat mode enabled                                              */
    CSL_I2C_REPEATMODE_ENABLE
} CSL_I2cRepeatMode;

/**
@} */

/**************************************************************************\
* I2C global data structure declarations                                    *
\**************************************************************************/

/**
\addtogroup CSL_I2C_DATASTRUCT
@{ */

/**
 *  \brief I2C callback pointer structure
 *
 *  This structure contains pointers to I2C interrupt callback functions
 */
typedef struct CSL_I2cIsrAddr {
    /**  \brief ISR for arbitration loss event                               */
    CSL_I2C_EVENT_ISR    alAddr;
    /**  \brief ISR for no acknowledgement event                             */
    CSL_I2C_EVENT_ISR    nackAddr;
    /**  \brief ISR for access ready event                                   */
    CSL_I2C_EVENT_ISR    ardyAddr;
    /**  \brief ISR for receive ready event                                  */
    CSL_I2C_EVENT_ISR    rrdyAddr;
    /**  \brief ISR for transmit ready event                                 */
    CSL_I2C_EVENT_ISR    xrdyAddr;
    /**  \brief ISR for stop condition detected event                        */
    CSL_I2C_EVENT_ISR    scdAddr;
    /**  \brief ISR for address as slave event                               */
    CSL_I2C_EVENT_ISR    aasAddr;
} CSL_I2cIsrAddr;

/**
 *  \brief I2C Config structure
 *
 *  This structure contains the I2C register configuration parameters
 *  The members of this structure directly maps to a particular register of I2C
 */
typedef struct CSL_I2cConfig {
    /**  \brief I2C own address register - Holds the own slave address       */
    Uint16    icoar;
    /**  \brief I2C interrupt mask register - Enables/Disables interrupts    */
    Uint16    icimr;
    /**  \brief I2C clock low register - Clock divider low vale              */
    Uint16    icclkl;
    /**  \brief I2C clock high register - Clock divider high value           */
    Uint16    icclkh;
    /**  \brief I2C count register - Number of words to read/write           */
    Uint16    iccnt;
    /**  \brief I2C slave address register - Address of the slave device     */
    Uint16    icsar;
    /**  \brief I2C mode register - Mode of data transfer                    */
    Uint16    icmdr;
    /**  \brief I2C extended mode register - Enables extended mode feature   */
    Uint16    icemdr;
    /**  \brief I2C prescaler register - Value of the I2C prescaler          */
    Uint16    icpsc;
} CSL_I2cConfig;

/**
 *  \brief I2C Setup structure
 *
 *  This structure contains the I2C setup parameters.
 *  Using this structure individual parameters can be passed
 */
typedef struct CSL_I2cSetup {
    /**  \brief I2C address mode - 7bit/10bit                                */
    CSL_I2cAddrMode      addrMode;
    /**  \brief Number of bits in each transferred word                      */
    CSL_I2cBitCount      bitCount;
    /**  \brief Enables/Disables I2C data loopback mode                      */
    CSL_I2cLoopback      loopBack;
    /**  \brief Enables/Disables I2C free running mode                       */
    CSL_I2cFreeMode      freeMode;
    /**  \brief Enables/Disables I2C repeat mode                             */
    CSL_I2cRepeatMode    repeatMode;
    /**  \brief I2C own slave address - don't care if master                 */
    Uint16               ownAddr;
    /**  \brief value of the system clock (In MHz)                           */
    Uint32               sysInputClk;
    /**  \brief I2C bus frequency in KHz- a number between 10 and 400        */
    Uint32               i2cBusFreq;
} CSL_I2cSetup;

/**
 *  \brief I2C Object structure
 *
 *  This structure maintains information of I2C object
 */
typedef struct CSL_I2cObj {
    /** I2C register overlay structure pointer                               */
    CSL_I2cRegsOvly      i2cRegs;
    /** System control register overlay structure pointer                    */
    CSL_SysRegsOvly      sysCtrlRegs;
    /** I2C Isr dispatch table                                               */
    CSL_I2C_EVENT_ISR    I2C_isrDispatchTable[CSL_I2C_EVT_COUNT];
} CSL_I2cObj;

/** Handle to the I2C object structure                                       */
typedef CSL_I2cObj *pI2cHandle;

/**
@} */


/**************************************************************************\
* I2C function declarations                                                 *
\**************************************************************************/

/** @addtogroup CSL_I2C_FUNCTION
 @{ */

/** ============================================================================
 *   @n@b I2C_init
 *
 *   @b Description
 *   @n Initializes the I2C CSL module
 *
 *   @b Arguments
 *   @verbatim
            instanceNum      I2C Hardware instance number
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
 *   @n  Initializes I2C module
 *
 *   @b Modifies
 *   @n I2C Object structure
 *
 *   @b Example
 *   @verbatim
            CSL_Status    status;
            status = I2C_init(CSL_I2C0);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_init(Uint16    instanceNum);


/** ============================================================================
 *   @n@b I2C_config
 *
 *   @b Description
 *   @n Configures the I2C module. Using this function application can pass the
 *   value of the each register to be configured.
 *
 *   @b Arguments
 *   @verbatim
            i2cConfig      I2C config structure pointer
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Configures the I2C registers
 *
 *   @b Modifies
 *   @n I2C registers
 *
 *   @b Example
 *   @verbatim
            CSL_Status       status;
            CSL_I2cConfig    i2cConfig;

            i2cConfig.icoar  = CSL_I2C_ICOAR_DEFVAL;
            i2cConfig.icimr  = CSL_I2C_ICIMR_DEFVAL;
            i2cConfig.icclkl = CSL_I2C_ICCLK_DEFVAL;
            i2cConfig.icclkh = CSL_I2C_ICCLK_DEFVAL;
            i2cConfig.iccnt  = CSL_I2C_DATA_SIZE;
            i2cConfig.icsar  = CSL_I2C_ICSAR_DEFVAL;
            i2cConfig.icmdr  = CSL_I2C_ICMDR_WRITE_DEFVAL;
            i2cConfig.icemdr = CSL_I2C_ICEMDR_DEFVAL;
            i2cConfig.icpsc  = CSL_I2C_ICPSC_DEFVAL;

            status = I2C_init(CSL_I2C0);
            ....
            status = I2C_config(&i2cConfig);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_config(CSL_I2cConfig    *i2cConfig);


/** ============================================================================
 *   @n@b I2C_getConfig
 *
 *   @b Description
 *   @n Gets the I2C configuration parameters
 *
 *   @b Arguments
 *   @verbatim
            i2cgetConfig      I2C Configuration structure pointer
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_config should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Populates the I2C config structure
 *
 *   @b Modifies
 *   @n Structure pointed by getI2cConfig
 *
 *   @b Example
 *   @verbatim
            CSL_Status       status;
            CSL_I2cConfig    i2cConfig;
            CSL_I2cConfig    i2cgetConfig

            i2cConfig.icoar  = CSL_I2C_ICOAR_DEFVAL;
            i2cConfig.icimr  = CSL_I2C_ICIMR_DEFVAL;
            i2cConfig.icclkl = CSL_I2C_ICCLK_DEFVAL;
            i2cConfig.icclkh = CSL_I2C_ICCLK_DEFVAL;
            i2cConfig.iccnt  = CSL_I2C_DATA_SIZE;
            i2cConfig.icsar  = CSL_I2C_ICSAR_DEFVAL;
            i2cConfig.icmdr  = CSL_I2C_ICMDR_WRITE_DEFVAL;
            i2cConfig.icemdr = CSL_I2C_ICEMDR_DEFVAL;
            i2cConfig.icpsc  = CSL_I2C_ICPSC_DEFVAL;

            status = I2C_init(CSL_I2C0);
            ....
            status = I2C_config(&i2cConfig);
            ....
            ....
            status = I2C_getConfig(&i2cgetConfig);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_getConfig(CSL_I2cConfig    *i2cgetConfig);


/** ============================================================================
 *   @n@b I2C_setup
 *
 *   @b Description
 *   @n Sets the I2C hardware registers.
 *      This functions task is same as that of I2C_config, but this function
 *      provides more flexible interface to the application. Application can
 *      pass the individual configuration values rather than the register value.
 *
 *   @b Arguments
 *   @verbatim
            i2cSetup      I2C setup structure
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Sets the I2C register values
 *
 *   @b Modifies
 *   @n I2C registers
 *
 *   @b Example
 *   @verbatim
            #define CSL_I2C_SYS_CLK        (12)    // In MHz
            #define CSL_I2C_BUS_FREQ       (10)    // In KHz

            CSL_Status       status;
            CSL_I2cSetup     i2cSetup;

            i2cSetup.addrMode    = CSL_I2C_ADDR_7BIT;
            i2cSetup.bitCount    = CSL_I2C_BC_8BITS;
            i2cSetup.loopBack    = CSL_I2C_LOOPBACK_DISABLE;
            i2cSetup.freeMode    = CSL_I2C_FREEMODE_DISABLE;
            i2cSetup.repeatMode  = CSL_I2C_REPEATMODE_DISABLE;
            i2cSetup.ownAddr     = CSL_I2C_OWN_ADDR;
            i2cSetup.sysInputClk = CSL_I2C_SYS_CLK;
            i2cSetup.i2cBusFreq  = CSL_I2C_BUS_FREQ;

            status = I2C_init(CSL_I2C0);
            ....
            status = I2C_setup(&i2cSetup);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_setup(CSL_I2cSetup    *i2cSetup);


/** ============================================================================
 *   @n@b I2C_write
 *
 *   @b Description
 *   @n Transmits the I2C data to I2C device.
 *
 *   @b Arguments
 *   @verbatim
            i2cWrBuf           I2C Data buffer pointer
            dataLength         Number bytes to transmit
            slaveAddr          Address of the slave device
            masterMode         Master mode flag
            startStopFlag      Flag for the start and stop bits
            timeout            Time out variable
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK               - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS    - Invalid parameter
 *   @li                    CSL_I2C_BUS_BUSY_ERR  - Busy Bit Error
 *   @li                    CSL_I2C_TIMEOUT_ERROR - Time out Error has occured
 *   @li                    CSL_I2C_NACK_ERR      - No acknowledgement Error
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init and I2C_config/I2C_setup should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Transmits the I2C data
 *
 *   @b Modifies
 *   @n I2C transmit register
 *
 *   @b Example
 *   @verbatim
            #define CSL_I2C_SYS_CLK          (12)    // In MHz
            #define CSL_I2C_BUS_FREQ         (10)    // In KHz
            #define CSL_I2C_DATA_SIZE        (16)
            #define CSL_I2C_EEPROM_ADDR      (0x50)

            Uint16           i2cDataBuf[CSL_I2C_DATA_SIZE];
            CSL_Status       status;
            CSL_I2cSetup     i2cSetup;

            i2cSetup.addrMode    = CSL_I2C_ADDR_7BIT;
            i2cSetup.bitCount    = CSL_I2C_BC_8BITS;
            i2cSetup.loopBack    = CSL_I2C_LOOPBACK_DISABLE;
            i2cSetup.freeMode    = CSL_I2C_FREEMODE_DISABLE;
            i2cSetup.repeatMode  = CSL_I2C_REPEATMODE_DISABLE;
            i2cSetup.ownAddr     = CSL_I2C_OWN_ADDR;
            i2cSetup.sysInputClk = CSL_I2C_SYS_CLK;
            i2cSetup.i2cBusFreq  = CSL_I2C_BUS_FREQ;

            status = I2C_init(CSL_I2C0);
            ....
            status = I2C_setup(&i2cSetup);
            ....
            ....
            status = I2C_write(i2cDataBuf, CSL_I2C_DATA_SIZE,
                               CSL_I2C_EEPROM_ADDR, TRUE, 1,
                               CSL_I2C_MAX_TIMEOUT);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_write(Uint16    *i2cWrBuf,
                     Uint16    dataLength,
                     Uint16    slaveAddr,
                     Bool      masterMode,
                     Uint16    startStopFlag,
                     Uint16    timeout);


/** ============================================================================
 *   @n@b I2C_read
 *
 *   @b Description
 *   @n Receives the I2C data from the I2C device.
 *
 *   @b Arguments
 *   @verbatim
            i2cRdBuf           I2C Data buffer pointer
            dataLength         Number bytes to receive
            slaveAddr          Address of the slave device
            masterMode         Master mode flag
            startStopFlag      Flag for the start and stop bits
            timeout            Time out variable
            checkBus           Check Bus busy flag
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li       CSL_SOK                      - Returned for success
 *   @li       CSL_ESYS_INVPARAMS           - Invalid parameter
 *   @li       CSL_I2C_BUS_BUSY_ERR         - Busy Bit Error
 *   @li       CSL_I2C_TIMEOUT_ERROR        - Time out Error has occured
 *   @li       CSL_I2C_RECEIVE_OVERFLOW_ERR - Recieve Register over flow error
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init and I2C_config/I2C_setup should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Receives the I2C data
 *
 *   @b Modifies
 *   @n I2C data buffer
 *
 *   @b Example
 *   @verbatim
            #define CSL_I2C_SYS_CLK          (12)    // In MHz
            #define CSL_I2C_BUS_FREQ         (10)    // In KHz
            #define CSL_I2C_DATA_SIZE        (16)
            #define CSL_I2C_EEPROM_ADDR      (0x50)
            #define CSL_I2CEEPROM_ADDR_SIZE  (2)

            Uint16           i2cDataBuf[CSL_I2C_DATA_SIZE];
            CSL_Status       status;
            CSL_I2cSetup     i2cSetup;

            i2cSetup.addrMode    = CSL_I2C_ADDR_7BIT;
            i2cSetup.bitCount    = CSL_I2C_BC_8BITS;
            i2cSetup.loopBack    = CSL_I2C_LOOPBACK_DISABLE;
            i2cSetup.freeMode    = CSL_I2C_FREEMODE_DISABLE;
            i2cSetup.repeatMode  = CSL_I2C_REPEATMODE_DISABLE;
            i2cSetup.ownAddr     = CSL_I2C_OWN_ADDR;
            i2cSetup.sysInputClk = CSL_I2C_SYS_CLK;
            i2cSetup.i2cBusFreq  = CSL_I2C_BUS_FREQ;

            status = I2C_init(CSL_I2C0);
            ....
            status = I2C_setup(&i2cSetup);
            ....
            ....
            status = I2C_read(i2cDataBuf, CSL_I2C_DATA_SIZE, CSL_I2C_EEPROM_ADDR,
                              TRUE, 1, CSL_I2C_MAX_TIMEOUT, FALSE);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_read(Uint16    *i2cRdBuf,
                    Uint16    dataLength,
                    Uint16    slaveAddr,
                    Bool      masterMode,
                    Uint16    startStopFlag,
                    Uint16    timeout,
                    Bool      checkBus);


/** ============================================================================
 *   @n@b I2C_eventEnable
 *
 *   @b Description
 *   @n Enables the I2C events for interrupt generation.
 *      I2C has several events which can generate interrupt to the CPU.
 *      Application can enable any of the event using this function.
 *
 *   @b Arguments
 *   @verbatim
            i2cEvent      Event to be enabled
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Enables I2C interrupt for a particular event
 *
 *   @b Modifies
 *   @n I2C Interrupt enable register
 *
 *   @b Example
 *   @verbatim
            CSL_Status       status;

            status = I2C_init(CSL_I2C0);
            ....
            ....
            status = I2C_eventEnable(CSL_I2C_EVENT_ICXRDY);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_eventEnable(CSL_I2cEvent    i2cEvent);


/** ============================================================================
 *   @n@b I2C_eventDisable
 *
 *   @b Description
 *   @n Disables the I2C events.
 *      I2C has several events which can generate interrupt to the CPU.
 *      Application can disable any of the event using this function.
 *
 *   @b Arguments
 *   @verbatim
            i2cEvent      Event to be disabled
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK            - Returned for success
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  I2C_init and I2C_eventEnable should be called successfully
 *
 *   <b> Post Condition </b>
 *   @n  Disables I2C interrupt for a particular event
 *
 *   @b Modifies
 *   @n I2C Interrupt enable register
 *
 *   @b Example
 *   @verbatim
            CSL_Status       status;

            status = I2C_init(CSL_I2C0);
            ....
            ....
            status = I2C_eventEnable(CSL_I2C_EVENT_ICXRDY);
            ....
            ....
            status = I2C_eventDisable(CSL_I2C_EVENT_ICXRDY);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_eventDisable(CSL_I2cEvent    i2cEvent);


/** ============================================================================
 *   @n@b I2C_setCallback
 *
 *   @b Description
 *   @n Sets the call back functions for different I2C interrupts.
 *      These callback functions are called from the Interrupt service
 *      routine based on the interrupt generated by a particular event
 *
 *   @b Arguments
 *   @verbatim
            i2cIsrAddr      I2C ISR address structure pointer
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
 *   @n  Sets the call functions
 *
 *   @b Modifies
 *   @n Callback pointer array
 *
 *   @b Example
 *   @verbatim
            void alCallback(void);      // Arbitration Loss event callback
            void nackCallback(void);    // No acknowledgement event callback
            void ardyCallback(void);    // register access ready event callback
            void rrdyCallback(void);    // receive data ready event callback
            void xrdyCallback(void);    // transmit data ready event callback
            void scdCallback(void);     // stop condition detected event callback
            void aasCallback(void);     // Address As Slave interrupt callback

            CSL_I2cIsrAddr    i2cIsrAddr;
            CSL_Status        status;

            i2cIsrAddr.alAddr   = alCallback;
            i2cIsrAddr.nackAddr = nackCallback;
            i2cIsrAddr.ardyAddr = ardyCallback;
            i2cIsrAddr.rrdyAddr = rrdyCallback;
            i2cIsrAddr.xrdyAddr = xrdyCallback;
            i2cIsrAddr.scdAddr  = scdCallback;
            i2cIsrAddr.aasAddr  = aasCallback;

            status = I2C_setCallback(i2cIsrAddr);
     @endverbatim
 *  ============================================================================
 */
CSL_Status I2C_setCallback(CSL_I2cIsrAddr    *i2cIsrAddr);


/** ============================================================================
 *   @n@b I2C_getEventId
 *
 *   @b Description
 *   @n Returns the I2C event ID. This event represent the I2C interrupt
 *      in the I2C interrupt vector register
 *
 *   @b Arguments
 *   @verbatim
            None
     @endverbatim
 *
 *   <b> Return Value </b>
 *   @n  I2C Event Id
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  Returns I2C event Id
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
            Int16    i2cEvent;

            i2cEvent = I2C_getEventId();
     @endverbatim
 *  ============================================================================
 */
Int16 I2C_getEventId(void);


/**
@} */



#ifdef __cplusplus
}
#endif

#endif    //_CSL_I2C_H_

