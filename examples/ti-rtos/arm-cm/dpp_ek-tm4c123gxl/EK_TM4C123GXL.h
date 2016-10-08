/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       EK_TM4C123GXL.h
 *
 *  @brief      EK_TM4C123GXL Board Specific APIs
 *
 *  The EK_TM4C123GXL header file should be included in an application as
 *  follows:
 *  @code
 *  #include <EK_TM4C123GXL.h>
 *  @endcode
 *
 *  ============================================================================
 */
#ifndef __EK_TM4C123GXL_H
#define __EK_TM4C123GXL_H

#ifdef __cplusplus
extern "C" {
#endif

/* LEDs on EK_TM4C123GXL are active high. */
#define EK_TM4C123GXL_LED_OFF (0)
#define EK_TM4C123GXL_LED_ON  (1)

/*!
 *  @def    EK_TM4C123GXL_GPIOName
 *  @brief  Enum of GPIO names on the EK_TM4C123GXL dev board
 */
typedef enum EK_TM4C123GXL_GPIOName {
    EK_TM4C123GXL_SW1 = 0,
    EK_TM4C123GXL_SW2,
    EK_TM4C123GXL_LED_RED,
    EK_TM4C123GXL_LED_BLUE,
    EK_TM4C123GXL_LED_GREEN,

    EK_TM4C123GXL_GPIOCOUNT
} EK_TM4C123GXL_GPIOName;

/*!
 *  @def    EK_TM4C123GXL_I2CName
 *  @brief  Enum of I2C names on the EK_TM4C123GXL dev board
 */
typedef enum EK_TM4C123GXL_I2CName {
    EK_TM4C123GXL_I2C0 = 0,
    EK_TM4C123GXL_I2C3,

    EK_TM4C123GXL_I2CCOUNT
} EK_TM4C123GXL_I2CName;

/*!
 *  @def    EK_TM4C123GXL_PWMName
 *  @brief  Enum of PWM names on the EK_TM4C123GXL dev board
 */
typedef enum EK_TM4C123GXL_PWMName {
    EK_TM4C123GXL_PWM6 = 0,
    EK_TM4C123GXL_PWM7,

    EK_TM4C123GXL_PWMCOUNT
} EK_TM4C123GXL_PWMName;

/*!
 *  @def    EK_TM4C123GXL_SDSPIName
 *  @brief  Enum of SDSPI names on the EK_TM4C123GXL dev board
 */
typedef enum EK_TM4C123GXL_SDSPIName {
    EK_TM4C123GXL_SDSPI0 = 0,

    EK_TM4C123GXL_SDSPICOUNT
} EK_TM4C123GXL_SDSPIName;

/*!
 *  @def    EK_TM4C123GXL_SPIName
 *  @brief  Enum of SPI names on the EK_TM4C123GXL dev board
 */
typedef enum EK_TM4C123GXL_SPIName {
    EK_TM4C123GXL_SPI0 = 0,
    EK_TM4C123GXL_SPI2,
    EK_TM4C123GXL_SPI3,

    EK_TM4C123GXL_SPICOUNT
} EK_TM4C123GXL_SPIName;

/*!
 *  @def    EK_TM4C123GXL_UARTName
 *  @brief  Enum of UARTs on the EK_TM4C123GXL dev board
 */
typedef enum EK_TM4C123GXL_UARTName {
    EK_TM4C123GXL_UART0 = 0,

    EK_TM4C123GXL_UARTCOUNT
} EK_TM4C123GXL_UARTName;

/*!
 *  @def    EK_TM4C123GXL_USBMode
 *  @brief  Enum of USB setup function on the EK_TM4C123GXL dev board
 */
typedef enum EK_TM4C123GXL_USBMode {
    EK_TM4C123GXL_USBDEVICE,
    EK_TM4C123GXL_USBHOST
} EK_TM4C123GXL_USBMode;

/*!
 *  @def    EK_TM4C123GXL_WatchdogName
 *  @brief  Enum of Watchdogs on the EK_TM4C123GXL dev board
 */
typedef enum EK_TM4C123GXL_WatchdogName {
    EK_TM4C123GXL_WATCHDOG0 = 0,

    EK_TM4C123GXL_WATCHDOGCOUNT
} EK_TM4C123GXL_WatchdogName;

/*!
 *  @def    EK_TM4C123GXL_WiFiName
 *  @brief  Enum of WiFi names on the EK_TM4C123GXL dev board
 */
typedef enum EK_TM4C123GXL_WiFiName {
    EK_TM4C123GXL_WIFI = 0,

    EK_TM4C123GXL_WIFICOUNT
} EK_TM4C123GXL_WiFiName;

/*!
 *  @brief  Initialize board specific DMA settings
 *
 *  This function creates a hwi in case the DMA controller creates an error
 *  interrrupt, enables the DMA and supplies it with a uDMA control table.
 */
extern void EK_TM4C123GXL_initDMA(void);

/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 *  This includes:
 *     - Flash wait states based on the process
 *     - Disable clock source to watchdog module
 *     - Enable clock sources for peripherals
 */
extern void EK_TM4C123GXL_initGeneral(void);

/*!
 *  @brief  Initialize board specific GPIO settings
 *
 *  This function initializes the board specific GPIO settings and
 *  then calls the GPIO_init API to initialize the GPIO module.
 *
 *  The GPIOs controlled by the GPIO module are determined by the GPIO_PinConfig
 *  variable.
 */
extern void EK_TM4C123GXL_initGPIO(void);

/*!
 *  @brief  Initialize board specific I2C settings
 *
 *  This function initializes the board specific I2C settings and then calls
 *  the I2C_init API to initialize the I2C module.
 *
 *  The I2C peripherals controlled by the I2C module are determined by the
 *  I2C_config variable.
 */
extern void EK_TM4C123GXL_initI2C(void);

/*!
 *  @brief  Initialize board specific PWM settings
 *
 *  This function initializes the board specific PWM settings and then calls
 *  the PWM_init API to initialize the PWM module.
 *
 *  The PWM peripherals controlled by the PWM module are determined by the
 *  PWM_config variable.
 */
extern void EK_TM4C123GXL_initPWM(void);

/*!
 *  @brief  Initialize board specific SDSPI settings
 *
 *  This function initializes the board specific SDSPI settings and then calls
 *  the SDSPI_init API to initialize the SDSPI module.
 *
 *  The SDSPI peripherals controlled by the SDSPI module are determined by the
 *  SDSPI_config variable.
 */
extern void EK_TM4C123GXL_initSDSPI(void);

/*!
 *  @brief  Initialize board specific SPI settings
 *
 *  This function initializes the board specific SPI settings and then calls
 *  the SPI_init API to initialize the SPI module.
 *
 *  The SPI peripherals controlled by the SPI module are determined by the
 *  SPI_config variable.
 */
extern void EK_TM4C123GXL_initSPI(void);

/*!
 *  @brief  Initialize board specific UART settings
 *
 *  This function initializes the board specific UART settings and then calls
 *  the UART_init API to initialize the UART module.
 *
 *  The UART peripherals controlled by the UART module are determined by the
 *  UART_config variable.
 */
extern void EK_TM4C123GXL_initUART(void);

/*!
 *  @brief  Initialize board specific USB settings
 *
 *  This function initializes the board specific USB settings and pins based on
 *  the USB mode of operation.
 *
 *  @param      usbMode    USB mode of operation
 */
extern void EK_TM4C123GXL_initUSB(EK_TM4C123GXL_USBMode usbMode);

/*!
 *  @brief  Initialize board specific Watchdog settings
 *
 *  This function initializes the board specific Watchdog settings and then
 *  calls the Watchdog_init API to initialize the Watchdog module.
 *
 *  The Watchdog peripherals controlled by the Watchdog module are determined
 *  by the Watchdog_config variable.
 */
extern void EK_TM4C123GXL_initWatchdog(void);

/*!
 *  @brief  Initialize board specific WiFi settings
 *
 *  This function initializes the board specific WiFi settings and then calls
 *  the WiFi_init API to initialize the WiFi module.
 *
 *  The hardware resources controlled by the WiFi module are determined by the
 *  WiFi_config variable.
 *
 *  A SimpleLink CC3100 device or module is required and must be connected to
 *  use the WiFi driver.
 */
extern void EK_TM4C123GXL_initWiFi(void);

#ifdef __cplusplus
}
#endif

#endif /* __EK_TM4C123GXL_H */
