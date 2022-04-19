# Quantum Leaps' Modifications

This STM32CubeL5 package (version 1.4.0, as of 2022-02-25) has been copied from:

https://github.com/STMicroelectronics/STM32CubeL5

It has been modified by Quantum Leaps to trim it down and leave only facilities pertaining to the board STM32 **NUCLEO-L552ZE-Q** used in QP examples.

The general structure of the STM32CubeL5 package has been **preserved** for compatibilty with the full STM32Cube package.

```
STM32CubeL5/
  |
  +-Drivers/       - deleted unused files, but preserved the structure
  | +-BSP/
  |
  +-CMSIS/         - deleted unused files, but preserved the structure
  |                  also, deleted the ST startup code, see NOTE below
  |
  +-STM32L5xx_HAL_Driver/ - deleted unused files, but preserved the structure
  |
  +-nucleo-l552ze/  - added directory with modified CMSIS-compliant startup code
    +-arm/
    | +-startup_stm32l552xx.s  - startup code for ARM-KEIL compiler (version 6 and 5)
    |
    +-gnu/
    | +-startup_stm32l552xx.c  - startup code for GNU-ARM compiler
    |
    +-iar/
    | +-startup_stm32l552xx.s  - startup code for IAR-ARM compiler
```
> **NOTE**<br> The directory `nucleo-l552ze` was added to STM32CubeL5 with the CMSIS-compliant startup code for various toolchains. This startup code has been re-designed to avoid danial of service behavior in the original ST code (endless loops in exception handlers). Also, the added startup code is better integrated with the QP assertions.

> **CAUTION**<br> The availability of some QP exmples for STM32Cube does **NOT** mean that STM32Cube firmware is recommended by Quantum Leaps. In fact, STM32Cube MCU Firmware Package is expressly **NOT** recommended for production code, especially in safety-related applications.

--------------
# STM32CubeL5 MCU Firmware Package

**STM32Cube** is an STMicroelectronics original initiative to ease the developers life by reducing efforts, time and cost.

**STM32Cube** covers the overall STM32 products portfolio. It includes a comprehensive embedded software platform delivered for each STM32 series.
   * The CMSIS modules (core and device) corresponding to the ARM(tm) core implemented in this STM32 product.
   * The STM32 HAL-LL drivers, an abstraction layer offering a set of APIs ensuring maximized portability across the STM32 portfolio.
   * The BSP drivers of each evaluation, demonstration or nucleo board provided for this STM32 series.
   * A consistent set of middleware components such as RTOS, USB, FatFS, graphics, touch sensing library...
   * A full set of software projects (basic examples, applications, and demonstrations) for each board provided for this STM32 series.

The **STM32CubeL5 MCU Package** projects are directly running on the STM32L5 series boards. You can find in each Projects/*Board name* directories a set of software projects (Applications/Demonstration/Examples).

## Release note

Details about the content of this release are available in the release note [here](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeL5/blob/master/Release_Notes.html).

## Boards available

  * STM32L5
    * [NUCLEO-L552ZE-Q](https://www.st.com/en/evaluation-tools/nucleo-l552ze-q.html)
    * [STM32L552E-EV](https://www.st.com/en/evaluation-tools/stm32l552e-ev.html)
    * [STM32L562E-DK](https://www.st.com/en/evaluation-tools/stm32l562e-dk.html)

## Troubleshooting

**Caution** : The issues and the pull-requests are **strictly limited** to submit problems or suggestions related to the software delivered in this repository.

**For any other question** related to the product, the hardware performance or characteristics, the tools, the environment, you can submit it to the **ST Community** on the STM32 MCUs related [page](https://community.st.com/s/group/0F90X000000AXsASAW/stm32-mcus).
