This folder contains the support code for the STM32F4-Disovery board.


STM32F4xx Standard Peripherals Library
======================================
In the sub-directories inc\ and src\ the code includes the STM32F4xx
Standard Peripherals Library Drivers (StdPeriph_Driver) V1.0.0RC1
from 25-August-2011, which is licensed as follows:

The enclosed firmware and all the related documentation are not covered
by a License Agreement, if you need such License you can contact your
local STMicroelectronics office.

THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING
CUSTOMERS WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER
FOR THEM TO SAVE TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE
HELD LIABLE FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH
RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH FIRMWARE
AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED
HEREIN IN CONNECTION WITH THEIR PRODUCTS.

This ST driver library has been downloaded from:

http://github.com/nabilt/STM32F4-Discovery-Firmware


CMSIS-Compliant Device Files
============================
The code also includes the CMSIS-compliant interface to the STM32F4xx
MCU files:

stm32f4xx.h
system_stm32f4xx.h
system_stm32f4xx.c
arm\startup_stm32f40x.s
gnu\startup_stm32f40x.c
iar\startup_stm32f40x.s

Adjusting the CPU Clock Speed
-----------------------------
The current setting is to run at 168MHz from the PLL, but the CPU clock
speed can be modified by editing the file system_stm32f4xx.c.
 