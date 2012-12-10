/*****************************************************************************
 *
 * \file
 *
 * \brief UC3-A3-Xplained board header file.
 *
 * This file contains definitions and services related to the features of the
 * UC3-A3-Xplained board.
 *
 * To use this board, define BOARD=UC3_A3_XPLAINED.
  *
 * Copyright (c) 2010-2011 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 ******************************************************************************/


#ifndef UC3_A3_XPLAINED_H_
#define UC3_A3_XPLAINED_H_

#include "compiler.h"


#ifdef __AVR32_ABI_COMPILER__ // Automatically defined when compiling for AVR32, not when assembling.
#  include "led.h"
#endif  // __AVR32_ABI_COMPILER__


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup at32uc3a3_xpld_config UC3-A3 Xplained Board Configuration
 * @{
 */

//! \name Oscillator Definitions
// @{

/*! \brief System oscillator frequencies (Hz.) and startup times (periods).
 *
 * RCOsc has no custom calibration by default. Set the following definition
 * to the appropriate value if a custom RCOsc calibration has been applied
 * to your part.
 */

//!< RCOsc frequency: Hz.

#define FRCOSC                      (AVR32_PM_RCOSC_FREQUENCY)

//!< Osc32 frequency (Hz.) and startup time (RCOsc periods).

#define FOSC32                      (32768)
#define OSC32_STARTUP               (AVR32_PM_OSCCTRL32_STARTUP_8192_RCOSC)

#define BOARD_OSC32_IS_XTAL         true
#define BOARD_OSC32_HZ              FOSC32
#define BOARD_OSC32_STARTUP_US      (570000)


//!< Osc frequency (Hz.) and startup time (RCOsc periods).

#define FOSC0                       (12000000)
#define OSC0_STARTUP                (AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC)

#define BOARD_OSC0_IS_XTAL          true
#define BOARD_OSC0_HZ               FOSC0
#define BOARD_OSC0_STARTUP_US       (18000)

// @}


//! \def Number of LEDs.

#define LED_COUNT                   4


//! \name UC3-A3-Xplained LED to GPIO pin mappings.
// @{

#define LED0_GPIO                   (AVR32_PIN_PB03)
#define LED1_GPIO                   (AVR32_PIN_PX22)
#define LED2_GPIO                   (AVR32_PIN_PB02)
#define LED3_GPIO                   (AVR32_PIN_PB06)

// @}


//! \name GPIO Connections of Push Buttons
// @{

#define GPIO_PUSH_BUTTON_0          (AVR32_PIN_PB01)
#define GPIO_PUSH_BUTTON_0_PRESSED  (0)

// @}


//! \name SDRAM Definitions
// @{

//!< Part header file of used SDRAM(s).
#define SDRAM_PART_HDR              "mt48lc16m16a2tg7e/mt48lc16m16a2tg7e.h"

//!< SDRAM(s) data bus width (always 16 bits on UC3)
#define SDRAM_DBW                    16
// @}

//! \name USART connections to the UC3B board controller
// @{
#define USART                       (&AVR32_USART1)
#define USART_RXD_PIN               AVR32_USART1_RXD_0_0_PIN
#define USART_RXD_FUNCTION          AVR32_USART1_RXD_0_0_FUNCTION
#define USART_TXD_PIN               AVR32_USART1_TXD_0_0_PIN
#define USART_TXD_FUNCTION          AVR32_USART1_TXD_0_0_FUNCTION
#define USART_IRQ                   AVR32_USART1_IRQ
#define USART_IRQ_GROUP             AVR32_USART1_IRQ_GROUP
#define USART_SYSCLK                SYSCLK_USART1
// @}

/*! \name TWI Master connections to GPIO
 *
 * \todo
 * The TWIM pins that are mapped here should correspond with the TWIM
 * interfaces selected in the conf_board.h file.  In addition, the
 * default values below map TWIM the pins on the Xplaned J1 block.
 * Modify the mappings or add appropriate conditional compilation as
 * needed.
 *
 * @{
 */
#define TWIMS0_TWD_PIN              AVR32_TWIMS0_TWD_0_0_PIN
#define TWIMS0_TWD_FUNCTION         AVR32_TWIMS0_TWD_0_0_FUNCTION
#define TWIMS0_TWCK_PIN             AVR32_TWIMS0_TWCK_0_0_PIN
#define TWIMS0_TWCK_FUNCTION        AVR32_TWIMS0_TWCK_0_0_FUNCTION
// @}

//!< Validate board support for the common sensor service.

#define                             COMMON_SENSOR_PLATFORM

/** @} */ // at32uc3a3_xpld_config group

#ifdef __cplusplus
}
#endif

#endif /* UC3_A3_XPLAINED_H_ */
