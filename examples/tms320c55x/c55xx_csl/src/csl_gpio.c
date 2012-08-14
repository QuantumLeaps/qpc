/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_gpio.c
 *
 *  @brief GPIO functional layer API source file
 *
 *  Path: \\(CSLPATH)\\src
 */

/* ============================================================================
 * Revision History
 * ================

 * ============================================================================
 */
#include "csl_gpio.h"

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
CSL_GpioObj*     GpioObj,
CSL_Status *     status
)
{
    GPIO_Handle hGpio = (CSL_GpioObj*)NULL;
    /* For Invalid handle */
    if(NULL == GpioObj)
    {
        *status = CSL_ESYS_BADHANDLE;
        return NULL;
    }

    GpioObj->numPins  = CSL_GPIO_NUM_PIN;
    /* Base Address of GPIO Registers */
    GpioObj->baseAddr = CSL_GPIO_REGS;
    hGpio = (GPIO_Handle)GpioObj;
    *status = CSL_SOK;

    return hGpio;
}

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
CSL_Status  GPIO_close (
GPIO_Handle      hGpio
)
{
    /* For Invalid handle */
    if(NULL == hGpio)
    {
        return CSL_ESYS_BADHANDLE;
    }
    hGpio->baseAddr = NULL;
    hGpio->numPins = 0;

    return CSL_SOK;
}

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

CSL_Status  GPIO_config(
GPIO_Handle          hGpio,
CSL_GpioConfig    *config
)
{
    /* For Invalid handle */
    if(NULL == hGpio)
    {
        return CSL_ESYS_BADHANDLE;
    }
    if(NULL == config)
    {
        return CSL_ESYS_INVPARAMS;
    }
    /* GPIO Direction Register */
    hGpio->baseAddr->IODIR1 = config->GPIODIRL;
    hGpio->baseAddr->IODIR2 = config->GPIODIRH;

    hGpio->baseAddr->IOINTEDG1 = config->GPIOINTTRIGL;
    hGpio->baseAddr->IOINTEDG2 = config->GPIOINTTRIGH;

    hGpio->baseAddr->IOINTEN1 = config->GPIOINTENAL;
    hGpio->baseAddr->IOINTEN2 = config->GPIOINTENAH;

    return CSL_SOK;
}
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
)
{
    /* For Invalid handle */
    if(NULL == hGpio)
    {
        return CSL_ESYS_BADHANDLE;
    }
    /* Invalid config structure */
    if(NULL == config)
    {
        return CSL_ESYS_INVPARAMS;
    }
    /* GPIO Direction Registers */
    config->GPIODIRL =hGpio->baseAddr->IODIR1 ;
    config->GPIODIRH =hGpio->baseAddr->IODIR2 ;

    /* GPIO Interrupt Edge Trigger Registers */
    config->GPIOINTTRIGL =hGpio->baseAddr->IOINTEDG1 ;
    config->GPIOINTTRIGH =hGpio->baseAddr->IOINTEDG2 ;

    /* GPIO Interrupt Enable Registers */
    config->GPIOINTENAL =hGpio->baseAddr->IOINTEN1 ;
    config->GPIOINTENAH =hGpio->baseAddr->IOINTEN2 ;

    return CSL_SOK;
}

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
GPIO_Handle     hGpio,
CSL_GpioPinNum  pinNum,
Uint16          *buffer
)
{
    Uint16 pin;
    /* For Invalid handle */
    if(NULL == hGpio)
    {
        return CSL_ESYS_BADHANDLE;
    }
    /* For Invalid buffer and wrong Pin index */
    if((NULL == buffer)||(CSL_GPIO_PIN31 < pinNum)||(pinNum < CSL_GPIO_PIN0))
    {
        return CSL_ESYS_INVPARAMS;
    }

    if(pinNum < CSL_GPIO_PIN16)
    {
        /* Lower Sixteen GPIO Pin - 0 to 15 GPIO pin */
        pin = pinNum;
        *buffer = CSL_FEXTR (hGpio->baseAddr->IOINDATA1,pin,pin);
    }
    else
    {
        /* Upper Sixteen 16 to 31 GPIO Pin              */
        pin = pinNum - CSL_GPIO_PIN16;
        *buffer = CSL_FEXTR (hGpio->baseAddr->IOINDATA2,pin,pin);
    }
    return CSL_SOK;
}

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
GPIO_Handle     hGpio,
CSL_GpioPinNum  pinNum,
Uint16          buffer
)
{
    Uint16 pin;
    /* For Invalid handle */
    if(NULL == hGpio)
    {
        return CSL_ESYS_BADHANDLE;
    }
    /* For wrong Pin Index */
    if((CSL_GPIO_PIN31 < pinNum)||(pinNum < CSL_GPIO_PIN0))
    {
        return CSL_ESYS_INVPARAMS;
    }
    /* For wrong o/p write data */
    if(buffer != GPIO_DRIVE_HIGH && buffer != GPIO_DRIVE_LOW)
    {
        return CSL_ESYS_INVPARAMS;
    }

    if(pinNum < CSL_GPIO_PIN16)
    {
        /* Lower Sixteen GPIO Pin - 0 to 15 GPIO pin */
        pin = pinNum;
        CSL_FINSR (hGpio->baseAddr->IOOUTDATA1,pin,pin,buffer);
    }
    else
    {
        /* Upper Sixteen 16 to 31 GPIO Pin              */
        pin = pinNum - CSL_GPIO_PIN16;
        CSL_FINSR(hGpio->baseAddr->IOOUTDATA2,pin,pin,buffer);
    }
    return CSL_SOK;
}

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
 *   @li                    CSL_SOK            - GPIO_configBit is successfull
 *   @li                    CSL_ESYS_BADHANDLE - The handle is passed is
 *                                                invalid
 *   @li                    CSL_ESYS_INVPARAMS -  Invalid parameter
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
)
{
    Uint16 pinNum;
    /* For Invalid handle */
    if(NULL == hGpio)
    {
        return CSL_ESYS_BADHANDLE;
    }
    /* For wrong Pin Index */
    if((CSL_GPIO_PIN31 < config->pinNum)||(config->pinNum < CSL_GPIO_PIN0))
    {
        return CSL_ESYS_INVPARAMS;
    }
    /* Wrong Pin direction */
    if(config->direction != CSL_GPIO_DIR_INPUT && \
    config->direction != CSL_GPIO_DIR_OUTPUT)
    {
        return CSL_ESYS_INVPARAMS;
    }
    if(CSL_GPIO_TRIG_RISING_EDGE != config->trigger && \
    CSL_GPIO_TRIG_FALLING_EDGE != config->trigger && \
    CSL_GPIO_TRIG_CLEAR_EDGE != config->trigger)
    {
        return CSL_ESYS_INVPARAMS;
    }


    if (config->pinNum < CSL_GPIO_PIN16)
    {
        /* Lower Sixteen GPIO Pin - 0 to 15 GPIO pin */
        pinNum = config->pinNum;
        CSL_FINSR(hGpio->baseAddr->IODIR1,pinNum,pinNum,config->direction);

        if(CSL_GPIO_TRIG_FALLING_EDGE == config->trigger || \
        CSL_GPIO_TRIG_RISING_EDGE == config->trigger )
        {
            CSL_FINSR(hGpio->baseAddr->IOINTEDG1, pinNum, pinNum,\
            config->trigger);
        }
    }

    else
    {
        /* Upper Sixteen 16 to 31 GPIO Pin              */
        pinNum = config->pinNum - CSL_GPIO_PIN16;
        CSL_FINSR(hGpio->baseAddr->IODIR2,pinNum,pinNum,config->direction);

        if(CSL_GPIO_TRIG_FALLING_EDGE == config->trigger || \
        CSL_GPIO_TRIG_RISING_EDGE == config->trigger )
        {
            CSL_FINSR(hGpio->baseAddr->IOINTEDG2, pinNum, pinNum,\
            config->trigger);
        }
    }

    return CSL_SOK;
}

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
 *   <b> Return Value </b>
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
CSL_GpioPinNum pinNum
)
{
    Uint16 pin;
    /* For Invalid handle */
    if(NULL == hGpio)
    {
        return CSL_ESYS_BADHANDLE;
    }

    if((CSL_GPIO_PIN31 < pinNum)||(pinNum < CSL_GPIO_PIN0))
    {
        return CSL_ESYS_INVPARAMS;
    }

    if(pinNum < CSL_GPIO_PIN16)
    {
        /* Lower Sixteen GPIO Pin - 0 to 15 GPIO pin */
        pin = pinNum;
        CSL_FINSR(hGpio->baseAddr->IOINTEN1,pin,pin,CSL_GPIO_BIT_RESET);
    }
    else
    {
        /* Upper Sixteen 16 to 31 GPIO Pin              */
        pin = pinNum - CSL_GPIO_PIN16;
        CSL_FINSR(hGpio->baseAddr->IOINTEN2,pin,pin,CSL_GPIO_BIT_RESET);
    }

    return CSL_SOK;
}

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
CSL_GpioPinNum   pinNum
)
{
    Uint16 pin;
    /* For Invalid handle */
    if(NULL == hGpio)
    {
        return CSL_ESYS_BADHANDLE;
    }

    /* For wrong pin index */
    if((CSL_GPIO_PIN31 < pinNum) ||(pinNum < CSL_GPIO_PIN0))
    {
        return CSL_ESYS_INVPARAMS;
    }
    if(pinNum < CSL_GPIO_PIN16)
    {
        /* Lower Sixteen GPIO Pin - 0 to 15 GPIO pin */
        pin = pinNum;
        CSL_FINSR(hGpio->baseAddr->IOINTEN1,pin,pin,CSL_GPIO_BIT_SET);
    }
    else
    {
        /* Upper Sixteen 16 to 31 GPIO Pin              */
        pin = pinNum - CSL_GPIO_PIN16;
        CSL_FINSR(hGpio->baseAddr->IOINTEN2,pin,pin,CSL_GPIO_BIT_SET);
    }

    return CSL_SOK;
}

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
CSL_GpioPinNum pinNum
)
{
    Uint16 pin;
    /* For Invalid handle */
    if(NULL == hGpio)
    {
        return CSL_ESYS_BADHANDLE;
    }
    /* For wrong Pin Index */
    if((CSL_GPIO_PIN31 < pinNum)||(pinNum < CSL_GPIO_PIN0))
    {
        return CSL_ESYS_INVPARAMS;
    }
    if(pinNum < CSL_GPIO_PIN16)
    {
        /* Lower Sixteen GPIO Pin - 0 to 15 GPIO pin */
        pin = pinNum;
        CSL_FINSR(hGpio->baseAddr->IOINTFLG1,pin,pin,CSL_GPIO_BIT_SET);
    }
    else
    {
        /* Upper Sixteen 16 to 31 GPIO Pin              */
        pin = pinNum - CSL_GPIO_PIN16;
        CSL_FINSR(hGpio->baseAddr->IOINTFLG2,pin,pin,CSL_GPIO_BIT_SET);
    }

    return CSL_SOK;

}

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
CSL_GpioPinNum pinNum,
CSL_Status    *status
)
{
    int flagBit;
    Uint16 pin;
    /* For Invalid handle */
    if(NULL == hGpio)
    {
        *status = CSL_ESYS_BADHANDLE;
        return CSL_ESYS_BADHANDLE;
    }

    /* For wrong pin index */
    if((CSL_GPIO_PIN31 < pinNum)||(pinNum < CSL_GPIO_PIN0))
    {
        *status = CSL_ESYS_INVPARAMS;
        return CSL_ESYS_INVPARAMS;
    }

    if(pinNum < CSL_GPIO_PIN16)
    {
        /* Lower Sixteen GPIO Pin - 0 to 15 GPIO pin */
        pin = pinNum;
        flagBit = CSL_FEXTR(hGpio->baseAddr->IOINTFLG1,pin,pin );
    }
    else
    {
        /* Upper Sixteen 16 to 31 GPIO Pin              */
        pin = pinNum - CSL_GPIO_PIN16;
        flagBit = CSL_FEXTR(hGpio->baseAddr->IOINTFLG2,pin,pin );
    }

    *status = CSL_SOK;
    return flagBit;
}

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
 *
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
GPIO_Handle    hGpio
)
{
    /* For Invalid handle */
    if(NULL == hGpio)
    {
        return CSL_ESYS_BADHANDLE;
    }
    /* Reset the GPIO Registers */
    hGpio->baseAddr->IOINTEN1 = CSL_GPIO_IOINTEN1_RESETVAL ;
    hGpio->baseAddr->IOINTEN2 = CSL_GPIO_IOINTEN2_RESETVAL ;

    hGpio->baseAddr->IOINTFLG1 = (Uint16)(~CSL_GPIO_IOINTFLG1_RESETVAL);
    hGpio->baseAddr->IOINTFLG2 = (Uint16)(~CSL_GPIO_IOINTFLG2_RESETVAL);

    hGpio->baseAddr->IOINTEDG1 = (Uint16)(CSL_GPIO_IOINTEDG1_RESETVAL);
    hGpio->baseAddr->IOINTEDG2 = (Uint16)(CSL_GPIO_IOINTEDG2_RESETVAL);

    hGpio->baseAddr->IODIR1 = CSL_GPIO_IODIR1_RESETVAL;
    hGpio->baseAddr->IODIR2 = CSL_GPIO_IODIR2_RESETVAL;

    hGpio->baseAddr->IOOUTDATA1 = CSL_GPIO_IOOUTDATA1_RESETVAL;
    hGpio->baseAddr->IOOUTDATA2 = CSL_GPIO_IOOUTDATA2_RESETVAL;

    return CSL_SOK;
}
