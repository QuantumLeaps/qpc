# Quantum Leaps' Modifications

This STM32CubeH7 package (version 1.9.1, as of 2022-02-25) has been copied from:

https://github.com/STMicroelectronics/STM32CubeH7


It has been modified by Quantum Leaps to trim it down and leave only facilities pertaining to the board STM32 **NUCLEO-H743ZI** used in QP examples.

The general structure of the STM32CubeH7 package has been **preserved** for compatibilty with the full STM32Cube package.

```
STM32CubeH7/
  |
  +-Drivers/       - deleted unused files, but preserved the structure
  | +-BSP/
  |
  +-CMSIS/         - deleted unused files, but preserved the structure
  |                  also, deleted the ST startup code, see NOTE below
  |
  +-STM32H7xx_HAL_Driver/ - deleted unused files, but preserved the structure
  |
  +-nucleo-h743zi/  - added directory with modified CMSIS-compliant startup code
    +-arm/
    | +-startup_stm32h743xx.s  - startup code for ARM-KEIL compiler (version 6 and 5)
    |
    +-gnu/
    | +-startup_stm32h743xx.c  - startup code for GNU-ARM compiler
    |
    +-iar/
    | +-startup_stm32h743xx.s  - startup code for IAR-ARM compiler
```
> **NOTE**<br> The directory `nucleo-h743zi` was added to STM32CubeH7 with the CMSIS-compliant startup code for various toolchains. This startup code has been re-designed to avoid danial of service behavior in the original STCube code (endless loops in exception handlers). Also, the added startup code is better integrated with the QP assertions.

> **CAUTION**<br> The availability of some QP exmples for STM32Cube does **NOT** mean that STM32Cube firmware is recommended by Quantum Leaps. In fact, STM32Cube MCU Firmware Package is expressly **NOT** recommended for production code, especially in safety-related applications.

--------------
# STM32CubeH7 MCU Firmware Package

![latest tag](https://img.shields.io/github/v/tag/STMicroelectronics/STM32CubeH7.svg?color=brightgreen)

**STM32Cube** is an STMicroelectronics original initiative to ease developers' life by reducing efforts, time and cost.

**STM32Cube** covers the overall STM32 products portfolio. It includes a comprehensive embedded software platform delivered for each STM32 series.
   * The CMSIS modules (core and device) corresponding to the ARM(tm) core implemented in this STM32 product.
   * The STM32 HAL-LL drivers, an abstraction layer offering a set of APIs ensuring maximized portability across the STM32 portfolio.
   * The BSP drivers of each evaluation, demonstration or nucleo board provided for this STM32 series.
   * A consistent set of middleware libraries such as RTOS, USB, FatFS, graphics, touch sensing library...
   * A full set of software projects (basic examples, applications, and demonstrations) for each board provided for this STM32 series.

The **STM32CubeH7 MCU Package** projects are directly running on the STM32H7 series boards. You can find in each Projects/*Board name* directories a set of software projects (Applications/Demonstration/Examples).

In this FW Package, the modules **Middlewares/ST/TouchGFX**, **Middlewares/ST/STemWin**, and **Middlewares/ST/STM32_Audio** are not directly accessible. They must be downloaded from a ST server. The respective URL are available in a readme.txt file inside each module.

## Release note

Details about the content of this release are available in the release note [here](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/STM32CubeH7/blob/master/Release_Notes.html).

## Boards available

  * STM32H7
    * [NUCLEO-H7A3ZI-Q](https://www.st.com/en/product/nucleo-h7a3zi-q.html)
    * [NUCLEO-H723ZG](https://www.st.com/en/product/nucleo-h723zg.html)
    * [NUCLEO-H743ZI](https://www.st.com/en/product/nucleo-h743zi.html)
    * [NUCLEO-H743ZI2](https://www.st.com/en/product/nucleo-h743zi.html)
    * [NUCLEO-H753ZI](https://www.st.com/en/product/nucleo-h753zi.html)
    * [NUCLEO-H753ZI2](https://www.st.com/en/product/nucleo-h753zi.html)
    * [NUCLEO-H745ZI-Q](https://www.st.com/en/product/nucleo-h745zi-q.html)
    * [NUCLEO-H755ZI-Q](https://www.st.com/en/product/nucleo-h755zi-q.html)
    * [STM32H7B3I-DK](https://www.st.com/en/product/stm32h7b3i-dk.html)
    * [STM32H7B3I-EVAL](https://www.st.com/en/product/stm32h7b3i-eval.html)
    * [STM32H735G-DK](https://www.st.com/en/product/stm32h735g-dk.html)
    * [STM32H743I-EVAL](https://www.st.com/en/product/stm32h743i-eval.html)
    * [STM32H743I-EVAL2](https://www.st.com/en/product/stm32h743i-eval.html)
    * [STM32H753I-EVAL](https://www.st.com/en/product/stm32h753i-eval.html)
    * [STM32H753I-EVAL2](https://www.st.com/en/product/stm32h753i-eval.html)
    * [STM32H745I-DISCO](https://www.st.com/en/product/stm32h745i-disco.html)
    * [STM32H747I-DISCO](https://www.st.com/en/product/stm32h747i-disco.html)
    * [STM32H747I-DISC1](https://www.st.com/en/product/stm32h747i-disco.html)
    * [STM32H747I-EVAL](https://www.st.com/en/product/stm32h747i-eval.html)
    * [STM32H757I-EVAL](https://www.st.com/en/product/stm32h757i-eval.html)
    * [STM32H750B-DK](https://www.st.com/en/product/stm32h750b-dk.html)

## Troubleshooting

**Caution** : The issues and the pull-requests are **strictly limited** to submit problems or suggestions related to the software delivered in this repository.

**For any other question** related to the product, the hardware performance or characteristics, the tools, the environment, you can submit it to the **ST Community** on the STM32 MCUs related [page](https://community.st.com/s/group/0F90X000000AXsASAW/stm32-mcus).
