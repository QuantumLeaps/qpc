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

/*
 *  ======== EK_TM4C123GXL.c ========
 *  This file is responsible for setting up the board specific items for the
 *  EK_TM4C123GXL board.
 */

#include <stdint.h>
#include <stdbool.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>

#include <driverlib/gpio.h>
#include <driverlib/i2c.h>
#include <driverlib/pin_map.h>
#include <driverlib/pwm.h>
#include <driverlib/ssi.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>
#include <driverlib/udma.h>

#include "EK_TM4C123GXL.h"

#ifndef TI_DRIVERS_UART_DMA
#define TI_DRIVERS_UART_DMA 0
#endif

/*
 *  =============================== DMA ===============================
 */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(dmaControlTable, 1024)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=1024
#elif defined(__GNUC__)
__attribute__ ((aligned (1024)))
#endif
static tDMAControlTable dmaControlTable[32];
static bool dmaInitialized = false;

/* Hwi_Struct used in the initDMA Hwi_construct call */
static Hwi_Struct dmaHwiStruct;

/*
 *  ======== dmaErrorHwi ========
 */
static Void dmaErrorHwi(UArg arg)
{
    System_printf("DMA error code: %d\n", uDMAErrorStatusGet());
    uDMAErrorStatusClear();
    System_abort("DMA error!!");
}

/*
 *  ======== EK_TM4C123GXL_initDMA ========
 */
void EK_TM4C123GXL_initDMA(void)
{
    Error_Block eb;
    Hwi_Params  hwiParams;

    if (!dmaInitialized) {
        Error_init(&eb);
        Hwi_Params_init(&hwiParams);
        Hwi_construct(&(dmaHwiStruct), INT_UDMAERR, dmaErrorHwi,
                      &hwiParams, &eb);
        if (Error_check(&eb)) {
            System_abort("Couldn't construct DMA error hwi");
        }

        SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
        uDMAEnable();
        uDMAControlBaseSet(dmaControlTable);

        dmaInitialized = true;
    }
}

/*
 *  =============================== General ===============================
 */
/*
 *  ======== EK_TM4C123GXL_initGeneral ========
 */
void EK_TM4C123GXL_initGeneral(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}

/*
 *  =============================== GPIO ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(GPIOTiva_config, ".const:GPIOTiva_config")
#endif

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOTiva.h>

/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in EK_TM4C123GXL.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array.  Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */
GPIO_PinConfig gpioPinConfigs[] = {
    /* Input pins */
    /* EK_TM4C123GXL_GPIO_SW1 */
    GPIOTiva_PF_4 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,
    /* EK_TM4C123GXL_GPIO_SW2 */
    GPIOTiva_PF_0 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,

    /* Output pins */
    /* EK_TM4C123GXL_LED_RED */
    GPIOTiva_PF_1 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* EK_TM4C123GXL_LED_BLUE */
    GPIOTiva_PF_2 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    /* EK_TM4C123GXL_LED_GREEN */
    GPIOTiva_PF_3 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW
};

/*
 * Array of callback function pointers
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in EK_TM4C123GXL.h
 * NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *       reduce memory usage (if placed at end of gpioPinConfigs array).
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL,  /* EK_TM4C123GXL_GPIO_SW1 */
    NULL   /* EK_TM4C123GXL_GPIO_SW2 */
};

/* The device-specific GPIO_config structure */
const GPIOTiva_Config GPIOTiva_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = sizeof(gpioPinConfigs)/sizeof(GPIO_PinConfig),
    .numberOfCallbacks = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
    .intPriority = (~0)
};

/*
 *  ======== EK_TM4C123GXL_initGPIO ========
 */
void EK_TM4C123GXL_initGPIO(void)
{
    /* EK_TM4C123GXL_GPIO_SW2 - PF0 requires unlocking before configuration */
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);

    /* Initialize peripheral and pins */
    GPIO_init();
}

/*
 *  =============================== I2C ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(I2C_config, ".const:I2C_config")
#pragma DATA_SECTION(i2cTivaHWAttrs, ".const:i2cTivaHWAttrs")
#endif

#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CTiva.h>

I2CTiva_Object i2cTivaObjects[EK_TM4C123GXL_I2CCOUNT];

const I2CTiva_HWAttrs i2cTivaHWAttrs[EK_TM4C123GXL_I2CCOUNT] = {
    {
        .baseAddr = I2C1_BASE,
        .intNum = INT_I2C1,
        .intPriority = (~0)
    },
    {
        .baseAddr = I2C3_BASE,
        .intNum = INT_I2C3,
        .intPriority = (~0)
    },
};

const I2C_Config I2C_config[] = {
    {
        .fxnTablePtr = &I2CTiva_fxnTable,
        .object = &i2cTivaObjects[0],
        .hwAttrs = &i2cTivaHWAttrs[0]
    },
    {
        .fxnTablePtr = &I2CTiva_fxnTable,
        .object = &i2cTivaObjects[1],
        .hwAttrs = &i2cTivaHWAttrs[1]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== EK_TM4C123GXL_initI2C ========
 */
void EK_TM4C123GXL_initI2C(void)
{
    /* I2C1 Init */
    /* Enable the peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

    /* Configure the appropriate pins to be I2C instead of GPIO. */
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

    /* I2C3 Init */
    /*
     * NOTE: TI-RTOS examples configure pins PD0 & PD1 for SSI3 or I2C3.  Thus,
     * a conflict occurs when the I2C & SPI drivers are used simultaneously in
     * an application.  Modify the pin mux settings in this file and resolve the
     * conflict before running your the application.
     */
    /* Enable the peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);

    /* Configure the appropriate pins to be I2C instead of GPIO. */
    GPIOPinConfigure(GPIO_PD0_I2C3SCL);
    GPIOPinConfigure(GPIO_PD1_I2C3SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);

    /*
     * These GPIOs are connected to PD0 and PD1 and need to be brought into a
     * GPIO input state so they don't interfere with I2C communications.
     */
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_7);

    I2C_init();
}

/*
 *  =============================== PWM ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(PWM_config, ".const:PWM_config")
#pragma DATA_SECTION(pwmTivaHWAttrs, ".const:pwmTivaHWAttrs")
#endif

#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTiva.h>

PWMTiva_Object pwmTivaObjects[EK_TM4C123GXL_PWMCOUNT];

const PWMTiva_HWAttrs pwmTivaHWAttrs[EK_TM4C123GXL_PWMCOUNT] = {
    {
        .baseAddr = PWM1_BASE,
        .pwmOutput = PWM_OUT_6,
        .pwmGenOpts = PWM_GEN_MODE_DOWN | PWM_GEN_MODE_DBG_RUN
    },
    {
        .baseAddr = PWM1_BASE,
        .pwmOutput = PWM_OUT_7,
        .pwmGenOpts = PWM_GEN_MODE_DOWN | PWM_GEN_MODE_DBG_RUN
    }
};

const PWM_Config PWM_config[] = {
    {
        .fxnTablePtr = &PWMTiva_fxnTable,
        .object = &pwmTivaObjects[0],
        .hwAttrs = &pwmTivaHWAttrs[0]
    },
    {
        .fxnTablePtr = &PWMTiva_fxnTable,
        .object = &pwmTivaObjects[1],
        .hwAttrs = &pwmTivaHWAttrs[1]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== EK_TM4C123GXL_initPWM ========
 */
void EK_TM4C123GXL_initPWM(void)
{
    /* Enable PWM peripherals */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    /*
     * Enable PWM output on GPIO pins.  Board_LED1 and Board_LED2 are now
     * controlled by PWM peripheral - Do not use GPIO APIs.
     */
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2 |GPIO_PIN_3);

    PWM_init();
}

/*
 *  =============================== SDSPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SDSPI_config, ".const:SDSPI_config")
#pragma DATA_SECTION(sdspiTivaHWattrs, ".const:sdspiTivaHWattrs")
#endif

#include <ti/drivers/SDSPI.h>
#include <ti/drivers/sdspi/SDSPITiva.h>

SDSPITiva_Object sdspiTivaObjects[EK_TM4C123GXL_SDSPICOUNT];

const SDSPITiva_HWAttrs sdspiTivaHWattrs[EK_TM4C123GXL_SDSPICOUNT] = {
    {
        .baseAddr = SSI2_BASE,

        .portSCK = GPIO_PORTB_BASE,
        .pinSCK = GPIO_PIN_4,
        .portMISO = GPIO_PORTB_BASE,
        .pinMISO = GPIO_PIN_6,
        .portMOSI = GPIO_PORTB_BASE,
        .pinMOSI = GPIO_PIN_7,
        .portCS = GPIO_PORTA_BASE,
        .pinCS = GPIO_PIN_5,
    }
};

const SDSPI_Config SDSPI_config[] = {
    {
        .fxnTablePtr = &SDSPITiva_fxnTable,
        .object = &sdspiTivaObjects[0],
        .hwAttrs = &sdspiTivaHWattrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== EK_TM4C123GXL_initSDSPI ========
 */
void EK_TM4C123GXL_initSDSPI(void)
{
    /* Enable the peripherals used by the SD Card */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);

    /* Configure pad settings */
    GPIOPadConfigSet(GPIO_PORTB_BASE,
            GPIO_PIN_4 | GPIO_PIN_7,
            GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

    GPIOPadConfigSet(GPIO_PORTB_BASE,
            GPIO_PIN_6,
            GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOPadConfigSet(GPIO_PORTA_BASE,
            GPIO_PIN_5,
            GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

    GPIOPinConfigure(GPIO_PB4_SSI2CLK);
    GPIOPinConfigure(GPIO_PB6_SSI2RX);
    GPIOPinConfigure(GPIO_PB7_SSI2TX);

    /*
     * These GPIOs are connected to PB6 and PB7 and need to be brought into a
     * GPIO input state so they don't interfere with SPI communications.
     */
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_1);

    SDSPI_init();
}

/*
 *  =============================== SPI ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(SPI_config, ".const:SPI_config")
#pragma DATA_SECTION(spiTivaDMAHWAttrs, ".const:spiTivaDMAHWAttrs")
#endif

#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPITivaDMA.h>

SPITivaDMA_Object spiTivaDMAObjects[EK_TM4C123GXL_SPICOUNT];

#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(spiTivaDMAscratchBuf, 32)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=32
#elif defined(__GNUC__)
__attribute__ ((aligned (32)))
#endif
uint32_t spiTivaDMAscratchBuf[EK_TM4C123GXL_SPICOUNT];

const SPITivaDMA_HWAttrs spiTivaDMAHWAttrs[EK_TM4C123GXL_SPICOUNT] = {
    {
        .baseAddr = SSI0_BASE,
        .intNum = INT_SSI0,
        .intPriority = (~0),
        .scratchBufPtr = &spiTivaDMAscratchBuf[0],
        .defaultTxBufValue = 0,
        .rxChannelIndex = UDMA_CHANNEL_SSI0RX,
        .txChannelIndex = UDMA_CHANNEL_SSI0TX,
        .channelMappingFxn = uDMAChannelAssign,
        .rxChannelMappingFxnArg = UDMA_CH10_SSI0RX,
        .txChannelMappingFxnArg = UDMA_CH11_SSI0TX
    },
    {
        .baseAddr = SSI2_BASE,
        .intNum = INT_SSI2,
        .intPriority = (~0),
        .scratchBufPtr = &spiTivaDMAscratchBuf[1],
        .defaultTxBufValue = 0,
        .rxChannelIndex = UDMA_SEC_CHANNEL_UART2RX_12,
        .txChannelIndex = UDMA_SEC_CHANNEL_UART2TX_13,
        .channelMappingFxn = uDMAChannelAssign,
        .rxChannelMappingFxnArg = UDMA_CH12_SSI2RX,
        .txChannelMappingFxnArg = UDMA_CH13_SSI2TX
    },
    {
        .baseAddr = SSI3_BASE,
        .intNum = INT_SSI3,
        .intPriority = (~0),
        .scratchBufPtr = &spiTivaDMAscratchBuf[2],
        .defaultTxBufValue = 0,
        .rxChannelIndex = UDMA_SEC_CHANNEL_TMR2A_14,
        .txChannelIndex = UDMA_SEC_CHANNEL_TMR2B_15,
        .channelMappingFxn = uDMAChannelAssign,
        .rxChannelMappingFxnArg = UDMA_CH14_SSI3RX,
        .txChannelMappingFxnArg = UDMA_CH15_SSI3TX
    }
};

const SPI_Config SPI_config[] = {
    {
        .fxnTablePtr = &SPITivaDMA_fxnTable,
        .object = &spiTivaDMAObjects[0],
        .hwAttrs = &spiTivaDMAHWAttrs[0]
    },
    {
        .fxnTablePtr = &SPITivaDMA_fxnTable,
        .object = &spiTivaDMAObjects[1],
        .hwAttrs = &spiTivaDMAHWAttrs[1]
    },
    {
        .fxnTablePtr = &SPITivaDMA_fxnTable,
        .object = &spiTivaDMAObjects[2],
        .hwAttrs = &spiTivaDMAHWAttrs[2]
    },
    {NULL, NULL, NULL},
};

/*
 *  ======== EK_TM4C123GXL_initSPI ========
 */
void EK_TM4C123GXL_initSPI(void)
{
    /* SPI0 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    /* Need to unlock PF0 */
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);

    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 |
                                    GPIO_PIN_4 | GPIO_PIN_5);

    /* SSI2 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);

    GPIOPinConfigure(GPIO_PB4_SSI2CLK);
    GPIOPinConfigure(GPIO_PB5_SSI2FSS);
    GPIOPinConfigure(GPIO_PB6_SSI2RX);
    GPIOPinConfigure(GPIO_PB7_SSI2TX);

    GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 |
                                    GPIO_PIN_6 | GPIO_PIN_7);

    /* SSI3 */
    /*
     * NOTE: TI-RTOS examples configure pins PD0 & PD1 for SSI3 or I2C3.  Thus,
     * a conflict occurs when the I2C & SPI drivers are used simultaneously in
     * an application.  Modify the pin mux settings in this file and resolve the
     * conflict before running your the application.
     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);

    GPIOPinConfigure(GPIO_PD0_SSI3CLK);
    GPIOPinConfigure(GPIO_PD1_SSI3FSS);
    GPIOPinConfigure(GPIO_PD2_SSI3RX);
    GPIOPinConfigure(GPIO_PD3_SSI3TX);

    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                                    GPIO_PIN_2 | GPIO_PIN_3);

    EK_TM4C123GXL_initDMA();
    SPI_init();
}

/*
 *  =============================== UART ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(UART_config, ".const:UART_config")
#pragma DATA_SECTION(uartTivaHWAttrs, ".const:uartTivaHWAttrs")
#endif

#include <ti/drivers/UART.h>
#if TI_DRIVERS_UART_DMA
#include <ti/drivers/uart/UARTTivaDMA.h>

UARTTivaDMA_Object uartTivaObjects[EK_TM4C123GXL_UARTCOUNT];

const UARTTivaDMA_HWAttrs uartTivaHWAttrs[EK_TM4C123GXL_UARTCOUNT] = {
    {
        .baseAddr = UART0_BASE,
        .intNum = INT_UART0,
        .intPriority = (~0),
        .rxChannelIndex = UDMA_CH8_UART0RX,
        .txChannelIndex = UDMA_CH9_UART0TX,
    }
};

const UART_Config UART_config[] = {
    {
        .fxnTablePtr = &UARTTivaDMA_fxnTable,
        .object = &uartTivaObjects[0],
        .hwAttrs = &uartTivaHWAttrs[0]
    },
    {NULL, NULL, NULL}
};
#else
#include <ti/drivers/uart/UARTTiva.h>

UARTTiva_Object uartTivaObjects[EK_TM4C123GXL_UARTCOUNT];
unsigned char uartTivaRingBuffer[EK_TM4C123GXL_UARTCOUNT][32];

/* UART configuration structure */
const UARTTiva_HWAttrs uartTivaHWAttrs[EK_TM4C123GXL_UARTCOUNT] = {
    {
        .baseAddr = UART0_BASE,
        .intNum = INT_UART0,
        .intPriority = (~0),
        .flowControl = UART_FLOWCONTROL_NONE,
        .ringBufPtr  = uartTivaRingBuffer[0],
        .ringBufSize = sizeof(uartTivaRingBuffer[0])
    }
};

const UART_Config UART_config[] = {
    {
        .fxnTablePtr = &UARTTiva_fxnTable,
        .object = &uartTivaObjects[0],
        .hwAttrs = &uartTivaHWAttrs[0]
    },
    {NULL, NULL, NULL}
};
#endif /* TI_DRIVERS_UART_DMA */

/*
 *  ======== EK_TM4C123GXL_initUART ========
 */
void EK_TM4C123GXL_initUART(void)
{
    /* Enable and configure the peripherals used by the uart. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Initialize the UART driver */
#if TI_DRIVERS_UART_DMA
    EK_TM4C123GXL_initDMA();
#endif
    UART_init();
}

/*
 *  =============================== USB ===============================
 */
/*
 *  ======== EK_TM4C123GXL_initUSB ========
 *  This function just turns on the USB
 */
void EK_TM4C123GXL_initUSB(EK_TM4C123GXL_USBMode usbMode)
{
    /* Enable the USB peripheral and PLL */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
    SysCtlUSBPLLEnable();

    /* Setup pins for USB operation */
    GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    if (usbMode == EK_TM4C123GXL_USBHOST) {
        System_abort("USB host not supported\n");
    }
}

/*
 *  =============================== Watchdog ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(Watchdog_config, ".const:Watchdog_config")
#pragma DATA_SECTION(watchdogTivaHWAttrs, ".const:watchdogTivaHWAttrs")
#endif

#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogTiva.h>

WatchdogTiva_Object watchdogTivaObjects[EK_TM4C123GXL_WATCHDOGCOUNT];

const WatchdogTiva_HWAttrs watchdogTivaHWAttrs[EK_TM4C123GXL_WATCHDOGCOUNT] = {
    {
        .baseAddr = WATCHDOG0_BASE,
        .intNum = INT_WATCHDOG,
        .intPriority = (~0),
        .reloadValue = 80000000 // 1 second period at default CPU clock freq
    },
};

const Watchdog_Config Watchdog_config[] = {
    {
        .fxnTablePtr = &WatchdogTiva_fxnTable,
        .object = &watchdogTivaObjects[0],
        .hwAttrs = &watchdogTivaHWAttrs[0]
    },
    {NULL, NULL, NULL},
};

/*
 *  ======== EK_TM4C123GXL_initWatchdog ========
 *
 * NOTE: To use the other watchdog timer with base address WATCHDOG1_BASE,
 *       an additional function call may need be made to enable PIOSC. Enabling
 *       WDOG1 does not do this. Enabling another peripheral that uses PIOSC
 *       such as ADC0 or SSI0, however, will do so. Example:
 *
 *       SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
 *       SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG1);
 *
 *       See the following forum post for more information:
 *       http://e2e.ti.com/support/microcontrollers/stellaris_arm_cortex-m3_microcontroller/f/471/p/176487/654390.aspx#654390
 */
void EK_TM4C123GXL_initWatchdog(void)
{
    /* Enable peripherals used by Watchdog */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);

    Watchdog_init();
}

/*
 *  =============================== WiFi ===============================
 */
/* Place into subsections to allow the TI linker to remove items properly */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(WiFi_config, ".const:WiFi_config")
#pragma DATA_SECTION(wiFiCC3100HWAttrs, ".const:wiFiCC3100HWAttrs")
#endif

#include <ti/drivers/WiFi.h>
#include <ti/drivers/wifi/WiFiCC3100.h>

WiFiCC3100_Object wiFiCC3100Objects[EK_TM4C123GXL_WIFICOUNT];

const WiFiCC3100_HWAttrs wiFiCC3100HWAttrs[EK_TM4C123GXL_WIFICOUNT] = {
    {
        .irqPort = GPIO_PORTB_BASE,
        .irqPin = GPIO_PIN_2,
        .irqIntNum = INT_GPIOB,

        .csPort = GPIO_PORTE_BASE,
        .csPin = GPIO_PIN_0,

        .enPort = GPIO_PORTE_BASE,
        .enPin = GPIO_PIN_4
    }
};

const WiFi_Config WiFi_config[] = {
    {
        .fxnTablePtr = &WiFiCC3100_fxnTable,
        .object = &wiFiCC3100Objects[0],
        .hwAttrs = &wiFiCC3100HWAttrs[0]
    },
    {NULL,NULL, NULL},
};

/*
 *  ======== EK_TM4C123GXL_initWiFi ========
 */
void EK_TM4C123GXL_initWiFi(void)
{
    /* Configure EN & CS pins to disable CC3100 */
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);

    /* Configure SSI2 for CC3100 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
    GPIOPinConfigure(GPIO_PB4_SSI2CLK);
    GPIOPinConfigure(GPIO_PB6_SSI2RX);
    GPIOPinConfigure(GPIO_PB7_SSI2TX);
    GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7);

    /* Configure IRQ pin */
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPD);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_RISING_EDGE);

    SPI_init();
    EK_TM4C123GXL_initDMA();

    WiFi_init();
}
