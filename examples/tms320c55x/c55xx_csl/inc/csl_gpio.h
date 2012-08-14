/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_gpio.h
 *
 *  @brief GPIO functional layer API header file
 *
 *  Path: \\(CSLPATH)\\inc
 *
 */

/* ============================================================================
 * Revision History
 * ================
 * 22-Sept-2008 - Added GPIO for CSL
 * ============================================================================
 */

/** @defgroup CSL_GPIO_API GPIO
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 * GPIO peripheral provides general purpose pin that can be configured as
 * input or output.The purpose of this document is to provide a set of
 * CSL API for GPIO module
 *
 * @subsection References
 *    -# C5505_Spec_1.16.pdf
 *
 */

#ifndef _CSL_GPIO_H_
#define _CSL_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cslr.h>
#include <soc.h>
#include <cslr_gpio.h>
#include <csl_error.h>
#include <csl_types.h>
#include <csl_general.h>


/**
@defgroup CSL_GPIO_SYMBOL  GPIO Symbols Defined
@ingroup CSL_GPIO_API
*/
/**
@defgroup CSL_GPIO_DATASTRUCT  GPIO Data Structures
@ingroup CSL_GPIO_API
*/
/**
@defgroup CSL_GPIO_FUNCTION  GPIO Functions
@ingroup CSL_GPIO_API
*/
/**
@defgroup CSL_GPIO_ENUM GPIO Enumerated Data Types
@ingroup CSL_GPIO_API
*/

/**
@addtogroup CSL_GPIO_SYMBOL
@{
*/
/*****************************************************************************\
          GPIO global macro declarations
\*****************************************************************************/

/** Number of GPIO pins */
#define CSL_GPIO_NUM_PIN  32

/** Set the single bit */
#define CSL_GPIO_BIT_SET 1

/** Reset the single bit */
#define CSL_GPIO_BIT_RESET 0

/** Drive GPIO Pin o/p High */
#define GPIO_DRIVE_HIGH    1
/** Drive GPIO Pin o/p Low */
#define GPIO_DRIVE_LOW     0

/**
@} */

/**************************************************************************\
* GPIO global typedef declarations                                         *
\**************************************************************************/

/** @addtogroup CSL_GPIO_ENUM
 @{ */



/** Enumeration used for specifying the GPIO pin numbers */
typedef enum {
    /** Gpio pin 0 */
    CSL_GPIO_PIN0 = 0,
    /** Gpio pin 1 */
    CSL_GPIO_PIN1 =1,
    /** Gpio pin 2 */
    CSL_GPIO_PIN2 = 2,
    /** Gpio pin 3 */
    CSL_GPIO_PIN3 = 3,
    /** Gpio pin 4 */
    CSL_GPIO_PIN4 = 4,
    /** Gpio pin 5 */
    CSL_GPIO_PIN5 = 5,
    /** Gpio pin 6 */
    CSL_GPIO_PIN6 = 6,
    /** Gpio pin 7 */
    CSL_GPIO_PIN7 = 7,
    /** Gpio pin 8 */
    CSL_GPIO_PIN8 = 8,
    /** Gpio pin 9 */
    CSL_GPIO_PIN9 = 9,
    /** Gpio pin 10 */
    CSL_GPIO_PIN10 = 10,
    /** Gpio pin 11 */
    CSL_GPIO_PIN11 = 11,
    /** Gpio pin 12 */
    CSL_GPIO_PIN12 = 12,
    /** Gpio pin 13 */
    CSL_GPIO_PIN13 = 13,
    /** Gpio pin 14 */
    CSL_GPIO_PIN14 = 14,
    /** Gpio pin 15 */
    CSL_GPIO_PIN15 = 15,
    /** Gpio pin 16 */
    CSL_GPIO_PIN16 = 16,
    /** Gpio pin 17 */
    CSL_GPIO_PIN17 = 17,
    /** Gpio pin 18 */
    CSL_GPIO_PIN18 = 18,
    /** Gpio pin 19 */
    CSL_GPIO_PIN19 = 19,
    /** Gpio pin 20 */
    CSL_GPIO_PIN20 = 20,
    /** Gpio pin 21 */
    CSL_GPIO_PIN21 = 21,
    /** Gpio pin 22 */
    CSL_GPIO_PIN22 = 22,
    /** Gpio pin 23 */
    CSL_GPIO_PIN23 = 23,
    /** Gpio pin 24 */
    CSL_GPIO_PIN24 = 24,
    /** Gpio pin 25 */
    CSL_GPIO_PIN25 = 25,
    /** Gpio pin 26 */
    CSL_GPIO_PIN26 = 26,
    /** Gpio pin 27 */
    CSL_GPIO_PIN27 = 27,
    /** Gpio pin 28 = */
    CSL_GPIO_PIN28 = 28,
    /** Gpio pin 29 */
    CSL_GPIO_PIN29 = 29,
    /** Gpio pin 30 */
    CSL_GPIO_PIN30 = 30,
    /** Gpio pin 15 */
    CSL_GPIO_PIN31 = 31
} CSL_GpioPinNum;


/** \brief  Enums for configuring GPIO pin edge detection
 *
 */
typedef enum {

    /** Rising edge detection    */
    CSL_GPIO_TRIG_RISING_EDGE = 0,

    /** Falling edge detection   */
    CSL_GPIO_TRIG_FALLING_EDGE = 1,

    /** No edge detection       */
    CSL_GPIO_TRIG_CLEAR_EDGE = 2

} CSL_GpioTriggerType;


/** \brief  Enumeration for GPIO General Purpose Output Data */
typedef enum {

    /** GPIO as i/p          */
    CSL_GPIO_DIR_INPUT = 0,
    /** GPIO as o/p           */
    CSL_GPIO_DIR_OUTPUT = 1

} CSL_GpioDirection;


/** \brief  Enumeration for GPIO Control       */
typedef enum {

    /**  Configure the pin - 1) Pin Direction
                             2) Interrupt Polarity  */
    CSL_GPIO_CMD_CONFIG_BIT = 0,
    /**  Disable Interrupt for the particular pin   */
    CSL_GPIO_CMD_INT_DISABLE = 1,
    /**  Enable Interrupt for particular pin        */
    CSL_GPIO_CMD_INT_ENABLE = 2,
    /** Clear the interrupt flag for the particular
    interrrupt pin                                  */
    CSL_GPIO_CMD_CLEAR_INT_FLAG = 3

} CSL_GpioConfigCmd;

/**
@} */

/**
\addtogroup CSL_GPIO_DATASTRUCT
@{ */



/**
 * @brief Config structure of GPIO. This is used to setup and get
 * values of GPIO Registers using GPIO_config and GPIO_getConfig
 * function
 */
typedef struct  {

    /** GPIO Data Direction Register LSW         */
    volatile Uint16 GPIODIRL;
    /** GPIO Data Direction Register MSW         */
    volatile Uint16 GPIODIRH;

    /** GPIO Interrupt Edge Trigger Register LSW */
    volatile Uint16 GPIOINTTRIGL;
    /** GPIO Interrupt Edge Trigger Register MSW */
    volatile Uint16 GPIOINTTRIGH;

    /** GPIO Interrupt Enable Register LSW       */
    volatile Uint16 GPIOINTENAL;
    /** GPIO Interrupt Enable Register MSW       */
    volatile Uint16 GPIOINTENAH;

}CSL_GpioConfig;


/**
 *  \brief Input parameters for configuring a GPIO pin
 *  This is used to configure the direction and edge detection
 */
typedef struct {

    /** Pin number for GPIO     */
    CSL_GpioPinNum pinNum;

    /** Direction for GPIO Pin  */
    CSL_GpioDirection direction;

    /** GPIO pin edge detection */
    CSL_GpioTriggerType trigger;

} CSL_GpioPinConfig;

/**
 *  \brief structure parameters for GPIO Handle
 */
typedef struct{

    /** \brief BaseAddress of GPIO Registers */
    CSL_GpioRegsOvly baseAddr;

    /** \brief This is the maximum number of pins supported by this
    *   instance of GPIO
    */
    Uint8 numPins;

}CSL_GpioObj;

/** \brief this is a pointer to @a CSL_GpioObj and is passed as the first
 * parameter to all GPIO CSL APIs
 */
typedef CSL_GpioObj*  GPIO_Handle;

/**
@} */

/*******************************************************************************
 * GPIO function declarations
 ******************************************************************************/

/** @addtogroup CSL_GPIO_FUNCTION
 @{ */


/** ============================================================================
 *   @n@b GPIO_Open
 *
 *   @b Description
 *   @n This function returns the handle to the GPIO module,
 *      This handle is passed to all other CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
           GpioObj  -  GPIO object structure Pointer
           status   -  Pointer to CSL_Status
 *   @endverbatim
 *
 *   <b> Return Value </b>  GPIO_Handle
 *   @n                     Valid GPIO handle will be returned
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n  This API call should return a valid handle to GPIO module
 *
 *
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. GPIO object structure Pointer
 *
 *   @b Example
 *   @verbatim
            CSL_GpioObj           GpioObj
            GPIO_Handle           hGPIO;
            CSL_Status            *status;
            ...

            hGPIO = GPIO_open(&GpioObj,&status);
            ...

    @endverbatim
 *  ============================================================================
 */
GPIO_Handle GPIO_open (
CSL_GpioObj *GPIOobj,
CSL_Status  *status
 );

/** ============================================================================
 *   @n@b GPIO_close
 *
 *   @b Description
 *   @n This function closes the handle .
 *
 *   @b Arguments
 *   @verbatim
            hGPIO            Handle to the GPIO
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - GPIO_close is successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  GPIO_open API should be called before calling this API call
 *
 *   <b> Post Condition </b>
 *   @n  The handler will be closed
 *
 *   @b Modifies
 *   @n    1. hGPIO - GPIO handle
 *   @b Example
 *   @verbatim
            GPIO_Handle     hGPIO;
            CSL_status       status;
            ...

            status = GPIO_close(hGPIO);
     @endverbatim
 *  ============================================================================
 */
CSL_Status GPIO_close (
GPIO_Handle       hGPIO
);


/** ============================================================================
 *   @n@b GPIO_config
 *
 *   @b Description
 *   @n It configures the GPIO Registers according to the corresponding
 *     config structure.
 *   @b Arguments
 *   @verbatim
            GPIOHandle      Handle to GPIO module

            config          Config structure

 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_OK             - GPIO_config call is successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  GPIO_open API should be called before this API call
 *
 *   <b> Post Condition </b>
 *   @n GPIO Registers will be configured according to config structure values
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. GPIO Registers
 *
 *   @b Example
 *   @verbatim
         CSL_Status     status;
         CSL_GpioObj    GpioObj;
         GPIO_Handle    GPIOHandle ;
         CSL_GpioConfig config;
         config.GPIODIRL = 0x0011;
         config.GPIODIRH = 0x0101;
         config.GPIOINTTRIGL = 0x0101;
         config.GPIOINTTRIGH = 0x0010;
         config.GPIOINTENAL = 0x1100;
         config.GPIOINTENAH = 0x1010;
         ...
         GPIOHandle = GPIO_open (&GpioObj,&status);
         if(CSL_SOK == status){
         ...
         status = GPIO_config(GPIOHandle,&config);
         }
         ...
    @endverbatim
 *  ============================================================================
 */
CSL_Status GPIO_config (
GPIO_Handle                     GPIOHandle,
CSL_GpioConfig                 *config
);



/** ============================================================================
 *   @n@b GPIO_getConfig
 *
 *   @b Description
 *   @n It stores the GPIO H/w Registers values in GPIO config structure
 *
 *   @b Arguments
 *   @verbatim
            GPIOHandle      Handle to GPIO module

            config          Config structure

 *   @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_OK             - GPIO_getConfig call is
 *                                               successful
 *   @li                    CSL_ESYS_BADHANDLE - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  GPIO_open API should be called before this API call
 *
 *   <b> Post Condition </b>
 *   @n GPIO Registers values will be populated in config structure
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. config structure
 *
 *   @b Example
 *   @verbatim
         CSL_Status     status;
         CSL_GpioObj    GpioObj;
         GPIO_Handle    GPIOHandle ;
         CSL_GpioConfig config;
         ...
         GPIOHandle = GPIO_open (&GpioObj,&status);
         if(CSL_SOK == status){
         ...
         status = GPIO_getConfig(GPIOHandle,&config);
         }
         ...
    @endverbatim
 *  ============================================================================
 */
CSL_Status  GPIO_getConfig(
GPIO_Handle       hGpio,
CSL_GpioConfig    *config
);

/** ============================================================================
 *   @n@b GPIO_read
 *
 *   @b Description
 *   @n It reads the pin value - high or low
 *
 *   @b Arguments
 *   @verbatim
            hGPIO        Handle to the GPIO

            readBuffer   Pointer to the output buffer
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - GPIO_read is successfull
 *
 *   @li                    CSL_ESYS_BADHANDLE  - The handle is passed is
 *                                                invalid
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  GPIO_open and GPIO_config or(GPIO_configBit) should be called
 *       before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  The output value of pin will be populated in the output buffer
 *       passed in the API
 *
 *   @b Modifies
 *   @n buffer variable
 *
 *   @b Example
 *   @verbatim
        CSL_GpioObj     GpioObj;
        GPIO_Handle     hGPIO;
        CSL_GpioPinConfig config;
        CSL_GpioPinNum pinNum;
        Uint16   readBuffer;
        ....
        hGPIO = GPIO_open(&GpioObj,&status);
        ....
        config.pinNum = CSL_GPIO_PIN1;
        config.direction = CSL_GPIO_DIR_OUTPUT;
        config.trigger  = CSL_GPIO_TRIG_FALLING_EDGE;
        GPIO_configBit (hGPIO, &config);
        ...
        pinNum = config.pinNum;
        ....
       status = GPIO_read(hGPIO,pinNum,&readBuffer);
     @endverbatim
 *  ============================================================================
 */
CSL_Status GPIO_read (
GPIO_Handle     hGPIO,
CSL_GpioPinNum  pinNum,
Uint16          *buffer
);

/** ============================================================================
 *   @n@b GPIO_write
 *
 *   @b Description
 *   @n It writes value to the o/p pin  - high or low
 *
 *   @b Arguments
 *   @verbatim
            hGPIO        Handle to the GPIO

            writeBuffer   Pointer to the output buffer
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - GPIO_write is successfull
 *   @li                    CSL_ESYS_BADHANDLE  - The handle is passed is
 *                                                invalid
 *   @li                    CSL_ESYS_INVPARAMS - Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n GPIO_open and GPIO_config or (GPIO_configBit) should be called
 *      before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  It drives the o/p pin high or low according to writeBuffer
 *
 *   @b Modifies
 *   @n GPIO DATA OUT Registers
 *
 *   @b Example
 *   @verbatim
        CSL_GpioObj     GpioObj;
        GPIO_Handle     hGPIO;
        CSL_GpioPinConfig config;
        CSL_GpioPinNum pinNum;
        Uint16   writeBuffer = 1;
        ....
        hGPIO = GPIO_open(&GpioObj,&status);
        ....
        config.pinNum = CSL_GPIO_PIN1;
        config.direction = CSL_GPIO_DIR_OUTPUT;
        config.trigger  = CSL_GPIO_TRIG_FALLING_EDGE;
        GPIO_configBit (hGPIO, &config);
        ...
        pinNum = config.pinNum;
        ....

        status = GPIO_write(hGPIO,pinNum, writeBuffer);
     @endverbatim
 *  ============================================================================
 */
CSL_Status GPIO_write (
GPIO_Handle     hGPIO,
CSL_GpioPinNum  pinNum,
Uint16          buffer
);

/** ============================================================================
 *   @n@b GPIO_reset
 *
 *   @b Description
 *   @n It reset the given pin - 1) set pin dir as input
 *                               2) Disable the interrupt
 *
 *   @b Arguments
 *   @verbatim
            hGPIO         Handle to the GPIO

            PinNumber     GPIO pin number
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - GPIO_write is successfull
 *   @li                    CSL_ESYS_BADHANDLE  - The handle is passed is
 *                                                invalid
 *   @li                    CSL_ESYS_INVPARAMS -  Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n GPIO_open should be called before calling this API
 *
 *   <b> Post Condition </b>
 *   @n  It sets the default value in the Registers
 *
 *   @b Modifies
 *   @n GPIO Direction,GPIO Interrupt Enable and
 *      GPIO Interrupt Flag Registers
 *   @b Example
 *   @verbatim
            CSL_GpioObj    GpioObj;
            GPIO_Handle     hGPIO;
            CSL_GpioPinNum  pinNum;

            hGPIO = GPIO_open(&GpioObj,&status);
            if(CSL_SOK == status){
            ...

            status = GPIO_reset(hGPIO,pinNum);
            }
     @endverbatim
 *  ============================================================================
 */
CSL_Status GPIO_reset (
GPIO_Handle     hGPIO
);


/** ===========================================================================
 *   @n@b GPIO_configBit
 *
 *   @b Description
 *   @n This function used to configure the direction and rising and falling
        edge trigger GPIO registers.
 *
 *   @b Arguments
 *   @verbatim

            hGpio            Handle to GPIO instance
            config           Pointer to the CSL_GpioPinConfig structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK         - GPIO_configBit is successfull
 *   @li                    CSL_ESYS_BADHANDLE  - The handle is passed is
 *                                                invalid
 *   @li                    CSL_ESYS_INVPARAMS -  Invalid parameter
 *
 *
 *   <b> Pre Condition </b>
 *   @n  GPIO_open should be called before calling this API
 *
 *   <b> Post Condition </b>
 *    @n Configures any of the 32 GPIO signals
 *
 *   @b Modifies
 *   @n GPIO Direction Register,GPIO Edge triggerInterrupt Registers
 *
 *   @b Example
 *   @verbatim
        CSL_GpioObj     GpioObj;
        GPIO_Handle     hGPIO;
        CSL_GpioPinConfig config;
        ....
        hGPIO = GPIO_open(&GpioObj,&status);
        ....
        config.pinNum = CSL_GPIO_PIN1;
        config.direction = CSL_GPIO_DIR_OUTPUT;
        config.trigger  = CSL_GPIO_TRIG_FALLING_EDGE;
        GPIO_configBit (hGPIO, &config);


     @endverbatim
 * ============================================================================
 */
CSL_Status GPIO_configBit (
GPIO_Handle          hGpio,
CSL_GpioPinConfig *config
);

/** ===========================================================================
 *   @n@b GPIO_disableInt
 *
 *   @b Description
 *   @n This function disables the GPIO interrupt for the pin .
 *
 *   @b Arguments
 *   @verbatim

            hGPIO            Handle to GPIO instance.
            pinNum           Pin Number
     @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *    @n                CSL_SOK - GPIO_disableInt is successfull
 *    @n                CSL_ESYS_BADHANDLE  - Invalid Handle
 *    @n                CSL_ESYS_INVPARAMS  - Invalid Parameters
 *   <b> Pre Condition </b>
 *   @n  GPIO_open and GPIO_configBit should be called before
 *       calling this API
 *
 *   <b> Post Condition </b>
 *    @n Interrupt will be disabled
 *
 *   @b Modifies
 *   @n  GPIO Interrupt Enable Register
 *
 *   @b Example
 *   @verbatim
        CSL_GpioObj     GpioObj;
        GPIO_Handle     hGPIO;
        CSL_GpioPinConfig config;
        ....
        hGPIO = GPIO_open(&GpioObj,&status);
        ....
        config.pinNum = CSL_GPIO_PIN1;
        config.direction = CSL_GPIO_DIR_OUTPUT;
        config.trigger  = CSL_GPIO_TRIG_FALLING_EDGE;
        GPIO_configBit (hGPIO, &config);
        pinNum = config.pinNum;
        GPIO_disableInt(hGPIO,pinNum);


     @endverbatim
 * ============================================================================
 */
CSL_Status GPIO_disableInt (
GPIO_Handle    hGpio,
CSL_GpioPinNum pin
);


/** ===========================================================================
 *   @n@b GPIO_enableInt
 *
 *   @b Description
 *   @n This function enables the GPIO interrupt .
 *
 *   @b Arguments
 *   @verbatim

            hGPIO            Handle to GPIO instance.
            pinNum           Pin Number
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *    @n                CSL_SOK - GPIO_enableInt is successfull
 *    @n                CSL_ESYS_BADHANDLE  - Invalid Handle
 *    @n                CSL_ESYS_INVPARAMS  - Invalid Parameters
 *
 *   <b> Pre Condition </b>
 *   @n  GPIO_open and GPIO_configBit should be called before
 *       calling this API
 *
 *   <b> Post Condition </b>
 *    @n Interrupt will be enabled
 *
 *   @b Modifies
 *   @n  GPIO Interrupt Enable Register
 *
 *   @b Example
 *   @verbatim
        CSL_GpioObj     GpioObj;
        GPIO_Handle     hGPIO;
        CSL_GpioPinConfig config;
        ....
        hGPIO = GPIO_open(&GpioObj,&status);
        ....
        config.pinNum = CSL_GPIO_PIN1;
        config.direction = CSL_GPIO_DIR_OUTPUT;
        config.trigger  = CSL_GPIO_TRIG_FALLING_EDGE;
        GPIO_configBit (hGPIO, &config);
        .....
        pinNum = config.pinNum;
        GPIO_enableInt(hGpio,pinNum);

     @endverbatim
 * ============================================================================
 */

CSL_Status GPIO_enableInt (
GPIO_Handle      hGpio,
CSL_GpioPinNum   pin
);


/** ===========================================================================
 *   @n@b GPIO_clearInt
 *
 *   @b Description
 *   @n This function clears the GPIO interrupt flag bit for the
 *      particular pin .
 *
 *   @b Arguments
 *   @verbatim

            hGpio            Handle to GPIO instance.
            pinNum           Pin Number
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *    @n                CSL_SOK - GPIO_clearInt is successfull
 *    @n                CSL_ESYS_BADHANDLE  - Invalid Handle
 *    @n                CSL_ESYS_INVPARAMS  - Invalid Parameters

 *
 *   <b> Pre Condition </b>
 *   @n  GPIO_open and GPIO_configBit should be called before calling this API
 *
 *   <b> Post Condition </b>
 *    @n Interrupt will be cleared
 *
 *   @b Modifies
 *   @n  GPIO Interrupt Flag Register
 *
 *   @b Example
 *   @verbatim
        CSL_GpioObj     GpioObj;
        GPIO_Handle     hGPIO;
        CSL_GpioPinConfig config;
        ....
        hGPIO = GPIO_open(&GpioObj,&status);
        ....
        config.pinNum = CSL_GPIO_PIN1;
        config.direction = CSL_GPIO_DIR_OUTPUT;
        config.trigger  = CSL_GPIO_TRIG_FALLING_EDGE;
        GPIO_configBit (hGPIO, &config);
        .....
        pinNum = config.pinNum;

        ...
       GPIO_clearInt(hGPIO,pinNum);

     @endverbatim
 * ============================================================================
 */

CSL_Status GPIO_clearInt (
GPIO_Handle    hGpio,
CSL_GpioPinNum pin
);


/** ===========================================================================
 *   @n@b GPIO_statusBit
 *
 *   @b Description
 *   @n This function reads the GPIO interrupt flag bit for the
 *      particular pin .
 *
 *   @b Arguments
 *   @verbatim

            hGpio            Handle to GPIO instance.
            pinNum           Pin Number
     @endverbatim
 *
 *   <b> Return Value </b>
 *    @n Return the Interrupt Flag bit value for the pin 1 or 0
 *
 *   <b> Pre Condition </b>
 *   @n  GPIO_open and GPIO_configBit should be called before calling this API
 *
 *   <b> Post Condition </b>
 *    @n Reads the Interrupt Bit
 *
 *   @b Modifies
 *   @n 1 Status variable
 *   @n 2 Return value
 *
 *   @b Example
 *   @verbatim
        CSL_Status  status;
        int   flag;
        CSL_GpioObj     GpioObj;
        GPIO_Handle     hGPIO;
        CSL_GpioPinConfig config;
        ....
        hGPIO = GPIO_open(&GpioObj,&status);
        ....
        config.pinNum = CSL_GPIO_PIN1;
        config.direction = CSL_GPIO_DIR_OUTPUT;
        config.trigger  = CSL_GPIO_TRIG_FALLING_EDGE;
        GPIO_configBit (hGPIO, &config);
        .....
        pinNum = config.pinNum;
        GPIO_enableInt(hGpio,pinNum);
         ...
        flag = GPIO_statusBit(hGpio,pinNum);

     @endverbatim
 * ============================================================================
 */
int GPIO_statusBit (
GPIO_Handle    hGpio,
CSL_GpioPinNum pin,
CSL_Status    *status
);
/**
@} */

#ifdef __cplusplus
}
#endif

#endif /* _CSL_GPIO_H_ */

