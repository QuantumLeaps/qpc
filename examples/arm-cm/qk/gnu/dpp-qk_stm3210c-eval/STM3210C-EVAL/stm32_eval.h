/**
  ******************************************************************************
  * @file    stm32_eval.h
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   Header file for stm32_eval.c module.
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
#ifndef __STM32_EVAL_H
#define __STM32_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */


/** @defgroup STM32_EVAL_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32_EVAL_Exported_Constants
  * @{
  */

/**
  * @brief  Uncomment the line corresponding to the STMicroelectronics evaluation
  *   board used in your application.
  *
  *  Tip: To avoid modifying this file each time you need to switch between these
  *       boards, you can define the board in your toolchain compiler preprocessor.
  */
#if !defined (USE_STM3210B_EVAL) &&  !defined (USE_STM3210E_EVAL) &&  !defined (USE_STM3210C_EVAL)
 //#define USE_STM3210B_EVAL
 //#define USE_STM3210E_EVAL
 //#define USE_STM3210C_EVAL
#endif

#ifdef USE_STM3210B_EVAL
 #include "stm3210b_eval.h"
#elif defined USE_STM3210E_EVAL
 #include "stm3210e_eval.h"
#elif defined USE_STM3210C_EVAL
 #include "stm3210c_eval.h"
#else
 #error "Please select first the STM3210X_EVAL board to be used (in stm32_eval.h)"
#endif


typedef enum
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
} Led_TypeDef;

typedef enum
{
  Button_WAKEUP = 0,
  Button_TAMPER = 1,
  Button_KEY = 2,
  Button_RIGHT = 3,
  Button_LEFT = 4,
  Button_UP = 5,
  Button_DOWN = 6,
  Button_SEL = 7
} Button_TypeDef;

typedef enum
{
  Mode_GPIO = 0,
  Mode_EXTI = 1
} Button_Mode_TypeDef;

typedef enum
{
  JOY_NONE = 0,
  JOY_CENTER = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOY_State_TypeDef;

typedef enum
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;

/**
  * @}
  */

/** @defgroup STM32_EVAL_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32_EVAL_Exported_Functions
  * @{
  */
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(Button_TypeDef Button, Button_Mode_TypeDef Button_Mode);
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button);
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);

#ifdef __cplusplus
}
#endif


#endif /* __STM32_EVAL_H */
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

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
