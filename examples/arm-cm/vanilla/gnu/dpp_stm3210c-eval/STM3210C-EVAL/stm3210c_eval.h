/**
  ******************************************************************************
  * @file    stm3210c_eval.h
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   This file contains definitions for STM3210C_EVAL's Leds, push-buttons
  *          and COM ports hardware resources.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM3210C_EVAL_H
#define __STM3210C_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM3210C_EVAL
  * @{
  */


/** @defgroup STM3210C_EVAL_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup STM3210C_EVAL_Exported_Constants
  * @{
  */

/** @addtogroup STM3210C_EVAL_LED
  * @{
  */
#define LEDn                        4
#define LED1_GPIO_PORT              GPIOD
#define LED1_GPIO_CLK               RCC_APB2Periph_GPIOD
#define LED1_GPIO_PIN               GPIO_Pin_7

#define LED2_GPIO_PORT              GPIOD
#define LED2_GPIO_CLK               RCC_APB2Periph_GPIOD
#define LED2_GPIO_PIN               GPIO_Pin_13

#define LED3_GPIO_PORT              GPIOD
#define LED3_GPIO_CLK               RCC_APB2Periph_GPIOD
#define LED3_GPIO_PIN               GPIO_Pin_3

#define LED4_GPIO_PORT              GPIOD
#define LED4_GPIO_CLK               RCC_APB2Periph_GPIOD
#define LED4_GPIO_PIN               GPIO_Pin_4

/**
  * @}
  */

/** @addtogroup STM3210C_EVAL_BUTTON
  * @{
  */
#define BUTTONn                     3 /*!< Joystick pins are connected to an IO Expander (accessible through I2C1 interface) */

/**
 * @brief Wakeup push-button
 */
#define WAKEUP_BUTTON_PORT          GPIOA
#define WAKEUP_BUTTON_CLK           RCC_APB2Periph_GPIOA
#define WAKEUP_BUTTON_PIN           GPIO_Pin_0
#define WAKEUP_BUTTON_EXTI_LINE     EXTI_Line0
#define WAKEUP_BUTTON_PORT_SOURCE   GPIO_PortSourceGPIOA
#define WAKEUP_BUTTON_PIN_SOURCE    GPIO_PinSource0
#define WAKEUP_BUTTON_IRQn          EXTI0_IRQn

/**
 * @brief Tamper push-button
 */
#define TAMPER_BUTTON_PORT          GPIOC
#define TAMPER_BUTTON_CLK           RCC_APB2Periph_GPIOC
#define TAMPER_BUTTON_PIN           GPIO_Pin_13
#define TAMPER_BUTTON_EXTI_LINE     EXTI_Line13
#define TAMPER_BUTTON_PORT_SOURCE   GPIO_PortSourceGPIOC
#define TAMPER_BUTTON_PIN_SOURCE    GPIO_PinSource13
#define TAMPER_BUTTON_IRQn          EXTI15_10_IRQn

/**
 * @brief Key push-button
 */
#define KEY_BUTTON_PORT             GPIOB
#define KEY_BUTTON_CLK              RCC_APB2Periph_GPIOB
#define KEY_BUTTON_PIN              GPIO_Pin_9
#define KEY_BUTTON_EXTI_LINE        EXTI_Line9
#define KEY_BUTTON_PORT_SOURCE      GPIO_PortSourceGPIOB
#define KEY_BUTTON_PIN_SOURCE       GPIO_PinSource9
#define KEY_BUTTON_IRQn             EXTI9_5_IRQn
/**
  * @}
  */

/** @addtogroup STM3210C_EVAL_COM
  * @{
  */
#define COMn                        1

/**
 * @brief Definition for COM port1, connected to USART2 (USART2 pins remapped on GPIOD)
 */
#define EVAL_COM1                   USART2
#define EVAL_COM1_GPIO              GPIOD
#define EVAL_COM1_CLK               RCC_APB1Periph_USART2
#define EVAL_COM1_GPIO_CLK          RCC_APB2Periph_GPIOD
#define EVAL_COM1_RxPin             GPIO_Pin_6
#define EVAL_COM1_TxPin             GPIO_Pin_5

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM3210C_EVAL_Exported_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup STM3210C_EVAL_Exported_Functions
  * @{
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM3210C_EVAL_H */
/**
  * @}
  */


/**
  * @}
  */


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
