/*****************************************************************************
* Device driver for the Sharp Memory LCD LS013B7DH03
* Adapted and simplified from the file: displayls013b7dh03.c
* by Quantum Leaps, LLC
* 2016-05-12
*/
/*****************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *****************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "em_gpio.h"
#include "emstatus.h"

/* DISPLAY driver includes... */
#include "displaypalconfig.h"
#include "displaypal.h"
#include "display_ls013b7dh03.h"
#include "display_ls013b7dh03config.h"

/*******************************************************************************
 ********************************  DEFINES  ************************************
 ******************************************************************************/

/* LS013B7DH03 SPI commands */
#define LS013B7DH03_CMD_UPDATE     (0x01)
#define LS013B7DH03_CMD_ALL_CLEAR  (0x04)

/* Frequency of LCD polarity inversion. */
#define LS013B7DH03_POLARITY_INVERSION_FREQUENCY (128)


/*******************************************************************************
 **************************     GLOBAL FUNCTIONS      **************************
 ******************************************************************************/

/**************************************************************************//**
 * @brief  Initialize the LS013B7DH03 display driver
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
bool Display_init(void) {
    /* Initialize the Platform Abstraction Layer (PAL) interface.  */
    PAL_TimerInit();
    PAL_SpiInit();
    PAL_GpioInit();

    /* Setup GPIOs */
    PAL_GpioPinModeSet(LCD_PORT_SCLK, LCD_PIN_SCLK, palGpioModePushPull, 0);
    PAL_GpioPinModeSet(LCD_PORT_SI,   LCD_PIN_SI,   palGpioModePushPull, 0);
    PAL_GpioPinModeSet(LCD_PORT_SCS,  LCD_PIN_SCS,  palGpioModePushPull, 0);

    PAL_GpioPinModeSet(LCD_PORT_DISP_PWR,LCD_PIN_DISP_PWR,palGpioModePushPull,0);

    PAL_GpioPinModeSet(LCD_PORT_EXTCOMIN,LCD_PIN_EXTCOMIN,palGpioModePushPull,0);

    /* Setup system (via PAL) to toggle the EXTCOMIN pin every second. */
    if (PAL_GpioPinAutoToggle(LCD_PORT_EXTCOMIN, LCD_PIN_EXTCOMIN,
                              LS013B7DH03_POLARITY_INVERSION_FREQUENCY)
        != PAL_EMSTATUS_OK)
    {
        return false;
    }

    Display_enable(true);
    Display_clear();

    return true;
}

/**************************************************************************//**
 * @brief  Refresh the LS013B7DH03 display device.
 *
 * @param[in] device  Display device pointer.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
void Display_refresh(void) {
    /* Reinitialize the timer and SPI configuration.  */
    PAL_TimerInit();
    PAL_SpiInit();
}


/**************************************************************************//**
 * @brief  Enable or disable the display.
 *
 * @detail  This function enables or disables the display. Disabling the
 *          display does not make it lose it's data.
 *
 * @param[in] device  Display device pointer.
 *
 * @param[in] enable  Set this parameter to 'true' to enable the display.
 *                    Set to 'false' in order to disable the display.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
void Display_enable(bool enable) {
  if (enable) {
    /* Drive voltage on EFM_DISP_PWR_EN pin. */
    PAL_GpioPinOutSet(LCD_PORT_DISP_PWR, LCD_PIN_DISP_PWR);
  }
  else
  {
    /* Stop driving voltage on EFM_DISP_PWR_EN pin. */
    PAL_GpioPinOutClear(LCD_PORT_DISP_PWR, LCD_PIN_DISP_PWR);
  }
}


/**************************************************************************//**
 * @brief  Clear the display.
 *
 * @detail  This function clears the display.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
void Display_clear(void) {
  uint16_t cmd;

  /* Set SCS */
  PAL_GpioPinOutSet(LCD_PORT_SCS, LCD_PIN_SCS);

  /* SCS setup time: min 6us */
  PAL_TimerMicroSecondsDelay(6);

  /* Send command */
  cmd = LS013B7DH03_CMD_ALL_CLEAR;
  PAL_SpiTransmit((uint8_t *)&cmd, 2);

  /* SCS hold time: min 2us */
  PAL_TimerMicroSecondsDelay(2);

  /* Clear SCS */
  PAL_GpioPinOutClear( LCD_PORT_SCS, LCD_PIN_SCS );
}

/****************************************************************************/
/* send horizontal pixel array to the display.
* The pixel array has the full width of the display
* and height [in pixels] specified by 'h'
*
* @par pa  - pointer to the pixel array
* @par y   - start row
* @par h   - height of the pixel array
*/
void Display_sendPA(uint32_t const *pa,
                    uint8_t y, uint8_t h)
{
    uint32_t i;
    uint8_t const *p = (uint8_t const *)pa;
    uint16_t cmd;

    /* Need to adjust start row by one because LS013B7DH03 starts
    * counting lines from 1, while the DISPLAY interface starts from 0.
    */
    ++y;

    /* Assert SCS */
    PAL_GpioPinOutSet( LCD_PORT_SCS, LCD_PIN_SCS );

    /* SCS setup time: min 6us */
    PAL_TimerMicroSecondsDelay(6);

    /* Send update command and first line address */
    cmd = LS013B7DH03_CMD_UPDATE | (y << 8);
    PAL_SpiTransmit((uint8_t *)&cmd, 2);

    /* send pixels for all lines except the last... */
    for (i = 0; i < h - 1; ++i) {
        PAL_SpiTransmit((uint8_t *)p, LS013B7DH03_WIDTH/8);
        p += (LS013B7DH03_WIDTH/8);
        cmd = 0xFFU | ((y + i + 1) << 8);
        PAL_SpiTransmit((uint8_t *)&cmd, 2);
    }
    /* send pixels for the last line */
    PAL_SpiTransmit((uint8_t *)p, LS013B7DH03_WIDTH/8);
    cmd = 0xFFFFU;
    PAL_SpiTransmit((uint8_t *)&cmd, 2);

    /* SCS hold time: min 2us */
    PAL_TimerMicroSecondsDelay(2);

    /* De-assert SCS */
    PAL_GpioPinOutClear( LCD_PORT_SCS, LCD_PIN_SCS );
}

/** @endcond */
