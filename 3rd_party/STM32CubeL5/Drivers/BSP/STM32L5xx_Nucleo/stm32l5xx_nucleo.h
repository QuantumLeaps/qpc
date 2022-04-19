/**
  ******************************************************************************
  * @file    stm32l5xx_nucleo.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for:
  *          - LEDs and push-button available on STM32L5XX-Nucleo Kit
  *            from STMicroelectronics
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32L5XX_NUCLEO_H
#define STM32L5XX_NUCLEO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l5xx_nucleo_conf.h"
#include "stm32l5xx_nucleo_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L5XX_NUCLEO
  * @{
  */

/** @addtogroup STM32L5XX_NUCLEO_COMMON
  * @{
  */

/** @defgroup STM32L5XX_NUCLEO_COMMON_Exported_Types STM32L5XX_NUCLEO COMMON Exported Types
  * @{
  */
typedef enum
{
  LED1 = 0,
  LED_GREEN = LED1,
  LED2 = 1,
  LED_BLUE = LED2,
  LED3 = 2,
  LED_RED = LED3
} Led_TypeDef;

typedef enum
{
  BUTTON_USER = 0
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

#if (USE_BSP_COM_FEATURE == 1)
typedef enum
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;

typedef enum
{
  COM_WORDLENGTH_7B = UART_WORDLENGTH_7B,
  COM_WORDLENGTH_8B = UART_WORDLENGTH_8B,
  COM_WORDLENGTH_9B = UART_WORDLENGTH_9B
} COM_WordLengthTypeDef;

typedef enum
{
  COM_STOPBITS_1 = UART_STOPBITS_1,
  COM_STOPBITS_2 = UART_STOPBITS_2
} COM_StopBitsTypeDef;

typedef enum
{
  COM_PARITY_NONE = UART_PARITY_NONE,
  COM_PARITY_EVEN = UART_PARITY_EVEN,
  COM_PARITY_ODD  = UART_PARITY_ODD
} COM_ParityTypeDef;

typedef enum
{
  COM_HWCONTROL_NONE    = UART_HWCONTROL_NONE,
  COM_HWCONTROL_RTS     = UART_HWCONTROL_RTS,
  COM_HWCONTROL_CTS     = UART_HWCONTROL_CTS,
  COM_HWCONTROL_RTS_CTS = UART_HWCONTROL_RTS_CTS
} COM_HwFlowCtlTypeDef;

typedef struct
{
  uint32_t              BaudRate;
  COM_WordLengthTypeDef WordLength;
  COM_StopBitsTypeDef   StopBits;
  COM_ParityTypeDef     Parity;
  COM_HwFlowCtlTypeDef  HwFlowCtl;
} COM_InitTypeDef;

#define MX_UART_InitTypeDef COM_InitTypeDef

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef struct
{
  pUART_CallbackTypeDef  pMspInitCb;
  pUART_CallbackTypeDef  pMspDeInitCb;
} BSP_COM_Cb_t;
#endif
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @}
  */

/** @defgroup STM32L5XX_NUCLEO_COMMON_Exported_Constants STM32L5XX_NUCLEO COMMON Exported Constants
  * @{
  */

/**
  * @brief STM32L5xx NUCLEO BSP Driver version number
  */
#define STM32L5xx_NUCLEO_BSP_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define STM32L5xx_NUCLEO_BSP_VERSION_SUB1   (0x00U) /*!< [23:16] sub1 version */
#define STM32L5xx_NUCLEO_BSP_VERSION_SUB2   (0x03U) /*!< [15:8]  sub2 version */
#define STM32L5xx_NUCLEO_BSP_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define STM32L5xx_NUCLEO_BSP_VERSION       ((STM32L5xx_NUCLEO_BSP_VERSION_MAIN << 24U)\
                                               |(STM32L5xx_NUCLEO_BSP_VERSION_SUB1 << 16U)\
                                               |(STM32L5xx_NUCLEO_BSP_VERSION_SUB2 << 8U )\
                                               |(STM32L5xx_NUCLEO_BSP_VERSION_RC))

#define STM32L5XX_NUCLEO_BSP_BOARD_NAME  "STM32L5XX-NUCLEO"
#define STM32L5XX_NUCLEO_BSP_BOARD_ID    "MB1361C"

/** @defgroup STM32L5XX_NUCLEO_COMMON_LED STM32L5XX_NUCLEO COMMON LED
  * @{
  */
#define LEDn                                    3U

#define LED1_GPIO_PORT                          GPIOC
#define LED1_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED1_PIN                                GPIO_PIN_7

#define LED2_GPIO_PORT                          GPIOB
#define LED2_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED2_PIN                                GPIO_PIN_7

#define LED3_GPIO_PORT                          GPIOA
#define LED3_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED3_PIN                                GPIO_PIN_9
/**
  * @}
  */

/** @defgroup STM32L5XX_NUCLEO_COMMON_BUTTON STM32L5XX_NUCLEO COMMON BUTTON
  * @{
  */
#define BUTTONn                                 1U

#define BUTTON_USER_GPIO_PORT                   GPIOC
#define BUTTON_USER_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUTTON_USER_PIN                         GPIO_PIN_13
#define BUTTON_USER_EXTI_LINE                   EXTI_LINE_13
#define BUTTON_USER_EXTI_IRQn                   EXTI13_IRQn
/**
  * @}
  */

#if (USE_BSP_COM_FEATURE == 1)
/** @defgroup STM32L5XX_NUCLEO_COMMON_COM STM32L5XX_NUCLEO COMMON COM
  * @{
  */
#define COMn                                    2U

#define COM1_UART                               LPUART1
#define COM1_CLK_ENABLE()                       __HAL_RCC_LPUART1_CLK_ENABLE()
#define COM1_CLK_DISABLE()                      __HAL_RCC_LPUART1_CLK_DISABLE()
#define COM1_TX_GPIO_PORT                       GPIOG
#define COM1_TX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOG_CLK_ENABLE()
#define COM1_TX_PIN                             GPIO_PIN_7
#define COM1_TX_AF                              GPIO_AF8_LPUART1
#define COM1_RX_GPIO_PORT                       GPIOG
#define COM1_RX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOG_CLK_ENABLE()
#define COM1_RX_PIN                             GPIO_PIN_8
#define COM1_RX_AF                              GPIO_AF8_LPUART1

#define COM2_UART                               USART3
#define COM2_CLK_ENABLE()                       __HAL_RCC_USART3_CLK_ENABLE()
#define COM2_CLK_DISABLE()                      __HAL_RCC_USART3_CLK_DISABLE()
#define COM2_TX_GPIO_PORT                       GPIOD
#define COM2_TX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOD_CLK_ENABLE()
#define COM2_TX_PIN                             GPIO_PIN_8
#define COM2_TX_AF                              GPIO_AF7_USART3
#define COM2_RX_GPIO_PORT                       GPIOD
#define COM2_RX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOD_CLK_ENABLE()
#define COM2_RX_PIN                             GPIO_PIN_9
#define COM2_RX_AF                              GPIO_AF7_USART3
/**
  * @}
  */
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @}
  */

/** @addtogroup STM32L5XX_NUCLEO_COMMON_Exported_Variables
  * @{
  */
#if (USE_BSP_COM_FEATURE == 1)
extern UART_HandleTypeDef hcom_uart[COMn];
#endif
/**
  * @}
  */

/** @addtogroup STM32L5XX_NUCLEO_COMMON_Exported_Functions
  * @{
  */
int32_t        BSP_GetVersion(void);
const uint8_t* BSP_GetBoardName(void);
const uint8_t* BSP_GetBoardID(void);

/** @addtogroup STM32L5XX_NUCLEO_COMMON_LED_Functions
  * @{
  */
int32_t BSP_LED_Init(Led_TypeDef Led);
int32_t BSP_LED_DeInit(Led_TypeDef Led);
int32_t BSP_LED_On(Led_TypeDef Led);
int32_t BSP_LED_Off(Led_TypeDef Led);
int32_t BSP_LED_Toggle(Led_TypeDef Led);
int32_t BSP_LED_GetState(Led_TypeDef Led);
/**
  * @}
  */

/** @addtogroup STM32L5XX_NUCLEO_COMMON_BUTTON_Functions
  * @{
  */
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
int32_t BSP_PB_DeInit(Button_TypeDef Button);
int32_t BSP_PB_GetState(Button_TypeDef Button);
void    BSP_PB_Callback(Button_TypeDef Button);
void    BSP_PB_IRQHandler(Button_TypeDef Button);
/**
  * @}
  */

#if (USE_BSP_COM_FEATURE == 1)
/** @addtogroup STM32L5XX_NUCLEO_COMMON_COM_Functions
  * @{
  */
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);
int32_t BSP_COM_DeInit(COM_TypeDef COM);
#if (USE_COM_LOG == 1)
int32_t BSP_COM_SelectLogPort(COM_TypeDef COM);
#endif
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *CallBacks);
#endif
HAL_StatusTypeDef MX_LPUART1_Init(UART_HandleTypeDef* huart, MX_UART_InitTypeDef *MXInit);
HAL_StatusTypeDef MX_USART3_Init(UART_HandleTypeDef* huart, MX_UART_InitTypeDef *MXInit);
/**
  * @}
  */
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32L5XX_NUCLEO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
